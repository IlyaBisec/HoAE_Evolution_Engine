#pragma once

class cQuat {
public:
	float x;
	float y;
	float z;
	float w;
	
	cQuat();
	cQuat(const float X, const float Y, const float Z, const float W);
	
	void SetIdentity();
	void SetZero();
	void Set(const float X, const float Y, const float Z, const float W);
	void Set(const cVec3 &XYZ, const float W);

	float & operator [] (const int index);
	float operator [] (const int index) const;

	const cQuat operator - () const;

	static const cQuat Mul(const cQuat &, const cQuat &);
	static const cQuat Div(const cQuat &, const cQuat &);

	void operator += (const cQuat &);
	void operator -= (const cQuat &);
	void operator *= (const cQuat &);
	void operator /= (const cQuat &);
	void operator *= (const float);
	void operator /= (const float);

	const cQuat operator + (const cQuat &) const;
	const cQuat operator - (const cQuat &) const;
	const cQuat operator * (const cQuat &) const;
	const cQuat operator / (const cQuat &) const;
	const cQuat operator * (const float) const;
	friend const cQuat operator * (const float, const cQuat &);
	const cQuat operator / (const float) const;
	
	bool operator == (const cQuat &) const;
	bool operator != (const cQuat &) const;
	static bool Equals(const cQuat &, const cQuat &, const float Eps = cMath::Epsilon);
	static bool EqualRotations(const cQuat &, const cQuat &, const float Eps = cMath::Epsilon);
	static bool SameHemisphere(const cQuat &, const cQuat &);
	
	float CalcW() const;
	float Length() const;
	float LengthSq() const;
	cQuat & Normalize();
	bool IsNormalized(const float Eps = cMath::Epsilon) const;
	
	static const cQuat Conjugate(const cQuat &);
	static float Dot(const cQuat &, const cQuat &);
	static const cQuat Exp(const cQuat &);
	static const cQuat Invert(const cQuat &);
	static const cQuat Lerp(const cQuat &q0, const cQuat &q1, const float s, const bool ShortestPath = true);
	static const cQuat Ln(const cQuat &);
	static const cQuat LnDif(const cQuat &, const cQuat &);
	static const cQuat Normalize(const cQuat &);
	static const cQuat Slerp(const cQuat &q0, const cQuat &q1, const float s, const bool ShortestPath = true);
	static const cQuat Squad(const cQuat &q1, const cQuat &A, const cQuat &B, const cQuat &C, const float s);
	static void SquadSetup(const cQuat &q0, const cQuat &q1, const cQuat &q2, const cQuat &q3, cQuat &A, cQuat &B, cQuat &C);
	
	static const cQuat Identity;
	static const cQuat Zero;
	
	int GetDimension() const;

	const cAngles ToAngles() const;
	const cRotation ToRotation() const;
	const cMat3 ToMat3() const;
	const cMat4 ToMat4() const;
	const cVec3 ToAngularVelocity() const;
	
	const float * ToFloatPtr() const;
	float * ToFloatPtr();
	const cStr ToStr(const int nPrec = 2) const;
};

// cQuat.ctor : ()
inline cQuat::cQuat() {
}

// cQuat.ctor : (const float, const float, const float, const float)
inline cQuat::cQuat(const float X, const float Y, const float Z, const float W)
: x(X), y(Y), z(Z), w(W) {}

// cQuat::SetIdentity : void ()
inline void cQuat::SetIdentity() {
	x = y = z = 0.0f;
	w = 1.0f;
}

// cQuat::SetZero : void ()
inline void cQuat::SetZero() {
	x = y = z = w = 0.0f;
}

// cQuat::Set : void (const float, const float, const float, const float)
inline void cQuat::Set(const float X, const float Y, const float Z, const float W) {
	x = X;
	y = Y;
	z = Z;
	w = W;
}

// cQuat::Set : void (const cVec3 &, const float)
inline void cQuat::Set(const cVec3 &XYZ, const float W) {
	x = XYZ.x;
	y = XYZ.y;
	z = XYZ.z;
	w = W;
}

