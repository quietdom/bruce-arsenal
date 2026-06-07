#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_now.h>
#include <globals.h>

static volatile bool chatRunning = false;
static char chatMessages[20][128];
static int chatMsgCount = 0;
static int chatMsgIdx = 0;
static uint8_t peerMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static volatile bool chatGotMessage = false;
static char lastMsg[128] = "";

static void chatOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {}
static void chatOnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len > 0 && len < 128) {
        memcpy(lastMsg, incomingData, len);
        lastMsg[len] = '\0';
        chatGotMessage = true;
    }
}

static void addChatMsg(const char *msg) {
    strncpy(chatMessages[chatMsgIdx], msg, 127);
    chatMessages[chatMsgIdx][127] = '\0';
    chatMsgIdx = (chatMsgIdx + 1) % 20;
    if (chatMsgCount < 20) chatMsgCount++;
}

void arsenal_espnow_chat(void) {
    ARSENAL_HEAP_CHECK();
    memset(chatMessages, 0, sizeof(chatMessages));
    chatMsgCount = 0;
    chatMsgIdx = 0;
    chatGotMessage = false;
    lastMsg[0] = '\0';

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        displayRedStripe("ESP-NOW init failed");
        delay(1500);
        return;
    }

    esp_now_register_send_cb(chatOnDataSent);
    esp_now_register_recv_cb(chatOnDataRecv);

    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, peerMAC, 6);
    peer.channel = 0;
    peer.encrypt = false;
    esp_now_add_peer(&peer);

    chatRunning = true;

    drawMainBorderWithTitle("ESP-NOW Chat");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.print("Listening for messages...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

    while (chatRunning) {
        if (chatGotMessage) {
            chatGotMessage = false;
            addChatMsg(lastMsg);
        }

        drawMainBorderWithTitle("ESP-NOW Chat");
        int y = 36;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);

        int start = chatMsgIdx - 1;
        int shown = 0;
        for (int off = 0; off < 20 && shown < 7; off++) {
            int i = (start - off + 20) % 20;
            if (chatMessages[i][0] == '\0') continue;
            tft.setCursor(12, y);
            String msg = String(chatMessages[i]).substring(0, 28);
            tft.print(msg);
            y += 12;
            shown++;
        }

        if (chatMsgCount == 0) {
            tft.setCursor(12, y + 10);
            tft.print("No messages yet");
        }

        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop  Sel:send", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        if (check(SelPress)) {
            while (check(SelPress)) delay(10);
            char input[128] = "";
            Serial.println("[Chat] Type message:");
            unsigned long start = millis();
            while (millis() - start < 15000) {
                if (Serial.available()) {
                    char c = Serial.read();
                    if (c == '\n' || c == '\r') break;
                    int len = strlen(input);
                    if (len < 127) { input[len] = c; input[len+1] = '\0'; }
                }
                if (check(EscPress)) break;
                delay(10);
            }
            if (strlen(input) > 0) {
                esp_now_send(peerMAC, (uint8_t *)input, strlen(input));
                char formatted[144];
                snprintf(formatted, sizeof(formatted), "Me: %s", input);
                addChatMsg(formatted);
            }
        }

        esp_task_wdt_reset();
        delay(100);
    }

    esp_now_unregister_send_cb();
    esp_now_unregister_recv_cb();
    esp_now_deinit();
}
