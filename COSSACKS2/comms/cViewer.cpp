#include "comms.h"

//*****************************************************************************
// Viewer
//****************************************************************************

//-----------------------------------------------------------------------------
// cViewer::CtorArgs::SetDefaults
//-----------------------------------------------------------------------------
void cViewer::CtorArgs::SetDefaults(const eUpAxis ua) {
	Pos.SetZero();
	Orient.SetZero();
	Fov = 60.0f;
	zNear = 0.1f;
	zFar = 2000.0f;
	UpAxis = ua;
	IsRightHanded = true;
	IsInfinite = false;
} // cViewer::CtorArgs::SetDefaults

//-----------------------------------------------------------------------------
// cViewer.ctor
//-----------------------------------------------------------------------------
cViewer::cViewer(const CtorArgs &Args /*= CtorArgs()*/) {
	SetName("Viewer");
	
	m_UpAxis = Args.UpAxis;
	m_Pos = Args.Pos;
	m_Orient = Args.Orient;
	
	m_Fov = Args.Fov;
	m_zNear = Args.zNear;
	m_zFar = Args.zFar;
	m_ViewPort.Set(0, 0, 640, 480);
	
	m_IsRightHanded = Args.IsRightHanded;
	m_IsInfinite = Args.IsInfinite;
	
	Update();
} // cViewer.ctor

//-----------------------------------------------------------------------------
// cViewer::Update
//-----------------------------------------------------------------------------
void cViewer::Update() {
	if(m_UpAxis == UpAxisY) {
		m_Orient.ToVectors(&m_Forward, &m_Right, &m_Up);
	} else {
		cAssert(m_UpAxis == UpAxisZ);
		m_Orient.ToVectorsZ(&m_Forward, &m_Right, &m_Up);
	}
	
	if(!m_IsRightHanded) {
		m_Forward *= -1.0f;
	}
	
	// World2View && View2World:
	m_World2View.SetIdentity();
	m_World2View.SetCol0(cVec4(m_Right, -cVec3::Dot(m_Right, m_Pos)));
	m_World2View.SetCol1(cVec4(m_Up, -cVec3::Dot(m_Up, m_Pos)));
	m_World2View.SetCol2(cVec4(-m_Forward, -cVec3::Dot(-m_Forward, m_Pos)));
	m_View2World = cMat4(cMat3(cMat3::RowsCtor, m_Right, m_Up, -m_Forward), m_Pos);
	
	m_Aspect = (float)m_ViewPort.GetWidth() / (float)m_ViewPort.GetHeight();
	if(m_IsInfinite) {
		m_View2Proj = cMat4::PerspectiveYFovRHInf(m_Fov, m_Aspect, m_zNear);
	} else {
		m_View2Proj = cMat4::PerspectiveYFovRH(m_Fov, m_Aspect, m_zNear, m_zFar);
	}
	cMat4::Invert(m_View2Proj, &m_Proj2View);
	
	m_Proj2Screen.SetIdentity();
	m_Proj2Screen(0, 0) = (float)m_ViewPort.GetWidth() * 0.5f;
	m_Proj2Screen(1, 1) = -(float)m_ViewPort.GetHeight() * 0.5f;
	m_Proj2Screen(3, 0) = (float)m_ViewPort.GetWidth() * 0.5f + (float)m_ViewPort.GetLeft();
	m_Proj2Screen(3, 1) = (float)m_ViewPort.GetHeight() * 0.5f + (float)m_ViewPort.GetTop();
	
	m_Screen2Proj.SetIdentity();
	m_Screen2Proj(0, 0) = 2.0f / (float)m_ViewPort.GetWidth();
	m_Screen2Proj(1, 1) = -2.0f / (float)m_ViewPort.GetHeight();
	m_Screen2Proj(3, 0) = -2.0f * (float)m_ViewPort.GetLeft() / (float)m_ViewPort.GetWidth() - 1.0f;
	m_Screen2Proj(3, 1) = 2.0f * (float)m_ViewPort.GetTop() / (float)m_ViewPort.GetHeight() + 1.0f;

	m_World2Proj = cMat4::Mul(m_World2View, m_View2Proj);
	m_Proj2World = cMat4::Mul(m_Proj2View, m_View2World);

	m_World2Screen = cMat4::Mul(m_World2Proj, m_Proj2Screen);
	m_Screen2World = cMat4::Mul(m_Screen2Proj, m_Proj2World);

	m_View2Screen = cMat4::Mul(m_View2Proj, m_Proj2Screen);
	m_Screen2View = cMat4::Mul(m_Screen2Proj, m_Proj2View);

	// Building frustum:
	cVec4 d[6];

	d[0] = m_World2Proj.GetCol3() - m_World2Proj.GetCol0(); // Right plane
	d[1] = m_World2Proj.GetCol3() + m_World2Proj.GetCol0(); // Left plane
	d[2] = m_World2Proj.GetCol3() - m_World2Proj.GetCol1(); // Top plane
	d[3] = m_World2Proj.GetCol3() + m_World2Proj.GetCol1(); // Bottom plane
	d[4] = m_World2Proj.GetCol3() - m_World2Proj.GetCol2(); // Far plane
	d[5] = m_World2Proj.GetCol3() + m_World2Proj.GetCol2(); // Near plane

	for(int i = 0; i < 6; i++) {
		const float l = d[i].ToVec3().Length();
		if(l != 0.0f) {
			d[i] /= l;
		}
		m_Planes[i].SetNormal(d[i].ToVec3());
		m_Planes[i].SetDistanceToOrig(-d[i].w);
	}
} // cViewer::Update

