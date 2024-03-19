#include "stdheader.h"
#include "ua_Ability.h"
#include "ua_Modificators.h"
#include "ua_Item.h"
#include "ua_Orders.h"
#include "ua_Extern.h"
#include "GameInterface.h"
#include "PluginsInterface.h"
#include "MassivePlayer.h"

//
//#include "UnitsInterface.h"
#include ".\cvi_MainMenu.h"
//==================================================================================================================//
//
word GetDir(int dx,int dy);
void ChangeUnitCoor(OneObject* OB,int newX,int newY);
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
//void ProduceObjLink(OneObject* OBJ);
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
DLLEXPORT int GetRND(int Max);
bool DrawSelPatch(float x,float y,int Type,float Radius,DWORD Color);
extern int TrueTime;

void EraseObjectWithoutAbilities(OneObject* OB);

//
bool DisableInvisibility(OneObject* Damager);
ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
bool TryAddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
HeroVariableStorage* DetectHero(OneObject* OB);
void RotUnitLink(OneObject* OB);
//==================================================================================================================//
NewAbility::NewAbility(){
	//SoundExecute=GetSound("potion_use");
	CastingShotAmount=1;
	//Type=0;
}
ActiveUnitAbility* NewAbility::GetNewActiveAbility(){
	OneAbility* nAA=new OneAbility;
	nAA->SetA(this);
	
	//
	nAA->ItemDrop=ItemDrop;
	nAA->Amount=Amount;
	//

	nAA->Opened=(NecessarySkills.GetAmount()==0);

	if(!Aura&&AutoCast){
		nAA->TypeAbil=6;
	}

	return nAA;
}

void CreateSupportSkillList(ActiveUnitAbility* AA, DynArray<WeaponModificator*>& eWeap){
	UnitAbility* UA=AA->GetA();
	if(UA){
		int n=UA->SupportSkills.GetAmount();
		for(int i=0;i<n;i++){
			UnitAbility* uaSS=UA->SupportSkills[i]->Get();
			if( uaSS && uaSS!=UA ){
				itr_AA.Create(AA->OB);
				while(ActiveUnitAbility* aa=itr_AA.Next()){
					if( uaSS==aa->GetA() && aa->TypeAbil==2 ){
						WeaponModificator* eWeapon=uaSS->eWeapon.Get();
						if(eWeapon){
							eWeap.Add(eWeapon);
						}
						CreateSupportSkillList(aa,eWeap);
					}
				}
			}
		}
	}
};

void MarkObjectLikeModified(OneObject* OB, bool InitAbility);

