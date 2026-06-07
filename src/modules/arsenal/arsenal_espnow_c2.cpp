#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_now.h>
#include <globals.h>

static volatile bool c2Running = false;
static uint8_t c2Peers[8][6];
static int c2PeerCount = 0;
static volatile bool c2GotCmd = false;
static char c2LastCmd[128] = "";
static int c2CommandsSent = 0;

static void c2OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {}
static void c2OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len > 0 && len < 128) {
        memcpy(c2LastCmd, incomingData, len);
        c2LastCmd[len] = '\0';
        c2GotCmd = true;

        bool found = false;
        for (int i = 0; i < c2PeerCount; i++) {
            if (memcmp(c2Peers[i], mac, 6) == 0) { found = true; break; }
        }
        if (!found && c2PeerCount < 8) {
            memcpy(c2Peers[c2PeerCount], mac, 6);
            c2PeerCount++;
        }
    }
}

void arsenal_espnow_c2(void) {
    ARSENAL_HEAP_CHECK();
    c2PeerCount = 0;
    c2CommandsSent = 0;
    c2GotCmd = false;
    memset(c2Peers, 0, sizeof(c2Peers));

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        displayRedStripe("ESP-NOW init failed");
        delay(1500);
        return;
    }

    esp_now_register_send_cb(c2OnDataSent);
    esp_now_register_recv_cb(c2OnDataRecv);

    esp_now_peer_info_t peer = {};
    uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    memcpy(peer.peer_addr, broadcast, 6);
    peer.channel = 0;
    peer.encrypt = false;
    esp_now_add_peer(&peer);

    c2Running = true;
    unsigned long startTime = millis();

    while (c2Running) {
        if (c2GotCmd) {
            c2GotCmd = false;
        }

        drawMainBorderWithTitle("ESP-NOW C2");
        int y = 40;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Connected devices: %d", c2PeerCount);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Commands sent: %d", c2CommandsSent);
        y += 14;
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Uptime: %lus", elapsed);
        y += 16;

        for (int i = 0; i < c2PeerCount; i++) {
            tft.setCursor(12, y);
            tft.printf("Device %d: %02X:%02X:%02X:%02X:%02X:%02X",
                i + 1, c2Peers[i][0], c2Peers[i][1], c2Peers[i][2],
                c2Peers[i][3], c2Peers[i][4], c2Peers[i][5]);
            y += 12;
        }

        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop  Sel:cmd", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        if (check(SelPress)) {
            while (check(SelPress)) delay(10);
            char cmd[128] = "";
            Serial.println("[C2] Type command to broadcast:");
            unsigned long start = millis();
            while (millis() - start < 15000) {
                if (Serial.available()) {
                    char c = Serial.read();
                    if (c == '\n' || c == '\r') break;
                    int len = strlen(cmd);
                    if (len < 127) { cmd[len] = c; cmd[len+1] = '\0'; }
                }
                if (check(EscPress)) break;
                delay(10);
            }
            if (strlen(cmd) > 0) {
                esp_now_send(broadcast, (uint8_t *)cmd, strlen(cmd));
                c2CommandsSent++;
                drawMainBorderWithTitle("ESP-NOW C2");
                tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                tft.setTextSize(FP);
                tft.setCursor(12, 50);
                tft.printf("Sent: %s", cmd);
                delay(1000);
            }
        }

        esp_task_wdt_reset();
        delay(100);
    }

    esp_now_unregister_send_cb();
    esp_now_unregister_recv_cb();
    esp_now_deinit();
}
