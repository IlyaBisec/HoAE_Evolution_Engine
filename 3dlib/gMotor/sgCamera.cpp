/*****************************************************************************/
/*	File:	sgCamera.cpp
/*	Desc:	Camera-related routines
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-21-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgMovable.h"
#include "sgRoot.h"
#include "kIOHelpers.h"
#include "sgApplication.h"

#include "sgCamera.h"

#ifndef _INLINES
#include "sgCamera.inl"
#endif  // _INLINES

Matrix4D GetCameraTM()
{
    return sg::BaseCamera::GetActiveCamera()->GetTransform();
}

Matrix4D GetCameraProjTM()
{
    return sg::BaseCamera::GetActiveCamera()->GetProjM();
}

ICamera* GetCamera() 
{ 
    return sg::BaseCamera::GetActiveCamera(); 
}

BEGIN_NAMESPACE( sg )


/*****************************************************************************/
/*	Canvas implementation
/*****************************************************************************/
Canvas::Canvas() 
{
	m_Extents.Set( 0.0f, 0.0f, 1024.0f, 768.0f );
	m_Flags = 0;
	m_Color = 0; 
	SetAutoScale( true );
}

void Canvas::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Extents << m_Flags << m_Color;
} // Canvas::Serialize

void Canvas::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_Extents >> m_Flags >> m_Color;
} // Canvas::Unserialize

void Canvas::Render()
{
	bool bAutoScale = GetAutoScale();
	Rct ext = m_Extents;
	if (bAutoScale)
	{
		Rct cvp  = IRS->GetViewPort();
		float sw = cvp.w / 1024.0f;
		float sh = cvp.h / 768.0f;
		ext.x *= sw;
		ext.w *= sw;
		ext.y *= sh;
		ext.h *= sh;
	}

	IRS->SetViewPort( ext );
	bool bClearTarget  = GetViewportFlag( vfClearColor		);
	bool bClearDepth   = GetViewportFlag( vfClearDepth		);
	bool bClearStencil = GetViewportFlag( vfClearStencil	);

	if (bClearTarget || bClearDepth || bClearStencil)
	{
		IRS->ClearDevice( bClearTarget, m_Color, bClearDepth, bClearStencil );
	}
} // Canvas::Render

void Canvas::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Canvas", this );
	pm.f( "Left",			m_Extents.x	);
	pm.f( "Top",			m_Extents.y	);
	pm.f( "Width",			m_Extents.w	);
	pm.f( "Height",			m_Extents.h	);
	pm.f( "Back Color",		m_Color, "color" );
	pm.p( "Clear Color",	GetClearColor,	SetClearColor	);
	pm.p( "Clear Depth",	GetClearDepth,	SetClearDepth	);
	pm.p( "Clear Stencil",	GetClearStencil,SetClearStencil );
} // Canvas::Expose

/*****************************************************************************/
/*	BaseCamera implementation
/*****************************************************************************/
DWORD		BaseCamera::s_ActiveCamera  = c_BadID;
bool		BaseCamera::s_bFrozen		= false;

BaseCamera::BaseCamera()
{
	m_bTweakAspect = true;
} // BaseCamera::BaseCamera

BaseCamera::~BaseCamera()
{
}

void BaseCamera::Reset()
{
	tm.setIdentity();
	m_Proj.setIdentity();

} // BaseCamera::Reset

bool BaseCamera::SetDirUp( const Vector3D& dir,	const Vector3D& up )
{
	Vector3D right;
	right.cross( up, dir );
	
	return SetRightUpDir( right, up, dir );
} // BaseCamera::SetDirUp

void BaseCamera::SetLookAt( const Vector3D& v )
{
    Vector3D la = GetLookAt();
    float dist = la.distance( GetPos() );
    Vector3D pos( v );
    pos.addWeighted( GetDir(), -dist );
    SetPos( pos );
} // BaseCamera::SetLookAt

bool BaseCamera::SetDirRight( const Vector3D& dir,	const Vector3D& right )
{
	Vector3D up;
	up.cross( dir, right );

	return SetRightUpDir( right, up, dir );
} // BaseCamera::SetDirRight

