#include <Costume.h>

#include <SoftwareSerial.h>

#include <JsonParser.h>

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(11, 10,7,6, 5,4);

SoftwareSerial RFID(2, 3); // RX and TX

#define MAX_AUTHORIZED_KEYS 100
#define RFID_LENGHT 14

Costume costume(13);

int data1 = 0;
char dataRFID=0;
int ok = -1;
int PIN_BUTTON = 4;

int PIN_RED = 9;
int PIN_GREEN = 8;
int PIN_BLUE = 11;

String currentID="";


// use first sketch in http://wp.me/p3LK05-3Gk to get your tag numbers
int newtag[14] = { 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // used for read comparisons
String golden_keys[MAX_AUTHORIZED_KEYS];

String master_key="25348484856705355525167663";
boolean is_register=false;

void setup()
{
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
  pinMode(PIN_RED, OUTPUT); // for status LEDs
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_BUTTON, INPUT); 
  lcd.begin(16, 2);

  for (int a=0;a<MAX_AUTHORIZED_KEYS;a++){
    golden_keys[a]="";
  }
  golden_keys [0]="25348484856705355506565503";
  lcd.print("init");
}

boolean comparetag(int aa[14], int bb[14])
{
  boolean ff = false;
  int fg = 0;
  for (int cc = 0 ; cc < RFID_LENGHT ; cc++)
  {
    if (aa[cc] == bb[cc])
    {
      fg++;
    }
  }
  if (fg == 14)
  {
    ff = true;
  }
  return ff;
}

void checkmytags() // compares each tag against the tag just read
{
  ok = 0; // this variable helps decision-making,
  // if it is 1 we have a match, zero is a read but no match,
  // -1 is no read attempt made
  if (currentID.compareTo(master_key)==0){
    Serial.println("Master key");
    ok++;
    return;
  }
  if (is_register)
    Serial.println("Is registering...");

  for (int key=0;key<MAX_AUTHORIZED_KEYS;key++){
    Serial.print(key);
    Serial.print(" ");
    Serial.print(golden_keys[key]);
    Serial.println();

    if (currentID.compareTo(golden_keys[key])==0){
      ok++;
      Serial.println("found "+currentID);
      break;
    }
    else if (golden_keys[key].length()==0&&is_register){
      golden_keys[key]=String(currentID);
      Serial.println("Golden :"+golden_keys[key]+ " - "+ currentID);
      is_register=false;
    }

  }
}

unsigned long hexToDec(String hexString) {
  long decValue = 0;
  int nextInt;
  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));

    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }

  return decValue;
}

String decToHex(byte decValue, byte desiredStringLength) {
  String hexString = String(decValue, HEX);
  while (hexString.length() < desiredStringLength) hexString = "0" + hexString;
  return hexString;
}

long readTags2(){
  byte i = 0;
  byte val = 0;
  byte code[6];
  byte checksum = 0;
  byte bytesread = 0;
  byte tempbyte = 0;
  long rfidID=0;

  if(RFID.available() > 0) {


    if((val = RFID.read()) == 2) {                  // check for header 



      bytesread = 0; 
      while (bytesread < 12) {                        // read 10 digit code + 2 digit checksum
        //        Serial.println("read 10 digit code: ");

        if( RFID.available() > 0) { 
          val = RFID.read();
          if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) { // if header or stop bytes before the 10 digit reading 
            break;                                    // stop reading
          }
          //Serial.print(val);
          // Do Ascii/Hex conversion:
          if ((val >= '0') && (val <= '9')) {
            val = val - '0';
          } 
          else if ((val >= 'A') && (val <= 'F')) {
            val = 10 + val - 'A';
          }

          // Every two hex-digits, add byte to code:
          if (bytesread & 1 == 1) {
            // make some space for this hex-digit by
            // shifting the previous hex-digit with 4 bits to the left:
            code[bytesread >> 1] = (val | (tempbyte << 4));

            if (bytesread >> 1 != 5) {                // If we're at the checksum byte,
              checksum ^= code[bytesread >> 1];       // Calculate the checksum... (XOR)
            };
          } 
          else {
            tempbyte = val;                           // Store the first hex digit first...
          };

          bytesread++;                                // ready to read next digit
        } 
      } 

      // Output to Serial:
      String monCode="";
      if (bytesread == 12) {                          // if 12 digit read is complete
        // Serial.print("5-byte code: ");
        for (i=2; i<5; i++) {
          monCode=monCode+decToHex(code[i],2);
          if (code[i] < 16) Serial.print("0");
          // Serial.print(code[i], HEX);
          //Serial.print("("+String(decToHex(code[i],2))+")");          
          //Serial.print(" ");
        }

        rfidID = hexToDec(monCode);
        //Serial.println("le code est=>"+String(rfidID));

        // Serial.print("Checksum: ");
        //Serial.print(code[5], HEX);
        //Serial.println(code[5] == checksum ? " -- passed." : " -- error.");
        // Serial.println();
      }

      bytesread = 0;
      Serial.print(String(rfidID));
    }
  }
  return rfidID;
}
void readTags()
{
  ok = -1;

/*

  if (RFID.available() > 0) 
  {
    currentID="";
    // read tag numbers
    delay(50); // needed to allow time for the data to come in from the serial buffer.

    Serial.println(dataRFID);
    for (int z = 0 ; z < RFID_LENGHT ; z++) // read the rest of the tag
    {

      data1=RFID.read();
      //data1 = RFID.read();
      newtag[z] = data1;
      currentID+=data1;

    }

    if (currentID.compareTo(master_key)==0)
      is_register=!is_register;

    Serial.print(currentID);
    delay(2000);
    Serial.flush(); // stops multiple reads

    // do the tags match up?  
    // checkmytags();
  }
*/
readTags2();

  String code="";
  String name ="";
  if (Serial.available())
  {
    String result="";
    result=Serial.readString();
    ok++;

    //lcd.print(result);
    delay(2000);
    code = result.substring(0,2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(code+" "+code.compareTo("G1"));
    name = result.substring(2);
    lcd.setCursor(0, 1);
    lcd.print(name);
    delay(2000);
    if (code.compareTo("G1")==0){
      displayStatusOK(name,"Golden");
    }   
    else{
      displayStatusError(name,"Business");
    }

    Serial.flush();





  }
  ok = -1;
  if (is_register){
    displayStatusOK("Registering", "...");
    //delay(500);
    displayStatusError("Registering","...");
    //delay(500);

  }
}

void loop()
{
  costume.lightShoulder(COLOR_GREEN);
  //costume.sendRFID("toto");
  readTags();
}





void resetStatusControl(){
  delay(2000);
  lcd.clear();

  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GREEN, LOW);
}

void displayStatusError(String name, String description){
  digitalWrite(PIN_RED, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(name);
  lcd.setCursor(0, 1);
  lcd.print(description);
  resetStatusControl();



}

void displayStatusOK(String name, String description){
  digitalWrite(PIN_GREEN, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(name);
  lcd.setCursor(0, 1);
  lcd.print(description);
  resetStatusControl();
}
















