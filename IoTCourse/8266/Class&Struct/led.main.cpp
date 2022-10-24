#include "Led.h"

#define LED_1_PIN LED_BUILTIN

Led led1(LED_1_PIN);

void setup() { 
    Serial.begin(9600);
}

void loop() {
    led1.on();
    delay(5000);
    led1.off();
    delay(500);
  }