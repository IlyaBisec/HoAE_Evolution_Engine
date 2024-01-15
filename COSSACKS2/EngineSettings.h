//
#pragma once

#include "comms\comms.h"
//
// MissGlobalSET /////////////////////////////////////////////////////////
class MissGlobalSET : public BaseClass
{
public:
	MissGlobalSET()		{ Play_Task_Sound=false; };
	~MissGlobalSET()	{};
	
	virtual bool ForceSimplification() {return true;};

	// Color Settings
	SubSection	ColorSetings;
	DWORD	vGroupNameColor;
	DWORD	vGroupLineColor;
	Delimiter	D0;
	DWORD	NodeNameColor;
	DWORD	NodeLineColor;
	DWORD	NodeIDColor;

	// TASK SET  //
	SubSection	TASK_MENU;
	bool	DonotShowComleteQuest;
	int		minTASK_Lx;
	int		TsskMarge_x;
	int		TsskMarge_y;
	int		TextMarge_x;
	int		TextMarge_y;
	void	SetDeff_TASK_MENU()	{ 
		DonotShowComleteQuest=true;
		minTASK_Lx=200; 
		TsskMarge_x=10; 
		TsskMarge_y=10; 
		TextMarge_x=5; 
		TextMarge_y=5; 
		NotActiveDiffuse=0x99FFFFFF; 
	};
	DWORD	NotActiveDiffuse;
	class SetDeff_ALEX : public BaseFunction
	{
	public:
		void EvaluateFunction(){
			MissGlobalSET* pPAR = get_parent<MissGlobalSET>();
			if (pPAR!=NULL) {
				pPAR->SetDeff_TASK_MENU();
			};
		};
		SAVE(SetDeff_ALEX);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}SetDeff_TM_ALEX;

	// For play sound after and front
	bool	Play_Task_Sound;
	_str	PTS_FileName;
	_str	PTS_FileName_Give;

	SAVE(MissGlobalSET);
		REG_AUTO(TASK_MENU);
		REG_MEMBER(_bool,DonotShowComleteQuest);
		REG_MEMBER(_int,minTASK_Lx);
		REG_MEMBER(_int,TsskMarge_x);
		REG_MEMBER(_int,TsskMarge_y);
		REG_MEMBER(_int,TextMarge_x);
		REG_MEMBER(_int,TextMarge_y);
		REG_MEMBER(_color,NotActiveDiffuse);
		REG_AUTO(SetDeff_TM_ALEX);
		REG_MEMBER(_bool,Play_Task_Sound);
		REG_FILEPATH(PTS_FileName,".ogg");
		REG_FILEPATH(PTS_FileName_Give,".ogg");
		REG_AUTO(ColorSetings);
			REG_MEMBER(_color,vGroupNameColor);
			REG_MEMBER(_color,vGroupLineColor);
			REG_AUTO(D0);
			REG_MEMBER(_color,NodeNameColor);
			REG_MEMBER(_color,NodeLineColor);
			REG_MEMBER(_color,NodeIDColor);
	ENDSAVE;
};
//////////////////////////////////////////////////////////////////////////

class engInterface:public BaseClass{
public:
	int VolumeFullScreenBik;
	int VolumeSoundEvents;
	int VolumeMissDialogs;
	//
	int selBorder;
	DWORD selColor;
	//
	word fSettlement;
	int sprSettlement;
	bool sprSettlementAni;
	int sprSettlementAniX;
	int sprSettlementAniY;
	int SettlementSpriteHeight;
	bool StartHelp;
	int neTime;
	char* neLivingPlaces; // not enought
	char* neSklad;
	char* neResourceForProduce;
	char* neAmmunition;
	char* neSettlementCaptured;
	char* neSettlementLost;
	char* neBrigadeTerminated;
	char* neBrigadeLost;
	//
	int HintX;
	int HintY;
	//
	int svFrescoMusic;
	int svFrescoVoice;
	//
	bool ShowPeasantDamage;
	bool ShowPreCampMessage;

	int MagicCardsVisibleAmount;

	SAVE(engInterface){
		REG_MEMBER(_int,VolumeFullScreenBik);
		REG_MEMBER(_int,VolumeSoundEvents);
		REG_MEMBER(_int,VolumeMissDialogs);
		REG_MEMBER(_int,selBorder);
		REG_MEMBER(_color,selColor);
		REG_MEMBER(_gpfile,fSettlement);
		REG_SPRITE(sprSettlement,fSettlement);
		REG_MEMBER(_bool,sprSettlementAni);
		REG_MEMBER(_int,sprSettlementAniX);
		REG_MEMBER(_int,sprSettlementAniY);		
		REG_MEMBER(_int,SettlementSpriteHeight);
		REG_MEMBER(_bool,StartHelp);
		REG_MEMBER(_int,neTime);
		REG_MEMBER(_textid,neLivingPlaces);
		REG_MEMBER(_textid,neSklad);
		REG_MEMBER(_textid,neResourceForProduce);
		REG_MEMBER(_textid,neAmmunition);
		REG_MEMBER(_textid,neSettlementCaptured);
		REG_MEMBER(_textid,neSettlementLost);
		REG_MEMBER(_textid,neBrigadeTerminated);
		REG_MEMBER(_textid,neBrigadeLost);
		REG_MEMBER(_int,HintX);
		REG_MEMBER(_int,HintY);
		REG_MEMBER(_int,svFrescoMusic);
		REG_MEMBER(_int,svFrescoVoice);
		REG_MEMBER(_bool,ShowPeasantDamage);
		REG_MEMBER(_bool,ShowPreCampMessage);
	}ENDSAVE;

