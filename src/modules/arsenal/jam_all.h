#ifndef __ARSENAL_JAM_ALL_H__
#define __ARSENAL_JAM_ALL_H__

#include <Arduino.h>


enum JamBand {
    JAM_WIFI_24 = 0,
    JAM_BLE,
    JAM_SUBGHZ,
    JAM_NRF24,
    JAM_BAND_COUNT
};

struct JamBandState {
    const char *name;
    bool enabled;
    bool available;
    bool active;
    uint8_t level;
};

struct JamAllState {
    JamBandState bands[JAM_BAND_COUNT];
    unsigned long startTime;
    unsigned long timeout;
    int selectedBand;
    bool running;
};

void jamall_init(JamAllState &state);
void jamall_draw_gui(JamAllState &state);
void jamall_start_band(JamAllState &state, JamBand band);
void jamall_stop_band(JamAllState &state, JamBand band);
void jamall_start_all(JamAllState &state);
void jamall_stop_all(JamAllState &state);
void jamall_update(JamAllState &state);

#endif
