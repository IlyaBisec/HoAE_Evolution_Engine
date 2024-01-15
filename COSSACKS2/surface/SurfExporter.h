#ifndef __SurfExporter__
#define __SurfExporter__

#include "..\GameExtension.h"
#include "..\Surface\Surface.h"

namespace Surface {

class SurfExporter : public GameExtension {
public:
	virtual bool CheckActivityOfEditor() { return false; }
	virtual void ActivateEditor();
	static void CheckExt3(char *pStr, const char *pExt);
	static void Export(const SurfMesh &Mesh);
};

} // Surface

#endif // __SurfExporter__