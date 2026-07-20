#if !defined(LITE_VERSION)
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

// Defensive counterpart to the deauther: passively sniffs for deauth and
// disassociation frames and reports who is sending them and who they target.
// Useful to tell a user they are being deauthed (e.g. by another Bruce/Flipper
// or a rogue AP) instead of just wondering why their wifi keeps dropping.

static volatile uint32_t dd_deauth_count = 0;
static volatile uint32_t dd_disassoc_count = 0;
static volatile uint32_t dd_broadcast_count = 0;
static char dd_last_src[18] = "??";
static char dd_last_dst[18] = "??";
static uint8_t dd_last_channel = 0;
static unsigned long dd_last_alert = 0;

static void fmtMac(const uint8_t *m, char *out, size_t n) {
    snprintf(out, n, "%02X:%02X:%02X:%02X:%02X:%02X",
             m[0], m[1], m[2], m[3], m[4], m[5]);
}

static bool isBroadcast(const uint8_t *m) {
    for (int i = 0; i < 6; i++) if (m[i] != 0xFF) return false;
    return true;
}

static void ddPromiscuousCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    if (pkt->rx_ctrl.sig_len < 26) return;

    uint8_t fc0 = frame[0];
    uint8_t subtype = (fc0 & 0xF0) >> 4;
    // 0x0C = deauth, 0x0A = disassociation
    if (subtype != 0x0C && subtype != 0x0A) return;

    const uint8_t *dst = &frame[4];
    const uint8_t *src = &frame[10];

    if (subtype == 0x0C) dd_deauth_count++;
    else dd_disassoc_count++;

    if (isBroadcast(dst)) dd_broadcast_count++;

    fmtMac(src, dd_last_src, sizeof(dd_last_src));
    fmtMac(dst, dd_last_dst, sizeof(dd_last_dst));
    dd_last_channel = pkt->rx_ctrl.channel;
    dd_last_alert = millis();
}

void arsenal_deauth_detector(void) {
    ARSENAL_HEAP_CHECK();

    drawMainBorderWithTitle("Deauth Detector");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Listening for deauth frames");
    tft.setCursor(12, 66);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 20, 1);

    dd_deauth_count = 0;
    dd_disassoc_count = 0;
    dd_broadcast_count = 0;
    dd_last_alert = 0;

    wifi_mode_t prevMode;
    esp_wifi_get_mode(&prevMode);
    bool wasPromis = false;

    WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(false);
    delay(10);
    esp_wifi_set_promiscuous_filter(NULL);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(ddPromiscuousCb);

    uint8_t channel = 1;
    unsigned long lastDraw = 0;
    unsigned long lastHop = 0;
    unsigned long startTime = millis();

    while (!check(EscPress)) {
        // Hop channels 1-13 to catch deauths on any band.
        if (millis() - lastHop > 500) {
            lastHop = millis();
            esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
            channel = (channel % 13) + 1;
        }

        if (millis() - lastDraw > 250) {
            lastDraw = millis();
            drawMainBorderWithTitle("Deauth Detector");
            int y = 45;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Deauth: %lu  Disassoc: %lu",
                       (unsigned long)dd_deauth_count, (unsigned long)dd_disassoc_count);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Broadcast: %lu", (unsigned long)dd_broadcast_count);
            y += 14;

            if (dd_last_alert > 0 && (millis() - dd_last_alert) < 3000) {
                tft.setTextColor(TFT_RED, bruceConfig.bgColor);
                tft.setCursor(12, y);
                tft.printf("ALERT ch%u", dd_last_channel);
                y += 14;
                tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
                tft.setCursor(12, y);
                tft.printf("from: %s", dd_last_src);
                y += 14;
                tft.setCursor(12, y);
                tft.printf("to:   %s", dd_last_dst);
            } else {
                tft.setTextColor(tft.color565(120, 120, 120), bruceConfig.bgColor);
                tft.setCursor(12, y);
                unsigned long elapsed = (millis() - startTime) / 1000;
                tft.printf("Quiet for %lus", elapsed);
            }

            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 20, 1);
        }

        esp_task_wdt_reset();
        delay(10);
    }

    esp_wifi_set_promiscuous(false);
    WiFi.mode(prevMode);
    (void)wasPromis;
}
#endif
