#pragma once
#include "MapTemplates.h"
//////////////////////////////////////////////////////////////////////////

class cvi_Info: public BaseClass {
public:
	int Damage;
	int Damage2;
	int DamageTypeSprite;
	int Damage2TypeSprite;
	int ShieldMag;
	int ShieldPir;
	int ShieldCut;
	int ShieldCrs;
	int LifeMax;
	int LifeRegen;
	int ManaMax;
	int ManaRegen;
	int FireRate;
	int FireRange;
	int Vision;
	int Speed;
	int BuildTime;
	int PriceFood;
	int PriceWood;
	int PriceStone;
	int PriceGold;
	int PriceIron;
	int PriceCryst;
	_str Description;

	bool CanAttack;

	SAVE(cvi_Info){
		REG_MEMBER(_int,Damage);
		REG_MEMBER(_int,Damage2);
		REG_MEMBER(_int,DamageTypeSprite);
		REG_MEMBER(_int,Damage2TypeSprite);
		REG_MEMBER(_int,ShieldMag);
		REG_MEMBER(_int,ShieldPir);
		REG_MEMBER(_int,ShieldCut);
		REG_MEMBER(_int,ShieldCrs);
		REG_MEMBER(_int,LifeMax);
		REG_MEMBER(_int,LifeRegen);
		REG_MEMBER(_int,ManaMax);
		REG_MEMBER(_int,ManaRegen);
		REG_MEMBER(_int,FireRate);
		REG_MEMBER(_int,FireRange);
		REG_MEMBER(_int,Vision);
		REG_MEMBER(_int,Speed);
		REG_MEMBER(_int,BuildTime);
		REG_MEMBER(_int,PriceFood);
		REG_MEMBER(_int,PriceWood);
		REG_MEMBER(_int,PriceStone);
		REG_MEMBER(_int,PriceGold);
		REG_MEMBER(_int,PriceIron);
		REG_MEMBER(_int,PriceCryst);
		REG_AUTO(Description);
		REG_MEMBER(_bool,CanAttack);
	}ENDSAVE;

	void Process();

};

class cvi_Options: public BaseClass {
private:
	int LastPreSet;
public:
	int PreSet;
	
	int Shadows;
	bool Reflection;
	int ColorBits;
	bool Antialiasing;
	int Shaders;
	int Bump;

	bool SpeedVariation;
	int  AutoSavePeriod;

	void Init();
	void Accept();

	SAVE(cvi_Options){
		REG_ENUM(_index,PreSet,opt_PreSet);
		REG_ENUM(_index,Shadows,opt_Shadows);
		REG_MEMBER(_bool,Reflection);
		REG_ENUM(_index,ColorBits,opt_ColorBit);
		REG_MEMBER(_bool,Antialiasing);
		REG_ENUM(_index,Shaders,opt_Shaders);
		REG_ENUM(_index,Bump,opt_Shaders); //opt_Bump
		REG_MEMBER(_bool,SpeedVariation);
		REG_ENUM(_index,AutoSavePeriod,AUTOSAVEPERIOD);
		REG_METHOD(Init);
		REG_METHOD(Accept);
	}ENDSAVE;

	void Process();
};

class cvi_InGamePanel : public ReferableBaseClass
{
public:
	cvi_InGamePanel(void);
	
	//int Level;
	//bool VirtualUp;
	//int AbIndex;
	void StartFrame(char* Message, int OpenTime, int ShowTime, int CloseTime);
	char* Mess;
	int Start;
	int Open;
	int Show;
	int Close;
	
	// сердечки героя
	bool HeroLive1;
	bool HeroLive2;
	bool HeroLive3;

	cvi_Options Opt;

	_str EndGameTitle;
	_str EndGameMessage;
	short EndGamePicFile;
	int EndGamePicSprite;

	short PreviewFile;
	int PreviewSprite;

	cvi_Info Info;

	//bool HeroLifeAlert;
	//DWORD HeroLifeAlertColor;
	_str HeroLifeAlert;

	int GameModeForSave;
	void SetGameModeForSaveSin();
	void SetGameModeForSaveCamp();
	void SetGameModeForSave();
	int IsGameModeForSaveSin();
	int IsGameModeForSaveCamp();

