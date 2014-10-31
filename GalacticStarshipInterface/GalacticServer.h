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
    String registerPassenger(String rfid);

  void endConnection();
  void startConnectionGET(String restAPI);
void startConnectionPOST(String restAPI,String data);

private:
  int _pin;
  EthernetClient client;

};

#endif

