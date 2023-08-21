/* Title: Servo.c
 * Purpose: PWM setup for one servo
 * Created on July 16, 2023, 10:49 AM
 * Compiler: MPLAB X IDE  XC8 v6.05 PICKIT 4 
 * Author: Brian Gomez Jimenez
 * Input; N/A
 * Output: RC1
 * Final Created on July 25, 2023, 5:20 PM
 */
#include <xc.h>
#include "Pulse.h"
#define _XTAL_FREQ 4000000      // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4     // system clock is 1 usec
//** Calling functions (PWM)
void TMR2_Initialize(void); //Initializing Timer 2
void TMR2_Start(void);//Start Timer 2 
void PWM_Output_Pin_Enable (void);//Initialize Output pins
void PWM2_Initialize(void);//Initialize PWM
void PWM2_LoadDutyValue(uint16_t dutyValue);//Load DutyValue
//**MAIN **//
void main(void) {
    //Initialize PORTC
    ANSELC = 0b00000000;
    TRISC = 0b00000000; 
    PORTC = 0b00000000; 
    //Loading functions
    TMR2_Initialize();
    TMR2_Start();        
    PWM_Output_Pin_Enable();
    PWM2_Initialize();
    PWM2_LoadDutyValue(16);
    //**BEGIN LOOP**//
    while (1) {
        //Sweep pulse width from 500us to 2500us (going forward)
        for(int i = 16; i<78; i++){
            PWM2_LoadDutyValue(i);
            __delay_ms(25);
        }
        //Sweep pulse width from 2500us to 500us (going backwards)
        for(int i = 78; i>16; i--){
            PWM2_LoadDutyValue(i);
            __delay_ms(25);  
        }

    }
}
