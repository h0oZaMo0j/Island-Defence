/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* missile.cpp
***************************************************************************/

#include "missile.h"

Missile::Missile(vec3f myPos, vec3f myV, double myRadius, int mySlices, int myStacks, 
    float myR, float myG, float myB, bool hostileFlag)
{
    pos = myPos;
    v = myV;
    radius = myRadius;
    slices = mySlices; stacks = myStacks;
    r = myR; g = myG; b = myB;
    isHostile = hostileFlag;
}

// Draw missile
void Missile::drawMissile()
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glColor3f(r, g, b);
    glutSolidSphere(radius, slices, stacks);
    glPopMatrix();
}

// Draw missile trajectory
void Missile::drawTrajectory(Wave wave, Global global, float gravity)
{
    float r0 = pos.x;
    float v0 = v.x;
    float r1 = pos.y;
    float v1 = v.y;
    float r2 = pos.z;
    float v2 = v.z;

    glPushMatrix();
    glBegin(GL_LINE_STRIP);
    glColor3f(r, g, b);
    for (float i = 0; i < 1; i += .0005f)
    {
        // Position
        r0 += v0 * i;
        r1 += v1 * i;
        r2 += v2 * i;

        // Velocity
        v1 += gravity * i;

        // Only draw above sine wave
        if (r1 > wave.calculateY(r0, global))
            glVertex3f(r0, r1, r2);
    }
    glEnd();
    glPopMatrix();
}

// Collision with sine wave and defences
bool Missile::checkCollision(std::vector<Defence> &defences, Wave wave, Global global)
{
    if (pos.y < radius + wave.calculateY(pos.x, global))
        return true;
    for (auto &d : defences)
    {
        if ((pos.x > d.pos.x - d.radius && pos.x < d.pos.x + d.radius)
            && (pos.y > d.pos.y - d.radius && pos.y < d.pos.y + d.radius)
            && (pos.z > d.pos.z - d.radius && pos.z < d.pos.z + d.radius))
        {
            return true;
        }           
    }

    return false;
}

// Collision with boat
bool Missile::checkCollision(Boat boat, Wave wave, Global global)
{
    float x0 = pos.x;
    float y0 = pos.y;
    float z0 = pos.z;
    float x1 = boat.pos.x;
    float y1 = wave.calculateY(boat.pos.x, global);
    float z1 = boat.pos.z;

    // Check x/y
    float c = sqrt((x0 - x1)*(x0 - x1) + (y0 - y1) * (y0 - y1));

    if (c < radius + 0.5f * boat.scale)
        return true;
    
    // Check x/z
    c = sqrt((x0 - x1)*(x0 - x1) + (z0 - z1)*(z0 - z1));
        
    if (c < radius + 0.5f * boat.scale)
        return true;

    return false;
}

bool Missile::checkCollision(std::vector<Boat> &boats, Wave wave, Global global)
{
    for (auto &b : boats)
    {
        if (checkCollision(b, wave, global) && isHostile == false)
        {
            // Reduce boat health
            --b.health;
            return true;
        }
    }

    return false;
}

// Collision with island
bool Missile::checkCollision(Island i, Wave wave, Global global)
{

    // Base of island
    if (pos.x > i.x - i.w / 2 - radius && pos.x < i.x + i.w / 2 + radius
        && pos.y < i.y + i.l / 2 + radius
        && pos.z > i.x - i.w / 2 - radius && pos.z < i.x + i.w / 2 + radius)
    {
        return true;
    }

    return false;
}