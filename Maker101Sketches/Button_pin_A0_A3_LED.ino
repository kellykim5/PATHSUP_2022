/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin[4] = {A0,A1,A2,A3};  // the numbers of the pushbutton pins
const int ledPin[4] =  {6,5,4,3};      // the number of the LED pin

// variables will change:
int buttonState[4] = {0,0,0,0};         // variable for reading the pushbutton status

void setup() {
  for (int i =0; i<=3; i++) {
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin[i], INPUT);
    // initialize the LED pin as an output:
    pinMode(ledPin[i], OUTPUT);
  }
}

void loop() {
  for (int i =0; i<=3; i++) {
    // read the state of the pushbutton value:
    buttonState[i] = digitalRead(buttonPin[i]);
    
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState[i]) {
      // turn LED on:
      digitalWrite(ledPin[i], HIGH);
    } else {
      // turn LED off:
      digitalWrite(ledPin[i], LOW);
    }
  }  
}
