#include "comms.h"

//-----------------------------------------------------------------------------
// cViewPort2.ctor
//-----------------------------------------------------------------------------
cViewPort2::cViewPort2() {
	Home();
	dib.Free();
}

//-----------------------------------------------------------------------------
// cViewPort2::dtor
//-----------------------------------------------------------------------------
cViewPort2::~cViewPort2() {
	FreeBatches();
}

//-----------------------------------------------------------------------------
// cViewPort2::Home
//-----------------------------------------------------------------------------
void cViewPort2::Home() {
	At = cVec2::Zero;
	Scale = 1.0f;
	CalcT();
}

//-----------------------------------------------------------------------------
// cViewPort2::CalcT
//-----------------------------------------------------------------------------
void cViewPort2::CalcT() {
	cMat3 t = cMat3::OfTranslation(- At);
	cMat3 s = cMat3::OfScale(cVec2(Scale));
	cMat3 c = cMat3::OfTranslation(cVec2(float(dib.GetWidth()), float(dib.GetHeight())) / 2.0f);
	T = t * cMat3::OfScale(cVec2(1.0f, - 1.0f)) * s * c;
}

//-----------------------------------------------------------------------------
// cViewPort2::Init
//-----------------------------------------------------------------------------
void cViewPort2::Init(int Lx, int Ly) {
	if(Lx < 1 || Ly < 1) return;

	if(T.IsZero()) {
		T = cMat3::Identity;
	}

	if(dib.GetWidth() > 0 && dib.GetHeight() > 0 && (dib.GetWidth() != Lx || dib.GetHeight() != Ly)) {
		T = cMat3::OfFitToRect(cVec2::Zero, cVec2(float(Lx), float(Ly)),
			cVec2::Zero, cVec2(float(Lx), float(Ly)), false, false) * T;
	}

	dib.Init(Lx, Ly);

	CalcT();
}

//-----------------------------------------------------------------------------
// cViewPort2::Pick
//-----------------------------------------------------------------------------
const cVec2 cViewPort2::Pick(int x, int y) const {
	cVec2 u(0.0f);
	cMat3 iT;
	if(cMat3::Invert(T, &iT)) {
		u = cVec2::TransformCoordinate(cVec2(float(x), float(y)), iT);
	}
	return u;
}

//-----------------------------------------------------------------------------
// cViewPort2::Pan
//-----------------------------------------------------------------------------
void cViewPort2::Pan(int dx, int dy) {
	At -= Pick(dx, dy) - Pick(0, 0);
	CalcT();
}

//-----------------------------------------------------------------------------
// cViewPort2::Zoom
//-----------------------------------------------------------------------------
void cViewPort2::Zoom(int dx, int dy) {
	const float Speed = 1.0f / 300.f;
	const float MinScale = 0.1f;
	const float MaxScale = 10.0f;

	Scale += Speed * float(dx + dy);
	Scale = cMath::Clamp(Scale, MinScale, MaxScale);

	CalcT();
}

//-------------------------------------------------------------------------------------
// cViewPort2::Message
//-------------------------------------------------------------------------------------
bool cViewPort2::Message(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	static POINTS ptsBegin, ptsEnd;
	static bool IsPanning = false;
	static bool IsZooming = false;
	switch(Msg) {
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			if(GetKeyState(VK_MENU) < 0 && WM_RBUTTONDOWN == Msg) {
				IsZooming = true;
			} else if(WM_MBUTTONDOWN == Msg) {
				IsPanning = true;
			}
			if(IsPanning || IsZooming) {
				ptsBegin = MAKEPOINTS(lParam);
				SetCapture(hWnd);
				return true;
			}
			return false;
		case WM_MOUSEMOVE:
			if(!IsPanning && !IsZooming) return false;
			
			ptsEnd = MAKEPOINTS(lParam);
			int dx, dy;
			dx = ptsEnd.x - ptsBegin.x;
			dy = ptsEnd.y - ptsBegin.y;
			
			if(IsPanning) {
				Pan(dx, dy);
			} else if(IsZooming) {
				Zoom(dx, dy);
			}
			ptsBegin = ptsEnd;
			return true;
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			if(!IsPanning && !IsZooming) return false;
			
			ReleaseCapture();
			IsPanning = false;
			IsZooming = false;
			return true;
		case WM_KEYDOWN:
			if(VK_HOME == wParam) {
				Home();
				return true;
			} else return false;
		case 0x020A://WM_MOUSEWHEEL:
			short d;
			d = (short)HIWORD(wParam);
			d /= 120/*WHEEL_DELTA*/ / 24;
			Zoom(d, 0);
			return true;
		default:
			return false;
	}
}

