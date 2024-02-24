;---------------------
; Title: Waveform Generator with Delay
;---------------------
; Program Details:
; The purpose of this program is to demonstrate how to call a delay function. 

; Inputs: Inner_loop ,Outer_loop 
; Outputs: PORTD
; Date: Feb 24, 2024
; File Dependencies / Libraries: None 
; Compiler: xc8, 2.4
; Author: Farid Farahmand
; Versions:
;       V1.3: Changes the loop size
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 


;---------------------
; Initialization
;---------------------
#include "AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
Inner_loop  equ 5 // in decimal
Outer_loop  equ 5
 
;---------------------
; Program Constants
;---------------------
REG10   equ     10h   // in HEX
REG11   equ     11h
REG01   equ     1h

;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1	  PORTD,1

;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    GOTO        _start1

    ORG          0020H           ; Begin assembly at 0020H

_start1:
    MOVLW       0b11111100
    MOVWF       TRISD,0
    MOVWF       REG01,0

_onoff:
    MOVFF       REG01,PORTD
    MOVLW       Inner_loop
    MOVWF       REG10
    MOVLW       Outer_loop
    MOVWF       REG11

_loop1:
    DECF        REG10,1
    BNZ         _loop1
    ;DECF        REG11,1 // outer loop
    ;BNZ        _loop1
    
    COMF        REG01,1 // Negate the register 
    BRA         _onoff
END
