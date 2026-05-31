#pragma once
#include "..\ClassEngine\more_types.h"
#include "MapTemplates.h"
#include "vui_Action.h"
//////////////////////////////////////////////////////////////////////////
class cva_DeskAction: public vui_Action{ 
	virtual bool isExpandableFor(SimpleDialog* SD);
};
class cva_DeskArray: public cva_DeskAction{ 
public:	
	virtual int GetNUnit(){ return 0; };
	virtual void SetUnit(int ID, SimpleDialog* SD){};
	virtual void SetFrameState(SimpleDialog* SD);
	SAVE(cva_DeskArray){
		//REG_PARENT(vui_Action);
		//REG_AUTO(Name);
		//REG_MEMBER_EX(_bool,init,Init);
	}ENDSAVE;
};
