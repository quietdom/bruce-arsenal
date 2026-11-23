#if !LITE_VERSION
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <NimBLEDevice.h>
#include <WiFi.h>
#include <globals.h>


static const char *SPAM_NAMES[] = {
    "FBI Surveillance Van #3",
    "NSA_PRISM_Node_7",
    "Putin's iPhone",
    "Area 51 Guest WiFi",
    "COVID-5G-Tower",
    "Your Mom's iPad",
    "Free Candy Van BT",
    "Hack Me If You Can",
    "Bill Gates Chip v2",
    "DROP TABLE devices;--",
    "Not A Virus Trust Me",
    "Illuminati HQ",
    "Skynet Core Node",
    "CIA Black Site #42",
    "Elon's Neuralink",
    "Hidden Camera #7",
    "Police Stingray",
    "DEA Monitoring",
    "Anonymous Member",
    "Witness Protection",
    "Time Traveler Phone",
    "Alien Mothership",
    "Matrix Backdoor",
    "Batcave Network",
    "Decepticon Relay",
    "HYDRA Base Alpha",
    "Bruce Was Here",
    "ur wifi is mine lol",
    "Hack In Progress...",
    "GET OUT OF MY SWAMP",
};
static const int NUM_NAMES = sizeof(SPAM_NAMES) / sizeof(SPAM_NAMES[0]);

void arsenal_bt_name_spammer(void) {
    ARSENAL_SAFE_RUN([]() {
        NimBLEDevice::deinit(true);
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        delay(100);
        if (!NimBLEDevice::init("")) {
            displayError("BLE init failed", true);
            return;
        }
        NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();

        NimBLEDevice::setSecurityAuth(false, false, false);

        int nameIndex = 0;
        int totalSent = 0;
        int totalFailed = 0;
        unsigned long startTime = millis();

        while (true) {

            NimBLEAdvertisementData advData;
            NimBLEAdvertisementData scanRespData;

            const char *name = SPAM_NAMES[nameIndex % NUM_NAMES];
            advData.setName(name);
            advData.setFlags(0x06);

            pAdvertising->setAdvertisementData(advData);
            pAdvertising->setScanResponseData(scanRespData);

            bool ok = pAdvertising->start();
            delay(50);
            pAdvertising->stop();

            if (ok) totalSent++; else totalFailed++;
            nameIndex++;


            if (totalSent % 5 == 0) {
                drawMainBorderWithTitle("Name Spammer");
                int y = 45;
                int padX = 12;

                tft.setTextColor(TFT_MAGENTA, bruceConfig.bgColor);
                tft.setTextSize(FP);
                tft.setCursor(padX, y);
                tft.print("SPAMMING BLE NAMES");
                y += 16;

                tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
                tft.setCursor(padX, y);
                tft.printf("Sent: %d  Failed: %d", totalSent, totalFailed);
                y += 14;

                unsigned long elapsed = (millis() - startTime) / 1000;
                tft.setCursor(padX, y);
                tft.printf("Elapsed: %lus", elapsed);
                y += 14;

                tft.setCursor(padX, y);
                tft.printf("Rate: ~%d/sec", totalSent / max(1UL, elapsed));
                y += 18;

                tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                tft.setCursor(padX, y);
                String currentName = String(name).substring(0, 24);
                tft.print(">" + currentName);

                tft.setTextColor(TFT_RED, bruceConfig.bgColor);
                tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 20, 1);
            }

            if (check(EscPress)) { returnToMenu = true; break; }
            esp_task_wdt_reset();
            delay(20);
        }

        pAdvertising->stop();
        NimBLEDevice::init("");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        pAdvertising = nullptr;
        vTaskDelay(100 / portTICK_PERIOD_MS);
        #if defined(CONFIG_IDF_TARGET_ESP32C5)
        esp_bt_controller_deinit();
        #else
        NimBLEDevice::deinit();
        #endif
    });
}
#endif