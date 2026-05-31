#ifndef	__TOPO__GRAPH__ 
#define	__TOPO__GRAPH__

#pragma pack(1)
struct Radio{
	char* xi;
	char* yi;
	word N;
};
#define RRad 127
extern Radio Rarr[RRad];

void CreateAreas();
void InitAreas();
void EraseAreas();
void DrawAreas(int xs,int ys,int x0,int y0,int Lx,int Ly);

#define SI_Center	1
#define SI_Meln		2
#define SI_Sklad	4
#define SI_Shahta	8
#define SI_Tower	16
#define SI_ArtDepo	32
#define SI_House	64
#define SI_Barrack  128
#define SI_Farm		256
#define SI_Other	512
#define SI_Near     1024
#define SI_FarLand  8192
#define SI_Okrug    16384
#define SI_Central  32768
struct StrategyInfo{
public:
	word BuildInfo;
	byte NPeasants;
	byte NShortRange;
	byte NLongRange;
	byte NMortir;
	byte NTowers;
	byte NPushek;
};
struct OneLinkInfo{
	word  NextAreaID;
	word  NextAreaDist;	
	DWORD LinkNatMask;
};
struct Area{
	short x;
	short y;
	byte  Importance;
	byte  NTrees;
	byte  NStones;
	word  NMines;
	word* MinesIdx;
	word  NLinks;
	OneLinkInfo* Link;
	word  MaxLink;

	StrategyInfo SINF[8];
};
/*
struct MediaTop{
	word* MotionLinks;
	word* LinksDist;
	word* TopRef;
	int NAreas;
	Area* TopMap;
};
*/

//extern MediaTop GTOP[2];

extern TopLx;
extern TopLy;
extern MaxTop;
extern int TopSH;

extern word* TopRef;
extern word* LinksDist;
extern word* MotionLinks;

extern Area* TopMap;
extern int NAreas;
extern int MaxArea;

word GetTopRef(int Ofs, byte TopType=0);		// Ofs = x+(y<<TopSH)
word GetLinksDist(int Ofs, byte TopType=0,byte NI=0);		// Ofs = StartTop + FinTop * NAreas
word GetMotionLinks(int Ofs, byte TopType=0,byte NI=0);	// Ofs = StartTop + FinTop * NAreas
Area* GetTopMap(int Ofs, byte TopType=0);		// Ofs = Top
int GetNAreas(byte TopType=0);					// TopType - type of links in top map

//*__forceinline*/ word SafeTopRef(int x,int y);
//*__forceinline*/ void SafeSetTopRef(int x,int y,word Val);

__forceinline word SafeTopRef(int x,int y){
	if(x>=0&&y>=0&&x<TopLx&&y<TopLx)return GetTopRef(x+(y<<TopSH),0);
	else return 0xFFFF;
};
__forceinline void SafeSetTopRef(int x,int y,word Val){
	if(x>=0&&y>=0&&x<TopLx&&y<TopLx) TopRef[x+(y<<TopSH)]=Val;
};

void ClearTopology();
void SetupTopology();
void FreeTopology();

void CreateStrategyInfo();
//---------
#define WTopLx (TopLx)
#define WTopSH (TopSH)
#define WCLSH 2
#define WCLLX 4
//---------

#endif//__TOPO__GRAPH__