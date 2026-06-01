#include "stdheader.h"
#include "GameExtension.h"
#include ".\cext_VisualInterface.h"
#include ".\cvi_HeroButtons.h"
#include ".\vui_Effects.h"
#include ".\cvi_campaign.h"
#include ".\cvi_singleplayerdata.h"
#include ".\vui_GlobalHotKey.h"
#include ".\cvi_market.h"
#include ".\cvi_Missions.h"
#include "ua_Item.h"
#include "UnitsInterface.h"
#include "Surface\Surface.h"
#include ".\cvi_InGamePanel.h"

#include "GameInterface.h"

//////////////////////////////////////////////////////////////////////////
CIMPORT void GetDipSimpleBuildings(int &NDips, DIP_SimpleBuilding** &Dips);
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
CEXPORT UnitsGroup* GetUnitsGroup(GAMEOBJ* Group);
DLLEXPORT OneObject* GetOBJ(int Index);
DLLEXPORT bool GetZoneR(GAMEOBJ* Zone,int* R);
CEXPORT void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2);
DIALOGS_API void PlaySound(char* Name,int x, int y);
DIALOGS_API void PlaySound(char* Name);
extern word NPlayers;
extern byte PlayGameMode;
//////////////////////////////////////////////////////////////////////////

class Voice: public BaseClass{
public:
	_str File;
	int StartTime;
};

ClassArray<Voice> VoiceQueue;
const int VoiceQueueChanel=1;

void AddVoiceToQueue(char* FileName){
	Voice* V = new Voice;
	V->File=FileName;
	V->StartTime=GetTickCount();
	VoiceQueue.Add(V);
	//Log.Warning("Add:  %s",FileName);
}

void VoiceQueueProcess(){
	if( VoiceQueue.GetAmount() && ov_StreamFinished(VoiceQueueChanel) ){
		Voice* V=VoiceQueue[0];
		int T=GetTickCount();
		if(T-V->StartTime<7000){
			ov_Stop( VoiceQueueChanel );
			ov_Play(V->File.pchar(),VoiceQueueChanel);
			//Log.Warning("Play: %s",V->File.pchar());
		}
		VoiceQueue.DelElement(0);
	}
}

//
bool SetlRadiusVisible=false;
int SetlRadiusID;
//
bool ShowSetlRadius(){
	if(SetlRadiusVisible){
		SetlRadiusVisible=false;
		DIP_SimpleBuilding** DSB;
		int NDips;
		GetDipSimpleBuildings(NDips,DSB);
		DIP_SimpleBuilding* CDSB=DSB[SetlRadiusID];
		UnitsGroup* UG=GetUnitsGroup(&CDSB->CentralGroup);
		OneObject* OB=GetOBJ(UG->IDS[0]);
		if(OB){	//&&(OB->Selected||(GetKeyState(VK_MENU)&0x8000))
			int r0,r1;
			GetZoneR(&CDSB->BigZone,&r0);
			GetZoneR(&CDSB->VeryBigZone,&r1);
			DrawColoredCircle(OB->RealX>>4,OB->RealY>>4,r0,0,0xFFFF2020,0xFFFFFFFF,0x2FFF2020,0x2FFFFFFF);
		}
	}	
	return false;
}
//
bool cva_VI_Setl::LeftClick(SimpleDialog* SD){
	DIP_SimpleBuilding** SBs;
	int NDips;
	GetDipSimpleBuildings(NDips,SBs);
	if(NDips&&SD->ID<NDips){
		DIP_SimpleBuilding* SB=SBs[SD->ID];
		if(SB){
			OneObject* OB=SB->GetMainObj();
			if(OB){
				void CmdSelObject(OneObject* OB);
				CmdSelObject(OB);
			}
		}
	}
	return true;
}
void cva_VI_Setl::SetFrameState(SimpleDialog* SD){
	if(!SD->Visible){
		return;
	}
	if(SD->MouseOver){
		SetlRadiusVisible=true;
		SetlRadiusID=SD->ID;
	}
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP){
		if(EngSettings.vInterf.sprSettlementAni){
			int N=GPS.GPNFrames(GP->FileID);
			if(N){
				static int t=GetTickCount();
				int T=GetTickCount();
				int spr=((T-t)/80)%N;
				GP->SetSpriteID(spr);
			}
			GPPicture* gP=NULL;
			if(GP->DSS.GetAmount()==0){
				DialogsDesk* DD=new DialogsDesk;
				GP->AddDialog(DD);
				DD->SetWidth(1000);
				DD->SetHeight(1000);
				gP=DD->addGPPicture(NULL,0,0,EngSettings.Resource.File,0);				
			}else{
				gP=dynamic_cast<GPPicture*>(GP->DSS[0]->DSS[0]);
			}
			if(gP){
				gP->Setx(EngSettings.vInterf.sprSettlementAniX);
				gP->Sety(EngSettings.vInterf.sprSettlementAniY);
				DIP_SimpleBuilding** SBs;
				int NDips;
				GetDipSimpleBuildings(NDips,SBs);
				if(NDips&&GP->ID<NDips){
					DIP_SimpleBuilding* SB=SBs[GP->ID];
					if(SB){
						GP->Nation=SB->Owner;
						byte res=0xFF;
						for(int i=0;i<6;i++){
							if(SB->Produce[i]){
								res=i;
								break;
							}
						}
						if(res!=0xFF){
							gP->SetSpriteID(EngSettings.Resource.Sprite[res]);						
						}
					}
				}
			}
		}
	}
}

void TakeResLink(OneObject* OBJ);

