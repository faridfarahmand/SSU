#ifndef KEYPAD_H
#define KEYPAD_H

#include "gpio.h"
#include <stdint.h>

#define KEYPAD_PORT GPIOA

/* Rows */
#define KEYPAD_ROW0_PIN 0U
#define KEYPAD_ROW1_PIN 1U
#define KEYPAD_ROW2_PIN 2U
#define KEYPAD_ROW3_PIN 3U

/* Columns */
#define KEYPAD_COL0_PIN 4U
#define KEYPAD_COL1_PIN 5U
#define KEYPAD_COL2_PIN 6U
#define KEYPAD_COL3_PIN 7U

void KEYPAD_Init(void);
uint8_t KEYPAD_GetIndex(void);

#endif
