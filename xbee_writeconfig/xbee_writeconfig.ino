//#include <SoftwareSerial.h>

//SoftwareSerial xbee(2, 3); // RX, TX

int NETWORK_ID=1234;
int MY_ID=1;
//int MY_ID=2;
//int MY_ID=3;

int PIN=7;
boolean isConfig=true;

void setup()
{
  Serial.begin(9600);

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
    configure();
  }
 
  Serial.println();
}

void configure(){

  Serial.println("ATID 1234");
  delay(100);
  Serial.println("ATMY 1");
  delay(100);
  Serial.println("ATWR");
  delay(100);
  Serial.println();
}













