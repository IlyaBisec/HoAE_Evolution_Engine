#ifndef __Optimize__
#define __Optimize__

#include "..\stdheader.h"
#include "..\GameExtension.h"
#include "Surface.h"

namespace Surface {

class Optimize : public GameExtension {
public:
	Optimize();
	bool CheckActivityOfEditor();
	void ClearActivityOfEditor();
	void ActivateEditor();
	void DrawEditorInterface();
private:
	char m_Stats[128];
	typedef enum { S_INIT, S_OPTIMIZE, S_PRESTATS, S_STATS } eStage;
	eStage m_Stage;
	bool m_fActive;
	DWORD m_tStatsStart;
};

} // Surface

#endif // __Refresh__