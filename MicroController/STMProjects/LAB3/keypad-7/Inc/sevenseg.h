#ifndef SEVENSEG_H
#define SEVENSEG_H

#include "gpio.h"
#include <stdint.h>

#define SEVENSEG_PORT GPIOB

#define SEVENSEG_SEG_A_PIN 0U
#define SEVENSEG_SEG_B_PIN 1U
#define SEVENSEG_SEG_C_PIN 2U
#define SEVENSEG_SEG_D_PIN 3U
#define SEVENSEG_SEG_E_PIN 4U
#define SEVENSEG_SEG_F_PIN 5U
#define SEVENSEG_SEG_G_PIN 6U

void SevenSeg_Init(void);
void SevenSeg_Clear(void);
void SevenSeg_DisplayRaw(uint8_t pattern);
void SevenSeg_DisplayDigit(uint8_t digit);
void SevenSeg_DisplayChar(char c);

#endif