bool OneAbility::Process(){
	
	//if( OB->NewBuilding && !OB->Ready ) return true; //GEC- GecOptim Ubrano za nenadobnostiu
	//
	//be_Process.CallFunction((ActiveUnitAbility*)this);
	//

	OB->newMons->ItemDropValue=ItemDrop;

	NewAbility* UA=(NewAbility*)GetA();
	if(UA){

		// burning
		if( OB->newMons->NewParams.AntiMage && !UA->AntiMageNotBurning ){
			if( (UA->Type==0 || UA->Type==4) && !( TypeAbil==0 || TypeAbil==4 || TypeAbil==5) ){
				OB->ActiveAbility->AntiMageSkillPoints+=UA->ShopPrice;
				return false;
			}
		}

		addrand(TypeAbil);

		if(TypeAbil==0){
			//OneObject* Caster=Group[CasterID];
			GlobalUnitsContext::setCaster(CasterID,CasterSN);
		}else{
			GlobalUnitsContext::setCaster(OB->Index,OB->Serial);
		}		

		if( TypeAbil==0 || UA->ClassType!=0 ){
			UnitModificatorsIterator iterator_UM;
			iterator_UM.Create(this,ABL_CenterScreen);
			while(mod_CenterScreen* M=(mod_CenterScreen*)iterator_UM.Next()){
				// центрировать экран
				int x=OB->RealX>>4;
				int y=OB->RealY>>4;
				GameInterface::ScrollToPositionOnMap(x,y,400);
				if(UA->OnDieDeleteEffect){
					return false;
				}
				break;
			}
		}

		// --- hack ---

		if(TypeAbil!=0){
			//if(UA->Aura){
			//TypeAbil=2;
			if(UA->Type==3){
				TypeAbil=2;
			}
			//}
			if(!UA->Aura&&UA->AutoCast){
				TypeAbil=6;
			}
		}
		// --- hack ---

		if( TypeAbil==1 && !UA->Aura ){
			OB->ActiveAbility->CardManaRegeneration+=UA->ManaValue;//2;//UA->ManaValue/25;
		}

		// drop to ground
		if(Droped){
			//// DropPackage
			//uaa_DropPackage* aDP=dynamic_cast<uaa_DropPackage*>(this);
			//if(aDP){
			//	UA=NULL;
			//	ua_DropPackage* uDP=(ua_DropPackage*)aDP->GetA();
			//	int N=uDP->DropPackage.GetAmount();
			//	if(N>0){
			//		int i=0;
			//		float m=32768.0/N;
			//		do{
			//			int rnd=float(rando())/m;
			//			ClassRef<NewAbility>* rDP=uDP->DropPackage[rnd];
			//			if(rDP){
			//				UA=rDP->Get();
			//			}
			//			i++;
			//		}while(!UA&&i<N);
			//	}
			//	if(!UA){
			//		return false;
			//	}
			//}
			////
			//const int r=50*16;
			//int x=OB->RealX+r-(rando()%(r*2));
			//int y=OB->RealY+r-(rando()%(r*2));
			////int id=NATIONS[OB->NNUM].CreateNewMonsterAt(x,y,UA->GroundMD.UnitType/*NIndex*/,true);
			//SetActiveProducer();
			//int id=NATIONS[7].CreateNewMonsterAt(x,y,UA->GroundMD.UnitType/*NIndex*/,true);
			//SetActiveProducer(NULL);

			//addrand(id);
			//addrand(UA->GroundMD.UnitType);

			//if(id!=-1){
			//	void FindPositionForUnit(OneObject* OB);
			//	FindPositionForUnit(Group[id]);
			//	ActiveUnitAbility* newA=UA->GetNewActiveAbility();
			//	AddActiveUnitAbility(id,newA);
			//	if(TypeAbil==5){
			//		SeekSameAndClothe();
			//	}
			//	return false;
			//}
			return false;
		}

		if( TypeAbil==0 ){

			// End Effect
			if( DeleteTime<TrueTime && !UA->Permanent ){
				eLiveState LS=OB->GetLiveState();
				if (LS==ls_LiveBody || LS<ls_Erased&&UA->OnEndEffectErase) {
					WeaponModificator* WM=UA->OnEndEffectCreateWeapon.Get();
					if(WM){
						word UnitIndex=OB->Index;
						int x=OB->RealX>>4;
						int y=OB->RealY>>4;
						int z=OB->RZ+OB->OverEarth;

						addrand(CasterID);

						AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
						AWP->Radius=UA->Radius;
						CreateNewActiveWeapon(WM->Name.pchar(),CasterID,x,y,z,UnitIndex,x,y,z,AWP);
					}
					int sc=CITY[OB->NNUM].Account;
					if(UA->OnEndEffectErase){
						EraseObjectWithoutAbilities(OB);
					}else
						if(UA->OnEndEffectDie&&OB->Ready){//TheBlackHunter, ??.09.22. Mines deletes without OB->Ready
							OB->Die();
						}
						CITY[OB->NNUM].Account=sc;
				}

				// Teleport only when end effect
				if(UA->Teleport){
					int x=Tx*16;
					int y=Ty*16;
					OneObject* tOB=Group[tIndex];
					if( tIndex!=0xFFFF && tOB && tOB->Serial==tSerial ){
						x=tOB->RealX;
						y=tOB->RealY;
					}
					byte dir=GetDir(x-OB->RealX,y-OB->RealY);
					if(UA->Direction){				
						float fi=((float)dir/256.f)*(2.f*3.1415f);
						int dist=UA->Distance.Get();
						x=OB->RealX+((int)((float)dist*cosf(fi))*16);
						y=OB->RealY+((int)((float)dist*sinf(fi))*16);
					}else{
						OneObject* cOB=Group[CasterID];
						if( CasterID!=0xFFFF && cOB && cOB->Serial==CasterSN ){
							int dx=OB->RealX-cOB->RealX;
							int dy=OB->RealY-cOB->RealY;
							x += dx;
							y += dy;
						}
					}
					addrand(tx);
					addrand(ty);
					ChangeUnitCoor(OB,x,y);
					//OB->ClearOrders();
					//OB->DestX=-1;
					OB->RealDir=dir;
					OB->RealDirPrecise=dir<<8;
				}

				 // ƒобавить жизнь
                //if(UA->HeroLife){
                //    HeroVariableStorage* HVS=DetectHero(OB);
                //    if( HVS ){
                //        HVS->Lives++;
                //    }
                //}


                return false;
            }
            if( DeleteTime<TrueTime && UA->Permanent&&UA->EndEffectIfEnemyNear ){
                eLiveState LS=OB->GetLiveState();
                if (LS==ls_LiveBody || LS<ls_Erased&&UA->OnEndEffectErase) {
                    WeaponModificator* WM=UA->OnEndEffectCreateWeapon.Get();
                    if(WM){
                        word UnitIndex=OB->Index;
                        int x=OB->RealX>>4;
                        int y=OB->RealY>>4;
                        int z=OB->RZ+OB->OverEarth;

                        addrand(CasterID);

                        AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
                        AWP->Radius=UA->Radius;
                        CreateNewActiveWeapon(WM->Name.pchar(),CasterID,x,y,z,UnitIndex,x,y,z,AWP);
                    }
                    int sc=CITY[OB->NNUM].Account;
                    if(UA->OnEndEffectErase){
                        EraseObjectWithoutAbilities(OB);
                    }else
                        if(UA->OnEndEffectDie&&OB->Ready){//TheBlackHunter, ??.09.22. Mines deletes without OB->Ready
                            OB->Die();
                        }
                        CITY[OB->NNUM].Account=sc;
                }
            }

			if( OB->GetLiveState()!=ls_LiveBody ){
				if(UA->OnDieDeleteEffect){
					if(UA->OnEndEffectErase){
						EraseObjectWithoutAbilities(OB);
					}					
					return false;
				}else{
					return true;
				}
			}

			//	EndEffectIfEnemyNear
			if(UA->EndEffectIfEnemyNear){
				if((rando()%5)==0){
					if(TrueTime-eAuraStartTime>=UA->EndEffectIfEnemyNearStartTime){
						int R=UA->EndEffectIfEnemyNearRadius;
						itr_UnitsInRadius.Create(OB->RealX>>4,OB->RealY>>4,R);
						while(OneObject* oB=itr_UnitsInRadius.Next()){
							if(!(oB->Sdoxlo||(OB->NMask&oB->NMask))){
								DeleteTime=TrueTime;
							}
						}
					}
				}
			}
			//
			if(DieTime&&TrueTime>DieTime){
				//OB->Die();
				//return false;
			}

			// Modificators

			//bool fear=false;
			itr_UM.Create(this,ABL_Fear);
			while (mod_Fear* M=(mod_Fear*)itr_UM.Next()) {
				void PanicUnit(OneObject* OB);
				PanicUnit(OB);
			}
			//if (!fear && OB->LocalOrder&&OB->LocalOrder->DoLink==&PanicUnitLink) OB->ClearLastOrder();

			itr_UM.Create(this,ABL_AttackCaster);
			while( mod_AttackCaster* M=(mod_AttackCaster*)itr_UM.Next() ){
				if(ExecuteTime<TrueTime){
					ExecuteTime=TrueTime+UA->AuraCastPeriod;
					// проверить и навесить эффекты
					int r=UA->Radius.Get();
					int x=OB->RealX>>4;
					int y=OB->RealY>>4;
					//
					itr_UnitsInRadius.Create(x,y,r);
					while(OneObject* oB=itr_UnitsInRadius.Next()){
						if( oB->GetLiveState()==ls_LiveBody && (oB->MaxMana==0 || M->IncludeUnitWithMana) && !(/*DetectHero(oB) || */(oB->NMask&OB->NMask)) ){
							if(!oB->ActiveAbility){
								oB->ActiveAbility = new UnitActiveAbilityList(oB);
							}
							oB->ActiveAbility->AttackCasterID=OB->Index;
							oB->ActiveAbility->AttackCasterSN=OB->Serial;
						}
					}
				}
			}

		}
		if(  TypeAbil==0 || UA->ClassType==1 ){
			//if(OB->MotionStyle==8)
			//	MarkObjectLikeModified(OB,false);
			itr_UM.Create(this,ABL_OverEarth);
			while (mod_OverEarth* M=(mod_OverEarth*)itr_UM.Next()) {
				if(OB->MotionStyle==8){
					//OB->newMons->FlyHeight=M->FinalHeight;
				}else{
					if( OB->OverEarth>M->FinalHeight ){
						NewAnimation* naAtt9=OB->newMons->GetAnimation(anm_Attack+9);
						if( naAtt9 && naAtt9->Enabled && OB->NewState!=10 ){
							OB->NewState=10;
							OB->GroundState=10;
							TryToStand(OB,false);
							//return false;
						}
						OB->OverEarth-=M->Speed*GameSpeed/256;
						if(OB->OverEarth<M->FinalHeight) OB->OverEarth=M->FinalHeight;
					}else{
						OB->OverEarth+=M->Speed*GameSpeed/256;
						if(OB->OverEarth>M->FinalHeight) OB->OverEarth=M->FinalHeight;
					}
				}
				if (M->Die && !OB->Sdoxlo && OB->OverEarth==M->FinalHeight) {
					OB->Die();
				}
			}
		}
		if(OB->Sdoxlo){
			return true;
		}
		// States
		if (!EnableStates && UA->States.GetAmount()) {
			EnableStates=true;
			ChangeState(0);
		}
		if (EnableStates) {
			if(toState!=CurState){
				ua_State* S=UA->States[toState];
				if(S){
					if (S->TargetSelector.SelectArea) {
						if(AreaSelected){
							ChangeState(toState);
						}
					}else{
						//arX=OB->RealX>>4;
						//arY=OB->RealY>>4;
						arX=OB->GetAttX()>>4;
						arY=OB->GetAttY()>>4;
						ChangeState(toState);
					}
				}
			}
			//ua_State* S=UA->States[CurState];
			//if(S){
			//	for (int i=0; i<S->Impulses.GetAmount(); i++) {
			//		ua_Impulse* I=S->Impulses[i];
			//		if(I&&(!I->Event.Get()||I->Event.Get()->Check(this))){
			//			for(int j=0;j<I->Operations.GetAmount();j++){
			//				ua_Operation* O=I->Operations[j];
			//				if(O) O->Perform(this);
			//			}
			//		}
			//	}
			//	for (i=0; i<S->TimeLine.Tracks.GetAmount(); i++) {
			//		ua_Track* T=S->TimeLine.Tracks[i];
			//		if(T){
			//			// not started
			//			if (!trInit[i] && TrueTime>=T->StartTime) {
			//				trInit[i]=true;
			//			}
			//			// started
			//			if (trInit[i]) {
			//			}
			//		}
			//	}
			//}
			if (opDisappear) {
				return false;
			}
		}

		bool clothe = TypeAbil==4&&Clothed;
		if(clothe){
			CasterID=OB->Index;
		}

		if( TypeAbil==0 || clothe ){
			if(LastImpulseWeaponTime<TrueTime){
				LastImpulseWeaponTime=TrueTime+UA->ImpulseCreateWeaponPeriod;

				WeaponModificator* WM=UA->OnImpulseCreateWeapon.Get();
				if(WM){
					word UnitIndex=OB->Index;
					int x=OB->RealX>>4;
					int y=OB->RealY>>4;
					int z=OB->RZ+OB->OverEarth;

					addrand(CasterID);
					AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
					AWP->Radius=UA->Radius;
					CreateNewActiveWeapon(WM->Name.pchar(),CasterID,x,y,z,UnitIndex,x,y,z,AWP);
				}
			}
		}
		if( TypeAbil==0 ){

			bool Cond=UA->Conditions.Check(this);

			if(UA->Aura){				
				if(!Cond){
					return false;
				}
			}

			// Main condition visible
			if(UA->Visible){
				if(Cond){
					Visible=true;
				}
			}else{
				Visible=false;
			}

			// Permanent
			if(UA->Permanent){
				Visible=false;
			}

			// MultyCast+MaxCasts
			EffectCount=1;
			if ( UA->MultyCast){
				ActiveUnitAbility* First=NULL;
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if (AA->TypeAbil==0&& AA->GetA()==UA) {
						if(AA!=this){
							if ((!UA->MaxCasts)||(UA->MaxCasts&&AA->EffectCount<UA->MaxCasts)){
							AA->EffectCount++;
							Visible=false;
						}
						}
						break;
					}
				}
			}

		}else{

			NewAnimation* naDir=NULL;
			if(UA->DirectCasting){
				naDir=OB->newMons->GetAnimation(anm_CastDirect);
			}

			//
			// навешивание еффекта
			//
			UnitAbility* uaEff=UA;
			//if(UA->ClassType==1){
				uaEff=UA->eAbility.Get();
				if(!uaEff){
					uaEff=UA;
				}
			//}

			if( UA->Aura ){

				//if( AutoCast && UA->Button ){
				//		AutoCast=false;
				//	}
				//}

				if( AutoCast || !UA->Button ){

					// вычислить текущий кадр
					// Condition modified by Xeno
					if(GetAuraActiveState() && (UA->AuraCastPeriod == 0 || ExecuteTime < TrueTime))
					{

						int CastPeriod=UA->AuraCastPeriod;
						
						// отключить при безмолвии
						//bool silence=false;
						//OB->ActiveAbility->ActiveAbilities.modifySilence(silence,silence);

						bool Cond=UA->Conditions.Check(this);

						if( Cond/* && !silence*/){

							// анимаци€ директ кастинг
							if( naDir && naDir->Enabled ){
								OB->NewAnm=naDir;
								OB->SetZeroFrame();
								OB->InMotion=false;
							}

							// eWeapon
							WeaponModificator* WM=UA->eWeapon.Get();
							if(WM){
								int MV=UA->ManaValue; // использование маны дл€ оружи€ ауры
								OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);
								if(OB->Mana>=MV){
									OB->Mana-=MV;

									AdditionalWeaponParams* AWP = new AdditionalWeaponParams();

									AWP->Damage=UA->Damage.Get();
									AWP->AttType=0;
									AWP->Radius=UA->Radius.Get();

									AWP->EffectDuration=ExecuteTime+200;

									word UnitIndex=OB->Index;
									int x=OB->RealX>>4;
									int y=OB->RealY>>4;
									int z=OB->RZ+OB->OverEarth;
									if( naDir && naDir->Enabled ){
										int SC=int(naDir->Scale*100);
										x+=(SC*naDir->HotRadius)/100*TCos[OB->RealDir]>>8;
										y+=(SC*naDir->HotRadius)/100*TSin[OB->RealDir]>>8;
										z+=naDir->HotHeight*SC/100;
									}

									Tx=x;
									Ty=y;
									Tz=z;

									addrand(UnitIndex);

									CreateNewActiveWeapon(WM->Name.pchar(),UnitIndex,x,y,z,tIndex,Tx,Ty,Tz,AWP);
								}else{
									AutoCast=false;
								}

							}else{

								const int AddCastPeriod=2000;

								CastPeriod=CastPeriod/2+GetRND(CastPeriod);

								// проверить и навесить эффекты
								int r=UA->Radius.Get();
								int x=OB->RealX>>4;
								int y=OB->RealY>>4;

								if(UA->TargetMaxAmount==1&&UA->FriendlyTarget){
									itr_AA.Create(OB);
									bool find=false;
									while(ActiveUnitAbility* aA=itr_AA.Next()){
										ActiveUnitAbility* oA=(ActiveUnitAbility*)(aA);
										if(oA&&oA->ParentAbilitySerial==Serial){
											oA->DeleteTime=ExecuteTime+AddCastPeriod;
											find=true;
											break;
										}
									}
									if(!find){

										// добавить новый эффект
										ActiveUnitAbility* newA=AddOneEffect(NULL,uaEff,OB,OB);
										if(newA){
											newA->ParentAbilitySerial=Serial;
											newA->DeleteTime=ExecuteTime+AddCastPeriod;											
											newA->InvisDetected=OB->NMask;
										}

									}
								}else{
									itr_UnitsInRadius.Create(x,y,r);
									while(OneObject* oB=itr_UnitsInRadius.Next()){
										bool target=false;
										target|=UA->EnemyTarget&&!(oB->NMask&OB->NMask) || UA->FriendlyTarget&&(oB->NMask&OB->NMask);
										target&=UA->BodyStyleTarget&(1<<oB->newMons->BodyStyle);
										bool sum=false;
										if(oB->ActiveAbility)oB->ActiveAbility->ActiveAbilities.modifySummon(sum);
										target&=!UA->SummonTarget||sum;
										if(target){
											itr_AA.Create(oB);
											bool find=false;
											while(ActiveUnitAbility* aA=itr_AA.Next()){
												OneAbility* oA=(OneAbility*)(aA);
												if(oA&&oA->ParentAbilitySerial==Serial){
													oA->DeleteTime=ExecuteTime+AddCastPeriod;
													find=true;
													break;
												}
											}
											if(!find){
												// добавить новый эффект
												ActiveUnitAbility* newA=AddOneEffect(NULL,uaEff,oB,OB);
												if(newA){
													newA->ParentAbilitySerial=Serial;
													newA->DeleteTime=ExecuteTime+AddCastPeriod;											
													newA->InvisDetected=OB->NMask;
												}
											}
										}
									}
								}
							}
						}

						ExecuteTime=TrueTime+CastPeriod;
					}
				}
			}else{
				// не аура, создание оружи€
				if(Casting){

					if(CastingShotsPrepared>0){
						CastingShotsPrepared--;

						if(DeleteAfterUse){
							Deleted=true;
						}
						// Invisible
						if( UA->BreakInvisibility){
							//bool Death=false;
					        //OB->ActiveAbility->ActiveAbilities.modifyDeath(Death,Death);
					         if(!((OB->ActiveAbility)&&(OB->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Death)))){
		                       DisableInvisibility(OB);
	                       }
						}//by TheBlackHunter, edit pls
						//if( UA->BreakInvisibility )
						//	DisableInvisibility(OB);
						//OB->ActiveAbility->ActiveAbilities.InvisibleEnableTime=TrueTime-1;

						//CurShot++;
						//ExecuteTime=TrueTime+UA->CastingShotDuration;

						// создать суппортные AntiMage weapons
						DynArray<WeaponModificator*> eWeap;
						CreateSupportSkillList(this,eWeap);
						int n=eWeap.GetAmount();
						for(int i=0;i<n;i++){
							WeaponModificator* eW=eWeap[i];
							
							AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
							AWP->Damage=UA->Damage.Get();
							AWP->AttType=0;//UA->AttType;
							AWP->Radius=UA->Radius.Get();
							AWP->EffectDuration=UA->EffectDuration;

							word UnitIndex=OB->Index;
							int x=OB->RealX>>4;
							int y=OB->RealY>>4;
							int z=OB->RZ+OB->OverEarth;
							if( naDir && naDir->Enabled ){
								int SC=int(naDir->Scale*100);
								x+=(SC*naDir->HotRadius)/100*TCos[OB->RealDir]>>8;
								y+=(SC*naDir->HotRadius)/100*TSin[OB->RealDir]>>8;
								z+=naDir->HotHeight*SC/100;
							}
							addrand(UnitIndex);
							CreateNewActiveWeapon(eW->Name.pchar(),UnitIndex,x,y,z,tIndex,Tx,Ty,Tz,AWP);
						}
						
						// наличие сопр€женного оружи€
						WeaponModificator* WM=UA->eWeapon.Get();
						if(WM){
							AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
							AWP->Damage=UA->Damage.Get();
							AWP->AttType=0;//UA->AttType;
							AWP->Radius=UA->Radius.Get();
							AWP->EffectDuration=UA->EffectDuration;

							word UnitIndex=OB->Index;
							int x=OB->RealX>>4;
							int y=OB->RealY>>4;
							int z=OB->RZ+OB->OverEarth;
							if( naDir && naDir->Enabled ){
								int SC=int(naDir->Scale*100);
								x+=(SC*naDir->HotRadius)/100*TCos[OB->RealDir]>>8;
								y+=(SC*naDir->HotRadius)/100*TSin[OB->RealDir]>>8;
								z+=naDir->HotHeight*SC/100;
							}

							addrand(UnitIndex);
							CreateNewActiveWeapon(WM->Name.pchar(),UnitIndex,x,y,z,tIndex,Tx,Ty,Tz,AWP);

							if( VAI() ){
								int r=AWP->Radius;
								if(r<100)
									r=100;
								VAI()->OnCastSpell(OB->NNUM,UA,Tx,Ty,r);
							}

						}else{

							OneObject* tOB=Group[tIndex];
							if( tIndex!=0xFFFF && tOB && tOB->Serial==tSerial ){
								ActiveUnitAbility* AA=AddOneEffect(NULL,uaEff,tOB,OB);
								if(AA)
									AA->TargetUnitType=tOB->NIndex;
							}else{
								int r=UA->Radius.Get();
								int dist=UA->Distance.Get();
								if(dist==0){
									int x=OB->RealX>>4;
									int y=OB->RealY>>4;
									itr_UnitsInRadius.Create(x,y,r);
								}else{
									itr_UnitsInRadius.Create(Tx,Ty,r);
								}
								while(OneObject* oB=itr_UnitsInRadius.Next()){
									bool target=false;
									target|=UA->EnemyTarget&&!(oB->NMask&OB->NMask) || UA->FriendlyTarget&&(oB->NMask&OB->NMask);
									if(target){
										AddOneEffect(NULL,uaEff,oB,OB);
									}
								}

								if( VAI() )
									VAI()->OnCastSpell(OB->NNUM,UA,Tx,Ty,r<100?100:r);

							}

						}

						// Invisible
						//OB->ActiveAbility->ActiveAbilities.InvisibleEnableTime=TrueTime+1000;

					}else{

						Order1* OR=OB->LocalOrder;
						//if( /*Prepared &&*/ !OR || !(OR->DoLink==&PrepareSpellLink || OR->DoLink==&CastingSpellLink) )
						//	Casting=false;
						bool casting=false;
						while(OR){
							if( OR->DoLink==&PrepareSpellLink || OR->DoLink==&CastingSpellLink ){
								casting=true;
								break;
							}
							OR=OR->NextOrder;
						};
						if(!casting)
							Casting=false;

					}

				}
			}	
		}

		if(Deleted){
			return false;
		}
		return true;
	}
	return false;
}
bool OneAbility::isTarget(){
	if(TypeAbil==0||TypeAbil==4){
		return false;
	}
	NewAbility* UA=(NewAbility*)GetA();
	if(UA){
		if( OB->newMons->NewParams.AntiMage && UA->Type==7 && TypeAbil==1 ){
			return false;
		}
		int dist=UA->Distance.Get();
		if(dist==0){
			return false;
		}
		return !UA->Aura;
	}
	return true;
}

