#include "stdheader.h"
#include "ua_Item.h"
#include ".\cvi_MainMenu.h"
#include ".\cvi_HeroButtons.h"
#include "UnitsInterface.h"
#include "UnitChar.h"
#include "ua_Extern.h"
#include "ua_Modificators.h"
//==================================================================================================================//
//
void ProduceObjLink(OneObject* OBJ);
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
void EraseObject(OneObject* OB);
DLLEXPORT int GetRND(int Max);
extern int TrueTime;
CEXPORT char* GetResourceName(byte ResID);
bool SetHeroToSettlement(OneObject* Hero,OneObject* Settlement);

OneItem* AddItem(word ItemNIndex, OneObject* Dest);
bool DetectMagazine(OneObject* OB);

void EraseObjectWithoutAbilities(OneObject* OB);

//
//==================================================================================================================//
//

ua_CookBook CookBook;

bool ua_CookBook::CheckProduct(OneObject* OB){
	
	if(DetectMagazine(OB)){
		return false;
	}

	for(int i=0;i<Cookies.GetAmount();i++){
		ua_Cookie* Cookie=Cookies[i];
		if(Cookie){
			
			static DynArray<ActiveUnitAbility*> Check;

			int N=Cookie->Ingredients.GetAmount();
			if(N){
				Check.Check(N);
				Check.NValues=N;
				for(int i=0;i<N;i++) Check[i]=NULL;

				int Count=N;

				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){

					UnitAbility* UA=AA->GetA();
					if( UA && !AA->Deleted ){

						for(int i=0;i<N;i++){
							if( !Check[i] && UA==Cookie->Ingredients[i]->Get() ){
								bool skip=false;
								for(int j=0;j<Check.GetAmount();j++){
									if(Check[j]==AA)
										skip=true;
								}
								if(!skip){
									Check[i]=AA;
									Count--;
								}
							}
						}

					}
					if(Count==0) break;

				}

				if(Count<=0){
					for(int i=0;i<N;i++){
						Check[i]->Deleted=true;
					}

					UnitAbility* UA=Cookie->Product.Get();
					if(UA){
						ActiveUnitAbility* newA=UA->GetNewActiveAbility();
						return AddActiveUnitAbility(OB->Index,newA);
					}
				}
			}
		}
	}
	return false;
};

//

NewItem::NewItem(){
	SoundExecute=GetSound("potion_use");
	SoundDress=GetSound("items_on");
	SoundUndress=GetSound("items_off");
	//Type==2;
};
ActiveUnitAbility* NewItem::GetNewActiveAbility(){
	OneItem* nAA = new OneItem;
	nAA->SetA(this);

	nAA->Amount=Amount;
	if(Clothe){
		nAA->TypeAbil=4;
		if(!CanNotBeClothe)
			nAA->Clothed=true;
	}else{
		nAA->TypeAbil=5;
		if(!CanNotBeClothe)
			nAA->Clothed=true;
	}
	nAA->ItemDrop=ItemDrop;

	return nAA;
}

ActiveUnitAbility* NewItemTransformer::GetNewActiveAbility(){
	OneItemTransformer* nAA = new OneItemTransformer;
	//
	nAA->Amount=Amount;
	//
	nAA->SetA(this);
	return nAA;
}
UnitAbility* OneItemTransformer::GetA(){
	NewItemTransformer* NI=(NewItemTransformer*)ActiveUnitAbility::GetA();
	if(NI){
		AdvCharacter* AC=OB->MoreCharacter;
		int id=0;
		for(int i=0;i<NAttTypes;i++){
			if(AC->MaxDamage[i]>0){
				id=AC->WeaponKind[i];				
				break;
			}
		}
		if(id<4){
			//return NI->WeaponKind[id].Get();
			UnitAbility* UA=NI->WeaponKind+id;
			//
			FileID=UA->FileID;
			SpriteID=UA->SpriteID;
			if(UA->Hint) Hint=GetTextByID(UA->Hint);
			//
			return UA;
		}
	}
	return NULL;
};
/*
bool OneItemTransformer::Process(){
	return true;
};
*/
//
ActiveUnitAbility* NewMagazine::GetNewActiveAbility(){
	OneMagazine* nAA = new OneMagazine;	
	nAA->SetA(this);
	return nAA;
}
ActiveUnitAbility* ua_DropPackage::GetNewActiveAbility(){
	uaa_DropPackage* nAA=new uaa_DropPackage;
	nAA->SetA(this);
	return nAA;
};

