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
// class lora {
#define BAND 434500000.00
#define spreadingFactor 9
#define SignalBandwidth 31.25E3
#define codingRateDenominator 8
#define preambleLength 8

// missing config im stupid
#define displayName "Brucetest"
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
            String wholeFile = file.readStringUntil('\n'); // Read one line up to newline
            tft.drawString(wholeFile, 10, yPos);           // Print line at (10, yPos)
            yPos += ySpacing;                              // Move down for next line
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
    }
    tft.fillScreen(TFT_BLACK);
    update = true;
    Serial.println("Initializing LoRa...");
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
    LoRa.begin(BAND);
    LoRa.setSpreadingFactor(spreadingFactor);
    LoRa.setSignalBandwidth(SignalBandwidth);
    LoRa.setCodingRate4(codingRateDenominator);
    LoRa.setPreambleLength(preambleLength);
    Serial.println("LoRa Started");
    mainloop();
}

void loraconf() {
    // unfinished
    Serial.println("testingconf");
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(bruceConfig.bgColor);
    tft.setTextSize(1);
    tft.drawString("LoRa Config maybe idk", 10, 10);
}

void test() {
    // nothing
}