bool BaseCamera::SetUpRight( const Vector3D& up, const Vector3D& right )
{
	Vector3D dir;
	dir.cross( up, right );

	return SetRightUpDir( right, up, dir );
} // BaseCamera::SetUpRight

void BaseCamera::SetYawPitchRoll( float yaw, float pitch, float roll )
{
	Quaternion qrot;
	qrot.FromEulerAngles( yaw, pitch, roll );
	
	Vector3D pos;
	GetPos( pos );
	tm.rotation( qrot );
	SetPos( pos );

} // BaseCamera::SetYPR

void BaseCamera::FaceToPt( const Vector3D& pt, const Vector3D& rotAxis )
{
	assert( false );
} // BaseCamera::FaceToPt

void BaseCamera::FaceToPt( const Vector3D& pt )
{
	Vector3D dir( pt );
	Vector3D pos, up;
	GetPos( pos );
	GetUp( up );
	dir -= pos;
	dir.normalize();

	SetDirUp( dir, up );
} // BaseCamera::FaceToPt

void BaseCamera::FaceToPt( const Vector3D& pt, float distance )
{
	assert( false );
} // BaseCamera::FaceToPt

void BaseCamera::GetCameraSpaceFrustum( Frustum& frustum ) const
{
	frustum.FromCameraMatrix( m_Proj );
} // BaseCamera::GetCameraSpaceFrustum

void BaseCamera::GetWorldSpaceFrustum( Frustum& frustum ) const
{
	frustum.FromCameraMatrix( GetViewProjM() );
} // BaseCamera::GetWorldSpaceFrustum

void BaseCamera::GetObjectSpaceFrustum( Frustum& frustum, const Matrix4D& objTM ) const
{
	Matrix4D m( objTM );
	Matrix4D cm = GetViewProjM();
	m *= cm;
	frustum.FromCameraMatrix( m );
} // BaseCamera::GetObjectSpaceFrustum

Matrix4D BaseCamera::WorldToScreenSpace( const Rct* pViewport ) const
{
	Matrix4D res = GetViewProjM(); 
	res *= ProjectionToScreenSpace( pViewport );
	return res;
} // BaseCamera::WorldToScreenSpace

Matrix4D BaseCamera::ScreenToWorldSpace( const Rct* pViewport ) const
{
    Matrix4D m = WorldToScreenSpace( pViewport );
    m.inverse();
    return m;
} // BaseCamera::ScreenToWorldSpace

Matrix4D BaseCamera::ProjectionToScreenSpace( const Rct* pViewport ) const
{
	Matrix4D tm( Matrix4D::identity );
	const Rct& vp = pViewport ? *pViewport : IRS->GetViewPort();

	tm.e00 =  vp.w * 0.5f;
	tm.e11 = -vp.h * 0.5f;
	tm.e30 =  vp.w * 0.5f;
	tm.e31 =  vp.h * 0.5f;

	return tm;
} // BaseCamera::ProjectionToScreenSpace

void BaseCamera::Interpolate( float t, const Vector3D& sLookAt, const Vector3D& sDir,
										const Vector3D& dLookAt, const Vector3D& dDir )
{
	Vector3D sUp = Vector3D::oZ;
	Vector3D dUp = Vector3D::oZ;

	Vector3D sDir_( sDir );
	Vector3D dDir_( dDir );

	Vector3D sRight, dRight; 
	sRight.cross( sUp, sDir );
	dRight.cross( dUp, dDir );

	Vector3D::orthonormalize( sDir_, sRight, sUp );
	Vector3D::orthonormalize( dDir_, dRight, dUp );

	Matrix3D sRot( sRight, sUp, sDir_ );
	Matrix3D dRot( dRight, dUp, dDir_ );

	Quaternion sRotQ( sRot );
	Quaternion dRotQ( dRot );

	Quaternion rotQ;
	rotQ.Slerp( sRotQ, dRotQ, t );
	Matrix3D rot( rotQ );
	Vector3D pos;
	pos.addWeighted( sLookAt, dLookAt, 1.0f - t, t ); 

	SetRightUpDir( rot.getV0(), rot.getV1(), rot.getV2() );	
	SetLookAt( pos );
} // BaseCamera::Interpolate

