#ifndef __MapDiscr__
#define __MapDiscr__
#define FANTASY

//you should specify this define to protect using this exe for russian localisation
//#define NORUSSTEXT
/*                    Map discription
 *    
 * This file describes map cells, animations, monsters, buildings,
 * flying monsters, on-water monsters...
 */
//#define CDVERSION
//#define CHECK_SYNC_FAILURE
#define NO_CHEATS_IN_MULTIPLAYER
#define CONQUEST
#define NOBOIDS
//#define NOLITTLESHIFT
#pragma pack(1)

#ifndef DIALOGS_API
#ifndef DIALOGS_USER
#define DIALOGS_API __declspec(dllexport)
#else//DIALOGS_USER
#define DIALOGS_API __declspec(dllimport)
#define BaseMesh void
#endif//DIALOGS_USER
#endif//DIALOGS_API

typedef unsigned short word;

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "smart_assert.h"
#include <DString.h>
#include <xmlQuote.h>
#include <ClassEngine.h>
#include <more_types.h>
#include <IMediaManager.h>
#include "AntiBug.h"
#include "Icons.h"

#define ULIMIT 65535
#define LULIMIT 65000
extern byte MYNATION;
#define SetMyNation(x) {MYNATION=x^133;}
#define MyNation (MYNATION^133)
#define CEXPORT __declspec(dllexport)
#define NCOMM 2
extern int GameSpeed;
CEXPORT
char* GetTextByID(char* ID);
#define BRIGDELAY 50
//#define ADDSH 1
extern int ADDSH;
#include "gFile.h"
//Maximum size of cells map
extern int MAXCX;
#define MAXCY MAXCX
#include "MapTemplates.h"
#include "StructuresExtensions.h"
//#define MAXCX (64<<ADDSH)
//#define MAXCY (64<<ADDSH)
//#define MAXCIOFS (MAXCX*MAXCY)
//maximal amount of units in cell
#define MAXINCELL 256
//A*MAXCX=A<<SHFCX
//#define SHFCX (6+ADDSH)
//A*MAXINCELL=SHFCELL
#define SHFCELL 8
//Size of cell
#define CELLSIZE 4
//A*CELLSIZE=A<<CELL2
#define CELL2 2
extern int URESRC[8][8];
#define XRESRC(i,j) (URESRC[i][j]^134525)
#define SetXRESRC(i,j,k) URESRC[i][j]=(k^134525)
#define AddXRESRC(i,j,k) URESRC[i][j]=(((URESRC[i][j]^134525)+k)^134525);
extern int VAL_SHFCX;
extern int VAL_MAXCX;
extern int VAL_MAXCIOFS;
extern int VAL_SPRNX;
extern int VAL_SPRSIZE;
extern int VAL_MAPSX;
extern int MapShift;
extern int WLX;
extern int WMPSIZE;
extern int MaxWX;
extern int MaxWY;
extern int MAPSX;
extern int MAPSY;
extern int MAPSHF;
extern int BMSX;
extern int TopLx;
extern int TopLy;
extern int MaxTop;
extern int TopSH;
extern int B3SX;
extern int B3SY;
extern int B3SZ;
extern int MaxSector;
extern int MaxTH;
extern int MTHShift;
extern int VertInLine;
extern int SectInLine;
extern int MaxPointIndex;
extern int MaxLineIndex;
extern int StratLx;
extern short* THMap;// Map of heights in vertices
//extern byte* AddTHMap;
extern byte* TexMap;//Map of textures in vertices
extern int MAXCIOFS;
extern int TSX;

#define SINGLE_ORDER 0
#define HEAD_ORDER   1
#define TILE_ORDER   2
struct Coor3D{
	int x,y,z;
};
typedef void HandlePro(int);
class Weapon;
class SelGroup;
class NewMonster;
#ifdef _USE3D
class OneObjectEx;
#endif // _USE3D

