#include "stdheader.h"
#include "unitability.h"
#include "UnitsInterface.h"
#include "BE_HEADERS.h"
#include "GameInterface.h"
#include "Camera\Navigator.h"
#include "CurrentMapOptions.h"
#include "TankController.h"
#include "MassivePlayer.h"
#include "Surface\Obj3D.h"

#include "ua_Item.h"

#define xassert(x) assert( (x) && __FUNCTION__[0] )
// USE FRO RETURN REFERENCE FOR TEMPORARY OBJECTS, fro DLL exports ///////
// Added by Vitya 
namespace __g{
	_str			g_refSTR;
	pParamList		g_refPARAMS;
	pGroup			g_refGROUP;
	iNode			g_refNODE;
	iRect			g_refRECT;
	RoundZone		g_refRZONE;
	SquareZone		g_refSZONE;
	pARRAY			g_refARRAY;
	pSMARTOBJECT	g_refSMARTOBJECT;
	pABILITY		g_refABILITY;
	pWEAPONMOD		g_refWEAPONMOD;
	pSTR_DIALOG		g_refDIALOG;
	pCAMERA			g_refCAMERA;

	float			g_refFLOAT;
	int				g_refINT;
	bool			g_refBOOL;
	DWORD			g_refDWORD;
};
void Kill_g_simple_refs(){
	__g::g_refFLOAT=0;
	__g::g_refINT=0;
	__g::g_refBOOL=false;
	__g::g_refDWORD=0;
}
void Kill_g_refs(){
	__g::g_refSTR="";
	//__g::g_refPARAMS.CLEAR();
	__g::g_refGROUP.set(NULL);
	__g::g_refNODE.set(NULL);
	__g::g_refRECT.set(0,0,0,0);
	__g::g_refRZONE.SetXYR(0,0,0);
	__g::g_refSZONE.SetRect(iRect(0,0,0,0));
	__g::g_refARRAY.CLEAR();
	__g::g_refSMARTOBJECT.set(NULL);
	__g::g_refABILITY.set((ReferableBaseClass*)NULL);
	__g::g_refWEAPONMOD.set(NULL);
	__g::g_refDIALOG.set(NULL);
	__g::g_refCAMERA.set(NULL);
	__g::g_refFLOAT=0;
	__g::g_refINT=0;
	__g::g_refBOOL=false;
	__g::g_refDWORD=0;
}
//////////////////////////////////////////////////////////////////////////
//construction
//////pUnitType/////
pUnitType::pUnitType(){
	Type=0;
}
pUnitType::pUnitType(int id){
	Type=id;
	xassert(id>=0 && id<NATIONS->NMon);
}
pUnitType::pUnitType(const char* Name){
	SetUnitTypeName(Name);
}
pUnitType::pUnitType(pUnit& unit){
	OfUnit(unit);	
}
//setting
void pUnitType::SetUnitType(int id){
	Type=id;
	xassert(id>=0 && id<NATIONS->NMon);
}
void pUnitType::SetUnitTypeName(const char* name){
	for(int i=0;i<NATIONS->NMon;i++){
		if(!strcmp(name,NATIONS->Mon[i]->MonsterID)){
			Type=i;
			return;
		}
	}
	xassert(true);
}
void pUnitType::OfUnit(pUnit& unit){
	OneObject* OB=unit.object();
	if(OB)Type=OB->NIndex;
}
pUnitType& pUnitType::operator = (pUnitType&  op){	
	Type=op.Type;
	return *this;
}
pUnitType& pUnitType::operator = (const char* s){
	SetUnitTypeName(s);
	return *this;
}
pUnitType& pUnitType::operator = (int id){
	SetUnitType(id);
	return *this;
}
pUnitType& pUnitType::operator = (pUnit& unit){
	OfUnit(unit);
	return *this;    
}
//compare operators
bool pUnitType::operator == (pUnitType& op){
	return op.Type==Type;
}
bool pUnitType::operator == (int id){
	return Type==id;
}
bool pUnitType::operator == (const char* name){    
	return !strcmp(GetName(),name);
}
bool pUnitType::operator == (pUnit& unit){
	OneObject* OB=unit.object();
	if(OB)return OB->NIndex==Type;
	return false;
}
const char*	pUnitType::GetName(){
	return NATIONS->Mon[Type]->MonsterID;
}
const char* pUnitType::GetFullName(){
	return NATIONS->Mon[Type]->Message;    
}
const char* pUnitType::GetMDFileName(){
	return NATIONS->Mon[Type]->newMons->MD_File;
}
const char* pUnitType::GetNationPreffix(){
	const char* s=GetName();
	const char* s1=strstr(s,"(");
	if(s1)return s1;
	else return "";
}
extern char** NatsIDS;
const char* pUnitType::GetNationName(){
	return NatsIDS[GetNationID()];
}
int pUnitType::GetNationID(){
	return NATIONS->Mon[Type]->NatID;
}
//////pUnit/////
OneObject* pUnit::object() const {
	if(ID!=0xFFFF){
		OneObject* OB=Group[ID];
		if(OB && OB->Serial==SN)return OB;
	}
	return NULL;
}
int pUnit::GetLiveStatus(){
	if( Valid() && object()->GetLiveState()==ls_LiveBody )
		return 1;
	return 0;
}; 
bool DetectMagazine(OneObject* OB);
bool pUnit::CheckVulnerabilityFrom(byte EnemyNI, pUnitType* UT){
	OneObject* OB=object();
	if( OB && EnemyNI<8 ){
		if( OB->Sdoxlo && OB->Hidden ) 
			return false;
		if(OB->GetLiveState()!=ls_LiveBody) 
			return false;

		if(UT){
			byte KillMask=NATIONS[EnemyNI].Mon[UT->GetType()]->newMons->KillMask;
			byte MathMask=OB->MathMask;
			if( !(KillMask&MathMask) )
				return false;
		}

		byte NMask=NATIONS[EnemyNI].NMask;
		if(NMask&OB->NMask) 
			return false;
		if(OB->newMons->Immortal) 
			return false;
		if(OB->ActiveAbility){
			bool inv=false;
			OB->ActiveAbility->ActiveAbilities.modifyInvulnerability(inv,inv);
			if(inv) return false;
			bool sv=true;
			inv=false;
			byte Det=0;
			OB->ActiveAbility->ActiveAbilities.modifyInvisibility(inv,inv,Det,sv); // pUnit::CheckVulnerabilityFrom
			if( inv && !(NMask&Det) ) 
				return false;
		}
		if( DetectMagazine(OB) )
			return false;
		if( DetectChest(OB) )
			return false;
		if( DetectBonus(OB) )
			return false;

		return true;
	}
	return false;
};
bool pUnit::IsVisible(byte NI){
	OneObject* OB=object();
	if( OB && NI<8 ){
		if( OB->ActiveAbility ){
			byte NMask=NATIONS[NI].NMask;
			bool inv=false;
			byte Det=0;
			bool sv=true;
			OB->ActiveAbility->ActiveAbilities.modifyInvisibility(inv,inv,Det,sv); //  pUnit::IsVisible
			if( inv && !(NMask&Det) ) 
				return false;
		}
		return true;
	}
	return false;
};
int	pUnit::GetLifeAfterNextShot(){
	OneObject* OB=object();
	if(OB){
		return ATTLIST.GetLifeAfterNextShot(OB);
	}
	return 0;
};
int pUnit::GetAdditionalRadius(){
	OneObject* OB=object();
	if(OB){
        return OB->newMons->AddShotRadius;
	}else return 0;
}
int pUnitType::GetAdditionalRadius(){
	NewMonster* NM=GetNewMonster();
	if(NM){
		return NM->AddShotRadius;
	}else return 0;
}

void			pUnitType::GetCost(byte NI,/*word NIndex,*/int* Cost/*,int x,int y*/){
	void GetUnitCost(byte NI,word NIndex,int* Cost,int x,int y);
	GetUnitCost(NI,Type,Cost,-1,-1);
};

iVector2D pUnit::GetBasicAttackRange(){
	iVector2D R(0,0);
	OneObject* OB=object();
	if(OB){
        R.Min=OB->MoreCharacter->MinR_Attack;
		R.Max=OB->MoreCharacter->MaxR_Attack;
	}
	return R;
}
iVector2D pUnit::GetAttackRange(){
	iVector2D R(0,0);
	OneObject* OB=object();
	if(OB){
		R.Min=OB->MoreCharacter->MinR_Attack;
		R.Max=OB->GetMaxAttackRadius();
	}
	return R;
}
iVector2D pUnit::GetBasicAttackRange(int Type){
	xassert(Type>=0 && Type<NAttTypes);
	iVector2D R(0,0);
	OneObject* OB=object();
	if(OB){
		R.Min=OB->MoreCharacter->AttackRadius1[Type];
		R.Max=OB->MoreCharacter->AttackRadius2[Type];
	}
	return R;
}
iVector2D pUnitType::GetAttackRange(){
	AdvCharacter* ADC=NATIONS->Mon[Type]->MoreCharacter;
	iVector2D v(ADC->MinR_Attack,ADC->MaxR_Attack);
	return v;
}
iVector2D pUnitType::GetAttackRange(int AttType){
	AdvCharacter* ADC=NATIONS->Mon[Type]->MoreCharacter;
	xassert(AttType>=0 && AttType<NAttTypes);
	iVector2D v(ADC->AttackRadius1[AttType],ADC->AttackRadius2[AttType]);
	return v;
}
NewMonster* pUnitType::GetNewMonster(){
	return NATIONS->Mon[Type]->newMons;
}
GeneralObject* pUnitType::GetGeneralObject(byte NI){
	xassert(NI<8);
	return NATIONS[NI].Mon[Type];
}
AdvCharacter* pUnitType::GetAdvCharacter(byte NI){
	xassert(NI<8);
	return NATIONS[NI].Mon[Type]->MoreCharacter;
}
NewMonster* pUnit::GetNewMonster(){
	OneObject* OB=object();
	if(OB)return OB->newMons;
	return NULL;
}
GeneralObject* pUnit::GetGeneralObject(){
	OneObject* OB=object();
	if(OB)return OB->Ref.General;
	return NULL;
}
AdvCharacter* pUnit::GetAdvCharacter(){
	OneObject* OB=object();
	if(OB)return OB->MoreCharacter;
	return NULL;
}
bool pUnit::CheckIfModified(){
	OneObject* OB=object();
	if(OB)return OB->newMons->CharacterWasModified;
	return false;
}
void MarkObjectLikeModified(OneObject* OB);
void pUnit::ModifyUnit(){
	OneObject* OB=object();
	if(OB)MarkObjectLikeModified(OB);
}
bool SetDefault(OneObject* OB);
void pUnit::UnModifyUnit(){
	OneObject* OB=object();
	if(OB)SetDefault(OB);
}
iVector2D pUnit::GetAttackRange(int Type){
	xassert(Type>=0 && Type<NAttTypes);
	iVector2D R(0,0);
	OneObject* OB=object();
	if(OB){
		R.Min=OB->MoreCharacter->AttackRadius1[Type];
		R.Max=OB->GetMaxAttackRadius(Type);		
	}
	return R;
}
int pUnit::DistTo(pUnit& OtherUnit){
	OneObject* OB=object();
	if(OB){
		OneObject* OOB=OtherUnit.object();
		if(OOB){
			return Norma(OOB->RealX-OB->RealX,OOB->RealY-OB->RealY)>>4;
		}
	}
	return 0;
}
int pUnit::DistTo(iVector2D V){
	iVector2D v=GetCoor();
	v.x-=V.x;
	v.y-=V.y;
	return Norma(v.x,v.y);
}
pUnit::pUnit(word _ID,word _SN){
	ID=_ID;
	SN=_SN;
}
pUnit::pUnit(word _ID){
	Set(_ID);	
}
pUnit::pUnit(OneObject* OB){
	if(OB){
		ID=OB->Index;
		SN=OB->Serial;
	}
}
pUnit::pUnit(){
	ID=0xFFFF;
	SN=0xFFFF;
}
pUnit::pUnit(DWORD UniqID){
	SetUniqID(UniqID);
}
void pUnit::Set(word _ID,word _SN){
	ID=_ID;
	SN=_SN;
}
void pUnit::Set(word _ID){
	SN=0xFFFF;
	ID=_ID;
	if(ID!=0xFFFF){
		OneObject* OB=Group[ID];
		if(OB)SN=OB->Serial;
	}
}
void pUnit::Set(OneObject* OB){
	if(OB){
		ID=OB->Index;
		SN=OB->Serial;
	}
}
// for LUA export
void	pUnit::Set_ID(int ID){
	Set((word)ID);
};
int		pUnit::Get_SN	() const{
	return (int)SN;	
};
int		pUnit::Get_ID	() const{
	return (int)ID;
};
DWORD	pUnit::Get_UID	() const{
	OneObject* pOB = object();
	return ( ( pOB!=NULL ) ? ( pOB->UniqID ) : ( 0xFFFF ) );
};
// --------------
iVector2D pUnit::GetCoor(){
	iVector2D v(0,0);
	OneObject* OB=object();
	if(OB){
		v.x=OB->RealX>>4;
		v.y=OB->RealY>>4;
	}
	return v;
}
iVector3D pUnit::GetCoor3D(){
	iVector3D v(0,0,0);
	OneObject* OB=object();
	if(OB){
		v.x=OB->RealX>>4;
		v.y=OB->RealY>>4;
		v.z=OB->RZ;
	}
	return v;
}
int pUnit::GetZ(){
	OneObject* OB=object();
	if(OB)return OB->RZ;
	return 0;
}
byte pUnit::GetNation(){
	OneObject* OB=object();
	if(OB)return OB->NNUM;
	return 0;
};
bool pUnit::IsBusy(){
	OneObject* OB=object();
	if(OB)return OB->LocalOrder!=NULL;
	return false;
}
bool pUnit::IsStand(){
	OneObject* OB=object();
	if(OB)return OB->DestX<0;
	else return false;
}
bool pUnit::IsBuilding(){
	OneObject* OB=object();
	if(OB)return OB->NewBuilding;
	else return false;
}
bool pUnit::IsAttacking(){
	OneObject* OB=object();
	if(OB)return OB->Attack;
	else return false;
}
void TakeResLink(OneObject* OBJ);
void TakeResourceFromSpriteLink(OneObject* OBJ);
bool pUnit::IsTakingResources(){
	OneObject* OB=object();
	if(OB){
		Order1* OR=OB->LocalOrder;
		int n=0;
		//while(OR && (n++)<100){
		if(OR){ // ИИ не нужно гоняться за идущими крестьянами
			if( OR->DoLink==&TakeResLink || OR->DoLink==&TakeResourceFromSpriteLink ) 
				return true;
			OR=OR->NextOrder;
		}
	}
	return false;
}
void BuildObjLink(OneObject* OBJ);
bool pUnit::IsRepairing(){
	OneObject* OB=object();
	if(OB){
		Order1* OR=OB->LocalOrder;
		int n=0;
		//while(OR && (n++)<100){
		if(OR){ // ИИ не нужно гоняться за идущими крестьянами
			if(OR->DoLink==&BuildObjLink)return true;
			OR=OR->NextOrder;
		}
	}
	return false;
}
int pUnit::GetStandTicks(){
	OneObject* OB=object();
	if(OB)return OB->StandTime;
	else return 0;
}
pUnit pUnit::GetCurrentVictim(){
	OneObject* OB=object();
	if(OB && OB->Attack){
		pUnit u(OB->EnemyID,OB->EnemySN);
		return u;
	}else{
		pUnit u(0xFFFF,0xFFFF);
		return u;
	}
}
void pUnit::ClearOrders(){
	OneObject* OB=object();
	if(OB)OB->ClearOrders();
}
bool pUnit::Valid(){
	return object()!=NULL;
}
DWORD pUnit::GetCurrentAnimationType(){
	OneObject* OB=object();
	if(OB){
		return OB->NewAnm->Code;
	}
	return 0;
}
int pUnit::GetCurrentAnimationFrame(){
	OneObject* OB=object();
	if(OB){
		return OB->NewCurSprite;
	}
	return 0;
}
int pUnit::GetPrevAnimationFrame(){
	OneObject* OB=object();
	if(OB){
		return OB->NewCurSpritePrev;
	}
	return 0;
}
bool pUnit::CheckIfFrameIsNow(int Frame){
	OneObject* OB=object();
	if(OB){
		return Frame>OB->NewCurSpritePrev && Frame<=OB->NewCurSprite;
	}
	return true;
}
bool pUnit::IsFrameFinished(){
	OneObject* OB=object();
	if(OB)return OB->FrameFinished;
	else return true;
}
bool pUnit::AnimationCanBeBroken(){
	OneObject* OB=object();
	if(OB){
		return OB->NewCurSprite>=OB->NewAnm->CanBeBrokenAfterFrame;
	}
	return false;
}
DWORD pUnit::GetMask(){
	OneObject* OB=object();
	if(OB){
		return OB->NMask;
	}
	return 0;
}
int pUnit::GetSpeed(){
	OneObject* OB=object();
	if(OB){
		return OB->GroupSpeed;
	}
	return 0;
}
iVector2D pUnit::GetLocalDesination(){	
	OneObject* OB=object();
	if(OB)return iVector2D(OB->DestX>>4,OB->DestY>>4);
	else return iVector2D(-1,-1);
}
void pUnit::SetLife(int Value){
	OneObject* OB=object();
	if(OB)OB->Life=Value;
}
int pUnit::GetLife(){
	OneObject* OB=object();
	if(OB)return OB->Life;
	else return 0; 
}
int pUnit::GetMaxLife(){
	OneObject* OB=object();
	if(OB)return OB->MaxLife;
	else return 1; 
}
void pUnit::SetSearchVictim(bool SearchState){
	OneObject* OB=object();
	if(OB)OB->NoSearchVictim=!SearchState;
}
bool pUnit::GetSearchVictim(){
	OneObject* OB=object();
	if(OB)return !OB->NoSearchVictim;
	else return false;
}
void NewMonsterSendToLink(OneObject* OB);
void NewMonsterSmartSendToLink(OneObject* OBJ);
void NewMonsterPreciseSendToLink(OneObject* OB);

iVector2D pUnit::GetGlobalDesination(){
	OneObject* OB=object();
	if(!OB)return iVector2D(-1,-1);
	iVector2D v(OB->DestX>>4,OB->DestY>>4);	
	Order1* OR=OB->LocalOrder;
	int n=0;
	while(OR && (n++)<100){
		if(OR->DoLink==&NewMonsterSendToLink){
			v=iVector2D(OR->info.MoveToXY.x*16+8,OR->info.MoveToXY.y*16+8);
		}else
			if(OR->DoLink==&NewMonsterSmartSendToLink){
				v=iVector2D(OR->info.SmartSend.x+OR->info.SmartSend.dx,OR->info.SmartSend.y+OR->info.SmartSend.dy);
			}else
				if(OR->DoLink==&NewMonsterPreciseSendToLink){
					v=iVector2D(OR->info.MoveToXY.x>>4,OR->info.MoveToXY.y>>4);
				}
				OR=OR->NextOrder;
	}
	return v;
}
void pUnit::SetCurrentAnimation(DWORD Code){
	OneObject* OB=object();
	if(OB){
		OB->NewAnm=OB->newMons->Animations->Get(Code);
		OB->SetZeroFrame();
	}
}
void pUnit::ApplyAbility(const char* AbilityName){
	GameInterface::ApplyAbility(AbilityName,*this,*this);
}
void pUnit::BornWeaponUnderUnit(const char* WeaponName,int extraZ,pUnit* DestUnit){
	GameInterface::CreateSpecialEffect(WeaponName,*this,extraZ,*DestUnit,0);
}
void pUnit::SetGroupSpeed(int Speed){
	OneObject* OB=object();
	if(OB){
		OB->GroupSpeed=Speed;
	}
}
void pUnit::SetMask(DWORD Mask){
	OneObject* OB=object();
	if(OB){
		OB->NMask=Mask;
	}
}
void pUnit::SetCoor(iVector2D pos){
	OneObject* OB=object();
	if(OB){
		void ChangeUnitCoor(OneObject* OB,int newX,int newY);
		addrand(pos.x);
		addrand(pos.y);
		ChangeUnitCoor(OB,pos.x<<4,pos.y<<4);
	}
}
void pUnit::SetZ(int z){
	OneObject* OB=object();
	if(OB){
		OB->OverEarth=z;
	}
}
void pUnit::SetScale(float Scale){

}
DLLEXPORT void ChangeUnitColor(int Index,byte DstNI);
void pUnit::SetNation(byte NI){
	OneObject* OB=object();
	if(OB!=NULL){
		ChangeUnitColor(OB->Index,NI);
	};
};
void pUnit::SetUniqID(DWORD UniqID){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB && OB->UniqID==UniqID){
			Set(OB);
			break;
		}
	}
}
pUnit& pUnit::operator = (pUnit& unit){
	ID=unit.ID;
	SN=unit.SN;
	return *this;
}
pUnit& pUnit::operator = (word _ID){
	Set(_ID);
	return *this;
}
bool pUnit::operator == (pUnitType& Type){	
	return	Type==(*this);
}
bool pUnit::operator != (pUnitType& Type){
	return !( Type==(*this) );	
};
bool pUnit::operator == (pUnit& unit){
	return ID==unit.ID && SN==unit.SN;
}
void pUnit::SendUnitFarTo(int x,int y,int dx,int dy,int Prio,int OrdType){
	OneObject* OB=object();
	if(OB)OB->NewMonsterSmartSendTo(x,y,dx,dy,Prio,OrdType);
}
void pUnit::SendUnitNearAndPrecise(int	x,int y,int Prio,int OrdType){
	OneObject* OB=object();
	if(OB)OB->NewMonsterPreciseSendTo(x<<4,y<<4,Prio,OrdType);
}
bool pUnit::AttackUnit(pUnit& Victim,int NTimes,int Prio,int OrdType){
	OneObject* OB=object();
	if(OB)return OB->AttackObj(Victim.index(),Prio,OrdType,NTimes);
	return false;
}
bool pUnit::AttackPoint(int x,int y,int NTimes,int Prio,int OrdType){
	OneObject* OB=object();
	if(OB)return OB->AttackPoint(x,y,GetTotalHeight(x,y),NTimes,0,OrdType,Prio);
	return false;
}
bool CheckPossibilityToAttackEnemy(OneObject* OB,OneObject* EOB);
bool pUnit::CheckAttackPossibility(pUnit& Victim){
	OneObject* OB=object();
	OneObject* EN=Victim.object();
	return OB && EN && CheckPossibilityToAttackEnemy(OB,EN);

}
void pUnit::MakeStepTo(int x,int y){
	OneObject* OB=object();
	if(OB)OB->CreatePath(x>>4,y>>4);
}
void pUnit::Die(){
	OneObject* OB=object();
	if(OB)OB->Die();
}
void pUnit::Erase(){
	OneObject* OB=object();
	if(OB){
		OB->CloseObject();
		Group[ID]=NULL;
		free(OB);
		//OB->Die(true);
		//OB=object();
		//if(OB)OB->Sdoxlo=6000;
	}
}
void pUnit::MakeDamageByAttackType(int Damage,int AttType,pUnit& AttackingUnit){
	OneObject* OB=object();
	if(OB)OB->MakeDamage(Damage,Damage,AttackingUnit.object(),AttType);
}
void pUnit::MakeDamageByDamageType(int Damage,int DamType,pUnit& AttackingUnit){
	OneObject* OB=object();
	if(OB)OB->MakeDamage(Damage,Damage,AttackingUnit.object(),DamType+128);
}
bool pUnit::TakeResource(int ResID){
	OneObject* OB=object();
	if(OB)return (OB->TakeResource(OB->RealX>>4,OB->RealY>>4,ResID,128+16,0)<0x0FFFFFFF);
}
bool pUnit::TakeResource(int x,int y,int ResID){
	OneObject* OB=object();
	if(OB)return (OB->TakeResource(x,y,ResID,128+16,0)<0x0FFFFFFF);
}
bool pUnit::TakeResourceRelative(int dx,int dy,int ResID){
	OneObject* OB=object();
	if(OB)return (OB->TakeResource(dx+(OB->RealX>>4),dy+(OB->RealY>>4),ResID,128+16,0)<0x0FFFFFFF);   
}
void DetonateUnit(OneObject* OB,int CenterX,int CenterY,int Force);
void pUnit::DetonateUnit(int dx,int dy,int Force){
	iVector2D v=GetCoor();
	OneObject* OB=object();
	if(OB)::DetonateUnit(OB,v.x+dx,v.y+dy,Force);
}
void pUnit::ProduceUnit(pUnitType& Type,pCollectorGroup* Dest,int Time){
	OneObject* OB=object();
	if(OB){
		ActiveGroup* G=Dest?Dest->group():NULL;
		if(G)OB->Produce(Type.GetType());
		else OB->Produce(Type.GetType(),Dest->GetCollectorIndex());
	}
}
bool pUnit::ProduceDefaultUnit(pUnitType& Type,pCollectorGroup* Dest){
	OneObject* OB=object();
	if(OB){
		ActiveGroup* G=Dest?Dest->group():NULL;
		if(G)OB->Produce(Type.GetType());
		else OB->Produce(Type.GetType(),Dest->GetCollectorIndex());
	}
	return true;
}
void pUnit::GetProduceList(pUnitType& Producer,pUnitTypeList& List){
}
void pUnit::GetEnabledProduceList(pUnit& Producer,pUnitTypeList& List){
}
void pUnit::GetUpgradeList(pUpgradeList& List){
}
void pUnit::GetEnabledUpgradeList(pUpgradeList& List){
}

