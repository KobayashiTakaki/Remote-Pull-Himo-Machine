#include <Servo.h>

Servo servo;
int pinSv = 3;

void pullHimo(){
  servo.attach(pinSv);
  servo.write(110);
  delay(300);
  servo.write(0);
  delay(300);
  servo.detach();
}

void setup() {
  pinMode(pinSv, OUTPUT);
}

void loop() {
  
}

