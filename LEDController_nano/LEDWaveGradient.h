#ifndef LEDWaveGradient_h
#define LEDWaveGradient_h

#include "stdlib.h"
#include "stdint.h"

/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * LEDWaveGradient
 * ================================================================
 *
 */

typedef struct color_t{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} color_t;

typedef struct wavePoint_t{
  color_t color;
  int duration;
} wavePoint_t;

typedef struct stepWidth_t{
  float dR;
  float dG;
  float dB;
} stepWidth_t;

typedef struct LED_t{
  color_t color;
  uint8_t curPoint;
  uint8_t stepCount;
} LED_t;

typedef struct wave_t{
  wavePoint_t *wavePoints;
  stepWidth_t *stepWidths;
  int numWavePoints;
  int waveWidth;
} wave_t;

class LEDWaveGradient {
    public:
      // constructor
      LEDWaveGradient(int maxWaves, int numLEDs);
      // destructor
      ~LEDWaveGradient();
      // adds a new wave
      void addWave(int numWavePoints, int waveWidth);
      // add point to wave, last color should be the same as the first color
      int addPoint(uint8_t red, uint8_t green, uint8_t blue, uint8_t duration);
      // starts a wave
      int startWave(int waveIdx);
      // step one timestep
      void step();
      // get red from idx
      int getRed(int LEDIdx);
      // get green from idx
      int getGreen(int LEDIdx);
      // get blue from idx
      int getBlue(int LEDIdx);

    private:
      // calculates the direction vector for the line going from color 1 to color 2
      void calcStepWidth(color_t *c1, color_t *c2, int duration, stepWidth_t *output);

      int _numLEDs;
      uint8_t _currentWave;
      int _numUsedWaves;
      int _maxWaves;
      int _pointCount;

      LED_t *_LEDs;
      wave_t *_waves;
};

#endif