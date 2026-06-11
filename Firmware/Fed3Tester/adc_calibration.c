#include "adc_calibration.h"

/* Notes: Initial tests show that the "adc_sum_to_uV" is 2x faster compared "to adc_sum16_to_mV" */

int32_t adc_sum16_to_uV(uint16_t adc_sum16)
{
    int32_t diff = (int32_t)adc_sum16 - ADC_CAL_LOW_SUM16;
    int32_t delta_uV;

    if (diff >= 0)
    {
        delta_uV = (diff * ADC_CAL_GAIN_Q10 + 512) >> 10;
    }
    else
    {
        delta_uV = -(((-diff) * ADC_CAL_GAIN_Q10 + 512) >> 10);
    }

    return ADC_CAL_LOW_UV + delta_uV;
}

int32_t adc_sum16_to_mV(uint16_t adc_sum16)
{
    int32_t diff = (int32_t)adc_sum16 - ADC_CAL_LOW_SUM16;
    int32_t voltage_mV_q20;

    voltage_mV_q20 = ADC_CAL_LOW_MV_Q20 + diff * ADC_CAL_GAIN_MV_Q20;

    // Round and convert Q20 to signed integer mV
    if (voltage_mV_q20 >= 0)
    {
        return (voltage_mV_q20 + (1L << 19)) >> 20;
    }
    else
    {
        return -(((-voltage_mV_q20) + (1L << 19)) >> 20);
    }
}