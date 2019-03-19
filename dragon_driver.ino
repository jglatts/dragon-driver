/*
 * Author: John Glatts
 * Brief: Arduino code to generate STEP IN signals to send to a Dragon Diver 29A55
 * Date: 3/8/2019
 *
 */

int step_pin = 8;
int dir_relay = 9;
int limit_pin = 10;

void setup() {
    pinMode(limit_pin, INPUT_PULLUP);
    pinMode(step_pin, OUTPUT);
    pinMode(dir_relay, OUTPUT);
}


void loop() {
    // keep stepping
    for(int i=0;; i++){
        if (digitalRead(limit_pin) != HIGH) {
            // stop the motor
            digitalWrite(dir_relay, HIGH);
            analogWrite(step_pin, 0);
            delay(1000);
            analogWrite(step_pin, 0);
            delay(1400);
        } else {
            digitalWrite(dir_relay, LOW);
            analogWrite(step_pin, 64);
            delay(200);
        }
    }
}