extern UnitActiveAbilityList* v_Targ_MA;

bool OneAbility::CanTarget(word TargetUnit, int x, int y, int z){
	//
	if(TypeAbil==0){
		return false;
	}
	NewAbility* UA=(NewAbility*)GetA();
	if (x > 0 && y > 0)
	{
		// by XENO: check if the ability can be casted over water
		if (!UA->OverWater)
		{
			MotionField *pMF = &MFIELDS[0];
			int ix = x>>4, iy = y>>4;
			if(pMF->CheckPt(ix, iy)) { overWater = true; return false; }
			else overWater = false;
		}
	}
	if(UA->Button){
		// мана
		if( !(UA->Aura&&AutoCast) ){
			int MV=UA->ManaValue; // CanTarget
			OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);
			int MoneyV=UA->MoneyValue;
			OB->ActiveAbility->ActiveAbilities.modifyManaValue(MoneyV,MoneyV);
			int Money=XRESRC(OB->NNUM,MoneyID);

			if( OB->Mana<MV || Money<MoneyV ){
				if(OB->ActiveAbility==v_Targ_MA){
					v_Targ_MA=NULL;
				}
				return false;
			}
		}
		// перезар€дка
		if(GetCoolDownProc()>0){
			return false;
		}
		if( x>=0 && y>=0 ){
			// разрешенные цели
			int N=UA->TargetMaxAmount;
			if( N==1 ){
				int dist=UA->NoCastIfOutOfDistance;
				if( dist>0 ){
					int d=Norma(x-OB->RealX/16,y-OB->RealY/16);
					if( d>dist )
						return false;
				}
				OneObject* obb=Group[TargetUnit];
				if( TargetUnit!=0xFFFF && obb ){
					bool res=false;
					if( !UA->BuildingTarget && obb->NewBuilding ){
						return false;
					}
					if( UA->EnemyTarget && !(obb->NMask&OB->NMask) ){
						res=true;
					}
					if( UA->FriendlyTarget && (obb->NMask&OB->NMask) ){
						res=true;
					}
					if(res){
						if(TryAddOneEffect(NULL,UA,obb,OB)){
							return true;
						}
					}
				}
				return false;
			}
		}
		return true;
	}
	return false;
}
bool OneAbility::OnClick(){	
	if (EnableStates) {
		evLeftClick=true;
		return true;
	}
	return Execute(OB->Index,OB->RealX>>4,OB->RealY>>4,OB->RZ);;
};

