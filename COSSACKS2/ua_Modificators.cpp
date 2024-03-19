#include "stdheader.h"
#include "ua_Modificators.h"
#include "cvi_MainMenu.h"
#include "PluginsInterface.h"
#include "ai_scripts.h"
#include "UnitAbility.h"
#include "UnitAbilityExtension.h"
//
Weapon* GetWeaponFromModificator(WeaponModificator* WM);
HeroVariableStorage* DetectHero(OneObject* OB);
extern int TrueTime;
DLLEXPORT void OBJ_ChangeNation(OneObject* OB, byte DstNat);
void AttackObjLink(OneObject* OBJ);

//#define	SetMask(x) { mod_##x* m=dynamic_cast<mod_##x*>(UM); \
	if(m){	V->InfluenceMask.set_bit(ABL_##x); return; } }

//bool CheckInfluenceMask(ActiveUnitAbility* V, UnitModificator* UM, DWORD Mask){
//}
bool CheckInfluenceMask(ActiveUnitAbility* V, DWORD Mask){
	UnitAbility* UA=V->GetA();
	if(UA){
		for(int i=0;i<UA->Modificators.GetAmount();i++){
			UnitModificator* UM=UA->Modificators[i];
			if(UM->InfluenceMask==0xFFFFFFFF){
				UnitModificatorComplex* mC=(UnitModificatorComplex*)UM;
				if(mC){
					for(int j=0;j<mC->Complex.GetAmount();j++){
						UnitModificator* UM=mC->Complex[j];
						if(UM&&UM->InfluenceMask==Mask)
							return true;
					}
				}
			}else{
				if(UM->InfluenceMask==Mask)
					return true;
			}
		}
	}
	return false;
}

void SetInfluenceMask(ActiveUnitAbility* V, UnitModificator* UM){

	if(UM->InfluenceMask!=0xFFFFFFFF){
		addrand(UM->InfluenceMask);
		V->InfluenceMask.set_bit(UM->InfluenceMask);
	}

}

int ActiveAbilityListArray::Add(ActiveUnitAbility* V){
	UnitAbility* UA=V->GetA();
	// Blocking
	if(V&&V->TypeAbil==0){
		if(UA){
			for(int i=0;i<UA->Blocking.GetAmount();i++){
				UnitAbility* BUA=UA->Blocking[i]->Get();
				if(BUA){
					itr_AA.Create(V->OB);
					while(ActiveUnitAbility* aA=itr_AA.Next()){
						if(aA->TypeAbil==0){
							if(BUA==aA->GetA()){
								aA->HealShots-=BUA->HealShots;
								aA->DamageShots-=BUA->DamageShots;
								delete V;
								return -1;
							};
						}						
					}
				}
			}

		}
	}
	// Set unical serial number
	if(V){
		V->Serial=V->CurSerial++;
		V->BirthTime=TrueTime;
		addrand(V->Serial);
		addrand(V->BirthTime);
	}
	int p=ClassArray<ActiveUnitAbility>::Add(V);
	
	addrand(p);

	// Item clothe
	if(V->TypeAbil==4){
		HeroVariableStorage* HVS=DetectHero(V->OB);
		if( HVS && HVS->LoadFromProfile ){
			IGI.RenewedInventary=true;
		}
	}

	// Set modificators
	if(UA){
		if( V && (V->TypeAbil==0||V->TypeAbil==4&&DetectHero(V->OB)||V->TypeAbil==6|| 
			UA->ClassType==1 && (!(V->OB->newMons->NewParams.AntiMage&&UA->Type==7) || V->TypeAbil==2)) ){
			for(int i=0;i<UA->Modificators.GetAmount();i++){
				UnitModificator* UM=UA->Modificators[i];
				SetInfluenceMask(V,UM);
				if( UM->InfluenceMask==0xFFFFFFFF ){
					UnitModificatorComplex* mC=(UnitModificatorComplex*)UM;
					if(mC){
						for(int j=0;j<mC->Complex.GetAmount();j++){
							SetInfluenceMask(V,mC->Complex[j]);
						}
					}
				}else if( UM->InfluenceMask==ABL_DeleteAbilityAfterNShots ){
					mod_DeleteAbilityAfterNShots* M=(mod_DeleteAbilityAfterNShots*)UM;
					if(M){
						int Shot=V->OB->ActiveAbility->NShots;
						V->DeleteShot=Shot+M->NShots;
					}
				}
			}
			V->Eff=NULL;
		}
	}
	ResetInfluenceMask();
	OneObject* OB=V->OB;
	if( OB->LocalOrder && OB->LocalOrder->DoLink==&AttackObjLink ){
		OneObject* Enemy=Group[OB->EnemyID];
		if( Enemy && Enemy->Serial==OB->EnemySN ){
			bool inv=false;
			byte detected=0;
			bool enable_search=false;
			modifyInvisibility(inv,inv,detected,enable_search); // отменять приказ атаки если на героя добавили абилку с действием невидимости
			if( inv && !((detected&Enemy->NMask)||enable_search) ){
				OB->DeleteLastOrder();
			}
			bool comp=false;
			modifyCompassion(comp,comp); // отменять приказ атаки для жалостливых
			if( comp ){
				OB->DeleteLastOrder();
			}
			//bool insub=false;
			//modifyInsubordination(insub,insub);
			//if( insub ){
			//	OB->DeleteLastOrder();
			//}
		}
	}

	return p;
}
void ActiveAbilityListArray::Del(int pos,int N){
	if(pos>=GetAmount())return;

	ActiveUnitAbility* V=(*this)[pos];

	itr_UM.Create(V,ABL_OverEarth);
	while(UnitModificator* UM=itr_UM.Next()){
		if (V->OB->NewState==10) {
			V->OB->NewState=0;
			V->OB->GroundState=0;
			TryToStand(V->OB,false);
		}
	}

	ClassArray<ActiveUnitAbility>::Del(pos,N);
	ResetInfluenceMask();
}

void ActiveAbilityListArray::ResetInfluenceMask(){
	
	InfluenceMask.clear_all();
	AnimationMask=false;

	for(int i=0;i<GetAmount();i++){
		ActiveUnitAbility* AA=(*this)[i];
		if(AA){
			InfluenceMask|=AA->InfluenceMask;
			if(!AnimationMask)
				AnimationMask|=AA->CheckAnimationMask();
		}
	}
};
//
UnitModificatorsIterator itr_UM;
//#define	SetMask(x) { mod_##x* m=dynamic_cast<mod_##x*>(UM->Modificators[i]); \
	if(m){	InfluenceMask.set_bit(ABL_##x); continue; } }
//
void ActiveUnitAbility::SetInfluenceMask(){
};

template <class X> void vOperand::Calculate(X Base, X& Value, bool Reverse){
	int Min=MinValue;
	X V=num;
	if(numType==1){
		//numType - 
			//0 - целое значение (Actual)
			//1 - процент, работает как булевая переменная(Вротебал) (Percent)
		if(percentType==0){
			//percentType - 
			//0 -от данного (From Current)
			//1 - базового (From Base)
			V=Value*num/100;
			Min=Value*Min/100;
		}else{
			V=Base*num/100;
			Min=Base*Min/100;
		}
	}

	if(Reverse){//Не знаю, нахера тут реверс, но да ладно. Реверс не интересует, вниз к елсу
		switch(sign){
		case 0: // plus
			//if(Value<V+MinValue) Value=MinValue;
			//	else Value-=V;
			Value-=V;
			break;
		case 1: // minus			
			Value+=V;
			break;
		case 2: // sub			
			if(V!=0){
				Value/=V;
				if(Value<Min) 
					Value=Min;
			}
			break;
		case 3: // dev
			Value*=V;
			break;
		case 4: // set
			Value=V;
			if(Value<Min) 
				Value=Min;
			break;
		};
	}else{
		switch(sign){
		case 0: // plus			
			Value+=V;
			break;
		case 1: // minus			
			if( Value>MinValue ){
				if( Value<V+MinValue )
					Value=MinValue;
				else
					Value-=V;
			}
			break;
		case 2: // sub			
			Value*=V;
			break;
		case 3: // dev			
			if(V!=0){
				Value/=V;
				if(Value<Min) 
					Value=Min;
			}
			break;
		case 4: // set
			Value=V;
			if(Value<Min) 
				Value=Min;
			break;
		};
	}
};

template <class X> void vOperand::CalculateRatio(X Base, X& Value, X &Relative, bool Reverse){
	X V=num;
	if(numType==1){
		if(percentType==0){
			V=Value*num/100;
		}else{
			V=Base*num/100;
		}
	}
    
	X Val=Value;

	if(Reverse){
		switch(sign){
		case 0: // plus			
			if(Value>=V) Value-=V; else Value=0;
			break;
		case 1: // minus			
			Value+=V;
			break;
		case 2: // sub			
			if(V!=0) Value/=V;
			break;
		case 3: // dev
			Value*=V;
			break;
		case 4: // set
			Value=V;
			break;
		};
	}else{
		switch(sign){
		case 0: // plus			
			Value+=V;
			break;
		case 1: // minus			
			if(Value>=V) Value-=V; else Value=0;
			break;
		case 2: // sub			
			Value*=V;
			break;
		case 3: // dev			
			if(V!=0) Value/=V;
			break;
		case 4: // set
			Value=V;
			break;
		};
	}

	X dVal=Value-Val;
	X dRel=dVal*Relative/Val;

	Relative+=dRel;
};
//
bool UnitModificatorConditionArray::Check (ActiveUnitAbility* AA) {
	if (GetAmount()==0) {
		return true;
	}
	for (int i=0; i<GetAmount(); i++) {
		bool chk=Values[i]->Check(AA);
		if(Values[i]->Not) chk=!chk;
		if (chk) {
			return true;
		}
	}
	return false;
};
bool UnitModificatorConditionArray::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="Conditions:";
	if(!ClassEditor::CheckIfOpen(this)){
		int N=GetAmount();
		if(N){
			dest+="\n";
			for (int i=0;i<N;i++){
				dest+="   %$";
				dest+=i;
				dest+="$%";
				if (i+1<N){
					if(options&&options[0]){
						dest+=options;
					}else{
						dest+=" or\n";
					}
				}
			};
		}else{
			dest+=" Always";
		}
	}
	return true;
}
//
int UnitModificatorArray::Add(UnitModificator* V){
	return ClassArray<UnitModificator>::Add(V);
};
inline void UnitModificatorArray::Del(int pos,int N){
	ClassArray<UnitModificator>::Del(pos,N);
};
bool UnitModificatorArray::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="Modificators:";
	if(!ClassEditor::CheckIfOpen(this)){
		int N=GetAmount();
		if(N){
			dest+="\n";
			for (int i=0;i<N;i++){
				dest+="   %$";
				dest+=i;
				dest+="$%";
				if (i+1<N) {
					dest+="\n";
				}
			};
		}else{
			dest+=" Empty";
		}
	}
	return true;
}

bool umc_Complex::Check (ActiveUnitAbility* AA) {
	for (int i=0; i<Complex.GetAmount(); i++) {
		//if (!Complex[i]->Check(AA)) {
		bool chk=Complex[i]->Check(AA);
		if(Complex[i]->Not) chk=!chk;
		if (!chk) {
			return false;
		}
	}
	return true;
}
word GetDir(int dx,int dy);
bool umc_DirectionDifference::Check (ActiveUnitAbility* AA){
	OneObject* Caster=Group[AA->CasterID];
	if( AA->CasterID!=0xFFFF && Caster && Caster->Serial==AA->CasterSN ){
		byte CasterDir=GetDir(AA->OB->RealX-Caster->RealX,AA->OB->RealY-Caster->RealY);
		byte Dir=abs(char(AA->OB->RealDir)-char(CasterDir));
		int d=abs(char(Direction)-char(Dir));
		if( d>Difference )
			return false;
	}
	return true;
}
bool umc_HeroAtHome::Check (ActiveUnitAbility* AA){
	return NATIONS[AA->OB->NNUM].HeroAtHome;
}

bool umc_Ready::Check (ActiveUnitAbility* AA){
	return AA->OB->Ready;
}
bool umc_LandTypeInPoint::Check (ActiveUnitAbility* AA) {
	int GetLandTypeInPoint(int x,int y);
	int LT=GetLandTypeInPoint(AA->OB->RealX>>4,AA->OB->RealY>>4);
	return LT==LandType;
}

bool umc_TimePeriod::Check (ActiveUnitAbility* AA) {
	int T=TrueTime-AA->EffectTime;
	if (T>=StartTime && (Unlimited||T<=StartTime+Duration)) {
		return true;
	}
	return false;
}
bool umc_MotionStyle::Check (ActiveUnitAbility* AA) {
	if(AA->OB->MotionStyle==Style){
		return true;
	}
	return false;
}
bool umc_IsMoving::Check (ActiveUnitAbility* AA){
	if( AA->OB && AA->OB->ActiveAbility ){
		if(AA->OB->DestX>0){
			return Norma(AA->OB->RealX-AA->OB->DestX,AA->OB->RealY-AA->OB->DestY)>10*16;
		}
	}
	return false;
}
bool umc_OverWater::Check (ActiveUnitAbility* AA){
	if( AA->OB && AA->OB->ActiveAbility ){
		//int x = AA->OB->RealX>>4, y = AA->OB->RealY>>4;
		//int H = GetTotalHeight(x, y); 
		/*Improvement by Flame_Master*/
		int x = AA->OB->RealX / 16;
		int y = AA->OB->RealY / 16;
		int H = GetTotalHeight(x, y);
		return H<=3;
	}
	return false;
}
bool umc_UnitType::Check (ActiveUnitAbility* AA){
	//OneObject* OB=AA->OB;
	//word NIndex=OB->NIndex;
	//if(OB->ActiveAbility){
	//	NIndex=OB->ActiveAbility->BornNIndex;
	//}
	word NIndex=AA->OB->NIndex;
	if(AA->OB->ActiveAbility)
		NIndex=AA->OB->ActiveAbility->BornNIndex;

	if(!init){
		init=true;
		memset(type,0,sizeof(type));
		int N=Type.GetAmount();
		if(N){
			for(int i=0;i<N;i++){
				type[Type[i]->UnitType]=true;
			}
		}
		if(NIndex>=2048){
			Log.Error("umc_UnitType: Unit NIndex>=2048");
		}
	}

	return type[NIndex];
};
bool umc_CasterUnitType::Check (ActiveUnitAbility* AA){
	int N=Type.GetAmount();
	if(N){
		if(AA->CasterID!=0xFFFF){
			OneObject* OB=Group[AA->CasterID];
			if(OB&&OB->Serial==AA->CasterSN){
				word NIndex=OB->NIndex;
				if(OB->ActiveAbility){
					NIndex=OB->ActiveAbility->BornNIndex;
				}
				for(int i=0;i<N;i++){
					if(Type[i]->UnitType==NIndex){
						return true;
					}
				}
			}
		}
	}
	return false;
};
bool umc_FriendlyToCaster::Check (ActiveUnitAbility* AA){
	OneObject* Caster=Group[AA->CasterID];
	if (AA->CasterID!=0xFFFF && Caster && Caster->Serial==AA->CasterSN) {
		return Caster->NMask&AA->OB->NMask;
	}
	return false;
};
bool umc_Caster::Check (ActiveUnitAbility* AA){
	if (AA->OB->Index==AA->CasterID && AA->OB->Serial==AA->CasterSN){
		return true;
	}
	return false;
};
bool umc_CasterLevel::Check(ActiveUnitAbility* AA){
	if (AA->CasterLevel>=Level){
		return true;
	}
	return false;
};
bool umc_Summon::Check (ActiveUnitAbility* AA){
	if (AA->OB->ActiveAbility) {
		bool val=false;
		AA->OB->ActiveAbility->ActiveAbilities.modifySummon(val);
		return val;
	}
	return false;
};
bool umc_Frozen::Check (ActiveUnitAbility* AA){
	if (AA->OB->ActiveAbility) {
		bool val=false;
		AA->OB->ActiveAbility->ActiveAbilities.modifyFrozen(val);
		return val;
	}
	return false;
};
bool umc_Incorporeal::Check (ActiveUnitAbility* AA){
	return AA->OB->newMons->Incorporeal;
};
bool umc_UpgradeIsDone::Check (ActiveUnitAbility* AA){
	if (AA->OB) {
		return NATIONS[AA->OB->NNUM].UPGRADE[UpgradeID]->Done;
	}
	return false;
};
bool umc_Hero::Check (ActiveUnitAbility* AA){
	return DetectHero(AA->OB);
};
bool umc_Building::Check (ActiveUnitAbility* AA){
	return AA->OB->NewBuilding;
};
bool umc_Hidden::Check (ActiveUnitAbility* AA){
	return AA->OB->Hidden;
};
bool umc_LiveBody::Check (ActiveUnitAbility* AA){
	return AA->OB->GetLiveState()==ls_LiveBody;
};
bool umc_LockType::Check (ActiveUnitAbility* AA){
	return AA->OB->LockType==LockType;
};
bool umc_LifeLess::Check (ActiveUnitAbility* AA){
	if(ValueType==0){
		return AA->OB->Life<=Value;
	}else{
		return AA->OB->Life*100/AA->OB->MaxLife/*MoreCharacter->LifeMax*/<Value;
	}
};
bool umc_AttackRadiusMax::Check (ActiveUnitAbility* AA){
	bool res=false;
	static bool recursion=false;
	if(!recursion){
		recursion=true;
		OneObject* OB=NULL;
		if( Caster && AA->CasterID!=0xFFFF ){
			OB=Group[AA->CasterID];
			if( !( OB && OB->Serial==AA->CasterSN ) ){
				OB=AA->OB;
			}
		}else{
			OB=AA->OB;
		}
		int R=OB->GetMaxAttackRadius();
		res=PerformAI_Op(R,Radius,Compare);
	}
	recursion=false;
	return res;
};
bool umc_WeaponKind::Check (ActiveUnitAbility* AA){
	bool res=false;
	static bool recursion=false;
	if(!recursion){
		recursion=true;
		AdvCharacter* AC=AA->OB->MoreCharacter;
		for(int i=0;i<NAttTypes;i++){
			if(AC->MaxDamage[i]){
				int WK=AC->WeaponKind[i];
				AA->OB->ActiveAbility->ActiveAbilities.modifyWeaponKind(WK,WK,i);
				res=(Kind==WK);
				break;
			}
		}
	}
	recursion=false;
	return res;
};
bool umc_FindTargetNearCaster::Check(ActiveUnitAbility* AA) {
	bool res=false;
	OneObject* Caster=GlobalUnitsContext::getCaster();
	if(Caster){

		int x=Caster->RealX>>4;
		int y=Caster->RealY>>4;
		int radius=Radius;

		if (Units) itr_UnitsInRadius.Create(x,y,radius);
		if (Buildings) itr_GetBuildingsInRadius.Create(x,y,radius);

		while(true){
			OneObject* ob=NULL;
			if( Units ) ob=itr_UnitsInRadius.Next();
			if( Buildings && !ob ) ob=itr_GetBuildingsInRadius.Next();
			if(ob){

				eLiveState LS=ob->GetLiveState();
				if( LiveBody&&LS==ls_LiveBody || DeadBody&&LS==ls_DeadBody || Bones&&LS==ls_Bones ){
					if( !Wounded || ob->Life<ob->MaxLife ){
						if( Caster->NMask&ob->NMask ){
							if(Friend)
								return true;
						}else{
							if(Enemy)
								return true;
						}
					}
				}

			}else{
				break;
			}
		}
	}
	return false;
}
bool umc_Invisibility::Check (ActiveUnitAbility* AA){
	bool res=false;
	OneObject* OB=AA->OB;
	if( Subject==0 && AA->CasterID!=0xFFFF ){
		OB=Group[AA->CasterID];
		if( OB && OB->Serial!=AA->CasterSN ){
			OB=NULL;
		}
	}
	static bool recursion=false;
	if( OB && !recursion){
		recursion=true;
		byte Detected;
		bool EnableSearchVictim;
		if(OB->ActiveAbility)
			OB->ActiveAbility->ActiveAbilities.modifyInvisibility(res,res,Detected,EnableSearchVictim);
	}
	recursion=false;
	return res;
};
void UnitModificatorsIterator::Collect(UnitModificatorConditionArray* UMCs, UnitModificatorArray* UMs){
	if(UMCs->Check(A)){
		for (int i=0; i<UMs->GetAmount() ;i++) if(N<2048) {
			UnitModificator* M=(*UMs)[i];
			if (M){
				if (M->InfluenceMask==0xFFFFFFFF) {
					UnitModificatorComplex* UMC=(UnitModificatorComplex*)M;
					Collect(&UMC->Conditions,&UMC->Complex);
				} else if (InfluenceMask.check_bit(M->InfluenceMask) && M->Conditions.Check(A)) {
					UM[N]=M;
					N++;
				}
			}
		}
	}
}
void UnitModificatorsIterator::CollectAll(UnitModificatorConditionArray* UMCs, UnitModificatorArray* UMs){
	if( A==NULL || !UMCs || UMCs->Check(A) ){
		for (int i=0; i<UMs->GetAmount() ;i++) if(N<2048) {
			UnitModificator* M=(*UMs)[i];
			if (M){
				if (M->InfluenceMask==0xFFFFFFFF) {
					UnitModificatorComplex* UMC=(UnitModificatorComplex*)M;
					CollectAll(&UMC->Conditions,&UMC->Complex);
				} else if (/*InfluenceMask.check_bit(M->InfluenceMask) && */M->Conditions.Check(A)) {
					UM[N]=M;
					N++;
				}
			}
		}
	}
}
void UnitModificatorsIterator::Create(ActiveUnitAbility* AA, DWORD Mask){
	N=0;
	Current=0;
	if(Mask==0xFFFFFFFF){
		UnitAbility* UA=AA->GetA();
		if(UA){
			if( AA->TypeAbil==0 || (AA->TypeAbil==4 /*&& (AA->Clothed||Mask==ABL_Symbol)*/) || AA->TypeAbil==5 || (AA->TypeAbil!=4 && UA->ClassType==1) ){
				A=AA;
				//InfluenceMask.clear_all();
				//InfluenceMask.set_bit(Mask);
				CollectAll(NULL/*&UA->Conditions*/,&UA->Modificators);				
			}
		}
	}else{
		if(AA->InfluenceMask.check_bit(Mask)){
			UnitAbility* UA=AA->GetA();
			if(UA){
				if( AA->TypeAbil==0 || (AA->TypeAbil==4 && (AA->Clothed||Mask==ABL_Symbol)) || (AA->TypeAbil!=4 && UA->ClassType==1) ){
					A=AA;
					InfluenceMask.clear_all();
					InfluenceMask.set_bit(Mask);
					Collect(&UA->Conditions,&UA->Modificators);				
				}
			}
		}
	}
};
void UnitModificatorsIterator::Create(UnitAbility* UA){
	N=0;
	Current=0;
	A=NULL;
	if(UA){
		CollectAll(&UA->Conditions,&UA->Modificators);				
	}
};
UnitModificator* UnitModificatorsIterator::Next(){
	if(Current<N){
		UnitModificator* M=UM[Current];
		Current++;
		return M;
	}
	return NULL;
};

//
//#define SetUM(x) GlobalUnitsContext::setCaster(CasterID,CasterSN);\
//	itr_UM.Create(this,ABL_##x);\
//	while (mod_##x* M=(mod_##x*)itr_UM.Next())
//

void ActiveUnitAbility::modifyMotionSpeed		(int BasicSpeed,int& CurrentSpeed,bool& Stun){
	if( OB->GetLiveState()==ls_LiveBody ){
		SetUM(MotionSpeed){
			Stun=M->Stun;
			if(M->Trap){
				CurrentSpeed=0;
			}else{
				M->MotionSpeed.Calculate(BasicSpeed,CurrentSpeed);
			}
		}
	}
};
void ActiveUnitAbility::modifyProtection		(int AttType,int Basic,int& Current,OneObject* Damager){
	if (AttType>=0) {
		SetUM(Protection){
			if(AttType<M->Operand.GetAmount()){
				M->Operand[AttType]->Calculate(Basic,Current);
			}
		}
	}
};
void ActiveUnitAbility::modifyShield            (int Basic,int& Current,OneObject* Damager){
	SetUM(Shield){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyDamage            (int AttType,int Basic,int& Current,OneObject* Victim){
	//addrand(Current);
	//if(AttType==0){
		SetUM(Damage){
			M->Operand.Calculate(Basic,Current);
			//addrand(Current);
		}
	//}
};
void ActiveUnitAbility::modifyAttackRate        (int AttackType,int Basic,int& Current,OneObject* Victim){
	SetUM(AttackRate){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyLifeRegeneration  (int Basic,int& Current){
	SetUM(LifeRegeneration){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyDamReduct (int Basic, int& Current,int ReductType){
	SetUM(DamReduct){
		if (ReductType!=M->ReductionType) break;
		//Reduction Type: 0 - "Clean" damage(before calculating defence)
		//1 - with defence
		M->Operand.Calculate(Basic,Current);
	}
}
void ActiveUnitAbility::modifyIgnoreProtection(int Basic,int& Current){
	SetUM(IgnoreProtection){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyVampire (word Basic, word& Current,int Damage,OneObject* Damager,OneObject* Victim){
	SetUM(Vampire){
		switch(M->SourceType){
		case 0: //Damage
			break;
		case 1: //VictimLife
			Damage=Victim->MaxLife;
			break;
		case 2: //DamagerLife
			Damage=Damager->MaxLife;
			break;
		case 3: //CleanDamage
			Damage=Damager->CleanDamage;
			break;
		};
		M->Operand.Calculate(Damage,Damage);
		if ((Damager->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Alteration))){
		Damager->ActiveAbility->ActiveAbilities.modifyAlteration(Damage,Damage,Damager);
		}
		if(Damage<1) 
			Damage=1;
		Current+=Damage;
		if(Current>Damager->MaxLife){
			Current=Damager->MaxLife;
			break;
		}

		//addrand(Current);
	}
}

void ActiveUnitAbility::modifyAlteration		(int Basic, int& Current, OneObject* Target){
	bool ErFix=0;
	SetUM(Alteration){
		switch(M->ModifType){ 
		case 0: //Thorn
			if (!(Target->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Thorn))){
				ErFix=1;
			}
			break;
		case 1: //Vampire
			if (!(Target->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Vampire))){
				ErFix=1;
			}
			break;
		};
		if (ErFix==1){
			break;
		}
	 M->Operand.Calculate(Basic,Current);
	}
}
//Не ломайте голову, что это, просто примите факт того, что это нужно, для вызова виртуальной функции
void ActiveUnitAbility::modifyThorn				(word Basic, word& Current,int Damage,OneObject* Damager,OneObject* Victim, byte AttType){
	SetUM(Thorn){
		switch(M->SourceType){
		case 0: //Damage
			break;
		case 1: //VictimLife
			Damage=Victim->MaxLife;
			break;
		case 2: //DamagerLife
			Damage=Damager->MaxLife;
			break;
		case 3: //CleanDamage
			Damage=Damager->CleanDamage;
			break;
		};
		M->Operand.Calculate(Damage,Damage);
		if ((Victim->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Alteration))){
		Victim->ActiveAbility->ActiveAbilities.modifyAlteration(Damage,Damage,Victim);
		}
		int AttRadius=Damager->GetMaxAttackRadius(AttType);
		if((M->RangeThorn&&AttRadius>150)||(!M->RangeThorn&&AttRadius<150)||(M->AnyDistanceThorn)){
		if(Damage<1){
			Damage=1;
		}
		if(Current<=Damage){
			Current=0;
			//OneObject* Caster=NULL;
			//if(CasterID!=0xFFFF){
			//	Caster=Group[CasterID];
			//	if(Caster&&Caster->Serial!=CasterSN){
			//		Caster=NULL;
			//	}
			//}
			//if(Caster){
				void OnDie(int Killer,int Victim);
				OnDie(Victim->Index,Damager->Index);
				if (!ext_OnUnitDie(Damager,Victim)) return;
			//}
			//OnUnitDie(Damager,Victim);
			Damager->Die();
			break;
		}else{
			Current-=Damage;
			//int AttType=128+5;
			//Damager->MakeDamage(Damage,Damage,Victim,AttType,true);
		}
		void ViewDamage(OneObject* OBJ,int value);
		ViewDamage(Damager,-Damage);
		}
	}
}

//void ActiveUnitAbility::modifyDamagerLife		(word Basic, word& Current,int Damage,OneObject* Damager,OneObject* Victim){
//	SetUM(DamagerLife){
		//switch(M->SourceType){
		//case 0: //Damage
		//	break;
		//case 1: //VictimLife
		//	Damage=Victim->MaxLife;
		//	break;
		//case 2: //DamagerLife
		//	Damage=Damager->MaxLife;
		//	break;
		//};

		//M->Operand.Calculate(Damage,Damage);

//		if(M->LifeType==0){ // Vampire			
//			Current+=Damage;
//			if(Current>Damager->MaxLife) Current=Damager->MaxLife;
//		}else{ // Thorn
//			if(Current<Damage){
//				Current=0;
//				OneObject* Caster=NULL;
//				if(CasterID!=0xFFFF){
//					Caster=Group[CasterID];
//					if(Caster&&Caster->Serial!=CasterSN){
//						Caster=NULL;
//					}
//				}
//				if(Caster){
//					void OnDie(int Killer,int Victim);
//					OnDie(CasterID,Damager->Index);
//				}
//				Damager->Die();
//			}else{
//				Current-=Damage;
//			}
//
//			//Damager->MakeDamage(Damage,Damage,Caster,128+10,true); // 10 - some weapon type
//
//		}
//	}
//};
void ActiveUnitAbility::modifyAttackRadiusMax	(int Basic,int& Current){
	SetUM(AttackRadiusMax){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyWeapon			(Weapon* Basic,Weapon** Current,bool Action){	
	SetUM(Weapon){
		if(M->WeaponShots==0||WeaponShots<M->WeaponShots){
			WeaponModificator* WM=M->Weapon.Get();
			if(WM){
				Weapon* W=GetWeaponFromModificator(WM);
				if(W){
					//addrand(123456);
					*Current=W;
					if(Action){
						WeaponShots++;
					}					
				}
			}
		}
	}
}
void ActiveUnitAbility::modifyInvisibility		(bool Basic,bool& Current,byte& Detected,bool& EnableSearchVictim){
	SetUM(Invisibility){		
		if(M->Invisibility){			
			Current=true;
		}
		Detected|=OB->NMask;
		if(M->Detector){			
			Detected|=InvisDetected;
		}
		if(M->SearchVictim){			
			EnableSearchVictim=true;
		}
	}
};
bool isNotInvisible(OneObject* OB, byte SeekerNMask){
	if(OB&&OB->ActiveAbility){
		byte mask=0;
		bool inv=false;
		bool EnableSearchVictim;
		//addrand(72);
		OB->ActiveAbility->ActiveAbilities.modifyInvisibility(false,inv,mask,EnableSearchVictim); // isNotInvisible
		//addrand(71);
		if(inv){				
			if( !(mask&SeekerNMask) ){
				return false;
			}
		}
	}
	return true;
}
bool DisableInvisibility(OneObject* Damager){
	bool rez=false;
	if (Damager && Damager->ActiveAbility && Damager->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Invisibility)) {

		// Invisible
		Damager->ActiveAbility->ActiveAbilities.InvisibleEnableTime=TrueTime+1000;

		itr_AA.Create(Damager);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			bool del=false;
			itr_UM.Create(AA,ABL_Invisibility);
			while (mod_Invisibility* M=(mod_Invisibility*)itr_UM.Next()) {
				if(M->SearchVictim){
					return false;
				}
				if(M->Invisibility&&!M->Detector){
					del=true;
					rez=true;
				}
			}
			if(del){
				AA->DeleteTime=TrueTime;
			}
		}

	}
	return rez;
}
void ActiveUnitAbility::modifyManaValue			(int Basic,int& Current){
	SetUM(ManaValue){
		M->Operand.Calculate(Basic,Current);					
	}
}
void ActiveUnitAbility::modifyLife				(word Basic,word& Current){
	int ShotTime=LastLifeShotTime;
	SetUM(Life){

		//addrand(Current);

		if(Current==0){
			return;
		}
		if(M->Period==0) M->Period=1;

		int dT=TrueTime-BirthTime-M->FirstShotTime;
		if(dT>=0){
			dT=TrueTime-ShotTime;

			if(M->Shots==0) M->Shots=1;

			if(dT>=0 && M->Shots>LifeShot){
				LastLifeShotTime=TrueTime+M->Period;
				LifeShot++;

				int Prev=Current;

				int Bas=Basic;
				int Cur=Current;
				
				M->Operand.Calculate(Bas,Cur);

				if(Prev>Cur){
					// Damage					
					int Damage=Prev-Cur;

					if(M->MinLifeAfterDamage>0){
						int MaxDamage=Prev-M->MinLifeAfterDamage;
						if(MaxDamage<0) MaxDamage=0;
						if(Damage>MaxDamage) Damage=MaxDamage;
					}
					
					if(Damage>0){
						OneObject* Caster=NULL;
						if( CasterID!=0xFFFF && CasterID!=OB->Index ){
							Caster=Group[CasterID];
							if(Caster&&(Caster->Serial!=CasterSN||(OB->NMask&Caster->NMask))){
								Caster=NULL;
							}
						}

						int AttType=128+5;
						if( M->TakeProtection /*&& !M->ManaBurn*/ ) AttType=128+3; // 3 - magic

						if( OB->NNUM!=7/*!OB->AI_Guard*/ || OB->Attack || !GetA()->Aura ){
							if( M->ManaBurn ){
								if( OB->Mana<Damage )
									Damage=OB->Mana;
								OB->Mana-=Damage;
							}
							if(Damage>0){
								int BDM=1;
								if(Caster && M->IgnoreDamageOnBuildingMultiply){
									BDM=Caster->newMons->DamageOnBuildingMultiplicator;
									Caster->newMons->DamageOnBuildingMultiplicator=1;
								}                            
								OB->MakeDamage(Damage,Damage,Caster,AttType,true);
								if(Caster && M->IgnoreDamageOnBuildingMultiply){
									Caster->newMons->DamageOnBuildingMultiplicator=BDM;
								}
							}
						}
					}
				}else{
					// Heal
					Current=Cur;
					if( Prev<Cur && VUI() ){
						UnitAbility* UA=GetA();
						if( !(UA && UA->Aura || UA->ClassType==1) )
							VUI()->AddOnUnitInfoArray(OB);
					}
				}
			}
		}

		//addrand(Current);
	}
}
void ActiveUnitAbility::modifyMana				(word Basic, word& Current){
	SetUM(Mana){
		if(M->Period<=0) M->Period=1;

		int dT=TrueTime-BirthTime-M->FirstShotTime;
		if(dT>0){
			dT=TrueTime-LastManaShotTime;
			
			if(M->Shots==0) M->Shots=1;

			if(dT>=M->Period && M->Shots>ManaShot){
				int Bas=Basic;
				int Cur=Current;
				
				M->Operand.Calculate(Bas,Cur);

				if(Cur<0) Cur=0;
				Current=Cur;

				LastManaShotTime=TrueTime;
				ManaShot++;
			}
		}
	}
}
void ActiveUnitAbility::modifyLifeMax			(word Basic, word& Current){
	SetUM(LifeMax){
		M->Operand.Calculate(Basic,Current);					
		//addrand(Current);
	}
}
void ActiveUnitAbility::modifyManaMax			(word Basic, word& Current){
	SetUM(ManaMax){
		M->Operand.Calculate(Basic,Current);
		//addrand(Current);
	}
}

void ActiveUnitAbility::modifyMaterial			(byte Basic, byte& Current){
	SetUM(Material){
		switch(M->Operation){
			case 0:
				Current=1<<M->AddMatherial;
				break;
			case 1:
				Current&=1<<M->AddMatherial;
				break;
			case 2:
				Current|=1<<M->AddMatherial;
				break;
		};
	}
}

void ActiveUnitAbility::modifyManaRegeneration	(int Basic, int& Current){
	SetUM(ManaRegeneration){
		M->Operand.Calculate(Basic,Current);
	}
}

void ActiveUnitAbility::modifyVision			(byte Basic, byte& Current){
	if(vGameMode==gmCamp)
		return;
	SetUM(Vision){
		int b=Basic;
		int c=Current;
		M->Operand.Calculate(b,c);
		if(c>=0) 
			Current=c;
		else 
			Current=0;
	}
}

void ActiveUnitAbility::modifyMagicImmunity     (bool Basic,bool& Current,OneObject* Caster){
	//SetUM(MagicImmunity){
		Current=true;
	//}
};
void ActiveUnitAbility::modifySkillImmunity     (bool Basic,bool& Current,OneObject* Caster){
	//SetUM(MagicImmunity){
		Current=true;
	//}
};
void ActiveUnitAbility::modifyProduceEnable		(bool Basic, bool& Current){
	SetUM(ProduceEnable){
		Current=M->State;
	}
};
void ActiveUnitAbility::modifyScale				(word Basic, word& Current){
	SetUM(Scale){
		M->Operand.Calculate(Basic,Current);
	}
	if(Current==0) Current=1;
};
bool GetObjInsubordination(OneObject* OB){
	if(OB->ActiveAbility){
		//addrand(OB->Index);
		bool Insub=false;
		OB->ActiveAbility->ActiveAbilities.modifyInsubordination(false,Insub);
		return Insub;
	}
	return false;
}
void ActiveUnitAbility::modifyInsubordination	(bool Basic, bool& Current){
	SetUM(Insubordination){
		Current=true;
		return;
	}
};
void ActiveUnitAbility::modifyModelAnimation	(bool Basic,bool& Current){
	SetUM(ModelAnimation){
	}
};
void ActiveUnitAbility::modifyLevelForEffects	(int Basic, int& Current){
	SetUM(LevelForEffects){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyTradeSkill		(int Basic, int& Current){
	SetUM(TradeSkill){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyLearning			(int Basic, int& Current){
	SetUM(Learning){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyWeaponKind		(int Basic, int& Current, int AttType){
	SetUM(WeaponKind){
		if(M->AttType==AttType){
			Current=M->Kind;
		}
	}
};
void ActiveUnitAbility::modifyEvasion			(int Basic, int& Current){
	SetUM(Evasion){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyMiss				(int Basic, int& Current){
	SetUM(Miss){
		M->Operand.Calculate(Basic,Current);
	}
};
void ActiveUnitAbility::modifyExtraAnimation	(int Basic, int& Current){
	SetUM(ExtraAnimation){
		M->Modify(Basic,Current);
	}
}
void ActiveUnitAbility::modifyExtraAbility			(OneObject* NewUnit){
	SetUM(ExtraAbility){
		M->Modify(NewUnit);
	}
}
void ActiveUnitAbility::modifyDeleteAbility			(OneObject* NewUnit){
	SetUM(DeleteAbility){
		M->DeleteAb(NewUnit);
	}
}

bool CheckPossibilityToAttackEnemy(OneObject* OB,OneObject* EOB);
DLLEXPORT int GetRND(int Max);

void ActiveUnitAbility::modifySearchVictim		(OneObject*& Victim, int Radius, int& NShots){
	SetUM(SearchVictim){
		itr_UnitsInRadiusFromOtherNations.Create(OB->NNUM,OB->RealX>>4,OB->RealY>>4,Radius);
		int R=10000;
		switch(M->Type){
			case 0: { //HeroPriority
				OneObject* Hero=NULL;
				int RHero=10000;
				while(OneObject* VOB=itr_UnitsInRadiusFromOtherNations.Next()){
					if(CheckPossibilityToAttackEnemy(OB,VOB)){
						if( VOB->AI_Guard==false || VOB->Attack ){
							int r=Norma(VOB->RealX-OB->RealX,VOB->RealY-OB->RealY)>>4;
							//if(r>minR)r+=4000;
							if( RHero>r && DetectHero(VOB) ){
								Hero=VOB;
								RHero=r;
							}else
							if( R>r ){
								Victim=VOB;
								R=r;
							}
						}
					}
				}
				if(Hero){
					Victim=Hero;
					NShots=1;
				}
				break;
			}
			case 1: //Rabies
				NShots=1;
				while(OneObject* VOB=itr_UnitsInRadiusFromOtherNations.Next()){
					if(CheckPossibilityToAttackEnemy(OB,VOB)){
						if( VOB->AI_Guard==false || VOB->Attack ){
							int r=Norma(VOB->RealX-OB->RealX,VOB->RealY-OB->RealY)>>4;
							//if(r>minR)r+=4000;
							//if( /*r<R &&*/ (!Victim||GetRND(50)) ){
								R=r;
								Victim=VOB;
								if(GetRND(10)==7)
									break;
							//}
						}
					}
				}
				break;
			case 2: { //CrowdPriority
				int Box=Radius/100;
				break;
			}
		};
	}
};

void ActiveUnitAbility::modifyMotionStyle		(byte Basic, byte& Current){
	SetUM(MotionStyle){
		Current=M->Style;
	}
}

void ActiveUnitAbility::modifyLockType		(byte Basic, byte& Current){
	SetUM(LockType){
		Current=M->Type;
	}
}
void ActiveUnitAbility::modifyNMask		(byte Basic, byte& Current){
	SetUM(NMask){
		Current=M->NMask;
		//M->Operand.Calculate(Basic,Current);
	}
};

inline bool ActiveUnitAbility::isSilence(){
	SetUM(Silence){
		return true;
	}
	return false;
};
inline bool ActiveUnitAbility::isSkillSilence(){
	SetUM(SkillSilence){
		return true;
	}
	return false;
};
inline bool ActiveUnitAbility::isDeath(){
	SetUM(Death){
		return true;
	}
	return false;
};
inline bool ActiveUnitAbility::isInvulnerability(){
	SetUM(Invulnerability){
		return true;
	}
	return false;
};
inline bool ActiveUnitAbility::isCompassion(){
	SetUM(Compassion){
		return true;
	}
	return false;
};
inline bool ActiveUnitAbility::isSummon(){
	SetUM(Summon){
		return true;
	}
	return false;
};
inline bool ActiveUnitAbility::isFrozen(){
	SetUM(Frozen){
		return true;
	}
	return false;
};
inline bool ActiveUnitAbility::isCenterScreen(){
	SetUM(CenterScreen){
		return true;
	}
	return false;
};
void ActiveUnitAbility::execSpellOnAttack(OneObject* Enemy, int ActType,int TargetType){
	SetUM(SpellOnAttack){
		UnitAbility* UA=M->Spell.Get();
		if (ActType!=M->ActionType) break;
		if (TargetType!=M->CastType) break;
		if(UA){
			switch(TargetType){
		    case 0://CastOnOwner
				AddOneEffect(NULL,UA,OB,OB);
			  break;
			case 1://CastOnEnemy
				AddOneEffect(NULL,UA,Enemy,OB);
			  break;
			}
		}
	}
};
void ActiveUnitAbility::execDeleteAbilityOnReceiveDamage(){
	SetUM(DeleteAbilityOnReceiveDamage){
		Deleted=true;
		break;
	}
};
