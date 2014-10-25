#include <JsonGenerator.h>
#include <JsonParser.h>


#include <SPI.h>
#include <Ethernet.h>
#include "XBee.h"
#include "GalacticServer.h"
#include "Costume.h"
#include "Passenger.h"
#include <SoftwareSerial.h>
//SoftwareSerial console(2, 3);

using namespace ArduinoJson::Parser;

GalacticServer starship(1);
//XBee xbee = XBee();
JsonParser<32> parser;

void setup() {
  Serial.begin(9600);
  // console.begin(Serial);

  starship.initConnection();
}

String scannedRFID="";
void loop()
{
  
  //console.println("--------------------------");

  if (Serial.available())
  {scannedRFID="";
    scannedRFID=Serial.readString();
  }

  if (scannedRFID != "")

  {    Serial.flush();
    Serial.println("RFID1="+scannedRFID);


    Passenger passenger;
    passenger= getPassenger(scannedRFID);
     if (passenger.isGolden()==1){
      Serial.println(scannedRFID+" Bienvenue "+ passenger.getFirstname());
    }
    else{
      Serial.println(scannedRFID+" Seconde classe");
    }
    delay(2000);
    scannedRFID="";
  }






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
  String json;
  json=  starship.getPassenger(rfidToCheck);
 
  //char json[]="{\"error\":false,\"rfid\":\"qsdsqd\",\"firstname\":\"ssss\",\"lastname\":\"fleudry\",\"check_count\":0,\"is_golden\":1}";

  //char *json='{"error":false,"rfid":"12098HJ","firstname":"seb","lastname":"fleury","check_count":2,"is_golden":1}';

  //char json [] = "{\"firstname\":\"KO\",\"is_golden\":1,\"data\":[48.756080,2.302038]}";
  char parseJson[json.length()+1];

  json.toCharArray(parseJson,json.length()+1);
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














