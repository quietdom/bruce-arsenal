#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"

// --- Helper macro for new stubs ---
#define ARSENAL_NEW_STUB(func_name, display_name) \
    void func_name(void) { \
        drawMainBorderWithTitle(display_name); \
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor); \
        tft.setTextSize(FM); \
        tft.drawCentreString("Initializing...", tftWidth / 2, tftHeight / 2 - 10, 1); \
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor); \
        tft.setTextSize(FP); \
        tft.drawCentreString(display_name " active", tftWidth / 2, tftHeight / 2 + 10, 1); \
        delay(1500); \
    }

// Config / QoL
ARSENAL_NEW_STUB(arsenal_config_ap, "AP Config")
ARSENAL_NEW_STUB(arsenal_config_dashboard, "Dashboard Config")
ARSENAL_NEW_STUB(arsenal_pin_lock, "PIN Lock")

// WiFi Attacks
ARSENAL_NEW_STUB(arsenal_wpa_handshake_grabber, "WPA Grabber")
ARSENAL_NEW_STUB(arsenal_beacon_flood, "Beacon Flood")
ARSENAL_NEW_STUB(arsenal_selective_deauth, "Selective Deauth")
ARSENAL_NEW_STUB(arsenal_auth_flood, "Auth Flood")
ARSENAL_NEW_STUB(arsenal_ap_clone_flood, "AP Clone Flood")
ARSENAL_NEW_STUB(arsenal_arp_poisoner, "ARP Poisoner")
ARSENAL_NEW_STUB(arsenal_ssl_strip, "SSL Strip Lite")
ARSENAL_NEW_STUB(arsenal_upnp_port_opener, "UPnP Opener")
ARSENAL_NEW_STUB(arsenal_default_cred_scanner, "Def. Cred Scanner")
ARSENAL_NEW_STUB(arsenal_dns_tunnel, "DNS Tunnel")
ARSENAL_NEW_STUB(arsenal_wps_pin_attack, "WPS PIN Attack")
ARSENAL_NEW_STUB(arsenal_rogue_ap_detector, "Rogue AP Detector")

// RF / Sub-GHz
ARSENAL_NEW_STUB(arsenal_nrf24_mousejack, "NRF24 MouseJack")
ARSENAL_NEW_STUB(arsenal_doorbell_replay, "Doorbell Replay")
ARSENAL_NEW_STUB(arsenal_garage_brute_force, "Garage Brute")
ARSENAL_NEW_STUB(arsenal_car_keyfob_logger, "Keyfob Logger")
ARSENAL_NEW_STUB(arsenal_frequency_scanner, "Freq Scanner")
ARSENAL_NEW_STUB(arsenal_flipper_import, "Flipper Import")

// BLE
ARSENAL_NEW_STUB(arsenal_bt_audio_rickroll, "BT Rickroll")
ARSENAL_NEW_STUB(arsenal_bt_device_profiler, "BT Profiler")

// Phishing Portals
ARSENAL_NEW_STUB(arsenal_phish_windows_update, "Phish WinUpdate")
ARSENAL_NEW_STUB(arsenal_phish_wifi_speed, "Phish WiFi Speed")
ARSENAL_NEW_STUB(arsenal_phish_oauth, "Phish OAuth")
ARSENAL_NEW_STUB(arsenal_phish_device_found, "Phish DevFound")

// Intelligence / Recon
ARSENAL_NEW_STUB(arsenal_wifi_probe_log, "Probe Logger")
ARSENAL_NEW_STUB(arsenal_ssid_history_logger, "SSID History")
ARSENAL_NEW_STUB(arsenal_service_banner_grabber, "Banner Grabber")
ARSENAL_NEW_STUB(arsenal_smart_home_scanner, "SmartHome Scan")
ARSENAL_NEW_STUB(arsenal_wifi_channel_chart, "Channel Chart")
ARSENAL_NEW_STUB(arsenal_people_counter, "People Counter")
ARSENAL_NEW_STUB(arsenal_device_nickname, "Device Nickname")

// Detection
ARSENAL_NEW_STUB(arsenal_flipper_detector, "Flipper Detector")
ARSENAL_NEW_STUB(arsenal_hacker_detector, "Hacker Detector")
ARSENAL_NEW_STUB(arsenal_rf_silence_enforcer, "RF Silence")

// Comms
ARSENAL_NEW_STUB(arsenal_espnow_chat, "ESPNOW Chat")
ARSENAL_NEW_STUB(arsenal_espnow_c2, "ESPNOW C2")
ARSENAL_NEW_STUB(arsenal_dead_drop_mesh, "Dead Drop Mesh")
ARSENAL_NEW_STUB(arsenal_ir_data_transfer, "IR Data Transfer")
ARSENAL_NEW_STUB(arsenal_multi_device_sync, "Multi-Device Sync")

// Evasion
ARSENAL_NEW_STUB(arsenal_time_based_randomizer, "Time Randomizer")

// Utility
ARSENAL_NEW_STUB(arsenal_password_generator, "Password Gen")
ARSENAL_NEW_STUB(arsenal_nfc_business_card, "NFC Biz Card")
ARSENAL_NEW_STUB(arsenal_attack_stats, "Attack Stats")
