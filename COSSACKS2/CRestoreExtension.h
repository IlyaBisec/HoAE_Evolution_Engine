#ifndef __CRESTOREEXTENSION__
#define __CRESTOREEXTENSION__

#include "GameExtension.h"
#include "Surface.h"

class CRestoreExtension : public GameExtension
{
public:
	CRestoreExtension(UniversalMap *pUniversalMap)
	{
		m_bActive = false, m_bFirst = true;
		m_pUniversalMap = pUniversalMap;
	}
	~CRestoreExtension() { m_pUniversalMap = NULL; }
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
			ShowString(10, RealLy - 40, "Restoring...", &BigWhiteFont);
			m_bFirst = false;
		}
		else
		{
			m_pUniversalMap->Backup_reStore();
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
CRestoreExtension g_RestoreExtension(&g_UniversalMap);

#endif