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

/******************************************************/
/*Timer 4 */
void TMR2_Initialize_2(void){
    // Set TMR4 to the options selected in the User Interface

    // T2CS FOSC/4; 
    T4CLKCON = 0x01; //Page 336? This refers to TxCLK register and selects the 
    //TIMER2 Clock Source

    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; 
    T4HLT = 0x00; //Page 340

    // T2RSEL T2CKIPPS pin; 
    T4RST = 0x00; //Page 337

    // PR2 155; 
    T4PR = 0x9B; //Page 338
    //sets PWM period to 20ms. This is calculated using equation 23-1 on Page 
    //356 for PWM period and designed for a MG996r servo

    // TMR4 0; 
    T4TMR = 0x00; //Page 338

    // Clearing IF flag.
    PIR7bits.TMR4IF = 0; //Page 141

    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on; 
    T4CON = 0xF0;//0xE0; //Page 339
}

void TMR4_Start_2(void){
    // Start the Timer by writing to TMRxON bit
    T4CONbits.TMR4ON = 1; //Page 339
}
/******************************************************/
/*Timer 6 */
void TMR2_Initialize_6(void){
    // Set TMR4 to the options selected in the User Interface

    // T2CS FOSC/4; 
    T6CLKCON = 0x01; //Page 336? This refers to TxCLK register and selects the 
    //TIMER2 Clock Source

    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; 
    T6HLT = 0x00; //Page 340

    // T2RSEL T2CKIPPS pin; 
    T6RST = 0x00; //Page 337

    // PR2 155; 
    T6PR = 0x9B; //Page 338
    //sets PWM period to 20ms. This is calculated using equation 23-1 on Page 
    //356 for PWM period and designed for a MG996r servo

    // TMR4 0; 
    T6TMR = 0x00; //Page 338

    // Clearing IF flag.
    PIR9bits.TMR6IF = 0; //Page 141

    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on; 
    T6CON = 0xF0;//0xE0; //Page 339
}
void TMR4_Start_6(void){
    // Start the Timer by writing to TMRxON bit
    T6CONbits.TMR6ON = 1; //Page 339
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
void PWM2_Initialize_2(void){
    // Set the PWM2 to the options selected in the User Interface
	
	// MODE PWM; EN enabled; FMT left_aligned; 
	CCP1CON = 0x8C;  // enable / Right-aligned format  //Page 351
	
	// RH 127; 
	CCPR1H = 0x0;    //Page 354
	
	// RL 192; 
	CCPR1L = 0x0f;   //Page 354
    //Timer4 
	// C1TSEL[1:0]: CCP1 Timer Selection bits
    // 0b10 = CCP1 is based off Timer4 in Capture/Compare mode and Timer4 in PWM mode
	CCPTMRS0bits.C1TSEL = 0x02; //Page 352   
}
void PWM2_Initialize_3(void){
    // Set the PWM2 to the options selected in the User Interface
	
	// MODE PWM; EN enabled; FMT left_aligned; 
	CCP3CON = 0x8C;  // enable / Right-aligned format  //Page 351
	
	// RH 127; 
	CCPR3H = 0x0;    //Page 354
	
	// RL 192; 
	CCPR3L = 0x0f;   //Page 354
    //Timer4 
	// C3TSEL[1:0]: CCP3 Timer Selection bits
    // 0b10 = CCP1 is based off Timer6 in Capture/Compare mode and Timer4 in PWM mode
	CCPTMRS0bits.C3TSEL = 0x03; //Page 352   
}
void PWM2_Initialize_4(void){
    // Set the PWM2 to the options selected in the User Interface
	
	// MODE PWM; EN enabled; FMT left_aligned; 
	CCP4CON = 0x8C;  // enable / Right-aligned format  //Page 351
	
	// RH 127; 
	CCPR4H = 0x0;    //Page 354
	
	// RL 192; 
	CCPR4L = 0x0f;   //Page 354
    //Timer4 
	// C4TSEL[1:0]: CCP4 Timer Selection bits
    // 11= CCP4isbasedoffTimer5inCapture/ComparemodeandTimer6inPWMmode
	CCPTMRS0bits.C4TSEL = 0x03; //Page 352   
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
void PWM2_LoadDutyValue_2(uint16_t dutyValue){
    dutyValue &= 0x03FF;

    // Load duty cycle value /Right-aligned format by default FMT=0
    if(CCP1CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR1H = dutyValue >> 8;
        CCPR1L = dutyValue;
    }
    else
    {
        CCPR1H = dutyValue >> 8;
        CCPR1L = dutyValue;
    }
}
void PWM2_LoadDutyValue_3(uint16_t dutyValue){
    dutyValue &= 0x03FF;

    // Load duty cycle value /Right-aligned format by default FMT=0
    if(CCP3CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR3H = dutyValue >> 8;
        CCPR3L = dutyValue;
    }
    else
    {
        CCPR3H = dutyValue >> 8;
        CCPR3L = dutyValue;
    }
}
void PWM2_LoadDutyValue_4(uint16_t dutyValue){
    dutyValue &= 0x03FF;

    // Load duty cycle value /Right-aligned format by default FMT=0
    if(CCP4CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR4H = dutyValue >> 8;
        CCPR4L = dutyValue;
    }
    else
    {
        CCPR4H = dutyValue >> 8;
        CCPR4L = dutyValue;
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
    RC2PPS = 0x09; //Page 283 2 y ccp1
    RB0PPS = 0x0B; //Page 283 3   CCP3 
    RB1PPS = 0x0C;  //4 CCP4
    
    //************PPS Lock Sequence****************/
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
    //************PPS Lock Sequence****************/
}
/*ADC*/
void ADC_Initialize_(void)
{
       //Setup ADC
    ADCON0bits.FM = 1;  //right justify
    ADCON0bits.CS = 1; //ADCRC Clock
    TRISAbits.TRISA0 = 1; //Set RA0 to input
    ANSELAbits.ANSELA0 = 1; //Set RA0 to input
    TRISAbits.TRISA1 = 1; //Set RA1 to input
    ANSELAbits.ANSELA1 = 1; //Set RA1 to analog
    TRISAbits.TRISA2 = 1; //Set RA2 to input
    ANSELAbits.ANSELA2 = 1; //Set RA2 to input
    TRISAbits.TRISA3 = 1; //Set RA3 to input
    ANSELAbits.ANSELA3 = 1; //Set RA3 to analog
    ADCON0bits.ON = 1; //Turn ADC On    
}
int ADC_readValue(int channel)
{
    int digital;
    ADCON0bits.GO = 1;  // Start A/D conversion
    // Set the channel in the ADPCH register
    ADPCH = channel;
    ADCON0bits.GO = 1;  // Start A/D conversion
    while (ADCON0bits.GO);  // Wait for ADC conversion to complete
    digital = (ADRESH * 256) + ADRESL; // Combine 8-bit MSB and 8-bit LSB
    return digital;
}