/*****************************************************************************/
/*	File:	sgCamera.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-21-2003
/*****************************************************************************/

BEGIN_NAMESPACE( sg )

/*****************************************************************************/
/*	Canvas implementation
/*****************************************************************************/
_inl float Canvas::GetX() const
{
	return m_Extents.x;
}

_inl float Canvas::GetY() const
{
	return m_Extents.y;
}

_inl float Canvas::GetW() const
{
	return m_Extents.w;
}

_inl float Canvas::GetH() const
{
	return m_Extents.h;
}

_inl void Canvas::SetX( float _x ) 
{
	m_Extents.x = _x;
}

_inl void Canvas::SetY( float _y )
{
	m_Extents.y = _y;
}

_inl void Canvas::SetW( float _w )
{
	m_Extents.w = _w;
}

_inl void Canvas::SetH( float _h )
{
	m_Extents.h = _h;
}

/*****************************************************************************/
/*	BaseCamera implementation
/*****************************************************************************/
_inl void BaseCamera::GetPos( Vector3D& pos ) const
{
	pos.x = tm.e30;
	pos.y = tm.e31;
	pos.z = tm.e32;
}

_inl void BaseCamera::GetDir( Vector3D& dir ) const
{
	dir.x = tm.e20;
	dir.y = tm.e21;
	dir.z = tm.e22;
}

_inl void BaseCamera::GetUp( Vector3D& up ) const
{
	up.x = tm.e10;
	up.y = tm.e11;
	up.z = tm.e12;
}

_inl void BaseCamera::GetRight( Vector3D& right ) const
{
	right.x = tm.e00;
	right.y = tm.e01;
	right.z = tm.e02;
}

_inl Vector3D BaseCamera::GetCenter( float ratio ) const
{
	float cZ = (GetZn() + GetZf()) * ratio;
	Vector3D pos = GetPos();
	Vector3D dir = GetDir();
	dir *= cZ;
	pos += dir;
	return pos;
} // BaseCamera::GetCenter

_inl Vector3D BaseCamera::GetPos() const
{
	return Vector3D( tm.e30, tm.e31, tm.e32 );
}

_inl Vector3D BaseCamera::GetDir() const
{
	return Vector3D( tm.e20, tm.e21, tm.e22 );
}

_inl Vector3D BaseCamera::GetUp() const
{
	return Vector3D( tm.e10, tm.e11, tm.e12 );
}

_inl Vector3D BaseCamera::GetRight() const
{
	return Vector3D( tm.e00, tm.e01, tm.e02 );
}

_inl bool BaseCamera::SetRightUpDir(	const Vector3D& right, 
										const Vector3D& up, 
										const Vector3D& dir ) 
{
	Vector3D nRight	( right );
	Vector3D nUp	( up	);
	Vector3D nDir	( dir	);

	if (!Vector3D::orthonormalize( nDir, nRight, nUp ))
	{
		return false;
	}

	tm.e00 = nRight.x;
	tm.e01 = nRight.y;
	tm.e02 = nRight.z;

	tm.e10 = nUp.x;
	tm.e11 = nUp.y;
	tm.e12 = nUp.z;

	tm.e20 = nDir.x;
	tm.e21 = nDir.y;
	tm.e22 = nDir.z;

	return true;
} // BaseCamera::SetRightUpDir

_inl void BaseCamera::SetPos( const Vector3D& pos )
{
	tm.e30 = pos.x;
	tm.e31 = pos.y;
	tm.e32 = pos.z;
} // BaseCamera::SetPos

_inl void BaseCamera::SetPos( float x, float y, float z )
{
	tm.e30 = x;
	tm.e31 = y;
	tm.e32 = z;
} // BaseCamera::SetPos

_inl void BaseCamera::SetViewM( const Matrix4D& viewM )
{
	tm = viewM;
} // BaseCamera::SetViewM

_inl void BaseCamera::SetProjM( const Matrix4D& projM )
{
	m_Proj = projM;
} // BaseCamera::SetProjM

_inl void BaseCamera::ProjectionToScreenSpace( float& x, float& y, const Rct* pViewport ) 
{
	Rct vp = pViewport ? *pViewport : IRS->GetViewPort();

	x = (x + 1.0f) * vp.w * 0.5f;
	y = (1.0f - y) * vp.h * 0.5f;

	x += vp.x;
	y += vp.y;
} // BaseCamera::ProjectionToScreenSpace

