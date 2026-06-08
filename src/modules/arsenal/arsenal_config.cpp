#include "arsenal_config.h"
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <globals.h>


static ArsenalConfig g_config = {
    "ArsenalNet", "arsenal32", "admin", "arsenal", "", false
};

static bool g_configLoaded = false;


ArsenalConfig &arsenal_config(void) { return g_config; }


static void parseLine(String line, String &key, String &val) {
    line.trim();
    int eq = line.indexOf('=');
    if (eq < 0) { key = ""; val = ""; return; }
    key = line.substring(0, eq);
    key.trim();
    val = line.substring(eq + 1);
    val.trim();
}


static void copyToBuf(char *dst, size_t cap, const String &src) {
    size_t n = min((size_t)src.length(), cap - 1);
    memcpy(dst, src.c_str(), n);
    dst[n] = '\0';
}


void arsenal_config_load(void) {
    if (g_configLoaded) return;
    if (!setupSdCard()) { g_configLoaded = true; return; }

    if (!SD.exists("/arsenal/config.txt")) {
        g_configLoaded = true;
        return;
    }

    File f = SD.open("/arsenal/config.txt", FILE_READ);
    if (!f) { g_configLoaded = true; return; }

    while (f.available()) {
        String line = f.readStringUntil('\n');
        String key, val;
        parseLine(line, key, val);
        if (key.length() == 0) continue;

        if (key == "ap_ssid")   copyToBuf(g_config.apSsid, sizeof(g_config.apSsid), val);
        else if (key == "ap_pass")   copyToBuf(g_config.apPass, sizeof(g_config.apPass), val);
        else if (key == "dash_user") copyToBuf(g_config.dashUser, sizeof(g_config.dashUser), val);
        else if (key == "dash_pass") copyToBuf(g_config.dashPass, sizeof(g_config.dashPass), val);
        else if (key == "pin") {
            copyToBuf(g_config.pin, sizeof(g_config.pin), val);
            g_config.pinEnabled = (val.length() == 4);
        }
    }
    f.close();
    g_configLoaded = true;
}


void arsenal_config_save(void) {
    if (!setupSdCard()) return;
    if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");

    File f = SD.open("/arsenal/config.txt", FILE_WRITE);
    if (!f) return;

    f.println("# Arsenal configuration");
    f.printf("ap_ssid=%s\n", g_config.apSsid);
    f.printf("ap_pass=%s\n", g_config.apPass);
    f.printf("dash_user=%s\n", g_config.dashUser);
    f.printf("dash_pass=%s\n", g_config.dashPass);
    if (g_config.pinEnabled) {
        f.printf("pin=%s\n", g_config.pin);
    }
    f.close();
}


void arsenal_config_reset(void) {
    strcpy(g_config.apSsid, "ArsenalNet");
    strcpy(g_config.apPass, "arsenal32");
    strcpy(g_config.dashUser, "admin");
    strcpy(g_config.dashPass, "arsenal");
    strcpy(g_config.pin, "");
    g_config.pinEnabled = false;
    g_configLoaded = false;
}


