// It sends the value read by a Potentiometer (A5) 

const int pot = A5;

void setup() {
  pinMode(pot, INPUT);
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(pot);
  Serial.println(value);
  delay(1000);
}
