/*****************************************************************************/
/*    File:    vShadowManager.cpp
/*    Desc:    Shadow manager interface implementation
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "ICamera.h"
#include "IMediaManager.h"
#include "kContext.h"
#include "vShadowManager.h"
#include "ITerrain.h"
#include "sgShader.h"
#include "sgTexture.h"

ShadowManager g_ShadowMgr;
IShadowManager* IShadowMgr = &g_ShadowMgr;

/*****************************************************************************/
/*  ShadowManager implementation
/*****************************************************************************/
ShadowManager::ShadowManager()
{
    m_ShadowColor       = 0xFFAAAAAA;
    m_ShadowMapID       = -1;
    m_ShadowMapID2      = -1;
    m_SMapWidth         = 0;
    m_SMapHeight        = 0;
    m_bNeedClearSMap    = true;
    m_LightDir          = Vector3D( -0.5f, -0.1f, -0.5f );
    m_bEnabled          = true;
    m_bClipToGround     = true;
    m_ClipBias          = 0.0f;
    m_ShadowQuality     = sqUnknown;
    m_ShadowMapTM       = Matrix4D::identity;
    m_UVPostProjTM      = Matrix4D::identity;

    m_LightDir.normalize();
} // ShadowManager::ShadowManager

void ShadowManager::Init()
{
    if (m_bInited) return;
    SetShadowQuality( m_ShadowQuality );
    m_bInited = true;
} // ShadowManager::Init

void ShadowManager::SetShadowMapSide( int w, int h )
{
    if (h == 0) h = w;
    if (m_SMapWidth == w && m_SMapHeight == h) return;
    m_SMapWidth  = w;
    m_SMapHeight = h;
    if (m_ShadowMapID != -1) IRS->DeleteTexture( m_ShadowMapID );
    if (m_ShadowMapID2 != -1) IRS->DeleteTexture( m_ShadowMapID2 );

    m_ShadowMapID = IRS->CreateTexture( "ShadowMap", m_SMapWidth, m_SMapHeight, cfARGB8888, 1, tmpDefault, true );    
    m_ShadowMapID2 = IRS->CreateTexture( "ShadowMap2", m_SMapWidth, m_SMapHeight, cfARGB8888, 1, tmpDefault, true );    
    //  clear shadowmap

    IRS->PushRenderTarget   ( m_ShadowMapID     );
    IRS->ClearDevice        ( 0        );
    IRS->PopRenderTarget    ();

    IRS->PushRenderTarget   ( m_ShadowMapID2    );
    IRS->ClearDevice        ( 0        );
    IRS->PopRenderTarget    ();

} // ShadowManager::SetShadowMapSide

void ShadowManager::SetShadowQuality( ShadowQuality quality )
{
    switch (quality)
    {
    case sqNoShadows:
        //SetShadowMapSide( 2 );
        //m_bEnabled = false;
        //break;
    case sqBlobs:
        //SetShadowMapSide( 2 );
        //m_bEnabled = false;
        //break;
    case sqLow:
        SetShadowMapSide( 256, 256 );
        m_bEnabled = true;
        break;
    case sqMedium:
        SetShadowMapSide( 512, 512 );
        m_bEnabled = true;
        break;
    case sqHigh:
        SetShadowMapSide( 1024, 1024 );
        m_bEnabled = true;
        break;
    default:
        assert( false );
    }
    m_ShadowQuality = quality;
} // ShadowManager::SetShadowQuality

void ShadowManager::SetLightDir( const Vector3D& dir )
{
    m_LightDir = dir;
} // ShadowManager::SetLightDir

