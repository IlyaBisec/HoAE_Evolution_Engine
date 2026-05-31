#include "comms.h"

const cColor cColor::Zero		= cColor(0.0f, 0.0f, 0.0f, 0.0f);
const cColor cColor::Black		= cColor(0.00f, 0.00f, 0.00f, 1.00f);
const cColor cColor::Gray75		= cColor(0.75f, 0.75f, 0.75f, 1.00f);
const cColor cColor::Gray50		= cColor(0.50f, 0.50f, 0.50f, 1.00f);
const cColor cColor::Gray25		= cColor(0.25f, 0.25f, 0.25f, 1.00f);
const cColor cColor::White		= cColor(1.00f, 1.00f, 1.00f, 1.00f);
const cColor cColor::Red		= cColor(1.00f, 0.00f, 0.00f, 1.00f);
const cColor cColor::Green		= cColor(0.00f, 1.00f, 0.00f, 1.00f);
const cColor cColor::Blue		= cColor(0.00f, 0.00f, 1.00f, 1.00f);
const cColor cColor::Cyan		= cColor(0.00f, 1.00f, 1.00f, 1.00f);
const cColor cColor::Magenta	= cColor(1.00f, 0.00f, 1.00f, 1.00f);
const cColor cColor::Yellow		= cColor(1.00f, 1.00f, 0.00f, 1.00f);
const cColor cColor::Orange		= cColor(1.00f, 0.50f, 0.00f, 1.00f);
const cColor cColor::Pink		= cColor(0.73f, 0.40f, 0.48f, 1.00f);
const cColor cColor::Brown		= cColor(0.40f, 0.35f, 0.08f, 1.00f);
const cColor cColor::Purple		= cColor(0.60f, 0.00f, 0.60f, 1.00f);

// cColor::GrayScale : cColor(const cColor &)
cColor cColor::GrayScale(const cColor &Src) {
	float Gray = Src.r * 0.2125f + Src.g * 0.7154f + Src.b * 0.0721f;
	return cColor(Gray, Gray, Gray, Src.a);
}

// cColor::AdjustSaturation : cColor(const cColor &, float)
cColor cColor::AdjustSaturation(const cColor &Src, float s) {
	const float Gray = Src.ToLuminance();
	cColor C;
	C.r = cMath::Lerp(Gray, Src.r, s);
	C.g = cMath::Lerp(Gray, Src.g, s);
	C.b = cMath::Lerp(Gray, Src.b, s);
	C.a = Src.a;
	return C;
}

// cColor::AdjustContrast : cColor(const cColor &, float)
cColor cColor::AdjustContrast(const cColor &Src, float c) {
	cColor C;
	C.r = cMath::Lerp(0.5f, Src.r, c);
	C.g = cMath::Lerp(0.5f, Src.g, c);
	C.b = cMath::Lerp(0.5f, Src.b, c);
	C.a = Src.a;
	return C;
}

//-----------------------------------------------------------------------------
// cColor::ToWebColor
//-----------------------------------------------------------------------------
const cStr cColor::ToWebColor() const {
	cStr str("#");
	DWORD dw = ToBgra();
	for(int i = 5; i >= 0; i--) {
		str << cStr::Format("%x", (dw >> 4 * i) & 0xf);
	}
	return str;
} // cColor::ToWebColor

//-----------------------------------------------------------------------------
// cColor::Clamp01 : cColor(const cColor &)
//-----------------------------------------------------------------------------
cColor cColor::Clamp01(const cColor &c) {
	float r = cMath::Clamp01(c.r);
	float g = cMath::Clamp01(c.g);
	float b = cMath::Clamp01(c.b);
	float a = cMath::Clamp01(c.a);
	return cColor(r, g, b, a);
}