#ifndef clock_h
#define clock_h
#include "Arduino.h"
#include <DS3231.h>
#include <TM1637Display.h>
struct timeValue {
    int hours;
    int minutes;
    int seconds;
};

class Clock {
public:
    Clock();
    void setTime(timeValue time);
    timeValue getTime();
    void update(timeValue time);
private:
    DS3231 c1;
    bool h12Flag, pmFlag;

};
#endif
