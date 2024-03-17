;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This is the headerfile containing one or more functions 
; Note that all variables used in the functions must be defined here
; We must also have PSECT statement at the begining  
; ORG is used to speciafy where the program must be assembled
; See this for more details: 
; https://docs.google.com/presentation/d/1ncyPaBJeA2svo2R948PHlVa4i-EYXyIp/edit?usp=sharing&ouid=116265364704461279990&rtpof=true&sd=true
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   
    
PSECT absdata,abs,ovrld        ; Do not change
ORG	    0x100
    
;---------------------
; Definitions & Variables
;---------------------    
what_button EQU		20h    ; this is used in the main and the header
 

;-------------------------------------
; Call Functions
;------------------------------------- 
_check_keypad:
    movf what_button, w			;	we want to copy our last digit that was pressed into w

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
