/***************************************************************
Tinyscreen + Accelerometer (pedometer)

Display Accelerometer data (no buttons)

Code by Kelly Kim

***************************************************************/
#include <TinyScreen.h>
#include <Wire.h>         // For I2C communication with sensor
#include "BMA250.h"       // For interfacing with the accel. sensor

/* Accelerometer sensor variables for the sensor and its values */
BMA250 accel_sensor;

/* variables for counting steps: used in updatePedometer() function */
int iter = 0; // this serves as a counter
int sample_interval = 40; //minimum interval a step can occur
                          //can change it but 40 seemed good experimentally
int sampleOld = 0; //saves the accelerometer info from previous loop
bool stepAlert = true; //flag to see if step occured. again, only one step can happen in 50 cycles.
int totalSteps = 0; //total number of steps

/* calibrated values for accelerometer */
float xavg, yavg, zavg;

/* serial communication */
#if defined(ARDUINO_ARCH_SAMD)
 #define SerialMonitorInterface SerialUSB
#else
 #define SerialMonitorInterface Serial
#endif

/* Tinyscreen object */
TinyScreen display = TinyScreen(TinyScreenPlus);

void setup() {
  
  /* Wire setup */
  Wire.begin();
  
  /* Tinyscreen display setup */
  display.begin();
  display.setBrightness(10);

  /* Set up the BMA250 acccelerometer sensor */
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); 

  /* display "PEDOMETER" at the beginning */
  welcome_accel();

  /* calibrate accelerometer sesor */
  calibrate();

  /* clear screen */
  display.clearScreen();
}

void loop() {

  /* Display steps */
  display.setCursor(5,15);
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.println("TOTAL STEPS");
  display.setCursor(40,35);
  display.println(totalSteps);

  /* calculate steps using accelerometer input */
  updatePedometer();
  
  /* without the delay, the sensor won't read! */
  delay(20);
}

/* calibrate accelerometer */
void calibrate(){
  
  float sumx=0;
  float sumy=0;
  float sumz=0;

  /* get average from 100 cycles */
  for (int i=0;i<100;i++){
    accel_sensor.read();
    
    sumx = sumx + accel_sensor.X;
    sumy = sumy + accel_sensor.Y;
    sumz = sumz + accel_sensor.Z;
  }
  
  xavg = sumx/100.0;
  yavg = sumy/100.0;
  zavg = sumz/100.0;
}

/* calculate steps */
void updatePedometer() {

  /* read sensor data (X, Y, Z) */
  accel_sensor.read();
  
  int sum = pow(accel_sensor.X - xavg, 2) + pow(accel_sensor.Y - yavg, 2) + pow(accel_sensor.Z - zavg, 2);
  int sqrt_sum = sqrt(sum);

  /* threshold for step counting. can change values */
  int threshold = 75; 
  
  float diff = abs(sqrt_sum - sampleOld);

  /* if difference exceeds threshold, and if this is the first step in the sample_interval */
  if (diff >= threshold && stepAlert && iter < sample_interval){
    
    totalSteps = totalSteps + 1; //step!
    stepAlert = false; // toggle flag
  }
  
  if (iter == sample_interval){
    
    stepAlert = true; //toggle flag
    iter = 0; //initialize iteration
  }

  iter++; //update iteration
  sampleOld = sqrt_sum; //"current" value is now "old" for the next iteration
}

/* displays "PEDOMTER" */
void welcome_accel(){
  display.on();
  display.clearScreen();
  display.setCursor(6,15);
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.fontColor(TS_8b_Yellow,TS_8b_Black);
  display.println("PEDOMETER");
  delay(3000); // wait 3 seconds
}
