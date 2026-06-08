#include "arsenal.h"

#if !LITE_VERSION

#include "core/display.h"
#include "core/mykeyboard.h"
#include <NimBLEDevice.h>
#include <globals.h>

static const uint8_t RICKROLL_AUDIO[] = {
    0x44, 0x03, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void arsenal_bt_audio_rickroll(void) {
    ARSENAL_HEAP_CHECK();

    drawMainBorderWithTitle("BT Rickroll");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.print("Scanning for audio devices...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc:cancel"), tftWidth / 2, tftHeight - 20, 1);

    NimBLEDevice::deinit(true);
    NimBLEDevice::init("Rick Astley");
    NimBLEScan *pScan = NimBLEDevice::getScan();
    pScan->setActiveScan(false);
    pScan->setInterval(100);
    pScan->setWindow(99);

    struct FoundDevice {
        String name;
        String address;
        int8_t rssi;
        bool isAudio;
    };
    FoundDevice devices[8];
    int devCount = 0;

    class RickScanCb : public NimBLEScanCallbacks {
    public:
        FoundDevice *list;
        int *count;
        RickScanCb(FoundDevice *d, int *c) : list(d), count(c) {}
        void onResult(NimBLEAdvertisedDevice *dev) {
            if (*count >= 8) return;
            String name = dev->getName().c_str();
            String addr = dev->getAddress().toString().c_str();
            int8_t rssi = dev->getRSSI();
            bool isAudio = false;
            String lower = name;
            lower.toLowerCase();
            if (lower.indexOf("speaker") >= 0 || lower.indexOf("audio") >= 0 ||
                lower.indexOf("headphone") >= 0 || lower.indexOf("earbuds") >= 0 ||
                lower.indexOf("airpod") >= 0 || lower.indexOf("buds") >= 0 ||
                lower.indexOf("jbl") >= 0 || lower.indexOf("sony") >= 0 ||
                lower.indexOf("bose") >= 0 || lower.indexOf("beats") >= 0)
                isAudio = true;

            if (dev->haveServiceUUID()) {
                if (dev->isAdvertisingService(NimBLEUUID((uint16_t)0x110B)) ||
                    dev->isAdvertisingService(NimBLEUUID((uint16_t)0x110A)))
                    isAudio = true;
            }

            for (int i = 0; i < *count; i++) {
                if (list[i].address == addr) return;
            }

            list[*count].name = name.length() > 0 ? name : "Unknown";
            list[*count].address = addr;
            list[*count].rssi = rssi;
            list[*count].isAudio = isAudio;
            (*count)++;
        }
    };

    RickScanCb cb(devices, &devCount);
    pScan->setScanCallbacks(&cb);
    pScan->start(5, false);
    pScan->clearResults();

    if (devCount == 0) {
        displayRedStripe("No devices found");
        NimBLEDevice::deinit(true);
        return;
    }

    options.clear();
    for (int i = 0; i < devCount; i++) {
        String label = devices[i].name + " " + String(devices[i].rssi) + "dB";
        if (devices[i].isAudio) label += " [AUDIO]";
        options.push_back({label, [i]() {}});
    }
    int targetIdx = -1;
    for (int i = 0; i < devCount; i++) {
        options[i].operation = [i, &targetIdx]() { targetIdx = i; };
    }
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Target Device");

    if (targetIdx < 0) {
        NimBLEDevice::deinit(true);
        return;
    }

    drawMainBorderWithTitle("BT Rickroll");
    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Target: %s", devices[targetIdx].name.c_str());
    tft.setCursor(12, 66);
    tft.print("Sending rickroll...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

    NimBLEAdvertising *pAdv = NimBLEDevice::getAdvertising();
    int count = 0;

    while (!check(EscPress)) {
        NimBLEDevice::getAdvertising()->stop();

        NimBLEAdvertisementData advData;
        std::string payload;
        for (int i = 0; i < 16; i++) payload += (char)RICKROLL_AUDIO[i];
        advData.setManufacturerData(payload);
        advData.setFlags(0x06);
        pAdv->setAdvertisementData(advData);
        pAdv->start();
        delay(80);
        pAdv->stop();
        count++;

        if (count % 10 == 0) {
            drawMainBorderWithTitle("BT Rickroll");
            int y = 50;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Target: %s", devices[targetIdx].name.c_str());
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Packets sent: %d", count);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);
        }
        esp_task_wdt_reset();
    }

    pAdv->stop();
    NimBLEDevice::deinit(true);
}

#endif
