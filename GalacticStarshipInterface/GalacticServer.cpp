

#include "Arduino.h"
#include "GalacticServer.h"

#define DEBUG false

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,0,1);
IPAddress server(192,168,0,100);  

GalacticServer::GalacticServer(int pin)
{
  if (DEBUG)

    Serial.println("GalacticServer init");

}

void GalacticServer::initConnection(){

  // start the Ethernet connection:
  /*if (Ethernet.begin(mac) == 0) {
   Serial.println("Failed to configure Ethernet using DHCP");
   // no point in carrying on, so do nothing forevermore:
   // try to congifure using IP address instead of DHCP:
   Ethernet.begin(mac, ip);
   }*/
  Ethernet.begin(mac, ip);
  delay(1000);
}


void GalacticServer::startConnectionGET(String restAPI){
  if (DEBUG)

    Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    if (DEBUG)

      Serial.println("connected");
    // Make a HTTP request:
    client.println("GET "+restAPI+" HTTP/1.1");
    client.println("Host: StarShipInterface");
    client.println("Connection: close");
    client.println();
  }
  else {
    // kf you didn't get a connection to the server:
    if (DEBUG)
      Serial.println("connection failed");
  }
}
void GalacticServer::startConnectionPOST(String restAPI,String data){
  if (DEBUG)

    Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    if (DEBUG)

      Serial.println("connected");
    // Make a HTTP request:
    client.println("POST "+restAPI+" HTTP/1.1");
    client.println("Host: StarShipInterface");
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    client.println();
    client.println("Content-Length: %u\r\n"+strlen(data));
    client.println(data);
  }
  else {
    // kf you didn't get a connection to the server:
    if (DEBUG)
      Serial.println("connection failed");
  }
}


void GalacticServer::endConnection()
{
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    if (DEBUG){
      Serial.println();
      Serial.println("disconnecting.");
    }
    client.stop();
    client.flush();

    // do nothing forevermore:
    //while(true);
    //delay(5000);
  }
}

String GalacticServer::getPassengers()
{
  startConnectionGET("/passenger");
  String passengersJSON="";
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    passengersJSON= String(passengersJSON+c);
  } 
  if (DEBUG) 
    Serial.print(passengersJSON);
  endConnection();
  return passengersJSON;

}

String GalacticServer::getPassenger(String rfid){
  startConnection("/passenger/"+rfid);
  String passengerJSON;
  // if there are incoming bytes available
  // from the server, read them and print them:
  int connectLoop = 0;
  boolean startRead=false;
  while(client.connected())
  {
    while (client.available()) {
      char c = client.read();
      //Serial.print(c);
      if (c == '{' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part 
        // passengerJSON= String(passengerJSON+'"');
        passengerJSON= String(passengerJSON+c);

      }
      else if(startRead){

        //if(c != '}'){
        // if (c=='"')
        //passengerJSON= String(passengerJSON+"\\");
        passengerJSON= String(passengerJSON+c);
        //}
        //if (c=='}')
        //        passengerJSON= String(passengerJSON+'"');

      }
      connectLoop = 0;  
    }  

    connectLoop++;

    // if more than 10000 milliseconds since the last packet
    if(connectLoop > 10000)
    {
      // then close the connection from this end.
      if (DEBUG){
        Serial.println();
        Serial.println(F("Timeout"));
      }
      client.stop();
    }
    // this is a delay for the connectLoop timing
    delay(1);
  }


  if (DEBUG)
    Serial.println(passengerJSON);
  endConnection();
  return passengerJSON;
}


String GalacticServer::registerPassenger(String rfid){
  startConnectionPOST("/golden","rfid="+rfid);
  String passengerJSON;
  // if there are incoming bytes available
  // from the server, read them and print them:
  int connectLoop = 0;
  boolean startRead=false;
  while(client.connected())
  {
    while (client.available()) {
      char c = client.read();
      //Serial.print(c);
      if (c == '{' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part 
        // passengerJSON= String(passengerJSON+'"');
        passengerJSON= String(passengerJSON+c);

      }
      else if(startRead){

        //if(c != '}'){
        // if (c=='"')
        //passengerJSON= String(passengerJSON+"\\");
        passengerJSON= String(passengerJSON+c);
        //}
        //if (c=='}')
        //        passengerJSON= String(passengerJSON+'"');

      }
      connectLoop = 0;  
    }  

    connectLoop++;

    // if more than 10000 milliseconds since the last packet
    if(connectLoop > 10000)
    {
      // then close the connection from this end.
      if (DEBUG){
        Serial.println();
        Serial.println(F("Timeout"));
      }
      client.stop();
    }
    // this is a delay for the connectLoop timing
    delay(1);
  }


  if (DEBUG)
    Serial.println(passengerJSON);
  endConnection();
  return passengerJSON;
}















