#include "stdheader.h"
#include "BE_HEADERS.h"
#include "HashTop.h"

typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);

// Aditional Functions /////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------//
struct aiLOOK_ROUND {
	aiLOOK_ROUND(){
		searchMR_R		= 0;
		MeleeN			= 0;
		RangeN			= 0;
		searchNR_R		= 0;
		pNEAREST		= NULL;
		searchWE_R		= 0;
		pWEEKEST		= NULL;
		searchXY_R		= 0;
		escX			= 0;
		escY			= 0;
		UOB				= NULL;
		};
	aiLOOK_ROUND(OneObject* pOB){
		searchMR_R		= 0;
		MeleeN			= 0;
		RangeN			= 0;
		searchNR_R		= 0;
		pNEAREST		= NULL;
		searchWE_R		= 0;
		pWEEKEST		= NULL;
		searchXY_R		= 0;
		escX			= 0;
		escY			= 0;
		( pOB&&(!pOB->Sdoxlo||pOB->Hidden) ) ? ( UOB=pOB ) : ( UOB=NULL );
		};
	aiLOOK_ROUND(OneObject* pOB,int sMR_R,int sNR_R,int sWE_R,int sXY_R=0){
		searchMR_R		= sMR_R;
		MeleeN			= 0;
		RangeN			= 0;
		searchNR_R		= sNR_R;
		pNEAREST		= NULL;
		searchWE_R		= sWE_R;
		pWEEKEST		= NULL;
		searchXY_R		= sXY_R;
		escX			= 0;
		escY			= 0;
		( pOB&&(!pOB->Sdoxlo||pOB->Hidden) ) ? ( UOB=pOB ) : ( UOB=NULL );
		};

	int searchMR_R;			// Радиус поиска для MeleeN и RangeN
	int MeleeN;				// Кол-во юнитов с рукопашной атакой
	int RangeN;				// Кол-во юнитов с стрелковой атакой
	
	int		   searchNR_R;	// Радиус поиска для pNEAREST
	OneObject* pNEAREST;	// Ближайший юнит

	int		   searchWE_R;	// Радиус поиска для pWEEKEST
	OneObject* pWEEKEST;	// Слабейший юнит

	int		   searchXY_R;	// Радиус поиска для escX, escY
	int		   escX;		// Направление отступления по оси X
	int		   escY;		// Направление отступления по оси Y

	OneObject* UOB;			// Юнит для которого собираем информацию

	int MAX_SEARCH_R(){ 
		return max( max( max( searchNR_R, searchWE_R ), searchMR_R), searchXY_R );
	};
};

inline int OB_DIST(OneObject* pOB1, OneObject* pOB2){
	return Norma(pOB1->RealX - pOB2->RealX,pOB1->RealY - pOB2->RealY);
};

bool	cbf_aiLOOK_ROUND(OneObject* OB,void* param){

	aiLOOK_ROUND* LOOK_ROUND = reinterpret_cast<aiLOOK_ROUND*>(param);

	int dist = OB_DIST(OB,LOOK_ROUND->UOB);
	if(OB&&(!OB->Sdoxlo||OB->Hidden)){						
		if ( (OB->NMask & LOOK_ROUND->UOB->NMask) == 0 ){	// Только враги
			if (dist<=LOOK_ROUND->searchMR_R) {				// Проверка типа юнита
				(OB->Ref.General->MoreCharacter->MaxR_Attack<=150) ? (LOOK_ROUND->MeleeN++) : (LOOK_ROUND->RangeN++);
			};
			if (dist<=LOOK_ROUND->searchNR_R) {				// Поиск ближайшего
				if (LOOK_ROUND->pNEAREST==NULL  || dist<OB_DIST(LOOK_ROUND->pNEAREST,LOOK_ROUND->UOB))	
					LOOK_ROUND->pNEAREST = OB;
			};
			if (dist<=LOOK_ROUND->searchWE_R) {				// Поиск слабейшего
				if (LOOK_ROUND->pWEEKEST==NULL || OB->Life<LOOK_ROUND->pWEEKEST->Life) 
					LOOK_ROUND->pWEEKEST = OB;
			};
			if (dist<=LOOK_ROUND->searchXY_R) {
				int normValue = LOOK_ROUND->searchXY_R;
				if ( OB_DIST(OB,LOOK_ROUND->UOB) > 1 ) {
					normValue = LOOK_ROUND->searchXY_R / OB_DIST(OB,LOOK_ROUND->UOB);
				};
				LOOK_ROUND->escX += (LOOK_ROUND->UOB->RealX - OB->RealX) * normValue;
				LOOK_ROUND->escY += (LOOK_ROUND->UOB->RealY - OB->RealY) * normValue;
			};
		};
	};

	return true;
};
//--------------------------------------------------------------------------------//

