#ifndef __Scissors__
#define __Scissors__

#include "SelExt.h"

namespace Surface {

class Scissors : public SelExt {
	DWORD OnLMB(SelExtArgs &Args);
	void OnDraw(SelExtArgs &Args);
	void OnActivate();
	void OnClear();
};

} // Surface

#endif // __Scissors__