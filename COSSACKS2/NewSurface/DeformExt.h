#ifndef __DeformExt__
#define __DeformExt__

#include "SelExt.h"

class DeformExt : public SelExt {
	DWORD OnLMB(SelExtParams &Params);
	DWORD OnRMB(SelExtParams &Params);
	void OnDraw(SelExtParams &Params);

	static bool ApproxByPlane(const SelExtParams &Params, cVector &Orig, cVector &Dir);
	static void DeformOneVertex(const cVector &Orig, const cVector &Dir, int Radius, OneMapVertex &vtx);
};

#endif // __DeformExt__