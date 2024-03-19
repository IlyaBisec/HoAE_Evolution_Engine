#include "comms.h"

cRender *cRender::Instance = NULL;

//--------------------------------------------------------------------------------------------------------------------------------------------
// cRender::DrawSector
//--------------------------------------------------------------------------------------------------------------------------------------------
void cRender::DrawSector(const cVec3 &Center, const float Radius, const cVec3 &Fm, const cVec3 &To, const cColor &Color, const float dAngle) {
	const cVec3 n0 = Fm.ToNormal();
	const cVec3 n1 = To.ToNormal();
	const float Angle = cVec3::Angle(n0, n1);
	const int NTris = cMath::Max(1, (int)cMath::Round(Angle / dAngle));
	
	// Vertices:
	cList<cVertex::PositionColored> vb;
	cList<int> ib;
	cVertex::PositionColored u;
	u.Color = Color.ToRgba();
	u.Pos = Center, vb.Add(u);
	u.Pos = Center + n0 * Radius, vb.Add(u);
	for(int nTri = 1; nTri <= NTris; nTri++) {
		u.Pos = Center + cVec3::Slerp(n0, n1, (float)nTri / (float)NTris).ToNormal() * Radius;
		vb.Add(u);
	}
	// Indexes:
	const int s = cVec3::Dot(GetViewer()->GetForward(), cVec3::Cross(n0, n1)) > 0.0f ? 1 : 0;
	for(int nTri = 1; nTri <= NTris; nTri++) {
		ib.Add(0);
		ib.Add(nTri + s);
		ib.Add(nTri + (s + 1) % 2);
	}

	if(ib.Count() >= 3) {
		DrawPrimitives(cRender::Topology::TriangleList, cVertex::PositionColored::GetFormat(), vb.ToPtr(), vb.Count(), ib.ToPtr(), ib.Count());
	}

} // cRender::DrawSector

//--------------------------------------------------------------------------------------------------------------------
// cRender::DrawCube
//--------------------------------------------------------------------------------------------------------------------
void cRender::DrawCube(const cVec3 &Center, const float Side, const cColor &Color, const cQuat &q /*= cQuat::Identity*/) {
	// Offsets:
	const float h = 0.5f * Side;
	cVec3 Offsets[8] = { cVec3(-h, -h, -h), cVec3(h, -h, -h), cVec3(h, -h, h), cVec3(-h, -h, h),
		cVec3(-h, h, -h), cVec3(h, h, -h), cVec3(h, h, h), cVec3(-h, h, h) };
	for(int i = 0; i < 8; i++) {
		Offsets[i] *= q;
	}

	// Vertices:
	cList<cVertex::PositionColored> vb;
	cVertex::PositionColored u;

	u.Color = Color.ToRgba();
	for(i = 0; i < 8; i++) {
		u.Pos = Center + Offsets[i];
		vb.Add(u);
	}
	
	// Indexes:
	const int ib[] = {
		0, 1, 3,
		1, 2, 3,
		4, 7, 5,
		5, 7, 6,
		0, 7, 4,
		0, 3, 7,
		3, 6, 7,
		3, 2, 6,
		2, 5, 6,
		2, 1, 5,
		1, 4, 5,
		1, 0, 4
	};

	DrawPrimitives(cRender::Topology::TriangleList, cVertex::PositionColored::GetFormat(), vb.ToPtr(), vb.Count(), ib, sizeof(ib) / sizeof(int));
} // cRender::DrawCube

