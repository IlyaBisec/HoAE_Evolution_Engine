#ifndef __CFILLMODEEXTENSION__
#define __CFILLMODEEXTENSION__

#include "GameExtension.h"
#include "Surface.h"

class CFillmodeExtension : public GameExtension
{
public:
	CFillmodeExtension(UniversalMap *pUniversalMap)
	{
		m_bActive = false;
		m_pUniversalMap = pUniversalMap;
	}
	~CFillmodeExtension() { m_pUniversalMap = NULL; }
	virtual bool CheckActivityOfEditor() { return m_bActive; }
	virtual void ActivateEditor()
	{
		if(m_bActive) return;
		m_bActive = true;
		m_NDrawCalls = 0;
	}
	virtual void OnDrawOnMapOverAll()
	{
		if(!m_bActive) return;
		char *strMode[] = {"Fillmode: Wireframe", "Fillmode: Solid"};
		if(m_NDrawCalls == 0)
		{
			m_NMode = m_pUniversalMap->bWire ? 1 : 0;
			m_pUniversalMap->bWire = !m_pUniversalMap->bWire;
			void ResetGroundCache();
			ResetGroundCache();
		}
		ShowString(10, RealLy - 40, strMode[m_NMode], &BigWhiteFont);
		m_NDrawCalls++;
		if(m_NDrawCalls > 25) m_bActive = false;
	}

private:
	bool m_bActive;
	int m_NDrawCalls;
	int m_NMode; // 0 = Wireframe, 1 = Solid
	UniversalMap *m_pUniversalMap;
};

extern UniversalMap g_UniversalMap;
CFillmodeExtension g_FillmodeExtension(&g_UniversalMap);

#endif