#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <globals.h>

void arsenal_nrf24_mousejack(void) {
    ARSENAL_HEAP_CHECK();
    drawMainBorderWithTitle("NRF24 MouseJack");
    int y = 40;
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, y);
    y += 14;
    tft.print("Requires NRF24L01 module");
    tft.setCursor(12, y);
    y += 14;
    tft.print("Connect to GPIO pins:");
    tft.setCursor(12, y);
    y += 14;
    tft.print("  CE=4  CSN=5");
    tft.setCursor(12, y);
    y += 14;
    tft.print("  SCK=18 MOSI=23 MISO=19");
    y += 20;
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.setCursor(12, y);
    y += 14;
    tft.print("Once wired, use SubGHz");
    tft.setCursor(12, y);
    tft.print("menu for RF operations.");
    tft.drawCentreString("Press any key", tftWidth / 2, tftHeight - 20, 1);
    while (!check(EscPress) && !check(SelPress)) delay(100);
}
