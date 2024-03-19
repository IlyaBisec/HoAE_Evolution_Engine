#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__
#include <ClassEngine.h>
#include <more_types.h>
#pragma pack( push )
#pragma pack( 1 )
#include "EngineSettings.h"
class GameClass:public BaseClass{
public:
	void SaveToFile(char* fName);
	bool LoadFromFile(char* fname);
	SAVE(GameClass)		
	ENDSAVE
};
typedef LinearArray<int,_accamulator> StrAccamulator;
class CLeibzigDemo:public BaseClass{
public:
	bool Enabled;
	bool RecBroken;
    StrAccamulator RecordsList;
	StrAccamulator MissionsList;
	StrAccamulator VideoList;
	int VideoScaleX;
    int VideoScaleY;
	int StayTime;
	char* GetStr(StrAccamulator* str,int index){
		static Enumerator* E=NULL;
		if(!E)E=ENUM.Get("STDENUM");
		return E->Get((*str)[index]);
	}
	bool StartRecord();
	bool StartVideo();
	CLeibzigDemo(){
		StayTime=60000;
		Enabled=0;
		RecBroken=0;
	};
	SAVE(CLeibzigDemo)
		REG_MEMBER(_bool,Enabled);
		REG_AUTO_ENUM(RecordsList,STDENUM);
		REG_AUTO_ENUM(MissionsList,STDENUM);
		REG_AUTO_ENUM(VideoList,STDENUM);
		REG_MEMBER(_int,StayTime);
		REG_MEMBER(_int,VideoScaleX);
		REG_MEMBER(_int,VideoScaleY);
	ENDSAVE
};

class SoundAndVideoOptions:public BaseClass{
public:
	void Init(){
		ScreenSizeX=1024;
		ScreenSizeY=768;
		SoundVolume=50;
		MusicVolume=50;
		RequiredMsPerFrame=40;
		ScrollSpeed=4;
		MusicPlayMode=1;
		WaterQuality=true;
		RazduplingTimeForDemo=20000;
		SleepTimeForDemo=100;
		SelectionType=0;
		DrawReflectedTerrain=true;
		DrawReflectedModels=true;
		DrawWater=true;
        DrawEffects=true;
		DialogAdditionViewTime=4000;
        DrawUnits=true;
        Draw3DUnits=true;
        Draw3DShadows=true;
        Draw2DUnits=true;
        Draw2DShadows=true;
        DrawRoads=true;
        DrawTrees=true;
        DrawBumpOnLandcape=false;
        DrawHQWater=true;
        DrawSpritesIn2Pass=true;
        HQShaders=true;
		GameSpeedAutoAdjustment=true;
		DrawBumpOnUnits=true;
		//LangIndex=0;
		RelativDialogSoundVolume=0.8;
		RelativDialogMusicVolume=0.5;
		RelativeVideoToSoundVolume=1.0;
		EnablePostEffects=false;
		MotionBlurAlpha=1;
		AutoBrightDegree=1;
		AutoBrightAdaptSpeed=20;
	}
	SoundAndVideoOptions(){
        Init();		
	}
	void reset_class(void* DataPtr){
		((SoundAndVideoOptions*)DataPtr)->Init();
	}
	int ScreenSizeX;
	int ScreenSizeY;
	int SoundVolume;//0..100
	int MusicVolume;//0..100
	int RequiredMsPerFrame;
	int ScrollSpeed;
	int MusicPlayMode;//0-no music 1-random music
	bool WaterQuality;//true-good,false-bad
	int SleepTimeForDemo;
	int RazduplingTimeForDemo;
	int SelectionType;
	bool IsometricMode;
	bool DrawReflectedTerrain;
	bool DrawReflectedModels;
	bool DrawWater;
    bool DrawEffects;
    bool DrawUnits;
    bool Draw3DUnits;
    bool Draw3DShadows;
    bool Draw2DUnits;
    bool Draw2DShadows;
    bool DrawRoads;
    bool DrawTrees;
    bool DrawBumpOnLandcape;
    bool DrawHQWater;
    bool HQShaders;
    bool DrawSpritesIn2Pass;
	bool DrawBumpOnUnits;
	bool GameSpeedAutoAdjustment;
	bool EnablePostEffects;
	float MotionBlurAlpha;
	float AutoBrightDegree;
	float AutoBrightAdaptSpeed;

