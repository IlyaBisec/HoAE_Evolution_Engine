#pragma once
//////////////////////////////////////////////////////////////////////////
#include "..\ClassEngine\more_types.h"
#include "VictoryConditions.h"
#include "IWater.h"
//
class VictoryType:public ClassPtr<VictoryConditionRoot>{ //ClassArray<VictoryConditionRoot>{
public:
	/*
	virtual int GetExpansionRules(){
		if(!GetAmount())return 2;
		else return 0;
	}
	*/
};
//==================================================================================================================//
class PlayGameProcess : public BaseClass
{
public:
	SAVE(PlayGameProcess);
	ENDSAVE;
	virtual bool Process();
};
//------------------------------------------------------------------------------------------------------------------//
class PlayGameProcessList : public ClassArray<PlayGameProcess>
{
public:
	SAVE(PlayGameProcessList);
	ENDSAVE;
	virtual int GetExpansionRules();
	bool Process();
};
//------------------------------------------------------------------------------------------------------------------//
class StartTacticalAI : public PlayGameProcess
{
public:
	StartTacticalAI();
	int NI;
	SAVE(StartTacticalAI);
	REG_PARENT(PlayGameProcess);
	REG_MEMBER(_int,NI);
	ENDSAVE;
	virtual bool Process();
};
class PreviewBinkVideo : public PlayGameProcess
{
public:
	_str BinkFile;
	SAVE(PreviewBinkVideo){
		REG_PARENT(PlayGameProcess);
		REG_FILEPATH(BinkFile,".bik");		
	}ENDSAVE;
	virtual bool Process();
};
//==================================================================================================================//

class StartResources:public BaseClass{
public:
	int Wood;
	int Gold;
	int Stone;
	int Food;
	int Iron;
	int Coal;
	SAVE(StartResources);
	REG_MEMBER(_int,Wood);
	REG_MEMBER(_int,Gold);
	REG_MEMBER(_int,Stone);
	REG_MEMBER(_int,Food);
	REG_MEMBER(_int,Iron);
	REG_MEMBER(_int,Coal);
	ENDSAVE;
};
class MapPlayerInfo:public BaseClass{
public:
	StartResources StartRes;	
	bool AutoSelectBuildings;	
	_str CustomAI_Script;
	bool DontSelectPanicers;
	bool DontAffectFogOfWar;
	// vital
	bool DisableInSingle;
	bool DisableInMultiplayer;
	bool DisableNationSelect;
	byte Nation;				// in NatList
	bool DisableStrategicAI;	// razvitie, postroyka unitov
	bool LockTeam;
	int  Team;
	bool AutoTransferUnitsToNearestTeam;
	// vitya (for mission)	//  [3/3/2005]
	bool UseStartPos;		// Set start camera position in (cX,cY)
	int	 cX,cY;
	// vitay end

	SAVE(MapPlayerInfo);
	SAVE_SECTION(1);
		REG_MEMBER(_bool,DisableInSingle);
		REG_MEMBER(_bool,DisableInMultiplayer);
		REG_MEMBER(_bool,DisableStrategicAI);
		REG_AUTO(StartRes);
		REG_MEMBER(_bool,AutoSelectBuildings);
		REG_MEMBER(_bool,DontSelectPanicers);
		REG_MEMBER(_bool,DontAffectFogOfWar);
		REG_AUTO(CustomAI_Script);
		// vital		
		REG_MEMBER(_bool,DisableNationSelect);
	SAVE_SECTION(2);
		REG_ENUM(_byte_index,Nation,NationsName);
	SAVE_SECTION(1);
		REG_MEMBER(_bool,AutoTransferUnitsToNearestTeam);
		REG_MEMBER(_bool,LockTeam);
	SAVE_SECTION(4);
		REG_ENUM(_index,Team,TeamList);
		// vitya (for mission)	//  [3/3/2005]
	SAVE_SECTION(1);
		REG_MEMBER(_bool,UseStartPos);
	SAVE_SECTION(0x10);
		REG_MEMBER(_int,cX);
		REG_MEMBER(_int,cY);
		// vitya end
	ENDSAVE;
	
