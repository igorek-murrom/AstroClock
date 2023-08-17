#include "led.hpp"

Led::Led(byte pinRED, byte pinGREEN, byte pinBLUE) {
    pinR = pinRED;
    pinG = pinGREEN;
    pinB = pinBLUE;
    timeRGB = millis();
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);
    off();
}

void Led::update() {
    analogWrite(pinR, 255 - (curR * curBrightness * status));
    analogWrite(pinG, 255 - (curG * curBrightness * status));
    analogWrite(pinB, 255 - (curB * curBrightness * status));
}

bool Led::getStatus() {
    if (status == 1) return true;
    return false;
}

void Led::setColor(int R, int G, int B) {
    R = constrain(R, 0, 255);
    G = constrain(G, 0, 255);
    B = constrain(B, 0, 255);

    curR = R;
    curG = G;
    curB = B;
}

void Led::setBrightness(int percent) {
    percent = constrain(percent, 0, 100);
    curBrightness = percent / 100.0;
}

void Led::on() {
    status = 1;
}

void Led::off() {
    status = 0;
}

bool Led::RGB(int delay) {
    if (millis() - timeRGB < delay) return false;
    else timeRGB = millis();

    curRGB[0] = curR;
    curRGB[1] = curG;
    curRGB[2] = curB;

    if (num < 0) num += 3;

    if (stateRGB) {
        if (curRGB[num] == 255) {
            num -= 1;
            num %= 3;
            stateRGB = !stateRGB;
            return true;
        } else curRGB[num] += 1;
    } else {
        if (curRGB[num] == 0) {
            num += 2;
            num %= 3;
            stateRGB = !stateRGB;
            return true;
        } else curRGB[num] -= 1;
    }

    curR = curRGB[0];
    curG = curRGB[1];
    curB = curRGB[2];
    return false;
}