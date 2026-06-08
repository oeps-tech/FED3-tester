#ifndef _APP_FUNCTIONS_H_
#define _APP_FUNCTIONS_H_
#include <avr/io.h>


/************************************************************************/
/* Define if not defined                                                */
/************************************************************************/
#ifndef bool
	#define bool uint8_t
#endif
#ifndef true
	#define true 1
#endif
#ifndef false
	#define false 0
#endif


/************************************************************************/
/* Prototypes                                                           */
/************************************************************************/
void app_read_REG_SWITCHES(void);
void app_read_REG_READINGS(void);
void app_read_REG_CONTROL(void);
void app_read_REG_CONTROL_SET(void);
void app_read_REG_CONTROL_CLEAR(void);

bool app_write_REG_SWITCHES(void *a);
bool app_write_REG_READINGS(void *a);
bool app_write_REG_CONTROL(void *a);
bool app_write_REG_CONTROL_SET(void *a);
bool app_write_REG_CONTROL_CLEAR(void *a);


#endif /* _APP_FUNCTIONS_H_ */