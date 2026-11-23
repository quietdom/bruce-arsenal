#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <globals.h>

static const char *COMMON_PINS[] = {
    "52045123", "95749738", "02211064", "75627092",
    "65435627", "98765432", "55555555", "12345678",
    "22334455", "11223344", "00000000", "11111111",
    "12344321", "31333788", "66666666", "87654321"
};
static const int NUM_PINS = sizeof(COMMON_PINS) / sizeof(COMMON_PINS[0]);

struct WpsTarget {
    String ssid;
    uint8_t bssid[6];
    int32_t rssi;
    uint8_t channel;
};

void arsenal_wps_pin_attack(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);

    drawMainBorderWithTitle("WPS PIN Reference");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Scanning for WPS...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

    int n = WiFi.scanNetworks(false, false);
    if (n == 0) {
        displayRedStripe("No networks found");
        delay(1500);
        return;
    }

    WpsTarget targets[15];
    int targetCount = 0;

    for (int i = 0; i < n && targetCount < 15; i++) {
        if (WiFi.SSID(i).length() == 0) continue;
        memcpy(targets[targetCount].bssid, WiFi.BSSID(i), 6);
        targets[targetCount].ssid = WiFi.SSID(i);
        targets[targetCount].rssi = WiFi.RSSI(i);
        targets[targetCount].channel = WiFi.channel(i);
        
        targetCount++;
    }

    WiFi.scanDelete();

    if (targetCount == 0) {
        displayRedStripe("No targets found");
        delay(1500);
        return;
    }

    options.clear();
    for (int i = 0; i < targetCount; i++) {
        String label = targets[i].ssid + " (" + String(targets[i].rssi) + "dB)";

        options.push_back({label, []() {}});
    }
    int selected = -1;
    for (int i = 0; i < targetCount; i++) {
        int idx = i;
        options[i].operation = [&selected, idx]() { selected = idx; };
    }
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Select Target");

    if (selected < 0) return;

    WpsTarget &tgt = targets[selected];

    drawMainBorderWithTitle("WPS PIN Reference");
    int y = 36;
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, y);
    tft.printf("Target: %s", tgt.ssid.c_str());
    y += 14;
    tft.setCursor(12, y);
    tft.printf("BSSID: %02X:%02X:%02X:%02X:%02X:%02X",
               tgt.bssid[0], tgt.bssid[1], tgt.bssid[2],
               tgt.bssid[3], tgt.bssid[4], tgt.bssid[5]);
    y += 14;
    tft.setCursor(12, y);
    tft.printf("RSSI: %d dB  Ch: %d", (int)tgt.rssi, tgt.channel);
    y += 20;
    tft.setTextColor(TFT_ORANGE, bruceConfig.bgColor);
    tft.print("Common WPS PINs:");
    y += 14;
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    for (int i = 0; i < NUM_PINS && i < 10; i++) {
        tft.setCursor(12, y);
        tft.printf("  %s", COMMON_PINS[i]);
        y += 12;
    }
    if (NUM_PINS > 10) {
        tft.setCursor(12, y);
        tft.printf("  ...and %d more", NUM_PINS - 10);
    }
    y += 16;
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.print("Try these PINs on target");
    tft.drawCentreString(String("Esc:done"), tftWidth / 2, tftHeight - 20, 1);

    while (!check(EscPress)) delay(100);
    returnToMenu = true;
}
