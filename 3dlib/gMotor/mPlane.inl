/*****************************************************************
/*  File:   mPlane.inl                                          
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "mTransform.h"
/*****************************************************************
/*	Plane implementation
/*****************************************************************/
_inl float Plane::from3Points( const Vector3D& v1, const Vector3D& v2, const Vector3D& v3 )
{
	Vector3D av, bv;
	av.sub( v1, v2 );
	bv.sub( v3, v2 );
	Vector3D* normal = reinterpret_cast<Vector3D*>( &a );

	normal->cross( av, bv );
	float area = normal->normalize();
	d = -normal->dot( v2 );
	return area*0.5f;
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

_inl Vector3D Plane::GetPoint() const
{
	if (fabs( a ) > c_SmallEpsilon) return Vector3D( -d/a, 0.0f, 0.0f );
	if (fabs( b ) > c_SmallEpsilon) return Vector3D( 0.0f, -d/b, 0.0f );
	if (fabs( c ) > c_SmallEpsilon) return Vector3D( 0.0f, 0.0f, -d/c );
	assert( false );
	return Vector3D::null;
} // Plane::GetPoint

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

_inl Ray3D Plane::Mirror( const Ray3D& ray ) const
{
	float t = -ray.getOrig().dot( normal() ) - d;
	Vector3D proj( ray.getOrig() );
	proj.addWeighted( normal(), 2.0f * t );

	float rt = -(ray.getOrig().dot( normal() ) + d)/(ray.getDir().dot( normal() ));
	Vector3D rdir( ray.getOrig() );
	rdir.addWeighted( ray.getDir(), rt );
	rdir -= proj;
	rdir.normalize();

	return Ray3D( proj, rdir );
} // Plane::Mirror

_inl void Plane::ProjectVec( Vector3D& vec ) const
{
	float pr = vec.dot( normal() );
	vec.x -= a * pr;
	vec.y -= b * pr;
	vec.z -= c * pr;
} // Plane::ProjectVec

_inl void Plane::ProjectPt( Vector3D& pt ) const
{
	ProjectVec( pt );
	pt.addWeighted( normal(), d );
} // Plane::ProjectPt

//  decompose onto plane normal/plane projection components
_inl void Plane::Decompose( const Vector3D& vec, Vector3D& normC, Vector3D& projC ) const
{
	float pr = vec.dot( normal() );
	normC = normal();
	normC *= pr;
	projC.sub( vec, normC );
} // Plane::Decompose

_inl bool Plane::OnPositiveSide( const Vector3D& v ) const
{
	return a*v.x + b*v.y + c*v.z + d >= 0.0f;
}





