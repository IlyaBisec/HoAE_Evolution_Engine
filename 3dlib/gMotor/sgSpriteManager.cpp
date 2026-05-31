/*****************************************************************/
/*  File:   sgSpriteManager.cpp
/*  Desc:   Sprite manager implementation
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "rsVertex.h"
#include "kCache.h"

#include "mMath2D.h"
#include "mTriangle.h"

#include "kHash.hpp"
#include "kResource.h"

#include "sgSpriteManager.h"
#include "sgGQuad.h"
#include "sgG17.h"
#include "sgG18.h"
#include "sgGN16.h"
#include "sgGU16.h"
#include "sgGU15.h"
#include "sgG2D.h"
#include "sgGP2.h"

#include "kUtilities.h" 
#include "kStrUtil.h"
#include "kBmptool.h"
#include "kFilePath.h"
#include "IResourceManager.h"

#include <algorithm>


#ifndef _INLINES
#include "sgSpriteManager.inl"
#endif // _INLINES

__forceinline void __prefetch64(DWORD _ptr){
    __asm{
        mov ebx,DWORD PTR _ptr
        and ebx,0xFFFFFFF0;
        prefetchnta [ebx]
    }
}



SpriteManager    g_SpriteManager;
ISpriteManager* GetSpriteManager()
{
    return &g_SpriteManager;
}

DIALOGS_API ISpriteManager* ISM = GetSpriteManager();

void DrawSpriteAsShadow(    int spID, int frID,         //  sprite ID 
                            const Vector3D& lightDir,   //  light direction
                            const Vector3D& pos,        //  sprite pos
                            int cx, int cy,
                            float scale )               //  sprite scale
{
    float crs = -lightDir.y*scale;
    float srs = lightDir.x*scale;

    Matrix4D m = Matrix4D::identity;
    m.e00 = crs;
    m.e01 = srs;
    m.e02 = 0.0f;
    m.e03 = 0.0f;

    m.e10 = -srs;
    m.e11 = crs;
    m.e12 = 0.0f;
    m.e13 = 0.0f;

    m.e20 = 0.0f;
    m.e21 = 0.0f;
    m.e22 = scale;
    m.e23 = 0.0f;

    m.e30 = pos.x - crs*cx + cy*srs;
    m.e31 = pos.y - crs*cy - cx*srs;
    m.e32 = pos.z;
    m.e33 = 1.0f;

    ISM->DrawWSprite( spID, frID, m );
} // DrawSpriteAsShadow

void DrawSpriteAsBillboard( int spID, int frID,         //  sprite ID 
                            const Vector3D& pos,        //  sprite pos
                            const Matrix3D& camTM,      //  camera rotation matrix
                            int cx, int cy,             //  sprite pivot
                            DWORD color,                //  national color
                            float scale )               //  sprite scale
{
    Matrix4D m;    

    m.e00 = camTM.e00*scale;
    m.e01 = camTM.e01*scale;
    m.e02 = camTM.e02*scale;
    m.e03 = 0.0f;

    m.e10 = -camTM.e10*scale;
    m.e11 = -camTM.e11*scale;
    m.e12 = -camTM.e12*scale;
    m.e13 = 0.0f;

    m.e20 = camTM.e20*scale;
    m.e21 = camTM.e21*scale;
    m.e22 = camTM.e22*scale;
    m.e23 = 0.0f;

    m.e30 = scale*(camTM.e10*cy - camTM.e00*cx) + pos.x;
    m.e31 = scale*(camTM.e11*cy - camTM.e01*cx) + pos.y;
    m.e32 = scale*(camTM.e12*cy - camTM.e02*cx) + pos.z;
    m.e33 = 1.0f;

    ISM->DrawWSprite( spID, frID, m, color );
} // DrawSpriteAsBillboard
void DrawSpriteAsVerticalBillboard( int spID, int frID,         //  sprite ID 
                            const Vector3D& pos,        //  sprite pos
                            const Matrix3D& camTM,      //  camera rotation matrix
                            int cx, int cy,             //  sprite pivot
                            DWORD color,                //  national color
                            float scale,				//  sprite scale
							float fall_stage)           
{
	Matrix4D m;   
	//static float st = 0.9;
	float s = sqrt(camTM.e00*camTM.e00+camTM.e01*camTM.e01);
    m.e00 = camTM.e00*scale/s;
    m.e01 = camTM.e01*scale/s;
    m.e02 = 0;
    m.e03 = 0.0f;

	if(fall_stage<1){
		float st=(1-fall_stage)*3;//sqrt(1-fall_stage*fall_stage);
		m.e10 = -camTM.e10*scale*st;
		m.e11 = -camTM.e11*scale*st;
		m.e12 = -scale*fall_stage*1.15;
	}else{
		m.e10 = 0;
		m.e11 = 0;
		m.e12 = -scale*fall_stage*1.15;
	}
    m.e13 = 0.0f;

    m.e20 = 0;
    m.e21 = 0;
    m.e22 = 0;
    m.e23 = 0.0f;

    m.e30 = pos.x - m.e00*cx - m.e10*cy;
    m.e31 = pos.y - m.e01*cx - m.e11*cy;
    m.e32 = pos.z - m.e02*cx - m.e12*cy;
    m.e33 = 1.0f;

    ISM->DrawWSprite( spID, frID, m, color );
	/*
    Matrix4D m;    

	Vector3D V(camTM.e00,camTM.e01,0);
	V.normalize();

    m.e00 = V.x*scale;
    m.e01 = V.y*scale;
    m.e02 = 0;
    m.e03 = 0.0f;

	V=Vector3D(camTM.e10,camTM.e11,0);
	V.normalize();

    m.e10 = -V.x*scale;
    m.e11 = -V.y*scale;
    m.e12 = 0;
    m.e13 = 0.0f;

    m.e20 = 0;
    m.e21 = 0;
    m.e22 = scale;
    m.e23 = 0.0f;

    m.e30 = -m.e10-m.e00 + pos.x;
    m.e31 = -m.e11-m.e01 + pos.y;
    m.e32 = -m.e12-m.e02 + pos.z;
    m.e33 = 1.0f;

    ISM->DrawWSprite( spID, frID, m, color );
	*/
} // DrawSpriteAsBillboard

/*****************************************************************
/*    SpritePackage implementation
/*****************************************************************/
BYTE*            SpritePackage::s_WorkBuffer            = NULL;
DWORD            SpritePackage::s_WorkBufferSize        = 0;
BYTE*            SpritePackage::s_UnpackBuffer        = NULL;
DWORD            SpritePackage::s_UnpackBufferSize    = 0;
float            SpritePackage::s_UV[c_GPTexSide*2 + 1];
std::vector<IPackageCreator*>    SpritePackage::s_Creators;

G18Creator        g_G18Creator;
G17Creator        g_G17Creator;
GU16Creator       g_GU16Creator;
GU15Creator       g_GU15Creator;
GN16Creator       g_GN16Creator;
GU2DCreator       g_GU2DCreator;
GP2Creator        g_GP2Creator;

SpritePackage::SpritePackage()
{
    memset( this, 0, sizeof( *this ) );
    
    m_NDirections   = 1;
    m_bHasDepthData = false;
    m_bHasColorData = false;
    m_AABB          = AABoundBox::null;
} // SpritePackage::SpritePackage

SpritePackage::~SpritePackage()
{
    Cleanup();
}

void SpritePackage::Cleanup()
{
    m_FileMapping.Close();
} // SpritePackage::Cleanup

