/*****************************************************************************/
/*    File:    vWater.cpp
/*    Desc:    Water interface implementation
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "IWater.h"
#include "ITerrain.h"
#include "kHash.hpp"
#include "IMediaManager.h"
#include "IShadowManager.h"

//  global water interface pointer
IWaterscape*        IWater;

struct WaterQuad
{
    DWORD           m_IBStamp;
    DWORD           m_VBStamp;

    int             m_IBPos;
    int             m_VBPos;

    bool            m_bNoWater;
            
    WaterQuad() { Free(); }
    void    Free()
    {
        m_IBStamp   = 0;
        m_VBStamp   = 0;
        m_bNoWater  = false;
    }
}; // struct WaterQuad

const int c_WaterIBufferBytes = 1024*1024*1;
const int c_WaterVBufferBytes = 1024*1024*8;
const int c_WaterHQuads = 128;
const int c_WaterWQuads = 64;
const int c_WaterGridResolution = 8;

/*****************************************************************************/
/*  Class:  Waterscape
/*  Desc:   Implementation of the water interface
/*****************************************************************************/
class Waterscape : public IWaterscape
{
    typedef Vertex2t             WaterVertex;

    int                         m_IBID;
    int                         m_VBID;
    int                         m_Shader;
    VertexDeclaration           m_VDecl;

    BaseMesh                    m_Grid;
    BaseMesh                    m_BaseGrid;
    WaterQuad                   m_Quads[c_WaterHQuads*c_WaterWQuads];

    WaterColorCB                GetWaterColor;
	
	bool						m_OrthoModeIsEnabled;
	float						m_Waterline;
	float						m_BoilingCoef;
public:
                                Waterscape      ();
    virtual void                Render          ();
	virtual void				Reset			();
    virtual void                Refresh         ( const Rct& area );
	virtual void				OrthoModeEnable();
	virtual void				OrthoModeDisable();

    virtual void                SetWaterColorCB ( WaterColorCB cb ) { GetWaterColor = cb; }
    virtual void                SetShaderQuality( int Level );

	virtual void				SetWaterline(const float Waterline);
	virtual float				GetWaterline() const { return m_Waterline; }
	
	virtual const Rct			CalcVisibleWaterRect() const;
	
	virtual void				SetBoilingCoef(const float BoilingCoef) { m_BoilingCoef = BoilingCoef; }

protected:
   
}; // class Waterscape
Waterscape      g_Water;

//-----------------------------------------------------------------------------
// Waterscape::SetWaterline
//-----------------------------------------------------------------------------
void Waterscape::SetWaterline(const float Waterline) {
	m_Waterline = Waterline;
	IRMap->SetReflectionPlane(Plane(Vector3D(0.0f, 0.0f, GetWaterline()), Vector3D::oZ));
} // Waterscape::SetWaterline

const float c_WaterDepthScale = 2.0f;
DWORD DefGetWaterColor( float x, float y )
{
    float h = ITerra->GetH( x, y );
    DWORD clrWater = 0x002233BB;
    h = -h*c_WaterDepthScale;
    clamp( h, 0.0f, 255.0f );
    DWORD alpha = h;
    return clrWater | (alpha<<24);
} // DefGetWaterColor

/*****************************************************************************/
/*  Waterscape  implementation
/*****************************************************************************/
Waterscape::Waterscape()
{
    IWater              = this;
    m_IBID              = -1;
    m_VBID              = -1;
    GetWaterColor       = DefGetWaterColor;
    m_VDecl = CreateVertexDeclaration( WaterVertex::format() );

    CreatePatchGrid<WaterVertex>( m_BaseGrid, Rct( 0.0f, 0.0f, 1.0f, 1.0f ), 
                                    c_WaterGridResolution - 1, 
                                    c_WaterGridResolution - 1 );   
    m_Grid = m_BaseGrid;
	m_OrthoModeIsEnabled = false;
	m_Waterline = 0.0f;
} // Waterscape::Waterscape

void Waterscape::OrthoModeEnable() {
	m_OrthoModeIsEnabled = true;
}

void Waterscape::OrthoModeDisable() {
	m_OrthoModeIsEnabled = false;
}

void Waterscape::Reset()
{
	for (int i = 0; i < c_WaterWQuads*c_WaterHQuads; i++)
	{
		m_Quads[i].Free();
	}	
} // Waterscape::Reset