// aiOBJECT ////////////////////////////////////////////////////////////////////////
bool					aiOBJECT::SetState(aiOBJECT_STATES_ENUM newState){
	newSTATE = newState;
	updateST = true;
	return	true;
};
aiOBJECT_STATES_ENUM	aiOBJECT::GetState(){
	return STATE;
};
bool					aiOBJECT::PROCESS(){
	if (STATE!=aiOSE_CLEAR && updateST==false) {
		if ( !UpdateState() ){
			STATE = aiOSE_CLEAR;
			updateST = true;
		};
	}else if (STATE!=newSTATE) {
		if (STATE==aiOSE_CLEAR && updateST) {
			if ( OnEnterState() ) {
				STATE = newSTATE;
				updateST = false;
				newSTATE = aiOSE_CLEAR;
			};
		}else{
			if ( OnExitState() )  STATE = aiOSE_CLEAR;
		};
	};
	return	true;
};

// aiARCHERS /////////////////////////////////////////////////////////////
void	aiARCHER::UpdateStateStr(){
	char	st0[3];
	char	st1[3];
	GetObjectSt_StrONE(STATE,st0,3);
	GetObjectSt_StrONE(newSTATE,st1,3);
	char	tempST[10];
	if (updateST) {
		sprintf(tempST,"%s%s%s",st0,"->",st1);
	}else{
		sprintf(tempST,"%s%s%s",st0,"-|",st1);
	};
	strSATE = tempST;
};

void	aiARCHER::ShowStateStr(bool show/*=false*/){
	OneObject* pOB = GetUnitOB();
    if (pOB!=NULL && show) {
		UpdateStateStr();

		float	mX = (float)((pOB->RealX)>>4);
		float	mY = (float)((pOB->RealY)>>4);
		float	mZ = (float)GetHeight(mX,mY);
        Vector4D	p(mX,mY,mZ,1);
		WorldToScreenSpace(p);
		
		extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
		static	int dy = -190;
		static	int dx = -15;
		ShowStringEx(p.x+dx,p.y+dy,strSATE.str,&fon18w);
	};
};

void	aiARCHER::SetUnit(word id){
	if (id!=0xFFFF){
		OneObject* OB=Group[id];
		if(OB&&(OB->Hidden||!OB->Sdoxlo)){
			u_id = OB->Index;
			u_sn = OB->Serial;
			OB->NoSearchVictim = true;
		};
	};
};

void	aiARCHER::SetLogic(aiFL_TYPE type){
	u_logicType = type;
};

void	aiARCHER::GetFiling(iaARCHER_FILINGS& filings){
	filings.Clean();
	OneObject* pOB = GetUnitOB();
	if (pOB!=NULL) {
		int maxR_Attack = pOB->Ref.General->MoreCharacter->MaxR_Attack;
		aiLOOK_ROUND	param(pOB,maxR_Attack<<4,(2*maxR_Attack)<<4,maxR_Attack<<4);
		if (param.UOB!=NULL){
			PerformActionOverUnitsInRadius((param.UOB->RealX)>>4,(param.UOB->RealY)>>4,(param.MAX_SEARCH_R())>>4,
											cbf_aiLOOK_ROUND,&param);
			if (param.pNEAREST)					filings.distance = (float)(OB_DIST(param.UOB,param.pNEAREST)>>4);
			if (param.MeleeN+param.RangeN>0)	filings.melePART = (float)param.MeleeN/((float)param.MeleeN+(float)param.RangeN);
												filings.life	 = (float)param.UOB->Life;
		};
	};
	filings.fl_type		= u_logicType;
};

OneObject*	aiARCHER::GetUnitOB(){
	OneObject* OB = NULL;
	if (u_id!=0xFFFF) {
		OB = Group[u_id];
		if (OB&&(OB->Hidden||!OB->Sdoxlo)&&OB->Serial==u_sn) {
			return OB;
		};
		OB = NULL;
	};
	return OB;
};

