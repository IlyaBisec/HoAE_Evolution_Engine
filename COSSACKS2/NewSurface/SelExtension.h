#ifndef __SelExtension__
#define __SelExtension__

#include "ToolExtension.h"
#include "ITerrain.h"
#include "ExtOptions.h"

typedef enum { CMCCW, CMCW, CMNone } eCullMode;
int g_SelRadius = ExtOptions::dRadius;

class SelExtension : public GameExtension {
public:
	SelExtension();
	virtual bool CheckActivityOfEditor();
	virtual void ClearActivityOfEditor();
	virtual void ActivateEditor();
	virtual bool OnMouseWheel(int);
	virtual bool OnMouseHandling(int mx, int my, bool &LMB, bool &RMB, int, int, bool);
	virtual void OnDrawOnMapOverAll();
private:
	bool m_fActive;
	UniversalMap *m_pUniversalMap;
	int *m_pSelRadius;
	static int PickTri(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &, eCullMode);
	OneMapVerts m_Markers;
};

SelExtension::SelExtension() {
	m_fActive = false;
	m_pUniversalMap = &g_UniversalMap;
	m_pSelRadius = &g_SelRadius;
}

bool SelExtension::CheckActivityOfEditor() {
	return m_fActive;
}

void SelExtension::ClearActivityOfEditor() {
	m_fActive = false;
}

void SelExtension::ActivateEditor() {
	m_fActive = true;
}

bool SelExtension::OnMouseWheel(int Delta) {
	if(!m_fActive) return false;
	*m_pSelRadius += (Delta > 0 ? ExtOptions::dRadius : - ExtOptions::dRadius);
	if(*m_pSelRadius < 0) *m_pSelRadius = 0;
	return true;
}

// SelExtension::PickTri
int SelExtension::PickTri(const OneMapVertex &t0, const OneMapVertex &t1, const OneMapVertex &t2, eCullMode CullMode) {
	const int x1 = t0.x, x2 = t1.x, x3 = t2.x;
	const int y1 = t0.y, y2 = t1.y, y3 = t2.y;

	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	int S1 = Sign(C1);
	int S2 = Sign(C2);
	int S3 = Sign(C3);

	bool IsPicked = false;
	switch(CullMode) {
		case CMCCW:
			IsPicked = S1 > 0 && S2 > 0 && S3 > 0;
			break;
		case CMCW:
			IsPicked =  S1 < 0 && S2 < 0 && S3 < 0;
			break;
		default:
			IsPicked = (S1 > 0 && S2 > 0 && S3 > 0) || (S1 < 0 && S2 < 0 && S3 < 0);
	}
	if(!IsPicked) return 0;
	return (t0.z + t1.z + t2.z) / 3;
} // VertexBasedMeshRef::PickTri


#define SELEXTENSION_TIMING // COMMENT THIS!

#ifdef SELEXTENSION_TIMING
LARGE_INTEGER liSelExtensionOnMouseBeginTime, liSelExtensionOnMouseEndTime;
LONGLONG llSelExtensionOnMouseTotalTime = 0;
DWORD SelExtensionOnMouseCallsCount = 0;
#endif // SELEXTENSION_TIMING

inline void rgb2argb(DWORD &argb, DWORD rgb) {
	argb = (0xFF << 24) + (GetRValue(rgb) << 16) + (GetGValue(rgb) << 8) + GetBValue(rgb);
}

int SelExt_xCursor = 0, SelExt_yCursor = 0;
UINT_PTR CALLBACK CCHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	RECT rc;
	extern HWND g_hWnd;
	int Lx, Ly, dx, dy, x, y;
	if(uiMsg == WM_INITDIALOG) {
		// g_hWnd:
		GetWindowRect(g_hWnd, &rc);
		Lx = rc.right - rc.left;
		Ly = rc.bottom - rc.top;
		// hDlg:
		GetWindowRect(hDlg, &rc);
		dx = rc.right - rc.left, dy = rc.bottom - rc.top;
		x = SelExt_xCursor - dx / 2, y = SelExt_yCursor - dy / 2;
		// Posing:
		comms::Clamp(x, 0, Lx - dx);
		comms::Clamp(y, 0, Ly - dy);
		SetWindowPos(hDlg, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		return TRUE;
	}
	return FALSE;
}

