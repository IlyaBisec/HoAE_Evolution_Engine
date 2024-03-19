#include "stdheader.h"
#include "unitability.h"
#include "UnitAbility2.h"
#include "Abilities.h"
#include "GameExtension.h"
#include "UnitAbilityExtension.h"
#include "UnitTypeGroup.h"
#include ".\cvi_singleplayerdata.h"
#include "ua_Item.h"
#include "ua_Ability.h"
#include "ua_Modificators.h"
#include "ua_Settlement.h"
#include "ComplexShooter.h"

#include "UnitsInterface.h"
#include "ITerrain.h"

#include ".\cvi_MainMenu.h"
#include ".\cvi_HeroButtons.h"

#include "Camera\Navigator.h"
#include "IWater.h"

#include "MassivePlayer.h"

//
CEXPORT char* GetResourceName(byte ResID);
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
extern int TrueTime;
extern CHeroesCollector HeroesCollector;
extern AbilityList Abilities;
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
void MarkObjectLikeModified(OneObject* OB);
//==================================================================================================================//
DWORD ActiveUnitAbility::CurSerial=0;
//

DWORD fnRecreateAllHeroEnum(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int n=UnitTypeGroups.Groups.GetAmount();
	if( PrevCallHash!=n ){
		PrevCallHash=n;
		UnitTypeGroups.CreateAllHeroEnum(out);
	}
	return PrevCallHash;
}
void RegisterUnitTypeGroup(){
	REG_CLASS(TypeGroup);
	REG_CLASS(TypeGroupList);
	REG_CLASS(UnitTypeGroup);
	Enumerator* E=ENUM.Get("AllHeroEnum");
	E->SetProcEnum(new RefreshableEnumerator(fnRecreateAllHeroEnum));
}

