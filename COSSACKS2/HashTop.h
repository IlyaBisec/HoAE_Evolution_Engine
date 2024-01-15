#ifndef	__HASH__TOP__
#define __HASH__TOP__

// Ofs=Dst*NAreas+Src Key0=Ofs&1023 Key1=Ofs>>10
struct HashTop{	
	unsigned K1:16;		// Key1 to search
	unsigned ML:13;		// MotionLinks
	unsigned LD:13;		// LinkDist
	unsigned Prio:3;	// priory	
	unsigned NI:3;	    // nation	

	HashTop(){Clear();}
	void Clear(){K1=0xFFFF;}
};

#define NKey0 1024
#define NKey1 40
#define NKey NKey0*NKey1

class HashTopTable{
public:
	byte TopType;
	//HashTop* Key0[NKey0];
	HashTop Key1[NKey];
	//int NKey1;

	void SetUp(byte LockType) { TopType=LockType; TopRef=znew(word,TopLx*TopLx); }
	void Free() { free(TopRef); TopRef=NULL; }

	void Clear(){for(int i=0;i<NKey;i++) Key1[i].Clear();}
	HashTop* GetHashTop(int Ofs,byte NI);
	bool CalculateWay(int Ofs, HashTop* HT,DWORD Mask);

	word* TopRef;
	Area* TopMap;
	int   NAreas;
	int MaxArea;

	__forceinline Area* GetTopArea(int Index){
		assert(Index>=0&&Index<NAreas);
		return TopMap+Index;
	}
	__forceinline int GetTopAreaIndex(int x,int y){//x,y-topological coords
		if(x>=0&&x<TopLx&&y>=0&&y<TopLy){
			return TopRef[x+(y<<TopSH)];
		}else return 0xFFFF;
	}
	__forceinline int GetNextAreaOnTheWayTo(int Start,int Final,byte NI){
		assert(Start>=0&&Start<NAreas);
		assert(Start>=0&&Start<NAreas);
		if(Final==Start)return Start;
		
		addrand(Start);
		addrand(Final);

		HashTop* HT=GetHashTop(Start+Final*NAreas,NI);
		if(HT){
			int ML=HT->ML;
			if(ML==8191)ML=0xFFFF;
			return ML;
		}
		return 0xFFFF;
	}	
	void CreateAreas();
	void ReCreateAreas(int x0,int y0,int x1,int y1);
	bool AddArea(short x,short y,byte Sliv);
	void AddLink(int N1,int N2,DWORD Mask=0xFFFFFFFF);
	void EraseAreas();
};

extern HashTopTable HashTable[NMFIELDS];

void PreCreateTopLinks(int);
extern int MAXSPR;

#endif//__HASH__TOP__