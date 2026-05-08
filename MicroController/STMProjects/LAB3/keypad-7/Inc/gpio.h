#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define __IO volatile
#define __NOP() __asm volatile ("nop")

#define PERIPH_BASE             0x40000000UL
#define APB1PERIPH_BASE         (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE         (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE         (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE         (PERIPH_BASE + 0x08000000UL)

#define RCC_BASE                (AHB1PERIPH_BASE + 0x00001000UL)

#define GPIOA_BASE              (AHB2PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE              (AHB2PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE              (AHB2PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE              (AHB2PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE              (AHB2PERIPH_BASE + 0x1000UL)
#define GPIOF_BASE              (AHB2PERIPH_BASE + 0x1400UL)
#define GPIOG_BASE              (AHB2PERIPH_BASE + 0x1800UL)
#define GPIOH_BASE              (AHB2PERIPH_BASE + 0x1C00UL)

typedef struct
{
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFR[2];
    __IO uint32_t BRR;
    __IO uint32_t ASCR;
} GPIO_TypeDef;

typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t ICSCR;
    __IO uint32_t CFGR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t PLLSAI1CFGR;
    __IO uint32_t PLLSAI2CFGR;
    __IO uint32_t CIER;
    __IO uint32_t CIFR;
    __IO uint32_t CICR;
    uint32_t RESERVED0;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    __IO uint32_t AHB3RSTR;
    uint32_t RESERVED1;
    __IO uint32_t APB1RSTR1;
    __IO uint32_t APB1RSTR2;
    __IO uint32_t APB2RSTR;
    uint32_t RESERVED2;
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
} RCC_TypeDef;

#define GPIOA                   ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                   ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC                   ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD                   ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE                   ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF                   ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG                   ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH                   ((GPIO_TypeDef *) GPIOH_BASE)
#define RCC                     ((RCC_TypeDef *) RCC_BASE)

#define RCC_AHB2ENR_GPIOAEN     (1UL << 0)
#define RCC_AHB2ENR_GPIOBEN     (1UL << 1)
#define RCC_AHB2ENR_GPIOCEN     (1UL << 2)
#define RCC_AHB2ENR_GPIODEN     (1UL << 3)
#define RCC_AHB2ENR_GPIOEEN     (1UL << 4)
#define RCC_AHB2ENR_GPIOFEN     (1UL << 5)
#define RCC_AHB2ENR_GPIOGEN     (1UL << 6)
#define RCC_AHB2ENR_GPIOHEN     (1UL << 7)

typedef enum
{
    GPIO_MODE_INPUT  = 0U,
    GPIO_MODE_OUTPUT = 1U
} GPIO_Mode_t;

typedef enum
{
    GPIO_NO_PULL   = 0U,
    GPIO_PULL_UP   = 1U,
    GPIO_PULL_DOWN = 2U
} GPIO_Pull_t;

typedef enum
{
    GPIO_PIN_RESET = 0U,
    GPIO_PIN_SET   = 1U
} GPIO_PinState_t;

void GPIO_EnableClock(GPIO_TypeDef *GPIOx);
void GPIO_InitPin(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_Mode_t mode, GPIO_Pull_t pull);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_PinState_t state);
GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin);

#endif
