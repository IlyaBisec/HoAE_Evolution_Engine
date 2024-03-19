/*****************************************************************/
/*  File:   sgSpriteManager.h
/*  Desc:   Sprite manager
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __SGSPRITEMANAGER_H__
#define __SGSPRITEMANAGER_H__

#include "gmDefines.h"
#include "kResFile.h"
#include "kCache.h"
#include "kHash.hpp"
#include "kResource.h"
#include "kTimer.h"
#include "kPoolAllocator.h"
#include "IRenderSystem.h"
#include "ISpriteManager.h"
#include "IResourceManager.h"
#include "kFileMapping.h"
#include "kQuadLayout.h"
#include "kStaticArray.hpp"

#include "sg.h"



const int       c_FrameInstanceHashTableSize    = 7919;
const int       c_FrameInstanceHashPoolSize     = 5912;
const int       c_GPHashTableSize               = 7919;
const int       c_MaxSpriteSurfaces             = 256;//256;
const int       c_MaxGPSeqs                     = 2048;
const int       c_GPTexSide                     = 256;//256;    //  side of texture in gp cache
const int       c_GPTexSidePow                  = 9;      //  2 power of c_GPTexSide
const int       c_MaxGPSurfaceClients           = 4096;
const float     c_SpriteManagerVMEMQuote        = 0.5;        
const int       c_PixelCapacityBias             = 32*32;
const int       c_MaxFramesInPackedSegment      = 256;
const int       c_MaxFrameChunkBlocks           = 16;
const int       c_MaxGPPalettes                 = 2;
const int       c_MaxFrameInstancesPerSurface   = 512;
const int       c_MaxVertDrawn                  = 16384;
const int       c_MaxIndDrawn                   = c_MaxVertDrawn*6;
const int       c_MaxSpritesDrawn               = 5912;
const float     c_GPTexel                       = 1.0f / c_GPTexSide;
const float     c_HalfGPTexel                   = -c_GPTexel*0.375f;
const float     c_HalfPixel                     = 0.5f;
const float     c_PurgeTimeout                  = 30.0f;

class Group;
class SpritePackage;
class GPFont;
class FrameChunk;
class FrameInstance;
class SpriteManager;

/*****************************************************************************/
/*    Class:    FrameVert
/*    Desc:    Single vertex of the frame chunk mesh
/*****************************************************************************/
struct FrameVert
{
    float            x, y, z;
    float            u, v;
    DWORD           color;
}; // struct FrameVert 

/*****************************************************************************/
/*    Class:    FrameChunk
/*    Desc:    Single chunk of the sprite image
/*****************************************************************************/
class FrameChunk
{
public:
    WORD                 m_NVert;        //  number of vertices in the chunk
    WORD                 m_NTri;         //  number of triangles in the chunk

    FrameVert*           m_Vert;         //  array of vertices
    WORD*                m_Idx;          //    array of indices. When NULL quad indices are used
    WORD                 m_SurfaceID;    //    index of the texture surface
    WORD                 m_TextureID;    //    index of the texture surface


    FrameChunk() : m_SurfaceID(0xFFFF), m_TextureID(0xFFFF) {}
}; // class FrameChunk

/*****************************************************************************/
/*    Class:    SpriteInstance
/*    Desc:    Sprite instance, which is drawn in the current frame
/*****************************************************************************/
class SpriteInstance
{
    FrameInstance*        m_pFrame;        //  frame instance
    Matrix4D            m_TM;            //    sprite transform
    DWORD                m_Flags;        //    flags
    DWORD                m_Color;
}; // class SpriteInstance

/*****************************************************************************/
/*    Class:    FrameInstance
/*****************************************************************************/
class FrameInstance 
{
public:
    //  frame instance attributes are packed into DWORD key:
    struct Key
    {
        WORD        m_GPID;
        WORD        m_SprID;
        DWORD        m_Color;
        BYTE        m_LOD;

        Key( WORD gpID, WORD sprID, DWORD color, BYTE lod ) 
            :    m_GPID    ( gpID  ),
                m_SprID    ( sprID    ),
                m_Color    ( color    ),
                m_LOD    ( lod    ){}
        Key(){}

