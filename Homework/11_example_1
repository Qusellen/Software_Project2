#include <Servo.h>

// Arduino pin assignment
#define PIN_LED   9   // LED active-low
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servo motor

// configurable parameters for sonar
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 180.0  // minimum distance to be measured (unit: mm) (18cm)
#define _DIST_MAX 360.0   // maximum distance to be measured (unit: mm) (36cm)[cite: 2]

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

#define _EMA_ALPHA 0.3    // EMA weight of new sample (range: 0 to 1)

// Target Distance
#define _TARGET_LOW  250.0
#define _TARGET_HIGH 290.0

// duty duration for myservo.writeMicroseconds()
#define _DUTY_MIN 500  // servo full clockwise position (0 degree)[cite: 2]
#define _DUTY_NEU 1500 // servo neutral position (90 degree)[cite: 2]
#define _DUTY_MAX 2400 // servo full counterclockwise position (180 degree)[cite: 2]

// global variables
float  dist_ema, dist_prev = _DIST_MAX; // unit: mm
unsigned long last_sampling_time;       // unit: ms

Servo myservo;

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);    // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);     // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 

  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);

  // initialize USS related variables
  dist_prev = _DIST_MIN; 

  // initialize serial port
  Serial.begin(57600);
}

void loop() {
  float  dist_raw, dist_filtered;
  
  // wait until next sampling time.
  if (millis() < last_sampling_time + INTERVAL)
    return;

  // get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  // the range filter & LED control
  if ((dist_raw == 0.0) || (dist_raw > _DIST_MAX)) {
      dist_filtered = dist_prev;
      digitalWrite(PIN_LED, HIGH); // 범위를 벗어나면 LED OFF (Active-low)
  } else if (dist_raw < _DIST_MIN) {
      dist_filtered = dist_prev;
      digitalWrite(PIN_LED, HIGH); // 범위를 벗어나면 LED OFF (Active-low)
  } else {    // In desired Range (18cm ~ 36cm)[cite: 2]
      dist_filtered = dist_raw;
      dist_prev = dist_raw;
      digitalWrite(PIN_LED, LOW);  // 범위 내에 들어오면 LED ON (Active-low)[cite: 2]
  }

  // EMA filter equation implementation
  dist_ema = (_EMA_ALPHA * dist_filtered) + ((1.0 - _EMA_ALPHA) * dist_ema);

  // adjust servo position according to the USS read value
  int servo_angle;
  if (dist_ema <= _DIST_MIN) {
    servo_angle = 0;   // 18cm 이하: 0도[cite: 2]
  } else if (dist_ema >= _DIST_MAX) {
    servo_angle = 180; // 36cm 이상: 180도[cite: 2]
  } else {
    // 18cm ~ 36cm 사이: 거리에 비례하여 0도 ~ 180도 사이 연속 변화[cite: 2]
    servo_angle = map(dist_ema, _DIST_MIN, _DIST_MAX, 0, 180);
  }

  // Convert angle (0-180) to servo duty microseconds (_DUTY_MIN ~ _DUTY_MAX)
  int duty = map(servo_angle, 0, 180, _DUTY_MIN, _DUTY_MAX);
  myservo.writeMicroseconds(duty);

  // output the distance to the serial port
  Serial.print("Min:");    Serial.print(_DIST_MIN);
  Serial.print(",Low:");   Serial.print(_TARGET_LOW);
  Serial.print(",dist:");  Serial.print(dist_ema);
  Serial.print(",Servo:"); Serial.print(servo_angle);  
  Serial.print(",High:");  Serial.print(_TARGET_HIGH);
  Serial.print(",Max:");   Serial.print(_DIST_MAX);
  Serial.println("");
 
  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
