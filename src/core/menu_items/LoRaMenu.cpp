#include "LoRaMenu.h"
#include "core/display.h"
#include "core/utils.h"
#include "modules/lora/LoRaRF.h"

void LoRaMenu::optionsMenu() {
    options = {
        {"Chat",             []() { lorachat(); }      },
        {"Change username",  []() { changeusername(); }},
        {"Change Frequency", []() { chfreq(); }        },
    };
    addOptionToMainMenu();
    String txt = "LoRa (" + String(bruceConfig.loraspc);
    ")";
    loopOptions(options, MENU_TYPE_SUBMENU, txt.c_str());
}

void LoRaMenu::drawIcon(float scale) {
    clearIconArea();
    int centerX = iconCenterX;
    int centerY = iconCenterY;
    int boxWidth = scale * 26;
    int boxHeight = scale * 16;
    int cornerRadius = scale * 4;

    tft.fillRoundRect(
        centerX - boxWidth / 2,
        centerY - boxHeight / 2,
        boxWidth,
        boxHeight,
        cornerRadius,
        bruceConfig.priColor
    );

    int slotWidth = scale * 18;
    int slotHeight = scale * 3;
    tft.fillRect(
        centerX - slotWidth / 2, centerY - slotHeight / 2, slotWidth, slotHeight, bruceConfig.bgColor
    );

    int postWidth = scale * 4;
    int postHeight = scale * 15;
    tft.fillRect(
        centerX - postWidth / 2, centerY + boxHeight / 2, postWidth, postHeight, bruceConfig.priColor
    );

    tft.setTextSize(1);
    tft.setTextColor(bruceConfig.priColor);
    tft.drawCentreString("LoRa", centerX, centerY + boxHeight / 2 + postHeight + 5, 1);
}

void LoRaMenu::drawIconImg() {
    drawImg(
        *bruceConfig.themeFS(), bruceConfig.getThemeItemImg(bruceConfig.theme.paths.lora), 0, imgCenterY, true
    );
}
