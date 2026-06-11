#ifndef _SOUND_H
#define _SOUND_H
#include "cpu.h"


typedef struct
{
	uint16_t interval_on;
	uint16_t interval_off;
	uint8_t repetitions;
	
	uint8_t prescaler;
	uint16_t target_count;
	uint16_t duty_cycle_count;
	
} sound_t;


void produce_sound_new_button (void);
void produce_sound_pass (void);
void produce_sound_fail (void);


#endif /* _SOUND_H */