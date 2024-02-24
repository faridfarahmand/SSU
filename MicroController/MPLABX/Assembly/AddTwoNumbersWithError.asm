;---------------------
; Title: ADD TWO BYTES
;---------------------
;Program Details:
; The purpose of this program is to load two bytes, Byte1 (F2H) and Byte2 (22H), in data registers REG0 and; REG1, respectively, and add the bytes.
; If the sum generates a carry, clear the data register REG2; otherwise, save the sum in REG2.

	
;-------
; Read This:
;-------
; Make sure you can compile the program. Correct all teh errors. 
; You may also need to change the path for the AssemblyConfig.inc; this file contains all the configuration bits
; To simulate go to Windows tab, then Target Memory View--> File Registers and view teh registers
; Make sure you add the header	
	

;---------------------
; Initialization
;---------------------
#include "AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Definitions
;---------------------
#define LED1	  PORTD,1

;---------------------
; Program Inputs
;---------------------
BYTE1	EQU		0xF2	;Data bytes 
BYTE2	EQU		0x32
BYTT3	EQU		0x32

;---------------------
; Registers
;---------------------
REG00	EQU		0x00	;Data Register addresses
REG01	EQU		0x01
REG2	EQU		0x02

;---------------------
; Main Program
;---------------------
PSECT absdata,abs,ovrld        ; Do not change
   
	ORG	00		;Reset vector
	GOTO	STRT

	ORG	0020H		;Begin assembly at 0020H
START	
	MOVLW	BYTE1		;Load F2H into W register
        MOVWF	RG00,0		;Save F2H in REG0
	MOVLW	BTE3		;Load 32H into W register
	ADDWF	REG00,0,0	;Add byte in REG0 to REG1
	BN    	SAVE		;If no carry, go to location SAVE
SAVE	
	MOVLF	REG02,0 	;Save Result or clear REG
	SLEEP			;Power down
	END
; The program may contain logical errors and should be debugged. Follow the comments and correct the errors


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
; Definitions
;---------------------
#define LED1	  PORTD,1

;---------------------
; Program Inputs
;---------------------
BYTE1	EQU		0xF2	;Data bytes 
BYTE2	EQU		0x32
BYTT3	EQU		0x32

;---------------------
; Registers
;---------------------
REG00	EQU		0x00	;Data Register addresses
REG01	EQU		0x01
REG2	EQU		0x02

;---------------------
; Main Program
;---------------------
PSECT absdata,abs,ovrld        ; Do not change
   
	ORG	00		;Reset vector
	GOTO	STRT

	ORG	0020H		;Begin assembly at 0020H
START	
	MOVLW	BYTE1		;Load F2H into W register
        MOVWF	RG00,0		;Save F2H in REG0
	MOVLW	BTE3		;Load 32H into W register
	ADDWF	REG00,0,0	;Add byte in REG0 to REG1
	BN    	SAVE		;If no carry, go to location SAVE
SAVE	
	MOVLF	REG02,0 	;Save Result or clear REG
	SLEEP			;Power down
	END
