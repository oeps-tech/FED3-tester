#include "sound.h"
#include "adc_filter.h"
#include "buttons.h"
#include "app_ios_and_regs.h"

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

bool previous_start = false;
bool start_en_led = true;
bool previous_buzzer_ok = false;
bool previous_buzzer_nok = false;

void pcb_test (void)
{
	if (previous_start == false && buttons.start)
	{
		if (start_en_led)
		{
			start_en_led = false;
			produce_sound_new_button();
			set_EN_VUSB_TO_DUT;
			set_LED_3V3_AND_VBAT;
		}
		else
		{
			start_en_led = true;
			clr_EN_VUSB_TO_DUT;
			clr_LED_3V3_AND_VBAT;
		}
	}
	
	if (buttons.prog_ok)  {clr_EN_DUT_LDO_MODE; set_LED_CURRENT;}
	if (buttons.prog_nok) {set_EN_DUT_LDO_MODE; clr_LED_CURRENT;}
	
	if (buttons.rgb_ok)  {set_SELECT_VBAT_CURR_HIGH; set_LED_LEDS;}
	if (buttons.rgb_nok) {clr_SELECT_VBAT_CURR_HIGH; clr_LED_LEDS;}

	if (buttons.motor_ok)  {set_EN_3V7_TO_VBAT; set_LED_VCC_RGB;}
	if (buttons.motor_nok) {clr_EN_3V7_TO_VBAT; clr_LED_VCC_RGB;}
	
	if (previous_buzzer_ok  == false && buttons.buzzer_ok)  {produce_sound_pass(); set_EN_DUT_3V3_LDO; set_LED_PASS;}
	if (previous_buzzer_nok == false && buttons.buzzer_nok) {produce_sound_fail(); clr_EN_DUT_3V3_LDO; set_LED_FAIL;}
	if (!buttons.buzzer_ok)  {clr_LED_PASS;}
	if (!buttons.buzzer_nok) {clr_LED_FAIL;}
	
	previous_start = buttons.start;
	previous_buzzer_ok = buttons.buzzer_ok;
	previous_buzzer_nok = buttons.buzzer_nok;
}