/*****************************************************************************/
/*    File:    mNoise.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    06-26-2003
/*****************************************************************************/
#ifndef __MNOISE_H__
#define __MNOISE_H__

_inl float Noise            ( int x, int y );
_inl float SmoothedNoise    ( int x, int y );
_inl float PerlinNoise        ( float x, float y );
_inl float PerlinNoise        ( float x, float y, float z );
_inl float PerlinNoise        ( float x, float y, float z, float t );

#ifdef _INLINES
#include "mNoise.inl"
#endif // _INLINES

#endif // __MNOISE_H__