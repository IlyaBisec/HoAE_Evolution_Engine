#include "stdheader.h"
#include "mp3\oggvor.h"
#include ".\cvi_campaign.h"
#include ".\cvi_singleplayerdata.h"
#include ".\cvi_MainMenu.h"
#include ".\cvi_HeroButtons.h"
#include "UnitAbilityIcon.h"
#include "BE_BaseClasses.h"
#include ".\cvi_InGamePanel.h"
//
#include "Dialogs\MISSIONSCRIPT\MissionScript.h"	//Vitya //  [11/11/2003]
#include "Dialogs\MISSIONSCRIPT\EndMissionData.h"	//Vitya //  [11/12/2003]
#include "Dialogs\ClasDeclaration.h"				//Vitya.
#include "Dialogs\CPlayerSave.hpp"					//Grey.
//////////////////////////////////////////////////////////////////////////
cvi_Campaign* vmCamp=NULL;
int vmCampID=-1;
ClonesArray<cvi_Campaign> vmCampaigns;
const char* vmCampaignXML="dialogs\\campaign.xml";
//////////////////////////////////////////////////////////////////////////
cvi_Campaign::cvi_Campaign(void)
{
}

cvi_Campaign::~cvi_Campaign(void)
{
}
bool cvi_Campaign::Process(){
	Missions[curMission]->Process();
	return false;
}
//////////////////////////////////////////////////////////////////////////
extern bool vmVictory;
extern bool vmEndCampaign;
extern long StartAboutTime;
extern long FinishAboutTime;
//
bool cvi_Mission::Process(){
	int n=Scene.GetAmount();
	if(curFrame>=0&&curFrame<n){
		if(!Scene[curFrame]->Process()){
			curFrame++;
			if(curFrame<n){
				Scene[curFrame]->Play();
			}else{
				/*
				if(vmEndCampaign){	//&&v_MainMenu.ModalDesk!="CampSelect"
					v_MainMenu.ModalDesk="CampSelect";
				}else{
					v_MainMenu.ModalDesk="Campaign";
					if(vmVictory&&vmCamp){
						vmCamp->curMission++;
						cva_Camp_StartMission::MissionID=vmCamp->curMission;
					}
				}
				*/
			}
		}
	}else{
		if(curFrame>=n){
			curFrame=-1;
			if(vmEndCampaign){	//&&v_MainMenu.ModalDesk!="CampSelect"
				//v_MainMenu.ModalDesk="CampSelect";
				StartAboutTime=GetTickCount();
				FinishAboutTime=0xFFFFFFF;
				LastKey=0;
				v_MainMenu.ModalDesk="Credits";
			}else{
				v_MainMenu.ModalDesk="Campaign";
#ifndef _DEMO_
				if(vmVictory&&vmCamp){
					vmCamp->curMission++;
					cva_Camp_StartMission::MissionID=vmCamp->curMission;
					cvi_Mission* cM=vmCamp->Missions[vmCamp->curMission];
					if(cM){
						cM->curFrame=-1;
					}
				}
#endif _DEMO_
			}
		}
	}
	return false;
}
void cvi_Mission::StartScene(){
	int n=Scene.GetAmount();
	for(int i=0;i<n;i++){
		Scene[i]->Play();
	}
};
//////////////////////////////////////////////////////////////////////////
extern char PlName[64];
extern _str PlayerName;
extern CPlayerSAVE_STRUCT vNewProf;
extern byte vmDifficulty[8];
extern bool MissListScrollPos;
//
bool vmVictory=false;
bool vmEndCampaign=false;
void EnableNextMission(){
#ifdef _DEMO_
	//vmCamp->curMission=8;
	vmSinglePlayerData.NDoneMission[vmCampID]=100;
#else
	int* NDM=vmSinglePlayerData.NDoneMission+vmCampID;
	if(vmCamp->curMission==*NDM){
		(*NDM)++;
		//vmCamp->curMission=(*NDM);
		cva_Camp_StartMission::MissionID=(*NDM);
		MissListScrollPos=true;
	}else{
		if(cva_Camp_StartMission::MissionID<*NDM)
			cva_Camp_StartMission::MissionID++;
	}
	if(vmCamp->curMission+1>=vmCamp->Missions.GetAmount()){
		vmEndCampaign=true;
		StartAboutTime=GetTickCount();
		FinishAboutTime=0xFFFFFFF;
		LastKey=0;
		v_MainMenu.ModalDesk="Credits";
	}else{
		vmEndCampaign=false;
	}
	vmPlayerData.Write();
#endif _DEMO_
}

