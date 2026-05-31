#include "stdheader.h"
#include "../../UnitAbility.h"
#include "../../WeaponSystem.h"
#include "../../GameInterface.h"
#include <../../BE_HEADERS.h>

#ifdef __LUA__

// class C++ -> Lua //////////////////////////////////////////////////////
// binde External Classes from "GameInterface.h"
void	bind_GameInterface(lua_State* L){
	using namespace luabind;
	module(L)
	[
		class_<pUnit>("UNIT")
		.def(	constructor<>()		)
		.property(	"ID"			,	&pUnit::Get_ID	,	&pUnit::Set_ID		)
		.property(	"SN"			,	&pUnit::Get_SN							)
		.property(	"UID"			,	&pUnit::Get_UID	,	&pUnit::SetUniqID	)
		//container management
		.property(	"HIDDEN"		,	&pUnit::GetHideState	,	&pUnit::SetHideState	)
		.def(	"GoToContainer"					,	&pUnit::GoToContainer					)
		.def(	"PushOutExactUnit"				,	&pUnit::PushOutUnit00					)
		.def(	"PushOutAnyUnit"				,	&pUnit::PushOutUnit01					)
		.def(	"PushOutAllUnits"				,	&pUnit::PushOutAllUnits					)
		.def(	"GetInsideAmount"				,	&pUnit::GetInsideAmount					)
		.def(	"GetInsideUnitID"				,	&pUnit::GetInsideUnitID					)
		.def(	"InsertToContainer"				,	&pUnit::InsertToContainer				)
		.def(	"RemoveMeFromContainer"			,	&pUnit::RemoveFromContainer00			)
		.def(	"RemoveMeFromExactContainer"	,	&pUnit::RemoveFromContainer01			)
		//get state functions
		.def(	"IsBusy"						,	&pUnit::IsBusy							)
		.def(	"IsStand"						,	&pUnit::IsStand							)
		// hero managment
		.property(	"HERO"			,	&pUnit::isHero							)
		.property(	"HERO_LEVEL"	,	&pUnit::GetHeroLevel					)
		.property(	"COLECT_EXPA"	,	&pUnit::GetHeroCollectExpaStatus	,	&pUnit::SetHeroCollectExpaStatus	)
	];
};
//////////////////////////////////////////////////////////////////////////

#endif//__LUA__