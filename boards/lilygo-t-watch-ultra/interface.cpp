#include "core/powerSave.h"
#include "core/utils.h"
#include <Wire.h>
#include <XPowersLib.h>
#include <interface.h>

// AXP2101 Power Management
XPowersAXP2101 axp192;

// XL9555 IO Expander
#include <ExtensionIOXL9555.hpp>
ExtensionIOXL9555 io;

// Touch
#define TOUCH_MODULES_CST_SELF
#include <TouchDrvCSTXXX.hpp>
TouchDrvCSTXXX touch;

// Haptic
#include "SensorDRV2605.hpp"
SensorDRV2605 drv;

static bool xl9555_ready = false;

/***************************************************************************************
** Function name: _setup_gpio()
** Location: main.cpp
** Description:   initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    // Initialize I2C bus
    Wire.begin(SDA, SCL);
    delay(10);

    // Initialize XL9555 IO Expander
    xl9555_ready = io.init(Wire, 0x20);
    if (xl9555_ready) {
        Serial.println("XL9555 init success");
        // Enable display power (GPIO7 = bit 7 of Port 0)
        io.pinMode(EXPANDS_DISP_EN, OUTPUT);
        io.digitalWrite(EXPANDS_DISP_EN, HIGH);
        delay(10);
        // Enable haptic driver (GPIO6 = bit 6 of Port 0)
        io.pinMode(EXPANDS_DRV_EN, OUTPUT);
        io.digitalWrite(EXPANDS_DRV_EN, HIGH);
        delay(10);
        // Touch reset (GPIO10 = bit 2 of Port 1)
        io.pinMode(EXPANDS_TOUCH_RST, OUTPUT);
        io.digitalWrite(EXPANDS_TOUCH_RST, LOW);
        delay(10);
        io.digitalWrite(EXPANDS_TOUCH_RST, HIGH);
        delay(10);
    } else {
        Serial.println("XL9555 init failed");
    }

    // Initialize AXP2101 Power Management
    axp192.init(Wire, SDA, SCL);
    axp192.setVbusVoltageLimit(XPOWERS_AXP2101_VBUS_VOL_LIM_4V36);
    axp192.setVbusCurrentLimit(XPOWERS_AXP2101_VBUS_CUR_LIM_900MA);
    axp192.setSysPowerDownVoltage(2600);
    // Power rails for T-Watch Ultra
    axp192.setALDO1Voltage(3300); // RTC VBAT
    axp192.setALDO2Voltage(3300); // TFT BACKLIGHT VDD (AMOLED enable)
    axp192.setALDO3Voltage(3300); // Screen touch VDD
    axp192.setALDO4Voltage(3300); // Radio VDD
    axp192.enableALDO1();
    axp192.enableALDO2();
    axp192.enableALDO3();
    axp192.enableALDO4();
    axp192.disableDC2();
    axp192.disableDC4();
    axp192.disableDC5();
    axp192.disableBLDO1();
    axp192.disableCPUSLDO();
    axp192.disableDLDO1();
    axp192.disableDLDO2();
    // Power key config
    axp192.setPowerKeyPressOffTime(XPOWERS_POWEROFF_4S);
    axp192.setPowerKeyPressOnTime(XPOWERS_POWERON_128MS);
    axp192.disableTSPinMeasure();
    // Battery ADC
    axp192.enableBattDetection();
    axp192.enableVbusVoltageMeasure();
    axp192.enableBattVoltageMeasure();
    axp192.enableSystemVoltageMeasure();
    axp192.setChargingLedMode(XPOWERS_CHG_LED_OFF);
    // IRQ
    axp192.disableIRQ(XPOWERS_AXP2101_ALL_IRQ);
    axp192.enableIRQ(
        XPOWERS_AXP2101_BAT_INSERT_IRQ | XPOWERS_AXP2101_BAT_REMOVE_IRQ |
        XPOWERS_AXP2101_VBUS_INSERT_IRQ | XPOWERS_AXP2101_VBUS_REMOVE_IRQ |
        XPOWERS_AXP2101_PKEY_SHORT_IRQ | XPOWERS_AXP2101_PKEY_LONG_IRQ |
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ | XPOWERS_AXP2101_BAT_CHG_START_IRQ
    );
    axp192.clearIrqStatus();
    axp192.setPrechargeCurr(XPOWERS_AXP2101_PRECHARGE_50MA);
    axp192.setChargerConstantCurr(XPOWERS_AXP2101_CHG_CUR_300MA);
    axp192.setChargerTerminationCurr(XPOWERS_AXP2101_CHG_ITERM_25MA);
    axp192.setChargeTargetVoltage(XPOWERS_AXP2101_CHG_VOL_4V35);
    axp192.setButtonBatteryChargeVoltage(3300);
    axp192.enableButtonBatteryCharge();

    // Initialize Touch (CST9217)
    touch.setPins(EXPANDS_TOUCH_RST, TP_INT);
    touch.begin(Wire, 0x1A, SDA, SCL);
    touch.setMaxCoordinates(TFT_WIDTH, TFT_HEIGHT);
    touch.setSwapXY(false);
    touch.setMirrorXY(false, false);

    // Disable RF and NRF Menus for default (no RF module on watch)
    bruceConfig.disabledMenus.push_back("RF");
    bruceConfig.disabledMenus.push_back("NRF24");

    // Haptic driver
    if (!drv.begin(Wire, SDA, SCL)) {
        Serial.println("Failed to find DRV2605.");
    } else {
        Serial.println("Init DRV2605 Sensor success!");
        drv.selectLibrary(1);
        drv.setMode(SensorDRV2605::MODE_INTTRIG);
        drv.useERM();
        // Startup buzz
        drv.setWaveform(0, 70);
        drv.setWaveform(1, 0);
        drv.run();
    }

    bruceConfigPins.gpsBaudrate = 38400;
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Location: main.cpp
** Description:   second stage gpio setup to make a few functions work
***************************************************************************************/
void _post_setup_gpio() {
    // AMOLED has no backlight pin - brightness set via AXP2101 or display commands
}

