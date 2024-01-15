#include "stdheader.h"
#include ".\cvi_Missions.h"
#include ".\cvi_singleplayerdata.h"
#include ".\cvi_MainMenu.h"
//
#include "Dialogs\MISSIONSCRIPT\MissionScript.h"	//Vitya //  [11/11/2003]
#include "Dialogs\MISSIONSCRIPT\EndMissionData.h"	//Vitya //  [11/12/2003]
#include "Dialogs\ClasDeclaration.h"				//Vitya.
#include "Dialogs\CPlayerSave.hpp"					//Grey.
//
//////////////////////////////////////////////////////////////////////////
ClonesArray<cvi_Missions> EW2_Missions;
const char* EW2_MissionsXML="missions\\ew2.xml";
//
void va_EW2_Mis_List::Init(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		LD->DSS.Clear();
		/*
		for(int i=0;i<EW2_Missions.GetAmount();i++){
			LD->AddElement(EW2_Missions[i]->Name);
		}
		*/
		LD->CurrentElement=0;
		vmCampID=0;
	}
}
//
extern bool vmVictory;
extern CPlayerSAVE* Profiles;
extern CPlayerSAVE_STRUCT* CurPlayer;
//
extern bool vls_Active;
//
int CurEW2miss=-1;
void va_EW2_Mis_List::SetFrameState(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&CurPlayer){
		if(CurPlayer->c_NDone<v_ISys.CampaignNDone){
			CurPlayer->c_NDone=v_ISys.CampaignNDone;
		}
		int NDone=CurPlayer->c_NDone+1;
		if(NDone>=LD->DSS.GetAmount()){
			int n=EW2_Missions.GetAmount();
#ifdef _DEMO_
			n=2;
#endif _DEMO_
			for(int i=LD->DSS.GetAmount();i<n&&i<NDone;i++){
				if(CurEW2miss==NDone){					
					LD->CurrentElement=CurPlayer->c_NDone;
					Profiles->SaveXML();
				}
				LD->AddElement(EW2_Missions[i]->Name);
			}
		}
		CurEW2miss=LD->CurrentElement;
		// preview
		if( CurEW2miss>=0 && CurEW2miss<EW2_Missions.GetAmount() ){			
			cvi_Missions* CM=EW2_Missions[CurEW2miss];		
			//void SetMapPreviewJpg(char* FileName, int CenterX, int CenterY);
			//SetMapPreviewJpg(CM->MapPreview.pchar(),CM->MapCenterX,CM->MapCenterY);
			void SetMapPreviewJpg(cvi_Missions* CM);
			SetMapPreviewJpg(CM);
		}
	}
	vls_Active=false;
}
// va_EW2_Mis_Tap
bool vTapMissClick=false;
void va_EW2_Mis_Tap::SetFrameState(SimpleDialog* SD){
	TabButton* TB=dynamic_cast<TabButton*>(SD);
	if(TB){
		if(vTapMissClick){
			vTapMissClick=false;
			TB->_Click();
		}
	}
}
void SetF1Text(char* Help);
extern _str vm_F1_VictoryCond;
void va_EW2_Mis_Text::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	int n=EW2_Missions.GetAmount();
	SD->Visible=false;
	if(TB&&n&&CurEW2miss>=0&&CurEW2miss<n){		
		SD->Visible=true;
		if(vls_Active){
			static char* t=GetTextByID("#EW2_CampLoad");
			TB->SetMessage(t);
			//SetF1Text(t);
			vm_F1_VictoryCond=t;
		}else{
			TB->SetMessage(EW2_Missions[CurEW2miss]->Description);			
			//SetF1Text(EW2_Missions[CurEW2miss]->Description);
			vm_F1_VictoryCond=EW2_Missions[CurEW2miss]->Description;
		}
	}
}
void va_EW2_Mis_Picture::SetFrameState(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	int n=EW2_Missions.GetAmount();
	if(GP&&n&&CurEW2miss>=0&&CurEW2miss<n){
		GP->SetFileID(&EW2_Missions[CurEW2miss]->DescrGPFile);
		GP->SetSpriteID(EW2_Missions[CurEW2miss]->DescrGPSprite);
	}
	SD->Visible=(!vls_Active);
}
void va_EW2_Mis_CheckTutorial::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	int n=EW2_Missions.GetAmount();
	if(n&&CurEW2miss>=0&&CurEW2miss<n){
		SD->Visible = Tutorial==EW2_Missions[CurEW2miss]->Tutorial;
	}
}
//
extern char PlName[64];
extern _str PlayerName;
extern byte vmDifficulty[8];
//
void va_EW2_Mis_Start::SetFrameState(SimpleDialog* SD){
	if(!vls_Active){
		SD->Enabled=true;
	}	
}
bool va_EW2_Mis_Start::LeftClick(SimpleDialog* SD){
	if(CurEW2miss<EW2_Missions.GetAmount()&&!vls_Active){
		CurrentGameInfo* g=&GSets.CGame;
		strcpy(g->cgi_CurrentMap,EW2_Missions[CurEW2miss]->FileM3D.pchar());	
		// name correct
		strcpy(g->PL_INFO[0].name,PlayerName.pchar());
		for(int i=1;i<8;i++){
			static char* cn=GetTextByID("#CampComputerName");
			strcpy(g->PL_INFO[i].name,cn);
		}		
		strcpy(PlName,PlayerName.pchar());		
		//		
		int diff=CurPlayer->c_Difficulty;
		if(diff<0) diff=0;
		if(diff>=GlobalAI.Diff.GetAmount()) diff=GlobalAI.Diff.GetAmount()-1;	
		for(int i=0;i<8;i++){
			vmDifficulty[i]=diff;		
		}	
		//
		extern int rusDiff;
		CurPlayer->c_Difficulty=rusDiff;	
		//	
		vmVictory=false;
		//
		//v_MainMenu.ShowLoadProgress();	
		void  UnloadBeforeMission();
		UnloadBeforeMission();
		//
		GSets.CGame.UnitTransform=false;
		//
		g->RunSingleplayerGame(0);
		//
		void vmCheckVictory();
		vmCheckVictory();		
	}
	return true;
}