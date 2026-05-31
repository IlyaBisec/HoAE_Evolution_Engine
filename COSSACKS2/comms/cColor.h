#pragma once

class cColor {
public:
	float r;
	float g;
	float b;
	float a;


	cColor();
	cColor(const float R, const float G, const float B, const float A = 1.0f);

	enum eRgbaCtor { RgbaCtor };
	enum eBgraCtor { BgraCtor };

	cColor(const eRgbaCtor, DWORD Rgba);
	cColor(const eRgbaCtor, int Red, int Green, int Blue, int Alpha = 255); // w/o clamping
	cColor(const eBgraCtor, DWORD Bgra);

	explicit cColor(const float GrayLevel, const float A = 1.0f);

	void Set(const float GrayLevel, const float A = 1.0f);
	void Set(const float R, const float G, const float B, float A = 1.0f);
	void SetRgba(DWORD Rgba);
	void SetRgba(int Red, int Green, int Blue, int Alpha = 255);
	void SetBgra(DWORD Bgra);

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
	static cColor Clamp01(const cColor &);
	static cColor GrayScale(const cColor &);
	static cColor AdjustSaturation(const cColor &, float);
	static cColor AdjustContrast(const cColor &, float);

	static const cColor FromRgba(DWORD Rgba);
	static const cColor FromRgba(int Red, int Green, int Blue, int Alpha = 255);
	static const cColor FromBgra(DWORD Bgra);


	static DWORD Pack(float f);
	DWORD _ToBgra() const;
	
	const float * ToFloatPtr() const;
	float * ToFloatPtr();
	DWORD ToRgba() const;
	DWORD ToBgra() const;
	const cStr ToWebColor() const;
	float ToLuminance() const;

	static const cColor Zero;
	static const cColor Black;
	static const cColor Gray75;
	static const cColor Gray50;
	static const cColor Gray25;
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

//-----------------------------------------------------------------------------
// cColor.ctor
//-----------------------------------------------------------------------------
inline cColor::cColor(const eRgbaCtor, DWORD Rgba) {
	SetRgba(Rgba);
} // cColor.ctor


//-----------------------------------------------------------------------------
// cColor::SetRgba
//-----------------------------------------------------------------------------
inline void cColor::SetRgba(DWORD Rgba) {
	const float f = 1.0f / 255.0f;
	r = f * (float)(byte)Rgba;
	g = f * (float)(byte)(Rgba >> 8);
	b = f * (float)(byte)(Rgba >> 16);
	a = f * (float)(byte)(Rgba >> 24);
} // cColor::SetRgba

//-----------------------------------------------------------------------------
// cColor::SetRgba
//-----------------------------------------------------------------------------
inline void cColor::SetRgba(int Red, int Green, int Blue, int Alpha) {
	const float f = 1.0f / 255.0f;
	r = f * (float)Red;
	g = f * (float)Green;
	b = f * (float)Blue;
	a = f * (float)Alpha;
}

//-------------------------------------------------------------------------------
// cColor.ctor
//-------------------------------------------------------------------------------
inline cColor::cColor(const eRgbaCtor, int Red, int Green, int Blue, int Alpha) {
	SetRgba(Red, Green, Blue, Alpha);
} // cColor.ctor

//-----------------------------------------------------------------------------
// cColor::SetBgra
//-----------------------------------------------------------------------------
inline void cColor::SetBgra(DWORD Bgra) {
	const float f = 1.0f / 255.0f;
	b = f * (float)(byte)Bgra;
	g = f * (float)(byte)(Bgra >> 8);
	r = f * (float)(byte)(Bgra >> 16);
	a = f * (float)(byte)(Bgra >> 24);
} // cColor::SetBgra

//-----------------------------------------------------------------------------
// cColor.ctor
//-----------------------------------------------------------------------------
inline cColor::cColor(const eBgraCtor, DWORD Bgra) {
	SetBgra(Bgra);
} // cColor.ctor

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
inline void cColor::Set(const float GrayLevel, const float A) {
	r = g = b = GrayLevel;
	a = A;
}

// cColor::Set : cColor & (const float, const float, const float, const float)
inline void cColor::Set(const float R, const float G, const float B, const float A) {
	r = R;
	g = G;
	b = B;
	a = A;
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

// cColor::ToFloatPtr : const float * () const
inline const float * cColor::ToFloatPtr() const {
	return &r;
}

// cColor::ToFloatPtr : float * ()
inline float * cColor::ToFloatPtr() {
	return &r;
}

// cColor::FromRgba
inline const cColor cColor::FromRgba(DWORD Rgba) {
	return cColor(cColor::RgbaCtor, Rgba);
}

// cColor::FromRgba
inline const cColor cColor::FromRgba(int Red, int Green, int Blue, int Alpha) {
	return cColor(RgbaCtor, Red, Green, Blue, Alpha);
}

// cColor::FromBgra
inline const cColor cColor::FromBgra(DWORD Bgra) {
	return cColor(BgraCtor, Bgra);
}

//-----------------------------------------------------------------------------
// cColor::ToRgba
//-----------------------------------------------------------------------------
inline DWORD cColor::ToRgba() const {
	DWORD R = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (DWORD)(r * 255.0f + 0.5f);
	DWORD G = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (DWORD)(g * 255.0f + 0.5f);
	DWORD B = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (DWORD)(b * 255.0f + 0.5f);
	DWORD A = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (DWORD)(a * 255.0f + 0.5f);
	return R | G << 8 | B << 16 | A << 24;
} // cColor::ToRgba

//-----------------------------------------------------------------------------
// cColor::ToBgra
//-----------------------------------------------------------------------------
inline DWORD cColor::ToBgra() const {
	DWORD B = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (DWORD)(b * 255.0f + 0.5f);
	DWORD G = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (DWORD)(g * 255.0f + 0.5f);
	DWORD R = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (DWORD)(r * 255.0f + 0.5f);
	DWORD A = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (DWORD)(a * 255.0f + 0.5f);
	return B | G << 8 | R << 16 | A << 24;
} // cColor::ToBgra




// cColor::Pack : DWORD(float)
inline DWORD cColor::Pack(float f) {
	return f >= 1.0f ? 0xff : f <= 0.0f ? 0x00 : (DWORD)(f * 255.0f + 0.5f);
}

// cColor::ToBgra
inline DWORD cColor::_ToBgra() const {
	return Pack(b) | Pack(g) << 8 | Pack(r) << 16 | Pack(a) << 24;
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

// cColor::ToLuminance
inline float cColor::ToLuminance() const {
	return r * 0.2125f + g * 0.7154f + 0.0721f;
}