        bool operator ==( const Key& key ) 
        { 
            return    (m_SprID == key.m_SprID ) &&
                    (m_Color == key.m_Color ) &&
                    (m_GPID  == key.m_GPID  ) && 
                    (m_LOD     == key.m_LOD    ); 
        }

        WORD    GetSeqID    () const { return m_GPID;  }
        WORD    GetFrameID    () const { return m_SprID; }
        BYTE    GetLOD        () const { return m_LOD;   }
        DWORD    GetColor    () const { return m_Color; }

        void    SetSeqID    ( WORD  id ){ m_GPID  = id; }
        void    SetFrameID    ( WORD  id ){ m_SprID = id; }
        void    SetLOD        ( BYTE  id ){ m_LOD   = id; }
        void    SetColor    ( DWORD id ){ m_Color = id; }

        unsigned int hash() const
        {
            DWORD h = m_GPID + 541*m_SprID + m_Color;
            h = (h << 13) ^ h;
            h += 15731;
            return h;
        } // hash
    };
    void copy( const FrameInstance& fr )  { *this = fr; }
    bool equal( const FrameInstance& fr ) { return (m_Key == fr.m_Key); }

    _inl int                GetNChunks      () const { return m_NChunks; }
    _inl void               SetNChunks      ( int nChunks, void* buf ) { m_NChunks = nChunks; m_Chunk = (FrameChunk*)buf; }
    _inl bool               IsDrawnAtOnce   () const; 
    _inl BYTE*              GetPixelData    ()    const        { return NULL; }

    int                     GetSeqID        () const        { return m_Key.GetSeqID();             }
    int                     GetFrameID      () const        { return m_Key.GetFrameID();         }
    DWORD                   GetColor        () const        { return m_Key.GetColor();             }
    int                     GetLOD          () const        { return m_Key.GetLOD();             }

    void                    SetSeqID        ( WORD id )     { m_Key.SetSeqID    ( id );            }
    void                    SetFrameID      ( WORD id )     { m_Key.SetFrameID  ( id );            }
    void                    SetColor        ( DWORD id )    { m_Key.SetColor    ( id );            }
    void                    SetLOD          ( BYTE id )     { m_Key.SetLOD      ( id );            }

    void                    SetSegIdx       ( WORD segIdx ) { m_PackSegIdx = segIdx;            }
    WORD                    GetSegIdx       () const        { return m_PackSegIdx;                }                    
    bool                    IsCached        () const        { return m_PackSegIdx != 0xFFFF;    }

    void                    SetID           ( WORD id )     { m_ID = id;                        }
    DWORD                   GetID           () const        { return m_ID;                        }

    FrameChunk&             GetChunk        ( int idx )     { return m_Chunk[idx]; }
    const FrameChunk&       GetChunk        ( int idx ) const    { return m_Chunk[idx]; }

    void                    Drop            () { m_PackSegIdx = 0xFFFF; }
    const Key&              GetKey          () const { return m_Key; }

    float                   GetHeight       () const { return m_Height; }
    float                   GetWidth        () const { return m_Width; }
    void                    SetHeight       ( float h ) { m_Height = h; }
    void                    SetWidth        ( float w ) { m_Width = w; }

    const Rct&              GetBounds       () const { return m_BoundFrame; }
    AABoundBox              GetAABB         () const { return AABoundBox( m_BoundFrame, m_MinZ, m_MaxZ ); }
    void                    SetBounds       ( float x, float y, float w, float h ) { m_BoundFrame.Set( x, y, w, h ); }
    void                    SetZBounds      ( float minZ, float maxZ ) { m_MinZ = minZ; m_MaxZ = maxZ; }

    void                    Lock            () { m_bLocked = true; }
    void                    Unlock          () { m_bLocked = false; }
    bool                    IsLocked        () const { return m_bLocked; }

    FrameInstance() : m_PackSegIdx(0xFFFF), m_ID(0xFFFFFFFF), m_bLocked(false){}

public:
    FrameChunk*                m_Chunk;
    int                        m_NChunks;

