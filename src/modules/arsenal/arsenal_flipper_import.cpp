#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <globals.h>

static bool parseFlipperSub(const char *path, uint16_t **durations, int *count) {
    File f = SD.open(path, FILE_READ);
    if (!f) return false;

    int maxSamples = 2048;
    *durations = (uint16_t *)malloc(maxSamples * sizeof(uint16_t));
    if (!durations) { f.close(); return false; }

    *count = 0;
    bool inData = false;

    while (f.available() && *count < maxSamples) {
        String line = f.readStringUntil('\n');
        line.trim();
        if (line.startsWith("Filetype:")) continue;
        if (line.startsWith("Version:")) continue;
        if (line.startsWith("Frequency:")) continue;
        if (line.startsWith("Preset:")) continue;
        if (line.startsWith("Name:")) continue;
        if (line.startsWith("")) continue;

        if (line.startsWith("RAW_Data:")) {
            inData = true;
            String data = line.substring(9);
            data.trim();
            int idx = 0;
            while (idx < data.length() && *count < maxSamples) {
                while (idx < data.length() && data[idx] == ' ') idx++;
                int start = idx;
                while (idx < data.length() && data[idx] != ' ') idx++;
                if (idx > start) {
                    int val = data.substring(start, idx).toInt();
                    (*durations)[*count] = (uint16_t)abs(val);
                    (*count)++;
                }
            }
            continue;
        }

        if (inData) {
            int idx = 0;
            while (idx < line.length() && *count < maxSamples) {
                while (idx < line.length() && line[idx] == ' ') idx++;
                int start = idx;
                while (idx < line.length() && line[idx] != ' ') idx++;
                if (idx > start) {
                    int val = line.substring(start, idx).toInt();
                    (*durations)[*count] = (uint16_t)abs(val);
                    (*count)++;
                }
            }
        }
    }

    f.close();
    return (*count > 0);
}

void arsenal_flipper_import(void) {
    ARSENAL_HEAP_CHECK();
    if (!setupSdCard()) {
        displayRedStripe("SD card required");
        delay(1500);
        return;
    }

    File dir = SD.open("/arsenal/subghz");
    if (!dir || !dir.isDirectory()) {
        displayRedStripe("No /arsenal/subghz/");
        delay(1500);
        return;
    }

    options.clear();
    File entry = dir.openNextFile();
    int fileCount = 0;
    while (entry && fileCount < 15) {
        String name = String(entry.name());
        name = name.substring(name.lastIndexOf('/') + 1);
        if (name.endsWith(".sub")) {
            String label = name;
            options.push_back({label, []() {}});
            fileCount++;
        }
        entry = dir.openNextFile();
    }
    dir.close();

    if (fileCount == 0) {
        displayRedStripe("No .sub files found");
        delay(1500);
        return;
    }

    int selectedIdx = -1;
    for (int i = 0; i < fileCount; i++) {
        options[i].operation = [i, &selectedIdx]() { selectedIdx = i; };
    }
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Flipper .sub files");

    if (selectedIdx < 0) return;

    dir = SD.open("/arsenal/subghz");
    entry = dir.openNextFile();
    String selectedPath = "";
    int count = 0;
    while (entry) {
        String name = String(entry.name());
        name = name.substring(name.lastIndexOf('/') + 1);
        if (name.endsWith(".sub")) {
            if (count == selectedIdx) {
                selectedPath = String(entry.name());
                break;
            }
            count++;
        }
        entry = dir.openNextFile();
    }
    dir.close();

    if (selectedPath.length() == 0) return;

    uint16_t *durations = nullptr;
    int durCount = 0;

    drawMainBorderWithTitle("Flipper Import");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Loading .sub file...");

    if (!parseFlipperSub(selectedPath.c_str(), &durations, &durCount)) {
        displayRedStripe("Failed to parse file");
        delay(1500);
        return;
    }

    drawMainBorderWithTitle("Flipper Import");
    int y = 40;
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, y);
    tft.printf("File: %s", selectedPath.substring(selectedPath.lastIndexOf('/') + 1).c_str());
    y += 14;
    tft.setCursor(12, y);
    tft.printf("Samples: %d", durCount);
    y += 14;
    tft.print("Ready to replay");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:replay  Sel:done", tftWidth / 2, tftHeight - 20, 1);

    while (!check(EscPress) && !check(SelPress)) delay(100);

    if (check(EscPress)) {
        while (check(EscPress)) delay(10);
        if (initRfModule("tx", 433.92)) {
            drawMainBorderWithTitle("Flipper Import");
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, 50);
            tft.print("Replaying...");
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:stop", tftWidth / 2, tftHeight - 20, 1);

            for (int r = 0; r < 5; r++) {
                if (check(EscPress)) break;
                int pin = bruceConfigPins.rfModule == CC1101_SPI_MODULE ?
                    bruceConfigPins.CC1101_bus.io0 : bruceConfigPins.rfTx;
                bool high = true;
                for (int i = 0; i < durCount; i++) {
                    if (check(EscPress)) break;
                    digitalWrite(pin, high ? HIGH : LOW);
                    delayMicroseconds(durations[i]);
                    high = !high;
                }
                digitalWrite(pin, LOW);
                delay(500);
            }
            deinitRfModule();
        }
    }

    free(durations);
}
