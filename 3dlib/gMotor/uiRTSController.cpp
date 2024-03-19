/*****************************************************************************/
/*    File:    uiRTSController.cpp
/*    Desc:    Maya-like camera controller
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "ITerrain.h"
#include "ICamera.h"

#include "uiRTSController.h"
/*****************************************************************************/
/*    RTSController implementation
/*****************************************************************************/
IMPLEMENT_CLASS(RTSController);
RTSController::RTSController()
{
    m_Mx                = -1;
    m_My                = -1;

    m_bSnapToGround     = true;
    m_MoveSpeed         = 2000.0f;
    m_RotSpeed          = 4.0f;

    m_DivePos           = 1.0f;
    m_DiveSpeed         = 0.8f;

    m_DiveStartAng      = 0.0f;
    m_DiveEndAng        = c_PI/3.0f;
    m_DiveStartH        = 100.0f;
    m_DiveEndH          = 2000.0f;
    m_Dive              = 0.0f;
    m_LookAtDistance    = 2500.0f;
    m_DragRotation      = 0.0f;
    m_bDragRotation     = false;

    AlignToParent();
    SetFocusable( false );
} // RTSController::RTSController

void RTSController::Serialize( OutStream& os ) const
{
} // RTSController::Serialize

void RTSController::Unserialize( InStream& is )
{
} // RTSController::Unserialize

bool RTSController::OnMouseLBDown( int mX, int mY ) 
{ 
    return false;
} // RTSController::OnMouseLBDown

bool RTSController::OnMouseMBDown( int mX, int mY ) 
{ 
    return false;
} // RTSController::OnMouseMBDown

bool RTSController::OnMouseRBDown( int mX, int mY ) 
{ 
    if (GetKeyState( VK_MENU ) < 0) m_bDragRotation = true;
    return false;
} // RTSController::OnMouseRBDown

void RTSController::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "RTSController", this );
} // RTSController::Expose

bool RTSController::OnKeyDown( DWORD keyCode, DWORD flags )
{
    return false;
}

bool RTSController::OnKeyUp( DWORD keyCode, DWORD flags )
{
    return false;
}

void RTSController::Render()
{
    if (!IsEnabled()) return;
    UpdateCameraPosition();
} // RTSController::Render

void RTSController::UpdateCameraPosition()
{
    POINT pt;
    GetCursorPos( &pt );
    int mx = pt.x;
    int my = pt.y;

    float dive = 0;
    if (GetKeyState( VK_NEXT ) < 0 || GetKeyState( VK_PRIOR ) < 0)
    {
        dive = m_Timer.seconds();
        if (GetKeyState( VK_NEXT ) < 0) dive = -dive;
    }
    m_Dive = dive*m_DiveSpeed;

    ICamera* pCamera = GetCamera(); 
    if (!pCamera) return;
    Matrix4D deltaTM( Matrix4D::identity );
    Matrix4D camTM = pCamera->GetWorldTM();

    Vector3D trans = Vector3D::null;
    Vector3D forward = camTM.getV2();
    Vector3D right   = camTM.getV0();
    float    rot     = m_DragRotation;
    m_DragRotation   = 0.0f;

    m_DivePos += m_Dive;
    m_Dive *= 0.10f;
    clamp( m_DivePos, 0.0f, 1.0f );

    if (GetKeyState( VK_UP    ) < 0) trans += forward; 
    if (GetKeyState( VK_DOWN  ) < 0) trans -= forward;

    if (GetKeyState( VK_LEFT  ) < 0) rot -= 1.0f;
    if (GetKeyState( VK_RIGHT ) < 0) rot += 1.0f;

    Rct ext = GetScreenExtents();
    const float c_ExtBorder = 5.0f;
    if (my <= ext.y + c_ExtBorder)         trans += forward;  
    if (my >= ext.y + ext.h - c_ExtBorder) trans -= forward;  

    if (mx <= ext.x + c_ExtBorder)         trans -= right;
    if (mx >= ext.x + ext.w - c_ExtBorder) trans += right;

    trans.normalize();
    double dt = tmin<double>( m_Timer.seconds(), 0.1 );
    
    trans   *= m_MoveSpeed*dt*(1.0f + m_DivePos);
    rot     *= m_RotSpeed*dt;

    const float c_RotationRecoil = 100.0f;
    trans.addWeighted( forward, -fabs( rot )*c_RotationRecoil );

    Vector3D pivot( -camTM.e30, -camTM.e31, -camTM.e32 );
    pivot.addWeighted( Vector3D( -camTM.e20, -camTM.e21, -camTM.e22 ), m_LookAtDistance );
    deltaTM.translation( pivot );

    Matrix4D rotTM; rotTM.rotation( Vector3D::oZ, rot );
    deltaTM *= rotTM;
    deltaTM.e30 += trans.x - pivot.x;
    deltaTM.e31 += trans.y - pivot.y;
    deltaTM.e32 += trans.z - pivot.z;

    camTM *= deltaTM;
    
    Vector3D vy = ITerra->GetNormal( camTM.e30, camTM.e31 );
    Vector3D vz( camTM.e02, camTM.e12, camTM.e22 );
    Vector3D vx( camTM.e00, camTM.e10, camTM.e20 );
    vz.z = -sqrtf( vz.x*vz.x + vz.y*vz.y )*tanf( m_DiveStartAng + m_DivePos*(m_DiveEndAng - m_DiveStartAng) );
    vz.normalize();
    vy.cross( vz, vx );
    vy.normalize();
    vx.cross( vy, vz );

    camTM.e00 = vx.x; camTM.e01 = vy.x; camTM.e02 = vz.x;
    camTM.e10 = vx.y; camTM.e11 = vy.y; camTM.e12 = vz.y;
    camTM.e20 = vx.z; camTM.e21 = vy.z; camTM.e22 = vz.z;

    pCamera->SetWorldTM( camTM );

    //  correct camera height
    if (m_bSnapToGround)
    {        
        pCamera->SetWorldTM( camTM );
        Vector4D cl( -1.0f, -1.0f, 0.0f, 1.0f );
        Vector4D cr( 1.0f, -1.0f, 0.0f, 1.0f );
        Vector4D cc( 0.0f, -1.0f, 0.0f, 1.0f );
        pCamera->ToSpace( sProjection, sWorld,  cr );
        pCamera->ToSpace( sProjection, sWorld,  cl );
        pCamera->ToSpace( sProjection, sWorld,  cc );

        float HL = ITerra->GetH( cl.x, cl.y );
        float HR = ITerra->GetH( cr.x, cr.y );
        float HC = ITerra->GetH( cc.x, cc.y );
        
        float H = tmax( HL, HR, HC );
        camTM.e32 += H - tmin( cl.z, cc.z, cr.z );
    }

    camTM.e32 += m_DiveStartH + (m_DiveEndH - m_DiveStartH)*m_DivePos;
    pCamera->SetWorldTM( camTM );

    m_Timer.reset();
    m_Timer.start();
} // RTSController::UpdateCameraPosition

bool RTSController::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (m_bDragRotation) m_DragRotation = float( m_Mx - mX )*0.04f;
    m_Mx = mX;
    m_My = mY;

    return false;
} // RTSController::OnMouseMove

bool RTSController::OnMouseLBDbl( int mX, int mY )
{
    //  pick point we want to focus onto
    Vector3D pt;
    ITerra->Pick( mX, mY, pt );
    
    return false;
} // RTSController::OnMouseLBDbl


