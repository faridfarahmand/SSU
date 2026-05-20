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

static void KEYPAD_AllColumnsLow(void)
{
    GPIO_WritePin(KEYPAD_PORT, colPins[0], GPIO_PIN_RESET);
    GPIO_WritePin(KEYPAD_PORT, colPins[1], GPIO_PIN_RESET);
    GPIO_WritePin(KEYPAD_PORT, colPins[2], GPIO_PIN_RESET);
    GPIO_WritePin(KEYPAD_PORT, colPins[3], GPIO_PIN_RESET);
}

static void KEYPAD_SetColumnHigh(uint8_t col)
{
    KEYPAD_AllColumnsLow();
    GPIO_WritePin(KEYPAD_PORT, colPins[col], GPIO_PIN_SET);
}

void KEYPAD_Init(void)
{
    /* Rows = input pull-down */
    GPIO_InitPin(KEYPAD_PORT, rowPins[0], GPIO_MODE_INPUT, GPIO_PULL_DOWN);
    GPIO_InitPin(KEYPAD_PORT, rowPins[1], GPIO_MODE_INPUT, GPIO_PULL_DOWN);
    GPIO_InitPin(KEYPAD_PORT, rowPins[2], GPIO_MODE_INPUT, GPIO_PULL_DOWN);
    GPIO_InitPin(KEYPAD_PORT, rowPins[3], GPIO_MODE_INPUT, GPIO_PULL_DOWN);

    /* Columns = output */
    GPIO_InitPin(KEYPAD_PORT, colPins[0], GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(KEYPAD_PORT, colPins[1], GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(KEYPAD_PORT, colPins[2], GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(KEYPAD_PORT, colPins[3], GPIO_MODE_OUTPUT, GPIO_NO_PULL);

    KEYPAD_AllColumnsLow();
}

char KEYPAD_GetKey(void)
{
    while (1)
    {
        /* Column 0 -> 1,4,7,* */
        KEYPAD_SetColumnHigh(0U);

        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[0]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[0]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '1';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[1]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[1]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '4';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[2]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[2]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '7';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[3]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[3]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '*';
        }

        /* Column 1 -> 2,5,8,0 */
        KEYPAD_SetColumnHigh(1U);

        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[0]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[0]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '2';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[1]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[1]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '5';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[2]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[2]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '8';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[3]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[3]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '0';
        }

        /* Column 2 -> 3,6,9,# */
        KEYPAD_SetColumnHigh(2U);

        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[0]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[0]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '3';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[1]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[1]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '6';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[2]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[2]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '9';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[3]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[3]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return '#';
        }

        /* Column 3 -> A,B,C,D */
        KEYPAD_SetColumnHigh(3U);

        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[0]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[0]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return 'A';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[1]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[1]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return 'B';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[2]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[2]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return 'C';
        }
        if (GPIO_ReadPin(KEYPAD_PORT, rowPins[3]) == GPIO_PIN_SET)
        {
            while (GPIO_ReadPin(KEYPAD_PORT, rowPins[3]) == GPIO_PIN_SET) {}
            KEYPAD_AllColumnsLow();
            return 'D';
        }
    }
}
