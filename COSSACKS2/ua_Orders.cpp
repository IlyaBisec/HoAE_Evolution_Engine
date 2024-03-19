#include "stdheader.h"
#include ".\cvi_MainMenu.h"
#include "ua_Ability.h"
#include "ua_Orders.h"
#include "ua_Item.h"
#include "PluginsInterface.h"

bool EnterMagazine(OneObject* Hero, OneObject* Lib);
DLLEXPORT bool GetBuildingEntryPoint(int* x,int* y,word Index,int AddDistance);
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
void EraseObject(OneObject* OB);
void RotUnit(OneObject* OB,char Dir,byte OrdType, byte PrioryLevel);
word GetDir(int dx,int dy);
void EventReceiveItem(OneObject* Acceptor, OneObject* ItemOB, ActiveUnitAbility* AA, char* Msg=NULL);

// PickUpChest
ActiveUnitAbility* GetPickUpAbility(OneObject* OB){
	uaa_Chest* Ch=DetectChest(OB);
	if( Ch && !Ch->Empty ){
		itr_AA.Create(OB);
		while(ActiveUnitAbility* aA=itr_AA.Next()){
			if( aA!=Ch && aA->TypeAbil!=0 ){
				uaa_Chest* aCh=dynamic_cast<uaa_Chest*>(aA);
				OneMagazine* aMg=dynamic_cast<OneMagazine*>(aA);
				OneBonus* aBn=dynamic_cast<OneBonus*>(aA);
				UnitAbility* uA=aA->GetA();
				if( uA && !(uA->System||aCh||aMg||aBn || uA->Aura&&aA->TypeAbil!=4&&aA->TypeAbil!=5 ) ){
					return aA;
				}
			}
		}
	}
	return NULL;
}
void StopMotion(OneObject* OB){
	if(OB){
		OB->DeletePath();
		OB->DestX=-1;
		OB->InMotion=false;
		if( OB->LocalOrder )
			OB->LocalOrder->OrderType=12;
	}
}
// StartDialog
void StartDialogLink(OneObject* OB){
	Order1* OR=OB->LocalOrder;
	OB->PrioryLevel=OR->PrioryLevel;

	word ID=OR->info.MoveToObj.ObjIndex;
	word SN=OR->info.MoveToObj.SN;
	OneObject* DialogOB=Group[ID];

	addrand(ID);

	if (/*ID<MAXOBJECT && */DialogOB && DialogOB->Serial==SN){
		//int x,y;
		//if(!GetBuildingEntryPoint(&x,&y,ID,100)){
		//	x=DialogOB->RealX/16;
		//	y=DialogOB->RealY/16;
		//}
		int cR,cH,uH;
		OB->GetCylinder(cR,cH,uH);
		//int MaxNorm=64+cR;
		int MaxNorm=DialogOB->NewBuilding?cR+64:cR;
		DialogOB->GetCylinder(cR,cH,uH);
		//if(DialogOB->NewBuilding){
		//	MaxNorm+=32;
		//}
		MaxNorm+=cR+DialogOB->newMons->AddShotRadius;
		if( !DialogOB->newMons->NoCollision ){
			int CollisionR=OB->newMons->CollisionRadius+DialogOB->newMons->CollisionRadius;
			if(MaxNorm<CollisionR)
				MaxNorm=CollisionR;
		}
		int norm=Norma(OB->RealX-DialogOB->RealX,OB->RealY-DialogOB->RealY)/16;
		if(norm<MaxNorm){

			//TryToStand(OB,0);

			NewAnimation* NA=OB->newMons->GetAnimation(anm_PickUp);

			if( OR->info.MoveToObj.wep==0 ){
				// Magazine
				if(!EnterMagazine(OB,DialogOB)){
					return;
				}

				addrand(norm);

				StopMotion(OB);

				// запустить анимацию поднятия вещи
				OR->info.MoveToObj.wep=1;
				if( NA && NA->Enabled && OB->NewAnm!=NA ){
					OB->NewAnm=NA;
					OB->SetZeroFrame();
				}

			}

			if( NA && NA->Enabled && OB->NewAnm==NA ){
				int af=NA->ActiveFrame;
				if( af==0 || af==0xFF ){
					af=NA->NFrames*3/4;
				}
				if( OB->NewCurSpritePrev<af && OB->NewCurSprite>=af )
					OR->info.MoveToObj.wep=2;
			}else
				OR->info.MoveToObj.wep=2;

			if( OR->info.MoveToObj.wep==2 ){
				// Bonus
				bool ComBonusPickUp(byte NI, OneObject* Hero, OneObject* IOB);
				ComBonusPickUp(GSets.CGame.cgi_NatRefTBL[MyNation],OB,DialogOB);
				// Chest
				uaa_Chest* Ch=DetectChest(DialogOB);
				if( Ch && !Ch->Empty ){
					ActiveUnitAbility* aA=GetPickUpAbility(DialogOB);
					if(aA){
						UnitAbility* uA=aA->GetA();
						if(uA){
							/*if(uA->HeroLife){
								HeroVariableStorage* HVS=DetectHero(OB);
								if(HVS){
									HVS->Lives++;
								}
							}*/ //Adds to hero life as soon as the object is picked up
							//	EventReceiveItem(DialogOB,DialogOB,aA);
							//}else{
							ActiveUnitAbility* A=uA->GetNewActiveAbility();
							A->ItemDrop=aA->ItemDrop;

							// ----- EDIT BY XENO: sum magic card --------
							bool trytoadd = true;
							if (uA->Amount && !(aA->TypeAbil == 4 || aA->TypeAbil == 5))
							{
								itr_AA.Create(OB);
								while(ActiveUnitAbility* tAA = itr_AA.Next())
								{
									if(uA == tAA->GetA())
									{
										tAA->Amount += aA->Amount;
										trytoadd = false;
										break;
									}
								}
							}
							// ------- (remove 'trytoadd' to revert ----
							if(trytoadd && AddActiveUnitAbility(OB->Index,A))
								EventReceiveItem(OB,DialogOB,A);
							//}

							// визуальная информация
							WeaponModificator* wG=v_ISys.eGain.Get();
							if(wG){
								void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);

								//word UnitIndex=OB->Index;
								int sx=DialogOB->RealX>>4;
								int sy=DialogOB->RealY>>4;
								int sz=DialogOB->RZ+DialogOB->OverEarth;

								int dx=OB->RealX>>4;
								int dy=OB->RealY>>4;
								int dz=OB->RZ+OB->OverEarth;

								CreateNewActiveWeapon(wG->Name.pchar(),DialogOB->Index,sx,sy,sz,OB->Index,dx,dy,dz,NULL);
							}
						}
					}
					//EraseObject(DialogOB);
					//DialogOB->Die();
					Ch->Empty=true;
					Ch->NextSearchTime=TrueTime+500;
				}
			}else
				return;
		}else{
			//OB->CreatePath(x>>4,y>>4);
			OB->CreatePath(DialogOB->RealX/256,DialogOB->RealY/256);
			return;
		}
	}
	if( OB->LocalOrder && OB->LocalOrder->DoLink==&StartDialogLink ) 
		OB->DeleteLastOrder();
}
bool StartDialog(OneObject* OB, byte OrdType, OneObject* DialogOB){
	if( OB && DialogOB && !OB->UnlimitedMotion ){

		if(OB->ActiveAbility){
			bool PickUp=true;
			OB->ActiveAbility->ActiveAbilities.modifyItemPickUp(true,PickUp);
			if(!PickUp){
				return false;
			}
		}

		Order1* OR=OB->LocalOrder;
		if( OR && OR->DoLink==&StartDialogLink && OR->info.MoveToObj.ObjIndex==DialogOB->Index ) return false;
		
		OB->ClearOrders();

		OR=OB->CreateOrder(OrdType);
		if(OR){
			OR->DoLink=&StartDialogLink;
			OR->PrioryLevel=128+16;
			OR->info.MoveToObj.ObjIndex=DialogOB->Index;
			OR->info.MoveToObj.SN=DialogOB->Serial;
			OR->info.MoveToObj.wep=0; // состояние до анимации

			OB->PrioryLevel=OR->PrioryLevel;

			addrand(DialogOB->Index);
			
			return true;
		}
	}
	addrand(181);
	return false;
}