void BaseCamera::Interpolate( float t, const Vector3D& sLookAt, const Vector3D& dLookAt )
{
	Vector3D la;
	la.addWeighted( sLookAt, dLookAt, 1.0f - t, t ); 
	SetLookAt( la );
} // BaseCamera::Interpolate

void BaseCamera::GetPickFrustum( float bX, float bY, float eX, float eY, 
								Frustum& frustum )
{
	if (bX > eX) swap( bX, eX );
	if (bY > eY) swap( bY, eY );

	Vector4D lt( bX, bY, 0.0f, 1.0f );
	Vector4D lb( bX, eY, 0.0f, 1.0f );
	Vector4D rt( eX, bY, 0.0f, 1.0f );
	Vector4D RB( eX, eY, 1.0f, 1.0f );
	Vector4D LB( bX, eY, 1.0f, 1.0f );
	Vector4D RT( eX, bY, 1.0f, 1.0f );

	ScreenToWorldSpace( lt );
	ScreenToWorldSpace( lb );
	ScreenToWorldSpace( rt );
	ScreenToWorldSpace( RB );
	ScreenToWorldSpace( LB );
	ScreenToWorldSpace( RT );

	frustum.plNear.		from3Points( lt, rt, lb );
	frustum.plFar.		from3Points( LB, RB, RT );
	frustum.plLeft.		from3Points( LB, lt, lb );
	frustum.plRight.	from3Points( rt, RT, RB );
	frustum.plTop.		from3Points( lt, RT, rt );
	frustum.plBottom.	from3Points( lb, RB, LB );
} // BaseCamera::GetPickFrustum

void BaseCamera::GetPickRay( float curX, float curY, Line3D& ray, const Matrix4D* worldMatr )
{
	Vector4D orig( curX, curY, 0.0f, 1.0f );
	Vector4D dir ( curX, curY, 1.0f, 1.0f );
	ScreenToWorldSpace( orig );
	ScreenToWorldSpace( dir );

	orig.normW();
	dir.normW();

	if (worldMatr)
	{
		Matrix4D invW;
		invW.inverse( *worldMatr );
		orig.mul( invW );
		dir.mul( invW );
	}

	dir -= orig;
	dir.normalize();
	ray.setOrigDir( orig, dir );
} // Camera::getPickRay

Vector3D BaseCamera::GetLookAt() const
{
    //  HACK-HACK
    Vector3D dir = GetDir();
    dir *= 2500.0f;
    dir += GetPos();
    return dir;
} // BaseCamera::GetLookAt

Frustum	BaseCamera::GetFrustum() const
{
	Frustum fr;
	GetFrustum( fr );
	return fr;
} // BaseCamera::GetFrustum

void BaseCamera::GetFrustum( Frustum& fr ) const
{
	Matrix4D matr = GetViewProjM();
	fr.FromCameraMatrix( matr );
} // BaseCamera::GetFrustum

void BaseCamera::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );

	DWORD reserved = 0;
	
	//  for projection matrix we need different serialization scheme, as soon as 
	//  projection transformation is not affine
	
	os << m_Proj.e00 << m_Proj.e01 << m_Proj.e02 << m_Proj.e03;
	os << m_Proj.e10 << m_Proj.e11 << m_Proj.e12 << m_Proj.e13;
	os << m_Proj.e20 << m_Proj.e21 << m_Proj.e22 << m_Proj.e23;
	os << m_Proj.e30 << m_Proj.e31 << m_Proj.e32 << m_Proj.e33;

	os << reserved;
} // BaseCamera::Serialize

