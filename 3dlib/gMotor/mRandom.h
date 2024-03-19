/*****************************************************************
/*  File:   mRandom.h                                             
/*    Desc:    Random values utility functions                          
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#ifndef __MRANDOM_H__
#define __MRANDOM_H__

_inl float    rndValuef    ();
_inl float    rndValuef    ( float min, float max );
_inl int    rndValue    ( int min, int max );
void    rndInit        ( int seed );
int        rndValue    ();

#ifdef _INLINES
#include "mRandom.inl"
#endif

#endif // __MRANDOM_H__