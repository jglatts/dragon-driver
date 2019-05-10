/*
 *  Arduino library to use with the DragonDriver stepper module
 *  Author: John Glatts
 *  Date: 5/8/19
 *
 */

#include "Arduino.h"

class DragonDriver {
public:
    DragonDriver(int step_pin, int dir_pin, int limit_pin);
    void begin();
    void moveClockWise(int steps);
    void moveCounterClockWise(int steps);
    void stop();
    bool hasLimitBeenReached();
private:
    int _step_pin;
    int _dir_pin;
    int _limit_pin;
};