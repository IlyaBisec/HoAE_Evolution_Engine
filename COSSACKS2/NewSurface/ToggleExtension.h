#ifndef __ToggleExtension__
#define __ToggleExtension__

#include "GameExtension.h"
#include "Surface.h"

extern UniversalMap g_UniversalMap;

typedef enum {TTFillMode, TTMapOfHeightsShow, TTInvertNormals} eToggleType;

class ToggleExtension : public GameExtension {
public:
	ToggleExtension(eToggleType ToggleType, const cStr &StrOn, const cStr &StrOff) {
		m_ToggleType = ToggleType;
		m_StrOn = StrOn, m_StrOff = StrOff;

		m_pUniversalMap = &g_UniversalMap;
		m_bActive = false;
	}
	virtual bool CheckActivityOfEditor() {
		return m_bActive;
	}
	virtual void ActivateEditor() {
		if(m_bActive) return;
		m_bActive = true;
		m_nCallsCount = 0;
	}
	virtual void OnDrawOnMapOverAll() {
		if(!m_bActive) return;
//		char *strMode[] = {"Fillmode: Wireframe", "Fillmode: Solid"};
		if(m_nCallsCount == 0) {
			switch(m_ToggleType) {
				case TTFillMode:
//					m_IsOn = !m_pUniversalMap->bWire;
//					m_pUniversalMap->bWire = m_IsOn;
					m_IsOn = !g_NewSurfaceWire;
					g_NewSurfaceWire = m_IsOn;
					break;
				case TTMapOfHeightsShow:
					m_IsOn = !m_pUniversalMap->m_bDrawMapOfHeights;
					m_pUniversalMap->m_bDrawMapOfHeights = m_IsOn;
					break;
				case TTInvertNormals:
					m_pUniversalMap->InvertNormals();
					break;
				default:
					break;
			}
			void ResetGroundCache();
			ResetGroundCache();
		}
		ShowString(10, RealLy - 40, m_IsOn ? m_StrOn : m_StrOff, &BigWhiteFont);
		m_nCallsCount++;
		if(m_nCallsCount > m_nCallsShow) m_bActive = false;
	}

private:
	cStr m_StrOn, m_StrOff;
	bool m_bActive, m_IsOn;
	int m_nCallsCount;
	UniversalMap *m_pUniversalMap;
	eToggleType m_ToggleType;
	static const int m_nCallsShow = 25;
};

#endif