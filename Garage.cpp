#include "Arduino.h"
#include "Garage.h"

Garage::Garage(int lightPin, int garageOpenerPin, int bigDoorPin, int backDoorPin, int basementDoorPin) :
  _light(lightPin, LightDelay),
  _garageOpener(garageOpenerPin, GarageOpenerDelay),
  _bigDoor(bigDoorPin, BounceDelay),
  _backDoor(backDoorPin, BounceDelay),
  _basementDoor(basementDoorPin, BounceDelay)
{
}

void Garage::update() {
  _light.update();
  _garageOpener.update();
  _bigDoor.update();
  _backDoor.update();
  _basementDoor.update();

  if (isAnyDoorOpen()) {
    turnOnLight();
  }
}

boolean Garage::isBigDoorOpen() {
  return !_bigDoor.read();
}

boolean Garage::isBackDoorOpen() {
  return !_backDoor.read();
}

boolean Garage::isBasementDoorOpen() {
  return !_basementDoor.read();
}

boolean Garage::isAnyDoorOpen() {
  return (isBigDoorOpen() || isBackDoorOpen() || isBasementDoorOpen());
}

void Garage::turnOnLight() {
  _light.turnOn();
}

void Garage::pressGarageOpener() {
  _garageOpener.turnOn();
}
