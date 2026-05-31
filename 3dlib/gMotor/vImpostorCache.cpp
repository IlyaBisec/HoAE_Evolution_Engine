/*****************************************************************
/*  File:   vImpostorCache.h                                      
/*  Desc:   Impostor cache implementation
/*    Author: Ruslan Shestopalyuk
/*  Date:   Sep 2004
/*****************************************************************/
#include "stdafx.h"
#include "IImpostorCache.h"
#include "ICamera.h"
#include "IMediaManager.h"
#include "kQuadLayout.h"
#include "kHash.hpp"
#include "kBlockCache.hpp"

const int c_NumImpostorSurfaces     = 64;
const int c_ImpostorSurfaceSide     = 512;
const int c_RenderQSize             = 4096;
const int c_MaxAllocatedImpostors   = 2048;
/*****************************************************************************/
/*  Class:  ImpostorInstance
/*  Desc:   Instance of the impostor
/*****************************************************************************/
class ImpostorInstance
{
protected:
    DWORD       m_ModelID;      //  impostor model
    DWORD       m_AnimID;       //  impostor animation
    int         m_AnimTime;     //  quantized animation time
    int         m_Phi;          //  quantized model pitch in camera space
    int         m_Theta;        //  quantized model yaw in camera space
    int         m_QuadSide;     //  side of quad in the layout
    
    DWORD       m_LastUsed;     //  last frame when impostor was rendered
    Rct         m_UV;           //  uv on the impostor surface
    float       m_RefX, m_RefY; //  projected pivot coordinates
    float       m_ViewVol;      //  view volume of the impostor camera
    int         m_SurfaceID;    //  id of the impostor surface

    friend class ImpostorCache;
    friend class ImpostorSurface;

public:
                    ImpostorInstance() {}
                    ImpostorInstance( DWORD modelID, DWORD animID, int animTime, int theta, int phi, int quadSide )
                        :   m_ModelID   (modelID    ),
                            m_AnimID    (animID     ),
                            m_AnimTime  (animTime   ),
                            m_Phi       (phi        ),
                            m_Theta     (theta      ),
                            m_QuadSide  (quadSide   ),
                            m_LastUsed  (0          ),
                            m_SurfaceID (-1         ) {}

                    ImpostorInstance( DWORD modelID, int theta, int phi, int quadSide )
                         :  m_ModelID   (modelID    ),
                            m_AnimID    (0          ),
                            m_AnimTime  (0          ),
                            m_Phi       (phi        ),
                            m_Theta     (theta      ),
                            m_QuadSide  (quadSide   ),
                            m_LastUsed  (0          ),
                            m_SurfaceID (-1         ) {}

    unsigned int hash() const
    {
        DWORD h =   2741*m_Phi + 1987*m_Theta + 
                    541*m_ModelID + 173*m_AnimID + 
                    113*m_AnimTime + 7*m_QuadSide + 29;
        h = (h << 13) ^ h;
        h += 15731;
        return h;
    }

    bool equal( const ImpostorInstance& el )
    {
        return  (m_ModelID  == el.m_ModelID     ) &&
                (m_AnimID   == el.m_AnimID      ) &&
                (m_AnimTime == el.m_AnimTime    ) &&
                (m_Phi      == el.m_Phi         ) &&
                (m_Theta    == el.m_Theta       ) &&
                (m_QuadSide == el.m_QuadSide    );
    }
    
    void copy( const ImpostorInstance& el )
    {
        m_ModelID   = el.m_ModelID; 
        m_AnimID    = el.m_AnimID;  
        m_AnimTime  = el.m_AnimTime;
        m_Phi       = el.m_Phi;   
        m_Theta     = el.m_Theta;     
        m_QuadSide  = el.m_QuadSide;
        m_UV        = el.m_UV;
        m_SurfaceID = el.m_SurfaceID;
        m_RefX      = el.m_RefX;
        m_RefY      = el.m_RefY;
        m_ViewVol   = el.m_ViewVol;
    }

}; // class ImpostorInstance


/*****************************************************************************/
/*  Class:  ImpostorRenderTask
/*  Desc:   Element of the impostor render queue
/*****************************************************************************/
struct ImpostorRenderTask
{
    DWORD       m_ModelID;      //  impostored model id
    DWORD       m_AnimID;       //  model's animation id
    float       m_AnmTime;      //  animation time
    Matrix4D    m_TM;           //  models world transform

