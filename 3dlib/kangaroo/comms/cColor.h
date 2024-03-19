#pragma once

class cColor {
public:
	float r;
	float g;
	float b;
	float a;

	cColor();
	explicit cColor(const float GrayLevel, const float A = 1.0f);
	cColor(const float R, const float G, const float B, const float A = 1.0f);

	cColor & Set(const float GrayLevel, const float A = 1.0f);
	cColor & Set(const float R, const float G, const float B, float A = 1.0f);

	bool Equals(const cColor &) const;
	bool operator == (const cColor &) const;
	bool operator != (const cColor &) const;

	// Negative:
	cColor operator - () const; 

	// Scale:
	friend cColor operator * (float, const cColor &);
	cColor operator * (float) const;
	cColor & operator *= (float);
	cColor operator / (float) const;
	cColor & operator /= (float);

	// Add:
	cColor operator + (const cColor &) const;
	cColor & operator += (const cColor &);

	// Subtract:
	cColor operator - (const cColor &) const;
	cColor & operator -= (const cColor &);

	// Modulate:
	cColor operator * (const cColor &) const;
	cColor & operator *= (const cColor &);

	static cColor Lerp(const cColor &, const cColor &, const float);
	static cColor Lerp05(const cColor &, const cColor &);
	static cColor ClampTo01(const cColor &);
	static cColor GrayScale(const cColor &);
	static cColor AdjustSaturation(const cColor &, float);
	static cColor AdjustContrast(const cColor &, float);
	static DWORD Pack(float f);
	static cColor FromArgb(DWORD);
	static cColor FromAbgr(DWORD);

	const float * ToPtr() const;
	float * ToPtr();
	DWORD ToArgb() const;
	DWORD ToAbgr() const;
	cStr ToWebColor() const;
	
	static const cColor Black;
	static const cColor Grey75;
	static const cColor Grey50;
	static const cColor Grey25;
	static const cColor White;
	static const cColor Red;
	static const cColor Green;
	static const cColor Blue;
	static const cColor Cyan;
	static const cColor Magenta;
	static const cColor Yellow;
	static const cColor Orange;
	static const cColor Purple;
	static const cColor Pink;
	static const cColor Brown;
};

// cColor.ctor : ()
inline cColor::cColor() {
}

// cColor.ctor : (const float, const float)
inline cColor::cColor(const float GrayLevel, const float A)
: r(GrayLevel), g(GrayLevel), b(GrayLevel), a(A) {}

// cColor.ctor : (const float, const float, const float, const float)
inline cColor::cColor(const float R, const float G, const float B, const float A)
: r(R), g(G), b(B), a(A) {}

// cColor::Set : cColor & (const float, const float)
inline cColor & cColor::Set(const float GrayLevel, const float A) {
	r = g = b = GrayLevel;
	a = A;
	return *this;
}

// cColor::Set : cColor & (const float, const float, const float, const float)
inline cColor & cColor::Set(const float R, const float G, const float B, const float A) {
	r = R;
	g = G;
	b = B;
	a = A;
	return *this;
}

// cColor::Equals : bool(const cColor &) const
inline bool cColor::Equals(const cColor &Src) const {
	return Src.r == r && Src.g == g && Src.b == b && Src.a == a;
}

// cColor::operator == : bool(const cColor &) const
inline bool cColor::operator == (const cColor &Src) const {
	return Equals(Src);
}

// cColor::operator != : bool(const cColor &) const
inline bool cColor::operator != (const cColor &Src) const {
	return !Equals(Src);
}

// cColor::operator - : cColor() const
inline cColor cColor::operator - () const {
	return cColor(1.0f - r, 1.0f - g, 1.0f - b, a);
}

// operator * : cColor(float, const cColor &)
inline cColor operator * (float f, const cColor &c) {
	return cColor(f * c.r, f * c.g, f * c.b, f * c.a);
}

// cColor::operator * : cColor(float) const
inline cColor cColor::operator * (float f) const {
	return cColor(r * f, g * f, b * f, a * f);
}

// cColor::operator *= : cColor & (float)
inline cColor & cColor::operator *= (float f) {
	r *= f;
	g *= f;
	b *= f;
	a *= f;
	return *this;
}

