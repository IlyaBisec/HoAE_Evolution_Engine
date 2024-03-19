#include "stdheader.h"
#include "../../UnitAbility.h"
#include "../../WeaponSystem.h"
#include "../../GameInterface.h"
#include <../../BE_HEADERS.h>

#ifdef __LUA__

// class C++ -> Lua //////////////////////////////////////////////////////

void	bind_be_CNODE(lua_State* L){
	using namespace luabind;
	module(L)
	[
		def(	"ShowWarning"	,	ggg_WarnigMessage	)
	];

	module(L,"CLASS")
	[
		class_<be_CNODE, be_CVariableBase>("NODE")
		// constructors
		.def(	constructor<>()						)
		.def(	constructor<int,int>()				)
		.def(	constructor<int,int,int>()			)
		.def(	constructor<int,int,int,int>()		)
		.def(	constructor<int,int,int,int,int>()	)
		// description node params
		.def(	"SetName"		,	&be_CNODE::SetName		)	// Change node name
		.def(	"AS_BASE"		, 	&be_CNODE::GetAS_BASE	)
		// property
		.property(	"x"		,	&be_CNODE::vGetX		,	&be_CNODE::vSetX		)
		.property(	"y"		,	&be_CNODE::vGetY		,	&be_CNODE::vSetY		)
		.property(	"R"		,	&be_CNODE::vGetR		,	&be_CNODE::vSetR		)
		.property(	"Dir"	,	&be_CNODE::vGetDir		,	&be_CNODE::vSetDir		)
		.property(	"SegmFR",	&be_CNODE::vGetSegmFR	,	&be_CNODE::vSetSegmFR	)
		,
		def(	"GetNode"		,		__getNodeByName			),
		def(	"GetNode"		,		__getNodeByID			)
	];
};