    Rct         m_UV;           //  uv on the impostor surface
    float       m_RefX, m_RefY; //  projected pivot coordinates
    float       m_Side;         //  side of the impostor billboard, in world space
    int         m_SurfaceID;    //  id of the impostor surface
    float       m_Rotation;     //  rotation of the billboard in screen space
}; // class ImpostorRenderTask

/*****************************************************************************/
/*  Class:  ImpostorSurface
/*  Desc:   Single surface of the impostor cache
/*****************************************************************************/
class ImpostorSurface
{
protected:
    QuadLayout              m_Layout;
    int                     m_TextureID;
    
    //  allocated impostors
    ImpostorInstance*       m_Allocated[c_MaxAllocatedImpostors];
    int                     m_NAllocated;
    
    friend class            ImpostorCache;

public:
                            ImpostorSurface ();
    bool                    Allocate        ( ImpostorInstance& imp );
    void                    Free            ();

}; // class ImpostorSurface

typedef Hash<ImpostorInstance> ImpostorHash;
typedef BlockCache< ImpostorSurface, 
                    ImpostorInstance, 
                    c_NumImpostorSurfaces>  ImpostorSurfaceCache;
/*****************************************************************************/
/*  Class:  ImpostorCache
/*  Desc:   Impostor cache implementation
/*****************************************************************************/
class ImpostorCache : public IImpostorCache
{
    ImpostorHash            m_Hash;                     //  storage of impostor instance desriptors
    bool                    m_bInited;                  //  true when cache is initialized
    ImpostorSurfaceCache    m_Cache;                    //  impostor render targets cache
    ImpostorRenderTask      m_RenderQ[c_RenderQSize];   //  render queue
    ImpostorRenderTask*     m_SortedRenderQ [c_RenderQSize];
    int                     m_RenderQSize;              //  current size of the impostor render queue

    float                   m_TimeQuant;                //  animation time quantising ratio
    float                   m_ThetaQuant;               //  longtitude quantising ratio
    float                   m_PhiQuant;                 //  lattitude quantising ratio
    float                   m_PixelStretchRatio;        //  maximal ratio allowed for magnifying impostor pixels
    int                     m_NCreatedImpostors;        //  number of crated impostors on this frame
    int                     m_DepthID;                  //  depth buffer surface
    int                     m_LastRenderTarget;         //  last impostor render target used on this frame

public:
                        ImpostorCache   ();
                        ~ImpostorCache  ();
    virtual void        Flush           ();
    virtual void        DrawDebugInfo   ( int dbgSurf = -1 );
    virtual bool        Init            ();
    virtual void        Draw            ( DWORD modelID, const Matrix4D& tm );
    virtual void        Draw            ( DWORD modelID, DWORD animID, float anmTime, const Matrix4D& tm );
    virtual void        DumpSurfaces    ();
    
    static int CmpRenderTasks( const void *e1, const void *e2 )
    {
        const ImpostorRenderTask* pTask1 = *((const ImpostorRenderTask**)e1);
        const ImpostorRenderTask* pTask2 = *((const ImpostorRenderTask**)e2);
        return pTask1->m_SurfaceID - pTask2->m_SurfaceID;
    }

protected:
    bool                CreateImpostor  ( ImpostorInstance& imp );

}; // class ImpostorCache

IImpostorCache* IImpCache;
ImpostorCache   g_ImpostorCache;

/*****************************************************************************/
/*  ImpostorSurface implementation
/*****************************************************************************/
ImpostorSurface::ImpostorSurface() : m_NAllocated(0) 
{
    m_Layout.Init( c_ImpostorSurfaceSide );
}

bool ImpostorSurface::Allocate( ImpostorInstance& imp )
{
    if (m_NAllocated == c_MaxAllocatedImpostors) return false;
    WORD ax, ay;
    if (!m_Layout.AllocChunk( GetPower( (PowerOfTwo)imp.m_QuadSide ), ax, ay )) return false;
    imp.m_UV = Rct( ax, ay, imp.m_QuadSide, imp.m_QuadSide );
    imp.m_UV /= float( c_ImpostorSurfaceSide );
    m_Allocated[m_NAllocated++] = &imp;
    return true;
} // ImpostorSurface::Allocate

