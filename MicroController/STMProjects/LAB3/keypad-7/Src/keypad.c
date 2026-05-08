#include "keypad.h"

static const uint8_t rowPins[4] =
{
    KEYPAD_ROW0_PIN,
    KEYPAD_ROW1_PIN,
    KEYPAD_ROW2_PIN,
    KEYPAD_ROW3_PIN
};

static const uint8_t colPins[4] =
{
    KEYPAD_COL0_PIN,
    KEYPAD_COL1_PIN,
    KEYPAD_COL2_PIN,
    KEYPAD_COL3_PIN
};

static void KEYPAD_SetAllRowsHigh(void)
{
    GPIO_WritePin(KEYPAD_PORT, rowPins[0], GPIO_PIN_SET);
    GPIO_WritePin(KEYPAD_PORT, rowPins[1], GPIO_PIN_SET);
    GPIO_WritePin(KEYPAD_PORT, rowPins[2], GPIO_PIN_SET);
    GPIO_WritePin(KEYPAD_PORT, rowPins[3], GPIO_PIN_SET);
}

static void KEYPAD_SetSingleRowLow(uint8_t row)
{
    KEYPAD_SetAllRowsHigh();
    GPIO_WritePin(KEYPAD_PORT, rowPins[row], GPIO_PIN_RESET);
}

void KEYPAD_Init(void)
{
    GPIO_InitPin(KEYPAD_PORT, rowPins[0], GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(KEYPAD_PORT, rowPins[1], GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(KEYPAD_PORT, rowPins[2], GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(KEYPAD_PORT, rowPins[3], GPIO_MODE_OUTPUT, GPIO_NO_PULL);

    GPIO_InitPin(KEYPAD_PORT, colPins[0], GPIO_MODE_INPUT, GPIO_PULL_UP);
    GPIO_InitPin(KEYPAD_PORT, colPins[1], GPIO_MODE_INPUT, GPIO_PULL_UP);
    GPIO_InitPin(KEYPAD_PORT, colPins[2], GPIO_MODE_INPUT, GPIO_PULL_UP);
    GPIO_InitPin(KEYPAD_PORT, colPins[3], GPIO_MODE_INPUT, GPIO_PULL_UP);

    KEYPAD_SetAllRowsHigh();
}

/* returns 0..15, or 0xFF if no key */
uint8_t KEYPAD_GetIndex(void)
{
    uint8_t row;
    uint8_t col;

    for (row = 0U; row < 4U; row++)
    {
        KEYPAD_SetSingleRowLow(row);

        for (col = 0U; col < 4U; col++)
        {
            if (GPIO_ReadPin(KEYPAD_PORT, colPins[col]) == GPIO_PIN_RESET)
            {
                uint8_t index = (uint8_t)(row * 4U + col);

                while (GPIO_ReadPin(KEYPAD_PORT, colPins[col]) == GPIO_PIN_RESET)
                {
                    /* block until release */
                }

                KEYPAD_SetAllRowsHigh();
                return index;
            }
        }
    }

    KEYPAD_SetAllRowsHigh();
    return 0xFFU;
}
