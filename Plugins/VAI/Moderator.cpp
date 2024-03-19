
#include "stdafx.h"
#include "Moderator.h"
#include "RPG.h"

Battlefield BF;

Soldier::Soldier(){
	Clear();
};
Squad::Squad(){
	Clear();
};
void Squad::Process(){
	
	if(Life==0){
		if(Reinforce.GetAmount()>0){
			pUnit U=Reinforce[0];
			Body+=U;
			Reinforce-=U;
		}
		return;
	}
    
	Tactics();

	if( ReinforceTop!=Top ){
		iVector2D v=GameInterface::GetTZoneCoor(0,Top);
		//int n=Reinforce.GetAmount();
		//for(int i=0;i<n;i++){
		ForEachUnit(Reinforce, 
			//OneObject* OB=unit.object();
			//	if( OB->LocalOrder==NULL || OB->Attack ){
			iVector2D uv=unit.GetCoor();
			int dist=GameInterface::NORMA(v-uv);
			if(dist<200){
				Body+=unit;
				Reinforce-=unit;
			}else
				unit.SendUnitFarTo(v.x+GameInterface::random(-64,64),v.y+GameInterface::random(-64,64));
		);
	}

	if( State==0 || State==2 ){ // идем вперед к цели
		
		// если группа расползлась
		//int T=GameInterface::GetGlobalTime();
		//if(T<LastMoveTime){
		//	return;
		//}

		bool go=false;

		Body.ToBegin();
		pUnit unit;
		while(Body.Next(unit)){
            OneObject* OB=unit.object();
			if( !OB->Sdoxlo ){
				if( OB->LocalOrder==NULL || OB->Attack ){
					go=true;
					break;
				}else{
					if(GetRND(8)==0){
						iVector2D coor=unit.GetCoor();
						iVector2D dest=unit.GetGlobalDesination();
						iVector2D v=dest-coor;
						int d=GameInterface::NORMA(v);
						if(d<100){
							go=true;
							break;
						}
					}
				}
			}
		};

		short* Dang=DangerMap.GetValues();
		word* IDS=TargetMap.GetValues();

		if( go && Dang && IDS ){

			Center=Body.GetCenter();
			Top=GetTopZone(Center.x,Center.y);

			bool EnemySpell=false;

			int ntop=GameInterface::GetTZonesAmount(0);
			int T=GameInterface::GetGlobalTime();
			// обходим вражеские спелы
			Moderator* She=Shepherd.Get();
			if( She ){
				for(int i=0;i<ntop;i++){
					if( T<She->SpellDangerTime[i] ){
						Dang[i]=She->SpellDanger[i];
						EnemySpell=true;
					}
				}
			}

			int top=Top;
			int maxdang;
			word DST;

			if((IDS)&&(top&&top!=65535)&&(IDS[top]==0xFFFF)){

				int zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,100,&DST);
				if( zf<0xFFFE ){

					word* Way;
					int NWayPoint=GetLastFullWay(&Way);
					// так »» заносит на поворотах :)
					//if( NWayPoint>1 && !EnemySpell ){
					//	int i=NWayPoint-3;
					//	if(i<0)i=0;
					//	zf=Way[i];
					//}

					iVector2D targv=GameInterface::GetTZoneCoor(0,zf);

					if(SingleUnitMotion){
						ForEachUnit(Body, 
							OneObject* OB=unit.object();
							if( OB->LocalOrder==NULL || OB->Attack ){
								unit.SendUnitFarTo(targv.x,targv.y);
							}
						);
					}else{
						// лочим юниты не принадлежащие »»
						pUnitsList Unlimit;
						Body.ToBegin();
						pUnit unit;
						while(Body.Next(unit)){
							OneObject* OB=unit.object();
							if( OB->NNUM!=NI && !OB->UnlimitedMotion ){
								OB->UnlimitedMotion=true;
								Unlimit._add(unit);
							}
						}
						TargetWay.FastClear();
						//int dist=GetZonesDist(top,DST)*64;
						if( /*dist>700*/NWayPoint>3 || EnemySpell ){
							for(int i=NWayPoint-1;i>=0;i--){
								TargetWay.Add(Way[i]);
							}
							Body.SendGroupTo(targv.x,targv.y,DIR_AUTO,false);
						}else{
							TargetWay.Add(Top);
							TargetWay.Add(DST);
							targv=GameInterface::GetTZoneCoor(0,DST);
							Body.SendGroupTo(targv.x,targv.y,DIR_AUTO,true);
						}
						// –азлочиваем
						Unlimit.ToBegin();
						while(Unlimit.Next(unit)){
							OneObject* OB=unit.object();
							OB->UnlimitedMotion=false;
						}
					}

					//LastMoveTime=T+1000+GetRND(500);
					LastTop=zf;

					return;

				}

			}

		}

	}else if( State==2 ){

		// выйти из зоны действи€ вражеской магии
		Moderator* She=Shepherd.Get();
		if( She ){
			int ntop=GameInterface::GetTZonesAmount(0);
			int T=GameInterface::GetGlobalTime();
			if( T>She->SpellDangerSafeTopTime ){
				She->SpellDangerSafeTopTime=T+500;
				for(int i=0;i<ntop;i++){
					if(T<She->SpellDangerTime[i]){
						She->SpellDangerSafeTop[i]=0xFFFF;
					}else{
						She->SpellDangerSafeTop[i]=i;
					}
				}
			}

			bool go=false;

			Body.ToBegin();
			pUnit unit;
			while(Body.Next(unit)){
				OneObject* OB=unit.object();
				if( !OB->Sdoxlo ){
					if( OB->LocalOrder==NULL || OB->Attack ){
						go=true;
						break;
					}
				}
			};

			/*if(go)*/{
				DynArray<short> DM;
				DM.Add(0,ntop);
				short* Dang=DM.GetValues();
				word* IDS=She->SpellDangerSafeTop.GetValues();
				int maxdang;
				word DST;

				int zf=FindNextZoneOnTheSafeWayToObject(Top,Dang,IDS,&maxdang,100,&DST);
				if( zf<0xFFFE ){
					word* Way;
					int NWayPoint=GetLastFullWay(&Way);

					TargetWay.FastClear();
					for(int i=NWayPoint-1;i>=0;i--){
						TargetWay.Add(Way[i]);
					}
					iVector2D targv=GameInterface::GetTZoneCoor(0,zf);
					Body.SendGroupTo(targv.x,targv.y,DIR_AUTO,false);
				}
			}
		}

	}else{
		LastTop=Top;
	}
};

