/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* island.h
***************************************************************************/

#ifndef ISLAND_H
#define ISLAND_H

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
#include "helpers.h"
#include "missile.h"
#include "defence.h"

class Missile;
class Defence;

class Island
{
public:
    int health;
    int maxHealth;
    float r, g, b;
    float x, y;
    float w, l;
    float turretW, turretL;
    float turretRot;
    float baseRot;
    float increment;
    float fireTimer;
    float fireDelay;
    vec3f cannonPos;
    vec3f power;

    Island() { }
    Island(int, int, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    ~Island() { }
    void drawIsland();                                  // Draw island
    void drawCannon();
    void drawCylinder();                                // Draw island turret
    void fireMissile(std::vector<Missile>&, float);     // Fire missile
    void drawHealthBar(float, float);                   // Draw health bar
    void drawTrajectory(Wave, Global, float);          // Draw missile trajectory
    void fireDefence(std::vector<Defence>&, float);       // Fire defence
};

#endif	//ISLAND_H
