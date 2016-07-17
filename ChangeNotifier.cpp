#include "ChangeNotifier.h"

ChangeNotifier::ChangeNotifier(Garage* garage, PubSubClient* mqtt, const char* topic_root):
  _garage(garage),
  _mqtt(mqtt),
  _topicRoot(topic_root),
  _bigDoorOpen(false),
  _backDoorOpen(false),
  _basementDoorOpen(false)
{}

void ChangeNotifier::start() {

}

void ChangeNotifier::update() {
  boolean readBackDoor = _garage->isBackDoorOpen();
  boolean readBigDoor = _garage->isBigDoorOpen();
  boolean readBasementDoor = _garage->isBasementDoorOpen();

  if (readBackDoor != _backDoorOpen) {
    sendNotification("backDoor", readBackDoor);
    _backDoorOpen = readBackDoor;
  }

  if (readBigDoor != _bigDoorOpen) {
    sendNotification("bigDoor", readBigDoor);
    _bigDoorOpen = readBigDoor;
  }

  if (readBasementDoor != _basementDoorOpen) {
    sendNotification("basementDoor", readBasementDoor);
    _basementDoorOpen = readBasementDoor;
  }
}

void ChangeNotifier::sendNotification(char* doorName, boolean state) {
  String msg = state ? "OPEN" : "CLOSED";
  String topic = _topicRoot;
  topic += doorName;

  _mqtt->publish(topic.c_str(), msg.c_str(), true);
}
