#ifndef	__ENUM__STORAGE__ 
#define	__ENUM__STORAGE__

void	g_beREG_ALL_BE_ENUM();

// ENUMERATORS MAKROSS ///////////////////////////////////////////////////
DWORD	ADD_BE_PARAMS_TO_ENUM(	RefreshableEnumeratorContext*	in,
								EnumeratorOutputStream*			out,
								DWORD							PrevCallHash,
								const char*						ParTypeName
							 );

DWORD	ADD_BE_OBJECT_TO_ENUM(	RefreshableEnumeratorContext*	in,
								EnumeratorOutputStream*			out,
								DWORD							PrevCallHash,
								ClassArray<ReferableBaseClass>* pObjArray,
								const char*						ClassName
							 );

DWORD	VAR_BE_HALPER_TO_ENUM(	RefreshableEnumeratorContext*	in,
								EnumeratorOutputStream*			out,
								DWORD							PrevCallHash,
								const char*						ClassName
							 );
//////////////////////////////////////////////////////////////////////////

// GLOBAL VALUES UNUMERATORS /////////////////////////////////////////////

// be_CVariableBase - store all global variables on map
DWORD	fn_BE_VALUES_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										);

//--------------- MAP OBJECTS -------------------//
// be_CNODE
DWORD	fn_BE_NODES_ON_MAP_ENUM			(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

// Nodes Type 
DWORD	fn_NODE_TYPE_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

// be_CGROUP
DWORD	fn_BE_GROUPS_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

// be_CCAMERA
DWORD	fn_BE_CAMERA_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//--------------- VARIABLES ---------------------//
// be_CBOOL
DWORD	fn_BE_be_CBOOL_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// be_CDWORD
DWORD	fn_BE_be_CDWORD_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// be_CINT
DWORD	fn_BE_be_CINT_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// be_CFLOAT
DWORD	fn_BE_be_CFLOAT_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// be_CSTR
DWORD	fn_BE_be_CSTR_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// be_CARRAY
DWORD	fn_BE_be_CARRAY_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//------------------ FSM ------------------------//
// be_CFMS
DWORD	fn_BE_CFMS_ON_MAP_ENUM			(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// be_CSMARTOBJECT
DWORD	fn_BE_SMARTOBJECT_ENUM			(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// be_CMSG_FROMUSER
DWORD	fn_BE_USMSG_ENUM				(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//--------------- SPESIAL -----------------------//
// be_CSTR_DIALOG
DWORD	fn_BE_DIALOGS_ENUM				(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// Get spirit types from file
DWORD	fn_BE_SPIRIT_ENUM				(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// Get speakers types from file
DWORD	fn_BE_SPEAKERS_ENUM				(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
DWORD	fn_BE_SPEAKERSPICS_ENUM			(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//-------------- ABILITY ------------------------//
// UnitAbility
DWORD	fn_UnitAbility_ENUM				(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//---------- WeaponModificators -----------------//
// WeaponModificators
DWORD	fn_WeaponModificators_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//////////////////////////////////////////////////////////////////////////

// GLOBAJ FUNCTIONS ENUMERATOR ///////////////////////////////////////////
DWORD fn_BE_FUNCTION_ENUM				(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
//////////////////////////////////////////////////////////////////////////

// PARAMETERS ENUMERATORS ////////////////////////////////////////////////
DWORD fn_BE_CLASS_ENUM					(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
DWORD fn_BE_OBJECT_ENUM					(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// OLD ENUMERATOR ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ENUMERATORS ??? ///////////////////////////////////////////////////////
DWORD fn_BE_PAR_STATE_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
//////////////////////////////////////////////////////////////////////////

// vvTEXT
DWORD	fn_BE_vvTEXT_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// vvPOINT2D
DWORD	fn_BE_vvPOINT2D_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// vvPOINT_SET
DWORD	fn_BE_vvPOINT_SET_ON_MAP_ENUM	(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// vvVector3D
DWORD	fn_BE_vvVector3D_ON_MAP_ENUM	(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// vvDIALOG
DWORD	fn_BE_vvDIALOG_ON_MAP_ENUM		(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// vvMissionLOG
DWORD	fn_BE_vvMissionLOG_ON_MAP_ENUM	(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// lvCFilm
DWORD	fn_BE_lvCFilm_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// lvCGraphObject
DWORD	fn_BE_lvCGraphObject_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);
// lvCDeffFilmMenu
DWORD	fn_BE_lvCDeffFilmMenu_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////





#endif//__ENUM__STORAGE__


















