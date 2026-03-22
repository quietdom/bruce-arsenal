#ifndef __APP_STORE_MENU_H__
#define __APP_STORE_MENU_H__

#include <MenuItemInterface.h>

#if !defined(LITE_VERSION) && !defined(DISABLE_INTERPRETER)

class AppStoreMenu : public MenuItemInterface {
public:
    AppStoreMenu() : MenuItemInterface("App Store") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return false; }
    String themePath() { return ""; }
};

#endif
#endif
