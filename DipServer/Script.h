class CPlayerScript{
public:
	CPlayerScript(){Init();}
	~CPlayerScript(){Free();}
	
	byte PlayerID;
	bool Enable;
	bool Defeat;

	int NProtectGrp;	// NIndexes of units to defend 
	word* ProtectGrp;

	bool AIUpg[4096];
	bool UpgEnabled[4096];

	void Init(){memset(this,0,sizeof(*this));}
	void Free(){if(ProtectGrp){free(ProtectGrp);} Init();}

	bool AddProtect(word NIndex);
};

extern CPlayerScript PScript[8];

class CScript{
public:
	byte CurPlayer;
};
