/*****************************************************************************/
/*	File:	mSphere.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-25-2003
/*****************************************************************************/
#include <limits>
#undef max

/*****************************************************************
/*	Sphere implementation
/*****************************************************************/
_inl Sphere::Sphere( const Vector3D& center, float radius )
{
	m_Center = center; 
	m_Radius = radius;
}

_inl Sphere::Sphere( const Vector3D& a, const Vector3D& b )
{
    m_Center.add( a, b );
    m_Center *= 0.5f;
    m_Radius = m_Center.distance( a );
}

_inl Sphere::Sphere( const Vector3D& a, const Vector3D& b, const Vector3D& c )
{
    assert( false );
}

_inl Sphere::Sphere( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d )
{
    assert( false );    
} // Sphere::Sphere

_inl Sphere& Sphere::operator +=( const Sphere& sphere )
{
	Vector3D dir( sphere.GetCenter() );
	dir -= GetCenter();
	float d = dir.normalize();
	const float r = GetRadius();
	const float R = sphere.GetRadius();
	float pmin = tmin( -r, r, d - r, d - R, d + R );
	float pmax = tmax( -r, r, d - r, d - R, d + R );
	
	float nR = (pmax - pmin) * 0.5f;
	float c  = (pmax + pmin) * 0.5f;

	dir *= c;
	dir += GetCenter();
	SetCenter( dir );
	SetRadius( nR );
	return *this;
} // Sphere::operator +=

_inl XStatus Sphere::Intersect( const AABoundBox& aabb ) const
{
	return xsUnknown;
} // Sphere::Intersects

_inl XStatus Sphere::Intersect( const Sphere& sphere ) const
{
	Vector3D diff( m_Center );
	diff.sub( sphere.m_Center );
	float sqrLen = diff.norm2();
	float rsum = m_Radius + sphere.m_Radius;
	rsum *= rsum;
	if (sqrLen > rsum) return xsOutside;
	return xsIntersects; // TODO!!!!
} // Sphere::Intersect

_inl bool Sphere::Overlap( const Sphere& sphere ) const
{
	Vector3D diff( m_Center );
	diff.sub( sphere.m_Center );
	if (diff.norm2() > (m_Radius + sphere.m_Radius)*(m_Radius + sphere.m_Radius)) return false;
	return true;
} // Sphere::Overlap

_inl XStatus Sphere::Intersect( const Plane& plane  ) const
{
	float d = plane.dist2Pt( m_Center );
	if (d < -m_Radius) return xsOutside;
	if (d > m_Radius) return xsInside;
	return xsIntersects;
} // Sphere::Intersect (Plane)

_inl Vector3D Sphere::RandomPoint()
{
	float r1 = rndValuef();
	float r2 = rndValuef();
	float m  = sqrtf( r2 * (1.0f - r2) );

	Vector3D res( 2.0f * m_Radius * cosf( c_DoublePI * r1 )*m, 
				  2.0f * m_Radius * sinf( c_DoublePI * r1 )*m, 
				  m_Radius * (1.0f - 2.0f*r2) );
	res += GetCenter();
	return res;
} // Sphere::RandomPoint

_inl void Sphere::Transform( const Matrix4D& tm )
{
	tm.transformPt( m_Center );
	m_Radius *= tm.getV0().norm();  
} // Sphere::Transform

_inl void Sphere::ProjectPt( Vector3D& pt ) const
{
	pt -= m_Center;
	pt.normalize();
	pt *= m_Radius;
} // Sphere::ProjectPt

_inl void Sphere::Decompose( const Vector3D& pos, const Vector3D& dir, 
							 Vector3D& normC, Vector3D& tanC ) const
{
	normC.sub( pos, m_Center );
	normC.normalize();
	normC *= normC.dot( dir );
	tanC.sub( dir, normC );
} // Sphere::Decompose


