#include "stdheader.h"
#include ".\cvi_MainMenu.h"
#include ".\cvi_campaign.h"
#include ".\cvi_singleplayerdata.h"
#include ".\cvi_InGamePanel.h"
#include ".\cvi_HeroButtons.h"
#include "BE_BaseClasses.h"
#include "UnitsInterface.h"
#include "Camera\Navigator.h"
//////////////////////////////////////////////////////////////////////////
cvi_InGamePanel vmIGP;
//const char* vmSinglePlayerDataXML="dialogs\\SinglePlayerData.xml";
//////////////////////////////////////////////////////////////////////////
cvi_InGamePanel::cvi_InGamePanel(){
	Start=0;
	Open=0;
	Show=0;
	Close=0;
	StartNextCampMission=false;
}
//
bool igpStart=false;
bool vIGPanelMode=false;
//
void cvi_InGamePanel::StartFrame(char* Message, int OpenTime, int ShowTime, int CloseTime){	
	if(vGameMode == gmCamp&&vmCamp){
		SinglePlayerData_HeroesInfoList* HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
		if(HI){
			if(vmCamp->curMission>0){
				SinglePlayerData_HeroesInfoList* mHI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission-1];
				if(mHI){
					HI->FreeExp=mHI->FreeExp;
				}
			}else{
				HI->FreeExp=0;
			}
			//if(HI->FreeExp>0) v_MainMenu.StartDS("HeroesUpgrades", true);
		}
		Start=GetTickCount()+2000;
	}else{
		Start=GetTickCount();
	}
	Mess=Message;	
	Open=OpenTime;
	Show=ShowTime;
	Close=CloseTime;
	igpStart=true;
	vIGPanelMode=true;;
};
//////////////////////////////////////////////////////////////////////////
void cva_IGP_Frame::SetFrameState(SimpleDialog* SD){
	//vIGPanelMode=false;
	int t=GetTickCount()-vmIGP.Start;
	SD->Visible=(t<vmIGP.Open+vmIGP.Show+vmIGP.Close);
	if(SD->Visible){
		TextButton* tbMess=Mess.Get();
		if(tbMess){
			tbMess->SetMessage(vmIGP.Mess);
			int left,top,right,bottom;
			SD->GetMargin(left,top,right,bottom);
			int w=tbMess->GetWidth();
			//int h=tbMess->GetHeight();
			SD->SetWidth(w+left+right+60);
			//SD->SetHeight(h+top+bottom+15);
		}
		SD->DeepColor=true;
		byte Alpha=0xFF;
		if(t<vmIGP.Open){
			Alpha=0xFF*t/vmIGP.Open;
		}else
		if(t>vmIGP.Open+vmIGP.Show){
			t=vmIGP.Open+vmIGP.Show+vmIGP.Close-t;
			Alpha=0xFF*t/vmIGP.Close;
		}
		if(t<0){
			Alpha=0x00;			
		}else{
			vIGPanelMode=true;
		}
		SD->Diffuse=0x00FFFFFF+0x1000000*Alpha;
	}else{
		vIGPanelMode=false;
		if(t>vmIGP.Open+vmIGP.Show+vmIGP.Close){
			byte NI = GSets.CGame.cgi_NatRefTBL[MyNation]; // Nation Index
			extern cvi_HeroButtons vHeroButtons;
			OneObject *oo = vHeroButtons.GetObject(NI, 0);
			if(oo)
			{
				vIGPanelMode=false;
				if(igpStart&&vGameMode==gmCamp&&vmCamp){
					igpStart=false;
					if( vmCamp->curMission < vmSinglePlayerData.Heri[vmCampID]->CampaignMissions.GetAmount() &&
						vmCampID < vmSinglePlayerData.Heri.GetAmount() ){
						SinglePlayerData_HeroesInfoList* HI=vmSinglePlayerData.Heri[vmCampID]->CampaignMissions[vmCamp->curMission];
						if(HI&&HI->FreeExp>0) v_MainMenu.StartDS("HeroesUpgrades", true);

					}
				}
			}
		}		
	}
}
// vCreditsMode
extern bool vCreditsMode;
void cva_IGP_Credits::SetFrameState(SimpleDialog* SD){
	SD->Visible=vCreditsMode;
}
void cvi_InGamePanel::Init(){
	Name="InGamePanel";
	GetObjectGlobalID();
	int t=GetTickCount();
	EngSettings.ReadFromFile("ENGINESETTINGS.xml");
	LoadingRace=_str(rand()%(EngSettings.vInterf.NumOfRaceStyles));
	FirstLoadingType=(t>>3)&1;
};

void cvi_InGamePanel::Unloading(){
	HeroLive1=false;
	HeroLive2=false;
	HeroLive3=false;
	FirstTimeShowExIcon=0;
};

