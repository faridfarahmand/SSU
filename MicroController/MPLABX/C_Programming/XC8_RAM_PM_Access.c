/*
 * ---------------------
 * Title: Accessing RAM and FLASH
 * ---------------------
 * Program Details:
 *  The purpose of this program is to write into RAM and FLASH
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
// Variables 
// -------------------------- 
int x;
int myByte;
// --------------------------
// MAIN Function 
// -------------------------- 
//__eeprom char regNumber[10] = "A93213"; // not supported for PIC18F46K42
int variable_1 __at(0x60); // data memory location 0x60 - __at() is the construct 
volatile static unsigned int variable_2 __at(0x70); // write into RAM 

// place in the program memory (PM) - using constant qualifier 
const char seg_code[] __at(0x100) = { 0x3f, 0x06, 0x5b };
const char table[] __at(0x110) = { 0,1,2,3 };  // located in PM
const char myText __at(0x120);

__at(0x600) int main() // the code will start at RM location 0x600
{
	variable_1 = 0xAA;  // place at RAM location 0x60
	variable_2 = 0xBB;  // place at RAM location 0x70

	TRISD = 0;
	while(1) 
	{
   		PORTDbits.RD0 = 1;
	}
}
