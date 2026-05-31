#pragma once
//#include "stdheader.h"
#include "MapTemplates.h"

//////////////////////////////////////////////////////////////////////////

class cvi_Phrase: public BaseClass
{
public:
	char* talkerName;
	DWORD talkerColor;
	char* Message;
	bool  Deleted;
};

class cvi_MsgDialog: public BaseClass
{
public:
	char*	Title;
	ClassArray<cvi_Phrase> Phrases;	
	SAVE(cvi_MsgDialog){
		REG_AUTO(Phrases);
	}ENDSAVE;
};

class cvi_RomeHelp: public BaseClass
{
public:
	ClassArray<cvi_MsgDialog> MsgDialogs;
	ClassArray<cvi_MsgDialog> MsgObjectives;
	ClassArray<cvi_MsgDialog> MsgHints;
	SAVE(cvi_RomeHelp){
		REG_AUTO(MsgDialogs);
		REG_AUTO(MsgObjectives);
		REG_AUTO(MsgHints);
	}ENDSAVE;
};

bool GetRomeHelp(cvi_RomeHelp* RH);

regAc(cva_RomeHelp, vfS	
	ClassRef<TabDesk> tdObjective;
	ClassRef<TabDesk> tdDialog;
	ClassRef<TabDesk> tdHint;
	ClassRef<TabButton> tbDialog;
	ClassRef<TabButton> tbHint;
	,
	REG_AUTO(tdObjective);
	REG_AUTO(tdDialog);
	REG_AUTO(tdHint);
	REG_AUTO(tbDialog);
	REG_AUTO(tbHint);
);
