#include "sound.h"
#include "adc_filter.h"
#include "buttons.h"
#include "app_ios_and_regs.h"
#include "execution.h"

extern buttons_t buttons;

/************************************************************************
/ Functions to be used:
/
/   - adc_get_uV(index)
/   - adc_get_mV(index)
/   - produce_sound_new_button()
/   - produce_sound_pass()
/   - produce_sound_fail()
/
/ Variables to be used:
/
/   - buttons
/ ***********************************************************************/

static s_t s = {true, false, 0, 0, 0};

static void test_is_starting (void) { s.pass_or_fail = 0; }

static int8_t back_to_s0 (void) { return s.current_state * -1; }
static int8_t go_to_fail (void) { return 125 - s.current_state; };
static int8_t go_to_pass (void) { return 126 - s.current_state; };

int8_t get_current_execution_state (void)
{
	return s.current_state;
}

int8_t get_pass_or_fail (void)
{
	return s.pass_or_fail;
}

int8_t s_template (void)
{	
	if (s.new_entry)
	{
		/* Define timeout if necessary */
		s.timeout = 1000;
		
		/* Initialize hardware */
		// ...
	}
	
	if (s.timeout_elapsed)
	{
		/* Do what is supposed when timeout elapsed */
		return back_to_s0();
	}
	
	return 0;
}


/************************************************************************
/ STATE 0
/ Waits for the START button to exit
/ ************************************************************************/
int8_t s0 (void)
{	
	if (buttons.start)
	{
		test_is_starting();
		
		clr_LED_3V3_AND_VBAT;
		clr_LED_CURRENT;
		clr_LED_LEDS;
		clr_LED_VCC_RGB;
		clr_LED_PASS;
		clr_LED_FAIL;		
		
		return 1;
	}
	
	return 0;
}

/************************************************************************
/ STATE 1
/ Waits for the Programing OK button
/ Exist if a timeout occurs
/ ************************************************************************/
int8_t s1 (void)
{
	if (s.new_entry)
	{
		s.timeout = 2000;
		clr_EN_VUSB_TO_DUT;
	}
	
	if (s.timeout_elapsed)
	{
		produce_sound_new_button();
		set_LED_3V3_AND_VBAT;
		return 1;
	}
		
	return 0;
}

/************************************************************************
/ STATE 2
/ Waits for the Programing OK button
/ Exist if a timeout occurs
/ ************************************************************************/
int8_t s2 (void)
{
	if (s.new_entry)
	{
		s.timeout = 4000;
	}
	
	if (s.timeout_elapsed)
	{
		return go_to_fail();
	}
	
	if (buttons.prog_ok)
	{
		set_EN_VUSB_TO_DUT;
		return go_to_pass();
	}
	
	if (buttons.prog_nok)
	{
		set_EN_VUSB_TO_DUT;
		return go_to_fail();
	}
	
	return 0;
}

/* State 125 */
int8_t fail (void)
{
	s.pass_or_fail = -1;
	
	if (s.new_entry)
	{
		/* Initialize hardware */
		// ...
	}
	
	set_LED_FAIL;
	produce_sound_fail();
	return back_to_s0();
}

/* State 126 */
int8_t pass (void)
{
	s.pass_or_fail = 1;
	
	if (s.new_entry)
	{		
		/* Initialize hardware */
		// ...
	}
	
	set_LED_PASS;
	produce_sound_pass();	
	return back_to_s0();
}

uint8_t state_counter = 0;
	int8_t state_output;

void state_machine (void)
{
	//int8_t state_output;
	
	if (s.timeout)
	{
		if(--s.timeout == 0)
		{
			s.timeout_elapsed = true;
		}
	}
	switch (s.current_state)
	{
		case 0: state_output = s0(); break;
		case 1: state_output = s1(); break;
		case 2: state_output = s2(); break;
		//case 3: state_output = s3(); break;
		//case 4: state_output = s4(); break;
		//case 5: state_output = s5(); break;
		//case 6: state_output = s6(); break;
		//case 7: state_output = s7(); break;
		//case 8: state_output = s8(); break;
		//case 9: state_output = s9(); break;
		//case 10: state_output = s10(); break;
		case 125: state_output = fail(); break;
		case 126: state_output = pass(); break;
	}
	
	if (state_output != 0)
	{
		s.new_entry = true;
		s.timeout_elapsed = false;
		s.timeout = 0;
		
		s.current_state += state_output;
	}
	else
	{
		s.new_entry = false;
	}
}