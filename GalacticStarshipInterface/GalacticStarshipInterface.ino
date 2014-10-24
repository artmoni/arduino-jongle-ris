#include <JsonGenerator.h>
#include <JsonParser.h>


#include <SPI.h>
#include <Ethernet.h>
#include "XBee.h"
#include "GalacticServer.h"
#include "Costume.h"
#include "Passenger.h"


using namespace ArduinoJson::Parser;

GalacticServer starship(1);
//XBee xbee = XBee();
JsonParser<32> parser;

void setup() {
  Serial.begin(9600);
  //xbee.begin(Serial);

  starship.initConnection();
}

void loop()
{
  String rfid="";

 //getPassengers();

  rfid="12098HJ";
  Passenger passenger;
  passenger= getPassenger(rfid);
  if (passenger.isGolden()==1){
    Serial.println(rfid+" Bienvenue "+ passenger.getFirstname());
  }
  else{
    Serial.println(rfid+" Seconde classe");
  }
  delay(1000);

  rfid="sdsd";
  passenger= getPassenger(rfid);
  if (passenger.isGolden()==1){
    Serial.println(rfid+" Bienvenue "+ passenger.getFirstname());
  }
  else{
    Serial.println(rfid+" Seconde classe");
  }
  delay(1000);

  rfid="KO";
  passenger= getPassenger(rfid);
  if (passenger.isGolden()==1){
    Serial.println(rfid+" Bienvenue "+ passenger.getFirstname());
  }
  else{
    Serial.println(rfid+" Seconde classe");
  }
  delay(1000);


}

String getPassengers(){
  String json;
  json = starship.getPassengers();

  char* parseJson;
  json.toCharArray(parseJson,json.length());
  JsonObject root = parser.parse(parseJson);
  if (!root.success())
  {
    Serial.println("JsonParser.parse() failed");
    return "";
  }
  char* firstname = root["firstname"];
  return String(firstname);
}

Passenger getPassenger(String rfidToCheck){
  Passenger passenger;
  String json="";
json=  starship.getPassenger(rfidToCheck);

  char* parseJson;
  Serial.println("====>>"+json);
  json.toCharArray(parseJson,json.length());
  
  JsonObject root = parser.parse(parseJson);
  if (!root.success())
  {
    Serial.println("JsonParser.parse() failed");
    return passenger;
  }

  char * firstname =root["firstname"];
  char * lastname =root["lastname"];  
  char * rfid =root["rfid"];
  int is_golden=root["is_golden"];
  
  passenger.setFirstname(String(firstname));
  passenger.setLastname(String(lastname));
  passenger.setRFID(String(rfid));
  passenger.setGolden(short(is_golden));

  return passenger;
}