//
NewItem* DetectItem(word NIndex){
	
	NewItem* resNI=NULL;
	
	NewMonster* NM=NATIONS->Mon[NIndex]->newMons;
	if(NM->Ability){
		NM->Ability->Init(NM);
		int n=NM->Ability->AbilitiesList.GetAmount();
		for(int i=0;i<n;i++){
			UnitAbility* UA=NM->Ability->AbilitiesList[i];
			NewMagazine* OM=dynamic_cast<NewMagazine*>(UA);
			if(OM){
				return false;
			}
			NewBonus* NB=dynamic_cast<NewBonus*>(UA);
			if(NB){
				return false;
			}
			if(!resNI){
				NewItem* NI=dynamic_cast<NewItem*>(UA);
				if(NI){
					resNI=NI;
				}
			}
		}
	}
	return resNI;
}
UnitAbility* DetectMagicCard(word NIndex){
	UnitAbility* MC=NULL;
	NewMonster* NM=NATIONS->Mon[NIndex]->newMons;
	if(NM->Ability){
		NM->Ability->Init(NM);
		int n=NM->Ability->AbilitiesList.GetAmount();
		for(int i=0;i<n;i++){
			UnitAbility* AA=NM->Ability->AbilitiesList[i];
			//if (AA && AA->TypeAbil==4) {
				MC=AA;
			//}
			HeroVariableStorage* HA=dynamic_cast<HeroVariableStorage*>(AA);
			if(HA){
				return NULL;
			}
		}
	}
	return MC;
}

HeroVariableStorage* GetHeroVariableStorage(OneObject* OB);
HeroVariableStorage* DetectHero(OneObject* OB){
	if(OB&&OB->ActiveAbility){
		if( !OB->ActiveAbility->CheckHero || OB->ActiveAbility->Hero ){
			OB->ActiveAbility->CheckHero=true;
			//bool sum=false;
			//OB->ActiveAbility->ActiveAbilities.modifySummon(sum);
			//if(!sum){
				HeroVariableStorage* HVS=GetHeroVariableStorage(OB);
				OB->ActiveAbility->Hero=HVS;
				return HVS;
			//}
		}
	}
	return NULL;
	//if(OB&&OB->ActiveAbility){
	//	if(!OB->ActiveAbility->CheckHero){
	//		OB->ActiveAbility->CheckHero=true;
	//		int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
	//		for(int i=0;i<n;i++){
	//			HeroVariableStorage* HA=dynamic_cast<HeroVariableStorage*>(OB->ActiveAbility->ActiveAbilities[i]);
	//			if(HA){
	//				OB->ActiveAbility->Hero=HA;
	//				break;
	//			}
	//		}
	//	}
	//	return OB->ActiveAbility->Hero;
	//}
	//return NULL;
}
bool GetHero(OneObject* OB){
	//if(OB&&OB->ActiveAbility){
	//	return OB->ActiveAbility->CheckHero&&OB->ActiveAbility->Hero || DetectHero(OB);
	//}
	//return false;
	return DetectHero(OB);
}
uaa_Chest* DetectChest(OneObject* OB){
	if(OB&&OB->ActiveAbility){
		if( !OB->ActiveAbility->CheckChest || OB->ActiveAbility->Chest ){
			OB->ActiveAbility->CheckChest=true;
			if(!DetectHero(OB)){
				itr_AA.Create(OB);
				while(ActiveUnitAbility* aA=itr_AA.Next()){
					uaa_Chest* Ch=dynamic_cast<uaa_Chest*>(aA);
					if(Ch){
						OB->ActiveAbility->Chest=true;
						return Ch;
					}
				}
			}
			OB->ActiveAbility->Chest=false;
		}
	}
	return NULL;
}
bool isNotChest(OneObject* OB){
	return !DetectChest(OB);
}
bool uaa_Chest::Process(){
	if(NextSearchTime<TrueTime){
		
		//if(OB->newMons->Immortal)
		//	OB->NMask=0xFF;

		if(Empty){
			EraseObjectWithoutAbilities(OB);
			return false;
		}else{
			NextSearchTime=TrueTime+2000;
			itr_UnitsInRadius.Create(OB->RealX>>4,OB->RealY>>4,300);
			bool Open=false;
			while(OneObject* oB=itr_UnitsInRadius.Next()){
				if(DetectHero(oB)){
					Open=true;
					break;
				}
			}
			if(Open){
				NewAnimation* naAtt0=OB->newMons->GetAnimation(anm_Attack+0);
				if( naAtt0 && naAtt0->Enabled && OB->NewState!=1 ){
					OB->NewState=1;
					OB->GroundState=1;
					TryToStand(OB,false);
				}
			}else{
				OB->NewState=0;
				OB->GroundState=0;
				TryToStand(OB,false);
			}
		}
	}
	return OneAbility::Process();
}
OneItem* AddItem(word ItemNIndex, OneObject* Dest){
	NewItem* NI=DetectItem(ItemNIndex);
	if(NI){
		// add one item
		OneItem* I=(OneItem*)NI->GetNewActiveAbility();
		AddActiveUnitAbility(Dest->Index,I);
		//
		I->fst=false;
		I->NIndex=ItemNIndex;

		//CookBook.CheckProduct(Dest);

		return I;
	}
	return NULL;
}
ActiveUnitAbility* AddMagicCard(word ItemNIndex, OneObject* Dest){
	UnitAbility* UA=DetectMagicCard(ItemNIndex);
	if(UA){
		// add one item
		ActiveUnitAbility* A=(ActiveUnitAbility*)UA->GetNewActiveAbility();
		AddActiveUnitAbility(Dest->Index,A);
		//
		//A->fst=false;
		//A->NIndex=ItemNIndex;
		return A;
	}
	return NULL;
}
bool DetectMagazine(OneObject* OB){
	itr_AA.Create(OB);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		OneMagazine* OM=dynamic_cast<OneMagazine*>(AA);
		if(OM){
			return OM->Visible;
		}
	}
	return false;
}
OneMagazine* DetectMagazine2(OneObject* OB){
	itr_AA.Create(OB);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		OneMagazine* OM=dynamic_cast<OneMagazine*>(AA);
		if(OM){
			if(OM->Visible)	return OM;
				else return NULL;
		}
	}
	return NULL;
}

