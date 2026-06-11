#ifndef _BUTTONS_H
#define _BUTTONS_H
#include <avr/io.h>

#include "cpu.h"
#include "app_ios_and_regs.h"

#define BUTTONS_TIMEOUT 400/5 // 400 ms

typedef struct 
{
	bool start;
	bool prog_ok;
	bool prog_nok;
	bool rgb_ok;
	bool rgb_nok;
	bool motor_ok;
	bool motor_nok;
	bool buzzer_ok;
	bool buzzer_nok;
} buttons_t;

typedef struct
{
	uint8_t start;
	uint8_t prog_ok;
	uint8_t prog_nok;
	uint8_t rgb_ok;
	uint8_t rgb_nok;
	uint8_t motor_ok;
	uint8_t motor_nok;
	uint8_t buzzer_ok;
	uint8_t buzzer_nok;
} buttons_timeout_t;

void init_buttons_timer (void);

#endif /* _BUTTONS_H */