// CastingSpell
void CastingSpellLink(OneObject* OB){

	Order1* OR=OB->LocalOrder;
	OB->PrioryLevel=OR->PrioryLevel;

	int x=OR->info.Spell.Tx;
	int y=OR->info.Spell.Ty;
	int r=OR->info.Spell.Radius;
	//float scale=300.0/float(r);
	int cid=OR->info.Spell.CursorID;

	//bool DrawSelPatchDir(float x,float y,byte Dir,int Type,float ScaleX,float ScaleY,DWORD Color);
	//DrawSelPatchDir(x,y,OB->RealDir,cid,scale,scale,0x88FFFFFF);
	void add_SelPatch(float x,float y,int Type,float Radius,DWORD Color);
	add_SelPatch(x,y,cid,r,0x88FFFFFF);

	itr_AA.Create(OB);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		UnitAbility* UA=AA->GetA();
		if( UA && AA->Serial==OR->info.Spell.Serial ){
			AA->CastingSpell();
			break;
		}
	}

}
bool CastingSpell(OneObject* OB, byte OrdType){
	Order1* OR=OB->LocalOrder;
	OR->DoLink=&CastingSpellLink;

	if( VAI() ){
		int x=OR->info.Spell.Tx;
		int y=OR->info.Spell.Ty;
		int r=OR->info.Spell.Radius;
		itr_AA.Create(OB);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			UnitAbility* UA=AA->GetA();
			if( UA && AA->Serial==OR->info.Spell.Serial ){
				VAI()->OnCastSpell(OB->NNUM,UA,x,y,r<100?100:r);
				break;
			}
		}
	}

	return true;
}

