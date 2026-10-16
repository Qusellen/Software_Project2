#include <Servo.h>
#include <math.h>

//pin settings
#define PIN_SERVO 10
#define PIN_TRIG 12
#define PIN_ECHO 13
#define CAR_DETECT_DISTANCE 150 //machine sensing distance: 150mm (15sm)

Servo myServo;
bool isBarrierOpen = false; // Barrier status

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  myServo.attach(PIN_SERVO);
  myServo.write(0); //move to the closed position
  
  Serial.begin(57600);
  delay(1000); //wait
}

void loop() {
  float distance = USS_measure(PIN_TRIG, PIN_ECHO);

  if (distance == 0.0 || distance > 400.0) {
    delay(50);
    return;
  }
  
  //Check if the car has come within 15 cm.
  bool carDetected = (distance <= CAR_DETECT_DISTANCE);

  if (carDetected && !isBarrierOpen) {
    Serial.println("Car detected! Opening barrier (Sigmoid)...");
    moveBarrierSigmoid(0, 90); //Open using the sigmoid function.
    isBarrierOpen = true;
    delay(2000); // wait for the car to pass completely
  } 
  else if (!carDetected && isBarrierOpen) {
    Serial.println("Car passed! Closing barrier (Sine)...");
    moveBarrierSine(90, 0); // Close using the sine function
    isBarrierOpen = false;
    delay(1000);
  }

  delay(50);
}

// distance reading function from ultrasonic sensor
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, 25000) * 0.173;
}

// smoothly moving the shutter using a sigmoid function
void moveBarrierSigmoid(int startAngle, int stopAngle) {
  for(int i = 0; i <= 50; i++) {
    float x =- 6.0 + (12.0 * i / 50.0);
    float sigmoidValue = 1.0 / (1.0 + exp(-x));
    int angle = startAngle + (stopAngle - startAngle) * sigmoidValue;

    myServo.write(angle);
    delay(30);
  }
}

// Smoothly moving the shutter using a sine function.
void moveBarrierSine(int startAngle, int stopAngle) {
  for (int i = 0; i <= 50; i++) {
    float x = (float)i / 50.0;
    float sineValue = 0.5 * (1.0 - cos(PI * x)); // Sine ease-in-out formula
    int angle = startAngle + (stopAngle - startAngle) * sineValue;
    
    myServo.write(angle);
    delay(30);
  }
}
