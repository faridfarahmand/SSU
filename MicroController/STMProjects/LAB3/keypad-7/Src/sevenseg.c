#include "sevenseg.h"

/*
 * Common-anode, exact GPIOB ODR patterns for PB0..PB6.
 * bit0 -> a, bit1 -> b, bit2 -> c, bit3 -> d,
 * bit4 -> e, bit5 -> f, bit6 -> g
 * 0 = segment ON, 1 = segment OFF
 */
static const uint8_t sevenSegDigits[10] =
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
    0x10U  /* 9 */
};

void SevenSeg_Init(void)
{
    GPIO_InitPin(SEVENSEG_PORT, SEVENSEG_SEG_A_PIN, GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(SEVENSEG_PORT, SEVENSEG_SEG_B_PIN, GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(SEVENSEG_PORT, SEVENSEG_SEG_C_PIN, GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(SEVENSEG_PORT, SEVENSEG_SEG_D_PIN, GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(SEVENSEG_PORT, SEVENSEG_SEG_E_PIN, GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(SEVENSEG_PORT, SEVENSEG_SEG_F_PIN, GPIO_MODE_OUTPUT, GPIO_NO_PULL);
    GPIO_InitPin(SEVENSEG_PORT, SEVENSEG_SEG_G_PIN, GPIO_MODE_OUTPUT, GPIO_NO_PULL);

    SevenSeg_Clear();
}

void SevenSeg_Clear(void)
{
    SevenSeg_DisplayRaw(0x7FU);
}

void SevenSeg_DisplayRaw(uint8_t pattern)
{
    uint32_t odr;

    odr = SEVENSEG_PORT->ODR;
    odr &= ~0x7FU;
    odr |= (pattern & 0x7FU);
    SEVENSEG_PORT->ODR = odr;
}

void SevenSeg_DisplayDigit(uint8_t digit)
{
    if (digit <= 9U)
    {
        SevenSeg_DisplayRaw(sevenSegDigits[digit]);
    }
    else
    {
        SevenSeg_Clear();
    }
}

void SevenSeg_DisplayChar(char c)
{
    switch (c)
    {
        case '0': SevenSeg_DisplayRaw(0x40U); break;
        case '1': SevenSeg_DisplayRaw(0x79U); break;
        case '2': SevenSeg_DisplayRaw(0x24U); break;
        case '3': SevenSeg_DisplayRaw(0x30U); break;
        case '4': SevenSeg_DisplayRaw(0x19U); break;
        case '5': SevenSeg_DisplayRaw(0x12U); break;
        case '6': SevenSeg_DisplayRaw(0x02U); break;
        case '7': SevenSeg_DisplayRaw(0x78U); break;
        case '8': SevenSeg_DisplayRaw(0x00U); break;
        case '9': SevenSeg_DisplayRaw(0x10U); break;

        case 'A': SevenSeg_DisplayRaw(0x08U); break; /* A */
        case 'B': SevenSeg_DisplayRaw(0x03U); break; /* b */
        case 'C': SevenSeg_DisplayRaw(0x46U); break; /* C */
        case 'D': SevenSeg_DisplayRaw(0x21U); break; /* d */
        case '*': SevenSeg_DisplayRaw(0x3FU); break; /* - */
        case '#': SevenSeg_DisplayRaw(0x77U); break; /* _ */

        default:  SevenSeg_Clear(); break;
    }
}
