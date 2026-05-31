#ifndef __Layer__
#define __Layer__

#include "SelExt.h"

namespace Surface {

class Layer : public SelExt {
public:
	DWORD OnLMB(SelExtArgs &);
	DWORD OnRMB(SelExtArgs &);
	void OnDraw(SelExtArgs &);
	void OnDrawInterface();
	void OnActivate();
	void OnClear();
	static int FindLayerByName(const char *pLayerName, DWORD *pColor = NULL); // returns index, -1 if layer is not found
private:
	void ActivateLayer(int idLayer);
//	static int ScanXXOnDigits();
	static int ScanXOnArrows();
	static const DWORD ScanDelay = 300; // (ms)
	bool m_fInInterface;

	static bool SelCtrlCallback(int index, cStr &Name, DWORD &Color);
	SurfSelCtrl m_SelCtrl;

	static void ClearLayers(const SurfVertex &Center, int Radius);
};

} // Surface

#endif // __Layer__