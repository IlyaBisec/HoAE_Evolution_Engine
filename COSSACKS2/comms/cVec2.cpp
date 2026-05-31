#include "comms.h"

const cVec2 cVec2::Zero(0.0f);
const cVec2 cVec2::One(1.0f, 1.0f);
const cVec2 cVec2::Infinity(cMath::Infinity);
const cVec2 cVec2::AxisX(1.0f, 0.0f);
const cVec2 cVec2::AxisY(0.0f, 1.0f);
const cVec2 cVec2::AxisNegX(-1.0f, 0.0f);
const cVec2 cVec2::AxisNegY(0.0f, -1.0f);

// cVec2::ToStr : cStr(int) const
const cStr cVec2::ToStr(int nPrec) const {
	return cStr::FromFloatArray(ToFloatPtr(), GetDimension(), nPrec);
}

// operator << : cStr & (cStr &, const cVec2 &)
cStr & operator << (cStr &Str, const cVec2 &u) {
	Str += u.ToStr();
	return Str;
}


//----------------------------------------------------------------------------------------------
// cVec2::DistanceToLineSegSq : float(const cVec2 &, const cVec2 &, float *)
//----------------------------------------------------------------------------------------------
float cVec2::DistanceToLineSegSq(const cVec2 &A, const cVec2 &B, float *pScale) const {
	const cVec2 Ap = *this - A;
	const cVec2 AB = B - A;
	const float l2 = AB.LengthSq();
	float Scale;
	if(l2 < cMath::Epsilon) {
		Scale = 0.5f;
	} else {
		Scale = cVec2::Dot(Ap, AB) / l2;
	}
	if(pScale) {
		*pScale = Scale;
	}
	if(Scale <= 0.0f) {
		return cVec2::DistanceSq(*this, A);
	}
	if(Scale >= 1.0f) {
		return cVec2::DistanceSq(*this, B);
	}
	return cVec2::DistanceSq(*this, A + AB * Scale);
}