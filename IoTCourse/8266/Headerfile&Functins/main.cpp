#include <Arduino.h>
#include <math.h>
#include <myheader.h>

void loop()
{

  for (x = 0; x <= PI; x = x + 0.1)
  {
    k = myMultiplyFunction(x, y);
    y = sin(x);
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.println(k);
    delay(200);
  }
}