	void Init();
	engInterface(){
		Init();
	}
	virtual void reset_class(void* DataPtr){
		BaseClass* B=(BaseClass*)DataPtr;
        engInterface* I=dynamic_cast<engInterface*>(B);
        if(I) I->Init();
	}
};
class EngineSettings;
class ResourceSettings:public BaseClass{
public:
	word File;
	int Sprite[6];
	char* Name[6];
	bool EnableSprHint;
	int dx;
	int dy;
	int Lx;
	int Ly;
	int SprHint[6];
	SAVE(ResourceSettings){
		REG_MEMBER(_gpfile,File);
		//
		REG_SPRITE_EX(Sprite[0],Sprite_0,File);
		REG_MEMBER(_textid,Name[0]);
		REG_SPRITE_EX(Sprite[1],Sprite_1,File);
		REG_MEMBER(_textid,Name[1]);
		REG_SPRITE_EX(Sprite[2],Sprite_2,File);
		REG_MEMBER(_textid,Name[2]);
		REG_SPRITE_EX(Sprite[3],Sprite_3,File);
		REG_MEMBER(_textid,Name[3]);
		REG_SPRITE_EX(Sprite[4],Sprite_4,File);
		REG_MEMBER(_textid,Name[4]);
		REG_SPRITE_EX(Sprite[5],Sprite_5,File);
		REG_MEMBER(_textid,Name[5]);
		//
		REG_MEMBER(_bool,EnableSprHint);
		REG_MEMBER(_int,dx);
		REG_MEMBER(_int,dy);
		REG_MEMBER(_int,Lx);
		REG_MEMBER(_int,Ly);
		REG_SPRITE_EX(SprHint[0],SprHint_0,File);
		REG_SPRITE_EX(SprHint[1],SprHint_1,File);
		REG_SPRITE_EX(SprHint[2],SprHint_2,File);
		REG_SPRITE_EX(SprHint[3],SprHint_3,File);
		REG_SPRITE_EX(SprHint[4],SprHint_4,File);
		REG_SPRITE_EX(SprHint[5],SprHint_5,File);
	}ENDSAVE;

	bool SetHint(byte NI, int* Cost, _str* txt);
};
class cHintTemplate:public BaseClass{
public:
	_str MessageID;
	_str HotKeyID;
	_str AccessControlID;
	_str PriceID;
	//
	_str HotKey;
	_str AccessControl;
	_str Produce;
	//
	SAVE(cHintTemplate){
		REG_AUTO(MessageID);
		REG_AUTO(HotKeyID);
		REG_AUTO(AccessControlID);
		REG_AUTO(PriceID);
		//
		REG_AUTO(HotKey);
		REG_AUTO(AccessControl);
		REG_AUTO(Produce);
	}ENDSAVE;

	//bool SetHint(byte NI, int* Cost, _str* txt);
	bool AddHotKey(_str* txt, int Key);
};
class OneFireInfo:public BaseClass{
public:	
	int ModelID;
	int SoundID;
	float Scale;
	OneFireInfo(){
		ModelID=-1;
		Scale=1;
		SoundID=-1;
	}
	SAVE(OneFireInfo);
		REG_MEMBER(_ModelID,ModelID);		
		REG_MEMBER(_float0_10,Scale);
		REG_ENUM(_index,SoundID,ALL_SOUNDS);
	ENDSAVE;
	void reset_class(void* ptr){
		OneFireInfo* FI=(OneFireInfo*)ptr;
		FI->Scale=1.0f;
		FI->SoundID=-1;
	}
	bool ForceSimplification(){return true;}
};
class OneRandomEffectInfo:public BaseClass{
public:	
	int ModelID;
	float Scale;
	float Probability;
	float MaxBirthPerSecond;
	float EffectErasureTime;
	OneRandomEffectInfo(){
		ModelID=-1;
		Scale=1;
		Probability=1.0f;
		MaxBirthPerSecond=4;
		EffectErasureTime=5;
	}
	SAVE(OneRandomEffectInfo);
	REG_MEMBER(_ModelID,ModelID);
	REG_MEMBER(_float0_10,Scale);
	REG_MEMBER(_float01,Probability);
	REG_MEMBER(_float0_10,MaxBirthPerSecond);
	REG_MEMBER(_float0_10,EffectErasureTime);
	ENDSAVE;
	void reset_class(void* ptr){
		OneRandomEffectInfo* FI=(OneRandomEffectInfo*)ptr;
		FI->Scale=1.0f;
	}
	bool ForceSimplification(){return true;}
};
class NatColorSettings:public BaseClass{
public:
	DWORD CLR[8];
	SAVE(NatColorSettings);		
		REG_MEMBER_EX(_color,CLR[0],C0);
		REG_MEMBER_EX(_color,CLR[1],C1);
		REG_MEMBER_EX(_color,CLR[2],C2);
		REG_MEMBER_EX(_color,CLR[3],C3);
		REG_MEMBER_EX(_color,CLR[4],C4);
		REG_MEMBER_EX(_color,CLR[5],C5);
		REG_MEMBER_EX(_color,CLR[6],C6);
		REG_MEMBER_EX(_color,CLR[7],C7);
	ENDSAVE;
	NatColorSettings(){
		init();
	}
	void init(){
		//for(int i=0;i<8;i++){
		//	DWORD GetNatColor(int);
		//	CLR[i]=GetNatColor(i);
		//}
		CLR[0]=0xFFA50000;
		CLR[1]=0xFF003CC6;
		CLR[2]=0xFF29B694;
		CLR[3]=0xFF9C49B5;
		CLR[4]=0xFFF78610;
		CLR[5]=0xFF292839;
		CLR[6]=0xFFE7E3E7;
		CLR[7]=0xFF6B4110;
	}
	void reset_class(void* BC){
		NatColorSettings* N=(NatColorSettings*)BC;
		N->init();
	}
	virtual bool ShouldSaveInShortForm(){ return false; }
};
class BrigArrowParam:public BaseClass{
public:
	int   StartArrowWidth;
	int   FinalArrowWidth;
	DWORD StartArrowColor;
	DWORD FinalArrowColor;
	int   RadiusOfEnemyDetection;
	DWORD StartCircleColor;
	DWORD FinalCircleColor;
	DWORD CircleLineColor;
	SAVE(BrigArrowParam);
		REG_MEMBER(_int,StartArrowWidth);
		REG_MEMBER(_int,FinalArrowWidth);
		REG_MEMBER(_color,StartArrowColor);
		REG_MEMBER(_color,FinalArrowColor);
		REG_MEMBER(_int,RadiusOfEnemyDetection);
		REG_MEMBER(_color,StartCircleColor);
		REG_MEMBER(_color,FinalCircleColor);
		REG_MEMBER(_color,CircleLineColor);
	ENDSAVE;
	void Init(){
		StartArrowWidth=80;
		FinalArrowWidth=32;
		StartArrowColor=0x400000FF;
		FinalArrowColor=0x80FF0000;
		RadiusOfEnemyDetection=600;		
		StartCircleColor=0x00FFFFFF;
		FinalCircleColor=0x40FF0000;
		CircleLineColor=0xFFFF0000;
	}
	BrigArrowParam(){
		Init();
	}
	void reset_class(void* B){
		BrigArrowParam* I=dynamic_cast<BrigArrowParam*>((BaseClass*)B);
		if(I){
			I->BaseClass::reset_class(I);
			I->Init();
		}
	}
};
class OneMapName:public BaseClass{
public:
	_str MapName;

