/*****************************************************************/
/*  File:   ISpriteManager.h
/*  Desc:   Sprite manager abstract interface
/*  Desc:   3D rendering device abstract interface               
/*  Date:   Jan 2004                                             
/*****************************************************************/
#ifndef __ISPRITEMANAGER_H__
#define __ISPRITEMANAGER_H__

class Matrix4D;
class Plane;
class BaseMesh;
class Vector3D;
class Rct;
class Triangle2D;

const DWORD c_InsideChunks                = 0xFFFFFFFE;
const DWORD c_OutsideChunks               = 0x0;

/*****************************************************************/
/*  Enum:    SpriteStatistics
/*  Desc:    Different sprite manager statistics types
/*****************************************************************/
enum SpriteStatistics
{
    ssUnknown            = 0,
    ssMappedSize         = 1,    //  amount of filemapped memory space
    ssFrameInstances     = 2,    //  number of frame instances allocated so far
    ssFrameInstMemory    = 3        //    memory occupied by frame instances
}; // enum SpriteStatistics

/*****************************************************************/
/*  Class:    ISpriteManager
/*  Desc:    Sprite manager abstract interface
/*****************************************************************/
class ISpriteManager 
{
public:
    //  initializing system
    virtual void    Init        () = 0;
    //  sprite drawing functions
    virtual bool    DrawSprite  ( int gpID, int sprID, const Vector3D& pos, bool mirrored = false, DWORD color = 0 ) = 0;
    virtual bool    DrawWSprite ( int gpID, int sprID, const Vector3D& pos, bool mirrored = false, DWORD color = 0 ) = 0;

    virtual bool    DrawSprite  ( int gpID, int sprID, const Matrix4D& transf, DWORD color = 0 ) = 0;
    virtual bool    DrawWSprite ( int gpID, int sprID, const Matrix4D& transf, DWORD color = 0 ) = 0;
    virtual bool    DrawNSprite ( int gpID, int sprID, const Matrix4D& transf, DWORD color = 0 ) = 0;
    virtual bool    DrawNWSprite( int gpID, int sprID, const Matrix4D& transf, DWORD color = 0 ) = 0;
    virtual bool    DrawWSprite ( int gpID, int sprID, const Vector3D& pos, BaseMesh& geom, DWORD color = 0 ) = 0;
    virtual bool    DrawWChunk  ( int gpID, int sprID, int chunkID, const Vector3D& pos, 
                                        BaseMesh& geom, int cPoly, int nPoly, int cVert, int nVert, DWORD color = 0 ) = 0;

    //  storage manipulation
    virtual int     GetPackageID     ( const char* gpName ) = 0;
    virtual bool    LoadPackage      ( int gpID, const char* gpPath = NULL ) = 0;
    virtual void    UnloadPackage    ( int gpID ) = 0;
    virtual bool    IsPackageLoaded  ( int gpID ) const = 0;
    virtual void    SetPackagePath   ( int gpID, const char* gpPath ) = 0;
        
    //  drawing manipulation
    virtual void    EnableClipping   ( bool enable = true    ) = 0;
    virtual void    SetTexCoordBias  ( float bias = 0.0f     ) = 0;
    virtual float   SetScale         ( float scale           ) = 0;
    virtual void    SetLOD           ( int lod               ) = 0;
    
    virtual void    SetZBias         ( float zbias           ) = 0;
    virtual bool    SetClipArea      ( const Rct& rct        ) = 0;
    
    virtual void    SetCurrentDiffuse( DWORD color           ) = 0;
    virtual int     SetCurrentShader ( int shID              ) = 0;
    virtual void    EnableZBuffer    ( bool enable = true    ) = 0;
    virtual DWORD   GetCurrentDiffuse() const = 0;
    virtual int     GetCurrentShader () const = 0;
    virtual float   GetScale         () const = 0;    
    virtual Rct     GetClipArea      () const = 0;
    virtual DWORD   GetStatistics    ( SpriteStatistics stat ) const = 0;

    virtual bool    GetAABB          ( int gpID, AABoundBox& aabb ) const = 0;
    virtual bool    HasDepthData     ( int gpID ) const = 0;
    virtual bool    HasColorData     ( int gpID ) const = 0;

    virtual int     GetFrameHeight   ( int gpID, int sprID ) = 0;
    virtual int     GetFrameWidth    ( int gpID, int sprID ) = 0;

    virtual int     GetNFrames       ( int gpID ) = 0;
    virtual int     GetNPackages     () const = 0;
    virtual const char*    GetPackageName    ( int gpID ) = 0;
    virtual const char*    GetPackagePath    ( int gpID ) = 0;
    virtual DWORD    GetPackageMagic  ( int gpID ) = 0;
    
    virtual void     Precache         ( int gpID ) = 0;
    virtual bool     GetBoundFrame    ( int gpID, int sprID, Rct& frame, DWORD color = 0 ) = 0;
    virtual bool     GetAABB          ( int gpID, int sprID, AABoundBox& aabb, DWORD color = 0 ) = 0;
    virtual void     Purge            () = 0;

    virtual void     OnFrame          () = 0;
    virtual void     Flush            ( bool bResetWorldTM = true ) = 0;

    virtual DWORD    GetAlpha         ( int gpID, int sprID, DWORD color, int ptX, int ptY, bool precise = false ) = 0;
    virtual int      GetNQuads        ( int gpID, int sprID ) = 0;
    virtual bool     GetQuadRect      ( int gpID, int sprID, int quadID, Rct& rct ) = 0;
    virtual void     SetManagedCache  ( bool bManaged ) = 0;

}; // ISpriteManager

extern DIALOGS_API ISpriteManager* ISM;
ISpriteManager* GetSpriteManager();

#endif // __ISPRITEMANAGER_H__