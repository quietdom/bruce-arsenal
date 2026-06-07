#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

#define HACK_RING 16

struct HackSignal {
    char device[24];
    char detail[48];
    int8_t rssi;
    unsigned long timestamp;
};

static HackSignal hackRing[HACK_RING];
static int hackIdx = 0;
static int hackCount = 0;

static void hackerPromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;

    const uint8_t *srcMac = frame + 10;
    char detected[24] = "";

    if ((srcMac[0] & 0xFE) == 0x80 && srcMac[1] == 0 && srcMac[2] == 0 &&
        srcMac[3] == 0 && srcMac[4] == 0 && srcMac[5] == 0) {
        return;
    }

    for (int i = 0; i < len - 10; i++) {
        if (frame[i] == 'F' && frame[i+1] == 'l' && frame[i+2] == 'i' &&
            frame[i+3] == 'p' && frame[i+4] == 'p' && frame[i+5] == 'e') {
            strcpy(detected, "Flipper Zero");
            break;
        }
        if (frame[i] == 'H' && frame[i+1] == 'a' && frame[i+2] == 'c' &&
            frame[i+3] == 'k' && frame[i+4] == 'R' && frame[i+5] == 'F') {
            strcpy(detected, "HackRF");
            break;
        }
        if (frame[i] == 'P' && frame[i+1] == 'i' && frame[i+2] == 'n' &&
            frame[i+3] == 'e' && frame[i+4] == 'a' && frame[i+5] == 'p') {
            strcpy(detected, "WiFi Pineapple");
            break;
        }
    }

    if (detected[0] != '\0') {
        HackSignal &s = hackRing[hackIdx];
        strcpy(s.device, detected);
        snprintf(s.detail, sizeof(s.detail), "%02X:%02X:%02X:%02X:%02X:%02X",
            srcMac[0], srcMac[1], srcMac[2], srcMac[3], srcMac[4], srcMac[5]);
        s.rssi = pkt->rx_ctrl.rssi;
        s.timestamp = millis();
        hackIdx = (hackIdx + 1) % HACK_RING;
        hackCount++;
    }
}

void arsenal_hacker_detector(void) {
    ARSENAL_HEAP_CHECK();
    memset(hackRing, 0, sizeof(hackRing));
    hackIdx = 0;
    hackCount = 0;

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(hackerPromiscCb);

    unsigned long startTime = millis();

    while (true) {
        drawMainBorderWithTitle("Hacker Detector");
        int y = 40;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Scanning... %lus", elapsed);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Threats detected: %d", hackCount);
        y += 14;

        if (hackCount > 0) {
            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.print("THREATS DETECTED!");
            y += 14;
        }

        for (int off = 0; off < HACK_RING && off < 4; off++) {
            int i = (hackIdx - 1 - off + HACK_RING) % HACK_RING;
            if (hackRing[i].timestamp == 0) continue;
            tft.setCursor(12, y);
            tft.printf("[%s] %d", hackRing[i].device, (int)hackRing[i].rssi);
            y += 12;
        }

        if (hackCount == 0) {
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.print("No pentest devices detected");
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
