#include "comms.h"

//-----------------------------------------------------------------------------
// cSeg::IsZero
//-----------------------------------------------------------------------------
bool cSeg::IsZero() const {
	return m_Length < cMath::Epsilon;
}

//-----------------------------------------------------------------------------
// cSeg::IsValid
//-----------------------------------------------------------------------------
bool cSeg::IsValid() const {
	cAssert(m_Fm.IsValid());
	cAssert(m_To.IsValid());
	cAssert(m_Normal.IsNormalized());
	cAssert(cMath::IsValid(m_Radius));
	cAssert(m_Radius >= 0.0f);
	cAssert(!IsZero());
	return true;
} // cSeg::IsValid

//-----------------------------------------------------------------------------
// cSeg::SetFromEnds
//-----------------------------------------------------------------------------
void cSeg::SetFromEnds(const cVec3 &Fm, const cVec3 &To, const float Radius/* = cMath::Epsilon*/) {
	cAssert(Radius >= cMath::Epsilon);
	
	m_Radius = Radius;
	m_Fm = Fm;
	m_To = To;
	m_Normal = To - Fm;
	m_Length = m_Normal.Length();
	if(IsZero()) {
		m_InvLength = 1.0f;
		return;
	}
	m_InvLength = 1.0f / m_Length;
	m_Normal *= m_InvLength;
	cAssert(IsValid());
} // cSeg::SetFromEnds

//-----------------------------------------------------------------------------
// cSeg::SetFromRay
//-----------------------------------------------------------------------------
void cSeg::SetFromRay(const cVec3 &RayOrig, const cVec3 &RayDir, const float Radius/* = cMath::Epsilon*/) {
	cAssert(Radius >= cMath::Epsilon);

	m_Radius = Radius;
	m_Fm = RayOrig;
	m_To = RayOrig + RayDir;
	m_Normal = RayDir;
	m_Length = m_Normal.Length();
	if(IsZero()) {
		m_InvLength = 1.0f;
		return;
	}
	m_InvLength = 1.0f / m_Length;
	m_Normal *= m_InvLength;
	cAssert(IsValid());
} // cSeg::SetFromRay

//--------------------------------------------------------------------------------------
// cSeg::ProjectPoint
//--------------------------------------------------------------------------------------
const cSeg::Result cSeg::ProjectPoint(const cSeg::ConsiderAs To, const cVec3 &p) const {
	Result r;
	
	if(IsZero()) { // Seg is degraded to point.
		r.Time = 0.0f;
		r.CollidedPoint = GetFm();
		return r;
	}

	cAssert(IsValid());
	cAssert(p.IsValid());
	
	cAssert(To == cSeg::Line || To == cSeg::Seg || To == cSeg::Ray);
	
	const cVec3 Ap = p - m_Fm;
	const float pr = cVec3::Dot(Ap, GetNormal());
	
	r.Time = pr * GetInvLength();
	
	if((To & CLAMP_S0) && r.Time <= 0.0f) {
		r.CollidedPoint = m_Fm;
	} else if((To & CLAMP_S1) && r.Time >= 1.0f) {
		r.CollidedPoint = m_To;
	} else {
		r.CollidedPoint = m_Fm + pr * GetNormal();
	}
	return r;
} // cSeg::ProjectPoint

//------------------------------------------------------------------------------
// cSeg::Distance
//------------------------------------------------------------------------------
float cSeg::Distance(const cSeg::ConsiderAs Fm, const cSeg &P, const cVec3 &p) {
	return cVec3::Distance(p, P.ProjectPoint(Fm, p).CollidedPoint);
} // cSeg::Distance

