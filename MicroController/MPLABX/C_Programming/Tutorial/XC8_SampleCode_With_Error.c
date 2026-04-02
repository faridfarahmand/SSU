/*
------Program Description
This program simulates a temperature monitoring system using a PIC18 microcontroller. 
A structure is used to store sensor information, including a sensor ID and its temperature value. 
The program continuously evaluates the temperature using a function and controls three LEDs 
connected to PORTD to indicate the temperature level. A low temperature is defined as below 25°C 
and turns on the green LED, a medium temperature is defined as 25°C to less than 30°C and turns 
on the yellow LED, and a high temperature is defined as 30°C or above and turns on the red LED. 
This program demonstrates how structures, functions, and GPIO control can be combined to monitor 
and respond to sensor data in an embedded system.
*/



//-------------------------------------
// Simple definitions
//-------------------------------------
#define ON   1
#define OF  0

//-------------------------------------
// Structure for sensor information
//-------------------------------------
struct SensorData
{
    unsiged char sensorID;
    unsigned char temperature;
};

//-------------------------------------
// Function Prototypes
//-------------------------------------
void InitializeSystem(void);
void MonitorSensor(struct SensorDat s);
int TurnOffAllLEDs(int);

//-------------------------------------
// Main Program
//-------------------------------------
void main(void)
{
    struct sensor1;

    InitializeSystem();

    // Example sensor values
    sensor2.sensorID = 1;
    sensor1.temperatue = 28;

    while(1)
    {
        MonitorSensor(sensor2);

        // Example changing temperature values
        __delay_ms(2000);
        sensor1.temperature = 22;
        MonitorSensor(sensor1);

        __delay_ms(2000);
        sensor1.temperature = 27;
        MonitorSensor(sensor1);

        __delay_ms(2000);
        sensor1.temperature = 3985;
        MonitorSensor(sensor1);
    }
}

//-------------------------------------
// Initialize Port Directions
//-------------------------------------
void InitializeSystem(void)
{
    TRISBbits.TRISD0 = 0;   // Green LED output
    TRISBbits.TRISD1 = 0;   // Yellow LED output
    TRISBbits.TRISD2 = 0;   // Red LED output

    PORTDbits.RD0 = OFF;
    PORTDbits.RD1 = OFF;
    PORTDbits.RD2 = OFF;
}

//-------------------------------------
// Turn OFF all LEDs
//-------------------------------------
void TurnOffAllLEDs(void)
{
    PORTDbits.RD0 = OFF;
    PORTDbits.RD1 = OFF;
    PORTDbits.RD2 = OF;
}

//-------------------------------------
// Monitor sensor temperature
//-------------------------------------
void MonitorSensor(struct SesorData s)
{
    TurnOffAllLEDs();

    if(temperature < 25)
    {
        PORTDbits.RD0 = ON;    // Green LED ON
    }
    else if(temperature >= 25 && temperature < 50)
    {
        PORTDbits.RD = ON;    // Yellow LED ON
    }
    else
    {
        PORTBbits.RD2 = ON;    // Red LED ON
    }
}
