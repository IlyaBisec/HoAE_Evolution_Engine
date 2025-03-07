#pragma once

//*****************************************************************************
// cVec4
//*****************************************************************************
class cVec4 {
public:
	float x;
	float y;
	float z;
	float w;

	cVec4();
	explicit cVec4(const float S);
	cVec4(const float X, const float Y, const float Z, const float W);
	cVec4(const cVec2 &XY, const float Z, const float W);
	cVec4(const cVec3 &XYZ, const float W);

	void SetZero();
	void Set(const float S);
	void Set(const float X, const float Y, const float Z, const float W);
	void Set(const cVec2 &XY, const float Z, const float W);
	void Set(const cVec3 &XYZ, const float W);
	void Copy(const float *pSrc);
	
	float & operator [] (const int index);
	float operator [] (const int index) const;
	
	const cVec4 operator - () const;

	void operator += (const cVec4 &);
	void operator -= (const cVec4 &);
	void operator *= (const cVec4 &);
	void operator *= (const float);
	void operator /= (const cVec4 &);
	void operator /= (const float);

	void Transform(const cMat4 &);
	void operator *= (const cMat4 &);

	const cVec4 operator + (const cVec4 &) const;
	const cVec4 operator - (const cVec4 &) const;
	const cVec4 operator * (const cVec4 &) const;
	const cVec4 operator * (const float) const;
	friend const cVec4 operator * (const float, const cVec4 &);
	const cVec4 operator / (const cVec4 &) const;
	const cVec4 operator / (const float) const;
	
	static const cVec4 Transform(const cVec4 &, const cMat4 &);
	const cVec4 operator * (const cMat4 &) const;

	bool operator == (const cVec4 &) const;
	bool operator != (const cVec4 &) const;
	static bool Equals(const cVec4 &, const cVec4 &, const float Eps = cMath::Epsilon);

	float Length() const;
	float LengthSq() const;
	float Normalize();
	float NormalizeSafe(const cVec4 &Fallback = cVec4::AxisW);
	bool IsNormalized(const float Eps = cMath::Epsilon) const;
	bool IsZero(const float Eps = cMath::Epsilon) const;

	static const cVec4 Abs(const cVec4 &);
	static float Dot(const cVec4 &, const cVec4 &);
//	static cVec4 BaryCentric(const cVec4 &, const cVec4 &, const cVec4 &, const float, const float);
//	static cVec4 Clamp(const cVec4 &, const cVec4 &, const cVec4 &);
//	static cVec4 Cross(const cVec4 &, const cVec4 &, const cVec4 &);
	static float Length(const cVec4 &);
	static float LengthSq(const cVec4 &);
	static const cVec4 Lerp(const cVec4 &, const cVec4 &, const float);
	static const cVec4 Max(const cVec4 &, const cVec4 &);
	static const cVec4 Min(const cVec4 &, const cVec4 &);
	static const cVec4 Normalize(const cVec4 &);

	static const cVec4 Zero;
	static const cVec4 One;
	static const cVec4 Infinity;
	static const cVec4 AxisX;
	static const cVec4 AxisY;
	static const cVec4 AxisZ;
	static const cVec4 AxisW;
	static const cVec4 AxisNegX;
	static const cVec4 AxisNegY;
	static const cVec4 AxisNegZ;
	static const cVec4 AxisNegW;
	
	const float * ToFloatPtr() const;
	float * ToFloatPtr();

	const cVec2 & ToVec2() const;
	cVec2 & ToVec2();
	
	const cVec3 & ToVec3() const;
	cVec3 & ToVec3();

	const cStr ToStr(const int nPrec = 2) const;
	friend cStr & operator << (cStr &, const cVec4 &);
};

// cVec4.ctor : ()
inline cVec4::cVec4() {
}

// cVec4.ctor : (const float)
inline cVec4::cVec4(const float S)
: x(S), y(S), z(S), w(S) {}

// cVec4.ctor : (const float, const float, const float, const float)
inline cVec4::cVec4(const float X, const float Y, const float Z, const float W)
: x(X), y(Y), z(Z), w(W) {}

// cVec4.ctor : (const cVec2 &, const float, const float)
inline cVec4::cVec4(const cVec2 &XY, const float Z, const float W)
: x(XY.x), y(XY.y), z(Z), w(W) {}

// cVec4.ctor : (const cVec3 &, const float)
inline cVec4::cVec4(const cVec3 &XYZ, const float W)
: x(XYZ.x), y(XYZ.y), z(XYZ.z), w(W) {}

// cVec4::SetZero : void ()
inline void cVec4::SetZero() {
	x = y = z = w = 0.0f;
}

// cVec4::Set : void (const float)
inline void cVec4::Set(const float S) {
	x = y = z = w = S;
}

// cVec4::Set : void (const float, const float, const float, const float)
inline void cVec4::Set(const float X, const float Y, const float Z, const float W) {
	x = X;
	y = Y;
	z = Z;
	w = W;
}

