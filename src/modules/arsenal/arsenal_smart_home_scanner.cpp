#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <globals.h>

struct SmartDevice {
    IPAddress ip;
    char type[16];
    char name[48];
};

static SmartDevice devices[20];
static int devCount = 0;

static bool checkTuya(IPAddress &ip, SmartDevice &d) {
    WiFiClient client;
    client.setTimeout(100);
    if (!client.connect(ip, 6668, 100)) return false;
    uint8_t probe[] = {0x00, 0x00, 0x55, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07};
    client.write(probe, sizeof(probe));
    delay(200);
    bool found = client.available() > 0;
    client.stop();
    if (found) {
        strcpy(d.type, "Tuya");
        strcpy(d.name, "Tuya Device");
        return true;
    }
    return false;
}

static bool checkTasmota(IPAddress &ip, SmartDevice &d) {
    WiFiClient client;
    client.setTimeout(100);
    if (!client.connect(ip, 80, 100)) return false;
    client.print("GET / HTTP/1.0\r\nHost: " + ip.toString() + "\r\n\r\n");
    delay(200);
    String resp = "";
    while (client.available()) resp += (char)client.read();
    client.stop();
    if (resp.indexOf("Tasmota") >= 0 || resp.indexOf("tasmota") >= 0) {
        strcpy(d.type, "Tasmota");
        int s = resp.indexOf("Tasmota");
        if (s < 0) s = resp.indexOf("tasmota");
        int end = resp.indexOf('<', s);
        if (end < 0) end = s + 20;
        int len = end - s;
        if (len > 47) len = 47;
        strncpy(d.name, resp.substring(s, s + len).c_str(), 47);
        return true;
    }
    return false;
}

static bool checkESPHome(IPAddress &ip, SmartDevice &d) {
    WiFiClient client;
    client.setTimeout(100);
    if (!client.connect(ip, 80, 100)) return false;
    client.print("GET / HTTP/1.0\r\nHost: " + ip.toString() + "\r\n\r\n");
    delay(200);
    String resp = "";
    while (client.available()) resp += (char)client.read();
    client.stop();
    if (resp.indexOf("esphome") >= 0 || resp.indexOf("ESPHome") >= 0) {
        strcpy(d.type, "ESPHome");
        strcpy(d.name, "ESPHome Device");
        return true;
    }
    return false;
}

void arsenal_smart_home_scanner(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() != WIFI_STA || WiFi.status() != WL_CONNECTED) {
        displayRedStripe("WiFi not connected");
        delay(1500);
        return;
    }

    devCount = 0;
    IPAddress gateway = WiFi.gatewayIP();
    uint32_t gw = (uint32_t)gateway;
    uint32_t mask = (uint32_t)WiFi.subnetMask();
    uint32_t network = gw & mask;
    uint32_t total = (network | ~mask) - network - 1;
    if (total > 100) total = 100;

    drawMainBorderWithTitle("Smart Home Scan");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Scanning for smart devices...");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

    for (uint32_t i = 1; i <= total; i++) {
        if (check(EscPress)) break;

        IPAddress target((network + i) & 0xFF, ((network + i) >> 8) & 0xFF,
                         ((network + i) >> 16) & 0xFF, ((network + i) >> 24) & 0xFF);

        WiFiClient client;
        client.setTimeout(50);
        bool alive = client.connect(target, 80, 50);
        if (!alive) alive = client.connect(target, 8080, 50);
        client.stop();

        if (alive && devCount < 20) {
            SmartDevice &d = devices[devCount];
            d.ip = target;
            strcpy(d.type, "Unknown");
            strcpy(d.name, "HTTP Device");

            if (checkTuya(target, d)) { devCount++; continue; }
            if (checkTasmota(target, d)) { devCount++; continue; }
            if (checkESPHome(target, d)) { devCount++; continue; }
        }

        if (i % 10 == 0) {
            drawMainBorderWithTitle("Smart Home Scan");
            int y = 50;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Scanning: %s", target.toString().c_str());
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Devices found: %d", devCount);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
        }
        esp_task_wdt_reset();
    }

    drawMainBorderWithTitle("Smart Home Scan");
    tft.setTextColor(devCount > 0 ? TFT_GREEN : TFT_RED, bruceConfig.bgColor);
    tft.setTextSize(FP);
    int y = 38;
    tft.setCursor(12, y);
    tft.printf("Found %d devices:", devCount);
    y += 14;
    for (int i = 0; i < devCount && i < 6; i++) {
        tft.setCursor(12, y);
        tft.printf("[%s] %s", devices[i].type, devices[i].ip.toString().c_str());
        y += 12;
    }
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
    while (!check(EscPress)) delay(100);
}
