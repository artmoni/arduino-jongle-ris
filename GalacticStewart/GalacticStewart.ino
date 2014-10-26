#include "Rfid.h"

#include <Costume.h>

#include <SoftwareSerial.h>

#include <JsonParser.h>

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(11, 10,7,6, 5,4);


#define MAX_AUTHORIZED_KEYS 100
#define RFID_LENGHT 14

Costume costume(13);
Rfid leftHand;

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



void readTags()
{
  ok = -1;

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
  long passengerID=0;
  costume.lightShoulder(COLOR_GREEN);
  passengerID=leftHand.readRFID();
  if (passengerID!=0)
    Serial.print(String(passengerID));
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

















