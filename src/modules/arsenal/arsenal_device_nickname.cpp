#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <globals.h>

#define MAX_NICKS 32

struct NickEntry {
    uint8_t mac[6];
    char name[24];
    bool used;
};

static NickEntry nicks[MAX_NICKS];
static int nickCount = 0;

static void loadNicks() {
    nickCount = 0;
    if (!setupSdCard()) return;
    if (!SD.exists("/arsenal/nicknames.txt")) return;
    File f = SD.open("/arsenal/nicknames.txt", FILE_READ);
    if (!f) return;
    while (f.available() && nickCount < MAX_NICKS) {
        String line = f.readStringUntil('\n');
        line.trim();
        if (line.length() < 10) continue;
        int comma = line.indexOf(',');
        if (comma < 0) continue;
        String macStr = line.substring(0, comma);
        String name = line.substring(comma + 1);
        if (macStr.length() != 17 || name.length() == 0) continue;
        NickEntry &e = nicks[nickCount];
        unsigned int mac[6];
        if (sscanf(macStr.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X",
                   &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6) continue;
        for (int i = 0; i < 6; i++) e.mac[i] = (uint8_t)mac[i];
        strncpy(e.name, name.c_str(), 23);
        e.name[23] = '\0';
        e.used = true;
        nickCount++;
    }
    f.close();
}

static void saveNicks() {
    if (!setupSdCard()) return;
    if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
    File f = SD.open("/arsenal/nicknames.txt", FILE_WRITE);
    if (!f) return;
    for (int i = 0; i < nickCount; i++) {
        if (!nicks[i].used) continue;
        f.printf("%02X:%02X:%02X:%02X:%02X:%02X,%s\n",
            nicks[i].mac[0], nicks[i].mac[1], nicks[i].mac[2],
            nicks[i].mac[3], nicks[i].mac[4], nicks[i].mac[5], nicks[i].name);
    }
    f.close();
}

static const char *getNickname(const uint8_t *mac) {
    for (int i = 0; i < nickCount; i++) {
        if (!nicks[i].used) continue;
        bool match = true;
        for (int j = 0; j < 6; j++) {
            if (nicks[i].mac[j] != mac[j]) { match = false; break; }
        }
        if (match) return nicks[i].name;
    }
    return nullptr;
}

void arsenal_device_nickname(void) {
    ARSENAL_HEAP_CHECK();
    loadNicks();

    options.clear();
    options.push_back({"Add Nickname", []() {
        drawMainBorderWithTitle("Add Nickname");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 45);
        tft.print("Format: XX:XX:XX:XX:XX:XX");
        tft.setCursor(12, 60);
        tft.print("Enter MAC on keyboard");
        tft.setCursor(12, 76);
        tft.print("then name after comma");
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("See serial for input", tftWidth / 2, tftHeight - 20, 1);

        Serial.println("[Nick] Enter MAC,name (e.g. AA:BB:CC:DD:EE:FF,MyPhone):");
        unsigned long start = millis();
        String input = "";
        while (millis() - start < 30000) {
            if (Serial.available()) {
                char c = Serial.read();
                if (c == '\n' || c == '\r') break;
                input += c;
            }
            if (check(EscPress)) return;
            delay(10);
        }

        int comma = input.indexOf(',');
        if (comma < 10) return;
        String macStr = input.substring(0, comma);
        String name = input.substring(comma + 1);
        name.trim();
        if (macStr.length() != 17 || name.length() == 0 || name.length() > 23) return;
        if (nickCount >= MAX_NICKS) return;

        unsigned int mac[6];
        if (sscanf(macStr.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X",
                   &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6) return;

        NickEntry &e = nicks[nickCount];
        for (int i = 0; i < 6; i++) e.mac[i] = (uint8_t)mac[i];
        strncpy(e.name, name.c_str(), 23);
        e.name[23] = '\0';
        e.used = true;
        nickCount++;
        saveNicks();
        displayRedStripe("Nickname saved!");
        delay(1000);
    }});

    options.push_back({"List Nicknames", []() {
        drawMainBorderWithTitle("Nicknames");
        int y = 40;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        for (int i = 0; i < nickCount && i < 10; i++) {
            tft.setCursor(12, y);
            tft.printf("%02X:%02X..%s = %s",
                nicks[i].mac[0], nicks[i].mac[1], nicks[i].name);
            y += 12;
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress)) delay(100);
    }});

    options.push_back({"Clear All", []() {
        nickCount = 0;
        memset(nicks, 0, sizeof(nicks));
        saveNicks();
        displayRedStripe("All nicknames cleared");
        delay(1000);
    }});

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Device Nicknames");
}
