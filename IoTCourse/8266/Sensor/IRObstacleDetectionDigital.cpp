#include <Arduino.h>
#include <math.h>
//#include <myheader.h>

// PURPOSE:  KY-032 INFRARED OBSTACLE AVOIDANCE SENSOR MODULE
// AUTHOR:   Farid farahmand 
// Circuit:  refer to https://arduinomodules.info/ky-032-infrared-obstacle-avoidance-sensor-module/
// Operation: When object is within the range the module generates a 1. This is a digital sensor. 
//            See the circuit herehttps://www.engineersgarage.com/arduino-ir-led-ir-photodiode-proximity-sensor/  

// defines pins numbers
int ledPin = D6;      // LED pin on arduino
int detectorPin = D5;  // obstacle avoidance sensor interface
int val;              // variable to store result
int valAvg;           // variable to store average result
//int enablePin = 2;  // sensor enable interface (EN)


void setup()
{
  pinMode(ledPin, OUTPUT);  // Define LED as output interface
  pinMode(detectorPin, INPUT);  // Define obstacle avoidance sensor as input interface
  Serial.begin(9600); // Starts the serial communication
  
  // [uncomment and remove jumper on module to use enable pin (EN)]
  //pinMode(enablePin, OUTPUT);
  //digitalWrite(enablePin, HIGH);  // Enable sensor
}
void loop()
{
  val = digitalRead(detectorPin); // Read value from sensor
  if(val == LOW) // When the sensor detects an obstacle, the LED on the Arduino lights up
    {
      digitalWrite(ledPin, HIGH);
    }
    else
    {
      digitalWrite(ledPin, LOW);
    }

  Serial.println(val);   // print or plot

}
