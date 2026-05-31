
#include "MainEngine.h"

//**********************************************************************************
void	deleteSXYFarr( ClassArray<SXYF>* pXYF ){
	if ( pXYF==NULL ) return;
	//SXYF* pC = NULL;
	//int n = pXYF->GetAmount();
	//for (int i=0; i<n; i++){
	//	pC = (*pXYF)[i];
	//	(*pXYF)[i]=NULL;
	//	delete pC;
	//	pC = NULL;
	//}
	pXYF->Clear();
}
//-----------------------------------------------------------------------------------
void	deleteCTYPENUMarr( ClassArray<CTYPENUM>* pTN ){
	if (pTN==NULL)	return;
	//CTYPENUM* pC = NULL;
	//int n = pTN->GetAmount();
	//for (int i=0; i<n; i++){
	//	pC = (*pTN)[i];
	//	(*pTN)[i]=NULL;
	//	delete pC;
	//	pC = NULL;
	//}
	pTN->Clear();
}
//-----------------------------------------------------------------------------------
// FOR STARTPOINT
void	SetMissionStartPoint( CSAVE_MIS_STRUCT* pIMD ) 
{
	ClassArray<SXYF> Prm;
	getArmyStartPos( pIMD , &Prm );
	//if( Prm.GetAmountOfElements() < 1 )	return;
	if( Prm.GetAmountOfElements() > 0 ) {
		GAMEOBJ Z; 
		Z.Type		= 0x40000000 + 250;	
		Z.Index		= Prm[0]->X ;		
		Z.Serial	= Prm[0]->Y ;		
		SetStartPoint(&Z);
	}
	Prm.Clear();	
}
//-----------------------------------------------------------------------------------
	
bool	SetEnemyPosPoint( CSAVE_MIS_STRUCT* pIMD ) //new
{
	//POSITION DESCRIPTION
	_str	sDir; 
	sDir.operator=( pIMD->getDirection_sID() );
	ClassArray<SXYF> Prm;	ClassArray<SXYF> Cnt;	ClassArray<SXYF> Rsv;	ClassArray<SXYF> Art;

	if(pIMD->PlayerStat==0)  getDefenderPos( &Prm, &Cnt, &Rsv, &Art, sDir );
	if(pIMD->PlayerStat==1)  getAttackerPos( &Prm, &Cnt, &Rsv, &Art, sDir );

	bool have_point = false;
	// POINT ENEMY ARMY ON MINI-MAP
	if( Prm.GetAmountOfElements() > 0 ) {
		have_point = true;
		g_addTask(7, Prm[0]->X, Prm[0]->Y,"EnemyStart"); // bmFLAG.SkTASK_TL.xml 
	}
	//CLEAR POSITION
	Prm.Clear();	
	Cnt.Clear();	
	Rsv.Clear();
	Art.Clear();	

	return have_point;

	//getAttackerPos( &Prm, &Cnt, &Rsv, &Art, sDir );
	//getDefenderPos( &Prm, &Cnt, &Rsv, &Art, sDir );
}

