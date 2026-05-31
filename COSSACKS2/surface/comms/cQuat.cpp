#include "comms.h"

const cQuat cQuat::Identity(0.0f, 0.0f, 0.0f, 1.0f);
const cQuat cQuat::Zero(0.0f, 0.0f, 0.0f, 0.0f);

//-----------------------------------------------------------------------------
// cQuat::ToAngles : const cAngles () const
//-----------------------------------------------------------------------------
const cAngles cQuat::ToAngles() const {
	return ToMat3().ToAngles();
}

//-----------------------------------------------------------------------------
// cQuat::ToRotation : const cRotation () const
//-----------------------------------------------------------------------------
const cRotation cQuat::ToRotation() const {
	cVec3 Axis(x, y, z);
	float Angle = cMath::ACos(w);

	if(Angle == 0.0f) {
		Axis = cVec3::zAxis;
	} else {
		Axis.Normalize();
		Axis.FixDegenerateNormal();
		Angle *= 2.0f * cMath::DegsPerRad;
	}
	return cRotation(cVec3::Zero, Axis, Angle);
}

//-----------------------------------------------------------------------------
// cQuat::ToMat3() : const cMat3 () const
//-----------------------------------------------------------------------------
const cMat3 cQuat::ToMat3() const {
	float x2 = x + x;
	float y2 = y + y;
	float z2 = z + z;

	float xx = x * x2;
	float xy = x * y2;
	float xz = x * z2;
	
	float yy = y * y2;
	float yz = y * z2;
	float zz = z * z2;
	
	float wx = w * x2;
	float wy = w * y2;
	float wz = w * z2;

	cMat3 M;
	
	M[0][0] = 1.0f - (yy + zz);
	M[0][1] = xy - wz;
	M[0][2] = xz + wy;
	
	M[1][0] = xy + wz;
	M[1][1] = 1.0f - (xx + zz);
	M[1][2] = yz - wx;
	
	M[2][0] = xz - wy;
	M[2][1] = yz + wx;
	M[2][2] = 1.0f - (xx + yy);
	
	return M;
}

//-----------------------------------------------------------------------------
// cQuat::ToMat4 : const cMat4 () const
//-----------------------------------------------------------------------------
const cMat4 cQuat::ToMat4() const {
	return ToMat3().ToMat4();
}

//-----------------------------------------------------------------------------
// cQuat::ToAngularVelocity : const cVec3 () const
//-----------------------------------------------------------------------------
const cVec3 cQuat::ToAngularVelocity() const {
	return cVec3::Normalize(cVec3(x, y, z)) * cMath::ACos(w);
}

//-----------------------------------------------------------------------------
// cQuat::ToStr : const cStr (const int) const
//-----------------------------------------------------------------------------
const cStr cQuat::ToStr(const int nPrec) const {
	return cStr::FromArray(ToPtr(), GetDimension(), nPrec);
}

//-------------------------------------------------------------------------------------------------
// cQuat::Lerp : const cQuat (const cQuat &, const cQuat &, const float, const bool)
//-------------------------------------------------------------------------------------------------
const cQuat cQuat::Lerp(const cQuat &q0, const cQuat &q1, const float s, const bool ShortestPath) {
	float Cos = Dot(q0, q1);

	cQuat p1;
	if(Cos < 0.0f && ShortestPath) {
		p1 = -q1;
	} else {
		p1 = q1;
	}
	return Normalize(q0 + s * (p1 - q0));
}

//--------------------------------------------------------------------------------------------------
// cQuat::Slerp : const cQuat (const cQuat &, const cQuat &, const float, const bool)
//--------------------------------------------------------------------------------------------------
const cQuat cQuat::Slerp(const cQuat &q0, const cQuat &q1, const float s, const bool ShortestPath) {
	float Cos = Dot(q0, q1);
	cQuat t;
	if(Cos < 0.0f) {
		t = -q1;
		Cos = -Cos;
	} else {
		t = q1;
	}
	float Scale0, Scale1;
	if(1.0f - Cos > 0.0f) {
		float Angle = cMath::ACos(Cos);
		float iSin = 1.0f / cMath::Sin(Angle);
		Scale0 = cMath::Sin((1.0f - s) * Angle) * iSin;
		Scale1 = cMath::Sin(s * Angle) * iSin;
	} else {
		Scale0 = 1.0f - s;
		Scale1 = s;
	}
	return Scale0 * q0 + Scale1 * t;
}

