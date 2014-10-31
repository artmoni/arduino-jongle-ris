#ifndef Passenger_h
#define Passenger_h

#include "Arduino.h"


class Passenger
{
public:
  Passenger();
  void setFirstname(String name);
  void setLastname(String lastname);
  void setRFID(String rfid);
  void setGolden(long is_golden);
  long isGolden();
  String getFirstname();
 
private:
 String rfid;
 String lastname;
 String firstname;
 long is_golden;
 
 

};

#endif
