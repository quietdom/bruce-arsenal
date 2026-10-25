#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include <SD.h>
#include <globals.h>

static File logFile;
static bool loggingActive = false;
static String currentLogPath = "";
static int logEntries = 0;


void arsenal_log_start(void) {
    if (loggingActive) return;
    if (!setupSdCard()) return;

    if (!SD.exists("/arsenal/logs")) {
        SD.mkdir("/arsenal");
        SD.mkdir("/arsenal/logs");
    }


    unsigned long ts = millis();
    currentLogPath = "/arsenal/logs/session_" + String(ts) + ".txt";
    logFile = SD.open(currentLogPath, FILE_WRITE);
    if (logFile) {
        loggingActive = true;
        logEntries = 0;
        logFile.println("═══ Arsenal Session Log ═══");
        logFile.printf("Started: %lu ms after boot\n", ts);
        logFile.println("═══════════════════════════");
        logFile.println();
        logFile.flush();
    }
}


void arsenal_log_event(const char *category, const char *message) {
    if (!loggingActive || !logFile) return;

    unsigned long ts = millis() / 1000;
    logFile.printf("[%05lu] [%s] %s\n", ts, category, message);
    logFile.flush();
    logEntries++;
}


void arsenal_log_event(const char *category, String message) {
    arsenal_log_event(category, message.c_str());
}


void arsenal_log_stop(void) {
    if (!loggingActive) return;
    logFile.printf("\n═══ Session ended. %d events logged. ═══\n", logEntries);
    logFile.close();
    loggingActive = false;
}


void arsenal_session_log_menu(void) {
    ARSENAL_SAFE_RUN([]() {
        options.clear();


        if (loggingActive) {
            options.push_back({"Stop Logging", []() {
                arsenal_log_stop();
                displayRedStripe("Logging stopped");
                delay(1000);
            }});
            options.push_back({"Log: " + currentLogPath.substring(currentLogPath.lastIndexOf('/') + 1), []() {}});
            options.push_back({"Entries: " + String(logEntries), []() {}});
        } else {
            options.push_back({"Start Logging", []() {
                arsenal_log_start();
                if (loggingActive) {
                    displayRedStripe("Logging started!");
                } else {
                    displayRedStripe("SD card needed!");
                }
                delay(1000);
            }});
        }


        options.push_back({"View Past Logs", []() {
            if (!setupSdCard() || !SD.exists("/arsenal/logs")) {
                displayRedStripe("No logs found");
                delay(1000);
                return;
            }

            std::vector<Option> logOpts;
            File dir = SD.open("/arsenal/logs");
            while (true) {
                File entry = dir.openNextFile();
                if (!entry) break;
                if (!entry.isDirectory()) {
                    String name = String(entry.name());
                    name = name.substring(name.lastIndexOf('/') + 1);
                    size_t size = entry.size();
                    logOpts.push_back({name + " (" + String(size) + "B)", [name]() {

                        File f = SD.open("/arsenal/logs/" + name, FILE_READ);
                        if (f) {
                            drawMainBorderWithTitle("Log Viewer");
                            tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
                            tft.setTextSize(FONTSZ);
                            int y = 35;
                            int lines = 0;
                            while (f.available() && lines < 10) {
                                String line = f.readStringUntil('\n');
                                tft.setCursor(8, y);
                                tft.print(line.substring(0, 30));
                                y += 11;
                                lines++;
                            }
                            f.close();
                            tft.setTextColor(TFT_RED, bruceConfig.bgColor);
                            tft.drawCentreString(String("Press any key"), tftWidth / 2, tftHeight - 18, 1);
                            while (!check(EscPress) && !check(SelPress)) delay(100);
                            returnToMenu = true;
                        }
                    }});
                }
                entry.close();
            }
            dir.close();

            if (logOpts.empty()) {
                logOpts.push_back({"No logs found", []() {}});
            }
            addOptionToMainMenu();
            loopOptions(logOpts, MENU_TYPE_SUBMENU, "Session Logs");
        }});


        options.push_back({"Clear All Logs", []() {
            if (!setupSdCard()) return;
            File dir = SD.open("/arsenal/logs");
            while (true) {
                File entry = dir.openNextFile();
                if (!entry) break;
                if (!entry.isDirectory()) {
                    String path = String(entry.path());
                    entry.close();
                    SD.remove(path);
                } else {
                    entry.close();
                }
            }
            dir.close();
            displayRedStripe("Logs cleared");
            delay(1000);
        }});

        addOptionToMainMenu();
        loopOptions(options, MENU_TYPE_SUBMENU, "Session Log");
    });
}
