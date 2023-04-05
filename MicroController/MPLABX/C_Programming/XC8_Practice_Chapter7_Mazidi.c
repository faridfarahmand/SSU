
//****************************************
// Practicing with examples in Chapter 7
// PIC MICROCONTROLLER AND EMBEDDED SYSTEMS Using Assembly and C for PIC18
// by Muhammad Ali Mazidi - starting page 254 (PDF)
//****************************************
#include <xc.h> // must have this
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "ConfigFile.h"   // you can remove this if you already have the config bits setup in the file 
// file is avaailable: https://github.com/faridfarahmand/SSU/blob/main/MicroController/MPLABX/C_Programming/XC8_ConfigFile.h 

//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
#include "C:/Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h" // you may not need it

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4


//---------------------------
// ****** Example 7-0
// Example of Data types
//---------------------------
/* variable definition: */
//unsigned int a; 
//unsigned int b;
//unsigned int c;
//void main (void) {
//   /* actual initialization */
//   a = 0x2;
//   b = 0x3;
//   c = a + b;
//}

//---------------------------
// ****** Example 7-1
//---------------------------
// void main(void) {
//     TRISD= 0b00000000;//sets PORTB as all outputs
//     PORTD= 0b00000000;//turns off PORTB outputs so that the LED is initially off
//     unsigned char z; 
//
//     for(z=0; z<20; z++)
//         PORTD = z;
//     while(1);  // this makes the program loop here on its own - (1) indicates if TRUE
// }


//---------------------------
// ****** Example 7-3
//---------------------------
// unsigned char outputValue;
// void main(void) {
//     TRISB = 0;  
//     for(;;) // run forever
//     {
//         outputValue = 0;
//         outputValue = 0xAA;   
//     }     
// }

//---------------------------
// ****** Example 7-7
//---------------------------
// void  MSDelay(unsigned int,unsigned int); // function declaration 
// unsigned char outputValue; 
//
// #define SevenSeg PORTB // reminder: also does not show up as a variable
// #define mybit PORTBbits.RB7
//
// void main(void) {
//    

//     asm("BANKSEL	PORTB") ; // This is not a standard approach! Try not to mix assembly and C
//     asm("CLRF	PORTB") ;       //Init PORTB
//     asm("BANKSEL	LATB") ;    //Data Latch
//     asm("CLRF	LATB") ;
//     asm("BANKSEL	ANSELB") ;
//     asm("CLRF	ANSELB") ;      //digital I/O
//     asm("BANKSEL	TRISB") ;
//     asm("MOVLW	0b00000000") ;
//     asm("MOVWF	TRISB") ;
//
//     while(1) // run forever
//     {
//         outputValue = 0x55; 
//         SevenSeg = outputValue; 
//         MSDelay(25,165);
//         SevenSeg++;                 // just increment by one
//         mybit = 1;                  // change bit 7 of PORTB
//         MSDelay(25,165); 
//     }     
// } // end of main()
//
//// // function definition 
// void  MSDelay(unsigned int itime1, unsigned int itime2)
// {
//     unsigned int i; unsigned int j;
//     for (i=0; i<itime1; i++)    // should be able to see itime as a variable
//       for (j=0; j<itime2; j++);      
// }

//---------------------------
// ****** Example 7-13 
//---------------------------
//#define myInput PORTBbits.RB5
//#define myOutput PORTBbits.RB0
//
//void main(void) {
//    
//    asm("BANKSEL	PORTB") ;
//    asm("CLRF	PORTB") ;       //Init PORTB
//    asm("BANKSEL	LATB") ;    //Data Latch
//    asm("CLRF	LATB") ;
//    asm("BANKSEL	ANSELB") ;
//    asm("CLRF	ANSELB") ;      //digital I/O
//    asm("BANKSEL	TRISB") ;
//    //asm("MOVLW	0b00000000") ;
//    //asm("MOVWF	TRISB") ;
//
//    TRISBbits.TRISB5 = 1;   // bit 5 is an input
//    TRISBbits.TRISB0 = 0;   // bit 0 is an output
//
//    while(1) // run forever
//    {
//        if (myInput == 1)
//            myOutput = 1;
//        else 
//            myOutput = 0;
//    }     
//} // end of main()

//---------------------------
// ****** Example 7-x1 
// Writing into PM and RAM
//---------------------------