// cQuat::operator [] : float & (const int)
inline float & cQuat::operator [] (const int index) {
	cAssert(index >= 0 && index < 4);
	return (&x)[index];
}

// cQuat::operator [] : float (const int) const
inline float cQuat::operator [] (const int index) const {
	cAssert(index >= 0 && index < 4);
	return (&x)[index];
}

// cQuat::operator - : const cQuat () const
inline const cQuat cQuat::operator - () const {
	return cQuat(-x, -y, -z, -w);
}

// cQuat::Mul : const cQuat (const cQuat &, const cQuat &)
inline const cQuat cQuat::Mul(const cQuat &q0, const cQuat &q1) {
	cQuat r;
	r.x = q0.x * q1.w + q0.w * q1.x + q0.z * q1.y - q0.y * q1.z;
	r.y = q0.y * q1.w + q0.w * q1.y + q0.x * q1.z - q0.z * q1.x;
	r.z = q0.z * q1.w + q0.w * q1.z + q0.y * q1.x - q0.x * q1.y;
	r.w = q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
	return r;
}

// cQuat::Div : const cQuat (const cQuat &, const cQuat &)
inline const cQuat cQuat::Div(const cQuat &q0, const cQuat &q1) {
	return Mul(q0, Invert(q1));
}

// cQuat::operator += : void (const cQuat &)
inline void cQuat::operator += (const cQuat &q) {
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
}

// cQuat::operator -= : void (const cQuat &)
inline void cQuat::operator -= (const cQuat &q) {
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
}

// cQuat::operator *= : void (const cQuat &)
inline void cQuat::operator *= (const cQuat &q) {
	*this = Mul(*this, q);
}

// cQuat::operator /= : void (const cQuat &)
inline void cQuat::operator /= (const cQuat &q) {
	*this = Div(*this, q);
}

