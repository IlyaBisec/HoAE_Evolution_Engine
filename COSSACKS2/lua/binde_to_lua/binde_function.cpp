#include "stdheader.h"
#include "../../UnitAbility.h"
#include <../../BE_HEADERS.h>

// functions C++ -> Lua //////////////////////////////////////////////////

// functions from "ActiveScenary.cpp"
DLLEXPORT	byte	Trigg			(byte ID);
DLLEXPORT	void	SetTrigg		(byte ID,byte Val);
DLLEXPORT	word	WTrigg			(byte ID);
DLLEXPORT	void	SetWTrigg		(byte ID,word Val);
			void	RunTimer		(byte ID, int Long, bool trueTime);
DLLEXPORT	void	RunTimer		(byte ID,int Long);
DLLEXPORT	bool	TimerDone		(byte ID);
DLLEXPORT	bool	TimerDoneFirst	(byte ID);
DLLEXPORT	bool	TimerIsEmpty	(byte ID);
DLLEXPORT	void	FreeTimer		(byte ID);
DLLEXPORT	int		GetTime			(byte ID);
DLLEXPORT	int		GetGlobalTime	();
DLLEXPORT	int		GetAnimTime		();
DLLEXPORT	bool	NationIsErased	(byte Nat);
DLLEXPORT	void	ClearSelection	(byte Nat);
// COSS 2
CEXPORT		int		vdf_GetAmountOfSettlements(byte Owner,	int ResType, bool CheckUpgrades, int Level); 
DLLEXPORT	int		GetBrigadsAmount0(byte NI);
// SPESIAL
void AssignHint1(char* s,int time);
void AssignHint1_lua(const char* s,int time){
	AssignHint1((char*)s,time);
};
char* GetTextByID(char* id);
char* GetTextByID_lua(const char* id){
	return	GetTextByID((char*)id);
};
int	  getRando(int _max){
	if ( _max<=0 ) return rando();
	return rando()%_max;
};

bool	lua_binaryAND( DWORD a, DWORD b ){
	return (bool)( a & b );
};
bool	lua_binaryOR( DWORD a, DWORD b ){
	return (bool)( a | b );
};

bool GetInsideDialogState();
void SetInsideDialogState(bool State);

bool	lua_module(const char* fileName);
int		GetGlobalTime_lua(){
	return GetGlobalTime();	
};

#ifdef __LUA__
// base lua function in defoult lib
void	lua_base(lua_State* L){
	lua_baselibopen(L);
	lua_tablibopen(L);
	lua_iolibopen(L);
	lua_strlibopen(L);
	lua_mathlibopen(L);
	lua_dblibopen(L);
};

void	bind_ActiveScenary(lua_State* L){
	using namespace luabind;
	module(L)
	[
		def(	"lua_module"		,	(bool(*)(const char*))		lua_module		),

		def(	"Trigg"				,	Trigg										),
		def(	"SetTrigg"			,	SetTrigg									),
		def(	"WTrigg"			,	WTrigg										),
		def(	"SetWTrigg"			,	SetWTrigg									),
		def(	"Rando"				,	getRando									),

		def(	"RunTimer"			,	(void(*)(byte,int,bool))	RunTimer		),
		def(	"RunTimer"			,	(void(*)(byte,int))			RunTimer		),
		def(	"TimerDone"			,								TimerDone		),
		def(	"TimerDoneFirst"	,								TimerDoneFirst	),
		def(	"TimerIsEmpty"		,								TimerIsEmpty	),
		def(	"FreeTimer"			,								FreeTimer		),
		def(	"GetTime"			,								GetTime			),
		def(	"GetGlobalTime"		,								GetGlobalTime	),
		def(	"GetAnimTime"		,								GetAnimTime		),

		def(	"NationIsErased"	,								NationIsErased	),
		def(	"vdf_GetAmountOfSettlements"	,		vdf_GetAmountOfSettlements	),
		def(	"GetBrigadsAmount0"	,							 GetBrigadsAmount0	),

		def(	"AddFirers"			,								AddFirers		),
		def(	"AddTomahawks"		,								AddTomahawks	),
		def(	"AddPsKillers"		,								AddPsKillers	),
		def(	"AddStorm"			,								AddStorm		),

		def(	"be_gShowCloudDialog"		, (void(*)(be_CSTR_DIALOG*))	be_gShowCloudDialog				),
		def(	"GetLineCountInCloudDialog"	, (int (*)(be_CSTR_DIALOG*))	be_gGetLineCountInCloudDialog	),
		def(	"GetCloudDilogCancel"	,					be_gGetCloudDilogCancel	),
		def(	"SetCloudDilogCancel"	,					be_gSetCloudDilogCancel	),
		def(	"ClearCloudDilogSPEAKERS"	,			be_gClearCloudDilogSPEAKERS	),
		def(	"AddCloudDilogSPEAKERS"		,			be_gAddCloudDilogSPEAKERS	),

		def(	"FilePresent"		,	(bool(*)(const char*))be_gFilePresent		),

		def(	"AssignHint"		,	(void(*)(const char*,int))	AssignHint1_lua	),
		def(	"GetTextByID"		,	(char*(*)(const char*))		GetTextByID_lua	),
		def(	"PrepareString"		,	(bool(*)(be_CSTR*,be_CSTR*,be_CARRAY*))be_gPrepareString	),

		def(	"ActivateAlert"		,					be_gAddAlertOnMap			),

		def(	"ChengeNMaskInNode"	,					ChengeNMaskInNode_lua		),
		def(	"GetAreaX"			,					be_gGetAreaX				),
		def(	"GetAreaY"			,					be_gGetAreaY				),

		def(	"bAND"				,					lua_binaryAND				),
		def(	"bOR"				,					lua_binaryOR				),

		def(	"GetInsideDialogState"	,				GetInsideDialogState		),
		def(	"SetInsideDialogState"	,				SetInsideDialogState		)
	];
};

