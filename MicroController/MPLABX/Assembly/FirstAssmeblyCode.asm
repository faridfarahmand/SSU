;---------------------
; Title: ADD TWO BYTES
;---------------------
; Program Details:
; The purpose of this program is to load two bytes, Byte1 (F2H) and Byte2 (02H), in data registers REG0 
; The program may contain logical errors and should be debugged. Follow the comments and correct the errors

;-------
; Read This:
;-------
; You may need to change the path for the AssemblyConfig.inc; this file contains all the configuration bits
; To simulate go to the Windows tab, then Target Memory View--> File Registers and view the registers

		
; Inputs: Byte 1 and Byte 2
; Outputs: Register o
; Date: Feb 24, 2024
; File Dependencies / Libraries: None 
; Compiler: ASM 2.4
; Author: Farid Farahmand
; Versions:
;       V1.0: Oriiginal Program
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 

;---------------------
; Initialization
;---------------------
#include "C:\Users\Farid\Downloads\FirstThing\FirstAssemblyMPLAB.X\AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Definitions
;---------------------
#define LED1	  PORTD,1

;---------------------
; Program Inputs
;---------------------
BYTE01	EQU		0xF2	;Data bytes 
BYTE02	EQU		0x02

;---------------------
; Registers
;---------------------
REG00	EQU		0x00	;Data Register addresses

;---------------------
; Main Program
;---------------------
PSECT absdata,abs,ovrld        ; Do not change
   
	ORG	00		;Reset vector
	GOTO	START

	ORG	0020H		;Begin assembly at 0020H
START:	
	MOVLW	BYTE01		;Load F2H into W register
        MOVWF	REG00,0		;Save F2H in REG0
	MOVLW	BYTE02		;Load 02H into W register
	ADDWF	REG00,1,0	;Add byte in REG0 
	SLEEP			;Power down
	END


