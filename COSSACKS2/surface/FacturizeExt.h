#ifndef __FacturizeExt__
#define __FacturizeExt__

#include "SelExt.h"

struct feSample {
	BaseMesh bmSample, bmInfoBase;
	int x0, y0, x1, y1;
	struct {
		cStr WxH;
		cStr Title;
	} Info;
	bool IsPicked(int, int) const;
};

// feSample::IsPicked
inline bool feSample::IsPicked(int xMouse, int yMouse) const {
	// *********     * - included
	// *       .     . - excluded
	// *       .
	// *........
	return xMouse >= x0 && xMouse < x1 && yMouse >= y0 && yMouse < y1;
} // feSample::IsPicked

typedef cList<feSample> feSamples;

struct feSwatch {
	feSamples Samples;
	feSample ActiveSample;
};

typedef enum { FT_CUBIC, FT_VERTICAL, FT_HORIZONTAL } eFactType;

class FacturizeExt : public SelExt {
public:
	static eFactType GetKeyFTState();
	DWORD OnLMB(SelExtParams &Params);
	DWORD OnRMB(SelExtParams &Params);
	void OnDraw(SelExtParams &Params);
	void OnDrawInterface(SelExtParams &Params);
	void OnActivate();
	void OnClear();

	feSwatch m_Swatch;
	void GenOneSample(int x, int y, int L, int nFacture, feSample &Sample);
	void GenSwatch();
	void DrawOneSample(feSample &Sample);
	void DrawSwatch(int nCurSel);
	int GetCurSel();
	static void FacturizeOneVertex(OneMapVertex &vtx, int nFacture, eFactType FactType);
	void ActivateFacture(int nFacture);
};

#endif // __FacturizeExt__