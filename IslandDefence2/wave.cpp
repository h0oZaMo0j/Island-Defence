/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* missile.cpp
***************************************************************************/

#include "wave.h"

std::vector< std::tuple<float, float, float> > waterCoords;

Wave::Wave(int mySegments, Sinewave mySwA, Sinewave mySwB)
{
    segments = mySegments;
    swA = mySwA;
    swB = mySwB;
    
    // Add sinewaves to vector
    waves.push_back(swA);
    waves.push_back(swB);
}

// Draw water segments
void Wave::drawWater(Global g)
{
    float left = -3;
    float right = 3;
    float front = 3;
    float back = -3;
    float rangeX = right - left;
    float rangeZ = front - back;
    float xStep = rangeX / segments;
    float zStep = rangeZ / segments;

    waterCoords.clear();

    // Color w/ alpha
    glColor4f(0, 1, 1, 0.3f);

    for (int i = 0; i < segments; ++i)
    {
        glBegin(GL_QUAD_STRIP);

        float z = i * zStep + back;

        for (int j = 0; j <= segments; ++j)
        {
            // Calculate x value for current segment
            float x = j * xStep + left;

            // Displace y values using sine wave
            float y = calcSineWaveSum(waves, x, false, g.t);

            // Draw points
            glVertex3f(x, y, z);
            glVertex3f(x, y, z + zStep);

            // Normals
            glNormal3f(x, y, z);
            glNormal3f(x, y, + zStep);

            waterCoords.push_back(std::make_tuple(x, y, z));
        }

        glEnd();
    }  
}

// Draw tangents on water surface
void Wave::drawWaterTangents(Global g)
{
    float myX;
    float myY;
    float myZ;

    for (int i = 0; i < (int)waterCoords.size(); ++i)
    {
        myX = std::get<0>(waterCoords[i]);
        myY = std::get<1>(waterCoords[i]);
        myZ = std::get<2>(waterCoords[i]);

        // <a, b> for tangent is <1, dy/dx>
        drawVector(myX, myY, myZ, 1, calculateTangent(myX, g), 0, 0.1f, true, 1, 1, 1);
    }
}

// Draw normals on water surface
void Wave::drawWaterNormals(Global g)
{
    float myX;
    float myY;
    float myZ;

    for (int i = 0; i < (int)waterCoords.size(); ++i)
    {
        myX = std::get<0>(waterCoords[i]);
        myY = std::get<1>(waterCoords[i]);
        myZ = std::get<2>(waterCoords[i]);

        // <a, b> for normal is <-dy/dx, 1>
        drawVector(myX, myY, myZ,
            -calculateTangent(myX, g), 1, 0, 0.1f, true, 1, 1, 1);
    }
}

// Draw binormals on water surface
void Wave::drawWaterBinormals(Global g)
{
    float myX;
    float myY;
    float myZ;

    for (int i = 0; i < (int)waterCoords.size(); ++i)
    {
        myX = std::get<0>(waterCoords[i]);
        myY = std::get<1>(waterCoords[i]);
        myZ = std::get<2>(waterCoords[i]);

        // currently a straight line
        drawVector(myX, myY, myZ,
            0, 0, -myZ, 0.1f, true, 1, 1, 1);
    }
}

float Wave::calculateY(float x, Global g)
{
    return calcSineWaveSum(waves, x, false, g.t);
}

float Wave::calculateTangent(float x, Global g)
{
    return calcSineWaveSum(waves, x, true, g.t);       
}

