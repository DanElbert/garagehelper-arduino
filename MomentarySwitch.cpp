#include "Arduino.h"
#include "MomentarySwitch.h"

MomentarySwitch::MomentarySwitch(int pin, long msDelay): _pin(pin), _timer(msDelay), _state(HIGH) {
}

void MomentarySwitch::update() {
  if (_state == LOW && _timer.tock()) {
    _state = HIGH;
    digitalWrite(_pin, _state);
  }
}

void MomentarySwitch::turnOn() {
  if (_state == HIGH) {
    _state = LOW;
    digitalWrite(_pin, _state);
  }
  
  _timer.tick();
}
