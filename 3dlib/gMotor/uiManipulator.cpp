/*****************************************************************************/
/*    File:    uiManipulator.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    18.06.2004
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "uiManipulator.h"

IMPLEMENT_CLASS(uiMoveTool);
IMPLEMENT_CLASS( RotateTool     );
IMPLEMENT_CLASS( TranslateTool  );
IMPLEMENT_CLASS( ScaleTool      );

//-----------------------------------------------------------------------------
// uiMoveTool.ctor : ()
//-----------------------------------------------------------------------------
uiMoveTool::uiMoveTool() {
	m_DragMode = dmNone;
	m_XColor = 0xffff0000;
	m_YColor = 0xff00ff00;
	m_ZColor = 0xff0000ff;
	m_CenterColor = 0xffeeeeee;
	m_SelColor = 0xffffff00;
	m_HeightColor = 0xaaff00ff;
	m_TraceColor = 0xff808080;
	m_ArrowLen = 0.164f;
	m_HeadLen = 0.3f;
	m_HeadRadius = 0.06f;
	m_CenterSide = 20;
	m_MaxSelDist = 0.5f;
	m_HeightSide = 6;
	m_TraceSide = 6;

	m_CurPos = Vector3D::null;
	m_StartPos = Vector3D::null;
	m_StartMX = m_StartMY = 0;
	m_pTM = NULL;
}

//-----------------------------------------------------------------------------
// uiMoveTool::Expose : void(PropertyMap &)
//-----------------------------------------------------------------------------
void uiMoveTool::Expose(PropertyMap &pm) {
	pm.start<Parent>("uiMoveTool", this);
	pm.f("DragMode", m_DragMode, NULL, true);
	pm.f("XColor", m_XColor, "color");
	pm.f("YColor", m_YColor, "color");
	pm.f("ZColor", m_ZColor, "color");
	pm.f("CenterColor", m_CenterColor, "color");
	pm.f("ArrowLen", m_ArrowLen);
	pm.f("MaxSelDist", m_MaxSelDist);
	pm.f("HeadLen", m_HeadLen);
	pm.f("HeadRadius", m_HeadRadius);
	pm.f("CenterSide", m_CenterSide);
	pm.f("HeightColor", m_HeightColor);
	pm.f("HeightSide", m_HeightSide);
}

//-----------------------------------------------------------------------------
// uiMoveTool::BindTM : void(Matrix4D *)
//-----------------------------------------------------------------------------
void uiMoveTool::BindTM(Matrix4D *pSrc) {
	if(!pSrc) {
		m_pTM = NULL;
		return;
	}
	m_pTM = pSrc;
	m_CurPos = m_StartPos = m_pTM->getTranslation();
}

//-----------------------------------------------------------------------------
// uiMoveTool::CalcAxes : float(Vector3D &, Vector3D &, Vector3D &)
//-----------------------------------------------------------------------------
float uiMoveTool::CalcAxes(Vector3D &xAxis, Vector3D &yAxis, Vector3D &zAxis) {
	ICamera *pCam = GetCamera();
	if(!pCam) return 0.0f;
	Vector4D u(m_CurPos, 1);
	pCam->ToSpace(sWorld, sProjection, u);
	u.x += m_ArrowLen;
	pCam->ToSpace(sProjection, sWorld, u);
	u -= m_CurPos;
	float l = u.norm();
	xAxis = Vector3D(l, 0, 0);
	yAxis = Vector3D(0, l, 0);
	zAxis = Vector3D(0, 0, l);
	return l;
}

//-------------------------------------------------------------------------------------
// uiMoveTool::CalcTracePos : float(Vector3D &, Vector3D &, Vector3D &)
//-------------------------------------------------------------------------------------
float uiMoveTool::CalcTracePos(Vector3D &TraceX, Vector3D &TraceY, Vector3D &TraceZ) {
	ICamera *pCam = GetCamera();
	if(!pCam) return 0.0f;
	Vector4D u(m_StartPos, 1);
	pCam->ToSpace(sWorld, sProjection, u);
	u.x += m_ArrowLen * (1 - m_HeadLen);
	pCam->ToSpace(sProjection, sWorld, u);
	float l = (u - m_StartPos).norm();
	TraceX = m_StartPos + Vector3D(l, 0, 0);
	TraceY = m_StartPos + Vector3D(0, l, 0);
	TraceZ = m_StartPos + Vector3D(0, 0, l);
	return l;
}

//-----------------------------------------------------------------------------
// uiMoveTool::Render : void()
//-----------------------------------------------------------------------------
void uiMoveTool::Render() {
	if(!IsVisible()) return;
	if(!m_pTM) return;

	ICamera *pCam = GetCamera();
	if(!pCam) return;

	Vector3D xAxis, yAxis, zAxis;
	CalcAxes(xAxis, yAxis, zAxis);

	Vector3D TraceX, TraceY, TraceZ;
	CalcTracePos(TraceX, TraceY, TraceZ);

	rsEnableZ(false);
	IRS->ResetWorldTM();

	DWORD XClr = m_XColor, YClr = m_YColor, ZClr = m_ZColor, CClr = m_CenterColor,
		XTClr = m_TraceColor, YTClr = m_TraceColor, ZTClr = m_TraceColor, CTClr = m_TraceColor;
	switch(m_DragMode) {
		case dmDragX:
			XClr = m_SelColor;
			YClr = ZClr = YTClr = ZTClr = 0;
			break;
		case dmDragY:
			YClr = m_SelColor;
			XClr = ZClr = XTClr = ZTClr = 0;
			break;
		case dmDragZ:
			ZClr = m_SelColor;
			XClr = YClr = XTClr = YTClr = 0;
			break;
		case dmDragCenter:
			CClr = m_SelColor;
			break;
		default:
			XTClr = YTClr = ZTClr = CTClr = 0;
			break;
	}

	// Projecting:
	pCam->SetViewport(IRS->GetViewPort());
	Vector4D c(m_CurPos, 1);
	pCam->ToSpace(sWorld, sScreen, c);
	Vector4D h(m_CurPos.x, m_CurPos.y, 0, 1);
	pCam->ToSpace(sWorld, sScreen, h);
	Vector4D s(m_StartPos, 1);
	pCam->ToSpace(sWorld, sScreen, s);
	Vector4D mX(TraceX, 1);
	pCam->ToSpace(sWorld, sScreen, mX);
	Vector4D mY(TraceY, 1);
	pCam->ToSpace(sWorld, sScreen, mY);
	Vector4D mZ(TraceZ, 1);
	pCam->ToSpace(sWorld, sScreen, mZ);

	// Traces:
	rsRect(s.x, s.y, m_TraceSide, CTClr);
	rsRect(mX.x, mX.y, m_TraceSide, XTClr);
	rsRect(mY.x, mY.y, m_TraceSide, YTClr);
	rsRect(mZ.x, mZ.y, m_TraceSide, ZTClr);
	rsLine(m_StartPos, TraceX, XTClr);
	rsLine(m_StartPos, TraceY, YTClr);
	rsLine(m_StartPos, TraceZ, ZTClr);

	// XYZHandles:

	if(dmDragCenter == m_DragMode) {
		rsLine(m_CurPos, m_CurPos + xAxis * (1.0f - m_HeadLen), XClr);
		rsLine(m_CurPos, m_CurPos + yAxis * (1.0f - m_HeadLen), YClr);
		rsLine(m_CurPos, m_CurPos + zAxis * (1.0f - m_HeadLen), ZClr);
	} else {
		DrawArrow(m_CurPos, xAxis, XClr, 1.0f, m_HeadLen, m_HeadRadius);
		DrawArrow(m_CurPos, yAxis, YClr, 1.0f, m_HeadLen, m_HeadRadius);
		DrawArrow(m_CurPos, zAxis, ZClr, 1.0f, m_HeadLen, m_HeadRadius);
	}

	// Center handle:
	float x = c.x - m_CenterSide * 0.5f;
	float y = c.y - m_CenterSide * 0.5f;
	rsFrame(Rct(ceilf(x), ceilf(y), m_CenterSide, m_CenterSide), 0, CClr);

	// Height marker:
	x = h.x - m_HeightSide * 0.5f;
	y = h.y - m_HeightSide * 0.5f;
	rsRect(Rct(ceilf(x), ceilf(y), m_HeightSide, m_HeightSide), 0, m_HeightColor);
	rsLine(m_CurPos, Vector3D(m_CurPos.x, m_CurPos.y, 0.0f), m_HeightColor, m_HeightColor);

	rsFlush();
}

//-----------------------------------------------------------------------------
// uiMoveTool::OnMouseLBDown : bool(int, int)
//-----------------------------------------------------------------------------
bool uiMoveTool::OnMouseLBDown(int xMouse, int yMouse) {
	if(!IsVisible()) return false;
	if(GetKeyState(VK_MENU) < 0 || GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_SHIFT) < 0) return false;

	ICamera *pCam = GetCamera();
	if(!pCam || !m_pTM) return false;

	Ray3D ray = pCam->GetPickRay(xMouse, yMouse);

	Vector3D xAxis, yAxis, zAxis;
	float l = CalcAxes(xAxis, yAxis, zAxis);
	xAxis += m_CurPos;
	yAxis += m_CurPos;
	zAxis += m_CurPos;

	float dx = ray.dist2ToPoint(xAxis);
	float dy = ray.dist2ToPoint(yAxis);
	float dz = ray.dist2ToPoint(zAxis);
	float dc = ray.dist2ToPoint(m_CurPos);

	float dMin = tmin(dx, dy, dz, dc);
	float dSel = (l * m_MaxSelDist) * (l * m_MaxSelDist);

	if(m_DragMode == dmNone) {
		m_StartMX = xMouse;
		m_StartMY = yMouse;
		m_StartPos = m_CurPos;
	}
	m_DragMode = dmNone;
	if(dx == dMin && dx < dSel) m_DragMode = dmDragX;
	if(dy == dMin && dy < dSel) m_DragMode = dmDragY;
	if(dz == dMin && dz < dSel) m_DragMode = dmDragZ;
	if(dc == dMin && dc < dSel) m_DragMode = dmDragCenter;

	return false;
}

//-----------------------------------------------------------------------------
// uiMoveTool::OnMouseLBUp : bool(int, int)
//-----------------------------------------------------------------------------
bool uiMoveTool::OnMouseLBUp(int xMouse, int yMouse) {
	if(!IsVisible()) return false;
	m_DragMode = dmNone;
	return false;
}

//-----------------------------------------------------------------------------
// uiMoveTool::OnMouseMove : bool(int, int, DWORD)
//-----------------------------------------------------------------------------
bool uiMoveTool::OnMouseMove(int xMouse, int yMouse, DWORD Keys) {
	if(!IsVisible()) return false;

	if((Keys & MK_LBUTTON) == 0) m_DragMode = dmNone;
	if(m_DragMode == dmNone) return false;

	ICamera *pCam = GetCamera();
	if(!pCam || !m_pTM) return false;

	Ray3D ray = pCam->GetPickRay(xMouse, yMouse);
	Vector4D u(m_StartPos);
	pCam->ToSpace(sWorld, sScreen, u);
	u.x += xMouse - m_StartMX;
	u.y += yMouse - m_StartMY;
	pCam->ToSpace(sScreen, sWorld, u);
	u -= m_StartPos;
	u.w = 0.0f;

	float d = tmax(fabs(u.x), fabs(u.y), fabs(u.z));
	switch(m_DragMode) {
		case dmDragX:
			u.x = sign(u.x) * d;
			u.y = u.z = 0;
			break;
		case dmDragY:
			u.y = sign(u.y) * d;
			u.x = u.z = 0;
			break;
		case dmDragZ:
			u.z = sign(u.z) * d;
			u.x = u.y = 0;
			break;
		default:
			break;
	}
	m_CurPos = m_StartPos + u;
	assert(m_pTM);
	m_pTM->setTranslation(m_CurPos);
	return false;
}

/*****************************************************************************/
/*  TranslateTool implementation
/*****************************************************************************/
TranslateTool::TranslateTool()
{
    m_DragMode          = dmNone;
    m_XColor            = 0xFFFF0000;
    m_YColor            = 0xFF00FF00;
    m_ZColor            = 0xFF0000FF;
    m_PColor            = 0xFFEEEEEE;    
    m_SelColor          = 0xFFFFFF00;
    m_ArrowLen          = 0.15f;        
    m_MinSelDist        = 0.5f;  
    m_HeadLen           = 0.3f;         
    m_HeadR             = 0.06f;           
    m_SGizmoSide        = 20;      
    m_StartPos          = Vector3D::null;
    m_StartMX           = 0;
    m_StartMY           = 0;
    m_InitPos           = Vector3D::null;      
    m_CurPos            = Vector3D::null;
    m_pTM               = NULL;
    m_HMarkerColor      = 0xAAFF00FF;
} // TranslateTool::TranslateTool

