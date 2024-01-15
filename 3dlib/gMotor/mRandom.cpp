/*****************************************************************
/*  File:   mRandom.cpp                                      
/*    Desc:    Random values utility functions                          
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#include "stdafx.h"
#include "mRandom.h"

#ifndef _INLINES
#include "mRandom.inl"
#endif

static int s_LastRnd = 1;
int rndValue()
{
    return (((s_LastRnd = s_LastRnd * 214013L + 2531011L) >> 16) & 0x7fff);
}

void rndInit( int seed )
{
    s_LastRnd = seed;
}