HeroVariableStorage* DetectHero(OneObject* OB);
void GetDieOrStunCoolDown(OneObject* OB, int& DieCD, int& StunCD);

void cvi_InGamePanel::Process(){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	OneObject* OB=vHeroButtons.GetObject(NI,0);
	if(OB){
		HeroVariableStorage* HVS=DetectHero(OB);
		if(HVS){
			HeroLive1 = HVS->Lives>0;
			HeroLive2 = HVS->Lives>1;
			HeroLive3 = HVS->Lives>2;
		}
	}
	//Opt.Process();

	//HeroLifeAlert=false;
	HeroLifeAlert="";
	OneObject* HOB=vHeroButtons.GetObject(NI,0);
	if(HOB){
		int DieCD,StunCD;
		GetDieOrStunCoolDown(HOB,DieCD,StunCD);
		int dL=700*HOB->Life/HOB->MaxLife;
		if( dL<255+50 || DieCD!=0 ){
			DWORD Color=0x88000088;
			if(DieCD==0){
				dL-=50;
				if(dL<0)
					dL=0;
				Color=((255-dL)<<24)+0xFF0000;
			}
			//HeroLifeAlert=true;
			//HeroLifeAlertColor=((255-dL)<<24)+0xFF0000;
			HeroLifeAlert.print("{tex Interf3\\textures\\hero_alert_03.dds 0 0 %d %d  0 0 255 255  %x %x %x %x}",RealLx,RealLy,Color,Color,Color,Color);
		}
	}

};

extern int KillSpeedVariation;

void cvi_Options::Init(){
	LastPreSet=PreSet;
	Shadows=(GSets.ShadowQuality==0)?0:1;
	Shaders=(GSets.SVOpt.HQShaders)?0:1;
	Bump=(GSets.SVOpt.DrawBumpOnUnits)?0:1;
	Reflection=GSets.SVOpt.DrawReflectedTerrain;
	Antialiasing=GSets.SVOpt.Antialiasing.Enabled;
	ColorBits=(GSets.SVOpt.TrueColor.Enabled)?0:1;
	//SpeedVariation=GSets.SVOpt.GameSpeedAutoAdjustment;
	//SpeedVariation=(KillSpeedVariation==0)?true:false;
	SpeedVariation=(KillSpeedVariation==0)?true:false;
	AutoSavePeriod=GSets.AutoSavePeriod;
};
//Preset-Ultra High(High Shaders, High Shadows, Reflection, High UnitBumpQuality,Antialising)
//Preset-High(High Shaders, High Shadows, No Reflection, High UnitBumpQuality)
//Preset-Medium(Default)(High Shaders, Low Shadows, No Reflection, low UnitBumpQuality)
//Preset-Low(Low Shaders, Low Shadows, No Reflection, 16bit color, low UnitBumpQuality)
//Preset-Custom (автоматически сменяеться на него при изменении какой либо настройки графики кроме Разрешения)
void cvi_Options::Process(){
	if( LastPreSet==-1 || LastPreSet!=PreSet ){
		// пользователь изменил preset
		switch(PreSet){
			case 0:
				Shaders=0; Shadows=0; Reflection=1; Antialiasing=1; ColorBits=0; Bump=0;
				break;
			case 1:
				Shaders=0; Shadows=0; Reflection=0; Antialiasing=0; ColorBits=0; Bump=0;
				break;
			case 2:
				Shaders=0; Shadows=1; Reflection=0; Antialiasing=0; ColorBits=0; Bump=1;
				break;
			case 3:
				Shaders=1; Shadows=1; Reflection=0; Antialiasing=0; ColorBits=1; Bump=1;
				break;
			//default:
			//	CB->CurLine=4;
			//	break;
		}
	}else{
		int cur=LastPreSet;
		if( Shaders==0 && Shadows==0 && Reflection==1 && Antialiasing==1 && ColorBits==0 && Bump==0 ){
			cur=0;
		}else
		if( Shaders==0 && Shadows==0 && Reflection==0 && Antialiasing==0 && ColorBits==0 && Bump==0 ){
			cur=1;
		}else
		if( Shaders==0 && Shadows==1 && Reflection==0 && Antialiasing==0 && ColorBits==0 && Bump==1 ){
			cur=2;
		}else
		if( Shaders==1 && Shadows==1 && Reflection==0 && Antialiasing==0 && ColorBits==1 && Bump==1 ){
			cur=3;
		}else{
			cur=4;
		}
		// пользователь изменил опцию влияющую на preset			
		if( LastPreSet!=cur ){
			PreSet=cur;
		}
	}
	LastPreSet=PreSet;
};
void cvi_Options::Accept(){
	GSets.ShadowQuality=(Shadows==0)?0:2;
	bool ChangeAA=GSets.SVOpt.Antialiasing.Enabled!=Antialiasing;
	bool ChangeCD=GSets.SVOpt.TrueColor.Enabled!=bool(1-ColorBits);
	
	GSets.SVOpt.WaterQuality=1-Shaders;
	GSets.SVOpt.DrawBumpOnLandcape=1-Shaders;
	GSets.SVOpt.DrawSpritesIn2Pass=1-Shaders;
	GSets.SVOpt.HQShaders=1-Shaders;

	GSets.SVOpt.DrawReflectedTerrain=Reflection;
	GSets.SVOpt.DrawBumpOnUnits=1-Bump;
	GSets.SVOpt.Antialiasing.Enabled=Antialiasing;
	GSets.SVOpt.TrueColor.Enabled=1-ColorBits;	

	//GSets.SVOpt.GameSpeedAutoAdjustment=SpeedVariation;
	KillSpeedVariation=(SpeedVariation)?0:256;
	GSets.AutoSavePeriod=AutoSavePeriod;

	if(ChangeAA)GSets.SVOpt.Antialiasing.Update();
	if(ChangeCD)GSets.SVOpt.TrueColor.Update();
	void ResetGroundCache();
	ResetGroundCache();
};

