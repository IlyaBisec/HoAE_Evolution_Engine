//=================================================================================================================//
#pragma once
#define BRIGADEORDER_RIFLEATTACK 1
#define BRIGADEORDER_GOONROAD 2
#define BRIGADEORDER_BITVA 3
#define BRIGADEORDER_KEEPPOSITION 4
#define BRIGADEORDER_HUMANGLOBALSENDTO 5
#define BRIGADEORDER_MAKEDIVERSION 6
//=================================================================================================================//
class CBrigadeOrder : public BaseClass
{
public:
	CBrigadeOrder();
	virtual ~CBrigadeOrder();
	CBrigadeOrder(Brigade* Br);

	ClassPtr<CBrigadeOrder> Next;
	SAVE(CBrigadeOrder)
		REG_AUTO(Next);
	ENDSAVE;

	virtual bool Process(Brigade* Br);
	virtual bool Init(Brigade* Br);

	virtual byte GetBrigadeOrderID();
	virtual byte GetBrigadeOrderPrio();
	virtual bool IsFast();
	virtual char* GetBrigadeOrderMessage();

	virtual int SaveBinaryData(void* Data);
	virtual bool LoadBinaryData(void* Data);
};
//=================================================================================================================//

class BrigadeOrder_RifleAttack : public CBrigadeOrder
{
public:
	BrigadeOrder_RifleAttack();
	virtual ~BrigadeOrder_RifleAttack();
	//BrigadeOrder_RifleAttack(Brigade* BR);


	virtual bool Process(Brigade *BR);
	virtual byte GetBrigadeOrderID();
	virtual bool Init(Brigade *BR);

	bool ProcessPre(Brigade *BR);
	word BrigID;
	Brigade* Brig;
	int x;
	int y;
	bool FirstProces;
	word EnemyList[512];
	word EnemyLife[512];
	word EnemyFireCount[512];
	int EnemyX;
	int EnemyY;
	static int EX;
	static int EY;
	word NEnemy;
	word NEnemyLife;
	word FireOrder[256];
	void FillEnemyList(Brigade* BR);
	int FillTime;
	bool SetTargetObj(OneObject *OB);
	static bool AddEnemy(OneObject* OB,void* param);
	static int compare(const void* p1,const void* p2);
	word OnlyOneBrig;
	void ClearTargetObj(Brigade *BR);
	void ClearLastOrders(Brigade *BR);
	
	SAVE(BrigadeOrder_RifleAttack)
		REG_PARENT(CBrigadeOrder);
		REG_MEMBER(_WORD,BrigID);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		//REG_MEMBER(_bool,FirstProces);
		REG_MEMBER(_WORD,OnlyOneBrig);
	ENDSAVE;
};
//=================================================================================================================//
#define MaxPointsXY 1024

class BrigadeOrder_GoOnRoad : public CBrigadeOrder
{
public:
	

	BrigadeOrder_GoOnRoad();
	~BrigadeOrder_GoOnRoad();
	
	virtual bool Process(Brigade *BR);
	virtual byte GetBrigadeOrderID();
	virtual bool IsFast();

	byte Prio;
	int DestTopZone;// зона куда идем
	bool ProcessPre(Brigade* BR);
	int PointsXY[MaxPointsXY]; // дорожные точки
	byte PointsDir[MaxPointsXY]; // направление в каждой точке
	int ShiftInColumn[16]; //сдвиг в колоне от дорожной точки
	int HeadIndex; // индекс головы колонны в PointsXY
	int HeadTopZoneIndex; // индекс топологической зоны головы колонны 
	int MaxPIndex; // сколько точек получили
	int LastMoveTime; // время последнего шага
	void SetNextPosAndDest(Brigade* BR);// шаг вперед
	int UnitsInLine; // ширена колонны
	void GetUnitCoordInColumn(int PointI,byte ColumnI, int &x, int &y);
	virtual bool Init(Brigade *BR);
	int lockType;
	int NLines;
	int HeadOBJIndex; // ведуший юнит
	word SA[256]; // служебный
	static int  compare(const void *arg1, const void *arg2); 
	static int fx, fy;
	static Brigade* fBrig;
	int GetMinDistBrigToPoint(Brigade* BR, int x, int y);
	bool FirstStep; // прямо как в жизни :)
	bool OffPlaseHead; // офицеры в голове колонны (или нет)
	void SetOffPoss(int LineI,Brigade* BR); // задать позиции офицерскому составу
	DWORD Owner; // идентификатор отряда для дороги
	word RoadFlag; // место на дороге (норма, стоп, обочина)
	void LockNextPoint(); // забронировать следующий отрезок дороги
	int LastStepTime;
	int DistInColumn;
	int PassNPoints;
	void SetUnitsSpeed(Brigade* BR);
	void SortBrigUnits(Brigade* BR, int xx, int yy);
	void RangeInLineMemb(Brigade* BR);
	bool GoAway();
	static bool AskGoAway(OneObject* OB,void* p);
	bool IsEnemyOnWay;
	