void ShadowManager::Render()
{
	if (!m_bEnabled){
		m_Casters.clear();
		m_Simples.clear();
		return;
	}
    if (!m_bInited) Init();

	bool AllowShadowBlurring=false;
    
    if (m_ShadowMapID == -1)
    {
        m_Casters.clear();
        m_Simples.clear();
        return;
    }

    ICamera* pCam = GetCamera();
	if (!pCam){
		m_Casters.clear();
		m_Simples.clear();
		return;
	}

    int nCasters = m_Casters.size();
    int nSimples = m_Simples.size();

    IRS->Flush();
	IRS->PushRenderTarget( AllowShadowBlurring?m_ShadowMapID2:m_ShadowMapID );
    IRS->ClearDevice( 0 ); 

    if (nCasters == 0 && nSimples==0) 
    {
        IRS->PopRenderTarget();
		m_Casters.clear();
		m_Simples.clear();
        return;
    }
   
    Matrix4D viewTM = IRS->GetViewTM();
    Matrix4D projTM = IRS->GetProjTM();
    Rct vp = IRS->GetViewPort();

    CalculateTSM();

    static int s_ShadowShader = IRS->GetShaderID( "projected_shadow_caster" );
    
    //Shader::PushOverride( s_ShadowShader );
	int shq = IRS->GetShadersQuality();
 	IRS->SetShadersQuality( 255 );

    Matrix4D lightProjTM( m_LightProjTM );
    lightProjTM *= m_UVPostProjTM;

    IRS->SetTextureFactor   ( 0xFFFFFFFF - m_ShadowColor     );
    IRS->SetViewTM          ( m_LightViewTM );
    IRS->SetProjTM          ( lightProjTM );
    IRS->ResetWorldTM       ();
    IRS->SetViewPort        ( Rct( 0, 0, m_SMapWidth, m_SMapHeight ), 0.0f, 1.0f, false );
    IRS->SetShaderAutoVars();

	//ITerra->SubstShader( s_ShadowShader );
	//ITerra->Render();
	//ITerra->SubstShader( -1 );

    for (int i = 0; i < nCasters; i++)
    {
        ShadowCaster& cst = m_Casters[i];

        if (m_bClipToGround)
        {
            Plane plane;
            Vector3D pos = cst.wTM.getTranslation();
            float h = ITerra->GetH( pos.x, pos.y );
            const Plane c_TopPlane = Plane( Vector3D( 0.0f, 0.0f, 10000.0f ), Vector3D( 0.0f, 0.0f, -1.0f ) );
            if (h >= pos.z)
            {
                pos.z = h;
                plane.fromPointNormal(  Vector3D( pos.x, pos.y, pos.z - m_ClipBias ), 
                                        ITerra->GetNormal( pos.x, pos.y ) );
                IRS->SetClipPlane( 0, plane );
            }
            else IRS->SetClipPlane( 0, c_TopPlane );
        }
		if(cst.CastCallback)
		{
            cst.CastCallback();
		}
		else
		{
			AABoundBox aabb = IMM->GetBoundBox( cst.mdlID );
			aabb.Transform( cst.wTM );
	        
			SetEntityContext( cst.context );
			IMM->StartModel( cst.mdlID, cst.wTM, CUR_CONTEXT );
			IMM->DrawModel();

            //debug:
            //Vector4D c = aabb.GetCenter();
            //c.w = 1.0f;
            //c *= m_LightViewTM;
            //c *= lightProjTM;
            //c.normW();
            int t = 0;
		}        
    }
	//Shader::PopOverride();
    // draw simple casters
    //written by Drew
    if(nSimples){
		IRS->SetClipPlane( 0, Plane::xOy );
        rsFlush();
        static int ShadowPalette = IRS->GetTextureID( "ShadowPalette.dds" );        
        //static int shader = IRS->GetShaderID( "hud" );
        rsSetTexture( ShadowPalette );
        rsSetShader( s_ShadowShader );
        Vector3D Ldir=Vector3D(2,1,0);//should be calculated
        Ldir.normalize();
        Vector3D Lort(Ldir.y,-Ldir.x,0);		
		IRS->SetShader( s_ShadowShader );
		IRS->SetTextureFactor   ( 0xFFFFFFFF - m_ShadowColor     );
        for(int i=0;i<nSimples;i++){
            SimpleCaster& sc=m_Simples[i];
            float u0=float(sc.TexFragmentIdx&3)/4.0f;
            float v0=float(sc.TexFragmentIdx>>2)/4.0f;
            if(sc.Aligning){//vertical                
                Vector3D V1=sc.Pos-Lort*sc.Width*0.5;
                Vector3D V2=sc.Pos+Lort*sc.Width*0.5;
                Vector3D VT(0,0,sc.Length);
                Vector3D VB(0,0,-sc.Length/3);
                rsQuad(V1+VT,V2+VT,V1+VB,V2+VB,Rct(u0,v0,0.25f,0.25f),Rct(0,0,0,0),sc.Color);
            }else{//horisontal
                Vector3D L=Ldir*sc.Length*0.5;
                Vector3D W=Lort*sc.Width*0.5;
                rsQuad(sc.Pos+L-W,sc.Pos+L+W,sc.Pos-L-W,sc.Pos-L+W,Rct(u0,v0,0.25f,0.25f),Rct(0,0,0,0),sc.Color);
            }
        }
        rsFlush();
        rsSetTexture(-1);
        rsRestoreShader();        
    }
	IRS->SetShadersQuality( shq );
    ////////////////////////////      
	ResetEntityContext();
	if(AllowShadowBlurring){		
		//  restore render target
		IRS->PopRenderTarget();    
		IRS->Flush();
		IRS->PushRenderTarget( m_ShadowMapID );
		IRS->ClearDevice( 0xFFFFFFFF ); 

		rsSetTexture( m_ShadowMapID2 );
		static int shader = IRS->GetShaderID( "hud" );
		rsSetShader( shader );

		float duv = 1.0f/m_SMapWidth;
		rsRect(Rct(0,0,m_SMapWidth,m_SMapWidth),Rct(0,0    ,1,1),0,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF);
		if(!nSimples){
			rsRect(Rct(0,0,m_SMapWidth,m_SMapWidth),Rct(duv*3,duv*2,1,1),0,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF);
			rsRect(Rct(0,0,m_SMapWidth,m_SMapWidth),Rct(duv*2,duv*3,1,1),0,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF);    
			rsRect(Rct(0,0,m_SMapWidth,m_SMapWidth),Rct(duv,duv,1,1),0,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF);
		    

			rsRect(Rct(0,0,m_SMapWidth,m_SMapWidth),Rct(duv,0  ,1,1),0,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF);
			rsRect(Rct(0,0,m_SMapWidth,m_SMapWidth),Rct(0,duv  ,1,1),0,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF,0x50FFFFFF);
		}
	    

		rsFlush();
		rsSetTexture(-1);
		rsRestoreShader();    
	}

    //  draw white frame to prevent border effects when clamping uv
	static DWORD fColor=0xFF000000;
    rsFrame( Rct( 0.0f, 0.0f, m_SMapWidth, m_SMapWidth ), 0.0f, fColor );
	rsFlush();
    float fadeW = m_SMapWidth/8.0f;

    //rsRect(Rct(0,0,fadeW,m_SMapWidth)                   ,Rct(0,0,0,0),0,0xFFFFFFFF,0x00FFFFFF,0xFFFFFFFF,0x00FFFFFF);
    //rsRect(Rct(m_SMapWidth-fadeW,0,fadeW,m_SMapWidth)   ,Rct(0,0,0,0),0,0x00FFFFFF,0xFFFFFFFF,0x00FFFFFF,0xFFFFFFFF);
    //rsRect(Rct(0,0,m_SMapWidth,fadeW)                   ,Rct(0,0,0,0),0,0xFFFFFFFF,0xFFFFFFFF,0x00FFFFFF,0x00FFFFFF);
    //rsRect(Rct(0,m_SMapWidth-fadeW,m_SMapWidth,fadeW)   ,Rct(0,0,0,0),0,0x00FFFFFF,0x00FFFFFF,0xFFFFFFFF,0xFFFFFFFF);

    //rsFlush();

    IRS->Flush();

    IRS->PopRenderTarget();
    //Shader::PopOverride();
    
    //BlurShadowMap();   
    


    //IRS->SaveTexture( m_ShadowMapID, "c:\\dumps\\shmap.dds" );

    //  restore camera parameters
    IRS->SetViewTM( viewTM );
    IRS->SetProjTM( projTM );
    IRS->SetViewPort( vp );

    //  clear casters list
    m_Casters.clear();
    m_Simples.clear();
    m_bNeedClearSMap = true;
} // ShadowManager::Render

