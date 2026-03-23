#include "AppsMenu.h"
#include "core/display.h"
#include "core/utils.h"
#if !defined(LITE_VERSION) && !defined(DISABLE_INTERPRETER)
#include "core/app_launcher.h"
#endif

void AppsMenu::optionsMenu() {
#if !defined(LITE_VERSION) && !defined(DISABLE_INTERPRETER)

    std::vector<AppManifest> apps = discoverApps();

    if (apps.empty()) {
        displayTextLine("No apps found.");
        displayTextLine("Put apps in /apps/ on SD.");
        delay(2000);
        return;
    }

    options.clear();
    for (size_t i = 0; i < apps.size(); i++) {
        AppManifest app = apps[i];
        String label = app.name;
        if (!app.version.isEmpty()) label += " v" + app.version;
        options.push_back({label.c_str(), [app]() { launchApp(app); }});
    }
    addOptionToMainMenu();

    loopOptions(options, MENU_TYPE_SUBMENU, "Apps");
#endif
}

void AppsMenu::drawIcon(float scale) {
    clearIconArea();

    int iconW = scale * 56;
    int iconH = scale * 56;
    if (iconW % 2 != 0) iconW++;
    if (iconH % 2 != 0) iconH++;

    int cx = iconCenterX;
    int cy = iconCenterY + (scale * 12);

    int gap = scale * 6;
    int sq = (iconW - gap) / 2;

    int x1 = cx - sq - gap / 2;
    int y1 = cy - sq - gap / 2;
    int x2 = cx + gap / 2;
    int y2 = cy + gap / 2;

    int r = scale * 5;

    tft.drawRoundRect(x1, y1, sq, sq, r, bruceConfig.priColor);
    tft.fillRoundRect(x2, y1, sq, sq, r, bruceConfig.priColor);
    tft.fillRoundRect(x1, y2, sq, sq, r, bruceConfig.priColor);
    tft.drawRoundRect(x2, y2, sq, sq, r, bruceConfig.priColor);
}