// PrepareSpell
void PrepareSpellLink(OneObject* OB){
	
	Order1* OR=OB->LocalOrder;
	OB->PrioryLevel=OR->PrioryLevel;

	int x=OR->info.Spell.Tx;
	int y=OR->info.Spell.Ty;

	word TargetID=OR->info.Spell.TargetID;
	word TargetSN=OR->info.Spell.TargetSN;
	if( TargetID!=0xFFFF ){
		OneObject* Target=Group[TargetID];
		if( Target && Target->Serial==TargetSN ){
			x=Target->RealX/16;
			y=Target->RealY/16;
		}else{
			OB->DeleteLastOrder();
			return;
		}
	}
	
	int d=OR->info.Spell.Dist;

	addrand(x);
	addrand(y);
	addrand(d);
	int distancia;
	word dir=0xFFFF;
	if(d>0){
		int dx=x-OB->RealX/16;
		int dy=y-OB->RealY/16;
		int dist=Norma(dx,dy);

		addrand(dist);
		distancia = dist;
		if(dist>d&&!OB->NewBuilding){
			OB->CreatePath(x>>4,y>>4);
			return;
		}
		if(dist>0){
			dir=GetDir(dx,dy);
		}
	}

	bool stun=false;
	int speed=100;
	OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(speed,speed,stun); // оглушили перед кастованием

	addrand(stun);

	if(!stun){
		StopMotion(OB);
		CastingSpell(OB,0);

		addrand(dir);

		if(dir!=0xFFFF) RotUnit(OB,dir,1,128+16);
	}
	if (distancia > d&&OB->NewBuilding) {
		OB->DeleteLastOrder();
	}
}
bool PrepareSpell(OneObject* OB, byte OrdType, DWORD AblSerial, OneObject* Target, int Tx, int Ty, int Dist, int Radius, int CursorID){ //OneAbility* AA
	if( OB && !OB->UnlimitedMotion ){
		Order1* OR=OB->CreateOrder(OrdType);
		if(OR){
			OR->DoLink=&PrepareSpellLink;
			OR->PrioryLevel=128+16;

			OR->info.Spell.Serial=AblSerial;

			if( Target ){
				OR->info.Spell.TargetID=Target->Index;
				OR->info.Spell.TargetSN=Target->Serial;
			}else{
				OR->info.Spell.TargetID=0xFFFF;
			}			

			OR->info.Spell.Tx=Tx;
			OR->info.Spell.Ty=Ty;
			OR->info.Spell.Dist=Dist;

			OR->info.Spell.Radius=Radius;
			OR->info.Spell.CursorID=CursorID;

			OB->PrioryLevel=OR->PrioryLevel;

			addrand(AblSerial);
			addrand(Dist);
			
			return true;
		}
	}
	return false;
}

