#include "stdheader.h"
#include "UnitsInterface.h"
#include ".\cvi_market.h"

//////////////////////////////////////////////////////////////////////////
cvi_Market* vMarket=NULL;
cvi_Market vMarketInGame;
//////////////////////////////////////////////////////////////////////////
// turn map market for Grey
cvi_Market* v_addMarket(DialogsSystem* Parent, int x, int y, vtf_GetResource* Get, vtf_AddResource* Add){
	cvi_Market* M=new cvi_Market;
	M->Load();
	M->mGet=Get;
	M->mAdd=Add;
	M->mInit=true;
	M->mDialogName="dialogs\\v\\TURN_MARKET.DIALOGSDESK.DIALOGS.XML";	
	M->mDialog=new DialogsDesk;
	if(M->mDialog->SafeReadFromFile(M->mDialogName.pchar())){
		M->mDialog->AllocPtr=(char*)M;		
		M->mDialog->Setx(x);
		M->mDialog->Sety(y);		
		Parent->DSS.Add(M->mDialog);
		return M;
	}else{
		delete M->mDialog;
		delete M;
	};
	return NULL;
};
//
cvi_Market::cvi_Market(void)
{
	mInit=false;
	mDialog=NULL;
	mGet=NULL;
	mAdd=NULL;
}

cvi_Market::~cvi_Market(void)
{
}

SimpleDialog* cvi_Market::Init(){
	if(!mInit){
		Load();
		mInit=true;
		mDialogName="Dialogs\\v\\Market.DialogsDesk.dialogs.xml";
		mDialog=new DialogsDesk;		
		if(!mDialog->SafeReadFromFile(mDialogName.pchar())){
			delete mDialog;
			mDialog=NULL;
		}
		return mDialog;
	}
	return NULL;
};

