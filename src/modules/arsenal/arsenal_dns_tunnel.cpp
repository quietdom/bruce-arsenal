#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <globals.h>

static bool dnsTunnelActive = false;
static char tunnelData[256];
static int tunnelIdx = 0;
static int queriesSent = 0;
static char tunnelDomain[64] = "arsenal.example.com";
static WiFiUDP dnsUDP;
static uint8_t dnsBuffer[512];

static void encodeToSubdomains(const uint8_t *data, int len, char *out, size_t outCap) {
    out[0] = '\0';
    int pos = 0;
    for (int i = 0; i < len && pos < (int)outCap - 8; i++) {
        char hex[4];
        snprintf(hex, sizeof(hex), "%02x.", data[i]);
        strcat(out + pos, hex);
        pos += 3;
    }
    strcat(out + pos, tunnelDomain);
}

static void handleDnsQuery(const uint8_t *pkt, int len) {
    if (len < 12) return;

    memcpy(dnsBuffer, pkt, len);
    uint16_t id = (dnsBuffer[0] << 8) | dnsBuffer[1];
    uint16_t flags = (dnsBuffer[2] << 8) | dnsBuffer[3];

    if ((flags & 0x8000) != 0) return;

    int qdcount = (dnsBuffer[4] << 8) | dnsBuffer[5];
    int offset = 12;

    char qname[256];
    int qi = 0;
    while (offset < len && pkt[offset] != 0 && qi < 255) {
        int segLen = pkt[offset++];
        if (qi > 0) qname[qi++] = '.';
        for (int j = 0; j < segLen && offset < len; j++) {
            qname[qi++] = pkt[offset++];
        }
    }
    qname[qi] = '\0';
    offset += 5;

    if (strstr(qname, tunnelDomain)) {
        int dataStart = 0;
        char *dot = qname;
        while (dot && dot < qname + qi) {
            char *nextDot = strchr(dot, '.');
            if (!nextDot) break;
            int segLen = nextDot - dot;
            if (segLen == 2) {
                char hex[3] = {dot[0], dot[1], '\0'};
                uint8_t byte = (uint8_t)strtol(hex, nullptr, 16);
                if (tunnelIdx < (int)sizeof(tunnelData) - 1) {
                    tunnelData[tunnelIdx++] = byte;
                    tunnelData[tunnelIdx] = '\0';
                }
            }
            dot = nextDot + 1;
        }
        queriesSent++;
    }

    dnsBuffer[2] = 0x81;
    dnsBuffer[3] = 0x80;
    dnsBuffer[6] = 0x00;
    dnsBuffer[7] = 0x01;
    dnsBuffer[8] = 0x00;
    dnsBuffer[9] = 0x01;
    dnsBuffer[10] = 0x00;
    dnsBuffer[11] = 0x00;

    int ansOffset = offset;
    while (ansOffset < len && dnsBuffer[ansOffset] != 0) ansOffset++;
    ansOffset += 5;

    dnsBuffer[ansOffset++] = 0xC0;
    dnsBuffer[ansOffset++] = 0x0C;
    dnsBuffer[ansOffset++] = 0x00;
    dnsBuffer[ansOffset++] = 0x01;
    dnsBuffer[ansOffset++] = 0x00;
    dnsBuffer[ansOffset++] = 0x01;
    dnsBuffer[ansOffset++] = 0x00;
    dnsBuffer[ansOffset++] = 0x00;
    dnsBuffer[ansOffset++] = 0x00;
    dnsBuffer[ansOffset++] = 0x3C;
    dnsBuffer[ansOffset++] = 0x00;
    dnsBuffer[ansOffset++] = 0x04;
    dnsBuffer[ansOffset++] = 192;
    dnsBuffer[ansOffset++] = 168;
    dnsBuffer[ansOffset++] = 4;
    dnsBuffer[ansOffset++] = 1;

    dnsUDP.beginPacket(dnsUDP.remoteIP(), dnsUDP.remotePort());
    dnsUDP.write(dnsBuffer, ansOffset);
    dnsUDP.endPacket();
}

void arsenal_dns_tunnel(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() != WIFI_STA && WiFi.getMode() != WIFI_AP) {
        displayRedStripe("WiFi required");
        delay(1500);
        return;
    }

    tunnelIdx = 0;
    queriesSent = 0;
    memset(tunnelData, 0, sizeof(tunnelData));

    dnsUDP.begin(53);
    dnsTunnelActive = true;

    drawMainBorderWithTitle("DNS Tunnel");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Listening on port 53");
    tft.setCursor(12, 66);
    tft.printf("Domain: %s", tunnelDomain);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

    while (dnsTunnelActive) {
        int packetSize = dnsUDP.parsePacket();
        if (packetSize > 0) {
            uint8_t pkt[512];
            int len = dnsUDP.read(pkt, sizeof(pkt));
            handleDnsQuery(pkt, len);
        }

        if (millis() % 500 < 50) {
            drawMainBorderWithTitle("DNS Tunnel");
            int y = 45;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Queries: %d", queriesSent);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Data bytes: %d", tunnelIdx);
            y += 14;
            if (tunnelIdx > 0) {
                int show = tunnelIdx > 20 ? 20 : tunnelIdx;
                char preview[21];
                memcpy(preview, tunnelData + tunnelIdx - show, show);
                preview[show] = '\0';
                tft.setCursor(12, y);
                tft.printf("Last: %s", preview);
            }
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
        }

        if (check(EscPress)) dnsTunnelActive = false;
        esp_task_wdt_reset();
        delay(10);
    }

    dnsUDP.stop();

    if (tunnelIdx > 0 && setupSdCard()) {
        if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
        File f = SD.open("/arsenal/dns_tunnel.txt", FILE_WRITE);
        if (f) {
            f.write(tunnelData, tunnelIdx);
            f.close();
        }
    }
}
