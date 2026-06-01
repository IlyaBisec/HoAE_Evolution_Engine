/*****************************************************************************/
/*    File:    vCamera.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    
/*****************************************************************************/
#include "stdafx.h"
#include "vCamera.h"
#include "kAssert.h"

ICamera* g_pActiveCamera = NULL;
ICamera* GetCamera()
{
    return g_pActiveCamera;
}

void SetCamera( ICamera* pCamera )
{
    g_pActiveCamera = pCamera;
}

IMPLEMENT_CLASS(OrthoCamera);
IMPLEMENT_CLASS(PerspCamera);
/*****************************************************************************/
/*  Camera implementation
/*****************************************************************************/
Camera::Camera()
{
    m_ViewPort.Set( 0.0f, 0.0f, 1024.0f, 768.0f );

    m_SpaceTM[sWorld][sWorld]           = &Matrix4D::identity;
    m_SpaceTM[sWorld][sView]            = &m_ViewTM;
    m_SpaceTM[sWorld][sProjection]      = &m_WorldProjTM;
    m_SpaceTM[sWorld][sScreen]          = &m_FullTM;

    m_SpaceTM[sView][sWorld]            = &m_WorldTM;
    m_SpaceTM[sView][sView]             = &Matrix4D::identity;
    m_SpaceTM[sView][sProjection]       = &m_ProjTM;
    m_SpaceTM[sView][sScreen]           = &m_ViewScreenTM;

    m_SpaceTM[sProjection][sWorld]      = &m_ProjWorldTM;
    m_SpaceTM[sProjection][sView]       = &m_InvProjTM;
    m_SpaceTM[sProjection][sProjection] = &Matrix4D::identity;
    m_SpaceTM[sProjection][sScreen]     = &m_ScreenTM;

    m_SpaceTM[sScreen][sWorld]          = &m_InvFullTM;
    m_SpaceTM[sScreen][sView]           = &m_ScreenViewTM;
    m_SpaceTM[sScreen][sProjection]     = &m_ScreenProjTM;
    m_SpaceTM[sScreen][sScreen]         = &Matrix4D::identity;

    m_WorldTM.setIdentity();
} // Camera::Camera

Line3D Camera::GetPickRay( float curX, float curY )
{
    Line3D ray;
    Vector4D orig( curX, curY, 0.0f, 1.0f );
    Vector4D dir ( curX, curY, 1.0f, 1.0f );
    ToSpace( sScreen, sWorld, orig );
    ToSpace( sScreen, sWorld, dir );

    orig.normW();
    dir.normW();
    dir -= orig;
    dir.normalize();
    ray.setOrigDir( orig, dir );
    return ray; 
} // Camera::GetPickRay

void Camera::Serialize( OutStream& os ) const
{

}

void Camera::Unserialize( InStream&    is )
{

}

void Camera::Expose( PropertyMap& pm )
{
    
}

void Camera::Render()
{
    IRS->SetViewPort( m_ViewPort );
    IRS->SetViewTM( m_ViewTM );
    IRS->SetProjTM( m_ProjTM );
	Matrix4D wvpTM( IRS->GetWorldTM() );
	wvpTM *= IRS->GetViewTM();
	wvpTM *= IRS->GetProjTM();
	//wvpTM.transpose();
	IRS->SetWorldViewProjTM(wvpTM);
    SetCamera( this );
} // Camera::Render

void Camera::SetOrientation( const Vector3D& right, const Vector3D& up, const Vector3D& dir )
{
    Vector3D vx( right ), vy( up ), vz( dir );
    vz.orthonormalize( vy, vx );
    m_WorldTM.e00 = vx.x; m_WorldTM.e01 = vx.y; m_WorldTM.e02 = vx.z;
    m_WorldTM.e10 = vy.x; m_WorldTM.e11 = vy.y; m_WorldTM.e12 = vy.z;
    m_WorldTM.e20 = vz.x; m_WorldTM.e21 = vz.y; m_WorldTM.e22 = vz.z;
    Update();
} // Camera::SetOrientation

void Camera::GetOrientation( Vector3D& right, Vector3D& up, Vector3D& dir )
{
    right.x = m_WorldTM.e00; right.y = m_WorldTM.e01; right.z = m_WorldTM.e02;
    up.x    = m_WorldTM.e10; up.y    = m_WorldTM.e11; up.z    = m_WorldTM.e12;
    dir.x   = m_WorldTM.e20; dir.y   = m_WorldTM.e21; dir.z   = m_WorldTM.e22;
} // Camera::GetOrientation

