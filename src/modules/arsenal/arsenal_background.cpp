#include "arsenal_background.h"
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include "core/utils.h"
#include <SD.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>


volatile ArsenalOpsecLevel g_opsecLevel = OPSEC_CLEAR;
volatile bool g_arsenalEvasionActive = false;
volatile int g_arsenalDeauthsDetected = 0;
volatile int g_arsenalCredsCapture = 0;
volatile bool g_arsenalLowPowerMode = false;

static TaskHandle_t backgroundTask = nullptr;
static bool backgroundRunning = false;


static unsigned long lastMACRotate = 0;
static unsigned long lastChannelHop = 0;
static unsigned long lastDecoyFrame = 0;
static uint8_t evasionChannel = 1;


static const unsigned long MAC_ROTATE_NORMAL = 30000;
static const unsigned long CHANNEL_HOP_NORMAL = 200;
static const unsigned long DECOY_NORMAL = 100;
static const unsigned long TASK_DELAY_NORMAL = 50;


static const unsigned long MAC_ROTATE_LOW = 120000;
static const unsigned long CHANNEL_HOP_LOW = 1000;
static const unsigned long DECOY_LOW = 500;
static const unsigned long TASK_DELAY_LOW = 200;

static inline unsigned long getMACInterval() { return g_arsenalLowPowerMode ? MAC_ROTATE_LOW : MAC_ROTATE_NORMAL; }
static inline unsigned long getHopInterval() { return g_arsenalLowPowerMode ? CHANNEL_HOP_LOW : CHANNEL_HOP_NORMAL; }
static inline unsigned long getDecoyInterval() { return g_arsenalLowPowerMode ? DECOY_LOW : DECOY_NORMAL; }
static inline unsigned long getTaskDelay() { return g_arsenalLowPowerMode ? TASK_DELAY_LOW : TASK_DELAY_NORMAL; }


static uint8_t decoy_beacon[] = {
    0x80, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x00, 0x11, 0x00,
    0x00, 0x06, 'N', 'O', 'I', 'S', 'E', '!'
};


static void opsecPassiveCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *frame = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    if (len < 24) return;

    uint8_t subtype = (frame[0] >> 4) & 0x0F;


    if (subtype == 0x0C || subtype == 0x0A) {
        g_arsenalDeauthsDetected = g_arsenalDeauthsDetected + 1;
    }
}


