#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/wifi/wifi_common.h"
#include <WiFi.h>
#include <globals.h>
#include <lwip/etharp.h>
#include <lwip/netif.h>


static const uint16_t SCAN_PORTS[] = {
    21, 22, 23, 25, 53, 80, 110, 139, 143, 443, 445, 993, 995, 3389, 8080, 8443
};
static const int NUM_PORTS = sizeof(SCAN_PORTS) / sizeof(SCAN_PORTS[0]);

struct HostInfo {
    IPAddress ip;
    uint8_t mac[6];
    String vendor;
    std::vector<uint16_t> openPorts;
};

static std::vector<HostInfo> discoveredHosts;


extern String oui_lookup_vendor(uint8_t *mac);

static bool tcpPortOpen(IPAddress ip, uint16_t port, int timeoutMs = 200) {
    WiFiClient client;
    client.setTimeout(timeoutMs);
    bool open = client.connect(ip, port, timeoutMs);
    client.stop();
    return open;
}

static void arpScan(IPAddress gateway, IPAddress subnet) {
    discoveredHosts.clear();

    uint32_t gw = (uint32_t)gateway;
    uint32_t mask = (uint32_t)subnet;
    uint32_t network = gw & mask;
    uint32_t broadcast = network | ~mask;
    uint32_t totalHosts = broadcast - network - 1;

    if (totalHosts > 254) totalHosts = 254;

    drawMainBorderWithTitle("Network Scan");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);

    for (uint32_t i = 1; i <= totalHosts; i++) {
        if (check(EscPress)) { returnToMenu = true; return; }

        IPAddress target(
            (network >> 0) & 0xFF,
            (network >> 8) & 0xFF,
            (network >> 16) & 0xFF,
            ((network >> 0) & 0xFF) + i
        );


        uint32_t hostIP = network + i;
        target = IPAddress(
            (hostIP) & 0xFF,
            (hostIP >> 8) & 0xFF,
            (hostIP >> 16) & 0xFF,
            (hostIP >> 24) & 0xFF
        );


        WiFiClient client;
        client.setTimeout(50);
        bool alive = client.connect(target, 80, 50);
        if (!alive) alive = client.connect(target, 443, 50);
        client.stop();

        if (alive) {
            HostInfo host;
            host.ip = target;
            memset(host.mac, 0, 6);

            ip4_addr_t ipAddr;
            ipAddr.addr = (uint32_t)target;
            struct eth_addr *eth_ret = NULL;
            const ip4_addr_t *ip_ret = NULL;
            if (etharp_find_addr(netif_default, &ipAddr, &eth_ret, &ip_ret) >= 0 && eth_ret) {
                memcpy(host.mac, eth_ret->addr, 6);
                host.vendor = oui_lookup_vendor(host.mac);
            } else {
                host.vendor = "?";
            }
            discoveredHosts.push_back(host);
        }


        if (i % 10 == 0) {
            tft.fillRect(12, tftHeight - 30, tftWidth - 24, 16, bruceConfig.bgColor);
            tft.setCursor(12, tftHeight - 30);
            tft.printf("Scanning %d/%d | Found: %d", (int)i, (int)totalHosts, (int)discoveredHosts.size());
        }

        esp_task_wdt_reset();
        delay(5);
    }
}

static void portScanHost(HostInfo &host) {
    host.openPorts.clear();
    for (int i = 0; i < NUM_PORTS; i++) {
        if (check(EscPress)) { returnToMenu = true; return; }
        if (tcpPortOpen(host.ip, SCAN_PORTS[i], 150)) {
            host.openPorts.push_back(SCAN_PORTS[i]);
        }
        esp_task_wdt_reset();
    }
}

void arsenal_network_scanner(void) {
    ARSENAL_SAFE_RUN([]() {

        if (!wifiConnected) {
            if (!wifiConnectMenu()) return;
        }

        IPAddress gateway = WiFi.gatewayIP();
        IPAddress subnet = WiFi.subnetMask();


        arpScan(gateway, subnet);

        if (discoveredHosts.empty()) {
            displayRedStripe("No hosts found");
            delay(1500);
            return;
        }


        options.clear();
        for (size_t i = 0; i < discoveredHosts.size(); i++) {
            HostInfo &h = discoveredHosts[i];
            String label = h.ip.toString() + " [" + h.vendor.substring(0, 10) + "]";
            options.push_back({label, [&h, i]() {

                drawMainBorderWithTitle("Port Scan");
                tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
                tft.setTextSize(FP);
                tft.setCursor(12, 50);
                tft.print("Scanning " + discoveredHosts[i].ip.toString());

                portScanHost(discoveredHosts[i]);


                std::vector<Option> portOpts;
                if (discoveredHosts[i].openPorts.empty()) {
                    portOpts.push_back({"No open ports found", []() {}});
                } else {
                    for (uint16_t port : discoveredHosts[i].openPorts) {
                        portOpts.push_back({"Port " + String(port) + " OPEN", []() {}});
                    }
                }
                addOptionToMainMenu();
                loopOptions(portOpts, MENU_TYPE_SUBMENU, "Open Ports");
            }});
        }

        addOptionToMainMenu();
        loopOptions(options, MENU_TYPE_SUBMENU, "Hosts Found");
    });
}
