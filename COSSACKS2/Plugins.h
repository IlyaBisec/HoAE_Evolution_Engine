
// 
// Этот файл необходимо включать только в dll-плагины
//

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
typedef unsigned short word;
typedef unsigned char byte;

class Brigade{
};

#include "common.h"
#include "NewAnimation.h"
#include "Weapon.h"
#include "StructuresExtensions.h"
#include "ExIcons.h"
#include "NewMonster.h"
#include "OneSprite.h"
#include "vui_Action.h"
#include "vui_InterfSynchro.h"
#include "GameExtension.h"
#include "GameInterface.h"

#include "UnitsInterface.h"
#include "cvi_HeroButtons.h"

#include "OneObject.h"
#include "Iterators.h"

//
#include "IRenderSystem.h"
#include "rsRenderSystem.h"
#include "IMediaManager.h"
#include "IResourceManager.h"
#include "vMesh.h"
//

#include "supereditor.h"
#include "UnitAbility.h"

#include "mesh\UniHash.h"

#include "PluginsInterface.h"

void StartDialogLink(OneObject* OB);
bool StartDialog(OneObject* OB, byte OrdType, OneObject* DialogOB);

// TODO: reference additional headers your program requires here