void MarkObjectLikeModified(OneObject* OB);

bool OneMagazine::Process(){
	
	//if(OB->newMons->Immortal)
	//	OB->NMask=0xFF;

	//if(OB->NewBuilding){
	//	//MarkObjectLikeModified(OB);
	//	OB->newMons->Capture=true;
	//}

	//byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//if(/*OB->NNUM==NI&&*/(OB->ImSelected&(1<<NI))){
	byte NI=0;
	if(true){
		// найти героя в радиусе магазина
		OneObject* Hero=Group[HeroID];
		if(!Hero||HeroID==0xFFFF||HeroSN!=Hero->Serial||!DetectHero(Hero)||Norma(OB->RealX-Hero->RealX,OB->RealY-Hero->RealY)>16*1000){
			Hero=NULL;
			HeroID=0xFFFF;
			itr_UnitsInRadius.Create(OB->RealX>>4,OB->RealY>>4,1000);
			while(OneObject* O=itr_UnitsInRadius.Next()){
				if(DetectHero(O)){
					Hero=O;
					HeroID=O->Index;
					HeroSN=O->Serial;
					break;
				}
			}
		}
		// установить возможность постройки предметов
		Nation* Nat=NATIONS+NI;
		//City* CT=CITY+NI;
		//word NIndex=SP->NIndex;
		//
		int NBuild=Nat->PACount[OB->NIndex];
		word* Build=Nat->PAble[OB->NIndex];
		for(int i=0;i<NBuild;i++,Build++){
			NewItem* NI=DetectItem(*Build);
			if(NI){
				GeneralObject* IGO=Nat->Mon[*Build];
				IGO->Enabled=Hero;
				IGO->newMons->MinIconFile=NI->FileID;
				IGO->newMons->MinIconIndex=NI->SpriteID;
				//price
				for(int c=0;c<8;c++){
					Nation* NT=&NATIONS[c];
					GeneralObject* GO=NT->Mon[*Build];
					AdvCharacter* ADC=GO->MoreCharacter;
					for (int i=0;i<8;i++) ADC->NeedRes[i]=0;
					ADC->NeedRes[GoldID]=NI->ShopPrice;
				}
			}
		}
		// очистка инвентаря
		/*
		int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i=0;i<n;i++){
			ActiveUnitAbility* AUA=OB->ActiveAbility->ActiveAbilities[i];
			if(AUA&&(AUA->TypeAbil==4||AUA->TypeAbil==5)){
				((OneItem*)AUA)->Deleted=true;
			}
		}
		*/
		if(Hero){
			// поиск строящегося предмета в приказах здания
			//if(OB->LocalOrder&&OB->LocalOrder->DoLink==&ProduceObjLink){
			//	word NIndex=OB->LocalOrder->info.Produce.ObjIndex;
			//	OneItem* OI=AddItem(NIndex,Hero);
			//	if(OI){
			//		// delete order
			//		OB->DeleteLastOrder();
			//	}else{
			//		ActiveUnitAbility* AA=AddMagicCard(NIndex,Hero);
			//		if(AA){
			//			// delete order
			//			OB->DeleteLastOrder();
			//		}
			//	}
			//}
			// отображение предметов в инвентаре героя
			/*
			int n=Hero->ActiveAbility->ActiveAbilities.GetAmount();
			for(int i=0;i<n;i++){
				ActiveUnitAbility* AUA=Hero->ActiveAbility->ActiveAbilities[i];
				if(AUA&&(AUA->TypeAbil==4||AUA->TypeAbil==5)){
					//OneItem* I=new OneItem;
					//AUA->Copy(I);
					OneItem* I=(OneItem*)AUA->GetA()->GetNewActiveAbility();
					if(I){
						AddActiveUnitAbility(OB->Index,I);
						I->Clothed=AUA->Clothed;
					}
				}
			}
			*/
		}
	}
	return true;
};
//
bool OneItem::Process(){
	NewItem* NI=(NewItem*)GetA();
	if(NI){
		//if(TypeAbil==0||TypeAbil==4){
		//	if(NI->WeaponShots){
		//		if(WeaponShots>=NI->WeaponShots){
		//			return false;
		//		}else{
		//			return true;
		//		}
		//	}
		//}
		if(Amount<=0){
			if(TypeAbil==5){
				SeekSameAndClothe();
			}
			return false;
		}
		if(Deleted){
			return false;
		}
	}
	//
	return OneAbility::Process();
};

ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
extern int vClotheSet[];

