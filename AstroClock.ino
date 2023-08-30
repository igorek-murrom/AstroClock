#include "led.hpp"
#include "clock.hpp"
#include <GyverTM1637.h>
#include <GyverEncoder.h>

#define MAX_MINUTE_BRIGHT 60
#define DELAY_SET_BLINK 350

Clock c1;
Led led(9, 10, 6);
GyverTM1637 display(2, 3);
Encoder encoder(7, 8, 4, TYPE2);
timeValue curTime, alarmTime, instTime;
byte* values;
bool setAlarm = false, workAlarm = false, setCurTime = false, pointFlag = false;
int difference;
int bright = 0;
uint32_t timerSet, timerDisp;

void setup() {
    Serial.begin(115200);

    timerSet = millis();
    timerDisp = millis();

    led.setColor(255, 190, 0);
    led.on();

    encoder.setDirection(REVERSE);

    display.brightness(5);

    curTime = c1.getTime();
    alarmTime = curTime;
    instTime = curTime;
}

void loop() {
        encoder.tick();

    // обрабатываем энкодер
    if (encoder.isTurn()) {
        if (setAlarm) {
            if (encoder.isRight()) alarmTime.minutes++;
            if (encoder.isLeft()) alarmTime.minutes--;
            if (encoder.isRightH()) alarmTime.hours++;
            if (encoder.isLeftH()) alarmTime.hours--;
            if (encoder.isFastR()) alarmTime.minutes += 5;
            if (encoder.isFastL()) alarmTime.minutes -= 5;
        }
        else if (setCurTime) {
            if (encoder.isRight()) instTime.minutes++;
            if (encoder.isLeft()) instTime.minutes--;
            if (encoder.isRightH()) instTime.hours++;
            if (encoder.isLeftH()) instTime.hours--;
            if (encoder.isFastR()) instTime.minutes += 5;
            if (encoder.isFastL()) instTime.minutes -= 5;
        }
    } else if (encoder.isDouble() and !setAlarm) {
        if (setCurTime) c1.setTime(instTime);
        setCurTime = !setCurTime;
    } else if (encoder.isSingle() and !setCurTime) {
        if (setAlarm) workAlarm = true;
        else workAlarm = false;
        bright = 0;
        setAlarm = !setAlarm;
    }
    encoder.resetStates();

    // инициализируем время резерва и установки или корректируем зарезервированное время
    alarmTime = correctTime(alarmTime);
    instTime = correctTime(instTime);

    // заменяем время показа на зарезервированное или установачное
    if (setAlarm) curTime = alarmTime;
    else if (setCurTime) curTime = instTime;
    else curTime = c1.getTime();

    // обработка точки в зависимости от флагов
    if (setAlarm) pointFlag = true;
    else if (setCurTime) {
        if (millis() - timerSet > DELAY_SET_BLINK) {
            pointFlag = !pointFlag;
            timerSet = millis();
        }
    } else pointFlag = false;

    // раскладываем время на байты
    values = time2byte(curTime);

    // выводим на дисплей время

    if (millis() - timerDisp > 4) {
        display.point(pointFlag, false);
        display.display(values);
        timerDisp = millis();
    }

    // расчитываем время и яркость до будильника
    if (workAlarm and !setCurTime and !setAlarm) {
        difference = differenceMinute(alarmTime, curTime);
        if (difference < MAX_MINUTE_BRIGHT) bright = map(difference, 0, MAX_MINUTE_BRIGHT, 100, 0);
        else workAlarm = false;
        Serial.println(bright);
    }

    // обновляем ленту RGB
    led.setBrightness(bright);
    led.update();
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