	SAVE(OneMapName);
		REG_FILEPATH(MapName,"*.M3D");
	ENDSAVE;
	bool ForceSimplification(){return true;}
};
class SpecialSpeedForMaps:public BaseClass{
public:
	_str Description;
	int Speed;
	ClonesArray<OneMapName> MapsList;

	SAVE(SpecialSpeedForMaps);
		REG_CLASS(OneMapName);
		REG_AUTO(Description);
		REG_MEMBER(_int,Speed);
		REG_AUTO(MapsList);
	ENDSAVE;
	SpecialSpeedForMaps(){
        Speed=30;
	}
	const char* GetThisElementView(const char*){
		return Description.pchar();
	}	
};
class OnePrecashedSprite:public BaseClass{
public:
    word gpFileName;

	SAVE(OnePrecashedSprite);
		REG_MEMBER(_gpfile,gpFileName);
	ENDSAVE;

	bool ForceSimplification(){return true;}
};

void CREATE_ENUM_RTSMODECAMERACONTROLS();

class UnitAbility;
class EngineSettings: public BaseClass{
public:
	class RefreshSurface:public BaseFunction{
	public:
		void EvaluateFunction(){
			void RefreshWater();
			RefreshWater();
			void MakeAllDirtyGBUF();
			MakeAllDirtyGBUF();
		}
		SAVE(RefreshSurface){
			REG_PARENT(BaseFunction);
		}ENDSAVE;
	};
	bool DisableLog;
	SubSection ColorParams;

	bool  DynamicalReflection;
	RefreshSurface Refresh;
	DWORD MiniRoadColor;
	DWORD MiniWaterColor;
	DWORD WaterColorModulator;
	DWORD MiniFogOfWarColor;
	DWORD FogOfWarColor;
	DWORD LightDiffuse;
	DWORD LightAmbient;
	DWORD AtmosphereColor;
	DWORD ShadowsColor;
	DWORD SunColor;
	DWORD ShadowColor;
	DWORD EnemyHighliting;
	DWORD FriendsHighliting;
	DWORD AllyHighliting;
	DWORD MiniMapColorModulator;
	NatColorSettings NatColor;

	SubSection GeneralGameplayParams;

	int   AutoProduceFoodLimit;	
	bool  AutoChangeFormationType;
	bool  AllowChangeFormationDistance;	
	int   GreaterFormationDistanceScale;
	int   LowerFormationDistanceScale;
	int   MoraleType;
	bool  AllowTiring;
	bool  DontUseAgressiveState;
	int   MarkerDx;
	int   MarkerDy;
	int   DisbandFormLimit;
	int   MaxBrigAddDamage;
	int   DefaultMissOnHeight;
	int   DefaultMissOnHeightMax;
	int   DefaultMissInsideUnitsDamage;
	bool  DontUseRoads;
	int   MinDistanceToEnterRoad;
	int   MinTopDistanceToEnterRoad;
	int   MinDistForLineFormations;
	bool  DebugTopologyMode;
	float TerrainZBias;
    float EffectsZBias;
    float CameraFactor;
	bool  DrawBrigadeDestPositions;
	int   BrigadeWaitingCycles;
	bool  BrigadeVseZaOdnogoInAgresivMode;
	bool  BrigadeVseZaOdnogoInNormalMode;

	int	  BrigadeAlarmDist1;
	int	  BrigadeAlarmDist2;
	int	  BrigadeAlarmDist3;
	int	  BrigadeAlarmRadius;
	int	  BrigadeAlarmRadiusTime;
	int	  BoidsOffLimit;
	int   RestFrequency;
	int   CannonAddShotDistPer100_Height;

	int   DamageOnBuildingMultiplicator;
    bool  DontDieIfHungry;
    float SpriteUnitsScale;

	SubSection AI_Params;

	bool  EnableTacticalAI;
	bool  EnableVit2Alex;
	bool  EnableAutosavingInMissions;
	bool  EnableCapturing;
	bool  CaptureOnlyWithFormations;
	bool  DefendOnlyWithFormations;
	bool  LightenSelectedUnits;
	bool  DrawShotsFog;

	int KillerAwardPercentForCampaign;
	int   DefaultGameSpeedForCampaign;
	int   DefaultGameSpeedForSkirmish;	
	ClonesArray<SpecialSpeedForMaps> SpecifySpeedForMaps;
	bool  AllowCameraRotation;	
	//
	ClonesArray<OneFireInfo> FiresList;
	ClonesArray<OneFireInfo> SmokeList;
	ClonesArray<OneRandomEffectInfo> WaterBlobsList;
	ClonesArray<OneRandomEffectInfo> RunDustList;
	//
	SubSection VitalInterfaceSettings;

	engInterface vInterf;	// constants for vital interface
	bool AllyNetralBuildings;
	ResourceSettings Resource;
	cHintTemplate HintTemplate;	
	//
	
	ClonesArray<OnePrecashedSprite> PrecashedSprites;

	//
	SubSection      GraphicsParams;

    int TreesDirectionVariation;
    int TreesSizeVariation;

	SubSection      CameraParams;

//	float FOW_start;
//	float FOW_final;
//	float HorisontrAngle_start;
//	float HorisontrAngle_final;
//	float Distance_start;
//	float Distance_final;
	Delimiter fp;

	//*************************************************************************
	// LimitCamArgs
	//*************************************************************************
	class EngineSettingsLimitCamArgs : public BaseClass {
	public:
		void SetDefaults();
		class LimitCamArgsDefaults : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(LimitCamArgsDefaults);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} Defaults;

		const RECT GetRect() const;
		
		int MinMapXEdit;
		int MaxMapXEdit;
		int MinMapYEdit;
		int MaxMapYEdit;
		int MinMapXGame;
		int MaxMapXGame;
		int MinMapYGame;
		int MaxMapYGame;
		Delimiter Separator;
		
