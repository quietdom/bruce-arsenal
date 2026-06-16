#include "WifiMenu.h"
#include "core/display.h"
#include "core/settings.h"
#include "core/utils.h"
#include "core/wifi/webInterface.h"
#include "core/wifi/wg.h"
#include "core/wifi/wifi_common.h"
#include "core/wifi/wifi_mac.h"
#include "modules/ethernet/ARPScanner.h"
#include "modules/wifi/ap_info.h"
#include "modules/wifi/clients.h"
#include "modules/wifi/evil_portal.h"
#include "modules/wifi/karma_attack.h"
#include "modules/wifi/netcut.h"
#include "modules/wifi/responder.h"
#include "modules/wifi/scan_hosts.h"
#include "modules/wifi/sniffer.h"
#include "modules/wifi/wifi_atks.h"
#include "modules/wifi/dhcp_starvation.h"
#include "modules/wifi/dns_spoofer.h"
#include "modules/wifi/cred_forward.h"

#if !LITE_VERSION
#include "modules/pwnagotchi/pwnagotchi.h"
#include "modules/wifi/wifi_recover.h"
#endif

#include "modules/wifi/socks4_proxy.h"
#include "modules/wifi/tcp_utils.h"
#include "modules/arsenal/arsenal.h"
#include "modules/arsenal/arsenal_config.h"

bool showHiddenNetworks = false;

void WifiMenu::optionsMenu() {
    returnToMenu = false;
    options.clear();
    if (WiFi.status() != WL_CONNECTED) {
        options = {
            {"Connect to Wifi", lambdaHelper(wifiConnectMenu, WIFI_STA)},
            {"Start WiFi AP", [=]() {
                 wifiConnectMenu(WIFI_AP);
                 displayInfo("pwd: " + bruceConfig.wifiAp.pwd, true);
             }},
        };
    }
    if (WiFi.getMode() != WIFI_MODE_NULL) { options.push_back({"Turn Off WiFi", wifiDisconnect}); }
    if (WiFi.getMode() == WIFI_MODE_STA || WiFi.getMode() == WIFI_MODE_APSTA) {
        options.push_back({"AP info", displayAPInfo});
    }
    options.push_back({"Wifi Atks", wifi_atk_menu});
    options.push_back({"Evil Portal", [=]() { EvilPortal(); }});
    options.push_back({"NetCut", [=]() { netcutMenu(); }});
#if !LITE_VERSION
    options.push_back({"Cred Forward", credForward});
    options.push_back({"DNS Spoofer", dnsSpoofer});
    options.push_back({"DHCP Starvation", dhcpStarvation});
    options.push_back({"Auto-Phish", arsenal_captive_portal_autophish});
    options.push_back({"HTTP Proxy", arsenal_ssl_strip});
    options.push_back({"Selective Deauth", arsenal_selective_deauth});
    options.push_back({"WPA Handshake", arsenal_wpa_handshake_grabber});
    options.push_back({"UPnP Port Opener", arsenal_upnp_port_opener});
    options.push_back({"Default Creds", arsenal_default_cred_scanner});
    options.push_back({"WiFi Bruteforce", arsenal_wifi_bruteforce});
    options.push_back({"Listen TCP", listenTcpPort});
    options.push_back({"Client TCP", clientTCP});
    options.push_back({"SOCKS4 Proxy", []() { socks4Proxy(1080); }});
    options.push_back({"TelNET", telnet_setup});
    options.push_back({"SSH", lambdaHelper(ssh_setup, String(""))});
    options.push_back({"Sniffer", sniffer_setup});
    options.push_back({"Scan Hosts", [=]() {
                           bool doScan = true;
                           if (!wifiConnected) doScan = wifiConnectMenu();
                           if (doScan) {
                               esp_netif_t *esp_netinterface =
                                   esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
                               if (esp_netinterface == nullptr) {
                                   Serial.println("Failed to get netif handle");
                                   return;
                               }
                               ARPScanner{esp_netinterface};
                           }
                       }});
    options.push_back({"Wireguard", wg_setup});
    options.push_back({"Responder", responder});
    options.push_back({"Brucegotchi", brucegotchi_start});
    options.push_back({"WiFi Pass Recovery", wifi_recover_menu});
#endif
    options.push_back({"Auth Flood", arsenal_auth_flood});
    options.push_back({"AP Clone Flood", arsenal_ap_clone_flood});
    options.push_back({"DNS Tunnel", arsenal_dns_tunnel});
    options.push_back({"WPS PIN Info", arsenal_wps_pin_attack});
    options.push_back({"Rogue AP Detect", arsenal_rogue_ap_detector});
    options.push_back({"WiFi Probe Log", arsenal_wifi_probe_log});
    options.push_back({"SSID History", arsenal_ssid_history_logger});
    options.push_back({"Channel Chart", arsenal_wifi_channel_chart});
    options.push_back({"Fingerprint", arsenal_device_fingerprinter});
    options.push_back({"Banner Grab", arsenal_service_banner_grabber});
#if !LITE_VERSION
    options.push_back({"Karma Attack", arsenal_karma_attack});
#endif
    options.push_back({"Config", [this]() { configMenu(); }});
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "WiFi");
    options.clear();
}