void ImpostorSurface::Free()
{
    m_Layout.Free();
    for (int i = 0; i < m_NAllocated; i++) m_Allocated[i]->m_SurfaceID = -1;
    m_NAllocated = 0;
    IRS->PushRenderTarget( m_TextureID );
    IRS->ClearDevice( 0x00000000 );
    IRS->PopRenderTarget();
} // ImpostorSurface::Free

/*****************************************************************************/
/*  ImpostorCache implementation
/*****************************************************************************/
ImpostorCache::ImpostorCache()
{
    IImpCache           = this;
    m_bInited           = false;
    m_RenderQSize       = 0;
    m_TimeQuant         = 1000.0f/16.0f;
    m_ThetaQuant        = c_PI/8.0f;
    m_PhiQuant          = c_PI/8.0f;
    m_NCreatedImpostors = 0;
    m_PixelStretchRatio = 0.5f;
    m_DepthID           = -1;
    m_LastRenderTarget  = -1;
} // ImpostorCache::ImpostorCache

ImpostorCache::~ImpostorCache()
{
}

const float c_BorderW = 8.0f;
void ImpostorCache::DrawDebugInfo( int dbgSurf )
{
    if (dbgSurf >= c_NumImpostorSurfaces) dbgSurf = -1;
    int nSide = sqrtf( float( c_NumImpostorSurfaces ) );
    int cImp = 0;
    Rct vp = IRS->GetViewPort();
    float surfS = vp.h/float( nSide ) - c_BorderW;
    float bX = (vp.w - (surfS + c_BorderW)*float( nSide ))*0.5f;

    if (dbgSurf >= 0) 
    {
        surfS = c_ImpostorSurfaceSide;
         bX = (vp.w - surfS)*0.5f;
    }

    float cX = bX;
    float cY = 0.0f;
    int cSurf = 0;
    for (int i = 0; i < nSide; i++)
    {
        cX = bX;
        for (int j = 0; j < nSide; j++)
        {
            if (dbgSurf >= 0 && cSurf != dbgSurf) { cSurf++; continue; }
            Rct rct( cX, cY, surfS, surfS );
            rsSetTexture( m_Cache.GetBlock( cSurf ).m_TextureID );
            static int shHud = IRS->GetShaderID( "hud" );
            rsSetShader( shHud );
            rsRect( rct, Rct::unit, 0.0f, 0xFFFFFFFF );
            rsFrame( rct, 0.0f, 0x44FF0000 );
            rsFlushPoly2D();
            cSurf++;
            if (cSurf >= c_NumImpostorSurfaces) break;
            if (dbgSurf < 0) cX += surfS + c_BorderW;
        }
        if (cSurf >= c_NumImpostorSurfaces) break;
        if (dbgSurf < 0) cY += surfS + c_BorderW;
    }

    DrawText( 10, 100, 0xFFFF0000, "CreatedImpostors:%d", m_NCreatedImpostors );
    rsRestoreShader();
    rsFlushLines2D();
    FlushText();
} // ImpostorCache::DrawDebugInfo


template <class TVert>
bool AddQuad(BaseMesh& pri, const Vector3D& pos, 
             float sizeX, float sizeY, float refX, float refY,
             float rot, DWORD color, const Rct& uv, const Matrix4D& camTM )
{
    int nVert = pri.getNVert();
    if (nVert == pri.getMaxVert()) return false;
    TVert* v = ((TVert*)pri.getVertexData() + nVert);

    float cr = 1.0f;
    float sr = 0.0f;
    if (rot > 0.0f)
    {
        cr = cosf( rot );
        sr = sinf( rot );
    }
    float xc = refX * sizeX;
    float yc = refY * sizeY;

    Matrix4D bTM(   cr*sizeX,       sr*sizeX,       0.0f, 0.0f,
                    -sr*sizeY,      cr*sizeY,       0.0f, 0.0f, 
                    0.0f,           0.0f,           1.0f, 0.0f,
                    -cr*xc + sr*yc, -sr*xc - cr*yc, 0.0f, 1.0f );

    bTM *= camTM;
    Vector4D lt( 0.0f, 0.0f, 0.0f, 1.0f );
    Vector4D rt( 1.0f, 0.0f, 0.0f, 1.0f );
    Vector4D lb( 0.0f, 1.0f, 0.0f, 1.0f );
    Vector4D rb( 1.0f, 1.0f, 0.0f, 1.0f );

    lt *= bTM;
    rt *= bTM;
    lb *= bTM;
    rb *= bTM;

    v[0].x = lt.x + pos.x;
    v[0].y = lt.y + pos.y;
    v[0].z = lt.z + pos.z;

    v[1].x = rt.x + pos.x;
    v[1].y = rt.y + pos.y;
    v[1].z = rt.z + pos.z;

    v[2].x = lb.x + pos.x;
    v[2].y = lb.y + pos.y;
    v[2].z = lb.z + pos.z;

    v[3].x = rb.x + pos.x;
    v[3].y = rb.y + pos.y;
    v[3].z = rb.z + pos.z;

    //  color and texture coordinates
    v[0].diffuse = color;
    v[0].u         = uv.x;
    v[0].v         = uv.y + uv.h;

    v[1].diffuse = color;
    v[1].u         = uv.x + uv.w;
    v[1].v         = uv.y + uv.h;

    v[2].diffuse = color;
    v[2].u         = uv.x;
    v[2].v         = uv.y;

    v[3].diffuse = color;
    v[3].u         = uv.x + uv.w;
    v[3].v         = uv.y;

    pri.setNPri ( nVert/2 + 2 );
    pri.setNVert( nVert + 4 );
    return true;
} // AddQuad

