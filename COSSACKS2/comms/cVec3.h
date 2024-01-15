#pragma once

class cVec3 {
public:
	float x;
	float y;
	float z;

	cVec3();
	explicit cVec3(const float S);
	cVec3(const float X, const float Y, const float Z);
	cVec3(const cVec2 &XY, const float Z);

	cVec3 & Copy(const float *pSrc);
	cVec3 & SetZero();
	cVec3 & SetOne();
	cVec3 & Set(const float S);
	cVec3 & Set(const float X, const float Y, const float Z);
	cVec3 & Set(const cVec2 &XY, const float Z);

	float & operator [] (const int index);
	float operator [] (const int index) const;

	const cVec3 operator - () const;

	cVec3 & operator += (const cVec3 &);
	cVec3 & operator -= (const cVec3 &);
	cVec3 & operator *= (const cVec3 &);
	cVec3 & operator *= (const float);
	cVec3 & operator /= (const cVec3 &);
	cVec3 & operator /= (const float);
	
	cVec3 & TransformCoordinate(const cMat4 &);
	cVec3 & TransformNormal(const cMat4 &);

	cVec3 & Transform(const cMat3 &);
	cVec3 & operator *= (const cMat3 &);

	cVec3 & Rotate(const cQuat &);
	cVec3 & operator *= (const cQuat &);
	
	cVec3 & Rotate(const cRotation &);
	cVec3 & operator *= (const cRotation &);

	const cVec3 operator + (const cVec3 &) const;
	const cVec3 operator - (const cVec3 &) const;
	const cVec3 operator * (const cVec3 &) const;
	const cVec3 operator * (const float) const;
	friend const cVec3 operator * (const float, const cVec3 &);
	const cVec3 operator / (const cVec3 &) const;
	const cVec3 operator / (const float) const;

	static const cVec4 Transform(const cVec3 &, const cMat4 &); // (x, y, z, 1)
	static const cVec3 TransformCoordinate(const cVec3 &, const cMat4 &);
	static const cVec3 TransformNormal(const cVec3 &, const cMat4 &);
	
	static const cVec3 Transform(const cVec3 &u, const cMat3 &);
	const cVec3 operator * (const cMat3 &) const;
	
	static const cVec3 Rotate(const cVec3 &, const cQuat &);
	const cVec3 operator * (const cQuat &) const;
	
	static const cVec3 Rotate(const cVec3 &, const cRotation &);
	const cVec3 operator * (const cRotation &) const;

	bool operator == (const cVec3 &) const;
	bool operator != (const cVec3 &) const;
	static bool Equals(const cVec3 &, const cVec3 &, const float Eps = cMath::Epsilon);

	float Length() const;
	float LengthSq() const;
	float Normalize();
	float NormalizeSafe(const cVec3 &Fallback = cVec3::AxisZ);
	bool FixDegenerateNormal();
	bool FixDenormals();
	bool IsValid() const;
	bool IsNormalized(const float Eps = cMath::Epsilon) const;
	bool IsZero(const float Eps = cMath::Epsilon) const;

	static const cVec3 Abs(const cVec3 &);
	static float Angle(const cVec3 &, const cVec3 &);
	static float AreaSigned(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2);
//	static float Perimeter(const cVec3 &, const cVec3 &, const cVec3 &);
//	static float InCircle(cVec3 &Center, const cVec3 &, const cVec3 &, const cVec3 &);
//	static float CircCircle(cVec3 &Center, const cVec3 &t0, const cVec3 &t1, const cVec3 &t3);
	static const cVec3 BaryCentric(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, const float f, const float g);
	static const cVec3 Clamp(const cVec3 &, const cVec3 &, const cVec3 &);
	static const cVec3 Cross(const cVec3 &, const cVec3 &);
	static float Distance(const cVec3 &, const cVec3 &);
	static float DistanceSq(const cVec3 &, const cVec3 &);
	static cVec3 FromSpherical(const float Radius, const float Phi, const float Psi);
	static float Dot(const cVec3 &, const cVec3 &);
	static float Length(const cVec3 &);
	static float LengthSq(const cVec3 &);
	static const cVec3 Lerp(const cVec3 &, const cVec3 &, const float);
	static const cVec3 Lerp05(const cVec3 &, const cVec3 &);
	static const cVec3 Max(const cVec3 &, const cVec3 &);
	static const cVec3 Min(const cVec3 &, const cVec3 &);
	static const cVec3 Normalize(const cVec3 &);
	static const cVec3 Reflect(const cVec3 &RayDir, const cVec3 &Normal);
	static const cVec3 Refract(const cVec3 &RayDir, const cVec3 &Normal, const float Eta);
	static const cVec3 Slerp(const cVec3 &n0, const cVec3 &n1, const float s);
	static const cVec3 Truncate(const cVec3 &u, const float MaxLen);
	static const cVec3 RandRange1();
	static const cVec3 RandNormal();
	
