#include "comms.h"

//*****************************************************************************
// MoveTool
//*****************************************************************************

//-----------------------------------------------------------------------------
// cMoveTool.ctor
//-----------------------------------------------------------------------------
cMoveTool::cMoveTool() {
	SetName("MoveTool");
	
	m_Mode = MODE_NONE;
	
	// Size:
	m_AxisLen = 82.0f;
	m_MaxSelDistance = 0.12f;

	// Axis Cull:
	m_MaxVisibleDot = 0.98f;
	
	m_pRotate = NULL;
	m_pTranslate = NULL;
	m_Space = m_DesiredSpace = SPACE_WORLD;
} // cMoveTool.ctor

//-----------------------------------------------------------------------------
// cMoveTool::Bind
//-----------------------------------------------------------------------------
void cMoveTool::Bind(cVec3 *pTranslate) {
	m_pRotate = NULL;
	m_pTranslate = pTranslate;
	m_Space = m_DesiredSpace = SPACE_WORLD;
} // cMoveTool::Bind

//-----------------------------------------------------------------------------
// cMoveTool::SetSpace
//-----------------------------------------------------------------------------
void cMoveTool::SetSpace(const SPACE Space) {
	if(!m_pRotate) {
		cAssert(m_Space == SPACE_WORLD && m_DesiredSpace == SPACE_WORLD);
		return;
	}
	m_DesiredSpace = Space;
	if(MODE_NONE == m_Mode) {
		m_Space = m_DesiredSpace;
	}
} // cMoveTool::SetSpace

//-----------------------------------------------------------------------------
// cMoveTool::Bind
//-----------------------------------------------------------------------------
void cMoveTool::Bind(cQuat *pRotate, cVec3 *pTranslate, const SPACE Space) {
	Bind(pTranslate);
	m_pRotate = pRotate;
	SetSpace(Space);
} // cMoveTool::Bind

//-----------------------------------------------------------------------------
// cMoveTool::CalcFrame
//-----------------------------------------------------------------------------
bool cMoveTool::CalcFrame() {
	if(!m_pTranslate) { 
		return false;
	}
	// CurPos:
	m_Frame.CurPos = *m_pTranslate;
	
	// AxisLen:
	cVec3 u = cVec3::TransformCoordinate(m_Frame.CurPos, cRender::GetInstance()->GetViewer()->GetViewProjectionScreenMatrix());
	u.x += m_AxisLen;
	u.TransformCoordinate(cRender::GetInstance()->GetViewer()->GetViewProjectionScreenMatrixInverse());
	u -= m_Frame.CurPos;
	const float AxisLen = u.Length();

	// Axis (0..2):
	cVec3 AxisNormal;
	for(int iAxis = 0; iAxis < 3; iAxis++) {
		AxisNormal.SetZero();
		AxisNormal[iAxis] = 1.0f;
		if(SPACE_OBJECT == m_Space) {
			cAssert(m_pRotate);
			AxisNormal.Rotate(*m_pRotate);
		}
		m_Frame.Axis[iAxis].SetFromEnds(m_Frame.CurPos, m_Frame.CurPos + AxisNormal * AxisLen);
	}

	m_Frame.MaxSelDistance = m_MaxSelDistance * AxisLen;

	// Update segs visibility only in "None" mode:
	if(MODE_NONE == m_Mode) {
		const cVec3 ToEye = (cRender::GetInstance()->GetViewer()->GetPos() - m_Frame.CurPos).ToNormal();
		for(int iAxis = 0; iAxis < 3; iAxis++) {
			const float d = cMath::Abs(cVec3::Dot(ToEye, m_Frame.Axis[iAxis].GetNormal()));
			m_Frame.AxisIsVisible[iAxis] = d <= m_MaxVisibleDot;
		}
	}
	return true;
} // cMoveTool::CalcFrame

