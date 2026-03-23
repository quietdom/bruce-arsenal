#include "EthernetMenu.h"
#if !defined(LITE_VERSION)
#include "core/display.h"
#include "core/settings.h"
#include "core/utils.h"
#include "modules/ethernet/ARPScanner.h"
#include "modules/ethernet/DHCPStarvation.h"
#include "modules/ethernet/EthernetHelper.h"
#include "modules/ethernet/MACFlooding.h"

void EthernetMenu::start_ethernet() {
    eth = new EthernetHelper();
    if (!eth->setup()) {
        displayError("W5500 not found");
        delete eth;
        eth = nullptr;
        return;
    }
    while (!eth->is_connected()) { delay(100); }
}

void EthernetMenu::optionsMenu() {
    options = {
        {"Scan Hosts",
         [this]() {
             start_ethernet();
             if (eth != nullptr) {
                 run_arp_scanner();
                 eth->stop();
             } else {
                    displayError("W5500 not found");
             }
         }                        },
        {"DHCP Starvation",
         [this]() {
             start_ethernet();
             if (eth != nullptr) {
                 DHCPStarvation();
                 eth->stop();
             } else {
                    displayError("W5500 not found");
             }
         }                        },
        {"MAC Flooding",    [this]() {
             start_ethernet();
             if (eth != nullptr) {
                 MACFlooding();
                 eth->stop();
             } else {
                    displayError("W5500 not found");
             }
         }}
    };
    addOptionToMainMenu();

    delay(200);

    loopOptions(options, MENU_TYPE_SUBMENU, "Ethernet");
}

void EthernetMenu::drawIcon(float scale) {
    clearIconArea();

    int cx = iconCenterX;
    int cy = iconCenterY;

    // Dimensions
    int w = 48 * scale;
    int h = 40 * scale;
    // Make sure they remain even for absolute symmetry
    if (w % 2 != 0) w++;
    if (h % 2 != 0) h++;

    int x1 = cx - w / 2;
    int x2 = cx + w / 2;
    int y1 = cy - h / 2;
    int y2 = cy + h / 2;

    int t = 3 * scale; // Thickness of the connector walls
    if (t < 2) t = 2;

    int nw = 20 * scale; // Notch width
    if (nw % 2 != 0) nw++;
    int nh = 12 * scale; // Notch height

    // --- Outer Frame ---
    // Top border
    tft.fillRect(x1, y1, w, t, bruceConfig.priColor);
    // Left border
    tft.fillRect(x1, y1, t, h, bruceConfig.priColor);
    // Right border
    tft.fillRect(x2 - t, y1, t, h, bruceConfig.priColor);
    
    // Bottom Left piece
    int sideW = (w - nw) / 2 + t;
    tft.fillRect(x1, y2 - t, sideW, t, bruceConfig.priColor);
    // Bottom Right piece
    tft.fillRect(x2 - sideW, y2 - t, sideW, t, bruceConfig.priColor);

    // Notch Vertical Left
    tft.fillRect(cx - nw / 2, y2 - nh - t, t, nh + t, bruceConfig.priColor);
    // Notch Vertical Right
    tft.fillRect(cx + nw / 2 - t, y2 - nh - t, t, nh + t, bruceConfig.priColor);
    // Notch Top Horizontal
    tft.fillRect(cx - nw / 2, y2 - nh - t, nw, t, bruceConfig.priColor);

    // --- 8 Pins ---
    int pinW = 2 * scale;
    if (pinW < 1) pinW = 1;
    int pinH = (h / 2) - t; // Make pins hang down halfway inside

    int step = (28 * scale) / 7; // Distribute 8 pins over 7 gaps
    if (step < 2) step = 2;
    int totalSpread = step * 7; 
    int firstPinX = cx - (totalSpread / 2);
    
    for (int i = 0; i < 8; i++) {
        int px = firstPinX + (i * step) - (pinW / 2);
        // Draw pins clearly separated
        tft.fillRect(px, y1 + t, pinW, pinH, bruceConfig.priColor);
    }
}
#endif