void UnitAbilityExtension::OnClassRegistration(){

	//REG_CLASS(be_CSO_REF);
	//REG_CLASS(be_CMSG_REF);
    
	//REG_CLASS(ua_VillageSetOwner);
	//REG_CLASS(ua_VillageBuildUnits);

	REG_CLASS(ClassRef<UnitAbility>);
	REG_CLASS(ClassRef<NewAbility>);
	REG_CLASS(ClassRef<NewItem>);

	REG_CLASS(MoneyAward);

	REG_CLASS(ActiveAbilityListArray);
	//============ Modificators ===================	
	REG_CLASS(UnitModificatorCondition);
	REG_CLASS(UnitModificatorConditionArray);
	REG_CLASS(UnitModificator);
	REG_CLASS(UnitModificatorArray);
	REG_CLASS(ModifySprite);
	//
	REG_CLASS(umc_Complex);
	REG_CLASS(umc_Invisibility);
	REG_CLASS(umc_DirectionDifference);
	REG_CLASS(umc_Ready);
	REG_CLASS(umc_HeroAtHome);
	REG_CLASS(umc_TimePeriod);
	REG_CLASS(umc_MotionStyle);
	REG_CLASS(umc_UnitType);
	REG_CLASS(umc_IsMoving);
	REG_CLASS(umc_OverWater);
	REG_CLASS(umc_CasterUnitType);
	REG_CLASS(umc_Caster);
	REG_CLASS(umc_CasterLevel);
	REG_CLASS(umc_FriendlyToCaster);
	REG_CLASS(umc_Summon);
	REG_CLASS(umc_Frozen);
	REG_CLASS(umc_Incorporeal);
	REG_CLASS(umc_UpgradeIsDone);
	REG_CLASS(umc_LandTypeInPoint);
	REG_CLASS(umc_Hero);
	REG_CLASS(umc_Building);
	REG_CLASS(umc_Hidden);
	REG_CLASS(umc_LiveBody);
	REG_CLASS(umc_LockType);
	REG_CLASS(umc_LifeLess);
	REG_CLASS(umc_AttackRadiusMax);
	REG_CLASS(umc_WeaponKind);
	REG_CLASS(umc_FindTargetNearCaster);
	//
	REG_CLASS(UnitModificatorComplex);

	REG_CLASS(mod_Death);
	REG_CLASS(mod_DamReduct);
	REG_CLASS(mod_Vampire);	
	REG_CLASS(mod_Thorn);
	REG_CLASS(mod_Alteration);

	REG_CLASS(mod_Vision);
	REG_CLASS(mod_Material);
	REG_CLASS(mod_Position);
	REG_CLASS(mod_OverEarth);
	REG_CLASS(mod_Fear);
	REG_CLASS(mod_Damage);
	REG_CLASS(mod_MotionSpeed);
	REG_CLASS(mod_AttackRadiusMax);
	REG_CLASS(mod_Silence);
	REG_CLASS(mod_SkillSilence);
	REG_CLASS(mod_Compassion);
	REG_CLASS(mod_Invulnerability);
	
	REG_CLASS(mod_Protection);
	REG_CLASS(pOperand);
	REG_CLASS(cProtections);
	REG_CLASS(mod_IgnoreProtection);

	REG_CLASS(mod_Shield);
	REG_CLASS(mod_AttackRate);
	REG_CLASS(mod_MagicImmunity);
	REG_CLASS(mod_SkillImmunity);
	REG_CLASS(mod_LifeRegeneration);
	REG_CLASS(mod_ManaRegeneration);
	REG_CLASS(mod_Weapon);
	REG_CLASS(mod_Invisibility);
	REG_CLASS(mod_Summon);
	//REG_CLASS(mod_DamagerLife);	
	REG_CLASS(mod_ManaValue);
	REG_CLASS(mod_Life);
	REG_CLASS(mod_Mana);
	REG_CLASS(mod_LifeMax);
	REG_CLASS(mod_ManaMax);
	REG_CLASS(mod_AttackCaster);
	REG_CLASS(mod_Plymorph);
    REG_CLASS(mod_ProduceEnable);
	REG_CLASS(mod_Scale);

	REG_CLASS(mod_Control);
	REG_CLASS(mod_NMask);
	REG_CLASS(mod_Insubordination);

	REG_CLASS(mod_ModelAnimation);
	REG_CLASS(mod_LevelForEffects);
	REG_CLASS(mod_TradeSkill);
	REG_CLASS(mod_Learning);
	REG_CLASS(mod_WeaponKind);
	REG_CLASS(mod_Evasion);
	REG_CLASS(mod_Miss);
	REG_CLASS(mod_ExtraAnimation);
	REG_CLASS(mod_ItemPickUp);
	REG_CLASS(mod_ExtraAbility);
	REG_CLASS(mod_DeleteAbility);
	REG_CLASS(mod_SearchVictim);
	REG_CLASS(mod_Symbol);
	REG_CLASS(mod_MotionStyle);
	REG_CLASS(mod_LockType);
	REG_CLASS(mod_CenterScreen);

	REG_CLASS(mod_SpellOnAttack);
	REG_CLASS(mod_DeleteAbilityOnReceiveDamage);
	REG_CLASS(mod_KillMask);
	REG_CLASS(mod_DeleteAbilityAfterNShots);

	// Button
	REG_CLASS(ua_Button);
	// TargetSelector
	REG_CLASS(ua_TargetSelector);
	// TimeLine
	REG_CLASS(ua_TimeLine);
	REG_CLASS(ua_Track);
	REG_CLASS(uat_UnitAnimation);
	
	REG_CLASS(uat_VisualEffect);


	REG_CLASS(uat_Modificator);
	REG_CLASS(VisualEffectArray);
	// Event
	REG_CLASS(ua_Event);
	REG_CLASS(uae_LeftClick);
	REG_CLASS(uae_RightClick);
	REG_CLASS(uae_TimelineIsDone);
	// Operation
	REG_CLASS(ua_Operation);
	REG_CLASS(uao_ChangeState);
	REG_CLASS(uao_Disappear);
	REG_CLASS(uao_ObjectDie);
	// State
	REG_CLASS(ua_State);
	REG_CLASS(ua_Impulse);	
	//
	REG_CLASS(ref_NewItem);
	//
	REG_CLASS_EX(ua_Chest,"New");
	REG_CLASS_EX(uaa_Chest,"New");
	REG_CLASS_EX(ua_DropPackage,"New");
	REG_CLASS_EX(uaa_DropPackage,"New");
	
	REG_CLASS_EX(ua_Image,"New");
	REG_CLASS_EX(ua_Effect,"New");

	REG_CLASS_EX(NewBonus,"New");
	REG_CLASS_EX(NewAbility,"New");
	REG_CLASS_EX(NewItem,"New");
	REG_CLASS_EX(NewItemTransformer,"New");	
	REG_CLASS_EX(NewMagazine,"New");
	REG_CLASS_EX(MagicCard,"New");

	REG_CLASS(ua_Cookie);

	REG_CLASS(MDState);
	REG_CLASS(ChangeMDAbiliti);
	REG_CLASS(ChangeMDAbilitiActive);

	//==============================================================//
	//=================     UnitAbility        =====================//
	//==============================================================//

	REG_CLASS(UA_Lazy);
	REG_CLASS(AA_Lazy);

	REG_CLASS(ua_AtHome);
	REG_CLASS(aa_AtHome);

	REG_CLASS(ua_Settlement);
	REG_CLASS(uaa_Settlement);
	REG_CLASS(ua_BeMsg);
	REG_CLASS(uaa_BeMsg);

	REG_CLASS(ActiveUnitAbility);
	REG_CLASS(OneMagazine);
	REG_CLASS(OneBonus);
	REG_CLASS(uaa_Chest);
	REG_CLASS(OneItem);
	REG_CLASS(OneItemTransformer);
	REG_CLASS(uaa_DropPackage);

	REG_CLASS(UnitAbility);
	REG_CLASS(UnitAbility2);
	REG_CLASS(BaseClassUnitType);
	REG_CLASS(UnitTypeList);
	REG_CLASS(UnitAbilityAura);
	REG_CLASS(UnitAbilityMagicEffect);
	REG_CLASS(VampiricAbility);
	REG_CLASS(AbilityList);
	REG_CLASS(MonsterAbility);
	
	REG_CLASS(UnitActiveAbilityList);
	REG_CLASS(CoolDown);
	REG_CLASS(UnitsComposition);
	REG_CLASS(OneUnitInComposition);
	REG_CLASS(LeadSeveralUnitsAbility);
	REG_CLASS(BeSlaveOfUnit);
	REG_CLASS(CannonAutoShot);
	REG_CLASS(CannonAutoShotActive);
	REG_CLASS(PushUnitsAbility);
	REG_CLASS(PushAllOnTheWay);
	REG_CLASS(BlizardAbility);
	REG_CLASS(ActiveBlizardAbility);
	REG_CLASS(LeveledActiveUnitAbility);
	REG_CLASS(LifeLimitation);
	REG_CLASS(ActiveLifeLimitation);
	REG_CLASS(GreetingMe);
	REG_CLASS(ActiveGreetingMe);
	REG_CLASS(ActiveGreeting);
	REG_CLASS(NationBonus);
	REG_CLASS(ActiveNationBonus);
	REG_CLASS(BrigadeChangeMoral);
	REG_CLASS(ActiveBrigadeChangeMoral);
	REG_CLASS(ActiveChangedMoralForBrigade);
	REG_CLASS(BrigadeTimeChangeMoral);
	REG_CLASS(ActiveBrigadeTimeChangeMoral);
	REG_CLASS(ActiveTimeChangedMoralForBrigade);
	REG_CLASS(BrigadeIconInfo);
	REG_CLASS(HeroLight);
	REG_CLASS(ActiveHeroLight);
	REG_CLASS(LifeRegeneration);
	REG_CLASS(ActiveLifeRegeneration);
	REG_CLASS(AddBrigadeBonus);
	REG_CLASS(ActiveAddBrigadeBonus);
	REG_CLASS(ActiveBrigadeBonus);
	REG_CLASS(SetMineBonus);
	REG_CLASS(ActiveSetMineBonus);
	REG_CLASS(ActiveMineBonus);
	REG_CLASS(IncreaseMaxLife);
	REG_CLASS(ActiveAdditionalLife);
	REG_CLASS(ActiveIncreaseMaxLife);
	REG_CLASS(BuildingShield);
	REG_CLASS(ActiveAddBuildingShield);
	REG_CLASS(ActiveBuildingShield);
	REG_CLASS(BeSlowNearUnits);
	REG_CLASS(aa_BeSlowNearUnits);
	REG_CLASS(FollowBrigade);
	REG_CLASS(ActiveFollowBrigade);
	REG_CLASS(Behaviour);
	REG_CLASS(ActiveBehaviour);
	REG_CLASS(RomeHero);
	REG_CLASS(ActiveRomeHero);
	REG_CLASS(LeveledActiveUnitAbility);
	REG_CLASS(BeSlowNearUnits);
	REG_CLASS(MakeDamageOnDeath);
	REG_CLASS(aa_MakeDamageOnDeath);

	REG_CLASS(BigUnit);
	REG_CLASS(ActiveBigUnit);
	REG_CLASS(AddUnitBonus);
	REG_CLASS(ActiveAddUnitBonus);
	REG_CLASS(PassiveAddUnitBonus);
	REG_CLASS(AggressiveMove);
	REG_CLASS(ActiveAggressiveMove);
	REG_CLASS(AggressiveSplashFire);
	REG_CLASS(ActiveAggressiveSplashFire);
	REG_CLASS(PermanentLandTypeChange);
	REG_CLASS(ActivePermanentLandTypeChange);
	REG_CLASS(ExtraShootingElement);
	REG_CLASS(ActiveExtraShootingElement);
    REG_CLASS(_3D_shooter_simulator);
	REG_CLASS(Active_3D_shooter_simulator);
	REG_CLASS(suAI_Ability);
	REG_CLASS(suAI_Abilities);
	REG_CLASS(ua_SingleUnitAI);
	REG_CLASS(aa_SingleUnitAI);
    void reg_uaFlyBodyParams();
    reg_uaFlyBodyParams();
	void RegBuilClass();
	RegBuilClass();
	//==============================================================//
	//=================     HeroAbility        =====================//
	//==============================================================//
	REG_CLASS(Probability);
	REG_CLASS(UpHeroParam);
	REG_CLASS(ChooseUpHeroParam);
	REG_CLASS(UpHeroParamList);
	REG_CLASS(HeroAbility);
	REG_CLASS(UpLife);
	REG_CLASS(UpMana);
	REG_CLASS(LetPass);
	REG_CLASS(HeroVariableStorage);
	REG_CLASS(UpAttack);
	REG_CLASS(UpVision);
	REG_CLASS(UpRange);
	REG_CLASS(UpAttackSpeed);
	REG_CLASS(UpMotionSpeed);
	REG_CLASS(UpShield);
	REG_CLASS(UpLifeRegeneration);
	REG_CLASS(UpManaRegeneration);
	REG_CLASS(UpLevelParam);
	REG_CLASS(UpSearchEnemyRadius);
	REG_CLASS(UpVariable);
	REG_CLASS(HeroVariable);
//	REG_CLASS(CardPlace);
//	REG_CLASS(ActiveCardPlace);
	//==============================================================//
	//=================     HeroAbility        =====================//
	//==============================================================//
	REG_CLASS(CBrigadeOrder);
	REG_CLASS(BrigadeOrder_RifleAttack);
	REG_CLASS(BrigadeOrder_GoOnRoad);
	REG_CLASS(BrigadeOrder_Bitva);
	REG_CLASS(BrigadeOrder_HumanEscape);
	REG_CLASS(BrigadeOrder_KeepPositions);
	REG_CLASS(BrigadeOrder_HumanGlobalSendTo);
	//==============================================================//
	//=================         Other          =====================//
	//==============================================================//
	RegisterUnitTypeGroup();

	REG_CLASS(SinglePlayerData_HeroAbilityInfo);
	REG_CLASS(SinglePlayerData_HeroInfo);
	REG_CLASS(SinglePlayerData_HeroesInfoList);
	REG_CLASS(SinglePlayerData_CampaignHeroInfo);
	//
	REG_CLASS(cvi_SinglePlayerData);
	//==============================================================//
	//=================       MagicCard        =====================//
	//==============================================================//	
	REG_CLASS(MagicSpell);
	REG_CLASS(LifeCast);
	REG_CLASS(AttSpeedCast);
	REG_CLASS(MoveSpeedCast);
	REG_CLASS(ShieldCast);
}
void UnitAbilityExtension::OnUnloading()
{
	for(int i=0;i<8;i++){
		CampaignKillerAward[i]=0;
		for(int j=0;j<8;j++)
			SkirmishKillerAward[i][j]=0;
	}

	ActiveUnitAbility::CurSerial=0;
	for(i=0;i<MAXOBJECT;i++)
	{
		OneObject* OB=Group[i];
		if(OB)
		{
			if(OB->ActiveAbility)
			{
				delete (OB->ActiveAbility);
				OB->ActiveAbility=NULL;
			}
		}
	};
	HeroesCollector.Reset();
}

