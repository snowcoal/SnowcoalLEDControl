#include <FastLED.h>
#include "LEDTypes.h"
#include "LEDPattern.h"
#include "LEDPatternController.h"
#include "WaveGradientLEDPattern.h"
#include "StaticColorLEDPattern.h"

#define MEGA
// #define NANO
// #define NANO_TEST

// nano settings
#ifdef NANO
#define LED_PIN 2
#define MAX_WAVES 1
#define NUM_LEDS 195
#define PT_DURATION 50
#define WAVE_WIDTH 195
#define MAX_PATTERNS 1
#endif

// nano testing settings
#ifdef NANO_TEST
#define LED_PIN 2
#define MAX_WAVES 1
#define NUM_LEDS 30
#define PT_DURATION 100
#define WAVE_WIDTH 30
#define MAX_PATTERNS 1
#endif

// mega settings
#ifdef MEGA
#define LED_PIN 35
#define MAX_WAVES 1
#define NUM_LEDS 100
#define PT_DURATION 100
#define WAVE_WIDTH 30
#define MAX_PATTERNS 10
#endif

CRGB leds[NUM_LEDS];

WaveGradientLEDPattern* _WaveGradientLEDPattern_0;
WaveGradientLEDPattern* _WaveGradientLEDPattern_1;
WaveGradientLEDPattern* _WaveGradientLEDPattern_2;

StaticColorLEDPattern* _StaticColorLEDPattern_0;

LEDPatternController* _LEDPatternController;
int count;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(255);
  FastLED.clear();

  _LEDPatternController = new LEDPatternController(NUM_LEDS, MAX_PATTERNS);

  _WaveGradientLEDPattern_0 = new WaveGradientLEDPattern(6, WAVE_WIDTH);
  _LEDPatternController->addPattern(_WaveGradientLEDPattern_0);
  _WaveGradientLEDPattern_0->addPoint(239, 0, 255, PT_DURATION);
  _WaveGradientLEDPattern_0->addPoint(160, 0, 255, PT_DURATION);
  _WaveGradientLEDPattern_0->addPoint(37, 0, 255, PT_DURATION);
  _WaveGradientLEDPattern_0->addPoint(0, 181, 255, PT_DURATION);
  _WaveGradientLEDPattern_0->addPoint(37, 0, 255, PT_DURATION);
  _WaveGradientLEDPattern_0->addPoint(160, 0, 255, PT_DURATION);

  _WaveGradientLEDPattern_1 = new WaveGradientLEDPattern(4, WAVE_WIDTH);
  _LEDPatternController->addPattern(_WaveGradientLEDPattern_1);
  _WaveGradientLEDPattern_1->addPoint(0x00, 0x9E, 0x49, PT_DURATION);
  _WaveGradientLEDPattern_1->addPoint(0x40, 0x4E, 0x68, PT_DURATION);
  _WaveGradientLEDPattern_1->addPoint(0x68, 0x21, 0x7A, PT_DURATION);
  _WaveGradientLEDPattern_1->addPoint(0x40, 0x4E, 0x68, PT_DURATION);

  _WaveGradientLEDPattern_2 = new WaveGradientLEDPattern(4, WAVE_WIDTH);
  _LEDPatternController->addPattern(_WaveGradientLEDPattern_2);
  _WaveGradientLEDPattern_2->addPoint(0xFF, 0xF1, 0x00, PT_DURATION);
  _WaveGradientLEDPattern_2->addPoint(0xF2, 0x6C, 0x15, PT_DURATION);
  _WaveGradientLEDPattern_2->addPoint(0xE8, 0x11, 0x23, PT_DURATION);
  _WaveGradientLEDPattern_2->addPoint(0xF2, 0x6C, 0x15, PT_DURATION);

  _StaticColorLEDPattern_0 = new StaticColorLEDPattern(255, 255, 255);
  _LEDPatternController->addPattern(_StaticColorLEDPattern_0);

  _LEDPatternController->startPattern(0);

}


void loop() {

  _LEDPatternController->step();
 
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(_LEDPatternController->getRed(i), _LEDPatternController->getGreen(i), _LEDPatternController->getBlue(i));
  }
  _LEDPatternController->step();

  FastLED.show();

  count++;
  if(count > 500){
    count = 0;
    _LEDPatternController->cyclePattern();
  }
}




