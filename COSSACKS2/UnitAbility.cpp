#include "stdheader.h"
#include "BE_HEADERS.h"
#include "unitability.h"
#include "AI_Scripts.h"
#include "WeaponSystem.h"
#include "BrigadeAI.h"
#include "cvi_singleplayerdata.h"
#include "cvi_Campaign.h"
#include "UnitAbilityIcon.h"
#include "ua_Item.h"
#include "ua_Modificators.h"
#include ".\cvi_MainMenu.h"
#include "MonsterAbility.h"
#include "mesh\unihash.h"
#include "GameInterface.h"
#include "Abilities.h"

void SetActiveProducer(OneObject* OB);
//==================================================================================================================//
void TestCGARB();
bool ChangeUnitNIndex(OneObject* OB, word NewNIndex, int ThroughState);
DLLEXPORT void OBJ_ChangeNation(OneObject* OB, byte DstNat);
//
extern int NationID;
AbilityList Abilities;
BaseClass* GetAbilityClass(){
	return &Abilities.Abilities;
}
int GetTotalHeight0(int x,int y){
	int H=GetTotalHeight(x,y);
	if(H<0)H=0;
	return H;
}
void DeleteAbilities(OneObject* OB){
	if(OB&&OB->ActiveAbility){
		OB->ActiveAbility->ActiveAbilities.Clear();
	}
}
void ClearActiveAbilities(){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		DeleteAbilities(OB);
	}
}
bool ProcessAbilityClass(ClassEditor* CE,BaseClass* BC,int Options){
	if(Options==3||Options==4)
	{
		for(int i=0;i<NNewMon;i++)
		{
			if(NewMon[i]->Ability)
			{
				NewMon[i]->Ability->AbilitiesList.Clear();
				NewMon[i]->Ability->Feeled=false;
			}
		}
		ClearActiveAbilities();
	}
	bool ProcessWeaponClass(ClassEditor* CE,BaseClass* BC,int Options);
	ProcessWeaponClass(CE,BC,Options);
	return false;
}
//==================================================================================================================//
extern int TrueTime;
extern int AnimTime;
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
int PerformActionOverBuildingsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, int Damage, int AttType);
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
word GetDir(int dx,int dy);
extern Nation NATIONS[8];
extern int GetHeight(int x, int y);
extern HeroVariableStorage* CurrentHeroAbility;
HeroVariableStorage* GetHeroVariableStorage(OneObject* OB);
void RotateMon(OneObject* OB,char angle);
void PanicUnit(OneObject* OBJ);
void PerformNewUpgrade(Nation* NT,int UIndex,OneObject* OB);
//void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
void AddGroundCircle(int x,int y,int R,DWORD Color);
extern BrigadeAI WiselyFormations;
typedef bool cbCheckSprite(OneSprite* OS, void* Param);
int GetSpritesInRadius(int x, int y, int Radius, cbCheckSprite* cbF, void* Param);
int GetPointToLineDist(int x,int y,int x1,int y1,int x2,int y2);
CEXPORT bool GetPeaceMode();
extern int vmCampID;
extern ClonesArray<cvi_Campaign> vmCampaigns;
extern veGameMode vGameMode;
char* GetTextByID(char* id);
extern bool vGameLoaing;
DLLEXPORT int GetMyNation();
//==================================================================================================================//
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
void InitUnitAbilities(OneObject* OB){
	if(OB->ActiveAbility){
		OB->ActiveAbility->reset_class(OB->ActiveAbility);
		OB->ActiveAbility->Init();
		OB->ActiveAbility->OB=OB;
		OB->ActiveAbility->SetBornNIndex();
		OB->ActiveAbility->TransparencyStartTime=TrueTime;
		OB->ActiveAbility->CheckHero=false;
		OB->ActiveAbility->CheckChest=false;
	}
	NewMonster* NM=OB->newMons;
	if(NM->Ability){
		NM->Ability->Init(NM);
		MonsterAbility* MA=NM->Ability;
		for(int i=0;i<MA->AbilitiesList.GetAmount();i++){
			UnitAbility* UA=MA->AbilitiesList[i];
			if(UA){
				ActiveUnitAbility* newA=UA->GetNewActiveAbility();

				bool add=true;
				if(newA->TypeAbil==1){
					if( GSets.RoomParams.StartCards < MA->CommonRoomStartParam[i] ){
						add=false;
					}
				}else if(newA->TypeAbil==4){
					if( GSets.RoomParams.StartArtfs < MA->CommonRoomStartParam[i] ){
						add=false;
					}
				}

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
								add=false;
								break;
							}
						}
					}
				}

				if( add && AddActiveUnitAbility(OB->Index,newA) ){
					if(AddSkill){
						newA->TypeAbil=2;
					}

					int id=MA->ItemDrop[i];
					if(id!=-1){
						newA->ItemDrop=id;
					}

					if(i<MA->Names.GetAmount()){
						newA->Name=MA->Names[i]->pchar();
						OneAbility* OA=dynamic_cast<OneAbility*>(newA);
						if(OA){
							MA->be_Processes[i]->Copy(&OA->be_Process,false);
							MA->be_LeftClicks[i]->Copy(&OA->be_LeftClick,false);
							MA->be_EnterDialogs[i]->Copy(&OA->be_EnterDialog,false);
						}

						newA->DeleteAfterUse=MA->DeleteAfterUse[i];
					}

				}else{
					delete newA;
				};
			}
		}
	}
}
//UnitAbility::UnitAbility(){
//}
UnitAbility::~UnitAbility(){
	g_beOBJ_CONTROLER.SetState("UnitAbility");
}

bool UnitAbility::OnUnitBirth(OneObject* Newbie)
{
	if(Newbie){
		ActiveUnitAbility* newA=GetNewActiveAbility();
		return AddActiveUnitAbility(Newbie->Index,newA);
	} 
	return false;
}
ActiveUnitAbility* UnitAbility::GetNewActiveAbility(){
	ActiveUnitAbility* nAA=new ActiveUnitAbility;
	nAA->SetA(this);
	nAA->ItemDrop=ItemDrop;
	return nAA;
}
// by vital
void UnitAbility::CopyToActive(ActiveUnitAbility* ab){
	ab->SetA(this);
};
const char* UnitAbility::GetThisElementView(const char* LocalName){
	if(Name.pchar()){
		static char cc[256];
		Enumerator* E=ENUM.Get("UA_ClassType");
		if( E && ClassType>0 ){
			sprintf(cc,"%s (%s): {CW}%s{C}",E->GetStr(ClassType),LocalName,Name.pchar());
		}else{
			sprintf(cc,"%s: {CW}%s{C}",LocalName,Name.pchar());
		}
		return cc;
	}else return LocalName;
}
//==================================================================================================================//
UnitAbility2::UnitAbility2()
{
	UnitAbility2::UnitAbility();
}
//==================================================================================================================//
MagicSpell::MagicSpell(){
	//	operand=-1;
	sign=true;
};
int MagicSpell::CastSpell(OneObject* OB){
	return 0;
};
int MagicSpell::CancelSpell(OneObject* OB){
	return 0;
};
//==================================================================================================================//
LifeCast::LifeCast(){
	sign=true;
	name="Life";
};
int LifeCast::CastSpell(OneObject* OB){
	realchng=0;
	if(sign){
		realchng=num;
	}
	else{ 
		realchng=-num;
	};
	OB->Life+=realchng;
	OB->MaxLife+=realchng;
	return realchng;
};
int LifeCast::CancelSpell(OneObject* OB){
	OB->Life-=realchng;
	OB->MaxLife-=realchng;
	return realchng;
};
//==================================================================================================================//
AttSpeedCast::AttSpeedCast(){
	sign=true;
	name="Atack Speed";
};
int AttSpeedCast::CastSpell(OneObject* OB){
	realchng=0;
	if(sign){ 
		realchng=num;
	}
	else{ 
		realchng=-num;
	}
	//	OB-> +=realchng;
	return realchng;
};
int AttSpeedCast::CancelSpell(OneObject* OB){
	//	OB->Attack-=realchng;
	return realchng;
};
//==================================================================================================================//
MoveSpeedCast::MoveSpeedCast(){
	sign=true;
	name="Move Speed";
};
int MoveSpeedCast::CastSpell(OneObject* OB){
	realchng=0;
	if(sign){ 
		realchng=num;
		if(realchng+OB->UnitSpeed>255){
			realchng=255-OB->UnitSpeed;
		}
		if(realchng+OB->Speed>255){
			realchng=255-OB->Speed;
		}
	}
	else{ 
		realchng=-num;
		if(OB->UnitSpeed-realchng<0){
			realchng=-OB->UnitSpeed;
		}
		if(OB->Speed-realchng<0){
			realchng=-OB->Speed;
		}
	}
	OB->UnitSpeed+=realchng;
	OB->Speed+=realchng;
	return realchng;
};
int MoveSpeedCast::CancelSpell(OneObject* OB){
	OB->UnitSpeed-=realchng;
	OB->Speed-=realchng;
	return realchng;
};
//==================================================================================================================//
ShieldCast::ShieldCast(){
	sign=true;
	name="Shield";
};
int ShieldCast::CastSpell(OneObject* OB){
	realchng=0;
	if(sign){ 
		realchng=num;
	}
	else{ 
		realchng=-num;
	}
	OB->AddShield+=realchng;
	return realchng;
};
int ShieldCast::CancelSpell(OneObject* OB){
	OB->AddShield-=realchng;
	return realchng;
};
//==================================================================================================================//
BaseClassUnitType::BaseClassUnitType()
{
	UnitType=0;
}
const char* BaseClassUnitType::GetThisElementView(const char* LocalName){
	static char cc[256];
	if(LocalName){
		sprintf(cc,"%s: {CW}%s{C}",LocalName,NATIONS[0].Mon[UnitType]->MonsterID);
	}else{
		sprintf(cc,"{CG}%s{C}",NATIONS[0].Mon[UnitType]->MonsterID);
	}
	return cc;
}
//==================================================================================================================//
int UnitTypeList::GetExpansionRules()
{
	return 1;
}
UnitAbilityAura::UnitAbilityAura()
{
	AddDamage=0;
	AddShield=0;
	Cure=0;
	Radius=0;
}
bool UnitAbilityAura::OnUnitBirth(OneObject* Newbie)
{
	if(Newbie)
	{
		ActiveUnitAbilityAura* AA = new ActiveUnitAbilityAura();
		//AA->UnitIndex=Newbie->Index;
		AA->OB=Newbie;
		AA->SetA(this);
		CopyToActive(AA);
		return AddActiveUnitAbility(Newbie->Index, AA);
	}
	return false;
}
//==================================================================================================================//
ActiveUnitAbilityAura::ActiveUnitAbilityAura()
{
}
int ActiveUnitAbilityAura::GetRadius()
{
	UnitAbilityAura* UA = (UnitAbilityAura*)(GetA());
	if(UA)
	{
		return UA->Radius;
	}
	return 0;
}
bool ActiveUnitAbilityAura::Process()
{
	if(!OB)
	{
		if(GetUnitIndex()!=0xFFFF)
			OB=Group[GetUnitIndex()];
	}
	if(OB&&(!OB->Sdoxlo)&&GetA())
	{
		UnitAbilityAura* UA = (UnitAbilityAura*)(GetA());
		int pr[10];
		pr[0]=OB->NMask;
		pr[1]=(int)GetA()->Name.pchar();
		pr[2]=OB->Index;
		pr[3]=UA->Index;
		pr[4]=UA->AddDamage;
		pr[5]=UA->AddShield;
		pr[6]=UA->SumAuraEffect;
		pr[7]=UA->EnemyUnitEffect;
		pr[8]=UA->FriendlyUnitEffect;
		pr[9]=(int)(&UA->ChoiceUnitType);
		PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,UA->Radius,&ActiveUnitAbilityAura::CheckAbil,pr);
		return true;
	}
	return false;
}
bool ActiveUnitAbilityAura::CheckAbil(OneObject* OBn,void* param)
{
	int* pr = (int*)param;
	byte mas=(byte)pr[0];
	char* Nam=(char*)pr[1];
	int UnitIndex=pr[2];
	int AbilIndex=pr[3];
	int SumAura=pr[6];
	int EnE=pr[7];
	int FrE=pr[8];
	UnitTypeList* CUT=(UnitTypeList*)pr[9];
	if(OBn&&(!OBn->Sdoxlo))
	{
		bool fr=OBn->NMask&mas;
		if((fr&&FrE)||((!fr)&&EnE))
		{
			int nu=CUT->GetAmount();
			bool st=true;
			if(nu)
			{
				st=false;
				for(int j=0;j<nu;j++)
				{
					if((*CUT)[j]->UnitType==OBn->NIndex)
					{
						st=true;
						break;
					}
				}
			}
			if(st)
			{
				if(!OBn->ActiveAbility)
				{
					OBn->ActiveAbility = new UnitActiveAbilityList(OBn);
				}
				ActiveUnitAbility* PR=NULL;
				bool add=true;
				int n = OBn->ActiveAbility->ActiveAbilities.GetAmount();
				for(int i=0;i<n;i++)
				{
					if(!strcmp(Nam,OBn->ActiveAbility->ActiveAbilities[i]->Name.pchar()))
					{
						if(SumAura)
						{
							if(((AuraEffect*)OBn->ActiveAbility->ActiveAbilities[i])->Creator==UnitIndex)
							{
								add=false;
								break;
							}
						}
						else
						{
							add=false;
							break;
						}
					}
				}
				if(add)
				{
					AuraEffect* AE = new AuraEffect();
					//AE->UnitIndex=OBn->Index;
					AE->Creator=UnitIndex;
					AE->AbilityIndex=AbilIndex;
					//CopyToActive(AE);
					OBn->ActiveAbility->AddActiveUnitAbility(AE);
					OBn->AddDamage+=pr[4];
					OBn->AddShield+=pr[5];
					AE->Name=Nam;
				}
			}
		}
	}
	return true;
}
//==================================================================================================================//
UnitAbilityMagicEffect::UnitAbilityMagicEffect()
{
	WeaponModificatorName="";
}
bool UnitAbilityMagicEffect::OnUnitBirth(OneObject* OBJ)
{
	if(OBJ)
	{
		ActiveUnitAbilityMagicEffect* AA = new ActiveUnitAbilityMagicEffect();
		//AA->UnitIndex=OBJ->Index;
		AA->OB=OBJ;
		AA->SetA(this);
		AA->Visible=Visible;
		AA->HVS=GetHeroVariableStorage(OBJ);
		int n=Variables.GetAmount();
		for(int i=0;i<n;i++)
		{
			HeroVariable* HV = new HeroVariable();
			HV->Name=Variables[i]->Name.pchar();
			HV->Value=Variables[i]->Value;
			AA->HVS->Variables.Add(HV);
		}
		CopyToActive(AA);
		return AddActiveUnitAbility(OBJ->Index, AA);
	}
	return false;
}
//==================================================================================================================//
ActiveUnitAbilityMagicEffect::ActiveUnitAbilityMagicEffect()
{
	Tx=-1;
	Ty=-1;
	Tz=-1;
	Target=0xFFFF;
	CoolDownTime=0;
	OnOff=false;
	HVS=NULL;
	CheckHero=false;
	TempRadius=-1;
}
bool ActiveUnitAbilityMagicEffect::Process()
{
	if((!HVS)&&(!CheckHero))
	{
		HVS=GetHeroVariableStorage(OB);
		CheckHero=true;
	}
	CurrentHeroAbility=HVS;
	if((!OB)&&GetUnitIndex()!=0xFFFF)
		OB=Group[GetUnitIndex()];
	if(OB&&!OB->Sdoxlo)
	{
		if(AnimTime>=CoolDownTime)CoolDownTime=0;


		UnitAbilityMagicEffect* A = (UnitAbilityMagicEffect*) GetA();
		if(A)
		{
			Hint.Clear();
			Hint.Add(GetTextByID(Name.pchar()));
			if(!CoolDownTime)
			{
				Hint.Add("\\Damage: ");
				Hint.Add(A->Damage.Get());
				Hint.Add(" Mana: ");
				Hint.Add(A->ManaCost.Get());
			}
			else
			{
				Hint="\\CoolDown: ";
				Hint.Add(((CoolDownTime-AnimTime)*100)/(A->CoolDownTime.Get()*25*256));
				Hint.Add("%");
			}

			if(OB->DestX<1&&A->ContinueUse&&Tx!=-1&&CanTarget(Target,Tx,Ty,Tz)&&OB->Mana>=A->ManaCost.Get())
			{
				if((AnimTime-UseTime)>=A->UsePause.Get()*25*256/10)
				{
					ApplayWeapon();
				}
			}
			else
			{
				Tx=-1;
			}
			if(A->ManualOnOff&&OnOff&&OB->Mana>=A->ManaCost.Get())
			{
				if((AnimTime-UseTime)>=A->UsePause.Get()*25*256/10)
				{
					ApplayWeapon();
				}
			}
			if(A->TipaAura)
			{
				if((AnimTime-UseTime)>=A->UsePause.Get()*25*256/10)
				{
					ApplayWeapon();
				}
			}
		}
	}
	return true;
}
bool ActiveUnitAbilityMagicEffect::isTarget()
{
	UnitAbilityMagicEffect* A = (UnitAbilityMagicEffect*) GetA();
	if(A)
	{
		return A->NeedTarget;
	}
	return false;
}
bool ActiveUnitAbilityMagicEffect::CanTarget(word TargetUnit, int x, int y, int z)
{
	UnitAbilityMagicEffect* A = (UnitAbilityMagicEffect*) GetA();
	if(OB&&A&&A->NeedTarget&&CoolDownTime==0)
	{
		int sx=OB->RealX>>4;
		int sy=OB->RealY>>4;
		int Ds=Norma(sx-x,sy-y);
		if(Ds>=A->MinDist.Get()&&Ds<A->MaxDist.Get())
		{
			if(A->EnemyUnitTarget||A->FriendlyUnitTarget)
			{
				if(TargetUnit!=0xFFFF)
				{
					OneObject* OT = Group[TargetUnit];
					if(OT)
					{
						if(OB->NMask&OT->NMask)
						{
							if(A->FriendlyUnitTarget)
								return true;
						}
						else
						{
							if(A->EnemyUnitTarget)
								return true;
						}
					}
				}
			}
			else
				return true;
		}
	}
	return false;
}
bool ActiveUnitAbilityMagicEffect::Execute(word TargetUnit, int x, int y, int z)
{
	bool rez=false;
	CurrentHeroAbility=HVS;
	UnitAbilityMagicEffect* A = (UnitAbilityMagicEffect*) GetA();
	if(CanTarget(TargetUnit,x,y,z)&&OB&&A&&OB->Mana>=A->ManaCost.Get())
	{
		Tx=x;
		Ty=y;
		Tz=z;
		Target=TargetUnit;
		if(Tz==0||Tz==-1)Tz=GetHeight(x,y);
		if(ApplayWeapon())
		{
			CoolDownTime=AnimTime+A->CoolDownTime.Get()*25*256;
			rez=true;
		}
	}
	TempRadius=-1;
	return rez;
}
bool ActiveUnitAbilityMagicEffect::OnClick()
{
	bool rez=false;
	UnitAbilityMagicEffect* A = (UnitAbilityMagicEffect*) GetA();
	if(OB&&A&&A->ManualOnOff)
	{
		OnOff=!OnOff;
		rez=true;
	}
	if(OB&&(!A->ManualOnOff)&&(!A->NeedTarget)&&CoolDownTime==0&&OB->Mana>=A->ManaCost.Get())
	{
		Tx=OB->RealX>>4;
		Ty=OB->RealY>>4;
		Tz=OB->RZ;
		Target=0xFFFF;
		CurrentHeroAbility=HVS;
		if(ApplayWeapon())
		{
			CoolDownTime=AnimTime+A->CoolDownTime.Get()*25*256;
			rez=true;
		}
	}
	return rez;
}
bool ActiveUnitAbilityMagicEffect::DrawCursor(int x,int y)
{
	if(TempRadius==-1)
	{
		TempRadius=0;
		UnitAbilityMagicEffect* BA = (UnitAbilityMagicEffect*)GetA();
		if(BA)
		{
			TempRadius=BA->Radius.Get();
		}
	}

	return true;
}
bool ActiveUnitAbilityMagicEffect::ApplayWeapon()
{
	bool rez=false;
	UnitAbilityMagicEffect* A = (UnitAbilityMagicEffect*) GetA();
	if(OB&&A)
	{
		int sx=OB->RealX>>4;
		int sy=OB->RealY>>4;
		int sz=OB->RZ;
		if(A->SetPointTargetPoint)
		{
			sx=Tx;
			sy=Ty;
			sz=Tz;
		}
		AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
		AWP->Damage=A->Damage.Get();
		AWP->AttType=A->AttType;
		AWP->Radius=A->Radius.Get();
		AWP->N=A->N.Get();
		if(A->EnemyUnitTarget||A->FriendlyUnitTarget)
		{
			if(Target!=0xFFFF)
			{
				OneObject* OT = Group[Target];
				if(OT)
				{
					CreateNewActiveWeapon(A->WeaponModificatorName.pchar(),GetUnitIndex(),sx,sy,sz,Target,Tx,Ty,Tz,AWP);
				}
			}
		}
		else
		{
			CreateNewActiveWeapon(A->WeaponModificatorName.pchar(),GetUnitIndex(),sx,sy,sz,Target,Tx,Ty,Tz,AWP);
		}
		UseTime=AnimTime;
		OB->Mana-=A->ManaCost.Get();
	}
	return rez;
}
//==================================================================================================================//
VampiricAbility::VampiricAbility()
{
	LifeProc=0;
}
bool VampiricAbility::OnUnitBirth(OneObject* OBJ)
{
	if(OBJ)
	{
		ActiveVampiricAbility* ab = new ActiveVampiricAbility();
		//ab->UnitIndex=OBJ->Index;
		ab->OB=OBJ;
		ab->Visible=Visible;
		ab->SetA(this);
		CopyToActive(ab);
		return AddActiveUnitAbility(OBJ->Index, ab);
	}
	return false;
}
//==================================================================================================================//
bool ActiveVampiricAbility::OnMakeDamage(OneObject* Take,int& Damage)
{
	if(Take&&OB&&(!OB->Sdoxlo)&&GetA()&&Take->NewBuilding!=1)
	{
		OB->Life+=(Take->MaxLife*((VampiricAbility*)GetA())->LifeProc)/100;
		if(OB->Life>OB->MaxLife)
			OB->Life=OB->MaxLife;
		return true;
	}
	return false;
}
bool ActiveVampiricAbility::Process()
{
	if((!OB)&&GetUnitIndex()!=0xFFFF)
		OB=Group[GetUnitIndex()];
	if(OB&&(!OB->Sdoxlo))
		return true;
	return false;
}
//==================================================================================================================//
bool BlizardAbility::OnUnitBirth(OneObject* OBJ)
{
	if(OBJ)
	{
		ActiveBlizardAbility* ab = new ActiveBlizardAbility();
		//ab->UnitIndex=OBJ->Index;
		ab->OB=OBJ;
		ab->Visible=Visible;
		ab->SetA(this);
		ab->HVS=GetHeroVariableStorage(OBJ);
		if(ab->HVS)
		{
			int n=Variables.GetAmount();
			for(int i=0;i<n;i++)
			{
				HeroVariable* HV = new HeroVariable();
				HV->Name=Variables[i]->Name.pchar();
				HV->Value=Variables[i]->Value;
				ab->HVS->Variables.Add(HV);
			}
		}
		CopyToActive(ab);
		return AddActiveUnitAbility(OBJ->Index, ab);
	}
	return false;
}
//==================================================================================================================//
ActiveBlizardAbility::ActiveBlizardAbility()
{
	Tx=-1;
	Ty=-1;
	InUseTime=-1;
	CoolDownTime=0;
	Hint="";
	CheckHero=false;
	HVS=NULL;
	TempRadius=-1;
}
bool ActiveBlizardAbility::Process()
{
	if((!OB)&&GetUnitIndex()!=0xFFFF)
		OB=Group[GetUnitIndex()];
	if((!HVS)&&(!CheckHero))
	{
		HVS=GetHeroVariableStorage(OB);
		CheckHero=true;
	}
	CurrentHeroAbility=HVS;

	BlizardAbility* A = (BlizardAbility*)GetA();
	if(OB&&!OB->Sdoxlo&&A)
	{
		if(AnimTime>=CoolDownTime)CoolDownTime=0;

		if(!CoolDownTime)
		{
			Hint="AcidicRain\\Damage: ";
			Hint.Add(A->Damage.Get());
			Hint.Add(" Mana: ");
			Hint.Add(A->ManaCost.Get());
		}
		else
		{
			Hint="AcidicRain\\CoolDown: ";
			Hint.Add(((CoolDownTime-AnimTime)*100)/(A->CoolDownTime.Get()*25*256));
			Hint.Add("%");
		}
		if(Tx!=-1)
		{
			if(OB->DestX<1&&OB->Mana>=A->ManaCost.Get())
			{
				if((AnimTime-InUseTime)>=(A->DamagePause*25*256/10))
				{
					AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
					AWP->Damage=A->Damage.Get();
					AWP->AttType=A->AttType;
					AWP->Radius=A->Radius.Get();
					CreateNewActiveWeapon(A->EffectName.pchar(),GetUnitIndex(),Tx,Ty,Tz,0xFFFF,Tx,Ty,Tz,AWP);
					InUseTime=AnimTime;
					OB->Mana-=A->ManaCost.Get();
				}			
			}
			else
			{
				Tx=-1;
			}
		}
	}
	return true;
}
bool ActiveBlizardAbility::isTarget()
{
	return true;
}
bool ActiveBlizardAbility::CanTarget(word TargetUnit, int x, int y, int z)
{
	bool rez=false;
	BlizardAbility* A = (BlizardAbility*)GetA();
	if(OB&&(!OB->Sdoxlo)&&CoolDownTime==0&&A)
	{
		int ds = Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
		if(ds<A->MaxDist)
			rez=true;
	}
	return rez;
}
bool ActiveBlizardAbility::Execute(word TargetUnit, int x, int y, int z)
{
	bool rez=CanTarget(TargetUnit,x,y,z);
	if(rez)
	{
		BlizardAbility* A = (BlizardAbility*)GetA();
		CurrentHeroAbility=HVS;
		if(OB->Mana>=A->ManaCost.Get())
		{
			Tz=GetHeight(x,y)+300;
			Tx=x;
			Ty=y;
			AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
			AWP->Damage=A->Damage.Get();
			AWP->AttType=A->AttType;
			AWP->Radius=A->Radius.Get();
			CreateNewActiveWeapon(A->EffectName.pchar(),GetUnitIndex(),Tx,Ty,Tz,0xFFFF,Tx,Ty,Tz,AWP);
			InUseTime=AnimTime;
			CoolDownTime=AnimTime+A->CoolDownTime.Get()*25*256;
			OB->Mana-=A->ManaCost.Get();
		}
	}
	TempRadius=-1;
	return rez;
}
bool ActiveBlizardAbility::DrawCursor(int x,int y)
{
	if(TempRadius==-1)
	{
		TempRadius=0;
		BlizardAbility* BA = (BlizardAbility*)GetA();
		if(BA)
		{
			TempRadius=BA->Radius.Get();
		}
	}

	return true;
}
//==================================================================================================================//
bool _util_LoadClassLikeFolder(BaseClass* BC,const char* xmlName,const char* FieldName);
void _util_SaveClassLikeFolder(BaseClass* BC,const char* xmlName,const char* FieldName);
bool ArrayAbilities::SafeReadFromFile(const char* Name){
	_util_LoadClassLikeFolder(this,Name,"Name");
	return true;
}
bool ArrayAbilities::WriteToFile(const char* Name){
	_util_SaveClassLikeFolder(this,Name,"Name");
	return true;    
}
bool ArrayAbilities::ReadFromFile(const char* Name){
	_util_LoadClassLikeFolder(this,Name,"Name");
	return true;
}
bool AbilityList::LoadAbilities(char* FileName){
	bool rez=false;
	if(FileName){
		rez=Abilities.SafeReadFromFile(FileName);
	}

	int n=Abilities.GetAmount();
	for(int i=0;i<n;i++){
		Abilities[i]->Index=i;
	}

	return rez;
}
//==================================================================================================================//
int ArrayAbilities::GetExpansionRules()
{
	return 2;
}
//==================================================================================================================//
MonsterAbility::MonsterAbility()
{
	Feeled=false;
}
void MonsterAbility::Init(NewMonster* NM){
	if(!Feeled){
		int n = AbilityNames.GetAmount();
		AbilitiesList.Clear();
		for(int i=0;i<n;i++){
			UnitAbility* UA=NULL;
			char* name=AbilityNames[i]->pchar();
			if(name){
				int m = Abilities.Abilities.GetAmount();
				for(int j=0;j<m;j++){
					if(!stricmp(name,Abilities.Abilities[j]->Name.pchar())){
						UA=Abilities.Abilities[j];
						break;
					}
				}
			}
			AbilitiesList.Add(UA);
		}
		Feeled=true;
	}
};
bool MonsterAbility::Process(int UnitIndex)
{	
	/*
	int n = AbilitiesList.GetAmount();
	for(int i=0;i<n;i++)
	{
	if(AbilitiesList[i]->AutoCast)
	AbilitiesList[i]->Execute(UnitIndex, 0xFFFF, -1, -1, -1);
	}
	*/
	return true;
}
//==================================================================================================================//
ActiveUnitAbility::~ActiveUnitAbility(void)
{
	if(IconInfo)
	{
		delete IconInfo;
		IconInfo=NULL;
	}
}
bool ActiveUnitAbility::Process()
{
	bool rez=false;
	if((!OB)&&GetUnitIndex()!=0xFFFF)
	{
		OB=Group[GetUnitIndex()];
	}
	if(OB)
		rez=Process(OB);
	return rez;
}
bool ActiveUnitAbility::Process(OneObject* OB)
{
	return false;
}
bool ActiveUnitAbility::OnClick()
{
	return false;
}
bool ActiveUnitAbility::OnRightClick()
{
	return false;
}
bool ActiveUnitAbility::Execute(word TargetUnit, int x, int y, int z)
{
	return true;
}
bool ActiveUnitAbility::CanApply()
{
	return true;
}
bool ActiveUnitAbility::isTarget()
{
	return false;
}
bool ActiveUnitAbility::CanTarget(word TargetUnit, int x, int y, int z)
{
	return true;
}
bool ActiveUnitAbility::isActive()
{
	return false;
}
bool ActiveUnitAbility::OnMakeDamage(OneObject* Take,int& Damage)
{
	return false;
}
bool ActiveUnitAbility::DrawCursor(int x,int y)
{
	return false;
}
bool ActiveUnitAbility::IsHero()
{
	return false;
}
int ActiveUnitAbility::GetRadius()
{
	return 0;
}
bool ActiveUnitAbility::ShowRadius()
{
	if(OB)
	{
		int R=GetRadius();
		if(R)
		{
			AddGroundCircle(OB->RealX>>4,OB->RealY>>4,R,0xFFFFFF00);
			return true;
		}
	}
	return false;
}
bool ActiveUnitAbility::CanYouAddToObject(OneObject* OB,void* Param)
{
	return false;
}
bool ActiveUnitAbility::AddToObject(OneObject* OB,void* Param)
{
	if(FillParam(OB, Param))
	{
		return AddToObject(OB);
	}
	return false;
}
bool ActiveUnitAbility::AddToObject(OneObject* B)
{
	OB=B;
	//UnitIndex=B->Index;
	if(!OB->ActiveAbility)
	{
		B->ActiveAbility = new UnitActiveAbilityList(B);	
	}
	return B->ActiveAbility->AddActiveUnitAbility(this);
}
bool ActiveUnitAbility::FillParam(OneObject* OB, void* Param)
{
	return false;
}
ActiveUnitAbility* ActiveUnitAbility::GetActionAbilityExample()
{
	return NULL;
}
int ActiveUnitAbility::GetUsePause()
{
	return 0;
}
int ActiveUnitAbility::AddActionAbilityOverUnitsInRadius()
{
	int rez=0;
	int R=GetRadius();
	if(OB&&AnimTime>(GetUsePause()*25*256/10+UseTime)&&GetCoolDownProc()==0&&R)
	{
		rez=PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,R,&ActiveUnitAbility::ApplyActionAbility,(void*)this);
		UseTime=AnimTime;
	}
	return rez;
}