//------------------------------------------------------------------------------------------------------------------------
// cQuat::SquadSetup : void (const cQuat &, const cQuat &, const cQuat &, const cQuat &, cQuat &, cQuat &, cQuat &)
//------------------------------------------------------------------------------------------------------------------------
void cQuat::SquadSetup(const cQuat &q0, const cQuat &q1, const cQuat &q2, const cQuat &q3, cQuat &A, cQuat &B, cQuat &C) {
	cQuat p0 = Dot(q0, q1) >= 0.0f ? q0 : -q0;
	cQuat p2 = Dot(q1, q2) >= 0.0f ? q2 : -q2;
	cQuat p3 = Dot(p2, q3) >= 0.0f ? q3 : -q3;

	A = q1 * Exp(-0.25f * (LnDif(q1, p2) + LnDif(q1, p0)));
	B = p2 * Exp(-0.25f * (LnDif(p2, p3) + LnDif(p2, q1)));
	C = p2;
}

//--------------------------------------------------------------------------------------------------------
// cQuat::Squad : const cQuat (const cQuat &, const cQuat &, const cQuat &, const cQuat &, const float)
//--------------------------------------------------------------------------------------------------------
const cQuat cQuat::Squad(const cQuat &q1, const cQuat &A, const cQuat &B, const cQuat &C, const float s) {
	return Slerp(Slerp(q1, C, s), Slerp(A, B, s), 2.0f * s * (1.0f - s));
}

//-----------------------------------------------------------------------------
// cQuat::Exp : const cQuat (const cQuat &)
//-----------------------------------------------------------------------------
const cQuat cQuat::Exp(const cQuat &q) {
	const float Angle = cMath::Sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
	float Cos = cMath::Cos(Angle);
	
	if(Angle == 0.0f) {
		return cQuat(0.0f, 0.0f, 0.0f, Cos);
	} else {
		float s = cMath::Sin(Angle) / Angle;
		return cQuat(s * q.x, s * q.y, s * q.z, Cos);
	}
}

//-----------------------------------------------------------------------------
// cQuat::Ln : const cQuat (const cQuat &)
//-----------------------------------------------------------------------------
const cQuat cQuat::Ln(const cQuat &q) {
	const float lPure = cMath::Sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
	if(lPure == 0.0f) {
		return cQuat(0.0f, 0.0f, 0.0f, 0.0f);
	} else {
		float s = cMath::ATan(lPure, q.w) / lPure;
		return cQuat(s * q.x, s * q.y, s * q.z, 0.0f);
	}
}

//-----------------------------------------------------------------------------
// cQuat::LnDif : const cQuat(const cQuat &, const cQuat &)
//-----------------------------------------------------------------------------
const cQuat cQuat::LnDif (const cQuat &q0, const cQuat &q1) {
	cQuat Dif = Invert(q0) * q1;
	assert(Dif.IsNormalized()); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	return Ln(Dif);
}


/*
	if(!cMath::IsZero(Angle)) {
		float s = cMath::Sin(Angle) / Angle;
		return cQuat(s * q.x, s * q.y, s * q.z, Cos);
	} else {
		return cQuat(q.x, q.y, q.z, Cos);
	}

const cQuat cQuat::_Exp(const cQuat &q) {
	float l = q.Length();
	if(l == 0.0f) {
		return cQuat(0.0f, 0.0f, 0.0f, cMath::Exp(q.w));
	} else {
		float c = (cMath::Exp(q.w) * cMath::Sin(l)) / l;
		return cQuat(c * q.x, c * q.y, c * q.z, cMath::Exp(q.w) * cMath::Cos(l));
	}
}*/



//	float s = lPure > cMath::Epsilon ? cMath::ATan(lPure, q.w) / lPure : 0.0f;
//	return cQuat(s * q.x, s * q.y, s * q.z, 0.0f);
//}

/*
const cQuat cQuat::_Ln(const cQuat &q) {
	float l = q.Length();
	if(l == 0.0f) {
		if(q.w > 0.0f) {
			return cQuat(0.0f, 0.0f, 0.0f, cMath::Log(q.w));
		} else {
			return cQuat(cMath::Pi, 0.0f, 0.0f, cMath::Log(-q.w));
		}
	} else {
		float c = cMath::ATan(l, q.w) / l;
		return cQuat(c * q.x, c * q.y, c * q.z, 0.5f * cMath::Log(q.w * q.w + l * l));
	}
}*/