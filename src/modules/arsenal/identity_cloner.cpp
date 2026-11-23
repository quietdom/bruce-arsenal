#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/utils.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

struct ScannedIdentity {
    uint8_t mac[6];
    String ssidProbe;
    int rssi;
    String vendor;
};

static std::vector<ScannedIdentity> identities;
static bool captureActive = false;


static void probeCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT || !captureActive) return;

    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;

    if (len < 24) return;


    uint8_t frameType = frame[0];
    if ((frameType & 0xFC) != 0x40) return;


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
                ssid += (char)frame[pos + 2 + i];
            }
        }
        pos += 2 + tagLen;
    }


    for (auto &id : identities) {
        if (memcmp(id.mac, srcMAC, 6) == 0) {
            id.rssi = pkt->rx_ctrl.rssi;
            if (ssid.length() > 0 && id.ssidProbe.length() == 0) {
                id.ssidProbe = ssid;
            }
            return;
        }
    }


    if (identities.size() < 30) {
        ScannedIdentity id;
        memcpy(id.mac, srcMAC, 6);
        id.ssidProbe = ssid;
        id.rssi = pkt->rx_ctrl.rssi;
        id.vendor = "";
        identities.push_back(id);
    }
}

static String macStr(uint8_t *mac) {
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buf);
}

void arsenal_identity_cloner(void) {
    ARSENAL_SAFE_RUN([]() {
        identities.clear();


        WiFi.mode(WIFI_STA);
        esp_wifi_set_promiscuous(true);
        esp_wifi_set_promiscuous_rx_cb(probeCallback);
        captureActive = true;

        drawMainBorderWithTitle("Identity Cloner");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.drawCentreString(String("Capturing probe requests..."), tftWidth / 2, tftHeight / 2 - 10, 1);
        tft.drawCentreString(String("Press Sel when ready"), tftWidth / 2, tftHeight / 2 + 10, 1);


        int ch = 1;
        while (true) {
            esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
            ch = (ch % 14) + 1;

            if (check(EscPress)) {
                returnToMenu = true;
                captureActive = false;
                esp_wifi_set_promiscuous(false);
                return;
            }
            if (check(SelPress)) break;


            tft.fillRect(12, tftHeight - 35, tftWidth - 24, 14, bruceConfig.bgColor);
            tft.setCursor(12, tftHeight - 35);
            tft.printf("Captured: %d identities", (int)identities.size());

            esp_task_wdt_reset();
            delay(100);
        }

        captureActive = false;
        esp_wifi_set_promiscuous(false);

        if (identities.empty()) {
            displayRedStripe("No identities captured");
            delay(1500);
            return;
        }


        options.clear();
        for (size_t i = 0; i < identities.size(); i++) {
            ScannedIdentity &id = identities[i];
            String label = macStr(id.mac).substring(0, 11) + " " +
                           String(id.rssi) + "dB";
            if (id.ssidProbe.length() > 0) {
                label += " [" + id.ssidProbe.substring(0, 8) + "]";
            }
            options.push_back({label, [i]() {

                ScannedIdentity &target = identities[i];
                esp_wifi_set_mac(WIFI_IF_STA, target.mac);

                drawMainBorderWithTitle("Identity Cloner");
                tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                tft.setTextSize(FP);
                int y = 50;
                tft.setCursor(12, y);
                tft.print("CLONED!");
                y += 16;
                tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
                tft.setCursor(12, y);
                tft.print("MAC: " + macStr(target.mac));
                y += 14;
                if (target.ssidProbe.length() > 0) {
                    tft.setCursor(12, y);
                    tft.print("Probe: " + target.ssidProbe);
                }
                y += 20;
                tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                tft.setCursor(12, y);
                tft.print("You are now invisible.");
                delay(3000);
            }});
        }

        addOptionToMainMenu();
        loopOptions(options, MENU_TYPE_SUBMENU, "Select Target");
    });
}
