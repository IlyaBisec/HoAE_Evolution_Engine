#ifndef __LUA__MANAGER__
#define __LUA__MANAGER__

#ifdef  __LUA__
// LUA CLASS /////////////////////////////////////////////////////////////
bool	lua_module(const char* fileName);
class LUA_MGR : public BaseClass
{
public:
	LUA_MGR();
	~LUA_MGR();
public:
	lua_State*	LUA_STATE;	

	// 	Function for global operation with lua
	bool	OPEN();			// Open lua
	bool	CLOSE();		// Close lua
	bool	BIND( void ( *BindFunction )(lua_State*) );	// bind function and class to lua state

	bool	useDEBUGER;
	bool	DEBUG_PREP();	// prepare lua debuger for cur lua state

	bool	LOAD_LUA_FILE(const char* filename);		// Load file in lua state

public:
	bool	luabind_OPEN();			// open luabind for cur lua state	
	bool	load_lua_file(const char* filename);

	//  loadin files using module(char* filename);
	ClonesArray<_str>	H_list;		// List of hiders file using for create lua state
	bool				isFNameInList(const char* filename);	// check if allredy loaded
	bool				addFNameToList(const char* filename);	// add new file name in .h liast
	void				clearFList();
};
extern	LUA_MGR* pCUR_LUA_MGR;
//////////////////////////////////////////////////////////////////////////
#endif//__LUA__

#endif//__LUA__MANAGER__