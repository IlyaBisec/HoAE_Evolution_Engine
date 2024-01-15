#include "stdheader.h"
#include "unitability.h"
#include "ua_Item.h"
#include "WeaponSystem.h"
#include "ua_Modificators.h"
#include "cvi_singleplayerdata.h"
#include "cvi_Campaign.h"
#include "CurrentMapOptions.h"
//#include ".\cvi_HeroButtons.h"
//==================================================================================================================//
CHeroesCollector HeroesCollector;
extern int TrueTime;
extern int AnimTime;
extern MapOptions MOptions;
extern HeroVariableStorage* CurrentHeroAbility;
extern AbilityList Abilities;
//==================================================================================================================//
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
HeroVariableStorage* GetHeroVariableStorage(OneObject* OB);
bool DrawSelPatch(float x,float y,int Type,float Radius,DWORD Color);
//==================================================================================================================//

UpHeroParam::UpHeroParam()
{
	Index=-1;
	FileID=0;
	SpriteID=0;
	Hint="";
	Special=false;
	IsInPass=false;
}
bool UpHeroParam::Realize(OneObject* OB)
{
	return false;
}
//==================================================================================================================//
UpLife::UpLife()
{
	AddMaxLife=0;
}
bool UpLife::Realize(OneObject* OB)
{
	if(OB&&AddMaxLife)
	{
		OB->MoreCharacter->Life+=AddMaxLife;
		if(OB->newMons->CharacterWasModified)OB->newMons->Life=OB->MoreCharacter->Life;

		OB->MaxLife+=AddMaxLife;
		OB->Life+=AddMaxLife; // <-----
		return true;
	}
	return false;
}

