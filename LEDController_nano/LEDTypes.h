/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * LEDWaveGradient
 * ================================================================
 *
 */

#ifndef LEDTypes_h
#define LEDTypes_h

#include "stdlib.h"
#include "stdint.h"

typedef struct color_t{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} color_t;

typedef struct LED_t{
  color_t color;
  uint8_t curPoint;
  uint8_t stepCount;
} LED_t;

#endif