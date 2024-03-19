#pragma once
//#include "unitability.h"
#include "HeroAbility.h"
#include "ua_Ability.h"
//==================================================================================================================//
class NewMonster;
//==================================================================================================================//
class NewItem: public NewAbility{
public:	
	NewItem();

	bool Clothe;
	//int Amount;
	bool SelfTarget;

	int SoundDress;
	int SoundUndress;

	//
	//int ItemDrop;	// если убивают NewMonster::ItemDrop==ItemDrop тогда есть вероятность что выпадет эта вещь из убитого
	//int ItemDropPart; // доля выпадения, 0 - никогда, 10 - если у юнита один предмет то - 50%, если 2 - 33%, если 3 - 25%
	//

	SAVE(NewItem){
		REG_BASE(UnitAbility);
		REG_MEMBER(_bool,Clothe);
		//REG_MEMBER(_int,Amount);
		REG_MEMBER(_bool,SelfTarget);

		REG_MEMBER(_bool,CanNotBeClothe);
		REG_ENUM(_index, TypeClothe, ItemClotheType);

		REG_ENUM(_index,SoundDress,ALL_SOUNDS);
		REG_ENUM(_index,SoundUndress,ALL_SOUNDS);

		//REG_MEMBER(_int,ItemDropPart);
		//REG_MEMBER(_bool,HeroLife);

		REG_PARENT(NewAbility);
	}ENDSAVE;

	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class OneItem: public OneAbility{
public:
	__constructor(OneItem){
		TypeAbil=4;
		Droped=false;
	}

	word NIndex; // newmonster of item OneObject
	bool Deleted;
	NewMonster* NM;
	//
	bool MayBeDropped;
	//
	SAVE(OneItem){
		REG_PARENT(OneAbility);
		REG_MEMBER(_int,Amount);
		REG_MEMBER(_short,NIndex);
	}ENDSAVE;
	//
	virtual bool Process();
	//
	virtual bool OnClick();
	virtual bool OnRightClick();
	virtual bool isTarget();
	virtual bool CanTarget(word TargetUnit, int x, int y, int z);
	virtual int GetCoolDownProc();

	virtual bool GetAuraActiveState();
	//
};
class ua_DropPackage: public NewItem{
public:
	ClonesArray< ClassRef<NewAbility> > DropPackage;
	
	SAVE(ua_DropPackage){
		REG_BASE(UnitAbility);
		REG_AUTO(DropPackage);
		REG_PARENT(NewItem);
	}ENDSAVE;

	virtual ActiveUnitAbility* GetNewActiveAbility();

};
class uaa_DropPackage: public OneItem{
public:
	SAVE(uaa_DropPackage){
		REG_PARENT(OneItem);
	}ENDSAVE;

	virtual bool Process(){
		return OneAbility::Process();
	};
};
//
extern char* WeaponIDS[32];
//
class ref_NewItem: public NewItem{ //ClassRef<NewItem>
public:
	int Kind;
	/*
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest.Clear();
		dest.print("WeaponKind: %s",WeaponIDS[Kind]);		
		return true;
	}
	*/
	const char* GetThisElementView(const char* LocalName){
		static _str dest;
		dest.Clear();
		dest.print("WeaponKind: %s",WeaponIDS[Kind]);
		return dest.pchar();
	};
};
//
class NewItemTransformer: public NewItem{
public:	
	NewItemTransformer(){
		for(int i=0;i<4;i++){
			WeaponKind[i].Kind=i;
		}
	}
	//bool Clothe;
	//int Amount;
	//bool SelfTarget;
	//
	//int ItemDrop;	// если убивают NewMonster::ItemDrop==ItemDrop тогда есть вероятность что выпадет эта вещь из убитого
	//int ItemDropPart; // доля выпадения, 0 - никогда, 10 - если у юнита один предмет то - 50%, если 2 - 33%, если 3 - 25%
	ref_NewItem WeaponKind[4];
	//
	SAVE(NewItemTransformer){		
		REG_BASE(UnitAbility);
		REG_AUTO(WeaponKind[0]);
		REG_AUTO(WeaponKind[1]);
		REG_AUTO(WeaponKind[2]);
		REG_AUTO(WeaponKind[3]);
		//REG_PARENT(NewAbility);
		REG_PARENT(NewItem);
	}ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class OneItemTransformer: public OneItem{
public:	
	SAVE(OneItemTransformer){
		REG_PARENT(OneItem);
	}ENDSAVE;
	virtual UnitAbility* GetA();
	//virtual bool Process();
};
//
class ua_Cookie: public BaseClass {
public:
	ClassRef<NewItem> Product;
	ClonesArray< ClassRef<NewItem> > Ingredients;

