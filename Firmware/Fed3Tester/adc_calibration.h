#ifndef _ADC_CALIBRATION_H_
#define _ADC_CALIBRATION_H_
#include <avr/io.h>
#include "adc_user_definition.h"


#define ADC_CAL_LOW_SUM16     ADC_USER_CAL_LOW_SUM16
#define ADC_CAL_LOW_UV        ADC_USER_CAL_LOW_UV

#define ADC_CAL_HIGH_SUM16    ADC_USER_CAL_HIGH_SUM16
#define ADC_CAL_HIGH_UV       ADC_USER_CAL_HIGH_UV

#define ADC_CAL_DELTA_SUM16   (ADC_CAL_HIGH_SUM16 - ADC_CAL_LOW_SUM16)
#define ADC_CAL_DELTA_UV      (ADC_CAL_HIGH_UV - ADC_CAL_LOW_UV)


// 0.49991 V = 499.91 mV
// Stored as Q20 fixed point: 499.91 * 2^20
#define ADC_CAL_LOW_MV_Q20 \
    ((int32_t)((((int64_t)ADC_CAL_LOW_UV * (1LL << 20)) + 500) / 1000))

// Gain = (ADC_CAL_HIGH_UV - ADC_CAL_LOW_UV) / (ADC_CAL_HIGH_SUM16 - ADC_CAL_LOW_SUM16)
//      = 30.84150478 uV per ADC_sum16 count
// Stored as Q10 fixed point: gain * 2^10
#define ADC_CAL_GAIN_Q10 \
    ((int32_t)((((int64_t)ADC_CAL_DELTA_UV * (1LL << 10)) + (ADC_CAL_DELTA_SUM16 / 2)) / ADC_CAL_DELTA_SUM16))

// Gain = ((ADC_CAL_HIGH_UV - ADC_CAL_LOW_UV) / 1000) / (ADC_CAL_HIGH_SUM16 - ADC_CAL_LOW_SUM16)
//      = 0.03084150478 mV per ADC_sum16 count
// Stored as Q20 fixed point: gain * 2^20
#define ADC_CAL_GAIN_MV_Q20 \
    ((int32_t)((((int64_t)ADC_CAL_DELTA_UV * (1LL << 20)) + (((int64_t)ADC_CAL_DELTA_SUM16 * 1000) / 2)) / ((int64_t)ADC_CAL_DELTA_SUM16 * 1000)))


// Convert sum of 16 ADC samples to microvolts
int32_t adc_sum16_to_uV(uint16_t adc_sum16);

// Convert sum of 16 ADC samples to millivolts
int32_t adc_sum16_to_mV(uint16_t adc_sum16);


#endif /* _ADC_CALIBRATION_H_ */