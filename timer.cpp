#include "Arduino.h"
#include "timer.h"

Timer::Timer(long msDelay) {
  _msDelay = (unsigned long)msDelay;
}

void Timer::tick() {
  _lastTick = millis();
}

boolean Timer::tock() {
  unsigned long now = millis();
  unsigned long delta = 0;
  
  if (now >= _lastTick) {
    delta = now - _lastTick;
  } else {
    delta = ~((unsigned long)0) - (_lastTick - now);
  }
  
  return delta >= _msDelay;
}
