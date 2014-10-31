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

#define RFID_LENGHT 14

// notes in the melody:
int melody2[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int melody[] = {
  NOTE_E2, NOTE_D8, NOTE_G3};
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
int PIN_BUTTON = 4;



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
  /*
   Serial.print("+++");
   delay(1000);
   Serial.println();
   Serial.print("ATDL 13A200");
   delay(100);
   Serial.println();
   Serial.print("ATDH 40B451CF"); 
   Serial.println();
   Serial.print("ATCN"); 
   Serial.println();*/


  pinMode(PIN_BUTTON, INPUT); 
  lcd.begin(16, 2);


  //golden_keys [0]="25348484856705355506565503";
  lcd.print("Welcome agent");
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

short checkmytags(long currentID) // compares each tag against the tag just read
{
  if (currentID ==master_key){
    lcd.print("Master key");
    return true;
  }
  if (is_register)
    Serial.println("Is registering...");

  for (int key=0;key<MAX_GOLDEN;key++){
    Serial.print(key);
    Serial.print(" ");
    Serial.print(golden_keys[key]);
    Serial.println();

    if (currentID==golden_keys[key]){

      Serial.println("found GOLDEN="+currentID);
      return GOLDEN;
    }/*
    else 
     
     
     if (golden_keys[key]==0&&is_register){
     golden_keys[key]=currentID;
     Serial.println("Golden :"+String(golden_keys[key])+ " - "+ currentID);
     is_register=false;
     }*/

  }

  for (int key=0;key<MAX_ELU;key++){


    if (currentID==elu_keys[key]){

      Serial.println("found ELU="+currentID);
      return ELU;
    }

  }
  for (int key=0;key<MAX_DDAY;key++){


    if (currentID==dday_keys[key]){

      Serial.println("found GOLDEN_DDAY="+currentID);
      return GOLDEN_DDAY;
    }

  }
  return BUSINESS;
}



void readTagsRemote()
{


  String code="";
  String name ="";

  if (Serial.available())
  {
    String result="";
    result=Serial.readString();

    delay(100);
    code = result.substring(0,2);
    name = result.substring(2);
    delay(100);
    if (code.compareTo("G0")==0){
      displayStatusError(name,"Business");
    }
    else
      if (code.compareTo("G1")==0){
        displayStatusOK(name,"Golden");
      }   
      else if (code.compareTo("G2")==0){
        displayStatusOK(name,"Elu");
      }

    Serial.flush();

  }
  if (is_register){
    displayStatusOK("Registering", "...");
    //delay(500);
    displayStatusError("Registering","...");
    //delay(500);

  }
}

long timeCheckRFID;
long timeDisplayAccess=0;
long timeoutRegistering=0;
void loop()
{
  passengerID=0;

  passengerID=leftHand.readRFID();
  if (passengerID!=0){
    timeCheckRFID=millis();

    if (passengerID ==master_key){
      displayChecking("~ Master key ~","Promote to Golden");
      timeoutRegistering=millis();
      is_register=true;
    }
    else{
      Serial.print(String(passengerID));       
      
      displayChecking("~ Checking Access ~",String(passengerID));
    }
    /*lcd.clear();
     lcd.setCursor(0, 1);
     lcd.print(String(passengerID));
     
     delay(100);
     int passenger_type = checkmytags(passengerID);
     if (passenger_type==GOLDEN){
     displayStatusOK("","Golden");
     
     }
     else if (passenger_type==BUSINESS){
     displayStatusError("","Business");
     
     } 
     else if (passenger_type==ELU){
     displayStatusWarning("","Elu");
     
     }
     */
  }
  readTagsRemote();


  if (millis()-timeCheckRFID>=3000&&timeCheckRFID!=0){
   // costume.turnOFFShoulder();
  costume.lightOFFChecking();
    timeCheckRFID=0;
  }
  if (millis()-timeDisplayAccess>=3000&&timeDisplayAccess!=0){
    costume.turnOFFShoulder();
    costume.resetHandLED();
    timeDisplayAccess=0;
  }
  if (millis()-timeoutRegistering>=10000&&timeoutRegistering!=0){
    is_register=false;
    timeoutRegistering=0;
    lcd.clear();
          //displayChecking("~ Master key ~","Timeout Registering");

  }
}



void resetStatusControl(){
  //delay(2000);
  lcd.clear();
  lcd.noBlink();
}

void displayStatusError(String name, String description){
  //digitalWrite(PIN_RED, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(name);
  lcd.setCursor(0, 1);
  lcd.print(description);

  tone(3,NOTE_E3,1000);

  costume.turnONShoulder(COLOR_RED);
  // resetStatusControl();
  timeDisplayAccess=millis();
}

void displayStatusOK(String name, String description){
  // digitalWrite(PIN_GREEN, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(description);
  lcd.setCursor(0, 1);
  lcd.print(name);
  tone(3,NOTE_D8,1000);

  costume.turnONShoulder(COLOR_BLUE); 
  //resetStatusControl();
  timeDisplayAccess=millis();

}

void displayStatusWarning(String name, String description){
  // digitalWrite(PIN_GREEN, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(description);
  lcd.setCursor(0, 1);
  lcd.print(name);

  tone(3,NOTE_C3,700);

  costume.turnONShoulder(COLOR_GREEN); 
  // resetStatusControl();
  timeDisplayAccess=millis();

}

void displayChecking(String line1, String description){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(description);
  lcd.blink();
  tone(3,NOTE_C8,700);
  costume.lightONChecking();
  timeDisplayAccess=millis();

}




























