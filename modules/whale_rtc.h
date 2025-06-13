#ifndef WHALE_RTC_H
#define WHALE_RTC_H

#include <stdint.h>
#include <stdbool.h>

#define RTC_24H_MODE 1
#define RTC_12H_MODE 0

struct w_rtc_datetime_t {
	uint8_t years;
	uint8_t months;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
};



int w_rtc_init(void);
int w_rtc_hw_rst(void);

int w_rtc_hour_format_set(bool timeformat);
int w_rtc_hour_format_get(bool *timeformat);

int w_rtc_datetime_set(struct w_rtc_datetime_t *datetime);
int w_rtc_datetime_get(struct w_rtc_datetime_t *datetime);

#endif
