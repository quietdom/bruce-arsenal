#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <globals.h>

static char nfcURL[128] = "https://github.com/quietdom";

static void loadNfcConfig() {
    if (!setupSdCard()) return;
    if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
    if (SD.exists("/arsenal/nfc_card.txt")) {
        File f = SD.open("/arsenal/nfc_card.txt", FILE_READ);
        if (f) {
            String line = f.readStringUntil('\n');
            line.trim();
            if (line.length() > 0 && line.length() < 128) {
                strncpy(nfcURL, line.c_str(), 127);
                nfcURL[127] = '\0';
            }
            f.close();
        }
    }
}

static void saveNfcConfig() {
    if (!setupSdCard()) return;
    File f = SD.open("/arsenal/nfc_card.txt", FILE_WRITE);
    if (f) {
        f.println(nfcURL);
        f.close();
    }
}

void arsenal_nfc_business_card(void) {
    ARSENAL_HEAP_CHECK();
    loadNfcConfig();

    drawMainBorderWithTitle("NFC Biz Card");
    int y = 40;
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, y);
    tft.print("NFC tag emulation");
    y += 14;
    tft.setCursor(12, y);
    tft.printf("URL: %s", String(nfcURL).substring(0, 24).c_str());
    y += 14;
    tft.setCursor(12, y);
    tft.print("Requires PN532 module");
    y += 14;
    tft.setCursor(12, y);
    tft.print("Connected via I2C/SPI");
    y += 16;
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:done  Sel:edit", tftWidth / 2, tftHeight - 20, 1);

    while (true) {
        if (check(EscPress)) break;
        if (check(SelPress)) {
            while (check(SelPress)) delay(10);
            drawMainBorderWithTitle("NFC Biz Card");
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, 45);
            tft.print("Enter URL on serial:");
            tft.setCursor(12, 60);
            tft.printf("Current: %s", String(nfcURL).substring(0, 24).c_str());
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:cancel", tftWidth / 2, tftHeight - 20, 1);

            Serial.println("[NFC] Enter URL:");
            unsigned long start = millis();
            String input = "";
            while (millis() - start < 30000) {
                if (Serial.available()) {
                    char c = Serial.read();
                    if (c == '\n' || c == '\r') break;
                    input += c;
                }
                if (check(EscPress)) break;
                delay(10);
            }

            if (input.length() > 0 && input.length() < 128) {
                strncpy(nfcURL, input.c_str(), 127);
                nfcURL[127] = '\0';
                saveNfcConfig();
                displayRedStripe("URL updated!");
                delay(1000);
            }

            drawMainBorderWithTitle("NFC Biz Card");
            y = 40;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("URL: %s", String(nfcURL).substring(0, 24).c_str());
            y += 14;
            tft.setCursor(12, y);
            tft.print("Place tag near reader");
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:done  Sel:edit", tftWidth / 2, tftHeight - 20, 1);
        }
        delay(100);
    }
}
