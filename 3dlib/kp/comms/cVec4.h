#pragma once

class cVec4 {
public:
	float x;
	float y;
	float z;
	float w;

	cVec4();
	explicit cVec4(const float S);
	cVec4(const float X, const float Y, const float Z, const float W);
	cVec4(const cVec3 &XYZ, const float W);

	cVec4 & Set(const float X, const float Y, const float Z, const float W);
	cVec4 & Set(const cVec3 &XYZ, const float W);
	cVec4 & Copy(const float *pSrc);

	float & operator [] (const int index);
	float operator [] (const int index) const;

	cVec4 operator - () const;

	cVec4 & operator += (const cVec4 &);
	cVec4 & operator -= (const cVec4 &);
	cVec4 & operator *= (const float);
	cVec4 & operator /= (const float);

	cVec4 & Transform(const cMat4 &);
	cVec4 & operator *= (const cMat4 &);

	cVec4 operator + (const cVec4 &) const;
	cVec4 operator - (const cVec4 &) const;
	cVec4 operator * (const float) const;
	friend cVec4 operator * (const float, const cVec4 &);
	cVec4 operator / (const float) const;

	float operator * (const cVec4 &) const;

	static cVec4 Transform(const cVec4 &, const cMat4 &);
	cVec4 operator * (const cMat4 &) const;

	bool operator == (const cVec4 &) const;
	bool operator != (const cVec4 &) const;
	bool Equals(const cVec4 &, const float Epsilon = 0.001f) const;

	float Length() const;
	float LengthSq() const;
	float Normalize();

	static float Dot(const cVec4 &, const cVec4 &);
//	static cVec4 BaryCentric(const cVec4 &, const cVec4 &, const cVec4 &, const float, const float);
//	static cVec4 CatmullRom(const cVec4 &, const cVec4 &, const cVec4 &, const cVec4 &, const float);
//	static cVec4 Clamp(const cVec4 &, const cVec4 &, const cVec4 &);
//	static cVec4 Cross(const cVec4 &, const cVec4 &, const cVec4 &);
//	static cVec4 Hermite(const cVec4 &, const cVec4 &, const cVec4 &, const cVec4 &, const float);
	static float Length(const cVec4 &);
	static float LengthSq(const cVec4 &);
	static cVec4 Lerp(const cVec4 &, const cVec4 &, const float);
	static cVec4 Max(const cVec4 &, const cVec4 &);
	static cVec4 Min(const cVec4 &, const cVec4 &);
	static cVec4 Normalize(const cVec4 &);

	static const cVec4 Zero;
	static const cVec4 Unit;
	static const cVec4 xAxis;
	static const cVec4 yAxis;
	static const cVec4 zAxis;
	static const cVec4 wAxis;