void pUnit::GoToContainer(pUnit& container){
	OneObject* OB=object();
	OneObject* COB=container.object();
	if(OB && COB){
        OB->GoToMine(COB->Index,128+16,0);		
	}
}
void pUnit::PushOutUnit(pUnitType& Type){
	OneObject* OB=object();	
	if(OB){
		OB->LeaveMine(Type.GetType());
	}    
}
void pUnit::PushOutUnit(pUnit& Unit){
	OneObject* COB=object();	
	OneObject* OB=Unit.object();	
	if(OB && COB){
		for(int i=0;i<COB->NInside;i++)if(COB->Inside[i]==OB->Index){
            if(i!=COB->NInside-1)swap(COB->Inside[i],COB->Inside[COB->NInside-1]);
			break;
		}
		COB->LeaveMine(OB->NIndex);		
	}
}
void pUnit::PushOutUnit(){
	OneObject* COB=object();	
	if(COB){
		if(COB->NInside){
			pUnit u(COB->Inside[COB->NInside-1]);
			pUnitType ut(u);
			COB->LeaveMine(ut.GetType());
		}
	}	
}
void pUnit::PushOutAllUnits(){
	OneObject* COB=object();	
	if(COB){
		for(int i=0;i<COB->NInside;i++)PushOutUnit();
	}    
}
int  pUnit::GetInsideAmount(){
	OneObject* COB=object();	
	if(COB)return COB->NInside;
	return 0;
}
int	pUnit::GetInsideUnitID(int LID){
	OneObject* COB=object();	
	if(COB&&LID>=0&&LID<COB->NInside){
		return (int)(COB->Inside[LID]);
	};
	return -1;
};
void pUnit::GetInsideUnits(pUnitsList& units){
	OneObject* COB=object();	
	if(COB){
		for(int i=0;i<COB->NInside;i++){
			pUnit u(COB->Inside[i]);
			units._add(u);
		}
	}    
}
int  pUnit::GetAmountOfMovingTo(pUnit& container){
	int GetAmountOfMoversTo(OneObject* Dest,OneObject* Mover=NULL);
	OneObject* COB=object();	
	if(COB)return GetAmountOfMoversTo(COB);
	else 0;

}
void pUnit::GetMovingTo(pUnitsList& Units){
	//not implemented yet
}
bool pUnit::RemoveFromContainer(){
	OneObject* OB=object();
	if(OB){
		pUnit u(OB->LockedInBuilding);
		RemoveFromContainer(u);
	}
	return false;
}
bool pUnit::RemoveFromContainer(pUnit& Container){
    OneObject* OB=object();
	OneObject* COB=Container.object();
	if(OB && COB){
		int N=COB->NInside;
		for(int i=0;i<N;i++)if(COB->Inside[i]==OB->Index){
            OB->LockedInBuilding=0xFFFF;
			if(i<N-1)memcpy(COB->Inside+i,COB->Inside+i+1,(N-i-1)*2);
			COB->NInside--;
			if(!COB->NInside){
				free(COB->Inside);
				COB->Inside=NULL;
			}
			if(!COB->newMons->ShowInsideUnits)OB->Hidden=false;
			return true;
		}
	}
	return false;
}
void pUnit::InsertToContainer(pUnit& Container){
	OneObject* OB=object();
	OneObject* COB=Container.object();
	if(OB && COB){
		COB->Inside=(word*)realloc(COB->Inside,(COB->NInside+1)*2);
		COB->Inside[COB->NInside++]=OB->Index;
		if(COB->newMons->ShowInsideUnits){
            OB->LockedInBuilding=COB->Index;
		}else{
            OB->Hidden=true;
			OB->Sdoxlo=true;
		}
		SetCoor(Container.GetCoor());
	}	
}
DLLEXPORT bool GetBuildingEntryPoint(int* x,int* y,word Index,int AddDistance);
iVector2D pUnit::GetOutputPoint(int AdditionalDistance){
    iVector2D V=GetCoor();
	GetBuildingEntryPoint(&V.x,&V.y,ID,AdditionalDistance);
	return V;
}
bool GetBuildingInternalPoint(int* x,int* y,word Index);
iVector2D pUnit::GetInsidePoint(){
	iVector2D V=GetCoor();
	GetBuildingInternalPoint(&V.x,&V.y,ID);
	return V;
}
void pUnit::Hide(){
    OneObject* OB=object();
	if(OB)OB->Hidden=true;
}
void pUnit::UnHide(){
	OneObject* OB=object();
	if(OB)OB->Hidden=false;
}
bool pUnit::Hidden(){
	OneObject* OB=object();
	if(OB)return OB->Hidden;
	return true;
}

// for LUA export
void							pUnit::PushOutUnit00(pUnit* Unit){
	if (Unit!=NULL) PushOutUnit(*Unit);
};
void							pUnit::PushOutUnit01(){
	PushOutUnit();
};
bool							pUnit::RemoveFromContainer00(){
	return RemoveFromContainer();
};
bool							pUnit::RemoveFromContainer01(pUnit* Container){
	if (Container!=NULL) return 	RemoveFromContainer(*Container);
	return false;
};
void							pUnit::SetHideState( bool state ){
	( ( state ) ? ( ((pUnit*)this)->Hide() ) : ( ((pUnit*)this)->UnHide() ) );	
};
bool							pUnit::GetHideState() const{
	return ((pUnit*)this)->Hidden();
};

// --------------

//////RawUnit/////
RawUnit::RawUnit(){
	ID=0xFFFF;
	SN=0xFFFF;
}
RawUnit::RawUnit(word _id,word _sn){
	ID=_id;
	SN=_sn;
}
OneObject* RawUnit::object(){
	if(ID!=0xFFFF){
		OneObject* OB=Group[ID];
		if(OB && OB->Serial==SN)return OB;
	}
	return NULL;
}
//////pUnitList/////
RawUnit pUnitsList::_getunit(int idx){
	return units[idx];
}
void pUnitsList::_setunit(int idx,RawUnit& u){
	units[idx]=u;
}
int pUnitsList::_getamount(){
	return units.GetAmount();
}
void pUnitsList::_add(RawUnit& u){    
	units.Add(u);
}
void pUnitsList::_del(int pos,int n){
	units.Del(pos,n);
}
int	pUnitsList::_find(RawUnit& r){
	int n=units.GetAmount(); //_getamount()
	for(int i=0;i<n;i++){
		RawUnit& u=units[i];
		if( u.ID==r.ID && u.SN==r.SN ){
			return i;
		}
	}
	return -1;
};
void pUnitsList::Select(byte NI, bool add ){
	//int NI = getNation();
	if ( add==false ){
		pSelected::units(NI).Clear();
		pImSelected::units(NI).Clear();
	};
	ForEachUnit	(	(*this), 
			pSelected::units(NI)+=unit; 
			pImSelected::units(NI)+=unit; 
		);
};
void pUnitsList::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	pUnitsList* pUL=(pUnitsList*)ClassPtr;
	static DString str;
	str.Clear();
	int n=pUL->GetAmount();
	for(int i=0;i<n;i++){
		str.print("%d ",pUL->units[i].ID);
	}
    xml.AddSubQuote("pUnitsList",str.pchar());
};
bool pUnitsList::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	xmlQuote* pxml=xml.GetSubQuote("pUnitsList");
	if(pxml){
		pUnitsList* pUL=(pUnitsList*)ClassPtr;
		pUL->Clear();
		static DString str;
		str=pxml->Get_string();
		if(str.pchar()[0]){
			static DString sid;
			while(true){
				str.ExtractWord(sid);
				int id=-1;
				char* c=str.pchar();
				int n=sscanf(c,"%d",&id);
				if(n>0&&id!=-1){
					OneObject* OB=Group[id];
					if(OB){
						RawUnit u(OB->Index,OB->Serial);
						pUL->units.Add(u);
					}
				}else{
					break;
				}
			}
		}
	}
	return true;
};

