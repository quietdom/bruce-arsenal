#ifndef __ARSENAL_H__
#define __ARSENAL_H__

#include <globals.h>
#include <esp_task_wdt.h>
#include "core/utils.h"

#ifndef LITE_VERSION
#define LITE_VERSION 0
#endif

#define ARSENAL_SAFE_RUN(func)                                        \
    do {                                                              \
        if (ESP.getFreeHeap() < 20000) {                              \
            Serial.println(F("[Arsenal] Low memory, aborting."));     \
            displayRedStripe(F("Low memory!"), TFT_WHITE, TFT_RED);  \
            delay(1500);                                              \
            return;                                                   \
        }                                                             \
        func();                                                       \
    } while (0)

#define ARSENAL_HEAP_CHECK()                                          \
    do {                                                              \
        if (ESP.getFreeHeap() < 20000) {                              \
            Serial.println(F("[Arsenal] Low memory, aborting."));     \
            displayRedStripe(F("Low memory!"), TFT_WHITE, TFT_RED);  \
            delay(1500);                                              \
            return;                                                   \
        }                                                             \
    } while (0)


void arsenal_network_scanner(void);
void arsenal_dhcp_starvation(void);
void arsenal_karma_attack(void);
void arsenal_dns_spoofer(void);
#if !LITE_VERSION
void arsenal_captive_portal_templates(void);
void arsenal_captive_portal_autophish(void);
#endif
#if !LITE_VERSION
void arsenal_wifi_bruteforce(void);
#endif
#if !LITE_VERSION
void arsenal_cred_forward(void);
#endif


#if !LITE_VERSION
void arsenal_ble_tracker(void);
void arsenal_bt_name_spammer(void);
void arsenal_airtag_spoofer(void);
void arsenal_bt_audio_jammer(void);
void arsenal_sms_notification_spoofer(void);
#endif


void arsenal_device_fingerprinter(void);
void arsenal_opsec_monitor(void);
void arsenal_oui_lookup(void);


void arsenal_mac_rotator(void);
void arsenal_channel_hopper(void);
void arsenal_decoy_traffic(void);
void arsenal_identity_cloner(void);
void arsenal_qr_poisoner(void);


void arsenal_jam_all(void);


void arsenal_remote_dashboard(void);


void arsenal_script_browser(void);

#if !LITE_VERSION
void arsenal_wifi_bruteforce(void);
#endif


void arsenal_attack_scheduler(void);


void arsenal_log_start(void);
void arsenal_log_event(const char *category, const char *message);
void arsenal_log_event(const char *category, String message);
void arsenal_log_stop(void);
void arsenal_session_log_menu(void);


// Config / QoL
void arsenal_config_ap(void);
void arsenal_config_dashboard(void);
void arsenal_pin_lock(void);

// WiFi Attacks
#if !LITE_VERSION
void arsenal_wpa_handshake_grabber(void);
void arsenal_beacon_flood(void);
void arsenal_selective_deauth(void);
#endif
void arsenal_auth_flood(void);
void arsenal_ap_clone_flood(void);
#if !LITE_VERSION
void arsenal_arp_poisoner(void);
#endif
#if !LITE_VERSION
void arsenal_ssl_strip(void);
#endif
#if !LITE_VERSION
void arsenal_upnp_port_opener(void);
void arsenal_default_cred_scanner(void);
#endif
void arsenal_dns_tunnel(void);
void arsenal_wps_pin_attack(void);
void arsenal_rogue_ap_detector(void);

// RF / Sub-GHz
#if !LITE_VERSION
void arsenal_nrf24_mousejack(void);
void arsenal_doorbell_replay(void);
void arsenal_garage_brute_force(void);
void arsenal_car_keyfob_logger(void);
#endif
void arsenal_frequency_scanner(void);
void arsenal_flipper_import(void);

// BLE
#if !LITE_VERSION
void arsenal_bt_audio_rickroll(void);
void arsenal_bt_device_profiler(void);
#endif

// Phishing Portals
void arsenal_phish_windows_update(void);
void arsenal_phish_wifi_speed(void);
void arsenal_phish_oauth(void);
void arsenal_phish_device_found(void);

// Intelligence / Recon
void arsenal_wifi_probe_log(void);
void arsenal_ssid_history_logger(void);
void arsenal_service_banner_grabber(void);
#if !LITE_VERSION
void arsenal_smart_home_scanner(void);
#endif
void arsenal_wifi_channel_chart(void);
#if !LITE_VERSION
void arsenal_people_counter(void);
#endif
void arsenal_device_nickname(void);

// Detection
#if !LITE_VERSION
void arsenal_flipper_detector(void);
void arsenal_hacker_detector(void);
void arsenal_rf_silence_enforcer(void);
#endif

// Comms
#if !LITE_VERSION
void arsenal_espnow_chat(void);
void arsenal_espnow_c2(void);
#endif
#if !LITE_VERSION
void arsenal_dead_drop_mesh(void);
void arsenal_ir_data_transfer(void);
void arsenal_multi_device_sync(void);
#endif

// Evasion
void arsenal_time_based_randomizer(void);

// Utility
void arsenal_password_generator(void);
#if !LITE_VERSION
void arsenal_nfc_business_card(void);
#endif
void arsenal_attack_stats(void);


void arsenal_dim_on_attack(void);
void arsenal_dim_restore(void);
void arsenal_auto_dim_toggle(void);
bool arsenal_auto_dim_is_enabled(void);
bool arsenal_is_dimmed(void);


#endif