extern short WeaponIcn [32];

void cvi_Info::Process(){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		
		CanAttack=(SP->OB->MoreCharacter->MaxDam>0);

		if( IGI.Inventory && SP->Type==ve_UT_units ){

			vui_UnitInfo* I=&SP->Inf.Units;

			//const char* cLives=GetTextByID("#Lives");
			//const char* cLifeRegen=GetTextByID("#LifeRegen");
			//const char* cManaRegen=GetTextByID("#ManaRegen");
			//const char* cAttackRate=GetTextByID("#AttackRate");
			//const char* cAttackRadius=GetTextByID("#AttackRadius");
			//const char* cMotionSpeed=GetTextByID("#MotionSpeed");
			//const char* cKillCount=GetTextByID("#KillCount");

			OneObject* OB=SP->OB;
			//NewMonster* NM=OB->Ref.General->newMons;
			NewMonster* NM=OB->newMons;

			Damage=I->Damage[0]; //WeaponIcn[I->WeapType[SD->ID]]
			Damage2=I->Damage[1];
			
			DamageTypeSprite=WeaponIcn[I->WeapType[0]];
			Damage2TypeSprite=WeaponIcn[I->WeapType[1]];

			ShieldMag=I->Defence[0];
			ShieldPir=I->Defence[1];
			ShieldCut=I->Defence[3];
			ShieldCrs=I->Defence[2];

			LifeMax=OB->MaxLife;
			ManaMax=OB->MaxMana;

			HeroVariableStorage* HV=DetectHero(OB);
			if( HV && HV->Hero ){
				//Hint.print("%s: %d\n",cLives,HV->Lives);
				/*int */LifeRegen=HV->Hero->LifeRegeneration+HV->UpLifeRegeneration;
				int LR=LifeRegen;
				/*int */ManaRegen=HV->Hero->ManaRegeneration+HV->UpManaRegeneration+OB->ActiveAbility->CardManaRegeneration/100;
				int MR=ManaRegen;
				if(OB->ActiveAbility){
					OB->ActiveAbility->ActiveAbilities.modifyLifeRegeneration(LifeRegen,LR);
					OB->ActiveAbility->ActiveAbilities.modifyManaRegeneration(ManaRegen,MR);
				}
				LifeRegen=LR; //LifeRegen
				ManaRegen=MR; //ManaRegen
			}

			int GetAttSpeedBase(OneObject* OB,int AttType);
			int GetAttSpeed(OneObject* OB,int AttType);
			int AttackRate=GetAttSpeedBase(OB,0);
			int FR=GetAttSpeed(OB,0);
			FireRate=FR; //AttackRate

			int AttackRadius=OB->MoreCharacter->MaxR_Attack;
			int AR=OB->GetMaxAttackRadius();
			FireRange=AR; //AttackRadius

			byte VT=OB->MoreCharacter->VisionType;
			if(OB->ActiveAbility){		
				OB->ActiveAbility->ActiveAbilities.modifyVision(VT,VT);
			}
			Vision=VT;

			int BaseR=OB->GroupSpeed;
			int MotionSpeed=0;
			if(OB->MotionStyle==6){ // COMPLEXOBJECT
				MotionSpeed=OB->GroupSpeed;
			}else if(OB->MotionStyle==7){ // SINGLESTEP
				MotionSpeed=OB->GroupSpeed;
				MotionSpeed=(MotionSpeed*NM->SpeedScale)>>8;
				BaseR=MotionSpeed;
				MotionSpeed=(MotionSpeed*OB->MoreCharacter->Speed)/100;
			}else if(OB->MotionStyle==8){ // FLY
				MotionSpeed=((NM->MotionDist*NM->SpeedScale)>>8)*OB->MoreCharacter->Speed/100;
				BaseR=MotionSpeed;
			}
			int MS=MotionSpeed;
			if(OB->ActiveAbility){
				bool Stun=false;
				MS<<=5;
				OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(BaseR<<5,MS,Stun);
				MS>>=5;
			}		
			Speed=MS; //MotionSpeed

			BuildTime=I->BuildingTime;

			PriceFood=I->Price[0];
			PriceWood=I->Price[1];
			PriceStone=I->Price[2];
			PriceGold=I->Price[3];
			PriceIron=I->Price[4];
			PriceCryst=I->Price[5];

			Description.Clear();
			if(NM->GetLongMessage()){
				Description=NM->GetLongMessage();
			}

		}
	}
}

