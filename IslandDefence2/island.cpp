/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* island.cpp
***************************************************************************/

#include "island.h"

Island::Island(int myHealth, int myMaxHealth, float myR, float myG, float myB, 
    float myX, float myY, float myW, float myL, float myTurretW, float myTurretL, 
    float myTurretRot, float myBaseRot, float myIncrement, float myFireTimer, float myFireDelay)
{
    health = myHealth;
    maxHealth = myMaxHealth;
    r = myR; g = myG; b = myB;
    x = myX; y = myY;
    w = myW; l = myL;
    turretW = myTurretW; turretL = myTurretL;
    turretRot = myTurretRot;
    baseRot = myBaseRot;
    increment = myIncrement;
    fireTimer = myFireTimer; 
    fireDelay = myFireDelay;

    //cannonPos = {0, 2.0f, 0.25f};
}

// Draw island
void Island::drawIsland()
{
    /*glPushMatrix();
    glTranslatef(x, y, 0);
    glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex2f(-w / 2, l / 2);
    glVertex2f(-w / 2, -l / 2);
    glVertex2f(w / 2, -l / 2);
    glVertex2f(w / 2, l / 2);
    glEnd();
    // Turret joint
    glTranslatef(0, l / 2, 0);
    glRotatef(turretRot, 0.0f, 0.0f, 1.0f);
    // Turret
    glTranslatef(turretL / 2, 0, 0);
    drawTurret();
    glPopMatrix();*/
    glPushMatrix();
    glColor3f(0.5, 0.35, 0.05);

    glTranslatef(0, -0.4f, 0);
    glRotatef(-90.0, 1.0f, 0.0f, 0.0f);

    float r = 0.75;
    const int slices = 15, stacks = 10;
    float theta, phi;
    float x1, y1, z1, x2, y2, z2;
    float step_theta = 2.0 * M_PI / slices, step_phi = M_PI / stacks;

    for (int j = 0; j <= stacks / 2; j++) {
        phi = j / (float)stacks / 2 * M_PI;
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            theta = i / (float)slices * 2.0 * M_PI;
            x1 = r * sinf(phi) * cosf(theta);
            y1 = r * sinf(phi) * sinf(theta);
            z1 = r * cosf(phi);
            x2 = r * sinf(phi + step_phi) * cosf(theta);
            y2 = r * sinf(phi + step_phi) * sinf(theta);
            z2 = r * cosf(phi + step_phi);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }

    glPopMatrix();

}

void Island::drawCannon() {
    glPushMatrix();

    //glTranslatef(0, -0.25, 0);
    glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
    drawCylinder();

    glRotatef(90.0, 1.0f, 0.0f, 0.0f);
    glScalef(0.5, 0.5, 0.5);
    // Manage rotation of cannon base
    glRotatef(baseRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.25, 1.1, 0);
    glRotatef(-90.0, 0.0f, 1.0f, 0.0f);
 
    drawCylinder();

    glRotatef(90.0, 0.0f, 1.0f, 0.0f);
    glScalef(0.4, 0.4, 0.4);
    // Manage rotation of cannon
    glRotatef(turretRot, 1.0f, 0.0f, 0.0f);
    glTranslatef(-0.6, 0.1, 0.4);
    drawCylinder();

    glPopMatrix();

}

// Draw cylinder
void Island::drawCylinder()
{
    /*glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2f(-turretL / 2, -turretW / 2);
    glVertex2f(turretL / 2, -turretW / 2);
    glVertex2f(turretL / 2, turretW / 2);
    glVertex2f(-turretL / 2, turretW / 2);
    glEnd();
    glPopMatrix();*/

    glPushMatrix();


    glColor3f(0.0, 0.5, 0.5);
    float x, y;
    float radius = 0.2;
    float height = 0.5;
    float angle = 0.0;
    float angle_stepsize = 0.1;

    //Draw tube
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while (angle < 2 * M_PI) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glTexCoord2f(1, 1);
        glVertex3f(x, y, height);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, 0.0);
        angle = angle + angle_stepsize;
    }

    glTexCoord2f(1, 1);
    glVertex3f(radius, 0.0, height);
    glTexCoord2f(0, 0);
    glVertex3f(radius, 0.0, 0.0);

    glEnd();

    //Draw circle on top of cylinder
    glBegin(GL_POLYGON);
    angle = 0.0;
    while (angle < 2 * M_PI) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glTexCoord2f(1, 1);
        glVertex3f(x, y, height);
        angle = angle + angle_stepsize;
    }

    glTexCoord2f(0, 0);
    glVertex3f(radius, 0.0, height);

    glEnd();

    glPopMatrix();
}

// Fire island missile
void Island::fireMissile(std::vector<Missile> &m, float force)
{
    // Calculate vector components
    float rx = sin(degToRad(baseRot));
    float ry = -sin(degToRad(turretRot));
    float rz = cos(degToRad(baseRot));

    // Calculate spawn position for missile (at end of cannon)
    float spawnX = x + rx * turretL * sin(degToRad(turretRot));
    float spawnY = y + l - turretW;
    float spawnZ = x + rz * turretL * sin(degToRad(turretRot));

    Missile missile = { { spawnX, spawnY, spawnZ },
    { rx * force * 1.7f, ry * force * 2, rz * force * 2 }, 0.02, 8, 10, 1, 1, 1, false };

    m.push_back(missile);
}

// Draw health bar
void Island::drawHealthBar(float barW, float barH)
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

// Draw missile trajectory
void Island::drawTrajectory(Wave wave, Global global, float gravity)
{
    float r0 = cannonPos.x;
    float v0 = power.x;
    float r1 = cannonPos.y;
    float v1 = power.y;
    float r2 = cannonPos.z;
    float v2 = power.z;

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

// Fire island defence
void Island::fireDefence(std::vector<Defence> &d, float force)
{
    // Calculate vector components
    float rx = sin(degToRad(baseRot));
    float ry = -sin(degToRad(turretRot));
    float rz = cos(degToRad(baseRot));

    // Calculate spawn position for missile (at end of cannon)
    float spawnX = x + rx * turretL * sin(degToRad(turretRot));
    float spawnY = y + l - turretW;
    float spawnZ = x + rz * turretL * sin(degToRad(turretRot));

    Defence defence = { { spawnX, spawnY, spawnZ },
    { rx * force, ry * force, rz * force }, 0.01, 8, 10, 1, 1, 1, 0.2, 0, 5.0f, 0.5f };

    d.push_back(defence);
}