/* Title; Joystick sensor.C
 * Purpose: Interface with Joystick sensor
 * Compiler: MPLAB X IDE v6.05
 * Author: Brian Gomez Jimenez
 * Input; joystick_SW, ADC_ANA0, ADC_ANA01
 * Output: RC0-RC4,LED correlate to direction of joystick(up,down,left right)
 * Final Created on July 14, 2023: 7:02 PM
 */
//**Definitions**//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xc.h>
#include "ConfigFile.h"
#define _XTAL_FREQ 4000000      // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4     // system clock is 1 usec   
#define  joystick_ref 1024      //resolution
int digital;
uint16_t  joy_X;
uint16_t  joy_Y;
//** Calling functions
void ADC_Initialize_(void);
int ADC_readValue(int);
//** MAIN **//
void main(void) {
    int x;
    int y; 
    ADC_Initialize_();
    //** Initialize ports**//  
    PORTC = 0b00000000;
    LATC = 0b00000000;
    ANSELC = 0b00000000;
    TRISC = 0b00000000;
    //**loop**//
    while (1) {
        // read AN0 and AN1
        x= ADC_readValue(0);
        y = ADC_readValue(1);
        // read ADC and adjust resolution
        joy_X = x * (joystick_ref / 4096.0);//resolution formula, 0-1024 in x - direction
        joy_Y = y  * (joystick_ref / 4096.0);//resolution formula, 0-1024 in y- direction
     
        //** X-Direction **//
        if (joy_X > 800) {
            PORTCbits.RC0 = 1; // Turn on RC0 (joystick is to the left)

        }
        if (joy_X < 100){
           PORTCbits.RC1 = 1; // Turn on RC1 (joystick is to the right)

        }
        // Check if joy_X is between 800 and 100 one less , when joystick is in middle
        if (joy_X >= 99 && joy_X <= 799) {
            // Turn off RC0 and RC1
            PORTCbits.RC0 = 0; // Turn off RC0
            PORTCbits.RC1 = 0; // Turn off RC1
        }
        //** Y-Direction **//
        if (joy_Y > 800) {
            PORTCbits.RC3 = 1; // Turn on RC3 (joystick is up )
        }
        if (joy_Y < 100){
           PORTCbits.RC2 = 1; // Turn on RC2 (joystick is down)           
        }
        // Check if joy_X is between 800 and 100 one less 
        if (joy_Y >= 99 && joy_Y <= 799) {
            // Turn off RC0 and RC1
            PORTCbits.RC2 = 0; // Turn off RC2
            PORTCbits.RC3 = 0; // Turn off RC3
        }
        //** Joystick button(if pressed = all lights light up) not implemented!!!
//        if (joystick_SW == 1){
//            PORTC = 0b00001111;
//            __delay_ms(500);
//            PORTC = 0b00000000;        
}
}

void ADC_Initialize_(void)//Initializes ADC. PAGE #619 in Data sheet for 
//PIC18F46k42
{
       //Setup ADC
    ADCON0bits.FM = 1;  //right justify
    ADCON0bits.CS = 1; //ADCRC Clock
    TRISAbits.TRISA0 = 1; //Set RA0 to input
    ANSELAbits.ANSELA0 = 1; //Set RA0 to input
    TRISAbits.TRISA1 = 1; //Set RA0 to input
    ANSELAbits.ANSELA1 = 1; //Set RA0 to analog
    ADCON0bits.ON = 1; //Turn ADC On    
}
int ADC_readValue(int channel)// Function that reads multiple ADC channels at once. 
{
    int digital;
    ADCON0bits.GO = 1;  // Start A/D conversion
    // Set the channel in the ADPCH register
    ADPCH = channel;
    ADCON0bits.GO = 1;  // Start A/D conversion
    while (ADCON0bits.GO);  // Wait for ADC conversion to complete
    digital = (ADRESH * 256) + ADRESL; // Combine 8-bit MSB and 8-bit LSB
    return digital;
}
/*Insert in main when reading ADC from CoolTerm
 * ADCON0bits.GO = 1; // Start ADC conversion
        while (ADCON0bits.GO); // Wait for ADC conversion to complete
        digital = (ADRESH * 256) + ADRESL;

        // Calculate the voltage value based on ADC result
        joy_X = digital * (Vref / 4096.0);

        // Format the voltage value as a string
        sprintf(data_r, "Joystick Value: %d\r\n", joy_X);

        //sprintf(data_r, "%d ", joy_X);
        for (uint8_t i = 0; i < strlen(data_r); i++) {
            UART1_Write(data_r[i])
 */


