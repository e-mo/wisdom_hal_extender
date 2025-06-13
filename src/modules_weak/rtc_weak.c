// real-time clock functions to be defined in the rtc module (mostly pcf8523 now)

#include "whale_rtc.h"

#define _WEAK_ __attribute__((weak))

#define UNIMPLEMENTED (-1)


_WEAK_ int w_rtc_init(void) { return UNIMPLEMENTED; } //init i2c, default settings (cap, ampm, bat switchover, etc)
//_WEAK_ int w_rtc_hw_rst(void) { return UNIMPLEMENTED; } //hardware FULL reset - see datasheet for initial condx

//_WEAK_ int w_rtc_hour_format_set(bool timeformat) { return UNIMPLEMENTED; } //24hr or 12hr am/pm (ew)
//_WEAK_ int w_rtc_hour_format_get(bool *timeformat) { return UNIMPLEMENTED; }