void WifiMenu::configMenu() {
    std::vector<Option> wifiOptions;
    wifiOptions.push_back({"Change MAC", wifiMACMenu});
    wifiOptions.push_back({"Add Evil Wifi", addEvilWifiMenu});
    wifiOptions.push_back({"Remove Evil Wifi", removeEvilWifiMenu});
    wifiOptions.push_back({bruceConfig.TerminalLog ? "SSH/Telnet Log OFF" : "SSH/Telnet Log ON", [this]() {
                               bruceConfig.setTerminalLog(!bruceConfig.TerminalLog);
                               configMenu();
                           }});
    wifiOptions.push_back({"Evil Wifi Settings", [this]() {
                               std::vector<Option> evilOptions;
                               evilOptions.push_back({"Set Gateway IP", setEvilGatewayIp});
                               evilOptions.push_back({"Password Mode", setEvilPasswordMode});
                               evilOptions.push_back({"Rename /creds", setEvilEndpointCreds});
                               evilOptions.push_back({"Allow /creds access", setEvilAllowGetCreds});
                               evilOptions.push_back({"Rename /ssid", setEvilEndpointSsid});
                               evilOptions.push_back({"Allow /ssid access", setEvilAllowSetSsid});
                               evilOptions.push_back({"Display endpoints", setEvilAllowEndpointDisplay});
                               evilOptions.push_back({"Back", [this]() { configMenu(); }});
                               loopOptions(evilOptions, MENU_TYPE_SUBMENU, "Evil Wifi Settings");
                           }});
    {
        String hidden__wifi_option = String("Hidden Networks:") + (showHiddenNetworks ? "ON" : "OFF");
        Option opt(hidden__wifi_option.c_str(), [this]() {
            showHiddenNetworks = !showHiddenNetworks;
            displayInfo(String("Hidden Networks:") + (showHiddenNetworks ? "ON" : "OFF"), true);
            configMenu();
        });
        wifiOptions.push_back(opt);
    }
    wifiOptions.push_back({"Back", [this]() { optionsMenu(); }});
    loopOptions(wifiOptions, MENU_TYPE_SUBMENU, "WiFi Config");
}

void WifiMenu::drawIcon(float scale) {
    clearIconArea();
    int deltaY = scale * 20;
    int radius = scale * 6;
    tft.fillCircle(iconCenterX, iconCenterY + deltaY, radius, bruceConfig.priColor);
    tft.drawArc(iconCenterX, iconCenterY + deltaY, deltaY + radius, deltaY, 130, 230, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY + deltaY, 2 * deltaY + radius, 2 * deltaY, 130, 230, bruceConfig.priColor, bruceConfig.bgColor);
}
