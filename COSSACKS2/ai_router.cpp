#include "stdheader.h"
#include "Extensions.h"
#include "CurrentMapOptions.h"

class OneScriptString:public BaseClass{
public:
	_str ScriptName;
	SAVE(OneScriptString);
		REG_FILEPATH(ScriptName,"*.ai.xml");
	ENDSAVE;
	bool ForceSimplification(){return true;}
};
class OneAI_route:public BaseClass{
public:
	OneAI_route(){
		Nation = 0;
		CheckStartResources = false;
		StartResources = 1000;
		LandType = 0;
		AIDifficulty = 0;
		AIGameType = 0;
		MapName = "";
	}
	int Nation;
	bool CheckStartResources;
	int StartResources;
	int LandType;
	int VictoryCondition;
	int AIDifficulty;
	int AIGameType;
	_str MapName;
	ClonesArray<OneScriptString> AI_scripts;
	SAVE(OneAI_route);
		REG_ENUM(_index,Nation,NATIONS);
		REG_MEMBER(_bool,CheckStartResources);
		SAVE_SECTION(1);
		REG_MEMBER(_int,StartResources);
		SAVE_SECTION(0xFFFFFFF);
		REG_ENUM(_index,LandType,LANDTYPE);
		REG_ENUM(_index,VictoryCondition,AI_CheckVictoryCondition);
		REG_ENUM(_index,AIDifficulty,DIFFICULTIES);
		REG_ENUM(_index,AIGameType,GAMETYPES);
		REG_FILEPATH(MapName,"*.m3d");
		REG_AUTO(AI_scripts);
	ENDSAVE;
	DWORD GetClassMask(){
		return CheckStartResources?0xFFFFFFFF:0xFFFFFFFE;
	}
	const char* GetThisElementView(const char* LocalName){
		// Not updated! Basically, this method is unuseful
		char* s=GetGlobalBuffer();
		s[0]=0;
		Enumerator* EN=ENUM.Get("NATIONS");
		Enumerator* EL=ENUM.Get("LANDTYPE");
		Enumerator* ED=ENUM.Get("DIFFICULTIES");
		Enumerator* EG=ENUM.Get("GAMETYPES");
		if(EN&&EL&&ED&&EG){
			if(CheckStartResources && !MapName.equal("")) sprintf(s,"{CR}%s{CB} res=%d{CY} Land=%s Difficulty=%s GameType=%s Map Name=%s",EN->Get(Nation),StartResources,EL->Get(LandType), ED->Get(AIDifficulty), EG->Get(AIGameType), MapName);
			else if (CheckStartResources) sprintf(s,"{CR}%s{CB} res=%d{CY} Land=%s Difficulty=%s GameType=%s",EN->Get(Nation),StartResources,EL->Get(LandType), ED->Get(AIDifficulty), EG->Get(AIGameType));
			else sprintf(s,"{CR}%s{CB} res=ANY{CY} Land=%s Difficulty=%s GameType=%s",EN->Get(Nation),EL->Get(LandType), ED->Get(AIDifficulty), EG->Get(AIGameType));
			return s;
		}
		return NULL;
	}
};
ClonesArray<OneAI_route> AI_Router;
void RegAI_router(){
	REG_CLASS(OneScriptString);
    REG_CLASS(OneAI_route);
	AI_Router.SafeReadFromFile("ai\\router.xml");
	AddStdEditor("AI router",&AI_Router,"ai\\router.xml",RCE_DEFAULT);
}
int ConvertNationToSkirmish(int NatID){
	extern char** NatNames;
	Enumerator* E=ENUM.Get("NATIONS");
	char NName[32];
	strcpy(NName,NatNames[NatID]);
	strupr(NName);
	int v=E->Get(NName);
	return v;
}
bool GetHero(OneObject* OB);
DLLEXPORT int GetRND(int Max);
char* GetBestAI(int NI,char* mapname){
	bool IsIsl=false;
	int diff = CITY[NI].Difficulty+1;
	for(int i=0;i<7;i++)if(i!=NI&&CITY[i].MyIsland!=0xFF){
		if(CITY[i].MyIsland!=CITY[NI].MyIsland)IsIsl=true;
	}
	int Nat=-1;
	for(int j=0;j<MAXOBJECT;j++){
		OneObject* OB=Group[j];
		if(OB&&OB->NNUM==NI&&!OB->Sdoxlo&&(OB->newMons->Peasant||OB->NewBuilding||GetHero(OB))){
			Nat=OB->Ref.General->NatID;
			break;
		};
	};
	if(Nat>=0){
		int stres=XRESRC(NI,0);
		char* ais[512];
		int nai=0;
		for(int i=0;i<AI_Router.GetAmount();i++){
			OneAI_route* AR=AI_Router[i];
			if(AR){
				int v=ConvertNationToSkirmish(Nat);
				bool yes=(AR->Nation==v);
				if(AR->CheckStartResources){
					yes&=stres==AR->StartResources;
				}
				if(AR->LandType){
					yes&=bool(AR->LandType-1)==IsIsl;
				}
				if(AR->VictoryCondition){
					VictoryConditionRoot* VT=MOptions.VictoryRule.Get();
					if(VT){
						const char* CC=VT->GetClassName();
						if(AR->VictoryCondition==1&&strcmp(CC,"VC_DestroyTownHalls"))yes=false;
						if(AR->VictoryCondition==2&&strcmp(CC,"VC_ScoreGame"))yes=false;
						if(AR->VictoryCondition==3&&strcmp(CC,"VC_CaptureGame"))yes=false;
						if(AR->VictoryCondition==4&&strcmp(CC,"VC_AnnihilateHero"))yes=false;
					}else yes=false;
				}
				if(AR->AIDifficulty != 0){
					yes&=AR->AIDifficulty==diff;
				}
				if(AR->AIGameType != 0){
					yes&=AR->AIGameType==(GSets.RoomParams.RTS_mode + 1);
				}
				if(!AR->MapName.equal("")){
					yes&=AR->MapName.equal(mapname);
				}
				if(yes){
					for(int i=0;i<AR->AI_scripts.GetAmount();i++){
						if(nai<512){
							ais[nai]=AR->AI_scripts[i]->ScriptName.pchar();
							nai++;
						}
					}
				}
			}
		}
		if(nai){
			int s=GetRND(nai);
			//int s=0;
			//int L=strlen(mapname);
			//for(int i=0;i<L;i++){
			//	s+=mapname[i];
			//}
			//s%=nai;
			return ais[s];
		}
	}
	return NULL;
}