#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

#define MAX_CHANNELS 14
static int channelCount[MAX_CHANNELS];
static bool chartRunning = false;

static void chartPromiscCb(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (!chartRunning) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    int ch = pkt->rx_ctrl.channel - 1;
    if (ch >= 0 && ch < MAX_CHANNELS) {
        channelCount[ch]++;
    }
}

void arsenal_wifi_channel_chart(void) {
    ARSENAL_HEAP_CHECK();
    memset(channelCount, 0, sizeof(channelCount));
    chartRunning = true;

    bool bgWasRunning = arsenal_background_is_running();
    if (bgWasRunning) arsenal_background_stop();

    if (WiFi.getMode() == WIFI_MODE_NULL) WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(chartPromiscCb);

    unsigned long startTime = millis();

    while (true) {
        drawMainBorderWithTitle("Channel Chart");
        int y = 36;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Collecting... %lus", elapsed);
        y += 16;

        int maxCount = 1;
        for (int i = 0; i < MAX_CHANNELS; i++) {
            if (channelCount[i] > maxCount) maxCount = channelCount[i];
        }

        int barMaxW = tftWidth - 50;
        for (int i = 0; i < MAX_CHANNELS; i++) {
            int barW = (int)((long)channelCount[i] * barMaxW / maxCount);
            uint16_t color = TFT_GREEN;
            if (channelCount[i] > maxCount * 0.7) color = TFT_RED;
            else if (channelCount[i] > maxCount * 0.3) color = TFT_YELLOW;

            tft.setCursor(8, y);
            tft.printf("%2d", i + 1);
            tft.fillRect(28, y, barW, 10, color);
            tft.setCursor(28 + barW + 4, y);
            tft.printf("%d", channelCount[i]);
            y += 12;
        }

        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        delay(300);
    }

    chartRunning = false;
    esp_wifi_set_promiscuous(false);
    if (bgWasRunning) arsenal_background_start();
}
