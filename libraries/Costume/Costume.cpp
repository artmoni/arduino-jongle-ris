#include "Arduino.h"
#include "Costume.h"


Costume::Costume(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Costume::lightShoulder(int color)
{
  switch (color)
  {
  case COLOR_BLUE:
    digitalWrite(RUBANLED_LEFT_PIN_BLUE, HIGH);
    digitalWrite(RUBANLED_RIGHT_PIN_BLUE, HIGH);
    break;
  case COLOR_RED:
    digitalWrite(RUBANLED_LEFT_PIN_GREEN, HIGH);
    digitalWrite(RUBANLED_RIGHT_PIN_GREEN, HIGH);
    break;
  case COLOR_GREEN:
    digitalWrite(RUBANLED_LEFT_PIN_GREEN, HIGH);
    digitalWrite(RUBANLED_RIGHT_PIN_GREEN, HIGH);
    break;
  default:
    break;
  }

  delay(3000); 
  digitalWrite(RUBANLED_LEFT_PIN_BLUE, LOW);
  digitalWrite(RUBANLED_RIGHT_PIN_BLUE, LOW);
  digitalWrite(RUBANLED_LEFT_PIN_GREEN, LOW);
  digitalWrite(RUBANLED_RIGHT_PIN_GREEN, LOW);
  digitalWrite(RUBANLED_LEFT_PIN_GREEN, LOW);
  digitalWrite(RUBANLED_RIGHT_PIN_GREEN, LOW);
}

void Costume::lightButton()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}

void Costume::sendRFID(String rfid){
  Serial.print(rfid);

}








