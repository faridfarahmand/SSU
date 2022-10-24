#include <Arduino.h> 

struct student{
   String name;
   int age;
   int roll_no;
};

void setup() {
   // put your setup code here, to run once:
   Serial.begin(9600);
   Serial.println();
   student A = {"Yash", 25, 26};
   Serial.println(A.name);
   Serial.println(A.age);
   Serial.println(A.roll_no);

   A.age = 27;
   Serial.println(A.age);
}

void loop() {
   // put your main code here, to run repeatedly:
}
