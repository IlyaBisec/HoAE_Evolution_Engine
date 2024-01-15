#ifndef __Generate__
#define __Generate__

#include "..\stdheader.h"
#include "..\GameExtension.h"
#include "Surface.h"

namespace Surface {

class Generate : public GameExtension {
public:
	Generate();
	bool CheckActivityOfEditor();
	void ClearActivityOfEditor();
	void ActivateEditor();
	void DrawEditorInterface();
private:
	void PassMessage();
	int m_CurPass;
	static void Message(cStr &Msg);
	char m_Stats[128];
	typedef enum { S_INIT, S_PASSES, S_PRESTATS, S_STATS } eStage;
	eStage m_Stage;
	bool m_fActive;
	DWORD m_tStatsStart;
};

} // Surface

#endif // __Refresh__