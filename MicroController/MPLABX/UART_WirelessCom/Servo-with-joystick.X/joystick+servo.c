/* Title: joystick+servo.c
 * Purpose: Interfacing two joystick controlling four servos
 * Created on July 16, 2023, 10:49 AM
 * Compiler: MPLAB X IDE  XC8 v6.05 PICKIT 4 
 * Author: Brian Gomez Jimenez
 * Input; ADC_ANA0, ADC_ANA01,ADC_ANA02,ADC_ANA03
 * Output: RB0,RB1,RC1,RC2; Pins to servos. 
 * Final Created on July 25, 2023, 5:20 PM
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xc.h>
#include "ConfigFile.h"
#include "Pulse.h"
#define _XTAL_FREQ 4000000      // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4     // system clock is 1 usec   

int digital;
uint16_t  joy_X;
uint16_t  joy_Y;
uint16_t  black_pot;
uint16_t gold_pot; 
//** Calling functions (ADC)
void ADC_Initialize_(void);
int ADC_readValue(int);
//** Calling functions (PWM)
void TMR2_Initialize(void); //first servo
void TMR2_Initialize_2(void); //second servo
void TMR2_Initialize_6(void);//third & fourth servo share timer 6. 
//** Start timers TMRx
void TMR2_Start(void);// timer_2
void TMR4_Start_2(void);//timer_4
void TMR4_Start_6(void);//timer_6
/****OUTPUT PINS PWM */
void PWM_Output_Pin_Enable (void);
//servo 1 PWM
void PWM2_Initialize(void);
void PWM2_LoadDutyValue(uint16_t dutyValue);
//servo 2 PWM
void PWM2_Initialize_2(void);
void PWM2_LoadDutyValue_2(uint16_t dutyValue);
//servo 3 PWM
void PWM2_Initialize_3(void);
void PWM2_LoadDutyValue_3(uint16_t dutyValue);
//servo 4 PWM
void PWM2_Initialize_4(void);
void PWM2_LoadDutyValue_4(uint16_t dutyValue); 
//****MAIN****//
void main(void) {
    int x;// x input on joystick #1
    int y; // y input on joystick #1
    int p_1; //x input  on joystick #2
    int p_2; //y input on joystick #2
    //**Calling functions**//
    ADC_Initialize_();
    TMR2_Initialize();
    TMR2_Initialize_2();
    TMR2_Initialize_6();
    TMR2_Start(); 
    TMR4_Start_2();
    TMR4_Start_6();
    PWM_Output_Pin_Enable();  
    PWM2_Initialize();
    PWM2_LoadDutyValue(16);// pwm 1 sets servo to starting position of 0 degrees
    //this is calculated using equation 24-2 on Page 356 and a ADEEPT servo
    //with a pulse width from 500us(0 degrees) to 2500us(180 degrees)
    PWM2_Initialize_2();  //pwm 2  
    PWM2_LoadDutyValue_2(16);
    //pwm 3 
    PWM2_Initialize_3();
    PWM2_LoadDutyValue_3(16);
    //pwm 4
    PWM2_Initialize_4();
    PWM2_LoadDutyValue_4(16);
    //** Initialize Servo Output C1,C2. See 'PWM_Output_Pin_Enable ();' for pins
    PORTC = 0b00000000;
    ANSELC = 0b00000000;
    TRISC = 0b00000000;
    //** Initialize Servo Output B0,B1. See 'PWM_Output_Pin_Enable ();' for pins
    ANSELB = 0b00000000;
    TRISB = 0b00000000; 
    PORTB = 0b00000000;

    //**loop**//
    while (1) {
        //see read_ADC() and Initialize_ADC() in header. 
        x= ADC_readValue(0);//ANA0
        y = ADC_readValue(1)//ANA1
        p_1 = ADC_readValue(2);//ANA2
        p_2 = ADC_readValue(3);//ANA3
        // configuring the range, right now set at 16-78,can confirm using serial terminal 
        joy_X = ((float)((x / 4096.0) * 62.0) + 16.0);
        joy_Y = ((float)((y / 4096.0) * 62.0) + 16.0);
        black_pot = ((float)((p_1 / 4096.0) * 62.0) + 16.0);
        gold_pot = ((float)((p_2 / 4096.0) * 62.0) + 16.0);
        // read servos pwm according to adc
        PWM2_LoadDutyValue(joy_X);//RC1
        PWM2_LoadDutyValue_2(joy_Y);//RC2
        PWM2_LoadDutyValue_3(black_pot);//RB0
        PWM2_LoadDutyValue_4(gold_pot); //RB1
}
}