bool SaveCharacterChangeToXml(OneObject* OB,xmlQuote& xml);
void LoadCharacterFromXml(OneObject* OB,xmlQuote& xml);
BasicObjectChars* CreateCharacterFromNewMonster(NewMonster* NM);

bool SaveActiveAbilitiesToXml(OneObject* OB,xmlQuote& xml){
	if(OB->ActiveAbility){

		// ActiveAbilities
		xmlQuote* aa=new_xmlQuote("ActiveAbilities");
		OB->ActiveAbility->Save(*aa,OB->ActiveAbility);
		xml.AddSubQuote(aa);

		// AbilitiesName
		ClonesArray<_str> AbilitiesName;
		itr_AA.Create(OB);
		while(ActiveUnitAbility* AA=itr_AA.Next()){

			_str* Name = new _str;

			UnitAbility* UA=AA->GetA();
			if(UA){
				(*Name)=UA->Name;
			}else{
				(*Name)="ChooseUpHeroParam";
			}

			AbilitiesName.Add(Name);

		}
		xmlQuote* an=new_xmlQuote("AbilitiesName");
		AbilitiesName.Save(*an,&AbilitiesName);
		xml.AddSubQuote(an);

		// MoreCharacter
		if( OB->ActiveAbility->BornNM && OB->ActiveAbility->BornNM->CharacterWasModified && OB->ActiveAbility->BornAC!=OB->MoreCharacter ){
			NewMonster* NM=OB->ActiveAbility->BornNM;
			
			xmlQuote* my_id=new_xmlQuote("MyIndex");
			my_id->Assign_int(NM->MyIndex);
			xml.AddSubQuote(my_id);

			xmlQuote* mc=new_xmlQuote("BasicObjectChars");
			NewMonster* cNM=OB->newMons;
			AdvCharacter* cAC=OB->MoreCharacter;
			OB->newMons=OB->ActiveAbility->BornNM;
			OB->MoreCharacter=OB->ActiveAbility->BornAC;
            xmlQuote* mci=new_xmlQuote();

			//SaveCharacterChangeToXml(OB,*mci);			
			BasicObjectChars* BOC=CreateCharacterFromNewMonster(OB->newMons);
			mci->SetQuoteName(BOC->GetClassName());
			BOC->Save(*mci,BOC,NULL);
			delete(BOC);
			
			mc->AddSubQuote(mci);
			OB->newMons=cNM;
			OB->MoreCharacter=cAC;
			xml.AddSubQuote(mc);
			
		}

		return true;

	}
	return false;
}
Enumerator* AbilEnum=NULL;
Enumerator* SkipList=NULL;
Enumerator* ExVocab=NULL;
void CreateAbilEnum(){
	if(!AbilEnum){
		AbilEnum=ENUM.Get("ABILENUM");
		int N=Abilities.Abilities.GetAmount();
		for(int i=0;i<N;i++){
            AbilEnum->Add(Abilities.Abilities[i]->Name.pchar());
		}
		SkipList=ENUM.Get("ABILSKIPLIST");
		SkipList->Add("Hint");
		SkipList->Add("SpriteID");
		SkipList->Add("FileID");

		ExVocab=ENUM.Get("ABILEXVOCAB");
		ExVocab->Add("ActiveAbilities");
		ExVocab->Add("HeroVariableStorage");
		ExVocab->Add("OneAbility");
		ExVocab->Add("OneItem");
	}
}

