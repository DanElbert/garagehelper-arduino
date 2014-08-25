#ifndef MomentarySwitch_h
#define MomentarySwitch_h

#include "Arduino.h"
#include "timer.h"

class MomentarySwitch {
  public:
    MomentarySwitch(int pin, long msDelay);
    void update();
    void turnOn();
  private:
    int _pin;
    int _state;
    Timer _timer;
};

#endif