void vmCheckVictory(){
	if(vmCamp && vmCamp->curMission>=0){
		if(vmVictory){
			EnableNextMission();
//#ifdef _DEMO_
//			//vmCamp->curMission=8;
//			vmSinglePlayerData.NDoneMission[vmCampID]=100;
//#else
//			int* NDM=vmSinglePlayerData.NDoneMission+vmCampID;
//			if(vmCamp->curMission==*NDM){
//				(*NDM)++;
//			}
//
//			if(vmCamp->curMission+1>=vmCamp->Missions.GetAmount()){
//				vmEndCampaign=true;
//			}else{
//				vmEndCampaign=false;
//			}
//
//			vmPlayerData.Write();
//#endif _DEMO_
		}else{
			if(vmCamp->curMission<vmCamp->Missions.GetAmount()){
				cvi_Mission* cM=vmCamp->Missions[vmCamp->curMission];
				cM->curFrame=cM->Scene.GetAmount(); //cM->Scene.GetAmount()-1;
			}
		}
	}
	extern int CurEW2miss;
	extern CPlayerSAVE_STRUCT* CurPlayer;
	if(vmVictory){
		if( CurPlayer && CurEW2miss>=CurPlayer->c_NDone ){
			CurPlayer->c_NDone=CurEW2miss+1;
			CurEW2miss=CurPlayer->c_NDone+1;
		}
		extern bool vTapMissClick;
		vTapMissClick=true;
	}
}
void cvi_mfGame::Play(){
	//vmPlayerData.Write();
	
	CurrentGameInfo* g=&GSets.CGame;
	strcpy(g->cgi_CurrentMap,MissFileName.pchar());	
	strcpy(PlName,PlayerName.pchar());
	//
	int diff=vmSinglePlayerData.Difficulty;
	if(diff<0) diff=0;
	if(diff>=GlobalAI.Diff.GetAmount()) diff=GlobalAI.Diff.GetAmount()-1;
	for(int i=0;i<8;i++){
		vmDifficulty[i]=diff;		
	}	
	//
	extern int rusDiff;
	vmSinglePlayerData.Difficulty=rusDiff;	
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
	if(vGameMode==gmSin){
		v_MainMenu.ModalDesk="Singles";
		//cvi_Mission* cM=vmCamp->Missions[vmCamp->curMission];
		//cM->curFrame=cM->Scene.GetAmount()-1;
		return;
	}
	//
	v_MainMenu.Cross=1;
	v_MainMenu.CrossTime=0;
	v_MainMenu.CrossSD=NULL;
	//
	if(PostStatistic){
		// show statistic
	}	
	vmCheckVictory();
}
//
void cvi_ChangeDifficulty::Play(){
	vmSinglePlayerData.Difficulty+=Value;
};
//
cvi_mfFrescos* cva_Fresco_Next::FR;
bool cva_Fresco_Next::Clicked;
//int cva_Fresco_Next::CUR;
//
int GetBitPicWidth(char* FileName){
	if(FileName){
		int id=IPM->GetImageID(FileName);
		return IPM->GetWidth(id);
	}
	return 0;
}
//
bool fCycle=true;
//bool fCycle=false;
float fCycleSpeed=25.0f/1000.0f;
//
DynArray<int> mv_FrescoWidth;
//DynArray<SQPicture*> mv_Fresco;
/*
void mv_FrescoClear(){
	int n=mv_Fresco.GetAmount();
	for(int i=0;i<n;i++) delete mv_Fresco[i];
	mv_Fresco.Clear();
}
*/
int vov_Time=0;
int ScreenLx;
void cvi_Fresco::Play(raBitPicture& aBP, TextButton* TB){
	int aN=aBP.GetAmount();
	if(aN==0) return;	
	BitPicture* BP=aBP[0]->Get();
	if(!(BP&&TB)) return;
	TB->SetMessage(Text);
	if(Scroll){
		ScrollLx=0;
		ScrollSpeed=0;
		//
		mv_FrescoWidth.Clear();
		//mv_FrescoClear();
		int x=0;
		int n=Pics.GetAmount();
		for(int i=0;i<aN;i++){			
			//SQPicture* SQP=new SQPicture;
			//SQP->LoadPicture(Pics[i]->pchar());
			//mv_Fresco.Add(SQP);
			BP=aBP[i]->Get();
			if(BP){
				bool cyc=fCycle&&n==2&&(i==2||i==3);
				if(i<n||cyc){
					if(cyc){
						BP->SetFileName(Pics[i-2]->pchar());
					}else{
						BP->SetFileName(Pics[i]->pchar());
					}					
					BP->Setx(x);
					int bLx=BP->Pic.GetLx();
					BP->SetWidth(bLx);
					mv_FrescoWidth.Add(bLx);
					x+=bLx;
                    BP->Visible=true;
				}else{
					BP->Visible=false;
				}
			}
		}
		//
		SimpleDialog* SD=(SimpleDialog*)BP->ParentDS;
		if(SD){
			ScrollLx=SD->GetWidth();
			ScreenLx=SD->GetWidth();
		}		
	}else{
		BP->FileName.Assign(Picture.pchar());
	}	
	ov_Play(Sound.pchar(),0);
	ov_SetVolume(EngSettings.vInterf.svFrescoVoice);
	vov_Time=GetTickCount()+2000;
};
int ScrolID;  // very important
void cvi_Fresco::Process(raBitPicture& aBP, TextButton* TB){
	if(Scroll&&ScrollLx&&GetTickCount()>vov_Time){
		if(ScrollSpeed==0){
			int t=ov_GetStreamLength(0)*1000-6000;
			//int t=5*60*1000-6000;
			//
			if(t<=0) t=1;
			//
			if(t>0){
				int Lx=-ScrollLx;
				int n=Pics.GetAmount();
				if(n){
					for(int i=0;i<n;i++){
						Lx+=mv_FrescoWidth[i];//GetBitPicWidth(Pics[i]->pchar());
					}	
					ScrollLx=-Lx;
					ScrollSpeed=float(Lx)/float(t);
					if(fCycle){
						ScrollSpeed=fCycleSpeed;
					}
					ScrollTime=GetTickCount();
					//Lx=GetBitPicWidth(Pics[0]->pchar());
					//ScrollTime=Lx/ScrollSpeed;
					ScrolID=0;
				}
			}			
		}
		if(ScrollSpeed>0 && mv_FrescoWidth[0]>0){
			double t=ScrollTime-double(GetTickCount());
			double x=ScrollSpeed*t;
			if(!fCycle&&x<=ScrollLx){
				x=ScrollLx;
			}			
			int np=Pics.GetAmount();
			int fCx[10];
			if(fCycle){
				if(np==2)np+=2;
				bool Correct=false;
				while(x+mv_FrescoWidth[ScrolID]<=0){
					ScrolID=(ScrolID+1)%np;
					x+=mv_FrescoWidth[ScrolID];
					//x=0;
					//ScrollTime-=t+x/ScrollSpeed;
					Correct=true;
				}
				if(Correct){
					ScrollTime-=t+x/ScrollSpeed;
				}				
				int i=ScrolID;
				int xx=x;
				do{
					fCx[i]=xx;
					xx+=mv_FrescoWidth[i];
					i=(i+1)%np;
				}while(i!=ScrolID);
			}
			//Log.Info("Frame time: %d",int(t));
			if(fCycle){
				//Log.Info("Scroll ID: %d",ScrolID);
			}
			int n=aBP.GetAmount();
			for(int i=0;i<n;i++){
				BitPicture* BP=aBP[i]->Get();
				if(BP){
					if(i<np){
						int Lx=mv_FrescoWidth[i];
						if(fCycle){
							x=fCx[i];
						}						
						if(x<RealLx&&x+Lx>0){
							BP->Visible=true;
							BP->Setx(x);
							BP->SetWidth(Lx);
							//Log.Warning("   frame: %d, x: %d, Lx: %d",i,int(x),Lx);
						}else{
							BP->Visible=false;
						}
						x+=Lx;
					}else{
						BP->Visible=false;
					}
				}				
			}			
		}
	}
};
bool cvi_Fresco::isFinished(){
	if(GetTickCount()>vov_Time){
		bool fin=ov_StreamFinished(0);
		if(fin){
			//mv_FrescoClear();
			return true;
		}else{
			return false;
		}		
	}else{
		return false;
	}
	
};
//
void SetCDVolumeEx(int Vol);
CEXPORT int GetCDVolume();
//
int vfVol;
//
void cvi_mfFrescos::Play(){
	if(Frescos.GetAmount()>0){
		v_MainMenu.ModalDesk="Fresco";
		curFresco=-1;
		cva_Fresco_Next::FR=this;
		cva_Fresco_Next::Clicked=false;
		/*
		int n=Frescos.GetAmount();
		for(int i=0;)
		v_MainMenu.ModalDesk="Campaign";
		*/
		vfVol=GetCDVolume();
		SetCDVolumeEx(EngSettings.vInterf.svFrescoMusic);
	}
}
bool cvi_mfFrescos::Process(){	
	if(curFresco>=Frescos.GetAmount()){
		//
		ov_Stop(0);
		SetCDVolumeEx(vfVol);
		//
		cva_Fresco_Next::FR=NULL;
		//v_MainMenu.ModalDesk="Campaign";
		//
		return false;
	}else{
		cva_Fresco_Next::FR=this;
		return true;
	}
}
//
CEXPORT int GetCDVolume();
void SetCDVolumeEx(int Vol);
//
void cvi_mfMovie::Play(){
	//bool find=false; // история роликов
	//for(int i=0;i<vmSinglePlayerData.Movies.GetAmount();i++){
	//	if(vmSinglePlayerData.Movies[i]->Get()==this){
	//		find=true;
	//		break;
	//	}
	//}
	//if(!find){
	//	ClassRef<cvi_mfMovie>* crM=new ClassRef<cvi_mfMovie>;
	//	crM->Set(this);
	//	vmSinglePlayerData.Movies.Add(crM);
	//	vmPlayerData.Write();
	//}
	
	int vol=GetCDVolume();
	SetCDVolumeEx(0);
	ov_Stop(1); // закрыть звуковой буфер, облачко
	void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
	PlayFullscreenVideo( Movie.pchar() , px , py ); //0.17
	SetCDVolumeEx(vol);
	if(PassMission){
		EnableNextMission();
	}
}
//////////////////////////////////////////////////////////////////////////
void cva_Campaign::SetFrameState(SimpleDialog* SD){
	vmCamp->Process();
}
//
//DWORD cva_Mission::Over=0xFFFFFFFF;
//DWORD cva_Mission::Finished=0xFFFFFFFF;
//DWORD cva_Mission::Next=0xFFFFFFFF;
//DWORD cva_Mission::Selected=0xFFFFFFFF;
word GetDir(int dx,int dy);
//
int m_CurMiss=-1;
bool MissListScrollPos=false;
void cva_MissList::SetFrameState(SimpleDialog* SD){
	if(vmCamp){
		m_CurMiss=0;
		ListDesk* LD=dynamic_cast<ListDesk*>(SD);
		if(LD){
			int nl=LD->DSS.GetAmount();
			int nm=vmCamp->Missions.GetAmount();
			for(int i=nl;i<nm;i++){
				LD->AddElement(vmCamp->Missions[i]->Name);
				MissListScrollPos=true;
			}
		}
	}
}
void cva_Mission::SetFrameState(SimpleDialog* SD){
	if(vmCamp){
		if(m_CurMiss!=-1){
			Index=m_CurMiss;
			m_CurMiss++;
		}
		VitButton* VB=Main.Get();
		if(!VB)	VB=dynamic_cast<VitButton*>(SD);
		SD->Visible=true;		
		if(VB){
			VB->Diffuse=0xFFFFFFFF;
			int nm=vmCamp->Missions.GetAmount();
			int nd=vmSinglePlayerData.NDoneMission[vmCampID];
			//
			int id=cva_Camp_StartMission::MissionID;
			if(id<0||id>=nm){
#ifdef _DEMO_				
				cva_Camp_StartMission::MissionID=0;
#else
				id=nd;
				if(id>=nm){
					id=nm-1;
				}
				cva_Camp_StartMission::MissionID=id;
#endif _DEMO_
			}
			
			GPPicture* gpFrontier=SD->DSS.GetAmount()?dynamic_cast<GPPicture*>(SD->DSS[0]):NULL;
			if(gpFrontier){
				gpFrontier->Diffuse=vmCamp->Frontier;
			}
			//
			if(Index<nm&&Index!=cva_Camp_StartMission::MissionID){
				VB->SetHint(vmCamp->Missions[Index]->Name);
			}			
#ifdef _DEMO_
			SD->Visible = false;
#ifdef _PRESSMAN_
			if(vmCampID==0){
				SD->Visible = (Index&7)==0 && Index<=vmSinglePlayerData.NDoneMission[vmCampID];
				if(Index<=vmSinglePlayerData.NDoneMission[vmCampID]){
					if(Index==6) SD->Visible = true;
				}
				int id=cva_Camp_StartMission::MissionID;
				if(!(id==0||id==6||id==8)){
					cva_Camp_StartMission::MissionID=0;
				}
			}else
			if(vmCampID==2){
				if(Index==1) SD->Visible = true;
				cva_Camp_StartMission::MissionID=1;
				vmSinglePlayerData.NDoneMission[vmCampID]=100;
			}
#else
			SD->Visible = (Index&7)==0 && Index<=vmSinglePlayerData.NDoneMission[vmCampID];
#endif _PRESSMAN_

			if(SD->Visible){
				if(Colored){
					VB->Diffuse=vmCamp->Finished;
				}else{
					VB->State=0;
				}
			}
#else
			VB->Diffuse=0xFFFFFFFF;
			if(Index<vmSinglePlayerData.NDoneMission[vmCampID]){
				// captured
				if(Colored){
					VB->Diffuse=vmCamp->Finished;
				}else{
					VB->State=0;
				}
				cvi_Mission* M=vmCamp->Missions[Index];
				VB->SetMessage(M->Name);
			}else
			if(Index==vmSinglePlayerData.NDoneMission[vmCampID]){
				// not captured but available
				if(Colored){
					VB->Diffuse=vmCamp->Next;
				}else{
					VB->State=0; //1
				}
				cvi_Mission* M=vmCamp->Missions[Index];
				VB->SetMessage(GetTextByID(M->Name));
				VB->Diffuse=0xFFFFFF00;
			}else{
				// not available
				SD->Visible=false;
			};			
#endif _DEMO_
			if(VB->MouseOver){
				if(Colored){
					if(gpFrontier){
						gpFrontier->Diffuse=vmCamp->Over;
					}else{
						VB->Diffuse=ModDWORD(VB->Diffuse,vmCamp->Over); //vmCamp->Over
					}
				}
			}
			if(cva_Camp_StartMission::MissionID==Index && SD->ParentDS && SD->ParentDS->ParentDS){
				
				// текущая выделенная карта
				if(MissListScrollPos){
					MissListScrollPos=false;
					ListDesk* LD=dynamic_cast<ListDesk*>(SD->ParentDS);
					if( LD ){
						VScrollBar* VS=LD->VScroller;
						if(VS){
							if(nd-1>0){
								VS->SPos=VS->SMaxPos*Index/(nd-1);
							}
						}
					}
				}

				cvi_Mission* M=vmCamp->Missions[Index];
				vmIGP.PreviewFile=M->PreviewFile;
				vmIGP.PreviewSprite=M->PreviewSprite;

				GPPicture* gpHero=(GPPicture*)SD->ParentDS->ParentDS->Find("Hero");
				if(gpHero){
					//gpHero->Visible=true;
					cvi_Mission* M=vmCamp->Missions[Index];
					//
					int xHero=SD->x+SD->GetWidth()/2;
					int yHero=SD->y+SD->GetHeight()/2;
					if(M->MapX>0){
						//xHero=M->MapX;
						//yHero=M->MapY;
					}
					extern int mouseX,mouseY;
					byte dir=GetDir(mouseX-xHero,(mouseY-yHero)*2);
					static float dirHero=dir;
					//
					int nG=GPS.GPNFrames(vmCamp->heroGP)/16;
					static timeRun=0;
					static fRun=0;
					int fL=GPS.GPNFrames(vmCamp->heroGP_Left)*byte(64-dir)/256;
					int cL=fL%nG;
					int fR=GPS.GPNFrames(vmCamp->heroGP_Right)*byte(dir-64)/256;
					int cR=fR%nG;					
					//
					//dir/=16;
					//dirHero/=16;
					//
					static int animHero=0;
					static int timeHero=GetTickCount();
					//
					int fG=((GetTickCount()-timeRun)/40+fRun)%nG;
					/*
					if(dir-byte(dirHero)<16||byte(dirHero)-dir<16){
						animHero=0;						
						timeRun=GetTickCount();
						fRun=cL;
						fG=cL;
					}else{						
						if(animHero==0&&cL==fG){
							animHero=1;
						}
						if(animHero==1){
							int t=GetTickCount();
							dirHero-=(t-timeHero)/10;
							timeHero=t;
						}
					}
					*/
					if(animHero==0){						
						gpHero->SetFileID(&vmCamp->heroGP);
						gpHero->SpriteID=fG*16+byte(dir-64)/16; //byte(dirHero)
					}else{
						gpHero->SetFileID(&vmCamp->heroGP_Left);
						gpHero->SpriteID=fL;
					}
					gpHero->Setx(xHero+vmCamp->heroDx);
					gpHero->Sety(yHero+vmCamp->heroDy);
				}
				if(Colored){
					if(gpFrontier){
						gpFrontier->Diffuse=vmCamp->Selected;
					}else{
						VB->Diffuse=vmCamp->Selected;
					}					
				}else{
					VB->State=2;
				}
			}
		}
	}else{
		SD->Visible=false;
	}
}
bool cva_Mission::LeftClick(SimpleDialog* SD){
	if(Index>=0&&Index<vmCamp->Missions.GetAmount()&&Index<=vmSinglePlayerData.NDoneMission[vmCampID]){
		cva_Camp_StartMission::MissionID=Index;
	}
	return true;
}
// cva_Missions_XY
void cva_Missions_XY::SetFrameState(SimpleDialog* SD){
	SD->Visible=true;
	int nm=vmCamp->Missions.GetAmount();
	int nGP=SD->DSS.GetAmount();
	for(int i=0;i<nGP;i++){
		SimpleDialog* sd=SD->DSS[i];
		if(sd) sd->Visible=false;
		GPPicture* GP=dynamic_cast<GPPicture*>(sd);
		if(GP){
			cvi_Mission* M=(i<nm)?vmCamp->Missions[i]:NULL;
			if(M&&M->MapX>0&&i==cva_Camp_StartMission::MissionID&&i<=vmSinglePlayerData.NDoneMission[vmCampID]){
				GP->Visible=true;
				GP->Setxy(M->MapX-GP->GetWidth()/2,M->MapY-GP->GetHeight()/2);
			}
		}		
	}
}
//
extern cvi_mfFrescos* DemoF;
void cva_Fresco_Desk::SetFrameState(SimpleDialog* SD){
	cvi_mfFrescos* DF=dynamic_cast<cvi_mfFrescos*>(DemoF);
	if(DF){
		DF->Process();
		if(DF->curFresco<DF->Frescos.GetAmount()){
			if(DF->curFresco==-1||cva_Fresco_Next::Clicked||DF->Frescos[DF->curFresco]->isFinished()){
				cva_Fresco_Next::Clicked=false;
				DF->curFresco++;
				if(DF->curFresco<DF->Frescos.GetAmount()){
					DF->Frescos[DF->curFresco]->Play(Picture,Text.Get());
				}
			}
			if(DF->curFresco<DF->Frescos.GetAmount()){
				DF->Frescos[DF->curFresco]->Process(Picture,Text.Get());
			}
		}else{
			v_MainMenu.ModalDesk="Fresco demo";
			DemoF=NULL;
		}
		return;
	}
	//
	if(!vmCamp) return;
	//
	vmCamp->Process();
	//
	if(!vmCamp) return;
	//
	cvi_mfFrescos* FR=cva_Fresco_Next::FR;
	//Scene[curFrame]
	if(FR){
		VitButton* MT=MissTitle.Get();
		if(MT){
			MT->SetMessage(vmCamp->Missions[vmCamp->curMission]->Name);
		}		
		if(FR->curFresco<FR->Frescos.GetAmount()){		
			if(FR->curFresco==-1||cva_Fresco_Next::Clicked){ //||FR->Frescos[FR->curFresco]->isFinished()
				cva_Fresco_Next::Clicked=false;
				FR->curFresco++;
				if(FR->curFresco<FR->Frescos.GetAmount()){
					FR->Frescos[FR->curFresco]->Play(Picture,Text.Get());
				}
			}else
			if(FR->curFresco!=-1){
				FR->Frescos[FR->curFresco]->Process(Picture,Text.Get());
			}
		}else{
			if(v_MainMenu.ModalDesk!="CampSelect"){
				//v_MainMenu.ModalDesk="Campaign";
			}else{
				cva_Fresco_Next::FR=NULL;
				vmCamp=NULL;
				vmCampID=-1;
			}
		}
	}
}
//
void cva_Fresco_Next::SetFrameState(SimpleDialog* SD){
}
bool cva_Fresco_Next::LeftClick(SimpleDialog* SD){
	Clicked=true;
	return true;
}
//
void cva_Fresco_Close::SetFrameState(SimpleDialog* SD){
	if(cva_Fresco_Next::FR){
		int n=cva_Fresco_Next::FR->Frescos.GetAmount();
		int cur=cva_Fresco_Next::FR->curFresco;
		if(cur>=n){
			LeftClick(SD);
		}
	}
}
bool cva_Fresco_Close::LeftClick(SimpleDialog* SD){
	if(cva_Fresco_Next::FR){
		int n=cva_Fresco_Next::FR->Frescos.GetAmount();
		cva_Fresco_Next::FR->curFresco=n;
	}
	return true;
}
void cva_Camp_Select::SetFrameState(SimpleDialog* SD){
	vmSinglePlayerData.CampMessageCheck=true;
	//
	DemoF=NULL;
	SD->Enabled=false;
	ListDesk* LD=List.Get();
	if(LD){
		int n=vmCampaigns.GetAmount();
		if(n>0){
			if(vmSinglePlayerData.NDoneMission[0]<vmCampaigns[0]->Missions.GetAmount()){
				n=1;
			}
#ifdef _DEMO_
			n=1;			
#ifdef _PRESSMAN_
			n=4;
			if(LD->CurrentElement==1||LD->CurrentElement==3){
				LD->CurrentElement=0;
			}
#else
			LD->CurrentElement=0;
#endif _PRESSMAN_
#endif _DEMO_
			SD->Enabled=true;
			if(LD->DSS.GetAmount()!=n){
				LD->DSS.Clear();
//#ifdef _DEMO_
//				LD->AddElement(vmCampaigns[0]->Name);
//#else
				for(int i=0;i<n;i++){
#ifdef _PRESSMAN_
					_str name;
					//if(i==1||i==3){
					//	name.print("{CR}%s{C}",vmCampaigns[i]->Name);
					//}else{
						name=vmCampaigns[i]->Name;
					//}
					LD->AddElement(name.pchar());
					if(i==1||i==3){
						SimpleDialog* E=LD->DSS[i];
						if(E) E->Enabled=false;
					}
#else
					LD->AddElement(vmCampaigns[i]->Name);
#endif _PRESSMAN_
				}
//#endif _DEMO_
//#ifdef _PRESSMAN_
//				LD->AddElement(vmCampaigns[1]->Name);				
//#endif _PRESSMAN_
				LD->CurrentElement=0;
			}			

			vmCamp=vmCampaigns[LD->CurrentElement];
			vmIGP.PreviewFile=vmCamp->PreviewFile;
			vmIGP.PreviewSprite=vmCamp->PreviewSprite;

		}
	}
}
bool cva_Camp_Select::LeftClick(SimpleDialog* SD){	
	vmCamp=NULL;
	vmCampID=-1;
	ListDesk* LD=List.Get();
	if(LD){
		int i=LD->CurrentElement;
		if(i>=0&&i<vmCampaigns.GetAmount()){
			vmCamp=vmCampaigns[LD->CurrentElement];		
			vmCampID=LD->CurrentElement;
			vmEndCampaign=false;
		}		
	}
	if(!vmCamp){
		v_MainMenu.ModalDesk="CampSelect";
	}	
	return true;
}
void cva_CCamp_Name::SetFrameState(SimpleDialog* SD){
	if(vmCamp){
		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if(VB){
			VB->SetMessage(vmCamp->Name);
		}
	}
}
void cva_CCamp_Sectors::SetFrameState(SimpleDialog* SD){
	if(vmCamp){
		SD->Visible=(vmCamp->DeskName==SD->Name);
	}
}

