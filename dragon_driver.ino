/*
 * Author: John Glatts
 * Brief: Arduino code to generate STEP IN signals to send to a Dragon Diver 29A55
 * Date: 3/8/2019
 */

int step_pin = 8;
int dir_pin = 9;
int limit_pin = 10;
int led = LED_BUILTIN;  // unused get rid of this at some point

void setup() {
  pinMode(led, OUTPUT);
  pinMode(limit_pin, INPUT_PULLUP);
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
}


void loop() {
  // playing with the timing
  /*
  for(int i=0; i<1; i++){
    if (digitalRead(limit_pin) != HIGH) {
          // stop the motor
          analogWrite(step_pin, 0);
          delay(3000);
          
    } else {
          analogWrite(step_pin, i);
          delay(5);
    }
  }
  for(int i=1; i>0; i--){
    if (digitalRead(limit_pin) != HIGH) {
          // stop the motor
          analogWrite(step_pin, 0);
          delay(3000);
    } else {
          analogWrite(step_pin, i);
          delay(5);
    }
  }
  delay(50000);
}
  */
  // DIR and STEP test
  digitalWrite(dir_pin, HIGH);
  analogWrite(step_pin, 1);
  delay(5);
  digitalWrite(dir_pin, LOW);
  analogWrite(step_pin, 1);
  delay(5);
}  
