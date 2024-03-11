;---------------------
; Title: Keypad
;---------------------
; Program Details:
; The purpose of this program is to check which number has been pressed 
; on the keypad and then turn on PORD accordingly, showing the binary
; representation. 
    
; Inputs: RB3. 4. 6. 7
; Outputs: RD0-7 & RB0,1,2
; Setup: The Curiosity Board

; Date: Feb 24, 2024
; File Dependencies / Libraries: It is required to include the
;   AssemblyConfig.inc in the Header Folder
; Compiler: xc8, 2.4
; Author: Farid Farahmand
; Versions:
;       V1.0: Original
; Useful links:
; Keypad: https://www.circuitbasics.com/how-to-set-up-a-keypad-on-an-arduino/ 

;---------------------
; Initialization
;---------------------
#include "./AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------


;---------------------
; Program Constants
;---------------------
what_button EQU		20h
BYTE01	EQU		0xF2		;Data bytes
BYTE02	EQU		0x32
REG00	EQU		0x00		;Data Register addresses
REG01	EQU		0x01
REG02	EQU		0x02
REG10	EQU		0x10

;---------------------
; Definitions
;---------------------


;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change

    ORG          0                ;Reset vector
    GOTO        _setup

    ORG          0020H           ; Begin assembly at 0020H


_setup:
    clrf what_button
    clrf WREG
    RCALL _setupPortD
    RCALL _setupPortB
    clrf PORTB


_main:
    RCALL _check_keypad
    MOVFF   what_button,PORTD
    
;    MOVLW	0xAA
;    MOVWF	REG10
;    BYTE	0x10
    
    GOTO    _main


;-------------------------------------
; Call Functions
;-------------------------------------
_setupPortD:
    BANKSEL	PORTD ;
    CLRF	PORTD ;Init PORTA
    BANKSEL	LATD ;Data Latch
    CLRF	LATD ;
    BANKSEL	ANSELD ;
    CLRF	ANSELD ;digital I/O
    BANKSEL	TRISD ;
    MOVLW	0b00000000 ;Set RD[7:1] as outputs
    MOVWF	TRISD ;and set RD0 as ouput
    RETURN

_setupPortB:
    BANKSEL	PORTB ;
    CLRF	PORTB ;Init PORTB
    BANKSEL	LATB ;Data Latch
    CLRF	LATB ;
    BANKSEL	ANSELB ;
    CLRF	ANSELB ;digital I/O
    BANKSEL	TRISB ;
    MOVLW	0b11111000 ;
    MOVWF	TRISB ;
    RETURN

_check_keypad:
    movf what_button, w			

    bsf PORTB, 0			;	lets scan the first column of keys
    btfsc PORTB, 3			;	has the 1 key been pressed? if yes then
    movlw 1				;	copy decimal number 01 into w. but if not then continue on.
    btfsc PORTB, 4			;	has the 4 key been pressed? if yes then
    movlw 4				;	copy decimal number 04 into w. but if not then continue on.
    btfsc PORTB, 7			;	has the 7 key been pressed? if yes then
    movlw 7				;	copy decimal number 07 into w. but if not then continue on.
    btfsc PORTB, 6			;	has the * key been pressed? if yes then
    movlw 10				;	copy decimal number 10 into w. but if not then continue on.
    bcf PORTB, 0			;	now we have finished scanning the first column of keys

    bsf PORTB, 1			;	lets scan the middle column of keys
    btfsc PORTB, 3			;	has the 2 key been pressed? if yes then
    movlw 2				;	copy decimal number 02 into w. but if not then continue on.
    btfsc PORTB, 4			;	has the 5 key been pressed? if yes then
    movlw 5				;	copy decimal number 05 into w. but if not then continue on.
    btfsc PORTB, 7			;	has the 8 key been pressed? if yes then
    movlw 8				;	copy decimal number 08 into w. but if not then continue on.
    btfsc PORTB, 6			;	has the 0 key been pressed? if yes then
    movlw 0				;	copy decimal number 00 into w. but if not then continue on.
    bcf PORTB, 1			;	now we have finished scanning the middle column of keys

    bsf PORTB, 2			;	lets scan the last column of keys
    btfsc PORTB, 3			;	has the 3 key been pressed? if yes then
    movlw 3				;	copy decimal number 03 into w. but if not then continue on.
    btfsc PORTB, 4			;	has the 6 key been pressed? if yes then
    movlw 6				;	copy decimal number 06 into w. but if not then continue on.
    btfsc PORTB, 7			;	has the 9 key been pressed? if yes then
    movlw 9				;	copy decimal number 09 into w. but if not then continue on.
    btfsc PORTB, 6			;	has the # key been pressed? if yes then
    movlw 11				;	copy decimal number 11 into w. but if not then continue on.
    bcf PORTB, 2			;	now we have finished scanning the last column of keys

    movwf what_button	
return						

BYTE	MACRO	REGXX
    LFSR	FSR1,REGXX
    MOVFF	POSTDEC1,REG00
    MOVFF	INDF1,REG00
ENDM
    END
