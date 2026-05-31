#include "comms.h"

const cVec3 cVec3::Zero(0.0f);
const cVec3 cVec3::One(1.0f);
const cVec3 cVec3::Infinity(cMath::Infinity);
const cVec3 cVec3::AxisX(1.0f, 0.0f, 0.0f);
const cVec3 cVec3::AxisY(0.0f, 1.0f, 0.0f);
const cVec3 cVec3::AxisZ(0.0f, 0.0f, 1.0f);
const cVec3 cVec3::AxisNegX(-1.0f, 0.0f, 0.0f);
const cVec3 cVec3::AxisNegY(0.0f, -1.0f, 0.0f);
const cVec3 cVec3::AxisNegZ(0.0f, 0.0f, -1.0f);

// cVec3::ToStr : cStr(int) const
cStr cVec3::ToStr(int nPrec) const {
	return cStr::FromFloatArray(ToFloatPtr(), GetDimension(), nPrec);
}

// operator << : cStr & (cStr &, const cVec3 &)
cStr & operator << (cStr &Str, const cVec3 &u) {
	Str += u.ToStr();
	return Str;
}

//-----------------------------------------------------------------------------
// cVec3::ToAngles : cAngles() const
//-----------------------------------------------------------------------------
cAngles cVec3::ToAngles() const {
	float Pitch, Yaw;
	if(x == 0.0f && y == 0.0f) {
		Yaw = 0.0f;
		if(z > 0.0f) {
			Pitch = 90.0f;
		} else {
			Pitch = 270.0f;
		}
	} else {
		Yaw = cMath::Deg(cMath::ATan(y, x));
		if(Yaw < 0.0f) {
			Yaw += 360.0f;
		}
		float Forward = cMath::Sqrt(x * x + y * y);
		Pitch = cMath::Deg(cMath::ATan(z, Forward));
		if(Pitch < 0.0f) {
			Pitch += 360.0f;
		}
	}
	return cAngles(- Pitch, Yaw, 0.0f);
}

//-----------------------------------------------------------------------------
// cVec3::ToYaw : float() const
//-----------------------------------------------------------------------------
float cVec3::ToYaw() const {
	float Yaw;
	if(x == 0.0f && y == 0.0f) {
		Yaw = 0.0f;
	} else {
		Yaw = cMath::Deg(cMath::ATan(y, x));
		if(Yaw < 0.0f) {
			Yaw += 360.0f;
		}
	}
	return Yaw;
}

//-----------------------------------------------------------------------------
// cVec3::ToPitch : float() const
//-----------------------------------------------------------------------------
float cVec3::ToPitch() const {
	float Pitch;
	if(x == 0.0f && y == 0.0f) {
		if(z > 0.0f) {
			Pitch = 90.0f;
		} else {
			Pitch = 270.0f;
		}
	} else {
		float Forward = cMath::Sqrt(x * x + y * y);
		Pitch = cMath::Deg(cMath::ATan(z, Forward));
		if(Pitch < 0.0f) {
			Pitch += 360.0f;
		}
	}
	return - Pitch;
}


//--------------------------------------------------------------------------------------
// cVec3::FromSpherical : cVec3(const float, const float, const float)
//--------------------------------------------------------------------------------------
cVec3 cVec3::FromSpherical(const float Radius, const float Phi, const float Psi) {
	float x = Radius * cMath::Sin(Phi) * cMath::Sin(Psi);
	float y = Radius * cMath::Cos(Psi);
	float z = Radius * cMath::Cos(Phi) * cMath::Sin(Psi);
	return cVec3(x, y, z);
/*	cVec3 p;
	p.x = Radius * cMath::Sin(Phi) * cMath::Sin(Psi);
	p.y = Radius * cMath::Cos(Phi) * cMath::Sin(Psi);
	p.z = Radius * cMath::Cos(Psi);
	return p;*/
}

//-------------------------------------------------------------------------------
// cVec3::Slerp
//-------------------------------------------------------------------------------
const cVec3 cVec3::Slerp(const cVec3 &n0, const cVec3 &n1, const float s) {
	if(s <= 0.0f) {
		return n0;
	} else if(s >= 1.0f) {
		return n1;
	}
	
	float Cos = cVec3::Dot(n0, n1);
	float Scale0, Scale1;
	if(1.0f - Cos > cMath::Epsilon) {
		float Angle = cMath::ACos(Cos);
		float Sin = cMath::Sin(Angle);
		Scale0 = cMath::Sin((1.0f - s) * Angle) / Sin;
		Scale1 = cMath::Sin(s * Angle) / Sin;
	} else {
		Scale0 = 1.0f - s;
		Scale1 = s;
	}
	return n0 * Scale0 + n1 * Scale1;
}

