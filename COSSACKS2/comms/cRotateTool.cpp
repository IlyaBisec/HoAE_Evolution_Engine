#include "comms.h"

//*****************************************************************************
// RotateTool
//*****************************************************************************

//-----------------------------------------------------------------------------
// cRotateTool.ctor
//-----------------------------------------------------------------------------
cRotateTool::cRotateTool() {
	SetName("RotateTool");
	
	// Size:
	m_OuterRadius = 104.0f;
	m_InnerRadius = 0.86f;
	m_MaxSelDistance = 0.12f;
	
	m_pRotate = NULL;
	m_pTranslate = NULL;

	m_Mode = MODE_NONE;
	m_Space = m_DesiredSpace = SPACE_WORLD;
} // cRotateTool.ctor

//-----------------------------------------------------------------------------
// cRotateTool::Bind
//-----------------------------------------------------------------------------
void cRotateTool::Bind(cQuat *pRotate, cVec3 *pTranslate, const SPACE Space) {
	if(!pRotate || !pTranslate) {
		m_pRotate = NULL;
		m_pTranslate = NULL;
	} else {
		m_pRotate = pRotate;
		m_pTranslate = pTranslate;
	}
	m_Space = m_DesiredSpace = Space;
} // cRotateTool::Bind

//-----------------------------------------------------------------------------
// cRotateTool::SetSpace
//-----------------------------------------------------------------------------
void cRotateTool::SetSpace(const SPACE Space) {
	if(m_DesiredSpace != Space) {
		m_DesiredSpace = Space;
	}
	if(MODE_NONE == m_Mode) {
		m_Space = m_DesiredSpace;
	}
} // cRotateTool::SetSpace

//-----------------------------------------------------------------------------
// cRotateTool::CalcFrame
//-----------------------------------------------------------------------------
bool cRotateTool::CalcFrame() {
	if(!m_pRotate || !m_pTranslate) {
		return false;
	}
	
	m_Frame.Pos = *m_pTranslate;
	
	// AxisNormal[0..3]:
	if(SPACE_WORLD == m_Space) {
		m_Frame.AxisNormal[0] = cVec3::AxisX;
		m_Frame.AxisNormal[1] = cVec3::AxisY;
		m_Frame.AxisNormal[2] = cVec3::AxisZ;
	} else if(SPACE_OBJECT == m_Space) {
		const cMat3 R = m_pRotate->ToMat3();
		for(int iAxis = 0; iAxis < 3; iAxis++) {
			m_Frame.AxisNormal[iAxis] = R.GetRow(iAxis).ToNormal();
		}
	} else {
		cAssertM(0, "Illegal Space");
	}
	m_Frame.AxisNormal[3] = cRender::GetInstance()->GetViewer()->GetForward();

	// Size:
	cVec3 u = cVec3::TransformCoordinate(m_Frame.Pos, cRender::GetInstance()->GetViewer()->GetViewProjectionScreenMatrix());
	u.x += m_OuterRadius;
	u.TransformCoordinate(cRender::GetInstance()->GetViewer()->GetViewProjectionScreenMatrixInverse());
	u -= m_Frame.Pos;
	m_Frame.OuterRadius = u.Length();
	m_Frame.InnerRadius = m_Frame.OuterRadius * m_InnerRadius;
	m_Frame.MaxSelDistance = m_Frame.OuterRadius * m_MaxSelDistance;

	return true;
} // cRotateTool::CalcFrame