bool arsenal_pin_check(void) {
    arsenal_config_load();
    if (!g_config.pinEnabled || strlen(g_config.pin) != 4) return true;

    char entered[8] = {0};
    int pos = 0;

    drawMainBorderWithTitle("PIN Lock");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FM);
    tft.drawCentreString(String("Enter 4-digit PIN"), tftWidth / 2, 60, 1);
    tft.setTextSize(FP);
    tft.drawCentreString(String("Sel=next  Esc=cancel"), tftWidth / 2, 80, 1);

    int digitY = 110;
    while (pos < 4) {
        tft.fillRect(0, digitY, tftWidth, 40, bruceConfig.bgColor);
        for (int i = 0; i < 4; i++) {
            int boxX = tftWidth / 2 - 60 + i * 30;
            tft.drawRect(boxX, digitY, 24, 32, bruceConfig.priColor);
            if (i < pos) {
                tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                tft.drawCentreString(String("*"), boxX + 12, digitY + 8, 1);
            } else if (i == pos) {
                tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                tft.drawCentreString(String(entered[i]), boxX + 12, digitY + 8, 1);
            }
        }
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);

        if (check(NextPress)) {
            entered[pos] = (entered[pos] - '0' + 1) % 10 + '0';
            while (check(NextPress)) delay(10);
        } else if (check(PrevPress)) {
            entered[pos] = (entered[pos] - '0' + 9) % 10 + '0';
            while (check(PrevPress)) delay(10);
        } else if (check(SelPress)) {
            while (check(SelPress)) delay(10);
            if (strlen(entered) == 0) entered[pos] = '0';
            pos++;
        } else if (check(EscPress)) {
            while (check(EscPress)) delay(10);
            return false;
        }
        delay(20);
    }

    return strcmp(entered, g_config.pin) == 0;
}


void arsenal_pin_set(const char *newPin) {
    if (strlen(newPin) != 4) return;
    strncpy(g_config.pin, newPin, 4);
    g_config.pin[4] = '\0';
    g_config.pinEnabled = true;
    arsenal_config_save();
}


void arsenal_pin_clear(void) {
    g_config.pin[0] = '\0';
    g_config.pinEnabled = false;
    arsenal_config_save();
}


static bool editField(const char *title, char *buf, size_t cap, bool mask = false) {
    String current = String(buf);
    String result = keyboard(current, cap - 1, title, mask);
    if (result.length() == 0) return false;
    copyToBuf(buf, cap, result);
    return true;
}


static void showConfigEditor(int focus) {
    arsenal_config_load();
    bool dirty = false;

    while (true) {
        options.clear();

        options.push_back({String("AP SSID: ") + g_config.apSsid, [&dirty]() {
            if (editField("AP SSID (max 32)", g_config.apSsid, sizeof(g_config.apSsid))) dirty = true;
        }});
        options.push_back({String("AP Pass: ") + String(strlen(g_config.apPass) > 0 ? "********" : "(none)"), [&dirty]() {
            if (editField("AP Password (8-63)", g_config.apPass, sizeof(g_config.apPass), true)) dirty = true;
        }});
        options.push_back({String("Dash User: ") + g_config.dashUser, [&dirty]() {
            if (editField("Dashboard User", g_config.dashUser, sizeof(g_config.dashUser))) dirty = true;
        }});
        options.push_back({String("Dash Pass: ") + String(strlen(g_config.dashPass) > 0 ? "********" : "(none)"), [&dirty]() {
            if (editField("Dashboard Password", g_config.dashPass, sizeof(g_config.dashPass), true)) dirty = true;
        }});

        String pinLabel = g_config.pinEnabled ? (String("PIN: ") + g_config.pin) : "PIN: (disabled)";
        options.push_back({pinLabel, [&dirty]() {
            String p = keyboard("", 4, "Set 4-digit PIN (blank=disable)", true);
            if (p.length() == 4) {
                copyToBuf(g_config.pin, sizeof(g_config.pin), p);
                g_config.pinEnabled = true;
                dirty = true;
                displayRedStripe("PIN enabled");
            } else if (p.length() == 0) {
                arsenal_pin_clear();
                dirty = true;
                displayRedStripe("PIN disabled");
            } else {
                displayRedStripe("PIN must be 4 digits");
            }
            delay(1000);
        }});

        options.push_back({"Save & Exit", [&dirty]() {
            if (dirty) arsenal_config_save();
        }});
        options.push_back({"Reset to Defaults", []() {
            arsenal_config_reset();
            arsenal_config_save();
        }});

        if (focus >= 0 && focus < (int)options.size()) {
            loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal Config", focus);
        } else {
            loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal Config");
        }
        break;
    }
}


void arsenal_config_ap(void) {
    ARSENAL_HEAP_CHECK();
    showConfigEditor(0);
}