bool SaveActiveAbilitiesToBinStream(OneObject* OB,BinStream* bs){
	if(OB->ActiveAbility){		
		CreateAbilEnum();
		itr_AA.Create(OB);
		int N=0;
		while(ActiveUnitAbility* AA=itr_AA.Next())N++;		
		bs->WriteWORD(N+1);
		OB->ActiveAbility->SaveBin(OB->ActiveAbility,NULL,bs,ExVocab,SkipList);		
		itr_AA.Create(OB);		
		bs->WriteBYTE(132);
		while(ActiveUnitAbility* AA=itr_AA.Next()){			
			UnitAbility* UA=AA->GetA();
			if(UA){
				int i=AbilEnum->Get(UA->Name.pchar());
				bs->WriteWORD(i);
			}else{
				bs->WriteWORD(0xFFFF);
			}
		}
		return true;
	}else bs->WriteWORD(0);
	return false;
}
bool LoadActiveAbilitiesFromBinStream(OneObject* OB,BinStream* BS){
    int N=BS->ReadWORD();
	if(N){
		N--;
		CreateAbilEnum();
		// ActiveAbilities
		if(!OB->ActiveAbility){
			OB->ActiveAbility = new UnitActiveAbilityList(OB);
		}
		UnitActiveAbilityList UAA_List;
		ActiveAbilityListArray& UnitAbilities=UAA_List.ActiveAbilities;
		ClonesArray<_str> AbilitiesName;
		
		BaseClass* BC=&UAA_List;
		BC->reset_class(BC);
		BC->LoadBin(BC,NULL,BS,ExVocab);					

		byte X=BS->ReadBYTE();

		for(int i=0;i<N;i++){
            _str* s=new _str;
			WORD id=BS->ReadWORD();
			if(id==0xFFFF)*s="ChooseUpHeroParam";
			else *s=AbilEnum->GetStr(id);
			AbilitiesName.Add(s);
		}
		// добавл€ем шмотки и востанавливаем HeroVariableStorage
		if(OB->ActiveAbility){
			// ¬останавливаем UnitActiveAbilityList
			UAA_List.Copy(OB->ActiveAbility);			
			OB->ActiveAbility->ActiveAbilities.reset_class(&OB->ActiveAbility->ActiveAbilities);
			OB->ActiveAbility->ModelAnimationNIndex=OB->ActiveAbility->BornNIndex;
		}

		DynArray<int> HeroUpIndex;

		int n=UnitAbilities.GetAmount();
		for(int i=0;i<n;i++){
			ActiveUnitAbility* A=UnitAbilities[i];
			if( A ){
				A->OB=OB;
				if( i<AbilitiesName.GetAmount() ){
					char* aName=AbilitiesName[i]->pchar();
					if(aName){
						int m = Abilities.Abilities.GetAmount();
						for(int j=0;j<m;j++){
							UnitAbility* UA=Abilities.Abilities[j];
							if( UA && UA->Name==aName ){
								ActiveUnitAbility* newA=UA->GetNewActiveAbility();
								newA->OB=OB;
								newA->TypeAbil=A->TypeAbil;
								OB->ActiveAbility->ActiveAbilities.Add(newA);
								A->Copy(newA);
								break;
							}else if(strcmp(aName,"ChooseUpHeroParam")==0){
								ChooseUpHeroParam* Ch=(ChooseUpHeroParam*)A;
								HeroUpIndex.Add(Ch->UpIndex);
								break;
							}
						}
					}
				}
			}
		}
		OB->ActiveAbility->ActiveAbilities.ResetInfluenceMask();

		// UpHeroParams
		HeroVariableStorage* HVS=DetectHero(OB);
		if(HVS){
			int n=HeroUpIndex.GetAmount();
			for(int i=0;i<n;i++)
				HVS->AddChooseUpHeroParam(HeroUpIndex[i]);
		}		
	}
	return true;
}
bool LoadActiveAbilitiesFromXml(OneObject* OB,xmlQuote& xml){

	// ActiveAbilities
	if(!OB->ActiveAbility){
		OB->ActiveAbility = new UnitActiveAbilityList(OB);
	}

	//// удал€ем старые абилки
	//HeroVariableStorage* HV=NULL;
	////ActiveAbilityListArray& UnitAbilities=OB->ActiveAbility->ActiveAbilities;
	//int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
	//for(int i=0;i<n;i++){
	//	ActiveUnitAbility* A=OB->ActiveAbility->ActiveAbilities[i];
	//	if( A ){
	//		//HeroVariableStorage* HVS=dynamic_cast<HeroVariableStorage*>(A);
	//		//if( HVS && !HV ){
	//		//	HV=HVS;
	//		//}else{
	//			OB->ActiveAbility->ActiveAbilities.DelElement(i);
	//			i--;
	//			n--;
	//		//}
	//	}
	//}

	UnitActiveAbilityList UAA_List;
	ActiveAbilityListArray& UnitAbilities=UAA_List.ActiveAbilities;
	ClonesArray<_str> AbilitiesName;

	ErrorPager Error;
	xmlQuote* xmlUA=xml.GetSubQuote("ActiveAbilities");
	if(xmlUA){
		BaseClass* BC=&UAA_List;
		BC->reset_class(BC);
		BC->Load(*xmlUA,BC,&Error);
	}		
	xmlQuote* xmlAN=xml.GetSubQuote("AbilitiesName");
	if(xmlAN){
		BaseClass* BC=&AbilitiesName;
		BC->reset_class(BC);
		BC->Load(*xmlAN,BC,&Error);
	}

	xmlQuote* xmlMI=xml.GetSubQuote("MyIndex");
	xmlQuote* xmlBOC=xml.GetSubQuote("BasicObjectChars");
	if( xmlMI && xmlBOC ){
		NewMonster* cNM=OB->newMons;
        AdvCharacter* cAC=OB->MoreCharacter;
		word cMyIndex=OB->newMons->MyIndex;
		OB->newMons->MyIndex=xmlMI->Get_int();
        //GeneralObject* GO=NATIONS[OB->NNUM].Mon[OB->NIndex];
        //OB->newMons=GO->newMons;
        //OB->MoreCharacter=GO->MoreCharacter;
        LoadCharacterFromXml(OB,*(xmlBOC->GetSubQuote(0)));
        OB->ActiveAbility->BornNM=OB->newMons;
        OB->ActiveAbility->BornAC=OB->MoreCharacter;
        OB->newMons=cNM;
        OB->MoreCharacter=cAC;
		OB->newMons->MyIndex=cMyIndex;
    }else{
		//OB->ActiveAbility->BornNM=OB->newMons;
		//OB->ActiveAbility->BornAC=OB->MoreCharacter;
	}

	// добавл€ем шмотки и востанавливаем HeroVariableStorage
	if(OB->ActiveAbility){
		// ¬останавливаем UnitActiveAbilityList
		UAA_List.Copy(OB->ActiveAbility);
		//OB->ActiveAbility->reset_class(OB->ActiveAbility);
		OB->ActiveAbility->ActiveAbilities.reset_class(&OB->ActiveAbility->ActiveAbilities);
		//OB->ActiveAbility->Init();
		//OB->ActiveAbility->OB=OB;
		//OB->ActiveAbility->SetBornNIndex();
		//OB->ActiveAbility->TransparencyStartTime=TrueTime;
		OB->ActiveAbility->ModelAnimationNIndex=OB->ActiveAbility->BornNIndex;
	}
	
	DynArray<int> HeroUpIndex;

	int n=UnitAbilities.GetAmount();
	for(int i=0;i<n;i++){
		ActiveUnitAbility* A=UnitAbilities[i];
		if( A ){
			A->OB=OB;
			//HeroVariableStorage* HVS=dynamic_cast<HeroVariableStorage*>(A);
			//if(HVS){
			//	if(HV){
			//		HV->Level=HVS->Level;
			//		HV->Experience=HVS->Experience;
			//		HV->UpLifeRegeneration=HVS->UpLifeRegeneration;
			//		HV->UpManaRegeneration=HVS->UpManaRegeneration;
			//		HV->ExperienceToNextLevel=HVS->ExperienceToNextLevel;
			//	}else{
			//		HV = new HeroVariableStorage;
			//		HVS->Copy(HV);
			//		AddActiveUnitAbility(OB->Index,HV);
			//	}
			//}else
			if( /*A->TypeAbil!=0 &&*/ /*A->TypeAbil!=7 &&*/ i<AbilitiesName.GetAmount() ){
				char* aName=AbilitiesName[i]->pchar();
				if(aName){
					int m = Abilities.Abilities.GetAmount();
					for(int j=0;j<m;j++){
						UnitAbility* UA=Abilities.Abilities[j];
						if( UA && UA->Name==aName ){
							ActiveUnitAbility* newA=UA->GetNewActiveAbility();
							newA->OB=OB;
							newA->TypeAbil=A->TypeAbil;
							//UA->OnAddToObject(OB);
							//AddActiveUnitAbility(OB->Index,newA);
							OB->ActiveAbility->ActiveAbilities.Add(newA);
							A->Copy(newA);
							break;
						}else if(strcmp(aName,"ChooseUpHeroParam")==0){
							ChooseUpHeroParam* Ch=(ChooseUpHeroParam*)A;
							HeroUpIndex.Add(Ch->UpIndex);
							break;
						}
					}
				}
			}
		}
	}
	OB->ActiveAbility->ActiveAbilities.ResetInfluenceMask();
	
	// UpHeroParams
	HeroVariableStorage* HVS=DetectHero(OB);
	if(HVS){
		int n=HeroUpIndex.GetAmount();
		for(int i=0;i<n;i++)
			HVS->AddChooseUpHeroParam(HeroUpIndex[i]);
	}

	//// ¬останавливаем UnitActiveAbilityList
	//UnitAbilities.Clear();
	//UAA_List.Copy(OB->ActiveAbility);

	return true;
}
void SaveAllChangedCharactersToXml(xmlQuote& xml);
void LoadAllCharactersFromXml(xmlQuote& xml);
bool UnitAbilityExtension::OnGameSaving(xmlQuote& xml){

    xmlQuote* ccxml = new_xmlQuote("ChangedCharacters");
    SaveAllChangedCharactersToXml(*ccxml);
    xml.AddSubQuote(ccxml);

	xmlQuote* cur_sn = new_xmlQuote("CurSerial");
	cur_sn->Assign_int(ActiveUnitAbility::CurSerial);
	xml.AddSubQuote(cur_sn);
	
	//Log.Message("Begin Save Abilities");
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if( OB && OB->ActiveAbility && OB->ActiveAbility->ActiveAbilities.GetAmount() ){
			xmlQuote* ob_id = new_xmlQuote("Index");
			ob_id->Assign_int(OB->Index);
			xml.AddSubQuote(ob_id);
			xmlQuote* ob_ua = new_xmlQuote("Abilities");
			SaveActiveAbilitiesToXml(OB,*ob_ua);
			xml.AddSubQuote(ob_ua);
			//Log.Message("ID: %d, Name: %s",OB->Index,OB->newMons->Message);
		}
	};
	return true;
}
bool UnitAbilityExtension::OnGameLoading(xmlQuote& xml){
	
	xmlQuote* cur_sn = xml.GetSubQuote("CurSerial");
	int cs=cur_sn->Get_int();   

	xmlQuote* ccxml=xml.GetSubQuote("ChangedCharacters");
    LoadAllCharactersFromXml(*ccxml);

	//Log.Message("Begin Load Abilities");
	int i=0;
	int n=xml.GetNSubQuotes();
	for(int x=2;x<n;x+=2){
		xmlQuote* ob_id = xml.GetSubQuote(x);
		int id=ob_id->Get_int();
		while( i<MAXOBJECT ){
			OneObject* OB=Group[i];
			i++;
			if(OB){
				if( OB->Index==id ){
					xmlQuote* ob_ua = xml.GetSubQuote(x+1);
					LoadActiveAbilitiesFromXml(OB,*ob_ua);
					if(OB->ActiveAbility){
						word CLife=OB->MaxLife;						
						OB->ActiveAbility->ActiveAbilities.modifyLifeMax(OB->MaxLife,CLife);
						int DL=CLife-OB->MaxLife;
						CLife=OB->MaxLife-DL;
						//if(OB->MaxLife)OB->Life=OB->Life*CLife/OB->MaxLife;
						if(OB->newMons->CharacterWasModified)OB->newMons->Life=CLife;
						if(OB->MoreCharacter->Allocated)OB->MoreCharacter->Life=CLife;
					}
					//Log.Message("ID: %d, Name: %s",OB->Index,OB->newMons->Message);
					break;
				}else{
					// сбросить абилки юнитам
					if(OB->ActiveAbility){
						OB->ActiveAbility->ActiveAbilities.reset_class(&OB->ActiveAbility->ActiveAbilities);
					}
				}
			}
		}
	}

	ActiveUnitAbility::CurSerial=cs;
    

	return true;
}
//
bool DisableInvisibility(OneObject* OB);
DLLEXPORT int GetRND(int Max);