void	bind_be_CGROUP(lua_State* L){
	using namespace luabind;
	module(L,"CLASS")
	[
		class_< be_CGROUP, be_CVariableBase >( "GROUP" )
			// constructors
			.def(	constructor<>()														)
			.def(	constructor<const char*>()											)
			.property(		"N"				,		&be_CGROUP::GetTotalAmount			)
			.property(		"HP"			,		&be_CGROUP::GetHP			,		&be_CGROUP::SetHP			)
			.property(		"SearchEnemy"	,		&be_CGROUP::GetSearchWict	,		&be_CGROUP::SetSearchWict	)
			.property(		"UnderAttack"	,		&be_CGROUP::GetUA			,		&be_CGROUP::SetUA			)
			.property(		"NO_ORDERS_NUM"	,		&be_CGROUP::NO_ORDERS_NUM			)
			.property(		"MOVE_NUM"		,		&be_CGROUP::MOVE_NUM				)
			.property(		"ATTACK_NUM"	,		&be_CGROUP::ATTACK_NUM				)
			.property(		"Xc"			,		&be_CGROUP::GetGroupX				)
			.property(		"Yc"			,		&be_CGROUP::GetGroupY				)
			.property(		"Nat"			,		&be_CGROUP::GetNation				)
			.property(		"Ready"			,		&be_CGROUP::GetReady				)
			.property(		"NMASK"			,		&be_CGROUP::GetNMASK			,	&be_CGROUP::SetNMASK		 )
			.property(		"DestDelta"		,		&be_CGROUP::GetSendDispersion	,	&be_CGROUP::SetSendDispersion)
			.property(		"AIGuard"		,		&be_CGROUP::getAIGuard			,	&be_CGROUP::setAIGuard		 )
			.property(		"LockType"		,		&be_CGROUP::GetLockType				)
			.def(	"GetORDER"				,		&be_CGROUP::GetORDER				)
			.def(	"ClearOrders"			,		&be_CGROUP::ClearOrders				)
			.def(	"GetUnitORDER"			,		&be_CGROUP::GetUnitORDER			)
			.def(	"RemoveDeadUnits"		,		&be_CGROUP::RemoveDeadUnits			)
			.def(	"RemAllUnits"			,		&be_CGROUP::RemAllUnits				)
			.def(	"RemUnitsByNation"		,		&be_CGROUP::RemUnitsByNation		)
			.def(	"RemoveNUnitsToCGroup"	,		&be_CGROUP::RemoveNUnitsToCGroup	)
			.def(	"RemoveUnitsInZoneToCGroup",	&be_CGROUP::RemoveUnitsInZoneToCGroup )
			.def(	"AddSelectedUnits"		,		&be_CGROUP::AddSelectedUnits		)
			.def(	"AddUnitsInZone"		,		&be_CGROUP::AddUnitsInZone			)
			.def(	"AddUnitsTypeInZone"	,		&be_CGROUP::AddUnitsTypeInZone		)
			.def(	"AddEnemyUnitsInZone"	,		&be_CGROUP::AddEnemyUnitsInZone		)
			.def(	"AddUnitByGID"			,		&be_CGROUP::AddUnitGID				)
			.def(	"GetName"				,		&be_CGROUP::GetName					)
			.def(	"KillUnits"				,		&be_CGROUP::KillUnits				)
			.def(	"EraseUnits"			,		&be_CGROUP::EraseUnits				)
			.def(	"SetNation"				,		&be_CGROUP::SetNation				)
			.def(	"SetLockType"			,		&be_CGROUP::SetLockType				)
			.def(	"SendTo"				,		&be_CGROUP::SendTo					)	
			.def(	"SendToI"				,		&be_CGROUP::SendToI					)
			.def(	"SendToPosition"		,		&be_CGROUP::SendToPosition			)
			.def(	"SendToDispersePosition",		&be_CGROUP::SendToDispersePosition	)
			.def(	"MarkTime"				,		&be_CGROUP::MarkTime				)
			.def(	"SendSmartlyTo"			,		&be_CGROUP::SendSmartlyTo			)
			.def(	"ChangeDirection"		,		&be_CGROUP::ChangeDirection			)	
			.def(	"SetAgresiveST"			,		&be_CGROUP::SetAgresiveST			)
			.def(	"SetInStandGround"		,		&be_CGROUP::SetInStandGround		)
			.def(	"SetStandGround"		,		&be_CGROUP::SetStandGround			)
			.def(	"ChengeFormation"		,		&be_CGROUP::ChengeFormation			)
			.def(	"SelectUnits"			,		&be_CGROUP::SelectUnits				)
			.def(	"SelectUnitsInZone"		,		&be_CGROUP::SelectUnitsInZone		)
			.def(	"TakeRess"				,		&be_CGROUP::TakeRess				)
			.def(	"TakeFood"				,		&be_CGROUP::TakeFood				)
			.def(	"TakeWood"				,		&be_CGROUP::TakeWood				)
			.def(	"TakeStone"				,		&be_CGROUP::TakeStone				)
			.def(	"UnitNumByType"			,		&be_CGROUP::GetTotalAmount2			)
			.def(	"UnitNumTotal"			,		&be_CGROUP::GetTotalAmount			)
			.def(	"UnitNumInZone"			,		&be_CGROUP::GetAmountInZone			)
			.def(	"GetGroupCenter"		,		&be_CGROUP::GetGroupCenter			)
			.def(	"GetGroupX"				,		&be_CGROUP::GetGroupX				)
			.def(	"GetGroupY"				,		&be_CGROUP::GetGroupY				)
			.def(	"GetXi"					,		&be_CGROUP::GetXi					)
			.def(	"GetYi"					,		&be_CGROUP::GetYi					)
			.def(	"GetGID"				,		&be_CGROUP::GetGID					)
			.def(	"GetDIRi"				,		&be_CGROUP::GetDIRi					)
			.def(	"GetGIDi"				,		&be_CGROUP::GetGIDi					)
			.def(	"GetDirection"			,		&be_CGROUP::GetDirection			)
			.def(	"GetNation"				,		&be_CGROUP::GetNation				)
			.def(	"GetNationAmout"		,		&be_CGROUP::GetNationAmout			)
			.def(	"GetAgresiveState"		,		&be_CGROUP::GetAgresiveState		)
			.def(	"GetBrigadeList"		,		&be_CGROUP::GetBrigadeList			)
			.def(	"GetAmountOfNewUnits"	,		&be_CGROUP::GetAmountOfNewUnits		)
			.def(	"GetAmountOfRemUnits"	,		&be_CGROUP::GetAmountOfRemUnits		)
			.def(	"GetIsTired"			,		&be_CGROUP::GetIsTired				)
			.def(	"GetNInside"			,		&be_CGROUP::GetNInside				)
			.def(	"GetLeaveAbility"		,		&be_CGROUP::GetLeaveAbility			)
			.def(	"GetNofBRLoadedGun"		,		&be_CGROUP::GetNofBRLoadedGun		)
			.def(	"GetEnemyUnitsAroundN"	,		&be_CGROUP::GetEnemyUnitsAroundNumber	)
			.def(	"ChekPosition"			,		&be_CGROUP::ChekPosition			)
			.def(	"CheckAttackPossibilityG"	,	&be_CGROUP::CheckAttackPossibilityG		)
			.def(	"CheckAttackPossibilityXYR"	,	&be_CGROUP::CheckAttackPossibilityXYR	)
			.def(	"AttackGroup_NearestOB"	,		&be_CGROUP::AttackGroup_NearestOB	)
			.def(	"AttackEnemy00"			,		&be_CGROUP::AttackEnemy00			)
			.def(	"IsGRPAttackMe"			,		&be_CGROUP::IsGRPAttackMe			)
			.def(	"Produce"				,		&be_CGROUP::Produse00				)
			.def(	"SetRallyPoint"			,		&be_CGROUP::SetRallyPoint			)	
			.def(	"ExploringMAP"			,		&be_CGROUP::ExploringMAP			)
			.def(	"DeleteItemDrops"		,		&be_CGROUP::DeleteItemDrops			)
			.def(	"idAttackEnemy"			,		&be_CGROUP::idAttackEnemy			)
			.def(	"idSendTo"				,		&be_CGROUP::idSendTo				)
			.def(	"SetAIGuard"			,		&be_CGROUP::SetAIGuard				)
			.def(	"GiveExperience"		,		&be_CGROUP::GiveExpa				)
			.def(	"FillTN_array"			,		&be_CGROUP::FillTN_array			)
			.def(	"FillDIFF_TN_array"		,		&be_CGROUP::FillDIFF_TN_array		)
			.def(	"DisableCapturability"	,		&be_CGROUP::DisableCapturability	)
			.enum_("UnitState")
			[
				value(	"_NO_ORDERS"	,	0	),
				value(	"_MOVE"			,	1	),
				value(	"_ATTACK"		,	2	),
				value(	"_SOME_ORDER"	,	4	),
				value(	"_BRIG_ORDER"	,	8	)
			]
	//		,
	//		def(	"GetGroup"				,	( be_CGROUP* (*) (const char*) )	__getGrpByName	),
	//		def(	"GetGroup"				,	( be_CGROUP* (*) (int) )			__getGrpByID	)
	];
};



