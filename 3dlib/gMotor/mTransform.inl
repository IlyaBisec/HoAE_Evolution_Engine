/*****************************************************************
/*  File:   mTransform.inl                                      
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "mQuaternion.h"

_inl bool OrthoProjectionTM( Matrix4D& m, float viewVolW, float wToH, float zn, float zf )
{
    float	  w = viewVolW;
    float	  h = viewVolW / wToH;
    float	  dz = zf - zn;
    if (fabs( dz ) < c_SmallEpsilon ||
        fabs( w  ) < c_SmallEpsilon || 
        fabs( h  ) < c_SmallEpsilon) return false;

    m.e00 = 2.0f/w; m.e01 = 0.0f;	m.e02 = 0.0f;			m.e03 = 0.0f;
    m.e10 = 0.0f;	m.e11 = 2.0f/h; m.e12 = 0.0f;			m.e13 = 0.0f;
    m.e20 = 0.0f;	m.e21 = 0.0f;	m.e22 = 1.0f/(zf - zn); m.e23 = 0.0f;
    m.e30 = 0.0f;	m.e31 = 0.0f;	m.e32 = -zn/(zf - zn);	m.e33 = 1.0f;
    return true;
} // OrthoProjectionTM

/*****************************************************************
/*	Matrix3D implementation
/*****************************************************************/
_inl Matrix3D::Matrix3D( const Matrix4D& m )
{
	e00 = m.e00; e01 = m.e01; e02 = m.e02;
	e10 = m.e10; e11 = m.e11; e12 = m.e12;
	e20 = m.e20; e21 = m.e21; e22 = m.e22;
}

_inl Matrix3D::Matrix3D( float _e00, float _e01, float _e02,
						 float _e10, float _e11, float _e12,
						 float _e20, float _e21, float _e22 ) 
						 : 
						 e00(_e00), e01(_e01), e02(_e02),
						 e10(_e10), e11(_e11), e12(_e12),
						 e20(_e20), e21(_e21), e22(_e22) {}

_inl Matrix3D::Matrix3D( const Vector3D& v0,  
						 const Vector3D& v1,
						 const Vector3D& v2 )
{
	e00 = v0.x; e01 = v0.y; e02 = v0.z;
	e10 = v1.x; e11 = v1.y; e12 = v1.z;
	e20 = v2.x; e21 = v2.y; e22 = v2.z;
} // Matrix3D::Matrix3D

/*---------------------------------------------------------------*
/*  Func:	Matrix3D::scaling
/*	Desc:	Builds scaling matrix 
/*	Parm:	sx, sy, sz - x, y, z, axis scales
/*---------------------------------------------------------------*/
_inl void Matrix3D::scaling( float sx, float sy, float sz )
{
	setIdentity();
	e00 = sx;
	e11 = sy;
	e22 = sz;
}

_inl float	Matrix3D::trace() const
{
	return e00 + e11 + e22;
}

/*---------------------------------------------------------------*
/*  Func:	Matrix3D::rotation
/*	Desc:	Builds rotation matrix from the quaternion
/*	Parm:	q - rotation quaternion
/*	Rmrk:	Rotation matrix is pre-multiply transform matrix
/*---------------------------------------------------------------*/
_inl void Matrix3D::rotation( const Quaternion& q )
{
	float xx, yy, zz, xy, yz, xz, sx, sy, sz;
	xx = 2.0f * q.v.x * q.v.x;
	yy = 2.0f * q.v.y * q.v.y;
	zz = 2.0f * q.v.z * q.v.z;
	xy = 2.0f * q.v.x * q.v.y;
	yz = 2.0f * q.v.y * q.v.z;
	xz = 2.0f * q.v.x * q.v.z;
	sx = 2.0f * q.s   * q.v.x;
	sy = 2.0f * q.s	  * q.v.y;
	sz = 2.0f * q.s   * q.v.z;

	e00 = 1.0f - yy - zz;
	e01 = xy + sz;
	e02 = xz - sy;

	e10 = xy - sz;
	e11 = 1.0f - xx - zz;
	e12 = yz + sx;

	e20 = xz + sy;
	e21 = yz - sx;
	e22 = 1.0f - xx - yy;
}//  Matrix3D::rotation

_inl void Matrix3D::rotationXY( float cosPhi, float sinPhi )
{
	e00 = cosPhi;	e01 = sinPhi;	e02 = 0.0f;
	e10 = -sinPhi;	e11 = cosPhi;	e12 = 0.0f;
	e20 = 0.0f;		e21 = 0.0f;		e22 = 1.0f;
}

_inl void Matrix3D::rotationYZ( float cosPhi, float sinPhi )
{
	e00 = 1.0f;		e01 = 0.0f;		e02 = 0.0f;
	e10 = 0.0f;		e11 = cosPhi;	e12 = sinPhi;	
	e20 = 0.0f;		e21 = -sinPhi;	e22 = cosPhi;	
}