word DrawHealthID=0xFFFF;
int DrawHealthCount=0;

//
bool UnitAbilityExtension::OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType){

	// DamagerLife modificator
	if(Damager){
		if(AttType<128){
			if( Damager->ActiveAbility ){
				if( DamagedUnit && !DamagedUnit->NewBuilding )
					Damager->ActiveAbility->ActiveAbilities.modifyVampire(Damager->Life,Damager->Life,Damage,Damager,DamagedUnit);

				int Miss=0;
				Damager->ActiveAbility->ActiveAbilities.modifyMiss(0,Miss);
				if(Miss>90) Miss=90;

				if(Miss>GetRND(100)){
					return false;
				}
				// 0 - Receive Damage, 1 - Make Damage
				//0 - Owner, 1 - Enemy
				Damager->ActiveAbility->ActiveAbilities.execSpellOnAttack(DamagedUnit,1,0);
				Damager->ActiveAbility->ActiveAbilities.execSpellOnAttack(DamagedUnit,1,1);
				//// Invisible
				//Damager->ActiveAbility->ActiveAbilities.InvisibleEnableTime=TrueTime+1000;
			}
		}

		if(Damager->NNUM==0){
			DrawHealthID=DamagedUnit->Index;
			DrawHealthCount=16;
		}

		if( Damager->ActiveAbility ){
			UnitActiveAbilityList* AAL=Damager->ActiveAbility;
			AAL->NShots++;
			if( AAL->ActiveAbilities.InfluenceMask.check_bit(ABL_DeleteAbilityAfterNShots) ){
				itr_AA.Create(Damager);
				while( ActiveUnitAbility* AA=itr_AA.Next() ){
					if( AA->DeleteShot && AA->DeleteShot<AAL->NShots ){
						AA->Deleted=true;
					}
				}
			}
		}
	}

	if( DamagedUnit->ActiveAbility && AttType<128 ){
		int Evasion=0;
		DamagedUnit->ActiveAbility->ActiveAbilities.modifyEvasion(0,Evasion);
		if(Evasion>90) Evasion=90;

		if(Evasion>GetRND(100)){
			return false;
		}

		DamagedUnit->ActiveAbility->ActiveAbilities.execDeleteAbilityOnReceiveDamage();
		DamagedUnit->ActiveAbility->ActiveAbilities.execSpellOnAttack(Damager,0,0);
		DamagedUnit->ActiveAbility->ActiveAbilities.execSpellOnAttack(Damager,0,1);
	}
	if( DamagedUnit->ActiveAbility 
				&& DamagedUnit->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Thorn) ){
				if (Damager){
					DamagedUnit->ActiveAbility->ActiveAbilities.modifyThorn(Damager->Life,Damager->Life,Damage,Damager,DamagedUnit,AttType);
				}
				}//Thorn, fixed, TheBlackHunter, 1.07.22
	// Stop invisibility if not dead
	//bool Death=false;
	//Damager->ActiveAbility->ActiveAbilities.modifyDeath(Death,Death);
	// if(!Death){
							//bool Death=false;
					        //OB->ActiveAbility->ActiveAbilities.modifyDeath(Death,Death);
				if(Damager){
				if(Damager->ActiveAbility){
					if (!(Damager->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Death))){
		                       DisableInvisibility(Damager);
				}
				}
				}//deathfix, TheBlackHunter, 30.06.2022
	//} ѕереработать эти куски кода.

	//if( DamagedUnit ){
	//	if( DamagedUnit->newMons->NewParams.VulnerableOnlyForAntimage ){
	//	}
	//}

	return true;
};
bool UnitAbilityExtension::OnAttemptToAttack(OneObject* Attacker,OneObject* Victim){
	if( Attacker && Victim ){
		// Stop invisibility
		//DisableInvisibility(Attacker);
		if( Victim->newMons->NewParams.VulnerableOnlyForAntimage ){
			if( Victim->Life==Victim->MaxLife && !Attacker->newMons->NewParams.AntiMage )
				return false;
		}
	}
	return true;
};
ActiveUnitAbility* GetDropAbility(OneObject* OB, UnitAbility* &UA){
	
	// hack begin
	bool bonus=false;
	OneBonus* OBon=DetectBonus(OB);
	if(OBon){
		NewBonus* NB=(NewBonus*)OBon->GetA();
		bonus=(NB->Name=="Bonus Sunduk");
	}
	// hack end

	DynArray<ActiveUnitAbility*> Items;
	DynArray<int> Values;
	int curVal=0;
	itr_AA.Create(OB);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		if( AA->TypeAbil!=0 && (AA->ItemDrop>0||bonus) ){

			UnitAbility* UA=AA->GetA();
			if( UA && !UA->System ){
				// не дропать магазины, бонусы и сундуки
				OneMagazine* M=dynamic_cast<OneMagazine*>(AA);
				if(M) continue;
				OneBonus* B=dynamic_cast<OneBonus*>(AA);
				if(B) continue;
				uaa_Chest* C=dynamic_cast<uaa_Chest*>(AA);
				if(C) continue;

				// hack begin
				if(AA->ItemDrop<=0)
					AA->ItemDrop=10;
				// hack end

				Items.Add(AA);
				curVal+=AA->ItemDrop;
				Values.Add(curVal);
			}

		}
	}
	if(curVal){
		curVal=rando()%curVal;
		for(int i=0;i<Items.GetAmount();i++){
			if(curVal<Values[i]){
				ActiveUnitAbility* A=Items[i];
				if(A){
					UA=A->GetA();
					uaa_DropPackage* aDP=dynamic_cast<uaa_DropPackage*>(A);
					if(aDP){
						ua_DropPackage* uDP=(ua_DropPackage*)aDP->GetA();
						int N=uDP->DropPackage.GetAmount();
						if(N>0){
							int i=0;
							float m=32768.0/N;
							do{
								int rnd=float(rando())/m;
								addrand(rnd);
								ClassRef<NewAbility>* rDP=uDP->DropPackage[rnd];
								if(rDP){
									UA=rDP->Get();
								}
								i++;
							}while(!UA&&i<N);
						}
					}
					return A;
				}
			}
		}
	}
	return NULL;
}

