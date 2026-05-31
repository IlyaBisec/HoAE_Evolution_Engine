/*****************************************************************
/*  File:   mRandom.inl                                         
/*	Desc:	Random values utility functions						  
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#include "mRandom.h"
_inl float rndValuef()
{
	float rndval = static_cast<float>( rndValue() );
	rndval /= (float)RAND_MAX;
	return rndval;
}

_inl float rndValuef( float min, float max )
{
	return min + static_cast<float>( max - min ) * rndValuef(); 
}

_inl int rndValue( int min, int max )
{
	if (min == max) return min;
	return min + rndValue()%(max - min);
}