void cva_CampDiff::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){		
		CB->Clear();
		_str Line;
		int def=0;
		for(int i=0;i<GlobalAI.Diff.GetAmount();i++){
			vsDiffLevel* dl=GlobalAI.Diff+i;
			if(dl->Enable) Line="";
			else Line="{C C0665F57}";
			Line+=dl->Name;
			CB->AddLine(Line.pchar());
			if(dl->Enable==2&&def==0) def=i;
		}
		//CB->CurLine=def;
		CB->CurLine=vmSinglePlayerData.Difficulty;
	}		
}
void cva_CampDiff::SetFrameState(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		//VB->SetMessage(vmCamp->Name);
		TextButton* tbMD=tbMissDescr.Get();
		if(tbMD){
			int mid=cva_Camp_StartMission::MissionID;
			if(mid>=0){
				int n=std::min(vmSinglePlayerData.NDoneMission[vmCampID]+1,vmCamp->Missions.GetAmount());
				if(n&&mid>=n){
					mid=n-1;
					cva_Camp_StartMission::MissionID=mid;
				}
				if(mid>=0) tbMD->SetMessage(vmCamp->Missions[mid]->GameDescription);
			}
		}
		static int time=0;
		if(time<GetTickCount()&&CB->NLines>1){
			//CB->CurLine=vmSinglePlayerData.Difficulty;
		}else{
			vmSinglePlayerData.Difficulty=CB->CurLine;
		}
		time=GetTickCount()+1000;
		//
		vNewProf.m_iDifficulty=CB->CurLine;
	}
}
void cva_CMiss_Name::SetFrameState(SimpleDialog* SD){
	int mid=cva_Camp_StartMission::MissionID;
	if(mid>=0){
		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if(VB){		
			VB->SetMessage(vmCamp->Missions[mid]->Name);
		}
		TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
			TB->SetMessage(vmCamp->Missions[mid]->Name);
		}
	}
}
//
int cva_Camp_StartMission::MissionID=-1;
void cva_Camp_StartMission::Init(SimpleDialog* SD){
	//MissionID=-1;
}
void cva_Camp_StartMission::SetFrameState(SimpleDialog* SD){
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	if(VB){
	}
	SD->Enabled=MissionID!=-1;
	if( vmIGP.StartNextCampMission && v_MainMenu.Cross==0 ){
		vmIGP.StartNextCampMission=false;
		if(SD->Enabled){
			SD->v_Actions.LeftClick(SD);
			v_MainMenu.CrossTime=10000/*FadeTime*/;
		}
	}
}
int rusDiff=1;
bool cva_Camp_StartMission::LeftClick(SimpleDialog* SD){
	vmCamp->curMission=MissionID;
	if(vmCamp->Missions[MissionID]->Scene.GetAmount()>0){
		rusDiff=vmSinglePlayerData.Difficulty;
		//vmCamp->Missions[MissionID]->Scene[0]->Play();
		//vmCamp->Missions[MissionID]->curFrame=0;
		vmCamp->Missions[MissionID]->StartScene();
	}
	return true;
}
//
void rcm_InitStatistic(){
	if( vGameMode==gmCamp&&gExpMan()&&vmCamp&& vmCampID>=0&&vmCampID<vmSinglePlayerData.Heri.GetAmount() ){
		if(vmCamp->curMission>=0&&vmCamp->curMission<vmSinglePlayerData.Heri[vmCampID]->CampaignMissions.GetAmount()){
			SinglePlayerData_HeroesInfoList* HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
			if(HI){
				HI->FreeExp+=gExpMan()->GetExp();
			}
			//v_MainMenu.StartDS("EndGameMessage",true);
		}
	}
}
// cva_RC_Statistic
void cva_RC_Statistic::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if( vGameMode==gmCamp&&gExpMan() && vmCampID<vmSinglePlayerData.Heri.GetAmount() ){
		//
		SinglePlayerData_HeroesInfoList* HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
		//
		SD->Visible=true;
		static _str txt;
		TextButton* tbCurExp=CurExp.Get();
		if(tbCurExp){
			txt.Clear();
			txt.print("%d",HI->FreeExp);
			tbCurExp->SetMessage(txt.pchar());		
		}
		DialogsDesk* ddIT=InfoTable.Get();
		if(ddIT){		
			for(int i=0;i<gExpMan()->GetNCtg();i++){
				if(i<ddIT->DSS.GetAmount()){
					DialogsDesk* ddLine=(DialogsDesk*)ddIT->DSS[i];
					for(int j=0;j<gExpMan()->GetNCol(i);j++){
						if(j+1<ddLine->DSS.GetAmount()){
							TextButton* tbFild=(TextButton*)ddLine->DSS[j+1];
							gExpMan()->GetExpInfo(txt,i,j);
							tbFild->SetMessage(txt.pchar());
						}
					}
				}
			}
		}
	}	
}

