#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc1_pa0_init(void);
uint16_t adc1_read(void);
uint32_t adc_to_millivolts(uint16_t adc_value);

#endif
