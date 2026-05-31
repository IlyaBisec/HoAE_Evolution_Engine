/*****************************************************************************/
/*    File:    sgGP2.h
/*    Desc:    .GP2 format loader
/*    Author:  Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __SGGP2_H__
#define __SGGP2_H__

#include "kStaticArray.hpp"
#include "kHash.hpp"

#pragma pack(push)
#pragma pack(1)
/*****************************************************************
/*    Struct:    GP2Header
/*    Desc:    GP2 file header, as it is in the file
/*****************************************************************/
struct GP2Header
{
    DWORD                    m_Magic;                
    DWORD                    m_FileSize;            
    DWORD                    m_FramesNumber;        
    DWORD                    m_VerticesOffset;        
    DWORD                    m_VerticesNumber;        
    DWORD                    m_IndicesOffset;        
    DWORD                    m_IndicesNumber;        
    DWORD                    m_MeshesOffset;        
    DWORD                    m_MeshesNumber;        
    DWORD                    m_TexturesInfoOffset;    
    DWORD                    m_TexturesOffset;        
    DWORD                    m_TexturesNumber;        
    DWORD                    m_InfoLen;    
    
    WORD                    m_Width;        
    WORD                    m_Height;        
    BYTE                    m_Directions;
    BYTE                    m_VertexFormat;
};  // struct GP2Header

const int c_GP2HeaderSize = sizeof( GP2Header );

struct GP2FrameInfo 
{
    WORD		m_MeshesOffset;	
	WORD		m_MeshesNumber;		
	short       m_FrameX0;
	short       m_FrameY0;
	WORD        m_FrameWidth;
	WORD        m_FrameHeight;       
}; // struct GP2FrameInfo 
const int c_GP2FrameInfoSize = sizeof( GP2FrameInfo );

struct GP2TextureInfo 
{
    WORD		m_Width;			     
    WORD		m_Height;			      
    WORD		m_Flags;			     
    WORD		m_Reserved;
    DWORD		m_TextureOffset;	  
    DWORD		m_TextureSize;	
    DWORD		m_VerticesOffset;	       
    DWORD		m_VerticesNumber;   
    DWORD		m_IndicesOffset;	      
    DWORD		m_IndicesNumber;	      
}; // struct GP2TextureInfo
const int GP2TextureInfoSize = sizeof( GP2TextureInfo );

struct GP2Vertex 
{
    WORD		x;	                    
    WORD		y;
    WORD		u;	
    WORD		v;                    
}; // struct GP2Vertex

const int c_GP2VertexSize = sizeof(GP2Vertex);

struct GP2Mesh 
{
	WORD		m_TextureIdx;			
	DWORD		m_VerticesOffset;	    
	DWORD		m_VerticesNumber;	
	DWORD		m_TrianglesOffset;	
	DWORD		m_TrianglesNumber;	
}; // struct GP2Mesh

const int c_GP2MeshSize = sizeof( GP2Mesh );
#pragma pack(pop)

/*****************************************************************************/
/*  Struct: GP2TextureKey
/*  Desc:   Key, used for mapping between package/texture pair to the texture
/*           cache surface
/*****************************************************************************/
struct GP2TextureKey
{
    int             m_GPID;
    int             m_TexID;
    int             m_SurfID;
    
    GP2TextureKey() : m_GPID( -1 ), m_TexID( -1 ), m_SurfID( -1 ) {}
    
    GP2TextureKey( int gpID, int texID ) : m_GPID( gpID ), m_TexID( texID ), m_SurfID( -1 ) {}
    
    unsigned int hash() const
    {
        return 103*m_GPID + m_TexID;
    }
    bool equal( const GP2TextureKey& el ) { return (m_GPID == el.m_GPID) && (m_TexID == el.m_TexID); }
    void copy( const GP2TextureKey& el ) { *this = el; }
}; // struct GP2TextureKey

/*****************************************************************************/
/*  Struct: GP2Surface
/*  Desc:   Describes single surface in the gp2 surface cache
/*****************************************************************************/
struct GP2Surface
{
    int     m_Index;
    int     m_TexID;
    
    int     m_GPID;
    int     m_GPTexID;
}; // struct GP2Surface

//  maximal GP2 frames drawn on frame
const int c_MaxGP2FrameInst = 16384;
const int c_NumGP2Surfaces  = 512;
const int c_GP2TexSide      = 256;

typedef static_array<FrameInstance*, c_MaxGP2FrameInst> FrameInstArray;
typedef Hash<GP2TextureKey>                             GP2TextureHash;
typedef static_array<GP2Surface, c_NumGP2Surfaces>      GP2SurfaceArray;
typedef TypedPoolAllocator<FrameInstance,1024*1024*4>   FrameInstAllocator;

/*****************************************************************************/
/*    Class:    GP2Package
/*****************************************************************************/
class GP2Package : public SpritePackage
{
public:
                            GP2Package      ();
    virtual void            Init            ( const BYTE* data );
    virtual int             GetFrameWidth   ( int sprID );
    virtual int             GetFrameHeight  ( int sprID );
    virtual Rct             GetFrameBounds  ( int sprID );
    virtual DWORD           GetMagic        () const { return 'D2PG'; }
    
    virtual DWORD           GetFileSize     () const { return m_PixelDataSize; }
    virtual FrameInstance*  PrecacheFrame   ( int sprID, DWORD color = 0, int lod = 0 );
    virtual DWORD           GetAlpha        ( FrameInstance* frameInst, int ptX, int ptY, bool bPrecise = false );
    
    virtual void            Cleanup         (){}
    static void             FreeSurface     ( int surfID );
    static void             OnFrame         ();
    static void             InitSurfaceCache();
    static void             CleanCache      ();

private:    
    int                     m_PixelDataSize;

    DWORD                   m_VerticesOffset;        
    DWORD                   m_VerticesNumber;        
    DWORD                   m_IndicesOffset;        
    DWORD                   m_IndicesNumber;        
    DWORD                   m_MeshesOffset;        
    DWORD                   m_MeshesNumber;        
    DWORD                   m_TexturesInfoOffset;    
    DWORD                   m_TexturesOffset;    
    DWORD                   m_TexturesNumber;
    DWORD                   m_InfoLen;        
    WORD                    m_Width;        
    WORD                    m_Height;    

    int                     m_HeaderSize;
    BYTE                    m_VertexFormat;
    int                     m_VertexSize;

    GP2FrameInfo*           GetFrameInfo    ( int sprID );
    int                     PrecacheTexture ( int texID, DWORD color, int lod, float& begU, float& begV );
    
    static FrameInstArray       s_FrameInst;
    static GP2SurfaceArray      s_Surfaces;
    static int                  s_ScratchSurf;
    static int                  s_LastSurface;
    static GP2TextureHash       s_SurfMapping;
    static FrameInstAllocator   s_FrameAllocator;
}; // class GP2Package

/*****************************************************************************/
/*    Class:    GP2Creator
/*    Desc:    Loader of the GP2 sprite package
/*****************************************************************************/
class GP2Creator : public IPackageCreator
{
public:
                               GP2Creator       ();
    virtual SpritePackage*     CreatePackage    ( char* fileName, const BYTE* data );
    virtual const char*        Description      () const;

}; // class GP2Creator



#endif // __SGGP2_H__