typedef struct
{
	GPPicture *gpCardA, *gpCardB, *gpCardC, *gpHeroeA, *gpHeroeB, *gpHeroeC,
		*gpUpgradeAA, *gpUpgradeAB, *gpUpgradeAC,
		*gpUpgradeBA, *gpUpgradeBB, *gpUpgradeBC,
		*gpUpgradeCA, *gpUpgradeCB, *gpUpgradeCC;
	TextButton *tbHATitle, *tbHBTitle, *tbHCTitle, *tbAllExp,
		*tbLPriceAA, *tbLPriceAB, *tbLPriceAC,
		*tbLPriceBA, *tbLPriceBB, *tbLPriceBC,
		*tbLPriceCA, *tbLPriceCB, *tbLPriceCC;
}hu_TControls;

hu_TControls hu_Controls;
//
bool vPlus[3];
//
void cva_HeroesUpgrades::SetFrameState(SimpleDialog *SD)
{
	ZeroMemory(&hu_Controls, sizeof(hu_TControls));
	hu_Controls.gpCardA = CardA.Get(),
		hu_Controls.gpCardB = CardB.Get(),
		hu_Controls.gpCardC = CardC.Get(),
		hu_Controls.gpHeroeA = HeroeA.Get(),
		hu_Controls.gpHeroeB = HeroeB.Get(),
		hu_Controls.gpHeroeC = HeroeC.Get(),
		hu_Controls.tbHATitle = HATitle.Get(),
		hu_Controls.tbHBTitle = HBTitle.Get(),
		hu_Controls.tbHCTitle = HCTitle.Get(),
		hu_Controls.tbLPriceAA = LPriceAA.Get(),
		hu_Controls.tbLPriceAB = LPriceAB.Get(),
		hu_Controls.tbLPriceAC = LPriceAC.Get(),
		hu_Controls.tbLPriceBA = LPriceBA.Get(),
		hu_Controls.tbLPriceBB = LPriceBB.Get(),
		hu_Controls.tbLPriceBC = LPriceBC.Get(),
		hu_Controls.tbLPriceCA = LPriceCA.Get(),
		hu_Controls.tbLPriceCB = LPriceCB.Get(),
		hu_Controls.tbLPriceCC = LPriceCC.Get(),
		hu_Controls.tbAllExp = AllExp.Get(),
		hu_Controls.gpUpgradeAA = UpgradeAA.Get(),
		hu_Controls.gpUpgradeAB = UpgradeAB.Get(),
		hu_Controls.gpUpgradeAC = UpgradeAC.Get(),
		hu_Controls.gpUpgradeBA = UpgradeBA.Get(),
		hu_Controls.gpUpgradeBB = UpgradeBB.Get(),
		hu_Controls.gpUpgradeBC = UpgradeBC.Get(),
		hu_Controls.gpUpgradeCA = UpgradeCA.Get(),
		hu_Controls.gpUpgradeCB = UpgradeCB.Get(),
		hu_Controls.gpUpgradeCC = UpgradeCC.Get();
	if(!hu_Controls.gpCardA || !hu_Controls.gpCardB || !hu_Controls.gpCardC ||
		!hu_Controls.gpHeroeA || !hu_Controls.gpHeroeB || !hu_Controls.gpHeroeC ||
		!hu_Controls.tbHATitle || !hu_Controls.tbHBTitle || !hu_Controls.tbHCTitle ||
		!hu_Controls.tbLPriceAA || !hu_Controls.tbLPriceAB || !hu_Controls.tbLPriceAC ||
		!hu_Controls.tbLPriceBA || !hu_Controls.tbLPriceBB || !hu_Controls.tbLPriceBC ||
		!hu_Controls.tbLPriceCA || !hu_Controls.tbLPriceCB || !hu_Controls.tbLPriceCC ||
		!hu_Controls.gpUpgradeAA || !hu_Controls.gpUpgradeAB || !hu_Controls.gpUpgradeAC ||
		!hu_Controls.gpUpgradeBA || !hu_Controls.gpUpgradeBB || !hu_Controls.gpUpgradeBC ||
		!hu_Controls.gpUpgradeCA || !hu_Controls.gpUpgradeCB || !hu_Controls.gpUpgradeCC ||
		!hu_Controls.tbAllExp) return;
	// Cards Visibility
	hu_Controls.gpCardA->Visible = hu_Controls.gpCardB->Visible =
	hu_Controls.gpCardC->Visible = true;
	hu_Controls.gpCardA->SetSpriteID(3);
	hu_Controls.gpCardB->SetSpriteID(3);
	hu_Controls.gpCardC->SetSpriteID(3);
	hu_Controls.tbHATitle->Visible = hu_Controls.tbHBTitle->Visible =
		hu_Controls.tbHCTitle->Visible = false;
	hu_Controls.gpHeroeA->Visible = hu_Controls.gpHeroeB->Visible =
		hu_Controls.gpHeroeC->Visible = false;
	hu_Controls.gpUpgradeAA->Visible = hu_Controls.gpUpgradeAB->Visible =
		hu_Controls.gpUpgradeAC->Visible = false;
	hu_Controls.gpUpgradeBA->Visible = hu_Controls.gpUpgradeBB->Visible =
		hu_Controls.gpUpgradeBC->Visible = false;
	hu_Controls.gpUpgradeCA->Visible = hu_Controls.gpUpgradeCB->Visible =
		hu_Controls.gpUpgradeCC->Visible = false;
	hu_Controls.tbLPriceAA->Visible = hu_Controls.tbLPriceAB->Visible =
		hu_Controls.tbLPriceAC->Visible = false;
	hu_Controls.tbLPriceBA->Visible = hu_Controls.tbLPriceBB->Visible =
		hu_Controls.tbLPriceBC->Visible = false;
	hu_Controls.tbLPriceCA->Visible = hu_Controls.tbLPriceCB->Visible =
		hu_Controls.tbLPriceCC->Visible = false;
	// Getting heroes handlers
	OneObject *ooHeroes[3] = {NULL, NULL, NULL};
	byte NI = GSets.CGame.cgi_NatRefTBL[MyNation];
	int i = 0;
	for(i = 0; i < 3; i++) ooHeroes[i] = vHeroButtons.GetObject(NI, i);
	// Cards Visibility
	if(ooHeroes[0] != NULL)
	{
		hu_Controls.gpCardA->SetSpriteID(1);
		hu_Controls.gpHeroeA->Visible = true;
		hu_Controls.tbHATitle->Visible = true;
		//hu_Controls.tbLPriceAA->Visible = hu_Controls.tbLPriceAB->Visible =	hu_Controls.tbLPriceAC->Visible = true;
		hu_Controls.tbLPriceAB->Visible = true;
		hu_Controls.gpUpgradeAA->Visible = hu_Controls.gpUpgradeAB->Visible =
			hu_Controls.gpUpgradeAC->Visible = true;
	}
	if(ooHeroes[1] != NULL)
	{
		hu_Controls.gpCardB->SetSpriteID(1);
		hu_Controls.gpHeroeB->Visible = true;
		hu_Controls.tbHBTitle->Visible = true;
		//hu_Controls.tbLPriceBA->Visible = hu_Controls.tbLPriceBB->Visible =	hu_Controls.tbLPriceBC->Visible = true;
		hu_Controls.tbLPriceBB->Visible = true;
		hu_Controls.gpUpgradeBA->Visible = hu_Controls.gpUpgradeBB->Visible =
			hu_Controls.gpUpgradeBC->Visible = true;
	}
	if(ooHeroes[2] != NULL)
	{
		hu_Controls.gpCardC->SetSpriteID(1);
		hu_Controls.gpHeroeC->Visible = true;
		hu_Controls.tbHCTitle->Visible = true;
		//hu_Controls.tbLPriceCA->Visible = hu_Controls.tbLPriceCB->Visible = hu_Controls.tbLPriceCC->Visible = true;
		hu_Controls.tbLPriceCB->Visible = true;
		hu_Controls.gpUpgradeCA->Visible = hu_Controls.gpUpgradeCB->Visible =
			hu_Controls.gpUpgradeCC->Visible = true;
	}
	// Setting title and pictures
	GPPicture *gpPictures[3] = {hu_Controls.gpHeroeA, hu_Controls.gpHeroeB,
		hu_Controls.gpHeroeC};
	TextButton *tbTitles[3] = {hu_Controls.tbHATitle, hu_Controls.tbHBTitle,
		hu_Controls.tbHCTitle};
	for(i = 0; i < 3; i++)
		if(ooHeroes[i] != NULL)
		{
			NewMonster *pNM = NATIONS->Mon[ooHeroes[i]->NIndex]->newMons;
			if(pNM != NULL)
			{
				if(pNM->BigIconFile != 0xFFFF)
				{
					gpPictures[i]->FileID = pNM->BigIconFile;
					gpPictures[i]->SetSpriteID(pNM->BigIconIndex);
				}
				else // This situation shouldn't be exist, but...
				{
					gpPictures[i]->FileID = pNM->IconFileID;
					gpPictures[i]->SetSpriteID(pNM->IconID);
				}
				tbTitles[i]->SetMessage(pNM->Message);
			}
		}
	// Free experience

	char Str[4096];
	SinglePlayerData_HeroesInfoList* HI=NULL;
	if(vmCampID >= 0)
	{		
		HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
		if(HI)
		{
			sprintf(Str, "%d", HI->FreeExp);
			hu_Controls.tbAllExp->SetMessage(Str);
		}
	}
	int dExp=0;
	_str T; T=dExp;
	OneObject* OO;
	//
	OO=ooHeroes[0];
	if(OO != NULL)
	{		
		if(OO&&OO->ActiveAbility){
			int n = OO->ActiveAbility->ActiveAbilities.GetAmount();
			for(int i=0;i<n;i++){
				ActiveRomeHero* Hr = dynamic_cast<ActiveRomeHero*> (OO->ActiveAbility->ActiveAbilities[i]);
				if(Hr){
					dExp=Hr->GetExperienceToNextLevel()-Hr->GetExperience();
					break;
				}
			}
		}
		if(HI&&HI->FreeExp<dExp){
			//T.Clear();
			//T.print("{CR}%d{C}",dExp);
			vPlus[0]=false;
		}else{			
			vPlus[0]=true;
		}
		T=dExp;
	}
	hu_Controls.tbLPriceAA->SetMessage(T.pchar());
	hu_Controls.tbLPriceAB->SetMessage(T.pchar());
	hu_Controls.tbLPriceAC->SetMessage(T.pchar());

	OO=ooHeroes[1];
	if(OO != NULL)
	{		
		if(OO&&OO->ActiveAbility){
			int n = OO->ActiveAbility->ActiveAbilities.GetAmount();
			for(int i=0;i<n;i++){
				ActiveRomeHero* Hr = dynamic_cast<ActiveRomeHero*> (OO->ActiveAbility->ActiveAbilities[i]);
				if(Hr){
					dExp=Hr->GetExperienceToNextLevel()-Hr->GetExperience();
					break;
				}
			}
		}
		if(HI&&HI->FreeExp<dExp){
			//T.Clear();
			//T.print("{CR}%d{C}",dExp);
			vPlus[1]=false;
		}else{			
			vPlus[1]=true;
		}
		T=dExp;
	}
	hu_Controls.tbLPriceBA->SetMessage(T.pchar());
	hu_Controls.tbLPriceBB->SetMessage(T.pchar());
	hu_Controls.tbLPriceBC->SetMessage(T.pchar());

	OO=ooHeroes[2];
	if(OO != NULL)
	{		
		if(OO&&OO->ActiveAbility){
			int n = OO->ActiveAbility->ActiveAbilities.GetAmount();
			for(int i=0;i<n;i++){
				ActiveRomeHero* Hr = dynamic_cast<ActiveRomeHero*> (OO->ActiveAbility->ActiveAbilities[i]);
				if(Hr){
					dExp=Hr->GetExperienceToNextLevel()-Hr->GetExperience();
					break;
				}
			}
		}
		if(HI&&HI->FreeExp<dExp){
			//T.Clear();
			//T.print("{CR}%d{C}",dExp);
			vPlus[2]=false;
		}else{			
			vPlus[2]=true;
		}
		T=dExp;
	}
	hu_Controls.tbLPriceCA->SetMessage(T.pchar());
	hu_Controls.tbLPriceCB->SetMessage(T.pchar());
	hu_Controls.tbLPriceCC->SetMessage(T.pchar());
}
int dExp = 0;
void cva_HeroeXUpgradeX::SetFrameState(SimpleDialog *SD)
{
	GPPicture *gpAbility = NULL, *gpPlus = NULL, *gpLevel = NULL;
	gpAbility = Ability.Get(), gpPlus = Plus.Get(), gpLevel = Level.Get();
	if(!gpAbility || !gpPlus || !gpLevel) return;

	OneObject *OO = NULL;
	byte NI = GSets.CGame.cgi_NatRefTBL[MyNation];
	int n = 0;
	OO = vHeroButtons.GetObject(NI, idHeroe);
	LeveledActiveUnitAbility *Lv = NULL;
	UnitAbilityIcon *UAI = NULL;
	if(OO != NULL)
		if(OO->ActiveAbility != NULL)
		{
			n = OO->ActiveAbility->ActiveAbilities.GetAmount();
			int id = 0;
			for(int i = 0; i < n; i++)
			{
				Lv = dynamic_cast<LeveledActiveUnitAbility*> (OO->ActiveAbility->ActiveAbilities[i]);
				if(Lv&&Lv->Visible) // Ability visibility
				{
					if(idUpgrade == id)
					{
						UAI = Lv->GetUnitAbilityIconInfo();
						if(UAI)
						{
							gpAbility->FileID = UAI->GetGPFileID(0);
							gpAbility->SetSpriteID(UAI->GetGPSpriteID(0));
							SD->SetHint(Lv->GetHint());
							//gpAbility->SetHint(Lv->GetHint());
							gpLevel->SetSpriteID(*Lv->Le);
							gpPlus->Visible=(*Lv->Le<5)&&(vPlus[idHeroe]);
						}
						break;
					}
					id++;
				}
			}
		}

	if(OO&&OO->ActiveAbility){
		for(int i=0;i<n;i++){
			ActiveRomeHero* Hr = dynamic_cast<ActiveRomeHero*> (OO->ActiveAbility->ActiveAbilities[i]);
			if(Hr){
				dExp=Hr->GetExperienceToNextLevel()-Hr->GetExperience();

			}
		}
	}
}

