#include "comms.h"

//*****************************************************************************
// ScaleTool
//*****************************************************************************

//-----------------------------------------------------------------------------
// cScaleTool.ctor
//-----------------------------------------------------------------------------
cScaleTool::cScaleTool() {
	SetName("ScaleTool");
	
	m_Mode = MODE_NONE;
	
	// Size:
	m_LineLen = 82.0f;
	m_MaxSelDistance = 0.12f;

	// Axis cull:
	m_MaxVisibleDot = 0.98f;

	m_pScale = NULL;
	m_pRotate = NULL;
	m_pTranslate = NULL;
} // cScaleTool.ctor

//-----------------------------------------------------------------------------
// cScaleTool::Bind
//-----------------------------------------------------------------------------
void cScaleTool::Bind(cVec3 *pScale, cQuat *pRotate, cVec3 *pTranslate) {
	if(!pScale || !pRotate || !pTranslate) {
		m_pScale = NULL;
		m_pRotate = NULL;
		m_pTranslate = NULL;
	} else {
		m_pScale = pScale;
		m_pRotate = pRotate;
		m_pTranslate = pTranslate;
	}
} // cScaleTool::Bind

//-----------------------------------------------------------------------------
// cScaleTool::CalcFrame
//-----------------------------------------------------------------------------
bool cScaleTool::CalcFrame() {
	if(!m_pScale || !m_pRotate || !m_pTranslate) {
		return false;
	}
	
	// HandlePos[3] (Center)
	m_Frame.HandlePos[3] = *m_pTranslate;
	
	// LineLen:
	cVec3 u = cVec3::TransformCoordinate(m_Frame.HandlePos[3], cRender::GetInstance()->GetViewer()->GetViewProjectionScreenMatrix());
	u.x += m_LineLen;
	u.TransformCoordinate(cRender::GetInstance()->GetViewer()->GetViewProjectionScreenMatrixInverse());
	u -= m_Frame.HandlePos[3];
	m_Frame.LineLen = u.Length();
	
	// AxisNormal && HandlePos[0 - 2]:
	const cMat3 R = m_pRotate->ToMat3();
	for(int iAxis = 0; iAxis < 3; iAxis++) {
		float Sign = cMath::Sign((*m_pScale)[iAxis]);
		if(Sign == 0.0f) {
			Sign = 1.0f;
		}
		m_Frame.AxisNormal[iAxis] = Sign * R.GetRow(iAxis).ToNormal();
		m_Frame.HandlePos[iAxis] = m_Frame.HandlePos[3] + m_Frame.AxisNormal[iAxis] * m_Frame.LineLen;
	}
	m_Frame.AxisNormal[3] = cRender::GetInstance()->GetViewer()->GetRight();

	m_Frame.MaxSelDistance = m_MaxSelDistance * m_Frame.LineLen;
	
	const int xMouse = cInput::GetMouseX();
	const int yMouse = cInput::GetMouseY();
	m_Frame.PickRay = cRender::GetInstance()->GetViewer()->GetPickRay(xMouse, yMouse);

	// Update axes visibility:
	const cVec3 ToEye = (cRender::GetInstance()->GetViewer()->GetPos() - m_Frame.HandlePos[3]).ToNormal();
	for(int iAxis = 0; iAxis < 3; iAxis++) {
		const float d = cMath::Abs(cVec3::Dot(ToEye, m_Frame.AxisNormal[iAxis]));
		m_Frame.AxisIsVisible[iAxis] = d <= m_MaxVisibleDot;
		m_Frame.AxisIsVisible[3] = true; // Central handle is always visible
	}
	
	return true;
} // cScaleTool::CalcFrame

