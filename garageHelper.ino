#include <SPI.h>
#include <Ethernet.h>

#include "Timer.h"
#include "Switch.h"
#include "MomentarySwitch.h"
#include "Garage.h"
#include "WebServer.h"
#include "ChangeNotifier.h"

const int ssdPin = 4;

const int relay1Pin = 9;
const int relay2Pin = 8;

const int switch1Pin = 7;
const int switch2Pin = 6;
const int switch3Pin = 5;

byte macAddr[] = { 0x0E, 0x33, 0x2E, 0x96, 0x89, 0x0C };

Garage garage(relay1Pin, relay2Pin, switch1Pin, switch2Pin, switch3Pin);
WebServer webServer(80, &garage);
ChangeNotifier notifier(&garage);

// Debug function to return amount of free ram during runtime
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
  
  Serial.println("Starting ethernet...");
  Ethernet.begin(macAddr);
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  webServer.start();
  notifier.start();
}

void loop() {
  garage.update();
  webServer.update();
  notifier.update();
}
