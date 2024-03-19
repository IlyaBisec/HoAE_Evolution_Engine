#pragma once

class cVec2 {
public:
	float x;
	float y;

	cVec2();
	explicit cVec2(const float S);
	cVec2(const float X, const float Y);

	cVec2 & Copy(const float *pSrc);
	cVec2 & SetZero();
	cVec2 & SetOne();
	cVec2 & Set(const float S);
	cVec2 & Set(const float X, const float Y);

	float operator [] (const int index) const;
	float & operator [] (const int index);

	cVec2 operator - () const;

	cVec2 & operator += (const cVec2 &);
	cVec2 & operator -= (const cVec2 &);
	cVec2 & operator *= (const float);
	cVec2 & operator /= (const float);

	cVec2 & Transform(const cMat2 &);
	cVec2 & operator *= (const cMat2 &);
	cVec2 & TransformCoordinate(const cMat3 &);
	cVec2 & TransformNormal(const cMat3 &);
	cVec2 & TransformCoordinate(const cMat4 &);
	cVec2 & TransformNormal(const cMat4 &);

	cVec2 operator + (const cVec2 &) const;
	cVec2 operator - (const cVec2 &) const;
	cVec2 operator * (const float) const;
	friend cVec2 operator * (const float, const cVec2 &);
	cVec2 operator / (const float) const;

	float operator * (const cVec2 &) const;

	static cVec2 Transform(const cVec2 &, const cMat2 &);
	cVec2 operator * (const cMat2 &) const;
	static cVec3 Transform(const cVec2 &, const cMat3 &); // (x, y, 1)
	static cVec2 TransformCoordinate(const cVec2 &, const cMat3 &); // (x, y, 1)
	static cVec2 TransformNormal(const cVec2 &, const cMat3 &); // (x, y, 0)
	static cVec4 Transform(const cVec2 &, const cMat4 &); // (x, y, 0, 1)
	static cVec2 TransformCoordinate(const cVec2 &, const cMat4 &); // (x, y, 0, 1), projects the result back into w = 1.
	static cVec2 TransformNormal(const cVec2 &, const cMat4 &); // (x, y, 0, 0)

	bool operator == (const cVec2 &) const;
	bool operator != (const cVec2 &) const;
	static bool Equals(const cVec2 &, const cVec2 &, const float Eps = cMath::Epsilon);

	float Length() const;
	float LengthSq() const;
	float Normalize();
	bool IsValid() const;
	bool IsNormalized(const float Eps = cMath::Epsilon) const;
	bool IsZero(const float Eps = cMath::Epsilon) const;

	static const cVec2 Abs(const cVec2 &);
	static float Angle(const cVec2 &, const cVec2 &);
	static float AreaSigned(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2);
//	static float Perimeter(const cVec2 &, const cVec2 &, const cVec2 &);
//	static float InCircle(cVec2 &Center, const cVec2 &t0, const cVec2 &t1, const cVec2 &t2);
//	static float CircCircle(cVec2 &Center,const cVec2 &t0, const cVec2 &t1, const cVec2 &t2);
	static float Ccw(const cVec2 &, const cVec2 &);
	static const cVec2 Clamp(const cVec2 &, const cVec2 &, const cVec2 &);
	static float Distance(const cVec2 &, const cVec2 &);
	static float DistanceSq(const cVec2 &, const cVec2 &);
	static float Dot(const cVec2 &, const cVec2 &);
	static float Length(const cVec2 &);
	static float LengthSq(const cVec2 &);
	static const cVec2 Lerp(const cVec2 &, const cVec2 &, const float);
	static const cVec2 Lerp05(const cVec2 &, const cVec2 &);
	static const cVec2 Max(const cVec2 &, const cVec2 &);
	static const cVec2 Min(const cVec2 &, const cVec2 &);
	static const cVec2 Normalize(const cVec2 &);
	static const cVec2 PerpCw(const cVec2 &);
	static const cVec2 PerpCcw(const cVec2 &);
	
	float DistanceToLineSegSq(const cVec2 &A, const cVec2 &B, float *pScale = NULL) const;
	
	static cVec2 FromBaryCentric(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, const float u, const float v);
	static cVec2 FromPolar(const float Radius, const float Angle);

	static const cVec2 Zero;
	static const cVec2 One;
	static const cVec2 Infinity;
	static const cVec2 AxisX;
	static const cVec2 AxisY;
	static const cVec2 AxisNegX;
	static const cVec2 AxisNegY;
	
