#pragma once

class cSphere {
private:
	cVec3 m_Center;
	float m_Radius;
public:
	cSphere();
	cSphere(const cVec3 &Center, const float Radius);

	const cVec3 & GetCenter() const;
	void SetCenter(const cVec3 &Center);
	float GetRadius() const;
	void SetRadius(const float Radius);
	void Set(const cVec3 &Center, const float Radius);

	void Clear();
	bool IsCleared() const;

	void SetToPoint(const cVec3 &p);
	bool ExtendToPoint(const cVec3 &p);

	void Translate(const cVec2 &);
	void Translate(const cVec3 &);

	float DistanceToPoint(const cVec2 &p) const;
	float DistanceToPoint(const cVec3 &p) const;
	float SignedDistanceToPlane(const cPlane &P) const;
	
	// Intersection points are: RayOrig + Scale1 * RayDir and RayOrig + Scale2 * RayDir.
	// RayOrig is inside sphere if Scale1 < 0 and Scale2 > 0.
	bool RayIntersection(const cVec3 &RayOrig, const cVec3 &RayDir, float &Scale1, float &Scale2) const;
	static bool RayIntersection(const cSphere *Array, const int nCount, const size_t Stride, const cVec3 &RayOrig, const cVec3 &RayDir, int &Index, cVec3 &Point, cVec3 &Normal);
	bool LineIntersection(const cVec3 &A, const cVec3 &B) const;
	bool TriIntersection(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, cVec3 *pCross = NULL, cVec3 *pNormal = NULL) const;

	static cSphere FromPoints(const cVec3 &p0, const cVec3 &p1, const cVec3 &p2, const cVec3 &p3);
	static cSphere FromPoints(const cVec2 *pPoints, const int nCount);
	static cSphere FromPoints(const cVec3 *pPoints, const int nCount);
};

// cSphere.ctor : ()
inline cSphere::cSphere() {
}

// cSphere.ctor : (const cVec3 &, const float)
inline cSphere::cSphere(const cVec3 &Center, const float Radius)
: m_Center(Center), m_Radius(Radius) {}

// cSphere::GetCenter : const cVec3 & () const
inline const cVec3 & cSphere::GetCenter() const {
	return m_Center;
}

// cSphere::SetCenter : void(const cVec3 &)
inline void cSphere::SetCenter(const cVec3 &Center) {
	m_Center = Center;
}

// cSphere::GetRadius : float() const
inline float cSphere::GetRadius() const {
	return m_Radius;
}

// cSphere::SetRadius : void(const float)
inline void cSphere::SetRadius(const float Radius) {
	m_Radius = Radius;
}

// cSphere::Set : void(const cVec3 &, const float)
inline void cSphere::Set(const cVec3 &Center, const float Radius) {
	SetCenter(Center);
	SetRadius(Radius);
}

// cSphere::SetToPoint : void(const cVec3 &)
inline void cSphere::SetToPoint(const cVec3 &p) {
	SetCenter(p);
	SetRadius(0.0f);
}

// cSphere::ExtendToPoint : bool(const cVec3 &)
inline bool cSphere::ExtendToPoint(const cVec3 &p) {
	const float d2 = cVec3::DistanceSq(m_Center, p);
	if(d2 <= cMath::Square(m_Radius)) {
		return false;
	}
	m_Radius = cMath::Sqrt(d2);
	return true;
}

// cSphere::Translate : void(const cVec2 &)
inline void cSphere::Translate(const cVec2 &t) {
	m_Center += cVec3(t, 0.0f);
}

// cSphere::Translate : void(const cVec3 &)
inline void cSphere::Translate(const cVec3 &t) {
	m_Center += t;
}

// cSphere::DistanceToPoint : float(const cVec2 &) const
inline float cSphere::DistanceToPoint(const cVec2 &p) const {
	return cMath::Max(cVec2::Distance(m_Center.ToVec2(), p) - m_Radius, 0.0f);
}

// cSphere::DistanceToPoint : float(const cVec3 &) const
inline float cSphere::DistanceToPoint(const cVec3 &p) const {
	return cMath::Max(cVec3::Distance(m_Center, p) - m_Radius, 0.0f);
}

// cSphere::Clear : void()
inline void cSphere::Clear() {
	m_Center.SetZero();
	m_Radius = -1.0f;
}

// cSphere::IsCleared : bool() const
inline bool cSphere::IsCleared() const {
	return m_Radius < 0.0f;
}