void Squad::SetContext(){
	
	Life=0;
	if( Body._getamount()==0 )
			return;

	Moderator* Shep=Shepherd.Get();
	SoldierMap& mSoldier=Shep->AllSoldiers;

	if( BF.CurSquad.Get()!=this ){
		pSelected Sel(GetMyNation());
		if(Sel._getamount()>0){
			RawUnit& U=Sel._getunit(0);
			if( Body._find(U)!=-1 ){
				BF.CurSquad.Set(this);
			};
		}
	}

	//int T=GameInterface::GetGlobalTime();

	Center=Body.GetCenter();
	Top=GetTopZone(Center.x,Center.y);

	pUnit u;
	ForEachUnit(Body,
		Life+=unit.GetLife();
		if( !u.Valid() && unit.Valid() ){
			u=unit;
		}
	);
	if(Life==0)
			return;

	Damage=0;
	Damage2=0;
	if(u.Valid()){
		AdvCharacter* AC=u.GetAdvCharacter();
		for(int i=0;i<NAttTypes;i++){
			if( AC->MaxDamage[i]>0 && AC->AttackRadius2[i]>0 ){
				if(Damage==0){
					Damage=AC->MaxDamage[i];
					WeaponKind=AC->WeaponKind[i];
				}else
				if(Damage2==0){
					Damage2=AC->MaxDamage[i];
					WeaponKind2=AC->WeaponKind[i];
				}
			}
		}
	}

	//int Price;
	//int PriceDrone;
	//int PriceMiner;
	
	int NZ=GameInterface::GetTZonesAmount(0);
	
	DynArray<int> DM;
	DM.Add(0,NZ);

	DynArray<int> PriceMap;
	PriceMap.Add(0,NZ);		
	//DynArray<int> PriceMinerMap;
	//PriceMinerMap.Add(0,NZ);

	// Maps
	int NMask=GameInterface::GetNationMask(NI);

	int MaxDanger=0;
	int MaxPrice=0;

	scan(mSoldier,Soldier* sEnemy,DWORD* key){
		int top=*key;
		if( top<0xFFFE /*&& sEnemy->Damage>0*/ ){

			pUnitType UT(NIndex);
			pUnitType EnemyUT(sEnemy->NIndex);

			if( !(NMask&GameInterface::GetNationMask(sEnemy->NI)) ){

				byte KillMask=UT.GetNewMonster()->KillMask;
				byte MathMask=EnemyUT.GetNewMonster()->MathMask;

				if( KillMask&MathMask ){
					AdvCharacter* AC=UT.GetAdvCharacter(NI);
					int eDam = sEnemy->Damage>0 ? max(sEnemy->Damage-AC->Protection[sEnemy->WeaponKind],1) : 0;
					//if(eDam<1) eDam=1;
					int sb=Shepherd.Get()->GetSplashBonus(sEnemy->NIndex);
					if(sb)
						eDam*=sb;
					int EnemyForce=sEnemy->Life*eDam;

					AdvCharacter* EnemyAC=EnemyUT.GetAdvCharacter(NI);
					int Dam=Damage-EnemyAC->Protection[WeaponKind];
					if(Dam<1) Dam=1;
					int Force=Life*Dam;

					// карта опастности
					if(true/*sEnemy->NI!=7*/){
						//int F=EnemyForce-Force/2; // пока нашу силу учитывать не будем
						int F=EnemyForce/Dam;
						//int n=GameInterface::GetTZoneNLinks(0,top);
						//for(int i=0;i<n;i++){
						//	TLinkInfo* LI=GameInterface::GetTZoneLink(0,top,i);
						//	DM[LI->NextAreaID]+=F;
						//}
						if(sEnemy->NI!=7){
							F/=100;
							PriceMap[top]+=sEnemy->Price;
							PriceMap[top]+=sEnemy->PriceMiner;
							if(MaxPrice<PriceMap[top])
								MaxPrice=PriceMap[top];
						}
						DM[top]+=F;
					}else{
						//DM[top]+=3;//eDam;
						int F=EnemyForce/Dam;
						DM[top]+=F;
					}
					if(MaxDanger<DM[top])
						MaxDanger=DM[top];
				}

			}

		}
	}scan_end;

	DangerMap.Fill(0);

	int d=MaxDanger>1024?MaxDanger/1024:1;

	//int* SpellDangerTime=NULL;
	Moderator* She=Shepherd.Get();
	//if( She  ){
	//	SpellDangerTime=She->SpellDangerTime.GetValues();
	//}

	int T=GameInterface::GetGlobalTime();

	for(int i=0;i<NZ;i++){
		int danger=DM[i];
		// учитываем вражеские спелы
		if( She && T<She->SpellDangerTime[i] )
			danger=She->SpellDanger[i];
		if(danger){
			danger=danger/d+3;
			DangerMap[i]=danger;
		}
		//if(danger<SHRT_MAX){
		//	if(danger>0)
		//		DangerMap[i]=danger;
		//	else
		//		DangerMap[i]=0;
		//}else
		//	DangerMap[i]=SHRT_MAX;
	}
	
	GameInterface::SmoothMap(DangerMap,1); // в будующем нужно учесть радиусы атаки юнитов

	// раскучковываем отр€ды
	int n=Shep->Squads.GetAmount();
	for(int i=0;i<n;i++){
		Squad* Sqd=Shep->Squads[i];
		if( Sqd!=this ){
			DangerMap[Sqd->Top]+=Sqd->Life/d+3;
		}
	}

	// составление карты целей
	Squadron* Chi=Deacon.Get();
	if(!Chi){
		TargetMap.Fill(0xFF);

		//bool attack=true;

		// попытка присоединитс€ к св€занному типу
		DynArray<int>* UTL=Shepherd.Get()->GetUnitTypeLinks(NIndex);
		if(UTL){
			//attack=false;
			ClonesArray<Squad>& Squads=Shepherd.Get()->Squads;
			int n=Squads.GetAmount();
			for(int i=0;i<n;i++){
				Squad* Sqd=Squads[i];
				if( Sqd && Sqd!=this && Sqd->Life ){
					bool link=false;
					for(int l=0;l<UTL->GetAmount();l++){
						if( (*UTL)[l]==Sqd->NIndex ){
							int top = Sqd->LastTop<0xFFFE ? Sqd->LastTop : Sqd->Top;
                            if(top<0xFFFE){
    							TargetMap[top]=top;
	    						//attack=true;
		    					break;
                            }
                        }
					}
				}
			}
		}else // атака базы только несв€занными типами юнитов

		// атака целей
		if(MaxPrice>0){
			int nTargets=0;
			if(Difficulty<12) Difficulty=12;
			int divisor=Difficulty;
			while( nTargets==0 && divisor<100 ){
				int min_price=MaxPrice/divisor;
				if(divisor==85){
					min_price=0;
				}
				for(int i=0;i<NZ;i++){
					int p=PriceMap[i];
					if(p>min_price){
						TargetMap[i]=i;
						nTargets++;
					}
				}
				divisor++;
			}
		}else{
			//GameInterface::LogWarning("нет целей - тупим");
		}

		// защиты базы
		for(int i=0;i<NZ;i++){
			if(Shepherd.Get()->ProtectBaseMap[i])
				TargetMap[i]=i;
		}

	}

};

