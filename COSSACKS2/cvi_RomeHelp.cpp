#pragma once
#include "stdheader.h"
#include "VictoryConditions.h"
#include "CurrentMapOptions.h"
#include ".\cvi_RomeHelp.h"

//////////////////////////////////////////////////////////////////////////
void rhSetHelpDesk(ClassArray<cvi_MsgDialog>* HA, TabDesk* TD){
	if(TD&&TD->DSS.GetAmount()>0&&TD->DSS[0]->DSS.GetAmount()>0){
		int iy=TD->DSS[0]->Gety();
		int i;
		for(i=0;i<HA->GetAmount();i++){
			cvi_MsgDialog* MD=(*HA)[i];
			if(MD){
				DialogsDesk* ddD;
				if(i>=TD->DSS.GetAmount()){
					ddD=new DialogsDesk;
					if(TD->DSS[0]->Copy(ddD)){
						TD->DSS.Add(ddD);
					}else{
						delete ddD;							
						Log.Error("Can't copy Dialog: cva_RomeHelp::SetFrameState()");
						continue;
					}
				}else{
					ddD=(DialogsDesk*)TD->DSS[i];
				}
				ddD->Visible=true;
				ddD->Sety(iy);
				//
				DialogsDesk* dd=dynamic_cast<DialogsDesk*>(ddD->DSS[0]);
				int jj=0;
				if(!dd){
					dd=dynamic_cast<DialogsDesk*>(ddD->DSS[1]);
					jj=1;
				}
				if(!dd) continue;
				//
				TextButton* tbTit=(TextButton*)ddD->Find("Title");				
				if(tbTit){
					if(MD->Title){
						tbTit->SetMessage(MD->Title);
					}					
				}
				//				
				int jy=dd->Gety();
				//iy+=jy;
				bool Del=false;
				int j;
				for(j=0;j<MD->Phrases.GetAmount();j++){
					cvi_Phrase* P=MD->Phrases[j];
					if(P){
						int id=j;						
						if(tbTit){
							if(j==0) id=jj;
								else id=j+1;							
						}
						DialogsDesk* ddP;
						if(id>=ddD->DSS.GetAmount()){
							ddP=new DialogsDesk;
							if(dd->Copy(ddP)){
								ddD->DSS.Add(ddP);
							}else{
								delete ddP;							
								Log.Error("Can't copy Phrase: cva_RomeHelp::SetFrameState()");
								continue;
							}
						}else{
							ddP=(DialogsDesk*)ddD->DSS[id];
						}
						ddP->Visible=true;
						ddP->Sety(jy);
						TextButton* tbT=(TextButton*)ddP->Find("Objective Talker");
						TextButton* tbM=(TextButton*)ddP->Find("Objective Message");
						if(tbT&&tbM){							
							tbT->SetMessage(P->talkerName);
							tbM->SetMessage(P->Message);
							tbT->Enabled=tbM->Enabled=!P->Deleted;							
							int hT=tbT->GetHeight()+tbT->Gety();
							int hM=tbM->GetHeight()+tbM->Gety();
							int h=hT>hM?hT:hM;
							//h+=6;
							//iy+=h;
							jy+=h;
							int left,top,right,bottom;
							ddP->GetMargin(left,top,right,bottom);
							ddP->SetHeight(h+top+bottom);
							//iy+=top+bottom;
							//jy+=top+bottom;
						}
						Del=P->Deleted;
					}
				}
				if(tbTit) j++;
				for(;j<ddD->DSS.GetAmount();j++){
					ddD->DSS[j]->Visible=false;
				}
				jy+=10;
				GPPicture* gpD=(GPPicture*)ddD->Find("Delimiter");
				if(gpD){
					gpD->Visible=true;
					gpD->Sety(jy);
					jy+=gpD->GetHeight(); //+10
				}
				//
				int left,top,right,bottom;
				ddD->GetMargin(left,top,right,bottom);				
				ddD->SetHeight(jy+bottom+top);
				//
				GPPicture* gpBx=(GPPicture*)ddD->Find("Box");
				GPPicture* gpBd=(GPPicture*)ddD->Find("Bird");
				if(gpBx&&gpBd){
					gpBx->Visible=i>0;
					gpBd->Visible=gpBx->Visible&&Del;
				}
				//
				iy+=jy+10;
			}
		}
		// hide block
		for(;i<TD->DSS.GetAmount();i++){
			TD->DSS[i]->Visible=false;
		}
	}	
}

