#include "ir_remote_app.h"
#include "custom_ir.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/utils.h"
#include <globals.h>

// Each button maps to an IR protocol + address + command. Values are the
// hex payloads straight out of the .ir file format this firmware already
// parses (address/command as little-endian hex strings). The database is
// small on purpose: the common buttons a universal remote needs. More
// brands can be added by appending entries to REMOTES below.

struct IrButton {
    const char *label;
    const char *protocol;
    const char *address;   // hex, little-endian, matches .ir "address:"
    const char *command;   // hex, little-endian, matches .ir "command:"
    uint8_t bits;
};

struct IrRemote {
    const char *brand;
    const IrButton *buttons;
    uint8_t buttonCount;
};

// ---- LG (NEC, address 04) ----
static const IrButton LG_BTN[] = {
    {"Power",   "NEC", "04 00 00 00", "08 00 00 00", 32},
    {"Mute",    "NEC", "04 00 00 00", "09 00 00 00", 32},
    {"Vol+",    "NEC", "04 00 00 00", "02 00 00 00", 32},
    {"Vol-",    "NEC", "04 00 00 00", "03 00 00 00", 32},
    {"CH+",     "NEC", "04 00 00 00", "00 00 00 00", 32},
    {"CH-",     "NEC", "04 00 00 00", "01 00 00 00", 32},
    {"Source",  "NEC", "04 00 00 00", "0B 00 00 00", 32},
    {"Menu",    "NEC", "04 00 00 00", "43 00 00 00", 32},
    {"OK",      "NEC", "04 00 00 00", "41 00 00 00", 32},
    {"Up",      "NEC", "04 00 00 00", "40 00 00 00", 32},
    {"Down",    "NEC", "04 00 00 00", "41 00 00 00", 32},
    {"Back",    "NEC", "04 00 00 00", "5A 00 00 00", 32},
};

// ---- Samsung (Samsung, address 07) ----
static const IrButton SAMSUNG_BTN[] = {
    {"Power",   "SAMSUNG", "07 00 00 00", "02 00 00 00", 32},
    {"Mute",    "SAMSUNG", "07 00 00 00", "0F 00 00 00", 32},
    {"Vol+",    "SAMSUNG", "07 00 00 00", "10 00 00 00", 32},
    {"Vol-",    "SAMSUNG", "07 00 00 00", "11 00 00 00", 32},
    {"CH+",     "SAMSUNG", "07 00 00 00", "12 00 00 00", 32},
    {"CH-",     "SAMSUNG", "07 00 00 00", "13 00 00 00", 32},
    {"Source",  "SAMSUNG", "07 00 00 00", "0A 00 00 00", 32},
    {"Menu",    "SAMSUNG", "07 00 00 00", "1A 00 00 00", 32},
    {"OK",      "SAMSUNG", "07 00 00 00", "58 00 00 00", 32},
    {"Up",      "SAMSUNG", "07 00 00 00", "1A 00 00 00", 32},
    {"Down",    "SAMSUNG", "07 00 00 00", "1B 00 00 00", 32},
    {"Back",    "SAMSUNG", "07 00 00 00", "58 00 00 00", 32},
};

// ---- Sony (Sony, address 01) ----
static const IrButton SONY_BTN[] = {
    {"Power",   "SONY",   "01 00 00 00", "15 00 00 00", 20},
    {"Mute",    "SONY",   "01 00 00 00", "14 00 00 00", 20},
    {"Vol+",    "SONY",   "01 00 00 00", "12 00 00 00", 20},
    {"Vol-",    "SONY",   "01 00 00 00", "13 00 00 00", 20},
    {"CH+",     "SONY",   "01 00 00 00", "10 00 00 00", 20},
    {"CH-",     "SONY",   "01 00 00 00", "11 00 00 00", 20},
    {"Source",  "SONY",   "01 00 00 00", "A5 00 00 00", 20},
    {"Menu",    "SONY",   "01 00 00 00", "4D 00 00 00", 20},
    {"OK",      "SONY",   "01 00 00 00", "4D 00 00 00", 20},
    {"Up",      "SONY",   "01 00 00 00", "4D 00 00 00", 20},
    {"Down",    "SONY",   "01 00 00 00", "4E 00 00 00", 20},
    {"Back",    "SONY",   "01 00 00 00", "81 00 00 00", 20},
};

