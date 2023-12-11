// Title: UART_MCC
// Purpose: In this project we used MCC to configure UART1 module. 
// The main.c include several simple calls in which we can send characters or
// receive characters. 
// Compiler: MPLAB X IDE XC8 v6.05
// Author: Oscar Avendano
// Input: Characters: t,f
// Output: Tx RC7,Rx RC6, RD0,RD1
// PIC18F46K42 Settings % Definitions 

#include "mcc_generated_files/system/system.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <xc.h> 
#define LED0  PORTDbits.RD0
#define LED1  PORTDbits.RD1
#define _XTAL_FREQ 4000000      // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4     // system clock is 1 usec                                                        

///***INITIALIZE***///
int main(void){

    
//** Sending Characters to UART **//    
const char* both = "t";
const char* off = "f";
char endline;
char uart_rd;
char data[50];

    //**Calling Functions**//
    SYSTEM_Initialize();
    
    //**Initializing PORTS
    PORTD = 0b00000000;
    TRISD = 0b0000000;
    LATD = 0b0000000;
    ANSELD = 0b00000000;


        // This loop receives data from a terminal and executes commands according to the input.
        // i.e input: "t", output: both LEDs turn on.
        // i.e input: "f", output: both LEDs turn off.
        while (1) 
        {
            
            // Executes once data is received.
            if (UART1_IsRxReady()) 
            {
                // Reads the received data.
                // Also stores the "\n" in the endline char.
                // NOTE - no need for the endline variable if using no-line ending!
                uart_rd = UART1_Read();
                endline = UART1_Read();
                
                // Stores the character read with the following format.
                sprintf(data, "You Entered %c \r\n", uart_rd);
             
                // Prints the character that was received.
                for (uint8_t i = 0; i < strlen(data); i++) 
                {   
                    UART1_Write(data[i]);
                }
                
                // Once data is received, interpret it and proceed accordingly.
                // If character received is 't' turn on both LED0 & LED1.
                if (strcmp(&uart_rd, both) == 0) 
                {
                    // Turn LED0 and LED1 on.
                    PORTD = 0b00000011;

                    // Prints message letting user know LEDs are on. 
                    sprintf(data, "BOTH are ON\r\n");

                    for (uint8_t i = 0; i < strlen(data); i++) 
                    {
                        UART1_Write(data[i]);
                    }
                    
                    __delay_ms(500);
                }
                
                // If character received is 'f' turn off both LED0 & LED1.
                if (strcmp(&uart_rd, off) == 0) 
                {
                    // Turn off LED0 and LED1.
                    PORTD = 0b00000000;
            
                    // Prints message letting user know LEDs are off. 
                    sprintf(data, "Everything OFF \r\n");
            
                    for (uint8_t i = 0; i < strlen(data); i++) 
                    {
                        UART1_Write(data[i]);
                    }
                    
                    __delay_ms(500);
                } 
                
             
            }
            else
            {
                
                // Prints message letting user know what to enter. 
                sprintf(data, "Please enter 't' or 'f'. \n");

                for (uint8_t i = 0; i < strlen(data); i++) 
                {
                    UART1_Write(data[i]);
                }
                
                // Only continue once a char is received.
                while(!UART1_IsRxReady());

            }
        }
        
   
    
    return 0;
}