	DWORD GetClassMask(){
		DWORD M=1;
		if(DisableNationSelect) M|=2;
		if(LockTeam) M|=4;
		if(UseStartPos) M|=0x10;
		return M;
	}

};
class MapScriptReplace:public BaseClass{
public:
	_str OriginalNDS;
	_str CustomNDS;
	SAVE(MapScriptReplace);
	REG_AUTO(OriginalNDS);
	REG_AUTO(CustomNDS);
	ENDSAVE;
};
class ScriptRepList:public ClassArray<MapScriptReplace>{
public:
	virtual int GetExpansionRules(){
		return 1;
	}
};
class StartResScope:public BaseClass{
public:
	bool DisableTeams;
	MapPlayerInfo Player[MaxNatColors+1];
	int GetN_Single();
	int GetN_Multi();
	SAVE(StartResScope)
		REG_MEMBER(_bool,DisableTeams);
		for(int i=0;i<=MaxNatColors;i++){
			char cc[32];
			sprintf(cc,"Player%d",i);
			REG_AUTO_EX2(Player[i],cc);
		}
	ENDSAVE;
};
class cmo_Preview: public BaseClass{
public:
	cmo_Preview(){ Temp="Temp\\temp.bmp"; }
	_str Temp;
	bool Player[MaxNatColors+1];
	_picfile UserPicture;
	SAVE(cmo_Preview)
		for(int i=0;i<=MaxNatColors;i++){
			char cc[32];
			sprintf(cc,"Player%d",i);
			REG_MEMBER_EX2(_bool,Player[i],cc);
		}
		REG_AUTO(UserPicture);
	ENDSAVE;
};
//===============test===================
class Test1:public BaseClass{
public:
	AUTONEW(Test1);
};
class Test2:public BaseClass{
public:
	AUTONEW(Test2);
};
class Test12:public Test1,public Test2{
public:
	int x12;
	SAVE_EX(Test12,Test1);
	REG_PARENT(Test1);
	REG_PARENT(Test2);
	REG_MEMBER(_int,x12);
	ENDSAVE;
};
class Test11:public Test1{
public:
	int x11;
	SAVE(Test11);
	REG_PARENT(Test1);	
	REG_MEMBER(_int,x11);
	ENDSAVE;
};
class Test22:public Test2{
public:
	int x22;
	SAVE(Test22);
	REG_PARENT(Test2);	
	REG_MEMBER(_int,x22);
	ENDSAVE;
};

#include "Surface\Surface.h"
typedef BaseClass* fnDllInterface();

class DllExtension:public BaseClass{
public:
	_str DllName;
	_str LastLoadDllName;
	HMODULE LibID;
	BaseClass* Interface;
	void LoadDll(const char* Name, bool Permanent);
	void UnLoadDll();
	SAVE(DllExtension);
		REG_FILEPATH(DllName,".dll,.cl");
	ENDSAVE;
	__constructor(DllExtension){
        DllName="";
		LibID=NULL;
		Interface=NULL;
	}
	~DllExtension(){
		UnLoadDll();
	}
	virtual bool ForceSimplification(){
		return true;
	}
};
#include "Camera\Track.h"
//======================================
class SimpleCTrack:public CameraTrack{
public:
	bool ForceSimplification(){ return true; } 
};
class NamedCameraTrack:public ReferableBaseClass{
public:
	_str Name;
	SimpleCTrack Track;
	SAVE(NamedCameraTrack);
		REG_AUTO(Name);
		REG_AUTO(Track);
	ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="CameraTrack: {CY}%$Name$%";
		return true;
	}
};

class MapOptions:public BaseClass{
public:
	MapOptions(){
		Init();
	}
	void Init();
	virtual void reset_class(void *DataPtr);