void TranslateTool::SetPosition( const Vector3D& pos )
{
    m_InitPos   = pos;
    m_CurPos    = pos;
    m_StartPos  = pos;
    m_DragMode  = dmNone;
} // TranslateTool::SetPosition

void TranslateTool::BindTM( Matrix4D* pTM )
{
    if (!pTM) { m_pTM = NULL; return; }
    SetPosition( pTM->getTranslation() );
    m_pTM = pTM;
} // TranslateTool::BindNode

void TranslateTool::UnbindNode()
{
    m_pTM = NULL;
}

float TranslateTool::GetWorldFrame( Vector3D&x, Vector3D& y, Vector3D& z )
{
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pTM) return 0.0f;
    Vector4D vs( m_CurPos );
    pCam->ToSpace( sWorld, sProjection,  vs ); vs.x += m_ArrowLen;
    pCam->ToSpace( sProjection, sWorld,  vs ); vs -= m_CurPos;
    float len = vs.norm();
    x = Vector3D( len, 0, 0 );
    y = Vector3D( 0, len, 0 );
    z = Vector3D( 0, 0, len );
    return len;
} // TranslateTool::GetWorldFrame

void TranslateTool::Render()
{
    if (!IsVisible()) return;

    ICamera* pCam = GetCamera();
    if (!pCam || !m_pTM) return;

    //  calculate world-space length of the gizmo arrows 
    //  (to keep constant length in screen space)
    Vector3D dX, dY, dZ;
    GetWorldFrame( dX, dY, dZ );

    //  draw arrows
    rsEnableZ( false );
    IRS->ResetWorldTM();
    DWORD clrX = (m_DragMode == dmDragX) ? m_SelColor : m_XColor;
    DWORD clrY = (m_DragMode == dmDragY) ? m_SelColor : m_YColor;
    DWORD clrZ = (m_DragMode == dmDragZ) ? m_SelColor : m_ZColor;
    DWORD clrP = (m_DragMode == dmDragCenter) ? m_SelColor : m_PColor;
    DrawArrow( m_CurPos, dX, clrX, 1.0f, m_HeadLen, m_HeadR );
    DrawArrow( m_CurPos, dY, clrY, 1.0f, m_HeadLen, m_HeadR );
    DrawArrow( m_CurPos, dZ, clrZ, 1.0f, m_HeadLen, m_HeadR );

    //  draw screen space drag gizmo
    Vector4D spos( m_CurPos );
    Vector4D gpos( Vector3D( m_CurPos.x, m_CurPos.y, 0.0f ) );
    pCam->SetViewport( IRS->GetViewPort() );
    pCam->ToSpace( sWorld, sScreen,  spos );
    pCam->ToSpace( sWorld, sScreen,  gpos );
    float x = spos.x - m_SGizmoSide*0.5f;
    float y = spos.y - m_SGizmoSide*0.5f;
    x = ceilf( x ); y = ceilf( y );
    Rct rct( x, y, m_SGizmoSide, m_SGizmoSide );
    rsFrame( rct, 0, clrP );

    //  height marker
    x = gpos.x - m_SGizmoSide*0.1f;
    y = gpos.y - m_SGizmoSide*0.1f;
    x = ceilf( x ); y = ceilf( y );
    rct = Rct( x, y, m_SGizmoSide*0.2f, m_SGizmoSide*0.2f );
    rsRect( rct, 0, m_HMarkerColor );
    rsLine( m_CurPos, Vector3D( m_CurPos.x, m_CurPos.y, 0.0f ), m_HMarkerColor, m_HMarkerColor );
    
    rsFlush();
} // TranslateTool::Render

