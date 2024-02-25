#include <FastLED.h>
#include "LEDWaveGradient.h"

#define LED_PIN 2
#define NUM_LEDS 195
#define PT_DURATION 50
#define WAVE_WIDTH 195

CRGB leds[NUM_LEDS];

LEDWaveGradient *_LEDWaveGradient;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(255);
  FastLED.clear();

  _LEDWaveGradient = new LEDWaveGradient(2, NUM_LEDS);
  _LEDWaveGradient->addWave(6, 195);
  _LEDWaveGradient->addPoint(239, 0, 255, PT_DURATION);
  _LEDWaveGradient->addPoint(160, 0, 255, PT_DURATION);
  _LEDWaveGradient->addPoint(37, 0, 255, PT_DURATION);
  _LEDWaveGradient->addPoint(0, 181, 255, PT_DURATION);
  _LEDWaveGradient->addPoint(37, 0, 255, PT_DURATION);
  _LEDWaveGradient->addPoint(160, 0, 255, PT_DURATION);

  // _LEDWaveGradient->addWave(4, 65);
  // _LEDWaveGradient->addPoint(137, 41, 173, 100);
  // _LEDWaveGradient->addPoint(67, 106, 172, 100);
  // _LEDWaveGradient->addPoint(67, 183, 184, 100);
  // _LEDWaveGradient->addPoint(67, 106, 172, 100);

}

void loop() {
  // put your main code here, to run repeatedly:

  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(_LEDWaveGradient->getRed(i), _LEDWaveGradient->getGreen(i), _LEDWaveGradient->getBlue(i));
  }
  _LEDWaveGradient->step();

  FastLED.show();

}


