/* 
 * File:   PWM.h
 * Author: student
 *
 * Created on May 1, 2023, 9:02 AM
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */



//void PWM_Output_D8_Enable (void);
//void PWM_Output_D8_Disable (void);

///////////////  TIMER 2
void TMR2_Initialize(void)
{
    // Set TMR2 to the options selected in the User Interface

    // T2CS FOSC/4; 
    T2CLKCON = 0x01;

    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; 
    T2HLT = 0x00;

    // T2RSEL T2CKIPPS pin; 
    T2RST = 0x00;

    // PR2 255; 
    T2PR = 0xFF;

    // TMR2 0; 
    T2TMR = 0x00;

    // Clearing IF flag.
    PIR4bits.TMR2IF = 0;

    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on; 
    T2CON = 0xA0;
}

//void TMR2_ModeSet(TMR2_HLT_MODE mode)
//{
//   // Configure different types HLT mode
//    T2HLTbits.MODE = mode;
//
//
//void TMR2_ExtResetSourceSet(TMR2_HLT_EXT_RESET_SOURCE reset)
//{
//    //Configure different types of HLT external reset source
//    T2RSTbits.RSEL = reset;
//}

void TMR2_Start(void)
{
    // Start the Timer by writing to TMRxON bit
    T2CONbits.TMR2ON = 1;
}

void TMR2_StartTimer(void)
{
    TMR2_Start();
}

void TMR2_Stop(void)
{
    // Stop the Timer by writing to TMRxON bit
    T2CONbits.TMR2ON = 0;
}

void TMR2_StopTimer(void)
{
    TMR2_Stop();
}

uint8_t TMR2_Counter8BitGet(void)
{
    uint8_t readVal;

    readVal = TMR2;

    return readVal;
}

uint8_t TMR2_ReadTimer(void)
{
    return TMR2_Counter8BitGet();
}

void TMR2_Counter8BitSet(uint8_t timerVal)
{
    // Write to the Timer2 register
    TMR2 = timerVal;
}

void TMR2_WriteTimer(uint8_t timerVal)
{
    TMR2_Counter8BitSet(timerVal);
}

void TMR2_Period8BitSet(uint8_t periodVal)
{
   PR2 = periodVal;
}

void TMR2_LoadPeriodRegister(uint8_t periodVal)
{
   TMR2_Period8BitSet(periodVal);
}

//bool TMR2_HasOverflowOccured(void)
//{
//    // check if  overflow has occurred by checking the TMRIF bit
//    bool status = PIR4bits.TMR2IF;
//    if(status)
//    {
//        // Clearing IF flag.
//        PIR4bits.TMR2IF = 0;
//    }
//    return status;
//}



///////////// END OF TIMER 


void PWM_Output_D8_Enable (void){
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    // Set D8 as the output of CCP2
    RB3PPS = 0x0A;

    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
}

void PWM_Output_D8_Disable (void){
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    // Set D8 as GPIO pin
    RB3PPS = 0x00;

    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
    
    TRISBbits.TRISB3 = 0;
}

void PWM2_Initialize(void)
{
    // Set the PWM2 to the options selected in the User Interface
	
	// MODE PWM; EN enabled; FMT left_aligned; 
	CCP2CON = 0x8C;  // enable / Right-aligned format  
	
	// RH 127; 
	CCPR2H = 0x0;    
	
	// RL 192; 
	CCPR2L = 0x0f;    

	// Selecting Timer 2
	CCPTMRS0bits.C2TSEL = 0x1;
    
}
void PWM2_LoadDutyValue(uint16_t dutyValue)
{
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

 _Bool PWM2_OutputStatusGet(void)
{
    // Returns the output status
    return(CCP2CONbits.OUT);
}
