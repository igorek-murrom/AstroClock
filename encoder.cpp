#include "encoder.hpp"

Encoder::Encoder(byte _pinA, byte _pinB, byte _pinButton) {
    pinA = _pinA; pinB = _pinB; pinButton = _pinButton;
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(pinButton, INPUT_PULLUP);
    AOldState = digitalRead(pinA);
    ButtonOldState = digitalRead(pinButton);
}

int Encoder::update() {
    int returnable = 0;
    AState = digitalRead(pinA);
    if (AState != AOldState) {
        BState = digitalRead(pinB);
        if (AState == BState) value ++;
        else value--;
        AOldState = AState;
        returnable = 1;
    }
    if (ButtonState != ButtonOldState) {
        ButtonOldState = ButtonState;
        if (returnable == 1) returnable = 3;
        else returnable = 2;
    }
    return returnable;
}

void Encoder::setValue(int val) {
    value = val;
}

void Encoder::read(int* encoder, int* button) {
    *encoder = value; *button = !ButtonState;
}