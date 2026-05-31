#ifndef __CmdExt__
#define __CmdExt__

#include "..\stdheader.h"
#include "..\GameExtension.h"
#include "Surface.h"

typedef enum { CmdInvertAllNormals, CmdNewMap, CmdToggleFillMode, CmdToggleShowMapOfHeights,
CmdToggleShowTriNormals, CmdToggleShowVertNormals, CmdToggleNavType} eCmd;

class CmdExt : public GameExtension {
public:
	CmdExt(eCmd);
	virtual bool CheckActivityOfEditor();
	virtual void ActivateEditor();
private:
	eCmd m_Cmd;
	UniversalMap *m_pUniversalMap;
};

#endif // __CmdExt__