//���� ������ � ���������)
struct MapCell{
	//word Flags;		//������������
//	bool LandLock:1;	//��� ����� � ����
	bool WaterLock:1;	//��� ����� � ���
	bool AirLock:1;		//��� ����� � ���
	bool TempLock:1;    //����������������
	byte LayerID;		//��� �� � �������� �������
    word MonsterID;		//���������������� 
	word BuildingID;	//������ ���������
	word FlyID;			//��� ���
//	word TileID;		//�������, ���� �����
};//8 bytes

//�������������
struct ObjectRef{
	unsigned ObjID:8;	//�����
	unsigned Location:2;//����������/�� ���
	void* lpObj;		//���� ���
};
//����������� ����
#define MaxObject ULIMIT
//��� ����
typedef ObjectRef ObjArray[MaxObject];
class Brigade;
//���� ����
struct OneSlide{
	word  FileID;   //����������
	small dx;		//���� ����� ������
	small dy;		//-----/----- � �����
	word  spr;		//����������� ���
	word SoundID;   //������
};
typedef OneSlide MovieType[256];
typedef MovieType* lpOneMovie ;
struct  Octant{
	byte count;
	byte ticks;
	small gdx;
	small gdy;
	OneSlide* Movie;
};
typedef Octant Animation[8];
//����� ����� �� �����
struct MoreAnimation{
	Octant* Anm;
	word WhatFor;		//������������ ��� ����
	word Kind;          // 
	//word Reserved;
};
typedef MoreAnimation AnimArray[32]; 
typedef word WordArray[32768];
typedef WordArray* lpWordArray;
struct ObjIcon{
	//word FileID;
	word spr;
};
#include "Upgrade.h"
//���� ��������������)
#define EmptyID 0
//#define MonsterID 1

//---------------------SYCRO------------------
DLLEXPORT
int RandNew(char*,int);
DLLEXPORT
void AddRandNew(char*,int,int);
DLLEXPORT
void AddUN(char* File,int Line,int Param,int Type);

#ifdef CHECK_SYNC_FAILURE

#define rando() RandNew(__FILE__,__LINE__)
#define addrand(v) AddRandNew(__FILE__,__LINE__,v)
#define addname(v) AddUN(__FILE__,__LINE__,v,0)

#else

#define rando() RandNew(__FILE__,__LINE__)
#define addrand(v)
#define addname(v)

#endif

#include "NewAnimation.h"


class MonsterAbility;
class UnitAbility;

#include "ExIcons.h"
#include "NewMonster.h"
class GOrder;
struct GlobalIconInfo{
	HandlePro* HPLeft;
	HandlePro* HPRight;
	int LParam;
	int RParam;
	int IconSpriteID;
	char* Hint;
};
typedef bool GOrderFn(OneObject* OB,GOrder* GOR,int LParam,int RParam);
typedef int IconInfo(GOrder* GOR,int IcoIndex,OneObject* OB,GlobalIconInfo* GIN);
class GOrder{
public:
	GOrderFn* CheckDisconnectionAbility;
	GOrderFn* Disconnect;
	GOrderFn* KillOrder;
	IconInfo* GetIcon;
	void*     Data;
	GOrder();
	~GOrder();
};
class Legion;
class Nation;
class FireObjectInfo{
public:
	byte* FireSprite;
	byte* FireStage;
	byte delay;
	short NeedFires;
	short RealFires;
	void  Erase();
};
class FireInfo{
public:
	byte BStage;
	FireObjectInfo* Objs[2];
	void Erase();
};
class UnitActiveAbilityList;
//����� � ����������
#include "OneObject.h"
#pragma warning( default : 4035  )
//���� ���
class Nation;

//���� ��������(��� �����)
#include "Nation.h"

extern OneObject* Group[ULIMIT];

