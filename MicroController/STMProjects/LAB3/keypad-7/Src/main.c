#include "keypad.h"
#include "sevenseg.h"
#include <stdint.h>

/*
 * Common-anode raw patterns for PB0..PB6
 * bit0=a, bit1=b, ..., bit6=g
 */
static void DisplayHex(uint8_t value)
{
    static const uint8_t hexPattern[16] =
    {
        0x40U, /* 0 */
        0x79U, /* 1 */
        0x24U, /* 2 */
        0x30U, /* 3 */
        0x19U, /* 4 */
        0x12U, /* 5 */
        0x02U, /* 6 */
        0x78U, /* 7 */
        0x00U, /* 8 */
        0x10U, /* 9 */
        0x08U, /* A */
        0x03U, /* b */
        0x46U, /* C */
        0x21U, /* d */
        0x06U, /* E */
        0x0EU  /* F */
    };

    if (value < 16U)
    {
        SevenSeg_DisplayRaw(hexPattern[value]);
    }
}

int main(void)
{
    uint8_t idx;

    KEYPAD_Init();
    SevenSeg_Init();
    SevenSeg_Clear();

    while (1)
    {
        idx = KEYPAD_GetIndex();

        if (idx != 0xFFU)
        {
            DisplayHex(idx);
        }
    }
}
