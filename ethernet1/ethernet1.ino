

#include <SPI.h>
#include <Ethernet.h>
#include "XBee.h"
#include "GalacticServer.h"
#include "Costume.h"


 GalacticServer starship(1);
XBee xbee = XBee();

void setup() {
  Serial.begin(9600);
  xbee.begin(Serial);

  Serial.println("setup");
 starship.startConnection();
}

void loop()
{
  //Serial.println("loop");
  starship.getPassengers();
  starship.endConnection();

  
}
