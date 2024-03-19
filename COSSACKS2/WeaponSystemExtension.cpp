#include "stdheader.h"
#include "WeaponSystem.h"
#include "GameExtension.h"
#include "WeaponSystemExtension.h"
//==================================================================================================================//
void WeaponSystemExtension::OnClassRegistration(){
	REG_CLASS(NewFrame);
	
	//==============================================================//
	REG_CLASS(TargetDesignation);
	REG_CLASS(WeaponParams);
	REG_CLASS(AdditionalWeaponParams);
	//
	//======WeaponSystem======//
	//
	REG_CLASS(WeaponSystem);
	REG_CLASS(WeaponModificator);
	REG_CLASS(WeaponProcess);
	//
	//======PointModificator======//
	//
	REG_CLASS(PointModificator);
	//
	REG_CLASS_EX(SelfMurder,"Base");
	REG_CLASS_EX(PlaySomeSound,"Base");

	REG_CLASS_EX(DrawOne,"Graph");
	REG_CLASS_EX(graph_ChangeColor,"Graph");
	REG_CLASS_EX(graph_ChangeScale,"Graph");
	REG_CLASS_EX(graph_Rotate,"Graph");
    REG_CLASS_EX(ChangeAttachedEffectMask,"Graph");

	REG_CLASS_EX(pm_ApplyStandartChasm,"Terrain");
	REG_CLASS_EX(pm_ApplyLandTypeSlow,"Terrain");
	REG_CLASS_EX(pm_ApplyLandType,"Terrain");
	REG_CLASS_EX(pm_OnWaterClearLockPoints,"Terrain");
	REG_CLASS_EX(pm_OnWaterRestoreLockPoints,"Terrain");
	REG_CLASS_EX(pm_OnLandSetLockPoints,"Terrain");
	REG_CLASS_EX(pm_OnLandRestoreLockPoints,"Terrain");
	REG_CLASS_EX(pm_FogSpot,"Terrain");
	REG_CLASS_EX(ChangeTreesLandTypeInRadius,"Terrain");

	REG_CLASS_EX(Jump,"Motion");
	REG_CLASS_EX(BalisticMotion,"Motion");
	REG_CLASS_EX(pm_Roam,"Motion");
	REG_CLASS_EX(FollowUnit,"Motion");
	
	REG_CLASS_EX(StaticMotion,"Motion");
	REG_CLASS_EX(HarmonicMotion,"Motion");	
	REG_CLASS_EX(Wave,"Motion");
	REG_CLASS_EX(Motion,"Motion");
	REG_CLASS_EX(RandomizePosition,"Motion");

	REG_CLASS(MassModify);
	REG_CLASS(MassModifyCollect);
	REG_CLASS(mmc_UnitsPrice);
	REG_CLASS(mmc_UnitsAmount);
	REG_CLASS(mmc_UnitsLife);
	REG_CLASS(mmc_Resurrect);
	REG_CLASS(MassModifyDistribute);
	REG_CLASS(mmd_TakeResource);
	REG_CLASS(mmd_TakeLife);
	REG_CLASS(mmd_TakeMana);
	REG_CLASS(mmd_Summon);

	REG_CLASS_EX(MassBirthNew,"Weapon");
	REG_CLASS_EX(MassDelete,"Weapon");
	REG_CLASS_EX(ChangeModificator,"Weapon");
	REG_CLASS_EX(BirthNew,"Weapon");
	//
	REG_CLASS_EX(pm_StealResourceFromBuildings,"Unit");
	REG_CLASS_EX(Polymorph,"Unit");
	REG_CLASS_EX(MakeDamage,"Unit");
	REG_CLASS_EX(BirthNewAbility,"Unit");
	REG_CLASS_EX(BirthNewUnit,"Unit");
	REG_CLASS_EX(BirthNewUnitsFromSprites,"Unit");
	REG_CLASS_EX(DeleteSpritesInRadius,"Unit");
	REG_CLASS_EX(ChangeNation,"Unit");
	//
	// TargetFinder
	REG_CLASS(TargetFinder);
	REG_CLASS(SpriteInRadius);
	REG_CLASS(UnitsInRadius);
	REG_CLASS(RandomPosInRadius);
	REG_CLASS(UserDefinedPoints);
	REG_CLASS(UserDefinedPointCoor);
	//
	//======WeaponEvent======//
	//
	REG_CLASS(WeaponEvent);
	REG_CLASS(WeaponEventPtr);
	//
	REG_CLASS(AND_several_events);
	REG_CLASS(OR_several_events);
	REG_CLASS(NOT_event);
	REG_CLASS(True);	
	REG_CLASS(TimePeriod);
	REG_CLASS(we_Timer);
	REG_CLASS(we_Random);
	REG_CLASS(Trigger);
	REG_CLASS(TargetReached);	
	REG_CLASS(IsFirstStep);
	REG_CLASS(IsTargetDie);
	REG_CLASS(IsTargetInvisible);
	REG_CLASS(WaterIsThere);
	REG_CLASS(TerritoryLockedForUnits);
	REG_CLASS(TerritoryLockedForShips);
	REG_CLASS(RandomEvent);
	//REG_CLASS(Conditions);
}
void WeaponSystemExtension::OnUnloading()
{
	GameWeaponSystem.ClearAllActiveWeapons();
}
bool WeaponSystemExtension::OnGameSaving(xmlQuote& xml)
{
	GameWeaponSystem.ActiveWeapons.Save(xml,&GameWeaponSystem.ActiveWeapons);
	return true;
}
bool WeaponSystemExtension::OnGameLoading(xmlQuote& xml)
{
	OnUnloading();
	ErrorPager Error;
	GameWeaponSystem.ActiveWeapons.Load(xml,&GameWeaponSystem.ActiveWeapons,&Error,NULL);
	return true;
}
//==================================================================================================================//