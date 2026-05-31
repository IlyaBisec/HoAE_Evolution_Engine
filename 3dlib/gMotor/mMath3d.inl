/*****************************************************************
/*  File:   mMath3D.inl                                          
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "mQuaternion.h"

_inl float rndValuef()
{
    float rndval = static_cast<float>( rand() );
    rndval /= (float)RAND_MAX;
    return rndval;
}

_inl float rndValuef( float min, float max )
{
    return min + static_cast<float>( max - min ) * rndValuef(); 
}

_inl float RadToDeg( float rad ) 
{ 
	return 180.0f * rad / c_PI; 
}

_inl float DegToRad( float deg ) 
{ 
	return c_PI * deg / 180.0f; 
}

template <typename T>
void SmoothFilter( T* arr, int width, int height )
{
	int w1 = width	- 1;
	int h1 = height - 1;
	int cp = width + 1;
	int val;
	for (int i = 1; i < w1; i++)
	{
		for (int j = 1; j < h1; j++)
		{
			val =
				arr[cp - width - 1] + arr[cp - width] + arr[cp - width + 1]	+
				arr[cp		   - 1]	+ arr[cp		] + arr[cp		   + 1]	+ 
				arr[cp + width - 1]	+ arr[cp + width] + arr[cp + width + 1];
			val /= 9;
			arr[cp] = val;
			cp++;
		}
		cp += 2;
	}
}

_inl float minf( float a, float b )
{
	return (a < b) ? a : b;
}
_inl float maxf( float a, float b )
{
	return (a > b) ? a : b;
}

_inl float minf( float a, float b, float c )
{
	return minf( minf( a, b ), c );
}
_inl float maxf( float a, float b, float c )
{
	return maxf( maxf( a, b ), c );
}

_inl float 
minf( float a, float b, float c, float d )
{
	return minf( minf( a, b ), minf( c, d ) );
}

_inl float maxf( float a, float b, float c, float d )
{
	return maxf( maxf( a, b ), maxf( c, d ) );
}

_inl float minf(	float a, float b, float c, float d, 
					float e, float f, float g, float h )
{
	return minf(	minf( a, b, c, d ),
					minf( e, f, g, h ) );
}

_inl float maxf(	float a, float b, float c, float d, 
		float e, float f, float g, float h )
{
	return maxf(	maxf( a, b, c, d ),
					maxf( e, f, g, h ) );
}

_inl float mini( float a, float b )
{
	return (a < b) ? a : b;
}
_inl float maxi( float a, float b )
{
	return (a > b) ? a : b;
}

_inl float mini( float a, float b, float c, float d )
{
	return mini( mini( a, b ), mini( c, d ) );
}

_inl float maxi( float a, float b, float c, float d )
{
	return maxi( maxi( a, b ), maxi( c, d ) );
}

/*****************************************************************
/*	Vector3D implementation
/*****************************************************************/
_inl Vector3D::Vector3D( float _x, 
						 float _y, 
						 float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

_inl Vector3D::Vector3D( const Vector3D& orig )
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
}

_inl Vector3D::Vector3D( const Vector4D& orig )
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
}

_inl float Vector3D::TriArea( const Vector3D& v ) const 
{
	Vector3D cr( y * v.z - z * v.y, 
				 x * v.z - z * v.x,
				 x * v.y - y * v.x );

	return static_cast<float>( sqrt( cr.x * cr.x + 
									 cr.y * cr.y + 
									 cr.z * cr.z ) / 2 ); 
}

_inl float Vector3D::angle( const Vector3D& v )
{
	float ang = acos( dot( v ) / (norm() * v.norm()) );
	return ang;
} // Vector3D::angle

_inl void Vector3D::cross( const Vector3D& v1, const Vector3D& v2 ) 
{
	x = v1.y * v2.z - v1.z * v2.y; 
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}

_inl void Vector3D::set( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

_inl void Vector3D::sub( const Vector3D& v1, const Vector3D& v2 )
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
	z = v1.z - v2.z;
}

_inl void Vector3D::add( const Vector3D& v1, const Vector3D& v2 )
{
	x = v1.x + v2.x;
	y = v1.y + v2.y;
	z = v1.z + v2.z;
}

