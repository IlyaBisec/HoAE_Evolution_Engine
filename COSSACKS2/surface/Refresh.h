#ifndef __Refresh__
#define __Refresh__

#include "..\stdheader.h"
#include "..\GameExtension.h"
#include "Surface.h"

namespace Surface {

class Refresh : public GameExtension {
public:
	Refresh();
	bool CheckActivityOfEditor();
	void ClearActivityOfEditor();
	void ActivateEditor();
	void DrawEditorInterface();
	typedef enum { A_CENTER, A_CORNER } eAlign;
	static void Message(cStr &Msg, eAlign = A_CENTER);
	static const int tShowStats = 6; // seconds
private:
	void DirectPassMessage();
	void LODsPassMessage();
	int m_CurPass, m_PassesCount;
	char m_Stats[128];
	typedef enum { S_LABEL, S_REFRESH, S_LOCAL, S_DIRECTPASSES, S_TRYINITLODSPASSES,
		S_LODSPASSES, S_PRESTATS, S_STATS } eStage;
	eStage m_Stage;
	bool m_fActive;
	DWORD m_tStatsStart;
	static const int dBorder = 3;
	static const int dCornerOffset = 20;
};

} // Surface

#endif // __Refresh__