//  estimates screen space quad area side, occupied when projecting given bounding box
float EstimateScreenSpacePixels( const AABoundBox& aabb, const Vector3D& pos )
{
    ICamera* cam = GetCamera();
    if (!cam) return 0.0f;
    Vector3D minv, maxv;
    return 0.0f;
} // EstimateScreenSpacePixels

void ImpostorCache::Flush()
{
    if (m_RenderQSize == 0) return;
    ICamera* pCam = GetCamera();
    Matrix4D camTM      = pCam->GetWorldTM();
    Matrix4D prTM       = pCam->GetProjTM();

    Rct      viewPort   = IRS->GetViewPort();
    Matrix4D viewTM     = IRS->GetViewTM();
    Matrix4D projTM     = IRS->GetProjTM();
    Matrix4D rotTM      = camTM;
    rotTM.setTranslation( Vector3D::null );

    //  precache animated models' impostors
    int cTask = 0;
    m_NCreatedImpostors = 0;
    m_LastRenderTarget = -1;
    for (int i = 0; i < m_RenderQSize; i++)
    {
        ImpostorRenderTask& rt = m_RenderQ[cTask];
        //  calculate impostor attributes
        const Matrix4D& tm = rt.m_TM;
        Vector3D dir;
        dir.x = tm.e30 - camTM.e30;
        dir.y = tm.e31 - camTM.e31;
        dir.z = tm.e32 - camTM.e32;
        //AABoundBox aabb = IMM->GetBoundBox( rt.m_ModelID );
        //float pix       = EstimateScreenSpacePixels( aabb, dir )*m_PixelStretchRatio;
        
        dir.normalize();
        Vector3D td, up;
        td.x = dir.x*tm.e00 + dir.y*tm.e01 + dir.z*tm.e02;
        td.y = dir.x*tm.e10 + dir.y*tm.e11 + dir.z*tm.e12;
        td.z = dir.x*tm.e20 + dir.y*tm.e21 + dir.z*tm.e22;

        //  object 'up' axis to camera space
        up.x = tm.e02*camTM.e00 + tm.e12*camTM.e01 + tm.e22*camTM.e02;
        up.y = tm.e02*camTM.e10 + tm.e12*camTM.e11 + tm.e22*camTM.e12;
        up.z = tm.e02*camTM.e20 + tm.e12*camTM.e21 + tm.e22*camTM.e22;

        float rot       = atan2f( up.x, up.y );  
        float scale     = tm.getV0().norm();
        float theta     = atan2f( td.y, td.x );
        float phi       = atan2f( td.z, sqrtf( td.x*td.x + td.y*td.y ) );

        int nTime       = rt.m_AnmTime/m_TimeQuant;
        int nTheta      = theta/m_ThetaQuant;
        int nPhi        = phi/m_PhiQuant;
        int qSide       = 64;

        //  look for an impostor instance in the cache
        int hIdx = m_Hash.add( ImpostorInstance( rt.m_ModelID, rt.m_AnimID, nTime, nTheta, nPhi, qSide ) );
        ImpostorInstance& hImp = m_Hash.elem( hIdx );        
        if (hImp.m_SurfaceID == -1) 
        {
            if (!CreateImpostor( hImp )) continue;
            m_NCreatedImpostors++;
        }
        else
        {
            m_Cache.HitBlock( hImp.m_SurfaceID );
        }
        rt.m_SurfaceID  = hImp.m_SurfaceID;
        rt.m_UV         = hImp.m_UV; 
        rt.m_RefX       = hImp.m_RefX;
        rt.m_RefY       = hImp.m_RefY;
        rt.m_Side       = hImp.m_ViewVol*scale;
        rt.m_Rotation   = rot;
        m_SortedRenderQ[cTask] = &rt;
        cTask++;
    }
    m_RenderQSize = cTask;
    if (m_NCreatedImpostors > 0) 
    {
        IRS->PopRenderTarget();
        IRS->SetViewPort    ( viewPort );
        IRS->SetViewTM      ( viewTM );
        IRS->SetProjTM      ( projTM );

    }

    //  sort impostors by texture
    qsort( m_SortedRenderQ, m_RenderQSize, sizeof( ImpostorRenderTask* ), CmpRenderTasks );
    
    //  render impostors as billboards
    static BaseMesh bm;
    if (bm.getMaxVert() == 0)
    {
        bm.create( c_RenderQSize*4, 0, vfVertex2t );
        bm.setIsQuadList( true );
        bm.setShader( IRS->GetShaderID( "cached_impostor" ) );
    }

    int cImp = 0;
    int nImp = 0;
    int cSurf = m_SortedRenderQ[0]->m_SurfaceID;
    IRS->ResetWorldTM();

    bm.setNVert( 0 );
    bm.setNPri ( 0 );
    while (cImp < m_RenderQSize)
    {
        const ImpostorRenderTask& rt = *m_SortedRenderQ[cImp]; 
        int surfID = rt.m_SurfaceID;
        if (surfID != cSurf && surfID != -1)
        //  render batch
        {
            const ImpostorSurface& surf = m_Cache.GetBlock( cSurf );
            bm.setTexture( surf.m_TextureID );
            bm.setNVert( nImp*4 );
            bm.setNPri ( nImp*2 );

            DrawBM( bm );
            
            bm.setNVert( 0 );
            bm.setNPri ( 0 );
            cSurf = surfID;
            nImp = 0;
        }
        //  add billboard quad to the batch
        float sizeX = rt.m_Side;
        float sizeY = rt.m_Side;
        AddQuad<Vertex2t>( bm, rt.m_TM.getTranslation(), 
                            sizeX, sizeY, rt.m_RefX, rt.m_RefY, rt.m_Rotation, 
                            0xFFFFFFFF, rt.m_UV, rotTM );
        cImp++;
        nImp++;
    }
    
    //  last bucket pass
    const ImpostorSurface& surf = m_Cache.GetBlock( cSurf );
    bm.setTexture( surf.m_TextureID );
    bm.setNVert( nImp*4 );
    bm.setNPri ( nImp*2 );
    DrawBM( bm );

    //  clear render queue
    m_RenderQSize = 0;
} // ImpostorCache::Flush


