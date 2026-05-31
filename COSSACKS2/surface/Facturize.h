#ifndef __Facturize__
#define __Facturize__

#include "SelExt.h"

namespace Surface {

class Facturize : public SelExt {
public:
	Facturize() { m_nCurFacture = 0; }
	DWORD OnLMB(SelExtArgs &);
	DWORD OnRMB(SelExtArgs &);
	void OnDraw(SelExtArgs &);
	void OnDrawInterface();
	void OnActivate();
private:
	int m_nCurFacture;
	SurfSwatchCtrl m_Swatch;
	static bool SwatchCallBack(int index, int &idTex);
	static void Process(SurfRef &Ref, int nFacture, SelExtArgs* Args);
};

} // Surface

#endif // __Facturize__