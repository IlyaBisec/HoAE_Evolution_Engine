#ifndef __LocalExporter__
#define __LocalExporter__

#include "Layer.h"

namespace Surface {

class LocalExporter : public SelExt {
public:
	DWORD OnLMB(SelExtArgs &);
	DWORD OnRMB(SelExtArgs &);
	void OnDraw(SelExtArgs &);
	void OnActivate();
	void OnClear();
private:
	int m_idMaskLayer;
	DWORD m_ClrMaskLayer;
};

} // Surface

#endif // __LocalExporter__