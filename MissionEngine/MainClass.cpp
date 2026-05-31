
#include "MainEngine.h"

//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////    SARMY_DESCRIPTION    /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
SARMY_DESCRIPTION::SARMY_DESCRIPTION(){
	Init();
}
//------------------------------------
SARMY_DESCRIPTION::SARMY_DESCRIPTION(ClassArray<CTYPENUM>* pA){
	Init();
	setArmy(pA);	
}
//------------------------------------
void	SARMY_DESCRIPTION::Init(){
	SQ_NUM			= 0;
	SQ_K_NUM		= 0;
	SQ_P_NUM		= 0;	
	CANNON_NUM		= 0;
	MOBILITY		= 0;	
}
//------------------------------------
bool	SARMY_DESCRIPTION::setArmy(ClassArray<CTYPENUM>* pA)
{
	if( pA == NULL )			return false;
	int  sqType = pA->GetAmount();
	int  TI = 0 ,   NS = 0; 
	for(int i=0; i < sqType ; i++)
	{
		NS = ((*pA)[i])->nSquarnd;
		if( GetObjectBranch( ((*pA)[i])->TypeName_sID.str ) == 0 )		{ SQ_P_NUM	  +=  NS ; } else
		if( GetObjectBranch( ((*pA)[i])->TypeName_sID.str ) == 1 )		{ SQ_K_NUM	  +=  NS ; } 
		else 															{ CANNON_NUM  +=  NS ; }
		//if( GetObjectBranch( ((*pA)[i])->TypeName_sID.str ) == 2 )		{ CANNON_NUM  +=  NS ; }  
	}
	SQ_NUM   = SQ_P_NUM + SQ_K_NUM + CANNON_NUM ;
	MOBILITY = (int)(((float)SQ_K_NUM/(float)SQ_NUM)*100.0f);
}
//------------------------------------
bool	SARMY_DESCRIPTION::operator==(const SARMY_DESCRIPTION& ARM){
	if ( (ARM.SQ_NUM-ARM.SQ_NUM/3-1)<=SQ_NUM && SQ_NUM<=(ARM.SQ_NUM+ARM.SQ_NUM/3+1) ){
		if ( (ARM.MOBILITY-ARM.MOBILITY/3)<=MOBILITY && MOBILITY<=(ARM.MOBILITY+ARM.MOBILITY/3) ){ 
			return true;
		}
	}
	return	false;
}
//------------------------------------
bool	SARMY_DESCRIPTION::operator<(const SARMY_DESCRIPTION& ARM){
	if ( SQ_NUM+SQ_NUM/3-1 < ARM.SQ_NUM )	return true;
	return false;
}
//------------------------------------
bool	SARMY_DESCRIPTION::operator>(const SARMY_DESCRIPTION& ARM){
	if ( SQ_NUM > ARM.SQ_NUM+ARM.SQ_NUM/3+1 )	return true;
	return false;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////    CARMY_GAMEOBJ    ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//------------------------------------
CARMY_GAMEOBJ::CARMY_GAMEOBJ(){
	Init();
} 
//------------------------------------
CARMY_GAMEOBJ::~CARMY_GAMEOBJ(){
	Init();
}
//------------------------------------
void	CARMY_GAMEOBJ::Init(){
	clearPrimary();
	clearControl();
	clearReserve();
}
//------------------------------------
void	CARMY_GAMEOBJ::addGR(ClassArray<GAMEOBJ>* pAR, GAMEOBJ* pGR){
	if (pAR && pGR && pGR->Type=='UNIT') {
		pAR->Add(pGR);
	}
}
//------------------------------------
void	CARMY_GAMEOBJ::addPrimaryGR(GAMEOBJ* pGR){
	addGR(&Primary,pGR);
}
//------------------------------------
void	CARMY_GAMEOBJ::addControlGR(GAMEOBJ* pGR){
	addGR(&Control,pGR);
}
//------------------------------------
void	CARMY_GAMEOBJ::addReserveGR(GAMEOBJ* pGR){
	addGR(&Reserve,pGR);
}
//------------------------------------
void	CARMY_GAMEOBJ::addGR(ClassArray<GAMEOBJ>* pAR, GAMEOBJ* pGR, int n){
	if (pAR && pGR && ( 0<=n && n<pAR->GetAmount() ) ) {
		RemoveGroup(pGR,(*pAR)[n]);
	}
}
//------------------------------------
void	CARMY_GAMEOBJ::addPrimaryGR(GAMEOBJ* pGR, int n){
	addGR(&Primary,pGR,n);
}
//------------------------------------
void	CARMY_GAMEOBJ::addControlGR(GAMEOBJ* pGR, int n){
	addGR(&Control,pGR,n);
}
//------------------------------------
void	CARMY_GAMEOBJ::addReserveGR(GAMEOBJ* pGR, int n){
	addGR(&Reserve,pGR,n);
}
//------------------------------------
void	CARMY_GAMEOBJ::delGrpFromArr(ClassArray<GAMEOBJ>* pAR, int n){
	if (pAR && ( 0<=n && n<pAR->GetAmount() )) {
		pAR->Del(n,1);
	}
}
//------------------------------------
void	CARMY_GAMEOBJ::delPrimaryGR(int n){
	delGrpFromArr(&Primary,n);
}
//------------------------------------
void	CARMY_GAMEOBJ::delControlGR(int n){
	delGrpFromArr(&Control,n);
}
//------------------------------------
void	CARMY_GAMEOBJ::delReserveGR(int n){
	delGrpFromArr(&Reserve,n);
}
//------------------------------------
void	CARMY_GAMEOBJ::clearArr(ClassArray<GAMEOBJ>* pAR){
	if (pAR) {
		pAR->Clear();
	}
}
//------------------------------------
void	CARMY_GAMEOBJ::clearPrimary(){
	clearArr(&Primary);
}
//------------------------------------
void	CARMY_GAMEOBJ::clearControl(){
	clearArr(&Control);
}
//------------------------------------
void	CARMY_GAMEOBJ::clearReserve(){
	clearArr(&Reserve);
}
//------------------------------------
int		CARMY_GAMEOBJ::getARMY_SQ_NUM(){
	int SQ_NUM = 0;
	SQ_NUM += getPrimarySQ_NUM();
	SQ_NUM += getControlSQ_NUM();
	SQ_NUM += getReserveSQ_NUM();
	return	SQ_NUM;
}
//------------------------------------
int		CARMY_GAMEOBJ::getARMY_UT_NUM(){
	int UT_NUM = 0;
	UT_NUM += getPrimaryUT_NUM();
	UT_NUM += getControlUT_NUM();
	UT_NUM += getReserveUT_NUM();
	return	UT_NUM;
}
//------------------------------------
int		CARMY_GAMEOBJ::getSQ_NUM(ClassArray<GAMEOBJ>* pAR){
	int	SQ_NUM = 0;
	if (pAR) {
		for (int i=0; i<pAR->GetAmount(); i++){
			SQ_NUM += getSQ_NUM(pAR,i);
		}
	}
	return	SQ_NUM;
}
//------------------------------------
int		CARMY_GAMEOBJ::getPrimarySQ_NUM(){
	return getSQ_NUM(&Primary);
}
//------------------------------------
int		CARMY_GAMEOBJ::getControlSQ_NUM(){
	return getSQ_NUM(&Control);
}
//------------------------------------
int		CARMY_GAMEOBJ::getReserveSQ_NUM(){
	return getSQ_NUM(&Reserve);
}
//------------------------------------
int		CARMY_GAMEOBJ::getSQ_NUM(ClassArray<GAMEOBJ>* pAR, int n){
	if (pAR&&(0<=n&&n<pAR->GetAmount())) {
		return	GetBrigadsAmount((*pAR)[n]);
	}
	return 0;
}
//------------------------------------
int		CARMY_GAMEOBJ::getPrimarySQ_NUM(int n){
	return getSQ_NUM(&Primary,n);
}
//------------------------------------
int		CARMY_GAMEOBJ::getControlSQ_NUM(int n){
	return getSQ_NUM(&Control,n);
}
//------------------------------------
int		CARMY_GAMEOBJ::getReserveSQ_NUM(int n){
	return getSQ_NUM(&Reserve,n);
}
//------------------------------------
int		CARMY_GAMEOBJ::getUT_NUM(ClassArray<GAMEOBJ>* pAR){
	int	UT_NUM = 0;
	if (pAR) {
		for (int i=0; i<pAR->GetAmount(); i++){
			UT_NUM += getUT_NUM(pAR,i);
		}
	}
	return	UT_NUM;
}
//------------------------------------
int		CARMY_GAMEOBJ::getPrimaryUT_NUM(){
	return getUT_NUM(&Primary);
}
//------------------------------------
int		CARMY_GAMEOBJ::getControlUT_NUM(){
	return getUT_NUM(&Control);
}
//------------------------------------
int		CARMY_GAMEOBJ::getReserveUT_NUM(){
	return getUT_NUM(&Reserve);
}
//------------------------------------
int		CARMY_GAMEOBJ::getUT_NUM(ClassArray<GAMEOBJ>* pAR, int n){
	if (pAR&&(0<=n&&n<pAR->GetAmount())) {
		return	GetTotalAmount0((*pAR)[n]);
	}
	return 0;
}
//------------------------------------
int		CARMY_GAMEOBJ::getPrimaryUT_NUM(int n){
	return	getUT_NUM(&Primary,n);
}
//------------------------------------
int		CARMY_GAMEOBJ::getControlUT_NUM(int n){
	return	getUT_NUM(&Control,n);
}
//------------------------------------
int		CARMY_GAMEOBJ::getReserveUT_NUM(int n){
	return	getUT_NUM(&Reserve,n);
}
//------------------------------------
SXYF	CARMY_GAMEOBJ::getGR_XYF(ClassArray<GAMEOBJ>* pAR, int n){
	SXYF	PT(0,0,0);
	if (pAR && (0<=n && n<pAR->GetAmount())) {
		GAMEOBJ* pGR = (*pAR)[n];
		int x,y;
		GetGrpCenter(pGR,&x,&y);
		PT.set(x,y,0);
	}
	return PT;
}
//------------------------------------
SXYF	CARMY_GAMEOBJ::getPrimarySQ_XYF(int n){
	return	getGR_XYF(&Primary,n);
}
//------------------------------------
SXYF	CARMY_GAMEOBJ::getControlSQ_XYF(int n){
	return	getGR_XYF(&Control,n);
}
//------------------------------------
SXYF	CARMY_GAMEOBJ::getReserveSQ_XYF(int n){
	return	getGR_XYF(&Reserve,n);
}
//------------------------------------
*/
//----------------------------------------------------------------------------------








