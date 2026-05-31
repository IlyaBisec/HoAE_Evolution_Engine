class UnitsGroup{
public:
	word* IDS;
	word* SNS;
	int N;
	int Max;

	byte NI;
	byte NMASK;
	int Index;
	void* GOrder;

	//-----------debug---------
	char* file;
	int   Line;
	//-------------------------

};

CIMPORT UnitsGroup* GetUnitsGroup(GAMEOBJ* Group);

CIMPORT bool AddEnemyCaptBuildTopList(word* IDS,byte NI);
CIMPORT bool AddEnemyCenterTopList(word* IDS,byte NI);

CIMPORT void CreateDangerMapBattle(byte NI,int* DangMap,int MaxSize,int* FearArray,int ItrAmount);

DLLIMPORT void CreateTopListEnArmyBtl(word* IDS,byte NI,int MinPS);
DLLIMPORT int GetZonesDist(int Start,int Fin);

CIMPORT word getDir(int dx,int dy);