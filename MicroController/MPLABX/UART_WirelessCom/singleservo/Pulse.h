/*
 * File:   Header Functions
 * Author: Brian Gomez Jimenez
 *
 * Created on July 16, 2023, 10:49 AM
 */
/**************Timers 2 *******************/
void TMR2_Initialize(void){
    // Set TMR2 to the options selected in the User Interface

    // T2CS FOSC/4; 
    T2CLKCON = 0x01; //Page 336? This refers to TxCLK register and selects the 
    //TIMER2 Clock Source

    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; 
    T2HLT = 0x00; //Page 340

    // T2RSEL T2CKIPPS pin; 
    T2RST = 0x00; //Page 337

    // PR2 155; 
    T2PR = 0x9B; //Page 338
    //T2PR = 199; 
    //sets PWM period to 20ms. This is calculated using equation 23-1 on Page 
    //356 for PWM period and designed for a MG996r servo

    // TMR2 0; 
    T2TMR = 0x00; //Page 338

    // Clearing IF flag.
    PIR4bits.TMR2IF = 0; //Page 141

    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on; 
    T2CON = 0xF0;//0xE0; //Page 339
}

void TMR2_Start(void){
    // Start the Timer by writing to TMRxON bit
    T2CONbits.TMR2ON = 1; //Page 339
}



void PWM2_Initialize(void){
    // Set the PWM2 to the options selected in the User Interface
	
	// MODE PWM; EN enabled; FMT left_aligned; 
	CCP2CON = 0x8C;  // enable / Right-aligned format  //Page 351
	
	// RH 127; 
	CCPR2H = 0x0;    //Page 354
	
	// RL 192; 
	CCPR2L = 0x0f;   //Page 354

	// Selecting Timer 2
	CCPTMRS0bits.C2TSEL = 0x1; //Page 352   
}



void PWM2_LoadDutyValue(uint16_t dutyValue){
    dutyValue &= 0x03FF;

    // Load duty cycle value /Right-aligned format by default FMT=0
    if(CCP2CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
    else
    {
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
}


/****OUTPUT PINS PWM */
void PWM_Output_Pin_Enable (void){
    //************PPS Unlock Sequence****************/
    PPSLOCK = 0x55; //Page 284
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS
    //************PPS Unlock Sequence****************/
    
    //Set C1 as the output of CCP2
    //This can be changed to whatever pin that you want using RxyPPS register 
    //and the Table 17-2 
    RC1PPS = 0x0A; //Page 283 1 x ccp2
    
    //************PPS Lock Sequence****************/
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
    //************PPS Lock Sequence****************/
}