_inl void BaseCamera::ScreenToProjectionSpace( float& x, float& y, const Rct* pViewport ) 
{
	const Rct& vp = pViewport ? *pViewport : IRS->GetViewPort();

	x -= vp.x;			
	x /= vp.w * 0.5f;	
	x -= 1.0f;			

	y -= vp.y;
	y /= vp.h * 0.5f;
	y = 1.0f - y;
} // BaseCamera::ScreenToProjectionSpace	

_inl void BaseCamera::WorldToScreenSpace( Vector4D& pos, const Rct* pViewport ) const
{
	WorldToProjectionSpace( pos );
	ProjectionToScreenSpace( pos.x, pos.y, pViewport );
}//  BaseCamera::WorldToScreenSpace

_inl void BaseCamera::WorldToScreenSpace( Vector3D& pos ) const
{
    Vector4D p( pos );
    p.mul( GetViewProjM() );
    p.normW();
    ProjectionToScreenSpace( p.x, p.y );
    pos = p;
}

_inl void BaseCamera::WorldToProjectionSpace( Vector4D& pos ) const
{
	pos.mul( GetViewProjM() );
	pos.normW();
} // BaseCamera::WorldToProjectionSpace

_inl Matrix4D BaseCamera::WorldToProjectionSpace() const
{
	return GetViewProjM();
} // BaseCamera::WorldToProjectionSpace

_inl void BaseCamera::ProjectionToWorldSpace( Vector4D& pos ) const
{
	//  TODO - make it more efficient
	Matrix4D im;
	im.inverse( GetViewProjM() );
	pos.mul( im );
	pos.normW();
} // BaseCamera::ProjectionToWorldSpace

_inl void BaseCamera::ScreenToWorldSpace( Vector4D& pos, const Rct* pViewport ) const
{
	ScreenToProjectionSpace( pos.x, pos.y, pViewport );
	ProjectionToWorldSpace( pos );
} // BaseCamera::ScreenToWorldSpace

_inl void BaseCamera::ProjectionToCameraSpace( Vector4D& pos ) const
{
	assert( false );
	pos *= tm;
} // BaseCamera::ProjectionToCameraSpace

_inl void BaseCamera::CameraToProjectionSpace( Vector4D& pos ) const
{
	pos *= m_Proj;
	pos.normW();
} // BaseCamera::ProjectionToCameraSpace

_inl void BaseCamera::ScreenToCameraSpace( Vector4D& pos, const Rct* pViewport ) const
{
	assert( false );
	ScreenToProjectionSpace( pos.x, pos.y, pViewport );
	ProjectionToCameraSpace( pos );
} // BaseCamera::ScreenToCameraSpace

_inl void BaseCamera::WorldToCameraSpace( Vector4D& pos ) const
{
	Matrix4D m;
	GetViewM( m );
	pos *= m;
} // BaseCamera::WorldToCameraSpace

_inl void BaseCamera::CameraToWorldSpace( Vector4D& pos ) const
{
	pos *= tm;
} // BaseCamera::WorldToCameraSpace

_inl Matrix4D BaseCamera::CameraToProjectionSpace() const
{
	return m_Proj;
} // BaseCamera::ProjectionToCameraSpace

_inl void BaseCamera::GetViewM( Matrix4D& m ) const
{
	m.inverse( tm );
} // BaseCamera::GetViewM

_inl Matrix4D BaseCamera::GetViewProjM() const
{
	Matrix4D m;
	m.inverse( tm );
	m *= m_Proj;
	return m;
} // BaseCamera::GetViewProjM

_inl Matrix4D BaseCamera::GetViewM() const
{
	Matrix4D m;
	m.inverse( tm );
	return m;
}

_inl Matrix4D BaseCamera::GetProjM() const
{
	return m_Proj;
}

_inl void BaseCamera::GetProjM( Matrix4D& m ) const
{
	m = m_Proj;
}

_inl void BaseCamera::GetInvViewM( Matrix4D& m ) const
{
	m = tm;
}

_inl void BaseCamera::GetInvProjM( Matrix4D& m ) const
{
	m.inverse( m_Proj );
}

_inl void BaseCamera::ZBiasIncrease( float ratio )
{	
	float Zn = GetZn();
	float Zf = GetZf();
	float rate = ratio * (Zf - Zn);
	Zf += rate;
	if (Zn > rate) Zn += rate;

	SetZnf( Zn, Zf );
} // BaseCamera::ZBiasIncrease

_inl void BaseCamera::ZBiasDecrease( float ratio )
{	
	float Zn = GetZn();
	float Zf = GetZf();
	float rate = ratio * (Zf - Zn);
	Zf -= rate;
	if (Zn > rate) Zn -= rate;

	SetZnf( Zn, Zf );
} // BaseCamera::ZBiasDecrease