void	bind_groupMAP(lua_State* L){
	using namespace luabind;
/*	module(L)
	[
		def(	"GetGroupByName"		,	( be_CGROUP* (*) (const char*) )	__getGrpByName	)
	];*/
};

void	bind_valuesMAP(lua_State* L){
	using namespace luabind;
	module(L,"CLASS")
	[
		class_< be_CVariableBase >( "VariableBase" )
			.def(	"SetRandomPower"	, be_CVariableBase::SetRandomApplyPower )
			.def(	"AS_BASE"			, be_CVariableBase::GetAS_BASE			)
			.property(	"Name"			,	&be_CVariableBase::GetName	,	&be_CVariableBase::SetName_LUA	)
		,
		class_< be_CINT	, be_CVariableBase >( "INT" )
			.def(	constructor<>()						)
			.def(	constructor<const char*>()			)
			.property(	"Value"	,	&be_CINT::GetValue	,	&be_CINT::SetValue	)
		,
		class_< be_CBOOL , be_CVariableBase >( "BOOL" )
			.def(	constructor<>()						)
			.def(	constructor<const char*>()			)
			.property(	"Value"	,	&be_CBOOL::GetValue	,	&be_CBOOL::SetValue	)
		,
		class_< be_CDWORD	, be_CVariableBase >( "DWORD" )
			.def(	constructor<>()						)
			.def(	constructor<const char*>()			)
			.property(	"Value"	,	&be_CDWORD::GetValue,	&be_CDWORD::SetValue)
		,
		class_< be_CFLOAT	, be_CVariableBase >( "FLOAT" )
			.def(	constructor<>()						)
			.def(	constructor<const char*>()			)
			.property(	"Value"	,	&be_CFLOAT::GetValue,	&be_CFLOAT::SetValue)
		,
		class_< be_CSTR	, be_CVariableBase >( "STR" )
			.def(	constructor<>()						)
			.def(	constructor<const char*>()			)
			.property(	"Value"	,	&be_CSTR::GetValue,	&be_CSTR::SetValue	)
		,
		class_< be_CSTR_DIALOG, be_CVariableBase>( "STR_DIALOG" )
			.def(	constructor<>()						)
			.def(	constructor<const char*>()			)
			.property(	"ogFile",	&be_CSTR_DIALOG::Get_ogFile				)
			.property(	"Group"	,	&be_CSTR_DIALOG::Get_Group				)
		,
		class_< be_CARRAY, be_CVariableBase>( "ARRAY" )
			.def(	constructor<>()						)
			.def(	constructor<const char*>()			)
			.def(	"ADD"			,	&be_CARRAY::AddValue	)
			.def(	"DEL"			,	&be_CARRAY::DelValue	)
			.def(	"SIZE"			,	&be_CARRAY::GetAmount	)
			.def(	"GET"			,	&be_CARRAY::GetValue	)
			.def(	"strGET"		,	&be_CARRAY::strGetValue	)
			.def(	"GetParmList"	,	&be_CARRAY::GetParmList	)
		,
		class_< be_CMSG >( "MSG" )
			.property( "str"			,	&be_CMSG::GetMSGName,	&be_CMSG::SetMSGName )
			.property( "sender"			,	&be_CMSG::GetSender		)
			.property( "receiver"		,	&be_CMSG::GetReceiver	)
			.def(	"GET_PARAMS"	,	&be_CMSG::GET_PARAMS	)
		,
		class_< be_CSMARTOBJECT, be_CVariableBase >( "SMARTOBJ" )
			.property(	"next_STATE"	,	&be_CSMARTOBJECT::GetNextState	, &be_CSMARTOBJECT::SetNextState	)
			.property(	"STATE"			,	&be_CSMARTOBJECT::GetState		, &be_CSMARTOBJECT::SetState		)
			.property(	"FSC"			,	&be_CSMARTOBJECT::GetForceSTST	, &be_CSMARTOBJECT::SetForceSTST	)
			.def(	"GET_OBJ"		,	&be_CSMARTOBJECT::GET_OBJECT	)
			.def(	"SET_OBJ"		,	&be_CSMARTOBJECT::SET_OBJECT	)
			.def(	"GET_PARAMS"	,	&be_CSMARTOBJECT::GET_PARAMS	)
		,
		class_< UnitAbility, be_CVariableBase >( "ABILITY" )
		.enum_("ABIL")
			[
				value(	"ABSENT"	,	0	),
				value(	"ACTIVE"	,	1	),
				value(	"PASIVE"	,	2	),
				value(	"HIDE"		,	4	)
			]
		.enum_("EXECABIL")
			[
				value(	"Casting"		,	1	),
				value(	"CanNotTarget"	,	2	),
				value(	"WaitForCast"	,	4	),
				value(	"AnotherCasting",	8	),
				value(	"Absent"		,	0	)
			]
		,
		class_< WeaponModificator, be_CVariableBase >( "WEAPONMOD" )
/*
		class_<	vvTEXT			,	vvBASE	>	(	"vvTEXT"			)
		.property(	"TextID"	,	&vvTEXT::Get_TextID		,		&vvTEXT::Set_TextID		)
		.property(	"oggFile"	,	&vvTEXT::Get_oggFile	,		&vvTEXT::Set_oggFile	)
		.property(	"SpeakerID"	,	&vvTEXT::Get_SpeakerID	,		&vvTEXT::Set_SpeakerID	)
		,
		class_<	vvPICTURE		,	vvBASE	>	(	"vvPICTURE"			)
		.property(	"SpriteNUM"	,	&vvPICTURE::GetSpriteNUM								)
		.property(	"SpriteID"	,	&vvPICTURE::GetSpriteID	,		&vvPICTURE::SetSpriteID	)
		.def_readonly(	"FileID"	,	&vvPICTURE::FileID	)
		.def_readwrite(	"dx"		,	&vvPICTURE::dx		)
		.def_readwrite(	"dy"		,	&vvPICTURE::dy		)
		.def_readwrite(	"lx"		,	&vvPICTURE::lx		)
		.def_readwrite(	"ly"		,	&vvPICTURE::ly		)
		,
		class_<	vvPOINT2D		,	vvBASE	>	(	"vvPOINT2D"			)
		.property(	"x"			,	&vvPOINT2D::GetX		,		&vvPOINT2D::SetX		)
		.property(	"y"			,	&vvPOINT2D::GetY		,		&vvPOINT2D::SetY		)
		,
		class_<	vvPOINT_SET		,	vvBASE	>	(	"vvPOINT_SET"		)
		.def(	"Clear"				,	&vvPOINT_SET::CleanARR			)
		.def(	"Add"				,	&vvPOINT_SET::AddPoint			)
		.property(	"N"			,	&vvPOINT_SET::GetNUM									)
		.def(	"GetPoint"			,	&vvPOINT_SET::Get_vvPOINT2D		)
		,
		class_<	vvVector3D		,	vvBASE	>	(	"vvVector3D"		)
		.def_readwrite(	"x"			,	&vvVector3D::x		)
		.def_readwrite(	"y"			,	&vvVector3D::y		)
		.def_readwrite(	"z"			,	&vvVector3D::z		)
		.def_readwrite(	"mX"		,	&vvVector3D::mX		)
		.def_readwrite(	"mY"		,	&vvVector3D::mY		)
		,
		class_<	vvDIALOG		,	vvBASE	>	(	"vvDIALOG"			)
		,
		class_<	vvMissionLOG	,	vvBASE	>	(	"vvMissionLOG"		)
		.def(	"Clear"				,	&vvMissionLOG::Clear			)
		.def(	"SetQuestData"		,	&vvMissionLOG::SetQuestData		)
		.def(	"SetKilsData"		,	&vvMissionLOG::SetKilsData		)
		.def(	"SetTimeData"		,	&vvMissionLOG::SetTimeData		)
		.def(	"AddCopmleteQuest"	,	&vvMissionLOG::AddCopmleteQuest	)
		.def(	"AddKillsCopmlete"	,	&vvMissionLOG::AddKillsCopmlete	)
		.def(	"AddTimeCopmlete"	,	&vvMissionLOG::AddTimeCopmlete	)
		.def(	"WriteToLogClass"	,	&vvMissionLOG::WriteToLogClass	)
		,
		class_<	vvMESSGES		,	vvBASE	>	(	"vvMESSGES"			)
		,
		class_<	vvMISSMGR		,	vvBASE	>	(	"vvMISSMGR"			)
		.def(	"PAUSE"				,	&vvMISSMGR::STPS_ENABLED		)
		.def(	"RESET"				,	&vvMISSMGR::REST_ENABLED		)
		.def(	"NEXT"				,	&vvMISSMGR::NEXT_ENABLED		)
		,
		class_<	vvBrigAI		,	vvBASE	>	(	"vvBrigAI"			)
		,
		class_<	vvTASKS_CII		,	vvBASE	>	(	"vvTASKS_CII"		)
		.def(	"AddTASK"			, &vvTASKS_CII::addLT_TASK_lua		)
		.def(	"AddHINT"			, &vvTASKS_CII::addLT_HINT_lua		)
		.def(	"AddELSE"			, &vvTASKS_CII::addLT_ELSE_lua		)
		.def(	"DelTASK"			, &vvTASKS_CII::delLT_TASK_lua		)
		.def(	"DelHINT"			, &vvTASKS_CII::delLT_HINT_lua		)
		.def(	"DelELSE"			, &vvTASKS_CII::delLT_ELSE_lua		)
		.def(	"SetTASK_compl"		, &vvTASKS_CII::setLT_TASK_COMPLITE	)
		,
		class_<	lvCTeraforming	,	vvBASE	>	(	"lvCTeraforming"	)
		,
		def(	"get_vv"	,	__getValByName<vvBASE>			),
		def(	"get_tg"	,	__getValByName<be_CBOOL>		),
		def(	"get_wd"	,	__getValByName<be_CDWORD>			),
		def(	"get_in"	,	__getValByName<be_CINT>		),
		def(	"get_tx"	,	__getValByName<vvTEXT>			),
		def(	"get_pc"	,	__getValByName<vvPICTURE>		),
		def(	"get_p2"	,	__getValByName<vvPOINT2D>		),
		def(	"get_ps"	,	__getValByName<vvPOINT_SET>		),
		def(	"get_v3"	,	__getValByName<vvVector3D>		),
		def(	"get_dl"	,	__getValByName<vvDIALOG>		),
		def(	"get_ml"	,	__getValByName<vvMissionLOG>	),
		def(	"get_ms"	,	__getValByName<vvMESSGES>		),
		def(	"get_mm"	,	__getValByName<vvMISSMGR>		),
		def(	"get_bi"	,	__getValByName<vvBrigAI>		),
		def(	"get_tc"	,	__getValByName<vvTASKS_CII>		),
		def(	"get_tr"	,	__getValByName<lvCTeraforming>	),
		def(	"get_fz"	,	__getValByName<vvFuzzyRule>		)
		,
		class_<	vvFuzzyRule		,	vvBASE	>	(	"vvFuzzyRule"	)
		.def(	"getDegree"	,	&vvFuzzyRule::IsTrueToWhatDegree	)
		.def(	"getName"	,	&vvFuzzyRule::GetName				)
		,
		def(	"FuzzyAND"	,	__FuzzyAND							)*/
	];
};

