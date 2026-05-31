
#pragma	once

// class C++ -> Lua //////////////////////////////////////////////////////

// binde lvCNode to lua
void	bind_be_CNODE(lua_State* L);

// binde lvCGroup to lua
void	bind_be_CGROUP(lua_State* L);

// binde GroupMAP to lua
void	bind_groupMAP(lua_State* L);

// binde all vvBASE inherit class
void	bind_valuesMAP(lua_State* L);

// binde all lvCGraphObject inherit class
void	bind_GraphObjMAP(lua_State* L);

// binde LUA function caller
void	bind_be_CLUA_FN_STORE(lua_State* L);

// binde HashTop table for orientation on map
void	bind_be_CHashTopMAP(lua_State* L);

// binde External Classes from "GameInterface.h"
void	bind_GameInterface(lua_State* L);

//////////////////////////////////////////////////////////////////////////

