#ifndef __RefreshExt__
#define __RefreshExt__

#include "..\stdheader.h"
#include "..\GameExtension.h"
#include "Surface.h"

class RefreshExt : public GameExtension {
public:
	RefreshExt();
	RefreshExt(eSSIllumination Illumination);
	bool CheckActivityOfEditor();
	void ClearActivityOfEditor();
	void ActivateEditor();
	void DrawEditorInterface();
private:
	char m_Stats[128];
	typedef enum { SLabel, SRefresh, SStats } eStage;
	eStage m_Stage;
	bool m_fActive, m_fIlluminate;
	eSSIllumination m_Illumination;
	UniversalMap *m_pUniversalMap;
	DWORD m_tStatsStart;
};

#endif // __RefreshExt__