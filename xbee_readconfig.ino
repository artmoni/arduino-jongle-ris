#include <SoftwareSerial.h>

//SoftwareSerial xbee(2, 3); // RX, TX

int NETWORK_ID=666;

boolean isConfig=false;

void setup()
{
  Serial.begin(9600);
  // xbee.begin(9600);
}

void loop()
{  
  Serial.println("------------------");
  delay(1200);
  Serial.print("+++");
  delay(1200);
  bool bOK = false;
  while (Serial.available() > 0) {
    Serial.write(Serial.read());
    bOK = true;
  }

  if (isConfig==true){
     isConfig=!isConfig;
    //Serial.println();
    Serial.println("ATID3332");
    delay(100);
    //Serial.println();
    Serial.println("ATWR");
    readXbee();
    
   
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

    // Serial.println("The serial number of the XBee module (HIGH 32bits)");
    Serial.println();
    Serial.println("ATSH");
    readXbee();

    //Serial.println("The serial number of the XBee module (LOW 32bits)");
    Serial.println();
    Serial.println("ATSL");
    readXbee();

    //Serial.println(" The 16-bit address of the module. ");
    Serial.println();
    Serial.println("ATMY");
    readXbee();

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

  Serial.println();
}

void readXbee(){
  delay(100);
  while (Serial.available() > 0) {
    Serial.write(Serial.read());

  }
  //Serial.println();
}
void configure(){
  Serial.println();

  Serial.println("ATID"+NETWORK_ID);
  delay(100);
  //Serial.println("Write newly configured parameter values to non-volatile (long-term) storage");
  Serial.println();
  //Serial.println("ATWR");
  delay(100);
  Serial.println();
}











