/*****************************************************************************/
/*	File:	mNoise.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-26-2003
/*****************************************************************************/
#include "mSplines.h"

_inl float LinearInterpolate( float a, float b, float alpha ) 
{
	return a*(1.0f - alpha) + b*alpha;
} // CosineInterpolate

_inl float Noise( int x, int y ) 
{
	int n = x + y * 57;
	n = ( n << 13 ) ^ n;
	return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff ) / 1073741824.0f );    
} // Noise

//  2D noise smoothed with Gaussian filter kernel
_inl float SmoothedNoise( int x, int y )
{
	float corners = Noise( x - 1, y - 1 ) + Noise( x - 1, y + 1 ) + Noise( x + 1, y - 1 ) + Noise( x + 1, y + 1 );
	corners /= 16.0f;
	float sides = Noise( x - 1, y ) + Noise( x + 1, y ) + Noise( x, y - 1 ) + Noise( x, y + 1 );
	sides /= 8.0f;
	float center = Noise( x, y );
	center /= 4.0f;
    return corners + sides + center;
} // SmoothedNoise

_inl float PerlinNoise( float x, float y ) 
{
	float nx = floorf( x );
	float ny = floorf( y );
	float fx = x - nx;
	float fy = y - ny;

	float lt = SmoothedNoise( nx,		ny		);
	float rt = SmoothedNoise( nx + 1,	ny		);
	float lb = SmoothedNoise( nx,		ny + 1	);
	float rb = SmoothedNoise( nx + 1,	ny + 1	);

	float t = CosineInterpolate( fx, lt, rt );
	float b = CosineInterpolate( fx, lb, rb );

	return CosineInterpolate( fy, t, b );
} // PerlinNoise

_inl float PerlinNoise( float x, float y, float z )
{
	return PerlinNoise( PerlinNoise( x, y ), z );
} // PerlinNoise

_inl float PerlinNoise( float x, float y, float z, float t )
{
	return PerlinNoise( PerlinNoise( x, y ), PerlinNoise( z, t ) );
} // PerlinNoise