void ShadowManager::BlurShadowMap()
{
    //return;
    IRS->SetTexture( m_ShadowMapID, 0 );
    IRS->SetTexture( m_ShadowMapID, 1 );

    static BaseMesh bm;
    if (bm.getNVert() == 0)
    {
        bm.create( 4, 0, vfVertexTnL );
        bm.setIsQuadList( true );
        bm.setNVert( 4 );
    }
    VertexTnL* v = (VertexTnL*)bm.getVertexData();

    v[0].x = 0.0f;
    v[0].y = 0.0f;
    v[0].z = 0.0f;
    v[0].w = 1.0f;
    v[0].u = 0.0f;
    v[0].v = 0.0f;

    v[1].x = m_SMapWidth;
    v[1].y = 0.0f;
    v[1].z = 0.0f;
    v[1].w = 1.0f;
    v[1].u = 1.0f;
    v[1].v = 0.0f;

    v[2].x = 0.0f;
    v[2].y = m_SMapHeight;
    v[2].z = 0.0f;
    v[2].w = 1.0f;
    v[2].u = 0.0f;
    v[2].v = 1.0f;

    v[3].x = m_SMapWidth;
    v[3].y = m_SMapHeight;
    v[3].z = 0.0f;
    v[3].w = 1.0f;
    v[3].u = 1.0f;
    v[3].v = 1.0f;

    static int shID = IRS->GetShaderID( "shadow_map_filter" );
    bm.setShader( shID );

    static int t = IRS->GetTextureID( "env.jpg" );
    bm.setTexture( m_ShadowMapID, 0 );
    bm.setTexture( m_ShadowMapID, 1 );

    DrawBM( bm );
} // ShadowManager::BlurShadowMap

