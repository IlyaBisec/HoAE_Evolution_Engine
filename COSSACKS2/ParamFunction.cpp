#include "stdheader.h"
#include "BE_HEADERS.h"
#include "GameInterface.h"

// be_CParamFunction /////////////////////////////////////////////////////
be_CParamFunction::be_CParamFunction(){
	g_beOBJ_CONTROLER.SetState("be_CParamFunction");
};
be_CParamFunction::~be_CParamFunction(){
	g_beOBJ_CONTROLER.SetState("be_CParamFunction");
};
const	char*	be_CParamFunction::GetThisElementView(const char* LocalName){
	if ( Name.pchar()==NULL || Name.pchar()[0]==0 )	return LocalName;
	// Function Name
	_str strText	 = "{C ffFFFF00}";
	strText			+= Name;
	strText			+= "( ";
	// Param Types
	be_CBasePrametr* pPAR = NULL;
	int ArgN = InArgList.GetAmount();
	for ( int i=0; i<ArgN; i++ ){
		strText		+= "{C ff00FF00}";
		pPAR = (be_CBasePrametr*)(InArgList[i]);
		strText		+= pPAR->ParamType;
		pPAR = NULL;
		if ( i+1!=ArgN )
			strText	+= "{C ffFFFF00}, ";
	};
	// end
	strText			+= " {C ffFFFF00})";
	char* pText = GetGlobalBuffer();
	if (pText==NULL)	return LocalName;
	sprintf( pText, "%s", strText.pchar() );
	return pText;
};
int				be_CParamFunction::CallFunction(){
	return 0;
};
BE_USAGE_FLAGS	be_CParamFunction::GetParentUSAGE(){
	if (be_CFunctionBase::GetParentUSAGE()==BE_IF_CALL)	return be_CFunctionBase::GetParentUSAGE();
	return BE_IF_EDIT;
};
// be_CBaseLUAFunction ///////////////////////////////////////////////////
be_CBaseLUAFunction::be_CBaseLUAFunction(){
	pDLL_Function = NULL;
	g_beOBJ_CONTROLER.SetState("be_CBaseLUAFunction");
};
be_CBaseLUAFunction::~be_CBaseLUAFunction(){
	pDLL_Function = NULL;
	g_beOBJ_CONTROLER.SetState("be_CBaseLUAFunction");
};
const	char*	be_CBaseLUAFunction::GetThisElementView(const char* LocalName){
	if ( Name.pchar()==NULL || Name.pchar()[0]==0 )	return LocalName;
	// Function Name
	_str strText	 = "{C ffFFFF00}";
	strText			+= Name;
	strText			+= "( ";
	// Param Types
	be_CBasePrametr* pPAR = NULL;
	int ArgN = InArgList.GetAmount();
	for ( int i=0; i<ArgN; i++ ){
		strText		+= "{C ff00FF00}";
		pPAR = (be_CBasePrametr*)(InArgList[i]);
		strText		+= pPAR->ParamType;
		pPAR = NULL;
		if ( i+1!=ArgN )
			strText	+= "{C ffFFFF00}, ";
	};
	// end
	strText			+= " {C ffFFFF00})";
	char* pText = GetGlobalBuffer();
	if (pText==NULL)	return LocalName;
	sprintf( pText, "%s", strText.pchar() );
	return pText;
};
typedef int (*CALL_FN)(pParamList*);
int				be_CBaseLUAFunction::CallFunction(){
	if ( pDLL_Function!=NULL ){
		CALL_FN pCallDLL_FN = ( CALL_FN )pDLL_Function;
		pParamList	pPL(&InArgList);
		return pCallDLL_FN(&pPL);
	};
	return 0;
};
bool			be_CBaseLUAFunction::DLL_VAILID(){
	if (pDLL_Function==NULL){
		void* FindFunctionInPermanentDLLs(const char* Name);
		static _str	decorName;
		decorName  = "?";
		decorName += Name;
		decorName += "@@YAHPAVpParamList@@@Z";
		pDLL_Function = FindFunctionInPermanentDLLs(decorName.pchar());
	};
	return (pDLL_Function!=NULL);	
};
BE_USAGE_FLAGS	be_CBaseLUAFunction::GetParentUSAGE(){
	if (be_CFunctionBase::GetParentUSAGE()==BE_IF_CALL)	return be_CFunctionBase::GetParentUSAGE();
	return BE_IF_EDIT;
};
// be_CRefParamFunction //////////////////////////////////////////////////
BE_USAGE_FLAGS	be_CRefParamFunction::GetParentUSAGE(){
	if (be_CFunctionBase::GetParentUSAGE()==BE_IF_CALL)	return be_CFunctionBase::GetParentUSAGE();
	return BE_IF_USE;
};
const	char*	be_CRefParamFunction::GetThisElementView(const char* LocalName){
	if ( CallingFunctionName.Get()!=NULL ){
		_str strText;
		strText  = "{C ffFFFF00}";
		strText += CallingFunctionName.pchar();
		strText += "( ";

		// add input params for function
		for ( int i=0; i<InArgList.GetAmount(); i++ ){

			be_CBasePrametr* pPAR = (be_CBasePrametr*)(InArgList[i]);
			pPAR->UniveraslParam.PrepareParam();
			
			bool PARAM_OK = false;
			if ( pPAR->UniveraslParam.Local==true ){
				strText += "{C 0xFF00ffff}";
				be_CVariableBase* pLVAR = (be_CVariableBase*)pPAR->UniveraslParam.GlobalReference.Get();
				if (pLVAR){
					_str strVALUE = "";
					pLVAR->GetValueAsSTR(strVALUE);
					strText += strVALUE;
					PARAM_OK = true;
				};
			}else{
				ReferableBaseClass* pCPAR = pPAR->UniveraslParam.GlobalReference.Get();
				if (pCPAR&&g_beCheckChar(pCPAR->GetClassName())) {
					strText += "{C 0xFF00ffff}";
					if (strcmp(pCPAR->GetClassName(),"be_CBasePrametr")==0) {
						be_CBasePrametr* pBP = (be_CBasePrametr*)pCPAR;
						if (pBP) strText += pBP->ParamName.pchar();
					}else{
						if (g_beCheckChar(pCPAR->Name.pchar())) strText += pCPAR->Name;
					};
					PARAM_OK = true;
				};
			};

			if ( PARAM_OK ){
				strText += "{C ffFFFF00}";
			}else{
				strText += InArgList[i]->ParamType;
			};

			if ( i!=InArgList.GetAmount()-1 ) strText += ", ";
		};

		strText += " )";
		char* pText = GetGlobalBuffer();
		if (pText==NULL)	return LocalName;
		sprintf( pText, "%s", strText.pchar() );

		InArgList.UpdateEnumForOpenParam();

		return pText;
	};

	return "{C ffFFFF00}Select Function Name";
};
void			be_CRefParamFunction::CreateUseFnName(){
	CallingFunctionName.ClearRef();
	be_CBaseParamFunction* pFn = (be_CBaseParamFunction*)(CallingFunctionName.Get());
	if (pFn!=NULL) {
		InArgList.CreateCopyFrom(pFn->InArgList);
	};
};
void			be_CRefParamFunction::CCreateParamList::EvaluateFunction(){
	((be_CRefParamFunction*)(GetParent()))->CreateUseFnName();
};
int				be_CRefParamFunction::CallFunction(ActiveUnitAbility* _pAbility){
	pAbility = _pAbility;
	int ret = CallFunction();
	pAbility = NULL;
	return ret;
};
int				be_CRefParamFunction::CallFunction(ActiveUnitAbility* _pAbility,OneObject* pOB){
	pAbilityActivator = pOB;
	int ret = CallFunction(_pAbility);
	pAbilityActivator = NULL;
	return ret;
};
int				be_CRefParamFunction::CallFunction(){
	int retValue = 0;

	BE_USAGE_FLAGS	l_OLD_USAGE_FLAG=g_BE_USAGE_FLAG;
	g_BE_USAGE_FLAG=BE_IF_CALL;

	be_CBaseParamFunction*	pBFn = (be_CParamFunction*)(CallingFunctionName.Get());
	if (pBFn!=NULL){
		_str FnClassName = pBFn->GetClassName();
		if (g_beCheckChar(FnClassName.pchar())) {
			if ( FnClassName == "be_CParamFunction" ) {
				be_CParamFunction* pBFnPar = (be_CParamFunction*)(pBFn);
				if (pBFnPar!=NULL) {
				//	if ( pBFnPar->SetParamData(InArgList)==true ){
						pBFnPar->SetParamData(InArgList);
						retValue = pBFnPar->FunctionBody.CallFunction();
				//	};
				};
			}else if ( FnClassName == "be_CBaseLUAFunction" ) {
				be_CBaseLUAFunction* pBFnLUA = (be_CBaseLUAFunction*)(pBFn);
				if (pBFnLUA!=NULL){
				//	if ( pBFnLUA->SetParamData(InArgList)==true ){
						pBFnLUA->SetParamData(InArgList);

						if ( pBFnLUA->DLL_VAILID() ){
							//try{
								retValue = pBFnLUA->CallFunction();
							//}catch (...) {
							//	DriveMode()->PROCESS=false;	
							//	_str _errorMessage = pBFnLUA->Name;
							//	_errorMessage += "(\n";
							//	_str _params_data;
							//	pBFnLUA->InArgList._string(_params_data);
							//	_errorMessage += _params_data;
							//	_errorMessage += "\n)";
							//	MessageBox( NULL, _errorMessage.pchar(), "ERROR call DLL Function", MB_OK );
							//};
							
						}else {
#ifdef  __LUA__
							if (BattleHandler()->LUA_MISS.LUA_STATE){
								try{
									retValue = luabind::call_function<int>(	BattleHandler()->LUA_MISS.LUA_STATE, 
																			pBFnLUA->Name.pchar(),
																			boost::ref(pBFnLUA->InArgList)
																		);
								}catch (...) {
									DriveMode()->PROCESS=false;
								//	_str errLUA_stack = lua_tostring(BattleHandler()->LUA_MISS.LUA_STATE, -1);
									BattleHandler()->LUA_SAFE_CLOSE_MISS();
									_str errTOPIC	= "ERROR : Call Function from LUA REF FN LIST";
									_str errMSG		= pBFnLUA->GetThisElementView(pBFnLUA->Name.pchar());
								//	errMSG += "\n\n";
								//	errMSG += errLUA_stack;
									MessageBox( NULL, errMSG.pchar(), errTOPIC.pchar(), MB_OK );
								};
							};
#endif//__LUA__
						};
				//	};
				};
			};
		};
	};

	g_BE_USAGE_FLAG=l_OLD_USAGE_FLAG;

	return retValue;
};
//////////////////////////////////////////////////////////////////////////

void	be_REG_CLASS_ParamFunction(){
	REG_CLASS_EX(be_CParamFunction,"{C ff0000FF}PARAM FUNC");	
	REG_CLASS_EX(be_CBaseLUAFunction,"{C ff0000FF}PARAM FUNC");	
	REG_CLASS_EX(be_CRefParamFunction,"{C ff0000FF}PARAM FUNC");
};

//////////////////////////////////////////////////////////////////////////


