static void arsenalBackgroundLoop(void *param) {
    while (backgroundRunning) {
        unsigned long now = millis();


        if (g_arsenalDeauthsDetected > 10) {
            g_opsecLevel = OPSEC_DANGER;
        } else if (g_arsenalDeauthsDetected > 3) {
            g_opsecLevel = OPSEC_CAUTION;
        } else {
            g_opsecLevel = OPSEC_CLEAR;
        }


        static unsigned long lastReset = 0;
        if (now - lastReset > 60000) {
            g_arsenalDeauthsDetected = 0;
            lastReset = now;
        }


        if (g_arsenalEvasionActive) {

            if (now - lastMACRotate > getMACInterval()) {
                uint8_t mac[6];
                for (int i = 0; i < 6; i++) mac[i] = random(256);
                mac[0] |= 0x02;
                mac[0] &= 0xFE;
                esp_wifi_set_mac(WIFI_IF_STA, mac);
                lastMACRotate = now;
            }


            if (now - lastChannelHop > getHopInterval()) {
                evasionChannel = (evasionChannel % 14) + 1;
                esp_wifi_set_channel(evasionChannel, WIFI_SECOND_CHAN_NONE);
                lastChannelHop = now;
            }


            if (now - lastDecoyFrame > getDecoyInterval()) {
                for (int i = 0; i < 6; i++) decoy_beacon[10 + i] = random(256);
                decoy_beacon[10] |= 0x02;
                memcpy(decoy_beacon + 16, decoy_beacon + 10, 6);
                for (int i = 0; i < 6; i++) decoy_beacon[36 + i] = 'A' + random(26);
                esp_wifi_80211_tx(WIFI_IF_STA, decoy_beacon, sizeof(decoy_beacon), false);
                lastDecoyFrame = now;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(getTaskDelay()));
    }
    vTaskDelete(NULL);
}


void arsenal_background_start(void) {
    if (backgroundRunning) return;
    backgroundRunning = true;


    if (WiFi.getMode() == WIFI_MODE_NULL) {
        WiFi.mode(WIFI_STA);
    }


    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(opsecPassiveCallback);


    xTaskCreatePinnedToCore(arsenalBackgroundLoop, "arsenal_bg", 4096, NULL, 1, &backgroundTask, 0);
}

void arsenal_background_stop(void) {
    if (!backgroundRunning) return;
    backgroundRunning = false;
    g_arsenalEvasionActive = false;
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_wifi_set_promiscuous(false);
}

bool arsenal_background_is_running(void) {
    return backgroundRunning;
}


void arsenal_draw_opsec_dot(void) {

    int dotX = 2;
    int dotY = 12;
    int dotR = 4;

    uint16_t color;
    switch (g_opsecLevel) {
        case OPSEC_CLEAR:   color = TFT_GREEN; break;
        case OPSEC_CAUTION: color = TFT_YELLOW; break;
        case OPSEC_DANGER:  color = TFT_RED; break;
        default:            color = TFT_GREEN; break;
    }

    tft.fillCircle(dotX, dotY, dotR, color);
}


void arsenal_evasion_toggle(void) {
    if (!backgroundRunning) {
        arsenal_background_start();
    }
    g_arsenalEvasionActive = !g_arsenalEvasionActive;
}

bool arsenal_evasion_is_active(void) {
    return g_arsenalEvasionActive;
}


struct ComboPreset {
    String name;
    std::vector<String> features;
};

static std::vector<ComboPreset> loadCombos() {
    std::vector<ComboPreset> combos;


    ComboPreset stealth;
    stealth.name = "Stealth Mode";
    stealth.features = {"mac_rotator", "channel_hopper", "decoy_traffic"};
    combos.push_back(stealth);

    ComboPreset fullAttack;
    fullAttack.name = "Full Attack";
    fullAttack.features = {"dhcp_starvation", "dns_spoofer", "karma_attack", "bt_spammer"};
    combos.push_back(fullAttack);

    ComboPreset recon;
    recon.name = "Passive Recon";
    recon.features = {"ble_tracker", "device_fingerprinter", "opsec_monitor"};
    combos.push_back(recon);


    if (setupSdCard() && SD.exists("/arsenal/combos")) {
        File dir = SD.open("/arsenal/combos");
        while (true) {
            File entry = dir.openNextFile();
            if (!entry) break;
            if (!entry.isDirectory()) {
                String filename = String(entry.name());
                filename = filename.substring(filename.lastIndexOf('/') + 1);
                if (filename.endsWith(".txt")) {
                    ComboPreset userCombo;
                    userCombo.name = filename.substring(0, filename.length() - 4);
                    while (entry.available()) {
                        String line = entry.readStringUntil('\n');
                        line.trim();
                        if (line.length() > 0 && !line.startsWith("#")) {
                            userCombo.features.push_back(line);
                        }
                    }
                    if (!userCombo.features.empty()) {
                        combos.push_back(userCombo);
                    }
                }
            }
            entry.close();
        }
        dir.close();
    }

    return combos;
}

void arsenal_combo_menu(void) {
    auto combos = loadCombos();

    options.clear();
    for (auto &combo : combos) {
        String label = combo.name + " (" + String(combo.features.size()) + ")";
        options.push_back({label, [combo]() {
            drawMainBorderWithTitle("Combo");
            tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
            tft.setTextSize(FP);
            tft.setCursor(12, 45);
            tft.print("Running: " + combo.name);
            int y = 65;
            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            for (auto &f : combo.features) {
                tft.setCursor(12, y);
                tft.print("+ " + f);
                y += 12;
            }
            tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
            tft.drawCentreString(String("Activating..."), tftWidth / 2, tftHeight - 20, 1);
            delay(1500);


            for (auto &f : combo.features) {
                if (f == "mac_rotator" || f == "channel_hopper" || f == "decoy_traffic") {
                    if (!arsenal_background_is_running()) arsenal_background_start();
                    g_arsenalEvasionActive = true;
                    break;
                }
            }
        }});
    }


    options.push_back({"+ Create New Combo", []() {
        drawMainBorderWithTitle("New Combo");
        tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
        tft.setTextSize(FP);
        tft.setCursor(12, 50);
        tft.print("Create a .txt file in:");
        tft.setCursor(12, 66);
        tft.print("/arsenal/combos/");
        tft.setCursor(12, 86);
        tft.print("One feature per line:");
        tft.setCursor(12, 102);
        tft.print("mac_rotator");
        tft.setCursor(12, 114);
        tft.print("channel_hopper");
        tft.setCursor(12, 126);
        tft.print("decoy_traffic");
        tft.setTextColor(TFT_RED, bruceConfig.bgColor);
        tft.drawCentreString(String("Press any key"), tftWidth / 2, tftHeight - 20, 1);
        while (!check(EscPress) && !check(SelPress)) delay(100);
    }});


    String evasionLabel = g_arsenalEvasionActive ? "Evasion: ON (stop)" : "Evasion: OFF (start)";
    options.push_back({evasionLabel, []() {
        arsenal_evasion_toggle();
        displayRedStripe(g_arsenalEvasionActive ? "Evasion ON" : "Evasion OFF");
        delay(1000);
    }});


    String powerLabel = g_arsenalLowPowerMode ? "Power: LOW (switch to normal)" : "Power: NORMAL (switch to low)";
    options.push_back({powerLabel, []() {
        g_arsenalLowPowerMode = !g_arsenalLowPowerMode;
        displayRedStripe(g_arsenalLowPowerMode ? "Low Power ON" : "Normal Power");
        delay(1000);
    }});

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Combo Presets");
}
