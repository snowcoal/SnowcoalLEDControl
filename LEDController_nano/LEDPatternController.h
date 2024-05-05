/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * LEDPatternController
 * ================================================================
 *
 */

#ifndef LEDPatternController_h
#define LEDPatternController_h

#include "LEDTypes.h"

class LEDPattern;

class LEDPatternController {
  public:
    LEDPatternController(const int numLEDs, const int maxPatterns);
    ~LEDPatternController();

    void addPattern(LEDPattern* pattern);
    void startPattern(int patternIdx);
    void cyclePattern();
    void step();

    int getRed(int LEDIdx);
    int getGreen(int LEDIdx);
    int getBlue(int LEDIdx);

    LED_t* getLEDs();
    int getNumLEDs();

  private:

    LED_t* _LEDs;
    int _numLEDs;
    LEDPattern** _patterns;
    int _currentPattern;
    int _numUsedPatterns;
    int _maxPatterns;
};



#endif