	int GetDimension() const;

	const float * ToFloatPtr() const;
	float * ToFloatPtr();

	void ToPolar(float &Radius, float &Angle) const;
	float ToBaryCentric(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, float &u, float &v) const;
	bool IsInsideTri(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2) const;

	const cStr ToStr(int nPrec = 2) const;
	friend cStr & operator << (cStr &, const cVec2 &);

	const cVec2 ToNormal() const;
};

// cVec2.ctor : ()
inline cVec2::cVec2() {
}

// cVec2.ctor : (const float)
inline cVec2::cVec2(const float S)
: x(S), y(S) {}

// cVec2.ctor : (const float, const float)
inline cVec2::cVec2(const float X, const float Y)
: x(X), y(Y) {}

// cVec2::Copy : cVec2 & (const float *)
inline cVec2 & cVec2::Copy(const float *pSrc) {
	cAssert(pSrc);
	x = pSrc[0];
	y = pSrc[1];
	return *this;
}

// cVec2::SetZero : cVec2 & ()
inline cVec2 & cVec2::SetZero() {
	x = y = 0.0f;
	return *this;
}

// cVec2::SetOne : cVec2 & ()
inline cVec2 & cVec2::SetOne() {
	x = y = 1.0f;
	return *this;
}

// cVec2::Set : cVec2 & (const float)
inline cVec2 & cVec2::Set(const float S) {
	x = y = S;
	return *this;
}

// cVec2::Set : cVec2 & (const float, const float)
inline cVec2 & cVec2::Set(const float X, const float Y) {
	x = X;
	y = Y;
	return *this;
}

// cVec2::operator [] : float(const int) const
inline float cVec2::operator [] (const int index) const {
	cAssert(index >= 0 && index < 2);
	return (&x)[index];
}

// cVec2::operator [] : float & (const int)
inline float & cVec2::operator [] (const int index) {
	cAssert(index >= 0 && index < 2);
	return (&x)[index];
}

// cVec2::operator - : cVec2() const
inline cVec2 cVec2::operator - () const {
	return cVec2(-x, -y);
}

// cVec2::operator += : cVec2 & (const cVec2 &)
inline cVec2 & cVec2::operator += (const cVec2 &u) {
	x += u.x;
	y += u.y;
	return *this;
}

// cVec2::operator -= : cVec2 & (const cVec2 &)
inline cVec2 & cVec2::operator -= (const cVec2 &u) {
	x -= u.x;
	y -= u.y;
	return *this;
}

// cVec2::operator *= : cVec2 & (const float)
inline cVec2 & cVec2::operator *= (const float s) {
	x *= s;
	y *= s;
	return *this;
}

// cVec2::operator /= : cVec2 & (const float)
inline cVec2 & cVec2::operator /= (const float s) {
	float is = 1.0f / s;
	x *= is;
	y *= is;
	return *this;
}

// cVec2::operator + : cVec2(const cVec2 &) const
inline cVec2 cVec2::operator + (const cVec2 &u) const {
	return cVec2(x + u.x, y + u.y);
}

// cVec2::operator - : cVec2(const cVec2 &) const
inline cVec2 cVec2::operator - (const cVec2 &u) const {
	return cVec2(x - u.x, y - u.y);
}

// cVec2::operator * : cVec2(const float) const
inline cVec2 cVec2::operator * (const float s) const {
	return cVec2(x * s, y * s);
}

// cVec2::friend operator * : cVec2(const float, const cVec2 &)
inline cVec2 operator * (const float s, const cVec2 &u) {
	return cVec2(s * u.x, s * u.y);
}

// cVec2::operator / : cVec2(const float) const
inline cVec2 cVec2::operator / (const float s) const {
	float is = 1.0f / s;
	return cVec2(x * is, y * is);
}

// cVec2::operator * : float(const cVec2 &) const
inline float cVec2::operator * (const cVec2 &u) const {
	return x * u.x + y * u.y;
}

// cVec2::operator == : bool(const cVec2 &) const
inline bool cVec2::operator == (const cVec2 &u) const {
	return x == u.x && y == u.y;
}

// cVec2::operator != : bool(const cVec2 &) const
inline bool cVec2::operator != (const cVec2 &u) const {
	return x != u.x || y != u.y;
}

