
struct Target{

	word ID;
	word SN;
	
	bool Storm;
	bool Capture;

};

class MWeapon{
public:
	MWeapon(word wk);

	word WeapKind;

	int Damage;
	int MinR;
	int MaxR;
	int Delay;		// время на перезарядку в кадрах анимации
	int Prepare;	// время на подготовку оружия (из положения вольно к готовсь)

};

class Branch{
public:
	Branch(word nindex);
	
	word NIndex;

	int NWeap;
	MWeapon* Weap;
	
	int NAllWeap;
	int* Dang;

	//int Life;
	int Speed;		// количество точек за 100 кадров анимации

};

///////////////////////////////////////////////////

class Squad{
public:
	//Squad();
	//Squad(word BrgID);

	int Index;

	byte NI;

	GAMEOBJ Group;
	word Brig;
	word NIndex;

	int Top;
	int LastTop;

	int xc,yc;			// coor of center group

	word TarTop[2];		// top zone of target (primary,secondary)
	word TarZone[2][2];	// top zone to solve nearest target (primary,secondary) (direct,round)
	word TarDist[2][2];	// distance to target (primary,secondary) (direct,round)

	int LastMoveTime;

	byte Target;			// number of target to solve
	byte MovingType;		// direct or round moving to target
	bool StandGround;		// wait or set stand ground for squad (if Brig!=0xFFFF)

	//int GetTop();
	bool FindTargetZone(short* DangerMap, word* ObjIds, word& TOP, word& TZ, word& Dst);
	void MoveToTop(short* DangerMap);

	///////////////////
	int ArmyID;
	int ArmyDist;

	int SquadOrder;

	int Power;			// текущая сила рукопашного боя
	
};
typedef Squad* SqdT;

class GreatSQD{
public:
	bool operator()(Squad* x, Squad* y){
		return  x->ArmyDist > y->ArmyDist;
	}
};

struct ArmyLink{
	word Front;
	word Rear;
	word Left;
	word Right;
};
/*
struct Polk{
	byte NI;

	int NSquad;
	word SquadID[40];
	
	int Power;

	int PolkOrder;
	void ProcessOrder(){
		switch(PolkOrder){
		case 1: RunGlobalAttack(); break;
		case 2: SetCircle(); break;
		case 3: SetBlockWay(); break;
		}
	};

	void SetGlobalAttack(){PolkOrder=1;}
	void SetCircle(){PolkOrder=2;}
	void SetBlockWay(){PolkOrder=3;}
	
	void RunGlobalAttack();
	void RunCircle();
	void RunBlockWay();
	
};
*/
struct ActiveArmy{
	int Index;

	void Clear(int ID=-1);

	word SID[256];		// squad id
	word NS;			// n of men
	word PID[256];		// safe position round army
	word NP;			// n of position

	word TentID;		// our tent id, this tent may will captured
	word TentWay[40];	// nearest way to tent
	int NTentWay;
	int TentWayDist;

	word TopCenter;		// top zone of center army
	int xc;
	int yc;
	int n;				// number of army ocupied top zone

	int Power;			// текущая сила рукопашного боя армии
	//int Vistrel;		// текущая сила стрелков армии
	//int Cannon;			// к-во пушек у армии

/*
	Polk Actor;			// полк ответственный за эту армию врага
*/
	ArmyLink Link;	
	
};
typedef ActiveArmy* AAT;

class LessAA{
public:
	bool operator()(AAT x, AAT y){ return  x->TentWayDist < y->TentWayDist; }
};

class GreatAA{
public:
	bool operator()(AAT x, AAT y){ return  x->TentWayDist > y->TentWayDist; }
};

struct ArmyTopInfo{
	int Life[4096];			// сумарная жизнь
	int Damage[4096];		// сумарная атака

	int Power[4096];		// life*damage
};

struct Enemy{	
	
	int ArmyMap[4096];

	ArmyTopInfo TopInf;

	int NAA;
	ActiveArmy AA[20];

};

///////////////////////////////////////////////////

class Mind{
public:
	Mind();
	~Mind();

	void Free();
	void Clear();
	void Init(byte ni, int type=0);
	void Process();

	byte NI;
	int Type;	// type of script

	void Process0();	// standart attack
	void Process1();	// defend base
	void Process2();	// round the enemy
	void Process3();	// global attack

	void ProcessCannons(int TimeToStorm);
	
	void ActivateArmy(ActiveArmy* AA, int NA);

	int NSqd;
	Squad* Sqd;

	word AddSquad(word BrgID, word NIndex);
	void RefreshSquadInfo();

	GAMEOBJ New;
	GAMEOBJ Panic;

	int LastGlobalMove;
	
	word NTrgList;
	word NSqdList[20];
	
	word TrgList[20];
	word SqdList[20][20];
	word DstList[20][2][20];

	int Fear[256];
	short DangerMap[4096];
	
	int MapVistrel[4096];
	int MapKartech[4096];

	int ShowMode;
	void ShowVistrel(){SetDangerMap(MapVistrel);}
	void ShowKartech(){SetDangerMap(MapKartech);}

	// game situation

	int SetGameGoals();

	word DefTent[4096];
	word CapTent[4096];
	
	Enemy ENM;
	int GetArmyActorPower(ActiveArmy* arm);		// силу отрядов сопровождающих армию врага
	
	void MarkArmy();

	int RunState;	// окружить, напасть, преградить путь
	void SetLink0();

	void GlobalAttack();

	int NCannon;
	word Cannon[256];
	word CannSN[256];

	word CanGuardSqd;

	int NSheep;
	word Sheep[1024];
	word SheSN[1024];

	GAMEOBJ Repaires;	// group repaires
};

extern Mind Player[8];

///////////////////////////////////////////////////

class CSquad{
public:
	
	GAMEOBJ Group;
	int BrigID;

	int PolkID;

};

class CPolk{
public:
	byte NI;	// nation

	int NSqd;
	Squad* Sqd;
	Squad* AddSqd();

};

class CMind{
public:

	GAMEOBJ Victory;
	GAMEOBJ Defeat;

/*
	int NPolk;
	CPolk *Polk;
*/
};