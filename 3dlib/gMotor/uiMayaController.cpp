/*****************************************************************************/
/*    File:    uiMayaController.cpp
/*    Desc:    Maya-like camera controller
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "ICamera.h"
#include "uiWidget.h"
#include "uiMayaController.h"

/*****************************************************************************/
/*    MayaController implementation
/*****************************************************************************/
IMPLEMENT_CLASS(MayaController);
MayaController::MayaController()
{

    m_Mx                = -1;
    m_My                = -1;
    m_Mode                = cmIdle;
    m_bLockRotate        = false;
    m_bLockPan            = false;
    m_bLockZoom            = false;
	m_IsInverted = false;

    AlignToParent();
    SetFocusable( false );
    SetName( "MayaController" );
} // MayaController::MayaController

Vector3D MayaController::ScreenToVector( float x, float y )
{
    Rct vp = IRS->GetViewPort();
    Vector3D res; res.zero();
    const float c_RotateRadius = 5.0f;

    float halfW = vp.w*0.5f;
    float halfH = vp.h*0.5f;
    res.x = -(x - vp.x - halfW)/(c_RotateRadius * halfW);
    res.y = -(y - vp.y - halfH)/(c_RotateRadius * halfH);
    res.z = 0.0f;

    res.z   = 0.0f;
    float mag = res.norm2();

    if (mag > 1.0f)
    {
        float scale = 1.0f / sqrt( mag );
        res.x *= scale;
        res.y *= scale;
    }
    else res.z = sqrt( 1.0f - mag );

    return res;
} // MayaController::ScreenToVector

void MayaController::Zoom()
{
    m_Mode = cmZoom;
} // MayaController::Zoom

void MayaController::Pan()
{
    m_Mode = cmPan;
} // MayaController::Pan

void MayaController::Rotate()
{
    m_Mode = cmRotate;
} // MayaController::Rotate

void MayaController::Idle()
{
    m_Mode = cmIdle;
} // MayaController::Idle

void MayaController::Serialize( OutStream& os ) const
{
} // MayaController::Serialize

void MayaController::Unserialize( InStream& is )
{
} // MayaController::Unserialize

bool MayaController::OnMouseLBDown( int mX, int mY ) 
{ 
    if (GetKeyState( VK_MENU ) >= 0) return false;
    Rotate(); 
    return false;
} // MayaController::OnMouseLBDown

bool MayaController::OnMouseMBDown( int mX, int mY ) 
{ 
    if (GetKeyState( VK_MENU ) >= 0) return false;
    Pan(); 
    return false;
} // MayaController::OnMouseMBDown

bool MayaController::OnMouseRBDown( int mX, int mY ) 
{ 
    if (GetKeyState( VK_MENU ) >= 0) return false;
    Zoom(); 
    return false;
} // MayaController::OnMouseRBDown

void MayaController::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "MayaController", this );
    pm.f( "LockRotate",        m_bLockRotate    );
    pm.f( "LockPan",        m_bLockPan        );
    pm.f( "LockZoom",        m_bLockZoom        );
} // MayaController::Expose

inline void ScreenToProjectionSpace( float& x, float& y, const Rct& vp ) 
{
    x -= vp.x;            
    x /= vp.w * 0.5f;    
    x -= 1.0f;            

    y -= vp.y;
    y /= vp.h * 0.5f;
    y = 1.0f - y;
} // ScreenToProjectionSpace

bool MayaController::OnMouseMove( int mX, int mY, DWORD keys )
{
    float x1 = m_Mx;
    float y1 = m_My;
    float x2 = mX;
    float y2 = mY;

    ICamera* pCamera = GetCamera(); 
    if (!pCamera) return false;    
    Matrix4D camTM = pCamera->GetWorldTM();	
    Vector4D worldCenter( 0.0f, 0.0f, 0.0f, 1.0f );
    pCamera->ToSpace( sWorld, sProjection,  worldCenter );

    //  transform 
    Rct ext = m_Extents;
    ScreenToProjectionSpace( x1, y1, m_Extents );
    ScreenToProjectionSpace( x2, y2, m_Extents );

	Matrix4D deltaTM( Matrix4D::identity );

    if (m_Mode == cmRotate && !m_bLockRotate)
    {
        Vector4D center( 0.0f, 0.0f, worldCenter.z, 1.0f );
        Vector3D dir = pCamera->GetDir();
        pCamera->ToSpace( sProjection, sWorld,  center );

        //  vertical mouse movement pitches camera around axis, parallel
        //  to the camera right axis, and going through point of interest
        Matrix4D pitchTM;
        Line3D pitchAxis( center, Vector3D( camTM.e00, camTM.e01, camTM.e02 ) );

		float pitchAng = asin( (y2 - y1) * 2.0f * (m_IsInverted ? 1.0f : - 1.0f));
        float pitch = asin( sqrtf( (dir.x*dir.x + dir.y*dir.y)/dir.norm2() ) );
        Vector3D ypr = camTM.EulerXYZ();        
        
		pitchTM.rotation( pitchAxis, pitchAng );

        deltaTM *= pitchTM;

        //  horizontal mouse movement rotates camera around axis, parallel
        //  to the world oZ axis, and going through point of interest
        Matrix4D rollTM;
		float rollAng = asin( (x1 - x2) * 2.0f * (m_IsInverted ? 1.0f : - 1.0f));

        Line3D rollAxis( center, Vector3D::oZ );
		rollTM.rotation( rollAxis, rollAng);

        deltaTM *= rollTM;
    }
    else if (m_Mode == cmPan && !m_bLockPan)
    {
        //  panning maps screen space mouse movement to the camera movement in the 
        //  plane, parallel to the clipping planes
        Vector4D w1( x1, y1, worldCenter.z, 1.0f );
        Vector4D w2( x2, y2, worldCenter.z, 1.0f );
        pCamera->ToSpace( sProjection, sWorld,  w1 );
        pCamera->ToSpace( sProjection, sWorld,  w2 );
        w1 -= w2;
        Matrix4D panTM;
        panTM.translation( w1 );
        deltaTM *= panTM;
    }
    else if (m_Mode == cmZoom && !m_bLockZoom)
    {
        //  zooming moves camera forward/backward for perspective camera
        //  and changes view volume width for ortho camera
        float dz = x1 - x2;
        if (fabs( y2 - y1 ) > fabs( dz )) dz = y2 - y1;    
        Vector4D w1( 0.0f, 0.0f, 0.7f - dz, 1.0f );
        Vector4D w2( 0.0f, 0.0f, 0.7f + dz, 1.0f );
        pCamera->ToSpace( sProjection, sWorld,  w1 );
        pCamera->ToSpace( sProjection, sWorld,  w2 );
        w1 -= w2;
		w1*=4;

        Matrix4D zoomTM( Matrix4D::identity );
        //if (pCamera->HasFn( PerspCamera::Magic() ))
        {
            zoomTM.translation( w1 );
        }
        /*else if (pCamera->HasFn( OrthoCamera::Magic() ))
        {
            OrthoCamera* pOrtho = (OrthoCamera*)pCamera;
            float w = pOrtho->GetViewVolW();
            w += w1.z;
            if (w > 0.0f) pOrtho->SetViewVolW( w );
        }*/
        deltaTM *= zoomTM;
    }

    camTM *= deltaTM;
    pCamera->SetWorldTM( camTM );
    m_Mx = mX;
    m_My = mY;

    return false;
} // MayaController::OnMouseMove

