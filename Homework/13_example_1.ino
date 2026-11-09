#include <Servo.h>

// Arduino pin assignment
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10

// configurable parameters for sonar
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 180.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 360.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

// configurable parameters for Servo
#define _DUTY_MIN 500  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1500 // servo neutral position (90 degree)
#define _DUTY_MAX 2400 // servo full counter-clockwise position (180 degree)

#define _POS_START _DUTY_MIN    // servo start position (0도)
#define _POS_END   _DUTY_MAX    // servo end position (180도)

// 💡 Энд хурдаа өөрчилж туршина:
// - 60 секундын турш 180° явах бол: 3 (эсвэл 3.0)
// - 300 секундын турш (5 минут) явах бол: 0.3
#define _SERVO_SPEED 3.0 

// Loop Interval
#define INTERVAL 20   // servo update interval (unit: msec)

// global variables
unsigned long last_sampling_time; // unit: msec
Servo myservo;

float duty_change_per_interval; // float төрөр болгож өөрчлөв (0 болохоос сэргийлнэ)
float duty_target;    // Target duty time
float duty_curr;      // Current duty time

int toggle_interval, toggle_interval_cnt;

void setup() {
  // initialize GPIO pins
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 

  myservo.attach(PIN_SERVO); 
  
  duty_target = duty_curr = _POS_START;
  myservo.writeMicroseconds((int)duty_curr);
  
  // initialize serial port
  Serial.begin(57600);  

  // convert angular velocity into duty change per interval (float тооцоолол)
  duty_change_per_interval = 
    (float)(_DUTY_MAX - _DUTY_MIN) * (_SERVO_SPEED / 180.0) * ((float)INTERVAL / 1000.0);
  
  // initialize variables for servo update.
  toggle_interval = (int)((180.0 / _SERVO_SPEED) * 1000.0 / INTERVAL);
  toggle_interval_cnt = toggle_interval;
  
  // initialize last sampling time
  last_sampling_time = 0;
}

void loop() {
  float dist_raw;

  // wait until next sampling time. 
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  // adjust duty_curr toward duty_target by duty_change_per_interval
  if (duty_target > duty_curr) {
    duty_curr += duty_change_per_interval;
    if (duty_curr > duty_target)
        duty_curr = duty_target;
  } else {
    duty_curr -= duty_change_per_interval;
    if (duty_curr < duty_target)
      duty_curr = duty_target;
  }

  // update servo position
  myservo.writeMicroseconds((int)duty_curr);

  // output the read value to the serial port
  Serial.print("Min:0");
  Serial.print(",duty_target:"); Serial.print(duty_target);
  Serial.print(",duty_curr:");   Serial.print(duty_curr);
  Serial.print(",dist_raw:");   Serial.print(dist_raw);
  Serial.println(",Max:3000");

  // toggle duty_target between _DUTY_MIN and _DUTY_MAX.
  if (toggle_interval_cnt >= toggle_interval) {
    toggle_interval_cnt = 0;
    if (duty_target == _POS_START)
      duty_target = _POS_END;
    else
      duty_target = _POS_START;
  } else {
    toggle_interval_cnt++;
  }

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