ActiveUnitAbility* CurProcAA=NULL; // примен€етьс€ в BHP_AbilityEffectTime

SimpleDialog* sdAblHotKey=NULL;
const char* ActiveUnitAbility::GetHint() {
	static DString H;
	static DWORD LastSerial=0xFFFFFFFF;
	if(LastSerial==Serial){
		return H.pchar();
	}
	LastSerial=Serial;
	H.Clear();

	UnitAbility* UA=GetA();
	if(UA){
		UnitAbility* HintUA=UA->HintUA.Get();
		mod_Symbol* M=NULL;
		itr_UM.Create(this,0xFFFFFFFF);
		while(UnitModificator* m=itr_UM.Next()){
			M=dynamic_cast<mod_Symbol*>(m);
			if(M)
				break;
		};
		static bool init=false;
		if(!init){
			init=true;
			CE_WCP.SetStdPresentation();
		}
		//if(v_ISys.DebugHint){
		//	_str ModStr="";
		//	CE_WCP.GetPresentation(ModStr,&UA->Modificators);
		//	h.Clear();
		//	h+=U->Name.pchar();
		//	h+="\n";
		//	h+=ModStr;
		//	h+="\n";
		//	h+=hn;
		//}
		int FID=M?M->FileID:UA->FileID;
		int SID=M?M->SpriteID:UA->SpriteID;
		if( TypeAbil==0 && UA->Type!=2 ){
			FID=UA->EffectFileID;
			SID=UA->EffectSpriteID;
		}
		int lx=GPS.GetGPWidth(FID,SID);
		int ly=GPS.GetGPHeight(FID,SID);
		// «аголовок абилки
		DString Title=UA->HintName;
		if( sdAblHotKey && sdAblHotKey->HotKey!=0 && UA->Button ){
			char* GetKeyName(word Key);
			Title+=" ({C FFBCBCBC}";
			if(sdAblHotKey->HotCtrl){
				Title+="Ctrl+";
			}
			Title.print("%s{C FFFFCB2B})",GetTextByID(GetKeyName(sdAblHotKey->HotKey)));
		}
		//DString Descr=UA->Hint;
		DString Descr=Hint.pchar();
		if(M){
			Title=M->HintName;
			Descr=M->Hint;
		}
		H.print("{rframe {G %d %d 0 0 %d %d}}{Fgr14}{C FFFFCB2B}%s\n{dy6}",FID,SID,lx,ly,Title.pchar());
		// стоимость абилки дл€ каста
		if(TypeAbil!=0){
			if( UA->ManaValue || UA->MoneyValue ){
				H+="{Fsm01}";
				int MV=UA->ManaValue; 
				if(MV){
					OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);
					//h+="\n";
					static char* cManaCost=GetTextByID("#ManaCost");
					if(OB->Mana>=MV){
						H.print("{C 9FBCBCBC}%s: {C FFB39A58}%d",cManaCost,MV);
					}else{
						H.print("{C 9FBCBCBC}%s: {C FFFF0000}%d",cManaCost,MV);
					}
				}
				MV=UA->MoneyValue;
				if(MV){
					OB->ActiveAbility->ActiveAbilities.modifyManaValue(MV,MV);
					//h+="\n";
					static char* cMoneyCost=GetTextByID("#MoneyCost");
					int Money=XRESRC(OB->NNUM,MoneyID);
					if(Money>=MV){
						H.print(" {C 9FBCBCBC}%s: {C FFB39A58}%d",cMoneyCost,MV);
					}else{
						H.print(" {C 9FBCBCBC}%s: {C FFFF0000}%d",cMoneyCost,MV);
					}
				}
				H+="\n{dy6}";
			}
		}
		// описание абилки
		H.print("{Ftm12}{C FFB5B09C}%s\n{dy6}",Descr.pchar());
		// действие еффекта абилки
		DString ER;
		DString ED;
		DString EN;
		if( HintUA && TypeAbil!=0 ){
			CurProcAA=this;
			OneObject* COB=GlobalUnitsContext::getCaster();
			GlobalUnitsContext::setCaster(OB->Index,OB->Serial);
			itr_UM.Create(HintUA);
			if(COB)
				GlobalUnitsContext::setCaster(COB->Index,COB->Serial);
		}else{
			itr_UM.Create(this,0xFFFFFFFF);
		}
		while(UnitModificator* M=itr_UM.Next()){
			static _str e;
			CE_WCP.GetPresentation(e,M);
			if( !e.isClear() && e.pchar()[3]!='_' ){
				enActionType AT=M->GetActionType();
				switch(AT){
				case atRise:
					ER.print("%s\n",e.pchar());
					break;
				case atDecrease:
					ED.print("%s\n",e.pchar());
					break;
				case atNeutral:
					EN.print("%s\n",e.pchar());
					break;
				}
			}
		}
		//if(!UA->UnlimitedAmount){
		//	static char* sch=GetTextByID("#LimitedAmount");
		//	EN.print("%s\n",sch);
		//}
		if(UA->SellAfterUse){
			static char* sch=GetTextByID("#SellAfterUse");
			EN.print("%s\n",sch);
		}else if( TypeAbil==1 && !(UA->Aura||UA->Type==4) && DetectHero(OB) ){
			static char* sch=GetTextByID("#SkipMagicCard");
			EN.print("%s\n",sch);
		}
		if(UA->Permanent){
			static char* sch=GetTextByID("#Permanent");
			EN.print("%s\n",sch);
		}
		bool er=!ER.isClear();
		bool ed=!ED.isClear();
		bool en=!EN.isClear();
		if(er||ed||en){
			static char* Influence=GetTextByID("#Influence");
			H.print("{Fsm01}{C 9FBCBCBC}%s:\n{Ftm12}{dy6}",Influence);
			if(er){
				H.print("{C FF00F300}%s\n",ER.pchar());
			}
			if(ed){
				H.print("{C FFFF0000}%s\n",ED.pchar());
			}
			if(en){
				H.print("{C FFFFF200}%s\n",EN.pchar());
			}
		}
		//h=hh;
		//if( !U->Aura && U->MoveToEndAfterUse && UA->Clothed ){
		//	h+="\n \n";
		//	h+=v_ISys.HintSkipMagicCard;
		//	h+="\n \n";
		//}
		// стоимость абилки дл€ купли-продажи
		if(TypeAbil!=0){
			bool InLib=OB->ActiveAbility->LibID!=0xFFFF;
			if( InLib || UA->SellAfterUse /*&& DetectHero(OB)*/ ){
				if(UA->ShopPrice){
					H+="{dy6}{Fsm01}";
					if(InLib){
						static char* cBuyPrice=GetTextByID("#BuyPrice");
						int GetAbilityBuyPrice(OneObject* Buyer, ActiveUnitAbility* AA);
						int price = GetAbilityBuyPrice(OB,this);
						int Money=XRESRC(OB->NNUM,MoneyID);
						if(Money>=price){
							H.print("{C 9FBCBCBC}%s: {C FFB39A58}%d ",cBuyPrice,price);
						}else{
							H.print("{C 9FBCBCBC}%s: {C FFFF0000}%d ",cBuyPrice,price);
						}
					}
					static char* cSellPrice=GetTextByID("#SellPrice");
					int GetAbilitySellPrice(OneObject* Seller, ActiveUnitAbility* AA);
					int price = GetAbilitySellPrice(OB,this);
					if(UA->SellAfterUse)
						price*=1+GSets.RoomParams.StartRes*2;
					H.print("{C 9FBCBCBC}%s: {C FFB39A58}%d\n",cSellPrice,price);
				}
			}
			//if( UA->Type==0 && UA->Amount ){
			//	char* cCharges="Charges";
			//	H.print("{C 9FBCBCBC}%s: {C FFB39A58}%d\n",cCharges,Amount);
			//}
		}else{
			if(EffectCount>1){
				static char* cMulticast=GetTextByID("#Multicast");
				H.print("{dy6}{Fsm01}{C 9FBCBCBC}%s %d",cMulticast,EffectCount);
			}
		}
	}else{
		H.print("{Ftm14}{C FF00F300}%s",GetTextByID(Hint.pchar()));
	}
	return H.pchar();
}
bool UnitAbility::GetSprite(ActiveUnitAbility* AA, int& File, int& Spr){
	int n=Sprite.GetAmount();
	for(int i=0;i<n;i++){
		ModifySprite* MS=Sprite[i];
		if(MS){
			if( MS->Condition.Check(AA) ){
				File=MS->FileID;
				Spr=MS->SpriteID;
				return true;
			};
		}
	}
	return false;
};
void ActiveUnitAbility::GetButton(int& FileID, int& SpriteID, int& FileID_Char, int& FileID_Inv){
	FileID=this->FileID;
	SpriteID=this->SpriteID;
	UnitAbility* UA=GetA();
	if(UA){
		if( TypeAbil==1 || TypeAbil==2 ){
			FileID_Char=UA->FileID;
			FileID_Inv=UA->FileID;
		}else{
			FileID_Char=UA->FileID_Char;
			FileID_Inv=UA->FileID_Inv;
		}
		if(EnableStates){
			//ua_State* S=UA->States[CurState];
			//if(S){
			//	FileID=S->Button.FileID;
			//	SpriteID=S->Button.SpriteID;
			//	Hint=S->Button.Hint;
			//}
		}else{
			if( !UA->GetSprite(this,FileID,SpriteID) ){
				if(TypeAbil==0){
					FileID=UA->EffectFileID;
					SpriteID=UA->EffectSpriteID;
				}
				itr_UM.Create(this,0xFFFFFFFF);
				while(UnitModificator* m=itr_UM.Next()){
					mod_Symbol* M=dynamic_cast<mod_Symbol*>(m);
					if(M){
						FileID=M->FileID;
						SpriteID=M->SpriteID;
						if( M->FileID_Char==0 && M->FileID_Inv==0 ){
							const char* fn=GPS.GetGPName(M->FileID);
							if(fn){
								DString str;
								str=fn;
								str.Replace0("goods_","goods_character_");
								M->FileID_Char=GPS.PreLoadGPImage(str.pchar());
								str=fn;
								str.Replace0("goods_","goods_inventory_");
								M->FileID_Inv=GPS.PreLoadGPImage(str.pchar());
							}
						}
						FileID_Char=M->FileID_Char;
						FileID_Inv=M->FileID_Inv;
						break;
					}
				};
				//itr_UM.Create(this,ABL_Symbol);
				//while( mod_Symbol* M=(mod_Symbol*)itr_UM.Next() ){
				//	FileID=M->FileID;
				//	SpriteID=M->SpriteID;
				//	if( M->FileID_Char==0 && M->FileID_Inv==0 ){
				//		const char* fn=GPS.GetGPName(M->FileID);
				//		if(fn){
				//			DString str;
				//			str=fn;
				//			str.Replace0("goods_","goods_character_");
				//			M->FileID_Char=GPS.PreLoadGPImage(str.pchar());
				//			str=fn;
				//			str.Replace0("goods_","goods_inventory_");
				//			M->FileID_Inv=GPS.PreLoadGPImage(str.pchar());
				//		}
				//	}
				//	FileID_Char=M->FileID_Char;
				//	FileID_Inv=M->FileID_Inv;
				//}
			}
		}
	}
}

//bool AbilityTrade(){
//}

int GetAbilityBuyPrice(OneObject* Buyer, ActiveUnitAbility* AA){
	int TradeSkill=100;
	Buyer->ActiveAbility->ActiveAbilities.modifyTradeSkill(TradeSkill,TradeSkill);
	UnitAbility* UA=AA->GetA();
	if( UA ){
		int price=UA->ShopPrice*100/TradeSkill;
		if( UA->Type==0 && UA->Amount>0&&UA->Consumable){
			price/=UA->Amount;
		}
		return price;
	}
    return -1;
}

int GetAbilitySellPrice(OneObject* Seller, ActiveUnitAbility* AA){
	int TradeSkill=100;
	Seller->ActiveAbility->ActiveAbilities.modifyTradeSkill(TradeSkill,TradeSkill);
	UnitAbility* UA=AA->GetA();
	if( UA ){
		int price=UA->ShopPrice*TradeSkill/100*UA->SellMultiplier;
		if( UA->Type==0 && UA->Amount>0&&UA->Consumable){
			price=price*AA->Amount/UA->Amount;
		}
		return price;
	}
	return -1;
}

bool ActiveUnitAbility::TryClothe(bool ClotheState, int SlotID, bool ErrorHint){
	return false;
};

bool ActiveUnitAbility::SeekSameAndClothe(){
	UnitAbility* UA=GetA();
	ActiveUnitAbility* AANotClothed=NULL;
	int AASlot=-1;
	itr_AA.Create(OB);
	while(ActiveUnitAbility* aA=itr_AA.Next()){
		if( aA!=this && aA->TypeAbil==TypeAbil ){
			UnitAbility* uA=aA->GetA();
			if(uA==UA){
				if(aA->Clothed){
					AANotClothed=NULL;
					break;
				}else if( AASlot<aA->Slot || !AANotClothed ){
					AANotClothed=aA;
				}
			}
		}
	}
	if(AANotClothed){
		AANotClothed->Clothed=true;
		AANotClothed->Slot=Slot;
		return true;
	}
	return false;
}

bool ActiveUnitAbility::SetClothe(bool ClotheState, int SlotID){
	UnitAbility* UA=GetA();
	if(UA){
		if(UA->CanNotBeClothe)
			return false;
		if( Clothed!=ClotheState ){
			if(ClotheState){
				if(TypeAbil==4){
					if(UA->Conditions.Check(this)){
						int n=vClotheSet[UA->TypeClothe];
						if(n>0){
							ActiveUnitAbility* TakeOff=NULL;
							ActiveUnitAbility* FirstSlot=NULL;
							int i=0;
							itr_AA.Create(OB);
							while(ActiveUnitAbility* AA=itr_AA.Next()){
								if( AA->TypeAbil==4 && AA!=this ){
									UnitAbility* uA=AA->GetA();
									if(uA->TypeClothe==UA->TypeClothe){
										if(AA->Clothed){
											i++;
											//if(i>n){
											//	//AA->Clothed=false;
											//	AA->Slot=-1;
											//}
											if( AA->Slot==0 ){
												FirstSlot=AA;
											}
											if( SlotID!=-1 && SlotID==AA->Slot /*|| AA->Slot==0*//*n==1*/ ){
												//if( !TakeOff )
												TakeOff=AA;
											}
										}
									}
								}
							}
							if( TakeOff ){
								TakeOff->Clothed=false;
								TakeOff->Slot=Slot;
								if(SlotID==-1)
									SlotID=TakeOff->Slot;
							}else{
								if( i>=n ){
									if( FirstSlot ){
										FirstSlot->Clothed=false;
										FirstSlot->Slot=Slot;
										SlotID=0;
									}else
										return false;
								}
							}
						}
					}
				}else if(TypeAbil==5){
					ActiveUnitAbility* PrevSlotAA=NULL;
					//addrand(SlotID);
					itr_AA.Create(OB);
					while(ActiveUnitAbility* AA=itr_AA.Next()){
						if( AA->TypeAbil==5 && AA->Clothed ){
							if( AA->GetA()==UA ){
								return false;
							}
							if( AA->Slot==SlotID ){
								PrevSlotAA=AA;
							}
						}
					}
					if( PrevSlotAA ){
						PrevSlotAA->Clothed=false;
						PrevSlotAA->Slot=Slot;
					}
				}
				NewItem* NI=dynamic_cast<NewItem*>(UA);
				if( NI && NI->SoundDress!=-1 && OB->NNUM==MyNation )
					PlayEffect(NI->SoundDress,0,0);
				ext_OnPutOnItem(OB->Index,this);
			}else{
				NewItem* NI=dynamic_cast<NewItem*>(UA);
				if( NI && NI->SoundUndress!=-1 && OB->NNUM==MyNation )
					PlayEffect(NI->SoundUndress,0,0);
				ext_OnPutOffItem(OB->Index,this);
			}
			Clothed=ClotheState;
		}
		Slot=SlotID;
		return true;
	}
	return false;
};

bool ActiveUnitAbility::ApplyActionAbility(OneObject* OB,void* Param)
{
	bool rez=false;
	if(OB&&Param)
	{
		ActiveUnitAbility* A = (ActiveUnitAbility*)Param;
		ActiveUnitAbility* Ex = A->GetActionAbilityExample();
		if(Ex)
		{
			if(Ex->CanYouAddToObject(OB,Param))
			{
				ActiveUnitAbility* N = (ActiveUnitAbility*) Ex->new_element();
				rez=N->AddToObject(OB,Param);
				if(!rez)
					delete N;
			}
		}
	}
	return rez;
}
//UnitAbility* ActiveUnitAbility::GetA()
//{
//	if(BaseUA==NULL){
//		BaseUA=UnitAbilityRef.Get();
//	}
//	return BaseUA;
//}
void ActiveUnitAbility::SetA(UnitAbility* A)
{
	FileID=A->FileID;
	SpriteID=A->SpriteID;
	Hint.Clear();
	///if(A->HintName) Hint+=A->HintName; //Hint=GetTextByID(A->Hint);
	if(A->Hint) Hint+=A->Hint;
	//
	UnitAbilityRef.Set(A);
}
UnitAbility* ActiveUnitAbility::GetAW()
{
	UnitAbility* rez=UnitAbilityPtr.Get();
	if((!rez)&&UnitAbilityRef.Get())
	{
		UnitAbility* UCopy =(UnitAbility*) UnitAbilityRef.Get()->new_element();
		if(UCopy)
		{
			UnitAbilityRef.Get()->Copy(UCopy);
			UnitAbilityPtr.Set(UCopy);
			rez=UCopy;
		}
	}
	return rez;
}
UnitAbilityIcon* ActiveUnitAbility::GetUnitAbilityIconInfo()
{
	if(Visible)
	{
		if(!IconInfo)
		{
			IconInfo = new UnitAbilityIcon();
			IconElement* IE = new IconElement();
			IE->FileID=FileID;
			IE->SpriteID=SpriteID;
			IE->Hint=Hint.pchar();
			IconInfo->AddElement(IE);
		}
		return IconInfo;
	}
	return NULL;
}
//==================================================================================================================//
CoolDown::CoolDown()
{
	Type=5;
	Name="CoolDown";
	UnitAbilityIndex=-1;
	Value=0;
	LastProcesedTime=TrueTime;
}
CoolDown::CoolDown(int UAbilityIndex, int V)
{
	Type=5;
	Name="CoolDown";
	UnitAbilityIndex=UAbilityIndex;
	Value=V;
	LastProcesedTime=TrueTime;
}
bool CoolDown::Process()
{
	int in=TrueTime-LastProcesedTime;
	Value-=in;
	if(Value<0)Value=0;
	LastProcesedTime=TrueTime;
	if(Value)return true;
	return false;
}
//==================================================================================================================//
void UnitActiveAbilityList::Init(){
	BaseClass::reset_class(this);
	OB=NULL;

	NewNMask=0;
	MaskWasEdited=0;

	BornNIndex=0xFFFF;
	BornNNUM=0xFF;
	BornNMask=0;
	BornKillMask=0xFF;
	BornFlyHeight=0;

	ModelAnimationNIndex==0xFFFF;
	//Hero=NULL;
	Transparency=0xFF;
	TransparencyLast=0xFF;
	LibID=0xFFFF;
}
UnitActiveAbilityList::UnitActiveAbilityList(){
	Init();
}
UnitActiveAbilityList::UnitActiveAbilityList(OneObject* Ob){
	Init();
	OB=Ob;
	SetBornNIndex();
	TransparencyStartTime=TrueTime;
}
void UnitActiveAbilityList::SetBornNIndex(){
	BornNIndex=OB->NIndex;
	ModelAnimationNIndex=BornNIndex;
	BornNNUM=OB->NNUM;
	BornNMask=OB->NMask;
	BornNM=OB->newMons;
	BornAC=OB->MoreCharacter;
	BornKillMask=OB->newMons->KillMask;
};

