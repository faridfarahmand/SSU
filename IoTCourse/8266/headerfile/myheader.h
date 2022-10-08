void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  Serial.begin(9600);
}
float myMultiplyFunction(float x, float y)
{
  float result;
  result = sq(x) * sq(y);
  return result;
}
float x = 0;
float y = 0;
float k;