bool ImpostorCache::CreateImpostor( ImpostorInstance& imp )
{
    //  allocate place on the texture surface
    int surfID = m_Cache.Allocate( &imp );
    if (surfID == -1) return false;
    imp.m_SurfaceID = surfID;
    
    //  render model to the surface
    const ImpostorSurface& surf = m_Cache.GetBlock( surfID );
    if (surf.m_TextureID != m_LastRenderTarget)
    {
        IRS->PushRenderTarget( surf.m_TextureID, m_DepthID );
        IRS->ClearDevice( 0, false, true );
        m_LastRenderTarget = surf.m_TextureID;
    }
    Rct vp( imp.m_UV );
    vp *= c_ImpostorSurfaceSide;
    IRS->SetViewPort( vp );
    
    AABoundBox aabb = IMM->GetBoundBox( imp.m_ModelID );
    float radius = aabb.GetDiagonal() * 0.5f;
    radius *= 1.3f; // HACK!!!! Should be exact bbox
    Vector3D cn = aabb.GetCenter();

    Vector3D lookDir;
    float phi   = float( imp.m_Phi )*m_PhiQuant;
    float theta = float( imp.m_Theta )*m_ThetaQuant;
    lookDir.x   = cosf( phi )*cosf( theta );
    lookDir.y   = cosf( phi )*sinf( theta );
    lookDir.z   = sinf( phi );

    ICamera* pCam = GetCamera();
    Matrix4D impViewTM, impProjTM;
    //  setup orthogonal camera
    Matrix4D viewM = pCam->GetWorldTM();
    Vector3D vZ = lookDir;
    Vector3D vY = Vector3D::oZ; 
    Vector3D vX;
    vX.cross( vY, vZ );
    vX.z = 0.0f;
    vX.normalize();
    vY.cross( vZ, vX );
    vY.normalize();
    impViewTM.e00 = vX.x; impViewTM.e10 = vY.x; impViewTM.e20 = vZ.x; impViewTM.e03 = 0.0f;
    impViewTM.e01 = vX.y; impViewTM.e11 = vY.y; impViewTM.e21 = vZ.y; impViewTM.e13 = 0.0f;
    impViewTM.e02 = vX.z; impViewTM.e12 = vY.z; impViewTM.e22 = vZ.z; impViewTM.e23 = 0.0f;
    impViewTM.e30 = cn.x; impViewTM.e31 = cn.y; impViewTM.e32 = cn.z; impViewTM.e33 = 1.0f;
    impViewTM.inverse();

    //  create light projection matrix
    OrthoProjectionTM( impProjTM, radius*2.0f, 1.0f, -radius*2.0f, radius*2.0f );

    cn = impViewTM.getTranslation();
    impProjTM.transformPt( cn );
    imp.m_RefX = (cn.x + 1.0f)*0.5f;
    imp.m_RefY = (cn.y + 1.0f)*0.5f;
    imp.m_ViewVol = radius*2.0f;

    IRS->SetViewTM  ( impViewTM );
    IRS->SetProjTM  ( impProjTM );

    IMM->StartModel( imp.m_ModelID, Matrix4D::identity );
    IMM->AnimateModel( imp.m_AnimID, float( imp.m_AnimTime )*m_TimeQuant );
    IMM->DrawModel();

    return true;
} // ImpostorCache::CreateImpostor

