#ifndef __ToolExtension__
#define __ToolExtension__

#include "GameExtension.h"
#include "Surface.h"
//#include "NavExt.h"

extern UniversalMap g_UniversalMap;
const int ToolExtensionInitialRadius = 64;
int g_ToolExtensionRadius = ToolExtensionInitialRadius;

class ToolExtension : public GameExtension {
public:
	ToolExtension(eToolType ToolType) {
		m_pUniversalMap = &g_UniversalMap;
		m_pRadius = &g_ToolExtensionRadius;
		m_ToolType = ToolType;
		m_bActive = false;
	}
	virtual bool CheckActivityOfEditor() {
		return m_bActive;
	}
	virtual void ClearActivityOfEditor() {
		m_bActive = false;
	}
	virtual void ActivateEditor() {
		if(m_bActive) return;
		m_bActive = true;
		m_CoordX = m_CoordY = 0;
	}
	virtual bool OnMouseWheel(int Delta) {
		if(!m_bActive) return false;
		*m_pRadius += (Delta > 0 ? kRadiusStep : - kRadiusStep);
		if(*m_pRadius < kMinRadius) *m_pRadius = kMinRadius;
		return true;
	}
	virtual void OnDrawOnMapOverAll() {
		if(!m_bActive) return;
		void ShowPen(int, int, int, DWORD);
		ShowPen(m_CoordX, m_CoordY, *m_pRadius, comms::argbWhite);
	}
	virtual bool OnMouseHandling(int, int, bool &LMB, bool &RMB, int MapX, int MapY, bool) {
		if(!m_bActive || g_Navigator.m_Mode) return false;
		m_CoordX = MapX, m_CoordY = MapY;

		ToolParams Params;
		Params.cx = m_CoordX, Params.cy = m_CoordY, Params.r = *m_pRadius, Params.dH = kDeformStep;
		if(LMB || RMB) {
			m_pUniversalMap->Tool(m_ToolType, Params);
			return true;
		}
		return false;
	}

	static const int kMinRadius = 32, kRadiusStep = 16, kDeformStep = 10;
private:
	bool m_bActive;
	int m_CoordX, m_CoordY, *m_pRadius;
	eToolType m_ToolType;
	UniversalMap *m_pUniversalMap;
};

#endif // ToolExtension