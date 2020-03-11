/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* boat.cpp
***************************************************************************/

#include "boat.h"

Boat::Boat(int myHealth, int myMaxHealth, float myR, float myG, float myB,
    float myBody[], float myIncrement[], float myScale, float myRotZ, float myRotY,
    float myW, float myL, float myD, GLUquadric* myQobj, float myCannonL, float myCannonR,
    float myFireTimer, float myFireDelay, vec3f myPos, float myV)
{
    health = myHealth;
    maxHealth = myMaxHealth;
    r = myR; g = myG; b = myB;
    body[hull] = myBody[hull];
    body[cannon] = myBody[cannon];
    increment[hull] = myIncrement[hull];
    increment[cannon] = myIncrement[cannon];
    scale = myScale;
    rotZ = myRotZ; rotY = myRotY;
    w = myW; l = myL; d = myD;
    cannonL = myCannonL; cannonR = myCannonR;
    fireTimer = myFireTimer; fireDelay = myFireDelay;
    pos = myPos;
    v = myV;
}

// Draw hull of boat
void Boat::drawHull()
{
    glPushMatrix();
    glColor3f(r, g, b);
    glScalef(l, w, d);
    glutSolidCube(1);
    glPopMatrix();
}

// Draw cannon of boat
void Boat::drawCannon()
{
    /*glPushMatrix();
    glColor3f(r, g, b);
    glScalef(1, 1, 1);
    gluCylinder(gluNewQuadric(), 0.25, 0.25, 2, 64, 1);
    glPopMatrix();*/

    glPushMatrix();

    glColor3f(r, g, b);
    float x, y;
    float radius = cannonR;
    float height = cannonL;
    float angle = 0.0f;
    float angle_stepsize = 0.1f;

    //Draw tube
    glBegin(GL_QUAD_STRIP);
    angle = 0.0f;
    while (angle < 2 * M_PI) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        glVertex3f(x, y, 0.0f);
        angle = angle + angle_stepsize;
    }

    glVertex3f(radius, 0.0f, height);
    glVertex3f(radius, 0.0f, 0.0f);

    glEnd();

    //Draw circle on top of cylinder
    glBegin(GL_POLYGON);
    angle = 0.0f;
    while (angle < 2 * M_PI) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        angle = angle + angle_stepsize;
    }

    glVertex3f(radius, 0.0f, height);

    glEnd();

    glPopMatrix();
}

// Draw boat
void Boat::drawBoat(Wave wave, Global global)
{
    glPushMatrix();

    // Boat joint
    glTranslatef(pos.x, wave.calculateY(pos.x, global), pos.z);
    
    //Calculate z-rotation of boat using tangent of sine wave
    rotZ = radToDeg(atanf(wave.calculateTangent(pos.x, global)));
    // Boat Rotation
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glScalef(scale, scale, scale);
    // Axes
    drawAxes();
    // Hull
    drawHull();
    // Cannon joint
    //glTranslatef(cannonX, cannonY, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(-body[cannon], 1.0f, 0.0f, 0.0f);
    
    // Cannon
    drawCannon();

    glPopMatrix();
}

// Fire boat missile
void Boat::fireMissile(std::vector<Missile> &m, float force, Wave wave, Global global)
{
    // Calculate x,y components of boat rotation
    float rx = cos(degToRad(rotZ));
    float ry = sin(degToRad(rotZ));

    // Calculate x,y components for combination of boat and cannon rotation
    float rrx = rx * cos(degToRad(body[cannon])) - ry * sin(degToRad(body[cannon]));
    float rry = rx * sin(degToRad(body[cannon])) + ry * cos(degToRad(body[cannon]));

    float rrx2 = rrx * cos(degToRad(rotY));
    float rrz = rrx * -sin(degToRad(rotY));
    
    // Calculate spawn position for missile (at end of cannon)
    float spawnX = pos.x + scale * cannonL * rrx2;
    float spawnY = wave.calculateY(pos.x, global) + scale * cannonL * rry;
    float spawnZ = pos.z + scale * cannonL * rrz;

    // Instantiate missile
    Missile missile = { { spawnX, spawnY, spawnZ },
    { rrx2 * force, rry * force, rrz * force }, 0.02, 8, 10, 1, 1, 1, true };

    // Add missile to vector for iteration
    m.push_back(missile);
}

// Fire boat defence
void Boat::fireDefence(std::vector<Defence> &d, float force, Wave wave, Global global)
{
    // Calculate x,y components of boat rotation
    float rx = cos(degToRad(rotZ));
    float ry = sin(degToRad(rotZ));

    // Calculate x,y components for combination of boat and cannon rotation
    float rrx = rx * cos(degToRad(body[cannon])) - ry * sin(degToRad(body[cannon]));
    float rry = rx * sin(degToRad(body[cannon])) + ry * cos(degToRad(body[cannon]));

    // Calculate spawn position for missile (at end of cannon)
    float spawnX = body[hull] + cannonL * scale * rrx;
    float spawnY = wave.calculateY(body[hull], global) + cannonL * scale * rry;

    // Instantiate missile
    Defence defence = { { spawnX, spawnY },
    { rrx * force, rry * force }, 0.02, 8, 10, 1, 1, 1, 0.1, 0, 1.25f, 0.5f };

    // Add missile to vector for iteration
    d.push_back(defence);
}

// Draw health bar
void Boat::drawHealthBar(float barW, float barH)
{
    glPushMatrix();
    glBegin(GL_QUAD_STRIP);
    glColor3f(r, g, b);
    for (int i = 0; i <= health; ++i)
    {
        glVertex2f(i * barW / maxHealth, barH / 2);
        glVertex2f(i * barW / maxHealth, -barH / 2);
    }
    glEnd();
    glPopMatrix();
}

// Positions and orients boat around island
void Boat::positionBoat(float d)
{
    // Select random x-coord between 0 and max spawn distance from island
    float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / d));

    // Calculate appropriate z-coord to ensure boat spawns at correct distance
    float z = sqrtf(d * d - x * x);

    
    // Randomly decide whether coords are positive or negative
    int r = rand() % 2;   
    if (r == 0)
        x = -x;

    r = rand() % 2;
    if (r == 0)
        z = -z;

    // Set coords
    pos.x = x;
    pos.z = z;

    // Calculate boat rotation to face island
    float angle = abs(atanf(z/x));

    // Find supplementary angle if x > 0
    if (x > 0)
        angle = degToRad(180) - angle;
    // Find inverse if z < 0
    if (z < 0)
        angle = -angle;

    // Set y-axis rotation
    rotY = radToDeg(angle);
}

// Update position
void Boat::move(Global g)
{
    // x axis
    if(pos.x < 0)
        pos.x += v * g.dt;
    else
        pos.x -= v * g.dt;

    // z axis
    if (pos.z < 0)
        pos.z += v * g.dt;
    else
        pos.z -= v * g.dt;
}

// Collision with island
bool Boat::checkCollision(Island i, Wave wave, Global global)
{

    // Base of island
    if (pos.x > i.x - i.w / 2 - l / 2 * scale && pos.x < i.x + i.w / 2 + l / 2 * scale
        && pos.z > i.x - i.w / 2 - l / 2 * scale && pos.z < i.x + i.w / 2 + l / 2 * scale)
    {
        return true;
    }

    return false;
}