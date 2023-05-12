/* USE THIS CODE WITH USART_MCC.zip 
 * In this project we used MCC to configure UART1 module. 
 * The main.c include several simple calls in which we can send characters or
 * receive characters. 
 *  
 *
 * Author: Farid Farahmand
 */


// PIC18F46K42 Configuration Bit Settings


#include "mcc_generated_files/system/system.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <xc.h> 

int main(void)
{
char uart_rd;
time_t toc;
time(&toc);
srand((int) toc);
char data[50];
char data_r[50];
char myChar;
char txChar;
float voltage=2.0;
uint8_t count = 0;
   
    SYSTEM_Initialize();

    while (1) {
        // generate a random number with one decimal place
////        voltage = (float)rand()/1000.0;
////        sprintf(data, "%.1f", voltage);
////        strcat(data," V  \r\n");	/*Concatenate result and unit to print*/
////        for (uint8_t i = 0; i < strlen(data); i++) { // print one char at a time
////            UART1_Write(data[i]);
////        }
        
        // Generate 1-99 and make sure all the number spit out
        count++;
        sprintf(data, "Th count value is %u\r\n", count);
        if (count < 100) {
            for (uint8_t i = 0; i < strlen(data); i++) { // print one char at a time
                UART1_Write(data[i]);
            }
        }else {
            count = 0;
        }
        __delay_ms(500);

       // Send a character, receive it and then display - Use No-Line Ending        
////        if (UART1_IsRxReady()) { // If data is received,
////
////            uart_rd = UART1_Read(); // read the received data,
////            sprintf(data_r, "You Entered %c\r\n", uart_rd);
////
////                for (uint8_t i = 0; i < strlen(data_r); i++) { // print one char at a time
////                    UART1_Write(data_r[i]);
////                }
////
////           // UART1_Write(uart_rd); // and send data via UART
////           // UART1_Write(0XD); //NEW LINE
////        }
           
        
        
    } // while loop   
}
