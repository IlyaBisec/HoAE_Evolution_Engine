 
#include "MainEngine.h"

#define		QUEST_NATION0	1
#define		QUEST_NATION1	2
#define		QUEST_NATION2	4


//------------------class registration---------------------------
void	REG_ALL_CLASS(){
	REG_CLASS(CKillEnemy00SPROC);
}
void	UNREG_ALL_CLASS() {
	UNREG_CLASS("CKillEnemy00SPROC");
}
//-------------------------------------------------------------------------------------------------------------
byte	GetQuestsNation(int ai_ni, int player_ni) {
	return	4;
	//if ( QUEST_NATION2!=ai_ni && QUEST_NATION2!=player_ni )  return  QUEST_NATION2;
	//if ( QUEST_NATION1!=ai_ni && QUEST_NATION1!=player_ni )  return  QUEST_NATION1;
	//return	QUEST_NATION0;
}
byte	GetTrapsSquadLimit(int player_SQ_NUM, int dif) {
	if( player_SQ_NUM > 4 )	
		return  dif==0? 1 : ( player_SQ_NUM/4 + player_SQ_NUM/(15-3*dif) ) ;
	return	0;
}
int		GetSquadsAmount( int nation ) {
	return  GetBrigadsAmount0( nation ) + GetCannonsAmount( nation );
}
char* GiveMeCoolAttackerSquadType( int nation ) { //grey [10.08.2004]
	char* strID = NULL;
	int   sqNN  = InitData.AttackerForce.GetAmountOfElements();
	for(int i=0; i < sqNN ; i++)
		if( GetObjectBranch( InitData.AttackerForce[ i ]->TypeIndex_ID ) != 2 )  
			strID = InitData.AttackerForce[ i ]->TypeName_sID.str;
	return  strID;
}

bool	CheckAddExperience( int nation, int AddKills, int cond, int param ) //grey [17.12.2004]
{
	int prob = rand()%100;
	if( 33 < prob && prob < 70 ) { // 40%
		for(int i=0; i < GetCITYNBrigs( nation ) ; i++)
			SetBrigadeNewMorale( nation , i , 0xAAAA , AddKills );

		char  textID[256];	
		int   messId = (rand()%10)/4 +1 ; //0-1-2 +1=1..k3
		sprintf(textID, "#AddLowExp2SQ%d", messId );
		AddMessageIntoMessCampCII( "#RessPage", GetTextByID(textID) , 0xAAA ,-1,-1,-1,-1, "Ress" );
		//ShowPageParam("#InfoPage", GetTextByID(textID) );

		return  true;
	}
	return  false;
}
//int		ProcessRandomConds() { //grey [15.12.2004]
//	int  randtype = rand()%100 ;
//	
//	if( 20 < randtype && randtype < 40 ) {
//		return  0 ;
//	}
//	if( 40 < randtype && randtype < 60 ) {
//		return  1 ;
//	}
//	if( 60 < randtype ) {
//		return  2 ;
//	}
//
//	return 0;
//}

//-------------------------------------------------------------------------------------------------------------
//-------------------------set---------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void	setResource( byte NI, int wood, int food, int stone, int gold, int iron, int coal ){
	if ( 0<=NI && NI<=6 ) {
		SetResource( NI, WOOD,  wood  );
		SetResource( NI, GOLD,  gold  );
		SetResource( NI, STONE, stone );
		SetResource( NI, FOOD,  food  );
		SetResource( NI, IRON,  iron  );	
		SetResource( NI, COAL,  coal  );
	}
}
//-------------------------------------------------------------------------------------------------------------
void	setStartPoint( int x, int y ){
	GAMEOBJ	zStartPiont;	
	zStartPiont.Type = 0x40000000;
	zStartPiont.Index	= x;
	zStartPiont.Serial	= y;
	SetStartPoint(&zStartPiont);
}
//------------------------create---------------------------------
//-------------------------------------------------------------------------------------------------------------
void	CreateHeroArmy( byte NI, ClassArray<CTYPENUM>* pARM, SXYF* pPOS ){
	//CreateHeroArmy(NULL,NI,pARM,pPOS->X,pPOS->Y,pPOS->F);
	CreateHeroArmy1(NULL,NI,pARM,pPOS->X,pPOS->Y,pPOS->F);
}
//-------------------------------------------------------------------------------------------------------------
int		findeMaxNSQ( ClassArray<CTYPENUM>* pARM ){
	if ( pARM==NULL || pARM->GetAmount()==0 )	return -1;
	int maxNSQ = ((*pARM)[0])->nSquarnd;
	int id	   = 0;
	int curNSQ = 0;
	for (int i=1; i<pARM->GetAmount(); i++){
		curNSQ = ((*pARM)[i])->nSquarnd;
		if (curNSQ>maxNSQ){
			maxNSQ=curNSQ;
			id=i;
		}
	}
	return id;
}
//-------------------------------------------------------------------------------------------------------------
int		findeIDbyTYPEid(ClassArray<CTYPENUM>* pTN, int id){ //
	int rid = -1;
	if (pTN!=NULL && pTN->GetAmount()>0){
		for (int i=0; (i<pTN->GetAmount())&&(rid==-1); i++){
			if (((*pTN)[i])->TypeIndex_ID==id){
				rid=i;
			}
		}
	}
	return	rid;
} //
//-------------------------------------------------------------------------------------------------------------