// cVec2::Equals : bool(const cVec2 &, const cVec2 &, const float)
inline bool cVec2::Equals(const cVec2 &u, const cVec2 &v, const float Eps) {
	if(cMath::Abs(u.x - v.x) > Eps) return false;
	if(cMath::Abs(u.y - v.y) > Eps) return false;
	return true;
}

// cVec2::Length : float() const
inline float cVec2::Length() const {
	return sqrtf(x * x + y * y);
}

// cVec2::LengthSq : float() const
inline float cVec2::LengthSq() const {
	return x * x + y * y;
}

// cVec2::Normalize : float()
inline float cVec2::Normalize() {
	float l = Length();
	if(l > 0) {
		float il = 1.0f / l;
		x *= il;
		y *= il;
	}
	return l;
}

// cVec2::IsValid : bool () const
inline bool cVec2::IsValid() const {
	return cMath::IsValid(x) && cMath::IsValid(y);
}

// cVec2::IsNormalized : bool(const float) const
inline bool cVec2::IsNormalized(const float Eps) const {
	return cMath::IsOne(LengthSq(), Eps);
}

// cVec2::IsZero : bool(const float) const
inline bool cVec2::IsZero(const float Eps) const {
	if(!cMath::IsZero(x, Eps)) return false;
	if(!cMath::IsZero(y, Eps)) return false;
	return true;
}

// cVec2::Normalize : const cVec2(const cVec2 &)
inline const cVec2 cVec2::Normalize(const cVec2 &u) {
	float l = cMath::Sqrt(u.x * u.x + u.y * u.y);
	if(l > 0) {
		float il = 1.0f / l;
		return cVec2(u.x * il, u.y * il);
	}
	return u;
}

// cVec2::FromPolar : cVec2(const float, const float)
inline cVec2 cVec2::FromPolar(const float Radius, const float Angle) {
	return cVec2(Radius * cMath::Cos(Angle), Radius * cMath::Sin(Angle));
}

// cVec2::FromBaryCentric : cVec2(const cVec2 &, const cVec2 &, const cVec2 &, const float, const float)
inline cVec2 cVec2::FromBaryCentric(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, const float u, const float v) {
	return t0 + u * (t1 - t0) + v * (t2 - t0);
}

// cVec2::Abs : const cVec2(const cVec2 &)
inline const cVec2 cVec2::Abs(const cVec2 &u) {
	return cVec2(cMath::Abs(u.x), cMath::Abs(u.y));
}

// cVec2::Angle : float(const cVec2 &, const cVec2 &)
inline float cVec2::Angle(const cVec2 &u, const cVec2 &v) {
	const float d = u.x * v.y - v.x * u.y;
	const float s = d != 0.0f ? cMath::Sign(d) : 1.0f;
	const float D = cVec2::Dot(u, v);
	const float ll = u.Length() * v.Length();
	return s * cMath::Deg(cMath::ACos(D / ll));
}

// cVec2::AreaSigned : float(const cVec2 &, const cVec2 &, const cVec2 &)
inline float cVec2::AreaSigned(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2) {
	const cVec2 e1 = t1 - t0;
	const cVec2 e2 = t2 - t0;
	return Ccw(e1, e2) * 0.5f;
}

// cVec2::ToBaryCentric : void(const cVec2 &, const cVec2 &, const cVec2 &, float &, float &) const
inline float cVec2::ToBaryCentric(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, float &u, float &v) const {
	const float A = AreaSigned(t0, t1, t2);
	if(A != 0) {
		const float iA = 1.0f / A;
		u = AreaSigned(t0, *this, t2) * iA;
		v = AreaSigned(t0, t1, *this) * iA;
	} else {
		u = v = 0.0f;
	}
	return A;
}

// cVec2::IsInsideTri : bool(const cVec2 &, const cVec2 &, const cVec2 &) const
inline bool cVec2::IsInsideTri(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2) const {
	float u, v;
	ToBaryCentric(t0, t1, t2, u, v);
	return u >= 0.0f && v >= 0.0f && 1.0f - u - v >= 0.0f;
}

// cVec2::ToPolar : void(float &, float &) const
inline void cVec2::ToPolar(float &Radius, float &Angle) const {
	Radius = Length();
	if(cMath::IsZero(Radius)) {
		Radius = 0.0f;
	} else {
		if(y >= 0.0f) Angle = cMath::ACos(x / Radius);
		else Angle = - cMath::ACos(x / Radius);
		Angle = cMath::AngleNormalizeTwoPi(Angle);
	}
}