		SAVE(EngineSettingsLimitCamArgs);
			REG_MEMBER(_int, MinMapXEdit);
			REG_MEMBER(_int, MaxMapXEdit);
			REG_AUTO(Separator);
			REG_MEMBER(_int, MinMapYEdit);
			REG_MEMBER(_int, MaxMapYEdit);
			REG_AUTO(Separator);
			REG_MEMBER(_int, MinMapXGame);
			REG_MEMBER(_int, MaxMapXGame);
			REG_AUTO(Separator);
			REG_MEMBER(_int, MinMapYGame);
			REG_MEMBER(_int, MaxMapYGame);
			REG_AUTO(Separator);
			REG_AUTO(Defaults);
		ENDSAVE;
	} LimitCamArgs;

	//*************************************************************************
	// CamArgs
	//*************************************************************************
	class EngineSettingsCamArgs : public BaseClass {
	public:
		void SetDefaults();
		class CamArgsDefaults : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(CamArgsDefaults);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} Defaults;
		
		float GetYFovDeg() const;
		float GetYFovRad() const;
		float GetPitchDeg() const;
		float GetPitchRad() const;
		float GetToEye() const;
		float GetZNear() const;
		float GetZFar() const;
		float GetFacturesScale() const;

		void Push();
		bool Pop();
		
		struct EngineSettingsCamArgsStruct : public BaseClass {
			bool ForceSimplification();

			int YFov_fm;
			int YFov_to;
			int Pitch_fm;
			int Pitch_to;
			int ToEye_fm;
			int ToEye_to;
			int ZNear;
			int ZFarFogFarDelta;
			float FacturesScale;
			Delimiter Separator;

			#define EngineSettingsCamArgs_DefYFov_fm			22
			#define EngineSettingsCamArgs_DefYFov_to			67
			#define EngineSettingsCamArgs_DefPitch_fm			31
			#define EngineSettingsCamArgs_DefPitch_to			21
			#define EngineSettingsCamArgs_DefToEye_fm			2886
			#define EngineSettingsCamArgs_DefToEye_to			1607
			#define EngineSettingsCamArgs_DefZNear				50
			#define EngineSettingsCamArgs_DefZFarFogFarDelta	256
			#define EngineSettingsCamArgs_DefFacturesScale		1.0f
			
			SAVE(EngineSettingsCamArgsStruct);
				REG_ISLIDER(YFov_fm, 1, 120, EngineSettingsCamArgs_DefYFov_fm);
				REG_ISLIDER(YFov_to, 1, 120, EngineSettingsCamArgs_DefYFov_to);
				REG_AUTO(Separator);
				REG_ISLIDER(Pitch_fm, 0, 90, EngineSettingsCamArgs_DefPitch_fm);
				REG_ISLIDER(Pitch_to, 0, 90, EngineSettingsCamArgs_DefPitch_to);
				REG_AUTO(Separator);
				REG_ISLIDER(ToEye_fm, 500, 4000, EngineSettingsCamArgs_DefToEye_fm);
				REG_ISLIDER(ToEye_to, 500, 3000, EngineSettingsCamArgs_DefToEye_to);
				REG_AUTO(Separator);
				REG_ISLIDER(ZNear, 10, 200, EngineSettingsCamArgs_DefZNear);
				REG_ISLIDER(ZFarFogFarDelta, 0, 2000, EngineSettingsCamArgs_DefZFarFogFarDelta);
				REG_AUTO(Separator);
				REG_FSLIDER(FacturesScale, 0.01f, 2.0f, EngineSettingsCamArgs_DefFacturesScale);
			ENDSAVE;
		} Args;

		SAVE(EngineSettingsCamArgs);
			REG_AUTO(Args);
			REG_AUTO(Defaults);
		ENDSAVE;
		private:
			cList<EngineSettingsCamArgsStruct> m_Stack;
	} CamArgs;

	//*************************************************************************
	// FogArgs
	//*************************************************************************
	class EngineSettingsFogArgs : public BaseClass {
	public:
		void SetDefaults();
		class FogArgsDefaults : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(FogArgsDefaults);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} Defaults;
		
		int FogType;	
		float FogDensity;	
		Delimiter Separator;
		float start_FogFarPlane;	
		float start_FogNearPlane;
		float final_FogFarPlane;	
		float final_FogNearPlane;
		DWORD HorisonColorModulator;
		DWORD HorisonColorBlender;
		int FogRingTotalWidth;
		int FogRingHorisonWidth;
		int FogRingHorMargin;
		int SkyRotationPeriod;
		int SkyRadius;
		int SkyShift;
		float SkyRingScanFromFr;
		float SkyRingScanStripeFr;
		bool ShowSkyDome;

		float GetFogNearPlane() const;
		float GetFogFarPlane() const;

		SAVE(EngineSettingsFogArgs);
			REG_ENUM(_index, FogType, FOGTYPE);
			REG_MEMBER(_float, FogDensity);				
			REG_MEMBER(_color, HorisonColorModulator);
			REG_MEMBER(_color, HorisonColorBlender);
			REG_MEMBER(_int, FogRingTotalWidth);
			REG_MEMBER(_int, FogRingHorisonWidth);

	#define EngineSettings_DefFogRingHorMargin	100

			REG_MEMBER(_int, FogRingHorMargin);

			REG_MEMBER(_int, SkyRotationPeriod);
			REG_AUTO(Separator);

	#define EngineSettings_DefSkyRadius		20000
	#define EngineSettings_DefSkyShift		-2000
			
			REG_MEMBER(_int, SkyRadius);
			REG_MEMBER(_int, SkyShift);
			REG_AUTO(Separator);

	#define EngineSettings_DefSkyRingScanFromFr		0.5f
	#define EngineSettings_DefSkyRingScanStripeFr	0.15f

			REG_FSLIDER(SkyRingScanFromFr, 0.0f, 1.0f, EngineSettings_DefSkyRingScanFromFr);
			REG_FSLIDER(SkyRingScanStripeFr, 0.0f, 1.0f, EngineSettings_DefSkyRingScanStripeFr);
			REG_AUTO(Separator);
			REG_MEMBER(_bool, ShowSkyDome);
			REG_AUTO(Separator);
			REG_MEMBER(_float, start_FogNearPlane);
			REG_MEMBER(_float, start_FogFarPlane);
			REG_AUTO(Separator);
			REG_MEMBER(_float, final_FogNearPlane);
			REG_MEMBER(_float, final_FogFarPlane);
			REG_AUTO(Separator);
			REG_AUTO(Defaults);
		ENDSAVE;
	} FogArgs;

	// CollidingCamera:
	Delimiter CollidingCameraSection;
	class CollidingCameraArgs : public BaseFunction {
	public:
		void Defaults();
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool CheckIfElementInvisible(int, void *, DWORD);

		class CollidingCameraReset : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(CollidingCameraReset);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} Reset;

		// Params:
		int Radius;
		int Mass;
		int Bump;
		int MoveForce;
		int JumpForce;
		int MaxVelocity;
		int Gravity;
		float AirFriction;
		float ContactFriction;
		bool WASD;
		bool ESDF;
		float MouseSpeed;
		bool HideUI;
		Delimiter D;
		bool ForceCameraParams, pForceCameraParams;
		float CameraMovingStage;
		int final_FogNearPlane;
		int final_FogFarPlane;
		int SkyShift;
		float TimeStep;
		float TimeScale;
		int TimeOut;
		float Floatage;
		
		SAVE(CollidingCameraArgs);
		REG_PARENT(BaseFunction);
		// Visible:
		REG_ISLIDER(Radius, 5, 300, 120);
		REG_ISLIDER(Mass, 5, 100, 10);
		REG_ISLIDER(Bump, 1, 100, 10);
		REG_FSLIDER(AirFriction, 0.001f, 0.2f, 0.01f);
		REG_FSLIDER(ContactFriction, 0.001f, 0.2f, 0.05f);
		REG_ISLIDER(MoveForce, 100, 2000, 300);
		REG_ISLIDER(JumpForce, 100, 2000, 600);
		REG_ISLIDER(Gravity, 1, 60, 15);
		REG_ISLIDER(MaxVelocity, 10, 2000, 1000);
		REG_FSLIDER(Floatage, 0.0f, 1.0f, 0.2f);
		REG_AUTO(D);
		REG_MEMBER(_bool, WASD);
		REG_MEMBER(_bool, ESDF);
		REG_AUTO(D);
		REG_FSLIDER(MouseSpeed, 0.001f, 0.004f, 0.002f);
		REG_MEMBER(_bool, HideUI);
		REG_AUTO(D);
		REG_MEMBER(_bool, ForceCameraParams);
		REG_ISLIDER(final_FogNearPlane, 100, 10000, 1000);
		REG_ISLIDER(final_FogFarPlane, 100, 10000, 1500);
		REG_FSLIDER(CameraMovingStage, 0.0f, 1.0f, 1.0f);
		REG_MEMBER(_int, SkyShift);
		REG_AUTO(D);
		REG_FSLIDER(TimeStep, 0.02, 0.15, 0.05);
		REG_FSLIDER(TimeScale, 0.005, 0.05, 0.01);
		REG_ISLIDER(TimeOut, 100, 2000, 500);

		REG_AUTO(D);
		REG_AUTO(Reset);
		ENDSAVE;
	} CollidingCamera;

	// ThirdPersonCamera:
	Delimiter ThirdPersonCameraSection;
	class ThirdPersonCameraArgs : public BaseFunction {
	public:
		void Defaults();
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool CheckIfElementInvisible(int, void *, DWORD);

		class ThirdPersonCameraReset : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(ThirdPersonCameraReset);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} Reset;

		bool ForceCameraParams;
		float CameraMovingStage;
		int final_FogNearPlane;
		int final_FogFarPlane;
		Delimiter D;
		int FlyingUnitZOffset;
		int GroundUnitZOffset;
		int toEye;
		int ElevOffset;
		int GroundUnitMinRotator;
		bool WASD;
		bool ESDF;

		SAVE(ThirdPersonCameraArgs);
		REG_PARENT(BaseFunction);
		REG_MEMBER(_bool, ForceCameraParams);
		REG_ISLIDER(final_FogNearPlane, 100, 10000, 1000);
		REG_ISLIDER(final_FogFarPlane, 100, 10000, 1500);
		REG_FSLIDER(CameraMovingStage, 0.0f, 1.0f, 1.0f);
		REG_AUTO(D);
		REG_ISLIDER(FlyingUnitZOffset, 0, 400, 0);
		REG_ISLIDER(GroundUnitZOffset, 0, 400, 0);
		REG_ISLIDER(GroundUnitMinRotator, 1, 100, 2);
		REG_ISLIDER(toEye, 10, 1000, 237);
		REG_ISLIDER(ElevOffset, 0, 90, 30);
		REG_AUTO(D);
		REG_MEMBER(_bool, WASD);
		REG_MEMBER(_bool, ESDF);
		REG_AUTO(D);
		REG_AUTO(Reset);
		ENDSAVE;
	} ThirdPersonCamera;

	//*************************************************************************
	// RTSModeCameraArgs
	//*************************************************************************
	class RTSModeCameraArgs : public BaseFunction {
	public:
		void Defaults();
		__constructor(RTSModeCameraArgs) {
			Defaults();
		}

		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);

		class RTSModeCameraArgsReset : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(RTSModeCameraArgsReset);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} Reset;

		Delimiter D;

		float LiftSpeed;
		float LiftSpringConstant;

		int DollyMin;
		int DollyMax;
		int DollySpeed;
		float DollySpringConstant;

		float RotationSpeed;
		float RotationSpringConstant;

		int OnMouseWheel;
		int OnInsertDelete;
		int OnPageUpPageDown;

		SAVE(RTSModeCameraArgs);
		REG_PARENT(BaseFunction);
		
		CREATE_ENUM_RTSMODECAMERACONTROLS();

