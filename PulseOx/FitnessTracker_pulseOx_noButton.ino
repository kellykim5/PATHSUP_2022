/***************************************************************
Tinyscreen + Pulse Oximeter

Just displays the pulse oximeter information
(no buttons)

Code modified by Kelly Kim

***************************************************************/

#include <SPI.h>
#include <TinyScreen.h>  //interfacing OLED screen
#include <Wire.h>
#include <Wireling.h>
#include <MAX30101.h>

/* TinyScreen+ is SAMD */
#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

/* pulse ox : connect pulse ox to port 1 */
MAX30101 pulseSensor = MAX30101();
int pulseSensorPort = 1;

/* Tinyscreen object */
TinyScreen display = TinyScreen(TinyScreenPlus);

void setup() {

  /* wireling setup */
  Wire.begin();
  Wireling.begin();
  Wireling.selectPort(pulseSensorPort);

  /* TinyScreen setup */
  display.begin();
  display.setBrightness(10);

  /* Serial monitor setup */
  SerialMonitorInterface.begin(9600);

  /* pulse ox sensor setup and begin */
  if (pulseSensor.begin()) {
    while (true) {
      delay(1000);
    }
  }
  
  delay(200);

  welcome();

}

void loop() {

  // pulseSensor.update() is necessary!! 
  if (pulseSensor.update()){

      //SerialMonitorInterface.println("In Pulse Ox function");
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
  
  delay(20);//Polling the sensor too often can cause extra noise. The sensor can buffer about 300ms of data with the default settings.
  
}

// display "PULSE OX" at the beginning
// this can be deleted
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
