#ifndef GalacticServer_h
#define GalacticServer_h

#include "Arduino.h"
#include <Ethernet.h>

class GalacticServer
{
  public:
    GalacticServer(int pin);
    void getPassengers();
    void endConnection();
       void startConnection();

  private:
    int _pin;
    EthernetClient client;
    
};

 #endif
