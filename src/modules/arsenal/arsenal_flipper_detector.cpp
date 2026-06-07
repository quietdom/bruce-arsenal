#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

struct DetectedSignal {
    char type[16];
    char detail[48];
    int8_t rssi;
    unsigned long timestamp;
};

#define SIG_RING 16
static DetectedSignal sigRing[SIG_RING];
static int sigIdx = 0;
static int flipperCount = 0;

static void flipperPromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;

    const uint8_t *srcMac = frame + 10;
    if (srcMac[0] == 0x80 && srcMac[1] == 0 && srcMac[2] == 0 && srcMac[3] == 0 &&
        srcMac[4] == 0 && srcMac[5] == 0) return;

    bool detected = false;

    for (int i = 0; i < len - 20; i++) {
        if (frame[i] == 'F' && frame[i+1] == 'l' && frame[i+2] == 'i' &&
            frame[i+3] == 'p' && frame[i+4] == 'p' && frame[i+5] == 'e') {
            detected = true;
            break;
        }
    }

    if (!detected && len > 30) {
        if (frame[0] == 0x40 && frame[24] == 0x00 && frame[25] == 0x07) {
            if (len > 32 && frame[30] == 'F' && frame[31] == 'l') detected = true;
        }
    }

    if (detected) {
        DetectedSignal &s = sigRing[sigIdx];
        strcpy(s.type, "Flipper Zero");
        snprintf(s.detail, sizeof(s.detail), "%02X:%02X:%02X..",
            srcMac[0], srcMac[1], srcMac[2]);
        s.rssi = pkt->rx_ctrl.rssi;
        s.timestamp = millis();
        sigIdx = (sigIdx + 1) % SIG_RING;
        flipperCount++;
    }
}

void arsenal_flipper_detector(void) {
    ARSENAL_HEAP_CHECK();
    memset(sigRing, 0, sizeof(sigRing));
    sigIdx = 0;
    flipperCount = 0;

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(flipperPromiscCb);

    unsigned long startTime = millis();

    while (true) {
        drawMainBorderWithTitle("Flipper Detector");
        int y = 40;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Scanning... %lus", elapsed);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Flipper signals: %d", flipperCount);
        y += 14;

        if (flipperCount > 0) {
            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.print("FLIPPER ZERO DETECTED!");
            y += 14;
        }

        for (int off = 0; off < SIG_RING && off < 4; off++) {
            int i = (sigIdx - 1 - off + SIG_RING) % SIG_RING;
            if (sigRing[i].timestamp == 0) continue;
            tft.setCursor(12, y);
            tft.printf("[%s] %s %d", sigRing[i].type, sigRing[i].detail, (int)sigRing[i].rssi);
            y += 12;
        }

        if (flipperCount == 0) {
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.print("No Flipper signals detected");
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
