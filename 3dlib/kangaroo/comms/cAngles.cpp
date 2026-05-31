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

//-------------------------------------------------------------------------------
// cAngles::ToVectors : void(cVec3 *, cVec3 *, cVec3 *) const
//-------------------------------------------------------------------------------
void cAngles::ToVectors(cVec3 *Forward, cVec3 *Right, cVec3 *Up) const {
	float PSin, PCos;
	cMath::SinCos(cMath::Rad(Pitch), PSin, PCos);
	float YSin, YCos;
	cMath::SinCos(cMath::Rad(Yaw), YSin, YCos);
	float RSin, RCos;
	cMath::SinCos(cMath::Rad(Roll), RSin, RCos);
	
	if(Forward) {
		Forward->Set(PCos * YCos, PCos * YSin, - PSin);
	}
	
	if(Right) {
		Right->Set(- RSin * PSin * YCos + RCos * YSin, - RSin * PSin * YSin - RCos * YCos, - RSin * PCos);
	}
	
	if(Up) {
		Up->Set(RCos * PSin * YCos - RSin * - YSin, RCos * PSin * YSin - RSin * YCos, RCos * PCos);
	}
}

//-----------------------------------------------------------------------------
// cAngles::ToForward : cVec3() const
//-----------------------------------------------------------------------------
cVec3 cAngles::ToForward() const {
	float PSin, PCos;
	cMath::SinCos(cMath::Rad(Pitch), PSin, PCos);
	float YSin, YCos;
	cMath::SinCos(cMath::Rad(Yaw), YSin, YCos);

	return cVec3(PCos * YCos, PCos * YSin, - PSin);
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

//-----------------------------------------------------------------------------
// cAngles::ToMat3 : cMat3 () const
//-----------------------------------------------------------------------------
cMat3 cAngles::ToMat3() const {
	float PSin, PCos;
	cMath::SinCos(cMath::Rad(Pitch), PSin, PCos);
	float YSin, YCos;
	cMath::SinCos(cMath::Rad(Yaw), YSin, YCos);
	float RSin, RCos;
	cMath::SinCos(cMath::Rad(Roll), RSin, RCos);

	cMat3 M;
	M.SetColumnX(cVec3(PCos * YCos, PCos * YSin, - PSin));
	M.SetColumnY(cVec3(RSin * PSin * YCos - RCos * YSin, RSin * PSin * YSin + RCos * YCos, RSin * PCos));
	M.SetColumnZ(cVec3(RCos * PSin * YCos + RSin * YSin, RCos * PSin * YSin - RSin * YCos, RCos * PCos));
	return M;
}

//-----------------------------------------------------------------------------
// cAngles::ToMat4 : cMat4 () const
//-----------------------------------------------------------------------------
cMat4 cAngles::ToMat4() const {
	return ToMat3().ToMat4();
}

//-----------------------------------------------------------------------------
// cAngles::ToStr : cStr(const int) const
//-----------------------------------------------------------------------------
cStr cAngles::ToStr(const int nPrec) const {
	return cStr::FromArray(ToPtr(), GetDimension(), nPrec);
}