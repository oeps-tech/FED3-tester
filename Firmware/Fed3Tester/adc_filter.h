#ifndef ADC_FILTER_H
#define ADC_FILTER_H
#include <avr/io.h>
#include "adc_user_definition.h"


#define ADC_QUANTITY ADC_USER_NUMBER_OF_ADCs
#define ADC_FILTER_N 16


extern uint16_t adc_filter_buffer[ADC_QUANTITY][ADC_FILTER_N];
extern uint8_t adc_filter_index;
extern uint16_t adc_filter_sum[ADC_QUANTITY];


#define adc_add_measurement(channel, value) \
{ \
    uint8_t index = adc_filter_index; \
	\
    adc_filter_sum[channel] -= adc_filter_buffer[channel][index]; \
    adc_filter_buffer[channel][index] = value; \
    adc_filter_sum[channel] += value; \
}

#define  adc_next_index() adc_filter_index = (adc_filter_index + 1) & 0x0F   // Fast version for ADC_FILTER_N = 16

#define adc_get_uV(a) adc_sum16_to_uV(adc_filter_sum[a])
#define adc_get_mV(a) adc_sum16_to_mV(adc_filter_sum[a])


#endif /* ADC_FILTER_H */