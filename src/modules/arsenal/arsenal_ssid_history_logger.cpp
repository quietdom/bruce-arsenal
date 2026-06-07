#include "arsenal.h"
#include "arsenal_background.h"
#include "arsenal_config.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

#define SSID_RING_SIZE 32

struct SsidEntry {
    char ssid[33];
    uint8_t bssid[6];
    int8_t rssi;
    uint8_t channel;
    bool used;
};

static SsidEntry ssidRing[SSID_RING_SIZE];
static int ssidIdx = 0;
static int ssidUnique = 0;

static void ssidPromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;

    uint8_t subtype = (frame[0] >> 4) & 0x0F;
    if (subtype != 0x08 && subtype != 0x04) return;

    const uint8_t *bssid = frame + 16;
    char ssid[33] = {0};
    int pos = 24;
    while (pos < len - 2) {
        uint8_t id = frame[pos];
        uint8_t sl = frame[pos + 1];
        if (id == 0x00 && sl > 0 && sl <= 32 && pos + 2 + sl <= len) {
            memcpy(ssid, frame + pos + 2, sl);
            ssid[sl] = '\0';
            break;
        }
        pos += 2 + sl;
    }

    if (ssid[0] == '\0') return;

    for (int i = 0; i < SSID_RING_SIZE; i++) {
        if (ssidRing[i].used && strcmp(ssidRing[i].ssid, ssid) == 0 &&
            memcmp(ssidRing[i].bssid, bssid, 6) == 0) {
            ssidRing[i].rssi = pkt->rx_ctrl.rssi;
            return;
        }
    }

    SsidEntry &e = ssidRing[ssidIdx];
    e.used = true;
    strncpy(e.ssid, ssid, 32);
    memcpy(e.bssid, bssid, 6);
    e.rssi = pkt->rx_ctrl.rssi;
    e.channel = pkt->rx_ctrl.channel;
    ssidIdx = (ssidIdx + 1) % SSID_RING_SIZE;
    ssidUnique++;
}

void arsenal_ssid_history_logger(void) {
    ARSENAL_HEAP_CHECK();
    memset(ssidRing, 0, sizeof(ssidRing));
    ssidIdx = 0;
    ssidUnique = 0;

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(ssidPromiscCb);

    unsigned long startTime = millis();

    while (true) {
        drawMainBorderWithTitle("SSID History");
        int y = 38;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.printf("Unique SSIDs: %d  %lus", ssidUnique, elapsed);
        y += 14;
        int shown = 0;
        for (int off = 0; off < SSID_RING_SIZE && shown < 8; off++) {
            int i = (ssidIdx - 1 - off + SSID_RING_SIZE) % SSID_RING_SIZE;
            if (!ssidRing[i].used) continue;
            tft.setCursor(12, y);
            tft.printf("%s %d ch%d", ssidRing[i].ssid, (int)ssidRing[i].rssi, (int)ssidRing[i].channel);
            y += 12;
            shown++;
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop  Sel:save", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        if (check(SelPress)) {
            while (check(SelPress)) delay(10);
            if (setupSdCard()) {
                if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
                File f = SD.open("/arsenal/ssid_history.log", FILE_APPEND);
                if (f) {
                    f.printf("\n# session %lu\n", millis());
                    for (int i = 0; i < SSID_RING_SIZE; i++) {
                        if (!ssidRing[i].used) continue;
                        f.printf("%s,%02X:%02X:%02X:%02X:%02X:%02X,%d,%d\n",
                            ssidRing[i].ssid, ssidRing[i].bssid[0], ssidRing[i].bssid[1],
                            ssidRing[i].bssid[2], ssidRing[i].bssid[3], ssidRing[i].bssid[4],
                            ssidRing[i].bssid[5], (int)ssidRing[i].rssi, (int)ssidRing[i].channel);
                    }
                    f.close();
                }
                displayRedStripe("Saved to ssid_history.log");
                delay(1000);
            }
        }
        esp_task_wdt_reset();
        delay(100);
    }

    esp_wifi_set_promiscuous(false);
    if (bgWasRunning) arsenal_background_start();
}
