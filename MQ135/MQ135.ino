const int mq135Pin = 34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(mq135Pin);
  if (value < 1000) {
    Serial.println("Fresh Air");
  } else if (value < 2000) {
    Serial.println("Normal Indoor Air");
  } else if (value < 3000) {
    Serial.println("Poor Air Quality (Gas Detected!)");
  } else {
    Serial.println("Hazardous Air!");
  }
  delay(1000);
}
