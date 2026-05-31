#pragma once
#include "MapTemplates.h"
#include "MapDiscr.h"
#include "vui_Action.h"
#include "cva_DeskArray.h"
/////////////////////////////////////////////////////////////////////////////////////

class cvi_PostGameStatistic
{
public:
	cvi_PostGameStatistic(void);
	~cvi_PostGameStatistic(void);

	char* Player;
	Nation* NT;
	DynArray<vsInfoArmy> Units;
	DynArray<word> UnitsNIndex;

	void Create(char* PlayerName, Nation* Nat);
};

class cva_DA_StatUnits: public cva_DeskArray{ 
public:	
	virtual int GetNUnit();
	virtual void SetUnit(int ID, SimpleDialog* SD);
	SAVE(cva_DA_StatUnits){
		REG_PARENT(vui_Action);
		//REG_AUTO(Name);
		//REG_MEMBER_EX(_bool,init,Init);
	}ENDSAVE;
};

///////////////////////////////////////////
// EW2 Campaign Statistics
///////////////////////////////////////////
regAc(cva_CampStat_Show, vfL);
regAc(cva_CampStat_Mode, vfI vfS);
regAc(cva_CampStat_PlFlag, vfS);
regAc(cva_CampStat_PlName, vfS);
regAc(cva_CampStat_PlScore, vfS);
regAc(cva_CampStat_Graphs, vfS);
regAc(cva_CampStat_Oy, vfS);
regAc(cva_CampStat_Ox, vfS);
regAc(cva_CampStat_Player, vfS);
regAc(cva_CampStat_Color, vfS);