bool IsWeak(pUnit& u){
	int L=u.GetLife()<<2;
	int Lmax=u.GetMaxLife();
	return L<Lmax;
}
void Squad::Tactics(){

	iVector2D v=Body.GetCenter();
	Top=GetTopZone(v.x,v.y);

	int T=GameInterface::GetGlobalTime();

	bool netral=false;
	Squadron* Chi=Deacon.Get();
	if(Chi){
		int dist=GameInterface::NORMA(Chi->tPoint-v/*Center*/);
		if(dist<500)
			netral=true;
	}

	pUnitType UT(NIndex);

	Moderator* She=Shepherd.Get();
	if( She ){
		//bool spell=false;
		itr_TopInRadius.Create(0,v.x,v.y,300);
		do{
			int top=itr_TopInRadius.Next();
			if(top!=-1){
				if( T<She->SpellDangerTime[Top] ){
					//spell=true;
					bool enemy=false;
					iVector2D r=UT.GetAttackRange();
					itr_UnitsInRadius.Create(v.x,v.y,200+r.Max);
					while(OneObject* OB=itr_UnitsInRadius.Next()){
						pUnit u(OB);
						if( u.CheckVulnerabilityFrom(NI,&UT) && (netral || !(OB->NNUM==7&&OB->AI_Guard)) && !OB->UnlimitedMotion ){
							enemy=true;
							break;
						}
					}
					if(!enemy){
						State=2;
						return;
					}
				}
				//SpellDangerTime[top]=T+UA->SpellDangerTime;
				//iVector2D t=GameInterface::GetTZoneCoor(0,top);
				//int dist=GameInterface::NORMA(x,t.x,y,t.y);
				//SpellDanger[top]=Danger-dist*D;
			}else
				break;
		}while(true);
	}

	pUnitsList Enemy;

	bool t_Weak=false;
	bool t_Attack=false;
	bool t_Repair=false;

	static DynArray<int> vLife; // сколько жизни останетс€ у врага после очередного удара всеми, кто его атакует
	vLife.FastClear();

	int vLifeAttack=0;

	itr_UnitsInRadius.Create(v.x,v.y,700);
	while(OneObject* OB=itr_UnitsInRadius.Next()){
		pUnit u(OB);
		if( u.CheckVulnerabilityFrom(NI,&UT) && (netral || !(OB->NNUM==7&&OB->AI_Guard)) && !OB->UnlimitedMotion ){
			Enemy._add(u);
			t_Weak|=IsWeak(u);
			t_Attack|=u.IsAttacking() || u.isHero();
			t_Repair|=u.IsRepairing();
			int Life=u.GetLifeAfterNextShot(); // проверка ATTLIST
			//Life<<=1;
			if(u.IsAttacking())
				vLifeAttack+=Life;
			vLife.Add(Life);
		}
	}
	itr_GetBuildingsInRadius.Create(v.x,v.y,700);
	while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
		pUnit u(OB);
		if( u.CheckVulnerabilityFrom(NI,&UT) && OB->NNUM!=7 && !OB->UnlimitedMotion ){
			Enemy._add(u);
			t_Weak|=IsWeak(u);
			t_Attack|=u.IsAttacking();
			t_Repair|=u.IsRepairing();
			int Life=u.GetLifeAfterNextShot();
			//Life<<=1;
			if(u.IsAttacking())
				vLifeAttack+=Life;
			vLife.Add(Life);
		}
	}
	if(Enemy._getamount()==0){
		Body.ToBegin();
		pUnit unit;
		while(Body.Next(unit)){
			OneObject* OB=unit.object();
			if( OB->GetLiveState()==ls_LiveBody && !OB->UnlimitedMotion ){
				if( unit.IsAttacking() ){
					pUnit e(OB->EnemyID,OB->EnemySN);
					if( e.IsAttacking() && Enemy._find(e)==-1 ){
						OneObject* eOB=e.object();
						if( eOB && eOB->GetLiveState()==ls_LiveBody ){
							iVector2D uv=unit.GetCoor();
							iVector2D ev=e.GetCoor();
							int d=GameInterface::NORMA(uv-ev);
							if(d<1200){
								Enemy._add(e);
								t_Weak|=IsWeak(e);
								t_Attack|=e.IsAttacking() || e.isHero();
								t_Repair|=e.IsRepairing();
								int Life=e.GetLifeAfterNextShot();
								//Life<<=1;
								if(e.IsAttacking())
									vLifeAttack+=Life;
								vLife.Add(Life);
							}
						}
					}
				}
			}
		}
	}

	State=0;

	int ne=Enemy._getamount();
	if(ne){
		//State=1; // воюем

		Body.ToBegin();
		pUnit unit;
		while(Body.Next(unit)){
			OneObject* OB=unit.object();
			if( OB->GetLiveState()==ls_LiveBody && OB->NNUM==NI ){
				
				OB->NoSearchVictim=true;

				if( unit.IsAttacking() ){
					State=1;
					pUnit enemy=unit.GetCurrentVictim();
					if( enemy.object() ){
						bool accept=false;
						if(t_Repair){
							if(enemy.IsRepairing()){
								accept=true;
							}
						}else
						if(t_Weak){
							iVector2D r=unit.GetAttackRange(0);
							if( IsWeak(enemy) /*&& r.Max>d+150*/ ){
								accept=true;
							}
						}else
						if(t_Attack){
							if( enemy.IsAttacking() || enemy.isHero() ){
								accept=true;
							}
						}else{
							accept=true;
						}
						if(accept){
							//не искать новую цель если уже при делах
							continue;
						}
					}
				}
				if( true/*!unit.IsBusy()*/ ){
					byte kmask=OB->newMons->KillMask;
					iVector2D uv=unit.GetCoor();
					Enemy.ToBegin();
					int e=0,eid=-1,edam=0,ewk=0;
					int dist=INT_MAX;
					pUnit victim;
					pUnit victim2;
					pUnit enemy;
					while(Enemy.Next(enemy)){
						OneObject* EOB=enemy.object();
						int vL=vLife[e];
						if( (kmask&EOB->MathMask) && !(OB->NMask&EOB->NMask) ){

							if( vL>0 || !victim2.Valid() ){

								iVector2D r=unit.GetAttackRange(0);

								pUnitType EnemyUT(enemy);
								//AdvCharacter* EnemyAC=EnemyUT.GetAdvCharacter(NI);
								AdvCharacter* EnemyAC=enemy.GetAdvCharacter();
								int wk=0;
								int Dam=Damage-EnemyAC->Protection[WeaponKind];
								if(Dam<1) Dam=1;
								if( !(OB->newMons->AttackMask[0]&EOB->MathMask) ) 
									Dam=0;
								int Dam2=Damage2-EnemyAC->Protection[WeaponKind2];
								if(Dam2<1) Dam2=1;
								if( !(OB->newMons->AttackMask[1]&EOB->MathMask) ) 
									Dam2=0;
								if(Dam<Dam2){
									wk=1;
									Dam=Dam2;
								}

								if(Dam){
									victim2=enemy;
									if(vL>0){
										int eLife=enemy.GetLife();
										if( Dam>eLife && eLife>0 ) 
											Dam=eLife;

										int d=GameInterface::NORMA(uv-enemy.GetCoor());
										d/=Dam;

										// усиленный удар по здани€м
										bool destroyer=false;
										if( enemy.IsBuilding() && OB->newMons->DamageOnBuildingMultiplicator ){
											destroyer=true;
											d/=2;//OB->newMons->DamageOnBuildingMultiplicator;
										}

										// атака сплешевого юнита
										int sb=Shepherd.Get()->GetSplashBonus(EOB->NIndex);
										if( sb ){
											if(r.Max>200){
												d/=3;
											}else{
												d*=3;
											}
										}

										// герой
										if( enemy.isHero() ){
											d/=15;
										}

										if( !victim.Valid() || dist>d ){
											bool accept=false;
											if(t_Repair){
												if(enemy.IsRepairing()){
													accept=true;
												}
											}else
											if( t_Weak && (r.Max-150)/Dam>d ){
												if( IsWeak(enemy) ){
													accept=true;
												}
											}else
											if( enemy.IsTakingResources() && r.Max<200 && Dam>1  ){
												accept=true;
											}else
											if( t_Attack && vLifeAttack>0 ){
												if( enemy.IsAttacking() /*|| enemy.&&!t_Repair*/ || enemy.isHero() ){
													accept=true;
												}
											}else{
												accept=true;
											}
											if( accept || destroyer ){
												victim=enemy;
												dist=d;
												eid=e;
												edam=Dam;
												ewk=wk;
											}
										}
									}
								}
							}
						}
						e++;
					};
					if(!victim.Valid()){
						victim=victim2;
						if(t_Repair){
							t_Repair=false;
						}else
						if(t_Weak){
							t_Weak=false;
						}else
						if(t_Attack){
							t_Attack=false;
						}
					}
					if(victim.Valid()){
						if(ewk==0){
							OB->RifleAttack=false;
							OB->ArmAttack=true;
						}else{
							OB->RifleAttack=true;
							OB->ArmAttack=false;
						}
						if( unit.AttackUnit(victim,1) ){
							if(edam>0){
								vLife[eid]-=edam;
								if(victim.IsAttacking()){
									vLifeAttack-=edam;
								}
							}
							State=1;
						}else{
							OB->NoSearchVictim=false;
							//GameInterface::LogWarning("не смогли атаковать - тупим");
						}
					}else{
						OB->NoSearchVictim=false;
						//GameInterface::LogWarning("нет цели - тупим");
					}
					if( !(OB->NoSearchVictim||OB->Attack) ){
						OB->ClearOrders();
					}
				}
			}
		};

	}else{
		//State=0;
	}

};

