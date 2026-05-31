#include "stdheader.h"
#include "Globals.h"
#include "FCompressor.h"
#include "vField.h"
#include "DrawModels.h"
#include "StringHash.h"
#pragma pack(push)
#pragma pack(8)
#include "FMM\FMManager.h"
#include "BrigadeAI.h"
#include "TacticalAI.h"
#include "IResourceManager.h"
#pragma pack(pop)
FMManager MManager;
CDirSound CDIRSND;
CGSCset GSFILES;
GFSYSTEM GFILES;
CCommCore IPCORE;
PLAYERSBACKUP PBACK;
PingSumm PSUMM;
NetCash NCASH;
RETRANS RETSYS;
LoosedPack LPACK;
TotalPeerEngine TPEN;
GlobalEnemyInfo GNFO;
Abilky ABILS;
SafeNet SAFNET[8];
//Combat C;
BlockBars LockBars;
BlockBars UnLockBars;
BlockBars HorceLockBars;
LocalGP ReportPanel;
SamplesSet SAMSET;
GlobalProgress GPROG;
BaseMesh BMS;
GP_System GPS;
UNIFONTS UFONTS;
FCompressor FCOMP;
Graph GRPS[10];
FieldModel	g_FieldModel;
AttList ATTLIST;
BordersScope BScope;
SortClass UNISORT;
PositionOrder PORD;
FCompressor FCOMP2;
OrdersScope OSC;
DialogsSystem INFORM(0,0);
DialogsSystem UNITINF(0,0);
DialogsSystem MINIMENU(0,0);
DialogsSystem RESSEND(0,0);
SprGroup TREES;
SprGroup STONES;
SprGroup HOLES;
SprGroup SPECIAL;
SprGroup ANMSPR;
SprGroup COMPLEX;
SprGroup WALLS;
SprGroup MODELS3D;
TimeReq ObjTimer;
ModelsScope RenderModels;
MotionField UnitsField;
Nation NATIONS[8];
ResDiscr RDS[8];
FormGroupDescription FormGrp;
OrderClassDescription OrderDesc[16];
OrderDescription ElementaryOrders[256];
AI_Description GlobalAI;
NewMonster* NewMon[2048];
NewAnimation WNewAnm[768];
StrHash UPGS;
MotionField MFIELDS[NMFIELDS];//0-Land,1-Water
MotionField TempMF;
RecordGame RGAME;
MissPack MISSLIST;
CampaginPack CAMPAGINS;
SinBattles SBattles;

DynArray<Rules*> BrigadeAI::AllRules;
BrigadeAI WiselyFormations;
TacticalAI T_AI;
void InitGlobals(){
    char cwd[_MAX_PATH];
    getcwd( cwd, _MAX_PATH );
    _chdir( IRM->GetHomeDirectory() );
    GlobalAI.Init();
	//
	Enumerator* E=ENUM.Get("NationsName");
	for(int i=0;i<GlobalAI.NAi;i++){
		E->Add(GlobalAI.Ai[i].NationID);
	};
	//
	void LoadConstStr();
	LoadConstStr();
	MISSLIST.LoadMissions();
	CAMPAGINS.Init();
	SBattles.Init();
	void InitGameConst();
	InitGameConst();
	void InitBrigVars();
	InitBrigVars();
	void SetupNatList();
	SetupNatList();
	void InitFishMap();
	InitFishMap();
	void CreateRadio();
	CreateRadio();
	void FilesInit();
	FilesInit();
	void EraseRND();
	EraseRND();
	void InitStack();
	InitStack();
	void InitEBuf();
	InitEBuf();
	void InitWater();
	InitWater();
}