void	bind_GraphObjMAP(lua_State* L){
	using namespace luabind;
/*	module(L,"GRAPH")
	[
		class_<	lvCGraphObject								>	(	"lvCGraphObject"		)
		.def(	"isVissible"		, &lvCGraphObject::isVissible	)
		.def(	"isInVisible"		, &lvCGraphObject::isInVisible	)
		,
		class_<	lvCDialogBased			,	lvCGraphObject	>	(	"lvCDialogBased"		)
		,
		class_<	lvCBlackScreen			,	lvCGraphObject	>	(	"lvCBlackScreen"		)
		.def(	"isVissible"		, &lvCBlackScreen::isVissible	)
		.def(	"isInVisible"		, &lvCBlackScreen::isInVisible	)
		,
		class_<	lvCMoveGP				,	lvCGraphObject	>	(	"lvCMoveGP"				)
		.def(	"isVissible"		, &lvCMoveGP::isVissible		)
		.def(	"isInVisible"		, &lvCMoveGP::isInVisible		)
		,
		class_<	lvCAAppearGP			,	lvCBlackScreen	>	(	"lvCAAppearGP"			)
		,
		class_<	lvCAnimateGP			,	lvCAAppearGP	>	(	"lvCAnimateGP"			)
		,
		class_<	lvCDeffFilmMenu			,	lvCGraphObject	>	(	"lvCDeffFilmMenu"		)
		.def(	"isVissible"		, &lvCDeffFilmMenu::isVissible	)
		.def(	"isInVisible"		, &lvCDeffFilmMenu::isInVisible	)
		,
		class_<	lvCDeffAnimeFilmMenu	,	lvCGraphObject	>	(	"lvCDeffAnimeFilmMenu"	)
		,
		def(	"GetGraph"				,	__getGraphByName		)
	];*/
};

