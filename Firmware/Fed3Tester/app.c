#include "hwbp_core.h"
#include "hwbp_core_regs.h"
#include "hwbp_core_types.h"

#include "app.h"
#include "app_funcs.h"
#include "app_ios_and_regs.h"

#define F_CPU 32000000
#include <util/delay.h>

/************************************************************************/
/* Declare application registers                                        */
/************************************************************************/
extern AppRegs app_regs;
extern uint8_t app_regs_type[];
extern uint16_t app_regs_n_elements[];
extern uint8_t *app_regs_pointer[];
extern void (*app_func_rd_pointer[])(void);
extern bool (*app_func_wr_pointer[])(void*);

/************************************************************************/
/* Initialize app                                                       */
/************************************************************************/
static const uint8_t default_device_name[] = "Fed3Tester";

void hwbp_app_initialize(void)
{
	/* Start core */
	core_func_start_core_V2(
		WHO_AM_I,
		MAJOR_HW_VERSION, MINOR_HW_VERSION, PATCH_HW_VERSION,
		MAJOR_FW_VERSION, MINOR_FW_VERSION, PATCH_FW_VERSION,
		(uint8_t*)(&app_regs),
		APP_NBYTES_OF_REG_BANK,
		APP_REGS_ADD_MAX - APP_REGS_ADD_MIN + 1,
		default_device_name,
		false,	// The device is _not_ able to repeat the harp timestamp clock
		false,	// The device is _not_ able to generate the harp timestamp clock
		0		// Default timestamp offset
	);
}

/************************************************************************/
/* Handle if a catastrophic error occur                                 */
/************************************************************************/
void core_callback_catastrophic_error_detected(void)
{
	
}

/************************************************************************/
/* User functions                                                       */
/************************************************************************/
/* Add your functions here or load external functions if needed */

/************************************************************************/
/* Initialization Callbacks                                             */
/************************************************************************/
void core_callback_define_clock_default(void) {}
	
void core_callback_initialize_hardware(void)
{
	/* Initialize IOs */
	/* Don't delete this function!!! */
	init_ios();
		
	/* Initialize ADCA with single ended input */
	adc_A_initialize_single_ended(ADC_REFSEL_INTVCC_gc);	// VCC/1.6 = 3.3/1.6 = 2.0625 V
	ADCA_CH0_INTCTRL |= ADC_CH_INTLVL_LO_gc;				// Enable ADC0 interrupt
	_delay_ms(100);
}

void core_callback_reset_registers(void)
{
	/* Initialize registers */
	
}

void core_callback_registers_were_reinitialized(void)
{
	/* Update registers if needed */
	
}

/************************************************************************/
/* Callbacks: Visualization                                             */
/************************************************************************/
void core_callback_visualen_to_on(void)
{
	/* Update visual indicators */
	
}

void core_callback_visualen_to_off(void)
{
	/* Clear all the enabled indicators */
	
}

/************************************************************************/
/* Callbacks: Change on the operation mode                              */
/************************************************************************/
void core_callback_device_to_standby(void) {}
void core_callback_device_to_active(void) {}
void core_callback_device_to_enchanced_active(void) {}
void core_callback_device_to_speed(void) {}

/************************************************************************/
/* Callbacks: 1 ms timer                                                */
/************************************************************************/
extern uint8_t adc_index;

uint16_t counter = 0;

void core_callback_t_before_exec(void) {}
void core_callback_t_after_exec(void) {}
void core_callback_t_new_second(void)
{
	counter = 0;
}
void core_callback_t_500us(void) {}
void core_callback_t_1ms(void)
{
	counter++;
		
	//if (counter % 10 == 0)
	if (counter == 2)
	{
	    /* Start conversation on ADCA Channel 1*/
	    adc_index = 0;
	    ADCA_CH0_MUXCTRL = (adc_index+1) << 3;
	    ADCA_CH0_CTRL |= ADC_CH_START_bm;	
	}
}

/************************************************************************/
/* Callbacks: uart control                                              */
/************************************************************************/
void core_callback_uart_rx_before_exec(void) {}
void core_callback_uart_rx_after_exec(void) {}
void core_callback_uart_tx_before_exec(void) {}
void core_callback_uart_tx_after_exec(void) {}
void core_callback_uart_cts_before_exec(void) {}
void core_callback_uart_cts_after_exec(void) {}
	
/************************************************************************/
/* Callbacks: clock control                                             */
/************************************************************************/
void core_callback_clock_to_repeater(void) {}
void core_callback_clock_to_generator(void) {}
void core_callback_clock_to_unlock(void) {}
void core_callback_clock_to_lock(void) {}

/************************************************************************/
/* Callbacks: Read app register                                         */
/************************************************************************/
bool core_read_app_register(uint8_t add, uint8_t type)
{
	/* Check if it will not access forbidden memory */
	if (add < APP_REGS_ADD_MIN || add > APP_REGS_ADD_MAX)
		return false;
	
	/* Check if type matches */
	if (app_regs_type[add-APP_REGS_ADD_MIN] != type)
		return false;
	
	/* Receive data */
	(*app_func_rd_pointer[add-APP_REGS_ADD_MIN])();	

	/* Return success */
	return true;
}

/************************************************************************/
/* Callbacks: Write app register                                        */
/************************************************************************/
bool core_write_app_register(uint8_t add, uint8_t type, uint8_t * content, uint16_t n_elements)
{
	/* Check if it will not access forbidden memory */
	if (add < APP_REGS_ADD_MIN || add > APP_REGS_ADD_MAX)
		return false;
	
	/* Check if type matches */
	if (app_regs_type[add-APP_REGS_ADD_MIN] != type)
		return false;

	/* Check if the number of elements matches */
	if (app_regs_n_elements[add-APP_REGS_ADD_MIN] != n_elements)
		return false;

	/* Process data and return false if write is not allowed or contains errors */
	return (*app_func_wr_pointer[add-APP_REGS_ADD_MIN])(content);
}