void Waterscape::Refresh( const Rct& area )
{
    Rct ext = ITerra->GetExtents();
    
    float qw = ext.w/float( c_WaterWQuads );
    float qh = ext.h/float( c_WaterHQuads );

    int qBegX = tmax<int>( 0, (area.x - ext.x - qw)/qw );
    int qBegY = tmax<int>( 0, (area.y - ext.y - qh)/qh );  
    int qEndX = tmin<int>( (area.r() - ext.x + qw)/qw, c_WaterWQuads - 1);
    int qEndY = tmin<int>( (area.b() - ext.y + qh)/qh, c_WaterHQuads - 1);

    for (int j = qBegY; j <= qEndY; j++)
    {
        for (int i = qBegX; i <= qEndX; i++)
        {
            int qIdx = i + j*c_WaterWQuads;
            if (qIdx >= c_WaterWQuads*c_WaterHQuads) continue;
            WaterQuad& wq = m_Quads[qIdx];
            wq.Free();
        }
    }
} // Waterscape::Refresh

//-----------------------------------------------------------------------------
// Waterscape::CalcVisibleWaterRect
//-----------------------------------------------------------------------------
const Rct Waterscape::CalcVisibleWaterRect() const {
	ICamera* pCam = GetCamera();
	Rct rc(0.0f, 0.0f, 0.0f, 0.0f);
	if(!pCam) {
		return rc;
	}
	Frustum Fr = pCam->GetFrustum();
	Line3D Dirs[] = { Line3D(Fr.ltn(), Fr.ltf() - Fr.ltn()), // Left Top
		Line3D(Fr.rtn(), Fr.rtf() - Fr.rtn()), // Right Top
		Line3D(Fr.lbn(), Fr.lbf() - Fr.lbn()), // Left Bottom
		Line3D(Fr.rbn(), Fr.rbf() - Fr.rbn()) }; // Right Bottom
	Plane WaterPlane(Vector3D(0.0f, 0.0f, GetWaterline()), Vector3D::oZ);
	int NInters = 0;
	Vector3D Inters[4];
	for(int nDir = 0; nDir < 4; nDir++) {
		Dirs[nDir].Normalize();
		if(Dirs[nDir].IntersectPlane(WaterPlane, Inters[NInters])) {
			if(Dirs[nDir].getDir().dot(Inters[NInters]-pCam->GetPosition())>0)
				NInters++;
		}
	}
	if(NInters<4)return Rct(0,0,16384,16384);
	float xMin = FLT_MAX, yMin = FLT_MAX, xMax = -FLT_MAX, yMax = -FLT_MAX;
	for(int i = 0; i < NInters; i++) {
		xMin = min(xMin, Inters[i].x);
		yMin = min(yMin, Inters[i].y);
		xMax = max(xMax, Inters[i].x);
		yMax = max(yMax, Inters[i].y);
	}
	rc.Set(xMin, yMin, xMax - xMin, yMax - yMin);
	if(rc.w < 0.0f) {
		rc.w = 0.0f;
	}
	if(rc.h < 0.0f) {
		rc.h = 0.0f;
	}
	return rc;
} // Waterscape::CalcVisibleWaterRect

