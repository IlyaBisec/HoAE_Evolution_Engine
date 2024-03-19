#include "stdheader.h"
#define NAME "CEW_KERNEL"
#define _WINSOCKAPI_
#include <malloc.h>
#include "resource.h"
#include "ddini.h"
#include "wintab\wintab.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "Megapolis.h"
#include "multipl.h"
#include "fog.h"
//#include "Nature.h"
#include <time.h>
//#include "Nucl.h"
#include "dialogs.h"
#include "smart_assert.h"
#include "3DSurf.h"
#include "CDirSnd.h"
#include "GSound.h"
#include "Video.h"
#include "dplay.h"
#include "MapSprites.h"
#include <crtdbg.h>
#include "RealWater.h"
#include "Transport.h"
#include "AntiBug.h"
#include "3DmapEd.h"
#include "fonts.h"
#include "diplomacy.h"
#define INDDEX1
#include "GP_Draw.h"
#include "Dialogs\InitFonts.h"
#include "gamesettings.h"
#include "mp3\oggvor.h"
#include "vital\vital_def.h"
#include "theovorb\theovorb.h"
#include "MassivePlayer.h"
bool UseSysMouse=0;
#ifdef _USE3D
#define TITLE "Heroes of Annihilated Empires"
#pragma pack( push )  
#pragma pack( 8 ) 
#include "IInput.h"
#pragma pack( pop )  
#endif // _USE3D
#include "GlobalActions.h"
#include "ActiveZone.h"
#include "Arc\\GSCtypes.h"
#include "ClassEditor.h"
#include "AI_Scripts.h"
#include "Dialogs\\MISSIONSCRIPT\\MissionScript.h"	//Vitya //  [11/4/2003]
#include "VictoryConditions.h"
#include "vui_GlobalHotKey.h"
#include "ITerrain.h"
#include "UndoMaster.h"

//#include ".\cvi_singleplayerdata.h"

#include "Graphs.h"
extern bool GameInProgress;
extern BlockBars LockBars;
extern BlockBars UnLockBars;
int DrawPixMode=0;
int DrawGroundMode=0;
int CurGroundTexture=0;
#include "FEX\FEXModule.h"
#include "GameExtensionInternal.h"
#include "Instrument.h"
#include "CurrentMapOptions.h"
#include "Weapon.h"
#include "kTimer.h"
#include "Surface\Surface.h"
#include "Surface\SurfaceGenerator.h"
#include "Surface\SurfProcess.h"
#include "Surface\CollidingCamera.h"
#include "Surface\TankController.h"
#include "Camera\Navigator.h"
#include "Surface\Messager.h"
#include "Surface\HiResScreenShot.h"
//
//
//#define GERMANVERSION
void KeyTestMem();
extern bool LockPause;
extern bool Recreate;
extern bool MiniActive;
void CmdChangeSpeed();
int FrmDec=1;
int SpeedSh=0;
void CheckGP();
void SelectAllBuildings(byte NI);
int LASTRAND,LASTIND;
void EraseAreas();
byte PauseMode=0;
bool VHMode=0;
int LastAttackDelay=0;
void ProcessFishing();
void RenderAllMap();
char* FormationStr=NULL;
extern char SaveFileName[128];
extern bool SetDestMode;
bool FastMode=false;
int GoldID;
int FoodID;
int StoneID;
int TreeID;
int CoalID;
int IronID;
int MoneyID=6;
CEXPORT char BuildVersion[32];
extern bool GoAndAttackMode;
extern int Inform;
extern int ShowGameScreen;
extern bool AttGrMode;

int g_RefreshRateOverriddenValue = 60; // Hz
bool g_RefreshRateIsOverridden = false;

void InitFishMap();
void LoadMessages();
int exRealLx;
int exRealLy;
int exFMode=1;
extern bool FullScreenMode;
extern bool GameNeedToDraw;
void GSSetup800();
void ClearMaps();
void CreateTotalLocking();
void CreateUnitsLocking();
bool TransMode;
byte LockGrid;
byte TopTypeMode=0;
bool PeaceMode;
byte LockMode;
void ProcessSprites();
void Reset3D();
void Init3DMapSystem();
void CreateRandomHMap();
void SetLight(int Ldx,int Ldy,int Ldz);
static Light=0;
int HiStyle;
extern bool ChangeNation;
extern bool MultiTvar;
extern DPID MyDPID;
extern DPID ServerDPID;
extern int curdx;
extern int curdy;
void Loadtextures();
extern bool CheapMode;
extern int WarSound;
extern int WorkSound;
extern int OrderSound;
extern int MidiSound;
extern int FPSTime;
extern int NMyUnits;
extern int NThemUnits;
void LoadNewAimations();
void MFix();
void WinnerControl(bool);
bool Fixed;
bool MUSTDRAW;
bool SHOWSLIDE=true;
int  HISPEED=0;
int Shifter;
int Multip;
bool MiniMode;
int AutoTime;
extern int sfVersion;
void OnMouseMoveRedraw();
void GetForces();
bool HealthMode;
#define TIMER_ID        1
#define TIMER_RATE      50
extern int MenuType;
extern bool MakeMenu;
extern byte SpecCmd;
extern int RealLx;
extern int RealLy;
extern bool BuildMode;
extern bool CINFMOD;
extern word rpos;
extern bool realLpressed;
extern bool realRpressed;
extern int MaxAllowedComputerAI;
extern bool GetCoord;
extern int FogMode;
extern CDirSound* CDS;
extern int RealPause;
void ProcessUFO();
void SaveGame(char* fnm,char* gg,int ID);
SFLB_DLLEXPORT SFLB_LoadGame(char* fnm,bool LoadNation);
void makeFden();
bool FullMini=true;
bool MiniMapVisible=1;
//byte Quality;
bool FASTMODE;
SFLB_DLLEXPORT InitDialogs();
int processMainMenu();
//+�� �� ��v ��
typedef void EventHandPro(void*);
void HandleMultiplayer();
bool EgoFlag;
int tmtmt;
int REALTIME; // game frame count
int TrueTime; // equal GetTickCount()
int AnimTime; // time linear to TrueTime 256/RequidMsPerFrame
int RealAnimTime=0;
bool RealAnimTimeIsRun=true;
bool MEditMode;
bool EditMapMode;
int HeightEditMode;
bool HelpMode;
bool ChoosePosition;
extern CDirSound CDIRSND;
extern BordersScope BScope;
int  ReliefBrush;
int  CostThickness;
int  TerrBrush;
int BlobMode;
int MaxSizeX;
int MaxSizeY;
byte EditMedia;
extern int curptr;
void InitDestn();
word Creator;
byte PlayerMask;
City CITY[8];

/*City CITY1;
City CITY2;
City CITY3;
City CITY4;
City CITY5;
City CITY6;
City CITY7;*/
static word MsPerFrame=40;
int Flips;
DIALOGS_API int LastKey;
DIALOGS_API bool KeyPressed;
int NMONS;
bool DeathMode;
bool AttackMode;
bool InfoMode;
int WaterEditMode;
void SetEgoFlag(bool EgoMod);
extern word PlayerMenuMode;
WIN32_FIND_DATA findfi;
struct EventsTag{
	EventHandPro* Pro;
	int	Type;
	int	Handle;
	bool Blocking;
	void* Param;
};
#define maxTask 32
void PlayerMenuWork();
int GetResID(char*);
EventsTag Events[maxTask];
void CmdSetSpeed(word Speed);
int RegisterEventHandler(EventHandPro* pro,int Type,void* param){
	for(int i=0;int(Events[i].Pro)&&i<maxTask;i++);
	if(i>=maxTask)return -1;
	Events[i].Pro=pro;
	Events[i].Type=Type;
	Events[i].Handle=i;
	Events[i].Blocking=false;
	Events[i].Param=param;
	return i;
};
void CloseEventHandler(int i){
	memset(&Events[i],0,sizeof Events[i]);
};

HWND                hwnd;
HWND g_hWnd; // by SERGYI
int tima;
int tmtim;
/*
 * Timer Callback 
 */
int xxx;
void ShowFon1();
void WaterCorrection();
extern bool TexMapMod;
extern bool RiverEditMode;
void ClearCurve();
extern bool TexPieceMode;
extern int DrawPixMode;
void SetRoadsEditMode(bool Rmode);
void ClearWallsEditMode();
void ClearModes(){
	DrawPixMode=0;
	DrawGroundMode=0;
	HeightEditMode=false;
	MEditMode=false;
	LockMode=0;
	WaterEditMode=false;
	TexMapMod=false;
	RiverEditMode=0;
	ClearCurve();
	TexPieceMode=0;
	SetRoadsEditMode(0);
	ClearWallsEditMode();
	ext_ClearActivityOfEditor();
};
void TimerProc(void)
{
	
	//LockSurface();
	//FlipPages();
	
	if(PlayerMenuMode==1){
		ShowFon1();
		for(int j=0;j<maxTask;j++)
		if(Events[j].Pro)(*(Events[j].Pro))(Events[j].Param);
		SetRLCWindow(0,1,MaxSizeX,RSCRSizeY-1,ScrWidth);
		//-ShowMPtr();
	}else{
		//ShowMPtr();
		//ShowMap();
		SetRLCWindow(0,1,MaxSizeY,RSCRSizeY-1,ScrWidth);
		//-ShowMPtr();
	};
	HandleMouse(mouseX,mouseY);
	MFix();
	FlipPages();
	//UnlockSurface();
	
	
}
//Loading...
void LoadEconomy();
void LoadNations();
void LoadWeapon();
void LoadNation(char* fn,byte msk,byte NIndex);
void LoadAllNewMonsters();
void InitNewMonstersSystem();
void LoadWaveAnimations();
extern NewAnimation* Shar;
void DoGen();
void InitDeathList();
GFILE* F=NULL;
void OpenMemReport(){
	//F=Gopen("mem.lst","w");
};
void CloseMemReport(){
	//Gclose(F);
	F=NULL;
};
void MemReport(char* str){
	/*
	if(!F)OpenMemReport();
	_CrtMemState M_state;
	_CrtMemCheckpoint(&M_state);
	Gprintf(F,"%s : %d\n" ,str,M_state.lSizes[1]);
	*/
};
char* GetTextByID(char* ID);
void LoadBorders();
UINT64 GETFREE(char* lpszPath){
	void* pGetDiskFreeSpaceEx;
	pGetDiskFreeSpaceEx=GetProcAddress(
	GetModuleHandle("kernel32.dll"),
		"GetDiskFreeSpaceExA");
	if (pGetDiskFreeSpaceEx){
		CHAR szTemp[4];
		ULARGE_INTEGER ForCaller,Total;
		ZeroMemory(szTemp,4);
		strncpy(szTemp,lpszPath,3);
  		GetDiskFreeSpaceEx(szTemp,&ForCaller,&Total,NULL);
		return (UINT64)ForCaller.QuadPart;
	}else{
		DWORD
			dwSectPerClust, 
            dwBytesPerSect,
            dwFreeClusters, 
            dwTotalClusters;

		CHAR szTemp[4];
		ZeroMemory(szTemp,4);
		strncpy(szTemp,lpszPath,2);
		GetDiskFreeSpace (szTemp, 
            &dwSectPerClust, 
            &dwBytesPerSect,
            &dwFreeClusters, 
            &dwTotalClusters);
		return (UINT64)(dwSectPerClust*dwBytesPerSect*dwFreeClusters);
	};
};
extern int ddd;
void SetupArrays();
extern byte* RivDir;
void ReadClanData();
void ReadMoraleData();
void LoadCTG();
void ReadChainObjectsDesc();
class OneTrigger;
void LoadVideoSeq(OneTrigger* OT, char* FileName);
class OneScript;
void LoadVideoSeqScript(OneScript* OS, char* FileName);
void CheckMemoryIntegrity();
extern OneTrigger VideoSeq;
extern OneTrigger VidOfSeq;
extern OneScript VideoSeqScript;
extern OneScript VidOfSeqScript;
extern OneScript SoundEvents;
void RegZG_Children();
bool Loading()
{	
	void InitTools();
	InitTools();
	UndoMaster::Init();
	void do3D();
	do3D();
	void CheckEngineSumm();
	CheckEngineSumm();
	void InitTexColors();
	InitTexColors();
	bool LoadBoidsParams();
	LoadBoidsParams();
	LoadCTG();
	ReadMoraleData();
	ReadClanData();
	//SetupArrays();
	RivDir=NULL;
	MemReport("Start");
	DWORD SectorsPerCluster;
	DWORD BytesPerSector; 
	DWORD NumberOfFreeClusters; 
	DWORD TotalNumberOfClusters;
	char CRDIR[200];
	GetWindowsDirectory(CRDIR,200);
	CRDIR[3]=0;
	
	void ShowMemLimit();
	//GetDiskFreeSpaceEx(CRDIR,&SectorsPerCluster,
	//							&BytesPerSector, 
	//							&NumberOfFreeClusters,
	//							&TotalNumberOfClusters);
	bool waserror=0;
	if(GETFREE(CRDIR)<500000000){
		waserror=1;
		//if(MessageBox( hwnd, GetTextByID("#MB_01"), GetTextByID("#MB_02"), MB_RETRYCANCEL )==IDCANCEL)
			//return 0;
		ShowMemLimit();
	};
	GetCurrentDirectory(200,CRDIR);
	CRDIR[3]=0;
	//GetDiskFreeSpace(CRDIR,&SectorsPerCluster,
	//							&BytesPerSector, 
	//							&NumberOfFreeClusters,
	//							&TotalNumberOfClusters);
	if(GETFREE(CRDIR)<500000000&&!waserror){
		//if(MessageBox( hwnd, GetTextByID("#MB_01"), GetTextByID("#MB_02"), MB_RETRYCANCEL)==IDCANCEL)
			//return 0;
		ShowMemLimit();
	};
	void ReadLayers();
	ReadLayers();
	InitDeathList();
	MemReport("InitDeathList");	
#ifdef COSSACKS2
	ReadChainObjectsDesc();
#endif //COSSACKS2
	//int GPID=GPS.PreLoadGPImage("interf3\\Fonts\\FontC10");	
	
	MemReport("InitNewMonstersSystem");
	MemReport("LoadMessages");
	LoadNations();
	MemReport("LoadNations");
	LoadFon();
	MemReport("LoadFon");	

	GoldID=GetResID("GOLD");
	FoodID=GetResID("FOOD");
	StoneID=GetResID("STONE");
	TreeID=GetResID("WOOD");
	CoalID=GetResID("COAL");
	IronID=GetResID("IRON");
	MoneyID=GetResID("MONEY");

	LoadEconomy();
	MemReport("LoadEconomy");

	Loadtextures();
	MemReport("LoadTextures");
	LoadFog(1);
	MemReport("LoadFog");
	LoadTiles();
	MemReport("LoadTiles");
	//LoadWalls();
	LoadLock();
	MemReport("LoadLock");
	//LoadMap("map13h.mpf");
	LoadPointer("cursor_y.rlc");
	MemReport("LoadPointer");
	LoadNewAimations();
	MemReport("LoadNewAnimations");
	LoadWeapon();
	MemReport("LoadWeapon");
	InitExplosions();
	MemReport("InitExplosions");
    InitSprites();
	MemReport("InitSprites");
	//DoGen();
	MemReport("DoGen");

	REG_CLASS(OneBoundEffect);
	REG_CLASS(LifeDependentEffect);
	REG_CLASS(MoveDependentEffect);	

	void RegSelEditor();
	RegSelEditor();	
	ext_BeforeLoadMD();

	REG_CLASS(MassSprite);
    REG_CLASS(CommonParameters);

	LoadAllNewMonsters();
	MemReport("LoadAllNewMonsters");
	LoadWaveAnimations();
#ifdef GERMANVERSION
	ResFile F=RReset("interf3\\Fonts\\FontC10");
	int sz=RFileSize(F);
	byte* buf=znew(byte,sz);
	RBlockRead(F,buf,sz);
	RClose(F);
#endif

	void InitInterfaceSystem();
	InitInterfaceSystem();

	MemReport("LoadWaveAnimations");
	LoadAllNations(1,0);
	MemReport("LoadAllNations0");
	LoadAllNations(2,1);
	MemReport("LoadAllNations1");
	LoadAllNations(4,2);
	MemReport("LoadAllNations2");
	LoadAllNations(8,3);
	MemReport("LoadAllNations3");
	LoadAllNations(16,4);
	MemReport("LoadAllNations4");
	LoadAllNations(32,5);
	MemReport("LoadAllNations5");
	LoadAllNations(64,6);
	MemReport("LoadAllNations6");
	LoadAllNations(128,7);
	MemReport("LoadAllNations7");
#ifdef GERMANVERSION
	int SM=0;
	for(int p=0;p<sz;p++)SM+=buf[p];
	free(buf);
#endif    

    CITY[0].CreateCity(0);
	MemReport("CreateCity0");
	CITY[1].CreateCity(1);
	MemReport("CreateCity1");
	CITY[2].CreateCity(2);
	MemReport("CreateCity2");
	CITY[3].CreateCity(3);
	MemReport("CreateCity3");
	CITY[4].CreateCity(4);
	MemReport("CreateCity4");
	CITY[5].CreateCity(5);
	MemReport("CreateCity5");
	CITY[6].CreateCity(6);
	MemReport("CreateCity6");
	CITY[7].CreateCity(7);
	MemReport("CreateCity7");		
	LoadPalettes();
	MemReport("LoadPalettes");
	InitPrpBar();
	MemReport("InitPrpBar");
	SetMyNation(0);
#ifdef GERMANVERSION
	ddd=SM-1101909;
#endif
	//InitSafeInfo();
	MemReport("InitSafeInfo");
	//InitSuperMortiraCells();
	MemReport("InitSuperMortiraCells");
	//CreateEnmBuildList();
	MemReport("CreateEnmBuildList");
	//CreateProtectionMap();
	FormationStr=GetTextByID("FORMATION");
	LoadVideoSeq(&VideoSeq,"Video\\vseq.dat");
	LoadVideoSeq(&VidOfSeq,"Video\\vofseq.dat");
	
	//InitL3();	

	REG_CLASS(StdBorder);
	REG_CLASS(ActiveGroup);
	REG_CLASS(ActiveZone);
	
	REG_CLASS(PlayGameProcess);
	REG_CLASS(StartTacticalAI);
	REG_CLASS(PreviewBinkVideo);		

	RegisterAllVictoryConditions();
	
	REG_LOADSAVE_FOR_CLASS(ClassArray<SimpleDialog>,"Dialogs\\","*.dss.xml");
	REG_LOADSAVE_FOR_CLASS(NewAnimation,"Dialogs\\","*.anm.xml");

	void RegEffectClass();
	RegEffectClass();
	RegZG_Children();
	RegMissionScriptClass();				//Vitya //  [11/4/2003]
	CIMPORT void RegisterDipClasses();
	RegisterDipClasses();	
	
	LoadBorders();
	void InitWalls();
	InitWalls();
	void RegisterPreviewers();
	RegisterPreviewers();
	
	void IstalAllExtensions();
	IstalAllExtensions();
	ext_OnClassRegistration();
	void PrintAllLeaks();
	//PrintAllLeaks();
	//	
	LoadVideoSeqScript(&VideoSeqScript,"Video\\vseq.qpp.xml");
	LoadVideoSeqScript(&VidOfSeqScript,"Video\\vofseq.qpp.xml");
	LoadVideoSeqScript(&SoundEvents,"Sound\\events.qpp.xml");

	void InsertThereRegistrationOfDifferentEditors();
	InsertThereRegistrationOfDifferentEditors();

    bool LoadAbilities(char* FileName);
    LoadAbilities("UnitsAbility\\base.ua.xml");
    bool LoadAllWeaponModificators(char* FileName);
    LoadAllWeaponModificators("WeaponSystem\\base.ws.xml");

	return 1;
};
extern int CurPalette;
/*void SaveScreenShot(char* Name){
#ifdef _USE3D
	extern IRenderSystem* IRS;
	IRS->SaveTexture(0,Name);
	return;
#endif
	byte PAL[1024];
	memset(PAL,0,1024);
	char ccx[120];
	sprintf(ccx,"%d\\agew_1.pal",CurPalette);
	ResFile f=RReset(ccx);
	for(int i=0;i<256;i++){
		int ofs=i<<2;
		RBlockRead(f,PAL+ofs+2,1);
		RBlockRead(f,PAL+ofs+1,1);
		RBlockRead(f,PAL+ofs,1);
	};
	RClose(f);
	f=RRewrite(Name);
	i=0x4D42;
	RBlockWrite(f,&i,2);
	i=RealLx*RealLy+1080;
	RBlockWrite(f,&i,4);
	i=0;
	int j=0x436;
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&j,4);
	j=0x28;
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&RealLx,4);
	RBlockWrite(f,&RealLy,4);
	j=0x080001;
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&i,4);
	j=0x0B12;
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,PAL,1024);
	for(i=0;i<RealLy;i++){
		char* pos=(char*)(int(ScreenPtr)+(RealLy-i-1)*SCRSizeX);
		RBlockWrite(f,pos,RealLx);
	};
	RClose(f);
};*/
void SaveBMP8(char* Name,int lx,int ly,byte* Data){
	byte PAL[1024];
	memset(PAL,0,1024);
	char ccc[128];
	sprintf(ccc,"%d\\agew_1.pal",CurPalette);
	ResFile f=RReset(ccc);
	for(int i=0;i<256;i++){
		int ofs=i<<2;
		RBlockRead(f,PAL+ofs+2,1);
		RBlockRead(f,PAL+ofs+1,1);
		RBlockRead(f,PAL+ofs,1);
	};
	RClose(f);
	f=RRewrite(Name);
	i=0x4D42;
	RBlockWrite(f,&i,2);
	i=lx*ly+1080;
	RBlockWrite(f,&i,4);
	i=0;
	int j=0x436;
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&j,4);
	j=0x28;
	RBlockWrite(f,&j,4);
	int LX=lx;
	int LY=ly;
	RBlockWrite(f,&LX,4);
	RBlockWrite(f,&LY,4);
	j=0x080001;
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&i,4);
	j=0x0B12;
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,PAL,1024);
	for(i=0;i<LY;i++){
		char* pos=(char*)(int(Data)+(ly-i-1)*lx);
		RBlockWrite(f,pos,lx);
	};
	RClose(f);
	
};
void SaveMiniScreenShot(char* Name){
	byte PAL[1024];
	memset(PAL,0,1024);
	ResFile f=RReset("agew_1.pal");
	for(int i=0;i<256;i++){
		int ofs=i<<2;
		RBlockRead(f,PAL+ofs+2,1);
		RBlockRead(f,PAL+ofs+1,1);
		RBlockRead(f,PAL+ofs,1);
	};
	RClose(f);
	f=RRewrite(Name);
	i=0x4D42;
	RBlockWrite(f,&i,2);
	i=RealLx*RealLy+1080;
	RBlockWrite(f,&i,4);
	i=0;
	int j=0x436;
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&j,4);
	j=0x28;
	RBlockWrite(f,&j,4);
	int LX=RealLx>>2;
	int LY=RealLy>>2;
	RBlockWrite(f,&LX,4);
	RBlockWrite(f,&LY,4);
	j=0x080001;
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&i,4);
	j=0x0B12;
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&j,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,&i,4);
	RBlockWrite(f,PAL,1024);
	for(i=0;i<LY;i++){
		char* pos=(char*)(int(ScreenPtr)+(RealLy-i*4-1)*SCRSizeX);
		for(int j=0;j<LX;j++)RBlockWrite(f,pos+j*4,1);
	};
	RClose(f);
};
/*void SaveScreen(){
	char ccc[128];
	CreateDirectory("Screenshots",0);
	for(int i=0;i<1000;i++){
		sprintf(ccc,"Screenshots\\screen%d.bmp",i);
		ResFile f=RReset(ccc);
		if(f==INVALID_HANDLE_VALUE){
			RClose(f);
			goto zzz;
		};
		RClose(f);
	};
	i=99;
zzz:
	sprintf(ccc,"Screenshots\\screen%d.bmp",i);
	SaveScreenShot(ccc);
};*/
extern bool NoText;
extern bool SHOWSLIDE;
void GFieldShow();
void SaveMiniScreenShot(char* Name);
void MiniRenderAllMap(){
	SHOWSLIDE=true;
	int nx=div(msx,smaplx).quot;
	int ny=div(msy,smaply).quot;
	//if(nx>3)nx=3;
	//if(ny>3)ny=3;
	char ccc[128];
	NoText=true;
	for(int y=0;y<ny;y++){
		for(int x=0;x<nx;x++){
			mapx=x*smaplx;
			mapy=y*smaply;
			PRFUNC(GFieldShow());
			FlipPages();
			int p=x+y*nx;
			if(p<10)sprintf(ccc,"scr00%d.bmp",p);
			else if(p<100)sprintf(ccc,"scr0%d.bmp",p);
			else sprintf(ccc,"scr%d.bmp",p);
			SaveMiniScreenShot(ccc);
		};
	};
	NoText=false;
};

