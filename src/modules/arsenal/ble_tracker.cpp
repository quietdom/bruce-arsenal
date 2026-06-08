#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <NimBLEDevice.h>
#include <globals.h>

struct TrackedDevice {
    String address;
    String name;
    int rssi;
    int seenCount;
    unsigned long firstSeen;
    unsigned long lastSeen;
    bool isTracker;
    String type;
};

static std::vector<TrackedDevice> trackedDevices;
static NimBLEScan *bleScan = nullptr;
static bool trackerAlert = false;
static const int TRACKER_THRESHOLD = 5;


static bool isAirTag(NimBLEAdvertisedDevice &dev) {
    if (dev.haveManufacturerData()) {
        std::string mfgData = dev.getManufacturerData();

        if (mfgData.length() >= 2) {
            if ((uint8_t)mfgData[0] == 0x4C && (uint8_t)mfgData[1] == 0x00) {

                if (mfgData.length() > 2 && (uint8_t)mfgData[2] == 0x12) {
                    return true;
                }
            }
        }
    }
    return false;
}


static bool isSmartTag(NimBLEAdvertisedDevice &dev) {
    if (dev.haveManufacturerData()) {
        std::string mfgData = dev.getManufacturerData();

        if (mfgData.length() >= 2) {
            if ((uint8_t)mfgData[0] == 0x75 && (uint8_t)mfgData[1] == 0x00) {
                return true;
            }
        }
    }
    return false;
}


static bool isTile(NimBLEAdvertisedDevice &dev) {
    if (dev.haveServiceUUID()) {

        return dev.isAdvertisingService(NimBLEUUID("0000feed-0000-1000-8000-00805f9b34fb"));
    }
    return false;
}

static String identifyDevice(NimBLEAdvertisedDevice &dev) {
    if (isAirTag(dev)) return "AirTag";
    if (isSmartTag(dev)) return "SmartTag";
    if (isTile(dev)) return "Tile";
    return "";
}

class ArsenalBLECallbacks : public NimBLEScanCallbacks {
    void onResult(NimBLEAdvertisedDevice *advertisedDevice) {
        String addr = advertisedDevice->getAddress().toString().c_str();
        int rssi = advertisedDevice->getRSSI();
        String name = advertisedDevice->haveName() ? advertisedDevice->getName().c_str() : "";
        String type = identifyDevice(*advertisedDevice);


        bool found = false;
        for (auto &dev : trackedDevices) {
            if (dev.address == addr) {
                dev.rssi = rssi;
                dev.seenCount++;
                dev.lastSeen = millis();
                if (type.length() > 0) {
                    dev.isTracker = true;
                    dev.type = type;
                }
                if (dev.seenCount >= TRACKER_THRESHOLD && dev.isTracker) {
                    trackerAlert = true;
                }
                found = true;
                break;
            }
        }

        if (!found && trackedDevices.size() < 50) {
            TrackedDevice dev;
            dev.address = addr;
            dev.name = name;
            dev.rssi = rssi;
            dev.seenCount = 1;
            dev.firstSeen = millis();
            dev.lastSeen = millis();
            dev.isTracker = type.length() > 0;
            dev.type = type.length() > 0 ? type : "Device";
            trackedDevices.push_back(dev);
        }
    }
};

void arsenal_ble_tracker(void) {
    ARSENAL_SAFE_RUN([]() {
        trackedDevices.clear();
        trackerAlert = false;

        NimBLEDevice::deinit(true);
        NimBLEDevice::init("");
        bleScan = NimBLEDevice::getScan();
        bleScan->setScanCallbacks(new ArsenalBLECallbacks());
        bleScan->setActiveScan(true);
        bleScan->setInterval(100);
        bleScan->setWindow(99);

        int scanRound = 0;

        while (true) {

            bleScan->start(3, false);
            bleScan->clearResults();
            scanRound++;


            drawMainBorderWithTitle("BLE Tracker");
            int y = 38;
            int padX = 8;


            if (trackerAlert) {
                tft.fillRect(padX, y, tftWidth - 2 * padX, 18, TFT_RED);
                tft.setTextColor(TFT_WHITE, TFT_RED);
                tft.setTextSize(FP);
                tft.drawCentreString(String("! TRACKER DETECTED !"), tftWidth / 2, y + 3, 1);
                y += 22;
            }

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(padX, y);
            tft.printf("Devices: %d | Scan #%d", (int)trackedDevices.size(), scanRound);
            y += 14;


            std::vector<TrackedDevice *> sorted;
            for (auto &d : trackedDevices) sorted.push_back(&d);
            std::sort(sorted.begin(), sorted.end(), [](TrackedDevice *a, TrackedDevice *b) {
                return a->rssi > b->rssi;
            });

            int maxShow = min((int)sorted.size(), 6);
            for (int i = 0; i < maxShow; i++) {
                TrackedDevice *d = sorted[i];
                y += 2;


                uint16_t color = d->isTracker ? TFT_RED : bruceConfig.priColor;
                tft.setTextColor(color, bruceConfig.bgColor);


                int barWidth = map(constrain(d->rssi, -90, -30), -90, -30, 2, 40);
                tft.fillRect(padX, y, barWidth, 10, color);
                tft.fillRect(padX + barWidth, y, 40 - barWidth, 10, tft.color565(30, 30, 30));


                String info = d->type.substring(0, 8) + " " + String(d->rssi) + "dB x" + String(d->seenCount);
                tft.setCursor(padX + 44, y);
                tft.print(info.substring(0, 26));
                y += 13;
            }

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 18, 1);

            if (check(EscPress)) break;
            esp_task_wdt_reset();
        }

        bleScan->stop();
        NimBLEDevice::deinit(true);
        trackedDevices.clear();
    });
}