Frustum Camera::GetPickFrustum( const Rct& rct )
{
    float bX = rct.x;
    float bY = rct.y;
    float eX = rct.x + rct.w;
    float eY = rct.y + rct.h;

    Vector4D lt( bX, bY, 0.0f, 1.0f );
    Vector4D lb( bX, eY, 0.0f, 1.0f );
    Vector4D rt( eX, bY, 0.0f, 1.0f );
    Vector4D RB( eX, eY, 1.0f, 1.0f );
    Vector4D LB( bX, eY, 1.0f, 1.0f );
    Vector4D RT( eX, bY, 1.0f, 1.0f );

    ToSpace( sScreen, sWorld,  lt );
    ToSpace( sScreen, sWorld,  lb );
    ToSpace( sScreen, sWorld,  rt );
    ToSpace( sScreen, sWorld,  RB );
    ToSpace( sScreen, sWorld,  LB );
    ToSpace( sScreen, sWorld,  RT );

    Frustum frustum;
    frustum.plNear.        from3Points( lt, rt, lb );
    frustum.plFar.        from3Points( LB, RB, RT );
    frustum.plLeft.        from3Points( LB, lt, lb );
    frustum.plRight.    from3Points( rt, RT, RB );
    frustum.plTop.        from3Points( lt, RT, rt );
    frustum.plBottom.    from3Points( lb, RB, LB );
    return frustum;
} // Camera::GetPickFrustum

void Camera::SetViewport( const Rct& rct )
{
    m_ViewPort = rct;
    Update();
} // Camera::SetViewport

void Camera::SetPosition( const Vector3D& pos )
{
    m_WorldTM.e30 = pos.x;
    m_WorldTM.e31 = pos.y;
    m_WorldTM.e32 = pos.z;
    Update();
} // Camera::SetPosition

void Camera::SetViewTM( const Matrix4D& m )
{
    m_WorldTM.affineInverse( m );
    Update();
} // Camera::SetViewTM

void Camera::SetProjTM( const Matrix4D& m )
{
    m_ProjTM = m;
    Update();
} // Camera::SetProjTM

void Camera::SetWorldTM( const Matrix4D& m )
{
    m_WorldTM = m;
    Update();
} // Camera::SetWorldTM

void Camera::Update()
{
    m_ViewTM.affineInverse( m_WorldTM );
    
    m_ScreenTM.e00 = m_ViewPort.w*0.5f; 
    m_ScreenTM.e01 = 0.0f; 
    m_ScreenTM.e02 = 0.0f; 
    m_ScreenTM.e03 = 0.0f;        

    m_ScreenTM.e10 = 0.0f; 
    m_ScreenTM.e11 = -m_ViewPort.h*0.5f; 
    m_ScreenTM.e12 = 0.0f; 
    m_ScreenTM.e13 = 0.0f; 

    m_ScreenTM.e20 = 0.0f; 
    m_ScreenTM.e21 = 0.0f; 
    m_ScreenTM.e22 = 1.0f; 
    m_ScreenTM.e23 = 0.0f;        
    
    m_ScreenTM.e30 = m_ViewPort.w*0.5f + m_ViewPort.x; 
    m_ScreenTM.e31 = m_ViewPort.h*0.5f + m_ViewPort.y; 
    m_ScreenTM.e32 = 0.0f; 
    m_ScreenTM.e33 = 1.0f;     

    m_ScreenProjTM.e00 = 2.0f/m_ViewPort.w; 
    m_ScreenProjTM.e01 = 0.0f; 
    m_ScreenProjTM.e02 = 0.0f; 
    m_ScreenProjTM.e03 = 0.0f;        

    m_ScreenProjTM.e10 = 0.0f; 
    m_ScreenProjTM.e11 = -2.0f/m_ViewPort.h;
    m_ScreenProjTM.e12 = 0.0f; 
    m_ScreenProjTM.e13 = 0.0f; 

    m_ScreenProjTM.e20 = 0.0f; 
    m_ScreenProjTM.e21 = 0.0f; 
    m_ScreenProjTM.e22 = 1.0f; 
    m_ScreenProjTM.e23 = 0.0f;        

    m_ScreenProjTM.e30 = -2.0f*m_ViewPort.x/m_ViewPort.w - 1.0f; 
    m_ScreenProjTM.e31 =  2.0f*m_ViewPort.y/m_ViewPort.h + 1.0f;
    m_ScreenProjTM.e32 = 0.0f; 
    m_ScreenProjTM.e33 = 1.0f;  

    m_InvProjTM.inverse( m_ProjTM );
    m_WorldProjTM.  mul( m_ViewTM, m_ProjTM );      
    m_ProjWorldTM.  mul( m_InvProjTM, m_WorldTM );      
    m_FullTM.       mul( m_WorldProjTM, m_ScreenTM );
    m_InvFullTM.    mul( m_ScreenProjTM, m_ProjWorldTM );
    m_ViewScreenTM. mul( m_ProjTM, m_ScreenTM );
    m_ScreenViewTM. mul( m_ScreenProjTM, m_InvProjTM );

    m_Frustum.FromCameraMatrix( m_WorldProjTM );
} // Camera::Update

