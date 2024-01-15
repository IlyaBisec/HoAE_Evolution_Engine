#ifndef __cPolynom__
#define __cPolynom__

#include "cUtils.h"

namespace comms {

class cPolynom {
public:
	static int CalcRoots2(float a, float b, float c, float *pRoots);
	static bool CalcLowestRoot(float a, float b, float c, float MaxRoot, float *pRoot);
};

//-----------------------------------------------------------------------------
// cPolynom::CalcRoots2 : int(float, float, float, float *)
//-----------------------------------------------------------------------------
inline int cPolynom::CalcRoots2(float a, float b, float c, float *pRoots) {
	float D = b * b - 4.0f * c;
	if(D < 0.0f) {
		return 0;
	}
	float SqrtD = cMath::Sqrt(D);
	pRoots[0] = (- b - SqrtD) / (2.0f * a);
	pRoots[1] = (- b + SqrtD) / (2.0f * a);
	if(D > 0.0f) {
		return 2;
	} else {
		return 1;
	}
}

} // comms

#endif // __cPolynom__