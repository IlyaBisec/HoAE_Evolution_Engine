#include "stdheader.h"
#include ".\mai_extension.h"
//////////////////////////////////////////////////////////////////////////

//AdvCharacter* ADC=OB->MoreCharacter

int mai_getUnitForce(AdvCharacter* ADC, AdvCharacter* Enm){
	int A=0;
	for(int i=0;i<NAttTypes;i++){
		int Ai=ADC->MaxDamage[i]-Enm->Protection[ADC->WeaponKind[i]];
		if(Ai>0){
			A+=Ai;
		}
	}
    return A*ADC->Life;
}
bool mai_getAverageUnit(UnitsGroup* UG, AdvCharacter& ADC, int& N){
	return true;
}
bool mai_getAverageUnit(byte NI, AdvCharacter& ADC, int& N){
	word* id=NatList[NI];
	int n=NtNUnits[NI];
	//
	N=0;
	ADC.Life=0;
	for(int i=0;i<NAttTypes;i++){
		ADC.MaxDamage[i]=0;
		ADC.WeaponKind[i]=i;
	}
	for(i=0;i<16;i++){
		ADC.Protection[i]=0;
	}
	for(i=0;i<n;i++){
		OneObject* OB=Group[id[i]];
		if(OB&&OB->LockType==0&&!(OB->Sdoxlo||OB->NewBuilding||OB->newMons->Peasant)){
			N++;
			AdvCharacter* adv=OB->MoreCharacter;
			ADC.Life+=adv->Life;
			for(int j=0;j<NAttTypes;j++){
				if(adv->MaxDamage[j]>0){
					float GetAttSpped(OneObject* OB,int AttType);
					ADC.MaxDamage[adv->WeaponKind[j]]+=adv->MaxDamage[j]/GetAttSpped(OB,j);
				}
			}
			for(j=0;j<16;j++){
				ADC.Protection[j]+=adv->Protection[j];
			}
		}        
	}	
	return true;
}
bool mai_getAverageUnit(byte NI, mai_Unit& Unit){
	word* id=NatList[NI];
	int n=NtNUnits[NI];
	//
	Unit.N=0;	
	Unit.M=0;		
	Unit.A.Clear();
	Unit.P.Clear();
	Unit.A.Check(16);
	Unit.A.NValues=16;			
	Unit.P.Check(16);
	Unit.P.NValues=16;
	//
	for(int i=0;i<n;i++){
		OneObject* OB=Group[id[i]];
		if(OB&&OB->LockType==0&&!(OB->Sdoxlo||OB->NewBuilding||OB->newMons->Peasant)){
			Unit.N++;
			AdvCharacter* adv=OB->MoreCharacter;
			Unit.M+=adv->Life;
			for(int j=0;j<NAttTypes;j++){
				if(adv->MaxDamage[j]>0){
					float GetAttSpped(OneObject* OB,int AttType);
					Unit.A[adv->WeaponKind[j]]+=adv->MaxDamage[j]/GetAttSpped(OB,j);
				}
			}
			for(j=0;j<16;j++){
				Unit.P[j]+=adv->Protection[j];
			}
		}        
	}
	if(Unit.N==0){
		Unit.A.Clear();
		Unit.P.Clear();
		return false;
	}else{
		Unit.M/=Unit.N;
		for(int i=0;i<NAttTypes;i++){
			Unit.A[i]/=Unit.N;
		}
		for(i=0;i<16;i++){				
			Unit.P[i]/=Unit.N;
		}
	}		
	return true;
}