bool cva_HeroeXUpgradeX::LeftClick(SimpleDialog *SD)
{
	GPPicture *gpAbility = NULL, *gpPlus = NULL, *gpLevel = NULL;
	gpAbility = Ability.Get(), gpPlus = Plus.Get(), gpLevel = Level.Get();
	if(!gpAbility || !gpPlus || !gpLevel) return true;

//	char Str[4096];
//	sprintf(Str, "idHeroe = %d\r\nidUpgrade = %d", idHeroe, idUpgrade);
///	MessageBox(NULL, Str, "LeftClick:", 0);
//	return true;

	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	OneObject* OB=vHeroButtons.GetObject(NI,idHeroe);
	if(OB == NULL) return true;

	int dExp=0;
	if(OB&&OB->ActiveAbility){
		int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i=0;i<n;i++){
			ActiveRomeHero* Hr = dynamic_cast<ActiveRomeHero*> (OB->ActiveAbility->ActiveAbilities[i]);
			if(Hr){
				dExp=Hr->GetExperienceToNextLevel()-Hr->GetExperience();
				break;
			}
		}
	}

	SinglePlayerData_HeroesInfoList* HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
	if(HI->FreeExp>=dExp&&dExp>0){						
		//GPPicture* GP=dynamic_cast<GPPicture*>(SD);
		GPPicture *GP = gpLevel;
		if(GP&&OB&&OB->ActiveAbility){
			int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
			int id=0;
			ActiveRomeHero* Hr = NULL;			
			for(int i=0;i<n;i++){
				Hr = dynamic_cast<ActiveRomeHero*> (OB->ActiveAbility->ActiveAbilities[i]);
				if(Hr){
					break;
				}
			}
			//
			for(int i=0;i<n;i++){
				LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[i]);
				if(Lv&&Lv->Visible){ // Ability Visibility
					if(idUpgrade==id&&Hr&&Lv->UpLevel()){
						Hr->UpLevel();
						Hr->AddExperience(dExp);
						HI->FreeExp-=dExp;
						break;
					}
					id++;
				}
			}
		}
	}	
	return true;
}

