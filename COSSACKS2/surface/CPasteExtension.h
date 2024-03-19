#ifndef __CPASTEEXTENSION__
#define __CPASTEEXTENSION__

#include "stdheader.h"
#include "GameExtension.h"
#include "surface.h"
#include "UniversalMapClipboard.h"

class CPasteExtension : public GameExtension
{
public:
	CPasteExtension(UniversalMapClipboard *pUniversalMapClipboard)
	{
		Release();
		m_pUniversalMapClipboard = pUniversalMapClipboard;
	}
	~CPasteExtension() { Release(); }
	void Release(void)
	{
		m_pUniversalMapClipboard = NULL;
		m_CoordX = m_CoordY = m_CoordZ = 0;
		m_bActive = false;
	}
	virtual bool CheckActivityOfEditor() { return m_bActive; }
	virtual void ClearActivityOfEditor() { m_bActive = false; }
	virtual void ActivateEditor()
	{
		m_bActive = m_pUniversalMapClipboard->m_Mesh.Vert.NValues &&
			m_pUniversalMapClipboard->m_Mesh.Idxs.NValues &&
			m_pUniversalMapClipboard->m_Contour.NValues;
		m_CoordX = m_CoordY = m_CoordZ = 0;
		m_pUniversalMapClipboard->m_bInvalid = true;
	}
	virtual bool OnMouseWheel(int Delta)
	{
		if(!m_bActive) return false;
		if(GetKeyState(VK_LSHIFT) & 0x8000) m_CoordZ += (Delta > 0 ? m_dZ : - m_dZ);
		else m_pUniversalMapClipboard->Rotate(Delta);
		const int MaxDCoordZ = 400;
		if(m_CoordZ > MaxDCoordZ) m_CoordZ = MaxDCoordZ;
		if(m_CoordZ < - MaxDCoordZ) m_CoordZ = - MaxDCoordZ;
		return true;
	}
	virtual bool OnMouseHandling(int mx, int my, bool &LeftPressed,
		bool &RightPressed, int MapCoordX, int MapCoordY, bool OverMiniMap)
	{
		if(!m_bActive) return false;
		m_CoordX = MapCoordX, m_CoordY = MapCoordY;

		void ResetGroundCache();
		if(LeftPressed)
		{
			m_bActive = false;
			m_pUniversalMapClipboard->m_pUniversalMap->Backup_Store();
			m_pUniversalMapClipboard->Paste(m_CoordX, m_CoordY, m_CoordZ);
			ResetGroundCache();
			return true;
		}
		return false;
	}
	virtual void OnDrawOnMapOverAll()
	{
		if(!m_bActive) return;
		if(!m_pUniversalMapClipboard->m_Mesh.Vert.NValues ||
			!m_pUniversalMapClipboard->m_Mesh.Idxs.NValues ||
			!m_pUniversalMapClipboard->m_Contour.NValues) return;
		int x0 = m_CoordX - m_pUniversalMapClipboard->m_Radius,
			y0 = m_CoordY - m_pUniversalMapClipboard->m_Radius,
			x1 = m_CoordX + m_pUniversalMapClipboard->m_Radius,
			y1 = m_CoordY + m_pUniversalMapClipboard->m_Radius;
		bool bRedden = false;
		int QuadX0 = m_pUniversalMapClipboard->m_pUniversalMap->QuadX0,
			QuadY0 = m_pUniversalMapClipboard->m_pUniversalMap->QuadY0,
			UMLx = m_pUniversalMapClipboard->m_pUniversalMap->UMLx,
			UMLy = m_pUniversalMapClipboard->m_pUniversalMap->UMLy;
		if(x0 -  QuadX0 < UniversalMap::c_CtrlCVOffset ||
			y0 - QuadY0 < UniversalMap::c_CtrlCVOffset ||
			QuadX0 + UMLx - x1 < UniversalMap::c_CtrlCVOffset ||
			QuadY0 + UMLy - y1 < UniversalMap::c_CtrlCVOffset) bRedden = true;
		m_pUniversalMapClipboard->Draw(m_CoordX, m_CoordY, m_CoordZ, bRedden);
		// Info:
		ShowString(10, RealLy - 40, "Paste", &BigWhiteFont);
	}

	static const int m_dZ = 10;
private:
	bool m_bActive;
	int m_CoordX, m_CoordY, m_CoordZ;
	UniversalMapClipboard *m_pUniversalMapClipboard;
};

extern UniversalMapClipboard g_UniversalMapClipboard;
CPasteExtension g_PasteExtension(&g_UniversalMapClipboard);

#endif