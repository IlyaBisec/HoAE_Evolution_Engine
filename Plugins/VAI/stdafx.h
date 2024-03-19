// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "..\..\COSSACKS2\Plugins.h"

// TODO: reference additional headers your program requires here

class VAI_IExchangeInterface: public VAI_IExchange {
public:
	virtual bool AddSquad(byte NI, GAMEOBJ* Src);
	virtual bool CheckCleanArea(byte NI, iVector2D& A);
	virtual bool CheckProtectBase(byte NI);
	virtual int GetDistFromBase(byte NI, int top);

	virtual void SetSplashBonus(byte NI, word NIndex, int Bonus);
	//void ClearUnitTypeLinks();
	virtual void SetUnitTypeLinks(byte NI, word NIndex, LinearArray<int,_int>* Links);
	
	virtual void OnCastSpell(byte NI, UnitAbility* UA, int x, int y, int r);
	virtual void OnAIStarted(byte NI);

};
extern VAI_IExchangeInterface IEI;

class VAI_GameInterface: public GameInterface {
public:
	//pTrigger TG;
	//ClonesArray<pUnitsList> uList;

	virtual void OnInitAfterMapLoading();
	virtual void OnInitAfterGameLoading();
	virtual void OnEditorStart();

	virtual void ProcessingGame();
	virtual void OnUnloading();
	virtual bool OnMapUnLoading();

	virtual bool OnGameSaving(xmlQuote& xml);
	virtual bool OnGameLoading(xmlQuote& xml);

	//virtual bool OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType);

	//virtual void OnDrawOnMapAfterTransparentEffects();
	virtual void OnDrawOnMapOverAll();

	virtual void OnClassRegistration();
	virtual void OnClassUnRegister();

	virtual IExchangeInterface*		ExposeMyInterfaces(int InterfaceIndex){
		if(InterfaceIndex==0) return &IEI;
		return NULL;
	}
};
//extern VAI_GameInterface GI;


class Iterator_TopInRadius{
private:
	int Layer;
	int BaseTop;
	int X,Y,R;
	DynArray<int> Tops;
	int CurrentTop;
public:
	void Add(int top){
		if( top>=0 && top<0xFFFE ){
			Tops.Add(top);
			int nl=GameInterface::GetTZoneNLinks(Layer,top);
			for(int l=0;l<nl;l++){
				TLinkInfo* TL=GameInterface::GetTZoneLink(0,top,l);
				if( TL && TL->NextAreaID<0xFFFE ){
					int n=Tops.GetAmount();
					for(int i=0;i<n;i++){
						if(Tops[i]==TL->NextAreaID){
							TL=NULL;
							break;
						}
					}
					if(TL){
						int t=TL->NextAreaID;
						iVector2D v=GameInterface::GetTZoneCoor(0,t);
						int r=GameInterface::NORMA(X,v.x,Y,v.y);
						if(r<=R)
							Add(t);
					}
				}
			}
		}
	}
	void Create(int layer, int x, int y, int r){
		Layer=layer;
		X=x;
		Y=y;
		R=r;
		BaseTop=GameInterface::GetTZoneIndex(Layer,X,Y);
		Tops.Clear();
		Add(BaseTop);
		CurrentTop=0;
	};
	int Next(){
		if(CurrentTop<Tops.GetAmount()){
			return Tops[CurrentTop++];
		}
		return -1;
	};
};
extern Iterator_TopInRadius itr_TopInRadius;