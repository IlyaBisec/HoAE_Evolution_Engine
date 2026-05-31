struct SetlOneBrigAbilka{
	byte Slot;
	int DSB_ID;		// Dip_Simple_Building_ID
	word ID;		// in array ABILS
	byte AmountMax;
	byte AmountStart;
	int Restore;	//Time to restore one amount
	char* Hint;
	void Init(int DSB_id);
	void Draw();
};

#define ReinfBrig 20
struct S_ReinforceBrig{
	int BrigID;
	word CopID;
	word CopSN;
	void Process();
};

class DIALOGS_API SetlUpgrade:public BaseClass{
public:
	SetlUpgrade(){
	}
	int Index;
	int DSB_ID;	// Dip_Simple_Building_ID

	word IconFile;
	int Icon;
	int X,Y;
	char HintID[32];
	char* Hint;
	int Duration;

	byte EffType;
	BaseResources Effect;
	
	byte PriType;
	BaseResources Price;

	bool isComplete;
	bool isRun;
	int RunTime;

	// SAVE
	//virtual int GetSaveData(byte* buf,bool write);
	//virtual int SetSaveData(byte* buf);
	//
	void Init(int id, int DSB_id, DString* str);
	bool Run();
	void Process();
	void Draw();	
	void Draw(DialogsSystem* DS, int x, int y);
	//
	SAVE(SetlUpgrade){
		SetReadOnlyMode(); REG_MEMBER(_int, Index);
		SetReadOnlyMode(); REG_MEMBER(_int, DSB_ID);
		REG_MEMBER(_gpfile,IconFile);
		REG_SPRITE(Icon,IconFile);
		REG_MEMBER(_int,X);
		REG_MEMBER(_int,Y);
		REG_MEMBER(_textid,Hint);
		REG_MEMBER(_int,Duration);	
		REG_ENUM(_byte_index,EffType,SU_EFF_TYPE);
		REG_AUTO(Effect);
		REG_ENUM(_byte_index,PriType,SU_PRICE);
		REG_AUTO(Price);
		SetReadOnlyMode(); REG_MEMBER(_bool, isComplete);
		SetReadOnlyMode(); REG_MEMBER(_bool, isRun);
		SetReadOnlyMode(); REG_MEMBER(_int, RunTime);
	}ENDSAVE;

};
class  SettlementInfo:public GroupExInfo{
public:
	SettlementInfo(){
		InfID=1;
	}
	void Load();
	void Save(char* FileName);

	_str SourceFile;
	byte LoadingRules;
	byte StartOwner;
	BaseResources MaxResAmount;
	_str CopID;
	int CopBuidTime;
	int CopMaxAmount;
	_str PeonID;
	int PeonBuidTime;
	BaseResources PeonMaxAmount;
	BaseResources PeonTimeOnProduce;
	_str ObozID;
	BaseResources ObozResAmount;
	class UpgArray:public ClassArray<SetlUpgrade>{
	public:
		virtual int GetExpansionRules(){
			return 1;
		}
	};
	UpgArray Upgrades;
	_str SecondGroupName;

	SAVE(SettlementInfo){	
		REG_LOADSAVE("Dip\\","*.dip.xml");
		REG_PARENT(GroupExInfo);
		REG_AUTO(SourceFile);
		REG_ENUM(_byte_index,LoadingRules,LoadingRules);
		REG_MEMBER(_BYTE,StartOwner);
		REG_AUTO(SecondGroupName);
		REG_AUTO(MaxResAmount);
		REG_AUTO(CopID);
		REG_MEMBER(_int,CopBuidTime)
		REG_MEMBER(_int,CopMaxAmount)
		REG_AUTO(PeonID);
		REG_MEMBER(_int,PeonBuidTime)
		REG_AUTO(PeonMaxAmount);
		REG_AUTO(PeonTimeOnProduce);
		REG_AUTO(ObozID);
		REG_AUTO(ObozResAmount);
		REG_AUTO(Upgrades);
	}ENDSAVE;
};

#define MaxCaravan 25
struct DipCaravan{
	word ID;
	word SN;

	word DSBID;		// dip simple building id

	word DestStorageID;
	word DestStorageSN;	
	
	//order
	// 0 - wait
	// 1 - building
	// 2 - loading
	// 3 - run order
	byte State;

	byte Owner;
	
	byte ResID;
	int ResAmount;
	
	void Init(word dsbid);
	bool isEmpty();
	bool isFree();	
	bool Run(byte res, int amount);
	void Process();

};
#define BrgAbNx 3
#define BrgAbNy 3
struct BrigAbility{
	int ID;
	//int Place;
	int RestoreTime;
	int AmountMax;

	int AmountCur;
};

struct DipComParams{
	char MessageID[ 32];
	char HintID   [ 32];
	char ProcessID[ 32];
	int  Cost     [  6];
	char ComID    [ 16];
	char Params   [256];
};