//
void cext_VisualInterface::OnDrawOnMapAfterTransparentEffects(){
	// settlement picture with hint
	if(EngSettings.vInterf.fSettlement==-1){
		EngSettings.vInterf.fSettlement=GPS.PreLoadGPImage("interf3\\f_icons");
		EngSettings.vInterf.sprSettlement=19;
	}
	DIP_SimpleBuilding** SBs;
	int NDips;
	GetDipSimpleBuildings(NDips,SBs);
	if(NDips){
		static DialogsSystem* dsSettl=new DialogsSystem;	
		int i = 0;
		for(i;i<NDips;i++){
			GPPicture* GP=NULL;
			if(i>=dsSettl->DSS.GetAmount()){
				GP=new GPPicture;
				GP->FileID=EngSettings.vInterf.fSettlement;
				GP->SetSpriteID(EngSettings.vInterf.sprSettlement);
				dsSettl->DSS.Add(GP);
				vui_Action* A=new cva_VI_Setl;
				GP->v_Actions.Add(A);
			}else{
				GP=(GPPicture*)dsSettl->DSS[i];
			}
			GP->Visible=false;
			DIP_SimpleBuilding* SB=SBs[i];
			if(SB){
				int x,y;
				SB->GetCenter(x,y);
				int z=GetTotalHeight(x,y);			
				if(x>0&&GetObjectVisibilityInFog(x,y,z,NULL)){
					int dH=EngSettings.vInterf.SettlementSpriteHeight;
					OneObject* MainOB=SB->GetMainObj();
					if(MainOB){
						short N=MainOB->newMons->NBars;
						short* B=MainOB->newMons->Bars3D;
						dH=0;
						for(int i=0;i<N;i++){
							int H=B[i*5+4];
							if(H>dH){
								dH=H;
							}
						}
						//dH-=GPS.GetGPHeight(EngSettings.vInterf.fSettlement,EngSettings.vInterf.sprSettlement);					
					}
					Vector3D v = SkewPt(x,y,z+dH);
					WorldToScreenSpace(v);
					//!IMM->IsPointVisible(v)
					if(v.x>-200&&v.y>-200&&v.x<RealLx+200&&v.y<RealLy+200){
						//GPS.ShowGP(v.x,v.y,EngSettings.vInterf.fSettlement,EngSettings.vInterf.sprSettlement,SB->Owner);
						GP->Visible=true;
						GP->Setx(v.x);
						GP->Sety(v.y);
						//
						GP->ID=i;
						//GP->UserParam=y;
						//
						GP->Nation=SB->Owner;
						_str hint;
						static char* tidSettl=GetTextByID("#SettlHint");
						for(int i=0;i<6;i++){
							if(SB->Produce[i]){
								int r=SB->Resource[i];
								/*
								if(r>SB->CaravanCapacity[i]){
								r=SB->CaravanCapacity[i];
								}
								*/
								hint.print(tidSettl,RDS[i].Name,r,SB->CaravanCapacity[i]);
								break;
							}						
						}					
						GP->SetHint(hint.pchar());
					}
				}			
			}
		}
		for(;i<dsSettl->DSS.GetAmount();i++){
			dsSettl->DSS[i]->Visible=false;
		}
		dsSettl->ProcessDialogs();
	}

	// peasant resource visualization
	//for(int i=0;i<MAXOBJECT;i++){
	//	OneObject* OB=Group[i];
	//	if( OB && !OB->Sdoxlo && OB->newMons->Peasant && OB->RAmount>0 ){
	//		int x=OB->GetAttX()>>4;
	//		int y=OB->GetAttY()>>4;
	//		int z=OB->RZ;
	//		if( GetObjectVisibilityInFog(x,y,z,OB) ){
	//			Order1* Or=OB->LocalOrder;
	//			while(Or){
	//				if( Or->DoLink==&TakeResLink ){
	//					x+=OB->newMons->selShift*TCos[OB->RealDir]/256;
	//					y+=OB->newMons->selShift*TSin[OB->RealDir]/256;
	//					int r,h;
	//					OB->GetCylinder(r,h,z);
	//					Vector3D v = Vector3D(x,y,h+z);
	//					WorldToScreenSpace(v);
	//					if( v.x>-200 && v.y>-200 && v.x<RealLx+200 && v.y<RealLy+200 ){
	//						byte ResID=Or->info.TakeRes.ResID;
	//						int FileID=EngSettings.Resource.File;
	//						int SpriteID=EngSettings.Resource.Sprite[ResID];
	//						int Lx=GPS.GetGPWidth(FileID,SpriteID);
	//						int Ly=GPS.GetGPHeight(FileID,SpriteID);
	//						int T=(GetTickCount()>>8)+OB->Index;
	//						Ly+=abs(2-(T&3))-1+10;
	//						GPS.ShowGP(v.x-Lx/2,v.y-Ly,FileID,SpriteID,OB->NNUM);
	//					}
	//					break;
	//				}
	//				Or=Or->NextOrder;
	//			}
	//		}
	//	}
	//}
};
DialogsSystem cext_VisualInterface::Credits;
DialogsSystem* GetShowAboutDS(){
	return &cext_VisualInterface::Credits;
}

bool cext_VisualInterface::OnMouseHandling(int mx,int my,bool& LeftPressed,bool& RightPressed, int MapCoordX,int MapCoordY,bool OverMiniMap){
	mouseX=mx;
	mouseY=my;
	mapX=MapCoordX;
	mapY=MapCoordY;
	return false;
};

void cext_VisualInterface::OnDrawOnMapOverAll(){
	// Credits
	extern bool vCreditsMode;
	if(vCreditsMode){
		if(Credits.DSS.GetAmount()==0){
			//Credits.ReadFromFile("dialogs\\mm\\Credits.DialogsSystem.xml");
		}
		Credits.ProcessDialogs();
		void ShowAbout();
		ShowAbout();
		SetCurPtr(15);
	}
	if(ShowAIPointInfo){
		void ShowPointStaticInfo(int x, int y, int mapX, int mapY);
		ShowPointStaticInfo(mouseX, mouseY, mapX, mapY);
	}
};

DLLEXPORT void ShowPage(char* Name);
CEXPORT bool isInCampaign();
void ACT(int x);

bool EndGameMessActive=false;
bool cext_VisualInterface::OnEndGameMessage(int NI,int VictStatus){
	
	extern _str vm_F1_VictoryCond;
	vm_F1_VictoryCond.Clear();
	
	GameExit=true;
	if(EndGameMessActive) return true;
	
	VictoryConditionRoot* VCond=MOptions.VictoryRule.Get();
	if(/*VCond&&*/NI==MyNation){ //&&cva_VI_EndGame::isStarted
		static DWORD ShowTime=0;
		if(cva_VI_EndGame::isStarted){
			cva_VI_EndGame::isStarted=false;
			if(VictStatus==2){			
				ACT(75);
			}else{
				ACT(76);
			}			
			GSets.CGame.ViewMask=0;//2+4;
			ShowTime=GetTickCount()+2000;
		}
		GameExit=false;
		if(ShowTime<GetTickCount()||(GetKeyState(VK_ESCAPE)&0x8000)||!isInCampaign()){
			GameExit=true;
			cva_VI_EndGame::isStarted=true;	
			
			cva_VI_EndGame::VictStatus=VictStatus;			
			
			EndGameMessActive=true;
			
			if(cva_VI_EndGame::UserMessage){
				for(int i=0;i<SCENINF.NPages;i++){
					if(!strcmp(cva_VI_EndGame::UserMessage,SCENINF.PageID[i])){				
						ShowPage(cva_VI_EndGame::UserMessage);
						cva_VI_EndGame::UserMessage=NULL;
						return true;
					}
				}
			}			
			if(v_MainMenu.StartDS("EndGameMessage",true)){
				cva_VI_EndGame::UserMessage=NULL;
			}
		}
		return true;
	}	
	return false;
}