void SpritePackage::Precache()
{
    m_FileMapping.TouchData();
} // SpritePackage::Precache

SpritePackage* SpritePackage::CreatePackage( const char* fname )
{
    FilePath path( fname );
    char name[_MAX_PATH];
    strcpy( name, fname );
    FileMapping fm( path.GetFullPath() );
    BYTE* pFileData = fm.GetPointer();
    if (!pFileData) return NULL;
    //  cycle through package factory and try to create package
    SpritePackage* pPackage = NULL;
    for (int i = 0; i < s_Creators.size(); i++)
    {
        pPackage = s_Creators[i]->CreatePackage( name, pFileData );
        if (pPackage) break;
    }
    if (!pPackage) return NULL;
    pPackage->m_FileMapping = fm;    
    pPackage->SetPath( path.GetFullPath() );
    pPackage->SetName( path.GetFileName() );

    int cPos = 0;
    int fSize = fm.GetFileSize(); 
    while (cPos < fSize)
    {
        DWORD magic = *((DWORD*)&pFileData[cPos]);
        DWORD size  = *((DWORD*)&pFileData[cPos + 4]); 
        if (magic == 'LAPG')
        {
            pPackage->SetPaletteOffset( 0, cPos + 8 + 18 );
            pPackage->SetPaletteOffset( 1, cPos + 8 + 18 + 1024 + 8 );
        }
        else if (magic == 'FNIG')
        {
            pPackage->SetNDirections( *((WORD*)&pFileData[cPos + 8 + 16]) );
        }
        cPos += size;
    }
    return pPackage;
};  //  SpritePackage::CreatePackage

/*****************************************************************/
/*  SpriteSurface implementation
/*****************************************************************/
DWORD SpriteSurface::s_DropStamp = 1;
bool SpriteSurface::Free()
{
    int nInst = m_pFrameInstance.size();
    for (int i = 0; i < nInst; i++) 
    {
        if (m_pFrameInstance[i]->IsLocked()) 
        {
            return false;
        }
    }

    for (int i = 0; i < nInst; i++) 
    {
        m_pFrameInstance[i]->Drop();
    }
    m_Layout.Free();

    m_DropStamp = ++s_DropStamp;
    m_pFrameInstance.clear();

    GU2DPackage::FreeSurface( m_Index );

    return true;
} // SpriteSurface::Free

/*****************************************************************/
/*  SpriteManager implementation
/*****************************************************************/
SpriteManager::SpriteManager() 
{
    SetName( "SpriteManager" );

    m_CurScale            = 1.0f;
    m_CurLOD            = 0;
    m_CurDiffuse        = 0xFFFFFFFF;
    m_bInited            = false;
    m_bClippingEnabled    = true;
    m_UVBias            = 0.0f;
    m_ZBias                = 0.04f;
    m_pSurfaces            = AddChild<Group>( "Surfaces" );
    m_pPackages            = AddChild<Group>( "Packages" );
    m_CurSurface        = 0;
    ISM                    = this;
    m_NPackages            = 0;
    m_TotalPackageBytes = 0;
    m_bUseSSE           = false;
    m_VMEMQuote         = c_SpriteManagerVMEMQuote;
    m_LockSurfID        = -1;
    m_LockRect          = Rct::unit;
    m_CanvasTex         = -1;

} // SpriteManager::SpriteManager

SpriteManager::~SpriteManager()
{
    for (int i = 0; i < m_NPackages; i++)
    {
        delete m_PackageReg[i].m_pPackage;
    }
}

//void SpriteManager::Expose( PropertyMap& pm )
//{
//    pm.start<Parent>( "SpriteManager", this );
//    pm.p( "NumSurfaces", GetNSurfaces );
//    pm.p( "EnableZ", IsZBufferEnabled, EnableZBuffer );
//    pm.f( "ZBias", m_ZBias );
//    pm.p( "MappedSize", GetMappedDataSize );
//    pm.m( "Purge", Purge );
//
//} // SpriteManager::Expose

void SpriteManager::Init()
{
    m_bUseSSE = HaveSSE();
    if (m_bInited) return;

    GP2Package::InitSurfaceCache();

    float texMem = IRS->GetTexMemorySize();
    float surfSz = c_GPTexSide*c_GPTexSide*2 + 1024;
    int nSurf    = tmin( c_MaxSpriteSurfaces, (int)((texMem*m_VMEMQuote)/surfSz) );
    m_Surface.resize( nSurf );

    TextureMemoryPool memPool = tmpDefault;

    for (int i = 0; i < nSurf; i++)
    {
        m_Surface[i].m_Index = i;

        char texName[256];
        sprintf( texName, "SpriteSurf%02d", i );
        int texID = IRS->CreateTexture( texName, c_GPTexSide, c_GPTexSide, cfARGB4444, 1, memPool );
        m_Surface[i].SetTextureID( texID );
        assert( texID != -1 );
        Texture* pTexture = m_pSurfaces->AddChild<Texture>( texName );
        pTexture->SetTexID( m_Surface[i].m_TexID );
    }
    m_CanvasTex = IRS->CreateTexture( "SpriteCanvas", c_GPTexSide, c_GPTexSide, cfARGB4444, 1, tmpSystem );

    m_Shader = IRS->GetShaderID( "hudZDisable" );
    Texture* pWhiteTex = AddChild<Texture>( "white.tga" );
    pWhiteTex->Render();
    
    EnableZBuffer( true );
    EnableClipping();

    static const int c_DefaultGPInstAllocatorPage = 1024*1024;
    m_InstanceAllocator.SetPageSize( c_DefaultGPInstAllocatorPage );

    float side = static_cast<float>( c_GPTexSide ); 
    for (int i = 0; i <= c_GPTexSide*2; i++)
    {
        SpritePackage::s_UV[i] = (static_cast<float>( i ))/side;
    }

    IRS->AddClient( this );
    m_bInited = true;

    static int shSprite2D = IRS->GetShaderID( "hud" );
    m_Prim.create            ( c_MaxVertDrawn, c_MaxIndDrawn, vfVertexTnL, ptTriangleList );
    m_Prim.setPriType        ( ptTriangleList );
    m_Prim.setShader         ( shSprite2D     );
    m_Prim.setVertexFormat   ( vfVertexTnL    );
    m_Prim.setNPri           ( 0 );
    m_Prim.setNInd           ( 0 );
    m_Prim.setNVert          ( 0 );
}  // SpriteManager::Init

void SpriteManager::Purge()
{
    Flush();
    m_FrameReg.reset();
    m_InstanceAllocator.Purge();
    for (int i = 0; i < m_Surface.size(); i++)
    {
        m_Surface[i].m_Layout.Free();
        m_Surface[i].m_pFrameInstance.clear();
        m_Surface[i].m_DropStamp = m_Surface[i].s_DropStamp++;
    }

    for (int i = 0; i < m_NPackages; i++)
    {
        PackageStub& stub = m_PackageReg[i];
        SpritePackage* pSeq = stub.m_pPackage;
        if (!pSeq) continue;
        pSeq->Cleanup();
    }

    GU2DPackage::CleanCache();
    GP2Package::CleanCache();
} // SpriteManager::Purge

DWORD SpriteManager::GetPackageMagic( int gpID )
{
    if (gpID < 0 || gpID >= m_NPackages) return 0xFFFFFFFF;
	PackageStub& stub = m_PackageReg[gpID];
	if (stub.m_bNoFile) return 0xFFFFFFFF; 
	SpritePackage* pPackage = stub.m_pPackage;
	if (!pPackage) 
	{
		LoadPackage( gpID );
		pPackage = GetPackage( gpID );
		if (!pPackage) return 0xFFFFFFFF;
	}

	return pPackage->GetMagic();
} // SpriteManager::GetPackageMagic

