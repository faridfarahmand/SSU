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
