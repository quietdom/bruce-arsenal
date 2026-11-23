#ifndef LITE_VERSION
#include "arsenal.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include <globals.h>

extern void karma_setup();

void arsenal_karma_attack(void) {
    ARSENAL_SAFE_RUN([]() {
        karma_setup();
    });
}
#endif
