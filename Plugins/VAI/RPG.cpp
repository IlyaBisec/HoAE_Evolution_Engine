
#include "stdafx.h"
#include "RPG.h"
#include "Moderator.h"

RPG_AI RPG;

void SetTopMap(DynArray<word>& Map, int CurTop, word Value){
	Map[CurTop]=Value;
	//int n=GameInterface::GetTZonesAmount(0);
	int n=GameInterface::GetTZoneNLinks(0,CurTop);
	for(int l=0;l<n;l++){
		TLinkInfo* LI=GameInterface::GetTZoneLink(0,CurTop,l);
		if(LI){
			int t=LI->NextAreaID;
			if(Map[t]==0xFFFE){
				SetTopMap(Map,t,Value);
			}
		}
	}
}

void RPG_AI::Unload(){
	Group.Clear();
};
void RPG_AI::Process(){

	// init
	SoldierMap& mSoldier=BF.M[0].AllSoldiers;
	if( !Init && mSoldier.size()>0 ){
		Init=true;
		//Creeps

		//itr_GetUnitsOfNation.Create(7);
		//while(OneObject* OB=itr_GetUnitsOfNation.Next()){
		//	if( OB && OB->NNUM==7 ){
		//		int top=GameInterface::GetTZoneIndex(OB->RealX>>4,OB->RealY>>4);
		//		// найти U_Creep
		//		RPG_U_Creep* UC=NULL;
		//		int n=Creeps.GetAmount();
		//		for(int i=0;i<n;i++){
		//			RPG_U_Creep* Crp=Creeps[i];
		//			if(Crp){
		//              // ищем топологию
		//				int nt=Crp->Tops.GetAmount();
		//				for(int t=0;t<nt;t++){
		//					int Top=Crp->Tops[t];
		//					if(top==Top){
		//						UC=Crp;
		//						break;
		//					}
		//					int nl=GameInterface::GetTZoneNLinks(0,Top);
		//					for(int l=0;l<nl;l++){
		//						TLinkInfo* LI=GameInterface::GetTZoneLink(0,Top,l);
		//						Top=
		//					}
		//				}
		//			}
		//		}
		//	}
		//}		
		//scan_key(mSoldier,top,Soldier* sEnemy){
		//	int top=*key;
		//}scan_end;
		int nt=GameInterface::GetTZonesAmount(0);
		ClonesArray<pUnitsList> UnitsInTop;
		UnitsInTop.Add(NULL,nt);
		//DynArray<word> CreepIndex;
		CreepIndex.Clear();
		CreepIndex.Add(0xFFFF,nt);

		itr_GetUnitsOfNation.Create(7);
		while(OneObject* OB=itr_GetUnitsOfNation.Next()){
			if( OB && OB->NNUM==7 ){
				int top=GameInterface::GetTZoneIndex(0,OB->RealX>>4,OB->RealY>>4);
				CreepIndex[top]=0xFFFE;
				pUnitsList* UL=UnitsInTop[top];
				if(!UL){
					UL=new pUnitsList;
					UnitsInTop[top]=UL;
				}
				pUnit u(OB);
				UL->_add(u);
			}
		}
		int curID=0;
		for(int i=0;i<nt;i++){
			word cID=CreepIndex[i];
			if(cID==0xFFFE){
				cID=curID++;
				SetTopMap(CreepIndex,i,cID);
				RPG_U_Creep* UC=new RPG_U_Creep;
				Creeps.Add(UC);
			}
			if(cID!=0xFFFF){
				RPG_U_Creep* UC=Creeps[cID];
				UC->Tops.Add(i);
				UC->Units+=*(UnitsInTop[i]);
			}
		}
	}
	//if(NATIONS[OB->NNUM].AI_Enabled){
	//	ai_HeroBase* ai=AI.Get();
	//	if(!ai){
	//		ai=new ai_Hero;
	//		AI.Set(ai);
	//	}
	//	if(ai){
	//		ai->Process(this);
	//	}
	//}
	int n=Group.GetAmount();
	for(int i=0;i<n;i++){
		//OneObject* DetectHero();
		//GameInterface
		//OneObject* OB=Group[i]->Unit.object();
		RPG_AI_Object* O=Group[i];
		if( O && !O->Unit.isStuned() ){
			HeroVariableStorage* HVS=O->Unit.DetectHero();
			if( O->RPG ){
				//O->Process();
				O->AI.Process(HVS);
			}else
				O->AI.Process(HVS);
		}
	}
};
void RPG_AI::OnAIStarted(byte NI){
	itr_GetUnitsOfNation.Create(NI);
	while(OneObject* OB=itr_GetUnitsOfNation.Next()){
		if( OB->GetLiveState()==ls_LiveBody ){
			pUnit U(OB);
			AddUnit(U);
		}
	}
};
void RPG_AI::AddUnit(/*OneObject* OB*/pUnit& U){
	//	if( OB->GetLiveState()==ls_LiveBody ){
	//		pUnit U(OB);
	if( U.isHero() ){
		bool find=false;
		int n=Group.GetAmount();
		for(int i=0;i<n;i++){
			RPG_AI_Object* aiO=Group[i];
			if( aiO && aiO->Unit==U ){
				find=true;
				break;
			}
		}
		if(!find){
			RPG_AI_Object* aiO=new RPG_AI_Object;
			aiO->Unit=U;
			aiO->RPG=true;
			Group.Add(aiO);
		}
	}
	//	}
};

