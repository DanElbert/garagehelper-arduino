#ifndef ChangeNotifier_h
#define ChangeNotifier_h

#include "Arduino.h"
#include <PubSubClient.h>
#include "Timer.h"
#include "Garage.h"

class ChangeNotifier {
  public:
    ChangeNotifier(Garage* garage, PubSubClient* mqtt);

    void start();
    void update();
  private:
    void sendNotification(char* doorName, boolean state);

    const* char _topicRoot;
    

    Garage* _garage;
    PubSubClient _mqtt;

    boolean _backDoorOpen;
    boolean _bigDoorOpen;
    boolean _basementDoorOpen;
};

#endif
