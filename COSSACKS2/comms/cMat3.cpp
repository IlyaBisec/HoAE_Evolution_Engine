#include "comms.h"
	
const cMat3 cMat3::Zero(cMat3::ZeroCtor);
const cMat3 cMat3::Identity(cMat3::IdentityCtor);

// cMat3::GetDeterminant
float cMat3::GetDeterminant() const {
//	cAssert(IsValid());

	float d2_12_01 = m_Rows[1][0] * m_Rows[2][1] - m_Rows[1][1] * m_Rows[2][0];
	float d2_12_02 = m_Rows[1][0] * m_Rows[2][2] - m_Rows[1][2] * m_Rows[2][0];
	float d2_12_12 = m_Rows[1][1] * m_Rows[2][2] - m_Rows[1][2] * m_Rows[2][1];
	
	return m_Rows[0][0] * d2_12_12 - m_Rows[0][1] * d2_12_02 + m_Rows[0][2] * d2_12_01;
}

// cMat3::RotationX
const cMat3 cMat3::RotationX(const float Angle) {
	cAssert(cMath::IsValid(Angle));

	float Sin, Cos;
	cMath::SinCos(cMath::Rad(Angle), Sin, Cos);
	
	cMat3 Rx;
	Rx.SetRow0(1.0f, 0.0f, 0.0f);
	Rx.SetRow1(0.0f, Cos, Sin);
	Rx.SetRow2(0.0f, -Sin, Cos);

	cAssert(Rx.IsOrthonormal());

	return Rx;
}

// cMat3::RotationY
const cMat3 cMat3::RotationY(const float Angle) {
	cAssert(cMath::IsValid(Angle));

	float Sin, Cos;
	cMath::SinCos(cMath::Rad(Angle), Sin, Cos);
	
	cMat3 Ry;
	Ry.SetRow0(Cos, 0.0f, -Sin);
	Ry.SetRow1(0.0f, 1.0f, 0.0f);
	Ry.SetRow2(Sin, 0.0f, Cos);

	cAssert(Ry.IsOrthonormal());

	return Ry;
}

// cMat3::RotationZ
const cMat3 cMat3::RotationZ(const float Angle) {
	cAssert(cMath::IsValid(Angle));

	float Sin, Cos;
	cMath::SinCos(cMath::Rad(Angle), Sin, Cos);
	
	cMat3 Rz;
	Rz.SetRow0(Cos, Sin, 0.0f);
	Rz.SetRow1(-Sin, Cos, 0.0f);
	Rz.SetRow2(0.0f, 0.0f, 1.0f);

	cAssert(Rz.IsOrthonormal());

	return Rz;
}

// cMat3::RotationZXY
const cMat3 cMat3::RotationZXY(const float Roll, const float Pitch, const float Yaw) {
	const cMat3 R = cMat3::RotationZ(Roll) * cMat3::RotationX(Pitch) * cMat3::RotationY(Yaw);
	cAssert(R.IsOrthonormal());
	return R;
}

// cMat3::RotationZXZ
const cMat3 cMat3::RotationZXZ(const float Roll, const float Pitch, const float Yaw) {
	const cMat3 R = cMat3::RotationZ(Roll) * cMat3::RotationX(Pitch) * cMat3::RotationZ(Yaw);
	cAssert(R.IsOrthonormal());
	return R;
}

