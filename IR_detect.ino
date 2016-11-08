unsigned long timeS;
int pinIR = 4;

void setup() {
  pinMode(pinIR, INPUT_PULLUP);
  timeS = 0;
  Serial.begin(9600);
}

void loop() {
  while(digitalRead(pinIR) == HIGH){}
  
  Serial.print("HIGH :");
  Serial.println(micros() - timeS);
  timeS = micros();
  
  while(digitalRead(pinIR) == LOW){}
  
  Serial.print("LOW :");
  Serial.println(micros() - timeS);
  timeS = micros();
}
