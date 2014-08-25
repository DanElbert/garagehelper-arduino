#include <SPI.h>
#include <Ethernet.h>

#include "timer.h"
#include "Switch.h"
#include "DoorSwitch.h"
#include "MomentarySwitch.h"
#include "WebServer.h"

const int ssdPin = 4;

const int relay1Pin = 9;
const int relay2Pin = 8;

const int switch1Pin = 7;
const int switch2Pin = 6;
const int switch3Pin = 5;

const int bounceDelay = 500;
const long lightDelay = 2000;
const long garageOpenerDelay = 250;

MomentarySwitch light(relay1Pin, lightDelay);
MomentarySwitch garageOpener(relay2Pin, garageOpenerDelay);
DoorSwitch bigDoor(switch1Pin, bounceDelay);
DoorSwitch basementDoor(switch2Pin, bounceDelay);
DoorSwitch backDoor(switch3Pin, bounceDelay);
WebServer webServer(80);

int freeRam ()
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  Serial.begin(9600);
  
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(ssdPin, OUTPUT);
  
  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);
  pinMode(switch3Pin, INPUT_PULLUP);
  
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(ssdPin, HIGH);
  
  webServer.start();
}

void loop() {
  light.update();
  garageOpener.update();
  bigDoor.update();
  basementDoor.update();
  backDoor.update();
  webServer.update();
  
  if (bigDoor.isOpen() || basementDoor.isOpen() || backDoor.isOpen()) {
    light.turnOn();
  }
}