void RPG_AI_Group::Process(){
	//int n=GetAmount();
	//RPG_AI_Object	
}

void RPG_AI_Object::Process(){

	// not move
	//Dressing();
	//Drinking();

	ScriptCards();
	ScriptClothes();
	ScriptBottles();

	//Reflexion();
	//ActivateAbility();
	//ActivateSkill();

	// move
	//GiveBonus();		// - активаци€ бонуса (move)
	//GiveChest();		// - подн€тие предмета (move)
	//BuySettlement();	// - покупка сел (move)
	//Casting();			// - применить карточку (move)
	//Creeping();			// - крипинг - атака нейтральных юнитов (move)
	//Fighting();			// - вступаем в бой с противником или нан€тым отр€дом из села (move)
	//Escaping();			// - отступаем или заманиваем противника (move)
	//Shopping();			// - покупка/продажа: бутылки, предмета, карточки (move)

	StateProcess();
}

void RPG_AI_Object::StateProcess(){
	State.Clear();
	StateScript();	
	int n=State.GetAmount();
	if(n>0){
		// сортировка
		for(int i=0;i<n;i++){
			int Prio=-1000000;
			int maxid=i;
			for(int j=i;j<n;j++){
				RPG_AI_ObjectState* S=State[i];
				if( Prio<S->Priority ){
					maxid=j;
					Prio=S->Priority;
				}
			}
			if( maxid!=i ){
				State.Move(maxid,i);
			}
		}
		// отработка состо€ни€
		for(int i=0;i<n;i++){
			if( State[i]->Process(this) )
				break;
		}
		
	}
};

void RPG_AI_Object::AddStatePrio(ObjState OS,int Prio){
	int n=State.GetAmount();
	int i;
	for(i=0;i<n;i++){
		RPG_AI_ObjectState* S=State[i];
		if( S->State==OS ){
			S->Priority+=Prio;
		}
	}
	if( i==n ){
		RPG_AI_ObjectState* S=new RPG_AI_ObjectState;
		S->Priority=Prio;
		State.Add(S);
	}
};
bool AOS_GiveBonus::Process(RPG_AI_Object* Obj){
	return true;
};
bool AOS_GiveChest::Process(RPG_AI_Object* Obj){
	return true;
};
bool AOS_BuySettlement::Process(RPG_AI_Object* Obj){
	return true;
};
bool AOS_Casting::Process(RPG_AI_Object* Obj){
	return true;
};

word SendToTarget(pUnit Unit, int TargetTop, DynArray<short>& DangerMap, DynArray<word>& TargetMap, int& Distance){
	return 0xFFFF;
}

bool AOS_Creeping::Process(RPG_AI_Object* Obj){
	iVector2D vU=Obj->Unit.GetCoor();
	int top=GameInterface::GetTZoneIndex(0,vU.x,vU.y);

	int nt=GameInterface::GetTZonesAmount(0);

	int maxdang;
	DynArray<short> DANG(0,nt);
	short* Dang=DANG.GetValues();
	word* IDS=RPG.CreepIndex.GetValues();
	word DST;

	int zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,100,&DST);
	if( zf!=0xFFFF ){
		RPG_U_Creep* Crp=RPG.Creeps[DST];
		if(Crp){
			int n=Crp->Tops.GetAmount();
			if(n>0){
				int dist=GameInterface::NORMA(0,top,Crp->Tops[0]);
				for(int i=1;i<n;i++){
					int d=GameInterface::NORMA(0,top,Crp->Tops[i]);
					if(dist>d){
						dist=d;
					}
				}
				if( dist>700 ){
					// идем
				}else{
					// крипимс€
				}
				return true;
			}
		}
	}
	return false;
};
bool AOS_Fighting::Process(RPG_AI_Object* Obj){
	return true;
};
bool AOS_Escaping::Process(RPG_AI_Object* Obj){
	return true;
};
bool AOS_Shopping::Process(RPG_AI_Object* Obj){
	return true;
};

