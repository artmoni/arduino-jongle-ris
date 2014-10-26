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


#define PIN_GREEN 8
#define PIN_RED 7

using namespace ArduinoJson::Parser;

GalacticServer starship(1);
//XBee xbee = XBee();
JsonParser<32> parser;

void setup() {
  Serial.begin(9600);
  // console.begin(Serial);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  starship.initConnection();

  /*
  Serial.print("+++");
   delay(1000);
   Serial.println();
   Serial.print("ATDL13A200");
   delay(100);
   Serial.println();
   Serial.print("ATDH40AFBESD4"); 
   Serial.println();
   Serial.print("ATCN"); 
   Serial.println();
  */ 
  delay(100);
}

String scannedRFID="";
void loop()
{

  //console.println("--------------------------");



  if (Serial.available())
  {
    scannedRFID="";
    scannedRFID=Serial.readString();
  }

  if (scannedRFID != "")

  {    
    resetStatusControl();
    Serial.flush();
    //Serial.println("RFID1="+scannedRFID);


    Passenger passenger;
    passenger= getPassenger(scannedRFID);
    if (passenger.isGolden()==0){
      //Serial.println(scannedRFID+" Bienvenue "+ passenger.getFirstname());
      Serial.print("G0");
      displayStatusError();
    }
    else{
      //Serial.println(scannedRFID+" Seconde classe");
      Serial.print("G1");
      displayStatusOK();
    }
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
    //Serial.println("JsonParser.parse() failed");
    displayStatusError();
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
    //Serial.println("JsonParser.parse() failed");
    displayStatusError();
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


void resetStatusControl(){
   delay(2000);
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GREEN, LOW);
}

void displayStatusError(){
  digitalWrite(PIN_RED, HIGH);
 resetStatusControl();
}

void displayStatusOK(){
  digitalWrite(PIN_GREEN, HIGH);
 resetStatusControl();
}













