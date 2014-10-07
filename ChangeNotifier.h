#ifndef ChangeNotifier_h
#define ChangeNotifier_h

#include "Arduino.h"
#include <Ethernet.h>
#include "Garage.h"

class ChangeNotifier {
  public:
    ChangeNotifier(Garage* garage);
    
    void update();
  private:
    void sendNotification(boolean backDoorOpen, boolean basementDoorOpen, boolean bigDoorOpen);
  
    Garage* _garage;
    EthernetClient _client;
    
    boolean _backDoorOpen = false;
    boolean _bigDoorOpen = false;
    boolean _basementDoorOpen = false;
};

#endif
