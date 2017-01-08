#include <Servo.h>
#include <avr/sleep.h> //スリープするためのライブラリ

Servo servo;
unsigned long timeWakeup = 0; //wakeupした時間を入れておく変数
unsigned long timeSt = 0; //pinIRがLOWになった時間を入れておく変数
unsigned long timeCnt = 0; //pinIRがどれだけの間LOWだったかの時間を入れておく変数
int pinInt = 2; //割り込み用のピン番号
int pinIR = 4; //赤外線受信モジュールの信号を読み取るピン番号
int pinSv = 3; //サーボモーターに信号を送るピン番号
int pinLed = 6; //LED光らせるピン番号
int flagA = 0; //5000マイクロ秒より長い赤外線を受信したことを記録しておく変数
int flagB = 0; //500マイクロ秒より長くて5000マイクロ秒より短い赤外線を受信したことを記録しておく変数

//サーボモーターを動かす関数
void pullHimo(){
  Serial.println("pull");
  servo.attach(pinSv);
  servo.write(0);
  servo.write(120);
  delay(300);
  servo.write(0);
  delay(300);
  servo.detach();
}

//LEDをチカっと光らせる関数
void ledChika(){
  digitalWrite(pinLed, LOW);
  digitalWrite(pinLed, HIGH);
  delay(1);
  digitalWrite(pinLed, LOW);
}

//スリープから復帰した時に実行する処理
void wakeup(){
  noInterrupts(); //割り込み検出を無効にする
}

void setup(){
  pinMode(pinIR, INPUT_PULLUP);
  pinMode(pinSv, OUTPUT);
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(0, wakeup, FALLING); //割り込み番号0(2番ピン)、復帰時にはwakeup()を実行、ピン電圧がFALLING(HIGH→LOW)の時割り込み発生
  pinMode(pinInt, INPUT_PULLUP); //割り込み用ピンをプルアップに設定
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); //スリープモードを設定
}

void loop(){
  Serial.println("wakeup");
  timeWakeup = millis();
  Serial.println(timeWakeup);

  flagA = 0;
  flagB = 0;
  while(millis() - timeWakeup < 200){ //wakeupしてから200ミリ秒経過するまでループを継続する
    if(digitalRead(pinIR) == LOW){
      timeSt = micros();
      while(digitalRead(pinIR) == LOW){
        timeCnt = micros() - timeSt; //LOWになっている時間を計測する
      }
      
      Serial.print("timeCnt:");
      Serial.println(timeCnt); //LOWになっていた時間を表示する
      
      if(timeCnt > 5000){
        flagA += 1; //5000マイクロ秒より長かったらflagAを増やす
      }else if(500 < timeCnt && timeCnt < 5000){
        flagB += 1; //500マイクロ秒より長くて5000マイクロ秒より短かかったらflagBを増やす
      }else{
        flagA = 0; //条件に合わない長さ(500マイクロ秒以下)だったら、flagはリセットして、whileループを抜ける
        flagB = 0;
        break;
      }
      
      timeCnt = 0;
    }
  
    if(flagB > 5 && flagA > 0){ //flagAとflagBが一定回数以上になったら
      ledChika(); //LEDをチカっとさせて
      pullHimo(); //サーボモーターを動かす
      flagA = 0;
      flagB = 0;
      break; //whileループを抜ける
    }
  }
  
  //もろもろの変数をリセットする
  timeCnt = 0;
  timeSt = 0;
  timeWakeup = 0;
  flagA = 0;
  flagB = 0;

  //ここまで来ちゃったらスリープする
  Serial.println("sleep");
  Serial.flush();
  interrupts(); //割り込みを有効にする
  sleep_mode(); //スリープする
}
