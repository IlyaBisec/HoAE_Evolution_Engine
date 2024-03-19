/*****************************************************************************/
/*	File:	gpBucket.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	19.03.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kHash.hpp"
#include "kResource.h"
#include "gpPackage.h" 
#include "gpBucket.h"

#ifndef _INLINES
#include "gpBucket.inl"
#endif // !_INLINES

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	GPBucket implementation
/*****************************************************************************/
int		GPBucket::s_CurShader = 0;
float	GPBucket::s_TexCoordBias = 0.0f;


void BendVS::setTime( DWORD _time )
{
	time = _time;
	ratio = sin( float( time )/ 1000.0f + phase );
}

void BendVS::ProcessVertices( Vertex2t* vert, int nV ) const
{
	float coeff = ratio * 0.0001f / height;
	for (int i = 0; i < nV; i++)
	{
		if (vert[i].y < cY)
		{
			float yres = cY - vert[i].y;
			vert[i].x += coeff * yres * yres * yres;
		}
		(*(Vector3D*)(&(vert[i]))) *= tr;
	}
} // BendVS::ProcessVertices

END_NAMESPACE(sg)

