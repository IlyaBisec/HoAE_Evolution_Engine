#ifndef __LocalGenerator__
#define __LocalGenerator__

#include "Layer.h"

namespace Surface {

class LocalGenerator : public SelExt {
public:
	DWORD OnLMB(SelExtArgs &);
	DWORD OnRMB(SelExtArgs &);
	void OnDraw(SelExtArgs &);
	void OnActivate();
	void OnClear();
	static void InitMaskLayer(int &id, DWORD &Color);
private:
	int m_idMaskLayer;
	DWORD m_ClrMaskLayer;
};

} // Surface

#endif // __LocalGenerator__