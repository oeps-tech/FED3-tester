#include "sound.h"
#include "adc_filter.h"
#include "buttons.h"
#include "app_ios_and_regs.h"
#include "execution.h"
#include "analog_conversion.h"
#include "adc_calibration.h"

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

/************************************************************************/
/* STATE INITS AND FUNCTION -  DON'T CHANGE THEM!!!                     */
/* ????????????????????????????????????????????????????  */
/*                                                                      */
/************************************************************************/
static s_t s = {true, true, false, 0, 0, 0};

//static void test_is_starting (void) { s.pass_or_fail = 0; }

int8_t back_to_s0 (void)
{
	s.first_run = true;
	return s.current_state * -1;
}

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


/************************************************************************/
/* USER FUNTIONS                                                        */
/* ????????????????????????????????????????????????????  */
/*                                                                      */
/************************************************************************/
void hardware_standby_configuration (void)
{
	clr_LED_3V3_AND_VBAT;
	clr_LED_CURRENT;
	clr_LED_LEDS;
	clr_LED_VCC_RGB;
	clr_LED_PASS;
	clr_LED_FAIL;
	
	clr_EN_3V7_TO_VBAT;
	set_EN_DUT_LDO_MODE;
	clr_EN_DUT_3V3_LDO;
	clr_SELECT_VBAT_CURR_HIGH;
	clr_EN_VUSB_TO_DUT;
}