//-----------------------------------------------------------------------------
// cViewer::CullPoint
//-----------------------------------------------------------------------------
bool cViewer::CullPoint(const cVec3 &p) const {
	for(int i = 0; i < 6; i++) {
		if(m_Planes[i].SignedDistanceToPoint(p) < 0.0f) {
			return true;
		}
	}
	return false;
} // cViewer::CullPoint

//-----------------------------------------------------------------------------
// cViewer::CullBounds
//-----------------------------------------------------------------------------
bool cViewer::CullBounds(const cBounds &b) const {
	cVec3 Points[8];
	b.ToPoints(Points);
	for(int p = 0; p < 8; p++) {
		for(int i = 0; i < 6; i++) {
			if(m_Planes[i].SignedDistanceToPoint(Points[p]) > 0.0f) {
				return false;
			}
		}
	}
	return true;
} // cViewer::CullBounds

// cViewer::GetPickRay
const cSeg cViewer::GetPickRay(const int xScreen, const int yScreen) const {
	cVec3 Fm((float)xScreen, (float)yScreen, 0.0f);
	Fm.TransformCoordinate(m_Screen2World);
	return cSeg(cSeg::RayCtor, Fm, Fm - m_Pos);
}

void cViewer::SetUpAxis(const eUpAxis UpAxis) {
	m_UpAxis = UpAxis;
	Update();
}

//*****************************************************************************
// FreeViewer
//*****************************************************************************

//-----------------------------------------------------------------------------
// cFreeViewer::CtorArgs::SetDefaults
//-----------------------------------------------------------------------------
void cFreeViewer::CtorArgs::SetDefaults(const eUpAxis ua) {
	cViewer::CtorArgs::SetDefaults(ua);
	UpAxis = ua;
	DefLookAt.SetZero();
	DefToEye = 20.0f;
	if(ua == UpAxisY) {
		DefOrient.Set(-28, 45, 0);
	} else if(ua == UpAxisZ) {
		DefOrient.Set(62, 135, 0);
	}
} // cFreeViewer::CtorArgs::SetDefaults

//---------------------------------------------------------------------------------------------------------
// cFreeViewer.ctor
//---------------------------------------------------------------------------------------------------------
cFreeViewer::cFreeViewer(const CtorArgs &Args /*= CtorArgs()*/) : cViewer(cViewer::CtorArgs(Args.UpAxis)) {
	SetName("FreeViewer");
	
	m_DefLookAt = Args.DefLookAt;
	m_DefOrient = Args.DefOrient;
	m_DefToEye = Args.DefToEye;
	Home();

	m_Mode = Mode::None;
} // cFreeViewer.ctor

