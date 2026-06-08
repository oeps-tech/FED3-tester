#ifndef _APP_IOS_AND_REGS_H_
#define _APP_IOS_AND_REGS_H_
#include "cpu.h"

void init_ios(void);
/************************************************************************/
/* Definition of input pins                                             */
/************************************************************************/
// SW_START               Description: 
// SW_PROG_OK             Description: 
// SW_PROG_NOK            Description: 
// SW_RGB_OK              Description: 
// SW_RGB_NOK             Description: 
// SW_MOTOR_OK            Description: 
// SW_MOTOR_NOK           Description: 
// SW_BUZZER_OK           Description: 
// SW_BUZZER_NOK          Description: 

#define read_SW_START read_io(PORTD, 0)         // SW_START
#define read_SW_PROG_OK read_io(PORTD, 1)       // SW_PROG_OK
#define read_SW_PROG_NOK read_io(PORTD, 2)      // SW_PROG_NOK
#define read_SW_RGB_OK read_io(PORTD, 3)        // SW_RGB_OK
#define read_SW_RGB_NOK read_io(PORTD, 4)       // SW_RGB_NOK
#define read_SW_MOTOR_OK read_io(PORTD, 5)      // SW_MOTOR_OK
#define read_SW_MOTOR_NOK read_io(PORTD, 6)     // SW_MOTOR_NOK
#define read_SW_BUZZER_OK read_io(PORTD, 7)     // SW_BUZZER_OK
#define read_SW_BUZZER_NOK read_io(PORTA, 0)    // SW_BUZZER_NOK

/************************************************************************/
/* Definition of output pins                                            */
/************************************************************************/
// LED_3V3_AND_VBAT       Description: 
// LED_CURRENT            Description: 
// LED_LEDS               Description: 
// LED_VCC_RGB            Description: 
// LED_PASS               Description: 
// LED_FAIL               Description: 
// EN_VUSB_TO_DUT         Description: 
// BUZZER                 Description: 
// EN_3V7_TO_VBAT         Description: 
// EN_DUT_LDO_MODE        Description: 
// SELECT_VBAT_CURR_HIGH  Description: 
// EN_DUT_3V3_LDO         Description: 

/* LED_3V3_AND_VBAT */
#define set_LED_3V3_AND_VBAT set_io(PORTC, 0)
#define clr_LED_3V3_AND_VBAT clear_io(PORTC, 0)
#define tgl_LED_3V3_AND_VBAT toggle_io(PORTC, 0)
#define read_LED_3V3_AND_VBAT read_io(PORTC, 0)

/* LED_CURRENT */
#define set_LED_CURRENT set_io(PORTC, 1)
#define clr_LED_CURRENT clear_io(PORTC, 1)
#define tgl_LED_CURRENT toggle_io(PORTC, 1)
#define read_LED_CURRENT read_io(PORTC, 1)

/* LED_LEDS */
#define set_LED_LEDS set_io(PORTC, 2)
#define clr_LED_LEDS clear_io(PORTC, 2)
#define tgl_LED_LEDS toggle_io(PORTC, 2)
#define read_LED_LEDS read_io(PORTC, 2)

/* LED_VCC_RGB */
#define set_LED_VCC_RGB set_io(PORTC, 3)
#define clr_LED_VCC_RGB clear_io(PORTC, 3)
#define tgl_LED_VCC_RGB toggle_io(PORTC, 3)
#define read_LED_VCC_RGB read_io(PORTC, 3)

/* LED_PASS */
#define set_LED_PASS set_io(PORTC, 5)
#define clr_LED_PASS clear_io(PORTC, 5)
#define tgl_LED_PASS toggle_io(PORTC, 5)
#define read_LED_PASS read_io(PORTC, 5)

/* LED_FAIL */
#define set_LED_FAIL set_io(PORTC, 6)
#define clr_LED_FAIL clear_io(PORTC, 6)
#define tgl_LED_FAIL toggle_io(PORTC, 6)
#define read_LED_FAIL read_io(PORTC, 6)

/* EN_VUSB_TO_DUT */
#define set_EN_VUSB_TO_DUT set_io(PORTC, 4)
#define clr_EN_VUSB_TO_DUT clear_io(PORTC, 4)
#define tgl_EN_VUSB_TO_DUT toggle_io(PORTC, 4)
#define read_EN_VUSB_TO_DUT read_io(PORTC, 4)

/* BUZZER */
#define set_BUZZER set_io(PORTC, 7)
#define clr_BUZZER clear_io(PORTC, 7)
#define tgl_BUZZER toggle_io(PORTC, 7)
#define read_BUZZER read_io(PORTC, 7)

