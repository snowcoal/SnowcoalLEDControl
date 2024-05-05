/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * LEDPattern
 * ================================================================
 *
 */

#ifndef LEDPattern_h
#define LEDPattern_h

#include "stdlib.h"
#include "stdint.h"

#include "LEDTypes.h"

class LEDPatternController;

class LEDPattern {
  public:
    // will have to update the LEDs after 1 time step
    virtual void step();
    // will have to set the initial LED values of the pattern
    virtual void init();
    
    void associateController(LEDPatternController* controller);
    int getRed(int LEDIdx);
    int getGreen(int LEDIdx);
    int getBlue(int LEDIdx);

  protected:
    LED_t *_LEDs;
    int _numLEDs;
};

#endif