ActiveUnitAbility* GetPickUpAbility(OneObject* OB);
extern word NPlayers;
extern word COMPSTART[8];
int ConvertNationToSkirmish(int SkirmisID);
void EraseObject(OneObject* OB);
CEXPORT char* GetPlayerName(byte NI);

bool UnitAbilityExtension::OnUnitDie(OneObject* Dead,OneObject* Killer){
	static OneObject* LastDead=NULL;
	static int LastTime=0;

	if (LastDead==Dead && LastTime==TrueTime) return true;
	LastDead=Dead;
	LastTime=TrueTime;

	// Hero killing
	HeroVariableStorage* HVS=DetectHero(Dead);
	if(HVS){
		HeroAbility* HA=(HeroAbility*)HVS->GetA();
		if(HA){
			UnitAbility* DieUA=HA->OnDieUnitAbility.Get();
			if(DieUA){
				itr_AA.Create(Dead);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					UnitAbility* UA=AA->GetA();
					if( UA==DieUA ){
						return false;
					}
				}
				char* DName=GetPlayerName(Dead->NNUM);
				HVS->Lives--;
				if(HVS->Lives>0){
					OneObject* OB=Dead;

					itr_AA.Create(OB);
					while(ActiveUnitAbility* AA=itr_AA.Next()){
						AA->AutoCast=false;
						AA->Casting=false;
						UnitAbility* UA=AA->GetA();
						if( UA && DieUA!=UA ){
							if( AA->TypeAbil==0  && !(UA->IgnoreMagicImmunity||UA->Permanent||AA->InfluenceMask.check_bit(ABL_MagicImmunity)) ){
								AA->Deleted=true;
							}
						}
					}
					HVS->Experience=0;
					OB->Sdoxlo=0;
					OB->Life=OB->MaxLife;
					OB->NewAnm=OB->newMons->GetAnimation(anm_Stand);
					OB->SetZeroFrame();

					SetActiveProducer(OB);
					int id=NATIONS[OB->NNUM].CreateNewMonsterAt(OB->RealX,OB->RealY,OB->NIndex,true,OB->RealDir);
					SetActiveProducer(NULL);
					if(id!=-1){
						OneObject* Corpse=Group[id];
						if(Corpse->ActiveAbility)
							Corpse->ActiveAbility->ActiveAbilities.Clear();
						vHeroButtons.Remove(Corpse);
						Corpse->Die();
					}

					//OB->DeleteLastOrder();

					//itr_AA.Create(OB);
					//while(ActiveUnitAbility* AA=itr_AA.Next()){
					//	UnitAbility* UA=AA->GetA();
					//	if( AA->TypeAbil==0 && UA!=DieUA ){
					//		AA->Deleted=true;
					//	}
					//}

					AddOneEffect(NULL,DieUA,Dead,Dead);

					DString txt;
					if( Killer ){
						void HeroesGatherExperience(OneObject* Victim,word Killer);
						HeroesGatherExperience(OB,Killer->Index);
					}
					if( Killer && Killer->NNUM!=7 ){
						// player
						static char* str=GetTextByID("HEROKILL");
						char* KName=GetPlayerName(Killer->NNUM);
						txt.print(str,KName,DName,HVS->Level,HVS->Lives);
					}else{
						// creeps
						static char* str=GetTextByID("KILLBYCREEPS");
						txt.print(str,DName,HVS->Level,HVS->Lives);
					}
					AssignHint1(txt.pchar(),250);
					return false;
				}else{
					DString txt;
					if( Killer && Killer->NNUM!=7 ){
						// player
						static char* str=GetTextByID("HEROKILL2");
						char* KName=GetPlayerName(Killer->NNUM);
						txt.print(str,KName,DName,HVS->Level);
					}else{
						// creeps
						static char* str=GetTextByID("KILLBYCREEPS2");
						txt.print(str,DName,HVS->Level);
					}
					AssignHint1(txt.pchar(),250);
				}
			}
		}
	}

	// ItemDrops
	if (!(HVS||DetectChest(Dead)||DetectBonus(Dead))) {
		UnitAbility* UA=NULL;
		ActiveUnitAbility* A=GetDropAbility(Dead,UA);
		if(A){
			//A->Droped=true;
			A->DropDown();
		}
	}
	if(Killer){

		// за уничтожение сундуков давать ресурс
		ActiveUnitAbility* AA=GetPickUpAbility(Dead);
		if(AA){
			UnitAbility* UA=AA->GetA();
			if(UA){
				int price = UA->ShopPrice/4;
				int price2 = price*2;

				CommonParameters* CP=&AA->OB->newMons->NewParams;
			if (!UA->EnableCustomReward){
				if( CP->PickUp_KillingAward_Gold ){
					AddXRESRC(Killer->NNUM,GoldID,CP->PickUp_KillingAward_Gold);
				}else
					AddXRESRC(Killer->NNUM,GoldID,price+GetRND(price2));
				if( CP->PickUp_KillingAward_Iron ){
					AddXRESRC(Killer->NNUM,IronID,CP->PickUp_KillingAward_Iron);
				}else
					AddXRESRC(Killer->NNUM,IronID,price+GetRND(price2));
				if( CP->PickUp_KillingAward_Coal ){
					AddXRESRC(Killer->NNUM,CoalID,CP->PickUp_KillingAward_Coal);
				}else
					AddXRESRC(Killer->NNUM,CoalID,price+GetRND(price2));
			}else{
				if(UA->FoodReward)AddXRESRC(Killer->NNUM,FoodID,UA->FoodReward+GetRND(UA->RandomFoodReward));
				if(UA->WoodReward)AddXRESRC(Killer->NNUM,TreeID,UA->WoodReward+GetRND(UA->RandomWoodReward));
				if(UA->StoneReward)AddXRESRC(Killer->NNUM,StoneID,UA->StoneReward+GetRND(UA->RandomStoneReward));
				if(UA->GoldReward)AddXRESRC(Killer->NNUM,GoldID,UA->IronReward+GetRND(UA->RandomGoldReward));
				if(UA->IronReward)AddXRESRC(Killer->NNUM,IronID,UA->GoldReward+GetRND(UA->RandomIronReward));
				if(UA->CoalReward)AddXRESRC(Killer->NNUM,CoalID,UA->CoalReward+GetRND(UA->RandomCoalReward));

			}
				EraseObject(Dead);
			}
		}
		/*	if(AA){
			UnitAbility* UA=AA->GetA();
			if(UA){
				int price = UA->ShopPrice/4;
				CommonParameters* CP=&AA->OB->newMons->NewParams;
				if( CP->PickUp_KillingAward_Gold ){
					AddXRESRC(Killer->NNUM,GoldID,CP->PickUp_KillingAward_Gold);
				}else
					AddXRESRC(Killer->NNUM,GoldID,price);
				if( CP->PickUp_KillingAward_Iron ){
					AddXRESRC(Killer->NNUM,IronID,CP->PickUp_KillingAward_Iron);
				}else
					AddXRESRC(Killer->NNUM,IronID,price);
				if( CP->PickUp_KillingAward_Coal ){
					AddXRESRC(Killer->NNUM,CoalID,CP->PickUp_KillingAward_Coal);
				}else
					AddXRESRC(Killer->NNUM,CoalID,price);
				EraseObject(Dead);
			}
		}*/
//For Reborn
		// давать награду только если киллер той же национальности что и игрок
		//bool PassKillerNat=false;
		//for(int i=0;i<NPlayers||COMPSTART[i];i++){
		//	PlayerInfo* I=GSets.CGame.PL_INFO+i;
		//	if(Killer->NNUM==I->ColorID){
		//		int KillerNatID=NATIONS->Mon[Killer->NIndex]->NatID;
		//		KillerNatID=ConvertNationToSkirmish(KillerNatID);
		//		PassKillerNat=(KillerNatID==I->NationID);
		//		break;
		//	}
		//}
		//bool Summon=false;
		//bool Control=false;
		//if(Killer->ActiveAbility){
		//	Killer->ActiveAbility->ActiveAbilities.modifySummon(Summon);
		//	Control=(Killer->NNUM!=Killer->ActiveAbility->BornNNUM);
		//}

		if( true/*PassKillerNat || Summon || Control*/ ){
			NewMonster* NM=Dead->newMons;
			for(int i=0;i<8;i++){
				int a=NM->KillerAward[i];
				int R=NM->KillerAwardRandom[i];
				if (R) a+=rando()%R;
				if(a){
					if( vGameMode==gmCamp ){
						if( EngSettings.KillerAwardPercentForCampaign ){
							int KA=CampaignKillerAward[i]+a;
							a=KA/EngSettings.KillerAwardPercentForCampaign;
							CampaignKillerAward[i]=KA%EngSettings.KillerAwardPercentForCampaign;					
						}
					}else{
						if( NM->NewParams.SkirmishAwardPercent ){
							int KA=SkirmishKillerAward[i][Killer->NNUM]+a*NM->NewParams.SkirmishAwardPercent;
							a=KA/100;
							SkirmishKillerAward[i][Killer->NNUM]=KA%100;
						}
					}
					if(a){
						AddXRESRC(Killer->NNUM,MoneyID,a);
					}
					//int x=Dead->RealX>>4;
					//int y=Dead->RealY>>4;
					//byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
					//if(NI==Killer->NNUM){
					//	char s[64];
					//	sprintf(s,"+%d %s",a,GetResourceName(MoneyID));
					//	void AddIllustrator(int x,int y,char* text,RLCFont* Font,byte Type,DWORD Color);
					//	AddIllustrator(x+100-(rand()%200),y+100-(rand()%200),s,&SmallWhiteFont,254,0xFF00FFFF);
					//}
				}
			}
		}
	}
	// Create Weapon
	if(Dead->OverEarth>-100/*!Dead->newMons->Incorporeal*/){
		word UnitIndex=Dead->Index;
		int x=Dead->RealX>>4;
		int y=Dead->RealY>>4;
		int z=Dead->RZ+Dead->OverEarth;
		
		itr_AA.Create(Dead);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			UnitAbility* UA=AA->GetA();
			if( UA && (AA->TypeAbil==0/*||AA->TypeAbil==4*/ || UA->ClassType>0) ){
				WeaponModificator* WM=UA->OnDieCreateWeapon.Get();
				if(WM){
					word FromID=UnitIndex;
					OneObject* CastOB=Group[AA->CasterID];
					if(CastOB&&CastOB->Serial==AA->CasterSN){
						FromID=AA->CasterID;
					}

					AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
					AWP->Radius=UA->Radius;
					CreateNewActiveWeapon(WM->Name.pchar(),FromID,x,y,z,UnitIndex,x,y,z,AWP);
				}
			}
		}
	}
	return true;
};

ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
extern AbilityList Abilities;

