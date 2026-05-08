#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>

#define __IO volatile

typedef struct {
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
} gpio_regs_t;

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t ICSCR;
    __IO uint32_t CFGR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t PLLSAI1CFGR;
    __IO uint32_t PLLSAI2CFGR;
    __IO uint32_t CIER;
    __IO uint32_t CIFR;
    __IO uint32_t CICR;
    __IO uint32_t RESERVED0;
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
    __IO uint32_t AHB3ENR;
    uint32_t RESERVED3;
    __IO uint32_t APB1ENR1;
    __IO uint32_t APB1ENR2;
    __IO uint32_t APB2ENR;
    uint32_t RESERVED4;
    __IO uint32_t AHB1SMENR;
    __IO uint32_t AHB2SMENR;
    __IO uint32_t AHB3SMENR;
    uint32_t RESERVED5;
    __IO uint32_t APB1SMENR1;
    __IO uint32_t APB1SMENR2;
    __IO uint32_t APB2SMENR;
    uint32_t RESERVED6;
    __IO uint32_t CCIPR;
    uint32_t RESERVED7;
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
    __IO uint32_t CRRCR;
    __IO uint32_t CCIPR2;
} rcc_regs_t;

typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t OAR1;
    __IO uint32_t OAR2;
    __IO uint32_t TIMINGR;
    __IO uint32_t TIMEOUTR;
    __IO uint32_t ISR;
    __IO uint32_t ICR;
    __IO uint32_t PECR;
    __IO uint32_t RXDR;
    __IO uint32_t TXDR;
} i2c_regs_t;

#define RCC     ((rcc_regs_t  *)0x40021000UL)
#define GPIOB   ((gpio_regs_t *)0x48000400UL)
#define I2C1    ((i2c_regs_t  *)0x40005400UL)

#define RCC_AHB2ENR_GPIOBEN      (1UL << 1)
#define RCC_APB1ENR1_I2C1EN      (1UL << 21)
#define RCC_APB1RSTR1_I2C1RST    (1UL << 21)

#define I2C_CR1_PE               (1UL << 0)

#define I2C_CR2_START            (1UL << 13)
#define I2C_CR2_STOP             (1UL << 14)
#define I2C_CR2_NACK             (1UL << 15)
#define I2C_CR2_AUTOEND          (1UL << 25)

#define I2C_ISR_TXIS             (1UL << 1)
#define I2C_ISR_NACKF            (1UL << 4)
#define I2C_ISR_STOPF            (1UL << 5)
#define I2C_ISR_TC               (1UL << 6)
#define I2C_ISR_BUSY             (1UL << 15)

#define I2C_ICR_NACKCF           (1UL << 4)
#define I2C_ICR_STOPCF           (1UL << 5)
#define I2C_ICR_BERRCF           (1UL << 8)
#define I2C_ICR_ARLOCF           (1UL << 9)
#define I2C_ICR_OVRCF            (1UL << 10)

/* ================= ADC / GPIOA ADDITIONS ================= */

typedef struct {
    __IO uint32_t ISR;
    __IO uint32_t IER;
    __IO uint32_t CR;
    __IO uint32_t CFGR;
    __IO uint32_t CFGR2;
    __IO uint32_t SMPR1;
    __IO uint32_t SMPR2;
    uint32_t RESERVED0;
    __IO uint32_t TR1;
    __IO uint32_t TR2;
    __IO uint32_t TR3;
    uint32_t RESERVED1;
    __IO uint32_t SQR1;
    __IO uint32_t SQR2;
    __IO uint32_t SQR3;
    __IO uint32_t SQR4;
    __IO uint32_t DR;
} adc_regs_t;

typedef struct {
    __IO uint32_t CSR;
    uint32_t RESERVED;
    __IO uint32_t CCR;
    __IO uint32_t CDR;
} adc_common_regs_t;

#define GPIOA       ((gpio_regs_t *)0x48000000UL)
#define ADC1        ((adc_regs_t *)0x50040000UL)
#define ADC_COMMON  ((adc_common_regs_t *)0x50040300UL)

#define RCC_AHB2ENR_GPIOAEN      (1UL << 0)
#define RCC_AHB2ENR_ADCEN        (1UL << 13)

#define ADC_ISR_ADRDY            (1UL << 0)
#define ADC_ISR_EOC              (1UL << 2)
#define ADC_ISR_EOS              (1UL << 3)

#define ADC_CR_ADEN              (1UL << 0)
#define ADC_CR_ADDIS             (1UL << 1)
#define ADC_CR_ADSTART           (1UL << 2)
#define ADC_CR_ADSTP             (1UL << 4)
#define ADC_CR_ADVREGEN          (1UL << 28)
#define ADC_CR_DEEPPWD           (1UL << 29)
#define ADC_CR_ADCAL             (1UL << 31)

#define ADC_CFGR_CONT            (1UL << 13)

#define ADC_CCR_CKMODE_0         (1UL << 16)

#endif
