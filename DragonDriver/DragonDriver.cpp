/*
 *  Arduino library to use with the DragonDriver stepper module
 *  Author: John Glatts
 *  Date: 5/8/19
 *
 *  ToDo
 *      - add fns() for CW and CCW
 *      - Figure out how to get both limit switches in sync
 *          - the front limit is clicked then b/c of the delay, it skips over the second limit switch
 *
 */
#include "Arduino.h"
#include "DragonDriver.h"


/* Initialize the Dragon Driver */
DragonDriver::DragonDriver(int step_pin, int dir_pin, int limit_pin_front, int limit_pin_back) {
    _step_pin = step_pin;
    _dir_pin = dir_pin;
    _limit_pin_front = limit_pin_front;
    _limit_pin_back = limit_pin_back;
}


/* Setup */
void DragonDriver::begin() {
    pinMode(_step_pin, OUTPUT);
    pinMode(_dir_pin, OUTPUT);
    pinMode(_limit_pin_front, INPUT_PULLUP);
    pinMode(_limit_pin_back, INPUT_PULLUP);
}


/* Test the speed */
void DragonDriver::testSpeed() {
    // re-set the pin after the for-loop
    pinMode(_step_pin, OUTPUT);

    // turn motor on and off
    for (int i = 0; i < 3; ++i) {
        analogWrite(_step_pin, 125);
        delay(750);

        // stop the motor shortly
        pinMode(_step_pin, OUTPUT);
        digitalWrite(_step_pin, LOW);
        delay(1000);
    }
    pinMode(_step_pin, OUTPUT);
    digitalWrite(_step_pin, LOW);
    delay(1000);
}


/* Test the Dir, Verifying that front and back movement is working */
void DragonDriver::testDirection() {
    // keep stepping
    for(;;){
        if (hasLimitFrontBeenReached()) {
            digitalWrite(_dir_pin, LOW);
            analogWrite(_step_pin, 100);
            delay(750);
        }
        else if (hasLimitBackBeenReached()) {
            digitalWrite(_dir_pin, HIGH);
            analogWrite(_step_pin, 100);
            delay(750);
        }
        else {
            analogWrite(_step_pin, 0);
        }
    }
}


/* Find home position -- using the front limit switch */
void DragonDriver::findHome() {

    // move the motor away -- to not hit anything
    digitalWrite(_dir_pin, LOW);
    analogWrite(_step_pin, 100);
    delay(500);

    do {
        // slowly find the limit switch
        digitalWrite(_dir_pin, HIGH);
        analogWrite(_step_pin, 100);
        delay(8);
    } while (!hasLimitFrontBeenReached());

    // hold the position
    analogWrite(_step_pin, 0);
}


bool DragonDriver::hasLimitFrontBeenReached() {
    return digitalRead(_limit_pin_front) != HIGH;
}


bool DragonDriver::hasLimitBackBeenReached() {
    return digitalRead(_limit_pin_back) != HIGH;
}


/* Move the stepper clockwise, by the given steps */
void DragonDriver::moveClockWise(int steps){
    // clean this up to a nice for-loop
    if (hasLimitFrontBeenReached()) {
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

    if (hasLimitFrontBeenReached()) {
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
