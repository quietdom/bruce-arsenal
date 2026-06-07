#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <globals.h>

#define DEAD_DROP_PATH "/arsenal/deaddrop"
#define MAX_DROPS 32

struct DeadDrop {
    char sender[16];
    char message[128];
    unsigned long timestamp;
};

static DeadDrop drops[MAX_DROPS];
static int dropCount = 0;

static void loadDrops() {
    dropCount = 0;
    if (!setupSdCard()) return;
    if (!SD.exists(DEAD_DROP_PATH)) return;
    File f = SD.open(DEAD_DROP_PATH, FILE_READ);
    if (!f) return;
    while (f.available() && dropCount < MAX_DROPS) {
        String line = f.readStringUntil('\n');
        line.trim();
        if (line.length() < 5) continue;
        int p1 = line.indexOf('|');
        if (p1 < 0) continue;
        int p2 = line.indexOf('|', p1 + 1);
        if (p2 < 0) continue;
        DeadDrop &d = drops[dropCount];
        strncpy(d.sender, line.substring(0, p1).c_str(), 15);
        d.sender[15] = '\0';
        strncpy(d.message, line.substring(p1 + 1, p2).c_str(), 127);
        d.message[127] = '\0';
        d.timestamp = line.substring(p2 + 1).toInt();
        dropCount++;
    }
    f.close();
}

static void saveDrops() {
    if (!setupSdCard()) return;
    if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
    if (!SD.exists(DEAD_DROP_PATH)) SD.mkdir(DEAD_DROP_PATH);
    File f = SD.open(String(DEAD_DROP_PATH) + "/drops.txt", FILE_WRITE);
    if (!f) return;
    for (int i = 0; i < dropCount; i++) {
        f.printf("%s|%s|%lu\n", drops[i].sender, drops[i].message, drops[i].timestamp);
    }
    f.close();
}

void arsenal_dead_drop_mesh(void) {
    ARSENAL_HEAP_CHECK();
    loadDrops();

    options.clear();
    options.push_back({"Read Drops", []() {
        loadDrops();
        drawMainBorderWithTitle("Dead Drop");
        int y = 38;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Messages: %d", dropCount);
        y += 14;
        for (int i = dropCount - 1; i >= 0 && y < tftHeight - 30; i--) {
            tft.setCursor(12, y);
            tft.printf("[%s] %s", drops[i].sender, String(drops[i].message).substring(0, 20).c_str());
            y += 12;
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress)) delay(100);
    }});

    options.push_back({"Write Drop", []() {
        if (dropCount >= MAX_DROPS) {
            displayRedStripe("Drop box full");
            delay(1000);
            return;
        }
        drawMainBorderWithTitle("Write Drop");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("Type on serial:");
        tft.setCursor(12, 66);
        tft.print("sender,message");

        Serial.println("[DeadDrop] sender,message:");
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
        if (comma < 1) return;
        DeadDrop &d = drops[dropCount];
        strncpy(d.sender, input.substring(0, comma).c_str(), 15);
        d.sender[15] = '\0';
        strncpy(d.message, input.substring(comma + 1).c_str(), 127);
        d.message[127] = '\0';
        d.timestamp = millis();
        dropCount++;
        saveDrops();
        displayRedStripe("Drop saved!");
        delay(1000);
    }});

    options.push_back({"Clear Drops", []() {
        dropCount = 0;
        memset(drops, 0, sizeof(drops));
        if (setupSdCard() && SD.exists(String(DEAD_DROP_PATH) + "/drops.txt")) {
            SD.remove(String(DEAD_DROP_PATH) + "/drops.txt");
        }
        displayRedStripe("Drops cleared");
        delay(1000);
    }});

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Dead Drop Mesh");
}
