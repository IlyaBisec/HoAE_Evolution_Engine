#ifndef __SelExt__
#define __SelExt__

#include "..\stdheader.h"
#include "..\GameExtension.h"
#include "Surface.h"
#include "ExtOptions.h"
#include "ITerrain.h"

struct SelExtParams {
	VertexBasedMeshRef RefTo;
	DWORDS VertsTo;
	int Radius;
};

class SelExt : public GameExtension {
public:
	enum InvalFlags {
		IF_CACHE = 1 << 0,
		IF_GEOMETRY = 1 << 1 | IF_CACHE
	};

	SelExt();
	virtual bool CheckActivityOfEditor();
	virtual void ClearActivityOfEditor();
	virtual void ActivateEditor();
	virtual bool OnMouseWheel(int);
	virtual bool OnMouseHandling(int mx, int my, bool &LMB, bool &RMB, int, int, bool);
	virtual void OnDrawOnMapOverAll();
	virtual void DrawEditorInterface();

	virtual void OnActivate() {}
	virtual void OnClear() {}
	virtual DWORD OnLMB(SelExtParams &Params) { return 0; }
	virtual DWORD OnRMB(SelExtParams &Params) { return 0; }
	virtual void OnDraw(SelExtParams &Params) {}
	virtual void OnDrawInterface(SelExtParams &Params) {}

	static void DrawWindow(int x0, int y0, int x1, int y1, DWORD argbColor, int Border);
	static void DrawGrid(const VertexBasedMeshRef &Ref, DWORD argbColor);
private:
	bool m_fActive;
	UniversalMap *m_pUniversalMap;
	int *m_pSelRadius;

	SelExtParams m_Params;
};

extern bool g_fSpaceIsBlocked;

#endif // __SelExt__