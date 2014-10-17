#ifndef Switch_h
#define Switch_h

#include "Arduino.h"
#include "Timer.h"

class Switch {
  public:
    Switch(int pin, long msDelay);
    void update();
    boolean read();
  private:
    int _pin;
    boolean _waiting;
    int _state;
    Timer _timer;
};

#endif
