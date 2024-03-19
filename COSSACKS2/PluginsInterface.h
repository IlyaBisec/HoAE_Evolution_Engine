
#pragma once

#include "GameInterface.h"

// dll-s exchange management	
// if you want to expose(share) your interface, you should create

#define GUID_VAI_IExchange 0x12340000
#define GUID_VUI_IExchange 0x12340001

class VAI_IExchange: public IExchangeInterface {
public:	
	virtual DWORD GetUniqID() { return GUID_VAI_IExchange; }

	virtual bool AddSquad(byte NI, GAMEOBJ* Src) = 0;
	virtual bool CheckCleanArea(byte NI, iVector2D& A) = 0;
	virtual bool CheckProtectBase(byte NI) = 0;
	virtual int GetDistFromBase(byte NI, int top) = 0;

	virtual void SetSplashBonus(byte NI, word NIndex, int Bonus) = 0;
	//void ClearUnitTypeLinks();
	virtual void SetUnitTypeLinks(byte NI, word NIndex, LinearArray<int,_int>* Links) = 0;

	virtual void OnCastSpell(byte NI, UnitAbility* UA, int x, int y, int r) = 0;
	virtual void OnAIStarted(byte NI) = 0;
};
inline VAI_IExchange* VAI(){
	static VAI_IExchange* IE=(VAI_IExchange*)GameInterface::GetExchangeInterface(GUID_VAI_IExchange);
	if(!IE){
		GameInterface::LogWarning("Can not load: Plugins\\VAI.dll");
	}
	return IE;
}

class VUI_IExchange: public IExchangeInterface {
public:	
	virtual DWORD GetUniqID() { return GUID_VUI_IExchange; }

	//virtual void SetUnitInfo(OneObject* OB, int* Price) = 0;
	virtual void AddOnUnitInfoArray(OneObject* OB) = 0;
};
inline VUI_IExchange* VUI(){
	static VUI_IExchange* IE=(VUI_IExchange*)GameInterface::GetExchangeInterface(GUID_VUI_IExchange);
	if(!IE){
		GameInterface::LogWarning("Can not load: Plugins\\VUI.dll");
	}
	return IE;
}