void	bind_be_CLUA_FN_STORE(lua_State* L){
	using namespace luabind;
	module(L)
	[
		def(	"BOOL"		,	&g_beConvBOOL		),
		def(	"DWORD"		,	&g_beConvDWORD		),
		def(	"INT"		,	&g_beConvINT		),
		def(	"FLOAT"		,	&g_beConvFLOAT		),
		def(	"GROUP"		,	&g_beConvGROUP		),
		def(	"NODE"		,	&g_beConvNODE		),
		def(	"STR"		,	&g_beConvSTR		),
		def(	"ARRAY"		,	&g_beConvARRAY		),
		def(	"STR_DIALOG",	&g_beConvSTR_DIALOG	),
		def(	"ABILITY"	,	&g_beConvABIL		),
		def(	"WEAPONMOD"	,	&g_beConvWEAPONMOD	)
	];

	module(L)
	[
		def(	"ADD_FN"		,	( void (*) (const char*) )							g_be_LUA_ADD_FN			),
		def(	"ADD_FN_PARAM"	,	( void (*) (const char*,const char*,const char*) )	g_be_LUA_ADD_FN_PARAM	),

		class_<be_CParamList>("PARAM_LIST")
		.def(	constructor<>()			  )
		.def(	"GET"				,	&be_CParamList::GetParam			)
		.def(	"SET"				,	&be_CParamList::SetParam			)
		.def(	"CLEAR"				,	&be_CParamList::CLEAR				)
		.def(   "CHECK"				,	&be_CParamList::CheckParam			)
		.def(	"CreateParam_GROUP"	,	&be_CParamList::CreateParam_GROUP	)
		.def(	"CreateParam_NODE"	,	&be_CParamList::CreateParam_NODE	)
		.def(	"CreateParam_FLOAT"	,	&be_CParamList::CreateParam_FLOAT	)
		.def(	"CreateParam_STR"	,	&be_CParamList::CreateParam_STR		)
		.def(	"CreateParam_INT"	,	&be_CParamList::CreateParam_INT		)
		.def(	"CreateParam_DWORD"	,	&be_CParamList::CreateParam_DWORD	)
		.def(	"CreateParam_BOOL"	,	&be_CParamList::CreateParam_BOOL	)
		.def(	"CreateParam_ARRAY"	,	&be_CParamList::CreateParam_ARRAY	)
		.def(	"CreateParam_ABILITY",	&be_CParamList::CreateParam_UintAbil)
		.def(	"CreateParam_WEAPONMOD",&be_CParamList::CreateParam_WeaponMod)
	];
};
void	bind_be_CHashTopMAP(lua_State* L){
	using namespace luabind;
	module(L)
	[
		class_<be_CAreaListPowerData>("AREA_LIST_POWER_DATA")
		.def(	constructor<>()				)
		.def(	constructor<int,int>()		)
        .property(	"AN"		,	&be_CAreaListPowerData::getAreaNumber			)
		.def(		"getAreaID"	,	&be_CAreaListPowerData::getArea					)
		.property(	"MAP"		,	&be_CAreaListPowerData::getMeleeAttackPOWER		)
		.property(	"MUN"		,	&be_CAreaListPowerData::getMeleeUnitsN			)
		.property(	"RAP"		,	&be_CAreaListPowerData::getRangeAttackPOWER		)
		.property(	"RUN"		,	&be_CAreaListPowerData::getRangeUnitsN			)
		.property(	"UDP"		,	&be_CAreaListPowerData::getUnitsDefencePOWER	)
		.property(	"BAP"		,	&be_CAreaListPowerData::getBuildingAttackPOWER	)
		.property(	"BN"		,	&be_CAreaListPowerData::getBuildingsN			)
		.property(	"BDP"		,	&be_CAreaListPowerData::getBuildingDefencePOWER	)
		.def(		"create"	,	&be_CAreaListPowerData::create					)
		.property(	"minAST_AID",	&be_CAreaListPowerData::getWeakestAST_AID		)
		,
		class_<be_CTotalALPD>("TOTAL_ALPD")
		.def(	constructor<>()				)
		.def(	constructor<int,int,int>()	)
		.property(	"N"			,	&be_CTotalALPD::getN							)
		.def(		"getALPD"	,	&be_CTotalALPD::getALPD							)
		.def(		"create"	,	&be_CTotalALPD::create							)
	];
};
//////////////////////////////////////////////////////////////////////////

#endif//__LUA__







