//-----------------------------------------------------------------------------
// cMoveTool::OnRender
//-----------------------------------------------------------------------------
void cMoveTool::OnRender() {
	if(!CalcFrame()) { // Tool is not binded.
		return;
	}
	
	// Colors:
	const cColor TraceColor = cColor::Gray50;
	const cColor SelColor = cColor::Yellow;
	const cColor CenterColor = MODE_CENTER == m_Mode ? SelColor : cColor::White;
	const cColor AxisColor[3] = {
		MODE_ALONG_X == m_Mode ? SelColor : cColor::Red,
		MODE_ALONG_Y == m_Mode ? SelColor : cColor::Green,
		MODE_ALONG_Z == m_Mode ? SelColor : cColor::Blue
	};

	// Appearance:
	const float LineWidth = 2.0f;
	const float HeadLenFr = 0.3f;
	const float CurHeadLen = HeadLenFr * m_Frame.Axis[0].GetLength();
	const float HeadRadius = 0.06f * m_Frame.Axis[0].GetLength();
	const int CenterSubDivs = 12;
	const int ArrowSubDivs = 12;
	const float LabelShift = 0.2f * m_Frame.Axis[0].GetLength();
	
	const bool ShowAxis[3] = {
		m_Frame.AxisIsVisible[0] && m_Mode != MODE_ALONG_Y && m_Mode != MODE_ALONG_Z,
		m_Frame.AxisIsVisible[1] && m_Mode != MODE_ALONG_X && m_Mode != MODE_ALONG_Z,
		m_Frame.AxisIsVisible[2] && m_Mode != MODE_ALONG_X && m_Mode != MODE_ALONG_Y
	};
	const cVec3 LineEnds[3] = {
		ShowAxis[0] ? m_Frame.Axis[0].GetTo() - CurHeadLen * m_Frame.Axis[0].GetNormal() : cVec3::Zero,
		ShowAxis[1] ? m_Frame.Axis[1].GetTo() - CurHeadLen * m_Frame.Axis[1].GetNormal() : cVec3::Zero,
		ShowAxis[2] ? m_Frame.Axis[2].GetTo() - CurHeadLen * m_Frame.Axis[2].GetNormal() : cVec3::Zero
	};
	
	// Drawing Traces in any move mode (only in move mode StartAxis[0..2] are valid):
	if(m_Mode != MODE_NONE) {
		const float TraceQuadSide = 0.1f * m_Frame.StartAxis[0].GetLength();
		const float StartHeadLen = HeadLenFr * m_Frame.StartAxis[0].GetLength();
		const cVec3 StartLineEnds[3] = {
			ShowAxis[0] ? m_Frame.StartAxis[0].GetTo() - StartHeadLen * m_Frame.StartAxis[0].GetNormal() : cVec3::Zero,
			ShowAxis[1] ? m_Frame.StartAxis[1].GetTo() - StartHeadLen * m_Frame.StartAxis[1].GetNormal() : cVec3::Zero,
			ShowAxis[2] ? m_Frame.StartAxis[2].GetTo() - StartHeadLen * m_Frame.StartAxis[2].GetNormal() : cVec3::Zero
		};
		
		for(int iAxis = 0; iAxis < 3; iAxis++) {
			if(!ShowAxis[iAxis]) {
				continue;
			}
			// Line Trace:
			cRender::GetInstance()->DrawLine(m_Frame.StartPos, StartLineEnds[iAxis], TraceColor, LineWidth);
			// Quad Trace:
			cRender::GetInstance()->DrawBillboardQuad(StartLineEnds[iAxis], TraceQuadSide, TraceColor);
		}
		// Center Trace:
		cRender::GetInstance()->DrawBillboardQuad(m_Frame.StartPos, TraceQuadSide, TraceColor);
	}

	// Drawing Axes:
	for(int iAxis = 0; iAxis < 3; iAxis++) {
		if(!ShowAxis[iAxis]) {
			continue;
		}
		// Line:
		cRender::GetInstance()->DrawLine(m_Frame.CurPos, LineEnds[iAxis], AxisColor[iAxis], LineWidth);
		// Arrow:
		if(m_Mode != MODE_CENTER) {
			cRender::GetInstance()->DrawCone(m_Frame.Axis[iAxis].GetTo(), m_Frame.Axis[iAxis].GetNormal(), HeadRadius, CurHeadLen, AxisColor[iAxis], ArrowSubDivs);
		}
	}

	// Center:
	cRender::GetInstance()->DrawBillboardCircle(m_Frame.CurPos, m_Frame.MaxSelDistance, CenterColor, CenterSubDivs, LineWidth);

	// Drawing labels in any move mode:
	if(m_Mode != MODE_NONE) {
		const cVec3 Shift = m_Frame.CurPos - m_Frame.StartPos;
		for(int iAxis = 0; iAxis < 3; iAxis++) {
			if(!ShowAxis[iAxis]) {
				continue;
			}
			cStr Label(cStr::csYellow);
			Label << cStr::FromFloat(Shift[iAxis]);
			const cVec3 LabelPos = m_Frame.Axis[iAxis].GetTo() + cRender::GetInstance()->GetViewer()->GetUp() * LabelShift;
			cRender::GetInstance()->DrawStrCentered(Label, LabelPos);
		}
	}
} // cMoveTool::OnRender