_inl void Matrix3D::rotationXZ( float cosPhi, float sinPhi )
{
	e00 = cosPhi;	e01 = 0.0f;		e02 = -sinPhi;
	e10 = 0.0f;		e11 = 1.0f;		e12 = 0.0f;
	e20 = sinPhi;	e21 = 0.0f;		e22 = cosPhi;
}

_inl void Matrix3D::shearXY( float cosPhi, float sinPhi )
{
	setIdentity();
	e01 = sinPhi / cosPhi;
} // Matrix3D::shearXY

_inl void Matrix3D::shearYZ( float cosPhi, float sinPhi )
{
	setIdentity();
	e12 = sinPhi / cosPhi;
} // Matrix3D::shearYZ

_inl void Matrix3D::shearXZ( float cosPhi, float sinPhi )
{
	setIdentity();
	e02 = sinPhi / cosPhi;
} // Matrix3D::shearXZ

/*---------------------------------------------------------------*
/*  Func:	Matrix3D::rotation
/*	Desc:	Builds rotation matrix from axis of rotation and angle
/*	Parm:	axis  - axis of rotation
/*			angle - rotation angle 
/*---------------------------------------------------------------*/
_inl void Matrix3D::rotation( const Vector3D& axis, float angle )
{
	Quaternion q;
	q.FromAxisAngle( axis, angle );
	rotation( q );
} // Matrix3D::rotation

_inl void Matrix3D::rotation( float rotX, float rotY, float rotZ )
{
	rotationYZ( cosf( rotX ), sinf( rotX ) );
	Matrix3D rot;
	rot.rotationXZ( cosf( rotY ), sinf( rotY ) );
	*this *= rot;
	rot.rotationXY( cosf( rotZ ), sinf( rotZ ) );
	*this *= rot;
} // Matrix3D::rotation

/*---------------------------------------------------------------------------*/
/*	Func:	Matrix3D::det	
/*	Desc:	finds determinant of matrix
/*---------------------------------------------------------------------------*/
_inl float Matrix3D::det() const
{

	return	e00*(e11*e22 - e21*e12) + 
		e01*(e20*e12 - e10*e22) + 
		e02*(e10*e21 - e20*e11);
} // Matrix3D::det

_inl void Matrix3D::copy( const Matrix3D& orig )
{
	memcpy( (void*)this, (void*)&orig, sizeof( Matrix3D ) );
}

_inl void Matrix3D::copy( const Matrix4D& orig )
{
	e00 = orig.e00; e01 = orig.e01; e02 = orig.e02;
	e10 = orig.e10; e11 = orig.e11; e12 = orig.e12;
	e20 = orig.e20; e21 = orig.e21; e22 = orig.e22;
}

_inl void Matrix3D::txtSave( FILE* fp, const char* name ) const
{
	if (name)
	{
		fprintf( fp,	"%s={\n{%-8.8f, %-8.8f, %-8.8f}, \n"
			"{%-8.8f, %-8.8f, %-8.8f}, \n"
			"{%-8.8f, %-8.8f, %-8.8f} \n}\n",
			name,
			e00, e01, e02, 
			e10, e11, e12, 
			e20, e21, e22 );
	}
	else
	{
		fprintf( fp,	"={\n{%-8.8f, %-8.8f, %-8.8f}, \n"
			"{%-8.8f, %-8.8f, %-8.8f}, \n"
			"{%-8.8f, %-8.8f, %-8.8f} \n}\n",
			e00, e01, e02, 
			e10, e11, e12, 
			e20, e21, e22 );
	}
} // Matrix3D::txtSave

/*---------------------------------------------------------------------------*/
/*	Func:	Matrix3D::inverse
/*	Desc:	Calculates inverse matrix with Kramer's rule 
/*	Ret:	matrix determinant
/*---------------------------------------------------------------------------*/
_inl float Matrix3D::inverse( const Matrix3D& m )
{
	float det;
	copy( m );
	Inverse3x3<float>( &e00, det );
	return det;
} // Matrix3D::inverse

_inl float Matrix3D::inverse()
{
	Matrix3D tmp( *this );
	return inverse( tmp );
} // Matrix3D::inverse

_inl void Matrix3D::setNull()
{
	memset( getBuf(), 0, 9 * sizeof( e00 ) );
}

_inl void Matrix3D::setIdentity()
{
	setNull();
	e00 = 1.0f;
	e11 = 1.0f;
	e22 = 1.0f;
}

_inl Matrix3D& Matrix3D::operator *=( const Matrix3D& r )
{
	Matrix3D tmp;
	tmp.mul( *this, r );
	(*this) = tmp;
	return (*this);
}

_inl void Matrix3D::transpose()
{
	float tmp = e01; e01 = e10; e10 = tmp;
    tmp = e02; e02 = e20; e20 = tmp;
    tmp = e12; e12 = e21; e21 = tmp;
}

