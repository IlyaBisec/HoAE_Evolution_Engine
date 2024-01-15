#include "CmdExt.h"
#include "NavExt.h"

//-----------------------------------------------------------------------------
// .ctor : void(eCmd)
//-----------------------------------------------------------------------------
CmdExt::CmdExt(eCmd Cmd) {
	m_Cmd = Cmd;
	m_pUniversalMap = &g_UniversalMap;
}

//-----------------------------------------------------------------------------
// CmdExt::CheckActivityOfEditor : bool()
//-----------------------------------------------------------------------------
bool CmdExt::CheckActivityOfEditor() {
	return false;
}

//-----------------------------------------------------------------------------
// CmdExt::ActivateEditor : void()
//-----------------------------------------------------------------------------
void CmdExt::ActivateEditor() {
	char NewMapText[] = "Are you sure you want to create new map?";
	char NewMapCaption[] = "Confirm Create New Map";
	switch(m_Cmd) {
		case CmdInvertAllNormals:
			m_pUniversalMap->InvertNormals();
			break;
		case CmdNewMap:
			extern HWND g_hWnd;
			if(MessageBox(g_hWnd, NewMapText, NewMapCaption, MB_YESNO) == IDYES) {
				m_pUniversalMap->Create();
			}
			break;
		case CmdToggleFillMode:
			extern eTerraFill g_TerraFill;
			if(g_TerraFill == TFWire) {
				g_TerraFill = TFSolid;
			} else {
				g_TerraFill = TFWire;
			}
			break;
		case CmdToggleShowMapOfHeights:
			extern bool g_ShowMapOfHeights;
			g_ShowMapOfHeights = !g_ShowMapOfHeights;
			break;
		case CmdToggleShowTriNormals:
			extern bool g_fShowTriNormals;
			g_fShowTriNormals = !g_fShowTriNormals;
			break;
		case CmdToggleShowVertNormals:
			extern bool g_fShowVertNormals;
			g_fShowVertNormals = !g_fShowVertNormals;
			break;
		case CmdToggleNavType:
			if(g_NavExt.Args.Type == NavArgs::Adv) {
				g_NavExt.Args.Type = NavArgs::InGame;
			} else if(g_NavExt.Args.Type == NavArgs::InGame) {
				g_NavExt.Args.Type = NavArgs::Adv;
			}
			break;
	}
	void ResetGroundCache();
	ResetGroundCache();
}