//-----------------------------------------------------------------------------------
void	getStrategicZones( ClassArray<STRAT_ZONE>* pStr ) //grey
{
	if ( !pStr )		  { return; }
	pStr->Clear();
	
	int	 zN = GetAmountOfZones();
	STRAT_ZONE*	 pZ = NULL;
	for ( int i=0; i < zN ; i++ ) 	
	{
		pZ = reinterpret_cast<STRAT_ZONE*>(GetExZoneInfo(GetZoneHandle(i), _STRAT_ZONE_LV ,0));
		if( pZ ) pStr->Add( pZ );
	}
}
//-----------------------------------------------------------------------------------
void	getStrategicZones( int* pID , int* pSt , int NUM ) //grey
{
	if( NUM <= 0 )		  { return; }

	int	 zN = GetAmountOfZones();
	STRAT_ZONE*	 pZ = NULL;

	int	 n  = 0;
	for(int i=0; i < zN && n < NUM ; i++ ) 	
	{
		pZ = reinterpret_cast<STRAT_ZONE*>(GetExZoneInfo(GetZoneHandle(i), _STRAT_ZONE_LV ,0));
		if( pZ ) {
			pID[n] = i ; 
			pSt[n] = pZ->State ;
			n++;
		}
	}
}
//-----------------------------------------------------------------------------------
int 	getStrategicTowerZones( int* pID , int* pState , int* pWeight ) //grey [04.08.2004]
{
	TOWER_ZONE*	 pZ = NULL;
	int	 zN = GetAmountOfZones() , n = 0;
	for(int i=0; i < zN ; i++ ) {
		pZ = reinterpret_cast<TOWER_ZONE*>(GetExZoneInfo(GetZoneHandle(i), _TOWER_ZONE_LV ,0));
		if( pZ ) {
			pID[n]		= i ; 
			pState[n]	= pZ->State ;
			pWeight[n]	= pZ->Weight ;
			n++;
		}
	}
	return  n;
}
//-----------------------------------------------------------------------------------
void	getArmyStartPos( CSAVE_MIS_STRUCT* pIMD , ClassArray<SXYF>* pPrm )
{
	if ( pPrm==NULL )  { return; }
	pPrm->Clear();
	int	zN = GetAmountOfZones();
	CZM_DRS_LV*	pZ = NULL;
	Enumerator*	pSDIR_ENUM = ENUM.Get("SDIR_ENUM");
	Enumerator*	pSDCT_ENUM = ENUM.Get("SDCT_ENUM");
	//POSITION DESCRIPTION
	_str	sDir; 
	sDir.operator=( pIMD->getDirection_sID() );
	if ( pSDIR_ENUM->GetAmount()==0 || pSDCT_ENUM->GetAmount()==0 )	{ return; }
	int			zDIR = -1;
	int			cDIR = pSDIR_ENUM->Get(sDir.str);
	SXYF*		pDD = NULL;
	for ( int i=0; i<zN; i++ ) {	// InitData.PlayerStat =  a 0 / d 1
		if(pIMD->PlayerStat==0) { pZ = reinterpret_cast<CZM_DRS_LV*>(GetExZoneInfo(GetZoneHandle(i),_CZM_ARS_LV,0)); }
		if(pIMD->PlayerStat==1) { pZ = reinterpret_cast<CZM_DRS_LV*>(GetExZoneInfo(GetZoneHandle(i),_CZM_DRS_LV,0)); }
		if ( pZ!=NULL ) 
			for ( int ii=0; ii<pZ->m_DIR_TYPE.GetAmount(); ii++ ) {
				zDIR = (pZ->m_DIR_TYPE[ii])->m_CoverDirs;
				if ( zDIR == cDIR ) {
					pDD = new SXYF();
					pDD->X	= GetZoneX(i);
					pDD->Y	= GetZoneY(i);
					pDD->F	= (pZ->m_DIR_TYPE[ii])->m_Direction;
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 0 ) { pPrm->Add(pDD); 
																	return; /*!!!!!*/   }
				}
			}
	}
}
/*
SXYF	getArmyStatrtPos(int sect_id, _str& dir_id){
	SXYF	zR(0,0,0);
	int				zN	= GetAmountOfZones();
	CZM_ARS_LV*		pZD = NULL;
	DynArray<int>	zT;
	Enumerator*	SDIR_ENUM	= ENUM.Get("SDIR_ENUM");
	if ( SDIR_ENUM->Values.GetAmount() ) {
		for ( int i=0; i<zN; i++ ){
			pZD = reinterpret_cast<CZM_ARS_LV*>(GetExZoneInfo( GetZoneHandle(i), _CZM_ARS_LV, 0 ));
			if ( pZD!=NULL ) {
				int ttt = SDIR_ENUM->Get(dir_id.str);
				if ( pZD->m_ZoneType == SDIR_ENUM->Get(dir_id.str) ) {
					zT.Add(i);				}			}		}	}
	zN = zT[rand()%zT.GetAmount()];    
	zT.Clear();
	if		 ( dir_id == "East"		  )	{
		zR.set( GetZoneX(zN), GetZoneY(zN), 128 );
	}else if ( dir_id == "North-East" )	{
		zR.set( GetZoneX(zN), GetZoneY(zN), 96  );
	}else if ( dir_id == "North"	  ) {
		zR.set( GetZoneX(zN), GetZoneY(zN), 64  );
	}else if ( dir_id == "North-West" ) {
		zR.set( GetZoneX(zN), GetZoneY(zN), 32  );
	}else if ( dir_id == "West"		  ) {
		zR.set( GetZoneX(zN), GetZoneY(zN), 0   );
	}else if ( dir_id == "South-West" ) {
		zR.set( GetZoneX(zN), GetZoneY(zN), 224 );
	}else if ( dir_id == "South"	  ) {
		zR.set( GetZoneX(zN), GetZoneY(zN), 192 );
	}else if ( dir_id == "South-East" ) {
		zR.set( GetZoneX(zN), GetZoneY(zN), 160 );	}
	return	zR;
}
*/
//-----------------------------------------------------------------------------------
//void	getDefenderPos( ClassArray<SXYF>* pPrm, ClassArray<SXYF>* pCnt, ClassArray<SXYF>* pRsv, _str& dir_id ){
void	getDefenderPos( ClassArray<SXYF>* pPrm, ClassArray<SXYF>* pCnt, ClassArray<SXYF>* pRsv, ClassArray<SXYF>* pArt, _str& dir_id ){
	if ( pPrm==NULL && pCnt==NULL && pRsv==NULL )	return;
	pPrm->Clear();	pCnt->Clear(); pRsv->Clear();
    int	zN = GetAmountOfZones();
	CZM_DRS_LV*	pZ = NULL;
	Enumerator*	pSDIR_ENUM = ENUM.Get("SDIR_ENUM");
	Enumerator*	pSDCT_ENUM = ENUM.Get("SDCT_ENUM");
	if ( pSDIR_ENUM->GetAmount()==0 || pSDCT_ENUM->GetAmount()==0 )	return;
    int			zDIR = -1;
	int			cDIR = pSDIR_ENUM->Get(dir_id.str);
	SXYF*		pDD = NULL;
	for ( int i=0; i<zN; i++ ){
		pZ = reinterpret_cast<CZM_DRS_LV*>(GetExZoneInfo( GetZoneHandle(i), _CZM_DRS_LV, 0 ));
		if ( pZ!=NULL ) {
			for ( int ii=0; ii<pZ->m_DIR_TYPE.GetAmount(); ii++ ){
				zDIR = (pZ->m_DIR_TYPE[ii])->m_CoverDirs;
				if ( zDIR == cDIR ) {
					pDD = new SXYF();
					pDD->X	= GetZoneX(i);
					pDD->Y	= GetZoneY(i);
					pDD->F	= (pZ->m_DIR_TYPE[ii])->m_Direction;
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 0 ) { pPrm->Add(pDD); }else 
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 1 ) { pCnt->Add(pDD); }else
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 2 ) { pRsv->Add(pDD); }else
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 3 ) { pArt->Add(pDD); }//grey
					pDD = NULL;
				}			}		}	}
}
//-----------------------------------------------------------------------------------
//void	getAttackerPos( ClassArray<SXYF>* pPrm, ClassArray<SXYF>* pCnt, ClassArray<SXYF>* pRsv, _str& dir_id ){
void	getAttackerPos( ClassArray<SXYF>* pPrm, ClassArray<SXYF>* pCnt, ClassArray<SXYF>* pRsv, ClassArray<SXYF>* pArt, _str& dir_id ){
	if ( pPrm==NULL && pCnt==NULL && pRsv==NULL )	return;
	pPrm->Clear();	pCnt->Clear(); pRsv->Clear(); pArt->Clear();
	int	zN = GetAmountOfZones();
	CZM_DRS_LV*	pZ = NULL;
	Enumerator*	pSDIR_ENUM = ENUM.Get("SDIR_ENUM");
	Enumerator*	pSDCT_ENUM = ENUM.Get("SDCT_ENUM");
	if ( pSDIR_ENUM->GetAmount()==0 || pSDCT_ENUM->GetAmount()==0 )	return;
	int			zDIR = -1;
	int			cDIR = pSDIR_ENUM->Get(dir_id.str);
	SXYF*		pDD = NULL;
	for ( int i=0; i<zN; i++ ){
		pZ = reinterpret_cast<CZM_DRS_LV*>(GetExZoneInfo( GetZoneHandle(i), _CZM_ARS_LV, 0 ));
		if ( pZ!=NULL ) {
			for ( int ii=0; ii<pZ->m_DIR_TYPE.GetAmount(); ii++ ){
				zDIR = (pZ->m_DIR_TYPE[ii])->m_CoverDirs;
				if ( zDIR == cDIR ) {
					pDD = new SXYF();
					pDD->X	= GetZoneX(i);
					pDD->Y	= GetZoneY(i);
					pDD->F	= (pZ->m_DIR_TYPE[ii])->m_Direction;
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 0 ) { pPrm->Add(pDD); }else 
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 1 ) { pCnt->Add(pDD); }else
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 2 ) { pRsv->Add(pDD); }else
					if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 3 ) { pArt->Add(pDD); }//grey
					//if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 0 ) { pPrm->Add(pDD); }else 
					//if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 1 ) { pCnt->Add(pDD); }else
					//if ( (pZ->m_DIR_TYPE[ii])->m_CoverType == 2 ) { pRsv->Add(pDD); }
					pDD = NULL;
					}
				}
			}
		}
	}
