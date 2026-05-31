#ifndef __CCTRLVEXTENSION__
#define __CCTRLVEXTENSION__

#include "GameExtension.h"
#include "Surface.h"

class CCtrlVExtension : public GameExtension
{
public:
	CCtrlVExtension(CUniversalMapClipboard *pUniversalMapClipboard, UniversalMap *pUniversalMap)
	{
		m_pUniversalMapClipboard = pUniversalMapClipboard;
		m_pUniversalMap = pUniversalMap;
		IsActive = false;
	}
	virtual bool CheckActivityOfEditor();
	virtual void ClearActivityOfEditor();
	virtual bool OnMouseHandling(int mx, int my, bool &LeftPressed, bool &RightPressed,
		int MapCoordX, int MapCoordY, bool OverMiniMap);
	virtual bool OnMouseWheel(int Delta);
	virtual void OnDrawOnMapOverAll();
	virtual void ActivateEditor();
private:
	bool IsActive;
	CUniversalMapClipboard *m_pUniversalMapClipboard;
	UniversalMap *m_pUniversalMap;
	int CoordX, CoordY, CoordZ;
};

#endif