    WORD                    m_Width, m_Height;
    Rct                        m_BoundFrame;
    float                   m_MinZ, m_MaxZ;

    Key                        m_Key;    
    DWORD                    m_ID;
    WORD                    m_PackSegIdx;
    bool                    m_bLocked;

    friend class            SpriteManager;
}; // class FrameInstance

class SpritePackage;
/*****************************************************************************/
/*    Class:    IPackageCreator
/*    Desc:    Loader of the sprite package interface
/*****************************************************************************/
class IPackageCreator
{
public:
    virtual SpritePackage*        CreatePackage( char* fileName, const BYTE* data ) = 0;
    virtual const char*            Description() const = 0;
}; // class IPackageCreator

/*****************************************************************
/*    Class:    SpritePackage
/*    Desc:    Sprite sequence 
/*****************************************************************/
class SpritePackage : public IResource
{
public:
                                SpritePackage       ();
                                ~SpritePackage      ();

    int                         GetNFrames          () const { return m_NFrames; }
    int                         GetNDirections      () const { return m_NDirections; }
    void                        SetNDirections      ( int ndir ) { m_NDirections = ndir; }

    DWORD                       GetID               () const { return m_ID; }
    void                        SetID               ( DWORD id ) { m_ID = id; }

    void                        SetName             ( const char* name ) { m_Name = name; }
    void                        SetPath             ( const char* path ) { m_Path = path; }
    const char*                 GetName             () const { return m_Name.c_str(); }
    const char*                 GetPath             () const { return m_Path.c_str(); }

    _inl bool                   IsValid             () const;
    int                         GetWorkBufSize      () const { return m_WorkBufSize; }

    virtual void                Cleanup             ();
    bool                        HasName             ( const char* name ) const  { return m_Name == name; }
    const char*                 GetFileName         () const                    { return m_FileMapping.GetFileName();}
    const BYTE*                 GetFileData         ()                          { return m_FileMapping.GetPointer(); }
    
    //  implemented by packages of concrete type
    virtual void                Init                ( const BYTE* data ){}
    virtual int                 GetFrameNSquares    ( int sprID ) { return 0; }
    virtual int                 GetFrameWidth       ( int sprID ) { return 0; }
    virtual int                 GetFrameHeight      ( int sprID ) { return 0; }
    virtual Rct                 GetFrameBounds      ( int sprID )
        { return Rct( 0.0f, 0.0f,   GetFrameWidth       ( sprID ), GetFrameHeight( sprID ) ); }
    virtual DWORD               GetFileSize         () const { return 0; }
    virtual DWORD               GetMagic            () const { return 0xFFFFFFFF; }

    virtual DWORD               GetAlpha            ( FrameInstance* frameInst, int ptX, int ptY, bool bPrecise = false ){ return c_OutsideChunks; }
    
    virtual FrameInstance*      PrecacheFrame       ( int sprID, DWORD color = 0, int lod = 0 ) { return false; }
    static void                 RegisterCreator     ( IPackageCreator* iCreator ) { s_Creators.push_back( iCreator ); }
    
    void                        Precache            ();

    const AABoundBox&           GetAABB             () const { return m_AABB; }
    bool                        HasDepthData        () const { return m_bHasDepthData; }
    bool                        HasColorData        () const { return m_bHasColorData; }

    virtual bool                Reload              () { return false; }

protected:
    const BYTE*                 GetPalette          ( int palIdx ) { return GetFileData() + m_PaletteOffset[palIdx];}
    void                        SetPaletteOffset    ( int palID, int offs ) { m_PaletteOffset[palID] = offs; }

    static SpritePackage*       CreatePackage       ( const char* fname );
    void                        TouchData           () { m_FileMapping.TouchData(); }


    std::string                 m_Path;              //  package path, from the root
    std::string                 m_Name;              //  string ID 

