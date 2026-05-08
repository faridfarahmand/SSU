#include "gpio.h"

void GPIO_EnableClock(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == GPIOA)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    }
    else if (GPIOx == GPIOB)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    }
    else if (GPIOx == GPIOC)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    }
    else if (GPIOx == GPIOD)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    }
    else if (GPIOx == GPIOE)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    }
    else if (GPIOx == GPIOF)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
    }
    else if (GPIOx == GPIOG)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
    }
    else if (GPIOx == GPIOH)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
    }

    (void)RCC->AHB2ENR;
}

void GPIO_InitPin(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_Mode_t mode, GPIO_Pull_t pull)
{
    GPIO_EnableClock(GPIOx);

    GPIOx->MODER &= ~(0x3UL << (pin * 2U));
    GPIOx->MODER |= ((uint32_t)mode << (pin * 2U));

    if (mode == GPIO_MODE_OUTPUT)
    {
        GPIOx->OTYPER &= ~(1UL << pin);
        GPIOx->OSPEEDR &= ~(0x3UL << (pin * 2U));
    }

    GPIOx->PUPDR &= ~(0x3UL << (pin * 2U));
    GPIOx->PUPDR |= ((uint32_t)pull << (pin * 2U));
}

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_PinState_t state)
{
    if (state == GPIO_PIN_SET)
    {
        GPIOx->BSRR = (1UL << pin);
    }
    else
    {
        GPIOx->BSRR = (1UL << (pin + 16U));
    }
}

GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    return ((GPIOx->IDR & (1UL << pin)) != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}
