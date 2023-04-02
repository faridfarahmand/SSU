#include <xc.h>
//****************************************
// Practicing with examples in Chapter 7
// PIC MICROCONTROLLER AND EMBEDDED SYSTEMS Using Assembly and C for PIC18
// by Muhammad Ali Mazidi - starting page 254 (PDF)
//****************************************



//---------------------------
// ****** Example 7-1
//---------------------------
// void main(void) {
//     unsigned char z; 
//     TRISB = 0; 
//     for(z=0; z<20; z++)
//         PORTB = z;
//     return; // or while(1)
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

// #define SevenSeg PORTB // not need to have ; - als does not showup as a variable
// #define mybit PORTBbits.RB7

// void main(void) {
    
//     asm("BANKSEL	PORTB") ;
//     asm("CLRF	PORTB") ;       //Init PORTB
//     asm("BANKSEL	LATB") ;    //Data Latch
//     asm("CLRF	LATB") ;
//     asm("BANKSEL	ANSELB") ;
//     asm("CLRF	ANSELB") ;      //digital I/O
//     asm("BANKSEL	TRISB") ;
//     asm("MOVLW	0b00000000") ;
//     asm("MOVWF	TRISB") ;

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

// // function definition 
// void  MSDelay(unsigned int itime1, unsigned int itime2)
// {
//     unsigned int i; unsigned int j;
//     for (i=0; i<itime1; i++)    // should be able to see itime as a variable
//       for (j=0; j<itime2; j++);      
// }

//---------------------------
// ****** Example 7-13 
//---------------------------
#define myInput PORTBbits.RB5
#define myOutput PORTBbits.RB0

void main(void) {
    
    asm("BANKSEL	PORTB") ;
    asm("CLRF	PORTB") ;       //Init PORTB
    asm("BANKSEL	LATB") ;    //Data Latch
    asm("CLRF	LATB") ;
    asm("BANKSEL	ANSELB") ;
    asm("CLRF	ANSELB") ;      //digital I/O
    asm("BANKSEL	TRISB") ;
    //asm("MOVLW	0b00000000") ;
    //asm("MOVWF	TRISB") ;

    TRISBbits.TRISB5 = 1;   // bit 5 is an input
    TRISBbits.TRISB0 = 0;   // bit 0 is an output

    while(1) // run forever
    {
        if (myInput == 1)
            myOutput = 1;
        else 
            myOutput = 0;
    }     
} // end of main()
