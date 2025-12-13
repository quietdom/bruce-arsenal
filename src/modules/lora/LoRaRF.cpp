#if !defined(LITE_VERSION)
#include "LoRaRF.h"
#include "WString.h"
#include "core/config.h"
#include "core/configPins.h"
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <LoRa.h>
#include <core/display.h>
#include <core/mykeyboard.h>
#include <core/utils.h>
#include <globals.h>
#include <vector>

extern BruceConfigPins bruceConfigPins;

bool update = false;
String msg;
String rcvmsg;
String displayName;
bool intlora = true;
// scrolling thing
std::vector<String> messages;
int scrollOffset = 0;
const int maxMessages = 19;

#define spreadingFactor 9
#define SignalBandwidth 31.25E3
#define codingRateDenominator 8
#define preambleLength 8

int contentWidth = tftWidth - 20;
int yStart = 35;
int yPos = yStart;
int ySpacing = 10;
int rightColumnX = tftWidth / 2 + 10;

void reciveMessage() {
    if (LoRa.available()) {
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
            rcvmsg = "";
            while (LoRa.available()) { rcvmsg += (char)LoRa.read(); }
            Serial.println("Recived:" + rcvmsg);
            File file = LittleFS.open("/chats.txt", "a");
            file.println(rcvmsg);
            file.close();
            messages.push_back(rcvmsg);
            if (messages.size() > maxMessages) { scrollOffset = messages.size() - maxMessages; }
            update = true;
        }
    }
}

// render stuff

