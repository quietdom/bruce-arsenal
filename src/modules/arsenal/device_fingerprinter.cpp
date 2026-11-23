#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>


extern String oui_lookup_vendor(uint8_t *mac);

struct FingerprintedDevice {
    uint8_t mac[6];
    String vendor;
    String osGuess;
    int probeCount;
    int rssi;
    std::vector<String> probedSSIDs;
    unsigned long firstSeen;
    unsigned long lastSeen;
    bool randomizedMAC;
};

static std::vector<FingerprintedDevice> fingerprints;
static bool fpRunning = false;


static String guessOS(uint8_t *mac, const std::vector<String> &ssids, int probeInterval) {

    String vendor = oui_lookup_vendor(mac);
    vendor.toLowerCase();


    if (vendor.indexOf("apple") >= 0) {
        return "iOS/macOS";
    }


    if (vendor.indexOf("samsung") >= 0 || vendor.indexOf("xiaomi") >= 0 ||
        vendor.indexOf("huawei") >= 0 || vendor.indexOf("oppo") >= 0 ||
        vendor.indexOf("oneplus") >= 0 || vendor.indexOf("google") >= 0) {
        return "Android";
    }


    if (vendor.indexOf("intel") >= 0 || vendor.indexOf("microsoft") >= 0 ||
        vendor.indexOf("dell") >= 0 || vendor.indexOf("hp") >= 0 ||
        vendor.indexOf("lenovo") >= 0) {
        return "Windows";
    }


    if (mac[0] & 0x02) {


        if (ssids.size() <= 2) return "iOS (random)";

        if (ssids.size() > 5) return "Android (random)";
        return "Phone (random)";
    }


    if (vendor.indexOf("raspberry") >= 0 || vendor.indexOf("espressif") >= 0) {
        return "Linux/IoT";
    }

    return "Unknown";
}

static void fpProbeCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (!fpRunning || type != WIFI_PKT_MGMT) return;

    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;


    if ((frame[0] & 0xFC) != 0x40) return;

    uint8_t srcMAC[6];
    memcpy(srcMAC, frame + 10, 6);
    if (srcMAC[0] & 0x01) return;


    String ssid = "";
    int pos = 24;
    while (pos < len - 2) {
        uint8_t tagType = frame[pos];
        uint8_t tagLen = frame[pos + 1];
        if (pos + 2 + tagLen > len) break;
        if (tagType == 0 && tagLen > 0) {
            for (int i = 0; i < tagLen; i++) {
                char c = (char)frame[pos + 2 + i];
                if (c >= 32 && c <= 126) ssid += c;
            }
        }
        pos += 2 + tagLen;
    }


    FingerprintedDevice *dev = nullptr;
    for (auto &d : fingerprints) {
        if (memcmp(d.mac, srcMAC, 6) == 0) {
            dev = &d;
            break;
        }
    }

    if (!dev && fingerprints.size() < 40) {
        FingerprintedDevice newDev;
        memcpy(newDev.mac, srcMAC, 6);
        newDev.vendor = oui_lookup_vendor(srcMAC);
        newDev.osGuess = "";
        newDev.probeCount = 0;
        newDev.rssi = pkt->rx_ctrl.rssi;
        newDev.firstSeen = millis();
        newDev.lastSeen = millis();
        newDev.randomizedMAC = (srcMAC[0] & 0x02) != 0;
        fingerprints.push_back(newDev);
        dev = &fingerprints.back();
    }

    if (dev) {
        dev->probeCount++;
        dev->rssi = pkt->rx_ctrl.rssi;
        dev->lastSeen = millis();

        if (ssid.length() > 0) {
            bool exists = false;
            for (auto &s : dev->probedSSIDs) {
                if (s == ssid) { exists = true; break; }
            }
            if (!exists && dev->probedSSIDs.size() < 10) {
                dev->probedSSIDs.push_back(ssid);
            }
        }


        dev->osGuess = guessOS(dev->mac, dev->probedSSIDs, 0);
    }
}

