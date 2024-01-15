#ifndef _POLK_H_
#define _POLK_H_
//====================================================================//
class DIALOGS_API Polk
{
public:
	Polk();
	virtual ~Polk();
	void virtual MovePolkTo(int x, int y, int Dir=0);
	int  virtual GetEnemyForce();
	int  virtual GetPolkIndex();
	bool virtual ReSetFormation(int FormIndex);
	void virtual AddSelectedToPolk(byte Nat);
	void virtual AddBrigadeToPolk(word Nat, int BrIndex);
	int  virtual GetFormationIndex(int UnitType);	
	void virtual RemanPolkFrom(GAMEOBJ* Group);
	int  virtual GetUnitsAmountInPolk(bool reman=false);
	int  virtual GetNFormations();
	bool virtual GetFormation(int Index, byte& NI, int& BrigID);
	bool virtual AddBrigade(Brigade* B);
	bool virtual AddCannon(OneObject* Ob);
	bool virtual GetPolkFrontCenter(int& x, int& y);
};
//====================================================================//
#endif