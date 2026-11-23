#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <WiFi.h>
#include <globals.h>


#include "modules/others/qrcode_menu.h"


static const char *QR_URLS[] = {
    "http://192.168.4.1",
    "http://192.168.4.1/login",
    "http://192.168.4.1/update",
    "Custom URL...",
};
static const int NUM_URLS = sizeof(QR_URLS) / sizeof(QR_URLS[0]);

void arsenal_qr_poisoner(void) {
    ARSENAL_SAFE_RUN([]() {

        options.clear();
        int selectedUrl = -1;

        for (int i = 0; i < NUM_URLS; i++) {
            options.push_back({String(QR_URLS[i]), [i, &selectedUrl]() {
                selectedUrl = i;
            }});
        }

        loopOptions(options, MENU_TYPE_SUBMENU, "QR Target URL");

        if (selectedUrl < 0) return;

        String url;
        if (selectedUrl == NUM_URLS - 1) {

            url = "http://192.168.4.1";
        } else {
            url = String(QR_URLS[selectedUrl]);
        }


        drawMainBorderWithTitle("QR Poisoner");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, tftHeight - 30);
        tft.print(url.substring(0, 28));
        tft.drawCentreString(String("Esc to exit"), tftWidth / 2, tftHeight - 16, 1);


        int qrSize = min(tftWidth, tftHeight) - 60;
        int qrX = (tftWidth - qrSize) / 2;
        int qrY = 32;


        tft.fillRect(qrX, qrY, qrSize, qrSize, TFT_WHITE);
        tft.setTextColor(TFT_BLACK, TFT_WHITE);
        tft.setTextSize(FP);
        tft.drawCentreString(String("QR CODE"), tftWidth / 2, qrY + qrSize / 2 - 10, 1);
        tft.drawCentreString(url.substring(0, 20), tftWidth / 2, qrY + qrSize / 2 + 5, 1);


        while (true) {
            if (check(EscPress)) { returnToMenu = true; break; }
            delay(100);
        }
    });
}
