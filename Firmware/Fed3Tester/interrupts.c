#include "cpu.h"
#include "hwbp_core_types.h"
#include "app_ios_and_regs.h"
#include "app_funcs.h"
#include "hwbp_core.h"

/************************************************************************/
/* Declare application registers                                        */
/************************************************************************/
extern AppRegs app_regs;

/************************************************************************/
/* Interrupts from Timers                                               */
/************************************************************************/
// ISR(TCC0_OVF_vect, ISR_NAKED)
// ISR(TCD0_OVF_vect, ISR_NAKED)
// ISR(TCE0_OVF_vect, ISR_NAKED)
// ISR(TCF0_OVF_vect, ISR_NAKED)
// 
// ISR(TCC0_CCA_vect, ISR_NAKED)
// ISR(TCD0_CCA_vect, ISR_NAKED)
// ISR(TCE0_CCA_vect, ISR_NAKED)
// ISR(TCF0_CCA_vect, ISR_NAKED)
// 
// ISR(TCD1_OVF_vect, ISR_NAKED)
// 
// ISR(TCD1_CCA_vect, ISR_NAKED)

/************************************************************************/
/* ADC                                                                  */
/************************************************************************/
uint8_t adc_index;
uint8_t adc_filter_index = 0;

#define ADC_QUANTITY 7
#define ADC_FILTER_N 16
uint16_t adc[ADC_QUANTITY][ADC_FILTER_N];

#if ADC_FILTER_N < 17
	uint16_t adc_sum[ADC_QUANTITY];
#else
	uint32_t adc_sum[ADC_QUANTITY];
#endif


ISR(ADCA_CH0_vect, ISR_NAKED)
{
	adc[adc_index][adc_filter_index] = ((int16_t)(ADCA_CH0_RES & 0x0FFF));
	
	adc_index++;
	
	if (adc_index != ADC_QUANTITY)
	{
		ADCA_CH0_MUXCTRL = (adc_index+1) << 3;
		ADCA_CH0_CTRL |= ADC_CH_START_bm;
	}
	else
	{		
		if (++adc_filter_index == ADC_FILTER_N)
		{
			adc_filter_index = 0;
		}
		
		for (uint8_t i = 0; i < ADC_QUANTITY; i++)
		{
			adc_sum[i] = 0;
		}
		
		for (uint8_t i = 0; i < ADC_QUANTITY; i++)
		{
			for (uint8_t j = 0; j < ADC_FILTER_N; j++)
			{
				adc_sum[i] += adc[i][j];
			}
		}
		
		/* Transform to float */
		
		for (uint8_t i = 0; i < ADC_QUANTITY; i++)
		{
			app_regs.REG_READINGS[i] = (adc_sum[i] / ADC_FILTER_N) * (3.3/1.6/4096.0);
		}
		
		app_regs.REG_READINGS[8] = 0;
		app_regs.REG_READINGS[9] = 0;
		app_regs.REG_READINGS[10] = 0;
		app_regs.REG_READINGS[11] = 0;
		app_regs.REG_READINGS[12] = 0;
		
		if (!read_SW_START)
			app_regs.REG_READINGS[8] = 1;
		
		if (!read_SW_PROG_OK)
			app_regs.REG_READINGS[9] = 1;
		if (!read_SW_PROG_NOK)
			app_regs.REG_READINGS[9] = -1;
			
		if (!read_SW_RGB_OK)
			app_regs.REG_READINGS[10] = 1;
		if (!read_SW_RGB_NOK)
			app_regs.REG_READINGS[10] = -1;
			
		if (!read_SW_MOTOR_OK)
			app_regs.REG_READINGS[11] = 1;
		if (!read_SW_MOTOR_NOK)
			app_regs.REG_READINGS[11] = -1;
			
		if (!read_SW_BUZZER_OK)
			app_regs.REG_READINGS[12] = 1;
		if (!read_SW_BUZZER_NOK)
			app_regs.REG_READINGS[12] = -1;
	
		core_func_send_event(ADD_REG_READINGS, true);
	}
	
	reti();
}