	static const cVec3 Zero;
	static const cVec3 One;
	static const cVec3 Infinity;
	static const cVec3 AxisX;
	static const cVec3 AxisY;
	static const cVec3 AxisZ;
	static const cVec3 AxisNegX;
	static const cVec3 AxisNegY;
	static const cVec3 AxisNegZ;
	
	int GetDimension() const;

	const float * ToFloatPtr() const;
	float * ToFloatPtr();

	const cVec2 & ToVec2() const;
	cVec2 & ToVec2();
	
	cStr ToStr(int nPrec = 2) const;
	friend cStr & operator << (cStr &, const cVec3 &);

	void ToSpherical(float &r, float &Phi, float &Psi) const;

	cAngles ToAngles() const;
	float ToYaw() const;
	float ToPitch() const;
	
	float ToBaryCentric(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, float &f, float &g) const;

	const cVec3 ToNormal() const;
	void ToPerps(cVec3 &X, cVec3 &Y) const;
	const cVec3 ToPerp() const;
};

// cVec3.ctor : ()
inline cVec3::cVec3() {
}

// cVec3.ctor : (const float)
inline cVec3::cVec3(const float S)
: x(S), y(S), z(S) {}

// cVec3.ctor : (const float, const float, const float)
inline cVec3::cVec3(const float X, const float Y, const float Z)
: x(X), y(Y), z(Z) {}

// cVec3.ctor : (const cVec2 &, const float)
inline cVec3::cVec3(const cVec2 &XY, const float Z)
: x(XY.x), y(XY.y), z(Z) {}

// cVec3::SetZero : cVec3 & ()
inline cVec3 & cVec3::SetZero() {
	x = y = z = 0.0f;
	return *this;
}

// cVec3::SetOne : cVec3 & ()
inline cVec3 & cVec3::SetOne() {
	x = y = z = 1.0f;
	return *this;
}

// cVec3::Set : cVec3 & (const float)
inline cVec3 & cVec3::Set(const float S) {
	x = y = z = S;
	return *this;
}

// cVec3::Set : cVec3 & (const float, const float, const float)
inline cVec3 & cVec3::Set(const float X, const float Y, const float Z) {
	x = X;
	y = Y;
	z = Z;
	return *this;
}

// cVec3::Set : cVec3 & (const cVec2 &, const float)
inline cVec3 & cVec3::Set(const cVec2 &XY, const float Z) {
	x = XY.x;
	y = XY.y;
	z = Z;
	return *this;
}

// cVec3::Copy : cVec3 & (const float *)
inline cVec3 & cVec3::Copy(const float *pSrc) {
	cAssert(pSrc);
	x = pSrc[0];
	y = pSrc[1];
	z = pSrc[2];
	return *this;
}

// cVec3::operator [] : float & (const int)
inline float & cVec3::operator [] (const int index) {
	cAssert(index >= 0 && index < 3);
	return (&x)[index];
}

// cVec3::operator [] : float(const int) const
inline float cVec3::operator [] (const int index) const {
	cAssert(index >= 0 && index < 3);
	return (&x)[index];
}

