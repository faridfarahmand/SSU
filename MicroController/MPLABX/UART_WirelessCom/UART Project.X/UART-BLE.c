/* Title: USART_MCC
 * Purpose:  * In this project we used MCC to configure UART1 module. 
 * The main.c include several simple calls in which we can send characters or
 * receive characters. 
 * Compiler: MPLAB X IDE XC8 v6.05
 * Author: Brian Gomez Jimenez
 * Input: Characters: o,b,t,f
 * Output: Tx RC7,Rx RC6, RD0,RD1
//**PIC18F46K42 Settings % Definitions**/
#include "mcc_generated_files/system/system.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <xc.h> 
#define LED  PORTDbits.RD0
#define _XTAL_FREQ 4000000      // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4     // system clock is 1 usec                                                        

///***INITIALIZE***///
int main(void){
    //** Sending Characters to UART **//    
const char* on = "o";
const char* both = "b";
const char* on_two = "t";
const char* off = "f";
char my_name[]= "Brian Gomez Jimenez \r\n";//
char uart_rd;
char data_r[50];
    //**Calling Functions**//
    SYSTEM_Initialize();
    //**Initializing PORTS
    PORTD = 0b00000000;
    TRISD = 0b0000000;
    LATD = 0b0000000;
    ANSELD = 0b00000000;

    //** UNCOMMENT TO SEND "Brian Gomez Jimenez" THROUGH TERMINAL**//
    //WRITE TO TERMINAL ONLY!!!!!!!!!
//    while(1) {
//        
//            for (uint8_t i = 0; i < strlen(my_name); i++) {
//                UART1_Write(my_name[i]);
//                __delay_ms(100); 
//            }
//                
//            }
while (1) {// READ AND WRITE TO TERMINAL!!!!!!
        // Send a character, receive it and then display - Use No-Line Ending 
        if (UART1_IsRxReady()) {// If data is received,
            uart_rd = UART1_Read();// read the received data,
            
            sprintf(data_r, "You Entered %c \r\n", uart_rd);//PRINT WHAT CHARACTER YOU SENT
            //for (uint8_t i = 0; i < strlen(data_r); i++) {
                //UART1_Write(data_r[i]);
            }
            
            if (strcmp(&uart_rd, on) == 0) {// IF CHARACTER IS 'o' turn on RD0
                LED = 1;
                __delay_ms(1000);
                sprintf(data_r, "RD0 is on \r\n");//DISPLAY ON TERMINAL
                for (uint8_t i = 0; i < strlen(data_r); i++) {
                    UART1_Write(data_r[i]);
                }
            }
            
                if (strcmp(&uart_rd, on_two) == 0) {//IF CHARACTER IS 't' turn on RD1
                    PORTD = 0b00000010;
                    __delay_ms(1000);
                    sprintf(data_r, "RD1 is ON \r\n");//DISPLAY ON TERMINAL
                    for (uint8_t i = 0; i < strlen(data_r); i++) {
                        UART1_Write(data_r[i]);
                    }
            }
            
                    if (strcmp(&uart_rd, both) == 0) {//IF CHARACTER IS 'b' turn both RD0 & RD1
                        PORTD = 0b00000011;
                        __delay_ms(1000);
                        sprintf(data_r, "BOTH are ON\r\n");//DISPLAY ON TERMINAL 
                        for (uint8_t i = 0; i < strlen(data_r); i++) {
                            UART1_Write(data_r[i]);
                        }
            }
                        if (strcmp(&uart_rd, off) == 0){//IF CHARACTER IS 'f' turn everything off
                            PORTD = 0b00000000;
                            __delay_ms(1000);
                            sprintf(data_r, "Everything OFF \r\n");//DISPLAY ON TERMINAL
                            for (uint8_t i = 0; i < strlen(data_r); i++) {
                                UART1_Write(data_r[i]);
                        }
                        }
        

}
}
