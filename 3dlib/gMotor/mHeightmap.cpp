/*****************************************************************************/
/*    File:    mHeightmap.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "mNoise.h"
#include "mHeightmap.h"
#include "kIOHelpers.h"

#ifndef _INLINES
#include "mHeightmap.inl"
#endif // !_INLINES

/*********************************************************************************************/
/*  HeightMap implementation
/*********************************************************************************************/
HeightMap* PutPixel::pMap = NULL;
const AABoundBox& HeightMap::CalculateAABB()
{
    float* pH = GetArrayPtr();
    for (int i = 0; i < GetNSideNodes(); i++)
    {
        for (int j = 0; j < GetNSideNodes(); j++)
        {
            float h = *pH;
            if (m_AABB.minv.z > h) m_AABB.minv.z = h;
            if (m_AABB.maxv.z < h) m_AABB.maxv.z = h;
            pH++;
        }    
    }
    return m_AABB;
} // HeightMap::CalculateAABB

void HeightMap::DisturbPerlin( float dx, float dy, float sx, float sy, float dh, float hs )
{
    float* pH = GetArrayPtr();    
    for (int i = 0; i < GetNSideNodes(); i++)
    {
        float wy = float( i ) * m_GridStep;
        for (int j = 0; j < GetNSideNodes(); j++)
        {
            float wx = float( j ) * m_GridStep;
            float h = PerlinNoise( (dx + wx)*sx, (dy + wy)*sy ) * hs + dh;    
            *pH += h;
            pH++;
        }    
    }

} // HeightMap::GeneratePerlin

void HeightMap::SetHeight( float val )
{
    SetValue( val );
}

void HeightMap::Scale( float val )
{
    int hMapSz = GetSize();
    for (int i = 0; i < hMapSz; i++) SetValue( i, GetValue( i ) * val );
}

XStatus    HeightMap::Intersect( const Sphere& sphere,    const Rct* pArea )
{
    assert( false );
    return xsUnknown;
}

XStatus    HeightMap::Intersect( const AABoundBox& aabb, const Rct* pArea )
{
    assert( false );
    return xsUnknown;
}

XStatus    HeightMap::Intersect( const Segment3D& seg,    const Rct* pArea )
{
    assert( false );
    return xsUnknown;
}

bool HeightMap::Pick( const Ray3D& ray, Vector3D& pt, const Rct* pArea )
{
    Rct rct = pArea ? *pArea : Rct( m_AABB.minv.x, m_AABB.minv.y, m_AABB.GetDX(), m_AABB.GetDY() );
    Vector3D dst = ray.getDir();
    dst *= m_AABB.Distance2( ray.getOrig() );
    dst += ray.getOrig();

    Vector2D a = Vector2D( ray.getOrig().x, ray.getOrig().y );
    Vector2D b = Vector2D( dst.x, dst.y );
    rct.ClipSegment( a, b );
    
    Vector3D va( a.x, a.y, ray.GetZ( a.x, a.y ) );
    Vector3D vb( b.x, b.y, ray.GetZ( b.x, b.y ) );
    float H = GetHeight( a.x, a.y );

    int nSteps = va.distance( vb ) / m_GridStep;
    Vector3D d; d.sub( vb, va ); d /= nSteps;
    int prevSig = H < va.z ? -1 : 1;
    for (int i = 0; i < nSteps; i++)
    {
        va += d;
        H = GetHeight( va.x, va.y );
        int sig = H < va.z ? -1 : 1;
        if (prevSig != sig)
        {
            pt = va;
            return true;
        }
        prevSig = sig;
    }
    return false;
} // HeightMap::Pick

InStream&    operator >> ( InStream& is,    HeightMap& hmap )
{
    hmap.Unserialize( is );
    is >> hmap.m_AABB >> hmap.m_GridStep;
    return is;
}

OutStream&    operator << ( OutStream& os, const  HeightMap& hmap )
{
    hmap.Serialize( os );
    os << hmap.m_AABB << hmap.m_GridStep;
    return os;
}

