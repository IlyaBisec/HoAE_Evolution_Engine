#ifndef __NullExt__
#define __NullExt__

#include "GameExtension.h"

class NullExt : public GameExtension {
public:
	virtual bool CheckActivityOfEditor() { return false; }
	virtual void ActivateEditor() {}
};

extern int CurSurfaceType;
class SelectSurfaceType : public GameExtension {
	int SurfaceType;
public:
	SelectSurfaceType(int Type){
		SurfaceType=Type;
	}
	virtual bool CheckActivityOfEditor() {
		return false;
	}
	virtual void ActivateEditor() {
		CurSurfaceType=SurfaceType;
	}
};

#endif // __NullExt__