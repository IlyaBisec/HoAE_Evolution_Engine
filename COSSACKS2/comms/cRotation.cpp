#include "comms.h"

//-----------------------------------------------------------------------------
// cRotation::ToAngles : cAngles() const
//-----------------------------------------------------------------------------
cAngles cRotation::ToAngles() const {
	return ToMat3().ToAngles();
}

//-----------------------------------------------------------------------------
// cRotation::ToQuat
//-----------------------------------------------------------------------------
const cQuat cRotation::ToQuat() const {
	float HalfAngle = cMath::Rad(m_Angle) * 0.5f;
	float Sin = cMath::Sin(HalfAngle);
	return cQuat(Sin * m_Axis.x, Sin * m_Axis.y, Sin * m_Axis.z, cMath::Cos(HalfAngle));
}

// cRotation::ToMat3
const cMat3 & cRotation::ToMat3() const {
	if(!m_CachedMatrixIsValid) {
		m_CachedMatrix = cMat3::RotationAxis(m_Axis, m_Angle);
		m_CachedMatrixIsValid = true;
	}
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