//-----------------------------------------------------------------------------
// cViewPort2::Fit
//-----------------------------------------------------------------------------
void cViewPort2::Fit(const cBounds &B, const int Border) {
	bool xSize = !cMath::IsZero(B.GetSizeX());
	bool ySize = !cMath::IsZero(B.GetSizeY());
	cVec2 w = cVec2(float(dib.GetWidth()), float(dib.GetHeight())) - cVec2(float(2 * Border));

	if(xSize && ySize) {
		Scale = cMath::Min(w.x / B.GetSizeX(), w.y / B.GetSizeY());
	} else if(xSize) {
		Scale = w.x / B.GetSizeX();
	} else if(ySize) {
		Scale = w.y / B.GetSizeY();
	}

	At = B.GetCenter().ToVec2();
	CalcT();
}

//-------------------------------------------------------------------------------------------------------------
// cViewPort2::DrawSphere
//-------------------------------------------------------------------------------------------------------------
void cViewPort2::DrawSphere(const cVec2 &c, float Radius, const cColor &Color, const bool TransformRadius) {
	cVec2 t = cVec2::TransformCoordinate(c, T);
	float r = TransformRadius ? cMath::Max(1.0f, cVec2::Length(cVec2::TransformNormal(cVec2(Radius, 0.0f), T))) : Radius;
	dib.Sphere(int(t.x), int(t.y), int(r + 0.5f), Color);
}

//-----------------------------------------------------------------------------
// cViewPort2::DrawStr
//-----------------------------------------------------------------------------
void cViewPort2::DrawStr(const cStr &Str, const cVec2 &Pos) {
	cVec2 p = cVec2::TransformCoordinate(Pos, T);
	dib.Str(Str, int(p.x), int(p.y));
}

//-------------------------------------------------------------------------------
// cViewPort2::DrawStrCentered
//-------------------------------------------------------------------------------
void cViewPort2::DrawStrCentered(const cStr &Str, const cVec2 &Center) {
	cVec2 p = cVec2::TransformCoordinate(Center, T);
	dib.StrCenter(Str, int(p.x), int(p.y));
}

//--------------------------------------------------------------------------------------
// cViewPort2::DrawLine
//--------------------------------------------------------------------------------------
void cViewPort2::DrawLine(const cVec2 &l0, const cVec2 &l1, const cColor &Color) {
	cVec2 p0 = cVec2::TransformCoordinate(l0, T);
	cVec2 p1 = cVec2::TransformCoordinate(l1, T);
	dib.Line(int(p0.x), int(p0.y), int(p1.x), int(p1.y), cDib24::Pixel::FromColor(Color));
}

//--------------------------------------------------------------------------------------------
// cViewPort2::DrawRect
//--------------------------------------------------------------------------------------------
void cViewPort2::DrawRect(const cVec2 &bbMin, const cVec2 &bbMax, const cColor &Color) {
	cVec2 p0 = cVec2::TransformCoordinate(bbMin, T);
	cVec2 p1 = cVec2::TransformCoordinate(bbMax, T);
	cDib24::Pixel p = cDib24::Pixel::FromColor(Color);
	dib.Line(int(p0.x), int(p0.y), int(p1.x), int(p0.y), p);
	dib.Line(int(p0.x), int(p0.y), int(p0.x), int(p1.y), p);
	dib.Line(int(p1.x), int(p1.y), int(p1.x), int(p0.y), p);
	dib.Line(int(p1.x), int(p1.y), int(p0.x), int(p1.y), p);
}

//-------------------------------------------------------------------------------------------------------------------
// cViewPort2::DrawCircle
//-------------------------------------------------------------------------------------------------------------------
void cViewPort2::DrawCircle(const cVec2 &c, const float Radius, const cColor &Color, const bool TransformRadius) {
	cVec2 t = cVec2::TransformCoordinate(c, T);
	float r = TransformRadius ? cMath::Max(1.0f, cVec2::Length(cVec2::TransformNormal(cVec2(Radius, 0.0f), T))) : Radius;
	dib.Circle(int(t.x), int(t.y), int(r + 0.5f), Color);
}

//-----------------------------------------------------------------------------------------------------------------------
// cViewPort2::DrawRound
//-----------------------------------------------------------------------------------------------------------------------
void cViewPort2::DrawRound(const cVec2 &Center, const float Radius, const cColor &Color, const bool TransformRadius) {
	cVec2 tCenter = cVec2::TransformCoordinate(Center, T);
	float r = TransformRadius ? cMath::Max(1.0f, cVec2::Length(cVec2::TransformNormal(cVec2(Radius, 0.0f), T))) : Radius;
	dib.Round(int(tCenter.x), int(tCenter.y), int(r + 0.5f), Color);
}