DLLEXPORT int GetGlobalTime();
CEXPORT void AddPulseSquare(int x, int y);

void cext_VisualInterface::OnUnitBirth(OneObject* NewUnit){
	if(vHeroButtons.Add(NewUnit)){
	//	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//	if(NewUnit->NNUM==NI&&GetGlobalTime()>15000){
	//		char ccc[200];
	//		sprintf(ccc,BLDBLD,NewUnit->Ref.General->Message);
	//		AssignHint1(ccc,100);
	//		LastActionX=NewUnit->RealX>>4;
	//		LastActionY=NewUnit->RealY>>4;
	//	}
	};
};

bool cext_VisualInterface::OnUnitCapture(OneObject* CapturedUnit,OneObject* Capturer){
	if(CapturedUnit){
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		byte NMask=NATIONS[NI].NMask;
		if(CapturedUnit->newMons->PortBranch==2){
			if(CapturedUnit->NNUM==NI){
				ACT(96);
			}else
			//if(!(CapturedUnit->NMask&NMask)){
			if(Capturer&&Capturer->NNUM==NI){
				ACT(95);
			}
		}else
		if(CapturedUnit->NewBuilding){
			if(CapturedUnit->NNUM==NI){
				static char* txt=GetTextByID("#MyBldCaptured");
				_str t;
				t.print(txt,CapturedUnit->newMons->Message);
				//AssignHint1(t.pchar(),100);
			}else
			if(Capturer&&Capturer->NNUM==NI){
				static char* txt=GetTextByID("#EnemyBldCaptured");
				_str t;
				t.print(txt,CapturedUnit->newMons->Message);
				//AssignHint1(t.pchar(),100);
			}
		}
	}
	return true;
};
//
bool cext_VisualInterface::OnUnitDie(OneObject* Dead,OneObject* Killer){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(Dead&&Killer){
		if(Dead->newMons->PortBranch==2){			
			byte NMask=NATIONS[NI].NMask;
			if(Dead->NNUM==NI){
				ACT(94);
			}else
			if(!(Dead->NMask&NMask)){
				ACT(93);
			}
		}
	}
	return true;
};
//
int NextDamageAlert=0;
//
bool cext_VisualInterface::OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType){
	if(Damager&&GSets.DisableFriendlyFire&&NPlayers==1&&(DamagedUnit->NMask&Damager->NMask)&&CITY[Damager->NNUM].Difficulty<2){
		return false; //&&Damage>0
	}
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(!PlayGameMode&&DamagedUnit->NNUM==NI&&Damager){
		byte myNMask=NATIONS[NI].NMask;
		byte enNMask=NATIONS[Damager->NNUM].NMask;
		// enemy make damage
		if(!(myNMask&enNMask)){			
			vHeroButtons.checkDamage(DamagedUnit);
			if(NextDamageAlert<TrueTime&&DamagedUnit->BrigadeID!=0xFFFF){
				int myPortBranch=DamagedUnit->newMons->PortBranch;
				int enPortBranch=Damager->newMons->PortBranch;
				if(Damager->BrigadeID!=0xFFFF||enPortBranch==2){
					int x=DamagedUnit->RealX>>4;
					int y=DamagedUnit->RealY>>4;
					Vector3D point(x,y,GetHeight(x,y));
					if(!IMM->IsPointVisible(point)){
						NextDamageAlert=TrueTime+20000;
						AddPulseSquare(x,y);
						//
						int dist=Norma(DamagedUnit->RealX-Damager->RealX,DamagedUnit->RealY-Damager->RealY)>>4;						
						bool melee = (dist<80); // AttType
						//
						int act=61;						
						if(enPortBranch==2){
							// cannon
							act=63;
						}else{
							if(myPortBranch==0&&enPortBranch==1){
								// cavalery
								if(melee){
									act=64;
								}
							}else{
								if(melee){
									act=62;
								}
							}
						}
						ACT(act);
					}
				}
			}
		}
	}
	if(DamagedUnit->NNUM==NI&&DamagedUnit->Usage==CenterID&&Damager&&Damager->NNUM!=NI/*&&Damager->BrigadeID!=0xFFFF*/){
		VC_DestroyTownHalls* VC=dynamic_cast<VC_DestroyTownHalls*>(MOptions.VictoryRule.Get());
		if(VC){
			void fes_neTownHall(OneObject* TH);
			fes_neTownHall(DamagedUnit);
		}
	}
	if(DamagedUnit->NewBuilding){
		if(EngSettings.DamageOnBuildingMultiplicator>0){
			Damage*=EngSettings.DamageOnBuildingMultiplicator;
		}
		if(Damager && Damager->newMons->DamageOnBuildingMultiplicator>0){
			Damage*=Damager->newMons->DamageOnBuildingMultiplicator;
		}		
	}
    vui_SelPoint* SP=OIS.GetLastSelPoint();	
	if( SP && SP->OB==DamagedUnit ){
		if( Damager ){
			int AttType=Damager->NewState-1;
			int WPK=Damager->MoreCharacter->WeaponKind[AttType];
			if(Damager->ActiveAbility){
				Damager->ActiveAbility->ActiveAbilities.modifyWeaponKind(WPK,WPK,AttType);
			}
			SP->TimeLastActivationDefence[WPK]=TrueTime;
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
extern bool vIGPanelMode;
extern int LastActionX;
extern int LastActionY;
extern int LastActionTime;
//
bool vGameLoaing=false;
bool vFirstHelp=false;

bool vFastUpg=false;

void cext_VisualInterface::OnUnloading(){

	GSets.CGame.InterfaceNatID=-1;

	//v_GlobalHotKeys.Init();
	VoiceQueue.Clear();

	DialogsHistory.Income=true;
	DialogsHistory.Text.Clear();

	vmIGP.Unloading();

	extern bool InsideDialogState;
	InsideDialogState=true;

	vFastUpg=false;
	EndGameMessActive=false;
	//
	extern bool vTaskCII;
	vTaskCII=false;
	//
	extern int NextDamageAlert;
	NextDamageAlert=0;
	//
	void AlertOnUnloading();
	AlertOnUnloading();
	//
	GSets.CGame.SilenceMessageEvents=false;
	//
	void fes_Init();
	fes_Init();
	//
	vHeroButtons.SetVisible(true);
	void BrigPanelShowAll();
	BrigPanelShowAll();
	//	
	v_MainMenu.Cross=0;
	vGameLoaing=false;
	vIGPanelMode=false;
	vHeroButtons.Clear();
	//
	extern byte LockGrid;
	extern bool TransMode;
	extern bool HealthMode;
	//
	LockGrid=false;
	TransMode=false;
	HealthMode=true;
	// ai
	aiI.Unload();
	// market
	//cvi_Market* vMarket;
	///if(vMarket) vMarket->Load();
	vMarket=&vMarketInGame;
	//cvi_Market vMarketInGame;
	vMarketInGame.Load();
	//
	cva_VI_EndGame::isStarted=true;
	cva_VI_EndGame::VictStatus=0;
	cva_VI_EndGame::UserMessage=NULL;
	//
	LastActionX=0;
	LastActionY=0;
	LastActionTime=0;
	//
	cva_VI_EndGame::isStarted=true;	
	//
	vFirstHelp=false;
	//
	DialogsSystem* DS=&VI_Zone;
	int n=DS->DSS.GetAmount();
	for(int i=0;i<n;i++){
		GPPicture* GP=dynamic_cast<GPPicture*>(DS->DSS[i]);
		if(GP){
			GP->Visible=false;
		}
	}

	OIS.SelPoint.Clear();
};
bool cext_VisualInterface::OnMapUnLoading(){
	OIS.SelPoint.Clear();
	return true;
};
//
void SelectHeroOnStartMap(){
	byte MyNI=GSets.CGame.cgi_NatRefTBL[MyNation];
	for(byte NI=0;NI<7;NI++){
		OneObject* OB=vHeroButtons.GetObject(NI,0);
		if(OB){
			pSelected Sel(NI);
			pImSelected ImSel(NI);
			Sel.Clear();
			ImSel.Clear();
			pUnit U(OB);
			Sel += U;
			ImSel += U;
			if(NI==MyNI)
				SetScreenCenterToXY(OB->RealX>>4,OB->RealY>>4);
		}
	}
}
void cext_VisualInterface::OnInitAfterMapLoading(){
	
	for(int i=0;i<8;i++)
		if(XRESRC(i,MoneyID)==50000) 
			SetXRESRC(i,MoneyID,0);

	//// select Hero on start map
	//byte MyNI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//for(byte NI=0;NI<7;NI++){
	//	OneObject* OB=vHeroButtons.GetObject(NI,0);
	//	if(OB){
	//		pSelected Sel(NI);
	//		pImSelected ImSel(NI);
	//		Sel.Clear();
	//		ImSel.Clear();
	//		pUnit U(OB);
	//		Sel += U;
	//		ImSel += U;
	//		if(NI==MyNI)
	//			SetScreenCenterToXY(OB->RealX>>4,OB->RealY>>4);
	//	}
	//}

};
//
DialogsSystem cext_VisualInterface::PulseSquare;
CEXPORT void AddPulseSquare(int x, int y){
	static bool NoXmlFile=false;
	if(NoXmlFile) return;
	GPPicture* Pulse=NULL;
	DialogsSystem* DS=&cext_VisualInterface::PulseSquare;
	int n=DS->DSS.GetAmount();
	for(int i=0;i<n;i++){
		GPPicture* GP=dynamic_cast<GPPicture*>(DS->DSS[i]);
		if(GP){ //!GP->Visible
			int na=GP->v_Actions.GetAmount();
			for(int a=0;a<na;a++){
				vui_BasicEffect* BE=dynamic_cast<vui_BasicEffect*>(GP->v_Actions[a]);
				if(BE){
					if(BE->isFinished()){
						if(!Pulse){
							Pulse=GP;
							//break;
							//BE->StartEffect();			
						}
					}else{
						bool exist=Norma(GP->ID-x,GP->UserParam-y)<512;
						if(exist){
							return;
						}
					}
				}
			}
		}
	}
	if(!Pulse){
		Pulse=new GPPicture;
		if(Pulse->ReadFromFile("dialogs\\Alert.GPPicture.Dialogs.xml")){
			DS->DSS.Add(Pulse);
		}else{
			delete Pulse;
			NoXmlFile=true;
			Log.Warning("Not found file: dialogs\\Alert.GPPicture.Dialogs.xml");
			return;
		}
	}
	//LastActionX=x;
	//LastActionY=y;
	//LastActionTime=TrueTime;
	Pulse->ID=x;
	Pulse->UserParam=y;
	n=Pulse->v_Actions.GetAmount();
	for(int i=0;i<n;i++){
		vui_BasicEffect* BE=dynamic_cast<vui_BasicEffect*>(Pulse->v_Actions[i]);
		if(BE){
            BE->StartEffect();			
		}
	}	
}
extern int AnimTime;
class CPulseSquarePoint
{
public:
	CPulseSquarePoint(int xx, int yy)
	{
		x=xx;
		y=yy;
		birthTime=AnimTime;
	};
	int x;
	int y;
	int birthTime;
};
class CPulseSquarePointStorage
{
public:
	CPulseSquarePointStorage() {};
	~CPulseSquarePointStorage() 
	{
		int n=Points.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(Points[i])
			{
				delete (Points[i]);
				Points[i]=NULL;
			}
		}
		Points.Clear();
	};
	DynArray<CPulseSquarePoint*> Points;
	void AddPoint(int x, int y)
	{
		Points.Add( new CPulseSquarePoint(x, y));
	}
	bool CanAlarm(int x, int y, int DeadRadius, int actulAnimTime)
	{
		int n=Points.GetAmount();
		for(int i=0;i<n;i++)
		{
			CPulseSquarePoint* p=Points[n-1-i];
			if(p&&(AnimTime-p->birthTime)<actulAnimTime)
			{
				if(Norma(p->x-x,p->y-y)<DeadRadius)
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		return true;
	}
};
CPulseSquarePointStorage PulseSquarePointStorage;
int ActCommand=0;
bool AddPulseSquare(int x, int y, int deadRadius, int actulAnimTime)
{
	if(PulseSquarePointStorage.CanAlarm(x, y, deadRadius, actulAnimTime))
	{
		PulseSquarePointStorage.AddPoint(x, y);
		AddPulseSquare(x, y);
		void ACT(int x);
		//ACT(ActCommand);
		return true;
	}
	return false;
}
//
DialogsSystem cext_VisualInterface::VI_Zone;
CEXPORT void Add_VI_Zone(int x, int y, DWORD color){
	static bool NoXmlFile=false;
	if(NoXmlFile) return;
	GPPicture* Pulse=NULL;
	DialogsSystem* DS=&cext_VisualInterface::VI_Zone;
	int n=DS->DSS.GetAmount();
	for(int i=0;i<n&&!Pulse;i++){
		GPPicture* GP=dynamic_cast<GPPicture*>(DS->DSS[i]);
		if(GP){ //!GP->Visible
			bool find=Norma(GP->ID-x,GP->UserParam-y)<256;
			int na=GP->v_Actions.GetAmount();
			for(int a=0;a<na;a++){
				vui_BasicEffect* BE=dynamic_cast<vui_BasicEffect*>(GP->v_Actions[a]);
				if(BE){
					if(BE->isFinished()){
						if(!Pulse){
							Pulse=GP;
							find=false;
							break;
							//BE->StartEffect();			
						}
					}
				}
			}
			if(find){
				GP->Diffuse=color;
				GP->Visible=true;
				return;
			}
		}
	}
	if(!Pulse){
		Pulse=new GPPicture;
		if(Pulse->ReadFromFile("dialogs\\VI_Zone.GPPicture.Dialogs.xml")){
			DS->DSS.Add(Pulse);
		}else{
			delete Pulse;
			NoXmlFile=true;
			Log.Warning("Not found file: dialogs\\VI_Zone.GPPicture.Dialogs.xml");
			return;
		}
	}
	Pulse->ID=x;
	Pulse->UserParam=y;
	Pulse->Diffuse=color;
	Pulse->Visible=true;
	n=Pulse->v_Actions.GetAmount();
	for(int i=0;i<n;i++){
		vui_BasicEffect* BE=dynamic_cast<vui_BasicEffect*>(Pulse->v_Actions[i]);
		if(BE){
            BE->StartEffect();			
		}
	}	
}
//
int BrigMiniBoxX=-1;
int BrigMiniBoxY;
void ShowBrigMiniBox(int x, int y){
	BrigMiniBoxX=x;
	BrigMiniBoxY=y;
};
//
CEXPORT int GetXOnMiniMap(int x,int y);
CEXPORT int GetYOnMiniMap(int x,int y);
void cext_VisualInterface::OnDrawOnMiniMap(int x,int y,int Lx,int Ly){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP||BrigMiniBoxX!=-1){
		if(SP&&BrigMiniBoxX==-1){
			BrigMiniBoxX=SP->rX>>4;
			BrigMiniBoxY=SP->rY>>4;
		}
		static int f=GPS.PreLoadGPImage("interf3\\minimap");
		int spr=27;
		int px=GetXOnMiniMap(BrigMiniBoxX,BrigMiniBoxY)-GPS.GetGPWidth(f,spr)/2;
		int py=GetYOnMiniMap(BrigMiniBoxX,BrigMiniBoxY)-GPS.GetGPHeight(f,spr)/2;
		GPS.ShowGP(px,py,f,spr,MyNation);
		BrigMiniBoxX=-1;
	}
	//
	DialogsSystem* DS=&cext_VisualInterface::PulseSquare;
	int n=DS->DSS.GetAmount();
	for(int i=0;i<n;i++){
		GPPicture* GP=dynamic_cast<GPPicture*>(DS->DSS[i]);
		if(GP){ //!GP->Visible
			int px=GetXOnMiniMap(GP->ID,GP->UserParam);
			int py=GetYOnMiniMap(GP->ID,GP->UserParam);
			int w=GP->GetWidth();
			int h=GP->GetHeight();
			//GP->Setx((px>>6)-w/2+x);
			//GP->Sety((py>>6)-h/2+y);
			GP->Setx(px-w/2);
			GP->Sety(py-h/2);
		}
	}
	DS->ProcessDialogs();
	//
	DS=&cext_VisualInterface::VI_Zone;
	n=DS->DSS.GetAmount();
	for(int i=0;i<n;i++){
		GPPicture* GP=dynamic_cast<GPPicture*>(DS->DSS[i]);
		if(GP){ //!GP->Visible
			int px=GetXOnMiniMap(GP->ID,GP->UserParam);
			int py=GetYOnMiniMap(GP->ID,GP->UserParam);
			int w=GP->GetWidth();
			int h=GP->GetHeight();
			//GP->Setx((px>>6)-w/2+x);
			//GP->Sety((py>>6)-h/2+y);
			GP->Setx(px-w/2);
			GP->Sety(py-h/2);
		}
	}
	DS->ProcessDialogs();
}
void cext_VisualInterface::OnClassRegistration(){

	REG_CLASS(cDialogsHistory);

	REG_CLASS(oi_UnitInfo);

	REG_CLASS(OISelection);
	OIS.GetObjectGlobalID();
	OIS.Name="ObjectInterfaceSelection";

	REG_CLASS(cvi_Options);
	REG_CLASS(cvi_Info);

	REG_CLASS(spCond_SPClass);
	REG_CLASS(spCond_SPClassSet);

	REG_CLASS(OneGPFile);
	REG_CLASS(NatGPArray);

	CookBook.SafeReadFromFile("dialogs\\CookBook.xml");
	NationalGP.SafeReadFromFile("dialogs\\nationalgp.xml");

	//
	REG_CLASS(cvi_GPFile);
	REG_CLASS(cvi_NatPics);
	REG_CLASS(cvi_Icon);
	//
	void vNewProfInit();
	vNewProfInit();
	//
	REG_CLASS(cvi_FontStyle);	
	REG_CLASS(cvi_Point);
	REG_CLASS(cvi_Missions);	
	REG_CLASS(ClonesArray<cvi_Missions>);
	//
	REG_CLASS(cvi_MissionFrame);
	REG_CLASS(cvi_mfGame);
	//REG_CLASS(cvi_mfStatistic);
	REG_CLASS(cvi_mfMovie);
	REG_CLASS(cvi_Fresco);
	REG_CLASS(cvi_mfFrescos);
	REG_CLASS(cvi_ChangeDifficulty);
	REG_CLASS(cvi_Mission);
	REG_CLASS(cvi_Campaign);
	REG_CLASS(ClassRef<cvi_mfMovie>);
	// Init
	vmCampaigns.ReadFromFile(vmCampaignXML);
	vmPlayerData.Read();	
	extern _str PlayerName;
	PlayerName=vmSinglePlayerData.PlayerName;
	// GameSettings
	GSets.gpRally=GPS.PreLoadGPImage("Interf3\\exitpoint");
	GSets.gpMove=GPS.PreLoadGPImage("Interf3\\moveon");
	//
	REG_CLASS(IncomingAbility);
	REG_CLASS(cvi_Hero);
	REG_CLASS(cvi_HeroButtons);
	//
	v_GlobalHotKeys.ReadFromFile(v_GlobalHotKeysXML);

	//v_ISys.ReadFromFile(v_ISysXML);
	v_FontStyle.ReadFromFile(v_FontStyleXML);
	if(v_FontStyle.Font!=0){
		v_FontStyle.SetFonts.EvaluateFunction();
	}
	//
	EW2_Missions.ReadFromFile(EW2_MissionsXML);
	//
	void ArcadeModeDDLoad();
	ArcadeModeDDLoad();
	// Credits
	Credits.ReadFromFile("dialogs\\mm\\Credits.DialogsSystem.xml");
	//
	int vgf_UI_Process();
	vgf_UI_Process();
}
void cext_VisualInterface::ProcessingGame(){

	void VoiceQueueProcess();
	VoiceQueueProcess();

	vmIGP.Process();

	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_units){
		if(SP->Inf.Units.HoldPosition)
			OIS.HoldPosition=1;
		else
			OIS.HoldPosition=0;
	}else{
		OIS.HoldPosition=0;
	}

	//
	void AlertBrigProcess();
	AlertBrigProcess();
	//
	extern int tmtmt;
	if(tmtmt<100)v_MainMenu.Cross=0;
	//
	if(EngSettings.vInterf.StartHelp&&!vFirstHelp){
		vFirstHelp=true;
		v_MainMenu.StartDS("F1game",true);
	}
	// ai
	aiI.Process();
	return;

	// check ImSellected accordance
	//if(GetRND(20)!=16) return;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Sdoxlo){
			for(byte ni=0;ni<8;ni++){
				if(OB->ImSelected&(1<<ni)){
					int n=ImNSL[ni];
					word* id=ImSelm[ni];
					word* sn=ImSerN[ni];
					bool exist=false;
					for(int j=0;j<n;j++){
						if(id[j]==OB->Index&&sn[j]==OB->Serial){
							exist=true;
							break;
						}
					}
					if(n&&!exist){
						Log.Error("Wrong ImSelected=%d in object id=%d, ni=%d",OB->ImSelected,OB->Index,OB->NNUM);
					}
				}
			}

		}
	}
	
};
//
void cext_VisualInterface::OnBrigadeCreated(Brigade* BR){
	Nation* NT=BR->CT->Nat;
	NT->NBrProduced[BR->MembID]++;
	//
	BR->M=0;
	BR->HideTime=0;
	BR->mmActive=0;
	//
	BR->Alert.Init=false;
};
void cext_VisualInterface::OnBrigadeKilled(Brigade* BR,byte KillerNation){
	if(BR->NMemb>40){
		byte NI=BR->CT->NI;
		//byte NMask=NATIONS[NI].NMask;
		byte MyNI=GSets.CGame.cgi_NatRefTBL[MyNation];
		//byte MyNMask=NATIONS[MyNI].NMask;
		if(NI!=7){
			if((NI==MyNI||KillerNation==MyNI)){
				bool CheckLastDeffenderEvent(Brigade* BR);
				int x,y;
				if(CheckLastDeffenderEvent(BR)&&BR->GetCenter(&x,&y)){
					if(NI==MyNI){
						ACT(121);
						AddPulseSquare(x,y);
					}else{
						ACT(122);
						AddPulseSquare(x,y);
					}
				}else{
					void fes_neErasedBrigade(byte BrigadeNI);
					fes_neErasedBrigade(NI);
				}
			}
		}
	}
}

