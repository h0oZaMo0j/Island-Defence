/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* islandDefence.h
***************************************************************************/

#ifndef ISLAND_DEFENCE_H
#define ISLAND_DEFENCE_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <utility>
#include <cstdio>
#include <ctime>
#include "boat.h"
#include "island.h"
#include "missile.h"
#include "defence.h"
#include "global.h"
#include "wave.h"

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

#endif	//ISLAND_DEFENCE_H
