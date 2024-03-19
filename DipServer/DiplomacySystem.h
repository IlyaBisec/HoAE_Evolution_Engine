struct HealGroup{
	GAMEOBJ Group;

	int Top;
	int LastTop;
	int LastMoveTime;

	byte Owner;
	word BrigID;

};
/////////////////////////////////////////////////////////////////////////////////
struct StormGroup{
	GAMEOBJ Group;
	
	GAMEOBJ StrGrp;	// Storm group
	word StormID;
	word StormSN;

	byte Owner;

	bool SingleUnitMotion;

	int Top;
	int LastTop;

	int LastMoveTime;

	int Difficulty;

	bool CannonGuard;	// охрана пушки
};
struct CannonGroup{
	GAMEOBJ Group;
	int ArtDepoX;
	int ArtDepoY;
	byte Owner;
	
	int Top;
	int LastTop;

	int DefForce; // сила защитников
};
struct TomahawkGroup{
	byte Owner;
	word Base;
	int DRadius;
	int CMass;
	GAMEOBJ Group;
	int x,y;
	int LastMoveTime;
	int LastThinkTime;

	int AttackTime;
	word ShieldID;
	int MaxAttR;

	bool Strelok;

	int LastTop;
	
	int LastNEnemy; // in to search enemy amount
};
struct FiringGroup{
	GAMEOBJ Group;
	byte Owner;
	
	word CurrentFiringBuilding;
	word FBSN;	// firing building serial number

	int LastMoveTime;
	int LastThinkTime;

	int LastTop;
};
struct KillersGroup{
	GAMEOBJ Group;
	byte Owner;
	bool InBattleNow;
	int LastMoveTime;
	int LastThinkTime;

	int LastTop;
	bool SeakMine;
};
struct ZooGroup{
	GAMEOBJ Group;
	int xc,yc;
	word Type;
	int MaxAmount;
	bool Empty;
	bool Attract;
	int LastMotionTime;
	int MotionFrequency;
};
struct ZooBirthZone{
	GAMEOBJ Zone;
	word Type;
	word NIndex[8];
};
class DiplomacySystem{
public:	
	BasicDiploRelation** DIPS;
	int  NDIPS;	
	int  BusyPageIndex;//==-1 if no busy page shown, othervice index of busy tribe

	// Coord of AI	
	int AIX[7];
	int AIY[7];
	bool AI_set[7];	
	bool FriendAI;

	bool SetAI(byte nat, int x, int y);
	
	//---------walking units list----------
	int NWalk;
	word* WID;
	word* WSN;
	//--------- Storm --------------- 
	StormGroup* STORMS;
	int NStorms;	
	//--------- Cannons ---------------
	CannonGroup* CANNONS;
	int NCannons;	
	//-------------firers list-------------
	FiringGroup* FIRERS;
	int NFirers;
	//------------PeasantKillers-----------
	KillersGroup* KILLERS;
	int NKillers;
	//------------ Tamagavc ---------------
	TomahawkGroup* TOMAHAWKS;
	int NTomahawks;	
	//------------ Healers ---------------
	DynArray<HealGroup> HEALERS;
    bool hNatEnable[8];
	short hDang[8][4096];	// nation
	//int hFear[256];

	//---------ZOOLOGICAL GROUPS-----------
	short* DangerMap;
	int DMSize;
	short StartPopulation[2048];
	ZooGroup* ZOO;
	int NZOO;
	ZooBirthZone* BZOO;
	int NBZOO;
	//-------------------------------------
	DiplomacySystem();
	//-------------------------------------
	void AddNewZooGroup(ZooGroup* ZGrp);
	void AddZooBirthZone(ZooBirthZone* BZone);
	void ProcessZoology();
	//-------------------------------------
	void ResearchMap();
	void SetCallBacks();
	void PerformCommand(char* Data,int size);
	void Process();
	bool CreateContactsList(bool first);
	
	word AddDipCenter(char* FileID, char* Dip, char* GDance, char* ZDance);

	void AddWalker(word ID);
	void AddFirers(GAMEOBJ* Src,byte Owner);
	void AddCannon(GAMEOBJ* Src,byte Owner);
	StormGroup* AddStorm(GAMEOBJ* Src, byte Owner, int Diff=2);
	void AddKillers(GAMEOBJ* Src,byte Owner, bool SeakMine=true);
	void AddTomahawks(GAMEOBJ* Src,byte Owner,word Base=0,int DriftRadius=0, int CriticalMass=0);
	//
	void AddHealers(GAMEOBJ* Src,byte Owner);
	//
	void Cannon2Alex(GAMEOBJ* Src, byte Owner);
};
