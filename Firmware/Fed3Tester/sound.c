#include "buttons.h"
#include "cpu.h"
#include "app_ios_and_regs.h"
#include "sound.h"

static uint16_t sound_timer_counter;
static uint8_t sound_timer_repetitions;
static bool sound_under_repetition;

static sound_t sound;

void play_sound (void)
{
	sound_under_repetition = false;
	sound_timer_counter = sound.interval_on;
	sound_timer_repetitions = 1 + sound.repetitions * 2;
	
	/* Initialize a 1 ms timer to handle the buttons */
	timer_type0_pwm(&TCD0, sound.prescaler, sound.target_count, sound.duty_cycle_count, INT_LEVEL_LOW, INT_LEVEL_LOW);
}

void produce_sound_new_button (void)
{
	sound.interval_on = 500/5;  // 1/5 = 200 ms
	sound.interval_off = 0;
	sound.repetitions = 0;
	
	/*500 Hz */
	sound.prescaler = TIMER_PRESCALER_DIV256;
	sound.target_count = 250;
	sound.duty_cycle_count = 250/2;
	
	play_sound();
}

void produce_sound_pass (void)
{
	sound.interval_on = 500/10;   // 1/10 = 100 ms;
	sound.interval_off = 500/10;  // 1/10 = 100 ms;
	sound.repetitions = 4;
	
	/* 500 Hz */
	sound.prescaler = TIMER_PRESCALER_DIV256;
	sound.target_count = 250;
	sound.duty_cycle_count = 250/2;
	
	play_sound();
}

void produce_sound_fail (void)
{
	sound.interval_on = 78*4;   // 1*4 = 4 s;
	sound.interval_off = 0;
	sound.repetitions = 0;
	
	/* ~78 Hz */
	sound.prescaler = TIMER_PRESCALER_DIV256;
	sound.target_count = 1590;
	sound.duty_cycle_count = 1590/2;
	
	play_sound();
}

ISR(TCD0_OVF_vect, ISR_NAKED)
{
	if (!sound_under_repetition)
	{
		set_BUZZER;
	}
		
	reti();
}

ISR(TCD0_CCA_vect, ISR_NAKED)
{
	clr_BUZZER;
	
	if (--sound_timer_counter == 0)
	{
		if (--sound_timer_repetitions == 0)
		{
			timer_type0_stop(&TCD0);
		}
			
		if (sound_under_repetition)
		{
			sound_timer_counter =  sound.interval_on;
			sound_under_repetition = false;
		}
		else
		{
			sound_timer_counter =  sound.interval_off;
			sound_under_repetition = true;
		}
	}
	
	reti();
}