#ifndef WISDOM_HAL_EXTENSION_H
#define WISDOM_HAL_EXTENSION_H

#include <stdbool.h>
#include <assert.h>

#include "whale_radio.h"
#include "whale_rtc.h"

// Return value of all unimplemented functions
#define FUNC_UNIMPLEMENTED (-1)

#define MODULE_UNINITIALIZED (0x0u) // 0b00
#define MODULE_OK            (0x1u) // 0b01
#define MODULE_ERROR         (0x2u) // 0b10
#define MODULE_UNIMPLEMENTED (0x3u) // 0b11
#define MODULE_STATE_MASK (MODULE_UNIMPLEMENTED)

#define STATE_FIELD_WIDTH    (2u)

#define W_RADIO_MODULE    (0x1)
#define W_CELLULAR_MODULE (0x2)
#define W_RTC_MODULE      (0x4)

enum whale_error {
	WHALE_OK,
	WHALE_ERROR
};

uint16_t WHALE_MODULE_STATE = {0};

// TODO: Tie logging system into this function when it is implemented
static inline int whale_init(int modules) {
	int whale_state = WHALE_OK;
	int init_rval = {0};

	// RADIO
	if (modules & W_RADIO_MODULE) {
		init_rval = w_radio_init();
		if (init_rval == W_RADIO_OK) {
			WHALE_MODULE_STATE |= 
				(MODULE_OK << ((W_RADIO_MODULE/2)*STATE_FIELD_WIDTH));
		} else if (init_rval != FUNC_UNIMPLEMENTED) {
			whale_state = WHALE_ERROR;
			WHALE_MODULE_STATE |= 
				(MODULE_ERROR << ((W_RADIO_MODULE/2)*STATE_FIELD_WIDTH));
			// Handle error here (maybe log so more details can be retrieved?)
		} else {
			WHALE_MODULE_STATE |= 
				(MODULE_UNIMPLEMENTED << ((W_RADIO_MODULE/2)*STATE_FIELD_WIDTH));
		}
	}

	// CELLULAR
	//init_rval = w_cellular_init();
	//if (init_rval == W_CELLULAR_OK) {
	//	WHALE_MODULE_STATE |= W_CELLULAR_MODULE;	
	//} else {
	//	rval = WHALE_ERROR;
	//	// Log error here
	//}

	//// RTC
	//init_rval = w_rtc_init();
	//if (init_rval == W_RTC_OK) {
	//	WHALE_MODULE_STATE |= W_RTC_MODULE;	
	//} else {
	//	rval = WHALE_ERROR;
	//	// Log error here
	//}
	
	return whale_state;
}

static inline int w_module_state_query(int w_module) {
	int state = WHALE_MODULE_STATE >> ((w_module/2) * STATE_FIELD_WIDTH);
	return state & MODULE_STATE_MASK;
}

#endif // WISDOM_HAL_EXTENSION_H
