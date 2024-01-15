#include "CommonDip.h"
#include "UnitsGroup.h"
#include "Mind.h"
#include "Script.h"
#include "..\cossacks2\smart_assert.h"

CPlayerScript PScript[8];

CIMPORT char* GetCurMissionMapName();
CEXPORT
void LoadScript(){
	for(int i=0;i<8;i++) PScript[i].Free();
	char* MN=GetCurMissionMapName();
	if(MN){
		char ScName[64];
		strcpy(ScName,MN);
		int L=strlen(ScName);
		ScName[L-3]='s';
		ScName[L-2]='c';
		ScName[L-1]='r';
		GFILE* F=Gopen(ScName,"r");
		if(F){
			char line[256];
			byte curp=0xFF;		// current player
			int curm=0;			// current mode
			//byte dif;			// difficalty

			CIMPORT byte* GetBattleArmyValue();
			byte* AV=GetBattleArmyValue();

			while(ReadWinString(F,line,256)){
				if(line[0]=='/'&&line[1]=='/') continue;
				if(strstr(line,"[Player")){
					sscanf(strstr(line,"[Player")+7,"%d",&curp);
					curm=0;					
				}else
				if(	strstr(line,"[DifBonus]")||
					strstr(line,"[ArmyBonus]")){
					curp=0xFF;
					curm=0;
				}else
				if(AV && strstr(line,"[ArmyValue]")){
					curp=0xFF;
					curm=4;
					memset(AV,1,GetNMon(0));
				}else
				if(curm==4){
					char buf[64];
					int n;
					if(sscanf(line,"%s%d",buf,&n)==2){
						GAMEOBJ ut;
						if(RegisterUnitType(&ut,buf)){
							AV[ut.Index]=n;
						};
					}
				}else
				if(curp<8){
					if(strstr(line,"[Defeat]")){
						curm=1;
					}else
					if(strstr(line,"[AIUpg")){
						byte dif=0xFF;
						sscanf(strstr(line,"[AIUpg")+6,"%d",&dif);
						if(GetDiff(curp)==dif){
							curm=2;
						}else{
							curm=0;
						}
					}else
					if(strstr(line,"[UpgEnabled]")){
						curm=3;
					}else
					if(curm){
						CPlayerScript* PS=PScript+curp;
						if(curm==1){
							char type[64]; 
							type[0]=0;
							if(sscanf(line,"%s",type)&&type[0]){
								GAMEOBJ ut;
								if(RegisterUnitType(&ut,type)){								
									PS->Enable=1;
									PS->AddProtect(ut.Index);				
								}
							}
						}else
						if(curm==2||curm==3){
							char upg[128];
							upg[0]=0;
							if(sscanf(line,"%s",upg)&&upg[0]){
								GAMEOBJ up;
								if(RegisterUpgrade(&up,upg)){
									if(curm==2)
										PS->AIUpg[up.Index]=1;
									else if(curm==3)
										PS->UpgEnabled[up.Index]=1;
								}
							}
						}
					}				
				}
			};
			Gclose(F);

		}else{
			MessageBox(NULL,ScName,"Loading script FAILED...",MB_ICONWARNING|MB_OK);
			assert(false);
		}
	}else{
		MessageBox(NULL,"GetCurMissionMapName()","Init mission FAILED...",MB_ICONWARNING|MB_OK);
		assert(false);
	}
}

bool CPlayerScript::AddProtect(word NIndex){
	ProtectGrp=(word*)realloc(ProtectGrp,(NProtectGrp+1)<<1);
	ProtectGrp[NProtectGrp]=NIndex;
	NProtectGrp++;
	return true;
};