_inl void Matrix3D::mul( const Matrix3D& l, const Matrix3D& r )
{
	e00 = l.e00 * r.e00 + l.e01 * r.e10 + l.e02 * r.e20;
	e01 = l.e00 * r.e01 + l.e01 * r.e11 + l.e02 * r.e21;
	e02 = l.e00 * r.e02 + l.e01 * r.e12 + l.e02 * r.e22;

	e10 = l.e10 * r.e00 + l.e11 * r.e10 + l.e12 * r.e20;
	e11 = l.e10 * r.e01 + l.e11 * r.e11 + l.e12 * r.e21;
	e12 = l.e10 * r.e02 + l.e11 * r.e12 + l.e12 * r.e22;

	e20 = l.e20 * r.e00 + l.e21 * r.e10 + l.e22 * r.e20;
	e21 = l.e20 * r.e01 + l.e21 * r.e11 + l.e22 * r.e21;
	e22 = l.e20 * r.e02 + l.e21 * r.e12 + l.e22 * r.e22;
}

_inl Matrix3D& Matrix3D::mulLeft( const Matrix3D& l )
{
	Matrix3D r( *this );
	mul( l, r );
	return *this;
} // Matrix3D::mulLeft

_inl Matrix3D& Matrix3D::operator *=( const float f )
{
	e00 *= f;
	e01 *= f;
	e02 *= f;

	e10 *= f;
	e11 *= f;
	e12 *= f;

	e20 *= f;
	e21 *= f;
	e22 *= f;

	return (*this);
}

_inl Matrix3D& Matrix3D::operator +=( const float f )
{
	e00 += f;
	e01 += f;
	e02 += f;

	e10 += f;
	e11 += f;
	e12 += f;

	e10 += f;
	e11 += f;
	e12 += f;

	return (*this);
}

_inl Matrix3D& Matrix3D::operator +=( const Matrix3D& r )
{
	e00 += r.e00;
	e01 += r.e01;
	e02 += r.e02;

	e10 += r.e10;
	e11 += r.e11;
	e12 += r.e12;

	e10 += r.e10;
	e11 += r.e11;
	e12 += r.e12;

	return (*this);
}

_inl Vector3D Matrix3D::EulerXYZ() const
{
	Vector3D theta;

	float ce02 = e02;
	clamp( ce02, -1.0f, 1.0f );
	theta.y = asin( ce02 );
	if (theta.y < c_HalfPI)
	{
		if (theta.y > -c_HalfPI)
		{
			theta.x = atan2( -e12, e22 );
			theta.z = atan2( -e01, e00 );
		}
		else
		{
			theta.x = -atan2( e10, e11 );
			theta.z = 0.0f;
		}
	}
	else
	{
		theta.x = atan2( e10, e11 );
		theta.z = 0.0f;
	}
	theta.reverse();
	return theta;
} // Matrix3D::EulerXYZ

_inl void Matrix3D::SetColumns( const Vector3D& v0, const Vector3D& v1, const Vector3D& v2 )
{
	e00 = v0.x; e10 = v0.y; e20 = v0.z;
	e01 = v1.x; e11 = v1.y; e21 = v1.z;
	e02 = v2.x; e12 = v2.y; e22 = v2.z;
} // Matrix3D::SetColumns

_inl void Matrix3D::SetRows( const Vector3D& v0, const Vector3D& v1, const Vector3D& v2 )
{
	e00 = v0.x; e01 = v0.y; e02 = v0.z;
	e10 = v1.x; e11 = v1.y; e12 = v1.z;
	e20 = v2.x; e21 = v2.y; e22 = v2.z;
} // Matrix3D::SetRows

/*****************************************************************
/*	Matrix4D implementation
/*****************************************************************/
_inl Matrix4D::Matrix4D(float _e00, float _e01, float _e02, float _e03,
						float _e10, float _e11, float _e12, float _e13,
						float _e20, float _e21, float _e22, float _e23,
						float _e30, float _e31, float _e32, float _e33 )
{
	e00 = _e00; e01 = _e01; e02 = _e02; e03 = _e03;
	e10 = _e10; e11 = _e11; e12 = _e12; e13 = _e13;
	e20 = _e20; e21 = _e21; e22 = _e22; e23 = _e23;
	e30 = _e30; e31 = _e31; e32 = _e32; e33 = _e33;
}

_inl Matrix4D::Matrix4D( const Vector3D& vx, const Vector3D& vy, const Vector3D& vz )
{
    e00 = vx.x; e01 = vy.x; e02 = vz.x; e03 = 0.0f;
    e10 = vx.y; e11 = vy.y; e12 = vz.y; e13 = 0.0f;
    e20 = vx.z; e21 = vy.z; e22 = vz.z; e23 = 0.0f;
    e30 = 0.0f; e31 = 0.0f; e32 = 0.0f; e33 = 1.0f;
}

_inl Matrix4D::Matrix4D( const Vector3D& vx, const Vector3D& vy, const Vector3D& vz, const Vector3D& tr )
{
    e00 = vx.x; e01 = vy.x; e02 = vz.x; e03 = 0.0f;
    e10 = vx.y; e11 = vy.y; e12 = vz.y; e13 = 0.0f;
    e20 = vx.z; e21 = vy.z; e22 = vz.z; e23 = 0.0f;
    e30 = tr.x; e31 = tr.y; e32 = tr.z; e33 = 1.0f; 
}

