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

/*
// Includes
//#include <Wire.h>
#include <SSD1306Wire.h> // you need to install the ESP8266 oled driver for SSD1306
// by Daniel Eichorn and Fabrice Weinberg to get this file!
// It's in the arduino library manager :-D

#include <SPI.h>
#include <LoRa.h>    // this is the one by Sandeep Mistry,
// (also in the Arduino Library manager :D )

// display descriptor
SSD1306Wire display(0x3c, 4, 15);

// definitions
//SPI defs for LoRa radio
#define SS 33
#define RST -1
#define DI0 32
// #define BAND 429E6

// LoRa Settings
#define BAND 434500000.00
#define spreadingFactor 9
// #define SignalBandwidth 62.5E3
#define SignalBandwidth 31.25E3
#define codingRateDenominator 8
#define preambleLength 8

// we also need the following config data:
// GPIO47 — SX1278’s SCK
// GPIO45 — SX1278’s MISO
// GPIO48 — SX1278’s MOSI
// GPIO21 — SX1278’s CS
// GPIO14 — SX1278’s RESET
// GPIO26 — SX1278’s IRQ(Interrupt Request)

// misc vars
String msg;
String displayName;
String sendMsg;
char chr;
int i = 0;

// Helpers func.s for LoRa
String mac2str(int mac)
{
 String s;
 byte *arr = (byte*) &mac;
 for (byte i = 0; i < 6; ++i)
 {
   char buff[3];
   // yea, this is a sprintf... fite me...
   sprintf(buff, "%2X", arr[i]);
   s += buff;
   if (i < 5) s += ':';
 }
 return s;
}

// let's set stuff up!
void setup() {
 // reset the screen
 pinMode(16, OUTPUT);
 digitalWrite(16, LOW); // set GPIO16 low to reset OLED
 delay(50);
 digitalWrite(16, HIGH);
 Serial.begin(115200);
 while (!Serial); //If just the the basic function, must connect to a computer

 // Initialising the UI will init the display too.
 display.init();
 display.flipScreenVertically();
 display.setFont(ArialMT_Plain_10);
 display.setTextAlignment(TEXT_ALIGN_LEFT);
 display.drawString(5, 5, "LoRa Chat Node");
 display.display();

 SPI.begin(0, 36, 26, 33);
 LoRa.setPins(SS, RST, DI0);
 Serial.println("LoRa Chat Node");
 if (!LoRa.begin(BAND)) {
   Serial.println("Starting LoRa failed!");
   while (1);
 }

 Serial.print("LoRa Spreading Factor: ");
 Serial.println(spreadingFactor);
 LoRa.setSpreadingFactor(spreadingFactor);

 Serial.print("LoRa Signal Bandwidth: ");
 Serial.println(SignalBandwidth);
 LoRa.setSignalBandwidth(SignalBandwidth);

 LoRa.setCodingRate4(codingRateDenominator);

 LoRa.setPreambleLength(preambleLength);

 Serial.println("LoRa Initial OK!");
 display.drawString(5, 20, "LoRaChat is running!");
 display.display();
 delay(2000);
 Serial.println("Welcome to LoRaCHAT!");
 // get MAC as initial Nick
 int MAC = ESP.getEfuseMac();
 displayName = mac2str(MAC);
 //displayName.trim(); // remove the newline
 Serial.print("Initial nick is "); Serial.println(displayName);
 Serial.println("Use /? for command help...");
 Serial.println(": ");
 display.clear();
 display.drawString(5, 20, "Nickname set:");
 display.drawString(20, 30, displayName);
 display.display();
 delay(1000);
}

void loop() {
 // Receive a message first...
 int packetSize = LoRa.parsePacket();
 if (packetSize) {
   display.clear();
   display.drawString(3, 0, "Received Message!");
   display.display();
   while (LoRa.available()) {
     String data = LoRa.readString();
     display.drawString(20, 22, data);
     display.display();
     Serial.println(data);
   }
 } // once we're done there, we read bytes from Serial
 if (Serial.available()) {
   chr = Serial.read();
   Serial.print(chr); // so the user can see what they're doing :P
   if (chr == '\n' || chr == '\r') {
     msg += chr; //msg+='\0'; // should maybe terminate my strings properly....
     if (msg.startsWith("/")) {
       // clean up msg string...
       msg.trim(); msg.remove(0, 1);
       // process command...
       char cmd[1]; msg.substring(0, 1).toCharArray(cmd, 2);
       switch (cmd[0]){
         case '?':
           Serial.println("Supported Commands:");
           Serial.println("h - this message...");
           Serial.println("n - change Tx nickname...");
           Serial.println("d - print Tx nickname...");
           break;
         case 'n':
           displayName = msg.substring(2);
           Serial.print("Display name set to: "); Serial.println(displayName);
           break;
         case 'd':
           Serial.print("Your display name is: "); Serial.println(displayName);
           break;
         default:
           Serial.println("command not known... use 'h' for help...");
       }
       msg = "";
     }
     else {
       // ssshhhhhhh ;)
       Serial.print("Me: "); Serial.println(msg);
       // assemble message
       sendMsg += displayName;
       sendMsg += "> ";
       sendMsg += msg;
       // send message
       LoRa.beginPacket();
       LoRa.print(sendMsg);
       LoRa.endPacket();
       display.clear();
       display.drawString(1, 0, sendMsg);
       display.display();
       // clear the strings and start again :D
       msg = "";
       sendMsg = "";
       Serial.print(": ");
     }
   }
   else {
     msg += chr;
   }
 }
}


#define LORA_SCK 0
#define LORA_MISO 26
#define LORA_MOSI 25
#define LORA_CS 33
#define LORA_RST -1
#define LORA_DIO0 32



*/