//-----------------------------------------------------------------------------
// cVec3::ToSpherical : void(float &, float &, float &) const
//-----------------------------------------------------------------------------
void cVec3::ToSpherical(float &Radius, float &Phi, float &Psi) const {
	Radius = Length();
	if(cMath::IsZero(Radius)) {
		Radius = 0.0f;
	} else {
		Psi = cMath::ACos(y / Radius);
		float l = cMath::Sqrt(cMath::Square(x) + cMath::Square(z));
		if(cMath::IsZero(l)) {
			Phi = 0.0f;
		} else {
			if(z >= 0.0f) Phi = cMath::ASin(x / l);
			else Phi = cMath::Pi - cMath::ASin(x / l);
			Phi = cMath::AngleNormalizeTwoPi(Phi);
		}
	}
}

//-----------------------------------------------------------------------------
// cVec3::FixDegenerateNormal
//-----------------------------------------------------------------------------
bool cVec3::FixDegenerateNormal() {
	if(x == 0.0f) {
		if(y == 0.0f) {
			if(z > 0.0f) {
				if(z != 1.0f) {
					z = 1.0f;
					return true;
				}
			} else {
				if(z != -1.0f) {
					z = -1.0f;
					return true;
				}
			}
			return false;
		} else if(z == 0.0f) {
			if(y > 0.0f) {
				if(y != 1.0f) {
					y = 1.0f;
					return true;
				}
			} else {
				if(y != -1.0f) {
					y = -1.0f;
					return true;
				}
			}
			return false;
		}
	} else if(y == 0.0f) {
		if(z == 0.0f) {
			if(x > 0.0f) {
				if(x != 1.0f) {
					x = 1.0f;
					return true;
				}
			} else {
				if(x != -1.0f) {
					x = -1.0f;
					return true;
				}
			}
			return false;
		}
	}
	if(cMath::Abs(x) == 1.0f) {
		if(y != 0.0f || z != 0.0f) {
			y = z = 0.0f;
			return true;
		}
		return false;
	} else if(cMath::Abs(y) == 1.0f) {
		if(x != 0.0f || z != 0.0f) {
			x = z = 0.0f;
			return true;
		}
		return false;
	} else if(cMath::Abs(z) == 1.0f) {
		if(x != 0.0f || y != 0.0f) {
			x = y = 0.0f;
			return true;
		}
		return false;
	}
	return false;
}

//-----------------------------------------------------------------------------
// cVec3::RandRange1
//-----------------------------------------------------------------------------
const cVec3 cVec3::RandRange1() {
	cVec3 r;
	r.x = cMath::RandRange1();
	r.y = cMath::RandRange1();
	r.z = cMath::RandRange1();
	return r;
} // cVec3::RandRange1

//-----------------------------------------------------------------------------
// cVec3::RandNormal
//-----------------------------------------------------------------------------
const cVec3 cVec3::RandNormal() {
	cVec3 r;
	float lSq;
	do {
		r = RandRange1();
		lSq = r.LengthSq();
	} while(lSq > 1.0f || lSq < cMath::Epsilon);
	float il = 1.0f / cMath::Sqrt(lSq);
	r *= il;
	return r;
} // cVec3::RandNormal

//-----------------------------------------------------------------------------
// cVec3::ToPerp
//-----------------------------------------------------------------------------
/*const cVec3 cVec3::ToPerp() const {
	const cVec3 s = Abs(*this);
	cVec3 u;

	if(s.x < s.y && s.x < s.z) {
		u = AxisX;
	} else if(s.y < s.z) {
		u = AxisY;
	} else {
		u = AxisZ;
	}
	return Cross(*this, u);
} // cVec3::ToPerp*/

// cVec3::ToPerps
void cVec3::ToPerps(cVec3 &X, cVec3 &Y) const {
	const cVec2 v(x, z);
	const float Alpha = v.Length() != 0.0f ? cVec2::Angle(v, cVec2::AxisY) : 0.0f;
	const float Beta = cMath::Deg(cMath::ASin(y / Length()));
	const cMat3 S = cMat3::RotationY(-Alpha) * cMat3::RotationX(Beta);
	X = S.GetCol0();
	Y = S.GetCol1();
}

// cVec3::ToPerp
const cVec3 cVec3::ToPerp() const {
	cVec3 X, Y;
	ToPerps(X, Y);
	return X;
}

//-----------------------------------------------------------------------------
// cVec3::FixDenormals
//-----------------------------------------------------------------------------
bool cVec3::FixDenormals() {
	bool IsDenormal = false;
	if(cMath::IsZero(x)) {
		x = 0.0f;
		IsDenormal = true;
	}
	if(cMath::IsZero(y)) {
		y = 0.0f;
		IsDenormal = true;
	}
	if(cMath::IsZero(z)) {
		z = 0.0f;
		IsDenormal = true;
	}
	if(cMath::IsOne(x)) {
		x = cMath::Sign(x);
		IsDenormal = true;
	}
	if(cMath::IsOne(y)) {
		y = cMath::Sign(y);
		IsDenormal = true;
	}
	if(cMath::IsOne(z)) {
		z = cMath::Sign(z);
		IsDenormal = true;
	}
	return IsDenormal;
} // cVec3::FixDenormals