typedef char** lplpCHAR;
typedef char*  lpCHAR;
typedef int*   lpINT;
class SelGroup{
public:
	word* Member;
	word* SerialN;
	word NMemb;
	bool CanMove:1;
	bool CanSearchVictim:1;
	bool CanHelpToFriend:1;
	bool Egoizm:1;
	SelGroup();
	void CreateFromSelection(byte NI);
	void SelectMembers(byte NI,bool Shift);
	void DeleteMembers(byte NI);
	void ImSelectMembers(byte NI,bool Shift);
};


//��� ������ � ���
#define MaxObj ULIMIT
#define maximage 2048

extern RLCTable MImage[maximage];
extern RLCTable miniMImage[maximage];
void LoadMonsters();
#define maxmap (128<<1)//ADDSH)  //Don't change it!
void LoadLock();
#define MaxAsmCount 16384
#define OneAsmSize 256
#define OneAShift 8;
#define MaxOrdCount 32768
#define OneOrdSize 32;
#define OneOShift 5;
char* GetAsmBlock();
void FreeAsmBlock(char* p );
void InitAsmBuf();
Order1* GetOrdBlock();
//void FreeOrdBlock(Order1* p );
void InitOrdBuf();
extern Order1  OrdBuf[MaxOrdCount];
extern bool	AsmUsage[MaxAsmCount];
extern int	msx;
extern int msy;
extern void Except();
//��� ����� �����(2^n only !)
#define StSize 8192
#define StMask StSize-1;
extern word ComStc[StSize];
extern word StHead;
extern word StTile;
#define FreeTime 600;
void CarryOutOrder();
void InitStack();
void doooo();
extern word Creator;
extern Nation NAT;
extern int	smapx;
extern int	smapy;
extern int	smaplx;
extern int	smaply;
extern int minix;
extern int	miniy;
//extern HWND hwnd;
void MakePostProcess();
void MakeWPostProcess();
void PrepareProcessing();
extern int Flips;
//extern void FreeOrdBlock(Order1* p );
extern int	mapx;
extern int	mapy;
//byte CreateExObj(Weapon* Wep,short x,short y,
//				 short dx,short dy,short v,byte Mask,OneObject* Send);
//byte CreateExObjDPoint(Weapon* Wep,short x,short y,
//				 short dx,short dy,short v,byte Mask,OneObject* Send,byte dsx,byte dsy);
void InitExplosions();
void ProcessExpl();
extern Weapon FlyFire1;
extern Weapon Vibux1;
void CloseExplosions();
extern byte PlayerMask;
extern bool EgoFlag;
void AddAsk(word ReqID,byte x,byte y,char zdx,char zdy);
extern SelGroup SelSet[80];
extern Weapon* WPLIST[1024];
int CreateZone(int x,int y,int lx,int ly,HandlePro* HPro,int Index,char* Hint);
void DeleteZone(int i);
void ShowProp();
void InitPrpBar();
void ShowAbility();
extern word* Selm[8];
extern word* SerN[8];
extern word* ImSelm[8];
extern word* ImSerN[8];
extern word ImNSL[8];
extern word NSL[8];
void CmdCreateSelection(byte NI,byte x,byte y,byte x1,byte y1);
void CmdSendToXY(byte NI,int x,int y,short Dir);
void CmdAttackObj(byte NI,word ObjID,short DIR);
void CmdCreateTerrain(byte NI,byte x,byte y,word Type);
void CmdCreateBuilding(byte NI,int x,int y,word Type);
void CmdProduceObj(byte NI,word Type);
void CmdMemSelection(byte NI,byte Index);
void CmdRememSelection(byte NI,byte Index);
void CmdBuildObj(byte NI,word ObjID);
void CmdBuildWall(byte NI,short xx,short yy);
void CmdRepairWall(byte NI,short xx,short yy);
void CmdDamageWall(byte NI,word LIN);
void CmdTakeRes(byte NI,int x,int y,byte ResID);
void CmdPerformUpgrade(byte NI,word UI);
void CmdCreateKindSelection(byte NI,byte x,byte y,byte x1,byte y1,byte Kind);
void CmdCreateTypeSelection(byte NI,byte x,byte y,byte x1,byte y1,word Type);
void CmdCreateGoodSelection(byte NI,int x,int y,int x1,int y1);
void CmdCreateGoodKindSelection(byte NI,int x,int y,int x1,int y1,byte Kind);
void CmdCreateGoodTypeSelection(byte NI,int x,int y,int x1,int y1,word Type);
void CmdSetDst(byte NI,int x,int y);
void CmdSendToPoint(byte NI,byte x,byte y);
void CmdAttackToXY(byte NI,byte x,byte y);
void CmdStop(byte NI);
void CmdStandGround(byte NI);
void CmdPatrol(byte NI,int x,int y);
void CmdRepair(byte NI,byte x,byte y);
void CmdGetResource(byte NI,byte x,byte y);
void CmdSendToTransport(byte NI,word ID);
void CmdUnload(byte NI,byte x,byte y);
void CmdDie(byte NI);
void CmdContinueAttackPoint(byte NI,byte x,byte y);
void CmdContinueAttackWall(byte NI,byte x,byte y);
void CmdSitDown(byte NI);
void CmdNucAtt(byte NI,byte x,byte y);
void CmdChooseSelType(byte NI,word ID);
void CmdChooseUnSelType(byte NI,word ID);
void CmdGoToMine(byte NI,word ID);
void CmdLeaveMine(byte NI,word Type);
void CmdErseBrigs(byte NI);
void CmdChooseSelBrig(byte NI,word ID);
void CmdChooseUnSelBrig(byte NI,word ID);
void CmdMakeStandGround(byte NI);
void CmdCancelStandGround(byte NI);
void CmdCrBig(byte NI,int i);
void CmdSelBrig(byte NI,byte Type,word ID);