//-----------------------------------------------------------------------------
// cRotateTool::OnButtonDown
//-----------------------------------------------------------------------------
bool cRotateTool::OnButtonDown(const int Code) {
	if(Code != cInput::LeftButton) {
		return false;
	}
	if(!CalcFrame()) {
		return false;
	}

	const int xMouse = cInput::GetMouseX();
	const int yMouse = cInput::GetMouseY();
	
	const cSeg PickRay = cRender::GetInstance()->GetViewer()->GetPickRay(xMouse, yMouse);

	const cPlane P(m_Frame.Pos, m_Frame.AxisNormal[3]);
	cVec3 Around;
	const bool f = P.RayIntersection(PickRay.GetFm(), PickRay.GetNormal(), NULL, &Around);
	cAssert(f);
	const float dToCenter = cVec3::Distance(m_Frame.Pos, Around);
	if(cMath::Equals(dToCenter, m_Frame.OuterRadius, m_Frame.MaxSelDistance)) {
		m_Mode = MODE_AROUND_VIEW;
		m_Frame.StartNormal = (Around - m_Frame.Pos).ToNormal();
		m_Frame.RotationAxis = m_Frame.AxisNormal[3];
	} else {
		cSphere InnerSphere(m_Frame.Pos, m_Frame.InnerRadius);
		float Scale1, Scale2;
		if(InnerSphere.RayIntersection(PickRay.GetFm(), PickRay.GetNormal(), Scale1, Scale2)) {
			const float Scale = cMath::Min(Scale1, Scale2);
			m_Frame.StartNormal = (PickRay.GetFm() + Scale * PickRay.GetNormal() - m_Frame.Pos).ToNormal();
			const float D[3] = {
				cVec3::Dot(m_Frame.StartNormal, m_Frame.AxisNormal[0]),
				cVec3::Dot(m_Frame.StartNormal, m_Frame.AxisNormal[1]),
				cVec3::Dot(m_Frame.StartNormal, m_Frame.AxisNormal[2])
			};
			const int iMin = cMath::MinIndex(cMath::Abs(D[0]), cMath::Abs(D[1]), cMath::Abs(D[2]));
			if(cMath::Abs(D[iMin]) < m_MaxSelDistance) {
				m_Mode = (MODE)(MODE_AROUND_X + iMin);
				m_Frame.StartNormal = (m_Frame.StartNormal - m_Frame.AxisNormal[iMin] * D[iMin]).ToNormal();
				cAssert(cMath::IsZero(cVec3::Dot(m_Frame.StartNormal, m_Frame.AxisNormal[iMin])));
				m_Frame.RotationAxis = m_Frame.AxisNormal[iMin];
			} else {
				m_Mode = MODE_FREE;
			}
		}
	}

	if(MODE_NONE == m_Mode) {
		return false;
	}
	
	m_Frame.StartRotation = *m_pRotate;
	m_Frame.CurNormal = m_Frame.StartNormal;
	m_Frame.Angle = 0.0f;
	
	// Set capturing in any rotate mode.
	cWidgets::SetCapture();
	return true;
} // cRotateTool::OnButtonDown

//-----------------------------------------------------------------------------
// cRotateTool::OnButtonUp
//-----------------------------------------------------------------------------
bool cRotateTool::OnButtonUp(const int Code) {
	if(MODE_NONE == m_Mode) {
		return false;
	}
	if(Code != cInput::LeftButton) {
		return false;
	}
	// Releasing capture from any rotate mode:
	m_Mode = MODE_NONE;
	if(m_Space != m_DesiredSpace) {
		m_Space = m_DesiredSpace;
	}
	cWidgets::ReleaseCapture();
	return true; 
} // cRotateTool::OnButtonUp

//-----------------------------------------------------------------------------
// cRotateTool::OnMouseMove
//-----------------------------------------------------------------------------
bool cRotateTool::OnMouseMove(const int DeltaX, const int DeltaY) {
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
	if(!CalcFrame()) {
		return false;
	}
	
	const int xMouse = cInput::GetMouseX();
	const int yMouse = cInput::GetMouseY();
	const cSeg PickRay = cRender::GetInstance()->GetViewer()->GetPickRay(xMouse, yMouse);

	if(MODE_AROUND_VIEW == m_Mode) {
		const cPlane P(m_Frame.Pos, m_Frame.AxisNormal[3]);
		cVec3 Around;
		const bool f = P.RayIntersection(PickRay.GetFm(), PickRay.GetNormal(), NULL, &Around);
		cAssert(f);
		m_Frame.CurNormal = (Around - m_Frame.Pos).ToNormal();
	} else {
		cSphere InnerSphere(m_Frame.Pos, m_Frame.InnerRadius);
		float Scale1, Scale2;
		if(InnerSphere.RayIntersection(PickRay.GetFm(), PickRay.GetNormal(), Scale1, Scale2)) {
			const float Scale = cMath::Min(Scale1, Scale2);
			m_Frame.CurNormal = (PickRay.GetFm() + Scale * PickRay.GetNormal() - m_Frame.Pos).ToNormal();
		} else {
			const cSeg::Result r = PickRay.ProjectPoint(cSeg::Ray, m_Frame.Pos);
			m_Frame.CurNormal = (r.CollidedPoint - m_Frame.Pos).ToNormal();
		}
		if(m_Mode >= MODE_AROUND_X && m_Mode <= MODE_AROUND_Z) {
			const int iAxis = m_Mode - MODE_AROUND_X;
			cAssert(iAxis >= 0 && iAxis <= 2);
			m_Frame.CurNormal = (m_Frame.CurNormal - m_Frame.AxisNormal[iAxis] * cVec3::Dot(m_Frame.CurNormal, m_Frame.AxisNormal[iAxis])).ToNormal();
			cAssert(cMath::IsZero(cVec3::Dot(m_Frame.CurNormal, m_Frame.AxisNormal[iAxis])));
		}
	}

	if(!m_Frame.CurNormal.IsNormalized()) {
		m_Frame.CurNormal = m_Frame.StartNormal;
	}

	const float Angle = cVec3::Angle(m_Frame.StartNormal, m_Frame.CurNormal);
	cVec3 Axis = cVec3::Cross(m_Frame.StartNormal, m_Frame.CurNormal).ToNormal();
	if(m_Mode != MODE_FREE) {
		const float Sign = cMath::Sign(cVec3::Dot(m_Frame.RotationAxis, Axis));
		Axis = Sign > 0.0f ? m_Frame.RotationAxis : -m_Frame.RotationAxis;
		m_Frame.Angle = Sign * Angle;
	}
	if(Axis.IsNormalized()) {
		cRotation R(cVec3::Zero, Axis, Angle);
		*m_pRotate = m_Frame.StartRotation * R.ToQuat();
	}
	return true;
} // cRotateTool::OnMouseMove

