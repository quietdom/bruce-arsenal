#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "modules/rf/rf_utils.h"
#include <globals.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>

struct CapturedFob {
    float frequency;
    uint32_t protocol;
    uint64_t code;
    uint8_t bits;
    int rssi;
    unsigned long timestamp;
};

#define FOB_RING_SIZE 16
static CapturedFob fobRing[FOB_RING_SIZE];
static int fobIdx = 0;
static int fobCount = 0;

void arsenal_car_keyfob_logger(void) {
    ARSENAL_HEAP_CHECK();
    if (bruceConfigPins.rfModule != CC1101_SPI_MODULE) {
        displayRedStripe("CC1101 module not found");
        delay(1500);
        return;
    }
    memset(fobRing, 0, sizeof(fobRing));
    fobIdx = 0;
    fobCount = 0;

    float freqs[] = {315.0, 433.92, 868.35, 915.0};
    int freqCount = 4;
    int freqIdx = 0;

    drawMainBorderWithTitle("Keyfob Logger");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(12, 50);
    tft.printf("Scanning: %.2f MHz", freqs[freqIdx]);
    tft.setCursor(12, 66);
    tft.print("Press any RF fob/key");
    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
    tft.drawCentreString("Esc:stop  Sel:freq", tftWidth / 2, tftHeight - 20, 1);
    delay(1000);

    unsigned long startTime = millis();

    while (true) {
        deinitRfModule();
        delay(50);
        if (!initRfModule("rx", freqs[freqIdx])) {
            displayRedStripe("RF init failed");
            delay(1500);
            return;
        }

        ELECHOUSE_cc1101.SetRx();
        unsigned long scanStart = millis();

        while (millis() - scanStart < 3000) {
            if (check(EscPress)) goto done;
            if (check(SelPress)) {
                while (check(SelPress)) delay(10);
                freqIdx = (freqIdx + 1) % freqCount;
                scanStart = millis();
                break;
            }

            if (ELECHOUSE_cc1101.checkRssi(100)) {
                int rssi = ELECHOUSE_cc1101.getRssi();
                if (rssi > -70) {
                    uint32_t code = ELECHOUSE_cc1101.CheckRcv();
                    if (code != 0) {
                        CapturedFob &f = fobRing[fobIdx];
                        f.frequency = freqs[freqIdx];
                        f.code = code;
                        f.bits = 24;
                        f.rssi = rssi;
                        f.timestamp = millis();
                        fobIdx = (fobIdx + 1) % FOB_RING_SIZE;
                        if (fobCount < FOB_RING_SIZE) fobCount++;
                    }
                }
            }
            esp_task_wdt_reset();
            delay(5);
        }

        drawMainBorderWithTitle("Keyfob Logger");
        int y = 38;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, y);
        tft.printf("Freq: %.2f MHz", freqs[freqIdx]);
        y += 14;
        tft.setCursor(12, y);
        tft.printf("Captured: %d", fobCount);
        y += 14;
        int shown = 0;
        for (int off = 0; off < FOB_RING_SIZE && shown < 6; off++) {
            int i = (fobIdx - 1 - off + FOB_RING_SIZE) % FOB_RING_SIZE;
            if (fobRing[i].frequency == 0) continue;
            tft.setCursor(12, y);
            tft.printf("%.0fMHz %08lX %d", fobRing[i].frequency, (unsigned long)fobRing[i].code, (int)fobRing[i].rssi);
            y += 12;
            shown++;
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString("Esc:stop  Sel:freq", tftWidth / 2, tftHeight - 20, 1);
    }

done:
    deinitRfModule();

    if (fobCount > 0 && setupSdCard()) {
        if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
        File f = SD.open("/arsenal/keyfobs.log", FILE_APPEND);
        if (f) {
            for (int i = 0; i < FOB_RING_SIZE; i++) {
                if (fobRing[i].frequency == 0) continue;
                f.printf("%.2f,%08lX,%d,%d,%lu\n",
                    fobRing[i].frequency, (unsigned long)fobRing[i].code,
                    (int)fobRing[i].bits, (int)fobRing[i].rssi, fobRing[i].timestamp);
            }
            f.close();
        }
    }
}
