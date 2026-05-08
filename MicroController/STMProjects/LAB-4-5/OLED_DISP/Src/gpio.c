#include "chip.h"
#include "gpio.h"

void gpio_i2c1_pins_init(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    /* PB8 = I2C1_SCL, PB9 = I2C1_SDA */
    GPIOB->MODER &= ~((3UL << (8U * 2U)) | (3UL << (9U * 2U)));
    GPIOB->MODER |=  ((2UL << (8U * 2U)) | (2UL << (9U * 2U)));

    GPIOB->OTYPER |=  (1UL << 8U) | (1UL << 9U);

    GPIOB->OSPEEDR &= ~((3UL << (8U * 2U)) | (3UL << (9U * 2U)));
    GPIOB->OSPEEDR |=  ((2UL << (8U * 2U)) | (2UL << (9U * 2U)));

    GPIOB->PUPDR &= ~((3UL << (8U * 2U)) | (3UL << (9U * 2U)));
    GPIOB->PUPDR |=  ((1UL << (8U * 2U)) | (1UL << (9U * 2U)));

    GPIOB->AFR[1] &= ~((0xFUL << 0U) | (0xFUL << 4U));
    GPIOB->AFR[1] |=  ((4UL   << 0U) | (4UL   << 4U));
}