void RPG_AI_Object::StateScript(){

	AddStatePrio(osCreeping,15);

	// если мало места в рюкзаке бежать в магазин
	AddStatePrio(osShopping,10);
	// если нужно баночки бежать в магазин и хватает денег
	AddStatePrio(osShopping,10);

};

void AI_Obj_Set::Change(UnitAbility* UA,int Value){
	if(UA==NULL)
		return;
	AI_Obj_SetPoint* SetPoint=NULL;
	int n=GetAmount();
	for(int i=0;i<n;i++){
		AI_Obj_SetPoint* SP=(*this)[i];
		if(SP){
			if(SP->Ability.Get()==UA){
				SetPoint=SP;
			}
		}
	}
	if(!SetPoint){
		SetPoint=new AI_Obj_SetPoint(UA);
		Add(SetPoint);
	}
	SetPoint->ChangePriority(Value);
};

void AI_Obj_Set::Sort(){
};

int AI_Obj_Set::Get(ActiveUnitAbility* AA){
	UnitAbility* UA=AA->GetA();
	int n=GetAmount();
	for(int i=0;i<n;i++){
		AI_Obj_SetPoint* SP=(*this)[i];
		if(SP){
			if(SP->Ability.Get()==UA){
				return SP->Priority;
			}
		}
	}
	return NULL;
};

#define DeclareUA(x) static UnitAbility* x=GameInterface::GetUnitAbility(#x);

void RPG_AI_Object::ScriptCards(){
};

class ActiveSet: public BaseClass {
public:
	DynArray<ActiveUnitAbility*> AA;
	DynArray<int> Prio;
};

