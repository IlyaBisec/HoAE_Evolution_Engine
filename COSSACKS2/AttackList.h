#pragma pack ( push )
#pragma pack ( 1 )
#define NARows 1791
class AttList{
	struct OneVictim;
	struct OneVictim{
		word ID;
		word SN;
		DynArray<word> AttIDSN;
	};
	typedef DynArray<OneVictim*> VArr;
	VArr VList[NARows];    
public:
	void Add (OneObject* Attacker,OneObject* Victim);
	void Del (OneObject* Attacker,OneObject* Victim);	
	void DelV(OneObject* Victim);
	int  GetNAttackers(OneObject* Victim);
	int  GetLifeAfterNextShot(OneObject* Victim);
	void Refresh();
	void Clear();
	~AttList();
};
extern AttList ATTLIST;
#pragma pack ( pop )