void DropDownItemLink(OneObject* OB){
	Order1* OR=OB->LocalOrder;
	OB->PrioryLevel=OR->PrioryLevel;

	int x=OR->info.Spell.Tx;
	int y=OR->info.Spell.Ty;
	int d=100;

	NewAnimation* NA=OB->newMons->GetAnimation(OR->info.Spell.CursorID);

	addrand(x);
	addrand(y);
	addrand(d);

	word dir=0xFFFF;
	if(d>0){
		int dx=x-OB->RealX/16;
		int dy=y-OB->RealY/16;
		int dist=Norma(dx,dy);

		addrand(dist);

		if(dist>d){
			OB->CreatePath(x>>4,y>>4);
			return;
		}

	}

	bool stun=false;
	int speed=100;
	OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(speed,speed,stun); // оглушили перед кастованием

	addrand(stun);

	if(!stun){

		if( dir!=0xFFFF && abs(int(OB->RealDir)-int(dir))>15 ){
			RotUnit(OB,dir,1,128+16);
			return;
		}

		int& animation=OR->info.Spell.Radius;
		
		if( animation==0 ){
			StopMotion(OB);
			if( NA && NA->Enabled ){
				animation=1;				
				OB->NewAnm=NA;
				OB->SetZeroFrame();
			}else{
				animation=2;
			}
		}else if( animation==1 ){
			if( OB->NewAnm!=NA || OB->FrameFinished ){
				animation=2;
			}
		}
		if( animation==2 ){
			DWORD AblSerial=OR->info.Spell.Serial;

			itr_AA.Create(OB);
			while(ActiveUnitAbility* AA=itr_AA.Next()){
				UnitAbility* UA=AA->GetA();
				if( UA && AA->Serial==AblSerial ){
					AA->DropDown(x,y);
					break;
				}
			}

			addrand(dir);

			if(dir!=0xFFFF) RotUnit(OB,dir,1,128+16);

			OB->DeleteLastOrder();
		}
	}

}
bool DropDownItem(OneObject* OB, byte OrdType, DWORD AblSerial, int Tx, int Ty, int Dist){
	if( OB && !OB->UnlimitedMotion ){
		Order1* OR=OB->CreateOrder(OrdType);
		if(OR){
			OR->DoLink=&DropDownItemLink;
			OR->PrioryLevel=128+16;

			OR->info.Spell.Serial=AblSerial;

			OR->info.Spell.Tx=Tx;
			OR->info.Spell.Ty=Ty;

			OR->info.Spell.Radius=0;

			int& anm=OR->info.Spell.CursorID;
			int& dst=OR->info.Spell.Dist;

			anm=-1;
			dst=Dist;

			int distMax=-1;
			int anmMax=-1;

			int dx=Tx-OB->RealX/16;
			int dy=Ty-OB->RealY/16;
			int dist=Norma(dx,dy);

			AdvCharacter* AC=OB->MoreCharacter;
			for(int i=0;i<NAttTypes;i++){
				int d1=AC->AttackRadius1[i];
				int d2=AC->AttackRadius2[i];
				if( AC->MaxDamage[i]==0 ){
					if( dist>=d1 && dist<=d2 ){
						anm=i;
					}
					if( distMax<d2 ){
						distMax=d2;
						anmMax=i;
					}
				}
			}

			if( anm==-1 ){
				anm=anmMax;
				dst=distMax;
			}else{
				dst=dist;
			}
			anm+=anm_Attack;

			//OR->info.Spell.Radius=Radius;
			//OR->info.Spell.CursorID=CursorID;

			OB->PrioryLevel=OR->PrioryLevel;

			addrand(AblSerial);
			addrand(Dist);

			return true;
		}
	}
	return false;
}
