/*****************************************************************/
/*  File:   Statistics.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "kStatistics.h"

float GetAverageFPS()
{
    return Stats::GetAverageFPS();
}

/*****************************************************************/
/*  Stats implementation
/*****************************************************************/
float        Stats::FPS;
int            Stats::frames;
int            Stats::totalSeconds;
int            Stats::totalFrames;
float        Stats::counter        [c_MaxEvents];
char*        Stats::counterName    [c_MaxEvents];
int            Stats::nCounters = 0;

Stats        Stats::inst;
int            Stats::checkTime = 0;

char        Stats::nameBuf[c_MaxCounterNameSymbols];
char*        Stats::pFreeNameBuf;    

Stats::Stats() 
{
    pFreeNameBuf = nameBuf;
}

float Stats::GetFPS()
{
    return FPS;
}

float Stats::GetAverageFPS()
{
    return static_cast<float>( totalFrames ) / 
                static_cast<float>( totalSeconds );
}

void Stats::OnFrame()
{
    int time = GetTickCount();
    if (time >= checkTime)
    {
        checkTime = time + 1000;
        FPS = static_cast<float>( frames );
        totalSeconds++;
        totalFrames += frames;

        frames = 0;
    }
    else 
    {
        frames++;
    }

    for (int i = 0; i < c_MaxEvents; i++)
    {
        counter[i]    = 0.0f;
    }
}

float Stats::GetCounterValue( int eventID )
{
    return counter[eventID];
}

void Stats::IncrementCounter( int valID, float amount )
{
    counter[valID] += amount;
} // Stats::IncrementCounter
    
float Stats::GetCounterValue( const char* eventName )
{
    for (int i = 0; i < nCounters; i++)
    {
        if (!strcmp( eventName, counterName[i])) 
        {
            return counter[i];
        }
    }
    return 0;
} // Stats::GetCounterValue
    
int    Stats::RegisterCounter( const char* name )
{
    //  look if already registered
    for (int i = 0; i < nCounters; i++)
    {
        if (!strcmp( name, counterName[i])) 
        {
            return i;
        }
    }
    //  new counter => register
    strcpy( pFreeNameBuf, name );
    counterName[nCounters] = pFreeNameBuf;
    counter[nCounters] = 0;
    pFreeNameBuf += strlen( name ) + 1;
    nCounters++;
    return nCounters - 1;
} // Stats::RegisterCounter
