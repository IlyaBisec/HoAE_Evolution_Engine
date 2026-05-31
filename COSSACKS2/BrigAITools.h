#ifndef BrigAITools_def
#define BrigAITools_def

// CellSystem

typedef byte CellType;

class  CellSquad{
public:
	bool Init(int NCells);
	CellType* Cells;

	/*
	void Init();
	bool Process(int x, int y);
	*/
};

class CellSystemS{
public:
	CellSystemS(){ Clear(); }
	void Clear(){ memset(this,0,sizeof(*this)); }	
	void Init();
	void Free();
	
	CellSquad** Squads;

	byte BitMap;
	byte BitSquare;
	byte BitCell;

	//bool AddCell(int x, int y, CellType* cell);
	CellType* GetCell(int x, int y, bool alloc);

//	void 
	
	/*
	bool AddDust(int x, int y, byte r, byte g, byte b, byte a);
	int GetSprayDust(int x,int y);
	SprayCell* GetSprayCell(int x,int y);

	void Process();
	void Draw();
	*/

};

extern CellSystemS TreeShotBar;

// AI classification



#endif //BrigAITools_def