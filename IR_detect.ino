unsigned long timeSt;
int pinIR = 4;

void setup() {
  pinMode(pinIR, INPUT_PULLUP);
  timeSt = 0;
  Serial.begin(9600);
}

void loop() {
  timeSt = micros();
  while(digitalRead(pinIR) == HIGH){}
  Serial.print("HIGH: ");
  Serial.println(micros() - timeSt);

  timeSt = micros();  
  while(digitalRead(pinIR) == LOW){}
  Serial.print("LOW: ");
  Serial.println(micros() - timeSt);
}
