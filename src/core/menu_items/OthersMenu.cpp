#include "OthersMenu.h"
#include "core/display.h"
#include "core/utils.h"
#include "modules/badusb_ble/ducky_typer.h"
#include "modules/bjs_interpreter/interpreter.h"
#include "modules/others/clicker.h"
#include "modules/others/ibutton.h"
#include "modules/others/mic.h"
#include "modules/others/qrcode_menu.h"
#include "modules/others/tururururu.h"
#include "modules/others/u2f.h"
#include "modules/arsenal/arsenal.h"
#include "modules/arsenal/arsenal_config.h"
#include "modules/arsenal/arsenal_background.h"

void OthersMenu::optionsMenu() {
    options = {
        {"QRCodes",         qrcode_menu                     },
        {"Megalodon",       shark_setup                     },
#if defined(MIC_SPM1423) || defined(MIC_INMP441)
        {"Microphone",      [this]() { micMenu(); }         },
#endif
#if !LITE_VERSION
#if defined(USB_as_HID)
        {"BadUSB & HID",    [this]() { badUsbHidMenu(); }},
#endif
        {"iButton",         setup_ibutton                   },
#endif
        {"OPSEC Monitor",   arsenal_opsec_monitor           },
        {"OUI Lookup",      arsenal_oui_lookup              },
        {"Probe Log",       arsenal_wifi_probe_log          },
        {"Banner Grabber",  arsenal_service_banner_grabber  },
#if !LITE_VERSION
        {"SmartHome Scan",  arsenal_smart_home_scanner      },
#endif
        {"Channel Chart",   arsenal_wifi_channel_chart      },
#if !LITE_VERSION
        {"People Counter",  arsenal_people_counter          },
#endif
        {"Device Nickname", arsenal_device_nickname         },
        {"MAC Rotator",     arsenal_mac_rotator             },
        {"Channel Hopper",  arsenal_channel_hopper          },
        {"Decoy Traffic",   arsenal_decoy_traffic           },
        {"Identity Cloner", arsenal_identity_cloner         },
        {"Time Randomizer", arsenal_time_based_randomizer   },
        {"Win Update",      arsenal_phish_windows_update    },
        {"WiFi Speed",      arsenal_phish_wifi_speed        },
        {"OAuth Phish",     arsenal_phish_oauth             },
        {"Device Found",    arsenal_phish_device_found      },
#if !LITE_VERSION
        {"Flipper Detector",arsenal_flipper_detector        },
        {"Hacker Detector", arsenal_hacker_detector         },
        {"RF Silence",      arsenal_rf_silence_enforcer     },
        {"ESP-NOW Chat",    arsenal_espnow_chat             },
        {"ESP-NOW C2",      arsenal_espnow_c2               },
        {"Dead Drop Mesh",  arsenal_dead_drop_mesh          },
        {"IR Data Transfer",arsenal_ir_data_transfer        },
        {"Multi-Device Sync",arsenal_multi_device_sync      },
        {"NFC Biz Card",    arsenal_nfc_business_card       },
#endif
        {"Attack Stats",    arsenal_attack_stats            },
        {"Password Gen",    arsenal_password_generator      },
        {"QR Poisoner",     arsenal_qr_poisoner             },
        {"Jam All",         arsenal_jam_all                 },
#if !LITE_VERSION
        {"Dashboard",       arsenal_remote_dashboard        },
#endif
        {"Combos",          arsenal_combo_menu              },
        {"Scheduler",       arsenal_attack_scheduler        },
        {"Scripts",         arsenal_script_browser          },
        {"Session Log",     arsenal_session_log_menu        },
        {"Config AP",       arsenal_config_ap               },
        {"Config Dash",     arsenal_config_dashboard        },
        {"PIN Lock",        arsenal_pin_lock                },
    };
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Others");
}

void OthersMenu::badUsbHidMenu() {
    options = {
#ifndef LITE_VERSION
        {"BadUSB",       [=]() { ducky_setup(hid_usb, false); }   },
        {"USB Keyboard", [=]() { ducky_keyboard(hid_usb, false); }},
#endif
#ifdef USB_as_HID
        {"USB Clicker",  clicker_setup                            },
        {"USB U2F",      u2f_setup                                },
#endif
        {"Back",         [this]() { optionsMenu(); }              },
    };
    loopOptions(options, MENU_TYPE_SUBMENU, "BadUSB & HID");
}

void OthersMenu::micMenu() {
    options = {
#if defined(MIC_SPM1423) || defined(MIC_INMP441)
        {"Spectrum", mic_test                   },
        {"Record",   mic_record_app             },
#endif
        {"Back",     [this]() { optionsMenu(); }},
    };
    loopOptions(options, MENU_TYPE_SUBMENU, "Microphone");
}

void OthersMenu::drawIcon(float scale) {
    clearIconArea();
    int radius = scale * 7;
    tft.fillCircle(iconCenterX, iconCenterY, radius, bruceConfig.priColor);
    tft.drawArc(iconCenterX, iconCenterY, 2.5 * radius, 2 * radius, 0, 340, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY, 3.5 * radius, 3 * radius, 20, 360, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY, 4.5 * radius, 4 * radius, 0, 200, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY, 4.5 * radius, 4 * radius, 240, 360, bruceConfig.priColor, bruceConfig.bgColor);
}
