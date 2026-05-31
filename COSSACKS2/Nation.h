#pragma once

class DIALOGS_API AnmObject{
public:
	NewAnimation* NewAnm;
	int x,y,z;//?????
	int vx,vy,vz;//????
	int az;//?????
	int xd,yd,zd;//????????
	short GraphDZ;
	short Frame;
	int   LongFrame;
	int   PrevLongFrame;
	short NTimes;
	Weapon* Weap;
	word    Damage;
	OneObject* Sender;
	word ASerial;
	word DestObj;
	word DestSN;
	byte AttType;
	char WeaponKind;
};
class City;
class DIALOGS_API Needness{
public:
	byte NeedType;//==0-monster,==1-Upgrade
	word MonID;
	byte GroupSize;
	byte Amount;
	word Probability;
	word MoneyPercent;
};
struct DIALOGS_API SWPAR{
	word Range;
	//byte MinMagic;
	bool Enabled:1;
	bool Fly:1;
};
struct DIALOGS_API sAI_Req{
	byte Kind;//0-unit,1-upgrade,2-group
	word ObjID;
	word Amount;//if upgrade:1-Done 2-Enabled
};
struct DIALOGS_API sAI_Devlp{
	byte Kind;//0-unit,1-upgrade
	byte Source;//0-general,1-army,2-selo,3-science
	byte ConKind;//0-unit,2-group
	word ObjID;
	word ConID;
	word Amount;
	word GoldPercent;
	word AtnPercent;
};
struct DIALOGS_API sAI_Cmd{
	byte Kind;//1-army,2-selo,3-science
	word Info[8];
};
class DIALOGS_API Branch{
public:
	int  RESAM[8];
	word RESP[8];
	int  RESRM[8];
	void AddTo(byte ResID,int Amount);
	void AddPerc(byte ResID,int Amount,int perc);
	void AddEntire(byte ResID,int Amount);
	void Check(byte NI);
	void Init();
	int GetMonsterCostPercent(byte NI,word NIndex);
	int GetUpgradeCostPercent(byte NI,word NIndex);
};
//???? ???????
struct DIALOGS_API U_Grp{
	word N;
	word* UIDS;
	word* UVAL;
};
struct DIALOGS_API vsInfoArmy{
	int Active;
	int Killed;
	int Lost;
};
/*
struct vsInfoRes{
int Peasa
};
*/
typedef void VoidProc();
class DIALOGS_API Nation{
public:
	char SCRIPT[16];

	bool HeroAtHome;

	int NMon;
	bool GoldBunt:1;
	GeneralObject* Mon[2048];
	byte SoundMask[2048];
	byte VictState;//0-? 1-defeat 2-victory
	//--- academy ---
	bool vsInfoInit;
	word NErased[2048];		// killed enemy
	word NKilled[2048];		// lost
	word NProduced[2048];
	//
	word NBrErased[2048];	// brigade killed enemy
	word NBrLost[2048];		// brigade lost
	word NBrProduced[2048];
	//
	vsInfoArmy NBrInfantry;	// infantry brigades PortBranch=0
	vsInfoArmy NBrCavalery;	// cavalery brigades PortBranch=1
	vsInfoArmy NCannons;	// cannons PortBranch=2
	//----Resource control-------
	int ResTotal[8];
	int ResOnUpgrade[8];
	int ResOnMines[8];
	int ResOnUnits[8];
	int ResOnBuildings[8];
	int ResOnLife[8];
	int ResBuy[8];
	int ResSell[8];	
	//-------COMMAND CENTERS----------	
	DynArray<word> CentIDS;
	DynArray<word> CentSNS;
	int LastCheckTime;
	OneObject* GetNearestCenter(int UType,int x,int y);//real coords
	//--------------------------------
	City* CITY;
	int NGidot;
	int NFarms;
	int AddFarms;
	word NArtdep[MaxArtDep];
	word NArtUnits[MaxArtDep];
	word* PAble[2048];
	word PACount[2048];
	char* AIndex[2048];
	char* AHotKey[2048];
	int BranchPercent[NBRANCH];
	//Upgrade UPG;
	int NUpgrades;
	NewUpgrade* UPGRADE[4096];
	int NOct;
	int NSlides;
	//AI Statements
	int CasheSize;
	int TAX_PERCENT;
	int CASH_PUSH_PROBABILITY;
	int NationalAI;//0..32768-determines speed of development
	int AI_Level_MIN;
	int AI_Level_MAX;
	int AI_Forward;
	short DangerSound;
	short VictorySound;
	short ConstructSound;
	short BuildDieSound;
	short UnitDieSound;
	word  LastAttackTime;
	//byte MagicDelay;
	word Harch;
	word NLmenus;
	word* Lmenus;
	word NAmenus;
	word* Amenus;
	word NWmenus;
	word* Wmenus;
	word NCmenus;
	word* Cmenus;
	word NNeed;
	Needness NEED[1024];
	int ResRem[8];
	int ResSpeed[8];
	//ENDAI
	byte NNUM;
	int  NFinf;
	byte palette[256];
	byte NMask;
	word NIcons;
	WIcon* wIcons[1024];
	word NCOND;
	word CLSize[4096];//Access controlling
	word* CLRef[4096];
	//Strange weapon prameters
	word SWRange[256];
	SWPAR SWP[256];
	//-------------NEW AI--------------
	word NGrp;        //Groups of types definition
	word GRSize[32];
	word* GRRef[32];
	word  GAmount[32];//Result of calculation
	word N_AI_Levels;
	word N_AI_Req[256];
	word N_AI_Devlp[256];
	word N_AI_Cmd[256];
	sAI_Req* AI_Req[256];
	sAI_Devlp* AI_Devlp[256];
	sAI_Cmd*   AI_Cmd[256];
	word AI_Level;
	word NPBal;
	word* PBalance;
	word NMineBL;
	word* PBL;

