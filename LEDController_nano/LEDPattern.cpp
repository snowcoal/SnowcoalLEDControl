/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * LEDPattern
 * ================================================================
 *
 */

#include "LEDTypes.h"
#include "LEDPatternController.h"
#include "LEDPattern.h"

/*
 * associates a pre-existing LED controller with the LEDPattern object
 *
 * this provides the LED_t array which the LEDPattern will use and set
 *
 * numLEDs - number of LEDs to power. More LEDs = more ram used.
 * 
 * LEDs - LED array to associate
 * 
 * 
 */
void LEDPattern::associateController(LEDPatternController* controller) {
  _numLEDs = controller->getNumLEDs();
  _LEDs = controller->getLEDs();
}

/*
 * Gets red color of an LED
 */
int LEDPattern::getRed(int LEDIdx){
  return _LEDs[LEDIdx].color.red;
}


/*
 * Gets green color of an LED
 */
int LEDPattern::getGreen(int LEDIdx){
  return _LEDs[LEDIdx].color.green;
}


/*
 * Gets blue color of an LED
 */
int LEDPattern::getBlue(int LEDIdx){
  return _LEDs[LEDIdx].color.blue;
}