void TranslateTool::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "TranslateTool", this );
    pm.f( "DragMode",       m_DragMode, NULL, true );
    pm.f( "XColor",         m_XColor, "color"   );
    pm.f( "YColor",         m_YColor, "color"   );
    pm.f( "ZColor",         m_ZColor, "color"   );
    pm.f( "SColor",         m_PColor, "color"   );         
    pm.f( "ArrowLen",       m_ArrowLen          );
    pm.f( "MinSelDist",     m_MinSelDist        );
    pm.f( "HeadLen",        m_HeadLen           );
    pm.f( "HeadR",          m_HeadR             );
    pm.f( "SGizmoSide",     m_SGizmoSide        );
    pm.f( "HMarkerColor",   m_HMarkerColor      );
} // TranslateTool::Expose

bool TranslateTool::OnMouseLBDown( int mX, int mY )
{
    if (!IsVisible()) return false;
    if (GetKeyState( VK_MENU    ) < 0 || 
        GetKeyState( VK_CONTROL ) < 0 ||
        GetKeyState( VK_SHIFT   ) < 0) return false; 

    ICamera* pCam = GetCamera();
    if (!pCam || !m_pTM) return false;

    Ray3D ray = pCam->GetPickRay( mX, mY );

    Vector3D dX, dY, dZ;
    float len = GetWorldFrame( dX, dY, dZ );
    dX += m_CurPos;
    dY += m_CurPos;
    dZ += m_CurPos;

    float dx = ray.dist2ToPoint( dX );
    float dy = ray.dist2ToPoint( dY );
    float dz = ray.dist2ToPoint( dZ );
    float dp = ray.dist2ToPoint( m_CurPos );

    float dmin = tmin( dx, dy, dz, dp );
    float minSelDist = len*m_MinSelDist;
    minSelDist *= minSelDist;

    if (m_DragMode == dmNone) {m_StartMX = mX; m_StartMY = mY; m_StartPos = m_CurPos;}
    m_DragMode = dmNone;
    if (dx == dmin && dx < minSelDist) m_DragMode = dmDragX;
    if (dy == dmin && dy < minSelDist) m_DragMode = dmDragY;
    if (dz == dmin && dz < minSelDist) m_DragMode = dmDragZ;
    if (dp == dmin && dp < minSelDist) m_DragMode = dmDragCenter;
    return false;
} // TranslateTool::OnMouseLBDown

