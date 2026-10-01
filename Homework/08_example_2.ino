// 핀 설정 (이전 실습과 동일하다고 가정)
#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_LED 9

// 거리 제한 값 설정
#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

// 샘플링 주기를 25ms로 변경
#define INTERVAL 25 

unsigned long last_sampling_time = 0; // 단위: msec

// 거리 측정 함수 (기본 제공 함수)
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  // 수신된 초음파의 시간을 마이크로초 단위로 측정
  long duration = pulseIn(ECHO, HIGH, 50000); 
  if (duration == 0) return 0.0;
  
  // 시간을 밀리미터(mm)로 변환 (음속 340m/s)
  return (duration * 340.0) / 2.0 / 1000.0; 
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  digitalWrite(PIN_TRIG, LOW);
  pinMode(PIN_ECHO, INPUT);
  
  digitalWrite(PIN_LED, HIGH); // 초기 상태에서 LED 끄기 (Active low이므로 HIGH/255 적용)
  Serial.begin(57600); // 시리얼 모니터 및 플로터 통신 속도
}

void loop() {
  float distance;
  int led_brightness = 255; // 기본값: 완전히 꺼짐 (Active Low)

  // Polling: 다음 샘플링 시간까지 대기
  if (millis() < (last_sampling_time + INTERVAL)) {
    return;
  }

  // 거리 측정
  distance = USS_measure(PIN_TRIG, PIN_ECHO);

  // --- 거리에 따른 LED 밝기 계산 ---
  if (distance >= 100.0 && distance <= 200.0) {
    // 100mm(255: 꺼짐) -> 200mm(0: 최대 밝기) 구간 선형 비례
    led_brightness = 255 - (int)((distance - 100.0) * 255.0 / 100.0);
  } 
  else if (distance > 200.0 && distance <= 300.0) {
    // 200mm(0: 최대 밝기) -> 300mm(255: 꺼짐) 구간 선형 비례
    led_brightness = (int)((distance - 200.0) * 255.0 / 100.0);
  } 
  else {
    // 100mm 미만 또는 300mm 초과 거리에서는 꺼짐
    led_brightness = 255; 
  }

  // 계산 오류 방지를 위해 PWM 값을 0에서 255 사이로 제한
  led_brightness = constrain(led_brightness, 0, 255);

  // LED에 밝기 값 출력
  analogWrite(PIN_LED, led_brightness);

  // 시리얼 플로터 출력을 위한 부분
  Serial.print("Min:"); Serial.print(_DIST_MIN);
  Serial.print(", distance:"); Serial.print(distance);
  Serial.print(", Max:"); Serial.print(_DIST_MAX);
  Serial.println("");

  // 마지막 샘플링 시간 업데이트
  last_sampling_time += INTERVAL;
}
