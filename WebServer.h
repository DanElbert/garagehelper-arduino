#ifndef WebServer_h
#define WebServer_h

#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>

#define STRING_BUFFER_SIZE 128

typedef char BUFFER[STRING_BUFFER_SIZE];

enum ProcessStep {
  None,
  Headers,
  Content,
  Response
};

class WebServer {
  public:
    WebServer(int port);
    void start();
    void update();
  private:
    static byte _mac[];
    BUFFER _buffer;
    ProcessStep _step = None;
    EthernetClient _client;
    EthernetServer _server;
    
    boolean getNextHttpLine(EthernetClient & client, BUFFER & readBuffer);
};

#endif
