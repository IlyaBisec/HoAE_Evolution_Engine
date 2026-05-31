#include "..\common.h"
#include "..\IntExplorer\ParseRQ.h"
#include "import.h"
#include "DString.h"

#include <list>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
//-------------------------------------------//
struct Resource
{
	char name[64];
	int type;
};
//-------------------------------------------//
class ResourceList
{
public:
	ResourceList();
	virtual ~ResourceList();
	static int ResourceAmount;
	static vector<Resource> Resources;
	vector<Resource>::iterator ResourcesI;
	vector<int> Amo;
	static vector<int> BasePrice;
	vector<int>::iterator AmoI;
	void First();
	bool Next();
	void SetAmo(int vle);
	int GetAmo();
	void AddAmo(int vle);
	void AddResource(char *T, int type, int BasePricee=0);
	void SetAmo(int index, int vle);
	int GetAmo(int index);
	void AddAmo(int index, int vle);
	void ClearAmo();
	char* GetName(int index);
	int GetIndex(char* c);
	int GetType(int index);
	void LoadValue(vector<int> Up);
	void AddValueVector(vector<int> Up);
	bool ReadFromFile(GFILE* f);
};
//-------------------------------------------//
struct NamedResourceList
{
	char Name[64];
	ResourceList RL;
};
struct Can_produse
{
	char Name[32];
	int Param;
};
//-------------------------------------------//
class InterfaceOBJ
{
public:
	InterfaceOBJ();
	virtual ~InterfaceOBJ();
	void virtual Show();
	bool static Show(word Index);
	void static AddNewIndex(int Index, InterfaceOBJ* OBJ);
private:
	static InterfaceOBJ** FLIndex[512];
	static int ArrSZ; 
};
//-------------------------------------------//
class UnitInfo
{
public:
	UnitInfo();
	virtual ~UnitInfo();
	int x;
	int y;
	int GetDist(int dx,int dy);
	UnitInfo* GetUnitI();
	byte NMask();
	word NIndex();
	void UpdateCoord();
	void SetIndex(int In);
	int GetIndex();
	OneObject* OB;
private:
	int Index;
};

//-------------------------------------------//
class MobileUnit : public UnitInfo
{
public:
	MobileUnit();
	virtual ~MobileUnit();
	void MoveTo(int x,int y);
	GAMEOBJ Z;
	void UpdateCoord();
};
//-------------------------------------------//
class Peasant : public MobileUnit
{
public:
	Peasant();
	GAMEOBJ G;
	GAMEOBJ TargetZone;
	int TargetBildingIndex;
	void Die();
	byte Nation;
	GAMEOBJ *BildingUnitsCollector;
};
//-------------------------------------------//
class UnitDescription
{
public:
	UnitDescription();
	virtual ~UnitDescription();
	int NType;
	char Type[64];
	int StartMoney;
	int Tax;
	int StartPopulation;
	vector<NamedResourceList> RLVector;
	vector<Can_produse> CP;
	bool Load(char* f);
	vector<int> GetRLByName(char* n);
	bool ReadRL(GFILE* f, char* name);
	bool ReadCP(GFILE* f);
};
//-------------------------------------------//
class FindIfUnitDescription
{
public:
	bool operator()(UnitDescription UD)const
	{
		return UD.NType==NType;
	}
	static int NType;
};
//-------------------------------------------//
class NationParams
{
public:
	NationParams();
	~NationParams();
	GAMEOBJ NationPeasantType;
	int Nalog;
	int Kazna;
	int Profit;
	int NI;
	void MoneyAdd(int Sum);
	void Init(int Ni);

};
//-------------------------------------------//
class Business : public InterfaceOBJ
{
public:
	Business();
	virtual ~Business();
	int Tax;// %
	int PayTax(int Sum);
	int Nalogi;
	int Profit;//%
	int MyMoney;
	int BaseMoney;
	bool Bay(Business* Ukogo,int Sum);
	bool Sel(Business* Komu,int Sum);
	Business* GetBusiness();
	MobileUnit Unit;
	void FillParams(UnitDescription* UD, int Index);
	NationParams* NP;
	bool AutoFlag;
	void Show();
};//-------------------------------------------//
class Warehouse : public Business
{
public:
	Warehouse();
	virtual ~Warehouse();
	int Population;
	int Hunger;
	ResourceList AvailableRL;
	ResourceList NeededResourcesRL;
	ResourceList OutgoingRL;
	ResourceList PriceKf;
	ResourceList CheckAmo;
	ResourceList Price;
	ResourceList MinPrice;
	ResourceList In;
	ResourceList Out;
	ResourceList Booked;
	ResourceList BookedPrice;
	int CheckTime;
	int CheckTime2;
	int TimeInterval;
	void ProofPrice();
	int virtual GetPrice(int ResourceID);
	int NeedAmo(int ResourceID);
	void AddResource(int ResourceID, int Amo);
	void ShowParams();
	Warehouse* GetObject();
	void FillParams(UnitDescription* UD, int Index);
	bool ActivDipD;
	void MobilizateUnit(GAMEOBJ* Kuda);
	GAMEOBJ OBJ_ComeIntoBilding;
	byte WarehouseType;
	void CorrectPrice(Warehouse **TradePoints, int TradePointsSize);
	int GetMinPrice(Warehouse **TradePoints, int TradePointsSize, int ResID);
	void MakeGovernmentOrder(int ResID, int Amo);
};
//-------------------------------------------//
class Market : public Warehouse
{
public:
	Market();
	virtual ~Market();
	int GetPrice(int ResourceID);
	void FillParams(UnitDescription* UD, int Index);
};
//-------------------------------------------//
class Village : public Warehouse
{
public:
	Village();
	virtual ~Village();
	ResourceList ProduceRL;