//-----------------------------------------------------------------------------
// cScaleTool::OnRender
//-----------------------------------------------------------------------------
void cScaleTool::OnRender() {
	if(!m_pScale || !m_pRotate || !m_pTranslate) { // Tool is not binded.
		return;
	}

	if(MODE_NONE == m_Mode) { // Update frame in non - scale mode.
		CalcFrame();
	}

	// Colors:
	const cColor SelColor = cColor::Yellow;
	const cColor TraceColor = cColor::Gray50;
	// Appearance:
	const float LineWidth = 2.0f;
	const float TraceQuadSide = 0.096f * m_Frame.LineLen;
	const float HandleRadius = 0.06f * m_Frame.LineLen;
	const float LabelShift = 0.2f * m_Frame.LineLen;
	
	const bool ShowAxis[3] = {
		m_Frame.AxisIsVisible[0] && m_Mode != MODE_ALONG_Y && m_Mode != MODE_ALONG_Z,
		m_Frame.AxisIsVisible[1] && m_Mode != MODE_ALONG_X && m_Mode != MODE_ALONG_Z,
		m_Frame.AxisIsVisible[2] && m_Mode != MODE_ALONG_X && m_Mode != MODE_ALONG_Y
	};
	const cVec3 CurEnds[3] = {
		MODE_NONE == m_Mode ? m_Frame.HandlePos[0] : m_Frame.HandlePos[3] + m_Frame.AxisNormal[0] * m_Frame.LineLen * m_Frame.RelScale[0],
		MODE_NONE == m_Mode ? m_Frame.HandlePos[1] : m_Frame.HandlePos[3] + m_Frame.AxisNormal[1] * m_Frame.LineLen * m_Frame.RelScale[1],
		MODE_NONE == m_Mode ? m_Frame.HandlePos[2] : m_Frame.HandlePos[3] + m_Frame.AxisNormal[2] * m_Frame.LineLen * m_Frame.RelScale[2],
	};

	// Drawing Traces in any scale mode:
	if(m_Mode != MODE_NONE) {
		for(int iAxis = 0; iAxis < 3; iAxis++) {
			if(!ShowAxis[iAxis]) {
				continue;
			}
			// Line Trace:
			cRender::GetInstance()->DrawLine(m_Frame.HandlePos[3], m_Frame.HandlePos[iAxis], TraceColor, LineWidth);
			// Quad Trace:
			cRender::GetInstance()->DrawBillboardQuad(m_Frame.HandlePos[iAxis], TraceQuadSide, TraceColor);
		}
	}
	
	// Drawing Axes:
	const cColor AxisColor[3] = {
		MODE_ALONG_X == m_Mode ? SelColor : cColor::Red,
		MODE_ALONG_Y == m_Mode ? SelColor : cColor::Green,
		MODE_ALONG_Z == m_Mode ? SelColor : cColor::Blue
	};
	for(int iAxis = 0; iAxis < 3; iAxis++) {
		if(!ShowAxis[iAxis]) {
			continue;
		}
		// Line:
		const cVec3 End = CurEnds[iAxis];//MODE_NONE == m_Mode ? m_Frame.HandlePos[iAxis] : m_Frame.HandlePos[3] + m_Frame.AxisNormal[iAxis] * m_Frame.LineLen * m_Frame.RelScale[iAxis];
		cRender::GetInstance()->DrawLine(m_Frame.HandlePos[3], End, cColor::Gray25, LineWidth);
		// Handle:
		cRender::GetInstance()->DrawCube(End, 2.0f * HandleRadius, AxisColor[iAxis], *m_pRotate);
	}

	// Center:
	const cColor CenterColor = MODE_UNIFORM == m_Mode ? SelColor : cColor::White;
	cRender::GetInstance()->DrawCube(m_Frame.HandlePos[3], 2.0f * HandleRadius, CenterColor, *m_pRotate);

	// Drawing labels in any scale mode:
	if(m_Mode != MODE_NONE) {
		cStr Label(cStr::csYellow);
		cVec3 LabelPos;
		const int iCurAxis = m_Mode - MODE_ALONG_X;
		cAssert(iCurAxis >= 0 && iCurAxis <= 3);
		if(MODE_UNIFORM == m_Mode) {
			LabelPos = m_Frame.HandlePos[3] + (cRender::GetInstance()->GetViewer()->GetRight() + cRender::GetInstance()->GetViewer()->GetUp()) * LabelShift;
		} else {
			LabelPos = CurEnds[iCurAxis] + cRender::GetInstance()->GetViewer()->GetUp() * LabelShift;
		}
		Label << cStr::FromFloat(m_Frame.RelScale[iCurAxis % 3]);
		cRender::GetInstance()->DrawStrCentered(Label, LabelPos);
	}
} // cScaleTool::OnRender