void ActiveUnitAbility::SetVisible(bool State){
	Visible=State;
};

DLLEXPORT bool GetBuildingEntryPoint(int* x,int* y,word Index,int AddDistance);
bool GetHero(OneObject* OB);
void MarkObjectLikeModified(OneObject* OB);
void MarkObjectLikeModified(OneObject* OB, bool InitAbility);

bool UnitActiveAbilityList::PostProcess() {

	//Transparency
	if(TrueTime-TransparencyStartTime>255){
		byte Tr=0xFF;
		uaa_Chest* Ch=DetectChest(OB);
		if(Ch){
			if(Ch->Empty){
				Tr=0;
			}
		}else{
			byte mask=0;
			bool inv=false;
			bool EnableSearchVictim;
			ActiveAbilities.modifyInvisibility(false,inv,mask,EnableSearchVictim); // нарисовать невидимку
			if(inv){
				// not detected
				byte mymask=NATIONS[MyNation].NMask;
				if(!(mask&mymask)){
					Tr=0x00;
				}else{
					Tr=0x80;
				}
			}
		}
		if(Transparency!=Tr){
			TransparencyLast=Transparency;
			Transparency=Tr;
			TransparencyStartTime=TrueTime;
		}
	}

	// NIndex
	int NewNIndex=0xFFFF;
	int ThroughState=0;
	if(Morph){
		if( OB->NIndex!=Morph->NewType.UnitType ){
			NewNIndex=Morph->NewType.UnitType;
			ThroughState=Morph->ThroughState;
		}
	}else{
		if( OB->NIndex!=BornNIndex ){
			NewNIndex=BornNIndex;
			ThroughState=BornState;
		}
	}
	if(NewNIndex!=0xFFFF){
		bool Blink = ThroughState==0&&!OB->NewBuilding;
		if(!ChangingNIndex){
			ChangingNIndex=true;
			if(Blink){
				TransparencyLast=Transparency;
				Transparency=0x00;
				TransparencyStartTime=TrueTime;
			}
			OB->UnlimitedMotion=true;
		}
		addrand(Blink);
		addrand(TransparencyStartTime);
		if( TrueTime-TransparencyStartTime>255 || !Blink ){
			bool modify=false;
			if( OB->newMons->CharacterWasModified ){
				modify=true;
				//delete(OB->newMons);
				//delete(OB->MoreCharacter);
			}
			if(ChangeUnitNIndex(OB, NewNIndex, ThroughState)){
				if(NewNIndex==BornNIndex){
					OB->newMons=OB->ActiveAbility->BornNM;
					OB->MoreCharacter=OB->ActiveAbility->BornAC;
				}
				if(Morph){
					if( Morph->Permanent ){
						if( modify && !OB->newMons->CharacterWasModified ){
							MarkObjectLikeModified(OB,false);
						}
						OB->ActiveAbility->BornNIndex=OB->NIndex;
						OB->ActiveAbility->BornNM=OB->newMons;
						OB->ActiveAbility->BornAC=OB->MoreCharacter;
						HeroVariableStorage* HVS=DetectHero(OB);
						if( HVS ){
							HeroAbility* HA=(HeroAbility*)HVS->GetA();
							if( OB->MaxMana<HA->Mana ){
								OB->Mana=HA->Mana;
								OB->MoreCharacter->MaxMana=HA->Mana;
								OB->MaxMana=OB->MoreCharacter->MaxMana;
							}
						}
						if( OB->Nat->AI_Enabled && OB->newMons->Peasant ){
							SCENINF.UGRP[OB->Nat->CITY->LasyPeasantsGroup].AddNewUnit(OB);
							SCENINF.UGRP[OB->Nat->CITY->AllWariorsGroup].RemoveUnit(OB);
						}
					}
					if( Morph->ReInitAbilities ){
						bool Change=ChangingNIndex;
						InitUnitAbilities(OB);
						ChangingNIndex=Change;
					}
				}
			}
		}
	}else{
		if(ChangingNIndex){
			OB->UnlimitedMotion=false;
			ChangingNIndex=false;
		}
	}
	addrand(OB->Life);
	
	//if(OB->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Newborn)){
	//	OB->ActiveAbility->ActiveAbilities.modifyNewborn(OB);
	//}
	// ModelAnimationNIndex
	if(!Morph){
		if( ModelAnimation ){
			word NIndex=0xFFFF;
			if( ModelAnimation->TakeFromTarget ){
				NIndex=TargetModelAnimationUnitType;
			}else{
				NIndex=ModelAnimation->Model.UnitType;
			}
			if( NIndex!=0xFFFF && NIndex!=ModelAnimationNIndex ){
				ModelAnimationNIndex=NIndex;
				if(OB->newMons&&OB->newMons->CharacterWasModified){
					NewMonster* NM=NATIONS[OB->NNUM].Mon[NIndex]->newMons;
					OB->newMons->Animations=NM->Animations;
					//OB->newMons->Animations->ANM.Clear();
					//for(int i=0;i<NM->Animations->ANM.GetAmount();i++){
					//	NewAnimation* N=new NewAnimation;
					//	NM->Animations->ANM[i]->Copy(N,false);		
					//	OB->newMons->Animations->Add(N,N->Code);
					//}
				}
			}
		} else {
			word NIndex=OB->NIndex;
			if( ModelAnimationNIndex!=0xFFFF && ModelAnimationNIndex!=NIndex ){
				ModelAnimationNIndex=NIndex;
				if(OB->newMons&&OB->newMons->CharacterWasModified){
					NewMonster* NM=NATIONS[OB->NNUM].Mon[NIndex]->newMons;\
					OB->newMons->Animations=NM->Animations;
					//OB->newMons->Animations->ANM.Clear();
					//for(int i=0;i<NM->Animations->ANM.GetAmount();i++){
					//	NewAnimation* N=new NewAnimation;
					//	NM->Animations->ANM[i]->Copy(N,false);		
					//	OB->newMons->Animations->Add(N,N->Code);
					//}
				}
			}
		}
	}

	//// центрировать экран
	//if( ActiveAbilities.isCenterScreen() ){
	//	int x=OB->RealX>>4;
	//	int y=OB->RealY>>4;
	//	GameInterface::ScrollToPositionOnMap(x,y,400);
	//	return false;
	//}

	// дл€ мертвых далее не ходить
	if( OB->GetLiveState()!=ls_LiveBody )
		return true;

	OB->MotionStyle=OB->newMons->MotionStyle;
	ActiveAbilities.modifyMotionStyle(OB->MotionStyle,OB->MotionStyle);

	OB->LockType=OB->newMons->LockType;
	ActiveAbilities.modifyLockType(OB->LockType,OB->LockType);

	// MaxLife
	word MaxLife=OB->MoreCharacter->Life;
	ActiveAbilities.modifyLifeMax(MaxLife,MaxLife);
	if(OB->MaxLife!=MaxLife){
		addrand(OB->MaxLife);
		addrand(MaxLife);
		addrand(OB->Life);
		//word OldLife;
		//OldLife=OB->Life;
		if(OB->MaxLife){
			OB->Life=((int(OB->Life)<<3)/OB->MaxLife*MaxLife)>>3;
			//OB->Life=((int(OB->Life)<<7)/OB->MaxLife*MaxLife)>>7;
			OB->MaxLife=MaxLife;
		}else{
			OB->Life=MaxLife;
			OB->MaxLife=MaxLife;
		}
		addrand(OB->Life);
	}

	// MaxMana
	word MaxMana=OB->MoreCharacter->MaxMana;
	addrand(MaxMana);
	addrand(OB->Mana);
	addrand(OB->MaxMana);
	//if(MaxMana){
		ActiveAbilities.modifyManaMax(MaxMana,MaxMana);
		addrand(MaxMana);
		if(MaxMana<0){
			MaxMana=0;
		}
		if( OB->MaxMana!=MaxMana ){
			if(OB->MaxMana){
				OB->Mana=((int(OB->Mana)<<3)/OB->MaxMana*MaxMana)>>3;
				//OB->Mana=((int(OB->Mana)<<7)/OB->MaxMana*MaxMana)>>7;
			}else{
				OB->Mana=MaxMana;
			}				
			OB->MaxMana=MaxMana;
			addrand(OB->Mana);
		}
	//}

	// Control, NNUM
	if(ControlNI==0xFF){
		if(BornNNUM==0xFF)BornNNUM=OB->NNUM;
		if(OB->NNUM!=BornNNUM){			
			OBJ_ChangeNation(OB,BornNNUM);
		}
	}else{
		if(OB->NNUM!=ControlNI){
			OBJ_ChangeNation(OB,ControlNI);
		}
	}
	
	// NMask
	/*if(NMask==-1){
		if(BornNMask==0)
			BornNMask=OB->NMask;
		if(OB->NMask!=BornNMask)
			OB->NMask=BornNMask;
	}else{
		if(OB->NMask!=NMask)
			OB->NMask=NMask;
	}
*/
	
	/*if(!NMaskBeginProcess){
	 NMaskBeginProcess=1;
	 NMask=-1;
	}*/
	if (NMask!=-1){
		if(!MaskWasEdited){
			NewNMask=OB->NMask;
		}
		ActiveAbilities.modifyNMask(OB->NMask,OB->NMask);
		MaskWasEdited=1;
	}
	if ((MaskWasEdited)&&(NMask==-1)){
		MaskWasEdited=0;
		OB->NMask=NewNMask;
	}
	// KillMask
	if( CurKillMask==0xFF ){
		if( BornKillMask==0xFF )
			BornKillMask=OB->newMons->KillMask;
		if( OB->KillMask!=BornKillMask )
			OB->KillMask=BornKillMask;
	}else{
		if( OB->KillMask!=CurKillMask )
			OB->KillMask=CurKillMask;
	}

	// FlyHeight
	if(OB->MotionStyle==8){
		if(FlyHeight==-1000){
			if(BornFlyHeight==0)
				BornFlyHeight=OB->newMons->FlyHeight;
			if(OB->newMons->FlyHeight!=BornFlyHeight)
				OB->newMons->FlyHeight=BornFlyHeight;
		}else{
			MarkObjectLikeModified(OB,false);
			if(OB->newMons->FlyHeight!=FlyHeight)
				OB->newMons->FlyHeight=FlyHeight;
		}
	}

	// Matherial
	byte MathMask=OB->newMons->MathMask;
	ActiveAbilities.modifyMaterial(MathMask,MathMask);
	//if(OB->MathMask!=MathMask){
	OB->MathMask=MathMask;
	//}

	// AttackCaster
	if(AttackCasterID!=0xFFFF){
		if (OB->EnemyID==0xFFFF || OB->EnemyID!=AttackCasterID || OB->EnemySN!=AttackCasterSN) {
			OneObject* Cast=Group[AttackCasterID];
			if(Cast&&Cast->Serial==AttackCasterSN){
				OB->AttackObj(AttackCasterID,128+64,0);
			}else{
				AttackCasterID=0xFFFF;
			}
		}else{
			AttackCasterID=0xFFFF;
		}
	}

	// Library
	if(LibID!=0xFFFF){
		if(GetHero(OB)){

			bool CloseLib=false;

			OneObject* Lib=Group[LibID];
			if( Lib && !Lib->Sdoxlo && Lib->Serial==LibSN ){
				// close magazine window
				int LibX,LibY;
				if(!GetBuildingEntryPoint(&LibX,&LibY,Lib->Index,100)){
					LibX=Lib->RealX/16;
					LibY=Lib->RealY/16;
				}
				int dist=Norma(OB->RealX/16-LibX,OB->RealY/16-LibY);

				OneMagazine* DetectMagazine2(OneObject* OB);
				OneMagazine* OM=DetectMagazine2(Lib);

				if( !OM || dist>=500 ){
					//CloseLib=true;
				}
			}else{
				CloseLib=true;
			}

			if(OB->LocalOrder&&OB->LocalOrder->PrioryLevel>=16){
				void StartDialogLink(OneObject* OB);
				Order1* Or=OB->LocalOrder;
				bool find=false;
				while(Or){
					if(Or->DoLink==&StartDialogLink){
						find=true;
						break;
					}
					Or=Or->NextOrder;
				}
				if(!find){
					CloseLib=true;
				}
			}

			byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];

			if(CloseLib){
				addrand(LibID);
				LibID=0xFFFF;
				if(OB->NNUM==NI){
					IGI.Library=false;
					IGI.Inventory=false;
				}
			}

			if( OB->NNUM==NI && !IGI.Library && LibID!=0xFFFF ){
				//addrand(LibID);
				//LibID=0xFFFF;
				vui_IS_ExitMagazine EM(OB);
				EM.InterfAction();
			}

		}else{
			LibID==0xFFFF;
		}
	}
	return true;
}
void ActiveUnitAbility::MoveToEnd(){
	if(OB){
		int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i=0;i<n;i++){
			ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
			if(AA==this){
				OB->ActiveAbility->ActiveAbilities.Move(i,n-1);
				break;
			}
		}
	}
};

bool UnitActiveAbilityList::Process() {

	if(!OB) return false;

	Morph=NULL;
	ModelAnimation=NULL;
	ControlNI=0xFF;
	CurKillMask=0xFF;
	NMask=-1;
	FlyHeight=-1000;

	bool rez=false;
	int n = ActiveAbilities.GetAmount();

	addrand(n);

	word Index=OB->Index;
	word Serial=OB->Serial;

	if( OB->GetLiveState()==ls_LiveBody ){
		if( ActiveAbilities.InfluenceMask.check_bit(ABL_Life) ){
			ActiveAbilities.modifyLife(OB->MaxLife,OB->Life);
			if( OB->Life>OB->MaxLife )
				OB->Life=OB->MaxLife;
		}
		if( ActiveAbilities.InfluenceMask.check_bit(ABL_Mana) ){
			ActiveAbilities.modifyMana(OB->MaxMana,OB->Mana);
			if( OB->Mana<0 ) 
				OB->Mana=0;
			if( OB->Mana>OB->MaxMana )
				OB->Mana=OB->MaxMana;
		}
	}

	// check clothed
	int nbut_max=EngSettings.vInterf.MagicCardsVisibleAmount;
	if(OB->MoreCharacter->MaxDamage[1]>0)
		nbut_max--;
	int nbut=0;
	//int nbottle=0;

	MagicsAmount=0;

	bool hero=GetHero(OB);
	bool init=false;
	
	CurProcAA=NULL;

	for(int i=0;i<n;i++){
		ActiveUnitAbility* AA=ActiveAbilities[i];
		CurProcAA=AA;

		/*if(AA->MoveToEnd && n>1){  //GEC- GecOptim Ubrano za nenadobnostiu (ugos!)300samplov
			AA->MoveToEnd=false;
			ActiveAbilities.Move(i,ActiveAbilities.GetAmount()-1);
			AA=ActiveAbilities[i];
		}*/

		if(AA->CasterID!=0xFFFF) GlobalUnitsContext::setCaster(AA->CasterID,AA->CasterSN);
			else GlobalUnitsContext::setCaster(Index,Serial);
		GlobalUnitsContext::setAcceptor(Index,Serial);
		
		// Edit by Xeno: avoid calls of suAi if the creature is under human control
		pUnit unit(OB);
		bool abl;
		//if (dynamic_cast<aa_SingleUnitAI*>(AA) && NationID == unit.GetNation()) abl = false;
		//else 
			abl=AA->Process();
		if(!Group[Index])return true;

		if(!abl){
			addrand(AA->Serial);
			ActiveAbilities.Del(i,1);
			delete AA;
			i--;
			n--;
			continue;
		}else{
			rez=true;

			// система инвентар€, потом переделать
			if(hero){
				const int ClothedMax[6]={0,0,0,0,0,13};
				const int NotClothedMax[6]={0,10000,0,0,10,36};
				static int Clothed[6];
				static int NotClothed[6];
				const int vClotheN=10;
				const int vClotheSet[vClotheN] = {1,		1,			1,			1,			1,			4,		2,			1,			1,		1};
				static int ClothedItems[vClotheN];
				if(!init){
					init=true;
					memset(Clothed,0,sizeof Clothed);
					memset(NotClothed,0,sizeof NotClothed);
					memset(ClothedItems,0,sizeof ClothedItems);
				}
				bool drop=false;
				bool antimage=OB->newMons->NewParams.AntiMage;
				UnitAbility* UA=AA->GetA();
				if( UA && !UA->System ){
					int TA=AA->TypeAbil;
					/*if( UA->Type ){
					}else */if( TA==1 || TA==2 ){
						TA=1;
						if(antimage){
							AA->Clothed=true;
						}else if(Clothed[TA]<nbut_max){
							AA->Clothed=true;
							AA->Slot=Clothed[TA];
							Clothed[TA]++;
							MagicsAmount++;
						}else{
							AA->Clothed=false;
							AA->Slot=NotClothed[TA];
							NotClothed[TA]++;
						}
					}else if( TA==4 ){
						int& I=ClothedItems[UA->TypeClothe];
						int IMax=vClotheSet[UA->TypeClothe];
						if( AA->Clothed && I<IMax ){
							I++;
							if(AA->Slot>=IMax){
								AA->Slot=-1;
							}
						}else if( NotClothed[TA]<NotClothedMax[TA] ){
							NotClothed[TA]++;
							if( AA->Clothed || AA->Slot>=NotClothedMax[TA] ){
								AA->Clothed=false;
								AA->Slot=-1;
							}
						}else/* if( !AA->Clothed && I<IMax ){
							I++;
							AA->Clothed=true;
							AA->Slot=-1;
						}else*/{
							drop=true;
						}
					}else if( TA==5 ){
						if( AA->Clothed && Clothed[TA]<ClothedMax[TA] ){
							Clothed[TA]++;
							if(AA->Slot>=ClothedMax[TA]){
								AA->Slot=-1;
							}
						}else if( NotClothed[TA]<NotClothedMax[TA] ){
							NotClothed[TA]++;
							if( AA->Clothed || AA->Slot>=NotClothedMax[TA] ){
								AA->Clothed=false;
								AA->Slot=-1;
							}
						}else if( !AA->Clothed && Clothed[TA]<ClothedMax[TA] ){
							Clothed[TA]++;
							AA->Clothed=true;
							AA->Slot=-1;
						}else{
							drop=true;
						}
						//if( !drop && AA->Slot==-1){
						//	DynArray<bool> filled;
						//	int n=128;
						//	filled.Add(0,n);
						//	itr_AA.Create(OB);
						//	while(ActiveUnitAbility* aA=itr_AA.Next()){
						//		if( aA->TypeAbil==5 && aA->Clothed==AA->Clothed && aA->Slot!=-1 ){
						//			if( aA->Slot>=0 && aA->Slot<filled.GetAmount() ){
						//				filled[aA->Slot]=true; //if(filled[aA->Slot])aA->Slot=-1;else
						//			}
						//		}
						//	}
						//	for(int i=0;i<n;i++){
						//		if( !filled[i] ){
						//			AA->Slot=i;
						//			break;
						//		}
						//	}
						//}
					}
					
				}
				if(drop){
					// лишние вываливаютьс€
					addrand(AA->Droped);
					//AA->Droped=true;
					AA->DropDown();
					void EventReceiveItem(OneObject* Acceptor, OneObject* ItemOB, ActiveUnitAbility* AA, char* Msg=NULL);
					static char* txt=GetTextByID("#GM_ItemsOverflow");
					DString str;
					str.print("{C FFF00010}%s:",txt);
					EventReceiveItem(OB,NULL,AA,str.pchar());
				}else if( AA->Slot==-1 && (AA->TypeAbil==4||AA->TypeAbil==5) ){ //if( !drop && AA->Slot==-1 ){
					// присвоение номера слота
					DynArray<bool> filled;
					int n=128;
					filled.Add(0,n);
					itr_AA.Create(OB);
					while(ActiveUnitAbility* aA=itr_AA.Next()){
						UnitAbility* uA=aA->GetA();
						if( aA->TypeAbil==AA->TypeAbil && uA && (uA->TypeClothe==UA->TypeClothe||!aA->Clothed) && aA->Clothed==AA->Clothed && aA->Slot!=-1 ){
							if( aA->Slot>=0 && aA->Slot<filled.GetAmount() ){
								if(filled[aA->Slot])
									aA->Slot=-1;
								else
									filled[aA->Slot]=true;
							}
						}
					}
					for(int i=0;i<n;i++){
						if( !filled[i] ){
							AA->Slot=i;
							break;
						}
					}
				}
			}else{
				//if( AA->TypeAbil!=0 )
				//	AA->Clothed=true;
			}
		}

		uaModMask& aa=AA->InfluenceMask;
		if(OB->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_ExtraAbility)){
		OB->ActiveAbility->ActiveAbilities.modifyExtraAbility(OB);
		//if((AA->TypeAbil==4)&&(AA->Clothed=false)){
        }
		if(OB->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_DeleteAbility)){
		OB->ActiveAbility->ActiveAbilities.modifyDeleteAbility(OB);
        }
		if( aa.check_bit(ABL_Plymorph) ){
			itr_UM.Create(AA,ABL_Plymorph);
			while (mod_Plymorph* M=(mod_Plymorph*)itr_UM.Next()) {
				Morph=M;
				int NIndex=Morph->NewType.UnitType;
				if( Morph->Permanent && OB->NIndex==NIndex && OB->ActiveAbility->BornNIndex==NIndex ){
					AA->Deleted=true;
				}
			}
		}
		if( aa.check_bit(ABL_ModelAnimation) ){
			itr_UM.Create(AA,ABL_ModelAnimation);
			while (mod_ModelAnimation* M=(mod_ModelAnimation*)itr_UM.Next()) {
				ModelAnimation=M;
				TargetModelAnimationUnitType=AA->TargetUnitType;
			}
		}
		if( aa.check_bit(ABL_Control) ){
			OneObject* Caster=Group[AA->CasterID];
			if(Caster&&Caster->Serial==AA->CasterSN){
				itr_UM.Create(AA,ABL_Control);
				while(UnitModificator* UM=itr_UM.Next()){
					ControlNI=Caster->NNUM;
				}
			}
		}
		if( aa.check_bit(ABL_KillMask) ){
			CurKillMask=BornKillMask;
			itr_UM.Create(AA,ABL_KillMask);
			while( mod_KillMask* M=(mod_KillMask*)itr_UM.Next() ){
				byte Mask=1<<M->Material;
				if( M->Operation==0 ){
					CurKillMask=Mask;
				}else if( M->Operation==1 ){
					CurKillMask&=Mask;
				}else{
					CurKillMask|=Mask;
				}
			}
		}
		if( aa.check_bit(ABL_NMask) ){
			itr_UM.Create(AA,ABL_NMask);
			while(mod_NMask* M=(mod_NMask*)itr_UM.Next()){
				NMask=M->NMask;
			}
		}
		if(!( aa.check_bit(ABL_NMask)) ) NMask=-1;
		if( aa.check_bit(ABL_OverEarth) ){
			itr_UM.Create(AA,ABL_OverEarth);
			while(mod_OverEarth* M=(mod_OverEarth*)itr_UM.Next()){
				FlyHeight=M->FinalHeight;
			}
		}
	}

	if( OB /*&& !OB->Sdoxlo*/ )
		PostProcess();

	return rez;
}
DLLEXPORT void OBJ_ChangeNation(OneObject* OB, byte DstNat){
	if( OB && OB->GetLiveState()==ls_LiveBody && OB->NNUM!=DstNat && DstNat<8 ){
		bool sel=false;
		if( OB->Selected&(1<<OB->NNUM) ){
			sel=true;
			pUnit u(OB);
			pSelected::units(OB->NNUM)-=u;
			pImSelected::units(OB->NNUM)-=u;
		}
		DelObject(OB);
		OB->Nat->CITY->UnRegisterNewUnit(OB);
		OB->Ref.General=NATIONS[DstNat].Mon[OB->NIndex];
		OB->Nat=&NATIONS[DstNat];
		OB->NMask=OB->Nat->NMask;
		if( OB->ActiveAbility )
			OB->ActiveAbility->BornNMask=OB->NMask;
		OB->NNUM=DstNat;
		OB->Nat->CITY->RegisterNewUnit(OB);	
		AddObject(OB);
		if(sel){
			pUnit u(OB);
			pSelected::units(OB->NNUM)+=u;
			pImSelected::units(OB->NNUM)+=u;
		}
	}
}
DLLEXPORT void ChangeUnitColor(int Index,byte DstNI){
	if( Index<0 )
		return;
	OneObject* OB=Group[Index];
	if(OB->ActiveAbility){
		OB->ActiveAbility->BornNNUM=DstNI;
	}else{
		//OBJ_ChangeNation(OB,DstNI);
	}
	OBJ_ChangeNation(OB,DstNI);
};
bool UnitActiveAbilityList::AddActiveUnitAbility(ActiveUnitAbility* AA)
{
	if(AA){

		// delete if primary condition not check
		if (AA->TypeAbil==0) {
			if (AA->GetA() && !AA->GetA()->Conditions.Check(AA)) {
				return false;
			}
		}

		if(AA->TypeAbil==4){
			// вещи не одевать, чтобы игрок сам открывал инвентарь и одевал вещь
			AA->Clothed=false;
			AA->Slot=-1;
		}else if(AA->TypeAbil==5){
			// если такой предмет одет - новый не одевать
			bool take_off=false;
			UnitAbility* UA=AA->GetA();
			itr_AA.Create(OB);
			while(ActiveUnitAbility* aA=itr_AA.Next()){
				if( aA!=AA && aA->TypeAbil!=0 ){
					UnitAbility* uA=aA->GetA();
					if(UA==uA){
						if(aA->Clothed){
							 take_off=true;
							 break;
						}
					}
				}
			}
			if(take_off){
				AA->Clothed=false;
				AA->Slot=-1;
			}
		}

		ActiveAbilities.Add(AA);

		return true;
	}
	return false;
}
ActiveUnitAbility* UnitActiveAbilityList::GetActiveUnitAbility(const char* name)
{
	ActiveUnitAbility* rez=NULL;
	int n = ActiveAbilities.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(!strcmp(name,ActiveAbilities[i]->Name.pchar()))
		{
			rez=(ActiveAbilities[i]);
			break;
		}
	}
	return rez;
}
//==================================================================================================================//
void ApplyUnitAbility(int AbilityIndex,word Unit, word TargtUnit, int x, int y, int z)
{
	/*
	int n = Abilities.Abilities.GetAmount();
	if(AbilityIndex>-1&&AbilityIndex<n)
	{
		Abilities.Abilities[AbilityIndex]->Execute(Unit,TargtUnit,x,y,z);
	}
	*/
}
void TestCGARB();
void UnitAbilityTest(){
	return;
    //try{
        for (int i=0;i<MAXOBJECT;i++) {
            OneObject* OB=Group[i];
            if (OB) {
                if (OB && OB->ActiveAbility) {
                    int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
                    for(int i=0;i<OB->ActiveAbility->ActiveAbilities.GetAmount();i++){
                        ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
						int id;
						if(AA->OB)id=AA->OB->Index;
						int z=0;
                    }
                }
            }
        }  
		for(int B=0;B<nCP_Branches;B++){
			DynArray<OneClassPointer*>& CPA=CPGARB.CPointer[B];
			int TCN = CPA.GetAmount();			
			while (TCN--) {
				OneClassPointer* CP=CPA[TCN];
				if(CP->ClassPtr)CP->ClassPtr->GetObjectGlobalID();
				assert((CP->Index>>24)==B);
			}
		}
		extern NodePool* _nodepool;
		for(DWORD i=0;i<8192;i++){
			DWORD id=i+((i+1)<<16);
            SNode* s=_nodepool->GetNode(id);
			if(s){
				s->GetName();
			}
		}
    //}catch(...){
    //    assert(0);
    //}
}
__declspec(dllexport) void UnitAbilityTest2(){
	//UnitAbilityTest();
}
uni_hash<RawUnit> ActiveUnits;
void ClearActiveUnits(){
    ActiveUnits.reset();
}
void AddUnitToActiveList(OneObject* OB){
    ActiveUnits.add_once(OB->Index,RawUnit(OB->Index,OB->Serial));
}
void ProcessActiveUnits(){
    scan(ActiveUnits,RawUnit* u,DWORD* idx){
        OneObject* OB=u->object();
        if(OB){
            if (OB && OB->ActiveAbility) {
                OB->ActiveAbility->Process();                
           }            
        }else{
            ActiveUnits.del(*idx);
        }
    }scan_end;
}
void UnitAbilityProcess() {
    //UnitAbilityTest();
	//for( int i=REALTIME&7; i<MAXOBJECT; i+=8 ){
	////for( int i=REALTIME&3; i<MAXOBJECT; i+=4 ){
	//	if(OB->newMons->Ability){
	// 		OB->newMons->Ability->Process(i);
	//	}
	//	OneObject* OB=Group[i];
	//	if(OB){
	//		if(OB->ActiveAbility){
	//			//OB->ActiveAbility->Hero=DetectHero(OB);
	//			//OB->ActiveAbility->Chest=DetectChest(OB);
	//			OB->ActiveAbility->Process();
	//			int xxx = 0;//del it later
	//		}
	//	}
	//}
    ProcessActiveUnits();    
	//UnitAbilityTest();
}
void UnloadUnitsAbility()
{
	for(int i=0;i<MAXOBJECT;i++)
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
}
void OnMakeDamageProcess(OneObject* Sender,OneObject* Take,int& Damage)
{
	if(Sender&&Take)
	{
		if(Sender->ActiveAbility)
		{
			int n=Sender->ActiveAbility->ActiveAbilities.GetAmount();
			for(int i=0;i<n;i++)
			{
				Sender->ActiveAbility->ActiveAbilities[i]->OnMakeDamage(Take,Damage);
			}
		}
	}
}
//extern int ItemChoose;
bool ProcessMessages();
bool MMItemChoose(SimpleDialog* SD);

