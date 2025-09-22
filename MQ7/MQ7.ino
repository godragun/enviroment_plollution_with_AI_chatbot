const int mq7Digital = 25;

void setup() {
  Serial.begin(115200);
  pinMode(mq7Digital, INPUT);
}

void loop() {
  int state = digitalRead(mq7Digital);
  if (state == HIGH) {
    Serial.println("CO Detected!");
  } else {
    Serial.println("Clean Air");
  }
  delay(500);
}