#define RTSModeCameraArgs_DefLiftSpeed			0.1f
#define RTSModeCameraArgs_DefLiftSpringConstant	0.1f

		REG_FSLIDER(LiftSpeed, -1.0f, 1.0f, RTSModeCameraArgs_DefLiftSpeed);
		REG_FSLIDER(LiftSpringConstant, 0.01f, 1.0f, RTSModeCameraArgs_DefLiftSpringConstant);

		REG_AUTO(D);

#define RTSModeCameraArgs_DefDollyMin				-2000
#define RTSModeCameraArgs_DefDollyMax				2000
#define RTSModeCameraArgs_DefDollySpeed				150
#define RTSModeCameraArgs_DefDollySpringConstant	0.2f

		REG_ISLIDER(DollyMin, - 4000, 0, RTSModeCameraArgs_DefDollyMin);
		REG_ISLIDER(DollyMax, 0, 4000, RTSModeCameraArgs_DefDollyMax);
		REG_ISLIDER(DollySpeed, -400, 400, RTSModeCameraArgs_DefDollySpeed);
		REG_FSLIDER(DollySpringConstant, 0.01f, 1.0f, RTSModeCameraArgs_DefDollySpringConstant);
	
		REG_AUTO(D);

#define RTSModeCameraArgs_DefRotationSpeed			0.15f
#define RTSModeCameraArgs_DefRotationSpringConstant	0.250f

		REG_FSLIDER(RotationSpeed, -1.0f, 1.0f, RTSModeCameraArgs_DefRotationSpeed);
		REG_FSLIDER(RotationSpringConstant, 0.01f, 1.0f, RTSModeCameraArgs_DefRotationSpringConstant);

		REG_AUTO(D);
		REG_ENUM(_index, OnMouseWheel, RTSMODECAMERACONTROLS);
		REG_ENUM(_index, OnInsertDelete, RTSMODECAMERACONTROLS);
		REG_ENUM(_index, OnPageUpPageDown, RTSMODECAMERACONTROLS);
		REG_AUTO(D);
		REG_AUTO(Reset);
		ENDSAVE;

		static const int ROTATE = 0;
		static const int DOLLY = 1;
		static const int LIFT = 2;
	} RTSModeCamera;

	//*************************************************************************
	// TankControllerArgs
	//*************************************************************************
	class TankControllerArgs : public BaseFunction {
	public:
		void Defaults();
		__constructor(TankControllerArgs) {
			Defaults();
		}

		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);

		class TankControllerArgsReset : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(TankControllerArgsReset);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} Reset;

		Delimiter D;

		int ElevationShift;
		int ZOffset;
		int ToEye;

		SAVE(TankControllerArgs);
		REG_PARENT(BaseFunction);

