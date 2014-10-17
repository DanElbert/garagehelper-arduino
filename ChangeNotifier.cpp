#include "ChangeNotifier.h"

ChangeNotifier::ChangeNotifier(Garage* garage): 
  _garage(garage),
  _bigDoorOpen(false),
  _backDoorOpen(false),
  _basementDoorOpen(false),
  _keepaliveTimer(300000) 
{}

void ChangeNotifier::start() {
  _keepaliveTimer.tick();
}

void ChangeNotifier::update() {
  boolean readBackDoor = _garage->isBackDoorOpen();
  boolean readBigDoor = _garage->isBigDoorOpen();
  boolean readBasementDoor = _garage->isBasementDoorOpen();
  
  if (readBackDoor != _backDoorOpen || readBigDoor != _bigDoorOpen || readBasementDoor != _basementDoorOpen) {
    _backDoorOpen = readBackDoor;
    _bigDoorOpen = readBigDoor;
    _basementDoorOpen = readBasementDoor;
    sendNotification(readBackDoor, readBasementDoor, readBigDoor);
    _keepaliveTimer.tick();
  } else if (_keepaliveTimer.tock()) {
    sendKeepalive();
    _keepaliveTimer.tick();
  }
}

void ChangeNotifier::sendNotification(boolean backDoorOpen, boolean basementDoorOpen, boolean bigDoorOpen) {
  byte server[] = { 10, 0, 0, 50 };
  if (_client.connect(server, 80)) {
    
    String json = "{\"bigDoor\":";
    json = json + (bigDoorOpen ? "true" : "false");
    json = json + ", \"backDoor\":";
    json = json + (backDoorOpen ? "true" : "false");
    json = json + ", \"basementDoor\":";
    json = json + (basementDoorOpen ? "true" : "false");
    json = json + "}";
      
    String length = String(json.length());
    
    _client.println("POST /garage/helper/update HTTP/1.1");
    _client.println("Content-Type: application/json;charset=utf-8");
    _client.println("Host: garage.elbert.us");
    _client.println("content-length:" + length);
    _client.println("Connection: close");
    _client.println();
    _client.println(json);
    _client.println();
    _client.println();
    
    while (_client.available()) {
      // Read answer and print to serial debug
      char c = _client.read();
      Serial.print(c);
    }
    
    _client.flush();
    _client.stop();
  }
}

void ChangeNotifier::sendKeepalive() {
  sendNotification(_backDoorOpen, _basementDoorOpen, _bigDoorOpen);
}
