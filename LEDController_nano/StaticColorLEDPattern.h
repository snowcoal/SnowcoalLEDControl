/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * StaticColorLEDPattern
 * ================================================================
 *
 */

#ifndef StaticColorLEDPattern_h
#define StaticColorLEDPattern_h

#include "stdlib.h"
#include "stdint.h"

#include "LEDTypes.h"
#include "LEDPatternController.h"
#include "LEDPattern.h"

class StaticColorLEDPattern : public LEDPattern {
    public:
      // constructor
      StaticColorLEDPattern(int red, int green, int blue);
      // destructor
      ~StaticColorLEDPattern();
      // starts
      void init();
      // steps
      void step();

    private:
      uint8_t _red;
      uint8_t _green;
      uint8_t _blue;
};

#endif