// cVec4::Set : void (const cVec2 &, const float, const float)
inline void cVec4::Set(const cVec2 &XY, const float Z, const float W) {
	x = XY.x;
	y = XY.y;
	z = Z;
	w = W;
}

// cVec4::Set : void (const cVec3 &, const float)
inline void cVec4::Set(const cVec3 &XYZ, const float W) {
	x = XYZ.x;
	y = XYZ.y;
	z = XYZ.z;
	w = W;
}

// cVec4::Copy : void (const float *)
inline void cVec4::Copy(const float *pSrc) {
	cAssert(pSrc);
	x = pSrc[0];
	y = pSrc[1];
	z = pSrc[2];
	w = pSrc[3];
}

// cVec4::operator [] : float & (const int)
inline float & cVec4::operator [] (const int index) {
	cAssert(index >= 0 && index < 4);
	return (&x)[index];
}

// cVec4::operator [] : float (const int) const
inline float cVec4::operator [] (const int index) const {
	cAssert(index >= 0 && index < 4);
	return (&x)[index];
}

// cVec4::operator - : const cVec4 () const
inline const cVec4 cVec4::operator - () const {
	return cVec4(-x, -y, -z, -w);
}

// cVec4::operator += : void (const cVec4 &)
inline void cVec4::operator += (const cVec4 &u) {
	x += u.x;
	y += u.y;
	z += u.z;
	w += u.w;
}

// cVec4::operator -= : void (const cVec4 &)
inline void cVec4::operator -= (const cVec4 &u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
	w -= u.w;
}

// cVec4::operator *= : void (const cVec4 &)
inline void cVec4::operator *= (const cVec4 &u) {
	x *= u.x;
	y *= u.y;
	z *= u.z;
	w *= u.w;
}