const char* SpriteManager::GetPackagePath( int gpID )
{
    const PackageStub& stub = m_PackageReg[gpID];
    if (stub.m_Path[0] == 0)
    //  default path
    {
        static FilePath path;
        path.SetPath( m_PackageReg[gpID].m_Name );
        int resID = -1;

        path.SetExt( "gp2" );
        resID = IRM->FindResource( path.GetFullPath() );
        if (resID != -1)
        { 
            const char* root = IRM->GetFullPath( resID );
            return root; 
        }

        path.SetExt( "g2D" );
        resID = IRM->FindResource( path.GetFullPath() );
        if (resID != -1)
        { 
            const char* root = IRM->GetFullPath( resID );
            return root; 
        }

        path.SetExt( "g16" );
        resID = IRM->FindResource( path.GetFullPath() );
        if (resID != -1)
        { 
            const char* root = IRM->GetFullPath( resID );
            return root; 
        }

        path.SetExt( "g17" );
        resID = IRM->FindResource( path.GetFullPath() );
        if (resID != -1)
        { 
            const char* root = IRM->GetFullPath( resID );
            return root; 
        }
        return IRM->GetHomeDirectory();
    }
    return stub.m_Path;
} // SpriteManager::GetPackagePath

void SpriteManager::EnableZBuffer( bool enable )
{
    static int shSprite2D    = IRS->GetShaderID( "hudZDisable" );
    static int shSprite2DZ    = IRS->GetShaderID( "hud" );

    if (enable == m_bZBufferEnabled) return; 
    m_bZBufferEnabled = enable;

    Flush();
    if (enable) m_Shader = shSprite2DZ; else m_Shader = shSprite2D;
} // SpriteManager::EnableZBuffer

/*---------------------------------------------------------------------------*/
/*    Func:    SpriteManager::GetAlpha    
/*    Desc:    returns alpha value of the given pixel inside sprite
/*    Parm:    ptX, ptY - points coordinates in local sprite space
/*            precise - when true, alpha is returned even if there is no pixel 
/*                data in system memory
/*    Ret:    Alpha value or, if pixel data is unavailable:
/*                c_InsideChunks - pixel is inside chunks
/*                c_OutsideChunks - pixel is outside chunks
/*---------------------------------------------------------------------------*/
DWORD SpriteManager::GetAlpha( int gpID, int sprID, DWORD color, int ptX, int ptY, bool precise )
{
    if (gpID < 0 || gpID >= m_NPackages || sprID < 0) return c_OutsideChunks;
    PackageStub& stub = m_PackageReg[gpID];
	if (stub.m_bNoFile) return c_OutsideChunks; 
	SpritePackage* pPackage = stub.m_pPackage;
	if (!pPackage) 
	{
		LoadPackage( gpID );
		pPackage = GetPackage( gpID );
		if (!pPackage) return c_OutsideChunks;
	}
    if (!pPackage->GetFrameBounds( sprID ).PtIn( ptX, ptY )) return c_OutsideChunks;
    
    FrameInstance* frameInst = GetFrameInstance( gpID, sprID, color, m_CurLOD, precise );
    if (frameInst == NULL) return c_OutsideChunks;
    if (!pPackage) return c_OutsideChunks;
    return pPackage->GetAlpha( frameInst, ptX, ptY, precise );
} // SpriteManager::GetAlpha

int SpriteManager::FindPackageStub( const char* gpName ) const
{
    for (int i = 0; i < m_NPackages; i++)
    {
        if (!stricmp( gpName, m_PackageReg[i].m_Name )) return i;
    }
    return -1;
} // SpriteManager::FindPackageStub

int    SpriteManager::AddPackageStub( const char* gpName )
{
    if (m_NPackages == c_MaxGPSeqs)
    {
        Log.Error( "No more sprite package slots." );
        return -1;
    }
    PackageStub& stub = m_PackageReg[m_NPackages++];
    strcpy( stub.m_Name, gpName );
    return m_NPackages - 1;
} // SpriteManager::AddPackageStub

/*---------------------------------------------------------------*
/*  Func:    SpriteManager::GetPackageID
/*    Desc:    Allocates ID for package
/*    Parm:    gpName - file name of sprite package, without extension
/*    Ret:    identifier of the sprite package
/*---------------------------------------------------------------*/
int SpriteManager::GetPackageID( const char* gpName )
{
    int gpID = FindPackageStub( gpName );
    if (gpID == -1)
    {
        gpID = AddPackageStub( gpName ); 
    }
    assert( gpID != NO_ELEMENT );
    return gpID;
}//  GetPackageID

const int c_SpriteFileDataLimit = 1024*1024*192;
bool SpriteManager::LoadPackage( int gpID, const char* gpPath )
{
    if (gpID < 0 || gpID >= m_NPackages) return false;
    PackageStub& stub = m_PackageReg[gpID];
    SpritePackage* pSeq = stub.m_pPackage;
    if (!pSeq) 
    {
        chdir( IRM->GetHomeDirectory() );
        if (gpPath == NULL) gpPath = GetPackagePath( gpID );
        pSeq = SpritePackage::CreatePackage( gpPath );
        if (!pSeq) 
        {
            stub.m_bNoFile = true;
            return false;
        }

        //  check if we are breaking our loaded sprite data limit
        m_TotalPackageBytes += pSeq->GetFileSize();
        while (m_TotalPackageBytes > c_SpriteFileDataLimit)
        //  dispose random victim package
        {
            Flush(); 
            SpritePackage* pVictim = NULL;
            do{
                int disposeIdx = rndValue( 0, GetNPackages() );
                pVictim = m_PackageReg[disposeIdx].m_pPackage;
            }
            while(!pVictim);
            m_TotalPackageBytes -= pVictim->GetFileSize();
            pVictim->Cleanup();
        }

        stub.m_pPackage = pSeq;
        pSeq->SetID( gpID );
        //pSeq->m_FileMapping.TouchData();
        pSeq->SetPath( gpPath );
    }
    return true;
} // SpriteManager::LoadPackage

void SpriteManager::UnloadPackage( int gpID )
{
}

void SpriteManager::OnFrame()
{
    Flush();
    GP2Package::OnFrame();

} // SpriteManager::OnFrame

struct RenderBitCmp
{
    bool operator ()( const SpriteRenderBit* l, const SpriteRenderBit* r )
    {
        return (*l < *r);
    }
}; // struct RenderBitCmp

