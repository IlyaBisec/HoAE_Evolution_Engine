/*****************************************************************
/*  File:   Camera.inl                                           *
/*  Desc:                                                        *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Mar 2002                                             *
/*****************************************************************/
_inl const Vector3D& Camera::getPos() const
{
	return pos; 
}

_inl const Vector3D& Camera::getAt() const
{
	return at;
}

_inl const Vector3D& Camera::getDir() const
{
	return dir;
}

_inl const Vector3D& Camera::getUp() const
{
	return up;
}

_inl float Camera::getScrW() const
{
	return scrW;
}

_inl float Camera::getScrH() const
{
	return scrH;
}

_inl float Camera::getScrArea()	const
{
	return scrH * scrW;
}

_inl void Camera::setLookAt( float x, float y, float z )
{
	at.set( x, y, z );
    calcViewM();
}

_inl void Camera::setLookAt( const Vector3D& pt )
{
	at.copy( pt );
    calcViewM();
}

_inl void Camera::setViewM( const Matrix4D& matr )
{
     viewM = matr;
	 Vector3D npos	( matr.e30, matr.e31, matr.e32 );
	 Vector3D nup	( matr.e01, matr.e11, matr.e21 );
	 Vector3D ndir	( matr.e02, matr.e12, matr.e22 );
	 Vector3D nright( matr.e00, matr.e10, matr.e20 );
	 setPos( npos );
	 setDir( ndir );
	 setUp ( nup );
}

_inl void Camera::transform( const Matrix4D& matr )
{
	viewM *= matr;
}

_inl void Camera::setDir( float x, float y, float z )
{
	dir.set( x, y, z );
	dir.normalize();
	calcViewM();
	at.add( pos, dir );
}

_inl void Camera::setUp( float x, float y, float z )
{
	up.set( x, y, z );
    calcViewM();
}

_inl void Camera::setOrtho( bool isOrtho )
{
     bOrtho = isOrtho;
     calcProjM();
}

_inl void Camera::setViewVolumeWidth( float vvWidth )
{
	viewVolWidth = vvWidth;

	//  adjust field of view for perspective camera
	if (!bOrtho)
	{
		if (Zn > c_SmallEpsilon)
		{
			float ratio = vvWidth * 0.5f / Zn;
			if (ratio > c_SmallEpsilon) 
			{
				hFov = atan( ratio ) * 2.0f;
			}
		}
	}
	calcProjM();
}

_inl float Camera::getViewVolumeWidth() const
{
	if (bOrtho) 
	{
		return viewVolWidth;
	}
	else 
	{
		return Zn * tan( getHFOV() * 0.5f );
	}
}

_inl float Camera::getViewVolumeHeight() const
{
	return getViewVolumeWidth() * scrH / scrW;
}   

_inl bool Camera::isOrtho() const
{
     return bOrtho;
}

_inl void Camera::setDir( const Vector3D& _dir )
{
	dir.copy( _dir );
    calcViewM();
	at.add( pos, dir );
}

_inl void Camera::setDir( const Vector4D& _dir )
{
	dir.copy( _dir );
    calcViewM();
	at.add( pos, dir );
}

_inl void Camera::setUp( const Vector3D& _up )
{
	up.copy( _up );
    calcViewM();
}

_inl float Camera::getVFOV()	const
{
	return hFov * (scrH / scrW);
}

_inl float Camera::getHFOV()	const
{
	return hFov;
}

_inl void Camera::setHFOV( float _hfov )
{
	assert( _hfov != 0.0f );
	hFov = _hfov;
    calcProjM();
}

_inl float Camera::getZn() const
{
	return Zn; 
}

_inl float Camera::getZf() const
{
	return Zf;
}

_inl void Camera::setZn( float _Zn )
{
     Zn = _Zn;
     calcProjM();
}

_inl void Camera::setZf( float _Zf )
{
     Zf = _Zf;
     calcProjM();
}

_inl void Camera::setViewPort( int left, int top, int width, int height )
{
	scrX	= left;
	scrY	= top;
	scrW	= width;
	scrH	= height;
    calcProjM();
}

_inl const Matrix4D&	Camera::getViewM() const
{
	return viewM;
}

_inl const Matrix4D&	Camera::getProjM() const
{
	return projM;
}

_inl const Matrix4D&	Camera::getFullM() const
{
	return fullM;
}

_inl void Camera::ProjectionToScreenSpace( float& x, float& y ) const
{
	x = (x + 1.0f) * scrW * 0.5f;
	y = (1.0f - y) * scrH * 0.5f;

	x += scrX;
	y += scrY;
} // Camera::ProjectionToScreenSpace

_inl void Camera::ScreenToProjectionSpace( float& x, float& y ) const
{
	x -= scrX;			
	x /= scrW * 0.5f;	
	x -= 1.0f;			

	y -= scrY;
	y /= scrH * 0.5f;
	y = 1.0f - y;
} // Camera::ScreenToProjectionSpace	

/*---------------------------------------------------------------*
/*  Func:	Camera::WorldToScreenSpace
/*	Desc:	Transforms pos: world space=>screen space
/*	Parm:	pos - 3D world position
/*---------------------------------------------------------------*/
_inl void Camera::WorldToScreenSpace( Vector4D& pos ) const
{
	pos.mul( viewM );
    pos.mul( projM );
	pos.normW();
	ProjectionToScreenSpace( pos.x, pos.y );
}//  Camera::toScreen

_inl void Camera::ScreenToWorldSpace( Vector4D& pos ) const
{
	Vector4D res( pos );
	ScreenToProjectionSpace( pos.x, pos.y );
	Matrix4D pI;
	pI.inverse( getProjM() );
	pos.mul( pI );
	
	Matrix4D vI;
	vI.inverse( getViewM() );
	pos.mul( vI );
} // Camera::ScreenToWorldSpace

_inl void Camera::ZBiasIncrease( float ratio )
{
	float rate = ratio * (Zf - Zn);
	Zf += rate;
	Zn += rate;
	calcProjM();
} // Camera::ZBiasIncrease

_inl void Camera::ZBiasDecrease( float ratio )
{
	float rate = ratio * (Zf - Zn);
	Zf -= rate;
	Zn -= rate;
	calcProjM();
} // Camera::ZBiasDecrease

_inl const Frustum&	Camera::getFrustum() const
{
	return frustum;
}


