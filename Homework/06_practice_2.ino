#define LED_PIN 7

int current_period = 1000; 
int current_duty = 0;      

void set_period(int period) {
  if (period < 100) period = 100;
  if (period > 10000) period = 10000;
  current_period = period;
}

void set_duty(int duty) {
  if (duty < 0) duty = 0;
  if (duty > 100) duty = 100;
  current_duty = duty;
}

void generate_pwm() {
  long high_time = (long)current_period * current_duty / 100; 
  long low_time = current_period - high_time;
  if (high_time > 0) {
    digitalWrite(LED_PIN, LOW);
    delayMicroseconds(high_time);
  }

  if (low_time > 0) {
    digitalWrite(LED_PIN, HIGH);
    delayMicroseconds(low_time);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  set_period(1000); //10ms (10000us), 1ms (1000us), 0.1ms (100us)
}

void loop() {
  long step_duration = 10000;
  for (int d = 0; d <= 100; d += 2) {
    set_duty(d);
    unsigned long start_time = micros();
    while (micros() - start_time < step_duration) {
      generate_pwm();
    }
  }
  
  for (int d = 100; d >= 0; d -= 2) {
    set_duty(d);
    unsigned long start_time = micros();
    while (micros() - start_time < step_duration) {
      generate_pwm();
    }
  }
}
