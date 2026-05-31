#include "stdheader.h"
#include "BE_HEADERS.h"
#include "GroupPurpose.h"
#include "GameInterface.h"

bool	g_beNeighbor(int MFIELD,int Area0, int Area1){
	if ( MFIELD<0 ||MFIELD>=4 || Area0<0 || Area0>HashTable[MFIELD].NAreas || Area1<0 || Area1>HashTable[MFIELD].NAreas )
		return false;
	Area* pArea = HashTable[MFIELD].GetTopArea(Area0);
	if (pArea!=NULL){
		for ( int i=0; i<pArea->NLinks; i++ ){
			if ( pArea->Link[i].NextAreaID==Area1 ) return true;
		};
	};
	return 	false;
};

// be_CAreaPowerData /////////////////////////////////////////////////////
be_CAreaPowerData::be_CAreaPowerData(){
	AreaID					= -1;
	BuildingAttackPOWER		=  0;
	BuildingDefencePOWER	=  0;	
	MeleeAttackPOWER		=  0;		
	RangeAttackPOWER		=  0;		
	UnitsDefencePOWER		=  0;	
	MeleeUnitsN				=  0;
	RangeUnitsN				=  0;
	BuildingsN				=  0;
};
void	be_CAreaPowerData::resetData()	{
	BuildingAttackPOWER		= 0;
	BuildingDefencePOWER	= 0;
	MeleeAttackPOWER		= 0;
	RangeAttackPOWER		= 0;
	UnitsDefencePOWER		= 0;
	MeleeUnitsN				= 0;
	RangeUnitsN				= 0;
	BuildingsN				= 0;
};
// be_CAreaListPowerData /////////////////////////////////////////////////
be_CAreaListPowerData::be_CAreaListPowerData(int layer,int nat) : be_CAreaPowerData() {
	m_layer		= layer;
	m_nation	= nat;
};
int		be_CAreaListPowerData::getAreaNumber()			const{
	return AreasList.GetAmount();
};// get N of areas in areas list
int		be_CAreaListPowerData::getArea(int localID)		const{
	if ( 0<=localID && localID<getAreaNumber() ){
		return AreasList[localID];
	};
	return -1;
};// get area id from list
int		be_CAreaListPowerData::getMeleeAttackPOWER()	const{
	return	MeleeAttackPOWER;
};
int		be_CAreaListPowerData::getMeleeUnitsN()			const{
	return	MeleeUnitsN;
};
int		be_CAreaListPowerData::getRangeAttackPOWER()	const{
	return	RangeAttackPOWER;
};
int		be_CAreaListPowerData::getRangeUnitsN()			const{
	return	RangeUnitsN;
};
int		be_CAreaListPowerData::getUnitsDefencePOWER()	const{
	return	UnitsDefencePOWER;
};
int		be_CAreaListPowerData::getBuildingAttackPOWER()	const{
	return	BuildingAttackPOWER;
};
int		be_CAreaListPowerData::getBuildingsN()			const{
	return	BuildingsN;
};
int		be_CAreaListPowerData::getBuildingDefencePOWER()const{
	return	BuildingDefencePOWER;
};
int		be_CAreaListPowerData::getWeakestAST_AID()		const{
	int minAST = -1;
	int AreaID = -1;
	for ( int i=0; i<((be_CAreaListPowerData*)(this))->getAreaNumber(); i++ ){
        if ( minAST==-1 || minAST>g_beHTT_LIST[m_nation][m_layer].m_piAssist[getArea(i)] ){
			minAST=g_beHTT_LIST[m_nation][m_layer].m_piAssist[getArea(i)];
			AreaID = getArea(i);
		};
	};
	return AreaID;
};
bool	be_CAreaListPowerData::create( be_CGROUP* pGRP ){
	if ( pGRP!=NULL && pGRP->GetTotalAmount()>0 ) {
		int N = pGRP->GetTotalAmount();
		OneObject* pOB = NULL;
		while ( N-- ) {
			pOB = (*pGRP)[N];
			if (pOB!=NULL) {
				if ( pOB->NewBuilding==false ){	
					if ( pOB->MoreCharacter->MaxR_Attack>150 ){
						RangeAttackPOWER	+= pOB->newMons->MaxDamage[0];
						RangeUnitsN++;
					}else{
						MeleeAttackPOWER	+= pOB->newMons->MaxDamage[0];
						MeleeUnitsN++;
					};
					
					UnitsDefencePOWER		+= pOB->Life;
				}else{
					BuildingAttackPOWER		+= pOB->newMons->MaxDamage[0];
					BuildingDefencePOWER	+= pOB->Life;
					BuildingsN++;
				};				
			};
		};
		AreaID = -1;
		if ( pOB!=NULL && 0<=pOB->LockType && pOB->LockType<=3 ){
			int Xc,Yc;
			pGRP->GetGroupCenter(Xc,Yc);
			pOB = (*pGRP)[0];
			AreaID = HashTable[pOB->LockType].GetTopAreaIndex(Xc,Yc);
		};
		return true;
	};
	return false;
};
bool	be_CAreaListPowerData::create( pGroup& pGRP ){
	return create( pGRP.group() );
};
// be_CTotalALPD /////////////////////////////////////////////////////////
be_CTotalALPD::be_CTotalALPD(){
};
be_CTotalALPD::be_CTotalALPD(int layer,int nation,int power){ 
	create(layer,nation,power); 
};
int						be_CTotalALPD::getN() const{ 
	return ((ClassArray<be_CAreaListPowerData>*)(this))->GetAmount(); 
};
be_CAreaListPowerData*	be_CTotalALPD::getALPD(int i){
	if ( 0<=i && i<getN() )	{
		return (be_CAreaListPowerData*)((*this)[i]);
	};
	return NULL;
};
bool					be_CTotalALPD::create(int layer,int nation,int power){
	if ( 0<=nation && nation<8 && 0<=layer && layer<4 && 0<=power ){
		return (bool)(g_beHTT_LIST[nation][layer].AST_createPCL(this,power));
	};
	return false;
};
// be_HashTopTable ///////////////////////////////////////////////////////
be_HashTopTable::be_HashTopTable(){
//	m_iNAreas = 0;
//	Create();
//	Init();
};
be_HashTopTable::be_HashTopTable( int l_iNat, int l_iMFIELD ){
	Initialize(l_iNat,l_iMFIELD);
};
be_HashTopTable::be_HashTopTable( int l_iNAreas, int l_iNat, int l_iMFIELD ){
	m_iNation = l_iNat;
	m_iMFIELD = l_iMFIELD;
	m_iNAreas = l_iNAreas;
	Create();
	Init();
};
be_HashTopTable::~be_HashTopTable(){
	Clear();
	m_iNAreas = 0;
};
void		be_HashTopTable::Initialize( int l_iNat, int l_iMFIELD ){
	m_iNation = l_iNat;
	m_iMFIELD = l_iMFIELD;
	m_iNAreas = HashTable[m_iMFIELD].NAreas;
	lastTick  = 0;
	//Create();
	//Init();
};
bool		be_HashTopTable::Create(){
	if ( HashTable[m_iMFIELD].NAreas!=m_iNAreas ){
		EXP_Clear();
		AST_Clear();
		VSB_Clear();
		m_iCreate = false;
		m_iInit = false;
	};
	if ( m_iCreate==false ){
		m_iNAreas = HashTable[m_iMFIELD].NAreas;
		if ( m_iNAreas>0 ){
			EXP_Create();
			AST_Create();
			VSB_Create();
			m_iCreate = true;
		};
	};
	return m_iCreate;
};
void		be_HashTopTable::Clear(){
	EXP_Clear();	
	AST_Clear();
	VSB_Clear();
};
bool		be_HashTopTable::Init(){
	if ( m_iInit==false ){
		if ( m_iNAreas>0 ){
			for ( int i=0; i<m_iNAreas; i++ ){
				AST_Init( i );
				VSB_Init( i );
				EXP_Init( i );
			};
			for ( int i=0; i<m_piAssistPower.GetAmount(); i++ ){
				m_piAssistPower[i]->AreaID = -1;
			};
			m_iInit = true;
		};
	};
	return m_iInit;
};
void		be_HashTopTable::Update(){
	if ( m_iNAreas>0 ){
		for ( int i=0; i<m_iNAreas; i++ ){
			AST_Init( i );
			VSB_Init( i );
		};
		AST_ClearPD();
		AST_Update();
		for ( int i=0; i<m_iNAreas; i++ ){
			VSB_UpdateAround( i );
		};
		for ( int i=0; i<DriveMode()->TRIADA_NUM; i++ )	VSB_UpdateTRIADA();
		AST_ADD_Update();
	};
};
void		be_HashTopTable::Update(int delta){
	if ( delta<0 ) return;
	if ( GetTickCount()-lastTick>=delta ){
		Update();
		lastTick = GetTickCount();
		EXP_ReduceRate(1);
	};
};
void		be_HashTopTable::SafeUpdate(int delta){
	Create();
	Init();
	Update(delta);	
};
void		be_HashTopTable::EXP_Create(){
	m_piExploringRate = NULL;
	if ( m_iNAreas>0 ) m_piExploringRate = new short[m_iNAreas];
};
void		be_HashTopTable::EXP_Clear(){
	if ( m_piExploringRate!=NULL ){
		delete m_piExploringRate;
		m_piExploringRate = NULL;
	};
	m_piAssistPower.Clear();
};
void		be_HashTopTable::EXP_Init(int l_iAreaID){
	// Init
	m_piExploringRate[l_iAreaID] = 0;
	// Check dead Areas
	Area* l_pArea = HashTable[m_iMFIELD].GetTopArea(l_iAreaID);
	if ( l_pArea==NULL || l_pArea->NLinks==0 ){
		m_piExploringRate[l_iAreaID] = 0xFFFF;
	};
	if ( m_piExploringRate[l_iAreaID] != 0xFFFF && m_piVisible[l_iAreaID] == true ){
		m_piExploringRate[l_iAreaID] = 100;
	};
};
void		be_HashTopTable::EXP_Update(int l_iAreaID){
	if ( m_piExploringRate[l_iAreaID]!=0xFFFF && m_piVisible[l_iAreaID]==true ){
		m_piExploringRate[l_iAreaID] = 100;
	}
};
void		be_HashTopTable::EXP_ReduceRate(int l_iRR){
	if ( m_iNAreas>0 && m_piExploringRate!=NULL ){
		for ( int i=0; i<m_iNAreas; i++ ){
			if ( m_piVisible[i]==false && m_piExploringRate[i]!=0xFFFF ){
				if ( m_piExploringRate[i]!=0 && m_piExploringRate[i]!=1 ){
					m_piExploringRate[i] = ( (m_piExploringRate[i]-l_iRR<1) ? (1) : (m_piExploringRate[i]-l_iRR) );
				};
			};
		};
	};
};
void		be_HashTopTable::AST_Create(){
	m_piAssist = NULL;
	if ( m_iNAreas>0 ) m_piAssist		= new int[m_iNAreas];
	if ( m_iNAreas>0 ) m_piAssistAdd	= new int[m_iNAreas];
};
void		be_HashTopTable::AST_Clear(){
	if ( m_piAssist!=NULL ){
		delete m_piAssist;
		m_piAssist = NULL;
	};
	if ( m_piAssistAdd!=NULL ){
		delete m_piAssistAdd;
		m_piAssistAdd = NULL;
	};
};
void		be_HashTopTable::AST_Init(int l_iAreaID){
	m_piAssist[l_iAreaID]	 = 0;
	m_piAssistAdd[l_iAreaID] = 0;
};
void		be_HashTopTable::AST_Update(){
	int l_areaID = -1;
	be_CAreaPowerData*	l_pAPD = NULL;
	itr_GetUnitsOfNation.Create( m_iNation );
	while(OneObject* OB=itr_GetUnitsOfNation.Next()){
		if( OB->GetLiveState()==ls_LiveBody && OB->LockType==m_iMFIELD ){
			l_areaID = HashTable[m_iMFIELD].GetTopAreaIndex(OB->RealX/1024,OB->RealY/1024);
			if ( 0<=l_areaID && l_areaID!=0xFFFF && l_areaID!=0xFFFE ){
				m_piAssist[ l_areaID ] += 1;
				be_CAreaPowerData* l_pAPD = getAST_PD(l_areaID);
				if ( l_pAPD!=NULL ){
					if ( OB->NewBuilding==false ){	
						if ( OB->MoreCharacter->MaxR_Attack>150 ){
							l_pAPD->RangeAttackPOWER	+= OB->newMons->MaxDamage[0];
							l_pAPD->RangeUnitsN++;
						}else{
							l_pAPD->MeleeAttackPOWER	+= OB->newMons->MaxDamage[0];
							l_pAPD->MeleeUnitsN++;
						};
						
						l_pAPD->UnitsDefencePOWER		+= OB->Life;
					}else{
						l_pAPD->BuildingAttackPOWER		+= OB->newMons->MaxDamage[0];
						l_pAPD->BuildingDefencePOWER	+= OB->Life;
						l_pAPD->BuildingsN++;
					};
				};
			};
		};
	};

	//if ( m_piAssist[l_iAreaID] > 0 ){
	//	VSB_UpdateAround(l_iAreaID);
	//};
};
void		be_HashTopTable::AST_ADD_Update(){
	word NextAreaID = 0;
	for ( int i=0; i<m_iNAreas; i++ ){
		if ( m_piVisible[i]==true ){
			m_piAssistAdd[i] = 10;
			Area* pArea = HashTable[m_iMFIELD].GetTopArea(i);
			if ( pArea!=NULL && pArea->NLinks>0 ){
				for ( int ii=0; ii<pArea->NLinks; ii++ ){
					NextAreaID = pArea->Link[ii].NextAreaID;
					if ( NextAreaID>=0		&& NextAreaID!=0xFFFF && 
						 NextAreaID!=0xFFFE && m_piVisible[NextAreaID]==false )
					{
						m_piAssistAdd[NextAreaID] = 5;
					};
				};
			};
		};
	};
};
be_CAreaPowerData*	be_HashTopTable::getAST_PD(int l_iAreaID){
	int l_freeID = -1;
	for ( int i=0; i<m_piAssistPower.GetAmount(); i++ ){
		if ( m_piAssistPower[i]->AreaID==l_iAreaID ) return m_piAssistPower[i];
		if ( l_iAreaID==-1 && m_piAssistPower[i]->AreaID==-1 )	l_iAreaID = i;
	};
	if ( l_freeID!=-1 ){
		m_piAssistPower[l_freeID]->AreaID = l_iAreaID;
		return m_piAssistPower[l_freeID];
	}else{
		be_CAreaPowerData* pnewArea = new be_CAreaPowerData;
		pnewArea->AreaID = l_iAreaID;
		m_piAssistPower.Add(pnewArea);
		return pnewArea;
	};
	return NULL;	
};
void		be_HashTopTable::AST_ClearPD(){
	for ( int i=0; i<m_piAssistPower.GetAmount(); i++ ){
		m_piAssistPower[i]->resetData();
	};
};
bool		be_HashTopTable::AST_getConectedList(int power,ClonesArray< LinearArray<int,_int> >& refAreasArray){
	bool	OK = false;
	refAreasArray.Clear();
	for ( int i=0; i<m_iNAreas; i++ ){
		if ( !OK ) OK=true;
		if ( m_piAssist[i]>=power ){
			int lastID = -1;
			int ii=refAreasArray.GetAmount();
			while (ii--){
				int iii=refAreasArray[ii]->GetAmount();
				while (iii--){
					if ( g_beNeighbor(m_iMFIELD,i,(*refAreasArray[ii])[iii]) ){
						if ( lastID==-1 ){
							lastID = ii;
							refAreasArray[ii]->Add(i);
							break;
						}else{
							for ( int j=0; j<refAreasArray[ii]->GetAmount(); j++ ){
								refAreasArray[lastID]->Add( (*refAreasArray[ii])[j] );
							};
							refAreasArray.DelElement(ii);
							break;
						};
					};
				};
			};
			if ( lastID==-1 ){
				LinearArray<int,_int>* pNewArray = new LinearArray<int,_int>();
				pNewArray->Add(i);
				refAreasArray.Add(pNewArray); 
				pNewArray = NULL;
			};
		};
	};
	return OK;	
};
bool		be_HashTopTable::AST_createPCL(be_CTotalALPD* pDATA,int power/*=1*/){
	ClonesArray< LinearArray<int,_int> >	CL;
	if ( AST_getConectedList(power,CL) ) {
		be_CAreaListPowerData* pPLCL = NULL;
		be_CAreaPowerData*	   pPCL  = NULL;
		for ( int i=0; i<CL.GetAmount(); i++ ){
			pPLCL = new be_CAreaListPowerData(m_iMFIELD,m_iNation);
			for ( int ii=0; ii<CL[i]->GetAmount(); ii++ ){
				pPLCL->AreasList.Add( (*CL[i])[ii] );
				pPCL = getAST_PD( (*CL[i])[ii] );
				pPLCL->BuildingAttackPOWER	+= pPCL->BuildingAttackPOWER;
				pPLCL->BuildingsN			+= pPCL->BuildingsN;
				pPLCL->BuildingDefencePOWER += pPCL->BuildingDefencePOWER;
				pPLCL->MeleeAttackPOWER		+= pPCL->MeleeAttackPOWER;
				pPLCL->MeleeUnitsN			+= pPCL->MeleeUnitsN;
				pPLCL->RangeAttackPOWER		+= pPCL->RangeAttackPOWER;
				pPLCL->RangeUnitsN			+= pPCL->RangeUnitsN;
				pPLCL->UnitsDefencePOWER	+= pPCL->UnitsDefencePOWER;
			};
			pDATA->Add( pPLCL );
			pPLCL = NULL;
		};
		return true;
	};
	return false;
};
void		be_HashTopTable::AST_showPCL(int power/*=1*/){
	be_CTotalALPD DATA;
	if ( AST_createPCL(&DATA,power)==true ){
		SimpleEditClass("CONECTED AREAS LIST - POWER DATA",(BaseClass*)(&DATA));
	};
};
void		be_HashTopTable::VSB_Create(){
	m_piVisible = NULL;
	if ( m_iNAreas>0 ) m_piVisible = new bool[m_iNAreas];
};
void		be_HashTopTable::VSB_Clear(){
	if ( m_piVisible!=NULL ){
		delete m_piVisible;
		m_piVisible = NULL;
	};
};
void		be_HashTopTable::VSB_Init(int l_iAreaID){
	m_piVisible[l_iAreaID] = false;
};
void		be_HashTopTable::VSB_UpdateAround(int l_iAreaID){
	if ( m_piAssist[l_iAreaID]>0 ){
		m_piVisible[l_iAreaID] = true;
		Area* l_pArea = HashTable[m_iMFIELD].GetTopArea(l_iAreaID);
		if (l_pArea) {
			for ( int i=0; i<l_pArea->NLinks; i++ ){
				m_piVisible[ (l_pArea->Link[i]).NextAreaID ] = true;
				EXP_Update((l_pArea->Link[i]).NextAreaID);
			};	
		};
	};
};
bool		be_HashTopTable::VSB_UpdateTRIADA(){
	bool UPDATES = false;
//	int l_piVSB[10];
	int nVisibleLinks = 0;
	for ( int i=0; i<m_iNAreas; i++ ){
		if (m_piVisible[i]==false) {
		//	for ( int vsb=0; vsb<10; vsb++ ) l_piVSB[vsb]=-1;
			nVisibleLinks=0;
			Area* l_pArea = HashTable[m_iMFIELD].GetTopArea(i);
			if (l_pArea) {
				for ( int j=0; j<l_pArea->NLinks; j++ ){
					if ( m_piVisible[ (l_pArea->Link[j]).NextAreaID ] ){
		//				l_piVSB[nVisibleLinks] = (l_pArea->Link[j]).NextAreaID;
                        nVisibleLinks++;
					};
				};	
				if ( nVisibleLinks>0 && l_pArea->NLinks/nVisibleLinks<=2 ){
					m_piVisible[i]=true;
					EXP_Update(i);
					UPDATES = true;
				};
			};
		//	if (nVisibleLinks>=3){
		//		int maxVLink = 0;
		//		Area* l_pArea = NULL;
		//		for ( int v=0; (v<nVisibleLinks)&&(v<10)&&(maxVLink<2); v++ ){
		//			l_pArea = HashTable[m_iMFIELD].GetTopArea(l_piVSB[v]);
		//			for ( int j=0; j<l_pArea->NLinks; j++ ){
		//				for ( int l=0; (l<nVisibleLinks)&&(l<10); l++ ){
		//					if ( (l_pArea->Link[j]).NextAreaID == l_piVSB[l] ) maxVLink++;
		//				};
		//			};
		//			if ( maxVLink<2 ) maxVLink=0;
		//		};
		//		if ( maxVLink>=2 ){
		//			m_piVisible[i]=true;
		//			UPDATES = true;
		//		};
		//	};
		};
	};
	return UPDATES;
};

