#ifndef	__BE_HEADERS__
#define	__BE_HEADERS__

// COSSACKS2
//#include "../stdheader.h"
#include "TestEditor.h"
#include "GameExtension.h"				
#include "Save_XML_ToMap.h"				
//#include "AI_Scripts.h"
#include "ClassEditorsRegistration.h"
#include "ActiveZone.h"

#include "vui_Action.h"
#include "vui_Effects.h"

#include "AlertOnMiniMap.h"

// MANOWAR
#include "mp3\oggvor.h"

// MESSAGES
#include "HelpSystem.h"
#include ".\cvi_RomeHelp.h"
//#include ".\cvi_InGamePanel.h"

//--------------------
// - ADDED BY SILVER -
#undef max
#undef min
//--------------------

// Lua ///////////////////////////////////////////////////////////////////
#include	<lua_define.hpp>
#ifdef  __LUA__ 
	#include <lua_header.h>
	#ifdef  __LUA_DEBUGGER__ 
		#include <lua_debugger.h>
	#endif//__LUA_DEBUGGER__
#endif//__LUA__

// IPORT
#include "HashTop.h"
#include "TopoGraf.h"
#include "be_Import.h"
#include "be_HashTop.h"

// BASE USAGE CLASSES
#include "BE_BaseClasses.h"

// MISS GRAPH
#include "MissGraphObjects.h"

//AI
extern int TrueTime;
#include "ai_ex\FSMmsg.h"
#include "ai_ex\FSMstate.h"
#include "ai_ex\FSMclass.h"

// LEGO
#include "Parameter.h"
#include "Function.h"
#include "ComplexFunctionMode.h"
#include "ComplexFunction.h"
#include "LUA_Manager.h"
#include "BaseVariables.h"
#include "Helper_var.h"
#include "ValueStorage.h"
#include "ParamFunction.h"
#include "FunctionStorage.h"
#include "ScriptStorage.h"

// BATTLE EDITOR
#include "BE_ToolsManager.h"
#include "Debuger.h"
#include "ClassDeffIDS.h"				

#include "MessageMgr.h"

#include "BE_OPERATIONS.h"

#include "DataStorageXML.h"				
#include "BattlePainter.h"
#include "ENUM_Storage.h"

// BE2
#include "ClassTypeList.h"
#include "BE_Value.h"
#include "BE_Block.h"
#include "BE_Global.h"

#include "ProcessBattleEditor.h"

#include "MainInterface.h"

// std
#include <stdio.h>
#include <stdarg.h>

// vital
#include "UnitChar.h"

#endif//__BE_HEADERS__