UpMana::UpMana()
{
	AddMaxMana=0;
}
bool UpMana::Realize(OneObject* OB)
{
	if(OB&&AddMaxMana)
	{  OB->MoreCharacter->MaxMana+=AddMaxMana;
		if(OB->newMons->CharacterWasModified)OB->newMons->MaxMana=OB->MoreCharacter->MaxMana;

		OB->MaxMana=OB->MoreCharacter->MaxMana;
		OB->Mana+=AddMaxMana;
		return true;
	}
	return false;
}
//==================================================================================================================//
UpAttack::UpAttack(){
	AttackAdd=0;
}
bool UpAttack::Realize(OneObject* OB){
	if(OB&&AttackAdd){
		//OB->AddDamage+=AttackAdd;
		for(int i=0;i<NAttTypes;i++){
            if(OB->MoreCharacter->MaxDamage[i]){
                OB->MoreCharacter->MaxDamage[i]+=AttackAdd;
                if(OB->newMons->CharacterWasModified)OB->newMons->MaxDamage[i]=OB->MoreCharacter->MaxDamage[i];
            }
		}
	}
	return false;
}
//==================================================================================================================//
UpVision::UpVision()
{
	VisionAdd=0;
}
bool UpVision::Realize(OneObject* OB)
{
	if(OB&&VisionAdd)
	{
		OB->MoreCharacter->VisionType+=VisionAdd;
        if(OB->newMons->CharacterWasModified)OB->newMons->VisionType=OB->MoreCharacter->VisionType;
		//if(OB->MoreCharacter->VisionType>8)
		//	OB->MoreCharacter->VisionType=8;
	}
	return false;
}
//==================================================================================================================//
UpRange::UpRange()
{
	RageAdd=0;
	AttackType=0;
}
int UpRange::GetWeight(HeroVariableStorage* HVS, int Prob){
	int R=HVS->OB->MoreCharacter->MaxR_Attack;
	if(R>150&&R<900){
		return UpHeroParam::GetWeight(HVS,Prob);
	}
	return 0;
};
bool UpRange::Realize(OneObject* OB)
{
	if(OB&&RageAdd){
		AdvCharacter* AC=OB->MoreCharacter;

		int AT=0;
		if(AC->MaxDamage[1]){
			AT=1;
		}

		AC->AttackRadius2[AT]+=RageAdd;
        if(OB->newMons->CharacterWasModified)OB->newMons->AttackRadius2[AT]=AC->AttackRadius2[AT];
		if(AC->AttackRadius2[AT]>AC->MaxR_Attack){
			AC->MaxR_Attack=AC->AttackRadius2[AT];            
		}
	}
	return false;
}
//==================================================================================================================//
UpAttackSpeed::UpAttackSpeed()
{
	SpeedAdd=0;
}
bool UpAttackSpeed::Realize(OneObject* OB)
{
	if(OB&&SpeedAdd)
	{
		OB->MoreCharacter->AttackSpeed+=SpeedAdd;        
        if(OB->newMons->CharacterWasModified)OB->newMons->AttackSpeed=OB->MoreCharacter->AttackSpeed;
	}
	return false;
}
//==================================================================================================================//
UpMotionSpeed::UpMotionSpeed()
{
	MotionSpeedAdd=0;
}
bool UpMotionSpeed::Realize(OneObject* OB)
{
	if(OB&&MotionSpeedAdd)
	{
		OB->MoreCharacter->Speed+=MotionSpeedAdd;
        if(OB->newMons->CharacterWasModified)OB->newMons->Speed=OB->MoreCharacter->Speed;

	}
	return false;
}
//==================================================================================================================//
UpShield::UpShield()
{
	ShieldAdd=0;
	AttackType=0;
}
bool UpShield::Realize(OneObject* OB)
{
	if(OB&&ShieldAdd)
	{
		OB->MoreCharacter->Protection[AttackType]+=ShieldAdd;
        if(OB->newMons->CharacterWasModified)OB->newMons->Protection[AttackType]=OB->MoreCharacter->Protection[AttackType];
	}
	return false;
}
//==================================================================================================================//
UpLifeRegeneration::UpLifeRegeneration()
{
	RegenerationAdd=0;
}
bool UpLifeRegeneration::Realize(OneObject* OB)
{
	if(RegenerationAdd&&OB)
	{
		int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(OB->ActiveAbility->ActiveAbilities[i]->Type==7)//HeroVariableStorage
			{
				HeroVariableStorage* S = (HeroVariableStorage*)OB->ActiveAbility->ActiveAbilities[i];
				S->UpLifeRegeneration+=RegenerationAdd;
				break;
			}
		}
		return true;
	}
	return false;
}
UpManaRegeneration::UpManaRegeneration(){
	RegenerationAdd=0;
}
bool UpManaRegeneration::Realize(OneObject* OB){
	if(RegenerationAdd&&OB){
		int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i=0;i<n;i++){
			if(OB->ActiveAbility->ActiveAbilities[i]->Type==7){
				HeroVariableStorage* S = (HeroVariableStorage*)OB->ActiveAbility->ActiveAbilities[i];
				S->UpManaRegeneration+=RegenerationAdd;
				break;
			}
		}
		return true;
	}
	return false;
}
//==================================================================================================================//
UpSearchEnemyRadius::UpSearchEnemyRadius()
{
	RadiusAdd=0;
}
bool UpSearchEnemyRadius::Realize(OneObject* OB)
{
	if(OB&&RadiusAdd)
	{
		OB->MoreCharacter->VisRange+=(RadiusAdd<<4);
        if(OB->newMons->CharacterWasModified)OB->newMons->VisRange=OB->MoreCharacter->VisRange;
		return true;
	}
	return false;
}
//==================================================================================================================//
UpVariable::UpVariable()
{
	VarName="";
	AddValue=0;
}
bool UpVariable::Realize(OneObject* OB)
{
	bool rez=false;
	if(OB&&AddValue)
	{
		HeroVariableStorage* HVS=GetHeroVariableStorage(OB);
		if(HVS)
		{
			int* V=HVS->GetVarRef(VarName.pchar());
			if(V)
			{
				(*V)+=AddValue;
				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
LetPass::LetPass()
{
	Special=true;
}
bool LetPass::Realize(OneObject* OB)
{
	int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(OB->ActiveAbility->ActiveAbilities[i]->Type==7)//HeroVariableStorage
		{
			HeroVariableStorage* S = (HeroVariableStorage*)OB->ActiveAbility->ActiveAbilities[i];
			S->PassSelectHeroParametr=true;
			S->SelectHeroParamState=false;
			break;
		}
	}
	return false;
}
//==================================================================================================================//
ChooseUpHeroParam::ChooseUpHeroParam()
{
	Type=8;
	TypeAbil=3;
}

bool ChooseUpHeroParam::OnClick()
{
	if( GetUnitIndex()!=0xFFFF && UpIndex!=-1 && HeroAbilityRef.Get() ){
		OneObject* OB=Group[GetUnitIndex()];
		if(OB&&OB->ActiveAbility){

			HeroAbility* Hero = (HeroAbility*)HeroAbilityRef.Get(); //OB->newMons->Ability->AbilitiesList[HeroAbilityIndex];
			Hero->UpHeroParams[UpIndex]->Realize(OB);

			itr_AA.Create(OB);
			while(ActiveUnitAbility* AA=itr_AA.Next()){
				if(AA->Type==8){
					AA->Deleted=true;
				}
				if(AA->Type==7){
					HeroVariableStorage* S = (HeroVariableStorage*)AA;
					S->SelectHeroParamState=false;
				}
			}
		}
	}
	return false;
}
bool ChooseUpHeroParam::Process()
{
	return !Deleted;
}
//==================================================================================================================//
bool UpLevelParam::OnClick()
{
	return false;
}
bool UpLevelParam::Process()
{
	return true;
}
//==================================================================================================================//
int UpHeroParamList::GetExpansionRules()
{
	return 2;
}
//==================================================================================================================//
/*
int CardList::GetExpansionRules()
{
	return 2;
}
*/
//==================================================================================================================//
HeroAbility::HeroAbility()
	: GetExperienceProc(0)
	, GetExperienceProcAlly(75)
{
	ExperienceRadius=0;
	LifeRegeneration=0;
	Lives=3;
}
bool HeroAbility::OnUnitBirth(OneObject* Newbie)
{
	/*
	if(Newbie)
	{
		HeroVariableStorage* HVS = new HeroVariableStorage();

		HVS->UnitIndex=Newbie->Index;
		HVS->OB=Newbie;
		HVS->HeroAbilityIndex=Index;
		HVS->Hero=this;
		HVS->SetA(this);
		HVS->FileID=FileID;
		HVS->SpriteID=SpriteID;
		HVS->ReBornTime=ReBornTime;
		HVS->freePlaceCard=PlaceCard;
		HVS->CardRegeneration=CardRegeneration.Get();
		
		Newbie->Mana=Mana;
		Newbie->MoreCharacter->MaxMana=Mana;

		AddActiveUnitAbility(Newbie->Index,HVS);
		//HeroesCollector.Herosima.Add(HVS);
		return true;
	}
	*/
	return false;
}

extern word NPlayers;

ActiveUnitAbility* HeroAbility::GetNewActiveAbility(){
	HeroVariableStorage* nAA = new HeroVariableStorage;
	nAA->SetA(this);

	nAA->HeroAbilityIndex=Index;
	nAA->Hero=this;
	
	//nAA->ReBornTime=ReBornTime;

	//nAA->freePlaceCard=PlaceCard;
	nAA->CardRegeneration=CardRegeneration.Get();

	// by Xeno: new lives handler
	extern char* _HEROLIVES[];
	Lives=atoi(_HEROLIVES[GSets.RoomParams.HeroLives]+strlen("#mpopt_Lives"));
	if (MOptions.RPGOnly && (MOptions.MaxRPGLives >= 0 && (Lives > MOptions.MaxRPGLives || MOptions.SetLivesToMax))) Lives = MOptions.MaxRPGLives;

	nAA->Lives=Lives;
	/*
	if(NPlayers>1)
	{
		nAA->Lives+=GSets.RoomParams.HeroLives*2;
	}
	else
	{
		nAA->Lives-=vmSinglePlayerData.Difficulty;
		//if(nAA->Lives<0)nAA->Lives=0;
	}*/
	//if(NPlayers == 1 && !MOptions.RPGOnly)
	//{
	//	nAA->Lives-=vmSinglePlayerData.Difficulty;
	//	if(nAA->Lives<0) nAA->Lives = 0;
	//}

	//HeroesCollector.Herosima.Add(nAA);

	return nAA;
}
//==================================================================================================================//
HeroVariable::HeroVariable()
{
	Name="";
	Value=0;
}
//==================================================================================================================//
int* HeroVariableStorage::GetVarRef(const char* Name)
{
	int n=Variables.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(!strcmp(Variables[i]->Name.pchar(),Name))
		{
			return &Variables[i]->Value;
		}
	}
	return NULL;
}
void GiveExperience(OneObject* OB, int Expa){
	HeroVariableStorage* HVS=DetectHero(OB);
	if(HVS){
		HVS->Experience+=Expa;
	}
}

void IncreaseLevel(OneObject* OB, int Level)
{
	if (Level < 1) return;
	HeroVariableStorage* HVS=DetectHero(OB);
	if(HVS)
	{
		for (int i = 0; i < Level; i++)
		{
			HVS->Experience+=HVS->GetExperienceToNextLevel();
			HVS->UpLevel();
		}
	}
}

void GiveExpa(int Expa){
	int N=ImNSL[MyNation];
	for(int i=0;i<N;i++){
		word MID=ImSelm[MyNation][i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			GiveExperience(OB,Expa);
		}
	}
}
int HeroVariableStorage::GatherExperience(OneObject* Victim,word Killer)
{
	if( !(OB && EnableCollectExpa) ){
		return 0;
	}
	int rez=0;
	if(GetUnitIndex()!=0xFFFF)
	{
		OneObject* OB=Group[GetUnitIndex()];
		if((!OB->Sdoxlo))
		{
			if(Hero){
				if( Killer!=0xFFFF && !OB->newMons->NewParams.GiveExpaDisable ){
					if( !(Victim->NMask&OB->NMask) ){
						HeroVariableStorage* VHVS=DetectHero(Victim);
						OneObject* Ki = Group[Killer];
						bool summon=false;
						if( Ki->ActiveAbility ) 
							Ki->ActiveAbility->ActiveAbilities.modifySummon(summon);
						if( Killer==GetUnitIndex() || summon && OB->NNUM==Ki->NNUM ){
							rez=Victim->newMons->Expa;
							if(VHVS)
								rez+=VHVS->Level*EngSettings.HeroKillModulatorPerLevel;
							OB->ActiveAbility->ActiveAbilities.modifyLearning(rez,rez);
							Experience+=rez;
						}
						else if( OB->NNUM == Ki->NNUM || (OB->NMask&Ki->NMask) != 0 )
						{
							int dist = Norma( OB->RealX - Victim->RealX, OB->RealY - Victim->RealY ) / 16;
							if ( dist < Hero->ExperienceRadius + AddExperienceRadius )
							{
								if (VHVS)
									rez += VHVS->Level * EngSettings.HeroKillModulatorPerLevel;
								OB->ActiveAbility->ActiveAbilities.modifyLearning(rez,rez);
								int proc = (OB->NNUM == Ki->NNUM) ? Hero->GetExperienceProc : Hero->GetExperienceProcAlly;
								rez = Victim->newMons->Expa * proc / 100;
								Experience+=rez;
							}
						}
					}
				}
			}else{
				if(HeroAbilityIndex!=-1)
				{
					if(OB&&OB->newMons->Ability)
					{
						int n=OB->newMons->Ability->AbilitiesList.GetAmount();
						if(HeroAbilityIndex<n)
						{
							Hero = (HeroAbility*)GetA();//OB->newMons->Ability->AbilitiesList[HeroAbilityIndex];
						}
					}
				}
			}

		}
	}	
	return rez;
}
//==================================================================================================================//

bool GetHero(OneObject* OB);
CEXPORT char* GetMonsterID(int NIndex);

void RefreshHeroCampaignData(){
	if( vGameMode==gmCamp ){
		int nCamp=vmSinglePlayerData.Heri.GetAmount();
		if( vmCampID>=0 && vmCampID<nCamp ){
			SinglePlayerData_CampaignHeroInfo* hCamp=vmSinglePlayerData.Heri[vmCampID];
			if(hCamp){
				SinglePlayerData_HeroesInfoList* hMiss=NULL;

				int nMiss=hCamp->CampaignMissions.GetAmount();
				int curMiss=vmCampaigns[vmCampID]->curMission;
				if(nMiss<=curMiss){
					for(int i=0;i<curMiss-nMiss+1;i++){
						hMiss = new SinglePlayerData_HeroesInfoList;
						hCamp->CampaignMissions.Add(hMiss);
					}
				}else{
					hMiss = hCamp->CampaignMissions[curMiss];
				}
				if(hMiss){

					itr_GetUnitsOfNation.Create(MyNation);
					while(OneObject* OB=itr_GetUnitsOfNation.Next()){
						
						if(GetHero(OB)){

							SinglePlayerData_HeroInfo* hInfo=NULL;

							int nHero=hMiss->MissionHeroes.GetAmount();
							for(int h=0;h<nHero;h++){
								hInfo=hMiss->MissionHeroes[h];
								if( hInfo && hInfo->HeroType==GetMonsterID(OB->ActiveAbility->BornNIndex) ){
									break;
								}
								hInfo=NULL;
							}

							if(!hInfo){
								hInfo = new SinglePlayerData_HeroInfo;
								hInfo->HeroType=GetMonsterID(OB->ActiveAbility->BornNIndex);
								hMiss->MissionHeroes.Add(hInfo);
							}

							//int nHero=hMiss->MissionHeroes.GetAmount();
							//SinglePlayerData_HeroInfo* hInfo=NULL;
							//for(int h=0;h<nHero;h++){
							//	hInfo=hMiss->MissionHeroes[h];
								if( hInfo /*&& hInfo->HeroType==GetMonsterID(OB->ActiveAbility->BornNIndex)*/ ){

									// деньги
									hInfo->Money=XRESRC(OB->NNUM,MoneyID);

									// сохран€ем прокачки
									OB->MoreCharacter->Copy(&hInfo->MoreCharacter,false);

									// сохран€ем абилки
									if(OB->ActiveAbility){

										OB->ActiveAbility->ActiveAbilities.Copy(&hInfo->Abilities,false);

										//hInfo->Abilities.Clear();
										hInfo->AbilitiesName.Clear();
										
										itr_AA.Create(OB);
										while(ActiveUnitAbility* AA=itr_AA.Next()){
											
											_str* Name = new _str;

											UnitAbility* UA=AA->GetA();
											if(UA){
												//ActiveUnitAbility* Abl = new ActiveUnitAbility;
												//AA->Copy(Abl,false);
												//hInfo->Abilities.Add(Abl);
												(*Name)=UA->Name;
											}

											hInfo->AbilitiesName.Add(Name);

										}

									}

									//break;
								}
								
							//}

						}						
					}

				}
			}
		}
	}
}

extern bool vGameLoaing;
void CreateNewMonster(NewMonster* NM,AdvCharacter* AC);

bool HeroVariableStorage::Process(){
	if(Hero){
		Hint.Clear();
		//OneObject* OB=Group[UnitIndex];
		if(OB/*&&UnitIndex!=0xFFFF*/){
			OB->Excited=true;
			// load data for campaign
			if( !LoadFromProfile && OB->NNUM==MyNation ){

				if( vGameMode==gmCamp ){

					// добавить необходимые екземпл€ры

					int n=vmSinglePlayerData.Heri.GetAmount();
					SinglePlayerData_CampaignHeroInfo* hCamp=NULL;
					if(n<=vmCampID){
						for(int i=0;i<vmCampID-n+1;i++){
							hCamp = new SinglePlayerData_CampaignHeroInfo();
							vmSinglePlayerData.Heri.Add(hCamp);
						}
					}else{
						hCamp = vmSinglePlayerData.Heri[vmCampID];
					}
					
					SinglePlayerData_HeroesInfoList* hMiss=NULL;

					int nn=hCamp->CampaignMissions.GetAmount();
					int nm=vmCampaigns[vmCampID]->curMission;
					if(nn<=nm){
						for(int i=0;i<nm-nn+1;i++){
							hMiss = new SinglePlayerData_HeroesInfoList;
							hCamp->CampaignMissions.Add(hMiss);
						}
					}else{
						hMiss = hCamp->CampaignMissions[nm];
					}

					SinglePlayerData_HeroInfo* hInfo=NULL;

					int nHero=hMiss->MissionHeroes.GetAmount();
					for(int h=0;h<nHero;h++){
						hInfo=hMiss->MissionHeroes[h];
						if( hInfo && hInfo->HeroType==GetMonsterID(OB->ActiveAbility->BornNIndex) ){
							break;
						}
						hInfo=NULL;
					}

					if(!hInfo){
						hInfo = new SinglePlayerData_HeroInfo;
						hInfo->HeroType=GetMonsterID(OB->ActiveAbility->BornNIndex);

						// деньги
						hInfo->Money=XRESRC(OB->NNUM,MoneyID);

						// сохран€ем прокачки
						OB->MoreCharacter->Copy(&hInfo->MoreCharacter,false);

						// сохран€ем абилки
						if(OB->ActiveAbility){

							OB->ActiveAbility->ActiveAbilities.Copy(&hInfo->Abilities,false);

							//hInfo->Abilities.Clear();
							hInfo->AbilitiesName.Clear();
							
							itr_AA.Create(OB);
							while(ActiveUnitAbility* AA=itr_AA.Next()){
								
								_str* Name = new _str;

								UnitAbility* UA=AA->GetA();
								if(UA){
									//ActiveUnitAbility* Abl = new ActiveUnitAbility;
									//AA->Copy(Abl,false);
									//hInfo->Abilities.Add(Abl);
									(*Name)=UA->Name;
								}

								hInfo->AbilitiesName.Add(Name);

							}

						}

						hMiss->MissionHeroes.Add(hInfo);

					}

					// востанавливаем характеристики геро€ из прошлых миссий

					SinglePlayerData_HeroesInfoList* prevMiss = NULL;
					SinglePlayerData_HeroInfo* prevHero = NULL;

					int PreviosMissID=nm-1;
					while( prevHero==NULL && PreviosMissID>=0 && PreviosMissID<=nn ){
						prevMiss = hCamp->CampaignMissions[PreviosMissID];
						if(prevMiss){
							int nnn=prevMiss->MissionHeroes.GetAmount();
							for(int i=0;i<nnn;i++){
								hInfo=prevMiss->MissionHeroes[i];
								if( hInfo&&hInfo->HeroType==GetMonsterID(OB->ActiveAbility->BornNIndex) ){
									prevHero=hInfo;
									break;
								}
							}
						}
						PreviosMissID--;
					}

					if( prevHero && !vGameLoaing ){

						// деньги
						SetXRESRC(OB->NNUM,MoneyID,prevHero->Money);

						// востанавливаем прокачки
						prevHero->MoreCharacter.Copy(OB->MoreCharacter,false);

						// мега HACK
						if(OB->MoreCharacter->VisionType<OB->newMons->VisionType)
							OB->MoreCharacter->VisionType=OB->newMons->VisionType;
						CreateNewMonster(OB->newMons,OB->MoreCharacter);

						// удал€ем текущие шмотки
						//itr_AA.Create(OB);
						//while(ActiveUnitAbility* A=itr_AA.Next()){
						//	if( A->TypeAbil==4 || A->TypeAbil==5 ){
						//		A->Deleted=true;
						//	}
						//}

						// добавл€ем шмотки и востанавливаем HeroVariableStorage
						int n=prevHero->Abilities.GetAmount();
						for(int i=0;i<n;i++){
							ActiveUnitAbility* A=prevHero->Abilities[i];
							if( A ){
								HeroVariableStorage* HVS=dynamic_cast<HeroVariableStorage*>(A);
								if(HVS){
									//HVS->Copy(this,true);

									Level=HVS->Level;
									Experience=HVS->Experience;
									//int AddExperienceRadius;
									//int Knowledge;
									//bool PassSelectHeroParametr;
									//bool SelectHeroParamState;
									//LinearArray<int,_int> Param;
									UpLifeRegeneration=HVS->UpLifeRegeneration;
									UpManaRegeneration=HVS->UpManaRegeneration;
									//bool IsTired;
									//int SpeedMinus;
									ExperienceToNextLevel=HVS->ExperienceToNextLevel;
									//Lives=HVS->Lives;

								}else
								if( A->TypeAbil!=0 && A->TypeAbil!=7 && i<prevHero->AbilitiesName.GetAmount() ){
									
									//UnitAbility* UA=A->GetA();

									char* aName=prevHero->AbilitiesName[i]->pchar();
									if(aName){
										int m = Abilities.Abilities.GetAmount();
										for(int j=0;j<m;j++){
											UnitAbility* UA=Abilities.Abilities[j];
											if( UA && UA->Name==aName ){

												//OneItem* newA=(OneItem*)UA->GetNewActiveAbility();
												ActiveUnitAbility* newA=UA->GetNewActiveAbility();

												if(newA->Type==7){
													delete newA;
													break;
												}

												newA->ItemDrop=A->ItemDrop;
												newA->Amount=A->Amount;

												AddActiveUnitAbility(OB->Index,newA);

												//if(!UA->CanNotBeClothe)
												//	newA->Clothed=A->Clothed;
												
												newA->Clothed=A->Clothed;
												newA->Slot=A->Slot;
												break;

											}
										}
									}

								}
							}
						}

					}			

				}
				LoadFromProfile=true;
				
			}


			// process
			Hint.Add("Level: ");
			Hint.Add(Level);
			Hint.Add("\\");
			Hint.Add("Experience: ");
			Hint.Add(Experience);
			Hint.Add("\\ExpToNext: ");
			Hint.Add(ExperienceToNextLevel);

			if( OB->Sdoxlo && !OB->Hidden ){
				HeroAbility* HA=(HeroAbility*)GetA();
				if(HA){
					UnitAbility* DieUA=HA->OnDieUnitAbility.Get();
					if(DieUA){
						//if(Lives>0){
						//	itr_AA.Create(OB);
						//	while(ActiveUnitAbility* AA=itr_AA.Next()){
						//		AA->AutoCast=false;
						//		UnitAbility* UA=AA->GetA();
						//		if( UA && DieUA!=UA ){
						//			if( AA->TypeAbil==0  && !(UA->IgnoreMagicImmunity||UA->Permanent||AA->InfluenceMask.check_bit(ABL_MagicImmunity)) ){
						//				AA->Deleted=true;
						//			}
						//		}
						//	}
						//	Experience=0;
						//	Lives--;
						//	OB->Sdoxlo=0;
						//	OB->Life=OB->MaxLife;
						//	OB->NewAnm=OB->newMons->GetAnimation(anm_Stand);
						//	OB->SetZeroFrame();

						//	int id=NATIONS[OB->NNUM].CreateNewMonsterAt(OB->RealX,OB->RealY,OB->NIndex,true,OB->RealDir);
						//	if(id!=-1){
						//		OneObject* Corpse=Group[id];
      //                          if(Corpse->ActiveAbility)
						//		    Corpse->ActiveAbility->ActiveAbilities.Clear();
						//		vHeroButtons.Remove(Corpse);
						//		Corpse->Die();
						//	}

						//	Casting=false;
						//	//OB->DeleteLastOrder();
						//								
						//	itr_AA.Create(OB);
						//	while(ActiveUnitAbility* AA=itr_AA.Next()){
						//		UnitAbility* UA=AA->GetA();
						//		if( AA->TypeAbil==0 && UA!=DieUA ){
						//			AA->Deleted=true;
						//		}
						//	}

						//	// перенес в UnitAbilityExtension::OnUnitDie
						//	//addrand(OB->Index);
						//	//AddOneEffect(NULL,DieUA,OB,OB);
						//}
					}
				}

			}else{

				if(ExperienceToNextLevel==-1)ExperienceToNextLevel=GetExperienceToNextLevel();
				if(ExperienceToNextLevel!=-1){
					if(Experience>=ExperienceToNextLevel)
						UpLevel();
				}

				/*if(OB->Life<OB->MaxLife)*/{
					int LifeRegen = Hero->LifeRegeneration + UpLifeRegeneration;
					if(OB->ActiveAbility){
						OB->ActiveAbility->ActiveAbilities.modifyLifeRegeneration(LifeRegen,LifeRegen);
					}
					//if(LifeRegen<=0){
					//	LifeRegen=1;
					//}
					if(LastUpLifeRegenerationTime==-1)
						LastUpLifeRegenerationTime=AnimTime;
					int up =((AnimTime-LastUpLifeRegenerationTime)*LifeRegen)/(25*256)/10;
					if(up){
						int Life=int(OB->Life)+up;
						if(Life>=OB->MaxLife){
							OB->Life=OB->MaxLife;
							LastUpLifeRegenerationTime=-1;
						}else{
							if(Life<1)
								Life=1;
							else
								OB->Life=Life;
							LastUpLifeRegenerationTime+=up*25*256*10/LifeRegen;
						}
					}
				}
				/*if(OB->Mana<OB->MaxMana)*/{
					int ManaRegen = Hero->ManaRegeneration + UpManaRegeneration + OB->ActiveAbility->CardManaRegeneration/100;
					if(OB->ActiveAbility){
						OB->ActiveAbility->ActiveAbilities.modifyManaRegeneration(ManaRegen,ManaRegen);
					}
					//if(ManaRegen<=0){
					//	ManaRegen=1;
					//}
					if(LastUpManaRegenerationTime==-1)
						LastUpManaRegenerationTime=AnimTime;
					int up =((AnimTime-LastUpManaRegenerationTime)*ManaRegen)/(25*256)/10;
					if(up){
						int Mana=int(OB->Mana)+up;
						if(Mana>=OB->MaxMana){
							OB->Mana=OB->MaxMana;
							LastUpManaRegenerationTime=-1;
						}else{
							if(Mana<0)
								OB->Mana=0;
							else
								OB->Mana=Mana;
							LastUpManaRegenerationTime+=up*25*256*10/ManaRegen;
						}
					}
				}

				// ----- EDITED BY XENO: implementation of 3 different gameplay ---------
				// RTS only
				if( GSets.RoomParams.RTS_mode == 1 && vGameMode!=gmEdit ){
					itr_AA.Create(OB);
					while(ActiveUnitAbility* AA=itr_AA.Next()){
						UnitAbility* UA=AA->GetA();
						if( UA && !UA->Name.isClear() && strstr(UA->Name.pchar(),"MC_aSummonPeasants") ){
							AA->OnClick();
							break;
						}
					}
				}
				// RPG Only
				if( GSets.RoomParams.RTS_mode == 2 && vGameMode!=gmEdit ){
					itr_AA.Create(OB);
					while(ActiveUnitAbility* AA=itr_AA.Next()){
						UnitAbility* UA=AA->GetA();
						if( UA && !UA->Name.isClear() && strstr(UA->Name.pchar(),"MC_aSummonPeasants") ){
							AA->OnRightClick();
							break;
						}
						if( UA && !UA->Name.isClear() && strstr(UA->Name.pchar(),"MC_aHeroStart") ){
							AA->OnClick();
							break;
						}
					}
				}
				// RPG + RTS
				if( GSets.RoomParams.RTS_mode == 3 && vGameMode!=gmEdit ){ //(vGameMode == gmMul || vGameMode == gmSin)){
					itr_AA.Create(OB);
					while(ActiveUnitAbility* AA=itr_AA.Next())
					{
						UnitAbility* UA=AA->GetA();
						if( UA && !UA->Name.isClear() && strstr(UA->Name.pchar(),"MC_aSummonPeasants") )
						{
							AA->Execute(0xFFFF,0,0,0);
							break;
						}
					}
				}

				// AI
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

				//if(OB->GetTired<=0&&(!IsTired)){
				//	if(Hero->SpeedDownIfTired&&Hero->SpeedDownIfTired<101)
				//	{
				//		SpeedMinus=(OB->MoreCharacter->Speed*Hero->SpeedDownIfTired)/100;
				//		OB->MoreCharacter->Speed-=SpeedMinus;
				//		IsTired=true;
				//	}
				//}
				//if(IsTired&&OB->GetTired>0){
				//	OB->MoreCharacter->Speed+=SpeedMinus;
				//	IsTired=false;
				//}
			}
            OB->ActiveAbility->CardManaRegeneration=0;
		}

	}

	return true;
}
int HeroVariableStorage::GetExperienceToNextLevel()
{
	int rez=-1;
	if(Hero)
	{
		int n=Hero->LevelUp.GetAmount();
		if(Level<=n)
			rez=Hero->LevelUp[Level-1];
	}
	return rez;
}
void HeroVariableStorage::UpLevel()
{
	if( !SelectHeroParamState && OB->ActiveAbility->BornNIndex==OB->NIndex ){
		
		Level++;
		Experience=Experience-ExperienceToNextLevel;
		ExperienceToNextLevel=GetExperienceToNextLevel();
		CreateOptionUpHeroParams();
		CreateLevelUpParam();

		if( OB->NNUM==MyNation ){
			void HeroLevelUpEvent();
			HeroLevelUpEvent();
		}

	}
}
void HeroVariableStorage::CreateOptionUpHeroParams()
{
	int n=Hero->UpHeroParams.GetAmount();
	if(n){
		ClassArray<Probability> W;
		int Sum=0;
		for(int i=0;i<n;i++){
			int w1=0;
			int nn=Hero->UpHeroParams[i]->ProbabilityList.GetAmount();
			if(!Hero->UpHeroParams[i]->Special)
				for(int j=0;j<nn;j++){
					if(Level<=Hero->UpHeroParams[i]->ProbabilityList[j]->Level){
						w1=Hero->UpHeroParams[i]->GetWeight(this,j);
						break;
					}
				}
			if(w1){
				Probability* P = new Probability();
				P->Level=i;
				P->Weight=w1;
				W.Add(P);
				Sum+=w1;
			}
		}
		int wn=W.GetAmount();
		int wnn=wn;
		if(!PassSelectHeroParametr){
			if(Sum){
				//int s1=Sum;
				int u1 = rando()%Sum;
				for(int i=0;i<wn;i++){
					u1-=W[i]->Weight;
					if(u1<=0){
						AddChooseUpHeroParam(W[i]->Level);
						const char* UpName = Hero->UpHeroParams[W[i]->Level]->GetClassName();
						for(int k=0;k<wn;k++){
							if(!strcmp(UpName,Hero->UpHeroParams[W[k]->Level]->GetClassName())){
								Sum-=W[k]->Weight;
								W.Del(k,1);
								wn--;
								k--;
							}
						}
						break;
					}
				}
				if(Sum){
					u1 = rando()%Sum;
					for(int i=0;i<wn;i++){
						u1-=W[i]->Weight;
						if(u1<=0){
							AddChooseUpHeroParam(W[i]->Level);
							break;
						}
					}
				}
			}
			if(wnn>2){
				for(int i=0;i<n;i++){
					if(Hero->UpHeroParams[i]->Special)
						AddChooseUpHeroParam(i);
				}
			}
		}else{
			for(int i=0;i<wn;i++){
				if(Hero->UpHeroParams[W[i]->Level]->IsInPass)
					AddChooseUpHeroParam(W[i]->Level);
			}
			PassSelectHeroParametr=false;
		}
	}
}
void HeroVariableStorage::AddChooseUpHeroParam(int UpHeroParamIndex)
{
	UpHeroParam* U = Hero->UpHeroParams[UpHeroParamIndex];
	ChooseUpHeroParam* Ch = new ChooseUpHeroParam();
	Ch->OB=OB;
	
	//Ch->UnitIndex=UnitIndex;
	Ch->UpIndex=UpHeroParamIndex;
	Ch->FileID=U->FileID;
	Ch->SpriteID=U->SpriteID;
	Ch->Visible=true;
	Ch->Hint=U->Hint;

	//Ch->HeroAbilityIndex=HeroAbilityIndex;
	Ch->HeroAbilityRef.Set(Hero);
	
	AddActiveUnitAbility(GetUnitIndex(),Ch);
	SelectHeroParamState=true;
}
void HeroVariableStorage::CreateLevelUpParam()
{
	int n=Hero->UpParamForLevel.GetAmount();
	if(n)
	{
		ClassArray<Probability> W;
		int Sum=0;
		for(int i=0;i<Hero->UpParamForLevel.GetAmount();i++)
		{
			int w1=0;
			int nn=Hero->UpParamForLevel[i]->ProbabilityList.GetAmount();
			if(!Hero->UpParamForLevel[i]->Special) //UpHeroParams
				for(int j=0;j<nn;j++)
				{
					if(Level<=Hero->UpParamForLevel[i]->ProbabilityList[j]->Level)
					{
						w1=Hero->UpParamForLevel[i]->ProbabilityList[j]->Weight;
						break;
					}
				}
				if(w1)
				{
					Probability* P = new Probability();
					P->Level=i;
					P->Weight=w1;
					W.Add(P);
					Sum+=w1;
				}
		}
		int wn=W.GetAmount();
		if(Sum)
		{
			int u1 = rando()%Sum;
			for(int i=0;i<wn;i++)
			{
				u1-=W[i]->Weight;
				if(u1<=0)
				{
					UpHeroParam* U = Hero->UpParamForLevel[W[i]->Level];
					U->Realize(OB);
					break;
				}
			}
		}
	}
}
//==================================================================================================================//

HeroVariableStorage* DetectHero(OneObject* OB);

CHeroesCollector::CHeroesCollector(){
}
void CHeroesCollector::Reset(){
	HeroID.Clear();
	HeroSN.Clear();
};
void CHeroesCollector::OnDieProcess(OneObject* Victim,word Killer)
{
	if(Killer!=0xFFFF){

		int n=HeroID.GetAmount();
		for(int i=0;i<n;i++){
			OneObject* Hero=Group[HeroID[i]];
			if(Hero&&!Hero->Sdoxlo&&Hero->Serial==HeroSN[i]){
				HeroVariableStorage* Herosima=DetectHero(Hero);
				if(Herosima){
					Herosima->GatherExperience(Victim,Killer);
				}
			}
		}

		//int n=Herosima.GetAmount();
		//for(int i=0;i<n;i++)
		//{
		//	if(Herosima[i] && Herosima[i]->Level>=0 && Herosima[i]->UnitIndex>=0 && Herosima[i]->UnitIndex<MAXOBJECT)
		//	{
		//		OneObject* OB=Group[Herosima[i]->UnitIndex];
		//		if((!OB))//||(OB&&OB->Sdoxlo))
		//		{
		//			Herosima.Del(i,1);
		//			i--;
		//			n--;
		//		}
		//		else
		//		{
		//			if(!OB->Sdoxlo)
		//				Herosima[i]->GatherExperience(Victim,Killer);
		//		}
		//	}
		//}
	}
}
//==================================================================================================================//
extern RomeHeroCollector RomeHeroes; 
void HeroesGatherExperience(OneObject* Victim,word Killer)
{
	HeroesCollector.OnDieProcess(Victim,Killer);
	if(Killer!=0xFFFF)
	{
		OneObject* OB=Group[Killer];
		if(OB)
		{
			RomeHeroes.AddExp(Victim,OB);
		}
	}
}
//==================================================================================================================//
CUnitExperienceParm::CUnitExperienceParm()
{
	LifeK=0;
	DamageK=0;
	RangeK=0;
	SpeedK=0;
	xmlQuote xml;
	if(xml.ReadFromFile("UnitsAbility\\UnitExperienceParm.xml"))
	{
		ErrorPager EP;
		this->Load(xml,this,&EP);
	}
}
CUnitExperienceParm UnitExperienceParm;
//==================================================================================================================//
void SetUnitExperience(NewMonster* NM)
{
	NM->Expa=(UnitExperienceParm.LifeK*NM->Life)/100+(UnitExperienceParm.SpeedK*NM->MotionDist)/100;
	int MDamage=0;
	int range=0;
	for(int i=0;i<NAttTypes;i++)
	{
		if(NM->MaxDamage[i]>MDamage)
		{
			MDamage=NM->MaxDamage[i];
			range=NM->AttackRadius2[i];
		}
	}
	NM->Expa+=(UnitExperienceParm.DamageK*MDamage)/100+(UnitExperienceParm.RangeK*range)/100;
}
//==================================================================================================================//
HeroVariableStorage* GetHeroVariableStorage(OneObject* OB)
{
	if(OB&&OB->ActiveAbility){
		int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i=0;i<n;i++){
			ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
			if(AA&&AA->Type==7){
				return (HeroVariableStorage*)OB->ActiveAbility->ActiveAbilities[i];
			}
		}
	}	
	return NULL;
}
//==================================================================================================================//
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
MagicCardActive::MagicCardActive()
{
	fst=false;
	timer2=0;
	timer1=0;
	TypeAbil=-1;
};
bool MagicCardActive::Process()
{
	CurrentHeroAbility=GetHeroVariableStorage(OB);
	if(!CurrentHeroAbility&&(TypeAbil==1||TypeAbil==4)){
		return true;
	}
	//if(!Visible&&(TypeAbil==1||TypeAbil==4)&&CurrentHeroAbility->freePlaceCard>0){
	//	timer1=TrueTime+CurrentHeroAbility->CardRegeneration/CurrentHeroAbility->indexx;
	//	CurrentHeroAbility->freePlaceCard--;
	//	Visible=true;
	//	CurrentHeroAbility->indexx=1;
	//};
	if(Visible){
		MagicCard* A = (MagicCard*)GetA();
		if(A){
			if(TypeAbil==1||TypeAbil==4){
				Hint=A->Hint;
				Hint.Add("\\Damage: ");
				Hint.Add(A->Damage.Get());
				Hint.Add("\\Mana: ");
				Hint.Add(A->ManaCost.Get());
				if(GetCoolDownProc()>0){
					Hint.Add("\\CoolDown: ");
					Hint.Add(100-GetCoolDownProc());
					Hint.Add("%");
				}
				if(fst&&GetCoolDownProc()==0){
					fst=false;
					OneObject* OBj= NULL;
					if(A->EnemyTarget||A->FriendlyTarget){
						itr_UnitsInRadius.Create(Tx,Ty,A->Radius.Get());
						while(OBj=itr_UnitsInRadius.Next()){
							if((A->EnemyTarget&&!(OBj->NMask&OB->NMask))||(A->FriendlyTarget&&(OBj->NMask&OB->NMask))){
								bool mcb=true;
								if(OBj->ActiveAbility){
									for(int iii=0;iii<A->Blocking.GetAmountOfElements();iii++){
										for(int jjj=0;jjj<OBj->ActiveAbility->ActiveAbilities.GetAmount();jjj++){
											if(OBj->ActiveAbility->ActiveAbilities[jjj]->TypeAbil==0&&(OBj->ActiveAbility->ActiveAbilities[jjj]->GetA()==A->Blocking[iii]->Get())) mcb=false;
										}
									}
									for(int jj=0;jj<A->UnCast.GetAmountOfElements();jj++){
										for(int ii=0;ii<OBj->ActiveAbility->ActiveAbilities.GetAmountOfElements();ii++){
											UnitAbility* B = OBj->ActiveAbility->ActiveAbilities[ii]->GetA();
											if((B==A->UnCast[jj]->Get())&&OBj->ActiveAbility->ActiveAbilities[ii]->TypeAbil==0){
													MagicCardActive* actcrd=(MagicCardActive*)OBj->ActiveAbility->ActiveAbilities[ii];
													actcrd->timer2=TrueTime;
													actcrd->timer1=1;
											}
										}
									}
								}
								if(mcb){
									MagicCardActive* activecard=new MagicCardActive;
									activecard->OB=OBj;
									//activecard->UnitIndex=OBj->Index;
									activecard->timer2=TrueTime+A->LongTime.Get();
									activecard->timer1=A->LongTime.Get();
									activecard->Radius=A->Radius.Get();
									activecard->Damage=A->Damage.Get();
									activecard->TypeAbil=0;
									activecard->FileID=A->EffectFileID;
									activecard->SpriteID=A->EffectSpriteID;
									activecard->fst=true;
									activecard->Visible=true;
									activecard->SetA(A);
									bool rez=AddActiveUnitAbility(OBj->Index,activecard);
									if(!rez){
										return true;
									}
/*									for(int jj=0;jj<A->MSCast.GetAmount();jj++){
										A->MSCast[jj]->CastSpell(OBj);
									}*/
								}
							}
						}
					}
					Visible=false;
					int cou=OB->ActiveAbility->ActiveAbilities.GetAmount();
					for(int jj=0;jj<cou;jj++){
						if(OB->ActiveAbility->ActiveAbilities[jj]->Serial==Serial){
							OB->ActiveAbility->ActiveAbilities.MoveElement(jj,cou-jj);
							//CurrentHeroAbility->freePlaceCard++;
							return true;
						};
					};
				};
			}else
				if(TypeAbil==0){
					if (timer1!=0&&timer2!=0&&TrueTime<=timer2) {
						int tm=99-((timer2-TrueTime)*100)/(timer1);
						Hint=A->Hint;
						Hint.Add("\\Processed - ");
						Hint.Add(tm);
						Hint.Add("%");
						if(A->MSCast.GetAmount()>0){
							Hint.Add("\\");
							for(int jj=0;jj<A->MSCast.GetAmount();jj++){
								Hint.Add(A->MSCast[jj]->name);
								Hint.Add(" ");
								if(A->MSCast[jj]->sign)	Hint.Add("+");
								Hint.Add(A->MSCast[jj]->realchng);
								Hint.Add("  ");
							}
						}
						int cr=(tm)%5;
						if (cr==0&&fst){
							int Tz=GetHeight(OB->RealX>>4,OB->RealY>>4);
							AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
							AWP->Damage=Damage;
							AWP->AttType=0;
							CreateNewActiveWeapon(A->EffectName.pchar(),GetUnitIndex(),OB->RealX>>4,OB->RealY>>4,Tz,0xFFFF,OB->RealX>>4,OB->RealY>>4,Tz,AWP);
							fst=false;
						}
						if (cr!=0&&!fst) {
							fst=true;
						}
					};
					if(timer2!=0&&TrueTime>timer2){ 
						/*for(int jj=0;jj<A->MSCast.GetAmount();jj++){
							A->MSCast[jj]->CancelSpell(OB);
						}*/
						return false;
					}
				}
		}		
	}
	return true;
};
bool MagicCardActive::OnClick()
{
	if(TypeAbil==0||GetCoolDownProc()!=0) return false;
	CurrentHeroAbility=GetHeroVariableStorage(OB);
	if(isTarget()) fst=true;
	return false;
};
bool MagicCardActive::OnRightClick()
{
	return false;

	if(TypeAbil==0||GetCoolDownProc()!=0) return false;
	CurrentHeroAbility=GetHeroVariableStorage(OB);
	Visible=false;
	int cou=OB->ActiveAbility->ActiveAbilities.GetAmount();
	for(int jj=0;jj<cou;jj++){
		if(OB->ActiveAbility->ActiveAbilities[jj]->Serial==Serial){
			OB->ActiveAbility->ActiveAbilities.MoveElement(jj,cou-jj);
			//CurrentHeroAbility->freePlaceCard++;
			CurrentHeroAbility->indexx=4;
			return true;
		}
	}
	return false;
};
bool MagicCardActive::Execute(word TargetUnit, int x, int y, int z)
{
	bool rez=CanTarget(TargetUnit,x,y,z);
	int Tz;
	if(rez)
	{
		MagicCard* A = (MagicCard*)GetA();
		if(A){
			CurrentHeroAbility=GetHeroVariableStorage(OB);
			/*if(A->EnemyTarget||A->FriendlyTarget){
				rez=false;
				itr_UnitsInRadius.Create(x,y,A->Radius.Get());
				OneObject* OOB=itr_UnitsInRadius.Next();
				while(OOB&&!rez){
					if((A->EnemyTarget&&!(OOB->NMask&OB->NMask))||(A->FriendlyTarget&&(OOB->NMask&OB->NMask))) rez=true;
					OOB=itr_UnitsInRadius.Next();
				}
				if(!rez) return rez;
			}*/
			if(OB->Mana>=A->ManaCost.Get())
			{
				Tz=GetHeight(x,y);
				AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
				AWP->Damage=A->Damage.Get();
				AWP->AttType=0;
				CreateNewActiveWeapon(A->EffectName.pchar(),GetUnitIndex(),x,y,Tz,0xFFFF,x,y,Tz,AWP);
				OB->Mana-=A->ManaCost.Get();
			}
		}
		Tx=x;
		Ty=y;
		fst=true;
	}
	return rez;
}
bool MagicCardActive::isTarget()
{
	if(GetCoolDownProc()!=0) return false;
	return true;
}
bool MagicCardActive::DrawCursor(int x,int y)
{	
	if(TypeAbil==1||TypeAbil==4){
		CurrentHeroAbility=GetHeroVariableStorage(OB);
		MagicCard* A = (MagicCard*)GetA();
		if(A){
			int r=A->Radius.Get();
			DrawSelPatch(x,y,A->CursType,r,A->CursColor);
			if(A->EnemyTarget||A->FriendlyTarget){
				itr_UnitsInRadius.Create(x,y,r);
				while(OneObject* obb=itr_UnitsInRadius.Next())
				{
					if(A->EnemyTarget&&!(obb->NMask&OB->NMask))
					{
						obb->HighlightMask=1;
					}
					if(A->FriendlyTarget&&obb->NMask&OB->NMask)
					{
						obb->HighlightMask=2;
					}
				}			
				return true;
			}
		}
	}
	return false;
}
bool MagicCardActive::CanTarget(word TargetUnit, int x, int y, int z)
{
	if(GetCoolDownProc()!=0) return false;
	/*UnitAbility* A = GetA();
	if(A){
		for(int i=0;i<A->Blocking.GetAmountOfElements();i++){
			for(int j=0;j<OB->ActiveAbility->ActiveAbilities.GetAmount();j++){
				if(OB->ActiveAbility->ActiveAbilities[j]->TypeAbil==0&&(OB->ActiveAbility->ActiveAbilities[j]->GetA()==A->Blocking[i]->Get())) return false;
			}
		}
	};*/
	return true;
}
int MagicCardActive::GetCoolDownProc()
{
	int rez=0;
	CurrentHeroAbility=GetHeroVariableStorage(OB);
	if (timer1>TrueTime&&CurrentHeroAbility->CardRegeneration!=0) {
		rez=100*(timer1-TrueTime)/(CurrentHeroAbility->CardRegeneration)+1;
	}
	return rez;
}
//==================================================================================================================//
MagicCard::MagicCard()
{
	EnemyTarget=false;
	FriendlyTarget=false;
	ProcTime=0;
}
ActiveUnitAbility* MagicCard::GetNewActiveAbility(){	
	MagicCardActive* nAA=new MagicCardActive;
	nAA->SetA(this);
	return nAA;
}
bool MagicCard::OnUnitBirth(OneObject* Newbie){
	if(Newbie){
		MagicCardActive* newA=(MagicCardActive*)GetNewActiveAbility();
		//
		newA->timer2=LongTime.Get();
		newA->TypeAbil=1;
		newA->Radius=Radius.Get();
		newA->Damage=Damage.Get();
		//
		if(AddActiveUnitAbility(Newbie->Index,newA)){
			return true;
		}else{
			delete newA;
		}
	} 
	return false;
};
bool MagicCard::CanApply(OneObject* her, HeroVariableStorage* storage)
{
	return false;
}
int MagicCard::GetCoolDownProc(OneObject* her, HeroVariableStorage* storage)
{
	int rez=0;
	CurrentHeroAbility=storage;
	if (ProcTime>TrueTime&&CurrentHeroAbility->CardRegeneration!=0) {
		rez=100*(ProcTime-TrueTime)/(CurrentHeroAbility->CardRegeneration)+1;
	}
	if(ProcTime<=TrueTime) rez=0;
	return rez;
}
int MagicCard::GetRadius(OneObject* her, HeroVariableStorage* storage)
{
	CurrentHeroAbility=storage;
	return Radius.Get();
}
bool MagicCard::OnClick(OneObject* her, HeroVariableStorage* storage)
{
	return false;
}
bool MagicCard::Execute(OneObject* her, HeroVariableStorage* storage , word TargetUnit, int x, int y, int z)
{
	return false;
}
bool MagicCard::isTarget(OneObject* her, HeroVariableStorage* storage)
{
	return true;
}
bool MagicCard::CanTarget(OneObject* her, HeroVariableStorage* storage, word TargetUnit, int x, int y, int z)
{
	return false;
}
bool MagicCard::isActive(OneObject* her, HeroVariableStorage* storage)
{
	return false;
}
//==================================================================================================================//

int GetLandTypeInPoint(int x,int y);

ActiveUnitAbility* ua_AtHome::GetNewActiveAbility(){	
	aa_AtHome* nAA=new aa_AtHome;
	nAA->SetA(this);
	return nAA;
}
bool aa_AtHome::Process(){	
	ua_AtHome* UA=(ua_AtHome*)GetA();
	if(UA&&!OB->Sdoxlo){
		int LandType=GetLandTypeInPoint(OB->RealX>>4,OB->RealY>>4);
		NATIONS[OB->NNUM].HeroAtHome = (LandType==UA->HomeLand);
	}else{
		NATIONS[OB->NNUM].HeroAtHome = false;
	}
	return true;
}

void HeroAbility::OnAddToObject(OneObject* OB){
	if(OB->MaxMana<Mana){
		OB->Mana=Mana;
		OB->MoreCharacter->MaxMana=Mana;
		OB->MaxMana=OB->MoreCharacter->MaxMana;
	}
	if(HeroesCollector.HeroID.find(OB->Index)==-1){
		HeroesCollector.HeroID.Add(OB->Index);
		HeroesCollector.HeroSN.Add(OB->Serial);
	}
};