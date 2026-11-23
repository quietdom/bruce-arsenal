#ifndef __ARSENAL_BACKGROUND_H__
#define __ARSENAL_BACKGROUND_H__

#include <Arduino.h>


enum ArsenalOpsecLevel {
    OPSEC_CLEAR = 0,
    OPSEC_CAUTION = 1,
    OPSEC_DANGER = 2
};


extern volatile ArsenalOpsecLevel g_opsecLevel;
extern volatile bool g_arsenalEvasionActive;
extern volatile bool g_arsenalLowPowerMode;
extern volatile int g_arsenalDeauthsDetected;
extern volatile int g_arsenalCredsCapture;


void arsenal_background_start(void);
void arsenal_background_stop(void);
bool arsenal_background_is_running(void);


void arsenal_draw_opsec_dot(void);


void arsenal_evasion_toggle(void);
bool arsenal_evasion_is_active(void);


void arsenal_combo_menu(void);

#endif
