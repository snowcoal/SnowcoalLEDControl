/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * StaticColorLEDPattern
 * ================================================================
 *
 */

#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "Arduino.h"

#include "LEDTypes.h"
#include "LEDPattern.h"
#include "LEDPatternController.h"
#include "StaticColorLEDPattern.h"


/*
 * Constructor
 * 
 * red - static red color, green - static green color, blue - static blue color
 * 
 */
StaticColorLEDPattern::StaticColorLEDPattern(int red, int green, int blue) {
  _red = red;
  _blue = blue;
  _green = green;
}


/*
 * Destructor
 */
StaticColorLEDPattern::~StaticColorLEDPattern() {
  // unused
}


/*
 * Called after all points have been added to a wave, sets the static color
 *
 */
void StaticColorLEDPattern::init() {
  // calculate all the step widths
  for(int i = 0; i < _numLEDs; i++){
    _LEDs[i].color.red = _red;
    _LEDs[i].color.green = _green;
    _LEDs[i].color.blue = _blue;
  }
}

/*
 * Steps 1 timestep, and calculates the new colors for each LED
 *
 * This should be done once per iteration in loop()
 */
void StaticColorLEDPattern::step() {
  // unused, required for polymorphism
}