CEXPORT char* GetResourceName(byte ResID);
void AddIllustrator(int x,int y,char* text,RLCFont* Font,byte Type,DWORD Color);

bool OneAbility::OnRightClick(){
	NewAbility* UA=(NewAbility*)GetA();
	if( UA && !Deleted ){
		if( !UA->Aura && UA->MoveToEndAfterUse ){

			// перезар€дка
			int CD=GetCoolDownProc();
			// мана
			int MV=UA->ManaValue; // пропустить по RightClick
			OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);
			//MV>>=1; // понизить стоимость

			if( CD==0 && OB->Mana>=MV ){

				if( UA->SellAfterUse ){

					int GetAbilitySellPrice(OneObject* Seller, ActiveUnitAbility* AA);
					int price = GetAbilitySellPrice(OB,this);

					price*=1+GSets.RoomParams.StartRes*2;

					AddXRESRC(OB->NNUM,6,price);
					// hack begin
					for(int i=0;i<6;i++){
						SetXRESRC(OB->NNUM,i,0);
					}
					// hack end

					int x=OB->RealX>>4;
					int y=OB->RealY>>4;
					int z=OB->RZ+OB->OverEarth;

					byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
					if(NI==OB->NNUM){
						char s[64];
						sprintf(s,"+%d %s",price,GetResourceName(6));
						AddIllustrator(x+100-(rand()%200),y+100-(rand()%200),s,&SmallWhiteFont,254,0xFF00FFFF);
					}

					WeaponModificator* wG=v_ISys.eGain.Get();
					if(wG){
						CreateNewActiveWeapon(wG->Name.pchar(),OB->Index,x,y,z,OB->Index,x,y,z,NULL);						
					}

					Deleted=true;
				}

				OneObject* Lib=Group[OB->ActiveAbility->LibID];
				if( Lib && (OB->ActiveAbility->LibID==0xFFFF || OB->ActiveAbility->LibSN!=Lib->Serial) )
					Lib=NULL;

				addrand(OB->ActiveAbility->LibID);
				addrand(OB->ActiveAbility->LibSN);
				addrand(Clothed);

				if( Clothed || Lib ){
					MoveToEnd();
					if(Clothed){
						OB->Mana-=MV;
						eAuraStartTime=TrueTime;
					}
				}

				return true;
			}

		}
	}
	if(EnableStates){
		evRightClick=true;
		return true;
	}
	return MagicCardActive::OnRightClick();
};

extern ActiveUnitAbility* CurProcAA;

int OneAbility::GetCoolDownProc(){
	NewAbility* UA=(NewAbility*)GetA();
	if(UA){
		CurProcAA=this;
		if(CasterID==0xFFFF)
			GlobalUnitsContext::setCaster(OB->Index,OB->Serial);
		if( OB->newMons->NewParams.AntiMage && UA->Type==7 && TypeAbil==1 ){
			if( /*UA->AntiMageSupportSkill &&*/ !Opened )
				return 100;
			HeroVariableStorage* HVS=DetectHero(OB);
			if(HVS){
				HeroAbility* HA=(HeroAbility*)HVS->GetA();
				int LevelSP=HA->AntiMageLevels[HVS->AntiMageLevel];
				int SP=OB->ActiveAbility->AntiMageSkillPoints-LevelSP;
				return (SP>0)?0:100;
			}
			return 100;
		}else if(TypeAbil==0){
			if(!UA->Aura&&UA->EffectDuration){
				int dT=DeleteTime-TrueTime;
				//int c=100-100*dT/UA->EffectDuration;
				int c=100*dT/UA->EffectDuration;
				if(c<0)c=0;
				return c;
			}
		}else{

			// Silence
			if( TypeAbil==1 || TypeAbil==2) {
				if( UA->ManaValue>0 ){
					bool silence=false;
					OB->ActiveAbility->ActiveAbilities.modifySilence(silence,silence);
					if(silence){
						return 100;
					}
				}
			    if( UA->NonIgnoredSkill ){
					bool skillsilence=false;
					OB->ActiveAbility->ActiveAbilities.modifySkillSilence(skillsilence,skillsilence);
					if(skillsilence){
						return 100;
					}
				}
			}
			// проверка маны
			int MV=UA->ManaValue; // перезар€дка
			OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);

			int MoneyV=UA->MoneyValue;
			OB->ActiveAbility->ActiveAbilities.modifyManaValue(MoneyV,MoneyV);
			int Money=XRESRC(OB->NNUM,MoneyID);

			if( OB->Mana<MV || Money<MoneyV ){
				return 100;
			}

			//
			if(UA->CoolDownDuration){
				int dT=0;
				//int CD=UA->CoolDownDuration;
				//dT=TrueTime-CastingTime+CD;
				dT=CastingTime-TrueTime;
				if(dT<UA->CoolDownDuration){
					//int c=100-100*dT/CD;
					int c=100*dT/UA->CoolDownDuration;
					if(c<0)c=0;
					return c;
				}		
			}
		}
	}
	return 0;
};
void OneAbility::StartCoolDown(){

	eAuraStartTime=TrueTime;

	CastingTime=TrueTime;
	UnitAbility* UA=GetA();
	if(UA){
		CurProcAA=this;
		if(CasterID==0xFFFF)
			GlobalUnitsContext::setCaster(OB->Index,OB->Serial);
		CastingTime+=UA->CoolDownDuration;
	}

}

ActiveUnitAbility* AddAbility(UnitAbility* Source, OneObject* Dest, int TypeAbil);