void Moderator::Clear(){
	gInit=false;
	stDangTime=GetRND(2000);
	for(int i=0;i<8;i++){
		if(stDang[i]){
			free(stDang[i]);
			stDang[i]=NULL;
		}
	}
	Squads.Clear();
	
	ProtectBaseMap.NValues=0;//Fill(false);
	SpellDangerTime.NValues=0;
	SpellDanger.NValues=0;
	SpellDangerSafeTop.NValues=0;

	//SplashBonus.Clear();
	UnitTypeLinks.Clear();

	DistFromBase.NValues=0;
	BaseCenter.x=0;
	BaseTopCenter=0xFFFF;

};
void  Moderator::InitProcess(){
	int ntop=GameInterface::GetTZonesAmount(0);
	if(ProtectBaseMap.GetAmount()<ntop){
		//ProtectBaseMap.Add(false,ntop);
		//SpellDangerTime.Add(0,ntop);
		//SpellDanger.Add(0,ntop);
		//SpellDangerSafeTop.Add(0xFFFF,ntop);
		//ArmiesMap.Add(0xFFFF,ntop);
		//ArmiesFrontMap.Add(0xFFFF,ntop);
		ProtectBaseMap.Check(ntop);
		ProtectBaseMap.NValues=ntop;
		SpellDangerTime.Check(ntop);
		SpellDangerTime.NValues=ntop;
		SpellDanger.Check(ntop);
		SpellDanger.NValues=ntop;
		SpellDangerSafeTop.Check(ntop);
		SpellDangerSafeTop.NValues=ntop;
		ArmiesMap.Check(ntop);
		ArmiesMap.NValues=ntop;
		ArmiesFrontMap.Check(ntop);
		ArmiesFrontMap.NValues=ntop;
	}
	ProtectBaseMap.Fill(false);
	SpellDangerTime.Fill(0);
	SpellDanger.Fill(0);
	SpellDangerSafeTop.Fill(0xFFFF);
	ArmiesMap.Fill(0xFFFF);
	ArmiesFrontMap.Fill(0xFFFF);
};
void Moderator::Process(){
	
	if( Squads.GetAmount()==0 )
		return;

	int T=GameInterface::GetGlobalTime();

	if(stDangTime<T){
		stDangTime=T+800+GetRND(1000);
		ProcessResearch();
	}

	int NStorms=Squads.GetAmount();

	for(int i=0;i<NStorms;i++){

		Squad* Grp=Squads[i];
		if(!Grp) 
			continue;

		if(!ProcessGameObj(Grp)){
			if( Grp->TimeLastSetContext<T ){
				Grp->Shepherd.Set(this);
				Grp->TimeLastSetContext=T+1500+GetRND(1500);
				Grp->SetContext();
			}
			if( Grp->LastMoveTime<T ){
				Grp->Shepherd.Set(this);
				Grp->LastMoveTime=T+800+GetRND(600);
				Grp->Process();
			}
			if( Grp->TimeLastReinforce<T ){
				Grp->TimeLastReinforce=T+2000+GetRND(2000);
				ProcessReinforce(Grp);
			}
		}
	}

}
void Moderator::ProcessResearch(){
	//int nat=GetRND(18);
	//int it=maiGetDangIteration();
	//if(nat<8){
	//	if(stDang[NI]){
	//		CreateDangerMapForStorm(nat,stDang[nat],GetNZones(),gFear,it);
	//	}
	//}

	int ntop=GameInterface::GetTZonesAmount(0);

	// карта опонентов
	static DynArray<bool> OpponentMap;		
	if( OpponentMap.GetAmount()<ntop ){
		OpponentMap.Check(ntop);
		OpponentMap.NValues=ntop;
	}
	OpponentMap.Fill(false);

	int NMask=GameInterface::GetNationMask(NI);

	AllSoldiers.del_all();
	itr_GetAllUnits.Create();
	while(OneObject* OB=itr_GetAllUnits.Next()){
		pUnit u(OB);
		if( OB->GetLiveState()==ls_LiveBody ){ // рассматриваем только тех кого можем атаковать

			iVector2D v=u.GetCoor();
			//int top=GameInterface::GetTZoneIndex(0,v.x,v.y);
			int top=GetTopZone(v.x,v.y);
			if(top>=0xFFFE){
				//GameInterface::LogWarning("Top: %d - out of valid",top);
				continue;
			}

			if( u.CheckVulnerabilityFrom(NI) /*&& !(OB->NNUM==7*//*&&(OB->AI_Guard||OB->NewBuilding)*//*)*/ ){

				AdvCharacter* AC=u.GetAdvCharacter();

				if( !( (NMask&OB->NMask) || OB->AI_Guard || AC->MaxDam==0 || OB->NNUM==7/*OB->NewBuilding&&OB->NNUM==7*/ ) ){
					OpponentMap[top]=true;
				}

				Soldier* sFind=NULL;
				scan_key(AllSoldiers,top,Soldier* Sol){
					if( Sol->NI==OB->NNUM && Sol->NIndex==OB->NIndex ){
						sFind=Sol;
						break;
					}
				}scan_end;
				Soldier sNew;
				if(!sFind){
					sFind=&sNew;
					sFind->NI=OB->NNUM;
					sFind->NIndex=OB->NIndex;
					sFind->Damage=0;
					sFind->Damage2=0;
					if( OB->Ready && !(OB->newMons->Peasant||u.isHero()) ){
						for(int i=0;i<NAttTypes;i++){
							if( sFind->Damage<AC->MaxDamage[i] && AC->AttackRadius2[i]>0 ){
								sFind->Damage=AC->MaxDamage[i];
								sFind->WeaponKind=AC->WeaponKind[i];
							}
						}
						// боимс€ мину, мина может больно ударить
						if(sFind->Damage==0){
							sFind->Damage=10000;
							sFind->WeaponKind=0;
						}
					}
				}

				//
				//sFind->Body._add(u);
				//

				sFind->Life+=OB->Life;

				// ѕриоритеты по цел€м модератора
				// 1. ƒорогие юниты
				// 2. –абочие с приказом чинить (“актический)
				// 3. ёниты добывающие ресурсы: шахты, фермы, рабочие с приказом добычи
				// 4. –абочие с приказом строить (“актический)
				// 5. Ћюбой рабочий
				// “актический приоритет используетс€ только во врем€ ближнего бо€

				pUnitType ut(u);
				int Cost[8];
				ut.GetCost(sFind->NI,Cost);
				int price=0;
				for(int i=0;i<8;i++){
					price+=Cost[i];
				}
				sFind->Price+=price;

				NewMonster* NM=OB->newMons;
				// NM->ResConcentrator - склад
				if(NM->Producer/*&&NM->ResSubst*/){
					if(NM->ProdType){
						int NINS=OB->NInside;
						int work=(NM->FreeAdd+NINS*NM->PeasantAdd)/**GameSpeed/8*/;
						sFind->PriceMiner+=price*work*100;
					}
				}else
					if(NM->ResConsumer<0){
						sFind->PriceMiner+=price*(-NM->ResConsumer/3);
					}else
						if(NM->Peasant){
							if(!OB->Hidden){
								sFind->PriceDrone+=price*100;
								sFind->PriceMiner+=price*100; // временно
								//Order1* Ord=OB->LocalOrder;
								//while(Ord){
								//	if(Ord->DoLink==&TakeResLink){
								//		sFind->PriceMiner+=price;
								//		break;
								//	}
								//	Ord=Ord->NextOrder;
								//}
							}
						}
						AllSoldiers.add_once(top,*sFind);
			}else{
				if( u.IsBuilding() && OB->NNUM==NI ){
					ProtectBaseMap[top]=true;
					if(BaseCenter.x=0){
						BaseCenter=v;
					}
					BaseTopCenter=top;
				}
			}
		}
	}

	// расшир€ем зону защиты
	static DynArray<bool> PBM;
	if( PBM.GetAmount()<ntop ){
		PBM.Check(ntop);
		PBM.NValues=ntop;
	}
	for(int itr=0;itr<3;itr++){
		for(int top=0;top<ntop;top++){
			PBM[top]=ProtectBaseMap[top];
		}
		for(top=0;top<ntop;top++){
			if(PBM[top]){
				int n=GameInterface::GetTZoneNLinks(0,top);
				for(int i=0;i<n;i++){
					TLinkInfo* LI=GameInterface::GetTZoneLink(0,top,i);
					ProtectBaseMap[LI->NextAreaID]=true;
				}
			}
		}
	}

	// пересекаем карты защиты и оппонентов
	for(int top=0;top<ntop;top++){
		ProtectBaseMap[top]&=OpponentMap[top];
	}

	int n=Squadrons.GetAmount();
	for(int i=0;i<n;i++){
		Squadrons[i]->Process();
	}

}
bool Moderator::ProcessGameObj(Squad* Grp){
	GAMEOBJ* Group=&Grp->Group;
	GAMEOBJ* StrGrp=&Grp->StrGrp;
	int top=0xFFFF;
	DeleteHiddenUnits(StrGrp);

	LimitUnitsMotion(Group);

	int N=CleanGroup(Group)+CleanGroup(StrGrp);
	if(N){
		{
			int T=GameInterface::GetGlobalTime();

			// подготовка к штурму
			DWORD prp=GetCurrentPurpose(Group);
			int t=T-Grp->LastMoveTime;
			if(prp==0&&t>120||t>5156){

				//addrand(212);
				Grp->LastMoveTime=T;

				RemoveGroup(StrGrp,Group);
				maiSetGroupID(Group->Index,1);

				int xc=-1,yc;					
				GetGrpCenter(Group,&xc,&yc);

				if(xc>=0){
					int top=GetTopZone(xc,yc);
					Grp->Top=top;
					if(top>=0xFFFE){
						OneUnit UN;
						GetUnitInfo(Group,0,&UN);
						xc=UN.x;
						yc=UN.y;
						top=GetTopZone(xc,yc);
					}
					if(top>=0&&top<GetNZones()){

						byte Owner=Grp->Owner;

						bool Retreat=false;
						int MoveArea=0xFFFF;
						//bool atHero=GiveHeroOrder(Owner,Retreat,MoveArea);

						if(false){

							int dx,dy;
							if(GetTopZRealCoor(MoveArea,&dx,&dy)){

								int r=GetZonesDist(top,MoveArea)*64;

								bool Attack=false;
								if( !Retreat && r<700 ){
									GAMEOBJ Z;
									Z.Index=xc;
									Z.Serial=yc;
									Z.Type='@   '-'   '+400;
									Attack=AttackEnemyInZone2(&Grp->Group,&Z,Owner);
									if(Attack) 
										return true;
								}

								if( r>200 ){
									if(Retreat)
										SGP_MoveToPoint(Owner,Group,dx,dy,512,0,0,32); //32
									else
										SGP_MoveToPoint(Owner,Group,dx,dy,512,0,0,32); //32+1
								}

							}
							return true;

						}

						bool Attack=false;

						GAMEOBJ Z;
						Z.Index=xc;
						Z.Serial=yc;
						Z.Type='@   '-'   '+700;
						Attack=AttackEnemyInZone2(&Grp->Group,&Z,Owner);
						if(Attack) 
							return true;

						word BID=MakeOneShotToBuild(&Z,&Grp->Group,Owner);
						if(BID<0xFFFE){
							return true;
						}

						DWORD prp=GetCurrentPurpose(&Grp->Group);
						if(prp!=0)
							return true;

						short* Dang;
						word IDS[4096];
						int N=GetNZones();
						if(!stDang[Owner]){
							stDang[Owner]=(short*)malloc(GetNZones()<<1);
						}
						Dang=stDang[Owner];

						int DM[2048];
						memset(&DM,0,sizeof DM);
						for(int d=0;d<2048&&d<N;d++) DM[d]=Dang[d];
						SetDangerMap(DM);
						maiSetDangerMap(DM);

						bool find=false;
						int Force=2000;

						word DST=0xFFFF;
						int zf=0xFFFF;

						word StrmNIndex=0xFFFF;
						int n=GetNUnits(Group);
						//addrand(n);
						for(int i=0;i<n;i++){
							OneUnit OU;
							if(GetUnitInfo(Group,i,&OU)){
								StrmNIndex=OU.NIndex;
								//addrand(StrmNIndex);
								break;
							}
						}

						int maxdang;

						int NMin=n>>(5-Grp->Difficulty);
						SetMinEnemy(NMin);
						CreateTopListEnArmy(IDS,Owner,NMin);
						maiSetTargets(IDS);

						zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,100,&DST);
						//addrand(zf);

						maiSetWayArray();

						if(zf<0xFFFE){
							int tx=-1;
							int ty=-1;

							OneUnit OU;
							OU.Index=0xFFFF;
							if(DST!=0xFFFF&&GetUnitGlobalInfo(DST,&OU)){								
								tx=OU.x;
								ty=OU.y;
							}

							//addrand(DST);
							//addrand(tx);
							//addrand(ty);

							if(Grp->SingleUnitMotion){
								UnitsGroup* UG=GetUnitsGroup(Group);
								for(int i=0;i<UG->N;i++){
									OneObject* OB=GetOBJ(UG->IDS[i]);
									if(OB&&!OB->Sdoxlo&&OB->Serial==UG->SNS[i]){
										OBJ_SendTo(OB->Index,tx,ty,16+1,0);
									}
								}

								//addrand(1256);
								return true;
							}

							int r=GetTopDist(xc,yc,tx,ty);

							//addrand(r);
							if(r<25){
								int n=GetNUnits(Group);
								for(int i=0;i<n;i++){
									OneUnit OU;
									if(GetUnitInfo(Group,i,&OU)){
										int rr=GetTopDist(OU.x,OU.y,tx,ty);
										if(rr<r){
											r=rr;
											//addrand(r);
										}											
									}
								}
							}

							if(!find){
								if(zf!=top&&r>3){
									int x,y;

									GetTopZRealCoor(zf,&x,&y);
									//addrand(x);
									//addrand(y);

									//addrand(Attack);

									if(!Attack){
										DWORD prp=GetCurrentPurpose(Group);
										if(prp==0){
											//addrand(x);
											SGP_MoveToPoint(Owner,Group,x,y,GetDirEx(x-xc,y-yc),0,0,32+1);
										}
									}
								}
							}

						}			
						//addrand(top);
						Grp->LastTop=top;
					};
				};
			};
		};
		return true;
	}
	return false;
}

