
The purpose of this file s to show how to have different header files, each containing specific parts of the code. 
In this case we demosntrate how to separate functions, from config bits, and definitions. 

**configheader.h**
// all config files …..

** Initheader.h **
#include <xc.h> // include processor files - each processor file is guarded.  
void initFunc ( void ){
    //all register setup
}

** fucntionheader.h **
#define _XTAL_FREQ 4000000       // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
void xxxx(char buzzer);                                  
void yyyy (char disNum);   /

void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{ // define all the functions 
}


** main.c **
#include <xc.h> 
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
#include <stdio.h>
#include <stdlib.h>
#include "./ConfigHeader.h"
#include “./vHeader.h"
#include "./nHeader.h"

#define _XTAL_FREQ 4000000       // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4


// the rest of the program 