void	removeSame(ClassArray<CTYPENUM>* pBig, ClassArray<CTYPENUM>* pSml){ //
	if (pBig==NULL || pSml==NULL || pBig->GetAmount()==0 || pSml->GetAmount()==0 )	return;
	int cid = 0;
	for(int i=0; i < pBig->GetAmount() ; i++)
	{
		int NN = 0;
		for(int sm=0; sm < pSml->GetAmount(); sm++)
		{
			if ( ((*pBig)[i])->TypeIndex_ID == ((*pSml)[ sm ])->TypeIndex_ID ){
				NN = ((*pSml)[ sm ])->nSquarnd;
				if( ((*pBig)[i])->nSquarnd >= ((*pSml)[ sm ])->nSquarnd ){
					((*pBig)[i])->nSquarnd		-= ((*pSml)[ sm ])->nSquarnd;
					((*pSml)[ sm ])->nSquarnd    = 0;
				}
				else{
					((*pSml)[ sm ])->nSquarnd	-= ((*pBig)[i])->nSquarnd;
					((*pBig)[i])->nSquarnd		 = 0;
				}
			}
		}

		//cid = findeIDbyTYPEid(pSml,((*pBig)[i])->TypeIndex_ID);
		//if (cid!=-1){
		//	((*pBig)[i])->nSquarnd -= ((*pSml)[cid])->nSquarnd;
	}
} //

//void	removeSame(ClassArray<CTYPENUM>* pBig, ClassArray<CTYPENUM>* pSml){ //
//	if (pBig==NULL || pSml==NULL || pBig->GetAmount()==0 || pSml->GetAmount()==0 )	return;
//	int cid = 0;
//	for (int i=0; i<pBig->GetAmount(); i++){
//		cid = findeIDbyTYPEid(pSml,((*pBig)[i])->TypeIndex_ID);
//		if (cid!=-1){
//			((*pBig)[i])->nSquarnd -= ((*pSml)[cid])->nSquarnd;
//		}
//	}
//} //

