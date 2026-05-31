#pragma once
//////////////////////////////////////////////////////////////////////////
#include "MapTemplates.h"
//////////////////////////////////////////////////////////////////////////
class mai_Script: public BaseClass
{
public:
	mai_Script(void);
	~mai_Script(void);
	//
	bool Enable;
	SAVE(mai_Script){	
		REG_LOADSAVE("AI\\mai\\","*.mai_Script.xml");
		REG_MEMBER(_bool, Enable);
		//REG_MEMBER_EX(_bool,init,Init);
	}ENDSAVE;
};
