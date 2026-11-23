#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>


static uint8_t beacon_template[] = {
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
    'D', 'e', 'c', 'o', 'y', '_', '0', '0'
};

static uint8_t probe_template[] = {
    0x40, 0x00,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00,
    0x00, 0x06,
    'D', 'E', 'C', 'O', 'Y', '!'
};

static void randomMAC(uint8_t *dest) {
    for (int i = 0; i < 6; i++) dest[i] = random(256);
    dest[0] |= 0x02;
    dest[0] &= 0xFE;
}

static void sendDecoyBeacon() {
    randomMAC(beacon_template + 10);
    memcpy(beacon_template + 16, beacon_template + 10, 6);


    for (int i = 0; i < 8; i++) {
        beacon_template[36 + i] = 'A' + random(26);
    }

    esp_wifi_80211_tx(WIFI_IF_STA, beacon_template, sizeof(beacon_template), false);
}

static void sendDecoyProbe() {
    randomMAC(probe_template + 10);
    for (int i = 0; i < 6; i++) {
        probe_template[24 + i] = 'a' + random(26);
    }
    esp_wifi_80211_tx(WIFI_IF_STA, probe_template, sizeof(probe_template), false);
}

void arsenal_decoy_traffic(void) {
    ARSENAL_SAFE_RUN([]() {
        WiFi.mode(WIFI_STA);
        esp_wifi_set_promiscuous(true);

        int framesSent = 0;
        unsigned long startTime = millis();
        int channel = 1;
        int intensity = 5;

        while (true) {

            esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
            channel = (channel % 14) + 1;


            for (int i = 0; i < intensity; i++) {
                if (random(2) == 0) {
                    sendDecoyBeacon();
                } else {
                    sendDecoyProbe();
                }
                framesSent++;
            }


            if (framesSent % 20 == 0) {
                drawMainBorderWithTitle("Decoy Traffic");
                int y = 45;
                int padX = 12;

                tft.setTextColor(TFT_CYAN, bruceConfig.bgColor);
                tft.setTextSize(FP);
                tft.setCursor(padX, y);
                tft.print("GENERATING NOISE");
                y += 16;

                tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
                tft.setCursor(padX, y);
                tft.printf("Frames: %d", framesSent);
                y += 14;

                tft.setCursor(padX, y);
                tft.printf("Channel: %d", channel);
                y += 14;

                tft.setCursor(padX, y);
                tft.printf("Intensity: %d", intensity);
                y += 14;

                unsigned long elapsed = (millis() - startTime) / 1000;
                tft.setCursor(padX, y);
                tft.printf("Elapsed: %lus", elapsed);

                tft.setTextColor(TFT_RED, bruceConfig.bgColor);
                tft.drawCentreString(String("Esc:stop Up/Dn:intensity"), tftWidth / 2, tftHeight - 20, 1);
            }

            if (check(EscPress)) { returnToMenu = true; break; }
            if (check(UpPress) || check(NextPress)) {
                intensity += 2;
                if (intensity > 30) intensity = 30;
            }
            if (check(DownPress) || check(PrevPress)) {
                intensity -= 2;
                if (intensity < 1) intensity = 1;
            }

            esp_task_wdt_reset();
            delay(50);
        }

        esp_wifi_set_promiscuous(false);
    });
}
