#include "chip.h"
#include "gpio.h"
#include "i2c.h"

static int wait_until_set(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = 200000UL;
    while (((*reg) & mask) == 0U) {
        if (--timeout == 0U) {
            return -1;
        }
    }
    return 0;
}

static int wait_while_set(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = 200000UL;
    while (((*reg) & mask) != 0U) {
        if (--timeout == 0U) {
            return -1;
        }
    }
    return 0;
}

void i2c1_init(void)
{
    gpio_i2c1_pins_init();

    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

    /* I2C1 clock source = SYSCLK, keep default MSI = 4 MHz */
    RCC->CCIPR &= ~(3UL << 12U);
    RCC->CCIPR |=  (1UL << 12U);

    RCC->APB1RSTR1 |=  RCC_APB1RSTR1_I2C1RST;
    RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;

    I2C1->CR1 &= ~I2C_CR1_PE;

    /* 100 kHz, source clock = 4 MHz */
    I2C1->TIMINGR = 0x00000E14UL;

    I2C1->ICR = I2C_ICR_STOPCF | I2C_ICR_NACKCF | I2C_ICR_BERRCF |
                I2C_ICR_ARLOCF | I2C_ICR_OVRCF;

    I2C1->CR1 = I2C_CR1_PE;
}

int i2c1_write(uint8_t addr7, const uint8_t *data, uint16_t len)
{
    uint16_t i;

    if ((data == 0) || (len == 0U) || (len > 255U)) {
        return -1;
    }

    if (wait_while_set(&I2C1->ISR, I2C_ISR_BUSY) < 0) {
        return -1;
    }

    I2C1->ICR = I2C_ICR_STOPCF | I2C_ICR_NACKCF | I2C_ICR_BERRCF |
                I2C_ICR_ARLOCF | I2C_ICR_OVRCF;

    I2C1->CR2 = ((uint32_t)addr7 << 1U) |
                ((uint32_t)len   << 16U) |
                I2C_CR2_AUTOEND;

    I2C1->CR2 |= I2C_CR2_START;

    for (i = 0U; i < len; i++) {
        if (wait_until_set(&I2C1->ISR, I2C_ISR_TXIS | I2C_ISR_NACKF) < 0) {
            return -1;
        }

        if ((I2C1->ISR & I2C_ISR_NACKF) != 0U) {
            I2C1->ICR = I2C_ICR_NACKCF | I2C_ICR_STOPCF;
            return -1;
        }

        I2C1->TXDR = data[i];
    }

    if (wait_until_set(&I2C1->ISR, I2C_ISR_STOPF) < 0) {
        return -1;
    }

    I2C1->ICR = I2C_ICR_STOPCF;
    return 0;
}

int i2c1_write_byte(uint8_t addr7, uint8_t first, uint8_t second)
{
    uint8_t data[2];
    data[0] = first;
    data[1] = second;
    return i2c1_write(addr7, data, 2U);
}
