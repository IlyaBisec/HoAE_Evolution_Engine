/*****************************************************************************/
/*    File:    rsRenderSystem.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    23.01.2003
/*****************************************************************************/
#ifndef __RSRENDERSYSTEM_H__
#define __RSRENDERSYSTEM_H__
#pragma once

#include <vector>
#include <xstring>

#include "IRenderSystem.h"

/*****************************************************************************/
/*    Enum:    DeviceCapability
/*****************************************************************************/
enum DeviceCapability
{
    dcUnknown                        = 0,
    dcIndexedVertexBlending            = 1,
    dcBumpEnvMap                    = 2,
    dcBumpEnvMapLuminance            = 3,
    dcDynamicTextures                = 4,
    dcTnL                            = 5,
    dcPure                            = 6,
    dcRasterization                    = 7,
    dcBezier                        = 8,
    dcRTPatches                        = 9,
    dcNPatches                        = 10,
    dcREF                            = 11
};    // enum DeviceCapability

/*****************************************************************************/
/*    Class:    DeviceDescr
/*    Desc:    3D device capabilities
/*****************************************************************************/
class DeviceDescr
{
public:
    DeviceDescr();

    int                adapterOrdinal;
    std::string        devDriver;
    std::string        devDescr;
    
    int                texBlendStages;
    int                texInSinglePass;

    std::vector<DeviceCapability>        capBits;
    std::vector<DepthStencilFormat>    depthStencil;
    std::vector<ColorFormat>            renderTarget;    
}; // class DeviceDescr

//  primitive drawing helper utilities
class Rct;
class Vector3D;

DIALOGS_API void rsLine            ( float x1, float y1, float x2, float y2, float z, DWORD color );
DIALOGS_API void rsLine            ( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 );
DIALOGS_API void rsFrame        ( const Rct& rct, float z, DWORD color );

DIALOGS_API void rsFlushLines2D    ( bool bShaded = true );
DIALOGS_API void rsFlushLines3D    ( bool bShaded = true );
DIALOGS_API void rsFlushPoly2D    ( bool bShaded = true );
DIALOGS_API void rsFlushPoly3D    ( bool bShaded = true );
DIALOGS_API void rsFlush        ( bool bShaded = true );

DIALOGS_API void rsLine            ( const Vector3D& a, const Vector3D& b, DWORD color );
DIALOGS_API void rsLine            ( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 );
DIALOGS_API void rsEnableZ        ( bool enable = true );

DIALOGS_API void rsRestoreShader();
DIALOGS_API void rsSetShader    ( int shader );
DIALOGS_API void rsSetTexture    ( int texID, int stage = 0 );

DIALOGS_API void rsRect            ( const Rct& rct, float z, DWORD color );
DIALOGS_API void rsRect            ( const Rct& rct, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd );
DIALOGS_API void rsRect            ( const Rct& rct, const Rct& uv, float z, DWORD color );
DIALOGS_API void rsRect            ( const Rct& rct, const Rct& uv, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd );
DIALOGS_API void rsRect(const float cx, const float cy, const float Side, DWORD Clr);

DIALOGS_API void rsPanel(  const Rct& rct, float z, 
                        DWORD clrTop = 0xFFFFFFFF, 
                        DWORD clrMdl = 0xFFD6D3CE, 
                        DWORD clrBot = 0xFF848284 );

DIALOGS_API void rsColoredPanel ( const Rct& rct, float z, DWORD clr );
DIALOGS_API void rsThickPanel   ( const Rct& rct, float z, DWORD clr );
DIALOGS_API void rsInnerPanel   ( const Rct& rct, float z, DWORD clr );
//DIALOGS_API void rsArrowPanel   ( const Rct& rct, float z, DWORD clr, int dir );

DIALOGS_API void rsPoly            ( const Vector3D& a, const Vector3D& b, const Vector3D& c, 
                        DWORD acol, DWORD bcol, DWORD ccol );
DIALOGS_API void rsPoly            ( const Vector3D& a, const Vector3D& b, const Vector3D& c, 
                        float au, float av, float bu, float bv, float cu, float cv,
                        DWORD acol, DWORD bcol, DWORD ccol );
DIALOGS_API void rsPoly            ( const Vector3D& a, const Vector3D& b, const Vector3D& c, 
                     float au, float av, float bu, float bv, float cu, float cv,
                     float au2, float av2, float bu2, float bv2, float cu2, float cv2,
                     DWORD acol, DWORD bcol, DWORD ccol );
DIALOGS_API void rsPoly2D       ( const Vector3D& a, const Vector3D& b, const Vector3D& c, 
                        DWORD acol, DWORD bcol, DWORD ccol );

DIALOGS_API void rsPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD color );
DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
                     DWORD acol, DWORD bcol, DWORD ccol, DWORD dcol );

DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
                        const Rct& uv, DWORD acol, DWORD bcol, DWORD ccol, DWORD dcol );
DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
                      const Rct& uv, DWORD col );
DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
                      const Rct& uv, const Rct& uv2, DWORD col );

DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, DWORD color );

DIALOGS_API void rsQuad2D       ( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
                      DWORD acol, DWORD bcol, DWORD ccol, DWORD dcol, const Rct& uv );
//  Drawing different useful primitives
const int        c_DefaultSphereSegments = 8;
const float        c_DefRayLen                = 10000.0f; 
const float        c_DefHandleSide            = 5.0f;
const float        c_DefQuadSide            = 30.0f;
const float        c_DefNormalLen            = 8.0f;