bool TranslateTool::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (!IsVisible()) return false;

    if ((keys & MK_LBUTTON) == 0) m_DragMode = dmNone;
    if (m_DragMode == dmNone) return false; 
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pTM) return false;

    Ray3D ray = pCam->GetPickRay( mX, mY );
    Vector4D vs( m_StartPos ); pCam->ToSpace( sWorld, sScreen,  vs );
    vs.x += mX - m_StartMX;
    vs.y += mY - m_StartMY;
    pCam->ToSpace( sScreen, sWorld,  vs );
    vs -= m_StartPos;
    vs.w = 0.0f;

    if (m_DragMode == dmDragX) 
    {
        vs.x = sign( vs.x ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.y = vs.z = 0; 
    }
    else if (m_DragMode == dmDragY) 
    {
        vs.y = sign( vs.y ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.x = vs.z = 0; 
    }
    else if (m_DragMode == dmDragZ) 
    {
        vs.z = sign( vs.z ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.x = vs.y = 0; 
    }

    m_CurPos.add( m_StartPos, vs );

    if (m_pTM) 
    {
        Matrix4D tm = *m_pTM;
        tm.setTranslation( m_CurPos );
        *m_pTM = tm;
    }
    return false;
} // TranslateTool::OnMouseMove

bool TranslateTool::OnMouseLBUp( int mX, int mY )
{
    if (!IsVisible()) return false;

    m_DragMode = dmNone;
    return false;
} // TranslateTool::OnMouseLBUp

/*****************************************************************************/
/*  ScaleTool implementation
/*****************************************************************************/
ScaleTool::ScaleTool()
{
    m_DragMode          = dmNone;
    m_XColor            = 0xFFFF0000;
    m_YColor            = 0xFF00FF00;
    m_ZColor            = 0xFF0000FF;
    m_PColor            = 0xFFEEEEEE;    
    m_SelColor          = 0xFFFFFF00;
    m_MinSelDist        = 0.5f;  
    m_ArrowLen          = 0.15f;   
    m_HeadLen           = 0.15f;         
    m_SGizmoSide        = 20;      
    m_StartScale        = Vector3D::null;
    m_StartMX           = 0;
    m_StartMY           = 0;
    m_InitScale         = Vector3D::one;      
    m_CurScale          = Vector3D::one;
    m_pTM               = NULL;
    m_bLockUniform      = false;
    m_HMarkerColor      = 0xAAFF00FF;
} // ScaleTool::ScaleTool

void ScaleTool::SetScale( const Vector3D& s )
{
    m_InitScale = s;
    m_CurScale  = s;
    m_StartScale= s;
    m_DragMode  = dmNone;
} // ScaleTool::SetPosition

void ScaleTool::BindTM( Matrix4D* pTM )
{
    if (!pTM) { m_pTM = NULL; return; }
    SetScale( Vector3D( 1.0f, 1.0f, 1.0f ) );//pTM->getScale() );
    m_pTM = pTM;
} // ScaleTool::BindNode

float ScaleTool::GetWorldFrame( Vector3D&x, Vector3D& y, Vector3D& z )
{
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pTM) return 0.0f;
    Vector4D c0 = m_pTM->getTranslation(); c0.w = 1.0f;
    Vector4D c1 = c0;
    pCam->ToSpace( sWorld, sProjection,  c1 ); c1.x += m_ArrowLen;
    c1.normW();
    pCam->ToSpace( sProjection, sWorld,  c1 );
    c1 -= c0;
    Vector4D c = m_pTM->getTranslation();
    float len = c1.norm();
    x = Vector3D( len, 0, 0 );
    y = Vector3D( 0, len, 0 );
    z = Vector3D( 0, 0, len );
    x += c; y += c; z += c;
    return len;
} // ScaleTool::GetWorldFrame

void ScaleTool::Render()
{
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pTM) return;

    //  calculate world-space length of the gizmo arrows  
    //  (to keep constant length in screen space)
    Vector3D dX, dY, dZ;
    float len = GetWorldFrame( dX, dY, dZ );
    Vector3D c = m_pTM->getTranslation();

    //  draw arrows
    rsEnableZ( false );
    IRS->ResetWorldTM();
    DWORD clrX = (m_DragMode == dmDragX     ) ? m_SelColor : m_XColor;
    DWORD clrY = (m_DragMode == dmDragY     ) ? m_SelColor : m_YColor;
    DWORD clrZ = (m_DragMode == dmDragZ     ) ? m_SelColor : m_ZColor;
    DWORD clrP = (m_DragMode == dmDragCenter) ? m_SelColor : m_PColor;
    
    float hside = len*m_HeadLen*0.5f;

    rsLine( c, dX, m_XColor );
    rsLine( c, dY, m_YColor );
    rsLine( c, dZ, m_ZColor );
    rsFlush();
   
    DrawSphere( Sphere( dX, hside ), clrX, 0, 32 );
    DrawSphere( Sphere( dY, hside ), clrY, 0, 32 );
    DrawSphere( Sphere( dZ, hside ), clrZ, 0, 32 );
    DrawSphere( Sphere( c,  hside ), clrP, 0, 32 );

    rsFlush();
} // ScaleTool::Render