void ImpostorCache::Draw( DWORD modelID, const Matrix4D& tm )
{
    if (m_RenderQSize == c_RenderQSize) Flush();
    ImpostorRenderTask& rt = m_RenderQ[m_RenderQSize++];
    rt.m_ModelID  = modelID;
    rt.m_AnimID   = 0xFFFFFFFF;
    rt.m_AnmTime  = 0.0f;
    rt.m_TM       = tm;
} // ImpostorCache::Draw

void ImpostorCache::Draw( DWORD modelID, DWORD animID, float anmTime, const Matrix4D& tm )
{
    if (m_RenderQSize == c_RenderQSize) Flush();
    ImpostorRenderTask& rt = m_RenderQ[m_RenderQSize++];
    rt.m_ModelID    = modelID;
    rt.m_AnimID     = animID;
    rt.m_AnmTime    = anmTime;
    rt.m_TM         = tm;
    rt.m_SurfaceID  = -1;
} // ImpostorCache::Draw 

bool ImpostorCache::Init()
{
    if (m_bInited) return true;

    for (int i = 0; i < c_NumImpostorSurfaces; i++)
    {
        ImpostorSurface& surf = m_Cache.GetBlock( i );
        char tname[256];
        sprintf( tname, "ImpostorSurf%d", i );
        surf.m_TextureID = IRS->CreateTexture( tname, c_ImpostorSurfaceSide, c_ImpostorSurfaceSide, 
                                                cfARGB4444, 1, tmpDefault, true );
        surf.Free();
    }

    //  create depth buffer surface
    m_DepthID = IRS->CreateTexture( "ImpostorDepthBuffer", c_ImpostorSurfaceSide, c_ImpostorSurfaceSide, 
                                        cfUnknown, 1, tmpDefault, false, dsfD16 );
    m_bInited = true;
    return true;
} // ImpostorCache::Init

void ImpostorCache::DumpSurfaces()
{
    char fname[_MAX_PATH];
    for (int i = 0; i < c_NumImpostorSurfaces; i++)
    {
        sprintf( fname, "c:\\dumps\\textures\\ImpTex%02d.dds", i );
        IRS->SaveTexture( m_Cache.GetBlock( i ).m_TextureID, fname );
    }
} // ImpostorCache::DumpSurfaces