void SpriteManager::DrawBatches()
{
    int             nPasses = IRS->GetNShaderPasses( m_Prim.getShader() );

    for (int cPass = 0; cPass < nPasses; cPass++)
    {
        int             cSurf   = m_SortedRenderBits[0]->m_TexID;
        VertexFormat    cVF     = m_SortedRenderBits[0]->m_VF;
        VertexTnL*      vTnL    = (VertexTnL*)m_Prim.getVertexData();
        VertexTS*       v2t     = (VertexTS*)m_Prim.getVertexData();
        WORD*           pIdx    = m_Prim.getIndices();
        int             cV      = 0;
        int             cI      = 0;
        int             nB      = m_RenderBits.size();

        IRS->SetShader( m_Shader, cPass );
		IRS->SetShaderAutoVars();
        
        for (int i = 0; i < nB; i++)
        {
            const SpriteRenderBit& rb = *m_SortedRenderBits[i];
            for (int j = 0; j < rb.m_NChunks; j++)
            {
                const FrameChunk& chunk = rb.m_pChunk[j];
                int nV          = chunk.m_NVert;
                int nTri        = chunk.m_NTri;

                //  flush batch
                if (rb.m_TexID != cSurf     || 
                    rb.m_VF != cVF              ||
                    cV + nV >= c_MaxVertDrawn)
                {
                    m_Prim.setVertexFormat  ( cVF );
                    m_Prim.setNVert         ( cV );
                    m_Prim.setNInd          ( cI );
                    m_Prim.setNPri          ( cI / 3 );

                    IRS->SetShader( m_Prim.getShader(), cPass );
                    IRS->SetShaderAutoVars();
                    IRS->SetTexture( cSurf, 0 );
                    IRS->SetTexture( cSurf, 1 );

                    DrawPrimBM( m_Prim );
                    
                    cSurf   = rb.m_TexID;
                    cVF     = rb.m_VF;
                    cI      = 0;
                    pIdx    = m_Prim.getIndices();
                    cV      = 0;
                }

                //  fill indices
                int         nIdx = nTri * 3;
                FrameVert*  v    = chunk.m_Vert;
              
                if (chunk.m_Idx)
                {
                    for (int k = 0; k < nIdx; k++) pIdx[k] = chunk.m_Idx[k] + cV;
                }
                else
                {
                    pIdx[0] = cV + 0; pIdx[1] = cV + 1; pIdx[2] = cV + 2;
                    pIdx[3] = cV + 2; pIdx[4] = cV + 1; pIdx[5] = cV + 3;
                }
                pIdx += nIdx;
                cI   += nIdx;

                //  fill vertices
                if (rb.m_VF == vfVertexTnL)
                {
                    VertexTnL* vi = vTnL + cV;
                    FrameVert* fv = v;
					DWORD nc=rb.m_NationalColor&0x00FFFFFF;
					DWORD dc=rb.m_DiffuseColor;
                    for (int i = 0; i < nV; i++, cV++)
                    {
                        Vector3D pt( fv->x, fv->y, fv->z );
                        pt *= rb.m_TM;
                        vi->x = pt.x - c_HalfPixel;
                        vi->y = pt.y - c_HalfPixel;
                        vi->z = pt.z;
                        vi->w = 1.0f;
                        vi->u = fv->u;
                        vi->v = fv->v;
                        vi->diffuse  = dc;
                        vi->specular = nc;
                        vi++;
                        fv++;
                    }
                }
                else
                {
                    VertexTS* vi = v2t + cV;
                    FrameVert* fv = v;
					DWORD nc=rb.m_NationalColor&0x00FFFFFF;
					DWORD dc=rb.m_DiffuseColor;
                    for (int i = 0; i < nV; i++, cV++)
                    {                        
                        Vector3D pt( fv->x, fv->y, fv->z );
                        pt *= rb.m_TM;
                        vi->x = pt.x - c_HalfPixel;
                        vi->y = pt.y - c_HalfPixel;
                        vi->z = pt.z;
                        vi->u = fv->u;
                        vi->v = fv->v;
                        vi->diffuse  = dc;
                        vi->specular = nc;
                        vi++;
                        fv++;
                    }
                }
            }
        }

        //  last batch
        m_Prim.setNVert         ( cV );
        m_Prim.setNInd          ( cI );
        m_Prim.setNPri          ( cI / 3 );
        m_Prim.setVertexFormat  ( cVF );

        m_Prim.setTexture       ( cSurf );
	    m_Prim.setTexture		( cSurf, 1 );

        IRS->SetShader          ( m_Prim.getShader(), cPass );
        IRS->SetShaderAutoVars();
        IRS->SetTexture         ( cSurf, 0, false );
        IRS->SetTexture         ( cSurf, 1, false );            
        DrawPrimBM              ( m_Prim );
    }

} // SpriteManager::DrawBatches

DIALOGS_API void DrawPrimBM_Multipass( BaseMesh& bm );

