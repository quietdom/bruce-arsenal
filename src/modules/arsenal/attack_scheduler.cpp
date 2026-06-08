#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <globals.h>

struct ScheduledTask {
    String name;
    unsigned long startAfterMs;
    unsigned long durationMs;
    bool started;
    bool finished;
    void (*startFunc)(void);
};

static std::vector<ScheduledTask> scheduledTasks;
static bool schedulerRunning = false;
static unsigned long schedulerStartTime = 0;


struct SchedulableFeature {
    const char *name;
    void (*func)(void);
};

static const SchedulableFeature FEATURES[] = {
    {"DHCP Starvation",  arsenal_dhcp_starvation},
    {"DNS Spoofer",      arsenal_dns_spoofer},
#ifndef LITE_VERSION
    {"Karma Attack",     arsenal_karma_attack},
#endif
    {"BT Name Spam",     arsenal_bt_name_spammer},
    {"AirTag Spoofer",   arsenal_airtag_spoofer},
    {"Decoy Traffic",    arsenal_decoy_traffic},
    {"Channel Hopper",   arsenal_channel_hopper},
};
static const int NUM_FEATURES = sizeof(FEATURES) / sizeof(FEATURES[0]);

void arsenal_attack_scheduler(void) {
    ARSENAL_SAFE_RUN([]() {
        scheduledTasks.clear();


        options.clear();
        int selectedFeature = -1;
        for (int i = 0; i < NUM_FEATURES; i++) {
            options.push_back({String(FEATURES[i].name), [i, &selectedFeature]() {
                selectedFeature = i;
            }});
        }
        loopOptions(options, MENU_TYPE_SUBMENU, "Schedule What?");
        if (selectedFeature < 0) return;


        options.clear();
        unsigned long delayMs = 0;
        options.push_back({"Now (0 delay)",    [&delayMs]() { delayMs = 0; }});
        options.push_back({"30 seconds",       [&delayMs]() { delayMs = 30000; }});
        options.push_back({"1 minute",         [&delayMs]() { delayMs = 60000; }});
        options.push_back({"2 minutes",        [&delayMs]() { delayMs = 120000; }});
        options.push_back({"5 minutes",        [&delayMs]() { delayMs = 300000; }});
        options.push_back({"10 minutes",       [&delayMs]() { delayMs = 600000; }});
        options.push_back({"30 minutes",       [&delayMs]() { delayMs = 1800000; }});
        options.push_back({"1 hour",           [&delayMs]() { delayMs = 3600000; }});
        loopOptions(options, MENU_TYPE_SUBMENU, "Start After?");


        options.clear();
        unsigned long durationMs = 0;
        options.push_back({"Until stopped",    [&durationMs]() { durationMs = 0; }});
        options.push_back({"30 seconds",       [&durationMs]() { durationMs = 30000; }});
        options.push_back({"1 minute",         [&durationMs]() { durationMs = 60000; }});
        options.push_back({"2 minutes",        [&durationMs]() { durationMs = 120000; }});
        options.push_back({"5 minutes",        [&durationMs]() { durationMs = 300000; }});
        options.push_back({"10 minutes",       [&durationMs]() { durationMs = 600000; }});
        loopOptions(options, MENU_TYPE_SUBMENU, "Run For?");


        ScheduledTask task;
        task.name = String(FEATURES[selectedFeature].name);
        task.startAfterMs = delayMs;
        task.durationMs = durationMs;
        task.started = false;
        task.finished = false;
        task.startFunc = FEATURES[selectedFeature].func;
        scheduledTasks.push_back(task);


        schedulerStartTime = millis();
        schedulerRunning = true;

        while (schedulerRunning) {
            unsigned long elapsed = millis() - schedulerStartTime;

            drawMainBorderWithTitle("Scheduler");
            int y = 42;
            int padX = 10;

            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
            tft.setTextSize(FP);

            for (auto &t : scheduledTasks) {
                tft.setCursor(padX, y);

                if (t.finished) {
                    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
                    tft.print("[DONE] " + t.name);
                } else if (t.started) {
                    tft.setTextColor(TFT_RED, bruceConfig.bgColor);
                    tft.print("[RUNNING] " + t.name);
                    if (t.durationMs > 0) {
                        unsigned long remaining = t.durationMs - (elapsed - t.startAfterMs);
                        y += 12;
                        tft.setCursor(padX + 8, y);
                        tft.printf("Stops in: %lus", remaining / 1000);
                    }
                } else {
                    tft.setTextColor(TFT_YELLOW, bruceConfig.bgColor);
                    unsigned long startsIn = t.startAfterMs - elapsed;
                    tft.print("[WAITING] " + t.name);
                    y += 12;
                    tft.setCursor(padX + 8, y);
                    tft.printf("Starts in: %lus", startsIn / 1000);
                }
                y += 16;
            }

            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
            tft.drawCentreString(String("Esc to cancel all"), tftWidth / 2, tftHeight - 18, 1);


            for (auto &t : scheduledTasks) {
                if (!t.started && elapsed >= t.startAfterMs) {
                    t.started = true;

                    t.startFunc();
                    t.finished = true;
                    schedulerRunning = false;
                    break;
                }
            }

            if (check(EscPress)) {
                schedulerRunning = false;
                break;
            }
            esp_task_wdt_reset();
            delay(200);
        }

        displayRedStripe("Scheduler stopped");
        delay(1000);
    });
}