bool OneItem::OnClick(){
	if( DetectMagazine(OB) || DetectBonus(OB) )
		return false;
	NewItem* NI=(NewItem*)GetA();
	if(NI){
		if(TypeAbil==4){
			SetClothe(!Clothed,-1);
		}else if(TypeAbil==5){
			HeroVariableStorage* HVS=DetectHero(OB);
			if( HVS ){
				HeroAbility* HA=(HeroAbility*)HVS->GetA();
				UnitAbility* uaDie=HA->OnDieUnitAbility.Get();
				if( uaDie ){
					itr_AA.Create(OB);
					while(ActiveUnitAbility* AA=itr_AA.Next()){
						UnitAbility* UA=AA->GetA();
						if( /*AA->TypeAbil==0 && */UA==uaDie){//Pause Mode test
							HVS=NULL;
							break;
						}
					}
				}
			}
			if( HVS && NI->Conditions.Check(this) ){
				AddOneEffect(NULL,NI,OB,OB);
				Amount--;
				if( NI->SoundExecute!=-1 && OB->NNUM==MyNation )
					AddEffect(OB->RealX/16,OB->RealY/16,NI->SoundExecute);
					//PlayEffect(NI->SoundExecute,0,0);

				// Hero life
				if(NI->HeroLife){
					HVS->Lives++;
				}

			}
		}
	}
	//return OneAbility::OnClick();
	return true;
};
bool OneItem::OnRightClick(){
	if( DetectMagazine(OB) || DetectBonus(OB) )
		return false;
	//NewItem* NI=(NewItem*)GetA();
	//if(NI){
	//	if(TypeAbil==5){
	//		if( NI->Conditions.Check(this) ){
	//			AddOneEffect(NULL,NI,OB,OB);
	//			Amount--;
	//			if( NI->SoundExecute!=-1 && OB->NNUM==MyNation )
	//				PlayEffect(NI->SoundExecute,0,0);
	//		}
	//	}
	//}
	return true;
};
bool OneItem::isTarget(){
	NewItem* NI=(NewItem*)GetA();	
	if(NI&&NI->SelfTarget){
		return false;
	}
	return OneAbility::isTarget();
}
bool OneItem::CanTarget(word TargetUnit, int x, int y, int z){
	if(GetCoolDownProc()>0)
		return false;
	return true;
}
int OneItem::GetCoolDownProc(){
	if( TypeAbil==4 || TypeAbil==5 ){
		return 0;
	}
	return OneAbility::GetCoolDownProc();
};
ActiveUnitAbility* AddAbility(UnitAbility* Source, OneObject* Dest, int TypeAbil=2);