//////pParamList/////
pParamList::pParamList(){
	_params = NULL;	
};
pParamList::pParamList(pParamList* pPL){
	if ( pPL->Valid() )	_params = pPL->params();
};
pParamList::pParamList(be_CParamList* pPL){
	if ( pPL!=NULL ) _params = pPL;
};
be_CParamList*	pParamList::params(){
	return _params;
};
bool			pParamList::Valid(){
	return ( params()!=NULL );	
};
char*			pParamList::getParentFunctionName(){
	__g::g_refSTR = "UnNoneFunctionName";
	if ( Valid()==false ) return __g::g_refSTR.pchar();
	BaseClass* pBC = params()->GetParent();
	be_CFunctionBase* pFB = NULL;
	while (pBC) {
		pFB = dynamic_cast<be_CFunctionBase*>( pBC );
		if ( pFB!=NULL ){
			if ( __g::g_refSTR == "UnNoneFunctionName" ){
				__g::g_refSTR =  pFB->Name;
			}else{
				__g::g_refSTR += "::";
				__g::g_refSTR += pFB->Name;
			};
		};
		pBC = pBC->GetParent();
	}
	return __g::g_refSTR.pchar();
};
bool	pParamList::getERROR(bool NotError,char* Type,char* Name){
	if (!NotError) {
		DriveMode()->PROCESS=false;
		if ( Name==NULL ){
			if ( Type!=NULL && Type[0]!=0 ){
				MessageBox( NULL, "Params list is NULL.", "SCRIPT ERROR <get parametr>", MB_OK );
			}else{
				_str msg = "get < ";
				msg += Type;
				msg += " > Params list is NULL.";
				MessageBox( NULL, msg.pchar(), "SCRIPT ERROR <get parametr>", MB_OK );
			}
		};
		if ( Name!=0 && Name[0]!=0 && Type!=NULL && Type[0]!=0 ){
			_str mes = "Function < ";
			mes += getParentFunctionName();
			mes += " > can not get parametr < ";
			mes += Type;
			mes += " : ";
			mes += Name;
			mes += " >";
			MessageBox( NULL, mes.pchar(), "SCRIPT ERROR <get parametr>", MB_OK );
		}
	};
	return (!NotError);
};
bool	pParamList::addERROR(bool NotError,char* Type){
	if (!NotError) {
		DriveMode()->PROCESS=false;
		if ( Type==NULL ){
			MessageBox( NULL, "Params list is NULL.", "SCRIPT ERROR <add parametr>", MB_OK );
		};
		if ( Type!=NULL && Type[0]!=0 ){
			_str mes = "Function < ";
			mes += getParentFunctionName();
			mes += " > can not add parametr < ";
			mes += Type;
			mes += " >";
			MessageBox( NULL, mes.pchar(), "SCRIPT ERROR <add parametr>", MB_OK );
		}
	};
	return (!NotError);
};
void	pParamList::CLEAR(){
	if ( Valid() )	params()->CLEAR();
};
void	pParamList::CreateOBJ_PL(){
	be_CParamList* pPL = new be_CParamList();
	_params = pPL;
};
void	pParamList::DeleteOBJ_PL(){
	if ( Valid() ){
		delete _params;
		_params = NULL;
	};
};
bool	pParamList::check( char* Name ){
	if ( Name!=NULL && Name[0]!=0 && Valid() ){
		if ( params()->CheckParam(Name) ){
			return ( getVOID(Name)!=NULL );
		}
	}
	return false;
};
bool	pParamList::IsLocal( char* Name ){
	if ( Valid() )	return params()->IsLocal(Name);
	return false;
};
pGroup	pParamList::getGROUP( char* Name ){
	if ( getERROR(Valid(), "GROUP") ) return __g::g_refGROUP;
	be_CGROUP* pGRP = (be_CGROUP*)(params()->GetValue(Name));
	if ( getERROR(pGRP!=NULL,"GROUP",Name) ) return __g::g_refGROUP;
	__g::g_refGROUP.set( pGRP );
	return __g::g_refGROUP;
};
iNode	pParamList::getNODE( char* Name ){
	if ( getERROR(Valid(), "NODE") ) return __g::g_refNODE;
	be_CNODE* pCND = (be_CNODE*)(params()->GetValue(Name));
	if ( getERROR(pCND!=NULL,"NODE",Name) ) return __g::g_refNODE;
	__g::g_refNODE.set( pCND );
	return __g::g_refNODE;
};
float&	pParamList::getFLOAT( char* Name ){
	if ( getERROR(Valid(), "FLOAT") ) return __g::g_refFLOAT;
	be_CFLOAT* pV = (be_CFLOAT*)(params()->GetValue(Name));
	if ( getERROR(pV!=NULL,"FLOAT",Name) ) return __g::g_refFLOAT;
	return pV->Value;
};
_str&	pParamList::getSTR( char* Name ){
	if ( getERROR(Valid(), "STR") ) return __g::g_refSTR;
	be_CSTR* pV = (be_CSTR*)(params()->GetValue(Name));
	if ( getERROR(pV!=NULL,"STR",Name) ) return __g::g_refSTR;
	return pV->Value;
};
int&	pParamList::getINT( char* Name ){
	if ( getERROR(Valid(), "INT") ) return __g::g_refINT;
	be_CINT* pV = (be_CINT*)(params()->GetValue(Name));
	if ( getERROR(pV!=NULL,"INT",Name) ) return __g::g_refINT;
	return pV->Value;
};
DWORD&	pParamList::getDWORD( char* Name ){
	if ( getERROR(Valid(), "DWORD") ) return __g::g_refDWORD;
	be_CDWORD* pV = (be_CDWORD*)(params()->GetValue(Name));
	if ( getERROR(pV!=NULL,"DWORD",Name) ) return __g::g_refDWORD;
	return pV->Value;
};
bool&	pParamList::getBOOL( char* Name ){
	if ( getERROR(Valid(), "BOOL") ) return __g::g_refBOOL;
	be_CBOOL* pV = (be_CBOOL*)(params()->GetValue(Name));
	if ( getERROR(pV!=NULL,"BOOL",Name) ) return __g::g_refBOOL;
	return pV->Value;
};
pARRAY	pParamList::getARRAY( char* Name ){
	if ( getERROR(Valid(), "ARRAY") ) return __g::g_refARRAY;
	be_CARRAY* pCAR = (be_CARRAY*)(params()->GetValue(Name));
	if ( getERROR(pCAR!=NULL,"ARRAY",Name) ) return __g::g_refARRAY;
	__g::g_refARRAY.set( pCAR );
	return __g::g_refARRAY;
};
pSTR_DIALOG	pParamList::getDIALOG( char* Name ){
	if ( getERROR(Valid(), "STR_DIALOG") ) return __g::g_refDIALOG;
	be_CSTR_DIALOG* pCDLG = (be_CSTR_DIALOG*)(params()->GetValue(Name));
	if ( getERROR(pCDLG!=NULL,"STR_DIALOG",Name) ) return __g::g_refDIALOG;
	__g::g_refDIALOG.set( pCDLG );
	return __g::g_refDIALOG;
};
pABILITY	pParamList::getABILILTY( char* Name ){
	if ( getERROR(Valid(), "ABILITY") ) return __g::g_refABILITY;
	ReferableBaseClass* pABIL = (ReferableBaseClass*)(params()->GetValue(Name));
	if ( getERROR(pABIL!=NULL,"ABILITY",Name) ) return __g::g_refABILITY;
	__g::g_refABILITY.set( pABIL );
	return __g::g_refABILITY;
};
pWEAPONMOD		pParamList::getWEAPONMOD( char* Name ){
	if ( getERROR(Valid(), "WEAPONMOD") ) return __g::g_refWEAPONMOD;
	ReferableBaseClass* pWM = (ReferableBaseClass*)(params()->GetValue(Name));
	if ( getERROR(pWM!=NULL,"WEAPONMOD",Name) ) return __g::g_refWEAPONMOD;
	__g::g_refWEAPONMOD.set( pWM );
	return __g::g_refWEAPONMOD;
};
pCAMERA		pParamList::getCAMERA( char* Name ){
	if ( getERROR(Valid(), "CAMERA") ) return __g::g_refCAMERA;
	ReferableBaseClass* pC = (ReferableBaseClass*)(params()->GetValue(Name));
	if ( getERROR(pC!=NULL,"CAMERA",Name) ) return __g::g_refCAMERA;
	__g::g_refCAMERA.set( pC );
	return __g::g_refCAMERA;
};
char*	pParamList::getValueName	( char* Name ){
	if ( getERROR(Valid(),	"VALUE_NAME") )	return NULL;
	ReferableBaseClass* pV = (ReferableBaseClass*)(params()->GetValue(Name));
	if ( getERROR(pV!=NULL,"VALUE_NAME",Name) ) return NULL;
	return pV->Name.pchar();
};
int		pParamList::getSIZE(){
	if ( getERROR(Valid(), "SIZE") ) return 0;
	return params()->GetAmount();		
};
bool	pParamList::setREF( char* Name, pGroup& GR ){
	if ( Valid() && GR.Valid() ) return setREF( Name, (void*)(GR.group()) );
	return false;
};
bool	pParamList::setREF( char* Name, iNode&	ND ){
	if ( Valid() && ND.Valid() ) return setREF( Name, (void*)(ND.node()) );
	return false;
};
bool	pParamList::setREF( char* Name, pARRAY&	AR ){
	if ( Valid() && AR.Valid() ) return setREF( Name, (void*)(AR.array()) );
	return false;
};
pGroup&	pParamList::addGROUP( char* Name ){
	if ( addERROR(Valid()) ) return __g::g_refGROUP;
	if ( addERROR(Name!=NULL&&Name[0]!=0,"GROUP") ) return __g::g_refGROUP;
	__g::g_refGROUP.set( params()->CreateParam_GROUP(Name) );
	return __g::g_refGROUP;
};
iNode&	pParamList::addNODE( char* Name ){
	if ( addERROR(Valid()) ) return __g::g_refNODE;
	if ( addERROR(Name!=NULL&&Name[0]!=0,"NODE") ) return __g::g_refNODE;
	__g::g_refNODE.set( params()->CreateParam_NODE(Name) );
	return __g::g_refNODE;
};
float&	pParamList::addFLOAT( char* Name ){
	if ( addERROR(Valid()) ) return __g::g_refFLOAT;
	if ( addERROR(Name!=NULL&&Name[0]!=0,"FLOAT") ) return __g::g_refFLOAT;
	return	(params()->CreateParam_FLOAT(Name))->Value;
};
_str&	pParamList::addSTR( char* Name ){
	if ( addERROR(Valid()) ) return __g::g_refSTR;
	if ( addERROR(Name!=NULL&&Name[0]!=0,"STR") ) return __g::g_refSTR;
	return	(params()->CreateParam_STR(Name))->Value;
};
int&	pParamList::addINT( char* Name ){
	if ( addERROR(Valid()) ) return __g::g_refINT;
	if ( addERROR(Name!=NULL&&Name[0]!=0,"INT") ) return __g::g_refINT;
	return	(params()->CreateParam_INT(Name))->Value;
};
DWORD&	pParamList::addDWORD( char* Name ){
	if ( addERROR(Valid()) ) return __g::g_refDWORD;
	if ( addERROR(Name!=NULL&&Name[0]!=0,"DWORD") ) return __g::g_refDWORD;
	return	(params()->CreateParam_DWORD(Name))->Value;
};
bool&	pParamList::addBOOL( char* Name ){
	if ( addERROR(Valid()) ) return __g::g_refBOOL;
	if ( addERROR(Name!=NULL&&Name[0]!=0,"BOOL") ) return __g::g_refBOOL;
	return	(params()->CreateParam_BOOL(Name))->Value;
};
pARRAY&	pParamList::addARRAY( char* Name ){
	if ( addERROR(Valid()) ) return __g::g_refARRAY;
	if ( addERROR(Name!=NULL&&Name[0]!=0,"ARRAY") ) return __g::g_refARRAY;
	__g::g_refARRAY.set( params()->CreateParam_ARRAY(Name) );
	return __g::g_refARRAY;
};
pParamList&	pParamList::operator=(pParamList& PL){
	if ( this!=&PL && PL.Valid() ){
		_params = PL.params();
	};
	return *this;
};
pParamList&	pParamList::set(be_CParamList* pPL){
	_params = pPL;
	return (*this);
};
void*		pParamList::getVOID( char* Name ){
	if ( Valid() ) return (void*)(params()->GetValue(Name));
	return NULL;
};
bool		pParamList::setREF(char* par_name,void* pData){
	if ( Valid() ){
		if ( pData ) return params()->SetParam(par_name,(be_CVariableBase*)pData);
		else		 return params()->SetParam(par_name,NULL);
	};
	return false;
};
//////////////////////////////////////////////////////////////////////////
// pGroup
//////////////////////////////////////////////////////////////////////////
#define	wrapGET(command)	if(Valid()){return group()->command;}else{return 0;};
#define	wrapSET(command)	if(Valid()){group()->command;};
pGroup::~pGroup(){
	be_CGROUP* g=group();
	if ( g==NULL ) return;
	if ( isLocal()==true ){
		_group.Set(NULL);
		delete g;
	};
};
pGroup::pGroup(){	
	_group.Set(NULL);
}
pGroup::pGroup(pGroup& GR){
	(*this)=GR;
};
pGroup::pGroup(be_CGROUP* pGR){
	if ( pGR!=NULL ){
		_group.Set( pGR );
	};
};
pGroup::pGroup(const char* Name){
	AttachTo(Name);
}
iVector2D		IUnitsIterator::GetCenter(){
	iVector2D vc(0,0);
	int i=0;
	//ForEachUnit()
	ToBegin();
	pUnit unit;
	while(NextLive(unit)){
		iVector2D v=unit.GetCoor();
		vc=vc+v;
		i++;
	}
	if(i>0){
		vc.x/=i;
		vc.y/=i;
	}
	return vc;
};
RawUnit			pGroup::_getunit(int idx){
	RawUnit R;
	be_CGROUP* _group=group();
	if(_group){
		OneUS& u=_group->Units[idx];
		R.ID=u.ID;
		R.SN=u.SN;
	}
	return R;
}
void			pGroup::_setunit(int idx,RawUnit& r){
	be_CGROUP* _group=group();
	if(_group){
		OneUS u(r.ID,r.SN);
		_group->Units[idx]=u;
	}
}
int				pGroup::_getamount(){
	be_CGROUP* _group=group();
	if(_group){
		return _group->Units.GetAmount();
	}	
	return 0;
}
void			pGroup::_add(RawUnit& r){
	be_CGROUP* _group=group();
	if(_group){
		_group->AddUnitGID(r.ID);
	};
};
void			pGroup::_del(int pos,int n){
	be_CGROUP* _group=group();
	if(_group){		
		_group->Units.Del(pos,n);
	}
}
be_CGROUP*		pGroup::group(){
	return _group.Get();
}
void			pGroup::AttachTo(const char* GroupName){
	_group.SetObjectName((char*)GroupName);
}
bool			pGroup::Valid(){
	return group()!=NULL;
}
pGroup&			pGroup::operator=(pGroup& pGR){
	if ( this!=&pGR && pGR.Valid() ){
		_group.Set( pGR.group() );
	};
	return *this;
};
pGroup&			pGroup::set(be_CGROUP* pGR){
	_group.Set(pGR);
	return *this;
};
// get set seme params
int				pGroup::getUN(){
	wrapGET( GetTotalAmount() );
};
int				pGroup::getUN(int NI){
	wrapGET( GetNationAmout(NI) );
};
int				pGroup::getUN(int NI, int UT){
	wrapGET( GetTotalAmount2(UT,NI) );
};
int				pGroup::getUN(iZone& ZN){
	if ( Valid()==false ) return 0;
	int UN = 0;
	ForEachUnit((*this), if ( ZN.IsIn( unit ) ) UN++; );
	return UN;
};
int				pGroup::getUN(int x, int y, int r){
	wrapGET( GetAmountInZone(x,y,r) );
};
int				pGroup::getDeadUN(){
	wrapGET( getDeadUN() );	
};
bool			pGroup::getReady(){
	wrapGET( GetReady() );
};
int				pGroup::getHP(){
	wrapGET( GetHP() );
};			
int				pGroup::getMP(){
	wrapGET( GetMP() );
};			
void			pGroup::setHP(int HP){
	wrapSET( SetHP(HP) );
};
bool			pGroup::getSearchEnemy(){
	wrapGET( GetSearchWict() );
};	
void			pGroup::setSearchEnemy(bool state){
	wrapSET( SetSearchWict(state) );
};
bool			pGroup::getUnderAttack(){
	wrapGET( GetUA() );
};
void			pGroup::setUnderAttack(bool	state){
	wrapSET( SetUA(state) );
};
int				pGroup::getNO_ORDERS_UN	(){
	wrapGET( NO_ORDERS_NUM() );
};
int				pGroup::getMOVE_UN		(){
	wrapGET( MOVE_NUM() );
};
int				pGroup::getATTACK_UN	(){
	wrapGET( ATTACK_NUM() );
};
int				pGroup::getXc	(){
	wrapGET( GetGroupX() );
};
int				pGroup::getYc	(){
	wrapGET( GetGroupY() );
};
bool			pGroup::getXcYc	(int& xc,int& yc){
	if ( Valid() ){
		group()->GetGroupCenter(xc,yc);
		return true;
	};
	return false;
};
int				pGroup::getDIR	(){
	wrapGET( GetDirection() );
};
int				pGroup::getXi	(int lid){
	wrapGET( GetXi(lid) );
};
int				pGroup::getYi	(int lid){
	wrapGET( GetYi(lid) );
};
int				pGroup::getDIRi	(int lid){
	wrapGET( GetDIRi(lid) );	
};
int				pGroup::getGIDi	(int lid){
	wrapGET( GetGIDi(lid) );
};
int				pGroup::getNation(){
	wrapGET( GetNation() );
};		
void			pGroup::setNation(int NI){
	wrapSET( SetNation(NI) );
};
int				pGroup::getNMASK(){
	wrapGET( GetNMASK() );
};
void			pGroup::setNMASK(DWORD NMASK){
	wrapSET( SetNMASK(NMASK) );
};
int				pGroup::getDestDelta(){
	wrapGET( GetSendDispersion() );
};
void			pGroup::setDestDelta(int DD){
	wrapSET( SetSendDispersion(DD) );
};
bool			pGroup::getAIGuard(){
	wrapGET( getAIGuard() );
};
void			pGroup::setAIGuard(int state){
	wrapSET( setAIGuard(state) );
};
int				pGroup::getLockType(){
	wrapGET( GetLockType() );
};		
void			pGroup::setLockType(int LT){
	wrapSET( SetLockType(LT) );
};
en_vGroup_Order	pGroup::getORDER(){
	if ( Valid() )	return group()->GetORDER();
	return vgNO_ORDERS;
};
en_vGroup_Order	pGroup::getORDERi(int lid){
	if ( Valid() )	return group()->GetUnitORDER(lid);
	return vgNO_ORDERS;
};
char*			pGroup::getName(){
	wrapGET( GetName() );
};
int				pGroup::getNew_UN(bool clearList){
	wrapGET( GetAmountOfNewUnits(clearList) );
};
int				pGroup::getRem_UN(bool clearList){
	wrapGET( GetAmountOfRemUnits(clearList) );
};
int				pGroup::getInside_UN(bool MaxUN){
	wrapGET( GetNInside(MaxUN) );
};
bool			pGroup::getLeaveAbility(){
	wrapGET( GetLeaveAbility() );
};
int				pGroup::getEnemyAround_UN(int r){
	wrapGET( GetEnemyUnitsAroundNumber(r) );
};
bool			pGroup::getAttackPossibility(pGroup& GR){
	wrapGET( CheckAttackPossibilityG(GR.group()) );	
};
bool			pGroup::getAttackPossibility(int x, int y, int r){
	wrapGET( CheckAttackPossibilityXYR(x,y,r) );
};
bool			pGroup::getAttackPossibility(iNode& node,int addR){
	if (Valid()&&node.Valid()) {
		return getAttackPossibility( node.getX(),node.getY(),node.getR()+addR );
	};
	return false;
};
bool			pGroup::getStandGround(){
	wrapGET( GetStandGround() );
};
void			pGroup::setStandGround(bool state){
	wrapSET( SetStandGround(state) );
};
bool			pGroup::getCapturability(){
	wrapGET( GetCapturability() );
};
void			pGroup::setCapturability(bool state){
	if ( Valid() ){
		( (state==true) ? (group()->EnableCapturability()) : (group()->DisableCapturability()) );
	};
};
int				pGroup::getRandomPower(){
	wrapGET( GetRandomApplyPower() );	
};	
void			pGroup::setRandomPower(int power){
	wrapSET( SetRandomApplyPower(power) );	
};
// check functions
bool			pGroup::IsGroupAttackMe(pGroup& GR){
	wrapGET( IsGRPAttackMe(GR.group()) );	
};
// add rem functions
void			pGroup::RemDeadUnits	(){
	wrapSET( RemoveDeadUnits() );	
};
void			pGroup::RemAllUnits		(){
	wrapSET( RemAllUnits() );	
};
int				pGroup::RemUnitsByNation(int NI){
	wrapGET( RemUnitsByNation(NI) );	
};
int				pGroup::RemNUnitsToGroup(pGroup& GR, int N){
	wrapGET( RemoveNUnitsToCGroup(GR.group(),N) );	
};
int				pGroup::RemNReadyUnitsToGroup(pGroup& GR, int N){
	wrapGET( RemoveNReadyUnitsToCGroup(GR.group(),N) );	
};
int				pGroup::RemUnitsInZone	(iNode& ZN, pGroup& GR){
	if ( GR.Valid() && ZN.Valid() ){
		wrapGET( RemoveUnitsInZoneToCGroup(ZN.node(),GR.group()) );
	};
	return 0;
};
int				pGroup::AddSelectedUnits	(int NI){
	wrapGET( AddSelectedUnits(NI) );
};
void			pGroup::AddUnitsInZone		(int NI, iNode& ZN, bool add){
	if ( ZN.Valid() )	AddUnitsInZone(NI,ZN.getX(),ZN.getY(),ZN.getR(),add);
};
void				pGroup::AddUnitsInZone		(int NI, int x, int y, int r, bool add){
	wrapSET( AddUnitsInZone(NI,x,y,r,_add) );
};
void			pGroup::AddUnitsTypeInZone	(int NI, int UT, iNode& ZN, bool add){
	if ( ZN.Valid() )	AddUnitsTypeInZone(NI,UT,ZN.getX(),ZN.getY(),ZN.getR(),add);
};
void			pGroup::AddUnitsTypeInZone	(int NI, int UT, int x, int y, int r, bool add){
	wrapSET( AddUnitsTypeInZone(NI,UT,x,y,r,add) );
};
void			pGroup::AddEnemyUnitsInZone	(int NI, iNode& ZN, bool add){
	if ( ZN.Valid() )	AddEnemyUnitsInZone(NI,ZN.getX(),ZN.getY(),ZN.getR(),add);
};
void			pGroup::AddEnemyUnitsInZone	(int NI, iNode& ZN, int addR, bool add){
	if ( ZN.Valid() )	AddEnemyUnitsInZone(NI,ZN.getX(),ZN.getY(),ZN.getR()+addR,add);	
};
void			pGroup::AddEnemyUnitsInZone	(int NI, int x, int y, int r, bool add){
	wrapSET( AddEnemyUnitsInZone(NI,x,y,r,add) );
};
bool			pGroup::AddUnitGID			(int gid){
	wrapGET( AddUnitGID(gid) );	
};
void			pGroup::AddUnitsFromGroup	(pGroup& GR){
	if ( GR.Valid() )	wrapSET( AddUnitsFromGroup( GR.group() ) );
};
// selections
void			pGroup::Select(bool add){
	if ( Valid() ){
		int NI = getNation();
		if ( add==false ){
			pSelected::units(NI).Clear();
			pImSelected::units(NI).Clear();
		};
		ForEachUnit	(	(*this), 
						pSelected::units(NI)+=unit; 
						pImSelected::units(NI)+=unit; 
					);
	};
};
void			pGroup::Select(iZone& ZN, bool add){
	if ( Valid() ){
		int NI = getNation();
		if ( add==false ){
			pSelected::units(NI).Clear();
			pImSelected::units(NI).Clear();
		};
		ForEachUnit	(	(*this),	
						if(ZN.IsIn(unit)==true)
						{ 
							pSelected::units(NI)+=unit; 
							pImSelected::units(NI)+=unit; 
						} 
					);
	};
};
void			pGroup::Select(int x, int y, int r, bool add){
	if ( Valid() ){
		RoundZone	ZN(x,y,r);
		Select(ZN,add);
	};
};
// commands
void			pGroup::ClearOrders(){
	wrapSET( ClearOrders() );	
};
void			pGroup::KillUnits(int N){
	wrapSET( KillUnits(N) );	
};
void			pGroup::EraseUnits(int N){
	wrapSET( EraseUnits(N) );	
};
void			pGroup::TakeRess(int RID){
	if ( Valid() ){
		ForEachUnit( (*this), unit.TakeResource(RID); );	
	};
};
void			pGroup::TakeFood(){
	TakeRess(3);
};
void			pGroup::TakeWood(){
	TakeRess(0);
};
void			pGroup::TakeStone(){
	TakeRess(2);
};
void			pGroup::Produce(int UT, int UN){
	wrapSET( Produse00(UT,UN) );
};
void			pGroup::Produce(int UT, int UN, pGroup& GR){
	if ( GR.Valid() )	wrapSET( Produse00(UT,UN,GR.group()) );
};
void			pGroup::Attack			(pGroup& GR, int power){
	wrapSET( AttackEnemy00(GR.group(),power) );	
};
void			pGroup::AttackUseIDS	(int OverOrders,bool nearest/*=false*/,int addR/*=0*/){
	wrapSET( idAttackEnemy(OverOrders,nearest,addR) );	
};
void			pGroup::AttackUseIDS	(int OverOrders,bool nearest,int addR,pARRAY& ARR){
	if ( ARR.Valid() ){
		wrapSET( idAttackEnemy(OverOrders,nearest,addR,ARR.array()) );	
	};
};
void			pGroup::AttackUseNearest(pGroup& GR){
	wrapSET( AttackGroup_NearestOB(GR.group()) );	
};
void			pGroup::Send(int x, int y, int dir, int type){
	wrapSET( SendTo(x,y,dir,type) );	
};
void			pGroup::Send(iNode& ZN, int dir, int type){
	Send( ZN.getX(),ZN.getY(),dir,type );	
};
void			pGroup::SendID(int lid, int x, int y, int dir, int type,bool withAttack){
	wrapSET( SendToI(x,y,dir,type,lid,withAttack) );
};
void UnitAbilityTest();
void			pGroup::SendID(int lid, iNode& ZN, int dir, int type,bool withAttack){
	//UnitAbilityTest();
	SendID(lid,ZN.getX(),ZN.getY(),dir,type,withAttack);
	//UnitAbilityTest();
};
void			pGroup::SendID(int x, int y, DWORD OverOrders, int lid, int dir, int type,bool withAttack,bool donotCheckPos){
	wrapSET( idSendTo( x,y,dir,OverOrders,lid,type,withAttack,donotCheckPos ) );
};
void			pGroup::SendID(iNode& ZN, DWORD OverOrders, int lid, int dir, int type,bool withAttack,bool donotCheckPos){
	SendID(ZN.getX(),ZN.getY(),OverOrders,lid,dir,type,withAttack,donotCheckPos);
};
void			pGroup::SendUseDisperse(int x, int y){
	wrapSET( SendToDispersePosition( x, y ) );	
};
void			pGroup::SendUseDisperse(iNode& ZN){
	SendUseDisperse(ZN.getX(),ZN.getY());
};
void			pGroup::ChangeDirection(int dir, int type){
	wrapSET( ChangeDirection( dir, type ) );
};
void			pGroup::MarkTime(int x, int y, int destD, int powerRND){
	wrapSET( MarkTime(x,y,destD,powerRND) );	
};
void			pGroup::MarkTime(iNode& node, int powerRND){
	if ( Valid() && node.Valid() ){
		addrand(node.getX());
		addrand(node.getY());
		addrand(powerRND);
		MarkTime(node.getX(),node.getY(),node.getR(),powerRND);
	}
};
// ability
void			pGroup::DeleteItemDrops(){
	wrapSET( DeleteItemDrops() );
};
void			pGroup::GiveExpa(int ExpaAmount){
	wrapSET( GiveExpa(ExpaAmount) );
};
void			pGroup::SetRallyPoint(int x, int y){
	wrapSET( SetRallyPoint(x,y) );
};
void			pGroup::SetRallyPoint(iNode& ZN){
	SetRallyPoint(ZN.getX(),ZN.getY());
};
// spesial
void			pGroup::SendSmartly(int x, int y, int stepsN, iNode& destZN){
	wrapSET( SendSmartlyTo(x,y,stepsN,destZN.node()) )
};
void			pGroup::SendSmartly(iNode& sZN, int stepsN, iNode& destZN){
	SendSmartly(sZN.getX(),sZN.getY(),stepsN,destZN);
};
void			pGroup::ExploringMap(){
	wrapSET( ExploringMAP() );	
};
bool			pGroup::FillTN_array(pARRAY AR){
	if ( AR.Valid()==false ) return false;
	wrapGET( FillTN_array( AR.array() ) );	
};
bool			pGroup::FillDIFF_TN_array(pARRAY ARRdest,pARRAY ARRsupport,int maxUN){
	if ( ARRdest.Valid()==false || ARRsupport.Valid()==false ) return false;
	wrapGET( FillDIFF_TN_array(ARRdest.array(),ARRsupport.array(),maxUN) );
};
//////////////////////////////////////////////////////////////////////////
// pUnit
//////////////////////////////////////////////////////////////////////////
pUnit pUnit::invalid(){
	static pUnit p(0xFFFF,0xFFFF);
	return p;
}
bool GetHero(OneObject* OB);
HeroVariableStorage* DetectHero(OneObject* OB);

bool	pUnit::isHero() const {
	return GetHero(object());
};
int		pUnit::GetHeroLevel() const {
	HeroVariableStorage* HVS=DetectHero();
	if(HVS){
		return HVS->Level;
	}
	return -1;
};
bool	pUnit::GetHeroCollectExpaStatus() const {
	HeroVariableStorage* HVS=DetectHero();
	if(HVS){
		return HVS->EnableCollectExpa;
	}
	return false;
};
void	pUnit::SetHeroCollectExpaStatus(const bool CollectExpaStatus) {
	HeroVariableStorage* HVS=DetectHero();
	if(HVS){
		HVS->EnableCollectExpa=CollectExpaStatus;
	}
};

void	pUnit::GiveExpa(int amount){
	if ( object() && isHero() )	{
		void GiveExperience(OneObject* OB, int Expa);
		GiveExperience(object(),amount);
	};
};

bool pUnit::isStuned(){
	bool Stun=false;
	OneObject* OB=object();
	if( OB && OB->ActiveAbility ){
		int R=100;
		OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(R,R,Stun);
	}
	return Stun;
};
////

