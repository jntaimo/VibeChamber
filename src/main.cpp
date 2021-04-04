#include <Arduino.h>
#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
#include <Audio.h>


// GUItool: begin automatically generated code
AudioInputUSB            usb1;           //xy=178,201
AudioAnalyzeFFT256       fft256_R;       //xy=357,396
AudioAnalyzeRMS          rmsR;           //xy=368,335
AudioAnalyzeRMS          rmsL;           //xy=414,71
AudioAnalyzeFFT256       fft256_L;       //xy=415,32
AudioMixer4              mixer1;         //xy=465,150
//AudioOutputUSB           usb2;           //xy=652,200
AudioAnalyzeRMS          rmsLR;           //xy=682,132
AudioOutputI2S           i2s1;           //xy=765,358
AudioConnection          patchCord1(usb1, 0, fft256_L, 0);
// AudioConnection          patchCord2(usb1, 0, usb2, 0);
AudioConnection          patchCord3(usb1, 0, rmsL, 0);
AudioConnection          patchCord4(usb1, 0, mixer1, 0);
AudioConnection          patchCord5(usb1, 1, fft256_R, 0);
//AudioConnection          patchCord6(usb1, 1, usb2, 1);
AudioConnection          patchCord7(usb1, 1, rmsR, 0);
AudioConnection          patchCord8(usb1, 1, mixer1, 1);
AudioConnection          patchCord9(mixer1, rmsLR);

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);

  // wait for a second
  delay(1000);

  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);

   // wait for a second
  delay(1000);
}