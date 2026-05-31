#include "comms.h"

const cMat4 cMat4::Zero(cMat4::ZeroCtor);
const cMat4 cMat4::Identity(cMat4::IdentityCtor);

//-------------------------------------------------------------------------------------------------------------------
// cMat4::OfLookAt : cMat4(const cVec3 &, float, float, float, float, eCoordSystem)
//-------------------------------------------------------------------------------------------------------------------
cMat4 cMat4::OfLookAt(const cVec3 &At, float Azim, float Elev, float toEye, float Roll, eCoordSystem Type) {
	cVec3 zAxis = cVec3::FromSpherical(1.0f, Azim, Elev);
	cVec3 Up = cVec3::FromSpherical(1.0f, Azim - Roll, Elev - cMath::Rad(45.0f)) - zAxis;
	cVec3 Pos = At + toEye * zAxis;
	if(CS_LEFTHANDED == Type) {
		zAxis *= -1.0f;
	}
	cVec3 xAxis = cVec3::Normalize(cVec3::Cross(Up, zAxis));
	cVec3 yAxis = cVec3::Normalize(cVec3::Cross(zAxis, xAxis));
	cVec3 t( - cVec3::Dot(xAxis, Pos), - cVec3::Dot(yAxis, Pos), - cVec3::Dot(zAxis, Pos));
	return cMat4::FromAxesAndTransl(xAxis, yAxis, zAxis, t);
}

//--------------------------------------------------------------------------------------------------
// cMat4::OfLookAtRH : cMat4(const cVec3 &, float, float, float, float)
//--------------------------------------------------------------------------------------------------
cMat4 cMat4::OfLookAtRH(const cVec3 &At, float Azim, float Elev, float ToEye, float Roll) {
	cVec3 zAxis = cVec3::FromSpherical(1.0f, Azim, Elev);
	cVec3 Eye = At + ToEye * zAxis;
	cVec3 Up = cVec3::FromSpherical(1.0f, Azim, Elev - cMath::HalfPi);
	cVec3 xAxis = cVec3::Normalize(cVec3::Cross(Up, zAxis));
	cVec3 yAxis = cVec3::Normalize(cVec3::Cross(zAxis, xAxis));
	cVec3 Transl(cVec3::Dot(xAxis, Eye), cVec3::Dot(yAxis, Eye), cVec3::Dot(zAxis, Eye));
	return cMat4::FromAxesAndTransl(xAxis, yAxis, zAxis, - Transl);
}

cMat4 cMat4::_OfLookAt(const cVec3 &At, float Azim, float Elev, float ToEye, float Roll) {
	cVec3 zAxis = - cVec3::FromSpherical(1.0f, Azim, Elev);
	cVec3 Eye = At - ToEye * zAxis;
	cVec3 Up = cVec3::FromSpherical(1.0f, Azim, Elev - cMath::HalfPi);
	cVec3 xAxis = cVec3::Normalize(cVec3::Cross(Up, zAxis));
	cVec3 yAxis = cVec3::Normalize(cVec3::Cross(zAxis, xAxis));
	cVec3 Transl(cVec3::Dot(Eye, xAxis), cVec3::Dot(Eye, yAxis), cVec3::Dot(Eye, zAxis));
	return cMat4::FromAxesAndTransl(xAxis, yAxis, zAxis, - Transl);
}

//--------------------------------------------------------------------------------------------------
// cMat4::OfLookAtLH : cMat4(const cVec3 &, float, float, float, float)
//--------------------------------------------------------------------------------------------------
cMat4 cMat4::OfLookAtLH(const cVec3 &At, float Azim, float Elev, float ToEye, float Roll) {
	cVec3 zAxis = - cVec3::FromSpherical(1.0f, Azim, Elev);
	cVec3 Eye = At - ToEye * zAxis;
	cVec3 Up = cVec3::FromSpherical(1.0f, Azim, Elev - cMath::HalfPi);
	cVec3 xAxis = cVec3::Normalize(cVec3::Cross(Up, zAxis));
	cVec3 yAxis = cVec3::Normalize(cVec3::Cross(zAxis, xAxis));
	cVec3 Transl(cVec3::Dot(xAxis, Eye), cVec3::Dot(yAxis, Eye), cVec3::Dot(zAxis, Eye));
	return cMat4::FromAxesAndTransl(xAxis, yAxis, zAxis, - Transl);
}

