#include "LoRaRF.h"
#include "WString.h"
#include "core/config.h"
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <LoRa.h>
#include <core/display.h>
#include <core/mykeyboard.h>
#include <core/utils.h>
#include <globals.h>
bool update = false;
String msg;
String rcvmsg;
String screendata;
String displayName = "Brucetest";
// class lora {
// define BAND 434500000.00
#define spreadingFactor 9
#define SignalBandwidth 31.25E3
#define codingRateDenominator 8
#define preambleLength 8

// missing config im stupid
int animtest = -120;
int contentWidth = tftWidth - 20;
int yStart = 35;
int yPos = yStart;
int ySpacing = 10;
int rightColumnX = tftWidth / 2 + 10;

void reciveMessage() {
    if (LoRa.available()) {
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
            while (LoRa.available()) { rcvmsg += (char)LoRa.read(); }
            Serial.println(rcvmsg);
            update = true;
            // make the rollable thingy
            // append to littlefs
            File file = LittleFS.open("/chats.txt", "a");
            file.println(rcvmsg);
            file.close();
            rcvmsg = "";
        }
    }
}

// render stuff

void render() {
    delay(100);
    tft.setTextSize(1);
    // tft.setFreeFont();
    tft.setTextColor(0x6DFC);
    tft.drawString("Lora Chat", 10, yStart);
    if (update) {
        // fix this
        File file = LittleFS.open("/chats.txt", "r");
        while (file.available()) {
            tft.setTextSize(1);
            tft.setTextColor(bruceConfig.priColor);
            String wholeFile = file.readStringUntil('\n');
            tft.drawString(wholeFile, 10, yPos);
            yPos += ySpacing;
        }
        file.close();
        update = false;
    }
}
void rendersetting() {
    // thing
}
// optional call funcs
void sendmsg() {
    tft.fillScreen(TFT_BLACK);
    msg = keyboard(msg, 256, "Message:");
    msg = String(displayName) + ": " + msg;
    Serial.println(msg);
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
    tft.fillScreen(TFT_BLACK);
    update = true;
    File file = LittleFS.open("/chats.txt", "a");
    file.println(msg);
    file.close();
    msg = "";
}

void checkKeypresses() {
    if (check(SelPress)) sendmsg();
}

void mainloop() {
    while (!check(EscPress)) {
        render();
        reciveMessage();
        checkKeypresses();
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
        doc["LoRa_Frequency"] = 434500000.00;
        doc["LoRa_Name"] = displayName;
        serializeJson(doc, file);
        file.close();
    }
    File file = LittleFS.open("/lora_settings.json", "r");
    StaticJsonDocument<128> doc;
    deserializeJson(doc, file);
    String displayName = doc["LoRa_Name"];
    double BAND = doc["LoRa_Frequency"];
    file.close();
    tft.fillScreen(TFT_BLACK);
    update = true;
    Serial.println("Initializing LoRa...");
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        return;
    }
    LoRa.setSpreadingFactor(spreadingFactor);
    LoRa.setSignalBandwidth(SignalBandwidth);
    LoRa.setCodingRate4(codingRateDenominator);
    LoRa.setPreambleLength(preambleLength);
    Serial.println("LoRa Started");
    mainloop();
}

// settings

void loraconf() {
    // unfinished
    Serial.println("testingconf");
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(bruceConfig.bgColor);
    tft.setTextSize(1);
    tft.drawString("LoRa Config maybe idk", 10, 10);
}

/*
void Changeusername() {
    tft.fillScreen(TFT_BLACK);
    String newname = keyboard(bruceConfig.username, 32, "New Username:")
}
    */