void cvi_Market::Load(){
	if(ResBuy==ResSell) ResBuy=-1;
}
bool NationEcoUnderstand(GFILE* f, NewMonster* NM) {
	int z;
	char cc[128];
	for (int i = 0; i < 6; i++) {
			z = Gscanf(f, "%s", cc);
			if (z != 1)EERR();
		float v1, v2;
		NM->EcResID[i] = GetResID(cc);
		NM->EcResIcn[i] = GetIconByName(cc);
		float x1, x2, x3, x4, x5, x6, x7, x8, x9, x10;
		float n1;
		z = Gscanf(f, "%s%g%g%g%s%g%g%s%g%s%g%g%g%s%g%g%s%g",
			cc, &x1, &x2, &x3,
			cc, &v1, &x4, cc, &x5,
			cc, &x6, &x7, &x8,
			cc, &v2, &x9, cc, &x10);
		if (z != 18) {
			sprintf(cc, "Not enough information: %s", cc);
			ErrM(cc);
		};
		NM->ResCostBuy0[i] = x1;
		NM->ResCostBuy1[i] = x2;
		NM->ResCostBuy2[i] = x3;
		NM->ExpBUY[i] = x4;
		NM->TimeBUY[i] = x5;
		NM->ResCostSell0[i] = x6;
		NM->ResCostSell1[i] = x7;
		NM->ResCostSell2[i] = x8;
		NM->ExpSELL[i] = x9;
		NM->TimeSELL[i] = x10;
		NM->ResCostBuy[i] = NM->ResCostBuy1[i];
		NM->ResCostSell[i] = NM->ResCostSell1[i];
		NM->ExpBUY[i] /= v1 * 100;
		NM->ExpSELL[i] /= v2 * 100;
		z = Gscanf(f, "%s%s%g",
			cc,cc,&n1);
		NM->NalogResID[i] = GetResID(cc);
		NM->Nalog[i] = n1;
	};
	return true; 
}
void cvi_Market::Process(){
	if(mDialog){ // &&NOPAUSE
		// limit sell amount
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		Nation* NT = &NATIONS[NI];
		vui_SelPoint* SP = OIS.GetLastSelPoint();
		if (SP) {
			if (SP->OB) {
				
				for (int i = 0; i < 6; i++) {
					EcResID[i] = SP->OB->newMons->EcResID[i];
					EcResIcn[i] = SP->OB->newMons->EcResIcn[i];
					ResCostBuy0[i] = SP->OB->newMons->ResCostBuy0[i];
					ResCostBuy1[i] = SP->OB->newMons->ResCostBuy1[i];
					ResCostBuy2[i] = SP->OB->newMons->ResCostBuy2[i];
					ExpBUY[i] = SP->OB->newMons->ExpBUY[i];
					TimeBUY[i] = SP->OB->newMons->TimeBUY[i];
					ResCostSell0[i] = SP->OB->newMons->ResCostSell0[i];
					ResCostSell1[i] = SP->OB->newMons->ResCostSell1[i];
					ResCostSell2[i] = SP->OB->newMons->ResCostSell2[i];
					ExpSELL[i] = SP->OB->newMons->ExpSELL[i];
					TimeSELL[i] = SP->OB->newMons->TimeSELL[i];
					NalogResID[i] = SP->OB->newMons->NalogResID[i];
					Nalog[i] = SP->OB->newMons->Nalog[i];
					//if ((ResCostSell[i] == ResCostSell1[i]) && (ResCostBuy[i] == ResCostBuy1[i])) {
					if(!(this->init[i])) {
					ResCostBuy[i] = SP->OB->newMons->ResCostBuy[i];
					ResCostSell[i] = SP->OB->newMons->ResCostSell[i];
					this->init[i] = true;
					}
				}
			}
		}
		if(ResSell!=-1){
			int SellID=EcResID[ResSell];
			if(mGet){
				if(NeedToSell>mGet(SellID)){
					NeedToSell=mGet(SellID);
				}        
			}else{
				if(NeedToSell>XRESRC(NI,SellID)){
					NeedToSell=XRESRC(NI,SellID);
				}        
			}
		}
		// balancing cost
		for(int i=0;i<6;i++){
			if(ResCostSell[i]>ResCostSell1[i])ResCostSell[i]-=TimeSELL[i];
			if(ResCostBuy[i]>ResCostBuy1[i])ResCostBuy[i]-=TimeBUY[i];
			if(ResCostSell[i]<ResCostSell1[i])ResCostSell[i]+=TimeSELL[i];
			if(ResCostBuy[i]<ResCostBuy1[i])ResCostBuy[i]+=TimeBUY[i];
		};
	}
};
void cvi_Market::SetResSell(byte ResID){
	for(int i=0;i<6;i++) 
		if(EcResID[i]==ResID){
			ResSell=i;
			if(ResBuy==i) ResBuy=-1;
			break;
		}
};
void cvi_Market::SetResBuy(byte ResID){
	for(int i=0;i<6;i++) 
		if(EcResID[i]==ResID){
			if(ResSell!=i) ResBuy=i;
			break;
		}
};

void cvi_Market::PerformOperation(byte Nation,byte SellRes,byte BuyRes,int SellAmount){
	int BuyID=EcResID[BuyRes];
	int SellID=EcResID[SellRes];
	//int* RES=&RESRC[Nation][0];
	int BuyAmount=0;
	addrand(Nation);
	addrand(SellRes);
	addrand(BuyRes);
	addrand(SellAmount);
	if(mGet&&mAdd){
		if(mGet(SellID)<SellAmount) SellAmount=mGet(SellID);
		addrand(ResCostSell[SellRes]*1000);
		addrand(ResCostBuy[BuyRes]*1000);
		BuyAmount=int(double(SellAmount)*ResCostSell[SellRes]/ResCostBuy[BuyRes]);
		addrand(BuyAmount);
		mAdd(SellID,-SellAmount);
		mAdd(BuyID,BuyAmount);
	}else{
		if(XRESRC(Nation,SellID)<SellAmount)SellAmount=XRESRC(Nation,SellID);
		addrand(ResCostSell[SellRes]*1000);
		addrand(ResCostBuy[BuyRes]*1000);
		BuyAmount=int(double(SellAmount)*ResCostSell[SellRes]/ResCostBuy[BuyRes]);
		addrand(BuyAmount);
		AddXRESRC(Nation,SellID,-SellAmount);
		AddXRESRC(Nation,BuyID,BuyAmount);
		NATIONS[Nation].ResBuy[BuyID]+=BuyAmount;
		NATIONS[Nation].ResSell[SellID]+=SellAmount;
		if (Nalog[SellRes] != 0) {
			int Nalogi = int(SellAmount*Nalog[SellRes]);
			AddXRESRC(Nation, NalogResID[SellRes], -Nalogi);
		}
	}
	double Ves=double(BuyAmount)*ExpBUY[BuyRes];
	ResCostBuy[BuyRes]=(ResCostBuy[BuyRes]+ResCostBuy2[BuyRes]*Ves)/(1+Ves);
	ResCostSell[BuyRes]=(ResCostSell[BuyRes]+ResCostSell2[BuyRes]*Ves)/(1+Ves);
	Ves=double(SellAmount)*ExpSELL[SellRes];
	ResCostSell[SellRes]=(ResCostSell[SellRes]+ResCostSell0[SellRes]*Ves)/(1+Ves);
	ResCostBuy[SellRes]=(ResCostBuy[SellRes]+ResCostBuy0[SellRes]*Ves)/(1+Ves);
};
//
void RestoreMarketPrice(){
	if(vMarket){
		vMarket->Load();
	}
}
bool ChangeMarketPrice(byte ResID, float Changer){
	if(vMarket){
		vMarket->ChangeResPrice(ResID,Changer);
	}
	//vMarketInGame.ChangeResPrice(ResID,Changer);
	return true;
}
/*
int GetEconomyData(byte** EC){
	*EC=(byte*)&ECO;
	return sizeof ECO;
}

*/
// Global function
void PerformTorg(byte Nation,byte SellRes,byte BuyRes,int SellAmount){
	if(vMarket->mDialog){
		vMarket->PerformOperation(Nation,SellRes,BuyRes,SellAmount);
	}
};