    int                         m_PaletteOffset[c_MaxGPPalettes];
    FileMapping                 m_FileMapping;       //  file mapping object
    int                         m_NFrames;           //  number of the sprites in package
    int                         m_NSegments;         //  number of the pack segments in package
    DWORD                       m_ID;                //  package ID
    int                         m_WorkBufSize;       //  unpacker maximal working buffer size 
    WORD                        m_NDirections;       //  number of rendered sprite directions
    bool                        m_bHasDepthData;
    bool                        m_bHasColorData;
    AABoundBox                  m_AABB;

    static std::vector<IPackageCreator*>    s_Creators;
    static BYTE*                            s_WorkBuffer;
    static DWORD                            s_WorkBufferSize;
    static BYTE*                            s_UnpackBuffer;
    static DWORD                            s_UnpackBufferSize;
    static float                            s_UV[c_GPTexSide*2 + 1];

    static _inl bool            AdjustWorkBuffer    ( int size );
    static _inl bool            AdjustUnpackBuffer  ( int size );

private:        
    friend class                SpriteManager;
}; // class SpritePackage

const int c_MaxSpritesPerSurface = 512;
/*****************************************************************************/
/*    Class:    SpriteRenderBit
/*    Desc:    Transactive rendering task for the part of the sprite, occupying
/*              the same texture surface
/*****************************************************************************/
struct SpriteRenderBit
{
    int                 m_TexID;            //  texture ID
    const FrameChunk*   m_pChunk;           //  pointer to chunks list
    int                 m_NChunks;          //  number of sprite chunks rendered from this surface

    DWORD               m_DiffuseColor;     //  sprite diffuse color multiplier
    DWORD               m_NationalColor;
    Matrix4D            m_TM;               //  sprite transform

    VertexFormat        m_VF;     

    inline bool operator <( const SpriteRenderBit& rt ) const
    {
        return m_TexID < rt.m_TexID;    
    }
}; // struct SpriteRenderBit

typedef static_array<FrameInstance*, c_MaxFrameInstancesPerSurface>     PFrameInstanceArray;
typedef static_array<SpriteRenderBit*, c_MaxSpritesDrawn>               PSpriteRenderBitArray;
typedef static_array<SpriteRenderBit, c_MaxSpritesDrawn>                SpriteRenderBitArray;
/*****************************************************************************/
/*    Class:    SpriteSurface
/*    Desc:    Sprite surface layout
/*****************************************************************************/
class SpriteSurface
{
public:
    SpriteSurface    () :    m_TexID(-1), m_DropStamp(0xFFFFFFFF) { m_Layout.Init( c_GPTexSide ); }
    _inl int                AddFrameInstance( FrameInstance* pInstance );
    
    bool                    Free            ();
    void                    SetTextureID    ( int id ) { m_TexID = id; }
    int                     m_Index;    //  index in the sprite manager's surface array 
    int                     m_TexID;    //  ID of texture used
    QuadLayout              m_Layout;
    PFrameInstanceArray     m_pFrameInstance;

    //  used for .gp2 drawing style
    int                     m_PackageID;
    int                     m_PackageTexID;
    DWORD                   m_DropStamp;    
    static DWORD            s_DropStamp;
}; // class SpriteSurface


const int c_MaxPackageNameLen = 64;
/*****************************************************************/
/*  Struct:    PackageStub
/*  Desc:    
/*****************************************************************/
struct PackageStub
{
                    PackageStub() : m_pPackage(0), m_bNoFile(false) { m_Name[0] = 0; m_Path[0] = 0; }

    char            m_Name[c_MaxPackageNameLen];
    char            m_Path[_MAX_PATH];
    bool            m_bNoFile;

    SpritePackage*    m_pPackage;
}; // struct PackageStub

typedef PointerHash< FrameInstance, 
                     FrameInstance::Key, 
                     c_FrameInstanceHashTableSize,  
                     c_FrameInstanceHashPoolSize>           GPFrameHash;

typedef static_array<SpriteSurface, c_MaxSpriteSurfaces>    SurfaceArray;
typedef TypedPoolAllocator<FrameInstance,1024*1024*4>       InstanceAllocator;
/*****************************************************************/
/*  Class:    SpriteManager
/*  Desc:    Sprite manager
/*****************************************************************/
class SpriteManager : public SNode, public ISpriteManager, public IDeviceClient
{
public:
                        SpriteManager      ();
    virtual             ~SpriteManager     ();
    
