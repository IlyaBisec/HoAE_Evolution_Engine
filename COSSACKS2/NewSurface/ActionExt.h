#ifndef __ActionExt__
#define __ActionExt__

#include "GameExtension.h"
#include "Surface.h"

extern UniversalMap g_UniversalMap;

typedef enum { ATInvertAllNormals, ATNewMap, ATFillMode, ATShowMapOfHeights, ATShowTriNormals, ATShowVertNormals } eActionType;

class ActionExt : public GameExtension {
public:
	ActionExt(eActionType);
	virtual bool CheckActivityOfEditor();
	virtual void ActivateEditor();
private:
	eActionType m_ActionType;
	UniversalMap *m_pUniversalMap;
};

// ActionExt
ActionExt::ActionExt(eActionType ActionType) {
	m_ActionType = ActionType;
	m_pUniversalMap = &g_UniversalMap;
} // ActionExt

// ActionExt::CheckActivityOfEditor
bool ActionExt::CheckActivityOfEditor() {
	return false;
} // ActionExt::CheckActivityOfEditor

// ActionExt::ActivateEditor
void ActionExt::ActivateEditor() {
	char NewMapText[] = "Are you sure you want to create new map?", NewMapCaption[] = "Confirm Create New Map";
	switch(m_ActionType) {
		case ATInvertAllNormals:
			m_pUniversalMap->InvertNormals();
			break;
		case ATNewMap:
			extern HWND g_hWnd;
			if(MessageBox(g_hWnd, NewMapText, NewMapCaption, MB_YESNO) == IDYES) {
				void CreateNew3DMap(int, DWORD);
				CreateNew3DMap(16000,0xff699d2d);
			}
			break;
		case ATFillMode:
			extern eTerraFill g_TerraFill;
			if(g_TerraFill == TFWire) {
				g_TerraFill = TFSolid;
			} else {
				g_TerraFill = TFWire;
			}
			break;
		case ATShowMapOfHeights:
			extern bool g_ShowMapOfHeights;
			g_ShowMapOfHeights = !g_ShowMapOfHeights;
			break;
		case ATShowTriNormals:
			extern bool g_fShowTriNormals;
			g_fShowTriNormals = !g_fShowTriNormals;
			break;
		case ATShowVertNormals:
			extern bool g_fShowVertNormals;
			g_fShowVertNormals = !g_fShowVertNormals;
			break;
	}
	void ResetGroundCache();
	ResetGroundCache();
} // ActionExt::ActivateEditor

#endif // __ActionExt__