/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * WaveGradientLEDPattern
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
#include "WaveGradientLEDPattern.h"


/*
 * Constructor
 * 
 * maxWaves - sets max number of allowed waves to swap between. More waves = more ram used.
 * 
 */
WaveGradientLEDPattern::WaveGradientLEDPattern(int numWavePoints, int waveWidth){
  _pointCount = 0;
  _LEDs = NULL;
  _numLEDs = 0;

  // fill in wave struct
  _wavePoints = (wavePoint_t*)malloc(sizeof(wavePoint_t) * numWavePoints);
  _stepWidths = (stepWidth_t*)malloc(sizeof(stepWidth_t) * numWavePoints);
  
  _numWavePoints = numWavePoints;
  _waveWidth = waveWidth;
}


/*
 * Destructor
 */
WaveGradientLEDPattern::~WaveGradientLEDPattern(){
  free(_wavePoints);
  free(_stepWidths);
}


/*
 * Adds a gradient point to the newest wave
 * 
 * duration - number of steps before wave changes to next point, max of 255
 *            Higher means the wave goes slower, smaller means it goes faster
 *            Exact timing will depend on how many LEDs there are, as more LEDs
 *            means the overall loop runs slower and vise versa.
 *            Note that the wave speed can also be made slower by adding a simple
 *            delay in the main loop.
 * 
 * red, green, blue - RGB colors for point
 * 
 * Returns 0 on success, -1 on error.
 *
 * Automatically initializes wave and set its to the current one, once all the points are added
 *
 */
int WaveGradientLEDPattern::addPoint(uint8_t red, uint8_t green, uint8_t blue, uint8_t duration){

  // dont allow more points to be added after max
  if(_pointCount >= _numWavePoints){
    return -1;
  }

  _wavePoints[_pointCount].duration = duration;
  _wavePoints[_pointCount].color.red = red;
  _wavePoints[_pointCount].color.green = green;
  _wavePoints[_pointCount].color.blue = blue;

  _pointCount++;

  // initialize wave if on last point
  if(_pointCount == _numWavePoints){
    init();
    _pointCount = 0;
  }

  return 0;
}


/*
 * Called after all points have been added to a wave, sets this wave to current
 *
 */
void WaveGradientLEDPattern::init(){
  // calculate all the step widths
  for(int i = 0; i < _numWavePoints - 1; i++){
    calcStepWidth(&(_wavePoints[i].color), &(_wavePoints[i+1].color), _wavePoints[i].duration, &(_stepWidths[i]));
  }

  // calculate last step width which goes from last point back to first point
  calcStepWidth(&(_wavePoints[_numWavePoints - 1].color), &(_wavePoints[0].color), _wavePoints[_numWavePoints - 1].duration, &(_stepWidths[_numWavePoints - 1]));

  // initialize LEDs
  int numWaves = _numLEDs / _waveWidth;
  int numLEDsPerWave = _numLEDs / numWaves;

  for(int i = 0; i < numWaves; i++){
    for(int j = 0; j < numLEDsPerWave; j++){
        // rescale led wave positions from [0, numLEDsPerWave] to [0, _numWavePoints]
        float pointPos = ((float)j * _numWavePoints) / (float)numLEDsPerWave;

        // get integer and decimal from the point position
        uint8_t pointIdx = (uint8_t)floor(pointPos);
        float subPoint = pointPos - (float)pointIdx;

        // rescale subpoint position from (0, 1) to (0, duration of point) to get current step
        float step = subPoint * (float)_wavePoints[pointIdx].duration;

        // calculate color = point start + step * step width
        int LEDIdx = i * numLEDsPerWave + j;
        _LEDs[LEDIdx].color.red = (uint8_t)round((float)_wavePoints[pointIdx].color.red + (step * _stepWidths[pointIdx].dR));
        _LEDs[LEDIdx].color.green = (uint8_t)round((float)_wavePoints[pointIdx].color.green + (step * _stepWidths[pointIdx].dG));
        _LEDs[LEDIdx].color.blue = (uint8_t)round((float)_wavePoints[pointIdx].color.blue + (step * _stepWidths[pointIdx].dB));
        _LEDs[LEDIdx].curPoint = pointIdx;
        _LEDs[LEDIdx].stepCount = (uint8_t)step;
      }
  }
}

/*
 * Steps 1 timestep, and calculates the new colors for each LED
 *
 * This should be done once per iteration in loop()
 */
void WaveGradientLEDPattern::step(){
  // loop through each LED
  for(int i = 0; i < _numLEDs; i++){
    int curPointIdx = _LEDs[i].curPoint;

    // calculate new color = current location + step width
    float newRed = (float)_LEDs[i].color.red + _stepWidths[curPointIdx].dR;
    float newGreen = (float)_LEDs[i].color.green + _stepWidths[curPointIdx].dG;
    float newBlue = (float)_LEDs[i].color.blue + _stepWidths[curPointIdx].dB;

    newRed = fmin(255.0, newRed);
    newRed = fmax(0.0, newRed);
    newGreen = fmin(255.0, newGreen);
    newGreen = fmax(0.0, newGreen);
    newBlue = fmin(255.0, newBlue);
    newBlue = fmax(0.0, newBlue);

    _LEDs[i].color.red = (uint8_t)round(newRed);
    _LEDs[i].color.green = (uint8_t)round(newGreen);
    _LEDs[i].color.blue = (uint8_t)round(newBlue);

    // case where LED moves to next point
    if(_LEDs[i].stepCount >= _wavePoints[curPointIdx].duration - 1){
      // either move to the next point or go back to the first point
      _LEDs[i].curPoint = curPointIdx >= _numWavePoints - 1 ? 0 : curPointIdx + 1;

      // reset step counter
      _LEDs[i].stepCount = 0;

      curPointIdx = _LEDs[i].curPoint;
      // set colors to next point colors
      _LEDs[i].color.red = _wavePoints[curPointIdx].color.red;
      _LEDs[i].color.green = _wavePoints[curPointIdx].color.green;
      _LEDs[i].color.blue = _wavePoints[curPointIdx].color.blue;
    }
    else{
      // increase count only when it doesnt go back to zero
      _LEDs[i].stepCount++;
    }
  }
}

/*
 * Calculates the width of a single step
 */
void WaveGradientLEDPattern::calcStepWidth(color_t *c1, color_t *c2, int duration, stepWidth_t *output){
  output->dR = ((float)((int)c2->red - (int)c1->red)) / (float)duration;
  output->dG = ((float)((int)c2->green - (int)c1->green)) / (float)duration;
  output->dB = ((float)((int)c2->blue - (int)c1->blue)) / (float)duration;
}

