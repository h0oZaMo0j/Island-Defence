/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* defence.h
***************************************************************************/

#ifndef DEFENCE_H
#define DEFENCE_H

#include "helpers.h"
#include "wave.h"
#include "global.h"
#include "island.h"

class Island;

// Class for defence projectiles
class Defence
{
public:
    vec3f pos;
    vec3f v;
    double radius;
    int slices, stacks;
    float r, g, b;
    double increment;
    float lingerTimer, lingerDelay;
    float gravMultiplier;

    Defence() { }
    Defence(vec3f, vec3f, double, int, int, float, float, float, double,
        float, float, float);
    ~Defence() { }
    void drawDefence();
    bool checkCollision(Wave, Global);
    bool checkCollision(Island, Wave, Global);
};

#endif	//DEFENCE_H
