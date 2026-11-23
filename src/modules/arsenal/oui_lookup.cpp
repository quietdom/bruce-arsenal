#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <WiFi.h>
#include <globals.h>

static const char *OUI_FILE = "/arsenal/oui.txt";


String oui_lookup_vendor(uint8_t *mac) {
    if (!setupSdCard()) return "No SD";

    char prefix[7];
    snprintf(prefix, sizeof(prefix), "%02X%02X%02X", mac[0], mac[1], mac[2]);

    File f = SD.open(OUI_FILE, FILE_READ);
    if (!f) return "No DB";

    String result = "Unknown";
    String line;

    while (f.available()) {
        line = f.readStringUntil('\n');
        line.trim();
        if (line.length() < 7) continue;


        if (line.substring(0, 6).equalsIgnoreCase(String(prefix))) {

            int eqPos = line.indexOf('=');
            if (eqPos > 0) {
                result = line.substring(eqPos + 1);
            }
            break;
        }
    }
    f.close();
    return result;
}


void arsenal_oui_lookup(void) {
    ARSENAL_SAFE_RUN([]() {
        if (!setupSdCard()) {
            displayRedStripe("SD card required!", TFT_WHITE, TFT_RED);
            delay(1500);
            return;
        }

        if (!SD.exists(OUI_FILE)) {
            drawMainBorderWithTitle("OUI Lookup");
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.setTextSize(FP);
            int y = 50;
            tft.setCursor(12, y);
            tft.print("OUI database not found!");
            y += 16;
            tft.setCursor(12, y);
            tft.print("Place oui.txt at:");
            y += 14;
            tft.setCursor(12, y);
            tft.print("/arsenal/oui.txt");
            y += 20;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(12, y);
            tft.print("Format: AABBCC=Vendor");
            y += 14;
            tft.setCursor(12, y);
            tft.print("One entry per line");

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Press any key"), tftWidth / 2, tftHeight - 20, 1);

            while (!check(EscPress) && !check(SelPress)) delay(100);
            returnToMenu = true;
            return;
        }


        drawMainBorderWithTitle("OUI Lookup");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.drawCentreString(String("Scanning WiFi..."), tftWidth / 2, tftHeight / 2, 1);

        WiFi.mode(WIFI_STA);
        int n = WiFi.scanNetworks(false, true);

        options.clear();
        for (int i = 0; i < n && i < 20; i++) {
            uint8_t *bssid = WiFi.BSSID(i);
            String vendor = oui_lookup_vendor(bssid);
            char entry[64];
            snprintf(entry, sizeof(entry), "%s [%s]",
                     WiFi.SSID(i).substring(0, 16).c_str(),
                     vendor.substring(0, 14).c_str());
            options.push_back({String(entry), []() {}});
        }

        if (options.empty()) {
            options.push_back({"No networks found", []() {}});
        }

        addOptionToMainMenu();
        loopOptions(options, MENU_TYPE_SUBMENU, "OUI Results");
        WiFi.scanDelete();
    });
}
