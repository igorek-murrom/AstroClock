#include "led.hpp"
#include "clock.hpp"
#include <EncButton.h>
#include <GyverTM1637.h>

#define MAX_MINUTE_BRIGHT 60

Led led(9, 10, 6); Clock c1;
GyverTM1637 display(2, 3);
EncButton<EB_TICK, 4, 5, 7> encoder;
timeValue curTime, reservedTime;
byte* values;
bool mode = false, setFlag = false, firstChange = true;
int difference, bright = 0, step;

void setup() {
    Serial.begin(115200);

    led.setColor(255, 190, 0);
    led.on();

    display.brightness(5);
}

void loop() {
    curTime = c1.getTime();

    if (encoder.tick()) {
        if (encoder.click()) {
            setFlag = false;
            if (mode) setFlag = true;
            if (firstChange) reservedTime = curTime;
            bright = 0;
            mode = !mode;
        }
        if (mode) {
            step = 0;
            if (encoder.turn()) {
                if (encoder.left()) step--;
                if (encoder.right()) step++;
                if (encoder.fast()) step *= 5;
                reservedTime.minutes += step;
            }
            if (encoder.turnH()) {
                if (encoder.leftH()) step--;
                if (encoder.rightH()) step++;
                if (encoder.fast()) step *= 3;
                reservedTime.hours += step;
            }
        }
        encoder.resetState();
    }

    if (mode) curTime = reservedTime;

    curTime = correctTime(curTime);
    values = time2byte(curTime);

    display.scroll(values, 100);
    display.point(mode);

    if (setFlag) {
        difference = differenceMinute(reservedTime, curTime);
        if (difference < MAX_MINUTE_BRIGHT) bright = map(difference, 0, MAX_MINUTE_BRIGHT, 100, 0);
    }

    led.setBrightness(bright);
    led.update();
}

timeValue correctTime(timeValue t) {
    if (t.minutes < 0) t.minutes = 59;
    if (t.hours < 0) t.hours = 23;
    t.hours %= 24; t.minutes %= 60;
    return t;
}

byte* time2byte(timeValue t) {
    static byte array[4];
    array[0] = curTime.hours / 10;
    array[1] = curTime.hours % 10;
    array[2] = curTime.minutes / 10;
    array[3] = curTime.minutes % 10;
    return array;
}

int differenceMinute(timeValue reserved, timeValue cur) {
    int resMin, curMin, diff;
    resMin = reserved.hours * 60 + reserved.minutes;
    curMin = cur.hours * 60 + cur.minutes;
    diff = resMin - curMin;
    if (resMin < curMin) diff += 1440;
    return diff;
}