// vui_Actions
//extern ResDiscr RDS[8];
void cvi_MarketDesk::SetFrameState(SimpleDialog* SD){
	if(SD->AllocPtr){
		vMarket=(cvi_Market*) SD->AllocPtr;
		vMarket->Process();
	}else{			
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		SD->Visible=OIS.SelPoint.GetAmount()==1&&SP&&SP->OB->Ready&&SP->OB->newMons->Rinok&&SP->NI==GSets.CGame.cgi_NatRefTBL[MyNation];
		//
		vMarket->mDialog=(DialogsDesk*)SD;
		vMarket->Process();
		//
	}
	if(SD->Visible){
		TextButton* ST=SellTxt.Get();
		if(ST){
			static char* st=GetTextByID("#MarketSellText");
			_str txt;
			if(vMarket->ResSell==-1){
				ST->Visible=false;
			}else{
				ST->Visible=true;
				txt.print(st,RDS[vMarket->EcResID[vMarket->ResSell]].Name);
				ST->SetMessage(txt.pchar());
			}
		}
		TextButton* BT=BuyTxt.Get();
		if(BT){
			static char* st=GetTextByID("#MarketBuyText");
			_str txt;
			if(vMarket->ResBuy==-1){
				BT->Visible=false;
			}else{
				BT->Visible=true;
				txt.print(st,RDS[vMarket->EcResID[vMarket->ResBuy]].Name);
				BT->SetMessage(txt.pchar());
			}
		}
	}
}
//
void cvi_M_SellAmount::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(vMarket->ResSell!=-1){
		GP_TextButton* TB=dynamic_cast<GP_TextButton*>(SD);
		if(TB){
			SD->Visible=true;
			_str txt;
			static char* t=GetTextByID("#SellAmountText");
			txt.print("%s: %d",t,vMarket->NeedToSell);
			TB->SetMessage(txt.pchar());
		}
		TextButton* T=dynamic_cast<TextButton*>(SD);
		if(T){
			SD->Visible=true;
			_str txt;			
			if(resName){
				static char* t=GetTextByID("#SellAmountText");
				txt.print("%s: %d",t,vMarket->NeedToSell);
			}else{
				txt=vMarket->NeedToSell;
			}
			T->SetMessage(txt.pchar());
		}
	}
}
//
int cvi_Market::GetNeedToBuy(){
	NeedToBuy=int(double(NeedToSell)*ResCostSell[ResSell]/ResCostBuy[ResBuy]);
	return NeedToBuy;
}
//
void cvi_M_BuyAmount::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(vMarket->ResSell!=-1&&vMarket->ResBuy!=-1){
		GP_TextButton* TB=dynamic_cast<GP_TextButton*>(SD);
		if(TB){
			SD->Visible=true;
			_str txt;
			static char* t=GetTextByID("#BuyAmountText");
			if(resName){
				txt.print("%s: %d",t,vMarket->GetNeedToBuy());
			}else{
				txt=vMarket->GetNeedToBuy();
			}
			TB->SetMessage(txt.pchar());
		}
		TextButton* T=dynamic_cast<TextButton*>(SD);
		if(T){
			SD->Visible=true;
			_str txt;
			if(resName){
				static char* t=GetTextByID("#BuyAmountText");
				txt.print("%s: %d",t,vMarket->GetNeedToBuy());
			}else{
				txt=vMarket->GetNeedToBuy();
			}
			T->SetMessage(txt.pchar());
		}
	}
}
//
void cvi_M_SellSelect::SetFrameState(SimpleDialog* SD){
	//SD->Visible=false;
	if(vMarket->mDialog){
		int ResID=GetResID(SD->Name.pchar());
		if(ResID>=0){
			int mResID=vMarket->GetSellResID();
			VitButton* Pic=dynamic_cast<VitButton*>(SD->Find("pic"));
			GPPicture* Act=dynamic_cast<GPPicture*>(SD->Find("active"));
			if(Pic){
				if(ResID==mResID){
					Pic->State=1;
					Pic->Diffuse=0xFFFFFFFF;
				}else{
					Pic->State=0;
					Pic->Diffuse=0x88FFFFFF;
				}
			}
			if(Act){
				Act->Visible=ResID==mResID;
			}
		}
	}
}
bool cvi_M_SellSelect::LeftClick(SimpleDialog* SD){
	if(vMarket->mDialog){		
		vMarket->SetResSell(GetResID(SD->Name.pchar()));
	}
	return true;
}
//
void cvi_M_BuySelect::SetFrameState(SimpleDialog* SD){
	//SD->Visible=false;
	if(vMarket->mDialog){
		int ResID=GetResID(SD->Name.pchar());
		if(ResID>=0){
			int mResID=vMarket->GetBuyResID();
			VitButton* Pic=dynamic_cast<VitButton*>(SD->Find("pic"));
			GPPicture* Act=dynamic_cast<GPPicture*>(SD->Find("active"));
			if(Pic){
				if(ResID==mResID){
					Pic->State=1;
					Pic->Diffuse=0xFFFFFFFF;
				}else{
					Pic->State=0;
					Pic->Diffuse=0x88FFFFFF;
				}
				int sResID=vMarket->GetSellResID();
				if(ResID==sResID){					
					Pic->Diffuse=0x22FFFFFF;
				}
			}
			if(Act){
				Act->Visible=ResID==mResID;
			}
		}
	}
}
bool cvi_M_BuySelect::LeftClick(SimpleDialog* SD){
	if(vMarket->mDialog){
		vMarket->SetResBuy(GetResID(SD->Name.pchar()));
	}
	return true;
}
//
DIALOGS_API void PlaySound(char* Name);
//
bool cvi_M_Buy::LeftClick(SimpleDialog* SD){
	if(vMarket->mDialog){
		if(vMarket->ResSell!=-1&&vMarket->ResBuy!=-1&&vMarket->GetNeedToBuy()>0){
			PlaySound("INTERF_TRADE");
			if(vMarket->mGet&&vMarket->mAdd){
				vMarket->PerformOperation(0xFF,vMarket->ResSell,vMarket->ResBuy,vMarket->NeedToSell);
			}else{
				CmdTorg(GSets.CGame.cgi_NatRefTBL[MyNation],vMarket->ResSell,vMarket->ResBuy,vMarket->NeedToSell);
			}			
		};
	}
	return true;
}
//
bool cvi_M_Reset::LeftClick(SimpleDialog* SD){
	if(vMarket->mDialog){
		vMarket->NeedToBuy=0;
		vMarket->NeedToSell=0;
	}
	return true;
}
//
bool cvi_M_ChangeAmount::LeftClick(SimpleDialog* SD){
	if(vMarket->mDialog){
		if(GetKeyState(VK_SHIFT)&0x8000)vMarket->NeedToSell+=SD->ID*100;
			else vMarket->NeedToSell+=SD->ID;
		if(vMarket->NeedToSell<0) vMarket->NeedToSell=0;
		if(vMarket->NeedToSell>500000) vMarket->NeedToSell=500000;
	}
	return true;
}
//