//-------------------------------------------------------------------------------------
// cMat4::OfProjection : cMat4(float, float, float, float)
//-------------------------------------------------------------------------------------
cMat4 cMat4::OfProjection(float Fov, float AspectWtoH, float zNear, float zFar) {
	float w = 2.0f * zNear * cMath::Tan(Fov * 0.5f);
	float h = w / AspectWtoH;

	cMat4 P;
	ZeroMemory(P.ToFloatPtr(), sizeof(cMat4));

	P[0][0] = 2 * zNear / w;
	P[1][1] = 2 * zNear / h;
	P[2][2] = zFar / (zFar - zNear);
	P[2][3] = 1.0f;
    P[3][2] = zNear * zFar / (zNear-zFar);
	
	return P;
}

//-------------------------------------------------------------------------------------------
// cMat4::PerspectiveYFovRH
//-------------------------------------------------------------------------------------------
const cMat4 cMat4::PerspectiveYFovRH(float YFov, float AspectWtoH, float zNear, float zFar) {
	cMat4 P(cMat4::ZeroCtor);

	const float h = cMath::Tan(0.5f * cMath::Rad(YFov));
	const float w = h * AspectWtoH;
	
	P(0, 0) = 1.0f / w;
	P(1, 1) = 1.0f / h;
	P(2, 2) = (zFar + zNear) / (zNear - zFar);
	P(2, 3) = -1.0f;
	P(3, 2) = 2.0f * zNear * zFar / (zNear - zFar);
	
	return P;
} // cMat4::PerspectiveYFovRH

//----------------------------------------------------------------------------------------------------
// cMat4::PerspectiveYFovRHInf
//----------------------------------------------------------------------------------------------------
const cMat4 cMat4::PerspectiveYFovRHInf(const float YFov, const float AspectWtoH, const float zNear) {
	cMat4 P(cMat4::ZeroCtor);

	const float h = cMath::Tan(0.5f * cMath::Rad(YFov));
	const float w = h * AspectWtoH;
	
	P(0, 0) = 1.0f / w;
	P(1, 1) = 1.0f / h;
	P(2, 2) = -1.0f;
	P(2, 3) = -1.0f;
	P(3, 2) = -2.0f * zNear;
	
	return P;
} // cMat4::PerspectiveYFovRHInf

//-------------------------------------------------------------------------------------------
// cMat4::PerspectiveYFovLH
//-------------------------------------------------------------------------------------------
const cMat4 cMat4::PerspectiveYFovLH(float YFov, float AspectWtoH, float zNear, float zFar) {
	cMat4 P(cMat4::ZeroCtor);

	const float h = cMath::Tan(0.5f * cMath::Rad(YFov));
	const float w = h * AspectWtoH;
	
	P(0, 0) = 1.0f / w;
	P(1, 1) = 1.0f / h;
	P(2, 2) = (zFar + zNear) / (zFar - zNear);
	P(2, 3) = 1.0f;
	P(3, 2) = - 2.0f * zNear * zFar / (zFar - zNear);
	
	return P;
} // cMat4::PerspectiveYFovLH

//----------------------------------------------------------------------------------------------------
// cMat4::PerspectiveYFovLHInf
//----------------------------------------------------------------------------------------------------
const cMat4 cMat4::PerspectiveYFovLHInf(const float YFov, const float AspectWtoH, const float zNear) {
	cMat4 P(cMat4::ZeroCtor);

	const float h = cMath::Tan(0.5f * cMath::Rad(YFov));
	const float w = h * AspectWtoH;

	P(0, 0) = 1.0f / w;
	P(1, 1) = 1.0f / h;
	P(2, 2) = 1.0f;
	P(2, 3) = 1.0f;
	P(3, 2) = -2.0f * zNear;

	return P;
} // cMat4::PerspectiveYFovLHInf

