#include "stdheader.h"
#include "UnitsInterface.h"
#include ".\cvi_academy.h"

//////////////////////////////////////////////////////////////////////////
cvi_Academy vAcademy;
//////////////////////////////////////////////////////////////////////////

cvi_Academy::cvi_Academy(void)
{
	mInit=false;
	mDialog=NULL;
}

cvi_Academy::~cvi_Academy(void)
{
}

SimpleDialog* cvi_Academy::Init(char* FileXML){
	if(!mInit){		
		mInit=true;
		mDialogName=FileXML;
		mDialog=new DialogsDesk;		
		if(!mDialog->SafeReadFromFile(mDialogName.pchar())){
			delete mDialog;
			mDialog=NULL;
		}
		return mDialog;
	}
	return NULL;
};

void cvi_Academy::Process(DialogsSystem* Parent){
	if(Parent){
		SimpleDialog* SD=Init("Dialogs\\v\\Academy.DialogsDesk.dialogs.xml");
		if(SD) Parent->DSS.Add(SD);
		if(mDialog){
		}
	}	
};

// vui_Actions
void cvi_AcademyDesk::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	SD->Visible=OIS.SelPoint.GetAmount()==1&&SP&&SP->OB->Ready&&SP->OB->Usage==DiplomatID;
}
//
void cvi_Acd_Inf::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		Nation* NT=NATIONS+NI;
		_str a;
		if(TB->Name=="Act"){
			a=NT->NBrInfantry.Active;
		}else
		if(TB->Name=="Kil"){
			a=NT->NBrInfantry.Killed;
		}else
		if(TB->Name=="Los"){
			a=NT->NBrInfantry.Lost;
		}
		TB->SetMessage(a.pchar());
	}	
}
//
void cvi_Acd_Cav::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		Nation* NT=NATIONS+NI;
		_str a;
		if(TB->Name=="Act"){
			a=NT->NBrCavalery.Active;
		}else
		if(TB->Name=="Kil"){
			a=NT->NBrCavalery.Killed;
		}else
		if(TB->Name=="Los"){
			a=NT->NBrCavalery.Lost;
		}
		TB->SetMessage(a.pchar());
	}	
}
//
void cvi_Acd_Can::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		int N=NATIONS->NMon;
		word* NIndex=NULL;
		if(TB->Name=="Act"){
			NIndex=CITY[NI].UnitAmount;
		}else
		if(TB->Name=="Kil"){
			NIndex=NATIONS[NI].NErased;
		}else
		if(TB->Name=="Los"){
			NIndex=NATIONS[NI].NKilled;
		}
		int a=0;
		for(int i=0;i<N;i++){
			NewMonster* NM=NATIONS->Mon[i]->newMons;
			if(NM&&NM->PortBranch==2){
				a+=NIndex[i];
			}
		}
		_str txt;
		txt=a;
		TB->SetMessage(txt.pchar());
	}	
}
//
void cvi_Acd_Peas::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		int ResID=GetResID(SD->Name.pchar());
		if(ResID>=0){
			byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
			_str txt;
			txt=CITY[NI].ResourcePeasants[ResID];
			TB->SetMessage(txt.pchar());
			SD->Visible=true;
		}
	}
}
//
void cvi_Acd_Spend::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		int ResID=GetResID(SD->Name.pchar());
		if(ResID>=0){
			byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
			Nation* NT=NATIONS+NI;			
			_str txt;
			txt=NT->ResOnUpgrade[ResID]+NT->ResOnMines[ResID]+NT->ResOnUnits[ResID]+
				NT->ResOnBuildings[ResID]+NT->ResOnLife[ResID];
			TB->SetMessage(txt.pchar());
			SD->Visible=true;
		}
	}
}