void SpriteManager::sse_DrawBatches()
{
    //int             nPasses = IRS->GetNShaderPasses( m_Prim.getShader() );

    //for (int cPass = 0; cPass < nPasses; cPass++)
    //{
        int             cSurf   = m_SortedRenderBits[0]->m_TexID;
        VertexFormat    cVF     = m_SortedRenderBits[0]->m_VF;
        VertexTnL*      vTnL    = (VertexTnL*)m_Prim.getVertexData();
        VertexTS*       v2t     = (VertexTS*)m_Prim.getVertexData();
        WORD*           pIdx    = m_Prim.getIndices();
        int             cV      = 0;
        int             cI      = 0;
        int             nB      = m_RenderBits.size();
        IRS->SetShader( m_Shader, 0 );
        IRS->SetShaderAutoVars();

        DWORD vv=DWORD(v2t);

        __prefetch64(vv);
        __prefetch64(vv+64);
        __prefetch64(vv+128);
        __prefetch64(vv+192);
        __prefetch64(vv+256);

        for (int i = 0; i < nB; i++)
        {
            __prefetch64(DWORD(m_SortedRenderBits[i]));            
        }
        for (int i = 0; i < nB; i++)
        {
            const SpriteRenderBit& rb = *m_SortedRenderBits[i];
            for (int j = 0; j < rb.m_NChunks; j++)
            {
                __prefetch64(DWORD(&rb.m_pChunk[j]));
            }
        }
        for (int i = 0; i < nB; i++)
        {
            const SpriteRenderBit& rb = *m_SortedRenderBits[i];
            for (int j = 0; j < rb.m_NChunks; j++)
            {
                const FrameChunk& chunk = rb.m_pChunk[j];
                int nV        = chunk.m_NVert;
                int nTri    = chunk.m_NTri;
                if (nV == 0 || nTri == 0) continue;
                DWORD vs=DWORD(chunk.m_Vert);
                __prefetch64(vs);
                __prefetch64(vs+64);
                __prefetch64(vs+128);
                __prefetch64(vs+192);
            }
        }

        for (int i = 0; i < nB; i++)
        {
            const SpriteRenderBit& rb = *m_SortedRenderBits[i];
            for (int j = 0; j < rb.m_NChunks; j++)
            {
                const FrameChunk& chunk = rb.m_pChunk[j];
                int nV        = chunk.m_NVert;
                int nTri    = chunk.m_NTri;
                if (nV == 0 || nTri == 0) continue;

                //  flush batch
                if (rb.m_TexID != cSurf     || 
                    rb.m_VF != cVF          ||
                    cV + nV >= c_MaxVertDrawn)
                {
                    m_Prim.setVertexFormat  ( cVF );
                    m_Prim.setNVert         ( cV );
                    m_Prim.setNInd          ( cI );
                    m_Prim.setNPri          ( cI / 3 );

                    //IRS->SetShader( m_Prim.getShader(), cPass );
                    IRS->SetTexture( cSurf, 0, false );
                    IRS->SetTexture( cSurf, 1, false );
                    DrawPrimBM_Multipass( m_Prim );

                    cSurf   = rb.m_TexID;
                    cVF     = rb.m_VF;
                    cI      = 0;
                    pIdx    = m_Prim.getIndices();
                    cV      = 0;
                }

                int             nIdx    = nTri * 3;
                FrameVert*      sV      = chunk.m_Vert;
                const Matrix4D* m       = &rb.m_TM;
                
                __declspec(align(16)) static const float PixelBias[4] = { -c_HalfPixel, -c_HalfPixel, 0.0f, 0.0f };
                if (rb.m_VF == vfVertexTS)
                {
                    void*       dV    = &v2t[cV];
                    __declspec(align(16)) static DWORD       CurDiffuse2t[4] = {0x00000000,0x00000000,0x00000000,0x00000000};
                    __declspec(align(16)) static const DWORD CurDifMask2t[4] = {0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF};

                    //__declspec(align(16)) static DWORD       CurSpecular2t[4] = {0x00000000,0x00000000,0x00000000,0x00000000};
                    //__declspec(align(16)) static const DWORD CurSpecMask2t[4] = {0x00000000, 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};

                    CurDiffuse2t[0] = rb.m_DiffuseColor;
                    CurDiffuse2t[1] = (rb.m_NationalColor & 0xFFFFFF);
                    __asm{
                        mov             ecx,    DWORD PTR [nV]
                        mov             ebx,    DWORD PTR [m]
                        mov             esi,    DWORD PTR [sV]
                        mov             edi,    DWORD PTR [dV]
                        movups          xmm0,   XMMWORD PTR [ebx]Matrix4D.e00
                        movups          xmm1,   XMMWORD PTR [ebx]Matrix4D.e10
                        movups          xmm2,   XMMWORD PTR [ebx]Matrix4D.e20
                        movups          xmm3,   XMMWORD PTR [ebx]Matrix4D.e30
                        xorps           xmm7,   xmm7

                    _man_loop_1:
                        movss           xmm4,   DWORD PTR  [esi]FrameVert.x
                        movss           xmm5,   DWORD PTR  [esi]FrameVert.y
                        movss           xmm6,   DWORD PTR  [esi]FrameVert.z
                        movlps          xmm7,   MMWORD PTR [esi]FrameVert.u
                        shufps          xmm7,   xmm7, 64

                        //  perform matrix multiplication
                        shufps          xmm4,   xmm4, 0
                        shufps          xmm5,   xmm5, 0
                        shufps          xmm6,   xmm6, 0
                        mulps           xmm4,   xmm0
                        mulps           xmm5,   xmm1
                        mulps           xmm6,   xmm2
                        addps           xmm4,   xmm3
                        addps           xmm5,   xmm6
                        addps           xmm4,   xmm5
                        //  adding pixel bias
                        addps           xmm4,   XMMWORD PTR [PixelBias]

                        //  diffuse color component
                        //andps           xmm4,   XMMWORD PTR [CurDifMask2t]
                        //orps            xmm4,   XMMWORD PTR [CurDiffuse2t]
                        
                        //  specular color component
                        andps           xmm7,   XMMWORD PTR [CurDifMask2t]
                        orps            xmm7,   XMMWORD PTR [CurDiffuse2t]
                        
                        //  move vertex contents from registers to memory
                        movntps         XMMWORD PTR [edi]VertexTS.x,xmm4
                        movntps         XMMWORD PTR [edi]VertexTS.diffuse,xmm7
                        
                        //  move to next vertex
                        add             esi,    TYPE FrameVert
                        add             edi,    TYPE VertexTS;
                        prefetchnta     BYTE PTR [edi+512]
                        dec             ecx
                        jnz             _man_loop_1

                        sfence
                    }
                }
                else
                {
                    void*       dV    = &vTnL[cV];
                    __declspec(align(16)) static DWORD       CurDiffuseTnL [4] = {0xFFFFFFFF, 0x00000000,0x00000000,0x00000000};
                    __declspec(align(16)) static const DWORD CurDifMaskTnL [4] = {0x00000000, 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
                    __declspec(align(16)) static DWORD       CurSpecularTnL[4] = {0x00000000, 0x00FFFFFF,0x00000000,0x00000000};
                    __declspec(align(16)) static const DWORD CurSpecMaskTnL[4] = {0xFFFFFFFF, 0x00000000,0xFFFFFFFF,0xFFFFFFFF};

                    __declspec(align(16)) static DWORD       WValue[4] = {0x00000000,0x00000000,0x00000000,0x3f800000};
                    __declspec(align(16)) static const DWORD WMask[4]  = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000};

                    CurDiffuseTnL[0] = rb.m_DiffuseColor;

                    __asm{
                        mov          ecx,    DWORD PTR [nV]
                        mov          ebx,    DWORD PTR [m]
                        mov          esi,    DWORD PTR [sV]
                        mov          edi,    DWORD PTR [dV]
                        movups       xmm0,   XMMWORD PTR [ebx]Matrix4D.e00
                        movups       xmm1,   XMMWORD PTR [ebx]Matrix4D.e10
                        movups       xmm2,   XMMWORD PTR [ebx]Matrix4D.e20
                        movups       xmm3,   XMMWORD PTR [ebx]Matrix4D.e30
                        xorps        xmm7,   xmm7

                    _man_loop_2:
                        movss        xmm4,   DWORD PTR  [esi]FrameVert.x
                        movss        xmm5,   DWORD PTR  [esi]FrameVert.y
                        movss        xmm6,   DWORD PTR  [esi]FrameVert.z
                        movlps       xmm7,   MMWORD PTR [esi]FrameVert.u
                        shufps       xmm7,   xmm7,64

                        shufps       xmm4,   xmm4,0
                        shufps       xmm5,   xmm5,0
                        shufps       xmm6,   xmm6,0
                        mulps        xmm4,   xmm0
                        mulps        xmm5,   xmm1
                        mulps        xmm6,   xmm2
                        addps        xmm4,   xmm3
                        addps        xmm5,   xmm6
                        addps        xmm4,   xmm5
                        //  pixel bias
                        addps        xmm4,   XMMWORD PTR [PixelBias]
                        
                        //  diffuse color component
                        andps        xmm7,   XMMWORD PTR [CurDifMaskTnL]
                        orps         xmm7,   XMMWORD PTR [CurDiffuseTnL]

                        //  specular color component
                        andps        xmm7,   XMMWORD PTR [CurSpecMaskTnL]
                        orps         xmm7,   XMMWORD PTR [CurSpecularTnL]

                        //  RHW component
                        andps        xmm4,   XMMWORD PTR [WMask ]
                        orps         xmm4,   XMMWORD PTR [WValue]
                        
                        
                        movntps      XMMWORD PTR [edi]VertexTnL.x,xmm4
                        movntps      XMMWORD PTR [edi]VertexTnL.diffuse,xmm7
                        
                        add          esi,    TYPE FrameVert
                        add          edi,    TYPE VertexTnL;
                        dec          ecx
                        jnz          _man_loop_2
                        sfence
                    }
                }
                
                //  fill indices
                if (chunk.m_Idx)
                {
                    __asm{
                            mov         ecx,    DWORD PTR [nIdx]
                            mov         ebx,    DWORD PTR [chunk]
                            mov         esi,    [ebx]chunk.m_Idx
                            mov         edi,    DWORD PTR [pIdx]
                            mov         ebx,    DWORD PTR [cV]
                            mov         edx,    ecx
                            shr         edx,    2
                            jz          _sm_loop_
                            movd        mm0,    ebx
                            pshufw      mm0,    mm0,0
                    _nw_loop_:
                            movq        mm2,    MMWORD PTR [esi]
                            paddusw     mm2,    mm0
                            movq        MMWORD PTR [edi],mm2
                            add         esi,    8
                            add         edi,    8
                            dec         edx
                            jnz         _nw_loop_
                            and         ecx,    11b
                            jz          _exit_ok_
                    _sm_loop_:
                            mov         ax,     WORD PTR [esi]
                            add         ax,     bx
                            mov         WORD PTR [edi],ax
                            add         esi,    2
                            add         edi,    2
                            dec         ecx
                            jnz         _sm_loop_
                    _exit_ok_:
                            emms
                    }
                }
                else
                {
                    pIdx[0] = cV + 0; pIdx[1] = cV + 1; pIdx[2] = cV + 2;
                    pIdx[3] = cV + 2; pIdx[4] = cV + 1; pIdx[5] = cV + 3;
                }
                pIdx += nIdx;
                cI   += nIdx;
                cV   += nV;
            }
        }

        //  last batch
        m_Prim.setNVert         ( cV );
        m_Prim.setNInd          ( cI );
        m_Prim.setNPri          ( cI / 3 );
        m_Prim.setVertexFormat  ( cVF );

        m_Prim.setTexture       ( cSurf );
        m_Prim.setTexture		( cSurf, 1 );

        IRS->SetTexture         ( cSurf, 0, false );
        IRS->SetTexture         ( cSurf, 1, false );            
        DrawPrimBM_Multipass    ( m_Prim );
    //}
} // SpriteManager::sse_DrawBatches