//-------------------------------------------------------------------------------------------
// cMat4::PerspectiveXFovRH
//-------------------------------------------------------------------------------------------
const cMat4 cMat4::PerspectiveXFovRH(float XFov, float AspectWtoH, float zNear, float zFar) {
	cMat4 P(cMat4::ZeroCtor);
	
	const float w = cMath::Tan(0.5f * cMath::Rad(XFov));
	const float h = w / AspectWtoH;
	
	P(0, 0) = 1.0f / w;
	P(1, 1) = 1.0f / h;
	P(2, 2) = (zFar + zNear) / (zNear - zFar);
	P(2, 3) = - 1.0f;
	P(3, 2) = 2.0f * zNear * zFar / (zNear - zFar);

	return P;
} // cMat4::PerspectiveXFovRH

//-------------------------------------------------------------------------------------------
// cMat4::PerspectiveXFovLH
//-------------------------------------------------------------------------------------------
const cMat4 cMat4::PerspectiveXFovLH(float XFov, float AspectWtoH, float zNear, float zFar) {
	cMat4 P(cMat4::ZeroCtor);
	
	const float w = cMath::Tan(0.5f * cMath::Rad(XFov));
	const float h = w / AspectWtoH;
	
	P(0, 0) = 1.0f / w;
	P(1, 1) = 1.0f / h;
	P(2, 2) = (zFar + zNear) / (zFar - zNear);
	P(2, 3) = 1.0f;
	P(3, 2) = - 2.0f * zNear * zFar / (zFar - zNear);
	
	return P;
} // cMat4::PerspectiveXFovLH


const cMat4 cMat4::Translation(const cVec3 &t) {
	cMat4 T(cMat4::Identity);
	T.SetTransl(t);
	return T;
}


