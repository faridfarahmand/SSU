#include "chip.h"
#include "adc.h"

#define ADC_CHANNEL_PA0  5U
#define ADC_MAX_VALUE    4095UL
#define VREF_MV          3300UL

static void adc_delay(void)
{
    for (volatile uint32_t i = 0; i < 10000UL; i++) {
    }
}

void adc1_pa0_init(void)
{
    /* 1. Enable GPIOA clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /* 2. Set PA0 as analog mode */
    GPIOA->MODER |= (3UL << (0U * 2U));

    /* 3. No pull-up, no pull-down on PA0 */
    GPIOA->PUPDR &= ~(3UL << (0U * 2U));

    /* 4. Enable analog switch for PA0 */
    GPIOA->ASCR |= (1UL << 0U);

    /* 5. Enable ADC clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

    /* 6. ADC clock = system clock divided by 1 */
    ADC_COMMON->CCR &= ~(3UL << 16U);
    ADC_COMMON->CCR |= ADC_CCR_CKMODE_0;

    /* 7. Exit deep-power-down mode */
    ADC1->CR &= ~ADC_CR_DEEPPWD;

    /* 8. Enable ADC voltage regulator */
    ADC1->CR |= ADC_CR_ADVREGEN;
    adc_delay();

    /* 9. Make sure ADC is disabled before calibration */
    if ((ADC1->CR & ADC_CR_ADEN) != 0U) {
        ADC1->CR |= ADC_CR_ADDIS;
        while ((ADC1->CR & ADC_CR_ADEN) != 0U) {
        }
    }

    /* 10. Calibrate ADC */
    ADC1->CR |= ADC_CR_ADCAL;
    while ((ADC1->CR & ADC_CR_ADCAL) != 0U) {
    }

    /* 11. Single conversion mode, 12-bit resolution */
    ADC1->CFGR = 0x00000000UL;

    /* 12. Sample time for channel 5 */
    ADC1->SMPR1 &= ~(7UL << (ADC_CHANNEL_PA0 * 3U));
    ADC1->SMPR1 |=  (4UL << (ADC_CHANNEL_PA0 * 3U));

    /* 13. Regular sequence length = 1 conversion */
    ADC1->SQR1 = 0x00000000UL;

    /* 14. First conversion = channel 5 */
    ADC1->SQR1 |= (ADC_CHANNEL_PA0 << 6U);

    /* 15. Clear ADC ready flag */
    ADC1->ISR = ADC_ISR_ADRDY;

    /* 16. Enable ADC */
    ADC1->CR |= ADC_CR_ADEN;

    /* 17. Wait until ADC is ready */
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0U) {
    }
}

uint16_t adc1_read(void)
{
    /* Clear old flags */
    ADC1->ISR = ADC_ISR_EOC | ADC_ISR_EOS;

    /* Start conversion */
    ADC1->CR |= ADC_CR_ADSTART;

    /* Wait until conversion is complete */
    while ((ADC1->ISR & ADC_ISR_EOC) == 0U) {
    }

    /* Read ADC result */
    return (uint16_t)(ADC1->DR & 0x0FFFU);
}

uint32_t adc_to_millivolts(uint16_t adc_value)
{
    return ((uint32_t)adc_value * VREF_MV) / ADC_MAX_VALUE;
}