//-----------------------------------------------------------------------------
// cMoveTool::OnButtonDown
//-----------------------------------------------------------------------------
bool cMoveTool::OnButtonDown(const int Code) {
	if(Code != cInput::LeftButton) {
		return false;
	}
	if(!CalcFrame()) { // Tool is not binded.
		return false;
	}

	cAssert(MODE_NONE == m_Mode);
	
	const int xMouse = cInput::GetMouseX();
	const int yMouse = cInput::GetMouseY();
	const cSeg PickRay = cRender::GetInstance()->GetViewer()->GetPickRay(xMouse, yMouse);

	cVec3 PickedPoint[4], q;
	float D[4]; // 0..2 - distance from pick ray to axis seg, 3 - distance from pick ray to center along view plane.
	int iInvisibleAxis = -1;
	for(int iAxis = 0; iAxis < 3; iAxis++) {
		if(!m_Frame.AxisIsVisible[iAxis]) {
			D[iAxis] = cMath::Infinity;
			iInvisibleAxis = iAxis;
			continue;
		}
		cSeg::ClosestPoints(cSeg::SegRay, m_Frame.Axis[iAxis], PickRay, PickedPoint[iAxis], q);
		D[iAxis] = cVec3::Distance(PickedPoint[iAxis], q);
	}
	const int iMin = cMath::MinIndex(D[0], D[1], D[2]);
	
	// Center plane (normal based on axis visibility):
	const cVec3 CenterNormal = iInvisibleAxis != -1 ? m_Frame.Axis[iInvisibleAxis].GetNormal() : cRender::GetInstance()->GetViewer()->GetForward();
	m_Frame.Center.SetFromPointAndNormal(m_Frame.CurPos, CenterNormal);
	if(m_Frame.Center.RayIntersection(PickRay.GetFm(), PickRay.GetNormal(), NULL, &PickedPoint[3])) {
		D[3] = cSeg::Distance(cSeg::Ray, PickRay, m_Frame.CurPos);
	} else {
		D[3] = cMath::Infinity;
	}

	if(D[3] < m_Frame.MaxSelDistance) { // Center has highest priority.
		m_Frame.PickedPoint = PickedPoint[3];
		m_Mode = MODE_CENTER;
	} else if(D[iMin] < m_Frame.MaxSelDistance) {
		m_Frame.PickedPoint = PickedPoint[iMin];
		m_Mode = (MODE)(MODE_ALONG_X + iMin);
	}
	
	if(m_Mode != MODE_NONE) {
		m_Frame.StartAxis[0] = m_Frame.Axis[0];
		m_Frame.StartAxis[1] = m_Frame.Axis[1];
		m_Frame.StartAxis[2] = m_Frame.Axis[2];
		m_Frame.StartPos = m_Frame.CurPos;
	}
	
	if(MODE_NONE == m_Mode) {
		return false;
	}
	// Capturing input in any move mode:
	cWidgets::SetCapture();
	return true;
} // cMoveTool::OnButtonDown

//-----------------------------------------------------------------------------
// cMoveTool::OnButtonUp
//-----------------------------------------------------------------------------
bool cMoveTool::OnButtonUp(const int Code) {
	if(MODE_NONE == m_Mode) {
		return false;
	}
	if(Code != cInput::LeftButton) {
		return false;
	}
	// Releasing capturing from any move mode:
	m_Mode = MODE_NONE;
	if(m_Space != m_DesiredSpace) {
		m_Space = m_DesiredSpace;
	}
	cWidgets::ReleaseCapture();
	return true;
} // cMoveTool::OnButtonUp

//-----------------------------------------------------------------------------
// cMoveTool::OnMouseMove
//-----------------------------------------------------------------------------
bool cMoveTool::OnMouseMove(const int DeltaX, const int DeltaY) {
	if(!cInput::IsDown(cInput::LeftButton)) {
		m_Mode = MODE_NONE;
		if(m_Space != m_DesiredSpace) {
			m_Space = m_DesiredSpace;
		}
		cWidgets::ReleaseCapture();
	}
	if(MODE_NONE == m_Mode) {
		return false;
	}
	
	const int xMouse = cInput::GetMouseX();
	const int yMouse = cInput::GetMouseY();
	const cSeg PickRay = cRender::GetInstance()->GetViewer()->GetPickRay(xMouse, yMouse);

	cVec3 CurPickedPoint = m_Frame.PickedPoint, q;
	if(MODE_CENTER == m_Mode) {
		m_Frame.Center.RayIntersection(PickRay.GetFm(), PickRay.GetNormal(), NULL, &CurPickedPoint);
	} else {
		const int iAxis = m_Mode - MODE_ALONG_X;
		cAssert(iAxis >= 0 && iAxis <= 2);
		cSeg::ClosestPoints(cSeg::LineRay, m_Frame.StartAxis[iAxis], PickRay, CurPickedPoint, q);
	}
	m_Frame.CurPos = m_Frame.StartPos + CurPickedPoint - m_Frame.PickedPoint;

	*m_pTranslate = m_Frame.CurPos;
	return true;
} // cMoveTool::OnMouseMove