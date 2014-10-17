#ifndef ChangeNotifier_h
#define ChangeNotifier_h

#include "Arduino.h"
#include <Ethernet.h>
#include "Timer.h"
#include "Garage.h"

class ChangeNotifier {
  public:
    ChangeNotifier(Garage* garage);
    
    void start();
    void update();
  private:
    void sendNotification(boolean backDoorOpen, boolean basementDoorOpen, boolean bigDoorOpen);
    void sendKeepalive();
  
    Timer _keepaliveTimer;
    Garage* _garage;
    EthernetClient _client;
    
    boolean _backDoorOpen;
    boolean _bigDoorOpen;
    boolean _basementDoorOpen;
};

#endif
