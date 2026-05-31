#ifndef __HiResScreenShot__
#define __HiResScreenShot__

#include "CmdExt.h"

namespace Surface {

class HiResScreenShot : public GameExtension {
public:
	HiResScreenShot();
	void MakeScreenShot();
	void OnDrawOnMapOverAll();
private:
	DWORD m_msStart;
	int m_ScrWidth, m_ScrHeight, m_ScrBpp, m_ScrRR;
	bool m_UIWasHidden;
	bool m_IsActive;

	static const int msDelayBeforeCapture = 3000;
};

} // Surface

extern Surface::HiResScreenShot g_HiResScreenShot;

#endif // __HiResScreenShot__