bool	aiARCHER::OnEnterState(){	// Добиваться newSTATE.
	bool result = true;
	switch(newSTATE) {
	case aiOSE_FREE:
		result=FreeAc_ON();
		break;
	case aiOSE_ATTACK:
		result=Attack_ON();
		break;
	case aiOSE_ESCAPE:
		result=Escape_ON();
		break;
	case aiOSE_DEFENCE:	// -NO-
		result = false;
		break;
	case aiOSE_SEARCH:	// -NO-
		result = false;
		break;
	case aiOSE_CLEAR:	// -> aiOSE_CLEAR
		break;
	case aiOSE_ERROR:	// -> aiOSE_CLEAR
		break;
	default:
		result = false;
		break;
	};
	return result;
};

bool	aiARCHER::UpdateState (){	// Оставаться в  STATE.
	bool result = true;
	switch(STATE) {
	case aiOSE_FREE:
		result=FreeAc_DO();
		break;
	case aiOSE_ATTACK:
		result=Attack_DO();
		break;
	case aiOSE_ESCAPE:
		result=Escape_DO();
		break;
	case aiOSE_DEFENCE:	// -NO-
		result = false;
		break;
	case aiOSE_SEARCH:	// -NO-
		result = false;
		break;
	case aiOSE_CLEAR:	// -> aiOSE_CLEAR
		break;
	case aiOSE_ERROR:	// -> aiOSE_CLEAR
		break;
	default:
		result = false;
		break;
	};
	return result;
};

bool	aiARCHER::OnExitState (){	// Выходить из   STATE.
	bool result = true;
	switch(STATE) {
	case aiOSE_FREE:
		result=FreeAc_OFF();
		break;
	case aiOSE_ATTACK:
		result=Attack_OFF();
		break;
	case aiOSE_ESCAPE:
		result=Escape_OFF();
		break;
	case aiOSE_DEFENCE:	// -NO-
		result = false;
		break;
	case aiOSE_SEARCH:	// -NO-
		result = false;
		break;
	case aiOSE_CLEAR:	// -> aiOSE_CLEAR
		break;
	case aiOSE_ERROR:	// -> aiOSE_CLEAR
		break;
	default:
		result = false;
		break;
	};
	return result;
};

bool	aiARCHER::Attack_ON	(){ 
	bool	retVal = false;
	OneObject* pOB = GetUnitOB();
	if (pOB!=NULL) {
		// Поиск слабейшего в масимальном радиусе обстрела.
		int maxR_Attack = pOB->Ref.General->MoreCharacter->MaxR_Attack;
		aiLOOK_ROUND	param(pOB,maxR_Attack<<4,(2*maxR_Attack)<<4,maxR_Attack<<4);
		PerformActionOverUnitsInRadius((param.UOB->RealX)>>4,(param.UOB->RealY)>>4,(param.MAX_SEARCH_R())>>4,
										cbf_aiLOOK_ROUND,&param);
		if (param.pWEEKEST!=NULL){
			pOB->AttackObj(param.pWEEKEST->Index,128+15,0,0);
			retVal = true;
		};
	};
	return retVal;
};

bool	aiARCHER::Attack_DO	(){ 
	bool	retVal = false;
	OneObject* pOB = GetUnitOB();
	if (pOB!=NULL) {
		if (pOB->EnemyID<0xFFFE && pOB->EnemySN<0xFFFE && pOB->Attack) {
			retVal = true;
		};
	};
	return retVal;
};

bool	aiARCHER::Attack_OFF(){ 
	bool	retVal = false;
	OneObject* pOB = GetUnitOB();
	if (pOB!=NULL) {
		pOB->ClearOrders();
		if (pOB->EnemyID>=0xFFFE && pOB->EnemySN>=0xFFFE && pOB->Attack==false) {
			retVal = true;
		};
	};
	return retVal; 
};

