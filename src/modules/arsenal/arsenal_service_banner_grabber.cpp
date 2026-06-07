#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <globals.h>

static const uint16_t BANNER_PORTS[] = {21, 22, 23, 25, 80, 110, 443, 445, 993, 995, 3389, 8080, 8443};
static const int NUM_PORTS = sizeof(BANNER_PORTS) / sizeof(BANNER_PORTS[0]);

struct BannerResult {
    IPAddress ip;
    uint16_t port;
    char banner[128];
};

static BannerResult results[20];
static int resultCount = 0;

void arsenal_service_banner_grabber(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() != WIFI_STA || WiFi.status() != WL_CONNECTED) {
        displayRedStripe("WiFi not connected");
        delay(1500);
        return;
    }

    resultCount = 0;
    IPAddress gateway = WiFi.gatewayIP();
    uint32_t gw = (uint32_t)gateway;
    uint32_t mask = (uint32_t)WiFi.subnetMask();
    uint32_t network = gw & mask;
    uint32_t total = (network | ~mask) - network - 1;
    if (total > 50) total = 50;

    drawMainBorderWithTitle("Banner Grab");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Scanning %d hosts...", (int)total);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

    for (uint32_t i = 1; i <= total; i++) {
        if (check(EscPress)) break;

        IPAddress target((network + i) & 0xFF, ((network + i) >> 8) & 0xFF,
                         ((network + i) >> 16) & 0xFF, ((network + i) >> 24) & 0xFF);

        for (int p = 0; p < NUM_PORTS && resultCount < 20; p++) {
            WiFiClient client;
            client.setTimeout(100);
            if (!client.connect(target, BANNER_PORTS[p], 100)) continue;

            delay(200);
            String banner = "";
            unsigned long start = millis();
            while (client.available() && millis() - start < 1000) {
                char c = client.read();
                if (c >= 0x20 && c < 0x7F) banner += c;
                if (banner.length() >= 120) break;
            }

            if (banner.length() == 0) {
                client.print("HEAD / HTTP/1.0\r\nHost: " + target.toString() + "\r\n\r\n");
                start = millis();
                while (client.available() && millis() - start < 500) {
                    char c = client.read();
                    if (c >= 0x20 && c < 0x7F) banner += c;
                    if (banner.length() >= 120) break;
                }
            }

            if (banner.length() > 0) {
                BannerResult &r = results[resultCount];
                r.ip = target;
                r.port = BANNER_PORTS[p];
                strncpy(r.banner, banner.c_str(), 127);
                r.banner[127] = '\0';
                resultCount++;
            }
            client.stop();
        }

        if (i % 5 == 0) {
            drawMainBorderWithTitle("Banner Grab");
            int y = 45;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Host: %s", target.toString().c_str());
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Banners: %d", resultCount);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
        }
        esp_task_wdt_reset();
    }

    drawMainBorderWithTitle("Banner Grab");
    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
    tft.setTextSize(FP);
    int y = 38;
    tft.setCursor(12, y);
    tft.printf("Found %d banners:", resultCount);
    y += 14;
    for (int i = 0; i < resultCount && i < 7; i++) {
        tft.setCursor(12, y);
        tft.printf("%s:%d", results[i].ip.toString().c_str(), results[i].port);
        y += 11;
        tft.setCursor(16, y);
        String b = String(results[i].banner).substring(0, 24);
        tft.print(b);
        y += 11;
    }
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
    while (!check(EscPress)) delay(100);
}
