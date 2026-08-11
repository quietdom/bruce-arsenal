#include "arsenal.h"

#if !LITE_VERSION

#include "core/display.h"
#include "core/mykeyboard.h"
#include "modules/rf/rf_utils.h"
#include "modules/rf/rf_send.h"
#include "modules/rf/rf_send.h"
#include <globals.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>

void arsenal_doorbell_replay(void) {
    ARSENAL_HEAP_CHECK();
    if (bruceConfigPins.rfModule != CC1101_SPI_MODULE) {
        displayRedStripe("CC1101 module not found");
        delay(1500);
        return;
    }

    RCSwitch rc = RCSwitch();

    drawMainBorderWithTitle("Doorbell Replay");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.print("Listening on 433.92MHz...");
    tft.setCursor(12, 60);
    tft.print("Press doorbell now");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

    if (!initRfModule("rx", 433.92)) {
        displayRedStripe("RF init failed");
        delay(1500);
        return;
    }

    ELECHOUSE_cc1101.SetRx();
    rc.enableReceive(bruceConfigPins.CC1101_bus.io0);

    int captured = 0;
    unsigned long startTime = millis();
    bool gotSignal = false;
    uint64_t lastCode = 0;

    while (!check(EscPress)) {
        if (rc.available()) {
            uint64_t val = rc.getReceivedValue();
            if (val != 0) {
                int rssi = ELECHOUSE_cc1101.getRssi();
                lastCode = val;
                captured++;
                gotSignal = true;
                drawMainBorderWithTitle("Doorbell Replay");
                tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                tft.setTextSize(FP);
                tft.setCursor(12, 50);
                tft.printf("Signal! Code: %08lX", (unsigned long)val);
                tft.setCursor(12, 66);
                tft.printf("RSSI: %d  Count: %d", rssi, captured);
                tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                tft.drawCentreString(String("Esc:stop & replay"), tftWidth / 2, tftHeight - 20, 1);
                rc.resetAvailable();
                delay(300);
            } else {
                rc.resetAvailable();
            }
        }
        esp_task_wdt_reset();
        delay(10);
    }

    returnToMenu = true;
    deinitRfModule();

    if (gotSignal) {
        options.clear();
        options.push_back({"Replay 5x", [lastCode]() {
            if (initRfModule("tx", 433.92)) rfTransmitCode(lastCode, 24, 350, 1, 50);
        }});
        options.push_back({"Replay 20x", [lastCode]() {
            if (initRfModule("tx", 433.92)) rfTransmitCode(lastCode, 24, 350, 1, 200);
        }});
        options.push_back({"Replay 100x", [lastCode]() {
            if (initRfModule("tx", 433.92)) rfTransmitCode(lastCode, 24, 350, 1, 1000);
        }});
        addOptionToMainMenu();
        loopOptions(options, MENU_TYPE_SUBMENU, "Replay");
    }
}

#endif
