/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* global.h
***************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

// Struct for global time
typedef struct
{
    bool isActive;
    float startTime;
    float t;
    float pauseT;
    float dt;
    bool OSD;
    int frames;
    float frameRate;
    float frameRateInterval;
    float lastFrameRateT;
    float spawnBoatTimer;
    float spawnBoatDelay;
} Global;

#endif	//GLOBAL_H
