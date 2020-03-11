/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* boat.h
***************************************************************************/

#ifndef BOAT_H
#define BOAT_H

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
#include "wave.h"
#include "global.h"
#include "missile.h"
#include "defence.h"
#include "island.h"

static enum { hull, cannon, n_joints } Joints;

class Missile;
class Defence;
class Island;

// Class for boats
class Boat
{
public:
    int health;
    int maxHealth;
    float r, g, b;
    float body[n_joints];
    float increment[n_joints];
    float scale;
    float rotZ;
    float rotY;
    float w, l, d;
    GLUquadric* qObj;
    float cannonL, cannonR;
    float fireTimer, fireDelay;
    vec3f pos;
    float v;

    Boat() { }
    Boat(int, int, float, float, float, float[], float[], float, float, float, 
        float, float, float, GLUquadric*, float, float, float, float, vec3f, float);
    ~Boat() { }
    void drawBoat(Wave, Global);
    void drawHull();
    void drawCannon();
    void fireMissile(std::vector<Missile>&, float, Wave, Global);
    void fireDefence(std::vector<Defence>&, float, Wave, Global);
    void drawHealthBar(float, float);
    void positionBoat(float);
    void move(Global);
    bool checkCollision(Island, Wave, Global);

};

#endif	//BOAT_H
