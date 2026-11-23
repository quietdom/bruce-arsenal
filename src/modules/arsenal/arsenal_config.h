#ifndef __ARSENAL_CONFIG_H__
#define __ARSENAL_CONFIG_H__

#include <Arduino.h>


struct ArsenalConfig {
    char apSsid[33];
    char apPass[64];
    char dashUser[33];
    char dashPass[64];
    char pin[8];
    bool pinEnabled;
};

ArsenalConfig &arsenal_config(void);
void arsenal_config_load(void);
void arsenal_config_save(void);
void arsenal_config_reset(void);

bool arsenal_pin_check(void);
void arsenal_pin_set(const char *newPin);
void arsenal_pin_clear(void);


struct ArsenalStats {
    uint32_t credsCaptured;
    uint32_t devicesSeen;
    uint32_t attacksRun;
    uint32_t probesLogged;
    uint32_t portalsServed;
    uint32_t wpaHandshakes;
    uint32_t deauthsDetected;
    uint32_t bleDevicesSeen;
    uint32_t passwordsGenerated;
    uint32_t combosExecuted;
    uint32_t uptimeStart;
    char lastSession[32];
};

ArsenalStats &arsenal_stats(void);
void arsenal_stats_load(void);
void arsenal_stats_save(void);
void arsenal_stats_reset(void);

inline void arsenal_stats_inc_creds()      { arsenal_stats().credsCaptured++;   }
inline void arsenal_stats_inc_devices()    { arsenal_stats().devicesSeen++;      }
inline void arsenal_stats_inc_attacks()    { arsenal_stats().attacksRun++;       }
inline void arsenal_stats_inc_probes()     { arsenal_stats().probesLogged++;     }
inline void arsenal_stats_inc_portals()    { arsenal_stats().portalsServed++;    }
inline void arsenal_stats_inc_handshakes() { arsenal_stats().wpaHandshakes++;    }
inline void arsenal_stats_inc_ble()        { arsenal_stats().bleDevicesSeen++;   }
inline void arsenal_stats_inc_passwords()  { arsenal_stats().passwordsGenerated++;}
inline void arsenal_stats_inc_combos()     { arsenal_stats().combosExecuted++;   }

#endif
