#include "ble_common.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <NimBLEDevice.h>

struct BlePacket {
    String address;
    String advType;
    int rssi;
    String localName;
    uint8_t advData[31];
    int advLen;
};

static BLEScan *snifferScan = nullptr;
static BlePacket lastPackets[32];
static int packetCount = 0;
static int totalPackets = 0;
static bool snifferRunning = false;

class SnifferAdvertisedDeviceCallback : public NimBLEAdvertisedDeviceCallbacks {
    void onResult(NimBLEAdvertisedDevice *advertisedDevice) override {
        if (packetCount >= 32) {
            for (int i = 0; i < 31; i++) {
                lastPackets[i] = lastPackets[i + 1];
            }
            packetCount = 31;
        }

        BlePacket &p = lastPackets[packetCount];
        p.address = advertisedDevice->getAddress().toString().c_str();
        p.rssi = advertisedDevice->getRSSI();
        p.localName = advertisedDevice->haveName() ? advertisedDevice->getName().c_str() : "";

        if (advertisedDevice->haveAdvType()) {
            uint8_t advType = advertisedDevice->getAdvType();
            switch (advType) {
                case 0: p.advType = "ADV_IND"; break;
                case 1: p.advType = "ADV_DIRECT_IND"; break;
                case 2: p.advType = "ADV_NONCONN_IND"; break;
                case 3: p.advType = "ADV_SCAN_IND"; break;
                default: p.advType = "UNKNOWN"; break;
            }
        } else {
            p.advType = "N/A";
        }

        p.advLen = 0;
        if (advertisedDevice->haveAdvData()) {
            std::string data = advertisedDevice->getAdvData();
            p.advLen = data.length();
            if (p.advLen > 31) p.advLen = 31;
            memcpy(p.advData, data.data(), p.advLen);
        }

        totalPackets++;
        packetCount++;
    }
};

static SnifferAdvertisedDeviceCallback snifferCallback;

static void drawSniffer() {
    displayRedStripe("BLE Sniffer - ESC to stop");

    int y = 30;
    int padX = 5;

    tft.setTextSize(1);
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setCursor(padX, y);
    tft.printf("Packets: %d", totalPackets);
    y += 14;

    int startIdx = packetCount - 6;
    if (startIdx < 0) startIdx = 0;

    for (int i = startIdx; i < packetCount; i++) {
        BlePacket &p = lastPackets[i];

        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setCursor(padX, y);

        tft.printf("%s %s %ddBm", p.address.substring(0, 8).c_str(),
                   p.advType.c_str(), p.rssi);

        y += 12;

        if (p.localName.length() > 0) {
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setCursor(padX + 5, y);
            tft.printf("%s", p.localName.substring(0, 28).c_str());
            y += 12;
        }

        y += 2;

        if (y > tftHeight - 30) break;
    }
}

void ble_sniffer() {
    packetCount = 0;
    totalPackets = 0;
    snifferRunning = true;

    NimBLEDevice::init("");
    snifferScan = NimBLEDevice::getScan();
    snifferScan->setAdvertisedDeviceCallbacks(&snifferCallback);
    snifferScan->setActiveScan(false);
    snifferScan->setInterval(100);
    snifferScan->setWindow(99);

    snifferScan->start(0, nullptr, false);

    while (snifferRunning) {
        drawSniffer();

        if (checkEscPress()) {
            snifferRunning = false;
            break;
        }

        delay(500);
    }

    snifferScan->stop();
    snifferScan->clearAdvertisedDeviceCallbacks();
    NimBLEDevice::deinit();
    snifferScan = nullptr;
}