void Camera::UnitTest()
{
    const float c_TestBias = 0.1f;
    Vector4D s, d; 
    d.random( -100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f );
    d.w = 1.0f;
    
    s = d;
    ToSpace( sWorld, sView, d );
    ToSpace( sView, sWorld, d );
    assert( s.isEqual( d, c_TestBias ) );

    s = d;
    ToSpace( sWorld, sScreen, d );
    ToSpace( sScreen, sWorld, d );
    assert( s.isEqual( d, c_TestBias ) );

    s = d;
    ToSpace( sWorld, sProjection, d );
    ToSpace( sProjection, sWorld, d );
    assert( s.isEqual( d, c_TestBias ) );

    s = d;
    ToSpace( sView, sProjection, d );
    ToSpace( sProjection, sView, d );
    assert( s.isEqual( d, c_TestBias ) );

    s = d;
    ToSpace( sView, sScreen, d );
    ToSpace( sScreen, sView, d );
    assert( s.isEqual( d, c_TestBias ) );

    s = d;
    ToSpace( sScreen, sProjection, d );
    ToSpace( sProjection, sScreen, d );
    assert( s.isEqual( d, c_TestBias ) );
} // Camera::Test

void Camera::SetDirection( const Vector3D& pos )
{
    m_WorldTM.e20 = pos.x;
    m_WorldTM.e21 = pos.y;
    m_WorldTM.e22 = pos.z;
}

Vector3D Camera::GetDirection() const
{
    return Vector3D( m_WorldTM.e20, m_WorldTM.e21, m_WorldTM.e22 );
}

void Camera::SetLookAt( const Vector3D& pos )
{
    Vector3D la = GetLookAt();
    float dist = la.distance( GetPosition() );
    Vector3D p( pos );
    p.addWeighted( GetDirection(), -dist );
    SetPosition( p );
}

Vector3D Camera::GetLookAt() const
{
    Vector3D dir = GetDirection();
    Vector3D pos = GetPosition();

    float t = -pos.z/dir.z;
    dir *= t;
    dir += pos;
    return dir;
} // Camera::GetLookAt

/*****************************************************************************/
/*  OrthoCamera implementation
/*****************************************************************************/
OrthoCamera::OrthoCamera()
{
    SetViewVolume( 1000.0f, 10.0f, 10000.0f, c_DefViewAspect );
    SetOrientation( Vector3D::oX, Vector3D::oZ, Vector3D::oY );
}

void OrthoCamera::Expose( PropertyMap& pm )
{
    Camera::Expose( pm );
} // OrthoCamera::Expose

void OrthoCamera::ShiftZ( float amount )
{
	return;//temporal hack-fix
    SetViewVolume( GetViewVolume(), GetZn() + amount, GetZf() + amount, GetAspect() );
    Render();
} // OrthoCamera::ShiftZ

/*****************************************************************************/
/*  PerspCamera implementation
/*****************************************************************************/
PerspCamera::PerspCamera()
{
    SetFOVx( c_HalfPI, 10.0f, 10000.0f, c_DefViewAspect );
    SetOrientation( Vector3D::oX, Vector3D::oZ, Vector3D::oY );
} // PerspCamera::PerspCamera

void PerspCamera::SetFOVx( float fovx, float zn, float zf, float aspect )
{
    float w = 2.0f * zn * tan( fovx * 0.5f );
    SetViewVolume( w, zn, zf, aspect );
} // PerspCamera::SetFOVx

void PerspCamera::SetFOVy( float fovy, float zn, float zf, float aspect )
{
    float h = 2.0f * zn * tan( fovy * 0.5f );
    float w = h*aspect;
    SetViewVolume( w, zn, zf, aspect );
} // PerspCamera::SetFOVy

void PerspCamera::SetViewVolume( float w, float zn, float zf, float aspect )
{
    assert( w > 0.0f && aspect >= 0.0f && zn > 0.0f && zf > zn );
    float h = w/aspect;
    Matrix4D tm;
    tm.e00 = 2*zn/w; tm.e01 = 0.0f;        tm.e02 = 0.0f;            tm.e03 = 0.0f;
    tm.e10 = 0.0f;     tm.e11 = 2*zn/h;    tm.e12 = 0.0f;            tm.e13 = 0.0f;
    tm.e20 = 0.0f;     tm.e21 = 0.0f;        tm.e22 = zf/(zf-zn);    tm.e23 = 1.0f;
    tm.e30 = 0.0f;     tm.e31 = 0.0f;        tm.e32 = zn*zf/(zn-zf);    tm.e33 = 0.0f;
    SetProjTM( tm );
} // PerspCamera::SetWH

float PerspCamera::GetViewVolume() const 
{ 
    assert( fabs( m_ProjTM.e00 ) > 0.0f ); 
    return 2.0f * GetZn() / m_ProjTM.e00; 
}

void PerspCamera::Expose( PropertyMap& pm )
{
    Camera::Expose( pm );
} // PerspCamera::Expose

void PerspCamera::ShiftZ( float amount )
{
	return;//temporal hack-fix
    SetFOVx( GetFOVx(), GetZn() + amount, GetZf() + amount, GetAspect() );
    Render();
} // PerspCamera::ShiftZ