const float c_MaxWaterDistance = 100000.0f;
void Waterscape::Render()
{
    IRS->Flush();
    //static int noiseTex = IRS->GetTextureID( "perlinvolume.dds" );
	static int noiseTex = IRS->GetTextureID( "simplenoise3d.dds" );

//	static int idWaterShPersp = IRS->GetShaderID("ocean");
//	static int idWaterShPerspLo = IRS->GetShaderID("low\\ocean");
	static int idWaterShOrtho = IRS->GetShaderID("ocean_ortho");

	// Waterline:
/*	static int idWaterline = IRS->GetShaderVarID(idWaterShPersp, "Waterline");
	IRS->SetShaderVar(idWaterShPersp, idWaterline, m_Waterline);
	static int idWaterlineLo = IRS->GetShaderVarID(idWaterShPerspLo, "Waterline");
	IRS->SetShaderVar(idWaterShPerspLo, idWaterlineLo, m_Waterline);
	// Boiling:
	static int idBoilingCoef = IRS->GetShaderVarID(idWaterShPersp, "BoilingCoef");
	IRS->SetShaderVar(idWaterShPersp, idBoilingCoef, m_BoilingCoef);
	static int idBoilingCoefLo = IRS->GetShaderVarID(idWaterShPerspLo, "BoilingCoef");
	IRS->SetShaderVar(idWaterShPerspLo, idBoilingCoefLo, m_BoilingCoef);*/

    int reflTex = IRMap->GetReflectionTextureID();
    const Matrix4D& reflTM = IRMap->GetReflectionTexTM(); 

    int shTex = IShadowMgr->GetShadowMapID();
    const Matrix4D& shTM = IShadowMgr->CalcShadowMapTM(); 

	// Calcing intersection with water plane:
/*    ICamera* pCam = GetCamera();
    if (!pCam) return;
	Frustum Fr = pCam->GetFrustum();
	Line3D Dirs[] = { Line3D(Fr.ltn(), Fr.ltf() - Fr.ltn()), // Left Top
		Line3D(Fr.rtn(), Fr.rtf() - Fr.rtn()), // Right Top
		Line3D(Fr.lbn(), Fr.lbf() - Fr.lbn()), // Left Bottom
		Line3D(Fr.rbn(), Fr.rbf() - Fr.rbn()) }; // Right Bottom
	Plane WaterPlane(Vector3D(0.0f, 0.0f, GetWaterline()), Vector3D::oZ);
	int NInters = 0;
	Vector3D Inters[4];
	for(int nDir = 0; nDir < 4; nDir++) {
		Dirs[nDir].Normalize();
		if(Dirs[nDir].IntersectPlane(WaterPlane, Inters[NInters])) {
			NInters++;
		}
	}
	float xMin = FLT_MAX, yMin = FLT_MAX, xMax = -FLT_MAX, yMax = -FLT_MAX;
	for(int i = 0; i < NInters; i++) {
		xMin = min(xMin, Inters[i].x);
		yMin = min(yMin, Inters[i].y);
		xMax = max(xMax, Inters[i].x);
		yMax = max(yMax, Inters[i].y);
	}*/

//    Frustum cullFrustum = pCam->GetFrustum();

  //  Vector3D v[12]; 
//    int nFV = cullFrustum.Intersection( Plane::xOy, v );
//	int nFV = cullFrustum.Intersection(Plane(Vector3D(0.0f, 0.0f, GetWaterline()), Vector3D::oZ), v);
/*    float xMin =  FLT_MAX;
    float yMin =  FLT_MAX;
    float xMax = -FLT_MAX;
    float yMax = -FLT_MAX;
    for (int i = 0; i < nFV; i++)
    {
        if (v[i].x < xMin) xMin = v[i].x;
        if (v[i].y < yMin) yMin = v[i].y;
        if (v[i].x > xMax) xMax = v[i].x;
        if (v[i].y > yMax) yMax = v[i].y;
    }
*/
	Rct rcVisible = CalcVisibleWaterRect();
	Rct ext = ITerra->GetExtents();

	float xMin = tmax(ext.x, rcVisible.x);
	float yMin = tmax(ext.y, rcVisible.y);
	float xMax = tmin(ext.GetRight(), rcVisible.GetRight());
	float yMax = tmin(ext.GetBottom(), rcVisible.GetBottom());
//    xMin = tmax( ext.x, xMin );
 //  yMin = tmax( ext.y, yMin );
//    xMax = tmin( ext.GetRight(),  xMax );
//    yMax = tmin( ext.GetBottom(), yMax );

    float qw = ext.w/float( c_WaterWQuads );
    float qh = ext.h/float( c_WaterHQuads );

    int qBegX = tmax<int>( 0, (xMin - ext.x - qw)/qw );
    int qBegY = tmax<int>( 0, (yMin - ext.y - qh)/qh );  
    int qEndX = tmin<int>( (xMax - ext.x + qw)/qw, c_WaterWQuads - 1);
    int qEndY = tmin<int>( (yMax - ext.y + qh)/qh, c_WaterHQuads - 1);

    qBegX -= 4;
    qBegY -= 4;
    qEndX += 4;
    qEndY += 6;

    clamp( qBegX, 0, c_WaterWQuads );
    clamp( qBegY, 0, c_WaterHQuads );
    clamp( qEndX, 0, c_WaterWQuads );
    clamp( qEndY, 0, c_WaterHQuads );

    if (m_IBID == -1) m_IBID = IRS->CreateIB( "WaterRenderer", c_WaterIBufferBytes, isWORD, false );
    if (m_VBID == -1) 
    {
        m_VDecl.m_TypeID = IRS->RegisterVType( m_VDecl );
        m_VBID = IRS->CreateVB( "WaterRenderer", c_WaterVBufferBytes, m_VDecl.m_TypeID, false );
    }
    IRS->SetIB( m_IBID );
    IRS->SetVB( m_VBID, m_VDecl.m_TypeID );

    IRS->SetShader( -1 );

    int nQ = 0;
    int nV = 0;
    int nP = 0;

	ICamera *pCam = GetCamera();
	if(!pCam) {
		return;
	}
	const Frustum Fr = pCam->GetFrustum();

    for (int j = qBegY; j <= qEndY; j++)
    {
        for (int i = qBegX; i <= qEndX; i++)
        {
            int qIdx = i + j*c_WaterWQuads;
            if (qIdx >= c_WaterHQuads*c_WaterWQuads) continue;
            WaterQuad& wq = m_Quads[qIdx];
            
            Rct qrct( float( i )*qw + ext.x, float( j )*qh + ext.y, qw, qh );
            AABoundBox aabb( qrct, -100.0f + GetWaterline(), 100.0f + GetWaterline());
            if (!Fr.Overlap( aabb ) || wq.m_bNoWater) continue;
            nQ++;
            
            bool bIBValid = IRS->IsIBStampValid( m_IBID, wq.m_IBStamp );
            bool bVBValid = IRS->IsVBStampValid( m_VBID, wq.m_VBStamp );

            if (!bIBValid || !bVBValid)
            {
                int nVert = m_BaseGrid.getNVert();
                WaterVertex* pSV = (WaterVertex*)m_BaseGrid.getVertexData();
                WaterVertex* pDV = (WaterVertex*)m_Grid.getVertexData();
                DWORD maxAlpha = 0;
                for (int k = 0; k < nVert; k++)
                {
                    pDV[k].x = pSV[k].x*qw + qrct.x;
                    pDV[k].y = pSV[k].y*qh + qrct.y;
                    DWORD color = GetWaterColor( pDV[k].x, pDV[k].y );
                    pDV[k].diffuse = color;
                    color = (color&0xFF000000)>>24;
                    if (color > maxAlpha) maxAlpha = color;
                }
                if (maxAlpha == 0) 
                {
                    wq.m_bNoWater = true;
                    continue;
                }
            }

            if (!bIBValid)
            {
                BYTE* pOut = IRS->LockAppendIB( m_IBID, m_Grid.getNInd(), wq.m_IBPos, wq.m_IBStamp );
                if (pOut) 
                {
                    memcpy( pOut, m_Grid.getIndices(), m_Grid.getNInd()*sizeof(WORD) );
                    IRS->UnlockIB( m_IBID );
                }
            }

            if (!bVBValid)
            {   
                BYTE* pOut  = IRS->LockAppendVB( m_VBID, m_Grid.getNVert(), wq.m_VBPos, wq.m_VBStamp );
                if (pOut) 
                {
                    memcpy( pOut, m_Grid.getVertexData(), m_Grid.getNVert()*m_VDecl.m_VertexSize );
                    IRS->UnlockVB( m_VBID );
                }
            }

            RenderTask& rt = IRS->AddTask();
            rt.m_bHasTM         = false;
			rt.m_ShaderID       = m_OrthoModeIsEnabled ? idWaterShOrtho : m_Shader;//shWater;
            rt.m_TexID[0]       = noiseTex;
            rt.m_TexID[1]       = reflTex;
            rt.m_TexID[2]       = shTex;
            rt.m_TexID[3]       = -1;
            rt.m_TexID[4]       = -1;
            rt.m_TexID[5]       = -1;
            rt.m_TexID[6]       = -1;
            rt.m_TexID[7]       = -1;
            rt.m_bTransparent   = true;
            rt.m_VBufID         = m_VBID;
            rt.m_FirstVert      = wq.m_VBPos;
            rt.m_NVert          = m_Grid.getNVert();
            rt.m_IBufID         = m_IBID;
            rt.m_FirstIdx       = wq.m_IBPos;
            rt.m_NIdx           = m_Grid.getNInd();
            rt.m_VType          = m_VDecl.m_TypeID;
            rt.m_bHasTM         = false;
            rt.m_Source         = "Water";

            nV += m_Grid.getNVert();
            nP += m_Grid.getNPri();
        }
    }    

    rsFlush();

    IRS->SetTextureTM       ( reflTM,   1 );
    IRS->SetTextureTM       ( shTM,     2 );
    IRS->ResetWorldTM       ();
    IRS->SetShaderAutoVars  ();
    
    IRS->Flush();

} // Waterscape::Render

void Waterscape::SetShaderQuality( int Level ){
    static int idWaterShPersp = IRS->GetShaderID("ocean");
    static int idWaterShPerspLo = IRS->GetShaderID("low\\ocean");
    if(Level)m_Shader=idWaterShPersp;
    else m_Shader=idWaterShPerspLo;

	// Since this func is calling every frame,
	// we can assign vars here.

	// Waterline && Boiling:
	static int idWaterline = IRS->GetShaderVarID(idWaterShPersp, "Waterline");
	static int idWaterlineLo = IRS->GetShaderVarID(idWaterShPerspLo, "Waterline");
	static int idBoilingCoef = IRS->GetShaderVarID(idWaterShPersp, "BoilingCoef");
	static int idBoilingCoefLo = IRS->GetShaderVarID(idWaterShPerspLo, "BoilingCoef");
	IRS->SetShaderVar(idWaterShPersp, idWaterline, m_Waterline);
	IRS->SetShaderVar(idWaterShPerspLo, idWaterlineLo, m_Waterline);
	IRS->SetShaderVar(idWaterShPersp, idBoilingCoef, m_BoilingCoef);
	IRS->SetShaderVar(idWaterShPerspLo, idBoilingCoefLo, m_BoilingCoef);
}


