#define PIN_LED 13
unsigned int count, toggle; 
int toggle_state(int toggle) { // 함수 선언 위치 오류: 표준 C++에서는 loop() 함수 내에서 호출하기 전에 toggle_state() 함수가 먼저 선언되거나 정의되어 있어야 함.
  return !toggle; // 논리적 오류: toggle_state() 함수가 전달받은 값을 반전시키지 않고 그대로 반환하고 있어서 "!" 썼음.
}

void setup() {
  pinMode(PIN_LED, OUTPUT); // 코드 끝에 세미콜론이 빠져 있슴.
  Serial.begin(115200); 
  while (!Serial) {
    ;
  }
  Serial.println("Hello World!"); // 대소문자 오류.
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle); 
}

void loop() {
  Serial.println(++count);
  toggle = toggle_state(toggle); // 주석 문법 오류: 코드 뒤의 설명 텍스트 앞에 주석 기호.
  digitalWrite(PIN_LED, toggle);
  delay(1000);
}
