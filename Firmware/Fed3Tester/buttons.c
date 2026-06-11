#include "buttons.h"
#include "cpu.h"
#include "app_ios_and_regs.h"

void init_buttons_timer (void)
{
	/* Initialize a 5 ms timer to handle the buttons */
	timer_type0_enable(&TCC0, TIMER_PRESCALER_DIV1024, 156, INT_LEVEL_LOW);
}

static buttons_t buttons_previous;
static buttons_t buttons_current;
buttons_t buttons;

buttons_t buttons_from_registers;
buttons_timeout_t buttons_from_registers_timeout;

ISR(TCC0_OVF_vect, ISR_NAKED)
{
	buttons_current.start = !read_SW_START;	
				
	buttons_current.prog_ok = !read_SW_PROG_OK;
	buttons_current.prog_nok = !read_SW_PROG_NOK;
				
	buttons_current.rgb_ok = !read_SW_RGB_OK;
	buttons_current.rgb_nok = !read_SW_RGB_NOK;
				
	buttons_current.motor_ok = !read_SW_MOTOR_OK;
	buttons_current.motor_nok = !read_SW_MOTOR_NOK;
	
	buttons_current.buzzer_ok = !read_SW_BUZZER_OK;
	buttons_current.buzzer_nok = !read_SW_BUZZER_NOK;
	
	if (buttons_current.start == buttons_previous.start) buttons.start = buttons_from_registers.start ? true : buttons_current.start;
	if (buttons_from_registers_timeout.start) buttons_from_registers_timeout.start--;
	else buttons_from_registers.start = false;
	
	if (buttons_current.prog_ok == buttons_previous.prog_ok) buttons.prog_ok = buttons_from_registers.prog_ok ? true : buttons_current.prog_ok;
	if (buttons_from_registers_timeout.prog_ok) buttons_from_registers_timeout.prog_ok--;
	else buttons_from_registers.prog_ok = false;
	
	if (buttons_current.prog_nok == buttons_previous.prog_nok) buttons.prog_nok = buttons_from_registers.prog_nok ? true : buttons_current.prog_nok;
	if (buttons_from_registers_timeout.prog_nok) buttons_from_registers_timeout.prog_nok--;
	else buttons_from_registers.prog_nok = false;
	
	if (buttons_current.rgb_ok == buttons_previous.rgb_ok) buttons.rgb_ok = buttons_from_registers.rgb_ok ? true : buttons_current.rgb_ok;
	if (buttons_from_registers_timeout.rgb_ok) buttons_from_registers_timeout.rgb_ok--;
	else buttons_from_registers.rgb_ok = false;
	
	if (buttons_current.rgb_nok == buttons_previous.rgb_nok) buttons.rgb_nok = buttons_from_registers.rgb_nok ? true : buttons_current.rgb_nok;
	if (buttons_from_registers_timeout.rgb_nok) buttons_from_registers_timeout.rgb_nok--;
	else buttons_from_registers.rgb_nok = false;
	
	if (buttons_current.motor_ok == buttons_previous.motor_ok) buttons.motor_ok = buttons_from_registers.motor_ok ? true : buttons_current.motor_ok;
	if (buttons_from_registers_timeout.motor_ok) buttons_from_registers_timeout.motor_ok--;
	else buttons_from_registers.motor_ok = false;
	
	if (buttons_current.motor_nok == buttons_previous.motor_nok) buttons.motor_nok = buttons_from_registers.motor_nok ? true : buttons_current.motor_nok;
	if (buttons_from_registers_timeout.motor_nok) buttons_from_registers_timeout.motor_nok--;
	else buttons_from_registers.motor_nok = false;
	
	if (buttons_current.buzzer_ok == buttons_previous.buzzer_ok) buttons.buzzer_ok = buttons_from_registers.buzzer_ok ? true : buttons_current.buzzer_ok;
	if (buttons_from_registers_timeout.buzzer_ok) buttons_from_registers_timeout.buzzer_ok--;
	else buttons_from_registers.buzzer_ok = false;
	
	if (buttons_current.buzzer_nok == buttons_previous.buzzer_nok) buttons.buzzer_nok = buttons_from_registers.buzzer_nok ? true : buttons_current.buzzer_nok;
	if (buttons_from_registers_timeout.buzzer_nok) buttons_from_registers_timeout.buzzer_nok--;
	else buttons_from_registers.buzzer_nok = false;
	
	buttons_previous = buttons_current;
	
	reti();
}
