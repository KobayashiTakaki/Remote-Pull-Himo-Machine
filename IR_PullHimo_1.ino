#include <Servo.h>

Servo servo;
unsigned long timeWakeup = 0; //wakeupした時間を入れておく変数
unsigned long timeSt = 0; //pinIRがLOWになった時間を入れておく変数
unsigned long timeCnt = 0; //pinIRがどれだけの間LOWだったかの時間を入れておく変数
int pinIR = 4; //赤外線受信モジュールの信号を読み取るピン番号
int pinSv = 3; //サーボモーターに信号を送るピン番号
int flagA = 0; //7000マイクロ秒より長い赤外線を受信したことを記録しておく変数
int flagB = 0; //500マイクロ秒より長くて7000マイクロ秒より短い赤外線を受信したことを記録しておく変数

//サーボモーターを動かす関数
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
    
    while (millis() - timeWakeup < 1000) { //wakeupしてから1000ミリ秒経過するまでループを継続する
      timeSt = 0;
      timeCnt = 0; 
      if (digitalRead(pinIR) == LOW) {
        timeSt = micros();
        while (digitalRead(pinIR) == LOW) {
          timeCnt = micros() - timeSt; //LOWになっている時間を計測する
        }
        Serial.print("timeCnt:");
        Serial.println(timeCnt); //LOWになっていた時間を表示する
        
        if (7000 < timeCnt) {
          flagA += 1; //7000マイクロ秒より長かったらflagAを増やす
        } else if (500 < timeCnt && timeCnt < 7000) {
          flagB += 1; //500マイクロ秒より長くて7000マイクロ秒より短かかったらflagBを増やす
        } else {
          flagA = 0;
          flagB = 0;
          break; //500マイクロ秒以下だったら、flagはリセットして、whileループを抜ける
        }
      }
    
      if (flagB > 5 && flagA > 0) { //flagAとflagBが一定回数以上になったら
        pullHimo(); //サーボモーターを動かす
        flagA = 0;
        flagB = 0;
        delay(300);
        break; //whileループを抜ける
      }
    }
  }
  
  //諸々の変数をリセットする
  timeWakeup = 0;
  timeSt = 0;
  timeCnt = 0;
  flagA = 0;
  flagB = 0;
  
}
