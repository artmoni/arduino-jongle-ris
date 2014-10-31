#include <JsonGenerator.h>
#include <JsonParser.h>


#include <SPI.h>
#include <Ethernet.h>
#include "XBee.h"
#include "GalacticServer.h"
//#include "Costume.h"
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
  delay(1000);
   Serial.print("+++");
   delay(1000);
   Serial.println();
   
   Serial.print("ATRS");
   Serial.println();
   Serial.print("ATDL 13A200");
   delay(100);
   Serial.println();
   Serial.print("ATDH 40AFBED4"); 
   Serial.println();
   //Serial.println();
   //Serial.print("ATCH C"); 
   Serial.println();
   Serial.print("ATCN"); 
   Serial.println();
   */

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
      Serial.print(passenger.getFirstname());

      displayStatusError();
    }
   else if (passenger.isGolden()==1){
      //Serial.println(scannedRFID+" Seconde classe");
      Serial.print("G1");
      Serial.print(passenger.getFirstname());
      displayStatusOK();
    }
    else if (passenger.isGolden()==2){
      Serial.print("G2");
      Serial.print(passenger.getFirstname());
      displayStatusOK();
    }
    else{
      Serial.print("GX");
      Serial.print(passenger.getFirstname());
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
  long is_golden=root["is_golden"];

  passenger.setFirstname(String(firstname));
  passenger.setLastname(String(lastname));
  passenger.setRFID(String(rfid));
  passenger.setGolden(is_golden);
   // Serial.println("Passenger isgolden="+String(is_golden)+" name"+rfid+" "+lastname+" "+firstname);

  return passenger;
}


void resetStatusControl(){
  //delay(2000);

  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GREEN, LOW);
}
int resetTimeout=0;
void displayStatusError(){
  digitalWrite(PIN_RED, HIGH);
  resetStatusControl();
}

void displayStatusOK(){
  digitalWrite(PIN_GREEN, HIGH);
  resetStatusControl();
}



void displayXbeeConfig(){
  Serial.print("+++");
  delay(2000);
  bool bOK = false;
  while (Serial.available() > 0) {
    Serial.write(Serial.read());
    bOK = true;
  }


  if(bOK)
  {

    //Serial.println("The network ID of the XBee module. ");
    Serial.println();
    Serial.println("ATID");
    readXbee();

    //Serial.println("The channel of the XBee module. ");
    Serial.println();
    Serial.println("ATCH");
    readXbee();
    Serial.println();
    Serial.println("ATSC");
    readXbee();
    // Serial.println("The serial number of the XBee module (HIGH 32bits)");
    Serial.println();
    Serial.println("ATSH");
    readXbee();

    //Serial.println("The serial number of the XBee module (LOW 32bits)");
    Serial.println();
    Serial.println("ATSL");
    readXbee();

    // Serial.println("The serial number of the XBee module (HIGH 32bits)");
    Serial.println();
    Serial.println("ATDH");
    readXbee();

    // Serial.println("The serial number of the XBee module (HIGH 32bits)");
    Serial.println();
    Serial.println("ATDL");
    readXbee();

    //Serial.println(" The 16-bit address of the module. ");
    Serial.println();
    Serial.println("ATMY");
    readXbee();

    /*
0 (1200 bps)
     1 (2400 bps)
     2 (4800 bps)
     3 (9600 bps)
     4 (19200 bps)
     5 (38400 bps)
     6 (57600 bps)
     7 (115200 bps) 
     */

    //Serial.println("The baud rate used for serial communication with the Arduino board or computer");
    Serial.println();
    Serial.println("ATBD");
    readXbee();

    //Serial.println("Exit command mode now");
    Serial.println();
    Serial.println("ATCN");
    readXbee();
    Serial.println();
  }
}

void readXbee(){
  delay(100);
  while (Serial.available() > 0) {
    Serial.write(Serial.read());
  }
}












