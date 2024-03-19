#include "stdheader.h"
#include "BE_HEADERS.h"

CEXPORT word GetPlayerNMask(byte NI){	
	return NATIONS[NI].NMask;
}

//
CIMPORT void AddStorm(GAMEOBJ* Src, byte Owner, int Diff);
CIMPORT void AddFirers(GAMEOBJ* Src,byte Owner);
CIMPORT void AddPsKillers(GAMEOBJ* Src,byte Owner,bool SeakMine=true);
CIMPORT void AddTomahawks(GAMEOBJ* Src,byte Owner,word Base=0,int DriftRadius=0, int CriticalMass=0);

int TempDynUGroup=-1;
void TempDynUGroupInit(){
	UnitsGroup* UG=SCENINF.GetNewGroup();
	TempDynUGroup=UG->Index;
}
class cext_VeryStupid: public GameExtension{
	virtual void OnInitAfterMapLoading(){
		TempDynUGroupInit();
	}
};
void Install_cext_VeryStupid(){
	InstallExtension(new cext_VeryStupid,	"Tupee ne pridumaesh, sorry");
}

UnitsGroup* CopyToTempUnitsGroup(be_CGROUP* Grp,bool remove){
	if(TempDynUGroup==-1) TempDynUGroupInit();
	UnitsGroup* UG=SCENINF.UGRP+TempDynUGroup;
	UG->Clear();	
	int n=Grp->GetTotalAmount();
	for(int i=0;i<n;i++){
		OneObject* OB=Grp->GetOneObj_lID(i);
		if(OB){
			UG->AddNewUnit(OB);
		}
	}
	if (remove) Grp->RemAllUnits();
	return UG;
}
void AddStorm(be_CGROUP* Grp, byte Owner, int Diff,bool remove){
	UnitsGroup* UG=CopyToTempUnitsGroup(Grp,remove);
	GAMEOBJ G={UG->Index,0,'UNIT'};

	addrand(UG->Index);
	addrand(Owner);
	addrand(Diff);

	AddStorm(&G,Owner,Diff);
}
void AddFirers(be_CGROUP* Grp,byte Owner,bool remove){
	UnitsGroup* UG=CopyToTempUnitsGroup(Grp,remove);
	GAMEOBJ G={UG->Index,0,'UNIT'};
	AddFirers(&G,Owner);
};
void AddPsKillers(be_CGROUP* Grp,byte Owner,bool remove,bool SeakMine=true){
	UnitsGroup* UG=CopyToTempUnitsGroup(Grp,remove);
	GAMEOBJ G={UG->Index,0,'UNIT'};
	AddPsKillers(&G,Owner,SeakMine);
};
void AddTomahawks(be_CGROUP* Grp,byte Owner,bool remove,word Base=0,int DriftRadius=0, int CriticalMass=0){
	UnitsGroup* UG=CopyToTempUnitsGroup(Grp,remove);
	GAMEOBJ G={UG->Index,0,'UNIT'};
	AddTomahawks(&G,Owner,Base,DriftRadius,CriticalMass);
};
//
CEXPORT bool isAttBuild(OneObject* OB){
	if(OB){
		return OB->newMons->AttBuild;
	}
	return false;	
}
CIMPORT void GetDipSimpleBuildings(int &NDips, DIP_SimpleBuilding** &Dips);
void EraseAllNetralPeasantsAndPolicemen(){
	DynArray<word> NIndex;
	//
	int NDips=0;
	DIP_SimpleBuilding** Dips=NULL;
	GetDipSimpleBuildings(NDips,Dips);
	for(int i=0;i<NDips;i++){
		DIP_SimpleBuilding* DSB=Dips[i];
		NIndex.Add(DSB->CopNIndex);
		NIndex.Add(DSB->PeasantNIndex);		
	}
	//
	for(i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Sdoxlo){
			for(int j=0;j<NIndex.GetAmount();j++){
				if(NIndex[j]==OB->NIndex){
					void EraseObject(OneObject* OB);
					EraseObject(OB);
					break;
				}
			}
		}		
	}
}