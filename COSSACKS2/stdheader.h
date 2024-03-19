#ifndef __STDHEADER_H__
#define __STDHEADER_H__

//////////////////////////////////////////////////////////////////////////
//#define _DEMO_
//#define _PRESSMAN_
//////////////////////////////////////////////////////////////////////////

#define CEXPORT __declspec(dllexport)
#define CIMPORT __declspec(dllimport)
#include "..\CommCore\CommCore.h"
#include "stdafx.h"
#include <malloc.h>
#include <direct.h>				// Vitya
#include "Limitations.h"
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "GP_Draw.h"
#include <IMediaManager.h>
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "multipl.h"
#include "fog.h"
#include "Nature.h"
#include <time.h>
#include "Nucl.h"
#include "Megapolis.h"
#include "Dialogs.h"
#include "fonts.h"
#include "GSound.h"
#include "3DGraph.h"
#include "3DMapEd.h"
#include "MapSprites.h"
#include "smart_assert.h"
#include <math.h>
#include "NewMon.h"
#include "IconTool.h"
#include "3DRandMap.h"
#include "path.h"
#include <crtdbg.h>
#include "ActiveScenary.h"
#include "DrawForm.h"
#include "Conststr.h"
#include <Process.h>
#include "MPlayer.h"
#include "Recorder.h"
#include "GSINC.H"
#include "TopoGraf.h"
#include "PeerClass.h"
#include "CEngine\goaceng.h"
#include "StrategyResearch.h"
#include "Safety.h"
#include "EinfoClass.h"
#include "mode.h"
#include "ClassEditor.h"
#include <kHash.hpp>
#include "MapTemplates.h"
#include "VideoLayer.h"
#include "cdirsnd.h"
#include <typeinfo.h>
#include "GameSettings.h"
#include "Language.h"
#pragma pack (push)
//#pragma pack (8)

#include "Scape3D.h"
#include "assert.h"

#pragma pack (pop)
#include "3DBars.h"
#include "ZBuffer.h"
#include "Transport.h"
#include "NewUpgrade.h"
#include "NewAI.h"
#include "RealWater.h"
#include "Sort.h"

#include "vital\vital_def.h"
#include "GlobalActions.h"
#include "ZonesAndGroupsExtraInfo.h"	//Vitya
#include "diplomacy.h"
#include "Dialogs\AllDialogs.h"
#define IDIRECTPLAY2_OR_GREATER
#include "dplay.h"
#include "dplobby.h"
#pragma pack(4)
#include "InetRaiting\stats\gstats.h"
#include "InetRaiting\stats\gpersist.h"
#include "InetRaiting\gp.h"
#pragma pack(1)
#include "IR.H"
#include "bmptool.h"

#include "AttackList.h" 

#include "ActiveZone.h"
#include "Graphs.h"
#include "Danger.h"
#include "3DSurf.h"

//#include "UnitsInterface.h"
#include "vui_info.h"
//#include "vui_Actions.h"
#include "vui_InterfSynchro.h"
#include "vui_IS.h"

//#include "cvi_MainMenu.h"
#include "FastMemPool.h"
#include "musorozbornik.h"
#include "GameExtensionInternal.h"

#include "Iterators.h"

#include "mTransform.h"
#include <IMediaManager.h>

extern CEXPORT int RealLx;
extern CEXPORT int RealLy;

#include "..\DipServer\BasicDiploRelation.h"
#include "..\DipServer\DIP_SimpleBuilding.h"
#include "..\DipServer\DiplomacySystem.h"
CIMPORT void GetDipSimpleBuildings(int &NDips, DIP_SimpleBuilding** &Dips);

extern DIALOGS_API bool KeyPressed;
extern DIALOGS_API int LastKey;
void PrintAllLeaks();
void SetLeakMode(bool Mode);
int GetTotalHeight(int x,int y);

class EditDialog:public BaseClass{
public:
	DialogsSystem Dialog;
	SAVE(EditDialog)
		REG_AUTO(Dialog);
	ENDSAVE;
};

extern	void	CREATE_SCRIPT();	//Vitya	//  [11/4/2003]
#define MaxNatColors 7
#define KeyNatColor 7
DIALOGS_API void VitalError(char* Mess);
_inline float sqrt(int x){return sqrt(float(x));}
_inline float atan2(int x,int y){return atan2(float(x),float(y));}

#include "Instrument.h"
extern City CITY[8];

#include "Weapon.h"
#include "BrigadeOrders.h"

#include "SuperEditor.h"
#include "QParser.h"

extern CEXPORT int ItemChoose;

extern int TrueTime;

#include "BE_HEADERS.h"
#include "MonsterAbility.h"

#define ALLOW_TIMING

#ifdef ALLOW_TIMING

__forceinline void __ptime(LARGE_INTEGER* LI){
    __asm{
        rdtsc;
        mov     ebx,DWORD PTR[LI]
        mov     DWORD PTR[ebx],eax;
        mov     DWORD PTR[ebx+4],edx;
    }
}
extern int NDX_calls;

#define __beginT() LARGE_INTEGER LI0;__ptime(&LI0);NDX_calls++;
#define __endT(tvar) LARGE_INTEGER LI1;__ptime(&LI1);LI1.QuadPart-=LI0.QuadPart;tvar+=LI1.LowPart;


#else //ALLOW_TIMING

#define __beginT()
#define __endT(tvar)

#endif //ALLOW_TIMING

#endif //__STDHEADER_H__