// cColor::operator / : cColor(float) const
inline cColor cColor::operator / (float f) const {
	float i = 1.0f / f;
	return cColor(r * i, g * i, b * i, a * i);
}

// cColor::operator /= : cColor & (float)
inline cColor & cColor::operator /= (float f) {
	float i = 1.0f / f;
	r *= i;
	g *= i;
	b *= i;
	a *= a;
	return *this;
}

// cColor::operator + : cColor(const cColor &) const
inline cColor cColor::operator + (const cColor &c) const {
	return cColor(r + c.r, g + c.g, b + c.b, a + c.a);
}

// cColor::operator += : cColor & (const cColor &)
inline cColor & cColor::operator += (const cColor &c) {
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	return *this;
}

// cColor::operator - : cColor(const cColor &) const
inline cColor cColor::operator - (const cColor &c) const {
	return cColor(r - c.r, g - c.g, b - c.b, a - c.a);
}

// cColor::operator -= : cColor & (const cColor &)
inline cColor & cColor::operator -= (const cColor &c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;
	return *this;
}

// cColor::operator * : cColor(const cColor &) const
inline cColor cColor::operator * (const cColor &c) const {
	return cColor(r * c.r, g * c.g, b * c.b, a * c.a);
}

// cColor::operator *= : cColor & (const cColor &)
inline cColor & cColor::operator *= (const cColor &c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;
	a *= c.a;
	return *this;
}

// cColor::ToPtr : const float * () const
inline const float * cColor::ToPtr() const {
	return &r;
}

// cColor::ToPtr : float * ()
inline float * cColor::ToPtr() {
	return &r;
}

// cColor::Pack : DWORD(float)
inline DWORD cColor::Pack(float f) {
	return f >= 1.0f ? 0xff : f <= 0.0f ? 0x00 : (unsigned char)(f * 255.0f + 0.5f);
}

// cColor::ToArgb : DWORD() const
inline DWORD cColor::ToArgb() const {
	return (Pack(a) << 24) | (Pack(r) << 16) | (Pack(g) << 8) | (Pack(b) << 0);
}

// cColor::FromArgb : cColor(DWORD)
inline cColor cColor::FromArgb(DWORD Color) {
	const float f = 1.0f / 255.0f;
	cColor C;
	C.a = f * (float)(unsigned char)(Color >> 24);
	C.r = f * (float)(unsigned char)(Color >> 16);
	C.g = f * (float)(unsigned char)(Color >> 8);
	C.b = f * (float)(unsigned char)(Color >> 0);
	return C;
}

// cColor::ToAbgr : DWORD() const
inline DWORD cColor::ToAbgr() const {
	return (Pack(r) << 0) | (Pack(g) << 8) | (Pack(b) << 16) | (Pack(a) << 24);
}

// cColor::FromAbgr : cColor(DWORD)
inline cColor cColor::FromAbgr(DWORD Color) {
	const float f = 1.0f / 255.0f;
	cColor C;
	C.a = f * (float)(unsigned char)(Color >> 24);
	C.b = f * (float)(unsigned char)(Color >> 16);
	C.g = f * (float)(unsigned char)(Color >> 8);
	C.r = f * (float)(unsigned char)(Color >> 0);
	return C;
}

// cColor::Lerp : cColor(const cColor &, const cColor &, const float)
inline cColor cColor::Lerp(const cColor &c1, const cColor &c2, const float s) {
	cColor c;
	c.r = c1.r + s * (c2.r - c1.r);
	c.g = c1.g + s * (c2.g - c1.g);
	c.b = c1.b + s * (c2.b - c1.b);
	c.a = c1.a + s * (c2.a - c1.a);
	return c;
}

// cColor::Lerp05 : cColor(const cColor &, const cColor &)
inline cColor cColor::Lerp05(const cColor &c0, const cColor &c1) {
	cColor c;
	c.r = 0.5f * (c0.r + c1.r);
	c.g = 0.5f * (c0.g + c1.g);
	c.b = 0.5f * (c0.b + c1.b);
	c.a = 0.5f * (c0.a + c1.a);
	return c;
}