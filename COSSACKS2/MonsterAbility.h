#pragma once

class MonsterAbility : public BaseClass
{
public:
	ClassArray<_str> AbilityNames;

	DynArray<int> ItemDrop;
	DynArray<int> CommonRoomStartParam;
	DynArray<UnitAbility*> AbilitiesList;

	ClonesArray<_str> Names;
	LinearArray<int,_int> PicFiles;
	LinearArray<int,_int> PicSprites;
	ClonesArray<_str> Hints;

	LinearArray<bool,_bool> DeleteAfterUse;

	ClonesArray< be_CRefParamFunction > be_Processes;
	ClonesArray< be_CRefParamFunction > be_LeftClicks;
	ClonesArray< be_CRefParamFunction > be_EnterDialogs;

	bool Feeled;

	SAVE(MonsterAbility){
		REG_AUTO(AbilityNames);

		REG_AUTO(Names);
		REG_AUTO(PicFiles);
		REG_AUTO(PicSprites);
		REG_AUTO(Hints);

		REG_AUTO(DeleteAfterUse);

		REG_AUTO(be_Processes);
		REG_AUTO(be_LeftClicks);
		REG_AUTO(be_EnterDialogs);

	}ENDSAVE;

	MonsterAbility();
	bool Process(int UnitIndex);
	void Init(NewMonster* NM);

};
