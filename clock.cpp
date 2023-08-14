#include "clock.hpp"

Clock::Clock() {
    Wire.begin();
}

void Clock::setTime(timeValue time) {
    c1.setClockMode(false);
    c1.setHour(time.hours);
    c1.setMinute(time.minutes);
    c1.setSecond(time.seconds);
}
    
timeValue Clock::getTime() {
    timeValue time;
    time.hours = c1.getHour(h12Flag, pmFlag);
    time.minutes = c1.getMinute();
    time.seconds = c1.getSecond();
    return time;
}

void Clock::update(timeValue time) {
    // continue
}