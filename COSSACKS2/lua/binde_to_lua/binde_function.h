
#pragma	once

// functions C++ -> Lua //////////////////////////////////////////////////

// base lua function in defoult lib
void	lua_base(lua_State* L);

// functions from "ActiveScenary.cpp"
void	bind_ActiveScenary(lua_State* L);

// function from Condition List
void	bind_Condition(lua_State* L);

// function from Operation List
void	bind_Operation(lua_State* L);

//////////////////////////////////////////////////////////////////////////