_inl void Matrix4D::setRotation( const Matrix3D& m )
{
	e00 = m.e00; e01 = m.e01; e02 = m.e02;
	e10 = m.e10; e11 = m.e11; e12 = m.e12;
	e20 = m.e20; e21 = m.e21; e22 = m.e22;
} // Matrix3D::setRotation

_inl Matrix4D::Matrix4D( const Vector3D& sc, const Quaternion& qrot, const Vector3D& tr )
{
	Matrix3D mrot;
	mrot.rotation( qrot );
	e00 = mrot.e00 * sc.x;	e01 = mrot.e01 * sc.x;	e02 = mrot.e02 * sc.x;	e03 = 0.0f;
	e10 = mrot.e10 * sc.y;	e11 = mrot.e11 * sc.y;	e12 = mrot.e12 * sc.y;	e13 = 0.0f;
	e20 = mrot.e20 * sc.z;	e21 = mrot.e21 * sc.z;	e22 = mrot.e22 * sc.z;	e23 = 0.0f;
	e30 = tr.x;				e31 = tr.y;				e32 = tr.z;				e33 = 1.0f;
}

_inl Matrix4D::Matrix4D( const Vector3D& sc, const Matrix3D& mrot, const Vector3D& tr )
{
	e00 = mrot.e00 * sc.x;	e01 = mrot.e01 * sc.x;	e02 = mrot.e02 * sc.x;	e03 = 0.0f;
	e10 = mrot.e10 * sc.y;	e11 = mrot.e11 * sc.y;	e12 = mrot.e12 * sc.y;	e13 = 0.0f;
	e20 = mrot.e20 * sc.z;	e21 = mrot.e21 * sc.z;	e22 = mrot.e22 * sc.z;	e23 = 0.0f;
	e30 = tr.x;				e31 = tr.y;				e32 = tr.z;				e33 = 1.0f;
}

_inl Matrix4D::Matrix4D( const Vector3D& sc, const Vector3D& rotAxis, float rotAngle, const Vector3D& tr )
{
	Matrix3D mrot;
	mrot.rotation( rotAxis, rotAngle );
	e00 = mrot.e00 * sc.x;	e01 = mrot.e01 * sc.x;	e02 = mrot.e02 * sc.x;	e03 = 0.0f;
	e10 = mrot.e10 * sc.y;	e11 = mrot.e11 * sc.y;	e12 = mrot.e12 * sc.y;	e13 = 0.0f;
	e20 = mrot.e20 * sc.z;	e21 = mrot.e21 * sc.z;	e22 = mrot.e22 * sc.z;	e23 = 0.0f;
	e30 = tr.x;				e31 = tr.y;				e32 = tr.z;				e33 = 1.0f;
}

_inl Matrix4D::Matrix4D( const Matrix3D& m )
{
	e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = 0.0f;
	e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = 0.0f;
	e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = 0.0f;
	e30 = 0.0f;  e31 = 0.0f;  e32 = 0.0f;  e33 = 1.0f;
}

_inl Matrix4D& Matrix4D::mul( const Matrix4D& a, const Matrix4D& b )
{
	getV0().mul( a.getV0(), b );
	getV1().mul( a.getV1(), b );
	getV2().mul( a.getV2(), b );
	getV3().mul( a.getV3(), b );
	return *this;
} // Matrix4D::mul

_inl float Matrix4D::quadraticForm( Vector4D& v )
{
	Vector4D tmp;
	tmp.mul( v, *this );
	return v.dot( tmp );
} // Matrix4D::quadraticForm

_inl bool Matrix4D::equal( const Matrix4D& m ) const
{
	return !memcmp(this,&m,sizeof Matrix4D);
	/*
	return (fabs( e00 - m.e00 ) <= c_SmallEpsilon &&
		fabs( e10 - m.e10 ) <= c_SmallEpsilon &&
		fabs( e20 - m.e20 ) <= c_SmallEpsilon &&
		fabs( e30 - m.e30 ) <= c_SmallEpsilon &&
		fabs( e01 - m.e01 ) <= c_SmallEpsilon &&
		fabs( e11 - m.e11 ) <= c_SmallEpsilon &&
		fabs( e21 - m.e21 ) <= c_SmallEpsilon &&
		fabs( e31 - m.e31 ) <= c_SmallEpsilon &&
		fabs( e02 - m.e02 ) <= c_SmallEpsilon &&
		fabs( e12 - m.e12 ) <= c_SmallEpsilon &&
		fabs( e22 - m.e22 ) <= c_SmallEpsilon &&
		fabs( e32 - m.e32 ) <= c_SmallEpsilon);
	*/
} // Matrix4D::equal

