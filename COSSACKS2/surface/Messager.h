#ifndef __Messager__
#define __Messager__

#include "CmdExt.h"

namespace Surface {

class Messager : public GameExtension {
public:
	Messager();
	void OnDrawOnMapOverAll();
	void DrawEditorInterface();
	void ShowMessage(const char *pStr);
private:
	cStr m_Msg;
	DWORD m_tStart;
	bool m_fActive;
	bool m_fFirstFrameWasShown;

	static const int tShowMsg = 2; // seconds
};

} // Surface

extern Surface::Messager g_Messager;

#endif // __Messager__