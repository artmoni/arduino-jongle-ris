/**
 * Simple xBee comunication example
 * by Jaime Patarroyo
 *
 * communicates two Wiring boards using xBee's modules,
 * pressing the button attached  to one Wiring, will 
 * light the board LED (48 for Wiring 1.0 boards, pin 
 * 15 for Wiring S) on the other board.
 */

// digital pin to attach the switch
int WLED = 8;

// variable to store the data received
int data;

void setup()
{
  // Starts serial communication
  Serial.begin(9600);
  // set pin as output
  pinMode(WLED, OUTPUT);
digitalWrite(WLED, LOW);
}

void loop()
{
  // if the switch is pressed
  delay(1000);
  // send the character 'a' though the xBee

  Serial.print('e');
  

  // if data available
  if (Serial.available())
  {
    // read data
    data = Serial.read();

    // if data received is 'a'
    if (data == 'a')
    {
      // turn ON the board LED
      digitalWrite(WLED, HIGH);
      Serial.print("turn ON");
    }
    else
    {
      // if not turn if OFF
      digitalWrite(WLED, LOW);
      Serial.print("turn OFF");
    }
  }

  // wait 100ms
  delay(100);
}

