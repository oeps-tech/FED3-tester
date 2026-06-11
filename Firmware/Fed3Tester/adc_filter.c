#include "adc_filter.h"

uint16_t adc_filter_buffer[ADC_QUANTITY][ADC_FILTER_N];
uint8_t adc_filter_index = 0;
uint16_t adc_filter_sum[ADC_QUANTITY];