void ProjToRect( Vector2D& v, const Rct& rc )
{
    v.x = (v.x + 1.0f)*rc.w*0.5f + rc.x;
    v.y = (1.0f - v.y)*rc.h*0.5f + rc.y;
}

void ProjToRect( Vector4D& v, const Rct& rc )
{
    v.x = (v.x + 1.0f)*rc.w*0.5f + rc.x;
    v.y = (1.0f - v.y)*rc.h*0.5f + rc.y;
}

struct DebugElm{
	DWORD Color;
	float x;
	float y;
	float x1;
	float y1;
	bool Line;
};
std::vector<DebugElm> DebPts;
void AddDLine(float x,float y,float x1,float y1,DWORD Color){
	DebugElm D;
	D.x=x;
	D.y=y;
	D.x1=x1;
	D.y1=y1;
	D.Line=true;
	D.Color=Color;
	DebPts.push_back(D);
}
void AddDPoint(float x,float y,DWORD Color){
	DebugElm D;
	D.x=x;
	D.y=y;
	D.Line=false;
	D.Color=Color;
	DebPts.push_back(D);
}
void DrawDebPts(){
	float cx=400;
	float cy=400;
	float sc=60;
	int n=5;
	rsRect(cx,cy,sc*n,0x40000000);
	for(int i=-n;i<=n;i++){
		rsLine(cx-sc*n,cy+i*sc,cx+sc*n,cy+i*sc,0,i==0?0xFFFF0000:0xFF000000);
	}
	for(int i=-n;i<=n;i++){
		rsLine(cx+i*sc,cy-n*sc,cx+i*sc,cy+n*sc,0,i==0?0xFFFF0000:0xFF000000);
	}
	rsFlush();	
	
	for(int i=0;i<DebPts.size();i++){
		DebugElm& D=DebPts[i];
		if(D.Line){
			rsLine(cx+D.x*sc,cy+D.y*sc,cx+D.x1*sc,cy+D.y1*sc,0,D.Color,D.Color);
		}else{
			rsRect(cx+D.x*sc,cy+D.y*sc,3,D.Color);            
		}
	}
	rsFlush();
	DebPts.clear();
}