//-----------------------------------------------------------------------------------
void	prepareAI_ZONE(_str& dir_id,LinearArray<int,_int>* pZID,LinearArray<int,_int>* pZALL)
{
	int				zN	= GetAmountOfZones();
	CAI_TOTAL_WAR*	pZD = NULL;
	Enumerator*	SDIR_ENUM	= ENUM.Get("SDIR_ENUM");
	if( SDIR_ENUM->GetAmount() ) {
		for(int i=0; i < zN ; i++ ) {
			pZD = reinterpret_cast<CAI_TOTAL_WAR*>(GetExZoneInfo( GetZoneHandle(i), AI_IMPORTANT_POINT, 0 ));
			if( pZD ) pZALL->Add(i);
			if( pZD != NULL && pZD->Extended ) 
			{
				pZID->Add(i);
				for (int j=0; pZD->Amount==0 && j < (pZD->Dir_Weight).GetAmount() ; j++)

					if( ((pZD->Dir_Weight)[j])->Direction == SDIR_ENUM->Get(dir_id.str) ) 
					{
						pZD->Amount=((pZD->Dir_Weight)[j])->Weight;
					}
			}
		}
	}
}
//**********************************************************************************
////////////////////////////////////////
/////////////// CTYPENUM ///////////////
////////////////////////////////////////
//----------------------------------------------
CTYPENUM::CTYPENUM()
{
	Kills.Clear();//grey
	TypeName_sID	= "NOTYPE";
	TypeIndex_ID	= 0;
	nSquarnd		= 0;
}
//----------------------------------------------
CTYPENUM::~CTYPENUM()
{
	Kills.Clear();//grey
	TypeName_sID	= "NOTYPE";//grey
}
//----------------------------------------------
CTYPENUM::CTYPENUM(char* strTN,int TI,int NS)
{
	Kills.Clear();//grey
	if (strTN!=NULL) TypeName_sID = strTN;
	else			 TypeName_sID = "NOTYPE";
	TypeIndex_ID	= TI;
	nSquarnd		= NS;
}
//----------------------------------------------
_str		CTYPENUM::getTypeName(){
	return	TypeName_sID;
}
//----------------------------------------------
int			CTYPENUM::getTypeIndx(){
	return	TypeIndex_ID;
}
//----------------------------------------------
int			CTYPENUM::getNSquard(){
	return	nSquarnd;
}
//----------------------------------------------
CTYPENUM	CTYPENUM::operator=(const CTYPENUM& teSQ){
	TypeName_sID = teSQ.TypeName_sID.str;
	TypeIndex_ID = teSQ.TypeIndex_ID;
	nSquarnd	 = teSQ.nSquarnd;
	return	*this;
}
//----------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CResource_BM   //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//CResource_BM::CResource_BM()
//{ 
//	wood=food=stone=gold=iron=coal=recruit=0; 
//};
//----------------------------------------------------------------------------------
CResource_BM::CResource_BM()
{ 
	Init(); 
};
//----------------------------------------------------------------------------------
CResource_BM::~CResource_BM()
{ 
	Init(); 
};
//----------------------------------------------------------------------------------
void	CResource_BM::Init()
{
	wood=food=stone=gold=iron=coal=recruit=0; 
}