/************************************************************************
/ STATE 1
/ ?? Waits for start button
/ ?? ?? Applies VUSB to DUT
/ ?? ?? Start 200 ms timeout.
/ ? ? If 3V3 above 0.1V -> FAIL
/ ? Go to next state
/ **********************************************************************/
int8_t s0 (void)
{
	if (buttons.start)
	{				
		set_EN_VUSB_TO_DUT;
		s.timeout = 200;
	}
	
	if (s.timeout_elapsed)
	{
		if (get_V_3V3() > 0.1) GO_TO_FAIL;
		
		GO_TO_NEXT_STATE;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 1
/ ?? Enables DUT 3V3 LDO
/ ?? Starts a 1 s timeout
/ ? If 3V3 not ok -> FAIL
/ ? If VBAT not ok -> FAIL
/ ? Save V_CHARGE PASS/FAIL
/ ? Go to next state
/ **********************************************************************/
bool v_charge_led_pass;

int8_t s1 (void)
{
	if (s.new_entry)
	{	
		set_EN_DUT_3V3_LDO;
		s.timeout = 1000;
	}
	
	if (s.timeout_elapsed)
	{		
		float v;
		v = get_V_3V3(); // 3% margin
		if (v > (3.3 * 1.03)) GO_TO_FAIL;
		if (v < (3.3 * 0.97)) GO_TO_FAIL;
		
		v = get_V_Vbat(); // 3% margin
		if (v > (4.2 * 1.03)) GO_TO_FAIL;
		if (v < (4.2 * 0.97)) GO_TO_FAIL;
		
		v = get_V_LedCharge(); // 5% margin
		v_charge_led_pass = true;
		if (v > (2.45 * 1.05)) v_charge_led_pass = false;
		if (v < (2.45 * 0.95)) v_charge_led_pass = false;
		
		
		set_LED_3V3_AND_VBAT;
		produce_sound_new_button();
		GO_TO_NEXT_STATE;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 2
/ ?? Start a 60 s timer
/ ? Exit with FAIL
/ If Programming ok? = FAIL -> FAIL
/ If Programming ok? = PASS -> Go to next state
/ 
/ **********************************************************************/
int8_t s2 (void)
{
	if (s.new_entry)
	{
		s.timeout = 60000;
	}
	
	if (s.timeout_elapsed)
	{
		GO_TO_FAIL;
	}
	
	if (buttons.prog_ok)
	{
		GO_TO_NEXT_STATE;
	}
	
	if (buttons.prog_nok)
	{
		GO_TO_FAIL;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 3
/ ?? Removes VUSB from DUT
/ ?? Disables DUT 3V3 LDO
/ ?? Disable LDO mode
/ ?? Start 1000 ms timeout to allow time for discharging capacitors
/ ? Go to next state
/ **********************************************************************/
int8_t s3 (void)
{
	if (s.new_entry)
	{
		clr_EN_VUSB_TO_DUT;
		clr_EN_DUT_3V3_LDO;
		clr_EN_DUT_LDO_MODE;
		s.timeout = 1000;
	}
	
	if (s.timeout_elapsed)
	{
		GO_TO_NEXT_STATE;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 4
/ ?? Applies 3.7V to VBAT
/ ?? Starts a 200 ms timeout
/ ? Go to next state
/ **********************************************************************/
int8_t s4 (void)
{
	if (s.new_entry)
	{
		set_EN_3V7_TO_VBAT;
		
		s.timeout = 200;
	}
	
	if (s.timeout_elapsed)
	{
		GO_TO_NEXT_STATE;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 5
/ ? If current is not around 180uA -> FAIL
/ ?? Selects higher current sense
/ ?? Enables DUT 3V3 LDO
/ ?? Start 4s timeout to allow time for current stabilization
/ ? Go to next state
/ **********************************************************************/
int8_t s5 (void)
{
	if (s.new_entry)
	{	
		float i;
		i = get_I_Vbat(); // 20% margin
		if (i > (0.000180 * 1.2)) GO_TO_FAIL;
		if (i < (0.000180 * 0.8)) GO_TO_FAIL;
		
		set_SELECT_VBAT_CURR_HIGH;
		set_EN_DUT_3V3_LDO;
		
		s.timeout = 4000;
	}
	
	if (s.timeout_elapsed)
	{
		GO_TO_NEXT_STATE;
	}
		
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 6
/ ? If current is not around 22.1 mA, FAIL
/ ? Check if VCC RGB voltage is zero at this point
/ ?? Go to next state
/ **********************************************************************/
bool vcc_rgb_0_v_pass;

int8_t s6 (void)
{
	if (s.new_entry)
	{
		float i;
		i = get_I_Vbat(); // 3% margin
		if (i > (0.0221 * 1.03)) GO_TO_FAIL;
		if (i < (0.0221 * 0.97)) GO_TO_FAIL;
		
		set_LED_CURRENT;
		
		vcc_rgb_0_v_pass = false;
		
		i = get_V_VccRgb();
		if (i < 0.1) vcc_rgb_0_v_pass = true;
	}
	
	GO_TO_NEXT_STATE;
}

/************************************************************************
/ STATE 7
/ ?? If previous result of charge LED is bad, FAIL
/ ?? Start 6 s timeout to allow VCC RGB to go up to around 3.7 V
/                                                    ____      ____
/ Checks if the LEDs have the next pattern   Red   _|    |____|    |___
/                                                  _      ____      ___
/                                            Green  |____|    |____|
/ ? Go to next state if PASS
/ **********************************************************************/
bool green_LED_pass;
bool red_LED_pass;

int8_t s7 (void)
{
	if (s.new_entry)
	{
		if (v_charge_led_pass == false) GO_TO_FAIL;
		
		green_LED_pass = false;
		red_LED_pass = false;
		
		s.timeout = 6000;
	}
	
	float r, g;
	
	r = get_V_LedRed();
	g = get_V_LedGreen();
	
	if ((r < (1.559 * 1.05)) && (r > (1.559 * 0.95))) // Red is on with 5% margin
	{
		if (g < 0.1) // Green is off
		{
			red_LED_pass = true;
		}
	}
	
	if ((g < (1.396 * 1.05)) && (g > (1.396 * 0.95))) // Green is on with 5% margin
	{
		if (r < 0.1) // Red is off
		{
			green_LED_pass = true;
		}
	}
	
	if (red_LED_pass && green_LED_pass)
	{
		set_LED_LEDS;
	}
	
	if (s.timeout_elapsed)
	{
		if (red_LED_pass && green_LED_pass)
		{		
			GO_TO_NEXT_STATE;
		}
		else
		{
			GO_TO_FAIL;
		}
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 8
/ ?? If previous vcc_rgb_0_v_pass is bad, FAIL
/ ? If VCC RGB is not around 3.703 V, FAIL
/ ?? Go to next state
/ **********************************************************************/
int8_t s8 (void)
{
	if (s.new_entry)
	{
		if (vcc_rgb_0_v_pass == false) GO_TO_FAIL;
		
		float v;
		v = get_V_VccRgb(); // 3% margin
		if (v > (3.703 * 1.03)) GO_TO_FAIL;
		if (v < (3.703 * 0.97)) GO_TO_FAIL;
		
		set_LED_VCC_RGB;		
		
		s.timeout = 10;
	}
	
	produce_sound_new_button();
	GO_TO_NEXT_STATE;
}

/************************************************************************
/ STATE 9
/ ?? Start a 20 s timer
/ ? Exit with FAIL
/ If RGBs ok? = FAIL -> FAIL
/ If RGBs ok? = PASS -> Go to next state
/
/ **********************************************************************/
int8_t s9 (void)
{
	if (s.new_entry)
	{
		s.timeout = 20000;
	}
	
	if (s.timeout_elapsed)
	{
		GO_TO_FAIL;
	}
	
	if (buttons.rgb_ok)
	{
		GO_TO_NEXT_STATE;
	}
	
	if (buttons.rgb_nok)
	{
		GO_TO_FAIL;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 10
/ ?? Starts a random ms timeout between 1 and 2 seconds
/ ? Play new button
/ ? Go to next state
/ **********************************************************************/
int8_t s10 (void)
{
	if (s.new_entry)
	{
		s.timeout = 1000 + ((int16_t)(ADCA_CH0_RES & 0x0003)) * (1000/3);
	}
	
	if (s.timeout_elapsed)
	{
		produce_sound_new_button();
		GO_TO_NEXT_STATE;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 11
/ ?? Start a 10 s timer
/ ? Exit with FAIL
/ If MOTOR ok? = FAIL -> FAIL
/ If MOTOR ok? = PASS -> Go to next state
/
/ **********************************************************************/
int8_t s11 (void)
{
	if (s.new_entry)
	{
		s.timeout = 10000;
	}
	
	if (s.timeout_elapsed)
	{
		GO_TO_FAIL;
	}
	
	if (buttons.motor_ok)
	{
		GO_TO_NEXT_STATE;
	}
	
	if (buttons.motor_nok)
	{
		GO_TO_FAIL;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 12
/ ?? Starts a random ms timeout between 1 and 2 seconds
/ ? Play new button
/ ? Go to next state
/ **********************************************************************/
bool v_charge_led_pass;

int8_t s12 (void)
{
	if (s.new_entry)
	{
		s.timeout = 1000 + ((int16_t)(ADCA_CH0_RES & 0x0003)) * (1000/3);
	}
	
	if (s.timeout_elapsed)
	{
		produce_sound_new_button();
		GO_TO_NEXT_STATE;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************
/ STATE 13
/ ?? Start a 10 s timer
/ ? Exit with FAIL
/ If BUZZER ok? = FAIL -> FAIL
/ If BUZZR ok? = PASS -> PASS
/
/ **********************************************************************/
int8_t s13 (void)
{
	if (s.new_entry)
	{
		s.timeout = 10000;
	}
	
	if (s.timeout_elapsed)
	{
		GO_TO_FAIL;
	}
	
	if (buttons.buzzer_ok)
	{
		GO_TO_PASS;
	}
	
	if (buttons.buzzer_nok)
	{
		GO_TO_FAIL;
	}
	
	MAINTAIN_STATE;
}

/************************************************************************/
/* STATE FUNTIONS -  DON'T CHANGE THEM!!!                               */
/* ????????????????????????????????????????????????????  */
/*                                                                      */
/************************************************************************/
/* State 125 */
int8_t fail (void)
{
	if (s.new_entry)
	{
		s.pass_or_fail = -1;
		s.timeout = 4000;
		
		produce_sound_fail();
		
		set_LED_FAIL;
	}
	
	if (s.timeout_elapsed)
	{
		clr_LED_FAIL;
	
		return back_to_s0();
	}

	return 0;
}

/* State 126 */
int8_t pass (void)
{
	if (s.new_entry)
	{	
		s.pass_or_fail = 1;
		s.timeout = 2000;		
		
		produce_sound_pass();
		
		set_LED_PASS;
	}
	
	if (s.timeout_elapsed)
	{
		clr_LED_PASS;
		
		return back_to_s0();
	}
	
	return 0;
}

uint8_t state_counter = 0;
int8_t state_output;
uint8_t inter_state_interval = 0;

void state_machine (void)
{
	//int8_t state_output;
	
	if (s.first_run)
	{
		s.first_run = false;
		hardware_standby_configuration();
	}
	
	if (s.timeout)
	{
		if(--s.timeout == 0)
		{
			s.timeout_elapsed = true;
		}
	}
	
	if (inter_state_interval)
	{
		inter_state_interval--;
	}
	else
	{	
		switch (s.current_state)
		{
			case 0: s.pass_or_fail = 0;
					state_output = s0(); break;
			case 1: state_output = s1(); break;
			case 2: state_output = s2(); break;
			case 3: state_output = s3(); break;
			case 4: state_output = s4(); break;
			case 5: state_output = s5(); break;
			case 6: state_output = s6(); break;
			case 7: state_output = s7(); break;
			case 8: state_output = s8(); break;
			case 9: state_output = s9(); break;
			case 10: state_output = s10(); break;
			case 11: state_output = s11(); break;
			case 12: state_output = s12(); break;
			case 13: state_output = s13(); break;
			case 125: state_output = fail(); break;
			case 126: state_output = pass(); break;
		}
	
		if (state_output != 0)
		{
			inter_state_interval = 10;
		
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
	
	
}