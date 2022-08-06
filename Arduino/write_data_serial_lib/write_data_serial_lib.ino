// It turns the led8 ON for 2sec if it receives the string "Hello, World"

const int LED = 8;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
    String incomingString = Serial.readString();
    if (incomingString == "Hello, World")
    {
      digitalWrite(LED, HIGH);
      delay(2000);
    }
  }
  digitalWrite(LED, LOW);
  delay(1000);
}
