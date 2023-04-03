/*
 * ---------------------
 * Title: Simple LED Blinking 
 * ---------------------
 * Program Details:
 *  The purpose of this program is to simply blink an LED 
 * Inputs: myDelay 
 * Outputs: RD0 (output)
 * Setup: C- Simulator
 * Date: Feb 24, 2023
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.4
 * Author: Farid Farahmand
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */

#include <xc.h>
#include "ConfigFile.h"
#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
#include <stdio.h>
#include <stdlib.h>


#define _XTAL_FREQ 4000000            // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

// --------------------------
// Function Declaration
// -------------------------- 

// --------------------------
// Variables 
// -------------------------- 

// --------------------------
// MAIN Function 
// -------------------------- 
void main(void){
     asm("BANKSEL	PORTD") ;
     asm("CLRF	PORTD") ;       //Initialize
     asm("BANKSEL	LATD") ;    //Data Latch
     asm("CLRF	LATD") ;
     asm("BANKSEL	ANSELD") ;
     asm("CLRF	ANSELD") ;      //digital I/O
     asm("BANKSEL	TRISD") ;
     asm("MOVLW	0b00000000") ;
     asm("MOVWF	TRISD") ;

     while(1) // run forever
     {
        PORTDbits.RD1 = 0; //turns on LED
        __delay_ms(500); //delays for 1 second
        //PORTD=0b00000011;//turns off LED
        PORTDbits.RD1 = 1; //turns on LED    
        __delay_ms(500); //delays for 1 second
    }
}
