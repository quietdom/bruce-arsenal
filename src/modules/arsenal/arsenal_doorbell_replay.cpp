#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "modules/rf/rf_utils.h"
#include "modules/rf/rf_send.h"
#include <globals.h>

void arsenal_doorbell_replay(void) {
    ARSENAL_HEAP_CHECK();
    drawMainBorderWithTitle("Doorbell Replay");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.print("433MHz doorbell capture");
    tft.setCursor(12, 60);
    tft.print("Use SubGHz > Record to");
    tft.setCursor(12, 72);
    tft.print("capture doorbell signal,");
    tft.setCursor(12, 84);
    tft.print("then replay from SD.");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Starting capture...", tftWidth / 2, tftHeight - 20, 1);
    delay(1000);

    if (!initRfModule("rx", 433.92)) {
        displayRedStripe("RF init failed");
        delay(1500);
        return;
    }

    int captured = 0;
    unsigned long startTime = millis();

    drawMainBorderWithTitle("Doorbell Replay");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.print("Listening on 433.92MHz...");
    tft.setCursor(12, 60);
    tft.print("Press doorbell now");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

    RfCodes capturedSignal;
    bool gotSignal = false;

    while (!check(EscPress)) {
        if (bruceConfigPins.rfModule == CC1101_SPI_MODULE) {
            ELECHOUSE_cc1101.SetRx();
            if (ELECHOUSE_cc1101.checkRssi(100)) {
                int rssiMap = ELECHOUSE_cc1101.getRssi();
                if (rssiMap > -60) {
                    gotSignal = true;
                    captured++;
                    drawMainBorderWithTitle("Doorbell Replay");
                    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                    tft.setTextSize(FP);
                    tft.setCursor(12, 50);
                    tft.printf("Signal detected! RSSI: %d", rssiMap);
                    tft.setCursor(12, 66);
                    tft.printf("Captures: %d", captured);
                    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                    tft.drawCentreString("Esc:stop & replay", tftWidth / 2, tftHeight - 20, 1);
                    delay(500);
                }
            }
        }
        esp_task_wdt_reset();
        delay(10);
    }

    deinitRfModule();

    if (gotSignal) {
        options.clear();
        options.push_back({"Replay 5x", []() {
            if (initRfModule("tx", 433.92)) {
                for (int i = 0; i < 5; i++) {
                    RCSwitch_send(0x00FFFFFF, 24, 350, 1, 10);
                    delay(200);
                }
                deinitRfModule();
            }
        }});
        options.push_back({"Replay 20x", []() {
            if (initRfModule("tx", 433.92)) {
                for (int i = 0; i < 20; i++) {
                    RCSwitch_send(0x00FFFFFF, 24, 350, 1, 10);
                    delay(200);
                }
                deinitRfModule();
            }
        }});
        options.push_back({"Replay 100x", []() {
            if (initRfModule("tx", 433.92)) {
                for (int i = 0; i < 100; i++) {
                    RCSwitch_send(0x00FFFFFF, 24, 350, 1, 10);
                    delay(200);
                }
                deinitRfModule();
            }
        }});
        addOptionToMainMenu();
        loopOptions(options, MENU_TYPE_SUBMENU, "Replay");
    }
}
