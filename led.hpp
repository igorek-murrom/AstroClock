#ifndef led_h
#define led_h
#include "Arduino.h"
class Led {
public:
    Led(byte pinRED, byte pinGREEN, byte pinBLUE);
    void setColor(int R, int G, int B);
    void setBrightness(int val);
    void on();
    void off();
    bool RGB(int delay);
    void update();
    bool getStatus();
private:
    byte pinR, pinG, pinB;
    int status;
    int curR, curG, curB;
    float curBrightness = 1;
    int num = 1;
    bool stateRGB = true;
    int curRGB[3];
    unsigned long timeRGB;
};
#endif