	float * ToPtr();
	const float * ToPtr() const;
	cVec2 & To2();
	const cVec2 & To2() const;
	cVec3 & To3();
	const cVec3 & To3() const;
//	cStr ToStr() const;
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

// cVec4.ctor : (const cVec3 &, const float)
inline cVec4::cVec4(const cVec3 &XYZ, const float W)
: x(XYZ.x), y(XYZ.y), z(XYZ.z), w(W) {}

// cVec4::Set : cVec4 & (const float, const float, const float, const float)
inline cVec4 & cVec4::Set(const float X, const float Y, const float Z, const float W) {
	x = X;
	y = Y;
	z = Z;
	w = W;
	return *this;
}

// cVec4::Set : cVec4 & (const cVec3 &, const float)
inline cVec4 & cVec4::Set(const cVec3 &XYZ, const float W) {
	x = XYZ.x;
	y = XYZ.y;
	z = XYZ.z;
	w = W;
	return *this;
}

// cVec4::Copy : cVec4 & (const float *)
inline cVec4 & cVec4::Copy(const float *pSrc) {
	assert(pSrc);
	x = pSrc[0];
	y = pSrc[1];
	z = pSrc[2];
	w = pSrc[3];
	return *this;
}

// cVec4::operator [] : float & (const int)
inline float & cVec4::operator [] (const int index) {
	assert(index >= 0 && index < 4);
	return (&x)[index];
}

// cVec4::operator [] : float(const int) const
inline float cVec4::operator [] (const int index) const {
	assert(index >= 0 && index < 4);
	return (&x)[index];
}

// cVec4::operator - : cVec4() const
inline cVec4 cVec4::operator - () const {
	return cVec4(-x, -y, -z, -w);
}

// cVec4::operator += : cVec4 & (const cVec4 &)
inline cVec4 & cVec4::operator += (const cVec4 &u) {
	x += u.x;
	y += u.y;
	z += u.z;
	w += u.w;
	return *this;
}

// cVec4::operator -= : cVec4 & (const cVec4 &)
inline cVec4 & cVec4::operator -= (const cVec4 &u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
	w -= u.w;
	return *this;
}

// cVec4::operator *= : cVec4 & (const float)
inline cVec4 & cVec4::operator *= (const float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

// cVec4::operator /= : cVec4 & (const float)
inline cVec4 & cVec4::operator /= (const float s) {
	float is = 1.0f / s;
	x *= is;
	y *= is;
	z *= is;
	w *= is;
	return *this;
}

// cVec4::operator + : cVec4(const cVec4 &) const
inline cVec4 cVec4::operator + (const cVec4 &u) const {
	return cVec4(x + u.x, y + u.y, z + u.z, w + u.w);
}

// cVec4::operator - : cVec4(const cVec4 &) const
inline cVec4 cVec4::operator - (const cVec4 &u) const {
	return cVec4(x - u.x, y - u.y, z - u.z, w - u.w);
}

// cVec4::operator * : cVec4(const float) const
inline cVec4 cVec4::operator * (const float s) const {
	return cVec4(x * s, y * s, z * s, w * s);
}

// cVec4::friend operator * : cVec4(const float, const cVec4 &)
inline cVec4 operator * (const float s, const cVec4 &u) {
	return cVec4(s * u.x, s * u.y, s * u.z, s * u.w);
}

// cVec4::operator / : cVec4(const float) const
inline cVec4 cVec4::operator / (const float s) const {
	float is = 1.0f / s;
	return cVec4(x * is, y * is, z * is, w * is);
}

// cVec4::operator * : float(const cVec4 &) const
inline float cVec4::operator * (const cVec4 &u) const {
	return x * u.x + y * u.y + z * u.z + w * u.w;
}

// cVec4::operator == : bool(const cVec4 &) const
inline bool cVec4::operator == (const cVec4 &u) const {
	return x == u.x && y == u.y && z == u.z && w == u.w;
}

// cVec4::operator != : bool(const cVec4 &) const
inline bool cVec4::operator != (const cVec4 &u) const {
	return x != u.x || y != u.y || z != u.z || w != u.w;
}

// cVec4::Equals : bool(const cVec4 &, const float) const
inline bool cVec4::Equals(const cVec4 &u, const float Epsilon) const {
	if(fabs(x - u.x) > Epsilon) return false;
	if(fabs(y - u.y) > Epsilon) return false;
	if(fabs(z - u.z) > Epsilon) return false;
	if(fabs(w - u.w) > Epsilon) return false;
	return true;
}

// cVec4::Length : float() const
inline float cVec4::Length() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

// cVec4::LengthSq : float() const
inline float cVec4::LengthSq() const {
	return x * x + y * y + z * z + w * w;
}

// cVec4::Normalize : float()
inline float cVec4::Normalize() {
	float l = sqrtf(x * x + y * y + z * z + w * w);
	if(l > 0) {
		float il = 1.0f / l;
		x *= il;
		y *= il;
		z *= il;
		w *= il;
	}
	return l;
}

// cVec4::Dot : float(const cVec4 &, const cVec4 &)
inline float cVec4::Dot(const cVec4 &u, const cVec4 &v) {
	return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

// cVec4::Length : float(const cVec4 &)
inline float cVec4::Length(const cVec4 &u) {
	return sqrtf(u.x * u.x + u.y * u.y + u.z * u.z + u.w * u.w);
}

// cVec4::LengthSq : float(const cVec4 &)
inline float cVec4::LengthSq(const cVec4 &u) {
	return u.x * u.x + u.y * u.y + u.z * u.z + u.w * u.w;
}

// cVec4::Lerp : cVec4(const cVec4 &, const cVec4 &, const float)
inline cVec4 cVec4::Lerp(const cVec4 &u, const cVec4 &v, const float s) {
	cVec4 r;
	r.x = u.x + s * (v.x - u.x);
	r.y = u.y + s * (v.y - u.y);
	r.z = u.z + s * (v.z - u.z);
	r.w = u.w + s * (v.w - u.w);
	return r;
}

// cVec4::Max : cVec4(const cVec4 &, const cVec4 &)
inline cVec4 cVec4::Max(const cVec4 &u, const cVec4 &v) {
	cVec4 r;
	r.x = u.x > v.x ? u.x : v.x;
	r.y = u.y > v.y ? u.y : v.y;
	r.z = u.z > v.z ? u.z : v.z;
	r.w = u.w > v.w ? u.w : v.w;
	return r;
}

// cVec4::Min : cVec4(const cVec4 &, const cVec4 &)
inline cVec4 cVec4::Min(const cVec4 &u, const cVec4 &v) {
	cVec4 r;
	r.x = u.x < v.x ? u.x : v.x;
	r.y = u.y < v.y ? u.y : v.y;
	r.z = u.z < v.z ? u.z : v.z;
	r.w = u.w < v.w ? u.w : v.w;
	return r;
}

// cVec4::Normalize : cVec4(const cVec4 &)
inline cVec4 cVec4::Normalize(const cVec4 &u) {
	float l = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z + u.w * u.w);
	if(l > 0) {
		float il = 1.0f / l;
		return cVec4(u.x * il, u.y * il, u.z * il, u.w * il);
	}
	return u;
}

// cVec4::ToPtr : float * ()
inline float * cVec4::ToPtr() {
	return (float *)&x;
}

// cVec4::ToPtr : const float * () const
inline const float * cVec4::ToPtr() const {
	return (const float *)&x;
}

// cVec4::To2 : cVec2 & ()
inline cVec2 & cVec4::To2() {
	return *reinterpret_cast<cVec2 *>(this);
}

// cVec4::To2 : const cVec2 & () const
inline const cVec2 & cVec4::To2() const {
	return *reinterpret_cast<const cVec2 *>(this);
}

// cVec4::To3 : cVec3 & ()
inline cVec3 & cVec4::To3() {
	return *reinterpret_cast<cVec3 *>(this);
}

// cVec4::To3 : const cVec3 & () const
inline const cVec3 & cVec4::To3() const {
	return *reinterpret_cast<const cVec3 *>(this);
}