// function from Condition List
void	bind_Condition(lua_State* L){
	using namespace luabind;
	module(L,"GET")
	[
		def(	"UnitsAmount"			,						GetUnitsAmount0_lua		),
		def(	"EnemyUnitsAmount"		,						GetEnemyUnitsAmount_lua	),
		def(	"GetEnemyBuildindsAmount"	,				GetEnemyBuildindsAmount_lua	),

		def(	"GetAmount_lua"			,						GetAmount_lua			),
        def(	"GetUnitsAmount2_lua"	,						GetUnitsAmount2_lua		),
		def(	"GetTotalAmount1_lua"	,						GetTotalAmount1_lua		),
		def(	"GetReadyAmount_lua"	,						GetReadyAmount_lua		),
		def(	"GetResource_lua"		,						GetResource_lua			),
		def(	"GetDiff_lua"			,						GetDiff_lua				),
		def(	"Trigg_lua"				,						Trigg_lua				),
		def(	"ogSTOP_lua"			,						ogSTOP_lua				),
		def(	"CameraSTOP_lua"		,						CameraSTOP_lua			),
		def(	"GetLMode_lua"			,						GetLMode_lua			),
		def(	"CheckButton_lua"		,						CheckButton_lua			),
		def(	"GetFormationType_lua"	,						GetFormationType_lua	),
		def(	"TestFillingAbility_lua",						TestFillingAbility_lua	),
		def(	"CInStandGround_lua"	,						CInStandGround_lua		),
		def(	"VillageOwner_lua"		,						VillageOwner_lua		),
		def(	"GetNofBrigInNode_lua"	,						GetNofBrigInNode_lua	),
		def(	"LoadingCoplite_lua"	,						LoadingCoplite_lua		),
		def(	"GetTime"				,						GetTrueTime				),
		def(	"GetNation_lua"			,						GetNation_lua			),
		// Ability Section
		def(	"GetAblOBNation"		,					GetAbilityOwnerNation_lua	),
		def(	"GetAbilityOwnerObject"	,					GetAbilityOwnerObject_lua	),
		def(	"GetAbilityActivatorObject" ,			GetAbilityActivatorObject_lua	),
		// Nation
		def(	"GetNKilled"			,						be_gGetNKilled			)
	];
};