void ScaleTool::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "ScaleTool", this );
    pm.f( "DragMode",       m_DragMode, NULL, true );
    pm.f( "LockUniform",    m_bLockUniform      );
    pm.f( "XColor",         m_XColor, "color"   );
    pm.f( "YColor",         m_YColor, "color"   );
    pm.f( "ZColor",         m_ZColor, "color"   );
    pm.f( "SColor",         m_PColor, "color"   );         
    pm.f( "MinSelDist",     m_MinSelDist        );
    pm.f( "ArrowLen",       m_ArrowLen          );
    pm.f( "HeadLen",        m_HeadLen           );
    pm.f( "SGizmoSide",     m_SGizmoSide        );
    pm.f( "HMarkerColor",   m_HMarkerColor      );
} // ScaleTool::Expose

bool ScaleTool::OnMouseLBDown( int mX, int mY )
{
    if (!IsVisible()) return false;
    if (GetKeyState( VK_MENU    ) < 0 || 
        GetKeyState( VK_CONTROL ) < 0 ||
        GetKeyState( VK_SHIFT   ) < 0) return false; 

    ICamera* pCam = GetCamera();
    if (!pCam || !m_pTM) return false;

    Ray3D ray = pCam->GetPickRay( mX, mY );

    Vector3D dX, dY, dZ;
    float len = GetWorldFrame( dX, dY, dZ );

    float dx = ray.dist2ToPoint( dX );
    float dy = ray.dist2ToPoint( dY );
    float dz = ray.dist2ToPoint( dZ );
    float dp = ray.dist2ToPoint( m_pTM->getTranslation() );

    float dmin = tmin( dx, dy, dz, dp );
    float minSelDist = len*m_MinSelDist;
    minSelDist *= minSelDist;

    if (m_DragMode == dmNone) {m_StartMX = mX; m_StartMY = mY; m_StartScale = m_CurScale;}
    m_DragMode = dmNone;
    if (dx == dmin && dx < minSelDist) m_DragMode = dmDragX;
    if (dy == dmin && dy < minSelDist) m_DragMode = dmDragY;
    if (dz == dmin && dz < minSelDist) m_DragMode = dmDragZ;
    if (dp == dmin && dp < minSelDist) m_DragMode = dmDragCenter;
    if (m_DragMode != dmNone && m_bLockUniform) m_DragMode = dmDragCenter;
    Vector3D trans;
    Quaternion rot;
    m_pTM->Factorize( m_CurScale, rot, trans );
    return false;
} // ScaleTool::OnMouseLBDown