void ShadowManager::DrawDebugInfo()
{
    IRS->ResetWorldTM();
	DrawDebPts();

	rsFrame(Rct( 0.0f, 0.0f, m_SMapWidth/2, m_SMapHeight/2 ),0.5,0X40FFFFFF);
	rsFlush();
	
    //  shadow map texture
    static int sh = IRS->GetShaderID( "hudZDisable" );
    rsSetShader( sh );
    rsSetTexture( m_ShadowMapID );
    DWORD clr = 0x30FFFFFF;
    rsRect( Rct( 0.0f, 0.0f, m_SMapWidth/2, m_SMapHeight/2 ), Rct::unit, 0.0f, clr, clr, clr, clr );
    rsFlush();
    //IRS->SaveTexture( m_ShadowMapID, "c:\\dumps\\ShadowMap.dds" );
    rsRestoreShader();
	


    Vector2D lt = m_LT;
    Vector2D rt = m_RT;
    Vector2D rb = m_RB;
    Vector2D lb = m_LB;

    Vector4D tlt( lt.x, lt.y, 0.0f, 1.0f );
    Vector4D tlb( lb.x, lb.y, 0.0f, 1.0f );
    Vector4D trt( rt.x, rt.y, 0.0f, 1.0f );
    Vector4D trb( rb.x, rb.y, 0.0f, 1.0f );

    tlt *= m_UVPostProjTM; tlt.normW();
    tlb *= m_UVPostProjTM; tlb.normW();
    trt *= m_UVPostProjTM; trt.normW();
    trb *= m_UVPostProjTM; trb.normW();

    Rct rc( 512.0f, 0.0f, 256.0f, 256.0f );
    Rct vp = IRS->GetViewPort();
    rsFlush();
    IRS->SetViewPort( rc, 0.0f, 1.0f, false );
    rsRect( rc, 0.0f, 0x22000000 );
    rsFlush();

    ProjToRect( lt, rc );
    ProjToRect( lb, rc );
    ProjToRect( rt, rc );
    ProjToRect( rb, rc );

    Rct trc( rc );
    trc.Inflate( -10 );
    ProjToRect( tlt, trc );
    ProjToRect( tlb, trc );
    ProjToRect( trt, trc );
    ProjToRect( trb, trc );

    rsLine( lt.x, lt.y, lb.x, lb.y, 0.0f, 0xFFFFFFFF );
    rsLine( lb.x, lb.y, rb.x, rb.y, 0.0f, 0xFFFFFFFF );
    rsLine( rb.x, rb.y, rt.x, rt.y, 0.0f, 0xFFFFFFFF );
    rsLine( rt.x, rt.y, lt.x, lt.y, 0.0f, 0xFFFFFFFF );

    rsLine( tlt.x, tlt.y, tlb.x, tlb.y, 0.0f, 0xFFFFFFFF );
    rsLine( tlb.x, tlb.y, trb.x, trb.y, 0.0f, 0xFF0000FF );
    rsLine( trb.x, trb.y, trt.x, trt.y, 0.0f, 0xFF00FF00 );
    rsLine( trt.x, trt.y, tlt.x, tlt.y, 0.0f, 0xFFFF0000 );

    rsFlush();
    IRS->SetViewPort( vp );
} // ShadowManager::DrawDebugInfo

const Matrix4D& ShadowManager::CalcShadowMapTM()
{
    ICamera* pCam = GetCamera();
    if (!pCam) return m_ShadowMapTM;
    Matrix4D camTM = pCam->GetWorldTM();
    const Matrix4D c_ProjToUV = Matrix4D(   0.5f,   0.0f,   0.0f, 0.0f,
                                            0.0f,   -0.5f,  0.0f, 0.0f,
                                            0.0f,   0.0f,   1.0f, 0.0f,
                                            0.5f,   0.5f,   0.0f, 1.0f );
    //  create shadow map texture matrix
    //  uv's for shadow layer are generated from CameraSpacePosition texgen
    m_ShadowMapTM = camTM;          // from camera space to world space
    m_ShadowMapTM *= m_LightViewTM; // from world space to light camera space
    m_ShadowMapTM *= m_LightProjTM; // from light camera space to light projection space
    m_ShadowMapTM *= m_UVPostProjTM;
    m_ShadowMapTM *= c_ProjToUV;    // from light projection space to uv space
    return m_ShadowMapTM;
} // ShadowManager::CalcShadowMapTM

void ShadowManager::AddProcCaster( cbCastCallback* cb ){
	if (!m_bEnabled) return;
	ShadowCaster caster;
	caster.CastCallback  = cb;
	caster.frame    = 0;
	caster.mdlID    = 0xFFFFFFFF;		
	m_Casters.push_back( caster );
}//ShadowManager::AddProcCaster

void ShadowManager::AddCastingAABB (AABoundBox& AB){
	if (!m_bEnabled) return;
	Vector3D v[2]={AB.minv,AB.maxv};
	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			for(int k=0;k<2;k++){
				AddCastingBoundaryPoint(Vector3D(v[i].x,v[j].y,v[k].z));
			}
		}
	}
}

bool ShadowManager::AddCaster( DWORD modelID, const Matrix4D& tm )
{
    if (!m_bEnabled) return false;
    if (modelID == 0xFFFFFFFF) return false;
    ShadowCaster caster;
	caster.CastCallback  = NULL;
    caster.frame    = IRS->GetCurFrame();
    caster.mdlID    = modelID;
    caster.wTM      = tm;
    caster.context  = GetEntityContext();

	AddCastingBoundaryPoint(Vector3D(tm.e30,tm.e31,tm.e32));

    m_Casters.push_back( caster );
    return true;
} // ShadowManager::AddCaster

