unsigned long timeS;
unsigned long timeCnt;
int pinIR = 4;
int flag = 0;

void setup() {
  pinMode(pinIR, INPUT_PULLUP);
  timeS = 0;
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
}

void loop() {
  while(digitalRead(pinIR) == HIGH){
  }
  Serial.print("HIGH :");
  Serial.println(micros() - timeS);
  timeS = micros();
  while(digitalRead(pinIR) == LOW){
  }
  Serial.print("LOW :");
  Serial.println(micros() - timeS);
  timeS = micros();
  
}
