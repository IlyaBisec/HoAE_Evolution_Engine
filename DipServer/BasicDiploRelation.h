class DiplomacySystem;
class DIP_API BasicDiploRelation{
public:
	//bool ready;
	byte Owner;
	int  DiploCost[6];
	char NameOfTribe[32];
	word Index;
	bool busy;
	GAMEOBJ CentralGroup;	
	char CurrentStateMessage[64];
	byte Busy;
	DiplomacySystem* DIP;
	virtual void InitThisTribe(char* Preffix,char* FirstGroupName,char* CommonID);
	virtual void InitThisTribe2(char* FileID, char* Dip, char* GDance, char* ZDance);
	virtual int GetAmountOfPropositions();
	virtual DWORD GetPossibility(int Index,char* Message,char* Hint);
	virtual void PerformPossibility(DWORD Code);
	virtual void ProcessTribe();
	virtual void PerformCommand(char* Data,int size);
	virtual int GetSaveData(byte* buf,bool write);
	virtual void SetSaveData(byte* buf,int size);
	virtual void Init();
	virtual void Free();
};