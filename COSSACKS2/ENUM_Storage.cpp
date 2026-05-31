#include "stdheader.h"
#include "BE_HEADERS.h" 
#include "unitability.h"

extern	AbilityList Abilities;

void	g_beREG_ALL_BE_ENUM(){
	Enumerator* E = NULL;
	// be_CVariableBase
	E=ENUM.Get("ALL_vVALUES_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_VALUES_ON_MAP_ENUM));
	// be_CGROUP
	E=ENUM.Get("ALL_be_CGROUPS_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_GROUPS_ON_MAP_ENUM));
	// be_CNODE
	E=ENUM.Get("ALL_be_CNODES_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_NODES_ON_MAP_ENUM));
	// Nodes Type
	E=ENUM.Get("NODE_TYPE_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_NODE_TYPE_ON_MAP_ENUM));
	// be_CCAMERA
	E=ENUM.Get("ALL_be_CCAMERAS_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_CAMERA_ON_MAP_ENUM));
	// be_CBOOL
	E=ENUM.Get("ALL_be_CBOOL_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_be_CBOOL_ON_MAP_ENUM));
	// be_CDWORD
	E=ENUM.Get("ALL_be_CDWORD_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_be_CDWORD_ON_MAP_ENUM));
	// be_CINT
	E=ENUM.Get("ALL_be_CINT_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_be_CINT_ON_MAP_ENUM));
	// be_CFLOAT
	E=ENUM.Get("ALL_be_CFLOAT_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_be_CFLOAT_ON_MAP_ENUM));
	// be_CSTR
	E=ENUM.Get("ALL_be_CSTR_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_be_CSTR_ON_MAP_ENUM));
	// be_CARRAY
	E=ENUM.Get("ALL_be_CARRAY_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_be_CARRAY_ON_MAP_ENUM));
	// be_CFSM
	E=ENUM.Get("ALL_FSM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_CFMS_ON_MAP_ENUM));
	// be_CSMARTOBJECT
	E=ENUM.Get("BE_SMARTOBJECT_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_SMARTOBJECT_ENUM));	
	// be_CMSG_FROMUSER
	E=ENUM.Get("BE_USMSG_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_USMSG_ENUM));	
	// be_CSTR_DIALOG
	E=ENUM.Get("BE_DIALOGS_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_DIALOGS_ENUM));	
	// SPIRIT from file
	E=ENUM.Get("BE_SPIRIT_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_SPIRIT_ENUM));
	// Speaker from file
	E=ENUM.Get("BE_SPEAKERS_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_SPEAKERS_ENUM));
	E=ENUM.Get("BE_SPEAKERSPICS_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_SPEAKERSPICS_ENUM));
	// be_CBaseLUAFunction	be_CBaseParamFunction
	E=ENUM.Get("BE_FUNCTION_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_FUNCTION_ENUM));
	// All Class Type in BE
	E=ENUM.Get("BE_CLASS_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_CLASS_ENUM));
	// PARAMETERS OBJECT ENUMERATOR
	E=ENUM.Get("BE_OBJECT_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_OBJECT_ENUM));

	// UnitAbility
	E=ENUM.Get("UnitAbility_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_UnitAbility_ENUM));

	// WeaponModificator
	E=ENUM.Get("WeaponModificators_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_WeaponModificators_ENUM));
	// 
};

// ENUMERATORS MAKROSS ///////////////////////////////////////////////////
// STRASHNIY HAK !!!!
bool g_beRECREATE = false;
DWORD	ADD_BE_PARAMS_TO_ENUM(	RefreshableEnumeratorContext* in,
								EnumeratorOutputStream* out,
								DWORD PrevCallHash,
								const char* ParTypeName
							 )
{
	if ( in->Context!=NULL ) {
		BaseClass* pBASE = dynamic_cast<BaseClass*>(in->Context);
		void* DataPtr;
		void *ExtraPtr;
		BaseClass* ClassPtr;
		int NC = 0;
		while ( pBASE!=NULL ) {
			if(pBASE->FindElementByName("InArgList",&ClassPtr,&DataPtr,&ExtraPtr,pBASE)){
				_str className = ClassPtr->GetClassName();
				if(!strncmp(ClassPtr->GetClassName(),"ClassArray.be_CPrametrBase",26)){
					be_CParamList* pPL=(be_CParamList*)DataPtr;
					for ( int i=0; i<pPL->GetAmount(); i++ ){
						be_CPrametrBase* pParBase = dynamic_cast<be_CPrametrBase*>((*pPL)[i]);
						if ( pParBase && g_beCheckChar(pParBase->ParamType.pchar()) ){
							bool add = ( strcmp(pParBase->GetParamType(),ParTypeName)==0 );
							if (add==false){
								OneClassStorage* pOCS = CGARB.GetClass(ParTypeName);
								if (pOCS!=NULL) {
									add = pOCS->CheckAll( pParBase->GetParamType(), true, true, true );
								};
							};
							if (add) {
								if (g_BE_USAGE_FLAG==BE_IF_CALL) {
									be_CBasePrametr* pBasePar = (be_CBasePrametr*)pParBase;
									pBasePar->PrepareParam(true);
									out->add( pParBase->ParamName.pchar(),	pBasePar->UniveraslParam.GlobalReference.ID, NULL);
								}else{
									out->add( pParBase->ParamName.pchar(),	(*pPL)[i]->GetObjectGlobalID(), NULL);
								}
								NC++;
							};
						};
					};
				//	return 0xFFFFFF;
					g_beRECREATE = true;
					return NC;
				};
			};
			pBASE = pBASE->GetParent();
		};
	};
	return 0xFFFFFFFF;
};

DWORD	ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	EnumeratorOutputStream*			out,
												ClassArray<ReferableBaseClass>* pObjArray,
												const char* pTopicName
										   )
{
	if ( pObjArray==NULL ) return 0;
	int NUM = 0;
	for( int i=0; i<pObjArray->GetAmount(); i++){
		if ( (*pObjArray)[i] != NULL && g_beCheckChar((*pObjArray)[i]->Name.pchar()) ){
			out->add(	(*pObjArray)[i]->Name.pchar()				,
						(*pObjArray)[i]->GetObjectGlobalID()	,
						pTopicName							
					);
			NUM++;
		};
	};
	return NUM;
}
DWORD	ADD_BE_OBJECT_TO_ENUM(	RefreshableEnumeratorContext* in,
								EnumeratorOutputStream* out,
								DWORD PrevCallHash,
								ClassArray<ReferableBaseClass>* pObjArray,
								const char* ClassName
							 )
{
	// Global test
	if ( pObjArray==NULL )									return PrevCallHash;
	if ( g_beCheckChar(ClassName)==false )					return PrevCallHash;
	// Check is new object has created in system?
	if ( g_beOBJ_CONTROLER.GetState(ClassName)==false && PrevCallHash!=0xFFFFFF && !g_beRECREATE )		return PrevCallHash;
		
	// Create topic
	char*	pTopic = ( ( strlen(ClassName)>4 ) ? ( (char*)&(ClassName[4]) ) : ( NULL ) );
	if ( strncmp(ClassName,"be_C",4)!=0 ) pTopic = (char*)ClassName;
		
	out->add("NULL",0xFFFFFFFF,pTopic);
	int NUM = 1;
	NUM += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(out,pObjArray,pTopic);
	out->sortByString();
	g_beOBJ_CONTROLER.DropState(ClassName);
	g_beRECREATE = false;
	return NUM;								
};
DWORD	VAR_BE_HALPER_TO_ENUM(	RefreshableEnumeratorContext* in,
								EnumeratorOutputStream* out,
								DWORD PrevCallHash,
								const char* ClassName
							 )
{
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,ClassName);
	if (retN!=0xFFFFFFFF&&retN!=0) return retN;
	g_beRECREATE = true;
	return ADD_BE_OBJECT_TO_ENUM(	in, 
									out, 
									PrevCallHash, 
									(ClassArray<ReferableBaseClass>*)(g_beVAR_STORE.GET((char*)ClassName)),
									ClassName
								);
};
//////////////////////////////////////////////////////////////////////////

// GLOBAL VALUES UNUMERATORS /////////////////////////////////////////////

// be_CVariableBase - store all global variables on map
DWORD	fn_BE_VALUES_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
//	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"be_CVariableBase");
//	if (retN!=0xFFFFFFFF) return retN;

	if ( g_beOBJ_CONTROLER.GetState("be_CVariableBase")==false	 &&
		 g_beOBJ_CONTROLER.GetState("UnitAbility")==false		 &&
		 g_beOBJ_CONTROLER.GetState("WeaponModificators")==false	)		return PrevCallHash;

	int ADD_N = 0;
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CBOOL"),
													"BOOL"				
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CDWORD")
													,"DWORD"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CINT")
													,"INT"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CFLOAT")
													,"FLOAT"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CSTR"),
													"STR"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CSTR_DIALOG"),
													"STR_DIALOG"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CARRAY"),
													"ARRAY"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CGROUP"),
													"GROUP"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CCAMERA"),
													"CAMERA"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CNODE"),
													"NODE"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CBLANK"),
													"BLANK"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CUnitType"),
													"UnitType"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CFurmationType"),
													"FurmationType"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CFSM"),
													"FSM"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CSMARTOBJECT"),
													"SMARTOBJECT"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)g_beVAR_STORE.GET("be_CMSG_FROMUSER"),
													"MSG_FROMUSER"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)( &(Abilities.Abilities) ),
													"UnitAbility"
												);
	ADD_N += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
													(ClassArray<ReferableBaseClass>*)( &(GameWeaponSystem.AllWeaponModificators) ),
													"WeaponModificators"
												);

	out->sortByString();

	g_beOBJ_CONTROLER.DropState("be_CVariableBase");
	g_beOBJ_CONTROLER.DropState("UnitAbility");
	g_beOBJ_CONTROLER.DropState("WeaponModificators");
	return ADD_N;
};

//--------------- MAP OBJECTS -------------------//
// be_CNODE
DWORD	fn_BE_NODES_ON_MAP_ENUM			(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CNODE" );
};

// Nodes Type 
DWORD	fn_NODE_TYPE_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD PrevCallHash
										)
{
	if (PrevCallHash==2) return PrevCallHash;
	out->add("NODE"		,0,NULL);	
	out->add("BLANK"	,1,NULL);	
	return 2;
};

// be_CGROUP
DWORD	fn_BE_GROUPS_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CGROUP" );
};

// be_CCAMERA
DWORD	fn_BE_CAMERA_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CCAMERA" );
};

//--------------- VARIABLES ---------------------//
// be_CBOOL
DWORD	fn_BE_be_CBOOL_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CBOOL" );
};
// be_CDWORD
DWORD	fn_BE_be_CDWORD_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CDWORD" );
};
// be_CINT
DWORD	fn_BE_be_CINT_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CINT" );
};
// be_CFLOAT
DWORD	fn_BE_be_CFLOAT_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CFLOAT" );
};
// be_CSTR
DWORD	fn_BE_be_CSTR_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CSTR" );
};
// be_CARRAY
DWORD	fn_BE_be_CARRAY_ON_MAP_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	VAR_BE_HALPER_TO_ENUM( in, out, PrevCallHash, "be_CARRAY" );
};

//------------------ FSM ------------------------//
// be_CFMS
DWORD	fn_BE_CFMS_ON_MAP_ENUM			(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	ADD_BE_OBJECT_TO_ENUM(	in, 
									out, 
									PrevCallHash, 
									(ClassArray<ReferableBaseClass>*)(g_beVAR_STORE.GET("be_CFSM")),
									"be_CFSM" 
								 );
};
// be_CSMARTOBJECT
DWORD	fn_BE_SMARTOBJECT_ENUM			(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	ADD_BE_OBJECT_TO_ENUM(	in, 
									out, 
									PrevCallHash, 
									(ClassArray<ReferableBaseClass>*)(g_beVAR_STORE.GET("be_CSMARTOBJECT")),
									"be_CSMARTOBJECT" 
								 );	
};
// be_CMSG_FROMUSER
DWORD	fn_BE_USMSG_ENUM				(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	if ( g_beOBJ_CONTROLER.GetState("be_CMSG_FROMUSER")==false ) return PrevCallHash;
	int msgN = 0;

	// FSM Sorted msg
	int fsmN = g_beVAR_STORE.FSM_LIST.GetAmount();
	be_CFSM* pFSM = NULL;
	be_CMSG_FROMUSER* pMSG = NULL;
	for ( int i=0; i<fsmN; i++ ){
		pFSM = g_beVAR_STORE.FSM_LIST[i];
		if (pFSM&&g_beCheckChar(pFSM->Name.pchar())) {
			out->add("NULL",0xFFFFFFFF,pFSM->Name.pchar());
			msgN++;
			int fsmmsgN = pFSM->HendledMSG.GetAmount();
            for ( int j=0; j<fsmmsgN; j++ ){
				pMSG = pFSM->HendledMSG[j];
				if (pMSG&&g_beCheckChar(pMSG->Name.pchar())){
					out->add(pMSG->Name.pchar(),pMSG->GetObjectGlobalID(),pFSM->Name.pchar());
					msgN++;
				};
			};
		};
	};

	// msg from storage
	int smsgN = g_beVAR_STORE.USERMSG_LIST.GetAmount();
	for ( int i=0; i<smsgN; i++ ){
		pMSG = g_beVAR_STORE.USERMSG_LIST[i];
		if (pMSG&&g_beCheckChar(pMSG->Name.pchar())){
			out->add(pMSG->Name.pchar(),pMSG->GetObjectGlobalID(),"STORE MSG");
			msgN++;
		};
	};

	out->sortByString();
	g_beOBJ_CONTROLER.DropState("be_CMSG_FROMUSER");
	return msgN;
};

//--------------- SPESIAL -----------------------//
// be_CSTR_DIALOG
DWORD	fn_BE_DIALOGS_ENUM				(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	return	ADD_BE_OBJECT_TO_ENUM(	in, 
									out, 
									PrevCallHash, 
									(ClassArray<ReferableBaseClass>*)(g_beVAR_STORE.GET("be_CSTR_DIALOG")),
									"be_CSTR_DIALOG" 
								 );
};
// Get spirit types from file
DWORD fn_BE_SPIRIT_ENUM					(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	static bool Init = true;
	if (Init) {
		Init=false;
		FILE* stream = fopen( "Missions\\ADDITION\\Spirit.txt" , "r" );
		if (stream) {
			int N = 0;
			char s[256];
			while ( !feof( stream ) ) {
				fscanf( stream, "%s", s );
				if ( s[0]=='#' ){
					out->add( &(s[1]), N, NULL);
					N++;
				};
			};
			PrevCallHash = N;
			fclose(stream);
		};
	};
	return PrevCallHash;
};
// Get speakers types from file
DWORD fn_BE_SPEAKERS_ENUM				(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	static bool Init = true;
	if (Init) {
		Init=false;
		FILE* stream = fopen( "Missions\\ADDITION\\Speakers.txt" , "r" );
		if (stream) {
			int N = 0;
			char s[256];
			while ( !feof( stream ) ) {
				fscanf( stream, "%s", s );
				if ( s[0]=='#' ){
					out->add( &(s[1]), N, NULL);
					N++;
				};
			};
			PrevCallHash = N;
			fclose(stream);
		};
	};
	return PrevCallHash;
};

DWORD fn_BE_SPEAKERSPICS_ENUM				(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	static bool Init = true;
	if (Init) {
		Init=false;
		FILE* stream = fopen( "Missions\\ADDITION\\SpeakersPics.txt" , "r" );
		if (stream) {
			int N = 0;
			char s[256];
			char gp[256];
			int sp;

			while ( !feof( stream ) ) {
				fscanf( stream, "%s %s %d", s, gp, &sp);
				int id=GPS.PreLoadGPImage(gp);
				if(id==-1){
					Log.Error("SpeakersPics.txt: unable to load %s",gp);
				}				
				out->add( s, id+(sp<<16), NULL );
				N++;
			};
			PrevCallHash = N;
			fclose(stream);
		};
	};
	return PrevCallHash;
};

//-------------- ABILITY ------------------------//
// UnitAbility
DWORD	fn_UnitAbility_ENUM				(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD PrevCallHash
										)
{
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"UnitAbility");
	if (retN!=0xFFFFFFFF&&retN!=0) return retN;

	return	ADD_BE_OBJECT_TO_ENUM(	in, 
									out, 
									PrevCallHash, 
									(ClassArray<ReferableBaseClass>*)( &(Abilities.Abilities) ),
									"UnitAbility"
								 );
};

//---------- WeaponModificators -----------------//
// WeaponModificators
DWORD	fn_WeaponModificators_ENUM		(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD PrevCallHash
										)
{
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"WeaponModificators");
	if (retN!=0xFFFFFFFF&&retN!=0) return retN;

	return	ADD_BE_OBJECT_TO_ENUM(	in, 
									out, 
									PrevCallHash, 
									(ClassArray<ReferableBaseClass>*)( &(GameWeaponSystem.AllWeaponModificators) ),
									"WeaponModificators"
								 );
};

//////////////////////////////////////////////////////////////////////////

// GLOBAJ FUNCTIONS ENUMERATOR ///////////////////////////////////////////
DWORD fn_BE_FUNCTION_ENUM				(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	if ( g_beOBJ_CONTROLER.GetState("be_CBaseLUAFunction"	) == false &&
		 g_beOBJ_CONTROLER.GetState("be_CParamFunction"		) == false 		)	return PrevCallHash;

	out->add("NULL",0xFFFFFFFF,"LUAFunction");
	out->add("NULL",0xFFFFFFFF,"ParamFunction");
	int NUM = 2;
	
	NUM += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
												(ClassArray<ReferableBaseClass>*)&(g_BE_FN_STORE.LUA_FN_STORE),
												"LUAFunction"	 
											  );
	NUM += ADD_BE_OBJECT_TO_ENUM_WITHOUT_CHECK(	out,
												(ClassArray<ReferableBaseClass>*)&(g_BE_FN_STORE.MODULE_FN_STORE),
												"ParamFunction"
											  );
	
	out->sortByString();
	g_beOBJ_CONTROLER.DropState("be_CBaseLUAFunction");
	g_beOBJ_CONTROLER.DropState("be_CParamFunction");
	
	return NUM;
};
//////////////////////////////////////////////////////////////////////////

// PARAMETERS ENUMERATORS ////////////////////////////////////////////////
DWORD fn_BE_CLASS_ENUM					(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	if (PrevCallHash==17) return PrevCallHash;

	out->add("NODE"			,		0,"-= MAP OBJECT =-");
	out->add("BLANK"		,		1,"-= MAP OBJECT =-");
	out->add("GROUP"		,		2,"-= MAP OBJECT =-");
	out->add("UnitType"		,		3,"-= MAP OBJECT =-");
	out->add("FurmationType",		4,"-= MAP OBJECT =-");
	out->add("CAMERA"		,		5,"-= MAP OBJECT =-");

	out->add("BOOL"			,6	,"-= VALUE =-");
	out->add("DWORD"		,7	,"-= VALUE =-");
	out->add("INT"			,8	,"-= VALUE =-");
	out->add("FLOAT"		,9	,"-= VALUE =-");
	out->add("STR"			,10	,"-= VALUE =-");
	out->add("STR_DIALOG"	,11	,"-= VALUE =-");
	out->add("ARRAY"		,12	,"-= VALUE =-");

	out->add("MSG_FROMUSER"	,13	,"-= FSM =-");
	out->add("FSM"			,14	,"-= FSM =-");
	out->add("SMARTOBJECT"	,15	,"-= FSM =-");

	out->add("UnitAbility"			,16	,"-= ABILITY =-");
	out->add("WeaponModificators"	,17	,"-= ABILITY =-");
    
    return 17;
};
DWORD fn_BE_OBJECT_ENUM					(	RefreshableEnumeratorContext*	in,
											EnumeratorOutputStream*			out,
											DWORD							PrevCallHash
										)
{
	static _str PrivTOPIC	= "";
	static int  PrivN		= -1;

	be_CUniveraslParam* pROOT = dynamic_cast<be_CUniveraslParam*>(in->Context);
	if (pROOT==NULL) return PrevCallHash;

	be_CBasePrametr* pBP = (be_CBasePrametr*)(pROOT->GetParent());

	if ( pBP==NULL ) return 0;

	if (g_beCheckChar(pBP->ParamType.pchar())==false) return 0;
	char s1[128];
    sprintf(s1,"%s",pBP->ParamType.pchar());

	if ( in->Context!=NULL ) {
		BaseClass* pBASE = dynamic_cast<BaseClass*>(in->Context);
		void* DataPtr;
		void *ExtraPtr;
		BaseClass* ClassPtr;
		int NC = 0;
		_str strCN = "";
		int iteration = 0;
		while ( pBASE!=NULL ) {
			strCN = pBASE->GetClassName();
			iteration++;
			bool OK = g_beCheckChar(strCN.pchar());
			OK = OK && ( strCN=="be_CParamFunction" || (strCN=="be_CSUBFUNCTION" && iteration>4) );
			if ( OK ){
				if(pBASE->FindElementByName("InArgList",&ClassPtr,&DataPtr,&ExtraPtr,pBASE)){
					_str className = ClassPtr->GetClassName();
					if(!strncmp(ClassPtr->GetClassName(),"ClassArray.be_CPrametrBase",26)){
						
						be_CParamList* pPL=(be_CParamList*)DataPtr;
						for ( int i=0; i<pPL->GetAmount(); i++ ){
							be_CPrametrBase* pParBase = dynamic_cast<be_CPrametrBase*>((*pPL)[i]);
							if ( pParBase && g_beCheckChar(pParBase->ParamType.pchar()) ){
								bool add = ( pParBase->ParamType == s1 );
								if (add==false){
									OneClassStorage* pOCS = CGARB.GetClass(s1);
									if (pOCS!=NULL) {
										add = pOCS->CheckAll( s1,true,true,true );
									};
								};
								if (add) {
									
									if (g_BE_USAGE_FLAG==BE_IF_CALL) {
										be_CBasePrametr* pBasePar = (be_CBasePrametr*)pParBase;
										pBasePar->PrepareParam(true);
										out->add( pParBase->ParamName.pchar(),	pBasePar->UniveraslParam.GlobalReference.ID, pParBase->ParamType.pchar());
									}else{
										out->add( pParBase->ParamName.pchar(),	(*pPL)[i]->GetObjectGlobalID(), pParBase->ParamType.pchar());
									}
									NC++;
								};
							};
						};
						PrivTOPIC	= s1;
						PrivN		= -1;
						if (NC>0) return NC;
					};
				};
			};
			pBASE = pBASE->GetParent();
		};
	};

	if ( g_beOBJ_CONTROLER.GetState("be_CVariableBase") || PrivTOPIC=="" || PrivN==-1 || PrivTOPIC!=s1 || PrevCallHash==0 ){

		Enumerator* E=ENUM.Get("ALL_vVALUES_ON_MAP");
		if (E==NULL) return 0;
		int HEN = E->GetAmount();
		if (HEN==0)	return 0;
		
		int ADD_N = 0;

		// Finde topic
		char* pTOPIC = NULL;
		char* pELEMT = NULL;
		bool begin = false;
		for ( int i=0; i<HEN; i++ ){
			pTOPIC = E->GetCategory(i);
			if ( g_beCheckChar(pTOPIC) && strcmp(pTOPIC,s1)==0 ){
				if ( ADD_N==0 ){
					out->add("NULL",0xFFFFFFFF,s1);
					ADD_N++;
				};
				pELEMT = E->GetStr(i);
				if ( g_beCheckChar(pELEMT) ) {
					out->add(pELEMT,E->GetVal(i),s1);
					ADD_N++;
				};
			};
			pTOPIC = NULL;
		};
				
		out->sortByString();

		PrivTOPIC	= s1;
		PrivN		= ADD_N;

		if (g_beOBJ_CONTROLER.GetState("be_CVariableBase")) g_beOBJ_CONTROLER.DropState("be_CVariableBase");

		return ADD_N;
	};

	return PrivN;
};
//////////////////////////////////////////////////////////////////////////





























