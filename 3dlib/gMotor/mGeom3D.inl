/*****************************************************************************/
/*	File:	mGeom3D.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-25-2003
/*****************************************************************************/
#include <limits>
#undef max

#include "mSphere.h"

/*****************************************************************
/*	Cylinder implementation
/*****************************************************************/
_inl bool Cylinder::IsInfinite() const
{
	return m_Radius < 0.0f;
}

_inl void Cylinder::SetInfinite()
{
	if (m_Radius < 0.0f) return;
	m_Radius = -m_Radius;
}

_inl Vector3D Cylinder::GetBase() const
{
	return m_Core.getOrig();
}

_inl Vector3D Cylinder::GetDir() const
{
	return m_Core.getDir();
}

_inl Vector3D Cylinder::GetTop() const
{
	return m_Core.getDest();
}

_inl Vector3D Cylinder::GetCenter() const
{
	return m_Core.getCenter();
}

_inl float Cylinder::GetRadius() const
{
	return m_Radius;
}

_inl XStatus Cylinder::Intersect( const Plane& plane  ) const
{
	return xsIntersects;
} // Cylinder::Intersect

_inl float Cylinder::GetHeight() const
{
	return m_Core.length();
} // Cylinder::GetHeight

_inl Matrix4D Cylinder::GetWorldTM() const
{
	Matrix4D res;
	Vector3D z( m_Core.getDir() );
	Vector3D x, y;
	z.CreateBasis( x, y );
	res.fromBasis( x, y, z, m_Core.getOrig() );
	return res;
} // Cylinder::GetWorldTM

/*****************************************************************
/*	Cone implementation
/*****************************************************************/
_inl float Cone::GetBaseRadius() const
{
	return GetHeight() * tanf( m_Angle );
} // Cone::GetBaseRadius

_inl float Cone::GetHeight() const
{
	Vector3D dir( m_Top );
	dir -= m_Base;
	return dir.norm();
} // Cone::GetHeight

_inl Vector3D Cone::GetDir() const
{
	Vector3D dir( m_Top );
	dir -= m_Base;
	dir.normalize();
	return dir;
} // Cone::GetDir

_inl Matrix4D Cone::GetWorldTM() const
{
	Matrix4D res;
	Vector3D z( GetDir() );
	Vector3D x, y;
	z.CreateBasis( x, y );
	res.fromBasis( x, y, z, GetBase() );
	return res;
} // Cone::GetWorldTM

_inl Vector3D Cone::RandomDir() const
{
	float r1 = rndValuef();
	float r2 = rndValuef();

	float m1 = 1.0f - r2 * (1.0f - cosf( m_Angle )); 
	float m2 = sqrtf( 1.0f - m1*m1 );
	return Vector3D( m2*cosf( c_DoublePI * r1 ), m2*sinf( c_DoublePI * r1 ), m1 );
} // Cone::RandomDir

/*****************************************************************
/*	Capsule implementation
/*****************************************************************/
_inl Vector3D Capsule::GetBase() const
{
	return seg.getOrig();
}

_inl Vector3D Capsule::GetDir() const
{
	return seg.getDir();
}

_inl Vector3D Capsule::GetTop() const
{
	return seg.getDest();
}

_inl Vector3D Capsule::GetCenter() const
{
	return seg.getCenter();
}

_inl float Capsule::GetRadius() const
{
	return radius;
}

_inl XStatus Capsule::Intersect( const Plane& plane  ) const
{
	return xsIntersects;
}

_inl Sphere Capsule::GetTopSphere() const
{
	return Sphere( GetBase(), GetRadius() );
}

_inl Sphere Capsule::GetBottomSphere() const
{
	return Sphere( GetTop(), GetRadius() );
}

_inl float Capsule::GetHeight() const
{
	return seg.length();
} // Cylinder::GetHeight

_inl Matrix4D Capsule::GetWorldTM() const
{
	Matrix4D res;
	Vector3D z( seg.getDir() );
	Vector3D x, y;
	z.CreateBasis( x, y );
	res.fromBasis( x, y, z, seg.getOrig() );
	return res;
} // Cylinder::GetWorldTM