void arsenal_config_dashboard(void) {
    ARSENAL_HEAP_CHECK();
    showConfigEditor(2);
}

void arsenal_pin_lock(void) {
    ARSENAL_HEAP_CHECK();
    arsenal_config_load();
    options.clear();

    if (g_config.pinEnabled) {
        options.push_back({"Change PIN", []() {
            String p = keyboard("", 4, "New 4-digit PIN", true);
            if (p.length() == 4) {
                arsenal_pin_set(p.c_str());
                displayRedStripe("PIN updated");
            } else {
                displayRedStripe("PIN must be 4 digits");
            }
            delay(1000);
        }});
        options.push_back({"Disable PIN", []() {
            arsenal_pin_clear();
                displayRedStripe("PIN disabled");
            delay(1000);
        }});
        options.push_back({"Verify PIN", []() {
            if (arsenal_pin_check()) {
                displayRedStripe("PIN correct");
            } else {
                displayRedStripe("Wrong PIN");
            }
            delay(1000);
        }});
    } else {
        options.push_back({"Enable PIN", []() {
            String p = keyboard("", 4, "Set 4-digit PIN", true);
            if (p.length() == 4) {
                arsenal_pin_set(p.c_str());
                displayRedStripe("PIN enabled");
            } else {
                displayRedStripe("PIN must be 4 digits");
            }
            delay(1000);
        }});
    }

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "PIN Lock");
}


static ArsenalStats g_stats = {0};

ArsenalStats &arsenal_stats(void) { return g_stats; }