	bool IsActive; // And again lame hack
	ClonesArray<NamedCameraTrack> CameraTracks;
	bool HoldBlackScreenTillTrackStarts;
	// Surf:
	Delimiter D;
	ClassArray<SurfArchway> Archways;
	// TankController:
	bool LinkCameraToTank;
	//bool Opt;
	VictoryType VictoryRule;
	bool RandomizePlayersPositions;
	int  MaxPlayers;
	int  MaxRPGLives;
	bool SetLivesToMax;
	bool DontCheckForAlonePlayer;
	bool DontTellAboutLivingPlaces;
	bool DontAllowCityLife;
    bool HideEnemyUnitsUnderFog;
	bool AutoChangeRoadsNation;
	bool RPGOnly;
	bool RTSOnly;
	bool DoNotChangeHeroes;
	bool ThirdPersonGame;
	StartResScope Players;
    ScriptRepList CustomNDS;
	ClonesArray<DllExtension> Plugins;
	PlayGameProcessList OnMissionStart;
	cmo_Preview Preview;
	DWORD FogColor;
    int SkyBoxModel;

	//********************************************************************************
	// MapOptionsLimitCamArgs
	//********************************************************************************
	class MapOptionsLimitCamArgs : public BaseClass {
	public:
		bool UseFromMap, PrevUseFromMap;
		int MinMapX, MaxMapX, MinMapY, MaxMapY;
		Delimiter Separator;

		float LeftShift, RightShift, TopShift, BottomShift;
		// Aux Vars:
		int LastShiftCalcMinMapX, LastShiftCalcMinMapY, LastShiftCalcMaxMapX, LastShiftCalcMaxMapY, LastShiftCalcXRes, LastShiftCalcYRes;
		float LastShiftCalcToEyeEx;

		void ZeroAuxVars();

		__constructor(MapOptionsLimitCamArgs) {
			ZeroAuxVars();
		}

		bool IsValid();
		void Validate();
		const RECT GetRect();
		const cBounds GetRectFixed();
		
		void SetFromEngineSettings();
		bool CheckIfElementInvisible(int, void *, DWORD);
		bool OnChangeMember(BaseClass *, void *, void *, const char *);

		class LimitCamArgsCopy : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(LimitCamArgsCopy);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} CopyFromEngineSettings;