#define TankControllerArgs_DefElevationShift	10
#define TankControllerArgs_DefZOffset			80
#define TankControllerArgs_DefToEye				250

		REG_ISLIDER(ElevationShift,  -90, 90, TankControllerArgs_DefElevationShift);
		REG_ISLIDER(ZOffset, 0, 400, TankControllerArgs_DefZOffset);
		REG_ISLIDER(ToEye, 0, 2000, TankControllerArgs_DefToEye);
		
		REG_AUTO(D);
		REG_AUTO(Reset);
		ENDSAVE;
	} TankController;

	SubSection		RoadsSettings;
	// VITYA (MISSIONS SETTINGS)  //
	SubSection		GeneralMissionsSettings;
	int				DefMusicVolumeInMissFilms;

	class bf_SetDialogSoundVolume: public BaseFunction{
	public:
		SAVE(bf_SetDialogSoundVolume){
			REG_PARENT(BaseFunction);
		}ENDSAVE;
		virtual bool ForceProperty() {
			return true;
		}
		void EvaluateFunction();
	};
	bf_SetDialogSoundVolume SetDialogSoundVolume;

	MissGlobalSET	MIS_SET;
	////////////////////////////////
	int				GameName;
	
	bool NeuroLearn;
	bool NeuroUse;

	int BuildIconLx;
	int BuildIconLy;
	int PortraitIconLx;
	int PortraitIconLy;
	int MaxSpaceExtraDistance;

	LinearArray<int,_int> NationalBordersStartID;
	
	SubSection CirclesAndArrowsColors;

	BrigArrowParam BrigadesArrowParam;

	DWORD AttackRangeFillInnerColor;
	DWORD AttackRangeFillOuterColor;
	DWORD AttackRangeLineOuterColor;

	DWORD BuildingAttackRangeInnerLineColor;
	DWORD BuildingAttackRangeOuterLineColor;
	DWORD BuildingAttackRangeInnerFillColor;
	DWORD BuildingAttackRangeOuterFillColor;

	DWORD AttackRangeFillColorInner1;
	DWORD AttackRangeFillColorOuter1;
	DWORD AttackRangeLineColorOuter1;

	DWORD AttackRangeFillColorInner2;
	DWORD AttackRangeFillColorOuter2;
	DWORD AttackRangeLineColorOuter2;
	
	DWORD AttackRangeFillColorInner3;
	DWORD AttackRangeFillColorOuter3;
	DWORD AttackRangeLineColorOuter3;

	DWORD FillCannonFillColor1;
	DWORD FillCannonFillColor2;

	SubSection PredefinedColors;

	DWORD Black;
	DWORD Red;	
	DWORD Yellow;
	DWORD White;	
	DWORD Gray;	
	DWORD Orange; 
	DWORD Disable;

	SubSection DefaultUnitsParams;

	int Default_Collision;
	int	 CollisionPushCoef;
	bool Default_LowCollision;
	int Default_FreeShotDist;
	int Default_DstZPoint;
	int Default_MaxMeleeAttackers;
	int Default_DeadUnitDisappearTime;
	int Default_SkeletonDisappearTime;

	SubSection ScoreConstants;

	int BornModulator;
	int KillModulator;
	int LooseModulator;
	int HeroKillModulatorPerLevel;
	int HeroLooseModulatorPerLevel;

	
	//SubSection Balance;
	//ClassRef<UnitAbility> SettlementUnitProduceAbility;
	Delimiter D;
	Vector3D LightDir;

	SAVE(EngineSettings);
		REG_AUTO(ColorParams);

		REG_MEMBER(_bool,DisableLog);

		REG_AUTO(Refresh);
		REG_MEMBER(_bool,DynamicalReflection);
		REG_MEMBER(_color,MiniMapColorModulator);
		REG_MEMBER(_color,MiniRoadColor);
		REG_MEMBER(_color,MiniWaterColor);
		REG_MEMBER(_color,WaterColorModulator);
		REG_MEMBER(_color,MiniFogOfWarColor);
		REG_MEMBER(_color,FogOfWarColor);
		REG_MEMBER(_color,LightDiffuse);
		REG_MEMBER(_color,LightAmbient);
		REG_MEMBER(_color,AtmosphereColor);
		REG_MEMBER(_color,ShadowsColor);
		REG_MEMBER(_color,SunColor);
		REG_MEMBER(_color,ShadowColor);
		REG_MEMBER(_color,EnemyHighliting);
		REG_MEMBER(_color,FriendsHighliting);
		REG_MEMBER(_color,AllyHighliting);
        REG_AUTO(NatColor);
		REG_AUTO(Refresh);

		REG_AUTO(CirclesAndArrowsColors);

		REG_MEMBER(_color,AttackRangeFillInnerColor);
		REG_MEMBER(_color,AttackRangeFillOuterColor);
		REG_MEMBER(_color,AttackRangeLineOuterColor);

		REG_MEMBER(_color,BuildingAttackRangeInnerLineColor);
		REG_MEMBER(_color,BuildingAttackRangeOuterLineColor);
		REG_MEMBER(_color,BuildingAttackRangeInnerFillColor);
		REG_MEMBER(_color,BuildingAttackRangeOuterFillColor);

		REG_MEMBER(_color,AttackRangeFillColorInner1);
		REG_MEMBER(_color,AttackRangeFillColorOuter1);
		REG_MEMBER(_color,AttackRangeLineColorOuter1);

		REG_MEMBER(_color,AttackRangeFillColorInner2);
		REG_MEMBER(_color,AttackRangeFillColorOuter2);
		REG_MEMBER(_color,AttackRangeLineColorOuter2);

		REG_MEMBER(_color,AttackRangeFillColorInner3);
		REG_MEMBER(_color,AttackRangeFillColorOuter3);
		REG_MEMBER(_color,AttackRangeLineColorOuter3);

		REG_MEMBER(_color,FillCannonFillColor1);
		REG_MEMBER(_color,FillCannonFillColor2);

		REG_AUTO(PredefinedColors);

		REG_MEMBER(_color,Black);
		REG_MEMBER(_color,Red);	
		REG_MEMBER(_color,Yellow);
		REG_MEMBER(_color,White);	
		REG_MEMBER(_color,Gray);	
		REG_MEMBER(_color,Orange); 
		REG_MEMBER(_color,Disable);        

		REG_AUTO  (GeneralGameplayParams);

        REG_MEMBER(_float01,SpriteUnitsScale);
		REG_ENUM(_index,GameName,BE_GAME_NAME);
		REG_MEMBER(_int,DefaultGameSpeedForCampaign);
		REG_MEMBER(_int,DefaultGameSpeedForSkirmish);
		REG_CLASS(SpecialSpeedForMaps);
		REG_AUTO(SpecifySpeedForMaps);
		REG_MEMBER(_int,AutoProduceFoodLimit);
		REG_MEMBER(_bool,AutoChangeFormationType);
		REG_MEMBER(_bool,AllowChangeFormationDistance);
		REG_MEMBER(_int,GreaterFormationDistanceScale);
		REG_MEMBER(_int,LowerFormationDistanceScale);
		REG_ENUM  (_index,MoraleType,MoraleType);
		REG_MEMBER(_bool,AllowTiring);		
		REG_MEMBER(_int,DisbandFormLimit);
		REG_MEMBER(_int,MaxBrigAddDamage);
		REG_MEMBER(_int,DefaultMissOnHeight);
		REG_MEMBER(_int,DefaultMissOnHeightMax);
		REG_MEMBER(_int,DefaultMissInsideUnitsDamage);		
		REG_MEMBER(_bool,DebugTopologyMode);		
		REG_MEMBER(_int,BrigadeWaitingCycles);
		REG_MEMBER(_bool,BrigadeVseZaOdnogoInAgresivMode);
		REG_MEMBER(_bool,BrigadeVseZaOdnogoInNormalMode);

		REG_MEMBER(_int,BrigadeAlarmDist1);
		REG_MEMBER(_int,BrigadeAlarmDist2);
		REG_MEMBER(_int,BrigadeAlarmDist3);
		REG_MEMBER(_int,BrigadeAlarmRadius);
		REG_MEMBER(_int,BrigadeAlarmRadiusTime);
		REG_MEMBER(_int,BoidsOffLimit);
		REG_MEMBER(_int,RestFrequency);
		REG_MEMBER(_int,CannonAddShotDistPer100_Height);

		REG_MEMBER(_int,DamageOnBuildingMultiplicator);

		REG_MEMBER(_bool,DontUseAgressiveState);
		REG_MEMBER(_bool,AllowCameraRotation);
		REG_AUTO(BrigadesArrowParam);

		REG_AUTO(AI_Params);

		REG_MEMBER(_bool,EnableTacticalAI);
		REG_MEMBER(_bool,EnableVit2Alex);	
		REG_MEMBER(_bool,EnableCapturing);		
		REG_MEMBER(_bool,CaptureOnlyWithFormations);
		REG_MEMBER(_bool,DefendOnlyWithFormations);
		REG_MEMBER(_bool,NeuroLearn);	
		REG_MEMBER(_bool,NeuroUse);		
		//
		REG_AUTO(VitalInterfaceSettings);
		REG_AUTO(vInterf);
		REG_MEMBER(_bool,AllyNetralBuildings);
		REG_MEMBER(_int,BuildIconLx);
		REG_MEMBER(_int,BuildIconLy);
		REG_MEMBER(_int,PortraitIconLx);
		REG_MEMBER(_int,PortraitIconLy);
		REG_AUTO(NationalBordersStartID);
		REG_AUTO(Resource);
		REG_AUTO(HintTemplate);
		REG_MEMBER(_int,MaxSpaceExtraDistance);
		//
		REG_AUTO(RoadsSettings);
		REG_MEMBER(_bool,DontUseRoads);
		REG_MEMBER(_int,MinDistanceToEnterRoad);
		REG_MEMBER(_int,MinTopDistanceToEnterRoad);
		REG_MEMBER(_int,MinDistForLineFormations);

		REG_AUTO(GraphicsParams);

		REG_MEMBER(_float,LightDir.x);
		REG_MEMBER(_float,LightDir.y);
		REG_MEMBER(_float,LightDir.z);

		REG_AUTO(D);
		
		REG_CLASS(OnePrecashedSprite);
		REG_AUTO(PrecashedSprites);
		REG_AUTO(Resource);
		REG_AUTO(HintTemplate);		
		REG_CLASS(OneFireInfo);
		REG_CLASS(OneRandomEffectInfo);
		REG_AUTO(FiresList);
		REG_AUTO(SmokeList);
		REG_AUTO(WaterBlobsList);
		REG_AUTO(RunDustList);
		REG_MEMBER(_bool,LightenSelectedUnits);
		REG_MEMBER(_int, MarkerDx);
		REG_MEMBER(_int, MarkerDy);
		REG_MEMBER(_float,TerrainZBias);
		REG_MEMBER(_float,EffectsZBias);
        REG_MEMBER(_float,CameraFactor);
		REG_MEMBER(_bool,DrawBrigadeDestPositions);
		REG_MEMBER(_bool,DrawShotsFog);
        REG_MEMBER(_int,TreesDirectionVariation);
        REG_MEMBER(_int,TreesSizeVariation);

		REG_AUTO(CameraParams);

