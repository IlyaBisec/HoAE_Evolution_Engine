#ifndef __Racist__
#define __Racist__

#include "SelExt.h"

namespace Surface {

class Racist : public SelExt {
public:
	DWORD OnLMB(SelExtArgs &);
	DWORD OnRMB(SelExtArgs &);
	void OnDraw(SelExtArgs &);
	void OnDrawInterface();
	void OnActivate();
private:
	void ActivateRace(int idRace);
	bool m_fInInterface;

	static bool SelCtrlCallback(int index, cStr &Name, DWORD &Color);
	SurfSelCtrl m_SelCtrl;
};

} // Surface

#endif // __Racist__