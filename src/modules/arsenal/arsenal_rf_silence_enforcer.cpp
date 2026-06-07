#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

static volatile bool rfSilenceRunning = false;
static volatile bool rfSilenceAlert = false;

static void silencePromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (!rfSilenceRunning) return;
    rfSilenceAlert = true;
}

void arsenal_rf_silence_enforcer(void) {
    ARSENAL_HEAP_CHECK();
    rfSilenceRunning = true;
    rfSilenceAlert = false;

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(silencePromiscCb);

    unsigned long startTime = millis();
    unsigned long lastClear = millis();
    int alertCount = 0;

    drawMainBorderWithTitle("RF Silence");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Monitoring RF activity...");
    tft.setCursor(12, 66);
    tft.print("Alert on any transmission");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop  Sel:clear", tftWidth / 2, tftHeight - 20, 1);
    delay(1000);

    while (true) {
        if (rfSilenceAlert) {
            rfSilenceAlert = false;
            alertCount++;

            tft.fillScreen(bruceConfig.bgColor);
            tft.drawRect(0, 0, tftWidth, tftHeight, TFT_RED);
            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.setTextSize(FM);
            tft.drawCentreString("RF DETECTED!", tftWidth / 2, tftHeight / 2 - 20, 1);
            tft.setTextSize(FP);
            tft.drawCentreString("Transmission detected!", tftWidth / 2, tftHeight / 2 + 5, 1);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.printf("Alerts: %d", alertCount);
            delay(1500);

            tft.fillScreen(bruceConfig.bgColor);
            drawMainBorderWithTitle("RF Silence");
        }

        drawMainBorderWithTitle("RF Silence");
        int y = 45;
        tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.print("STATUS: MONITORING");
        y += 14;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setCursor(12, y);
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.printf("Time: %lus", elapsed);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Alerts: %d", alertCount);
        y += 14;
        tft.setCursor(12, y);
        tft.print("Your devices should be");
        y += 12;
        tft.print("quiet during ops.");

        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop  Sel:clear", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        if (check(SelPress)) {
            while (check(SelPress)) delay(10);
            alertCount = 0;
            lastClear = millis();
        }

        esp_task_wdt_reset();
        delay(100);
    }

    rfSilenceRunning = false;
    esp_wifi_set_promiscuous(false);
    if (bgWasRunning) arsenal_background_start();
}
