
#include "stdheader.h"
#include "../../AI_Scripts.h"
#include "MissionScript.h"

//For create mission scrip.

MISSION_SCRIPTS	MISSION_SCRIPT;

DialogsSystem	DS_MS_LV;
ClassEditor		CE_MS_LV;
bool			MS_LV_VIS = false;
bool			MS_LV_CRT = true;

void	CREATE_SCRIPT(){
	
	if (MS_LV_CRT) {
		RegMissionScriptClass();
		MS_LV_CRT = false;
		CE_MS_LV.ClearAll();
		CE_MS_LV.CreateFromClass( &DS_MS_LV,10,10,700,700, &MISSION_SCRIPT, CED_MEMBERS|CED_TREE );

	}
	
	//if ((GetKeyState(VK_CONTROL)&0x8000)&&(GetKeyState('E')&0x8000)) {
	//	MS_LV_VIS = true;
	//}
	MS_LV_VIS = true;// grey [17.03.2004]
	while (MS_LV_VIS) {
		ProcessMessages();
		CE_MS_LV.Process();
		DS_MS_LV.ProcessDialogs();
		DS_MS_LV.RefreshView();
		
		if (GetKeyState(VK_ESCAPE)&0x8000) {
			MS_LV_VIS = false;
		}
	}

}



//*************************************************************************************
//*************************  BASE QUEST SCRIPT CLASS   ********************************
//*************************************************************************************


//FOR CREATE DIRECTION DATA BASE, SECTORS ATTACK.
int	CDirectonDataBase::getDirection(int from, int to){
	for (int i=0; i<ELEM.GetAmountOfElements(); i++){
		if (ELEM[i]->FromSecID==from&&ELEM[i]->ToSecID==to) {
			return	ELEM[i]->Direction;
		}
	}
	return	-1;
}