void UnitAbilityExtension::OnUnitWasProduced(OneObject* Producer,OneObject* NewObject){
	//if(Producer->NNUM==7){
	if(Producer->NMask&256){
		static UnitAbility* UA=NULL;
		static init=false;
		//UnitAbility* UA=EngSettings.SettlementUnitProduceAbility.Get();
		if( !(init||UA) ){
			init=true;
			for(int i=0;i<Abilities.Abilities.GetAmount();i++){
				if(Abilities.Abilities[i]->Name=="SettlementBalance"){
					UA=Abilities.Abilities[i];
				}
			}
		}
		if(UA){
			AddOneEffect(NULL,UA,NewObject,Producer);
		}
	}
};

void InitUnitAbilities(OneObject* OB);
bool GetHero(OneObject* OB);
bool DetectMagazine(OneObject* OB);
OneBonus* DetectBonus(OneObject* OB);
void MarkObjectLikeModified(OneObject* OB);

void UnitAbilityExtension::OnUnitBirth(OneObject* NewUnit){

	InitUnitAbilities(NewUnit);

	bool Hero=GetHero(NewUnit);
	//bool Shop=DetectMagazine(NewUnit);
	//bool Bonus=DetectBonus(NewUnit);

	if( Hero ){
		MarkObjectLikeModified(NewUnit);
	}

	//if( (Shop || Bonus) && !NewUnit->newMons->Capture ){
	//	MarkObjectLikeModified(NewUnit);
	//	NewUnit->newMons->Capture=true;
	//}

	//if(G->NNUM==7){
	//	void MarkObjectLikeModified(OneObject* OB);
	//	MarkObjectLikeModified(G);
	//}

	//bool DetectMagazine(OneObject* OB);
	//OneBonus* DetectBonus(OneObject* OB);
	//bool Mag=DetectMagazine(G);
	//bool Bon=DetectBonus(G);
	
	//G->newMons->Capture=G->NewBuilding&&(Mag||Bon);
	
	//NewUnit->newMons->CantCapture=!Hero;

};

