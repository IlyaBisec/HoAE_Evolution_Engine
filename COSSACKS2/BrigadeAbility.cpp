#include "stdheader.h"
#include "unitability.h"
#include "BrigadeAbility.h"
//==================================================================================================================//
extern int TrueTime;
extern int AnimTime;
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
bool AddBrigadeAbility(Brigade* Br,ActiveBrigadeAbility* ABA);
void AddGroundCircle(int x,int y,int R,DWORD Color);
char* GetTextByID(char* id);
extern bool vGameLoaing;
//==================================================================================================================//
ActiveBrigadeAbility::ActiveBrigadeAbility()
{
	Br=NULL;
	NI=-1;
	BrigadeIndex=-1;
}
bool ActiveBrigadeAbility::Process(Brigade* B)
{
	return false;
}
BrigadeIconInfo* ActiveBrigadeAbility::GetAbilityIcon()
{
	return NULL;
}
int ActiveBrigadeAbility::GetAddDamage()
{
	return 0;
}
int ActiveBrigadeAbility::GetAddShield()
{
	return 0;
}
//==================================================================================================================//
bool BrigadeActiveAbilityList::Process(Brigade* B)
{
	bool rez=false;
	int n = GetAmount();
	for(int i=0;i<n;i++)
	{
		ActiveBrigadeAbility* AB=(*this)[i];
		if(!AB->Process(B))
		{
			delete AB;
			Del(i,1);
			i--;
			n--;
		}
		else
		{
			rez=true;
		}
	}
	return rez;
}
//==================================================================================================================//
BrigadeChangeMoral::BrigadeChangeMoral()
{
	AddMoral=0;
	Radius=0;
}
bool BrigadeChangeMoral::OnUnitBirth(OneObject* Newbie)
{
	bool rez=false;
	if(Newbie)
	{
		ActiveBrigadeChangeMoral* ab = new ActiveBrigadeChangeMoral();
		//ab->UnitIndex=Newbie->Index;
		ab->OB=Newbie;
		ab->Visible=Visible;
		ab->FileID=FileID;
		ab->SpriteID=SpriteID;
		ab->SetA(this);
		ab->Hint.Add(GetTextByID(Hint));
		return AddActiveUnitAbility(Newbie->Index, ab);
	}
	return rez;
}
//==================================================================================================================//
ActiveBrigadeChangeMoral::ActiveBrigadeChangeMoral()
{
	LastCheckTime=0;
}
bool ActiveBrigadeChangeMoral::Process()
{
	bool rez=false;
	if((!OB)&&GetUnitIndex()!=0xFFFF)
	{
		OB=Group[GetUnitIndex()];
	}
	if((!OB)||(OB&&!OB->Sdoxlo))
	{
		BrigadeChangeMoral* CM = (BrigadeChangeMoral*)GetA();
		if(CM)
		{
			rez=true;
			if((LastCheckTime+25*256*2)<AnimTime)
			{
				PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,CM->Radius,&ActiveBrigadeChangeMoral::SetActiveAbil,(void*)this);
				LastCheckTime=AnimTime;
				HintParam=(CM->AddMoral+((*Le)-1)*CM->AddMoralForLevel)/10000;
			}
		}
	}
	if(OB&&OB->Sdoxlo>0&&OB->Sdoxlo<1000)
		rez=true;
	return rez;
}
bool ActiveBrigadeChangeMoral::ShowRadius()
{
	if(OB)
	{
		BrigadeChangeMoral* BM = (BrigadeChangeMoral*)GetA();
		AddGroundCircle(OB->RealX>>4,OB->RealY>>4,BM->Radius,0xFFFFFF00);
	}
	return true;
}
int ActiveBrigadeChangeMoral::GetHintParamOnLevel(int Lev)
{
	if(Lev>0&&Lev<6)
	{
		BrigadeChangeMoral* CM = (BrigadeChangeMoral*)GetA();
		if(CM)
		{
			return (CM->AddMoral+(Lev-1)*CM->AddMoralForLevel)/10000;
		}
	}
	return 0;
}
bool ActiveBrigadeChangeMoral::SetActiveAbil(OneObject* OB, void* Param)
{
	bool rez=false;
	if(OB&&OB->BrigadeID!=0xFFFF&&!OB->Sdoxlo)
	{
		ActiveBrigadeChangeMoral* CM = (ActiveBrigadeChangeMoral*)Param;
		BrigadeChangeMoral* BM = (BrigadeChangeMoral*)CM->GetA();
		if((BM->AddMoral>0&&OB->NNUM==CM->OB->NNUM)||(BM->AddMoral<0&&!(OB->NMask&CM->OB->NMask)))
		{
			Brigade* Br = &CITY[OB->NNUM].Brigs[OB->BrigadeID];
			bool have=false;
			if(Br&&Br->Ability)
			{
				int n =Br->Ability->GetAmount();
				for(int i=0;i<n;i++)
				{
					if((*Br->Ability)[i]->Type==10)
					{
						ActiveChangedMoralForBrigade* A = (ActiveChangedMoralForBrigade*)((*Br->Ability)[i]);
						if(A->HeroIndex==CM->OB->Index)
						{
							have=true;
							break;
						}
					}
				}
			}
			if(!have)
			{
				ActiveChangedMoralForBrigade* A = new ActiveChangedMoralForBrigade();
				A->Br=Br;
				A->NI=OB->NNUM;
				A->BrigadeIndex=OB->BrigadeID;
				A->MoralAdd=BM->AddMoral+(*(CM->Le)-1)*BM->AddMoralForLevel;
				A->Radius=BM->Radius;
				A->HeroIndex=CM->OB->Index;
				A->ChangeMoral.Set(BM);

				Br->Morale+=BM->AddMoral+(*(CM->Le)-1)*BM->AddMoralForLevel;
				AddBrigadeAbility(Br,A);
				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
ActiveChangedMoralForBrigade::ActiveChangedMoralForBrigade()
{
	Type=10;
	HeroIndex=0xFFFF;
}
bool ActiveChangedMoralForBrigade::Process(Brigade* B)
{
	bool rez=false;
	if(B&&HeroIndex!=0xFFFF)
	{
		int x,y;
		byte Dir;
		OneObject* OB = Group[HeroIndex];
		if(OB&&!OB->Sdoxlo)
		{
			B->GetCenter(&x,&y,&Dir);
			int R = Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
			if(R<(Radius+200))
			{
				rez=true;
			}
		}
	}
	if(!rez)
	{
		B->Morale-=MoralAdd;
		//if(B->Morale>B->MaxMorale)
			//B->Morale=B->MaxMorale;
		if(B->Morale<0)
			B->Morale=0;
	}
	return rez;
}
BrigadeIconInfo* ActiveChangedMoralForBrigade::GetAbilityIcon()
{	
	if(ChangeMoral.Get())
	{
		return &ChangeMoral.Get()->IconInfo;
	}
	return NULL;
}
//==================================================================================================================//
BrigadeTimeChangeMoral::BrigadeTimeChangeMoral()
{
	AddMoral=0;
	Radius=0;
	CoolDownTime=0;
}
bool BrigadeTimeChangeMoral::OnUnitBirth(OneObject* Newbie)
{
	bool rez=false;
	if(Newbie)
	{
		ActiveBrigadeTimeChangeMoral* ab = new ActiveBrigadeTimeChangeMoral();
		//ab->UnitIndex=Newbie->Index;
		ab->OB=Newbie;
		ab->Visible=Visible;
		ab->FileID=FileID;
		ab->SpriteID=SpriteID;
		ab->SetA(this);
		ab->Hint.Add(GetTextByID(Hint));
		return AddActiveUnitAbility(Newbie->Index, ab);
	}
	return rez;
}
//==================================================================================================================//
ActiveBrigadeTimeChangeMoral::ActiveBrigadeTimeChangeMoral()
{
	LastUseTime=0;
}
bool ActiveBrigadeTimeChangeMoral::OnClick()
{
	bool rez=false;
	if(*Vi)
	{
		return LeveledActiveUnitAbility::OnClick();
	}
	if(OB&&GetCoolDownProc()==0)
	{
		BrigadeTimeChangeMoral* CM = (BrigadeTimeChangeMoral*)GetA();
		if(CM)
		{
			rez=true;
			PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,CM->Radius,&ActiveBrigadeTimeChangeMoral::SetActiveAbil,(void*)this);
			LastUseTime=AnimTime;
		}
	}
	
	BrigadeTimeChangeMoral* BM = (BrigadeTimeChangeMoral*)GetA();
	HintParam=BM->AddMoral+((*Le)-1)*BM->MoralAddForLevel;

	return rez;
}
bool ActiveBrigadeTimeChangeMoral::CanApply()
{
	return true;
}
bool ActiveBrigadeTimeChangeMoral::Process()
{
	bool rez=false;
	if((!OB)&&GetUnitIndex()!=0xFFFF)
	{
		OB=Group[GetUnitIndex()];
	}
	if(OB&&!OB->Sdoxlo)
	{
		rez=true;
		BrigadeTimeChangeMoral* BM = (BrigadeTimeChangeMoral*)GetA();
		if(BM)
			HintParam=(BM->AddMoral+((*Le)-1)*BM->MoralAddForLevel)/10000;
	}
	if(OB&&OB->Sdoxlo>0&&OB->Sdoxlo<1000)
		rez=true;
	return rez;
}
bool ActiveBrigadeTimeChangeMoral::ShowRadius()
{
	if(OB)
	{
		BrigadeTimeChangeMoral* BM = (BrigadeTimeChangeMoral*)GetA();
		if(BM)
			AddGroundCircle(OB->RealX>>4,OB->RealY>>4,BM->Radius,0xFFFFFF00);
	}
	return true;
}
int ActiveBrigadeTimeChangeMoral::GetCoolDownProc()
{
	int rez=0;
	BrigadeTimeChangeMoral* A = (BrigadeTimeChangeMoral*)GetA();
	if(A)
	{
		int cool=A->CoolDownTime*25*256;
		if(cool)
			rez=(LastUseTime+cool-AnimTime)*100/cool;
		if(rez<0)
			rez=0;
	}
	return rez;
}
int ActiveBrigadeTimeChangeMoral::GetHintParamOnLevel(int Lev)
{
	if(Lev>0&&Lev<6)
	{
		BrigadeTimeChangeMoral* A = (BrigadeTimeChangeMoral*)GetA();
		if(A)
		{
			return (A->AddMoral+(Lev-1)*A->MoralAddForLevel)/10000;
		}
	}
	return 0;
}
bool ActiveBrigadeTimeChangeMoral::SetActiveAbil(OneObject* OB, void* Param)
{
	bool rez=false;
	if(OB&&OB->BrigadeID!=0xFFFF&&!OB->Sdoxlo)
	{
		ActiveBrigadeTimeChangeMoral* CM = (ActiveBrigadeTimeChangeMoral*)Param;
		BrigadeTimeChangeMoral* BM = (BrigadeTimeChangeMoral*)CM->GetA();
		if((BM->AddMoral>0&&OB->NNUM==CM->OB->NNUM)||(BM->AddMoral<0&&!(OB->NMask&CM->OB->NMask)))
		{
			Brigade* Br = &CITY[OB->NNUM].Brigs[OB->BrigadeID];
			bool have=false;
			if(Br&&Br->Ability)
			{
				int n =Br->Ability->GetAmount();
				for(int i=0;i<n;i++)
				{
					if((*Br->Ability)[i]->Type==11)
					{
						/*
						ActiveTimeChangedMoralForBrigade* A = (ActiveTimeChangedMoralForBrigade*)((*Br->Ability)[i]);
						if(A->Owner==CM->OB->Index)
						{
							
						}
						*/
						have=true;
						break;
					}
				}
			}
			if(!have)
			{
				ActiveTimeChangedMoralForBrigade* A = new ActiveTimeChangedMoralForBrigade();
				A->Br=Br;
				A->SetA(BM);
				/*
				A->NI=OB->NNUM;
				A->BrigadeIndex=OB->BrigadeID;
				A->MoralAdd=BM->AddMoral;
				A->Radius=BM->Radius;
				
				A->ChangeMoral.Set(BM);
				*/
				A->AddMoral=BM->AddMoral+(*(CM->Le)-1)*BM->MoralAddForLevel;
				Br->Morale+=A->AddMoral;
				AddBrigadeAbility(Br,A);
				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
ActiveTimeChangedMoralForBrigade::ActiveTimeChangedMoralForBrigade()
{
	SetTime=AnimTime;
	Type=11;
}
bool ActiveTimeChangedMoralForBrigade::Process(Brigade* B)
{
	bool rez=false;
	if(B)
	{
		BrigadeTimeChangeMoral* A = (BrigadeTimeChangeMoral*)GetA();
		if(A)
		{
			if(AnimTime<(SetTime+A->EffectTime*25*256))
			{
				rez=true;
			}
			else
			{
				if(Br){
					Br->Morale-=AddMoral;
					//if(Br->Morale>Br->MaxMorale)
						//Br->Morale=Br->MaxMorale;
					if(Br->Morale<0)
						Br->Morale=0;
				}
			}
		}
	}
	return rez;
}
BrigadeIconInfo* ActiveTimeChangedMoralForBrigade::GetAbilityIcon()
{
	BrigadeTimeChangeMoral* A = (BrigadeTimeChangeMoral*)GetA();
	if(A)
	{
		return &A->IconInfo;
	}
	return NULL;
}
//==================================================================================================================//
AddBrigadeBonus::AddBrigadeBonus()
{
	Radius=0;
	AddDamage=0;
	AddShield=0;
	AddAttackRange=0;
	UnitsSpeedBonus=0;
	ExpGrowSpeed=0;
	AttackRateBonus=0;
	CureUnits=0;
	AddMaxLife=0;
	EnemyTarget=false;
	CoolDownTime=0;
	EffectTime=0;
	UsePause=10;
}
ActiveUnitAbility*  AddBrigadeBonus::GetNewActiveAbility()
{
	return (ActiveUnitAbility*) new ActiveAddBrigadeBonus();
}
//==================================================================================================================//
ActiveAddBrigadeBonus::ActiveAddBrigadeBonus()
{
	LastUseTime=0;
}
bool ActiveAddBrigadeBonus::Process()
{
	bool rez=false;
	if(!OB)
	{
		if(GetUnitIndex()!=0xFFFF)
			OB=Group[GetUnitIndex()];
	}
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
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
		else
		if(A->AddAttackRange)
			HintParam=A->AddAttackRange+(*Le-1)*A->AddForLevel;
		else
		if(A->UnitsSpeedBonus)
			HintParam=A->UnitsSpeedBonus+(*Le-1)*A->AddForLevel;
		else
		if(A->ExpGrowSpeed)
			HintParam=A->ExpGrowSpeed+(*Le-1)*A->AddForLevel;
		else
		if(A->AttackRateBonus)
			HintParam=A->AttackRateBonus+(*Le-1)*A->AddForLevel;
		else
		if(A->CureUnits)
			HintParam=A->CureUnits+(*Le-1)*A->AddForLevel;
		else
		if(A->AddMaxLife)
			HintParam=A->AddMaxLife+(*Le-1)*A->AddForLevel;
		rez=true;
	}
	if(OB&&OB->Sdoxlo>0&&OB->Sdoxlo<1000)
		rez=true;
	return rez;
}
bool ActiveAddBrigadeBonus::CanApply()
{
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
	if(A&&A->CoolDownTime)
	{
		return true;
	}
	return false;
}
int ActiveAddBrigadeBonus::GetCoolDownProc()
{
	int rez=0;
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
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
bool ActiveAddBrigadeBonus::ShowRadius()
{
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
	if(OB&&A&&A->Radius>10)
	{
		AddGroundCircle(OB->RealX>>4,OB->RealY>>4,A->Radius,0xFFFFFF00);
		return true;
	}
	return false;
}
int ActiveAddBrigadeBonus::GetRadius()
{
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
	if(OB&&A)
	{
		return A->Radius;
	}
	return 0;
}
bool ActiveAddBrigadeBonus::OnClick()
{
	if(*Vi)
	{
		return LeveledActiveUnitAbility::OnClick();
	}
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
	if(A&&A->CoolDownTime)
	{
		if(GetCoolDownProc()==0)
		{
			return Exec();
		}
	}
	return false;
}
int ActiveAddBrigadeBonus::GetHintParamOnLevel(int Lev)
{
	if(Lev>0&&Lev<6)
	{
		AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
		if(A)
		{
			int Base=0;
			if(A->AddDamage)
				Base=A->AddDamage;
			else
			if(A->AddShield)
				Base=A->AddShield;
			else
			if(A->AddAttackRange)
				Base=A->AddAttackRange;
			else
			if(A->UnitsSpeedBonus)
				Base=A->UnitsSpeedBonus;
			else
			if(A->ExpGrowSpeed)
				Base=A->ExpGrowSpeed;
			else
			if(A->AttackRateBonus)
				Base=A->AttackRateBonus;
			else
			if(A->CureUnits)
				Base=A->CureUnits;
			else
			if(A->AddMaxLife)
				Base=A->AddMaxLife;

			return Base + (Lev-1)*A->AddForLevel;
		}
	}
	return 0;
}
bool ActiveAddBrigadeBonus::Exec()
{
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
	if(OB&&A)
	{
		LastUseTime=AnimTime;
		PerformActionOverUnitsInRadius(OB->RealX>>4,OB->RealY>>4,A->Radius,&ActiveAddBrigadeBonus::SetBonus,(void*)this);
		return true;
	}
	return false;
}
bool ActiveAddBrigadeBonus::SetBonus(OneObject* OB, void* Param)
{
	bool rez=false;
	if(OB&&(!OB->Sdoxlo)&&OB->BrigadeID!=0xFFFF&&OB->BrIndex%5==0)
	{
		ActiveAddBrigadeBonus* AA = (ActiveAddBrigadeBonus*)Param;
		AddBrigadeBonus* A = (AddBrigadeBonus*)AA->GetA();
		Brigade* Br = &CITY[OB->NNUM].Brigs[OB->BrigadeID];
		if(((AA->OB->NMask&OB->NMask)&&!A->EnemyTarget)||((!(AA->OB->NMask&OB->NMask))&&A->EnemyTarget))
		{
			bool have=false;
			if(Br->Ability)
			{
				int n= Br->Ability->GetAmount();
				for(int i=0;i<n;i++)
				{
					if((*Br->Ability)[i]->Type==12)
					{
						ActiveBrigadeBonus* ABB = (ActiveBrigadeBonus*)((*Br->Ability)[i]);
						//if(ABB->HeroIndex==AA->OB->Index)
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
					ActiveBrigadeBonus* ABB = new ActiveBrigadeBonus();
					ABB->HeroIndex=AA->OB->Index;
					ABB->EffectTime=A->EffectTime;
					ABB->SetA(A);

					AddBrigadeAbility(Br,ABB);

					//Br->AddDamage+=A->AddDamage;
					//Br->AddShield+=A->AddShield;
					if(A->AddDamage)
					{
						AA->HintParam=A->AddDamage+(*(AA->Le)-1)*A->AddForLevel;
						ABB->AddDamage=AA->HintParam;
					}
					else
					if(A->AddShield)
					{
						AA->HintParam=A->AddShield+(*(AA->Le)-1)*A->AddForLevel;
						ABB->AddShield=AA->HintParam;
					}
					else
					if(A->AddAttackRange)
					{
						AA->HintParam=A->AddAttackRange+(*(AA->Le)-1)*A->AddForLevel;
						ABB->AddAttackRange=AA->HintParam;
					}
					else
					if(A->UnitsSpeedBonus)
					{
						AA->HintParam=A->UnitsSpeedBonus+(*(AA->Le)-1)*A->AddForLevel;
						ABB->UnitsSpeedBonus=AA->HintParam;
					}
					else
					if(A->ExpGrowSpeed)
					{
						AA->HintParam=A->ExpGrowSpeed+(*(AA->Le)-1)*A->AddForLevel;
						ABB->ExpGrowSpeed=AA->HintParam;
					}
					else
					if(A->AttackRateBonus)
					{
						AA->HintParam=A->AttackRateBonus+(*(AA->Le)-1)*A->AddForLevel;
						ABB->AttackRateBonus=AA->HintParam;
					}
					else
					if(A->CureUnits)
					{
						AA->HintParam=A->CureUnits+(*(AA->Le)-1)*A->AddForLevel;
						ABB->CureUnits=AA->HintParam;
					}
					else
					if(A->AddMaxLife)
					{
						AA->HintParam=A->AddMaxLife+(*(AA->Le)-1)*A->AddForLevel;
						ABB->AddMaxLife=AA->HintParam;
					}
					/*
					ABB->AddDamage=A->AddDamage+(A->AddDamage!=0)*(*(AA->Le)-1)*A->AddForLevel;
					ABB->AddShield=A->AddShield+(A->AddShield!=0)*(*(AA->Le)-1)*A->AddForLevel;
					ABB->AddAttackRange=A->AddAttackRange+(A->AddAttackRange!=0)*(*(AA->Le)-1)*A->AddForLevel;
					ABB->UnitsSpeedBonus=A->UnitsSpeedBonus+(A->UnitsSpeedBonus!=0)*(*(AA->Le)-1)*A->AddForLevel;
					ABB->ExpGrowSpeed=A->ExpGrowSpeed+(A->ExpGrowSpeed!=0)*(*(AA->Le)-1)*A->AddForLevel;
					ABB->AttackRateBonus=A->AttackRateBonus+(A->AttackRateBonus!=0)*(*(AA->Le)-1)*A->AddForLevel;
					ABB->CureUnits=A->CureUnits+(A->CureUnits!=0)*(*(AA->Le)-1)*A->AddForLevel;
					ABB->AddMaxLife=A->AddMaxLife+(A->AddMaxLife!=0)*(*(AA->Le)-1)*A->AddForLevel;
					*/

					Br->AddAttackRange+=ABB->AddAttackRange;
					Br->UnitsSpeedBonus+=ABB->UnitsSpeedBonus;
					Br->ExpGrowSpeed+=ABB->ExpGrowSpeed;
					Br->AttackRateBonus+=ABB->AttackRateBonus;
					if(ABB->AddMaxLife)
					{
						for(int i=NBPERSONAL;i<Br->NMemb;i++)
						{
							if(Br->Memb[i]!=0xFFFF)
							{
								OneObject* Obb = Group[Br->Memb[i]];
								if(Obb)
								{
									int pr=(Obb->Life*10000)/Obb->MaxLife;
									Obb->MaxLife+=ABB->AddMaxLife;
									Obb->Life=(Obb->MaxLife*pr)/10000;
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
//==================================================================================================================//
ActiveBrigadeBonus::ActiveBrigadeBonus()
{
	Type=12;
	SetTime=AnimTime;
	OneSecondTime=AnimTime;
	EffectTime=0;
	HeroIndex=-1;
}
ActiveBrigadeBonus::~ActiveBrigadeBonus()
{
	if(ActBr)
	{
		if(AddMaxLife)
		{
			for(int i=NBPERSONAL;i<ActBr->NMemb;i++)
			{
				if(ActBr->Memb[i]!=0xFFFF)
				{
					OneObject* Obb = Group[ActBr->Memb[i]];
					if(Obb)
					{
						int pr=(Obb->Life*10000)/Obb->MaxLife;
						Obb->MaxLife-=AddMaxLife;
						Obb->Life=(Obb->MaxLife*pr)/10000;
					}
				}
			}
			AddMaxLife=0;
		}
	}
}
bool ActiveBrigadeBonus::Process(Brigade* B)
{
	bool rez=false;
	if(B&&HeroIndex!=0xFFFF)
	{
		AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
		if(A)
		{
			ActBr=B;
			OneObject* OB = Group[HeroIndex];
			if(OB&&!OB->Sdoxlo)
			{
				if(EffectTime==0)
				{
					int x,y;
					byte Dir;
					B->GetCenter(&x,&y,&Dir);
					int R = Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
					if(R<(A->Radius+200))
					{
						rez=true;
					}
				}
				else
					if(AnimTime<(SetTime+EffectTime*25*256))
					{
						rez=true;
					}
				if(rez&&CureUnits)
				{
					int lad = (AnimTime-OneSecondTime)*CureUnits/(25*256*10);
					if(lad)
					{
						for(int i=NBPERSONAL;i<B->NMemb;i++)
						{
							if(B->Memb[i]!=0xFFFF)
							{
								OneObject* Obb = Group[B->Memb[i]];
								if(Obb)
								{
									Obb->Life+=lad;
									if(Obb->Life>Obb->MaxLife)
										Obb->Life=Obb->MaxLife;
								}
							}
						}
						OneSecondTime=AnimTime+lad*25*256*10/CureUnits;
					}
				}
			}
			if(!rez)
			{
				/*
				B->AddDamage-=A->AddDamage;
				if(B->AddDamage<0)
					B->AddDamage=0;
				B->AddShield-=A->AddShield;
				if(B->AddShield<0)
					B->AddShield=0;
				*/
				B->AddAttackRange-=AddAttackRange;
				B->UnitsSpeedBonus-=UnitsSpeedBonus;
				B->ExpGrowSpeed-=ExpGrowSpeed;
				B->AttackRateBonus-=AttackRateBonus;
				if(AddMaxLife)
				{
					for(int i=NBPERSONAL;i<B->NMemb;i++)
					{
						if(B->Memb[i]!=0xFFFF)
						{
							OneObject* Obb = Group[B->Memb[i]];
							if(Obb)
							{
								int pr=(Obb->Life*10000)/Obb->MaxLife;
								Obb->MaxLife-=AddMaxLife;
								Obb->Life=(Obb->MaxLife*pr)/10000;
							}
						}
					}
					AddMaxLife=0;
				}
			}
		}
	}
	
	return rez;
}
BrigadeIconInfo* ActiveBrigadeBonus::GetAbilityIcon()
{
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
	if(A)
	{
		return &A->IconInfo;
	}
	return NULL;
}
int ActiveBrigadeBonus::GetAddDamage()
{
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
	if(A)
	{
		return AddDamage;
	}
	return 0;
}
int ActiveBrigadeBonus::GetAddShield()
{
	AddBrigadeBonus* A = (AddBrigadeBonus*)GetA();
	if(A)
	{
		return AddShield;
	}
	return 0;
}
//==================================================================================================================//
//==================================================================================================================//
bool AddBrigadeAbility(Brigade* Br,ActiveBrigadeAbility* ABA)
{
	bool rez=false;
	if(Br&&ABA)
	{
		if(!Br->Ability)
			Br->Ability= new BrigadeActiveAbilityList();
		Br->Ability->Add(ABA);
		rez=true;
	}
	return rez;
}
//==================================================================================================================//

