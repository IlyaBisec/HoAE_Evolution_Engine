#pragma once
#include "MapTemplates.h"
#include "vui_Action.h"

//////////////////////////////////////////////////////////////////////////
typedef int vtf_GetResource(int Res);
typedef int vtf_AddResource(int Res, int Amount);

class cvi_Market
{
public:
	cvi_Market(void);
	~cvi_Market(void);

	bool init[6];
	int EcResID[6];
	int EcResIcn[6];
	int ChertaIcn;
	int BuyIcn;
	int ClearIcn;
	int P10Icn;
	int P100Icn;
	int P1000Icn;
	int P10000Icn;
	int M10Icn;
	int M100Icn;
	int M1000Icn;
	int M10000Icn;
	double ResCostBuy  [6];
	double ResCostSell [6];
	double ResCostBuy0 [6];
	double ResCostSell0[6];
	double ResCostBuy1 [6];
	double ResCostSell1[6];
	double ResCostBuy2 [6];
	double ResCostSell2[6];
	double ExpBUY[6];
	double ExpSELL[6];
	double TimeBUY[6];
	double TimeSELL[6];
	double Nalog[6];
	int NalogResID[6];
	int NeedToBuy;
	int NeedToSell;
	int ResSell;
	int ResBuy;
	
	void Load();
	SimpleDialog* Init();
	//void CreateInterface(IconSet* ISET);
	void PerformOperation(byte Nation,byte SellRes,byte BuyRes,int SellAmount);
	void Process();

	int GetNeedToBuy();

	byte GetSellResID(){ return ResSell>=0?EcResID[ResSell]:ResSell; }
	byte GetBuyResID(){ return ResBuy>=0?EcResID[ResBuy]:ResBuy; }
	void SetResSell(byte ResID);
	void SetResBuy(byte ResID);

	bool ChangeResPrice(byte ResID, float Changer){//grey [15.01.2005]
		for(int i=0;i<6;i++) 
			if(EcResID[i]==ResID){
				ResID=i;				
				break;
			}
		ResCostBuy[ResID]=float(ResCostBuy[ResID])*Changer;
		if(ResCostBuy[ResID]<1) ResCostBuy[ResID]=1;
		ResCostSell[ResID]=float(ResCostSell[ResID])*Changer;
		if(ResCostSell[ResID]<1) ResCostSell[ResID]=1;
		return true;
	}

	bool mInit;
	_str mDialogName;
	DialogsDesk* mDialog;

	vtf_GetResource* mGet;
	vtf_AddResource* mAdd;

};

extern cvi_Market* vMarket;
extern cvi_Market vMarketInGame;

// vui_Actions
regAc(cvi_MarketDesk,vfS
	ClassRef<TextButton> SellTxt;	// resource name
	ClassRef<TextButton> BuyTxt;
	,
	REG_AUTO(SellTxt);
	REG_AUTO(BuyTxt);
);
regAc(cvi_M_SellAmount,vfS
	bool resName,
	REG_MEMBER(_bool,resName);
);
regA(cvi_M_SellSelect,vfS,vfL,vfRt);
regAc(cvi_M_BuyAmount,vfS
	bool resName,
	REG_MEMBER(_bool,resName);
);
regA(cvi_M_BuySelect,vfS,vfL,vfRt);
regA(cvi_M_Buy,vfL,vfRt);
regA(cvi_M_Reset,vfL,vfRt);
regA(cvi_M_ChangeAmount,vfL,vfRt);

// Extertnals
void EERR();
int GetIconByName(char* Name);
int GetResID(char*);
void CmdTorg(byte NI,byte SellRes,byte BuyRes,int SellAmount);
cvi_Market* v_addMarket(DialogsSystem* Parent, int x, int y, vtf_GetResource* Get, vtf_AddResource* Add);
