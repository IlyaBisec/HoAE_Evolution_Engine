#ifndef __Smooth__
#define __Smooth__

#include "SelExt.h"

namespace Surface {

class Smooth : public SelExt {
public:
	Smooth() {
		m_nSelectedMap = -1;
		m_DMScale = 1.0f;
		m_DMAngle = 0.0f;
	}
	DWORD OnLMB(SelExtArgs &Args);
	DWORD OnRMB(SelExtArgs &Args);
	void OnDraw(SelExtArgs &Args);
	void OnActivate();
	void OnDrawInterface();
	bool OnWheel(int nDelta);
private:
	SurfVertex m_DMCenter;
	float m_DMScale, m_DMAngle;
	int m_nSelectedMap;
	static int GetDMIdFromIndex(int index);

	static bool SwatchCtrlCallback(int index, int &idTex);
	SurfSwatchCtrl m_SwatchCtrl;

	void CalcUV(const SurfVertex &r, uv_pair &uv) const;
	bool ToBaseMesh(const SurfMesh &, BaseMesh &) const;
};

} // Surface

#endif // __Smooth__