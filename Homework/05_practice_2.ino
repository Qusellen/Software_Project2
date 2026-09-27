void setup() {
  pinMode(7, OUTPUT);
}

void loop() {
  digitalWrite(7, LOW); //// 1초 동안 켜짐 (Active-Low 방식: LOW = ON)
  delay(1000);

  for (int i = 0; i < 5; i++) { //// 1초 동안 5번 깜빡임
    digitalWrite(7, HIGH);
    delay(100);

    digitalWrite(7, LOW);
    delay(100);
  }
  digitalWrite(7, HIGH); //// 마지막에 꺼짐 (Active-Low 방식: HIGH = OFF)
  while (true); //// 더 이상 실행되지 않음
}