extern Nation NATIONS[8];

void InitEBuf();
void ExecuteBuffer();
extern char Prompt[80];
extern int PromptTime;
void CreateWaterMap();
extern CEXPORT int SCRSizeX;
extern CEXPORT int SCRSizeY;
extern CEXPORT int RSCRSizeX;
extern CEXPORT int RSCRSizeY;
extern CEXPORT int COPYSizeX;
void CmdGetOil(byte NI,word UI);

extern byte NLocks[64][64];
void SetLock(int x,int y,char val);
inline void IncLock(byte x,byte y){
	NLocks[y>>2][x>>2]++;
	SetLock(x,y,1);
};
inline void DecLock(byte x,byte y){
	NLocks[y>>2][x>>2]--;
	SetLock(x,y,1);
};
extern bool FASTMODE;
extern word MAXOBJECT;
void SetupHint();
CEXPORT void AssignHint(char* s,int time);
void GetChar(GeneralObject* GO,char* s);
void ProcessHint();
extern OneObject OBJECTS[ULIMIT];
extern short TSin[257];
extern short TCos[257];
extern short TAtg[257];
void SetFlyMarkers();
void ClearFlyMarkers();
typedef void UniqMethood(int n,int x,int y);
void HandleSW();
void CreateStrangeObject(int i,byte NI,int x,int y,word ID);
void ShowRLCItemMutno(int x,int y,lpRLCTable lprt,int n);
void ShowRLCItemFired(int x,int y,lpRLCTable lprt,int n);
/*byte CreateUniExObj(Weapon* Wep,int x,int y,
				 short v,byte Mask,
				 OneObject* Send,
				 byte dsx,byte dsy,
				 word DOBJ);*/
/*byte CreateLeadingObject(Weapon* Wep,int x,int y,
				 short v,byte Mask,
				 OneObject* Send,
				 word DestAnm);*/
