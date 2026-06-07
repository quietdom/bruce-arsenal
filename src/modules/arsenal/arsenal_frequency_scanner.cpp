#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "modules/rf/rf_utils.h"
#include <globals.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>

struct FreqPeak {
    float freq;
    int rssi;
};

void arsenal_frequency_scanner(void) {
    ARSENAL_HEAP_CHECK();
    drawMainBorderWithTitle("Freq Scanner");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.print("Sweeping 300-928 MHz...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
    delay(500);

    float sweepFreqs[] = {
        300.0, 303.9, 304.25, 307.0, 310.0, 312.1, 313.85, 314.35, 315.0, 318.0,
        330.0, 345.0, 348.0, 350.0, 387.0, 390.0, 418.0, 430.0, 431.5, 433.075,
        433.92, 434.075, 434.19, 434.39, 434.775, 440.175, 464.0, 467.75,
        779.0, 868.35, 868.4, 868.8, 868.95, 906.4, 915.0, 925.0, 928.0
    };
    int totalFreqs = sizeof(sweepFreqs) / sizeof(sweepFreqs[0]);

    FreqPeak peaks[10];
    int peakCount = 0;
    unsigned long startTime = millis();

    for (int f = 0; f < totalFreqs; f++) {
        if (check(EscPress)) break;

        deinitRfModule();
        delay(10);
        if (!initRfModule("rx", sweepFreqs[f])) continue;

        ELECHOUSE_cc1101.SetRx();
        delay(15);

        int rssi = ELECHOUSE_cc1101.getRssi();

        if (rssi > -60 && peakCount < 10) {
            peaks[peakCount].freq = sweepFreqs[f];
            peaks[peakCount].rssi = rssi;
            peakCount++;
        }

        drawMainBorderWithTitle("Freq Scanner");
        int y = 38;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Scanning: %.2f MHz", sweepFreqs[f]);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Progress: %d/%d", f + 1, totalFreqs);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Peaks found: %d", peakCount);
        y += 14;
        int barW = map(rssi, -100, 0, 0, tftWidth - 24);
        if (barW < 0) barW = 0;
        tft.fillRect(12, y, barW, 8, TFT_GREEN);
        tft.drawRect(12, y, tftWidth - 24, 8, bruceConfig.priColor);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("RSSI: %d dBm", rssi);
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

        esp_task_wdt_reset();
    }

    deinitRfModule();

    if (peakCount > 0) {
        drawMainBorderWithTitle("Freq Scanner");
        tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
        tft.setTextSize(FP);
        int y = 38;
        tft.setCursor(12, y);
        tft.printf("Found %d active freqs:", peakCount);
        y += 14;
        for (int i = 0; i < peakCount; i++) {
            tft.setCursor(12, y);
            tft.printf("%.2f MHz  %d dBm", peaks[i].freq, peaks[i].rssi);
            y += 12;
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress)) delay(100);
    }
}