// cMat3::RotationAxis
const cMat3 cMat3::RotationAxis(const cVec3 &Axis, const float Angle) {
	const float HalfAngle = cMath::Rad(Angle) * 0.5f;

	float Sin, Cos;
	cMath::SinCos(HalfAngle, Sin, Cos);
	
	const float qx = Sin * Axis.x;
	const float qy = Sin * Axis.y;
	const float qz = Sin * Axis.z;
	
	cMat3 R;
	R(0, 0) = 1.0f - 2.0f * (qy * qy + qz * qz);
	R(1, 1) = 1.0f - 2.0f * (qx * qx + qz * qz);
	R(2, 2) = 1.0f - 2.0f * (qx * qx + qy * qy);

	R(1, 0) = 2.0f * (qx * qy - Cos * qz);
	R(2, 0) = 2.0f * (qx * qz + Cos * qy);
	
	R(0, 1) = 2.0f * (qx * qy + Cos * qz);
	R(2, 1) = 2.0f * (qy * qz - Cos * qx);
	
	R(0, 2) = 2.0f * (qx * qz - Cos * qy);
	R(1, 2) = 2.0f * (qy * qz + Cos * qx);
	
	cAssert(cVec3::Dot(cVec3::Transform(Axis, R), Axis) >= 1.0f - 2.0f * cMath::Epsilon);

	return R;
}

// cMat3::ToVectors
void cMat3::ToVectors(cVec3 *Forward, cVec3 *Right /*= NULL*/, cVec3 *Up /*= NULL*/) const {
	if(Forward) {
		*Forward = -GetRow2(); // Negative Z - Axis
	}
	if(Right) {
		*Right = GetRow0(); // X - Axis
	}
	if(Up) {
		*Up = GetRow1(); // Y - Axis
	}
}

// cMat3::ToForward
const cVec3 cMat3::ToForward() const {
	return -GetRow2(); // Negative Z - Axis
}

// cMat3::GetScale
const cVec3 cMat3::GetScale() const {
	return cVec3(m_Rows[0].Length(), m_Rows[1].Length(), m_Rows[2].Length());
}

//-----------------------------------------------------------------------------
// cMat3::ToQuat
//-----------------------------------------------------------------------------
const cQuat cMat3::ToQuat() const {
	cQuat q;

	const float Trace = m_Rows[0][0] + m_Rows[1][1] + m_Rows[2][2];
	if(Trace > 0.0f) {
		const float t = Trace + 1.0f;
		const float s = 1.0f / cMath::Sqrt(t) * 0.5f;

		q[3] = s * t;
		q[0] = (m_Rows[1][2] - m_Rows[2][1]) * s;
		q[1] = (m_Rows[2][0] - m_Rows[0][2]) * s;
		q[2] = (m_Rows[0][1] - m_Rows[1][0]) * s;
	} else {
		int i = 0;
		if(m_Rows[1][1] > m_Rows[0][0]) {
			i = 1;
		}
		if(m_Rows[2][2] > m_Rows[i][i]) {
			i = 2;
		}
		
		const int Next[3] = { 1, 2, 0 };
		int j = Next[i];
		int k = Next[j];

		const float t = (m_Rows[i][i] - (m_Rows[j][j] + m_Rows[k][k])) + 1.0f;
		const float s = 1.0f / cMath::Sqrt(t) * 0.5f;

		q[i] = s * t;
		q[3] = (m_Rows[j][k] - m_Rows[k][j]) * s;
		q[j] = (m_Rows[i][j] + m_Rows[j][i]) * s;
		q[k] = (m_Rows[i][k] + m_Rows[k][i]) * s;
	}
	return q;
} // cMat3::ToQuat

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