const char* pGroup::Name(){
	be_CGROUP* g=group();
	if(g)return g->GetName();
	else return "";
}
void pGroup::Rename(const char* NewName){
	be_CGROUP* g=group();
	if(g)g->SetName(NewName);
}
void pGroup::AttachOrCreateNew(const char* name){
	AttachTo(name);
	if(!group()){
		be_CGROUP* g=NULL;
		if (name!=NULL&&name[0]!=0) g = new be_CGROUP(name);
		else						g = new be_CGROUP();
		g_beVAR_STORE.GROUP_LIST.Add(g);
		_group.Set(g);
	};
};
void pGroup::CreateLocal(char* Name){
	be_CGROUP* g=new be_CGROUP();
	_str	newName;
	if ( Name!=NULL && Name[0]!=0 ){
		newName = Name;
	}else{
		newName	 = "LOCAL_";
		newName += g->GetName();
	};
	g->SetName(newName.pchar());
	_group.Set(g);
};
bool pGroup::isLocal(){
	if ( group()==NULL ) return false;
	return (bool)( strstr( group()->GetName(), "LOCAL_" )==group()->GetName() );
};
void pGroup::Free(){
	be_CGROUP* g=group();
	if ( g==NULL ) return;
	_group.Set(NULL);
	if ( isLocal()==true ){
		delete g;
	}else{
		for(int i=0;i<g_beVAR_STORE.GROUP_LIST.GetAmount();i++){
			if(g_beVAR_STORE.GROUP_LIST[i]==g){
				g_beVAR_STORE.GROUP_LIST.DelElement(i);
				break;
			};
		};
	};
};
int pGroup::amount(){//amount of pGroups on map
	return g_beVAR_STORE.GROUP_LIST.GetAmount();
}
pGroup pGroup::getgroup(int idx){//getting specific pGroup
	pGroup p;
	be_CGROUP* g=g_beVAR_STORE.GROUP_LIST[idx];
	if(g){
		p._group.Set(g);
	}
	return p;
}
//////pCollectorGroup/////
RawUnit pCollectorGroup::_getunit(int idx){
	RawUnit R;
	ActiveGroup* _group=group();
	if(_group){
		OneUS& u=_group->Units[idx];
		R.ID=u.ID;
		R.SN=u.SN;
	}
	return R;
}
void pCollectorGroup::_setunit(int idx,RawUnit& r){
	ActiveGroup* _group=group();
	if(_group){
		OneUS u(r.ID,r.SN);
		_group->Units[idx]=u;
	}
}
int	 pCollectorGroup::_getamount(){
	ActiveGroup* _group=group();
	if(_group){
		return _group->Units.GetAmount();
	}	
	return 0;
}
void pCollectorGroup::_add(RawUnit& r){
	ActiveGroup* _group=group();
	if(_group){
		OneUS u(r.ID,r.SN);
		_group->Units.Add(u);
	}
}
void pCollectorGroup::_del(int pos,int n){
	ActiveGroup* _group=group();
	if(_group){		
		_group->Units.Del(pos,n);
	}
}
ActiveGroup* pCollectorGroup::group(){
	return _group.Get();
}
pCollectorGroup::pCollectorGroup(){	
}
pCollectorGroup::pCollectorGroup(const char* Name){
	AttachTo(Name);
}
void pCollectorGroup::AttachTo(const char* GroupName){
	_group.SetObjectName((char*)GroupName);
}
bool pCollectorGroup::Valid(){
	return group()!=NULL;
}
const char* pCollectorGroup::Name(){
	ActiveGroup* g=group();
	if(g)return g->Name.pchar();
	else return "";
}
void pCollectorGroup::Rename(const char* NewName){
	ActiveGroup* g=group();
	if(g)g->Name=NewName;
}
void pCollectorGroup::AttachOrCreateNew(const char* name){
	AttachTo(name);
	if(!Valid()){
		ActiveGroup* g=new ActiveGroup;
		g->Name=name;
		AGroups.Add(g);
	}
}
void pCollectorGroup::CreateUniq(){
	ActiveGroup* g=new ActiveGroup;
	g->Name="ActiveGroup_";
	LARGE_INTEGER L;
	QueryPerformanceCounter(&L);
	g->Name+=abs(int(L.LowPart));
	AGroups.Add(g);
}
void pCollectorGroup::Free(){
	ActiveGroup* g=group();
	if(g){
		for(int i=0;i<AGroups.GetAmount();i++){
			if(AGroups[i]==g){
				AGroups.DelElement(i);
				_group.Set(NULL);
				break;
			}
		}
	}
}
int pCollectorGroup::GetCollectorIndex(){
	ActiveGroup* g=group();
	if(g){
		for(int i=0;i<AGroups.GetAmount();i++){
			if(AGroups[i]==g)return i;
		}
	}
	return 0xFFFF;
}
int pCollectorGroup::amount(){//amount of pCollectorGroup in game
	return AGroups.GetAmount();
}
pCollectorGroup pCollectorGroup::getgroup(int idx){//getting specific pCollectorGroup
	pCollectorGroup p;
	p._group.Set(AGroups[idx]);
	return p;
}
//////pSelected/////
RawUnit pSelected::_getunit(int idx){
	xassert(NI>=0 && NI<8);
	xassert(idx>=0 && idx<NSL[NI]);
	RawUnit r(Selm[NI][idx],SerN[NI][idx]);
	return r;
}
void pSelected::_setunit(int idx,RawUnit& r){
	xassert(NI>=0 && NI<8);
	xassert(idx>=0 && idx<NSL[NI]);
	Selm[NI][idx]=r.ID;
	SerN[NI][idx]=r.SN;
	OneObject* OB=r.object();
	if(OB)OB->Selected|=1<<NI;
}
int pSelected::_getamount(){
	xassert(NI>=0 && NI<8);
	return NSL[NI];
}
void pSelected::_add(RawUnit& r){
	xassert(NI>=0 && NI<8);
    OneObject* ob=r.object();
	if( ob && (ob->Selected&(1<<NI)) )
		return;
	int n=NSL[NI];
	_addnelem(1);
	_setunit(n,r);
}
void pSelected::_del(int pos,int n){
	xassert(NI>=0 && NI<8);
	for(int i=0;i<n;i++){
		RawUnit r=_getunit(pos+i);
		OneObject* OB=r.object();
		if(OB)OB->Selected&=~(1<<NI);
	}
	if(pos==0 && n==NSL[NI]){
		if(n){
			free(Selm[NI]);
			free(SerN[NI]);
			Selm[NI]=NULL;
			SerN[NI]=NULL;
			NSL[NI]=0;
		}
	}else{
		int sz=(NSL[NI]-pos-n+1);
		assert(sz>=0 && sz<NSL[NI]);
		if(sz){
			memcpy(Selm[NI]+pos,Selm[NI]+pos+n,sz<<1);
			memcpy(SerN[NI]+pos,SerN[NI]+pos+n,sz<<1);
		}
		NSL[NI]-=n;
	}
}
void pSelected::_addnelem(int n){
	xassert(NI>=0 && NI<8);
	Selm[NI]=(word*)realloc(Selm[NI],(NSL[NI]+n)<<1);
	SerN[NI]=(word*)realloc(SerN[NI],(NSL[NI]+n)<<1);
	int n0=NSL[NI];	
	for(int i=0;i<n;i++){
		Selm[NI][n0+i]=0xFFFF;
		SerN[NI][n0+i]=0xFFFF;
	}
	NSL[NI]+=n;
}
pSelected& pSelected::units(int NI){
	static pSelected sel[8]={pSelected(0),pSelected(1),pSelected(2),pSelected(3),pSelected(4),pSelected(5),pSelected(6),pSelected(7)};
	xassert(NI>=0 && NI<8);
	return sel[NI];
}
//////pImSelected/////
RawUnit pImSelected::_getunit(int idx){
	xassert(NI>=0 && NI<8);
	xassert(idx>=0 && idx<ImNSL[NI]);
	RawUnit r(ImSelm[NI][idx],ImSerN[NI][idx]);
	return r;
}
void pImSelected::_setunit(int idx,RawUnit& r){
	xassert(NI>=0 && NI<8);
	xassert(idx>=0 && idx<ImNSL[NI]);
	ImSelm[NI][idx]=r.ID;
	ImSerN[NI][idx]=r.SN;
	OneObject* OB=r.object();
	if(OB)OB->ImSelected|=1<<NI;
}
int pImSelected::_getamount(){
	xassert(NI>=0 && NI<8);
	return ImNSL[NI];
}
void pImSelected::_add(RawUnit& r){
	xassert(NI>=0 && NI<8);
	OneObject* ob=r.object();
	if( ob && (ob->ImSelected&(1<<NI)) )
		return;
	int n=ImNSL[NI];
	_addnelem(1);
	_setunit(n,r);
}
void pImSelected::_del(int pos,int n){
	xassert(NI>=0 && NI<8);
	for(int i=0;i<n;i++){
		RawUnit r=_getunit(pos+i);
		OneObject* OB=r.object();
		if(OB)OB->ImSelected&=~(1<<NI);
	}
	if(pos==0 && n==ImNSL[NI]){
		if(n){
			free(ImSelm[NI]);
			free(ImSerN[NI]);
			ImSelm[NI]=NULL;
			ImSerN[NI]=NULL;
			ImNSL[NI]=0;
		}
	}else{
		int sz=(ImNSL[NI]-pos-n+1);
		assert(sz>=0 && sz<ImNSL[NI]);
		if(sz){
			memcpy(ImSelm[NI]+pos,ImSelm[NI]+pos+n,sz<<1);
			memcpy(ImSerN[NI]+pos,ImSerN[NI]+pos+n,sz<<1);
		}
		ImNSL[NI]-=n;
	}
}
void pImSelected::_addnelem(int n){
	xassert(NI>=0 && NI<8);
	ImSelm[NI]=(word*)realloc(ImSelm[NI],(ImNSL[NI]+n)<<1);
	ImSerN[NI]=(word*)realloc(ImSerN[NI],(ImNSL[NI]+n)<<1);
	int n0=ImNSL[NI];	
	for(int i=0;i<n;i++){
		ImSelm[NI][n0+i]=0xFFFF;
		ImSerN[NI][n0+i]=0xFFFF;
	}
	ImNSL[NI]+=n;
}
pImSelected& pImSelected::units(int NI){
	static pImSelected sel[8]={pImSelected(0),pImSelected(1),pImSelected(2),pImSelected(3),pImSelected(4),pImSelected(5),pImSelected(6),pImSelected(7)};
	xassert(NI>=0 && NI<8);
	return sel[NI];
}
//////pBrigade/////
Brigade* pBrigade::Brig(){
	xassert(NI>=0 && NI<8);
	xassert(Index>=0 && Index<512);
	return CITY[NI].Brigs+Index;
}
pBrigade::pBrigade(){
	NI=0;
	Index=0;
}
pBrigade::pBrigade(byte _NI,int _Index){
	NI=_NI;
	Index=_Index;
	xassert(NI>=0 && NI<8);
	xassert(Index>=0 && Index<512);
}
RawUnit pBrigade::_getunit(int idx){
	Brigade* B=Brig();
	xassert(idx>=0 && idx<B->NMemb);
	RawUnit r(B->Memb[idx],B->MembSN[idx]);
	return r;
}
void pBrigade::_setunit(int idx,RawUnit& r){
	Brigade* B=Brig();
	xassert(idx>=0 && idx<B->NMemb);
	B->Memb[idx]=r.ID;
	B->MembSN[idx]=r.SN;
}
int pBrigade::_getamount(){
	Brigade* B=Brig();
	return B->NMemb;
}
void pBrigade::_add(RawUnit& r){
	int n=ImNSL[NI];
	_addnelem(1);
	_setunit(n,r);
}
void pBrigade::_del(int pos,int n){
	Brigade* B=Brig();
	if(pos==0 && n==B->NMemb){
		free(B->Memb);
		free(B->MembSN);
		B->NMemb=0;
		B->MaxMemb=0;
	}else{
		int sz=B->NMemb-pos-n+1;
		if(sz){
			memcpy(B->Memb+pos,B->Memb+pos+n,sz<<1);
			memcpy(B->MembSN+pos,B->MembSN+pos+n,sz<<1);			
		}
		B->NMemb-=n;
	}
}
void pBrigade::_addnelem(int n){
	Brigade* B=Brig();
	if(B->NMemb+n>B->MaxMemb){
		B->MaxMemb=B->NMemb+n;
		B->Memb=(word*)realloc(B->Memb,B->MaxMemb<<1);
		B->MembSN=(word*)realloc(B->MembSN,B->MaxMemb<<1);
	}
	int n0=B->NMemb;
	for(int i=0;i<n;i++){
		B->Memb[n0+i]=0xFFFF;
		B->MembSN[n0+i]=0xFFFF;
	}
	B->NMemb+=n;
}
//////IUnitsIterator/////
void IUnitsIterator::_addnelem(int n){
	RawUnit r(0xFFFF,0xFFFF);
	for(int i=0;i<n;i++)_add(r);
}
OneObject* IUnitsIterator::object(int pos){
	RawUnit U=_getunit(pos);
	return U.object();
}
pUnit IUnitsIterator::unit(int Pos){
	RawUnit U=_getunit(Pos);
	pUnit p(U.ID,U.SN);
	return p;
}
void IUnitsIterator::ToBegin(int* pos){
	if(pos)*pos=0;
	else curpos=0;
}
void IUnitsIterator::ToN(int pos){
	curpos=pos;
	xassert(pos>=0 && pos<_getamount());
}
bool IUnitsIterator::Next(pUnit& u,int* pos){
	int n=_getamount();
	int& _pos=pos?*pos:curpos;
	while(_pos<n){		
		u=unit(_pos++);
		if(u.object())return true;
	};
	return false;
}
bool IUnitsIterator::NextLive(pUnit& u,int* pos){
	int n=_getamount();
	int& _pos=pos?*pos:curpos;
	while(_pos<n){		
		u=unit(_pos++);
		OneObject* OB=u.object();
		if(OB && !OB->Sdoxlo)return true;
	};
	return false;
}
OneObject* IUnitsIterator::Next(int* pos){
	int n=_getamount();
	int& _pos=pos?*pos:curpos;
	while(_pos<n){				
		OneObject* OB=object(_pos++);
		if(OB)return OB;
	};
	return NULL;
}
OneObject* IUnitsIterator::NextLive(int* pos){
	int n=_getamount();
	int& _pos=pos?*pos:curpos;
	while(_pos<n){				
		OneObject* OB=object(_pos++);
		if(OB && !OB->Sdoxlo)return OB;
	};
	return NULL;
}
void IUnitsIterator::DelCurrent(int* pos){
	int& _pos=pos?*pos:curpos;
	assert(_pos>0 && _pos<=_getamount());
	_setunit(_pos-1,RawUnit(0xFFFF,0xFFFF));
}
bool IUnitsIterator::HaveUnits(){
	RemoveFinallyDeadUnits();
	return _getamount()!=0;
}
bool IUnitsIterator::HaveLiveUnits(){
	int n=_getamount();
	for(int i=0;i<n;i++){
		RawUnit r=_getunit(i);
		OneObject* OB=r.object();
		if(OB && !OB->Sdoxlo)return true;
	}
	return false;
}
void IUnitsIterator::RemoveDeadUnits(){
	int n=_getamount();
	int c=0;
	for(int i=0;i<n;i++){
		RawUnit u=_getunit(i);
		OneObject* OB=u.object();
		if(OB && !OB->Sdoxlo){
			_setunit(c++,u);
		}
	}
	if(n>c)_del(c,n-c);
}
void IUnitsIterator::RemoveFinallyDeadUnits(){
	int n=_getamount();
	int c=0;
	for(int i=0;i<n;i++){
		RawUnit u=_getunit(i);
		OneObject* OB=u.object();
		if(OB)_setunit(c++,u);
	}
	if(n>c)_del(c,n-c);
}
void IUnitsIterator::Clear(){
	int n=_getamount();
	if(n)_del(0,n);
}
int IUnitsIterator::GetAmount(){
	return _getamount();
}

pUnit IUnitsIterator::operator [] (int pos){
	RawUnit U=_getunit(pos);
	pUnit p(U.ID,U.SN);
	return p;
}
IUnitsIterator& IUnitsIterator::operator += (IUnitsIterator& op){
	int N=_getamount();
	for(int i=0;i<N;i++){
		RawUnit r=_getunit(i);
		OneObject* OB=r.object();
		if(OB)OB->TempFlag=1;
	}
	int M=op._getamount();
	int na=0;
	for(int i=0;i<M;i++){
		RawUnit r=op._getunit(i);
		OneObject* OB=r.object();
		if(!(OB)||(OB&&!OB->TempFlag)){
			na++;
		}
	}
	int sz=_getamount();
	_addnelem(na);	
	na=0;
	for(int i=0;i<M;i++){
		RawUnit r=op._getunit(i);
		OneObject* OB=r.object();
		if(!(OB) || (OB && !OB->TempFlag)){
			_setunit(sz+na,r);
			na++;
		}
	}
	for(int i=0;i<N;i++){
		RawUnit r=_getunit(i);
		OneObject* OB=r.object();
		if(OB)OB->TempFlag=0;
	}
	RemoveFinallyDeadUnits();
	return *this;
}
IUnitsIterator& IUnitsIterator::operator -= (IUnitsIterator& op){
	int N=op._getamount();
	for(int i=0;i<N;i++){
		RawUnit r=op._getunit(i);
		OneObject* OB=r.object();
		if(OB)OB->TempFlag=1;
	}
	int M=_getamount();
	for(int i=0;i<M;i++){
		RawUnit r=_getunit(i);
		OneObject* OB=r.object();
		if (OB && OB->TempFlag){
			_setunit(i,RawUnit(0xFFFF,0xFFFF));
		}
	}
	for(int i=0;i<N;i++){
		RawUnit r=op._getunit(i);
		OneObject* OB=r.object();
		if(OB)OB->TempFlag=0;
	}
	RemoveFinallyDeadUnits();
	return *this;
}
IUnitsIterator& IUnitsIterator::operator &= (IUnitsIterator& op){
	int N=op._getamount();
	for(int i=0;i<N;i++){
		RawUnit r=op._getunit(i);
		OneObject* OB=r.object();
		if(OB)OB->TempFlag=1;
	}
	int M=_getamount();
	for(int i=0;i<N;i++){
		RawUnit r=_getunit(i);
		OneObject* OB=r.object();
		if (!(OB) || (OB && !OB->TempFlag)) {
			_setunit(i,RawUnit(0xFFFF,0xFFFF));
		}
	}
	for(int i=0;i<N;i++){
		RawUnit r=op._getunit(i);
		OneObject* OB=r.object();
		if(OB)OB->TempFlag=0;
	}
	RemoveFinallyDeadUnits();
	return *this;
}
void IUnitsIterator::Assign(IUnitsIterator& op){
	Clear();
	int n=op._getamount();
	_addnelem(n);
	for(int i=0;i<n;i++){
		RawUnit r=op._getunit(i);
		_setunit(i,r);
	}
}


