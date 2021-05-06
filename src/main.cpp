#include <Arduino.h>

#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
#include <Audio.h>



// GUItool: begin automatically generated code
AudioInputUSB            usb1;           //xy=98,759
AudioAnalyzeFFT256       fft256_R;       //xy=298,1015
AudioAnalyzeFFT256       fft256_L;       //xy=303,406
AudioAnalyzeRMS          rms_L;           //xy=313,460
AudioAnalyzeRMS          rms_R;           //xy=325,948
AudioAnalyzePeak         peak_L;          //xy=332,513
AudioAnalyzePeak         peakR;          //xy=348,899
AudioMixer4              mixer1;         //xy=391,760
AudioAnalyzeRMS          rmsLR;          //xy=655,687
AudioAnalyzeNoteFrequency notefreq1;      //xy=659,791
AudioOutputI2S           i2s1; //xy=803,975
AudioConnection          patchCord1(usb1, 0, fft256_L, 0);
AudioConnection          patchCord2(usb1, 0, rms_L, 0);
AudioConnection          patchCord3(usb1, 0, mixer1, 0);
AudioConnection          patchCord4(usb1, 0, peak_L, 0);
AudioConnection          patchCord5(usb1, 1, fft256_R, 0);
AudioConnection          patchCord6(usb1, 1, rms_R, 0);
AudioConnection          patchCord7(usb1, 1, mixer1, 1);
AudioConnection          patchCord8(usb1, 1, peakR, 0);
AudioConnection          patchCord9(mixer1, rmsLR);
AudioConnection          patchCord10(mixer1, notefreq1);
// GUItool: end automatically generated code


//Set up LEDs
#define NUM_LEDS 127
#define DATA_PIN 20
#define BRIGHTNESS 100
//#define CLOCK_PIN 13
CRGB leds[NUM_LEDS];
uint8_t FFT[NUM_LEDS];
float floatFFT[NUM_LEDS];

//FFT Display storage
//Assumes half of LED strip for left FFT and half for right

float logbase(uint16_t x, float base);
uint16_t logscale(uint16_t x, uint16_t xMax, uint16_t logMax, float base);
void updateFFTLog();
void updateFFTLinear();
void floatToIntFFTDumb(uint16_t scalar);
void updateLEDs();

void setup()
{
  // initialize LED digital pin as an output.
  FastLED.addLeds<WS2812SERIAL,DATA_PIN,BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  fill_rainbow(leds, NUM_LEDS, 0, 5);
  FastLED.show();
  AudioMemory(20);
  fft256_R.averageTogether(20);
}

void loop()
{

  EVERY_N_MILLISECONDS(10){updateFFTLinear();floatToIntFFTDumb(400);}
  EVERY_N_MILLISECONDS(10){updateLEDs();}
}

//Returns the log_base(x) 
//uses the change-of-base formula
//base should be greater than 1
//https://www.purplemath.com/modules/logrules5.htm 
float logbase(uint16_t x, float base){
  return log(x)/log(base);
}

//Converts a normal scale to logarithmic
//assumes the minimum of both the n scale and logarithmic scale are zero
//  xMax is the maximum value on the x scale input
//  logMax is the maximum value on the logarithmic scale output
//  base is the base of the log
uint16_t logscale(uint16_t x, uint16_t xMax, uint16_t logMax, float base){
  if (x == 0) return 0;
  float k = logMax/logbase(xMax, base);
  return k*logbase(x, base);
}

//Reads the current Fast Fourier Transform Values
//Replaces them in the FFT Array
void updateFFTLog(){
  uint16_t startBin = 0; 
  for (int i = 0; i < NUM_LEDS; i++){
    uint16_t endBin = logscale(i, NUM_LEDS, 127, 20);
    floatFFT[i] = fft256_R.read(startBin, endBin);
    startBin = endBin;
  }
}

void updateFFTLinear(){
  uint16_t startBin = 0; 
  uint16_t maxBin = 127;
  uint16_t binWidth = maxBin/NUM_LEDS;
  for (int i = 0; i < NUM_LEDS; i++){
    uint16_t endBin =  startBin + binWidth;
    floatFFT[i] = fft256_R.read(startBin, endBin);
    startBin = endBin;
  }
}

//Converts the float values for the FFT into 8 bit integer values
//multiplies the float values by a constant scalar
void floatToIntFFTDumb(uint16_t scalar){
  for (int i = 0; i < NUM_LEDS; i++){
    uint8_t val = min(255, floatFFT[i]*scalar);
    FFT[i] = val;
  }
}

//updates the LED colors
void updateLEDs(){
  for (int i = 0; i < NUM_LEDS; i++){
    uint8_t brightness = FFT[i];
    leds[i] = CRGB(0, brightness, 0);
  }
  FastLED.show();
}