	int DialogAdditionViewTime;

	float RelativDialogSoundVolume;	// [0.f,1.f]
	float RelativDialogMusicVolume;	// [0.f,1.f]
	float RelativeVideoToSoundVolume;

	class AntialiasingArgs : public BaseFunction {
	public:
		void Defaults();
		__constructor(AntialiasingArgs) {
			Defaults();
		}

		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		void EvaluateFunction();

		bool pEnabled, Enabled;

		SAVE(AntialiasingArgs);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_bool, Enabled);
		ENDSAVE();
	} Antialiasing;
	
	class TrueColorArgs : public BaseFunction {
	public:
		void Defaults();
		__constructor(TrueColorArgs) {
			Defaults();
		}

		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		void EvaluateFunction();

		bool pEnabled, Enabled;
		Delimiter D;
		bool ShowDeviceProperties;

		SAVE(TrueColorArgs);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_bool, Enabled);
			REG_AUTO(D);
			REG_MEMBER(_bool, ShowDeviceProperties);
			ENDSAVE();
	} TrueColor;

/*	class SafeRefreshRateArgs : public BaseFunction {
	public:
		void Defaults();
		__constructor(SafeRefreshRateArgs) {
			Defaults();
		}

		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		void EvaluateFunction();

		bool pEnabled, Enabled;

		SAVE(SafeRefreshRateArgs);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_bool, Enabled);
		ENDSAVE();
	} SafeRefreshRate;*/

	void Update() {
		Antialiasing.Update();
		TrueColor.Update();
//		SafeRefreshRate.Defaults();
	}
	Delimiter D;
	SAVE(SoundAndVideoOptions);
		REG_MEMBER(_int,ScreenSizeX);
		REG_MEMBER(_int,ScreenSizeY);
		REG_MEMBER(_int,SoundVolume);
		REG_MEMBER(_int,MusicVolume);
        REG_AUTO(D);
		REG_MEMBER(_bool,EnablePostEffects);
		REG_FSLIDER(MotionBlurAlpha,0,1,1);
		REG_FSLIDER(AutoBrightDegree,0,3,1);
		REG_FSLIDER(AutoBrightAdaptSpeed,1,100,20);
		REG_AUTO(D);
		REG_MEMBER(_int,RequiredMsPerFrame);
		REG_MEMBER(_int,ScrollSpeed);
		REG_MEMBER(_int,MusicPlayMode);
		//REG_MEMBER(_bool,WaterQuality);
		REG_MEMBER(_int,SleepTimeForDemo);
		REG_MEMBER(_int,RazduplingTimeForDemo);
		REG_ENUM(_index,SelectionType,UNITS_SEL_TYPE);
		REG_MEMBER(_bool,IsometricMode);
		REG_MEMBER(_bool,DrawReflectedTerrain);
		REG_MEMBER(_bool,DrawReflectedModels);
		REG_MEMBER(_bool,DrawWater);
        REG_MEMBER(_bool,DrawEffects);
        REG_MEMBER(_bool,DrawUnits);
        REG_MEMBER(_bool,Draw3DUnits);
        REG_MEMBER(_bool,Draw3DShadows);
        REG_MEMBER(_bool,Draw2DUnits);
        REG_MEMBER(_bool,Draw2DShadows);
        REG_MEMBER(_bool,DrawRoads);
        REG_MEMBER(_bool,DrawTrees);
        REG_MEMBER(_bool,DrawBumpOnLandcape);
        REG_MEMBER(_bool,DrawHQWater);
        REG_MEMBER(_bool,HQShaders);
        REG_MEMBER(_bool,DrawSpritesIn2Pass);
		REG_MEMBER(_bool,DrawBumpOnUnits);
		REG_MEMBER(_bool,GameSpeedAutoAdjustment);
		REG_MEMBER(_int,DialogAdditionViewTime);
		REG_AUTO(Antialiasing);
		REG_AUTO(TrueColor);
