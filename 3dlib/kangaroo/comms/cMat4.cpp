#include "comms.h"

const cMat4 cMat4::Zero(0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f);

const cMat4 cMat4::Identity(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);

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
	ZeroMemory(P.ToPtr(), sizeof(cMat4));

	P.m[0][0] = 2 * zNear / w;
	P.m[1][1] = 2 * zNear / h;
	P.m[2][2] = zFar / (zFar - zNear);
	P.m[2][3] = 1.0f;
    P.m[3][2] = zNear * zFar / (zNear-zFar);
	
	return P;
}

//-----------------------------------------------------------------------------------------
// cMat4::OfPerspectiveRH : cMat4(float, float, float, float)
//-----------------------------------------------------------------------------------------
cMat4 cMat4::OfPerspectiveYFovRH(float yFov, float AspectWtoH, float zNear, float zFar) {
	cMat4 P(cMat4::Zero);

	float h = cMath::Tan(0.5f * yFov);
	float w = h * AspectWtoH;

	P[0][0] = 1.0f / w;
	P[1][1] = 1.0f / h;
	P[2][2] = (zFar + zNear) / (zNear - zFar);
	P[2][3] = - 1.0f;
	P[3][2] = 2.0f * zNear * zFar / (zNear - zFar);

	return P;
}

//---------------------------------------------------------------------------------------------
// cMat4::OfPerspectiveXFovRH(float, float, float, float)
//---------------------------------------------------------------------------------------------
cMat4 cMat4::OfPerspectiveXFovRH(float xFov, float AspectWtoH, float zNear, float zFar) {
	cMat4 P(cMat4::Zero);

	float w = cMath::Tan(0.5f * xFov);
	float h = w / AspectWtoH;

	P[0][0] = 1.0f / w;
	P[1][1] = 1.0f / h;
	P[2][2] = (zFar + zNear) / (zNear - zFar);
	P[2][3] = - 1.0f;
	P[3][2] = 2.0f * zNear * zFar / (zNear - zFar);

	return P;
}

//-----------------------------------------------------------------------------------------
// cMat4::OfPerspectiveYFovLH : cMat4(float, float, float, float)
//-----------------------------------------------------------------------------------------
cMat4 cMat4::OfPerspectiveYFovLH(float yFov, float AspectWtoH, float zNear, float zFar) {
	cMat4 P(cMat4::Zero);

	float h = cMath::Tan(0.5f * yFov);
	float w = h * AspectWtoH;

	P[0][0] = 1.0f / w;
	P[1][1] = 1.0f / h;
	P[2][2] = (zFar + zNear) / (zFar - zNear);
	P[2][3] = 1.0f;
	P[3][2] = - 2.0f * zNear * zFar / (zFar - zNear);

	return P;
}

//---------------------------------------------------------------------------------------------
// cMat4::OfPerspectiveXFovLH : cMat4(float, float, float, float)
//---------------------------------------------------------------------------------------------
cMat4 cMat4::OfPerspectiveXFovLH(float xFov, float AspectWtoH, float zNear, float zFar) {
	cMat4 P(cMat4::Zero);

	float w = cMath::Tan(0.5f * xFov);
	float h = w / AspectWtoH;

	P[0][0] = 1.0f / w;
	P[1][1] = 1.0f / h;
	P[2][2] = (zFar + zNear) / (zFar - zNear);
	P[2][3] = 1.0f;
	P[3][2] = - 2.0f * zNear * zFar / (zFar - zNear);

	return P;
}

//-----------------------------------------------------------------------------
// cMat4::OfShadow : cMat4(const cVec4 &, const cPlane &)
//-----------------------------------------------------------------------------
cMat4 cMat4::OfShadow(const cVec4 &Light, const cPlane &Plane) {
	cPlane P(Plane);
	P.MutableNormal().Normalize();
	cVec4 L(Light);

	float d = cVec4::Dot(L, cVec4(P.GetNormal(), P.d));
	return cMat4(P.a * L.x + d, P.a * L.y,     P.a * L.z,     P.a * L.w,
					P.b * L.x,     P.b * L.y + d, P.b * L.z,     P.b * L.w,
					P.c * L.x,     P.c * L.y,     P.c * L.z + d, P.c * L.w,
					P.d * L.x,     P.d * L.y,     P.d * L.z,     P.d * L.w + d);
}