extern bool vls_ReInit;
extern bool EditMapMode;
extern bool InGame;

void cvi_InGamePanel::SetGameModeForSaveSin(){
	if( (vGameMode!=gmSin || GameModeForSave!=gmSin) && !(EditMapMode||InGame) ){
		vGameMode=gmSin;
		GameModeForSave=gmSin;
		vls_ReInit=true;
	}
};
void cvi_InGamePanel::SetGameModeForSaveCamp(){
	if( (vGameMode!=gmCamp || GameModeForSave!=gmCamp) && !(EditMapMode||InGame) ){
		vGameMode=gmCamp;
		GameModeForSave=gmCamp;
		vls_ReInit=true;
	}
};
void cvi_InGamePanel::SetGameModeForSave(){
	//if( GameModeForSave!=vGameMode ){
		GameModeForSave=vGameMode;
		vls_ReInit=true;
	//}
};
int cvi_InGamePanel::IsGameModeForSaveSin(){
	if( GameModeForSave==gmSin && !EditMapMode )
		return 1;
	else
		return 0;
};
int cvi_InGamePanel::IsGameModeForSaveCamp(){
	if( GameModeForSave==gmCamp && !EditMapMode )
		return 1;
	else
		return 0;
};

// minimap
void cvi_InGamePanel::MapRestoreCamera(){
	g_Navigator.m_TendToArgs.Defaults();
};
void SendAllySignal(int x, int y){
	extern int LastSignalX;
	extern int LastSignalY;
	extern int SignalDelay;
	extern DWORD SignalColor;
	LastSignalX=x<<4;
	LastSignalY=y<<4;
	SignalDelay=60*256;
	DWORD GetNatColor( int natIdx );
	SignalColor=GetNatColor(GSets.CGame.cgi_NatRefTBL[MyNation]);
	char chmess[128];
	sprintf(chmess,"@@@spot %d %d",x,y);
	void SendChat(char* str,bool Ally);
	SendChat(chmess,1);
}
//extern int LastMx;
//extern int LastMy;
extern float fMapX;
extern float fMapY;
void cvi_InGamePanel::MapSignal(){
	//SendAllySignal(LastMx,LastMy);
	SendAllySignal(fMapX*32,fMapY*32);
};
extern bool MiniMapVisible;
void cvi_InGamePanel::MapSetInterfaceState(){
	MapInterfaceState=(MapInterfaceState+1)%2;
	static bool LastMiniMapVisible=true;
	if( MapInterfaceState==1 ){
		LastMiniMapVisible=MiniMapVisible;
		MiniMapVisible=false;
	}else{
		MiniMapVisible=LastMiniMapVisible;
	}
};

bool cvi_InGamePanel::MapButtonsStateIsNormal(){
	return MiniMapVisible && (MapInterfaceState==0);
};
bool cvi_InGamePanel::MapButtonsStateIsMini(){
	return !MiniMapVisible && (MapInterfaceState==0);
};
bool cvi_InGamePanel::MapButtonsStateIsHide(){
	return (MapInterfaceState==1);
};
bool cvi_InGamePanel::MapButtonsStateIsNotHide(){
	return !MapButtonsStateIsHide();
};

void cvi_InGamePanel::SetStartNextCampMission(){
	StartNextCampMission=true;
};

bool cvi_InGamePanel::isLocalization0(){
	return GSets.Localization==0;
};
bool cvi_InGamePanel::isLocalization1(){
	return GSets.Localization==1;
};
bool cvi_InGamePanel::isPlayingTrack(){
//	int T=g_Navigator.GetPlayingTrackDuration();
//	int K=g_Navigator.GetPlayingTrackMaxSegKeysCount();
	return GSets.CGame.ViewMask==0 && g_Navigator.ShowInTrackFrame();//K>2/*T>1500*//*g_Navigator.IsPlayingTrack()*/;
}