//---------------------------------------------------------------------------------------
// cRender::DrawBillboardQuad
//---------------------------------------------------------------------------------------
void cRender::DrawBillboardQuad(const cVec3 &Center, const float Side, const cColor &Color) {
	const cVec3 Up = GetViewer()->GetUp();
	const cVec3 Right = GetViewer()->GetRight();
	cVertex::PositionColored vb[4];
	vb[0].Color = vb[1].Color = vb[2].Color = vb[3].Color = Color.ToRgba();
	vb[0].Pos = Center - Right * Side * 0.5f + Up * Side * 0.5f; // upper - left
	vb[1].Pos = vb[0].Pos - Up * Side; // bottom - left
	vb[2].Pos = vb[0].Pos + Side * Right; // upper - right
	vb[3].Pos = vb[1].Pos + Side * Right; // bottom - right
	int ib[6] = { 0, 1, 3, 0, 3, 2 };
	DrawPrimitives(cRender::Topology::TriangleList, cVertex::PositionColored::GetFormat(), vb, 4, ib, 6);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// cRender::DrawCircle
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void cRender::DrawCircle(const cVec3 &Center, const float Radius, const cVec3 &Axis, const cColor &Color, const int SubDivs /*= 12*/, const float Width /*= 1.0f*/) {
	cAssert(SubDivs >= 3);
	cRotation Circle(Center, Axis.ToNormal(), 0.0f);
	const cVec3 Start = Center + Circle.GetAxis().ToPerp().ToNormal() * Radius;
	cVec3 u[2];
	u[0] = Start;
	for(int i = 0; i < SubDivs - 1; i++) {
		Circle.SetAngle(360.0f / (float)SubDivs * (float)(i + 1));
		u[1] = cVec3::Rotate(Start, Circle);
		DrawLine(u[0], u[1], Color, Width);
		u[0] = u[1];
	}
	DrawLine(u[0], Start, Color, Width);
} // cRender::DrawCircle

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// cRender::DrawFacingCircle
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void cRender::DrawFacingCircle(const cVec3 &Center, const float Radius, const cVec3 &Axis, const cColor &Color, const int SubDivs /*= 12*/, const float Width /*= 1.0f*/) {
	cAssert(SubDivs >= 3);
	const cVec3 OnScreenCenter = cVec3::TransformCoordinate(Center, GetViewer()->GetViewProjectionScreenMatrix());
	const cVec3 OnScreenUp = cVec3::TransformCoordinate(Center + GetViewer()->GetUp() * Radius, GetViewer()->GetViewProjectionScreenMatrix());
	const float OnScreenRadius = cVec2::Distance(OnScreenCenter.ToVec2(), OnScreenUp.ToVec2());
	cRotation Circle(Center, Axis.ToNormal(), 0.0f);
	const cVec3 Start = Center + Circle.GetAxis().ToPerp().ToNormal() * Radius;
	cVec3 u[2], Cross;
	u[0] = Start;
	for(int i = 0; i < SubDivs; i++) {
		Circle.SetAngle(360.0f / (float)SubDivs * (float)(i + 1));
		u[1] = cVec3::Rotate(Start, Circle);
		const cVec3 c = cVec3::Lerp05(u[0], u[1]);
		const cVec3 FmEye = (c - GetViewer()->GetPos()).ToNormal();
		const float d = cVec3::Dot(FmEye, c - Circle.GetOrig());
		if(d <= 0.0f) {
			const cVec3 OnScreen = cVec3::TransformCoordinate(c, GetViewer()->GetViewProjectionScreenMatrix());
			const float r = cVec2::Distance(OnScreenCenter.ToVec2(), OnScreen.ToVec2());
			if(r <= OnScreenRadius) {
				DrawLine(u[0], u[1], Color, Width);
			}
		}
		u[0] = u[1];
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// cRender::DrawCone
//-------------------------------------------------------------------------------------------------------------------------------------------------
void cRender::DrawCone(const cVec3 &Apex, const cVec3 &Axis, const float Radius, const float Length, const cColor &Color, const int SubDivs /*= 12*/) {
	const cVec3 Normal = Axis.ToNormal();
	const cVec3 Cap = Apex - Length * Normal;
	cRotation Cone(Cap, Normal, 0.0f);
	
	// Vertices:
	cList<cVertex::PositionColored> vb;
	const cVec3 Start = Cap + Radius * Normal.ToPerp().ToNormal();
	cVertex::PositionColored u;
	u.Color = Color.ToRgba();
	u.Pos = Cap, vb.Add(u); // Cap Vertex
	u.Pos = Apex, vb.Add(u);
	u.Pos = Start, vb.Add(u);
	
	for(int i = 1; i < SubDivs; i++) {
		Cone.SetAngle(360.0f / (float)SubDivs * (float)i);
		u.Pos = cVec3::Rotate(Start, Cone);
		vb.Add(u);
	}
	const int OnBorderVertsCount = vb.Count() - 2;

	// Indexes:
	cList<int> ib;
	for(i = 0; i < SubDivs; i++) {
		// Cap:
		ib.Add(0);
		const int Next = (i + 1) % OnBorderVertsCount + 2;
		ib.Add(Next);
		ib.Add(i + 2);
		// Cone:
		ib.Add(i + 2);
		ib.Add(Next);
		ib.Add(1);
	}

	if(ib.Count() >= 3) {
		DrawPrimitives(cRender::Topology::TriangleList, cVertex::PositionColored::GetFormat(), vb.ToPtr(), vb.Count(), ib.ToPtr(), ib.Count());
	}
} // cRender::DrawCone

namespace comms {
	
	class SysFont {
	public:
		static void Init();
		static cList<cVec2> vbChars[95];
		static cList<int> ibChars[95];
		static float GetCx() { return 8.0f; }
		static float GetCy() { return 16.0f; }
	private:
		static bool m_IsCreated;
	};
} // comms

//-----------------------------------------------------------------------------
// cRender::DrawStr
//-----------------------------------------------------------------------------
void cRender::DrawStr(const cStr &Str, const cVec2 &luPos) {
	comms::SysFont::Init();

	cList<cVertex::TransformedColored> vb;
	cList<int> ib;
	cVertex::TransformedColored u;
	u.Color = cStr::ColorForIndex(0).ToRgba();
	cColor Color;
	
	const char *pStr = Str.ToCharPtr();
	cVec2 Shift(0.0f);
	do {
		if(cStr::IsColor(pStr, Color)) {
			u.Color = Color.ToRgba();
			pStr += 2;
			continue;
		}
		if(cStr::CharIsNewLine(*pStr)) {
			pStr += 1;
			Shift.y += comms::SysFont::GetCy();
			Shift.x = 0.0f;
			continue;
		}
		const int c = *pStr++;
		if(c == '\0') {
			break;
		}
		if(c < 32 || c > 126) {
			continue;
		}
		const int iShift = vb.Count();

		const cList<cVec2> &vbChar = comms::SysFont::vbChars[c - 32];
		for(int i = 0; i < vbChar.Count(); i++) {
			u.Pos.Set(luPos + Shift + vbChar[i], 0.0f, 1.0f);
			vb.Add(u);
		}

		const cList<int> &ibChar = comms::SysFont::ibChars[c - 32];
		for(int i = 0; i < ibChar.Count(); i++) {
			ib.Add(ibChar[i] + iShift);
		}

		Shift.x += comms::SysFont::GetCx();
	} while(true);
	
	cRender::GetInstance()->DrawPrimitives(cRender::Topology::PointList, cVertex::TransformedColored::GetFormat(), vb.ToPtr(), vb.Count());
} // cRender::DrawStr

//-----------------------------------------------------------------------------
// cRender::DrawStrCentered
//-----------------------------------------------------------------------------
void cRender::DrawStrCentered(const cStr &Str, const cVec2 &Center) {
	const int LinesMaxLength = Str.LinesMaxLengthWithoutColors();
	const int LinesCount = Str.LinesCount();
	const float Width = LinesMaxLength * comms::SysFont::GetCx();
	const float Height = LinesCount * comms::SysFont::GetCy();

	const cVec2 luPos = Center - cVec2(Width / 2.0f, Height / 2.0f);
	DrawStr(Str, luPos);
} // cRender::DrawStrCentered

//-----------------------------------------------------------------------------
// cRender::DrawStrCentered
//-----------------------------------------------------------------------------
void cRender::DrawStrCentered(const cStr &Str, const cVec3 &Center) {
	if(cRender::GetInstance()->GetViewer()->CullPoint(Center)) {
		return;
	}
	const cVec2 OnScreenCenter = cVec3::TransformCoordinate(Center, cRender::GetInstance()->GetViewer()->GetViewProjectionScreenMatrix()).ToVec2();
	DrawStrCentered(Str, OnScreenCenter);
} // cRender::DrawStrCentered

//-----------------------------------------------------------------------------
// cRender::DrawStr
//-----------------------------------------------------------------------------
void cRender::DrawStr(const cStr &Str, const cVec3 &luPos) {
	if(cRender::GetInstance()->GetViewer()->CullPoint(luPos)) {
		return;
	}
	const cVec2 OnScreenluPos = cVec3::TransformCoordinate(luPos, cRender::GetInstance()->GetViewer()->GetViewProjectionScreenMatrix()).ToVec2();
	DrawStr(Str, OnScreenluPos);
} // cRender::DrawStr

namespace comms {

	extern BYTE Chars8x16[95][16];
	
	bool SysFont::m_IsCreated = false;
	cList<cVec2> SysFont::vbChars[95];
	cList<int> SysFont::ibChars[95];

	//-------------------------------------------------------------------------
	// SysFont::Init
	//-------------------------------------------------------------------------
	void SysFont::Init() {
		if(m_IsCreated) {
			return;
		}

		const int cxChar = 8, cyChar = 16;
		
		for(int c = ' '; c <= '~'; c++) {
			cList<cVec2> &vb = vbChars[c - ' '];
			cList<int> &ib = ibChars[c - ' '];
			
			byte *pRow = comms::Chars8x16[c - ' '] + 15;
			for(int iy = 0; iy < cyChar; iy++) {
				for(int ix = 0; ix < cxChar; ix++) {
					if(*pRow & (1 << ix)) {
						vb.Add(cVec2((float)ix, (float)iy));
					}
				}
				pRow--;
			}
		}
		
		m_IsCreated = true;
	} // SysFont::Init

} // comms