/***************************************************************************************
** Function name: getBattery()
** location: display.cpp
** Description:   Delivers the battery value from 1-100
***************************************************************************************/
int getBattery() {
    int percent = axp192.getBatteryPercent();
    return percent;
}

/*********************************************************************
** Function: setBrightness
** location: settings.cpp
** set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    // CO5300 AMOLED brightness via display command
    // For now, use AXP2101 ALDO2 voltage as brightness proxy
    int voltage;
    if (brightval == 0) voltage = 1800;
    else if (brightval <= 25) voltage = 2500;
    else if (brightval <= 50) voltage = 2800;
    else if (brightval <= 75) voltage = 3000;
    else voltage = 3300;
    axp192.setALDO2Voltage(voltage);
}

struct TP {
    int16_t x[1], y[1];
};

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    TP t;
    static unsigned long tm = 0;
    if (millis() - tm > 200 || LongPress) {
        bool touched = touch.getPoint(t.x, t.y, 1);
        if (touched) {
            if (bruceConfigPins.rotation == 3) {
                t.y[0] = (tftHeight + 20) - t.y[0];
            }
            if (bruceConfigPins.rotation == 0) {
                int tmp = t.x[0];
                t.x[0] = tftWidth - t.y[0];
                t.y[0] = tftHeight - tmp;
            }
            if (bruceConfigPins.rotation == 2) {
                int tmp = t.x[0];
                t.x[0] = t.y[0];
                t.y[0] = tmp;
            }
            if (bruceConfigPins.rotation == 1) { t.x[0] = tftWidth - t.x[0]; }

            if (!wakeUpScreen()) AnyKeyPress = true;
            else return;

            touchPoint.x = t.x[0];
            touchPoint.y = t.y[0];
            touchPoint.pressed = true;
            touchHeatMap(touchPoint);

            tm = millis();
            drv.setWaveform(0, 75);
            drv.setWaveform(1, 0);
            drv.run();
        }
    }
}

/*********************************************************************
** Function: powerOff
** location: mykeyboard.cpp
** Turns off the device (or try to)
**********************************************************************/
void powerOff() { axp192.shutdown(); }

/*********************************************************************
** Function: checkReboot
** location: mykeyboard.cpp
** Btn logic to turn off the device
**********************************************************************/
void checkReboot() {}

/***************************************************************************************
** Function name: isCharging()
** Description:   Determines if the device is charging
***************************************************************************************/
bool isCharging() { return axp192.isCharging(); }

/*********************************************************************
** Function: goToDeepSleep
** location: mykeyboard.cpp
** Puts the device into DeepSleep
**********************************************************************/
void goToDeepSleep() { axp192.shutdown(); }