bool OneAbility::Execute(word TargetUnit, int x, int y, int z){

	CurProcAA=this;
	GlobalUnitsContext::setCaster(OB->Index,OB->Serial);
	OneObject* Acceptor=Group[TargetUnit];
	if( TargetUnit!=0xFFFF && Acceptor )
		GlobalUnitsContext::setAcceptor(Acceptor->Index,Acceptor->Serial);

	//
	//be_LeftClick.CallFunction((ActiveUnitAbility*)this);
	//

	arX=x;
	arY=y;
	if (EnableStates) {
		AreaSelected=true;
		return true;
	}

	NewAbility* UA=(NewAbility*)GetA();

	if( OB->newMons->NewParams.AntiMage && UA->Type==7 && TypeAbil==1 ){
		if( GetCoolDownProc()==0 ){
			HeroVariableStorage* HVS=DetectHero(OB);
			if(HVS){
				HeroAbility* HA=(HeroAbility*)HVS->GetA();
				int L=HVS->AntiMageLevel;
				int NL=HA->AntiMageLevels.GetAmountOfElements()-1;
				if( L>NL )
					L=NL;
				int LevelSP=(L>=0)?HA->AntiMageLevels[L]:200;
				int SP=OB->ActiveAbility->AntiMageSkillPoints-LevelSP;
				if( SP>0 ){
					HVS->AntiMageLevel++;
					OB->ActiveAbility->AntiMageSkillPoints=SP;
					ActiveUnitAbility* AMS=NULL;
					itr_AA.Create(OB);
					while(ActiveUnitAbility* AA=itr_AA.Next()){
						if( AA->GetA()==UA && AA->TypeAbil==2 ){
							AMS=AA;
							//AMS->LevelUp();
							//if( UA->AntiMageSkillName>0 ){
							//	switch(UA->AntiMageSkillName){
							//		case 1:
							//			OB->ActiveAbility->AML_SkillA=AMS->Level;
							//			break;
							//		case 2:
							//			OB->ActiveAbility->AML_SkillB=AMS->Level;
							//			break;
							//	}
							//}
							break;
						}
					}
					if(!AMS){
						UnitAbility* UMS=GetA();
						AMS=AddAbility(UMS,OB,2);
						AMS->Level=0;
						if( true/*!UMS->AntiMageSupportSkill*/ ){
							// открыть SupportSkills
							DynArray<ActiveUnitAbility*> NS;
							itr_AA.Create(OB);
							while(ActiveUnitAbility* AA=itr_AA.Next()){
								UnitAbility* UA=AA->GetA();
								if( AA->TypeAbil==1 && UA && UA->NecessarySkills.GetAmount()>0 ){
									NS.Add(AA);
								}
							}
							int n=NS.GetAmount();
							for(int i=0;i<n;i++){
								ActiveUnitAbility* AA=NS[i];
								UnitAbility* UN=AA->GetA();
								bool open=true;
								int m=UN->NecessarySkills.GetAmount();
								for(int j=0;j<m;j++){
									UnitAbility* uaNS=UN->NecessarySkills[j]->Get();
									if( UMS!=uaNS ){
										bool find=false;
										itr_AA.Create(OB);
										while(ActiveUnitAbility* AA=itr_AA.Next()){
											if( AA->TypeAbil==2 && AA->GetA()==uaNS ){
												if( AA->Level>0 ){
													find=true;
													break;
												}
											}
										}
										if(!find){
											open=false;
											break;
										}
									}
								}
								AA->Opened=open;
							}
						}
					}
					AMS->LevelUp();
				}
			}
		}
		return true;
	}

	if(!CanTarget(TargetUnit,x,y,z)){
		return false;
	}
	if(UA->Aura){
		if(AutoCast){
			AutoCast=false;
		}else{
			WeaponModificator* WM=UA->eWeapon.Get();
			if(!WM){
				int MV=UA->ManaValue; // включить ауру
				OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);
				if(OB->Mana<MV){
					return false;
				}else{
					OB->Mana-=MV;
				}
			}
			if(UA->DisableAnotherAuras){
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					UnitAbility* UA=AA->GetA();
					if(UA&&UA->DisableAnotherAuras){
						AA->AutoCast=false;
					}
				}
			}
			AutoCast=true;			
		}		
		ExecuteTime=TrueTime;

		StartCoolDown();
		//bool Death=false;
     	//OB->ActiveAbility->ActiveAbilities.modifyDeath(Death,Death);
	    if(!((OB->ActiveAbility)&&(OB->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Death)))){
		         DisableInvisibility(OB);
	    }
	}else{
		// проверка маны
		int MV=UA->ManaValue; // проверка перед кастованием
		OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);
		if(OB->Mana<MV){
			return false;
		}

		StateTime=TrueTime;
		EffectTime=TrueTime;
		Casting=true;
		Prepared=false;

		OneObject* tOB=Group[TargetUnit];
		if(tOB&&UA->TargetMaxAmount==1){
			tIndex=TargetUnit;
			tSerial=tOB->Serial;
			Tx=tOB->RealX/16;
			Ty=tOB->RealY/16;
		}else{
			tIndex=0xFFFF;
			Tx=x;
			Ty=y;
			tOB=NULL;
		};

		Tz=GetHeight(x,y);

		OneObject* Caster=GlobalUnitsContext::getCaster();
		if(Caster){
			addrand(Caster->Index);
			addrand(Caster->Serial);
		}

		addrand(UA->Distance);
		const char* str=UA->Name.pchar();
		if(str){
			int n=strlen(str);
			for(int i=0;i<n;i++){
				if(str[i]){
					addrand(str[i]);
				}else{
					break;
				}
			}
		}

		CastingShotsPrepared=0;
		int OrdType=UA->Type!=4?UA->CastingOrderType:1;
		int dist=UA->Direction?0:UA->Distance;
		PrepareSpell(OB,OrdType,Serial,tOB,Tx,Ty,dist,UA->Radius,UA->CursType);
		
		UnitAbility* uaBefore=UA->eBeforeCast.Get();
		if(uaBefore){
			AddOneEffect(NULL,uaBefore,OB,OB);
		}

	}

	return true;
}

//en_AblExecState GetAblExecuteState(char* AbilityName, OneObject* Caster, OneObject* Target, int x, int y, int z){
en_AblExecState GetAblExecuteState(UnitAbility* UA, OneObject* Caster, OneObject* Target, int x, int y, int z){
	//if(AbilityName){
	if(UA){
		
		bool casting=false;
		bool find=false;

		itr_AA.Create(Caster);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			//if(AA->Name==AbilityName){
			if(AA->GetA()==UA){
				if(AA->Casting){
					return aes_Casting;
				}else{
					if(!AA->CanTarget(Target->Index,x,y,z)){
						return aes_CanNotTarget;
					}
				}
				find=true;
			}else{
				if(AA->Casting){
					casting=true;
				}
			}
		}
		if(find){
			if(casting){
				return aes_AnotherCasting;
			}
			return aes_WaitForCast;
		}

	}
	return aes_Absent;
};

//en_AblExecState AblExecute(char* AbilityName, OneObject* Caster, OneObject* Target, int x, int y, int z){
en_AblExecState AblExecute(UnitAbility* UA, OneObject* Caster, OneObject* Target, int x, int y, int z){
	//if(AbilityName){
	if(UA){

		bool casting=false;
		bool find=false;

		word Index=Target?Target->Index:0xFFFF;

		itr_AA.Create(Caster);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			//if(AA->Name==AbilityName){
			if( AA->GetA()==UA && (UA->Type!=7 || AA->OB->newMons->NewParams.AntiMage && AA->TypeAbil==2) ){
				if( AA->TypeAbil==5 ){
					if(AA->CanTarget(Index,x,y,z)){
						AA->OnClick();
						return aes_Casting;
					}
				}else{
					if(AA->Casting){
						return aes_Casting;
					}else{
						if(AA->CanTarget(Index,x,y,z)){
							AA->Execute(Index,x,y,z);
							return aes_Casting;
						}else{
							return aes_CanNotTarget;
						}
					}
				}
				find=true;
			}else{
				if(AA->Casting){
					casting=true;
				}
			}
		}
		if(find){
			if(casting){
				return aes_AnotherCasting;
			}
			return aes_WaitForCast;
		}

	}
	return aes_Absent;
};

extern int AFW_x,AFW_y,AFW_r,AFW_curs;
extern DWORD AFW_color;

bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);