ActiveUnitAbility* AddAblNameAbility(OneObject* OB, AblName* AN){
	if(OB&&AN){
		char* name=AN->AbilityName.pchar();
		if(name){
			extern AbilityList Abilities;
			int m = Abilities.Abilities.GetAmount();
			for(int j=0;j<m;j++){
				if(!strcmp(name,Abilities.Abilities[j]->Name.pchar())){
					UnitAbility* UA=Abilities.Abilities[j];
					ActiveUnitAbility* newA=AddAbility(UA,OB,7);
					if(newA){
						newA->Name=AN->Name;
						newA->Hint=AN->Hint;
						newA->be_Dialog=AN;
						if(DriveMode()->PROCESS){
							newA->be_Dialog->be_EnterDialog.CallFunction(newA);
						}
						newA->FileID=AN->PicFile;
						newA->SpriteID=AN->PicSprite;
						addrand(UA->Index);
						return newA;
					}
					break;
				}
			}
		}
	}
	return NULL;
};

// special for StarForce
__declspec(dllexport)
bool EnterMagazine(OneObject* Hero, OneObject* Lib){
	
	if(!DetectMagazine(Lib)){
		return true;
	}

	if( Hero->ActiveAbility->LibID==Lib->Index && Hero->ActiveAbility->LibSN==Lib->Serial){
		return true;
	}
	Hero->ActiveAbility->LibID=Lib->Index;
	Hero->ActiveAbility->LibSN=Lib->Serial;

	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(NI==Hero->NNUM){
		IGI.Library=true;
		IGI.Inventory=true;
	}
	
	itr_AA.Create(Hero);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		if(AA->TypeAbil==7){
			AA->Deleted=true;
		}
	}

	addrand(Lib->Index);

	itr_AA.Create(Lib);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		if(AA->TypeAbil==7){
			UnitAbility* UA=AA->GetA();
			ActiveUnitAbility* newA=AddAbility(UA,Hero,7);
			if(newA){

				newA->Visible=AA->Visible;

				newA->Name=AA->Name;
				newA->Hint=AA->Hint;

				OneAbility* OA=dynamic_cast<OneAbility*>(newA);
				if(OA){
					((OneAbility*)AA)->be_Process.Copy(&OA->be_Process,false);
					((OneAbility*)AA)->be_LeftClick.Copy(&OA->be_LeftClick,false);
					((OneAbility*)AA)->be_EnterDialog.Copy(&OA->be_EnterDialog,false);
					if(DriveMode()->PROCESS){
						OA->be_EnterDialog.CallFunction(OA);
					}
				}

				addrand(UA->Index);
			}
		}
	}

	ClonesArray<AblName>* ANA=GetDialogAbilities(Lib);
	if(ANA){
		for(int i=0;i<ANA->GetAmount();i++){
			AblName* AN=(*ANA)[i];
			AddAblNameAbility(Hero,AN);
		}
	}

	ext_OnEnterShop(Hero->Index,Lib->Index);

	return true;
}

bool DetectMagazine(OneObject* OB);

void SetMagazineState(OneObject* OB, bool Enable){
	OneMagazine* OM=DetectMagazine2(OB);
	if(OM){
		OM->Visible=Enable;
	}
};
bool GetMagazineState(OneObject* OB){
	OneMagazine* OM=DetectMagazine2(OB);
	return OM;
};