//void				CreateFromZone(pZone& Zone,bool Add=false);
void IUnitsIterator::CreateFromZone(int x,int y,int R,bool Add){
	if(!Add)Clear();
	itr_UnitsInRadius.Create(x,y,R);
	while(OneObject* OB=itr_UnitsInRadius.Next()){
		RawUnit r(OB->Index,OB->Serial);
		_add(r);
	}
}
void IUnitsIterator::CreateFromSquare(int xmin,int ymin,int xmax,int ymax,bool Add){
	if(!Add)Clear();
	itr_GetUnitsInSquare.Create(xmin,ymin,xmax,ymax);
	while(OneObject* OB=itr_GetUnitsInSquare.Next()){
		RawUnit r(OB->Index,OB->Serial);
		_add(r);
	}
}
void IUnitsIterator::CreateFromAreaID(int Layer,int AreaID,collect_option Flags,bool Add){
	if(!Add)Clear();
	if ( Layer<0 || Layer>3 ) return;
	Area* pArea = HashTable[Layer].GetTopArea(AreaID);
	if ( pArea!=NULL ){
		int R = GameInterface::GetTZoneApproxRadius(Layer,AreaID);
		if ( Flags&COLLECT_UNITS ){
			itr_UnitsInRadius.Create(pArea->x*64,pArea->y*64,R+128);
			while(OneObject* OB=itr_UnitsInRadius.Next()){
				if ( AreaID == HashTable[Layer].GetTopAreaIndex( OB->RealX/1024, OB->RealY/1024 ) ){
					RawUnit r(OB->Index,OB->Serial);
					_add(r);
				};
			};
		};
		if ( Flags&COLLECT_BUILDINGS ){
			itr_GetBuildingsInRadius.Create(pArea->x*64,pArea->y*64,R+128);
			while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
				if ( AreaID == HashTable[Layer].GetTopAreaIndex( OB->RealX/1024, OB->RealY/1024 ) ){
					RawUnit r(OB->Index,OB->Serial);
					_add(r);
				};
			};
		};
	};
};
//limitation
void IUnitsIterator::LimitByNation(int NI){
	ToBegin();
	while(OneObject* OB=Next()){
		if(OB->NNUM!=NI)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
void IUnitsIterator::LimitByNMask(DWORD NMask){
	ToBegin();
	while(OneObject* OB=Next()){
		if(!(OB->NMask&NMask))DelCurrent();
	}
	RemoveFinallyDeadUnits();
};
void IUnitsIterator::LimitByType(int Type){
	ToBegin();
	while(OneObject* OB=Next()){
		if(OB->NIndex!=Type)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
void IUnitsIterator::LimitByType(pUnitType& Type){
	LimitByType(Type.GetType());
}
void IUnitsIterator::LimitByRound(int x,int y,int R){
	ToBegin();
	while(OneObject* OB=Next()){
		int r=Norma(x-(OB->RealX>>4),y-(OB->RealY>>4));
		if(r>R)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
void IUnitsIterator::LimitBySquare(int xmin,int ymin,int xmax,int ymax){
	ToBegin();
	while(OneObject* OB=Next()){
		int x=OB->RealX>>4;
		int y=OB->RealY>>4;
		if(!(x>=xmin && x<=xmax && y>=ymin && y<=ymax))DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
void IUnitsIterator::LimitByTZone(int Layer,int TZone){
	xassert(Layer>=0 && Layer<NMFIELDS);
	ToBegin();
	while(OneObject* OB=Next()){
		int x=OB->RealX>>10;
		int y=OB->RealY>>10;
		if(HashTable[Layer].GetTopAreaIndex(x,y)!=TZone)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
//excluding
void IUnitsIterator::ExcludeByNation(int NI){
	ToBegin();
	while(OneObject* OB=Next()){
		if(OB->NNUM==NI)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
void IUnitsIterator::ExcludeByNMask(DWORD NMask){
	ToBegin();
	while(OneObject* OB=Next()){
		if(OB->NMask&NMask)DelCurrent();
	}
	RemoveFinallyDeadUnits();
};
void IUnitsIterator::ExcludeByType(int Type){
	ToBegin();
	while(OneObject* OB=Next()){
		if(OB->NIndex==Type)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
void IUnitsIterator::ExcludeByType(pUnitType& Type){
	ExcludeByType(Type.GetType());
}
void IUnitsIterator::ExcludeByRound(int x,int y,int R){
	ToBegin();
	while(OneObject* OB=Next()){
		int r=Norma(x-(OB->RealX>>4),y-(OB->RealY>>4));
		if(r<=R)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
void IUnitsIterator::ExcludeBySquare(int xmin,int ymin,int xmax,int ymax){
	ToBegin();
	while(OneObject* OB=Next()){
		int x=OB->RealX>>4;
		int y=OB->RealY>>4;
		if(x>=xmin && x<=xmax && y>=ymin && y<=ymax)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
void IUnitsIterator::ExcludeByTZone(int Layer,int TZone){
	xassert(Layer>=0 && Layer<NMFIELDS);
	ToBegin();
	while(OneObject* OB=Next()){
		int x=OB->RealX>>10;
		int y=OB->RealY>>10;
		if(HashTable[Layer].GetTopAreaIndex(x,y)==TZone)DelCurrent();
	}
	RemoveFinallyDeadUnits();
}
IUnitsIterator& IUnitsIterator::operator += (pUnit& op){
	_add(op);
	return *this;
}
IUnitsIterator& IUnitsIterator::operator -= (pUnit& op){
	int pos=_find(op);
	if( pos!=-1 )
		_del(pos,1);
	return *this;
}
void ExGroupSendSelectedTo(byte NI,word* SMon,word* MSN,int Nsel,int x,int y,int DF,int DR,byte Prio,byte OrdType);

void IUnitsIterator::SendGroupTo(int x,int y,word Direction,bool WithAttack,int type){
	x<<=4;
	y<<=4;
	word* ids=new word[GetAmount()];
	word* sns=new word[GetAmount()];
	int n=0;
	ToBegin();
	while(OneObject* OB=NextLive()){
		ids[n]=OB->Index;
		sns[n]=OB->Serial;
		n++;
	}
	assert(n<=GetAmount());

    extern short LastDirection;
    LastDirection = Direction;
	ExGroupSendSelectedTo(0,ids,sns,n,x,y,0,0,WithAttack?128:128+16,type);
    LastDirection=512;

	delete[]ids;
	delete[]sns;
}
void IUnitsIterator::OrderToAttackEnemyUnitsInZone(iZone& Zone,bool AttackBuildingsAlso){
	pUnitsList L;
	Zone.CollectUnits(L,AttackBuildingsAlso?COLLECT_ALL:COLLECT_UNITS);
	if(L._getamount()){
		ToBegin();
		pUnit U;
		while(NextLive(U)){
			if(!U.IsAttacking()){
				//searching for victim				
				iVector2D range=U.GetAttackRange();
				int W=10000000;
                L.ToBegin();
				pUnit en;
				pUnit besten;
				while(L.NextLive(en)){
					if((!en.IsBuilding()) && U.CheckAttackPossibility(en)){
						int r=en.DistTo(U);
						int cW=0;
						if(r>=range.Min && r<=range.Max){
							cW=r;
						}else{
							cW=10000+r;
						}
						if(cW<W){
                            W=cW;
							besten=en;
						}
					}				
				}
				if(besten.Valid()){
					U.AttackUnit(en,0,low_prio);
				}else{
					while(L.NextLive(en)){
						if(en.IsBuilding() && U.CheckAttackPossibility(en)){
							int r=en.DistTo(U);
							int cW=0;
							if(r>=range.Min && r<=range.Max){
								cW=r;
							}else{
								cW=10000+r;
							}
							if(cW<W){
								W=cW;
								besten=en;
							}
						}				
					}
					if(besten.Valid()){
						U.AttackUnit(en,0,low_prio);
					}
				}
			}else{
				pUnit vic=U.GetCurrentVictim();
				if(!Zone.IsIn(vic)){
					U.ClearOrders();
				}else{
					if(vic.IsBuilding() && GameInterface::random()<512){
						U.ClearOrders();
					}
				}
			}
		}
	}
}
void IUnitsIterator::MoveUnitsOnOneStepToPosition(int x,int y,bool WithAttack){
}
///////////////////////////////////////////////////////////
/////////////////ZONES MANAGEMENT//////////////////////////
///////////////////////////////////////////////////////////
void iZone::CollectUnits(IUnitsIterator& it,collect_option Flag){
	iRect r=GetDimensions();
	if(Flag & COLLECT_UNITS){
		itr_GetUnitsInSquare.Create(r.xmin,r.ymin,r.xmax,r.ymax);
		while(OneObject* OB=itr_GetUnitsInSquare.Next()){
			RawUnit ru(OB->Index,OB->Serial);
			if(CheckIfInside(OB->RealX/16,OB->RealY/16,OB->RZ))it._add(ru);
		}
	}
	if(Flag & COLLECT_BUILDINGS){
		itr_GetBuildingsInSquare.Create(r.xmin,r.ymin,r.xmax,r.ymax);
		while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
			RawUnit ru(OB->Index,OB->Serial);
			if(CheckIfInside(OB->RealX/16,OB->RealY/16,OB->RZ))it._add(ru);
		}
	}
	if(Flag & COLLECT_MINES){
		itr_GetBuildingsInSquare.Create(r.xmin,r.ymin,r.xmax,r.ymax);
		while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
			if( OB->Usage==MineID ){
				RawUnit ru(OB->Index,OB->Serial);
				if(CheckIfInside(OB->RealX/16,OB->RealY/16,OB->RZ))it._add(ru);
			}
		}
	}
}
bool iZone::IsIn(pUnit& unit){
	iVector3D v=unit.GetCoor3D();
	return CheckIfInside(v.x,v.y,v.z);
}
//////////////////////////////////////////////////////////////////////////
// iNode
//////////////////////////////////////////////////////////////////////////
iNode::~iNode(){
	_node.Set(NULL);
	return;	
};
iNode::iNode(){
	_node.Set(NULL);
};
iNode::iNode(iNode& ND){
	(*this)=ND;
};
iNode::iNode(be_CNODE* pND){
	if ( pND!=NULL ){
		_node.Set( pND );
	};	
};
be_CNODE*	iNode::node(){
	return _node.Get();
};
bool		iNode::Valid(){
	return ( node()!=NULL );
};
bool		iNode::CheckIfInside(int x,int y,int z){
	be_CNODE* pND = node();
	if ( pND!=NULL ) return Norma(x-pND->x,y-pND->y)<pND->R;
	return iZone::CheckIfInside(x,y,z);
};
iRect		iNode::GetDimensions(){
	be_CNODE* pND = node();
	if ( pND!=NULL ){
		__g::g_refRECT.set(pND->x-pND->R,pND->y-pND->R,pND->x+pND->R,pND->y+pND->R);
		return __g::g_refRECT;
	}
	return iZone::GetDimensions();
};
void UnitAbilityTest();
iNode&		iNode::operator=(iNode& ND){
	if ( this!=&ND && ND.node()!=NULL ){
		//UnitAbilityTest();
		_node.Set( ND.node() );
		//UnitAbilityTest();
	};
	return *this;	
};
void		iNode::CollectUnits(IUnitsIterator& it,collect_option Flags){
	RoundZone RZ( *this );
	RZ.CollectUnits( it, Flags );
};
iNode&		iNode::set(be_CNODE* pND){
	_node.Set(pND);
	return (*this);
};
int			iNode::getX(){
	if ( node()!=NULL )	return node()->vGetX();
	return -1;
};
int			iNode::getY(){
	if ( node()!=NULL )	return node()->vGetY();
	return -1;
};
int			iNode::getR(){
	if ( node()!=NULL )	return node()->vGetR();
	return -1;
};
int			iNode::getDIR(){
	if ( node()!=NULL )	return node()->vGetDir();
	return -1;
};
int			iNode::getSFR(){
	if ( node()!=NULL )	return node()->vGetSegmFR();
	return -1;
};
void		iNode::setX(int x){
	if ( node()!=NULL )	node()->vSetX(x);
};
void		iNode::setY(int y){
	if ( node()!=NULL )	node()->vSetY(y);
};
void		iNode::setR(int R){
	if ( node()!=NULL )	node()->vSetR(R);
};
void		iNode::setDIR(int DIR){
	if ( node()!=NULL )	node()->vSetDir(DIR);
};
void		iNode::setSFR(int SFR){
	if ( node()!=NULL )	node()->vSetSegmFR(SFR);
};
char*		iNode::getName(){
	if ( node()!=NULL )	return node()->GetName();
	return NULL;
};
void		iNode::setName(char* Name){
	if ( node()!=NULL )	node()->SetName(Name);
};
//////////////////////////////////////////////////////////////////////////
// RoundZone
//////////////////////////////////////////////////////////////////////////
RoundZone::RoundZone(iNode& ND) {
	if ( ND.Valid() ){
		x = ND.getX();
		y = ND.getY();
		r = ND.getR();
	};
};
bool RoundZone::CheckIfInside(int _x,int _y,int _z){
	return Norma(x-_x,y-_y)<r;
}
iRect RoundZone::GetDimensions(){
	return iRect(x-r,y-r,x+r,y+r);
}
void RoundZone::CollectUnits(IUnitsIterator& it,collect_option Flag){	
	if(Flag & COLLECT_UNITS){
		itr_UnitsInRadius.Create(x,y,r);
		while(OneObject* OB=itr_UnitsInRadius.Next()){
			RawUnit ru(OB->Index,OB->Serial);
			it._add(ru);
		}        
	}
	if(Flag & COLLECT_BUILDINGS){
		itr_GetBuildingsInRadius.Create(x,y,r);
		while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
			RawUnit ru(OB->Index,OB->Serial);
			it._add(ru);
		}        
	}
	if(Flag & COLLECT_MINES){
		itr_GetBuildingsInRadius.Create(x,y,r);
		while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
			if( OB->Usage==MineID ){
				RawUnit ru(OB->Index,OB->Serial);
				it._add(ru);
			}
		}        
	}
}
RoundZone::RoundZone(){
	x=y=r=0;
}
RoundZone::RoundZone(const char* Name){	
	AttachTo(Name);
}
RoundZone::RoundZone(int _x,int _y,int _r){
	x=_x;y=_y;r=_r;
}
void RoundZone::AttachTo(const char* Name){
	ClassRef<be_CNODE> Node;
	Node.SetObjectName((char*)Name);
	be_CNODE* N=Node.Get();
	if(N){
		x=N->x;
		y=N->y;
		r=N->R;
	}
}
RoundZone& RoundZone::operator = (SquareZone& zone){
	iRect& rz=zone.GetRect();
	x=(rz.xmin+rz.xmax)/2;
	y=(rz.ymin+rz.ymax)/2;
	r=(rz.xmax-rz.xmin)/2;
	return *this;
}
RoundZone& RoundZone::operator = (RoundZone& zone){
	x=zone.x;
	y=zone.y;
	r=zone.r;
	return *this;
}
void RoundZone::SetXYR(int _x,int _y,int _r){
	x=_x;y=_y;r=_r;
}
iVector2D RoundZone::GetXY(){
	return iVector2D(x,y);
}
int RoundZone::GetR(){
	return r;
}
implement_class(RoundZone){
	REG_PARENT(iZone);
	REG_MEMBER(_int,x);
	REG_MEMBER(_int,y);
	REG_MEMBER(_int,r);
}implement_end;
/////SquareZone/////
SquareZone::SquareZone(iNode& ND) {
	if ( ND.Valid() ){
		r.xmin = ND.getX() - ND.getR()/2;
		r.xmax = ND.getX() + ND.getR()/2;
		r.ymin = ND.getY() - ND.getR()/2;
		r.ymax = ND.getY() + ND.getR()/2;
	};
};
bool SquareZone::CheckIfInside(int _x,int _y,int _z){
	return _x>=r.xmin && _x<=r.xmax && _y>=r.ymin && _y<=r.ymax;
}
iRect SquareZone::GetDimensions(){
	return r;
}
void SquareZone::CollectUnits(IUnitsIterator& it,collect_option Flag){
	if(Flag & COLLECT_UNITS){
		itr_GetUnitsInSquare.Create(r.xmin,r.ymin,r.xmax,r.ymax);
		while(OneObject* OB=itr_GetUnitsInSquare.Next()){
			it._add(RawUnit(OB->Index,OB->Serial));
		}
	}
	if(Flag & COLLECT_BUILDINGS){
		itr_GetBuildingsInSquare.Create(r.xmin,r.ymin,r.xmax,r.ymax);
		while(OneObject* OB=itr_GetBuildingsInSquare.Next()){
			it._add(RawUnit(OB->Index,OB->Serial));
		}
	}
	if(Flag & COLLECT_MINES){
		itr_GetBuildingsInSquare.Create(r.xmin,r.ymin,r.xmax,r.ymax);
		while(OneObject* OB=itr_GetBuildingsInSquare.Next()){
			if( OB->Usage==MineID ){
				it._add(RawUnit(OB->Index,OB->Serial));
			}
		}
	}
}
SquareZone::SquareZone(){
	r=iRect(0,0,0,0);
}
SquareZone::SquareZone(const char* Name){
	AttachTo(Name);
}
SquareZone::SquareZone(iRect& rect){
}
void SquareZone::AttachTo(const char* Name){
	ClassRef<be_CNODE> Node;
	Node.SetObjectName((char*)Name);
	be_CNODE* N=Node.Get();
	int R=N->R;
	r=iRect(N->x-R,N->y-R,N->x+R,N->y+R);
}
SquareZone& SquareZone::operator = (const char* Name){
	AttachTo(Name);
	return *this;
}
SquareZone& SquareZone::operator = (SquareZone& zone){
	r=zone.r;
	return *this;
}
SquareZone& SquareZone::operator = (RoundZone& zone){	
	r=iRect(zone.x-zone.r,zone.y-zone.r,zone.x+zone.r,zone.y+zone.r);
	return *this;
}
void SquareZone::SetRect(iRect& rect){
	r=rect;
}
iRect SquareZone::GetRect(){
	return r;
}
implement_class(SquareZone){
	REG_PARENT(iZone);
	REG_MEMBER(_int,r.xmin);
	REG_MEMBER(_int,r.ymin);
	REG_MEMBER(_int,r.xmax);
	REG_MEMBER(_int,r.ymax);
}implement_end;
/////MultiZone/////
bool MultiZone::CheckIfInside(int x,int y,int z){
	for(int i=0;i<zones.GetAmount();i++){
		if(zones[i]->CheckIfInside(x,y,z))return true;
	}
	return false;
}
iRect MultiZone::GetDimensions(){
	iRect D(100000,100000,0,0);
	for(int i=0;i<zones.GetAmount();i++){
		iRect d0=zones[i]->GetDimensions();
		D.xmin=tmin(D.xmin,d0.xmin);
		D.ymin=tmin(D.ymin,d0.ymin);
		D.xmax=tmax(D.xmax,d0.xmax);
		D.ymax=tmax(D.ymax,d0.ymax);
	}
	return D;
}
void MultiZone::CollectUnits(IUnitsIterator& it,collect_option Flag){
	for(int i=0;i<zones.GetAmount();i++){
		pUnitsList UL;
		zones[i]->CollectUnits(UL,Flag);
		it+=UL;
	}
}
MultiZone::MultiZone(){
}
MultiZone& MultiZone::operator += (const char* name){
	RoundZone* r=new RoundZone;
	r->AttachTo(name);
	if(r->x!=0 || r->y!=0 || r->r!=0){
		zones.Add(r);
	}else delete(r);
	return *this;
}
MultiZone& MultiZone::operator += (iZone& zone){
	iZone* z=(iZone*)zone.new_element();
	zone.Copy(z,false);
	zones.Add(z);
	return *this;
}
MultiZone& MultiZone::operator = (RoundZone& z){
	if(z.r>0){
		RoundZone* rz=new RoundZone;
		*rz=z;
		zones.Add(rz);
	}
	return *this;
}
MultiZone& MultiZone::operator = (SquareZone& z){
	if(z.r.xmax>z.r.xmin){
		SquareZone* sz=new SquareZone;
		*sz=z;
		zones.Add(sz);
	}
	return *this;
}
MultiZone& MultiZone::operator = (MultiZone& z){
	z.Copy(this,false);
	return *this;
}
MultiZone& MultiZone::operator = (const char* name){
	zones.Clear();
	(*this)+=name;
	return *this;
}
implement_class(MultiZone){
	REG_PARENT(iZone);
	REG_AUTO(zones);	
}implement_end;
/////pTimer/////
void pTimer::RunTimer(int RequiredPeriod){
	Reset();
	StartTime=TrueTime;
	DoneTime=RequiredPeriod;
}
bool pTimer::Done(){
	if(StartTime<0)return false;
	return (TrueTime-StartTime>DoneTime);
}
bool pTimer::DoneOnce(){
	if(Done){		
		if(!Picked){
			Picked=true;
			return true;
		}
	}
	return false;
}
int	 pTimer::Elapsed(){
	if(StartTime<0)return 0;
	return TrueTime-StartTime;
}
void pTimer::Reset(){
	StartTime=-1;
	DoneTime=0;
	Picked=false;
}

implement_class(pTimer){
	REG_MEMBER(_int,StartTime);
	REG_MEMBER(_int,DoneTime);
	REG_MEMBER(_bool,Picked);
}implement_end;

implement_class(pTrigger){
	REG_MEMBER(_bool,State);
}implement_end;
//////////////////////////////////////////////////////////////////////////
// pABILITY
//////////////////////////////////////////////////////////////////////////
pABILITY::pABILITY(){
	_ability = NULL;
};
pABILITY::pABILITY(char* name){
	set( name );	
};
pABILITY::pABILITY(pABILITY& ABLILITY){
	set( ABLILITY.ability() );
};
ReferableBaseClass*		pABILITY::ability(){
	return _ability;	
};
bool					pABILITY::Valid(){
	return (ability()!=NULL);
};
pABILITY&				pABILITY::operator=(pABILITY& AR){
	if ( this!=(&AR) && AR.Valid() ) _ability = AR.ability();
	return (*this);
};
pABILITY&				pABILITY::set(_str  AbilName){
	set( AbilName.pchar() );
	return (*this);
};
pABILITY&				pABILITY::set(char* AbilName){
	if ( AbilName!=NULL && AbilName[0]!=0 ){
		UnitAbility* getAblByName(const char* name);
		_ability =  (ReferableBaseClass*)(getAblByName(AbilName));
	};
	return (*this);
};
pABILITY&				pABILITY::set(ReferableBaseClass* pABL){
	_ability = pABL;
	return (*this);
};
//////////////////////////////////////////////////////////////////////////
// pWEAPONMOD
//////////////////////////////////////////////////////////////////////////
pWEAPONMOD::pWEAPONMOD(){
	_weaponmod = NULL;
};
pWEAPONMOD::pWEAPONMOD(pWEAPONMOD& WEAPONMOD){
	*this = WEAPONMOD;	
};
ReferableBaseClass*	pWEAPONMOD::weaponmod(){
	return _weaponmod;
};
bool				pWEAPONMOD::Valid(){
	return (weaponmod()!=NULL);
};
pWEAPONMOD&			pWEAPONMOD::operator=(pWEAPONMOD& WM){
	if ( this!=(&WM) && WM.Valid() ) _weaponmod = WM.weaponmod();
	return (*this);
};
pWEAPONMOD&			pWEAPONMOD::set(ReferableBaseClass* pWM){
	_weaponmod = pWM;
	return (*this);	
};
///////////////////////////////////////////////////////////
/////////////////CMERA MANAGEMENT//////////////////////////
///////////////////////////////////////////////////////////
ReferableBaseClass* pCAMERA::camera(){
	return _camera.Get();	
};
bool				pCAMERA::Valid(){
	return (camera()!=NULL);
};
void				pCAMERA::set(ReferableBaseClass* pC){
	_camera.Set( pC );	
};
//////////////////////////////////////////////////////////////////////////
// pARRAY
//////////////////////////////////////////////////////////////////////////
pARRAY::pARRAY(){
	_array.Set(NULL);
};
pARRAY::pARRAY(pARRAY& AR){
	(*this)=AR;	
};
pARRAY::pARRAY(be_CARRAY* pAR){
	if ( pAR )	_array.Set(pAR);
};
be_CARRAY*		pARRAY::array(){
	return	(be_CARRAY*)(_array.Get());
};
bool			pARRAY::Valid(){
	return array()!=NULL;	
};
bool			pARRAY::getERROR(bool NotError,char* Type,int lid){
	if (!NotError) {
		DriveMode()->PROCESS=false;
		if ( lid==-1 ){
			_str msg = "get < ";
			msg += Type;
			msg += " > ARRAY is NULL.";
			MessageBox( NULL, msg.pchar(), "SCRIPT ERROR <get element from ARRAY>", MB_OK );
		};
		if ( lid!=-1 ){
			_str mes = "ARRAY < ";
			mes += array()->GetName();
			mes += " > can not get element < ";
			mes += lid;
			mes += " > it is NULL";
			MessageBox( NULL, mes.pchar(), "SCRIPT ERROR <get element from ARRAY>", MB_OK );
		}
	};
	return (!NotError);		
};
void			pARRAY::CLEAR(){
	if (Valid()) {
		getParamList().CLEAR();
	};
};
void			pARRAY::del(int lid){
	if (Valid()) {
		array()->DelValue(lid+1);
	};
};
pGroup			pARRAY::getGROUP(int lid){
	if ( getERROR(Valid(), "GROUP") ) return __g::g_refGROUP;
	be_CGROUP* pGR = (be_CGROUP*)(array()->GetValue(lid+1));
	if ( getERROR(pGR!=NULL,"GROUP",lid) ) return __g::g_refGROUP;
	__g::g_refGROUP.set( pGR );
	return __g::g_refGROUP;
};
void UnitAbilityTest();
iNode			pARRAY::getNODE(int lid){
	//UnitAbilityTest();
	if ( getERROR(Valid(), "NODE") ) return __g::g_refNODE;
	be_CNODE* pND = (be_CNODE*)(array()->GetValue(lid+1));
	if ( getERROR(pND!=NULL,"NODE",lid) ) return __g::g_refNODE;
	__g::g_refNODE.set( pND );
	//UnitAbilityTest();
	return __g::g_refNODE;
};
float&			pARRAY::getFLOAT(int lid){
	if ( getERROR(Valid(), "FLOAT") ) return __g::g_refFLOAT;
	be_CFLOAT* pVL = (be_CFLOAT*)(array()->GetValue(lid-1));
	if ( getERROR(pVL!=NULL,"FLOAT",lid) ) return __g::g_refFLOAT;
	return pVL->Value;
};
_str&			pARRAY::getSTR(int lid){
	if ( getERROR(Valid(), "STR") ) return __g::g_refSTR;
	be_CSTR* pVL = (be_CSTR*)(array()->GetValue(lid+1));
	if ( getERROR(pVL!=NULL,"STR",lid) ) return __g::g_refSTR;
	return pVL->Value;
};
int&			pARRAY::getINT(int lid){
	if ( getERROR(Valid(), "INT") ) return __g::g_refINT;
	be_CINT* pVL = (be_CINT*)(array()->GetValue(lid+1));
	if ( getERROR(pVL!=NULL,"INT",lid) ) return __g::g_refINT;
	return pVL->Value;
};
DWORD&			pARRAY::getDWORD(int lid){
	if ( getERROR(Valid(), "DWORD") ) return __g::g_refDWORD;
	be_CDWORD* pVL = (be_CDWORD*)(array()->GetValue(lid+1));
	if ( getERROR(pVL!=NULL,"DWORD",lid) ) return __g::g_refDWORD;
	return pVL->Value;
};
bool&			pARRAY::getBOOL(int lid){
	if ( getERROR(Valid(), "BOOL") ) return __g::g_refBOOL;
	be_CBOOL* pVL = (be_CBOOL*)(array()->GetValue(lid+1));
	if ( getERROR(pVL!=NULL,"BOOL",lid) ) return __g::g_refBOOL;
	return pVL->Value;
};
pARRAY			pARRAY::getARRAY(int lid){
	if ( getERROR(Valid(), "ARRAY") ) return __g::g_refARRAY;
	be_CARRAY* pAR = (be_CARRAY*)(array()->GetValue(lid+1));
	if ( getERROR(pAR!=NULL,"ARRAY",lid) ) return __g::g_refARRAY;
	__g::g_refARRAY.set( pAR );
	return __g::g_refARRAY;
};
pSTR_DIALOG		pARRAY::getDIALOG(int lid){
	if ( getERROR(Valid(), "STR_DIALOG") ) return __g::g_refDIALOG;
	be_CSTR_DIALOG* pDLG = (be_CSTR_DIALOG*)(array()->GetValue(lid+1));
	if ( getERROR(pDLG!=NULL,"STR_DIALOG",lid) ) return __g::g_refDIALOG;
	__g::g_refDIALOG.set( pDLG );
	return __g::g_refDIALOG;
};
pParamList		pARRAY::getParamList(){
	if ( getERROR(Valid(), "PARAMS") ) return __g::g_refPARAMS;
	__g::g_refPARAMS.set( array()->GetParmList() );
	return	__g::g_refPARAMS;
};		
int				pARRAY::getSIZE(){
	if ( getERROR(Valid(), "SIZE") ) return 0;
	return array()->GetAmount();
};
pARRAY&			pARRAY::operator=(pARRAY& AR){
	if ( this!=&AR && AR.Valid() ){
		_array.Set(AR.array());
	};
	return *this;	
};
pARRAY&			pARRAY::set(be_CARRAY* pAR){
	_array.Set(pAR);
	return (*this);
};
//////////////////////////////////////////////////////////////////////////
// pSTR_DIALOG
//////////////////////////////////////////////////////////////////////////
pSTR_DIALOG::pSTR_DIALOG(){
	set(NULL);
};
pSTR_DIALOG::pSTR_DIALOG(pSTR_DIALOG& DIALOG){
	if (DIALOG.Valid()) set( DIALOG.dialog() );
};
be_CSTR_DIALOG*	pSTR_DIALOG::dialog(){
	return _dialog.Get();
};
bool			pSTR_DIALOG::Valid(){
	return (dialog()!=NULL);
};
pSTR_DIALOG&	pSTR_DIALOG::operator=(pSTR_DIALOG& DLG){
	if ( DLG.Valid() )  set( DLG.dialog() );
	else				set( NULL );
	return (*this);
};
pSTR_DIALOG&	pSTR_DIALOG::set(be_CSTR_DIALOG* pDLG){
	_dialog.Set(pDLG);
	return (*this);
};
char*			pSTR_DIALOG::get_ogFile(){
	if ( Valid() )	return dialog()->Get_ogFile();
	return NULL;
};
pGroup			pSTR_DIALOG::get_Group(){
	be_CGROUP* pGR = (be_CGROUP*)(dialog()->Get_Group());
	__g::g_refGROUP.set( pGR );
	return __g::g_refGROUP;		
};
//////////////////////////////////////////////////////////////////////////
/////////////////////	FSM		//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// pMSG
pMSG::pMSG(be_CMsgObject* msg){
	_msg = msg;
};
pMSG::pMSG(pMSG& MS){
	(*this)=MS;	
};
be_CMsgObject* pMSG::msg(){
	return _msg;
};
bool	 pMSG::Valid(){
	return (msg()!=NULL);
};
_str&	 pMSG::str(){
	return	msg()->name;
};
void	 pMSG::set_str(_str& name){
	set_str(name.pchar());	
};
void	 pMSG::set_str(char* name){
	if ( name!=NULL && name[0]!=0 )	msg()->name=name;
};
_str&		pMSG::getSender(){
	if ( Valid() )	__g::g_refSTR = msg()->sender_id;
	else			__g::g_refSTR = "0xFFFFFFFF";
	return __g::g_refSTR;
};
pParamList	pMSG::getPARAMS(){
	__g::g_refPARAMS.set( ((be_CMSG*)msg())->GET_PARAMS() );
	return __g::g_refPARAMS;	
};
pMSG&	 pMSG::operator=(pMSG& msg){
	if ( msg.Valid() ){
		_msg = msg.msg();
	};
	return *this;
};

// pSMARTOBJECT
pSMARTOBJECT::pSMARTOBJECT(){
	_obj.Set(NULL);
};
pSMARTOBJECT::pSMARTOBJECT(be_CSMARTOBJECT* obj){
	if (obj)	_obj.Set(obj);
	else		_obj.Set(NULL);
};
pSMARTOBJECT::pSMARTOBJECT(pSMARTOBJECT& SO){
	(*this)=SO;	
};
be_CSMARTOBJECT*	pSMARTOBJECT::obj(){
	return (be_CSMARTOBJECT*)(_obj.Get());	
};
bool				pSMARTOBJECT::Valid(){
	return (obj()!=NULL);
};
void				pSMARTOBJECT::setSTATE( char* stName ){
	if ( Valid() )	obj()->SetState( stName );
};
void				pSMARTOBJECT::setSTATE( _str& stName ){
	setSTATE(stName.pchar());	
};
_str&				pSMARTOBJECT::getSTATE(){
	if ( Valid() )	__g::g_refSTR = obj()->GetState();
	return __g::g_refSTR;
};
void				pSMARTOBJECT::next_STATE( char* stName ){
	if ( Valid() )	obj()->SetNextState( stName );
};
void				pSMARTOBJECT::next_STATE( _str& stName ){
	next_STATE( stName.pchar() );
};
void				pSMARTOBJECT::FSC( bool type ){
	if ( Valid() )	obj()->SetForceSTST(type);
};
pSMARTOBJECT&		pSMARTOBJECT::operator=(pSMARTOBJECT& obj){
	if ( obj.Valid() ) {
		_obj.Set( obj.obj() );
	};
	return *this;
};
pSMARTOBJECT&		pSMARTOBJECT::set(be_CSMARTOBJECT* pobj){
	_obj.Set(pobj);
	return (*this);
};
pGroup				pSMARTOBJECT::getOBJ_GROUP(){
	__g::g_refGROUP.set( (be_CGROUP*)(obj()->GET_OBJECT()) );
	return __g::g_refGROUP;
};
bool				pSMARTOBJECT::setOBJ(char* _name){
	if ( Valid() )	return obj()->SET_OBJECT(_name);
	return false;
};
bool				pSMARTOBJECT::setOBJ(pGroup _group){
	if ( Valid() )	return obj()->SET_OBJECT(_group);
	return false;
};
pParamList			pSMARTOBJECT::getPARAMS(){
	__g::g_refPARAMS.set( obj()->GET_PARAMS() );
	return __g::g_refPARAMS;
};
char*				pSMARTOBJECT::getName(){
	if ( Valid() )	return obj()->GetName();
	return NULL;
};
void				pSMARTOBJECT::setName(char* name){
	if ( Valid() )	obj()->SetName(name);
};
//////////////////////////////////////////////////////////////////////////
void GameInterface::SmoothMap(DynArray<short>& Map, int Iteration){

	static DynArray<int> DM;
	int NZ=Map.GetAmount();
	DM.Check(NZ);
	DM.NValues=NZ;

	for(int p=0;p<Iteration;p++){
		DM.Fill(0);		
		for(int j=0;j<NZ;j++){
			Area* AR=GetTopMap(j);
			int NL=AR->NLinks;
			int dang=Map[j]/(Iteration<<1);
			for(int p=0;p<NL;p++){
				int id=AR->Link[p].NextAreaID;
				if(id<NZ){					
					DM[id]+=dang;
				};
			};
		};
		for(j=0;j<NZ;j++){
			Map[j]+=DM[j];
		}
	};
}
void SmoothDangerMap(short* DangMap, int ItrAmount);
void GameInterface::SmoothDangerMap(short* DangMap, int ItrAmount){
	::SmoothDangerMap(DangMap,ItrAmount);
};
/////GameInterface/////
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
void GameInterface::CreateSpecialEffect(const char* name,int srcx,int srcy,int srcz,int dstx,int dsty,int dstz){
	CreateNewActiveWeapon((char*)name,0xFFFF,srcx,srcy,srcz,0xFFFF,dstx,dsty,dstz,NULL);
}
void GameInterface::CreateSpecialEffect(const char* name,pUnit& src,int ExtraSrcZ,pUnit& dst,int ExtraDstZ){
	iVector2D vs=src.GetCoor();
	int sz=src.GetZ();
	iVector2D vd=dst.GetCoor();
	int dz=dst.GetZ();
	//CreateNewActiveWeapon(name,src.ID,vs.x,vs.y,sz,dst.ID,vd.x,vd.y,dz,dst.ID,NULL);
}
UnitAbility* GameInterface::GetUnitAbility(char* UnitAbilityName){
	UnitAbility* getAblByName(const char* name);
	return getAblByName(UnitAbilityName);
};
//extern const vClotheN;
extern int vClotheSet[];
int GameInterface::GetClothesTypeCapacity(int TypeClothe){
	if( TypeClothe>0 && TypeClothe<10/*vClotheN*/ )
		return vClotheSet[TypeClothe];
	return 0;
};

bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
extern AbilityList Abilities;

UnitAbility* getAblByName(const char* name){
	int n=Abilities.Abilities.GetAmount();
	for(int i=0;i<n;i++){
		UnitAbility* UA=Abilities.Abilities[i];
		if(UA && UA->Name.equal(name))return UA;
	}    
	return NULL;
}

int GameInterface::GetAmountAbility(int UnitIndex, char* UnitAbilityName){
	int a=0;
	UnitAbility* UA=getAblByName(UnitAbilityName);
	if(UA){
		OneObject* OB=Group[UnitIndex];
		if(OB){
			itr_AA.Create(OB);
			while(ActiveUnitAbility* aA=itr_AA.Next()){
				UnitAbility* uA=aA->GetA();
				if(uA==UA){
					a++;
				}
			}
		}
	}
	return a;
}
bool GameInterface::SetAbilityState(int UnitIndex, char* UnitAbilityName, int State){ // State - enum enABL_STATE
	UnitAbility* UA=getAblByName(UnitAbilityName);
	if(UA){
		OneObject* OB=Group[UnitIndex];
		if(OB){
			itr_AA.Create(OB);
			while(ActiveUnitAbility* aA=itr_AA.Next()){
				UnitAbility* uA=aA->GetA();
				if(uA==UA){
					switch(State){
						case 1://ABL_ACTIVE:
							aA->Visible=true;
							if(aA->TypeAbil==4)
								aA->Clothed=true;
							break;
						case 2://ABL_PASIVE:
							aA->Visible=true;
							if(aA->TypeAbil==4)
								aA->Clothed=false;
							break;
						case 4://ABL_HIDE:
							aA->Visible=false;
							break;
					};
				}
			}
		}
	}
	return false;
}
bool GameInterface::SetAbilityPriceMoney(int UnitIndex, char* UnitAbilityName, int Price){
	UnitAbility* UA=getAblByName(UnitAbilityName);
	if(UA){
		OneObject* OB=Group[UnitIndex];
		if(OB){
			itr_AA.Create(OB);
			while(ActiveUnitAbility* aA=itr_AA.Next()){
				UnitAbility* uA=aA->GetA();
				if(uA==UA){
					uA->ShopPrice=Price;
				}
			}
		}
	}
	return false;
}


void GameInterface::CreateAbility(const char* name,pUnit& unit){
	if(unit.object()){		
		UnitAbility* UA=getAblByName(name);
		if(UA){
			ActiveUnitAbility* aua=UA->GetNewActiveAbility();
			if(aua){
				AddActiveUnitAbility(unit.ID,aua);
			}
		}
	}
}
void GameInterface::ApplyAbility(const char* name,pUnit& unit,pUnit& caster){
	if(unit.object()){		
		UnitAbility* UA=getAblByName(name);
		if(UA){
			AddOneEffect(NULL,UA,unit.object(),caster.object());			
		}
	}
}
void GameInterface::AlertOnMap(int _x,int _y,int _delay,int _nat){
	be_gAddAlertOnMap(_x,_y,_delay,_nat);	
};
void GameInterface::AddTomahawksAttackers(pGroup& Grp,byte Owner,bool remove,word Base,int DriftRadius, int CriticalMass){
	void AddTomahawks(be_CGROUP* Grp,byte Owner,bool remove,word Base,int DriftRadius, int CriticalMass);
	AddTomahawks( Grp.group(), Owner, remove, Base, DriftRadius, CriticalMass );
};
// UNITS ABILITY
bool	GameInterface::AddCarryAbility(int Index, pABILITY& UA){
	if (UA.Valid()) return AddCarryAbility_lua(Index,(UnitAbility*)(UA.ability()));
	return false;
};
bool	GameInterface::DeleteCarryAbility(int Index, pABILITY& UA){
	if (UA.Valid()) return DeleteCarryAbility_lua(Index,(UnitAbility*)(UA.ability()));
	return false;
};
ActiveUnitAbility* DetectCarryAbility(OneObject* OB, UnitAbility* UA);
ActiveUnitAbility*	GameInterface::DetectCarryAbility(int Index, pABILITY& UA){
	if( Index!=0xFFFF && UA.Valid() ) 
		return ::DetectCarryAbility(Group[Index],(UnitAbility*)(UA.ability()));
	return NULL;
};
ActiveUnitAbility*	GameInterface::DetectAbilityEffect(int Index, pABILITY& UA){
	UnitAbility* uA=(UnitAbility*)UA.ability();
	if(uA){
		itr_AA.Create(Group[Index]);
		while(ActiveUnitAbility* A=itr_AA.Next()){
			if( (A->TypeAbil==0||uA->ClassType==2) && A->GetA()==uA ){
				return A;
			}
		}
	}
	return NULL;
};
void	GameInterface::SetAbilityState(pGroup& GR,_str& AbilName,int state){
	if (GR.Valid()) SetAblState_lua( GR.group(),AbilName.pchar(), enABL_STATE(state) );
};
int		GameInterface::GetAbilityState(pGroup& GR,_str& AbilName){
	if (GR.Valid()) return GetAblState_lua( GR.group(), AbilName.pchar() );
	return 0;
};
int		GameInterface::GetAbilityState(pGroup& GR,pABILITY& Ability){	
	if (GR.Valid()&&Ability.Valid()){
		//be_CBaseParamFunction::pAbility = (ActiveUnitAbility*)Ability.ability();
		int a = GetAblState_lua( GR.group(), Ability.ability()->Name.pchar() );
		//be_CBaseParamFunction::pAbility = NULL;
		return a;
	}
	return 0;
};
int		GameInterface::GetAbilityOBNation(){
	return	GetAbilityOwnerNation_lua();
};
int		GameInterface::GetAbilityOwnerObject(pGroup& GR){
	if (GR.Valid()) return GetAbilityOwnerObject_lua(GR.group());
	return 0;
};
int		GameInterface::GetAbilityActivatorObject(pGroup& GR){
	if (GR.Valid()) return	GetAbilityActivatorObject_lua(GR.group());
	return 0;
};
int		GameInterface::CheckAbilityActivatorObject(pGroup& GR){
	if (GR.Valid()) return g_beCheckAbilityActivatorObject( GR.group() );	
	return 0;
};
bool	GameInterface::AbilityExecute(pABILITY& ABIL,pGroup& CASTER,pGroup& TARGET,int x,int y){
	return AblExecute_lua((UnitAbility*)(ABIL.ability()),CASTER.group(),TARGET.group(),x,y);
};
ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
ActiveUnitAbility* GameInterface::AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster){
	return ::AddOneEffect(WP,UA,Dest,Caster);
};
bool CheckInfluenceMask(ActiveUnitAbility* V, DWORD Mask);
bool GameInterface::CheckInfluenceMask(ActiveUnitAbility* V, DWORD Mask){
	return ::CheckInfluenceMask(V,Mask);
};
// WEAPON MODIFICATOR
void	GameInterface::CreateNewActiveWeaponMod(pWEAPONMOD& WM,int Index,int sx,int sy,int DestIndex,int dx,int dy){
	if (WM.Valid()) CreateNewActiveWeapon_lua(WM.weaponmod()->Name.pchar(),Index,sx,sy,DestIndex,dx,dy);
};
// CLOUD DIALOGS
int		GameInterface::dl_GetLineCountInCloudDialog(pSTR_DIALOG& DLG){
	if (DLG.Valid()) return be_gGetLineCountInCloudDialog(DLG.dialog());
	return 0;
};
int		GameInterface::GetGlobalTime(){
	int	GetGlobalTime_lua();
	return GetGlobalTime_lua();
};
void	GameInterface::dl_ShowCloudDialog(pSTR_DIALOG& DLG,pGroup& GR){
	if (DLG.Valid()){
		if ( GR.Valid() ) be_gShowCloudDialog(DLG.dialog(),GR.group());
		else			  be_gShowCloudDialog(DLG.dialog(),NULL);
	} 
};
void	GameInterface::dl_HideCloudDialog(){
	be_gShowCloudDialog(NULL,NULL);
};
bool	GameInterface::FilePresent(char* FName){
	return be_gFilePresent(FName);
};
bool	GameInterface::dl_GetCloudDilogCancel(){
	return be_gGetCloudDilogCancel();
};
void	GameInterface::dl_SetCloudDilogCancel(bool state){
	be_gSetCloudDilogCancel(state);
};
bool	GameInterface::dl_GetCloudDilogCenter(){
	return	be_gGetCloudDilogCenter();
};
void	GameInterface::dl_SetCloudDilogCenter(bool state){
	be_gSetCloudDilogCenter(state);	
};
void	GameInterface::dl_ClearCloudDilogSPEAKERS(){
	be_gClearCloudDilogSPEAKERS();
};
void	GameInterface::dl_AddCloudDilogSPEAKERS(pGroup GR){
	if (GR.Valid()) be_gAddCloudDilogSPEAKERS(GR.group());
};
bool	GameInterface::dl_getCenterOfSPEAKERS(int& x,int& y,pGroup& GR){
	if ( GR.Valid() ) return be_getCenterOfSPEAKERS(x,y,GR.group());
	return 	be_getCenterOfSPEAKERS(x,y);
};
bool	GameInterface::dl_getCurrentSpeakerPosition(int& x,int& y){
	return be_getCurrentSpeakerPosition(x,y);	
};
extern GameSettings GSets;
int		GameInterface::dl_getAdditionalDelay(){
	return GSets.SVOpt.DialogAdditionViewTime;
};
// SOUND
bool	GameInterface::ogg_Play(char* pFName){
	const NStream=4;
	for(int i=0;i<NStream;i++){
		if(ov_StreamFinished(i)){
			ov_Play(pFName,i);
			return true;
		}
	}
	return false;
};
void	GameInterface::ogg_Play(char* pFName,int stream){
	if(stream==1){
		int vol=GSets.SVOpt.SoundVolume;
		ov_SetVolume(vol*GSets.SVOpt.RelativDialogSoundVolume,1);
		void AddVoiceToQueue(char* FileName);
		AddVoiceToQueue(pFName);
		return;
	}
	ov_Play(pFName,stream);
};
void	GameInterface::ogg_Stop(int stream){
	ov_Stop(stream);
};
bool	GameInterface::ogg_StreamFinished(int stream){
	return	ov_StreamFinished(stream);
};
int		GameInterface::ogg_GetSoundVolume(){
	return 	GSets.SVOpt.SoundVolume;
};
// DLL FUNCTIONS managnent
void	GameInterface::dllfnClearList(){
	g_BE_FN_STORE.LUA_FN_CLEAR();
};
void	GameInterface::modulfnClearList(){
	g_BE_FN_STORE.MODUL_FN_CLEAR();
};
// FSM managnent
bool GameInterface::FreeSO_pVAR(pGroup& GR){
	if ( GR.Valid() )	return	be_gFreeSO_pVAR( (be_CVariableBase*)(GR.group()) );	
	return false;
};
pSMARTOBJECT  GameInterface::FindSO(char* name){
	be_CSMARTOBJECT* pSO = be_gFindSO(name);
	__g::g_refSMARTOBJECT.set(pSO);
	return __g::g_refSMARTOBJECT;
};
void GameInterface::SendMSG(char* name,pSMARTOBJECT& sender,pSMARTOBJECT& receiver){
	if ( name!=NULL&&name[0]!=0&&sender.Valid()&&receiver.Valid() ) {
		g_beSendMsg( name,sender.obj()->GetName(),receiver.obj()->GetName(), sender.obj(), receiver.obj() );
	};
};
void GameInterface::SendMSG(char* name,int delay,pSMARTOBJECT& sender,pSMARTOBJECT& receiver){
	if ( name!=NULL&&name[0]!=0&&sender.Valid()&&receiver.Valid() ) {
		g_beSendDelayedMsg( name,delay,sender.obj()->GetName(),receiver.obj()->GetName(), sender.obj(), receiver.obj() );
	};
};
bool GameInterface::CheckObjectFSM(char* ObjectName,char* FSM_Name){
	return	be_gCheckObjectFSM(ObjectName,FSM_Name);
};
pSMARTOBJECT GameInterface::GET_SO(char* FSM_Name){
	__g::g_refSMARTOBJECT.set( be_gGET_SO(FSM_Name) );
	return __g::g_refSMARTOBJECT;
};
pSMARTOBJECT GameInterface::FindSObyName(char* SO_Name){
	__g::g_refSMARTOBJECT.set( be_gFindSObyName( SO_Name ) );
	return __g::g_refSMARTOBJECT;
};
int	GameInterface::GetMsgSenderNation(_str strID){
	if (strID.pchar()!=NULL&&(strID.pchar())[0]!=0 ) return GetNation_lua(strID.pchar());
	return -1;
};
// Only for DLL add function decription managment
void GameInterface::AddFunctionToDeclarationList(char* fn_name){
	g_be_LUA_ADD_FN(fn_name);
};
void GameInterface::AddParamsToFunctionInList(char* fn_name,char* par_type,char* par_name){
	g_be_LUA_ADD_FN_PARAM(fn_name,par_type,par_name);
};

void GameInterface::WorldToScreenSpace( Vector3D& vec ){
	::WorldToScreenSpace( vec );
};
void GameInterface::DrawString(char* Str, iVector2D& v, DWORD col ){
	DWORD c=SmallWhiteFont.GetColor();
	SmallWhiteFont.SetColor(col);
	ShowString(v.x,v.y,Str,&SmallWhiteFont);
	SmallWhiteFont.SetColor(c);
}
void GameInterface::DrawLine(iVector2D& v0, iVector2D& v1/*, int dZ*/, DWORD col ){
	//GPS.DrawLine(Vector3D(v0.x,v0.y,GetHeight(v0.x,v0.y)+dZ),Vector3D(v1.x,v1.y,GetHeight(v1.x,v1.y)+dZ),col);
	GPS.DrawLine(v0.x,v0.y,v1.x,v1.y,col);
}
void GameInterface::DrawUnitsRanges(pUnitsList& units){
	int n=units._getamount();
	if(n>0){
		int NU=0;
		static DynArray<word> IDS;
		static DynArray<word> SNS;
		IDS.FastClear();
		SNS.FastClear();
		units.ToBegin();
		pUnit u;
		while(units.NextLive(u)){
			NU++;
			IDS.Add(u.ID);
			SNS.Add(u.SN);
		}
		void ShowUnitsRanges(word* IDS,word* SNS,int NU);
		ShowUnitsRanges(IDS.Values,SNS.Values,NU);
	}
}
void DrawSmartArrow(int x0,int y0,int x1,int y1,byte StartDirection,DWORD Color1,DWORD Color2,int StartThickness,int EndThickness);
void GameInterface::DrawSmartArrow(iVector2D& v0, iVector2D& v1/*, byte StartDirection, DWORD Color1, DWORD Color2*/){
	word GetDir(int dx,int dy);
	byte Dir=GetDir(v1.x-v0.x,v1.y-v0.y);
	::DrawSmartArrow(v0.x,v0.y,v1.x,v1.y,Dir,EngSettings.BrigadesArrowParam.StartArrowColor
		,EngSettings.BrigadesArrowParam.FinalArrowColor,EngSettings.BrigadesArrowParam.StartArrowWidth
		,EngSettings.BrigadesArrowParam.FinalArrowWidth);
}

bool GameInterface::ClassEditorCheckIfOpen(BaseClass* BC){
	return ClassEditor::CheckIfOpen(BC);
};
// -------------
int GameInterface::GetUnitsAmount(){
	return MAXOBJECT;
}
pUnit GameInterface::GetUnit(int Index){
	OneObject* OB=Group[Index];
	if(OB)return pUnit(Index,OB->Serial);
	else return pUnit(0xFFFF,0xFFFF);
}
int GameInterface::GetUnitsAmountInNation(int NI){
	xassert(NI>=0 && NI<8);
	return NtNUnits[NI];
}
pUnit GameInterface::GetUnitInNation(int NI,int Index){
	xassert(NI>=0 && NI<8);
	xassert(Index>=0 && Index<NtNUnits[NI]);
	word idx=NatList[NI][Index];
	if(idx!=0xFFFF){
		OneObject* OB=Group[idx];
		if(OB)return pUnit(idx,OB->Serial);
	}
	return pUnit(0xFFFF,0xFFFF);
}
pUnit GameInterface::CreateUnit(int x,int y,byte NI,pUnitType& Type,bool Anyway,directions_list direction){
	xassert(NI>=0 && NI<8);
	short d=direction;
	if(d>255)d=-1;
	word index=NATIONS[NI].CreateNewMonsterAt(x<<4,y<<4,Type.GetType(),Anyway,d);
	if(index!=0xFFFF){
		return pUnit(index);
	}
	return pUnit(0xFFFF,0xFFFF);
}
int GameInterface::CreateRandomUnits(int N,int x,int y,int R,byte NI,pUnitType& Type,pUnitsList* List,int MinDistBetweenUnits,bool Anyway,directions_list direction){
	xassert(NI>=0 && NI<8);
	short d=direction;
	if(d>255)d=-1;
	int natt=N*8;
	while((natt--) && N){
		int xu=x-R+(rando()%(2*R));
		int yu=y-R+(rando()%(2*R));
		int r=Norma(x-xu,y-yu);
		if(r<R){
			if(MinDistBetweenUnits>0){
				itr_UnitsInRadius.Create(xu,yu,MinDistBetweenUnits);
				if(itr_UnitsInRadius.Next())break;
			}
			word id=NATIONS[NI].CreateNewMonsterAt(xu<<4,yu<<4,Type.GetType(),Anyway,d);
			if(id!=0xFFFF){
				N--;
				pUnit u(id);
				if(List)List->_add(u);
			}
		}
	}
	return 0;
}
bool	GameInterface::CreateFormation(pGroup& storeGR,int x,int y,byte NI,pUnitType& Type,int FormationType,bool Anyway,directions_list direction){
	xassert(NI>=0 && NI<8);
	if (storeGR.Valid()==false) storeGR.CreateLocal();
	return PutNewFormation_lua(storeGR.group(),NI,Type.GetType(),FormationType,x,y,direction);
};
//void GameInterface::SetProduceList(byte NI,pUnitType& UnitType,pUnitTypeList& List);
//void GameInterface::SetUpgradeList(byte NI,pUnitType& UnitType,pUpgradeList& List);	
//void GameInterface::SetUnitForceEnable(byte NI,pUnitType& Type,bool state);
//void GameInterface::SetUnitDefaultEnable(byte NI,pUnitType& Type);


//low-level functions - x,y,R,Lx,Ly are measured in lockpoints; 1 lickpoint=16 pixels
void GameInterface::SetLockPt(int x,int y,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].BSetPt(x,y);
	}
}
void GameInterface::ClrLockPt(int x,int y,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].BClrPt(x,y);
	}
}
void GameInterface::XorLockPt(int x,int y,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].BXorPt(x,y);
	}
}
bool GameInterface::ChkLockPt(int x,int y,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		if(MFIELDS[i].CheckPt(x,y))return true;
	}
	return false;
}

void GameInterface::SetLockBar(int x,int y,int Lx,int Ly,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].BSetSQ(x,y,Lx,Ly);
	}
}
void GameInterface::ClrLockBar(int x,int y,int Lx,int Ly,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].BClrSQ(x,y,Lx,Ly);
	}    
}
void GameInterface::XorLockBar(int x,int y,int Lx,int Ly,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].BXorSQ(x,y,Lx,Ly);
	}    
}
bool GameInterface::ChkLockBar(int x,int y,int Lx,int Ly,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		if(MFIELDS[i].CheckBar(x,y,Lx,Ly))return true;
	}
	return false;
}