//-----------------------------------------------------------------------------
// cScaleTool::OnButtonDown
//-----------------------------------------------------------------------------
bool cScaleTool::OnButtonDown(const int Code) {
	if(Code != cInput::LeftButton) {
		return false;
	}
	if(!CalcFrame()) { // Tool is not binded.
		return false;
	}

	cAssert(MODE_NONE == m_Mode);
	
	float D[4];	// Distance from handle to pick ray
	for(int iAxis = 0; iAxis < 4; iAxis++) {
		D[iAxis] = m_Frame.AxisIsVisible[iAxis] ? cSeg::Distance(cSeg::Ray, m_Frame.PickRay, m_Frame.HandlePos[iAxis]) : cMath::Infinity;
	}
	const int iMin = cMath::MinIndex(D[0], D[1], D[2], D[3]);
	if(D[iMin] < m_Frame.MaxSelDistance) {
		const cSeg S(cSeg::RayCtor, m_Frame.HandlePos[3], m_Frame.AxisNormal[iMin]);
		cVec3 q;
		cSeg::ClosestPoints(cSeg::LineRay, S, m_Frame.PickRay, m_Frame.PickedPoint, q);
		
		m_Frame.AbsScale = *m_pScale;
		m_Frame.RelScale.Set(1.0f);
		m_Mode = (MODE)(MODE_ALONG_X + iMin);
	}

	if(MODE_NONE == m_Mode) {
		return false;
	}
	
	// Set capture in any scale mode:
	cWidgets::SetCapture();
	return true;
} // cScaleTool::OnButtonDown

//-----------------------------------------------------------------------------
// cScaleTool::OnButtonUp
//-----------------------------------------------------------------------------
bool cScaleTool::OnButtonUp(const int Code) {
	if(MODE_NONE == m_Mode) {
		return false;
	}
	if(Code != cInput::LeftButton) {
		return false;
	}
	// Releasing capture from any scale mode:
	m_Mode = MODE_NONE;
	cWidgets::ReleaseCapture();
	return true;
} // cScaleTool::OnButtonUp

//-----------------------------------------------------------------------------
// cScaleTool::OnMouseMove
//-----------------------------------------------------------------------------
bool cScaleTool::OnMouseMove(const int DeltaX, const int DeltaY) {
	if(!cInput::IsDown(cInput::LeftButton)) {
		m_Mode = MODE_NONE;
		cWidgets::ReleaseCapture();
	}
	if(MODE_NONE == m_Mode) {
		return false;
	}
	
	const int xMouse = cInput::GetMouseX();
	const int yMouse = cInput::GetMouseY();
	const cSeg PickRay = cRender::GetInstance()->GetViewer()->GetPickRay(xMouse, yMouse);
	
	cVec4 RelScale(1.0f);
	const int iAxis = m_Mode - MODE_ALONG_X;
	cAssert(iAxis >= 0 && iAxis < 4);
	const cSeg S(cSeg::RayCtor, m_Frame.HandlePos[iAxis], m_Frame.AxisNormal[iAxis]);
	cVec3 p, q;
	cSeg::ClosestPoints(cSeg::LineRay, S, PickRay, p, q);
	const cVec3 Shift = p - m_Frame.PickedPoint;
	const float Sign = cMath::Sign(cVec3::Dot(Shift, m_Frame.AxisNormal[iAxis]));
	RelScale[iAxis] = 1.0f + Sign * Shift.Length() / m_Frame.LineLen;
	
	if(MODE_UNIFORM == m_Mode) {
		for(int i = 0; i < 3; i++) {
			RelScale[i] = m_Frame.AxisIsVisible[i] ? RelScale.w : RelScale[i];
		}
	}
	m_Frame.RelScale = RelScale.ToVec3();

	*m_pScale = m_Frame.AbsScale * m_Frame.RelScale;
	return true;
} // cScaleTool::OnMouseMove