class Matrix4D;

class Frustum;
class Plane;
class Line3D;
class Triangle;
class Sphere;
class Cylinder;
class Capsule;
class Cone;
class AABoundBox;
class BaseMesh;

//DIALOGS_API void CreateArrow( BaseMesh& bm, const Vector3D& start, const Vector3D& dir, DWORD color, float len = 1.0f );
DIALOGS_API void DrawArrow( const Vector3D& start, const Vector3D& dir, DWORD color, 
                        float len = 1.0f, float head = 0.1f, float headR = 0.05f );

DIALOGS_API void DrawFrustum( const Frustum& frustum, 
                     DWORD fillColor, DWORD linesColor, 
                     bool drawNormals = false );
DIALOGS_API void DrawCube( const Vector3D& center, DWORD color, float side = c_DefHandleSide );
DIALOGS_API void DrawRay( const Line3D& ray, DWORD linesColor, 
                     float rayLen = c_DefRayLen, 
                     float handleSide = c_DefHandleSide );

DIALOGS_API void DrawPlane( const Plane& plane, DWORD fillColor, DWORD color,
                     const Vector3D* center = NULL,
                     float qSide = c_DefQuadSide, 
                     float nLen = c_DefNormalLen );

DIALOGS_API void DrawTriangle    ( const Triangle& tri, DWORD linesColor, DWORD fillColor );
DIALOGS_API void DrawAABB        ( const AABoundBox& aabb, DWORD fillColor, DWORD linesColor );
DIALOGS_API void DrawSphere        ( const Sphere& sphere, 
                     DWORD fillColor, DWORD linesColor, 
                     int nSegments = c_DefaultSphereSegments );
DIALOGS_API void DrawCircle        ( const Vector3D& center, const Vector3D& normal, float radius, 
                     DWORD fillColor, DWORD linesColor, int nSegments );
DIALOGS_API void DrawCircle        ( float x, float y, float radius, DWORD fillColor, DWORD linesColor, int nSegments );
DIALOGS_API void DrawCircle8    ( const Vector3D& center, const Vector3D& normal, float radius, 
                     DWORD fillColor, DWORD linesColor );

DIALOGS_API void DrawSpherePatch( const Sphere& sphere, 
                     DWORD fillColor, DWORD linesColor, 
                     float phiBeg, float phiEnd,
                     float thetaBeg, float thetaEnd,
                     int nSegments, const Matrix4D* pTM = NULL );
DIALOGS_API void DrawStar        ( const Sphere& sphere, DWORD begColor, DWORD endColor, 
                     int nSegments = c_DefaultSphereSegments );

DIALOGS_API void DrawAnchor     ( const Vector3D& pos, float side, DWORD color, bool bBothSides = true );
DIALOGS_API void DrawAnchor     ( const Vector3D& pos, float side, DWORD cx, DWORD cy, DWORD cz, bool bBothSides = true );

DIALOGS_API void DrawCylinder    ( const Cylinder& cylinder, 
                     DWORD fillColor, DWORD linesColor, bool bCapped,
                     int nSegments = c_DefaultSphereSegments );
DIALOGS_API void DrawCapsule    ( const Capsule& capsule, 
                     DWORD fillColor, DWORD linesColor, 
                     int nSegments = c_DefaultSphereSegments );

DIALOGS_API void DrawCone        ( const Cone& cone, 
                     DWORD fillColor, DWORD linesColor, 
                     int nSegments = c_DefaultSphereSegments );

DIALOGS_API void DrawFatSegment ( const Vector3D& beg, const Vector3D& end, const Vector3D& normal, 
                        float width, bool bRoundEnds, DWORD color, DWORD coreColor = 0 );

DIALOGS_API void DrawPoint      ( const Vector3D& pt, DWORD clr = 0xFFFF0000, float rad = 5.0f );

DIALOGS_API bool DrawText       ( int x, int y, DWORD color, const char* format,  ... );
DIALOGS_API bool DrawText       ( const Vector3D& pos, DWORD color, const char* format, ... );
DIALOGS_API int  GetTextWidth   ( const char* format, ... );
DIALOGS_API int  GetTextHeight  ();
DIALOGS_API void DrawChar(int x,int y,char c,DWORD Color);
DIALOGS_API int GetCharWidth(char c);
DIALOGS_API int GetCharHeight(char c);

DIALOGS_API int GetTextHeightEx( const char* str ,int& MaxWidth,int AddLineHeight);
DIALOGS_API bool DrawTextEx( int x, int y, DWORD color, const char* str,int MaxWidth,int AddLineHeight);
DIALOGS_API void SetCurrentFont(BYTE Size);

DIALOGS_API void FlushText      ();

DIALOGS_API void DrawFaces      ( BaseMesh& bm, bool bDrawNumbers = true );
DIALOGS_API void DrawVertices   ( BaseMesh& bm, bool bDrawNumbers = true, bool bDrawHandles = true );
DIALOGS_API void DrawRawMesh    (   std::vector<Vector3D>& vert, 
                        std::vector<int>& ind, const Matrix4D& tm );

DIALOGS_API void DrawCurve      ( float width, int nPoints, const Vector3D* points, const DWORD* colors );
DIALOGS_API void DrawGrid       ( float side, int nCells, DWORD clr, DWORD subClr );

#endif // __RSRENDERSYSTEM_H__