// function from Operation List
void	bind_Operation(lua_State* L){
	using namespace luabind;    
	module(L,"OPER")
	[
		def(	"SelectAll_lua"			,						SelectAll_lua			),
		def(	"SelectUnits_lua"		,						SelectUnits_lua			),
		def(	"SelectUnitsType_lua"	,						SelectUnitsType_lua		),
		def(	"ClearSelection_lua"	,						ClearSelection			),
		def(	"ChangeAS_lua"			,						ChangeAS_lua			),
		def(	"SelSendTo_lua"			,						SelSendTo_lua			),
		def(	"ChangeFriends_lua"		,						ChangeFriends_lua		),
		def(	"SetFriends_lua"		,						SetFriends_lua			),
		def(	"SetLightSpot_lua"		,						SetLightSpot_lua		),
		def(	"ClearLightSpot_lua"	,						ClearLightSpot_lua		),
		def(	"SetStartPoint_lua"		,						SetStartPoint_lua		),
		def(	"ShowVictory_lua"		,						ShowVictory_lua			),
		def(	"LooseGame_lua"			,						LooseGame_lua			),
		def(	"SavePosition_lua"		,						SavePosition_lua		),
		def(	"SavePositionArr_lua"	,						SavePositionArr_lua		),
		def(	"SetResource_lua"		,						SetResource_lua			),
		def(	"AddRessource_lua"		,						AddRessource_lua		),
		def(	"ActivateTacticalAI_lua",						ActivateTacticalAI_lua	),
		def(	"StartAIEx_lua"			,						StartAIEx_lua			),
		def(	"SetAIEnableState_lua"	,						SetAIEnableState_lua	),
		def(	"ShowDialog_lua"		,						ShowDialog_lua			),
		def(	"AddTextToDlg_lua"		,						AddTextToDlg_lua		),
		def(	"ClearDialog_lua"		,						ClearDialog_lua			),
		def(	"SetScrollLimit_lua"	,						SetScrollLimit_lua		),
		def(	"GetUTypeByName_lua"	,						GetUTypeByName_lua		),
		def(	"PutNewSquad_lua"		,						PutNewSquad_lua			),
		def(	"GetFormationID_lua"	,						GetFormationID_lua		),
		def(	"PutNewFormation_lua"	,						PutNewFormation_lua		),
		def(	"PutRandFormation_lua"	,						PutRandFormation_lua	),
		def(	"SetUnitStateCII_lua"	,						SetUnitStateCII_lua		),
		def(	"GroupHoldPOS_AI_lua"	,						GroupHoldPOS_AI_lua		),
		def(	"SetTired_lua"			,						SetTired_lua			),
		def(	"KillNatinInPOS_lua"	,						KillNatinInPOS_lua		),
		def(	"TeleportGroup"			,						TeleportGroup_lua		),
		// Ability Section
		def(	"AddCarryAbility"		,						AddCarryAbility_lua		),
		def(	"DetectCarryAbility"	,						DetectCarryAbility_lua	),
		def(	"DeleteCarryAbility_ptr",(bool(*)(int,UnitAbility*))DeleteCarryAbility_lua ),
		def(	"DeleteCarryAbility_str",(bool(*)(int,const char* ))DeleteCarryAbility_lua ),
		def(	"SetAblState"			,						SetAblState_lua			),
		def(	"GetAblState"			,						GetAblState_lua			),
		def(	"GetAblExecuteState"	,						GetAblExecuteState_lua	),
		def(	"AblExecute"			,						AblExecute_lua			),

		def(	"CreateNewActiveWeapon"	,					CreateNewActiveWeapon_lua	),

		def(	"DisableCapturability"	,						DisableCapturability_lua),
		def(	"SetAddFarms"			,						SetAddFarms_lua			)

	];
	module(L,"FSM")
	[
		def(	"SendMsg"				,						g_beSendMsg				),
		def(	"SendMsg"				,						g_beSendDelayedMsg		),

		def(	"FreeSO_SONane"			,(bool(*)(const char*)		)		be_gFreeSO_SOName	),
		def(	"FreeSO_pVAR"			,(bool(*)(be_CVariableBase*))		be_gFreeSO_pVAR		),

		def(	"GET_SO"				,(be_CSMARTOBJECT*(*)(const char*))	be_gGET_SO			),
		def(	"FindSO"				,(be_CSMARTOBJECT*(*)(const char*))	be_gFindSO			),
		def(	"FindSObyOBName"		,(be_CSMARTOBJECT*(*)(const char*)) be_gFindSObyOBName	),
		def(	"FindSObyName"			,(be_CSMARTOBJECT*(*)(const char*))	be_gFindSObyName	),
        def(	"CheckObjectFSM"		,(bool(*)(const char*,const char*))	be_gCheckObjectFSM	)
	];
	module(L,"SOUND")
	[
		def(	"Play"				,(void(*)(const char*,int))	ov_Play_lua				),
		def(	"Stop"				,(void(*)(int))				ov_Stop_lua				),
		def(	"SetVolume"			,(void(*)(int,int))			ov_SetVolume_lua		),
		def(	"Cyclic"			,(void(*)(bool,int))		ov_Cyclic_lua			),
		def(	"StreamFinished"	,(bool(*)(int))				ov_StreamFinished_lua	)
	];
};

//////////////////////////////////////////////////////////////////////////

#endif//__LUA__






