void ProcessUnitAbilityEditor(){
	return;
	xmlQuote xml;
	ItemChoose=-1;
	if(xml.ReadFromFile("Dialogs\\WeaponSystem.DialogsSystem.xml")){
		DialogsSystem DSS;
		ErrorPager EP;
		DSS.Load(xml,&DSS,&EP);
		SimpleDialog* Desk=DSS.Find("OptionsDesk");
		SimpleDialog* OK=DSS.Find("OK");
		SimpleDialog* CANCEL=DSS.Find("CANCEL");
		if(Desk&&OK&&CANCEL){
			int x0,y0,x1,y1;
			DSS.GetDialogsFrame(x0,y0,x1,y1);
			if(x1>x0){
				DSS.x=(RealLx-x1+x0)/2;
				DSS.y=(RealLy-y1+y0)/2;
				OK->OnUserClick=&MMItemChoose;
				OK->UserParam=1;
				CANCEL->OnUserClick=&MMItemChoose;
				CANCEL->UserParam=1;
				ClassEditor CE;
				CE.CreateFromClass(Desk,0,0,Desk->x1-Desk->x,Desk->y1-Desk->y,&Abilities,3,"EmptyBorder");
				int SaveTime = GetTickCount();
				do{
					//GameWeaponSystem.RefreshEnumerator();
					ProcessMessages();					
					DSS.ProcessDialogs();
					CE.Process();
					DSS.RefreshView();
					int TT = GetTickCount();
					if((SaveTime+2000)<TT)
					{
						xmlQuote* sxml = new_xmlQuote("ClassArray.UnitAbility");
						Abilities.Abilities.Save(*sxml,&Abilities.Abilities);
						sxml->WriteToFile("UnitsAbility\\base.ua.xml");
						SaveTime=TT;
						delete_xmlQuote(sxml);
					}
				}while(ItemChoose==-1);
			}
		}
	}	
}
extern word NNewMon;
extern NewMonster* NewMon[2048];

bool LoadAbilities(char* FileName)
{
	for(int i=0;i<NNewMon;i++)
	{
		if(NewMon[i]->Ability)
		{
			NewMon[i]->Ability->AbilitiesList.Clear();
			//NewMon[i].Ability->ItemDrop.Clear();
			NewMon[i]->Ability->Feeled=false;
		}
	}
	return Abilities.LoadAbilities(FileName);
}
ActiveUnitAbility* AddAbility(UnitAbility* Source, OneObject* Dest, int TypeAbil=2){
	if(Source&&Dest){
		ActiveUnitAbility* newA=Source->GetNewActiveAbility();
		newA->TypeAbil=TypeAbil;
		if(AddActiveUnitAbility(Dest->Index,newA)){
			return newA;
		};
		delete newA;
	}
	return NULL;
}
extern CHeroesCollector HeroesCollector;
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA)
{
	if(Unit!=0xFFFF)
	{
		OneObject* OB=Group[Unit];
		if(OB){

			AUA->OB=OB;
			//AUA->UnitIndex=OB->Index;

			UnitAbility* UA=AUA->GetA();
			if(UA){
				AUA->Visible=UA->Visible;
				// ---
				UA->OnAddToObject(OB);
				// ---
			}

			if(!OB->ActiveAbility){
				OB->ActiveAbility = new UnitActiveAbilityList(OB);	
			}
			bool res=OB->ActiveAbility->AddActiveUnitAbility(AUA);
			
			// Edit by Xeno to implement bottle cooking
			if( UA && (UA->Type==0||UA->Type==1||UA->Type==2) && (AUA->TypeAbil==4||AUA->TypeAbil==5) ){
				CookBook.CheckProduct(OB);
			}

			addrand(Unit);
			addrand(AUA->Serial);

			return res;
		}
	}
	return false;
}
void PushUnitsAbility::OnAddToObject(OneObject* OB){
	OwnerID=OB->Index;
	OwnerSN=OB->Serial;
};

bool AddMonsterAbility(MonsterAbility** MA,char* Name){
	if(MA&&Name){
		if((*MA)==NULL)
		{
			(*MA)=new MonsterAbility();
		}
		_str* n = new _str();
		n->Add(Name);
		(*MA)->AbilityNames.Add(n);
		return true;
	}
	return false;
}
//==================================================================================================================//
AuraEffect::AuraEffect()
{
}
bool AuraEffect::Process()
{
	if(GetUnitIndex()!=0xFFFF)
	{
		OneObject* OB = Group[GetUnitIndex()];
		if(OB&&(!OB->Sdoxlo))
		{
			UnitAbilityAura* UA =(UnitAbilityAura*)(Abilities.Abilities[AbilityIndex]);
			if(Creator!=0xFFFF)
			{
				OneObject* C = Group[Creator];
				if(C&&(!C->Sdoxlo))
				{
					int ds=Norma((OB->RealX>>4)-(C->RealX>>4),(OB->RealY>>4)-(C->RealY>>4));
					if(ds<UA->Radius)
					{
						return true;
					}
				}
			}
			OB->AddDamage-=UA->AddDamage;
			OB->AddShield-=UA->AddShield;
		}
	}
	return false;
}
//==================================================================================================================//

void AttackObjLink(OneObject* OBJ);

DWORD UnitModificator::CurSerial=0;
void KillUnitNoScore(OneObject* OB){
	int sc=CITY[OB->NNUM].Account;
	OB->Die();
	CITY[OB->NNUM].Account=sc;
}
OneObject* TransformUnitType(OneObject* OB,int DestType){
	if(OB&&OB->NIndex!=DestType){
		NATIONS[OB->NNUM].NProduced[OB->NIndex]--;
		if(/*OB->NewBuilding||*/OB->CObjIndex!=0xFFFF){
			int acc=CITY[OB->NNUM].Account;
			int NIND=OB->NIndex;
			int NK=NATIONS[OB->NNUM].NKilled[NIND];
			word ID=OB->Index;
			word SN=OB->Serial;
			word NI=OB->NNUM;
			int x=OB->RealX;
			int y=OB->RealY;
			OB->delay=6000;
			if(OB->LockType==1)OB->RealDir=32;
			KillUnitNoScore(OB);
			OB=Group[OB->Index];
			if(OB){
				OB->Sdoxlo=2500;				
				void EliminateBuilding(OneObject* OB);
				if(OB->NewBuilding)EliminateBuilding(OB);
				Group[OB->Index]=NULL;
			}
			SetActiveProducer(OB);
			word id=NATIONS[NI].CreateNewMonsterAt(x,y,DestType,1);
			SetActiveProducer(NULL);
			if(id!=0xFFFF){
				OB=Group[id];
				OB->Index=ID;
				OB->Serial=SN;
				if(OB->NewBuilding)for(int i=0;i<OB->NStages;i++)OB->NextStage();
			}
			CITY[OB->NNUM].Account=acc;
			NATIONS[OB->NNUM].NKilled[NIND]=NK;
			return OB;
		}else{
			NATIONS[OB->NNUM].NProduced[DestType]++;

			OB->NIndex=DestType;
			OB->Ref.General=NATIONS[OB->NNUM].Mon[DestType];

			bool modify=false;
			if(OB->newMons->CharacterWasModified){
				modify=true;
				delete(OB->newMons);
				delete(OB->MoreCharacter);
			}

			OB->newMons=OB->Ref.General->newMons;
			OB->MoreCharacter=OB->Ref.General->MoreCharacter;

			OB->LockType=OB->newMons->LockType;
			OB->NewState=0;
			OB->LocalNewState=0;
			OB->GroundState=0;
			OB->OverEarth=0;
			OB->GroupSpeed=OB->newMons->MotionDist;

			OB->Life=(((OB->Life*100)/OB->MaxLife)*OB->MoreCharacter->Life)/100;
			OB->MaxLife=OB->MoreCharacter->Life;

			
			if(OB->NewBuilding){
				OB->LoLayer=OB->newMons->GetAnimation(anm_StandLo);
				OB->HiLayer=OB->newMons->GetAnimation(anm_StandHi);
				OB->NewAnm=OB->newMons->GetAnimation(anm_Work);
			}else{
				OB->NewAnm=OB->newMons->GetAnimation(anm_Stand);
			}
            OB->SetZeroFrame();

			if(modify){
				MarkObjectLikeModified(OB);
			}else{
				InitUnitAbilities(OB);
				if(OB->ActiveAbility){
					OB->ActiveAbility->SetBornNIndex();
				}
			}

			if( OB->newMons->ArmAttack && OB->ArmAttack==false && OB->RifleAttack==false ){
				OB->RifleAttack=true;
			}
			
		}
	}
	return OB;
}
bool ChangeUnitNIndex(OneObject* OB, word NewNIndex, int ThroughState){
	addrand(NewNIndex);
	if(OB && !OB->Sdoxlo && NewNIndex<2048){
		addrand(NewNIndex);
		addrand(OB->NIndex);
		if(OB->NIndex==NewNIndex){
			return true;
			//return false;
		}
		if(ThroughState!=0){
			ThroughState++;
		}
		addname(OB->Index);
		addrand(ThroughState);
		addrand(OB->NewState);
		if( OB->NewState!=ThroughState ){
			if( OB->LocalOrder && OB->LocalOrder->DoLink==&AttackObjLink ){
				OB->DeleteLastOrder();
			}
			OB->NewState=ThroughState;
			OB->GroundState=ThroughState;
			addname(OB->Index);
			TryToStand(OB,false);
			addname(OB->Index);
			if(ThroughState!=0)
				return false;
		}

		//if (OB->OverEarth>=14) OB->OverEarth-=14;
		//	else OB->OverEarth=0;
		
		OB->DestX=-1;
		OB->DestY=-1;

		//OB->ClearOrders();

		//OB->UnlimitedMotion=false;
		//if(!OB->NewBuilding){
		//	void StayForSomeTime(OneObject* OB,byte OrdType,int Time);
		//	StayForSomeTime(OB,1,0);
		//}
		//OB->UnlimitedMotion=true;

		if(!OB->ActiveAbility){
			OB->ActiveAbility = new UnitActiveAbilityList(OB);
		}
		//int T=1000;
		//if(ThroughState==0&&!OB->NewBuilding){
		//	T=TrueTime-OB->ActiveAbility->HideStartTime;
		//}

		if (/*T<1000*/ThroughState==0 || OB->FrameFinished && OB->LocalNewState==ThroughState /*&& OB->OverEarth<=0*/) {

			// through alpha channel
			//if(OB->ActiveAbility){
			//	if(T>1000){
			//		OB->ActiveAbility->HideStartTime = TrueTime;
			//		return false;
			//	}
			//}
			//if (T<256) {
			//	return false;
			//}

			OB->NIndex=NewNIndex;
			OB->Ref.General=NATIONS[OB->NNUM].Mon[NewNIndex];
	
			OB->newMons=OB->Ref.General->newMons;
			OB->MoreCharacter=OB->Ref.General->MoreCharacter;
			OB->LockType=OB->newMons->LockType;
			OB->NewState=0;
			OB->LocalNewState=0;
			OB->GroundState=0;
			OB->OverEarth=0;
			OB->GroupSpeed=OB->newMons->MotionDist;
			
			OB->SetZeroFrame();
			if(OB->NewBuilding){
				OB->LoLayer=OB->newMons->GetAnimation(anm_StandLo);
				OB->HiLayer=OB->newMons->GetAnimation(anm_StandHi);
				OB->NewAnm=OB->newMons->GetAnimation(anm_Work);
			}else{
				OB->NewAnm=OB->newMons->GetAnimation(anm_Stand);
			}

			if(OB->newMons->ArmAttack){
				OB->ArmAttack=false;
				OB->RifleAttack=true;
			}else{
				OB->RifleAttack=false;
			};

			return true;
		}
		return false;
	}
	//return true;
	return false;
}

//==================================================================================================================//
class CBoidsParams : public BaseClass
{
public:
	CBoidsParams()
	{
		Radius=0;
		MinDist=0;
		OneUnitW=0;
		MainDirNorma=0;
		DensNorma=0;
		DensW=0;
		CentNorma=0;
		CentW=0;
		DeviationW=0;
		PushRadius=30;
		InFormation=100;
	};
	int Radius;
	int MinDist;
	int OneUnitW;
	int MainDirNorma;
	int DensNorma;
	int DensW;
	int CentNorma;
	int CentW;
	int DeviationW;
	int LockNorma;
	int LockW;
	int ChangeSpeedW;
	int PushRadius;
	int InFormation;
	SAVE(CBoidsParams)
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,MinDist);
		REG_MEMBER(_int,OneUnitW);
		REG_MEMBER(_int,MainDirNorma);
		REG_MEMBER(_int,DensNorma);
		REG_MEMBER(_int,DensW);
		REG_MEMBER(_int,CentNorma);
		REG_MEMBER(_int,CentW);
		REG_MEMBER(_int,DeviationW);
		REG_MEMBER(_int,LockNorma);
		REG_MEMBER(_int,LockW);
		REG_MEMBER(_int,ChangeSpeedW);
		REG_MEMBER(_int,PushRadius);
		REG_MEMBER(_int,InFormation);
	ENDSAVE;
};
CBoidsParams BoidsParams;
bool LoadBoidsParams()
{
	bool rez=false;
	xmlQuote Inf;
	if(Inf.ReadFromFile("UnitsAbility\\BoidsParams.xml"))
	{
		ErrorPager Error;
		if(BoidsParams.Load(Inf,&BoidsParams,&Error))
		{
			rez=true;
		}
	}
	return rez;
}
bool CheckNearest(OneObject* OBn,void* param)
{
	int* pr=(int*)param;
	OneObject* OB=(OneObject*)(pr[0]);
	if(OB!=OBn&&OBn&&(!OBn->Sdoxlo)&&(OB->NIndex&OBn->NIndex))
	{
		int ddx=OBn->RealX-OB->RealX;
		int ddy=OBn->RealY-OB->RealY;
		int ddd=Norma(ddx,ddy)+1;
		
		int OUW1=BoidsParams.OneUnitW/2;
		int OUW2=BoidsParams.OneUnitW/2;
		int rz1=pr[1]/2;
		int rz2=pr[1]/2;
		if(OB->newMons->BoidsMovingMinDist!=-1)rz1=OB->newMons->BoidsMovingMinDist<<4;
		if(OBn->newMons->BoidsMovingMinDist!=-1)rz2=OBn->newMons->BoidsMovingMinDist<<4;
		if(OB->newMons->BoidsMovingWeight!=-1)OUW1=OB->newMons->BoidsMovingWeight;
		if(OBn->newMons->BoidsMovingWeight!=-1)OUW2=OBn->newMons->BoidsMovingWeight;
		int rzz=rz1+rz2;
		if(OB->BrigadeID!=0xFFFF&&OB->BrigadeID==OBn->BrigadeID)rzz=(BoidsParams.InFormation*rzz)/100;
		int rz=ddd-rzz;
		if(rz<0)
		{
			int dx=(OUW1+OUW2)*ddx*rz/rzz;
			int dy=(OUW1+OUW2)*ddy*rz/rzz;
			if(ddd<(rzz>>1)){
				dx<<=1;
				dy<<=1;
			}
			if(ddd<(rzz>>2)){
                dx<<=2;
				dy<<=2;
			}
			pr[2]+=dx;
			pr[3]+=dy;			
		}
		/*
		if(rz>0)
		{
			ddx=ddx*rz/ddd;
			ddy=ddy*rz/ddd;
		}
		pr[2]+=ddx;
		pr[3]+=ddy;
		*/
		if(OBn->DestX>0)
		{
			pr[4]+=OBn->RealX;
			pr[5]+=OBn->RealY;
			return true;
		}
	}
	return false;
}
void BoidsSingleStep(OneObject* OB,int &dx, int &dy, int &ChangeSpeed)
{
	if(OB->BrigadeID!=0xFFFF&&BoidsParams.InFormation==0)
		return;
	if(OB->UnlimitedMotion)
		return;
	dx>>=3;
	dy>>=3;	
	//int Radius=150;
	//int MinDist=50;
	//int MainDirW=10000; 
	int ds=Norma(dx,dy)+1;
	int Param[6];
	Param[0]=(int)OB;
	Param[1]=BoidsParams.MinDist<<4;
	Param[2]=0;//New dx
	Param[3]=0;//New dy;
	Param[4]=0;//cx
	Param[5]=0;//cy;
	int n=PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,BoidsParams.Radius,CheckNearest,Param);
	int idx=(dx*BoidsParams.MainDirNorma)/ds;
	int idy=(dy*BoidsParams.MainDirNorma)/ds;
	int ndd=Norma(Param[2],Param[3])+1;
	int ndx=Param[2];
	int ndy=Param[3];
	if(ndd>BoidsParams.DensNorma)
	{
		ndx=(ndx*BoidsParams.DensNorma)/ndd;
		ndy=(ndy*BoidsParams.DensNorma)/ndd;
	}
	//int ndx=0;
	//int ndy=0;
	if(n)
	{
		int cx=Param[4]/n;
		int cy=Param[5]/n;
		int dcx=cx-OB->RealX;
		int dcy=cy-OB->RealY;
		int ddc=Norma(dcx,dcy)+1;
		if(ddc>BoidsParams.CentNorma)
		{
			dcx=(dcx*BoidsParams.CentNorma)/ddc;
			dcy=(dcy*BoidsParams.CentNorma)/ddc;
		}
		ndx=ndx*BoidsParams.DensW+dcx*BoidsParams.CentW;
		ndy=ndy*BoidsParams.DensW+dcy*BoidsParams.CentW;
		
		/*
		int ndd2=Norma(ndx,ndy)+1;
		ndx=(ndx*1000)/ndd2;
		ndy=(ndy*1000)/ndd2;
		*/
		if(ndx||ndy)
		{
			int dnd=Norma(ndx,ndy);
			int ldx=(ndx*BoidsParams.LockNorma)/dnd;
			int ldy=(ndy*BoidsParams.LockNorma)/dnd;
			ChangeSpeed=(BoidsParams.ChangeSpeedW*(idx*ndx+idy*ndy)/BoidsParams.MainDirNorma)/1000;
			
			if(MFIELDS[OB->LockType].CheckBar(OB->x+(ldx>>8),OB->y+(ldy>>8),1,1))
			{
				//ndx-=(ldx*BoidsParams.LockW)/BoidsParams.LockNorma;
				//ndy-=(ldy*BoidsParams.LockW)/BoidsParams.LockNorma;
				ndx=0;
				ndy=0;
			}
		}
		
		

		char BaseDir=GetDir(dx,dy);
		char NewDir=GetDir(ndx,ndy);
		byte raz=abs(BaseDir-NewDir);
		//OB->Speed+=24*(64-raz)/64;
	}
	
	
	if(ndx||ndy)
	{
		dx=idx+ndx*BoidsParams.DeviationW;
		dy=idy+ndy*BoidsParams.DeviationW;
	}
}
bool UnitLitleShiftLin(OneObject* OBn,void* param)
{
	int* pr=(int*)param;
	OneObject* OB=(OneObject*)(pr[0]);
	if(OB!=OBn&&OBn&&(!OBn->Sdoxlo))
	{
		int ddx=OB->RealX-OBn->RealX;
		int ddy=OB->RealY-OBn->RealY;
		int ddd=Norma(ddx,ddy);
		if(ddd==0)ddd=1;
		ddx=ddx*4096/ddd;
		ddy=ddy*4096/ddd;
		if(ddd<pr[1])
		{
			int rz=pr[1]-ddd;
			pr[2]+=(ddx*rz)/pr[1];
			pr[3]+=(ddy*rz)/pr[1];
			return true;
		}
	}
	return false;
}
void UnitLitleShift(OneObject* OB)
{
	if(!BoidsParams.PushRadius)return;
	if(OB->UnlimitedMotion)
		return;
	if(OB->BrigadeID!=0xFFFF){
		Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
		if(BR&&BR->WarType>0&&Norma(OB->RealX/16-BR->posX[OB->BrIndex],OB->RealY/16-BR->posY[OB->BrIndex])<8)return;
	}
	int Param[6];
	Param[0]=(int)OB;
	Param[1]=BoidsParams.PushRadius<<4;
	Param[2]=0;//New dx
	Param[3]=0;//New dy;
	int n=PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,40,UnitLitleShiftLin,Param);
	if(n)
	{
		int dx=Param[2];
		int dy=Param[3];
		int ddc=Norma(Param[2],Param[3])+1;
		if(ddc>600)
		{
			dx=(Param[2]*600)/ddc;
			dy=(Param[3]*600)/ddc;
		}
		addrand(dx);addrand(dy);
		OB->RealX+=dx/100;
		OB->RealY+=dy/100;
		addrand(OB->RealX);
		addrand(OB->RealY);
		extern MotionField UnitsField;
		UnitsField.BClrBar(OB->x,OB->y,OB->Lx);
		OB->x=(OB->RealX-((OB->Lx)<<7))>>8;
		OB->y=(OB->RealY-((OB->Lx)<<7))>>8;					
		UnitsField.BSetBar(OB->x,OB->y,OB->Lx);
	}
}

