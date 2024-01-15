#pragma once

class cBounds {
public:
	cBounds();
	explicit cBounds(const cVec3 &p);
	cBounds(const cVec3 &bMin, const cVec3 &bMax);

	const cVec3 & GetMin() const;
	const cVec3 & GetMax() const;
	
	void SetMin(const cVec3 &);
	void SetMax(const cVec3 &);

	cVec3 & MutableMin();
	cVec3 & MutableMax();

	float GetSizeX() const;
	float GetSizeY() const;
	float GetSizeZ() const;

	cVec3 GetCenter() const;

	void Clear();
	bool IsCleared() const;

	bool AddPoint(const cVec3 &);
	bool AddBounds(const cBounds &);

	void Translate(const cVec2 &);
	void Translate(const cVec3 &);
	
	float DistanceToPointSq(const cVec2 &p) const; // 0.0f for inside
	float DistanceToPointSq(const cVec3 &p) const; // 0.0f for inside

	bool ContainsPoint(const cVec2 &) const;
	bool ContainsPoint(const cVec3 &) const;
	bool ContainsBounds(const cBounds &) const;
	bool IntersectsBounds(const cBounds &) const;
//	bool LineIntersection(const cVec3 &A, const cVec3 &B) const;
//	bool RayIntersection(const cVec3 &RayOrig, const cVec3 &RayDir, float &Scale) const;

	static cBounds FromPoints(const cVec2 *pPoints, const int nCount);
	static cBounds FromPoints(const cVec3 *pPoints, const int nCount);
private:
	cVec3 m_Min;
	cVec3 m_Max;
};

// cBounds.ctor
inline cBounds::cBounds() {
}

// cBounds.cotr : (const cVec3 &)
inline cBounds::cBounds(const cVec3 &p)
: m_Min(p), m_Max(p) {}

// cBounds.ctor : (const cVec3 &, const cVec3 &)
inline cBounds::cBounds(const cVec3 &bMin, const cVec3 &bMax)
: m_Min(bMin), m_Max(bMax) {}

// cBounds::GetMin : const cVec3 & () const
inline const cVec3 & cBounds::GetMin() const {
	return m_Min;
}

// cBounds::GetMax : const cVec3 & () const
inline const cVec3 & cBounds::GetMax() const {
	return m_Max;
}

// cBounds::SetMin : void(const cVec3 &)
inline void cBounds::SetMin(const cVec3 &bMin) {
	m_Min = bMin;
}

// cBounds::SetMax : void(const cVec3 &)
inline void cBounds::SetMax(const cVec3 &bMax) {
	m_Max = bMax;
}

// cBounds::MutableMin : cVec3 & ()
inline cVec3 & cBounds::MutableMin() {
	return m_Min;
}

// cBounds::MutableMax : cVec3 & ()
inline cVec3 & cBounds::MutableMax() {
	return m_Max;
}

// cBounds::Clear : void()
inline void cBounds::Clear() {
	m_Min.Set(cMath::Infinity);
	m_Max.Set(- cMath::Infinity);
}

// cBounds::IsCleared : bool() const
inline bool cBounds::IsCleared() const {
	return m_Min.x > m_Max.x;
}

// cBounds::GetSizeX : float() const
inline float cBounds::GetSizeX() const {
	return m_Max.x - m_Min.x;
}

// cBounds::GetSizeY : float() const
inline float cBounds::GetSizeY() const {
	return m_Max.y - m_Min.y;
}

// cBounds::GetSizeZ : float() const
inline float cBounds::GetSizeZ() const {
	return m_Max.z - m_Min.z;
}

// cBounds::GetCenter : cVec3() const
inline cVec3 cBounds::GetCenter() const {
	return 0.5f * (m_Max + m_Min);
}

// cBounds::AddPoint : bool(const cVec3 &)
inline bool cBounds::AddPoint(const cVec3 &p) {
	bool IsExpanded = false;
	if(p.x < m_Min.x) {
		m_Min.x = p.x;
		IsExpanded = true;
	}
	if(p.y < m_Min.y) {
		m_Min.y = p.y;
		IsExpanded = true;
	}
	if(p.z < m_Min.z) {
		m_Min.z = p.z;
		IsExpanded = true;
	}
	if(p.x > m_Max.x) {
		m_Max.x = p.x;
		IsExpanded = true;
	}
	if(p.y > m_Max.y) {
		m_Max.y = p.y;
		IsExpanded = true;
	}
	if(p.z > m_Max.z) {
		m_Max.z = p.z;
		IsExpanded = true;
	}
	return IsExpanded;
}

// cBounds::AddBounds : bool(const cBounds &)
inline bool cBounds::AddBounds(const cBounds &B) {
	bool IsExpanded = false;
	if(B.m_Min.x < m_Min.x) {
		m_Min.x = B.m_Min.x;
		IsExpanded = true;
	}
	if(B.m_Min.y < m_Min.y) {
		m_Min.y = B.m_Min.y;
		IsExpanded = true;
	}
	if(B.m_Min.z < m_Min.z) {
		m_Min.z = B.m_Min.z;
		IsExpanded = true;
	}
	if(B.m_Max.x > m_Max.x) {
		m_Max.x = B.m_Max.x;
		IsExpanded = true;
	}
	if(B.m_Max.y > m_Max.y) {
		m_Max.y = B.m_Max.y;
		IsExpanded = true;
	}
	if(B.m_Max.z > m_Max.z) {
		m_Max.z = B.m_Max.z;
		IsExpanded = true;
	}
	return IsExpanded;
}

// cBounds::Translate : void(const cVec2 &)
inline void cBounds::Translate(const cVec2 &t) {
	m_Min += cVec3(t, 0.0f);
	m_Max += cVec3(t, 0.0f);
}

// cBounds::Translate : void(const cVec3 &)
inline void cBounds::Translate(const cVec3 &t) {
	m_Min += t;
	m_Max += t;
}

// cBounds::ContainsPoint : bool(const cVec2 &) const
inline bool cBounds::ContainsPoint(const cVec2 &p) const {
	return p.x >= m_Min.x && p.x <= m_Max.x &&
		p.y >= m_Min.y && p.y <= m_Max.y;
}

// cBounds::ContainsPoint : bool(const cVec3 &) const
inline bool cBounds::ContainsPoint(const cVec3 &p) const {
	return p.x >= m_Min.x && p.x <= m_Max.x &&
		p.y >= m_Min.y && p.y <= m_Max.y &&
		p.z >= m_Min.z && p.z <= m_Max.z;
}

// cBounds::ContainsBounds : bool(const cBounds &) const
inline bool cBounds::ContainsBounds(const cBounds &B) const {
	return B.m_Min.x >= m_Min.x && B.m_Max.x <= m_Max.x &&
		B.m_Min.y >= m_Min.y && B.m_Max.y <= m_Max.y &&
		B.m_Min.z >= m_Min.z && B.m_Max.z <= m_Max.z;
}

// cBounds::IntersectsBounds : bool(const cBounds &) const
inline bool cBounds::IntersectsBounds(const cBounds &B) const {
	return B.m_Min.x <= m_Max.x && B.m_Max.x >= m_Min.x &&
		B.m_Min.y <= m_Max.y && B.m_Max.y >= m_Min.y &&
		B.m_Min.z <= m_Max.z && B.m_Max.z >= m_Min.z;
}