//-----------------------------------------------------------------------------
// cMat4::Invert : bool (const cMat4, cMat4 *)
//-----------------------------------------------------------------------------
bool cMat4::Invert(const cMat4 &Fm, cMat4 *To) {
	const float d2_01_01 = Fm[0][0] * Fm[1][1] - Fm[0][1] * Fm[1][0];
	const float d2_01_02 = Fm[0][0] * Fm[1][2] - Fm[0][2] * Fm[1][0];
	const float d2_01_03 = Fm[0][0] * Fm[1][3] - Fm[0][3] * Fm[1][0];
	const float d2_01_12 = Fm[0][1] * Fm[1][2] - Fm[0][2] * Fm[1][1];
	const float d2_01_13 = Fm[0][1] * Fm[1][3] - Fm[0][3] * Fm[1][1];
	const float d2_01_23 = Fm[0][2] * Fm[1][3] - Fm[0][3] * Fm[1][2];
	
	const float d3_012_012 = Fm[2][0] * d2_01_12 - Fm[2][1] * d2_01_02 + Fm[2][2] * d2_01_01;
	const float d3_012_013 = Fm[2][0] * d2_01_13 - Fm[2][1] * d2_01_03 + Fm[2][3] * d2_01_01;
	const float d3_012_023 = Fm[2][0] * d2_01_23 - Fm[2][2] * d2_01_03 + Fm[2][3] * d2_01_02;
	const float d3_012_123 = Fm[2][1] * d2_01_23 - Fm[2][2] * d2_01_13 + Fm[2][3] * d2_01_12;
	
	const float Det = - Fm[3][0] * d3_012_123 + Fm[3][1] * d3_012_023 - Fm[3][2] * d3_012_013 + Fm[3][3] * d3_012_012;
	
	if(cMath::IsZero(Det, cMath::MatrixInvertEpsilon)) {
		return false;
	}

	const float iDet = 1.0f / Det;
	
	const float d2_03_01 = Fm[0][0] * Fm[3][1] - Fm[0][1] * Fm[3][0];
	const float d2_03_02 = Fm[0][0] * Fm[3][2] - Fm[0][2] * Fm[3][0];
	const float d2_03_03 = Fm[0][0] * Fm[3][3] - Fm[0][3] * Fm[3][0];
	const float d2_03_12 = Fm[0][1] * Fm[3][2] - Fm[0][2] * Fm[3][1];
	const float d2_03_13 = Fm[0][1] * Fm[3][3] - Fm[0][3] * Fm[3][1];
	const float d2_03_23 = Fm[0][2] * Fm[3][3] - Fm[0][3] * Fm[3][2];
	
	const float d2_13_01 = Fm[1][0] * Fm[3][1] - Fm[1][1] * Fm[3][0];
	const float d2_13_02 = Fm[1][0] * Fm[3][2] - Fm[1][2] * Fm[3][0];
	const float d2_13_03 = Fm[1][0] * Fm[3][3] - Fm[1][3] * Fm[3][0];
	const float d2_13_12 = Fm[1][1] * Fm[3][2] - Fm[1][2] * Fm[3][1];
	const float d2_13_13 = Fm[1][1] * Fm[3][3] - Fm[1][3] * Fm[3][1];
	const float d2_13_23 = Fm[1][2] * Fm[3][3] - Fm[1][3] * Fm[3][2];
	
	const float d3_023_012 = Fm[2][0] * d2_03_12 - Fm[2][1] * d2_03_02 + Fm[2][2] * d2_03_01;
	const float d3_023_013 = Fm[2][0] * d2_03_13 - Fm[2][1] * d2_03_03 + Fm[2][3] * d2_03_01;
	const float d3_023_023 = Fm[2][0] * d2_03_23 - Fm[2][2] * d2_03_03 + Fm[2][3] * d2_03_02;
	const float d3_023_123 = Fm[2][1] * d2_03_23 - Fm[2][2] * d2_03_13 + Fm[2][3] * d2_03_12;

	const float d3_123_012 = Fm[2][0] * d2_13_12 - Fm[2][1] * d2_13_02 + Fm[2][2] * d2_13_01;
	const float d3_123_013 = Fm[2][0] * d2_13_13 - Fm[2][1] * d2_13_03 + Fm[2][3] * d2_13_01;
	const float d3_123_023 = Fm[2][0] * d2_13_23 - Fm[2][2] * d2_13_03 + Fm[2][3] * d2_13_02;
	const float d3_123_123 = Fm[2][1] * d2_13_23 - Fm[2][2] * d2_13_13 + Fm[2][3] * d2_13_12;

	const float d3_013_012 = Fm[3][0] * d2_01_12 - Fm[3][1] * d2_01_02 + Fm[3][2] * d2_01_01;
	const float d3_013_013 = Fm[3][0] * d2_01_13 - Fm[3][1] * d2_01_03 + Fm[3][3] * d2_01_01;
	const float d3_013_023 = Fm[3][0] * d2_01_23 - Fm[3][2] * d2_01_03 + Fm[3][3] * d2_01_02;
	const float d3_013_123 = Fm[3][1] * d2_01_23 - Fm[3][2] * d2_01_13 + Fm[3][3] * d2_01_12;

	To->Elem(0,0) = - d3_123_123 * iDet;
	To->Elem(1,0) = + d3_123_023 * iDet;
	To->Elem(2,0) = - d3_123_013 * iDet;
	To->Elem(3,0) = + d3_123_012 * iDet;
	
	To->Elem(0,1) = + d3_023_123 * iDet;
	To->Elem(1,1) = - d3_023_023 * iDet;
	To->Elem(2,1) = + d3_023_013 * iDet;
	To->Elem(3,1) = - d3_023_012 * iDet;
	
	To->Elem(0,2) = + d3_013_123 * iDet;
	To->Elem(1,2) = - d3_013_023 * iDet;
	To->Elem(2,2) = + d3_013_013 * iDet;
	To->Elem(3,2) = - d3_013_012 * iDet;
	
	To->Elem(0,3) = - d3_012_123 * iDet;
	To->Elem(1,3) = + d3_012_023 * iDet;
	To->Elem(2,3) = - d3_012_013 * iDet;
	To->Elem(3,3) = + d3_012_012 * iDet;
	
	return true;
} // cMat4::Invert