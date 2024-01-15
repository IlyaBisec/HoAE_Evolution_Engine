/*****************************************************************************/
/*    File:    sgG17.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "kFilePath.h"
#include "sgSpriteManager.h"
#include "sgG17.h"
#include "IResourceManager.h"

#include "FCompressor.h"
FCompressor g_Compressor;

/*****************************************************************************/
/*    G17Creator implementation
/*****************************************************************************/
G17Creator::G17Creator()
{
    SpritePackage::RegisterCreator( this );
}

const char* c_CacheDir = "Cash";
SpritePackage* G17Creator::CreatePackage( char* fileName, const BYTE* data )
{
    FilePath path( fileName );
    if (path.HasExt( "g17" ))
    {
        //  construct cached .g16 path
        FilePath cpath( IRM->GetHomeDirectory() );
        cpath.SetExt( "g16" );
        cpath.AppendDir( c_CacheDir );
        if (cpath.Exists())
        {
            
            return NULL;
        }

        BYTE* pUnpacked = NULL;
        unsigned int unpSize   = 0;
        if (!g_Compressor.DecompressBlock( &pUnpacked, &unpSize, (unsigned char*)data )) return NULL;
    }
    return NULL;
} // G17Creator::Load

const char*    G17Creator::Description() const
{
    return "G17 Sprite Loader";
} // G17Creator::Description
    
