/************************************************************************************
/*  File:   mSphericalHarmonics.h                                             
/*    Desc:    SH stuff
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   Sep 2004                                          
/************************************************************************************/
#ifndef __SPHERICALHARMONICS_H__
#define __SPHERICALHARMONICS_H__

inline float sh00( float teta, float phi )
{
    return  sqrtf( 1.0f/(4.0f*c_PI ) );
}

inline float sh11( float teta, float phi )
{
    return sqrtf( 3.0f/(4.0f*c_PI) )*sinf( phi )*sqrtf( 1.0f - cosf( teta )*cosf( teta ) );
}

inline float sh01( float teta, float phi )
{
    return sqrtf( 3.0f/(4.0f*c_PI ) )*cosf( teta );
}

inline float sh11( float teta, float phi )
{
    return sqrtf( 3.0f/(4.0f*c_PI) )*cosf( phi )*sqrtf( 1.0f - cosf( teta )*cosf( teta ) );
}

#endif // __SPHERICALHARMONICS_H__