void arsenal_stats_load(void) {
    if (!setupSdCard()) return;
    if (!SD.exists("/arsenal/stats.json")) return;

    File f = SD.open("/arsenal/stats.json", FILE_READ);
    if (!f) return;

    String content = f.readString();
    f.close();

    #define PARSE_U32(name) \
        g_stats.name = (uint32_t)content.substring(content.indexOf("\"" #name "\":") + strlen(#name) + 3) \
                                .toInt()

    int idx;
    if ((idx = content.indexOf("\"creds\":")) >= 0)       g_stats.credsCaptured    = (uint32_t)content.substring(idx + 8).toInt();
    if ((idx = content.indexOf("\"devices\":")) >= 0)     g_stats.devicesSeen       = (uint32_t)content.substring(idx + 10).toInt();
    if ((idx = content.indexOf("\"attacks\":")) >= 0)     g_stats.attacksRun        = (uint32_t)content.substring(idx + 10).toInt();
    if ((idx = content.indexOf("\"probes\":")) >= 0)      g_stats.probesLogged      = (uint32_t)content.substring(idx + 9).toInt();
    if ((idx = content.indexOf("\"portals\":")) >= 0)     g_stats.portalsServed     = (uint32_t)content.substring(idx + 10).toInt();
    if ((idx = content.indexOf("\"handshakes\":")) >= 0)  g_stats.wpaHandshakes     = (uint32_t)content.substring(idx + 13).toInt();
    if ((idx = content.indexOf("\"deauths\":")) >= 0)     g_stats.deauthsDetected   = (uint32_t)content.substring(idx + 10).toInt();
    if ((idx = content.indexOf("\"ble\":")) >= 0)         g_stats.bleDevicesSeen    = (uint32_t)content.substring(idx + 6).toInt();
    if ((idx = content.indexOf("\"passwords\":")) >= 0)   g_stats.passwordsGenerated = (uint32_t)content.substring(idx + 12).toInt();
    if ((idx = content.indexOf("\"combos\":")) >= 0)      g_stats.combosExecuted    = (uint32_t)content.substring(idx + 9).toInt();

    #undef PARSE_U32
}


void arsenal_stats_save(void) {
    if (!setupSdCard()) return;
    if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");

    File f = SD.open("/arsenal/stats.json", FILE_WRITE);
    if (!f) return;

    f.println("{");
    f.printf("  \"creds\":%u,\n", g_stats.credsCaptured);
    f.printf("  \"devices\":%u,\n", g_stats.devicesSeen);
    f.printf("  \"attacks\":%u,\n", g_stats.attacksRun);
    f.printf("  \"probes\":%u,\n", g_stats.probesLogged);
    f.printf("  \"portals\":%u,\n", g_stats.portalsServed);
    f.printf("  \"handshakes\":%u,\n", g_stats.wpaHandshakes);
    f.printf("  \"deauths\":%u,\n", g_stats.deauthsDetected);
    f.printf("  \"ble\":%u,\n", g_stats.bleDevicesSeen);
    f.printf("  \"passwords\":%u,\n", g_stats.passwordsGenerated);
    f.printf("  \"combos\":%u\n", g_stats.combosExecuted);
    f.println("}");
    f.close();
}


void arsenal_stats_reset(void) {
    memset(&g_stats, 0, sizeof(g_stats));
    arsenal_stats_save();
}


static String formatDuration(unsigned long ms) {
    unsigned long s = ms / 1000;
    if (s < 60) return String(s) + "s";
    unsigned long m = s / 60;
    s = s % 60;
    if (m < 60) return String(m) + "m " + String(s) + "s";
    unsigned long h = m / 60;
    m = m % 60;
    if (h < 24) return String(h) + "h " + String(m) + "m";
    unsigned long d = h / 24;
    h = h % 24;
    return String(d) + "d " + String(h) + "h";
}


void arsenal_attack_stats(void) {
    ARSENAL_HEAP_CHECK();
    arsenal_stats_load();

    while (true) {
        options.clear();

        options.push_back({"Refresh", []() { arsenal_stats_load(); }});
        options.push_back({"Save to SD", []() {
            arsenal_stats_save();
            displayRedStripe("Stats saved");
            delay(1000);
        }});
        options.push_back({"Reset Stats", []() {
            arsenal_stats_reset();
            displayRedStripe("Stats reset");
            delay(1000);
        }});
        options.push_back({"Back", []() {}});

        drawMainBorderWithTitle("Attack Stats");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);

        int y = 38;
        int lineH = 16;
        tft.setCursor(10, y); tft.printf("Creds:     %u", g_stats.credsCaptured); y += lineH;
        tft.setCursor(10, y); tft.printf("Devices:   %u", g_stats.devicesSeen); y += lineH;
        tft.setCursor(10, y); tft.printf("Attacks:   %u", g_stats.attacksRun); y += lineH;
        tft.setCursor(10, y); tft.printf("Probes:    %u", g_stats.probesLogged); y += lineH;
        tft.setCursor(10, y); tft.printf("Portals:   %u", g_stats.portalsServed); y += lineH;
        tft.setCursor(10, y); tft.printf("Handshks:  %u", g_stats.wpaHandshakes); y += lineH;
        tft.setCursor(10, y); tft.printf("Deauths:   %u", g_stats.deauthsDetected); y += lineH;
        tft.setCursor(10, y); tft.printf("BLE devs:  %u", g_stats.bleDevicesSeen); y += lineH;
        tft.setCursor(10, y); tft.printf("Pwds:      %u", g_stats.passwordsGenerated); y += lineH;
        tft.setCursor(10, y); tft.printf("Combos:    %u", g_stats.combosExecuted); y += lineH;

        tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
        tft.setCursor(10, tftHeight - 22);
        tft.printf("Up: %s", formatDuration(millis()).c_str());

        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Sel=menu  Esc=exit"), tftWidth / 2, tftHeight - 10, 1);

        if (check(SelPress)) {
            while (check(SelPress)) delay(10);
            addOptionToMainMenu();
            loopOptions(options, MENU_TYPE_SUBMENU, "Stats Menu");
            arsenal_stats_save();
        } else if (check(EscPress)) {
            while (check(EscPress)) delay(10);
            arsenal_stats_save();
            return;
        }
        delay(50);
    }
}
