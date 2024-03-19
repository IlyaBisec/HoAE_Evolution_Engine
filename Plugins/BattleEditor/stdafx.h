// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
typedef unsigned short word;
typedef unsigned char byte;

class Brigade{
};


#include "..\..\COSSACKS2\common.h"
#include "..\..\COSSACKS2\NewAnimation.h"
#include "..\..\COSSACKS2\Weapon.h"
#include "..\..\COSSACKS2\StructuresExtensions.h"
#include "..\..\COSSACKS2\ExIcons.h"
#include "..\..\COSSACKS2\NewMonster.h"
#include "..\..\COSSACKS2\OneSprite.h"
#include "..\..\Cossacks2\vui_Action.h"
#include "..\..\Cossacks2\vui_InterfSynchro.h"
#include "..\..\Cossacks2\GameExtension.h"
#include "..\..\Cossacks2\GameInterface.h"
#include "..\..\Cossacks2\be_HashTop.h"
//
#include "IRenderSystem.h"
#include "IMediaManager.h"
#include "IResourceManager.h"
#include "vMesh.h"
//

// Internal Headers
typedef	GameInterface	GI;

#include "Macros.h"

#include "Helper.h"

#include "expFN_Mission.h"
#include "expFN_Group.h"
#include "expFN_Node.h"
#include "expFN_Get.h"
#include "expFN_Set.h"
#include "expFN_Ability.h"
#include "expFN_Text.h"

#include "fsm_States.h"
#include "fsm_Guard.h"
#include "fsm_Attack.h"
#include "fsm_Produce.h"
#include "fsm_Dialog.h"
#include "fsm_Saboteur.h"

extern "C" __declspec(dllimport) void AddRandNew(char*,int,int);
extern "C" __declspec(dllimport) void AddUN(char* File,int Line,int Param,int Type);
#define addrand(x) AddRandNew(__FILE__,__LINE__,x)

//////////////////////////////////////////////////////////////////////////




















