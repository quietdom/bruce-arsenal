#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

static bool macRotatorRunning = false;
static unsigned long lastRotation = 0;
static unsigned long rotationInterval = 30000;
static uint8_t currentMAC[6];
static int rotationCount = 0;

static void generateRandomMAC(uint8_t *mac) {
    for (int i = 0; i < 6; i++) mac[i] = random(256);
    mac[0] |= 0x02;
    mac[0] &= 0xFE;
}

static void applyRandomMAC() {
    generateRandomMAC(currentMAC);
    esp_wifi_set_mac(WIFI_IF_STA, currentMAC);
    rotationCount++;
    lastRotation = millis();
}

static String macToString(uint8_t *mac) {
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buf);
}

void arsenal_mac_rotator(void) {
    ARSENAL_SAFE_RUN([]() {

        if (WiFi.getMode() == WIFI_MODE_NULL) {
            WiFi.mode(WIFI_STA);
        }

        macRotatorRunning = true;
        rotationCount = 0;
        applyRandomMAC();

        while (true) {
            drawMainBorderWithTitle("MAC Rotator");

            int padX = 12;
            int y = 45;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);

            tft.setCursor(padX, y);
            tft.print("Current MAC:");
            y += 14;

            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.print(macToString(currentMAC));
            y += 20;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.printf("Interval: %lus", rotationInterval / 1000);
            y += 14;

            tft.setCursor(padX, y);
            tft.printf("Rotations: %d", rotationCount);
            y += 14;

            unsigned long nextIn = rotationInterval - (millis() - lastRotation);
            if (nextIn > rotationInterval) nextIn = 0;
            tft.setCursor(padX, y);
            tft.printf("Next in: %lus", nextIn / 1000);

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to stop | Sel to rotate now"), tftWidth / 2, tftHeight - 20, 1);


            if (millis() - lastRotation >= rotationInterval) {
                applyRandomMAC();
            }


            if (check(EscPress)) { returnToMenu = true; break; }
            if (check(SelPress)) {
                applyRandomMAC();
            }


            if (check(UpPress) || check(NextPress)) {
                rotationInterval += 5000;
                if (rotationInterval > 300000) rotationInterval = 300000;
            }
            if (check(DownPress) || check(PrevPress)) {
                if (rotationInterval > 5000) rotationInterval -= 5000;
            }

            delay(500);
        }

        macRotatorRunning = false;
    });
}
