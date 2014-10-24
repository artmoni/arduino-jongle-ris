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
  void setGolden(short is_golden);
  short isGolden();
  String getFirstname();
 
private:
 String rfid;
 String lastname;
 String firstname;
 boolean is_golden;
 
 

};

#endif
