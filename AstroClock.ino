#include "led.hpp"
#include "clock.hpp"
#include <GyverTM1637.h>
#include <GyverEncoder.h>

#define MAX_MINUTE_BRIGHT 60
#define PIN_LED 5
#define DELAY_SET_BLINK 50

Clock c1;
Led led(9, 10, 6);
GyverTM1637 display(2, 3);
Encoder encoder(7, 8, 4, TYPE2);
timeValue curTime, reservedTime, setTime;
byte* values;
bool mode = false, installFlag = false, firstChange = true, setMode = false, toInstall = false, pointFlag = false;
int difference;
uint8_t bright = 0;
unsigned long timerSet;

void setup() {
    Serial.begin(115200);

    led.setColor(255, 190, 0);
    led.on();

    encoder.setDirection(REVERSE);

    display.brightness(5);

    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
}

void loop() {
    // сбрасываем таймер и считываем время
    timerSet = millis();
    curTime = c1.getTime();

    // обрабатываем энкодер
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

    // инициализируем время резерва и установки или корректируем зарезервированное время
    if (firstChange) {
        reservedTime = curTime;
        setTime = curTime;
    } else {
        reservedTime = correctTime(reservedTime);
    }

    // заменяем время показа на зарезервированное или установачное
    if (mode) {
        curTime = reservedTime;
        toInstall = false;
    } else if (setMode) {
        curTime = setTime;
    }

    // если установка времени завершилась то обновляем время
    if (toInstall) {
        c1.setTime(setTime);
        toInstall = false;
    }

    // раскладываем время на байты
    values = time2byte(curTime);

    // обработка точки в зависимости от флагов
    if (mode) pointFlag = true;
    else if (setMode) {
        if (millis() - timerSet > DELAY_SET_BLINK) {
            pointFlag = !pointFlag;
            timerSet = millis();
        }
    }
    else pointFlag = false;

    // выводим на дисплей время
    display.point(pointFlag, false);
    display.display(values);

    // расчитываем время и яркость до будильника
    if (installFlag and !setMode and !toInstall) {
        difference = differenceMinute(reservedTime, curTime);
        if (difference < MAX_MINUTE_BRIGHT) bright = map(difference, 0, MAX_MINUTE_BRIGHT, 100, 0);
    }

    // обновляем ленту RGB
    led.setBrightness(bright);
    led.update();

    Serial.println(timerSet);
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

    Serial.print("installFlag flag->");
    Serial.print(installFlag);
    Serial.print("   ");

    Serial.print("toInstall flag->");
    Serial.print(toInstall);
    Serial.print("   ");

    Serial.print("firstChange flag->");
    Serial.print(firstChange);
    Serial.print("   ");

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