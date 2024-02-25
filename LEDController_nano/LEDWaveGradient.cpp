#include "LEDWaveGradient.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "Arduino.h"
/*
 * Copyright 2024 snowcoal
 * 
 * ================================================================
 * LEDWaveGradient
 * ================================================================
 *
 */


/*
 * Constructor
 * 
 * maxWaves - sets max number of allowed waves to swap between. More waves = more ram used.
 * 
 * numLEDs - number of LEDs to power. More LEDs = more ram used.
 *
 */
LEDWaveGradient::LEDWaveGradient(int maxWaves, int numLEDs){
  _numLEDs = numLEDs;
  _numUsedWaves = 0;
  _maxWaves = maxWaves;
  _currentWave = 0;
  _pointCount = 0;

  // create set of LEDs
  _LEDs = (LED_t*)malloc(sizeof(LED_t) * _numLEDs);
  // create waves
  _waves = (wave_t*)malloc(sizeof(wave_t) * _maxWaves);
}


/*
 * Destructor
 */
LEDWaveGradient::~LEDWaveGradient(){
  free(_LEDs);

  // free waves
  for(int i = 0; i < _numUsedWaves; i++){
    free(_waves[_numUsedWaves].wavePoints);
    free(_waves[_numUsedWaves].stepWidths);
  }
  free(_waves);
}


/*
 * Adds a new wave
 * 
 * numWavePoints - number of wave points to add, max of 255. More points = more ram used.
 *                 The program also interpolates between points so only add the points that
 *                 are actually needed for the gradient to look nice.
 * 
 * waveWidth - wave width in terms of number of LEDs, makes the wave thinner or wider
 *             The total number of LEDs should be divisible by this number for best results
 *
 * If more waves then maxWaves are added, they will overwrite the last wave
 *
 */