_inl Matrix4D& Matrix4D::operator +=( const Matrix4D& r )
{
	e00 += r.e00; e01 += r.e01; e02 += r.e02; e03 += r.e03;
	e10 += r.e10; e11 += r.e11; e12 += r.e12; e13 += r.e13;
	e20 += r.e20; e21 += r.e21; e22 += r.e22; e23 += r.e23;
	e30 += r.e30; e31 += r.e31; e32 += r.e32; e33 += r.e33;
	return *this;
}

_inl Matrix4D& Matrix4D::operator -=( const Matrix4D& r )
{
	e00 -= r.e00; e01 -= r.e01; e02 -= r.e02; e03 -= r.e03;
	e10 -= r.e10; e11 -= r.e11; e12 -= r.e12; e13 -= r.e13;
	e20 -= r.e20; e21 -= r.e21; e22 -= r.e22; e23 -= r.e23;
	e30 -= r.e30; e31 -= r.e31; e32 -= r.e32; e33 -= r.e33;
	return *this;
}

_inl void Matrix4D::st( float s, const Vector3D& tr )
{
	e00 = s;	e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
	e10 = 0.0f; e11 = s;	e12 = 0.0f; e13 = 0.0f;
	e20 = 0.0f; e21 = 0.0f;	e22 = s;	e23 = 0.0f;
	e30 = tr.x; e31 = tr.y; e32 = tr.z; e33 = 1.0f;
}

_inl void Matrix4D::st( const Vector3D& s, const Vector3D& tr )
{
	e00 = s.x;	e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
	e10 = 0.0f; e11 = s.y;	e12 = 0.0f; e13 = 0.0f;
	e20 = 0.0f; e21 = 0.0f;	e22 = s.z;	e23 = 0.0f;
	e30 = tr.x; e31 = tr.y; e32 = tr.z; e33 = 1.0f;
}

_inl Matrix4D& Matrix4D::mulVV( const Vector4D& v )
{
	e00 = v.x * v.x;
	e11 = v.y * v.y;
	e22 = v.z * v.z;
	e33 = v.w * v.w;

	e01 = e10 = v.x * v.y;
	e02 = e20 = v.x * v.z;
	e03 = e30 = v.x * v.w;
	e12 = e21 = v.y * v.z;
	e13 = e31 = v.y * v.w;
	e23 = e32 = v.z * v.w;

	return *this;
} // Matrix4D::mulVV

_inl Matrix4D& Matrix4D::operator *=( const Matrix4D& r )
{
	Matrix4D tmp;
	tmp.mul( *this, r );
	*this = tmp;
	return *this;
} // Matrix4D::operator *=

_inl Matrix4D& Matrix4D::mulLeft( const Matrix4D& m )
{
	Matrix4D tmp;
	tmp.mul( m, *this );
	*this = tmp;
	return *this;
} // Matrix4D::mulLeft

_inl void Matrix4D::transformPt( Vector3D& pt ) const
{
	float cx = pt.x * e00 + pt.y * e10 + pt.z * e20 + e30;
	float cy = pt.x * e01 + pt.y * e11 + pt.z * e21 + e31;
	float cz = pt.x * e02 + pt.y * e12 + pt.z * e22 + e32;
	pt.x = cx; pt.y = cy; pt.z = cz;
} // Matrix4D::transformPt

_inl void Matrix4D::transformVec( Vector3D& vec ) const
{
	float cx = vec.x * e00 + vec.y * e10 + vec.z * e20;
	float cy = vec.x * e01 + vec.y * e11 + vec.z * e21;
	float cz = vec.x * e02 + vec.y * e12 + vec.z * e22;
	vec.x = cx; vec.y = cy; vec.z = cz;
} // Matrix4D::transformVec

_inl Matrix4D& Matrix4D::operator *=( float w )
{
	e00 *= w; e01 *= w; e02 *= w; 
	e10 *= w; e11 *= w; e12 *= w; 
	e20 *= w; e21 *= w; e22 *= w; 
	e30 *= w; e31 *= w; e32 *= w; 
	return *this;
}

_inl Matrix4D& Matrix4D::addWeighted( const Matrix4D& r, float w )
{
	e00 += r.e00 * w; e01 += r.e01 * w; e02 += r.e02 * w; 
	e10 += r.e10 * w; e11 += r.e11 * w; e12 += r.e12 * w; 
	e20 += r.e20 * w; e21 += r.e21 * w; e22 += r.e22 * w; 
	e30 += r.e30 * w; e31 += r.e31 * w; e32 += r.e32 * w; 
	return *this;
}

_inl float Matrix4D::norm2() const
{
	return	e00*e00 + e01*e01 + e02*e02 + e03*e03 + 
		e10*e10 + e11*e11 + e12*e12 + e13*e13 + 
		e20*e20 + e21*e21 + e22*e22 + e23*e23 + 
		e30*e30 + e31*e31 + e32*e32 + e33*e33;
} // Matrix4D::norm2

