/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* missile.h
***************************************************************************/

#ifndef MISSILE_H
#define MISSILE_H

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

#include <vector>
#include "helpers.h"
#include "wave.h"
#include "global.h"
#include "boat.h"

class Boat;
class Island;
class Defence;

// Class for missiles
class Missile
{
public:
    vec3f pos;
    vec3f v;
    double radius;
    int slices, stacks;
    float r, g, b;
    bool isHostile;

    Missile() { }
    Missile(vec3f, vec3f, double, int, int, float, float, float, bool);
    ~Missile() { }
    void drawMissile();
    void drawTrajectory(Wave, Global, float);
    bool checkCollision(std::vector<Defence>&, Wave, Global);
    bool checkCollision(Boat, Wave, Global);
    bool checkCollision(std::vector<Boat>&, Wave, Global);
    bool checkCollision(Island, Wave, Global);
};

#endif	//MISSILE_H
