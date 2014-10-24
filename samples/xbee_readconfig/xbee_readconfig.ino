

int PIN=7;

void setup()
{
  Serial.begin(9600);

  pinMode(PIN, OUTPUT);
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

  
  if(bOK)
  {
    displayXbeeConfig();
  }

  digitalWrite(PIN, HIGH);
  delay(100);
  digitalWrite(PIN, LOW);

  Serial.println();
}

void displayXbeeConfig(){

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

void readXbee(){
  delay(100);
  while (Serial.available() > 0) {
    Serial.write(Serial.read());

  }
}