bool ItemPickUp(word ItemID){
	if(ItemID!=0xFFFF&&OIS.SelPoint.GetAmount()){
		OneObject* Item=Group[ItemID];
		if( Item ){
			vui_SelPoint* SP=OIS.GetLastSelPoint();
			if( !( SP->OB->newMons->NewParams.AntiMage && 
				Item->newMons->NewParams.VulnerableOnlyForAntimage) ){
					byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
					bool NMask=Item->NMask&NATIONS[NI].NMask;

					bool shop = /*NMask && */DetectMagazine(Item);

					uaa_Chest* Ch=DetectChest(Item);
					bool chest = Ch && !Ch->Empty;

					OneBonus* Bonus=DetectBonus(Item);
					bool bonus = /*NMask && */Bonus && Bonus->GetCoolDownProc()==0 && Bonus->isCharged();

					if( shop || chest || bonus ){
						int n=OIS.SelPoint.GetAmount();
						//for(int i=0;i<n;i++){
						if(n>0){
							int i=0;
							vui_SelPoint* SP=OIS.SelPoint+i;
							byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
							if( SP->NI==NI && DetectHero(SP->OB) && ItemID!=SP->OB->Index ){
								VitCursor=11;
								if(Rpressed){
									OneObject* Hero=SP->OB;
									vui_IS_ItemPickUp IPU;
									IPU.Data.HeroID=Hero->Index;
									IPU.Data.HeroSN=Hero->Serial;
									IPU.Data.ItemID=Item->Index;
									IPU.Data.ItemSN=Item->Serial;
									IPU.InterfAction();
									return true;
								}
							}
						}
					}

			}
		}
	}
	return false;
};


bool ComItemPickUp(OneObject* Hero, OneObject* IOB){
	
	addrand(IOB->Index);

	bool StartDialog(OneObject* OB, byte OrdType, OneObject* DialogOB);
	if(StartDialog(Hero,0,IOB)){
		return false;
	};

	return false;
}
//
//==== Bonus ===//
//
ActiveUnitAbility* NewBonus::GetNewActiveAbility(){
	OneBonus* nAA = new OneBonus;
	nAA->SetA(this);
	//
	nAA->TypeAbil=2;
	nAA->Charge=Charge;
	//
	return nAA;
};
bool OneBonus::Process(){

	//if(OB->newMons->Immortal)
	//	OB->NMask=0xFF;

	//if(OB&&OB->NewBuilding){
	//	//MarkObjectLikeModified(OB);
	//	OB->newMons->Capture=true;
	//}

	//
	if(DriveMode()->PROCESS){
		be_Process.CallFunction((ActiveUnitAbility*)this);
		if(be_Dialog){
			be_Dialog->be_Process.CallFunction((ActiveUnitAbility*)this);
		}
	}
	//

	NewBonus* NB=(NewBonus*)GetA();
	if( isCharged() ){
		if(NB->Charge>0){
			int Cool=GetCoolDownProc();
			Visible=(Cool==0);
		}
		if(TypeAbil!=0){
			TypeAbil=2;
		}
		bool res=OneAbility::Process();
		return res;
	}
	Visible=false;
	return false;
};
OneBonus* DetectBonus(OneObject* OB){
	if(OB&&!OB->Sdoxlo&&OB->ActiveAbility){
		itr_AA.Create(OB);
		while(ActiveUnitAbility* aA=itr_AA.Next()){
			OneBonus* aB=dynamic_cast<OneBonus*>(aA);
			if( aB && aB->Visible && aB->TypeAbil!=0 ){
				return aB;
			}
		}
	}
	return NULL;
}
//
ActiveUnitAbility* AddAbility(UnitAbility* Source, OneObject* Dest, int TypeAbil);
//
bool BonusPickUp(word BonusID){
	if(BonusID!=0xFFFF&&OIS.SelPoint.GetAmount()){
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(SP&&DetectHero(SP->OB)){
			OneObject* Hero=SP->OB;
			OneObject* IOB=Group[BonusID];
			OneBonus* Bonus=DetectBonus(IOB);
			if (Bonus && Bonus->GetCoolDownProc()==0 && Bonus->isCharged()) {

				vui_IS_BonusPickUp IPU;
				IPU.Data.NI=GSets.CGame.cgi_NatRefTBL[MyNation];
				IPU.Data.HeroID=Hero->Index;
				IPU.Data.HeroSN=Hero->Serial;
				IPU.Data.BonusID=IOB->Index;
				IPU.Data.BonusSN=IOB->Serial;
				IPU.InterfAction();

				return true;
			}
		}
	}
	return false;
};

