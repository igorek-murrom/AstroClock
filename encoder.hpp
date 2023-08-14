#ifndef encoder_h
#define encoder_h
#include "Arduino.h"
class Encoder {
public:
    Encoder(byte _pinA, byte _pinB, byte _pinButton);
    int update();
    void setValue(int val);
    void read(int* encoder, int* button);
private:
    byte pinA, pinB, pinButton;
    int AState, AOldState, BState;
    int ButtonState, ButtonOldState;
    int value = 0;
};
#endif