    virtual void        Render             (); 
    virtual void        Init               ();
    virtual void        Purge              ();

    //  sprite drawing functions    
    virtual    bool     DrawSprite         ( int gpID, int sprID, const Vector3D& pos, bool mirrored = false, DWORD color = 0 );
    virtual    bool     DrawWSprite        ( int gpID, int sprID, const Vector3D& pos, bool mirrored = false, DWORD color = 0 );

    virtual    bool     DrawSprite         ( int gpID, int sprID, const Matrix4D& transf, DWORD color = 0 );
    virtual    bool     DrawWSprite        ( int gpID, int sprID, const Matrix4D& m, DWORD color = 0 );
    virtual    bool     DrawNSprite        ( int gpID, int sprID, const Matrix4D& transf, DWORD color = 0 );
    virtual    bool     DrawNWSprite       ( int gpID, int sprID, const Matrix4D& transf, DWORD color = 0 );

    virtual    bool     DrawWSprite        ( int gpID, int sprID, const Vector3D& pos, BaseMesh& geom, DWORD color = 0 );

    bool                DrawWChunk         ( int gpID, int sprID, int chunkID, const Vector3D& pos, 
                                                BaseMesh& geom, int cPoly, int nPoly, int cVert, int nVert, DWORD color );
    //  storage manipulation
    virtual    int      GetPackageID       ( const char* gpName );
    virtual    bool     LoadPackage        ( int gpID, const char* gpPath = NULL );
    virtual    void     SetPackagePath     ( int gpID, const char* gpPath );
    virtual    void     UnloadPackage      ( int gpID );
    virtual    void     Precache           ( int gpID );

    //  drawing manipulation
    virtual void        EnableClipping      ( bool enable = true );
    virtual float       SetScale            ( float scale );
    virtual void        SetLOD              ( int lod ) { m_CurLOD = lod; }
    virtual float       GetScale            () const { return m_CurScale; }    
    virtual void        SetZBias            ( float zbias ) { m_ZBias = zbias; }

    virtual bool        SetClipArea         ( const Rct& rct );
    virtual Rct         GetClipArea         () const { return m_ClipArea; }
    virtual int         GetFrameHeight      ( int gpID, int sprID );
    virtual int         GetFrameWidth       ( int gpID, int sprID );
    
    virtual bool        IsPackageLoaded     ( int gpID ) const;
    virtual int         GetNFrames          ( int gpID );
    
    virtual const char* GetPackageName      ( int gpID );
    virtual const char* GetPackagePath      ( int gpID );
    virtual DWORD       GetPackageMagic     ( int gpID );

    virtual void        SetManagedCache     ( bool bManaged ) {}
    virtual int         GetNPackages        () const {     return m_NPackages; }

    virtual void        SetCurrentDiffuse   ( DWORD color ) { m_CurDiffuse = color; }
    virtual DWORD       GetCurrentDiffuse   () const { return m_CurDiffuse; }
    virtual int         SetCurrentShader    ( int shID ) { int sh = m_Shader; m_Shader = shID; return sh; }
    virtual int         GetCurrentShader    () const { return m_Shader; }
    virtual void        SetTexCoordBias     ( float bias = 0.0f ) { m_TexBias = bias; }

    
    virtual bool        GetBoundFrame       ( int gpID, int sprID, Rct& frame, DWORD color = 0 );
    virtual bool        GetAABB             ( int gpID, int sprID, AABoundBox& aabb, DWORD color = 0 );

    virtual void        OnFrame             ();
    virtual void        Flush               ( bool bResetWorldTM = true );
    virtual void        EnableZBuffer       ( bool enable = true );
    bool                IsZBufferEnabled    () const { return m_bZBufferEnabled; }
    virtual DWORD       GetAlpha            ( int gpID, int sprID, DWORD color, int ptX, int ptY, bool precise = false );
    virtual int         GetNQuads           ( int gpID, int sprID );
    virtual bool        GetQuadRect         ( int gpID, int sprID, int quadID, Rct& rct );
    virtual DWORD       GetStatistics       ( SpriteStatistics stat ) const;

