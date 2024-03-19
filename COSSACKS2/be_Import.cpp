#include "stdheader.h"
#include "BE_HEADERS.h"
#include "ua_Extern.h"


// Units ABILITY Import
bool	AddCarryAbility_lua(int Index, UnitAbility* UA){
	if ( UA ){
		OneObject* pOB = Group[Index];
		if ( pOB && ( !pOB->Sdoxlo || pOB->Hidden ) ) {
			return ( AddCarryAbility( pOB, UA ) != NULL );
		}
	};
	return false;
};
// specially for StarForce
__declspec(dllexport)
bool	DetectCarryAbility_lua(int Index, UnitAbility* UA){
	if ( UA ){
		OneObject* pOB = Group[Index];
		if ( pOB && ( !pOB->Sdoxlo || pOB->Hidden ) ) {
			return ( DetectCarryAbility( pOB, UA ) != NULL );
		}
	};
	return false;
};

__declspec(dllexport)
bool	DeleteCarryAbility_lua(int Index, UnitAbility* UA ){
	if ( UA ){
		OneObject* pOB = Group[Index];
		if ( pOB && ( !pOB->Sdoxlo || pOB->Hidden ) ) {
			return DeleteCarryAbility( pOB, UA );
		};
	};	
	return false;
};

__declspec(dllexport)
bool	DeleteCarryAbility_lua(int Index, const char* Name){
	if ( g_beCheckChar(Name)==true ){
		OneObject* pOB = Group[Index];
		if ( pOB && ( !pOB->Sdoxlo || pOB->Hidden ) ) {
			return DeleteCarryAbility( pOB, (char*)Name );
		};
	};	
	return false;
};

__declspec(dllexport)
void		SetAblState_lua( be_CGROUP* pGRP, const char* AblName, enABL_STATE state ){
	if ( AblName==NULL || AblName[0]==0 ) return;
		
	if ( be_CFunctionBase::pAbility!=NULL ){
		if ( be_CFunctionBase::pAbility->OB!=NULL && 
			(!be_CFunctionBase::pAbility->OB->Sdoxlo || be_CFunctionBase::pAbility->OB->Hidden) )
		{
			SetAblState(be_CFunctionBase::pAbility->OB,AblName,state);
		};
	};

	if ( pGRP==NULL )	 return;
	int N = pGRP->GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = pGRP->GetOneObj_lID(N);
		if ( pOB!=NULL )	SetAblState(pOB,AblName,state);
	};
};

enABL_STATE	GetAblState_lua( be_CGROUP* pGRP, const char* AblName ){
	if ( AblName==NULL || AblName[0]==0 ) return ABL_ABSENT;
		
	//if ( be_CFunctionBase::pAbility!=NULL ){
	//	if ( be_CFunctionBase::pAbility->OB!=NULL && 
	//		(!be_CFunctionBase::pAbility->OB->Sdoxlo || be_CFunctionBase::pAbility->OB->Hidden) )
	//	{
	//    	return GetAblState(be_CFunctionBase::pAbility->OB,AblName);
	//	};
	//};
	if(pGRP->GetTotalAmount()>0){
		OneObject* OB=(*pGRP)[0];
		if(OB)return GetAblState(OB,AblName);
	}
	return ABL_ABSENT;
};

int			GetAbilityOwnerNation_lua(){
	if ( be_CFunctionBase::pAbility!=NULL ){
		if ( be_CFunctionBase::pAbility->OB!=NULL && 
			(!be_CFunctionBase::pAbility->OB->Sdoxlo || be_CFunctionBase::pAbility->OB->Hidden) )
		{
			return be_CFunctionBase::pAbility->OB->NNUM;
		};
	};
	return -1;
};

int			GetAbilityOwnerObject_lua(be_CGROUP* pGR){
	if (be_CFunctionBase::pAbility==NULL) return 0;
	if ( pGR==NULL ) return 0;
	pGR->RemAllUnits();
    OneObject* pOB = be_CFunctionBase::pAbility->OB;
	if ( pOB!=NULL && (!pOB->Sdoxlo || pOB->Hidden) ){
		pGR->AddUnitGPT(pOB);
		addname(pOB->Index);
		return 1;
	};
	return 0;
}

int			GetAbilityActivatorObject_lua(be_CGROUP* pGR){
	if ( pGR==NULL ) return 0;
	pGR->RemAllUnits();
	OneObject* pOB = be_CFunctionBase::pAbilityActivator;
	if ( pOB!=NULL && (!pOB->Sdoxlo || pOB->Hidden) ){
		pGR->AddUnitGPT(pOB);
		addname(pOB->Index);
		return 1;
	};
	return 0;
};

int			g_beCheckAbilityActivatorObject(be_CGROUP* pGR){
	if ( be_CFunctionBase::pAbilityActivator==NULL || pGR==NULL || pGR->GetTotalAmount()<=0 ) return 0;
	if ( pGR->_CheckGPT(be_CFunctionBase::pAbilityActivator)!=0xFFFF ) return 1;
	return 0;
}

ClonesArray<AblName>* GetDialogAbilities(OneObject* OB){
	be_CSUBFUNCTION* pSF = NULL;
	int i = 0;
	while ( pSF=(be_CSUBFUNCTION*)g_beVAR_STORE.GET("be_CSUBFUNCTION",i++) ) {
		if ( pSF->IsMyOneObject(OB) ){
			return &(pSF->AbilityList);
		};
	};
	return NULL;
};

en_AblExecState GetAblExecuteState_lua(	UnitAbility*		UA, 
									    be_CGROUP*		Caster, 
										be_CGROUP*		Target, 
										int				x	  , 
										int				y
										)
{
	if (	UA==NULL || Caster==NULL || (*Caster)[0]==NULL ){
		return	aes_Absent;
	};
	return GetAblExecuteState(UA,(*Caster)[0],(*Target)[0],x,y,GetHeight(x,y));											
};

en_AblExecState AblExecute_lua(	UnitAbility*	UA, 
								be_CGROUP*	Caster, 
								be_CGROUP*	Target, 
								int			x	  , 
								int			y
							  )
{	
	if (	UA==NULL || Caster==NULL ){
		return	aes_Absent;
	};
	int Nc = Caster->GetTotalAmount();
	while (Nc--) {
		if ( (*Caster)[Nc]!=NULL ){
			OneObject* obTarget=NULL;
			if( Target )
				obTarget=(*Target)[0];
			AblExecute(UA,(*Caster)[Nc],obTarget,x,y,GetHeight(x,y));
		};
	};
	return aes_Casting;
};

void DisableCapturability_lua(be_CGROUP* pGRP){
	if (pGRP==NULL)	return;
	int N = pGRP->GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		if ( pOB=(*pGRP)[N] )	DisableCapturability(pOB);
	}
};

void CreateNewActiveWeapon_lua(	const char*				WMName,
								int						Index,
								int						sx, 
								int						sy, 
								int						DestIndex, 
								int						dx, 
								int						dy
							  )
{
	int rIndex		= 0xFFFF;
	int rDestIndex	= 0xFFFF;
	if ( Index>0 && Index<0xFFFF ) rIndex = Index;
	if ( DestIndex>0 && DestIndex<0xFFFF ) rDestIndex = DestIndex;
	CreateNewActiveWeapon(	(char*)WMName,
							rIndex,
							sx,sy,max(GetHeight(sx,sy),0),
							rDestIndex,
							dx,dy,max(GetHeight(dx,dy),0),
							NULL
						 );	
};
//////////////////////////////////////////////////////////////////////////
























