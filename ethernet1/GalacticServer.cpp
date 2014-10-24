

#include "Arduino.h"
#include "GalacticServer.h"

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,0,1);
IPAddress server(192,168,0,100);  

GalacticServer::GalacticServer(int pin)
{
Serial.println("GalacticServer init");
 
}

void GalacticServer::startConnection(){
 
  // start the Ethernet connection:
  //Serial.println("IP 192,168,0,1");
  Ethernet.begin(mac, ip);
 // if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    //Ethernet.begin(mac, ip);
  //}
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /passenger/12098HJ HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}


void GalacticServer::endConnection()
{
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    //while(true);
    delay(5000);
  }
}

void GalacticServer::getPassengers()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
}




