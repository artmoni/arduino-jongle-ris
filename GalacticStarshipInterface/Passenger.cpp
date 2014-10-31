#include "Arduino.h"
#include "Passenger.h"

Passenger::Passenger(){
}
void Passenger::setFirstname(String name){
  this->firstname=name;
}
void Passenger::setLastname(String lastname){
  this->lastname=lastname;
}
void Passenger::setRFID(String rfid){
  this->rfid=rfid;
}
void Passenger::setGolden(long is_golden){
  this->is_golden=is_golden;
}
long Passenger::isGolden(){
  return this->is_golden;
}

String Passenger::getFirstname(){
  return this->firstname;
}

