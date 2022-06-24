/*
  Combination of code from TinyCircuits (http://tinycircuits.com)
  and Eloquent Arduino (https://eloquentarduino.github.io)

  Combined and modified by Joseph Young.

  In particular, see:
  https://eloquentarduino.github.io/2019/12/word-classification-using-arduino/
  https://eloquentarduino.github.io/2019/11/how-to-train-a-classifier-in-scikit-learn/
  https://github.com/eloquentarduino/EloquentArduino/blob/master/deprecated/examples/MicromlWakeWordIdentificationExample/MicromlWakeWordIdentificationExample.ino
*/

#include <arduinoFFT.h>
// uncomment when doing classification
#include "model.h"
#define MIC A0
#define NUM_SAMPLES 64
#define SAMPLING_FREQUENCY 1024
#define INTERVAL 5
#define SOUND_THRESHOLD 3

// this class will be different if you used another type of classifier, just check the model.h file
Eloquent::ML::Port::RandomForest classifier;

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

#include <Wireling.h> // Interface with Wirelings 

unsigned int samplingPeriod;
unsigned long microSeconds;

int32_t backgroundSound;
double features[NUM_SAMPLES];
float  ffeatures[NUM_SAMPLES];
arduinoFFT fft;

void setup() {
    Wireling.begin();
    
    SerialMonitorInterface.begin(9600);
    while (!SerialMonitorInterface); // must open Serial Monitor to execute following code
  
    delay(3000);
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));
    calibrate();

}

void loop() {
    if (!soundDetected()) {
        delay(10);
        return;
    }

    captureWord();
    printFeatures();

// uncomment when doing classification
    SerialMonitorInterface.print("You said ");

    for (uint16_t i = 0; i < NUM_SAMPLES; i++) {
        ffeatures[i] = float(features[i]);
    }
    
    SerialMonitorInterface.println(classifier.predictLabel(ffeatures));

   delay(1000);

}

/**
    Get analog readings
    @return
    */
int16_t readMic() {
    return analogRead(MIC);
    return (analogRead(MIC) - 512) >> 2;
}

/**
    Get "ambient" volume
    */
void calibrate() {
    for (int i = 0; i < 200; i++)
        backgroundSound += readMic();

    backgroundSound /= 200;

    SerialMonitorInterface.print("Threshold set at ");
    SerialMonitorInterface.println(backgroundSound);
}

bool soundDetected() {
    return abs(readMic() - backgroundSound) >= SOUND_THRESHOLD;
}

void captureWord() {
    for (uint16_t i = 0; i < NUM_SAMPLES; i++) {
        microSeconds = micros();
        features[i] = readMic();

        while(micros() < (microSeconds + samplingPeriod));
    }

    fft.Windowing(features, NUM_SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);

}

void printFeatures() {
    const uint16_t numFeatures = sizeof(features) / sizeof(double);

    for (int i = 0; i < numFeatures; i++) {
        SerialMonitorInterface.print(features[i]);
        SerialMonitorInterface.print(i == numFeatures - 1 ? '\n' : ',');
    }

}
