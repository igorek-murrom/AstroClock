#include "led.hpp"
#include "clock.hpp"
// #include <EncButton.h>
#include <GyverTM1637.h>
#include <GyverEncoder.h>

#define MAX_MINUTE_BRIGHT 60
#define PIN_LED 5

Clock c1;
Led led(9, 10, 6);
GyverTM1637 display(2, 3);
Encoder encoder(7, 8, 4, TYPE2);
// EncButton<EB_TICK, 7, 8, 5> encoder;
timeValue curTime, reservedTime, setTime;
byte* values;
bool mode = false, installFlag = false, firstChange = true, setMode = false, toInstall = false;
int difference;
uint8_t bright = 0;

void setup() {
    Serial.begin(115200);

    led.setColor(255, 190, 0);
    led.on();
    encoder.setDirection(REVERSE);
    display.brightness(5);
}

void loop() {
    curTime = c1.getTime();

    encoder.tick();
    if (mode and encoder.isTurn() and !firstChange) {
        if (encoder.isRight()) reservedTime.minutes++;
        if (encoder.isLeft()) reservedTime.minutes--;
        if (encoder.isRightH()) reservedTime.hours++;
        if (encoder.isLeftH()) reservedTime.hours--;
        if (encoder.isFastR()) reservedTime.minutes += 5;
        if (encoder.isFastL()) reservedTime.minutes -= 5;
    } else if (setMode and encoder.isTurn()) {
        if (encoder.isRight()) setTime.minutes++;
        if (encoder.isLeft()) setTime.minutes--;
        if (encoder.isRightH()) setTime.hours++;
        if (encoder.isLeftH()) setTime.hours--;
        if (encoder.isFastR()) setTime.minutes += 5;
        if (encoder.isFastL()) setTime.minutes -= 5;
    }
    if (encoder.isDouble() and !mode) {
        if (setMode) toInstall = true;
        setMode = !setMode;
    } else if (encoder.isSingle() and !setMode) {
        installFlag = false;
        if (mode) installFlag = true;
        bright = 0;
        firstChange = false;
        mode = !mode;
    }
    encoder.resetStates();

    if (firstChange) {
        reservedTime = curTime;
        setTime = curTime;
    } else {
        reservedTime = correctTime(reservedTime);
    }

    if (mode) {
        curTime = reservedTime;
        toInstall = false;
    } else if (setMode) {
        curTime = setTime;
    }

    if (toInstall) {
        c1.setTime(setTime);
        toInstall = false;
    }

    values = time2byte(curTime);

    // display.scroll(values, 100);
    // display.point(mode);
    blinkLed(setMode);
    
    if (installFlag and !setMode and !toInstall) {
        difference = differenceMinute(reservedTime, curTime);
        if (difference < MAX_MINUTE_BRIGHT) bright = map(difference, 0, MAX_MINUTE_BRIGHT, 100, 0);
    }

    led.setBrightness(bright);
    led.update();

    debug();
}

timeValue correctTime(timeValue t) {
    t.hours = (t.hours % 24 + 24) % 24;
    t.minutes = (t.minutes % 60 + 60) % 60;
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

void debug() {
    Serial.print("mode flag->");
    Serial.print(mode);
    Serial.print("   ");

    Serial.print("setMode flag->");
    Serial.print(setMode);
    Serial.print("   ");

    // Serial.print("installFlag flag->");
    // Serial.print(installFlag);
    // Serial.print("   ");

    // Serial.print("toInstall flag->");
    // Serial.print(toInstall);
    // Serial.print("   ");

    // Serial.print("firstChange flag->");
    // Serial.print(firstChange);
    // Serial.print("   ");

    Serial.print("current time->");
    Serial.print(curTime.hours);
    Serial.print(":");
    Serial.print(curTime.minutes);
    Serial.print("   ");

    Serial.print("reserved time->");
    Serial.print(reservedTime.hours);
    Serial.print(":");
    Serial.print(reservedTime.minutes);
    Serial.print("   ");

    Serial.print("difference->");
    Serial.print(difference);
    Serial.print("   ");

    Serial.print("bright->");
    Serial.print(bright);
    Serial.println();
}

void blinkLed(bool mode) {
    int value;
    if (mode) value = 255;
    else value = 0;
    analogWrite(PIN_LED, value);
}