bool ScaleTool::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (!IsVisible()) return false;

    if ((keys & MK_LBUTTON) == 0) m_DragMode = dmNone;
    if (m_DragMode == dmNone) return false; 
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pTM) return false;

    Ray3D ray = pCam->GetPickRay( mX, mY );
    Vector4D vs( m_pTM->getTranslation() ); pCam->ToSpace( sWorld, sScreen,  vs );
    vs.x += mX - m_StartMX;
    vs.y += mY - m_StartMY;
    pCam->ToSpace( sScreen, sWorld,  vs );
    vs.w = 0.0f;

    if (m_DragMode == dmDragX) 
    {
        vs.x = sign( vs.x ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.y = vs.z = 0; 
    }
    else if (m_DragMode == dmDragY) 
    {
        vs.y = sign( vs.y ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.x = vs.z = 0; 
    }
    else if (m_DragMode == dmDragZ) 
    {
        vs.z = sign( vs.z ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.x = vs.y = 0; 
    }
    else if (m_DragMode == dmDragCenter)
    {
        vs.x = sign( vs.x ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.y = vs.z = vs.x; 
    }

    vs /= 1000.0f;
    m_CurScale += vs;

    if (m_pTM) 
    {
        Matrix4D& tm = *m_pTM;
        Vector3D scale;
        Quaternion rot;
        Vector3D trans;
        tm.Factorize( scale, rot, trans );
        tm = Matrix4D( m_CurScale, rot, trans );
    }
    return false;
} // ScaleTool::OnMouseMove

bool ScaleTool::OnMouseLBUp( int mX, int mY )
{
    if (!IsVisible()) return false;

    m_DragMode = dmNone;
    return false;
} // ScaleTool::OnMouseLBUp


/*****************************************************************************/
/*  RotateTool implementation
/*****************************************************************************/
RotateTool::RotateTool()
{
    m_DragMode          = dmNone;
    m_XColor            = 0xFFFF0000;
    m_YColor            = 0xFF00FF00;
    m_ZColor            = 0xFF0000FF;
    m_PColor            = 0xAA00FFFF;    
    m_SelColor          = 0xFFFFFF00;
    m_ArrowLen          = 0.1f;        
    m_MinSelDist        = 0.5f;  
    m_HeadLen           = 0.2f;         
    m_StartPos          = Vector3D::null;
    m_StartMX           = 0;
    m_StartMY           = 0;
    m_InitPos           = Vector3D::null;      
    m_CurPos            = Vector3D::null;
} // RotateTool::RotateTool

void RotateTool::SetPosition( const Vector3D& pos )
{
    m_InitPos   = pos;
    m_CurPos    = pos;
    m_StartPos  = pos;
    m_DragMode  = dmNone;
} // RotateTool::SetPosition

void RotateTool::BindNode( TransformNode* pNode )
{
    if (!pNode) { m_pNode = NULL; return; }
    SetPosition( pNode->GetWorldTM().getTranslation() );
    m_pNode = pNode;
} // RotateTool::BindNode

void RotateTool::UnbindNode()
{
    m_pNode = NULL;
}

float RotateTool::GetWorldFrame( Vector3D&x, Vector3D& y, Vector3D& z )
{
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pNode) return 0.0f;
    Vector4D vs( m_CurPos );
    pCam->ToSpace( sWorld, sProjection,  vs ); vs.x += m_ArrowLen;
    pCam->ToSpace( sProjection, sWorld,  vs ); vs -= m_CurPos;
    float len = vs.norm();
    x = Vector3D( len, 0, 0 );
    y = Vector3D( 0, len, 0 );
    z = Vector3D( 0, 0, len );
    return len;
} // RotateTool::GetWorldFrame

void RotateTool::Render()
{
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pNode) return;

    //  calculate world-space length of the gizmo arrows  
    //  (to keep constant length in screen space)
    Vector3D dX, dY, dZ;
    float len = GetWorldFrame( dX, dY, dZ );

    //  draw arrows
    rsEnableZ( false );
    IRS->ResetWorldTM();
    DWORD clrX = (m_DragMode == dmDragX     ) ? m_SelColor : m_XColor;
    DWORD clrY = (m_DragMode == dmDragY     ) ? m_SelColor : m_YColor;
    DWORD clrZ = (m_DragMode == dmDragZ     ) ? m_SelColor : m_ZColor;
    DWORD clrP = (m_DragMode == dmDragCenter) ? m_SelColor : m_PColor;
    
    float hside = len*m_HeadLen*0.5f;

    rsLine( m_CurPos, dX, m_XColor );
    rsLine( m_CurPos, dY, m_YColor );
    rsLine( m_CurPos, dZ, m_ZColor );
   
    DrawAABB( AABoundBox( dX, hside ), clrX, 0 );
    DrawAABB( AABoundBox( dY, hside ), clrY, 0 );
    DrawAABB( AABoundBox( dZ, hside ), clrZ, 0 );
    DrawAABB( AABoundBox( m_CurPos, hside ), clrP, 0 );

    rsFlushLines2D();
    rsFlushLines3D();
    rsFlushPoly3D();

} // RotateTool::Render

void RotateTool::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "RotateTool", this );
    pm.f( "DragMode",   m_DragMode, NULL, true );
    pm.f( "XColor",     m_XColor, "color"   );
    pm.f( "YColor",     m_YColor, "color"   );
    pm.f( "ZColor",     m_ZColor, "color"   );
    pm.f( "SColor",     m_PColor, "color"   );         
    pm.f( "ArrowLen",   m_ArrowLen          );
    pm.f( "MinSelDist", m_MinSelDist        );
    pm.f( "HeadLen",    m_HeadLen           );
} // RotateTool::Expose

