/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* missile.h
***************************************************************************/

#ifndef HELPERS_H
#define HELPERS_H

#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

// Used to convert to seconds
const int milli = 1000;

// Definition for vec2f
typedef struct { float x, y; } vec2f;

// Definition for vec3f
typedef struct { float x, y, z; } vec3f;

// Definition for a sine wave
typedef struct {
    float A;
    float k;
    float w;

} Sinewave;

// Convert between rad and deg
float radToDeg(float);
float degToRad(float);

// Draw vector 
void drawVector(float, float, float, float, float, float, float, bool, float, float, float);

// Draw axes
void drawAxes();

// Calculate the y-value of a sine wave
float calcSineWave(Sinewave, float, bool, float);

// Calculate the sum of two sine waves
float calcSineWaveSum(std::vector<Sinewave>, float, bool, float);

#endif	//HELPERS_H