void Moderator::ProcessReinforce(Squad* SQ){

	TypeStats* TS=GetStats(SQ->NIndex);
	int NU=SQ->Body.GetAmount()+SQ->Reinforce.GetAmount();
	if( NU>0 && NU<TS->SquadStrengthMax/3 ){

		DynArray<int> Index;
		DynArray<int> Amount;
		DynArray<int> Dist;

		//bool find=false;
		int n=Squads.GetAmount();
		for(int i=0;i<n;i++){
			Squad* Sqd=Squads[i];
			if( Sqd->Life>0 && Sqd!=SQ ){
				int a=TS->SquadStrengthMax-(Sqd->Body.GetAmount()+Sqd->Reinforce.GetAmount());
				if( a>0 ){
					//GameInterface::GetTZoneIndex()
					//IDS[Sqd->Top]=i;
					//find=true;
					int d=GameInterface::NORMA(0,Top,Sqd->Top)/a;
					if( d<3600 ){
						int nid=Index.GetAmount();
						for(int id=0;id<nid;id++){
							if( Dist[id]>d ){
								break;
							}
						}
						Index.Insert(id,i);
						Amount.Insert(id,a);
						Dist.Insert(id,d);
					}
				}
			}
		}
		n=Index.GetAmount();
		if(n>0/*find*/){

			SQ->Body+=SQ->Reinforce;
			SQ->Reinforce.Clear();
			
			//DynArray<short> DM;
			//DM.Add(0,ntop);
			//short* Dang=DM.GetValues();

			//int maxdang;
			//word DST;

			//while( int NU=SQ->Body.GetAmount() ){
			//	int zf=FindNextZoneOnTheSafeWayToObject(SQ->Top,Dang,IDS,&maxdang,100,&DST);
			//	if( zf<0xFFFE ){
			//		Squad* Sqd=Squads[DST];
			//		int nu=TS->SquadStrengthMax-(Sqd->Body.GetAmount()+Sqd->Reinforce.GetAmount());
					ForEachUnit(SQ->Body, 
						for(int i=0;i<n;i++){
							if( Amount[i]>0 ){
								break;
							}
						}
						if(i==n)
							i=0;
						Amount[i]--;
						int id=Index[i];
						Squads[id]->Reinforce+=unit;
					);
			//	}
			//}

			SQ->Body.Clear();
			SQ->Life=0;

		}
	}
	
};