void ShadowManager::SetShadowColor( DWORD color )
{
    ColorValue fColor( color );
    float ratio = 1.0f - fColor.a;
    fColor.r = ratio + (1.0f - ratio)*fColor.r;
    fColor.g = ratio + (1.0f - ratio)*fColor.g;
    fColor.b = ratio + (1.0f - ratio)*fColor.b;
    m_ShadowColor = fColor;
} // ShadowManager::SetShadowColor

Vector3D PutLineOnPoints(std::vector<Vector3D>& pts,Vector3D normal,float ots){
	int n=pts.size();
	float maxv=10000000;
	for(int i=0;i<n;i++){
		Vector3D v=pts[i];
		float d=v.dot(normal);
		if(d<maxv)maxv=d;
	}
	return normal*(maxv-0.6*ots);
}
Vector3D IntersectLines(Vector3D P1,Vector3D N1,Vector3D P2,Vector3D N2){
	float D=N1.x*N2.y-N1.y*N2.x;
	float E=N1.x*P1.x+N1.y*P1.y;
	float F=N2.x*P2.x+N2.y*P2.y;
    float x=(E*N2.y-F*N1.y)/D;
	float y=(F*N1.x-E*N2.x)/D;
	return Vector3D(x,y,0);
}
Matrix3D MapQuadToUnitSquare( const Vector2D& a, const Vector2D& b, 
                              const Vector2D& c, const Vector2D& e )
{
    Matrix3D m;
    float dx1 = b.x - c.x;
    float dx2 = e.x - c.x;
    float dy1 = b.y - c.y;
    float dy2 = e.y - c.y;
    float sx  = a.x - b.x + c.x - e.x; 
    float sy  = a.y - b.y + c.y - e.y;  
    if (fabs( sx ) < c_Epsilon && fabs( sy ) < c_Epsilon)
    {
        m.e00 = b.x - a.x; m.e10 = c.x - b.x; m.e20 = a.x;
        m.e01 = b.y - a.y; m.e11 = c.y - b.y; m.e21 = a.y;
        m.e02 = 0.0f;      m.e12 = 0.0f;      m.e22 = 1.0f;
    }
    else
    {
        float d = dx1*dy2 - dy1*dx2;
        m.e02 = (sx*dy2 - sy*dx2)/d;      
        m.e12 = (dx1*sy - dy1*sx)/d;      
        m.e22 = 1.0f;
        m.e00 = b.x - a.x + m.e02*b.x; 
        m.e10 = e.x - a.x + m.e12*e.x; 
        m.e20 = a.x;
        m.e01 = b.y - a.y + m.e02*b.y; 
        m.e11 = e.y - a.y + m.e12*e.y; 
        m.e21 = a.y;
    }
    m.inverse();
    //m *= 1.0f/m.e22;
    return m;
} // MapQuadToUnitSquare

