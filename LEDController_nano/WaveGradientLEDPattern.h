/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * WaveGradientLEDPattern
 * ================================================================
 *
 */

#ifndef WaveGradientLEDPattern_h
#define WaveGradientLEDPattern_h

#include "stdlib.h"
#include "stdint.h"

#include "LEDTypes.h"
#include "LEDPatternController.h"
#include "LEDPattern.h"

typedef struct wavePoint_t{
  color_t color;
  int duration;
} wavePoint_t;

typedef struct stepWidth_t{
  float dR;
  float dG;
  float dB;
} stepWidth_t;

class WaveGradientLEDPattern : public LEDPattern {
    public:
      // constructor
      WaveGradientLEDPattern(int numWavePoints, int waveWidth);
      // destructor
      ~WaveGradientLEDPattern();
      // add point to wave, last color should be the same as the first color
      int addPoint(uint8_t red, uint8_t green, uint8_t blue, uint8_t duration);
      // starts the wave
      void init();
      // steps the wave
      void step();

    private:
      // calculates the direction vector for the line going from color 1 to color 2
      void calcStepWidth(color_t *c1, color_t *c2, int duration, stepWidth_t *output);

      int _numWavePoints;
      int _waveWidth;
      int _pointCount;

      wavePoint_t *_wavePoints;
      stepWidth_t *_stepWidths;
};

#endif