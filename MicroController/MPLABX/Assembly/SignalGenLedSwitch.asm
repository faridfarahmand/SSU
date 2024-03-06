;---------------------
; Title: Generate a waveform when the switch is pressed
;---------------------
; Program Details:
; The purpose of this program is to generate a waveform when a switch is pressed. 
;   When the input port is HIGH no waveform is generated. When RD1 is LOW 
;   a waveform is generated. The frequency of the waveform is controlled 
;   by the values of the Inner_loop and Outer_loop
;   In this example, we utilize the CALL /RETURN concepts
    
    
; Inputs: Inner_loop ,Outer_loop, RD1 (input)
; Outputs: RD0 (output)
; Setup: The Curiosity Board
    
; Date: Feb 24, 2024
; File Dependencies / Libraries: It is required to include the 
;   AssemblyConfig.inc in the Header Folder
; Compiler: xc8, 2.4
; Author: Farid Farahmand
; Versions:
;       V1.0: Original
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 

;---------------------
; Initialization
;---------------------
#include "./myConfigFile.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
Inner_loop  equ 150 // in decimal
Outer_loop  equ 200
 
;---------------------
; Program Constants
;---------------------
REG10   equ     10h   // in HEX
REG11   equ     11h

;---------------------
; Definitions
;---------------------
#define SWITCH    PORTD,1  
#define LED0      PORTD,0
#define LED1      PORTB,0
   
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    GOTO        _initialization

    ORG          0020H           ; Begin assembly at 0020H
 
_initialization: 
    RCALL _setupPortD
    RCALL _setupPortB
    
_main:
    ;BTFSC	SWITCH  ; By removing these two lines the input has no impact
    ;BRA	_main
    BTG		LED0
    BTG		LED1
    CALL	loopDelay ; we can use RCALL
    BRA         _main
    
;-----The Delay Subroutine    
loopDelay: 
    MOVLW       Inner_loop
    MOVWF       REG10
    MOVLW       Outer_loop
    MOVWF       REG11
_loop1:
    DECF        REG10,1
    BNZ         _loop1
    DECF        REG11,1 // outer loop
    BNZ        _loop1
    RETURN
 
_setupPortD:
    BANKSEL	PORTD ;
    CLRF	PORTD ;Init PORTA
    BANKSEL	LATD ;Data Latch
    CLRF	LATD ;
    BANKSEL	ANSELD ;
    CLRF	ANSELD ;digital I/O
    BANKSEL	TRISD ;
    MOVLW	0b11111110 ;Set RD[7:1] as inputs
    MOVWF	TRISD ;and set RD0 as ouput
    RETURN
 
_setupPortB:
    BANKSEL	PORTB ;
    CLRF	PORTB ;Init PORTA
    BANKSEL	LATB ;Data Latch
    CLRF	LATB ;
    BANKSEL	ANSELB ;
    CLRF	ANSELB ;digital I/O
    BANKSEL	TRISB ;
    MOVLW	0b11111110 ;Set RD[7:1] as inputs
    MOVWF	TRISB ;and set RD0 as ouput
    RETURN    
    
    END


