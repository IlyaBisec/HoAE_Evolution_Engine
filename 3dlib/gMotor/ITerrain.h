/*****************************************************************************/
/*    File:    ITerrain.h
/*    Desc:    Interface for working with terrain
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-15-2003
/*****************************************************************************/
#ifndef __ITERRAIN_H__
#define __ITERRAIN_H__

#include "IVertexBuffer.h"

class Vector3D;
class Rct;
class AABoundBox;
class ITerrainChunk;
class BaseMesh;

/*****************************************************************************/
/*    Class:    ITerrainCore
/*****************************************************************************/
class ITerrainCore
{
public:
    virtual bool                CreateTexture   ( int texID, const Rct& mapExt )        { return false; }
    virtual bool                UseTextureCache () const { return false; }
    virtual bool                CreateGeomery   ( const Rct& mapExt, int lod )          { return false; }
    virtual bool                GetAABB         ( const Rct& mapExt, AABoundBox& aabb ) { return false; }
    virtual void                SetHeight       ( float x, float y, float h ) {}
    virtual Vector3D            GetNormal       ( float x, float y ) = 0;
    virtual Vector3D            GetAvgNormal    ( float x, float y, float radius ) = 0;
    virtual float               GetHeight       ( float x, float y ) = 0;
    virtual VertexDeclaration   GetVDecl        () const = 0;
    virtual void                SetExtents      ( const Rct& rct ) {}
}; // class ITerrainCore

/*****************************************************************************/
/*    Class:    ITerrain
/*    Desc:    Interface for terrain manipulation
/*****************************************************************************/
typedef void tpInvalidateQuadCallback( Rct& rct);
class ITerrain
{
public:
    //  renders terain
    virtual void        Render              ()                           = 0;
    
    //  callback for calculating potentially visible set
    virtual void        InvalidateTexture   ( const Rct* rct = NULL )    = 0;
    virtual void        InvalidateGeometry  ( const Rct* rct = NULL )    = 0;
    virtual void        InvalidateAABB      ( const Rct* rct = NULL )    = 0;
    virtual void        SetInvalidateCallback( tpInvalidateQuadCallback* cb) = 0;

    //  forces to use quads of given LOD only
    virtual void        ForceLOD            ( int lod )                  = 0;
    virtual void        SetCore             ( ITerrainCore* pCore )      = 0;
    
    //  sets extents of the whole terrain
    virtual void        SetExtents          ( const Rct& ext )           = 0;
    virtual void        SetHeightmapPow     ( int hpow )                 = 0;
    virtual void        SetLODBias          ( float bias )               = 0;
    
    virtual Vector3D    GetNormal           ( float x, float y ) const   = 0;
    virtual Vector3D    GetAvgNormal        ( float x, float y, float radius ) const = 0;
    virtual float       GetH                ( float x, float y ) const   = 0;
    virtual void        SetH                ( int x, int y, float z )    = 0;

    virtual void        Init                ()                           = 0;
    virtual void        SetDrawCulling      ( bool bValue = true )       = 0;
    virtual void        SetDrawGeomCache    ( bool bValue = true )       = 0;
    virtual void        SetDrawTexCache     ( bool bValue = true )       = 0;
    virtual void        ResetDrawQueue      ()                           = 0;
    virtual BaseMesh*   AllocateGeometry    ()                           = 0;

    virtual void        ShowNormals         ( bool bShow = true )        = 0;
    virtual Rct         GetExtents          () const                     = 0;

    virtual void        DrawPatch           (   float worldX, float worldY, 
                                                float width, float height, 
                                                float rotation, 
                                                const Rct& uv, 
                                                float ux, float uy,
                                                int texID, DWORD color, 
                                                bool bAdditive )        = 0;
    virtual void        FlushPatches        ()                          = 0;
    virtual void        SubstShader         ( int shaderID )            = 0; 

    //  casts ray to the heightmap
    virtual bool        Pick                ( const Vector3D& orig, 
                                              const Vector3D& dir, 
                                              Vector3D& pt )            = 0;
    virtual bool        Pick                ( int mX, int mY, 
                                              Vector3D& pt )            = 0;
    
}; // class ITerrain

extern ITerrain*        ITerra;
#endif // __ITERRAIN_H__