bool OneAbility::DrawCursor(int x,int y){
	UnitAbility* UA = (UnitAbility*)GetA();
	if(UA){
		CurProcAA=this;
		GlobalUnitsContext::setCaster(OB->Index,OB->Serial);
		// рисуем курсор		
		int N=UA->TargetMaxAmount;
		if(N==1) return false;

		int r=UA->Radius.Get();

		DWORD color=UA->CursColor;

		int dist=UA->NoCastIfOutOfDistance;
		if( dist>0 ){
			int d=Norma(x-OB->RealX/16,y-OB->RealY/16);
			if( d>dist ){
				//color = (((color>>24)/3)<<24) + (color&0xFFFFFF);
				color = (0x10<<24) + (color&0xFFFFFF);
			}
		}
		if (overWater) color = 0xFFFF0000;
		if(true/*GetObjectVisibilityInFog(x,y,GetHeight(x,y),NULL)*/){
			GPS.FlushBatches();
			DrawSelPatch(x,y,UA->CursType,r,color);
			GPS.FlushBatches();
		}else{
			AFW_x=x;
			AFW_y=y;
			AFW_curs=UA->CursType;
			AFW_r=r;
			AFW_color=color;
		}
		
		// подсвечиваем наведенные юниты
		if(UA->EnemyTarget||UA->FriendlyTarget){
			itr_UnitsInRadius.Create(x,y,r);
			while(OneObject* obb=itr_UnitsInRadius.Next()){
				if( !obb->NewBuilding || UA->BuildingTarget ){
					eLiveState LS=obb->GetLiveState();
					if( UA->LiveBody&&LS==ls_LiveBody || UA->DeadBody&&LS==ls_DeadBody || UA->Bones&&LS==ls_Bones ){
						if( UA->EnemyTarget && !(obb->NMask&OB->NMask) ){
							obb->HighlightMask|=1;
							obb->HighlightMask|=8;
						}
						if( UA->FriendlyTarget && (obb->NMask&OB->NMask) ){
							obb->HighlightMask|=2;
							obb->HighlightMask|=8;
						}
					}
				}
			}
		}
	}
	return true;
}
//
int xAA, yAA, zAA;
NewAnimation* OneAbility::GetAuraAnimation(){
	NewAbility* UA=(NewAbility*)GetA();
	if( UA && UA->Conditions.Check(this) ){
		int n=UA->VisualEffects.GetAmount();
		for(int i=0;i<n;i++){
			UA->VisualEffects[i]->Process(this,i);
		}
	}
	return NULL;
};
bool OneAbility::CheckAnimationMask(){
	NewAbility* UA=(NewAbility*)GetA();
	if(UA){		
		int n=UA->VisualEffects.GetAmount();
		for(int i=0;i<n;i++){
			if( UA->VisualEffects[i]->CheckDraw(this) ){
				return true;
			};
		}
	}
	return false;
}
//

ActiveUnitAbility* AddCarryAbility(OneObject* OB, UnitAbility* UA, bool DeleteAfterUse){
	if(OB&&UA){
		// AntiMageSkills
		bool AddSkill=false;
		if( OB->newMons->NewParams.AntiMage &&  UA->Type==7 ){
			itr_AA.Create(OB);
			while( ActiveUnitAbility* AA=itr_AA.Next() ){
				if( UA==AA->GetA() ){
					if( AA->TypeAbil==1 ){
						AddSkill=true;
					}else{
						//AA->Level++;
						AA->LevelUp();
						return AA;
					}
				}
			}
		}
		ActiveUnitAbility* A=UA->GetNewActiveAbility();
		if(AddSkill){
			A->TypeAbil=2;
		}
		AddActiveUnitAbility(OB->Index,A);
		A->DeleteAfterUse=DeleteAfterUse;
		return A;
	}
	return NULL;
}

ActiveUnitAbility* DetectCarryAbility(OneObject* OB, UnitAbility* UA){
	itr_AA.Create(OB);
	while(ActiveUnitAbility* A=itr_AA.Next()){
		if (A->TypeAbil!=0 && A->GetA()==UA){
			return A;
		}
	}
	return NULL;
};

bool DeleteCarryAbility(OneObject* OB, UnitAbility* UA){
	if(UA){
		itr_AA.Create(OB);
		while(ActiveUnitAbility* A=itr_AA.Next()){
			if( A->GetA()==UA && !A->Deleted ){
				A->Deleted=true;
				return true;
			}
		}
	}
	return false;
};

bool DeleteCarryAbility(OneObject* OB, char* Name){
	if(Name){
		itr_AA.Create(OB);
		while(ActiveUnitAbility* A=itr_AA.Next()){
			if( A->Name==Name ){
				A->Deleted=true;
				return true;
			}
		}
	}
	return false;
};

bool MagicImmunityResponse;
bool SkillImmunityResponse;

bool CheckAddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster){
	if(UA&&Dest){

		MagicImmunityResponse=false;
		SkillImmunityResponse=false;

		if( DetectChest(Dest) ){
			return false;
		}

		if(Dest->ActiveAbility){

			// MagicImmunity
			if( !(UA->IgnoreMagicImmunity||UA->Permanent) ){
				bool Immunity=false;
				Dest->ActiveAbility->ActiveAbilities.modifyMagicImmunity(Immunity,Immunity,Caster);
				if(Immunity){
					MagicImmunityResponse=true;
					return false;
				}
			}
			//SkillImmunity
			if (UA->NonIgnoredSkill ){
				bool SkillImmunity=false;
				Dest->ActiveAbility->ActiveAbilities.modifySkillImmunity(SkillImmunity,SkillImmunity,Caster);
				if(SkillImmunity){
					SkillImmunityResponse=true;
					return false;
				}
			}

			// Blocking
			for(int i=0;i<UA->Blocking.GetAmount();i++){
				itr_AA.Create(Dest);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					UnitAbility* UA1=AA->GetA();
					UnitAbility* UA2=UA->Blocking[i]->Get();
					if (UA1 && UA1==UA2) {
						return false;
					}
				}
			}

		}

		return true;
	}
	return false;
}
bool TryAddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster){
	if(CheckAddOneEffect(WP,UA,Dest,Caster)){

		// check main Condition
		static ActiveUnitAbility A;
		A.SetA(UA);
		A.TypeAbil=0;
		A.EffectTime=TrueTime;
		A.OB=Dest;
		//A.UnitIndex=Dest->Index;
		A.CasterID=Caster->Index;
		A.CasterSN=Caster->Serial;

		bool cond=UA->Conditions.Check(&A);

		if(cond){
			return true;
		}

	}
	return false;
}

