/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* helpers.cpp
***************************************************************************/

#include "helpers.h"

// Convert between rad and deg
float radToDeg(float r)
{
    return r * 180 / (float)M_PI;
}
float degToRad(float d)
{
    return d * (float)M_PI / 180;
}

// Draw vector <a,b> starting at (x,y) with scale s
void drawVector(float x, float y, float z, float a, float b, float c, float s, bool normalize, float R, float G, float B)
{
    glBegin(GL_LINES);
    glColor3f(R, G, B);

    // Starting coords
    glVertex3f(x, y, z);

    // Normalize vector
    if (normalize)
    {
        a /= sqrtf(a * a + b * b + c * c);
        b /= sqrtf(a * a + b * b + c * c);
        c /= sqrtf(a * a + b * b + c * c);
    }

    // Scale vector
    a *= s;
    b *= s;
    c *= s;

    // Vector addition
    a = (a + x);
    b = (b + y);
    c = (c + z);

    // Ending coords
    glVertex3f(a, b, c);

    glEnd();
}

// Draw set of x/y axes
void drawAxes()
{
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(2, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 2, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 2);
    glEnd();
    glPopMatrix();
}

// Calculate the y-value of a sine wave
float calcSineWave(Sinewave sw, float x, bool dvs, float t)
{
    // Calculate derivative?
    if (!dvs)
    {
        return sw.A * sin(sw.k / sw.w * x + sw.w * t);
    }
    else
    {
        return sw.A * sw.k / sw.w * cos(sw.k / sw.w * x + sw.w * t);
    }
}

// Calculate the sum of two sine waves
float calcSineWaveSum(std::vector<Sinewave> s, float x, bool dvs, float t)
{
    float y = 0;

    for (int i = 0; i < (int)s.size(); ++i)
    {
        y += calcSineWave(s[i], x, dvs, t);
    }

    return y;

}