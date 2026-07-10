#include "cpu.h"
#include "hwbp_core_types.h"
#include "app_ios_and_regs.h"
#include "app_funcs.h"
#include "hwbp_core.h"

#include "adc_filter.h"
#include "adc_calibration.h"
#include "buttons.h"
#include "send_periodic_event.h"

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
uint8_t analog_index;
uint16_t event_update_counter = 0;

ISR(ADCA_CH0_vect, ISR_NAKED)
{
	adc_add_measurement(analog_index, ((int16_t)(ADCA_CH0_RES & 0x0FFF)));	
	
	analog_index++;
	
	if (analog_index != ADC_QUANTITY)
	{
		ADCA_CH0_MUXCTRL = (analog_index+1) << 3;
		ADCA_CH0_CTRL |= ADC_CH_START_bm;
	}
	else
	{		
		adc_next_index();
		
		/* The measurements are complete */
		/* Send event each 10 ms */
		if ((event_update_counter % 10) == 0)
		{
			send_periodic_event();
		}
	}
	
	reti();
}