void cva_RomeHelp::SetFrameState(SimpleDialog* SD){
	static int time=0;
	if(GetTickCount()>time){		
		//
		static cvi_RomeHelp* RH=new cvi_RomeHelp;
		RH->reset_class(RH);		
		VictoryConditionRoot* VC=MOptions.VictoryRule.Get();
		if(VC){
			//
			cvi_Phrase* P=new cvi_Phrase;
			P->talkerName=GetTextByID("RM_VictRule");
			P->Message=GetTextByID(VC->GameF1Text.pchar());			
			//
			cvi_MsgDialog* MD=new cvi_MsgDialog;
			MD->Phrases.Add(P);
			RH->MsgObjectives.Add(MD);			
		}else{
			GetRomeHelp(RH);
		}
		//
		rhSetHelpDesk(&RH->MsgObjectives,tdObjective.Get());
		//
		//TabDesk* tdObj=tdObjective.Get();
		/*
		if(tdObj&&tdObj->DSS.GetAmount()>0&&tdObj->DSS[0]->DSS.GetAmount()>0){
			int iy=tdObj->DSS[0]->Gety();
			for(int i=0;i<RH->MsgDialogs.GetAmount();i++){
				cvi_MsgDialog* MD=RH->MsgDialogs[i];
				if(MD){
					DialogsDesk* ddD;
					if(i>=tdObj->DSS.GetAmount()){
						ddD=new DialogsDesk;
						if(tdObj->DSS[0]->Copy(ddD)){
							tdObj->DSS.Add(ddD);
						}else{
							delete ddD;							
							Log.Error("Can't copy Dialog: cva_RomeHelp::SetFrameState()");
							continue;
						}
					}else{
						ddD=(DialogsDesk*)tdObj->DSS[i];
					}
					ddD->Sety(iy);
					int left,top,right,bottom;					
					int jy=ddD->DSS[0]->Gety();
					for(int j=0;j<MD->Phrases.GetAmount();j++){
						cvi_Phrase* P=MD->Phrases[j];
						if(P){
							DialogsDesk* ddP;
							if(j>=ddD->DSS.GetAmount()){
								ddP=new DialogsDesk;
								if(ddD->DSS[0]->Copy(ddP)){
									ddD->DSS.Add(ddP);
								}else{
									delete ddP;							
									Log.Error("Can't copy Phrase: cva_RomeHelp::SetFrameState()");
									continue;
								}
							}else{
								ddP=(DialogsDesk*)ddD->DSS[j];
							}
							ddP->Sety(jy);
							TextButton* tbT=(TextButton*)ddP->Find("Objective Talker");
							TextButton* tbM=(TextButton*)ddP->Find("Objective Message");
							if(tbT&&tbM){
								tbT->SetMessage(P->talkerName);
								tbM->SetMessage(P->Message);
								int hT=tbT->GetHeight();
								int hM=tbM->GetHeight();
								int h=hT>hM?hT:hM;
								iy+=h;
								jy+=h;
							}							
						}
					}
					ddD->GetMargin(left,top,right,bottom);
					iy+=bottom+5;
					ddD->SetHeight(jy);
					iy+=top+15;
				}
			}
		}
		*/
		//
		//
		TabDesk* tdHnt=tdHint.Get();
		bool hint=RH->MsgHints.GetAmount();
		rhSetHelpDesk(&RH->MsgHints,tdHnt);
		if(tbHint.Get()) tbHint.Get()->Visible=hint||VC==NULL;
		//
		TabDesk* tdDlg=tdDialog.Get();
		bool dialog=RH->MsgDialogs.GetAmount();
		rhSetHelpDesk(&RH->MsgDialogs,tdDlg);
		if(tbDialog.Get()) tbDialog.Get()->Visible=dialog||VC==NULL;
		/*
		if(tdDlg&&tdDlg->DSS.GetAmount()>0&&tdDlg->DSS[0]->DSS.GetAmount()>0){
			int iy=tdDlg->DSS[0]->Gety();
			for(int i=0;i<RH->MsgDialogs.GetAmount();i++){
				cvi_MsgDialog* MD=RH->MsgDialogs[i];
				if(MD){
					DialogsDesk* ddD;
					if(i>=tdDlg->DSS.GetAmount()){
						ddD=new DialogsDesk;
						if(tdDlg->DSS[0]->Copy(ddD)){
							tdDlg->DSS.Add(ddD);
						}else{
							delete ddD;							
							Log.Error("Can't copy Dialog: cva_RomeHelp::SetFrameState()");
							continue;
						}
					}else{
						ddD=(DialogsDesk*)tdDlg->DSS[i];
					}
					ddD->Sety(iy);
					int left,top,right,bottom;					
					int jy=ddD->DSS[0]->Gety();
					for(int j=0;j<MD->Phrases.GetAmount();j++){
						cvi_Phrase* P=MD->Phrases[j];
						if(P){
							DialogsDesk* ddP;
							if(j>=ddD->DSS.GetAmount()){
								ddP=new DialogsDesk;
								if(ddD->DSS[0]->Copy(ddP)){
									ddD->DSS.Add(ddP);
								}else{
									delete ddP;							
									Log.Error("Can't copy Phrase: cva_RomeHelp::SetFrameState()");
									continue;
								}
							}else{
								ddP=(DialogsDesk*)ddD->DSS[j];
							}
							ddP->Sety(jy);
							TextButton* tbT=(TextButton*)ddP->Find("Objective Talker");
							TextButton* tbM=(TextButton*)ddP->Find("Objective Message");
							if(tbT&&tbM){
								tbT->SetMessage(P->talkerName);
								tbM->SetMessage(P->Message);
								int hT=tbT->GetHeight();
								int hM=tbM->GetHeight();
								int h=hT>hM?hT:hM;
								iy+=h;
								jy+=h;
							}							
						}
					}
					ddD->GetMargin(left,top,right,bottom);
					iy+=bottom+5;
					ddD->SetHeight(jy);
					iy+=top+15;
				}
			}
		}	
		*/
	}
	time=GetTickCount()+2000;
}