//////////////////////////////////////////////////////////////////////////
bool	g_beCheckForShowTopArea(be_HashTopTable& pHTT,int iAreaID){
	if (DriveMode()->NOT_ZERO_ANLY==false) return true;
	if (pHTT.m_piAssist[iAreaID] || pHTT.m_piVisible[iAreaID] || 
		(pHTT.m_piExploringRate[iAreaID]!=0 && pHTT.m_piExploringRate[iAreaID]!=0xFFFF ) ) return true;
	return false;
};
bool	g_beGET_WAY(be_HashTopTable& HTT,int sAreaID,int dAreaID,LinearArray<int,_int>& AreasList){
	if ( sAreaID<0 || dAreaID<0 ) return false;
	if ( HashTable[HTT.m_iMFIELD].GetNextAreaOnTheWayTo(sAreaID,dAreaID,HTT.m_iNation)>=0xFFFE ) return false;
	Area* pArea00 = NULL;	int AreaID00 = sAreaID;
	Area* pArea01 = NULL;	int AreaID01 = 0xFFFF;

	AreasList.Add(AreaID00);
	while ( AreaID00!=dAreaID ) {
		pArea00 = HashTable[HTT.m_iMFIELD].GetTopArea(AreaID00);
		if ( pArea00!=NULL ){
			AreaID01 = HashTable[HTT.m_iMFIELD].GetNextAreaOnTheWayTo(dAreaID,AreaID00,HTT.m_iNation);
			if ( AreaID01==AreaID00 ) return true;
			pArea01 = HashTable[HTT.m_iMFIELD].GetTopArea(AreaID01);
			if ( pArea01!=NULL ){
				AreasList.Add(AreaID01);
			};
		};
		AreaID00 = AreaID01;
	};
	return true;
};
void	g_beDRAW_WAY(be_HashTopTable& HTT,int sAreaID,int dAreaID){
	if ( sAreaID<0 || dAreaID<0 ) return;
	if ( HashTable[HTT.m_iMFIELD].GetNextAreaOnTheWayTo(sAreaID,dAreaID,HTT.m_iNation)>=0xFFFE ) return;
	Area* pArea00 = NULL;	int AreaID00 = sAreaID;
	Area* pArea01 = NULL;	int AreaID01 = 0xFFFF;
    
	while ( AreaID00!=dAreaID ) {
		pArea00 = HashTable[HTT.m_iMFIELD].GetTopArea(AreaID00);
		if ( pArea00!=NULL ){
			AreaID01 = HashTable[HTT.m_iMFIELD].GetNextAreaOnTheWayTo(dAreaID,AreaID00,HTT.m_iNation);
			if ( AreaID01==AreaID00 ) return;
			pArea01 = HashTable[HTT.m_iMFIELD].GetTopArea(AreaID01);
			if ( pArea01!=NULL ){
				GPS.DrawLine(	Vector3D(pArea00->x*64+32,pArea00->y*64+32,GetHeight(pArea00->x*64+32,pArea00->y*64+32)), 
								Vector3D(pArea01->x*64+32,pArea01->y*64+32,GetHeight(pArea01->x*64+32,pArea01->y*64+32)), 
								0xFF0000ff
							);			
			};
		};
		AreaID00 = AreaID01;
	};
};
void	g_beDRAW_STORED_WAY(int lockType){
	if ( lockType<0 || lockType>3 ) return;
	if ( NWPTS<=0 ) return;
	Area* pArea0 = HashTable[lockType].GetTopArea(FULLWAY[NWPTS-1]);
	Area* pArea1 = NULL;
	int w = NWPTS-1;
	while ( w-- ){
		pArea1 = HashTable[lockType].GetTopArea(FULLWAY[w]);
		if ( pArea0!=NULL && pArea1!=NULL ){
			Vector3D	p0(pArea0->x*64+32,pArea0->y*64+32,GetHeight(pArea0->x*64+32,pArea0->y*64+32));
			Vector3D	p1(pArea1->x*64+32,pArea1->y*64+32,GetHeight(pArea1->x*64+32,pArea1->y*64+32));
			GPS.DrawLine( p0, p1, 0xFFffff00 );
		};
		pArea0 = pArea1;
	};
};// Draw way stored in FULLWAY areas array
void	g_beDRAW_HashTopTable(be_HashTopTable& pHTT){

	if ( DriveMode()->ShowHTT==false ) return;

	//rsEnableZ(true);

	pHTT.SafeUpdate(DriveMode()->UpdateDelta);
	
	Vector3D  XYZc = ICam->GetLookAt();
	int x   = ( ( ((int)XYZc.x-RealLx/2)/64-4>0 ) ? (((int)XYZc.x-RealLx/2)/64-4) : (0) );
	int y   = ( ( ((int)XYZc.y-RealLy)/64-8>0 ) ? (((int)XYZc.y-RealLy)/64-8) : (0) );
	int nx	= RealLx/64+12;
	int ny	= RealLy/32+14;

	int cx,cy;
	for ( int dy=0; dy<ny; dy++ )
	for ( int dx=0; dx<nx; dx++ )
	{
		cx = x+dx;
		cy = y+dy;

		Vector3D v0( (float)(cx*64)		, (float)(cy*64)	, (float)GetHeight(cx*64,cy*64)		 );
		Vector3D v1( (float)(cx*64+64)	, (float)(cy*64)	, (float)GetHeight(cx*64+64,cy*64)	 );
		Vector3D v2( (float)(cx*64)		, (float)(cy*64+64)	, (float)GetHeight(cx*64,cy*64+64)	 );
		Vector3D v3( (float)(cx*64+64)	, (float)(cy*64+64)	, (float)GetHeight(cx*64+64,cy*64+64));

		DWORD color = 0xFFffffff;

		int   iAreaID = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx,cy);
        if ( iAreaID!=0xFFFF && iAreaID<0xFFFE ){
			
			if ( g_beCheckForShowTopArea(pHTT,iAreaID) )
			{
				Area* pArea = HashTable[pHTT.m_iMFIELD].GetTopArea( iAreaID );
				if (pArea) {
					if ( (pHTT.m_piAssist && pHTT.m_piAssist[iAreaID]) || (pHTT.m_piVisible && pHTT.m_piVisible[iAreaID]) ){
						color = 0xFFffff00;
						if ( HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx-1,cy)!=iAreaID ) GPS.DrawLine(v0,v2,color);
						if ( HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx+1,cy)!=iAreaID ) GPS.DrawLine(v1,v3,color);
						if ( HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx,cy-1)!=iAreaID ) GPS.DrawLine(v0,v1,color);
						if ( HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx,cy+1)!=iAreaID ) GPS.DrawLine(v2,v3,color);
					}else{
						int privArea = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx,cy-1);
						bool PAID = (privArea!=0xFFFF && privArea<0xFFFE && privArea>=0);
						if ( PAID && privArea!=iAreaID && !pHTT.m_piAssist[privArea] && !pHTT.m_piVisible[privArea] ) 
							GPS.DrawLine(v0,v1,color);
						privArea = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx-1,cy);
						PAID = (privArea!=0xFFFF && privArea<0xFFFE && privArea>=0);
						if ( PAID && privArea!=iAreaID && !pHTT.m_piAssist[privArea] && !pHTT.m_piVisible[privArea] ) 
							GPS.DrawLine(v0,v2,color);
						if ( pHTT.m_piExploringRate[iAreaID]!=0 && pHTT.m_piExploringRate[iAreaID]!=0xFFFF ){
							privArea = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx+1,cy);
							PAID = (privArea!=0xFFFF && privArea<0xFFFE && privArea>=0);
							if ( PAID && privArea!=iAreaID && !pHTT.m_piAssist[privArea] && !pHTT.m_piVisible[privArea] ) 
								GPS.DrawLine(v1,v3,color);
							privArea = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx,cy+1);
							PAID = (privArea!=0xFFFF && privArea<0xFFFE && privArea>=0);
							if ( PAID && privArea!=iAreaID && !pHTT.m_piAssist[privArea] && !pHTT.m_piVisible[privArea] ) 
								GPS.DrawLine(v2,v3,color);
						};
					};
					

					if ( pArea->x==cx && pArea->y==cy ){
						if ( DriveMode()->ShowLinks ){
							for ( int l=0; l<pArea->NLinks; l++ ){
								if ( (pArea->Link[l]).NextAreaID > iAreaID ){
									if ( g_beCheckForShowTopArea(pHTT,(pArea->Link[l]).NextAreaID) ){
										Area* pLinkArea = HashTable[pHTT.m_iMFIELD].GetTopArea( (pArea->Link[l]).NextAreaID );
										if ( pLinkArea ) {
											GPS.DrawLine(	Vector3D(pArea->x*64+32,pArea->y*64+32,GetHeight(pArea->x*64+32,pArea->y*64+32)), 
															Vector3D(pLinkArea->x*64+32,pLinkArea->y*64+32,GetHeight(pLinkArea->x*64+32,pLinkArea->y*64+32)), 
															0xFF603913
														);
										};
									};
								};
							};
						};

						GPS.DrawLine(	Vector3D(cx*64+32-10,cy*64+32,GetHeight(cx*64+32-10,cy*64+32)), 
										Vector3D(cx*64+32+10,cy*64+32,GetHeight(cx*64+32+10,cy*64+32)), 
										color 
									);
						GPS.DrawLine(	Vector3D(cx*64+32,cy*64+32-10,GetHeight(cx*64+32,cy*64+32-10)), 
										Vector3D(cx*64+32,cy*64+32+10,GetHeight(cx*64+32,cy*64+32+10)), 
										color 
									);
						char  s[64];
						Vector3D v( cx*64+32, cy*64+32, GetHeight(cx*64+32,cy*64+32) );
						WorldToScreenSpace(v);
						if (DriveMode()->ShowEXP){
							sprintf(s,"%s%d","EXP = ",pHTT.m_piExploringRate[iAreaID]);
							ShowString(v.x+5,v.y+5,s,&SmallWhiteFont);			
						};
						if (DriveMode()->ShowAST){
							if ( pHTT.m_piAssist[iAreaID]!=0 ){
								sprintf(s,"%s%d","AST = ",pHTT.m_piAssist[iAreaID]);
								ShowString(v.x+5,v.y+5+10,s,&SmallWhiteFont);		
							}else{
								if ( pHTT.m_piAssistAdd[iAreaID]!=0 ){
									sprintf(s,"%s%d","AST ADD = +",pHTT.m_piAssistAdd[iAreaID]);
									ShowString(v.x+5,v.y+5+10,s,&SmallWhiteFont);		
								};
							};
						};
						if (DriveMode()->ShowVSB){	
							sprintf(s,"%s%d","VSB = ",pHTT.m_piVisible[iAreaID]);
							ShowString(v.x+5,v.y+5+20,s,&SmallWhiteFont);		
						};
						if (DriveMode()->ShowID){
							sprintf(s,"%s%d","ID  = ",iAreaID);
							ShowString(v.x+5,v.y+5+30,s,&SmallWhiteFont);		
						};
						if (DriveMode()->ShowXY){
							sprintf(s,"%s%d%s%d%s","XY  = (",cx,":",cy,")");
							ShowString(v.x+5,v.y+5+40,s,&SmallWhiteFont);		
						};
						if (DriveMode()->Showxy){
							sprintf(s,"%s%d%s%d%s","xy  = (",cx*64+32,":",cy*64+32,")");
							ShowString(v.x+5,v.y+5+50,s,&SmallWhiteFont);		
						};
					};
				};
			};
		}else{
			int privArea = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx-1,cy);
			if (privArea!=0xFFFF){
				if ( (pHTT.m_piAssist && pHTT.m_piAssist[privArea]) || (pHTT.m_piVisible && pHTT.m_piVisible[privArea]) )	
					color = 0xFFffff00;
				else															
					color = 0xFFffffff;
				GPS.DrawLine(v0,v2,color);
			}else
				GPS.DrawLine(v0,v2,0xFFff0000);

			privArea = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx+1,cy);
			if (privArea!=0xFFFF){
				if ( (pHTT.m_piAssist && pHTT.m_piAssist[privArea]) || (pHTT.m_piVisible && pHTT.m_piVisible[privArea]) )	
					color = 0xFFffff00;
				else															
					color = 0xFFffffff;
				GPS.DrawLine(v1,v3,color);
			}else
				GPS.DrawLine(v1,v3,0xFFff0000);

			privArea = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx,cy-1);
			if (privArea!=0xFFFF){
				if ( (pHTT.m_piAssist && pHTT.m_piAssist[privArea]) || (pHTT.m_piVisible && pHTT.m_piVisible[privArea]) )	
					color = 0xFFffff00;
				else															
					color = 0xFFffffff;
				GPS.DrawLine(v0,v1,color);
			}else
				GPS.DrawLine(v0,v1,0xFFff0000);

			privArea = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(cx,cy+1);
			if (privArea!=0xFFFF){
				if ( (pHTT.m_piAssist && pHTT.m_piAssist[privArea]) || (pHTT.m_piVisible && pHTT.m_piVisible[privArea]) )	
					color = 0xFFffff00;
				else															
					color = 0xFFffffff;
				GPS.DrawLine(v2,v3,color);
			}else
				GPS.DrawLine(v2,v3,0xFFff0000);

			GPS.DrawLine(v0,v3,0xFFff0000);
			GPS.DrawLine(v1,v2,0xFFff0000);
		};
	};

	g_beDRAW_WAY(pHTT,DriveMode()->sAreaID,DriveMode()->dAreaID);

	if ( DriveMode()->DrawStoreWAY )	g_beDRAW_STORED_WAY(DriveMode()->LockType);

	GPS.FlushLines();

	rsEnableZ(false);
	
	//char  s[64];
	//for ( int i=0; i<nx; i++ ){
	//	for ( int j=0; j<ny; j++ ){
	//		iAreaID = HashTable[pHTT.m_iMFIELD].GetTopAreaIndex(x+i,y+j);
	//		if ( iAreaID!=0xFFFF ){
	//			pArea = HashTable[pHTT.m_iMFIELD].GetTopArea( iAreaID );
	//			if (pArea) {
	//				//sprintf(s,"%s%d","EXP = ",pHTT.m_piExploringRate[iAreaID]);
	//				//ShowString(p.x,p.y,s,&SmallWhiteFont);			
	//				//sprintf(s,"%s%d","AST = ",pHTT.m_piAssist[iAreaID]);
	//				//ShowString(p.x,p.y+10,s,&SmallWhiteFont);			
	//				//sprintf(s,"%s%d","VSB = ",pHTT.m_piVisible[iAreaID]);
	//				//ShowString(p.x,p.y+20,s,&SmallWhiteFont);		
	//				//sprintf(s,"%s%d","ID  = ",iAreaID);
	//				//ShowString(p.x,p.y+30,s,&SmallWhiteFont);		
	//			};
	//		};
	//	};
	//};
};
//////////////////////////////////////////////////////////////////////////
// Class for store and manage HT for all nations
//////////////////////////////////////////////////////////////////////////
void				be_NationHTT::Initialize(int _Nation){
	m_Nation = _Nation;
	(*this)[HTT_LAND].Initialize(m_Nation,HTT_LAND);
	(*this)[HTT_WATER].Initialize(m_Nation,HTT_WATER);
	(*this)[HTT_LOWAIR].Initialize(m_Nation,HTT_LOWAIR);
	(*this)[HTT_HIGHAIR].Initialize(m_Nation,HTT_HIGHAIR);
};
void				be_NationHTT::SafeUpdate(int delta){
	if ( NtNUnits[m_Nation] ){
		(*this)[HTT_LAND].SafeUpdate(delta);
		(*this)[HTT_WATER].SafeUpdate(delta);
		(*this)[HTT_LOWAIR].SafeUpdate(delta);
		(*this)[HTT_HIGHAIR].SafeUpdate(delta);
	};
};
be_HashTopTable&	be_NationHTT::operator[](int _idx){
	switch(_idx) {
	case HTT_LAND:
		return LAND_HTT;
		break;
	case HTT_WATER:
		return WATER_HTT;
		break;
	case HTT_LOWAIR:
		return LOWAIR_HTT;
		break;
	case HTT_HIGHAIR:
		return HIGHAIR_HTT;
		break;
	};
	assert(0);
	return	LAND_HTT;
};
be_HashTopTableList		g_beHTT_LIST;	// Hash Top Table for all nation
be_HashTopTableList::be_HashTopTableList(){
	for ( int _Nation=0; _Nation<8; _Nation++ ){
		HTT[_Nation].Initialize(_Nation);
	};
};
be_HashTopTableList::~be_HashTopTableList(){
	
};
void	be_HashTopTableList::SafeUpdate(int delta){
	for ( int _Nation=0; _Nation<8; _Nation++ ){
		HTT[_Nation].SafeUpdate(delta);
	};
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


// searching functions
be_HashTopTable* pCURR_HTT = NULL;
int		g_beSCOUT_FINDE_LIMIT = 0;

// SCOUT
word	g_beSCOUT_AI(int AID){
	g_beSCOUT_FINDE_LIMIT++;
	if ( pCURR_HTT->m_piExploringRate[AID]<1+(g_beSCOUT_FINDE_LIMIT/40)*10 && !pCURR_HTT->m_piVisible[AID] ){
		if ( rando()%6==0 ){
			return 1000;
		};
	}
	return 0xFFFF;
};
void	g_beSCOUT_UPDATE_HTT(){
	short* r = NULL;
	for ( int i=0; i<NWPTS; i++ ){
		r = &(pCURR_HTT->m_piExploringRate[ FULLWAY[i] ]);
		*r = ( (*r+(g_beSCOUT_FINDE_LIMIT/40+1)*10+30>=100) ? (100) : (*r+(g_beSCOUT_FINDE_LIMIT/40+1)*10) );
	};
};
bool	g_beFillFULLWAY_SCOUT(int x_pix,int y_pix){
	pCURR_HTT = &(g_beHTT_LIST[MyNation][HTT_LAND]);
	int start = HashTable[pCURR_HTT->m_iMFIELD].GetTopAreaIndex(x_pix/64,y_pix/64);
	int  MaxDanger = 0;
	int  DangSteps = 0;	// not use
	word DestObj   = 0;
	g_beSCOUT_FINDE_LIMIT = 0;
	FindNextZoneOnTheSafeWayToObjectWithCallback(	start,
													pCURR_HTT->m_piExploringRate,
													g_beSCOUT_AI,
													&MaxDanger,
													DangSteps,
													&DestObj
												);	
	g_beSCOUT_UPDATE_HTT();
	pCURR_HTT = NULL;
	return true;
};

// ATTACK
bool	g_beFillFULLWAY_ATTACK(int start,int finish,int myNation,int& MaxDanger,int DangSteps,int layer/*=HTT_LAND*/){
	if ( myNation<0 || myNation>8 ) return false;
	if ( layer<0 || layer>3 ) return false;	
	if ( start<0 || start>=HashTable[layer].NAreas || finish<0 || finish>=HashTable[layer].NAreas ) return false;
	// memory allotment
	int i= 0;
	int* pDM = new int[ HashTable[layer].NAreas ];
	for ( i=0; i<HashTable[layer].NAreas; i++ ) pDM[i] = 0;
	for ( i=0; i<HashTable[layer].NAreas; i++ ){
		for ( int ii=0; ii<8; ii++ ){
			if ( g_beHTT_LIST[ii][layer].m_iCreate && g_beHTT_LIST[ii][layer].m_iInit ){
				if ( ( NATIONS[ii].NMask & NATIONS[myNation].NMask ) == 0 ){
					pDM[i] += g_beHTT_LIST[ii][layer].m_piAssist[i]*10 + g_beHTT_LIST[ii][layer].m_piAssistAdd[i];
				};
			};
		};
	};
	// ----------------
	//int  MaxDanger = 0; // max danger on way
	//int  DangSteps = 0;	// max way length
	FindNextZoneOnTheSafeWayEx2(	
		start,
		finish,
		pDM,
		&MaxDanger,
		DangSteps,
		layer
	);	
	// free memory
	if ( pDM!=NULL ) delete [] pDM;
	// -----------
	return true;
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void	be_REG_CLASS_HashTop(){
	REG_CLASS(be_CAreaPowerData);	
	REG_CLASS(be_CAreaListPowerData);
	REG_CLASS(be_CTotalALPD);
};
//////////////////////////////////////////////////////////////////////////


