void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);

ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster){
	
	if(CheckAddOneEffect(WP,UA,Dest,Caster)){

		// Single Cast
		if( !UA->MultyCast ){
		//if( !UA->MultyCast || UA->Aura ){
			//int ncast=0;
			addrand(135);
			itr_AA.Create(Dest);
			while(ActiveUnitAbility* AA=itr_AA.Next()){
				if( AA->TypeAbil==0 && AA->GetA()==UA ){
					if(UA->Aura){
						if(WP){
							AdditionalWeaponParams* AWP=WP->AdditionalParams.Get();
							if( AWP && AWP->EffectDuration ){
								AA->DeleteTime=AWP->EffectDuration;
								addrand(AA->DeleteTime);
							}
						}
					}else{
						AA->DeleteTime=TrueTime+UA->EffectDuration;
						AA->CasterID=Caster->Index;
                        AA->CasterSN=Caster->Serial;
						addrand(UA->EffectDuration);
					}

					AA->LifeShot=0;
					AA->ManaShot=0;
					addrand(881);

					//ncast++;
					return NULL;
				}
			}
			//if( ncast>4 || ncast>0&&!UA->Aura )
				//return NULL;
		}
   if((UA->MultyCast )){
			int ncast=1;
			itr_AA.Create(Dest);
			while(ActiveUnitAbility* AA=itr_AA.Next()){
				if( AA->TypeAbil==0 && AA->GetA()==UA ){
					ncast++;
				}
			}
			if(UA->MaxCasts&&ncast>UA->MaxCasts)
				return NULL;
		}
		ActiveUnitAbility* newA=UA->GetNewActiveAbility();
		newA->TypeAbil=0;
		newA->EffectTime=TrueTime;
		if(Caster){
			addrand(194);
			newA->CasterID=Caster->Index;
			newA->CasterSN=Caster->Serial;
			HeroVariableStorage* HVS=DetectHero(Caster);
			if(HVS){
				newA->CasterLevel=HVS->Level;
				addrand(UA->Index);
			}
		}else{
			newA->CasterID=0xFFFF;
			addrand(333);
		}
		newA->Visible=false;

		//OneAbility* OA=dynamic_cast<OneAbility*>(newA);
		ActiveUnitAbility* OA=newA;
		addrand(111);
		if(OA){
			addrand(222);

			if(WP){
				// detector
				OneObject* From=Group[WP->From.UnitIndex];
				if(From){
					OA->InvisDetected=From->NMask;
				}
			}

			if(UA->Aura){
				if(WP){
					AdditionalWeaponParams* AWP=WP->AdditionalParams.Get();
					if( AWP && AWP->EffectDuration ){
						OA->DeleteTime=AWP->EffectDuration;
					}
				}
			}else{
				int duration=UA->EffectDuration;
				if(duration<=0){
					duration=500;
				}

				// ----- EDIT BY XENO -------
				// Special case for RTS+RPG mode
				if( GSets.RoomParams.RTS_mode == 3 && UA && !UA->Name.isClear() && strstr(UA->Name.pchar(),"MC_aSummonPeasants"))
				{
					duration = 6000;
				}

				OA->DeleteTime=TrueTime+duration;
			}

			OA->eAuraStartTime=TrueTime;
			OA->eHandler=GetTickCount();
		}
		
		if(!AddActiveUnitAbility(Dest->Index,newA)){
			delete newA;
			return NULL;
		};
		//ExtraAbility
		// Dispell
		bool Dispell=newA->InfluenceMask.check_bit(ABL_MagicImmunity);
		if(Dispell){
			itr_AA.Create(Dest);
			while(ActiveUnitAbility* A=itr_AA.Next()){
				UnitAbility* U=A->GetA();
				if(U){
					if( A->TypeAbil==0 && !(U->	IgnoreMagicImmunity||U->Permanent||A->InfluenceMask.check_bit(ABL_MagicImmunity)) ){
												
						A->Deleted=true;
						addrand(UA->Index);

					}
				}
			}
		}
		// Diskill
		bool Diskill=newA->InfluenceMask.check_bit(ABL_SkillImmunity);
		if(Diskill){
			itr_AA.Create(Dest);
			while(ActiveUnitAbility* A=itr_AA.Next()){
				UnitAbility* U=A->GetA();
				if(U){
					if(A->TypeAbil==0 &&U->NonIgnoredSkill ){
												
						A->Deleted=true;
						addrand(UA->Index);

					}
				}
			}
		}

		if(Dest->ActiveAbility){
			addrand(UA->Index);
			// UnCast
			for(int i=0;i<UA->UnCast.GetAmount();i++){
				addrand(Dest->Index);
				itr_AA.Create(Dest);
				while(ActiveUnitAbility* A=itr_AA.Next()){
					if( A->TypeAbil==0 ){
						UnitAbility* UA1=A->GetA();
						UnitAbility* UA2=UA->UnCast[i]->Get();
						if( UA1 && UA1==UA2 ){
							A->Deleted=true;
							//A->DeleteTime=TrueTime;
							addrand(UA->Index);
						}
					}
				}
			}
		}

		if( /*!UA->AntiMageSupportSkill &&*/ Caster ){
			int n=UA->SupportSkills.GetAmount();
			for(int i=0;i<n;i++){
				UnitAbility* uaSS=UA->SupportSkills[i]->Get();
				if( uaSS && uaSS!=UA ){
					bool find=false;
					itr_AA.Create(Caster);
					while(ActiveUnitAbility* aa=itr_AA.Next()){
						if( uaSS==aa->GetA() && aa->TypeAbil==2 ){
							find=true;
							break;
						}
					}
					if(find){
						WeaponModificator* eWeapon=uaSS->eWeapon.Get();
						if(eWeapon){
							//word UnitIndex=OB->Index;
							int sx=Caster->RealX>>4;
							int sy=Caster->RealY>>4;
							int sz=Caster->RZ+Caster->OverEarth;
							int dx=Dest->RealX>>4;
							int dy=Dest->RealY>>4;
							int dz=Dest->RZ+Dest->OverEarth;
							CreateNewActiveWeapon(eWeapon->Name.pchar(),Caster->Index,sx,sy,sz,Dest->Index,dx,dy,dz,NULL);
						}else
							AddOneEffect(WP,uaSS,Dest,Caster);
					}
				}
			}
		}

		return newA;
	}
	if(MagicImmunityResponse){
		WeaponModificator* wG=v_ISys.eMagicImmunityResponse.Get();
		if(wG){
			//word UnitIndex=OB->Index;
			int sx=Caster->RealX>>4;
			int sy=Caster->RealY>>4;
			int sz=Caster->RZ+Caster->OverEarth;

			int dx=Dest->RealX>>4;
			int dy=Dest->RealY>>4;
			int dz=Dest->RZ+Dest->OverEarth;

			CreateNewActiveWeapon(wG->Name.pchar(),Caster->Index,sx,sy,sz,Dest->Index,dx,dy,dz,NULL);
		}
	}
	if(SkillImmunityResponse){
		WeaponModificator* wG=v_ISys.eSkillImmunityResponse.Get();
		if(wG){
			//word UnitIndex=OB->Index;
			int sx=Caster->RealX>>4;
			int sy=Caster->RealY>>4;
			int sz=Caster->RZ+Caster->OverEarth;

			int dx=Dest->RealX>>4;
			int dy=Dest->RealY>>4;
			int dz=Dest->RZ+Dest->OverEarth;

			CreateNewActiveWeapon(wG->Name.pchar(),Caster->Index,sx,sy,sz,Dest->Index,dx,dy,dz,NULL);
		}
	}

	return NULL;
}
//
bool ActiveUnitAbility::ChangeState(int NewState){
	CurState=NewState;
	StateTime=TrueTime;
	memset(trInit,0,sizeof trInit);
	//memset(trInit,0,sizeof trInit);
	evLeftClick=false;
	evRightClick=false;
	evTimelineIsDone=false;
	opDisappear=false;
	AreaSelected=false;	
	
	v_Targ_MA=NULL;
	
	UnitAbility* UA=GetA();
	if(UA){
		int r=UA->Radius.Get();
		//int dist=UA->Distance.Get();
		//if(dist==0){
		//	int x=OB->RealX>>4;
		//	int y=OB->RealY>>4;
		//	itr_UnitsInRadius.Create(x,y,r);
		//}else{
			itr_UnitsInRadius.Create(arX,arY,r);
		//}
		while(OneObject* oB=itr_UnitsInRadius.Next()){
			ActiveUnitAbility* A=AddOneEffect(NULL,UA,OB,OB);
			if(A){
				A->CurState=CurState;
				A->toState=CurState;
				A->EnableStates=true;
				A->SetInfluenceMask();
				OB->ActiveAbility->ActiveAbilities.ResetInfluenceMask();
			}
		}
	}
	return true;
};

//////////////////////////////////////////////////////////////////////////
//void FallUnitLink(OneObject* OB){	
//	int dt=TrueTime-OB->LocalOrder->info.BuildObj.ObjIndex;
//	if(dt<0)return;	
//	int sp=800+(OB->Index%30)*30;
//    OB->OverEarth=OB->LocalOrder->info.BuildObj.ObjX-dt*dt/sp;
//	if(OB->OverEarth<-1500){
//		OB->Die();
//		OB->OverEarth=-1500;
//	}
//}
//void FallUnit(OneObject* OB,int Timeout){
//    OB->UnlimitedMotion=false;
//	Order1* OR=OB->CreateOrder(0);
//	if(OR){
//		OR->info.BuildObj.ObjIndex=TrueTime+Timeout;
//		OR->info.BuildObj.ObjX=OB->OverEarth;
//        OR->DoLink=&FallUnitLink;
//	}
//	OB->UnlimitedMotion=true;
//}

inline ActiveUnitAbility* GetActAblByName(OneObject* OB, const char* Name){
	itr_AA.Create(OB);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		if(AA->Name==Name || ( AA->GetA() && AA->GetA()->Name==Name )){
			return AA;
		}
	}
	return NULL;
}

void		SetAblState( OneObject* pOB, const char* AblName, enABL_STATE state ) {
	ActiveUnitAbility* AA=GetActAblByName(pOB,AblName);
	if(AA){
		switch(state){
			case ABL_ACTIVE:
				AA->Visible=true;
				if(AA->TypeAbil==4)
					AA->Clothed=true;
				break;
			case ABL_PASIVE:
				AA->Visible=true;
				if(AA->TypeAbil==4)
					AA->Clothed=false;
				break;
			case ABL_HIDE:
				AA->Visible=false;
				break;
		};
	}
};
enABL_STATE	GetAblState( OneObject* pOB, const char* AblName ) {
	ActiveUnitAbility* AA=GetActAblByName(pOB,AblName);
	if(AA){
		if(AA->Visible){			
			OneBonus* Bonus=DetectBonus(pOB);
			if( Bonus && Bonus->Charge==0 )
				return ABL_PASIVE;
			int CoolDown=AA->GetCoolDownProc();
			if(CoolDown!=0){
				return ABL_RECHARGE;
			}
			if( AA->TypeAbil==4 && !AA->Clothed )
				return ABL_PASIVE;
			return ABL_ACTIVE;
		}else{
			return ABL_HIDE;
		}
	}
	return ABL_ABSENT;
};
int			GetAblList( OneObject* pOB, ClonesArray<_str>& AblNameList ) {
	AblNameList.Clear();
	return 0;
};

///////////////////////////////////////////////////////////////////////////////////////

ActiveUnitAbility* ua_Image::GetNewActiveAbility(){
	aa_Image* nAA=new aa_Image;
	nAA->SetA(this);
	
	nAA->ItemDrop=ItemDrop;
	
	if(!Aura&&AutoCast){
		nAA->TypeAbil=6;
	}

	return nAA;
};
ActiveUnitAbility* ua_Effect::GetNewActiveAbility(){
	aa_Effect* nAA=new aa_Effect;
	nAA->SetA(this);
	//
	nAA->ItemDrop=ItemDrop;
	//

	if(!Aura&&AutoCast){
		nAA->TypeAbil=6;
	}

	return nAA;
};

