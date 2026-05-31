#include "stdheader.h"
#include "BE_HEADERS.h"

// be_CFN_ARRAY //////////////////////////////////////////////////////////
template<class T>
T*	be_CFN_ARRAY<T>::GET(const char* fn_name) const{
	T* pFN = NULL;
	if ( g_beCheckChar(fn_name) ){
		int FnN = GetAmount();
		while ( pFN==NULL && FnN-- ) {
			pFN = (T*)( (*this)[FnN] );
			if ( pFN==NULL || g_beCheckChar( pFN->Name.pchar() )==false || strcmp( pFN->Name.pchar(), fn_name )!=0 ){
				pFN = NULL;
			};
		};
	};
	return pFN;
};

template<class T>
void	be_CFN_ARRAY<T>::ADD_FN( const char* fn_name ){
	T* pFN = GET( fn_name );
	if ( pFN==NULL ) {
		pFN = new T();
		pFN->SetName( fn_name );
		Add( pFN );
	}else{
		pFN->ClearParamList();
	};
};

template<class T>
void	be_CFN_ARRAY<T>::ADD_FN_PARAM( const char* fn_name, const char* par_type, const char* par_name, const bool local ){
	T* pFN = GET( fn_name );
	if (pFN!=NULL) {
		pFN->AddParam(par_type,par_name,local);
	};
};

// be_CLUA_FN_STORE //////////////////////////////////////////////////////
void	be_CLUA_FN_STORE::ADD_FN( const char* fn_name ){
	be_CFN_ARRAY<be_CBaseLUAFunction>::ADD_FN(fn_name);
};
void	be_CLUA_FN_STORE::ADD_FN_PARAM( const char* fn_name, const char* par_type, const char* par_name, const bool local ){
	be_CFN_ARRAY<be_CBaseLUAFunction>::ADD_FN_PARAM(fn_name,par_type,par_name,false);	
};	

//////////////////////////////////////////////////////////////////////////
be_CTOTAL_FN_STORE	g_BE_FN_STORE;
void	g_be_LUA_ADD_FN( const char* fn_name ){
	g_BE_FN_STORE.LUA_FN_STORE.ADD_FN(fn_name);
};
void	g_be_LUA_ADD_FN_PARAM( const char* fn_name, const char* par_type, const char* par_name){
	g_BE_FN_STORE.LUA_FN_STORE.ADD_FN_PARAM(fn_name,par_type,par_name,false);
};
//////////////////////////////////////////////////////////////////////////

// be_CTOTAL_FN_STORE ////////////////////////////////////////////////////
void	be_CTOTAL_FN_STORE::LUA_FN_CLEAR(){
	int N = LUA_FN_STORE.GetAmount();
	while (N--) {
		delete LUA_FN_STORE[N];
		LUA_FN_STORE[N] = NULL;
	};
	LUA_FN_STORE.Clear();
};
void	be_CTOTAL_FN_STORE::MODUL_FN_CLEAR(){
	int N = MODULE_FN_STORE.GetAmount();
	while (N--) {
		delete MODULE_FN_STORE[N];
		MODULE_FN_STORE[N] = NULL;
	};
	MODULE_FN_STORE.Clear();
};
void	be_CTOTAL_FN_STORE::CLEAR(){
	LUA_FN_CLEAR();
	MODUL_FN_CLEAR();
};
bool	be_CTOTAL_FN_STORE::AskParentForUsingExpansionClass(char* MemberName,char* ClassName){
	if (strcmp(MemberName,"LUA_FN_STORE")==0) {
		return	( strcmp(ClassName,"be_CBaseLUAFunction") == 0 );
	};
	if (strcmp(MemberName,"MODULE_FN_STORE")==0) {
		return	( strcmp(ClassName,"be_CParamFunction") == 0 );
	};
	return true;
};
int		be_CTOTAL_FN_STORE::GetFileList(ClassArray<_str>& FilesList){
	int FN_N = LUA_FN_STORE.GetAmount();
	be_CBaseLUAFunction* pLUA_FN = NULL;
	_str	FileName;
	int NinLIST = 0;
	while (FN_N--) {
		pLUA_FN = ( be_CBaseLUAFunction* )( LUA_FN_STORE[FN_N] );
		if ( pLUA_FN!=NULL && g_beCheckChar(pLUA_FN->LUA_File.pchar())==true ) {
			int N = FilesList.GetAmount();
			bool add=true;
			while (N--&&add) {
				add = ( strcmp( pLUA_FN->LUA_File.pchar(), FilesList[N]->pchar() )!=0 );
			};
			if ( add ){
				_str* pNewFileName = new _str();
				(*pNewFileName) = pLUA_FN->LUA_File.pchar();
				FilesList.Add( pNewFileName );
				pNewFileName = NULL;
				NinLIST++;
			};
		};
	};
	return NinLIST;
};
//////////////////////////////////////////////////////////////////////////

void be_REG_CLASS_FnStorage(){
	REG_CLASS(be_CLUA_FN_STORE);
	REG_CLASS(be_CMODULE_FN_STORE);

	REG_CLASS(be_CTOTAL_FN_STORE);
};