void EventReceiveItem(OneObject* Acceptor, OneObject* ItemOB, ActiveUnitAbility* AA, char* Msg=NULL) {

	UnitAbility* UA=AA->GetA();
	if(UA){
		// информационное сообщение
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		if(Acceptor->NNUM==NI){
			
			static char* msg=GetTextByID("#GainMessage");
			if(Msg==NULL)
				Msg=msg;
			//h.print("%s %s",msg,newA->GetHint());

			OneObject* tOB=AA->OB;
			AA->OB=Acceptor;
			int f,s,fchr,finv;
			AA->GetButton(f,s,fchr,finv);
			//int lx=GPS.GetGPWidth(f,s);
			//int ly=GPS.GetGPHeight(f,s);
			
			DString Title=UA->HintName;
			mod_Symbol* M=NULL;
			itr_UM.Create(AA,0xFFFFFFFF);
			while(UnitModificator* m=itr_UM.Next()){
				M=dynamic_cast<mod_Symbol*>(m);
				if(M)
					break;
			};
			if(M)
				Title=M->HintName;

			AA->OB=tOB;

			static _str h;
			h.Clear();

			//h.print("{G %d %d 0 %d %d %d}{Ftm12}{C FF00F300}  %s %s",f,s,-ly/2,lx,ly,Msg,UA->HintName);
			h.print("{G %d %d}{Ftm12}{C FF00F300}  %s: %s",finv,s,Msg,Title.pchar());
			AssignHint1(h.pchar(),350);

			// отключено дабы не загромаждать экран
			//cvi_Hero* HB=vHeroButtons.Get(Acceptor);
			//if(HB){
			//	IncomingAbility* IA = new IncomingAbility;
			//	IA->Serial=AA->Serial;

			//	Vector3D v;
			//	v.x=ItemOB->RealX>>4;
			//	v.y=ItemOB->RealY>>4;
			//	v.z=ItemOB->RZ+ItemOB->OverEarth+150;

			//	IA->x=v.x;
			//	IA->y=v.y;

			//	HB->InAbl.Add(IA);
			//}

		}
	}

}

ActiveUnitAbility* GetDropAbility(OneObject* OB, UnitAbility* &UA);

bool ComBonusPickUp(byte NI, OneObject* Hero, OneObject* IOB){
	OneBonus* Bonus=DetectBonus(IOB);
	if( Bonus && Bonus->GetCoolDownProc()==0 && Bonus->isCharged() && Bonus->Visible && !(Bonus->Deleted) ){
		addrand(IOB->Index);
		NewBonus* NB=(NewBonus*)Bonus->GetA();
		if(NB){
			//int r=NB->Distance.Get();
			//if(r==0) r=300;
			//if(Norma(IOB->RealX-Hero->RealX,IOB->RealY-Hero->RealY)<r*16){

				Bonus->be_LeftClick.CallFunction(Bonus,Hero);
				
				ActiveUnitAbility* Ef=AddOneEffect(NULL,NB,Hero,Hero);
				if(Ef){
				}
				
				UnitAbility* UA=NULL;
				ActiveUnitAbility* A=GetDropAbility(IOB,UA);
				if( A && UA ){
					
					NewItem* NI=dynamic_cast<NewItem*>(UA);
					if( NI && NI->SoundDress!=-1 && Hero->NNUM==MyNation )
						PlayEffect(NI->SoundDress,0,0);

					ActiveUnitAbility* newA=UA->GetNewActiveAbility();
					if(newA){
						//newA->TypeAbil=TypeAbil;
						AddActiveUnitAbility(Hero->Index,newA);
						newA->DeleteAfterUse=Bonus->DeleteAfterUse;

						EventReceiveItem(Hero,IOB,newA);

						// визуальная информация
						WeaponModificator* wG=v_ISys.eGain.Get();  
						if(wG){
							void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
							
							//word UnitIndex=OB->Index;
							int sx=IOB->RealX>>4;
							int sy=IOB->RealY>>4;
							int sz=IOB->RZ+IOB->OverEarth;

							int dx=Hero->RealX>>4;
							int dy=Hero->RealY>>4;
							int dz=Hero->RZ+Hero->OverEarth;

							CreateNewActiveWeapon(wG->Name.pchar(),IOB->Index,sx,sy,sz,Hero->Index,dx,dy,dz,NULL);
						}
					}
				}else{
					
					if( NB->SoundExecute!=-1 && Hero->NNUM==MyNation )
						AddEffect(IOB->RealX/16,IOB->RealY/16,NB->SoundExecute);

					// Money
					if(GetRND(64)>32){
						for(int i=0;i<NB->Money.GetAmount();i++){
							int a=NB->Money[i]->Value;
							int R=NB->Money[i]->ValueRandom;
							addrand(a);
							addrand(R);
							addrand(i);
							if (R) a+=rando()%R;
							if(a){
								byte res=NB->Money[i]->Resource;
								addrand(res);
								AddXRESRC(Hero->NNUM,res,a);
								int x=IOB->RealX>>4;
								int y=IOB->RealY>>4;
								if(NI==Hero->NNUM){
									char s[64];
									sprintf(s,"+%d %s",a,GetResourceName(res));
									void AddIllustrator(int x,int y,char* text,RLCFont* Font,byte Type,DWORD Color);
									AddIllustrator(x+100-(rand()%200),y+100-(rand()%200),s,&SmallWhiteFont,254,0xFF00FFFF);
								}
							}
						}
					}else{
						int a=NB->Money2.Value;
						int R=NB->Money2.ValueRandom;
						addrand(a);
						addrand(R);
						if (R) a+=rando()%R;
						if(a){
							byte res=NB->Money2.Resource;
							//addrand(res);
							AddXRESRC(Hero->NNUM,res,a);
							int x=IOB->RealX>>4;
							int y=IOB->RealY>>4;
							if(NI==Hero->NNUM){
								char s[64];
								sprintf(s,"+%d %s",a,GetResourceName(res));
								void AddIllustrator(int x,int y,char* text,RLCFont* Font,byte Type,DWORD Color);
								AddIllustrator(x+100-(rand()%200),y+100-(rand()%200),s,&SmallWhiteFont,254,0xFF00FFFF);							
							}
						}
					}
				}
				
				// Hero life (Enabled for Test by FlameMaster)
				//if(NB->HeroLife){
				//	HeroVariableStorage* HVS=DetectHero(Hero);
				//	if(Hero){
				//		HVS->Lives++;
				//	}
				//}

				// Bonus
				Bonus->StartCoolDown();
				//Bonus->eAuraStartTime=TrueTime;

				Bonus->Charge--;

				if(NB->EraseAfterUse){
					EraseObjectWithoutAbilities(IOB);
				}
				return true;
			//}
		}
	}
	return false;
}