//---------------------------------------------------------------------------------------------------------
// cViewPort2::DrawTri
//---------------------------------------------------------------------------------------------------------
void cViewPort2::DrawTri(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, const cColor &Color) {
	cVec2 A = cVec2::TransformCoordinate(t0, T);
	cVec2 B = cVec2::TransformCoordinate(t1, T);
	cVec2 C = cVec2::TransformCoordinate(t2, T);
	float s = cVec2::AreaSigned(A, B, C);
	if(s > 0) cMath::Swap(B, C);
	dib.Tri(A, B, C, cDib24::Pixel::FromColor(Color));
}

//-------------------------------------------------------------------------------------------------------------
// cViewPort2::AddRect
//-------------------------------------------------------------------------------------------------------------
void cViewPort2::AddRect(const cVec2 &bbMin, const cVec2 &bbMax, const cColor &Color, const int nLayer) {
	cAssert(nLayer <= LayerMaxIndex);
	m_Layers[nLayer].Add(new BatchRect(bbMin, bbMax, Color));
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// cViewPort2::AddCircle
//-----------------------------------------------------------------------------------------------------------------------------------------
void cViewPort2::AddCircle(const cVec2 &Center, const float Radius, const cColor &Color, const bool TransformRadius, const int nLayer) {
	cAssert(nLayer <= LayerMaxIndex);
	m_Layers[nLayer].Add(new BatchCircle(Center, Radius, Color, TransformRadius));
}

//----------------------------------------------------------------------------------------------------------------------------------------
// cViewPort2::AddRound
//----------------------------------------------------------------------------------------------------------------------------------------
void cViewPort2::AddRound(const cVec2 &Center, const float Radius, const cColor &Color, const bool TransformRadius, const int nLayer) {
	cAssert(nLayer <= LayerMaxIndex);
	m_Layers[nLayer].Add(new BatchRound(Center, Radius, Color, TransformRadius));
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// cViewPort2::AddSphere
//-----------------------------------------------------------------------------------------------------------------------------------------
void cViewPort2::AddSphere(const cVec2 &Center, const float Radius, const cColor &Color, const bool TransformRadius, const int nLayer) {
	cAssert(nLayer <= LayerMaxIndex);
	m_Layers[nLayer].Add(new BatchSphere(Center, Radius, Color, TransformRadius));
}

//-----------------------------------------------------------------------------------------------------
// cViewPort2::AddLine
//-----------------------------------------------------------------------------------------------------
void cViewPort2::AddLine(const cVec2 &A, const cVec2 &B, const cColor &Color, const int nLayer) {
	cAssert(nLayer <= LayerMaxIndex);
	m_Layers[nLayer].Add(new BatchLine(A, B, Color));
}

//---------------------------------------------------------------------------------
// cViewPort2::AddStr
//---------------------------------------------------------------------------------
void cViewPort2::AddStr(const cStr &Str, const cVec2 &luPos, const int nLayer) {
	cAssert(nLayer <= LayerMaxIndex);
	m_Layers[nLayer].Add(new BatchStr(Str, luPos));
}

//------------------------------------------------------------------------------------------
// cViewPort2::AddStrCentered
//------------------------------------------------------------------------------------------
void cViewPort2::AddStrCentered(const cStr &Str, const cVec2 &Center, const int nLayer) {
	cAssert(nLayer <= LayerMaxIndex);
	m_Layers[nLayer].Add(new BatchStrCentered(Str, Center));
}

//-----------------------------------------------------------------------------
// cViewPort2::FreeBatches
//-----------------------------------------------------------------------------
void cViewPort2::FreeBatches() {
	for(int l = 0; l < LayerMaxIndex; l++) {
		m_Layers[l].FreeContents();
		m_Layers[l].Free();
	}
}

//-----------------------------------------------------------------------------
// cViewPort2::Batch::Draw
//-----------------------------------------------------------------------------
void cViewPort2::Batch::Draw(cViewPort2 &ViewPort) const {
	for(int i = 0; i < Count(); i++) {
		GetAt(i)->Draw(ViewPort);
	}
}

//-----------------------------------------------------------------------------
// cViewPort2::Render
//-----------------------------------------------------------------------------
void cViewPort2::Render() {
	for(int l = 0; l <= LayerMaxIndex; l++) {
		m_Layers[l].Draw(*this);
	}
}