#ifndef __NAVIGATIONEXTENSION_H__
#define __NAVIGATIONEXTENSION_H__

#include "GameExtension.h"

class NavigationExtension : public GameExtension
{
public:
	virtual bool OnMouseHandling(int mx, int my, bool &LeftPressed, bool &RightPressed,
		int MapCoordX, int MapCoordY, bool OverMiniMap);
private:

};

#endif