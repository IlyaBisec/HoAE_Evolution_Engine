/*****************************************************************************/
/*    File:    sgG2D.h
/*    Desc:    .G2D format loader
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __SGG2D_H__
#define __SGG2D_H__



#pragma pack(push)
#pragma pack(1)
/*****************************************************************
/*    Struct:    GU2DHeader
/*    Desc:    GU2D file header, as it is in the file
/*****************************************************************/
struct GU2DHeader
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
};  // struct GU2DHeader

const int c_GU2DHeaderSize = sizeof( GU2DHeader );

struct GU2DFrameInfo 
{
    DWORD                    m_MeshesOffset;        
    DWORD                    m_MeshesNumber;        
}; // struct GU2DFrameInfo 
const int c_GU2DFrameInfoSize = sizeof( GU2DFrameInfo );

struct GU2DTextureInfo 
{
    WORD                    m_Width;                
    WORD                    m_Height;                
    WORD                    m_Flags;                
    WORD                    m_Reserved;
    DWORD                    m_TextureOffset;    
    DWORD                    m_TextureSize;
    DWORD                    m_VerticesOffset;        
    DWORD                    m_VerticesNumber;    
    DWORD                    m_IndicesOffset;        
    DWORD                    m_IndicesNumber;        
}; // struct GU2DTextureInfo
const int GU2DTextureInfoSize = sizeof( GU2DTextureInfo );

struct GU2DVertex 
{
    short                    x;                    
    short                    y;
    short                    z;
    short                    u;                    
    short                    v;
}; // struct GU2DVertex

struct GU2DVertexC 
{
    short                    x;                    
    short                    y;
    short                    z;
    short                    u;                    
    short                    v;
    DWORD                   color;
}; // struct GU2DVertexC


const int c_GU2DVertexSize = sizeof(GU2DVertex);
const int c_GU2DVertexCSize = sizeof(GU2DVertexC);

struct GU2DMesh 
{
    WORD                    m_TextureIdx;    
    DWORD                    m_VerticesOffset;    
    DWORD                    m_VerticesNumber;    
    DWORD                    m_TrianglesOffset;    
    DWORD                    m_TrianglesNumber;    
}; // struct GU2DMesh

const int c_GU2DMeshSize = sizeof( GU2DMesh );
#pragma pack(pop)

/*****************************************************************************/
/*    Class:    GU2DTexture
/*    Desc:    GU2D texture caching status description
/*****************************************************************************/
class GU2DTexture
{
public:

}; // class GU2DTexture

const int c_MaxG2DTextures = 512;

/*****************************************************************************/
/*    Struct:    CacheStamp    
/*****************************************************************************/
struct CacheStamp
{
    DWORD        m_DropStamp;
    DWORD        m_SurfID;

    DWORD        m_GPID;
    DWORD        m_TexID;
    DWORD        m_Color;
    DWORD        m_LOD;

    float        ax;
    float        ay;

    unsigned int    hash    () const
    {
        DWORD h = m_GPID + 541*m_Color + 17*m_LOD + 103*m_TexID;
        h = (h << 13) ^ h;
        h += 15731;
        return h;
    }

    bool equal( const CacheStamp& el )
    {
        return    (m_GPID  == el.m_GPID) && 
                (m_TexID == el.m_TexID) && 
                (m_Color == el.m_Color) &&
                (m_LOD     == el.m_LOD);
    }

    void copy( const CacheStamp& el )
    {
        m_DropStamp    = el.m_DropStamp;
        m_SurfID    = el.m_SurfID;
        m_GPID        = el.m_GPID;
        m_TexID        = el.m_TexID;
        m_Color        = el.m_Color;
        m_LOD        = el.m_LOD;
        ax            = el.ax;
        ay            = el.ay;
    }

    CacheStamp() :  m_GPID(0xFFFFFFFF), m_Color(0xFFFFFFFF), 
                    m_LOD(0xFFFFFFFF), m_TexID(0xFFFFFFFF),
                    m_SurfID(0xFFFFFFFF), m_DropStamp(0xFFFFFFFF){}

    CacheStamp( int gpID, int texID, DWORD color, int lod ) : 
                    m_GPID(gpID), m_Color(color), m_LOD(lod), m_TexID(texID),
                    m_SurfID(0xFFFFFFFF), m_DropStamp(0xFFFFFFFF) {}
}; // struct CacheStamp

/*****************************************************************************/
/*    Class:    GU2DPackage
/*****************************************************************************/
class GU2DPackage : public SpritePackage
{
public:
                            GU2DPackage     ();
    virtual void            Init            ( const BYTE* data );
    virtual int             GetFrameWidth   ( int sprID );
    virtual int             GetFrameHeight  ( int sprID );
    virtual DWORD           GetFileSize     () const { return m_PixelDataSize; }
    virtual FrameInstance*  PrecacheFrame   ( int sprID, DWORD color = 0, int lod = 0 );
    virtual DWORD           GetAlpha        ( FrameInstance* frameInst, int ptX, int ptY, bool bPrecise = false );
    
    static void             CleanCache      () { s_StampReg.reset(); }
    virtual void            Cleanup         ();
    static void             FreeSurface     ( int surfID );
    virtual Rct             GetFrameBounds  ( int sprID );
    virtual DWORD           GetMagic        () const { return 'D2UG'; }
    

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

    static Hash<CacheStamp> s_StampReg;
    GU2DFrameInfo*            GetFrameInfo    ( int sprID );
    int                        PrecacheTexture ( int texID, DWORD color, int lod, float& begU, float& begV );
    CacheStamp&                GetCacheStamp    ( int texID, DWORD color, int lod );

}; // class GU2DPackage

/*****************************************************************************/
/*    Class:    GU2DCreator
/*    Desc:    Loader of the G2D sprite package
/*****************************************************************************/
class GU2DCreator : public IPackageCreator
{
public:
                            GU2DCreator        ();
    virtual SpritePackage*    CreatePackage    ( char* fileName, const BYTE* data );
    virtual const char*        Description        () const;

}; // class GU2DCreator



#endif // __SGG2D_H__