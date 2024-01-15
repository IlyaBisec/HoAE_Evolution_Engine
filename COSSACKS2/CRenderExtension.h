#ifndef __CRENDEREXTENSION__
#define __CRENDEREXTENSION__

#include "GameExtension.h"
#include "Surface.h"

class CRenderExtension : public GameExtension
{
public:
	CRenderExtension(UniversalMap *pUniversalMap)
	{
		m_bActive = false, m_bFirst = true;
		m_pUniversalMap = pUniversalMap;
	}
	~CRenderExtension() { m_pUniversalMap = NULL; }
	virtual bool CheckActivityOfEditor() { return m_bActive; }
	virtual void ActivateEditor()
	{
		if(m_bActive) return;
		m_bActive = m_bFirst = true;
	}
	virtual void OnDrawOnMapOverAll()
	{
		if(!m_bActive) return;
		if(m_bFirst)
		{
			ShowString(10, RealLy - 40, "Rendering...", &BigWhiteFont);
			m_bFirst = false;
		}
		else
		{
			m_pUniversalMap->Render();
			void ResetGroundCache();
			ResetGroundCache();
			m_bActive = false;
		}
	}

private:
	bool m_bActive, m_bFirst;

	UniversalMap *m_pUniversalMap;
};

extern UniversalMap g_UniversalMap;
CRenderExtension g_RenderExtension(&g_UniversalMap);

#endif