/*****************************************************************
/*	Frustum implementation
/*****************************************************************/
_inl bool Frustum::PtIn( const Vector3D& pt ) const
{	
	if (!plRight	.OnPositiveSide( pt )) return false;
	if (!plLeft		.OnPositiveSide( pt )) return false;
	if (!plTop		.OnPositiveSide( pt )) return false;
	if (!plBottom	.OnPositiveSide( pt )) return false;
	if (!plNear		.OnPositiveSide( pt )) return false;
	if (!plFar		.OnPositiveSide( pt )) return false;
	return true;
} // Frustum::PtIn

_inl void Frustum::Extrude( float val )
{
	plLeft.		Move( -val );
	plRight.	Move( -val );
	plTop.		Move( -val );
	plBottom.	Move( -val );
	plRight.	Move( -val );
	plNear.		Move( -val );

	UpdateVertices();
} // Frustum::Extrude

_inl void Frustum::Extrude( float left, float top, float right, float bottom )
{
    plLeft.		Move( -left     );
    plRight.	Move( -right    );
    plTop.		Move( -top      );
    plBottom.	Move( -bottom   );
    UpdateVertices();
} // Frustum::Extrude

_inl XStatus Frustum::Intersect( const AABoundBox& aabb ) const
{
	bool bInside = true;
	XStatus status = xsUnknown;

	//  first test for trivial rejection of the aabb/plane tests
	//  (most times there will be early outs in there)
	status = aabb.Intersect( plLeft );
	if (status == xsOutside) return xsOutside; 
	if (status == xsIntersects) bInside = false;

	status = aabb.Intersect( plRight );
	if (status == xsOutside) return xsOutside; 
	if (status == xsIntersects) bInside = false;
	
	status = aabb.Intersect( plFar );
	if (status == xsOutside) return xsOutside; 
	if (status == xsIntersects) bInside = false;

	status = aabb.Intersect( plBottom );
	if (status == xsOutside) return xsOutside; 
	if (status == xsIntersects) bInside = false;

	status = aabb.Intersect( plNear );
	if (status == xsOutside) return xsOutside; 
	if (status == xsIntersects) bInside = false;

	status = aabb.Intersect( plTop );
	if (status == xsOutside) return xsOutside; 
	if (status == xsIntersects) bInside = false;

	if (bInside) return xsInside;  

	//  test aabb segments against frustum
	if (Intersect( aabb.xyz(), aabb.xyZ()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.xyZ(), aabb.xYZ()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.xYZ(), aabb.xYz()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.xYz(), aabb.xyz()) != xsOutside) return xsIntersects;

	if (Intersect( aabb.Xyz(), aabb.XyZ()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.XyZ(), aabb.XYZ()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.XYZ(), aabb.XYz()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.XYz(), aabb.Xyz()) != xsOutside) return xsIntersects;

	if (Intersect( aabb.xyz(), aabb.xYz()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.xyZ(), aabb.xYZ()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.Xyz(), aabb.XYz()) != xsOutside) return xsIntersects;
	if (Intersect( aabb.XyZ(), aabb.XYZ()) != xsOutside) return xsIntersects;

	//  test frustum segments against aabb
	if (aabb.Intersect( ltn(), ltf() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( rtn(), rtf() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( rbn(), rbf() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( lbn(), lbf() ) != xsOutside) return xsIntersects;

	if (aabb.Intersect( ltn(), rtn() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( rtn(), rbn() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( rbn(), lbn() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( lbn(), ltn() ) != xsOutside) return xsIntersects;

	if (aabb.Intersect( ltf(), rtf() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( rtf(), rbf() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( rbf(), lbf() ) != xsOutside) return xsIntersects;
	if (aabb.Intersect( lbf(), ltf() ) != xsOutside) return xsIntersects;

	return xsOutside;
} // Frustum::Intersect

_inl bool Frustum::Overlap( const AABoundBox& aabb ) const
{
	return Intersect( aabb ) != xsOutside;
} // Frustum::Overlap

_inl bool Frustum::Overlap( const Sphere& sphere ) const
{
	if (sphere.Intersect( plLeft	) == xsOutside) return false; 
	if (sphere.Intersect( plRight	) == xsOutside) return false; 
	if (sphere.Intersect( plFar		) == xsOutside) return false; 
	if (sphere.Intersect( plBottom	) == xsOutside) return false; 
	if (sphere.Intersect( plTop		) == xsOutside) return false; 
	if (sphere.Intersect( plNear	) == xsOutside) return false; 
	return true;
} // Frustum::Overlap

_inl bool Frustum::Overlap( const Cylinder&	cylinder ) const
{
	if (cylinder.Intersect( plLeft	) == xsOutside) return false; 
	if (cylinder.Intersect( plRight	) == xsOutside) return false; 
	if (cylinder.Intersect( plFar	) == xsOutside) return false; 
	if (cylinder.Intersect( plBottom) == xsOutside) return false; 
	if (cylinder.Intersect( plTop	) == xsOutside) return false; 
	if (cylinder.Intersect( plNear	) == xsOutside) return false; 
	return true;
} // Frustum::Overlap

_inl bool Frustum::Overlap( const Capsule&	capsule ) const
{
	if (capsule.Intersect( plLeft	) == xsOutside) return false; 
	if (capsule.Intersect( plRight	) == xsOutside) return false; 
	if (capsule.Intersect( plFar	) == xsOutside) return false; 
	if (capsule.Intersect( plBottom	) == xsOutside) return false; 
	if (capsule.Intersect( plTop	) == xsOutside) return false; 
	if (capsule.Intersect( plNear	) == xsOutside) return false; 
	return true;
} // Frustum::Overlap

_inl  XStatus Frustum::Intersect( const Vector3D& orig, const Vector3D& dest ) const
{
	Vector3D dir; dir.sub( dest, orig );
	float inD  =  -std::numeric_limits<float>::max();
	float outD =  std::numeric_limits<float>::max();

	//  for each plane
	for (int i = 0; i < 6; i++)
	{
		const Plane& pl = GetPlane( i );
		float dirDot  = pl.normal().dot( dir );
		float origDot = pl.normal().dot( orig );
		
		if (fabs( dirDot ) <= c_SmallEpsilon)
		//  ray is parallel to plane
		{
			if (origDot + pl.d < 0) return xsOutside;
		}
		else
		{
			float hitDist = (-pl.d - origDot) / dirDot;  
			if (dirDot > 0.0f) 
			{ 
				if (hitDist > inD) inD = hitDist; 
			} 
			else 
			{ 
				if (hitDist < outD) outD = hitDist; 
			} 
		}
		if (inD > outD) return xsOutside;
	}

	if (inD  >= 0.0f && inD  <= 1.0f) return xsIntersects;
	if (outD >= 0.0f && outD <= 1.0f) return xsIntersects;
	if (inD  < 0.0f && outD > 1.0f) return xsInside;
	return xsOutside;
} // Frustum::Intersect

/*****************************************************************
/*	AABoundBox implementation
/*****************************************************************/
_inl AABoundBox::AABoundBox()
{
}

_inl AABoundBox::AABoundBox( float minx, float miny, float minz, float maxx, float maxy, float maxz )
{
    minv = Vector3D( minx, miny, minz );
    maxv = Vector3D( maxx, maxy, maxz );
}

_inl AABoundBox::AABoundBox( const Vector3D& _minv, const Vector3D& _maxv )
{
	minv.copy( _minv );
	maxv.copy( _maxv );
}

_inl AABoundBox::AABoundBox( const Vector3D& center, float halfSide )
{
	minv.x = center.x - halfSide;
	minv.y = center.y - halfSide;
	minv.z = center.z - halfSide;

	maxv.x = center.x + halfSide;
	maxv.y = center.y + halfSide;
	maxv.z = center.z + halfSide;
}

_inl AABoundBox::AABoundBox( const Vector3D& center, float cx, float cy, float cz )
{
	minv.x = center.x - cx;
	minv.y = center.y - cy;
	minv.z = center.z - cz;

	maxv.x = center.x + cx;
	maxv.y = center.y + cy;
	maxv.z = center.z + cz;
}

_inl AABoundBox::AABoundBox( const Rct& base, float minZ, float maxZ )
{
	minv.x = base.x;
	minv.y = base.y;
	minv.z = minZ;

	maxv.x = base.x + base.w;
	maxv.y = base.y + base.h;
	maxv.z = maxZ;
} // AABoundBox::AABoundBox

_inl void AABoundBox::GetCenter( Vector3D& dest ) const
{
	dest.copy( minv );
	dest += maxv;
	dest /= 2.0f;
} // AABoundBox::GetCenter

_inl Vector3D AABoundBox::GetCenter() const
{
	return Vector3D( (minv.x + maxv.x)*0.5f, (minv.y + maxv.y)*0.5f, (minv.z + maxv.z)*0.5f );
}

_inl float AABoundBox::GetDiagonal() const
{
	return minv.distance( maxv );
}

_inl float AABoundBox::Distance2( const Vector3D& p ) const
{
	float dx = p.x < minv.x ? (minv.x - p.x) : (p.x > maxv.x ? ( p.x - maxv.x) : 0.0f);
	float dy = p.y < minv.y ? (minv.y - p.y) : (p.y > maxv.y ? ( p.y - maxv.y) : 0.0f);
	float dz = p.z < minv.z ? (minv.z - p.z) : (p.z > maxv.z ? ( p.z - maxv.z) : 0.0f);
	return dx*dx + dy*dy + dz*dz;
} // AABoundBox::Distance2

_inl float AABoundBox::Distance( const Vector3D& p ) const
{
	return sqrtf( Distance2( p ) );
}

_inl void AABoundBox::Union( const AABoundBox& orig )
{
	if (minv.x > orig.minv.x) minv.x = orig.minv.x;
	if (minv.y > orig.minv.y) minv.y = orig.minv.y;
	if (minv.z > orig.minv.z) minv.z = orig.minv.z;

	if (maxv.x < orig.maxv.x) maxv.x = orig.maxv.x;
	if (maxv.y < orig.maxv.y) maxv.y = orig.maxv.y;
	if (maxv.z < orig.maxv.z) maxv.z = orig.maxv.z;
} // AABoundBox::Union

_inl void AABoundBox::Copy( const AABoundBox& orig )
{
	minv.copy( orig.minv );
	maxv.copy( orig.maxv );
} // AABoundBox::Copy

/*---------------------------------------------------------------------------*/
/*	Func:	AABoundBox::Transform
/*	Desc:	Finds axis-aligned bounding box after object's transformation
/*	Parm:	matr - transform matrix
/*---------------------------------------------------------------------------*/
_inl void AABoundBox::Transform( const Matrix4D& matr )
{
    Vector3D nmin, nmax;
    Vector3D vc[8];
    GetCorners( vc );
    vc[0] *= matr;
    minv.copy( vc[0] );
    maxv.copy( minv );
    for (int i = 1; i < 8; i++)
    {
        //  transform corner
        vc[i] *= matr;

        if (minv.x > vc[i].x) minv.x = vc[i].x;
        if (maxv.x < vc[i].x) maxv.x = vc[i].x;

        if (minv.y > vc[i].y) minv.y = vc[i].y;
        if (maxv.y < vc[i].y) maxv.y = vc[i].y;

        if (minv.z > vc[i].z) minv.z = vc[i].z;
        if (maxv.z < vc[i].z) maxv.z = vc[i].z;
    }
} // AABoundBox::Transform

/*---------------------------------------------------------------------------*/
/*	Func:	AABoundBox::GetCorners	
/*	Desc:	returns coordinates of all AABB corners
/*	Parm:	c - reference to array of 8 vectors
/*---------------------------------------------------------------------------*/
_inl void AABoundBox::GetCorners( Vector3D (&c)[8] ) const
{
    c[0].set( minv.x, minv.y, minv.z );
    c[1].set( maxv.x, minv.y, minv.z );
    c[2].set( minv.x, maxv.y, minv.z );
    c[3].set( maxv.x, maxv.y, minv.z );
    c[4].set( minv.x, minv.y, maxv.z );
    c[5].set( maxv.x, minv.y, maxv.z );
    c[6].set( minv.x, maxv.y, maxv.z );
    c[7].set( maxv.x, maxv.y, maxv.z );
} // AABoundBox::GetCorners

_inl XStatus AABoundBox::Intersect( const Plane& plane ) const
{
	Vector3D neg, pos;
	if (plane.a > 0.0f)
	{
		if (plane.b > 0.0f) 
		{ 
			if (plane.c > 0.0f) 
			{ 
				pos.set( maxv.x, maxv.y, maxv.z ); 
				neg.set( minv.x, minv.y, minv.z ); 
			}
			else 
			{ 
				pos.set( maxv.x, maxv.y, minv.z ); 
				neg.set( minv.x, minv.y, maxv.z ); 
			} 
		}
		else 
		{ 
			if (plane.c > 0.0f) 
			{ 
				pos.set( maxv.x, minv.y, maxv.z ); 
				neg.set( minv.x, maxv.y, minv.z ); 
			}
			else 
			{ 
				pos.set( maxv.x, minv.y, minv.z ); 
				neg.set( minv.x, maxv.y, maxv.z ); 
			} 
		}
	}
	else
	{	
		if (plane.b > 0.0f) 
		{ 
			if (plane.c > 0.0f) 
			{ 
				pos.set( minv.x, maxv.y, maxv.z ); 
				neg.set( maxv.x, minv.y, minv.z ); 
			}
			else 
			{ 
				pos.set( minv.x, maxv.y, minv.z ); 
				neg.set( maxv.x, minv.y, maxv.z ); 
			} 
		}
		else 
		{ 
			if (plane.c > 0.0f) 
			{ 
				pos.set( minv.x, minv.y, maxv.z ); 
				neg.set( maxv.x, maxv.y, minv.z ); 
			}
			else 
			{ 
				pos.set( minv.x, minv.y, minv.z ); 
				neg.set( maxv.x, maxv.y, maxv.z ); 
			} 
		}
	}

	if (plane.dist2Pt( pos ) < 0.0f) 
	{
		return xsOutside;
	}

	if (plane.dist2Pt( neg ) > 0.0f) 
	{
		return xsInside;
	}
	
	return xsIntersects;
} // AABoundBox::Intersect (Plane)

_inl bool AABoundBox::Overlap( const AABoundBox& aabb ) const
{
    return !IsOutside( aabb );
} // AABoundBox::Overlap    

_inl bool AABoundBox::IsOutside( const AABoundBox& aabb ) const
{
    return  (maxv.x < aabb.minv.x) || (minv.x > aabb.maxv.x) ||
            (maxv.y < aabb.minv.y) || (minv.y > aabb.maxv.y) ||
            (maxv.z < aabb.minv.z) || (minv.z > aabb.maxv.z);
} // AABoundBox::IsOutside

_inl XStatus AABoundBox::Intersect( const AABoundBox& aabb ) const
{
    if (IsOutside( aabb )) return xsOutside;
    //  FIXME
	return xsIntersects;
} // AABoundBox::Intersect (AABoundBox)

//  AABB/segment intersection
//  TODO: could slightly optimize this by unrolling loop and
//		substituting aabb plane equations
_inl XStatus AABoundBox::Intersect( const Vector3D& a, const Vector3D& b ) const
{
	Vector3D dir; dir.sub( b, a );
	float inD  =  -std::numeric_limits<float>::max();
	float outD =  std::numeric_limits<float>::max();

	//  for each plane
	for (int i = 0; i < 6; i++)
	{
		const Plane pl = GetPlane( i );
		float dirDot  = pl.normal().dot( dir );
		float origDot = pl.normal().dot( a );

		if (fabs( dirDot ) <= c_SmallEpsilon)
			//  ray is parallel to plane
		{
			if (origDot + pl.d < 0) return xsOutside;
		}
		else
		{
			float hitDist = (-pl.d - origDot) / dirDot;  
			if (dirDot > 0.0f) { inD = tmax( hitDist, inD ); } else { outD = tmin( hitDist, outD ); } 
		}
		if (inD > outD) return xsOutside;
	}

	if (inD  >= 0.0f && inD  <= 1.0f) return xsIntersects;
	if (outD >= 0.0f && outD <= 1.0f) return xsIntersects;
	if (inD  <  0.0f && outD >  1.0f) return xsInside;
	return xsOutside;
} // AABoundBox::Intersect

_inl Plane AABoundBox::GetPlane( int idx ) const
{
			if (idx == 0) return Plane( 1.0f, 0.0f, 0.0f, -minv.x );
	else	if (idx == 1) return Plane( -1.0f, 0.0f, 0.0f, maxv.x );
	else	if (idx == 2) return Plane( 0.0f, 1.0f, 0.0f, -minv.y );
	else	if (idx == 3) return Plane( 0.0f, -1.0f, 0.0f, maxv.y );
	else	if (idx == 4) return Plane( 0.0f, 0.0f, 1.0f, -minv.z );
	else	if (idx == 5) return Plane( 0.0f, 0.0f, -1.0f, maxv.z );
	assert( false );
	return Plane::xOy;
} // AABoundBox::GetPlane

_inl bool AABoundBox::PtIn( const Vector3D& pt ) const
{
	return	pt.x >= minv.x && pt.x <= maxv.x &&
			pt.y >= minv.y && pt.y <= maxv.y &&
			pt.z >= minv.z && pt.z <= maxv.z;
} // AABoundBox::PtIn

_inl bool AABoundBox::Overlap( const Ray3D& ray ) const
{
	Vector3D pt;
	
	bool AABBRayX(	const AABoundBox& aabb, 
					const Line3D& ray, Vector3D& point );

	return AABBRayX( *this, ray, pt );
} // AABoundBox::Overlap (Ray3D)

_inl bool AABoundBox::OverlapSegment( const Segment3D& seg ) const
{
	assert( false );
	return false;
} // AABoundBox::OverlapSegment

_inl bool AABoundBox::Overlap( const Sphere& sphere ) const
{
	assert( false );
	return false;
} // AABoundBox::Overlap (Sphere)

_inl void AABoundBox::Extend( const Vector3D& pt )
{
	if (minv.x > pt.x) minv.x = pt.x;
	if (maxv.x < pt.x) maxv.x = pt.x;

	if (minv.y > pt.y) minv.y = pt.y;
	if (maxv.y < pt.y) maxv.y = pt.y;

	if (minv.z > pt.z) minv.z = pt.z;
	if (maxv.z < pt.z) maxv.z = pt.z;
} // AABoundBox::InsertPoint

_inl void AABoundBox::Extend( const Triangle& tri )
{
	Extend( tri.GetAABB() );
} // AABoundBox::InsertPoint

_inl void AABoundBox::Extend( const AABoundBox& aabb )
{
	if (minv.x > aabb.minv.x) minv.x = aabb.minv.x;
	if (maxv.x < aabb.maxv.x) maxv.x = aabb.maxv.x;

	if (minv.y > aabb.minv.y) minv.y = aabb.minv.y;
	if (maxv.y < aabb.maxv.y) maxv.y = aabb.maxv.y;

	if (minv.z > aabb.minv.z) minv.z = aabb.minv.z;
	if (maxv.z < aabb.maxv.z) maxv.z = aabb.maxv.z;

} // AABoundBox::InsertPoint

_inl void AABoundBox::Set( float minx, float miny, float minz, float maxx, float maxy, float maxz )
{
	minv.set( minx, miny, minz );
	maxv.set( maxx, maxy, maxz );
} // AABoundBox::Set

_inl Sphere AABoundBox::GetBoundSphere() const
{
	return Sphere( GetCenter(), GetDiagonal() * 0.5f );
} // AABoundBox::GetBoundSphere

_inl bool AABoundBox::operator ==( const AABoundBox& orig ) const
{
    return (minv.isEqual( orig.minv ) && maxv.isEqual( orig.maxv ));
}

/*****************************************************************
/*	Triangle implementation
/*****************************************************************/
_inl XStatus Triangle::Intersect( const Plane& plane ) const
{
	float da = plane.dist2Pt( a );
	float db = plane.dist2Pt( b );
	float dc = plane.dist2Pt( c );
	if (da > 0.0f && db > 0.0f && dc > 0.0f) return xsOutside;
	if (da <= 0.0f && db <= 0.0f && dc <= 0.0f) return xsInside;
	return xsIntersects;
} // Triangle::Intersect

_inl float Triangle::Distance( const Ray3D& ray )
{
	Vector3D pa( a );
	Vector3D pb( b );
	Vector3D pc( c );
	
	pa -= ray.getOrig();
	pb -= ray.getOrig();
	pc -= ray.getOrig();
	
	float da = pa.dot( ray.getDir() );
	float db = pb.dot( ray.getDir() );
	float dc = pc.dot( ray.getDir() );
	
	if (fabs( da ) > fabs( db ) && fabs( da ) > fabs( dc )) return da;
	if (fabs( db ) > fabs( da ) && fabs( db ) > fabs( dc )) return db;
	return dc;
} // Triangle::Distance

_inl int Triangle::Clip( const Plane& pl, Vector3D (&pt)[2] )
{
	int nClip = 0;
	if (pl.ClipSegment( Ray3D( a, b ), pt[nClip] )) nClip++;
	if (pl.ClipSegment( Ray3D( b, c ), pt[nClip] )) nClip++;
	if (nClip < 2 && pl.ClipSegment( Ray3D( a, c ), pt[nClip] )) nClip++;
	if (nClip == 2)
	{
		if (pt[0].isEqual( pt[1] )) nClip--;
	}
	return nClip;
} // Triangle::Clip

_inl Plane Triangle::GetPlane() const
{
	Plane pl;
	pl.from3Points( a, b, c );
	return pl;
} // Triangle::GetPlane

_inl AABoundBox Triangle::GetAABB() const
{
	return AABoundBox(	Vector3D( tmin( a.x, b.x, c.x ), tmin( a.y, b.y, c.y ), tmin( a.z, b.z, c.z ) ), 
						Vector3D( tmax( a.x, b.x, c.x ), tmax( a.y, b.y, c.y ), tmax( a.z, b.z, c.z ) ) );
}

_inl const Vector3D& Triangle::ArgminX() const
{
	return (a.x <= b.x && a.x <= c.x) ? a : ((b.x <= a.x && b.x <= c.x) ? b : c);
}

_inl const Vector3D& Triangle::ArgmaxX() const
{
	return (a.x >= b.x && a.x >= c.x) ? a : ((b.x >= a.x && b.x >= c.x) ? b : c);
}

_inl const Vector3D& Triangle::ArgminY() const
{
	return (a.y <= b.y && a.y <= c.y) ? a : ((b.y <= a.y && b.y <= c.y) ? b : c);
}

_inl const Vector3D& Triangle::ArgmaxY() const
{
	return (a.y >= b.y && a.y >= c.y) ? a : ((b.y >= a.y && b.y >= c.y) ? b : c);
}

_inl const Vector3D& Triangle::ArgminZ() const
{
	return (a.z <= b.z && a.z <= c.z) ? a : ((b.z <= a.z && b.z <= c.z) ? b : c);
}

_inl const Vector3D& Triangle::ArgmaxZ() const
{
	return (a.z >= b.z && a.z >= c.z) ? a : ((b.z >= a.z && b.z >= c.z) ? b : c);
}

