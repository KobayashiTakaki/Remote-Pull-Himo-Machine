#include <Servo.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

Servo servo;
unsigned long timeS = 0;
unsigned long timeCnt = 0;
unsigned long timeWakeup = 0;
int pinInt = 2;
int pinIR = 4;
int pinSv = 3;
int pinLed = 6;
int flagA = 0;
int flagB = 0;

void pullHimo(){
  Serial.println("pull");
  servo.attach(pinSv);
  servo.writeMicroseconds(1000);
  for(int i = 1000; i <= 2200; i = i + 3){
    servo.writeMicroseconds(i);
    delay(1);
  }
  for(int i = 2200; i >= 1000; i = i - 3){
    servo.writeMicroseconds(i);
    delay(1);
  }
  //servo.write(110);
  //delay(300);
  //servo.write(0);
  //delay(300);
  servo.detach();
}

void ledChika(){
  digitalWrite(pinLed, LOW);
  digitalWrite(pinLed, HIGH);
  delay(100);
  digitalWrite(pinLed, LOW);
  delay(60);
  digitalWrite(pinLed, HIGH);
  delay(100);
  digitalWrite(pinLed, LOW);
  delay(60);
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
}

void wakeup(){
  noInterrupts();
}


void setup(){
  pinMode(pinIR, INPUT_PULLUP);
  pinMode(pinSv, OUTPUT);
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(0, wakeup, FALLING);
  pinMode(pinInt, INPUT_PULLUP);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop(){
  Serial.println("wakeup");
  timeWakeup = millis();
  Serial.println(timeWakeup);
  flagA = 0;
  flagB = 0;
  while(millis() - timeWakeup < 1000){
    if(digitalRead(pinIR) == LOW){
      timeS = micros();
      while(digitalRead(pinIR) == LOW){
        timeCnt = micros() - timeS;
      }
      
      Serial.print("timeCnt:");
      Serial.println(timeCnt);
      
      if(timeCnt > 7000){
        flagA += 1;
      }else if(500 < timeCnt && timeCnt < 7000){
        flagB += 1;
      }else{
        flagA = 0;
        flagB = 0;
        break;
      }
      
      timeCnt = 0;
      
    }
  
    if(flagB > 5 && flagA > 0){
      pullHimo();
      flagA = 0;
      flagB = 0;
      delay(300);
      break;
    }
  }
  timeCnt = 0;
  timeWakeup = 0;
  flagA = 0;
  flagB = 0;

  Serial.println("sleep");
  Serial.flush();
  interrupts();
  delay(100);
  sleep_mode();
}