	bool MainMenuStarted;
	_str LoadingRace;
	int FirstLoadingType;
	
	/*int LoadingRace;
	bool LoadingRace0(){ return LoadingRace==0; }
	bool LoadingRace1(){ return LoadingRace==1; }
	bool LoadingRace2(){ return LoadingRace==2; }
	bool LoadingRace3(){ return LoadingRace==3; }*/
	//Old Loading Screen System
	bool FirstLoadingType0(){ return FirstLoadingType==0; }
	bool FirstLoadingType1(){ return FirstLoadingType==1; }

	// minimap
	void MapRestoreCamera();
	void MapSignal();
	int MapInterfaceState;
	void MapSetInterfaceState();
	
	bool MapButtonsStateIsNormal();
	bool MapButtonsStateIsMini();
	bool MapButtonsStateIsHide();
	bool MapButtonsStateIsNotHide();

	bool StartNextCampMission;
	void SetStartNextCampMission();

	//bool isMagicsButtonVisible();
	//bool isBottleButtonVisible();

	bool isLocalization0();
	bool isLocalization1();
	
	bool isPlayingTrack();

	int FirstTimeShowExIcon;

	SAVE(cvi_InGamePanel){
		REG_PARENT(ReferableBaseClass);
		
		//REG_MEMBER(_int,Level);
		//REG_MEMBER(_bool,VirtualUp);
		//REG_MEMBER(_int,AbIndex);
		
		REG_MEMBER(_bool,HeroLive1);
		REG_MEMBER(_bool,HeroLive2);
		REG_MEMBER(_bool,HeroLive3);

		REG_AUTO(Opt);

		REG_AUTO(EndGameTitle);
		REG_AUTO(EndGameMessage);
		REG_MEMBER(_gpfile,EndGamePicFile);
		REG_MEMBER(_int,EndGamePicSprite);

		REG_MEMBER(_gpfile,PreviewFile);
		REG_MEMBER(_int,PreviewSprite);

		REG_AUTO(Info);

		//REG_MEMBER(_bool,HeroLifeAlert);
		//REG_MEMBER(_DWORD,HeroLifeAlertColor);
		REG_AUTO(HeroLifeAlert);

		REG_METHOD(SetGameModeForSaveSin);
		REG_METHOD(SetGameModeForSaveCamp);
		REG_METHOD(SetGameModeForSave);
		REG_FN_INT(IsGameModeForSaveSin);
		REG_FN_INT(IsGameModeForSaveCamp);

		REG_MEMBER(_bool,MainMenuStarted);

		REG_MEMBER(_str,LoadingRace);
		/*REG_FN_BOOL(LoadingRace0);
		REG_FN_BOOL(LoadingRace1);
		REG_FN_BOOL(LoadingRace2);
		REG_FN_BOOL(LoadingRace3);*/
		REG_FN_BOOL(FirstLoadingType0);
		REG_FN_BOOL(FirstLoadingType1);

		REG_METHOD(MapRestoreCamera);
		REG_METHOD(MapSignal);
		REG_MEMBER(_int,MapInterfaceState);
		REG_METHOD(MapSetInterfaceState);
		
		REG_FN_BOOL(MapButtonsStateIsNormal);
		REG_FN_BOOL(MapButtonsStateIsMini);
		REG_FN_BOOL(MapButtonsStateIsHide);
		REG_FN_BOOL(MapButtonsStateIsNotHide);

		REG_METHOD(SetStartNextCampMission);

		//REG_FN_BOOL(isMagicsButtonVisible);
		//REG_FN_BOOL(isBottleButtonVisible);

		REG_FN_BOOL(isLocalization0);
		REG_FN_BOOL(isLocalization1);

		REG_FN_BOOL(isPlayingTrack);
		
	}ENDSAVE;

	virtual bool CheckIfObjectIsGlobal() { return true; }
	
	void Unloading();
	void Process();
	void Init();

};
extern cvi_InGamePanel vmIGP;

//////////////////////////////////////////////////////////////////////////

regAc(cva_IGP_Frame, vfS 
	ClassRef<TextButton> Mess,
	REG_AUTO(Mess);
);
regAc(cva_IGP_Credits, vfS , );
