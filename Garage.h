#ifndef Garage_h
#define Garage_h

#include "Arduino.h"
#include "Switch.h"
#include "MomentarySwitch.h"

class Garage {
  public:
    Garage(int lightPin, int garageOpenerPin, int bigDoorPin, int backDoorPin, int basementDoorPin);

    static const int BounceDelay = 500;
    static const long LightDelay = 300000; // 5 minutes
    static const long GarageOpenerDelay = 1000;

    void update();
    boolean isBigDoorOpen();
    boolean isBackDoorOpen();
    boolean isBasementDoorOpen();
    boolean isAnyDoorOpen();
    void turnOnLight();
    void pressGarageOpener();
  private:
    MomentarySwitch _light;
    MomentarySwitch _garageOpener;
    Switch _bigDoor;
    Switch _backDoor;
    Switch _basementDoor;
};

#endif