void OneAbility::CastingSpell() {

	UnitAbility* UA=GetA();
	if(UA){

		CurProcAA=this;
		GlobalUnitsContext::setCaster(OB->Index,OB->Serial);

		if( !Prepared /*&& OB->LocalOrder->DoLink==&CastingSpellLink*/ ){

			bool prep=false;
			NewAnimation* naPrp=NULL;
			if(UA->PrepareCasting){
				naPrp=OB->newMons->GetAnimation(anm_CastPrepare);
			}
			if( naPrp && naPrp->Enabled ){
				// анимаци€ подготовки
				if(OB->NewAnm!=naPrp){
					OB->NewAnm=naPrp;
					OB->SetZeroFrame();
				}							
				int af=naPrp->ActiveFrame;
				if( af==0 || af==0xFF ){
					af=naPrp->NFrames*3/4;
				}
				if( OB->NewCurSpritePrev<af && OB->NewCurSprite>=af )
					prep=true;
			}else{
				prep=true;
			}
			if(prep){
				ExecuteTime=TrueTime;

				//// трата маны
				//int MV=UA->ManaValue; // использование маны при кастовании
				//OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);

				//int MoneyV=UA->MoneyValue;
				//OB->ActiveAbility->ActiveAbilities.modifyManaValue(MoneyV,MoneyV);
				//int Money=XRESRC(OB->NNUM,MoneyID);

				//if( OB->Mana>=MV && Money>=MoneyV ){

				//	// кастуем спел
				//	OB->Mana-=MV;
				//	AddXRESRC(OB->NNUM,MoneyID,-MoneyV);

					Prepared=true;
					CurShot=0;

				//	itr_AA.Create(OB);
				//	while(ActiveUnitAbility* aa=itr_AA.Next()){
				//		if( aa->TypeAbil!=0 ){
				//			UnitAbility* ua=aa->GetA();
				//			if(ua&&ua->Index==UA->Index){
				//				aa->StartCoolDown();
				//			}
				//		}
				//	}

				//	if(UA->MoveToEndAfterUse){
				//		//MoveToEnd=true;
				//	}
				//	DisableInvisibility(OB);
				//	if(!UA->UnlimitedAmount){
				//		Deleted=true;
				//	}

				//}else{
				//	Casting=false;
				//	OB->DeleteLastOrder();
				//}

			}
		}
		if( Prepared ){

			bool stun=false;
			int speed=100;
			OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(speed,speed,stun); // оглушили во врем€ кастовани€
			if( UA->ManaValue>0 )
				OB->ActiveAbility->ActiveAbilities.modifySilence(stun,stun);

			NewAnimation* naDir=NULL;
			if(UA->DirectCasting){
				naDir=OB->newMons->GetAnimation(anm_CastDirect);
			}

			if(!stun){
				if(ExecuteTime<=TrueTime){
					bool active= true;
					if( naDir && naDir->Enabled ){
						if( OB->NewAnm!=naDir || OB->FrameFinished ){
							OB->NewAnm=naDir;
							OB->SetZeroFrame();
						}
						int af=naDir->ActiveFrame;
						active = ( UA->CastingShotAmount>1 || af==0 || af==0xFF || OB->NewCurSpritePrev<af && OB->NewCurSprite>=af );
					}
					if(active){

						if(CurShot==0){
							// трата маны
							int MV=UA->ManaValue; // использование маны при кастовании
							OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);

							int MoneyV=UA->MoneyValue;
							OB->ActiveAbility->ActiveAbilities.modifyManaValue(MoneyV,MoneyV);
							int Money=XRESRC(OB->NNUM,MoneyID);

							if( OB->Mana>=MV && Money>=MoneyV ){

								// кастуем спел
								OB->Mana-=MV;
								AddXRESRC(OB->NNUM,MoneyID,-MoneyV);

								//Prepared=true;
								//CurShot=0;

								itr_AA.Create(OB);
								while(ActiveUnitAbility* aa=itr_AA.Next()){
									if( aa->TypeAbil!=0 ){
										UnitAbility* ua=aa->GetA();
										if( ua && ua->Index==UA->Index ){
											aa->StartCoolDown();
											//break;
										}
									}
								}

								// EDIT BY XENO: removed to implement card amount decrease
								if(UA->Consumable)
									//MoveToEnd();

								//DisableInvisibility(OB);
								//if(!UA->UnlimitedAmount){	
									if( UA->Type==0 && UA->Amount )
									{
										Amount--;
										if(Amount<=0) Deleted=true;
									}
									else Deleted=true;
								//} -------

								if( UA->FirstDirectSoundID!=-1 /*&& OB->NNUM==MyNation*/ )
									AddEffect(OB->RealX/16,OB->RealY/16,UA->FirstDirectSoundID);

							}else{
								//Casting=false;
								//OB->DeleteLastOrder();
								stun=true;
							}
						}

						if(!stun){
							CastingShotsPrepared++;

							//if(DeleteAfterUse){
							//	Deleted=true;
							//}

							CurShot++;
							ExecuteTime=TrueTime+UA->CastingShotDuration;

							//// наличие сопр€женного оружи€
							//WeaponModificator* WM=UA->eWeapon.Get();
							//if(WM){
							//	AdditionalWeaponParams* AWP = new AdditionalWeaponParams();

							//	AWP->Damage=UA->Damage.Get();
							//	AWP->AttType=0;//UA->AttType;
							//	AWP->Radius=UA->Radius.Get();

							//	AWP->EffectDuration=UA->EffectDuration;

							//	word UnitIndex=OB->Index;
							//	int x=OB->RealX>>4;
							//	int y=OB->RealY>>4;
							//	int z=OB->RZ+OB->OverEarth;
							//	if(naDir){
							//		int SC=int(naDir->Scale*100);
							//		x+=(SC*naDir->HotRadius)/100*TCos[OB->RealDir]>>8;
							//		y+=(SC*naDir->HotRadius)/100*TSin[OB->RealDir]>>8;
							//		z+=naDir->HotHeight*SC/100;
							//	}

							//	addrand(UnitIndex);
							//	CreateNewActiveWeapon(WM->Name.pchar(),UnitIndex,x,y,z,tIndex,Tx,Ty,Tz,AWP);

							//}else{

							//	OneObject* tOB=Group[tIndex];
							//	if( tIndex!=0xFFFF && tOB && tOB->Serial==tSerial ){
							//		AddOneEffect(NULL,uaEff,tOB,OB);
							//	}else{
							//		int r=UA->Radius.Get();
							//		int dist=UA->Distance.Get();
							//		if(dist==0){
							//			int x=OB->RealX>>4;
							//			int y=OB->RealY>>4;
							//			itr_UnitsInRadius.Create(x,y,r);
							//		}else{
							//			itr_UnitsInRadius.Create(Tx,Ty,r);
							//		}
							//		while(OneObject* oB=itr_UnitsInRadius.Next()){
							//			bool target=false;
							//			target|=UA->EnemyTarget&&!(oB->NMask&OB->NMask) || UA->FriendlyTarget&&(oB->NMask&OB->NMask);
							//			if(target){
							//				AddOneEffect(NULL,uaEff,oB,OB);
							//			}
							//		}
							//	}

							//}
						}

					}
				}
			}
			
			if(stun||!naDir||CurShot>=UA->CastingShotAmount){

				//if( CastingShotsPrepared<=0 ){
					//Casting=false;
					OB->DeleteLastOrder();
					ExecuteTime=TrueTime+5000;
				//}

			}

		}

	}
};
void OneAbility::DropDown(int x, int y){
	UnitAbility* UA=GetA();
	if( UA && OB->OverEarth>-100 ){
		// DropPackage
		uaa_DropPackage* aDP=dynamic_cast<uaa_DropPackage*>(this);
		if(aDP){
			UA=NULL;
			ua_DropPackage* uDP=(ua_DropPackage*)aDP->GetA();
			int N=uDP->DropPackage.GetAmount();
			if(N>0){
				int i=0;
				float m=32768.0/N;
				do{
					int rnd=float(rando())/m;
					ClassRef<NewAbility>* rDP=uDP->DropPackage[rnd];
					if(rDP){
						UA=rDP->Get();
					}
					i++;
				}while(!UA&&i<N);
			}
			if(!UA){
				return;
			}
		}

		Droped=true;

		ext_OnDropItem(OB->Index,this);

		if( x<0 ){
			const int r=50*16;
			x=OB->RealX+r-(rando()%(r*2));
			y=OB->RealY+r-(rando()%(r*2));
		}else{
			x<<=4;
			y<<=4;
		}
		SetActiveProducer();
		int id=NATIONS[7].CreateNewMonsterAt(x,y,UA->GroundMD.UnitType,true);
		SetActiveProducer(NULL);

		addrand(id);
		addrand(UA->GroundMD.UnitType);

		if(id!=-1){
			void FindPositionForUnit(OneObject* OB);
			FindPositionForUnit(Group[id]);
			ActiveUnitAbility* newA=UA->GetNewActiveAbility();
			AddActiveUnitAbility(id,newA);
			if(TypeAbil==5){
				SeekSameAndClothe();
			}
			return;
		}
	}
}