void render() {
    if (!update) return;
    tft.setTextSize(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(0x6DFC);
    if (!intlora) { tft.drawString("Lora Init Failed", 10, 13); }
    Serial.println(String(displayName));
    tft.drawString("USRN: " + String(displayName), 10, 25);

    int yPos = yStart;
    int endLine = scrollOffset + maxMessages;
    if (endLine > messages.size()) endLine = messages.size();
    for (int i = scrollOffset; i < endLine; i++) {
        tft.setTextColor(bruceConfig.priColor);
        tft.drawString(messages[i], 10, yPos);
        yPos += ySpacing;
    }
    update = false;
}

void loadMessages() {
    messages.clear();
    File file = LittleFS.open("/chats.txt", "r");
    while (file.available()) {
        String line = file.readStringUntil('\n');
        messages.push_back(line);
    }
    file.close();
    if (messages.size() > maxMessages) {
        scrollOffset = messages.size() - maxMessages;
    } else {
        scrollOffset = 0;
    }
}

// optional call funcs
void sendmsg() {
    Serial.println("C bttn");
    tft.fillScreen(TFT_BLACK);
    if (!intlora) {
        tft.setTextColor(bruceConfig.priColor);

        tft.setTextColor(TFT_RED);
        tft.setTextSize(2);
        tft.setCursor(10, tftHeight / 2 - 10);
        tft.print("LoRa not init!");

        tft.drawCentreString("LoRa not initialized!", tftWidth / 2, tftHeight / 2, 2);
        delay(1500);
        update = true;
        return;
    }
    msg = keyboard(msg, 256, "Message:");
    msg = String(displayName) + ": " + msg;
    if (msg == "") return;
    Serial.println(msg);
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
    tft.fillScreen(TFT_BLACK);
    update = true;
    File file = LittleFS.open("/chats.txt", "a");
    file.println(msg);
    file.close();

    messages.push_back(msg);
    if (messages.size() > maxMessages) { scrollOffset = messages.size() - maxMessages; }
    msg = "";
}

void upress() {
    Serial.println("Up Pressed");
    if (scrollOffset > 0) {
        scrollOffset--;
        update = true;
    }
}

void downpress() {
    Serial.println("Down Pressed");
    if (scrollOffset < messages.size() - maxMessages) {
        scrollOffset++;
        update = true;
    }
}

void mainloop() {
    long pressStartTime = 0;
    bool isPressing = false;
    bool breakloop = false;
    while (true) {
        render();
        reciveMessage();
        if (breakloop) { break; }
#ifndef HAS_KEYBOARD
        if (EscPress) {
            long _tmp = millis();

            LongPress = true;
            while (EscPress) {
                if (millis() - _tmp > 200) {
                    // start drawing arc after short delay; animate over 500ms
                    int sweep = 0;
                    long elapsed = millis() - (_tmp + 200);
                    if (elapsed > 0) sweep = 360 * elapsed / 500;
                    if (sweep > 360) sweep = 360;
                    tft.drawArc(
                        tftWidth / 2,
                        tftHeight / 2,
                        25,
                        15,
                        0,
                        sweep,
                        getColorVariation(bruceConfig.priColor),
                        bruceConfig.bgColor
                    );
                }
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
            // clear arc
            tft.drawArc(
                tftWidth / 2, tftHeight / 2, 25, 15, 0, 360, bruceConfig.bgColor, bruceConfig.bgColor
            );
            LongPress = false;
            // #endif

            // decide short vs long after release
            if (millis() - _tmp > 700) {
                // long press -> exit
                breakloop = true;
            } else {
                // short press -> scroll down; consume flag first
                check(EscPress);
                upress();
            }
        }

        if (check(NextPress)) downpress();
        if (check(SelPress)) sendmsg();
#else

        if (check(EscPress)) downpress();
        if (check(EscPress)) break;
        if (check(PrevPress)) upress();
        if (check(SelPress)) sendmsg();
#endif

        delay(20);
    }
}

void lorachat() {
    // set filesystem thing
    if (!LittleFS.exists("/chats.txt")) {
        File file = LittleFS.open("/chats.txt", "w");
        file.close();
        Serial.println("chat file created :)");
    }
    if (!LittleFS.exists("/lora_settings.json")) {
        Serial.println("creating lora settings .json file");
        StaticJsonDocument<128> doc;
        File file = LittleFS.open("/lora_settings.json", "w");
        doc["LoRa_Frequency"] = "434500000.00";
        doc["LoRa_Name"] = "BruceTest";
        serializeJson(doc, file);
        file.close();
    }
    File file = LittleFS.open("/lora_settings.json", "r");
    StaticJsonDocument<128> doc;
    deserializeJson(doc, file);
    displayName = doc["LoRa_Name"].as<String>();
    double BAND = doc["LoRa_Frequency"].as<String>().toDouble();
    file.close();
    tft.fillScreen(TFT_BLACK);
    update = true;
    Serial.println("Initializing LoRa...");
    Serial.println(
        "Pins: SCK:" + String(bruceConfigPins.LoRa_bus.sck) +
        " MISO:" + String(bruceConfigPins.LoRa_bus.miso) + " MOSI:" + String(bruceConfigPins.LoRa_bus.mosi) +
        " CS:" + String(bruceConfigPins.LoRa_bus.cs) + " RST:" + String(bruceConfigPins.LoRa_bus.io0) +
        " DIO0:" + String(bruceConfigPins.LoRa_bus.io2) + "BAND: " + String(BAND) +
        " DisplayName:  " + displayName
    );

    if (bruceConfigPins.LoRa_bus.sck == GPIO_NUM_NC || bruceConfigPins.LoRa_bus.miso == GPIO_NUM_NC ||
        bruceConfigPins.LoRa_bus.mosi == GPIO_NUM_NC || bruceConfigPins.LoRa_bus.cs == GPIO_NUM_NC) {
        Serial.println("LoRa pins not configured!");
        intlora = false;
        tft.drawString("LoRa pins not configured!", 10, 50);
        delay(2000);
        return;
    }

    if (bruceConfigPins.LoRa_bus.mosi == TFT_MOSI) {
#if TFT_MOSI > 0
        LoRa.setSPI(tft.getSPIinstance());
#endif
        Serial.println("Using TFT SPI for LoRa");
    } else if (bruceConfigPins.SDCARD_bus.mosi == bruceConfigPins.LoRa_bus.mosi) {
        LoRa.setSPI(sdcardSPI);
        Serial.println("Using SDCard SPI for LoRa");
    } else if (bruceConfigPins.NRF24_bus.mosi == bruceConfigPins.LoRa_bus.mosi ||
               bruceConfigPins.CC1101_bus.mosi == bruceConfigPins.LoRa_bus.mosi) {
        LoRa.setSPI(CC_NRF_SPI);
        CC_NRF_SPI.begin(
            (int8_t)bruceConfigPins.LoRa_bus.sck,
            (int8_t)bruceConfigPins.LoRa_bus.miso,
            (int8_t)bruceConfigPins.LoRa_bus.mosi
        );
        Serial.println("Using CC/NRF SPI for LoRa");
    } else {
        SPI.begin(
            bruceConfigPins.LoRa_bus.sck,
            bruceConfigPins.LoRa_bus.miso,
            bruceConfigPins.LoRa_bus.mosi,
            bruceConfigPins.LoRa_bus.cs
        );
        Serial.println("Using dedicated SPI for LoRa");
    }
    LoRa.setPins(bruceConfigPins.LoRa_bus.cs, bruceConfigPins.LoRa_bus.io0, bruceConfigPins.LoRa_bus.io2);
    // add return RETURN TO MENU THING **************
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        displayError("LoRa Init Failed (not SX127x)", true);
        intlora = false;
        return;
    }
    LoRa.setSpreadingFactor(spreadingFactor);
    LoRa.setSignalBandwidth(SignalBandwidth);
    LoRa.setCodingRate4(codingRateDenominator);
    LoRa.setPreambleLength(preambleLength);
    Serial.println("LoRa Started");
    tft.setTextWrap(true, true);
    tft.setTextDatum(TL_DATUM);
    loadMessages();
    mainloop();
}

// settings
// check the saving and loading
void changeusername() {
    tft.fillScreen(TFT_BLACK);
    String username = keyboard(username, 64, "");
    if (username == "") return;
    File file = LittleFS.open("/lora_settings.json", "r");
    StaticJsonDocument<128> doc;
    deserializeJson(doc, file);
    file.close();
    doc["LoRa_Name"] = username;
    file = LittleFS.open("/lora_settings.json", "w");
    serializeJson(doc, file);
    file.close();
}

void chfreq() {
    tft.fillScreen(TFT_BLACK);
    char buf[15];
    File file = LittleFS.open("/lora_settings.json", "r");
    StaticJsonDocument<128> doc;
    deserializeJson(doc, file);
    file.close();

    double dfreq = doc["LoRa_Frequency"].as<String>().toDouble();
    dfreq = dfreq / 1000000;
    snprintf(buf, sizeof(buf), "%.3f", dfreq);
    String freq = num_keyboard(buf, 12, "in Mhz");
    dfreq = freq.toDouble();
    if (dfreq == 0) {
        displayError("Invalid value");
        return;
    } else if (dfreq > 1000) {
        displayError("Invalid value, Exceeds 1Ghz");
        return;
    }
    dfreq = dfreq * 1000000;
    snprintf(buf, sizeof(buf), "%.2f", dfreq);
    doc["LoRa_Frequency"] = buf;

    file = LittleFS.open("/lora_settings.json", "w");
    serializeJson(doc, file);
    file.close();
}
#endif
