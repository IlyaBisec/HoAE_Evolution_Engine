#pragma once

class cRotation {
	friend class cVec3;
	friend class cAngles;
	friend class cQuat;
	friend class cMat3;

private:
	cVec3 m_Orig;
	cVec3 m_Axis;
	float m_Angle;
	
	mutable cMat3 m_CachedMatrix;
	mutable bool m_CachedMatrixIsValid;

public:
	cRotation();
	cRotation(const cVec3 &Orig, const cVec3 &Axis, const float Angle);

	cRotation & Set(const cVec3 &Orig, const cVec3 &Axis, const float Angle);

	const cVec3 & GetOrig() const;
	const cVec3 & GetAxis() const;
	float GetAngle() const;

	void SetOrig(const cVec3 &Orig);
	void SetAxis(const cVec3 &Axis);
	void SetAngle(const float Angle);

	void ReCalcMatrix() const;

	cRotation operator - () const;

	cRotation & operator *= (const float s);
	cRotation & operator /= (const float s);
	
	cRotation operator * (const float s) const;
	friend cRotation operator * (const float s, const cRotation &R);
	cRotation operator / (const float s) const;
	
	cRotation & Normalize180();
	cRotation & Normalize360();

	cAngles ToAngles() const;
	cQuat ToQuat() const;
	const cMat3 & ToMat3() const;
	cMat4 ToMat4() const;
	cVec3 ToAngularVelocity() const;
};

// cRotation.ctor
inline cRotation::cRotation() {
}

// cRotation.ctor : (const cVec3 &, const cVec3 &, const float)
inline cRotation::cRotation(const cVec3 &Orig, const cVec3 &Axis, const float Angle) {
	m_Orig = Orig;
	m_Axis = Axis;
	m_Angle = Angle;
	m_CachedMatrixIsValid = false;
}

// cRotation::Set : cRotation & (const cVec3 &, const cVec3 &, const float)
inline cRotation & cRotation::Set(const cVec3 &Orig, const cVec3 &Axis, const float Angle) {
	m_Orig = Orig;
	m_Axis = Axis;
	m_Angle = Angle;
	m_CachedMatrixIsValid = false;
	return *this;
}

// cRotation::GetOrig : const cVec3 & () const
inline const cVec3 & cRotation::GetOrig() const {
	return m_Orig;
}

// cRotation::GetAxis : const cVec3 & () const
inline const cVec3 & cRotation::GetAxis() const {
	return m_Axis;
}

// cRotation::GetAngle : float() const
inline float cRotation::GetAngle() const {
	return m_Angle;
}

// cRotation::SetOrig : void(const cVec3 &)
inline void cRotation::SetOrig(const cVec3 &Orig) {
	m_Orig = Orig;
}

// cRotation::SetAxis : void(const cVec3 &)
inline void cRotation::SetAxis(const cVec3 &Axis) {
	m_Axis = Axis;
	m_CachedMatrixIsValid = false;
}

// cRotation::SetAngle : void(const float)
inline void cRotation::SetAngle(const float Angle) {
	m_Angle = Angle;
	m_CachedMatrixIsValid = false;
}

// cRotation::ReCalcMatrix : void()
inline void cRotation::ReCalcMatrix() const {
	m_CachedMatrixIsValid = false;
	ToMat3();
}

// cRotation::operator - : cRotation() const
inline cRotation cRotation::operator - () const {
	return cRotation(m_Orig, m_Axis, - m_Angle);
}

// cRotation::operator *= : cRotation & (const float)
inline cRotation & cRotation::operator *= (const float s) {
	m_Angle *= s;
	m_CachedMatrixIsValid = false;
	return *this;
}

// cRotation::operator /= : cRotation & (const float)
inline cRotation & cRotation::operator /= (const float s) {
	assert(s != 0.0f);
	m_Angle /= s;
	m_CachedMatrixIsValid = false;
	return *this;
}

// cRotation::operator * : cRotation(const float) const
inline cRotation cRotation::operator * (const float s) const {
	return cRotation(m_Orig, m_Axis, m_Angle * s);
}

// cRotation::friend operator * : cRotation(const float, const cRotation &)
inline cRotation operator * (const float s, const cRotation &R) {
	return R * s;
}

// cRotation::operator / : cRotation(const float) const
inline cRotation cRotation::operator / (const float s) const {
	assert(s != 0.0f);
	return cRotation(m_Orig, m_Axis, m_Angle / s);
}

// cVec3::Rotate : cVec3 & (const cRotation &)
inline cVec3 & cVec3::Rotate(const cRotation &R) {
	if(!R.m_CachedMatrixIsValid) {
		R.ToMat3();
	}
	*this -= R.m_Orig;
	Transform(R.m_CachedMatrix);
	*this += R.m_Orig;
	return *this;
}

// cVec3::operator *= : cVec3 & (const cRotation &)
inline cVec3 & cVec3::operator *= (const cRotation &R) {
	return Rotate(R);
}

// cVec3::Rotate : const cVec3(const cVec3 &, const cRotation &)
inline const cVec3 cVec3::Rotate(const cVec3 &u, const cRotation &R) {
	if(!R.m_CachedMatrixIsValid) {
		R.ToMat3();
	}
	return cVec3::Transform(u - R.m_Orig, R.m_CachedMatrix) + R.m_Orig;
}

// cVec3::operator * : const cVec3(const cRotation &) const
inline const cVec3 cVec3::operator * (const cRotation &R) const {
	return Rotate(*this, R);
}