#include "stdheader.h"

//#define NOATTLIST

extern AttList ATTLIST;
void TestDynMem();
void AttList::Add (OneObject* Attacker,OneObject* Victim){
#ifdef NOATTLIST
	return;
#endif
	int pos=Victim->Index;
	int apos=pos%NARows;
	VArr* VA=&VList[apos];
	for(int j=0;j<VA->GetAmount();j++){
		if((*VA)[j]->ID==pos){
			DynArray<word>* WA=&((*VA)[j]->AttIDSN);
			word AIDX=Attacker->Index;
			for(int q=0;q<WA->GetAmount();q+=2)if((*WA)[q]==AIDX){
				return;
			}
			WA->Add(AIDX);
			WA->Add(Attacker->Serial);
			return;
		}
	}
	OneVictim* OV=new OneVictim;
	OV->ID=Victim->Index;
	OV->SN=Victim->Serial;
	OV->AttIDSN.Add(Attacker->Index);
	OV->AttIDSN.Add(Attacker->Serial);
	VA->Add(OV);	
}
void AttList::Del (OneObject* Attacker,OneObject* Victim){
#ifdef NOATTLIST
	return;
#endif
	int pos=Victim->Index;
	int apos=pos%NARows;
	VArr* VA=&VList[apos];
	for(int j=0;j<VA->GetAmount();j++){
		if((*VA)[j]->ID==pos){
			DynArray<word>* WA=&((*VA)[j]->AttIDSN);
			word AIDX=Attacker->Index;
			for(int q=0;q<WA->GetAmount();q+=2)if((*WA)[q]==AIDX){
                WA->Del(q,2);
				break;
			}
			if(!WA->GetAmount()){
				WA->Clear();
				VA->Del(j,1);
			}
			return;
		}
	}
}
void AttList::DelV(OneObject* Victim){
#ifdef NOATTLIST
	return;
#endif
	int pos=Victim->Index;
	int apos=pos%NARows;
	VArr* VA=&VList[apos];
	for(int j=0;j<VA->GetAmount();j++){
		if((*VA)[j]->ID==pos){			
			VA->Del(j,1);
			return;
		}
	}
}
int  AttList::GetNAttackers(OneObject* Victim){
#ifdef NOATTLIST
	return 0;
#endif
	int pos=Victim->Index;
	int apos=pos%NARows;
	VArr* VA=&VList[apos];
	for(int j=0;j<VA->GetAmount();j++){
		if((*VA)[j]->ID==pos){						
			int NA=(*VA)[j]->AttIDSN.GetAmount()>>1;
			NA-=Victim->newMons->MaxAttackersAmount-1;
			if(NA<0)NA=0;
			return NA;
		}
	}
	return 0;
}
int  AttList::GetLifeAfterNextShot(OneObject* Victim){
#ifdef NOATTLIST
	return Victim->Life;
#endif
	int Life=Victim->Life;
	int pos=Victim->Index;
	int apos=pos%NARows;
	VArr* VA=&VList[apos];
	for(int j=0;j<VA->GetAmount();j++){
		if((*VA)[j]->ID==pos){
			DynArray<word>* WA=&((*VA)[j]->AttIDSN);
			for(int q=0;q<WA->GetAmount();q+=2){
				word AID=(*WA)[q];
				word ASN=(*WA)[q+1];
				OneObject* AOB=Group[AID];
				if( AOB && AOB->Serial==ASN ){
					Life-=AOB->MoreCharacter->MaxDam;
				}
			}
			break;
		}
	}
	return Life;
}
void AttList::Refresh(){
#ifdef NOATTLIST
	return;
#endif
	for(int i=0;i<NARows;i++){
		VArr* VA=&VList[i];
		for(int j=0;j<VA->GetAmount();j++){
			OneObject* VOB=Group[(*VA)[j]->ID];
			if(VOB&&(!VOB->Sdoxlo)&&VOB->Serial==(*VA)[j]->SN&&rando()>100){
                DynArray<word>* WA=&((*VA)[j]->AttIDSN);
				for(int q=0;q<WA->GetAmount();q+=2){
					word AID=(*WA)[q];
					word ASN=(*WA)[q+1];
					OneObject* AOB=Group[AID];
					if(!(AOB&&(!AOB->Sdoxlo)&&AOB->Serial==ASN)){
                        WA->Del(q,2);
						q-=2;
					}
				}
				if(!WA->GetAmount()){					
					WA->Clear();
					VA->Del(j,1);
					j--;
				}
			}else{
				DynArray<word>* WA=&((*VA)[j]->AttIDSN);
				WA->Clear();
				VA->Del(j,1);
				j--;
			}
		}
	}
}
void AttList::Clear(){
#ifdef NOATTLIST
	return;
#endif
	for(int i=0;i<NARows;i++){
		VArr* VA=&VList[i];
		for(int j=0;j<VA->GetAmount();j++){
			DynArray<word>* WA=&((*VA)[j]->AttIDSN);
			WA->Clear();
			//delete(VA);
		}
		VA->Clear();
	}
}
AttList::~AttList(){
	Clear();
}