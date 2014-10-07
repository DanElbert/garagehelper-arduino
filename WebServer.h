#ifndef WebServer_h
#define WebServer_h

#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include "Garage.h"

#define STRING_BUFFER_SIZE 128

typedef char BUFFER[STRING_BUFFER_SIZE];

enum ProcessStep {
  None,
  Headers,
  Content,
  Response,
  Fail
};

enum RequestMethod {
  MethodUnknown,
  MethodGet,
  MethodPut,
  MethodPost,
  MethodPatch,
  MethodDelete
};

enum Route {
  RouteUnknown,
  RouteStatus,
  RouteGarageSwitch
};

struct request {
  RequestMethod method;
  Route route;
  boolean valid;
};

class WebServer {
  public:
    WebServer(int port, Garage* garage);
    void start();
    void update();
  private:
    Garage* _garage;
    BUFFER _buffer;
    ProcessStep _step = None;
    request _request;
    EthernetClient _client;
    EthernetServer _server;
    
    void processRequestLine();
    void sendResponse();
    void closeClient();
    boolean getNextHttpLine(EthernetClient & client, BUFFER & readBuffer);
};

#endif