	void Init();
	void Process();
	void ShowParams();
	void FillParams(UnitDescription* UD, int Index);
};
//-------------------------------------------//
class Transport : public Business
{
public:
	Transport();
	virtual ~Transport();
	Warehouse *Target;
	Warehouse *LoadTarget;
	Warehouse *Station;
	ResourceList RL;
	void Process();
	//void Bay(ResourceList *Ukogo, int chto, int skolko);
	//void Sel(ResourceList *Komu, int chto, int skolko);
	void Process(Warehouse **TradePoints, int TPSize, list<Transport*> *TrL);
	int GetEffect(Warehouse *W1,Warehouse *W2, list<Transport*> *TrL);
	void Load(Warehouse *Kuda);
	void UnLoad();
	Warehouse* GetTarget(Warehouse **TradePoints, int TPSize, list<Transport*> *TrL);
	bool IsLoad;
	void ShowParams();
	void FillParams(UnitDescription* UD, int Index);
};
//-------------------------------------------//
class Thing
{
public:
	Thing(); 
	ResourceList SpendRL;
	int Nation;
	int ResourceID;
	int Time;
	int Type;
	int NeedPeasant;
	int NeedHorce;
	char UnitTypeName[32];
	int UnitTypeID;
};
//-------------------------------------------//
class ProduceList
{
public:
	ProduceList();
	virtual ~ProduceList();
	static vector<Thing> Things;
	void AddThing(Thing Th);
	Thing* GetThing(int id);
	bool ReadPL(GFILE* f);
	bool ReadCreate(GFILE* f);
	int GetProduceIndex(int ResourceID);
	int GetProduceIndexByUnitName(char* UnitName);
};
//-------------------------------------------//
class Manufacture : public Warehouse
{
public:
	Manufacture();
	virtual ~Manufacture();
	static ProduceList PL;
	vector<int> ProduceListIndex;
	list<int> InProduceStack;
	bool IsWork;
	ResourceList InProduce;
	int InProduceTotalAmo;
	int ProduceResAmo;
	int ProduceTime;
	int GlobalTime;
	void Process(Warehouse **TradePoints, int TPSize);
	void FillParams(UnitDescription* UD, int Index);
	void ShowParams();
	void CMLunitsList(char* box);
	void CMLInProduceStack(char* box);
	void CMLobjectInfo(char* file);
	int GetProductPrice(int PLIndex);
	int GetPrice(int ResourceID);
};
//-------------------------------------------//
class Formation
{
public:
	Formation();
	virtual ~Formation();
	ResourceList NeedRL;
	GAMEOBJ FormType;
	char FormTypeTxt[20];
	char name[30];
	int ProduceListUnitID;
	int KolUnitsInForm;
	int ProduceListOffID;
	int ProduceListBarID;
	int ProduceListFlagID;
	Thing* UnitTh;
	Thing* OffTh;
	Thing* BarTh;
	Thing* FlagTh;
};
//-------------------------------------------//
class FormationList
{
public:
	FormationList();
	virtual ~FormationList();
	static vector<Formation> FL;
	//void AddNewFormaion();
	void FillFromFile(char* file);
};
//-------------------------------------------//
class AutoFormation
{
public:
	AutoFormation();
	virtual ~AutoFormation();
	GAMEOBJ Officers;
	GAMEOBJ Drummers;
	GAMEOBJ FlagBearers;
	GAMEOBJ Soldiers;
	GAMEOBJ DestGroup;
	Formation* Form;
	void CreateForm();
	void MoveTo(int x, int y);
	bool IsFormationReady();
	void Clear();
};
//-------------------------------------------//
class Barrack : public UnitInfo
{
public:
	Barrack();
	virtual ~Barrack();
	void Process();
	static FormationList Formations;
	list<int> InProduceStack;
	vector<int> ProduceList;
	list<int> Order;
	int MoneyToOrder;
	bool IsProduce;
	ResourceList AvailableRL;
	ResourceList NeededRL;
	ResourceList OrderRL;
	ResourceList PoehalZa;
	int MoneyToNeededRes;
	void Produce();
	bool StartProduce();
	AutoFormation FormationInProduce;
	bool ProduceFast(Thing* Chto, GAMEOBJ* Kuda); 
	void FillParams(UnitDescription* UD, int Index);
	void CMLunitsList(char* box);
	void CMLInProduceStack(char* box);
	void CMLOrderList(char* box);
	void CMLobjectInfo(char* file);
	int GetFormationsPrice(int FormationsID, int &Expensive);
	int GetFormationsPriceAndReservRes(int FormationsID);
	int static Compare(const void *W1, const void *W2 );
	int static ResID;
	UnitInfo static *ThisBilding;
	void FillTradePoints(Warehouse **TradePoints, int TPSize);
	NationParams *NP;
	Warehouse **TradePointsSorted;
	int TradePointsSize;
	void SortTradePoints();
	Barrack* This();
	bool InitFlag;
	//int ResourcesSize;
};