		SAVE(MapOptionsLimitCamArgs);
			REG_MEMBER(_bool, UseFromMap);
			REG_MEMBER(_bool, PrevUseFromMap);
			REG_AUTO(Separator);
			REG_MEMBER(_int, MinMapX);
			REG_MEMBER(_int, MaxMapX);
			REG_AUTO(Separator);
			REG_MEMBER(_int, MinMapY);
			REG_MEMBER(_int, MaxMapY);
			REG_AUTO(Separator);
			REG_AUTO(CopyFromEngineSettings);
		ENDSAVE;
	} LimitCamArgs;

	//*************************************************************************
	// MapOptionsFogArgs
	//*************************************************************************
	class MapOptionsFogArgs : public EngineSettings::EngineSettingsFogArgs {
	public:
		bool UseFromMap, PrevUseFromMap;
		Delimiter Separator;

		bool CheckIfElementInvisible(int, void *, DWORD);
		bool OnChangeMember(BaseClass *, void *, void *, const char *);

		void SetFromEngineSettings();
		class FogArgsCopy : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(FogArgsCopy);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} CopyFromEngineSettings;

		SAVE(MapOptionsFogArgs);
			REG_MEMBER(_bool, UseFromMap);
			REG_MEMBER(_bool, PrevUseFromMap);
			REG_AUTO(Separator);
			REG_PARENT(EngineSettings::EngineSettingsFogArgs);
			REG_AUTO(CopyFromEngineSettings);
		ENDSAVE;
	} FogArgs;

	//*************************************************************************
	// MapOptionsCamArgs
	//*************************************************************************
	class MapOptionsCamArgs : public EngineSettings::EngineSettingsCamArgs {
	public:
		bool UseFromMap, PrevUseFromMap;
		Delimiter Separator;

		bool CheckIfElementInvisible(int, void *, DWORD);
		bool OnChangeMember(BaseClass *, void *, void *, const char *);

		void SetFromEngineSettings();
		class CamArgsCopy : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(CamArgsCopy);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} CopyFromEngineSettings;

		SAVE(MapOptionsCamArgs);
			REG_MEMBER(_bool, UseFromMap);
			REG_MEMBER(_bool, PrevUseFromMap);
			REG_AUTO(Separator);
			REG_PARENT(EngineSettings::EngineSettingsCamArgs);
			REG_AUTO(CopyFromEngineSettings);
		ENDSAVE;
	} CamArgs;
	
	be_CARRAY LimitUnitsInSettlement;
	void LimitUnitsInSettlementRemoveDups();

	int GlobalMapScale; // 256 = 100 %
	
	bool OnChangeMember(BaseClass *, void *, void *, const char *);
	float GetGlobalScale() const {
        return (float)GlobalMapScale / 256.0f;
	};
	//
	int GetDiff(){ 
		return CITY[MyNation].Difficulty; 
	}
	void SetDiff(int v){
		CITY[MyNation].Difficulty=v;
	}	
	INT_PROPERTY(MapOptions,GetDiff,SetDiff);
	//ClassPtr<Test1> CPT1;
	//ClassPtr<Test2> CPT2;
	SAVE(MapOptions);
		//-------------------
		REG_CLASS(Test1);
		REG_CLASS(Test2);
		REG_CLASS_AMBIGUOUS(Test12,Test1);
		REG_CLASS(Test11);
		REG_CLASS(Test22);
		REG_CLASS(NamedCameraTrack);
		//-------------------
		REG_CLASS(DllExtension);
		REG_CLASS(MapScriptReplace);
		//REG_MEMBER(_bool,Opt);
		REG_MEMBER(_bool,RandomizePlayersPositions);
		REG_MEMBER(_int,MaxPlayers);
		REG_MEMBER(_bool,DontCheckForAlonePlayer);
		REG_MEMBER(_bool,DontTellAboutLivingPlaces);
		REG_MEMBER(_bool,DontAllowCityLife);
        REG_MEMBER(_bool,HideEnemyUnitsUnderFog);
		REG_MEMBER(_bool,AutoChangeRoadsNation);
		REG_MEMBER(_bool,RPGOnly);
		REG_MEMBER(_int,MaxRPGLives);
		REG_MEMBER(_bool,SetLivesToMax);
		REG_MEMBER(_bool,RTSOnly);
		REG_MEMBER(_bool,DoNotChangeHeroes);
		REG_MEMBER(_bool,ThirdPersonGame);
		REG_AUTO(VictoryRule);
		REG_AUTO(Players);
		//SAVE_SECTION(2);
		REG_AUTO(CustomNDS);
		REG_AUTO(OnMissionStart);
		REG_AUTO(Plugins);
		//REG_AUTO(CPT1);
		//REG_AUTO(CPT2);
		REG_AUTO(Preview);
		REG_MEMBER(_color,FogColor);
        REG_MEMBER(_ModelID,SkyBoxModel);
		REG_AUTO(CamArgs);
		REG_AUTO(FogArgs);
		REG_AUTO(LimitCamArgs);
		//
		REG_INTPROP(_int,Difficulty,GetDiff,SetDiff);
		// Surf:
		REG_AUTO(D);
		REG_AUTO(Archways);
		// TankController:
		REG_AUTO(D);
		REG_MEMBER(_bool, LinkCameraToTank);
		// Tracks:
		REG_AUTO(D);
		REG_AUTO(CameraTracks);
		REG_MEMBER(_bool, HoldBlackScreenTillTrackStarts);
		REG_AUTO(D);
		REG_AUTO(LimitUnitsInSettlement);
		REG_MEMBER(_int,GlobalMapScale);
	ENDSAVE;
	/*DWORD GetClassMask(){
        if(Opt)return 0xFFFFFFFF;
		else return 1;
	}*/
};
extern MapOptions MOptions;

//*****************************************************************************
// MapOptionsGameSaver
//*****************************************************************************
class MapOptionsGameSaver : public GameExtension {
public:
	bool OnGameSaving(xmlQuote &xml);
	bool OnGameLoading(xmlQuote &xml);
	bool OnCheatEntering(const char *);
};