	SAVE(BrigadeOrder_GoOnRoad)
		REG_PARENT(CBrigadeOrder);
		/*
		REG_MEMBER(_BYTE,Prio);
		REG_MEMBER(_int,DestTopZone);
		REG_MEMBER(_int,HeadIndex);
		REG_MEMBER(_int,HeadTopZoneIndex);
		REG_MEMBER(_int,UnitsInLine);
		REG_MEMBER(_int,lockType);
		REG_MEMBER(_int,NLines);
		REG_MEMBER(_int,HeadOBJIndex);
		*/
	ENDSAVE;
};
//=================================================================================================================//

class BrigadeOrder_Bitva : public CBrigadeOrder
{
public:
	BrigadeOrder_Bitva();
	

	virtual bool Process(Brigade *BR);
	virtual byte GetBrigadeOrderID();

	word StartTop;
	byte BitMask[1024*8];
	word Enm[512];
	word EnSN[512];
	byte NDang[512];
	int  NEn;
	int MinX;
	int MinY;
	int MaxX;
	int MaxY;
	void AddEnemXY(int x,int y,int MyTop,byte mask);

	SAVE(BrigadeOrder_Bitva)
		REG_PARENT(CBrigadeOrder);
	ENDSAVE;
};
//=================================================================================================================//
class BrigadeOrder_HumanEscape : public CBrigadeOrder
{
public:
	
	int x,y;
	short Dir;
	byte Prio,OrdType;
	virtual bool Process(Brigade *BR);
	SAVE(BrigadeOrder_HumanEscape)
		REG_PARENT(CBrigadeOrder);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_WORD,Dir);
		REG_MEMBER(_BYTE,Prio);
		REG_MEMBER(_BYTE,OrdType);
	ENDSAVE;
};
//=================================================================================================================//
class BrigadeOrder_KeepPositions : public CBrigadeOrder
{
public:
    int OrderTime;
	SAVE(BrigadeOrder_KeepPositions)
		REG_PARENT(CBrigadeOrder);
		REG_MEMBER(_int,OrderTime);
	ENDSAVE;
	virtual bool Process(Brigade *BR);
	virtual byte GetBrigadeOrderID();
};
//=================================================================================================================//
class BrigadeOrder_HumanGlobalSendTo : public CBrigadeOrder
{
public:
	
	int xx,yy,NextX,NextY,Prio;
	short Dir;
	word NextTop;
	virtual bool Process(Brigade *BR);
	virtual byte GetBrigadeOrderID();
	SAVE(BrigadeOrder_HumanGlobalSendTo)
		REG_PARENT(CBrigadeOrder);
		REG_MEMBER(_int,xx);
		REG_MEMBER(_int,yy);
		REG_MEMBER(_int,NextX);
		REG_MEMBER(_int,NextY);
		REG_MEMBER(_int,Prio);
		REG_MEMBER(_WORD,Dir);
		REG_MEMBER(_WORD,NextTop);
	ENDSAVE;
};
//=================================================================================================================//
class BrigadeOrder_MakeDiversion : public CBrigadeOrder
{
public:

	word IDX[320];
	word USN[320];
	int NU;
	int MaxU;
	int Phase;
	int LoadX;
	int LoadY;
	int CostPointIndex;
	int StartTime;
	int LastPassangerTime;

	virtual bool Process(Brigade *BR);
	virtual byte GetBrigadeOrderID();
	
	SAVE(BrigadeOrder_MakeDiversion)
		REG_PARENT(CBrigadeOrder);
		REG_MEMBER(_int,NU);
		REG_MEMBER(_int,MaxU);
		REG_MEMBER(_int,Phase);
		REG_MEMBER(_int,LoadX);
		REG_MEMBER(_int,LoadY);
		REG_MEMBER(_int,CostPointIndex);
		REG_MEMBER(_int,StartTime);
		REG_MEMBER(_int,LastPassangerTime);
	ENDSAVE;
};
//=================================================================================================================//