void GameInterface::SetLockRound(int x,int y,int D,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].SetRound(x,y,D);
	}
}
void GameInterface::ClrLockRound(int x,int y,int D,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].ClearRound(x,y,D);
	}
}
void GameInterface::XorLockRound(int x,int y,int D,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		MFIELDS[i].XorRound(x,y,D);
	}
}
bool GameInterface::ChkLockRound(int x,int y,int D,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		if(MFIELDS[i].CheckRound(x,y,D))return true;
	}
	return false;
}

void GameInterface::RefreshTopology(int x,int y,int Lx,int Ly,DWORD opt){
	for(int i=0;i<NMFIELDS;i++)if(opt & (1<<i)){
		HashTable[i].ReCreateAreas((x>>2)-1,(y>>2)-1,((x+Lx)>>2)+1,((y+Ly)>>2)+1);
	}
}

//extern int MinMapX;
//extern int MaxMapX;
//extern int MinMapY;
//extern int MaxMapY;
iRect GameInterface::GetMapLimitations(){
//	iRect r(MinMapX<<5,MinMapY<<5,MaxMapX<<5,MaxMapY<<5);
	const RECT rc = MOptions.LimitCamArgs.GetRect();
	iRect r(rc.left<<5,rc.top<<5,rc.right<<5,rc.bottom<<5);
	return r;
}
void GameInterface::SetMapLimitations(int x,int y,int x1,int y1){
	void SetScrollLimitations(int x0,int y0,int x1,int y1);
	SetScrollLimitations(x,y,x1,y1);
}
void GameInterface::SetDefaultLimitation(){
	void SetInitialScrollLimitations();
	SetInitialScrollLimitations();
}
void GameInterface::SetPositionOnMap(float x,float y){
	void SetCameraPos(float x,float y);
	SetCameraPos((float)x/32.0f,(float)y/32.0f);
}
void GameInterface::ScrollToPositionOnMap(float x,float y,int time){
	CameraDriver()->Set(x,y,time);	
	CameraDriver()->Set(true);
};
extern int LastMx;
extern int LastMy;
iVector2D GameInterface::GetCursorPosOnMap(){
	iVector2D v(LastMx,LastMy);
	return v;
}