void SpriteManager::Flush( bool bResetWorldTM )
{
    int nB = m_RenderBits.size();
    if (nB == 0) return;

    m_Prim.setShader( m_Shader );
    IRS->SetShaderAutoVars();
    if (bResetWorldTM) 
    {
        const Matrix4D& tm = IRS->GetWorldTM();
        if (tm.e00 != 1.0f || tm.e11 != 1.0f) IRS->ResetWorldTM();
    }
    //  sort render bits by sprite surface
    std::sort( m_SortedRenderBits.begin(), m_SortedRenderBits.end(), RenderBitCmp() );
    /*if (m_bUseSSE) sse_DrawBatches(); else */sse_DrawBatches();
    
    m_RenderBits.clear();
    m_SortedRenderBits.clear();
} // SpriteManager::Flush


/*---------------------------------------------------------------------------*/
/*    Func:    SpriteManager::DrawSprite
/*    Desc:    Generic sprite drawing function
/*    Parm:    gpID        - package ID
/*            sprID        - sprite ID
/*            x, y, z        - position in screen space
/*---------------------------------------------------------------------------*/
bool SpriteManager::DrawSprite(    int    gpID, int sprID, const Vector3D& pos, bool mirrored, DWORD color )
{
    Matrix4D tm;
    tm.e00 = m_CurScale;
    tm.e01 = 0.0f;
    tm.e02 = 0.0f;
    tm.e03 = 0.0f;

    tm.e10 = 0.0f;
    tm.e11 = m_CurScale;
    tm.e12 = 0.0f;
    tm.e13 = 0.0f;

    tm.e20 = 0.0f;
    tm.e21 = 0.0f;
    tm.e22 = m_CurScale;
    tm.e23 = 0.0f;
    
    tm.e30 = mirrored ? -pos.x : pos.x;
    tm.e31 = pos.y;
    tm.e32 = pos.z > m_ZBias ? pos.z - m_ZBias : pos.z;
    tm.e33 = 1.0f;
    return DrawSprite( gpID, sprID, tm, color );
} // SpriteManager::DrawSprite

/*---------------------------------------------------------------------------*/
/*    Func:    SpriteManager::DrawSprite
/*    Desc:    Draws post-transformed in screen space sprite
/*    Parm:    gpID        - package ID
/*            sprID        - sprite ID
/*            transf        - screen-space transform matrix 
/*---------------------------------------------------------------------------*/
bool SpriteManager::DrawSprite( int gpID, int sprID, const Matrix4D& transf, DWORD color )
{
    FrameInstance* frameInst = GetFrameInstance( gpID, sprID, color, m_CurLOD );
    if (frameInst == NULL) 
    {
        return false;
    }

    int    nChunks     = frameInst->GetNChunks(); 
    int curSurf  = -1;
    int cNChunks = 0;
    SpriteRenderBit* rb = NULL;
    
    if (m_RenderBits.size() + nChunks >= c_MaxSpritesDrawn) Flush();

    for (int i = 0; i < nChunks; i++)
    {
        const FrameChunk& chunk = frameInst->GetChunk( i );
        //  fill in render bit 
        if (chunk.m_SurfaceID != curSurf)
        {
            if (rb) rb->m_NChunks = cNChunks;
            rb = &m_RenderBits.expand();
            m_SortedRenderBits.push_back( rb );
            rb->m_pChunk        = &chunk;
            rb->m_DiffuseColor  = m_CurDiffuse;
            rb->m_NationalColor = color;
            rb->m_TexID         = chunk.m_TextureID != 0xFFFF ? 
                                                chunk.m_TextureID : 
                                                m_Surface[chunk.m_SurfaceID].m_TexID;
            rb->m_TM            = transf;
            rb->m_VF            = vfVertexTnL;

            curSurf             = chunk.m_SurfaceID;
            cNChunks            = 0;
        }
        cNChunks++;
    }
    if (rb) rb->m_NChunks = cNChunks;
    return true;
} // SpriteManager::DrawSprite

/*---------------------------------------------------------------------------*/
/*    Func:    SpriteManager::DrawWSprite
/*    Desc:    Draws post-transformed in screen space sprite
/*    Parm:    gpID        - package ID
/*            sprID        - sprite ID
/*            transf        - screen-space transform matrix 
/*---------------------------------------------------------------------------*/
bool SpriteManager::DrawWSprite( int gpID, int sprID, const Matrix4D& m, DWORD color )
{
    FrameInstance* frameInst = GetFrameInstance( gpID, sprID, color, m_CurLOD );
    if (frameInst == NULL) 
    {
        return false;
    }

    int    nChunks     = frameInst->GetNChunks(); 
    int curSurf  = -1;
    int cNChunks = 0;
    SpriteRenderBit* rb = NULL;

    if (m_RenderBits.size() + nChunks >= c_MaxSpritesDrawn) Flush();

    for (int i = 0; i < nChunks; i++)
    {
        const FrameChunk& chunk = frameInst->GetChunk( i );
        //  fill in render bit 
        if (chunk.m_SurfaceID != curSurf)
        {
            if (rb) rb->m_NChunks = cNChunks;
            rb = &m_RenderBits.expand();
            m_SortedRenderBits.push_back( rb );
            rb->m_pChunk        = &chunk;
            rb->m_DiffuseColor  = m_CurDiffuse;
            rb->m_NationalColor = color;
            rb->m_TexID         = chunk.m_TextureID != 0xFFFF ? 
                                                chunk.m_TextureID : 
                                                m_Surface[chunk.m_SurfaceID].m_TexID;
            rb->m_TM            = m;
            rb->m_VF            = vfVertexTS;

            curSurf             = chunk.m_SurfaceID;
            cNChunks            = 0;
        }
        cNChunks++;
    }
    if (rb) rb->m_NChunks = cNChunks;
    return true;
} // SpriteManager::DrawWSprite