// cQuat::operator *= : void (const float)
inline void cQuat::operator *= (const float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

// cQuat::operator /= : void (const float)
inline void cQuat::operator /= (const float s) {
	float is = 1.0f / s;
	x *= is;
	y *= is;
	z *= is;
	w *= is;
}

// cQuat::operator + : const cQuat (const cQuat &) const
inline const cQuat cQuat::operator + (const cQuat &q) const {
	return cQuat(x + q.x, y + q.y, z + q.z, w + q.w);
}

// cQuat::operator - : const cQuat (const cQuat &) const
inline const cQuat cQuat::operator - (const cQuat &q) const {
	return cQuat(x - q.x, y - q.y, z - q.z, w - q.w);
}

// cQuat::operator * : const cQuat (const cQuat &) const
inline const cQuat cQuat::operator * (const cQuat &q) const {
	return Mul(*this, q);
}

// cQuat::operator / : const cQuat (const cQuat &) const
inline const cQuat cQuat::operator / (const cQuat &q) const {
	return Div(*this, q);
}

// cQuat::operator * : const cQuat (const float) const
inline const cQuat cQuat::operator * (const float s) const {
	return cQuat(x * s, y * s, z * s, w * s);
}

// cQuat::friend operator * : const cQuat (const float, const cQuat &)
inline const cQuat operator * (const float s, const cQuat &q) {
	return cQuat(s * q.x, s * q.y, s * q.z, s * q.w);
}

// cQuat::operator / : const cQuat (const float) const
inline const cQuat cQuat::operator / (const float s) const {
	float is = 1.0f / s;
	return cQuat(x * is, y * is, z * is, w * is);
}

// cQuat::operator == : bool (const cQuat &) const
inline bool cQuat::operator == (const cQuat &q) const {
	return x == q.x && y == q.y && z == q.z && w == q.w;
}

// cQuat::operator != : bool (const cQuat &) const
inline bool cQuat::operator != (const cQuat &q) const {
	return x != q.x || y != q.y || z != q.z || w != q.w;
}

// cQuat::Equals : bool (const cQuat &, const cQuat &, const float)
inline bool cQuat::Equals(const cQuat &p, const cQuat &q, const float Eps) {
	if(cMath::Abs(p.x - q.x) > Eps) {
		return false;
	}
	if(cMath::Abs(p.y - q.y) > Eps) {
		return false;
	}
	if(cMath::Abs(p.z - q.z) > Eps) {
		return false;
	}
	if(cMath::Abs(p.w - q.w) > Eps) {
		return false;
	}
	return true;
}

// cQuat::EqualRotations : bool (const cQuat &, const cQuat &, const float)
inline bool cQuat::EqualRotations(const cQuat &q0, const cQuat &q1, const float Eps) {
	return Equals(q0, q1, Eps) || Equals(q0, -q1, Eps);
}

// cQuat::SameHemisphere : bool (const cQuat &, const cQuat &)
inline bool cQuat::SameHemisphere(const cQuat &q0, const cQuat &q1) {
	return Dot(q0, q1) >= 0.0f;
}

// cQuat::CalcW : float () const
inline float cQuat::CalcW() const {
	return cMath::Sqrt(cMath::Abs(1.0f - (x * x + y * y + z * z)));
}

// cQuat::Length : float () const
inline float cQuat::Length() const {
	return cMath::Sqrt(x * x + y * y + z * z + w * w);
}

// cQuat::LengthSq : float () const
inline float cQuat::LengthSq() const {
	return x * x + y * y + z * z + w * w;
}

// cQuat::Normalize : cQuat & ()
inline cQuat & cQuat::Normalize() {
	float l = Length();
	if(l > 0.0f) {
		float il = 1.0f / l;
		x *= il;
		y *= il;
		z *= il;
		w *= il;
	}
	return *this;
}

// cQuat::IsNormalized : bool (const float) const
inline bool cQuat::IsNormalized(const float Eps) const {
	return cMath::IsOne(LengthSq(), Eps);
}

// cQuat::Conjugate : const cQuat (const cQuat &)
inline const cQuat cQuat::Conjugate(const cQuat &q) {
	return cQuat(-q.x, -q.y, -q.z, q.w);
}

// cQuat::Dot : float (const cQuat &, const cQuat &)
inline float cQuat::Dot(const cQuat &q0, const cQuat &q1) {
	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

// cQuat::Invert : const cQuat (const cQuat &)
inline const cQuat cQuat::Invert(const cQuat &q) {
	float l2 = q.LengthSq();
	if(l2 > 0.0f) {
		return Conjugate(q) / l2;
	}
	return q;
}

// cQuat::Normalize : const cQuat (const cQuat &)
inline const cQuat cQuat::Normalize(const cQuat &q) {
	float l = cMath::Sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	if(l > 0.0f) {
		float il = 1.0f / l;
		return cQuat(q.x * il, q.y * il, q.z * il, q.w * il);
	}
	return q;
}

// cQuat::GetDimension : int () const
inline int cQuat::GetDimension() const {
	return 4;
}

// cQuat::ToFloatPtr : const float * () const
inline const float * cQuat::ToFloatPtr() const {
	return &x;
}

// cQuat::ToFloatPtr : float * ()
inline float * cQuat::ToFloatPtr() {
	return &x;
}

// cVec3::Rotate : cVec3 & (const cQuat &)
inline cVec3 & cVec3::Rotate(const cQuat &q) {
	return Transform(q.ToMat3());
}

// cVec3::operator *= : cVec3 & (const cQuat &)
inline cVec3 & cVec3::operator *= (const cQuat &q) {
	return Transform(q.ToMat3());
}

// cVec3::Rotate : const cVec3 (const cVec3 &, const cQuat &)
inline const cVec3 cVec3::Rotate(const cVec3 &u, const cQuat &q) {
	return cVec3::Transform(u, q.ToMat3());
}

// cVec3::operator * : const cVec3 (const cQuat &) const
inline const cVec3 cVec3::operator * (const cQuat &q) const {
	return cVec3::Transform(*this, q.ToMat3());
}