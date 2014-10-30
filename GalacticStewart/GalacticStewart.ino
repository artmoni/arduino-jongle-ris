/*
*  This program manage a costume
 *  Creator: Sebastien FLEURY
 *    La Compagnie Jongle&Ris
 * 
 */

#include "Rfid.h"
#include "Costume.h"
#include <SoftwareSerial.h>
#include <JsonParser.h>
#include <LiquidCrystal.h>
#include "pitches.h"
#include "golden.h"

/*
*
 * GLOBAL PARAMETERS
 */
#define MAX_AUTHORIZED_KEYS 1
#define RFID_LENGHT 14

// notes in the melody:
int melody2[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int melody[] = {
  NOTE_E2, NOTE_D8};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(11, 10,7,6, 5,4);




Costume costume(13);

//SeeedRFIDLib RFID(2,3);
//RFIDTag tag;
Rfid leftHand;

int data1 = 0;
char dataRFID=0;
int ok = -1;
int PIN_BUTTON = 4;

int PIN_RED = 8;
int PIN_GREEN = 11;
int PIN_BLUE = 9;



// use first sketch in http://wp.me/p3LK05-3Gk to get your tag numbers
int newtag[14] = { 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // used for read comparisons
//String golden_keys[MAX_AUTHORIZED_KEYS];

long master_key=9393987;
boolean is_register=false;
long passengerID=0;

void setup()
{
  Serial.begin(9600);  // start serial to PC 
  pinMode(PIN_RED, OUTPUT); // for status LEDs
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_BUTTON, INPUT); 
  lcd.begin(16, 2);


  //golden_keys [0]="25348484856705355506565503";
  lcd.print("init");
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(3, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(3);
  }
  costume.turnOFFShoulder();
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

boolean checkmytags(long currentID) // compares each tag against the tag just read
{
  ok = 0; // this variable helps decision-making,
  // if it is 1 we have a match, zero is a read but no match,
  // -1 is no read attempt made
  if (currentID ==master_key){
    Serial.println("Master key");
    ok++;
    return true;
  }
  if (is_register)
    Serial.println("Is registering...");

  for (int key=0;key<MAX_AUTHORIZED_KEYS;key++){
    Serial.print(key);
    Serial.print(" ");
    Serial.print(golden_keys[key]);
    Serial.println();

    if (currentID==golden_keys[key]){

      Serial.println("found "+currentID);
      return true;
    }
    else if (golden_keys[key]==0&&is_register){
      golden_keys[key]=currentID;
      Serial.println("Golden :"+String(golden_keys[key])+ " - "+ currentID);
      is_register=false;
    }

  }
  return false;
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

long time;
void loop()
{
  passengerID=0;


  passengerID=leftHand.readRFID();
  if (passengerID!=0){
    time=millis();


    Serial.println(String("ID="+passengerID));
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(String(passengerID));
    if (checkmytags(passengerID)){
      tone(3,NOTE_D8,1000);
      lcd.setCursor(0, 0);
      lcd.print("Golden");
      costume.turnONShoulder(COLOR_BLUE); 
    }
    else{
      tone(3,NOTE_G3,1000);
      lcd.setCursor(0, 0);
      lcd.print("Business");
      costume.turnONShoulder(COLOR_RED);
    }
  }
  if (millis()-time>=3000){
    costume.turnOFFShoulder();
    costume.resetHandLED();
  }
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























