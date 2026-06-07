#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "modules/ir/ir_utils.h"
#include <globals.h>

#define IR_CHUNK_SIZE 96

static void sendFileViaIR(const String &path) {
    File f = SD.open(path, FILE_READ);
    if (!f) {
        displayRedStripe("File not found");
        delay(1500);
        return;
    }

    uint32_t fileSize = f.size();
    IRsend irsend(bruceConfigPins.irTx);
    irsend.begin();
    setup_ir_pin(bruceConfigPins.irTx, OUTPUT);

    drawMainBorderWithTitle("IR Transfer");
    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Sending: %s", path.substring(path.lastIndexOf('/') + 1).c_str());
    tft.setCursor(12, 66);
    tft.printf("Size: %d bytes", (int)fileSize);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:cancel", tftWidth / 2, tftHeight - 20, 1);

    uint8_t chunk[IR_CHUNK_SIZE];
    uint32_t sent = 0;
    unsigned long startTime = millis();

    uint16_t header[8];
    header[0] = 0x00FF;
    header[1] = 0x00FF;
    header[2] = (fileSize >> 24) & 0xFF;
    header[3] = (fileSize >> 16) & 0xFF;
    header[4] = (fileSize >> 8) & 0xFF;
    header[5] = fileSize & 0xFF;
    header[6] = 0x00;
    header[7] = 0x00;
    for (int i = 0; i < 6; i++) header[7] ^= header[i];
    irsend.sendRaw(header, 8, 38000);
    delay(100);

    while (f.available() && !check(EscPress)) {
        int bytesRead = f.read(chunk, IR_CHUNK_SIZE);
        if (bytesRead <= 0) break;

        uint16_t rawData[IR_CHUNK_SIZE * 8 + 16];
        int rawIdx = 0;

        rawData[rawIdx++] = 0x00FF;
        rawData[rawIdx++] = 0x00FF;
        rawData[rawIdx++] = 0xAA55;

        uint8_t checksum = 0;
        for (int i = 0; i < bytesRead; i++) checksum ^= chunk[i];
        rawData[rawIdx++] = (checksum << 8) | bytesRead;

        for (int i = 0; i < bytesRead; i++) {
            for (int b = 7; b >= 0; b--) {
                if (chunk[i] & (1 << b))
                    rawData[rawIdx++] = 500;
                else
                    rawData[rawIdx++] = 1000;
            }
        }

        irsend.sendRaw(rawData, rawIdx, 38000);
        sent += bytesRead;
        delay(15);

        if (sent % (IR_CHUNK_SIZE * 10) == 0 || sent == fileSize) {
            drawMainBorderWithTitle("IR Transfer");
            int y = 45;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, y);
            tft.printf("Sent: %d / %d", (int)sent, (int)fileSize);
            y += 14;
            int pct = fileSize > 0 ? (int)((unsigned long)sent * 100 / fileSize) : 0;
            int barW = (tftWidth - 24) * pct / 100;
            tft.fillRect(12, y, barW, 10, TFT_GREEN);
            tft.drawRect(12, y, tftWidth - 24, 10, bruceConfig.priColor);
            y += 14;
            unsigned long elapsed = (millis() - startTime) / 1000;
            tft.setCursor(12, y);
            tft.printf("Time: %lus  %d%%", elapsed, pct);
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString("Esc:cancel", tftWidth / 2, tftHeight - 20, 1);
        }
        esp_task_wdt_reset();
    }

    f.close();
    digitalWrite(bruceConfigPins.irTx, LOW);

    drawMainBorderWithTitle("IR Transfer");
    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Transfer complete!");
    tft.setCursor(12, 66);
    tft.printf("Sent %d bytes", (int)sent);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
    while (!check(EscPress)) delay(100);
}

void arsenal_ir_data_transfer(void) {
    ARSENAL_HEAP_CHECK();
    if (!setupSdCard()) {
        displayRedStripe("SD card required");
        delay(1500);
        return;
    }

    File dir = SD.open("/arsenal");
    if (!dir || !dir.isDirectory()) {
        displayRedStripe("No /arsenal/ folder");
        delay(1500);
        return;
    }

    options.clear();
    int count = 0;
    File entry = dir.openNextFile();
    while (entry && count < 15) {
        String name = String(entry.name());
        name = name.substring(name.lastIndexOf('/') + 1);
        if (!entry.isDirectory() && name.length() > 0) {
            String label = name + " (" + String(entry.size()) + "B)";
            options.push_back({label, []() {}});
            count++;
        }
        entry = dir.openNextFile();
    }
    dir.close();

    if (count == 0) {
        displayRedStripe("No files to send");
        delay(1500);
        return;
    }

    int selIdx = -1;
    for (int i = 0; i < count; i++) {
        options[i].operation = [i, &selIdx]() { selIdx = i; };
    }
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Select File");

    if (selIdx < 0) return;

    dir = SD.open("/arsenal");
    entry = dir.openNextFile();
    int idx = 0;
    while (entry) {
        String name = String(entry.name());
        name = name.substring(name.lastIndexOf('/') + 1);
        if (!entry.isDirectory() && name.length() > 0) {
            if (idx == selIdx) {
                String path = String(entry.name());
                entry.close();
                dir.close();
                sendFileViaIR(path);
                return;
            }
            idx++;
        }
        entry = dir.openNextFile();
    }
    dir.close();
}
