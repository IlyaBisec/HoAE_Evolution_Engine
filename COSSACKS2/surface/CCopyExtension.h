#ifndef __CCOPYEXTENSION__
#define __CCOPYEXTENSION__

#include "stdheader.h"
#include "GameExtension.h"
#include "surface.h"
#include "UniversalMapClipboard.h"
#include "Navigator.h"

class CCopyExtension : public GameExtension
{
public:
	CCopyExtension(UniversalMapClipboard *pUniversalMapClipboard)
	{
		Release();
		m_pUniversalMapClipboard = pUniversalMapClipboard;
	}
	~CCopyExtension() { Release(); }
	void Release(void)
	{
		m_pUniversalMapClipboard = NULL;
		m_CoordX = m_CoordY = m_Radius = 0;
		m_bActive = false;
	}
	virtual bool CheckActivityOfEditor() { return m_bActive; }
	virtual void ClearActivityOfEditor() { m_bActive = false; }
	virtual void ActivateEditor()
	{
		UniversalMap *pUniversalMap = m_pUniversalMapClipboard->m_pUniversalMap;
		m_bActive = pUniversalMap->Vert.NValues &&
			pUniversalMap->Idxs.NValues && pUniversalMap->m_pQuads.NValues;
		m_CoordX = m_CoordY = 0;
		m_Radius = m_pUniversalMapClipboard->c_MinRadius;
	}
	virtual bool OnMouseWheel(int Delta)
	{
		if(!m_bActive) return false;
		const int dRadius = 40;
		m_Radius += (Delta > 0 ? dRadius : - dRadius);
		if(m_Radius < m_pUniversalMapClipboard->c_MinRadius)
			m_Radius = m_pUniversalMapClipboard->c_MinRadius;
		return true;
	}
	virtual bool OnMouseHandling(int mx, int my, bool &LeftPressed,
		bool &RightPressed, int MapCoordX, int MapCoordY, bool OverMiniMap)
	{
		if(!m_bActive) return false;
		m_CoordX = MapCoordX, m_CoordY = MapCoordY;

		if(LeftPressed && !g_Navigator.m_Mode) {
			m_bActive = false;
			m_pUniversalMapClipboard->Copy(m_CoordX, m_CoordY, m_Radius);
			return true;
		}
		return false;
	}
	virtual void OnDrawOnMapOverAll()
	{
		if(!m_bActive) return;
		DWORD NormalColor = 0xFFFFFFFF, InvalidColor = 0xFFFF0000;
		UniversalMap *pUniversalMap = m_pUniversalMapClipboard->m_pUniversalMap;
		if(!pUniversalMap->Vert.NValues || !pUniversalMap->Idxs.NValues ||
			!pUniversalMap->m_pQuads.NValues) return;
		int x0 = m_CoordX - m_Radius, y0 = m_CoordY - m_Radius,
			x1 = m_CoordX + m_Radius, y1 = m_CoordY + m_Radius;
		bool bRedden = false;
		int QuadX0 = pUniversalMap->QuadX0, QuadY0 = pUniversalMap->QuadY0,
			UMLx = pUniversalMap->UMLx, UMLy = pUniversalMap->UMLy;
		if(x0 -  QuadX0 < UniversalMap::c_CtrlCVOffset ||
			y0 - QuadY0 < UniversalMap::c_CtrlCVOffset ||
			QuadX0 + UMLx - x1 < UniversalMap::c_CtrlCVOffset ||
			QuadY0 + UMLy - y1 < UniversalMap::c_CtrlCVOffset) bRedden = true;
		DWORD Color = bRedden ? InvalidColor : NormalColor;
//		void ShowPen(int x,int y1, int R, DWORD ptc);
//		ShowPen(m_CoordX, m_CoordY, m_Radius, Color);
		VertexBasedMeshRef QuadsRef;
		pUniversalMap->GetQuadsRefCylinder(m_CoordX, m_CoordY, m_Radius, QuadsRef);
		VertexBasedMeshRef Ref;
		QuadsRef.GetRefCylinder(m_CoordX, m_CoordY, m_Radius, Ref);
		OneSplitMesh SplittedMesh;
		SplittedMesh.Split(Ref);
		DWORD OrFlag = bRedden ? 0x00FF0000 : 0x00FFFFFF;
		DWORD AlphaLevel = bRedden ? 0xFF : 0x30;
		SplittedMesh.Modify(OneMapVertex(0, 0, 10), AlphaLevel << 24, OrFlag, 0,
			OneSplitMesh::MF_OFFSET | OneSplitMesh::MF_AND | OneSplitMesh::MF_OR | OneSplitMesh::MF_SEA);
		SplittedMesh.Draw();
		// Info:
		ShowString(10, RealLy - 40, "Copy", &BigWhiteFont);
	}
private:
	bool m_bActive;
	int m_CoordX, m_CoordY, m_Radius;
	UniversalMapClipboard *m_pUniversalMapClipboard;
};

extern UniversalMapClipboard g_UniversalMapClipboard;
CCopyExtension g_CopyExtension(&g_UniversalMapClipboard);

#endif