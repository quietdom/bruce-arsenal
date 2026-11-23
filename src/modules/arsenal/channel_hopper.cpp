#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

static const uint8_t HOP_PATTERN[] = {1, 6, 11, 3, 8, 13, 2, 7, 12, 4, 9, 14, 5, 10};
static const int HOP_PATTERN_LEN = sizeof(HOP_PATTERN);

void arsenal_channel_hopper(void) {
    ARSENAL_SAFE_RUN([]() {
        if (WiFi.getMode() == WIFI_MODE_NULL) {
            WiFi.mode(WIFI_STA);
            esp_wifi_set_promiscuous(true);
        }

        int hopIndex = 0;
        int hopCount = 0;
        unsigned long hopInterval = 200;
        unsigned long lastHop = 0;
        unsigned long startTime = millis();

        while (true) {
            unsigned long now = millis();

            if (now - lastHop >= hopInterval) {
                uint8_t ch = HOP_PATTERN[hopIndex % HOP_PATTERN_LEN];
                esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
                hopIndex++;
                hopCount++;
                lastHop = now;
            }


            if (hopCount % 5 == 0) {
                drawMainBorderWithTitle("Channel Hopper");
                int y = 45;
                int padX = 12;

                tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
                tft.setTextSize(FP);

                uint8_t currentCh = HOP_PATTERN[(hopIndex - 1) % HOP_PATTERN_LEN];

                tft.setCursor(padX, y);
                tft.printf("Current Channel: %d", currentCh);
                y += 16;

                tft.setCursor(padX, y);
                tft.printf("Hops: %d", hopCount);
                y += 14;

                tft.setCursor(padX, y);
                tft.printf("Interval: %lums", hopInterval);
                y += 14;

                unsigned long elapsed = (now - startTime) / 1000;
                tft.setCursor(padX, y);
                tft.printf("Elapsed: %lus", elapsed);
                y += 20;


                tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                tft.setCursor(padX, y);
                tft.print("Ch: ");
                for (int i = 0; i < 14; i++) {
                    if (i + 1 == currentCh) {
                        tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                    } else {
                        tft.setTextColor(tft.color565(60, 60, 60), bruceConfig.bgColor);
                    }
                    tft.printf("%d ", i + 1);
                }

                tft.setTextColor(TFT_RED, bruceConfig.bgColor);
                tft.drawCentreString(String("Esc:stop Up/Dn:speed"), tftWidth / 2, tftHeight - 20, 1);
            }

            if (check(EscPress)) { returnToMenu = true; break; }
            if (check(UpPress) || check(NextPress)) {
                if (hopInterval > 20) hopInterval -= 20;
            }
            if (check(DownPress) || check(PrevPress)) {
                hopInterval += 20;
                if (hopInterval > 2000) hopInterval = 2000;
            }

            esp_task_wdt_reset();
            delay(10);
        }
    });
}