bool CheckAttAbility(OneObject* OB,word Patient);
void PrepareToEdit();
void PrepareToGame();
extern int MaxSizeX;
extern int MaxSizeY;
extern bool MiniMode;
void SetMiniMode();
void ClearMiniMode();
extern int Shifter;
extern int Multip;
void InitAllGame();
//x,y-coordinates of point on the 2D plane (unit:pix)
//returnfs index of building,otherwise 0xFFFF
word DetermineBuilding(int x,int y,byte NMask);
bool Create3DAnmObject(Weapon* Weap,int xs,int ys,int zs1,
					                int xd,int yd,int zd,
									OneObject* OB,byte AttType,word DestObj);
bool Create3DAnmObject(Weapon* Weap,int xs,int ys,int zs1,
					                int xd,int yd,int zd,
									OneObject* OB,byte AttType,word DestObj,char dz);
int div24(int y);
int Prop43(int y);

int GETV(char* Name);
char* GETS(char* Name);
void LoadAllNations(byte msk,byte NIndex);
int GetExMedia(char* Name);
extern NewAnimation** FiresAnm[2];
extern NewAnimation** PreFires[2];
extern NewAnimation** PostFires[2];
extern int            NFiresAnm[2];
typedef NewAnimation* lpNewAnimation;
extern int UnitsPerFarm;
extern int ResPerUnit;
extern int EatenRes;
#include "UnSyncro.h"
void AFile(char* str);
void AText(char* str);

//extern word fmap[FMSX][FMSX];
extern word* fmap;
//extern byte MCount[MAXCY*MAXCX];
extern byte* MCount;
//extern word NMsList[MAXCX*MAXCY*MAXINCELL];
//extern word* NMsList;

//extern word BLDList[MAXCX*MAXCY];
extern word** BLDList;
//extern byte NPresence[MAXCX*MAXCY];
extern byte* NPresence;
//------------sorting by nations-------------
extern word* NatList[8];
extern int   NtNUnits[8];
extern int   NtMaxUnits[8];
void SetupNatList();
void InitNatList();
void AddObject(OneObject* OB);
void DelObject(OneObject* OB);
void PlayAnimation(NewAnimation* NA,int Frame,int x,int y);
void MakeOrderSound(OneObject* OB,byte SMask);

extern int GoldID;
extern int FoodID;
extern int StoneID;
extern int TreeID;
extern int CoalID;
extern int IronID;
extern int MoneyID;

void UpdateAttackR(AdvCharacter* ADC);
//------------------IDS-----------------//
#define MelnicaID	0x01
#define MelnicaIDS  "MELNICA"

#define FarmID		0x02
#define FarmIDS		"FARM"

#define CenterID	0x03
#define CenterIDS	"CENTER"

#define SkladID		0x04
#define SkladIDS	"SKLAD"

#define TowerID		0x05
#define TowerIDS	"TOWER"

#define FieldID		0x06
#define FieldIDS	"FIELD"

#define MineID		0x07
#define MineIDS		"MINE"

#define FastHorseID	0x08
#define FastHorseIDS "FASTHORSE"

#define MortiraID	0x09
#define MortiraIDS  "MORTIRA"

#define PushkaID	0x0A
#define PushkaIDS   "PUSHKA"

#define GrenaderID  0x0B
#define GrenaderIDS "GRENADER"

#define HardWallID  0x0C
#define HardWallIDS "HARDWALL"

#define WeakWallID  0x0D
#define WeakWallIDS "WEAKWALL"

#define LinkorID	0x0E
#define LinkorIDS	"LINKOR"

#define WeakID		0x0F
#define WeakIDS	    "WEAK"

#define FisherID	0x10
#define FisherIDS	"FISHER"

#define ArtDepoID	0x11
#define ArtDepoIDS  "ARTDEPO"

#define SupMortID	0x12
#define SupMortIDS	"SUPERMORTIRA"

#define PortID		0x13
#define PortIDS	    "PORT"

#define LightInfID	0x14
#define LightInfIDS	"LIGHTINFANTRY"

#define StrelokID	0x15
#define StrelokIDS	"STRELOK"