//		REG_AUTO(SafeRefreshRate);
		REG_MEMBER(_float01,RelativDialogSoundVolume);
		REG_MEMBER(_float01,RelativDialogMusicVolume);
		REG_MEMBER(_float01,RelativeVideoToSoundVolume);
	ENDSAVE;
};
class PlayerParams: public BaseClass{
public:
	int Hero;
	int GamePlay;
	SAVE(PlayerParams){
		REG_MEMBER(_int,Hero);
		REG_MEMBER(_int,GamePlay);
	}ENDSAVE;
};
class CommonRoomParams: public BaseClass{
	byte* tempBuf;
	_str  prevXml;
public:
	//////////////////////////////////////////////////////////////////////////	
	//insert there different common parameters
    int GameSpeed;
	int HeroLives;
	int GameType;
	int StartRes;
	int StartCards;
	int StartArtfs;
	int RTS_mode;
	ClonesArray<PlayerParams> Players;

	SAVE(CommonRoomParams){
		
		// не нужно сохранять, эти параметры при старте должны сбрасываться на значения по умолчанию
		REG_ENUM(_index,GameSpeed,GAMESPEED);
		REG_ENUM(_index,HeroLives,HEROLIVES);
		REG_ENUM(_index,GameType,GAMETYPE);
		REG_ENUM(_index,StartRes,STARTRES);		
		REG_ENUM(_index,RTS_mode,RTS_MODE);

		//REG_ENUM(_index,StartCards,STARTCARDS);
		//REG_ENUM(_index,StartArtfs,STARTARTFS);
		//REG_AUTO(Players);
	}ENDSAVE;
	//////////////////////////////////////////////////////////////////////////	
	void Init();
	virtual void reset_class(void* ptr){
		BaseClass::reset_class(ptr);
		((CommonRoomParams*)ptr)->Init();
	}
	CommonRoomParams(){
		tempBuf=NULL;
		prevXml="";
		Init();
	}
	void InitBeforeGame();
	byte* GetCompressed(int& L);
	void SetCompressed(byte* data,int size);
	bool CheckChanges();
	//////////////////////////////////////////////////////////////////////////
	int GetNParams();
	Enumerator* GetEnum(int index);
	char* GetFieldName(int index);
	char* GetFieldValue(int index);
	void  SetFieldValue(int index,char* value);
};
class CurrentGameInfo:public BaseClass{
public:
	byte CurState;//0-outside room, 1 - host & in room, 2 - client & in room, 3 - in game  
	DWORD ViewMask;//1-res panel 2-minimap 4-units interface
	bool SilenceMessageEvents;
	//

	int InterfaceNatID;

	PlayerInfo PL_INFO[8];
	byte cgi_NatRefTBL [8];
	char cgi_CurrentMap[64];	
	int UnitLimit;
	void RunSingleplayerGame(byte Color);
	void StartSinglePlayerGame(const char* MapName,int Color);
	void PlaySinglePlayerGame();
	void PostSinglePlayerGame();
	void RunHostGame();
	void RunClientGame();
	void ProcessRoom();
	bool CreateRoom(char* RoomName,int MaxPlayers);
	bool JoinRoom(char* ip,char* RoomName);
	void LeaveRoom();
	PlayerInfo* GetCurrentPlayerInfo();
	PlayerInfo* GetHostPlayerInfo();
	CurrentGameInfo(){
		CurState=0;
		for(int i=0;i<8;i++)cgi_NatRefTBL[i]=i;
		ViewMask=255;
	}
	int GetReadyPercent();
	int GetPing(DWORD PlayerID);
	bool Kick(DWORD PlayerID);
	bool isHumanPlayer(byte Color);
	bool UnitTransform;
	bool ArcadeMode;
    int  SpecialGameSpeed;
};
class VideoFile:public BaseClass{
public:
	_str File;
	float ScaleX;
	float ScaleY;
	SAVE(VideoFile);
		REG_FILEPATH(File,".bik");
		REG_MEMBER(_float01,ScaleX);
		REG_MEMBER(_float01,ScaleY);
	ENDSAVE;
	VideoFile(){
        ScaleX=1;        
		ScaleY=1;
	}
};
class GameSettings:public GameClass{
public:
	virtual void reset_class(void* DataPtr);
	//insert there different constants and settings
	//and then register them below
	CLeibzigDemo LeibzigDemo;
	ClonesArray<VideoFile> VideoList;
	SoundAndVideoOptions SVOpt;
	bool CheckG17_dates;
	bool AllowFormationsStatesProcessing;
	bool ShowWayOnRoad;
	bool ShowFPS;
	bool HintDebugMode;
	byte StartMode;
	int AnimQuality;	// 0 - super , 1 - 1/2 frame drop, 2 - 2/3 frame drop
	CurrentGameInfo CGame;
	CommonRoomParams RoomParams;//translated in network
	bool ShowChat;
	int ShowRecListTime;
	int GameDayLength;	// in AnimTime
	//
	bool DisableHint;
	bool DisableVideo;
	bool DisableSelectionBar;
	bool DisableMinimap;
	int  AutoSavePeriod;
	bool AutoSaveWasDone;
	//
	bool ReleaseInterface;
	//
	bool ShowAurasMarker;
	word SelBarGP;
	word gpRally;
	word gpMove;
	bool DisableFriendlyFire;
	bool PeasantAutoWork;

