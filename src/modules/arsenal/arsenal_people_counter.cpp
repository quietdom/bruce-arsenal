#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

#define PEOPLE_RING_SIZE 64
#define PEOPLE_TIMEOUT_MS 300000

struct PeopleEntry {
    uint8_t mac[6];
    unsigned long lastSeen;
    bool used;
};

static PeopleEntry peopleRing[PEOPLE_RING_SIZE];
static int peopleIdx = 0;
static int uniqueCount = 0;
static unsigned long startTime = 0;

static void peoplePromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;

    uint8_t subtype = (frame[0] >> 4) & 0x0F;
    if (subtype != 0x04 && subtype != 0x00 && subtype != 0x08) return;

    const uint8_t *srcMac = frame + 10;
    unsigned long now = millis();

    for (int i = 0; i < PEOPLE_RING_SIZE; i++) {
        if (!peopleRing[i].used) continue;
        bool match = true;
        for (int j = 0; j < 6; j++) {
            if (peopleRing[i].mac[j] != srcMac[j]) { match = false; break; }
        }
        if (match) {
            peopleRing[i].lastSeen = now;
            return;
        }
    }

    PeopleEntry &e = peopleRing[peopleIdx];
    e.used = true;
    memcpy(e.mac, srcMac, 6);
    e.lastSeen = now;
    peopleIdx = (peopleIdx + 1) % PEOPLE_RING_SIZE;
    uniqueCount++;
}

void arsenal_people_counter(void) {
    ARSENAL_HEAP_CHECK();
    memset(peopleRing, 0, sizeof(peopleRing));
    peopleIdx = 0;
    uniqueCount = 0;
    startTime = millis();

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(peoplePromiscCb);

    while (true) {
        unsigned long now = millis();
        int activeCount = 0;
        for (int i = 0; i < PEOPLE_RING_SIZE; i++) {
            if (peopleRing[i].used && (now - peopleRing[i].lastSeen) < PEOPLE_TIMEOUT_MS) {
                activeCount++;
            }
        }

        drawMainBorderWithTitle("People Counter");
        int y = 40;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Active: %d", activeCount);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Total unique: %d", uniqueCount);
        y += 14;
        unsigned long elapsed = (now - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Time: %lus", elapsed);
        y += 14;
        tft.setCursor(12, y);
        if (elapsed > 0) {
            tft.printf("Rate: ~%.1f/min", (float)uniqueCount / elapsed * 60.0);
        }
        y += 16;
        int shown = 0;
        for (int off = 0; off < PEOPLE_RING_SIZE && shown < 4; off++) {
            int i = (peopleIdx - 1 - off + PEOPLE_RING_SIZE) % PEOPLE_RING_SIZE;
            if (!peopleRing[i].used) continue;
            bool active = (now - peopleRing[i].lastSeen) < PEOPLE_TIMEOUT_MS;
            tft.setCursor(12, y);
            tft.printf("%s %02X:%02X:%02X..", active ? "[A]" : "[ ]",
                peopleRing[i].mac[0], peopleRing[i].mac[1], peopleRing[i].mac[2]);
            y += 12;
            shown++;
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        esp_task_wdt_reset();
        delay(300);
    }

    esp_wifi_set_promiscuous(false);
    if (bgWasRunning) arsenal_background_start();
}