void RPG_AI_Object::ScriptClothes(){
	DeclareUA(Armor1);
	DeclareUA(Armor2);
	DeclareUA(Armor3);
	DeclareUA(Armor4);
	DeclareUA(Armor5);
	DeclareUA(Armor6);
	DeclareUA(Armor7);
	DeclareUA(Armor8);
	DeclareUA(Belt1);
	DeclareUA(Belt2);
	DeclareUA(Belt3);
	DeclareUA(Belt4);
	DeclareUA(Belt5);
	DeclareUA(Belt6);
	DeclareUA(Belt7);
	DeclareUA(Belt8);
	DeclareUA(Boots1);
	DeclareUA(Boots2);
	DeclareUA(Boots3);
	DeclareUA(Boots4);
	DeclareUA(Boots5);
	DeclareUA(Boots6);
	DeclareUA(Boots7);
	DeclareUA(Boots8);
	DeclareUA(Bracelet1);
	DeclareUA(Bracelet10);
	DeclareUA(Bracelet11);
	DeclareUA(Bracelet12);
	DeclareUA(Bracelet13);
	DeclareUA(Bracelet14);
	DeclareUA(Bracelet15);
	DeclareUA(Bracelet16);
	DeclareUA(Bracelet2);
	DeclareUA(Bracelet3);
	DeclareUA(Bracelet4);
	DeclareUA(Bracelet5);
	DeclareUA(Bracelet6);
	DeclareUA(Bracelet7);
	DeclareUA(Bracelet8);
	DeclareUA(Bracelet9);
	DeclareUA(Cloak1);
	DeclareUA(Cloak2);
	DeclareUA(Cloak3);
	DeclareUA(Cloak4);
	DeclareUA(Cloak5);
	DeclareUA(Cloak6);
	DeclareUA(Cloak7);
	DeclareUA(Cloak8);
	DeclareUA(Gloves1);
	DeclareUA(Gloves2);
	DeclareUA(Gloves3);
	DeclareUA(Gloves4);
	DeclareUA(Gloves5);
	DeclareUA(Gloves6);
	DeclareUA(Gloves7);
	DeclareUA(Gloves8);
	DeclareUA(Helm1);
	DeclareUA(Helm2);
	DeclareUA(Helm3);
	DeclareUA(Helm4);
	DeclareUA(Helm5);
	DeclareUA(Helm6);
	DeclareUA(Helm7);
	DeclareUA(Helm8);
	DeclareUA(Medalion1);
	DeclareUA(Medalion2);
	DeclareUA(Medalion3);
	DeclareUA(Medalion4);
	DeclareUA(Medalion5);
	DeclareUA(Medalion6);
	DeclareUA(Medalion7);
	DeclareUA(Medalion8);
	DeclareUA(Ring1);
	DeclareUA(Ring10);
	DeclareUA(Ring11);
	DeclareUA(Ring12);
	DeclareUA(Ring13);
	DeclareUA(Ring14);
	DeclareUA(Ring15);
	DeclareUA(Ring16);
	DeclareUA(Ring17);
	DeclareUA(Ring18);
	DeclareUA(Ring19);
	DeclareUA(Ring2);
	DeclareUA(Ring20);
	DeclareUA(Ring21);
	DeclareUA(Ring22);
	DeclareUA(Ring23);
	DeclareUA(Ring24);
	DeclareUA(Ring25);
	DeclareUA(Ring26);
	DeclareUA(Ring27);
	DeclareUA(Ring28);
	DeclareUA(Ring29);
	DeclareUA(Ring3);
	DeclareUA(Ring30);
	DeclareUA(Ring30_aAuraOfFire1);
	DeclareUA(Ring31);
	DeclareUA(Ring31_ability_soul);
	DeclareUA(Ring31_ability_soul2hero);
	DeclareUA(Ring32);
	DeclareUA(Ring4);
	DeclareUA(Ring5);
	DeclareUA(Ring6);
	DeclareUA(Ring7);
	DeclareUA(Ring8);
	DeclareUA(Ring9);

	Clothes.Clear();

	//Clothes.Change(Armor1,5);
	Clothes.Change(Armor2,5);
	Clothes.Change(Armor3,10);
	Clothes.Change(Armor4,15);
	Clothes.Change(Armor5,20);
	Clothes.Change(Armor6,25);
	Clothes.Change(Armor7,30);
	Clothes.Change(Armor8,35);

	//Clothes.Sort;

	ClonesArray<ActiveSet> ClothesTypes;
	ClothesTypes.Add(NULL,12);

	itr_AA.Create(Unit.object());
	while(ActiveUnitAbility* aa=itr_AA.Next()){
		UnitAbility* ua=aa->GetA();
		if( aa->TypeAbil==4 ){
			int t=ua->TypeClothe;
			ActiveSet* CT=ClothesTypes[t];
			if(!CT){
				CT=new ActiveSet;
				ClothesTypes[t]=CT;
			}
			int id=-1;
			int prio=Clothes.Get(aa);
			int n=CT->Prio.GetAmount();
			for(int i=0;i<n;i++){
				if(prio>CT->Prio[i]){
					id=i;
					break;
				}
			}
			if(id!=-1){
				CT->AA.Insert(id,aa);
				CT->Prio.Insert(id,prio);
			}else{
				CT->AA.Add(aa);
				CT->Prio.Add(prio);
			}
		}
	}
	int nt=ClothesTypes.GetAmount();
	for(int t=0;t<nt;t++){
		ActiveSet* CT=ClothesTypes[t];
		if(CT){
			int a=GameInterface::GetClothesTypeCapacity(t);
			int n=CT->AA.GetAmount();
			for(int i=n-1;i>=0;i--){
				ActiveUnitAbility* AA=CT->AA[i];
				if( i>=a || CT->Prio[i]<0 ){
					if( AA->Clothed ){
						AA->SetClothe(false,-1);
						//AA->Droped=true;
					}
				}else{
					if( !AA->Clothed )
						AA->SetClothe(true,-1);
				}
			}
		}
	}

};

void RPG_AI_Object::ScriptBottles(){
};

void RPG_AI_Object::GiveBonus(){}; // - активаци€ бонуса (move)
void RPG_AI_Object::GiveChest(){}; // - подн€тие предмета (move)

void RPG_AI_Object::BuySettlement(){}; // - покупка сел (move)

void RPG_AI_Object::Casting(){}; // - применить карточку (move)
void RPG_AI_Object::Creeping(){

};
void RPG_AI_Object::Fighting(){}; // - вступаем в бой с противником или нан€тым отр€дом из села (move)
void RPG_AI_Object::Escaping(){}; // - отступаем или заманиваем противника (move)

void RPG_AI_Object::Shopping(){};	// - покупка: бутылки, предмета, карточки (move)
// - продажа (offtake): бутылки, предмета, карточки (move)

void RPG_AI_Object::Dressing(){
	ScriptCards();
	ScriptClothes();
	ScriptBottles();
}; // - одеть/сн€ть пердмет ()
void RPG_AI_Object::Drinking(){}; // - выпить бутылку ()

void RPG_AI_Object::Reflexion(){}; // - включить/выключить ауру ()

