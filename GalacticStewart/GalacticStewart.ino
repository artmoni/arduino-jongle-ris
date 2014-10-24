#include <SoftwareSerial.h>
#include "Costume.h"
#include <JsonParser.h>

SoftwareSerial RFID(2, 3); // RX and TX

#define MAX_AUTHORIZED_KEYS 100
#define RFID_LENGHT 14

Costume costume(13);

int data1 = 0;
int ok = -1;
int PIN_BUTTON = 4;

int PIN_RED = 13;
int PIN_GREEN = 12;
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
   
  for (int a=0;a<MAX_AUTHORIZED_KEYS;a++){
    golden_keys[a]="";
  }
  golden_keys [0]="25348484856705355506565503";

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

  if (RFID.available() > 0) 
  {
    currentID="";
    // read tag numbers
    delay(50); // needed to allow time for the data to come in from the serial buffer.

    for (int z = 0 ; z < RFID_LENGHT ; z++) // read the rest of the tag
    {
      data1 = RFID.read();
      newtag[z] = data1;
      currentID+=data1;

    }

    if (currentID.compareTo(master_key)==0)
      is_register=!is_register;

    Serial.println(currentID);

    RFID.flush(); // stops multiple reads

    // do the tags match up?  
    checkmytags();
  }

  // now do something based on tag type
  if (ok > 0) // if we had a match
  {
    Serial.println("Accepted");
    digitalWrite(PIN_GREEN, HIGH);
    //   delay(1000);
    digitalWrite(PIN_RED, LOW);

    ok = -1;
  }
  else if (ok == 0) // if we didn't have a match
  {
    Serial.println("Rejected");
    digitalWrite(PIN_RED, HIGH);
    //delay(1000);
    digitalWrite(PIN_GREEN, LOW);

    ok = -1;
  }

  if (is_register){
    digitalWrite(PIN_GREEN, HIGH);
    digitalWrite(PIN_RED, HIGH);
    delay(500);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, LOW);
    delay(500);
    
  }
}

void loop()
{
costume.lightShoulder(COLOR_GREEN);
costume.sendRFID("toto");
  readTags();
}











