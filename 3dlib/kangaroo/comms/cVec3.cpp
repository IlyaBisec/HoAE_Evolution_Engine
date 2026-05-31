#include "comms.h"

const cVec3 cVec3::Zero(0.0f);
const cVec3 cVec3::Infinity(cMath::Infinity);
const cVec3 cVec3::Unit(1.0f);
const cVec3 cVec3::xAxis(1.0f, 0.0f, 0.0f);
const cVec3 cVec3::yAxis(0, 1.0f, 0.0f);
const cVec3 cVec3::zAxis(0.0f, 0.0f, 1.0f);

// cVec3::ToStr : cStr(int) const
cStr cVec3::ToStr(int nPrec) const {
	return cStr::FromArray(ToPtr(), GetDimension(), nPrec);
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
//--------------------------------------------------------------------------------------------------
// cVec3::ProjectToLineSeg
//--------------------------------------------------------------------------------------------------
const cVec3 cVec3::ProjectToLineSeg(const cVec3 &p, const cVec3 &A, const cVec3 &B, float *pScale) {
	const cVec3 Ap = p - A;
	const cVec3 AB = B - A;
	cVec3 u = cVec3::Normalize(AB);
	float l = AB.Length();
	
	float pr = cVec3::Dot(Ap, u);

	if(pScale) {
		*pScale = pr / l;
	}
	
	if(pr <= 0) {
		return A;
	}
	if(pr >= l) {
		return B;
	}
	
	return A + pr * u;
}

//-------------------------------------------------------------------------------------
// cVec3::DistanceToLineSegSq
//-------------------------------------------------------------------------------------
float cVec3::DistanceToLineSegSq(const cVec3 &A, const cVec3 &B, float *pScale) const {
	const cVec3 Ap = *this - A;
	const cVec3 AB = B - A;
	const float l2 = AB.LengthSq();
	float Scale;
	if(l2 < cMath::Epsilon) {
		Scale = 0.5f;
	} else {
		Scale = cVec3::Dot(Ap, AB) / l2;
	}
	if(pScale) {
		*pScale = Scale;
	}
	if(Scale <= 0.0f) {
		return cVec3::DistanceSq(*this, A);
	}
	if(Scale >= 1.0f) {
		return cVec3::DistanceSq(*this, B);
	}
	return cVec3::DistanceSq(*this, A + AB * Scale);
}

//-----------------------------------------------------------------------------------
// cVec3::DistanceToLineSeg
//-----------------------------------------------------------------------------------
float cVec3::DistanceToLineSeg(const cVec3 &A, const cVec3 &B, float *pScale) const {
	return cMath::Sqrt(DistanceToLineSegSq(A, B, pScale));
}

//-----------------------------------------------------------------------------------------------------------------
// cVec3::IsInsideConvexPolygon : bool(const cVec3 &, const cVec3 *, const int, const float)
//-----------------------------------------------------------------------------------------------------------------
bool cVec3::IsInsideConvexPolygon(const cVec3 &p, const cVec3 *pPoly, const int NVerts, const float Eps) {
	float Angle = 0.0f;
	for(int i = 0; i < NVerts; i++) {
		cVec3 u = pPoly[i] -  p;
		cVec3 v = pPoly[(i + 1) % NVerts] - p;
		Angle += cVec3::Angle(u, v);
	}
	
	if(Angle >= (1.0f - Eps) * cMath::TwoPi) {
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
// cVec3::IsInsideTri : bool(const cVec3 &, const cVec3 &, const cVec3 &) const
//--------------------------------------------------------------------------------------------
bool cVec3::IsInsideTri(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2) const {
	cVec3 e1 = t1 - t0;
	cVec3 e2 = t2 - t0;

	float a = cVec3::Dot(e1, e1);
	float b = cVec3::Dot(e1, e2);
	float c = cVec3::Dot(e2, e2);

	cVec3 u = *this - t0;
	float d = cVec3::Dot(u, e1);
	float e = cVec3::Dot(u, e2);
	float x = d * c - e * b;
	float y = e * a - d * b;
	float z = x + y - (a * c - b * b);

#define in(a) ((unsigned int &)a)
	return bool((in(z) & ~(in(x) | in(y))) & 0x80000000);
#undef in
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