    //
    int   ShadowQuality;
    bool EnableSubtitres;

	int Localization;

	//-----------registration section--------------
	SAVE(GameSettings){
		//register it there!
		REG_CLASS(VideoFile);
		REG_AUTO(LeibzigDemo);
		REG_AUTO(SVOpt);
		REG_AUTO(RoomParams);
		REG_AUTO(VideoList);
		REG_MEMBER(_bool,CheckG17_dates);
		REG_MEMBER(_bool,AllowFormationsStatesProcessing);
		REG_MEMBER(_bool,ShowWayOnRoad);
		REG_MEMBER(_bool,ShowFPS);
		REG_MEMBER(_bool,HintDebugMode);
		REG_ENUM  (_byte_index,StartMode,PerspMode);
        //REG_ENUM  (_index,ShadowQuality,ShadowQuality);
		REG_MEMBER(_int,ShadowQuality);
		REG_MEMBER(_int,AnimQuality);
		REG_MEMBER(_bool,ShowChat);
		REG_MEMBER(_int,ShowRecListTime);
		REG_MEMBER(_int,GameDayLength);
		//
		REG_MEMBER(_bool,DisableHint);
		REG_MEMBER(_bool,DisableVideo);
		REG_MEMBER(_bool,DisableSelectionBar);
		REG_MEMBER(_bool,DisableMinimap);
		//
		REG_MEMBER(_bool, ReleaseInterface);
		REG_MEMBER(_bool, ShowAurasMarker);
		REG_MEMBER(_gpfile,SelBarGP);
		REG_MEMBER(_gpfile,gpRally);
		REG_MEMBER(_gpfile,gpMove);
		REG_MEMBER(_bool,DisableFriendlyFire);
		REG_MEMBER(_bool,PeasantAutoWork);
		//
		REG_MEMBER(_bool,EnableSubtitres);		
		REG_ENUM(_index,AutoSavePeriod,AUTOSAVEPERIOD);
		REG_MEMBER(_int,Localization);
	}ENDSAVE;
	void Save(){
        SaveToFile("settings.xml");
	}
	void Update() {
		SVOpt.Update();
	}
	void Load(){
		LoadFromFile("settings.xml");
	}
	void Init();
	GameSettings(){		
		Init();
	}
};
//
#ifdef IMPLEMENT_CLASS_FACTORY
GameSettings GSets;
bool GetHintDebugMode(){
	return GSets.HintDebugMode;
}
//
void GameClass::SaveToFile(char* fName){
	xmlQuote xml("GameSettings");
	BaseClass::Save(xml,this);
	xml.WriteToFile(fName);
}
bool GameClass::LoadFromFile(char* fname){
	xmlQuote xml;
	if(xml.ReadFromFile(fname)){
		ErrorPager EP;
		BaseClass::Load(xml,this,&EP);
		return true;
	}else return false;
}
#else //IMPLEMENT_CLASS_FACTORY
extern GameSettings GSets;
#endif //IMPLEMENT_CLASS_FACTORY
#pragma pack( pop )
#endif //__GAME_SETTINGS_H__