//--------------------------------------------------------------------------------------
// cMat3::Invert : bool (const cMat3 &, cMat3 *)
//--------------------------------------------------------------------------------------
bool cMat3::Invert(const cMat3 &Fm, cMat3 *To) {
	const float d2_12_01 = Fm[1][0] * Fm[2][1] - Fm[1][1] * Fm[2][0];
	const float d2_12_02 = Fm[1][0] * Fm[2][2] - Fm[1][2] * Fm[2][0];
	const float d2_12_12 = Fm[1][1] * Fm[2][2] - Fm[1][2] * Fm[2][1];
	
	const float Det =  Fm[0][0] * d2_12_12 - Fm[0][1] * d2_12_02 + Fm[0][2] * d2_12_01;

	if(cMath::IsZero(Det, cMath::MatrixInvertEpsilon)) {
		return false;
	}

	const float iDet = 1.0f / Det;

	const float d2_01_01 = Fm[0][0] * Fm[1][1] - Fm[0][1] * Fm[1][0];
	const float d2_01_02 = Fm[0][0] * Fm[1][2] - Fm[0][2] * Fm[1][0];
	const float d2_01_12 = Fm[0][1] * Fm[1][2] - Fm[0][2] * Fm[1][1];

	const float d2_02_01 = Fm[0][0] * Fm[2][1] - Fm[0][1] * Fm[2][0];
	const float d2_02_02 = Fm[0][0] * Fm[2][2] - Fm[0][2] * Fm[2][0];
	const float d2_02_12 = Fm[0][1] * Fm[2][2] - Fm[0][2] * Fm[2][1];


	To->Elem(0, 0) = + d2_12_12 * iDet;
	To->Elem(1, 0) = - d2_12_02 * iDet;
	To->Elem(2, 0) = + d2_12_01 * iDet;

	To->Elem(0, 1) = - d2_02_12 * iDet;
	To->Elem(1, 1) = + d2_02_02 * iDet;
	To->Elem(2, 1) = - d2_02_01 * iDet;

	To->Elem(0, 2) = + d2_01_12 * iDet;
	To->Elem(1, 2) = - d2_01_02 * iDet;
	To->Elem(2, 2) = + d2_01_01 * iDet;
	
	return true;
} // cMat3::Invert

//-----------------------------------------------------------------------------
// cMat3::ToAngles : cAngles() const
//-----------------------------------------------------------------------------
cAngles cMat3::ToAngles() const {
	cAssert(IsOrthonormal());

	//														  ( CR*CY+SR*SP*SY	CP*SR	CY*SR*SP-CR*SY )
	//	RotationZ(Roll) * RotationX(Pitch) * RotationY(Yaw) = ( CR*SP*SY-CY*SR	CR*CP	CR*CY*SP+SR*SY )
	//														  ( CP*SY			-SP		CP*CY		   )

	cAngles Angles;
	Angles.Pitch = cMath::Deg(cMath::ASin(-m_Rows[2][1]));
	if(Angles.Pitch < 90.0f) {
		if(Angles.Pitch > -90.0f) {
			Angles.Yaw = cMath::Deg(cMath::ATan(m_Rows[2][0], m_Rows[2][2]));
			Angles.Roll = cMath::Deg(cMath::ATan(m_Rows[0][1], m_Rows[1][1]));
		} else {
			Angles.Yaw = cMath::Deg(-cMath::ATan(-m_Rows[1][0], m_Rows[0][0]));
			Angles.Roll = 0.0f;
		}
	} else {
		Angles.Yaw = cMath::Deg(cMath::ATan(-m_Rows[1][0], m_Rows[0][0]));
		Angles.Roll = 0.0f;
	}
	return Angles;
} // cMat3::ToAngles

//-----------------------------------------------------------------------------------------------
// cMat3::OfRotation
//-----------------------------------------------------------------------------------------------
const cMat3 cMat3::OfRotation(const cVec3 &xToAxis, const cVec3 &yToAxis, const cVec3 &zToAxis,
							  const cVec3 &xFmAxis, const cVec3 &yFmAxis, const cVec3 &zFmAxis) {
	cMat3 R;
	R.SetRow0(cVec3::Dot(xToAxis, xFmAxis), cVec3::Dot(xToAxis, yFmAxis), cVec3::Dot(xToAxis, zFmAxis));
	R.SetRow1(cVec3::Dot(yToAxis, xFmAxis), cVec3::Dot(yToAxis, yFmAxis), cVec3::Dot(yToAxis, zFmAxis));
	R.SetRow2(cVec3::Dot(zToAxis, xFmAxis), cVec3::Dot(zToAxis, yFmAxis), cVec3::Dot(zToAxis, zFmAxis));
	return R;
}

