#include "arsenal.h"

#if !LITE_VERSION

#include "core/display.h"
#include "core/mykeyboard.h"
#include <globals.h>
#include "modules/NRF24/nrf_mousejack.h"

void arsenal_nrf24_mousejack(void) {
    ARSENAL_HEAP_CHECK();
    // Hand off to the real MouseJack implementation in the NRF24 module
    // instead of a wiring-instructions screen. The pinout lives in the
    // NRF24 Config pins menu.
    nrf_mousejack();
}

#endif
