#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include "WebServer.h"

byte WebServer::_mac[] = { 0x0E, 0x33, 0x2E, 0x96, 0x89, 0x0C };

WebServer::WebServer(int port): _server(port) {
  _buffer[0] = 0;
}

void WebServer::start() {
  Serial.println("Starting ethernet...");
  Ethernet.begin(_mac);
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  _server.begin();
}

void WebServer::update() {
  
  if (_client) {
    
    switch (_step) {
      case None:
        getNextHttpLine(_client, _buffer);
        _step = Headers;
        Serial.println("Request line:");
        Serial.println(_buffer);
        Serial.println();
        Serial.println("Headers:");
        
        break;
      case Headers:
        if (getNextHttpLine(_client, _buffer)) {
          Serial.println(_buffer);
        } else {
          _step = Response;
        }
        break;
      
      case Content:
      
        break;
      case Response:
        delay(1);
        _client.stop();
        _step = None;
        Serial.println("Connection ended.");
        break;
      default:
        break;
    }
    
  } else {
    _client = _server.available();
    if (_client) {
      Serial.println("Connection started...");
    }
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
