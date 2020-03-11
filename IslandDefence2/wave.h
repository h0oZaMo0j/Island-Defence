/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* wave.h
***************************************************************************/

#ifndef WAVE_H
#define WAVE_H

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
#include <tuple>
#include "global.h"
#include "helpers.h"

// Class for sine wave
class Wave
{
public:
    int segments;           // Number of water segments
    Sinewave swA;
    Sinewave swB;
    std::vector<Sinewave> waves;

    Wave() { }
    Wave(int, Sinewave, Sinewave);
    ~Wave() { }
    void drawWater(Global);
    void drawWaterTangents(Global);
    void drawWaterNormals(Global);
    void drawWaterBinormals(Global);
    float calculateY(float, Global);
    float calculateTangent(float, Global);
};

#endif	//WAVE_H
