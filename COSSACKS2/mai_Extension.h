#pragma once
//////////////////////////////////////////////////////////////////////////
#include "MapTemplates.h"
#include "CurrentMapOptions.h"
#include "GameExtension.h"
#include ".\mai_vanguard.h"
#include ".\mai_monitor.h"
//
extern MapOptions MOptions;
//////////////////////////////////////////////////////////////////////////
//extern ClonesArray<mai_VanGuard> mai_VanGuards;
//////////////////////////////////////////////////////////////////////////
class mai_Extension: public GameExtension
{
public:
	mai_Extension(void);
	~mai_Extension(void);
	//
	virtual void ProcessingGame();
	//
	virtual void OnBrigadeCreated(Brigade* BR);
	virtual void OnBrigadeFreedManually(Brigade* BR);
	virtual void OnBrigadeKilled(Brigade* BR,byte KillerNation);
	//
	virtual void OnDrawOnMapAfterTransparentEffects();
	virtual void OnDrawOnMiniMap(int x,int y,int Lx,int Ly);
	virtual bool OnMouseHandling(int mx,int my,bool& LeftPressed,bool& RightPressed,int MapCoordX,int MapCoordY,bool OverMiniMap);
};

//////////////////////////////////////////////////////////////////////////
//int mai_getUnitForce(AdvCharacter* ADC, AdvCharacter* Enm);
//bool mai_getAverageUnit(UnitsGroup* UG, AdvCharacter& ADC, int& N);
//bool mai_getAverageUnit(byte NI, AdvCharacter& ADC, int& N);
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


