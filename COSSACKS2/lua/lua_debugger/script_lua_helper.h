#pragma once

extern "C"
	{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
	}
#include <luabind.hpp>

struct lua_State;
struct Proto;
struct lua_Debug;
class CScriptFile;

class CDbgLuaHelper  
{
public:
	void		RestoreGlobals				();
	void		CoverGlobals				();
	void		Describe					(char* szRet, int nIndex);
	bool		Eval						(const char* szCode, char* szRet);
	bool		GetCalltip					(const char *szWord, char *szCalltip);
	void		DrawGlobalVariables			();
	void		DrawLocalVariables			();
	const char* GetSource					();
/*
	static BOOL ErrorStringToFileLine(CString strError, CString &strPathName, int &nLine);
*/
#ifdef SCRIPT_FILE
	static BOOL LoadDebugLines(CScriptFile* pPF);
#endif

	CDbgLuaHelper							();
	virtual ~CDbgLuaHelper					();

	// debugger functions
	int			PrepareLua					(lua_State* );
	void		UnPrepareLua				(lua_State* ,int);
	void		PrepareLuaBind				();


	void		DrawStackTrace				();
	static int  OutputTop					(lua_State* );

	static void hookLua						(lua_State *, lua_Debug *);
	static int  hookLuaBind					(lua_State *);

	static int	errormessageLua				(lua_State* );
	static void errormessageLuaBind			(lua_State* );
	static void line_hook					(lua_State *, lua_Debug *);
	static void func_hook					(lua_State *, lua_Debug *);
	static void set_lua						(lua_State *);
	void		DrawVariable				(lua_State * l, const char* name, bool bOpenTable);
	void		DrawTable					(lua_State *l, const char* name, bool bRecursive=true);
	void		DrawVariableInfo			(char*);
protected:
	static		CDbgLuaHelper*				m_pThis;


	static lua_State*						L;
	lua_Debug*								m_pAr;
};
