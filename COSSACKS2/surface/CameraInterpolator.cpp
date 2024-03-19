#include "stdheader.h"
#include "CameraInterpolator.h"
#include "vCamera.h"

Surface::CameraInterpolator g_CameraInterpolator;

namespace Surface {

//-----------------------------------------------------------------------------
// CameraInterpolator.ctor : ()
//-----------------------------------------------------------------------------
CameraInterpolator::CameraInterpolator() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// CameraInterpolator::IsActive : bool() const
//-----------------------------------------------------------------------------
bool CameraInterpolator::IsActive() const {
	return m_fActive;
}

//-----------------------------------------------------------------------------
// CameraInterpolator::Clear : void()
//-----------------------------------------------------------------------------
void CameraInterpolator::Clear() {
	m_TMs.Clear();
}

//-----------------------------------------------------------------------------
// CameraInterpolator::SaveCurCamera : void()
//-----------------------------------------------------------------------------
void CameraInterpolator::SaveCurCamera() {
	m_TMs.Add(ICam->GetWorldTM());
}

//-----------------------------------------------------------------------------
// CameraInterpolator::Play : void()
//-----------------------------------------------------------------------------
void CameraInterpolator::Play() {
	if(m_fActive) return;

	m_CurPos = 0;
	m_fActive = true;
}

//-----------------------------------------------------------------------------
// CameraInterpolator::Stop : void()
//-----------------------------------------------------------------------------
void CameraInterpolator::Stop() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// CameraInterpolator::SetupCamera : void()
//-----------------------------------------------------------------------------
void CameraInterpolator::SetupCamera() {
	if(GetKeyState(VK_INSERT) < 0 && m_TMs.Count() >= 2) { // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Play(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	} // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	if(!m_fActive) return;

	SetupCamera(m_CurPos);
	m_CurPos += 0.01f;
	if(m_CurPos > 1) {
		Stop();
	}
}

//-----------------------------------------------------------------------------
// CameraInterpolator::SetupCamera : void(float)
//-----------------------------------------------------------------------------
void CameraInterpolator::SetupCamera(float s) {
	if(m_TMs.Count() < 2) return;
	const Matrix4D &m0 = m_TMs[0];
	const Matrix4D &m1 = m_TMs[1];
	Quaternion q0, q1;
	q0.FromMatrix(m0);
	q1.FromMatrix(m1);
	Vector3D p0 = m0.getTranslation();
	Vector3D p1 = m1.getTranslation();
	Vector3D p = p0 + s * (p1 - p0);
	Quaternion q;
	q.Slerp(q0, q1, s);
	Matrix3D R(q);
	Matrix4D T(R);
	T.setTranslation(p);
	ICam->SetWorldTM(T);
}

}; // Surface