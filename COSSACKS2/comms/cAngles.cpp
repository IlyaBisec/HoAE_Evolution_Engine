#include "comms.h"

const cAngles cAngles::Zero(0.0f, 0.0f, 0.0f);

//-----------------------------------------------------------------------------
// cAngles::Normalize360 : cAngles & ()
//-----------------------------------------------------------------------------
cAngles & cAngles::Normalize360() {
	for(int i = 0; i < 3; i++) {
		if((*this)[i] >= 360.0f || (*this)[i] < 0.0f) {
			(*this)[i] -= cMath::Floor((*this)[i] / 360.0f) * 360.0f;
			if((*this)[i] >= 360.0f) {
				(*this)[i] -= 360.0f;
			}
			if((*this)[i] < 0.0f) {
				(*this)[i] += 360.0f;
			}
		}
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cAngles::Normalize180 : cAngles & ()
//-----------------------------------------------------------------------------
cAngles & cAngles::Normalize180() {
	Normalize360();
	if(Pitch > 180.0f) {
		Pitch -= 360.0f;
	}
	if(Yaw > 180.0f) {
		Yaw -= 360.0f;
	}
	if(Roll > 180.0f) {
		Roll -= 360.0f;
	}
	return *this;
}


//-----------------------------------------------------------------------------
// cAngles::ToQuat : cQuat() const
//-----------------------------------------------------------------------------
cQuat cAngles::ToQuat() const {
	float ZSin, ZCos;
	cMath::SinCos(cMath::Rad(Yaw) * 0.5f, ZSin, ZCos);
	float YSin, YCos;
	cMath::SinCos(cMath::Rad(Pitch) * 0.5f, YSin, YCos);
	float XSin, XCos;
	cMath::SinCos(cMath::Rad(Roll) * 0.5f, XSin, XCos);

	float XSinYCos = XSin * YCos;
	float XCosYCos = XCos * YCos;
	float XSinYSin = XSin * YSin;
	float XCosYSin = XCos * YSin;
	
	return cQuat(XCosYSin * ZSin - XSinYCos * ZCos, - XCosYSin * ZCos - XSinYCos * ZSin,
		XSinYSin * ZCos - XCosYCos * ZSin, XCosYCos * ZCos + XSinYSin * ZSin);
}

// cAngles::ToMat3
const cMat3 cAngles::ToMat3() const {
	return cMat3::RotationZXY(Roll, Pitch, Yaw);
}

// cAngles::ToMat4
const cMat4 cAngles::ToMat4() const {
	return ToMat3().ToMat4();
}

// cAngles::ToMat3Z
const cMat3 cAngles::ToMat3Z() const {
	return cMat3::RotationZXZ(Roll, Pitch, Yaw);
}

// cAngles::ToMat4Z
const cMat4 cAngles::ToMat4Z() const {
	return ToMat3Z().ToMat4();
}

// cAngles::ToVectors
void cAngles::ToVectors(cVec3 *Forward, cVec3 *Right, cVec3 *Up) const {
	ToMat3().ToVectors(Forward, Right, Up);
}

// cAngles::ToForward
const cVec3 cAngles::ToForward() const {
	return ToMat3().ToForward();
}

// cAngles::ToVectorsZ
void cAngles::ToVectorsZ(cVec3 *Forward, cVec3 *Right, cVec3 *Up) const {
	ToMat3Z().ToVectors(Forward, Right, Up);
}

// cAngles::ToForward
const cVec3 cAngles::ToForwardZ() const {
	return ToMat3Z().ToForward();
}

//-----------------------------------------------------------------------------
// cAngles::ToStr : cStr(const int) const
//-----------------------------------------------------------------------------
const cStr cAngles::ToStr(const int nPrec) const {
	return cStr::FromFloatArray(ToFloatPtr(), GetDimension(), nPrec);
}