#define HardHorceID	0x16
#define HardHorceIDS "HARDHORCE"

#define PeasantID	0x17
#define PeasantIDS	"PEASANT"

#define HorseStrelokID	0x18
#define HorseStrelokIDS "HORSE-STRELOK"

#define FregatID	0x19
#define FregatIDS   "FREGAT"

#define GaleraID	0x1B
#define GaleraIDS   "GALERA"

#define IaxtaID	    0x1C
#define IaxtaIDS    "IAXTA"

#define ShebekaID	 0x1E
#define ShebekaIDS   "SHEBEKA"

#define ParomID      0x1F
#define ParomIDS     "PAROM"

#define ArcherID    0x20
#define ArcherIDS   "ARCHER"

#define MultiCannonID 0x21
#define MultiCannonIDS "MCANNON"

#define DiplomatID 0x22
#define DiplomatIDS "DIPLOMAT"

#define MentID 0x23
#define MentIDS "MENT"

#define EgerID 0x24
#define EgerIDS "EGER"

#define CostlyID 0x25
#define CostlyIDS "COSTLY"

//-----------------------------------------//
extern OrderClassDescription OrderDesc[16];
extern int NOClasses;
extern OrderDescription ElementaryOrders[256];
extern int NEOrders;
//-----------------------------------------//
void Susp(char* str);
#define SUSPCHECK
#ifdef SUSPCHECK
#define SUSP(x) Susp(x)
#else
#define SUSP(x) ;
#endif 
int OScale(int x);
extern short LastDirection;
#define MobilR 1024
void MemReport(char* str);

extern DWORD LOADNATMASK;
extern char NatCharLo[32][8];
extern char NatCharHi[32][8];
CEXPORT void RunPF(int i,const char* Desc);
CEXPORT void StopPF(int i);
void ShowPF();
extern bool GoAndAttackMode;
extern int FrmDec;
extern int SpeedSh;
extern int REALTIME;
typedef DWORD DPID1, FAR *LPDPID;
class PlayerInfo:public BaseClass{
public:
	DPID1 PlayerID;
	//
	char name[32];	
	byte NationID;
	byte ColorID;
	byte GroupID;
	char MapName[36+4];//36+8
	//
	DWORD MapHashValue;//new
	int ProfileID;
	DWORD Game_GUID;

	//
	byte Hero;
	//

	byte UserParam[7];
	byte Rank;
	word COMPINFO[8];//=(Nation:5<<11)+(Difficulty:3<<8)+(Team:3<<4)+(Color:4)
	int  CHKSUMM;
	byte MapStyle;	// 0 - no AI, >0 - diff level of AI
	byte HillType;	// new game or saved game
	byte StartRes;
	byte ResOnMap;
	byte Ready;	

	byte Host;
	byte Page;
	byte CD;
	word Version;
	byte VictCond;
	word GameTime;
};
//extern PlayerInfo GSets.CGame.PL_INFO[8];
CEXPORT void AssignHint1(char* s,int time);
CEXPORT void AssignHint1(char* s,int time,byte opt);
//-----------------New text files------------------//

extern int LX_fmap;
word GetV_fmap(int x,int y);
extern int VAL_SHFCX;
extern int VAL_MAXCX;
extern int VAL_MAXCIOFS;
extern short randoma[8192];
extern word TexFlags[256];
int AddTHMap(int);

word GetNMSL(int i);
void SetNMSL(int i,word W);
void CleanNMSL();
extern int LastActionX;
extern int LastActionY;

#define GM(x) (1<<x)
#define INITBEST 0x0FFFFFFF