//		REG_MEMBER(_float01,FOW_start);
//		REG_MEMBER(_float01,FOW_final);
//		REG_MEMBER(_float01,HorisontrAngle_start);
//		REG_MEMBER(_float01,HorisontrAngle_final);
//		REG_MEMBER(_float0_10,Distance_start);
//		REG_MEMBER(_float0_10,Distance_final);
		REG_AUTO(LimitCamArgs);
		REG_AUTO(CamArgs);
		REG_AUTO(FogArgs);
		REG_AUTO(CollidingCameraSection);
		REG_AUTO(CollidingCamera);
		REG_AUTO(ThirdPersonCameraSection);
		REG_AUTO(ThirdPersonCamera);
		REG_AUTO(RTSModeCamera);
		REG_AUTO(TankController);

		// VITYA (MISSIONS SETTINGS)  //
		REG_AUTO(GeneralMissionsSettings);	
		REG_MEMBER(_int,DefMusicVolumeInMissFilms);
		REG_AUTO(SetDialogSoundVolume);
		REG_AUTO(MIS_SET);
		////////////////////////////////		

		REG_AUTO(DefaultUnitsParams);

		REG_MEMBER(_int,CollisionPushCoef);
		REG_MEMBER(_int,Default_Collision);
		REG_MEMBER(_bool,Default_LowCollision);
		REG_MEMBER(_int,Default_FreeShotDist);
		REG_MEMBER(_int,Default_DstZPoint);
		REG_MEMBER(_int,Default_MaxMeleeAttackers);
		REG_MEMBER(_int,Default_DeadUnitDisappearTime);
		REG_MEMBER(_int,Default_SkeletonDisappearTime);

		REG_AUTO(ScoreConstants);

		REG_MEMBER(_int,BornModulator);
		REG_MEMBER(_int,KillModulator);
		REG_MEMBER(_int,LooseModulator);
		REG_MEMBER(_int,HeroKillModulatorPerLevel);
		REG_MEMBER(_int,HeroLooseModulatorPerLevel);

		//REG_AUTO(Balance);
		//REG_AUTO(SettlementUnitProduceAbility);

	ENDSAVE;	
	void Init(){
		DrawShotsFog=false;
		DynamicalReflection=false;
		MiniMapColorModulator=0xFFFFFFFF;
		MiniRoadColor=0xFF605040;
		MiniWaterColor=0xFF306080;
		MiniFogOfWarColor=0xB0505050;
		FogOfWarColor=0xFFFFFFFF;
		LightDiffuse=0xFF808080;
		LightAmbient=0xFF222222;
		WaterColorModulator=0xFF808080;
		AtmosphereColor=0;
		ShadowsColor=0xFF808080;
		SunColor=0xFF808080;
		EnemyHighliting   = 0xFFB04040;
		FriendsHighliting = 0xFF4040B0;
		AllyHighliting    = 0xFF40B040;
		AutoProduceFoodLimit=0;
		AutoChangeFormationType=0;
		AllowChangeFormationDistance=0;
		GreaterFormationDistanceScale=200;
		MoraleType=0;
		AllowTiring=1;
		ShadowsColor=0;
		DisbandFormLimit=25;
		MaxBrigAddDamage=10000;
		float GetTerrainZBias();
		TerrainZBias=GetTerrainZBias();
        EffectsZBias=150;
        CameraFactor=3.5f;
		DrawBrigadeDestPositions=false;
		BrigadeWaitingCycles=40;	
		BrigadeVseZaOdnogoInAgresivMode=false;
		BrigadeVseZaOdnogoInNormalMode=false;
		BrigadeAlarmDist1=1500;
		BrigadeAlarmDist2=1000;
		BrigadeAlarmDist3=700;
		BrigadeAlarmRadius=700;
		BrigadeAlarmRadiusTime=6;
		BoidsOffLimit=5000;
		RestFrequency=128*8;
		CannonAddShotDistPer100_Height=300;
        SpriteUnitsScale=0.8;

		EnableTacticalAI=false;
		EnableVit2Alex=false;
		EnableAutosavingInMissions=true;
		EnableCapturing=false;
		CaptureOnlyWithFormations=false;
		DefendOnlyWithFormations=false;
		NeuroLearn=false;
		NeuroUse=false;
		MinDistanceToEnterRoad=2500;
		MinTopDistanceToEnterRoad=1100;

		KillerAwardPercentForCampaign=10;
		DefaultGameSpeedForCampaign=25;
		DefaultGameSpeedForSkirmish=25;

		MinDistForLineFormations=1800;
		BuildIconLx=45;
		BuildIconLy=45;
		PortraitIconLx=45;
		PortraitIconLy=45;
		MaxSpaceExtraDistance=5;


		Black	 = 0xFF2E2317;
		Red	     = 0xFF8A1000;
		Yellow   = 0xFFD4C19C;
		White	 = 0xFFFFF7EF;
		Gray	 = 0xFF6D6862;
		Orange   = 0xFF6A3000;
		Disable  = 0xC0665F57;

		AttackRangeFillInnerColor=0x1EFF0000;
		AttackRangeFillOuterColor=0x28FF0000;
		AttackRangeLineOuterColor=0x80FF0000;

		BuildingAttackRangeInnerLineColor=0x40FF0000;
		BuildingAttackRangeOuterLineColor=0x90FF0000;
		BuildingAttackRangeInnerFillColor=0x40FF0000;
		BuildingAttackRangeOuterFillColor=0x80FF0000;

		AttackRangeFillColorInner1=0x0AFFFFFF;
		AttackRangeFillColorOuter1=0x3CFF0000;
		AttackRangeLineColorOuter1=0xFFFF0000;

		AttackRangeFillColorInner2=0x1EFF0000;
		AttackRangeFillColorOuter2=0x3CFFFF00;
		AttackRangeLineColorOuter2=0xFFFFFF00;

		AttackRangeFillColorInner3=0x1EFFFF00;
		AttackRangeFillColorOuter3=0x3C00FF00;
		AttackRangeLineColorOuter3=0xFF00FF00;

		FillCannonFillColor1=0x1000FF00;
		FillCannonFillColor2=0x6000FF00;

//		FOW_start = 0.5;
//		FOW_final = 0.5;
//		HorisontrAngle_start = 0.3;
//		HorisontrAngle_final = 0.1;
//		Distance_start = 5.0;
//		Distance_final = 2.0;

		CollisionPushCoef=16;
		Default_Collision=8;
		Default_LowCollision=true;
		Default_FreeShotDist=150;
		Default_DstZPoint=30;
		Default_MaxMeleeAttackers=12;
		Default_DeadUnitDisappearTime=1250;
		Default_SkeletonDisappearTime=1000;

		LimitCamArgs.SetDefaults();
		CamArgs.SetDefaults();
		FogArgs.SetDefaults();
		CollidingCamera.Defaults();
		ThirdPersonCamera.Defaults();

		BornModulator=1;
		KillModulator=3;
		LooseModulator=2;
		HeroKillModulatorPerLevel=200;
		HeroLooseModulatorPerLevel=100;
//		RTSModeCamera.Defaults();
		LightDir=Vector3D( -1.5f, -1.0f, -1.5f );
	}

	EngineSettings(){
		Init();
	}
	void reset_class(void* B){
        EngineSettings* I=dynamic_cast<EngineSettings*>((BaseClass*)B);
		if(I){
			I->BaseClass::reset_class(I);
			I->Init();
		}
	}
};
extern EngineSettings EngSettings;//
#include "Localization.h"