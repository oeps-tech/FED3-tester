#include "hwbp_core.h"

#include "buttons.h"
#include "analog_conversion.h"
#include "adc_calibration.h"
#include "adc_filter.h"
#include "execution.h"

extern buttons_t buttons;
extern AppRegs app_regs;

void send_periodic_event (void)
{
//  	for (uint8_t i = 0; i < ADC_QUANTITY; i++)
//  	{
//  		app_regs.REG_READINGS[i] = adc_get_uV(i);
//  	}
	
	app_regs.REG_READINGS[0] = get_V_Vbat();
	app_regs.REG_READINGS[1] = get_V_VccRgb();
	app_regs.REG_READINGS[2] = get_V_3V3();
	app_regs.REG_READINGS[3] = get_V_LedCharge();
	app_regs.REG_READINGS[4] = get_V_LedRed();
	app_regs.REG_READINGS[5] = get_V_LedGreen();
	app_regs.REG_READINGS[6] = get_I_Vbat();
		
	app_regs.REG_READINGS[7] = 0;
	app_regs.REG_READINGS[8] = 0;
	app_regs.REG_READINGS[9] = 0;
	app_regs.REG_READINGS[10] = 0;
	app_regs.REG_READINGS[11] = 0;
		
	if (buttons.start)      app_regs.REG_READINGS[7] = 1;
		
	if (buttons.prog_ok)    app_regs.REG_READINGS[8] = 1;
	if (buttons.prog_nok)   app_regs.REG_READINGS[8] = -1;
			
	if (buttons.rgb_ok)     app_regs.REG_READINGS[9] = 1;
	if (buttons.rgb_nok)    app_regs.REG_READINGS[9] = -1;
			
	if (buttons.motor_ok)   app_regs.REG_READINGS[10] = 1;
	if (buttons.motor_nok)  app_regs.REG_READINGS[10] = -1;
			
	if (buttons.buzzer_ok)  app_regs.REG_READINGS[11] = 1;
	if (buttons.buzzer_nok) app_regs.REG_READINGS[11] = -1;
	
	app_regs.REG_READINGS[12] = get_current_execution_state();
	app_regs.REG_READINGS[13] = get_pass_or_fail();

	core_func_send_event(ADD_REG_READINGS, true);
}