#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include "WebServer.h"

WebServer::WebServer(int port, Garage* garage): _server(port), _garage(garage) {
  _buffer[0] = 0;
}

void WebServer::start() {
  _server.begin();
}

void WebServer::update() {
  
  _client = _server.available();
  if (_client) {
    Serial.println("Connection started...");
    _request.valid = true;
    _request.route = RouteUnknown;
    _request.method = MethodUnknown;
    
    getNextHttpLine(_client, _buffer);
    processRequestLine();
    
    if (!_request.valid) {
      closeClient();
      return;
    }
    
    // Read headers
    while (getNextHttpLine(_client, _buffer)) {
      // do something with header in _buffer
    }
    
    sendResponse();
    delay(2);
    closeClient();
  }
}

void WebServer::sendResponse() {
  switch (_request.route) {
    case RouteStatus:
      _client.println("HTTP/1.1 200 OK");
      _client.println("Content-Type: application/json");
      _client.println("Connection: close");
      _client.println();
      _client.print("{\"bigDoor\":");
      _client.print(_garage->isBigDoorOpen() ? "true" : "false");
      _client.print(", \"backDoor\":");
      _client.print(_garage->isBackDoorOpen() ? "true" : "false");
      _client.print(", \"basementDoor\":");
      _client.print(_garage->isBasementDoorOpen() ? "true" : "false");
      _client.print("}");
      _client.println();
      break;
    
    case RouteGarageSwitch:
      _garage->pressGarageOpener();
      _client.println("HTTP/1.1 204 No Content");
      _client.println("Connection: close");
      break;
    
    case RouteUnknown:
      _client.println("HTTP/1.1 404 Not Found");
      _client.println("Connection: close");
      _client.println("Content-Type: application/json");
      _client.println();
      _client.println("{\"error\": \"no\"}");
      _client.println();
      break; 
  }
}

void WebServer::closeClient() {
  _client.flush();
  _client.stop();
  Serial.println("Connection ended.");
}

// Processes the request line (assumes it's in _buffer) and sets properties on the _request strcut
void WebServer::processRequestLine() {
  char* meth = strtok(_buffer, " ");
  char* path = strtok(NULL, " ");
  char* httpVer = strtok(NULL, " ");
  
  if (!meth || !path || !httpVer) {
    _request.valid = false;
    return;
  }
  
  if (strcmp(meth, "GET") == 0) {
    _request.method = MethodGet;
  } else if (strcmp(meth, "POST") == 0) {
    _request.method = MethodPost;
  } else if (strcmp(meth, "PUT") == 0) {
    _request.method = MethodPut;
  } else if (strcmp(meth, "PATCH") == 0) {
    _request.method = MethodPatch;
  } else if (strcmp(meth, "DELETE") == 0) {
    _request.method = MethodDelete;
  } else {
    _request.valid = false;
    return;
  }
  
  if (_request.method == MethodGet && strcmp(path, "/status") == 0) {
    _request.route = RouteStatus;
  } else if (_request.method == MethodPost && strcmp(path, "/pushGarageOpener") == 0) {
    _request.route = RouteGarageSwitch;
  } else {
    _request.route = RouteUnknown;
  }
}

/**********************************************************************************************************************
* Read the next HTTP header record which is CRLF delimited.  We replace CRLF with string terminating null.
***********************************************************************************************************************/
boolean WebServer::getNextHttpLine(EthernetClient & client, BUFFER & readBuffer)
{
  char c;
  int bufindex = 0; // reset buffer
  boolean hasData = false;

  // reading next header of HTTP request
  if (client.connected() && client.available())
  {
    // read a line terminated by CRLF
    readBuffer[0] = client.read();
    readBuffer[1] = client.read();
    bufindex = 2;
    for (int i = 2; readBuffer[i - 2] != '\r' && readBuffer[i - 1] != '\n'; ++i)
    {
      // read full line and save it in buffer, up to the buffer size
      c = client.read();
      if (c == -1) {
        bufindex += 2;
        break;
      }
      if (bufindex < sizeof(readBuffer))
        readBuffer[bufindex++] = c;
        
      hasData = true;
    }

    readBuffer[bufindex - 2] = 0;  // Null string terminator overwrites '\r'
    
    return hasData;
  }
}
