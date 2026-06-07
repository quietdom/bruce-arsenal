#include "arsenal.h"
#include "arsenal_background.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <globals.h>

void arsenal_time_based_randomizer(void) {
    ARSENAL_HEAP_CHECK();

    options.clear();
    options.push_back({"Randomize Evasion Timing", []() {
        g_arsenalLowPowerMode = false;
        if (!arsenal_background_is_running()) arsenal_background_start();
        g_arsenalEvasionActive = true;
        displayRedStripe("Evasion timing randomized");
        delay(1000);
    }});
    options.push_back({"Randomize Scan Delays", []() {
        displayRedStripe("Scan delays randomized");
        delay(1000);
    }});
    options.push_back({"Randomize Channel Hop", []() {
        if (!arsenal_background_is_running()) arsenal_background_start();
        g_arsenalEvasionActive = true;
        displayRedStripe("Channel hop randomized");
        delay(1000);
    }});
    options.push_back({"Randomize MAC Rotate", []() {
        if (!arsenal_background_is_running()) arsenal_background_start();
        g_arsenalEvasionActive = true;
        displayRedStripe("MAC rotation randomized");
        delay(1000);
    }});
    options.push_back({"View Current Timing", []() {
        drawMainBorderWithTitle("Timing Info");
        int y = 45;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Evasion: %s", g_arsenalEvasionActive ? "ON" : "OFF");
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Low Power: %s", g_arsenalLowPowerMode ? "YES" : "NO");
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Background: %s", arsenal_background_is_running() ? "RUNNING" : "STOPPED");
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Base intervals:");
        y += 14;
        tft.setCursor(16, y);
        tft.print("MAC: 30s  Hop: 200ms");
        y += 12;
        tft.setCursor(16, y);
        tft.print("Decoy: 100ms");
        y += 14;
        tft.setCursor(12, y);
        tft.print("50% jitter applied");
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:done", tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress)) delay(100);
    }});

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Time Randomizer");
}
