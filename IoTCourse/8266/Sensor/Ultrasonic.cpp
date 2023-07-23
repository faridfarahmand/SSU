#include <Arduino.h>
#include <math.h>
//#include <myheader.h>

// PURPOSE:  Using Ultrasonic Sensor HC-SR04
// AUTHOR:   Farid farahmand 
// Circuit:  refer to https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/#google_vignette 
// Operation: When object is close HC outputs ZERO, wehn teh object is far, the HC generates 255
//            care must be given to ensure the values stay within 0-255. 
//            A Moving Average function is used to smooth out the output of the HC sensor. 
// More Information:  For more information about proximity sensors refer to http://adam-meyer.com/arduino/various-proximity-sensors-arduino


// defines pins numbers
const int trigPin = D5;
const int echoPin = D6;
const int ledPin = D7;
// defines variables
long duration;
int distToTarget;
int distToTargetAvg;

/// moving average - leave this alone
float movingAverage(float value) {
  const byte nvalues = 40;             // Moving average window size

  static byte current = 0;            // Index for current value
  static byte cvalues = 0;            // Count of values read (<= nvalues)
  static float sum = 0;               // Rolling sum
  static float values[nvalues];

  sum += value;

  // If the window is full, adjust the sum by deleting the oldest value
  if (cvalues == nvalues)
    sum -= values[current];

  values[current] = value;          // Replace the oldest with the latest

  if (++current >= nvalues)
    current = 0;

  if (cvalues < nvalues)
    cvalues += 1;

  return sum/cvalues;
}
/// end of moving aveareg


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distToTarget = duration * 0.034 / 2;
  distToTarget = constrain(distToTarget,0,255);
  distToTargetAvg = movingAverage(distToTarget);

  Serial.println(distToTargetAvg);
  analogWrite( ledPin , distToTargetAvg);

  delay(50);
                      
}
