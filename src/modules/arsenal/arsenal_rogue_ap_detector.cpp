#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

struct RogueAP {
    char ssid[33];
    uint8_t bssid[6];
    int8_t rssi;
    uint8_t channel;
};

#define MAX_ROGUE 16
static RogueAP rogueList[MAX_ROGUE];
static int rogueCount = 0;

static void roguePromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;

    uint8_t subtype = (frame[0] >> 4) & 0x0F;
    if (subtype != 0x08) return;

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

    for (int i = 0; i < rogueCount; i++) {
        if (strcmp(rogueList[i].ssid, ssid) == 0 &&
            memcmp(rogueList[i].bssid, bssid, 6) != 0) return;
    }

    for (int i = 0; i < rogueCount; i++) {
        if (strcmp(rogueList[i].ssid, ssid) == 0 && memcmp(rogueList[i].bssid, bssid, 6) == 0) return;
    }

    if (rogueCount < MAX_ROGUE) {
        strncpy(rogueList[rogueCount].ssid, ssid, 32);
        memcpy(rogueList[rogueCount].bssid, bssid, 6);
        rogueList[rogueCount].rssi = pkt->rx_ctrl.rssi;
        rogueCount++;
    }
}

void arsenal_rogue_ap_detector(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);

    int n = WiFi.scanNetworks(false, false);
    if (n == 0) {
        displayRedStripe("No networks found");
        delay(1500);
        return;
    }

    options.clear();
    for (int i = 0; i < n && i < 15; i++) {
        String label = WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + "dB)";
        options.push_back({label, [i]() {}});
    }
    int targetIdx = -1;
    for (int i = 0; i < n && i < 15; i++) {
        options[i].operation = [i, &targetIdx]() { targetIdx = i; };
    }
    loopOptions(options, MENU_TYPE_SUBMENU, "Monitor SSID");

    if (targetIdx < 0) return;

    String targetSSID = WiFi.SSID(targetIdx);
    uint8_t origBSSID[6];
    memcpy(origBSSID, WiFi.BSSID(targetIdx), 6);
    WiFi.scanDelete();

    memset(rogueList, 0, sizeof(rogueList));
    rogueCount = 0;

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(roguePromiscCb);

    unsigned long startTime = millis();
    int origRSSI = -100;

    while (true) {
        drawMainBorderWithTitle("Rogue AP Detect");
        int y = 40;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("SSID: %s", targetSSID.c_str());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Orig: %02X:%02X:%02X", origBSSID[0], origBSSID[1], origBSSID[2]);
        y += 14;
        tft.setCursor(12, y);
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.printf("Scanning: %lus", elapsed);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("BSSIDs seen: %d", rogueCount);
        y += 14;

        if (rogueCount > 1) {
            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.print("ROGUE AP DETECTED!");
            y += 14;
            for (int i = 0; i < rogueCount && i < 3; i++) {
                tft.setCursor(12, y);
                bool isOrig = (memcmp(rogueList[i].bssid, origBSSID, 6) == 0);
                tft.printf("%s %02X:%02X:%02X %d",
                    isOrig ? "ORIG" : "ROGUE",
                    rogueList[i].bssid[0], rogueList[i].bssid[1], rogueList[i].bssid[2],
                    (int)rogueList[i].rssi);
                y += 12;
            }
        } else {
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.print("No clones detected yet");
        }

        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        esp_task_wdt_reset();
        delay(200);
    }

    esp_wifi_set_promiscuous(false);
    if (bgWasRunning) arsenal_background_start();
}
