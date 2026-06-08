#include "app_funcs.h"
#include "app_ios_and_regs.h"
#include "hwbp_core.h"


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
void app_read_REG_SWITCHES(void)
{
	//app_regs.REG_SWITCHES = 0;

}

bool app_write_REG_SWITCHES(void *a)
{
	uint16_t reg = *((uint16_t*)a);

	app_regs.REG_SWITCHES = reg;
	return true;
}


/************************************************************************/
/* REG_READINGS                                                         */
/************************************************************************/
// This register is an array with 14 positions
void app_read_REG_READINGS(void) {}
bool app_write_REG_READINGS(void *a)
{
	// V_Vbat, V_VccRgb, V_3V3, V_LedCharge, V_LedRed, V_LedGreen, I_VbatLow, I_VbatHigh, SW_Sart, SW_Prog, SW_Rgb, SW_Motor, SW_Buzzer, PassFail

	float *reg = ((float*)a);

	app_regs.REG_READINGS[0] = reg[0];
	return true;
}


/************************************************************************/
/* REG_CONTROL                                                          */
/************************************************************************/
void app_read_REG_CONTROL(void)
{
	//app_regs.REG_CONTROL = 0;

}

bool app_write_REG_CONTROL(void *a)
{
	uint16_t reg = *((uint16_t*)a);
	
	if (reg & B_EN_VUSB_TO_DUT) set_EN_VUSB_TO_DUT; else clr_EN_VUSB_TO_DUT;
	if (reg & B_EN_3V7_TO_VBAT) set_EN_3V7_TO_VBAT; else clr_EN_3V7_TO_VBAT;
	if (reg & B_EN_DUT_LDO_MODE) set_EN_DUT_LDO_MODE; else clr_EN_DUT_LDO_MODE;
	if (reg & B_SELECT_VBAT_CURR_HIGH) set_SELECT_VBAT_CURR_HIGH; else clr_SELECT_VBAT_CURR_HIGH;
		
	if (reg & B_LED_3V3_AND_VBAT) set_LED_3V3_AND_VBAT; else clr_LED_3V3_AND_VBAT;
	if (reg & B_LED_CURRENT) set_LED_CURRENT; else clr_LED_CURRENT;
	if (reg & B_LED_LEDS) set_LED_LEDS; else clr_LED_LEDS;
	if (reg & B_LED_VCC_RGB) set_LED_VCC_RGB; else clr_LED_VCC_RGB;
	if (reg & B_LED_PASS) set_LED_PASS; else clr_LED_PASS;
	if (reg & B_LED_FAIL) set_LED_FAIL; else clr_LED_FAIL;

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
	
	if (reg & B_EN_VUSB_TO_DUT) set_EN_VUSB_TO_DUT;
	if (reg & B_EN_3V7_TO_VBAT) set_EN_3V7_TO_VBAT;
	if (reg & B_EN_DUT_LDO_MODE) set_EN_DUT_LDO_MODE;
	if (reg & B_SELECT_VBAT_CURR_HIGH) set_SELECT_VBAT_CURR_HIGH;
		
	if (reg & B_LED_3V3_AND_VBAT) set_LED_3V3_AND_VBAT;
	if (reg & B_LED_CURRENT) set_LED_CURRENT;
	if (reg & B_LED_LEDS) set_LED_LEDS;
	if (reg & B_LED_VCC_RGB) set_LED_VCC_RGB;
	if (reg & B_LED_PASS) set_LED_PASS;
	if (reg & B_LED_FAIL) set_LED_FAIL;

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
	
	if (reg & B_EN_VUSB_TO_DUT) clr_EN_VUSB_TO_DUT;
	if (reg & B_EN_3V7_TO_VBAT) clr_EN_3V7_TO_VBAT;
	if (reg & B_EN_DUT_LDO_MODE) clr_EN_DUT_LDO_MODE;
	if (reg & B_SELECT_VBAT_CURR_HIGH) clr_SELECT_VBAT_CURR_HIGH;
	
	if (reg & B_LED_3V3_AND_VBAT) clr_LED_3V3_AND_VBAT;
	if (reg & B_LED_CURRENT) clr_LED_CURRENT;
	if (reg & B_LED_LEDS) clr_LED_LEDS;
	if (reg & B_LED_VCC_RGB) clr_LED_VCC_RGB;
	if (reg & B_LED_PASS) clr_LED_PASS;
	if (reg & B_LED_FAIL) clr_LED_FAIL;

	app_regs.REG_CONTROL_CLEAR = reg;
	return true;
}