ICamera* GameInterface::GetCamera(){
	return ICam;
}
void GameInterface::SetCameraMovingStage(float Stage){
	extern float CameraMovingStage;
	CameraMovingStage=Stage;
}
// nation 
int	GameInterface::GetNKilled( int NI, pUnitType& UT ){
	return	be_gGetNKilled(NI,UT.GetType());
};
int	GameInterface::GetResource(int nat,int resid){
	return GetResource_lua(nat,resid);	
};
bool GameInterface::AddRessource(int nat,int resid,int Amount){
	if ( 0<=nat&&nat<8 && 0<=resid&&resid<8 ){
		AddRessource_lua(nat,resid,Amount);
		return true;
	};
	return false;	
};
int	GameInterface::SetAddFarms(int Nat,int Farms,bool set){
	return SetAddFarms_lua(Nat,Farms,set);	
};
int	GameInterface::GetNationMask(int Nat){
	if ( 0<=Nat && Nat<8 )	return NATIONS[Nat].NMask;
	return 255;
};
//
int GameInterface::GetTLayersAmount(){
	return NMFIELDS;
}
int GameInterface::GetTZonesAmount(int Layer){
	xassert(Layer>=0 && Layer<NMFIELDS);
	return HashTable[Layer].NAreas;
}
void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
iVector2D GameInterface::GetTZoneCoor(int Layer,int Index){
	xassert(Layer>=0 && Layer<NMFIELDS);
	int n=HashTable[Layer].NAreas;
	
	/* --- EDIT BY XENO ----
	The realerror is in GetTopRef of VAI dll!! 
	It returns an impossible value for 'Top' in GroupPurpose::GetTopZone --> GetTopRef
	*/
	if (Index < 0) Index = 0;
	else if (Index >= n) Index = n;
	//xassert(Index>=0 && Index<n);

	iVector2D v(0,0);
	GetPreciseTopCenter(Index,Layer,v.x,v.y);
	return v;
}

int GameInterface::GetTZoneIndex(int Layer,int x,int y){
	xassert(Layer>=0 && Layer<NMFIELDS);
	return HashTable[Layer].GetTopAreaIndex(x>>6,y>>6);
}
int GameInterface::GetTZoneNLinks(int Layer,int Index){
	xassert(Layer>=0 && Layer<NMFIELDS);
	int n=HashTable[Layer].NAreas;
	if(Index<0 || Index>=n)return 0;
	Area* A=HashTable[Layer].TopMap+Index;
	return A->NLinks;
}
TLinkInfo* GameInterface::GetTZoneLink(int Layer,int Index,int LinkIndex){
	xassert(Layer>=0 && Layer<NMFIELDS);
	int n=HashTable[Layer].NAreas;
	if(Index<0 || Index>=n)return NULL;
	Area* A=HashTable[Layer].TopMap+Index;
	if(LinkIndex>=0 && LinkIndex<A->NLinks)return (TLinkInfo*)(A->Link+LinkIndex);
	return NULL;   
}
int GameInterface::GetNextZoneOnTheWay(int Layer,int StartIndex,int FinalIndex,byte NI){
	xassert(Layer>=0 && Layer<NMFIELDS);
	return HashTable[Layer].GetNextAreaOnTheWayTo(StartIndex,FinalIndex,NI);
}
int GameInterface::GetTZoneApproxRadius(int Layer,int Index){
	xassert(Layer>=0 && Layer<NMFIELDS);
	int n=HashTable[Layer].NAreas;
//	if (Index < 0) Index = 0;
    //else if (Index >= n) Index = n;
    xassert(Index>=0 && Index<n);
	Area* A=HashTable[Layer].TopMap+Index;
	int x0=A->x;            
	int y0=A->y;
	for(int r=0;r<32;r++){
		int nt=0;
		int N=Rarr[r].N;
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		for(int j=0;j<N;j++){
			int x=x0+xi[j];
			int y=y0+yi[j];
			if(HashTable[Layer].GetTopAreaIndex(x,y)==Index)nt++;			
		}
		if(nt==0)return r*32;
	}
	return r*32;
}
int		GameInterface::GetAreaX(int AreaID, int layerID){
	return be_gGetAreaX(AreaID,layerID);	
};
int		GameInterface::GetAreaY(int AreaID, int layerID){
	return be_gGetAreaY(AreaID,layerID);	
};
bool	GameInterface::ControlNodeArray(pARRAY& ARR,int LT,int NI,bool circle){
	return	be_gControlNodeArray(ARR.array(),LT,NI,circle);	
};
bool	GameInterface::MarkBadNode(pARRAY& ARR,int LT,int NI,bool circle){
	return be_gMarkBadNode(ARR.array(),LT,NI,circle);	
}
//vectors management
int		GameInterface::NORMA(int x1,int x2,int y1, int y2){
	return Norma( x1-x2,y1-y2 );
};
int		GameInterface::NORMA(iVector2D& v){
	return Norma( v.x, v.y );
};
DLLEXPORT int GetZonesDist2(int Start,int Fin,byte TopType);
int GameInterface::NORMA(int Layer,int StartTop,int FinalTop){
	return GetZonesDist2(StartTop,FinalTop,Layer);
};
float	GameInterface::DOT(iVector2D& v0,iVector2D& v1){
	return v0.x*v1.x+v0.x*v1.x;
};
int GameInterface::GetHeight(int x,int y){
	return ::GetHeight(x,y);
};
//camera managment
void	GameInterface::CAMERA_Play(pCAMERA& c){
	if ( c.Valid() ) ((be_CCAMERA*)(c.camera()))->CameraTracks.Play();
};
bool	GameInterface::CAMERA_IsPlaying(pCAMERA& c){
	if ( c.Valid() ) return ((be_CCAMERA*)(c.camera()))->CameraTracks.IsPlaying();
	return false;
};
void	GameInterface::CAMERA_Stop(pCAMERA& c){
	if ( c.Valid() ) ((be_CCAMERA*)(c.camera()))->CameraTracks.Stop();
};
void	GameInterface::CAMERA_Free(pCAMERA& c){
	if ( c.Valid() ) ((be_CCAMERA*)(c.camera()))->CameraTracks.Free();
};
//units management
extern bool NOPAUSE;
// special for StarForce
__declspec(dllexport)
void GameInterface::RunScript(){
	NOPAUSE=true;
}
__declspec(dllexport)
void GameInterface::PauseScript(){
	NOPAUSE=false;
}
extern bool LockPause;
void GameInterface::SetLockPause(bool state){
	LockPause = state;	
};
void CmdEndGame(byte NI,byte state,byte cause);
void GameInterface::Victory(){
	CmdEndGame(MyNation,2,0);
}
void GameInterface::Defeat(){
	CmdEndGame(MyNation,1,0);
}
void GameInterface::Victory(byte NI){
	xassert(NI<8);
	CmdEndGame(NI,2,0);
}
void GameInterface::Defeat(byte NI){
	xassert(NI<8);
	CmdEndGame(NI,1,0);
}
void GameInterface::SwithTankMode(){
	g_TankController.Toggle();
};
bool GameInterface::GetTankMode(){
	return 	g_TankController.IsActive();
};
bool GameInterface::GetInsideDialogState(){
	bool GetInsideDialogState();
	return 	GetInsideDialogState();
};
void GameInterface::SetInsideDialogState(bool state){
	void SetInsideDialogState(bool State);	
	SetInsideDialogState(state);
};
void GameInterface::PrintConsoleMessage(const char* mask,...){
	va_list args;
	va_start(args,mask);

	char* temp=GetGlobalBuffer();
	vsprintf(temp,mask,args);
	va_end(args);
	PrintConsoleMessage2(temp);
}
void GameInterface::PrintConsoleMessage2(const char* message, int time){
	AssignHint1((char*)GetTextByID((char*)message),time);
}
extern bool EnterChatMode;
void ClearKeyStack();
void GameInterface::ActivatePromptBar(){
	EnterChatMode=1;
	ClearKeyStack();
}
DLLEXPORT void ShowCentralText(char* ID,int time);
void GameInterface::PrintCentralMessage(const char* mask,...){
	va_list args;
	va_start(args,mask);
	char* temp=GetGlobalBuffer();
	vsprintf(temp,mask,args);
	va_end(args);
	PrintCentralMessage2(temp);
}
void GameInterface::PrintCentralMessage2(const char* message){
	ShowCentralText((char*)message,256);
}
void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
void GameInterface::PlayFullscreen(const char* FileName,float px,float py){
	PlayFullscreenVideo((char*)FileName,px,py);
}
int GameInterface::random(){
	return rando();
}
int GameInterface::random(int NValues){
    if(NValues)return (int(rando())*NValues)>>15;
}
int GameInterface::random(int MinValue,int MaxValue){
	if(MaxValue<MinValue)swap(MaxValue,MinValue);
	if(MaxValue>MinValue)return MinValue+random(MaxValue-MinValue);
	else return MinValue;
}
const char* GameInterface::GetTextByID(const char* ID){
	return ::GetTextByID((char*)ID);
}
bool GameInterface::PrepareString(_str& destSTR, _str& srcSTR, pARRAY& params){
	if ( destSTR.pchar()==NULL && srcSTR.pchar()==NULL ) return false;
	if ( params.getSIZE()==0 )	destSTR = GetTextByID( srcSTR.pchar() );
	else	be_gPrepareString( destSTR, srcSTR.pchar(), params.array()->ValuesArray );
	return true;	
};
ClonesArray<ClassRef<IUnitsIterator> > GameInterface::ActiveGroups;
void IUnitsIterator::AddNewOrder(IGroupOrder* Order,int NTimes,int Period){
	Order->StartTime=TrueTime;
	Order->NTimes=NTimes;
	Order->Period=Period;
	if(!Orders.GetAmount()){
		ClassRef<IUnitsIterator>* CRGO=new ClassRef<IUnitsIterator>;
		CRGO->Set(this);
		GameInterface::ActiveGroups.Add(CRGO);
		Order->GroupOwner.Set(this);
	}
	Orders.Add(Order);
}
void GameInterface::ProcessActiveGroups(){
	int n=GameInterface::ActiveGroups.GetAmount();
	for(int i=0;i<n;i++){
		ClassRef<IUnitsIterator>* CR=ActiveGroups[i];
		bool del=true;
		if(CR->Get()){
			IUnitsIterator* UI=CR->Get();
			if(UI){
				for(int j=0;j<UI->Orders.GetAmount();j++){
					bool ok=true;
					IGroupOrder* GO=UI->Orders[j];
					if(GO->Period==0)ok=GO->Process();
					else{
						int n=(TrueTime-GO->StartTime)/GO->Period;
						if(n>GO->NTimes){
							GO->NTimes++;
							ok=GO->Process();
						}
					}
					if(!ok){
						delete(GO);
						UI->Orders[j]=0;
						UI->DelElement(j);
						j--;
					}
				}				
				if(UI->Orders.GetAmount()==0){
					CR->Set(NULL);
				}else del=false;
			}
		}
		if(del){
			ActiveGroups.DelElement(i--);
			n--;
		}
	}
}
void ProcessActiveGroupsInGameInterface(){
	GameInterface::ProcessActiveGroups();
}
IExchangeInterface* GetIExchangeInterface(DWORD Code);
IExchangeInterface* GameInterface::GetExchangeInterface(DWORD GlobalID){
	return GetIExchangeInterface(GlobalID);
}
/////Class resgistration/////
void RegisetrGameInterfaceClassBlock(){
	REG_CLASS(iZone);
	REG_CLASS(RoundZone);
	REG_CLASS(SquareZone);
	REG_CLASS(MultiZone);
	REG_CLASS(OneDialogTopic);
	REG_CLASS(OneDialogItem);
	REG_CLASS(OneDialog);
	REG_CLASS(OneDialogsFolder);
	REG_CLASS(DialogInfo);
	REG_CLASS(pUnitsList);
}
/////Dialogs management/////
ClonesArray<DialogInfo> GameInterface::ActiveDialogs;

implement_class(DialogInfo){
    REG_AUTO(OnAnswer);
	REG_AUTO(CurrentDialog);
	REG_AUTO(Answerer);
	REG_AUTO(MsgSource);
	REG_MEMBER(_bool,Visible);
	REG_MEMBER(_int,CurrentAnswer);
	REG_MEMBER(_int,StartTime);
	REG_MEMBER(_int,AnswerTime);
	REG_MEMBER(_int,HideTimeout);
	REG_MEMBER(_int,AnswerHideTimeout);
	REG_MEMBER(_int,DefaultAnswerID);
}implement_end;

implement_class(AnswerFn){
	REG_PARENT(BaseFunction);
}implement_end;
void AnswerFn::EvaluateFunction(){
	DialogInfo* DI=get_parent<DialogInfo>();
	if(DI){
		if(SimpleDialog::CallParams.Clicked){
			DI->CurrentAnswer=SimpleDialog::CallParams.Clicked->AutoIndexInArray;
		}
	}
}

OneDialogsFolder AllQuests;

void RegQuestEditor(){
	AllQuests.SafeReadFromFile("Missions\\quests.xml");
	AddStdEditor("Quest dialogs editor",&AllQuests,"Missions\\quests.xml",RCE_DEFAULT);
}


DialogInfo::DialogInfo(){
    Visible=false;
	CurrentAnswer=-1;
	MsgSource=pUnit(0xFFFF,0xFFFF);
	MsgPosition=iVector3D(LastMx,LastMy,0);
	StartTime=TrueTime;
	AnswerTime=0;
	HideTimeout=60000;
	AnswerHideTimeout=6000;
	DefaultAnswerID=-1;
	Answerer=pUnit(0xFFFF,0xFFFF);
}
DialogInfo::~DialogInfo(){
}
DialogInfo& pDialogInfo::_di(){
	if(Get())return *((DialogInfo*)Get());
	else{
		static DialogInfo di;
		return di;
	}
}
int pDialogInfo::GetAnswer(){
	return _di().CurrentAnswer;
}
void pDialogInfo::ClearAnswerState(){
	_di().CurrentAnswer=-1;
}
void pDialogInfo::Show(){
	_di().Visible=true;
	_di().StartTime=TrueTime;
	char* vf=_di().CurrentDialog.Topic.VoiceFileReference.pchar();
	if(vf[0]){
		GameInterface::ogg_Play(vf);
	}
}
void pDialogInfo::Hide(){
	_di().Visible=false;
}
bool pDialogInfo::GetVisibleState(){
	return _di().Visible;
}
void pDialogInfo::ShowQuestionNearUnit(pUnit& Unit){
	_di().MsgSource=Unit;
	_di().MsgPosition=Unit.GetCoor3D();
}
void pDialogInfo::ShowAnswerNearUnit(pUnit& Unit){
    _di().Answerer=Unit;
}
void pDialogInfo::ForceSetAnswer(const char* ID,int Timeout){
	ClonesArray<OneDialogTopic>& di=_di().CurrentDialog.Answers;
	for(int i=0;i<di.GetAmount();i++){
		if(di[i]->ID.equal(ID)){
			ForceSetAnswer(i,Timeout);
		}
	}
}
void pDialogInfo::ForceSetAnswer(int idx,int Timeout){
	if(Timeout==0)_di().CurrentAnswer=idx;
	else{
		_di().StartTime=TrueTime;
		_di().AnswerTime=-1;
		_di().HideTimeout=Timeout;
		_di().DefaultAnswerID=-1;
	}
	xassert(idx>=-1 && idx<_di().CurrentDialog.Answers.GetAmount());
}
OneDialog* OneDialogsFolder::FindDialog(const char* ID){
	for(int i=0;i<Items.GetAmount();i++){
		OneDialog* D=dynamic_cast<OneDialog*>(Items[i]);
		if(D){
			if(D->DialogID.equal(ID))return D;
		}else{
			OneDialogsFolder* DF=dynamic_cast<OneDialogsFolder*>(Items[i]);
			if(DF){
				OneDialog* D=DF->FindDialog(ID);
				if(D)return D;
			}
		}
	}
	return NULL;
}
bool OneDialog::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="OneDialog: DialogID=%$DialogID$% Message={CY}%$Topic$%{C}\\      %$Answers,/$%";
	return true;
}
bool OneDialogTopic::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="({CR}%$ID$%{C}){CY}%$Message$%{C}";
	return true;
}
pDialogInfo GameInterface::GetDialog(const char* DialogID){
	pDialogInfo di;
    OneDialog* D=AllQuests.FindDialog(DialogID);
	if(D){
        DialogInfo* DI=new DialogInfo;
		ActiveDialogs.Add(DI);
		D->Copy(&DI->CurrentDialog,false);
		DI->DLG.SafeReadFromFile("Dialogs\\QDesk.DialogsSystem.xml");
		if(DI->DLG.DSS.GetAmount()){
			DI->Coor=dynamic_cast<DialogsDesk*>(DI->DLG.DSS[0]);
		}
		DI->DLG.AttachToClass(DI);
		di.Set(DI);
	}
	return di;
}
void pDialogInfo::SetActiveQuest(const char* DialogID){
	DialogInfo* DI=Get();
	if(DI){
		OneDialog* D=AllQuests.FindDialog(DialogID);
		if(D){
			D->Copy(&DI->CurrentDialog,false);
			DI->DLG.SafeReadFromFile("Dialogs\\QDesk.DialogsSystem.xml");
			if(DI->DLG.DSS.GetAmount()){
				DI->Coor=dynamic_cast<DialogsDesk*>(DI->DLG.DSS[0]);
			}
			DI->DLG.AttachToClass(DI);		
			DI->CurrentAnswer=-1;
		}
	}
}
iRect GetDialogBoundary(ParentFrame* PF){
	iRect mm(100000,100000,-100000,-100000);
	int x0=PF->LastDrawX;
	int y0=PF->LastDrawY;
	int x1=PF->LastDrawX+PF->x1-PF->x;
	int y1=PF->LastDrawY+PF->y1-PF->y;
	if(x0<mm.xmin)mm.xmin=x0;
	if(y0<mm.ymin)mm.ymin=y0;
	if(x1>mm.xmax)mm.xmax=x1;
	if(y1>mm.ymax)mm.ymax=y1;
	for(int i=0;i<PF->DSS.GetAmount();i++){
		iRect r=GetDialogBoundary(PF->DSS[i]);
		if(r.xmin<mm.xmin)mm.xmin=r.xmin;
		if(r.ymin<mm.ymin)mm.ymin=r.ymin;
		if(r.xmax>mm.xmax)mm.xmax=r.xmax;
		if(r.ymax>mm.ymax)mm.ymax=r.ymax;
	}
	return mm;
}
iRect GetDialogContentBoundary(ParentFrame* PF){
	iRect mm(100000,100000,-100000,-100000);
	for(int i=0;i<PF->DSS.GetAmount();i++){
		iRect r=GetDialogBoundary(PF->DSS[i]);
		if(r.xmin<mm.xmin)mm.xmin=r.xmin;
		if(r.ymin<mm.ymin)mm.ymin=r.ymin;
		if(r.xmax>mm.xmax)mm.xmax=r.xmax;
		if(r.ymax>mm.ymax)mm.ymax=r.ymax;
	}
	return mm;
}
void DialogInfo::Process(){
	if(StartTime>=0 && Visible){
		int Dt=TrueTime-StartTime;
		if(Dt>HideTimeout && CurrentAnswer==-1){
			if(DefaultAnswerID!=-1){
				CurrentAnswer=DefaultAnswerID;
			}else{
                Visible=false;				
				return;
			}
		}
		if(CurrentAnswer!=-1 && AnswerTime==0){
			AnswerTime=TrueTime;
            CurrentDialog.Answers[CurrentAnswer]->Copy(&CurrentDialog.Topic,false);
			CurrentDialog.Answers.Clear();
			char* vf=CurrentDialog.Topic.VoiceFileReference.pchar();
			if(vf[0]){
				GameInterface::ogg_Play(vf);
			}
		}
		if(AnswerTime>0){
			int Dt=TrueTime-AnswerTime;
			if(Dt>AnswerHideTimeout){
				Visible=false;
				return;
			}
		}
	}
}
void DialogInfo::Draw(){
	pUnit u=MsgSource;
	if(AnswerTime>0 && Answerer.Valid())u=Answerer;
	if(u.Valid()){
		MsgPosition=u.GetCoor3D();
	}
	Vector3D V(MsgPosition.x,MsgPosition.y,MsgPosition.z);
	WorldToScreenSpace(V);
	if(Coor){				
		int DX=V.x-Coor->x-50;
		int DY=V.y-Coor->y-Coor->GetHeight();
		//Coor->x+=DX;
		//Coor->y+=DY;
		//Coor->x1+=DX;
		//Coor->y1+=DY;
		/*
		iRect R=GetDialogContentBoundary(Coor);
		static int DefLy=-80;
		static int DefDx=30;
		int Dy=R.ymax-R.ymin-DefLy;				
		V.x-=DefDx;
		V.y-=Dy;

		int DX=V.x-Coor->x;
		int DY=V.y-Coor->y;
		Coor->x+=DX;
		Coor->y+=DY;
		Coor->x1+=DX;
		Coor->y1+=DY;
		*/
	}
	DLG.ProcessDialogs();
}
pDialogInfo& pDialogInfo::operator  = (pDialogInfo& op){
    Set(op.Get());
	return *this;
}
void DialogInfo::DrawAndProcessAll(){
	for(int i=0;i<GameInterface::ActiveDialogs.GetAmount();i++){
		DialogInfo* DI=GameInterface::ActiveDialogs[i];
		if(DI->Visible){
			DI->RegisterMembers();
            DI->Draw();
			DI->Process();
		}		
	}
}
void ShowQuestDialogs(){
	DialogInfo::DrawAndProcessAll();	
}
void GameInterface::ProcessEditor(char* EditorName){
    ::ProcessEditor(EditorName);
}
void GameInterface::ReplaceEditor(char* EditorName, BaseClass* pClass){
    ::ReplaceEditor(EditorName,pClass);
}
void GameInterface::AddStdEditor(char* Name,BaseClass* BC,const char* XmlName,DWORD Options,rce_ProcessClassCallback* Process,rce_InputCallback* Input,word HotKey){
    ::AddStdEditor(Name,BC,XmlName,Options,Process,Input,HotKey);
}
void DelStdEditor(const char* Name);
void GameInterface::DelStdEditor(char* Name){
    ::DelStdEditor(Name);
}
void GameInterface::SimpleEditClass(char* Name,BaseClass* BC,const char* XmlName,bool Autosave){
    ::SimpleEditClass(Name,BC,XmlName,Autosave);
}
void GameInterface::EditClass(char* Name,BaseClass* BC,const char* XmlName,DWORD Options,rce_ProcessClassCallback* Process,rce_InputCallback* Input){
    ::EditClass(Name,BC,XmlName,Options,Process,Input);
}
void GameInterface::CAMERA_SwitchToSelectedUnit(bool MoveInto,bool ManageUnit){
	if(MoveInto){
		if(!g_Navigator.m_fLinkToHero) {
			g_Navigator.Toggle(true);
			g_Navigator.LinkToHeroToggle();
		}
		g_Navigator.m_fDisableHeroManagement=ManageUnit;
	}else{
		if(g_Navigator.m_fLinkToHero){
		 	g_Navigator.Home();
		}
		g_Navigator.m_fDisableHeroManagement=false;
	}
}
_str GameInterface::NextLoadMap;
void GameInterface::RunGameOnMap(const char* name){
	NextLoadMap=name;	
}
void CreateNationalMaskForMap(char* Name);
void PrepareGameMedia(byte myid,bool SaveNR,bool DisableDLL);
void CheckIfNewMapLoadingNeed(){
	if(GameInterface::NextLoadMap.pchar()[0]){
		strcpy(GSets.CGame.cgi_CurrentMap,GameInterface::NextLoadMap.pchar());
		CreateNationalMaskForMap(GSets.CGame.cgi_CurrentMap);
		CurrentCampagin=-1;
		CurrentMission=-1;
		PrepareGameMedia(0,0,1);
		GameInterface::NextLoadMap="";
		DriveMode()->StartMission();
	}
}
const char* GameInterface::GetCurrentMapName(){
	return GSets.CGame.cgi_CurrentMap;
}
extern int FogMode;
void GameInterface::SwitchFogOfWar(bool State){
    FogMode=State;
}
void GameInterface::SwitchStopMode(bool State){
	NOPAUSE=State;
    LockPause=!State;
}
void					GameInterface::LogWarning( const char* format, ... ){
	va_list argList;
	va_start( argList, format );
	Log.Warning(format,argList);
	va_end( argList );
};
void GameInterface::RunCameraTrack(const char* Name){
	for(int i=0;i<MOptions.CameraTracks.GetAmount();i++){
		if(MOptions.CameraTracks[i]->Name.equal(Name)){
			MOptions.CameraTracks[i]->Track.Play();
		}
	}
}
bool GameInterface::CheckIfCameraIsMoving(){
	for(int i=0;i<MOptions.CameraTracks.GetAmount();i++){
		if(MOptions.CameraTracks[i]->Track.IsPlaying())return true;
	}
	return false;
}
bool GameInterface::CheckIfTrackExists(const char* Name){
	for(int i=0;i<MOptions.CameraTracks.GetAmount();i++){
		if(MOptions.CameraTracks[i]->Name.equal(Name))
			return true;
	}
	return false;
}
int GameInterface::GetAmountOfTracks(){
	return MOptions.CameraTracks.GetAmount();
}
const char*	GameInterface::GetTrackName(int Idx){
	return MOptions.CameraTracks[Idx]->Name.pchar();
}
void GameInterface::StopCameraTrack(){
	for(int i=0;i<MOptions.CameraTracks.GetAmount();i++){
		if(MOptions.CameraTracks[i]->Track.IsPlaying()){
			MOptions.CameraTracks[i]->Track.Stop();
		}
	}
}