// cVec4::operator *= : void (const float)
inline void cVec4::operator *= (const float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

// cVec4::operator /= : void (const cVec4 &)
inline void cVec4::operator /= (const cVec4 &u) {
	x /= u.x;
	y /= u.y;
	z /= u.z;
	w /= u.w;
}

// cVec4::operator /= : void (const float)
inline void cVec4::operator /= (const float s) {
	float is = 1.0f / s;
	x *= is;
	y *= is;
	z *= is;
	w *= is;
}

// cVec4::operator + : const cVec4 (const cVec4 &) const
inline const cVec4 cVec4::operator + (const cVec4 &u) const {
	return cVec4(x + u.x, y + u.y, z + u.z, w + u.w);
}

// cVec4::operator - : const cVec4 (const cVec4 &) const
inline const cVec4 cVec4::operator - (const cVec4 &u) const {
	return cVec4(x - u.x, y - u.y, z - u.z, w - u.w);
}

// cVec4::operator * : const cVec4 (const cVec4 &) const
inline const cVec4 cVec4::operator * (const cVec4 &u) const {
	return cVec4(x * u.x, y * u.y, z * u.z, w * u.w);
}

// cVec4::operator * : const cVec4 (const float) const
inline const cVec4 cVec4::operator * (const float s) const {
	return cVec4(x * s, y * s, z * s, w * s);
}

// cVec4::friend operator * : const cVec4 (const float, const cVec4 &)
inline const cVec4 operator * (const float s, const cVec4 &u) {
	return cVec4(s * u.x, s * u.y, s * u.z, s * u.w);
}

// cVec4::operator / : const cVec4 (const cVec4 &) const
inline const cVec4 cVec4::operator / (const cVec4 &u) const {
	return cVec4(x / u.x, y / u.y, z / u.z, w / u.w);
}

// cVec4::operator / : const cVec4 (const float) const
inline const cVec4 cVec4::operator / (const float s) const {
	float is = 1.0f / s;
	return cVec4(x * is, y * is, z * is, w * is);
}

// cVec4::operator == : bool (const cVec4 &) const
inline bool cVec4::operator == (const cVec4 &u) const {
	return x == u.x && y == u.y && z == u.z && w == u.w;
}

// cVec4::operator != : bool (const cVec4 &) const
inline bool cVec4::operator != (const cVec4 &u) const {
	return x != u.x || y != u.y || z != u.z || w != u.w;
}

// cVec4::Equals : bool (const cVec4 &, const cVec4 &, const float)
inline bool cVec4::Equals(const cVec4 &u, const cVec4 &v, const float Eps /*= cMath::Epsilon*/) {
	if(cMath::Abs(u.x - v.x) > Eps) {
		return false;
	}
	if(cMath::Abs(u.y - v.y) > Eps) {
		return false;
	}
	if(cMath::Abs(u.z - v.z) > Eps) {
		return false;
	}
	if(cMath::Abs(u.w - v.w) > Eps) {
		return false;
	}
	return true;
}

// cVec4::Length : float () const
inline float cVec4::Length() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

// cVec4::LengthSq : float () const
inline float cVec4::LengthSq() const {
	return x * x + y * y + z * z + w * w;
}

// cVec4::Normalize : float ()
inline float cVec4::Normalize() {
	const float l = Length();
	if(l > 0.0f) {
		(*this) /= l;
	}
	return l;
}

// cVec4::NormalizeSafe : float (const cVec4 &)
inline float cVec4::NormalizeSafe(const cVec4 &Fallback /*= cVec4::wAxis*/) {
	const float l = Length();
	if(l < cMath::Epsilon) {
		*this = Fallback;
		cAssert(IsNormalized());
		return 0.0f;
	} else {
		(*this) /= l;
		cAssert(IsNormalized());
		return l;
	}
}

// cVec4::IsNormalized : bool (const float) const
inline bool cVec4::IsNormalized(const float Eps) const {
	return cMath::IsOne(LengthSq(), Eps);
}

// cVec4::IsZero : bool(const float) const
inline bool cVec4::IsZero(const float Eps) const {
	if(!cMath::IsZero(x, Eps)) {
		return false;
	}
	if(!cMath::IsZero(y, Eps)) {
		return false;
	}
	if(!cMath::IsZero(z, Eps)) {
		return false;
	}
	if(!cMath::IsZero(w, Eps)) {
		return false;
	}
	return true;
}

// cVec4::Abs : const cVec4 (const cVec4 &)
inline const cVec4 cVec4::Abs(const cVec4 &u) {
	return cVec4(cMath::Abs(u.x), cMath::Abs(u.y), cMath::Abs(u.z), cMath::Abs(u.w));
}

// cVec4::Dot : float (const cVec4 &, const cVec4 &)
inline float cVec4::Dot(const cVec4 &u, const cVec4 &v) {
	return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

// cVec4::Length : float (const cVec4 &)
inline float cVec4::Length(const cVec4 &u) {
	return cMath::Sqrt(u.x * u.x + u.y * u.y + u.z * u.z + u.w * u.w);
}

// cVec4::LengthSq : float (const cVec4 &)
inline float cVec4::LengthSq(const cVec4 &u) {
	return u.x * u.x + u.y * u.y + u.z * u.z + u.w * u.w;
}

// cVec4::Lerp : const cVec4 (const cVec4 &, const cVec4 &, const float)
inline const cVec4 cVec4::Lerp(const cVec4 &u, const cVec4 &v, const float s) {
	cVec4 r;
	r.x = u.x + s * (v.x - u.x);
	r.y = u.y + s * (v.y - u.y);
	r.z = u.z + s * (v.z - u.z);
	r.w = u.w + s * (v.w - u.w);
	return r;
}

// cVec4::Max : const cVec4 (const cVec4 &, const cVec4 &)
inline const cVec4 cVec4::Max(const cVec4 &u, const cVec4 &v) {
	cVec4 r;
	r.x = u.x > v.x ? u.x : v.x;
	r.y = u.y > v.y ? u.y : v.y;
	r.z = u.z > v.z ? u.z : v.z;
	r.w = u.w > v.w ? u.w : v.w;
	return r;
}

// cVec4::Min : const cVec4 (const cVec4 &, const cVec4 &)
inline const cVec4 cVec4::Min(const cVec4 &u, const cVec4 &v) {
	cVec4 r;
	r.x = u.x < v.x ? u.x : v.x;
	r.y = u.y < v.y ? u.y : v.y;
	r.z = u.z < v.z ? u.z : v.z;
	r.w = u.w < v.w ? u.w : v.w;
	return r;
}

// cVec4::Normalize : const cVec4 (const cVec4 &)
inline const cVec4 cVec4::Normalize(const cVec4 &u) {
	const float l = cMath::Sqrt(u.x * u.x + u.y * u.y + u.z * u.z + u.w * u.w);
	if(l > 0.0f) {
		const float il = 1.0f / l;
		return cVec4(u.x * il, u.y * il, u.z * il, u.w * il);
	}
	return u;
}

// cVec4::ToFloatPtr : const float * () const
inline const float * cVec4::ToFloatPtr() const {
	return (const float *)&x;
}

// cVec4::ToFloatPtr : float * ()
inline float * cVec4::ToFloatPtr() {
	return (float *)&x;
}

// cVec4::ToVec2 : const cVec2 & () const
inline const cVec2 & cVec4::ToVec2() const {
	return *reinterpret_cast<const cVec2 *>(this);
}

// cVec4::ToVec2 : cVec2 & ()
inline cVec2 & cVec4::ToVec2() {
	return *reinterpret_cast<cVec2 *>(this);
}

// cVec4::ToVec3 : const cVec3 & () const
inline const cVec3 & cVec4::ToVec3() const {
	return *reinterpret_cast<const cVec3 *>(this);
}

// cVec4::ToVec3 : cVec3 & ()
inline cVec3 & cVec4::ToVec3() {
	return *reinterpret_cast<cVec3 *>(this);
}
