// PluginSample.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "ChessBattle.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
class DLL_Interface:public GameInterface{
public:
	ClonesArray<pUnitsList> uList;
	virtual void ProcessingGame();
    virtual void OnDrawOnMapAfterTransparentEffects(){
        void DrawTreesEditor();
        DrawTreesEditor();
    }
    virtual bool OnCheatEntering(const char* Cheat){
        if(!strcmp(Cheat,"edtree")){
            void CallTreesEditor();
            CallTreesEditor();
        }
        return false;
    }
	pTrigger TG;
};
//////////////////////////////////////////////////////////////////////////
DLL_Interface GI;
__declspec(dllexport) BaseClass* GetInterface(){
	return &GI;
}
class MyOrder:public IGroupOrder{
public:
	virtual bool Process(){
		return true;
	}
};
//////////////////////////////////////////////////////////////////////////
class SampClass:public BaseClass{
public:
    int v;
    bool x;
    SAVE(SampClass);
        REG_MEMBER(_int,v);
        REG_MEMBER(_bool,x);
    ENDSAVE;
};
void DLL_Interface::ProcessingGame(){
    /*
    SampClass S;
    S.v-123;
    S.x=true;
    S.WriteToFile("1.xml");
    S.SafeReadFromFile("1.xml");
    if(rand()<256)CreateSpecialEffect("Druid_shot",rand()%1023,rand()%1023,70,rand()%1023,rand()%1023,70);	
	pUnitType T("DruUPik(DR)");
	if(rand()<1){
		pUnit p=CreateUnit(rand()&1023,rand()&1023,rand()&7,T);
		ApplyAbility("UA_ClayGolem_Regeneration",p,p);
	}
	if(rand()<1){
		pUnitsList* L=new pUnitsList;
        uList.Add(L);
		CreateRandomUnits(10,rand()&1023,rand()&1023,80,rand()&7,T,L);
		L->AddNewOrder(new MyOrder);
	}
	if(TG.DoOnce()){
		pDialogInfo D=GetDialog("DLG1");
		D.Show();
		pUnit u1=CreateUnit(100,100,0,pUnitType("DruUPik(DR)"),true);
		pUnit u2=CreateUnit(200,120,0,pUnitType("DruUPik(DR)"),true);
		D.ShowQuestionNearUnit(u1);
		D.ShowAnswerNearUnit(u2);
	}
    */
    if(rand()<256){
        pGroup pg("mygrp");
        ForEachUnit(pg,unit.PushOutAllUnits());
    }
}