bool	aiARCHER::Escape_ON	(){ 
	bool	retValue = false;
	OneObject* pOB = GetUnitOB();
	int dX,dY;	// topolog coord
	if (pOB!=NULL) {
		if (pOB->DestX<0) {
			int maxR_Attack = pOB->Ref.General->MoreCharacter->MaxR_Attack;
			aiLOOK_ROUND	param(pOB,maxR_Attack<<4,(2*maxR_Attack)<<4,maxR_Attack<<4,(maxR_Attack/2)<<4);
			PerformActionOverUnitsInRadius((param.UOB->RealX)>>4,(param.UOB->RealY)>>4,(param.MAX_SEARCH_R())>>4,
											cbf_aiLOOK_ROUND,&param);
			dX = (param.escX>>4)/64;
			dY = (param.escY>>4)/64;
			if (dX!=0 || dY!=0){
				HashTopTable* pHashTable = &(HashTable[pOB->LockType]);
				if (pHashTable!=NULL) {
					int AreaID = pHashTable->GetTopAreaIndex( (pOB->RealX>>4)/64, (pOB->RealY>>4)/64 );
					Area* pArea = pHashTable->GetTopArea(AreaID);
					if (pArea!=NULL) {
						int			 nLinkInfo = pArea->MaxLink;
						OneLinkInfo* pLinkInfo = pArea->Link;
						if (pLinkInfo!=NULL) {
							Area* pNextArea = NULL;
							int nX,nY;
							int projection = 0;
							Area* nextAteaPTR = NULL;
							for (int i=0; i<nLinkInfo; i++){
								pNextArea = pHashTable->GetTopArea(pLinkInfo[i].NextAreaID);
								if (pLinkInfo[i].NextAreaDist>0) {
									int norma = Norma(pNextArea->x - pArea->x,pNextArea->y - pArea->y);
									if (norma>0) {
										nX = (pNextArea->x - pArea->x) / norma;
										nY = (pNextArea->y - pArea->y) / norma;
										int tProjection = dX*nX + dY*nY;
										if (tProjection>projection) {
											projection = tProjection;
											nextAteaPTR = pNextArea;
										};
									};
								};
							};
							if (nextAteaPTR!=NULL) {
								pOB->NewMonsterPreciseSendTo((nextAteaPTR->x*64)<<4,(nextAteaPTR->y*64)<<4,128+1,0);
								retValue = true;
							};
						};
					};	
				};
			};
		};
	};
	return retValue; 
};

bool	aiARCHER::Escape_DO	(){ 
	bool	retValue = false;
	OneObject* pOB = GetUnitOB();
	if (pOB!=NULL) {
		if (pOB->DestX!=-1 && pOB->DestY!=-1) {
			retValue = true;
		};
	};
	return retValue; 
};

bool	aiARCHER::Escape_OFF(){ 
	bool	retValue = false;
	OneObject* pOB = GetUnitOB();
	if (pOB!=NULL) {
		if (pOB->DestX==-1 && pOB->DestY==-1) {
			retValue = true;
		};
	};
	return retValue; 
};

bool	aiARCHER::FreeAc_ON	(){ 
	return true; 
};

bool	aiARCHER::FreeAc_DO	(){ 
	return false; 
};

bool	aiARCHER::FreeAc_OFF(){
	bool	retVal = false;
	OneObject* pOB = GetUnitOB();
	if (pOB!=NULL) {
		pOB->ClearOrders();
		retVal = true;
	};
	return retVal; 
};

// aiARCHERS_GRP /////////////////////////////////////////////////////////
extern	ClassArray<ActiveGroup> AGroups;
void	aiARCHERS_GRP::SetGrp(DWORD grpID,aiARCHER_LOGIC* pLogic){
	Clean();
	pLOGIC = pLogic;
	// Set GRP in CLASS
	if (grpID<AGroups.GetAmount()) {
		int Nu=AGroups[grpID]->Units.GetAmount();
		aiARCHER* pArcher = NULL;
		while (Nu--) {
			pArcher = new aiARCHER();
			pArcher->SetUnit(AGroups[grpID]->Units[Nu].ID);
			ArchersGrp.Add(pArcher);
			pArcher = NULL;
		};
	};
};

void	aiARCHERS_GRP::Clean(){
	int N = ArchersGrp.GetAmount();
	while (N--) {
		if (ArchersGrp[N]!=NULL) {
			delete ArchersGrp[N];
			ArchersGrp[N]=NULL;
		};
	};
	ArchersGrp.Clear();
};

void	aiARCHERS_GRP::PROCESS(){
	iaARCHER_FILINGS	FILING;
	int N = ArchersGrp.GetAmount();
	if (N==0 || pLOGIC == NULL )	return;

	while (N--) {
		if (ArchersGrp[N]->GetState()==aiOSE_CLEAR) {
			ArchersGrp[N]->GetFiling(FILING);
			ArchersGrp[N]->SetState(CovertArcherSt_to_ObjectSt(pLOGIC->getFL_STATE(FILING)));
		};
		ArchersGrp[N]->PROCESS();
	};
};

void	aiARCHERS_GRP::ShowState(){
	if (showST) {
		int N = ArchersGrp.GetAmount();
		if (N==0 || pLOGIC == NULL )	return;

		while (N--) {
			ArchersGrp[N]->ShowStateStr(showST);
		};
	};
};

//////////////////////////////////////////////////////////////////////////























