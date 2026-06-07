#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <globals.h>

static const char *UPNP_MSEARCH =
    "M-SEARCH * HTTP/1.1\r\n"
    "HOST:239.255.255.250:1900\r\n"
    "ST:urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n"
    "MAN:\"ssdp:discover\"\r\n"
    "MX:3\r\n\r\n";

static const char *ADD_PORT_TEMPLATE =
    "POST /tr0d4igd/control/InternetGatewayDevice/WANIPConnection/1 HTTP/1.1\r\n"
    "Host: %s:49152\r\n"
    "Content-Type: text/xml; charset=\"utf-8\"\r\n"
    "SOAPAction: \"urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\"\r\n"
    "Content-Length: %d\r\n\r\n"
    "%s";

struct UpnpDevice {
    char ip[16];
    int port;
    char friendlyName[64];
};

static UpnpDevice devices[8];
static int deviceCount = 0;
static int portsOpened = 0;

void arsenal_upnp_port_opener(void) {
    ARSENAL_HEAP_CHECK();
    if (WiFi.getMode() != WIFI_STA || WiFi.status() != WL_CONNECTED) {
        displayRedStripe("WiFi not connected");
        delay(1500);
        return;
    }

    deviceCount = 0;
    portsOpened = 0;

    drawMainBorderWithTitle("UPnP Opener");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.print("Discovering UPnP devices...");

    WiFiUDP udp;
    udp.begin(1900);

    for (int m = 0; m < 3; m++) {
        udp.beginPacket(IPAddress(239, 255, 255, 250), 1900);
        udp.write((const uint8_t *)UPNP_MSEARCH, strlen(UPNP_MSEARCH));
        udp.endPacket();
        delay(200);
    }

    unsigned long start = millis();
    while (millis() - start < 3000 && deviceCount < 8) {
        int size = udp.parsePacket();
        if (size > 0) {
            char buf[512];
            int len = udp.read(buf, sizeof(buf) - 1);
            buf[len] = '\0';
            IPAddress rip = udp.remoteIP();
            int rport = udp.remotePort();

            bool found = false;
            for (int i = 0; i < deviceCount; i++) {
                if (strcmp(devices[i].ip, rip.toString().c_str()) == 0) { found = true; break; }
            }
            if (!found && deviceCount < 8) {
                strncpy(devices[deviceCount].ip, rip.toString().c_str(), 15);
                devices[deviceCount].port = rport;
                char *name = strstr(buf, "Server:");
                if (name) {
                    name += 7;
                    while (*name == ' ') name++;
                    int nl = 0;
                    while (name[nl] != '\r' && name[nl] != '\0' && nl < 63) nl++;
                    strncpy(devices[deviceCount].friendlyName, name, nl);
                    devices[deviceCount].friendlyName[nl] = '\0';
                } else {
                    strcpy(devices[deviceCount].friendlyName, "Unknown UPnP");
                }
                deviceCount++;
            }
        }
    }
    udp.stop();

    if (deviceCount == 0) {
        displayRedStripe("No UPnP devices found");
        delay(1500);
        return;
    }

    options.clear();
    for (int i = 0; i < deviceCount; i++) {
        String label = String(devices[i].friendlyName) + " (" + String(devices[i].ip) + ")";
        options.push_back({label, [i]() {}});
    }
    int targetIdx = -1;
    for (int i = 0; i < deviceCount; i++) {
        options[i].operation = [i, &targetIdx]() { targetIdx = i; };
    }
    loopOptions(options, MENU_TYPE_SUBMENU, "Select Device");

    if (targetIdx < 0) return;

    UpnpDevice &dev = devices[targetIdx];
    char soapBody[512];
    snprintf(soapBody, sizeof(soapBody),
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
        "<s:Body><u:AddPortMapping xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\">"
        "<NewRemoteHost></NewRemoteHost><NewExternalPort>31337</NewExternalPort>"
        "<NewProtocol>TCP</NewProtocol><NewInternalPort>80</NewInternalPort>"
        "<NewInternalClient>%s</NewInternalClient><NewEnabled>1</NewEnabled>"
        "<NewPortMappingDescription>Arsenal</NewPortMappingDescription><NewLeaseDuration>0</NewLeaseDuration>"
        "</u:AddPortMapping></s:Body></s:Envelope>", WiFi.localIP().toString().c_str());

    char soapMsg[1024];
    snprintf(soapMsg, sizeof(soapMsg), ADD_PORT_TEMPLATE, dev.ip, (int)strlen(soapBody), soapBody);

    WiFiClient client;
    if (client.connect(dev.ip, 49152)) {
        client.write((const uint8_t *)soapMsg, strlen(soapMsg));
        delay(200);
        client.stop();
        portsOpened++;
    }

    drawMainBorderWithTitle("UPnP Opener");
    tft.setTextColor(TFT_GREEN, bruceConfig.priColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Device: %s", dev.friendlyName);
    tft.setCursor(12, 66);
    tft.print("Port 31337 opened!");
    tft.setCursor(12, 82);
    tft.printf("IP: %s", dev.ip);
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);

    while (!check(EscPress)) delay(100);
}
