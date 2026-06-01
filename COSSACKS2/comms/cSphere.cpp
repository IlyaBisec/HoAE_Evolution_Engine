#include "comms.h"

//-----------------------------------------------------------------------------
// cSphere::SignedDistanceToPlane : float(const cPlane &) const
//-----------------------------------------------------------------------------
float cSphere::SignedDistanceToPlane(const cPlane &P) const {
	float d = P.SignedDistanceToPoint(m_Center);
	if(d > m_Radius) {
		return d - m_Radius;
	}
	if(d < - m_Radius) {
		return d + m_Radius;
	}
	return 0.0f;
}

//------------------------------------------------------------------------------------------------------------------
// cSphere::RayIntersection : bool(const cVec3 &, const cVec3 &, float &, float &) const
//------------------------------------------------------------------------------------------------------------------
bool cSphere::RayIntersection(const cVec3 &RayOrig, const cVec3 &RayDir, float &Scale1, float &Scale2) const {
	// C - sphere center
	// R - sphere radius
	// O - ray origin
	// D - ray direction
	// The equation for a sphere centered at 'C' is:
	//	dot(p - C, p - C) - R^2 = 0.
	// The parameterized by 's' equation for a ray is:
	//	p = s * D + O.
	// Plugging the ray equation into the sphere equation, we get:
	//	dot(s * D + O - C, s * D + O - C) - R^2 = 0.
	// To solve, we'll need to get this into a * s^2 + b * s + c = 0 form to apply
	//	the quadratic formula to find the two solutions s = (- b +/- sqrt(b^2 - 4 * a * c)) / (2 * a).
	// Using distributive property of dot products:
	//	dot(D, D) * s^2 + 2 * dot(D, O - C) * s + dot(O - C, O - C) - R^2 = 0.
	//	a = dot(D, D)
	//	b = 2 * dot(D, O - C)
	//	c = dot(O - C, O - C) - R^2
	// If discriminant b^2 - 4 * a * c is negative, there are no intersections, and the ray misses the sphere.
	// If it's zero, there's one glancing intersection right on the edge of the sphere.
	// If it's positive, there are two intersections, one where the ray enters the sphere, and one where it exits.

	float a = cVec3::Dot(RayDir, RayDir);
	cVec3 u = RayOrig - m_Center;
	float b = cVec3::Dot(RayDir, u);
	float c = cVec3::Dot(u, u) - m_Radius * m_Radius;
	float D = b * b - a * c;
	if(D < 0.0f) {
		return false;
	}
	float SqrtD = cMath::Sqrt(D);
	float ia = 1.0f / a;
	Scale1 = (-b + SqrtD) * ia;
	Scale2 = (-b - SqrtD) * ia;
	
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
// cSphere::RayIntersection
//---------------------------------------------------------------------------------------------------------------------------------------------------------
bool cSphere::RayIntersection(const cSphere *Array, const int nCount, const size_t Stride, const cVec3 &RayOrig, const cVec3 &RayDir, int &Index, cVec3 &Point, cVec3 &Normal) {
	const size_t S = cMath::Max(sizeof(cSphere), Stride);
	const char *Ptr = reinterpret_cast<const char *>(Array);

	bool Hit = false;
	float dMin = cMath::Infinity;
	for(int i = 0; i < nCount; i++) {
		const cSphere *pSphere = reinterpret_cast<const cSphere *>(Ptr + S * i);
		float d = cVec3::Dot(RayDir, pSphere->m_Center - RayOrig);
		if(d > 0.0f && d < dMin) {
			cVec3 p = RayOrig + RayDir * d;
			cVec3 v = p - pSphere->m_Center;
			float d2 = pSphere->m_Radius * pSphere->m_Radius - cVec3::Dot(v, v);
			if(d2 > 0.0f) {
				Index = i;
				Point = p - RayDir * cMath::Sqrt(d2);
				Normal = Point - pSphere->m_Center;
				//dMin = d;
				dMin = cVec3::Distance(Point, RayOrig);
				Hit = true;
			}
		}
	}
	if(Hit) {
		Normal.Normalize();
	}
	return Hit;
}

//-----------------------------------------------------------------------------
// cSphere::LineIntersection : bool(const cVec3 &, const cVec3 &) const
//-----------------------------------------------------------------------------
bool cSphere::LineIntersection(const cVec3 &A, const cVec3 &B) const {
	const cSeg S(cSeg::EndsCtor, A, B);
	const float d = cSeg::Distance(cSeg::Seg, S, m_Center);
	if(d < m_Radius) {
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------
// cSphere::TriIntersection : bool(const cVec3 &, const cVec3 &, const cVec3 &, cVec3 *) const
//------------------------------------------------------------------------------------------------------------------
bool cSphere::TriIntersection(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, cVec3 *pCross, cVec3 *pNormal) const {
/*	cPlane P(t0, t1, t2);
	if(P.ClassifySphere(*this) == cPlane::S_CROSS) {
		cVec3 pr = P.ProjectPoint(m_Center);

		cVec3 Tri[] = {t0, t1, t2};
		if(cVec3::IsInsideConvexPolygon(pr, Tri, 3)) {
			if(pCross) {
				*pCross = pr;
			}
			if(pNormal) {
				*pNormal = P.GetNormal();
			}
			return true;
		}
*/
		// Does half radius sphere collide with tri edges ?
//		cSphere s(m_Center, m_Radius / 2.0f);
//		bool fSphereEdgeCollision = false;
//		for(int i = 0; i < 3; i++) {
//			if(s.LineIntersection(Tri[i], Tri[(i + 1) % 3])) {
//				fSphereEdgeCollision = true;
//				break;
//			}
//		}
//		cVec3 pr = cVec3::ProjectToPlane(m_Center, P);
/*		if(cVec3::IsInsideConvexPolygon(pr, Tri, 3) || fSphereEdgeCollision) {
			if(pOffset) {
				float dSphereToPlane = P.SignedDistanceTo(m_Center);
				float dOverlap = m_Radius - cMath::Abs(dSphereToPlane);
				*pOffset = comms::Sign(dSphereToPlane) * P.Normal() * dOverlap;
			}
			return true;
		}*/
	//}
	return false;
}

//--------------------------------------------------------------------------------------------------------------
// cSphere::FromPoints
//--------------------------------------------------------------------------------------------------------------
cSphere cSphere::FromPoints(const cVec3 &p0, const cVec3 &p1, const cVec3 &p2, const cVec3 &p3) {
	cSphere S(0.25f * (p0 + p1 + p2 + p3), 0.0f);
	S.ExtendToPoint(p0);
	S.ExtendToPoint(p1);
	S.ExtendToPoint(p2);
	S.ExtendToPoint(p3);
	return S;
}

//-----------------------------------------------------------------------------
// cSphere::FromPoints : cSphere(const cVec2 *, const float)
//-----------------------------------------------------------------------------
cSphere cSphere::FromPoints(const cVec2 *pPoints, const int nCount) {
	cAssert(pPoints);
	cAssert(nCount > 0);

	cVec2 bbMin(pPoints[0]), bbMax(pPoints[0]);
	for(int i = 1; i < nCount; i++) {
		bbMin = cVec2::Min(bbMin, pPoints[i]);
		bbMax = cVec2::Max(bbMax, pPoints[i]);
	}
	cVec2 c((bbMin + bbMax) * 0.5f);

	float MaxRadius2 = 0.0f;
	for(int i = 0; i < nCount; i++) {
		MaxRadius2 = cMath::Max(MaxRadius2, cVec2::DistanceSq(c, pPoints[i]));
	}

	return cSphere(cVec3(c, 0.0f), cMath::Sqrt(MaxRadius2));
}

//-----------------------------------------------------------------------------
// cSphere::FromPoints : cSphere(const cVec3 *, const float)
//-----------------------------------------------------------------------------
cSphere cSphere::FromPoints(const cVec3 *pPoints, const int nCount) {
	cAssert(pPoints);
	cAssert(nCount > 0);

	cVec3 bbMin(pPoints[0]), bbMax(pPoints[0]);
	for(int i = 1; i < nCount; i++) {
		bbMin = cVec3::Min(bbMin, pPoints[i]);
		bbMax = cVec3::Max(bbMax, pPoints[i]);
	}
	cVec3 c((bbMin + bbMax) * 0.5f);

	float MaxRadius2 = 0.0f;
	for(int i = 0; i < nCount; i++) {
		MaxRadius2 = cMath::Max(MaxRadius2, cVec3::DistanceSq(c, pPoints[i]));
	}

	return cSphere(c, cMath::Sqrt(MaxRadius2));
}