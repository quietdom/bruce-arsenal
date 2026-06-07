#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "modules/rf/rf_utils.h"
#include "modules/rf/rf_send.h"
#include <globals.h>

void arsenal_garage_brute_force(void) {
    ARSENAL_HEAP_CHECK();
    drawMainBorderWithTitle("Garage Brute");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 45);
    tft.print("12-bit garage brute force");
    tft.setCursor(12, 60);
    tft.print("Scans 4096 codes on 433MHz");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc to cancel", tftWidth / 2, tftHeight - 20, 1);
    delay(1500);

    options.clear();
    options.push_back({"Came 12-bit (350us)", []() { rf_bruteforce(); }});
    options.push_back({"Nice 12-bit (700us)", []() { rf_bruteforce(); }});
    options.push_back({"Ansonic 12-bit (555us)", []() { rf_bruteforce(); }});
    options.push_back({"Holtek 12-bit (430us)", []() { rf_bruteforce(); }});
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Protocol");
}
