#ifndef __Typify__
#define __Typify__

#include "SelExt.h"

namespace Surface {

class Typify : public SelExt {
public:
	DWORD OnLMB(SelExtArgs &);
	DWORD OnRMB(SelExtArgs &);
	void OnDraw(SelExtArgs &);
	void OnDrawInterface();
	static void TypeControl();
	void OnActivate();
private:
	static void ActivateType(int idType);
	static void Process(SurfRef &Ref, int idType, const SurfVertex &Center, int Radius);

	bool m_fInInterface;
	static bool SelCtrlCallback(int index, cStr &Name, DWORD &Color);
	SurfSelCtrl m_SelCtrl;
};

} // Surface

#endif // __Typify__