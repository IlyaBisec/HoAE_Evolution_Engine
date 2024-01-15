#pragma once
//#include "unitability.h"
//#include "vui_Action.h"
//#include "vui_InterfSynchro.h"
//////////////////////////////////////////////////////////////////////////
extern int TrueTime;
//////////////////////////////////////////////////////////////////////////

class DIALOGS_API IncomingAbility : public BaseClass {
public:
	
	DWORD Serial;
	int x;
	int y;

	SAVE(IncomingAbility){
		REG_MEMBER(_DWORD,Serial);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
	}ENDSAVE;
};
class DIALOGS_API cvi_Hero: public BaseClass {
public:

	word Index;
	word Serial;

	int DamageTime;
	ClonesArray<IncomingAbility> InAbl;

	SAVE(cvi_Hero){
		REG_MEMBER(_WORD,Index);
		REG_MEMBER(_WORD,Serial);
		REG_MEMBER(_int,DamageTime);
		REG_AUTO(InAbl);
	}ENDSAVE;

	cvi_Hero() {}

};

class DIALOGS_API cvi_HeroButtons: public BaseClass
{
private:
	bool Visible;
public:
	//LinearArray<word,_WORD> hIndex;
	//LinearArray<word,_WORD> hSerial;
	//LinearArray<int,_int> hDamageTime;

	ClonesArray<cvi_Hero> Hero;

	//
	SAVE(cvi_HeroButtons){
		REG_MEMBER(_bool,Visible);
		//REG_AUTO(hIndex);
		//REG_AUTO(hSerial);
		//REG_AUTO(hDamageTime);
		REG_AUTO(Hero);
	}ENDSAVE;
	//
	cvi_HeroButtons(){
		Visible=true;
	}	
	void Clear(){
		SetVisible(true);
		//hIndex.Clear();
		//hSerial.Clear();
		//hDamageTime.Clear();
		Hero.Clear();
	};
	void SetVisible(bool State){
		Visible=State;
	}
	bool GetVisible(){
		return Visible;
	}
	bool isHero(OneObject* OB);	
	void checkDamage(OneObject* OB);
	
	bool Add(OneObject* OB);
	bool Remove(OneObject* OB);

	int GetAmount(byte NI);
	OneObject* GetObject(byte NI, int ID);
	cvi_Hero* Get(OneObject* OB);
};

extern DIALOGS_API cvi_HeroButtons vHeroButtons;

// vui_Actions
regAc(cva_Hero_Button, vfS vfL vfD
	cva_Hero_Button();
	int Index;
	ClassRef<GPPicture> Pic;
	int PicLx;
	int PicLy;
	ClassRef<VitButton> FreeLevel;
	ClassRef<ProgressBar> LifeBar;
	ClassRef<ProgressBar> ManaBar;
	ClassRef<ProgressBar> CoolDown;
	,
	REG_MEMBER(_int,Index);
	REG_AUTO(Pic);
	REG_MEMBER(_int,PicLx);
	REG_MEMBER(_int,PicLy);
	REG_AUTO(FreeLevel);
	REG_AUTO(LifeBar);
	REG_AUTO(ManaBar);
	REG_AUTO(CoolDown);
);

//////////////////////////////////////////////////////////////////////////
void SetScreenCenterToXY(int x, int y);
void ImClearSelection(byte Nat);
void AddUnitToImSelected(byte NI,OneObject* OB);
void PrepareToImSelection(byte NI);
void FinalizeImSelection(byte NI);
//////////////////////////////////////////////////////////////////////////
