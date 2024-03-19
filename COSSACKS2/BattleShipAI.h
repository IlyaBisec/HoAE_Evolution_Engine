class TopInfo : public BaseClass
{
public:
	TopInfo();
	
	int NKillsF;
	int NKillsE;
	int NDamageF;
	int NDamageE;
	int NShots;
	int MinDistToEnmBld;
	int MinDistToFrnBld;
	int EnmBldW;
	int FrdBldW;

	int GetBldW(int Dist);

	SAVE(TopInfo)
	ENDSAVE;
};
class NationalNavy : public BaseClass
{
public:
	NationalNavy();
	NationalNavy(int NI, byte Top);
	~NationalNavy();
	
	int NI;
	byte TopType;
	int ProcessTime;

	//ClonesArray<int> DangerMap;
	ClassArray<TopInfo> TopZons;

	byte GetMask();
	void RegisterBuilding(OneObject* Blb, int K);
	void Process();
	
	SAVE(NationalNavy)
	ENDSAVE;
};

class BattleShipAI : public GameExtension
{
public:
	BattleShipAI();
	~BattleShipAI();

	ClassArray<NationalNavy> NationalsNavyInfo;

	virtual void ProcessingGame();
	virtual void OnGameStart();
	virtual void OnGameEnd();
	virtual bool OnMapLoading(xmlQuote& xml);
	virtual bool OnGameSaving(xmlQuote& xml);
	virtual void OnUnitBirth(OneObject* NewUnit);
	virtual bool OnUnitDie(OneObject* Dead,OneObject* Killer);
	virtual bool OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType);
	
	SAVE(BattleShipAI)
	ENDSAVE;

};