//int variable_1 __at(0x60); // data memory location 0x60 
//volatile static unsigned int variable_2 __at(0x70); // write into RAM 
//
//// place in the program memory (PM) - using constant qualifier 
//const char seg_code[] __at(0x100) = { 0x3f, 0x06, 0x5b };
//const char table[] __at(0x110) = { 0,1,2,3 }; 
//const char myText __at(0x120);
//
//__at(0x200A0) int main()
//{
//	
//	TRISD= 0b00000000;//sets PORTB as all outputs
//	PORTD= 0b00000000;//turns off PORTB outputs so that the LED is initially off
//	
//	variable_1 = 0xAA;
//	variable_2 = 0xBB;
//
//	TRISD = 0;
//	while(1) 
//	{
//   		variable_1++;
//	}
//}

//---------------------------
// ****** Example 7-x2 
// Generate a Random Number when a switch is pressed
//---------------------------
//unsigned char seed; 
//void main (void) 
//{
//    // PORTB is OUTPUT
//    PORTB = 0; LATB=0; ANSELB = 0;  TRISB=0;
// 
//    // PORTD.RD0 is input
//    PORTD = 0; LATD=0; ANSELD = 0; 
//    TRISDbits.TRISD0 = 1;   // bit 0 is an input
//    
//    seed = 1; 
//    while(1)
//    {
//        while (PORTDbits.RD0==0) // radom number is generated
//        {
//            seed++;
//            if (seed == 7)
//                seed = 1;
//            PORTB = seed;
//        }
//    }
//    
//}

//---------------------------
// ****** Example 7-x3 
//Increment numbers on 7Segment Display
//--------------------------

// program memory
//const char lookUp7Seg[] __at(0x100) = { 0x3f, 0x06, 0x5b, 0x5b, 0x4f, 0x66,
//                                        0x6d, 0x7d, 0x07, 0x7f, 0x6f }; 
//
//// data memory 
//char data1; 
//// function Get7Seg
//char Get7Seg (char bcd) 
//{
//    if (bcd<=9)
//        return lookUp7Seg[bcd];
//    else
//        return 0;
//}
//// main program
//void main (void) 
//{
//    // PORTB is OUTPUT
//    PORTB = 0; LATB=0; ANSELB = 0;  TRISB=0; 
//    unsigned char i;
//    for (i=0; i<10; i++)
//    {
//       data1 = Get7Seg(i);
//       PORTB = data1;
//    }
//    while(1); // stop here!
//    
//}

//---------------------------
// ****** Example 7-x4 
//Available functions in stdlib.h
//-------------------------- 
//
//#include <stdio.h> 
//#include <stdlib.h>
//#include <math.h>
//
//void main (void)
//{
//   int value1; int value2;
//   for (int i=0; i<10; i++)
//    {
//      value1 = rand();
//      value2 = 10*(float)value1/32767; // return integer
//      LATB = value2;
//      //printf("Today's lucky number is %d\n", value); // lots of memory usage if uncommented
//    }
//}

//---------------------------
// ****** Example 7-x5 
//Available functions in string.h
//-------------------------- 

// program memory
//#include <string.h>
//#include <stdio.h>
//
//int main(void)
//{
//  char buf1[50]  = {"PIC18 is Amazing!"}; // places in RAM starting reg 0x90
//  char buf2[50] = {"The IDE or the Chip?"};// places in RAM starting reg 0x00
//
//  strcpy(buf1, buf2); // function in string.h library
//  LATB = 1; // just for fun
//}


//---------------------------
// ****** Example 7-x6
//Solving Math Expressions
//-------------------------- 
//#include <stdio.h> 
//#include <stdlib.h>
//#include <math.h> 
//
//float Fr[10];
//float L=1.0e-3;
//float C=1.0e-6;
//float mysqrtbuff;
//
//// Calculating the resonant frequency = 1/(2pi * sqrt (L * C))
//void main (void) 
//{
//    int a; 
//    for (a=0; a<10; a++)
//    {
//        Fr[a] = 1/(6.2831853 * sqrt(L*C));
//        L += 1.e-6; // starting with 1 mH to 10 mH 
//    }
//}

//---------------------------
// ****** Example 7-x7
// Usage of strlen() 
//-------------------------- 
//#include <string.h>
//#include <stdio.h>
//char buffer[] = "Programming PIC 18 is Fun! ";
//unsigned int testChar __at(0x120); // number of digits in the string 
//char lenghtBuffer __at(0x100); // length of the string 
//char myChar __at(0x110); // actual digits in the string
//int a; 
//void main (void)
//{
//    lenghtBuffer = strlen(buffer); // store the length = 27 or 0x1B @0x100
//    for (a=0; a<strlen(buffer); a++)
//    {
//        if (buffer[a] >= '0' && buffer[a] <= '9' )
//        {
//            myChar = buffer[a]; //display character in the string
//            testChar++; // count the number of numbers in the string
//        }
//    }
//} 