// ---- Panasonic (Kaseikyo, address 02) ----
static const IrButton PANASONIC_BTN[] = {
    {"Power",   "KASEIKYO", "02 00 00 00", "38 00 00 00", 48},
    {"Mute",    "KASEIKYO", "02 00 00 00", "32 00 00 00", 48},
    {"Vol+",    "KASEIKYO", "02 00 00 00", "34 00 00 00", 48},
    {"Vol-",    "KASEIKYO", "02 00 00 00", "35 00 00 00", 48},
    {"CH+",     "KASEIKYO", "02 00 00 00", "30 00 00 00", 48},
    {"CH-",     "KASEIKYO", "02 00 00 00", "31 00 00 00", 48},
    {"Source",  "KASEIKYO", "02 00 00 00", "39 00 00 00", 48},
    {"Menu",    "KASEIKYO", "02 00 00 00", "8B 00 00 00", 48},
    {"OK",      "KASEIKYO", "02 00 00 00", "8D 00 00 00", 48},
    {"Up",      "KASEIKYO", "02 00 00 00", "8A 00 00 00", 48},
    {"Down",    "KASEIKYO", "02 00 00 00", "8B 00 00 00", 48},
    {"Back",    "KASEIKYO", "02 00 00 00", "8C 00 00 00", 48},
};

static const IrRemote REMOTES[] = {
    {"LG",       LG_BTN,        sizeof(LG_BTN)/sizeof(LG_BTN[0])},
    {"Samsung",  SAMSUNG_BTN,   sizeof(SAMSUNG_BTN)/sizeof(SAMSUNG_BTN[0])},
    {"Sony",     SONY_BTN,      sizeof(SONY_BTN)/sizeof(SONY_BTN[0])},
    {"Panasonic",PANASONIC_BTN, sizeof(PANASONIC_BTN)/sizeof(PANASONIC_BTN[0])},
};
static const int REMOTE_COUNT = sizeof(REMOTES)/sizeof(REMOTES[0]);

static void sendAppButton(const IrButton &b) {
    // Build the same IRCode the .ir parser produces and hand it to the
    // existing sender so all protocol encoders (NEC/Samsung/Sony/Kaseikyo)
    // are reused instead of re-implementing them here.
    String addr = b.address;
    String cmd = b.command;
    // value field is unused by the parsed path; pass 0.
    IRCode code(String(b.protocol), addr, cmd, String("0"), b.bits);
    sendIRCommand(&code, true);
}

void ir_remote_app() {
    // 1. pick brand
    options.clear();
    int picked = -1;
    for (int i = 0; i < REMOTE_COUNT; i++) {
        int idx = i;
        options.push_back({REMOTES[i].brand, [idx, &picked]() { picked = idx; }});
    }
    options.push_back({"Back", []() { returnToMenu = true; }});
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "IR Remote - Brand");
    if (picked < 0) return;

    const IrRemote &remote = REMOTES[picked];

    // 2. button grid loop
    while (true) {
        options.clear();
        for (int i = 0; i < remote.buttonCount; i++) {
            int idx = i;
            options.push_back({remote.buttons[i].label, [idx, remote]() {
                sendAppButton(remote.buttons[idx]);
                displayRedStripe("Sent!", getComplementaryColor2(bruceConfig.priColor), bruceConfig.priColor);
                delay(400);
            }});
        }
        options.push_back({"Back", []() { returnToMenu = true; }});
        addOptionToMainMenu();
        String title = String("IR Remote - ") + remote.brand;
        loopOptions(options, MENU_TYPE_SUBMENU, title.c_str());
        if (returnToMenu) break;
    }
}
