// BF.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Moderator.h"
#include "RPG.h"

//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

VAI_IExchangeInterface IEI;
VAI_GameInterface GI;

__declspec(dllexport) BaseClass* GetInterface(){
	return &GI;
}


//////////////////////////////////////////////////////////////////////////

void VAI_GameInterface::OnUnloading() {
	for(int i=0;i<8;i++) BF.M[i].Clear();
	BF.SplashBonus.Clear();
	RPG.Unload();
};

bool VAI_GameInterface::OnMapUnLoading(){
	OnUnloading();
	return true;
};

void VAI_GameInterface::ProcessingGame() {
	for(int i=0;i<8;i++){
		BF.M[i].NI=i;
		BF.M[i].Process();
	}
	RPG.Process();
}

bool VAI_IExchangeInterface::AddSquad(byte NI, GAMEOBJ* Src) {
	if(!(Src&&Src->Type=='UNIT'&&CleanGroup(Src))) return false;
	if(NI<8){
		return BF.M[NI].AddGrp(Src);
	}
	return false; 
}
bool VAI_IExchangeInterface::CheckCleanArea(byte NI, iVector2D& A){
	if(NI<8){
		return BF.M[NI].CheckCleanArea(A);
	}
	return false;
};
bool VAI_IExchangeInterface::CheckProtectBase(byte NI){
	if(NI<8){
		Moderator* m=BF.M+NI;
		if(m){
			int n=m->ProtectBaseMap.GetAmount();
			for(int i=0;i<n;i++){
				if(m->ProtectBaseMap[i])
					return true;
			}
		}
	}
	return false;
};
int VAI_IExchangeInterface::GetDistFromBase(byte NI, int top){
	if(NI<8){
		return BF.M[NI].GetDistFromBase(top);
	}
	return INT_MAX;
}
void VAI_IExchangeInterface::SetSplashBonus(byte NI, word NIndex, int Bonus){
	if(NI<8){
		BF.M[NI].SetSplashBonus(NIndex,Bonus);
	}
};
//void ClearUnitTypeLinks();
void VAI_IExchangeInterface::SetUnitTypeLinks(byte NI, word NIndex, LinearArray<int,_int>* Links){
	if(NI<8){
		BF.M[NI].SetUnitTypeLinks(NIndex,Links);
	}
};

void VAI_IExchangeInterface::OnCastSpell(byte NI, UnitAbility* UA, int x, int y, int r){
	if(NI<8){
		byte NMask=GameInterface::GetNationMask(NI);
		for(int i=0;i<8;i++){
			if( !(NMask&GameInterface::GetNationMask(i)) ){
				BF.M[i].OnCastSpell(UA,x,y,r);
			}
		}
	}
};
void VAI_IExchangeInterface::OnAIStarted(byte NI){
	if(NI<8){
		RPG.OnAIStarted(NI);
	}
};

void VAI_GameInterface::OnClassRegistration() {
	
	REG_CLASS(Battlefield);
	REG_CLASS(Moderator);
	REG_CLASS(Squadron);
	REG_CLASS(Squad);
	REG_CLASS(SquadRef);

	AddStdEditor("Battle Field",&BF,"",RCE_DEFAULT3,NULL,NULL,'V');

	REG_CLASS(RPG_AI);
	REG_CLASS(RPG_AI_Editor);
	REG_CLASS(RPG_AI_Group);
	//REG_CLASS(RPG_AI_Action);
	REG_CLASS(RPG_AI_Condition);
	REG_CLASS(RPG_AI_Object);
	REG_CLASS(ai_HeroBase);
	REG_CLASS(ai_Hero);

	REG_CLASS(RPG_Unit);
	REG_CLASS(RPG_U_Bonus);
	REG_CLASS(RPG_U_Chest);
	REG_CLASS(RPG_U_Settlement);
	REG_CLASS(RPG_U_Shop);
	REG_CLASS(RPG_U_Creep);
	REG_CLASS(RPG_U_Friend);
	REG_CLASS(RPG_U_Enemy);
	REG_CLASS(RPG_U_Battle);

	AddStdEditor("RPG AI",&RPG,"",RCE_DEFAULT3,NULL,NULL,'R');

}
void VAI_GameInterface::OnClassUnRegister() {
	UNREG_CLASS("Battlefield");
	UNREG_CLASS("Moderator");
	UNREG_CLASS("Squadron");
	UNREG_CLASS("Squad");
	UNREG_CLASS("SquadRef");
};

