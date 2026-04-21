/*
 * Demonstration of Watchdog Timer and Sleep mode on PIC18F46K42
 *
 * Behavior:
 *  - RD0 blinks every 100 ms during active mode
 *  - CLRWDT() is used during normal operation
 *  - After a few blink cycles, RD1 turns on and the MCU enters Sleep mode
 *  - The Watchdog Timer times out during Sleep and wakes the MCU
 *  - Execution resumes after SLEEP(), RD1 turns off, and blinking starts again
 *
 * This version replaces Dummy_Stuck() with Sleep so you can measure
 * the power difference between active mode and sleep mode.
 *
 * Author: Farid Farahmand
 */

// PIC18F46K42 Configuration Bit Settings

// CONFIG1L
#pragma config FEXTOSC = LP
#pragma config RSTOSC  = EXTOSC

// CONFIG1H
#pragma config CLKOUTEN = OFF
#pragma config PR1WAY   = ON
#pragma config CSWEN    = ON
#pragma config FCMEN    = ON

// CONFIG2L
#pragma config MCLRE    = EXTMCLR
#pragma config PWRTS    = PWRT_OFF
#pragma config MVECEN   = ON
#pragma config IVT1WAY  = ON
#pragma config LPBOREN  = OFF
#pragma config BOREN    = SBORDIS

// CONFIG2H
#pragma config BORV     = VBOR_2P45
#pragma config ZCD      = OFF
#pragma config PPS1WAY  = ON
#pragma config STVREN   = ON
#pragma config DEBUG    = OFF
#pragma config XINST    = OFF

// CONFIG3L ******* FOR WATCHDOG ********
#pragma config WDTCPS   = WDTCPS_31   // Divider ratio 1:65536; software control of WDTPS
#pragma config WDTE     = ON          // WDT enabled; SWDTEN ignored

// CONFIG3H
#pragma config WDTCWS   = WDTCWS_7    // Window always open
#pragma config WDTCCS   = SC          // Software Control clock source (31 kHz)

// CONFIG4L
#pragma config BBSIZE   = BBSIZE_512
#pragma config BBEN     = OFF
#pragma config SAFEN    = OFF
#pragma config WRTAPP   = OFF

// CONFIG4H
#pragma config WRTB     = OFF
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config WRTSAF   = OFF
#pragma config LVP      = ON

// CONFIG5L
#pragma config CP       = OFF

#include <xc.h>

#define _XTAL_FREQ 4000000UL

#define OUTPUT 0

#define BlinkLED_LAT   LATDbits.LATD0
#define WdtLED_LAT     LATDbits.LATD1
#define PORTD_DIR      TRISD

unsigned int count;

void Enter_Sleep_Mode(void);

void main(void)
{
    // Initialization
    ANSELD = 0x00;          // PORTD digital
    PORTD_DIR = OUTPUT;     // All PORTD pins as outputs
    LATD = 0x00;            // Initialize PORTD outputs low

    // Optional WDT register setting from your original code
    // Keep only if this matches your intended WDT postscaler setup
    WDTCON0 = 0b00011011;

    __delay_ms(1000);

    count = 0;
    BlinkLED_LAT = 0;
    WdtLED_LAT = 0;

    while(1)
    {
        BlinkLED_LAT = !BlinkLED_LAT;   // Toggle LED on RD0
        __delay_ms(100);

        CLRWDT();                       // Clear Watchdog Timer during active mode
        count++;

        if(count > 5)
        {
            WdtLED_LAT = 1;             // Indicate entry to low-power state
            Enter_Sleep_Mode();         // Enter Sleep instead of getting stuck

            // Code resumes here after WDT wake-up
            WdtLED_LAT = 0;
            count = 0;
        }
    }
}

void Enter_Sleep_Mode(void)
{
    BlinkLED_LAT = 0;   // Optional: turn off blink LED before sleeping
    CLRWDT();           // Good practice before entering Sleep

    SLEEP();            // Enter low-power Sleep mode
    NOP();              // Recommended instruction after SLEEP
}