//----------------------------------------------------------------------------------------------------------
// cSeg::ClosestPoints
//----------------------------------------------------------------------------------------------------------
void cSeg::ClosestPoints(const cSeg::ConsiderAs Between, const cSeg &P, const cSeg &Q, cVec3 &p, cVec3 &q) {
	// Consider two infinite lines:
	// P(s) = P0 + s(P1 - P0) = P0 + su
	// Q(t) = Q0 + t(Q1 - Q0) = Q0 + tv

	// Let w(s, t) = P(s) - Q(t) be a vector between points on the two lines.
	// It has minimum length, where it is perpendicular to both lines at the same time.
	// This is equivalent to satisfying the two equations:
	// u * w = 0
	// v * w = 0

	//              =>
	// u * (P(s) - Q(t)) = 0
	// v * (P(s) - Q(t)) = 0
	//             <=>
	// u * (P0 + su - Q0 - tv) = 0
	// v * (P0 + su - Q0 - tv) = 0
	//              =>
	// (u * u)s - (u * v)t = - u * w0
	// (v * u)s - (v * v)t = - v * w0, where w0 = P0 - Q0

	// Letting a = u * u, b = u * v, c = v * v, d = u * w0, e = v * w0, we solve for s and t as:

	// as - bt = - d
	// bs - ct = - e
	//       <=>
	// t = (d + as) / b   s = (bt - d) / a
	// t = (e + bs) / c   s = (ct - e) / b
	//        =>
	// (d + as) / b = (e + bs) / c
	// (bt - d) / a = (ct - e) / b
	//       <=>
	// cd + acs = be + b^2s
	// b^2t - bd = act - ae
	//       <=>
	//     /-----------------------------/
	//    / s = (be - cd) / (ac - b^2)  /
	//   /  t = (ae - bd) / (ac - b^2) /
	//  /-----------------------------/

	// When denominator ac - b^2 is zero, the two lines are parallel.
	// We can solve this by fixing the value of one parameter and using either equation to solve for the other.
	// Selecting s = 0, we get t = d / b = e / c.

	cVec3 u = P.GetTo() - P.GetFm();
	cVec3 v = Q.GetTo() - Q.GetFm();
	float a = cVec3::Dot(u, u);
	float b = cVec3::Dot(u, v);
	float c = cVec3::Dot(v, v);
	cVec3 w0 = P.GetFm() - Q.GetFm();
	float d = cVec3::Dot(u, w0);
	float e = cVec3::Dot(v, w0);
	float D = a * c - b * b;
	float s, sN, sD = D; // s = sN / sD, default sD = D >= 0
	float t, tN, tD = D; // t = tN / tD, default tD = D >= 0

	if(0.0f == D) { // The lines are almost parallel.
		sN = 0.0f; // Force using point Fm on this segment
		sD = 1.0f; // to prevent possible division by 0.0f later.
		tN = e;
		tD = c;
	} else { // Get the closest points:
		sN = b * e - c * d;
		tN = a * e - b * d;
		if((Between & CLAMP_S0) && sN < 0.0f) { // s < 0 => s = 0 edge is visible
			sN = 0.0f;
			tN = e;
			tD = c;
		} else if((Between & CLAMP_S1) && sN > sD) { // s > 1 => s = 1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if((Between & CLAMP_T0) && tN < 0.0f) { // t < 0 => t = 0 edge is visible
		tN = 0.0f;
		// Recompute s for this edge:
		if((Between & CLAMP_S0) && -d < 0.0f) {
			sN = 0.0f;
		} else if((Between & CLAMP_S1) && -d > a) {
			sN = sD;
		} else {
			sN = -d;
			sD = a;
		}
	} else if((Between & CLAMP_T1) && tN > tD) { // t > 1 => t = 1 edge is visible
		tN = tD;
		// Recompute s for this edge:
		if((Between & CLAMP_S0) && b - d < 0.0f) {
			sN = 0.0f;
		} else if((Between & CLAMP_S1) && b - d > a) {
			sN = sD;
		} else {
			sN = -d + b;
			sD = a;
		}
	}
	s = cMath::IsZero(sN) ? 0.0f : sN / sD;
	t = cMath::IsZero(tN) ? 0.0f : tN / tD;

	p = P.GetFm() + s * u;
	q = Q.GetFm() + t * v;
} // cSeg::ClosesetPoints

//----------------------------------------------------------------------------------
// cSeg::Distance
//----------------------------------------------------------------------------------
float cSeg::Distance(const cSeg::ConsiderAs Between, const cSeg &P, const cSeg &Q) {
	cVec3 p, q;
	ClosestPoints(Between, P, Q, p, q);
	return cVec3::Distance(p, q);
} // cSeg::Distance