/* EN_3V7_TO_VBAT */
#define set_EN_3V7_TO_VBAT set_io(PORTB, 0)
#define clr_EN_3V7_TO_VBAT clear_io(PORTB, 0)
#define tgl_EN_3V7_TO_VBAT toggle_io(PORTB, 0)
#define read_EN_3V7_TO_VBAT read_io(PORTB, 0)

/* EN_DUT_LDO_MODE */
#define set_EN_DUT_LDO_MODE clear_io(PORTB, 1)
#define clr_EN_DUT_LDO_MODE set_io(PORTB, 1)
#define tgl_EN_DUT_LDO_MODE toggle_io(PORTB, 1)
#define read_EN_DUT_LDO_MODE read_io(PORTB, 1)

/* SELECT_VBAT_CURR_HIGH */
#define set_SELECT_VBAT_CURR_HIGH set_io(PORTB, 2)
#define clr_SELECT_VBAT_CURR_HIGH clear_io(PORTB, 2)
#define tgl_SELECT_VBAT_CURR_HIGH toggle_io(PORTB, 2)
#define read_SELECT_VBAT_CURR_HIGH read_io(PORTB, 2)

/* EN_DUT_3V3_LDO */
#define set_EN_DUT_3V3_LDO set_io(PORTB, 3)
#define clr_EN_DUT_3V3_LDO clear_io(PORTB, 3)
#define tgl_EN_DUT_3V3_LDO toggle_io(PORTB, 3)
#define read_EN_DUT_3V3_LDO read_io(PORTB, 3)


/************************************************************************/
/* Registers' structure                                                 */
/************************************************************************/
typedef struct
{
	uint16_t REG_SWITCHES;
	float REG_READINGS[14];
	uint16_t REG_CONTROL;
	uint16_t REG_CONTROL_SET;
	uint16_t REG_CONTROL_CLEAR;
} AppRegs;

/************************************************************************/
/* Registers' address                                                   */
/************************************************************************/
/* Registers */
#define ADD_REG_SWITCHES                    32 // U16    Controls the switches like they were pressed in the physical board
#define ADD_REG_READINGS                    33 // FLOAT  V_Vbat, V_VccRgb, V_3V3, V_LedCharge, V_LedRed, V_LedGreen, I_VbatLow, I_VbatHigh, SW_Start, SW_Prog, SW_Rgb, SW_Motor, SW_Buzzer, PassFail
#define ADD_REG_CONTROL                     34 // U16    Controls internal signals
#define ADD_REG_CONTROL_SET                 35 // U16    Controls internal signals
#define ADD_REG_CONTROL_CLEAR               36 // U16    Controls internal signals

/************************************************************************/
/* PWM Generator registers' memory limits                               */
/*                                                                      */
/* DON'T change the APP_REGS_ADD_MIN value !!!                          */
/* DON'T change these names !!!                                         */
/************************************************************************/
/* Memory limits */
#define APP_REGS_ADD_MIN                    0x20
#define APP_REGS_ADD_MAX                    0x24
#define APP_NBYTES_OF_REG_BANK              64

/************************************************************************/
/* Registers' bits                                                      */
/************************************************************************/
#define B_START                            (1<<0)       // 
#define B_PROG_OK                          (1<<1)       // 
#define B_PROG_NOK                         (1<<2)       // 
#define B_RGB_OK                           (1<<3)       // 
#define B_RGB_NOK                          (1<<4)       // 
#define B_MOTOR_OK                         (1<<5)       // 
#define B_MOTOR_NOK                        (1<<6)       // 
#define B_BUZZER_OK                        (1<<7)       // 
#define B_BUZZER_NOK                       (1<<8)       // 
#define B_EN_VUSB_TO_DUT                   (1<<0)       // 
#define B_EN_3V7_TO_VBAT                   (1<<1)       // 
#define B_EN_DUT_LDO_MODE                  (1<<2)       // 
#define B_SELECT_VBAT_CURR_HIGH            (1<<3)       // 
#define B_EN_DUT_3V3_LDO                   (1<<4)       // 
#define B_LED_3V3_AND_VBAT                 (1<<5)       // 
#define B_LED_CURRENT                      (1<<6)       // 
#define B_LED_LEDS                         (1<<7)       // 
#define B_LED_VCC_RGB                      (1<<8)       // 
#define B_LED_PASS                         (1<<9)       // 
#define B_LED_FAIL                         (1<<10)      // 

#endif /* _APP_REGS_H_ */