_inl const Matrix4D& Matrix4D::operator =( const Matrix4D& matr )
{
    e00 = matr.e00;
    e01 = matr.e01;
    e02 = matr.e02;
    e03 = matr.e03;

    e10 = matr.e10;
    e11 = matr.e11;
    e12 = matr.e12;
    e13 = matr.e13;

    e20 = matr.e20;
    e21 = matr.e21;
    e22 = matr.e22;
    e23 = matr.e23;

    e30 =  matr.e30;
    e31 =  matr.e31;
    e32 =  matr.e32;
    e33 =  matr.e33;
	return *this;
} // Matrix4D::operator =

_inl const Matrix4D& Matrix4D::operator =( const Matrix3D& matr )
{
	e00 = matr.e00;
	e01 = matr.e01;
	e02 = matr.e02;
	e03 = 0.0f;

	e10 = matr.e10;
	e11 = matr.e11;
	e12 = matr.e12;
	e13 = 0.0f;

	e20 = matr.e20;
	e21 = matr.e21;
	e22 = matr.e22;
	e23 = 0.0f;

	e30 = 0.0f;
	e31 = 0.0f;
	e32 = 0.0f;
	e33 = 1.0f;

	return *this;
}

_inl void Matrix4D::setIdentity()
{
	memset( getBuf(), 0, 16 * sizeof( e00 ) );
	e00 = 1.0;
	e11 = 1.0;
	e22 = 1.0;
	e33 = 1.0;
}

_inl void Matrix4D::setNull()
{
	memset( getBuf(), 0, 16 * sizeof( e00 ) );
}

_inl Vector3D Matrix4D::EulerXYZ() const
{
    Vector3D theta;

    float ce02 = e02;
    clamp( ce02, -1.0f, 1.0f );
    theta.y = asin( ce02 );
    if (theta.y < c_HalfPI)
    {
        if (theta.y > -c_HalfPI)
        {
            theta.x = atan2( -e12, e22 );
            theta.z = atan2( -e01, e00 );
        }
        else
        {
            theta.x = -atan2( e10, e11 );
            theta.z = 0.0f;
        }
    }
    else
    {
        theta.x = atan2( e10, e11 );
        theta.z = 0.0f;
    }
    theta.reverse();
    return theta;
} // Matrix4D::EulerXYZ

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::translation
/*	Desc:	builds translation matrix
/*	Parm:	dx, dy, dz - shifts along the correspondent axes
/*---------------------------------------------------------------*/
_inl void Matrix4D::translation( float dx, float dy, float dz )
{
	setIdentity();
	e30 = dx; e31 = dy; e32 = dz;
}//  Matrix4D::translation

_inl void Matrix4D::translate( float dx, float dy, float dz )
{
	e30 += dx; e31 += dy; e32 += dz;
} // Matrix4D::translate

_inl void Matrix4D::translate( const Vector3D& trans )
{
	e30 += trans.x; e31 += trans.y; e32 += trans.z;
} // Matrix4D::translate

_inl void Matrix4D::toBasis(	const Vector3D& bX, 
							const Vector3D& bY, 
							const Vector3D& bZ,
							const Vector3D& trans )
{
	e00 = bX.x; e10 = bX.y; e20 = bX.z;
	e01 = bY.x; e11 = bY.y; e21 = bY.z;
	e02 = bZ.x; e12 = bZ.y; e22 = bZ.z;

	e30 = -trans.x; e31 = -trans.y; e32 = -trans.z;

	e03 = 0.0f; e13 = 0.0f; e23 = 0.0f;
	e33 = 1.0f;
} // Matrix4D::toBasis

_inl void Matrix4D::setTranslation( const Vector3D& tr )
{
	e30 = tr.x; e31 = tr.y; e32 = tr.z;
} // Matrix4D::setTranslation

_inl void Matrix4D::fromBasis(	const Vector3D& bX, 
							  const Vector3D& bY, 
							  const Vector3D& bZ,
							  const Vector3D& trans )
{
	e00 = bX.x; e01 = bX.y; e02 = bX.z;
	e10 = bY.x; e11 = bY.y; e12 = bY.z;
	e20 = bZ.x; e21 = bZ.y; e22 = bZ.z;

	e30 = trans.x; e31 = trans.y; e32 = trans.z;

	e03 = 0.0f; e13 = 0.0f; e23 = 0.0f;
	e33 = 1.0f;
} // Matrix4D::toBasis

_inl void Matrix4D::translation( const Vector3D& trans )
{
	setIdentity();
	e30 = trans.x; e31 = trans.y; e32 = trans.z;
}//  Matrix4D::translation

_inl void Matrix4D::transpose( const Matrix4D& orig )
{
	e00 = orig.e00; e01 = orig.e10; e02 = orig.e20; e03 = orig.e30;
	e10 = orig.e01; e11 = orig.e11; e12 = orig.e21; e13 = orig.e31;
	e20 = orig.e02; e21 = orig.e12; e22 = orig.e22; e23 = orig.e32;
	e30 = orig.e03; e31 = orig.e13; e32 = orig.e23; e33 = orig.e33;
}

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::scaling
/*	Desc:	builds scaling matrix
/*	Parm:	sx, sy, sz - scales along the correspondent axes
/*---------------------------------------------------------------*/
_inl void Matrix4D::scaling( float sx, float sy, float sz )
{
	setIdentity();
	e00 = sx; e11 = sy; e22 = sz;
}