extern UnitActiveAbilityList* v_Targ_MA;
extern int v_Targ_MA_id;
extern int v_Targ_MA_id2;

extern int LastMx;
extern int LastMy;

extern word Att;
extern word Friend;

int AFW_x, AFW_y, AFW_r, AFW_curs=-1;
DWORD AFW_color;

bool DrawSelPatch(float x,float y,int Type,float Radius,DWORD Color);

void UnitAbilityExtension::OnDrawOnMapAfterFogOfWar(){
	if(AFW_curs!=-1){
		AFW_curs=-1;

		GPS.FlushBatches();
		DrawSelPatch(AFW_x, AFW_y, AFW_curs, AFW_r, AFW_color);
		GPS.FlushBatches();
		ITerra->FlushPatches();

	}
};

void UnitAbilityExtension::OnDrawOnMapAfterLandscape(){
	if(v_Targ_MA){
		
		IRS->SetTextureFactor(0xFFFFFFFF);
        GPS.SetCurrentDiffuse(0xFFFFFFFF);

		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(SP&&SP->ActAbl==v_Targ_MA){
			ActiveUnitAbility* AA=NULL;
			for(int i=0;i<v_Targ_MA->ActiveAbilities.GetAmount();i++){
				ActiveUnitAbility* A=v_Targ_MA->ActiveAbilities[i];
				if(A&&A->Serial==v_Targ_MA_id2){
					AA=A;
					break;
				}
			}
			if(AA){
				word TID=Att!=0xFFFF?Att:Friend;	// index of target unit
				bool CanPress=AA->CanTarget(TID,LastMx,LastMy,0);
				if(!AA->DrawCursor(LastMx,LastMy)){
					if(CanPress){
						VitCursor=13;
					}else{
						VitCursor=14;
					}
				}else{
					VitCursor=15;
					ITerra->FlushPatches();
				}
			}
		}
	}
};

bool m_HeroLevelUp=false;
void HeroLevelUpEvent(){
	m_HeroLevelUp=true;
};

void PanicUnit(OneObject* OBJ);

void UnitAbilityExtension::ProcessingGame(){
	if(m_HeroLevelUp){
		m_HeroLevelUp=false;

		void AddUnlimitedEffect(int x,int y,int id);
		AddUnlimitedEffect(0,0,v_ISys.SoundLevelUp);

	}
	// Flood
	bool isFlood();
	if(isFlood()){
		int Z=IWater->GetWaterline();
		itr_GetAllUnits.Create();
		while(OneObject* OB=itr_GetAllUnits.Next()){
			eLiveState LS=OB->GetLiveState();
			if( LS==ls_LiveBody && !DetectHero(OB) ){
				int dZ=Z-(OB->RZ+OB->OverEarth);
				if(dZ>20){
					OB->Die();
					//PanicUnit(OB);
				}else if(dZ>45){
					//OB->Die();
				}else{
					PanicUnit(OB);
				}
			}else if(LS==ls_DeadBody){
				// всплываем
				OB->RZ=Z-OB->OverEarth;
			}
		}
	}
};
xmlQuote tmp;
bool UnitAbilityExtension::OnCheatEntering(const char* Cheat){
	_str Ch=Cheat;
	if(Ch=="AAGPFile"){
		AssignHint("AAGPFile start...",130);
		int n=Abilities.Abilities.GetAmount();
		for(int i=0;i<n;i++){
			//UnitAbility* UA=Abilities.Abilities[i];
			NewItem* UA=dynamic_cast<NewItem*>(Abilities.Abilities[i]);
			if(UA){
				const char* fn=GPS.GetGPName(UA->FileID);
				if(fn){
					DString str;
					str=fn;
					if(UA->Clothe){
						//str.Replace0("goods_","goods_character_");
						//UA->FileID_Char=GPS.PreLoadGPImage(str.pchar());
						//str=fn;
						//str.Replace0("goods_","goods_inventory_");
						//UA->FileID_Inv=GPS.PreLoadGPImage(str.pchar());
					}else{
						str+="_inventory";
						UA->FileID_Char=GPS.PreLoadGPImage(str.pchar());
						UA->FileID_Inv=UA->FileID_Char;
					}
				}
			}
		}
	}else if(Ch=="AACopy"){
		int n=ImNSL[MyNation];
		if(n>0){
			OneObject* OB=Group[ImSelm[MyNation][0]];
			if(OB&&OB->Serial==ImSerN[MyNation][0]){
				if( SaveActiveAbilitiesToXml(OB,tmp) ){
					AssignHint("Person copied",30);
					tmp.WriteToFile("SaveActiveAbilitiesToXml.xml");
				}else{
					AssignHint("Person not copied",130);
				}
			}
		}
		return true;
	}else if(Ch=="AAPaste"){
		int n=ImNSL[MyNation];
		if(n>0){
			OneObject* OB=Group[ImSelm[MyNation][0]];
			if(OB&&OB->Serial==ImSerN[MyNation][0]){
				LoadActiveAbilitiesFromXml(OB,tmp);
				AssignHint("Person pasted",130);
			}
		}
		return true;
	}else if(Ch=="AADir"){
		DString File;
		DString Units;
		DString Buildings;
		for(int i=0;i<8;i++){
			itr_GetUnitsOfNation.Create(i);
			while(OneObject* OB=itr_GetUnitsOfNation.Next()){
				DString str;
				str.print("%d, %s, (%d,%d). Ability: ",OB->Index,OB->Ref.General->Message,OB->RealX>>4,OB->RealY>>4);
				bool comma=false;
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if( AA->TypeAbil!=0 ){
						UnitAbility* UA=AA->GetA();
						if(UA){
							if(comma)
								str+=",";
							str+=UA->Name;
						}
					}
				}
				str+="/n";
				if(OB->NewBuilding){
					Units+=str;
				}else{
					Buildings+=str;
				}
			}
			File.print("ColorID = %d\n",i);
			File+=Units;
			File+=Buildings;
		}
		File.WriteToFile("AbilityDir.log");
		AssignHint("AbilityDir.log saved",130);
		return true;
	}
	return false;
}
//==================================================================================================================//