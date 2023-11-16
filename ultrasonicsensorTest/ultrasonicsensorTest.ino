const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

float duration_us, distance_cm;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH); // measure duration of pulse from ECHO pin
  distance_cm = 0.017 * duration_us; // calculate the distance: 0.034cm/μs devided by 2

  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
}