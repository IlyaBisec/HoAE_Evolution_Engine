#ifndef __CREFRESHEXTENSION__
#define __CREFRESHEXTENSION__

#include "GameExtension.h"
#include "Surface.h"

class CRefreshExtension : public GameExtension
{
public:
	CRefreshExtension(UniversalMap *pUniversalMap)
	{
		m_bActive = false, m_bFirst = true;
		m_pUniversalMap = pUniversalMap;
	}
	~CRefreshExtension() { m_pUniversalMap = NULL; }
	virtual bool CheckActivityOfEditor() { return m_bActive; }
	virtual void ActivateEditor()
	{
		if(m_bActive) return;
		m_bActive = m_bFirst = true;
		m_bTimeShow = false;
		m_NDrawCalls = 0;
	}
	virtual void OnDrawOnMapOverAll()
	{
		if(!m_bActive) return;
		if(m_bTimeShow)
		{
			if(m_NDrawCalls > 100) m_bActive = false;
			m_NDrawCalls++;
			ShowString(10, RealLy - 40, m_Str, &BigRedFont);
			return;
		}
		if(m_bFirst)
		{
			ShowString(10, RealLy - 40, "Refreshing...", &BigWhiteFont);
			m_bFirst = false;
		}
		else
		{
			float T = m_pUniversalMap->Refresh() / 1000.0f;
			sprintf(m_Str, "Refreshed in %.2f (s).", T);
			void ResetGroundCache();
			ResetGroundCache();
			m_bTimeShow = true;
		}
	}

private:
	int m_NDrawCalls;
	char m_Str[1024];
	bool m_bActive, m_bFirst, m_bTimeShow;
	UniversalMap *m_pUniversalMap;
};

extern UniversalMap g_UniversalMap;
CRefreshExtension g_RefreshExtension(&g_UniversalMap);

#endif