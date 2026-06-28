#include "ble_common.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <NimBLEDevice.h>

struct BlePacket {
    String address;
    int rssi;
    String localName;
};

static BlePacket lastPackets[32];
static int packetCount = 0;
static int totalPackets = 0;
static bool snifferRunning = false;

#ifdef NIMBLE_V2_PLUS
class SnifferScanCallback : public NimBLEScanCallbacks {
#else
class SnifferScanCallback : public NimBLEAdvertisedDeviceCallbacks {
#endif
    void onResult(NimBLEAdvertisedDevice *advertisedDevice) {
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

        totalPackets++;
        packetCount++;
    }
};

static SnifferScanCallback snifferCallback;

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

        tft.printf("%s %ddBm", p.address.substring(0, 8).c_str(), p.rssi);

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
    NimBLEScan *pScan = NimBLEDevice::getScan();
#ifdef NIMBLE_V2_PLUS
    pScan->setScanCallbacks(&snifferCallback);
#else
    pScan->setAdvertisedDeviceCallbacks(&snifferCallback);
#endif
    pScan->setActiveScan(false);
    pScan->setInterval(100);
    pScan->setWindow(99);

    pScan->start(0, nullptr, false);

    while (snifferRunning) {
        drawSniffer();

        if (check(EscPress)) {
            snifferRunning = false;
            break;
        }

        delay(500);
    }

    pScan->stop();
    NimBLEDevice::deinit(true);
}
