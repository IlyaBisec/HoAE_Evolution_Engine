#include "comms.h"

//*****************************************************************************
// Grid
//*****************************************************************************

//-----------------------------------------------------------------------------
// cGrid.ctor
//-----------------------------------------------------------------------------
cGrid::cGrid() {
	SetName("Grid");
	SetDefaults();
} // cGrid.ctor

//-----------------------------------------------------------------------------
// cGrid::SetDefaults
//-----------------------------------------------------------------------------
void cGrid::SetDefaults() {
	m_Size = 20.0f; // units
	m_UpAxisLength = 20.0f;
	m_GridLinesEvery = 5.0f; // units
	m_Subdivisions = 5;
	m_ShowAxes = true;
	m_ShowGridLines = true;
	m_ShowSubdivisionLines = true;
} // cGrid::SetDefaults

//-----------------------------------------------------------------------------
// cGrid::OnRender
//-----------------------------------------------------------------------------
void cGrid::OnRender() {
	const cViewer::eUpAxis UpAxis = cRender::GetInstance()->GetViewer()->GetUpAxis();

	cList<cVertex::PositionColored> vb;
	cVertex::PositionColored u[10];

	const DWORD SubdivisionLinesColor = cColor::Gray50.ToRgba();
	const DWORD GridLinesColor = cColor::Gray25.ToRgba();
	const cColor AxisColor = cColor::Black;
	const float AxisWidth = 2.0f;
	
	if(m_ShowSubdivisionLines && m_Subdivisions > 1) {
		const float d = m_GridLinesEvery / float(m_Subdivisions);
		const int NumSubdivisionLines = int(m_Size / d);
		for(int i = 1; i <= NumSubdivisionLines; i++) {
			if(i % m_Subdivisions == 0) {
				continue;
			}
			const float c = i * d;
			u[0].Pos.Set(-m_Size, 0, c);
			u[1].Pos.Set(m_Size, 0, c);
			u[0].Color = SubdivisionLinesColor;
			u[1].Color = SubdivisionLinesColor;

			u[2].Pos.Set(-m_Size, 0, -c);
			u[3].Pos.Set(m_Size, 0, -c);
			u[2].Color = SubdivisionLinesColor;
			u[3].Color = SubdivisionLinesColor;

			u[4].Pos.Set(c, 0, -m_Size);
			u[5].Pos.Set(c, 0, m_Size);
			u[4].Color = SubdivisionLinesColor;
			u[5].Color = SubdivisionLinesColor;
			
			u[6].Pos.Set(-c, 0, -m_Size);
			u[7].Pos.Set(-c, 0, m_Size);
			u[6].Color = SubdivisionLinesColor;
			u[7].Color = SubdivisionLinesColor;
			
			vb.Append(u, 8);
		}
	}
	if(m_ShowAxes) {
		// Along negative X:
		cRender::GetInstance()->DrawLine(cVec3::Zero, m_Size * cVec3::AxisNegX, AxisColor, AxisWidth);
		// Along negative non up axis:
		if(UpAxis == cViewer::UpAxisY) {
			cRender::GetInstance()->DrawLine(cVec3::Zero, m_Size * cVec3::AxisNegZ, AxisColor, AxisWidth);
		} else {
			cAssert(UpAxis == cViewer::UpAxisZ);
			cRender::GetInstance()->DrawLine(cVec3::Zero, m_Size * cVec3::AxisNegY, AxisColor, AxisWidth);
		}
		// Along positive X:
		cRender::GetInstance()->DrawLine(cVec3::Zero, m_Size * cVec3::AxisX, cColor::Red, AxisWidth);
		// Along positive up and remaining non up axes:
		if(UpAxis == cViewer::UpAxisY) {
			cRender::GetInstance()->DrawLine(cVec3::Zero, m_UpAxisLength * cVec3::AxisY, cColor::Green, AxisWidth);
			cRender::GetInstance()->DrawLine(cVec3::Zero, m_Size * cVec3::AxisZ, cColor::Blue, AxisWidth);
		} else {
			cAssert(UpAxis == cViewer::UpAxisZ);
			cRender::GetInstance()->DrawLine(cVec3::Zero, m_UpAxisLength * cVec3::AxisZ, cColor::Blue, AxisWidth);
			cRender::GetInstance()->DrawLine(cVec3::Zero, m_Size * cVec3::AxisY, cColor::Green, AxisWidth);
		}
	}
	if(m_ShowGridLines) {
		const int NumGridLines = int(m_Size / m_GridLinesEvery);
		for(int i = 1; i <= NumGridLines; i++) {
			const float d = i * m_GridLinesEvery;
			
			u[0].Pos.Set(-m_Size, 0, d);
			u[1].Pos.Set(m_Size, 0, d);
			u[0].Color = GridLinesColor;
			u[1].Color = GridLinesColor;
			
			u[2].Pos.Set(-m_Size, 0, -d);
			u[3].Pos.Set(m_Size, 0, -d);
			u[2].Color = GridLinesColor;
			u[3].Color = GridLinesColor;
			
			u[4].Pos.Set(d, 0, -m_Size);
			u[5].Pos.Set(d, 0, m_Size);
			u[4].Color = GridLinesColor;
			u[5].Color = GridLinesColor;
			
			u[6].Pos.Set(-d, 0, -m_Size);
			u[7].Pos.Set(-d, 0, m_Size);
			u[6].Color = GridLinesColor;
			u[7].Color = GridLinesColor;

			vb.Append(u, 8);
		}
	}
	
	if(UpAxis == cViewer::UpAxisZ) {
		for(int i = 0; i < vb.Count(); i++) {
			cVertex::PositionColored &r = vb[i];
			cMath::Swap(r.Pos.y, r.Pos.z);
		}
	} else {
		cAssert(UpAxis == cViewer::UpAxisY);
	}
	
	if(vb.Count()) {
		cRender::GetInstance()->DrawPrimitives(cRender::Topology::LineList, cVertex::PositionColored::GetFormat(), vb.ToPtr(), vb.Count());
	}
} // cGrid::OnRender