void RPG_AI_Object::ActivateAbility(){}; // - включить бег или уникальные способности ()
void RPG_AI_Object::ActivateSkill(){}; // - переключить тип атаки ()

//==================================================================================================================//

//CEXPORT bool GiveHeroOrder( byte NI, bool& Retreat, int& MoveArea ){
//	int n=HeroesCollector.HeroID.GetAmount();
//	for(int i=0;i<n;i++){
//		OneObject* Hero=Group[HeroesCollector.HeroID[i]];
//		if( Hero && !Hero->Sdoxlo && Hero->Serial==HeroesCollector.HeroSN[i] && Hero->NNUM==NI ){
//			HeroVariableStorage* Herosima=DetectHero(Hero);
//			if(Herosima){
//				ai_Hero* HAI=(ai_Hero*)Herosima->AI.Get();
//				if(HAI){
//					Retreat=HAI->Retreat;
//					MoveArea=HAI->MoveArea;
//					return true;
//				}
//			}
//		}
//	}
//	return false;
//}

int ai_Hero::CollectArmy(int BaseArea, word* ArmyMap, int ArmyID){
	int dang=Danger[BaseArea];
	if( dang && ArmyMap[BaseArea]==0xFFFF ){

		ArmyMap[BaseArea]=ArmyID;

		//Area* AR=GetTopMap(BaseArea);
		//int NL=AR->NLinks;
		int NL=GameInterface::GetTZoneNLinks(0,BaseArea);
		for(int p=0;p<NL;p++){
			TLinkInfo* TI=GameInterface::GetTZoneLink(0,BaseArea,p);
			if(TI){
				int id=TI->NextAreaID;
				dang+=CollectArmy(id,ArmyMap,ArmyID);
			}
		}
		return dang;

	}
	return 0;
};

void ai_Hero::CreateDangerMap(OneObject* Hero/*, int ItrAmount*/){

	memset(Danger.Values,0,Danger.NValues*sizeof(int));
	memset(TopList.Values,0xFF,TopList.NValues*sizeof(word));

	//byte MASK=NATIONS[Hero->NNUM].NMask;	

	//byte KillMask=Hero->newMons->KillMask;
	//for(int wp=0;wp<NAttTypes;wp++)
	//	KillMask |= Hero->newMons->AttackMask[wp];

	HomeArea=0xFFFF;

	itr_GetAllUnits.Create();
	//for(int i=0;i<MAXOBJECT;i++){
	while(OneObject* OB=itr_GetAllUnits.Next()){
		//OneObject* OB=Group[i];
		if( /*OB &&*/ !OB->Sdoxlo ){
			int z=GameInterface::GetTZoneIndex(0,OB->RealX>>4,OB->RealY>>4);
			if(z<Danger.GetAmount()){
				int DD=int(Danger[z]);
				if( !(OB->NMask&Hero->NMask) ){

					// DangerMap
					//if(OB->NewBuilding){
					//	AdvCharacter* ADC=OB->MoreCharacter;
					//	if(ADC->MaxDam){
					//		int Fear=16;//FearArray[OB->newMons->Category]*16;
					//		DD+=Fear;
					//		Area* AR=GetTopMap(z);
					//		int NL=AR->NLinks;							
					//		for(int p=0;p<NL;p++){
					//			int zl=AR->Link[p].NextAreaID;
					//			Danger[zl]+=Fear;
					//		}
					//	}else{
					//		//DD-=FearArray[OB->newMons->Category]*4;
					//	}
					//}else{
					//	if(OB->newMons->ResConcentrator){
					//		DD-=32;//FearArray[OB->newMons->Category]*32;
					//	}else
					//	if(OB->newMons->Peasant){						
					//		//DD=FearArray[OB->newMons->Category];
					//	}else
					//		DD+=2;//FearArray[OB->newMons->Category];
					//}

					if(OB->newMons->KillMask&Hero->MathMask){
						for(int wp=0;wp<NAttTypes;wp++){
							int dam=OB->MoreCharacter->MaxDamage[wp];
							if( dam && OB->newMons->AttackMask[wp]&Hero->MathMask ){
								if(OB->NNUM==7){
									if(MultiplayerForCriping>1)
										DD += 0;//1;
									else
										DD += OB->Life*dam;
								}else{
									DD += OB->Life*dam;
								}
								break;
							}
						}
					}

					Danger[z]=DD;

					// TopList
					//if(TopList[z]==0xFFFF){
					//	if( (OB->MathMask&Hero->newMons->KillMask) && !(OB->newMons->Immortal) ){
					//		if(!OB->NewBuilding)
					//			TopList[z]=i;
					//	};
					//}

				}else{
					if( HomeArea==0xFFFF && OB->NewBuilding ){
						HomeArea=z;
					}
				}
			}
		};
	};

	// заполн€ем массив армий
	//int MaxSize=GetNAreas();
	int MaxSize=GameInterface::GetTZonesAmount(0);

	memset(ArmyMap,0xFF,sizeof ArmyMap);

	TopArmies.FastClear();

	int A=0;
	for(int i=0;i<MaxSize;i++){

		// заполн€ем массив армий
		int dang=CollectArmy(i,ArmyMap,A);
		if(dang){
			ai_HeroArmyInfo GI;
			GI.Force=dang;
			TopArmies.Add(GI);
			A++;
		}

		// заполн€ем карту целей
		int a=ArmyMap[i];
		if(a!=0xFFFF){
			int force=TopArmies[a].Force;
			if( force>200 && force<hForce /*&& force>hForce/128*/ ){
				TopList[i]=a;
			}
			Danger[i]=force/128;
		}
	}

	// удал€ем сильные армии
	//for(j=0;j<MaxSize;j++){
	//	if(Danger[j]>hForce)
	//		TopList[j]=0xFFFF;
	//}

	//SmoothDangerMap(Danger.Values, ItrAmount);
	//for(int j=0;j<Danger.NValues;j++){
	//	int DD=Danger[j];
	//	if(DD>30000) DD=30000;
	//	else if(DD<0) DD=0;
	//	Danger[j]=DD;
	//}
};

