#include "comms.h"

//-----------------------------------------------------------------------------
// cRotation::ToAngles : cAngles() const
//-----------------------------------------------------------------------------
cAngles cRotation::ToAngles() const {
	return ToMat3().ToAngles();
}

//-----------------------------------------------------------------------------
// cRotation::ToQuat : cQuat() const
//-----------------------------------------------------------------------------
cQuat cRotation::ToQuat() const {
	float AngleOver2 = cMath::Rad(m_Angle) * 0.5f;
	float Sin, Cos;
	cMath::SinCos(AngleOver2, Sin, Cos);
	return cQuat(m_Axis.x * Sin, m_Axis.y * Sin, m_Axis.z * Sin, Cos);
}

//-----------------------------------------------------------------------------
// cRotation::ToMat3 : const cMat3 & () const
//-----------------------------------------------------------------------------
const cMat3 & cRotation::ToMat3() const {
	if(m_CachedMatrixIsValid) {
		return m_CachedMatrix;
	}

	float AngleOver2 = cMath::Rad(m_Angle) * 0.5f;
	float Sin, Cos;
	cMath::SinCos(AngleOver2, Sin, Cos);

	float x = m_Axis.x * Sin;
	float y = m_Axis.y * Sin;
	float z = m_Axis.z * Sin;

	float x2 = x + x;
	float y2 = y + y;
	float z2 = z + z;

	float xx = x * x2;
	float xy = x * y2;
	float xz = x * z2;

	float yy = y * y2;
	float yz = y * z2;
	float zz = z * z2;
	
	float wx = Cos * x2;
	float wy = Cos * y2;
	float wz = Cos * z2;

	m_CachedMatrix[0][0] = 1.0f - (yy + zz);
	m_CachedMatrix[0][1] = xy - wz;
	m_CachedMatrix[0][2] = xz + wy;

	m_CachedMatrix[1][0] = xy + wz;
	m_CachedMatrix[1][1] = 1.0f - (xx + zz);
	m_CachedMatrix[1][2] = yz - wx;

	m_CachedMatrix[2][0] = xz - wy;
	m_CachedMatrix[2][1] = yz + wx;
	m_CachedMatrix[2][2] = 1.0f - (xx + yy);

	m_CachedMatrixIsValid = true;
	return m_CachedMatrix;
}

//-----------------------------------------------------------------------------
// cRotation::ToMat4 : cMat4() const
//-----------------------------------------------------------------------------
cMat4 cRotation::ToMat4() const {
	return ToMat3().ToMat4();
}

//-----------------------------------------------------------------------------
// cRotation::ToAngularVelocity : cVec3() const
//-----------------------------------------------------------------------------
cVec3 cRotation::ToAngularVelocity() const {
	return m_Axis * cMath::Rad(m_Angle);
}

//-----------------------------------------------------------------------------
// cRotation::Normalize180 : cRotation & ()
//-----------------------------------------------------------------------------
cRotation & cRotation::Normalize180() {
	m_Angle -= cMath::Floor(m_Angle / 360.0f) * 360.0f;
	if(m_Angle > 180.0f) {
		m_Angle -= 360.0f;
	} else if(m_Angle < - 180.0f) {
		m_Angle += 360.0f;
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cRotation::Normalize360 : cRotation & ()
//-----------------------------------------------------------------------------
cRotation & cRotation::Normalize360() {
	m_Angle -= cMath::Floor(m_Angle / 360.0f) * 360.0f;
	if(m_Angle > 360.0f) {
		m_Angle -= 360.0f;
	} else if(m_Angle < 0.0f) {
		m_Angle += 360.0f;
	}
	return *this;
}