// cVec2::GetDimension : int() const
inline int cVec2::GetDimension() const {
	return 2;
}

// cVec2::ToFloatPtr : const float * () const
inline const float * cVec2::ToFloatPtr() const {
	return (const float *)&x;
}

// cVec2::ToFloatPtr : float * ()
inline float * cVec2::ToFloatPtr() {
	return (float *)&x;
}

// cVec2::Max : const cVec2(const cVec2 &, const cVec2 &)
inline const cVec2 cVec2::Max(const cVec2 &u, const cVec2 &v) {
	cVec2 r;
	r.x = u.x > v.x ? u.x : v.x;
	r.y = u.y > v.y ? u.y : v.y;
	return r;
}

// cVec2::Min : const cVec2(const cVec2 &, const cVec2 &)
inline const cVec2 cVec2::Min(const cVec2 &u, const cVec2 &v) {
	cVec2 r;
	r.x = u.x < v.x ? u.x : v.x;
	r.y = u.y < v.y ? u.y : v.y;
	return r;
}

// cVec2::Ccw : float(const cVec2 &, const cVec2 &)
inline float cVec2::Ccw(const cVec2 &u, const cVec2 &v) {
	return u.x * v.y - u.y * v.x;
}

// cVec2::Clamp : const cVec2(const cVec2 &, const cVec2 &, const cVec2 &)
inline const cVec2 cVec2::Clamp(const cVec2 &u, const cVec2 &brMin, const cVec2 &brMax) {
	cVec2 r;
	r.x = u.x < brMin.x ? brMin.x : (u.x > brMax.x ? brMax.x : u.x);
	r.y = u.y < brMin.y ? brMin.y : (u.y > brMax.y ? brMax.y : u.y);
	return r;
}

// cVec2::Distance : float(const cVec2 &, const cVec2 &)
inline float cVec2::Distance(const cVec2 &p0, const cVec2 &p1) {
	float dx = p0.x - p1.x;
	float dy = p0.y - p1.y;
	return cMath::Sqrt(dx * dx + dy * dy);
}

// cVec2::DistanceSq : float(const cVec2 &, const cVec2 &)
inline float cVec2::DistanceSq(const cVec2 &p0, const cVec2 &p1) {
	float dx = p0.x - p1.x;
	float dy = p0.y - p1.y;
	return dx * dx + dy * dy;
}

// cVec2::Dot : float(const cVec2 &, const cVec2 &)
inline float cVec2::Dot(const cVec2 &u, const cVec2 &v) {
	return u.x * v.x + u.y * v.y;
}

// cVec2::Length : float(const cVec2 &)
inline float cVec2::Length(const cVec2 &u) {
	return cMath::Sqrt(u.x * u.x + u.y * u.y);
}

// cVec2::LengthSq : float(const cVec2 &)
inline float cVec2::LengthSq(const cVec2 &u) {
	return u.x * u.x + u.y * u.y;
}

// cVec2::Lerp : const cVec2(const cVec2 &, const cVec2 &, const float)
inline const cVec2 cVec2::Lerp(const cVec2 &u, const cVec2 &v, const float s) {
	cVec2 r;
	r.x = u.x + s * (v.x - u.x);
	r.y = u.y + s * (v.y - u.y);
	return r;
}

// cVec2::Lerp05 : const cVec2(const cVec2 &, const cVec2 &)
inline const cVec2 cVec2::Lerp05(const cVec2 &u, const cVec2 &v) {
	cVec2 r;
	r.x = (u.x + v.x) * 0.5f;
	r.y = (u.y + v.y) * 0.5f;
	return r;
}

// cVec2::PerpCw : const cVec2(const cVec2 &)
inline const cVec2 cVec2::PerpCw(const cVec2 &u) {
	return cVec2(u.y, -u.x);
}

// cVec2::PerpCcw : const cVec2(const cVec2 &)
inline const cVec2 cVec2::PerpCcw(const cVec2 &u) {
	return cVec2(-u.y, u.x);
}

// cVec2::ToNormal : const cVec2 () const
inline const cVec2 cVec2::ToNormal() const {
	cVec2 u(*this);
	u.Normalize();
	return u;
}