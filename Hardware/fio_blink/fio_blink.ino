int incomingByte = 0;

void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);

  // blink twice at start up
  digitalWrite(13, LOW);
  delay(1000);

  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

   if (incomingByte == '0') {
      digitalWrite(13, LOW);
    } else if (incomingByte == '1') {
     digitalWrite(13, HIGH);
    }

  Serial.print("Fio received: ");
  Serial.write(incomingByte);
  Serial.write(10);
  }
}
