/*****************************************************************************/
/*	File:	mHeightmap.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-18-2003
/*****************************************************************************/
#include "mTriangle.h"
/*********************************************************************************************/
/*	HeightMap implementation
/*********************************************************************************************/
_inl void HeightMap::GetNormal( float x, float y, Vector3D& normal ) const
{
	float ndenom = 0.5f / m_GridStep;

	float l = GetHeight( x - m_GridStep, y );		
	float r = GetHeight( x + m_GridStep, y );		
	float u = GetHeight( x, y - m_GridStep );		
	float d = GetHeight( x, y + m_GridStep );		

	normal.x = (l - r) * ndenom;
	normal.y = (u - d) * ndenom;
	normal.z = 1.0f;
	normal.normalize();
} // HeightMap::GetNormal

_inl bool HeightMap::LocateNode( float x, float y, int& nx, int& ny ) const
{
	if (!m_AABB.XIn( x )) return false;
	if (!m_AABB.YIn( y )) return false;

	float bX = (x - m_AABB.minv.x)/m_GridStep;
	float bY = (y - m_AABB.minv.y)/m_GridStep;

	const float minX = float( floor( bX ) );
	const float minY = float( floor( bY ) );
	nx = int( minX );
	ny = int( minY );

	if ((bX - minX) * 2.0f > m_GridStep) nx++;
	if ((bY - minY) * 2.0f > m_GridStep) ny++;

	return true;
} // HeightMap::LocateNode

_inl Vector3D HeightMap::GetNormal( int x, int y ) const
{
	if (x < 1) x = 1;
	if (y < 1) y = 1;
	if (x >= GetNSideNodes() - 1) x = GetNSideNodes() - 2;
	if (y >= GetNSideNodes() - 1) y = GetNSideNodes() - 2;

	float ndenom = 0.5f / m_GridStep;

	float l = GetHeight( x - 1, y );		
	float r = GetHeight( x + 1, y );		
	float u = GetHeight( x, y - 1 );		
	float d = GetHeight( x, y + 1 );		
	
	Vector3D normal;
	normal.x = (l - r) * ndenom;
	normal.y = (u - d) * ndenom;
	normal.z = 1.0f;
	normal.normalize();
	return normal;
} // HeightMap::GetNormal

_inl Vector3D HeightMap::GetNormal( float x, float y ) const
{
	float ndenom = 0.5f / m_GridStep;

	float l = GetHeight( x - m_GridStep, y );		
	float r = GetHeight( x + m_GridStep, y );		
	float u = GetHeight( x, y - m_GridStep );		
	float d = GetHeight( x, y + m_GridStep );		

	Vector3D normal( (l - r) * ndenom, (u - d) * ndenom, 1.0f );
	normal.normalize();
	return normal;
} // HeightMap::GetNormal

_inl Vector3D HeightMap::GetNormal( float x, float y, float radius ) const
{
    float s = radius*2.0f;
    Vector3D n;
    n.zero();
    for (float cx = 0.0f; cx <= s; cx += m_GridStep)
    {
        for (float cy = 0.0f; cy <= s; cy += m_GridStep)
        {   
            n += GetNormal( cx + x, cy + y );
        }
    }
    n.normalize();
    return n;
} // HeightMap::GetNormal

_inl float HeightMap::GetHeight( float x, float y ) const
{
	// find square
	float bX = (x - m_AABB.minv.x)/m_GridStep;
	float bY = (y - m_AABB.minv.y)/m_GridStep;

	const float minX = float( floor( bX ) );
	const float minY = float( floor( bY ) );
	int intX = int( minX );
	int intY = int( minY );

	const float wx = bX - minX;
	const float wy = bY - minY;

	if (fabs( wx ) < c_SmallEpsilon && fabs( wy ) < c_SmallEpsilon) 
	{
		return GetValue( intX, intY );
	}
	const float p11 = GetValue( intX,		intY	 );
	const float p12 = GetValue( intX,		intY + 1 );
	const float p21 = GetValue( intX + 1,	intY	 );
	const float p22 = GetValue( intX + 1,	intY + 1 );

	const float px1 = p11 + wx * ( p21 - p11 );
	const float px2 = p12 + wx * ( p22 - p12 );

	const float p = px1 + wy * ( px2 - px1 );
	return p;
} // HeightMap::GetHeight

_inl void HeightMap::SetHeight( float x, float y, float val )
{
	// find square
	float bX = (x - m_AABB.minv.x)/m_GridStep;
	float bY = (y - m_AABB.minv.y)/m_GridStep;

	const float minX = float( floor( bX ) );
	const float minY = float( floor( bY ) );
	int intX = int( minX );
	int intY = int( minY );
	SetValue( intX, intY, val );
} // HeightMap::GetHeight

_inl void HeightMap::SetHeight( int x, int y, float val )
{
	if (!SetValue( x, y, val )) return;
	if (val > m_AABB.maxv.z) m_AABB.maxv.z = val;
	if (val < m_AABB.minv.z) m_AABB.minv.z = val;
} // HeightMap::SetHeight

_inl float HeightMap::GetHeight( int x, int y ) const
{
	return GetValue( x, y );
} // HeightMap::SetHeight

struct PutPixel 
{
	static HeightMap* pMap;
	static bool Put( int x, int y )
	{
		pMap->SetHeight( x, y, 400.0f );
		return true;
	}
}; // struct PutPixel

_inl void HeightMap::ToWorldSpace( float& x, float& y ) const
{
	x = x * m_GridStep + m_AABB.minv.x;
	y = y * m_GridStep + m_AABB.minv.y;
} // HeightMap::ToWorldSpace
