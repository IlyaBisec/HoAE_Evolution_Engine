// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "..\..\COSSACKS2\Plugins.h"

// TODO: reference additional headers your program requires here

class VUI_IExchangeInterface: public VUI_IExchange {
public:
	//virtual void SetUnitInfo(OneObject* OB, int* Price);
	virtual void AddOnUnitInfoArray(OneObject* OB);
};
extern VUI_IExchangeInterface IEI;

class DLL_Interface: public GameInterface {
public:
	pTrigger TG;
	ClonesArray<pUnitsList> uList;

	virtual void ProcessingGame();
	virtual void OnUnloading();

	virtual bool OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType);
	virtual void OnDrawOnMapAfterTransparentEffects();

	virtual void OnClassRegistration();
	virtual void OnClassUnRegister();

	//void AddOnUnitInfoArray(OneObject* OB);
	virtual IExchangeInterface*		ExposeMyInterfaces(int InterfaceIndex){
		if(InterfaceIndex==0) return &IEI;
		return NULL;
	}
};
extern "C" __declspec(dllimport) void AddRandNew(char*,int,int);
extern "C" __declspec(dllimport) void AddUN(char* File,int Line,int Param,int Type);
#define addrand(x) AddRandNew(__FILE__,__LINE__,x)
//extern DLL_Interface GI;
