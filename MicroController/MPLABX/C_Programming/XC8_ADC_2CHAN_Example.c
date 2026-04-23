/*
 * The purpose of this program is to demonstrate the use of the ADC module
 * on the PIC18F46K42 microcontroller.
 *
 * Two analog input channels (e.g., from an accelerometer) are sampled using
 * AN0 (RA0) and AN1 (RA1). The ADC converts these analog signals into digital
 * values, which are then scaled to corresponding voltage levels based on a
 * 3.3V reference.
 *
 * The results can be displayed (e.g., on an LCD or serial output) and verified
 * through simulation or hardware testing.
 *
 * Author: Farid Farahmand
 */


// PIC18F46K42 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config FEXTOSC = LP     // External Oscillator Selection (LP (crystal oscillator) optimized for 32.768 kHz; PFM set to low power)
#pragma config RSTOSC = EXTOSC  // Reset Oscillator Selection (EXTOSC operating per FEXTOSC bits (device manufacturing default))

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write-protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

#include <xc.h> // must have this
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _XTAL_FREQ 4000000
#define FCY (_XTAL_FREQ/4)

#define Vref 3.3

unsigned int digital_ch0;
unsigned int digital_ch1;

float voltage_ch0;
float voltage_ch1;

char data1[20];
char data2[20];

void ADC_Init(void);
unsigned int ADC_Read(unsigned char channel);

/*
   Two-channel ADC example for PIC18F46K42
   Assumptions:
   - Accelerometer Channel X -> RA0 / AN0
   - Accelerometer Channel Y -> RA1 / AN1
*/

void main(void)
{
    ADC_Init();

    while (1)
    {
        // Read Channel AN0
        digital_ch0 = ADC_Read(0x00);
        voltage_ch0 = digital_ch0 * ((float)Vref / 4096.0);

        // Read Channel AN1
        digital_ch1 = ADC_Read(0x01);
        voltage_ch1 = digital_ch1 * ((float)Vref / 4096.0);

        // Convert to strings if needed for LCD / serial display
        sprintf(data1, "CH0=%.2f V", voltage_ch0);
        sprintf(data2, "CH1=%.2f V", voltage_ch1);

        // Example:
        // LCD_String_xy(1,1,data1);
        // LCD_String_xy(2,1,data2);

        __delay_ms(100);
    }
}

void ADC_Init(void)
{
    // Result format and clock
    ADCON0bits.FM = 1;      // Right justified result
    ADCON0bits.CS = 1;      // ADCRC clock

    // Set pins as inputs
    TRISAbits.TRISA0 = 1;   // RA0 input
    TRISAbits.TRISA1 = 1;   // RA1 input

    // Set pins as analog
    ANSELAbits.ANSELA0 = 1; // RA0 analog
    ANSELAbits.ANSELA1 = 1; // RA1 analog

    // Default channel = AN0
    ADPCH = 0x00;

    // ADC timing registers
    ADCLK  = 0x00;
    ADPREL = 0x00;
    ADPREH = 0x00;
    ADACQL = 0x00;
    ADACQH = 0x00;

    // Clear result registers
    ADRESH = 0x00;
    ADRESL = 0x00;

    // Turn ADC on
    ADCON0bits.ON = 1;
}

unsigned int ADC_Read(unsigned char channel)
{
    ADPCH = channel;          // Select ADC input channel
    __delay_us(10);           // Small settling time after channel switch

    ADCON0bits.GO = 1;        // Start conversion
    while (ADCON0bits.GO);    // Wait until conversion is complete

    return (((unsigned int)ADRESH << 8) | ADRESL);
}
