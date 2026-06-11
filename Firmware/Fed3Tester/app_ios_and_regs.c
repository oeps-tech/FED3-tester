#include <avr/io.h>
#include "hwbp_core_types.h"
#include "app_ios_and_regs.h"

/************************************************************************/
/* Configure and initialize IOs                                         */
/************************************************************************/
void init_ios(void)
{	/* Configure input pins */
	io_pin2in(&PORTD, 0, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_START
	io_pin2in(&PORTD, 1, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_PROG_OK
	io_pin2in(&PORTD, 2, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_PROG_NOK
	io_pin2in(&PORTD, 3, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_MOTOR_OK
	io_pin2in(&PORTD, 4, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_MOTOR_NOK
	io_pin2in(&PORTD, 5, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_RGB_OK
	io_pin2in(&PORTD, 6, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_RGB_NOK
	io_pin2in(&PORTD, 7, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_BUZZER_OK
	io_pin2in(&PORTA, 0, PULL_IO_UP, SENSE_IO_EDGES_BOTH);               // SW_BUZZER_NOK

	/* Configure input interrupts */

	/* Configure output pins */
	io_pin2out(&PORTC, 0, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // LED_3V3_AND_VBAT
	io_pin2out(&PORTC, 1, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // LED_CURRENT
	io_pin2out(&PORTC, 2, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // LED_LEDS
	io_pin2out(&PORTC, 3, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // LED_VCC_RGB
	io_pin2out(&PORTC, 5, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // LED_PASS
	io_pin2out(&PORTC, 6, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // LED_FAIL
	io_pin2out(&PORTC, 4, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // EN_VUSB_TO_DUT
	io_pin2out(&PORTC, 7, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // BUZZER
	io_pin2out(&PORTB, 0, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // EN_3V7_TO_VBAT
	io_pin2out(&PORTB, 1, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // EN_DUT_LDO_MODE
	io_pin2out(&PORTB, 2, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // SELECT_VBAT_CURR_HIGH
	io_pin2out(&PORTB, 3, OUT_IO_DIGITAL, IN_EN_IO_EN);                  // EN_DUT_3V3_LDO

	/* Initialize output pins */
	clr_LED_3V3_AND_VBAT;
	clr_LED_CURRENT;
	clr_LED_LEDS;
	clr_LED_VCC_RGB;
	clr_LED_PASS;
	clr_LED_FAIL;
	clr_EN_VUSB_TO_DUT;
	clr_BUZZER;
	clr_EN_3V7_TO_VBAT;
	clr_EN_DUT_LDO_MODE;
	set_SELECT_VBAT_CURR_HIGH;
	clr_EN_DUT_3V3_LDO;
}

/************************************************************************/
/* Registers' stuff                                                     */
/************************************************************************/
AppRegs app_regs;

uint8_t app_regs_type[] = {
	TYPE_U16,
	TYPE_FLOAT,
	TYPE_U16,
	TYPE_U16,
	TYPE_U16
};

uint16_t app_regs_n_elements[] = {
	1,
	14,
	1,
	1,
	1
};

uint8_t *app_regs_pointer[] = {
	(uint8_t*)(&app_regs.REG_SWITCHES),
	(uint8_t*)(app_regs.REG_READINGS),
	(uint8_t*)(&app_regs.REG_CONTROL),
	(uint8_t*)(&app_regs.REG_CONTROL_SET),
	(uint8_t*)(&app_regs.REG_CONTROL_CLEAR)
};