//-----------------------------------------------------------------------------
// cFreeViewer::Update
//-----------------------------------------------------------------------------
void cFreeViewer::Update() {
	const cVec3 Forward = m_UpAxis == UpAxisY ? m_Orient.ToForward() : m_Orient.ToForwardZ();
	m_Pos = m_LookAt - Forward * m_ToEye;
	cViewer::Update();
} // cFreeViewer::Update

//-----------------------------------------------------------------------------
// cFreeViewer::Home
//-----------------------------------------------------------------------------
void cFreeViewer::Home() {
	m_LookAt = m_DefLookAt;
	m_Orient = m_DefOrient;
	m_ToEye = m_DefToEye;
	m_DrawTargetUntilAlt = false;
	Update();
} // cFreeViewer::Home

//----------------------------------------------------------------------------------
// cFreeViewer::Track
//----------------------------------------------------------------------------------
void cFreeViewer::Track(const int Dx, const int Dy, const float Step /*= 0.001f*/) {
	const float Fr = (float)Dx * Step * (10.0f + m_ToEye);
	const float Fu = (float)Dy * Step * (10.0f + m_ToEye);
	m_LookAt -= Fr * GetRight();
	m_LookAt += Fu * GetUp();
	Update();
} // cFreeViewer::Track

//-----------------------------------------------------------------------------------
// cFreeViewer::Tumble
//-----------------------------------------------------------------------------------
void cFreeViewer::Tumble(const int Dx, const int Dy, const float dAngle /*= 0.3f*/) {
	m_Orient.Yaw += (IsRightHanded() ? -1.0f : 1.0f) * (float)Dx * dAngle;
	m_Orient.Pitch -= (float)Dy * dAngle;
	Update();
} // cFreeViewer::Tumble

//--------------------------------------------------------------------------------------------------------------------
// cFreeViewer::Dolly
//--------------------------------------------------------------------------------------------------------------------
void cFreeViewer::Dolly(const int Dx, const int Dy, const float Step /*= 0.2f*/, const float MinDistance /*= 0.0f*/) {
	m_ToEye -= Step * (Dx + Dy);
	if(m_ToEye < MinDistance) {
		const float r = MinDistance - m_ToEye;
		m_ToEye = MinDistance;
		m_LookAt += r * GetForward();
	}
	Update();
} // cFreeViewer::Dolly

//---------------------------------------------------------------------------------
// cFreeViewer::Roll
//---------------------------------------------------------------------------------
void cFreeViewer::Roll(const int Dx, const int Dy, const float dAngle /*= 0.3f*/) {
	m_Orient.Roll -= (IsRightHanded() ? -1.0f : 1.0f) * dAngle * (float)(Dx + Dy);
	Update();
} // cFreeViewer::Roll

//---------------------------------------------------------------------------------------------------------
// cFreeViewer::Aim
//---------------------------------------------------------------------------------------------------------
void cFreeViewer::Aim(const int Delta, const float Speed /*= 1.2f*/, const float MinDistance /*= 0.0f*/) {
	m_DrawTargetUntilAlt = true;
	const float PrevToEye = m_ToEye;
	m_ToEye = cMath::Clamp(m_ToEye + Speed * (float)Delta, MinDistance, cMath::Infinity);
	const float d = m_ToEye - PrevToEye;
	m_LookAt += d * GetForward();
	Update();
} // cFreeViewer::Aim

//-----------------------------------------------------------------------------
// cFreeViewer::OnButtonDown
//-----------------------------------------------------------------------------
bool cFreeViewer::OnButtonDown(const int Code) {
	if(m_Mode != Mode::None) {
		return false;
	}
	if(Code == cInput::Home) {
		Home();
		return true;
	}
	if(cInput::IsDown(cInput::Alt)) {
		if(Code == cInput::LeftButton) {
			m_Mode = Mode::Tumble;
		} else if(Code == cInput::RightButton) {
			m_Mode = Mode::Dolly;
		} else if(Code == cInput::MiddleButton) {
			m_Mode = Mode::Track;
		}
	}
	if(m_Mode != Mode::None) {
		cWidgets::SetCapture();
		return true;
	}
	return false;
} // cFreeViewer::OnButtonDown

