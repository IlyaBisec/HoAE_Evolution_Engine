#include "cPolynom.h"

namespace comms {

//-------------------------------------------------------------------------------------
// cPolynom::CalcLowestRoot : bool(float, float, float, float, float *)
//-------------------------------------------------------------------------------------
bool cPolynom::CalcLowestRoot(float a, float b, float c, float MaxRoot, float *pRoot) {
	float r[2];
	int n = CalcRoots2(a, b, c, r);
	if(!n) return false;

	if(r[0] > r[1]) {
		Swap(r[0], r[1]);
	}
	if(r[0] > 0 && r[0] < MaxRoot) {
		*pRoot = r[0];
		return true;
	}
	if(r[1] > 0 && r[1] < MaxRoot) {
		*pRoot = r[1];
		return true;
	}
	return false;
}

} // comms