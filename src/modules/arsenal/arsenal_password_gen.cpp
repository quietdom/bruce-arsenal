#include "arsenal.h"
#include "arsenal_config.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <WiFi.h>
#include <esp_system.h>
#include <globals.h>


static const char CHARSET_LOWER[]  = "abcdefghijklmnopqrstuvwxyz";
static const char CHARSET_UPPER[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char CHARSET_DIGIT[]  = "0123456789";
static const char CHARSET_SYMBOL[] = "!@#$%^&*()-_=+[]{};:,.<>?/";


static void fillRandom(uint8_t *out, size_t n) {
    uint32_t r = 0;
    size_t filled = 0;
    while (filled < n) {
        r = esp_random();
        size_t take = min((size_t)4, n - filled);
        for (size_t i = 0; i < take; i++) {
            out[filled++] = (r >> (i * 8)) & 0xFF;
        }
    }
}


static String generatePassword(int len, int mode) {
    String pool = String(CHARSET_LOWER);
    if (mode >= 1) pool += CHARSET_UPPER;
    if (mode >= 2) pool += CHARSET_DIGIT;
    if (mode >= 3) pool += CHARSET_SYMBOL;

    String out;
    out.reserve(len);
    uint8_t buf[64];
    fillRandom(buf, len);
    for (int i = 0; i < len; i++) {
        out += pool[buf[i] % pool.length()];
    }
    return out;
}


void arsenal_password_generator(void) {
    ARSENAL_SAFE_RUN([]() {
        int lengths[] = {8, 12, 16, 24, 32};
        const char *lengthLabels[] = {"8 chars", "12 chars", "16 chars", "24 chars", "32 chars"};
        int lengthIdx = 2;
        int mode = 3;
        const char *modeLabels[] = {"lower", "alpha+num", "alnum+sym", "all printable"};

        String current = generatePassword(lengths[lengthIdx], mode);
        arsenal_stats_inc_attacks();
        arsenal_stats_inc_passwords();

        while (true) {
            options.clear();

            options.push_back({String("Length: ") + lengthLabels[lengthIdx], [lengths, &lengthIdx, &current, mode]() {
                lengthIdx = (lengthIdx + 1) % 5;
                current = generatePassword(lengths[lengthIdx], mode);
                arsenal_stats_inc_passwords();
            }});
            options.push_back({String("Mode: ") + modeLabels[mode], [&mode, lengths, &lengthIdx, &current]() {
                mode = (mode + 1) % 4;
                current = generatePassword(lengths[lengthIdx], mode);
                arsenal_stats_inc_passwords();
            }});
            options.push_back({"Regenerate", [lengths, lengthIdx, mode, &current]() {
                current = generatePassword(lengths[lengthIdx], mode);
                arsenal_stats_inc_passwords();
            }});
            options.push_back({"Save to /arsenal/passwords.txt", [&current]() {
                if (!setupSdCard()) {
                    displayRedStripe("SD card required");
                    delay(1000);
                    return;
                }
                if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
                File f = SD.open("/arsenal/passwords.txt", FILE_APPEND);
                if (!f) { displayRedStripe("Open failed"); delay(1000); return; }
                f.println(current);
                f.close();
                displayRedStripe("Saved");
                delay(1000);
            }});
            options.push_back({"Back", []() {}});

            drawMainBorderWithTitle("Password Gen");
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(8, 36);
            tft.printf("[%s] [%s]", lengthLabels[lengthIdx], modeLabels[mode]);

            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setTextSize(FM);
            int charW = 12;
            int maxLine = (tftWidth - 16) / charW;
            int len = current.length();
            int startY = 60;
            for (int i = 0; i < len; i += maxLine) {
                tft.setCursor(8, startY + (i / maxLine) * 18);
                tft.print(current.substring(i, min(len, i + maxLine)));
            }

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.drawCentreString("Sel=menu  Esc=exit", tftWidth / 2, tftHeight - 10, 1);

            if (check(SelPress)) {
                while (check(SelPress)) delay(10);
                addOptionToMainMenu();
                loopOptions(options, MENU_TYPE_SUBMENU, "Password Gen");
            } else if (check(EscPress)) {
                while (check(EscPress)) delay(10);
                return;
            }
            delay(50);
        }
    });
}