// cVec3::operator - : const cVec3() const
inline const cVec3 cVec3::operator - () const {
	return cVec3(-x, -y, -z);
}

// cVec3::operator += : cVec3 & (const cVec3 &)
inline cVec3 & cVec3::operator += (const cVec3 &u) {
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

// cVec3::operator -= : cVec3 & (const cVec3 &)
inline cVec3 & cVec3::operator -= (const cVec3 &u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

// cVec3::operator *= : cVec3 & (const cVec3 &)
inline cVec3 & cVec3::operator *= (const cVec3 &u) {
	x *= u.x;
	y *= u.y;
	z *= u.z;
	return *this;
}

// cVec3::operator *= : cVec3 & (const float)
inline cVec3 & cVec3::operator *= (const float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

// cVec3::operator /= : cVec3 & (const cVec3 &)
inline cVec3 & cVec3::operator /= (const cVec3 &u) {
	x /= u.x;
	y /= u.y;
	z /= u.z;
	return *this;
}

// cVec3::operator /= : cVec3 & (const float)
inline cVec3 & cVec3::operator /= (const float s) {
	float is = 1.0f / s;
	x *= is;
	y *= is;
	z *= is;
	return *this;
}

// cVec3::operator + : const cVec3(const cVec3 &) const
inline const cVec3 cVec3::operator + (const cVec3 &u) const {
	return cVec3(x + u.x, y + u.y, z + u.z);
}

// cVec3::operator - : const cVec3(const cVec3 &) const
inline const cVec3 cVec3::operator - (const cVec3 &u) const {
	return cVec3(x - u.x, y - u.y, z - u.z);
}

// cVec3::operator * : const cVec3(const cVec3 &) const
inline const cVec3 cVec3::operator * (const cVec3 &u) const {
	return cVec3(x * u.x, y * u.y, z * u.z);
}

// cVec3::operator * : const cVec3(const float) const
inline const cVec3 cVec3::operator * (const float s) const {
	return cVec3(x * s, y * s, z * s);
}

// cVec3::friend operator * : const cVec3(const float, const cVec3 &)
inline const cVec3 operator * (const float s, const cVec3 &u) {
	return cVec3(s * u.x, s * u.y, s * u.z);
}

// cVec3::operator / : const cVec3(const cVec3 &) const
inline const cVec3 cVec3::operator / (const cVec3 &u) const {
	return cVec3(x / u.x, y / u.y, z / u.z);
}

// cVec3::operator / : const cVec3(const float) const
inline const cVec3 cVec3::operator / (const float s) const {
	float is = 1.0f / s;
	return cVec3(x * is, y * is, z * is);
}

// cVec3::operator == : bool(const cVec3 &) const
inline bool cVec3::operator == (const cVec3 &u) const {
	return x == u.x && y == u.y && z == u.z;
}

// cVec3::operator != : bool(const cVec3 &) const
inline bool cVec3::operator != (const cVec3 &u) const {
	return x != u.x || y != u.y || z != u.z;
}

// cVec3::Equals : bool(const cVec3 &, const cVec3 &, const float)
inline bool cVec3::Equals(const cVec3 &u, const cVec3 &v, const float Eps /*= cMath::Epsilon*/) {
	if(cMath::Abs(u.x - v.x) > Eps) {
		return false;
	}
	if(cMath::Abs(u.y - v.y) > Eps) {
		return false;
	}
	if(cMath::Abs(u.z - v.z) > Eps) {
		return false;
	}
	return true;
}

// cVec3::Length : float() const
inline float cVec3::Length() const {
	return cMath::Sqrt(x * x + y * y + z * z);
}

// cVec3::LengthSq : float() const
inline float cVec3::LengthSq() const {
	return x * x + y * y + z * z;
}

// cVec3::Normalize : float()
inline float cVec3::Normalize() {
	float l = Length();
	if(l > 0) {
		float il = 1.0f / l;
		x *= il;
		y *= il;
		z *= il;
	}
	return l;
}

// cVec3::NormalizeSafe
inline float cVec3::NormalizeSafe(const cVec3 &Fallback /*= cVec3::AxisZ*/) {
	const float l = Length();
	if(l < cMath::Epsilon) {
		*this = Fallback;
		cAssert(IsNormalized());
		return 0.0f;
	} else {

		(*this) *= 1.0f / l;
		cAssert(IsNormalized());
		return l;
	}
}

// cVec3::IsValid : bool () const
inline bool cVec3::IsValid() const {
	return cMath::IsValid(x) && cMath::IsValid(y) && cMath::IsValid(z);
}

// cVec3::IsNormalized : bool(const float) const
inline bool cVec3::IsNormalized(const float Eps) const {
	return cMath::IsOne(LengthSq(), Eps);
}

// cVec3::IsZero : bool(const float) const
inline bool cVec3::IsZero(const float Eps) const {
	if(!cMath::IsZero(x, Eps)) return false;
	if(!cMath::IsZero(y, Eps)) return false;
	if(!cMath::IsZero(z, Eps)) return false;
	return true;
}

// cVec3::Abs : const cVec3(const cVec3 &)
inline const cVec3 cVec3::Abs(const cVec3 &u) {
	return cVec3(cMath::Abs(u.x), cMath::Abs(u.y), cMath::Abs(u.z));
}

// cVec3::Angle : float(const cVec3 &, const cVec3 &)
inline float cVec3::Angle(const cVec3 &u, const cVec3 &v) {
	const float D = cVec3::Dot(u, v);
	float ll = u.Length() * v.Length();
	return cMath::Deg(cMath::ACos(D / ll));
}

// cVec3::AreaSigned
inline float cVec3::AreaSigned(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2) {
	const cVec3 e1 = t1 - t0;
	const cVec3 e2 = t2 - t0;
	return Length(Cross(e1, e2)) * 0.5f;
}

// cVec3::BaryCentric
inline const cVec3 cVec3::BaryCentric(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, const float f, const float g) {
	const cVec3 e1 = t1 - t0;
	const cVec3 e2 = t2 - t0;
	return t0 + f * e1 + g * e2;
}

// cVec3::ToBaryCentric
inline float cVec3::ToBaryCentric(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, float &f, float &g) const {
	const float A = AreaSigned(t0, t1, t2);
	if(A != 0) {
		const float iA = 1.0f / A;
		f = AreaSigned(t0, *this, t2) * iA;
		g = AreaSigned(t0, t1, *this) * iA;
	} else {
		f = g = 0.0f;
	}
	return A;
}

// cVec3::Clamp
inline const cVec3 cVec3::Clamp(const cVec3 &u, const cVec3 &bbMin, const cVec3 &bbMax) {
	cVec3 r;
	r.x = u.x < bbMin.x ? bbMin.x : (u.x > bbMax.x ? bbMax.x : u.x);
	r.y = u.y < bbMin.y ? bbMin.y : (u.y > bbMax.y ? bbMax.y : u.y);
	r.z = u.z < bbMin.z ? bbMin.z : (u.z > bbMax.z ? bbMax.z : u.z);
	return r;
}

// cVec3::Cross
inline const cVec3 cVec3::Cross(const cVec3 &u, const cVec3 &v) {
	return cVec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

// cVec3::DistanceSq : float(const cVec3 &, const cVec3 &)
inline float cVec3::DistanceSq(const cVec3 &u, const cVec3 &v) {
	float dx = u.x - v.x;
	float dy = u.y - v.y;
	float dz = u.z - v.z;
	return dx * dx + dy * dy + dz * dz;
}

// cVec3::Distance : float(const cVec3 &, const cVec3 &)
inline float cVec3::Distance(const cVec3 &u, const cVec3 &v) {
	float dx = u.x - v.x;
	float dy = u.y - v.y;
	float dz = u.z - v.z;
	return cMath::Sqrt(dx * dx + dy * dy + dz * dz);
}

// cVec3::Dot : float(const cVec3 &, const cVec3 &)
inline float cVec3::Dot(const cVec3 &u, const cVec3 &v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

// cVec3::Length : float(const cVec3 &)
inline float cVec3::Length(const cVec3 &u) {
	return cMath::Sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

// cVec3::LengthSq : float(const cVec3 &)
inline float cVec3::LengthSq(const cVec3 &u) {
	return u.x * u.x + u.y * u.y + u.z * u.z;
}

// cVec3::Lerp
inline const cVec3 cVec3::Lerp(const cVec3 &u, const cVec3 &v, const float s) {
	cVec3 r;
	r.x = u.x + s * (v.x - u.x);
	r.y = u.y + s * (v.y - u.y);
	r.z = u.z + s * (v.z - u.z);
	return r;
}

// cVec3::Lerp05 : const cVec3(const cVec3 &, const cVec3 &)
inline const cVec3 cVec3::Lerp05(const cVec3 &u, const cVec3 &v) {
	cVec3 r;
	r.x = 0.5f * (u.x + v.x);
	r.y = 0.5f * (u.y + v.y);
	r.z = 0.5f * (u.z + v.z);
	return r;
}

// cVec3::Max : const cVec3(const cVec3 &, const cVec3 &)
inline const cVec3 cVec3::Max(const cVec3 &u, const cVec3 &v) {
	cVec3 r;
	r.x = u.x > v.x ? u.x : v.x;
	r.y = u.y > v.y ? u.y : v.y;
	r.z = u.z > v.z ? u.z : v.z;
	return r;
}

// cVec3::Min : const cVec3(const cVec3 &, const cVec3 &)
inline const cVec3 cVec3::Min(const cVec3 &u, const cVec3 &v) {
	cVec3 r;
	r.x = u.x < v.x ? u.x : v.x;
	r.y = u.y < v.y ? u.y : v.y;
	r.z = u.z < v.z ? u.z : v.z;
	return r;
}

// cVec3::Normalize
inline const cVec3 cVec3::Normalize(const cVec3 &u) {
	float l = cMath::Sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
	if(l > 0) {
		float il = 1.0f / l;
		return cVec3(u.x * il, u.y * il, u.z * il);
	}
	return u;
}

// cVec3::Reflect
inline const cVec3 cVec3::Reflect(const cVec3 &RayDir, const cVec3 &Normal) {
	return RayDir - 2.0f * Dot(RayDir, Normal) * Normal;
}

// cVec3::Refract
inline const cVec3 cVec3::Refract(const cVec3 &RayDir, const cVec3 &Normal, const float Eta) {
	float k = 1.0f - Eta * Eta * (1.0f - cMath::Square(Dot(Normal, RayDir)));
	if(k < 0.0f) return cVec3::Zero;
	return Eta * RayDir - (Eta * Dot(Normal, RayDir) + cMath::Sqrt(k)) * Normal;
}

// cVec3::Truncate
inline const cVec3 cVec3::Truncate(const cVec3 &u, const float MaxLen) {
	if(MaxLen <= 0) {
		return cVec3::Zero;
	}
	float l = u.Length();
	if(l > MaxLen) {
		float il = MaxLen / l;
		return cVec3(u.x * il, u.y * il, u.z * il);
	}
	return u;
}

// cVec3::GetDimension : int() const
inline int cVec3::GetDimension() const {
	return 3;
}

// cVec3::ToFloatPtr : const float * () const
inline const float * cVec3::ToFloatPtr() const {
	return (const float *)&x;
}

// cVec3::ToFloatPtr : float * ()
inline float * cVec3::ToFloatPtr() {
	return (float *)&x;
}

// cVec3::ToVec2 : const cVec2 & () const
inline const cVec2 & cVec3::ToVec2() const {
	return *reinterpret_cast<const cVec2 *>(this);
}

// cVec3::ToVec2 : cVec2 & ()
inline cVec2 & cVec3::ToVec2() {
	return *reinterpret_cast<cVec2 *>(this);
}

// cVec3::ToNormal : const cVec2 () const
inline const cVec3 cVec3::ToNormal() const {
	cVec3 u(*this);
	u.Normalize();
	return u;
}