/*---------------------------------------------------------------------------*/
/*    Func:    SpriteManager::DrawWSprite
/*    Desc:    Generic sprite drawing function
/*    Parm:    gpID        - package ID
/*            sprID        - sprite ID
/*            x, y, z        - position in screen space
/*---------------------------------------------------------------------------*/
bool SpriteManager::DrawWSprite( int gpID, int sprID, const Vector3D& pos, bool mirrored, DWORD color )
{
    Matrix4D tm;
    tm.e00 = m_CurScale;
    tm.e01 = 0.0f;
    tm.e02 = 0.0f;
    tm.e03 = 0.0f;

    tm.e10 = 0.0f;
    tm.e11 = m_CurScale;
    tm.e12 = 0.0f;
    tm.e13 = 0.0f;

    tm.e20 = 0.0f;
    tm.e21 = 0.0f;
    tm.e22 = m_CurScale;
    tm.e23 = 0.0f;

    tm.e30 = mirrored ? -pos.x : pos.x;
    tm.e31 = pos.y;
    tm.e32 = pos.z > m_ZBias ? pos.z - m_ZBias : pos.z;
    tm.e33 = 1.0f;
    return DrawWSprite( gpID, sprID, tm, color );
} // SpriteManager::DrawWSprite

/*---------------------------------------------------------------------------*/
/*    Func:    SpriteManager::DrawWChunk
/*    Desc:    Adds to GP queue single chunk from big complex sprite, 
/*                which is pulled onto its own geometry
/*    Parm:    gpID, sprID, chunkID - package, frame, chunk index
/*            geom - mesh with sprite geometry
/*            cPoly, nPoly - start polygon and number of polygons to draw
/*            cVert, nVert - start vertices and number of vertices to draw
/*    Ret:    true, when things go clear...well, nevermind
/*---------------------------------------------------------------------------*/
bool SpriteManager::DrawWChunk( int gpID, int sprID, int chunkID, 
                                const Vector3D& pos, BaseMesh& geom, 
                                int cPoly, int nPoly, 
                                int cVert, int nVert, 
                                DWORD color )
{
    FrameInstance* frameInst = GetFrameInstance( gpID, sprID, color, m_CurLOD );
    if (frameInst == NULL) return false;
    FrameChunk&    chunk    = frameInst->GetChunk( chunkID );
    if (chunk.m_NVert == 4)  //  HACK
    {
        chunk.m_NVert = nVert;
        chunk.m_NTri  = nPoly;
        VertexTS* pV  = ((VertexTS*)geom.getVertexData()) + cVert;
        WORD* pI      = geom.getIndices() + cPoly*3; 
        chunk.m_Idx   = (WORD*)m_InstanceAllocator.Allocate( nPoly*3*sizeof( WORD ) );
        FrameVert* nv = (FrameVert*)m_InstanceAllocator.Allocate( nVert*sizeof( FrameVert ) );
        for (int i = 0; i < nPoly; i++)
        {
            chunk.m_Idx[i*3 + 0] = pI[i*3 + 0] - cVert;
            chunk.m_Idx[i*3 + 1] = pI[i*3 + 1] - cVert;
            chunk.m_Idx[i*3 + 2] = pI[i*3 + 2] - cVert;
        }

        FrameVert* sv = chunk.m_Vert;
        for (int i = 0; i < nVert; i++)
        {
            FrameVert& v = nv[i]; 
            v.x     = pV[i].x;
            v.y     = pV[i].y;
            v.z     = pV[i].z;
            v.color = pV[i].diffuse;
            v.u     = pV[i].u*(sv[3].u - sv[0].u) + sv[0].u;
            v.v     = pV[i].v*(sv[3].v - sv[0].v) + sv[0].v;
        }
        chunk.m_Vert = nv;
    }
    
    SpriteRenderBit* rb = &m_RenderBits.expand();
    rb->m_NChunks       = 1;
    rb->m_pChunk        = &chunk;
    rb->m_DiffuseColor  = m_CurDiffuse;
    rb->m_NationalColor = color;
    rb->m_TexID         = chunk.m_TextureID != 0xFFFF ? 
                                       chunk.m_TextureID : 
                                       m_Surface[chunk.m_SurfaceID].m_TexID;
    rb->m_TM            = IRS->GetWorldTM();
    rb->m_VF            = vfVertexTS;
    m_SortedRenderBits.push_back( rb );
    return true;    
} // SpriteManager::DrawWChunk

bool SpriteManager::DrawWSprite( int gpID, int sprID, const Vector3D& pos, BaseMesh& geom, DWORD color )
{
    FrameInstance* frameInst = GetFrameInstance( gpID, sprID, color, m_CurLOD );
    if (frameInst == NULL) return false;

    int        nChunks    = frameInst->GetNChunks(); 
    BYTE*    pVert    = geom.getVertexData();
    int        stride    = geom.getVertexStride();

    /*for (int i = 0; i < nChunks; i++)
    {
        const FrameChunk&    chunk    = frameInst->GetChunk( i );
        SpriteBucket&    bucket    = m_Bucket[chunk.m_SurfaceID];
        bucket.SetCurVF        ( vfVertexTS );
        bucket.AddQuad        ( chunk, pos, pVert, m_CurDiffuse );
        if (bucket.IsEmpty()) 
        {
            bucket.SetEmpty( false );
            m_DrawnBuckets[m_NDrawnBuckets++] = bucket.m_Index;
        }
        pVert += stride * 4;
    }*/

    return true;    
} // SpriteManager::DrawWSprite

bool SpriteManager::GetAABB( int gpID, AABoundBox& aabb ) const
{
    SpritePackage* pPackage = GetPackage( gpID );
    if (!pPackage) return false;
    aabb = pPackage->GetAABB();
    return true;
} // SpriteManager::GetAABBl

bool SpriteManager::HasDepthData( int gpID ) const
{
   SpritePackage* pPackage = GetPackage( gpID );
   if (!pPackage) return false;
   return pPackage->HasDepthData();
} // SpriteManager::HasDepthData

bool SpriteManager::HasColorData( int gpID ) const
{
    SpritePackage* pPackage = GetPackage( gpID );
    if (!pPackage) return false;
    return pPackage->HasColorData();
} // SpriteManager::HasColorData

/*---------------------------------------------------------------------------*/
/*    Func:    SpriteManager::DrawNSprite
/*    Desc:    Draws post-transformed in screen space sprite, with normalized color
/*    Parm:    gpID        - package ID
/*            sprID        - sprite ID
/*            transf        - screen-space transform matrix 
/*---------------------------------------------------------------------------*/
bool SpriteManager::DrawNSprite( int gpID, int sprID, const Matrix4D& transf, DWORD color )
{
    FrameInstance* frameInst = GetFrameInstance( gpID, sprID, color, m_CurLOD );
    if (frameInst == NULL) 
    {
        return false;
    }

    //  move chunks to the batch 
    int    nChunks    = frameInst->GetNChunks(); 
    int    numV    = nChunks * 4;

   /* for (int i = 0; i < nChunks; i++)
    {
        FrameChunk&        chunk    = frameInst->GetChunk( i );
        SpriteBucket&    bucket    = m_Bucket[chunk.m_SurfaceID];
        bucket.SetCurVF            ( vfVertexTnL );
        bucket.AddFrameChunkSN    ( chunk, transf );
        if (bucket.IsEmpty()) 
        {
            bucket.SetEmpty( false );
            m_DrawnBuckets[m_NDrawnBuckets++] = bucket.m_Index;
        }
    }*/
    return true;
} // SpriteManager::DrawNSprite