	int POnFood;
	int POnWood;
	int POnStone;

	char* DLLName;
	VoidProc* ProcessAIinDLL;
	HINSTANCE hLibAI;
	//byte GoldMatrix[40];
	//byte IronMatrix[40];
	//byte CoalMatrix[40];
	//------------------SHAR----------------//StartSave
	byte SharStage;
	int SearchRadius;
	int SharX;
	int SharY;
	int SharZ;
	int SharVx;
	int SharVy;
	int SharVz;
	int SharAx;
	int SharAy;
	int SharAz;
	bool Vision:1;
	bool SharAllowed:1;
	bool SharPlaceFound:1;
	bool AI_Enabled:1;
	//---------Upgradable properties--------//
	word FoodEff;
	word WoodEff;
	word StoneEff;
	bool Geology;
	//---------------Constants--------------//
	word UID_PEASANT;//EndSave
	//word UID_TOWER;
	word UID_WALL;
	//word UID_WALL2;
	//word UID_MORTIRA;
	//word UID_PUSHKA;
	word UID_MINE;
	word UID_HOUSE;

	//U_Grp UGRP_TOWUP;
	U_Grp UGRP_MINEUP;
	//U_Grp UGRP_STRELKI;
	//U_Grp UGRP_LIGHTINF;
	//U_Grp UGRP_HARDINF;

	word  MINE_CAPTURE_RADIUS;
	word  MINE_UPGRADE1_RADIUS;
	word  MINE_UPGRADE2_RADIUS;
	word  DEFAULT_MAX_WORKERS;
	word  MIN_PBRIG;

	word  MU1G_PERCENT[3];
	word  MU1I_PERCENT[3];
	word  MU1C_PERCENT[3];

	word  MU2G_PERCENT[3];
	word  MU2I_PERCENT[3];
	word  MU2C_PERCENT[3];

	word  MU3G_PERCENT[3];
	word  MU3I_PERCENT[3];
	word  MU3C_PERCENT[3];
	//--------------------------------------
	char** History;
	int NHistory;
	//-----------------XRONIKA--------------
	byte ThereWasUnit;
	int NPopul;
	int MaxPopul;
	word* Popul;

	int NAccount;
	int MaxAccount;
	word* Account;

	int NUpgMade;
	int MaxUpgMade;
	word* UpgIDS;

	int*  UpgTime;
	void AddUpgrade(word ID,int time);
	void AddPopul(word N);
	//---------------NEW resource-----------
	Branch SELO;
	Branch ARMY;
	Branch SCIENCE;
	Branch GENERAL;
	//----------------------choose unit menu
	char***UnitNames;
	int*   NUnits;
	word** UnitsIDS;
	word FormUnitID;
	//---------------------------------
	void CreateNation(byte NMask,byte NIndex);
	int  CreateNewMonsterAt(int x,int y,int n,bool Anyway,short Dir=-1);
	void AssignWeapon(Weapon* Wpn,int i);
	int CreateBuilding(word ID,byte x,byte y);
	bool CheckBuilding(word ID,byte x,byte y);
	void GetUpgradeCostString(char* st,word UI);
	void CloseNation();
	void AddResource(byte Rid,int Amount);
	void ControlProduce(byte Branch,byte ResID,int Amount);
	int GetNationlKillingExpirience();
};