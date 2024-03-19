#ifndef __NullExtension__
#define __NullExtension__

#include "GameExtension.h"

class NullExtension : public GameExtension {
public:
	virtual bool CheckActivityOfEditor() {
		return false;
	}
	virtual void ActivateEditor() {
	}
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

#endif // __NullExtension__