bool Moderator::AddGrp(GAMEOBJ* Group){

	SetAI_Guard(Group,false);

	UnitsGroup* UG=GetUnitsGroup(Group);
	if(UG){

		int NIndex=0xFFFF;

		// RPG unit
		int N=0;//UG->N;
		for(int i=0;i<UG->N;i++){
			pUnit unit(UG->IDS[i],UG->SNS[i]);
			if( unit.isHero() ){
				RPG.AddUnit(unit);
				//N--;
			}else if( unit.Valid() && !unit.object()->UnlimitedMotion ){
				if(NIndex==0xFFFF){
					pUnitType UT(unit);
					NIndex=UT.GetType();
				}
				N++;
			}
		}
		if(N<=0)
			return false;

		TypeStats* TL=GetStats(NIndex);

		do{
			Squad* Grp=NULL;
			int n=Squads.GetAmount();
			for(int i=0;i<n;i++){
				Squad* S=Squads[i];
				if(S->Life==0){
					Grp=S;
					break;
				}
			}
			if(Grp){
				Grp->Clear();
			}else{
				Grp=new Squad;
				if(Squads.GetAmount()<2){
					Grp->Difficulty=77;
				}else{
					Grp->Difficulty=12+GetRND(70);
				}
				Squads.Add(Grp);
				Grp->Shepherd.Set(this);
			}
			int a=TL->SquadStrengthMax;
			for(i=0;i<UG->N;i++){
				pUnit unit(UG->IDS[i],UG->SNS[i]);
				if( unit.Valid() && !unit.object()->UnlimitedMotion ){
					Grp->Add(unit);
					UG->IDS[i]=0xFFFF;
					N--;
					a--;
					if(a<=0)
						break;
				}
			}
		}while(N>0);
		
		//EraseGroup(Group);
		//UG->N=0;
		CleanGroup(Group);

		return true;
	}
	
	//Grp->Owner=Owner;
	//SetGroupNMASK(&Grp->Group,GetNatNMASK(Owner));

	//RegisterDynGroup(&Grp->Group);
	//CopyUnits(Src,&Grp->Group,0,10000,0,1);
	//RegisterDynGroup(&Grp->StrGrp);
	//Grp->StormID=0xFFFF;

	//Grp->Top=0xFFFF;
	//Grp->LastTop=0xFFFF;

	//Grp->LastMoveTime=0;

	//Grp->CannonGuard=false;

	//Grp->Difficulty=2;

	//M.Squads.Add(Grp);
	return false;
};