//-------------------------------------------------------------------------------------------------------------
void	addTNbyRATE(ClassArray<CTYPENUM>* pFROM, ClassArray<CTYPENUM>* pTO, int rate){
	if ( pFROM==NULL || pTO==NULL || rate==0 || pFROM->GetAmount()==0 )	return;
	CTYPENUM*	pNewTN = NULL;
	for ( int i=0; i<pFROM->GetAmount(); i++ ){
		pNewTN = new CTYPENUM(	((*pFROM)[i])->TypeName_sID.str,
								((*pFROM)[i])->TypeIndex_ID,
								getNRate(((*pFROM)[i])->nSquarnd,rate)	);
		pTO->Add(pNewTN);
		pNewTN = NULL;
	}
}
//-------------------------------------------------------------------------------------------------------------
void	getNfromBigest(ClassArray<CTYPENUM>* pFrom, ClassArray<CTYPENUM>* pTO, int rate){
	if ( pFrom==NULL || pTO==NULL || pFrom->GetAmount()==0 )	return;
	int curID = findeMaxNSQ(pFrom);
	if (curID==-1)	return;
	int curRate = rate;
	if ( ((*pFrom)[curID])->nSquarnd < rate )	curRate = ((*pFrom)[curID])->nSquarnd;
	CTYPENUM* pNewTN = new CTYPENUM(	((*pFrom)[curID])->TypeName_sID.str,
										((*pFrom)[curID])->TypeIndex_ID,
										curRate							);
	((*pFrom)[curID])->nSquarnd -= curRate;
	pTO->Add(pNewTN);
}
//-------------------------------------------------------------------------------------------------------------
void	sortMaxToMin(ClassArray<CTYPENUM>* pCA){
	int		N = pCA->GetAmount();
	if (N<=1)	return;
    
	bool		stop	= true;
	do {
		stop=true;
        for (int i=0; i<N-1; i++){
			if ( ((*pCA)[i])->nSquarnd < ((*pCA)[i+1])->nSquarnd ) {
				pCA->MoveElement(i,1);
			}			
		}
	} while (!stop);
}
//-------------------------------------------------------------------------------------------------------------
void	sortMinToMax(ClassArray<CTYPENUM>* pCA){
	int		N = pCA->GetAmount();
	if (N<=1)	return;
    
	bool		stop	= true;
	do {
		stop=true;
        for (int i=0; i<N-1; i++){
			if ( ((*pCA)[i])->nSquarnd > ((*pCA)[i+1])->nSquarnd ) {
				pCA->MoveElement(i,1);
			}			
		}
	} while (!stop);
}
//-------------------------------------------------------------------------------------------------------------
void	createDefenderArmy(byte NI,SARMY_DESCRIPTION* pAA,SARMY_DESCRIPTION* pDA,ClassArray<SXYF>* pP,ClassArray<SXYF>* pC,ClassArray<SXYF>* pR,ClassArray<CTYPENUM>* pDAR){
	int nT = 0;	// ол-во войск защитника. -1 защитников менише, 0 поровну, 1 защитников больше. 
	int nM = 0;	//ћобильность войск защитника. -1 нападающие мобильней, 0 одинаковые, 1 оборон€ющиес€ мобильней. 
	if ((*pDA)<(*pAA))		nT = -1;
	else if ((*pDA)>(*pAA))	nT =  1;

	if (pDA->MOBILITY<((float)pAA->MOBILITY*0.70))		nM = -1;
	else if (pDA->MOBILITY>((float)pAA->MOBILITY*1.30))	nM =  1;
	
	//Share Kavalery and Trupers.
	ClassArray<CTYPENUM>	Pehota;
	ClassArray<CTYPENUM>	Kavaleriya;
	CTYPENUM*	pteSQ = NULL;
	for (int i=0; i<pDAR->GetAmount(); i++){
		pteSQ = new CTYPENUM();
		pteSQ->TypeName_sID = ((*pDAR)[i])->TypeName_sID.str;
		pteSQ->TypeIndex_ID = ((*pDAR)[i])->TypeIndex_ID;
        pteSQ->nSquarnd		= ((*pDAR)[i])->nSquarnd;
		if (GetObjectBranch(pteSQ->TypeName_sID.str)==1)	Kavaleriya.Add(pteSQ);
		else												Pehota.Add(pteSQ);
		pteSQ = NULL;
	}	
	
	ClassArray<CTYPENUM>	curUnits;		//“екущие юниты которые мы будем ставить.
	CTYPENUM*				pNewTE = NULL;
	if ( nT== 0 ) {
		//---------------------------------------------------------------------------
		//ѕо 1-2 отр€ду, самых многочисленных войск по зонам прикрыти€.	CONTROL
		//---------------------------------------------------------------------------
		int ncov = 1;
		if (Pehota.GetAmount()>=20)	ncov = 2;
		for ( int r=0; r<pC->GetAmount(); r++ ){
			getNfromBigest(&Pehota,&curUnits,rand()%ncov+1);
			CreateHeroArmy(NI,&curUnits,(*pC)[r]);	deleteCTYPENUMarr(&curUnits);
		}
		//---------------------------------------------------------------------------
		//–асстановка конницы.											RESERVE
		//---------------------------------------------------------------------------
		int curRate = 0;
		int KavRate = 0;
		if ( nM== 0 ) {
			KavRate = 70;
			}else
				if ( nM==-1 ) {
					KavRate = 100;
					}else
						if ( nM== 1 ) {
							KavRate = 50;
							}
		if ( pDA->SQ_K_NUM>0 && pR->GetAmount()>0 ){
			curRate = KavRate/(pR->GetAmount());
			addTNbyRATE(&Kavaleriya,&curUnits,curRate);
			for (int r=0; r<pR->GetAmount(); r++){
				CreateHeroArmy(NI,&curUnits,(*pR)[r]);	
				removeSame(&Kavaleriya,&curUnits);
			}
			deleteCTYPENUMarr(&curUnits);
		}
		//---------------------------------------------------------------------------
		//¬сех остальных на главные оборонительные позиции.				PRIMARY
		//---------------------------------------------------------------------------
		curRate = 100/(pP->GetAmount());
		if (pP->GetAmount()>1) {
			addTNbyRATE(&Pehota,	&curUnits,curRate);
			addTNbyRATE(&Kavaleriya,&curUnits,curRate);
		}
		for ( r=0; r<pP->GetAmount(); r++ ){
			if (r == pP->GetAmount()-1){
				addTNbyRATE(&Pehota,	&curUnits,100);
				addTNbyRATE(&Kavaleriya,&curUnits,100);
			}
			removeSame(&Kavaleriya,&curUnits);		removeSame(&Pehota,&curUnits);
			CreateHeroArmy(NI,&curUnits,(*pP)[r]);	deleteCTYPENUMarr(&curUnits);
		}
		//---------------------------------------------------------------------------
	}else
	if ( nT==-1 ) {
		//---------------------------------------------------------------------------
		//≈сли достаточно кавалерии - прикрыть зоны контрол€.			CONTROL
		//---------------------------------------------------------------------------
		if ( (nM==0 || nM==1) && pDA->SQ_K_NUM>0 ){
			for ( int r=0; r<pC->GetAmount(); r++ ){
				getNfromBigest(&Kavaleriya,&curUnits,1);
				CreateHeroArmy(NI,&curUnits,(*pC)[r]);	deleteCTYPENUMarr(&curUnits);
			}
		}
		//---------------------------------------------------------------------------
		//≈сли много кавалерии - ставим всю в резерв.					RESERVE
		//---------------------------------------------------------------------------
		if ( (nM==1) && pDA->SQ_K_NUM>0 ) {
			addTNbyRATE(&Kavaleriya,&curUnits,100/(pR->GetAmount()));
			for (int r=0; r<pR->GetAmount(); r++){
				CreateHeroArmy(NI,&curUnits,(*pR)[r]);	
				removeSame(&Kavaleriya,&curUnits);
			}
			deleteCTYPENUMarr(&curUnits);
		}
		//---------------------------------------------------------------------------
		//¬сех остальных на какуюто одну оборонительную позицию.		PIMARY
		//---------------------------------------------------------------------------
		addTNbyRATE(&Pehota,	&curUnits,100);
		addTNbyRATE(&Kavaleriya,&curUnits,100);

		CreateHeroArmy(NI,&curUnits,(*pP)[rand()%(pP->GetAmount())]);	deleteCTYPENUMarr(&curUnits);
		//---------------------------------------------------------------------------
	}else
	if ( nT== 1 ) {
		//---------------------------------------------------------------------------
		//ѕо 2-3 пехотных отр€да на зоны контрол€. + 1 кавалери€.		CONTROL
		//---------------------------------------------------------------------------
		for ( int r=0; r<pC->GetAmount(); r++ ){
			getNfromBigest(&Pehota,&curUnits,rand()%2+2);
			if (nM==1||nM==0){
				getNfromBigest(&Kavaleriya,&curUnits,1);
			}
			CreateHeroArmy(NI,&curUnits,(*pC)[r]);	deleteCTYPENUMarr(&curUnits);
		}
		//---------------------------------------------------------------------------
		//≈сли арми€ мобильна то конницу в резерв.						RESERV
		//---------------------------------------------------------------------------
		int nP = 0;
		if (nM==0)	nP=50;
		if (nM==1)	nP=100;
		if (nM==0||nM==1){
			addTNbyRATE(&Kavaleriya,&curUnits,nP/pR->GetAmount());
			for (int r=0; r<pR->GetAmount(); r++){
				CreateHeroArmy(NI,&curUnits,(*pR)[r]);	
				removeSame(&Kavaleriya,&curUnits);
			}
			deleteCTYPENUMarr(&curUnits);
		}
		//---------------------------------------------------------------------------
		//¬сех остальных на главные оборонительные позиции.				PRIMARY
		//---------------------------------------------------------------------------
		int	curRate = 100/(pP->GetAmount());
		if (pP->GetAmount()>1) {
			addTNbyRATE(&Pehota,	&curUnits,curRate);
			addTNbyRATE(&Kavaleriya,&curUnits,curRate);
		}
		for ( r=0; r<pP->GetAmount(); r++ ){
			if (r == pP->GetAmount()-1){
				addTNbyRATE(&Pehota,	&curUnits,100);
				addTNbyRATE(&Kavaleriya,&curUnits,100);
			}
			removeSame(&Kavaleriya,&curUnits);		removeSame(&Pehota,&curUnits);
			CreateHeroArmy(NI,&curUnits,(*pP)[r]);	deleteCTYPENUMarr(&curUnits);
		}
		//---------------------------------------------------------------------------
	}
	deleteCTYPENUMarr(&Kavaleriya);
	deleteCTYPENUMarr(&Pehota);
}
//-------------------------------------------------------------------------------------------------------------
void	createAttackerArmy( byte NI, SARMY_DESCRIPTION* pAA,
						   ClassArray<SXYF>* pP, ClassArray<SXYF>* pC, ClassArray<SXYF>* pR, ClassArray<SXYF>* pArt , 
						   ClassArray<CTYPENUM>* pDAR, int Real_NI )
{
	int nP = pAA->SQ_P_NUM;		//  ол-во пехотных отр€дов атакующего.
	int nK = pAA->SQ_K_NUM;		//  ол-во конных отр€дов атакующего.
	int nA = pAA->CANNON_NUM;	//  ол-во пушек атакующего.

	int nPr = nP + nK + nA;		// кол-во монахов.

	//Share Kavalery and Trupers.
	ClassArray<CTYPENUM>	Pehota;			Pehota.Clear();
	ClassArray<CTYPENUM>	Kavaleriya;		Kavaleriya.Clear();
	ClassArray<CTYPENUM>	Artillery;		Artillery.Clear();
	CTYPENUM*	pteSQ = NULL;
	for (int i=0; i<pDAR->GetAmount(); i++){
		pteSQ = new CTYPENUM();
		pteSQ->TypeName_sID = ((*pDAR)[i])->TypeName_sID.str;
		pteSQ->TypeIndex_ID = ((*pDAR)[i])->TypeIndex_ID;
		pteSQ->nSquarnd		= ((*pDAR)[i])->nSquarnd;
		if ( GetObjectBranch(pteSQ->TypeName_sID.str) == 0 )	Pehota.Add(pteSQ);
		if ( GetObjectBranch(pteSQ->TypeName_sID.str) == 1 )	Kavaleriya.Add(pteSQ);		
		if ( GetObjectBranch(pteSQ->TypeName_sID.str) == 2 )	Artillery.Add(pteSQ);
		//if (GetObjectBranch(pteSQ->TypeName_sID.str)==1)	Kavaleriya.Add(pteSQ);		//else												Pehota.Add(pteSQ);		pteSQ = NULL;
	}
	ClassArray<CTYPENUM>	curUnits;		//“екущие юниты которые мы будем ставить.
	CTYPENUM*				pNewTE = NULL;

	//*******************************************************************************
	//---------------------------------------------------------------------------
	// ѕо 1 пехотных отр€да на зоны контрол€.						CONTROL
	//---------------------------------------------------------------------------
	{
		int nC = pC->GetAmount();
		if (nP>0&&nC>0) {
			int nPC = (int)((float)nP*0.2f);
			if (nPC>0){
				int rate= (int)((float)nPC/(float)nC);
				if (rate==0)	rate=1;
				for ( int i=0; (rate>0)&&(i<nC); i++ ){
					if (i==nC-1) {
						rate = nPC;
					}
					if (rate>0){
						getNfromBigest(&Pehota,&curUnits,rate);
						CreateHeroArmy(NI,&curUnits,(*pC)[i]);
						deleteCTYPENUMarr(&curUnits);
					}
					nPC -= rate;
				}
				if (rate==0){
					getNfromBigest(&Pehota,&curUnits,nPC);
					CreateHeroArmy(NI,&curUnits,(*pC)[rand()%nC]);
					deleteCTYPENUMarr(&curUnits);
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	// ѕри любых раскладах - конницу в резерв.						RESERV
	//---------------------------------------------------------------------------
	{
		int nR = pR->GetAmount();
		if (nR>0&&nK>0) {
			int rate = (int)(ceil((double)nK/(double)nR)+0.1);
						
			for (int r=0; r<nR; r++){ 
                sortMaxToMin(&Kavaleriya);
				int rateC = rate;
				for (int i=0;(i<Kavaleriya.GetAmount())&&(rateC>0); i++){
					if ((Kavaleriya[i])->nSquarnd>0) {
						int minC = MinLV(rateC,(Kavaleriya[i])->nSquarnd);
						pNewTE = new CTYPENUM(	(Kavaleriya[i])->TypeName_sID.str,
												(Kavaleriya[i])->TypeIndex_ID,
												minC);
						(Kavaleriya[i])->nSquarnd	-= minC;
						rateC						-= minC;
						curUnits.Add(pNewTE);
						pNewTE=NULL;
					}
				}
				CreateHeroArmy(NI,&curUnits,(*pR)[r]);
				deleteCTYPENUMarr(&curUnits);
			}

		}
	}
	//---------------------------------------------------------------------------
	// ¬сех остальных на главные оборонительные позиции.			PRIMARY
	//---------------------------------------------------------------------------
	if (pP->GetAmount()!=0)	//{	//if (pP->GetAmount()==0)	return;
	{
		int	curRate = 100/(pP->GetAmount());
		if (pP->GetAmount()>1) {
			addTNbyRATE(&Pehota,	&curUnits,curRate);
			addTNbyRATE(&Kavaleriya,&curUnits,curRate);
		}
		for ( int r=0; r<pP->GetAmount(); r++ ){
			if (r == (pP->GetAmount())-1){
				deleteCTYPENUMarr(&curUnits);
				addTNbyRATE(&Pehota,	&curUnits,100);
				addTNbyRATE(&Kavaleriya,&curUnits,100);
			}
			CreateHeroArmy(NI,&curUnits,(*pP)[r]);
			removeSame(&Kavaleriya,&curUnits);		
			removeSame(&Pehota,&curUnits);
		}
		deleteCTYPENUMarr(&curUnits);	
	}


	//---------------------------------------------------------------------------
	// расст.пушки на позиции.										ARTILLERY
	//---------------------------------------------------------------------------
	int Cannons[100] ;									// ѕ”Ў »
	int CannonN		= 0 ;								// тек. кол-во пушек 
	for(int i=0; i<100; i++) Cannons[i] = -1 ;

	int ArtZoneNum	= pArt->GetAmount() ;				// кол-во зон дл€ пушек на карте
	int	CannonsNum	= 0 ;								// кол-во пушек у геро€
	for(int i=0; i < Artillery.GetAmount() ; i++) {
		for(int c=CannonsNum; c < CannonsNum + Artillery[i]->nSquarnd ; c++) Cannons[c] = Artillery[i]->TypeIndex_ID; 
		CannonsNum += Artillery[i]->nSquarnd ;
	}

	if( ArtZoneNum > 0 && CannonsNum > 0 ) 
	{
		int	NumAtZone = CannonsNum/ArtZoneNum ;			// кол-во пушек в одной зоне

		int		dX    = 210 ;							// расст. между 2м€ пушками на карте
		int		dirF  = 0;
		float	fi	  = 0 ,   cosF = 0 ,   sinF = 0 ,   delta = 0 ;

		for(int i=0; i < ArtZoneNum && CannonN < CannonsNum ; i++ ) { // Zones 
			dirF	= (*pArt)[ i ]->F ;
			fi		= ( dirF * 6.2832 ) / 256 ;	
			cosF	= cos(fi);
			sinF	= sin(fi);

			delta	= dX / 1.4142 ;

			int	dX1 =   delta * sinF ;
			int	dY1 = - delta * cosF ;

			int	NN = NumAtZone/2 ;				/*+ NumAtZone%2*/

			int	X0 = (*pArt)[ i ]->X ;		
			int Y0 = (*pArt)[ i ]->Y ;
			
			X0 = X0 - dX1 * NN ;
			Y0 = Y0 - dY1 * NN ;

			for(int art=0; CannonN < CannonsNum/(ArtZoneNum - i) && CannonN < CannonsNum ; art++ ) { // Add cannons
				if( Cannons[ CannonN ] > 0 ) {
					GAMEOBJ UNT;	UNT.Type  = 'UNIT';
					GAMEOBJ TYPE;	TYPE.Type = 'UTYP';   TYPE.Index = Cannons[ CannonN ];   TYPE.Serial = 0;
					GAMEOBJ Z;		Z.Type	  = 0x40000000+250;	  Z.Index = X0 ;   Z.Serial = Y0 ;
					CreateObject0(	&UNT, &fALONE_LV, &TYPE, NI , &Z, dirF ) ;
					ClearSelection( NI ); SelectUnits(&UNT, 0); SelSendTo(NI, &Z, dirF, 0); ClearSelection( NI ); 
					X0 += dX1 ;
					Y0 += dY1 ;
				}
				CannonN++ ; 
			}
		}
	}

	//---------------------------------------------------------------------------
	// расст.монахов.													PRIESTS
	//---------------------------------------------------------------------------
	char* UnType[10] = { "UnitPop(EN)", "UnitPop(FR)", "UnitPop(AU)", 
						 "UnitPop(PR)", "UnitPop(RU)", "UnitPop(EG)", 0, 0, 0, 0 };


	if( nPr > 0 && pP->GetAmountOfElements() > 0  ) 
	{
		float delta = 600 / 1.4142 ;// 400 / 
		int    dirF = (*pP)[0]->F ;
		float    fi = ( dirF * 6.2832 ) / 256 ;	

		int	 X0   = (*pP)[0]->X ;		
		int  Y0   = (*pP)[0]->Y ;

		int	 dX1  =  - delta * cos(fi) ;
		int	 dY1  =  - delta * sin(fi) ;

		X0 += dX1 ;
		Y0 += dY1 ;

		GAMEOBJ UNT;	UNT.Type  = 'UNIT';
		GAMEOBJ TYPE;	RegisterUnitType( &TYPE, UnType[ Real_NI ] );
		GAMEOBJ Z;		Z.Type	  = 0x40000000+250;	  Z.Index = X0 ;   Z.Serial = Y0 ;

		for(int i=0; i < nPr ; i++ ) {
			CreateObject0( &UNT, &fALONE_LV, &TYPE, NI, &Z, dirF ) ;  // Add one
		}
		ClearSelection( NI ); SelectUnitsType(&TYPE, NI, 0); SelSendTo(NI, &Z, dirF, 0); //ClearSelection( NI ); 
	}
	//if( ArtZoneNum > 0 && nPr > 0 ) 
	//{
	//	int  iZ = 0;
	//	if( ArtZoneNum > 2 ) iZ = ArtZoneNum/2;

	//	float delta = 400 / 1.4142 ;
	//	int    dirF = (*pArt)[ iZ ]->F ;
	//	float    fi = ( dirF * 6.2832 ) / 256 ;	

	//	int	 X0   = (*pArt)[ iZ ]->X ;		
	//	int  Y0   = (*pArt)[ iZ ]->Y ;

	//	int	 dX1  =  - delta * cos(fi) ;
	//	int	 dY1  =  - delta * sin(fi) ;

	//	X0 += dX1 ;
	//	Y0 += dY1 ;

	//	GAMEOBJ UNT;	UNT.Type  = 'UNIT';
	//	GAMEOBJ TYPE;	RegisterUnitType( &TYPE, UnType[ Real_NI ] );
	//	GAMEOBJ Z;		Z.Type	  = 0x40000000+250;	  Z.Index = X0 ;   Z.Serial = Y0 ;

	//	for(int i=0; i < nPr ; i++ ) {
	//		CreateObject0( &UNT, &fALONE_LV, &TYPE, NI, &Z, dirF ) ;  // Add one
	//	}
	//	ClearSelection( NI ); SelectUnitsType(&TYPE, NI, 0); SelSendTo(NI, &Z, dirF, 0); //ClearSelection( NI ); 
	//}
	//-------------

	//*******************************************************************************
	deleteCTYPENUMarr(&Kavaleriya);
	deleteCTYPENUMarr(&Pehota);
	deleteCTYPENUMarr( &Artillery );//grey
}
//-------------------------------------------------------------------------------------------------------------
void	createAttackerArmy(CSAVE_MIS_STRUCT* pIMD)
{
	//ARMY DESCRIPTION
	SARMY_DESCRIPTION	adATT(&(pIMD->AttackerForce));
	//POSITION DESCRIPTION
	_str	sDir; 
	sDir.operator=( pIMD->getDirection_sID() );
	ClassArray<SXYF> Prm;
	ClassArray<SXYF> Cnt;
	ClassArray<SXYF> Rsv;
	ClassArray<SXYF> Art;//grey
	getAttackerPos( &Prm, &Cnt, &Rsv, &Art, sDir );
	//SET ATTACKER ARMY ON MAP
	createAttackerArmy( pIMD->AttackerNation_ID, &adATT, &Prm, &Cnt, &Rsv, &Art, &(pIMD->AttackerForce), Real_Attacker_NI );
	//CLEAR POSITION
	Prm.Clear();	//deleteSXYFarr(&Prm);
	Cnt.Clear();	//deleteSXYFarr(&Cnt);
	Rsv.Clear();	//deleteSXYFarr(&Rsv);
	Art.Clear();	
}
//-------------------------------------------------------------------------------------------------------------
void	createDefenderArmy(CSAVE_MIS_STRUCT* pIMD)
{
	//ARMY DESCRIPTION
	SARMY_DESCRIPTION	adDFF(&(pIMD->DefenderForce));
	//POSITION DESCRIPTION
	_str	sDir; 
	sDir.operator=( pIMD->getDirection_sID() );
	ClassArray<SXYF> Prm;
	ClassArray<SXYF> Cnt;
	ClassArray<SXYF> Rsv;
	ClassArray<SXYF> Art;//grey
	getDefenderPos( &Prm, &Cnt, &Rsv, &Art, sDir );
	//SET DEFENDER ARMY ON MAP
	createAttackerArmy( pIMD->DefenderNation_ID, &adDFF, &Prm, &Cnt, &Rsv, &Art, &(pIMD->DefenderForce), Real_Defender_NI );
	//CLEAR POSITION
	Prm.Clear();	//deleteSXYFarr(&Prm);
	Cnt.Clear();	//deleteSXYFarr(&Cnt);
	Rsv.Clear();	//deleteSXYFarr(&Rsv);
	Art.Clear();	
}
//-------------------------------------------------------------------------------------------------------------
bool	createCamp(byte NI,GAMEOBJ* pT,GAMEOBJ* pF, GAMEOBJ* pDZ, int dir, GAMEOBJ* pG){
	if (NI<0||NI>7||pT==NULL||pF==NULL||pDZ==NULL||pG==NULL) {
		return false;
	}
	GAMEOBJ	tempGRP;
//	RegisterDynGroup(&tempGRP);
	int curN = GetTotalAmount0(pG);
	int i=1;
	double	Fi = ((double)(dir%256)/256.0)*2.0*3.1415;
	GAMEOBJ	z;	z.Index = pDZ->Index; z.Serial = pDZ->Serial; z.Type = 0x40000000+500;
	while (GetTotalAmount0(pG)==curN&&i<30) {
		z.Index += (int)(250.0*(double)i*cos(Fi));	
		z.Serial+= (int)(250.0*(double)i*sin(Fi));
		CreateObject0(&tempGRP,pF,pT,NI,&z,0);
		if (GetTotalAmount0(&tempGRP)==1){
			RemoveGroup(&tempGRP,pG);
		}
		DeleteDynGroup(&tempGRP);
		i++;
	}
	if (GetTotalAmount0(pG)==curN) {
		return false;
	}
	pDZ->Index = z.Index;
	pDZ->Serial= z.Serial;
	return true;
}
//-------------------------------------------------------------------------------------------------------------
void	createCamp(byte NI,GAMEOBJ* pTYPE,GAMEOBJ* pF,int n,GAMEOBJ* pZONE,GAMEOBJ* pGrp){
	if (NI<0||NI>7||pTYPE==NULL||pF==NULL||n==0||pZONE==NULL||pGrp==NULL) {
		return;
	}

	GAMEOBJ	DZ;		DZ.Type = 0x40000000+500;
	if ((pZONE->Type&0xFF000000)==0x40000000){	//ƒинамическа€.
		DZ.Index = pZONE->Index;
		DZ.Serial= pZONE->Serial;
	}else{										//—татическа€.
		int x,y;
		GetZoneCoor(pZONE,&x,&y);
		DZ.Index = x;
		DZ.Serial= y;
	}
	int DR = 0;
	int i=0,ii=0;
	int curSET = 1;
	int ready = 0;
	while (i<n&&ii<n*n){
		if (createCamp(NI,pTYPE,pF,&DZ,DR,pGrp)){
			i++;
			if (curSET*3==abs(i-ready)) {
				ready += curSET*3;
				curSET++;
			}
			if (abs(i-ready)%curSET==0&&i-ready!=0){
				DR += 64;	
			}
		}
		ii++;
	}
}
//-------------------------------------------------------------------------------------------------------------
void	createCamp(byte NI,GAMEOBJ* pZ,int N_SQ) {
	int myNI = GetPlayerNation();	//ѕотом поставить дл€ разных наций и разнх количеств.
	SetPlayerNation(NI);
	//PastePiece(pZ,"userpieces\\RusKaz.smp");
	int   nn = (N_SQ+3)/4;	
	if( nn < 1 )  nn = 1;		if( nn > 5 )  nn = 5;
	char  name[256];
	sprintf(name, "pieces\\Tents%d.smp", nn);
	PastePiece(pZ, name);
	//PastePiece(pZ,"pieces\\Tents1.smp");
	SetPlayerNation(myNI);
} //-------------------------------------------------------------------------------------------------------------
void	createCamp(CSAVE_MIS_STRUCT* pIMD, _str& dir){
	int				zN	= GetAmountOfZones();
	CAMP_LV*		pZD = NULL;
	Enumerator*	SDIR_ENUM			= ENUM.Get("SDIR_ENUM");
	GAMEOBJ	z; z.Type=0x40000000+500;
	if ( SDIR_ENUM->GetAmount() ) {
		for ( int i=0; i<zN; i++ ){
			pZD = reinterpret_cast<CAMP_LV*>(GetExZoneInfo( GetZoneHandle(i), _CAMP_LV, 0 ));
			if ( pZD!=NULL ) {
				z.Index=GetZoneX(i);z.Serial=GetZoneY(i);
                if (pZD->Direction == SDIR_ENUM->Get(dir.str)) {
					if (pZD->Status==0){
						createCamp(pIMD->AttackerNation_ID,&z,pIMD->getAttackerN_SQ());
					}else{
						createCamp(pIMD->DefenderNation_ID,&z,pIMD->getDefenderN_SQ());
					}
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------

//----------------------additional-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
int		MinLV(int a, int b){
	if (a<b) {
		return	a;
	}
	return b;
}
//-------------------------------------------------------------------------------------------------------------
int		MaxLV(int a, int b){
	if (a>b) {
		return a;
	}
	return b;
}
//-------------------------------------------------------------------------------------------------------------
void	setDigit(unsigned long int& X, int r, int n){
	X -= ((X/(10^r))%10)*(10^r);
	X += n*(10^r);
}
//-------------------------------------------------------------------------------------------------------------
int		getDigit(const unsigned long int X, int r){
	return	(X/(10^r))%10;
}
//-------------------------------------------------------------------------------------------------------------
int		getNRate(const int value, const int rate){
	int rv = value;
	if (rv<0) rv = -rv;
	double al = (double)rate/100.0;
	rv = (int)((double)rv*al+0.5);
	if (value<0) rv = -rv;
	return rv;
}
//-------------------------------------------------------------------------------------------------------------
int		GetCannonsAmount(int nation) { // grey [14.06.2004]
	char* art[50]  = { "ArtPus4(EN)",  "ArtPus4(FR)",  "ArtPus4(AU)",  "ArtPus4(PR)",  "ArtPus4(RU)",  "ArtPus4(EG)",  0 };
	char* artL[50] = { "ArtPus4L(EN)", "ArtPus4L(FR)", "ArtPus4L(AU)", "ArtPus4L(PR)", "ArtPus4L(RU)", "ArtPus4L(EG)", 0 };
	char* artG[50] = { "ArtPus4G(EN)", "ArtPus4G(FR)", "ArtPus4G(AU)", "ArtPus4G(PR)", "ArtPus4G(RU)", "ArtPus4G(EG)", 0 };
	
	GAMEOBJ Type;
	int  ArtPus = 0,  ArtPusL = 0,  ArtPusG = 0, ArtPusR = 0,  Sum = 0; //ArtPus4R(EN)

	RegisterUnitType( &Type, "ArtPus4R(EN)" );	ArtPusR  += GetTotalAmount1( &Type , nation );	

	for(int i=0; art[i]!=0 ; i++ ) {
		RegisterUnitType( &Type, art[i]  );		ArtPus  += GetTotalAmount1( &Type , nation );	
		RegisterUnitType( &Type, artL[i] );		ArtPusL += GetTotalAmount1( &Type , nation );	
		RegisterUnitType( &Type, artG[i] );		ArtPusG += GetTotalAmount1( &Type , nation );
	}
	Sum = ArtPus + ArtPusL + ArtPusG + ArtPusR;
	return  Sum;
}
//-------------------------------------------------------------------------------------------------------------
int		GetCapturedCannons(int nation, int realNI) { //new
	char* art[50]  = { "ArtPus4(EN)",  "ArtPus4(FR)",  "ArtPus4(AU)",  "ArtPus4(PR)",  "ArtPus4(RU)",  "ArtPus4(EG)",  0 };
	char* artL[50] = { "ArtPus4L(EN)", "ArtPus4L(FR)", "ArtPus4L(AU)", "ArtPus4L(PR)", "ArtPus4L(RU)", "ArtPus4L(EG)", 0 };
	char* artG[50] = { "ArtPus4G(EN)", "ArtPus4G(FR)", "ArtPus4G(AU)", "ArtPus4G(PR)", "ArtPus4G(RU)", "ArtPus4G(EG)", 0 };
	
	GAMEOBJ Type;
	int  ArtCapt = 0; 

	if( realNI != 0 ){
		RegisterUnitType( &Type, "ArtPus4R(EN)" );	
		ArtCapt += GetTotalAmount1( &Type , nation );
	}

	for(int i=0; art[i]!=0 ; i++ ) 
		if( realNI != i ){
			RegisterUnitType( &Type, art[i]  );		
			ArtCapt  += GetTotalAmount1( &Type , nation );

			RegisterUnitType( &Type, artL[i] );		
			ArtCapt += GetTotalAmount1( &Type , nation );

			RegisterUnitType( &Type, artG[i] );		
			ArtCapt += GetTotalAmount1( &Type , nation );
		}
	return  ArtCapt;
}
//-------------------------------------------------------------------------------------------------------------
void	CheckResource(int nation, int resource) {// GREY [02.03.2004]
	if( GetResource(nation, resource) < 100000 )   SetResource(nation, resource, 10000000);
}
void	CheckAllResources(int nation) {// GREY [02.03.2004]
	for(int i=0; i <= 5 ; i++)   CheckResource(nation, i);
}
//-------------------------------------------------------------------------------------------------------------














