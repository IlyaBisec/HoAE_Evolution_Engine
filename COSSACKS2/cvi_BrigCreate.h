#pragma once
#include "MapTemplates.h"
#include "vui_Action.h"
#include "vui_InterfSynchro.h"

//////////////////////////////////////////////////////////////////////////
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
CEXPORT int CreateBrigInZone(byte NI, int x, int y, word OfType, word SlType, byte Amount);
OfficerRecord* GetOfficerRecord(byte NI, word UnitType);
extern City CITY[8];
//void CorrectBrigadesSelection(byte NT);
//void ImCorrectBrigadesSelection(byte NT);
DLLEXPORT void ClearSelection(byte Nat);
//void SelectBrig(byte NI,word ID);
void SelBrigade(byte NI,byte Type,byte BNat,int ID);
//////////////////////////////////////////////////////////////////////////

struct sv_Param_CreateBrig{
	// input
	int ReqAmount;
	byte NI;
	word NIndex;	
	// output
	UnitsGroup UG;
	//
	static bool AddUnitCallBack(OneObject* OB,void* Param);
};

class cvi_BrigCreate
{
public:
	cvi_BrigCreate(void);
	~cvi_BrigCreate(void);

	bool mInit;
	_str mDialogName;
	DialogsDesk* mDialog;
	int mRadius;

	SimpleDialog* Init();
	void Process(OneObject* OB, bool Visible);

	OfficerRecord* mOR;
	OneObject* mOB;
	bool mActive;
	DynArray<int> mCrew;

	sv_Param_CreateBrig mParam;
};

extern cvi_BrigCreate vBrigCreate;

// vui_Actions
regA(cvi_A_BC_Active, vfS, vfL);
regA(cvi_A_BC_Crew, vfS, vfL);
regA(cvi_A_BC_CrewAmount, vfS);

// vui_InterfSynchro
class cvi_IS_BC_Create: public vui_IS_Base<cvi_IS_BC_Create>{
public:
	void SynchroAction();	
	struct{
		//byte NI;
		word ObjID;
		word ObjSN;
		word MembNIndex;
		word ODIndex;
	}Data;
};