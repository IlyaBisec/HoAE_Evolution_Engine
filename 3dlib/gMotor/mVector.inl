/*****************************************************************
/*  File:   mVector.inl                                          
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "mTransform.h"
#include "mPlane.h"
#include "mUtil.h"
#include "mRandom.h"

/*****************************************************************
/*	Vector3D implementation
/*****************************************************************/
_inl Vector3D::Vector3D( const Vector4D& orig ) : x(orig.x), y(orig.y), z(orig.z) 
{
}

_inl float Vector3D::Angle( const Vector3D& v )
{
	float ang = acos( dot( v ) / (norm() * v.norm()) );
	return ang;
} // Vector3D::angle

_inl float Vector3D::Angle( const Vector3D& v, const Vector3D& n )
{
	Vector3D cvec;
	cvec.cross( *this, v );
	float ang = acos( dot( v ) / (norm() * v.norm()) );
	if (cvec.dot( n ) > 0.0f) ang = c_DoublePI - ang;
	return ang;
} // Vector3D::Angle

_inl void Vector3D::cross( const Vector3D& v1, const Vector3D& v2 ) 
{
	x = v1.y * v2.z - v1.z * v2.y; 
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
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

_inl void Vector3D::centroid( const Vector3D& v0, const Vector3D& v1, const Vector3D& v2 )
{
    const float c_OneThird = 1.0f/3.0f;
    x = (v0.x + v1.x + v2.x)*c_OneThird;
    y = (v0.y + v1.y + v2.y)*c_OneThird;
    z = (v0.z + v1.z + v2.z)*c_OneThird;
} // Vector3D::centroid

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

_inl void Vector3D::round()
{
	x = roundf( x );
	y = roundf( y );
	z = roundf( z );
}

_inl float Vector3D::norm() const
{
	return sqrtf( x * x + y * y + z * z );
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

_inl const Vector3D& Vector3D::operator *=( const Vector3D& vec )
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	return *this;
}

_inl const Vector3D& Vector3D::operator *=( const Matrix3D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e10 + z * m.e20;
	tmp.y = x * m.e01 + y * m.e11 + z * m.e21;
	tmp.z = x * m.e02 + y * m.e12 + z * m.e22;

	(*this) = tmp;
	return *this;
}

//  for Manowar
_inl const Vector3D& Vector3D::operator *=( const Matrix4D& m )
{
	float cx = x * m.e00 + y * m.e10 + z * m.e20 + m.e30;
	float cy = x * m.e01 + y * m.e11 + z * m.e21 + m.e31;
	float cz = x * m.e02 + y * m.e12 + z * m.e22 + m.e32;
	x = cx; y = cy; z = cz;
	return *this;
}

_inl const Vector3D& Vector3D::mulPt( const Vector3D& v, const Matrix4D& m )
{
	float cx = v.x * m.e00 + v.y * m.e10 + v.z * m.e20 + m.e30;
	float cy = v.x * m.e01 + v.y * m.e11 + v.z * m.e21 + m.e31;
	float cz = v.x * m.e02 + v.y * m.e12 + v.z * m.e22 + m.e32;
	x = cx; y = cy; z = cz;
	return *this;
}

_inl const Vector3D& Vector3D::mulPt( const Matrix4D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e10 + z * m.e20 + m.e30;
	tmp.y = x * m.e01 + y * m.e11 + z * m.e21 + m.e31;
	tmp.z = x * m.e02 + y * m.e12 + z * m.e22 + m.e32;
	(*this) = tmp;
	return (*this);
}

_inl const Vector3D& Vector3D::mulLeft( const Matrix3D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e01 + z * m.e02;
	tmp.y = x * m.e10 + y * m.e11 + z * m.e12;
	tmp.z = x * m.e20 + y * m.e21 + z * m.e22;

	(*this) = tmp;
	return *this;
}

_inl const Vector3D& Vector3D::operator /=( const float val )
{
	x /= val;
	y /= val;
	z /= val;
	return *this;
}

_inl float Vector3D::normalize()
{
	float len = sqrtf( x*x + y*y + z*z );
    //if (len < c_Epsilon) return 0.0f;
    if(len>0){
	    x /= len;
	    y /= len;
	    z /= len;
    }
	return len;
}

#define IEEE_1_0 0x3f800000
_inl float Vector3D::normalize_fast()
{   return normalize();
    const float val = x*x + y*y + z*z;
    DWORD tmp = (DWORD(IEEE_1_0 << 1) + IEEE_1_0 - *(DWORD*)&val) >> 1;   
	const float d = *(float*)&tmp;                                             
	const float mul = d*(1.47f - 0.47f*val*d*d);
    x *= mul;
    y *= mul;
    z *= mul;
    return mul;
} // Vector3D::normalize_fast

_inl void Vector3D::copy( const Vector3D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
}

_inl void Vector3D::copy( const Vector4D& orig )
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

_inl void Vector3D::FromSpherical( float rho, float theta, float phi )
{
	float sinTheta = sinf( theta );
	x = rho * sinTheta * cos( phi );
	y = rho * sinTheta * sin( phi );
	z = rho * cos( theta );
} // Vector3D::FromSpherical

/*****************************************************************
/*	Vector4D implementation
/*****************************************************************/
_inl Vector4D::Vector4D( float _x, float _y, float _z, float _w ) : x(_x), y(_y), z(_z), w(_w)
{}

_inl Vector4D::Vector4D( const Vector4D& orig ) : x(orig.x), y(orig.y), z(orig.z), w(orig.w) 
{}

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

// for Manowar
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

_inl void Vector4D::cross( const Vector4D& v1, const Vector4D& v2 ) 
{
    x = v1.y * v2.z - v1.z * v2.y; 
    y = v1.z * v2.x - v1.x * v2.z;
    z = v1.x * v2.y - v1.y * v2.x;
}

_inl float Vector4D::dot( const Vector4D& v ) const
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
    if(len>0){    
	    x /= len;
	    y /= len;
	    z /= len;
	    w /= len;
    }
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

_inl bool Vector4D::isEqual( const Vector4D& orig, float eps ) const
{
    return (fabs( x - orig.x ) < eps && fabs( y - orig.y ) < eps && 
            fabs( z - orig.z ) < eps && fabs( w - orig.w ) < eps);  
} // Vector4D::isEqual

_inl void Vector4D::random( float minX, float maxX, 
                           float minY, float maxY, 
                           float minZ, float maxZ )
{
    x = rndValuef( minX, maxX );
    y = rndValuef( minY, maxY );
    z = rndValuef( minZ, maxZ );
    w = rndValuef( 0.0f, 1.0f );
} // Vector4D::random

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

float Line3D::dist2( const Line3D& line ) const
{
    assert( false );
    Vector3D d; d.sub( orig, line.orig );    
    float l1 = dir.norm2();
    float dirDot = -dir.dot( line.dir );
    float l2 = line.dir.norm2();
    float dDot1 = d.dot( dir );
    float ld = d.norm2();
    float det = fabs( l1*l2 - dirDot*dirDot );
    
    if (det >= c_Epsilon)
    // lines are not parallel 
    {
        float dDot2 = -d.dot( line.dir );
        float idet = 1.0f / det;
        float s = (dirDot*dDot2-l2*dDot1)*idet;
        float t = (dirDot*dDot1-l1*dDot2)*idet;
        return s*(l1*s + dirDot*t + 2.0f*dDot1) + t*(dirDot*s + l2*t + 2.0f*dDot2) + ld;
    }
    else
    // lines are parallel
    {
        return -dDot1*dDot1 + ld;
    }
} // Line3D::dist2

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

_inl void Line3D::setOrig( const Vector3D& v )
{
	orig = v;
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

_inl float Line3D::GetZ( float x, float y ) const
{
	if (fabs( dir.x ) > c_SmallEpsilon)
	{
		return orig.z + dir.z*(x - orig.x)/dir.x;
	}
	else if (fabs( dir.y ) > c_SmallEpsilon)
	{
		return orig.z + dir.z*(y - orig.y)/dir.y;
	}
	return orig.z;
} // Line3D::GetZ


