#pragma once

#include "unitability.h"
#include "UnitChar.h"

HeroVariableStorage* DetectHero(OneObject* OB);

bool GetInsideDialogState();
void SetInsideDialogState(bool State);
//

ActiveUnitAbility* AddCarryAbility(OneObject* OB, UnitAbility* UA, bool DeleteAfterUse=false);
ActiveUnitAbility* DetectCarryAbility(OneObject* OB, UnitAbility* UA);

UnitAbility* getAblByName(const char* name);
//ActiveUnitAbility* DetectCarryAbility(OneObject* OB, char* UnitAbilityName);

ActiveUnitAbility* AddAblNameAbility(OneObject* OB, AblName* AN);

bool DeleteCarryAbility(OneObject* OB, UnitAbility* UA);
bool DeleteCarryAbility(OneObject* OB, char* Name);

void SetMagazineState(OneObject* OB, bool Enable);
bool GetMagazineState(OneObject* OB);

//

enum enABL_STATE { ABL_ABSENT=0, ABL_ACTIVE=1, ABL_PASIVE=2, ABL_RECHARGE=3, ABL_HIDE=4 };

void		SetAblState( OneObject* pOB, const char* AblName, enABL_STATE state );
enABL_STATE	GetAblState( OneObject* pOB, const char* AblName );

int			GetAblList( OneObject* pOB, ClonesArray<_str>& AblNameList );

//Дать герою предмет/Нужна проверка наличия определенного предмета у героя 
//Открыть/Закрыть магазин

ClonesArray<AblName>* GetDialogAbilities(OneObject* OB);

enum en_AblExecState { aes_Casting=1, aes_CanNotTarget=2, aes_WaitForCast=4, aes_AnotherCasting=8, aes_Absent=0 };

//en_AblExecState GetAblExecuteState(char* AbilityName, OneObject* Caster, OneObject* Target, int x, int y, int z);
//en_AblExecState AblExecute(char* AbilityName, OneObject* Caster, OneObject* Target, int x, int y, int z);

en_AblExecState GetAblExecuteState(UnitAbility* UA, OneObject* Caster, OneObject* Target, int x, int y, int z);
en_AblExecState AblExecute(UnitAbility* UA, OneObject* Caster, OneObject* Target, int x, int y, int z);

void GiveExperience(OneObject* OB, int Expa);		// дать герою експу
void IncreaseLevel(OneObject* OB, int Level);

void DisableCapturability(OneObject* OB);

void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);

//

void DialogEvent(_str* Talker, _str* State, _str* Message, bool NewDialog);