void BaseCamera::Unserialize( InStream& is ) 
{
	Parent::Unserialize( is );
	DWORD reserved;

	is >> m_Proj.e00 >> m_Proj.e01 >> m_Proj.e02 >> m_Proj.e03;
	is >> m_Proj.e10 >> m_Proj.e11 >> m_Proj.e12 >> m_Proj.e13;
	is >> m_Proj.e20 >> m_Proj.e21 >> m_Proj.e22 >> m_Proj.e23;
	is >> m_Proj.e30 >> m_Proj.e31 >> m_Proj.e32 >> m_Proj.e33;

	is >> reserved;
} // BaseCamera::Serialize

void BaseCamera::Render()
{
	if (s_bFrozen) return;

	s_TMStack.Push( tm );
	m_WorldTM = s_TMStack.Top();

	if (m_bTweakAspect) TweakProjM( IRS->GetViewPort() );
	Set();
	s_ActiveCamera = GetID();

	Node::Render();

	s_TMStack.Pop();
} // BaseCamera::Render

void BaseCamera::Set()
{
	IRS->SetProjectionMatrix( GetProjM() );
	Matrix4D vM;
	vM.inverse( GetWorldTM() );
	IRS->SetViewMatrix( vM );
} // BaseCamera::Set

void BaseCamera::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "BaseCamera", this );
	pm.p( "ZNear",			GetZn,		SetZn	);
	pm.p( "ZFar",			GetZf,		SetZf	);
	pm.p( "AspectWtoH",	GetWToH,	SetWToH );
	pm.f( "TweakAspect", m_bTweakAspect );
} // BaseCamera::Expose

void BaseCamera::AdjustScreenSpaceSize( Matrix4D& m, float size )
{
	Vector4D pos( m.e30, m.e31, m.e32, 1.0f );
	Vector4D vs( pos );
	WorldToProjectionSpace( vs );
	vs.x += size;
	ProjectionToWorldSpace( vs );
	vs -= pos;
	float sc = vs.norm();

	Vector3D vx( m.e00, m.e10, m.e20 );
	Vector3D vy( m.e01, m.e11, m.e21 );
	Vector3D vz( m.e02, m.e12, m.e22 );

	Vector3D::orthonormalize( vx, vy, vz );

	m.e00 = vx.x; m.e01 = vy.x; m.e02 = vz.x; 
	m.e10 = vx.y; m.e11 = vy.y; m.e12 = vz.y; 
	m.e20 = vx.z; m.e21 = vy.z; m.e22 = vz.z; 

	Matrix4D scTM;
	scTM.scaling( sc );
	m.mulLeft( scTM );
} // BaseCamera::AdjustScreenSpaceSize

void BaseCamera::HideCameras( Node* pNode, bool hide )
{
	Iterator it( pNode, BaseCamera::FnFilter );
	while (it)
	{
		((Node*)it)->SetInvisible( hide );
		++it;
	}
} // BaseCamera::HideCameras

//  pans camera to make view direction go through point lookAt
void BaseCamera::Pan( const Vector3D& lookAt )
{
	Vector4D trans( lookAt );
	WorldToProjectionSpace( trans );
	trans.x = trans.x;
	trans.y = trans.y;
	trans.z = 0.0f;
	trans.w = 0.0f;
	ProjectionToWorldSpace( trans );
	trans += GetPos();
	SetPos( trans );
} // BaseCamera::Pan

/*****************************************************************************/
/*	PerspCamera implementation
/*****************************************************************************/
PerspCamera::PerspCamera()
{
	SetDirUp( Vector3D::oX, Vector3D::oZ );
	SetPerspFOVx( c_HalfPI, IRS ? IRS->GetViewPort().GetAspect() : 4.0f/3.0f, 10.0f, 10000.0f );
}

void PerspCamera::SetPerspFOVx( float fovx, float wToH, float zn, float zf )
{
	float w = 2.0f * zn * tan( fovx * 0.5f );
	float h = w / wToH;

	SetPerspWH( w, h, zn, zf );
} // PerspCamera::SetPerspFOV

void PerspCamera::SetPerspFOVy( float fovy, float wToH, float zn, float zf )
{
	float h = 2.0f * zn * tan( fovy * 0.5f );
	float w = h * wToH;

	SetPerspWH( w, h, zn, zf );
} // PerspCamera::SetPerspFOV

