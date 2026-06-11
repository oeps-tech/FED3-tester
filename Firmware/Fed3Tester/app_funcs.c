#include "app_funcs.h"
#include "app_ios_and_regs.h"
#include "hwbp_core.h"

#include "buttons.h"
#include "sound.h"


/************************************************************************/
/* Create pointers to functions                                         */
/************************************************************************/
extern AppRegs app_regs;

void (*app_func_rd_pointer[])(void) = {
	&app_read_REG_SWITCHES,
	&app_read_REG_READINGS,
	&app_read_REG_CONTROL,
	&app_read_REG_CONTROL_SET,
	&app_read_REG_CONTROL_CLEAR
};

bool (*app_func_wr_pointer[])(void*) = {
	&app_write_REG_SWITCHES,
	&app_write_REG_READINGS,
	&app_write_REG_CONTROL,
	&app_write_REG_CONTROL_SET,
	&app_write_REG_CONTROL_CLEAR
};


/************************************************************************/
/* REG_SWITCHES                                                         */
/************************************************************************/
extern buttons_t buttons_from_registers;
extern buttons_timeout_t buttons_from_registers_timeout;

void app_read_REG_SWITCHES(void) {}
bool app_write_REG_SWITCHES(void *a)
{
	uint16_t reg = *((uint16_t*)a);
	
	if (reg & B_START) { buttons_from_registers.start = true; buttons_from_registers_timeout.start = BUTTONS_TIMEOUT; }
	
	if (reg & B_PROG_OK) { buttons_from_registers.prog_ok = true; buttons_from_registers_timeout.prog_ok = BUTTONS_TIMEOUT; }
	if (reg & B_PROG_NOK) { buttons_from_registers.prog_nok = true; buttons_from_registers_timeout.prog_nok = BUTTONS_TIMEOUT; }
		
	if (reg & B_RGB_OK) { buttons_from_registers.rgb_ok = true; buttons_from_registers_timeout.rgb_ok = BUTTONS_TIMEOUT; }
	if (reg & B_RGB_NOK) { buttons_from_registers.rgb_nok = true; buttons_from_registers_timeout.rgb_nok = BUTTONS_TIMEOUT; }

	if (reg & B_MOTOR_OK) { buttons_from_registers.motor_ok = true; buttons_from_registers_timeout.motor_ok = BUTTONS_TIMEOUT; }
	if (reg & B_MOTOR_NOK) { buttons_from_registers.motor_nok = true; buttons_from_registers_timeout.motor_nok = BUTTONS_TIMEOUT; }
	
	if (reg & B_BUZZER_OK) { buttons_from_registers.buzzer_ok = true; buttons_from_registers_timeout.buzzer_ok = BUTTONS_TIMEOUT; }
	if (reg & B_BUZZER_NOK) { buttons_from_registers.buzzer_nok = true; buttons_from_registers_timeout.buzzer_nok = BUTTONS_TIMEOUT; }
		
	app_regs.REG_SWITCHES = reg;
	return true;
}


/************************************************************************/
/* REG_READINGS                                                         */
/************************************************************************/
// This register is an array with 13 positions
void app_read_REG_READINGS(void) {}
bool app_write_REG_READINGS(void *a)
{
	float *reg = ((float*)a);

	app_regs.REG_READINGS[0] = reg[0];
	return true;
}


