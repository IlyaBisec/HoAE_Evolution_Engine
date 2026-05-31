#define MAXFL 4096
class GFSYSTEM{
public:
	GFILE* FILES[MAXFL];
	GFSYSTEM();
	~GFSYSTEM();
	GFILE* GetFile();
	void FreeFile(GFILE* F);
};
struct NetCell{
	byte* Data;
	int size;
	DWORD SendTime;
	DWORD idTo;
};
class NetCash{
public:
	int NCells;
	int MaxCells;
	NetCell* CELLS;
	NetCash();
	~NetCash();
	void Add(byte* Data,int size,DWORD idTo);
	void AddOne(byte* Data,int size,DWORD idTo);
	void AddWithDelay(byte* Data,int size,DWORD idTo,int TimeDelay);
	void Process();
};
#define PingTimeout 2000
struct OneLPing{
	DWORD UniqID;
	int StartTime;
};
class OneLostID{
public:
	DWORD DPID;
	int NSent;
	int NReceived;
	OneLPing* PING;
	int NPings;
	int LastAccessTime;
	int LastReceiveTime;
};
class LoosedPack{
public:
	LoosedPack();
	~LoosedPack();
	OneLostID OLID[16];
	int  GetLoosePercent(DWORD ID);
	int GetLastAnswerTime(DWORD ID);
	void Clear();
	void DeleteBadPlayers();
	void Add(DWORD DPID,int ID);
	void Remove(int ID);
	void Process();
};
class NetSample{
public:
	word* Danger;
	word* Pretty;
	int NZ;
	int LastUpdate;
	void CreateDiversantMap(byte NI);
	void CreateGrenadersMap(byte NI);
	NetSample();
	~NetSample();
};
class SafeNet{
public:
	NetSample Diversant;
	NetSample Grenader;
	/*
	int LastUpdate;
	byte NI;
	int NZ;
	word* CellDanger;
	word* CellPretty;
	*/
	word FindNextCell(int F,int Start,NetSample* Net);
	word FindWayTo(int F,int Start,int Fin,NetSample* Net);
	//void CreateMaps();
	SafeNet();
	~SafeNet();

};
struct SampleRoot{
	byte RootType;
	byte Direction;
	short x;
	short y;
	DWORD AttachMask;
	DWORD Reserved;
};
struct SampleSrc{
	char Name[32];
	int dx;
	int dy;
	int NRoots;
	SampleRoot* ROOT;
	short* xi;
	short* yi;
	int Npt;
};
class SamplesSet{
public:
	int NSmp;
	SampleSrc* SSET;
	char CurrentSet[32];
	bool Passive;

	void Draw();
	void NewSet(char* Name);
	void SetActiveSet(char* Name);
	void AddSquare(int x,int y);
	void DelSquare(int x,int y,int mode);
	void EditRootParam(int x,int y);
	void ClearAll();
	SamplesSet();
	~SamplesSet(){
		ClearAll();
	};
};
class  OneGraph{
public:
	int Color;
	int* T;
	int* V;
	int  N;
	int  NMax;	
	//------------
	int MinT;
	int MaxT;
	int MinV;
	int MaxV;
	OneGraph();
	~OneGraph();
	void AddTV(int T,int V);
	void Clear();

};
class Graph{
public:
	char* Header;
	int  Index;
	OneGraph GRP[16];
	Graph();
	void Add(int t,int v,byte c);
	void Clear();
	void Draw(int x0,int y0,int Lx,int Ly);
};
struct OrdersScope{
	DynArray<Order1,50000> Orders;
	DynArray<Order1*> FreeBlocks;
};