static String macStr(uint8_t *mac) {
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buf);
}

void arsenal_device_fingerprinter(void) {
    ARSENAL_SAFE_RUN([]() {
        fingerprints.clear();
        fpRunning = true;

        WiFi.mode(WIFI_STA);
        esp_wifi_set_promiscuous(true);
        esp_wifi_set_promiscuous_rx_cb(fpProbeCallback);

        int ch = 1;
        unsigned long startTime = millis();

        while (true) {

            esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
            ch = (ch % 14) + 1;


            drawMainBorderWithTitle("Fingerprinter");
            int y = 36;
            int padX = 8;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);

            unsigned long elapsed = (millis() - startTime) / 1000;
            tft.setCursor(padX, y);
            tft.printf("Devices: %d | %lus | Ch:%d", (int)fingerprints.size(), elapsed, ch);
            y += 14;


            int maxShow = min((int)fingerprints.size(), 7);
            for (int i = 0; i < maxShow; i++) {
                FingerprintedDevice &d = fingerprints[i];
                y += 2;


                uint16_t color = bruceConfig.priColor;
                if (d.osGuess.indexOf("iOS") >= 0) color = TFT_WHITE;
                else if (d.osGuess.indexOf("Android") >= 0) color = TFT_GREEN;
                else if (d.osGuess.indexOf("Windows") >= 0) color = TFT_CYAN;

                tft.setTextColor(color, bruceConfig.bgColor);


                char line[40];
                snprintf(line, sizeof(line), "%s %s %ddB x%d",
                         macStr(d.mac).substring(9).c_str(),
                         d.osGuess.substring(0, 8).c_str(),
                         d.rssi, d.probeCount);
                tft.setCursor(padX, y);
                tft.print(line);
                y += 12;
            }

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc:stop Sel:details"), tftWidth / 2, tftHeight - 18, 1);

            if (check(EscPress)) { returnToMenu = true; break; }


            if (check(SelPress) && !fingerprints.empty()) {
                fpRunning = false;
                esp_wifi_set_promiscuous(false);

                options.clear();
                for (auto &d : fingerprints) {
                    String label = d.osGuess.substring(0, 10) + " " + macStr(d.mac).substring(9);
                    options.push_back({label, [&d]() {
                        drawMainBorderWithTitle("Device Details");
                        int dy = 40;
                        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
                        tft.setTextSize(FP);
                        tft.setCursor(8, dy); tft.print("MAC: " + macStr(d.mac)); dy += 14;
                        tft.setCursor(8, dy); tft.print("OS: " + d.osGuess); dy += 14;
                        tft.setCursor(8, dy); tft.print("Vendor: " + d.vendor.substring(0, 20)); dy += 14;
                        tft.setCursor(8, dy);                         tft.printf("RSSI: %ddB", d.rssi); dy += 14;
                        tft.setCursor(8, dy); tft.printf("Probes: %d", d.probeCount); dy += 14;
                        tft.setCursor(8, dy); tft.printf("Random MAC: %s", d.randomizedMAC ? "Yes" : "No"); dy += 14;
                        tft.setCursor(8, dy); tft.print("SSIDs:"); dy += 12;
                        for (size_t s = 0; s < min(d.probedSSIDs.size(), (size_t)3); s++) {
                            tft.setCursor(12, dy); tft.print("- " + d.probedSSIDs[s].substring(0, 20)); dy += 12;
                        }
                        while (!check(EscPress) && !check(SelPress)) delay(100);
                        returnToMenu = true;
                    }});
                }
                loopOptions(options, MENU_TYPE_SUBMENU, "Devices");


                fpRunning = true;
                esp_wifi_set_promiscuous(true);
                esp_wifi_set_promiscuous_rx_cb(fpProbeCallback);
            }

            esp_task_wdt_reset();
            delay(200);
        }

        fpRunning = false;
        esp_wifi_set_promiscuous(false);
    });
}
