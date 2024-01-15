#ifndef __Deform__
#define __Deform__

#include "SelExt.h"

namespace Surface {

class Deform : public SelExt {
public:
	typedef enum { DT_PUSH, DT_PULL } eDeformType;
	DWORD OnLMB(SelExtArgs &Args);
	DWORD OnRMB(SelExtArgs &Args);
	void OnDraw(SelExtArgs &Args);
	static bool ApproxByPlane(const SurfRef &Ref, dVector &Orig, dVector &Dir);
private:
	static DWORD Process(SurfRef &Ref, eDeformType Type, const SurfVertex &Centre, int Radius ,SelExtArgs *Args);
	static void DeformOneVertex(const dVector &Orig, const dVector &Dir, const SurfVertex &Centre, int Radius, SurfVertex &vtx);
	static void DisplaceOneVertex(SurfVertex &u);
	static void PlanarDeformOneVertex(const SurfVertex &Centre, int Radius, SurfVertex &vtx);
	static const int dDeform = 10;
};

} // Surface

#endif // __Deform__