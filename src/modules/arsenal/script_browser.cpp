#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include "core/utils.h"
#include <SD.h>
#include <globals.h>


extern void ducky_setup(void *hid, bool ble);
extern void *hid_usb;

static const char *ARSENAL_SCRIPTS_ROOT = "/arsenal";


struct ScriptCategory {
    const char *name;
    const char *path;
    const char *extensions;
    const char *description;
};

static const ScriptCategory CATEGORIES[] = {
    {"BadUSB/BadBLE", "/arsenal/badusb",  ".txt",          "DuckyScript payloads"},
    {"Sub-GHz",       "/arsenal/subghz",  ".sub",          "Sub-GHz signal captures"},
    {"IR Remotes",    "/arsenal/ir",      ".ir",           "Infrared remote signals"},
    {"Evil Portals",  "/arsenal/portals", ".html,.htm",    "Captive portal templates"},
    {"NFC",           "/arsenal/nfc",     ".nfc",          "NFC card dumps"},
    {"RFID",          "/arsenal/rfid",    ".rfid",         "125kHz RFID captures"},
    {"iButton",       "/arsenal/ibutton", ".ibutton",      "iButton key files"},
    {"JS Scripts",    "/arsenal/scripts", ".js",           "JavaScript automation"},
};
static const int NUM_CATEGORIES = sizeof(CATEGORIES) / sizeof(CATEGORIES[0]);


static int countFiles(const char *path, int maxDepth = 3, int depth = 0) {
    if (depth >= maxDepth) return 0;
    File dir = SD.open(path);
    if (!dir || !dir.isDirectory()) return 0;

    int count = 0;
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) break;

        if (entry.isDirectory()) {
            count += countFiles(entry.path(), maxDepth, depth + 1);
        } else {
            count++;
        }
        entry.close();

        if (count > 9999) break;
        esp_task_wdt_reset();
    }
    dir.close();
    return count;
}


static void executeScript(String filepath) {
    String ext = filepath.substring(filepath.lastIndexOf('.'));
    ext.toLowerCase();

    if (ext == ".txt") {


        drawMainBorderWithTitle("Running BadUSB");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("Script: " + filepath.substring(filepath.lastIndexOf('/') + 1));
        tft.setCursor(12, 70);
        tft.print("Executing DuckyScript...");


        File f = SD.open(filepath, FILE_READ);
        if (f) {
            tft.setCursor(12, 90);
            tft.print("Lines: " + String(f.size() / 20));
            f.close();
        }
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.drawCentreString(String("Press Sel to run, Esc to cancel"), tftWidth / 2, tftHeight - 20, 1);
        while (true) {
            if (check(EscPress)) { returnToMenu = true; return; }
            if (check(SelPress)) {

                displayRedStripe("Executing...");
                delay(1000);
                return;
            }
            delay(100);
        }
    } else if (ext == ".sub") {

        drawMainBorderWithTitle("Sub-GHz");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("Signal: " + filepath.substring(filepath.lastIndexOf('/') + 1));
        tft.setCursor(12, 70);
        tft.print("Press Sel to transmit");
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Esc to cancel"), tftWidth / 2, tftHeight - 20, 1);
        while (true) {
            if (check(EscPress)) { returnToMenu = true; return; }
            if (check(SelPress)) {

                displayRedStripe("Transmitting...");
                delay(2000);
                return;
            }
            delay(100);
        }
    } else if (ext == ".ir") {

        drawMainBorderWithTitle("IR Remote");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("IR: " + filepath.substring(filepath.lastIndexOf('/') + 1));
        tft.setCursor(12, 70);
        tft.print("Press Sel to transmit");
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Esc to cancel"), tftWidth / 2, tftHeight - 20, 1);
        while (true) {
            if (check(EscPress)) { returnToMenu = true; return; }
            if (check(SelPress)) {

                displayRedStripe("Transmitting IR...");
                delay(1000);
                return;
            }
            delay(100);
        }
    } else if (ext == ".html" || ext == ".htm") {

        drawMainBorderWithTitle("Evil Portal");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("Template: " + filepath.substring(filepath.lastIndexOf('/') + 1));
        File f = SD.open(filepath, FILE_READ);
        if (f) {
            tft.setCursor(12, 70);
            tft.print("Size: " + String(f.size()) + " bytes");
            f.close();
        }
        tft.setCursor(12, 90);
        tft.print("Press Sel to launch portal");
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Esc to cancel"), tftWidth / 2, tftHeight - 20, 1);
        while (true) {
            if (check(EscPress)) { returnToMenu = true; return; }
            if (check(SelPress)) {

                displayRedStripe("Launching portal...");
                delay(1000);
                return;
            }
            delay(100);
        }
    } else if (ext == ".js") {

        drawMainBorderWithTitle("JS Script");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("Script: " + filepath.substring(filepath.lastIndexOf('/') + 1));
        tft.setCursor(12, 70);
        tft.print("Press Sel to execute");
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Esc to cancel"), tftWidth / 2, tftHeight - 20, 1);
        while (true) {
            if (check(EscPress)) { returnToMenu = true; return; }
            if (check(SelPress)) {

                displayRedStripe("Running script...");
                delay(1000);
                return;
            }
            delay(100);
        }
    } else {

        drawMainBorderWithTitle("File Info");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("File: " + filepath.substring(filepath.lastIndexOf('/') + 1));
        tft.setCursor(12, 70);
        tft.print("Type: " + ext);
        File f = SD.open(filepath, FILE_READ);
        if (f) {
            tft.setCursor(12, 90);
            tft.print("Size: " + String(f.size()) + " bytes");
            f.close();
        }
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Press any key"), tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress) && !check(SelPress)) delay(100);
        returnToMenu = true;
    }
}