_inl void Matrix4D::scaling( float s )
{
	setIdentity();
	e00 = s; e11 = s; e22 = s;
}

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::rotation
/*	Desc:	builds 4D rotation matrix
/*	Parm:	axis, angle - axis and angle of rotation
/*---------------------------------------------------------------*/
_inl void Matrix4D::rotation( const Vector3D& axis, float angle )
{
	Matrix3D rot;
	rot.rotation( axis, angle );
	*this = rot;
}//  Matrix4D::rotation

_inl void Matrix4D::rotation( const Line3D& axis, float angle )
{
	Matrix4D m, rot;
	Vector3D shift( axis.getOrig() );
	shift.reverse();
	(*this).translation( shift );
	rot.rotation( axis.getDir(), angle );
	(*this) *= rot;
	shift.reverse();
	m.translation( shift );
	(*this) *= m;
} // Matrix4D::rotation

_inl void Matrix4D::srt(	float scale, 
						const Vector3D& axis, float angle, 
						const Vector3D& pos )
{
	Matrix4D tmp;
	scaling( scale, scale, scale );
	tmp.rotation( axis, angle );
	(*this) *= tmp;
	e30 = pos.x;
	e31 = pos.y;
	e32 = pos.z;
} // Matrix4D::srt

_inl void Matrix4D::shearing(	float sxy, float sxz, 
							 float syx, float syz,
							 float szx, float szy )
{
	e00 = 1.0f; e01 = syx;	e02 = szx;	e03 = 0.0f;
	e10 = sxy;  e11 = 1.0f; e12 = szy;  e13 = 0.0f;
	e20 = sxz;  e21 = syz;	e22 = 1;	e23 = 0.0f;
	e30 = 0.0f; e31 = 0.0f; e32 = 0.0f;	e33 = 1.0f;
} // Matrix4D::shearing

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::rotation
/*	Desc:	Builds rotation matrix from the quaternion
/*	Parm:	q - rotation quaternion
/*	Rmrk:	rotation matrix is pre-multiply transform matrix,
/*			in Direct3D style
/*---------------------------------------------------------------*/
_inl void Matrix4D::rotation( const Quaternion& q )
{
	Matrix3D rot;
	rot.rotation( q );
	*this = rot;
}//  Matrix4D::rotation

_inl void Matrix4D::mirrorXoY()
{
	setIdentity();
	e22 = -1.0f;
}

_inl void Matrix4D::mirrorXoZ()
{
	setIdentity();
	e11 = -1.0f;
}

_inl void Matrix4D::mirrorYoZ()
{
	setIdentity();
	e00 = -1.0f;
}

_inl void Matrix4D::flipXY()
{
	setNull();
	e01 = e10 = e22 = e33 = 1.0f;
}

_inl void Matrix4D::flipXZ()
{
	setNull();
	e02 = e20 = e11 = e33 = 1.0f;
}

_inl void Matrix4D::flipYZ()
{
	setNull();
	e12 = e21 = e00 = e33 = 1.0f;
}

_inl int Matrix4D::read( BYTE* sbuf, int maxSize )
{
	assert( maxSize >= sizeof( Matrix4D ) );
	memcpy( this, sbuf, sizeof( Matrix4D ) );
	return sizeof( Matrix4D );
} // Matrix4D::read

_inl int Matrix4D::write( BYTE* sbuf, int maxSize ) const
{
	assert( maxSize >= sizeof( Matrix4D ) );
	memcpy( sbuf, this, sizeof( Matrix4D ) );
	return sizeof( Matrix4D );
} // Matrix4D::write

_inl int Matrix4D::read( FILE* fp )
{
	fread( this, sizeof( Matrix4D ), 1, fp ); 
	return sizeof( Matrix4D );
} // Matrix4D::read

_inl int Matrix4D::write( FILE* fp ) const
{
	fwrite( this, sizeof( Matrix4D ), 1, fp ); 
	return sizeof( Matrix4D );
} // Matrix4D::write

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::affineInverse
/*	Desc:	Inverse of a matrix that is made up of only scales, 
/*			rotations, and translations
/*	Parm:	orig - original matrix
/*---------------------------------------------------------------*/
_inl void Matrix4D::affineInverse( const Matrix4D& orig )
{
	// rotational part is simply transposed 
	e00 = orig.e00;
	e10 = orig.e01;
	e20 = orig.e02;

	e01 = orig.e10;
	e11 = orig.e11;
	e21 = orig.e12;

	e02 = orig.e20;
	e12 = orig.e21;
	e22 = orig.e22;

	e03 = e13 = e23 = 0.0f;
	e33 = 1.0f;

	//  calculate translation part of affine inverse matrix
	e30 = -(e00*orig.e30 + e10*orig.e31 + e20*orig.e32);
	e31 = -(e01*orig.e30 + e11*orig.e31 + e21*orig.e32);
	e32 = -(e02*orig.e30 + e12*orig.e31 + e22*orig.e32);
}//  Matrix4D::affineInverse

