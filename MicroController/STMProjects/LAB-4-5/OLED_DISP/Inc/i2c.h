#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c1_init(void);
int  i2c1_write(uint8_t addr7, const uint8_t *data, uint16_t len);
int  i2c1_write_byte(uint8_t addr7, uint8_t first, uint8_t second);

#endif