void InitUnitAbilities(OneObject* OB);

bool DeleteItemDrops(OneObject* OB){
	bool res=false;
	itr_AA.Create(OB);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		if(AA->ItemDrop>0){
			AA->Deleted=true;
			res=true;
		}
	}
	if(res){
		if(!OB->newMons->CharacterWasModified){
			
			void MarkObjectLikeModified(OneObject* OB);
			MarkObjectLikeModified(OB);

			//itr_AA.Create(OB);
			//while(ActiveUnitAbility* AA=itr_AA.Next()){
			//	if(AA->ItemDrop>0){
			//		AA->Deleted=true;
			//		res=true;
			//	}
			//}

		}
		if(OB->newMons->CharacterWasModified){
			MonsterAbility* A=OB->newMons->Ability;
			if(A){
				for(int i=0;i<A->ItemDrop.GetAmount();i++){
					if(A->ItemDrop[i]>0){

						A->AbilityNames.DelElement(i);
						A->ItemDrop.Del(i,1);
						A->AbilitiesList.Del(i,1);

						//A->SmartObjects.DelElement(i);
						//A->SmartCommands.DelElement(i);

						A->Names.DelElement(i);
						A->PicFiles.DelElement(i);
						A->PicSprites.DelElement(i);
						A->Hints.DelElement(i);

						A->DeleteAfterUse.DelElement(i);

						A->be_Processes.DelElement(i);
						A->be_LeftClicks.DelElement(i);
						A->be_EnterDialogs.DelElement(i);

						i--;
					}
				}
			}
			InitUnitAbilities(OB);
		}		
	}
	return res;
}
extern NewMonster* NewMon[2048];
bool SetDefault(OneObject* OB){
	if(OB->newMons->CharacterWasModified){
		NewMonster* NM=NewMon[OB->newMons->MyIndex];
		//delete(OB->newMons);
		OB->newMons=NM;
		//delete(OB->MoreCharacter);
		OB->MoreCharacter=OB->Ref.General->MoreCharacter;

		InitUnitAbilities(OB);
	}
	return true;
}

bool OneItem::GetAuraActiveState(){
	return Clothed;
};