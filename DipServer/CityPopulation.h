#if !defined(DIP_SERVER_CITYPOPULATION)
#define DIP_SERVER_CITYPOPULATION

//#include "CommonDip.h"
//#include "..\IntExplorer\DString.h"
//#include "import.h"
//#include "..\common.h"
#include <xmlQuote.h>

class BildType;
struct SComeIn
{
	BildType* BT;
	const char *BildTypeName;
	int Probability;
};
class CityzenType
{
public:
	CityzenType();
	~CityzenType();
	DString Name;
	int NDSNameID;
	GAMEOBJ UnitType;
	int LifeTime;
	DynArray<int> Behaviour;
	DynArray<SComeIn> ComeIn;
	void Load(xmlQuote* x);
	int MaxWalkDist;
	int MinWalkDist;
	int MaxActionTime;
};
struct SBorn
{
	CityzenType *CT;
	const char* CityzenTypeName;
	int MaxTime;
};
class BildType
{
public:
	BildType();
	~BildType();
	DString Name;
	int NDSNameID;
	GAMEOBJ UnitType;
	DynArray<SBorn> CanBorn;
	void Load(xmlQuote* x);
};

class Cityzen
{
public:
	Cityzen();
	~Cityzen();
	CityzenType* CT;
	int LifeTime;
	word Index;
	OneObject* OB;
	GAMEOBJ Dyn;
	void Process();
	int State;
	int ProcesedTime;
	void ExecuteActions(int Comm);
	int GetSweetHomeIndex();
	int RandX;
	int RandY;
	int GetRandActions();
	int ActionTime;
	bool FreeForTalk;
	void SetMeeting();
};

class CityBild
{
public:
	CityBild();
	~CityBild();
	BildType* BT;
	int LastBornTime;
	word Index;
	OneObject* OB;
	GAMEOBJ DynGroup;
	void Process();
	int ProcesedTime;
};

class CityPopulation
{
public:
	CityPopulation();
	~CityPopulation();
	void Process();
	void LoadFromFile(char* file);
	
	static DynArray<CityzenType*> CType;
	static DynArray<BildType*> BType;

	static DynArray<CityBild*> CBild;
	static DynArray<Cityzen*> Civ;

	int GetCityzenTypeIndexByName(const char* nn);
	int GetBildTypeIndexByName(const char* nn);
	int GetUnitTypeIndex(const char* NDSName, GAMEOBJ* GG);

	void Init();
	void ResearchMap();
	void Clear();

	bool FirstStart;
	bool SetCallBack;

	static tpBirthCallback* PreviosBirthCallback;
	static int BirthCallback(int NewbornIndex);

};
#endif
