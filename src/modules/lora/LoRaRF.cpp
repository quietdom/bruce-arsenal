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
#include <vector>
bool update = false;
String msg;
String rcvmsg;
String displayName = "Brucetest";

// scrolling thing
std::vector<String> messages;
int scrollOffset = 0;
const int maxMessages = 10;

#define spreadingFactor 9
#define SignalBandwidth 31.25E3
#define codingRateDenominator 8
#define preambleLength 8

// missing config im stupid
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
            // make the rollable thingy
            // append to littlefs
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
    tft.drawString("Lora Chat", 10, yStart);

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
/*
 while (file.available()) {
            tft.setTextSize(1);
            tft.setTextColor(bruceConfig.priColor);
            String wholeFile = file.readStringUntil('\n');
            tft.drawString(wholeFile, 10, yPos);
            yPos += ySpacing;
        }
*/

void rendersetting() {
    // thing
}
// optional call funcs
void sendmsg() {
    Serial.println("C bttn");
    tft.fillScreen(TFT_BLACK);
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

    messages.push_back(msg);
    if (messages.size() > maxMessages) { scrollOffset = messages.size() - maxMessages; }
    msg = "";
}

void upress() {
    Serial.println("Up Pressed");
    if (scrollOffset > 0) {
        scrollOffset--;
        Serial.println("Scroll Up");
        update = true;
    }
}

void downpress() {
    Serial.println("Down Pressed");
    if (scrollOffset < messages.size() - maxMessages) {
        scrollOffset++;
        Serial.println("Scroll Down");
        update = true;
    }
}
/*void checkKeypresses() {
    if (check(SelPress)) sendmsg();
    if (UpPress) upress();
    if (DownPress) downpress();
}
*/
void mainloop() {
    long pressStartTime = 0;
    bool isPressing = false;

    while (true) {
        render();
        reciveMessage();

#if !defined(HAS_KEYBOARD) && !defined(HAS_ENCODER)
        // This is for devices like the M5StickC where one button has two functions.
        if (check(EscPress)) {
            // A press just started. Record the time and set a flag.
            isPressing = true;
            pressStartTime = millis();
        }

        if (isPressing && !EscPress) {
            // The button was just released.
            isPressing = false;
            if (millis() - pressStartTime < 700) {
                // It was a SHORT press because it was released quickly.
                upress();
            }
        }

        if (isPressing && (millis() - pressStartTime > 700)) {
            // The button is STILL being held down after 700ms.
            // This is a LONG press. Exit the loop.
            break;
        }
#else
        // This is for devices with dedicated keys (Cardputer, etc.)
        if (check(EscPress)) break;
        if (check(PrevPress)) upress();
#endif

        if (check(SelPress)) sendmsg();
        if (check(NextPress)) downpress();

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
    Serial.println(
        "Pins: SCK:" + String(LORA_SCK) + " MISO:" + String(LORA_MISO) + " MOSI:" + String(LORA_MOSI) +
        " CS:" + String(LORA_CS) + " RST:" + String(LORA_RST) + " DIO0:" + String(LORA_DIO0)
    );
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
    loadMessages();
    mainloop();
}

// settings

/*
void loraconf() {
    Serial.println("testingconf");
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(bruceConfig.bgColor);
    tft.setTextSize(1);
    tft.drawString("LoRa Config maybe idk", 10, 10);
}
*/

void changeusername() {
    tft.fillScreen(TFT_BLACK);
    String username = keyboard(username, 64, "");
    if (username == "") return;
    File file = LittleFS.open("/lora_settings.json", "w");
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
    String freq = keyboard(freq, 12, "");
    double dfreq = freq.toDouble();
    if (dfreq == 0) return;
    File file = LittleFS.open("/lora_settings.json", "w");
    StaticJsonDocument<128> doc;
    deserializeJson(doc, file);
    file.close();
    doc["LoRa_Frequency"] = dfreq;
    serializeJson(doc, file);
    file.close();
}
