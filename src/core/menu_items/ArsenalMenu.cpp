#include "ArsenalMenu.h"

#include "core/display.h"
#include "modules/arsenal/arsenal.h"

void ArsenalMenu::optionsMenu() {
    options = {
        {"WiFi", [this]() { wifiMenu(); }},
        {"RF / Sub-GHz", [this]() { rfMenu(); }},
        {"Bluetooth", [this]() { bleMenu(); }},
        {"Recon", [this]() { reconMenu(); }},
        {"Detection", [this]() { detectionMenu(); }},
        {"Comms", [this]() { commsMenu(); }},
        {"Utilities", [this]() { utilityMenu(); }},
        {"Config", [this]() { configMenu(); }},
        {"Attack Scheduler", arsenal_attack_scheduler},
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal");
}

void ArsenalMenu::wifiMenu() {
    options = {
        {"Network Scanner", arsenal_network_scanner},
        {"Karma Attack", arsenal_karma_attack},
        {"Deauth Detector", arsenal_deauth_detector},
        {"PMKID Capture", arsenal_pmkid_capture},
#if !LITE_VERSION
        {"WiFi Brute Force", arsenal_wifi_bruteforce},
        {"WPA Handshake", arsenal_wpa_handshake_grabber},
        {"Beacon Flood", arsenal_beacon_flood},
        {"Selective Deauth", arsenal_selective_deauth},
#endif
        {"Auth Flood", arsenal_auth_flood},
        {"AP Clone Flood", arsenal_ap_clone_flood},
#if !LITE_VERSION
        {"ARP Poisoner", arsenal_arp_poisoner},
        {"SSL Strip", arsenal_ssl_strip},
        {"UPnP Port Opener", arsenal_upnp_port_opener},
        {"Default Cred Scanner", arsenal_default_cred_scanner},
#endif
        {"DNS Tunnel", arsenal_dns_tunnel},
        {"WPS PIN Attack", arsenal_wps_pin_attack},
        {"Rogue AP Detector", arsenal_rogue_ap_detector},
#if !LITE_VERSION
        {"Captive Portal", arsenal_captive_portal_autophish},
#endif
        {"Phish Windows Update", arsenal_phish_windows_update},
        {"Phish WiFi Speed", arsenal_phish_wifi_speed},
        {"Phish OAuth", arsenal_phish_oauth},
        {"Phish Device Found", arsenal_phish_device_found},
        {"MAC Rotator", arsenal_mac_rotator},
        {"Channel Hopper", arsenal_channel_hopper},
        {"Decoy Traffic", arsenal_decoy_traffic},
        {"Identity Cloner", arsenal_identity_cloner},
        {"QR Poisoner", arsenal_qr_poisoner},
        {"Jam All", arsenal_jam_all},
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal WiFi");
}

void ArsenalMenu::rfMenu() {
    options = {
        {"Frequency Scanner", arsenal_frequency_scanner},
        {"Flipper Import", arsenal_flipper_import},
        {"RollJam", arsenal_rolljam},
#if !LITE_VERSION
        {"MouseJack", arsenal_nrf24_mousejack},
        {"Doorbell Replay", arsenal_doorbell_replay},
        {"Garage Brute Force", arsenal_garage_brute_force},
        {"Car Keyfob Logger", arsenal_car_keyfob_logger},
#endif
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal RF");
}

void ArsenalMenu::bleMenu() {
    options = {
#if !LITE_VERSION
        {"BT Name Spammer", arsenal_bt_name_spammer},
        {"BT Audio Jammer", arsenal_bt_audio_jammer},
        {"BT Audio Rickroll", arsenal_bt_audio_rickroll},
        {"BT Device Profiler", arsenal_bt_device_profiler},
        {"SMS Notification Spoofer", arsenal_sms_notification_spoofer},
#endif
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal Bluetooth");
}

void ArsenalMenu::reconMenu() {
    options = {
        {"Device Fingerprinter", arsenal_device_fingerprinter},
        {"OUI Lookup", arsenal_oui_lookup},
        {"WiFi Probe Log", arsenal_wifi_probe_log},
        {"SSID History", arsenal_ssid_history_logger},
        {"Service Banner Grabber", arsenal_service_banner_grabber},
#if !LITE_VERSION
        {"Smart Home Scanner", arsenal_smart_home_scanner},
        {"People Counter", arsenal_people_counter},
#endif
        {"WiFi Channel Chart", arsenal_wifi_channel_chart},
        {"Device Nickname", arsenal_device_nickname},
        {"OpSec Monitor", arsenal_opsec_monitor},
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal Recon");
}

void ArsenalMenu::detectionMenu() {
    options = {
#if !LITE_VERSION
        {"Flipper Detector", arsenal_flipper_detector},
        {"Hacker Detector", arsenal_hacker_detector},
        {"RF Silence Enforcer", arsenal_rf_silence_enforcer},
#endif
        {"Deauth Detector", arsenal_deauth_detector},
        {"Rogue AP Detector", arsenal_rogue_ap_detector},
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal Detection");
}

void ArsenalMenu::commsMenu() {
    options = {
#if !LITE_VERSION
        {"ESP-NOW Chat", arsenal_espnow_chat},
        {"ESP-NOW C2", arsenal_espnow_c2},
        {"Dead Drop Mesh", arsenal_dead_drop_mesh},
        {"IR Data Transfer", arsenal_ir_data_transfer},
        {"Multi-Device Sync", arsenal_multi_device_sync},
#endif
        {"Remote Dashboard", arsenal_remote_dashboard},
        {"Script Browser", arsenal_script_browser},
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal Comms");
}

void ArsenalMenu::utilityMenu() {
    options = {
        {"Password Generator", arsenal_password_generator},
        {"Attack Stats", arsenal_attack_stats},
        {"Time Randomizer", arsenal_time_based_randomizer},
        {"Session Log", arsenal_session_log_menu},
#if !LITE_VERSION
        {"NFC Business Card", arsenal_nfc_business_card},
#endif
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal Utilities");
}

void ArsenalMenu::configMenu() {
    options = {
        {"Configure AP", arsenal_config_ap},
        {"Configure Dashboard", arsenal_config_dashboard},
        {"PIN Lock", arsenal_pin_lock},
        {"Auto Dim", arsenal_auto_dim_toggle},
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Arsenal Config");
}

void ArsenalMenu::drawIcon(float scale) {
    clearIconArea();

    int radius = max(12, (int)(30 * scale));
    int centerX = iconCenterX;
    int centerY = iconCenterY;
    uint16_t color = bruceConfig.priColor;

    tft.drawCircle(centerX, centerY, radius, color);
    tft.drawCircle(centerX, centerY, radius / 2, color);
    tft.drawLine(centerX - radius - 8 * scale, centerY, centerX + radius + 8 * scale, centerY, color);
    tft.drawLine(centerX, centerY - radius - 8 * scale, centerX, centerY + radius + 8 * scale, color);
    tft.fillCircle(centerX, centerY, max(2, (int)(4 * scale)), color);
}
