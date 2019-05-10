/*
 *  Arduino library to use with the DragonDriver stepper module
 *  Author: John Glatts
 *  Date: 5/8/19
 *
 *  ToDo
 *      - add fns() for CW and CCW
 *
 */
#include "Arduino.h"
#include "DragonDriver.h"


/* Initialize the Dragon Driver */
DragonDriver::DragonDriver(int step_pin, int dir_pin, int limit_pin) {
    _step_pin = step_pin;
    _dir_pin = dir_pin;
    _limit_pin = limit_pin;

}


/* Setup */
void DragonDriver::begin() {
    pinMode(_step_pin, OUTPUT);
    pinMode(_dir_pin, OUTPUT);
    pinMode(_limit_pin, INPUT_PULLUP);
}


/* Move the stepper clockwise, by the given steps */
void DragonDriver::moveClockWise(int steps){
    // clean this up to a nice for-loop
    if (hasLimitBeenReached()) {
        // send motor back
        analogWrite(_step_pin, 0); // stop the motor
        delay(900);
        digitalWrite(_dir_pin, HIGH); // send motor back
        analogWrite(_step_pin, steps);
        delay(2000);

    } else {
        digitalWrite(_dir_pin, LOW);
        analogWrite(_step_pin, steps);
        delay(1000);
        analogWrite(_step_pin, steps);
        delay(900);  // small step
    }
    analogWrite(_step_pin, 0);
    delay(1000);
}


/* Move the stepper counter-clockwise, by the given steps */
void DragonDriver::moveCounterClockWise(int steps){
    if (hasLimitBeenReached()) {
        // send motor back
        analogWrite(_step_pin, 0); // stop the motor
        delay(900);
        digitalWrite(_dir_pin, LOW); // send motor back
        analogWrite(_step_pin, steps);
        delay(2000);

    } else {
        digitalWrite(_dir_pin, HIGH);
        analogWrite(_step_pin, steps);
        delay(1000);
        analogWrite(_step_pin, steps);
        delay(900);  // small step
    }
    analogWrite(_step_pin, 0);
    delay(1000);
}

/* change this xD */
bool DragonDriver::hasLimitBeenReached() {
    // add more to this
    return digitalRead(_limit_pin == HIGH); // debug this --> hopefully returns true or false
}

