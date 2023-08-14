#include "led.hpp"
#include "clock.hpp"
#include "encoder.hpp"

Led led(9, 10, 6); Clock c1; Encoder encoder(4, 5, 7);
timeValue t;
TM1637Display display(2, 3);

void setup() {

    Serial.begin(115200);
    // t.hours = 12; t.minutes = 27; t.seconds = 45;
    // c1.setTime(t);
    led.setBrightness(15);
    led.setColor(255,255,255);
    led.on();
}

void loop() {
    led.RGB(5);
    led.update();
}