const float c_MaxShadowMapDistance = 4000.0f;
void ShadowManager::CalculateTSM()
{
    ICamera* pCam = GetCamera();
    
    //  get camera frustum
    Frustum frustum = pCam->GetFrustum();
    
    //  create light view matrix
    Vector3D at;
    
    Vector4D c[4];    
    Vector3D pc[8];
    int nPt = frustum.Intersection( Plane::xOy, pc );
    c[0] = pc[0]; 
    c[1] = pc[1]; 
    c[2] = pc[2]; 
    c[3] = pc[3];       
	
	
    Vector3D pickPt;
    Vector3D dlt = frustum.ltf() - frustum.ltn(); dlt.normalize();
    Vector3D drt = frustum.rtf() - frustum.rtn(); drt.normalize();
    Vector3D drb = frustum.rbf() - frustum.rbn(); drb.normalize();
    Vector3D dlb = frustum.lbf() - frustum.lbn(); dlb.normalize();

    
    bool bLT = ITerra->Pick( frustum.ltn(), dlt, pickPt ); //c[0] = pickPt;
    bool bRT = ITerra->Pick( frustum.rtn(), drt, pickPt ); //c[1] = pickPt; 
    bool bRB = ITerra->Pick( frustum.rbn(), drb, pickPt ); //c[2] = pickPt; 
    bool bLB = ITerra->Pick( frustum.lbn(), dlb, pickPt ); //c[3] = pickPt;
    
    if (!bLT || !bRT)
    {
        c[0] = pCam->GetPosition() + dlt*c_MaxShadowMapDistance;
        c[1] = pCam->GetPosition() + drt*c_MaxShadowMapDistance;
        c[0].z = 0.0f;
        c[1].z = 0.0f;
    }
    

	//adding extra border (by Drew)
	{
		static float otsHT=0.0;
        static float otsHB=0.0;

        Vector3D dcT=Vector3D(c[1])-Vector3D(c[0]);
        Vector3D dcB=Vector3D(c[3])-Vector3D(c[2]);
        Vector3D cT=(Vector3D(c[1])+Vector3D(c[0]))*0.5;
        Vector3D cB=(Vector3D(c[3])+Vector3D(c[2]))*0.5;

        c[0]=Vector4D(cT-dcT*(0.5+otsHT));
        c[1]=Vector4D(cT+dcT*(0.5+otsHT));
        c[2]=Vector4D(cB-dcB*(0.5+otsHB));
        c[3]=Vector4D(cB+dcB*(0.5+otsHB));
        /*
		Vector3D D=pCam->GetDir();
		D.z=0;
		D.normalize();
		Vector3D DD=D;
		DD*=otsV;
		swap(D.x,D.y);
        float R1=c[0].distance(c[1]);
        float R2=c[2].distance(c[3]);
		D.y*=-1;
		D*=R1*otsHT;
		c[0]+=D;c[1]-=D;//c[2]-=D;c[3]+=D;
		c[0]+=DD;c[1]+=DD;//c[2]-=DD;c[3]-=DD;
        */
	}
    c[0].w=1;
    c[1].w=1;
    c[2].w=1;
    c[3].w=1;

	///////////////////////////////	
    
    at = pCam->GetPosition() + (drb + dlb)*pCam->GetZn();
    at.z = 0.0f;

    //  create light view matrix
    Vector3D vZ = m_LightDir;
    Vector3D vY = Vector3D::oZ; 
    Vector3D vX;
    vX.cross( vY, vZ );
    Vector3D::orthonormalize( vZ, vX, vY );
    Matrix4D m;
    m.e00 = vX.x; m.e01 = vX.y; m.e02 = vX.z; m.e03 = 0.0f;
    m.e10 = vY.x; m.e11 = vY.y; m.e12 = vY.z; m.e13 = 0.0f;
    m.e20 = vZ.x; m.e21 = vZ.y; m.e22 = vZ.z; m.e23 = 0.0f;
    m.e30 = at.x; m.e31 = at.y; m.e32 = at.z; m.e33 = 1.0f;
    m.inverse();
    m_LightViewTM = m;

    //  create light projection matrix
    Matrix4D lightProjTM;
    const float c_Radius = 1000.0f;
    OrthoProjectionTM( m_LightProjTM, c_Radius*2.0f, m_SMapWidth/m_SMapHeight, 
        -c_Radius*100.0f, c_Radius*100.0f );

    Matrix4D lightFullTM;
    lightFullTM.mul( m_LightViewTM, m_LightProjTM );
    

	int n=m_CastBounds.size();	
	

	for(int i=0;i<n;i++){
        Vector4D v=m_CastBounds[i];
		v*=lightFullTM;
		v.normW();
		m_CastBounds[i]=v;		
		//AddDPoint(v.x,v.y,0xFFFFFFFF);
	}	

    for (int i = 0; i < 4; i++)
    {
        c[i] *= lightFullTM;
        c[i].normW();
    }
	float d1=c[0].distance(c[2]);
	float d2=c[1].distance(c[3]);
	d1=max(d1,d2)/2;
	if(d1>1)d1=1;	
	d1*=0.2f;

	Vector3D N0(c[1].y-c[0].y,c[0].x-c[1].x,0);
	N0.normalize();
	Vector3D P0=PutLineOnPoints(m_CastBounds,N0,d1);
	Vector3D N1(c[2].y-c[1].y,c[1].x-c[2].x,0);
	N1.normalize();
	Vector3D P1=PutLineOnPoints(m_CastBounds,N1,d1);
	Vector3D N2(c[3].y-c[2].y,c[2].x-c[3].x,0);
	N2.normalize();
	Vector3D P2=PutLineOnPoints(m_CastBounds,N2,d1);
	Vector3D N3(c[0].y-c[3].y,c[3].x-c[0].x,0);
	N3.normalize();
	Vector3D P3=PutLineOnPoints(m_CastBounds,N3,d1);

	
	//AddDPoint(P0.x,P0.y,0xFFFF0000);
	//AddDLine(P0.x,P0.y,P0.x+N0.x,P0.y+N0.y,0xFFFF0000);
	//AddDPoint(P1.x,P1.y,0xFF00FF00);
	//AddDLine(P1.x,P1.y,P1.x+N1.x,P1.y+N1.y,0xFF00FF00);
	//AddDPoint(P2.x,P2.y,0xFF0000FF);
	//AddDLine(P2.x,P2.y,P2.x+N2.x,P2.y+N2.y,0xFF0000FF);
	//AddDPoint(P3.x,P3.y,0xFFFF00FF);
	//AddDLine(P3.x,P3.y,P3.x+N3.x,P3.y+N3.y,0xFFFF00FF);	

	//AddDLine(c[0].x,c[0].y,c[1].x,c[1].y,0xFFFF0000);
	//AddDLine(c[1].x,c[1].y,c[2].x,c[2].y,0xFF00FF00);
	//AddDLine(c[2].x,c[2].y,c[3].x,c[3].y,0xFF0000FF);
	//AddDLine(c[3].x,c[3].y,c[0].x,c[0].y,0xFFFF00FF);
	

	c[0]=IntersectLines(P0,N0,P3,N3);
	c[1]=IntersectLines(P1,N1,P0,N0);
	c[2]=IntersectLines(P2,N2,P1,N1);
	c[3]=IntersectLines(P3,N3,P2,N2);

	
	//AddDLine(c[0].x,c[0].y,c[1].x,c[1].y,0x80FF0000);
	//AddDLine(c[1].x,c[1].y,c[2].x,c[2].y,0x8000FF00);
	//AddDLine(c[2].x,c[2].y,c[3].x,c[3].y,0x800000FF);
	//AddDLine(c[3].x,c[3].y,c[0].x,c[0].y,0x80FF00FF);
	

    
    m_LT = Vector2D( c[0].x, c[0].y );
    m_RT = Vector2D( c[1].x, c[1].y );
    m_RB = Vector2D( c[2].x, c[2].y );
    m_LB = Vector2D( c[3].x, c[3].y );
    
    //  calculate trapezoid-to-uv transform
    Matrix3D qm = MapQuadToUnitSquare( m_LT, m_RT, m_RB, m_LB );
    float e33=qm.e22;    

    qm*=1.0f/e33;    
    Matrix4D pm;    
    pm.e00 = qm.e00; pm.e01 = qm.e01; pm.e02 = 0.0f; pm.e03 = qm.e02;
    pm.e10 = qm.e10; pm.e11 = qm.e11; pm.e12 = 0.0f; pm.e13 = qm.e12;
    pm.e20 = 0.0f;   pm.e21 = 0.0f;   pm.e22 = 1.0f; pm.e23 = 0.0f;
    pm.e30 = qm.e20; pm.e31 = qm.e21; pm.e32 = 0.0f; pm.e33 = qm.e22;
    if(e33<0){
        Matrix4D m=Matrix4D::identity;
        m.e00=-1;
        m.e11=-1;
        m.e33=-1;
        pm.mulLeft(m);
        //pm.e22=-1;
    }
    static float sc=1;
    float sc2=sc*2;
	Matrix4D c_UVToProj = Matrix4D(   sc2,  0.0f, 0.0f, 0.0f,
                                      0.0f, -sc2, 0.0f, 0.0f,
                                      0.0f, 0.0f, sc,   0.0f,
                                      -sc,   sc, 0.0f,  1 );
    pm *= c_UVToProj;
    SetUVPostProjTM( pm );
	m_CastBounds.clear();
} // ShadowManager::CalculateTSM
void ShadowManager::AddSimpleCaster ( Vector3D Pos,Vector3D Direction,float Width,float Length,int IndexInPalette,bool Aligning,DWORD Color){
	if (!m_bEnabled) return;
    SimpleCaster sc;
    sc.Pos=Pos;
    sc.Direction=Direction;
    sc.Width=Width;
    sc.Length=Length;
    sc.Aligning=Aligning;
    sc.TexFragmentIdx=IndexInPalette;
    sc.Color=Color;
    m_Simples.push_back(sc);
	AddCastingBoundaryPoint(Pos);
}