//void ai_Hero::CreateTopList(OneObject* Hero){
//	//byte NMask=NATIONS[NI].NMask;
//	int NA=GetNAreas();
//	memset(IDS,0xFF,NA*2);
//	for(int i=0;i<MAXOBJECT;i++){
//		OneObject* OB=Group[i];
//		if(OB&&OB->NewBuilding&&(!(OB->newMons->Immortal||OB->Sdoxlo||OB->InFire||OB->NMask&Hero->NMask||OB->NNUM==7))){
//			int top=GameInterface::GetTZoneIndex(0,OB->RealX>>4,OB->RealY>>4);
//			if(top<NA)IDS[top]=i;
//		};
//	};
//};

void ai_Hero::Process(HeroVariableStorage* HVS){

	if(!HVS)
		return;

	int TrueTime=GameInterface::GetGlobalTime();

	MultiplayerForCriping=1;
	if( HVS->Level>10 ) MultiplayerForCriping=2;
	//if( HVS->Level>20 && hForce>1000000 ) MultiplayerForCriping=2;
	//if(HVS->Level>30) MultiplayerForCriping=4;

	//int NZ=GetNAreas();
	int NZ=GameInterface::GetTZonesAmount(0);
	Danger.Check(NZ);
	Danger.NValues=NZ;
	TopList.Check(NZ);
	TopList.NValues=NZ;

	OneObject* Hero=HVS->OB;

	int x=HVS->OB->RealX>>4;
	int y=HVS->OB->RealY>>4;
	int z=GameInterface::GetTZoneIndex(0,x,y);

	MoveArea=z;

	// поиск вражеских армий
	if(DangerTime<=TrueTime){
		DangerTime=TrueTime+3000+GetRND(2000);
		CreateDangerMap(Hero);
	}

	// ближний бой
	hForce = Hero->Life*Hero->MoreCharacter->MaxDam;
	//if(Hero->Life*4<Hero->MaxLife)
	//	hForce/=4;
	if( HVS->Level<5 && Hero->Mana>75 ){
		hForce = hForce*5/2;
	}

	//eN=0;
	int eForce=0;
	OneObject* enemy=NULL;
	int enemyR=10000;
	OneObject* chest=NULL;

	itr_UnitsInRadius.Create(x,y,900);
	while(OneObject* OB=itr_UnitsInRadius.Next()){
		if( !OB->Sdoxlo && OB!=Hero){

			int r=GetTopDist(x,y,OB->RealX>>4,OB->RealY>>4)*64;
			if(r<1000){

				pUnit U(OB);
				// баночки и сундучки
				if( !chest && U.DetectChestNotEmpty() ){
					chest=OB;
				}

				// юниты
				if(OB->NMask&Hero->NMask){
					hForce += OB->Life*OB->MoreCharacter->MaxDam;
				}else{
					if( OB->newMons->KillMask&Hero->MathMask ){
						for(int wp=0;wp<NAttTypes;wp++){
							int dam=OB->MoreCharacter->MaxDamage[wp];
							if( dam && OB->newMons->AttackMask[wp]&Hero->MathMask ){

								if( !OB->AI_Guard||OB->Attack )
									eForce += OB->Life*dam;

								if(enemyR>r){
									int z=GameInterface::GetTZoneIndex(0,OB->RealX>>4,OB->RealY>>4);
									if(z<NZ){
										int a=ArmyMap[z];
										if(a!=0xFFFF){
											int force=TopArmies[a].Force;
											if( force>0 && force<hForce ){
												enemy=OB;
												enemyR=r;
											}
										}
									}
								}

								break;
							}
						}
					}
				}

			}

		}
	}

	Retreat=false;

	if( eForce==0 && !enemy ){

		// подходим	
		//if(!Hero->LocalOrder){
		if( SendTime<=TrueTime || !Hero->LocalOrder ){
			SendTime=TrueTime+4000+GetRND(1000);

			if(chest){

				// вз€ть шмотку
				//if( !Hero->LocalOrder || Hero->LocalOrder->DoLink!=StartDialogLink ){
				//	StartDialog(Hero,0,chest);
				//}

			}else{

				// идем
				int maxdang;
				word DST=0xFFFF;
				int dz=FindNextZoneOnTheSafeWayToObjectEx(z,Danger.Values,TopList.Values,&maxdang,3,&DST);
				int dx,dy;
				if( DST!=0xFFFF && GetTopZRealCoor(dz,&dx,&dy) ){
					Hero->NewMonsterSendTo(dx<<4,dy<<4,128+16,0);

					MoveArea=dz;
				}

			}
		}

	}else
		if( hForce>eForce && LifePrev<=Hero->Life ){
			// деремс€

			// кастуем
			bool Casting=false;

			int x,y,z;
			if(enemy){
				x=enemy->RealX>>4;
				y=enemy->RealY>>4;
				z=GameInterface::GetHeight(x,y);
				MoveArea=GameInterface::GetTZoneIndex(0,x,y);
			}

			if( CastTime<=TrueTime && (/*eForce>2000 ||*/ TrueTime-CastTime>12000) ){	// если враг в 2 раза слабее - не примен€ть магию

				ActiveUnitAbility* Spell=NULL;

				itr_AA.Create(Hero);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if(AA->Casting){
						Casting=true;
						break;
					}
					if( enemy && !Spell ){
						UnitAbility* UA=AA->GetA();
						if( AA->TypeAbil==1 && UA && !UA->Aura && UA->EnemyTarget ){
							if(AA->CanTarget(enemy->Index,x,y,z)){
								Spell=AA;
							};
						}
					}
				}
				if( !Casting && Spell ){
					CastTime=TrueTime+6000+GetRND(6000);
					Spell->Execute(enemy->Index,x,y,z);
					Casting=true;
				}
			}

			// рукопрекладствуем
			if( !(Casting||Hero->Attack) ){
				if(enemy){
					Hero->AttackObj(enemy->Index,128+16);
				}			
			}

		}else{

			if(LifePrev<=Hero->Life){
				Retreat=true;
			}

			// отступаем
			//if( !Hero->LocalOrder || Hero->Attack ){
			if( SendTime<=TrueTime || Hero->Attack ){
				SendTime=TrueTime+3000+GetRND(1000);
				int maxdang;
				word DST=0xFFFF;
				int dz=FindNextZoneOnTheSafeWayEx(z,HomeArea,Danger.Values,&maxdang,3);
				if(dz=0xFFFF){
					dz=HomeArea;
				}
				int dx,dy;
				if(GetTopZRealCoor(dz,&dx,&dy)){
					Hero->NewMonsterSendTo(dx<<4,dy<<4,128+16,0);

					MoveArea=dz;
				}
			}

		}

		bool upShPirc=false;
		int UpShPircType=0;
		if( HVS->Level>10 && GetRND(100)<45 ){
			upShPirc=true;
			if(Hero->MoreCharacter->Protection[0]>10){
				int minProt=1000;
				for(int i=0;i<4;i++){
					int pr=Hero->MoreCharacter->Protection[i];
					if(minProt>pr){
						minProt=pr;
						UpShPircType=i;
					}
				}
			}
		}

		bool upRange=false;
		if( HVS->Level>6 && GetRND(100)<60 ){
			upRange=true;
		}

		// карточка вызова рабочих
		UnitAbility* pUA=peonUA.Get();
		if(!peonInit){
			peonInit=true;
			char* an="MC_aSummonPeasants";

			//int m = Abilities.Abilities.GetAmount();
			//for(int j=0;j<m;j++){
			//	UnitAbility* UA=Abilities.Abilities[j];
			itr_AA.Create(Hero);
			while(ActiveUnitAbility* AA=itr_AA.Next()){
				UnitAbility* UA=AA->GetA();
				if( UA && strstr(UA->Name.pchar(),an) ){
					peonUA.Set(UA);
					pUA=UA;
					break;
				}
			}

		}

		itr_AA.Create(Hero);
		while(ActiveUnitAbility* AA=itr_AA.Next()){

			// прокачка
			if(AA->Type==8){
				ChooseUpHeroParam* UHP=(ChooseUpHeroParam*)AA;
				if(UHP->UpIndex!=-1){
					if(!upShPirc){
						if(!upRange){
							UpAttack* UpAt=dynamic_cast<UpAttack*>(HVS->Hero->UpHeroParams[UHP->UpIndex]);
							if(UpAt){
								UHP->OnClick();
								upLife=false;
								break;
							}
							if(!upLife){
								UpLife* UpLf=dynamic_cast<UpLife*>(HVS->Hero->UpHeroParams[UHP->UpIndex]);
								if(UpLf){
									UHP->OnClick();
									upLife=true;
									break;
								}
							}
						}
						UpRange* UpRg=dynamic_cast<UpRange*>(HVS->Hero->UpHeroParams[UHP->UpIndex]);
						if(UpRg){
							UHP->OnClick();
							break;
						}
					}
					UpShield* UpShPirc=dynamic_cast<UpShield*>(HVS->Hero->UpHeroParams[UHP->UpIndex]);
					if(UpShPirc&&UpShPirc->AttackType==UpShPircType){
						UHP->OnClick();
						break;
					}
					LetPass* UpLp=dynamic_cast<LetPass*>(HVS->Hero->UpHeroParams[UHP->UpIndex]);
					if(UpLp){
						UHP->OnClick();
						break;
					}
				}
			}

			if( !AA->Casting && pUA && AA->GetA()==pUA ){
				if(AA->Execute(0xFFFF,0,0,0)){
					//pUA=false;
				};
			}

			// одеть шмотки
			if(AA->TypeAbil==4){
				//OneItem* I=(OneItem*)AA;
				ActiveUnitAbility* I=(ActiveUnitAbility*)AA;
				UnitAbility* UA=AA->GetA();
				if( !(UA&&UA->CanNotBeClothe) )
					I->Clothed=true;
			}

			// пьем баночки
			if( BottleTime<=TrueTime && AA->TypeAbil==5 ){
				bool click=false;

				if( Hero->Life<50 ){
					if(GameInterface::CheckInfluenceMask(AA,ABL_Life)){
						click=true;
					}
				}
				if( Hero->Life*2 <= Hero->MaxLife ){
					if(GameInterface::CheckInfluenceMask(AA,ABL_LifeRegeneration)){
						click=true;
					}
				}
				if( Hero->Mana+150 <= Hero->MaxMana ){
					if(GameInterface::CheckInfluenceMask(AA,ABL_Mana)){
						click=true;
					}
					if(GameInterface::CheckInfluenceMask(AA,ABL_ManaRegeneration)){
						click=true;
					}
				}
				if( hForce>eForce ){
					// нападаем
					if(eForce){
						if(GameInterface::CheckInfluenceMask(AA,ABL_Damage)){
							click=true;
						}
						if(GameInterface::CheckInfluenceMask(AA,ABL_Shield)){
							click=true;
						}
						if(GameInterface::CheckInfluenceMask(AA,ABL_Protection)){
							click=true;
						}
					}				
				}else{
					// отступаем
					if(Hero->Life*4 <= Hero->MaxLife){
						if(GameInterface::CheckInfluenceMask(AA,ABL_MotionSpeed)){
							click=true;
						}
						if(GameInterface::CheckInfluenceMask(AA,ABL_Invisibility)){
							click=true;
						}
						if(GameInterface::CheckInfluenceMask(AA,ABL_Invulnerability)){
							click=true;
						}
					}
				}
				if(GameInterface::CheckInfluenceMask(AA,ABL_LifeMax)){
					click=true;
				}
				if(GameInterface::CheckInfluenceMask(AA,ABL_ManaMax)){
					click=true;
				}

				if(click){
					BottleTime=TrueTime+4000+GetRND(8000);
					AA->OnClick();
					break;
				}

			}

		}

		LifePrev=Hero->Life;

};
//==================================================================================================================//