bool Moderator::CheckCleanArea(iVector2D& A){
	
	if(Squads.GetAmount()==0)
		return true;

	// if near base
	itr_GetBuildingsInRadius.Create(A.x,A.y,1000);
	while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
		if(OB->NNUM==NI)
			return true;
	}

	pUnitsList eDef;

	bool find_enemy=false;
	itr_UnitsInRadiusFromOtherNations.Create(NI,A.x,A.y,500);
	while(OneObject* OB=itr_UnitsInRadiusFromOtherNations.Next()){
		if( !OB->Sdoxlo && OB->NNUM==7 ){
			find_enemy=true;
			eDef._add(pUnit(OB));
		}
	}
	if(find_enemy){
		Squadron* Sqd=NULL;
		Squadron*EmptySqd=NULL;
		int n=Squadrons.GetAmount();
		for(int i=0;i<n;i++){
			Squadron* S=Squadrons[i];
			if( S->Task==0 ){
				int norm=GameInterface::NORMA(A-S->tPoint);
				if(norm<900){
					Sqd=S;
					break;
				}
			}
			if( S->Task==-1 && !EmptySqd ){
				EmptySqd=S;
			}
		}
		if(!Sqd){
			if(EmptySqd){
				Sqd=EmptySqd;
			}else{
				Sqd=new Squadron;
                Squadrons.Add(Sqd);
			}
			Sqd->Commander.Set(this);
			Sqd->Task=0;
			Sqd->tPoint=eDef.GetCenter();
		}
		return false;
	}
	return true;
};
int Moderator::GetDistFromBase(int top){
    if(top>=0 && top<0xFFFE){
	    int NZ=GameInterface::GetTZonesAmount(0);
	    if(DistFromBase.GetAmount()<NZ){
		    DistFromBase.Check(NZ);
		    DistFromBase.NValues=NZ;
		    DistFromBase.Fill(0);
	    }
	    if(BaseTopCenter==0xFFFF){
		    BaseTopCenter=top;
	    }
	    int dist=DistFromBase[top];
	    if( dist==0 && BaseTopCenter!=top ){
		    //int topCenter=GetTopZone(BaseCenter.x,BaseCenter.y);
		    dist=GetZonesDist2(top,BaseTopCenter,0)<<6;
		    DistFromBase[top]=dist;
	    }
	    return dist;
    }else return 10000000;
};
TypeStats* Moderator::GetStats(int Type){
	TypeStats* TS=NULL;
	int n=Stats.GetAmount();
	for(int i=0;i<n;i++){
		TypeStats* ts=Stats[i];
		if( ts->NIndex==Type ){
			TS=ts;
			break;
		}
	}
	if(!TS){
		TS=new TypeStats;
		TS->NIndex=Type;
		TS->SquadStrengthMax=36;
	}
	return TS;
};
void Squadron::Process(){
		
	return;

	if( Task==0 ){

		// создаем отр€д целей
		tUnits.Clear();
		itr_UnitsInRadius.Create(tPoint.x,tPoint.y,600);
		while(OneObject* OB=itr_UnitsInRadius.Next()){
			pUnit u(OB);
			if( u.CheckVulnerabilityFrom(NI) && OB->NNUM==7 ){
				tUnits.Add(u);
			}
		}

		if( tUnits.Body._getamount()==0 ){
			Task=-1;
			return;
		}

		// собираем отр€ды и оцениваем шансы на победу
		int Force=0;
		int n=Staff.GetAmount();
		for(int i=0;i<n;i++){
			Squad* Sq=Staff[i]->Get();
			if(Sq){
				Force+=Sq->GetForcePercent(tUnits);
			}
		}
		if(Force<120){
			// мало каши, нужна подмога
			Moderator* Com=Commander.Get();
			if(Com){
				n=Com->Squads.GetAmount();
				for(i=0;i<n;i++){
					Squad* Sq=Com->Squads[i];
					Squadron* Chi=Sq->Deacon.Get();
					if(!Chi){
						int norm=GameInterface::NORMA(tPoint-Sq->Center);
						if(norm<4000){
							Sq->Deacon.Set(this);
							SquadRef* SR=new SquadRef;
							SR->Set(Sq);
							Staff.Add(SR);
							Force+=Sq->GetForcePercent(tUnits);
							if(Force>120)
								break;
						}
					}
				}
			}
		}

		// заполн€ем карту целей
		n=Staff.GetAmount();
		for(i=0;i<n;i++){
			Squad* Sq=Staff[i]->Get();
			if(Sq){
				Sq->TargetMap.Fill(0xFFFF);
				int top=GetTopZone(tPoint.x,tPoint.y);
				Sq->TargetMap[top]=top;
			}else{
				Staff.DelElement(i);
				i--;
				n--;
			}
		}

		// чтобы победить необходимо напасть всеми отр€дами одновременно

	}
};

