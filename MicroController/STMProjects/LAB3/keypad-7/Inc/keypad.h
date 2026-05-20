#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include "gpio.h"

/*
 * STM32L476RG keypad driver using Edison-style scan:
 * - Rows are INPUTS with PULL-DOWN
 * - Columns are OUTPUTS
 * - One column is driven HIGH at a time
 * - Driver blocks until key release
 * - No delay / no debounce loop
 *
 * Wiring:
 *   Rows (L1..L4):
 *     PA0 -> L1 = 123A
 *     PA1 -> L2 = 456B
 *     PA8 -> L3 = 789C
 *     PA9 -> L4 = *0#D
 *
 *   Columns (C1..C4):
 *     PA4 -> C1 = 147*
 *     PA5 -> C2 = 2580
 *     PA6 -> C3 = 369#
 *     PA7 -> C4 = ABCD
 */

#define KEYPAD_PORT GPIOA

/* Row inputs */
#define KEYPAD_ROW0_PIN 0U
#define KEYPAD_ROW1_PIN 1U
#define KEYPAD_ROW2_PIN 9U
#define KEYPAD_ROW3_PIN 8U

/* Column outputs */
#define KEYPAD_COL0_PIN 4U
#define KEYPAD_COL1_PIN 5U
#define KEYPAD_COL2_PIN 6U
#define KEYPAD_COL3_PIN 7U

void KEYPAD_Init(void);
char KEYPAD_GetKey(void);

#endif