class DIP_API DIP_SimpleBuilding:public BasicDiploRelation{
public:
	DipComParams* DIPPAR;
	int NDipPar;
	int ProcessID;
	//
	GAMEOBJ DestGroup;
	GAMEOBJ BigZone;
	GAMEOBJ VeryBigZone;
	//
	GAMEOBJ SecondGroup;
	GAMEOBJ ProduceGroup;
	//dance group
	short   ActorsType;
	short   NStartActors;
	GAMEOBJ Actors;
	GAMEOBJ DanceZone;
	int DanceStage;
	//repair
	GAMEOBJ Repairers;
	GAMEOBJ Defenders;
	GAMEOBJ Alexanders;
	GAMEOBJ ComeInside;
	// ac addon
	GAMEOBJ Friends;
	bool EasyTalk;		// no need diplomats (need anyone unit)
	//
	short RepType;
	short DefType;
	short MaxRep;
	short MaxDef;
	short AddDefAttack;
	short AddDefShield;
	//---------
	char CongratID[32];		// congratulation text
	char FirstMeetID[32];
	char ReSellID[32];
	char TellNo[32];
	char gpPix[32];
	int ReSellCost[6];
	int gpidx;
	static int CurIndex;
	union{
		struct ProdUnits{
			int Stage;
			int StartTime;
			GAMEOBJ UnitType;
			int Amount;
#ifdef AC_ADDON
			byte Speed;
#endif AC_ADDON
		}Prod;
	}info;
	bool DipProposition;
	byte CounsilNI;
	bool UseLikeSkarb;
	bool AlreadyUsedLikeSkarb;
	int StartRelTime;
	// Debug
	int NCopInfo;
	int NOwnerInfo;
	// Cossacks2
	bool Settlement;
	//
	word StorageID[8];
	word StorageSN[8];
	char CentralName[128];
	//
	int CopBuild;				// ticks
	int CopNIndex;
	int NCops;
	int DefPolk;
	//
	GAMEOBJ PeasGrp[6];
	GAMEOBJ PeasReinfGrp[6];	// пополнение крестьянами игрока (interface)
	int PeasantBuild;			// ticks
	int PeasantNIndex;
	int NPeasants[6];	
	int Produce[6];
	int LastProduce[6];
	int Resource[6];
	int ResourceMax[6];
	int CaravanNIndex;
	int CaravanCapacity[6];
	DipCaravan Caravan[MaxCaravan];
	GAMEOBJ NewUnits;
	BrigAbility BrgAb[BrgAbNx*BrgAbNy];	
	GAMEOBJ ReinfGrp;
	word ClientOfID;
	word ClientOfSN;
	SetlOneBrigAbilka BrigAbilka[9];
	ClassArray<SetlUpgrade> SetlUpg;
	//
#ifdef DIALOGS_DIP_USER
	static tpDrawOnMiniMapCallback* fDrawOnMiniMap;
	static int FlagFileID;
	static tpDrawOnMapCallback* fDrawOnMap;
	static int ArrowFileID;
	static word CurSetlSelected;
#endif DIALOGS_DIP_USER
	//
	virtual void InitThisTribe(char* Preffix,char* FirstGroupName,char* CommonID);
	virtual void InitThisTribe2(char* FileID, char* Dip, char* GDance, char* ZDance);
	virtual int GetAmountOfPropositions();
	virtual DWORD GetPossibility(int Index,char* Message,char* Hint);
	virtual void PerformPossibility(DWORD Code);
	virtual void ProcessTribe();
	virtual void PerformCommand(char* Data,int size);
	// SAVE
	virtual int GetSaveData(byte* buf,bool write);
	virtual void SetSaveData(byte* buf,int size);
	//
	virtual void Init();
	virtual void Free();
	void PerformProduceUnitsCommend();
	void PerformRazvedka();
	void PerformFiring();
	void PerformStorms();
	void PerformKillers();
	void PerformTomahawks();
	void CreateAbilityPage();
	void ProcessDance();
	void ShowNoMoney();
	// Cossacks2 func
	void SetUp();
	void GetCenter(int &x, int &y);
	OneObject* GetStorage(byte resID);
	OneObject* GetMainObj();
	//
	int setlGetWorkers(byte resID);
	int setlGetWorkersIntrf(byte resID);
	int setlGetWorkersFreePlacesIntrf(byte resID);
	bool setlAddWorker(byte resID, OneObject* OB);
	bool setlAddWorkerIntrf(byte resID, OneObject* OB);
	void setlFindNearStorages();
	void setlProcessCops();
	int setlGetNDeffenders(bool Reman=true);
	bool setlUpgIsRun(byte ResType, int Level){
		int n=SetlUpg.GetAmount();
		int l=-1;
		for(int i=0;i<n;i++){
			SetlUpgrade* su=SetlUpg[i];
			if(su->Effect[ResType]>0&&su->isRun){
				l++;			
			}
		}
		return l>=Level;
	};
	//
	void Save(SettlementInfo* si);
	void Load(char* GroupName, SettlementInfo* si);
	//
	void DrawSettlementInterface(int x0,int y0,int Lx0,int Ly0,int x1,int y1);
	void DrawSetlUpg(DialogsSystem* DS, int x, int y);
};

CEXPORT word GetPlayerNMask(byte NI);