    virtual  bool       GetAABB             ( int gpID, AABoundBox& aabb ) const;
    virtual  bool       HasDepthData        ( int gpID ) const;
    virtual  bool       HasColorData        ( int gpID ) const;

    virtual void        Expose              ( PropertyMap& pm ){}

    virtual void        OnDestroyRS         ();
    virtual void        OnCreateRS          () {}

    _inl FrameInstance* FindFrameInstance   ( int gpID, int sprID, DWORD color, WORD lod = 0 );
    _inl int            AllocateQuad        ( int sidePow, WORD& ax, WORD& ay, FrameInstance* pInst, int prevID );
    FrameInstance*      AllocFrameInstance  ( int nExtraBytes ) { return m_InstanceAllocator.NewInstance( nExtraBytes ); }
    DWORD               AddFrameInstance    ( FrameInstance* pInst ) { return m_FrameReg.add( pInst->GetKey(), pInst ); }
    int                 GetSurfaceTexID     ( int surfID ) { return m_Surface[surfID].m_TexID; }
    void                AddSurfaceClient    ( int surfID, FrameInstance* pInst ) { m_Surface[surfID].AddFrameInstance( pInst ); }
    DWORD               GetDropStamp        ( int surfID ) const { return m_Surface[surfID].m_DropStamp; }
    BYTE*               LockSurfRect        ( int surfID, const Rct& rect, int& pitch );
    void                UnlockSurfRect      ();

protected:

    void                Drop                ( FrameInstance* frInst );
    _inl SpritePackage* GetPackage          ( int idx ) { return idx >= 0 ? m_PackageReg[idx].m_pPackage : NULL; }
    _inl SpritePackage* GetPackage          ( int idx ) const { return idx >= 0 ? m_PackageReg[idx].m_pPackage : NULL; }

    int                 FindPackageStub     ( const char* gpName ) const;
    int                 AddPackageStub      ( const char* gpName );

    _inl FrameInstance* GetFrameInstance    ( int& gpID, int& sprID, DWORD color, WORD lod = 0, bool bPrecache = true );
    int                 GetMappedDataSize   () const { return GetStatistics( ssMappedSize ); }

    _inl int            UnswizzleFrameIndex ( int gpID, int sprID );
    int                 GetNSurfaces        () const { return m_Surface.size(); }
    
    void                DrawBatches         ();
    void                sse_DrawBatches     ();

private:
    
    GPFrameHash             m_FrameReg;
    PackageStub             m_PackageReg[c_MaxGPSeqs];
    int                     m_NPackages;

    InstanceAllocator       m_InstanceAllocator;

    SpriteRenderBitArray    m_RenderBits;
    PSpriteRenderBitArray   m_SortedRenderBits;

    float                   m_CurScale;
    DWORD                   m_CurDiffuse;
    bool                    m_bZBufferEnabled;
    WORD                    m_CurLOD;
    bool                    m_bClippingEnabled;
    bool                    m_bInited;
    float                   m_UVBias;
    float                   m_ZBias;
    float                   m_TexBias;
    int                     m_TotalPackageBytes;
    int                     m_Shader;

    bool                    m_bUseSSE;
    
    SurfaceArray            m_Surface;
    float                   m_VMEMQuote;

    int                     m_LockSurfID;
    Rct                     m_LockRect;
    int                     m_CanvasTex;

    int                     m_CurSurface;

    BaseMesh                m_Prim;
    Timer                   m_Timer;
    
    Rct                     m_ClipArea;
    Group*                  m_pSurfaces;
    Group*                  m_pPackages;
    Group*                  m_pShaders;
};  // class SpriteManager



extern SpriteManager g_SpriteManager;

#ifdef _INLINES
#include "sgSpriteManager.inl"
#endif // _INLINES

#endif // __SGSPRITEMANAGER_H__