/*****************************************************************************/
/*	OrthoCamera implementation
/*****************************************************************************/
_inl void OrthoCamera::SetOrthoW( float viewVolW, float wToH, float zn, float zf )
{
	OrthoProjectionTM( m_Proj, viewVolW, wToH, zn, zf );
} // OrthoCamera::SetOrthoW

_inl float OrthoCamera::GetZn() const
{
	return -m_Proj.e32/m_Proj.e22;
}

_inl float OrthoCamera::GetZf() const
{
	return (1.0f - m_Proj.e32)/m_Proj.e22;
}

_inl float OrthoCamera::GetWToH() const
{
	return m_Proj.e11/m_Proj.e00;
}

_inl float OrthoCamera::GetViewVolW() const
{
	return 2.0f / m_Proj.e00;
}

_inl void OrthoCamera::SetViewVolW( float _vVol )
{
	SetOrthoW( _vVol, GetWToH(), GetZn(), GetZf() );
}

_inl void OrthoCamera::SetZn( float _Zn )
{
	SetOrthoW( GetViewVolW(), GetWToH(), _Zn, GetZf() );
}

_inl void OrthoCamera::SetZf( float _Zf )
{
	SetOrthoW( GetViewVolW(), GetWToH(), GetZn(), _Zf );
}

_inl void OrthoCamera::SetZnf( float _Zn, float _Zf )
{
	SetOrthoW( GetViewVolW(), GetWToH(), _Zn, _Zf );
}

_inl void OrthoCamera::SetWToH( float _aspect )
{
	SetOrthoW( GetViewVolW(), _aspect, GetZn(), GetZf() );
}

/*****************************************************************************/
/*	PerspCamera implementation
/*****************************************************************************/
_inl float PerspCamera::GetZn() const
{
	assert( m_Proj.e22 > 0.0f );
	return -m_Proj.e32/m_Proj.e22;
}

_inl float PerspCamera::GetZf() const
{
	return m_Proj.e32/(1.0f - m_Proj.e22);
}

_inl float PerspCamera::GetFOVy() const
{
	return 2.0f * atan( GetNearH() / (2.0f * GetZn()) );
}

_inl float PerspCamera::GetFOVyDeg() const
{
	return GetFOVy() * 180.0f / c_PI;
}

_inl float PerspCamera::GetFOVxDeg() const
{
	return GetFOVx() * 180.0f / c_PI;
}

_inl void PerspCamera::SetFOVyDeg( float _fovYdeg )
{
	SetFOVy( c_PI * _fovYdeg / 180.0f );
}

_inl void PerspCamera::SetFOVxDeg( float _fovXdeg )
{
	SetFOVx( c_PI * _fovXdeg / 180.0f );
}

_inl float PerspCamera::GetFOVx() const
{
	return 2.0f * atan( GetNearW() / (2.0f * GetZn()) );	
}

_inl float PerspCamera::GetWToH() const
{
	return GetNearW() / GetNearH();
}

_inl void PerspCamera::SetZn( float _Zn )
{
	SetPerspFOVx( GetFOVx(), GetWToH(), _Zn, GetZf() );
}

_inl void PerspCamera::SetZf( float _Zf )
{
	SetPerspFOVx( GetFOVx(), GetWToH(), GetZn(), _Zf );
}

_inl void PerspCamera::SetZnf( float _Zn, float _Zf )
{
	SetPerspFOVx( GetFOVx(), GetWToH(), _Zn, _Zf );
}

_inl void PerspCamera::SetFOVy( float _fovY )
{
	SetPerspFOVy( _fovY, GetWToH(), GetZn(), GetZf() );
}

_inl void PerspCamera::SetFOVx( float _fovX )
{
	SetPerspFOVx( _fovX, GetWToH(), GetZn(), GetZf() );
}

_inl void PerspCamera::SetWToH( float _aspect )
{
	SetPerspWH( GetNearW(), _aspect, GetZn(), GetZf() );
}

_inl float PerspCamera::GetNearW() const
{
	assert( m_Proj.e00 > 0.0f );
	return 2.0f * GetZn() / m_Proj.e00;
}

_inl float PerspCamera::GetNearH() const
{
	assert( m_Proj.e11 > 0.0f );
	return 2.0f * GetZn() / m_Proj.e11;
}

_inl float PerspCamera::GetFarW() const
{
	return m_Proj.e22 * GetNearW() / (m_Proj.e22 - 1.0f);
}

_inl float PerspCamera::GetFarH() const
{
	return m_Proj.e22 * GetNearH() / (m_Proj.e22 - 1.0f);
}



END_NAMESPACE( sg )


