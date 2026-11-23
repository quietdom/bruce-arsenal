#ifndef __ARSENAL_MENU_H__
#define __ARSENAL_MENU_H__

#include <MenuItemInterface.h>

class ArsenalMenu : public MenuItemInterface {
public:
    ArsenalMenu() : MenuItemInterface("Arsenal") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return false; }
    const String& themePath() override { return bruceConfig.theme.paths.others; }

private:
    void wifiMenu(void);
    void rfMenu(void);
    void bleMenu(void);
    void reconMenu(void);
    void detectionMenu(void);
    void commsMenu(void);
    void utilityMenu(void);
    void configMenu(void);
};

#endif