_inl void Vector3D::sub( const Vector3D& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

_inl void Vector3D::add( const Vector3D& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
}

_inl void Vector3D::addWeighted( const Vector3D& v, float weight )
{
	x += v.x * weight;
	y += v.y * weight;
	z += v.z * weight;
}

_inl void Vector3D::addWeighted( const Vector3D& v1, const Vector3D& v2,
								 float w1, float w2 )
{
	x = v1.x * w1 + v2.x * w2;
	y = v1.y * w1 + v2.y * w2;
	z = v1.z * w1 + v2.z * w2;
}

_inl void Vector3D::zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

_inl float Vector3D::norm() const
{
	return static_cast<float>( sqrt( x * x + y * y + z * z ) );
}

_inl float Vector3D::distance( const Vector3D& orig ) const
{
	Vector3D tmp;
	tmp.copy( *this );
	tmp -= orig;
	return tmp.norm();
}

_inl float Vector3D::distance2( const Vector3D& orig ) const
{
	Vector3D tmp;
	tmp.copy( *this );
	tmp -= orig;
	return tmp.norm2();
}

_inl void Vector3D::reverse( const Vector3D& orig )
{
	x = -orig.x;
	y = -orig.y;
	z = -orig.z;
}

_inl void Vector3D::random( float minX, float maxX, 
							float minY, float maxY, 
							float minZ, float maxZ )
{
	x = rndValuef( minX, maxX );
	y = rndValuef( minY, maxY );
	z = rndValuef( minZ, maxZ );
} // Vector3D::random

_inl void Vector3D::reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

_inl float Vector3D::norm2() const
{
	return x * x + y * y + z * z;
}

_inl bool Vector3D::isEqual( const Vector3D& orig, float eps ) const
{
	return (fabs( x - orig.x ) < eps && 
			fabs( y - orig.y ) < eps && 
			fabs( z - orig.z ) < eps);  
} // Vector3D::isEqual

_inl bool Vector3D::isColinear( const Vector3D& orig, float eps ) const
{
	return	(fabs( x*orig.y - y*orig.x ) + fabs( x*orig.z - z*orig.x )) / norm2() <= eps;
} // Vector3D::isColinear

_inl void Vector3D::planeNormal(	const Vector3D& v1,
									const Vector3D& v2,
									const Vector3D& v3 )
{
	Vector3D t1, t2;
	t1.sub( v1, v2 );
	t2.sub( v3, v2 );
	cross( t2, t1 ); 
}

_inl float Vector3D::dot( const Vector3D& v ) const
{
	return x * v.x + y * v.y + z * v.z;
}

_inl const Vector3D& Vector3D::operator +=( const Vector3D& vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator -=( const Vector3D& vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator *=( const float val )
{
	x *= val;
	y *= val;
	z *= val;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator *=( const Matrix3D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e10 + z * m.e20;
	tmp.y = x * m.e01 + y * m.e11 + z * m.e21;
	tmp.z = x * m.e02 + y * m.e12 + z * m.e22;

	(*this) = tmp;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator *=( const Matrix4D& m )
{
	float cx = x * m.e00 + y * m.e10 + z * m.e20 + m.e30;
	float cy = x * m.e01 + y * m.e11 + z * m.e21 + m.e31;
	float cz = x * m.e02 + y * m.e12 + z * m.e22 + m.e32;
	x = cx; y = cy; z = cz;
	return *this;
}

_inl const Vector3D& 
Vector3D::mulPt( const Vector3D& v, const Matrix4D& m )
{
	float cx = v.x * m.e00 + v.y * m.e10 + v.z * m.e20 + m.e30;
	float cy = v.x * m.e01 + v.y * m.e11 + v.z * m.e21 + m.e31;
	float cz = v.x * m.e02 + v.y * m.e12 + v.z * m.e22 + m.e32;
	x = cx; y = cy; z = cz;
	return *this;
}

_inl const Vector3D& 
Vector3D::mulPt( const Matrix4D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e10 + z * m.e20 + m.e30;
	tmp.y = x * m.e01 + y * m.e11 + z * m.e21 + m.e31;
	tmp.z = x * m.e02 + y * m.e12 + z * m.e22 + m.e32;
	(*this) = tmp;
	return (*this);
}

_inl const Vector3D& 
Vector3D::mulLeft( const Matrix3D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e01 + z * m.e02;
	tmp.y = x * m.e10 + y * m.e11 + z * m.e12;
	tmp.z = x * m.e20 + y * m.e21 + z * m.e22;

	(*this) = tmp;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator /=( const float val )
{
	x /= val;
	y /= val;
	z /= val;
	return *this;
}

_inl void 
Vector3D::normalize()
{
	float len = static_cast<float>( sqrt( x * x + y * y + z * z) );
    if (len < c_Epsilon) return;
	x /= len;
	y /= len;
	z /= len;
}

_inl void	
Vector3D::copy( const Vector3D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
}

_inl void
Vector3D::copy( const Vector4D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
}

_inl const Vector3D& Vector3D::operator=( const Vector4D& orig )
{
	if (orig.w != 1.0f && orig.w > c_Epsilon)
	{
		x = orig.x / orig.w;
		y = orig.y / orig.w;
		z = orig.z / orig.w;
	}
	else
	{
		x = orig.x;
		y = orig.y;
		z = orig.z;
	}
	return *this; 
}

_inl const Vector3D& Vector3D::operator=( const Vector3D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
	return *this;
}

_inl void Vector3D::PlaneIntersectLine( Plane& p, const Vector3D& v1, const Vector3D& v2 )
{
	Vector3D v;
	v.sub( v2, v1 );
	float coef = p.normal().dot( v1 );
	coef += p.d;
	coef /= p.normal().dot( v );
	v *= coef;
	sub( v1, v );
} // Vector3D::PlaneIntersectLine

_inl void Vector3D::fromSpherical( float rho, float theta, float phi )
{
	float sinTheta = sinf( theta );
	x = rho * sinTheta * cos( phi );
	y = rho * sinTheta * sin( phi );
	z = rho * cos( theta );
} // Vector3D::fromSpherical

/*****************************************************************
/*	Matrix3D implementation
/*****************************************************************/
_inl Matrix3D::Matrix3D( const Matrix4D& m )
{
	e00 = m.e00; e01 = m.e01; e02 = m.e02;
	e10 = m.e10; e11 = m.e11; e12 = m.e12;
	e20 = m.e20; e21 = m.e21; e22 = m.e22;
}

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
/*	Rmrk:	rotation matrix is pre-multiply transform matrix,
/*			in Direct3D style
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

/*---------------------------------------------------------------*
/*  Func:	Matrix3D::rotation
/*	Desc:	Builds rotation matrix from axis of rotation and angle
/*	Parm:	axis  - axis of rotation
/*			angle - rotation angle 
/*---------------------------------------------------------------*/
_inl void Matrix3D::rotation( const Vector3D& axis, float angle )
{
	Quaternion q;
	q.fromAxisAngle( axis, angle );
	rotation( q );
} // Matrix3D::RotationFromAxisAngle

/*---------------------------------------------------------------*
/*  Func:	Matrix3D::rotationFromEuler
/*	Desc:	Builds rotation matrix from Euler angles
/*	Parm:	yaw		- yaw around the y-axis
/*			pitch	- pitch around the x-axis
/*			roll	- roll around the z-axis
/*---------------------------------------------------------------*/
_inl void Matrix3D::rotationFromEuler( float yaw, float pitch, float roll )
{
	float sinYaw	= sin( yaw );
	float cosYaw	= cos( yaw );
	float sinPitch	= sin( pitch );
	float cosPitch	= cos( pitch );
	float sinRoll	= sin( roll );
	float cosRoll	= cos( roll );
	
	assert( false ); // TODO
}


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
/*	Desc:	finds inverse matrix with Kramer's rule 
/*	Ret:	matrix determinant
/*---------------------------------------------------------------------------*/
_inl float Matrix3D::inverse( const Matrix3D& m )
{
	long double d = m.det();
	if (fabs( d ) < c_SmallEpsilon) return d;
	
	float* e  = getBuf();
	e00 = (-m.e12*m.e21 + m.e11*m.e22) / d;
	e01 = ( m.e02*m.e21 - m.e01*m.e22) / d;
	e02 = (-m.e02*m.e11 + m.e01*m.e12) / d;

	e10 = ( m.e12*m.e20 - m.e10*m.e22) / d;
	e11 = (-m.e02*m.e20 + m.e00*m.e22) / d;
	e12 = ( m.e02*m.e10 - m.e00*m.e12) / d;

	e20 = (-m.e11*m.e20 + m.e10*m.e21) / d;
	e21 = ( m.e01*m.e20 - m.e00*m.e21) / d;
	e22 = (-m.e01*m.e10 + m.e00*m.e11) / d;

	return d;
} // Matrix3D::inverse

_inl float Matrix3D::inverse()
{
	Matrix3D tmp( *this );
	return inverse( tmp );
} // Matrix3D::inverse

_inl bool Matrix3D::isOrthonormalBasis() const
{
	return true;
} // Matrix3D::isOrthonormalBasis


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
	register float tmp = e01; e01 = e10; e10 = tmp;
	tmp = e21; e21 = e20; e20 = tmp;
	tmp = e21; e21 = e12; e12 = tmp;
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

_inl Matrix3D& Matrix3D::operator *=( const float f )
{
	e00 *= f;
	e01 *= f;
	e02 *= f;

	e10 *= f;
	e11 *= f;
	e12 *= f;

	e10 *= f;
	e11 *= f;
	e12 *= f;

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
	return theta;
} // Matrix3D::EulerXYZ

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

_inl Matrix4D::Matrix4D()
{
	setNull();
}

_inl Matrix4D::Matrix4D( const Vector3D& sc, const Quaternion& qrot, const Vector3D& tr )
{
	Matrix3D mrot;
	mrot.rotation( qrot );
	e00 = mrot.e00 * sc.x;	e01 = mrot.e01 * sc.y;	e02 = mrot.e02 * sc.z;	e03 = 0.0f;
	e10 = mrot.e10 * sc.x;	e11 = mrot.e11 * sc.y;	e12 = mrot.e12 * sc.z;	e13 = 0.0f;
	e20 = mrot.e20 * sc.x;	e21 = mrot.e21 * sc.y;	e22 = mrot.e22 * sc.z;	e23 = 0.0f;
	e30 = tr.x;				e31 = tr.y;				e32 = tr.z;				e33 = 1.0f;
}

_inl Matrix4D::Matrix4D( const Vector3D& sc, const Matrix3D& mrot, const Vector3D& tr )
{
	e00 = mrot.e00 * sc.x;	e01 = mrot.e01 * sc.y;	e02 = mrot.e02 * sc.z;	e03 = 0.0f;
	e10 = mrot.e10 * sc.x;	e11 = mrot.e11 * sc.y;	e12 = mrot.e12 * sc.z;	e13 = 0.0f;
	e20 = mrot.e20 * sc.x;	e21 = mrot.e21 * sc.y;	e22 = mrot.e22 * sc.z;	e23 = 0.0f;
	e30 = tr.x;				e31 = tr.y;				e32 = tr.z;				e33 = 1.0f;
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

_inl bool Matrix4D::equal( const Matrix4D& m )
{
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

_inl void Matrix4D::transformPt( Vector3D& pt  ) const
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

_inl Matrix4D& Matrix4D::addWeighted( const Matrix4D& r, float w )
{
	e00 += r.e00 * w; e01 += r.e01 * w; e02 += r.e02 * w; e03 += r.e03 * w;
	e10 += r.e10 * w; e11 += r.e11 * w; e12 += r.e12 * w; e13 += r.e13 * w;
	e20 += r.e20 * w; e21 += r.e21 * w; e22 += r.e22 * w; e23 += r.e23 * w;
	e30 += r.e30 * w; e31 += r.e31 * w; e32 += r.e32 * w; e33 += r.e33 * w;
	return *this;
}

_inl float Matrix4D::norm2() const
{
	return	e00*e00 + e01*e01 + e02*e02 + e03*e03 + 
			e10*e10 + e11*e11 + e12*e12 + e13*e13 + 
			e20*e20 + e21*e21 + e22*e22 + e23*e23 + 
			e30*e30 + e31*e31 + e32*e32 + e33*e33;
} // Matrix4D::norm2

_inl Matrix4D& Matrix4D::operator *=( float w )
{
	e00 *= w; e01 *= w; e02 *= w; e03 *= w;
	e10 *= w; e11 *= w; e12 *= w; e13 *= w;
	e20 *= w; e21 *= w; e22 *= w; e23 *= w;
	e30 *= w; e31 *= w; e32 *= w; e33 *= w;
	return *this;
}

_inl const Matrix4D& Matrix4D::operator =( const Matrix4D& matr )
{
	memcpy( getBuf(), matr.getBuf(), 16 * sizeof( e00 ) );
	return *this;
}

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
	float Tx, Ty, Tz;

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

 	Tx = orig.e30;
	Ty = orig.e31;
	Tz = orig.e32;

	//  calculate translation part of affine inverse matrix
	e30 = -( e00 * Tx + e10 * Ty + e20 * Tz );
	e31 = -( e01 * Tx + e11 * Ty + e21 * Tz );
	e32 = -( e02 * Tx + e12 * Ty + e22 * Tz );
  
}//  Matrix4D::affineInverse

/*****************************************************************
/*	MatrixStack implementation
/*****************************************************************/
_inl MatrixStack::MatrixStack()
{
	top = 0;
}

_inl void MatrixStack::Push( const Matrix4D& m )
{
	assert( top < c_MatrixStackDepth - 1 );
	Matrix4D& tr = stack[top]; 
	tr = m;
	if (top > 0) tr *= stack[top - 1];
	top++;
} // MatrixStack::Push

_inl const Matrix4D& MatrixStack::Pop()
{
	if (top == 0)
	{
		return Matrix4D::identity;
	}
	top--;
	return Top();
} // MatrixStack::Pop

_inl const Matrix4D& MatrixStack::Top() const
{
	if (top <= 0) return Matrix4D::identity;
	return stack[top - 1];
} // MatrixStack::Top

_inl void MatrixStack::Reset( const Matrix4D& m )
{
	top			= 1;
	stack[0]	= m;
} //MatrixStack::Reset

/*****************************************************************
/*	Vector4D implementation
/*****************************************************************/
_inl Vector4D& Vector4D::mul( const Vector4D& v, const Matrix4D& m )
{
	x = v.x * m.e00 + v.y * m.e10 + v.z * m.e20 + v.w * m.e30;
	y = v.x * m.e01 + v.y * m.e11 + v.z * m.e21 + v.w * m.e31;
	z = v.x * m.e02 + v.y * m.e12 + v.z * m.e22 + v.w * m.e32;
	w = v.x * m.e03 + v.y * m.e13 + v.z * m.e23 + v.w * m.e33;
	return *this;
}

_inl Vector4D& Vector4D::mul( const Matrix4D& m, const Vector4D& v )
{
	x = v.x * m.e00 + v.y * m.e01 + v.z * m.e02 + v.w * m.e03;
	y = v.x * m.e10 + v.y * m.e11 + v.z * m.e12 + v.w * m.e13;
	z = v.x * m.e20 + v.y * m.e21 + v.z * m.e22 + v.w * m.e23;
	w = v.x * m.e30 + v.y * m.e31 + v.z * m.e32 + v.w * m.e33;
	return *this;
}

// v = v * M
_inl Vector4D& Vector4D::mul( const Matrix4D& m )
{
	Vector4D tmp; 
	tmp.x = x * m.e00 + y * m.e10 + z * m.e20 + w * m.e30;
	tmp.y = x * m.e01 + y * m.e11 + z * m.e21 + w * m.e31;
	tmp.z = x * m.e02 + y * m.e12 + z * m.e22 + w * m.e32;
	tmp.w = x * m.e03 + y * m.e13 + z * m.e23 + w * m.e33;
	*this = tmp;
	return *this;
}

_inl Vector4D&	Vector4D::mulWeighted( const Matrix4D& m, float weight )
{
	mul( m );
	x *= weight;
	y *= weight;
	z *= weight;
	return *this;
}

_inl Vector4D& Vector4D::operator *=( const Matrix4D& m )
{
	return mul( m );
}

//  v = M * v
_inl Vector4D& Vector4D::transform( const Matrix4D& m )
{
	Vector4D tmp; 
	tmp.x = x * m.e00 + y * m.e01 + z * m.e02 + w * m.e03;
	tmp.y = x * m.e10 + y * m.e11 + z * m.e12 + w * m.e13;
	tmp.z = x * m.e20 + y * m.e21 + z * m.e22 + w * m.e23;
	tmp.w = x * m.e30 + y * m.e31 + z * m.e32 + w * m.e33;
	*this = tmp;
	return *this;
}

_inl void Vector4D::set( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
	w = 1.0f;
}

_inl void Vector4D::set( float _x, float _y, float _z, float _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

_inl void Vector4D::sub( const Vector4D& v1, const Vector4D& v2 )
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
	z = v1.z - v2.z;
	w = v1.w - v2.w;
}

_inl void Vector4D::add( const Vector4D& v1, const Vector4D& v2 )
{
	x = v1.x + v2.x;
	y = v1.y + v2.y;
	z = v1.z + v2.z;
	w = v1.w + v2.w;
}

_inl void Vector4D::sub( const Vector4D& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

_inl void Vector4D::add( const Vector4D& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

_inl float Vector4D::norm() const
{
	return static_cast<float>( sqrt( x * x + y * y + z * z  + w * w ) );
}

_inl float Vector4D::norm2() const
{
	return x * x + y * y + z * z + w * w;
}

_inl float Vector4D::distance( const Vector4D& orig ) const
{
	Vector4D tmp;
	tmp.copy( *this );
	tmp -= orig;
	return tmp.norm();
}

_inl float		
Vector4D::dot( const Vector4D& v ) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

_inl const Vector4D& 
Vector4D::operator +=( const Vector4D& vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

_inl const Vector4D& Vector4D::operator -=( const Vector4D& vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

_inl const Vector4D& Vector4D::operator *=( const float val )
{
	x *= val;
	y *= val;
	z *= val;
	w *= val;
	return *this;
}

_inl const Vector4D& Vector4D::operator /=( const float val )
{
	x /= val;
	y /= val;
	z /= val;
	w /= val;
	return *this;
}

_inl void Vector4D::normalize()
{
	float len = static_cast<float>( sqrt( x * x + y * y + z * z + w * w) );
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

_inl void Vector4D::normW()
{
	if (fabs( w ) > c_SmallEpsilon)
	{
		x /= w;
		y /= w;
		z /= w;
		w = 1.0f;
	}
}

_inl void Vector4D::reverse()
{
	x = -x; y = -y; z = -z;
}

_inl void Vector4D::reverse( const Vector4D& orig )
{
	x = -orig.x; y = -orig.y; z = -orig.z; 
	w = orig.w;
}

_inl const Vector4D& Vector4D::copy( const Vector4D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
	w = orig.w;
	return *this;
}

_inl const Vector4D& Vector4D::operator =( const Vector3D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
	w = 1.0f;
	return *this;
}

_inl const Vector4D& Vector4D::operator =( const Vector4D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
	w = orig.w;
	return *this;
}

_inl float Vector4D::mul( const Vector4D& v )
{
	return v.x * x + v.y * y + v.z * z + v.w * w;
}

/*****************************************************************
/*  Quad3D implementation                                
/*****************************************************************/
void _inl 
Quad3D::CreateRect( const Vector3D& topLeft, 
							  const Vector3D& botRight,
							  const Vector3D& botLeft )
{
	a = topLeft;
	c = botRight;
	d = botLeft;

	b = a;
	b.add( c );
	b.sub( d );
} // Quad3D::CreateRect

/*---------------------------------------------------------------*
/*  Func:	Quad3D::getArea
/*	Desc:	Finds the area of the quad with given four vertices
/*				coords. It is assumed that vertices are in CW or
/*				CCW order
/*---------------------------------------------------------------*/
_inl float Quad3D::getArea() const
{	
	Vector3D v1, v2, v3, v4;
	v1.sub( b, a );
	v2.sub( d, a );
	v3.sub( b, c );
	v4.sub( d, c );
	return v1.TriArea( v2 ) + v3.TriArea( v4 );
} // Math3D::QuadArea

/*****************************************************************
/*	Plane implementation
/*****************************************************************/
_inl void Plane::from3Points(	const Vector3D& v1, 
								const Vector3D& v2,
								const Vector3D& v3 )
{
	Vector3D av, bv;
	av.sub( v1, v2 );
	bv.sub( v3, v2 );
	Vector3D* normal = reinterpret_cast<Vector3D*>( &a );

	normal->cross( av, bv );
	normal->normalize();
	d = - normal->dot( v2 );
} // Plane::from3Points

_inl bool Plane::ClipSegment( const Segment3D& ray, Vector3D& pt ) const
{
	float alpha = dist2Pt( ray.getOrig() );
	const Vector3D& dir = ray.getDir();
	float det = dir.x * a + dir.y * b + dir.z * c;
	if (fabs( det ) < c_SmallEpsilon) return false;
	alpha /= -det;
	if (alpha < 0.0f || alpha > 1.0f) return false;
	pt = ray.getOrig(); 
	pt.addWeighted( dir, alpha );
	return true;
} // Plane::ClipSegment

_inl bool Plane::Contains( const Vector3D& pt, float eps ) const
{
	return fabs( pt.dot( normal() ) + d ) <= eps;
} // Plane::Contains

_inl void Plane::fromPointNormal(	const Vector3D& pt, 
									const Vector3D& norm )
{
	a = norm.x;
	b = norm.y;
	c = norm.z;
	d = - pt.dot( norm );
}

_inl void Plane::fromPointNormal( const Vector4D& pt, 
								  const Vector4D& norm )
{
	a = norm.x;
	b = norm.y;
	c = norm.z;
	d = - pt.x * norm.x - pt.y * norm.y - pt.z * norm.z;
}

_inl float Plane::getZ( float x, float y ) const
{
	if (fabs( c ) <= c_SmallEpsilon) return 0.0f;
	return (-a*x - b*y - d) / c;
}

_inl float Plane::getY( float x, float z ) const
{
	if (fabs( b ) <= c_SmallEpsilon) return 0.0f;
	return (-a*x - c*z - d) / b;
}

_inl float Plane::getX( float y, float z ) const
{
	if (fabs( a ) <= c_SmallEpsilon) return 0.0f;
	return (-b*y - c*z - d) / a;
}

_inl bool Plane::isPerpendicular( const Plane& p ) const
{
	//  check the dot product of plane normals
	return fabs( a * p.a + b * p.b + c * p.c ) <= c_SmallEpsilon;
}

_inl Vector4D& Plane::asVector()
{
	return *(reinterpret_cast<Vector4D*>( &a ));
}

_inl const Vector4D& Plane::asVector() const 
{
	return *(reinterpret_cast<const Vector4D*>( &a ));
}

_inl void Plane::txtSave( FILE* fp )
{
	asVector().txtSave( fp );
}

_inl void Plane::normalize()
{
	float n = normal().norm();
	a /= n;
	b /= n;
	c /= n;
	d /= n;
} // Plane::normalize

/*---------------------------------------------------------------------------*/
/*	Func:	Plane::intersect
/*	Desc:	finds intersection point of three planes
/*	Parm:	p1 - 2nd plane
/*			p2 - 3rd plane
/*			pt - intersection point to return 
/*	Ret:	true if such point exist
/*---------------------------------------------------------------------------*/
_inl bool Plane::intersect( const Plane& p1, const Plane& p2, Vector3D& pt ) const
{
	Matrix3D A, Ainv;
	A.e00 = a;
	A.e10 = b;
	A.e20 = c;

	A.e01 = p1.a;
	A.e11 = p1.b;
	A.e21 = p1.c;

	A.e02 = p2.a;
	A.e12 = p2.b;
	A.e22 = p2.c;

	float det = Ainv.inverse( A );
	if (fabs( det ) < c_SmallEpsilon) return false;

	pt.x = -d;
	pt.y = -p1.d;
	pt.z = -p2.d;

	pt *= Ainv;
	return true;
}

_inl bool Plane::intersect( const Line3D& ray, Vector3D& pt ) const
{
	return ray.IntersectPlane( *this, pt );
} // Plane::intersect

_inl float Plane::dist2Pt( const Vector3D& v ) const
{
	return a * v.x + b * v.y + c * v.z + d;
}

_inl void Plane::MoveToPoint( const Vector3D& pt )
{
	d = -pt.dot( normal() );
} // Plane::MoveToPoint

/*****************************************************************************/
/*	Line3D implementation
/*****************************************************************************/
_inl float Line3D::project( const Vector3D& vec ) const
{
	Vector3D pt( vec );
	pt -= orig;
	return pt.dot( dir );
}

_inl float Line3D::dist2ToOrig( const Vector3D& vec ) const
{
	Vector3D pt( vec );
	pt -= orig;
	return pt.norm2();
}

_inl float Line3D::dist2ToPoint( const Vector3D& pt ) const
{
	Vector3D v;
	v.sub( pt, orig );
	float k2 = v.dot( dir );
	k2 *= k2;
	return v.norm2() - k2;
} // Line3D::dist2ToPoint
	
/*---------------------------------------------------------------------------*/
/*	Func:	Line3D::getPoint	
/*	Desc:	Finds point on the line	
/*	Parm:	prLen - distance of the result line point from line origin
/*---------------------------------------------------------------------------*/
_inl void Line3D::getPoint( float prLen, Vector3D& pt ) const
{
	pt.copy( dir );
	pt *= prLen;
	pt += orig;
}

_inl void Line3D::Normalize()
{
	float norm = dir.norm2();
	if (fabs( norm - 1.0f ) < c_SmallEpsilon) return;
	dir /= sqrtf( norm );
}

_inl bool Line3D::IntersectPlane( const Plane& pl, Vector3D& interPt ) const
{
	float alpha = orig.x*pl.a + orig.y*pl.b + orig.z*pl.c + pl.d;
	float det = dir.x*pl.a + dir.y*pl.b + dir.z*pl.c;
	if (fabs( det ) < c_SmallEpsilon) return false;
	interPt = orig;
	interPt.addWeighted( dir, -alpha / det );
	return true;
} // Line3D::IntersectPlane

_inl void Line3D::Transform( const Matrix4D& tr )
{
	Vector4D ndir( dir );
	ndir.x += orig.x; ndir.y += orig.y; ndir.z += orig.z;

	orig *= tr; 
	ndir.mul( tr );
	ndir -= orig;
	dir = ndir;
	dir.normalize();
} // Line3D::Transform

_inl Vector3D Line3D::getPoint( float prLen ) const
{
	Vector3D tmp( dir );
	tmp *= prLen;
	tmp += orig;
	return tmp;
}

_inl void Line3D::setOrigDir( const Vector3D& _orig, 
							  const Vector3D& _dir )
{
	orig.copy( _orig );
	dir.copy( _dir );
}

_inl void Line3D::setOrig( float x, float y, float z )
{
	orig.x = x;
	orig.y = y;
	orig.z = z;
} // Line3D::setOrig

_inl const Vector3D& Line3D::getOrig()	const
{
	return orig;
}

_inl const Vector3D& Line3D::getDir() const
{
	return dir;
}

_inl Vector3D Line3D::getDest() const
{
	Vector3D res( dir );
	res += orig;
	return res;
}

_inl Vector3D Line3D::getCenter() const
{
	Vector3D res( dir );
	res *= 0.5f;
	res += orig;
	return res;
}

_inl float Line3D::length()	const
{
	return dir.norm();
}

_inl bool Line3D::LineContains( const Vector3D& pt, float eps ) const
{
	Vector3D diff( pt );
	diff -= getOrig();
	return diff.isColinear( getDir(), eps );
} // Line3D::LineContains

_inl bool Line3D::SegmentContains( const Vector3D& pt, float eps) const
{
	Vector3D diff( pt );
	diff -= getOrig();
	float dot = diff.dot( getDir() );
	return diff.isColinear( getDir(), eps ) && (dot >= 0.0f) && (dot <= getDir().norm2());
} // Line3D::SegmentContains

/*****************************************************************
/*	Global functions implementation
/*****************************************************************/
/*---------------------------------------------------------------*
/*  Func:  RaySphereX                                            *
/*  Desc:  Founds if ray intersects sphere                       *
/*  Parm:  orig, dir - ray origin/direction                      *
/*		   center - sphere center								 *
/*		   R2 - sphere radius squared							 *
/*  Ret:   true if ray intersects sphere                         *
/*  Rmrk:  dir should be normalized!!!							 *
/*---------------------------------------------------------------*/
_inl bool RaySphereX( const Vector3D& orig, const Vector3D& dir,
   					  const Vector3D& center, float R2 )
{
	Vector3D tmp;
	tmp.sub( center, orig );
	float dirPr = tmp.dot( dir );
	dirPr *= dirPr;
	return tmp.norm2() - dirPr <= R2;
} // RaySphereX