_str GameInterface::InfoMessage="";
int  GameInterface::InfoMessageHideTime=0;

void GameInterface::OnUnloading(){
	InfoMessageHideTime=0;
	InfoMessage="";
}
int DrawMultilineText(int x,int y,char* s,RLCFont* DefaultFont,int& MaxLx,int MaxLy,byte CenterX,bool Draw);
void GameInterface::OnDrawOnMapOverAll(){
	if(InfoMessageHideTime>TrueTime && InfoMessage.pchar()[0]){
		static int posY=180;
		int MaxLx=300;
		int Ly=DrawMultilineText(-1000,-1000,InfoMessage.pchar(),&WhiteFont,MaxLx,100000,0,false);
		GPS.FlushBatches();
		GPS.DrawFillRect(3,posY,MaxLx+8,Ly+12,0x90000000);
		GPS.FlushBatches();
		MaxLx=300;
		DrawMultilineText(7,posY+4,InfoMessage.pchar(),&WhiteFont,MaxLx,100000,0,true);
	}
}
void GameInterface::PrintInfoMessage(int Duration,const char* mask,...){
	va_list args;
	va_start(args,mask);
	char* temp=GetGlobalBuffer();
	vsprintf(temp,mask,args);
	va_end(args);
    InfoMessage=temp;
	InfoMessageHideTime=TrueTime+Duration;
}
bool SaveActiveAbilitiesToXml(OneObject* OB,xmlQuote& xml);
bool LoadActiveAbilitiesFromXml(OneObject* OB,xmlQuote& xml);

void pUnit::SaveUnitCharacter(xmlQuote& XML){
    if(object())
        SaveActiveAbilitiesToXml(object(),XML);
}
void pUnit::LoadUnitCharacter(xmlQuote& XML){
    if(object())
        LoadActiveAbilitiesFromXml(object(),XML);
}
void RotUnit(OneObject* OB,char Dir,byte OrdType);
void pUnit::RotateUnit(byte Dir,int OrdType){
    OneObject* OB=object();
    if(OB)RotUnit(OB,Dir,OrdType);
}
void pUnit::SetUnitDirection(byte Dir){
    OneObject* OB=object();
    if(OB){
        OB->RealDir=Dir;
        OB->RealDirPrecise=DWORD(Dir)<<8;
    }
}
int pUnit::GetUnitZ(){
    OneObject* OB=object();
    if(OB){
        return OB->RZ;
    }else return 0;
}

int pUnit::GetUnitOverEarth(){
    OneObject* OB=object();
    if(OB){
        return OB->OverEarth;
    }else return 0;
}
void pUnit::SetUnitZ(int z){
    OneObject* OB=object();
    if(OB){
        OB->RZ=z;
		addrand(OB->RZ);
    }
}
void pUnit::SetUnitOverEarth(int z){
    OneObject* OB=object();
    if(OB){
        OB->OverEarth=z;
    }
}
HeroVariableStorage* pUnit::DetectHero() const {
	return ::DetectHero(object());
}

// PickUp
ActiveUnitAbility* GetPickUpAbility(OneObject* OB);
bool StartDialog(OneObject* OB, byte OrdType, OneObject* DialogOB);
void StartDialogLink(OneObject* OB);

ActiveUnitAbility* pUnit::GetPickUpAbility(){
	return ::GetPickUpAbility(object());
};
bool pUnit::PickUpUnit(pUnit& Chest, byte OrdType){
	return ::StartDialog(object(),OrdType,Chest.object());
};
pUnit pUnit::IsPickUping(){
	pUnit Chest;
	if( Valid() ){
		Order1* Or=object()->LocalOrder;
		//bool find=false;
		while(Or){
			if(Or->DoLink==&StartDialogLink){
				word ID=Or->info.MoveToObj.ObjIndex;
				word SN=Or->info.MoveToObj.SN;
				Chest.Set(ID,SN);
				//find=true;
				break;
			}
			Or=Or->NextOrder;
		}
		//if(!find){
		//	CloseLib=true;
		//}
	}
	return Chest;
};

bool pUnit::DetectChestNotEmpty() const {
	uaa_Chest* Ch=::DetectChest(object());
	if( Ch && !Ch->Empty ){
		return true;
	}
	return false;
};
bool pUnit::StartDialog(byte OrdType, OneObject* DialogOB){
	return ::StartDialog(object(),OrdType,DialogOB);
};
//
int GameInterface::height(int x,int y){
    return GetTotalHeight(x,y);
}

void GameInterface::AddDialogHistoryTalker(const char* chr){
	DialogsHistory.Income=true;
	DString txt;
	txt="#";
	txt+=chr;
	DialogsHistory.Text.print("{dy2}{Ftm12}{C FFFFCB2B}%s\n",GetTextByID(txt.pchar()));
};
void GameInterface::AddDialogHistoryMessage(const char* chr){
	DialogsHistory.Income=true;
	DialogsHistory.Text.print("{Fsm01}{C FFB5B09C}%s\n",chr);
};
void GameInterface::AddDialogHistoryEndChapter(){
	DialogsHistory.Income=true;
	static int gid=GPS.PreLoadGPImage("interf3\\InGame\\_common_one_piece");
	const int sid=5;
	static int lx=GPS.GetGPWidth(gid,sid);
	static int ly=GPS.GetGPHeight(gid,sid);
	DialogsHistory.Text.print("{dy4}{G %d %d 0 0 %d %d}\n{dy4}",gid,sid,lx,ly);
};
void GameInterface::AddDialogToHistory(pARRAY& dARR){
	if ( dARR.Valid() ){
		_str lastSP = "";
		int dN = dARR.getSIZE();
		for ( int i=0; i<dN; i++ ){
			be_CSTR_DIALOG* pCD = dARR.getDIALOG(i).dialog();
			if ( pCD ){
				pCD->Update();
				if ( lastSP!=pCD->Speaker ){
					lastSP = pCD->Speaker;
					AddDialogHistoryTalker( lastSP.pchar() );
				};
				AddDialogHistoryMessage( pCD->Text.pchar() );
			};
		};
		AddDialogHistoryEndChapter();
	};
};

//////////////////////////////////////////////////////////////////////////
NewAnimation* pUnit::GetAnimation(int Code,int Index){
    OneObject* OB=object();
	if(OB){
		return Index==-1?OB->newMons->Animations->Get(Code):OB->newMons->Animations->Get(Code,Index);
	}
	return NULL;
}
NewAnimation* pUnit::GetCurrentAnimation(){
	OneObject* OB=object();
	if(OB){
		return OB->NewAnm;
	}
}
void pUnit::CopyAnimation(int DestCode,NewAnimation* Anm){
	OneObject* OB=object();
	if(OB && Anm){
		NewAnimation* NA=new NewAnimation;
		Anm->Copy(NA,false);
		DWORD C0=Anm->Code;
		NA->Code=DestCode;
		OB->newMons->Animations->Add(NA,DestCode);
		NA->SrcCode=C0;
	}
}
void pUnit::SwapAnimations(int SrcCode,int DstCode){
    OneObject* OB=object();
	if(OB){
        int N=OB->newMons->Animations->ANM.GetAmount();
		for(int i=0;i<N;i++){
            NewAnimation* NA=OB->newMons->Animations->ANM[i];
			if(NA->SrcCode==DstCode)NA->Code=NA->SrcCode;
			if(NA->SrcCode==SrcCode)NA->Code=NA->SrcCode;
			if(NA->Code==SrcCode || NA->Code==DstCode){
				NA->Code=NA->SrcCode;
				if(NA->Code==SrcCode)NA->Code=DstCode;
				else if(NA->Code==DstCode)NA->Code=SrcCode;
			}
		}
		OB->newMons->Animations->CreateHashTable();		
	}
}
void pUnit::DelAnimation(int Code,int Index){
	OneObject* OB=object();
	if(OB){
		int N=OB->newMons->Animations->ANM.GetAmount();
		for(int i=0;i<N;i++){
			NewAnimation* NA=OB->newMons->Animations->ANM[i];
			if(NA->Code==Code){
				if(Index<=0){
					OB->newMons->Animations->ANM.DelElement(i);
					i--;
					N--;
					if(Index==0)break;
				}
				if(Index>0)Index--;
			}
		}
	}
}
int pUnit::GetAnimationsAmount(int Code){
	int n=0;
	OneObject* OB=object();
	if(OB){
		int N=OB->newMons->Animations->ANM.GetAmount();
		for(int i=0;i<N;i++){
			NewAnimation* NA=OB->newMons->Animations->ANM[i];
			if(NA->Code==Code)n++;
		}
	}
	return n;
}
bool pUnit::CheckAnimationPresence(int Code){
	return GetAnimationsAmount(Code)!=0;
}
DWORD pUnit::GetAnimationSource(NewAnimation* NA){
	return NA->SrcCode;
}
bool ProcessLoadSave(int x,int y,int Lx,int Ly,_str& str,const char* mask,const char* StartDir,bool Load,ParentFrame* Back);
bool GameInterface::ShowOpenFileDialog(_str& FileName,const char* mask,const char* StartDir){
	return ProcessLoadSave(0,0,RealLx,RealLy,FileName,mask,StartDir,true,NULL);
}
bool GameInterface::ShowSaveFileDialog(_str& FileName,const char* mask,const char* StartDir){
	return ProcessLoadSave(0,0,RealLx,RealLy,FileName,mask,StartDir,false,NULL);
}



bool FindSafeWayToObject(int StartTop,DynArray<int>& DangerMap,DynArray<word>& TargetMap,DynArray<word>& SafeWay){
	//TIME1=GetTickCount();
	//if(GetZonesDist(Start,Fin)>=0xFFFF)return 0xFFFF;
	int NAR=GetNAreas();
	if(StartTop<0||StartTop>=NAR)return 0xFFFF;
	static DynArray<word> PrecisePointWeight(NAR,0xFFFF);
	static DynArray<word> PrecisePointPrevIndex(NAR,0xFFFF);
	static DynArray<word> CandidatPointWeight(NAR,0xFFFF);
	static DynArray<word> CandidatPointPrevIndex(NAR,0xFFFF);
	static DynArray<word> CandidatList;	
	PrecisePointWeight.Fill(0xFFFF);
	CandidatPointWeight.Fill(0xFFFF);
	CandidatPointWeight[StartTop]=0;
	CandidatPointPrevIndex[StartTop]=0xFFFF;
	//CandidatList[0]=StartTop;
	//int NCandidats=1;
	CandidatList.FastClear();
	CandidatList.Add(StartTop);
	addrand(StartTop);

	do{
		//best candidate is a last candidate in sorted list
		
		//NCandidats--;
		//word TZ=CandidatList[NCandidats];
		int cid=CandidatList.GetAmount()-1;
		word TZ=CandidatList[cid];
		CandidatList.Del(cid,0);

		if(TZ!=0xFFFF){
			PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
			CandidatPointWeight[TZ]=0xFFFF;
			PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
			if(TargetMap[TZ]!=0xFFFF){
				//it finally happen!!!
				//int MaxDang=DangerMap[TZ];
				SafeWay.FastClear();
				SafeWay.Add(TZ);
				int T0=TZ;
				for(int q=0;T0!=0xFFFF;q++){
					T0=CandidatPointPrevIndex[T0];
					if(T0!=0xFFFF){
						SafeWay.Add(T0);
						//int w=DangerMap[T0];
						//if(w>MaxDang)MaxDang=w;
					};
				};
				//*MaxDanger=MaxDang;
				//*DestObj=TargetMap[TZ];
				//TIME1=GetTickCount()-TIME1;
				//if(NWPTS>1)return FULLWAY[NWPTS-2];
				//else return FULLWAY[NWPTS-1];
				return true;
			};
			Area* TAR=GetTopMap(TZ);
			int NL=TAR->NLinks;
			OneLinkInfo* LINK=TAR->Link;
			int w0=PrecisePointWeight[TZ];
			for(int i=0;i<NL;i++){
				int pi=LINK[i].NextAreaID;
				//adding point to the candidates list
				//1.checking if point is in precise list
				if(PrecisePointWeight[pi]==0xFFFF){

					int wi=LINK[i].NextAreaDist+w0+DangerMap[pi];
					
					//2.checking if point is already in candidates list
					int wc;
					bool add=1;
					if((wc=CandidatPointWeight[pi])!=0xFFFF){
						if(wc>wi){
							//new point is better. Need to delete old point
							int n=CandidatList.GetAmount();
							for(int j=0;j<n/*NCandidats*/;j++)if(CandidatList[j]==pi){
								//if(j<NCandidats-1)
								//	memcpy(CandidatList+j,CandidatList+j+1,(NCandidats-j-1)<<1);
								//j=NCandidats;
								//NCandidats--;
								CandidatList.Del(j,1);
								break;
							};
							CandidatPointWeight[pi]=wi;
						}else{
							add=0;
						};
					};
					if(add){
						//need to add point to candidates list and sort array
						int n=CandidatList.GetAmount();
						if(n/*NCandidats*/==0){
							//CandidatList[0]=pi;
							CandidatList.Add(pi);
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							//NCandidats++;
						}else{
							int IDXmax=0;
							int IDXmin=n/*NCandidats*/-1;
							int wcmax=CandidatPointWeight[CandidatList[IDXmax]];
							int wcmin=CandidatPointWeight[CandidatList[IDXmin]];
							if(wi<=wcmin){
								//CandidatList[NCandidats]=pi;
								//NCandidats++;
								CandidatList.Add(pi);
								CandidatPointWeight[pi]=wi;
								CandidatPointPrevIndex[pi]=TZ;
							}else if(wi>wcmax){
								//memmove(CandidatList+1,CandidatList,NCandidats<<1);
								//CandidatList[0]=pi;
								//NCandidats++;
								CandidatList.Insert(0,pi);
								CandidatPointWeight[pi]=wi;
								CandidatPointPrevIndex[pi]=TZ;
							}else{
								while(IDXmax!=IDXmin-1){
									int IDXmid=(IDXmin+IDXmax)>>1;
									int wm=CandidatPointWeight[CandidatList[IDXmid]];
									if(wm>wi){
										wcmax=wm;
										IDXmax=IDXmid;
									}else{
										wcmin=wm;
										IDXmin=IDXmid;
									};
								};
								//memmove(CandidatList+IDXmin+1,CandidatList+IDXmin,(NCandidats-IDXmin+1)<<1);
								//CandidatList[IDXmin]=pi;
								//NCandidats++;
								CandidatList.Insert(IDXmin,pi);
								CandidatPointWeight[pi]=wi;
								CandidatPointPrevIndex[pi]=TZ;
							};
						};
					};
				};
			};
		}else return false;
	}while(CandidatList.GetAmount());
	return false;
}

int GetWayDistance(DynArray<word>& Way){
	int dist=0;
	int n=Way.GetAmount()-1;
	for(int i=0;i<n;i++){

	}
	return dist;
}
void GameInterface::CreateMassRoom(const char* MapName,const char* PlName,int HeroType){
    MRoom->CreateServer(PlName,MapName,MyNation,HeroType);
}
bool GameInterface::JoinMassRoom(const char* PlName,const char* ServerAddr,int HeroType){
	return MRoom->JoinServer(ServerAddr,PlName,MyNation,HeroType);	
}
bool GameInterface::isHumanPlayer(byte Color){
	return GSets.CGame.isHumanPlayer(Color);
};
int pUnit::GetDamage(int AttType){
	OneObject* OB=object();
	if(OB){
		int Dam=OB->MoreCharacter->MaxDamage[AttType];
		if(OB->ActiveAbility){			
			OB->ActiveAbility->ActiveAbilities.modifyDamage(AttType,Dam,Dam,NULL);
		}
		return Dam;
	}
	return 0;
}
int	pUnit::GetBasicDamage(int AttType){
	OneObject* OB=object();
	if(OB){
		int Dam=OB->MoreCharacter->MaxDamage[AttType];
		return Dam;
	}
	return 0;
    
}
int	pUnit::GetMaxDamage(){
	OneObject* OB=object();	
	if(OB){
		int maxD=0;
		int at=0;
		for(int i=0;i<4;i++){
			int Dam=OB->MoreCharacter->MaxDamage[i];
			if(Dam>maxD){
				maxD=Dam;
				at=i;
			}
		}
		if(OB->ActiveAbility){			
			OB->ActiveAbility->ActiveAbilities.modifyDamage(at,maxD,maxD,NULL);
		}
		return maxD;
	}    
	return 0;
}
int	pUnit::GetProtection(int AttType){
	OneObject* OB=object();	
	if(OB){
        int sh=OB->MoreCharacter->Protection[AttType]+OB->MoreCharacter->Shield+OB->AddShield;
		if(OB->ActiveAbility){			
			OB->ActiveAbility->ActiveAbilities.modifyProtection(AttType,sh,sh,NULL);
		}
        return sh;
	}else return 0;    
}
void GameInterface::ActivateObjectState(const char* ObjectName,const char* StateName){
	SceneObjExtension::ActivateState(ObjectName,StateName,NULL);
}
void ShowUnitOpinion(OneObject* OB, char* Text, DWORD Color);
void pUnit::ShowOpinion(char* Text, DWORD Color){
	ShowUnitOpinion(object(),Text,Color);
};