//-----------------------------------------------------------------------------
// cRotateTool::OnRender
//-----------------------------------------------------------------------------
void cRotateTool::OnRender() {
	if(!CalcFrame()) {
		return;
	}
	
	const cColor SelColor = cColor::Yellow;
	const cColor XColor = MODE_AROUND_X == m_Mode ? SelColor : cColor::Red;
	const cColor YColor = MODE_AROUND_Y == m_Mode ? SelColor : cColor::Green;
	const cColor ZColor = MODE_AROUND_Z == m_Mode ? SelColor : cColor::Blue;
	const cColor OuterColor = MODE_AROUND_VIEW == m_Mode ? SelColor : cColor::White;
	const cColor InnerColor(cColor::Black);
	const cColor TraceColor(cColor::Gray50);
	const cColor TraceSectorColor(0.5f, 0.5f, 0.5f, 0.7f);
	
	const float TraceQuadSide = 0.08f * m_Frame.OuterRadius;
	const int SubDivs = 48;
	const float CircleWidth = 2.0f;
	const float TraceSectorDelta = 5.0f;
	const float LabelUp = 1.2f * m_Frame.OuterRadius;
	
	// Label:
	if(MODE_AROUND_X == m_Mode || MODE_AROUND_Y == m_Mode || MODE_AROUND_Z == m_Mode) {
		float Angles[3] = { 0.0f, 0.0f, 0.0f };
		const int iAxis = (int)(m_Mode - MODE_AROUND_X);
		Angles[iAxis] = m_Frame.Angle;
		cStr Label;
		Label << cStr::csYellow << "[" << cStr::FromFloatArray(Angles, 3) << "]";
		const cVec3 LabelPos = m_Frame.Pos + cRender::GetInstance()->GetViewer()->GetUp() * LabelUp;
		cRender::GetInstance()->DrawStrCentered(Label, LabelPos);
	}
	
	if(MODE_AROUND_X == m_Mode || MODE_AROUND_Y == m_Mode || MODE_AROUND_Z == m_Mode || MODE_AROUND_VIEW == m_Mode) {
		const float Radius = MODE_AROUND_VIEW == m_Mode ? m_Frame.OuterRadius : m_Frame.InnerRadius;
		const cVec3 p0 = m_Frame.Pos + m_Frame.StartNormal * Radius;
		const cVec3 p1 = m_Frame.Pos + m_Frame.CurNormal * Radius;
		cRender::GetInstance()->DrawBillboardQuad(m_Frame.Pos, TraceQuadSide, TraceColor);
		cRender::GetInstance()->DrawBillboardQuad(p0, TraceQuadSide, TraceColor);
		cRender::GetInstance()->DrawBillboardQuad(p1, TraceQuadSide, TraceColor);
		cRender::GetInstance()->DrawLine(m_Frame.Pos, p0, TraceColor, 2.0f);
		cRender::GetInstance()->DrawLine(m_Frame.Pos, p1, TraceColor, 2.0f);
		cRender::GetInstance()->SetBlendFunc(cRender::Blend::SourceAlpha, cRender::Blend::InvSourceAlpha);
		cRender::GetInstance()->DrawSector(m_Frame.Pos, Radius, m_Frame.StartNormal, m_Frame.CurNormal, TraceSectorColor, TraceSectorDelta);
	}
	cRender::GetInstance()->DrawBillboardCircle(m_Frame.Pos, m_Frame.InnerRadius, InnerColor, SubDivs, CircleWidth);
	cRender::GetInstance()->DrawBillboardCircle(m_Frame.Pos, m_Frame.OuterRadius, OuterColor, SubDivs, CircleWidth);

	cRender::GetInstance()->DrawFacingCircle(m_Frame.Pos, m_Frame.InnerRadius, m_Frame.AxisNormal[0], XColor, SubDivs, CircleWidth);
	cRender::GetInstance()->DrawFacingCircle(m_Frame.Pos, m_Frame.InnerRadius, m_Frame.AxisNormal[1], YColor, SubDivs, CircleWidth);
	cRender::GetInstance()->DrawFacingCircle(m_Frame.Pos, m_Frame.InnerRadius, m_Frame.AxisNormal[2], ZColor, SubDivs, CircleWidth);
} // cRotateTool::OnRender