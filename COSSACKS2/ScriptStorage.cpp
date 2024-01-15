#include "stdheader.h"
#include "BE_HEADERS.h"

be_CScriptStore	g_beSCRIPT_STORE("ScriptStorage");

DWORD	be_CScriptStore::GetClassMask(){
	if ( g_beDebuger.getVisible() ) return 0x00000002;
	return 0xFFFFFFFF;
};
void	be_CScriptStore::init(char* name/*=NULL*/){
	Name = ( (name!=NULL&&name[0]!=0) ? (name) : ("ScriptStorageExemple") ); 
};
be_CScriptStore::be_CScriptStore(){
	init();
};
be_CScriptStore::be_CScriptStore(char* name) {
	init(name);
	GetObjectGlobalID();
};
be_CScriptStore::~be_CScriptStore(){
	CLEAR();
};

void	be_CScriptStore::CLEAR(){
	MAIN_SCRIPT.reset_class(&MAIN_SCRIPT);
};

void	be_CScriptStore::CALL_ALL_SCREPTS(){
	MAIN_SCRIPT.CallFunction();	
};

void	be_REG_CLASS_ScriptsStore(){
	REG_CLASS(be_CScriptStore);		
};