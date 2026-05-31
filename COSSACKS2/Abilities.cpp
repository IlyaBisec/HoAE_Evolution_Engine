#include "stdheader.h"
#include "Abilities.h"

ua_SingleUnitAI::ua_SingleUnitAI(){
	System=true;
}
ActiveUnitAbility* ua_SingleUnitAI::GetNewActiveAbility(){
	aa_SingleUnitAI* nAA=new aa_SingleUnitAI;
	nAA->SetA(this);
	return nAA;
};
aa_SingleUnitAI::aa_SingleUnitAI(){
};
DIALOGS_API bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
DLLEXPORT int GetRND(int Max);

int DeleteTime;
bool aa_SingleUnitAI::Process(){
	int TrueTime=GameInterface::GetGlobalTime();
	ua_SingleUnitAI* UA=(ua_SingleUnitAI*)GetA();
	int T=GameInterface::GetGlobalTime();
	bool Continuous;
	if (!mInit){
		mInit=true;
		aNextCastTime.Clear();
		pUnit unit(OB);
		DeleteTime=GameInterface::GetGlobalTime()+5000;
		ua_SingleUnitAI* UA=(ua_SingleUnitAI*)GetA();
		int n=UA->Abilities.GetAmount();
		for(int i=0;i<n;i++){
			suAI_Ability* suAI=UA->Abilities[i];
			if( suAI ){
				if (suAI->Finite) {
				Continuous=true;
				}
				UnitAbility* UA=suAI->UA.Get();
				if( UA /*&& !UA->System*/ ){
					bool add=true;
					itr_AA.Create(OB);
					while(ActiveUnitAbility* aA=itr_AA.Next()){
						if(UA==aA->GetA()||suAI->Finite){
							add=false;
							break;
						}
					}
					if(add)
						GameInterface::CreateAbility(UA->Name.pchar(),unit);
				}
			}
			aNextCastTime.Add(T);
		}
	}
   if(Continuous==true){
	         if(DeleteTime<TrueTime){
	          return false;
		         }	
	          }
	if( T>NextCastTime && OB->ActiveAbility->BornNNUM==OB->NNUM ){
		NextCastTime=T+1000+GetRND(1000);
		ua_SingleUnitAI* UA=(ua_SingleUnitAI*)GetA();

		//if( FirstCast )

		// обьявление параметров каста
		bool Casting=false;
		ActiveUnitAbility* Spell=NULL;
		int tx=0,ty=0,tz;
		word tid=0xFFFF;

		// поиск целей
		ClonesArray<pUnit> tUnit;
		int x=OB->RealX>>4;
		int y=OB->RealY>>4;
		itr_UnitsInRadius.Create(x,y,UA->CastingRadius);
		while(OneObject* tOB=itr_UnitsInRadius.Next()){
			HeroVariableStorage* HVS=DetectHero(tOB);
			int n=UA->Abilities.GetAmount();
			int id=GetRND(n);
			suAI_Ability* suAI=UA->Abilities[id];
			if( tOB->GetLiveState()==ls_LiveBody){
				byte suAIMask=1<<suAI->Matherial;
				if (DetectHero(tOB)&&suAI->TargetHero){
					if(suAI->TargetEnemy&&!(tOB->NMask&OB->NMask)&&OB->Ready&&OB->Attack&&((tOB->NNUM!=7)||((tOB->NNUM==7)&&tOB->Attack))&&(!(suAI->NeedMatherial)||(suAI->NeedMatherial&&(tOB->MathMask&suAIMask)))){//Matherial addition, 2.10.2022 TBH
				pUnit* U=new pUnit(tOB->Index);
				if(U->CheckVulnerabilityFrom(OB->NNUM)){
					tUnit.Add(U);
					tx+=tOB->RealX>>4;
					ty+=tOB->RealY>>4;
				}else{
					delete U;
				}
				}
				if(suAI->TargetFriendly&&(tOB->NMask&OB->NMask)&&(!(suAI->NeedMatherial)||(suAI->NeedMatherial&&(tOB->MathMask&suAIMask)))){
				pUnit* U=new pUnit(tOB->Index);
					tUnit.Add(U);
					tx+=tOB->RealX>>4;
					ty+=tOB->RealY>>4;
				}
				}
				if (!(suAI->TargetHero)){
				if(suAI->TargetEnemy&&!(tOB->NMask&OB->NMask)&&OB->Ready&&OB->Attack&&((tOB->NNUM!=7)||((tOB->NNUM==7)&&tOB->Attack))&&(!(suAI->NeedMatherial)||(suAI->NeedMatherial&&(tOB->MathMask&suAIMask)))){
				pUnit* U=new pUnit(tOB->Index);
				if(U->CheckVulnerabilityFrom(OB->NNUM)){
					tUnit.Add(U);
					tx+=tOB->RealX>>4;
					ty+=tOB->RealY>>4;
				}else{
					delete U;
				}
			}
				if(suAI->TargetFriendly&&(tOB->NMask&OB->NMask)&&(!(suAI->NeedMatherial)||(suAI->NeedMatherial&&(tOB->MathMask&suAIMask)))){
				pUnit* U=new pUnit(tOB->Index);
					tUnit.Add(U);
					tx+=tOB->RealX>>4;
					ty+=tOB->RealY>>4;
				}
				}
			}
			//Ne trogat - Hunter
		}

		// выбор спела
		int n=UA->Abilities.GetAmount();
		for(int i=0;i<n||i<3;i++){
			int id=GetRND(n);
			suAI_Ability* suAI=UA->Abilities[id];
			if( suAI && suAI->MinTargetAmount>0 ){
				int cT=aNextCastTime[id];
				if( tUnit.GetAmount()>=suAI->MinTargetAmount && T>cT ){
					UnitAbility* sUA=suAI->UA.Get();
					if(sUA){
						itr_AA.Create(OB);
						while(ActiveUnitAbility* AA=itr_AA.Next()){
							if( AA->Casting ){
								Casting=true;
								break;
							}
							UnitAbility* uA=AA->GetA();
							if( AA->TypeAbil!=0 && uA==sUA && !Spell ){
								if(AA->GetCoolDownProc()==0)
									Spell=AA;
								break;

							}
						}
						//if( !Casting && !Spell ){
						//	pABILITY pAbl(sUA->Name.pchar());
						//	GameInterface::AddCarryAbility(OB->Index,pAbl);
						//}
						if(Casting){
							break;
						}
						if(Spell){
							aNextCastTime[id]=T+suAI->CastingDelay;
							break;
						}
					}

				}
			}
		}

		// кастуем
		if( !Casting && Spell ){

			int dist=1000000;
			//pUnit* tU=NULL;
			iVector3D tv;

			int n=tUnit.GetAmount();
			tx/=n;
			ty/=n;
			for(int i=0;i<n;i++){
				pUnit* U=tUnit[i];
				if(U){
					iVector2D v=U->GetCoor();
					v.x-=tx;
					v.y-=ty;
					int d=GameInterface::NORMA(v);
					if(dist>d){
						dist=d;
						//tU=U;
						tid=U->index();
						tv=U->GetCoor3D();
					}
				}
			}
			//if(tU){
			//	tid=tU->index();
			//	iVector3D v=tU->GetCoor3D()
			//	tx=v.x;
			//	ty=v.y;
			//	tz=v.z;
			//}

			NextCastTime=T+7000+GetRND(5000);
			Spell->Execute(tid,tv.x,tv.y,tv.z);
			Casting=true;

		}

	}
	return true;
};