//-------------------------------------------//
class TaxCollector 
{
public:
	TaxCollector();
	virtual ~TaxCollector();
	int Money;
	int TakeMoney(Business *TaxPayer);
	void Process(Business **TaxPayerList,int TPLSize, list<TaxCollector*> *TCL);
	MobileUnit Unit;
	void FillParams(UnitDescription* UD, int Index);
	NationParams *NP;
};
//-------------------------------------------//

class NationOrders
{
public: 
	NationOrders();
};
//-------------------------------------------//
class Bearer : public MobileUnit
{
public:
	Bearer();
	virtual ~Bearer();
	Warehouse* Target;
	int Money;
	ResourceList RL;
	void Load(Warehouse* Owner);
	void Load(Barrack* Owner);
	void UnLoad(Barrack* Owner);
	void Create(int x, int y, byte Nat);
	void Erase();
	byte State;
	GAMEOBJ Dyn;
	GAMEOBJ GType;
};
//-------------------------------------------//
class Carrier
{
public:
	Carrier();
	virtual ~Carrier();
	vector<Bearer*> BearerList;
	//int FreeBearerAmount;
	int Timer;
	int TimerDelay;
	void Process();
	//void Process(Warehouse **TradePoints, int TPSize);
	Warehouse* OwnerW;
	Barrack* OwnerB;
};
//-------------------------------------------//
class Economic  
{
public:
	Economic();
	virtual ~Economic();
	
	void ResearchMap();
	void Process();

	list<Village*> VillageList;
	list<Transport*> TransportList;
	list<Warehouse*> WarehouseList;
	list<TaxCollector*> TaxCollectorList;
	list<Manufacture*> ManufactureList;
	list<Market*> MarketList;
	list<Peasant> NeededPeasantList;
	list<Peasant> PeasantList;
	list<Barrack*> BarrackList;
	list<Carrier*> CarrierList;
	vector<UnitDescription> UDList;
	NationParams NP[8];
	GAMEOBJ Temp;
	void EconomicDipComm(char* com,char** params,int npr,ParsedRQ* Result);
	void PerformCommand(char* Data,int size);
	void Init();
	int gggg;
	bool ActivFlag;
	int GetFreePeasant(byte Nation);
	void Mobilization();
	void AddInPeasantList();
private:
	Warehouse **TradePoints;
	int TradePointsSize;
	Business **TaxPayerList;
	int TaxPayerListSize;
	void FillTradePoints();
	int GlobalTime;
	void LoadNation(char* nat);
};
//-------------------------------------------//













struct ImportantPoint
{
	int Weight;
	int x;
	int y;
};
class Troop
{
public:
	Troop();
	Troop(int Index);
	virtual ~Troop();
	void Move(int x, int y);
	//GAMEOBJ G;
	OneObject* OB;
	int Index;
	int Weight;
	int Strength;
	int Speed;
	int LastX;
	int LastY;
	int LastForce;
};
///////
typedef tp2D[64][64];
class FormMovement
{
public:
	FormMovement();
	virtual ~FormMovement();
	void ResearchMap();
	void Process();
	void FillImportantPoint();
	int GetDist(Troop* T1,Troop* T2);
	int GetDist(Troop* T1,ImportantPoint* IP);
	int TIMER;
	int CounterI;
	void GetDangerCoord(Troop* T, int &x,int &y);
	void AddEnerPoint(tp2D& Map, int Force, int sx, int sy);
private:
	Troop** FormList;
	int FormListSize;
	Troop** EnmFormList;
	int EnmFormListSize;
	ImportantPoint IP[30];
	int ImportantPointSize;
	tp2D DangMap;
	tp2D ForceMap;
	int GetDangFor(Troop* T, int x,int y);
};