//----------------------------------------------------------------------------------
int		CResource_BM::GetResource(int res)
{
	if(res==WOOD)	return	wood;
	if(res==FOOD)	return	food;
	if(res==STONE)	return	stone;
	if(res==GOLD)	return	gold;
	if(res==IRON)	return	iron;
	if(res==COAL)	return	coal;
	return	recruit;
}
//----------------------------------------------------------------------------------
int		CResource_BM::SetResource0(int res, int amount)
{
	if(res==WOOD)	return	wood = amount;
	if(res==FOOD)	return	food = amount;
	if(res==STONE)	return	stone = amount;
	if(res==GOLD)	return	gold = amount;
	if(res==IRON)	return	iron = amount;
	if(res==COAL)	return	coal = amount;
	return	recruit = amount;
}//----------------------------------------------------------------------------------
int		CResource_BM::SetResource(int res, int amount)
{
	int am = SetResource0( res, amount );
	if(am < 0)	am = SetResource0( res, 0 );
	return	am ;
}

//----------------------------------------------------------------------------------
int		CResource_BM::AddResource(int res, int amount)
{
	int am = SetResource0( res, GetResource( res ) + amount );
	if( am < 0 )	am = SetResource0( res, 0 );
	return	am;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//**********************************************************************************
////////////////////////////////////////
/////////// CSAVE_MIS_STRUCT ///////////
////////////////////////////////////////
//----------------------------------------------
CSAVE_MIS_STRUCT::CSAVE_MIS_STRUCT()
{
	Init();
}
//----------------------------------------------
CSAVE_MIS_STRUCT::~CSAVE_MIS_STRUCT()
{
	Init();
	//DeleteArrays();
}	
//----------------------------------------------
void	CSAVE_MIS_STRUCT::Init()
{
	AttackerForce.Clear();
	DefenderForce.Clear();

	Sector_ID			= 0;
	AttackDirection_ID	= "NO_DATA";
	AttackerNation_ID	= 0;
	AttackerRank_ID		= "NO_DATA";
	DefenderNation_ID	= 0;
	DefenderRank_ID		= "NO_DATA";
	PlayerStat			= 0;
	HeroPresent			= false;
}
//----------------------------------------------
void	CSAVE_MIS_STRUCT::DeleteArrays()
{
	AttackerForce.Clear();
	DefenderForce.Clear();
}
//----------------------------------------------
//For Create Mission Only.
int		CSAVE_MIS_STRUCT::getSector_ID(){
	return	Sector_ID;
}
//----------------------------------------------
char*	CSAVE_MIS_STRUCT::getDirection_sID(){
	return	AttackDirection_ID.str;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getPlayerStat(){
	return PlayerStat;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getAttackerNat(){
	return	AttackerNation_ID;
}
//----------------------------------------------
_str	CSAVE_MIS_STRUCT::getAttackerRank(){
	return	AttackerRank_ID;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getAttackerNdif(){
	return	AttackerForce.GetAmount();
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getAttackerN_SQ(){
	int N_SQ = 0;
	for (int i=0;i<AttackerForce.GetAmountOfElements();i++){
		N_SQ += AttackerForce[i]->getNSquard();
	}
	return	N_SQ;
}
//----------------------------------------------
_str	CSAVE_MIS_STRUCT::getAttackerType_sID(int n){
	if ( 0<=n && n<=getAttackerNdif() ) {
		return	(AttackerForce[n])->TypeName_sID;
	}
	_str	nodata;
	return	nodata;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getAttackerType_iID(int n){
	if ( 0<=n && n<=getAttackerNdif() ) {
		return	(AttackerForce[n])->TypeIndex_ID;
	}
	return	0;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getAttackerType_N(int n){
	if ( 0<=n && n<=getAttackerNdif() ) {
		return	(AttackerForce[n])->nSquarnd;
	}
	return	0;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getDefenderNat(){
	return	DefenderNation_ID;
}
//----------------------------------------------
_str	CSAVE_MIS_STRUCT::getDefenderRank(){
	return	DefenderRank_ID;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getDefenderNdif(){
	return	DefenderForce.GetAmount();
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getDefenderN_SQ(){
	int N_SQ = 0;
	for (int i=0;i<DefenderForce.GetAmountOfElements();i++){
		N_SQ += DefenderForce[i]->getNSquard();
	}
	return	N_SQ;
}
//----------------------------------------------
_str	CSAVE_MIS_STRUCT::getDefenderType_sID(int n){
	if ( 0<=n && n<=getDefenderNdif() ) {
		return	(DefenderForce[n])->TypeName_sID;
	}
	_str	nodata;
	return	nodata;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getDefenderType_iID(int n){
	if ( 0<=n && n<=getDefenderNdif() ) {
		return	(DefenderForce[n])->TypeIndex_ID;
	}
	return	0;
}
//----------------------------------------------
int		CSAVE_MIS_STRUCT::getDefenderType_N(int n){
	if ( 0<=n && n<=getDefenderNdif() ) {
		return	(DefenderForce[n])->nSquarnd;
	}
	return	0;
}
//----------------------------------------------------------------------------------