/************************************************************************/
/* REG_CONTROL                                                          */
/************************************************************************/
void app_read_REG_CONTROL(void) {}
bool app_write_REG_CONTROL(void *a)
{
	uint16_t reg = *((uint16_t*)a);
	
	if (reg & B_EN_VUSB_TO_DUT) {set_EN_VUSB_TO_DUT;} else {clr_EN_VUSB_TO_DUT;}
	if (reg & B_EN_3V7_TO_VBAT) {set_EN_3V7_TO_VBAT;} else {clr_EN_3V7_TO_VBAT;}
	if (reg & B_EN_DUT_LDO_MODE) {set_EN_DUT_LDO_MODE;} else {clr_EN_DUT_LDO_MODE;}
	if (reg & B_SELECT_VBAT_CURR_HIGH) {set_SELECT_VBAT_CURR_HIGH;} else {clr_SELECT_VBAT_CURR_HIGH;}
		
	if (reg & B_LED_3V3_AND_VBAT) {set_LED_3V3_AND_VBAT;} else {clr_LED_3V3_AND_VBAT;}
	if (reg & B_LED_CURRENT) {set_LED_CURRENT;} else {clr_LED_CURRENT;}
	if (reg & B_LED_LEDS) {set_LED_LEDS;} else {clr_LED_LEDS;}
	if (reg & B_LED_VCC_RGB) {set_LED_VCC_RGB;} else {clr_LED_VCC_RGB;}
	if (reg & B_LED_PASS) {set_LED_PASS;} else {clr_LED_PASS;}
	if (reg & B_LED_FAIL) {set_LED_FAIL;} else {clr_LED_FAIL;}
		
	if (reg & B_SOUND_WAIT_FOR_BUTTON) {produce_sound_new_button();}
	if (reg & B_SOUND_PASS) {produce_sound_pass();}
	if (reg & B_SOUND_FAIL) {produce_sound_fail();}

	app_regs.REG_CONTROL = reg;
	return true;
}


/************************************************************************/
/* REG_CONTROL_SET                                                      */
/************************************************************************/
void app_read_REG_CONTROL_SET(void)
{
	//app_regs.REG_CONTROL_SET = 0;

}

bool app_write_REG_CONTROL_SET(void *a)
{
	uint16_t reg = *((uint16_t*)a);
	
	if (reg & B_EN_VUSB_TO_DUT) {set_EN_VUSB_TO_DUT;}
	if (reg & B_EN_3V7_TO_VBAT) {set_EN_3V7_TO_VBAT;}
	if (reg & B_EN_DUT_LDO_MODE) {set_EN_DUT_LDO_MODE;}
	if (reg & B_SELECT_VBAT_CURR_HIGH) {set_SELECT_VBAT_CURR_HIGH;}
		
	if (reg & B_LED_3V3_AND_VBAT) {set_LED_3V3_AND_VBAT;}
	if (reg & B_LED_CURRENT) {set_LED_CURRENT;}
	if (reg & B_LED_LEDS) {set_LED_LEDS;}
	if (reg & B_LED_VCC_RGB) {set_LED_VCC_RGB;}
	if (reg & B_LED_PASS) {set_LED_PASS;}
	if (reg & B_LED_FAIL) {set_LED_FAIL;}
		
	if (reg & B_SOUND_WAIT_FOR_BUTTON) {produce_sound_new_button();}
	if (reg & B_SOUND_PASS) {produce_sound_pass();}
	if (reg & B_SOUND_FAIL) {produce_sound_fail();}

	app_regs.REG_CONTROL_SET = reg;
	return true;
}


/************************************************************************/
/* REG_CONTROL_CLEAR                                                    */
/************************************************************************/
void app_read_REG_CONTROL_CLEAR(void)
{
	//app_regs.REG_CONTROL_CLEAR = 0;

}

bool app_write_REG_CONTROL_CLEAR(void *a)
{
	uint16_t reg = *((uint16_t*)a);
	
	if (reg & B_EN_VUSB_TO_DUT) {clr_EN_VUSB_TO_DUT;}
	if (reg & B_EN_3V7_TO_VBAT) {clr_EN_3V7_TO_VBAT;}
	if (reg & B_EN_DUT_LDO_MODE) {clr_EN_DUT_LDO_MODE;}
	if (reg & B_SELECT_VBAT_CURR_HIGH) {clr_SELECT_VBAT_CURR_HIGH;}
	
	if (reg & B_LED_3V3_AND_VBAT) {clr_LED_3V3_AND_VBAT;}
	if (reg & B_LED_CURRENT) {clr_LED_CURRENT;}
	if (reg & B_LED_LEDS) {clr_LED_LEDS;}
	if (reg & B_LED_VCC_RGB) {clr_LED_VCC_RGB;}
	if (reg & B_LED_PASS) {clr_LED_PASS;}
	if (reg & B_LED_FAIL) {clr_LED_FAIL;}

	app_regs.REG_CONTROL_CLEAR = reg;
	return true;
}