_inl void Matrix4D::Blend2( const Matrix4D& ma, float wa, const Matrix4D& mb, float wb )
{
	e00 = ma.e00*wa + mb.e00*wb;
	e01 = ma.e01*wa + mb.e01*wb;
	e02 = ma.e02*wa + mb.e02*wb;

	e10 = ma.e10*wa + mb.e10*wb;
	e11 = ma.e11*wa + mb.e11*wb;
	e12 = ma.e12*wa + mb.e12*wb;

	e20 = ma.e20*wa + mb.e20*wb;
	e21 = ma.e21*wa + mb.e21*wb;
	e22 = ma.e22*wa + mb.e22*wb;

	e30 = ma.e30*wa + mb.e30*wb;
	e31 = ma.e31*wa + mb.e31*wb;
	e32 = ma.e32*wa + mb.e32*wb;
}

_inl void Matrix4D::Blend3(	const Matrix4D& ma, float wa, 
							 const Matrix4D& mb, float wb,
							 const Matrix4D& mc, float wc )
{
	e00 = ma.e00*wa + mb.e00*wb + mc.e00*wc;
	e01 = ma.e01*wa + mb.e01*wb + mc.e01*wc;
	e02 = ma.e02*wa + mb.e02*wb + mc.e02*wc;
							   
	e10 = ma.e10*wa + mb.e10*wb + mc.e10*wc;
	e11 = ma.e11*wa + mb.e11*wb + mc.e11*wc;
	e12 = ma.e12*wa + mb.e12*wb + mc.e12*wc;
							   
	e20 = ma.e20*wa + mb.e20*wb + mc.e20*wc;
	e21 = ma.e21*wa + mb.e21*wb + mc.e21*wc;
	e22 = ma.e22*wa + mb.e22*wb + mc.e22*wc;

	e30 = ma.e30*wa + mb.e30*wb + mc.e30*wc;
	e31 = ma.e31*wa + mb.e31*wb + mc.e31*wc;
	e32 = ma.e32*wa + mb.e32*wb + mc.e32*wc;
}

_inl void Matrix4D::Blend4(	const Matrix4D& ma, float wa, 
							 const Matrix4D& mb, float wb,
							 const Matrix4D& mc, float wc, 
							 const Matrix4D& md, float wd )
{
	e00 = ma.e00*wa + mb.e00*wb + mc.e00*wc + md.e00*wd;
	e01 = ma.e01*wa + mb.e01*wb + mc.e01*wc + md.e01*wd;
	e02 = ma.e02*wa + mb.e02*wb + mc.e02*wc + md.e02*wd;
										   
	e10 = ma.e10*wa + mb.e10*wb + mc.e10*wc + md.e10*wd;
	e11 = ma.e11*wa + mb.e11*wb + mc.e11*wc + md.e11*wd;
	e12 = ma.e12*wa + mb.e12*wb + mc.e12*wc + md.e12*wd;
										   
	e20 = ma.e20*wa + mb.e20*wb + mc.e20*wc + md.e20*wd;
	e21 = ma.e21*wa + mb.e21*wb + mc.e21*wc + md.e21*wd;
	e22 = ma.e22*wa + mb.e22*wb + mc.e22*wc + md.e22*wd;

	e30 = ma.e30*wa + mb.e30*wb + mc.e30*wc + md.e30*wd;
	e31 = ma.e31*wa + mb.e31*wb + mc.e31*wc + md.e31*wd;
	e32 = ma.e32*wa + mb.e32*wb + mc.e32*wc + md.e32*wd;
}

/*****************************************************************
/*	MatrixStack implementation
/*****************************************************************/
_inl MatrixStack::MatrixStack() : m_Top(0)
{
}

_inl void MatrixStack::Push( const Matrix4D& m, bool bOverride )
{
	assert( m_Top < c_MatrixStackDepth - 1 );
	Matrix4D& tr = m_Stack[m_Top]; 
	tr = m;
	if (m_Top > 0 && !bOverride) tr *= m_Stack[m_Top - 1];
	m_Top++;
} // MatrixStack::Push

_inl const Matrix4D& MatrixStack::Pop()
{
	if (m_Top == 0)  return Matrix4D::identity;
	m_Top--;
	return Top();
} // MatrixStack::Pop

_inl const Matrix4D& MatrixStack::Top() const
{
	if (m_Top <= 0) return Matrix4D::identity;
	return m_Stack[m_Top - 1];
} // MatrixStack::Top

_inl void MatrixStack::Reset( const Matrix4D& m )
{
	m_Top       = 1;
	m_Stack[0]	= m;
} //MatrixStack::Reset




