#include "gpio.h"
#include "keypad.h"
#include "sevenseg.h"

#define CODE_LENGTH 4U

static const char correctCode[CODE_LENGTH] = { 'A', '5', '2', 'D' };

static void DelayMs(volatile uint32_t ms)
{
    volatile uint32_t i;

    while (ms--)
    {
        /* Rough delay for STM32L476RG running at default clock.
           Adjust 3200 if you want faster/slower timing. */
        for (i = 0U; i < 3200U; i++)
        {
            __NOP();
        }
    }
}

static uint8_t IsCodeCorrect(const char *entered)
{
    uint8_t i;

    for (i = 0U; i < CODE_LENGTH; i++)
    {
        if (entered[i] != correctCode[i])
        {
            return 0U;
        }
    }

    return 1U;
}

static uint8_t IsAllowedCodeKey(char key)
{
    if ((key >= '0') && (key <= '9'))
    {
        return 1U;
    }

    if ((key >= 'A') && (key <= 'D'))
    {
        return 1U;
    }

    return 0U;
}

static void RunCircleAnimation(void)
{
    /* Common-anode raw patterns for one outer segment ON at a time.
       bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g
       In common-anode, 0 = ON and 1 = OFF. */
    static const uint8_t circleFrames[6] =
    {
        0x7EU, /* a */
        0x7DU, /* b */
        0x7BU, /* c */
        0x77U, /* d */
        0x6FU, /* e */
        0x5FU  /* f */
    };

    uint8_t loop;
    uint8_t frame;

    for (loop = 0U; loop < 4U; loop++)
    {
        for (frame = 0U; frame < 6U; frame++)
        {
            SevenSeg_DisplayRaw(circleFrames[frame]);
            DelayMs(50U);
        }
    }

    SevenSeg_Clear();
}

int main(void)
{
    char key;
    char enteredCode[CODE_LENGTH];
    uint8_t enteredCount = 0U;

    KEYPAD_Init();
    SevenSeg_Init();
    SevenSeg_Clear();

    while (1)
    {
        key = KEYPAD_GetKey();

        if (key == '\0')
        {
            continue;
        }

        /* Always show the pressed key first */
        SevenSeg_DisplayChar(key);

        if (key == '#')
        {
            enteredCount = 0U;
            SevenSeg_Clear();
            continue;
        }

        if (key == '*')
        {
            if ((enteredCount == CODE_LENGTH) && (IsCodeCorrect(enteredCode) != 0U))
            {
                RunCircleAnimation();
            }
            else
            {
                /* Wrong code -> show dash briefly */
                SevenSeg_DisplayChar('*');
                DelayMs(300U);
                SevenSeg_Clear();
            }

            enteredCount = 0U;
            continue;
        }

        if ((IsAllowedCodeKey(key) != 0U) && (enteredCount < CODE_LENGTH))
        {
            enteredCode[enteredCount] = key;
            enteredCount++;
        }
    }
}
