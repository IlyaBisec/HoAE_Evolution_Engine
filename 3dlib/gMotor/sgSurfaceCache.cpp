/*****************************************************************************/
/*    File:    sgSurfaceCache.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.12.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTexture.h"
#include "sgSurfaceCache.h"

#ifndef _INLINES
#include "sgSurfaceCache.inl"
#endif // _INLINES

IMPLEMENT_CLASS( SurfaceCache );
IMPLEMENT_CLASS( SurfaceCacheItem );

/*****************************************************************************/
/*    SurfaceCacheItem implementation
/*****************************************************************************/
SurfaceCacheItem::SurfaceCacheItem()
{
}

void SurfaceCacheItem::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "SurfaceCacheItem", this );
}

/*****************************************************************************/
/*    SurfaceCache implementation
/*****************************************************************************/
SurfaceCache::SurfaceCache()
{
    m_NSurfaces        = 0;    
    m_SurfaceSide    = (PowerOfTwo)256;    
    m_ColorFormat    = cfARGB4444;    
    m_MemoryPool    = mpVRAM;    
    m_bRenderTarget    = false;
}

SurfaceCache::~SurfaceCache    ()
{

}

void SurfaceCache::Render()
{

}

void SurfaceCache::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "SurfaceCache", this );
    pm.p( "NumSurfaces", &SurfaceCache::GetNSurfaces, &SurfaceCache::SetNSurfaces        );
    pm.p( "SurfaceSide", &SurfaceCache::GetSurfaceSide, &SurfaceCache::SetSurfaceSide        );
    pm.p( "ColorFormat", &SurfaceCache::GetColorFormat, &SurfaceCache::SetColorFormat        );
    pm.p( "RenderTarget", &SurfaceCache::IsRT, &SurfaceCache::SetIsRT                );
    pm.p( "MemoryPool", &SurfaceCache::GetMemoryPool, &SurfaceCache::SetMemoryPool        );
}

void SurfaceCache::CreateContents()
{
    ReleaseChildren();

}

void SurfaceCache::SetNSurfaces( int val )
{
    m_NSurfaces = val;
    CreateContents();
}

void SurfaceCache::SetSurfaceSide( PowerOfTwo val )
{
    m_SurfaceSide = val;
    CreateContents();
}

void SurfaceCache::SetColorFormat( ColorFormat val )
{
    m_ColorFormat = val;
    CreateContents();
}

void SurfaceCache::SetIsRT( bool val )
{
    m_bRenderTarget = val;
    CreateContents();
}

void SurfaceCache::SetMemoryPool( MemoryPool val )
{
    m_MemoryPool = val;
    CreateContents();
}



