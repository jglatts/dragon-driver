/*
 *  Arduino library to use with the DragonDriver stepper module
 *  Author: John Glatts
 *  Date: 5/8/19
 *
 */

#include "Arduino.h"

class DragonDriver {
public:
    DragonDriver(int step_pin, int dir_pin_front, int limit_pin_front, int limit_pin_back);
    void begin(),
         stop(),
         testSpeed(),
         testDirection(),
         testLimits(),
         moveClockWise(int steps),
         moveCounterClockWise(int steps);
    bool hasLimitFrontBeenReached();
    bool hasLimitBackBeenReached();
private:
    int _step_pin;
    int _dir_pin;
    int _limit_pin_front;
    int _limit_pin_back;
};
