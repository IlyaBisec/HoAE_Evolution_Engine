// luabind.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

extern "C"
{
#include "../lua\\include\\lua.h"
#include "../lua\\include\\lualib.h"
#include "../lua\\include\\lauxlib.h"
}


//#define	LUABIND_BUILDING

//#define LUABIND_EXPORT __declspec(dllexport)
//#define LUABIND_IMPORT __declspec(dllimport)

#include "luabind\\luabind.hpp"

//BOOL APIENTRY DllMain( HANDLE hModule, 
//                       DWORD  ul_reason_for_call, 
//                       LPVOID lpReserved
//					 )
//{
//    return TRUE;
//}