void LEDWaveGradient::addWave(int numWavePoints, int waveWidth){
  // fill in wave struct
  _waves[_numUsedWaves].wavePoints = (wavePoint_t*)malloc(sizeof(wavePoint_t) * numWavePoints);
  _waves[_numUsedWaves].stepWidths = (stepWidth_t*)malloc(sizeof(stepWidth_t) * numWavePoints);
  _waves[_numUsedWaves].numWavePoints = numWavePoints;
  _waves[_numUsedWaves].waveWidth = waveWidth;

  // _numUsedWaves is bounded by maxWaves
  _numUsedWaves = _numUsedWaves >= _maxWaves ? _maxWaves - 1 : _numUsedWaves + 1;
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
int LEDWaveGradient::addPoint(uint8_t red, uint8_t green, uint8_t blue, uint8_t duration){
  int waveIdx = _numUsedWaves - 1;

  if(waveIdx >= _numUsedWaves || waveIdx < 0){
    return -1;
  }

  wavePoint_t* wave = _waves[waveIdx].wavePoints;
  int numWavePoints = _waves[waveIdx].numWavePoints;

  // dont allow more points to be added after max
  if(_pointCount >= _waves[waveIdx].numWavePoints){
    return -1;
  }

  wave[_pointCount].duration = duration;
  wave[_pointCount].color.red = red;
  wave[_pointCount].color.green = green;
  wave[_pointCount].color.blue = blue;

  _pointCount++;

  // initialize wave if on last point
  if(_pointCount == numWavePoints){
    startWave(waveIdx);
    _pointCount = 0;
  }

  return 0;
}


/*
 * Called after all points have been added to a wave, sets this wave to current
 * 
 * waveIdx - which wave to initialize
 * 
 * Returns 0 on success, -1 on error.
 *
 */
int LEDWaveGradient::startWave(int waveIdx){
  if(waveIdx >= _numUsedWaves || waveIdx < 0){
    return -1;
  }

  _currentWave = waveIdx;

  wavePoint_t* wave = _waves[waveIdx].wavePoints;
  int numWavePoints = _waves[waveIdx].numWavePoints;
  int waveWidth = _waves[waveIdx].waveWidth;
  stepWidth_t* stepWidths = _waves[waveIdx].stepWidths;

  // calculate all the step widths
  for(int i = 0; i < numWavePoints - 1; i++){
    calcStepWidth(&(wave[i].color), &(wave[i+1].color), wave[i].duration, &(stepWidths[i]));
  }

  // calculate last step width which goes from last point back to first point
  calcStepWidth(&(wave[numWavePoints - 1].color), &(wave[0].color), wave[numWavePoints - 1].duration, &(stepWidths[numWavePoints - 1]));

  // initialize LEDs
  int numWaves = _numLEDs / waveWidth;
  int numLEDsPerWave = _numLEDs / numWaves;

  for(int i = 0; i < numWaves; i++){
    for(int j = 0; j < numLEDsPerWave; j++){
        // rescale led wave positions from [0, numLEDsPerWave] to [0, numWavePoints]
        float pointPos = ((float)j * numWavePoints) / (float)numLEDsPerWave;

        // get integer and decimal from the point position
        uint8_t pointIdx = (uint8_t)floor(pointPos);
        float subPoint = pointPos - (float)pointIdx;

        // rescale subpoint position from (0, 1) to (0, duration of point) to get current step
        float step = subPoint * (float)wave[pointIdx].duration;

        // calculate color = point start + step * step width
        int LEDIdx = i * numLEDsPerWave + j;
        _LEDs[LEDIdx].color.red = (uint8_t)round((float)wave[pointIdx].color.red + (step * stepWidths[pointIdx].dR));
        _LEDs[LEDIdx].color.green = (uint8_t)round((float)wave[pointIdx].color.green + (step * stepWidths[pointIdx].dG));
        _LEDs[LEDIdx].color.blue = (uint8_t)round((float)wave[pointIdx].color.blue + (step * stepWidths[pointIdx].dB));
        _LEDs[LEDIdx].curPoint = pointIdx;
        _LEDs[LEDIdx].stepCount = (uint8_t)step;
      }
  }

  return 0;
}

/*
 * Steps 1 timestep, and calculates the new colors for each LED
 *
 * This should be done once per iteration in loop()
 */
void LEDWaveGradient::step(){
  wavePoint_t* wave = _waves[_currentWave].wavePoints;
  int numWavePoints = _waves[_currentWave].numWavePoints;
  stepWidth_t* stepWidths = _waves[_currentWave].stepWidths;

  // loop through each LED
  for(int i = 0; i < _numLEDs; i++){
    int curPointIdx = _LEDs[i].curPoint;

    // calculate new color = current location + step width
    float newRed = (float)_LEDs[i].color.red + stepWidths[curPointIdx].dR;
    float newGreen = (float)_LEDs[i].color.green + stepWidths[curPointIdx].dG;
    float newBlue = (float)_LEDs[i].color.blue + stepWidths[curPointIdx].dB;

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
    if(_LEDs[i].stepCount >= wave[curPointIdx].duration - 1){
      // either move to the next point or go back to the first point
      _LEDs[i].curPoint = curPointIdx >= numWavePoints - 1 ? 0 : curPointIdx + 1;

      // reset step counter
      _LEDs[i].stepCount = 0;

      curPointIdx = _LEDs[i].curPoint;
      // set colors to next point colors
      _LEDs[i].color.red = wave[curPointIdx].color.red;
      _LEDs[i].color.green = wave[curPointIdx].color.green;
      _LEDs[i].color.blue = wave[curPointIdx].color.blue;
    }
    else{
      // increase count only when it doesnt go back to zero
      _LEDs[i].stepCount++;
    }
  }
}


/*
 * Gets red color of an LED
 */
int LEDWaveGradient::getRed(int LEDIdx){
  return _LEDs[LEDIdx].color.red;
}


/*
 * Gets green color of an LED
 */
int LEDWaveGradient::getGreen(int LEDIdx){
  return _LEDs[LEDIdx].color.green;
}


/*
 * Gets blue color of an LED
 */
int LEDWaveGradient::getBlue(int LEDIdx){
  return _LEDs[LEDIdx].color.blue;
}

/*
 * Calculates the width of a single step
 */
void LEDWaveGradient::calcStepWidth(color_t *c1, color_t *c2, int duration, stepWidth_t *output){
  output->dR = ((float)((int)c2->red - (int)c1->red)) / (float)duration;
  output->dG = ((float)((int)c2->green - (int)c1->green)) / (float)duration;
  output->dB = ((float)((int)c2->blue - (int)c1->blue)) / (float)duration;
}

