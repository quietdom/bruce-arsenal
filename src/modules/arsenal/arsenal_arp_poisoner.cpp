#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <lwip/etharp.h>
#include <lwip/netif.h>
#include <globals.h>

static void arpRequest(IPAddress &ip) {
    ip4_addr_t addr;
    IP4_ADDR(&addr, ip[0], ip[1], ip[2], ip[3]);
    etharp_request(netif_default, &addr);
}

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

    uint8_t gw[4] = {gateway[0], gateway[1], gateway[2], gateway[3]};
    uint8_t sn[4] = {subnet[0], subnet[1], subnet[2], subnet[3]};

    uint32_t network = ((uint32_t)gw[0] | ((uint32_t)gw[1] << 8) |
                        ((uint32_t)gw[2] << 16) | ((uint32_t)gw[3] << 24));
    uint32_t mask32 = ((uint32_t)sn[0] | ((uint32_t)sn[1] << 8) |
                       ((uint32_t)sn[2] << 16) | ((uint32_t)sn[3] << 24));
    uint32_t netBase = network & mask32;
    uint32_t netBcast = netBase | ~mask32;
    uint32_t total = netBcast - netBase - 1;
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

            uint32_t host = netBase + i;
            IPAddress tip((uint8_t)(host & 0xFF), (uint8_t)((host >> 8) & 0xFF),
                          (uint8_t)((host >> 16) & 0xFF), (uint8_t)((host >> 24) & 0xFF));

            if (tip == localIP) continue;

            arpRequest(tip);
            arpRequest(gateway);
            targets++;
            delay(2);
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
