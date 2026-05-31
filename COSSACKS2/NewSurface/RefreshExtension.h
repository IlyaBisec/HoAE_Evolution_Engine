#ifndef __RefreshExtension__
#define __RefreshExtension__

#include "GameExtension.h"
#include "Surface.h"

extern UniversalMap g_UniversalMap;

class RefreshExtension : public GameExtension {
public:
	RefreshExtension() {
		m_bActive = false;
		m_bFirst = true;
		m_pUniversalMap = &g_UniversalMap;
		m_fIlluminate = false;
	}
	RefreshExtension(eSSIllumination Illumination) {
		m_Illumination = Illumination;
		m_bActive = false, m_bFirst = true;
		m_pUniversalMap = &g_UniversalMap;
		m_fIlluminate = true;
	}
	~RefreshExtension() {
		m_pUniversalMap = NULL;
	}
	virtual bool CheckActivityOfEditor() {
		return m_bActive;
	}
	virtual void ActivateEditor() {
		if(m_bActive) return;
		m_bActive = m_bFirst = true;
		m_bTimeShow = false;
		m_NDrawCalls = 0;
	}
	virtual void OnDrawOnMapOverAll() {
		if(!m_bActive) return;
		if(m_bTimeShow) {
			if(m_NDrawCalls > 100) m_bActive = false;
			m_NDrawCalls++;
			ShowString(10, RealLy - 40, m_Str, &BigRedFont);
			return;
		}
		if(m_bFirst) {
			ShowString(10, RealLy - 40, "Refreshing...", &BigWhiteFont);
			m_bFirst = false;
		} else {
			float T;
			if(m_fIlluminate) {
				T = m_pUniversalMap->Refresh(m_Illumination) / 1000.0f;
			} else {
				T = m_pUniversalMap->Refresh() / 1000.f;
			}
			sprintf(m_Str, "Refreshed in %.2f (s).", T);
			void ResetGroundCache();
			ResetGroundCache();
			m_bTimeShow = true;
		}
	}

private:
	int m_NDrawCalls;
	char m_Str[1024];
	bool m_bActive, m_bFirst, m_bTimeShow, m_fIlluminate;
	UniversalMap *m_pUniversalMap;
	eSSIllumination m_Illumination;
};

#endif // __RefreshExtension__