//-----------------------------------------------------------------------------
// PrepareScreenShotFile : void(char *)
//-----------------------------------------------------------------------------
void PrepareScreenShotFile(char *pFileName) {
	CreateDirectory("ScreenShots", NULL);
	
	FILE *pFile = NULL;

	// Looking for the next free file:
	for(int i = 0; i < 999; i++) {
		sprintf(pFileName, "ScreenShots\\Screen%.3d.bmp", i);
		pFile = fopen(pFileName, "r");
		if(!pFile) {
			break;
		}
		fclose(pFile);
	}
}

//-----------------------------------------------------------------------------
// ScreenShot : void()
//-----------------------------------------------------------------------------
void ScreenShot() {
	char FileName[128];
	PrepareScreenShotFile(FileName);
	
	IRS->ScreenShotBMP(FileName);
	g_Messager.ShowMessage(FileName);
}// ScreenShot

//-----------------------------------------------------------------------------
// RenderAllMap
//-----------------------------------------------------------------------------
void RenderAllMap() {
	char Text[] = "Are you sure you want to render all map in screenshot?";
	char Caption[] = "Confirm Render All Map Screenshot";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) != IDYES) return;

	extern int g_ExtraViewVolume;
	g_ExtraViewVolume = 4000;
	OrthoCameraMode::Enable();

	SHOWSLIDE = true;
	int Nx = div(msx, smaplx).quot;
	int Ny = div(msy, smaply).quot;
	NoText = true;

	int W = GetSystemMetrics(SM_CXSCREEN);
	int H = GetSystemMetrics(SM_CYSCREEN);
	const int k = g_SurfEditor.RenderAllMapZoomOut;
	int BigW = W / k * Nx;
	int BigH = H / k * Ny;

	cDib24 BigDib(BigW, BigH);

	extern float fMapX;
	extern float fMapY;
	extern IRenderSystem *IRS;
	const char FileName[] = "$$$TempScreenShot$$$.bmp";

	cDib24 dib;


	PRFUNC(GFieldShow());
	FlipPages();
	extern bool g_SetupCameraLimitCamera;
	g_SetupCameraLimitCamera = false;
	const int nRepeats = 10;
	for(int iy = 0; iy < Ny; iy++) {
		for(int ix = 0; ix < Nx; ix++) {
			fMapX = ix * smaplx + smaplx / 2;
			fMapY = iy * smaply + smaply / 2;
			for(int d = 0; d < nRepeats; d++) {
				PRFUNC(GFieldShow());
				FlipPages();
				if(d == nRepeats - 1) { // Save only last time:
					IRS->ScreenShotBMP(FileName);
					if(!dib.LoadBMP(FileName)) {
						break;
					}
					dib.Scale(k);
					BigDib.Paste(ix * dib.GetWidth(), iy * dib.GetHeight(), dib);
				}
				if(GetKeyState(VK_ESCAPE) < 0) {
					ix = Nx;
					iy = Ny;
				}
			}
		}
	}

	OrthoCameraMode::Disable();
	g_ExtraViewVolume = 0;

	char Map[128];
	PrepareScreenShotFile(Map);
	for(int y=0;y<BigH;y++){
		for(int x=0;x<BigW;x++){
			cDib24::Pixel P=BigDib.GetPixel(x,y);
			swap(P.b,P.r);
			BigDib.SetPixel(x,y,P);
		}
	}
	BigDib.SaveBMP(Map);
	NoText = false;

	g_Messager.ShowMessage(Map);
	g_SetupCameraLimitCamera = true;
};
/*
 * finiObjects
 *
 * finished with all objects we use; release them
 */
static void finiObjects( void )
{
   FreeDDObjects();
} /* finiObjects */


void MyMCINotify(HWND hwnd,WPARAM wParam,LPARAM lParam)
{
  if((wParam==MCI_NOTIFY_SUCCESSFUL)&&(lParam==LPARAM(wAVIDeviceID)))
  {
	  //CloseAVI(hwnd);
	  Lpressed=true;
  }else
  if((wParam==MCI_NOTIFY_SUCCESSFUL)&&(lParam==LPARAM(wMIDDeviceID)))
  {
	  //CloseMIDI(hwnd);
  }else
  if(wParam==MCI_NOTIFY_SUCCESSFUL){
	  mciSendCommand(lParam,MCI_CLOSE,0,0);
  }
};

#define MaxQu 32
MouseStack MSTC[MaxQu];
MouseStack CURMS;
int NInStack=0;
void AddMouseEvent(int x,int y,bool L,bool R){
	if(NInStack<MaxQu){
		MSTC[NInStack].x=x;
		MSTC[NInStack].y=y;
		MSTC[NInStack].Lpressed=L;
		MSTC[NInStack].Rpressed=R;
		MSTC[NInStack].rLpressed=L;
		MSTC[NInStack].rRpressed=R;
		MSTC[NInStack].Control=(GetKeyState(VK_CONTROL)&0x8000)!=0;
		MSTC[NInStack].Shift=(GetKeyState(VK_SHIFT)&0x8000)!=0;
		NInStack++;
	};
};
int LastUMX=0;
int LastUMY=0;
int LastUTime=0;
MouseStack* ReadMEvent(){
	if(NInStack){
		CURMS=MSTC[0];
		if(NInStack>1)memcpy(MSTC,MSTC+1,(NInStack-1)*sizeof MouseStack);
		NInStack--;
		return &CURMS;
	};
	return NULL;
};
void ClearMStack(){
	NInStack=0;
};
void SetStateToLastPress(){
	///return;
	MouseStack* MEV;
	do{
        MEV=ReadMEvent();
		if(MEV){
			if(MEV->Lpressed||MEV->Rpressed){
				mouseX=MEV->x;
				mouseY=MEV->y;
				Lpressed=MEV->Lpressed;
				Rpressed=MEV->Rpressed;
			}
		}
	}while(MEV);
}
extern bool unpress;
extern bool IgnoreLUnpress;
CEXPORT
void UnPress(){
	for(int i=0;i<NInStack;i++){
		MSTC[i].Lpressed=0;
		MSTC[i].Rpressed=0;
	};
	unpress=1;
	Lpressed=0;
	Rpressed=0;
	//realLpressed=false;
	//realRpressed=false;
	ClearMStack();
	IgnoreLUnpress=1;
};
extern int CurPalette;
LRESULT CD_MCINotify(WPARAM wFlags, LONG lDevId);
int SHIFT_VAL=0;
void HandleMouse(int x,int y);
extern bool PalDone;
byte KeyStack[32];
byte AsciiStack[32];
int NKeys=0;
//int KeyPos=0;
byte LastAsciiKey=0;
void AddKey(byte Key,byte Ascii){
	if(NKeys>=32){
		memcpy(KeyStack,KeyStack+1,31);
		memcpy(AsciiStack,AsciiStack+1,31);
		NKeys--;
	};
	KeyStack[NKeys]=Key;
	AsciiStack[NKeys]=Ascii;
	NKeys++;
};
byte LastAscii=0;
int ReadKey(){
	if(NKeys){
		byte c=KeyStack[0];
		LastAscii=AsciiStack[0];
		if(NKeys){
			memcpy(KeyStack,KeyStack+1,NKeys-1);
			memcpy(AsciiStack,AsciiStack+1,NKeys-1);
		};
		NKeys--;
		return c;
	}else return -1;
};
void ClearKeyStack(){
	NKeys=0;
};
extern bool GUARDMODE;
extern bool PATROLMODE;
extern byte NeedToPopUp;
short WheelDelta=0;
CEXPORT int GetWheelDelta(bool clear){
	int d=WheelDelta;
	if(clear)WheelDelta=0;
	return d;
}
void IAmLeft();
void LOOSEANDEXITFAST();
extern bool DoNewInet;
bool ReadWinString(GFILE* F,char* STR,int Max);
extern bool RUNMAPEDITOR;
extern bool RUNUSERMISSION;
extern char USERMISSPATH[128];
void OnWTPacket(WPARAM wSerial, LPARAM hCtx);
bool IgnoreLUnpress=0;
bool IgnoreRUnpress=0;

extern bool g_bDrawKangaroo;

