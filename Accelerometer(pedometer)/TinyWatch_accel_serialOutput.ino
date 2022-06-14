/***************************************************************
Tinyscreen + Accelerometer

Simply display X, Y, Z values from the accelerometer

Code by Kelly Kim

***************************************************************/

/* Libraries */
#include <TinyScreen.h>
#include <Wire.h>         // For I2C communication with sensor
#include "BMA250.h"       // For interfacing with the accel. sensor

/* Accelerometer sensor variables for the sensor and its values */
BMA250 accel_sensor;

/* serial communication */
#if defined(ARDUINO_ARCH_SAMD)
 #define SerialMonitorInterface SerialUSB
#else
 #define SerialMonitorInterface Serial
#endif

void setup() {

  /* Start the serial monitor interface */
  SerialMonitorInterface.begin(115200);

  /* Wire setup */
  Wire.begin();

  /* Set up the BMA250 acccelerometer sensor */
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); 

  delay(200);
}

void loop() {
  
  /* read from the sensor */
  accel_sensor.read();

  /* display the X, Y, Z values */
  SerialMonitorInterface.print("X = ");
  SerialMonitorInterface.print(accel_sensor.X);
  
  SerialMonitorInterface.print("  Y = ");
  SerialMonitorInterface.print(accel_sensor.Y);
  
  SerialMonitorInterface.print("  Z = ");
  SerialMonitorInterface.println(accel_sensor.Z);

  /* Without the delay, there would not be any sensor output */
  delay(100);
}