bool RotateTool::OnMouseLBDown( int mX, int mY )
{
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pNode) return false;

    Ray3D ray = pCam->GetPickRay( mX, mY );

    Vector3D dX, dY, dZ;
    float len = GetWorldFrame( dX, dY, dZ );
    dX += m_CurPos;
    dY += m_CurPos;
    dZ += m_CurPos;

    float dx = ray.dist2ToPoint( dX );
    float dy = ray.dist2ToPoint( dY );
    float dz = ray.dist2ToPoint( dZ );
    float dp = ray.dist2ToPoint( m_CurPos );

    float dmin = tmin( dx, dy, dz, dp );
    float minSelDist = len*m_MinSelDist;
    minSelDist *= minSelDist;

    if (m_DragMode == dmNone) {m_StartMX = mX; m_StartMY = mY; m_StartPos = m_CurPos;}
    m_DragMode = dmNone;
    if (dx == dmin && dx < minSelDist) m_DragMode = dmDragX;
    if (dy == dmin && dy < minSelDist) m_DragMode = dmDragY;
    if (dz == dmin && dz < minSelDist) m_DragMode = dmDragZ;
    if (dp == dmin && dp < minSelDist) m_DragMode = dmDragCenter;
    return false;
} // RotateTool::OnMouseLBDown

