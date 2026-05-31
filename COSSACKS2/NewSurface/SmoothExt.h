#ifndef __SmoothExt__
#define __SmoothExt__

#include "SelExt.h"

class SmoothExt : public SelExt {
	DWORD OnLMB(SelExtParams &Params);
	void OnDraw(SelExtParams &Params);
};

#endif // __SmoothExt__