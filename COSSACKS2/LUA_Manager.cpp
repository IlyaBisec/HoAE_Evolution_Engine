#include "stdheader.h"
#include "BE_HEADERS.h"

#ifdef  __LUA__
// LUA CLASS /////////////////////////////////////////////////////////////
// 	Function for global operation with lua
LUA_MGR::LUA_MGR(){
	LUA_STATE=NULL;
	useDEBUGER=false;
};
LUA_MGR::~LUA_MGR(){
	CLOSE();
};
bool	LUA_MGR::OPEN(){
	LUA_STATE = lua_open();
	if (LUA_STATE==NULL)	return false;
	return luabind_OPEN();
};
bool	LUA_MGR::CLOSE(){
#ifdef	__LUA_DEBUGGER__
	if (useDEBUGER) g_LUA_DBG.UnPrepareLua(LUA_STATE,0);
	useDEBUGER=false;
#endif//__LUA_DEBUGGER__
	if (LUA_STATE!=NULL) lua_close(LUA_STATE);
	LUA_STATE=NULL;
	clearFList();
	return true;
};
bool	LUA_MGR::BIND( void ( *BindFunction )(lua_State*) ){
	if (LUA_STATE==NULL||BindFunction==NULL)	return false;
	BindFunction(LUA_STATE);
	return true;
};
bool	LUA_MGR::DEBUG_PREP(){
	if (LUA_STATE==NULL)	return false;
#ifdef	__LUA_DEBUGGER__
	g_LUA_DBG.PrepareLua(LUA_STATE);
	g_LUA_DBG.PrepareLuaBind();
	useDEBUGER=true;
#endif//__LUA_DEBUGGER__
	return true;
};
bool	LUA_MGR::luabind_OPEN(){
	if (LUA_STATE==NULL)	return false;
	luabind::open(LUA_STATE);
	return true;
};
//  loadin files using module(char* filename);
bool	LUA_MGR::isFNameInList(const char* filename){
	bool	IN_LIST=false;
	if (filename!=NULL&&strlen(filename)>0){
		int N=H_list.GetAmount();
		while (!IN_LIST&&N--) {
			if (strcmp(H_list[N]->pchar(),filename)==0)	IN_LIST=true;
		};
	};
	return IN_LIST;
};
bool	LUA_MGR::addFNameToList(const char* filename){
	if (filename==NULL||strlen(filename)==0)	return false;
	_str* pstr = new _str(); 
	(*pstr) = filename;
	pCUR_LUA_MGR->H_list.Add(pstr);
	return true;
};
void	LUA_MGR::clearFList(){
	H_list.Clear();
};
bool	LUA_MGR::LOAD_LUA_FILE(const char* filename){
	bool state=false;
	if (LUA_STATE==NULL) return state;
	pCUR_LUA_MGR=this;
	state = load_lua_file(filename);
	pCUR_LUA_MGR=NULL;
	return state;
};
bool	LUA_MGR::load_lua_file(const char* filename){
	if (filename!=NULL&&strlen(filename)>0){
		if (pCUR_LUA_MGR->isFNameInList(filename)==false){
			pCUR_LUA_MGR->addFNameToList(filename);
			int stst = lua_dofile(pCUR_LUA_MGR->LUA_STATE,filename);		
				return (stst==0);
		};
	};
	return false;
};
LUA_MGR* pCUR_LUA_MGR = NULL;
bool	lua_module(const char* fileName){
	if (pCUR_LUA_MGR==NULL) return false;
	return pCUR_LUA_MGR->load_lua_file(fileName);
};
//////////////////////////////////////////////////////////////////////////
#endif//__LUA__