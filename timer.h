#ifndef Timer_h
#define Timer_h

#include "Arduino.h"

class Timer {
  public:
    Timer(long msDelay);
    void tick();
    boolean tock();
  private:
    unsigned long _msDelay;
    unsigned long _lastTick;
};

#endif
