#include "stdheader.h"
#include "BE_HEADERS.h"

ActiveUnitAbility*	be_CFunctionBase::pAbility = NULL;
OneObject*			be_CFunctionBase::pAbilityActivator = NULL;

// be_CFunctionBase //////////////////////////////////////////////////////
int	be_CFunctionBase::CallFunction(){ 
	return 0; 
};
const char*		be_CFunctionBase::GetThisElementView(const char* LoaclName){
	if (LoaclName!=NULL){
		char* Presentation = GetGlobalBuffer();
		CE_WCP.GetPresentation(Presentation,this);
		if ( g_beCheckChar(Presentation) )	return Presentation;
	};
	return LoaclName;
};
BE_USAGE_FLAGS	be_CFunctionBase::GetParentUSAGE(){
	return g_BE_USAGE_FLAG;	
};
bool			be_CFunctionBase::isElementInParentClass(char* FildName){
	BaseClass* pBASE = this->GetParent();
	void* DataPtr;
	void *ExtraPtr;
	BaseClass* ClassPtr;
	while ( pBASE!=NULL ) {
		if(pBASE->FindElementByName("InArgList",&ClassPtr,&DataPtr,&ExtraPtr,pBASE)){
			return true;
		};
		pBASE = pBASE->GetParent();
	};
	return false;
};
void			be_CFunctionBase::clearCB(){
	dbgCB_IDS.Clear();	
};
void			be_CFunctionBase::addCB_ID(int _id){
	dbgCB_IDS.Add(_id);
};
int				be_CFunctionBase::getCB_ID(int index){
	if ( 0<=index && index<dbgCB_IDS.GetAmount() )	return dbgCB_IDS[index];
	return -1;
};
// be_CBaseParamFunction /////////////////////////////////////////////////
int be_CBaseParamFunction::CallFunction(){ 
	return 0; 
};
bool	be_CBaseParamFunction::CheckParam( const char* pParamName ){
	return InArgList.CheckParam( pParamName );
};
bool	be_CBaseParamFunction::SetParamData(be_CParamList& paramData){
	return InArgList.SetParamData(paramData);
};
bool	be_CBaseParamFunction::SetName(const char* fn_name){
	if ( g_beCheckChar(fn_name) ){
		Name = fn_name;
		return true;
	};
	return false;
};
void	be_CBaseParamFunction::ClearParamList(){
	InArgList.Clear();	
};
bool	be_CBaseParamFunction::AddParam( const char* par_type, const char* par_name, bool local ){
	return InArgList.AddParam(par_type,par_name,local);
};
// be_CBasePtrFunction ///////////////////////////////////////////////////
int be_CBasePtrFunction::CallFunction(){ 
	if ( Get()!=NULL ) 
		return Get()->CallFunction(); 
	return 0; 
};
bool be_CBasePtrFunction::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	if (ClassEditor::CheckIfOpen((BaseClass*)DataPtr))	dest = "...";
	else												dest = "%$this$%";
	return true;
};
// be_CBaseArrayFunction /////////////////////////////////////////////////
int be_CBaseArrayFunction::CallFunction(){ 
	int retVal = 0;
	int N=GetAmount(); 
	for ( int i=0; i<N; i++ ){
		addrand(i);
		g_beDebuger.callFunction( (*this)[i]->getCB_ID(0) );
		retVal = (*this)[i]->CallFunction();
	};
	return retVal;
};
bool be_CBaseArrayFunction::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	if (ClassEditor::CheckIfOpen((BaseClass*)DataPtr))	dest = "...";
	else												dest = "%$this,/$%";
	return true;
};
//////////////////////////////////////////////////////////////////////////
void	be_REG_CLASS_Function(){
	REG_CLASS_EX(be_CFunctionBase,		"{C ff0000FF}BASE FUNC");
	REG_CLASS_EX(be_CBaseParamFunction,	"{C ff0000FF}BASE FUNC");
	REG_CLASS_EX(be_CComment,			"{C ff0000FF}BASE FUNC")
	REG_CLASS_EX(be_CBasePtrFunction,	"{C ff0000FF}BASE FUNC");
	REG_CLASS_EX(be_CBaseArrayFunction,	"{C ff0000FF}BASE FUNC");
};