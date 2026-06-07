#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_now.h>
#include <globals.h>

struct SyncState {
    uint8_t attackId;
    uint8_t active;
    uint8_t channel;
    uint8_t reserved;
};

static volatile bool syncRunning = false;
static SyncState remoteState = {0, 0, 0, 0};
static SyncState localState = {0, 0, 0, 0};
static uint8_t syncPeer[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static volatile bool gotSync = false;

static void syncOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {}
static void syncOnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len == sizeof(SyncState)) {
        memcpy(&remoteState, incomingData, sizeof(SyncState));
        gotSync = true;
    }
}

void arsenal_multi_device_sync(void) {
    ARSENAL_HEAP_CHECK();
    memset(&localState, 0, sizeof(SyncState));
    memset(&remoteState, 0, sizeof(SyncState));
    gotSync = false;

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        displayRedStripe("ESP-NOW init failed");
        delay(1500);
        return;
    }

    esp_now_register_send_cb(syncOnDataSent);
    esp_now_register_recv_cb(syncOnDataRecv);

    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, syncPeer, 6);
    peer.channel = 0;
    peer.encrypt = false;
    esp_now_add_peer(&peer);

    syncRunning = true;
    unsigned long startTime = millis();
    unsigned long lastSend = 0;

    while (syncRunning) {
        unsigned long now = millis();

        if (now - lastSend > 1000) {
            localState.channel = WiFi.channel();
            esp_now_send(syncPeer, (uint8_t *)&localState, sizeof(SyncState));
            lastSend = now;
        }

        drawMainBorderWithTitle("Multi Sync");
        int y = 40;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Local state:");
        y += 14;
        tft.setCursor(16, y);
        tft.printf("Attack: %d  Active: %d", localState.attackId, localState.active);
        y += 14;
        tft.setCursor(16, y);
        tft.printf("Channel: %d", localState.channel);
        y += 16;
        tft.setCursor(12, y);
        tft.printf("Remote state:");
        y += 14;
        tft.setCursor(16, y);
        tft.printf("Attack: %d  Active: %d", remoteState.attackId, remoteState.active);
        y += 14;
        tft.setCursor(16, y);
        tft.printf("Channel: %d", remoteState.channel);
        y += 16;
        tft.setCursor(12, y);
        unsigned long elapsed = (now - startTime) / 1000;
        tft.printf("Synced: %lus", elapsed);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Connected: %s", gotSync ? "YES" : "NO");

        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop  Sel:toggle", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        if (check(SelPress)) {
            while (check(SelPress)) delay(10);
            localState.active = localState.active ? 0 : 1;
        }

        esp_task_wdt_reset();
        delay(100);
    }

    esp_now_unregister_send_cb();
    esp_now_unregister_recv_cb();
    esp_now_deinit();
}