// cva_RC_Hero
void cva_RC_Hero::SetFrameState(SimpleDialog* SD){
	if(SD->DSS.GetAmount()>1){
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];	
		OneObject* OB=vHeroButtons.GetObject(NI,HeroID);
		if(OB){
			SD->Visible=true;
			GPPicture* GP=dynamic_cast<GPPicture*>(SD->DSS[0]);
			if(GP){
				int file=OB->newMons->IconFileID;
				int spr=OB->newMons->IconID;
				GP->FileID=file;
				GP->SetSpriteID(spr);
			}
			_str txt;
			TextButton* TB=dynamic_cast<TextButton*>(SD->DSS[1]);
			if(TB){
				int dExp=0;				
				int n = OB->ActiveAbility->ActiveAbilities.GetAmount();				
				for(int i=0;i<n;i++){
					ActiveRomeHero* Hr = dynamic_cast<ActiveRomeHero*> (OB->ActiveAbility->ActiveAbilities[i]);
					if(Hr){
						dExp=Hr->GetExperienceToNextLevel()-*Hr->Ex/100;						
					}
				}
				txt=dExp;
				TB->SetMessage(txt.pchar());
			}
		}else{
			SD->Visible=false;
		}
	}
}
// cva_RC_Ability
void cva_RC_Ability::SetFrameState(SimpleDialog* SD){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];	
	OneObject* OB=vHeroButtons.GetObject(NI,HeroID);
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP&&OB&&OB->ActiveAbility){
		int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
		int id=0;
		for(int i=0;i<n;i++)
		{
			LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[i]);
			if(Lv)
			{				
				if(AbilityID==id){
					UnitAbilityIcon* UAI=Lv->GetUnitAbilityIconInfo();
					if(UAI){
						GP->FileID=UAI->GetGPFileID(0);
						GP->SetSpriteID(UAI->GetGPSpriteID(0));
						if(GP->DSS.GetAmount()>1){
							GP=(GPPicture*)GP->DSS[1];
							GP->SetSpriteID(*Lv->Le);
						}					
					}
				}
				id++;
			}
			ActiveRomeHero* Hr = dynamic_cast<ActiveRomeHero*> (OB->ActiveAbility->ActiveAbilities[i]);
			if(Hr){
				dExp=Hr->GetExperienceToNextLevel()-Hr->GetExperience();
				//dExp=100;
			}
		}
	}
}
bool cva_RC_Ability::LeftClick(SimpleDialog* SD){
	SinglePlayerData_HeroesInfoList* HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
	if(HI->FreeExp>=dExp&&dExp>0){		
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];	
		OneObject* OB=vHeroButtons.GetObject(NI,HeroID);
		GPPicture* GP=dynamic_cast<GPPicture*>(SD);
		if(GP&&OB&&OB->ActiveAbility){
			int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
			int id=0;
			ActiveRomeHero* Hr = NULL;
			for(int i=0;i<n;i++){
				Hr = dynamic_cast<ActiveRomeHero*> (OB->ActiveAbility->ActiveAbilities[i]);
				if(Hr){
					break;
				}
			}			
			for(i=0;i<n;i++){
				LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[i]);
				if(Lv){				
					if(AbilityID==id&&Hr&&Lv->UpLevel()){
						Hr->UpLevel();
						Hr->AddExperience(dExp);
						HI->FreeExp-=dExp;
						break;
					}
					id++;
				}
			}
		}
	}	
	return true;
}
// cva_SPD_FrescoList
DynArray<cvi_mfFrescos*> FA;
void cva_SPD_FrescoList::Init(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		LD->CurrentElement=-1;
		FA.Clear();
		LD->DSS.Clear();
		_str Line;
		for(int i=0;i<vmCampaigns.GetAmount();i++){
			cvi_Campaign* C=vmCampaigns[i];
			if(C){
				for(int j=0;j<C->Missions.GetAmount();j++){
					cvi_Mission* M=C->Missions[j];
					if(M){
						for(int k=0;k<M->Scene.GetAmount();k++){
							cvi_mfFrescos* F=dynamic_cast<cvi_mfFrescos*>(M->Scene[k]);
							if(F){
								FA.Add(F);
								Line.Clear();
								Line.print("%s: Miss %d ",C->Name,j+1);
								if(k==0){
									Line+="begin";
								}else{
									Line+="end";
								}
								LD->AddElement(Line.pchar());
							}
						}
					}
				}
			}
		}
		if(LD->DSS.GetAmount()) LD->CurrentElement=0;
	}
}
cvi_mfFrescos* DemoF=NULL;
void cva_SPD_FrescoList::Accept(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	int n=FA.GetAmount();
	DemoF=NULL;
	if(LD&&LD->CurrentElement>=0&&LD->CurrentElement<n){		
		DemoF=FA[LD->CurrentElement];
		DemoF->Play();
	}
}
void cva_SPD_FrescoList::SetFrameState(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	SD->Enabled=LD&&LD->CurrentElement!=-1;
}

cvi_Mission::cvi_Mission(){
	curFrame=-1;
};