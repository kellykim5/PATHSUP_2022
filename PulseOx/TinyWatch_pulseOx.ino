/***************************************************************
Tinyscreen + Pulse Oximeter

Just displays the pulse oximeter information
(no buttons)

Code modified by Kelly Kim

***************************************************************/

/* include the necessary libraries */
#include <SPI.h>
#include <TinyScreen.h>  
#include <RTCZero.h>
#include <Wire.h>
#include <Wireling.h>
#include <MAX30101.h>

/* Pulse ox : connect pulse ox to port 1 */
MAX30101 pulseSensor = MAX30101(); //pulse ox object
int pulseSensorPort = 1;  //indicates port
bool pulseOxUpdate; //store variable for successful reading

/* Tinyscreen object */
TinyScreen display = TinyScreen(TinyScreenPlus);


void setup() {

  /* wireling setup */
  Wire.begin();
  Wireling.begin();
  Wireling.selectPort(pulseSensorPort);

  /* TinyScreen setup */
  display.begin();
  display.on();
  display.setBrightness(10);

  delay(200);

  /* displays "PULSE OX" at the beginning */
  welcome();

  /* pulse ox sensor setup and begin */
  if (pulseSensor.begin()) {
    while (true) {
      delay(1000);
    }
  }
}

void loop() {

  /* pulseSensor.update() returns a boolean */
  pulseOxUpdate = pulseSensor.update();

  /* if pulse sensor reading is successful, display it */
  if (pulseOxUpdate == true){
    display.setFont(thinPixel7_10ptFontInfo);
    display.fontColor(TS_8b_White,TS_8b_Black);
    display.setCursor(0,0);
    display.print("[Pulse ox data]");
    display.setCursor(0,15);
    display.print("Temp: ");
    display.print(int(pulseSensor.temperature()));
    display.print(" 'C");
    display.setCursor(0,30);
    display.print("BPM: ");
    display.println(int(pulseSensor.BPM()));
    display.setCursor(0,45);
    display.print("Oxygen level: ");
    display.print(int(pulseSensor.oxygen()));
    display.print(" %");
  }

  /* wait 100ms before reading the sensor again */
  delay(100); 
}

/* function to display "PULSE OX" at the beginning */
void welcome(){

  display.on();
  display.clearScreen();
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.fontColor(TS_8b_Yellow,TS_8b_Black);
  display.setCursor(15,25);
  display.print("PULSE OX");
  delay(2000);
  display.clearScreen();
  
}