	SAVE(ua_Cookie){
		REG_AUTO(Product);
		REG_AUTO(Ingredients);
	}ENDSAVE;

};
class ua_CookBook: public BaseClass {
public:
	ClonesArray<ua_Cookie> Cookies;

	SAVE(ua_CookBook){
		REG_AUTO(Cookies);
	}ENDSAVE;

	bool CheckProduct(OneObject* OB);

};
extern ua_CookBook CookBook;
//
class NewMagazine: public UnitAbility{
public:
	int ShopCategory;
	SAVE(NewMagazine){
		REG_ENUM(_index,ShopCategory,ShopCategory);
		REG_PARENT(UnitAbility);
	}ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class OneMagazine: public ActiveUnitAbility{
public:
	OneMagazine(){
		TypeAbil=2;
	}
	virtual bool Process();
	//
	word HeroID;
	word HeroSN;
	//
	SAVE(OneMagazine){
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_short,HeroID);
		REG_MEMBER(_short,HeroSN);
	}ENDSAVE;
};

class MoneyAward: public BaseClass{
public:
	int Resource;
	int Value;
	int ValueRandom;

	SAVE(MoneyAward){
		REG_ENUM(_index, Resource, RESTYPE);
		REG_MEMBER(_int,Value);
		REG_MEMBER(_int,ValueRandom);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest.Clear();
		dest.print("MoneyAward: %d..%d ",Value,Value+ValueRandom);
		dest+="%$Resource$%";
		return true;
	}
};
//
class NewBonus: public NewAbility{
public:
	int Charge;
	ClonesArray<MoneyAward> Money;
	MoneyAward Money2;
	
	bool EraseAfterUse;
	bool ShowFocusedUnitHint;

	SAVE(NewBonus){
		REG_BASE(UnitAbility);
		REG_MEMBER(_int,Charge);
		REG_AUTO(Money);
		REG_AUTO(Money2);
		//REG_MEMBER(_bool,HeroLife);
		REG_MEMBER(_bool,EraseAfterUse);
		REG_MEMBER(_bool,ShowFocusedUnitHint);
		REG_PARENT(NewAbility);		
	}ENDSAVE;

	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class OneBonus: public OneAbility{
public:	
	int Charge;
	//bool Enable;
    
	word HeroID;
	word HeroSN;

	SAVE(OneBonus){		
		REG_PARENT(OneAbility);
		REG_MEMBER(_int,Charge);
		//REG_MEMBER(_bool,Enable);
		REG_MEMBER(_short,HeroID);
		REG_MEMBER(_short,HeroSN);
	}ENDSAVE;

	OneBonus(){
		TypeAbil=2;
		//Enable=true;
	}
	virtual bool Process();
	inline bool isCharged(){
		NewBonus* NB=(NewBonus*)GetA();
		return (Charge>0 || NB->Charge==0);
	}
};
//
class DIALOGS_API uaa_Chest: public OneAbility{
public:
	bool Empty;
	int NextSearchTime;

	SAVE(uaa_Chest){
		REG_PARENT(OneAbility);
		REG_MEMBER(_bool,Empty);
		REG_MEMBER(_int,NextSearchTime);
	}ENDSAVE;

	virtual bool Process();
	//virtual bool OnClick();
	//virtual bool OnRightClick();
	//virtual bool isTarget();
	//virtual bool CanTarget(word TargetUnit, int x, int y, int z);
};
class ua_Chest: public NewAbility{
public:	
	SAVE(ua_Chest){
		REG_BASE(UnitAbility);
		REG_PARENT(NewAbility);
	}ENDSAVE;

	virtual ActiveUnitAbility* GetNewActiveAbility(){
		uaa_Chest* nAA=new uaa_Chest;
		nAA->SetA(this);
		return nAA;
	};
};

HeroVariableStorage* DetectHero(OneObject* OB);
uaa_Chest* DetectChest(OneObject* OB);
OneBonus* DetectBonus(OneObject* OB);