//==================================================================================================================//
//================================           NEW BOIDS              ================================================//
#define NeighborDist 60
DynArray<word> NeighboringUnits;
DynArray<int> UnitsCoordAndPushForce;
void AddUnitToNeighboringListFromCell(OneObject* OB,int xx, int yy)
{
	SAFENEW;
		addrand(xx);
		addrand(yy);
		if(xx>=0&&yy>=0&&xx<VAL_MAXCX&&yy<VAL_MAXCX)
		{
			int R0=OB->CollisionRadius;
			int cell=xx+(yy<<VAL_SHFCX);
			int NMon=MCount[cell];
			if(NMon)
			{
				addrand(NMon);
				int xu=OB->RealX>>4;
				int yu=OB->RealY>>4;
				int ofs1=cell<<SHFCELL;
				word MID;                
				for(int i=0;i<NMon && i<36;i++)
				{
					MID=GetNMSL(ofs1+i);
					if(MID!=0xFFFF&&MID>OB->Index)
					{
						OneObject* OBn=Group[MID];
						if(OBn){
							int GecCheck=Norma((OBn->RealX>>4)-xu,(OBn->RealY>>4)-yu); //GEC- GecOptim gec vse tyt popoganit!!!
							if(GecCheck<R0+40){
								int R1=OBn->CollisionRadius;
								if(GecCheck<R0+R1+14){
									if(!(OBn->Sdoxlo != OB->Sdoxlo || OBn->UnlimitedMotion || OBn->NewBuilding || OBn->CObjIndex!=0xFFFF)){
										NeighboringUnits.Add(OB->Index);
										NeighboringUnits.Add(MID);
										addrand(MID);
										addrand(NeighboringUnits.GetAmount());							
									}
								}
							}
						}
					}
				}
			}
		}
	ENDNEW;
}
void AddUnitToNeighboringList(word Index)
{
	if(Index!=0xFFFF)
	{
		OneObject* OB=Group[Index];
		if(OB && !(/*OB->Sdoxlo ||*/ OB->UnlimitedMotion || OB->NewBuilding || OB->CObjIndex!=0xFFFF))
		{
			int x=OB->RealX>>9;
			int y=OB->RealY>>9;
			int x0=(x>>2)+1;
			int y0=(y>>2)+1;
			int x1=x0+(x&2)-1;
			int y1=y0+(y&2)-1;
			AddUnitToNeighboringListFromCell(OB,x0,y0);
			AddUnitToNeighboringListFromCell(OB,x0,y1);
			AddUnitToNeighboringListFromCell(OB,x1,y0);
			AddUnitToNeighboringListFromCell(OB,x1,y1);
		}
	}
}
void FillNeighboringList()
{
	SAFENEW;
		NeighboringUnits.FastClear();
		for(int i=0;i<MAXOBJECT;i++)
		{
			AddUnitToNeighboringList(i);
		}
	ENDNEW;
}
void CPF_Stage1(){
    SAFENEW;
		UnitsCoordAndPushForce.Check(MAXOBJECT<<2);
		int kk=UnitsCoordAndPushForce.GetAmount();
		if(kk<(MAXOBJECT<<2))
		{
			UnitsCoordAndPushForce.Add(0,(MAXOBJECT<<2)-kk);
		}
		//int Radius=BoidsParams.Radius<<4;
		int i;
		int ii;
		OneObject* OB;
		int* pUnitsCoordAndPushForce=UnitsCoordAndPushForce.GetValues();
		for(i=0;i<MAXOBJECT;i++)
		{
			ii=i<<2;
			OB=Group[i];
			if(OB)
			{
				pUnitsCoordAndPushForce[ii]=OB->RealX;
				pUnitsCoordAndPushForce[ii+1]=OB->RealY;
			}
			else
			{
				pUnitsCoordAndPushForce[ii]=0;
				pUnitsCoordAndPushForce[ii+1]=0;
			}
			pUnitsCoordAndPushForce[ii+2]=0;
			pUnitsCoordAndPushForce[ii+3]=0;
		}
	ENDNEW;
}
void CalculatePushForce()
{
    return;
	int n=NeighboringUnits.GetAmount()/2;
	addrand(n);
	if(n)
	{
        CPF_Stage1();		
		int MinDist=BoidsParams.MinDist<<4;
		int* pUnitsCoordAndPushForce=UnitsCoordAndPushForce.GetValues();
		word ActiveIndex=0xFFFF;
		OneObject* OB1=NULL;
		OneObject* OB2=NULL;
		int p=0;
		int dx;
		int dy;
		int Norm;
		int fx;
		int fy;
		int Rx1,Rx2;
		int Ry1,Ry2;
		int UPos1;
		int UPos2;
		for(int i=0;i<n;i++)
		{
			p=i<<1;
			//OB1=Group[NeighboringUnits[p]];
			//OB2=Group[NeighboringUnits[p+1]];
			UPos1=NeighboringUnits[p]<<2;
			UPos2=NeighboringUnits[p+1]<<2;
			addrand(UPos1);
			addrand(UPos2);
			//if(OB1&&OB2)
			{
				//dx=OB2->RealX-OB1->RealX;
				//dy=OB2->RealY-OB1->RealY;
				int * P1=pUnitsCoordAndPushForce+UPos1;
				int * P2=pUnitsCoordAndPushForce+UPos2;
				dx=(*P2)-(*P1);
				dy=(*(P2+1))-(*(P1+1));
				if(dx&&dy)
				{
					//addrand(dx);
					//addrand(dy);
					Norm=Norma(dx,dy)+1;
					int ddn=MinDist-Norm;
					if(ddn>0)
					{
						fx=(dx*ddn*MinDist)/Norm;
						fy=(dy*ddn*MinDist)/Norm;
						*(P1+2)+=-fx;
						*(P1+3)+=-fy;
						*(P2+2)+= fx;
						*(P2+3)+= fy;
						//addrand(fx);
						//addrand(fy);
					}					
				}
			}
		}
	}
}
void BoidsSingleStep2(OneObject* OB,int &dx, int &dy, int &ChangeSpeed)
{
	if(OB->UnlimitedMotion)
		return;
	UnitsCoordAndPushForce.Check(MAXOBJECT<<2);
	int kk=UnitsCoordAndPushForce.GetAmount();
	if(kk<(MAXOBJECT<<2))
	{
		SAFENEW;
			UnitsCoordAndPushForce.Add(0,(MAXOBJECT<<2)-kk);
		ENDNEW;
	}
	addrand(kk);
	dx>>=4;
	dy>>=4;
	int idd=Norma(dx,dy)+1;
	if(idd>16)
	{
		int idx=(dx*BoidsParams.MainDirNorma)/idd;
		int idy=(dy*BoidsParams.MainDirNorma)/idd;
		
		int UPos=OB->Index<<2;
		int ndx=UnitsCoordAndPushForce[UPos+2];
		addrand(ndx);
		int ndy=UnitsCoordAndPushForce[UPos+3];
		addrand(ndy);
		int ndd=Norma(ndx,ndy)+1;
		addrand(ndd);
		if(ndd>BoidsParams.DensNorma)
		{
			ndx=(ndx*BoidsParams.DensNorma)/ndd;
			ndy=(ndy*BoidsParams.DensNorma)/ndd;
		}
		addrand(ndx);
		addrand(ndy);

		ChangeSpeed=(BoidsParams.ChangeSpeedW*(idx*ndx+idy*ndy)/BoidsParams.MainDirNorma)/1000;
		addrand(ChangeSpeed);
		if(MFIELDS[OB->LockType].CheckPt(OB->x+(ndx>>8),OB->y+(ndy>>8)))
		{
			ndx=0;
			ndy=0;
		}
		dx=idx+ndx;
		dy=idy+ndy;		
	}
}
//==================================================================================================================//
//=====LEADING/SLAVING====//
bool LeadSeveralUnitsAbility::Process(){
	if(ObjID!=0xFFFF&&GetUnitIndex()!=0xFFFF){
		OneObject* OBJ=Group[GetUnitIndex()];
		if(OBJ&&!OBJ->Sdoxlo){
			OneObject* OB=Group[ObjID];
			if(OB&&OB->Serial==ObjSN){
				if(!OB->Sdoxlo){
					OneObject* OBJ=Group[GetUnitIndex()];
					addname(OBJ->Index);
					addname(OB->Index);
					OB->RealX=OBJ->RealX+((int(TCos[OBJ->RealDir])*dx+int(TSin[OBJ->RealDir])*dy)>>4);
					OB->RealY=OBJ->RealY+((int(TCos[OBJ->RealDir])*dy-int(TSin[OBJ->RealDir])*dx)>>4);
					addrand(OB->RealX);addrand(OB->RealY);
					OB->RZ=GetTotalHeight(OB->RealX>>4,OB->RealY>>4);
					addrand(OB->RZ);
					if(OB->RZ<2)OB->RZ=2;
					OB->OverEarth=dz;
					OB->NotSavable=1;
					OB->NotSelectable=1;
					OB->CantRestrictMotion=1;
					OB->StandGround=1;
					OB->DestX=-1;
					if(OB->LocalOrder&&!OB->Attack){
						OB->ClearOrders();
					}
					//OB->RealY+=dz<<3;
					extern MotionField UnitsField;
					UnitsField.BClrBar(OB->x,OB->y,OB->Lx);
					OB->x=(OB->RealX-((OB->Lx)<<7))>>8;
					OB->y=(OB->RealY-((OB->Lx)<<7))>>8;					
					UnitsField.BSetBar(OB->x,OB->y,OB->Lx);
				}
			}
		}
	}
	return true;
}
bool BeSlaveOfUnit::Process(){
	if(ObjID!=0xFFFF){
		OneObject* OBJ=Group[GetUnitIndex()];
		if(OBJ){
			OneObject* OB=Group[ObjID];
			if(OB&&OB->Serial==ObjSN){
				if(OB->Sdoxlo&&OB->Sdoxlo<400){
					if(OBJ->OverEarth>0){
						int dx=((GameSpeed*DeathSpeedX)/256);
						int dy=((GameSpeed*DeathSpeedY)/256);
						int COS=TCos[OB->GraphDir];
						int SIN=TSin[OB->GraphDir];
						int dx1=(dx*COS-dy*SIN)>>8;
						int dy1=(dy*COS+dx*SIN)>>8;
						OBJ->RealX+=dx1;
						OBJ->RealY+=dy1;
						addrand(OB->RealX);
						addrand(OB->RealY);
						LastZ-=(((GameSpeed*OBJ->Sdoxlo)>>8)*DeathAccelerationZ)/16;
						OBJ->OverEarth=LastZ/16;
						if(!OBJ->Sdoxlo)OBJ->Die();
						if(OBJ->OverEarth<=0){
							OBJ->OverEarth=0;							
						}
						return true;
					}
				}
				if(!OBJ->Sdoxlo){
					LastX=OBJ->RealX;
					LastY=OBJ->RealY;
					LastZ=OBJ->OverEarth<<4;
					return true;
				}
			}
			if(!OBJ->Sdoxlo)OBJ->Die();
		}
	}    
	return false;
}
bool UnitsComposition::OnUnitBirth(OneObject* OBJ){
	for(int i=0;i<Units.GetAmount();i++){
		OneUnitInComposition* OUIC=Units[i];
        int id=NATIONS[OBJ->NNUM].CreateNewMonsterAt(OBJ->RealX,OBJ->RealY,OUIC->UnitType,1,OBJ->RealDir);
		if(id!=-1){
			OneObject* OB=Group[id];
			LeadSeveralUnitsAbility* LSUA=new LeadSeveralUnitsAbility;
			LSUA->dx=OUIC->dx;
			LSUA->dy=OUIC->dy;
			LSUA->dz=OUIC->dz;
			LSUA->LeadDistance=OUIC->LeadDist;
			LSUA->ObjID=OB->Index;
			LSUA->ObjSN=OB->Serial;
            LSUA->ActionType=OUIC->LeadingType;
			//LSUA->UnitIndex=OBJ->Index;
			BeSlaveOfUnit* BSOU=new BeSlaveOfUnit;
			BSOU->ObjID=OBJ->Index;
			BSOU->ObjSN=OBJ->Serial;
			//BSOU->UnitIndex=id;
			BSOU->DeathAccelerationZ=OUIC->ZAccelerationWhenDie;
			BSOU->DeathSpeedX=OUIC->XSpeedWhenDie;
			BSOU->DeathSpeedY=OUIC->YSpeedWhenDie;
			CopyToActive(BSOU);
			AddActiveUnitAbility(id,BSOU);
			CopyToActive(LSUA);
			AddActiveUnitAbility(OBJ->Index,LSUA);
			LSUA->Process();
		}
	}
	return true;
}
//==================================================================================================================//
CannonAutoShot::CannonAutoShot()
{
	Visible=false;
}
bool CannonAutoShot::OnUnitBirth(OneObject* Newbie)
{
	if(Newbie)
	{
		CannonAutoShotActive* ab = new CannonAutoShotActive();
		ab->FileID=FileID;
		ab->SpriteID=SpriteID;
		//ab->UnitIndex=Newbie->Index;
		CopyToActive(ab);
		return AddActiveUnitAbility(Newbie->Index, ab);
	}
	return false;
}
//==================================================================================================================//
//==================================================================================================================//
bool GetPushkaChargeState(OneObject* OB,int& ChargeType,int& ChargeStage);
extern int GetEnemyUnitsAmount(byte NI,int xc,int yc,int R);
extern int GetFriendUnitsAmount(byte NI,int xc,int yc,int R);
int GetEnemyApproximateUnitsAmount(byte NI,int xc, int yc, int R)
{
	int rez=0;
	byte mask=NATIONS[NI].NMask;
	itr_UnitsInRadius.Create(xc,yc,R);
	while(OneObject* OB=itr_UnitsInRadius.Next()){
		if(!( (OB->NMask&mask) || OB->Sdoxlo || OB->BrigadeID==0xFFFF)){
			rez++;
		}
	}
	return rez;
}
bool CheckSectorForAutoShot(byte NI,int xc,int yc,byte ANG0,byte ANG1,int R){
	int NE=0;
	int NF=0;
	byte mask=NATIONS[NI].NMask;	
	char DA=ANG1-ANG0;
	if(DA<0){
		swap(ANG1,ANG0);
		DA=-DA;
	}
	byte AA=ANG0+DA/2;
	itr_UnitsInRadius.Create(xc+TCos[AA]*R/512,yc+TSin[AA]*R/512,R/2);
	while(OneObject* OB=itr_UnitsInRadius.Next()){
		if(!OB->Sdoxlo){
			byte A=GetDir(OB->RealX/16-xc,OB->RealY/16-yc);
            char DDA=A-ANG0;
			if(DDA>0&&DDA<DA){
				if(OB->NMask&mask){
					NF++;
					if(NF>3)return false;
				}else if(OB->BrigadeID!=0xFFFF){
					NE++;
				}
			}
		}
	}	
	return (NF<=3 && NE>2);
}
int FindCoordForCannon(OneObject* Cannon,int &xx,int &yy, byte Dir)
{
	int rez=0;
	if(Cannon)
	{
		//byte alfa=C->RealDir+EnmAng[SectorN].Dir;
		//byte from=alfa-16;
		byte from=(((Dir)>>4)<<4)-16;
		AdvCharacter* ADC=Cannon->MoreCharacter;
		int ChargeType;
		int ChargeStage;
		if(GetPushkaChargeState(Cannon,ChargeType, ChargeStage))
		{
			if(ChargeStage==100)
			{
				int MinDist=ADC->AttackRadius1[ChargeType];				
				int MaxDist=ADC->AttackRadius2[ChargeType];
				int MaxDistR=MaxDist;
				if(MinDist==0)MaxDist=MaxDist*10/6;
				if(MinDist<80){
					//kartech
					int pusd=(((Dir+8+512)>>4)&15)<<4;
					if(CheckSectorForAutoShot(Cannon->NNUM,Cannon->RealX/16,Cannon->RealY/16,pusd-13,pusd+13,MaxDist)){
						xx=Cannon->RealX/16+TCos[pusd]/2;
						yy=Cannon->RealY/16+TSin[pusd]/2;
                        return 1;
					}
					return 0;
				}
				MaxDist+=600;
				int StepN=64;
				int Step=(MaxDist-MinDist)/StepN;
				int x=Cannon->RealX>>4;
				int y=Cannon->RealY>>4;
				for(byte i=0;i<32;i++)
				{
					byte an=from+i;
					for(int k=0;k<StepN;k++)
					{
						int D=MinDist+k*Step;
						int dx=x+((TCos[an]*D)>>8);
						int dy=y+((TSin[an]*D)>>8);
						int dist=Norma(x-dx,y-dy);						
						if(dist<MaxDist)
						{
							char EnDir=char(GetDir(dx-x,dy-y));
							int enmd=((EnDir+8+512)>>4)&15;
							int enmd1=enmd;
							int enmd2=enmd;int pusd=((Dir+8+512)>>4)&15;							
							if(enmd==pusd)
							{
								int Razbros=100;//(512*(dist>>5)*ADC->Razbros)/32000;
								int Sq=(314*Razbros*Razbros)>>14;
								//int EAmo=GetEnemyUnitsAmount(Cannon->NNUM,dx,dy,Razbros);
								//int FAmo=GetFriendUnitsAmount(Cannon->NNUM,dx,dy,Razbros);
								int NE=GetEnemyApproximateUnitsAmount(Cannon->NNUM,dx,dy,Razbros);
								if(NE<3)NE=0;
								int Den=(NE*100000)/(Sq+1);//((EAmo-4*FAmo)*100000)/(Sq+1);
								if(Den>rez)
								{
									int DH=Cannon->RZ-GetTotalHeight0(xx,yy);
									if(DH<0)DH=0;
									else DH=(DH*EngSettings.CannonAddShotDistPer100_Height)/100;
									int R=Norma(dx-x,dy-y);
									if(R<MaxDistR+DH){
										rez=Den;
										xx=dx;
										yy=dy;
										if(MinDist==80){
											xx=x+((TCos[Cannon->RealDir]*120)>>8);
											yy=y+((TSin[Cannon->RealDir]*120)>>8);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return rez;
}
int FindCoordForCannon(OneObject* Cannon,int &xx,int &yy)
{
	return FindCoordForCannon(Cannon,xx,yy,Cannon->RealDir);
}
//==================================================================================================================//
CannonAutoShotActive::CannonAutoShotActive()
{
	On=false;
	Hint=GetTextByID("#AutoShot_is_Off");
}
void ClearCannonAutoShot(OneObject* OB){
	if(OB->ActiveAbility){
		int N=OB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i=0;i<N;i++){
			ActiveUnitAbility* AUA=OB->ActiveAbility->ActiveAbilities[i];
			if(!strcmp(AUA->GetClassName(),"CannonAutoShotActive")){
				CannonAutoShotActive* CAS=(CannonAutoShotActive*)AUA;
				if(CAS->On)CAS->OnClick();
			}
		}
	}
}
bool CannonAutoShotActive::Process()
{
	if(On&&GetUnitIndex()!=0xFFFF&&GetUnitIndex()!=-1)
	{
		OneObject* OB = Group[GetUnitIndex()];
		if(OB)
		{
			int GetDestDir(OneObject* OB);
			if( OB->DestX!=-1 || GetDestDir(OB)!=-1 )
			{
				OnClick();
				return true;
			}
			int xx,yy;
			if(FindCoordForCannon(OB,xx,yy))
			{
				OB->NewAttackPoint(xx,yy,128+16,0,0);
			}
		}
		bool TestFillingAbility(OneObject* OB);
		if(TestFillingAbility(OB)){
			OnClick();		
		}
	}	
	return true;
}
bool CannonAutoShotActive::OnClick()
{
	On=!On;
	if(On)
		Hint=GetTextByID("#AutoShot_is_ON");
	else
		Hint=GetTextByID("#AutoShot_is_OFF");
	return true;
}
//==================================================================================================================//
RomeHeroCollector RomeHeroes; 
//==================================================================================================================//
LeveledActiveUnitAbility::LeveledActiveUnitAbility()
{
	Level=1;
	VirtualUp=false;
	Le=&Level;
	Vi=&VirtualUp;
	HintParam=0;
}
bool LeveledActiveUnitAbility::UpLevel()
{
	bool rez=false;
	if(*Le<5)
	{
		(*Le)++;
		rez=true;
	}
	*Vi=false;
	return rez;
}
bool LeveledActiveUnitAbility::OnClick()
{
	if(*Vi)
	{
		if(OB)
		{
			OB->Life+=100;
			OB->MaxLife+=100;
			OB->AddDamage+=5;
			OB->AddShield+=1;
		}
		UpLevel();
		if(OB&&OB->ActiveAbility)
		{
			int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
			for(int i=0;i<n;i++)
			{
				LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[i]);
				if(Lv)
				{
					*(Lv->Vi)=false;
					UnitAbility2* A=(UnitAbility2*)Lv->GetA();
					if(A)
					{
						//Lv->SpriteID=A->SpriteID;
						//Lv->FileID=A->FileID;
					}
				}
			}
		}
		return true;
	}
	return false;
}
bool LeveledActiveUnitAbility::CanApply()
{
	return *Vi;
}
const char* LeveledActiveUnitAbility::GetHint()
{
	TempHint.Clear();
	for(int i=1;i<6;i++)
	{
		if(Le&&i==*Le)
		{
			TempHint.Add("{CR}");
			TempHint.Add(GetHintOnLevel(i));
			TempHint.Add("{C}");
			TempHint.Add("\\");
		}
		else
		{
			TempHint.Add(GetHintOnLevel(i));
			TempHint.Add("\\");
		}
	}
	/*
	TempHint=Hint.pchar();//GetTextByID(Hint.pchar());
	static char tm[256];
	tm[0]=0;
	if(HintParam)
	{
		//TempHint.Add(" %d");
		sprintf(tm,TempHint.pchar(),HintParam);
	}
	else
	{
		strcpy(tm,TempHint.pchar());
	}
	//TempHint=GetTextByID("Level");
	//TempHint.Add(" ");
	//TempHint.Add(*Le);
	//TempHint.Add("\\");
	TempHint=tm;
	*/
	return TempHint.pchar();
}
const char* LeveledActiveUnitAbility::GetHintOnLevel(int Lev)
{
	TempHintLv=Hint.pchar();
	if(Lev>0&&Lev<6)
	{
		int HP = GetHintParamOnLevel(Lev);
		static char tm[256];
		tm[0]=0;
		if(HP)
		{
			sprintf(tm,TempHintLv.pchar(),HP);
		}
		else
		{
			strcpy(tm,TempHintLv.pchar());
		}
		TempHintLv=tm;
	}
	return TempHintLv.pchar();
}
int LeveledActiveUnitAbility::GetHintParamOnLevel(int Lev)
{
	return 0;
}
//==================================================================================================================//
RomeHero::RomeHero()
{
	ExpProcIfNotKiller=100;
	IsHero=false;
	GatherExperienceInCampaign=false;
	UpOrder[0]=0;
	UpOrder[1]=0;
	UpOrder[2]=0;
	StayBack=false;
}
ActiveUnitAbility* RomeHero::GetNewActiveAbility()
{
	return new ActiveRomeHero();
}
//==================================================================================================================//
ActiveRomeHero::ActiveRomeHero()
{
	Level=1;
	Expa=0;
	ExpaToNextLevel=-1;
	WaitForAbilUp=false;
	LoadFromProfile=false;
	Le=NULL;
	Ex=NULL;
	Wa=NULL;
}
ActiveRomeHero::~ActiveRomeHero()
{
	RomeHeroes.DelRomeHero(this);
}
int ActiveRomeHero::GetExperienceToNextLevel()
{
	int rez=-1;
	RomeHero* Hero = (RomeHero*) GetA();
	if(Hero&&Le)
	{
		int n=Hero->LevelUp.GetAmount();
		if(*Le<=n)
		{
			for(int i=0;i<*Le;i++)
				rez+=Hero->LevelUp[i];
			if(rez>-1){
				rez++;
				//rez-=*Ex;
			}
		}	
	}
	return rez;
}
int ActiveRomeHero::UpLevel()
{
	return ++*Le;
}
int ActiveRomeHero::AddExperience(int Expa)
{
	return *Ex+=Expa*100;
}
int ActiveRomeHero::GetLevel()
{
	if(Le)
		return *Le;
	else
		return Level;
}
int ActiveRomeHero::GetExperience()
{
	if(Ex)
		return *Ex/100;
	else
		return Expa;
}
void ActiveRomeHero::GatherExperience(OneObject* Victim,OneObject* Killer, byte Part)
{
	RomeHero* Hero = (RomeHero*) GetA();
	if(Hero&&Ex&&((vGameMode==gmCamp&&Hero->GatherExperienceInCampaign)||vGameMode!=gmCamp))
	{
		int exp=0;
		if(OB==Killer)
			exp=Victim->newMons->Expa*100;
		else if (Part!=0)
			exp=(Victim->newMons->Expa*Hero->ExpProcIfNotKiller)/Part;
			//exp=(Victim->newMons->Expa*Hero->ExpProcIfNotKiller/100)/Part;
		(*Ex)+=exp;
		Hint=GetTextByID("Level: ");
		Hint.Add(*Le);
		Hint.Add(GetTextByID("\\ Experience: "));
		Hint.Add(*Ex/100);
	}
}
bool ActiveRomeHero::Process(OneObject* OB)
{
	if(!LoadFromProfile){
		if(vGameMode==gmCamp){
			int n=vmSinglePlayerData.Heri.GetAmount();
			SinglePlayerData_CampaignHeroInfo* CHI=NULL;
			if(n<=vmCampID){
				for(int i=0;i<vmCampID-n+1;i++){
					CHI = new SinglePlayerData_CampaignHeroInfo();
					vmSinglePlayerData.Heri.Add(CHI);
				}
			}else{
				CHI = vmSinglePlayerData.Heri[vmCampID];
			}
			SinglePlayerData_HeroesInfoList* HIL=NULL;
			SinglePlayerData_HeroesInfoList* PreviosHIL=NULL;
			SinglePlayerData_HeroInfo* PreviosHr=NULL;
			if(CHI){
				int nn=CHI->CampaignMissions.GetAmount();
				int nm=vmCampaigns[vmCampID]->curMission;
				if(nn<=nm){
					for(int i=0;i<nm-nn+1;i++){
						HIL = new SinglePlayerData_HeroesInfoList();
						CHI->CampaignMissions.Add(HIL);
					}
				}else{
					HIL = CHI->CampaignMissions[nm];
				}
				int PreviosMissID=nm-1;
				while(PreviosHr==NULL&&PreviosMissID>-1&&PreviosMissID<=nn){
					PreviosHIL = CHI->CampaignMissions[PreviosMissID];
					if(PreviosHIL){
						int nnn=PreviosHIL->MissionHeroes.GetAmount();
						for(int i=0;i<nnn;i++){
							if(PreviosHIL->MissionHeroes[i]->HeroType==OB->NIndex){
								PreviosHr=PreviosHIL->MissionHeroes[i];
								break;
							}
						}
					}
					PreviosMissID--;
				}
			}
			//
			SinglePlayerData_HeroInfo* Hr=NULL;
			int nnn=HIL->MissionHeroes.GetAmount();
			for(int i=0;i<nnn;i++){
				if(HIL->MissionHeroes[i]->HeroType==OB->NIndex){
					Hr=HIL->MissionHeroes[i];
					break;
				}
			}
			
			
			if(!Hr){
				Hr = new SinglePlayerData_HeroInfo();
				HIL->MissionHeroes.Add(Hr);
				Hr->HeroType=OB->NIndex;
				Hr->Level=Level;
				Hr->Expa=Expa;
				Hr->WaitForAbilUp=WaitForAbilUp;
			}
			
			Le=&Hr->Level;
			Ex=&Hr->Expa;
			Wa=&Hr->WaitForAbilUp;
			if(!vGameLoaing){
				*Le=1;
				*Ex=0;
				*Wa=false;
			}
			
			n = OB->ActiveAbility->ActiveAbilities.GetAmount();
			
			int In=0;
			int na=Hr->Abilki.GetAmount();
			for(int i=0;i<n;i++)
			{
				LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[i]);
				if(Lv)
				{
					SinglePlayerData_HeroAbilityInfo* AInfo = NULL;
					if(In<na)
					{
						AInfo=Hr->Abilki[In];
					}
					else
					{
						AInfo = new SinglePlayerData_HeroAbilityInfo();
						Hr->Abilki.Add(AInfo);
					}
					Lv->Le=&AInfo->Level;
					Lv->Vi=&AInfo->VirtualUp;
					if(!vGameLoaing)
					{
						*Lv->Le=1;
						*Lv->Vi=false;
					}
					In++;
				}
			}

			if(PreviosHr&&!vGameLoaing)
			{
				*Le=PreviosHr->Level;
				*Ex=PreviosHr->Expa;
				*Wa=PreviosHr->WaitForAbilUp;
				*Wa=false;

				n = OB->ActiveAbility->ActiveAbilities.GetAmount();
				int In=0;
				int na=PreviosHr->Abilki.GetAmount();
				for(int i=0;i<n;i++)
				{
					LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[i]);
					if(Lv)
					{
						UnitAbility2* A=(UnitAbility2*)Lv->GetA();
						if(A)
						{
							if(In<na)
							{
								int l=PreviosHr->Abilki[In]->Level;
								//*(Lv->Le)=l;
								if(*(Lv->Vi))
								{
									//Lv->SpriteID=A->SpriteID2;
									//Lv->FileID=A->FileID2;
								}
								for(int k=1;k<l;k++)
								{
									Lv->UpLevel();
								}
								*(Lv->Vi)=PreviosHr->Abilki[In]->VirtualUp;
								*Lv->Vi=false;
							}
							In++;
						}
					}
				}
			}			
			
		}else{
			Le=&Level;
			Ex=&Expa;
			Wa=&WaitForAbilUp;
			//
			for(int j=0;j<MAXOBJECT;j++)
			{
				OneObject* OO = Group[j];
				if(OO&&OO->NIndex==OB->NIndex&&OO->NNUM==OB->NNUM&&OO->Sdoxlo>=400)
				{
					int n = OO->ActiveAbility->ActiveAbilities.GetAmount();
					int inL=0;
					for(int i=0;i<n;i++)
					{
						LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OO->ActiveAbility->ActiveAbilities[i]);
						if(Lv)
						{
							int m = OB->ActiveAbility->ActiveAbilities.GetAmount();
							int inLbase=0;
							for(int k=0;k<m;k++)
							{
								LeveledActiveUnitAbility* LvNew = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[k]);
								if(LvNew)
								{
									if(inL==inLbase)
									{
										*LvNew->Le=*(Lv->Le);
										*LvNew->Vi=*(Lv->Vi);
										break;
									}
									inLbase++;
								}
							}
							inL++;
						}
						else
						{
							ActiveRomeHero *OORh = dynamic_cast<ActiveRomeHero*> (OO->ActiveAbility->ActiveAbilities[i]);
							if(OORh)
							{
								*Le=*(OORh->Le);
								*Ex=*(OORh->Ex);
								*Wa=*(OORh->Wa);
							}
						}
					}
					OO->Sdoxlo=1001;
					OB->MaxLife=OO->MaxLife;
					OB->Life=OO->MaxLife;
					OB->AddDamage=OO->AddDamage;
					OB->AddShield=OO->AddShield;
					//
					break;
				}
			}
		}
		RomeHeroes.AddRomeHero(this);
		LoadFromProfile=true;
	}


	if(vGameMode!=gmCamp)
	{	
		if(ExpaToNextLevel==-1)
		{
			ExpaToNextLevel=GetExperienceToNextLevel();
		}
		if((*Ex/100)>=ExpaToNextLevel&&ExpaToNextLevel!=-1&&!(*Wa))
		{
			(*Le)++;
			ExpaToNextLevel=GetExperienceToNextLevel();
			if(OB&&OB->ActiveAbility)
			{
				int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
				for(int i=0;i<n;i++)
				{
					LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[i]);
					if(Lv&&*(Lv->Le)<5)
					{
						*(Lv->Vi)=true;
						(*Wa)=true;
						/*
						UnitAbility2* A=(UnitAbility2*)Lv->GetA();
						if(A)
						{
							//Lv->SpriteID=A->SpriteID2;
							//Lv->FileID=A->FileID2;
						}
						*/
					}
				}
			}
			if(OB&&OB->NNUM==GetMyNation())
			{
				void PlaySound(char* Name);
				PlaySound("HERO_CAN_UP_LEVEL");
			}

			Hint=GetTextByID("Level: ");
			Hint.Add(*Le);
			Hint.Add(GetTextByID("\\ Experience: "));
			Hint.Add(*Ex);
		}
		if(*Wa)
		{
			if(OB&&OB->ActiveAbility)
			{
				int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
				*Wa=false;
				for(int i=0;i<n;i++)
				{
					LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (OB->ActiveAbility->ActiveAbilities[i]);
					if(Lv)
					{
						if(*(Lv->Vi))
						{
							*Wa=true;
							break;
						}
					}
				}
			}
		}
	}
	if(OB->Sdoxlo>1000)
	{
		return false;
	}
	if(OB->Sdoxlo>400)
	{
		OB->Sdoxlo=400;
	}
	return true;
}
bool ActiveRomeHero::IsHero()
{
	RomeHero* HL = (RomeHero*)GetA();
	if(HL)
	{
		return HL->IsHero;
	}
	return false;
}
int ActiveRomeHero::GetAmountFreeLevels()
{
	int rez=0;
	if(Le&&Wa&&*Wa)
	{
		RomeHero* H = (RomeHero*)GetA();
		if(H)
		{
			int n = H->LevelUp.GetAmount();
			int Exp=0;
			for(int i=0;i<n;i++)
			{
				Exp+=H->LevelUp[i];
				if((i+2)>=*Le&&Exp!=0)
				{
					if(Exp<=*Ex/100)
					{
						rez++;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return rez;
}
//==================================================================================================================//
RomeHeroCollector::RomeHeroCollector()
{
	Calk=false;
}
void RomeHeroCollector::ClearAll()
{
	//Herosima.Clear();
	//NHeroInMap.Clear();
}
bool RomeHeroCollector::AddExp(OneObject* Victim,OneObject* Killer)
{
	//int n=Herosima.GetAmount();
	//if(!Calk)
	//{
	//	int tm=0;
	//	NHeroInMap.Clear();
	//	for(int i=0;i<8;i++)NHeroInMap.Add(0);
	//	for(i=0;i<n;i++)
	//	{
	//		ActiveRomeHero* RH=Herosima[i];
	//		if(RH&&RH->OB&&!RH->OB->Sdoxlo)
	//		{
	//			NHeroInMap[RH->OB->NNUM]++;
	//		}
	//	}
	//	Calk=true;
	//}
	//int nnh=NHeroInMap.GetAmount();
	//if(nnh>Killer->NNUM)
	//{
	//	int nh=NHeroInMap[Killer->NNUM];
	//	for(int i=0;i<n;i++)
	//	{
	//		ActiveRomeHero* RH=Herosima[i];
	//		if(RH&&RH->OB&&RH->OB->NNUM==Killer->NNUM)
	//		{
	//			if(!RH->OB->Sdoxlo)
	//			{
	//				RH->GatherExperience(Victim,Killer,NHeroInMap[RH->OB->NNUM]);
	//			}
	//		}
	//	}
	//}
	return true;
}
void RomeHeroCollector::AddRomeHero(ActiveRomeHero* Her)
{
	//Herosima.Add(Her);
	//Calk=false;
}
void RomeHeroCollector::DelRomeHero(ActiveRomeHero* Her)
{
	//int n=Herosima.GetAmount();
	//for(int i=0;i<n;i++)
	//{
	//	if(Herosima[i]==Her)
	//	{
	//		Herosima[i]=NULL;
	//	}
	//}
	//Calk=false;
}
//==================================================================================================================//
LifeLimitation::LifeLimitation()
{
	DieWeaponEffect="";
}
bool LifeLimitation::OnUnitBirth(OneObject* Newbie)
{
	bool rez=false;
	if(Newbie)
	{
		ActiveLifeLimitation* ab = new ActiveLifeLimitation();
		//ab->UnitIndex=Newbie->Index;
		ab->OB=Newbie;
		ab->SetA(this);
		//ab->HVS=GetHeroVariableStorage(Newbie);
		ab->DieTime=AnimTime+LifeLength.Get()*25*256;
		CopyToActive(ab);
		return AddActiveUnitAbility(Newbie->Index, ab);
	}
	return rez;
}
//==================================================================================================================//
ActiveLifeLimitation::ActiveLifeLimitation()
{
	DieTime=0;
}
bool ActiveLifeLimitation::Process()
{
	bool rez=false;
	if(OB&&!OB->Sdoxlo)
	{
		if(DieTime!=0&&AnimTime>DieTime)
		{
			OB->Die();
			LifeLimitation* ab = (LifeLimitation*)GetA();
			if(ab->DieWeaponEffect.pchar()!="")
			{
				AdditionalWeaponParams* AWP = new AdditionalWeaponParams();
				CreateNewActiveWeapon(ab->DieWeaponEffect.pchar(),OB->Index,OB->RealX>>4,OB->RealY>>4,OB->RZ,0xFFFF,OB->RealX>>4,OB->RealY>>4,OB->RZ,AWP);
			}
		}
		else
		{
			rez=true;
		}
	}
	return rez;
}
//==================================================================================================================//
GreetingMe::GreetingMe()
{
	Radius=0;
}
bool GreetingMe::OnUnitBirth(OneObject* Newbie)
{
	bool rez=false;
	if(Newbie)
	{
		ActiveGreetingMe* ab = new ActiveGreetingMe();
		//ab->UnitIndex=Newbie->Index;
		ab->OB=Newbie;
		ab->Visible=Visible;
		ab->SetA(this);
		ab->Hint.Add(Name);
		CopyToActive(ab);
		return AddActiveUnitAbility(Newbie->Index, ab);
	}
	return rez;
}
//==================================================================================================================//
ActiveGreetingMe::ActiveGreetingMe()
{
	UseTime=0;
}
void TestCGARB();
bool ActiveGreetingMe::Process()
{
	bool rez=false;
	if((!OB)&&GetUnitIndex()!=0xFFFF)
	{
		OB=Group[GetUnitIndex()];
	}
	if(OB&&!OB->Sdoxlo)
	{
		if(OB->DestX>0)
		{
			if((UseTime+256*25*8)<AnimTime)
			{
				GreetingMe* Gm = (GreetingMe*)GetA();
				if(Gm)
				{
					int Param[6];
					Param[0]=OB->RealX>>4;
					Param[1]=OB->RealY>>4;
					Param[3]=OB->NNUM;
					Param[4]=OB->Index;
					PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,Gm->Radius,&ActiveGreetingMe::AddActiveGreeting,(void*)Param);
					UseTime=AnimTime;
				}
			}
		}
		rez=true;
	}
	return rez;
}
bool ActiveGreetingMe::AddActiveGreeting(OneObject* OB,void* Param)
{
	bool rez=false;
	if(OB&&!OB->Sdoxlo&&Param)
	{
		int* P = (int*) Param;
		if(OB->NNUM==P[3]&&OB->LocalOrder==NULL&&OB->Index!=P[4]&&OB->EnemyID==0xFFFF&&OB->DestX==-1&&OB->NewState==0)
		{
			OneObject* GreetingObject=Group[P[4]];
			char Dirr=(char)GetDir(GreetingObject->RealX-OB->RealX,GreetingObject->RealY-OB->RealY);
			char dd = char(OB->RealDir)-Dirr;
			if(abs(dd)<60)
			{
				if(OB->ActiveAbility)
				{
					int n = OB->ActiveAbility->ActiveAbilities.GetAmount();
					for(int i=0;i<n;i++)
					{
						if(OB->ActiveAbility->ActiveAbilities[i]->Type==9)
							return false;
					}
				}
				ActiveGreeting* g = new ActiveGreeting();
				g->GreetingObject=Group[P[4]];
				g->OB=OB;
				//g->UnitIndex=OB->Index;
				//CopyToActive(g);
				if(AddActiveUnitAbility(OB->Index, g))
				{
					rez=true;
				}
				else
				{
					delete g;
				}
			}
		}
	}
	return rez;
}
//==================================================================================================================//
ActiveGreeting::ActiveGreeting()
{
	Type=9;
	GreetingObject=NULL;
	PrevDir=-1;
	State=0;
}
bool ActiveGreeting::Process()
{
	if((!OB)&&GetUnitIndex()!=0xFFFF)
	{
		OB=Group[GetUnitIndex()];
	}
	if(OB&&GreetingObject&&!OB->Sdoxlo&&OB->EnemyID==0xFFFF&&OB->DestX==-1)
	{
		if(OB->FrameFinished)
		{
			if(State==0)
			{
				char Dirr=(char)GetDir(GreetingObject->RealX-OB->RealX,GreetingObject->RealY-OB->RealY);
				char dd = char(OB->RealDir)-Dirr;
				if(abs(dd)<50)
				{
					NewAnimation* NA =OB->newMons->GetAnimation(anm_Greeting);
					if(NA&&NA->Enabled)
					{
						OB->NewAnm=NA;
						OB->SetZeroFrame();
						State=2;
						return true;
					}
				}
				/*
				NewAnimation* NA =OB->newMons->GetAnimation(anm_Greeting);
				if(NA&&NA->Enabled)
				{
					if(abs(dd)<50)
					{
						State=1;
						return true;
					}
					
					if(OB->BrigadeID==0xFFFF)
					{
						if(abs(dd)>16)
						{
							PrevDir=OB->RealDir;
							RotateMon(OB,-dd);
						}
						State=1;
					}
					else
					{
						if(abs(dd)<60)
						{
							State=1;
						}
						else
						{
							return false;
						}
					}
				}
				*/
				return false;
			}
			/*
			if(State==1)
			{
				NewAnimation* NA =OB->newMons->GetAnimation(anm_Greeting);
				if(NA&&NA->Enabled)
				{
					OB->NewAnm=NA;
					OB->SetZeroFrame();
					State=2;
					return true;
				}
			}
			*/
			if(State==2)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}
//==================================================================================================================//
NationBonus::NationBonus()
{
	AddDamage=0;
	AddShield=0;
}
bool NationBonus::OnUnitBirth(OneObject* Newbie)
{
	bool rez=false;
	if(Newbie)
	{
		if(AddDamage||AddShield||UseUpgrade)
		{
			ActiveNationBonus* ab = new ActiveNationBonus();
			//ab->UnitIndex=Newbie->Index;
			ab->OB=Newbie;
			ab->Visible=Visible;
			ab->FileID=FileID;
			ab->SpriteID=SpriteID;
			ab->Hint.Add(GetTextByID(Name.pchar()));
			ab->SetA(this);
			CopyToActive(ab);
			if(AddActiveUnitAbility(Newbie->Index, ab))
			{
				int N = UnitsType.GetAmount();
				if(N)
				{
					for(int i=0;i<N;i++)
					{
						AdvCharacter* ADV = NATIONS[Newbie->NNUM].Mon[UnitsType[i]->UnitType]->MoreCharacter;
						if(ADV)
						{
							for(int j=0;j<NAttTypes;j++)
							{
								if(ADV->MaxDamage[j])
									ADV->MaxDamage[j]+=AddDamage;
							}
							ADV->Shield+=AddShield;
						}
					}
				}
				else
				{
					for(int i=0;i<NATIONS[Newbie->NNUM].NMon;i++)
					{
						AdvCharacter* ADV = NATIONS[Newbie->NNUM].Mon[i]->MoreCharacter;
						if(ADV)
						{
							for(int j=0;j<NAttTypes;j++)
							{
								if(ADV->MaxDamage[j])
									ADV->MaxDamage[j]+=AddDamage;
							}
							ADV->Shield+=AddShield;
						}
					}
				}
				if(UseUpgrade)
				{
					PerformNewUpgrade(NATIONS+Newbie->NNUM,UpgradeBonus,NULL);
				}
				rez=true;
			}
		} 
	}
	return rez;
}
//==================================================================================================================//
ActiveNationBonus::ActiveNationBonus()
{
	m_die=false;
}
bool ActiveNationBonus::Process()
{
	bool rez=false;
	if((!OB)&&GetUnitIndex()!=0xFFFF)
	{
		OB=Group[GetUnitIndex()];
	}
	if(OB)
	{
		if(*Le<6&&*Le>0)
		{
			NationBonus* NB = (NationBonus*) GetA();
			if(NB)
			{
				HintParam=NB->HintParam[*Le-1];
			}
		}
		rez=true;
	}
	if((!OB)||(OB&&OB->Sdoxlo))
	{
		rez=false;
		if(!m_die)
		{
			NationBonus* NB = (NationBonus*) GetA();
			if(NB)
			{
				int N = NB->UnitsType.GetAmount();
				if(N)
				{
					for(int i=0;i<N;i++)
					{
						AdvCharacter* ADV = NATIONS[OB->NNUM].Mon[NB->UnitsType[i]->UnitType]->MoreCharacter;
						if(ADV)
						{
							for(int j=0;j<NAttTypes;j++)
							{
								if(ADV->MaxDamage[j])
									ADV->MaxDamage[j]-=NB->AddDamage;
							}
							ADV->Shield-=NB->AddShield;
						}
					}
				}
				else
				{
					for(int i=0;i<NATIONS[OB->NNUM].NMon;i++)
					{
						AdvCharacter* ADV = NATIONS[OB->NNUM].Mon[i]->MoreCharacter;
						if(ADV)
						{
							for(int j=0;j<NAttTypes;j++)
							{
								if(ADV->MaxDamage[j])
									ADV->MaxDamage[j]-=NB->AddDamage;
							}
							ADV->Shield-=NB->AddShield;
						}
					}
				}
				if(NB->UseUpgrade)
				{
					if((*Le)>4)
						PerformNewUpgrade(NATIONS+OB->NNUM,NB->IfDieDowngradeL5,NULL);
					if((*Le)>3)
						PerformNewUpgrade(NATIONS+OB->NNUM,NB->IfDieDowngradeL4,NULL);
					if((*Le)>2)
						PerformNewUpgrade(NATIONS+OB->NNUM,NB->IfDieDowngradeL3,NULL);
					if((*Le)>1)
						PerformNewUpgrade(NATIONS+OB->NNUM,NB->IfDieDowngradeL2,NULL);
					PerformNewUpgrade(NATIONS+OB->NNUM,NB->IfDieDowngrade,NULL);
				}
			}
			m_die=true;
		}
	}
	if(OB&&OB->Sdoxlo>0&&OB->Sdoxlo<1000)
		rez=true;
	return rez;
}
bool ActiveNationBonus::UpLevel()
{
	bool rez=false;
	NationBonus* NB = (NationBonus*) GetA();
	if(*Le<5&&NB)
	{
		(*Le)++;
		int Up=-1;
		if(*Le==2)
			Up=NB->UpgradeBonusL2;
		if(*Le==3)
			Up=NB->UpgradeBonusL3;
		if(*Le==4)
			Up=NB->UpgradeBonusL4;
		if(*Le==5)
			Up=NB->UpgradeBonusL5;
		if(Up!=-1)
		{
			PerformNewUpgrade(NATIONS+OB->NNUM,Up,NULL);
			rez=true;
		}
	}
	return rez;
}
int ActiveNationBonus::GetHintParamOnLevel(int Lev)
{
	if(Lev<6&&Lev>0)
	{
		NationBonus* NB = (NationBonus*) GetA();
		if(NB)
		{
			return NB->HintParam[Lev-1];
		}
	}
	return 0;
}
//==================================================================================================================//
HeroLight::HeroLight()
{
	EffectName="";
	DieEffect="";
	IsHero=true;
}
bool HeroLight::OnUnitBirth(OneObject* Newbie)
{
	if(Newbie)
	{
		ActiveHeroLight* AA = new ActiveHeroLight();		
		//AA->UnitIndex=Newbie->Index;
		AA->OB=Newbie;
		AA->SetA(this);
		AA->Visible=Visible;
		CopyToActive(AA);
		return AddActiveUnitAbility(Newbie->Index, AA);
	}
	return false;
}
//==================================================================================================================//
ActiveHeroLight::ActiveHeroLight()
{
	IsInit=false;
}
bool ActiveHeroLight::IsHero()
{
	HeroLight* HL = (HeroLight*)GetA();
	if(HL)
	{
		return HL->IsHero;
	}
	return false;
}
bool ActiveHeroLight::Process()
{
	bool rez=false;
	if(!OB)
	{
		if(GetUnitIndex()!=0xFFFF)
			OB=Group[GetUnitIndex()];
	}
	HeroLight* HL = (HeroLight*)GetA();
	if(OB&&HL)
	{
		if(!OB->Sdoxlo)
		{
			if(!IsInit&&HL->EffectName.L>0)
			{

				CreateNewActiveWeapon(HL->EffectName.pchar(),OB->Index,OB->RealX>>4,OB->RealY>>4,OB->RZ,OB->Index,OB->RealX>>4,OB->RealY>>4,OB->RZ,NULL);
				IsInit=true;
			}
			rez=true;
		}
		else
		{
			if(HL->DieEffect.L>0)
				CreateNewActiveWeapon(HL->DieEffect.pchar(),OB->Index,OB->RealX>>4,OB->RealY>>4,OB->RZ,OB->Index,OB->RealX>>4,OB->RealY>>4,OB->RZ,NULL);
		}
	}
	return rez;
}
//==================================================================================================================//
LifeRegeneration::LifeRegeneration()
{
	Regeneration=0;
	Radius=0;
	UsePause=0;
	CoolDownTime=0;
}
ActiveUnitAbility* LifeRegeneration::GetNewActiveAbility()
{
	return (ActiveUnitAbility*) new ActiveLifeRegeneration();
}
//==================================================================================================================//
ActiveLifeRegeneration::ActiveLifeRegeneration()
{
	LastUseTime=0;
}
bool ActiveLifeRegeneration::Process()
{
	bool rez=false;
	if(!OB)
	{
		if(GetUnitIndex()!=0xFFFF)
			OB=Group[GetUnitIndex()];
	}
	LifeRegeneration* A = (LifeRegeneration*)GetA();
	if(OB&&(!OB->Sdoxlo)&&A)
	{
		if(!A->CoolDownTime)
		{
			if(AnimTime>(LastUseTime+A->UsePause*256*25/10))
			{
				Exec();
			}
		}
		rez=true;
	}
	return rez;
}
bool ActiveLifeRegeneration::CanApply()
{
	LifeRegeneration* A = (LifeRegeneration*)GetA();
	if(A&&A->CoolDownTime)
	{
		return true;
	}
	return false;
}
int ActiveLifeRegeneration::GetCoolDownProc()
{
	int rez=0;
	LifeRegeneration* A = (LifeRegeneration*)GetA();
	if(A&&A->CoolDownTime)
	{
		int cool=A->CoolDownTime*25*256;
		if(cool)
			rez=(LastUseTime+cool-AnimTime)*100/cool;
		if(rez<0)
			rez=0;
	}
	return rez;
}
bool ActiveLifeRegeneration::ShowRadius()
{
	LifeRegeneration* A = (LifeRegeneration*)GetA();
	if(OB&&A&&A->Radius>10)
	{
		AddGroundCircle(OB->RealX>>4,OB->RealY>>4,A->Radius,0xFFFFFF00);
		return true;
	}
	return false;
}
bool ActiveLifeRegeneration::OnClick()
{
	LifeRegeneration* A = (LifeRegeneration*)GetA();
	if(A&&A->CoolDownTime)
	{
		if(GetCoolDownProc()==0)
		{
			return Exec();
		}
	}
	return false;
}
bool ActiveLifeRegeneration::Exec()
{
	LifeRegeneration* A = (LifeRegeneration*)GetA();
	if(OB&&A)
	{
		LastUseTime=AnimTime;
		PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,A->Radius,&ActiveLifeRegeneration::AddLife,(void*)this);
		return true;
	}
	return false;
}
bool ActiveLifeRegeneration::AddLife(OneObject* OB, void* Param)
{
	if(OB&&!OB->Sdoxlo)
	{
		ActiveLifeRegeneration* AC = (ActiveLifeRegeneration*)Param;
		LifeRegeneration* A = (LifeRegeneration*)AC->GetA();
		if((A->Regeneration>0&&OB->NMask&AC->OB->NMask)||(A->Regeneration<0&&!(OB->NMask&AC->OB->NMask)))
		{
			OB->Life+=A->Regeneration;
			if(OB->Life>OB->MaxLife)
				OB->Life=OB->MaxLife;
			if(OB->Life<0)
				OB->Life=0;
			return true;
		}
	}
	return false;
}
//==================================================================================================================//
SetMineBonus::SetMineBonus()
{
	Bonus=0;
	Radius=0;
}
ActiveUnitAbility* SetMineBonus::GetNewActiveAbility()
{
	return (ActiveUnitAbility*) new ActiveSetMineBonus();
}
//==================================================================================================================//
ActiveSetMineBonus::ActiveSetMineBonus()
{
	LastUseTime=0;
}
bool ActiveSetMineBonus::Process()
{
	bool rez=false;
	if((!OB)&&GetUnitIndex()!=0xFFFF)
	{
		OB=Group[GetUnitIndex()];
	}
	if(OB&&!OB->Sdoxlo)
	{
		SetMineBonus* A = (SetMineBonus*)GetA();
		if(A&&A->Bonus)
		{
			if(AnimTime>(LastUseTime+25*256))
			{
				HintParam=A->Bonus+((*Le)-1)*A->BonusAddForLevel;
				PerformActionOverBuildingsInRadius(OB->RealX>>4,OB->RealY>>4,A->Radius,&ActiveSetMineBonus::AddMineBonus,(void*)this);
				LastUseTime=AnimTime;
			}
			rez=true;
		}
	}
	if(OB&&OB->Sdoxlo>0&&OB->Sdoxlo<1000&&!rez)
		rez=true;
	return rez;
}
int ActiveSetMineBonus::GetRadius()
{
	int rez=0;
	if(OB)
	{
		SetMineBonus* A=(SetMineBonus*)GetA();
		if(A)
			rez=A->Radius;
	}
	return rez;
}
int ActiveSetMineBonus::GetHintParamOnLevel(int Lev)
{
	if(Lev<6&&Lev>0)
	{
		SetMineBonus* A=(SetMineBonus*)GetA();
		if(A)
		{
			return A->Bonus+(Lev-1)*A->BonusAddForLevel;
		}
	}
	return 0;
}
bool ActiveSetMineBonus::AddMineBonus(OneObject* OB, void* Param)
{
	if(OB&&OB->Usage==MineID&&!OB->Sdoxlo)
	{
		ActiveSetMineBonus* AA=(ActiveSetMineBonus*)Param;
		SetMineBonus* A=(SetMineBonus*)AA->GetA();
		if((A->Bonus>0&&OB->NMask&AA->OB->NMask)||(A->Bonus<0&&!(OB->NMask&AA->OB->NMask)))
		{
			bool have=false;
			if(OB->ActiveAbility)
			{
				int n= OB->ActiveAbility->ActiveAbilities.GetAmount();
				for(int i=0;i<n;i++)
				{
					if(OB->ActiveAbility->ActiveAbilities[i]->Type==13)
					{
						ActiveMineBonus* AM = (ActiveMineBonus*)OB->ActiveAbility->ActiveAbilities[i];
						if(AM->HeroIndex==AA->OB->Index)
						{
							have=true;
							break;
						}
					}
				}
			}
			if(!have)
			{
				/*
				bool atype=false;
				int n=A->MineType.GetAmount();
				for(int i=0;i<n;i++)
				{
					if(OB->Usage==A->MineType[i]->UnitType)
					{
						atype=true;
						break;
					}
				}
				*/
				//if(atype)
				{
					ActiveMineBonus* AMB = new ActiveMineBonus();
					AMB->Radius=A->Radius;
					AMB->Bonus=A->Bonus+(*(AA->Le)-1)*A->BonusAddForLevel;
					AMB->HeroIndex=AA->OB->Index;
					AMB->OB=OB;
					//AMB->UnitIndex=OB->Index;
					//CopyToActive(AMB);
					AddActiveUnitAbility(OB->Index,AMB);
				}
			}
		}
	}
	return false;
}
//==================================================================================================================//
ActiveMineBonus::ActiveMineBonus()
{
	Type=13;
	Radius=0;
	Bonus=0;
	HeroIndex=-1;
}
bool ActiveMineBonus::Process()
{
	bool rez=false;
	if((!OB)&&GetUnitIndex()!=0xFFFF)
	{
		OB=Group[GetUnitIndex()];
	}
	if(OB&&!OB->Sdoxlo&&HeroIndex!=-1)
	{
		OneObject* Her = Group[HeroIndex];
		if(Her&&!Her->Sdoxlo)
		{
			int d = Norma((OB->RealX>>4)-(Her->RealX>>4),(OB->RealY>>4)-(Her->RealY>>4));
			if(d<Radius)
			{
				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
IncreaseMaxLife::IncreaseMaxLife()
{
	Points=0;
	Radius=0;
}
ActiveUnitAbility* IncreaseMaxLife::GetNewActiveAbility()
{
	return (ActiveUnitAbility*) new ActiveIncreaseMaxLife();
}
//==================================================================================================================//
ActiveAdditionalLife::ActiveAdditionalLife()
{
	Type=14;
	HeroIndex=-1;
	LifeAdded=0;
}
bool ActiveAdditionalLife::Process(OneObject* OB)
{
	bool rez=false;
	if(HeroIndex!=-1&&!OB->Sdoxlo)
	{
		IncreaseMaxLife* A = (IncreaseMaxLife*)GetA();
		if(A)
		{
			OneObject* H = Group[HeroIndex];
			if(H&&!H->Sdoxlo)
			{
				int R=Norma((OB->RealX>>4)-(H->RealX>>4),(OB->RealY>>4)-(H->RealY>>4));
				if(R<A->Radius)
				{
					rez=true;
				}
			}
			if(!rez)
			{
				int pr=(OB->Life*10000)/OB->MaxLife;
				OB->MaxLife-=LifeAdded;//A->Points;
				OB->Life=(OB->MaxLife*pr)/10000;
			}
		}
	}
	return rez;
}
bool ActiveAdditionalLife::CanYouAddToObject(OneObject* B,void* Param)
{
	bool rez=false;
	if(!B->Sdoxlo)
	{
		ActiveIncreaseMaxLife* AA = (ActiveIncreaseMaxLife*) Param;
		IncreaseMaxLife* A = (IncreaseMaxLife*) AA->GetA();
		if((A->Points>0&&AA->OB->NMask&B->NMask)||(A->Points<0&&!(AA->OB->NMask&B->NMask)))
		{
			bool have=false;
			if(B->ActiveAbility)
			{
				int n=B->ActiveAbility->ActiveAbilities.GetAmount();
				for(int i=0;i<n;i++)
				{
					if(B->ActiveAbility->ActiveAbilities[i]->Type==14)
					{
						ActiveAdditionalLife* AD = (ActiveAdditionalLife*)B->ActiveAbility->ActiveAbilities[i];
						if(AD->HeroIndex==AA->OB->Index)
						{
							have=true;
							break;
						}
					}
				}
			}
			if(!have)
			{
				int n = A->UnitType.GetAmount();
				if(n)
				{
					bool tt=false;
					for(int i=0;i<n;i++)
					{
						if(A->UnitType[i]->UnitType==B->NIndex)
						{
							tt=true;
							break;
						}
					}
					if(tt)
						rez=true;
				}
				else
				{
					rez=true;
				}
			}
		}
	}
	return rez;
}
bool ActiveAdditionalLife::FillParam(OneObject* B, void* Param)
{
	ActiveIncreaseMaxLife* AA = (ActiveIncreaseMaxLife*) Param;
	IncreaseMaxLife* A = (IncreaseMaxLife*) AA->GetA();
	if(AA)
	{
		HeroIndex=AA->OB->Index;
		SetA(A);
		LifeAdded=A->Points+A->AddForLevel*(*AA->Le-1);
		int pr=(B->Life*10000)/B->MaxLife;
		B->MaxLife+=LifeAdded;
		B->Life=(B->MaxLife*pr)/10000;
		return true;
	}
	return false;
}
//==================================================================================================================//
ActiveIncreaseMaxLife::ActiveIncreaseMaxLife()
{}
bool ActiveIncreaseMaxLife::Process(OneObject* OB)
{
	if(!OB->Sdoxlo)
	{
		AddActionAbilityOverUnitsInRadius();
		return true;
	}
	if(OB->Sdoxlo>0&&OB->Sdoxlo<1000)
		return true;
	return false;
}
int ActiveIncreaseMaxLife::GetRadius()
{
	IncreaseMaxLife* A = (IncreaseMaxLife*)GetA();
	if(A)
		return A->Radius;
	return 0;
}
ActiveUnitAbility* ActiveIncreaseMaxLife::GetActionAbilityExample()
{
	return &Example;
}
int ActiveIncreaseMaxLife::GetUsePause()
{
	return 20;
}
int ActiveIncreaseMaxLife::GetHintParamOnLevel(int Lev)
{
	if(Lev<6&&Lev>0)
	{
		IncreaseMaxLife* A = (IncreaseMaxLife*)GetA();
		if(A)
		{
			return A->Points+A->AddForLevel*(Lev-1);
		}
	}
	return 0;
}
//==================================================================================================================//
AddUnitBonus::AddUnitBonus()
{
	AddDamage=0;
	AddShield=0;
	EnemyTarget=false;
	CoolDownTime=0;
	EffectTime=0;
	UsePause=0;
	AddForLevel=0;
}
ActiveUnitAbility* AddUnitBonus::GetNewActiveAbility()
{
	return new ActiveAddUnitBonus();
}
//==================================================================================================================//
ActiveAddUnitBonus::ActiveAddUnitBonus()
{}
bool ActiveAddUnitBonus::Process(OneObject* OB)
{
	bool rez=false;
	AddUnitBonus* A = (AddUnitBonus*)GetA();
	if(OB&&(!OB->Sdoxlo)&&A)
	{
		if(!A->CoolDownTime)
		{
			if(AnimTime>(LastUseTime+A->UsePause*256*25/10))
			{
				Exec();
			}
		}
		if(A->AddDamage)
			HintParam=A->AddDamage+(*Le-1)*A->AddForLevel;
		else
		if(A->AddShield)
			HintParam=A->AddShield+(*Le-1)*A->AddForLevel;
			
				
		rez=true;
	}
	if(OB&&OB->Sdoxlo>0&&OB->Sdoxlo<1000&&!rez)
		rez=true;
	return rez;
}
bool ActiveAddUnitBonus::CanApply()
{
	AddUnitBonus* A = (AddUnitBonus*)GetA();
	if(A&&A->CoolDownTime)
	{
		return true;
	}
	return false;
}
int ActiveAddUnitBonus::GetCoolDownProc()
{
	int rez=0;
	AddUnitBonus* A = (AddUnitBonus*)GetA();
	if(A&&A->CoolDownTime)
	{
		int cool=A->CoolDownTime*25*256;
		if(cool)
			rez=(LastUseTime+cool-AnimTime)*100/cool;
		if(rez<0)
			rez=0;
	}
	return rez;
}
int ActiveAddUnitBonus::GetRadius()
{
	AddUnitBonus* A = (AddUnitBonus*)GetA();
	if(A)
	{
		return A->Radius;
	}
	return 0;
}
bool ActiveAddUnitBonus::OnClick()
{
	if(*Vi)
	{
		return LeveledActiveUnitAbility::OnClick();
	}
	AddUnitBonus* A = (AddUnitBonus*)GetA();
	if(A&&A->CoolDownTime)
	{
		if(GetCoolDownProc()==0)
		{
			return Exec();
		}
	}
	return false;
}
int ActiveAddUnitBonus::GetHintParamOnLevel(int Lev)
{
	if(Lev<6&&Lev>0)
	{
		AddUnitBonus* A = (AddUnitBonus*)GetA();
		if(A)
		{
			if(A->AddDamage)
				return A->AddDamage+(Lev-1)*A->AddForLevel;
			else
			if(A->AddShield)
				return A->AddShield+(Lev-1)*A->AddForLevel;
		}
	}
	return 0;
}
bool ActiveAddUnitBonus::Exec()
{
	AddUnitBonus* A = (AddUnitBonus*)GetA();
	if(OB&&A)
	{
		LastUseTime=AnimTime;
		PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,A->Radius,&ActiveAddUnitBonus::SetBonus,(void*)this);
		return true;
	}
	return false;
}
bool ActiveAddUnitBonus::SetBonus(OneObject* OB, void* Param)
{
	if(OB&&!OB->Sdoxlo)
	{
		ActiveAddUnitBonus* AA = (ActiveAddUnitBonus*)Param;
		AddUnitBonus* A = (AddUnitBonus*)AA->GetA();
		if(((AA->OB->NMask&OB->NMask)&&!A->EnemyTarget)||((!(AA->OB->NMask&OB->NMask))&&A->EnemyTarget))
		{
			bool have=false;
			if(OB->ActiveAbility)
			{
				int n= OB->ActiveAbility->ActiveAbilities.GetAmount();
				for(int i=0;i<n;i++)
				{
					if(OB->ActiveAbility->ActiveAbilities[i]->Type==16)
					{
						PassiveAddUnitBonus* ABB = (PassiveAddUnitBonus*)(OB->ActiveAbility->ActiveAbilities[i]);
						if(ABB->HeroIndex==AA->OB->Index&&ABB->GetA()==AA->GetA())
						{
							have=true;
							break;
						}
					}
				}
			}
			if(!have)
			{
				int nt = A->UnitType.GetAmount();
				bool intype=false;
				if(nt)
				{
					for(int i=0;i<nt;i++)
					{
						if(A->UnitType[i]->UnitType==OB->NIndex)
						{
							intype=true;
							break;
						}
					}
				}
				else
				{
					intype=true;
				}
				if(intype)
				{
					PassiveAddUnitBonus* ABB = new PassiveAddUnitBonus();
					ABB->HeroIndex=AA->OB->Index;
					ABB->SetA(A);
					AddActiveUnitAbility(OB->Index,ABB);  
					if(A->AddDamage)
					{
						AA->HintParam=A->AddDamage+(*(AA->Le)-1)*A->AddForLevel;
						ABB->AddDamage=AA->HintParam;
						OB->AddDamage+=ABB->AddDamage;
					}
					else
					if(A->AddShield)
					{
						AA->HintParam=A->AddShield+(*(AA->Le)-1)*A->AddForLevel;
						ABB->AddShield=AA->HintParam;
						OB->AddShield+=ABB->AddShield;
					}
				}
			}
		}
	}
	return false;
}
//==================================================================================================================//
PassiveAddUnitBonus::PassiveAddUnitBonus()
{
	Radius=0;
	AddDamage=0;
	AddShield=0;
	HeroIndex=0xFFFF;
	SetTime=AnimTime;
}
bool PassiveAddUnitBonus::Process(OneObject* OB)
{
	bool rez=false;
	if(OB&&HeroIndex!=0xFFFF)
	{
		AddUnitBonus* A = (AddUnitBonus*)GetA();
		if(A)
		{
			OneObject* Her = Group[HeroIndex];
			if(Her&&!Her->Sdoxlo)
			{
				if(A->EffectTime==0)
				{
					int R = Norma((Her->RealX>>4)-OB->RealX/16,(Her->RealY>>4)-OB->RealY/16);
					if(R<A->Radius)
					{
						rez=true;
					}
				}
				else
				if(AnimTime<(SetTime+A->EffectTime*25*256))
				{
					rez=true;
				}
			}
			if(!rez)
			{
				OB->AddDamage-=AddDamage;
				OB->AddShield-=AddShield;
				if(OB->AddDamage<0)
					OB->AddDamage=0;
				if(OB->AddShield<0)
					OB->AddShield=0;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
ActiveUnitAbility* PushAllOnTheWay::GetNewActiveAbility(){
	PushUnitsAbility* nAA=new PushUnitsAbility;
	nAA->SetA(this);

	Copy(&nAA->Params);

	return nAA;
};
bool PushAllOnTheWay::OnUnitBirth(OneObject* OBJ){
	PushUnitsAbility* PUAB=new PushUnitsAbility;
	Copy(&PUAB->Params);//.Copy(=*this;	
	PUAB->OwnerID=OBJ->Index;
	PUAB->OwnerSN=OBJ->Serial;
	PUAB->SetA(this);
	CopyToActive(PUAB);
	AddActiveUnitAbility(OBJ->Index,PUAB);  
	return true;
}
struct PU_TempStruct{
	int x,y;
	int ux,uy;
	char Dir;
	OneObject* Own;
	PushAllOnTheWay* P;
	PushUnitsAbility* PA;
};
bool ABL_PushUnits(OneObject* OB,void* p){
	if(OB->Sdoxlo || OB->NotSelectable)return true;	
	PU_TempStruct* PT=(PU_TempStruct*)p;
	if(OB->Index==PT->Own->Index)return true;
    if(OB->LockType==1 || OB->LockType==3)return true;	
	void PushUnitBackLink(OneObject* OBJ);
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&PushUnitBackLink)return false;
	char d0=GetDir((OB->RealX>>4)-PT->ux,(OB->RealY>>4)-PT->uy);
	int A=abs(int(char(d0-PT->Dir)));
    int A1=A;
    if(A1>64)A1=128-A1;
	int R=Norma(OB->RealX-(PT->ux<<4),OB->RealY-(PT->uy<<4))>>4;
	int R0=PT->P->PushForwardRadius-A1*(PT->P->PushForwardRadius-PT->P->PushLeftRightRadius)/64;
	if(A<PT->P->PushAngle&&R<R0){		
		if(!OB->newMons->RotationAtPlaceSpeed)OB->RealDir=d0+128;
		void PushUnitBack(OneObject* OB,byte OrdType,int Force);
		if((PT->P->PushEnemyUnits&&!(OB->NMask&PT->Own->NMask))||
			(PT->P->PushFriendlyUnits&&OB->NMask&PT->Own->NMask)){
				if(!OB->newMons->RotationAtPlaceSpeed){
					OB->RealDir=d0+128;
					PushUnitBack(OB,1,PT->P->PushForce);
				}
			}
		if((PT->P->DamageEnemyUnits&&!(OB->NMask&PT->Own->NMask))||
			(PT->P->DamageFriendlyUnits&&OB->NMask&PT->Own->NMask)){
				if(OB->NewAnm==NULL || OB->NewAnm->AnimationType != 1 || !PT->P->Move3DUnitsFromArea){
					int D=PT->P->MinDamage + ( ( ( PT->P->MaxDamage-PT->P->MinDamage )*rando()%100 )/100 );
					OB->MakeDamage( D,D,PT->Own,0 );
				}
			}
	}
}
void MoveUnitFromRadius(OneObject* OB,int xc,int yc,int Radius){
    //Radius+=OB->newMons->CollisionRadius;
    int dx=OB->RealX/16-xc;
    int dy=OB->RealY/16-yc;
    int R=Norma(dx,dy);
    if(R<4)R=4;
    if(R<Radius){
        dx=dx*Radius/R;
        dy=dy*Radius/R;
        void ChangeUnitCoor(OneObject* OB,int newX,int newY);
		addrand(xc+dx);
		addrand(yc+dy);
        ChangeUnitCoor(OB,(xc+dx)<<4,(yc+dy)<<4);
    }
}
bool ABL_AskGoAway(OneObject* OB,void* p){
	if(OB->Sdoxlo || OB->NotSelectable)return true;	
	PU_TempStruct* PT=(PU_TempStruct*)p;
	if(OB->Index==PT->Own->Index)return true;
	void PushUnitBackLink(OneObject* OBJ);
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&PushUnitBackLink)return false;
	char d0=GetDir(OB->RealX-PT->Own->RealX,OB->RealY-PT->Own->RealY);
	char dd=d0-PT->Own->RealDir;
    if(OB->NewAnm->AnimationType == 1 && PT->P->Move3DUnitsFromArea){//3d units
        MoveUnitFromRadius(OB,PT->x,PT->y,PT->P->PushLeftRightRadius);
    }
	if(OB->NMask&PT->Own->NMask&&OB->LockType!=1){
		int R=Norma((OB->RealX>>4)-PT->x,(OB->RealY>>4)-PT->y);        
		if(R<PT->P->PushForwardRadius){
			void RotUnitLink(OneObject* OB);
			void NewMonsterSendToLink(OneObject* OB);			
			if(OB->LocalOrder&&
				(OB->LocalOrder->DoLink==&NewMonsterSendToLink||
				OB->LocalOrder->DoLink==&RotUnitLink))return false;
			int pp=0;
			while(pp<100&&!OB->FrameFinished){
				OB->SetNextFrame();
				pp++;
			}
			int dy=PT->x-(PT->Own->RealX>>4);
			int dx=-(PT->y-(PT->Own->RealY>>4));
			int N=Norma(dx,dy);
			if(N){
				dx=dx*128*16/N;
				dy=dy*128*16/N;
			}            
			if(dd<0){
				OB->NewMonsterSendTo(OB->RealX-dx,OB->RealY-dy,128+16,1);            
			}else{
				OB->NewMonsterSendTo(OB->RealX+dx,OB->RealY+dy,128+16,1);
			}
		}
	}
	else
	{
		if(PT->P->CanScare)
		{
			if(OB->FrameFinished)
			{
				NewAnimation* NA =OB->newMons->GetAnimation(anm_Scare);
				if(NA&&NA->Enabled)
				{
					
					char Dirr=(char)GetDir(PT->Own->RealX-OB->RealX,PT->Own->RealY-OB->RealY);
					char dd = char(OB->RealDir)-Dirr;
					if(OB->BrigadeID==0xFFFF)
					{
						if(abs(dd)>16)
						{
							RotateMon(OB,-dd);
						}
					}

					if(abs(dd)<17)
					{
						OB->NewAnm=NA;
						PT->PA->Boldness++;
						OB->SetZeroFrame();
					}
				}
			}
		}
	}
}
bool enmUnits(OneObject* OB,void* param){
	byte NMask=byte(param);
	return !(OB->NMask&NMask||OB->Sdoxlo);
}
bool TestEnemyNearPoint(int x,int y,byte NMask){
	return PerformActionOverUnitsInRadius(x,y,180,enmUnits,(void*) NMask)!=0;
}
PushUnitsAbility::PushUnitsAbility()
{
	Boldness=0;
	LastTimeCheck=0;
}
void AddUnitToActiveList(OneObject* OB);
bool PushUnitsAbility::Process(){    
	//OneObject* OB=Group[OwnerID];
    if(OB){//&&OB->Serial==OwnerSN){
        AddUnitToActiveList(OB);
		if(OB->DestX>0&&!OB->Sdoxlo){
			static PU_TempStruct PU;
			int x0=(OB->RealX>>4)+(int(TCos[OB->RealDir])*Params.PushCenterShift)/256;
			int y0=(OB->RealY>>4)+(int(TSin[OB->RealDir])*Params.PushCenterShift)/256;
			PU.P=(PushAllOnTheWay*)GetA();
			PU.ux=x0;
			PU.uy=y0;
			PU.x=x0;
			PU.y=y0;
			PU.Dir=OB->RealDir;
			PU.Own=OB;
			//PU.P=Params;
			PU.PA=this;
			PerformActionOverUnitsInRadius(x0,y0,Params.PushForwardRadius+64,&ABL_PushUnits,&PU);
			PU.x=(x0)+((int(TCos[OB->RealDir])*Params.AskGoAwayDistance)>>8);
			PU.y=(y0)+((int(TSin[OB->RealDir])*Params.AskGoAwayDistance)>>8);
			PerformActionOverUnitsInRadius(PU.x,PU.y,Params.PushForwardRadius+64,&ABL_AskGoAway,&PU);
			if(Params.UseAttack1NearEnemy){
				OB->GroundState=TestEnemyNearPoint(x0,y0,OB->NMask);
				if(OB->GroundState && OB->InMotion){
					OB->NoSearchVictim=1;
					//OB->ActivityState=0;
				}else{
					OB->NoSearchVictim=0;
				}
			}else{
				OB->NoSearchVictim=0;
			}

		}
		if(!OB->Sdoxlo)
		{
			if(Params.CanScare)
			{
				if(Boldness>=Params.MaxBoldness)
				{
					PanicUnit(OB);
				}
				if(Boldness>0)
				{
					int re = ((AnimTime-LastTimeCheck)*Params.BoldnessRegeneration)/(25*256*10);
					if(re)
					{
						Boldness-=re;
						LastTimeCheck+=(re*25*256*10)/Params.BoldnessRegeneration;
					}
				}
				else
				{
					LastTimeCheck=AnimTime;
					Boldness=0;
				}
			}
		}
		return true;
	}
	return false;
}
//==================================================================================================================//
bool GetMineProduceBonus(OneObject* Mine,int& Proc)
{
	bool rez=false;
	if(Mine&&Mine->ActiveAbility)
	{
		int n= Mine->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(Mine->ActiveAbility->ActiveAbilities[i]->Type==13)
			{
				ActiveMineBonus* AM = (ActiveMineBonus*)Mine->ActiveAbility->ActiveAbilities[i];
				Proc+=AM->Bonus;
				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
BuildingShield::BuildingShield()
{
	Radius=0;
	AddShield=0;
	EnemyTarget=false;
}
ActiveUnitAbility* BuildingShield::GetNewActiveAbility()
{
	return new ActiveAddBuildingShield();
}
//==================================================================================================================//
ActiveAddBuildingShield::ActiveAddBuildingShield()
{
	LastUseTime=0;
}
bool ActiveAddBuildingShield::Process(OneObject* OB)
{
	if(OB&&!OB->Sdoxlo)
	{
		if((LastUseTime+256*25*3)<AnimTime)
		{
			BuildingShield* Gm = (BuildingShield*)GetA();
			if(Gm)
			{
				int Param[6];
				Param[0]=OB->NMask;
				Param[1]=Gm->EnemyTarget;
				Param[3]=Gm->AddShield+(*Le-1)*Gm->ShieldAddForLevel;
				HintParam=Param[3];
				Param[4]=OB->Index;
				Param[5]=(int)Gm;
				PerformActionOverBuildingsInRadius(OB->RealX>>4,OB->RealY>>4,Gm->Radius,&ActiveAddBuildingShield::AddBuildingShield,(void*)Param);
				LastUseTime=AnimTime;
			}
		}
		return true;
	}
	if(OB&&OB->Sdoxlo>0&&OB->Sdoxlo<1000)
		return true;
	return false;
}
bool ActiveAddBuildingShield::AddBuildingShield(OneObject* Ob, void* param)
{
	if(Ob&&Ob->NewBuilding&&!Ob->Sdoxlo)
	{
		int* P=(int*)param;
		byte mas=(byte)P[0];
		if((mas&Ob->NMask&&!P[1])||(!(mas&Ob->NMask)&&P[1]))
		{
			if(Ob->ActiveAbility)
			{
				int n = Ob->ActiveAbility->ActiveAbilities.GetAmount();
				for(int i=0;i<n;i++)
				{
					if(Ob->ActiveAbility->ActiveAbilities[i]->Type==15)
					{
						ActiveBuildingShield* abs = (ActiveBuildingShield*)(Ob->ActiveAbility->ActiveAbilities[i]);
						if(abs->HeroIndex==P[4])
						{
							return false;
						}
					}
				}
			}
			ActiveBuildingShield* ABS = new ActiveBuildingShield();
			ABS->AddToObject(Ob);
			ABS->HeroIndex=P[4];
			ABS->SetA((UnitAbility*)P[5]);
			ABS->AddShield=P[3];
			Ob->AddShield+=P[3];
			return true;
		}
	}
	return false;
}
int ActiveAddBuildingShield::GetRadius()
{
	BuildingShield* UA = (BuildingShield*)(GetA());
	if(UA)
	{
		return UA->Radius;
	}
	return 0;
}
int ActiveAddBuildingShield::GetHintParamOnLevel(int Lev)
{
	if(Lev<6&&Lev>0)
	{
		BuildingShield* Gm = (BuildingShield*)GetA();
		if(Gm)
		{
			return Gm->AddShield+(Lev-1)*Gm->ShieldAddForLevel;
		}
	}
	return 0;
}
//==================================================================================================================//
ActiveBuildingShield::ActiveBuildingShield()
{
	HeroIndex=-1;
	LastUseTime=0;
	Type=15;
}
bool ActiveBuildingShield::Process(OneObject* OB)
{
	if(OB&&!OB->Sdoxlo&&HeroIndex!=-1)
	{
		if((LastUseTime+25*256*3)>AnimTime) 
			return true;
		OneObject* Her = Group[HeroIndex];
		BuildingShield* UA = (BuildingShield*)(GetA());
		if(Her&&UA&&!Her->Sdoxlo)
		{
			int d = Norma((OB->RealX>>4)-(Her->RealX>>4),(OB->RealY>>4)-(Her->RealY>>4));
			if(d<UA->Radius)
			{
				LastUseTime=AnimTime;
				return true;
			}
			OB->AddShield-=AddShield;
		}
	}
	return false;
}
//==================================================================================================================//
FollowBrigade::FollowBrigade()
{
	MoveOutEnemy=false;
}
ActiveUnitAbility* FollowBrigade::GetNewActiveAbility()
{
	return new ActiveFollowBrigade();
}
//==================================================================================================================//
ActiveFollowBrigade::ActiveFollowBrigade()
{
	lastUseTime=0;
	lastMoveBack=0;
}
bool ActiveFollowBrigade::Process(OneObject* OB)
{
	if(!OB->Sdoxlo)
	{
		if((lastUseTime+3*25*256)<AnimTime&&!GSets.CGame.isHumanPlayer(OB->NNUM))
		{
			int n = WiselyFormations.AllForm.GetAmount();
			int x=OB->RealX/16;
			int y=OB->RealY/16;
			int mind=999999;
			Atom* MA=NULL;
			for(int i=0;i<n;i++)
			{
				Atom* A=WiselyFormations.AllForm[i];
				if(A&&A->BSelf&&A->NI==OB->NNUM&&A->IsAlive())
				{
					int dist=Norma(A->x-x,A->y-y);
					dist+=A->BSelf->Morale/10000*20;
					if(mind>dist)
					{
						mind=dist;
						MA=A;
					}
				}
			}
			if(MA&&mind>300)
			{
				int xx=MA->x+((150*TCos[MA->BSelf->Direction+128])>>8)-50+rando()%100;
				int yy=MA->y+((150*TSin[MA->BSelf->Direction+128])>>8)-50+rando()%100;
				OB->NewMonsterSmartSendTo(xx,yy,0,0,128+16,0);
			}
			lastUseTime=AnimTime;
		}
		if((lastMoveBack+2*25*256)<AnimTime&&!GSets.CGame.isHumanPlayer(OB->NNUM))
		{
			FollowBrigade *A=(FollowBrigade*)GetA();
			if(A&&A->MoveOutEnemy)
			{
				int par[6];
				par[0]=0;
				par[1]=0;
				par[2]=0;
				par[3]=(int)OB->NMask;
				par[4]=OB->RealX/16;
				par[5]=OB->RealY/16;
				PerformActionOverUnitsInRadius(OB->RealX/16,OB->RealY/16,400,&ActiveFollowBrigade::CheckEnemy,(void*) par);
				if(par[2])
				{
					par[0]/=par[2];
					par[1]/=par[2];
					int ds=Norma(par[0],par[1]);
					if(ds)
					{
						par[0]=par[0]*150/ds;
						par[1]=par[1]*150/ds;
						OB->NewMonsterSmartSendTo(OB->RealX/16+par[0],OB->RealY/16+par[1],0,0,128+16,0);
					}
				}
			}
			lastMoveBack=AnimTime;
		}
		return true;
	}
	return false;
}
bool ActiveFollowBrigade::CheckEnemy(OneObject* OB,void* param)
{
	bool rez=false;
	if(OB&&!OB->Sdoxlo)
	{
		int *par=(int*)param;
		byte mm=(byte)par[3];
		if(!(OB->NMask&mm))
		{
			int ds=Norma(OB->RealX/16-par[4],OB->RealY/16-par[5]);
			if(ds<401)
			{
				par[0]+=((par[4]-OB->RealX/16)*100)/(401-ds);
				par[1]+=((par[5]-OB->RealY/16)*100)/(401-ds);
				par[2]++;
				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
Behaviour::Behaviour()
{
	MoveDist=0;
	MoveTime=0;
	ChangeDir=0;
	MaxRestTime=0;
	Radius=0;
}
ActiveUnitAbility* Behaviour::GetNewActiveAbility()
{
	return new ActiveBehaviour();
}
//==================================================================================================================//
ActiveBehaviour::ActiveBehaviour()
{
	EndMoveTime=0;
	RestTime=0;
	BaseX=-1;
	BaseY=-1;
}
bool ActiveBehaviour::Process(OneObject* OB)
{
	if(!OB->Sdoxlo)
	{
		Behaviour* B = (Behaviour*)GetA();
		if(B)
		{
			if(!GetPeaceMode())
			{
				if(B->Radius>0&&BaseX==-1)
				{
					BaseX=OB->RealX/16;
					BaseY=OB->RealY/16;
				}
				if(EndMoveTime==0&&RestTime==0&&(B->MaxRestTime||B->MoveTime))
				{
					if(B->MaxRestTime)RestTime=AnimTime+25*256*(rando()%B->MaxRestTime);
					if(B->MoveTime)EndMoveTime=AnimTime+25*256*(rando()%B->MoveTime);
					if(RestTime<EndMoveTime)
						RestTime=0;
					else
						EndMoveTime=0;
				}
				if(EndMoveTime)
				{
					if(OB->DestX<0)
					{
						if(EndMoveTime>AnimTime||B->MaxRestTime==0)
						{
							Move(OB,B);
						}
						else
						{
							EndMoveTime=0;
							RestTime=AnimTime+25*256*(rando()%B->MaxRestTime);
						}
					}
				}
				if(RestTime&&B->MaxRestTime)
				{
					if(RestTime<AnimTime)
					{
						RestTime=0;
						EndMoveTime=AnimTime+25*256*(rando()%B->MoveTime);
						Move(OB,B);
					}
				}
			}
			return true;
		}
	}
	return false;
}
void ActiveBehaviour::Move(OneObject* OB, Behaviour* B)
{
	byte RDir=rando()%B->ChangeDir;
	int RDist=rando()%B->MoveDist;
	int cx=OB->RealX/16;
	int cy=OB->RealY/16;
	byte dr=OB->RealDir-B->ChangeDir/2+RDir;
	int dx=cx+RDist*TSin[dr]/256;
	int dy=cy+RDist*TCos[dr]/256;
	if(B->Radius>0)
	{
		if(Norma(BaseX-dx,BaseY-dy)>B->Radius)
			return;
	}
	int P[8];
	P[0]=cx;
	P[1]=cy;
	P[2]=dx;
	P[3]=dy;
	MotionField* MF=&MFIELDS[OB->LockType];

	if(!MF->CheckPt(dx>>4,dy>>4)&&GetSpritesInRadius((cx+dx)/2,(cy+dy)/2, B->MoveDist,ActiveBehaviour::CheckSprite, (void*)P)==0)
	{
		if(OB->LocalOrder==NULL)
			OB->NewMonsterSendTo(dx<<4,dy<<4,128+16,0);
		//OB->CreatePath(dx/16,dy/16);
	}
	else
	{
		RotateMon(OB,RDir-B->ChangeDir/2);
	}
}
bool ActiveBehaviour::CheckSprite(OneSprite* OS,void* Param)
{
	if(OS&&OS->OC->Aligning==2)//Vertical
	{
		int* P = (int*)Param;
		int dste=GetPointToLineDist(OS->x,OS->y,P[0],P[1],P[2],P[3]);
		if(dste<30)
		{
			return true;
		}
	}
	//int GetPointToLineDist(int x,int y,int x1,int y1,int x2,int y2);
	return false;
}
//==================================================================================================================//
bool aa_SlowCallback(OneObject* OB,void* param){
	DWORD M=(DWORD)param;
	return!(OB->NMask&M);
}
bool aa_BeSlowNearUnits::Process(OneObject* OB){
	int x0=OB->RealX/16+ABL.ShiftFromCenter*TCos[OB->RealDir]/256;
	int y0=OB->RealY/16+ABL.ShiftFromCenter*TSin[OB->RealDir]/256;
	DWORD M=ABL.BeSlowOnlyInEnemyGroup?OB->NMask:0;
    int N=PerformActionOverUnitsInRadius(x0,y0,ABL.Radius,aa_SlowCallback,(void*)M);
	if(N>ABL.MaxUnitsAmount){
		N=ABL.MaxUnitsAmount;
	}
	if(ABL.MaxUnitsAmount){
        float p=ABL.SlowDegree[N*(NSlowDegrees-1)/ABL.MaxUnitsAmount];
        OB->GroupSpeed=int(p*OB->newMons->MotionDist);
		OB->UnitSpeed=64;
	}
	return true;
}
ActiveUnitAbility* BeSlowNearUnits::GetNewActiveAbility(){
	return new aa_BeSlowNearUnits;
}
void BeSlowNearUnits::CopyToActive(ActiveUnitAbility* ab){
	aa_BeSlowNearUnits* AB=(aa_BeSlowNearUnits*)ab;
	Copy(&AB->ABL);
}
//=============================================================================================================================
ActiveUnitAbility* MakeDamageOnDeath::GetNewActiveAbility(){
	return new aa_MakeDamageOnDeath;
}
void MakeDamageOnDeath::CopyToActive(ActiveUnitAbility* ab){
	aa_MakeDamageOnDeath* AB=(aa_MakeDamageOnDeath*)ab;
	Copy(&AB->MD);
}
struct aa_md_param{
	DWORD Mask;
	int DamValue;
	OneObject* Source;
};
bool aa_MakeDamCallback(OneObject* OB,void* param){
	aa_md_param* M=(aa_md_param*)param;
	if(!(OB->NMask&M->Mask)){
		void PushUnitBack(OneObject* OB,byte OrdType,int Force);
		PushUnitBack(OB,0,100);
		OB->MakeDamage(M->DamValue,M->DamValue,M->Source,0);
	}
	return true;
}
bool aa_MakeDamageOnDeath::Process(OneObject* OB){
	if(OB->Sdoxlo&&!Done){
		aa_md_param M;
		M.DamValue=MD.DamageValue;
		M.Source=OB;
		M.Mask=(MD.DamageFriens?0:OB->NMask)|(MD.DamageEnemy?0:~OB->NMask);
		PerformActionOverUnitsInRadius(OB->RealX/16+(MD.xc*TCos[OB->RealDir]/256)-(MD.yc*TSin[OB->RealDir]/256)
			,OB->RealY/16+(MD.yc*TCos[OB->RealDir]/256)+(MD.xc*TSin[OB->RealDir]/256),MD.R,aa_MakeDamCallback,&M);
		Done=true;
	}
	return true;
}
void ApplyLandTypeSlow(char* From,char* To,int xc,int yc,int R1,int R2
					   ,int RiseTime,int GrowTime,int RemainTime,int EliminateTime,bool Forever
					   ,char* BirthWeapon,int BirthProbability
					   ,char* UndoWeapon ,int UndoProbability);
ActiveUnitAbility* PermanentLandTypeChange::GetNewActiveAbility(){
	ActivePermanentLandTypeChange* AA=new ActivePermanentLandTypeChange;
	AA->SetA(this);
	AA->CurRadius=0;
	AA->TimeOfPrevSpot=0;
	return AA;
}
void PermanentLandTypeChange::CreateNewSpot(int x,int y,int R){
	ApplyLandTypeSlow("",DestLandName.pchar(),x,y,R,R+100,PeriodOfSpotBirth,PeriodOfSpotBirth,100,0,true,WeaponOfBirth.pchar(),WeaponBirthProbability,NULL,0);
}
bool ActivePermanentLandTypeChange::Process(OneObject* OB){
	if( !OB->Sdoxlo && (!OB->NewBuilding||OB->Ready) ){
		PermanentLandTypeChange* PAR=(PermanentLandTypeChange*)GetA();
		if(CurRadius==0){
			CurRadius=PAR->FirstSpotRadius+100;
			TimeOfPrevSpot=TrueTime;
			PAR->CreateNewSpot(OB->RealX/16,OB->RealY/16,PAR->FirstSpotRadius+100);
		}
		if(TrueTime-TimeOfPrevSpot>PAR->PeriodOfSpotBirth*4){
			TimeOfPrevSpot=TrueTime;
			int xb=0;
			int yb=0;
			float bestw=2550;
			int minR=100000;
			GreyMap* GPM=NULL;
			for(int i=0;i<MLayers->MapsNames.GetAmount();i++){
				if(!strcmp(PAR->DestLandName.pchar(),MLayers->MapsNames[i]->pchar())){
					GPM=(GreyMap*)MLayers->Maps[i];
					break;
				}
			}
			if(GPM){
				for(int i=0;i<50;i++){
					byte A=rando()&255;
					int R=(CurRadius*(rando()&255))>>8;
					int x=OB->RealX/16+int(TCos[A])*CurRadius/256;
					int y=OB->RealY/16+int(TSin[A])*CurRadius/256;
					float w=GPM->GetGreyPrecise(float(x)/15360,float(y)/15360);
					if( w<128 && R<minR ){
						bestw=w;
						xb=x;
						yb=y;
						if(w<128)minR=R;
					}
				}
				if(bestw<128)PAR->CreateNewSpot(xb,yb,PAR->SpotRadius+100);
				CurRadius=tmin(CurRadius+30,minR+800);
			}        
		}
	}
	return true;
}
//=============================================================================================================================
//
HeroVariableStorage* DetectHero(OneObject* OB);
//
class UnitAbilityEnumerator:public ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){
		for(int i=0;i<Abilities.Abilities.GetAmount();i++){
			if(Abilities.Abilities[i]->Name.equal(ID))return i;
		}
		return -1;
	}	
	virtual char* GetValue(DWORD ID){
		if(ID==0xFFFFFFFF)return (char*)"---none---";		
		else return (char*)Abilities.Abilities[ID]->Name();		
	}	
	virtual int   GetAmount(){
		return Abilities.Abilities.GetAmount()+1;
	}
	virtual char* GetIndexedString(int idx){
		if(idx>0)return (char*)Abilities.Abilities[idx-1]->Name();
		else return (char*)"---none---";
    }
	virtual DWORD GetIndexedValue (int idx){
		if(idx>0)return idx-1;
		return -1;
	}
	virtual char* GetCategory(int idx){
		if(idx>0){
			UnitAbility* UA=Abilities.Abilities[idx-1];
			if(UA){
				return (char*)UA->GetClassName();
			}
		}
		return "---none---";
	}	
};
DWORD rePilgrimAbility(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int n=Abilities.Abilities.GetAmount();
	if(n!=PrevCallHash){
		for(int i=0;i<n;i++){
			char* Name=Abilities.Abilities[i]->Name.pchar();
			if(strlen(Name)>3){
				if( Name[0]=='P' && Name[1]=='G' && Name[2]=='_' ){\
					out->add(Name+3,i,"Main");
				}
			}
		}
	}
	return n;
}
void CreateUnitAbilityEnumeratorEnumerator(){

	UnitAbilityEnumerator* CE=new UnitAbilityEnumerator;
	Enumerator* E=ENUM.Get("ABILKI");
	E->SetProcEnum(CE);

	E=ENUM.Get("PILGRIM_ABILITY");
	E->SetProcEnum(new RefreshableEnumerator(rePilgrimAbility));

}
word GlobalUnitsContext::CasterID=0xFFFF;
word GlobalUnitsContext::CasterSN=0xFFFF;
word GlobalUnitsContext::AcceptorID=0xFFFF;
word GlobalUnitsContext::AcceptorSN=0xFFFF;
//
ChangeMDAbiliti::ChangeMDAbiliti()
{
}
ActiveUnitAbility* ChangeMDAbiliti::GetNewActiveAbility(){
	ChangeMDAbilitiActive* nAA=new ChangeMDAbilitiActive;
	nAA->SetA(this);
	return nAA;
}
ChangeMDAbilitiActive::ChangeMDAbilitiActive() {
	Type=6;
	TypeAbil=2;
	CurMD=-1;
}
bool ChangeMDAbilitiActive::Process() {
	ChangeMDAbiliti* NMD = (ChangeMDAbiliti*)GetA();
	if(NMD){

		if(CurMD==-1){
			OnClick();
		}

		//Casting=false;
		//MDState* S=NMD->MD[CurMD];
		//if(S){
		//	if( OB->NIndex!=S->Type.UnitType ){
		//		Casting=true;
		//	}
		//}

	}
	return true;
}
bool ChangeMDAbilitiActive::OnClick() {
	// Edit by Xeno to avoid a transformation over water
	int x = OB->RealX / 16;
	int y = OB->RealY / 16;
	int H = GetTotalHeight(x, y);
	if(GetCoolDownProc()==0 && H>=3) { //&& MFIELDS[OB->LockType].CheckPt(x, y)
		ChangeMDAbiliti* NMD = (ChangeMDAbiliti*)GetA();
		if(NMD){
			int NStates=NMD->MD.GetAmount();
			if(NStates){
				CurMD=(CurMD+1)%NStates;
				MDState* S=NMD->MD[CurMD];
				if(S){
					EffectTime=TrueTime;

					FileID=S->FileID;
					SpriteID=S->SpriteID;
					Hint=S->Hint;

					word NIndex=S->Type.UnitType;
					OB->ActiveAbility->BornNIndex=NIndex;
					OB->ActiveAbility->BornState=S->ThroughState;

					GeneralObject* GO=NATIONS[OB->NNUM].Mon[NIndex];
					OB->ActiveAbility->BornNM=GO->newMons;
					OB->ActiveAbility->BornAC=GO->MoreCharacter;

					if(ASerial.GetAmount()){
						itr_AA.Create(OB);
						while(ActiveUnitAbility* AA=itr_AA.Next()){
							//if(AA->TypeAbil!=0)
							int sid=ASerial.find(AA->Serial);
							if(sid!=-1){
								ASerial.Del(sid,1);
								AA->Deleted=true;

								addrand(sid);

							}
						}
					}

					int n=S->Abilities.GetAmount();
					for(int i=0;i<n;i++){
						ClassRef<UnitAbility>* refUA=S->Abilities[i];
						if(refUA){
							ActiveUnitAbility* AA=AddAbility(refUA->Get(),OB,2);
							if(AA){
								ASerial.Add(AA->Serial);
							}
						}
					}
				}

			}
		}
		return true;
	}
	return false;
}
int ChangeMDAbilitiActive::GetCoolDownProc(){
	UnitAbility* UA=(NewAbility*)GetA();
	if(UA){
		if(CurMD>=0&&UA->CoolDownDuration){
			int dT=0;
			dT=TrueTime-EffectTime;
			if(dT<UA->CoolDownDuration){
				int c=100-100*dT/UA->CoolDownDuration;
				if(c<0)c=0;
				return c;
			}		
		}
	}
	return 0;
}

void UnitAbility::Init(){
	Index=-1;
	Name="";
	HintName=NULL;
	Visible=true;

	SellMultiplier=0.4;

	BodyStyleTarget=0xFF;
	eAnDuration=10000000; //MAX_INT;

	CursType=0;
	CursColor=0x80FFFFFF;

	CastingShotAmount=1;

	EndEffectIfEnemyNearRadius=80;

	UnlimitedAmount=true;
	AuraCastPeriod=1000; // 1000; by Xeno

	g_beOBJ_CONTROLER.SetState("UnitAbility");

	BuildingTarget=true;
	OverWater=false;

	SpellDangerTime=3000;

	Amount=0;

	BreakInvisibility=true;
}

DWORD UnitAbility::GetClassMask(){
	DWORD M=0xFFFFFFFF;
	if(ClassType>0){
		M=1<<(ClassType-1);
		WeaponModificator* eW=eWeapon.Get();
		if( M==1 && !eW ){
			M|=4;
		}
	}
	//if( AntiMageSupportSkill )
	return M;
}

void ActiveAbilityListArray::modifyInvisibility		(bool Basic,bool& Current,byte& Detected,bool& EnableSearchVictim){
	//if( TrueTime>InvisibleEnableTime ){
		ListWalk(Invisibility)(Basic,Current,Detected,EnableSearchVictim);
	//}
}


OneObject* GlobalUnitsContext::getCaster(){
	if(CasterID!=0xFFFF){
        OneObject* OB=Group[CasterID];
		if(OB&&OB->Serial==CasterSN)return OB;
	}
	return NULL;
}
OneObject* GlobalUnitsContext::getAcceptor(){
	if(AcceptorID!=0xFFFF){
		OneObject* OB=Group[AcceptorID];
		if(OB&&OB->Serial==AcceptorSN)return OB;
	}
	return NULL;
}

int ActiveUnitAbility::GetUnitIndex(){
	if(OB){
		return OB->Index;
	}
	return 0xFFFF;
};

extern bool InAllEditors;

bool vOperand::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	//if( sign==0 || sign==2 ){
	//	dest=ColorPositive;
	//}else
	//if( sign==1 || sign==3 ){
	//	dest=ColorNegative;
	//}else{
	//	dest=ColorNeutral;
	//}
	DString op="%$Operator$% ";
	if(sign==3){
		static char* opr=GetTextByID("#div");
		op=opr;
		op+=" ";
	}else
	if(sign==4){
		op="";
	}
	//if(numType==0){
	//	dest+=op;
	//	dest+="%$Operand$%";
	//}else if(numType==1){
		if( v_ISys.DebugHint || InAllEditors ){
			dest+=op;
			dest+="%$Operand$%% %$Percent$%";
		}else{
			dest+=op;
			//dest+="%$Operand$%%";
			int a=num.Get();
			dest+=a;
			if(numType==1){
				dest+="%";//TBH 29.07.2023 Hint Fix Basa/Actual
				switch (percentType){
				case 0:
					dest+=" ";
					dest+=GetTextByID("#FromCurrent");
					break;
				case 1:
					dest+=" ";
					dest+=GetTextByID("#FromBase");
					break;
				}
			}

		}
	//}
	if( v_ISys.DebugHint || InAllEditors ){
		if( sign==1 || sign==3 || sign==4&&numType==1 ){
			dest+=", MinValue=%$MinValue$%";
		}
	}
	return true;
}

enActionType vOperand::GetActionType(bool Reverse){
	if( sign==0 || sign==2 ){
		return Reverse?atDecrease:atRise;
	}else
	if( sign==1 || sign==3 ){
		return Reverse?atRise:atDecrease;
	}
	return atNeutral;
}

ActiveUnitAbility* UA_Lazy::GetNewActiveAbility(){
	AA_Lazy* nAA=new AA_Lazy;
	nAA->SetA(this);
	return nAA;
};

void RestLink(OneObject* OB);
bool Rest(OneObject* OB, byte OrdType);

bool AA_Lazy::Process(){
	UA_Lazy* UA=(UA_Lazy*)GetA();
	if( UA && CheckTime<TrueTime ){
		CheckTime=TrueTime+UA->Delay;
		if( !OB->LocalOrder || OB->LocalOrder->DoLink!=&RestLink ){
			bool find=false;
			int x=OB->RealX>>4;
			int y=OB->RealY>>4;
			itr_UnitsInRadius.Create(x,y,UA->Radius);
			if( UA->Supervisor.UnitType==0 ){
				while( OneObject* oB=itr_UnitsInRadius.Next() ){
					if( oB->GetLiveState()==ls_LiveBody && (oB->NMask&OB->NMask) && oB->NIndex!=OB->NIndex ){
						find=true;
					}
				}
			}else{
				while( OneObject* oB=itr_UnitsInRadius.Next() ){
					if( oB->GetLiveState()==ls_LiveBody && (oB->NMask&OB->NMask) && oB->NIndex==UA->Supervisor.UnitType ){
						find=true;
					}
				}
			}
			if(!find){
				/*
				NewAnimation* NA=OB->newMons->GetAnimation(anm_Rest);
				if(OB->NewAnm!=NA){
				//void DisablePathRequest(OneObject* OB);
				//DisablePathRequest(OB);
				//OB->DestX=-1;
				OB->InMotion=false;
				//OR->OrderType=12;
				OB->NewAnm=NA;
				OB->SetZeroFrame();
				}
				*/
				Rest(OB,1);
			}
		}
	}
	return true;
}

void ActiveUnitAbility::LevelUp(){
	Level++;
	UnitAbility* UA=GetA();
	if( UA ){
		// AntiMageSkills - ”становить уровень кнопочке
		if( OB->newMons->NewParams.AntiMage && UA->Type==7 && TypeAbil==2 ){
			units_iterator::ActiveAbilities itr;
			itr.Create(OB);
			while(ActiveUnitAbility* AA=itr.Next()){
				if( UA==AA->GetA() && AA->TypeAbil==1 )
					AA->Level=Level;
			}
		}
	}
	//return true;
}