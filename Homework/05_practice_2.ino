oid setup() {
  pinMode(7, OUTPUT);
}

void loop() {

  // 1 секунд асаалттай
  digitalWrite(7, LOW);
  delay(1000);

  // 1 секундийн турш 5 удаа анивчина
  for (int i = 0; i < 5; i++) {
    digitalWrite(7, HIGH);
    delay(100);

    digitalWrite(7, LOW);
    delay(100);
  }

  // эцэст нь унтарна
  digitalWrite(7, HIGH);

  // дахиж ажиллахгүй
  while (true) {
  }
}
