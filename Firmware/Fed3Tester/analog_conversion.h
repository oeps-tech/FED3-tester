#ifndef _ANALOG_CONVERSION_H_
#define _ANALOG_CONVERSION_H_

#include "adc_filter.h"

#define get_V_Vbat()      (adc_get_uV(0) / ((5.1/(10+5.1))*1000000.0))
#define get_V_VccRgb()    (adc_get_uV(1) / ((5.1/(10+5.1))*1000000.0))
#define get_V_3V3()       (adc_get_uV(2) / ((5.1/(10+5.1))*1000000.0))
#define get_V_LedCharge() (adc_get_uV(3) / ((1000.0/(649+1000))*1000000.0))
#define get_V_LedRed()    (adc_get_uV(4) / 1000000.0)
#define get_V_LedGreen()  (adc_get_uV(5) / 1000000.0)
#define get_I_Vbat()      (adc_get_uV(6))

#endif /* _ANALOG_CONVERSION_H_ */