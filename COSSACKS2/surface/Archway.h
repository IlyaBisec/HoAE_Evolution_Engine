#ifndef __Archway__
#define __Archway__

#include "SelExt.h"

namespace Surface {

class Archway : public SelExt {
	void OnActivate();
	DWORD OnLMB(SelExtArgs &Args);
	DWORD OnRMB(SelExtArgs &Args);
	void OnDraw(SelExtArgs &Args);
	bool OnWheel(int);
	static void DrawArchway(const SurfArchway &Way, const cColor &Color);
private:
	static const int HeightDelta = 10;
	int m_MaxHeight, m_Radius;
};

} // Surface

#endif // __Deform__