/*
 * Author: John Glatts
 * Brief: Arduino code to generate STEP IN signals to send to a Dragon Diver 29A55
 * Date: 3/8/2019
 */

int step_pin = 8;
int dir_pin = 9;
int limit_pin = 10;

void setup() {
  pinMode(limit_pin, INPUT_PULLUP);
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
}


void loop() {
  // add limit switch next
  digitalWrite(dir_pin, HIGH);
  analogWrite(step_pin, 64);
  delay(900);
  digitalWrite(dir_pin, LOW);
  analogWrite(step_pin, 0);
  delay(9000);
  digitalWrite(dir_pin, LOW);
  analogWrite(step_pin, 64);
  delay(100);  // small step
  digitalWrite(dir_pin, HIGH);
  analogWrite(step_pin, 0);
  delay(900);
}