/*---------------------------------------------------------------------------*/
/*    Func:    SpriteManager::DrawNWSprite
/*    Desc:    Draws post-transformed in world space sprite, with normalized color
/*    Parm:    gpID        - package ID
/*            sprID        - sprite ID
/*            transf        - screen-space transform matrix 
/*---------------------------------------------------------------------------*/
bool SpriteManager::DrawNWSprite( int gpID, int sprID, const Matrix4D& transf, DWORD color )
{
    FrameInstance* frameInst = GetFrameInstance( gpID, sprID, color, m_CurLOD );
    if (frameInst == NULL) 
    {
        return false;
    }

    //  move chunks to the batch 
    int    nChunks    = frameInst->GetNChunks(); 
   /* for (int i = 0; i < nChunks; i++)
    {
        const FrameChunk&    chunk    = frameInst->GetChunk( i );
        SpriteBucket&        bucket    = m_Bucket[chunk.m_SurfaceID];
        bucket.SetCurVF         ( vfVertexTS );
        bucket.AddFrameChunkN( chunk, transf );
        if (bucket.IsEmpty()) 
        {
            bucket.SetEmpty( false );
            m_DrawnBuckets[m_NDrawnBuckets++] = bucket.m_Index;
        }
    }*/
    return true;
} // SpriteManager::DrawNWSprite

int    SpriteManager::GetNQuads( int gpID, int sprID )
{
    FrameInstance* frameInst = GetFrameInstance( gpID, sprID, 0x00000000 );
    if (!frameInst) return 0;
    return frameInst->GetNChunks();
} // SpriteManager::GetNQuads

bool SpriteManager::GetQuadRect( int gpID, int sprID, int quadID, Rct& rct )
{
    /*FrameInstance* frameInst = GetFrameInstance( gpID, sprID, 0x00000000 );
    if (!frameInst) return false;
    const FrameChunk& chunk = frameInst->GetChunk( quadID );
    rct.x = chunk.x;
    rct.y = chunk.y;
    rct.w = chunk.side;
    rct.h = chunk.side;*/
    return true;
} // SpriteManager::GetQuadRect

/*****************************************************************
/*  SpriteManager implementation                                         
/*****************************************************************/
void SpriteManager::Drop( FrameInstance* frInst )
{
    int id = m_FrameReg.find( frInst->GetKey() );
    if (id != NO_ELEMENT)
    {
        m_FrameReg.entry( id ).el = NULL;
    }
}

void SpriteManager::Precache( int gpID )
{
    SpritePackage* pPackage = GetPackage( gpID );
    if (!pPackage) 
    {
        LoadPackage( gpID );
        pPackage = GetPackage( gpID );
    }  
    if (!pPackage) return;
    pPackage->Precache();
} // SpriteManager::Precache

int SpriteManager::GetFrameHeight( int gpID, int sprID )
{
    SpritePackage* pPackage = GetPackage( gpID );
    if (!pPackage) 
    {
        PackageStub& stub = m_PackageReg[gpID];
        if (stub.m_bNoFile) return NULL;
        LoadPackage( gpID );
        pPackage = GetPackage( gpID );
    }
    if (!pPackage) return 0.0f;
    if (sprID < -2 || sprID >= pPackage->GetNFrames()) return 0.0f;
    return pPackage->GetFrameHeight( sprID );
} // SpriteManager::GetFrameHeight

int SpriteManager::GetFrameWidth( int gpID, int sprID )
{
    SpritePackage* pPackage = GetPackage( gpID );
    if (!pPackage) 
    {
        PackageStub& stub = m_PackageReg[gpID];
        if (stub.m_bNoFile) return NULL;
        LoadPackage( gpID );
        pPackage = GetPackage( gpID );
    }
    if (!pPackage) return 0.0f;
    if (sprID < -2 || sprID >= pPackage->GetNFrames()) return 0.0f;
    return pPackage->GetFrameWidth( sprID );
} // SpriteManager::GetFrameWidth

bool SpriteManager::GetBoundFrame( int gpID, int sprID, Rct& frame, DWORD color )
{
    const FrameInstance* frInst = GetFrameInstance( gpID, sprID, color );
    if (!frInst) return false;
    frame = frInst->GetBounds();
    return true;
} // SpriteManager::GetBoundFrame

bool SpriteManager::GetAABB( int gpID, int sprID, AABoundBox& aabb, DWORD color )
{
    const FrameInstance* frInst = GetFrameInstance( gpID, sprID, color );
    if (!frInst) return false;
    aabb = frInst->GetAABB();
    return true;
} // SpriteManager::GetAABB

void SpriteManager::SetPackagePath( int gpID, const char* gpPath )
{
    if (gpID < 0 || gpID >= m_NPackages) 
    {
        Log.Warning( "Incorrect GPID in SpriteManager::SetGPPath, gpID:%d, gpPath:<%s>",
            gpID, gpPath );
        return;
    }
    PackageStub& stub = m_PackageReg[gpID];
    strcpy( stub.m_Path, gpPath );
} // SpriteManager::SetPath

float SpriteManager::SetScale( float scale )
{
    float old = m_CurScale;
    m_CurScale = scale;

    if (m_CurScale > 0.5f)
    {
        m_CurLOD = 0;
    }
    else if (m_CurScale > 0.25f)
    {
        m_CurLOD = 1;
    }
    else if (m_CurScale > 0.125f)
    {
        m_CurLOD = 2;
    }
    else
    {
        m_CurLOD = 3;
    }

    return scale;
} // SpriteManager::SetScale

void SpriteManager::EnableClipping( bool enable )
{
    m_bClippingEnabled = enable;
}

bool SpriteManager::SetClipArea( const Rct& rct )
{
    if (m_ClipArea.x == rct.x && m_ClipArea.y == rct.y &&
        m_ClipArea.w == rct.w && m_ClipArea.h == rct.h)
    {
        return false;
    }
    m_ClipArea.x    = rct.x;
    m_ClipArea.y    = rct.y;
    m_ClipArea.w    = rct.w;
    m_ClipArea.h    = rct.h;

    //  viewport is changed, so we need to flush all batched stuff here
    Flush();
    if (IRS) IRS->SetViewPort( m_ClipArea );
    return true;
} // SpriteManager::SetClipArea

void SpriteManager::Render()
{
    OnFrame();
}

void SpriteManager::OnDestroyRS()
{
    Purge();
} // SpriteManager::OnDestroyRenderSystem

DWORD SpriteManager::GetStatistics( SpriteStatistics stat ) const
{
    if (stat == ssMappedSize)
    {
        DWORD res = 0;
        for (int i = 0; i < m_NPackages; i++)
        {
            const PackageStub& stub = m_PackageReg[i];
            SpritePackage* pPackage = stub.m_pPackage;
            if (!pPackage) continue;
            res += pPackage->GetFileSize();
        }
        return res;
    }
    else if (stat == ssFrameInstances)
    {
    
    }
    else if (stat == ssFrameInstMemory)
    {
    
    }
    return 0;
} // SpriteManager::GetStatistics

BYTE* SpriteManager::LockSurfRect( int surfID, const Rct& rect, int& pitch )
{
    int texID       = m_Surface[surfID].m_TexID;
    m_LockSurfID    = surfID;
    m_LockRect      = rect;
    return IRS->LockTexBits( m_CanvasTex, rect, pitch );
} // SpriteManager::LockSurfRect

void SpriteManager::UnlockSurfRect()
{
    if (m_LockSurfID < 0) return;
    int texID = m_Surface[m_LockSurfID].m_TexID;
    IRS->UnlockTexBits( m_CanvasTex );
    IRS->CopyTexture( texID, m_CanvasTex, &m_LockRect );
    m_LockSurfID = -1;
} // SpriteManager::UnlockSurfRect


