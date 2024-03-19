#ifndef __CUNDOEXTENSION__
#define __CUNDOEXTENSION__

#include "GameExtension.h"
#include "Surface.h"

class CUndoExtension : public GameExtension
{
public:
	CUndoExtension(UniversalMap *pUniversalMap)
	{
		m_bActive = false, m_bFirst = true;
		m_pUniversalMap = pUniversalMap;
	}
	~CUndoExtension() { m_pUniversalMap = NULL; }
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
			ShowString(10, RealLy - 40, "Undoing...", &BigWhiteFont);
			m_bFirst = false;
		}
		else
		{
			m_pUniversalMap->Backup_Undo();
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
CUndoExtension g_UndoExtension(&g_UniversalMap);

#endif