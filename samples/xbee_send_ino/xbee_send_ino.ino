#include <SoftwareSerial.h>

//SoftwareSerial XBee(2, 3); // RX, TX
 
void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  //XBee.begin(9600);
  Serial.begin(9600);
  /*
  Serial.print("+++");
   delay(1000);
   Serial.println();
   Serial.print("ATDH 13A200");
   delay(100);
   Serial.println();
   Serial.print("ATDL 40BE51CF"); 
   Serial.println();
   Serial.print("ATCN"); 
   Serial.println();
  */
}

void loop()
{/*
  if (Serial.available())
  { // If data comes in from serial monitor, send it out to XBee
    XBee.write(Serial.read());
    
  }*/
  Serial.print("A");
    delay(2000);
  /*if (XBee.available())
  { // If data comes in from XBee, send it out to serial monitor
    Serial.write(XBee.read());
  }*/
}
