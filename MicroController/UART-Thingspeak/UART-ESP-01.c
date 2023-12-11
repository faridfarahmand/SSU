// Title: UART_ESP-01
// Purpose: In this project we used use the UART connection of the PIC18F46k42
// to send AT commands to a ESP-01 which connects to a ThingSpeak server via WIFI.
// Compiler: MPLAB X IDE XC8 v6.05
// Author: Oscar Avendano
// Input: None
// Output: Tx RC7,Rx RC6
// PIC18F46K42 Settings % Definitions 

#include "mcc_generated_files/system/system.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <xc.h> 
#define _XTAL_FREQ 4000000      // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4     // system clock is 1 usec                                                        


// Used to write a char to terminal.
void UART1_Write_Char(char txData) 
{
    while (U1TXIF == 0); // Wait until transmit buffer is empty
    U1TXB = txData;
}

// Used to write a string to terminal.
void UART1_Write_String(char txData[]) 
{
    // Keep adding to txData until the full string is sent.
    for (int i = 0; i < strlen(txData); i++) 
    {    
        UART1_Write_Char(txData[i]);
    }

}

// Used to connect to a network.
void esp8266_initialize()
{
    
    // Set ESP8266 to Station mode
    UART1_Write_String("AT+CWMODE=1\r\n");
    __delay_ms(1000);

    // Replace with your Wi-Fi credentials
    UART1_Write_String("AT+CWJAP=\"Oscar iPhone\",\"FreeWifi\"\r\n");
    __delay_ms(8000); // Adjust the delay as needed
    
}


// Sends AT command and expects to receive a OK.
bool esp8266_status()
{
    //** Sending Characters to UART **//    
    char uart_rd[2];
    char data[50];
    
    // Send AT command and wait until the ESP sends back "OK"
    UART1_Write_String("AT\r\n");
    
        // Clear the uart_rd array.
        strcpy(uart_rd, "");
        strcpy(data, "");
    
        // Wait for first char input to be received.
        while(!UART1_IsRxReady());
        
        uart_rd[0] = UART1_Read();
        
        // Wait for second char input to be received.
        while(!UART1_IsRxReady());
        
        uart_rd[1] = UART1_Read();

        // Checks to see if ESP received "AT" command and 
        // responded with "OK"
        if(strcmp(uart_rd, "OK") == 0)
        {

            UART1_Write_String("ESP connection successful!\r\n");
            __delay_ms(500);
            
            return true;
        }
        else
        {
   
            UART1_Write_String("ESP connection failed!\r\n");
            __delay_ms(500);
            
            return false;
        }
        
    
}

// Send data to ThingSpeak using ESP-01 module
void SendDataToThingSpeak(int Value) 
{
    // Declare variables
    char command[100];
    char command1[100];

    // Construct the HTTP GET request with your API key and data
    sprintf(command,"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    UART1_Write_String(command);
    __delay_ms(2000);
    
    // Append Value passed in argument to command1 to get correct bytes to send.
    sprintf(command1, "GET /update?api_key=WQ55SIPE56XX5EE1&field1=%d\r\n", Value);
    
    // Sends the number of bytes needed.
    sprintf(command, "AT+CIPSEND=%d\r\n", (int)strlen(command1));
    UART1_Write_String(command);
    __delay_ms(1000);

    // Sends a GET command to update ThingSpeak.
    UART1_Write_String(command1);
    __delay_ms(1000);

    // Closes the connection to ThingSpeak.
    UART1_Write_String("AT+CIPCLOSE\r\n");
}


int main(void)
{
  

    // Initializes the PIC for the necessary settings needed for proper
    // UART to ESP communication.
    
    SYSTEM_Initialize();
    esp8266_initialize();
    
    // Declare Variables
    int data = 0;

    // Checks to see if a connection between PIC and ESP is etablished.
    if (esp8266_status() == true)
    {
        // Once established we can send data to ThingSpeak.
        // This can now be used for any form of data collection.
        // For example I am sending data points to create an exponential curve. 
        for (int i = 0; i < 5; i++)
        {
            // y = x^2
            data = i * i;
            
            SendDataToThingSpeak(data);
            
        }
        
    }

    
    return 0;
}