bool KangarooProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (!g_bDrawKangaroo) return false;
	switch( message )
	{
        case WM_LBUTTONDBLCLK:
            IInput->MouseLBDbl( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_MBUTTONDBLCLK:
            IInput->MouseMBDbl( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_RBUTTONDBLCLK:
            IInput->MouseRBDbl( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_RBUTTONDOWN:
            IInput->MouseRBDown( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_RBUTTONUP:
            IInput->MouseRBUp( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_LBUTTONDOWN:
            IInput->MouseLBDown( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_LBUTTONUP:
            IInput->MouseLBUp( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_MBUTTONDOWN:
            IInput->MouseMBDown( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_MBUTTONUP:
            IInput->MouseMBUp( LOWORD( lParam ), HIWORD( lParam ) );
            break;

        case WM_MOUSEMOVE:
            IInput->MouseMove( LOWORD( lParam ), HIWORD( lParam ), wParam );
            break;

        case WM_MOUSEWHEEL:
            {
                short WheelDelta = (short) HIWORD( wParam );
                IInput->MouseWheel( WheelDelta );
            }break;

        case WM_KEYDOWN:
            IInput->KeyDown( wParam, lParam );
            break;

        case WM_CHAR:
            IInput->Char( wParam, lParam );
            break;

        case WM_KEYUP:
            IInput->KeyUp( wParam, lParam );
            break;

        case WM_ERASEBKGND: return TRUE;
	}
	return true;
} // KangarooProc

bool g_fMMBIsDown = false;
bool g_fSpaceIsBlocked = false;
bool g_WASDKeysAreBlocked = false;
bool g_ESDFKeysAreBlocked = false;
bool g_fZXCKeysAreBlocked = false;
bool g_WERKeysAreBlocked = false; // Another hack to complete the twisted line of keys blocking flags...

long FAR PASCAL WindowProc( HWND hWnd, UINT message, 
                            WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps; 
    RECT        rc;
    static BYTE phase = 0;
//    HDC         hdc;
	if(ext_OnWindowsMessage(EditMapMode,hWnd,message,wParam,lParam))return 0;
	if (KangarooProc(hWnd, message, wParam, lParam)) return DefWindowProc(hWnd, message, wParam, lParam);
	if(g_CollidingCamera.CameraMessage(hWnd, message, wParam, lParam)) return 0;
    if(g_TankController.CameraMessage(hWnd, message, wParam, lParam)) return 0;

    switch( message )
    {
	case WT_PACKET:
		OnWTPacket(wParam,lParam);
		break;

	case 0xABCD:
		{
			GFILE* F=Gopen("UserMissions\\start.dat","r");
			if(F){
				ReadWinString(F,USERMISSPATH,120);
				Gclose(F);
				if(lParam==1)RUNMAPEDITOR=1;
				if(lParam==0)RUNUSERMISSION=1;
			};
		};
		break;
	case 0x020A://WM_MOUSEWHEEL:
		WheelDelta = (short)HIWORD(wParam);
		if(ext_OnMouseWheel(WheelDelta)){
			WheelDelta = 0;
		};
#ifdef COSSACKS2
		if(WheelDelta<0){
			SpecCmd=173;
		};
		if(WheelDelta>0){
			SpecCmd=174;
		};
#endif //COSSACKS2

		break;
	case MM_MCINOTIFY:
		MyMCINotify(hWnd,wParam,lParam);
		CD_MCINotify(wParam,lParam);
		break;
	case WM_LBUTTONDOWN:
		IgnoreLUnpress=0;
		wParam=wParam|MK_LBUTTON;
		Lpressed=true;
		realLpressed=true;
		Fixed=false;
		//xLpressed=true;
		SetMPtr(LOWORD(lParam),HIWORD(lParam),wParam);
		AddMouseEvent(mouseX,mouseY,Lpressed,Rpressed);
		//HandleMouse(mouseX,mouseY);

		break;
	case WM_LBUTTONUP:		
		wParam=wParam&!MK_LBUTTON;
		if(Fixed)Lpressed=false;
		realLpressed=false;
		//xLpressed=false;
		SetMPtr(LOWORD(lParam),HIWORD(lParam),wParam);
		if(!IgnoreLUnpress){
			AddMouseEvent(mouseX,mouseY,1,Rpressed);
			extern bool MouseOverMiniMap;
			extern bool NoPress;
			if(MouseOverMiniMap==false && NoPress==false 
				&& (abs(mouseX-LastUMX)+abs(mouseY-LastUMY))<16 
				&& GetTickCount()-LastUTime<600	)SpecCmd=241;
		}
		AddMouseEvent(mouseX,mouseY,0,Rpressed);
		//HandleMouse(mouseX,mouseY);		
		LastUMX=mouseX;
		LastUMY=mouseY;
		LastUTime=GetTickCount();
		break;
	case WM_RBUTTONDOWN:		
		IgnoreRUnpress=0;
		wParam=wParam|MK_RBUTTON;
		Rpressed=true;
		realRpressed=true;
		Fixed=false;
		//xRpressed=true;
		if(ScreenPtr)SetMPtr(LOWORD(lParam),HIWORD(lParam),wParam);
		AddMouseEvent(mouseX,mouseY,Lpressed,0);
		AddMouseEvent(mouseX,mouseY,Lpressed,1);
		//HandleMouse(mouseX,mouseY);
		break;
	case WM_RBUTTONUP:		
		wParam=wParam&!MK_RBUTTON;
		Rpressed=false;
		if(Fixed)Rpressed=false;
		realRpressed=false;
		//xRpressed=false;
		if(ScreenPtr)SetMPtr(LOWORD(lParam),HIWORD(lParam),wParam);
		AddMouseEvent(mouseX,mouseY,Lpressed,Rpressed);
		//HandleMouse(mouseX,mouseY);
		break;
	case WM_MBUTTONDOWN:
		g_fMMBIsDown = true;
		break;
	case WM_MBUTTONUP:
		g_fMMBIsDown = false;
		break;
	case WM_MOUSEMOVE:
		if(LOWORD(lParam)!=mouseX||HIWORD(lParam)!=mouseY){
			SetMPtr(LOWORD(lParam),HIWORD(lParam),wParam);
			OnMouseMoveRedraw();
		};
		break;
	case WM_LBUTTONDBLCLK:
		//SpecCmd=241;
		break;
		case WM_RBUTTONDBLCLK:
		break;

    case WM_ACTIVATEAPP:
		extern int PCursorDevice;
		PCursorDevice=-2;
        bActive = /*GetForegroundWindow()==hWnd;*/wParam;
		if(bActive){
			if(lpDDSPrimary){
				//LockSurface();
				//UnlockSurface();
				//FreeDDObjects();
				CreateDDObjects(hwnd); 
				LockSurface();
				UnlockSurface();
				LoadFog(CurPalette);
				char cc[64];
				sprintf(cc,"%d\\agew_1.pal",CurPalette);
				PalDone=0;
				LoadPalette(cc);
			};
		};
        break;

    case WM_CREATE:
		g_hWnd = hWnd;
        break;

    case WM_SETCURSOR:		
        return TRUE;

    case WM_TIMER:
        // Flip surfaces
        if( bActive )
        {
			//ShowMPtra();
			//TimerProc();
		}
        break;
	
	
    case WM_KEYDOWN:
		extern bool WasAtLeastOneTimeInInputBox;
		if(ext_OnKeyPressed(EditMapMode,WasAtLeastOneTimeInInputBox,wParam))break;
		LastKey = wParam;
		
		if(LastKey==' '&&(GetKeyState(VK_CONTROL)&0x8000)&&(GetKeyState(VK_MENU)&0x8000)&&(GetKeyState(VK_SHIFT)&0x8000)){						
			extern bool p_isRun;
			if(!p_isRun){
				void RunProfiler();
				RunProfiler();
				p_isRun=true;
			}else{
				p_isRun=false;
				void StopProfiler();
				StopProfiler();
				void ShowProfInfo();
				ShowProfInfo();
			}	
		}

		if(' ' == LastKey && g_fSpaceIsBlocked) break; // Hook for TerraTools.
		KeyPressed=true;
		GSets.LeibzigDemo.RecBroken=1;

		// ScreenShot:
		if(VK_F11 == LastKey) {
			if(GetKeyState(VK_CONTROL) < 0) {
				RenderAllMap();
			} else if(GetKeyState(VK_SHIFT) < 0) {
				g_HiResScreenShot.MakeScreenShot();
			} else {
				ScreenShot();
			}
		};

		//if((!GameInProgress)&&LastKey=='R'&&GetKeyState(VK_CONTROL)&0x8000)RecordMode=!RecordMode;
		{
			int nVirtKey = (int) wParam;    // virtual-key code 
			int lKeyData = lParam;    
			byte PST[256];
			GetKeyboardState(PST);
			word res;
			int s=ToAscii(nVirtKey,lKeyData,PST,&res,0);
			if(s==1){
				LastAsciiKey=res;
			}else LastAsciiKey=0;
			AddKey(wParam,LastAsciiKey);
		};

		break;
    case WM_PAINT:

        break;
	case WM_CLOSE:
		extern word NPlayers;
		extern bool InGame;
		if(NPlayers>1&&InGame&&tmtmt>40){
			void CmdEndGame(byte NI,byte state,byte cause);
			CmdEndGame(MyNation,1,132);			
		}
		if(DoNewInet){
			//IAmLeft();
			LOOSEANDEXITFAST();
		};
		return true;
    case WM_DESTROY:
		
        // ManOwaR :: Destroying theovorb helper thread
        tv_Done();

        finiObjects();
        PostQuitMessage( 0 );
		// For Silver
		// Please add restore screen mode here
		//
        IRS->ShutDown();
		// Quit exit without destructors
		_exit(EXIT_SUCCESS);
        break;
    //  added by Silver 17.06.04 to prevent screensaver enabling
    case WM_SYSCOMMAND:
        if (wParam == SC_MONITORPOWER || wParam == SC_SCREENSAVE) 
		{
			return TRUE;
		}
        break;
    }
    try{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}catch(...){
		ExitProcess(0);
		return 1;
	};
} /* WindowProc */

/*
 * doInit - do work required for every instance of the application:
 *                create the window, initialize data
 */
void ProcessGSaveMap();
void EditorKeyCheck();
void ProcessSaveInSquares();
bool ShowStatistics();
bool EnterChatMode=0;
char ChatString[128];
void ProcessChatKeys();
extern int WaitState;
bool RetryVideo=0;
extern byte PlayGameMode;
extern bool GameExit;
extern int LastCTRLPressTime;
bool CheckFNSend(int idx);
void ProcessVotingKeys();
extern bool RESMODE;
extern bool OptHidden;
extern word NPlayers;
bool CheckFlagsNeed();
void SetGameDisplayModeAnyway(int SizeX,int SizeY);
void ReverseLMode();
void FlipDipDialog();
extern bool DIP_DSS_Vis;
extern bool SHOWZONES;
extern byte InfoStyle;
CEXPORT
void SetCurPtr(int v);
extern int DipDlgDX;
extern int InfDX0;
extern int InfDX;

extern veGameMode vGameMode;
extern bool KeysAreFocusedOnInputBox;
extern bool WasAtLeastOneTimeInInputBox;

//bool g_fPauseOnEscape = false;

void GameKeyCheck(){
	if(KeysAreFocusedOnInputBox)return;
	if(PlayGameMode==1){
		if(KeyPressed&&LastKey){ //(GetKeyState(VK_ESCAPE)&0x8000)
			GameExit=true;
			RetryVideo=0;
			KeyPressed=0;
			return;
		};
	};
	ProcessVotingKeys();
	if(EnterChatMode){
		ProcessChatKeys();
		return;
	};
	if(EditMapMode){
		EditorKeyCheck();
		return;
	};
	if(KeyPressed){
		v_GlobalHotKeys.Process();
		KeyPressed=false;
		int wParam=LastKey;

		if(g_WASDKeysAreBlocked) {
			if('W' == wParam || 'A' == wParam || 'S' == wParam || 'D' == wParam) return;
		}
		if(g_ESDFKeysAreBlocked) {
			if('E' == wParam || 'S' == wParam || 'D' == wParam || 'F' == wParam) return;
		}
		if(g_WERKeysAreBlocked) {
			if('W' == wParam || 'E' == wParam || 'R' == wParam) {
				return;
			}
		}

			switch( wParam ){
				
		        case VK_ESCAPE:
					ClearModes();
					BuildMode=false;
					//BuildWall=false;
					GetCoord=false;
					curptr=0;
					curdx=0;
					curdy=0;
					PauseMode=0;
					SetDestMode=false;
					GoAndAttackMode=false;
					GUARDMODE=0;
					PATROLMODE=0;
					if(WaitState==1)WaitState=2;
					if(ShowGameScreen)ShowGameScreen=2;
					AttGrMode=0;
					//if(PlayerMenuMode==2)
					//MainMenu.ShowModal();
					//Options.ShowModal();
//					if(g_fPauseOnEscape) { // by SERGYI to enter main menu on ESC key.....
//						if(tmtmt > 32 && !LockPause) SpecCmd=137;
//					}
					break;
				//case 'G':
				//	smapx+=16;
				//	smaplx--;
				//	smapy+=16;
				//	smaply--;
				//	ShowFon();
				//	break;
				case 'H':
					//FullScreenMode=!FullScreenMode;
					//GameNeedToDraw=true;
					//GSSetup800();
					//ProcessGSaveMap();
					break;
				case 'E':
					//FastMode=!FastMode;
#ifndef NO_CHEATS_IN_MULTIPLAYER
					void SelectEditor();
					if(GetKeyState(VK_CONTROL)&0x8000)SelectEditor();
#else  //NO_CHEATS_IN_MULTIPLAYER
					void SelectEditor();
					if(NPlayers<=1
						&&(GetKeyState(VK_CONTROL)&0x8000)
						&&(GetKeyState(VK_MENU)&0x8000)
						&&(GetKeyState(VK_SHIFT)&0x8000))SelectEditor();
#endif //NO_CHEATS_IN_MULTIPLAYER
					break;
				case 192:
					break;
				case 8:					
					break;
				case 'V':
					/*
					switch(Light){
					case 0:SetLight(0,10,20);
						break;
					case 1:SetLight(0,12,17);
						break;
					case 2:SetLight(0,15,15);
						break;
					case 3:SetLight(0,17,12);
						break;
					case 4:SetLight(0,20,10);
						break;
					case 5:SetLight(0,17,12);
						break;
					case 6:SetLight(0,15,15);
						break;
					case 7:SetLight(0,12,17);
					};
					Light++;
					if(Light>7)Light=0;
					*/
					//SaveGame("save1.wsf","my game");
					break;
				case 'U':
					//g_Navigator.LinkToUnitToggle(); //gec demo only (for U)
					break;
					/*
					if(!GSets.LeibzigDemo.Enabled){
						if((GetKeyState(VK_CONTROL)&0x8000)&&
							(GetKeyState(VK_SHIFT)&0x8000)&&
							(GetKeyState(VK_MENU)&0x8000)){
								//ProcessDialogsEditor();
								void ProcessAIEditor();
								ProcessAIEditor();
								break;
						}else{
							if(!InfoStyle){
								if(Inform!=2)Inform=2;else Inform=0;
								MiniActive=0;
								DIP_DSS_Vis=0;
								InfDX=InfDX0;
								Recreate=1;
							};
						}
					}
					*/
					//break;
				case VK_OEM_4:
					if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=114;
					//MiniActive=0;
					//Recreate=1;
					break;
		       case VK_F12:
					//UnlockSurface();
					//CloseExplosions();
					//ShutdownMultiplayer();
		            //PostMessage(hwnd, WM_CLOSE, 0, 0);
#ifndef SELFRUN
				    MenuType=1;
					MakeMenu=true;
#endif
				    break;
				case VK_F1:
					/*
					if(!CheckFNSend(0)){
						MenuType=4;
						MakeMenu=true;
					};
					*/
					//Creator=0;
					break;
				case VK_F2:
					CheckFNSend(1);
					//
					break;
				case VK_F3:
					CheckFNSend(2);
					//
					break;
				case VK_F4:
					CheckFNSend(3);
					break;
				case VK_F5:
					CheckFNSend(4);
					break;
				case VK_F6:
					CheckFNSend(5);
					break;
				case VK_F7:
					CheckFNSend(6);
					break;
				case VK_F8:
					//CheckFNSend(7);
					//FlipDipDialog();
					break;
				case VK_F9:
					if(!CheckFNSend(8)){
						Creator=4096+255;
					};
					break;
				case 'R':
					//if((!GameInProgress)&&GetKeyState(VK_CONTROL)&0x8000)RecordMode=!RecordMode;
					break;				
				case VK_DELETE://VK_END://46: // Deleting selection:
					if(GetKeyState(VK_CONTROL)&0x8000){
						SpecCmd=200;
					}
					break;
				//case 'D':
				//	if(!(GetKeyState(VK_CONTROL)&0x8000)){
				//		if(!(GetKeyState(VK_SHIFT)&0x8000)){
				//			//FlipDipDialog();
				//		}else
				//		if((NPlayers<2||vGameMode==gmSin)){
				//			if((GetKeyState(VK_SHIFT)&0x8000)/*&&PlayGameMode*/){
				//				switch(HISPEED){
				//				case 0:
				//					HISPEED=1;
				//					break;
				//				case 1:
				//					HISPEED=2;
				//					break;
				//				case 2:
				//					HISPEED=3;
				//					break;
				//				default:
				//					HISPEED=0;
				//					break;
				//				};
				//			};
				//		};
				//	}else{
						//if(FrmDec==1)CmdSetSpeed(1+128);
						//else CmdSetSpeed(128);
				//	};
				//	break;
				case 'A':
					if(GetKeyState(VK_CONTROL)&0x8000){
						SpecCmd=1;
						LastKey=0;
					}
					break;
				case 'D':
					//SpecCmd=2;
					if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=201;
///#ifdef VIEW_TOP
					if( (GetKeyState(VK_CONTROL)&0x8000) 
						&& (GetKeyState(VK_MENU)&0x8000)
						&& (GetKeyState(VK_SHIFT)&0x8000)) SHOWZONES=!SHOWZONES;
//#endif //VIEW_TOP
					break;
				case 'W':
					break;
				case 'J':
					//RSCRSizeX++;
					/*
					if(PlayGameMode==2){
						int ExRX=RealLx;
						int ExRY=RealLy;
						if(RealLx!=1024||RealLy!=768)
							SetGameDisplayModeAnyway(1024,768);
						//ShowStatistics();
						if(RealLx!=ExRX||RealLy!=ExRY)SetGameDisplayModeAnyway(ExRX,ExRY);
					};
					*/
					break;
				case 'K':
					if(GetKeyState(VK_CONTROL)&0x8000)RealPause-=2;
					else RealPause+=2;
					//Quality++;
					//if(Quality>=4)Quality=0;
					//RSCRSizeX--;
					break;
				case 'N':
					//switch(RSCRSizeX){
					//	case 800:RSCRSizeX=1024;
					//			break;
					//	case 1024:RSCRSizeX=800+32;
					//			break;
					//	case 1280:RSCRSizeX=1600;
					//		break;
					//	case 1600:RSCRSizeX=800;
					//		break;
					//	default:
					//		RSCRSizeX=800;
					//};
					/*if(RSCRSizeX!=1024)RSCRSizeX=1024;
					else RSCRSizeX=800;*/
					//VHMode=!VHMode;
					break;
				case 'X':
					if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=5;
					break;
				//case 'F':

				//	//FASTMODE=true;
				//	break;
				case 'G':
					break;
				//	//FASTMODE=false;
				//	break;
				case 'C':
					if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=14;
					break;
				case 'Q':
					break;
				case 'B':
					if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=9;
					else SpecCmd=10;
					break;
				case 'Z':
					if(g_fZXCKeysAreBlocked) break;
					if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=11;
					else SpecCmd=241;
					LastKey=0;
					break;
				case 'F':
					if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=13;
					break;
				case VK_NUMPAD1:
					if(MEditMode)EditMedia=0;
					else{
#ifdef COSSACKS2
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(0)
						else SpecCmd=181;
#else 
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(0);
#endif
						PlayerMask=1;
						LastKey=0;
					};
					break;
				case VK_NUMPAD2:
					if(MEditMode)EditMedia=1;
					else{
#ifdef COSSACKS2
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(1)
						else SpecCmd=182;
#else
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(1);
#endif
						PlayerMask=2;
						LastKey=0;
					};
					break;
				case VK_NUMPAD3:
					if(MEditMode)EditMedia=2;
					else{
#ifdef COSSACKS2
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(2)
						else SpecCmd=183;
#else
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(2);
#endif
						PlayerMask=4;
						LastKey=0;
					};
					break;
				case VK_NUMPAD4:
					if(MEditMode)EditMedia=3;
					else{
#ifdef COSSACKS2		
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(3)
						else SpecCmd=184;
#else
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(3);
#endif
						PlayerMask=8;
						LastKey=0;
					};
					break;
				case VK_NUMPAD5:
					if(MEditMode)EditMedia=4;
					else{
#ifdef COSSACKS2
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(4)
						else SpecCmd=185;
#else
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(4);
#endif
						PlayerMask=16;
						LastKey=0;
					};
					break;
				case VK_NUMPAD6:
					if(MEditMode)BlobMode=1;
					else{
#ifdef COSSACKS2
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(5)
						else SpecCmd=186;
#else
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(5);
#endif
						PlayerMask=32;
						LastKey=0;
					};
					break;
				case VK_NUMPAD7:
					if(MEditMode)BlobMode=-1;
					else{
#ifdef COSSACKS2
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(6)
						else SpecCmd=187;
#else
						if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(6);
#endif
						PlayerMask=64;
						LastKey=0;
					};
					break;
				case VK_NUMPAD8:
#ifdef COSSACKS2
					if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(7)
					else SpecCmd=188;
#else
					if((NPlayers<2||vGameMode==gmSin)&&ChangeNation)SetMyNation(7);
#endif
					PlayerMask=128;
					LastKey=0;
					break;
#ifdef COSSACKS2
				case VK_NUMPAD9:
					SpecCmd=189;
					break;
#endif
				case VK_ADD:
					if(PlayGameMode==2){
                        if(HISPEED<5)HISPEED=5;
						else if(HISPEED<50)HISPEED=50;
						else HISPEED=1;
					}
					break;
				case VK_SUBTRACT:
					if(PlayGameMode==2){
						if(HISPEED>5)HISPEED=5;
						HISPEED=1;						
					}
					break;
				case 'I':
					
					if(!GSets.LeibzigDemo.Enabled){
						if((GetKeyState(VK_CONTROL)&0x8000)
							&&(GetKeyState(VK_MENU)&0x8000)
							&&(GetKeyState(VK_SHIFT)&0x8000)){
							if(!InfoStyle)InfoMode=!InfoMode;
						}
					}
					
					//InfoMode=1;
				break;
				case VK_CAPITAL:
					EgoFlag=!EgoFlag;
					SetEgoFlag(EgoFlag);
					break;
				case 'O':
					break;
				case 'P':
					//MsPerFrame++;
					//if(GetKeyState(VK_CONTROL)&0x8000)RotatePhiI();
					//RotatePhi();
					if(GetKeyState(VK_CONTROL)&0x8000){
						SpecCmd=113;
					}else{
					if(GetKeyState(VK_SHIFT)&0x8000){
						SpecCmd=3;
					}else
					if(MultiTvar){
						if(!InfoStyle){
							NeedToPopUp=2;
							Inform=0;
							InfDX=InfDX0;
						};
					};
					};
					break;
				case 19://Pause				
					if(tmtmt>32&&!LockPause) SpecCmd=137;
					//PauseMode=!PauseMode;
					break;
				case 13:
					//MakeMenu=true;
					//MenuType=3;
					if(!RESMODE){
						EnterChatMode=1;
						ClearKeyStack();
					};
					break;
/*				case 'L':					
					//LG.CreateLegionFromSelection(PlayerMask);
					//if(!MiniMode)SetMiniMode();
					//else ClearMiniMode();
					break;*/
				default:
					if(wParam>='0'&&wParam<='9'){						
						if(GetTickCount()-LastCTRLPressTime<150){							
							SpecCmd=50+wParam-'0';//CmdMemSelection(MyNation,wParam-'0');
						}else SpecCmd=60+wParam-'0';//CmdRememSelection(MyNation,wParam-'0'); 							
					};
			 };
	};
};
void CreateFastLocking();
void AddHill();
bool DelCurrentAZone();
void SelectNextGridMode();
void ProcessMapOptions();
void ResearchIslands();
void EnterRandomParams();
void GenerateRandomRoad(int idx);
bool CheckCurve();
void ClearCurve();
extern bool ToolsHidden;
void SetFractalTexture();
void AutoSMSSet();
void UpdateAllPieces();
void PerformPixUndo();
extern int PEN_RADIUS;
extern int PEN_BRIGHTNESS;
void LoadCurPixTexture(char* Name);
bool ManualTex=0;
void STBRR(char c){
	int v=c-'0';
	if(c=='0')v=10;
	if(v<1)v=1;
	if(v>10)v=10;
	if(DrawGroundMode){
		int cs=GetKeyState(VK_CONTROL)&0x8000;
		int as=GetKeyState(VK_SHIFT)&0x8000;
		if(cs&&!as)CurGroundTexture=10+v-1;
		else if(as&&!cs)CurGroundTexture=20+v-1;
		else if(cs&&as)CurGroundTexture=30+v-1;
		else CurGroundTexture=v-1;
		ManualTex=1;
		return;
	};
	if(GetKeyState(VK_CONTROL)&0x8000)PEN_BRIGHTNESS=v*25;
	else{
		char ccc[64];
		sprintf(ccc,"DrawTextures\\tex%d.bmp",v);
		LoadCurPixTexture(ccc);
		//PEN_RADIUS=4+v*4;
	};
};
extern int TexStDX;
extern int TexStDY;
void ReverseLMode();
void ProcessDialogsEditor();
int InCtrlR=0;

void EditorKeyCheck(){
	if(KeysAreFocusedOnInputBox)return;
	if(KeyPressed){
		v_GlobalHotKeys.Process();
		KeyPressed=false;
		int wParam=LastKey;

		if(g_WASDKeysAreBlocked) {
			if('W' == wParam || 'A' == wParam || 'S' == wParam || 'D' == wParam) return;
		}
		if(g_ESDFKeysAreBlocked) {
			if('E' == wParam || 'S' == wParam || 'D' == wParam || 'F' == wParam) return;
		}
		if(g_WERKeysAreBlocked) {
			if('W' == wParam || 'E' == wParam || 'R' == wParam) {
				return;
			}
		}

			switch( wParam ){
				case VK_RIGHT:
					if(DrawPixMode||DrawGroundMode)TexStDX=(TexStDX+1)&7;
					break;
				case VK_UP:
					if(DrawPixMode||DrawGroundMode)TexStDY=(TexStDY-1)&7;
					break;
				case VK_LEFT:
					if(DrawPixMode||DrawGroundMode)TexStDX=(TexStDX-1)&7;
					break;
				case VK_DOWN:
					if(DrawPixMode||DrawGroundMode)TexStDY=(TexStDY+1)&7;
					break;
				case 'E':
					///FastMode=!FastMode;
					void SelectEditor();
					if(GetKeyState(VK_CONTROL)&0x8000){
						//extern bool InEdit;
						//int IE=InEdit;
						//InEdit=false;
						SelectEditor();
						//InEdit=IE;
					}
					break;
				case '0':
					if(DrawPixMode||DrawGroundMode)STBRR(wParam);
					else if(!(GetKeyState(VK_CONTROL)&0x8000))ReliefBrush=0;
					break;
				case '1':
					if(DrawPixMode||DrawGroundMode)STBRR(wParam);else
						if(GetKeyState(VK_CONTROL)&0x8000)CostThickness=1;
						else ReliefBrush=1;
					break;
				case '2':
					if(DrawPixMode||DrawGroundMode)STBRR(wParam);else
					if(GetKeyState(VK_CONTROL)&0x8000)CostThickness=2;
					else ReliefBrush=2;
					break;
				case '3':
					if(DrawPixMode||DrawGroundMode)STBRR(wParam);else
					if(GetKeyState(VK_CONTROL)&0x8000)CostThickness=3;
					else ReliefBrush=3;
					break;
				case '4':
					// Surface shading: Wireframe
					if(!IsActiveSurfTool("Racist") && !IsActiveSurfTool("Layer")) {
						g_SurfEditor.Wireframe = true;
						ResetGroundCache();
					}
					break;
				case '5':
					// Surface shading: Solid
					if(!IsActiveSurfTool("Racist") && !IsActiveSurfTool("Layer")) {
						g_SurfEditor.Wireframe = false;
						ResetGroundCache();
					}
					break;
				case '6':
					if(DrawPixMode||DrawGroundMode)STBRR(wParam);else
					if(GetKeyState(VK_CONTROL)&0x8000)CostThickness=6;
					else ReliefBrush=9;
					break;
				case '7':
					if(DrawPixMode||DrawGroundMode)STBRR(wParam);else
					if(GetKeyState(VK_CONTROL)&0x8000)CostThickness=7;
					else ReliefBrush=20;
					break;
				case '8':
					if(DrawPixMode||DrawGroundMode)STBRR(wParam);else
					if(GetKeyState(VK_CONTROL)&0x8000)CostThickness=8;
					else ReliefBrush=50;
					break;
				case '9':
					if(DrawPixMode||DrawGroundMode)STBRR(wParam);else
					if(GetKeyState(VK_CONTROL)&0x8000)CostThickness=9;
					else ReliefBrush=100;
					break;
				case 'H':
					// Chamfer: [H]
					ActivateSurfTool("Chamfer");
					break;
					//FullScreenMode=!FullScreenMode;
					//GameNeedToDraw=true;
					//GSSetup800();
//					if( NSL[MyNation]==0 ){
//						RenderAllMap();
//					}
//					break;
				case 'V':
					// Paste piece:
					if(GetKeyState(VK_CONTROL) < 0) {
						SpecCmd = 218;
					}
					break;
/*					if(GetKeyState(VK_CONTROL)&0x8000){
						ClearModes();
						NeedToPopUp=20;
					}*//* else{
						ClearModes();
						NeedToPopUp=22;
						//SetFractalTexture();
						/*
						switch(Light){
						case 0:SetLight(-5,10,20);
							break;
						case 1:SetLight(-5,12,17);
							break;
						case 2:SetLight(-5,15,15);
							break;
						case 3:SetLight(-5,17,12);
							break;
						case 4:SetLight(-5,20,10);
							break;
						case 5:SetLight(-5,17,12);
							break;
						case 6:SetLight(-5,15,15);
							break;
						case 7:SetLight(-5,12,17);
						};
						Light++;
						if(Light>7)Light=0;
						*/
//					};
				case 13:
					if(!(GetKeyState(VK_CONTROL)&0x8000)){
						//MakeMenu=true;
						//MenuType=3;
						EnterChatMode=1;
						ClearKeyStack();
					}else KeyPressed=true;
					break;
				case 'J':
					//RSCRSizeX++;
					//ShowStatistics();
					break;
				case ' ':
					void GoToCurSelPosition();
					GoToCurSelPosition();
					break;
				case 19://Pause
				//case ' ':
					if(!LockPause)SpecCmd=137;
					//PauseMode=!PauseMode;
					break;
		        case VK_ESCAPE:
					AttGrMode=0;
					if(CheckCurve()){
						ClearCurve();
					}else{
						ClearModes();
						BuildMode=false;
						//BuildWall=false;
						GetCoord=false;
						curptr=0;
						curdx=0;
						curdy=0;
						GUARDMODE=0;
						PATROLMODE=0;
						PauseMode=false;
						SetDestMode=false;
						GoAndAttackMode=false;
					};
					//if(PlayerMenuMode==2)
					//MainMenu.ShowModal();
					//Options.ShowModal();
					break;
			   case VK_F10:
		       case VK_F12:
					//UnlockSurface();
					//CloseExplosions();
					//ShutdownMultiplayer();
		            //PostMessage(hwnd, WM_CLOSE, 0, 0);
					MenuType=1;
					MakeMenu=true;
				    break;
				//case VK_TAB:
				case 192:
					//HealthMode=!HealthMode;
					break;
			   case VK_F1:
				    //HelpMode=!HelpMode;
				    break;
			   case VK_F2:
//				    NeedToPopUp=6;
					break;
				case VK_F3:
					NeedToPopUp=15;
					break;
				case VK_F4:
					NeedToPopUp=4;
					break;
				case VK_F5:
					if(WaterEditMode){
						WaterEditMode=1;
						NeedToPopUp=8;
					}else{
						NeedToPopUp=5;
					};
					break;
				case 'U':
					//g_Navigator.LinkToUnitToggle(); //GEC demo only for disable ('U') key
					break;
/*					if(!GSets.LeibzigDemo.Enabled){
						if(!(GetKeyState(VK_CONTROL)&0x8000)||
							(GetKeyState(VK_SHIFT)&0x8000)&&
							(GetKeyState(VK_MENU)&0x8000)){
							//void ProcessAIEditor();
							//ProcessAIEditor();
							break;
						}else
						if((GetKeyState(VK_SHIFT)&0x8000)&&
							(GetKeyState(VK_CONTROL)&0x8000)){
							//ProcessDialogsEditor();
						}else{						
							if(GetKeyState(VK_CONTROL)&0x8000)CINFMOD=!CINFMOD;
							else if(Inform!=2)Inform=2;else Inform=0;
							DIP_DSS_Vis=0;
							extern int InfDX;
							extern int InfDX0;
							InfDX=InfDX0;
							MiniActive=0;
							Recreate=1;
						}
						//CINFMOD=0;
					}
					break;*/
				case 'F':
					// Fog: [Ctrl] + [F]
					// Facturize: [F]
					bool fFog, fFreeCamera;
					fFog = GetKeyState(VK_CONTROL) < 0;
					if(fFog) {
						FogMode = !FogMode;
					} else {
						ActivateSurfTool("Facturize");
					}
					break;
				case VK_F6:
					if(WaterEditMode){
						WaterEditMode=2;
						NeedToPopUp=9;
					}else{
						ClearModes();
						MakeMenu=true;
						MenuType=6;
					};
					break;
				case VK_F7:
					if(WaterEditMode){
						WaterEditMode=3;
						NeedToPopUp=10;
					}else{
#ifdef _USE3D
						if(GetKeyState(VK_CONTROL)&0x8000){
							void MakeAllDirtyGBUF();
							extern bool Mode3D;
							Mode3D=!Mode3D;
							MakeAllDirtyGBUF();
						}else NeedToPopUp=23;
#else
						Reset3D();
#endif
					};
					break;
				case VK_F8:
					if(WaterEditMode){
						WaterEditMode=4;
						NeedToPopUp=11;
					}else{ 
						if(GetKeyState(VK_CONTROL)&0x8000)NeedToPopUp=24;
						else NeedToPopUp=3;
					};
					break;
				case 'N':
					if(GetKeyState(VK_CONTROL)&0x8000){
						AutoSMSSet();
					}else{
						NeedToPopUp=7;						
					};
				case 'K':
					ActivateSurfTool("Grass");
					break;
				case VK_DELETE://VK_END://46: // Deleting selection:
					if( !DelCurrentAZone() && (GetKeyState(VK_CONTROL)&0x8000) ){
						SpecCmd=200;
					}
					break;
				case 'D':
					// Deform: [D]
					ActivateSurfTool("Deform");
					break;
//					if(!(GetKeyState(VK_CONTROL)&0x8000)){
//						if((GetKeyState(VK_SHIFT)&0x8000)/*&&PlayGameMode*/){
/*							switch(HISPEED){
							case 0:
								HISPEED=1;
								break;
							case 1:
								HISPEED=2;
								break;
							case 2:
								HISPEED=3;
								break;
							default:
								HISPEED=0;
								break;
							};
						}else{
							//FlipDipDialog();
						};
					}else{
						//if(FrmDec==1)CmdSetSpeed(1+128);
						//else CmdSetSpeed(128);
					};*/
				case 'A':
					if((GetKeyState(VK_CONTROL)&0x8000)&&(GetKeyState(VK_SHIFT)&0x8000)){
						void ProcessUnitAbilityEditor();
						ProcessUnitAbilityEditor();
					}else if(GetKeyState(VK_CONTROL)&0x8000){
						SpecCmd=1;
						LastKey=0;
					}
					//else if(NSL[MyNation])GoAndAttackMode=1;
					break;
				case 'S':
					// Smooth: [S]
					// Lock: [Ctrl] + [S]
					if( NSL[MyNation]==0 ){
						bool fLock;
						fLock = GetKeyState(VK_CONTROL) < 0;
						if(fLock) {
							switch(LockMode) {
							case 0:
								NeedToPopUp = 12;
								break;
							case 1:
								NeedToPopUp = 13;
								break;
							case 2:
								NeedToPopUp = 14;
								break;
							case 3:
								NeedToPopUp = 1;
								break;
							}
						} else if(!IsActiveSurfTool("Facturize")) {
							ActivateSurfTool("Smooth");
						}
					}
					break;
					//ClearModes();
/*					if((GetKeyState(VK_CONTROL)&0x8000)&&(GetKeyState(VK_SHIFT)&0x8000)){
						void ProcessSoundsEditor();
						ProcessSoundsEditor();
					}else
					if(GetKeyState(VK_CONTROL)&0x8000){
						//EnterRandomParams();
						//void FastReload(char* Name);
						//FastReload("Save\\1.sav");
					}else{
						switch(LockMode){
						case 0:
							NeedToPopUp=12;
							break;
						case 1:
							NeedToPopUp=13;
							break;
						case 2:
							NeedToPopUp=14;
							break;
						case 3:
							NeedToPopUp=1;
							break;
						};
					};
					break;*/
				case 'W':
					//if((GetKeyState(VK_CONTROL)&0x8000)&&(GetKeyState(VK_SHIFT)&0x8000)){
					//	void ProcessWeaponSystemEditor();
					//	ProcessWeaponSystemEditor();
					//}else
					if(GetKeyState(VK_CONTROL)&0x8000)ProcessSaveInSquares();
					else PeaceMode=!PeaceMode;
					break;
					/*
				case 'N':
					switch(RSCRSizeX){
						case 800:RSCRSizeX=1024;
								break;
						case 1024:RSCRSizeX=800+32;
								break;
						case 1280:RSCRSizeX=1600;
							break;
							case 1600:RSCRSizeX=800;
							break;
						default:
							RSCRSizeX=800;
					};*/
					/*if(RSCRSizeX!=1024)RSCRSizeX=1024;
					else RSCRSizeX=800;*/
					//break;
				case 'C':
					if(g_fZXCKeysAreBlocked) break;
					// Colorize: [C]
					if(GetKeyState(VK_CONTROL) < 0) {
						// Copy piece:
						SpecCmd = 217;
					} else ActivateSurfTool("Colorize");
					break;
				case 'G':
					// SurfGenerator:
					if(SURF.m_fActive) {
						SURF.m_fActive = false;
					} else {
						ShowSurfGenerator();
					}
					break;
				case 'X':
					if(g_fZXCKeysAreBlocked) break;
					//if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=5;
					//else SpecCmd=6;
					if(GetKeyState(VK_CONTROL)&0x8000){
						void CreateLightMap();
						CreateLightMap();
						EraseAreas();
						rando();
						CreateTotalLocking();						
						ResearchIslands();
						void CreateMiniMap();
						CreateMiniMap();
						void UnLoadAITools();
						UnLoadAITools();
						void RefreshRoads();
						RefreshRoads();
						void PutSpritesOnEarth();
						PutSpritesOnEarth();
						g_UniversalMap.CalcNormals();						
						ITerra->InvalidateGeometry();
					} else {
						// SurfEditor
						if(g_SurfEditor.m_fActive) {
							g_SurfEditor.m_fActive = false;
						} else if(!IsActiveSurfTool("Facturize")) {
							ShowSurfEditor();
						}
					};
					break;
				case 'Q':
					// Reset Surface Tools: [Q]
					// Topology mode: [Ctrl] + [Q]
					bool fTopology;
					fTopology = GetKeyState(VK_CONTROL) < 0;
					if(!fTopology) {
						ClearSurfTool();
					} else {
						if(EngSettings.DebugTopologyMode){
							if(!(GetKeyState(VK_CONTROL)&0x8000)) LockGrid++;
							if(LockGrid>NMFIELDS) LockGrid=0;						
							if(GetKeyState(VK_CONTROL)&0x8000) TopTypeMode++;
							if(TopTypeMode>NMFIELDS) TopTypeMode=0;
						}else{
							LockGrid=!LockGrid;
						}
						MiniActive=0;
						Recreate=1;
					}
					break;
				case 'B':
					bool fSelBuildings;
					fSelBuildings = GetKeyState(VK_CONTROL) < 0;
					if(!fSelBuildings) {
						// Booleans: [B]
						ActivateSurfTool("Combine");
					} else {
						SpecCmd = 9;
					}
					break;
				case 'Z':
					if(g_fZXCKeysAreBlocked) break;
					if(GetKeyState(VK_CONTROL)&0x8000)UndoMaster::Undo();
					/*
					if(g_fZXCKeysAreBlocked) break;
					if(DrawPixMode){
						PerformPixUndo();
					};
					if(GetKeyState(VK_CONTROL)&0x8000)SpecCmd=11;
					else SpecCmd=241;
					*/
					LastKey=0;
					break;
				//case VK_F11:
				//	WaterCorrection();
				//	break;
				case VK_F9:
					ClearSurfTool();
//					MEditMode=false;
//					HeightEditMode=false;
//					LockMode=0;
//					Creator=4096+255;
					break;
				case VK_NUMPAD1:
					SetMyNation(0);
					MEditMode=false;
					HeightEditMode=false;
					PlayerMask=1;
					break;
				case VK_NUMPAD2:
					SetMyNation(1);
					MEditMode=false;
					HeightEditMode=false;
					PlayerMask=2;
					break;
				case VK_NUMPAD3:
					SetMyNation(2);
					MEditMode=false;
					HeightEditMode=false;
					PlayerMask=4;
					break;
				case VK_NUMPAD4:
					SetMyNation(3);
					MEditMode=false;
					HeightEditMode=false;
					PlayerMask=8;
					break;
				case VK_NUMPAD5:
					SetMyNation(4);
					MEditMode=false;
					HeightEditMode=false;
					PlayerMask=16;
					break;
				case VK_NUMPAD6:
					SetMyNation(5);
					MEditMode=false;
					HeightEditMode=false;
					PlayerMask=32;
					break;
				case VK_NUMPAD7:
					SetMyNation(6);
					MEditMode=false;
					HeightEditMode=false;
					PlayerMask=64;
					break;
				case VK_NUMPAD8:
					SetMyNation(7);
					MEditMode=false;
					HeightEditMode=false;PlayerMask=128;
					break;
				case 'I':
					if(!GSets.LeibzigDemo.Enabled){
						if(GetKeyState(VK_CONTROL)&0x8000)InfoMode=!InfoMode;
						/*
						else if(Inform!=1)Inform=1;else{
							Inform=0;
							InfDX=InfDX0;
						};
						MiniActive=0;
						Recreate=1;
						*/
					}
					//InfoMode=1;
					break;
				case 'O':
					if(GetKeyState(VK_CONTROL)&0x8000){
						OptHidden=!OptHidden;
						NeedToPopUp=1;
					}else{
						TransMode=!TransMode;
						MiniActive=0;
						Recreate=1;
					};
					break;
					//WaterCorrection();
					//if(MsPerFrame)MsPerFrame--;
				case 'P':
					if(GetKeyState(VK_CONTROL)&0x8000){
						SpecCmd=113;
					}
					//MsPerFrame++;
					//if(GetKeyState(VK_CONTROL)&0x8000)RotatePhiI();
					//RotatePhi();
					NeedToPopUp=2;
					break;
				case 'R':
					if( (GetKeyState(VK_CONTROL)&0x8000) && (GetKeyState(VK_MENU)&0x8000) ){
#ifdef _USE3D
						OneSprite::ClearActiveSprites();
						void ClearActiveAbilities();
						ClearActiveAbilities();
						ext_OnReloading();
						InCtrlR=1;
						extern IRenderSystem* IRS;
						void ReloadRoads();
						void CreateWPalette();
						extern word NMines;
						NMines=0;
						//IRS->RecompileAllShaders();
						//IRS->ReloadAllTextures();
						bool LoadBoidsParams();
						LoadBoidsParams();
						void KillTimeReq();
						KillTimeReq();
						void LoadTextIcons();
						LoadTextIcons();
						void LoadFlagsInfo();
						LoadFlagsInfo();
						ReloadRoads();
						CreateWPalette();
						void InitSprites();
						InitSprites();
						void ReadChainObjectsDesc();
						ReadChainObjectsDesc();
						InCtrlR=2;
						void LoadAllNewMonsters();
						LoadAllNewMonsters();
						InCtrlR=3;
						for(int i=0;i<8;i++){
							NATIONS[i].CloseNation();
							//CITY[i].CloseCity();
						};
						InCtrlR=4;
						void LoadAllNations(byte msk,byte NIndex);
						LoadAllNations(1,255);
						InCtrlR=5;
						for(int i=0;i<MAXOBJECT;i++)if(Group[i]){
							OneObject* OB=Group[i];
							OB->Ref.General=NATIONS[OB->NNUM].Mon[OB->NIndex];
							OB->newMons=OB->Ref.General->newMons;
						}
						InCtrlR=6;
						LoadVideoSeq(&VideoSeq,"Video\\vseq.dat");
						LoadVideoSeq(&VidOfSeq,"Video\\vofseq.dat");
						LoadVideoSeqScript(&VideoSeqScript,"Video\\vseq.qpp.xml");
						LoadVideoSeqScript(&VidOfSeqScript,"Video\\vofseq.qpp.xml");
						LoadVideoSeqScript(&SoundEvents,"Sound\\events.qpp.xml");
						InCtrlR=0;
#endif // _USE3D
						//ProcessMapOptions();
					}else{
						// Racist Tool: [R]
						ActivateSurfTool("Racist");

/*						MEditMode=false;
						HeightEditMode=false;
						MakeMenu=true;
						MenuType=31;
#ifdef _USE3D
						void ReloadRoads();
						ReloadRoads();
#endif*/
					};
					break;
				case 'L':
					/*
					if(!MiniMode)SetMiniMode();
					else ClearMiniMode();
					MEditMode=false;
					*/
//					ReverseLMode();

					// Layer Tool: [L]
					ActivateSurfTool("Layer");
					break;
				case 'T':
					//HeightEditMode=false;
					//ChoosePosition=true;
					if(GetKeyState(VK_CONTROL) < 0) {
						ToolsHidden = !ToolsHidden;
					} else {
						g_TankController.Toggle();
					}
//					if(GetKeyState(VK_CONTROL)&0x8000){
//						ToolsHidden=!ToolsHidden;
//					};
					break;

				case 107://NUM +
					if(HeightEditMode)HiStyle=1;
					break;
				case 109://NUM -
					if(HeightEditMode)HiStyle=2;
					break;
				case 106://NUM *
					if(HeightEditMode)HiStyle=3;
					break;
				case 111://NUM /
					if(HeightEditMode)HiStyle=4;
					break;
				case 'M'://NUM 0
					if(GetKeyState(VK_CONTROL)&0x8000){
						//AddHill();
						///GenerateRandomRoad(5);
					}else{						
						extern bool MiniMapVisible;
						if(!MiniMapVisible){
							FullMini=1;
							MiniMapVisible=1;
						}else{
							if(FullMini)FullMini=0;
							else MiniMapVisible=0;
						}
					}
					MiniActive=0;
					Recreate=1;
					break;
				case 33:
					if(HeightEditMode)HiStyle=7;
					break;
				case 34:
					if(HeightEditMode)HiStyle=8;
					break;
				case 36:
					if(HeightEditMode)HiStyle=9;
					break;
				default:
					if(wParam>='0'&&wParam<='9'){
						if(GetKeyState(VK_SHIFT)&0x8000){
							int v=wParam-'0';
							SHIFT_VAL=SHIFT_VAL*10+v;
						}else{
							if(GetTickCount()-LastCTRLPressTime<150){
								CmdMemSelection(MyNation,wParam-'0');
							}else CmdRememSelection(MyNation,wParam-'0'); 
							//if(GetKeyState(VK_CONTROL)&0x8000)
							//	CmdMemSelection(MyNation,wParam-'0');
							//else CmdRememSelection(MyNation,wParam-'0');
						};
					};
			 };
	};
};
void SERROR();
void SERROR1();
void SERROR2();
bool PalDone;
bool InitScreen(){
#ifdef _USE3D	
	return true;
#endif
	PalDone=false;
	CreateDDObjects(hwnd); 
	PalDone=false;
	LoadPalette("agew_1.pal");//agew.pal");//grassw.pal");//dmw.pal");//"darkw.pal");
    if(!DDError)
    {
		LockSurface();
		UnlockSurface();
		if(!RealScreenPtr){
			MessageBox(hwnd,"Unable to initialise Direct Draw. It is possible that hardware acceleration is turned off.","Loading error[1]",MB_ICONSTOP);
			exit(0);
		};
		//LockSurface();
		//UnlockSurface();
#ifndef _USE3D
		ShowCursor(UseSysMouse);
#endif // !_USE3D
		RECT OOO;
		OOO.left=-16;
		OOO.top=-16;
		OOO.right=1500;
		OOO.bottom=1200;
		ClipCursor(&OOO);
		SetCursorPos(512,300);//!!!!!!!!!!!!!!!!
#ifndef _USE3D
		ShowCursor(UseSysMouse);
#endif // !_USE3D

		OOO.left=0;
		OOO.top=0;
		OOO.right=RealLx-1;
		OOO.bottom=RealLy-1;
		ClipCursor(&OOO);
		return true;
    }else{
		PlayEffect(0,0,0);
	};
	return false;
};

#ifndef _USE3D
BOOL CreateRGBDDObjects(HWND hwnd);
bool InitRGBScreen(){
	PalDone=false;
	CreateRGBDDObjects(hwnd); 
	PalDone=false;
    if(!DDError)
    {
		LockSurface();
		UnlockSurface();
		//LockSurface();
		//UnlockSurface();
		ShowCursor(true);
		RECT OOO;
		OOO.left=-16;
		OOO.top=-16;
		OOO.right=1500;
		OOO.bottom=1200;
		ClipCursor(&OOO);
		SetCursorPos(512,300);//!!!!!!!!!!!!!!!!
		OOO.left=0;
		OOO.top=0;
		OOO.right=RealLx-1;
		OOO.bottom=RealLy-1;
		ClipCursor(&OOO);
		return true;
	};
	return false;
};
#endif // !_USE3D 

BOOL CreateRGB640DDObjects(HWND hwnd);
bool InitRGB640Screen(){
	CreateRGB640DDObjects(hwnd); 
    if(!DDError)
    {
		LockSurface();
		UnlockSurface();
		//LockSurface();
		//UnlockSurface();
		ShowCursor(true);
		RECT OOO;
		OOO.left=0;
		OOO.top=0;
		OOO.right=640;
		OOO.bottom=480;
		ClipCursor(&OOO);
		SetCursorPos(639,479);//!!!!!!!!!!!!!!!!
#ifndef _USE3D
		ShowCursor(UseSysMouse);
#endif // !_USE3D

		return true;
	};
	return false;
};

byte NoCD=1;
DLLEXPORT
void __stdcall CDGINIT_EnCD(){
	NoCD=0;
};
bool xLockMouse;
extern int PlayMode;
void StopPlayCD();
void PlayRandomTrack();
bool First=1;
bool ProcessMessagesEx();
void ClearRGB();
BOOL GetCossacksDir(LPSTR lpszPath)
{
	HKEY	hKey;
	ULONG	lValue=255;
	UCHAR	szValue[255];

	if(RegOpenKeyEx(HKEY_CURRENT_USER,
		"SOFTWARE\\GSC Game World\\EW : Cossacks",
		0,KEY_QUERY_VALUE,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,
			"InstallDir",
			NULL,
			NULL,
			szValue,
			&lValue)==ERROR_SUCCESS)
		{
			strcpy(lpszPath,(LPSTR)szValue);
			RegCloseKey(hKey);
			return TRUE;
		};		

		RegCloseKey(hKey);
		return FALSE;
	}
	else
		return FALSE;
};
void PlayBinkFile(char* path);
HRESULT mpv_Play(LPCSTR lpcszMediaFileName);
void mpv_Done(void);
bool InVideo=0;
void PlayVideo(){
	//return;
	char CPATH[256];
	//if(!GetCossacksDir(CPATH))return;
	char AviName[256];
	sprintf(AviName,"Video\\1.bik",CPATH);
	ResFile f=RReset(AviName);
	if(f!=INVALID_HANDLE_VALUE){
		RClose(f);
	}else{
		xLockMouse=false;
		if(GSets.SVOpt.MusicPlayMode)PlayRandomTrack();
		return;
	};

	/*
	for(int k=0;k<16;k++){
		ResFile f=RReset(AviName);
		if(f!=INVALID_HANDLE_VALUE){
			RClose(f);
			goto plavi;
		};
		AviName[0]++;
	};
	*/
	//return;
plavi:
	StopPlayCD();
	ClearScreen();
	//FreeDDObjects();
	InVideo=1;
	InitRGB640Screen();
	MiniMode=false;
	CDIRSND.~CDirSound();
	PlayBinkFile("Video\\1.bik");
	PlayBinkFile("Video\\2.bik");
	PlayBinkFile("Video\\3.bik");
	PlayBinkFile("Video\\4.bik");
	PlayBinkFile("Video\\5.bik");
	if(First){
		sprintf(AviName,"Video\\cdv.avi",CPATH);
		ResFile f=RReset(AviName);
		if(f!=INVALID_HANDLE_VALUE){
			RClose(f);
			PlayAVI(hwnd,AviName,0);
			xLockMouse=true;
			do{
				ProcessMessagesEx();
				Sleep(30);
			}while(!(KeyPressed||Lpressed||!bActive));
			CloseAVI(hwnd);
			ClearRGB();
			First=0;
			KeyPressed=0;
			Lpressed=0;
		};
		sprintf(AviName,"%s\\Video\\logo.avi",CPATH);
		f=RReset(AviName);
		if(f!=INVALID_HANDLE_VALUE){
			RClose(f);
			PlayAVI(hwnd,AviName,60);
			xLockMouse=true;
			do{
				ProcessMessagesEx();
				Sleep(30);
			}while(!(KeyPressed||Lpressed||!bActive));
			CloseAVI(hwnd);
			First=0;
			KeyPressed=0;
			Lpressed=0;
			ClearRGB();
		};
	};
	sprintf(AviName,"%s\\Video\\kz.avi",CPATH);
	f=RReset(AviName);
	if(f!=INVALID_HANDLE_VALUE){
		RClose(f);
		PlayAVI(hwnd,AviName,60);
		xLockMouse=true;
		do{
			ProcessMessagesEx();
			Sleep(30);
		}while(!(KeyPressed||Lpressed||!bActive));
		CloseAVI(hwnd);
	};
	PlayBinkFile("Video\\6.bik");
	PlayBinkFile("Video\\7.bik");
	PlayBinkFile("Video\\8.bik");
	PlayBinkFile("Video\\9.bik");
	PlayBinkFile("Video\\10.bik");

	Lpressed=false;
	KeyPressed=false;
	ClearRGB();
	InVideo=0;
	//FreeDDObjects();
	InitScreen();
	CDIRSND.CreateDirSound(hwnd);
	CDS=&CDIRSND;
	LoadSounds("SoundList.txt");
	xLockMouse=false;
	if(GSets.SVOpt.MusicPlayMode)PlayRandomTrack();
};
extern bool Lpressed;
void FilesExit();
void ShowLoading();
void KeyTestMem();

#ifdef MAYA_BOOLEANS
// BEGIN: MAYA! MUST BE DELETED!
#include "Surface\Maya\include\MStatus.h"
#include "Surface\Maya\include\MLibrary.h"
MStatus g_MayaStatus;
// END: MAYA! MUST BE DELETED!
#endif // MAYA_BOOLEANS

static BOOL doInit( HINSTANCE hInstance, int nCmdShow )
{

    WNDCLASS            wc;
    char                buf[256];

    /*
     * set up and register window class
     */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    //wc.hIcon = NULL;//LoadIcon( hInstance, IDR_MAINFRAME );
	wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	//wc.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));

    wc.hCursor = NULL;//LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NAME;
    RegisterClass( &wc );
    
    /*
     * create a window
     */
    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW/*TOPMOST*/,
        NAME,
        TITLE,
        WS_POPUP,
        0, 0,
        GetSystemMetrics( SM_CXSCREEN ),
        GetSystemMetrics( SM_CYSCREEN ),
        NULL,
        NULL,
        hInstance,
        NULL );
		
    if( !hwnd )
    {
        return FALSE;
    };
	CreateDDObjects(hwnd); 
	CDIRSND.CreateDirSound(hwnd);
	CDS=&CDIRSND;	
	IMM->GetModelID("Models\\s_cursor.c2m");

    void test();
    test();
	InitNewMonstersSystem();

	void CreateQuadRenderers();
	CreateQuadRenderers();

	REG_CLASS(StdBorder);

	REG_CLASS(ParentFrame);
	REG_CLASS(DialogsSystem);
	REG_CLASS(SimpleDialog);

	REG_CLASS(ControlToClassConnector);
	REG_CLASS(SimpleClassConnector);
	REG_CLASS(IndexedClassConnector);
	REG_CLASS(CallingClassConnector);
	REG_CLASS(BoolClassConnector);
	REG_CLASS(IndexComparisonClassConnector);
	REG_CLASS(IntClassConnector);
	REG_CLASS(ArrayConnector);

	REG_CLASS(TextButton);
	REG_CLASS(GPPicture);
	REG_CLASS(GP_Button);
	REG_CLASS(GP_TextButton);
	REG_CLASS(Canvas);
	REG_CLASS(DialogsDesk);
	REG_CLASS(ComboBox);
	REG_CLASS(BitPicture);
	REG_CLASS(VScrollBar);
	REG_CLASS(HorisontalSlider);
	REG_CLASS(CheckBox);
	REG_CLASS(Scene3D);	
	REG_CLASS(VitButton);
	REG_CLASS(TabButton);
	REG_CLASS(TabDesk);
	REG_CLASS(ListDesk);
	REG_CLASS(ClassDesk);
	REG_CLASS(InputBoxOld);
	REG_CLASS(InputBox);//new
	REG_CLASS(ChatDesk);	
	REG_CLASS(ProgressBar);
	REG_CLASS(ColoredBar);

    REG_CLASS(Scene3D);
	REG_CLASS(TextureViewer);
    REG_CLASS(BackgroundMap);
    REG_CLASS(SceneElement);
    REG_CLASS(SceneCamera);
    REG_CLASS(ModelInScene);

	void REG_EXCHANGE_CLASSES();
	REG_EXCHANGE_CLASSES();
	//
	REG_CLASS(NewAnimation);
	REG_CLASS(AnimFrame3D);
	REG_CLASS(ClassPtr<NewAnimation>);

	BScope.SafeReadFromFile("dialogs\\borders.xml");
	InitFonts();

	ClearLoadMark();
	AddLoadMark("LoadMenu",50);
	AddLoadMark("InitGlobals",5);
	AddLoadMark("Sound",10);	
	AddLoadMark("Monsters",100);

	GSets.Load();
	//GSets.RoomParams.GameSpeed=1;
	GSets.SVOpt.RequiredMsPerFrame=30;

	extern IRenderSystem* IRS;

	// Antialiasing:
	if(GSets.SVOpt.Antialiasing.Enabled) IRS->AntialiasingEnable();
	else IRS->AntialiasingDisable();
	// TrueColor:
	if(GSets.SVOpt.TrueColor.Enabled) IRS->TrueColorEnable();
	else IRS->TrueColorDisable();
	// SafeRefreshRate:
	if(g_RefreshRateIsOverridden) {
		IRS->RefreshRateOverrideEnable(g_RefreshRateOverriddenValue);
	} else {
		IRS->RefreshRateOverrideDisable();
	}
	
	ScreenProp prop = IRS->GetScreenProp();
	RealLx = 1024; //1024
	RealLy = 768; //768
	prop.m_Width = RealLx;
	prop.m_Height = RealLy;
	IRS->SetScreenProp( prop );
	GPS.SetClipArea( 0, 0, RealLx, RealLy );
	void RegisterSurfaceClasses();
	RegisterSurfaceClasses();	

	void InitMenu(int State);
	InitMenu(0);	

	void SetupCamera();
	SetupCamera();
	void TestLayersColors();
	TestLayersColors();
	void LoadFactures();
	LoadFactures();
	// BEGIN: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#ifdef MAYA_BOOLEANS
	// BEGIN: MAYA! MUST BE DELETED!
	g_MayaStatus = MLibrary::initialize("HoAE");
	// END: MAYA! MUST BE DELETED!
#endif // MAYA_BOOLEANS

//	bool probeBooleans(void);
//	if(!probeBooleans()) Log.Error("probeBooleans() FAILED!");
//	bool testBooleans3D(void);
//	if(!testBooleans3D()) Log.Error("testBooleans3D(FAILED) !");
//	bool probeDraw();
//	if(!probeDraw()) MessageBox(NULL, "probeDraw() FAILED!", "--- ERROR ---", MB_OK);
	// END: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#ifdef TEST_3D
	bool probeDraw();
	///if(probeDraw() == false) MessageBox(NULL, "probeDraw() FAILED!",
	//	"--- ERROR ---", MB_OK);
#endif //TEST_3D
#ifdef TEST_3D_MATH
//	bool MathTestDraw();
//	MathTestDraw();
#endif //TEST_3D_MATH
	//UpdateWindow( hwnd );
    ShowWindow( hwnd, SW_SHOWNORMAL);//nCmdShow ); 
    UpdateWindow( hwnd );

	bool EnumDisplayModes();
	EnumDisplayModes();

	void InitGlobals();
	ShowLoadProgress("InitGlobals",0,10);
	InitGlobals();    
	
	//SetCapture(hwnd);
	ResFile F=RReset("version.dat");
	if(F!=INVALID_HANDLE_VALUE){
		word B=0;
		RBlockRead(F,&B,2);
		RClose(F);
		if(B>102){
			MessageBox(hwnd,"Unable to use this testing version.","WARNING!",0);
			FilesExit();
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		};
	};
	if(!Loading()){
		FilesExit();
		PostMessage(hwnd, WM_CLOSE, 0, 0);
		return 0;
	};
	
	void InitMenu(int State);
	InitMenu(1);

	//void PerformCheckCD();
	//PerformCheckCD();
	extern bool CanPlayInternetGame;
	if(!CanPlayInternetGame){
		void ShowCentralText0(char* sss);
        
        Timer tm;
        tm.start();
        while (tm.seconds() < 1.0f)
        {
		    ShowCentralText0(GetTextByID("#MB_03"));
		    GPS.FlushBatches();
		    FlipPages();
            Sleep( 10 );
        }
    }

	//mpv_Play("Video\\1.wmv");
	//mpv_Play("Video\\2.wmv");
	//mpv_Play("Video\\3.wmv");
	//mpv_Play("Video\\4.wmv");
	//mpv_Play("Video\\5.wmv");
	//mpv_Done();

	//HCURSOR mcr=LoadCursorFromFile("curs1.cur");
	//SetCursor(mcr);
	//ShowCursor(true);
	SetCursorPos(512,300);//!!!!!!!!!!!!!!!!
#ifndef _USE3D
	ShowCursor(UseSysMouse);
#endif //!_USE3D

	//HANDLE hh1=LoadImage(NULL,"mcur.bmp",IMAGE_BITMAP,32,32,LR_LOADFROMFILE);
	CurrentSurface=FALSE;
	
    /*
     * create the main DirectDraw object
     */
	PalDone=false;
/*	InitScreen();
	InitRGBScreen();
	xLockMouse=true;
	ResFile ff1=RReset("war2000.raw");
	for(int i=0;i<600;i++){
		RBlockRead(ff1,(char*)(int(RealScreenPtr)+4*800*i),4*800);
	};
	RClose(ff1);
	do{
		ProcessMessages();
	}while(!(Lpressed||KeyPressed));
	xLockMouse=false;
	FreeDDObjects();*/
	KeyPressed=false;
	MemReport("CreateDDObjects");
	//LoadPalette("darkw.pal");
	CHKALL();
    if(!DDError)
    {
#ifndef _USE3D
		LockSurface();
		UnlockSurface();
		LockSurface();
		UnlockSurface();
		if(!RealScreenPtr){
			MessageBox(hwnd,"Unable to initialise Direct Draw. It is possible that hardware acceleration is turned off.","Loading error[2]",MB_ICONSTOP);
			exit(0);
		};
#endif //_USE3D
#ifndef _USE3D
		ShowCursor(UseSysMouse);
#endif // !_USE3D

		RECT OOO;
		OOO.left=-16;
		OOO.top=-16;
		OOO.right=1500;
		OOO.bottom=1200;
		ClipCursor(&OOO);
		SetCursorPos(512,300);//!!!!!!!!!!!!!!!!
#ifndef _USE3D
		ShowCursor(UseSysMouse);
#endif // !_USE3D

		OOO.left=0;
		OOO.top=0;
		OOO.right=RealLx-1;
		OOO.bottom=RealLy-1;
		ClipCursor(&OOO);
		InVideo=1;
		//ShowLoading();
        if( SetTimer(hwnd, TIMER_ID, 20, NULL ) )
        {
            return TRUE;
        }
		//FlipPages();
    }
    wsprintf(buf, "Direct Draw Init Failed\n");
    MessageBox( hwnd, buf, "ERROR", MB_OK );
    finiObjects();
    DestroyWindow( hwnd );
    return FALSE;
}; /* doInit */
void AddDestn(byte x,byte y);
void ProcessNewMonsters();
int PrevTime=0;
void HandleMines();
void HealWalls();
void ProcessDeathList();
void ProcessEconomy();
void HandleMission();
//void CheckTops(){
//	int NT=GetNAreas()*GetNAreas();
//};

void ResearchCurrentIsland(byte Nat);
void ResearchBestPortToFish(byte Nat);
extern int NInGold[8];
extern int NInIron[8];
extern int NInCoal[8];
extern int NInStone[8];
extern int WasInGold[8];
extern int WasInIron[8];
extern int WasInCoal[8];
extern int WasInStone[8];
extern bool Tutorial;
void ProcessCostPoints();
void CheckArmies(City*);
void CheckGP();
extern int exFMode;
void CmdSetSpeed(word);
bool NOPAUSE=1;
void EnumPopulation();
extern bool TutOver;
void HandleShar(Nation* NT);
void AddRandomBlobs();
void ProcessMapAutosave();
void CHECKIT(){
//	for(int i=0;i<4;i++)assert(GNFO.EINF[3]->GAINF.ArmDistr);
};
void ProcessGuard();
void DecreaseVeruVPobedu();
int InEnum=0;
extern "C" CEXPORT
void CreateFields(byte NI);
extern int LastWaterchange;
extern int LastBrightspot;
void RESEARCH_WAVES();
int STARTT=0;
int STARTTICK=0;
int LASTT=0;
void KillTimers(){
	STARTT=0;
	STARTTICK=0;
	LASTT=0;

}
int RequiredMsPerFrame=40;
void ProcessWalls();
void TestCGARB();
void UnitAbilityTest();
int KillSpeedVariation=0;
void PreDrawGameProcess(){
	void CheckIfNewMapLoadingNeed();
	CheckIfNewMapLoadingNeed();
	//UnitAbilityTest();
	void InternalProcess();
	InternalProcess();
	//UnitAbilityTest();
	void CheckMemoryIntegrity();
	ATTLIST.Refresh();
	//UnitAbilityTest();
	ProcessWalls();
	//UnitAbilityTest();

	addrand(1234);
	//ext_ProcessingGame();
	void on_ProcessGame();
	on_ProcessGame();
	//UnitAbilityTest();
	addrand(5678);
	
	//forcing game speed
	if(GSets.SVOpt.RequiredMsPerFrame>5){
		/*
		if(GSets.CGame.SpecialGameSpeed>0){
			GSets.SVOpt.RequiredMsPerFrame=GSets.CGame.SpecialGameSpeed;
		}else{
			extern bool vCreditsMode;
			if(MOptions.VictoryRule.Get()&&!vCreditsMode){
				GSets.SVOpt.RequiredMsPerFrame=EngSettings.DefaultGameSpeedForSkirmish;
			}else{
				GSets.SVOpt.RequiredMsPerFrame=EngSettings.DefaultGameSpeedForCampaign;
			}
		}
		*/
	}
	//------------------
	//CheckMemoryIntegrity();
	RunPF((256*5)+7,"PreDrawGameProcess");	
	if(EditMapMode){
		if(LastWaterchange&&GetTickCount()-LastWaterchange>5000){
			EraseAreas();
			CreateTotalLocking();
			LastWaterchange=0;
		};
		if(LastBrightspot&&GetTickCount()-LastBrightspot>3000){
			RESEARCH_WAVES();
			LastBrightspot=0;
		};
	};
	//UnitAbilityTest();
	CHECKIT();
	//UnitAbilityTest();
	ProcessMapAutosave();
	//UnitAbilityTest();
	//AddRandomBlobs();
	CDS->ProcessSoundSystem();
	//UnitAbilityTest();
	if(NOPAUSE){
		for(int w=0;w<8;w++){
			WasInGold[w]=NInGold[w];
			WasInCoal[w]=NInCoal[w];
			WasInIron[w]=NInIron[w];
			WasInStone[w]=NInStone[w];
			NInGold[w]=0;
			NInCoal[w]=0;
			NInIron[w]=0;
			NInStone[w]=0;
		};
	};
	for(int g=0;g<8;g++){
		if(CITY[g].Account<0)CITY[g].Account=0;
	};
	if(exFMode!=SpeedSh){
		//CmdSetSpeed(exFMode+128);
	};
	//UnitAbilityTest();
	if((tmtmt&255)==32)EnumPopulation();
	//UnitAbilityTest();
	ProcessCostPoints();
	ProcessGuard();
	//UnitAbilityTest();
	//RunPF(9);
	if(NOPAUSE){
		ProcessEconomy();
		ProcessDeathList();
		//RefreshSafeMap();
	};
	//UnitAbilityTest();
	int tt=tmtmt&255;
	//if(tt==49)CreateEnmBuildList();
	//if(tt==199||tt==199-128)CreateProtectionMap();
	//SUSP("ResearchSafeCells");
	//if(NOPAUSE)ResearchSafeCells(200,400);
	SUSP("HealWalls");
	//HealWalls();
	if(LastAttackDelay>0)LastAttackDelay-=GameSpeed;
	if(LastAttackDelay<0)LastAttackDelay=0;
	//UnitAbilityTest();
	//assert(_CrtCheckMemory());
	addrand(21);
	//ProcessTow();
	addrand(45);
	for(int i=0;i<8;i++){
		memset(NATIONS[i].SoundMask,0,2048);
	};
	//CHKALL();
	InitDestn();
//	StopPF(9);
	GameKeyCheck();
	ProcessMessages();
	//if(!PauseMode){
	//if(div(tmtmt,41).rem==2){
	//	SUSP("CreateStrategyInfo");
	//	CreateStrategyInfo();
	//};
	SUSP("ProcessFishing");
	ProcessFishing();
	SUSP("HandleMines");
	HandleMines();
	//UnitAbilityTest();
	SUSP("WinnerControl");
	//RunPF(8);
	addrand(tmtmt);
	if((tmtmt&255)==1)ResearchCurrentIsland((tmtmt>>8)&7);
	if((tmtmt&127)==7)ResearchBestPortToFish((tmtmt>>7)&7);
	if(SHOWSLIDE)WinnerControl(false);
//	StopPF(8);
	//PrevTime=GetRealTime();
#ifdef NEWMORALE
	void RemakeMaxMorale();
	RemakeMaxMorale();
#else //NEWMORALE
	DecreaseVeruVPobedu();
#endif //NEWMORALE
	if(tima!=time(NULL)){
		int FL0=tmtim;
		Flips=FL0;//(Flips*20+FL0)/21;
		tmtim=0;
		tima=time(NULL);
	};
	if(NPlayers<=1){
		int TT=GetTickCount();
		if(TT-LASTT>400||!STARTT||!NOPAUSE){
			STARTT=TT;
			STARTTICK=tmtmt;
		}
		LASTT=TT;
		SHOWSLIDE=1;
		int DTMT=tmtmt-STARTTICK;
		int DTIME=TT-STARTT;
		int GSP=0;
		if(GSets.SVOpt.RequiredMsPerFrame==120){
			CmdSetSpeed(256);
			if(NPlayers>1&&!PlayGameMode){
				Sleep(GSets.SVOpt.SleepTimeForDemo);//GECALKA
			}
		}else{
			if(DTMT>8){				
				void CmdSetSpeed(word);
				GSP=DTIME/DTMT;		
				if(GSets.SVOpt.RequiredMsPerFrame==0) GSets.SVOpt.RequiredMsPerFrame=EngSettings.DefaultGameSpeedForCampaign;
				int spd=GSets.SVOpt.RequiredMsPerFrame;				
				if(vGameMode!=gmCamp/*NPlayers>1*/){
				switch(GSets.RoomParams.GameSpeed){
				case 1:
					spd=30;//Fast
					break;
				case 2:
					spd=60;//Slow
					break;
				case 3:
					spd=20;//Very fast
					break;
				case 4:
					spd=10;//Very very fast
					break;
				case 5:
					spd=5;//Marvellous fast
					break;
				default:
					spd=40;//Normal
					break;
				}
					if(EditMapMode && GSets.SVOpt.RequiredMsPerFrame==3 )spd=GSets.SVOpt.RequiredMsPerFrame;
				}
				int GSpeed=GSP*256/spd;
				if(GSpeed<32)GSpeed=32;
				if(GSpeed>2048)GSpeed=2048;				
				if(IsMassiveGame()){
                    GameSpeed=GSpeed;
				}else{
					if(NPlayers==1){
						if(KillSpeedVariation){
							if(GSpeed>KillSpeedVariation)GSpeed=KillSpeedVariation;
						}
						//if(!GSets.SVOpt.GameSpeedAutoAdjustment){
						//	if(GSpeed>256)GSpeed=256;
						//}
					}
					CmdSetSpeed(GSpeed);
				}
			}
		}
		if(DTMT>50){
			STARTTICK=tmtmt-30;
			STARTT=TT-30*GSP;
		}
	}
	if(!(tmtmt&63)){
		LASTRAND=rando();
		LASTIND=rpos;
	};
	tmtim++;
	addrand(3);
	InternalProcess();
//	StopPF(19);
	//RunPF(20);
	if(NOPAUSE){
		tmtmt++;
		REALTIME++;//=FrmDec;
		/*
		// vital check
		static int T=0;
		if(TrueTime==0){
			T=GetTickCount();
		}
		Log.Warning("TrueTime=%d, TickCount=%d",TrueTime,GetTickCount()-T);
		*/
		int gs=GameSpeed/6;
		TrueTime+=gs;//GSets.SVOpt.RequiredMsPerFrame)/256;
		AnimTime+=gs*6;
		if(RealAnimTimeIsRun)RealAnimTime+=GameSpeed;
		for(int g=0;g<8;g++){
			if(CITY[g].AutoEraseTime){
				CITY[g].AutoEraseTime--;
				if(!CITY[g].AutoEraseTime){
					int SCORES[8];
					for(int i=0;i<8;i++)SCORES[i]=CITY[i].Account;
					for(i=0;i<MAXOBJECT;i++){
						OneObject* OB=Group[i];
						if(OB&&(!OB->Sdoxlo)&&(OB->NNUM==g)){
							//erasing
							OB->delay=6000;
							if(OB->LockType)OB->RealDir=32;
							OB->Die();
							OB=Group[i];
							if(OB)OB->Sdoxlo=2500;
						};
					};
					for(i=0;i<8;i++)CITY[i].Account=SCORES[i];
				};
			};
		};
	};
	SUSP("ProcessSprites");
	//RunPF(7);
	addrand(2);
	if(NOPAUSE)ProcessSprites();
	//CheckArmies(NULL);
//	StopPF(7);
//	StopPF(20);
	//RunPF(21);
	for(int p=0;p<7;p++)if(!NATIONS[p].Harch){
		int   NMon=NtNUnits[p];
		word* mon=NatList[p];
		for(int q=0;q<NMon;q++){
			word MID=mon[q];
			if(MID!=0xFFFF){
				OneObject* OBJ=Group[MID];
				if(rando()<250&&OBJ&&OBJ->NewBuilding&&OBJ->NInside&&!OBJ->Sdoxlo){
					word Last=OBJ->Inside[OBJ->NInside-1];
					if(Last!=0xFFFF){
						OneObject* OB=Group[Last];
						if(OB && !OB->newMons->NotHungry)// && !u.isHero()) // Edit by Xeno to fix necro-death bug
						{
							OB->Nat->CITY->UnRegisterNewUnit(OB);
							OB->CloseObject();
							OBJ->NInside--;
							DelObject(OB);
							Group[OB->Index]=NULL;
						};
					};
				};
			};
		};
	};
	NMONS=0;
	SUSP("HandleTransport");
	HandleTransport();	
#ifdef _USE3D
	// Piro
	void PiroProcess();
	PiroProcess();
#endif
	//Alexander can insert there his callback
	void WiselyFormationsProcess();
	RunPF(256*5+26,"WiselyFormationsProcess");
	if(NATIONS->NMon)WiselyFormationsProcess();
	StopPF(256*5+26);
	addrand(1);
	ACTION_ProcessingGame();
	/////////////////////////////////////////
	int tmm=tmtmt&7;
	addrand(tmm);
	addrand(tmtmt);
	if(!tmm){
		RunPF(256*5+15,"CITY0");
		CITY[0].ProcessCreation();
		if(rando()<2048)CreateFields(0);
		StopPF(256*5+15);
	};
	addrand(tmtmt);
	if(NOPAUSE){
		for(int i=0;i<8;i++){
			int N=CITY[i].NBrigs;
			for(int j=0;j<N;j++){
				Brigade* BR=CITY[i].Brigs+j;
				if(BR->Enabled)
				{
					/*
					if(BR->BOrder&&BR->BOrder->Fast&&BR->BOrder->BLink)
					{
						BR->BOrder->BLink(BR);
					}
					*/
					if(BR->NewBOrder&&BR->NewBOrder->IsFast())
					{
						BR->NewBOrder->Process(BR);
					}
				}
				if(EngSettings.MoraleType==1&&BR->Enabled&&BR->WarType){
                    NewMonster* NM=NATIONS[i].Mon[BR->MembID]->newMons;
					int mm=(BR->NMemb-NBPERSONAL)*10000;
					if(BR->Morale<mm)BR->Morale+=(GameSpeed*NM->MoraleRegenerationSpeed)/256;
				}
			}
		}
		CHECKIT();
		if(tmm==1){
			RunPF(256*5+16,"CITY1");
			CITY[1].ProcessCreation();
			if(rando()<2048)CreateFields(1);
			StopPF(256*5+16);
		};
		CHECKIT();
		if(tmm==2){
			RunPF(256*5+17,"CITY2");
			CITY[2].ProcessCreation();
			if(rando()<2048)CreateFields(2);
			StopPF(256*5+17);
			addrand(tmtmt);
		};
		if(tmm==3){
			RunPF(256*5+18,"CITY3");
			CITY[3].ProcessCreation();
			if(rando()<2048)CreateFields(3);
			StopPF(256*5+18);
			addrand(tmtmt);
		};
		if(tmm==4){
			RunPF(256*5+19,"CITY4");
			CITY[4].ProcessCreation();
			if(rando()<2048)CreateFields(4);
			StopPF(256*5+19);
			addrand(tmtmt);
		};
		if(tmm==5){
			RunPF(256*5+20,"CITY5");
			CITY[5].ProcessCreation();
			if(rando()<2048)CreateFields(5);
			StopPF(256*5+20);
			addrand(tmtmt);
		};
		if(tmm==6){
			RunPF(256*5+21,"CITY6");
			CITY[6].ProcessCreation();
			if(rando()<2048)CreateFields(6);
			StopPF(256*5+21);
			addrand(tmtmt);
		};
		if(tmm==7||TutOver){
			RunPF(256*5+22,"CITY7");
			CITY[7].ProcessCreation();
			addrand(tmtmt);
			HandleMission();
			addrand(tmtmt);
			TutOver=0;
			StopPF(256*5+22);
			addrand(tmtmt);
		}
		int xt=(tmtmt&255);
		CHECKIT();
		//GNFO.Process();
		CHECKIT();
		for(i=0;i<8;i++){
			addrand(i);
			Nation* NT=NATIONS+i;
			NT->Harch+=NT->NGidot*ResPerUnit;
			int mult=2000*256/GameSpeed;
			int DHarch=NT->Harch/mult;
			/*if(DHarch){
				if(XRESRC(i,EatenRes)>DHarch){
					addrand(XRESRC(i,EatenRes));
					AddXRESRC(i,EatenRes,-DHarch);
					addrand(XRESRC(i,EatenRes));
					NATIONS[i].ResOnLife[EatenRes]+=DHarch; //TBH Experiment
					NT->AddResource(EatenRes,-DHarch);
					NT->Harch-=mult*DHarch;
					if(!NT->Harch)NT->Harch=1;
				}else{
					SetXRESRC(i,EatenRes,0);
					NT->Harch=0;
				};		

			};*/
			if(!NT->Harch){
				if(XRESRC(i,FoodID))NT->Harch=1;
				NATIONS[i].ResOnLife[FoodID]++;
			};
			mult=2000000*256/GameSpeed;
            rando();
			for(int j=0;j<8;j++){
				addrand(j);
				int R=NT->ResRem[j];
				R+=NT->ResSpeed[j]*10*256/*GameSpeed*//25;
				div_t dd=div(R,mult);
				R=dd.rem;
				addrand(XRESRC(i,j));
				addrand(dd.quot);
				AddXRESRC(i,j,-dd.quot);
				addrand(XRESRC(i,j));
				NATIONS[i].ResOnLife[j]+=dd.quot;
				if(XRESRC(i,j)<0){
					NATIONS[i].ResOnLife[j]+=XRESRC(i,j);
					SetXRESRC(i,j,0);
				};
				NT->AddResource(j,-dd.quot);
				NT->ResRem[j]=R;
				if(j==GoldID){
					if(XRESRC(i,j)<2)NT->GoldBunt=true;
					else NT->GoldBunt=false;
				};
			};
		};	
		rando();
		CHECKIT();
		ProcessNewMonsters();
		ObjTimer.Handle();
		CHECKIT();
	}else{
		byte Mask=NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMask;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			void UnitLight(OneObject* OB);
			if(OB && OB->Sdoxlo==0 && OB->NMask & Mask)UnitLight(OB);
		}
	}
//	StopPF(21);
	RunPF(1024+9,"ProcessExpl");
	ProcessExpl();
	StopPF(1024+9);
	for(i=0;i<8;i++){
		addrand(i);
		HandleShar(NATIONS+i);
	};

#ifndef HASH_TOP
	SUSP("DynamicalTopology");
	ProcessDynamicalTopology();
#endif //HASH_TOP
	StopPF((256*5)+7);
	addrand(46);	
	InternalProcess();
};
bool ProcessMessages();
extern word NPlayers;;
void CmdSaveNetworkGame(byte NI,int ID,char* Name);
int SaveTime;
extern char SaveFileName[128];
void ProcessNature();
bool NeedEBuf=0;
int GLOBALTIME=0;
int PGLOBALTIME=0;
int PitchTicks=0;
int MaxPingTime=0;
int RealPause=0;
int RealStTime=0;
int RealGameLength=0;
int CurrentStepTime=80;
int GetRealTime();
void MakePause(int dt){
	int t0=GetRealTime();
	do{
		ProcessMessages();
	}while(GetRealTime()-t0<dt);
};
int SUBTIME=0;
void ProcessScreen();
void GSYSDRAW();
extern int StepX;
extern int StepY;
int TAverage=50;
void DrawScr();
void ADDGR(int g,int t,int v,byte c);
void WaitToTime(int Time){
	int T00=GetTickCount();
	int dt0=int(Time)-int(GetRealTime());
	ADDGR(6,tmtmt,dt0,0xD0);
	bool DoDraw=dt0>(TAverage>>1);
	do{
		if(DoDraw){
			int T0=GetRealTime();
			if(T0-Time<0){
				int tt=T0;
				DrawScr();
				int dt=GetRealTime()-tt;
				TAverage=(TAverage+TAverage+TAverage+dt)>>2;
				SUBTIME+=GetRealTime()-T0;
			};
		};
		ProcessMessages();
	}while(int(Time)-int(GetRealTime())>0);
	int ttx=GetRealTime();
	//ADDGR(9,T00,GetTickCount()-T00,0xD0);
	//ADDGR(9,T00,0,0x00);
	//ADDGR(9,T00,20,0x01);
};
int NeedCurrentTime=0;
extern bool PreNoPause;
extern EXBUFFER EBufs[MaxPL];
void StopPlayCD();
void ProcessUpdate();
extern byte CaptState;
extern byte SaveState;
void WritePitchTicks();
void ReadPichTicks();
void ShowCentralText0(char* sss);
void CmdChangePeaceTimeStage(int Stage);
int PrevCheckTime=0;
extern int PeaceTimeStage;
extern int PeaceTimeLeft;
#define CHECKNSL for(int i=0;i<7;i++)addrand(NSL[i]+1);
void PostDrawGameProcess(){
	RunPF(1024,"PostDrawGameProcess");
	RGAME.TryToFlushNetworkStream(0);
	if(PlayGameMode==0&&(NPlayers<2||vGameMode==gmSin))PitchTicks=0;
	if(PlayGameMode==0){
		if(MaxPingTime)WaitToTime(NeedCurrentTime);
		else PitchTicks=0;
	};
	if(PlayGameMode){
		ReadPichTicks();
		if(PitchTicks)MaxPingTime=1;
		else MaxPingTime=0;
	}else{
		WritePitchTicks();
	};
	//ProcessNature();
	void ProcessAmbientSound();
	ProcessAmbientSound();
	if(MaxPingTime){
		//addrand(CurrentStepTime);
		addrand(PitchTicks);
		//addrand(MaxPingTime);
	};
	NeedEBuf=0;
	GLOBALTIME++;
	if(RealStTime==0)RealGameLength=0;
	NeedCurrentTime+=CurrentStepTime;
	if(GLOBALTIME-PGLOBALTIME>PitchTicks){
		CurrentStepTime-=CurrentStepTime>>5;
		RealGameLength=GetRealTime()-RealStTime;
		RunPF(1024+1,"HandleMultiplayer");
		HandleMultiplayer();
        StopPF(1024+1);
		SYN1.Clear();
		SYN.Copy(&SYN1);
		//SYN1.Save();
		PreNoPause=0;
		ExecuteBuffer();
		if(PreNoPause)NOPAUSE=0;
		PGLOBALTIME=GLOBALTIME;
		RealStTime=GetRealTime();
		if(PlayGameMode){
			addrand(PitchTicks);
			ReadPichTicks();
			addrand(PitchTicks);
			if(PitchTicks)MaxPingTime=1;
		}else{
			if(NPlayers>1&&MaxPingTime){
				if(CurrentStepTime){
					addrand(PitchTicks);
					addrand(MaxPingTime);
					addrand(CurrentStepTime);
					PitchTicks=2+((MaxPingTime)/CurrentStepTime);
					addrand(PitchTicks);
					//addrand(MaxPingTime);
				}else PitchTicks=0;
			}else PitchTicks=0;
			WritePitchTicks();
		};
	};
	//int stt0=GetRealTime();
	//if(RealPause>0)MakePause(RealPause/PitchTicks);
	//RealStTime-=GetRealTime()-stt0;
	//if(!HISPEED)SHOWSLIDE=true;
	//else SHOWSLIDE=!div(tmtmt,HISPEED+1).rem;
	//HandleHExpls();
	//HandleSW();

	if(PlayGameMode==2){
		SHOWSLIDE=!div(tmtmt,HISPEED+1).rem;
	}else SHOWSLIDE=true;

	int difTime=GetRealTime()-AutoTime;
	ProcessUpdate();
	extern int _AUTOSAVEPERIODV[];
	int MaxDT=_AUTOSAVEPERIODV[GSets.AutoSavePeriod]*1000*60;
	if(MaxDT==0)MaxDT=60000*2000;
    /*
	if(NPlayers<=1&&EngSettings.EnableAutosavingInMissions){
        SaveState=4;
	}
	switch(SaveState){
	case 0:
		MaxDT=60000;
		break;
	case 1:
		MaxDT=60000*2;
		break;
	case 2:
		MaxDT=60000*4;
		break;
	case 3:
		MaxDT=60000*6;
		break;
	case 4:
		MaxDT=60000*8;
		break;
	case 5:
		MaxDT=60000*10;
		break;
	case 6:
		MaxDT=60000*2000;
		break;
	};
	*/
	extern int LastScrollTime;
	extern bool WasSelectionBar;
	bool GoodTime=GSets.CGame.ViewMask&&GetTickCount()-LastScrollTime>1000&&!WasSelectionBar;
	if(GoodTime&&difTime>MaxDT){//&&!(PlayGameMode||SaveState==6)){
		if( !g_Navigator.IsPlayingTrack() ){
			if(NPlayers>1){
				//for(int i=0;i<NPlayers;i++){
				//	if(EBufs[i].Enabled){
				//		if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
				//			int NP=0;
				//			for(int j=0;j<NPlayers;j++)if(EBufs[j].Enabled)NP++;
				//			char cc1[128];
				//			sprintf(cc1,"Save\\NetAutoSave %d players",NP);
				//			//CmdSaveNetworkGame(MyNation,0,cc1);
				//		};
				//		i=100;
				//	};
				//};
			}else{
				if(!EditMapMode){
					if(NATIONS[MyNation].VictState!=1&&!SCENINF.LooseGame){
						ShowCentralText0(GetTextByID("AUTOSAVING"));
						FlipPages();
						char* GetAutoSaveFileName();
						char* FN=GetAutoSaveFileName();
						//void RenameAnyway(char* src,char* dst);
						//RenameAnyway("Save\\AUTO.sav","Save\\PREVIOUS_AUTO.sav");
						SaveGame(FN,FN,0);
						GSets.AutoSaveWasDone=true;
					};
				};
			};
		}
	    AutoTime=GetRealTime();
	};
	if(!PrevCheckTime)PrevCheckTime=GetRealTime();
	if(GetRealTime()-PrevCheckTime>30000){
		PrevCheckTime=GetRealTime();
		//if(PeaceTimeLeft/60<PeaceTimeStage){
			CmdChangePeaceTimeStage(PeaceTimeLeft/60);
		//};
	};
	/*
	if(NPlayers>1&&MyDPID==ServerDPID&&SaveTime-GetRealTime()>60000*5){
		CmdSaveNetworkGame(MyNation,GetRealTime(),"NETWORK SAVE");
		SaveTime=GetRealTime();
	};
	*/
	/*if((NPlayers<2||vGameMode==gmSin)){
		do{
			ProcessMessages(); 
			if(PauseMode)GameKeyCheck();
		}while((int(GetRealTime())-PrevTime<(FPSTime+FPSTime))||PauseMode);
	};*/
	PrevTime=GetRealTime();
	StopPF(1024);
};

/*
 * WinMain - initialization, message loop
 */
void InitWaves();
void AllGame();
extern byte MI_Mode;
extern int RES[8][8];
void ClearAllLModeData();
void PrepareToEdit(){
	ClearAllLModeData();
	ClearMStack();
	MI_Mode=1;
	ReliefBrush=3;
	TerrBrush=2;
	EditMedia=0;
	HeightEditMode=false;
	MEditMode=false;
	EditMapMode=true;
	FogMode=0;
	HelpMode=true;
	ChoosePosition=false;
	CheapMode=false;
	NMyUnits=1;
	NThemUnits=1;
	AutoTime=GetRealTime()+180000;
    ObjTimer.~TimeReq();
	InitWaves();
	PeaceMode=false;
	LockMode=0;
	SaveFileName[0]=0;
	LockBars.Clear();
	UnLockBars.Clear();
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			RES[i][j]=50000;
		};
	};
	void CreateMiniMap();
	CreateMiniMap();	
};
byte PlayGameMode=0;
//extern char GSets.CGame.cgi_CurrentMap[64];
extern int TIMECHANGE[8];
extern int AddTime;
extern int NeedAddTime;
extern byte XVIIIState;
extern char RECFILE[128];
void PerformNewUpgrade(Nation* NT,int UIndex,OneObject* OB);
extern byte MI_Mode;
bool IsGameActive();
void ClearAllLModeData();

void PrepareToGame(){
	ClearAllLModeData();
	RecordMode=0;
	if(!PlayGameMode){
		if(NPlayers>1){
			RecordMode=1;
			//sprintf(RECFILE,"Autorecord\\%s",GSets.CGame.cgi_CurrentMap);
		};
	};
	if(RecordMode&&!PlayGameMode){
		//RecordMode=1;
		RGAME.StartRecord(GSets.CGame.cgi_CurrentMap);
	};
	MI_Mode=1;
	memset(TIMECHANGE,0,sizeof TIMECHANGE);
	AddTime=0;
	NeedAddTime=0;
	ClearMStack();
	ReliefBrush=3;
	TerrBrush=2;
	EditMedia=0;
	HeightEditMode=false;
	MEditMode=false;
	EditMapMode=false;
	FogMode=1;
	HelpMode=false;
	ChoosePosition=false;
	CheapMode=false;
	NMyUnits=1;
	NThemUnits=1;
	SaveTime=GetRealTime();
	AutoTime=GetRealTime();
	//if(NPlayers>1)AutoTime=GetRealTime()+180000;
	//else AutoTime=GetRealTime()-60000;
    ObjTimer.~TimeReq();
	InitWaves();
	PeaceMode=false;
	LockMode=0;
	SaveFileName[0]=0;
	FrmDec=2;
	SpeedSh=1;
	exFMode=1;
	CurrentStepTime=80;
	ext_OnGameStart();
};
void ShowIntro(){

};
void SetupRenderStack();
//byte* BLOBA;
lpCHAR FLIST[4096];
int NFILES=0;
void AText(char* str){
	
};
void AFile(char* str){
	AText(str);
};
void SaveAllFiles(){
	AText("btne0.bpx");
	AText("btne1.bpx");
	AText("btne2.bpx");
	AText("btne3.bpx");
	AText("btne4.bpx");
	AText("btne5.bpx");
	AText("btne6.bpx");
	AText("btne7.bpx");
	AText("btne8.bpx");
	AText("btne9.bpx");
	AText("btne10.bpx");
	AText("btne11.bpx");
	AText("btnu0.bpx");
	AText("btnu1.bpx");
	AText("btnu2.bpx");
	AText("btnu3.bpx");
	AText("btnu4.bpx");
	AText("btnu5.bpx");
	AText("btnu6.bpx");
	AText("btnu7.bpx");
	AText("btnu8.bpx");
	AText("btnu9.bpx");
	AText("btnu10.bpx");
	AText("btnu11.bpx");
	AText("hied.bpx");
	AText("scr_l.bpx");
	AText("scr_r.bpx");
	AText("scr_u_g.bpx");
	AText("scrL0.bpx");
	AText("trees.bpx");
	AText("scr_10.bpx");
	AText("stones.bpx");
	AText("pan17.bpx");
	AText("opt.bpx");
	AText("scr_line.bpx");
	AText("Gala1.bpx");
	AText("Gala0.bpx");
	AText("mnmenu0.bpx");
	AText("mnmenu01.bpx");
	AText("pan16.bpx");
	AText("scr_v_u.bpx");
	AText("scr_v_d.bpx");
	AText("scr_v.bpx");
	AText("pan15.bpx");
	AText("scr_15.bpx");
	AText("units.bpx");
	AText("195x20.bpx");
	AText("scr_13.bpx");
	AText("scr_16.bpx");
	AText("edmenu.bpx");
	AText("textur.bpx");
	AText("scr_12.bpx");
	AText("scr_10.bpx");
	AText("multi_m0.bpx");
	AText("gf24y.rlc");
	AText("gf24w.rlc");
	AText("cur_mn.rlc");
	AText("agew_1.pal");
	AText("2w.pal");
	AText("1w.pal");
	/*
	GFILE* f=Gopen("Filelist.lst","w");
	for(int i=0;i<NFILES;i++)Gprintf(f,"%s\n",FLIST[i]);
	Gclose(f);
	*/
};
void CreateRadio();
extern int ScrollSpeed;
void UnLoading();
void EraseRND(){
	char** RNDF=NULL;
	int NRND=0;
	int MaxRND=0;
	DWORD* RndData=NULL;
	word* Ridx=NULL;

	WIN32_FIND_DATA FD;
	HANDLE HF=FindFirstFile("RN? *.m3d",&FD);
	if(HF!=INVALID_HANDLE_VALUE){
		bool r=true;
		do{
			if(NRND>=MaxRND){
				MaxRND+=300;
				RNDF=(char**)realloc(RNDF,4*MaxRND);
				RndData=(DWORD*)realloc(RndData,4*MaxRND);
				Ridx=(word*)realloc(Ridx,2*MaxRND);
			};
			Ridx[NRND]=NRND;
			RNDF[NRND]=znew(char,strlen(FD.cFileName)+1);
			strcpy(RNDF[NRND],FD.cFileName);
			RndData[NRND]=FD.ftCreationTime.dwHighDateTime;
			NRND++;
			r=FindNextFile(HF,&FD)!=0;
		}while(r);
		if(NRND>3){
			SortClass SORT;
			SORT.CheckSize(NRND);
			memcpy(SORT.Parms,RndData,4*NRND);
			memcpy(SORT.Uids,Ridx,2*NRND);
			SORT.NUids=NRND;
			SORT.Sort();
			memcpy(Ridx,SORT.Uids,2*NRND);
			SORT.Copy(Ridx);
			for(int i=0;i<NRND-3;i++){
				DeleteFile(RNDF[Ridx[i]]);
			};
		};
		if(NRND){
			for(int i=0;i<NRND;i++)free(RNDF[i]);
			free(RNDF);
			free(Ridx);
			free(RndData);
		};
	};
};

void __stdcall CDGINIT_INIT3();
void FilesInit();
void FilesExit();
void PlayCDTrack(int Id);
void PlayRandomTrack();
extern int PlayMode;
int GetRealTime();
void CreateReg(){
	char path [300];
	char path1[350];
	GetCurrentDirectory(300,path);
	int ps1=0;
	int ps=0;
	char c;
	do{
		c=path[ps];
		if(c=='\\'){
			path1[ps1]='\\';
			path1[ps1+1]='\\';
			ps1+=2;
		}else{
			path1[ps1]=c;
			ps1++;
		};
		ps++;
	}while(c);
	GFILE* f=Gopen("Cossacks.reg","w");
	Gprintf(f,"REGEDIT4\n[HKEY_LOCAL_MACHINE\\Software\\Microsoft\\DirectPlay]\n[HKEY_LOCAL_MACHINE\\Software\\Microsoft\\DirectPlay\\Applications]\n[HKEY_LOCAL_MACHINE\\Software\\Microsoft\\DirectPlay\\Applications\\Cossacks]\n\"CommandLine\"=\"\"\n\"CurrentDirectory\"=\"%s\"\n\"File\"=\"dmcr.exe\"\n\"Guid\"=\"{5BFDB060-06A4-11d0-9C4F-00A0C705475f}\"\n\"Path\"=\"%s\"\n",path1,path1);
	Gclose(f);
};
typedef bool tpShowDialog(int NModes,int* Sizex,int* Sizey,int* Current);
tpShowDialog* lpShowDialog;
extern int ModeLX[32];
extern int ModeLY[32];
extern int NModes;
//#ifndef MAKE_PTC
void SetCDVolume(int Vol);
bool RunSMD(){
	bool rs=0;
	//if(EnumModesOnly())
	{
		HINSTANCE hLib=LoadLibrary("SelectMode.dll ");
		if(hLib){
			GFILE* fff=Gopen("mode.dat","rt");
			RealLx=1024;
			RealLy=768;
			GSets.SVOpt.ScreenSizeX=1024;
			GSets.SVOpt.ScreenSizeY=768;
			GSets.SVOpt.SoundVolume=0;
			WorkSound=0;
			OrderSound=0;
			GSets.SVOpt.MusicVolume=0;
			extern int RequiredMsPerFrame;
			GSets.SVOpt.RequiredMsPerFrame=EngSettings.DefaultGameSpeedForCampaign;
			if(fff){
				Gscanf(fff,"%d%d%d%d%d%d%d%d%d%d%d",&GSets.SVOpt.ScreenSizeX,&GSets.SVOpt.ScreenSizeY,&GSets.SVOpt.SoundVolume,&OrderSound,&OrderSound,&GSets.SVOpt.MusicVolume,&FPSTime,&GSets.SVOpt.ScrollSpeed,&exFMode,&GSets.SVOpt.MusicPlayMode,&GSets.SVOpt.RequiredMsPerFrame);
				SetCDVolume(GSets.SVOpt.MusicVolume);
				Gclose(fff);
			};
			int cr=0;
			for(int j=0;j<NModes;j++)if(ModeLX[j]==GSets.SVOpt.ScreenSizeX&&ModeLY[j]==GSets.SVOpt.ScreenSizeY)cr=j;
			lpShowDialog=(tpShowDialog*)GetProcAddress(hLib,"ShowDialog");
			if(lpShowDialog){
				if(lpShowDialog(NModes,ModeLX,ModeLY,&cr)){
					GSets.SVOpt.ScreenSizeX=ModeLX[cr];
					GSets.SVOpt.ScreenSizeY=ModeLY[cr];
					GFILE* fff=Gopen("mode.dat","wt");
					if(fff){
						extern int RequiredMsPerFrame;
						Gprintf(fff,"%d %d %d %d %d %d %d %d %d %d %d",GSets.SVOpt.ScreenSizeX,GSets.SVOpt.ScreenSizeY,GSets.SVOpt.SoundVolume,OrderSound,OrderSound,GSets.SVOpt.MusicVolume,FPSTime,GSets.SVOpt.ScrollSpeed,exFMode,GSets.SVOpt.MusicPlayMode,RequiredMsPerFrame);
						SetCDVolume(GSets.SVOpt.MusicVolume);
						Gclose(fff);
					};
					rs=1;
				};
			};
			FreeLibrary(hLib);
		};
	};
	return rs;
};
//#endif
SFLB_DLLEXPORT SFLB_InitDialogs();
CEXPORT
word dwVersion=150;
//#define TESTFORCE
#ifdef STARFORCE
HGLOBAL PTR_AI=NULL;
void __declspec(dllexport) SFINIT0_InitAI(){
	PTR_AI=GlobalAlloc(GMEM_FIXED,4);
	((DWORD*)PTR_AI)[0]=0x4351A234;
};
HGLOBAL PTR_MISS=NULL;
void __declspec(dllexport) SFINIT1_InitMISS(){
	PTR_MISS=GlobalAlloc(GMEM_FIXED,4);
	((DWORD*)PTR_MISS)[0]=0x4351A234;
};
HGLOBAL PTR_LAND=NULL;
void __declspec(dllexport) SFINIT2_InitLAND(){
	PTR_LAND=GlobalAlloc(GMEM_FIXED,4);
	((DWORD*)PTR_LAND)[0]=0x4344EA34;
};
#endif
void Rept (LPSTR sz,...)
{
	/*
        char ach[256];
        va_list va;

        va_start( va, sz );
        vsprintf ( ach, sz, va );   
        va_end( va );
		FILE* f=fopen("log.log","a");
		fprintf(f,ach);
		fclose(f);
	*/
};
void LogIt (LPSTR sz,...)
{
	
        char ach[256];
        va_list va;

        va_start( va, sz );
        vsprintf ( ach, sz, va );   
        va_end( va );
		FILE* f=fopen("log.log","a");
		fprintf(f,ach);
		fclose(f);
	
};
extern bool RUNMAPEDITOR;
extern bool RUNUSERMISSION;
extern char USERMISSPATH[128];
void CheckAttr();
void TestHash();
void CheckIntegrity();
extern bool TOTALEXIT;
int GetRankByScore(int Score);
extern char LobbyVersion[32];
void InitWinTab();
void StartTest();
void FinExplorer();
char AI_Log[256]="";
char CurrentGameLogDir[256]="";
void ClearMessages();
extern MEMORYSTATUS FEX_MemStatus1;
void DelLog();
void DDLog (LPSTR sz,...);

void InitProfiler();
void KeyTestMem();

//*****************************************************************************
// ATTENTION! This class must be full copy from Launcher project!
//*****************************************************************************
// RegArgs
//*****************************************************************************
class RegArgs {
public:
	int GetRefreshRate() const { return m_nRefreshRate; }
	void SetRefreshRate(const int nRefreshRate) { m_nRefreshRate = nRefreshRate; }
	bool RefreshRateIsValid() const { return m_nRefreshRate >= RefreshRateMin && m_nRefreshRate <= RefreshRateMax; }

	void ReadWrite(bool ToRead, const char *RegCompany = "GSC Game World",
		const char *RegGame = "Heroes of Annihilated Empires");
private:
	int m_nRefreshRate;

	static const int RefreshRateMin = 60;
	static const int RefreshRateMax = 150;
} g_RegArgs;

// -----------------------------------------------------------------------------------
// RegReadWrite
// -----------------------------------------------------------------------------------
void RegArgs::ReadWrite(bool ToRead, const char *RegCompany, const char *RegGame) {
	if(ToRead) {
		m_nRefreshRate = 0;
	}
	const char RefreshRate[] = "RefreshRate";
	
	HKEY hSoftwareKey = NULL, hCompanyKey = NULL, hGameKey = NULL;
	BYTE Value[4] = {0, 0, 0, 0};
	DWORD dwValue = 0, dwType = 0;

	
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software", 0, KEY_READ | KEY_WRITE, &hSoftwareKey) == ERROR_SUCCESS) {
		if(RegCreateKeyEx(hSoftwareKey, RegCompany, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hCompanyKey, NULL) == ERROR_SUCCESS) {
			if(RegCreateKeyEx(hCompanyKey, RegGame, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hGameKey, NULL) == ERROR_SUCCESS) {
				if(ToRead) {
					dwValue = sizeof(dwValue);
					dwType = REG_DWORD;
					if(RegQueryValueEx(hGameKey, RefreshRate, 0, &dwType, Value, &dwValue) == ERROR_SUCCESS) {
						m_nRefreshRate = Value[0];
						if(!RefreshRateIsValid()) {
							m_nRefreshRate = 0;
						}
					}
				} else {
					Value[0] = m_nRefreshRate;
					RegSetValueEx(hGameKey, RefreshRate, 0, REG_DWORD, Value, sizeof(Value));
				}
				RegCloseKey(hGameKey);
			}
			RegCloseKey(hCompanyKey);
		}
		RegCloseKey(hSoftwareKey);
	}
}

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        LPSTR lpCmdLine, int nCmdShow)
{	
    // ManOwaR :: Initializing theovorb helper thread
    if ( tv_Init() != TV_OK ) {
        MessageBox( NULL , "Cannot start theovorb helper thread", "Media Thread Initialization Error" , MB_ICONERROR | MB_OK );
        return 1;
    };

	// Surface Processing Tools:
	if(lpCmdLine) {
		strupr(lpCmdLine);
		if(strstr(lpCmdLine, "/SURF")) {
			g_SurfProcess.Show(hInstance);
			ExitProcess(0);
			return 0;
		}
	}
	// Safe Refresh Rate:
	if(lpCmdLine && strlen(lpCmdLine) > 0) {
		strupr(lpCmdLine);
		if(strstr(lpCmdLine, "/SAFE")) {
			g_RefreshRateIsOverridden = true;
		}
/*		const char Magic[] = "/REFRESHRATE";
		char *c = strstr(lpCmdLine, Magic);
		if(c) {
			c += strlen(Magic);
			int Hz = atoi(c);
			if(Hz >= 60 && Hz <= 200) {
				g_RefreshRateOverriddenValue = Hz;
				g_RefreshRateIsOverridden = true;
			}
		}*/
	}
	g_RegArgs.ReadWrite(true);
	if(g_RegArgs.RefreshRateIsValid()) {
		g_RefreshRateOverriddenValue = g_RegArgs.GetRefreshRate();
		g_RefreshRateIsOverridden = true;
	}

	void RunAntiSuspendThread();	
	//TestHash();
	DelLog();
	FEX_BEGIN();
	if(FEX_MemStatus1.dwTotalVirtual<1024*1024*700){
		if(MessageBox( hwnd,"Not enough virtual memory (at least 700M required).\nMay be your Windows swap-file is limited.\n", "LOADING ERROR...", MB_RETRYCANCEL )==IDCANCEL)
			return 0;

	};
	FILE* F3=fopen("loading.log","w");
	if(F3)fclose(F3);

	FILE* F7=fopen("ai.log","r");
	if(F7){
		fscanf(F7,"%s",AI_Log);
		fclose(F7);
	};
	///SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_LOWEST);
	char* ss=strstr(lpCmdLine,"/MAPEDITOR");
	if(ss){
		RUNMAPEDITOR=1;
		strcpy(USERMISSPATH,ss+11);
	}else{
		ss=strstr(lpCmdLine,"/MISSION");
		if(ss){
			RUNUSERMISSION=1;
			strcpy(USERMISSPATH,ss+9);
		};
	};
	if(strstr(lpCmdLine,"/SAFEMEM")){
		extern bool SmartLeakDoctor;
		extern bool FAST_MEMORY;
		extern byte AUTO_SEEK_MANY_HANDLES;
		SmartLeakDoctor=true;
		FAST_MEMORY=false;
		AUTO_SEEK_MANY_HANDLES=1;
	}
	Shifter=5;
	Multip=0;
	ScreenPtr=NULL;
	SetupRenderStack();
	ChangeNation=false;
	MultiTvar=false;
	AutoTime=0;
	BlobMode=0;
	MEditMode=false;
	CostThickness=4;
	EditMedia=0;	
	//LoadMediaFile();
	SpecCmd=0;
	sfVersion=291;
	strcpy(BuildVersion,"V 2.10");//"V 1.46"); //CDV ASK GEC 13.03
	//Quality=2;
	RealLx=1024;
	RealLy=768;
	GSets.SVOpt.ScreenSizeX=1024;
	GSets.SVOpt.ScreenSizeY=768;
	GSets.SVOpt.SoundVolume=0;
	WorkSound=0;
	OrderSound=0;
	GSets.SVOpt.MusicVolume=0;
	GSets.SVOpt.RequiredMsPerFrame=40;
	WaterEditMode=false;
	InitObjs3();
	FPSTime=50;
	GFILE* FF1=Gopen("Lobby.txt","r");
	if(FF1){
		int vv=dwVersion;
		Gscanf(FF1,"%d %s %s",&vv,LobbyVersion,BuildVersion);
		dwVersion=dwVersion+(vv<<8);
		Gclose(FF1);
	};
	GFILE* fff=Gopen("mode.dat","rt");
	AText("mode.dat");
	GSets.SVOpt.ScrollSpeed=5;
	if(fff){
		Gscanf(fff,"%d%d%d%d%d%d%d%d%d%d",&GSets.SVOpt.ScreenSizeX,&GSets.SVOpt.ScreenSizeY,&GSets.SVOpt.SoundVolume,&OrderSound,&OrderSound,&GSets.SVOpt.MusicVolume,&FPSTime,&GSets.SVOpt.ScrollSpeed,&exFMode,&GSets.SVOpt.MusicPlayMode);
		Gclose(fff);
	};
	
#ifdef SELFRUN
	RealLx=GSets.SVOpt.ScreenSizeX;
	RealLy=GSets.SVOpt.ScreenSizeY;
#endif
	bool ExMode=0;
	for(int i=0;i<NModes;i++){
		if(ModeLX[i]==GSets.SVOpt.ScreenSizeX&&ModeLY[i]==GSets.SVOpt.ScreenSizeY)ExMode=1;
	};
	if(!ExMode){
		GSets.SVOpt.ScreenSizeX=1024;
		GSets.SVOpt.ScreenSizeY=768;
	};
	HealthMode=true;				
	tima=0;
	PlayerMask=1;
	Flips=0;
	tmtim=0;
	InfoMode=true;
    MSG         msg;
	DeathMode=false;
	AttackMode=false;
	SetDebugMode();

	LockGrid=false;
	/*
	FILE* F=fopen("cew.dll","r");
	if(!F){
		MessageBox(NULL,"CEW.DLL not found. Unable to run Cossacks.","Error...",MB_ICONERROR);
		return 0;
	}else fclose(F);	
	*/
	makeFden();
	MemReport("MakeFDen");
	PlayerMenuMode=1;
	Creator=4096+255;
	xxx=0;
    TransMode=false;
	MUSTDRAW=false;	
    DLL0(SFLB_InitDialogs);

	memset(Events,0,sizeof Events);
	MemReport("LoadingFonts");
	GetRealTime();
	_dbgPrintState("LoadStage","Before DoInit");
    if( !doInit( hInstance, nCmdShow ) )
    {
        return FALSE;
    };
	_dbgPrintState("LoadStage","DoInit done");
	if(strstr(lpCmdLine,"/PROFILE")){
		//InitProfiler();
	}

	if(strstr(lpCmdLine,"/LEAKS")){
		extern bool SmartLeakDoctor;
		SmartLeakDoctor=1;
	}

	// Forcing additional screen resolution from command line args (if any):
	const char *AddRes = strstr(lpCmdLine, "/ADDRES");
	if(AddRes != NULL) {
		cList<cStr> Tokens;
		cStr::Split(AddRes, Tokens);
		if(Tokens.Count() >= 3) { // At least we need two additional args with "X" and "Y".
			const int XRes = cStr::ToInt(Tokens[1]);
			const int YRes = cStr::ToInt(Tokens[2]);
			// Validating:
			if(XRes >= 320 && XRes <= 4096 && YRes >= 200 && YRes <= 4096) {
				// Resolution is valid. Append it to the end of the already enumerated list.
				ModeLX[NModes] = XRes;
				ModeLY[NModes] = YRes;
				NModes++;
				IRS->AddScreenResolution(XRes, YRes, 60);
			}
		}
	}

	extern IRenderSystem* IRS;
	ScreenProp prop = IRS->GetScreenProp();	

	extern bool g_bPerspCamera;
	g_bPerspCamera=!GSets.StartMode;
	//	
	EngSettings.CollidingCamera.Defaults();
	EngSettings.SafeReadFromFile("EngineSettings.xml");
	GSets.Update(); // For multisample type update.
	
	for(int i=0;i<EngSettings.PrecashedSprites.GetAmount();i++){		
	    ISM->Precache(EngSettings.PrecashedSprites[i]->gpFileName);
	}
	//
	if(GSets.LeibzigDemo.Enabled){
		RealLx=GSets.SVOpt.ScreenSizeX;
		RealLy=GSets.SVOpt.ScreenSizeY;
	}
	prop.m_Width = RealLx;
	prop.m_Height = RealLy;
	IRS->SetScreenProp( prop );
	GPS.SetClipArea( 0, 0, RealLx, RealLy );
	void TestSomeClass();
	TestSomeClass();
	//
	SetCDVolume(GSets.SVOpt.MusicVolume);
	extern bool RandomMode;
	RandomMode=GSets.SVOpt.MusicPlayMode==1;
	//mpl_Play("mp3\\co_Green01.mp3");
	//
	InitWinTab();
	LoadFog(2);
	LoadPalette("1\\agew_1.pal");
	MemReport("doInit");
	SetupMultiplayer(hInstance);
	MemReport("SetupMultiplayer");
	InitMultiDialogs();
	MemReport("InitMultiDialogs");
	CloseMemReport();
	SetupHint();
	tmtmt=0;
	REALTIME=0;
	TrueTime=0;
	AnimTime=0;
	//processMainMenu();
	KeyPressed=false;
	SetCursorPos(512,300);
	mouseX=0;
	mouseY=0;
	SetMPtr(512,300,0);
	mouseX=512;
	mouseY=300;
	OnMouseMoveRedraw(); 
	//if(H)FreeLibrary(H);
#ifdef _USE3D
	void LoadAllGModels();
	LoadAllGModels();
#endif //_USE3D
	//if(PlayMode)PlayRandomTrack();
	StartTest();
	_dbgPrintState("LoadStage","Before Video");

	GSets.LeibzigDemo.StartVideo();
	/*if(GSets.VideoList.GetAmount()){
		void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
		for(int i=0;i<GSets.VideoList.GetAmount();i++){
            PlayFullscreenVideo(GSets.VideoList[i]->File.pchar(),1.0-GSets.VideoList[i]->ScaleX,1.0-GSets.VideoList[i]->ScaleY);
		}
	}*/
	if(GSets.LeibzigDemo.Enabled){
		int n=GetTickCount()%1000;
		for(int i=0;i<n;i++)rand();
		do{
			do{
				GSets.LeibzigDemo.StartVideo();
				if(!GSets.LeibzigDemo.RecBroken){
					GSets.LeibzigDemo.StartRecord();
					if(!GSets.LeibzigDemo.RecBroken)return 0;
				}
			}while(!GSets.LeibzigDemo.RecBroken);
			GSets.LeibzigDemo.RecBroken=0;
			int nm=GSets.LeibzigDemo.MissionsList.GetAmount();
			if(nm){
				int p=rand()%nm;
				const char* mname=GSets.LeibzigDemo.GetStr(&GSets.LeibzigDemo.MissionsList,p);
				for(int i=0;i<MISSLIST.NMiss;i++)if(!strcmp(mname,MISSLIST.MISS[i].ID)){
					bool ProcessSingleMission(int n,int Diff);
					ProcessSingleMission(i,0);
					void PlayGame();
					//IRS->ClearDeviceTarget();
					PlayGame();
					return 0;
					break;
				}
			}
		}while(true);
		return;
	}
#ifdef SELFRUN
	void PlayRecfile(char* RecFile);
	do{
		extern bool HideBorderMode;
		extern bool NoMoveMode;
		void MakeAllDirtyGBUF();
		// by ManOwaR
		char szCommandLine[512],*pszOpt=NULL;
		strcpy(szCommandLine,GetCommandLine());
		pszOpt=strstr(szCommandLine,"\playrec=");
		if(pszOpt){			
			HideBorderMode=1;
			NoMoveMode=1;
			MakeAllDirtyGBUF();
			IRS->ClearDeviceTarget();
			PlayRecfile(pszOpt+8);
			return;
			IRS->ClearDeviceTarget();
			MakeAllDirtyGBUF();}
		else 
			return;
	}while(true);
#endif
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				PostQuitMessage(msg.wParam);
                FEX_END();
				return;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(bActive){
			//int Ticki=GetRealTime();	
			//while(GetRealTime()-Ticki<MsPerFrame);
			InVideo=0;
			_dbgPrintState("LoadStage","Before AllGame");
			AllGame();
			_dbgPrintState("LoadStage","After AllGame");
			
			//
            void vmPlayerDataWrite();
			vmPlayerDataWrite();
			//

			ClearScreen();
			GSets.Save();
			UnLoading();
			CloseExplosions();
			ShutdownMultiplayer(1);
			//for(int i=0;i<30000000;i++)if(BLOBA[i]!=0x41)assert(false);
			//free(BLOBA);
			
			SaveAllFiles();
			GFILE* fff=Gopen("mode.dat","wt");
			if(fff){
				Gprintf(fff,"%d %d %d %d %d %d %d %d %d %d",GSets.SVOpt.ScreenSizeX,GSets.SVOpt.ScreenSizeY,GSets.SVOpt.SoundVolume,OrderSound,OrderSound,GSets.SVOpt.MusicVolume,FPSTime,GSets.SVOpt.ScrollSpeed,exFMode,GSets.SVOpt.MusicPlayMode);
				Gclose(fff);
			};
			//CDS->~CDirSound();
			FilesExit();
			StopPlayCD();
			if(TOTALEXIT)
				ShellExecute(NULL,"open","http://www.goa.com/goa/z-home.asp?gotogame=8247",NULL,NULL,SW_MAXIMIZE);
#ifdef STARFORCE
			if(PTR_AI)GlobalFree(PTR_AI);
			if(PTR_MISS)GlobalFree(PTR_MISS);
#endif
			ClearMessages();
			void FreeAll();
			FreeAll();
		    PostMessage(hwnd, WM_CLOSE, 0, 0);
			FinExplorer();
		};
	};
	FEX_END();
    return 0;

}; /* WinMain */
void CleanAll();
extern int* DMap;
void ClearRadio();
void ClearNatList();
void CloseSoundSystem();
void FreeIcons();
void FreeNations();
void FreeRDS();
void FreeTextures();
extern RLCTable mptr;
void FreeWNames();
void FreeFiresAnm();
void CloseExplosions();
void ClearMonNames();
void FreeAll(){
	ClearMonNames();
	CloseExplosions();
	FreeFiresAnm();
	FreeWNames();
	FreeTextures();
	FreeRDS();
	FreeNations();
	FreeIcons();
	CloseSoundSystem();
	ClearNatList();
	CleanAll();
	free(DMap);
	ClearRadio();
	if(mptr)free(mptr);
	mptr=NULL;
};
void ShowProfInit(){
    IRS->ClearDevice(0xFF4040A0);
    void DrawCenteredText(int x,int y,RLCFont* F,char* s,...);
	DrawCenteredText(RealLx/2,RealLy/2,&BigWhiteFont,"Profiler initialisation.\\Please wait...\\ \\{FN}Then you can press NumLock to enable profiling.\\To view results, unpress NumLock and press CTRL-ALT-SHIFT-SPACE{F}");
	FlipPages();
}
bool CanPlayInternetGame=0;
void CheckEngineSumm(){	
    ProcessMessages();
	DWORD CD=0;
    GFILE* F=Gopen("flist.dat","r");
	if(F){
		char s[256];
		int r=0;
		DWORD S=0;
		DWORD XOR=0x12365498;
		do{
			r=ReadWinString(F,s,255);
			if(r){
				ResFile FL=RReset(s);
				if(FL!=INVALID_HANDLE_VALUE){
					int sz=RFileSize(FL);
                    char* buf=new char[sz];
					RBlockRead(FL,buf,sz);
					for(int i=0;i<sz;i++)S+=buf[i];
					sz>>=2;
					for(int i=0;i<sz;i++)S+=((DWORD*)buf)[i]^XOR;
					RClose(FL);
				}
			}
		}while(r);
        Gclose(F);
		char s2[16];
		s2[0]='f';
		s2[1]='c';
		s2[2]='d';
		s2[3]=0;
		strcat(s2,".dat");
		F=Gopen(s2,"r");
		CD=S;
		if(F){
			DWORD X=0;
			Gscanf(F,"%X",&X);
			Gclose(F);
			CanPlayInternetGame=X==S;
			if( (GetKeyState('K')&0x8000) && (GetKeyState('E')&0x8000) && (GetKeyState('Y')&0x8000) ){
				F=Gopen(s2,"w");
				if(F){
					Gprintf(F,"%X",S);
					Gclose(F);
				}
			}
		}
	}
    sprintf(LobbyVersion+strlen(LobbyVersion)," [%04X]",CD&65535);
}