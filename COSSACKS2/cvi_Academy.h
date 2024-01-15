#pragma once
#include "MapTemplates.h"
#include "vui_Action.h"

//////////////////////////////////////////////////////////////////////////

class cvi_Academy
{
public:
	cvi_Academy(void);
	~cvi_Academy(void);

	bool mInit;
	_str mDialogName;
	DialogsDesk* mDialog;

	SimpleDialog* Init(char* FileXML);
	void Process(DialogsSystem* Parent);
};

extern cvi_Academy vAcademy;

// vui_Actions
regA(cvi_AcademyDesk,vfS);
regA(cvi_Acd_Inf,vfS);
regA(cvi_Acd_Cav,vfS);
regA(cvi_Acd_Can,vfS);
regA(cvi_Acd_Peas,vfS);
regA(cvi_Acd_Spend,vfS);

// Extertnals
DLLEXPORT int GetBrigadsAmount0(byte NI);
DLLEXPORT int GetExtraction(byte ResID);
extern City CITY[8];
int GetResID(char*);
