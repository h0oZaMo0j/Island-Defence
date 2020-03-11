/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* defence.cpp
***************************************************************************/

#include "defence.h"

Defence::Defence(vec3f myPos, vec3f myV, double myRadius, int mySlices, int myStacks,
    float myR, float myG, float myB, double myIncrement, float myLingerTimer, 
    float myLingerDelay, float myGravMultiplier)
{
    pos = myPos;
    v = myV;
    radius = myRadius;
    slices = mySlices; stacks = myStacks;
    r = myR; g = myG; b = myB;
    increment = myIncrement;
    lingerTimer = myLingerTimer; lingerDelay = myLingerDelay;
    gravMultiplier = myGravMultiplier;
}

// Draw defence missile
void Defence::drawDefence()
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glColor3f(r, g, b);
    glutWireSphere(radius, slices, stacks);
    glPopMatrix();
}

// Collision with sine wave
bool Defence::checkCollision(Wave wave, Global global)
{
    if (pos.y < radius + wave.calculateY(pos.x, global))
        return true;

    return false;
}

// Collision with island
bool Defence::checkCollision(Island i, Wave wave, Global global)
{

    // Base of island
    if (pos.x > i.x - i.w / 2 && pos.x < i.x + i.w / 2
        && pos.y < i.y + i.l / 2
        && pos.z > i.x - i.w / 2 && pos.z < i.x + i.w / 2)
    {
        return true;
    }

    return false;
}