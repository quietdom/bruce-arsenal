#ifndef WIFI_DEAUTHER_H
#define WIFI_DEAUTHER_H

#include "clients.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/net_utils.h"
#include "core/utils.h"
#include "core/wifi/wifi_common.h"
#include "core/wifi/webInterface.h"
#include "scan_hosts.h"
#include "wifi_atks.h"
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <globals.h>
#include <iomanip>
#include <iostream>
#include <lwip/dns.h>
#include <lwip/err.h>
#include <lwip/etharp.h>
#include <lwip/igmp.h>
#include <lwip/inet.h>
#include <lwip/init.h>
#include <lwip/ip_addr.h>
#include <lwip/mem.h>
#include <lwip/memp.h>
#include <lwip/netif.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/timeouts.h>
#include <modules/wifi/sniffer.h>
#include <sstream>
#include <vector>

// Expanded deauth reason codes for better effectiveness
inline const uint8_t DEAUTH_REASONS[] = {
    0x01, // Unspecified
    0x04, // Disassociated due to inactivity
    0x06, // Class 2 frame from non-authenticated STA
    0x07, // Class 3 frame from non-associated STA
    0x08, // Sending STA is leaving
    0x0A, // Requested by AP
    0x0D, // Invalid PMKID
    0x0F, // 4-way handshake timeout
    0x12, // Disassociated due to AP resource
    0x28  // SA Query timeout
};
inline const int DEAUTH_REASON_COUNT = sizeof(DEAUTH_REASONS) / sizeof(DEAUTH_REASONS[0]);

// Global deauth state for burst control and adaptive behavior
inline struct {
    uint8_t reason_index = 0;
    uint32_t burst_counter = 0;
    bool storm_active = false;
    uint32_t last_burst_time = 0;
    uint8_t consecutive_failures = 0;
} deauth_state;

// APs with same SSID for mesh network targeting
struct APInfo {
    uint8_t bssid[6];
    int channel;
};
inline std::vector<APInfo> sameSSID_APs;

void getGatewayMAC(uint8_t gatewayMAC[6]);
bool isMACZero(const uint8_t* mac);
bool macCompare(const uint8_t* mac1, const uint8_t* mac2);
void cacheSameSSIDAPs();
bool ensureWiFiConnected();
int getAPChannel(const uint8_t* target_bssid);
bool tryMonitorMode(uint8_t channel);
void buildOptimizedDeauthFrame(uint8_t* frame,
                              const uint8_t* dest,
                              const uint8_t* src,
                              const uint8_t* bssid,
                              uint8_t reason = 0x07,
                              bool is_disassoc = false);
void sendDeauthFrame(const uint8_t* frame, bool enhanced_mode);
void sendDeauthBurst(uint8_t* frame1, uint8_t* frame2, uint8_t* frame3, uint8_t* frame4, bool enhanced_mode, uint8_t reason);
void stationDeauth(Host host);
void deauthAll();
void deauthTargetList(const std::vector<Host>& targets);

#endif
