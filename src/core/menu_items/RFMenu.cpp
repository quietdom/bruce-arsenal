#include "RFMenu.h"
#include "core/display.h"
#include "core/settings.h"
#include "core/utils.h"
#include "modules/rf/record.h"
#include "modules/rf/rf_bruteforce.h"
#include "modules/rf/rf_jammer.h"
#include "modules/rf/rf_listen.h"
#include "modules/rf/rf_scan.h"
#include "modules/rf/rf_send.h"
#include "modules/rf/rf_spectrum.h"
#include "modules/rf/rf_waterfall.h"
#include "modules/arsenal/arsenal.h"
#include "modules/arsenal/arsenal_config.h"

void RFMenu::optionsMenu() {
    options = {
        {"Scan/copy",       [=]() { RFScan(); }       },
#if !LITE_VERSION
        {"Record RAW",      rf_raw_record             },
        {"Custom SubGhz",   sendCustomRF              },
#endif
        {"Spectrum",        rf_spectrum               },
#if !LITE_VERSION
        {"RSSI Spectrum",   rf_CC1101_rssi            },
        {"SquareWave Spec", rf_SquareWave             },
        {"Spectogram",      rf_waterfall              },
#if defined(BUZZ_PIN) or defined(HAS_NS4168_SPKR) and defined(RF_LISTEN_H)
        {"Listen",          rf_listen                 },
#endif
        {"Bruteforce",      rf_bruteforce             },
        {"Jammer",          [=]() { RFJammer(true); } },
        {"NRF24 MouseJack", arsenal_nrf24_mousejack   },
        {"Doorbell Replay", arsenal_doorbell_replay   },
        {"Rolljam",         arsenal_rolljam           },
        {"Garage Brute",    arsenal_garage_brute_force},
        {"Keyfob Logger",   arsenal_car_keyfob_logger },
#endif
        {"Freq Scanner",    arsenal_frequency_scanner },
        {"Flipper Import",  arsenal_flipper_import    },
        {"Config",          [this]() { configMenu(); }},
    };
    addOptionToMainMenu();
    delay(200);
    String txt = "Radio Frequency";
    if (bruceConfigPins.rfModule == CC1101_SPI_MODULE) txt += " (CC1101)";
    else txt += " Tx: " + String(bruceConfigPins.rfTx) + " Rx: " + String(bruceConfigPins.rfRx);
    loopOptions(options, MENU_TYPE_SUBMENU, txt.c_str());
}

void RFMenu::configMenu() {
    options = {
        {"RF TX Pin", lambdaHelper(gsetRfTxPin, true)},
        {"RF RX Pin", lambdaHelper(gsetRfRxPin, true)},
        {"RF Module", setRFModuleMenu},
        {"RF Frequency", setRFFreqMenu},
        {"Back", [this]() { optionsMenu(); }},
    };
    loopOptions(options, MENU_TYPE_SUBMENU, "RF Config");
}

void RFMenu::drawIcon(float scale) {
    clearIconArea();
    int radius = scale * 7;
    int deltaRadius = scale * 10;
    int triangleSize = scale * 30;
    if (triangleSize % 2 != 0) triangleSize++;
    tft.fillCircle(iconCenterX, iconCenterY - radius, radius, bruceConfig.priColor);
    tft.fillTriangle(iconCenterX, iconCenterY, iconCenterX - triangleSize / 2, iconCenterY + triangleSize, iconCenterX + triangleSize / 2, iconCenterY + triangleSize, bruceConfig.priColor);
    tft.drawArc(iconCenterX, iconCenterY - radius, 2.5 * radius, 2 * radius, 40, 140, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY - radius, 2.5 * radius + deltaRadius, 2 * radius + deltaRadius, 40, 140, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY - radius, 2.5 * radius + 2 * deltaRadius, 2 * radius + 2 * deltaRadius, 40, 140, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY - radius, 2.5 * radius, 2 * radius, 220, 320, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY - radius, 2.5 * radius + deltaRadius, 2 * radius + deltaRadius, 220, 320, bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawArc(iconCenterX, iconCenterY - radius, 2.5 * radius + 2 * deltaRadius, 2 * radius + 2 * deltaRadius, 220, 320, bruceConfig.priColor, bruceConfig.bgColor);
}
