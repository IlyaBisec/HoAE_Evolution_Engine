#include "stdheader.h"
#include ".\cvi_brigcreate.h"
//////////////////////////////////////////////////////////////////////////
cvi_BrigCreate vBrigCreate;
//////////////////////////////////////////////////////////////////////////
cvi_BrigCreate::cvi_BrigCreate(void)
{
	mInit=false;
	mDialog=NULL;
	//
	mActive=false;
	mRadius=800;	// < 1500
}
cvi_BrigCreate::~cvi_BrigCreate(void)
{
}
SimpleDialog* cvi_BrigCreate::Init(){
	if(!mInit){
		mInit=true;
		mDialogName="Dialogs\\v\\BrigCreate.DialogsDesk.dialogs.xml";
		mDialog=new DialogsDesk;		
		if(!mDialog->SafeReadFromFile(mDialogName.pchar())){
			delete mDialog;
			mDialog=NULL;
		}
		return mDialog;
	}
	return NULL;
	//vsdActAbilityOne=(SimpleDialog*)vddActAbility->DSS[0]->new_element();
	//vddActAbility->DSS[0]->Copy(vsdActAbilityOne);
	//vddActAbility->DSS.Clear();
}
void cvi_BrigCreate::Process(OneObject* OB, bool Visible){
	if(mDialog){
		mOB=NULL;
		if(OB->BrigadeID==0xFFFF && Visible){
			mOR=GetOfficerRecord(OB->NNUM,OB->NIndex);
			if(mOR&&mOR->NStroi&&mOR->SDES->NAmount){
				mOB=OB;
				//if(GetRND(100)<)
				mParam.NI=OB->NNUM;
				mParam.NIndex=OB->NIndex;
				mParam.ReqAmount=0;
				for(int i=0;i<mOR->SDES->NAmount;i++){
					if(mOR->SDES->Amount[i]>mParam.ReqAmount) mParam.ReqAmount=mOR->SDES->Amount[i];
				}
				mParam.UG.N=0;
				PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,mRadius,mParam.AddUnitCallBack,&mParam);
			}
			//if(mOB!=OB){
			//	mActive=false;
			//}
		}
		mDialog->Visible=mOB!=NULL;
	}
}
// vui_Actions
void cvi_A_BC_Active::SetFrameState(SimpleDialog* SD){
	/*
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP){
		static int spr=GP->SpriteID;
		GP->SpriteID=vBrigCreate.mActive?spr+1:spr;
	}
	*/
}
bool cvi_A_BC_Active::LeftClick(SimpleDialog* SD){
	if(vBrigCreate.mInit){
		vBrigCreate.mActive=!vBrigCreate.mActive;
	}
	return true;
}
//
void cvi_A_BC_Crew::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(vBrigCreate.mActive && vBrigCreate.mOB && SD->ID<vBrigCreate.mOR->SDES->NAmount){
		SD->Visible=true;
		if(vBrigCreate.mOR->SDES->Amount[SD->ID]<=vBrigCreate.mParam.UG.N){
			SD->Diffuse=0xFFFFFFFF;
		}else{
			SD->Diffuse=0xC0FFFFFF;
		}
	}
}
bool cvi_A_BC_Crew::LeftClick(SimpleDialog* SD){
	if(vBrigCreate.mInit&&vBrigCreate.mOB&&SD->Diffuse==0xFFFFFFFF){
		cvi_IS_BC_Create com;
		com.Data.ObjID=vBrigCreate.mOB->Index;
		com.Data.ObjSN=vBrigCreate.mOB->Serial;
		//com.Data.ODIndex=vBrigCreate.mOR->SDES->LocalID[SD->ID];
		com.Data.ODIndex=SD->ID;
		com.InterfAction();
	}
	return true;
}
//
void cvi_A_BC_CrewAmount::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB && vBrigCreate.mActive && vBrigCreate.mOB && SD->ID<vBrigCreate.mOR->SDES->NAmount){
		SD->Visible=true;
		_str txt;
		txt=vBrigCreate.mOR->SDES->Amount[SD->ID];
		TB->SetMessage(txt.pchar());
	}
}
// vui_InterfSynchro
void cvi_IS_BC_Create::SynchroAction(){
	OneObject* OB=Group[Data.ObjID];
	if(OB&&OB->Serial==Data.ObjSN&&!OB->Sdoxlo){
		byte NI=OB->NNUM;
		int bid=CreateBrigInZone(NI, OB->RealX>>4, OB->RealY>>4, 0xFFFF, OB->NIndex, Data.ODIndex);		
		if(bid>=0){
			// select brigade only
			word* id=ImSelm[NI];
			word* sn=ImSerN[NI];
			for(int i=0;i<ImNSL[NI];i++){
				if(id[i]!=0xFFFF){
					OneObject* ob=Group[id[i]];
					if(ob){
						ob->ImSelected&=~GM(NI);
					}
				}
			}
			if(!ImNSL[NI]){
				ImSelm[NI]=(word*)realloc(ImSelm[NI],2);
				ImSerN[NI]=(word*)realloc(ImSerN[NI],2);
			}else{
				ImNSL[NI]=0;
			}
			id=Selm[NI];
			sn=SerN[NI];
			for(int i=0;i<NSL[NI];i++){
				if(id[i]!=0xFFFF){
					OneObject* ob=Group[id[i]];
					if(ob){
						ob->Selected&=~GM(NI);
					}
				}
			}
			if(!NSL[NI]){
				Selm[NI]=(word*)realloc(Selm[NI],2);
				SerN[NI]=(word*)realloc(SerN[NI],2);
			}else{
				NSL[NI]=0;
			}
			Brigade* BR=CITY[NI].Brigs+bid;
			for(int i=0;i<BR->NMemb;i++){
				if(BR->Memb[i]!=0xFFFF){
					OneObject* ob=Group[BR->Memb[i]];
					if(ob){
						ImSelm[NI][0]=BR->Memb[i];
						ImSerN[NI][0]=BR->MembSN[i];
						ob->ImSelected|=GM(NI);
						ImNSL[NI]=1;
						Selm[NI][0]=BR->Memb[i];
						SerN[NI][0]=BR->MembSN[i];
						ob->Selected|=GM(NI);
						NSL[NI]=1;
						break;
					}
				}
			}
			//SelBrigade(NI,0,NI,bid);
		}
		void CorrectBrigadesSelection(byte NT);
		void ImCorrectBrigadesSelection(byte NT);
		CorrectBrigadesSelection(NI);
		ImCorrectBrigadesSelection(NI);
		//
		/*
		byte NI=OB->NNUM;
		word UID=Data.ObjID;
		word ort=Data.ODIndex;
		word Type=OB->NIndex;
		if(NSL[NI]){
			int N=NSL[NI];
			for(int j=1;j<N;j++){
				word MID=Selm[NI][j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB)OB->Selected&=~GM(NI);
				};
			};
			NSL[NI]=1;
			addrand(NSL[NI]+1);
		};
		if(OB&&(!OB->Sdoxlo)&&OB->BrigadeID==0xFFFF){
			int bind=CITY[GSets.CGame.cgi_NatRefTBL[NI]].GetFreeBrigade();
			Brigade* BR=CITY[GSets.CGame.cgi_NatRefTBL[NI]].Brigs+bind;
			BR->Enabled=true;
			if(!BR->CreateNearOfficer(Group[UID],Type,ort)){
				BR->Enabled=false;
			}else{
				CorrectBrigadesSelection(NI);
				ImCorrectBrigadesSelection(NI);
			};
		};
		*/
	}
};
// sv_CreateBrig
bool sv_Param_CreateBrig::AddUnitCallBack(OneObject* OB,void* Param){
	sv_Param_CreateBrig* Par=(sv_Param_CreateBrig*)Param;
	if(Par->UG.N<Par->ReqAmount && OB->NNUM==Par->NI && OB->NIndex==Par->NIndex &&
		!(OB->Sdoxlo||OB->InArmy||OB->UnlimitedMotion)){
		Par->UG.AddNewUnit(OB);
		return true;
	}
	return false;
}