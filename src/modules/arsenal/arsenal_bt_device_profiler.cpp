#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <globals.h>

struct ProfiledDevice {
    String name;
    String address;
    int8_t rssi;
    String services[16];
    int serviceCount;
    String manufacturerData;
};

static ProfiledDevice profiledDevices[8];
static int profiledCount = 0;

class ProfilerCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice *dev) override {
        if (profiledCount >= 8) return;
        String addr = dev->getAddress().toString().c_str();
        for (int i = 0; i < profiledCount; i++) {
            if (profiledDevices[i].address == addr) return;
        }

        ProfiledDevice &d = profiledDevices[profiledCount];
        d.name = dev->getName().length() > 0 ? dev->getName().c_str() : "Unknown";
        d.address = addr;
        d.rssi = dev->getRSSI();
        d.serviceCount = 0;
        d.manufacturerData = "";

        if (dev->haveManufacturerData()) {
            String mfg = dev->getManufacturerData();
            char hex[8];
            for (int i = 0; i < (int)mfg.length() && d.serviceCount < 16; i++) {
                snprintf(hex, sizeof(hex), "%02X", (uint8_t)mfg[i]);
                d.manufacturerData += hex;
            }
        }

        if (dev->haveServiceUUID()) {
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x180F))) d.services[d.serviceCount++] = "Battery";
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x180A))) d.services[d.serviceCount++] = "DeviceInfo";
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x1800))) d.services[d.serviceCount++] = "GenericAccess";
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x1801))) d.services[d.serviceCount++] = "GenericAttr";
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x110B))) d.services[d.serviceCount++] = "A2DP-Sink";
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x110A))) d.services[d.serviceCount++] = "A2DP-Source";
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x110C))) d.services[d.serviceCount++] = "AVRCP";
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x111E))) d.services[d.serviceCount++] = "Handsfree";
            if (dev->isAdvertisingService(BLEUUID((uint16_t)0x1124))) d.services[d.serviceCount++] = "HID";
            if (dev->isAdvertisingService(BLEUUID("0000feed-0000-1000-8000-00805f9b34fb"))) d.services[d.serviceCount++] = "Tile";
        }

        if (d.serviceCount == 0 && d.manufacturerData.length() == 0) {
            d.services[d.serviceCount++] = "Adv-only";
        }

        profiledCount++;
    }
};

void arsenal_bt_device_profiler(void) {
    ARSENAL_HEAP_CHECK();
    memset(profiledDevices, 0, sizeof(profiledDevices));
    profiledCount = 0;

    drawMainBorderWithTitle("BT Profiler");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Scanning BLE devices...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

    BLEDevice::init("");
    BLEScan *pScan = BLEDevice::getScan();
    ProfilerCallbacks cb;
    pScan->setAdvertisedDeviceCallbacks(&cb, true);
    pScan->setActiveScan(true);
    pScan->setInterval(100);
    pScan->setWindow(99);
    pScan->start(8, false);
    pScan->clearResults();

    if (profiledCount == 0) {
        displayRedStripe("No BLE devices found");
        BLEDevice::deinit(false);
        return;
    }

    for (int i = 0; i < profiledCount; i++) {
        drawMainBorderWithTitle("BT Profiler");
        int y = 36;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("%d/%d: %s", i + 1, profiledCount, profiledDevices[i].name.c_str());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Addr: %s", profiledDevices[i].address.c_str());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("RSSI: %d dBm", (int)profiledDevices[i].rssi);
        y += 14;
        if (profiledDevices[i].serviceCount > 0) {
            tft.setCursor(12, y);
            tft.print("Services:");
            y += 12;
            for (int s = 0; s < profiledDevices[i].serviceCount; s++) {
                tft.setCursor(16, y);
                tft.printf("+ %s", profiledDevices[i].services[s].c_str());
                y += 11;
            }
        }
        if (profiledDevices[i].manufacturerData.length() > 0) {
            tft.setCursor(12, y);
            tft.printf("Mfg: %s", profiledDevices[i].manufacturerData.substring(0, 20).c_str());
            y += 12;
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop  Sel:next", tftWidth / 2, tftHeight - 20, 1);

        while (!check(EscPress) && !check(SelPress)) delay(100);
        if (check(EscPress)) break;
        while (check(SelPress)) delay(10);
    }

    BLEDevice::deinit(false);
}
