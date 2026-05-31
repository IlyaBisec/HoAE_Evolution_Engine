#pragma once
#include "MapTemplates.h"
//////////////////////////////////////////////////////////////////////////

class cvi_HotKey : public BaseClass
{
public:	
	int Level;
	bool VirtualUp;
	int AbIndex;
	SAVE(cvi_HotKey){
		REG_MEMBER(_int,Level);
		REG_MEMBER(_bool,VirtualUp);
		REG_MEMBER(_int,AbIndex);
	}ENDSAVE;
};

class cvi_HotKeys : public BaseClass
{
public:
};