//#define INETTESTVERSION
//#define DeleteLastOrder() DeleteLastOrder();addrand(77)
#define CreatePath(x1,y1) CreatePath(x1,y1);addrand(99);
#define NewMonsterSendTo(x,y,Prio,OrdType) NewMonsterSendTo(x,y,Prio,OrdType);addrand(33)
#define NewMonsterPreciseSendTo(x,y,Prio,OrdType) NewMonsterPreciseSendTo(x,y,Prio,OrdType);addrand(44)
#define NewMonsterSmartSendTo(x,y,dx,dy,Prio,OrdType) NewMonsterSmartSendTo(x,y,dx,dy,Prio,OrdType);addrand(55)

#ifndef _USE3D
#define memset(a,b,c) try{memset(a,b,c);}catch(...){}
#endif 

extern bool LMode;
extern byte* XYShift;//map of horisontal shifting
#define SETXSHIFT(v,x) {if(x<-8)x=-8;if(x>7)x=7;if(v>=0&&v<MaxPointIndex)XYShift[v]=(XYShift[v]&0xF0)|((x>>1)+8);};
#define SETYSHIFT(v,x) {if(x<-8)x=-8;if(x>7)x=7;if(v>=0&&v<MaxPointIndex)XYShift[v]=(XYShift[v]&0x0F)|(((x>>1)+8)<<4)};
#define GETXSHIFT(v,x) (int(XYShift[v]&0x0F)-8)
#define GETYSHIFT(v,x) (int(XYShift[v]>>4)-8)
_inline void FreeXYShift(){
	if(XYShift)free(XYShift);
	XYShift=NULL;
}
_inline void SetupXYShift(){
	XYShift=znew(byte,(MaxTH+1)*MaxTH);
	int szz=(MaxTH+1)*MaxTH*2;
	memset(XYShift,0x88,(MaxTH+1)*MaxTH);
}
_inline void ClearXYShift(){
	memset(XYShift,0x88,(MaxTH+1)*MaxTH);
}
void _dbgLogVar(const char* var,const char* val);
void _dbgPrintState(const char* var,char* mask,...);
#define GraphStackSize 16
#define MaxGraphColumns 8
#define MaxSubBranches 8
class PerfGraph{
    int History[GraphStackSize][MaxGraphColumns*MaxSubBranches];
	LARGE_INTEGER HStart[MaxGraphColumns*MaxSubBranches];
public:
	PerfGraph();
    void Start(int Kind);
    void Start(int Kind,int SubBranch);
	void End(int Kind);
    void End(int Kind,int SubBranch);
	void New();
	void Draw(int x,int y,int Lx);
};
extern PerfGraph PGR1;
CEXPORT int GetXOnMiniMap(int x,int y);
CEXPORT int GetYOnMiniMap(int x,int y);
void DrawFillRectOnMiniMap(int x,int y,int x1,int y1,DWORD Color);
void DrawFillRectOnMiniMap(int x,int y,int x1,int y1,DWORD Color1,DWORD Color2,DWORD Color3,DWORD Color4);
void DrawRectOnMiniMap(int x,int y,int x1,int y1,DWORD Color);
void DrawLineOnMiniMap(int x,int y,int x1,int y1,DWORD Color);
void DrawCircleOnMiniMap(int x,int y,int R,DWORD Color);
void ClearLoadMark();
void AddLoadMark(char* Mark, int Value);
void ShowLoadProgress(char* Mark, int v, int vMax);
void LogBattle(char* mask,...);
void LogBattle(int NI,char* mask,...);
char* GetSt_UName(int NIndex);
typedef ClassArray<_str> FileList ;
void CreateSortedFileList(const char* Directory,const char* Mask,FileList& result,bool subdir=false);
_inline char* newstr(char* s){
	if(s){
		char* ss=new char[strlen(s)+1];
		strcpy(ss,s);
		return ss;
	}
	return NULL;
}
_inline getTimeScaledV(int v){
	return ((v*GameSpeed)>>8)+int((GameSpeed&255)>(rando()&255));
}
_inline getTimeScaled1(){
	return (GameSpeed>>8)+int((GameSpeed&255)>(rando()&255));
}
#endif __MapDiscr__