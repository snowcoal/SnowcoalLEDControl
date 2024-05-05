/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * LEDPatternController
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

/*
 * Constructor
 *
 * numLEDs - number of LEDs to power. More LEDs = more ram used.
 *
 * maxPatterns - max number of allowed patterns
 *
 */
LEDPatternController::LEDPatternController(const int numLEDs, const int maxPatterns) {
  _patterns = (LEDPattern**) malloc(maxPatterns * sizeof(LEDPattern*));
  _LEDs = (LED_t*) malloc(numLEDs * sizeof(LED_t));

  _currentPattern = 0;
  _numUsedPatterns = 0;
  _maxPatterns = maxPatterns;
  _numLEDs = numLEDs;
}

/*
 * Destructor - does not delete each pattern
 *
 */
LEDPatternController::~LEDPatternController() {
  free(_LEDs);
  free(_patterns);
}

/*
 * Set the current Pattern to display
 * 
 */
void LEDPatternController::startPattern(int patternIdx) {
  if(patternIdx < _numUsedPatterns && patternIdx > 0){
    _currentPattern = patternIdx;
    _patterns[_currentPattern]->init();
  }
}

/*
 * Shifts the current pattern to the next one in the list
 * 
 */
void LEDPatternController::cyclePattern() {
  _currentPattern = (_currentPattern + 1) % _numUsedPatterns;
  startPattern(_currentPattern);
}

/*
 * Steps 1 timestep
 * 
 */
void LEDPatternController::step() {
  _patterns[_currentPattern]->step();
}

/*
 * Add a new light pattern to the controller
 * 
 * pattern - pattern to add, must exist already. If the pattern array
 *           is full, it overwrites the last one
 *
 */
void LEDPatternController::addPattern(LEDPattern* pattern) {
  _patterns[_numUsedPatterns] = pattern;
  // _numUsedPatterns is bounded by _maxPatterns
  _numUsedPatterns = fmin(_numUsedPatterns + 1, _maxPatterns - 1);
  // associate this controller
  pattern->associateController(this);
}

/*
 * Retrieve the amount of LEDs
 *
 */
int LEDPatternController::getNumLEDs() {
  return _numLEDs;
}
   
/*
 * Retrieve the LED array
 *
 */
LED_t* LEDPatternController::getLEDs() {
  return _LEDs;
}

/*
 * gets the red value from the current pattern
 * 
 * LEDIdx - the light to get the value for
 *
 */
int LEDPatternController::getRed(int LEDIdx) {
  return _patterns[_currentPattern]->getRed(LEDIdx);
}

/*
 * gets the green value from the current pattern
 * 
 * LEDIdx - the light to get the value for
 *
 */
int LEDPatternController::getGreen(int LEDIdx) {
  return _patterns[_currentPattern]->getGreen(LEDIdx);
}

/*
 * gets the blue value from the current pattern
 * 
 * LEDIdx - the light to get the value for
 *
 */
int LEDPatternController::getBlue(int LEDIdx) {
  return _patterns[_currentPattern]->getBlue(LEDIdx);
}