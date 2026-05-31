#pragma once

class cSeg {
public:
	struct Result {
		float Time;
		cVec3 Normal;
		cVec3 CollidedPoint;
	};

	cSeg() {}

	enum eEndsCtor	{ EndsCtor };
	enum eRayCtor	{ RayCtor };
	
	cSeg(const eEndsCtor, const cVec3 &Fm, const cVec3 &To, const float Radius = cMath::Epsilon) { SetFromEnds(Fm, To, Radius); }
	cSeg(const eRayCtor, const cVec3 &RayOrig, const cVec3 &RayDir, const float Radius = cMath::Epsilon) { SetFromRay(RayOrig, RayDir, Radius); }
	
	void SetFromEnds(const cVec3 &Fm, const cVec3 &To, const float Radius = cMath::Epsilon);
	void SetFromRay(const cVec3 &RayOrig, const cVec3 &RayDir, const float Radius = cMath::Epsilon);
	
	bool IsZero() const;
	bool IsValid() const;

	const cVec3 & GetFm() const		{ cAssert(IsValid()); return m_Fm; }
	const cVec3 & GetTo() const		{ cAssert(IsValid()); return m_To; }
	float GetRadius() const			{ cAssert(IsValid()); return m_Radius; }
	const cVec3 & GetNormal() const	{ cAssert(IsValid()); return m_Normal; }
	float GetLength() const			{ cAssert(IsValid()); return m_Length; }
	float GetInvLength() const		{ cAssert(IsValid()); return m_InvLength; }

	const cVec3 GetHitPoint(const float Time) const;
	const cVec3 GetCenter() const;

	// w/o using radius:
	
	enum ConsiderAs { CLAMP_S0 = 0x10, CLAMP_S1 = 0x20, CLAMP_T0 = 0x40, CLAMP_T1 = 0x80,
		Line = 0,
		Ray = CLAMP_S0,
		Seg = CLAMP_S0 | CLAMP_S1,
		LineLine = 1,
		SegSeg = CLAMP_S0 | CLAMP_S1 | CLAMP_T0 | CLAMP_T1,
		RayRay = CLAMP_S0 | CLAMP_T0,
		SegRay = CLAMP_S0 | CLAMP_S1 | CLAMP_T0,
		LineRay = CLAMP_T0
	};
	
	const cSeg::Result ProjectPoint(const ConsiderAs To /* Line, Ray, Seg */, const cVec3 &p) const; // i.e. closest point
	// ---------------------------------
	//    Time is in  |  p is projected
	// ---------------------------------
	//	(-INF, 0.0f)  |	   before Fm
	//	[0.0f, 1.0f]  |	   inside Fm - To
	//	(1.0f, +INF)  |	   beyond To
	// ---------------------------------
	static float Distance(const ConsiderAs Fm /* Line, Ray, Seg */, const cSeg &P, const cVec3 &p);
	
	static void ClosestPoints(const ConsiderAs Between /* LineLine, SegSeg, RayRay, SegRay */, const cSeg &P, const cSeg &Q, cVec3 &p, cVec3 &q);
	static float Distance(const ConsiderAs Between /* LineLine, SegSeg, RayRay, SegRay */, const cSeg &P, const cSeg &Q);
	
	// w/ using radius:

private:
	cVec3 m_Fm;
	cVec3 m_To;
	float m_Radius;
	cVec3 m_Normal;
	float m_Length, m_InvLength;

};

//-----------------------------------------------------------------------------
// cSeg::GetHitPoint
//-----------------------------------------------------------------------------
inline const cVec3 cSeg::GetHitPoint(const float Time) const {
	cAssert(IsValid());
	return cVec3::Lerp(GetFm(), GetTo(), Time);
} // cSeg::GetHitPoint

//-----------------------------------------------------------------------------
// cSeg::GetCenter
//-----------------------------------------------------------------------------
inline const cVec3 cSeg::GetCenter() const {
	cAssert(IsValid());
	return cVec3::Lerp05(GetFm(), GetTo());
} // cSeg::GetCenter