DLLEXPORT void ShowVictory(char* Message, byte NI=0xFF);
void EraseObject(OneObject* OB);

bool cext_VisualInterface::OnCheatEntering(const char* Cheat){
	char Com[256];
	if(!strcmp(Cheat,"VICTORY")){
		if(NPlayers<2)
			ShowVictory(NULL);
	}else
	if(strstr(Cheat,"DelSpr")){
		char SprName[128];
		if(sscanf(Cheat,"%s%s",Com,SprName)>1){
			for(int i=0;i<MaxSprt;i++){
				OneSprite* OS=&Sprites[i];
				if( OS->Enabled && OS->OC &&OS->OC->Name==SprName ){
					OS->Enabled=false;
				}
			}
			return true;
		}
	}
	if(!strcmp(Cheat,"ErasePeasants")){
        void EraseAllNetralPeasantsAndPolicemen();
		EraseAllNetralPeasantsAndPolicemen();
		AssignHint1("Erasing peasants and policemen complete",100);
		return true;
	}
	if(!strcmp(Cheat,"EraseHiddenBuildings")){
		int n=0;
		itr_GetAllUnits.Create();
		while(OneObject* OB=itr_GetAllUnits.Next()){
			if(OB&&OB->NewBuilding&&OB->Sdoxlo&&OB->Hidden){
				OB->Hidden=false;
				EraseObject(OB);
				n++;
			}
		}
		sprintf(Com,"%d building(s) erased",n);
		AssignHint1(Com,100);
		return true;
	}
	if(strstr(Cheat,"Expaplease")){
		int Expa=0;
		if(sscanf(Cheat,"%s%d",Com,&Expa)>1){
			void GiveExpa(int Expa);
			GiveExpa(Expa);
		}
		return true;;
	}else
	if(strstr(Cheat,"Ineedmoney")){
		int M=0;
		if(sscanf(Cheat,"%s%d",Com,&M)>1){
			AddXRESRC(MyNation,MoneyID,M);
		}
		return true;
	}else
	if(!strcmp(Cheat,"DeleteDrop")){
		itr_GetSelected.Create(MyNation);
		while(OneObject* OB=itr_GetSelected.Next()){
			bool DeleteItemDrops(OneObject* OB);
			DeleteItemDrops(OB);
		};
	}else
	if(!strcmp(Cheat,"SetDefault")){
		itr_GetSelected.Create(MyNation);
		while(OneObject* OB=itr_GetSelected.Next()){
			bool SetDefault(OneObject* OB);
			SetDefault(OB);
		};
	}else	if(!strcmp(Cheat,"tooslow")){
		vFastUpg=true;
	}else	if(!strcmp(Cheat,"toofast")){
		vFastUpg=false;
	}else	if(!strcmp(Cheat,"SelModify")){
		pSelected Sel(MyNation);
		pImSelected ImSel(MyNation);
		Sel.Clear();
		ImSel.Clear();
		itr_GetAllUnits.Create();
		while(OneObject* OB=itr_GetAllUnits.Next()){
			if( OB->newMons->CharacterWasModified && OB->NNUM==MyNation && !OB->NewBuilding ){
				pUnit U(OB);
				Sel+=U;
				ImSel+=U;
			}
		}
	}else	if(!strcmp(Cheat,"ShowAIPointInfo")){
		ShowAIPointInfo=!ShowAIPointInfo;
		AssignHint1("ShowAIPointInfo",10);
	}
	return false;
};