void PerspCamera::SetPerspWH( float w, float h, float zn, float zf )
{
	assert( w > 0.0f && h >= 0.0f && zn > 0.0f && zf > zn );

	m_Proj.e00 = 2*zn/w; m_Proj.e01 = 0.0f;		m_Proj.e02 = 0.0f;			m_Proj.e03 = 0.0f;
	m_Proj.e10 = 0.0f;	 m_Proj.e11 = 2*zn/h;	m_Proj.e12 = 0.0f;			m_Proj.e13 = 0.0f;
	m_Proj.e20 = 0.0f;	 m_Proj.e21 = 0.0f;		m_Proj.e22 = zf/(zf-zn);	m_Proj.e23 = 1.0f;
	m_Proj.e30 = 0.0f;	 m_Proj.e31 = 0.0f;		m_Proj.e32 = zn*zf/(zn-zf);	m_Proj.e33 = 0.0f;

} // PerspCamera::SetPerspWH

void PerspCamera::SetPerspW( float w, float wtoh, float zn, float zf )
{
	assert( w > 0.0f && wtoh >= 0.0f && zn > 0.0f && zf > zn );

	float h = w / wtoh;
	m_Proj.e00 = 2*zn/w; m_Proj.e01 = 0.0f;		m_Proj.e02 = 0.0f;			m_Proj.e03 = 0.0f;
	m_Proj.e10 = 0.0f;	 m_Proj.e11 = 2*zn/h;	m_Proj.e12 = 0.0f;			m_Proj.e13 = 0.0f;
	m_Proj.e20 = 0.0f;	 m_Proj.e21 = 0.0f;		m_Proj.e22 = zf/(zf-zn);	m_Proj.e23 = 1.0f;
	m_Proj.e30 = 0.0f;	 m_Proj.e31 = 0.0f;		m_Proj.e32 = zn*zf/(zn-zf);	m_Proj.e33 = 0.0f;
} // PerspCamera::SetPerspW

void PerspCamera::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PerspCamera", this );
	pm.p( "FovX", GetFOVxDeg, SetFOVxDeg );

} // PerspCamera::Expose

void PerspCamera::SetFOV( float fov )
{
    SetFOVx( fov );
} // PerspCamera::SetFOV

void PerspCamera::TweakProjM( const Rct& rct )
{
	SetPerspWH( GetNearW(), GetNearW() * rct.h / rct.w, GetZn(), GetZf() );
}

void PerspCamera::SetProjection( float volW, float aspect, float zn, float zf )
{
	SetPerspFOVx( GetFOVx(), aspect, zn, zf );
	//SetPerspW( volW, aspect, zn, zf );
}

void PerspCamera::ShiftZ( float amount )
{
	SetPerspFOVx( GetFOVx(), GetWToH(), GetZn() + amount, GetZf() + amount );
	Render();
}

/*****************************************************************************/
/*	OrthoCamera implementation
/*****************************************************************************/
OrthoCamera::OrthoCamera()
{
	SetDirUp( Vector3D::oX, Vector3D::oZ );
	SetOrthoW( 1000.0f, IRS ? IRS->GetViewPort().GetAspect() : 4.0f/3.0f, 10.0f, 10000.0f );
}

void OrthoCamera::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "OrthoCamera", this );
	pm.p( "ViewVolume Width", GetViewVolW, SetViewVolW );
} // OrthoCamera::Expose

void OrthoCamera::TweakProjM( const Rct& rct )
{
	SetOrthoW( GetViewVolW(), rct.w / rct.h, GetZn(), GetZf() );
}

void OrthoCamera::SetProjection( float volW, float aspect, float zn, float zf )
{
	SetOrthoW( volW, aspect, zn, zf );
}

void OrthoCamera::ShiftZ( float amount )
{
	SetOrthoW( GetViewVolW(), GetWToH(), GetZn() + amount, GetZf() + amount );
	Render();
}

void OrthoCamera::GetFrustum( Frustum& fr ) const
{
	BaseCamera::GetFrustum( fr );
}




END_NAMESPACE( sg )