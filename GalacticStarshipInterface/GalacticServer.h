#ifndef GalacticServer_h
#define GalacticServer_h

#include "Arduino.h"
#include <Ethernet.h>

class GalacticServer
{
public:
  GalacticServer(int pin);
  void initConnection();
  String getPassengers();
  String getPassenger(String rfid);
  void endConnection();
  void startConnection(String restAPI);

private:
  int _pin;
  EthernetClient client;

};

#endif