bool cext_VisualInterface::OnMapSaving(xmlQuote& xml){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Sdoxlo){
			itr_AA.Create(OB);
			while(ActiveUnitAbility* AA=itr_AA.Next()){
				HeroVariableStorage* HVS=dynamic_cast<HeroVariableStorage*>(AA);
				if(HVS){
					xmlQuote* xExp = new_xmlQuote("HeroExpa");
					xExp->AddSubQuote("UniqID",OB->UniqID);
					xExp->AddSubQuote("Level",HVS->Level);
					xExp->AddSubQuote("Expa",HVS->Experience);
					xml.AddSubQuote(xExp);
					break;
				}
			}
		}
	}
	return true;
};
bool cext_VisualInterface::OnMapLoading(xmlQuote& xml){
	int n=xml.GetNSubQuotes();
	for(int i=0;i<n;i++){
		xmlQuote* xExp = xml.GetSubQuote(i);
		if(xExp){

			DWORD UniqID=xExp->GetSubQuote(0)->Get_int();
			int Level=xExp->GetSubQuote(1)->Get_int();
			int Expa=xExp->GetSubQuote(2)->Get_int();

			for(int i=0;i<MAXOBJECT;i++){
				OneObject* OB=Group[i];
				if (OB && OB->UniqID==UniqID) {
					itr_AA.Create(OB);
					while(ActiveUnitAbility* AA=itr_AA.Next()){
						HeroVariableStorage* HVS=dynamic_cast<HeroVariableStorage*>(AA);
						if(HVS){
							HVS->Level=Level;
							HVS->Experience=Expa;
							break;
						}
					}
					break;
				}
			}
		}
	}
	return true;
};