//-----------------------------------------------------------------------------
// cFreeViewer::OnMouseMove
//-----------------------------------------------------------------------------
bool cFreeViewer::OnMouseMove(const int DeltaX, const int DeltaY) {
	if(Mode::None == m_Mode) {
		return false;
	}
	switch(m_Mode) {
		case Mode::Tumble:
			if(!cInput::IsDown(cInput::LeftButton)) {
				m_Mode = Mode::None;
			} else {
				if(cInput::IsDown(cInput::RightButton)) {
					Roll(DeltaX, DeltaY);
				} else {
					Tumble(DeltaX, DeltaY);
				}
			}
			break;
		case Mode::Dolly:
			if(!cInput::IsDown(cInput::RightButton)) {
				m_Mode = Mode::None;
			} else {
				if(cInput::IsDown(cInput::LeftButton)) {
					Roll(DeltaX, DeltaY);
				} else {
					Dolly(DeltaX, DeltaY);
				}
			}
			break;
		case Mode::Track:
			if(!cInput::IsDown(cInput::MiddleButton)) {
				m_Mode = Mode::None;
			} else {
				Track(DeltaX, DeltaY);
			}
			break;
		default:
			cAssertM(0, "Illegal Mode");
			break;
	}
	if(Mode::None == m_Mode) {
		cWidgets::ReleaseCapture();
		return false;
	}
	return true;
} // cFreeViewer::OnMouseMove

//-----------------------------------------------------------------------------
// cFreeViewer::OnButtonUp
//-----------------------------------------------------------------------------
bool cFreeViewer::OnButtonUp(const int Code) {
	if(Mode::None == m_Mode) {
		return false;
	}
	switch(m_Mode) {
		case Mode::Tumble:
			if(Code == cInput::LeftButton) {
				m_Mode = Mode::None;
			}
			break;
		case Mode::Dolly:
			if(Code == cInput::RightButton) {
				m_Mode = Mode::None;
			}
			break;
		case Mode::Track:
			if(Code == cInput::MiddleButton) {
				m_Mode = Mode::None;
			}
			break;
		default:
			cAssertM(0, "Illegal Mode");
			break;
	}
	if(Mode::None == m_Mode) {
		cWidgets::ReleaseCapture();
		return true;
	}
	return false;
} // cFreeViewer::OnButtonUp

//-----------------------------------------------------------------------------
// cFreeViewer::OnMouseWheel
//-----------------------------------------------------------------------------
bool cFreeViewer::OnMouseWheel(const int Delta) {
	if(cInput::IsDown(cInput::Alt)) {
		Aim(Delta);
		return true;
	}
	return false;
} // cFreeViewer::OnMouseWheel

/*
		case WM_MOUSEWHEEL:
			if(GetKeyState(VK_MENU) < 0) {
				Aim((short)HIWORD(wParam));
				return true;
			}
			*/

//-----------------------------------------------------------------------------
// cFreeViewer::OnRender
//-----------------------------------------------------------------------------
void cFreeViewer::OnRender() {
	if(m_Mode != Mode::None && m_DrawTargetUntilAlt) {
		m_DrawTargetUntilAlt = false;
	}
	if(m_DrawTargetUntilAlt && !cInput::IsDown(cInput::Alt)) {
		m_DrawTargetUntilAlt = false;
	}
	if(m_Mode == Mode::None && !m_DrawTargetUntilAlt) {
		return;
	}

	const float l = 1.0;
	const cColor Color(cColor::RgbaCtor, 0xee, 0xcc, 0x55, 0xff);
	
	const cVec3 Cross[] = {
		/* x: */ cVec3(m_LookAt.x - l, m_LookAt.y, m_LookAt.z), cVec3(m_LookAt.x + l, m_LookAt.y, m_LookAt.z),
		/* y: */ cVec3(m_LookAt.x, m_LookAt.y - l, m_LookAt.z), cVec3(m_LookAt.x, m_LookAt.y + l, m_LookAt.z),
		/* z: */ cVec3(m_LookAt.x, m_LookAt.y, m_LookAt.z - l), cVec3(m_LookAt.x, m_LookAt.y, m_LookAt.z + l)
	};
	
	for(int i = 0; i < 3; i++) {
		cRender::GetInstance()->DrawLine(Cross[2 * i + 0], Cross[2 * i + 1], Color, 2.0f);
	}
} // cFreeViewer::OnRender