static void browseDirectory(String path, int depth) {
    options.clear();

    struct DirEntry {
        String name;
        String fullPath;
        bool isDir;
        size_t size;
    };
    std::vector<DirEntry> entries;

    File dir = SD.open(path);
    if (!dir || !dir.isDirectory()) {
        displayRedStripe("Cannot open folder");
        delay(1000);
        return;
    }

    while (true) {
        File entry = dir.openNextFile();
        if (!entry) break;

        DirEntry e;
        e.name = String(entry.name()).substring(String(entry.name()).lastIndexOf('/') + 1);
        e.fullPath = String(entry.path());
        e.isDir = entry.isDirectory();
        e.size = entry.size();
        entries.push_back(e);
        entry.close();

        if (entries.size() > 100) break;
        esp_task_wdt_reset();
    }
    dir.close();


    std::sort(entries.begin(), entries.end(), [](const DirEntry &a, const DirEntry &b) {
        if (a.isDir != b.isDir) return a.isDir > b.isDir;
        return a.name < b.name;
    });


    for (auto &e : entries) {
        String label;
        if (e.isDir) {
            label = "[" + e.name.substring(0, 18) + "]";
        } else {
            label = e.name.substring(0, 22);
        }

        options.push_back({label, [e, depth]() {
            if (e.isDir) {
                browseDirectory(e.fullPath, depth + 1);
            } else {
                executeScript(e.fullPath);
            }
        }});
    }

    if (entries.empty()) {
        options.push_back({"(empty folder)", []() {}});
    }

    String title = path.substring(path.lastIndexOf('/') + 1);
    if (title.length() == 0) title = "Scripts";
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, title.c_str());
}


void arsenal_script_browser(void) {
    if (ESP.getFreeHeap() < 20000) {
        displayRedStripe("Low memory!", TFT_WHITE, TFT_RED);
        delay(1500);
        return;
    }

    if (!setupSdCard()) {
        displayRedStripe("SD card required!");
        delay(1500);
        return;
    }


    if (!SD.exists(ARSENAL_SCRIPTS_ROOT)) {
        drawMainBorderWithTitle("Script Browser");
        tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
        tft.setTextSize(FP);
        int y = 45;
        tft.setCursor(12, y); tft.print("No scripts found!"); y += 16;
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setCursor(12, y); tft.print("Clone the repo to SD:"); y += 14;
        tft.setCursor(12, y); tft.print("/arsenal/badusb/"); y += 12;
        tft.setCursor(12, y); tft.print("/arsenal/subghz/"); y += 12;
        tft.setCursor(12, y); tft.print("/arsenal/ir/"); y += 12;
        tft.setCursor(12, y); tft.print("/arsenal/portals/"); y += 12;
        tft.setCursor(12, y); tft.print("/arsenal/nfc/"); y += 12;
        tft.setCursor(12, y); tft.print("/arsenal/rfid/"); y += 12;
        tft.setCursor(12, y); tft.print("/arsenal/scripts/"); y += 16;
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Press any key"), tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress) && !check(SelPress)) delay(100);
        returnToMenu = true;
        return;
    }


    options.clear();
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        const ScriptCategory &cat = CATEGORIES[i];
        String label = String(cat.name);

        if (SD.exists(cat.path)) {

            File d = SD.open(cat.path);
            int count = 0;
            if (d && d.isDirectory()) {
                while (true) {
                    File f = d.openNextFile();
                    if (!f) break;
                    count++;
                    f.close();
                    if (count > 999) break;
                }
                d.close();
            }
            label += " (" + String(count) + (count > 999 ? "+" : "") + ")";
        } else {
            label += " (empty)";
        }

        options.push_back({label, [cat]() {
            if (SD.exists(cat.path)) {
                browseDirectory(String(cat.path), 0);
            } else {
                displayRedStripe("Folder not found");
                delay(1000);
            }
        }});
    }


    options.push_back({"Browse All...", []() {
        browseDirectory(String(ARSENAL_SCRIPTS_ROOT), 0);
    }});

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Script Browser");
}