extern int CurEW2miss;
extern _str vm_F1_VictoryCond;
CEXPORT char* GetIDByText(char* Text);

bool cext_VisualInterface::OnGameSaving(xmlQuote& xml){	
	//
	xmlQuote* xmlHB = new_xmlQuote("vHeroButtons");
	vHeroButtons.Save(*xmlHB,&vHeroButtons);
	xml.AddSubQuote(xmlHB);
	//
	xmlQuote* xmlCI = new_xmlQuote("Campaign Info");
	xmlCI->AddSubQuote("GameMode",vGameMode);
	if(vGameMode==gmCamp){
		xmlCI->AddSubQuote("CampaignID",vmCampID);
		xmlCI->AddSubQuote("MissionID",cva_Camp_StartMission::MissionID);
		if(vmCamp&&vmCamp->curMission>=0&&vmCamp->curMission<vmCamp->Missions.GetAmount()){
			xmlCI->AddSubQuote("curFrame",vmCamp->Missions[vmCamp->curMission]->curFrame);
		}
	}
	xml.AddSubQuote(xmlCI);
	//
	xmlQuote* xmlHI = new_xmlQuote("spdHeroInfo");
	if(vGameMode==gmCamp){		
		if(vmCamp&&vmCamp->curMission>=0&&vmCamp->curMission<vmCamp->Missions.GetAmount()){
			SinglePlayerData_HeroesInfoList* HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
			vHeroButtons.Save(*xmlHI,HI);
		}		
	}
	xml.AddSubQuote(xmlHI);
	//
	xmlQuote* xmlEW2M = new_xmlQuote("curEW2mission");
	xmlEW2M->AddSubQuote("CurEW2miss",CurEW2miss);
	xmlEW2M->AddSubQuote("SilenceMessageEvents",GSets.CGame.SilenceMessageEvents);
	xml.AddSubQuote(xmlEW2M);
	//
	xmlQuote* xmlEW2F1 = new_xmlQuote("curEW2help");	
	if(!vm_F1_VictoryCond.isClear()){
		xmlEW2F1->AddSubQuote("vm_F1_VictoryCond",GetIDByText(vm_F1_VictoryCond.pchar()));
	}
	xml.AddSubQuote(xmlEW2F1);

	xmlQuote* xmlDH = new_xmlQuote("DialogsHistory");
	DialogsHistory.Save(*xmlDH,&DialogsHistory);
	xml.AddSubQuote(xmlDH);
	return true;
};
bool cext_VisualInterface::OnGameLoading(xmlQuote& xml){
	if(vmCamp && vmCamp->curMission>=0 && vmCamp->curMission<vmCamp->Missions.GetAmount()){
		cvi_Mission* cM=vmCamp->Missions[vmCamp->curMission];
		cM->curFrame=cM->Scene.GetAmount()-1;
	}
	vGameLoaing=true;
	vmCamp=NULL;
	ErrorPager Error(1);
	//
	xmlQuote* xmlHB=xml.GetSubQuote(0);

	if (xmlHB!=NULL){
		vHeroButtons.reset_class(&vHeroButtons);
		vHeroButtons.Load(*xmlHB,&vHeroButtons,&Error);	
	}
	if (xml.GetSubQuote(1)!=NULL){
		//vValuesMap()->Load(*(xml.GetSubQuote(0)),vValuesMap(),&Error);
		xmlQuote* xmlCI = xml.GetSubQuote(1);
		if(xmlCI->GetNSubQuotes()>0){
			vGameMode=(veGameMode)xmlCI->GetSubQuote(0)->Get_int();
			if(vGameMode==gmCamp&&xmlCI->GetNSubQuotes()>2){
				v_MainMenu.ModalDesk="Campaign";
				vmCampID=xmlCI->GetSubQuote(1)->Get_int();
				vmCamp=vmCampaigns[vmCampID];
				vmCamp->curMission=xmlCI->GetSubQuote(2)->Get_int();
				cva_Camp_StartMission::MissionID=vmCamp->curMission;
				if(vmCamp && vmCamp->curMission>=0 && vmCamp->curMission<vmCamp->Missions.GetAmount()){
					vmCamp->Missions[vmCamp->curMission]->curFrame=xmlCI->GetSubQuote(3)->Get_int();
				}
			}
		}
	}
	if (xml.GetSubQuote(2)!=NULL && vmCamp){
		if( vmCampID < vmSinglePlayerData.Heri.GetAmount() &&
			vmCamp->curMission < vmSinglePlayerData.Heri[vmCampID]->CampaignMissions.GetAmount() ){
			SinglePlayerData_HeroesInfoList* HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
			if(HI){
				HI->reset_class(HI);
				HI->Load(*(xml.GetSubQuote(2)),HI,&Error);
			}
		}
	}
	if (xml.GetSubQuote(3)!=NULL){
		xmlQuote* xmlEW2M=xml.GetSubQuote(3);
		if(xmlEW2M->GetNSubQuotes()>0){
			CurEW2miss=xmlEW2M->GetSubQuote(0)->Get_int();
		}
		if(xmlEW2M->GetNSubQuotes()>1){
			GSets.CGame.SilenceMessageEvents=xmlEW2M->GetSubQuote(1)->Get_int();
		}
	}
	vm_F1_VictoryCond.Clear();
	if (xml.GetSubQuote(4)!=NULL){
		xmlQuote* xmlEW2F1=xml.GetSubQuote(4);
		if(xmlEW2F1->GetNSubQuotes()>0){
			char* id=(char*)xmlEW2F1->GetSubQuote(0)->Get_string();
			vm_F1_VictoryCond=GetTextByID(id);
		}
	}
	
	xmlQuote* xmlDH=xml.GetSubQuote("DialogsHistory");
	if(xmlDH){
		DialogsHistory.reset_class(&DialogsHistory);
		DialogsHistory.Load(*xmlDH,&DialogsHistory,&Error);
	}
	
	return true;
};
/////////////////////////////////////////////////////////////////////////////////////////////////
// actions
/////////////////////////////////////////////////////////////////////////////////////////////////
bool cva_VI_EndGame::isStarted=true;
int cva_VI_EndGame::VictStatus=0;
char* cva_VI_EndGame::UserMessage=NULL;
//
void cva_VI_EndGame::SetFrameState(SimpleDialog* SD){	
	// hack begin
	if( v_MainMenu.Exit )
		return;
	// hack end
	if( isStarted ){
		EndGameMessActive=true;
		isStarted=false;
		TextButton* dT=dTitle.Get();		
		TextButton* dM=dMessage.Get();
		VitButton* dB=dButton.Get();
		if(VictStatus==2){
			// victory
			if(dT) dT->SetMessage(VicTitle);
			if(dM){
				if(UserMessage) dM->SetMessage(UserMessage);
					else dM->SetMessage(VicMessage);
			}
			if(dB) dB->SetMessage(VicButton);

			vmIGP.EndGameTitle=VicTitle;
			if(UserMessage) vmIGP.EndGameMessage=UserMessage;
				else vmIGP.EndGameMessage=VicMessage;
			vmIGP.EndGamePicSprite=VicSprite;

		}else{
			// defeat
			if(dT) dT->SetMessage(DefTitle);
			if(dM){
				if(UserMessage) dM->SetMessage(UserMessage);
					else dM->SetMessage(DefMessage);
			}
			if(dB) dB->Message=DefButton;

			vmIGP.EndGameTitle=DefTitle;
			if(UserMessage) vmIGP.EndGameMessage=UserMessage;
			else vmIGP.EndGameMessage=DefMessage;
			vmIGP.EndGamePicSprite=DefSprite;

		}

		vmIGP.EndGamePicFile=gpFile;
		
		GPPicture* dP=dPicture.Get();
		if(dP){
			dP->FileID=gpFile;
			if(VictStatus==2){
				dP->SetSpriteID(VicSprite);
			}else{
				dP->SetSpriteID(DefSprite);
			}
		}
	}
}
/// ai Base ///
aiInfo aiI;
CEXPORT bool CorrectPosition(byte NI, int& x, int& y){
	return aiI.Base[NI].CorrectPosition(x,y);
}
//
void aiBase::Unload(){
	Enable=false;
	cX=-1;
	cUnitX=-1;
	cR=3000;
	rDefenders=2000;
}
bool aiBase::CorrectPosition(int& x, int& y){	
	addrand(655);
	if(eR<cR&&cX!=-1&&eX!=-1){
		// enemy on the base
		if(Norma(cX-x,cY-y)<cR||Norma(eX-x,eY-y)<rDefenders){
			// correct position
			/*
			if(cUnitX!=-1){
				x=cX;
				y=cY;
			}
			*/
			x=eX;
			y=eY;
			addrand(432);
			return true;
		}
	}
	addrand(652);
	return false;
};
void aiBase::Process(){
	int cn=0;
	int cx=0;
	int cy=0;
	int cr=10000;
	eX=-1;
	eY=0;
	eR=10000;
	//
	addrand(325);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject*OB=Group[i];
		if(OB&&!OB->Sdoxlo){
			if(OB->NNUM==NI){
				// my
				void TakeResLink(OneObject* OBJ);
				if(OB->LocalOrder&&OB->LocalOrder->DoLink==&TakeResLink){
					cn++;
					int x=OB->RealX>>4;
					int y=OB->RealY>>4;
					cx+=x;
					cy+=y;
					if(cX!=-1){
						int r=Norma(x-cX,y-cY);
						if(cr>r){
							cr=r;
							cUnitX=x;
							cUnitY=y;
						}
					}
				}
			}else
			if(OB->NMask&NATIONS[NI].NMask){
				// ally			
			}else{
				// enemy
				int x=OB->RealX>>4;
				int y=OB->RealY>>4;
				if(cX!=-1){
					int r=Norma(x-cX,y-cY);
					if(eR>r){
						eR=r;
						eX=x;
						eY=y;
					}
				}
			}
		}
	}
	//
	if(cn){
		cX=cx/cn;
		cY=cy/cn;
	}	
};
void aiInfo::Process(){
	int T=GetGlobalTime();
	if(T>RefreshTime){
		addrand(765);
		RefreshTime=T+MAXOBJECT+100;		
		Base[CurID].Process();
		CurID=(CurID+1)%8;
	}
};
void aiInfo::Unload(){
	RefreshTime=8000;
	CurID=0;
	for(int i=0;i<8;i++){
		Base[i].Unload();
		Base[i].NI=i;
	}
};
/// ai Base ///
