#include "comms.h"
	
const cMat3 cMat3::Zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
const cMat3 cMat3::Identity(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

//---------------------------------------------------------------------------------------------------------------------
// cMat3::OfFitToRect : cMat3(const cVec2 &, const cVec2 &, const cVec2 &, const cVec2 &, bool, bool)
//---------------------------------------------------------------------------------------------------------------------
cMat3 cMat3::OfFitToRect(const cVec2 &bbMin, const cVec2 &bbMax,
							   const cVec2 &rcMin, const cVec2 &rcMax, bool xScale, bool yScale) {
	cVec2 b = bbMax - bbMin;
	bool bxIsNotZero = !cMath::IsZero(b.x);
	bool byIsNotZero = !cMath::IsZero(b.y);

	cVec2 L = rcMax - rcMin;

	float s = 1.0f;
	if(bxIsNotZero && byIsNotZero && xScale && yScale) {
		s = cMath::Min(L.x / b.x, L.y / b.y);
	} else if(bxIsNotZero && xScale) {
		s = L.x / b.x;
	} else if(byIsNotZero && yScale) {
		s = L.y / b.y;
	}
	
	cVec2 d = (rcMin - s * bbMin + rcMax - s * bbMax) / 2.0f;
	return OfTranslation(d) * OfScale(cVec2(s));
} // cMat3::OfFitToRect

//-----------------------------------------------------------------------------
// cMat3::Invert : bool(const cMat3 &, cMat3 *, const float)
//-----------------------------------------------------------------------------
bool cMat3::Invert(const cMat3 &Src, cMat3 *pDst, const float Eps) {
	float d2_12_01 = Src[1][0] * Src[2][1] - Src[1][1] * Src[2][0];
	float d2_12_02 = Src[1][0] * Src[2][2] - Src[1][2] * Src[2][0];
	float d2_12_12 = Src[1][1] * Src[2][2] - Src[1][2] * Src[2][1];
	
	float det =  Src[0][0] * d2_12_12 - Src[0][1] * d2_12_02 + Src[0][2] * d2_12_01;

	if(cMath::IsZero(det, Eps)) {
		return false;
	}

	float idet = 1.0f / det;

	float d2_01_01 = Src[0][0] * Src[1][1] - Src[0][1] * Src[1][0];
	float d2_01_02 = Src[0][0] * Src[1][2] - Src[0][2] * Src[1][0];
	float d2_01_12 = Src[0][1] * Src[1][2] - Src[0][2] * Src[1][1];

	float d2_02_01 = Src[0][0] * Src[2][1] - Src[0][1] * Src[2][0];
	float d2_02_02 = Src[0][0] * Src[2][2] - Src[0][2] * Src[2][0];
	float d2_02_12 = Src[0][1] * Src[2][2] - Src[0][2] * Src[2][1];


	pDst->m[0][0] = + d2_12_12 * idet;
	pDst->m[1][0] = - d2_12_02 * idet;
	pDst->m[2][0] = + d2_12_01 * idet;

	pDst->m[0][1] = - d2_02_12 * idet;
	pDst->m[1][1] = + d2_02_02 * idet;
	pDst->m[2][1] = - d2_02_01 * idet;

	pDst->m[0][2] = + d2_01_12 * idet;
	pDst->m[1][2] = - d2_01_02 * idet;
	pDst->m[2][2] = + d2_01_01 * idet;
	
	return true;
}

//-----------------------------------------------------------------------------
// cMat3::ToAngles : cAngles() const
//-----------------------------------------------------------------------------
cAngles cMat3::ToAngles() const {
	float Sin = m[0][2];
	if(Sin > 1.0f) {
		Sin = 1.0f;
	} else if(Sin < -1.0f) {
		Sin = -1.0f;
	}
	
	float Angle = - cMath::ASin(Sin);
	float Cos = cMath::Cos(Angle);

	cAngles A;
	A.Pitch = cMath::Deg(Angle);
	if(Cos > 8192.0f * cMath::Epsilon) {
		A.Yaw = cMath::Deg(cMath::ATan(m[0][1], m[0][0]));
		A.Roll = cMath::Deg(cMath::ATan(m[1][2], m[2][2]));
	} else {
		A.Yaw = cMath::Deg(-cMath::ATan(m[1][0], m[1][1]));
		A.Roll = 0.0f;
	}
	return A;
}