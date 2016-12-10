#include <Servo.h>

Servo servo;
unsigned long timeWakeup = 0;
unsigned long timeSt = 0;
unsigned long timeCnt = 0;
int pinIR = 4;
int pinSv = 3;
int flagA = 0;
int flagB = 0;

void pullHimo(){
  Serial.println("pull");
  servo.attach(pinSv);
  servo.write(110);
  delay(300);
  servo.write(0);
  delay(300);
  servo.detach();
}

void setup(){
  pinMode(pinIR, INPUT_PULLUP);
  pinMode(pinSv, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if (digitalRead(pinIR) == LOW) {
    Serial.println("wakeup");
    timeWakeup = millis();
    
    while(millis() - timeWakeup < 1000){
      timeSt = 0;
      timeCnt = 0; 
      if(digitalRead(pinIR) == LOW){
        timeSt = micros();
        while(digitalRead(pinIR) == LOW){
          timeCnt = micros() - timeSt;
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
      }
    
      if(flagB > 5 && flagA > 0){
        pullHimo();
        flagA = 0;
        flagB = 0;
        delay(300);
        break;
      }
    }
  }
 
  timeWakeup = 0;
  timeSt = 0;
  timeCnt = 0;
  flagA = 0;
  flagB = 0;
  
}