bool RotateTool::OnMouseMove( int mX, int mY, DWORD keys )
{
    if ((keys & MK_LBUTTON) == 0) m_DragMode = dmNone;
    if (m_DragMode == dmNone) return false; 
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pNode) return false;

    Ray3D ray = pCam->GetPickRay( mX, mY );
    Vector4D vs( m_StartPos ); pCam->ToSpace( sWorld, sScreen,  vs );
    vs.x += mX - m_StartMX;
    vs.y += mY - m_StartMY;
    pCam->ToSpace( sScreen, sWorld,  vs );
    vs -= m_StartPos;
    vs.w = 0.0f;

    if (m_DragMode == dmDragX) 
    {
        vs.x = sign( vs.x ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.y = vs.z = 0; 
    }
    else if (m_DragMode == dmDragY) 
    {
        vs.y = sign( vs.y ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.x = vs.z = 0; 
    }
    else if (m_DragMode == dmDragZ) 
    {
        vs.z = sign( vs.z ) * tmax( fabs( vs.x ), fabs( vs.y ), fabs( vs.z ) );
        vs.x = vs.y = 0; 
    }

    m_CurPos.add( m_StartPos, vs );

    if (m_pNode) 
    {
        Matrix4D tm = m_pNode->GetWorldTM();
        tm.setTranslation( m_CurPos );
        m_pNode->SetWorldTM( tm );
    }
    return false;
} // RotateTool::OnMouseMove

bool RotateTool::OnMouseLBUp( int mX, int mY )
{
    m_DragMode = dmNone;
    return false;
} // RotateTool::OnMouseLBUp