void Soldier::Clear(){
	NI=0;
	NIndex=0xFFFF;
	Life=0;
	Damage=0;
	Price=0;
	PriceDrone=0;
	PriceMiner=0;
	TimeLastSetContext=0;
};
void Soldier::Add(pUnit& u){
	OneObject* OB=u.object();
	if(OB->Life){
		if(Life==0){
			NI=OB->NNUM;
			NIndex=OB->NIndex;
			Damage=0;
			Damage2=0;
			AdvCharacter* AC=u.GetAdvCharacter();
			for(int i=0;i<NAttTypes;i++){
				if( AC->MaxDamage[i]>0 && AC->AttackRadius2[i]>0 ){
					if(Damage==0){
						Damage=AC->MaxDamage[i];
						WeaponKind=AC->WeaponKind[i];
					}else
					if(Damage2==0){
						Damage2=AC->MaxDamage[i];
						WeaponKind2=AC->WeaponKind[i];
					}else
						break;
				}
			}
		}
		Life+=OB->Life;
	}
};

void Squad::Clear(){
	Soldier::Clear();
	Body.Clear();
	int NZ=GameInterface::GetTZonesAmount(0);
	if(DangerMap.GetAmount()<NZ){
		DangerMap.Check(NZ);
		DangerMap.NValues=NZ;
	}
	if(TargetMap.GetAmount()<NZ){
		TargetMap.Check(NZ);
		TargetMap.NValues=NZ;
	}
	Deacon.Set(NULL);
	LastTop=0xFFFF;
	LastMoveTime=0;
	TimeLastReinforce=0;
};
void Squad::Add(pUnit& u){
	Soldier::Add(u);
	Body._add(u);
};
void Squad::Refresh(){
};
void Soldier::GetForce(Soldier& Enemy, int& Force, int& eForce ){
	pUnitType UT(NIndex);
	AdvCharacter* AC=UT.GetAdvCharacter(NI);

	pUnitType eUT(Enemy.NIndex);
	AdvCharacter* eAC=eUT.GetAdvCharacter(Enemy.NI);

	int eDam=Enemy.Damage-AC->Protection[Enemy.WeaponKind];
	if(eDam<1) eDam=1;
	eForce=Enemy.Life*eDam;

	int Dam=Damage-eAC->Protection[WeaponKind];
	if(Dam<1) Dam=1;
	Force=Life*Dam;
};
int Soldier::GetForceRelative(Soldier& Enemy){
	int Force,eForce;
	GetForce(Enemy,Force,eForce);
	return Force-eForce;
};
int Soldier::GetForcePercent(Soldier& Enemy){
	int Force,eForce=0;
	GetForce(Enemy,Force,eForce);
	if(eForce==0){
		return INT_MAX;
	}
	return Force*100/eForce;
};

int Moderator::GetSplashBonus(word NIndex){
	int n=BF.SplashBonus.GetAmount();
	if(NIndex<n){
		return BF.SplashBonus[NIndex];
	}
	return 0;
};
void Moderator::SetSplashBonus(word NIndex, int Bonus){
	int n=BF.SplashBonus.GetAmount();
	if(NIndex>=n){
		BF.SplashBonus.Check(NIndex);
		BF.SplashBonus.NValues=NIndex+1;
	}
	BF.SplashBonus[NIndex]=Bonus;
};

//void Moderator::ClearUnitTypeLinks(){
//};

void Moderator::SetUnitTypeLinks(word NIndex, LinearArray<int,_int>* Links){
	TypeLinks* TL=NULL;
	int n=UnitTypeLinks.GetAmount();
	for(int i=0;i<n;i++){
		TypeLinks* tl=UnitTypeLinks[i];
		if( tl && tl->Type==NIndex ){
			TL=tl;
			break;
		}
	}
	if(!TL){
		TL=new TypeLinks;
		TL->Type=NIndex;
		UnitTypeLinks.Add(TL);
	}
	TL->Links.Clear();
	int nl=Links->GetAmount();
	for(int l=0;l<nl;l++){
		TL->Links.Add((*Links)[l]);
	}
};

void Moderator::OnCastSpell(UnitAbility* UA, int x, int y, int r){
	if( UA->SpellDangerTime>0 ){
		int Danger=UA->SpellDanger==0?2048:UA->SpellDanger;
		int D=Danger/2/(r+UA->SpellAddRadius);
		int T=GameInterface::GetGlobalTime();
		itr_TopInRadius.Create(0,x,y,r+UA->SpellAddRadius);
		do{
			int top=itr_TopInRadius.Next();
			if(top!=-1){
				//if(T>SpellDangerTime[top]){
				//}
				SpellDangerTime[top]=T+UA->SpellDangerTime;
				iVector2D t=GameInterface::GetTZoneCoor(0,top);
				int dist=GameInterface::NORMA(x,t.x,y,t.y);
				SpellDanger[top]=Danger-dist*D;
			}else
				break;
		}while(true);
	}
};
DynArray<int>* Moderator::GetUnitTypeLinks(word NIndex){
	int n=UnitTypeLinks.GetAmount();
	for(int i=0;i<n;i++){
		TypeLinks* tl=UnitTypeLinks[i];
		if( tl && tl->Type==NIndex ){
			return &tl->Links;
		}
	}
	return NULL;
};
