#include "arsenal.h"
#include "arsenal_background.h"
#include "arsenal_config.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>


#define PROBE_RING_SIZE 24
#define PROBE_LOG_PATH  "/arsenal/probes.log"

struct ProbeEntry {
    uint8_t mac[6];
    char ssid[33];
    int8_t rssi;
    bool used;
};

static ProbeEntry probeRing[PROBE_RING_SIZE];
static int probeIdx = 0;
static File probeLogFile;
static bool probeLogActive = false;
static bool probePromiscWasOn = false;
static bool probeBgWasRunning = false;


static bool macEquals(const uint8_t *a, const uint8_t *b) {
    for (int i = 0; i < 6; i++) if (a[i] != b[i]) return false;
    return true;
}


static int findProbe(const uint8_t *mac, const char *ssid) {
    for (int i = 0; i < PROBE_RING_SIZE; i++) {
        if (!probeRing[i].used) continue;
        if (macEquals(probeRing[i].mac, mac) && strcmp(probeRing[i].ssid, ssid) == 0) return i;
    }
    return -1;
}


static void extractSSID(const uint8_t *frame, int len, char *out, size_t cap) {
    out[0] = '\0';
    if (len < 26) return;
    int pos = 24;
    while (pos < len - 2) {
        uint8_t id = frame[pos];
        uint8_t sl = frame[pos + 1];
        if (id == 0x00 && sl > 0 && sl <= 32 && pos + 2 + sl <= len) {
            size_t n = min((size_t)sl, cap - 1);
            memcpy(out, frame + pos + 2, n);
            out[n] = '\0';
            return;
        }
        pos += 2 + sl;
    }
}


static void probePromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;

    uint8_t subtype = (frame[0] >> 4) & 0x0F;
    if (subtype != 0x04) return;

    const uint8_t *srcMac = frame + 10;
    char ssid[33];
    extractSSID(frame, len, ssid, sizeof(ssid));
    if (ssid[0] == '\0') return;

    if (findProbe(srcMac, ssid) >= 0) return;

    ProbeEntry &e = probeRing[probeIdx];
    memcpy(e.mac, srcMac, 6);
    strncpy(e.ssid, ssid, sizeof(e.ssid) - 1);
    e.ssid[sizeof(e.ssid) - 1] = '\0';
    e.rssi = pkt->rx_ctrl.rssi;
    e.used = true;
    probeIdx = (probeIdx + 1) % PROBE_RING_SIZE;

    if (probeLogActive && probeLogFile) {
        probeLogFile.printf("%lu,%02X:%02X:%02X:%02X:%02X:%02X,%d,%s\n",
            millis(), srcMac[0], srcMac[1], srcMac[2], srcMac[3], srcMac[4], srcMac[5],
            (int)e.rssi, ssid);
        probeLogFile.flush();
    }

    arsenal_stats_inc_probes();
    arsenal_stats_inc_devices();
}


void arsenal_wifi_probe_log(void) {
    ARSENAL_SAFE_RUN([]() {
        if (!setupSdCard()) {
            displayRedStripe("SD card required");
            delay(1500);
            return;
        }
        if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");

        memset(probeRing, 0, sizeof(probeRing));
        probeIdx = 0;
        probeLogFile = SD.open(PROBE_LOG_PATH, FILE_APPEND);
        probeLogActive = (bool)probeLogFile;
        probePromiscWasOn = false;

        if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);

        probeBgWasRunning = arsenal_background_is_running();
        if (probeBgWasRunning) arsenal_background_stop();
        esp_wifi_set_promiscuous(true);
        esp_wifi_set_promiscuous_rx_cb(probePromiscCb);
        if (probeLogFile) {
            probeLogFile.printf("\n# session start %lu\n", millis());
            probeLogFile.flush();
        }

        bool running = true;
        while (running) {
            drawMainBorderWithTitle("Probe Log");
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(8, 38);
            tft.printf("Logging: %s", probeLogActive ? "ON" : "OFF");
            tft.setCursor(8, 52);
            tft.printf("Unique: %d", PROBE_RING_SIZE);

            int y = 70;
            int shown = 0;
            for (int off = 0; off < PROBE_RING_SIZE && shown < 8; off++) {
                int i = (probeIdx - 1 - off + PROBE_RING_SIZE) % PROBE_RING_SIZE;
                if (!probeRing[i].used) continue;
                tft.setCursor(8, y);
                tft.printf("%02X:%02X..%s %d",
                    probeRing[i].mac[0], probeRing[i].mac[1],
                    probeRing[i].ssid, (int)probeRing[i].rssi);
                y += 12;
                shown++;
            }

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString("Esc=stop  Sel=clear", tftWidth / 2, tftHeight - 10, 1);

            if (check(EscPress)) {
                while (check(EscPress)) delay(10);
                running = false;
            } else if (check(SelPress)) {
                while (check(SelPress)) delay(10);
                memset(probeRing, 0, sizeof(probeRing));
                probeIdx = 0;
            }
            delay(60);
        }

        esp_wifi_set_promiscuous(false);
        if (probeLogFile) {
            probeLogFile.printf("# session end %lu\n", millis());
            probeLogFile.close();
        }
        probeLogActive = false;

        if (probeBgWasRunning) arsenal_background_start();
    });
}