void ShowChooseColorDialog(int xCursor, int yCursor, DWORD &argbColor) {
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	static DWORD rgbCurrent;

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	extern HWND g_hWnd;
	cc.hwndOwner = g_hWnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.lpfnHook = CCHookProc;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ENABLEHOOK;
	SelExt_xCursor = xCursor, SelExt_yCursor = yCursor;
	if(ChooseColor(&cc)) {
		rgbCurrent = cc.rgbResult;
		rgb2argb(argbColor, rgbCurrent);
	}
}

bool SelExtension::OnMouseHandling(int sx, int sy, bool &LMB, bool &RMB, int, int, bool OverMiniMap) {
	if(!m_fActive || g_Navigating || OverMiniMap) return false;

	static DWORD argbCurColor = comms::argbYellow;
	int iCurVert = -1;

	#ifdef SELEXTENSION_TIMING
		assert(QueryPerformanceCounter(&liSelExtensionOnMouseBeginTime));
	#endif // SELEXTENSION_TIMING

	// GetPickRay -> Orig, Dir:
	cVector Orig, Dir;
	Line3D l = ICam->GetPickRay(sx, sy);
	Vector3D O = l.Orig(), D = l.Dir();
	Orig.Set(O.x, O.y, O.z);
	Dir.Set(D.x, D.y, D.z);

	// Ref of potential Tris:
	ints QuadNums;
	for(int iQ = 0; iQ < m_pUniversalMap->m_pQuads.NValues; iQ++) {
		const OneMapVertex &src = m_pUniversalMap->m_pQuads[iQ]->m_Centre;
		cVector vec(src.x, src.y, src.z);
		double l = vec.DistToLine(Orig, Dir);
		if(l < m_pUniversalMap->m_pQuads[iQ]->m_Radius) {
			QuadNums.Add(iQ);
		}
	}

	VertexBasedMeshRef Ref;
	Ref.pOrigin = m_pUniversalMap;
	for(int i = 0; i < QuadNums.Count(); i++) {
		Ref.AddRef(*m_pUniversalMap->m_pQuads[QuadNums[i]]);
	}

	// Projecting verts:
	DWORDS VertNums;
	cHashIndex reIndexingTable;
	Ref.ExtractUniqueVerts(VertNums, reIndexingTable);
	if(!VertNums.Count()) return false;

	OneMapVerts pVerts;
	pVerts.Lock(VertNums.Count());
//	m_Markers.Lock(VertNums.Count()); // toSee Quads: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	cMatrix T = cMatrix::OfTransform3D(Dir, Orig);
	cVector vec;
	for(int i = 0; i < VertNums.Count(); i++) {
		const OneMapVertex &src = m_pUniversalMap->Vert[VertNums[i]];
//		m_Markers[i] = src; // toSee Quads: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		vec.Set(src.x, src.y, src.z);
		vec *= T;
		pVerts[i].Set((short)vec.x, (short)vec.y, (short)vec.z);
	}

	int prev_oi[3], prev_i[3]; // Inds of picked tri.
	// Nearest nTri -> nPicked:
	int nTri, oi[3], k, key, index, inds[3], Z, zMax = comms::IntMin, nPicked = -1;
	for(int iTri = 0; iTri < Ref.m_TriNums.NValues; iTri++) {
		nTri = Ref.m_TriNums[iTri];
		// reIndexing Tri -> inds:
		m_pUniversalMap->GetTri(nTri, oi[0], oi[1], oi[2]);
		for(k = 0; k < 3; k++) {
			key = reIndexingTable.GenerateKey(oi[k]);
			for(index = reIndexingTable.First(key); index != -1; index = reIndexingTable.Next(index)) {
				if(VertNums[index] == oi[k]) break;
			}
			assert(index != -1);
			inds[k] = index;
		}
		// Picking:
		const OneMapVertex &p0 = pVerts[inds[0]], &p1 = pVerts[inds[1]], &p2 = pVerts[inds[2]];
		Z = PickTri(p0, p1, p2, CMNone);
		if(Z != 0 && Z > zMax) {
			zMax = Z;
			nPicked = nTri;
			prev_oi[0] = oi[0], prev_oi[1] = oi[1], prev_oi[2] = oi[2];
			prev_i[0] = inds[0], prev_i[1] = inds[1], prev_i[2] = inds[2];
		}
	}

	if(nPicked != -1) {
		// Finding central vertex -> oiPickedVert:
		double d2Min = comms::DoubleMax, d2;
		int oiPickedVert;
		for(k = 0; k < 3; k++) {
			const OneMapVertex &p = pVerts[prev_i[k]];
			d2 = Square((double)p.x) + Square((double)p.y);
			if(d2 < d2Min) {
				d2Min = d2;
				oiPickedVert = prev_oi[k];
			}
		}
		// Center:
		iCurVert = oiPickedVert; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		m_Markers.Free();
		const OneMapVertex &v = m_pUniversalMap->Vert[oiPickedVert];
		m_Markers.Add(OneMapVertex(v.x, v.y, v.z, argbCurColor));
		// Finding surrounding verts:
		for(iTri = 0; iTri < Ref.m_TriNums.NValues; iTri++) {
			nTri = Ref.m_TriNums[iTri];
			m_pUniversalMap->GetTri(nTri, oi[0], oi[1], oi[2]);
			for(k = 0; k < 3; k++) {
				if(oi[k] == oiPickedVert) {
					const OneMapVertex &v = m_pUniversalMap->Vert[oi[(k + 1) % 3]];
					m_Markers.Add(OneMapVertex(v.x, v.y, v.z, comms::argbWhite));
				}
			}
		}
	}

	#ifdef SELEXTENSION_TIMING
	assert(QueryPerformanceCounter(&liSelExtensionOnMouseEndTime));
	llSelExtensionOnMouseTotalTime += liSelExtensionOnMouseEndTime.QuadPart - liSelExtensionOnMouseBeginTime.QuadPart;
	SelExtensionOnMouseCallsCount++;
	if(GetKeyState(VK_LCONTROL) & 0x8000) {
		// in counts:
		char Str[4096], Buffer[1024];
		LONGLONG llAverageTime = llSelExtensionOnMouseTotalTime / SelExtensionOnMouseCallsCount;
		sprintf(Str, "Average <SelExtension::OnMouseHangling> timing: %d (counts)", llAverageTime);
		// in ms (accurate within .01):
		LARGE_INTEGER liFrequency;
		assert(QueryPerformanceFrequency(&liFrequency));
		llAverageTime *= 100000;
		llAverageTime /= liFrequency.QuadPart;
		double ms = double(llAverageTime) / 100.;
		sprintf(Buffer, " / %.2f (ms)", ms);
		strcat(Str, Buffer);
		// ----------------------------------------------------
		extern HWND g_hWnd;
		MessageBox(g_hWnd, Str, "--- Info ---", 0);
	}
	#endif // SELEXTENSION_TIMING

	if(RMB) {
		extern bool Rpressed;
		ShowChooseColorDialog(sx, sy, argbCurColor);
		Rpressed = false;
	}
	if(LMB && iCurVert != -1 && !OverMiniMap) {
		// Tool:
		m_pUniversalMap->Vert[iCurVert].Color = argbCurColor;
		OneMapVertex bbMin, bbMax;
		m_Markers.BBox(bbMin, bbMax);

		Rct rc(bbMin.x, bbMin.y, bbMax.x - bbMin.x, bbMax.y - bbMin.y);
		ITerra->InvalidateGeometry(&rc);
	}
	return true;
}

void SelExtension::OnDrawOnMapOverAll() {
	if(!m_fActive) return;

	char Str[128];
	sprintf(Str, "m_Markers.Count() == %d", m_Markers.Count());
	ShowString(10, RealLy - 40, Str, &BigWhiteFont);

	VertexBasedMesh Markers;
	Markers.GenMarkers(m_Markers);
	VertexBasedMeshRef Ref;
	Markers.GetRefAll(Ref);
	OneSplitMesh Splitted;
	Splitted.Split(Ref, true);
	Splitted.Draw();
}

#endif // __SelExtension__