void VAI_GameInterface::OnDrawOnMapOverAll() {

	if(BF.ShowDebugInfo){
		rsQuad(Vector3D(100,100,100),Vector3D(300,100,100),Vector3D(100,300,100),Vector3D(300,300,100),0x80FFFFFF,0x80FFFF00,0xF000FFFF,0x8000FF00);
		rsFlush();
		Squad* Sq=BF.CurSquad.Get();
		if(Sq){
			//Sq->Body.Select(GetMyNation(),false);
			GameInterface::DrawUnitsRanges(Sq->Body);

			int n;

			//n=Sq->TargetWay.GetAmount();
			//if(n>0){
			//	iVector2D vTarg=GameInterface::GetTZoneCoor(0,Sq->TargetWay[n-1]);
			//	Vector3D V0(Sq->Center.x,Sq->Center.y,100);
			//	V0-=Vector3D(80,80,-200);
			//	GameInterface::DrawSmartArrow(iVector2D(300,300),iVector2D(700,700));
			//	rsFlush();
			//}	

			int T=GameInterface::GetGlobalTime();
			Moderator* She=Sq->Shepherd.Get();

			n=Sq->DangerMap.GetAmount();
			for(int i=0;i<n;i++){
				iVector2D v2d=GameInterface::GetTZoneCoor(0,i);
				Vector3D v(v2d.x,v2d.y,0);
				GameInterface::WorldToScreenSpace(v);
				char txt[128];
				v2d.x=v.x-40;
				v2d.y=v.y;
				bool top=false;

				int danger=Sq->DangerMap[i];
				int CreepIndex=-1;
				RPG_U_Creep* UC=NULL;
				if( i<RPG.CreepIndex.GetAmount() && RPG.CreepIndex[i]!=0xFFFF ){
					//UC=RPG.Creeps[RPG.CreepIndex[i]];
					CreepIndex=RPG.CreepIndex[i];
				}
				
				if( danger>0 || CreepIndex!=-1 ){
					top=true;
					sprintf(txt,"Top: %d",i);
					GameInterface::DrawString(txt,v2d,0xFFDDCCEE);
					v2d.y-=20;
				}
				if( danger>0 ){
					sprintf(txt,"Danger: %d",danger);
					GameInterface::DrawString(txt,v2d,0xFFEE3333);
					v2d.y-=20;
				}
				if( CreepIndex!=-1 ){
					sprintf(txt,"Creeps: %d",CreepIndex);
					GameInterface::DrawString(txt,v2d,0xFF5533EE);
					v2d.y-=20;
				}
				if( She && T<She->SpellDangerTime[i] ){
					sprintf(txt,"SpellTime: %d (%d)",She->SpellDangerTime[i]-T,She->SpellDanger[i]);
					GameInterface::DrawString(txt,v2d,0xFFFF8888);
					v2d.y-=20;
				}
				if(Sq->TargetMap[i]!=0xFFFF){
					sprintf(txt,"Target",Sq->TargetMap[i]);
					GameInterface::DrawString(txt,v2d,0xFFFF2222);
					v2d.y-=20;
				}
				if(Sq->Top==i){
					sprintf(txt,"CurrentTop",Sq->TargetMap[i]);
					GameInterface::DrawString(txt,v2d,0xFF22FF22);
					v2d.y-=20;
				}
				int nw=Sq->TargetWay.GetAmount();
				for(int w=0;w<nw;w++){
					if(Sq->TargetWay[w]==i){
						if(!top){
							sprintf(txt,"Top: %d",i);
							GameInterface::DrawString(txt,v2d,0xFFDDCCEE);
							v2d.y-=20;
							top=true;
						}
						sprintf(txt,"WayPoint: %d",w);
						GameInterface::DrawString(txt,v2d,0xFF88DDEE);
						v2d.y-=20;
						//break;
					}
				}
			}
			rsFlush();
			ISM->Flush();
		}
	}
	
};

bool VAI_GameInterface::OnGameSaving(xmlQuote& xml){
	xmlQuote* bf=new_xmlQuote("BattleField");
	BF.Save(*bf,&BF);
	xml.AddSubQuote(bf);
	return true;
}
bool VAI_GameInterface::OnGameLoading(xmlQuote& xml){
	ErrorPager Error;
	xmlQuote* bf=xml.GetSubQuote("BattleField");
	if(bf){
		BF.reset_class(&BF);
		BF.Load(*bf,&BF,&Error);
	}
	return true;
}
void VAI_GameInterface::OnInitAfterMapLoading(){
	for(int i=0;i<8;i++)
		BF.M[i].InitProcess();
};
void VAI_GameInterface::OnInitAfterGameLoading(){
	for(int i=0;i<8;i++)
		BF.M[i].InitProcess();
};

void VAI_GameInterface::OnEditorStart(){
	for(int i=0;i<8;i++)
		BF.M[i].InitProcess();
};