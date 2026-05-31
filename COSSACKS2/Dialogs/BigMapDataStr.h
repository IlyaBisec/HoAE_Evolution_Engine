
extern	int		g_dXM,g_dYM;							// Координаты верхнего левого угла карты.
extern	int		g_WM,g_HM;								// Реальная ширина и высота карты.

extern	RECT	g_MAP_RECT;
extern	CHero*	pAtackerHero;							// Герой атакующий сектор.
extern	CHero*	pDefendeHero;							// Герой защитник.

//Указатели на основные управляющие классы.
extern	CROOTDRIVER			ROOT_SCENARIO;
extern	CMISSIONINIT		MissionInitData;			// Данные по инициализации миссии.	// MISSION INIT DATA.
extern	CEndMissionData		EndMissionData;				// SAVE MISSION END DATA.
extern	CSAVE_MIS_STRUCT	smsSAVE;
extern	CScript				SCRIPT_DIRECTION;			// Данные по направлениям ударов.
extern	CPlayerSAVE_STRUCT*	pSAVE;						// Структура с данными по игроку.
extern	int		gi_bmSAVE_Nation;						// nation id (from player Save)
extern	int		gi_bmMISS_FRAG_LIMIT;					// frag-limit для отрядов в миссиях

//**********************************************************************************
//************************  FOR USE IN MISSION ENGINE  *****************************
//**********************************************************************************
CIMPORT
int GetFormationIndexBySizeIndex(byte NI, word UnitType, byte SizeIndex);
CIMPORT
int PutNewFormationBySizeIndex(byte NI, word UnitType, byte SizeIndex, int x, int y, byte Dir=0);
CIMPORT
int PutNewFormationBySizeIndex(byte NI,char* UnitType, byte SizeIndex, int x, int y, byte Dir=0);//grey
CIMPORT
void SetCurrentProgress(char* Message,int Stage,int maxStage);
CIMPORT
void AddBrigadeToGroup(byte NI, GAMEOBJ* pGroup, int Brig_ID);
CIMPORT		
word GetObjectBranch(char* NDS_Name);	// род войск юнита: 0 - пехота, 1 - конница, 2 - пушки 
CIMPORT		
word GetObjectBranch(word NIndex);		// род войск юнита: 0 - пехота, 1 - конница, 2 - пушки 
DLLEXPORT
int GetTotalAmount0(GAMEOBJ* Units);
DLLEXPORT 
int GetTotalAmount1(GAMEOBJ* UnitType,byte Nation);
DLLEXPORT
void ClearSelection(byte Nat);
DLLEXPORT
void SaveSelectedUnits(byte NI,GAMEOBJ* Units,bool add);
DLLEXPORT
int InsertUnitToGroup(GAMEOBJ* Src,GAMEOBJ* Dst,int Index);
DLLEXPORT
int GetKilled(GAMEOBJ* Units);
DLLEXPORT
void SelectUnitsType(GAMEOBJ* UnitsType,byte Nat,bool Add);

//**********************************************************************************
//----------------------------------------------------------------------------------
void	BigMapPause( float time ) // grey [22.03.2004]
{
	int  tt2 = 0;
	int  tt		 = GetTickCount();
	int  counter = time * 1000 + tt ;

	do { tt2 = GetTickCount(); } 
	while( counter - tt2 > 0 );

	tt = GetTickCount();
}
//----------------------------------------------------------------------------------
void	BigMapPause() // grey [22.03.2004]
{
	float  a_PAUSE = 1 ; //2 ; 			// seconds:   1 = 1 sec
	BigMapPause( a_PAUSE );
}
//----------------------------------------------------------------------------------
void	BigMapSetMessage( TextButton* ptb , char* st ) {
	if( !ptb ) { return; }
	ptb->SetMessage( st ) ;
}
//----------------------------------------------------------------------------------
void	BigMapSetMessage( GP_TextButton* ptb , char* st ) {
	if( !ptb ) { return; }
	ptb->SetMessage( st ) ;
}
//----------------------------------------------------------------------------------
void	VityaError(char* Mess){
	MessageBox(NULL,Mess,"Vitya's error",MB_TOPMOST);
	assert(false);
}
//----------------------------------------------------------------------------------
int		GetBigMapPlayerColor(int nation) // Get Big Map Nation Color
{
	if( nation >= 0 && nation < 2*MAX_PLAYERS )  return  ROOT_SCENARIO.m_iMapColor[nation];
	return 255;

	//if( nation >= 0 && nation < 2*MAX_PLAYERS )  return  ROOT_SCENARIO.m_iMapColor[nation] & 0x00FFFFFF;
	//return	GetNatColor(PID) & 0x00FFFFFF; //DWORD g_NationalColors[] = 
}
//----------------------------------------------------------------------------------
bool	CreateDirectoryInCur(char* DirName){
	char	FilePath[_MAX_PATH];
	if (_getcwd(FilePath,_MAX_PATH) != NULL){
		if (_chdir(DirName)==0){
			_chdir(FilePath);
			return true;
		}
		else{
			if (_mkdir(DirName)==0){
				return true;
			}
			else{
				VityaError("Can't create directory : CreateDirectoryInCur();");
				return false;
			}
		}
	}
	else{
		VityaError("Can't get current directory : CreateDirectoryInCur();");
		return false;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool	PushOnTop(DialogsSystem* pdsMenu,int psdPic){
	//if (pdsMenu==NULL || psdPic==NULL)
	//	{
	//	return false;
	//	}

	//SimpleDialog*	psdSD = reinterpret_cast<SimpleDialog*>(psdPic);
	//int		i=0;
	//bool	finde = false;
	//while (pdsMenu->DSS[i]!=NULL)
	//	{
	//	if (pdsMenu->DSS[i] == psdSD)
	//		{
	//		finde = true;
	//		}
	//	if (finde)
	//		{
	//		pdsMenu->DSS[i] = pdsMenu->DSS[i+1];
	//		if (pdsMenu->DSS[i]==NULL)
	//			{
	//			pdsMenu->DSS[i] = psdSD;
	//			}
	//		}
	//	
	//	i++;
	//	};

	return true;
}
//----------------------------------------------------------------------------------
float	GetAngle(float x,float y){	//Угол между вектором (x,y) и (1,0); Fi>=0&&Fi<2*Pi
	const float Pi = 3.141592;

	float	dx = x;	if (x<0)	{ dx = -x; }
	float	dy = y;	if (y<0)	{ dy = -y; }

	//x==0||y==0
	if (dx<=0.01&&dy<=0.01)	{ return 0.0;			}
	if (dx<=0.01&&y>0.0)	{ return Pi/2.0;		}
	if (dx<=0.01&&y<0.0)	{ return (3.0*Pi)/2.0;	}
	if (dy<=0.01&&x>0.0)	{ return 0.0;			}
	if (dy<=0.01&&x<0.0)	{ return Pi;			}

	//x<>0&&y<>0
	float	Fi = acos(abs(x)/sqrt(x*x+y*y));

	if (x<0.0&&y>0.0)		{ Fi = Pi-Fi;		}
	if (x<0.0&&y<0.0)		{ Fi = Pi+Fi;		}
	if (x>0.0&&y<0.0)		{ Fi = 2.0*Pi-Fi;	}

	return	Fi;
}
//----------------------------------------------------------------------------------
void	RoundAngle(int Fi){
	while ( Fi<0 ) {
		Fi += 256;
	}
	if ( Fi>=256 ) {
		Fi = Fi%256;
	}
}
//----------------------------------------------------------------------------------
int		GetGoldForRess(int resID , int pop ) {
	int  res = -1;
	switch(resID) {
	case 1:			// FOOD						//Iron. 
		//return		SECT_INCOME[ FOOD ] + SECT_ADDING[ FOOD ] ;	
		res = FOOD ;
		break;
	case 2:			// GOLD						//Coal. 
		//return		SECT_INCOME[ GOLD ] + SECT_ADDING[ GOLD ] ;	
		res = GOLD ;
		break;
	case 3:			// IRON						//Gold. 
		//return		SECT_INCOME[ IRON ] + SECT_ADDING[ IRON ] ;	
		res = IRON ;
		break;
	case 4:			// COAL							//Weet. 
		//return		SECT_INCOME[ COAL ] + SECT_ADDING[ COAL ] ; 
		res = COAL ;
		break;
	default:		//No ressource.
		return		0;
	}
	if(res==-1)  return 0;
	return  SECT_INCOME[ res ] + SECT_ADDING[ res ] ; 
}
//----------------------------------------------------------------------------------
int		GetExpPercentByRankID( int expa, int rank, int& nextlvl )
{
	int ret = 0;
	switch( rank ) {      
		case 0:
				ret = 100 * expa / LTN_EXPS_LIMIT ;
				nextlvl = LTN_EXPS_LIMIT; // - expa ;
				break;
		case 1:
				ret = 100 * (expa - LTN_EXPS_LIMIT) / (CPT_EXPS_LIMIT - LTN_EXPS_LIMIT) ;	
				nextlvl = CPT_EXPS_LIMIT;// - expa ;
				break;
		case 2:
				ret = 100 * (expa - CPT_EXPS_LIMIT) / (MJR_EXPS_LIMIT - CPT_EXPS_LIMIT) ; 
				nextlvl = MJR_EXPS_LIMIT;// - expa ;
				#ifdef DemoAI
					ret = 100 ; 
					nextlvl = 0xFFFF ;
				#endif
				break;
		case 3:
				ret = 100 * (expa - MJR_EXPS_LIMIT) / (COL_EXPS_LIMIT - MJR_EXPS_LIMIT) ;	
				nextlvl = COL_EXPS_LIMIT;// - expa ;
				break; 
		case 4:
				ret = 100 * (expa - COL_EXPS_LIMIT) / (GEN_EXPS_LIMIT - COL_EXPS_LIMIT) ;	
				nextlvl = GEN_EXPS_LIMIT;// - expa ;
				break; 
		case 5:
				ret = 100 ;	// 100% экспы!!!
				nextlvl = 0xFFFF ;
	}
	if( rank < 5 && ret > 89 ) ret = 89; // && ret > 95 ) ret = 99;
	if( nextlvl == 0 )  nextlvl = 1;
	return ret ;
}
//----------------------------------------------------------------------------------
char*	GetRankByID(int rank){
	switch( rank ) {
	case 0:
		return GetTextByID("#LIEUTENANT");
	case 1:
		return GetTextByID("#CAPTAIN");
	case 2:
		return GetTextByID("#MAJOR");
	case 3:
		return GetTextByID("#COLONEL");
	case 4:
		return GetTextByID("#GENERAL");
	case 5:
		return GetTextByID("#MARSHAL");
	default:
		return NULL;
	}
}
//----------------------------------------------------------------------------------
char*	GetContByID(int cont){
	switch( cont ) {
	case 0:
		return GetTextByID("#ENGLAND");
	case 1:
		return GetTextByID("#FRANCE");
	case 2:
		return GetTextByID("#AUSTRIA");
	case 3:
		return GetTextByID("#PRUSSIA");
	case 4:
		return GetTextByID("#RUSSIA");
	case 5:
		return GetTextByID("#EGYPT");
	default:
		return NULL;
	}
}
//----------------------------------------------------------------------------------
int		GetSQNumByDefenceID( int SecDef , int NI , int SecPop , int SecID )
{
	int num = ROOT_SCENARIO.m_iGarrSqN[ 0 ]; // Start defending squads number	
	if( SecDef > 0 && SecDef <= MAX_DEFENCE )	{ 
		num = ROOT_SCENARIO.m_iGarrSqN[ SecDef ]; 
	}
	if( ROOT_SCENARIO.m_pSect->GetResourceID( SecID ) == 2 ) // CAPITAL SECTOR (main res - Gold)
		num += ROOT_SCENARIO.m_iGarrAdd[ 0 ];

	if( NI == -1 ) {
		num  = ROOT_SCENARIO.m_iRebelAdd[ 0 ];
		if( SecPop > 0 && SecPop <= 2 ) { 
			num = ROOT_SCENARIO.m_iRebelAdd[ SecPop ];
		}
	}
	return num ;
}
//----------------------------------------------------------------------------------
int		SEC_DEF_POWER( int defence ) 
{
	int  Add	= ROOT_SCENARIO.m_iAddSecDefPower;		if( Add <= 0 )		Add   =  2;
	int  Power	= ROOT_SCENARIO.m_iMinSecDefPower;		if( Power <= 0 )	Power = 11;
	
	return  ( Power + Add * defence + defence/2 );
	//#define SEC_DEF_POWER(defence) ( 11 + 2*defence )
}
//----------------------------------------------------------------------------------
void	setCoordSD( SimpleDialog* psd, int x, int y ) {
	if( !psd )  return; 
	int	addX = x - psd->x;
	int	addY = y - psd->y;

	psd->ShiftDialog(addX,addY);
}
//----------------------------------------------------------------------------------
int		GetSquadAmountByType( byte Nation, word NIndex )
{
	// кол-во солдат в отряде ( из NATIONS )
	int  sqNUM = 1 ;	
	word OffID, BarabanID, FlagID;
	if( GetOfficersType( Nation , NIndex , OffID, BarabanID, FlagID) ) { 
		GeneralObject* GO=NATIONS[ Nation ].Mon[ OffID ];
		sqNUM = GO->OFCR->SDES[0].Amount[0];
	}
	return  sqNUM;
}
//----------------------------------------------------------------------------------
bool	GetBigMapSquadCost( byte Nation , word NIndex , int& sqNUM , int* sqCOST )
{
	for(int res=0; res<=COAL ; res++)   sqCOST[res] = 0; 

	// кол-во солдат в отряде ( из NATIONS )
	sqNUM = 1 ;	
	word OffID, BarabanID, FlagID;
	if( GetOfficersType( Nation , NIndex , OffID, BarabanID, FlagID) ) { 
		GeneralObject* GO=NATIONS[ Nation ].Mon[ OffID ];
		sqNUM = GO->OFCR->SDES[0].Amount[0];
	}

	// формирование массива стоимости отряда sqCOST
	int OffCOST[8], BarCOST[8], FlgCOST[8];  for(int i=0; i<8; i++) OffCOST[i] = BarCOST[i] = FlgCOST[i] = 0 ;	
	if( OffID != NIndex) {
		if(	OffID != 0xFFFF )		GetUnitCost( Nation , OffID ,	  OffCOST ,-1,-1);
		if(	FlagID != 0xFFFF )		GetUnitCost( Nation , FlagID ,	  FlgCOST ,-1,-1);
		if(	BarabanID != 0xFFFF )	GetUnitCost( Nation , BarabanID , BarCOST ,-1,-1);
	}
	if( sqNUM == 1 )	{ sqCOST[FOOD] += 4*50 ; }					// artillery 
	// стоим. 1 юнита
	int COST4[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 } ;	
	GetUnitCost( Nation , NIndex , COST4 ,-1,-1);
	// соим. отряда
	for(int res=0; res<=COAL ; res++)   sqCOST[res] += sqNUM * COST4[res] +OffCOST[res]+BarCOST[res]+FlgCOST[res];

	// кол-во юнитов в отряде
	if( sqNUM == 1 )	{ sqNUM = SQ_Amount2 ; }					// artillery

	return true;
}
//----------------------------------------------------------------------------------
DLLEXPORT	char*	GetResourseName( int id ) { //grey
	if( id<0 || id>5 ) return " ";
	return RDS[id].Name;
}
//----------------------------------------------------------------------------------
//---------------------------------------------------------------
class CArmyXY : public BaseClass
{
public:
	int		Type,N,H,W,aH,aW,maxW,X,Y;

	CArmyXY()	{ Type=0; N=0; H=0; W=0; aH=0; aW=0; maxW=0; X=0; Y=0; };
};
class STYPENUM : public BaseClass 
{
public:
	STYPENUM() { ID=0; N=0; /*NAME = "";*/};
	int ID;
	int N;	//_str NAME;
};
int MinLV(int a,int b){
	if (a<=b)	return a;
	return b;
}
int	MaxLV(int a,int b){
	if (a>=b)	return a;
	return b;
}
void	sortSQUARD_LINE(int* pLINE,int N){
	
}
DLLEXPORT
void	CreateHeroArmy1(GAMEOBJ* pGroup, int NI, ClassArray<CTYPENUM>* pArmy, int X0, int Y0, int Fi ){
	if (X0==0&&Y0==0&&Fi==0)	return;
	if ( pArmy==NULL || pArmy->GetAmount()==0 )	return;

	RoundAngle(Fi);

	//Разбить армию на конницу и пехоту.
	ClassArray<STYPENUM>	Pehota;	//ПЕХОТА.
	ClassArray<STYPENUM>	Kavalr;	//КАВАЛЕРИЯ.
	STYPENUM*				pTMP=NULL;
	for (int i=0; i<pArmy->GetAmount(); i++){
		pTMP = new STYPENUM();
		pTMP->ID = ((*pArmy)[i])->TypeIndex_ID;
		pTMP->N  = ((*pArmy)[i])->nSquarnd;
		if (GetObjectBranch(pTMP->ID)==1)	Kavalr.Add(pTMP);
		else								Pehota.Add(pTMP);
		pTMP=NULL;
	}

	//Подщитаем кол-во отрядов в арми.
	int NSP=0;	//Кол-во трядов ПЕХОТЫ.
	for (int i=0; i<Pehota.GetAmount(); i++){
		NSP += (Pehota[i])->N;
	}
	int NSK=0;	//Кол-во отрядов КАВАЛЕРИИ.
	for (int i=0; i<Kavalr.GetAmount(); i++ ){
		NSK += (Kavalr[i])->N;
	}
	int	N_SQARD = NSK+NSP;

	int   XNP  = 0, YNP = 0;
	int** pPEH = NULL;
	bool  bP   = false;

	//ПЕХОТА
	if (NSP>0){
		bP=true;
		//Посчитаем размер сетки.
		YNP=0;									//Кол-во отрядов по вертикали. ПЕХОТА.
		XNP=(int)(ceil(sqrt((double)NSP))+0.1);	//Кол-во отрядов по горизонтали. ПЕХОТА.
		if (XNP*(XNP-1)>=NSP){ YNP=XNP-1;}
		else				 { YNP=XNP;	 }

		//Выделить память для армии.
		pPEH = new int*[YNP];		//ПЕХОТА.
		for (int i=0; i<YNP; i++ )	{
			pPEH[i] = new int[XNP+1]; 
			for (int ii=0; ii<XNP+1; ii++){ pPEH[i][ii]=-1; }
			pPEH[i][0]=0;
		}

		//Расталкать армию по ячейкам. ПЕХОТА.
		while (NSP>0) {
			for (int j=0; j<YNP; j++){
				int delta = 0;
				int curID = -1;
				for (int i=0;i<Pehota.GetAmount();i++){
					if ( (Pehota[i])->N > 0 && (abs(XNP-pPEH[j][0]-(Pehota[i])->N)<=delta||curID==-1) ){
						delta = abs(XNP-pPEH[j][0]-(Pehota[i])->N);
						curID = i;
					}
				}
				if (curID!=-1){
					while (pPEH[j][0]<XNP&&(Pehota[curID])->N>0){
						pPEH[j][pPEH[j][0]+1] = (Pehota[curID])->ID; //pNAMES[j] = (Pehota[curID])->NAME;//grey
						pPEH[j][0]			+= 1;
						(Pehota[curID])->N	-= 1;
						NSP--;
					}
				}
			}
		}
		
		//Растосовка внутри одного строя по типам для красаты. ПЕХОТА.
		for (int i=0;i<YNP;i++){
			sortSQUARD_LINE(pPEH[i],XNP);
		}
	}

	int XNK=0,YNK=0;
	int** pKAV=NULL;
	bool	bK=false;
	//КАВАЛЕРИЯ
	if (NSK>0) {
		bK=true;
		//Посчитаем размер сетки.
		XNK=2;									//Кол-во отрядов по вертикали. КАВАЛЕРИЯ.
		YNK=(int)(ceil((double)NSK/2.0)+0.1);	//Кол-во отрядов по горизонтали. КАВАЛЕРИЯ.
//		if (YNK*(YNK-1)>=NSK){ XNK=YNK-1;}
//		else				 { XNK=YNK;	 }
		if (NSK==1)	{ XNK=1; YNK=1;	}
		if (NSK==2)	{ XNK=1; YNK=2; }		
		
		//Выделить память для армии.
		pKAV = new int*[YNK];		//КАВАЛЕРИЯ.
		for (int i=0; i<YNK; i++ )	{
			pKAV[i] = new int[XNK+1];
			for ( int ii=0; ii<XNK+1; ii++ ){ pKAV[i][ii]=-1; }
			pKAV[i][0]=0;
		}
		
		//Расталкать армию по ячейкам. КАВАЛЕРИЯ.
		while (NSK>0) {
			for (int j=0; j<YNK; j++){
				int delta = 0;
				int curID = -1;
				for (int i=0;i<Kavalr.GetAmount();i++){
					if ( (Kavalr[i])->N > 0 && (abs(XNK-pKAV[j][0]-(Kavalr[i])->N)<=delta||curID==-1) ){
						delta = abs(XNK-pKAV[j][0]-(Kavalr[i])->N);
						curID = i;
					}
				}
				if (curID!=-1){
					while (pKAV[j][0]<XNK&&(Kavalr[curID])->N>0){
						pKAV[j][pKAV[j][0]+1] = (Kavalr[curID])->ID;
						pKAV[j][0]			+= 1;
						(Kavalr[curID])->N	-= 1;
						NSK--;
					}
				}
			}
		}

		//Растосовка внутри одного строя по типам для красаты. КАВАЛЕРИЯ.
		for (int i=0;i<YNK;i++){
			sortSQUARD_LINE(pKAV[i],XNK);
		}
	}

	//Расстановка войск на карте.
	double rFi = ((double)Fi/256.0)*3.14159*2.0 - (64.0/256.0)*3.1415*2.0;
	int X0_P=0,Y0_P=0,X0_K=0,Y0_K=0;
	int dx_P=0,dy_P=0,dx_K=0,dy_K=0;
	if ( (0<=Fi&&Fi<=16)||(112<=Fi&&Fi<=144)||(239<=Fi&&Fi<=255) ) {
		dy_P=50;dx_P=100;
		dy_K=70;dx_K=120;
		}else if ( (48<=Fi&&Fi<=80)||(176<=Fi&&Fi<=208) ){
			dy_P=100;dx_P=50;
			dy_K=120;dx_K=70;
			}else if ( (16<Fi&&Fi<48)||(80<Fi&&Fi<112)||(144<Fi&&Fi<176)||(208<Fi&&Fi<239) ) {
				dy_P=70;dx_P=50;
				dy_K=90;dx_K=70;
				}else{
					dy_P=80;dx_P=80;
					dy_K=100;dx_K=100;
					}
	if (bP) {
		int fid = GetFormationIndexBySizeIndex(NI,(Pehota[0])->ID,0);
		dx_P += ElementaryOrders[fid].Width;
		dy_P += ElementaryOrders[fid].Hight;
	}
	if (bK) {
		int fid = GetFormationIndexBySizeIndex(NI,(Kavalr[0])->ID,0);
		dx_K += ElementaryOrders[fid].Width;
		dy_K += ElementaryOrders[fid].Hight;
	}
	if (bP&&bK){
		X0_P=-(dx_P*XNP)/2;
		Y0_P=(dy_P*YNP+dy_K*YNK)/2;
		X0_K=-(dx_K*XNK)/2;
		Y0_K=(dx_P*YNP+dy_K*YNK)/2-(dx_P*YNP)-100;
	}
	else if (bP){
		X0_P=-(dx_P*XNP)/2;
		Y0_P=(dy_P*YNP)/2;
	}
	else if (bK){
		X0_K=-(dx_K*XNK)/2;
		Y0_K=(dy_K*YNK)/2;
	}

	//Считаем координаты и ставим.
	int n_STAGE=0;
	if (bP){ // пехота
		for (int j=0; j<YNP; j++){
			for (int i=0; i<XNP; i++){
				int x = X0_P+dx_P/2+((XNP-pPEH[j][0])*dx_P)/2+i*dx_P;
				int y = Y0_P-dy_P/2-j*dy_P;
				int xr = (int)(((double)x*cos(rFi)-(double)y*sin(rFi))) + X0;
				int yr = (int)(((double)x*sin(rFi)+(double)y*cos(rFi))) + Y0;
				PutNewFormationBySizeIndex(NI,pPEH[j][i+1],0,xr<<4,yr<<4,Fi); 
				//SetCurrentProgress("Create Army",n_STAGE++,N_SQARD);
			}
		}
	}
	if (bK){ // кавалерия
		for (int j=0; j<YNK; j++){
			for (int i=0; i<XNK; i++){
				int x = X0_K+dx_K/2+((XNK-pKAV[j][0])*dx_K)/2+i*dx_K;
				int y = Y0_K-dy_K/2-j*dy_K;
				int xr = (int)(((double)x*cos(rFi)-(double)y*sin(rFi))) + X0;
				int yr = (int)(((double)x*sin(rFi)+(double)y*cos(rFi))) + Y0;
				PutNewFormationBySizeIndex(NI,pKAV[j][i+1], 0 ,xr<<4,yr<<4,Fi); //2
				//SetCurrentProgress("Create Army",n_STAGE++,N_SQARD);
			}
		}
	}

	//Очистить выделенную память.
	Pehota.Clear();
	Kavalr.Clear();
	if (bP){
		//Очистка памяти.
		for (int i=0; i<YNP; i++ )	{ delete[](pPEH[i]); }
		delete[]pPEH;
	}
	if (bK) {
		//Очистка памяти.
		for (int i=0; i<YNK; i++ ) { delete[](pKAV[i]); }
		delete[]pKAV;
	}
}
DLLEXPORT
void	CreateHeroArmy( GAMEOBJ* pGroup, int NI, ClassArray<CTYPENUM>* pArmy, int X0, int Y0, int Fi ){
	
	if (X0==0&&Y0==0&&Fi==0)	return;
	if ( pArmy==NULL || pArmy->GetAmount()==0 )	return;

	RoundAngle(Fi);
	
	//	0 - Создать массив данных
	ClassArray<CArmyXY>	ArmyXY;
	int					ArmyN = pArmy->GetAmount();

	int			nS = 0;
	CArmyXY*	pAXY = NULL;
	for ( int i=0; i<ArmyN; i++ ){
		nS = ((*pArmy)[i])->nSquarnd;
		if ( nS>0 ) {
			pAXY = new CArmyXY();
			pAXY->Type	= ((*pArmy)[i])->TypeIndex_ID;
			pAXY->N		= nS;
			ArmyXY.Add( pAXY );
			pAXY		= NULL;
		}
	}
	ArmyN = ArmyXY.GetAmount();

	//	1 - Сортируем по кол-ву отрядов.
	for (int i=0; i<ArmyN-1; i++ ){
		if ( (ArmyXY[i])->N < (ArmyXY[i+1])->N  ) {
			pAXY = ArmyXY[i];
			ArmyXY[i]	= ArmyXY[i+1];
			ArmyXY[i+1] = pAXY;
		}
	}

	//	2 - Задаём H,W,aH,aW. Подшитуем maxW,Y. 
	int fid = 0;
	int	MAX_W = 0;
	int	N_SQARD = 0;
	for (int i=0; i<ArmyN; i++ ){
		N_SQARD += (ArmyXY[i])->N;

		fid = GetFormationIndexBySizeIndex( NI,(ArmyXY[i])->Type,0);
		(ArmyXY[i])->H = ElementaryOrders[fid].Hight;
		(ArmyXY[i])->W = ElementaryOrders[fid].Width;
		
		if ( (0<=Fi&&Fi<=16)||(112<=Fi&&Fi<=144)||(239<=Fi&&Fi<=255) ) {
			(ArmyXY[i])->aH = 50;
			(ArmyXY[i])->aW = 100;
		}else if ( (48<=Fi&&Fi<=80)||(176<=Fi&&Fi<=208) ){
			(ArmyXY[i])->aH = 100;
			(ArmyXY[i])->aW = 50;
		}else if ( (16<Fi&&Fi<48)||(80<Fi&&Fi<112)||(144<Fi&&Fi<176)||(208<Fi&&Fi<239) ) {
			(ArmyXY[i])->aH = 70;
			(ArmyXY[i])->aW = 50;
		}else{
			(ArmyXY[i])->aH = 80;
			(ArmyXY[i])->aW = 80;
		}
		
		for ( int ii=0; ii<(ArmyXY[i])->N; ii++ ){
			(ArmyXY[i])->maxW += ((ArmyXY[i])->W) + 2*((ArmyXY[i])->aW);
		}
		if (MAX_W<(ArmyXY[i])->maxW) {
			MAX_W = (ArmyXY[i])->maxW;
		}
		if (i!=0) {
			(ArmyXY[i])->Y =	((ArmyXY[i-1])->Y)+((ArmyXY[i-1])->aH)+((ArmyXY[i])->aH)+
								((((ArmyXY[i-1])->H))+(((ArmyXY[i])->H)))/2;
		}
		else{
			(ArmyXY[i])->Y =	((ArmyXY[i])->H)/2 + (ArmyXY[i])->aH;
		}
	}
	
	//	3 - Подшитуем X для первого отряда каждого типа.
	int MAX_H = 0;
	for (int i=0; i<ArmyN; i++ ){
		(ArmyXY[i])->X = ((ArmyXY[i])->aW) + ((ArmyXY[i])->W)/2 + MAX_W/2 - ((ArmyXY[i])->maxW)/2;
		MAX_H += (ArmyXY[i])->H + 2*((ArmyXY[i])->aH);
	}
	
	//	4 - Перещет X и Y для конкретного отряда. Смещение, поворот. Установка отряда.
	int	XC	=	MAX_H/2; 
	int	YC	=	MAX_W/2;
	int	X=0,Y=0,dX=0;
	int x,y,xt,yt;
	double	rFi = (((double)Fi)/256.0)*3.1415*2.0 - (64.0/256.0)*3.1415*2.0;
	int	n_STAGE = 0;
	int BID = -1;
	for (int i=0; i<ArmyN; i++ ){
		Y	= (ArmyXY[i])->Y;
		dX	= (ArmyXY[i])->W + 2*((ArmyXY[i])->aW);
		for ( int ii=0; ii<(ArmyXY[i])->N; ii++ ){
			if ( ii!=0 ) {
				X += dX;
			}else{
				X = (ArmyXY[i])->X;
			}
			//Теперь у нас есть (X,Y) для тряда в моих координатах.
			x = X - MAX_W/2;
			y = Y - MAX_H/2;
			//Поворот.
			xt = (int)(((double)x)*cos(rFi)-((double)y)*sin(rFi));
			yt = (int)(((double)x)*sin(rFi)+((double)y)*cos(rFi));
			//Смещение.
			xt += X0;
			yt += Y0;
			//Установка.
			BID = PutNewFormationBySizeIndex(NI,(ArmyXY[i])->Type,0,xt<<4,yt<<4,Fi); 
			//PutNewFormationBySizeIndex(byte NI,char* UnitType, byte SizeIndex, int x, int y, byte Dir=0)
			if (pGroup!=NULL) AddBrigadeToGroup(NI,pGroup,BID);
			//SetCurrentProgress("Create Army",n_STAGE++,N_SQARD);
		}
	}

	//	5 - Убираем мусор.
	for (int i=0; i<ArmyN; i++ ){
		pAXY = ArmyXY[i];
		if (pAXY) { delete	pAXY; }
		ArmyXY[i]=NULL;
	}
	ArmyXY.Clear();
}


//**********************************************************************************
void	SaveCannonsForNation( ClassArray<CRestArmy_LV>& pArmy, int NI )
{
	char* art[50]  = { "ArtPus4(EN)",  "ArtPus4(FR)",  "ArtPus4(AU)",  "ArtPus4(PR)",  "ArtPus4(RU)",  "ArtPus4(EG)",  0 };
	char* artL[50] = { "ArtPus4L(EN)", "ArtPus4L(FR)", "ArtPus4L(AU)", "ArtPus4L(PR)", "ArtPus4L(RU)", "ArtPus4L(EG)", 0 };
	char* artG[50] = { "ArtPus4G(EN)", "ArtPus4G(FR)", "ArtPus4G(AU)", "ArtPus4G(PR)", "ArtPus4G(RU)", "ArtPus4G(EG)", 0 };
	char* artR[50] = { "ArtPus4R(EN)", 0 };

	GAMEOBJ Type;
	int ArtPus = 0,  ArtPusL = 0,  ArtPusG = 0 , ArtPusR = 0;
	for(int i=0; art[i]!=0 ; i++ ) {
		RegisterUnitType( &Type, art[i]  );		ArtPus  += GetTotalAmount1( &Type , NI );	
		RegisterUnitType( &Type, artL[i] );		ArtPusL += GetTotalAmount1( &Type , NI );	
		RegisterUnitType( &Type, artG[i] );		ArtPusG += GetTotalAmount1( &Type , NI );
	}

	GAMEOBJ CannonGrp;
	RegisterDynGroup(&CannonGrp);

	int NAT_ID = MissionInitData.AttackerNation_ID;
	if( NI == EndMissionData.DefenderNI )  NAT_ID = MissionInitData.DefenderNation_ID;
	if( ArtPus > 0 ) {
		RegisterUnitType( &Type, art[ NAT_ID ] );
		CRestArmy_LV* pSQ = new CRestArmy_LV();
		pSQ->inTypeID = Type.Index ;
		pSQ->setTypeName( art[ NAT_ID ] ) ; 
		pSQ->inAmount = ArtPus;
		pArmy.Add(pSQ);

		ClearSelection(NI);
		SelectUnitsType(&Type, NI , 0 );
		SaveSelectedUnits( NI, &CannonGrp , 0 );
		for(int id=0; id < GetTotalAmount0( &CannonGrp ) ; id++) {
			GAMEOBJ  OneGrp;
			RegisterDynGroup(&OneGrp);
			InsertUnitToGroup( &CannonGrp , &OneGrp , id );
			int kils = GetKilled( &OneGrp );
			pArmy[ pArmy.GetAmountOfElements()-1 ]->Kills.Add( GetKilled( &OneGrp ) );
		}
	}
	if( ArtPusL > 0 ) {
		RegisterUnitType( &Type, artL[ NAT_ID ] );
		CRestArmy_LV* pSQ = new CRestArmy_LV();
		pSQ->inTypeID = Type.Index ;
		pSQ->setTypeName( artL[ NAT_ID ] ) ; 
		pSQ->inAmount = ArtPusL ;
		pArmy.Add(pSQ);	

		ClearSelection(NI);
		SelectUnitsType(&Type, NI , 0 );
		SaveSelectedUnits( NI, &CannonGrp , 0 );
		for(int id=0; id < GetTotalAmount0( &CannonGrp ) ; id++) {
			GAMEOBJ  OneGrp;
			RegisterDynGroup(&OneGrp);
			InsertUnitToGroup( &CannonGrp , &OneGrp , id );
			pArmy[ pArmy.GetAmountOfElements()-1 ]->Kills.Add( GetKilled( &OneGrp ) );
		}
	}
	if( ArtPusG > 0 ) {
		RegisterUnitType( &Type, artG[ NAT_ID ] );
		CRestArmy_LV* pSQ = new CRestArmy_LV();
		pSQ->inTypeID = Type.Index ;
		pSQ->setTypeName( artG[ NAT_ID ] ) ; 
		pSQ->inAmount = ArtPusG ;
		pArmy.Add(pSQ);	

		ClearSelection(NI);
		SelectUnitsType(&Type, NI , 0 );
		SaveSelectedUnits( NI, &CannonGrp , 0 );
		for(int id=0; id < GetTotalAmount0( &CannonGrp ) ; id++) {
			GAMEOBJ  OneGrp;
			RegisterDynGroup(&OneGrp);
			InsertUnitToGroup( &CannonGrp , &OneGrp , id );
			pArmy[ pArmy.GetAmountOfElements()-1 ]->Kills.Add( GetKilled( &OneGrp ) );
		}
	}
	
	//
	RegisterUnitType( &Type, artR[ 0 ] );		
	ArtPusR  += GetTotalAmount1( &Type , NI );	

	if( ArtPusR > 0 ) {
		RegisterUnitType( &Type, artR[ 0 ] );
		CRestArmy_LV* pSQ = new CRestArmy_LV();
		pSQ->inTypeID = Type.Index ;
		pSQ->setTypeName( artR[ 0 ] ) ; 
		pSQ->inAmount = ArtPusR ;
		pArmy.Add(pSQ);	

		ClearSelection(NI);
		SelectUnitsType(&Type, NI , 0 );
		SaveSelectedUnits( NI, &CannonGrp , 0 );
		for(int id=0; id < GetTotalAmount0( &CannonGrp ) ; id++) {
			GAMEOBJ  OneGrp;
			RegisterDynGroup(&OneGrp);
			InsertUnitToGroup( &CannonGrp , &OneGrp , id );
			pArmy[ pArmy.GetAmountOfElements()-1 ]->Kills.Add( GetKilled( &OneGrp ) );
		}
	}
}
//-----------------------------------------------------------------------------------------

DLLEXPORT	void	SaveEndMissionData(CEndMissionData& EMD /*, bool*/ )
{
	const int FRAG_LIMIT = gi_bmMISS_FRAG_LIMIT ; 

	EndMissionData.operator =(EMD);
	//Заполним остатками армий.
	int  id = -1;	
	CRestArmy_LV* pSQ = NULL;
	//ATTACKER
	int  NI = EndMissionData.AttackerNI;
	EndMissionData.AttackerArmy.Clear();
	for(int i=0; i < CITY[NI].NBrigs ; i++) {
		if( CITY[NI].Brigs[i].Enabled ) {
			id = -1;
			for(int b=0; b < EndMissionData.AttackerArmy.GetAmountOfElements() && id == -1 ; b++) {
				if( CITY[NI].Brigs[i].MembID==EndMissionData.AttackerArmy[b]->inTypeID ) { id = b; }
			}
			if( id == -1 ) {
				pSQ = new CRestArmy_LV();
				pSQ->inTypeID = CITY[NI].Brigs[i].MembID ;
				pSQ->setTypeName( NATIONS[0].Mon[ pSQ->inTypeID ]->MonsterID ) ; //grey
				pSQ->inAmount = 1;
				EndMissionData.AttackerArmy.Add(pSQ);
				pSQ = NULL;
				id  = EndMissionData.AttackerArmy.GetAmountOfElements()-1 ;
			} else {
				EndMissionData.AttackerArmy[id]->inAmount += 1;
			}
			int kills = CITY[NI].Brigs[i].GetBrigExp();
			if( kills > FRAG_LIMIT ) kills = FRAG_LIMIT ;
			EndMissionData.AttackerArmy[id]->Kills.Add( kills ) ; 
		}	
	}

	//DEFENDER
	NI = EndMissionData.DefenderNI;
	EndMissionData.DefenderArmy.Clear();
	//if( MissionInitData.GetHeroPresent() ) {//new
	for(int i=0; i < CITY[NI].NBrigs ; i++) {
			if( CITY[NI].Brigs[i].Enabled ) {
				id = -1;
				for(int b=0; b < EndMissionData.DefenderArmy.GetAmountOfElements() && id == -1 ; b++) {
					if( CITY[NI].Brigs[i].MembID == EndMissionData.DefenderArmy[b]->inTypeID ) { id = b; }
				}
				if( id == -1 ) {
					pSQ = new CRestArmy_LV();

					pSQ->inTypeID = CITY[NI].Brigs[i].MembID;
					pSQ->setTypeName( NATIONS[0].Mon[ pSQ->inTypeID ]->MonsterID ) ; //grey
					pSQ->inAmount = 1;
					//pSQ->Kills.Add( CITY[NI].Brigs[i].NKills ) ;//grey
					EndMissionData.DefenderArmy.Add(pSQ);
					pSQ = NULL;
					id  = EndMissionData.DefenderArmy.GetAmountOfElements()-1 ; //id = 0 ;
				} else {
					EndMissionData.DefenderArmy[id]->inAmount += 1;
					//EndMissionData.DefenderArmy[id]->Kills.Add( CITY[NI].Brigs[i].NKills ) ;
				}
				//int kills = CITY[NI].Brigs[i].NKills ; 
				int kills = CITY[NI].Brigs[i].GetBrigExp() ; 
				if( kills > FRAG_LIMIT ) kills = FRAG_LIMIT ;
				EndMissionData.DefenderArmy[id]->Kills.Add( kills ) ;
			}
		}
	//}

	SaveCannonsForNation( EndMissionData.AttackerArmy , EndMissionData.AttackerNI );
	SaveCannonsForNation( EndMissionData.DefenderArmy , EndMissionData.DefenderNI );

	if( ROOT_SCENARIO.m_bSaveLogs )  EndMissionData.SaveXML();// -=== !!!NEEDED!!! ===-
}

//-----------------------------------------------------------------------------------------


//**********************************************************************************
//**********************************************************************************
//**********************************************************************************

//Битва между разными AI.
bool	CROOTDRIVER::SimulateBattle(CHero* pAttackHero, CHero* pDefendHero, CSectStatData* pSector)//Победил ли атакующий.
{
	if( !pSector )				{ return false; }
	if( !pAttackHero )			{ return false; }	

	// check for valid defender		//very new!!!
	//if( pDefendHero ) {
	//	int  DEF_NI  = pDefendHero->m_inPlayerID;
	//	int  OwnerId = m_pSect->GetSecOwner( pSector->m_inID );
	//	if(  OwnerId != DEF_NI && 
	//		!ROOT_SCENARIO.m_pDipl->CheckContract( m_pDipl->m_pDiplData->GetAllyId(), OwnerId, DEF_NI ) && 
	//		 DEF_NI >= 0 && DEF_NI < MAX_PLAYERS ) {
	//		ROOT_SCENARIO.m_pPlayer[ DEF_NI ]->TryRetreat( pDefendHero , false );
	//		pDefendHero = NULL;
	//	}
	//} //very new!!!

	pAttackHero->AddBattle();
	if(pDefendHero) pDefendHero->AddBattle();

	int  diff		= pSAVE->m_iDifficulty ,  ex0 = 4*QuestDisbSquadExp + QuestCompliteExp ,  ex2 = DisbandSQExp ;
	int  AttShots	= 0,		 DefShots	= 0;			// ~кол-во залпов отряда (время проведенное в бою)
	int  AttackExpa = ex0*diff,	 DefendExpa = ex2*diff;		// для подсчета экспы
	int  ATT_NI		= pAttackHero->GetPlayerID();
	int  DEF_NI		= m_pSect->GetSecOwner( pSector->m_inID ); 
	if(pDefendHero)  DEF_NI = pDefendHero->GetPlayerID();

	// параметры защиты у сектора
	int  SecDef     = pSector->GetSecDefence();													
	int  SEC_DEF_SQ = GetSQNumByDefenceID( SecDef , 0 , 0 , pSector->GetSectID() ); // отряды самообороны в секторе	
	if( rand()%15 <= SecDef )  SecDef+=2 ; // CRAZY RULE // moral++

	// Посчитаем вес вероятности победы для противоборствующих сторон.
	int  AW  = pAttackHero->GetARMY_Power();				// вес при схватке для атакующего.
	int  DW  = SEC_DEF_SQ * SEC_DEF_POWER( SecDef ) ;		// вес при схватке для обороняющегося.
	if(pDefendHero)  DW += pDefendHero->GetARMY_Power();

	if( AW >= DW && pAttackHero->GetRank() < 2 && SecDef > 1 ) {
		AW = DW/2 ;	} // dont kill 2 def.level if not a MAJOR
	if( AW >= DW && pAttackHero->GetRank() < 4 && SecDef >= MAX_DEFENCE && rand()%10 > 0 ) { //2*DW ) {
		AW = DW/2 ;	} // dont kill 3 def.level if not a GENERAL
	
	// CHECK Food&Coal
	int  chkr = 1151;
	if( m_pPlayer[ ATT_NI ]->GetResource(FOOD) < chkr )  AW = ( AW - AW/3 );
	if( m_pPlayer[ ATT_NI ]->GetResource(COAL) < chkr )  AW = ( AW - AW/3 );
	if( DEF_NI >= 0 && DEF_NI < MAX_PLAYERS ) {
	if( m_pPlayer[ DEF_NI ]->GetResource(FOOD) < chkr )  DW = ( DW - DW/3 );
	if( m_pPlayer[ DEF_NI ]->GetResource(COAL) < chkr )  DW = ( DW - DW/3 ); 
	}

	// НАЧИНАЕМ БОЙ ДО СМЕРТИ.
	int    AHRank	  = pAttackHero->GetRank();
	bool   attRetreat = false ;
	while( pAttackHero->GetARMY_NSQ() > 0 && SEC_DEF_SQ > 0 && !attRetreat ) {
		if( (rand()%(AW+DW)) < AW ) {	// грохнули отряд защитника.
			AttShots++;		SEC_DEF_SQ--;						AttackExpa += DisbandSQExp ; //+экспа: disband	
			pAttackHero->AddFormKilled();		
		}else{
			DefShots++;		if(pAttackHero->KillOneSQUARD()) {	DefendExpa += DisbandSQExp ;	
			if( 4*pAttackHero->GetARMY_NSQ()/3 < SEC_DEF_SQ && (rand()%100 < 35+10*AHRank) )	//if( ( pAttackHero->GetARMY_NSQ() <= SEC_DEF_SQ/2 -4 +AHRank) && (rand()%100 < 40+10*AHRank) ) 
				attRetreat = true;
			if(  pAttackHero->GetARMY_NSQ() == pAttackHero->GetBranch_NSQ( 2 ) )  
				attRetreat = true; 
			if( pDefendHero )  pDefendHero->AddFormKilled();   }
		}
	}
	SecDef			= pSector->GetSecDefence();
	int RES_SEC_DEF = GetSQNumByDefenceID( SecDef , 0 , 0 , pSector->GetSectID() )/( SecDef + 1 ) ;
	RES_SEC_DEF = SEC_DEF_SQ / ( (RES_SEC_DEF!=0)?RES_SEC_DEF:1 ) ; 
	if(  pSector->m_inDefence    > RES_SEC_DEF )           { pSector->m_inDefence = RES_SEC_DEF; } // сбили защиту
	if(  pSector->m_inPopulation > RES_SEC_DEF &&
		(pSector->m_inPopulation > 1 || rand()%10 > 5 ) )  { pSector->m_inPopulation-- ; } // уменьшилось население
	//new
	int  showSecID = pAttackHero->getPreviousSecID();
	if( showSecID == pSector->GetSectID() || showSecID < 0 )  showSecID = pSector->GetSectID();
	//ShowInGameSector( pAttackHero->getSectorID() , 0.5 );
	ShowInGameSector( showSecID , BM_SHOW_SECPAUSE/2 );

	if( pDefendHero ) {
		int    DHRank = pDefendHero->GetRank();
		bool   defRetreat = false ;
		while( pAttackHero->GetARMY_NSQ() > 0 && pDefendHero->GetARMY_NSQ() > 0 && !attRetreat && !defRetreat ) {
			if( (rand()%(AW+DW)) < AW ) {	// грохнули отряд защитника.
				AttShots++;		if(pDefendHero->KillOneSQUARD()) {	AttackExpa += DisbandSQExp ;
				if( 4*pDefendHero->GetARMY_NSQ()/3 <= pAttackHero->GetARMY_NSQ() && (rand()%100 < 35+10*DHRank) ) 
					defRetreat = true; 
				if( pDefendHero->GetARMY_NSQ() == pDefendHero->GetBranch_NSQ( 2 ) )  
					defRetreat = true; 
				pAttackHero->AddFormKilled();   }
			}else{
				DefShots++;		if(pAttackHero->KillOneSQUARD()) {	DefendExpa += DisbandSQExp ;
				if( 4*pAttackHero->GetARMY_NSQ()/3 <= pDefendHero->GetARMY_NSQ() && (rand()%100 < 35+10*AHRank) ) 
					attRetreat = true;
				if(pAttackHero->GetARMY_NSQ() == pAttackHero->GetBranch_NSQ( 2 ) )  
					attRetreat = true; 
				pDefendHero->AddFormKilled();   }
			}
		}
		//new
		//ShowInGameSector( pAttackHero->getSectorID() , 0.5 );
		ShowInGameSector( showSecID , BM_SHOW_SECPAUSE/2 );
	}
	if(attRetreat)	{
		int stopp = 1;
	}

	// бой окончен!
	// take battle cost
	AttShots += rand()%( AttShots<2 ? 2 : AttShots/2 );		DefShots += rand()%( DefShots<2 ? 2 : DefShots/2 );		
	int		BatTime	= ( AttShots + DefShots ) / 2 +1 ;		// ~ battle time
	int		mFood	= ( 500 + rand()%500 ) * BatTime;		// ~ food out
	int		mCoal	= ( 350 + rand()%500  );				// ~ coal out
	int		add0	= 1000 * diff;							// addings from "virtual" mission
	if( ATT_NI == 4 )  AttShots = AttShots/2;
	if( ATT_NI == 5 )  AttShots = AttShots/4;
	m_pPlayer[ ATT_NI ]->AddResources(0, - mFood + add0 ,0, add0 , add0 , - AttShots * mCoal + add0 );
	if( DEF_NI == 4 )  DefShots = DefShots/2;
	if( DEF_NI == 5 )  DefShots = DefShots/4;
	if( DEF_NI >=0 && DEF_NI < MAX_PLAYERS )
		m_pPlayer[ DEF_NI ]->AddResources(0, - mFood/2 ,0,0,0, - ( DefShots * mCoal )/2 );

	// Get battle experience 
	int chance = ( rand()%10 )/5 ; // quest completed num  -  0-1
	AttackExpa += QuestDisbSquadExp * ( 2*chance + 2*diff + 1 ) ;	// + экспа за квест.отр. - 0+0+1=1 -> 2+4+1=7 (+8)
	AttackExpa += QuestCompliteExp  * ( chance   + diff/2 ) ;		// + экспа за квесты	 - 0+0  =0 -> 1+1=2	  (+2)	
	//AttackExpa += 34 * chance ;									// + экспа за села

	pAttackHero->AddExperience( AttackExpa );						// экспа за бой
	if(pDefendHero) pDefendHero->AddExperience( DefendExpa );		// экспа за бой

	pAttackHero->SetBattleNKills();									// add nkills
	if(pDefendHero) pDefendHero->SetBattleNKills();					// add nkills
	

	if( pAttackHero->GetARMY_NSQ() == 0 || attRetreat ) {
		if(pDefendHero) pDefendHero->AddVictory();
		if(pDefendHero) pDefendHero->AddExperience(HeroDefExp + SecCaptExp); //экспа за поражение героя и защиту сектора
		return false;	// атакер проиграл	
	}

	pAttackHero->AddVictory();
	pAttackHero->AddExperience( SecCaptExp );						// экспа за захват сектора
	if(pDefendHero) pAttackHero->AddExperience( HeroDefExp );		// экспа за поражение героя

	return true;		// атакер победил
}

//**********************************************************************************
////////////////////////////////////////
//////////////// CError ////////////////
////////////////////////////////////////
//----------------------------------------------
CError::CError(char* pchError){
	if (pchError!=NULL){
		strcpy(m_chErrorStatus,pchError);
	}
	else{
		ResetError();
	}
}
//----------------------------------------------
CError::CError(){
	ResetError();
}
//----------------------------------------------
void	CError::ResetError(){
	strcpy(m_chErrorStatus,"NO_ERROR");
}
//----------------------------------------------
void	CError::AddErrorMessage(char* pchError){
	char	a_strTemp[256];
	if (pchError!=NULL){
		strcpy(a_strTemp,m_chErrorStatus);
		sprintf(m_chErrorStatus,"%s%s%s",a_strTemp,pchError,"\n");
	}
}
//----------------------------------------------
bool	CError::IsError(){
	if ((m_chErrorStatus!=NULL) && (strcmp(m_chErrorStatus,"NO_ERROR")!=0)){
		return true;
	}
	return false;
}	
//----------------------------------------------
void	CError::SendError(char* pchError){
	if (pchError!=NULL){
		strcpy(m_chErrorStatus,pchError);
	}
}
//----------------------------------------------
void	CError::ShowError(char* pchError){
	if (IsError()){
		strcpy(m_chErrorStatus,pchError);
		VityaError(m_chErrorStatus);
	}
}
//----------------------------------------------
void	CError::ShowError(){
	if (IsError()){
		VityaError(m_chErrorStatus);
	}
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
///////////// CSectStatData ////////////
////////////////////////////////////////
//----------------------------------------------
CSectStatData::CSectStatData() 
{
	Init();
}
//----------------------------------------------
CSectStatData::CSectStatData(DString* pDS) 
{
	Init();
	GetData(pDS);
}
//----------------------------------------------
CSectStatData::~CSectStatData() 
{
	Init();
}
//----------------------------------------------
void	CSectStatData::Init() 
{
	//new
	m_pinNIDS.Clear();
	m_pinArrowD.Clear();	//m_pinNIDS		= NULL;	//m_pinArrowD	= NULL;
	//new

	x0		= y0	  = 0;
	m_curX  = m_curY  = 0;
	m_SityX = m_SityY = 0;
	m_FortX = m_FortY = 0;

	m_inID			= -1;
	m_inDeffOwner	= -1;
	m_inMapOwner	= -1;
	m_inNON			= 0;
	m_inNV			= 0;
	m_inSRID		= 0;
	m_inPopulation	= 0;
	m_inDefence		= 0;
	m_iRecruits		= 0;
	m_iSabotageID	= 0;
	
	m_psdSec		= NULL;
	m_psdSity		= NULL;
	m_psdFort		= NULL;

	m_chSectorName.DString::Clear();//new
	m_chSectorName = "NONAME";

	m_chRes.DString::Clear();//new
	m_chRes = GetTextByID( "StratRes0" );
}
//----------------------------------------------
void	CSectStatData::SetSecAddress(SimpleDialog* pSD){
	if (pSD!=NULL)	m_psdSec = pSD;
}
//----------------------------------------------
void	CSectStatData::SetSityAddress(SimpleDialog* pSD){
	if (pSD!=NULL)	m_psdSity = pSD;
}
//----------------------------------------------
void	CSectStatData::SetFortAddress(SimpleDialog* pSD){
	if (pSD!=NULL)	m_psdFort = pSD;
}
//----------------------------------------------
void	CSectStatData::SetVissible(bool flag){
	m_psdSec->Visible = flag;
}
//----------------------------------------------
void	CSectStatData::SetSectColor(int PID)//grey
{
	if( PID == 0xFFFF ) {
		m_psdSec->Diffuse = 0x00FFFFFF;
	} else {
		int id = 6;
		if( PID >= 0 && PID < MAX_PLAYERS )  id = PID;
		m_psdSec->Diffuse = ALPHA_SECT + GetBigMapPlayerColor( id );
	}
	#ifdef DemoAI	//DEMO
		if( m_inID != 1 && m_inID != 2 && m_inID != 5 && m_inID != 6 ) { 
			m_psdSec->Diffuse = 0x30000000 + GetBigMapPlayerColor( PID ) ; // 0x88000000
		}
	#endif			//DEMO
}
//void	CSectStatData::SetSectColor(int PID)//grey
//{
//	if (PID==-1) {
//		m_psdSec->Diffuse = 0x00FFFFFF;
//	}
//	else {
//		m_psdSec->Diffuse = ALPHA_SECT + GetBigMapPlayerColor( PID );
//		#ifdef DemoAI	//DEMO
//			if( m_inID != 1 && m_inID != 2 && m_inID != 5 && m_inID != 6 ) { 
//				m_psdSec->Diffuse = 0x30000000 + GetBigMapPlayerColor( PID ) ; // 0x88000000
//			}
//		#endif			//DEMO
//	}		
//}
//void	CSectStatData::SetSectColor(int PID)
//{
//	if (PID==-1){
//		m_psdSec->Diffuse = 0x00FFFFFF;
//	}
//	else{
//		m_psdSec->Diffuse = ALPHA_SECT + GetBigMapPlayerColor(PID);
//	}
//}
//----------------------------------------------
void	CSectStatData::SetColor(int color){
	m_psdSec->Diffuse = m_psdSec->Diffuse & 0xFF000000;
	m_psdSec->Diffuse += color;
}
//----------------------------------------------
void	CSectStatData::SetSectAColor(int color) {
	m_psdSec->Diffuse = color;
}
//----------------------------------------------
void	CSectStatData::SetSectAlpha(int a){
	m_psdSec->Diffuse = m_psdSec->Diffuse & 0x00FFFFFF;
	m_psdSec->Diffuse += a;
}
//----------------------------------------------
void	CSectStatData::SetDeffColor()
{
	SetSectColor(m_inDeffOwner);
	GPPicture* a_pgppSity = reinterpret_cast <GPPicture*> (m_psdSity);
	GPPicture* a_pgppFort = reinterpret_cast <GPPicture*> (m_psdFort);
	
	int CID = 6;
	if( m_inDeffOwner >= 0 && m_inDeffOwner < MAX_PLAYERS )  CID = m_inDeffOwner;
	a_pgppSity->Nation = a_pgppFort->Nation = ROOT_SCENARIO.m_iNationID[ CID ] ;

	//if( m_inDeffOwner >= 0 && m_inDeffOwner < 2*MAX_PLAYERS ) {
	//	a_pgppSity->Nation = ROOT_SCENARIO.m_iNationID[m_inDeffOwner] ;
	//	//a_pgppSity->Nation = m_inDeffOwner ;
	//}
	//else {
	//	a_pgppSity->Nation = ROOT_SCENARIO.m_iNationID[6] ;
	//	//a_pgppSity->Nation = 6 ;
	//}
}
//---------------------------------------------- 
void	CSectStatData::SetSityType() {
	if( m_psdSity != NULL ) {
		GPPicture* a_pgppSity = reinterpret_cast <GPPicture*> (m_psdSity);
		a_pgppSity->SpriteID  = a_pgppSity->ShapeSpriteID = m_inPopulation;

		////if (a_pgppSity->SpriteID!=(m_inPopulation + (MAX_DEFENCE * m_inDefence)) || a_pgppSity->ShapeSpriteID!=(m_inPopulation + (MAX_DEFENCE * m_inDefence))) {
		//a_pgppSity->SpriteID = a_pgppSity->ShapeSpriteID =  m_inPopulation + (MAX_DEFENCE * m_inDefence);
		////a_pgppSity->ShapeSpriteID	= m_inPopulation + (MAX_DEFENCE * m_inDefence);
		////}
	}
	if( m_psdFort != NULL ) {
		GPPicture* a_pgppFort = reinterpret_cast <GPPicture*> (m_psdFort);
		a_pgppFort->SpriteID  = a_pgppFort->ShapeSpriteID = m_inDefence;

		////if (a_pgppFort->SpriteID!=(m_inPopulation + (MAX_DEFENCE * m_inDefence)) || a_pgppFort->ShapeSpriteID!=(m_inPopulation + (MAX_DEFENCE * m_inDefence))) {
		//a_pgppFort->SpriteID = a_pgppFort->ShapeSpriteID =  m_inPopulation + (MAX_DEFENCE * m_inDefence);
		////a_pgppFort->ShapeSpriteID	= m_inPopulation + (MAX_DEFENCE * m_inDefence);
		////}
	}}
//---------------------------------------------- 
bool	CSectStatData::GetData(DString* pDS){
	char		s[256];

	DString		a_dsWR;
	pDS->ExtractWord(a_dsWR);
	if (a_dsWR=="#SECTOR#"){
		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="ID"){
			pDS->ExtractWord(a_dsWR);
			m_inID = atoi(a_dsWR.pchar());
		}
		else{
			sprintf(s,"%s,%d","<ID> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="Center"){
			pDS->ExtractWord(a_dsWR);
			x0 = atoi(a_dsWR.pchar());
			pDS->ExtractWord(a_dsWR);
			y0 = atoi(a_dsWR.pchar());
		}
		else{
			sprintf(s,"%s,%d","<Center> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="SityXY"){
			pDS->ExtractWord(a_dsWR);
			m_SityX = atoi(a_dsWR.pchar());
			pDS->ExtractWord(a_dsWR);
			m_SityY = atoi(a_dsWR.pchar());
		}
		else{
			sprintf(s,"%s,%d","<SityXY> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		//new
		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="FortXY"){
			pDS->ExtractWord(a_dsWR);
			m_FortX = atoi(a_dsWR.pchar());
			pDS->ExtractWord(a_dsWR);
			m_FortY = atoi(a_dsWR.pchar());
		}
		else{
			sprintf(s,"%s,%d","<FortXY> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}
		//new //last add

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="DefOwner") {
			pDS->ExtractWord(a_dsWR);
			m_inMapOwner = atoi(a_dsWR.pchar());
		}
		else{
			sprintf(s,"%s,%d","<DefOwner> isn't correct, load init data, SECTOR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="Defence"){
			pDS->ExtractWord(a_dsWR);
			m_inDefence = atoi(a_dsWR.pchar());
		}
		else{
			sprintf(s,"%s,%d","<Defence> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="Population"){
			pDS->ExtractWord(a_dsWR);
			m_inPopulation = atoi(a_dsWR.pchar());
			m_iRecruits = (m_inPopulation + 1)* SECT_MAX_RECRTS;
		}
		else{
			sprintf(s,"%s,%d","<Population> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="SectorName"){
			pDS->ExtractWord(a_dsWR);
			m_chSectorName = GetTextByID( a_dsWR.pchar() );
		}
		else{
			sprintf(s,"%s,%d","<SectorName> isn't correct, load init data, SECTOR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="Neighbors"){
			pDS->ExtractWord(a_dsWR);
			m_inNON = atoi(a_dsWR.pchar());
		//	m_pinNIDS = new int[m_inNON];
			for (int i=0; i<m_inNON; i++){
				pDS->ExtractWord(a_dsWR);
		//		m_pinNIDS[i] = atoi(a_dsWR.pchar());
				m_pinNIDS.Add( atoi(a_dsWR.pchar()) );
			}
		}
		else{
			sprintf(s,"%s,%d","<Neighbors> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="Arrow"){
		//	m_pinArrowD = new int[m_inNON*3];
			for (int i=0; i<m_inNON*3; i++){
				pDS->ExtractWord(a_dsWR);
		//		m_pinArrowD[i] = atoi(a_dsWR.pchar());
				m_pinArrowD.Add( atoi(a_dsWR.pchar()) );
			}
		}
		else{
			sprintf(s,"%s,%d","<Arrow> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}
		
		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="Villages"){
			pDS->ExtractWord(a_dsWR);
			m_inNV = atoi(a_dsWR.pchar());
		}
		else{
			sprintf(s,"%s,%d","<Villages> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="Ressource"){
			pDS->ExtractWord(a_dsWR);
			m_inSRID = atoi(a_dsWR.pchar());
			if (m_inSRID!=0){
				char	buff[64];
				sprintf(buff,"%s%d","StratRes",m_inSRID);
				m_chRes = GetTextByID( buff );
			}
		}
		else{
			sprintf(s,"%s,%d","<Ressource> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		pDS->ExtractWord(a_dsWR);
		if (a_dsWR=="Owner"){
			pDS->ExtractWord(a_dsWR);
			m_inDeffOwner = atoi(a_dsWR.pchar());
		}
		else{
			sprintf(s,"%s,%d","<Owner> isn't correct, load init data, SECTROR ",m_inID);
			VityaError(s);
		}

		m_curX = x0;
		m_curY = y0;
	}
	else{
		VityaError("<#SECTOR#> isn't correct, load init data");
	}

	return false;
}
//----------------------------------------------
bool	CSectStatData::ConvertCenters(int wnew,int hnew,int wold,int hold){
	if (wold<=0 || hold<=0)
		return false;

	if (wold!=wnew){
		m_SityX = ((double)m_SityX/(double)wold)*(double)wnew;
		m_FortX = ((double)m_FortX/(double)wold)*(double)wnew;
		x0		= ((double)x0/(double)wold)*(double)wnew;
		for (int i=0; i<m_inNON; i++){
			m_pinArrowD[i*3] = ((double)m_pinArrowD[i*3]/(double)wold)*(double)wnew;;
		}
	}
	if (hold!=hnew){
		m_SityY = ((double)m_SityY/(double)wold)*(double)wnew;
		m_FortY = ((double)m_FortY/(double)wold)*(double)wnew;
		y0		= ((double)y0/(double)hold)*(double)hnew;
		for (int i=0; i<m_inNON; i++){
			m_pinArrowD[i*3+1] = ((double)m_pinArrowD[i*3+1]/(double)wold)*(double)wnew;;
		}
	}

	m_curX = x0;
	m_curY = y0;

	return true;
}
//----------------------------------------------
void	CSectStatData::DeposeTo(int x,int y){
	m_curX = x0 + x;
	m_curY = y0 + y;
	if (m_psdSec!=NULL){
		m_psdSec->x  = m_curX;
		m_psdSec->y  = m_curY;
		m_psdSec->x1 = m_curX+GPS.GetGPWidth(m_psdSec->ShapeFileID,m_psdSec->ShapeSpriteID);
		m_psdSec->y1 = m_curY+GPS.GetGPHeight(m_psdSec->ShapeFileID,m_psdSec->ShapeSpriteID);
	}
	m_curX = m_SityX + x;
	m_curY = m_SityY + y;
	if (m_psdSity!=NULL){
		int W = GPS.GetGPWidth(m_psdSity->ShapeFileID,m_psdSity->ShapeSpriteID);
		int H = GPS.GetGPHeight(m_psdSity->ShapeFileID,m_psdSity->ShapeSpriteID);
		m_psdSity->x  = m_curX - (W / 2);
		m_psdSity->y  = m_curY - (H / 2);
		m_psdSity->x1 = m_curX + (W / 2);
		m_psdSity->y1 = m_curY + (H / 2);
	}	
	
	//new
	m_curX = m_FortX + x;
	m_curY = m_FortY + y;
	if (m_psdFort!=NULL){
		int W = GPS.GetGPWidth(m_psdFort->ShapeFileID,m_psdFort->ShapeSpriteID);
		int H = GPS.GetGPHeight(m_psdFort->ShapeFileID,m_psdFort->ShapeSpriteID);
		m_psdFort->x  = m_curX - (W / 2);
		m_psdFort->y  = m_curY - (H / 2);
		m_psdFort->x1 = m_curX + (W / 2);
		m_psdFort->y1 = m_curY + (H / 2);
	}
}
//----------------------------------------------
void	CSectStatData::VissibleIfMouse(){
	if (m_psdSec->MouseOver){
		m_psdSec->Visible = true;
	}
	else{
		m_psdSec->Visible = false;
	}
}
//----------------------------------------------
char*	CSectStatData::GetSectorName(){ //grey
	return	m_chSectorName.pchar();
}
//----------------------------------------------
int		CSectStatData::GetSecDefence(){ //grey
	return	m_inDefence;
}
//----------------------------------------------
int		CSectStatData::GetSecPopulation(){ //grey
	return	m_inPopulation;
}
//----------------------------------------------
char*	CSectStatData::GetResourceName(){
	return  m_chRes.pchar();
}
//----------------------------------------------
int		CSectStatData::GetSectID(){
	return m_inID;
}
//----------------------------------------------
int		CSectStatData::GetVillageNUM(){
	return	m_inNV;
}
//----------------------------------------------
int		CSectStatData::GetResourceID(){
	return	m_inSRID;
}
//----------------------------------------------
bool	CSectStatData::GetMouseOver(){
	if (m_psdSec==NULL) 	return	false;
	return	m_psdSec->MouseOver;
}
//----------------------------------------------
int		CSectStatData::GetNNeighbor(){
	return	m_inNON;
}
//----------------------------------------------
int		CSectStatData::GetNeighborID(int idx){
	return	m_pinNIDS[idx];
}
//----------------------------------------------
int		CSectStatData::GetDeffOwner(){
	return	m_inDeffOwner;
}
//----------------------------------------------
void	CSectStatData::GetSityXY(int &x,int &y){
	x = m_FortX; //m_SityX
	y = m_FortY; //m_SityY
}
//----------------------------------------------
void	CSectStatData::GetCurXY(int &x,int &y){
	x = m_curX;
	y = m_curY;
}
//----------------------------------------------
bool	CSectStatData::GetEnable(){
	return	m_psdSec->Enabled;
}
//----------------------------------------------
void	CSectStatData::GetAroowCoord(int& x,int& y,int& a,int n){
	x = m_pinArrowD[n*3+0];
	y = m_pinArrowD[n*3+1];
	a = m_pinArrowD[n*3+2];
}
//----------------------------------------------
SimpleDialog*	CSectStatData::GetSityPic() {
	//new	//last add!!!
	return	m_psdFort;	//return	m_psdSity;
	//new
}
//----------------------------------------------
void	CSectStatData::SetVisible(bool state){
	SetSectVisible(state);
	SetSityVisible(state);
}
//----------------------------------------------
void	CSectStatData::SetDeffOwner(int PID){
	if (-1<=PID&&PID<MAX_PLAYERS) {
		m_inDeffOwner=PID;
	}
}
//----------------------------------------------
void	CSectStatData::SetEnable(bool state) {
	m_psdSec->Enabled	= state;
	m_psdSity->Enabled	= state;
	m_psdFort->Enabled	= state;
}
//----------------------------------------------
void	CSectStatData::SetSectVisible(bool state) {
	m_psdSec->Visible  = state;
}
//----------------------------------------------
void	CSectStatData::SetSityVisible(bool state) {
	m_psdSity->Visible = state;
	m_psdFort->Visible = state;
}
//----------------------------------------------

void	CSectStatData::CreateHint2()
{
	_str	a_Hint;
	a_Hint = "";
	DYNHINT_RA( m_psdSec , a_Hint.pchar() );
}

//----------------------------------------------
void	CSectStatData::CreateHint()
{
	//
	if(!this->m_psdSec->Enabled)  return;
	//

	_str	a_Hint;
	a_Hint = "{FM}{CR}";	
	if( m_chSectorName.pchar() )  a_Hint += m_chSectorName;//very new!!! 8/12/04
	a_Hint += "{FS}{C FF502515}" ;
	a_Hint += " \\ " ; //	a_Hint += " \\  \\ " ;
	if( m_chRes != GetTextByID( "StratRes0" ) ) {
		a_Hint += GetTextByID( "#MainRes" );
		a_Hint += "{C FF640100}  " ;
		a_Hint += m_chRes;
		a_Hint += " \\ " ;
	}

	Enumerator*	pDefLvl_ENUM = ENUM.Get( "DefLvl_ENUM" );

	a_Hint += "{C FF502515}" ;						
	a_Hint += GetTextByID( "#Defence#" );
	a_Hint += "{C FF640100}  " ;
	a_Hint += pDefLvl_ENUM->Get( m_inDefence );
	a_Hint += "{C} ";
	char  text[256];	
	sprintf(text, GetTextByID("#CHINT_DefenceSQN"), GetSQNumByDefenceID(m_inDefence,m_inDeffOwner,m_inPopulation, m_inID ) );
	a_Hint += text;

	a_Hint += "{C FF502515} \\ " ; //a_Hint += "{C FF502515} \\  \\ " ;
	a_Hint += GetTextByID( "#CWM_Left_Click" );
	a_Hint += " \\ " ;
	a_Hint += GetTextByID( "#CWM_Right_Click_OnMap" );

	DYNHINT_RA( m_psdSec , a_Hint.pchar() );
	//DYNHINT_RA( m_psdSity , a_Hint.pchar() );
}
//void	CSectStatData::CreateHint()
////void	CSectStatData::CreateHint( char* owner ) //grey
//{
//	_str	a_Hint;
//	//a_Hint += "{FS}{CR}" ;	
//	a_Hint += "{FM}{CR}";
//	a_Hint += m_chSectorName;
//	a_Hint += "{FS}{C FF502515}" ;
//	if( m_chRes != GetTextByID( "StratRes0" ) ) {
//		//a_Hint += owner ;						//a_Hint += " \\ ... \\ " ; 
//		//a_Hint += m_chSectorName;				//a_Hint += " \\ ... \\ " ; a_Hint += GetTextByID( "#Vill" ); a_Hint += " - "; a_Hint += m_inNV;
//		a_Hint += " \\  \\ " ;
//		a_Hint += GetTextByID( "#MainRes" );
//		a_Hint += "  {CG}";
//		a_Hint += m_chRes;
//	}
//
//	a_Hint += "{C FF502515} \\  \\ " ;
//	a_Hint += GetTextByID( "#CWM_Left_Click" );
//	a_Hint += " \\ " ;
//	a_Hint += GetTextByID( "#CWM_Right_Click_OnMap" );
//
//	DYNHINT_RA( m_psdSec , a_Hint.pchar() );
//	
//	Enumerator*	pDefLvl_ENUM = ENUM.Get( "DefLvl_ENUM" );
//
//	a_Hint.DString::Clear();
//	//a_Hint += "{FS}{CR}" ;			
//	a_Hint += "{FM}{CR}";			
//	a_Hint += m_chSectorName;
//	a_Hint += "{FS}{C FF502515}" ;
//	a_Hint += " \\  \\ " ;
//	a_Hint += GetTextByID( "#Population#" );
//	a_Hint += " " ;
//	a_Hint += (m_inPopulation + 1) * 100 ; // * 10000 ;
//	a_Hint += " 000" ;
//	a_Hint += " \\ " ;
//	a_Hint += GetTextByID( "#Defence#" );
//	a_Hint += " {CG}" ;
//	a_Hint += pDefLvl_ENUM->Get( m_inDefence );
//
//	a_Hint += "{C FF502515} \\  \\ " ;
//	a_Hint += GetTextByID( "#CWM_Left_Click" );//!!!very new!!!
//	a_Hint += " \\ " ;
//	a_Hint += GetTextByID( "#CWM_Right_Click_OnMap" );//!!!very new!!!
//
//	DYNHINT_RA( m_psdSity , a_Hint.pchar() );
//}

//----------------------------------------------
void	CSectStatData::SetNewArrowOffset(int offset) {
	m_inArrowOffset = offset;
}
//----------------------------------------------
int		CSectStatData::GetNewArrowOffset() {
	return m_inArrowOffset;
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
///////////// CSectDynmData ////////////
////////////////////////////////////////
//----------------------------------------------
CSectDynmData::CSectDynmData()
{
	m_inOwner = -1;
}
//----------------------------------------------
CSectDynmData::~CSectDynmData()
{
	m_inOwner = -1;
}
//----------------------------------------------
void	CSectDynmData::SetOwner(int PID)
{
	m_inOwner = PID;
}
//----------------------------------------------
int		CSectDynmData::GetOwner(){
	return	m_inOwner;
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
/////////////// CSectData //////////////
////////////////////////////////////////
//----------------------------------------------
CSectData::CSectData(){
	Init();
}
//----------------------------------------------
CSectData::CSectData(char* DataFileName){
	if (DataFileName==NULL){
		VityaError("Cant load init data from file. Dont have file name.");
		return; 
	}

	Init();

	DString		a_DS;
	a_DS.ReadFromFile(DataFileName);

	if(!a_DS.pchar()) return;//VERY NEW //FOR FUNTASIA
	
	DString		a_DS2;
	a_DS.ExtractWord(a_DS2);
	if (a_DS2=="#SECTORN#"){
		a_DS.ExtractWord(a_DS2);
		m_inNOS = atoi(a_DS2.pchar());	
	}
	else{
		VityaError("<#SECTORN#> isn't correct, load init data");
		return; 
	}

	a_DS.ExtractWord(a_DS2);
	if (a_DS2=="#MAPSIZE#"){
		a_DS.ExtractWord(a_DS2);
		m_fWM = atoi(a_DS2.pchar());
		a_DS.ExtractWord(a_DS2);
		m_fHM = atoi(a_DS2.pchar());
	}
	else{
		VityaError("<#MAPSIZE#> isn't correct, load init data");
		return; 
	}

	if (m_inNOS>0){
	//	m_pssdDT = new CSectStatData[m_inNOS];
	//	m_psddDT = new CSectDynmData[m_inNOS];
		CSectStatData*	pSD = NULL;
		CSectDynmData*	pDD = NULL;
		for (int i=0; i<m_inNOS; i++){
			pSD = new CSectStatData();
			pDD = new CSectDynmData();
			m_pssdDT.Add(pSD);
			m_psddDT.Add(pDD);
			pSD = NULL;
			pDD = NULL;
		}
	}
	
	for (int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->GetData(&a_DS);
		m_psddDT[i]->SetOwner(m_pssdDT[i]->GetDeffOwner());
//		m_psddDT[i]->GetData(.....);
	}
}
//----------------------------------------------
CSectData::~CSectData()
{
	Init();
}
//----------------------------------------------
void	CSectData::Init()
{
	m_pssdDT.Clear();
	m_psddDT.Clear();
	m_inNOS			= 0;

	m_fWM			= 0;
	m_fHM			= 0;
	m_pdsMenu		= NULL;
	m_inSecOffset	= 0;
	m_inSityOffset	= 0;
	for(int i=0; i < MAX_ARROW ; i++) {
		m_pgppArows[i] = NULL;
	}
	for(int i=0; i < MAX_ARROW * MAX_ARROW ; i++) {
		m_pgppNewArrows[i]		= NULL;
		m_pgppNewArrowsShad[i]	= NULL;
	}
}
//----------------------------------------------
void	CSectData::UPDATE(){
	for (int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->SetSityType();
		if (m_psddDT[i]->GetOwner()!=m_pssdDT[i]->GetDeffOwner()){
			m_pssdDT[i]->SetDeffOwner(m_psddDT[i]->GetOwner());
			m_pssdDT[i]->SetDeffColor();
		}
	}
}
//----------------------------------------------
void	CSectData::CreateTempDataFiles(){
	FILE*	file = NULL;
	char	FileName[256];
	
	if (!CreateDirectoryInCur("temp")){
		VityaError("Can't create subdir for temp data.");
		return;
	}

	for (int i=0; i<m_inNOS; i++){
		sprintf(FileName,"%s%d%s","temp\\Sect",i,".txt");
		if ((file=fopen(FileName,"w"))!=NULL){
			fprintf(file,"%s%s",m_pssdDT[i]->GetSectorName(),"\n");
			fprintf(file,"%s%s%d%s",GetTextByID("#Vill")," - ",m_pssdDT[i]->GetVillageNUM(),"\n");
			if (m_pssdDT[i]->GetResourceID()!=0){
				fprintf(file,"%s%s",m_pssdDT[i]->GetResourceName(),"\n");
			}
			fclose(file);
		}
		else{
			VityaError("Can't open file for temp data.");
		}
	}
}
//----------------------------------------------
void	CSectData::AddArrowPictures(LocalGP* pPic,int sprID){
	for (int i=0; i < MAX_ARROW ; i++) {
		m_pgppArows[i] = m_pdsMenu->addGPPicture(NULL,0,0,pPic->GPID,sprID);
		
		m_pgppArows[i]->ShapeFileID	= pPic->GPID;
		m_pgppArows[i]->ShapeSpriteID	= sprID;
		m_pgppArows[i]->Diffuse		= 0xffFFFFFF;
		m_pgppArows[i]->Visible		= false;
	}
}
//----------------------------------------------
void	CSectData::AddNewArrowPictures(LocalGP* pPic,int n )
{
	m_inNewArrowsNum = n ; 
	if( n > MAX_ARROW * MAX_ARROW )		m_inNewArrowsNum = MAX_ARROW*MAX_ARROW;

	for(int i=0; i < m_inNewArrowsNum ; i++) {
		//m_pgppNewArrowsShad.Add(m_pdsMenu->addGPPicture(NULL,0,0,pPic->GPID,i));
		//m_pgppNewArrows.Add(m_pdsMenu->addGPPicture(NULL,0,0,pPic->GPID,i));
		m_pgppNewArrowsShad[i]	= m_pdsMenu->addGPPicture(NULL,0,0,pPic->GPID,i) ;
		m_pgppNewArrows[i]		= m_pdsMenu->addGPPicture(NULL,0,0,pPic->GPID,i) ;

		m_pgppNewArrowsShad[i]->ShapeFileID		= pPic->GPID;
		m_pgppNewArrowsShad[i]->ShapeSpriteID	= i;
		m_pgppNewArrowsShad[i]->Diffuse			= 0xAAFFFFFF;
		m_pgppNewArrowsShad[i]->Visible			= false;

		m_pgppNewArrows[i]->ShapeFileID			= pPic->GPID;
		m_pgppNewArrows[i]->ShapeSpriteID		= i;
		m_pgppNewArrows[i]->Diffuse				= 0xAAFFFFFF;
		m_pgppNewArrows[i]->Visible				= false;
	}

	int curOffset = 0;
	for	(int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->SetNewArrowOffset(curOffset);
		curOffset += m_pssdDT[i]->GetNNeighbor();
	}	
}
//----------------------------------------------
char*	CSectData::GetSectorName(int id) {	// grey
	if(id<m_inNOS && id>=0)
		return m_pssdDT[id]->GetSectorName();
	return  NULL;
}
//----------------------------------------------
int		CSectData::GetSecDefence(int id){	//grey
	if(id<m_inNOS && id>=0)
		return m_pssdDT[id]->GetSecDefence();
	return  -1;
}
//----------------------------------------------
int		CSectData::GetSecPopulation(int id){	//grey
	if(id<m_inNOS && id>=0)
		return m_pssdDT[id]->GetSecPopulation();
	return  -1;
}
//----------------------------------------------
int		CSectData::GetSecVillageNUM(int id){	//grey
	if(id<m_inNOS && id>=0)
		return m_pssdDT[id]->GetVillageNUM();
	return  -1;
}
//----------------------------------------------
char*	CSectData::GetResourceName(int id){
	if(id<m_inNOS && id>=0)
		return	m_pssdDT[id]->GetResourceName();
	return  NULL;
}
//----------------------------------------------
int		CSectData::GetResourceID(int id){
	if( id < m_inNOS && id >= 0 )
		return  m_pssdDT[id]->GetResourceID();
	return  -1;
}
//----------------------------------------------
void	CSectData::ReCountCoord(int wnew,int hnew){
	for (int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->ConvertCenters(wnew,hnew,m_fWM,m_fHM);
	}
}
//----------------------------------------------
void	CSectData::DeposeTo(int x,int y,bool setDefColor){
	if (setDefColor){
		for (int i=0; i<m_inNOS; i++){
			m_pssdDT[i]->SetDeffColor();
		}
	}

	//
	//if (m_pssdDT[0]->GetEnable()==false){ 
	//	return; 
	//}
	//if (m_pssdDT[0]->Visible==false){ 
	//	return; 
	//}
	//

	for (int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->DeposeTo(x,y);
		if (m_pssdDT[i]->GetMouseOver()){
		//	ViewNeighbor(i);
		}
	}
}
//----------------------------------------------
void	CSectData::SetSecOnMap(int offset, DialogsSystem* psdMenu){
	m_inSecOffset = offset;
	SimpleDialog* psdTemp = NULL;
	if (m_pdsMenu==NULL){
		m_pdsMenu = psdMenu;
	}

	for (int i=0; i<m_inNOS; i++){
		psdTemp = m_pdsMenu->Find(m_inSecOffset+i);
		m_pssdDT[i]->SetSecAddress(psdTemp);
		m_pssdDT[i]->DeposeTo(0,0);
	}
}
//----------------------------------------------
void	CSectData::SetSityOnMap(int offset, DialogsSystem* psdMenu)
{
	SimpleDialog* psdTemp = NULL;
	
	m_inSityOffset = offset;
	if (m_pdsMenu==NULL){
		m_pdsMenu = psdMenu;
	}

	for(int i=0; i < m_inNOS; i++) {
		psdTemp = m_pdsMenu->Find(m_inSityOffset+ 2*i ); //(m_inSityOffset+i);
		m_pssdDT[i]->SetSityAddress(psdTemp);
		//new
		psdTemp = m_pdsMenu->Find(m_inSityOffset+ 2*i +1 ); //(m_inSityOffset+i);
		m_pssdDT[i]->SetFortAddress(psdTemp);
		//new
		m_pssdDT[i]->SetSityType();
		m_pssdDT[i]->DeposeTo(0,0);
	}
}
//----------------------------------------------
void	CSectData::DriveMouse(int x,int y){
	for (int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->VissibleIfMouse();
	}
}
//----------------------------------------------
void	CSectData::ViewNeighbor(int SecID,int Player,CPlayer* ppPlayer)
{
	//int sid = 0;
	//for (int i=0; i<m_pssdDT[SecID]->GetNNeighbor(); i++){
	//	sid = m_pssdDT[SecID]->GetNeighborID(i);
	//	if (ppPlayer->IsUnitInSect(sid)==false){
	//		if (m_pssdDT[sid]->GetDeffOwner()!=Player)	m_pssdDT[sid]->SetSectAColor(+RGB(0,0,250));
	//		else										m_pssdDT[sid]->SetSectAColor(+RGB(0,250,0));
	//		m_pssdDT[sid]->SetVissible(true);
	//	}
	//}

	//for (int i=0; i<MAX_ARROW; i++){
	//	m_pgppArows[i]->Visible = false;
	//}
	
	//НЕ РИСОВАТЬ СТРЕЛКИ "БОЛЬШИЕ"
	/*
	int snid = 0;
	int x1,y1,x2,y2;
	for (int i=0; i<m_pssdDT[SecID]->GetNNeighbor(); i++){
		snid = m_pssdDT[SecID]->GetNeighborID(i);
		m_pssdDT[SecID]->GetCurXY(x1,y1);
		m_pssdDT[snid]->GetCurXY(x2,y2);
		setArrowCoord(x1,y1,x2,y2,i);

		if (ppPlayer->IsUnitInSect(snid)==false){
			if (m_pssdDT[snid]->GetDeffOwner()!=Player)	m_pgppArows[i]->Diffuse = 0xFF000000+RGB(0,0,250);//m_pgppArows[i]->Diffuse = +RGB(0,0,250);
			else										m_pgppArows[i]->Diffuse = 0xFF000000+RGB(0,250,0);//m_pgppArows[i]->Diffuse = +RGB(0,250,0);
			m_pssdDT[snid]->SetVissible(true);
		}
		else{
			m_pgppArows[i]->Visible = false;
		}
	}
	*/
}
//----------------------------------------------
// REAL VIEW NEIGHBOR!!!!
void	CSectData::ViewNeighbor(int dx,int dy,int SecID,int Player,CPlayer* ppPlayer) // функция отрисовки стрелок
{
	int x = 0, y = 0, a = 0, snid = 0;

	for (int i=0; i < m_inNewArrowsNum; i++) {
		if( m_pgppNewArrows[i] )       m_pgppNewArrows[i]->Visible		= false;
		if( m_pgppNewArrowsShad[i] )   m_pgppNewArrowsShad[i]->Visible	= false;
	}
	
	int  secOffset = m_pssdDT[SecID]->GetNewArrowOffset();
	for(int i=0; (i<m_pssdDT[SecID]->GetNNeighbor())&&(SecID!=-1)&&(ppPlayer!=NULL); i++)
	{
		//rd	= GPS.GetGPWidth(m_pgppArows[i]->ShapeFileID,m_pgppArows[i]->ShapeSpriteID);
		//rh	= GPS.GetGPHeight(m_pgppArows[i]->ShapeFileID,m_pgppArows[i]->ShapeSpriteID);
		m_pssdDT[SecID]->GetAroowCoord(x,y,a,i);
		//m_pgppArows[i]->SetRotScale(rd/2,rh/2,-((float)a/360.0)*(2.0*3.1415),0.3,0.3);
		//m_pgppArows[i]->x = x + dx;
		//m_pgppArows[i]->y = y + dy;

		if( secOffset+i >= m_inNewArrowsNum ) { continue ; } 
		m_pgppNewArrows[secOffset+i]->x		= x + dx;		m_pgppNewArrows[secOffset+i]->y	    = y + dy;
		m_pgppNewArrowsShad[secOffset+i]->x	= x + dx + 2;	m_pgppNewArrowsShad[secOffset+i]->y	= y + dy + 5;
				
		snid = m_pssdDT[SecID]->GetNeighborID(i);


		//if(ppPlayer->IsUnitInSect(snid)==false) {
		if( ppPlayer->IsUnitInSect(snid)==false && CanEnterIntoSector( Player , snid ) ) 
		{
			if( m_pssdDT[snid]->GetDeffOwner() != Player && CanAttackSector( Player , snid ) ) {
				m_pgppNewArrows[secOffset+i]->Diffuse		= 0xFF000000+RGB(0,0,230);
				m_pgppNewArrowsShad[secOffset+i]->Diffuse	= 0x55000000+RGB(0,0,0);
			}
			else {
				m_pgppNewArrows[secOffset+i]->Diffuse		= 0xFF000000+RGB(0,195,0);
				m_pgppNewArrowsShad[secOffset+i]->Diffuse	= 0x55000000+RGB(0,0,0);
			}
			m_pgppNewArrows[secOffset+i]->Visible		= true;
			m_pgppNewArrowsShad[secOffset+i]->Visible	= true;

			//new
			m_pgppNewArrows[secOffset+i]->x1 = m_pgppNewArrows[secOffset+i]->x + GPS.GetGPWidth(m_pgppNewArrows[secOffset+i]->FileID,m_pgppNewArrows[secOffset+i]->SpriteID&4095)-1;
			m_pgppNewArrows[secOffset+i]->y1 = m_pgppNewArrows[secOffset+i]->y + GPS.GetGPHeight(m_pgppNewArrows[secOffset+i]->FileID,m_pgppNewArrows[secOffset+i]->SpriteID&4095)-1;
			m_pgppNewArrowsShad[secOffset+i]->x1 = m_pgppNewArrowsShad[secOffset+i]->x + GPS.GetGPWidth(m_pgppNewArrowsShad[secOffset+i]->FileID,m_pgppNewArrowsShad[secOffset+i]->SpriteID&4095)-1;
			m_pgppNewArrowsShad[secOffset+i]->y1 = m_pgppNewArrowsShad[secOffset+i]->y + GPS.GetGPHeight(m_pgppNewArrowsShad[secOffset+i]->FileID,m_pgppNewArrowsShad[secOffset+i]->SpriteID&4095)-1;
			//new

			//m_pgppArows[i]->Visible = true;
		}
		else{
			//m_pgppArows[i]->Visible = false;
		}
	}
}
//----------------------------------------------
void	CSectData::InVisibleNeighbour(int SecID){
	for (int i=0; i<m_pssdDT[SecID]->GetNNeighbor(); i++){
		m_pssdDT[m_pssdDT[SecID]->GetNeighborID(i)]->SetVissible(false);
	}
}
//----------------------------------------------
int		CSectData::GetSectorsNum(){
	return	m_inNOS;
}
//----------------------------------------------
void	CSectData::GetSecSityXY(int ID,int &x,int &y){
	m_pssdDT[ID]->GetSityXY(x,y);
}
//----------------------------------------------
int		CSectData::GetNeighborsNum(int id){
	if (id<m_inNOS && id>=0)
		return m_pssdDT[id]->GetNNeighbor();
	return -1;
}
//----------------------------------------------
int		CSectData::GetNeighborsIDs(int id,int* ids,int n){
	if (id>=m_inNOS && id<0)
		return -1;

	if (ids==NULL || n<=0)
		return -1;

	int a_inNN = m_pssdDT[id]->GetNNeighbor();
	for (int i=0; (i<n)&&(i<a_inNN); i++){
		ids[i] = m_pssdDT[id]->GetNeighborID(i);
	}

	return a_inNN;
}
//----------------------------------------------
int		CSectData::GetSecOwner(int id){
	if( id >= 0 && id < m_inNOS )
		return m_psddDT[id]->GetOwner();
	return -1;
}
//----------------------------------------------
int		CSectData::GetHistoryOwner(int id) 
{ 
	if( id < 0 || id >= GetSectorsNum() )	{ return -1; }

	return  m_pssdDT[id]->GetHistoryOwner(); 
}
//----------------------------------------------
int		CSectData::GetPlayerSect(int PID){
	int n = 0;
	for (int i=0; i<m_inNOS; i++){
		if (m_psddDT[i]->GetOwner()==PID){
			n++;
		}
	}
	return n;
}
//----------------------------------------------
int		CSectData::GetMouseOverSect(){
	int i = 0;
	while (i<m_inNOS && m_pssdDT[i]->GetMouseOver()==false){
		i++;
	};
	if (i==m_inNOS){
		return -1;
	}
	return i;
}
//----------------------------------------------
int		CSectData::GetSecIDbyOwner(int NI)
{
	int ID = -1,  ID2 = -1,  ID3 = -1;
	for(int i=0; i < m_psddDT.GetAmountOfElements() && ID == -1 ; i++)
		if( m_psddDT[i]->GetOwner() == NI ) {
			if( ID2 < 0 || m_pssdDT[ID2]->GetSecDefence() < m_pssdDT[i]->GetSecDefence() )  ID2 = i ;
			if( m_pssdDT[i]->GetResourceID() == 2 ) {
				if( ID3 < 0 )								ID3 = i;  /*GOLD!*/  //grey ver.  //new
				if( m_pssdDT[i]->GetHistoryOwner() == NI )	ID  = i;  /*GOLD! -- capital*/  //grey ver. //new
			}
			//ID2 = i ;
			//if( m_pssdDT[i]->GetResourceID() == 2 ) { ID = i; } /*GOLD! -- capital*/  //grey ver.
		}
	if( ID < 0 )  ID = ID3; //new
	
	if( ID < 0 )  return  ID2;
	return	ID;
}
//----------------------------------------------
int		CSectData::GetPlayerSect(int* pSecID,int n,int PID)
{
	int nret = GetPlayerSect(PID);
	int a = 0;
	for(int i=0; i < m_inNOS ; i++) {
		if( m_psddDT[i]->GetOwner()==PID && (a<nret) && (a<n) ) {
			pSecID[a] = m_pssdDT[i]->GetSectID();
			a++;
		}
	}

	return nret;
}
//----------------------------------------------
SimpleDialog*	CSectData::GetSityPic(int id){
	return m_pssdDT[id]->GetSityPic();
}
//----------------------------------------------
bool CheckIfPtrExists(void* ptr);
void	CSectData::SetVisible(bool state){
	for (int i=0; i<m_inNOS; i++){
		assert(CheckIfPtrExists(m_pssdDT[i]));
		m_pssdDT[i]->SetVisible(state);
	}
}
//----------------------------------------------
void	CSectData::SetEnable(bool state){
	for (int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->SetEnable(state);
	}
	//
	//if( !state )  CreateHint2();
	//
}
//----------------------------------------------
void	CSectData::SetSecVisible(int id,bool state){
	if (id<m_inNOS && id>=0){
		m_pssdDT[id]->SetVissible(state);
	}
}
//----------------------------------------------
void	CSectData::SetSecColor(int id,int color,bool visible){
	if (0<=id && id<m_inNOS){
		m_pssdDT[id]->SetSectVisible(visible);
		m_pssdDT[id]->SetSectAColor(color);
	}
}
//----------------------------------------------
void	CSectData::SetDeffColor() 
{
	for (int i=0; i<m_inNOS; i++) {
		m_pssdDT[i]->SetDeffColor();
	}	
}
//----------------------------------------------
void	CSectData::setRedDip( int PlayerId , int* pPID,int n)
{
	for(int i=0; i < m_inNOS ; i++) {
		m_pssdDT[i]->SetDeffColor() ;
		if( GetSecOwner( i ) != PlayerId ) m_pssdDT[i]->SetSectColor( 0xFFFF ) ;
	}
	//SetDeffColor();

	////Все цвета полностью прозрачные.
	////for (int i=0; i<m_inNOS; i++)
	////	{
	////	if(GetSecOwner(i)!=GetSecOwner(m_LastLMCSecID))  //grey
	////		m_pssdDT[i]->SetSectAlpha(0x00); //m_pssdDT->SetSectAlpha(0x00);
	////PlayerId=GetSecOner(this->LastLMC)
	//   //  }

	if (pPID==NULL || n<=0){ 
		return; 
	}
	////Все цвета полностью прозрачные.
	////for (int i=0; i<m_inNOS; i++)
	////	{
	////	m_pssdDT->SetSectAlpha(0x00);
	//	//  }
	for (int i=0; i<n; i++){
		setOwnerColor(pPID[i],RED_SECT_COLOR,ALPHA_SECT);
	}
}
//----------------------------------------------
void	CSectData::setGreenDip( int PlayerId , int* pPID,int n){
	for (int i=0; i<n; i++){
		setOwnerColor(pPID[i],GREEN_SECT_COLOR,ALPHA_SECT);
	}
}
//----------------------------------------------
void	CSectData::setOwnerColor(int PID){
	setOwnerColor(PID,0x00FFFFFF,0x00000000);
}
//----------------------------------------------
void	CSectData::SetSelectedColor( int id , bool visible ) ////grey - for SELECTED
{
	if( !visible )  return; 	
	if( 0 <= id && id < m_inNOS ) {
		int nat = GetSecOwner( id ), CID = 6 ;
		if( nat >= 0 && nat < MAX_PLAYERS )  CID = nat;
		m_pssdDT[id]->SetSectAColor( SELECTED_SECT_COLOR + GetBigMapPlayerColor( CID ) );
		//m_pssdDT[id]->SetSectAColor( 0x88000000 + GetBigMapPlayerColor( GetSecOwner( CID ) ) );
	}	
}
//void	CSectData::SetSelectedColor( int id , bool visible ) ////grey - for SELECTED
//{
//	if( !visible ) return ;
//	if( GetSecOwner( id ) < 0 )   m_pssdDT[id]->SetSectAColor( SELECTED_SECT_COLOR ) ;
//	else						  m_pssdDT[id]->SetSectAColor( 0x88000000 + GetBigMapPlayerColor( GetSecOwner( id ) ) );
//	//m_pssdDT[id]->SetSectAColor( SELECTED_SECT_COLOR ) ;
//}
//----------------------------------------------
void	CSectData::setOwnerColor(int PID,int color,int alpha){
	for (int i=0; i<m_inNOS; i++){
		if (GetSecOwner(i)==PID){
			if		(alpha==0 && color==0x00FFFFFF){
				m_pssdDT[i]->SetDeffColor();
			}
			else if (alpha==0 && color!=0x00FFFFFF){
				m_pssdDT[i]->SetColor(color);
			}
			else if (alpha!=0 && color!=0x00FFFFFF){
				m_pssdDT[i]->SetSectAColor(alpha + color);
			}
		}
	}
}
//----------------------------------------------
void	CSectData::setArrowCoord(int x1,int y1,int x2,int y2, int id){
//	int		cx	= 0,	cy	=	0;		//Координаты центра стрелочки.
//	float	a	= 0.0;					//Угол поворота.
//	float	sx	= 0.0,	sy	=	1.0;	//Масштаб.
//	int		rd	= 0,	rh	=	0;		//Реальная высота и ширина картинки.
//
//	a	= GetAngle((float)(x2-x1),(float)(y2-y1));
//	cx	= (x2+x1)/2;	cy = (y2+y1)/2;
//	rd	= GPS.GetGPWidth(m_pgppArows[id]->ShapeFileID,m_pgppArows[id]->ShapeSpriteID);
//	rh	= GPS.GetGPHeight(m_pgppArows[id]->ShapeFileID,m_pgppArows[id]->ShapeSpriteID);
////	sy	= (float)rd / (float)rh;
//	float	d = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)) * (2.0/3.0);
//	sx	= d/(float)rd;
//
//	m_pgppArows[id]->SetRotScale(rd/2,rh/2,a,sx,sx);
//	m_pgppArows[id]->x = cx;
//	m_pgppArows[id]->y = cy;
//	m_pgppArows[id]->Visible = true;
}
//----------------------------------------------
void	CSectData::setArrowVisible(bool state)
{
	for (int i=0; i < m_inNewArrowsNum ; i++) { /*MAX_ARROW*/
		if( m_pgppNewArrows[i] )       m_pgppNewArrows[i]->Visible		= state ;
		if( m_pgppNewArrowsShad[i] )   m_pgppNewArrowsShad[i]->Visible	= state;
	}
	//!!!NEW!!! CSectData VS. DialogsSystem
}
//----------------------------------------------
void	CSectData::CreateHint(){
	for (int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->CreateHint();
	}
}
//----------------------------------------------

void	CSectData::CreateHint2(){
	for (int i=0; i<m_inNOS; i++){
		m_pssdDT[i]->CreateHint2();
	}
}

//----------------------------------------------
int		CSectData::GetNNeighbor(int id){
	return m_pssdDT[id]->GetNNeighbor();
}
//----------------------------------------------
int		CSectData::GetNeighborID(int id,int idx){
	return m_pssdDT[id]->GetNeighborID(idx);
}
//----------------------------------------------
int		CSectData::GetDeffOwner(int id){
	return m_psddDT[id]->GetOwner();
}
//----------------------------------------------
void	CSectData::GetSityXY(int id,int &x,int &y){
	m_pssdDT[id]->GetSityXY(x,y);
}
//----------------------------------------------
void	CSectData::SetSecDefeatDefence(int id)
{
	m_pssdDT[id]->m_inDefence = 0 ;
	if( m_pssdDT[id]->m_inPopulation > 0 && rand()%10 > 7 )  m_pssdDT[id]->m_inPopulation -= 1 ;
}
//----------------------------------------------
//----------------------------------------------
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
////////////// CSectorMenu /////////////
////////////////////////////////////////
//----------------------------------------------
CSectorMenu::CSectorMenu(CSectData* psdSectors){
	Init();
	if (psdSectors!=NULL)	m_psdSectData = psdSectors;
}
//----------------------------------------------
CSectorMenu::~CSectorMenu(){
	//if (m_ppsdMiniMap	!= NULL)		delete[]m_ppsdMiniMap;
	//if (m_ppsdFlag		!= NULL)	delete[]m_ppsdFlag;
	//if (m_ppsdPic		!= NULL)		delete[]m_ppsdPic;
	//for (int i=0; i<m_inTextBtN; i++){
	//	if (m_ppsdTextBt[i]!=NULL){
	//		GP_TextButton* pgptbA = reinterpret_cast<GP_TextButton*>(m_ppsdTextBt[i]);
	//		if (pgptbA->Message!=NULL){
	//			delete[](pgptbA->Message);
	//		}
	//	}
	//}
	//if (m_ppsdTextBt	!= NULL)		delete[]m_ppsdTextBt;

	Init();
}
//----------------------------------------------
void	CSectorMenu::Init()
{
	//X0		=	76;	//Y0		=	168;
	X0		=	690 /*700*/ ;
	Y0		=	110 /*120*/ ;

	dX		=	4;		
	dY		=	2;		
	delta	=	5;		

	m_psdSectData		= NULL;
	m_inCurSectID		= 0;

	m_ptbCounName		= NULL;//grey
	m_ptbSectName		= NULL;//grey
	m_ptbPopulation		= NULL;//grey
	m_ptbRecruits		= NULL;//grey

	m_ppsdMiniMap		= NULL;
	//m_inNMap			= 0;
	//m_ppsdPic			= NULL;	//m_inPicN			= 0;
	m_ppsdFlag			= NULL;
	//m_inFlagN			= 0;
	//m_ppsdTextBt		= NULL;	//m_inTextBtN			= 0;
	//m_pgpbBT01		= NULL;	//m_pgpbBT02			= NULL;

	//this->m_ptbDefenceT = NULL;//grey 
	m_ptbDefLevel   = NULL;//grey 
	m_pgpbDefence   = NULL;//grey
	m_pgpbDiversion = NULL;//grey
	m_pgppMMapBord  = NULL;//grey 
	m_pgppResPict   = NULL;//grey
	m_pDeLimit		= NULL;//grey

	m_iResNum = 7;
	for(int i =0; i < m_iResNum ; i++) { this->m_ptbResADD[i] = NULL; }

	for(int i=0; i < m_iDTN; i++)  m_ptbDText[i] = NULL;

}
//----------------------------------------------
void	CSectorMenu::CreateMiniMaps(DialogsSystem* pdsMenu,LocalGP* plgpMap,int size)
{
	if (pdsMenu==NULL || plgpMap==NULL || size==0)	return;

	m_ppsdMiniMap = pdsMenu->addGPPicture(NULL,0,0,plgpMap->GPID,0);
	//m_inNMap = size;
	//m_ppsdMiniMap = new SimpleDialog*[m_inNMap];
	//for (int i=0; i<size; i++){
	//	m_ppsdMiniMap[i] = dynamic_cast<SimpleDialog*>(pdsMenu->addGPPicture(NULL,0,0,plgpMap->GPID,i));
	//	m_ppsdMiniMap[i]->ShapeFileID	= plgpMap->GPID;
	//	m_ppsdMiniMap[i]->ShapeSpriteID = i;
	//	m_ppsdMiniMap[i]->Visible = false;
	//}
}
//----------------------------------------------
void	CSectorMenu::CreatePictureAndButton( DialogsSystem* psdMenu,LocalGP* plgpPic,LocalGP* pFlags )
{
	if(psdMenu==NULL || plgpPic==NULL)		return;

	char sTMP[256];
	strcpy( sTMP, "..." );
	
	//Font for text in sector menu.
	RLCFont*	font = &BlackFont;		//RLCFont*	a_prlcfPgPas = &YellowFont;

	//Add players flags.
	m_ppsdFlag = psdMenu->addGPPicture(NULL,0,0,pFlags->GPID,0);
	m_ppsdFlag->Visible = false;
	//m_inFlagN = MAX_PLAYERS+1;
	//m_ppsdFlag = new SimpleDialog*[m_inFlagN];
	//for (int i=0; i < m_inFlagN ; i++){
	//	m_ppsdFlag[i] = dynamic_cast<SimpleDialog*>(psdMenu->addGPPicture(NULL,0,0,pFlags->GPID,i));
	//	m_ppsdFlag[i]->ShapeFileID		= plgpPic->GPID;
	//	m_ppsdFlag[i]->ShapeSpriteID	= i;
	//	m_ppsdFlag[i]->Visible			= false;
	//}

	//Add country name button.
	m_ptbCounName = psdMenu->addTextButton(NULL,0,0, "_" ,&fonMenuTitle2[1],&fonMenuTitle2[1],&fonMenuTitle2[1], 0 ); //&BigRedFont,&BigRedFont,&BigRedFont, 0 );
	BigMapSetMessage( m_ptbCounName , sTMP );
	m_ptbCounName->Visible = false;

	//Add sector name button.
	m_ptbSectName = psdMenu->addTextButton(NULL,0,0, "_" ,&GrayFont,&GrayFont,&GrayFont, 0 );
	BigMapSetMessage( m_ptbSectName , sTMP );
	m_ptbSectName->Visible = false;
	
	// ADD Population text
	m_ptbPopulation = psdMenu->addTextButton(NULL,0,0, "_" ,&GrayFont,&GrayFont,&GrayFont , 0 );//SmallGrayFont1
	BigMapSetMessage( m_ptbPopulation , sTMP );
	m_ptbPopulation->Visible = false;
	
	// ADD Recruits text
	m_ptbRecruits = psdMenu->addTextButton(NULL,0,0, "_" ,&GrayFont,&GrayFont,&GrayFont , 0 ); 
	BigMapSetMessage( m_ptbRecruits , sTMP );
	m_ptbRecruits->Visible = false;

	// ADD Defence button
	LocalGP		a_lgpTextButton("Interf3\\TotalWarGraph\\dActions");
	m_pgpbDefence = psdMenu->addGP_TextButton(NULL,0,0,GetTextByID( "#UpgradeDef#" ),a_lgpTextButton.GPID,0,&RedFont,&OrangeFont);
	m_pgpbDefence->FontDy		= -2 ; // -1 ; //
	m_pgpbDefence->UserParam	= 0;
	m_pgpbDefence->Sprite		= 1; //active
	m_pgpbDefence->Sprite1		= 0; //passive
	m_pgpbDefence->Visible		= false;
	m_pgpbDefence->DisabledFont	= &GrayFont ;		
	m_pgpbDefence->OnUserClick  = &OnPIButtonClicked; // On Press This

	m_pgpbDefence->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
	m_pgpbDefence->AssignSound( "INTERF_UPDEF", CLICK_SOUND );

	_str	a_Hint;
	a_Hint = "{FS}{C FF502515}"; a_Hint += GetTextByID("#CHINT_UpgDef"); DYNHINT_RA(m_pgpbDefence,a_Hint.pchar());

	// ADD defence level
	m_ptbDefLevel = psdMenu->addTextButton(NULL,0,0, sTMP ,&GrayFont,&GrayFont,&GrayFont, 0 ); 
	BigMapSetMessage( m_ptbDefLevel , sTMP );
	m_ptbDefLevel->Visible		= false;
	
	// ADD Mini-Map border
	LocalGP		a_lgpElems("Interf3\\TotalWarGraph\\bmElements");
	m_pgppMMapBord			= psdMenu->addGPPicture(NULL,0,0,a_lgpElems.GPID, 1 ) ;
	// ADD resource pict 	
	m_pgppResPict			= psdMenu->addGPPicture(NULL,0,0,a_lgpElems.GPID, 4 ) ;
	_str	a_HintRP;
	a_HintRP = "{FS}{C FF502515}"; a_HintRP += GetTextByID("#CHINT_SectIncomeRes"); DYNHINT_RA(m_pgppResPict,a_HintRP.pchar());

	// ADD income-resource text
	for(int i=0; i < m_iResNum ; i++) {
		m_ptbResADD[i] = psdMenu->addTextButton(NULL,0,0, "" ,&RedFont,&RedFont,&RedFont, 1 ); //0 );
		BigMapSetMessage( m_ptbResADD[i] , sTMP );
		m_ptbResADD[i]->Visible = false;;
	}

	// ADD Diversion button
	m_pgpbDiversion = psdMenu->addGP_TextButton(NULL,0,0,GetTextByID( "#CWB_Diversion" ),a_lgpTextButton.GPID,0,&RedFont,&OrangeFont);
	m_pgpbDiversion->FontDy			= -2 ; // -1 ;
	m_pgpbDiversion->UserParam		= 0;
	m_pgpbDiversion->Sprite			= 1; //active
	m_pgpbDiversion->Sprite1		= 0; //passive
	//DEMO
	#ifdef DemoAI
		m_pgpbDiversion->Sprite  = 0; //active
		m_pgpbDiversion->ActiveFont = m_pgpbDiversion->PassiveFont = &GrayFont; //OrangeFont;
	#endif
	//DEMO
	m_pgpbDiversion->Visible		= false;
	m_pgpbDiversion->DisabledFont	= &GrayFont ;		
	m_pgpbDiversion->OnUserClick	= &OnPIButtonClicked; // On Press This

	m_pgpbDiversion->AssignSound( "INTERF_NAVEL",	 MOUSE_SOUND );
	m_pgpbDiversion->AssignSound( "INTERF_ENTERSAB", CLICK_SOUND );

	_str	a_Hint2;
	a_Hint2 = "{FS}{C FF502515}"; a_Hint2 += GetTextByID("#CHINT_Diversion"); DYNHINT_RA(m_pgpbDiversion,a_Hint2.pchar());
	
	// разделитель
	m_pDeLimit = psdMenu->addGPPicture(NULL, 0 , 0 ,a_lgpElems.GPID, 30 );
	m_pDeLimit->Visible = false;
}
//----------------------------------------------
void	CSectorMenu::SetMenuPosition()
{
	this->dX	 =  4 ;		
	this->dY	 =  3 ; 
	this->delta	 =  11 ;

	int YYY = 114 ; 
	// флаги														// 897,112
	m_ppsdFlag->x   = X0 + 214 ;
	m_ppsdFlag->y   = YYY ;
	m_ppsdFlag->x1 += m_ppsdFlag->x;
	m_ppsdFlag->y1 += m_ppsdFlag->y;
	//for (int i=0; i<m_inFlagN; i++) {	// флаги	// 897,112
	//	m_ppsdFlag[i]->x   = X0 + 214 ;
	//	m_ppsdFlag[i]->y   = YYY ;
	//	m_ppsdFlag[i]->x1 += m_ppsdFlag[i]->x;
	//	m_ppsdFlag[i]->y1 += m_ppsdFlag[i]->y;
	//}
	
	YYY += 5;
	// Country name text.
	m_ptbCounName->x   = X0 +15 ; // X0 + 2*dX ;
	m_ptbCounName->y   = YYY ;
	m_ptbCounName->x1 += m_ptbCounName->x ;
	m_ptbCounName->y1 += m_ptbCounName->y ;

	X0 += 11 ;

	YYY = m_ptbCounName->y1 + 3*dY ; //YYY = m_ptbCounName->y1 + delta +dY ;   
	// Sector name.
	m_ptbSectName->x   = m_ptbCounName->x; //X0 + 2*dX ;
	m_ptbSectName->y   = YYY ;
	m_ptbSectName->x1 += m_ptbSectName->x ;
	m_ptbSectName->y1 += m_ptbSectName->y ;

	YYY = m_ptbSectName->y1 + delta ;
	//Sector mini map.
	m_ppsdMiniMap->x   = X0 + dX ;
	m_ppsdMiniMap->y   = YYY;	
	m_ppsdMiniMap->x1 += m_ppsdMiniMap->x;
	m_ppsdMiniMap->y1 += m_ppsdMiniMap->y;	
	//for (int i=0; i<m_inNMap; i++){
	//	m_ppsdMiniMap[i]->x   = X0 + dX ;
	//	m_ppsdMiniMap[i]->y   = YYY;	
	//	m_ppsdMiniMap[i]->x1 += m_ppsdMiniMap[i]->x;
	//	m_ppsdMiniMap[i]->y1 += m_ppsdMiniMap[i]->y;	
	//}
	// Mini-Map Border
	m_pgppMMapBord->x   = m_ppsdMiniMap->x ;
	m_pgppMMapBord->y   = m_ppsdMiniMap->y ;
	//m_pgppMMapBord->x   = m_ppsdMiniMap[0]->x ;
	//m_pgppMMapBord->y   = m_ppsdMiniMap[0]->y ;
	m_pgppMMapBord->x1 += m_pgppMMapBord->x ;
	m_pgppMMapBord->y1 += m_pgppMMapBord->y ;

	YYY = m_pgppMMapBord->y1 + delta +dY ;

	// Sector population.
	m_ptbPopulation->x   = X0 + 2*dX -20; //-16 ; //-12 ;
	m_ptbPopulation->y   = YYY ;
	m_ptbPopulation->x1 += m_ptbPopulation->x ;
	m_ptbPopulation->y1 += m_ptbPopulation->y ;

	// Sector recruits.
	m_ptbRecruits->x   = X0 +127 ; //+132 ;
	m_ptbRecruits->y   = YYY ;		
	m_ptbRecruits->x1 += m_ptbRecruits->x ;
	m_ptbRecruits->y1 += m_ptbRecruits->y ;

	YYY = m_ptbPopulation->y1 + delta ;
	// resource pict 	
	m_pgppResPict->x   = X0 + dX ; 
	m_pgppResPict->y   = YYY ;
	m_pgppResPict->x1 += m_pgppResPict->x ;
	m_pgppResPict->y1 += m_pgppResPict->y ;

	// income-resource text
	for(int i=0; i < 4 ; i++) {	
		m_ptbResADD[i]->x += m_pgppResPict->x +35 +56*i ;			m_ptbResADD[i]->x1 += m_ptbResADD[i]->x;
		m_ptbResADD[i]->y += m_pgppResPict->y +45 ;					m_ptbResADD[i]->y1 += m_ptbResADD[i]->y;
	}
	for(int i=4; i < m_iResNum ; i++) {
		m_ptbResADD[i]->x = m_pgppResPict->x +50 +62*(i - 4) ;		m_ptbResADD[i]->x1 += m_ptbResADD[i]->x;
		m_ptbResADD[i]->y = m_pgppResPict->y +106 ;					m_ptbResADD[i]->y1 += m_ptbResADD[i]->y;
	}

	YYY = m_pgppResPict->y1 + delta +dY ;
	// Defence Level text
	m_ptbDefLevel->x   = X0 + 2*dX ; 
	m_ptbDefLevel->y   = YYY;
	m_ptbDefLevel->x1 += m_ptbDefLevel->x;
	m_ptbDefLevel->y1 += m_ptbDefLevel->y;

	YYY = m_ptbDefLevel->y1 + delta; 
	// Defence button
	m_pgpbDefence->x   = X0 + dX ; 
	m_pgpbDefence->y   = YYY;
	m_pgpbDefence->x1 += m_pgpbDefence->x ;
	m_pgpbDefence->y1 += m_pgpbDefence->y ;

	// Diversions button
	m_pgpbDiversion->x  = m_pgpbDefence->x ; 
	m_pgpbDiversion->y  = m_pgpbDefence->y ;
	m_pgpbDiversion->x1 = m_pgpbDefence->x1 ;
	m_pgpbDiversion->y1 = m_pgpbDefence->y1 ;

	YYY = m_pgpbDefence->y1 + delta +1 ;
	int  dwx = m_pgppResPict->x + ( m_pgppResPict->x1 - m_pgppResPict->x - m_pDeLimit->x1 + m_pDeLimit->x ) / 2 +2 ;	
	// разделитель
	m_pDeLimit->x   = dwx ;
	m_pDeLimit->y   = YYY ; 
	m_pDeLimit->x1 += m_pDeLimit->x ;
	m_pDeLimit->y1 += m_pDeLimit->y ;
}
//----------------------------------------------
void	CSectorMenu::SetMenuData( int sectID , CPlayer* pPlayer[MAX_PLAYERS] )
{	
	if( sectID == m_inCurSectID )	{ return; }

	m_inCurSectID = sectID;

	SetVisible(false);

	//m_ppsdMiniMap[m_inCurSectID]->Visible = true;
	m_ppsdMiniMap->Visible  = true;
	m_ppsdMiniMap->SpriteID = m_inCurSectID;

	CheckDefenceButton(   NULL, NULL, m_inCurSectID , m_psdSectData->GetSecOwner( m_inCurSectID ) );
	CheckDiversionButton( NULL, NULL, m_inCurSectID , m_psdSectData->GetSecOwner( m_inCurSectID ) );

	char	s[256], sTMP[256];
	// Country Name
	int PID = m_psdSectData->GetSecOwner( m_inCurSectID );
	if( PID >= 0 && PID < MAX_PLAYERS  ) 
			sprintf(s, "%s" , pPlayer[ PID ]->GetPlayerName() ) ;
	else	sprintf(s, "%s" , GetTextByID( "#REBEL" ) ) ;
	m_ptbCounName->SetMessage( s );
	m_ptbCounName->Visible = true;
	// Country Flag
	m_ppsdFlag->SpriteID = m_psdSectData->GetSecOwner( m_inCurSectID ) + 1;
	if( m_ppsdFlag->SpriteID > 0 && m_ppsdFlag->SpriteID <= MAX_PLAYERS  )
		m_ppsdFlag->Visible  = true;
	//int fid = m_psdSectData->GetSecOwner( m_inCurSectID ) + 1;
	//if( fid > 0 && fid < m_inFlagN  ) {
	//	m_ppsdFlag[fid]->Visible = true;
	//}

	// Sector Name
	sprintf(s, "%s  {R FF000000}%s{C}" , GetTextByID("#CDT_Area") , m_psdSectData->GetSectorName(m_inCurSectID) ) ;
	m_ptbSectName->SetMessage( s );
	m_ptbSectName->Visible = true;

	// Population "Level"
	//int	population = m_psdSectData->GetSecPopulation( m_inCurSectID ) + 1 ;
	//sprintf(s, "%s {R FF000000 }%d 000{C}" , GetTextByID("#Population#") , population * 100 ) ; // #CWT_PopltnLVL%d
	int	population = m_psdSectData->GetSecPopulation( m_inCurSectID ) ;
	if( population < 0 || population > MAX_POPULATION )  population = 0;
	sprintf(sTMP, "#CWT_PopltnLVL%d" , population++ );//very new
	sprintf(s,    "%s {R FF000000}%s{C}" , GetTextByID("#Population#") , GetTextByID( sTMP ) ) ;//very new
	m_ptbPopulation->SetMessage( s );
	m_ptbPopulation->Visible = true;
	// Recruits "Present/Total"
	int recrt = m_psdSectData->m_pssdDT[ m_inCurSectID ]->m_iRecruits ;
	if( recrt < 100 && population < 1 ) { 
		sprintf(s, "%s  {R FF000000}%d/%d{C}" , GetTextByID("#Mobilization#") , recrt , ++population * SECT_MAX_RECRTS );	}
	else { 
		sprintf(s, "%s {R FF000000}%d/%d{C}" , GetTextByID("#Mobilization#") , recrt , population * SECT_MAX_RECRTS ); }
	m_ptbRecruits->SetMessage( s );
	m_ptbRecruits->Visible = true;

	// Defence Level text
	Enumerator*	pDefLvl_ENUM = ENUM.Get( "DefLvl_ENUM" );
	int SecDef = m_psdSectData->GetSecDefence( m_inCurSectID );
	if( SecDef >= 0 && SecDef <= MAX_DEFENCE ) {
		sprintf(s, "%s {R FF000000}%s{C}" , GetTextByID("#Defence#") , pDefLvl_ENUM->Get( SecDef ) ) ;
		m_ptbDefLevel->SetMessage( s );
	}
	m_ptbDefLevel->Visible  = true;
	
	// Mini-Map Border	
	m_pgppMMapBord->Visible = true;
	// resourse pict 	
	m_pgppResPict->Visible  = true;
	// разделитель
	m_pDeLimit->Visible		= true;

	// + resource[i]
	sprintf(s,"+%d", SECT_INCOME[ WOOD ] );	
	m_ptbResADD[0]->SetMessage( s );	// wood
	sprintf(s,"+%d", SECT_INCOME[ FOOD ] );
	m_ptbResADD[1]->SetMessage( s );	// food
	sprintf(s,"+%d", SECT_INCOME[ STONE ] );
	m_ptbResADD[2]->SetMessage( s );	// stone
	sprintf(s,"+%d", SECT_INCOME[ GOLD ] );
	m_ptbResADD[3]->SetMessage( s );	// gold
	sprintf(s,"+%d", SECT_INCOME[ IRON ] );
	m_ptbResADD[4]->SetMessage( s );	// iron
	sprintf(s,"+%d", SECT_INCOME[ COAL ] );
	m_ptbResADD[5]->SetMessage( s );	// coal

	sprintf(s,"+%d", population * SECT_REG_RECRTS );
	m_ptbResADD[6]->SetMessage( s );	// recruit

	int AddId  = 0 ;
	int resID		  = m_psdSectData->GetResourceID( m_inCurSectID ) ;
	if( resID > 0 )
	{
		switch(resID) {
			case 1:				// FOOD
					AddId = 1 ; 
					break;
			case 2:				// GOLD
					AddId = 3 ; 
					break;
			case 3:				// IRON
					AddId = 4 ; 
					break;
			case 4:				// COAL
					AddId = 5 ; 
					break;
			default:			//No ressource.
					AddId = -1 ; 
		}

		if( AddId > 0 )
		{
			sprintf(s,"+%d", GetGoldForRess( resID , population ) );
			m_ptbResADD[ AddId ]->SetMessage( s );	// main resource
		}
	}

	// Diversions
	char	zero[256];	strcpy( zero , "+0" );
	if( m_psdSectData->m_pssdDT[ m_inCurSectID ]->m_iSabotageID%10 > 0 ) { // (recruits = 0)
		m_ptbResADD[6]->SetMessage( zero );	// recruit
	}
	if( m_psdSectData->m_pssdDT[ m_inCurSectID ]->m_iSabotageID%100 / 10 > 0 ) { // (all resources = 0)
		m_ptbResADD[0]->SetMessage( zero );	// wood
		m_ptbResADD[1]->SetMessage( zero );	// food
		m_ptbResADD[2]->SetMessage( zero );	// stone
		m_ptbResADD[3]->SetMessage( zero );	// gold
		m_ptbResADD[4]->SetMessage( zero );	// iron
		m_ptbResADD[5]->SetMessage( zero );	// coal
	}

	if( !m_ptbResADD[0]->Visible ) for(int i=0; i < m_iResNum ; i++) { m_ptbResADD[i]->Visible = true; }
}
//----------------------------------------------
void	CSectorMenu::SetEnabled(bool status)
{
	//for (int i=0; i<m_inNMap; i++ )	m_ppsdMiniMap[i]->Enabled = status;
	//for (int i=0; i<m_inFlagN; i++ )	m_ppsdFlag[i]->Enabled = status;
	m_ppsdFlag->Enabled		 = status;
	m_ppsdMiniMap->Enabled	 = status;

	m_ptbCounName->Enabled   = status ;
	m_ptbSectName->Enabled   = status ;
	m_ptbPopulation->Enabled = status ;
	m_ptbRecruits->Enabled   = status ;

	// resourse pict 	
	m_pgppResPict->Enabled   = status ;
	m_pDeLimit->Enabled		 = status ;

	// Defence Button
	m_pgpbDefence->Enabled   = status ;
	if( status )   CheckDefenceButton( NULL, NULL, m_inCurSectID , m_psdSectData->GetSecOwner( m_inCurSectID ) );

	// Defence Level text
	m_ptbDefLevel->Enabled   = status ;

	for(int i=0; i < m_iResNum; i++) m_ptbResADD[i]->Enabled = status ;

	// Diversions button
	m_pgpbDiversion->Enabled   = status ;	
}
//----------------------------------------------
void	CSectorMenu::SetEnabledState( bool state )
{
#ifndef DemoAI
	// normal state
	if( state ) {
		m_pgpbDefence->ActiveFont	= m_pgpbDiversion->ActiveFont	= &RedFont;
		m_pgpbDefence->PassiveFont	= m_pgpbDiversion->PassiveFont	= &OrangeFont;
		m_pgpbDefence->DisabledFont	= m_pgpbDiversion->DisabledFont = &GrayFont;
	}
	// active buttons
	if( !state && m_pgpbDefence->Sprite == 2 ) {
		m_pgpbDefence->PassiveFont	= m_pgpbDefence->DisabledFont	= &RedFont;	
	}
	if( !state && m_pgpbDiversion->Sprite == 2 ) {
		m_pgpbDiversion->PassiveFont = m_pgpbDiversion->DisabledFont = &RedFont;
	}
	// not active/not enabled 
	if( !state && m_pgpbDefence->Sprite != 2   &&  m_pgpbDefence->Enabled ) {
		m_pgpbDefence->DisabledFont	= &OrangeFont;
	}
	if( !state && m_pgpbDiversion->Sprite != 2 &&  m_pgpbDiversion->Enabled ) {
		m_pgpbDiversion->DisabledFont = &OrangeFont;
	}
#endif DemoAI
}
//----------------------------------------------
void	CSectorMenu::SetVisible(bool status)
{
	//for(int i=0; i < m_inFlagN ; i++)	m_ppsdFlag[i]->Visible	  = status;
	//for(int i=0; i < m_inNMap ; i++)	m_ppsdMiniMap[i]->Visible = status;
	m_ppsdFlag->Visible		 = status;
    m_ppsdMiniMap->Visible	 = status;

	m_ptbCounName->Visible   = status ;
	m_ptbSectName->Visible   = status ;
	m_ptbPopulation->Visible = status ;
	m_ptbRecruits->Visible   = status ;
	m_pgppMMapBord->Visible  = status ;	// Mini-Map Border
	m_pgppResPict->Visible   = status ;	// resourse pict 
	m_pDeLimit->Visible		 = status ;	// разделитель 

	m_pgpbDefence->Visible   = status ;	// Defence Button
	m_ptbDefLevel->Visible   = status ;	// Defence Level text
	m_pgpbDiversion->Visible = status ;	// Diversions button

	for(int i=0; i < m_iResNum; i++)  m_ptbResADD[i]->Visible = status ;
}
//----------------------------------------------
bool	CSectorMenu::CheckDefenceButton( CDiplMenu* pDiplMenu , CPlayer* pPlayer, int sectID , int playerNation ) //grey
{
	if( sectID < 0 )													return false;
	if( m_psdSectData && sectID >= m_psdSectData->GetSectorsNum() )		return false;

	if(m_psdSectData->GetSecOwner(sectID) != playerNation) { m_pgpbDefence->Visible = false; return true; }
	else												   { m_pgpbDefence->Visible = m_pgpbDefence->Enabled = true; }


#ifdef DemoAI	//DEMO
	if(m_pgpbDefence->Enabled && sectID != 1 && sectID != 2 && sectID != 5 && sectID != 6) 
		m_pgpbDefence->Enabled = false;
#endif			//DEMO 


	if( !pDiplMenu || !pPlayer || !m_psdSectData )	return false;


	bool  DefPressed = false;
	if( m_pgpbDefence->UserParam ) {
		DefPressed = true;
		m_pgpbDefence->UserParam = 0;

		m_pgpbDefence->Sprite = m_pgpbDefence->Sprite1 = 2 ; //active	
	}


	_str  a_Hint, a_Red, tBlack, tBrown;		
	a_Red   = "{CR}";	  tBlack = "{C FF101010}";   tBrown = "{C FF502515}";
	a_Hint  = "{FS}";
	a_Hint += tBrown;
	a_Hint += GetTextByID("#CHINT_UpgDef");
	a_Hint += " \\ "; //	a_Hint += " \\ ("; 


	bool  DefDisabled = false;
	if( m_psdSectData->GetSecDefence( sectID ) > m_psdSectData->GetSecPopulation( sectID ) ) 
	{ 
		DefDisabled = true;

		if( m_psdSectData->GetSecDefence(sectID) >= MAX_DEFENCE ) {
			a_Hint += GetTextByID("#UPDEFMAX"); 
		}
		else {
			a_Hint += GetTextByID("#UPDEFLOWP"); 
		}
	}	
	else {	
		if( !m_pgpbDefence->Enabled )  m_pgpbDefence->Enabled = true;
	}


	int		SecDef  = m_psdSectData->GetSecDefence( sectID ) + 1 ;
	int		mult	= (SecDef<2)? 1 :( (SecDef==2)? SECT_DEF_multiplier2 : SECT_DEF_multiplier3 );

	int		dCOST[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
	dCOST[ WOOD  ] = SECT_DEFENCE[ WOOD  ] * mult ;
	dCOST[ STONE ] = SECT_DEFENCE[ STONE ] * mult ;
	dCOST[ GOLD  ] = SECT_DEFENCE[ GOLD  ] * mult ;
	dCOST[ RECRT ] = SECT_DEFENCE[ RECRT ] * mult ; 		//dCOST[ FOOD  ] = SECT_DEFENCE[ FOOD  ] * mult ; 


	bool	EnRes[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } ;
	bool	EnoughRes = true;
	for(int res=0; res <= RECRT ; res++) {
		if( dCOST[res] > 0 && pPlayer->GetResource( res ) < dCOST[ res] )  EnRes[res] = false;
		EnoughRes = EnoughRes && EnRes[res];
	}


	_str	a_Cost;
	EngSettings.Resource.SetHint( playerNation, dCOST, &a_Cost);//new
	a_Cost += BIGMAP_RECRTICON ;	
	a_Cost += tBlack ;
	if( !EnRes[RECRT] )  a_Cost += a_Red;
	a_Cost += dCOST[RECRT];
	if( !EnRes[RECRT] )  a_Cost += tBlack;


	//int  ResNN = 3, ResLST[10] = { RECRT, WOOD, STONE, GOLD, 0,0,0,0,0,0 };

	//_str	a_Cost;
	//for(int res=0; res <= ResNN ; res++)
	//{
	//	int resID = ResLST[res];
	//	if( !EnRes[resID] )  a_Cost += a_Red;
	//	a_Cost += dCOST[ resID ];		
	//	a_Cost += " ";		
	//	if( resID == RECRT )		{  a_Cost += GetTextByID("ResName9");	}
	//	else						{  a_Cost += RDS[ resID ].Name;	}
	//	if( !EnRes[resID] )   a_Cost += tBlack;
	//	if( res != ResNN )	  a_Cost += ", ";
	//}


	// SetNewHint
	if(!DefDisabled)  a_Hint += a_Cost.pchar();
	//a_Hint += ")";
	DYNHINT_RA( m_pgpbDefence , a_Hint.pchar() );


	char  text[256];
	if( DefPressed ) {
		if( DefDisabled ) {
			if( m_psdSectData->GetSecDefence(sectID) >= MAX_DEFENCE ) {
				sprintf(text, GetTextByID("#UPDEFMAX") );
			}
			else {
				sprintf(text, GetTextByID("#UPDEFLOWP") );
			}
			ShowBigMapGlobalMessage(1, 33 , 17 , text ,0); 
		}
		else {
			if( EnoughRes ) {
				sprintf(text, GetTextByID("#CWM_UpgradeDef") , a_Cost.pchar() );
				if( ShowBigMapGlobalMessage(2, 28 , 17 , text ,0) == 1 ) {
					m_psdSectData->m_pssdDT[ sectID ]->m_inDefence = SecDef ;
					for(int rr=0; rr < COAL ; rr++) pPlayer->AddResource( rr,  -dCOST[ rr  ] );
					DeleteRecruitsInSectors( playerNation , sectID , dCOST[ RECRT ] );
				}
			}
			else { 
				int pic = 16; if( m_psdSectData->GetSecOwner(sectID) == 5 ) pic = 17;
				sprintf(text, GetTextByID("#CWM_NoEnoughRes") , a_Cost.pchar() );
				ShowBigMapGlobalMessage(1, pic , 17 , text ,0); 
			}
		}

		m_pgpbDefence->Sprite  = 1; //active
		m_pgpbDefence->Sprite1 = 0; //passive

		m_inCurSectID = -1;
		return true;
	}
	return false;


		//if(m_psdSectData->GetSecDefence(sectID) > m_psdSectData->GetSecPopulation(sectID)) { 
		//	if( m_pgpbDefence->Enabled) m_pgpbDefence->Enabled = false; return true;		}	
		//else {	if(!m_pgpbDefence->Enabled) m_pgpbDefence->Enabled = true;	}
		//
			//_str	a_Cost ;	//a_Cost += "(" ; 
			//a_Cost  = dCOST[ RECRT ] ;		a_Cost += " " ;	    a_Cost += GetTextByID("ResName9") ;	a_Cost += ", " ;
			//a_Cost += dCOST[ WOOD  ] ;		a_Cost += " " ;		a_Cost += RDS[ WOOD  ].Name ;		a_Cost += ", " ;
			////a_Cost += dCOST[ FOOD  ] ;		a_Cost += " " ;		a_Cost += RDS[ FOOD  ].Name ;		a_Cost += ", " ;
			//a_Cost += dCOST[ STONE ] ;		a_Cost += " " ;		a_Cost += RDS[ STONE ].Name ;		a_Cost += ", " ;
			//a_Cost += dCOST[ GOLD  ] ;		a_Cost += " " ;		a_Cost += RDS[ GOLD  ].Name ;		//a_Cost += ")"  ;

			//if( pPlayer->GetResource( WOOD  ) >= dCOST[ WOOD  ] && /*pPlayer->GetResource( FOOD  ) >= dCOST[ FOOD  ] && */
			//	pPlayer->GetResource( STONE ) >= dCOST[ STONE ] && pPlayer->GetResource( GOLD  ) >= dCOST[ GOLD  ] && 
			//	pPlayer->GetResource( RECRT ) >= dCOST[ RECRT ] ) 
			//{
			//	sprintf(text, GetTextByID("#CWM_UpgradeDef") , a_Cost.pchar() );
			//	if( ShowBigMapGlobalMessage(2, 28 , 17 , text ,0) == 1 ) {
			//		m_psdSectData->m_pssdDT[ sectID ]->m_inDefence = SecDef ;
			//		for(int rr=0; rr < COAL ; rr++) pPlayer->AddResource( rr,  -dCOST[ rr  ] );
			//		DeleteRecruitsInSectors( playerNation , sectID , dCOST[ RECRT ] );
			//	}
			//}
			//else { 
			//	int pic = 16; if( m_psdSectData->GetSecOwner(sectID) == 5 ) pic = 17;
			//	sprintf(text, GetTextByID("#CWM_NoEnoughRes") , a_Cost.pchar() );
			//	ShowBigMapGlobalMessage(1, pic , 17 , text ,0); 
			//}

	//	}

	//	m_pgpbDefence->Sprite  = 1; //active
	//	m_pgpbDefence->Sprite1 = 0; //passive
	//	
	//	m_inCurSectID = -1;
	//	return true;
	//}
	//return false;
}
//----------------------------------------------
bool	CSectorMenu::CheckDiversionButton( CDiplMenu* pDiplMenu , CPlayer* pPlayer, int sectID , int playerNation )
{
	if(	m_psdSectData->GetSecOwner(sectID) == playerNation )	{ m_pgpbDiversion->Visible = false; return true; }
	else														{ m_pgpbDiversion->Visible = true; }

	#ifdef DemoAI	//DEMO
		m_pgpbDiversion->Enabled = false; return true ;
	#endif			//DEMO

	//
	_str	a_Hint2;
	a_Hint2 = "{FS}{C FF502515}"; 
	a_Hint2 += GetTextByID("#CHINT_Diversion"); 
	if(  m_psdSectData->m_pssdDT[ sectID ]->m_iSabotageID%10 > 0  && 
		 m_psdSectData->m_pssdDT[ sectID ]->m_iSabotageID%100 / 10 > 0 &&
		(m_psdSectData->m_pssdDT[sectID]->m_iSabotageID%1000 / 100 > 0 || m_psdSectData->GetSecDefence(sectID)==0) && 
		(m_psdSectData->GetSecOwner(sectID) == -1 || ROOT_SCENARIO.GetHeroInSect( sectID, 0xFFFF ) != NULL ) )
	{
		a_Hint2 += " \\ "; //	a_Hint += " \\ ("; 
		a_Hint2 += GetTextByID("#SABSECDONE") ;
	}
	DYNHINT_RA(m_pgpbDiversion,a_Hint2.pchar());
	//

	if( m_pgpbDiversion->UserParam ) {
		m_pgpbDiversion->UserParam = 0;
		m_pgpbDiversion->Sprite = m_pgpbDiversion->Sprite1 = 2 ; //active	

		ShowBigMapGlobalMessage( 10 , 23 , 20 , "_" , 0 );

		m_pgpbDiversion->Sprite  = 1; //active
		m_pgpbDiversion->Sprite1 = 0; //passive
		this->m_inCurSectID = -1;//new
		return true ;
	}
	return true ;
}
//----------------------------------------------
bool	CSectorMenu::ShowDiversionActions( DialogsSystem* pMenu , int x0, int y0 )
{
	char sTMP[256];
	sprintf( sTMP, "#CWM_Diversion0" );		// - ( 5 + 3 * 0 ) * def  ) ;

	RLCFont*	ActFont = &fonMenuText[1] ;	//NEED NEW FONTS? 
	RLCFont*	PasFont = &fonMenuText[0] ;	//NEED NEW FONTS? 
	RLCFont*	DisFont = &fonMenuText[6] ;	//NEED NEW FONTS? 

	LocalGP		a_lgpMess("Interf3\\TotalWarGraph\\dMessage");	
	//
	//TextButton* ptbText0 = NULL;	TextButton* ptbText1 = NULL;	TextButton* ptbText2 = NULL;	TextButton* ptbText3 = NULL;
	for(int i=0; i < m_iDTN; i++)  m_ptbDText[i] = NULL;
	//

	int	 mDY = 20 , ddx = 4; //2;	//int	 mDY = 8 ; //15 ;

	int  secID = m_inCurSectID ;
	int  def   = m_psdSectData->GetSecDefence( secID );	if( def < 0 || def > MAX_DEFENCE )  def = 0;


	// (recruits = 0)				id = 1
	int money	= SECT_SABOTAGE[0][def];	// money	= SECT_SABOTAGE[0] * ( def + 1 ) ;
	int pt		= SECT_SAB_PERCENT[0][def] ; // percent
	//
	m_ptbDText[0] = pMenu->addGP_TextButton( NULL, x0 +3  , y0 +ddx /*+3*/ , sTMP, a_lgpMess.GPID, 10,PasFont,PasFont);
	m_ptbDText[0]->Center		= false;
	m_ptbDText[0]->DisabledFont = DisFont;
	//
	//sprintf( sTMP, GetTextByID( "#CWM_Diversion0" ) , money ,  pt );		// - ( 5 + 3 * 0 ) * def  ) ;
	//m_ptbDText[0] = pMenu->addTextButton( NULL, x0 , y0 + mDY , sTMP,ActFont,PasFont,DisFont,0);
	//m_ptbDText[0]->AssignID(0x180);
	m_ptbDText[0]->UserParam	= money ;		
	m_ptbDText[0]->OnUserClick	= &OnDiversionClicked ;
	m_ptbDText[0]->AssignSound( "INTERF_SAB",	CLICK_SOUND );		 
	m_ptbDText[0]->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );

	_str	a_Hint2;  a_Hint2.DString::Clear();
	a_Hint2 = "{FS}{C FF502515}";  a_Hint2 += GetTextByID("#CHINT_Diversion0");  DYNHINT_RA( m_ptbDText[0] ,a_Hint2.pchar());

	if( m_psdSectData->m_pssdDT[ secID ]->m_iSabotageID%10 > 0 )				m_ptbDText[0]->Visible = false;


	// (all resources = 0)			id = 10
	money	= SECT_SABOTAGE[1][def]; // * ( def + 1 ) ; // 1000 * ( def + 1 ) ;
	pt		= SECT_SAB_PERCENT[1][def] ; // percent
	//sprintf( sTMP, GetTextByID( "#CWM_Diversion1" ) , money ,  pt );		// - ( 5 + 3 * 1 ) * def  ) ;
	m_ptbDText[1] = pMenu->addGP_TextButton( NULL, m_ptbDText[0]->x , m_ptbDText[0]->y1 +ddx , sTMP, a_lgpMess.GPID, 10,PasFont,PasFont);
	m_ptbDText[1]->Center		= false;
	m_ptbDText[1]->DisabledFont = DisFont;
	m_ptbDText[1]->UserParam	= money + 1  ;
	m_ptbDText[1]->OnUserClick	= &OnDiversionClicked ;
	m_ptbDText[1]->AssignSound( "INTERF_SAB",	CLICK_SOUND );		 
	m_ptbDText[1]->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );

	a_Hint2.DString::Clear();
	a_Hint2 = "{FS}{C FF502515}";  a_Hint2 += GetTextByID("#CHINT_Diversion1");  DYNHINT_RA( m_ptbDText[1] ,a_Hint2.pchar());

	if( m_psdSectData->m_pssdDT[ secID ]->m_iSabotageID%100 / 10 > 0 )			m_ptbDText[1]->Visible = false;


	// (defence - 1)				id = 100
	money	= SECT_SABOTAGE[2][def]; // * ( def + 1 ) ; // 3000 * ( def + 1 ) ;
	pt		= SECT_SAB_PERCENT[2][def] ; // percent
	//sprintf( sTMP, GetTextByID( "#CWM_Diversion2" ) , money ,  pt );		// - ( 5 + 3 * 2 ) * def  ) ;
	m_ptbDText[2] = pMenu->addGP_TextButton( NULL, m_ptbDText[0]->x , m_ptbDText[1]->y1 +ddx , sTMP, a_lgpMess.GPID, 10,PasFont,PasFont);
	m_ptbDText[2]->Center		= false;
	m_ptbDText[2]->DisabledFont = DisFont;
	m_ptbDText[2]->UserParam	= money + 2 ;
	m_ptbDText[2]->OnUserClick	= &OnDiversionClicked ;
	m_ptbDText[2]->AssignSound( "INTERF_SAB",	CLICK_SOUND );		 
	m_ptbDText[2]->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );

	a_Hint2.DString::Clear();
	a_Hint2 = "{FS}{C FF502515}";  a_Hint2 += GetTextByID("#CHINT_Diversion2");  DYNHINT_RA( m_ptbDText[2] ,a_Hint2.pchar());

	if( m_psdSectData->m_pssdDT[ secID ]->m_iSabotageID%1000 / 100 > 0 )		m_ptbDText[2]->Visible = false;
	if( !def )																	m_ptbDText[2]->Visible = false;


	// (rebellion, nation = -1)		id = 1000
	money	= SECT_SABOTAGE[3][def]; // * ( def + 1 ) ; // 5000 * ( def + 1 ) ;
	pt		= SECT_SAB_PERCENT[3][def] ; // percent
	//sprintf( sTMP, GetTextByID( "#CWM_Diversion3" ) , money ,  pt );		// - ( 5 + 3 * 3 ) * def  ) ;
	m_ptbDText[3] = pMenu->addGP_TextButton( NULL, m_ptbDText[0]->x , m_ptbDText[2]->y1 +ddx , sTMP, a_lgpMess.GPID, 10,PasFont,PasFont);
	m_ptbDText[3]->Center		= false;
	m_ptbDText[3]->DisabledFont = DisFont;
	m_ptbDText[3]->UserParam	= money + 3 ;
	m_ptbDText[3]->OnUserClick	= &OnDiversionClicked ;
	m_ptbDText[3]->AssignSound( "INTERF_SAB",	CLICK_SOUND );		 
	m_ptbDText[3]->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );

	a_Hint2.DString::Clear();
	a_Hint2 = "{FS}{C FF502515}";  a_Hint2 += GetTextByID("#CHINT_Diversion3");  DYNHINT_RA(m_ptbDText[3],a_Hint2.pchar());

	if( m_psdSectData->GetSecOwner( secID ) == -1 || 
		ROOT_SCENARIO.GetHeroInSect( secID , 0xFFFF ) != NULL )					m_ptbDText[3]->Visible = false;


	//
	SetDiversionTexts( def ); //very new!!!

	bool  DivDisabled = true;	
	for(int i=0; i < m_iDTN; i++) {
		if( m_ptbDText[i]->Visible )  DivDisabled = false;
	}
	if( DivDisabled ) {
		ShowBigMapGlobalMessage( 1, 24 , 20 , GetTextByID("#SABSECDONE") ,0 ); 
		return false;
	}
	//

	return true;

	//new
	//if( m_psdSectData->m_pssdDT[ secID ]->m_iSabotageID%10 > 0 )					ptbText0->Enabled = false ;
	//if( m_psdSectData->m_pssdDT[ secID ]->m_iSabotageID%100 / 10 > 0 )			ptbText1->Enabled = false ;
	//if( m_psdSectData->m_pssdDT[ secID ]->m_iSabotageID%1000 / 100 > 0 || !def )	ptbText2->Enabled = false ;
	//if( m_psdSectData->GetSecOwner( secID ) == -1 || 
	//	ROOT_SCENARIO.GetHeroInSect( secID , 0xFFFF ) != NULL )						ptbText3->Enabled = false ;

	//return true;
}

//--------------------------------------------------------------------------------------------

void	CSectorMenu::SetDiversionTexts( int def )
{
	char	sTMP[256];
	
	int  money = 0, pt = 0;
	for(int i=0; i < m_iDTN ; i++)
	{
		_str	txt;		//;{CR}

		money	= SECT_SABOTAGE[i][def];		// money
		pt		= SECT_SAB_PERCENT[i][def] ;	// percent

		if( money > ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->GetResource( GOLD  ) )  txt += "{CR}";
		
		txt += money;
		txt += "{C}";

		char  textID[256];
		sprintf( textID, "#CWM_Diversion%d" , i ); //"#CWM_Diversion0"

		sprintf( sTMP, GetTextByID( textID ) , txt.pchar() ,  pt );		// - ( 5 + 3 * 0 ) * def  ) ;
		_str	txt2;
		txt2 = " ";
		txt2 += sTMP;
		if( m_ptbDText[i] ) {
			m_ptbDText[i]->SetMessage( txt2.pchar() ); //sTMP ) ;
			if( ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->GetResource( GOLD  ) < money )  
				m_ptbDText[i]->Enabled = false;
		}
	}
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------


//**********************************************************************************
////////////////////////////////////////
///////////////// CUnit ////////////////
////////////////////////////////////////
//----------------------------------------------
CUnit::CUnit(){
	Init();
}
//----------------------------------------------
CUnit::~CUnit(){
	Init();
}
//----------------------------------------------
void	CUnit::Init(){
	m_Alive		 = true;
	m_chName	 = "NONAME";
	m_inID		 = GetLastID();
	m_inPlayerID = 0;
	m_inSectorID = m_inPreviousSecID = -1;
	m_psdMapPic  = m_psdPotret = NULL;
	m_iBlinkStat = 0;
//	strcpy( m_chName, "NONAME" );
}
//----------------------------------------------
void	CUnit::SetVisible( bool state ){
	if (m_psdMapPic!=NULL)	m_psdMapPic->Visible = state;
//	if (m_psdPotret!=NULL)	m_psdPotret->Visible = state;
}
//----------------------------------------------
void	CUnit::SetEnabled( bool state )//grey
{
	if(m_psdMapPic!=NULL)   m_psdMapPic->Enabled = state;
	//if(m_psdPotret!=NULL)	m_psdPotret->Visible = state;
}
//----------------------------------------------
void	CUnit::SetOnClickMap(pCULL_FUN_CLICK pf,int p){
	if (pf!=NULL){
		m_psdMapPic->OnUserClick	= pf;
		if (p!=-1)	{ m_psdMapPic->UserParam = p;		}
		else		{ m_psdMapPic->UserParam = m_inID;	}
	}
}
//----------------------------------------------
void	CUnit::SetName(char* name){
	if (name==NULL){
	//	strcpy(m_chName,"NONAME");
		m_chName = "NONAME";
	}
	else{
	//	strcpy(m_chName,GetTextByID(name));
		m_chName = GetTextByID( name );
	}
}
//----------------------------------------------
void	CUnit::setPlayerID(int id){
	if (id>=0 && id<=7){
		m_inPlayerID = id;
	}
	else{
		m_inPlayerID = 0;
	}
}
//----------------------------------------------
void	CUnit::SetSectorID(int id){
	if (m_inSectorID!=id) {
		m_inPreviousSecID = m_inSectorID;
	}
	m_inSectorID = id;
}
//----------------------------------------------
void	CUnit::setDeffColor()
{
	GPPicture* MapPic = (GPPicture*)m_psdMapPic;
	MapPic->Nation = ROOT_SCENARIO.m_iNationID[m_inPlayerID] ;
	//m_psdMapPic->Diffuse = 0xFF000000 + GetBigMapPlayerColor(m_inPlayerID);
}
//----------------------------------------------
int		CUnit::getSectorID(){
	return	m_inSectorID;
}
//----------------------------------------------
int		CUnit::getPreviousSecID(){
	return m_inPreviousSecID;
}
//----------------------------------------------
int		CUnit::getUniqueID(){
	return m_inID;
}
//----------------------------------------------
int		CUnit::GetPlayerID(){
	return m_inPlayerID;
}
//----------------------------------------------
char*	CUnit::GetName(){
	return	m_chName.pchar();
}
//----------------------------------------------
SimpleDialog*	CUnit::getMapPicPt(){
	//return	reinterpret_cast<int>(m_psdMapPic);
	return	m_psdMapPic;
}
//----------------------------------------------
void	CUnit::setMapPic(SimpleDialog* psd){
	if (psd!=NULL){
		m_psdMapPic = psd;
	}
}
//----------------------------------------------
void	CUnit::SetPotret(SimpleDialog* psd){
	if (psd!=NULL){
		m_psdPotret = psd;
	}
}
//----------------------------------------------
void	CUnit::setMapPicXY(int x,int y){
	m_psdMapPic->x = x;
	m_psdMapPic->y = y;
	m_psdMapPic->x1 = x+GPS.GetGPWidth ( m_psdMapPic->ShapeFileID, m_psdMapPic->ShapeSpriteID );
	m_psdMapPic->y1 = y+GPS.GetGPHeight( m_psdMapPic->ShapeFileID, m_psdMapPic->ShapeSpriteID );
}
//----------------------------------------------
void	CUnit::SetPotretXY(int x,int y){
	m_psdPotret->x = x;
	m_psdPotret->y = y;
	m_psdPotret->x1 = x+GPS.GetGPWidth ( m_psdPotret->ShapeFileID, m_psdPotret->ShapeSpriteID );
	m_psdPotret->y1 = y+GPS.GetGPHeight( m_psdPotret->ShapeFileID, m_psdPotret->ShapeSpriteID );
}
//----------------------------------------------
void	CUnit::setMapPicRR(){
	setMapPicRR(m_psdSector);
}
//----------------------------------------------
void	CUnit::setMapPicLL(){
	setMapPicLL(m_psdSector);
}
//----------------------------------------------------------------------------------

void	CUnit::setMapPicRR(SimpleDialog* psd)
{
	GPPicture* pic = (GPPicture*)m_psdMapPic;
	int dMx = pic->x1 - pic->x;
	int dMy = pic->y1 - pic->y;


	//
	pic->x = psd->x1;				pic->x1 = pic->x + dMx;
	pic->y = psd->y1 - dMy;			pic->y1 = pic->y + dMy;

	//int dx = -27 /*25*/  , dy = -3; //0 ;
	int dx = -32 , dy = 2;//4; //0 ;
	pic->x += dx;			pic->x1 += dx;
	pic->y += dy;			pic->y1 += dy; 
	//


	//pic->x = psd->x;				pic->x1 = pic->x + dMx;
	//pic->y = psd->y;				pic->y1 = pic->y + dMy;

	//int dx = 5 , dy = 36 ;
	//int dx = 20 /*5*/ , dy = 5 /*36*/ ;
	//int dx = 10 , dy = 0 ;
	//pic->x -= dx;			pic->x1 -= dx;
	//pic->y -= dy;			pic->y1 -= dy;
	////if(psd->x1-psd->x>70) { pic->x -= 33 /*30*/; } //Roma-fix
	//


	//GP->Diffuse=MulDWORD(GetNatColor(ColorID),300);
	pic->SpriteID = pic->ShapeSpriteID = 1; //Right
	if( m_inPlayerID == gi_bmSAVE_Nation && ((CHero*)this)->m_inFuel > 0 )  
		pic->SpriteID = pic->ShapeSpriteID = 2; //selected Right

	CheckBlink();

	// VIC VER
	//int dSx = GPS.GetGPWidth ( psd->ShapeFileID, psd->ShapeSpriteID );
	//int dSy = GPS.GetGPHeight( psd->ShapeFileID, psd->ShapeSpriteID );
	//int dMx = GPS.GetGPWidth ( m_psdMapPic->ShapeFileID, m_psdMapPic->ShapeSpriteID );
	//int dMy = GPS.GetGPHeight( m_psdMapPic->ShapeFileID, m_psdMapPic->ShapeSpriteID );

	//m_psdMapPic->x = psd->x + dSx /*+ 5*/;	m_psdMapPic->x1 = m_psdMapPic->x + dMx;	// VIC VER
	//m_psdMapPic->y = psd->y + dSy - dMy;		m_psdMapPic->y1 = m_psdMapPic->y + dMy;	// VIC VER
}
//----------------------------------------------

void	CUnit::setMapPicLL(SimpleDialog* psd)
{
	GPPicture* pic = (GPPicture*)m_psdMapPic;
	int dMx = pic->x1 - pic->x ;
	int dMy = pic->y1 - pic->y;


	//
	pic->x = psd->x  - dMx;			pic->x1 = pic->x + dMx;
	pic->y = psd->y1 - dMy;			pic->y1 = pic->y + dMy;

	int dx = 39 /*40*/ , dy = 2;//6; //0 ;
	pic->x += dx;			pic->x1 += dx; 	
	pic->y += dy;			pic->y1 += dy; 
	//


	//pic->x = psd->x;				pic->x1 = pic->x + dMx;
	//pic->y = psd->y;				pic->y1 = pic->y + dMy;

	//int dx = 5 , dy = 36 ;
	//int dx = 20 /*5*/ , dy = 5 /*36*/ ;
	//int dx = 10 , dy = 0 ;
	//int dx = 20 /*9*/ , dy = 5 ;
	//pic->x += dx;			pic->x1 += dx;
	//pic->y -= dy;			pic->y1 -= dy;
	//if(psd->x1-psd->x>70) { pic->x += 33 /*30*/; } //Roma-fix


	pic->SpriteID = pic->ShapeSpriteID = 0; //Left
	if( m_inPlayerID == gi_bmSAVE_Nation && ((CHero*)this)->m_inFuel > 0 )  
		pic->SpriteID = pic->ShapeSpriteID = 2; //selected Right

	CheckBlink();
}
//void	CUnit::setMapPicLL(SimpleDialog* psd)
//{
//	int dSx = GPS.GetGPWidth ( psd->ShapeFileID, psd->ShapeSpriteID );
//	int dSy = GPS.GetGPHeight( psd->ShapeFileID, psd->ShapeSpriteID );
//	int dMx = GPS.GetGPWidth ( m_psdMapPic->ShapeFileID, m_psdMapPic->ShapeSpriteID );
//	int dMy = GPS.GetGPHeight( m_psdMapPic->ShapeFileID, m_psdMapPic->ShapeSpriteID );
//
//	m_psdMapPic->x = psd->x - dMx /*- 5*/;		m_psdMapPic->x1 = m_psdMapPic->x + dMx;
//	m_psdMapPic->y = psd->y + dSy - dMy;	m_psdMapPic->y1 = m_psdMapPic->y + dMy;
//
//	//new
//	GPPicture* pic = (GPPicture*)m_psdMapPic;
//	pic->SpriteID  = pic->ShapeSpriteID = 0 ; //left
//	//new
//}
//----------------------------------------------
bool	CUnit::CheckBlink()//new
{
	//if( m_Blink ) 
	//{
	//	int BLINK_TIME_VALUE = 400;

	//	int bt = GetTickCount();
	//	if( bt > m_iBlinkTime + BLINK_TIME_VALUE ) {
	//		m_iBlinkTime = bt;
	//		m_iBlinkStat--;
	//	}

	//	if( m_iBlinkStat < 0 ) {
	//		m_iBlinkStat = 1;      //15; //20 
	//	}

	//	int delta = 2* m_iBlinkStat;

	//	m_psdMapPic->x += delta;		m_psdMapPic->y -= delta;

	//	// end it
	//	if( m_iBlinkLimit - bt < 0 || ((CHero*)this)->m_CurStatus == CHero::CAPTURE ) {
	//		m_Blink = m_iBlinkTime = m_iBlinkLimit = 0;
	//	}
	//	//if(  m_iBlinkStat == 1 || ((CHero*)this)->m_CurStatus == CHero::CAPTURE ) { 
	//	//	m_Blink = m_iBlinkTime = 0;;
	//	//}
	//}
	return  m_Blink;
}
//----------------------------------------------------------------------------------
void	CUnit::PrepareBlink()//new
{
	int BLINK_TIME_VALUE = 7000 ; //5000; //500 ;

	m_Blink		  = true;
	m_iBlinkStat  = 0;
	m_iBlinkTime  = 0;
	m_iBlinkLimit = GetTickCount() + BLINK_TIME_VALUE; //0;
}
//void	CUnit::PrepareBlink()//new
//{
//	m_Blink		 = true;
//	m_iBlinkTime = 0;
//	m_iBlinkStat = 0;
//}
//----------------------------------------------
void	CUnit::setVisibleMapPic(bool stat){
	m_psdMapPic->Visible = stat;
}
//----------------------------------------------
void	CUnit::SetSector(CSectData* pSec,int id){
	if (pSec==NULL || id==-1)	{ return; }

	//if( m_inSectorID != id /*&& m_inPreviousSecID != id*/ ) {		//new //Gr8y
		m_inPreviousSecID = m_inSectorID;
	//}		//new //Gr8y
	m_inSectorID = id;
	m_psdSector	 = pSec->GetSityPic(id);
}
//----------------------------------------------
void	CUnit::SetSector(CSectData* psdS){
	int	al = 0;
	psdS->GetPlayerSect(&al,1,m_inPlayerID);
	m_psdSector = psdS->GetSityPic(al);
	//if (m_inSectorID!=al) {		//new //Gr8y
		m_inPreviousSecID = m_inSectorID;
	//}		//new //Gr8y
	m_inSectorID = al;
}
//----------------------------------------------

void	CUnit::UpdatePic(CSectData* psdData)
{
	// Выставить координаты на карте, возможно скролили.
	if( CanSetRightHeroAlign( m_inPlayerID , m_inSectorID ) ) {	//if (psdData->GetSecOwner(m_inSectorID)==m_inPlayerID){
		setMapPicRR();
	} 
	else {
		setMapPicLL();
	}
	// Задать цвет.
	setDeffColor();
}
//void	CUnit::UpdatePic(CSectData* psdData){
//	// Выставить координаты на карте, возможно скролили.
//	if (psdData->GetSecOwner(m_inSectorID)==m_inPlayerID){
//		setMapPicRR();
//	}
//	else{
//		setMapPicLL();
//	}
//	// Задать цвет.
//	setDeffColor();
//}

//----------------------------------------------
void	CUnit::MoveWithMouse(){
	int	w = GPS.GetGPWidth ( m_psdMapPic->ShapeFileID, m_psdMapPic->ShapeSpriteID );
	int	h = GPS.GetGPHeight( m_psdMapPic->ShapeFileID, m_psdMapPic->ShapeSpriteID );
	m_psdMapPic->x	= mouseX - w/2;
	m_psdMapPic->y	= mouseY - h/2;
	m_psdMapPic->x1 = m_psdMapPic->x + w;
	m_psdMapPic->y1 = m_psdMapPic->y + h;
}
//----------------------------------------------
bool	CUnit::operator =(CUnit& other){
	if (other.m_chName.pchar()!=NULL){
		m_chName.operator =(other.m_chName.pchar());					/*strcpy(,other.m_chName);*/
	}
	else{
		m_chName.operator =("NONAME");	/*strcpy(m_chName,"NONAME");*/
	}

	if (other.m_inID>=0)			m_inID			= other.m_inID;
	else							m_inID			= -1;
	
	if (other.m_inPlayerID>=0)		m_inPlayerID	= other.m_inPlayerID; 
	else							m_inPlayerID	= -1;
	if (other.m_inSectorID>=0)		m_inSectorID	= other.m_inSectorID;
	else							m_inSectorID	= -1;
	if (other.m_inPreviousSecID>=0)	m_inPreviousSecID	= other.m_inPreviousSecID;
	else							m_inPreviousSecID	= -1;

	if (other.m_psdMapPic!=NULL)	m_psdMapPic		= other.m_psdMapPic;
	else							m_psdMapPic		= NULL;
	if (other.m_psdPotret!=NULL)	m_psdPotret		= other.m_psdPotret;
	else							m_psdPotret		= NULL;
	if (other.m_psdSector!=NULL)	m_psdSector		= other.m_psdSector;
	else							m_psdSector		= NULL;
	return true;
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
//////////////// CSquard ///////////////
////////////////////////////////////////
//----------------------------------------------
//void	CSquard::SDESCRIPTION::Init(){
//	TypeName.DString::Clear();
//	TypeName	+= "NONAME";
//	MeleeDMG	 = 0;
//	RifleDMG	 = 0;
//	//GrenadeDMG	 = 0;	//Price		 = 0;	
//	//for(int i=0; i<8; i++) Cost[i]=0;
//}
////----------------------------------------------
//void	CSquard::SDESCRIPTION::UPDATE(int NIndex)
//{
//	if( NIndex < 0 || NIndex == 0xFFFF ) return;// grey [28.12.2004]
//	//Заполним поля для текущего типа отряда.
//	Init();	
//	TypeName.DString::Clear();
//	TypeName  += NATIONS[0].Mon[NIndex]->newMons->Message;
//
//	MeleeDMG   = NATIONS[0].Mon[NIndex]->MoreCharacter->MaxDamage[0];
//	RifleDMG   = NATIONS[0].Mon[NIndex]->MoreCharacter->MaxDamage[1];
//	//GrenadeDMG = NATIONS[0].Mon[NIndex]->MoreCharacter->MaxDamage[2];
//}
//----------------------------------------------
//----------------------------------------------
CSquard::CSquard()
{
	Init();
}
//----------------------------------------------
CSquard::~CSquard()
{
	Init();
}
//----------------------------------------------
void	CSquard::Init()
{
	Kills.Clear();

	setTypeGlobID(-1);
	setSQ_MAX(0);
	setSQ_NUM(0);
	
	TypeName.DString::Clear();
	TypeName	 = "noname";
	MeleeDMG	 = 0;
	RifleDMG	 = 0;
	GrenadeDMG	 = 0;
	//DESCRIPTION.Init();
}
//----------------------------------------------
//void	CSquard::setFreeSQinArmy(int N){
//	FreeSQinArmy = N;
//}
//----------------------------------------------
void	CSquard::setTypeGlobID(int id) {
	if (-1<=id&&id<0xFFFF) {
		TypeGlobID = id;

		//new
		if( id < 0 || id == 0xFFFF )  return;// grey [28.12.2004]
		//Заполним поля для текущего типа отряда.
		TypeName.DString::Clear();
		TypeName   = NATIONS[0].Mon[id]->newMons->Message;
		MeleeDMG   = NATIONS[0].Mon[id]->MoreCharacter->MaxDamage[0];
		RifleDMG   = NATIONS[0].Mon[id]->MoreCharacter->MaxDamage[1];
		GrenadeDMG = NATIONS[0].Mon[id]->MoreCharacter->MaxDamage[2];
		//DESCRIPTION.UPDATE(TypeGlobID);
		//new
	}
}
//----------------------------------------------
void	CSquard::setSQ_MAX(int n){
	if (0<=n&&n<0xFFFF) {
		SQ_MAX = n;
	}
}
//----------------------------------------------
void	CSquard::setSQ_NUM(int n){
	if (0<=n&&n<0xFFFF) {
		SQ_NUM = n;
	}
}
//----------------------------------------------
int		CSquard::getTypeGlobID(){
	return	TypeGlobID;
}
//----------------------------------------------
int		CSquard::getSQ_MAX(){
	return	SQ_MAX;
}
//----------------------------------------------
int		CSquard::getSQ_NUM(){
	return	SQ_NUM;
}
//----------------------------------------------
//bool	CSquard::addSQ(){
//	if (AvailableReinforse>0&&FreeSQinArmy>0&&SQ_MAX>SQ_NUM) {
//		SQ_NUM++;
//		return true;
//	}
//	return false;
//}
bool	CSquard::addSQ(int freeSQNum){
	if (/*AvailableReinforse>0 &&*/ freeSQNum>0 && SQ_MAX>SQ_NUM ) {
		SQ_NUM++;
		return true;
	}
	return false;
}

//----------------------------------------------
//void	CSquard::DecreaseARMY(bool st){
//	if(st) 
//	{
//		FreeSQinArmy--;
//		AvailableReinforse--;
//	}
//}
//----------------------------------------------
//void	CSquard::DecreaseARMY(int n) 
//{
//	if(n > 0) {
//		FreeSQinArmy -= n ;
//		AvailableReinforse -= n ;
//	}
//}
////----------------------------------------------
//void	CSquard::IncreaseARMY(int n) 
//{
//	if(n > 0) {
//		FreeSQinArmy += n ;
//		AvailableReinforse += n ;
//	}
//}
//----------------------------------------------
void	CSquard::UPDATE()
{
	//Обновить все данные.
	//new
	//DESCRIPTION.UPDATE(TypeGlobID);
	//new
}
//----------------------------------------------
CSquard	CSquard::operator=(CSquard& sq){
	//AvailableReinforse	=	sq.AvailableReinforse;
	//FreeSQinArmy		=	sq.FreeSQinArmy;
	TypeGlobID			=	sq.TypeGlobID;	
	SQ_MAX				=	sq.SQ_MAX;		
	SQ_NUM				=	sq.SQ_NUM;		

	UPDATE();

	return *this;
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
///////////////// CHero ////////////////
////////////////////////////////////////
//----------------------------------------------
CHero::CHero() 
{
	Init();
}
//----------------------------------------------
CHero::~CHero() 
{
	Init();
}
//----------------------------------------------
void	CHero::Init() {
	CUnit::Init();
	m_CurStatus		= FREE;
    bigPotret		= NULL;
	m_inHeroID		= 0;
	m_inRankID		= 0;
	m_inStatID		= 0;
	m_inFuel		= 1;
	m_inExperience	= 0;
	m_iFreeSQNum	= 0;
	m_iBatlN	= m_iVictN = 0;
	m_iFormLost = m_iFormKilled = 0;
	//new
	ARMY.Clear();
}
//----------------------------------------------
void	CHero::SetOnClickMap( pCULL_FUN_CLICK pf, int p ) 
{
	if ( pf!=NULL ) {
		m_psdMapPic->OnUserClick	= pf;
		if ( p!=0 ) {
			m_psdMapPic->UserParam	= p;
		}
		m_psdMapPic->UserParam		= reinterpret_cast<int>(this);		
	}
}
//----------------------------------------------
void	CHero::SetOnRClickMap( pCULL_FUN_CLICK pf, int p ){
	if ( pf!=NULL ) {
		m_psdMapPic->OnUserRightClick	= pf;
		if ( p!=0 ) {
			m_psdMapPic->UserParam		= p;
		}
		m_psdMapPic->UserParam			= reinterpret_cast<int>(this);
	}
}
//----------------------------------------------
void	CHero::SetHeroID( int id ) {
	m_inHeroID = id;
}
//----------------------------------------------
void	CHero::SetBigPotret( SimpleDialog* pPic ) {
	if ( pPic!=NULL )
		bigPotret = pPic;
}
//----------------------------------------------
void	CHero::SetVisibleMiniMenu(bool status) {
	m_psdPotret->Visible = status;
}
//----------------------------------------------
void	CHero::ReEnableHero()//Bключить все клики на герое, + все сделать видимым!!!
{
	m_Alive = true;

	m_psdMapPic->Visible = true;
	m_psdPotret->Visible = true;
	m_psdMapPic->Enabled = true;
	m_psdPotret->Enabled = true;

	bigPotret->Visible = true;
	bigPotret->Enabled = true;
}
//----------------------------------------------
void	CHero::DisableHero()//Отключить все клики на герое, + все сделать не видимым.
{
	m_Alive = false;

	m_psdMapPic->Visible = false;
	m_psdPotret->Visible = false;
	m_psdMapPic->Enabled = false;
	m_psdPotret->Enabled = false;

	bigPotret->Visible = false;
	bigPotret->Enabled = false;
}
//----------------------------------------------
void	CHero::SetARMY()
{
	const int MaxSQN = 100000;

	int  nkls = 0;
	int  NKills[MaxSQN];

	//int*  pNSQ	= NULL; 
	int  NSQ[MaxSQN];				

	int NT = ARMY.GetAmountOfElements();
	if( NT > 0 ) {
		//pNSQ = new int[NT];
		for(int i=0; i < NT ; i++) 
		{
			//pNSQ[i] = (ARMY[i])->SQ_NUM;
			NSQ[i] = (ARMY[i])->SQ_NUM;
			for(int k=0; k < NSQ[i] ; k++) {  /*k < pNSQ[i]*/
				NKills[nkls] = 0;
				if( ARMY[ i ]->Kills.GetAmountOfElements() > k )  NKills[nkls] = ARMY[i]->Kills[k];
				nkls++;
			}
		}
	}
	//Полностью создать новую армию.
	CSquard*	pSQ = NULL;
	ARMY.Clear();
	int  kk = 0;
	int  newNT = pArmyLimit->GetMaxDiffSqd(m_inRankID,m_inPlayerID);
	for(int i=0; i < newNT; i++) {
		pSQ = new CSquard();
		pSQ->setTypeGlobID	( pArmyLimit->GetNSqdNID(m_inRankID,m_inPlayerID,i) );
		pSQ->setSQ_MAX		( pArmyLimit->GetNSqdNum(m_inRankID,m_inPlayerID,i) );
		pSQ->setSQ_NUM		(0);
		pSQ->UPDATE();
		ARMY.Add(pSQ);
		pSQ = NULL;
	}
	GetARMY_FreeNSQ();

	int  NK = 0;
	//if( pNSQ!=NULL ) {
	if( NT > 0 ) {
		for(int i=0; (i<NT)&&(i<newNT) ; i++) 
		{
			if( ARMY[i]->getSQ_MAX() > 0 && NSQ[i] > 0 ) // pNSQ[i] > 0 )
			{
				for(int j=0; j < NSQ[i] /*pNSQ[i]*/ ; j++)  AddSQbyArrID(i);

				for(int k= NK ; k < /*pNSQ[i]*/ NSQ[i] + NK && k < nkls ; k++)   
					ARMY[ i ]->Kills.Add( NKills[ k ] );
				NK += NSQ[i]; //pNSQ[i];
			}
		}
	}

	//if (pNSQ)	delete[]pNSQ;
}
//----------------------------------------------
void	CHero::SetDefaultARMY(){ 
	for (int i=0; i<ARMY.GetAmountOfElements(); i++){
		for (int j=0; j<ARMY[i]->getSQ_MAX(); j++)	AddSQbyArrID(i);
	}
}
//----------------------------------------------
void	CHero::AddSQbyArrID(int id)
{
	if ( 0<=id && id<ARMY.GetAmountOfElements() ) 
	{				
		bool	success = ARMY[id]->addSQ( GetARMY_FreeNSQ() );
		DecreaseARMY(success);
	}
}

//----------------------------------------------
void	CHero::DelSQbyArrID(int id)
{
	if ( 0<=id && id<ARMY.GetAmountOfElements() ) 
	{				
		ARMY[id]->setSQ_NUM( ARMY[id]->getSQ_NUM() - 1 ) ;	
	}
}
//----------------------------------------------
bool	CHero::DecreaseARMY(bool st) // !!! НЕ НАДО??? !!!
{
	if(st) m_iFreeSQNum--;
	return  st;
}
bool	CHero::DecreaseARMY(int n) // !!! НЕ НАДО??? !!!
{
	if(m_iFreeSQNum-n>=0) {
		m_iFreeSQNum -= n ;
		return true;
	}
	m_iFreeSQNum = 0 ;
	return  false;
}
//----------------------------------------------
bool	CHero::IncreaseARMY(int n)
{
	m_iFreeSQNum += n ;
	return  true;
}
//----------------------------------------------
int		CHero::GetARMY_NSQ(){
	int rN = 0;
	for (int i=0; i<ARMY.GetAmountOfElements(); i++){
		rN += ARMY[i]->getSQ_NUM();
	}
	return rN;
}
//----------------------------------------------
int		CHero::GetBranch_NSQ( int branch )
{
	int rN = 0;
	for(int i=0; i < ARMY.GetAmountOfElements() ; i++)
		if( GetObjectBranch( ARMY[i]->getTypeGlobID() ) == branch ) rN += ARMY[i]->getSQ_NUM();
	return rN;
}
//----------------------------------------------
int		CHero::GetARMY_FreeNSQ()
{
	m_iFreeSQNum = pArmyLimit->GetMaxSqd( m_inRankID , m_inPlayerID ) - GetARMY_NSQ() ;
	return  m_iFreeSQNum;
}
//----------------------------------------------
int		CHero::GetARMY_Power( bool realPower )
{
	int  mypower = GetRank() * 11 ;

	if( !realPower && rand()%10 == 0 ) return ( mypower + 1 ) * 50 /* *100*/; // CRAZY RULE

	int  ABSW[3] = { 10, 6, 3 }; // сила отрядов (по brench: 0 - пехота, 1 - конница, 2 - пушки)
	for(int br=0; br < 3 ; br++) {
		int n = 0;
		for(int sq=0; sq < ARMY.GetAmountOfElements() ; sq++)
			if( GetObjectBranch( ARMY[sq]->getTypeGlobID() ) == br ) {
				n++;
				mypower += ( ABSW[br] + n/2 ) * ARMY[sq]->getSQ_NUM();
				
				if( !realPower && ROOT_SCENARIO.m_pPlayer[m_inPlayerID] && 
					 ROOT_SCENARIO.m_pPlayer[m_inPlayerID]->GetAggressive() >= 100 )  mypower += 2*n;
			}
	}
	return mypower;
}
//----------------------------------------------
void	CHero::SetRank(int rID){
	if (0<=rID&&rID<MAX_RANKS) {
		m_inRankID = rID;
		SetARMY();
	}
}
//----------------------------------------------
void	CHero::RaiseRank() {
	SetRank(m_inRankID+1);
}
//----------------------------------------------
void	CHero::LowerRank(){
	SetRank(m_inRankID-1);
}
//----------------------------------------------
bool	CHero::KillOneSQUARD()
{
	bool  KILL = false;
	if( GetARMY_NSQ() > 0 ) {
		for(int i=0; i < ARMY.GetAmountOfElements() && !KILL ; i++)			// убиваем сначала "слабые" отряды
			if( ARMY[i]->getSQ_NUM() > 0 && rand()%100 < 50 ) 
			{
				if( GetSquadAmountByType(m_inPlayerID, ARMY[i]->getTypeGlobID()) > 20 && // if(!cannons)
					ARMY[i]->Kills.GetAmountOfElements() > 0 &&
					ARMY[i]->getSQ_NUM() <= ARMY[i]->Kills.GetAmountOfElements() && 
					rand()%100 < ARMY[i]->Kills[ ARMY[i]->getSQ_NUM()-1 ] ) { 
						continue; 
					}
				ARMY[i]->killSQ();
				KILL = true;
			}
		for(int i=ARMY.GetAmountOfElements()-1 ; i >= 0 && !KILL ; i--)		// убить один из "сильных" отрядов
			if( ARMY[i]->getSQ_NUM() > 0 && rand()%100 < 50 ) 
			{
				if( ARMY[i]->Kills.GetAmountOfElements() > 0 &&
					ARMY[i]->getSQ_NUM() <= ARMY[i]->Kills.GetAmountOfElements() && 
					rand()%100 < ARMY[i]->Kills[ ARMY[i]->getSQ_NUM()-1 ] ) { 
						continue; 
					}
				ARMY[i]->killSQ();
				KILL = true;
			}
		for(int i=0; i < ARMY.GetAmountOfElements() && !KILL ; i++)			// убить один "слабый" отряд
			if( ARMY[i]->getSQ_NUM() > 0 ) 
			{					
				int PROBTY = rand()%100;
				if( ARMY[i]->Kills.GetAmountOfElements() > 0 && 
					ARMY[i]->getSQ_NUM() <= ARMY[i]->Kills.GetAmountOfElements() && 
					ARMY[i]->Kills[ ARMY[i]->getSQ_NUM()-1  ] > 55  &&  PROBTY < 85 ) {	//90 ) {
						return false; 
					} 
				ARMY[i]->killSQ();
				KILL = true;
			}
	}
	//new
	if( KILL )  AddFormLost();
	//new
	return	KILL;
}
//----------------------------------------------

int		CHero::GetFormLost()//new
{
	return m_iFormLost;
}
//----------------------------------------------
int		CHero::GetFormKilled()//new
{
	return m_iFormKilled;
}
//----------------------------------------------
int		CHero::AddFormLost( int nn /* = 1*/)//new 
{
	if( nn > 0 ) {
		m_iFormLost += nn ;
	}
	return  m_iFormLost ;
}
//----------------------------------------------
int		CHero::AddFormKilled( int nn /* = 1*/)//new 
{
	if( nn > 0 ) {
		m_iFormKilled += nn ;
	}
	return  m_iFormKilled ;
}

//----------------------------------------------
void	CHero::SetSquardNumByType(int n, int type)
{
	if( n < 0 )			{ return; }

	for(int i=0; i < ARMY.GetAmountOfElements() ; i++)
		if( ARMY[i]->getTypeGlobID() == type ) {
			if( n > ARMY[i]->getSQ_MAX() )   { n = ARMY[i]->getSQ_MAX(); }
			ARMY[i]->setSQ_NUM( n );		
			return;
		}
}
//----------------------------------------------
void	CHero::SetNKills( LinearArray<int,_int>& NKills , int type )
{
	for(int i=0; i < ARMY.GetAmountOfElements() ; i++)
		if( ARMY[i]->getTypeGlobID() == type ) 
		{
			ARMY[i]->Kills.Clear();
			int Num = NKills.GetAmountOfElements() ;
			// sorting
			for(int n=0; n < Num-1 ; n++) {
				int max = n;
				for(int k=n+1; k < Num ; k++)   
					if( NKills[max] < NKills[k] ) max = k ;
				if( max != n )   for(int m=max; m > n ; m--) NKills.MoveElement(m, -1 ); 
			}
			// set squads NKills
			for(int k=0; k < Num ; k++) ARMY[i]->Kills.Add( NKills[k] );
			return;
		}
}
//----------------------------------------------
void	CHero::SetBattleNKills()
{
	LinearArray<int,_int>	Kills;
	int    diff				= pSAVE->m_iDifficulty;
	const  int FRAG_LIMIT	= gi_bmMISS_FRAG_LIMIT; 

	for(int sq=0; sq < ARMY.GetAmountOfElements() ; sq++) 
	{
		Kills.Clear();

		for(int n=0; n < ARMY[ sq ]->getSQ_NUM() ; n++) 
		{
			int  frags =  rand()%100  + sq + n +2 + 10*diff  +1 ; // new frags	
			int  kills =  frags * ( rand()%3 + 1 ) ; 

			if( diff < 2 && GetPlayerID() != gi_bmSAVE_Nation )	kills = kills / ( DIFF_LEVELS_N - diff ) +1;

			if( ARMY[ sq ]->Kills.GetAmountOfElements() > n )	kills += ARMY[ sq ]->Kills[ n ] ; // + old frags
			if( kills > FRAG_LIMIT )  kills = FRAG_LIMIT ;
			Kills.Add( kills ) ; 
		}

		SetNKills( Kills , ARMY[ sq ]->getTypeGlobID() );
	}
}
//----------------------------------------------
//void	CHero::SetSquardNumByType(int n, int type)
//{
//	if(n<0)		{ return; }
//
//	for(int i=0; i<ARMY.GetAmountOfElements() ; i++)
//		if( ARMY[i]->getTypeGlobID() == type ) 
//		{
//			//if( n > ARMY[i]->getSQ_NUM() )   { n = ARMY[i]->getSQ_NUM(); }
//			if( n > ARMY[i]->getSQ_MAX() )   { n = ARMY[i]->getSQ_MAX(); }
//
//			IncreaseARMY( ARMY[i]->getSQ_NUM() );			
//			ARMY[i]->setSQ_NUM( n );		
//			DecreaseARMY( n );
//
//			return;
//			//int sq_max = ARMY[i]->getSQ_MAX();		
//			//if(n <= sq_max) {
//			//	ARMY[i]->setSQ_NUM( n ); 
//			//	DecreaseARMY( n );			
//			//else {
//			//	ARMY[i]->setSQ_NUM( sq_max ); 
//			//	DecreaseARMY( sq_max );			
//			//}
//			//if(n == 0 && i == 0) { ARMY[i]->setSQ_NUM( 1 ); }		DecreaseARMY(bool st)
//			//return;
//		}
//}
////----------------------------------------------
void	CHero::SetHeroDefeatArmy()
{
	bool  have_rest = false;
	for(int i=0; i < ARMY.GetAmountOfElements() ; i++)
	{
		ARMY[ i ]->Kills.Clear();
		if( !have_rest && ARMY[i]->getSQ_NUM() > 0 ) { 
			have_rest = true;
			IncreaseARMY( ARMY[i]->getSQ_NUM() - 1 );
			ARMY[i]->setSQ_NUM( 1 ); 
		}
		else {
			IncreaseARMY( ARMY[i]->getSQ_NUM() );
			ARMY[i]->setSQ_NUM( 0 );   
		}
	}
	if( GetARMY_NSQ() == 0 && ARMY.GetAmountOfElements() > 0 ) { ARMY[0]->setSQ_NUM( 1 ); DecreaseARMY( 1 ); }
}
//----------------------------------------------
void	CHero::SetFuel(int Dist){
	if (Dist>=0) {
		m_inFuel = Dist;
	}
}
//----------------------------------------------
bool	CHero::AddFuel(int Dist){
	if (abs(Dist)>m_inFuel) {
		return false;
	}else{
		m_inFuel += Dist;
	}
	return true;
}
//----------------------------------------------
void	CHero::AddBattle() 
{
	m_iBatlN++ ;
}
//----------------------------------------------
void	CHero::AddVictory()
{
	m_iVictN++ ;
}
//----------------------------------------------
void	CHero::AddExperience( int EXP ) 
{
	if( EXP > 0 )  m_inExperience += EXP;
	if( m_inExperience < 0 )		m_inExperience = 0;
	if( m_inExperience > 500000 )	m_inExperience = 500000;
}
//----------------------------------------------
int		CHero::GetExperience()
{
	return	m_inExperience;
}
//----------------------------------------------
void	CHero::UpdateRankWithExperience()
{
	if( m_inExperience < 0 )  return;

	if( m_inExperience       < LTN_EXPS_LIMIT )	{	 
		SetRank(0);
	}else if( m_inExperience < CPT_EXPS_LIMIT )	{ 
		SetRank(1);
	}else if( m_inExperience < MJR_EXPS_LIMIT )	{ 
		SetRank(2);
	}else if( m_inExperience < COL_EXPS_LIMIT )	{ 
		SetRank(3);
	}else if( m_inExperience < GEN_EXPS_LIMIT )	{ 
		SetRank(4);
	}else /*if( m_inExperience  > MAR_EXPS_LIMIT )*/	{ 
		SetRank(5);
	}
	CreateHint();
}
//----------------------------------------------
int		CHero::GetRewardsStatus( int addStat )
{
	if( m_inStatID < MAX_H_STATUS ) 
	{ 
		int RID = m_inRankID;
		if( RID == 0 )  RID = 1;
		// формула пересчета нового статуса героя
		float  newStatId = ( (float) m_iVictN / RID )  +   //very new!!!   //last add!!!
						   ( (float) m_inExperience / ( ( 7*m_inRankID + RID ) * LTN_EXPS_LIMIT/2 ) ) ; // 5*m_inRankID
		// условия первой награды (выдать награду обязательно)
		if( m_inStatID < 1 && ( m_iVictN == 1 || m_inExperience >= LTN_EXPS_LIMIT/2 ) )
			newStatId++;

		if( m_inStatID < newStatId || addStat > 0 ) { 
			m_inStatID++;   
			m_inExperience += LTN_EXPS_LIMIT;
			UpdateRankWithExperience();
		}	
	}
	return  m_inStatID;
	//int  newStatId = m_iVictN/( m_inRankID + 1 ) + m_inExperience/( ( 5*m_inRankID + 1  ) * 1000 ) ;
	//if( m_inStatID > MAX_H_STATUS )    { m_inStatID = MAX_H_STATUS ; }
}
//----------------------------------------------
int		CHero::GetHeroID() {
	return	m_inHeroID;
}
//----------------------------------------------
int		CHero::GetRank() {
	return m_inRankID;
}
//----------------------------------------------
int		CHero::GetStat() {
	return  m_inStatID;
}
//----------------------------------------------
int		CHero::GetFuel() {
	return	m_inFuel;
}
//----------------------------------------------
SimpleDialog*	CHero::GetBigPortret() {
	return	bigPotret;
}
//----------------------------------------------
void	CHero::UpdatePic( CSectData* psdData ) {
	CUnit::UpdatePic( psdData );
	if ( m_CurStatus == CAPTURE )	{
		MoveWithMouse();
	}
}
//----------------------------------------------
void	CHero::CreateMiniMenu()
{
	const	int		x0 = 709 - m_psdPotret->x;
	const	int		y0 = 546 - m_psdPotret->y;	

	m_psdPotret->x  += x0;
	m_psdPotret->y  += y0;
	m_psdPotret->x1 += x0;
	m_psdPotret->y1 += y0;
}
//----------------------------------------------
void	CHero::CreateEmptyHint()
{
	_str	a_Hint;
	a_Hint = "";
	DYNHINT_RA(m_psdMapPic,a_Hint.pchar());
}
//----------------------------------------------
void	CHero::CreateHint()
{
	if( m_CurStatus == CAPTURE )	{ CreateEmptyHint(); return; }

	_str  a_Hint;
	a_Hint  = "{FM}{CR}" ; 					a_Hint += m_chName.pchar() ;	//a_Hint += "{C}" ;
	a_Hint += "{C FF502515}, " ;			a_Hint += GetRankByID( m_inRankID ) ;
	a_Hint += " (" ;						a_Hint += GetContByID( m_inPlayerID ) ;

	a_Hint += "){FS}{C FF502515} \\ ";	a_Hint += GetTextByID( "#Victories" );	//a_Hint += "){FS}{C FF502515} \\  \\ ";
	a_Hint += "{C FF640100} " ;				a_Hint += m_iVictN;
	a_Hint += "{C},  ";						a_Hint += GetTextByID( "#Battles" );
	a_Hint += "{C FF640100} " ;				a_Hint += m_iBatlN;
	a_Hint += "{C} \\ ";					a_Hint += GetTextByID( "#Experience" );
	a_Hint += "{C FF640100} " ;				a_Hint += m_inExperience;
	a_Hint += "{C} \\ " ;					a_Hint += GetTextByID( "#SquadsNum" );	
	a_Hint += "{C FF640100} " ;				a_Hint += GetARMY_NSQ() ;

	if( m_inPlayerID == gi_bmSAVE_Nation ) {
		a_Hint += " \\ " ;		//		a_Hint += " \\  \\ " ;			
		if( GetFuel() <= 0 )	{ a_Hint += GetTextByID( "#CHINT_HeroMove0" ); }
		else					{ a_Hint += GetTextByID( "#CHINT_HeroMove1" ); }
	}
	a_Hint += "{C FF502515} \\ " ; //a_Hint += "{C FF502515} \\  \\ " ;
	a_Hint += GetTextByID( "#CWM_Left_Click" );
	DYNHINT_RA( m_psdMapPic , a_Hint.pchar() );
}
//----------------------------------------------
bool	CHero::operator =( CHero& other ) {
	CUnit::operator =( other );
	
	pArmyLimit		= other.pArmyLimit;

	m_inRankID		= other.m_inRankID;
	m_inStatID		= other.m_inStatID;
	m_inFuel		= other.m_inFuel;
	m_inHeroID		= other.m_inHeroID;
	m_inExperience	= other.m_inExperience;
	bigPotret		= other.bigPotret;
    
	//АРМИЯ.
	ARMY.Clear();
	CSquard* pSQ = NULL;
	for (int i=0; i<other.ARMY.GetAmountOfElements(); i++){
		pSQ = new CSquard();
		pSQ->operator=(*(other.ARMY[i]));
		ARMY.Add(pSQ);
		pSQ=NULL;
	}

	return true;
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
//////////////// CPlayer ///////////////
////////////////////////////////////////
//----------------------------------------------
CPlayer::CPlayer(){
	Init();
};
//----------------------------------------------
CPlayer::CPlayer(int ID){
	Init();
	if( !SetPlayerID(ID) ) {
		ShowError();
	}
}
//----------------------------------------------
CPlayer::CPlayer(int ID, char* Name){
	Init();
	if( !SetPlayerID(ID) ) {
		ShowError();
	}
	if( !SetPlayerName(Name) ) {
		ShowError();
	}
}
//----------------------------------------------
CPlayer::~CPlayer()
{
	DeleteHeroes();
	DeleteSquard();
	Init();
}
//----------------------------------------------
void	CPlayer::Init()
{
	m_PlayerID		= 0;
	m_Alive			= true;
	m_chPlayerName	= "NONAME";

	m_OneHero.Init();
	m_phrHero		= &m_OneHero;
	m_inHeroNum		= 0;

	//m_psdSquard	= NULL;
	//m_inSquardNum	= 0;
	//m_phrHero		= NULL;

	for(int i=0; i < MAX_HEROES ; i++)  m_pgppPortH[i] = NULL;
	
	m_Aggressive = 0; // парам.агрессивности страны!			//m_Aggressive = 7 ; 
	SetAggressiveDefault();
}
//----------------------------------------------
int		CPlayer::GetAggressive() 
{
	return  m_Aggressive; // парам.агрессивности страны!
}
//----------------------------------------------
int		CPlayer::SetAggressive( int value ) 
{
	int  newVal = value;
	if( value < 0 )  newVal = 0;	// min  Aggressive  value
	if( value > 9 )  newVal = 9;	// max  Aggressive  value
	return  m_Aggressive = newVal;  // парам.агрессивности страны!
}
//----------------------------------------------
int		CPlayer::GetAggressiveDefault()
{
	int  agr = 7;
	switch( m_PlayerID ) {
		case 0:		agr = 7 ;		break;
		case 1:		agr = 8 ;		break;
		case 2:		agr = 8 ;		break;//agr = 6 ;		break;
		case 3:		agr = 4 ;		break;
		case 4:		agr = 8 ;		break;
		case 5:		agr = 4 ;		break;
	}
	return  agr;
}
//----------------------------------------------
int		CPlayer::SetAggressiveDefault()
{
	return  m_Aggressive = GetAggressiveDefault(); // парам.агрессивности страны!
}
//----------------------------------------------
bool	CPlayer::LoadSquard(){
	return true;
}
//----------------------------------------------
bool	CPlayer::SaveSquard(){
	return true;
}
//----------------------------------------------
bool	CPlayer::LoadHero(){
	return true;
}
//----------------------------------------------
bool	CPlayer::SaveHero(){
	return true;
}
//----------------------------------------------
bool	CPlayer::InitMemNHero(int n){
	//if (n<=0||n>100){
	//	SendError("ILL-DEFINED N FOR HERO");
	//}
	//if (m_phrHero!=NULL){
	//	SendError("CAN NOT INIT MEM FOR HERO, IT'S ALREDY PRESENT");
	//}
	//if (!IsError()){
	//	m_phrHero = new CHero[n];
	//	m_inHeroNum = n;
	//	return true;
	//}

	return false;
}
//----------------------------------------------
bool	CPlayer::InitMemNSquard(int n){
	//if (n<=0||n>100){
	//	SendError("ILL-DEFINED N FOR SQUARD");
	//}
	//if (m_psdSquard!=NULL){
	//	SendError("CAN NOT INIT MEM FOR SQUARD, IT'S ALREDY PRESENT");
	//}
	//if (!IsError()){
	//	m_psdSquard = new CSquard[n];
	//	m_inSquardNum = n;
	//	return true;
	//}

	return false;
}
//----------------------------------------------
void	CPlayer::DeleteHeroes() {
	//if( m_phrHero!=NULL ) {
	//	delete[]m_phrHero;
	//	m_phrHero = NULL;
	//}
	//m_inHeroNum = 0;
}
//----------------------------------------------
void	CPlayer::DeleteSquard() {
	//if( m_psdSquard!=NULL ) {
	//	delete[]m_psdSquard;
	//	m_psdSquard = NULL;
	//}
	//m_inSquardNum = 0;
}
//----------------------------------------------
bool	CPlayer::IsHeroIDPresent(int id) {
	bool	res = false;
	for (int i=0; (i<m_inHeroNum)&&(!res); i++){
		if (m_phrHero[i].GetHeroID()==id)	res=true;
	}	
	return	res;
}
//----------------------------------------------
bool	CPlayer::Load(){
	return true;
}
//----------------------------------------------
bool	CPlayer::Save(){
	return true;
}
//----------------------------------------------
bool	CPlayer::UpdateSectID(){
	for (int i=0; i<m_inHeroNum; i++){
		m_phrHero[i].SetSector(m_psdSectData,-1);
	}
	return true;
}
//----------------------------------------------
bool	CPlayer::UpdatePic(){
	for (int i=0; i<m_inHeroNum; i++){
		m_phrHero[i].UpdatePic(m_psdSectData);
	}
	return true;
}
//----------------------------------------------
void	CPlayer::SetSetcData(CSectData* psd){
	if (psd!=NULL){
		m_psdSectData = psd;
	}
}
//----------------------------------------------
void	CPlayer::AddPortH(){
	//int x  = 76 ,	y  = 168;
	//int Lx = 248,	Ly = 491;
	int x  = 700 ,	y  = 140 ;
	int Lx = 248,	Ly = 530 ;
	//DialogsDesk* pDD = DataForAddHero.pMenu->AddDialogsDesk(x,y,Lx,Ly,"EmptyBorder");
	m_pDD = DataForAddHero.pMenu->AddDialogsDesk(x,y,Lx,Ly,"EmptyBorder");//DM
	
	LocalGP*	pLGP = DataForAddHero.pPORTRAIT->getLGPSmallP(m_PlayerID);;
	for (int i=0; i<MAX_HEROES; i++){
		m_pgppPortH[i] = m_pDD->addGPPicture(NULL,0,0,pLGP->GPID,i);
		//m_pgppPortH[i] = pDD->addGPPicture(NULL,0,0,pLGP->GPID,i);
		m_pgppPortH[i]->ShapeFileID	= pLGP->GPID;
		m_pgppPortH[i]->ShapeSpriteID = i;
	}
	SetPortHVisible( false );
}//Вставить картинки героев в меню со скролером.
//----------------------------------------------
void	CPlayer::SetPortHVisible(bool state)
{
	m_pDD->Visible = false;//DM
	m_pDD->Enabled = false;//DM
	for (int i=0; i<MAX_HEROES; i++){
		if (m_pgppPortH[i]!=NULL) {
			m_pgppPortH[i]->Visible = state;
			m_pgppPortH[i]->Enabled = state;
		}
	}
}//Выставить видимость для всех.
//----------------------------------------------
void	CPlayer::ShowPortH()
{
	m_pDD->Visible = false;//DM
	m_pDD->Enabled = false;//DM
	//PREPARE
	int H = GPS.GetGPHeight( m_pgppPortH[0]->ShapeFileID, m_pgppPortH[0]->ShapeSpriteID );
	GPPicture* pGPP = NULL;
	for (int i=0; i<m_inHeroNum; i++) { //		if( m_phrHero[i].Alive ) { //!!!by grey!!!
		pGPP = m_pgppPortH[ m_phrHero[i].GetHeroID() ];
		setCoordSD( pGPP, 10, 10+i*(H+15) );
		pGPP->UserParam		= reinterpret_cast<int>(&m_phrHero[i]);
		pGPP->OnUserClick	= DataForAddHero.pfHeroRClick;
		pGPP->Visible = true;
		pGPP->Enabled = true;
	}
}//Отобразить только существующих героев.
//----------------------------------------------
bool	CPlayer::IsUnitInSect(int sid){
	bool	result = false;
	for (int i=0; (i<m_inHeroNum)&&(!result); i++)
		if (m_phrHero[i].getSectorID()==sid)	result = true;
	return result;
}
//----------------------------------------------
bool	CPlayer::SetPlayerID(int ID)
{
	if( ID >= 0 ) //if (ID>=0 && ID<8)
	{
		m_PlayerID = ID;
		SetAggressiveDefault();
		return true;
	}
	SendError("ILL-DEFINED PLAYER ID");
	return false;
}
//----------------------------------------------
bool	CPlayer::SetPlayerName(char* Name){
	if (Name!=NULL){
		m_chPlayerName = Name;
		return true;
	}
	SendError("ILL-DEFINED PLAYER NAME");
	return false;
}
//----------------------------------------------
void	CPlayer::SetVisible(bool state)//grey
{
	for (int i=0; i < m_inHeroNum ; i++) {
		m_phrHero[i].SetVisible( m_phrHero[i].m_Alive && state );//if( m_phrHero[i].m_Alive )   
	}
}
//void	CPlayer::SetVisible(bool state)//grey
//{
//	for (int i=0; i < m_inHeroNum ; i++) {
//		if(m_phrHero[i].m_Alive)   m_phrHero[i].SetVisible( state );
//	}
//}
//----------------------------------------------
//void	CPlayer::SetVisible(bool state){
//	for (int i=0; i<m_inHeroNum; i++){
//		m_phrHero[i].SetVisible(state);
//	}
//}
//----------------------------------------------
void	CPlayer::SetEnabled(bool state)//grey
{
	for (int i=0; i < m_inHeroNum ; i++) {
		m_phrHero[i].SetEnabled( m_phrHero[i].m_Alive && state );//if( m_phrHero[i].m_Alive )
		//
		//m_phrHero[i].CreateEmptyHint();
		//
	}
}
//void	CPlayer::SetEnabled(bool state)//grey
//{
//	for (int i=0; i < m_inHeroNum ; i++) {
//		if( m_phrHero[i].m_Alive )  m_phrHero[i].SetEnabled( state );   
//	}
//}
//void	CPlayer::SetEnabled(bool state){
//	for (int i=0; i < m_inHeroNum ; i++){
//		m_phrHero[i].SetEnabled( state );
//	}
//}
//----------------------------------------------
bool	CPlayer::AddMoney(int m){
	//if (m>m_inMoney)	return false;
	//m_inMoney += m;		
	AddResource(GOLD, m);
	return true;
}
//----------------------------------------------
char*	CPlayer::GetPlayerName(){
	return	m_chPlayerName.pchar();
}
//----------------------------------------------
int		CPlayer::GetPlayerMoney()
{
	return GetResource(GOLD);			
}
//----------------------------------------------
int		CPlayer::GetResource(int res)
{
	if( res == RECRT )  SetRecruit();
	return Resource.GetResource(res);
}
//----------------------------------------------
int		CPlayer::SetResource(int res, int amount)
{
	return Resource.SetResource(res, amount);
}
//----------------------------------------------
int		CPlayer::AddResource(int res, int amount)
{
	return Resource.AddResource(res, amount);
}
//----------------------------------------------
bool	CPlayer::AddResources( int wood, int food, int stone, int gold, int iron, int coal ) 
{ 
	SetRecruit();
	AddResource( WOOD , wood );		AddResource( FOOD , food );			AddResource( STONE, stone);
	AddResource( GOLD , gold );		AddResource( IRON , iron );			AddResource( COAL , coal );
	return true; 
}
//----------------------------------------------

bool	CPlayer::BuyResource( int res )
{
	int BUY_AMOUNT    = 3000 ; // 4000 ;		//1000 ;
	int MIN_RES_LIMIT = 5000 ;
	int sell[6] = { WOOD, STONE, FOOD, IRON, GOLD, COAL };
	if( res == WOOD  )   { sell[1] = WOOD ; }
	if( res == STONE )   { sell[0] = STONE; }

	//экономия еды
	if( GetResource(FOOD) < Safe_FOOD_Amount )  sell[2] = res;

	for(int i=0; i<6 ; i++) 
	{
		if( sell[ i ] != res && GetResource( sell[ i ] ) > MIN_RES_LIMIT + BUY_AMOUNT ) 
		{
			int  amo = BUY_AMOUNT / 4 ;			
			if( sell[ i ] == GOLD )  amo = BUY_AMOUNT *  4 ;				//10 ;
			if( sell[ i ] == COAL )  amo = BUY_AMOUNT *  2 ;				//*  5 ;
			if( sell[ i ] == FOOD && res == GOLD )  amo = BUY_AMOUNT / 11 ;

			AddResource( sell[ i ] , - BUY_AMOUNT );
			AddResource( res	   ,   amo );		//типа - купили
			return true;
		}
	}
	return false;
}
//----------------------------------------------
bool	CPlayer::BuyResourceForAmount( int res , int amount )
{
	CResource_BM	ResBackUp = Resource;

	int TRY_NUM = 1000 ;
	// покупать "бесконечно" (пока не получим искомое кол-во или пока есть остальные ресурсы)
	int  quit = false ;
	for(int tr=0; tr < TRY_NUM && !quit && GetResource( res ) < amount ; tr++ )
		if( !BuyResource( res ) )  quit = true; 

	if( GetResource( res ) < amount ) { 
		Resource = ResBackUp;
		return false; 
	}
	return true;
}
//bool	CPlayer::BuyResourceForAmount( int res , int amount )//grey
//{
//	int TRY_NUM = 1000 ;
//	// покупать "бесконечно" (пока не получим искомое кол-во или пока есть остальные ресурсы)
//	for(int tr=0; tr < TRY_NUM && GetResource( res ) < amount ; tr++ )
//		if( !BuyResource( res ) )		{ return false; }
//	return true;
//}
//----------------------------------------------
bool	CPlayer::EnoughResourceAmount( int res , int amount )
{
	CResource_BM	ResBackUp = Resource ;

	int TRY_NUM = 7 ;
	// покупать TRY_NUM раз (пока не получим искомое кол-во или пока есть остальные ресурсы)
	int  quit = false ;
	for(int tr=0; tr < TRY_NUM && !quit && GetResource( res ) < amount ; tr++ )
		if( !BuyResource( res ) )  quit = true; 

	if( GetResource( res ) < amount ) { 
		Resource = ResBackUp;
		return false; 
	}
	return true;
}
//bool	CPlayer::EnoughResourceAmount( int res , int amount )
//{
//	int TRY_NUM = 7 ;
//	// покупать TRY_NUM раз (пока не получим искомое кол-во или пока есть остальные ресурсы)
//	for(int tr=0; tr < TRY_NUM && GetResource( res ) < amount ; tr++ )
//		if( !BuyResource( res ) )		{ return false; }
//
//	if( GetResource( res ) < amount )	{ return false; }
//	
//	return true;
//}
//----------------------------------------------
void	CPlayer::UpdateResources()
{
	int    diff		= pSAVE->m_iDifficulty;
	float  res_div  = 1;			
	if( m_PlayerID != gi_bmSAVE_Nation && diff < DIFF_LEVELS_N ) {
		res_div = ROOT_SCENARIO.UpdateResDiv[ diff ] ;			// урезать рес-сы компам! в зависимости от diff 
		if( res_div <= 0 )  res_div = 1 ;
	}

	for(int id=0; id < m_psdSectData->GetSectorsNum() ; id++)
		if( m_psdSectData->GetSecOwner( id ) == m_PlayerID &&
			m_psdSectData->m_pssdDT[ id ]->m_iSabotageID%100 / 10 == 0 ) //Sabotage!!!
		{
			int res = m_psdSectData->GetResourceID(id);
			// добыча ресурса в секторе
			if( res != 0 ) {
				if(res==1) { res = FOOD; } else
				if(res==2) { res = GOLD; } else
				if(res==3) { res = IRON; } else
				if(res==4) { res = COAL; }
				
				float  res_div0 = 1;
				if( res != FOOD )  res_div0 = res_div ;
				// осн.ресурс сектора		- ADDING -
				Resource.AddResource( res , SECT_ADDING[ res ] / res_div0 );
			}
			// ресурс дает каждый сектор	- INCOME -
			Resource.AddResource( FOOD ,	SECT_INCOME[ FOOD ] );
			Resource.AddResource( WOOD ,	SECT_INCOME[ WOOD ] / res_div );
			Resource.AddResource( STONE,	SECT_INCOME[ STONE] / res_div );
			Resource.AddResource( GOLD ,	SECT_INCOME[ GOLD ] / res_div );
			Resource.AddResource( IRON,		SECT_INCOME[ IRON ] / res_div );
			Resource.AddResource( COAL,		SECT_INCOME[ COAL ] / res_div );
		}
	
	SetRecruit();
}
//----------------------------------------------
void	CPlayer::SetRecruit()
{
	Resource.SetResource( RECRT, 0 );

	for(int id=0; id < m_psdSectData->GetSectorsNum() ; id++)
		if( m_psdSectData->GetSecOwner(id) == m_PlayerID ) 
		{
			if( m_psdSectData->m_pssdDT[id]->m_iSabotageID%10 )   m_psdSectData->m_pssdDT[id]->m_iRecruits = 0; //// Sabotage!!!
			//if( m_psdSectData->m_iSabotageID[ 0 ][ id ] )   m_psdSectData->m_pssdDT[id]->m_iRecruits = 0; 

			Resource.AddResource( RECRT, m_psdSectData->m_pssdDT[id]->m_iRecruits ); 
		}
}
//void	CPlayer::SetRecruit() ///grey
//{
//	Resource.SetResource( RECRT, Resource.GetResource( RECRT )/2 );
//	//Resource.SetResource( RECRT, 0 );
//	for(int id=0; id < m_psdSectData->GetSectorsNum() ; id++)
//		if( m_psdSectData->GetSecOwner(id) == m_PlayerID ) 
//		{
//			Resource.AddResource( RECRT, (m_psdSectData->GetSecPopulation(id) + 1)*SECT_ADD_RECRT ); 
//		}
//}
//----------------------------------------------
bool	CPlayer::AddSquard(){
	//if (m_psdSquard==NULL && m_inSquardNum==0){
	//	m_psdSquard = new CSquard[1];
	//	m_inSquardNum = 1;
	//}
	//else
	//	if (m_psdSquard!=NULL && m_inSquardNum!=0){
	//		CSquard* a_psTemp = new CSquard[m_inSquardNum];
	//		for (int i=0; i<m_inSquardNum; i++){
	//			a_psTemp[i] = m_psdSquard[i];
	//		}
	//		delete[]m_psdSquard;
	//		m_psdSquard = new CSquard[m_inSquardNum+1];
	//		for (int i=0; i<m_inSquardNum; i++){
	//			m_psdSquard[i] = a_psTemp[i];
	//		}
	//		m_inSquardNum++;
	//		delete[]a_psTemp;
	//	}
	//	else{
	//		char	error[256];
	//		sprintf(error,"%s%d","ILL SQUARD, PLAYER ",m_PlayerID);
	//		SendError(error);
	//		return false;
	//	}
	return true;
}
//----------------------------------------------
bool	CPlayer::DeleteSquard(int ID){
	//if (m_psdSquard==NULL && m_inSquardNum==0){
	//	return true;
	//}
	//else
	//	if (m_psdSquard!=NULL && m_inSquardNum!=0){
	//		CSquard* a_psTemp = new CSquard[m_inSquardNum];
	//		for (int i=0; i<m_inSquardNum; i++){
	//			a_psTemp[i] = m_psdSquard[i];
	//		}
	//		delete[]m_psdSquard;
	//		m_psdSquard = new CSquard[m_inSquardNum-1];
	//		int j=0;
	//		for (int i=0; i<m_inSquardNum; i++){
	//			if (i!=ID){
	//				m_psdSquard[j] = a_psTemp[i];
	//				j++;
	//			}
	//		}
	//		m_inSquardNum--;
	//		delete[]a_psTemp;
	//	}
	//	else{
	//		SendError("CAN'T DELETE SQUARD");
	//		return false;
	//	}
	return true;
}
//----------------------------------------------
void	CPlayer::SetHeroMiniMenuVisible(bool status)
{
	for(int i=0; i<m_inHeroNum; i++ ) {
		m_phrHero[i].SetVisibleMiniMenu(status);
		//if(m_phrHero[i].m_Alive) m_phrHero[i].SetVisibleMiniMenu(status);
	}
}
//----------------------------------------------
void	CPlayer::TryShowSPortret(int idH)
{
	for(int i=0; i<m_inHeroNum; i++ ) {
		if( m_phrHero[i].GetHeroID() == idH ) {
			m_phrHero[i].SetVisibleMiniMenu(true);
			//if(m_phrHero[i].m_Alive) m_phrHero[i].SetVisibleMiniMenu(true);
		}
	}
}
//----------------------------------------------
CHero*	CPlayer::GetHeroByPID(int pid){
	for ( int i=0; i<m_inHeroNum; i++ ){
		if ( m_phrHero[i].GetHeroID()==pid ) {
			return	&(m_phrHero[i]);
		}
	}
	return NULL;
}
//----------------------------------------------
CHero*	CPlayer::GetHeroInSector(int sid){
	CHero*	pH = NULL;
	for(int i=0; i < m_inHeroNum && pH == NULL ; i++) {
		if ( m_phrHero[i].m_Alive && m_phrHero[i].getSectorID()==sid ) {
			pH = &(m_phrHero[i]);
		}
	}	
	return	pH;
}
//----------------------------------------------
int		CPlayer::GetNewHeroID() //grey
{
	int maxID = -1;			/* m_inHeroID = 0 .. 9 */

	// найти след. по списку героя
	for(int i=0; i<m_inHeroNum; i++) {
		if( m_phrHero[i].m_inHeroID > maxID ) { maxID = m_phrHero[i].m_inHeroID; }
	}
	if( maxID+1 < MAX_HEROES )   { return maxID+1; }

	// найти свободное id героя
	for(int id=0; id<MAX_HEROES; id++) {
		bool find = false;
		for(int i=0; i<m_inHeroNum && !find ; i++) {
			if( !m_phrHero[i].m_Alive && m_phrHero[i].m_inHeroID == id ) { find = true; }
		}
		if( !find )   { return id; }
	}

	return MAX_HEROES;
}

//----------------------------------------------
//int		CPlayer::AddHero(){
//	if (m_phrHero==NULL && m_inHeroNum==0){
//		m_phrHero = new CHero[1];
//		m_inHeroNum = 1;
//		return 0;
//	}
//	else
//		if (m_phrHero!=NULL && m_inHeroNum!=0){
//			CHero* a_phTemp = new CHero[m_inHeroNum];
//			for (int i=0; i<m_inHeroNum; i++){
//				a_phTemp[i] = m_phrHero[i];
//			}
//			delete[]m_phrHero;
//			m_phrHero = new CHero[m_inHeroNum+1];
//			for (int i=0; i<m_inHeroNum; i++){
//				m_phrHero[i] = a_phTemp[i];
//			}
//			m_inHeroNum++;
//			delete[]a_phTemp;
//			return m_inHeroNum-1;
//		}
//		else{
//			char	error[256];
//			sprintf(error,"%s%d","ILL HERO, PLAYER ",m_PlayerID);
//			SendError(error);
//			return -1;
//		}
//	return 0;
//}
//----------------------------------------------

int		CPlayer::AddHero()
{
	m_inHeroNum = 1;
	return 0;
}

//----------------------------------------------
bool	CPlayer::AddHero(DialogsSystem* pds,LocalGP* mlgp,int mid)
{
	int a_nid = AddHero();
	SimpleDialog* psd = NULL;

	GPPicture*	pict	= pds->addGPPicture(NULL,0,0,mlgp->GPID,mid);
	pict->ShapeFileID	= mlgp->GPID;
	pict->ShapeSpriteID	= mid;
	pict->Nation		= ROOT_SCENARIO.m_iNationID[m_PlayerID];
	pict->Visible		= false;

	if( m_PlayerID == gi_bmSAVE_Nation ) {
		pict->AssignSound( "INTERF_ONHERO", CLICK_SOUND );
	} else {
		pict->AssignSound( "INTERF_SECTOR", CLICK_SOUND );
	}
	//pict->Rotated		= true;

	psd = dynamic_cast<SimpleDialog*>( pict );
	m_phrHero[a_nid].setMapPic(psd);

	//psd = dynamic_cast<SimpleDialog*>(pds->addGPPicture(NULL,0,0,mlgp->GPID,mid));
	//psd->ShapeFileID	= mlgp->GPID;
	//psd->ShapeSpriteID	= mid;
	//psd->Visible		= false;
	//m_phrHero[a_nid].setMapPic(psd);
	
	return true;
}
//----------------------------------------------
bool	CPlayer::AddHero(int sectID,int heroID)
{
	if( sectID < 0 )					{ return false; }
	if( m_inHeroNum >= MAX_HEROES )		{ return false; }

	//grey //						 // very new comment!!! LAST ADD!!!
	//if( IsHeroIDPresent(heroID) )
	//{ 
	//	CHero*	pH = GetHeroByPID(heroID); 
	//	if(pH)	{ 
	//		pH->m_Alive = true; 
	//		pH->ReEnableHero();  /*true!*/ 
	//		pH->SetSector( m_psdSectData, sectID ); 
	//		return true; 
	//	}
	//	return false;  
	//}
	//grey //						 // very new comment!!! LAST ADD!!!

	DataForAddHero.pMenu->addClipper(DataForAddHero.mx0,DataForAddHero.my0,DataForAddHero.mx1,DataForAddHero.my1);
	AddHero(DataForAddHero.pMenu,DataForAddHero.pMapPic, 1 ) ; //0 ); //nwe// grey
	DataForAddHero.pMenu->addClipper(DataForAddHero.sx0,DataForAddHero.sy0,DataForAddHero.sx1,DataForAddHero.sy1);
	for (int i=0; i<m_inHeroNum; i++){
		m_phrHero[i].SetOnClickMap(DataForAddHero.pfHeroClick,-1);
		//m_phrHero[i].setOnRClickMap(DataForAddHero.pfHeroRClick,-1);
		m_phrHero[i].setPlayerID(m_PlayerID);
		m_phrHero[i].pArmyLimit = DataForAddHero.pArmyLimit;
	}
	m_phrHero[m_inHeroNum-1].SetSector(DataForAddHero.pSectData,sectID);
	m_phrHero[m_inHeroNum-1].SetVisible(true);
	
    SetNewHeroData(heroID);

	return true;
}
//----------------------------------------------
void	CPlayer::SetNewHeroData(int heroID) 
{
	int rn = heroID;
	bool	find = true;
	if (heroID==-1){
		rn = rand()%MAX_HEROES;
		find = false;
		int i=0;
		while(!find&&i<MAX_HEROES) {
			if (IsHeroIDPresent(rn)) {
				rn++;	rn = rn%MAX_HEROES;
			}
			else {
				find = true;
			}
			i++;
		};
	}
	
	
	if (find) {
		m_phrHero[m_inHeroNum-1].SetHeroID(rn);
		//Создать имя для героя.
		char	name[256];
		sprintf(name,"%s","#HERO_");
		switch(m_PlayerID) {
		case 0:
			sprintf(name,"%s%s%d","#HERO_","EG_",rn);
			break;
		case 1:
			sprintf(name,"%s%s%d","#HERO_","FR_",rn);
			break;
		case 2:
			sprintf(name,"%s%s%d","#HERO_","AS_",rn);
			break;
		case 3:
			sprintf(name,"%s%s%d","#HERO_","PR_",rn);
			break;
		case 4:
			sprintf(name,"%s%s%d","#HERO_","RS_",rn);
			break;
		case 5:
			sprintf(name,"%s%s%d","#HERO_","EP_",rn);
			break;
		}
		m_phrHero[m_inHeroNum-1].SetName(GetTextByID(name));
		
		//Задать портрет героя.
		m_phrHero[m_inHeroNum-1].SetBigPotret(DataForAddHero.pPORTRAIT->getBigP(m_PlayerID,rn));
		m_phrHero[m_inHeroNum-1].SetPotret(DataForAddHero.pPORTRAIT->getSmallP(m_PlayerID,rn));

		m_phrHero[m_inHeroNum-1].CreateMiniMenu();
		
		m_phrHero[m_inHeroNum-1].SetARMY();

		m_phrHero[m_inHeroNum-1].CreateHint();
	}
}	
//----------------------------------------------
bool	CPlayer::DeleteHero(int ID)
{
	if( ID < 0 || ID >= m_inHeroNum )			{ return false; }

	if( !m_phrHero && m_inHeroNum == 0 ) { 
		return true;  
	}
	else
		if( m_phrHero && m_inHeroNum != 0 ) {
			m_phrHero[ID].DisableHero();
		}
		else{
			SendError("CAN'T DELETE HERO");
			return false;
		}
	return true;
}
//----------------------------------------------
void	CPlayer::TryRetreat( CHero* pH, bool die ) // попробовать отступить если не убили.
{
	if( pH ) {
		//Поиск индекса героя.
		int hid = -1;
		for(int i=0; i < m_inHeroNum && hid == -1 ; i++) {
			if( pH == (m_phrHero + i) ) {
				hid = i;
			}
		}
		//Должен умереть.
		if( die ) {
			DeleteHero(hid);
		} else {
			//Поиск сектора для отступления.
			int   nsARR[1000];
			int   NofS	= m_psdSectData->GetNeighborsIDs( pH->getSectorID(), nsARR, 100);
			int   secID = -1,  secID2 = -1,  secID3R = -1;
			bool  find  = false;
			for(int i=0; i < NofS && !find ; i++) {
				int curID = nsARR[i];
				if( CanStepIntoSector( m_PlayerID , curID ) ) {
					secID = curID;
					if( pH->getPreviousSecID() == curID )  find = true;
				} else { 
					int  ARR2[1000] ;
					int  NN = m_psdSectData->GetNeighborsIDs( curID , ARR2 , 1000 );
					for(int j=0; j < NN ; j++) { //проход по соседям соседей тек.сектора
						int id = ARR2[ j ] ;
						if( CanStepIntoSector( m_PlayerID, id ) && (secID2 < 0 || rand()%10 > 5) )  secID2 = id;
						if( !ROOT_SCENARIO.GetHeroInSect( id , 0xFFFF ) && (secID3R < 0 || rand()%10 > 5 ) )  
							secID3R = id; // find ANY neib sector
					}
				}
				if( !ROOT_SCENARIO.GetHeroInSect( curID , 0xFFFF ) && ( secID3R < 0 || rand()%10 > 5 ) )  
					secID3R = curID; // find ANY neib sector // EMERGEMCY!!!
			}
			if( secID < 0 && secID2 >= 0 )  { secID = secID2; }
			
			int  pathID = -1 ,  min_path = 0xFFFF ,  min_ID = -1;
			if( secID < 0 ) {
				for(int id=0; id < m_psdSectData->GetSectorsNum() ; id++) //НАЙТИ СВОБОДН. СЕКТОР!
					if( CanStepIntoSector( m_PlayerID , id ) ) {
						bool change = true;
						if(  secID >= 0 ) { 
							if( pH->getSectorID() < 12 && secID >= 12 ) 
								change = false; // чтобы герои из начальных секторов (0-10) не попадали в сектора с id > 20  
							if( m_psdSectData->GetSecOwner(secID) == m_PlayerID && m_psdSectData->GetResourceID(secID) == 2 ) //GOLD 
								change = false; //capital 
						}
						if( change )  secID = id;

						secID2 = ROOT_SCENARIO.MakePathSectorToSector(m_PlayerID, pH->getSectorID(), id, -1, pathID);
						if( secID2 != -1  &&  
							pathID != -1  &&  min_path > ROOT_SCENARIO.Path[ pathID ][0] ) { 
							min_ID	 = id ;		
							min_path = ROOT_SCENARIO.Path[ pathID ][0];		// Path[ PID ][0] - path length
						}
				}
				if( min_ID != -1 )  
					secID = min_ID;
			}

			//new
			if( secID < 0 && secID3R >= 0 ) { // EMERGEMCY!!!
				secID = secID3R ;
			}
			//new

			//if( secID < 0 && m_psdSectData->GetPlayerSect( m_PlayerID ) == 0 ) { 
			if( m_psdSectData->GetPlayerSect( m_PlayerID ) == 0 && pH->GetBranch_NSQ(0)+pH->GetBranch_NSQ(1) < 1 ) {
				DeleteHero(hid); //или: сделать отступление в столичный сектор или последний оставшийся
			} else {
				pH->SetFuel( 0 );
				if( secID >= 0 )  pH->SetSector( m_psdSectData , secID );
			}
		}
	}
}
//----------------------------------------------
bool	CPlayer::IsHeroPresentInSector(int id){
	bool	present = false;
	for (int i=0; (!present)&&(i<m_inHeroNum); i++){
		if (((m_phrHero[i]).getSectorID())==id) {
			present = true;
		}
	}
	return present;
}
//----------------------------------------------
void	CPlayer::SetHeroFuel(int HID, int Dist){
	for (int i=0; i<m_inHeroNum; i++){
		if (m_phrHero[i].GetHeroID()==HID) {
			m_phrHero[i].SetFuel(Dist);
		}
	}
}
//----------------------------------------------
void	CPlayer::SetHeroesFuel(int Dist)
{
	for (int i=0; i < m_inHeroNum; i++)
	{
		m_phrHero[i].SetFuel(Dist);
	}
}
//----------------------------------------------
int		CPlayer::GetPlayerMoves()
{
	int move = 0 ;
	for(int i=0; i < m_inHeroNum ; i++) {
		if( m_phrHero[i].m_Alive && m_phrHero[i].m_inFuel > 0 ) { move++; }
		//if(m_phrHero[i].m_inFuel>0) { move++; }
	}
	return move;
}
//----------------------------------------------
void	CPlayer::SetDefaultARMY(int heroID){
	if (heroID==-1) {
		for (int i=0; i<m_inHeroNum; i++){
			m_phrHero[i].SetDefaultARMY();
		}
	}else{
		if (0<=heroID&&heroID<m_inHeroNum) {
			m_phrHero[heroID].SetDefaultARMY();
		}
	}
	return;
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
///////////////// SRank ////////////////
////////////////////////////////////////
//----------------------------------------------
SRank::SRank(){
	Init();
	}//SRank::SRank();
//----------------------------------------------
SRank::~SRank(){
	for (int i=0; i<MAX_COUNTRYS; i++)
		TYPES_CONT[i].DeleteTYPES();
	Init();
}//SRank::~SRank();
//----------------------------------------------
void	SRank::Init(){
	rankName = "NO_RANK";
	rankID = -1;
	for (int i=0; i<MAX_COUNTRYS; i++){
		TYPES_CONT[i].country		= "NO_COUNTRY";
		TYPES_CONT[i].maxSquards	= 0;
		TYPES_CONT[i].TN			= 0;
		TYPES_CONT[i].TYPES			= NULL;
		TYPES_CONT[i].DeleteTYPES();
    }
}//void		SRank::Init();
//----------------------------------------------
void	SRank::DeleteTYPE(int cid){
	if (cid>=0&&cid<MAX_COUNTRYS)
		TYPES_CONT[cid].DeleteTYPES();
}//void		SRank::DeleteTYPE(int cid);		
//----------------------------------------------
void	SRank::DeleteTYPE(){
	for (int i=0; i<MAX_COUNTRYS; i++)
		TYPES_CONT[i].DeleteTYPES();
}//void		SRank::DeleteTYPE();	
//----------------------------------------------
void	SRank::CreateNTYPES(int cid,int n){
	if (n<=0)	return;
    if (cid>=0&&cid<MAX_COUNTRYS) {
		TYPES_CONT[cid].DeleteTYPES();
        TYPES_CONT[cid].TYPES = new SSquardData[n];
		TYPES_CONT[cid].TN = n;
	}
}//void		SRank::CreateNTYPES(int cid,int n);
//----------------------------------------------
void	SRank::AddTYPE(int cid){
	if (cid>=MAX_COUNTRYS || TYPES_CONT[cid].TYPES==NULL)	return;
	SSquardData* ptsd=NULL;
	if (TYPES_CONT[cid].TN!=0){
		ptsd = new SSquardData[TYPES_CONT[cid].TN];
		for	(int i=0; i<TYPES_CONT[cid].TN; i++)		ptsd[i]=TYPES_CONT[cid].TYPES[i];
		if (TYPES_CONT[cid].TYPES!=NULL) delete[]TYPES_CONT[cid].TYPES;
	}
	TYPES_CONT[cid].TYPES = new SSquardData[TYPES_CONT[cid].TN+1];
	if (TYPES_CONT[cid].TN!=0) {
		for (int i=0; i<TYPES_CONT[cid].TN; i++)		TYPES_CONT[cid].TYPES[i]=ptsd[i];
		delete[]ptsd;
	}	
	TYPES_CONT[cid].TN++;
}//void		SRank::AddTYPE(int cid);
//----------------------------------------------
void	SRank::SCUType::DeleteTYPES(){
	if (TYPES!=NULL) {
		delete[]TYPES;
		TYPES=NULL;
	}
	TN=0;
}//void	SRank::SCUType::DeleteTYPES();
//----------------------------------------------
void	SRank::SCUType::DeleteZeroTYPE(){
	int rTN = 0;
	for (int i=0; i<TN; i++){
		if (TYPES[i].maxSquards>0) rTN++;
	}
	if (rTN==0)	return;
	SSquardData* tempSD = new SSquardData[rTN];
	int ci = 0;
	for (int i=0; i<TN; i++){
		if (TYPES[i].maxSquards>0) {
			tempSD[ci] = TYPES[i];
			ci++;
		}
	}
	DeleteTYPES();
	TN=rTN;
	TYPES = new SSquardData[rTN];
	for (int i=0; i<rTN; i++)	TYPES[i]=tempSD[i];
	delete[]tempSD;
}//void	SRank::SCUType::DeleteZeroTYPE();
//----------------------------------------------
void	SRank::SCUType::operator =(SCUType& cut){
//	country = cut.country;
//	maxSquards=cut.maxSquards;
	TN=cut.TN;
	if (TYPES!=NULL)	delete[]TYPES;
	TYPES = new SSquardData[TN];
	for (int i=0; i<TN; i++)	TYPES[i]=cut.TYPES[i];
}//void	SRank::SCUType::operator =(SCUType& cut);
//----------------------------------------------
void	SRank::SCUType::operator +=(SSquardData& sd){
	SSquardData* psdt=NULL;
	if (TN!=0) {
		psdt = new SSquardData[TN];
		for (int i=0; i<TN; i++)	psdt[i]=TYPES[i];
		if (TYPES!=NULL) delete[]TYPES;
	}
	TYPES = new SSquardData[TN+1];
	if (TN!=0) {
		for (int i=0; i<TN; i++)		TYPES[i]=psdt[i];
		delete[]psdt;
	}
	TN++;
	TYPES[TN-1]=sd;
}//void	SRank::SCUType::operator +=(SSquardData& sd);
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
/////////////// CArmyLimit /////////////
////////////////////////////////////////
////----------------------------------------------
CArmyLimit::CArmyLimit(char* fName)	{
	Load(fName);
}
//CArmyLimit::CArmyLimit(char* fName);
//----------------------------------------------
void	CArmyLimit::SetNSqdNID(int rankID, int countryID, int n, int id ){
	if ( 0<=rankID && rankID<MAX_RANKS && 0<=countryID && countryID<MAX_COUNTRYS ) {
		if ( 0<=n && n<HERO_RANKS[rankID].TYPES_CONT[countryID].TN ) {
			HERO_RANKS[rankID].TYPES_CONT[countryID].TYPES[n].curID = id;
		}
	}
}
//----------------------------------------------
int		CArmyLimit::GetMaxDiffSqd( int rankID, int countryID ){
	if ( 0<=rankID && rankID<MAX_RANKS && 0<=countryID && countryID<MAX_COUNTRYS ) {
		return HERO_RANKS[rankID].TYPES_CONT[countryID].TN;
	}
	return 0;
}
//----------------------------------------------
int		CArmyLimit::GetMaxSqd    ( int rankID, int countryID ){
	if ( 0<=rankID && rankID<MAX_RANKS && 0<=countryID && countryID<MAX_COUNTRYS ) {
		return HERO_RANKS[rankID].TYPES_CONT[countryID].maxSquards;
	}
	return 0;
}
//----------------------------------------------
int		CArmyLimit::GetNSqdNum	 ( int rankID, int countryID, int n ){
	if ( 0<=rankID && rankID<MAX_RANKS && 0<=countryID && countryID<MAX_COUNTRYS ) {
		if ( 0<=n && n<HERO_RANKS[rankID].TYPES_CONT[countryID].TN ) {
			return HERO_RANKS[rankID].TYPES_CONT[countryID].TYPES[n].maxSquards;
		}
	}
	return 0;
}
//----------------------------------------------
char*	CArmyLimit::GetNSqdID    ( int rankID, int countryID, int n ){
	if ( 0<=rankID && rankID<MAX_RANKS && 0<=countryID && countryID<MAX_COUNTRYS ) {
		if ( 0<=n && n<HERO_RANKS[rankID].TYPES_CONT[countryID].TN ) {
			return HERO_RANKS[rankID].TYPES_CONT[countryID].TYPES[n].unitName.pchar();
		}
	}
	return	NULL;
}
//----------------------------------------------
int		CArmyLimit::GetNSqdNID(int rankID, int countryID, int n ){
	if ( 0<=rankID && rankID<MAX_RANKS && 0<=countryID && countryID<MAX_COUNTRYS ) {
		if ( 0<=n && n<HERO_RANKS[rankID].TYPES_CONT[countryID].TN ) {
			return HERO_RANKS[rankID].TYPES_CONT[countryID].TYPES[n].curID;
		}
	}
	return	0;
}
//----------------------------------------------
bool	CArmyLimit::Load(char* fName){
	if (fName==NULL)	return	true;

	DString		a_DS; //Парсим.
	a_DS.ReadFromFile(fName);
	DString		a_DS2;//Заполняем.
	DString		a_DS3;//Заполняем.
	
	for (int i=0; i<MAX_RANKS; i++){
		a_DS.ExtractWord(a_DS2);
		a_DS.ExtractWord(a_DS3);
		HERO_RANKS[i].rankID=atoi(a_DS3.pchar());
		HERO_RANKS[i].rankName = GetTextByID( a_DS2.pchar() );
		
		for (int ii=0; ii<MAX_COUNTRYS; ii++){
			a_DS.ExtractWord(a_DS2);
			HERO_RANKS[i].TYPES_CONT[ii].country = GetTextByID( a_DS2.pchar() );
			a_DS.ExtractWord(a_DS2);
			HERO_RANKS[i].TYPES_CONT[ii].maxSquards=atoi(a_DS2.pchar());

			if (i!=0){
				HERO_RANKS[i].TYPES_CONT[ii]=HERO_RANKS[i-1].TYPES_CONT[ii];
				for (int iii=0; iii<HERO_RANKS[i].TYPES_CONT[ii].TN; iii++){
					a_DS.ExtractWord(a_DS2);
					HERO_RANKS[i].TYPES_CONT[ii].TYPES[iii].maxSquards=atoi(a_DS2.pchar());
				}
			}
			
			a_DS.ExtractWord(a_DS2);
			for (int iii=0; iii<atoi(a_DS2.pchar()); iii++){
				SSquardData	dsT;
				a_DS.ExtractWord(a_DS3);
				dsT.unitName = a_DS3;
				a_DS.ExtractWord(a_DS3);
				dsT.maxSquards=atoi(a_DS3.pchar());

				HERO_RANKS[i].TYPES_CONT[ii] += dsT;
			}
		}
	}
	DelZoroTYPE();	
	return true;
}//bool	CArmyLimit::Load(char* fName);
//----------------------------------------------
void	CArmyLimit::DelZoroTYPE(){
	for (int i=0; i<MAX_RANKS; i++){
		for (int ii=0; ii<MAX_COUNTRYS; ii++){
			HERO_RANKS[i].TYPES_CONT[ii].DeleteZeroTYPE();
		}
	}
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
//////////// CHERO_MINI_MENU ///////////
////////////////////////////////////////
//----------------------------------------------
CHERO_MINI_MENU::CHERO_MINI_MENU() {
	INIT();
}
//----------------------------------------------
void	CHERO_MINI_MENU::INIT() {
	X0			= 0;
	Y0			= 0;
	lpszName = "NONAME";
	lpszRank = "NORANK";
	inStat		= 0;
	inFlag		= 0;
	//pgptbReinforce	= NULL;	//pgptbUpgrade	= NULL;	//bReinf		= true;	//bUpgrd		= true;
	pgptbName		= NULL;
	pgptbRank		= NULL;
	for(int i=0; i<MAX_H_STATUS; i++ ) pgppStat[i] = NULL;
	for(int j=0; j<MAX_PLAYERS+1;j++ ) pgppFlag[j] = NULL;
	//grey
	iSQ_NN = iSQ_MAX = 0;
 	iExpa		 = 0 ;  		// expa
 	iNLExpa		 = 0 ;  		// expa
	iExpPercent	 = 0 ;			// exp-percent for next level
	
	//chSQNum      = "000/000";// SQ-num
	ptbExpa		 = NULL ;	// expa
	pgppExpBar	 = NULL ;	// expa-BAR
	pgppExpBarPIC= NULL ;
	pgppExpBack  = NULL ;	// BackGround for expa

	ptbSQNum	 = NULL ;	// SQ-num
	pgppMenuBack = NULL ;	// BackGround for HERO_MINI_MENU
	m_pHero		 = NULL;
	ptbVictN	 = NULL;
}
//----------------------------------------------
void	CHERO_MINI_MENU::INIT( int x0, int y0 ) {
	INIT();
	X0	= x0;
	Y0	= y0;
}
//----------------------------------------------
void	CHERO_MINI_MENU::AddBTNS_MENU( LocalGP*	pGP, DialogsSystem*	pMN )
{
	if ( pGP==NULL || pMN==NULL ) { return; }
	//RLCFont* font = &SmallBlackFont ;	int		 fontDY = 8;
	char	sTMP[256];
	strcpy( sTMP , "..." );

	LocalGP		a_lgpElems("Interf3\\TotalWarGraph\\bmElements");
	pgppMenuBack		  =	pMN->addGPPicture(NULL,0,0, a_lgpElems.GPID , 2) ;
	pgppMenuBack->Visible = false;

	pgptbName			 = pMN->addTextButton(NULL,0,0, "_" , &RedFont,&RedFont,&RedFont, 0);
	BigMapSetMessage( pgptbName , sTMP );
	pgptbName->Visible	 = false;
	pgptbRank			 = pMN->addTextButton(NULL,0,0, "_" , &SpecialBlackFont,&SpecialBlackFont,&SpecialBlackFont, 0);
	BigMapSetMessage( pgptbRank , sTMP );
	pgptbRank->Visible	 = false;

	LocalGP		a_lgpExpBar("Interf3\\TotalWarGraph\\bmExpBar");
	pgppExpBack			 = pMN->addGPPicture(NULL,0,0, a_lgpExpBar.GPID , 0 ) ;
	pgppExpBack->Visible = false;

	pgppExpBar			 = pMN->AddDialogsDesk( 0, 0, pgppExpBack->x1, pgppExpBack->y1 ,"NullBorder");
	pgppExpBar->Visible	 = false; 
	pgppExpBarPIC		 = pgppExpBar->addGPPicture(NULL,0,0, a_lgpExpBar.GPID , 1 ) ;

	ptbVictN			= pMN->addTextButton(NULL,0,0, "_" ,&SmallGrayFont1,&SmallGrayFont1,&SmallGrayFont1 /*SmallGrayFont*/ ,0);
	if(ptbVictN)  BigMapSetMessage( ptbVictN , sTMP );	
	if(ptbVictN)  ptbVictN->Visible	 = false;

	ptbExpa				 = pMN->addTextButton(NULL,0,0, "_" ,&SmallGrayFont1,&SmallGrayFont1,&SmallGrayFont1 /*SpecialGrayFont*/ ,0);
	BigMapSetMessage( ptbExpa , sTMP );	
	ptbExpa->Visible	 = false;

	ptbSQNum			 = pMN->addTextButton(NULL,0,0, "_" ,&GrayFont,&GrayFont,&GrayFont,0);
	BigMapSetMessage( ptbSQNum , sTMP );
	ptbSQNum->Visible	 = false;
}
//----------------------------------------------
void	CHERO_MINI_MENU::AddSTAT_MENU( LocalGP*	pGP, DialogsSystem*	pMN )
{
	if ( pGP==NULL || pMN==NULL ) {
		return;
	}
	pgppStat[0]	= pMN->addGPPicture(NULL,0,0,pGP->GPID, 0 );
	pgppStat[0]->Visible = false;
}
//----------------------------------------------
void	CHERO_MINI_MENU::AddFLAG_MENU( LocalGP*	pGP, DialogsSystem*	pMN )
{
	if ( pGP==NULL || pMN==NULL ) {
		return;
	}
	pgppFlag[0]	= pMN->addGPPicture(NULL,0,0,pGP->GPID, 0 );
	pgppFlag[0]->Diffuse = 0x9FFFFFFF;
	pgppFlag[0]->Visible = false;
}
//----------------------------------------------
void	CHERO_MINI_MENU::SetMPos( int x0, int y0 ){
	X0	= x0;
	Y0	= y0;
	ArrangeIcons();
}
//----------------------------------------------
void	CHERO_MINI_MENU::UpdateMenu()
{
	char  s[256];
	_str  tBlack, tBrown, tRed, font0;
	tBlack = "{C FF101010}";  tBrown = "{C FF502515}";  tRed = "{C FF640100}";	font0 = "{FS}";
	if( lpszName.pchar() && lpszName != "NONAME" ) {
		BigMapSetMessage( pgptbName , lpszName.pchar() ) ;
	}
	if( lpszRank.pchar() && lpszRank != "NORANK" ) {
		BigMapSetMessage( pgptbRank , lpszRank.pchar() ) ;
	}
	SetStatVisible(false);
	if( 0 <= inStat && inStat <= MAX_H_STATUS ) {
		pgppStat[0]->Visible = true;
	}
	SetFlagVisible(false);
	if( 0 <= inFlag && inFlag <= MAX_PLAYERS+1 ) {
		pgppFlag[0]->Visible = true;
	}

	sprintf(s, BIGMAP_EXPSFORMAT , GetTextByID("#Experience") , iExpa ) ;
	ptbExpa->SetMessage( s );

	sprintf(s, BIGMAP_SQNNFORMAT , GetTextByID("#SquadsN"), iSQ_NN , iSQ_MAX );
	ptbSQNum->SetMessage( s );
	// sq hint
	_str	a_Hint;	
	a_Hint = font0;		a_Hint += tBrown;	
	a_Hint += GetTextByID("#SquadsNum");	a_Hint += " / ";		a_Hint += GetTextByID("#MaxSquadsNum");	  
	DYNHINT_RA( ptbSQNum , a_Hint.pchar() );
	//a_Hint += GetTextByID("#SquadsNum");	  a_Hint += " ";		a_Hint += tRed; 
	//a_Hint += iSQ_NN;						  a_Hint += tBrown;		a_Hint += "  /  ";	
	//a_Hint += GetTextByID("#MaxSquadsNum");	  a_Hint += " ";		a_Hint += tRed;	a_Hint += iSQ_MAX;

	// victories hint
	if( m_pHero && ptbVictN ) {
		sprintf(s, BIGMAP_VICTFORMAT , GetTextByID("#Victories"), m_pHero->m_iVictN , m_pHero->m_iBatlN );
		ptbVictN->SetMessage( s );
		
		a_Hint.DString::Clear();	a_Hint = font0;		a_Hint += tBrown;	
		a_Hint += GetTextByID("#CHINT_Victories");			DYNHINT_RA( ptbVictN , a_Hint.pchar() );
	}

	pgppExpBar->x1 = pgppExpBar->x + iExpPercent ;

	// EXP-HINT
	a_Hint.DString::Clear();	a_Hint = font0;		a_Hint += tBrown;	
	if( iNLExpa != 0xFFFF )  { a_Hint += GetTextByID("#CHINT_Next_Exp_Level"); a_Hint += " "; a_Hint += tRed; a_Hint += iNLExpa; }
	else					 { a_Hint += tRed; a_Hint += GetTextByID( "#CHINT_MaxExpLevel" ) ; }
	DYNHINT_RA( ptbExpa	 , a_Hint.pchar() );
	DYNHINT_RA( pgppExpBack , a_Hint.pchar() );
}
//----------------------------------------------
void	CHERO_MINI_MENU::CheckVisible(){
	if (pgptbName->Visible==false) {
		SetVisible(true);

		UpdateMenu();//grey
	}
	//UpdateMenu();//vic
}
//----------------------------------------------

void	CHERO_MINI_MENU::SetEnabled( bool  state )
{
	pgptbName->Enabled		= state ;
	pgptbRank->Enabled		= state ;
	pgppStat[0]->Enabled	= state ;
	pgppFlag[0]->Enabled	= state ;
	ptbExpa->Enabled		= state ;	// expa
	pgppExpBar->Enabled		= state ;	// expa-BAR
	pgppExpBack->Enabled	= state ;
	ptbSQNum->Enabled		= state ;	// SQ-num
	pgppMenuBack->Enabled	= state ;
	if(ptbVictN)  ptbVictN->Enabled = state ; 
}

//----------------------------------------------
void	CHERO_MINI_MENU::SetVisible( bool  state )
{
	pgptbName->Visible		= state;
	pgptbRank->Visible		= state;
	SetStatVisible( state );
	SetFlagVisible( state );//TEMP
	//pgptbReinforce->Visible	= state;	//pgptbUpgrade->Visible	= stat;
	ptbExpa->Visible		= state ;	// expa
	pgppExpBar->Visible		= state ;	// expa-BAR
	pgppExpBack->Visible	= state ;
	ptbSQNum->Visible		= state ;	// SQ-num
	pgppMenuBack->Visible	= state ;
	if(ptbVictN)  ptbVictN->Visible		= state ; 
}
//----------------------------------------------
void	CHERO_MINI_MENU::SetName( char* name ){
	if ( name!=NULL ) {
		lpszName = name;
	}
}
//----------------------------------------------
void	CHERO_MINI_MENU::SetRank( int	rank )
{
	switch( rank ) {      
	case 0:
		lpszRank = GetTextByID( "#LIEUTENANT" );
		break;
	case 1:
		lpszRank = GetTextByID( "#CAPTAIN" );
		break;
	case 2:
		lpszRank = GetTextByID( "#MAJOR" );	
		break;
	case 3:
		lpszRank = GetTextByID( "#COLONEL" );
		break;
	case 4:
		lpszRank = GetTextByID( "#GENERAL" );
		break;
	case 5:
		lpszRank = GetTextByID( "#MARSHAL" );
		break;
	}
	iExpPercent = GetExpPercentByRankID( iExpa, rank , iNLExpa );
}
//----------------------------------------------
void	CHERO_MINI_MENU::SetStat( int stat ) {
	inStat = stat;
	if( 0 <= stat && stat <= MAX_H_STATUS ) {
		pgppStat[0]->SpriteID = stat;
		//pgppStat[0]->y = Y0 +25 + 2*(MAX_H_STATUS - stat)/10;

		// awards hint
		_str  a_Hint, tBlack, tBrown, tRed;	tBlack = "{C FF101010}";	 tBrown = "{C FF502515}";	 tRed = "{C FF640100}";
		a_Hint  = "{FS}";
		a_Hint += tBrown;	a_Hint += GetTextByID("#CWT_Quantity");		a_Hint += " ";		
		a_Hint += tRed;		a_Hint += inStat;							DYNHINT_RA(pgppStat[0], a_Hint.pchar());
	}
}
//----------------------------------------------
void	CHERO_MINI_MENU::SetFlag( int flag ) {
	if( 0 <= flag && flag <= MAX_PLAYERS+1 ) {
		inFlag = pgppFlag[0]->SpriteID = flag;
		// flag hint
		int PID = flag - 1;
		if( PID >= 0 && PID < MAX_PLAYERS && ROOT_SCENARIO.m_pPlayer[ PID ] ) { 
			_str  a_Hint,  tBlack,  tRed;	tBlack = "{C FF101010}";	 tRed = "{C FF640100}";
			a_Hint  = "{FS}";
			a_Hint += tRed;		a_Hint += ROOT_SCENARIO.m_pPlayer[ PID ]->GetPlayerName();
			DYNHINT_RA(pgppFlag[0], a_Hint.pchar());
		}
	}
}
//----------------------------------------------
void	CHERO_MINI_MENU::SetAll( CHero* pHero )
{
	m_pHero		= pHero;

	iSQ_NN		= pHero->GetARMY_NSQ() ;
	iSQ_MAX		= pHero->GetARMY_NSQ() + pHero->GetARMY_FreeNSQ() ;

	iExpa		= pHero->m_inExperience ;	// expa
	iExpPercent = 0 ;						// expa-BAR

	SetName( pHero->GetName() );
	SetRank( pHero->GetRank() );
	SetStat( pHero->GetStat() );
	SetFlag( pHero->GetPlayerID()+1 ); 

	UpdateMenu();
}
//----------------------------------------------
void	CHERO_MINI_MENU::SetAll( char* name, int rank, int stat, int flag, CHero* pHero ) 
{
	SetAll( pHero );
}
//----------------------------------------------
//void	CHERO_MINI_MENU::setReinforce( bool	stat ){
//	pgptbReinforce->Enabled = stat;
//}
//----------------------------------------------
//void	CHERO_MINI_MENU::setUpgrade( bool	stat ){
//	pgptbUpgrade->Enabled = stat;
//}
//----------------------------------------------
void	CHERO_MINI_MENU::SetAllBtn( bool	stat ){
	//setReinforce( stat );
	//setUpgrade  ( stat );
}
//----------------------------------------------
void	CHERO_MINI_MENU::GetMPos( int& x0, int& y0 ){
	x0 = X0;
	y0 = Y0;
}
//----------------------------------------------
char*	CHERO_MINI_MENU::GetName(){
	return lpszName.pchar();
}
//----------------------------------------------
char*	CHERO_MINI_MENU::GetRank(){
	return lpszRank.pchar();
}
//----------------------------------------------
int		CHERO_MINI_MENU::GetStat(){
	return inStat;
}
//----------------------------------------------
int		CHERO_MINI_MENU::GetFlag(){
	return inFlag;
}
//----------------------------------------------
void	CHERO_MINI_MENU::GetAll( char* name, char* rank, int& stat, int& flag ){
	name = GetName();
	rank = GetRank();
	stat = GetStat();
	flag = GetFlag();
}
//----------------------------------------------
//bool	CHERO_MINI_MENU::getReinforce(){
//	return	pgptbReinforce->Enabled;
//}
//----------------------------------------------
//bool	CHERO_MINI_MENU::getUpgrade(){
//	return	pgptbUpgrade->Enabled;
//}
//----------------------------------------------
void	CHERO_MINI_MENU::ArrangeIcons()
{
	const int dx = 4;
	const int dy = 4;

	setCoordSD( pgppMenuBack, X0 ,		Y0 );										// 694 , 540
	setCoordSD( pgppFlag[0],  X0 +10 ,  Y0 +99 /*pgppMenuBack->x +10 , pgppMenuBack->y1	-32*/	);/*pgppMenuBack->x +10*/  /*X0 +10*/ 

	X0 += 131; 
	Y0 +=   6; //6; 	
	
	setCoordSD( pgptbName,		X0 , 	Y0			);						// X0 = 830   Y0 = 530
	setCoordSD( pgptbRank,		X0 , 	Y0 +16	/*15*/	);
	setCoordSD( pgppStat[0],	X0 ,	Y0 +BIGMAP_STATS_DIST );

	setCoordSD( ptbVictN,		X0 ,	Y0 +54	/*55*/	/*pgppMenuBack->y1	-70*/ /*72*/	);	/* - ( ptbVictN->y1 - ptbVictN->y ) -2*/
	setCoordSD( ptbExpa,		X0 , 	Y0 +68	/*68*/	/*pgppMenuBack->y1	-57*/ /*59*/	);
	setCoordSD( pgppExpBar,		X0 , 	Y0 +82	/*83*/	/*pgppMenuBack->y1	-42*/ /*44*/	);
	setCoordSD( pgppExpBack,	X0 ,	pgppExpBar->y	);

	setCoordSD( ptbSQNum,		X0 ,	Y0 +110		/*pgppMenuBack->y1	-15*/			);

}
//----------------------------------------------
void	CHERO_MINI_MENU::SetStatVisible(bool stat) {
	for ( int i=0; i<MAX_H_STATUS; i++ ){
		if(pgppStat[i])  pgppStat[i]->Visible = false ;
	}
	pgppStat[0]->Visible = stat;
}
//----------------------------------------------
void	CHERO_MINI_MENU::SetFlagVisible(bool stat) {
	for ( int i=0; i<MAX_PLAYERS+1; i++ ){
		if(pgppFlag[i])  pgppFlag[i]->Visible = false;
	}
	//new
	pgppFlag[0]->Visible = stat;
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
//////////// CHERO_PORTRAIT ////////////
////////////////////////////////////////
//----------------------------------------------
CHERO_PORTRAIT::CHERO_PORTRAIT() {
	Init();
	CreateArrays();
}
//----------------------------------------------
CHERO_PORTRAIT::~CHERO_PORTRAIT() {
	DeleteAll();
}
//----------------------------------------------
void	CHERO_PORTRAIT::Init() {
	for( int i=0; i<MAX_PLAYERS; i++ ) {
		pPORTRAIT [i]=NULL;
		pPORTRAITs[i]=NULL;
		//ppPORTbig  [i]=NULL;
		//ppPORTsmall[i]=NULL;
		for(int h=0; h < MAX_HEROES; h++) {//new grey!
			ppPORTbig[i][h] = ppPORTsmall[i][h] = NULL;
		}
	}
}
//----------------------------------------------
void	CHERO_PORTRAIT::CreateArrays() {
	//DeleteSmallP();
	//DeleteBigP();
	//for ( int i=0; i<MAX_PLAYERS; i++ ) {
	//	ppPORTbig  [i]	= new GPPicture*[ MAX_HEROES ];
	//	ppPORTsmall[i]	= new GPPicture*[ MAX_HEROES ];
	//}
}
//----------------------------------------------
void	CHERO_PORTRAIT::DeleteSmallP() {
	//for ( int i=0; i<MAX_PLAYERS; i++ ) {
	//	if ( ppPORTsmall[i]!=NULL ) {
	//		delete[](ppPORTsmall[i]);
	//		ppPORTsmall[i]=NULL;
	//	}
	//}
}
//----------------------------------------------
void	CHERO_PORTRAIT::DeleteBigP() {
	//for ( int i=0; i<MAX_PLAYERS; i++ ) {
	//	if ( ppPORTbig[i]!=NULL ) {
	//		delete[](ppPORTbig[i]);
	//		ppPORTbig[i]=NULL;
	//	}
	//}
}
//----------------------------------------------
void	CHERO_PORTRAIT::DeleteAll() {
	DeleteSmallP();
	DeleteBigP();
}
//----------------------------------------------
void	CHERO_PORTRAIT::addPortrait( LocalGP* pGP, LocalGP* pGPs, int nat ) {
	if ( 0<=nat && nat<=MAX_PLAYERS ) {
		pPORTRAIT [nat]=pGP;
		pPORTRAITs[nat]=pGPs;
	}
}
//----------------------------------------------
void	CHERO_PORTRAIT::addToMenu( DialogsSystem* pMenu ) {
	for ( int i=0; i<MAX_PLAYERS; i++ ) {
		for ( int ii=0; ii<MAX_HEROES; ii++ ) {
			if (pPORTRAIT[i]!=NULL) {
				ppPORTbig  [i][ii] = pMenu->addGPPicture( NULL,0,0,pPORTRAIT[i]->GPID,ii );
				ppPORTbig  [i][ii]->ShapeFileID		= pPORTRAIT[i]->GPID;
				ppPORTbig  [i][ii]->ShapeSpriteID	= ii;
				ppPORTbig  [i][ii]->Visible = false;

				ppPORTsmall[i][ii] = pMenu->addGPPicture( NULL,0,0,pPORTRAITs[i]->GPID,ii );
				ppPORTsmall[i][ii]->ShapeFileID		= pPORTRAITs[i]->GPID;
				ppPORTsmall[i][ii]->ShapeSpriteID	= ii;
				ppPORTsmall[i][ii]->Visible = false;
			}
		}
	}
}
//----------------------------------------------
SimpleDialog*	CHERO_PORTRAIT::getSmallP(int p, int n ){
	if ( ( 0<=p && p<MAX_PLAYERS ) && ( 0<=n && n<MAX_HEROES ) ) {
		return ppPORTsmall[p][n];
	}
	return NULL;
}
//----------------------------------------------
SimpleDialog*	CHERO_PORTRAIT::getBigP(int p, int n ){
	if ( ( 0<=p && p<MAX_PLAYERS ) && ( 0<=n && n<MAX_HEROES ) ) {
		return ppPORTbig[p][n];
	}
	return NULL;
}
//----------------------------------------------
LocalGP*		CHERO_PORTRAIT::getLGPSmallP( int p ){
	if ( 0<=p && p<MAX_PLAYERS ){
		return pPORTRAITs[ p ];
	}
	return NULL;
}
//----------------------------------------------
LocalGP*		CHERO_PORTRAIT::getLGPBigP  ( int p ){
	if ( 0<=p && p<MAX_PLAYERS ){
		return pPORTRAIT[ p ];
	}
	return NULL;
}
//----------------------------------------------
void	CHERO_PORTRAIT::setVisibleS(bool state){
	for ( int i=0; i<MAX_PLAYERS; i++ ){
		for ( int ii=0; ii<MAX_HEROES; ii++ ){
			ppPORTsmall[i][ii]->Visible = state;
		}
	}
}
//----------------------------------------------
void	CHERO_PORTRAIT::setVisibleB(bool state){
	for ( int i=0; i<MAX_PLAYERS; i++ ){
		for ( int ii=0; ii<MAX_HEROES; ii++ ){
			ppPORTbig[i][ii]->Visible = state;
		}
	}
}
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
////////////// CROOTDRIVER /////////////
////////////////////////////////////////
void	CScenarioLV::Init()
{
	// campaign ID
	m_inBigMapCampID	= 0 ; //grey //new

	// MENU
	m_inActivMenu		= 0;
	m_bMenuIsChange		= true;

	// MAP
	m_bMapVisible		= true;
	m_bMapEnable		= true;
	m_bMapMouseMove		= true;

	// SECTORS
	m_bSectVisible				= true;
	m_bSectEnable				= true;
	m_bSectMouseMove			= true;
	m_bSectSetPlayerColor		= true;
	m_bSectMenuVisible			= true;
	m_inLastLMCSecID			= START_SECTOR_ID; //0; //grey //!!!VERY NEW!!!
	m_inSectLastMouseOver		= -1 ; //grey //!!!VERY NEW!!!
	m_inSectNeighbor			= -1;

	// PLAYERS
	m_bPlayerVisible			= true;
	m_bPlayerEnable				= true;
	m_bPlayerHeroCapture		= false;
	m_inPlayerInUseHero			= -1;
	m_inPlayerInUseHero			= 0;
	m_bHeroMiniMap				= true;

	// PERSONAL HERO MENU
	m_bPERSONAL_HERO_MENU		= false;

	// BRIEFING
	m_bBriefingVisible			= false;
	m_bBriefingEnabled			= false;
	m_inAttackedSector			= -1;	// /* ...убрать!... */
	
	// DIPLOMACY	
	m_bDiplVisible				= false;
	m_inMapButtonPressed		=  2; //1; //VERY NEW
	m_inDiplActionPressed		= -1;

	//info
	m_bInfoVisible				= true;
    m_inInfoButtonPressed		= -1;

	// MESSAGES
	m_bMessagesVisible			= false;

	// MARKET
	m_bMarketVisible			= false;

	// HELP
	m_bHelpVisible				= false;

}
//----------------------------------------------

//**********************************************************************************
///////////////////////////////////////////////////////////
//////////////   CROOT_CONST_DATA   ///////////////////////
///////////////////////////////////////////////////////////
//----------------------------------------------
CROOT_CONST_DATA::CROOT_CONST_DATA() {
	Init();
}
//----------------------------------------------
CROOT_CONST_DATA::~CROOT_CONST_DATA() {
	Init();
}
//----------------------------------------------
void	CROOT_CONST_DATA::Init()
{
	m_inAttackedNation	 = -1; 
	m_inCurTurn			 = m_inCurActiveAI = m_inCurHeroMove = 0;
	m_iBigMap_FOOD_RATIO = m_iBigMap_User_RESOURCES = m_iBigMap_AI___RESOURCES = 0;  

	FormKilledNum	= FormKilledNum2 = 0;
	QuestEXP		= QuestNum = QuestComplete = QuestSQNum = 0; 
	for(int i=0; i < 10 ; i++)				IncomeRes[i] = IncomeRes2[i] = AddAIRes[i] = 0;
	for(int i=0; i < DIFF_LEVELS_N ; i++)	UpdateResDiv[i] = 0;
	
	int  SQNN[5] = { 4, 6, 12, 18, 18 };
	for(int i=0; i <= MAX_DEFENCE ; i++) {
		m_iGarrSqN[i]  = SQNN[i];	// Squads Num for Defence Level
		m_iGarrAdd[i]  =  4;
		m_iRebelAdd[i] = 10;
	}

	m_iMinSecDefPower	= 11;
	m_iAddSecDefPower	=  2;

	char*  natsID[50]			= {  "(EN)", "(FR)", "(AU)", "(PR)", "(RU)", "(EG)", "(NE)", 0  };
	int    a_NationalColors[]	= {  0xFFA50000, 0xFF003CC6, 0xFF29B694, 0xFF9C49B5, 
									 0xFFF78610, 0xFF292839, 0xFFE7E3E7, 0xFF6B4110	 };
	for(int i=0; i < MAX_PLAYERS+1 ; i++) {
		m_iNationID[i]  = 0;
		m_chNationID[i] = natsID[i];
		m_iMapColor[i]  = ALPHA_SECT + a_NationalColors[i];
	}
	m_CheatsStage = 0;
}	
//----------------------------------------------
//----------------------------------------------

////////////////////////////////////////////////////////////
//////////////   CROOTDRIVER   /////////////////////////////
////////////////////////////////////////////////////////////
CROOTDRIVER::CROOTDRIVER() {
	Init();
}
//----------------------------------------------
CROOTDRIVER::~CROOTDRIVER() {
	Init();
}
//----------------------------------------------
void	CROOTDRIVER::Init()
{
	m_bSaveLogs = false;	//  true;		//false;		// 
	m_bEndGame	= false;

	m_ShowHeroMove0   = true;
	m_EnabledMapClick = true;

	m_pMenu			= NULL;
	m_pMap			= NULL;
	m_pSect			= NULL;
	m_pSectMenu		= NULL;
	m_pDipl			= NULL;
	m_pMessages		= NULL;
	m_pHmm			= NULL;
	m_pHp			= NULL;
	m_pHPers		= NULL;
	m_pBriefing		= NULL;
	m_phpDesk		= NULL;
	m_pPLAYER_SAVE	= NULL;
	for(int i=0; i < MAX_PLAYERS ; i++)  m_pPlayer[i] = NULL;

	m_Scenario.Init();
	CROOT_CONST_DATA::Init();
}
//----------------------------------------------
void	CROOTDRIVER::SetMenuEnabled( bool state )
{
	m_pResPanel->SetEnabled( state );
	m_pDipl->m_pActButtons->MainMenuEnabled(m_pSect, state , m_Scenario.m_inActivMenu);
	for(int i=0; i < MAX_PLAYERS ; i++)   
		if( m_pPlayer[i] && m_pPlayer[i]->m_Alive )  m_pPlayer[i]->SetEnabled( state );

	// europe map
	if( m_Scenario.m_inActivMenu == 0 ) 
	{
		m_pSect->SetEnable( state ); 
		m_pSectMenu->SetEnabledState( state );
		m_pSectMenu->SetEnabled( state );
		m_pHmm->SetEnabled( state );
	}
	//diplomacy
	if( m_Scenario.m_inActivMenu == 1 ) 
	{
		m_pSect->SetEnable( state ); 
		m_pDipl->m_pMapButtons->SetEnabled( state );
		m_pDipl->m_pInfoTexts->SetEnabled( state );//new

		if( !state )  m_pDipl->m_pActButtons->SetDisabledFont();
		m_pDipl->m_pActButtons->SetEnabled( state );
		if( state )   m_pDipl->m_pActButtons->SetDisabledFont();
	}
	// commanders
	if( m_Scenario.m_inActivMenu == 2 )
	{
		int secOwner = m_Scenario.m_inLastHeroClick/100 ;
		if( !state )	m_phpDesk->SetEnabled( false , MAX_HEROES );		
		if( state )		m_phpDesk->SetEnabled( true  , m_pPlayer[ secOwner ]->m_inHeroNum );

		m_pHPers->setEnabledButtons( state );
	}
	// market
	if( m_Scenario.m_inActivMenu == 3 )
	{
		m_pMarket->SetEnabled( state );
	}
	// messages
	if( m_Scenario.m_inActivMenu == 4 )
	{
		m_pMessages->m_pInform->SetEnabled( state );
	}
}
//----------------------------------------------
int		CROOTDRIVER::GetFreePlayerPos(){
	int pos = -1;
	for (int i=0; (i<MAX_PLAYERS)&&(pos==-1); i++){
		if (m_pPlayer[i]==NULL){
			pos = i;
		}
	}
	return pos;
}//int		GetFreePlayerPos();	
//----------------------------------------------

void	CROOTDRIVER::PushHeroOnTop2()//last add!!!
{
	int	 ppIDSec[10000];			
	for(int i=0; i < 10000; i++)  ppIDSec[i] = 0;

	// Поиск индексов соответствующих указателям.
	int	HNUMBER = 0, ii=0, di=0;

	for(int i=0; i < m_pSect->m_inNewArrowsNum ; i++) {
		int  nid = 2*i;
		ppIDSec[ nid ] = reinterpret_cast<int>( m_pSect->m_pgppNewArrowsShad[i] );
		ppIDSec[nid+1] = reinterpret_cast<int>( m_pSect->m_pgppNewArrows[i] );
	}
	HNUMBER += 2*m_pSect->m_inNewArrowsNum;

	//int HIDS[MAX_PLAYERS*MAX_HEROES];

	int CurID	= -1;
	int LastID	= -1;

	for(int i = HNUMBER-1 ; i >= 0 ; i--) 
	{
		LastID = CurID = -1;

		di = m_pMenu->DSS.GetAmount()-1;
		while( di >= 0 )
		{
			if( LastID < 0 ) {
				if( (reinterpret_cast<SimpleDialog*>(ppIDSec[ i ])) == m_pMenu->DSS[ di ] )
					LastID = di ;
			}
			else {
				for(int pla=0; pla < MAX_PLAYERS && CurID < 0 ; pla++)
					for(int h=0; h < m_pPlayer[pla]->m_inHeroNum ; h++)
						if( m_pPlayer[pla]->m_phrHero[h].getMapPicPt() == m_pMenu->DSS[ di ] ) 
							CurID = di ;
			}

			di--;
			if( LastID != -1 && CurID != -1 )
				break;
		}

		if( CurID != LastID && CurID < LastID && LastID != -1 && CurID != -1 ) {
			SimpleDialog* pSD		=	m_pMenu->DSS[CurID];
			m_pMenu->DSS[CurID]		=	m_pMenu->DSS[LastID];
			m_pMenu->DSS[LastID]	=	pSD;
		}
	}
}
	//int CurID	= 0;
	//int LastID	= 0;

	//int HIDS[MAX_PLAYERS*MAX_HEROES];
	//i = m_pMenu->DSS.GetAmount();
	//while( i >= 0 ) { 					/*m_pMenu->DSS[i]!=NULL ) {*/

	//	for(int pla=0; pla < MAX_PLAYERS ; pla++) {
	//		for(int h=0; h < m_pPlayer[pla]->m_inHeroNum ; h++) {
	//			HIDS[pla][h] = -1;
	//			if( m_pMenu->DSS[i] == m_pPlayer[pla]->m_phrHero[h].getMapPicPt() ) 
	//			{
	//				HIDS[pla][h] = i;
	//			}
	//		}
	//	}

	//	if( m_pMenu->DSS[i] == psdHero ) {
	//		CurID = i;
	//	}
	//	for(ii=0; ii < HNUMBER-1 ; ii++) {
	//		if( (reinterpret_cast<SimpleDialog*>(ppIDSec[ii])) == m_pMenu->DSS[i] ) {
	//			LastID = i;
	//		}
	//	}
	//	i--;
	//}
	//i=0;
	//while( i < m_pMenu->DSS.GetAmount() ) {					/*m_pMenu->DSS[i]!=NULL ) {*/
	//	if( m_pMenu->DSS[i] == psdHero ) {
	//		CurID = i;
	//	}
	//	for(ii=0; ii < HNUMBER-1 ; ii++) {
	//		if( (reinterpret_cast<SimpleDialog*>(ppIDSec[ii])) == m_pMenu->DSS[i] ) {
	//			LastID = i;
	//		}
	//	}
	//	i++;
	//}

	//if( CurID != LastID ) {
	//	m_pMenu->DSS[CurID]		=	m_pMenu->DSS[LastID];
	//	m_pMenu->DSS[LastID]	=	psdHero;
	//}
//}

//----------------------------------------------

void	CROOTDRIVER::PushHeroOnTop(SimpleDialog* psdHero)
{
	// Поиск индексов соответствующих указателям.
	int	HNUMBER = 0;
	int i=0,ii=0;
	for(i=0; i < MAX_PLAYERS ; i++){
		HNUMBER += m_pPlayer[i]->m_inHeroNum;
	};

	int	 ppIDSec[10000];			for(int i=0; i < 10000; i++)  ppIDSec[i] = 0;//new

	for(i=0; i < MAX_PLAYERS ; i++) {
		for(ii=0; ii < m_pPlayer[i]->m_inHeroNum ; ii++) {
			ppIDSec[i+ii] = reinterpret_cast<int>(m_pPlayer[i]->m_phrHero[ii].getMapPicPt());
		}
	}
	for(int i=0; i < m_pSect->m_inNewArrowsNum ; i++) {//new
		int  nid = HNUMBER + 2*i;
		ppIDSec[ nid ] = reinterpret_cast<int>( m_pSect->m_pgppNewArrowsShad[i] );
		ppIDSec[nid+1] = reinterpret_cast<int>( m_pSect->m_pgppNewArrows[i] );
		//ppIDSec[ nid ] = reinterpret_cast<int>( m_pSect->m_pgppNewArrows[i] );
		//ppIDSec[nid+1] = reinterpret_cast<int>( m_pSect->m_pgppNewArrowsShad[i] );
	}
	HNUMBER += 2*m_pSect->m_inNewArrowsNum;//new

	int CurID	= 0;
	int LastID	= 0;

	i=0;
	while( i < m_pMenu->DSS.GetAmount() /*m_pMenu->DSS[i]!=NULL*/) {
		if( m_pMenu->DSS[i] == psdHero ) {
			CurID = i;
		}
		for(ii=0; ii < HNUMBER-1 ; ii++) {
			if( (reinterpret_cast<SimpleDialog*>(ppIDSec[ii])) == m_pMenu->DSS[i] ) {
				LastID = i;
			}
		}
		i++;
	}

	if( CurID != LastID ) {
		m_pMenu->DSS[CurID]		=	m_pMenu->DSS[LastID];
		m_pMenu->DSS[LastID]	=	psdHero;
	}
}

//----------------------------------------------
bool	CROOTDRIVER::SetHeroMiniMenuVisible(bool state)
{
	for (int i=0; i<MAX_PLAYERS ; i++)
		this->m_pPlayer[i]->SetHeroMiniMenuVisible(state);
	return state;
}
//----------------------------------------------
bool	CROOTDRIVER::operator +=(DialogsSystem*		pMenu){
	if (pMenu!=NULL){
		m_pMenu = pMenu;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator +=(CPicesPict*		pMap){
	if (pMap!=NULL){
		m_pMap = pMap;
		return true;
	}
	return false;
}//bool	operator+=	(CPicesPict* pMap   );
//----------------------------------------------
bool	CROOTDRIVER::operator +=(CSectData*			pSect){
	if (pSect!=NULL){
		m_pSect = pSect;
		return true;
	}
	return false;
}//bool	operator+=	(CSectData*  pSect  );
//----------------------------------------------
bool	CROOTDRIVER::operator+=	(CSectorMenu*		pSectMN){
	if (pSectMN!=NULL){
		m_pSectMenu = pSectMN;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator +=(CPlayer*			pPlayer){
	if (pPlayer!=NULL){
		int pos = GetFreePlayerPos();
		if (pos!=-1){
			m_pPlayer[pos] = pPlayer;
			return true;
		}
    }
	return false;
}//bool	operator+=	(CPlayer*	 pPlayer);
//----------------------------------------------
bool	CROOTDRIVER::operator +=(CDiplMenu*			pDipl){
	if (pDipl!=NULL){
		m_pDipl = pDipl;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator+=	(CDiplInfoMessages*	pMessages){ 
	if (pMessages!=NULL){
		m_pMessages = pMessages;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator +=(CHERO_MINI_MENU*	pHMM){
	if (pHMM!=NULL) {
		m_pHmm = pHMM;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator +=(CHERO_PORTRAIT*	pHp){
	if ( pHp!=NULL ) {
		m_pHp = pHp;
		return	true;
	}
	return	false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator+=	(CPERSONAL*			pHP  ){
	if (pHP!=NULL) {
		m_pHPers = pHP;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator+=	(CBriefing*			pBriefing) { 
	if (pBriefing!=NULL){
		m_pBriefing = pBriefing;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator+=	( CHeroPersDesk*	phpDesk ) {
	if (phpDesk!=NULL){
		m_phpDesk = phpDesk;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator+=	( CBigMapQuest*		pQuest ) {
	if( pQuest ) {
		m_pQuest = pQuest;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator+=	( CResPanel_BM*		pResPanel ) {
	if( pResPanel ) {
		m_pResPanel = pResPanel;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator+=	( CMarketDesk*		pMarket ) { 
	if( pMarket ) {
		m_pMarket = pMarket;
		return true;
	}
	return false;
}
//----------------------------------------------
bool	CROOTDRIVER::operator+=	( CBigMapHelp*		pHelp ) { 
	if( pHelp ) {
		m_pHelp = pHelp;
		return true;
	}
	return false;
}
//----------------------------------------------
void	CROOTDRIVER::UPDATE_SCENARIO()
{
	// ПАНЕЛЬ РЕСУРСОВ
	m_pResPanel->Refresh( m_pPlayer , gi_bmSAVE_Nation );

	m_pMap->SetVisible(m_Scenario.m_bMapVisible);
	m_pMap->SetEnable (m_Scenario.m_bMapEnable && this->m_EnabledMapClick );	//m_pMap->SetEnable(m_Scenario.m_bMapEnable);

	m_pSect->SetVisible(m_Scenario.m_bSectVisible);
	m_pSect->SetEnable ( m_Scenario.m_bSectEnable && this->m_EnabledMapClick );	//m_pSect->SetEnable(m_Scenario.m_bSectEnable);
	m_pSect->SetSelectedColor( m_Scenario.m_inLastLMCSecID , m_Scenario.m_bMapVisible );

	//m_pSectMenu->SetVisible( m_Scenario.m_bSectMenuVisible );	//grey comment
	m_pSectMenu->m_inCurSectID = -1; //grey 
	m_pSectMenu->SetMenuData( m_Scenario.m_inLastLMCSecID , m_pPlayer ); //grey 

	m_pHmm->UpdateMenu();

	//HERO MINI MENU
	if( m_Scenario.m_bHeroMiniMap ) {
		m_pHmm->CheckVisible();
	}
	//else{
	//	m_pHmm->SetVisible(false);
	//	m_pHp->setVisibleS(false); // HHHH-comment
	//}

	m_pHPers->setVisible(false);
	m_pHPers->UpdateData(m_Scenario.m_inLastHeroClick%100);
	m_pHPers->UpdateCoord();
	m_pHPers->setVisible(m_Scenario.m_bPERSONAL_HERO_MENU);

	for (int i=0; i < MAX_PLAYERS ; i++) {
		m_pPlayer[i]->SetVisible( m_Scenario.m_bPlayerVisible );
		m_pPlayer[i]->SetEnable ( m_Scenario.m_bPlayerEnable  );
		if (m_Scenario.m_inActivMenu!=3) {
			//m_pPlayer[i]->SetPortHVisible(false);
		}
		else {
			if ( i == (m_Scenario.m_inLastHeroClick/100) ) {
			//	m_pPlayer[i]->ShowPortH();
			}
		}
		//if (m_Scenario.m_bSectMenuVisible && (m_Scenario.m_inLastHeroClick/100)==i){ // HHHH-comment UNDO
		if (m_Scenario.m_bHeroMiniMap && (m_Scenario.m_inLastHeroClick/100)==i) { // HHHH-comment
			m_pPlayer[i]->TryShowSPortret(m_Scenario.m_inLastHeroClick%100);
		}		
	}
	int secOwner = m_Scenario.m_inLastHeroClick/100;
	m_phpDesk->m_Visited = false;
	m_phpDesk->Show( m_pPlayer , m_pPlayer[ secOwner ]->m_phrHero , m_pPlayer[ secOwner ]->m_inHeroNum , 
					 m_Scenario.m_inLastHeroClick%100 , m_Scenario.m_bPERSONAL_HERO_MENU );

	// update hints
	m_pSect->CreateHint(); // хинты по секторам	
	for(int i=0 ; i < MAX_PLAYERS ; i++) 
		for(int h=0; h < m_pPlayer[i]->m_inHeroNum ; h++) {
			m_pPlayer[i]->m_phrHero[h].CreateHint(); // хинты по героям
		}
	//
	if( ROOT_SCENARIO.m_Scenario.m_bDiplVisible )  m_pDipl->m_pActButtons->CreateHint(); //grey 
	//
}
//----------------------------------------------
void	CROOTDRIVER::PROCESS(int& g_dXM,int& g_dYM,int& g_WM,int& g_HM, int playerNation, int Diff )
{
	// проверка горячих клавиш в меню!
	CheckHotKeys();

	//заглушка для break-point
	if (m_Scenario.m_bDiplVisible) {
		int strf = 0;
	}

	// processing data
	if (m_Scenario.m_bPlayerHeroCapture){
		m_pMap->ScrollPic(mouseX,mouseY);
	}
	else{
		m_pMap->MoveWithMouse();
	}
	m_pMap->GetMapSize(g_dXM,g_dYM,g_WM,g_HM);

	m_pSect->DeposeTo(g_dXM,g_dYM,m_Scenario.m_bSectSetPlayerColor);
	m_pSect->UPDATE();
	if (m_Scenario.m_bPlayerHeroCapture){
		m_Scenario.m_inSectLastMouseOver = m_pSect->GetMouseOverSect();
	}
	
	for (int i=0; i<MAX_PLAYERS; i++) 
		if( m_pPlayer[i]->m_Alive ) {
			if( m_Scenario.m_inSectLastMouseOver!=-1 && m_Scenario.m_bPlayerHeroCapture==false ) {
				m_pPlayer[i]->UpdateSectID();
				m_Scenario.m_inSectLastMouseOver = -1;
			}
			m_pPlayer[i]->UpdatePic();
	} 

	m_pSect->setArrowVisible(false);
	if( m_Scenario.m_inSectNeighbor != -1 && m_Scenario.m_bMapVisible ) {
		//m_pSect->ViewNeighbor(m_Scenario.m_inSectNeighbor,m_Scenario.m_inPlayerInUseHero,m_pPlayer[m_Scenario.m_inPlayerInUseHero]);
		m_pSect->ViewNeighbor(g_dXM,g_dYM,m_Scenario.m_inSectNeighbor,m_Scenario.m_inPlayerInUseHero,m_pPlayer[m_Scenario.m_inPlayerInUseHero]);
	}

	//Sectors menu.
	if (m_Scenario.m_bSectMenuVisible) {
		//m_pSectMenu->SetMenuData(m_Scenario.m_inLastLMCSecID);
		m_pSectMenu->SetMenuData( m_Scenario.m_inLastLMCSecID , m_pPlayer );//grey
        m_pSectMenu->CheckDefenceButton(m_pDipl,m_pPlayer[playerNation],m_Scenario.m_inLastLMCSecID,playerNation);
		m_pSectMenu->CheckDiversionButton(m_pDipl,m_pPlayer[playerNation],m_Scenario.m_inLastLMCSecID,playerNation);
	}
	else {
		m_pSectMenu->SetVisible(false);
		//m_pHp->setVisibleS(false); // HHHH-comment UNDO
	}

	//HERO MINI MENU
	if (m_Scenario.m_bHeroMiniMap) {
		m_pHmm->CheckVisible();
	}
	else{
		m_pHmm->SetVisible(false);
		m_pHp->setVisibleS(false); // HHHH-comment
	}


	// INFO desk
	m_pMessages->ShowMessageDesk( m_Scenario.m_bMessagesVisible );
	if( m_Scenario.m_bMessagesVisible ) {
		m_pMessages->Process( m_Scenario.m_inInfoButtonPressed );
		m_Scenario.m_inInfoButtonPressed = -1;
	}
    
	// DIPLOMACY menu
	bool DiplActEnable = (m_Scenario.m_inAttackedSector<0 && m_pSect->GetSecOwner(m_Scenario.m_inLastLMCSecID)!=m_pDipl->m_pDiplData->GetPlayerNId() ); 
	m_pDipl->Show( m_Scenario.m_bDiplVisible , DiplActEnable );
	if( m_Scenario.m_bDiplVisible ) {
		m_pDipl->Process(&m_Scenario.m_inMapButtonPressed,m_Scenario.m_inDiplActionPressed,m_Scenario.m_inActivMenu,m_Scenario.m_bMenuIsChange, DiplActEnable );
		m_Scenario.m_inDiplActionPressed = -1;
	}

	// BRIEFING menu
	//m_pBriefing->Refresh(m_Scenario.m_bBriefingVisible, m_Scenario.m_inAttackedSector);

	// PERSONAL menu - UNITS Pages
	if( m_Scenario.m_bPERSONAL_HERO_MENU ) { 
		if( m_pHPers->CheckUnitsPages() )   m_pHPers->UpdateData( m_Scenario.m_inLastHeroClick%100 ) ;
	}
	// personal heroes desk
	int secOwner = m_Scenario.m_inLastHeroClick/100 ;
	m_phpDesk->Show( m_pPlayer , m_pPlayer[ secOwner ]->m_phrHero , m_pPlayer[ secOwner ]->m_inHeroNum , 
					 m_Scenario.m_inLastHeroClick%100 , m_Scenario.m_bPERSONAL_HERO_MENU ) ;

	// MARKET desk
	m_pMarket->Show( m_pPlayer , playerNation , m_Scenario.m_bMarketVisible ) ;

	// Show selected sector
	m_pSect->SetSelectedColor(m_Scenario.m_inLastLMCSecID, m_Scenario.m_bMapVisible && !m_Scenario.m_bDiplVisible );
	
	// ПАНЕЛЬ РЕСУРСОВ
	m_pResPanel->Refresh( m_pPlayer , playerNation );

	// Help Window 
	if( m_pHelp ) m_pHelp->Show( m_Scenario.m_inActivMenu, m_Scenario.m_bHelpVisible ) ;
}
//----------------------------------------------
CHero*	CROOTDRIVER::GetHeroInSect( int sid ,int NOT_NAT )
{
	CHero* pH = NULL;
	for(int i=0; i < MAX_PLAYERS && pH == NULL ; i++)
		if( i != NOT_NAT ) {
			pH = m_pPlayer[i]->GetHeroInSector( sid );
		}
	return	pH;
}
//----------------------------------------------

bool	CROOTDRIVER::CheckRightsForHeroInSector(CHero* pHero) //new
{
	if( !pHero ) return false;

	int secOwner = ROOT_SCENARIO.m_pSect->GetSecOwner( pHero->m_inSectorID ) ;
	if( pHero->m_inPlayerID == secOwner )									return true;

	int PASS = m_pDipl->m_pDiplData->GetPassId();
	if( m_pDipl->CheckContract( PASS , pHero->m_inPlayerID , secOwner ) )	return true;

	return false;
}

//----------------------------------------------
void	CROOTDRIVER::ShowInGameSector()
{
	ShowInGameSector( this->m_Scenario.m_inLastLMCSecID , 0 , false , false );
	//ShowInGameSector( this->m_Scenario.m_inLastLMCSecID , 0 , false );
}
//----------------------------------------------
void	CROOTDRIVER::ShowInGameSector( int secID , float time , bool standard /*= true*/ , bool slow /*= true*/ )
{
	if( secID < 0 || secID >= m_pSect->GetSectorsNum() )	{ return; }
	if( time < 0 )  standard = false;

	// подготовка - сохр. предыдущее состояние
	//int		i_LastLMCSecID	= m_Scenario.m_inLastLMCSecID;
	//int		i_LastHeroClick	= m_Scenario.m_inLastHeroClick;
	//bool	b_HeroMiniMap	= m_Scenario.m_bHeroMiniMap ;

	m_Scenario.m_inLastLMCSecID	= secID ;

	if( m_Scenario.m_inActivMenu == 0 ) {
		//m_Scenario.m_bHeroMiniMap
		m_Scenario.m_bHeroMiniMap = false;
		//
		CHero*	pH = GetHeroInSect( secID , 0xFFFF );
		if( pH && pH->GetPlayerID() != m_inCurActiveAI &&  standard ) pH = GetHeroInSect( secID , pH->GetPlayerID() );	
		if( pH ) {
			ON_HERO_R_CLICK_VIRTUAL( pH );
			m_Scenario.m_bHeroMiniMap	 = true;
			m_Scenario.m_inLastHeroClick = pH->GetHeroID() + pH->GetPlayerID()*100 ;
		}
		//else { m_Scenario.m_bHeroMiniMap = false; }
	}



// *** FOR VITYA CORRECTION ***
	// put sector to center of map
	int  a_inMW = 600 ,  a_inMH = 530; // 600x531

	if( slow )
	{
		int curX=0; int t0,t1;
		int	curY=0;
		m_pMap->GetMapSize(curX,curY,t0,t1);
		int  secX  = 0 ,  secY  = 0 ;												
		m_pSect->GetSecSityXY( secID , secX , secY );

		int  dMX = a_inMW/2 - secX + 75/* - t0*/;	
		int  dMY = a_inMH/2 - secY + 113/* - t1*/;	

		// просто скролим куда нибудь, потом разберемся с координатами
		float tdx = dMX-curX;	float xsign = ( (tdx>=0.0f) ? (1.0f) : (-1.0f) );
		float tdy = dMY-curY;	float ysign = ( (tdy>=0.0f) ? (1.0f) : (-1.0f) );
		float delta = 10.0f;
		float tdXY = sqrt( tdx*tdx + tdy*tdy );
		bool	move = (bool)( tdXY > delta );
		if ( move ){
			float deltaX = (fabs(tdx)/tdXY)*delta;
			float deltaY = (fabs(tdy)/tdXY)*delta;
			DWORD	lasttime	= GetTickCount();
			float xTEIL = 0.0f;
			float yTEIL = 0.0f;
			while ( move && (xsign!=0.0f||ysign!=0.0f) ) {
				realRpressed=false;
				realLpressed=false;

				if ( GetTickCount()-lasttime > 35 ) {
					lasttime = GetTickCount();

					move = m_pMap->SetPosition( (int)(xsign*deltaX+xTEIL), (int)(ysign*deltaY+yTEIL) );
					
					xTEIL = (xsign*deltaX+xTEIL) - (int)(xsign*deltaX+xTEIL);
					yTEIL = (ysign*deltaY+yTEIL) - (int)(ysign*deltaY+yTEIL);

					UPDATE_SCENARIO();
					PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , pSAVE->m_iDifficulty );

					m_pMenu->ProcessDialogs();    
					m_pMenu->RefreshView();

					m_pMap->GetMapSize(curX,curY,secX,secY);
					if ( abs(curX-dMX) < deltaX )	xsign = 0.0f;
					if ( abs(curY-dMY) < deltaY )	ysign = 0.0f;
				};
			};
		};
		
		m_pMap->GetMapSize(curX,curY,secX,secY);
		m_pMap->SetPosition( dMX-curX, dMY-curY );
	}
	else {
		// установка сектора игрока по центру экрана	(сектор героя игрока (0-го))
		int  secX = 0, secY = 0; /*secID = m_Scenario.m_inLastLMCSecID,  */
		m_pSect->GetSecSityXY( secID , secX , secY );

		int  dMX = a_inMW / 2  - secX ;			
		int  dMY = a_inMH / 2  - secY ;
		m_pMap->SetPosition( 1000 , 1000 );
		m_pMap->SetPosition( dMX , dMY );
	}

	UPDATE_SCENARIO();
	PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , 0 );

	m_pMenu->ProcessDialogs();    m_pMenu->RefreshView();


	if( standard ) {
		if( time == 0 )		{ BigMapPause(); }  
		else				{ BigMapPause( time ); } 
	}
// *** FOR VITYA CORRECTION ***



	//m_Scenario.m_inLastLMCSecID		= i_LastLMCSecID;
	//m_Scenario.m_inLastHeroClick	= i_LastHeroClick;
	//m_Scenario.m_bHeroMiniMap 		= b_HeroMiniMap;
}
//----------------------------------------------
bool	CROOTDRIVER::ShowHeroMove( CHero* pHero , int fromSecID , int toSecID )
{
	if( !pHero )											return false;
	
	if( fromSecID == toSecID ) {
		return true;//new
	}

	int  NatID = pHero->GetPlayerID(); // hero nation
	int  SecID = pHero->getSectorID(); // from Sector
	if( fromSecID >= 0 )  SecID = fromSecID;

	if( NatID < 0 || NatID >= MAX_PLAYERS )					return false;
	if( SecID < 0 || SecID >= m_pSect->GetSectorsNum() )	return false;

	// подготовка - сохр. предыдущее состояние
	//int		i_LastLMCSecID	= m_Scenario.m_inLastLMCSecID;
	//int		i_LastHeroClick	= m_Scenario.m_inLastHeroClick;
	//bool	b_HeroMiniMap	= m_Scenario.m_bHeroMiniMap ;

	//////new
	//////m_Scenario.m_inLastLMCSecID	= SecID ;
	//////m_Scenario.m_bHeroMiniMap	= true ;

	
	//
	m_Scenario.m_inLastLMCSecID	  = SecID ;
	ON_HERO_R_CLICK_VIRTUAL( pHero );
	//m_Scenario.m_bHeroMiniMap	  = true ;
	//


	//m_Scenario.m_bHeroMiniMap = false;
	//if( m_ShowHeroMove0 )
	//{
	//	m_ShowHeroMove0 = false;
	//	CHero*	pHero = GetHeroInSect( fromSecID, 0xFFFF ); 
	//	if(pHero) { 
	//		ON_HERO_R_CLICK_VIRTUAL( pHero ); 
	//		m_Scenario.m_bHeroMiniMap = true;
	//	}
	//	else { SetHeroMiniMenuVisible(false); }
	//}
	//else {
	//	m_ShowHeroMove0 = true;
	//}


	//UPDATE_SCENARIO();
	UPDATE_SCENARIO();
	PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , 0 );

	//m_Scenario.m_inLastLMCSecID		= i_LastLMCSecID ;
	//m_Scenario.m_inLastHeroClick	= i_LastHeroClick ;
	//m_Scenario.m_bHeroMiniMap 		= b_HeroMiniMap;

	//////new


	int  x = 0, y = 0, a = 0, snid = 0;
	int  dx = g_dXM ,  dy = g_dYM ;

	int  secOffset = m_pSect->m_pssdDT[SecID]->GetNewArrowOffset();
	for (int i=0; i < m_pSect->m_pssdDT[SecID]->GetNNeighbor() ; i++)
	{
		if( toSecID != m_pSect->GetNeighborID( SecID, i ) ) { continue ; } 

		m_pSect->m_pssdDT[SecID]->GetAroowCoord(x,y,a,i);

		if( secOffset+i >= m_pSect->m_inNewArrowsNum ) { continue ; } 
		m_pSect->m_pgppNewArrows[secOffset+i]->x		= x + dx;		m_pSect->m_pgppNewArrows[secOffset+i]->y	    = y + dy;
		m_pSect->m_pgppNewArrowsShad[secOffset+i]->x	= x + dx + 2;	m_pSect->m_pgppNewArrowsShad[secOffset+i]->y	= y + dy + 5;
				
		if( m_pSect->m_pssdDT[toSecID]->GetDeffOwner() != NatID && CanAttackSector( NatID , toSecID ) ) {
			m_pSect->m_pgppNewArrows[secOffset+i]->Diffuse		= 0xFF000000+RGB(0,0,230);
			m_pSect->m_pgppNewArrowsShad[secOffset+i]->Diffuse	= 0x55000000+RGB(0,0,0);
		} else {
			m_pSect->m_pgppNewArrows[secOffset+i]->Diffuse		= 0xFF000000+RGB(0,195,0);
			m_pSect->m_pgppNewArrowsShad[secOffset+i]->Diffuse	= 0x55000000+RGB(0,0,0);
		}
		m_pSect->m_pgppNewArrows[secOffset+i]->Visible		= true;
		m_pSect->m_pgppNewArrowsShad[secOffset+i]->Visible	= true;

		m_pSect->m_pgppNewArrows[secOffset+i]->x1	  = m_pSect->m_pgppNewArrows[secOffset+i]->x + GPS.GetGPWidth(m_pSect->m_pgppNewArrows[secOffset+i]->FileID,m_pSect->m_pgppNewArrows[secOffset+i]->SpriteID&4095)-1;
		m_pSect->m_pgppNewArrows[secOffset+i]->y1	  = m_pSect->m_pgppNewArrows[secOffset+i]->y + GPS.GetGPHeight(m_pSect->m_pgppNewArrows[secOffset+i]->FileID,m_pSect->m_pgppNewArrows[secOffset+i]->SpriteID&4095)-1;
		m_pSect->m_pgppNewArrowsShad[secOffset+i]->x1 = m_pSect->m_pgppNewArrowsShad[secOffset+i]->x + GPS.GetGPWidth(m_pSect->m_pgppNewArrowsShad[secOffset+i]->FileID,m_pSect->m_pgppNewArrowsShad[secOffset+i]->SpriteID&4095)-1;
		m_pSect->m_pgppNewArrowsShad[secOffset+i]->y1 = m_pSect->m_pgppNewArrowsShad[secOffset+i]->y + GPS.GetGPHeight(m_pSect->m_pgppNewArrowsShad[secOffset+i]->FileID,m_pSect->m_pgppNewArrowsShad[secOffset+i]->SpriteID&4095)-1;

		m_pMenu->ProcessDialogs();    m_pMenu->RefreshView();
		BigMapPause( BM_SHOW_SECPAUSE/2 ); // 0.5 );
		
		return true;
	}
	return false;
}
//----------------------------------------------
int		CROOTDRIVER::FindNearEnemySector( int iCS0, int AI_NI , int MyPower )
{
	int   iCAS   = 0xFFFF;									// Текущий атакуемый сектор.
	int   iDef   = 0xFFFF ;									// Defence атакуемого сектора. 
	int   diff   = pSAVE->m_iDifficulty;

	int   NN     = m_pSect->GetNeighborsNum( iCS0 );		// Кол-во соседей у сектора плацдарма.
	int	  WAR	 = m_pDipl->m_pDiplData->GetWarId(); 
	int	  PEACE	 = m_pDipl->m_pDiplData->GetPeaceId(); 
	int	  PACT	 = m_pDipl->m_pDiplData->GetPactId(); 
	int	  PASS	 = m_pDipl->m_pDiplData->GetPassId(); 
	int	  ALLY	 = m_pDipl->m_pDiplData->GetAllyId(); 
	int   types[4] = { PEACE, PACT, PASS, ALLY };		
	int   allies[100] ,  allyN = m_pDipl->GetContractsById( ALLY , AI_NI , allies );
	bool  MAD_ON = true;

	for(int sec=0; sec < NN ; sec++) 
	{	
		int  pNID  = m_pSect->GetNeighborID( iCS0, sec );
		int  owner = m_pSect->GetSecOwner( pNID );
		if( owner == AI_NI || !CanEnterIntoSector( AI_NI , pNID ) )  { continue; }
		if( m_inCurTurn > 1 && m_pDipl->GetContractPeriod(PEACE, AI_NI, owner ) == MAX_CONT_VALPERIOD ) {
			continue; } // PEACE declared just now 

		bool find  = false;
		for(int i=0; i<4&&!find ;i++) if(m_pDipl->m_pDiplData->GetRequestId(types[i], AI_NI, owner) >= 0) find = true;

		if( !find && !m_pDipl->CheckContract( PACT , owner , AI_NI ) ) 
		{ 
			int  secDef   = m_pSect->GetSecDefence( pNID ) ;
			int  DefPower = GetSectorDefPower( pNID ) ;
			DefPower = 2 * DefPower / 3 ;

			if( m_pSect->GetResourceID( pNID ) == 2 && rand()%100 < 75 ) { //GOLD sector
				  DefPower /= ( 2 + diff ) ;		// DefPower/5;
				  MAD_ON = false;
			}

			if( MyPower >= DefPower && DefPower < iDef ) {
				iDef = DefPower ; iCAS = pNID;							// атака - pNID не наш сектор. (и нет Пакта)
			}

			DefPower = 2 * DefPower / 3 ;
			if(  m_pSect->GetHistoryOwner( pNID ) == AI_NI && MyPower >= DefPower && 
				(m_pSect->GetHistoryOwner( iCAS ) != AI_NI || (m_pSect->GetHistoryOwner( iCAS ) == AI_NI && DefPower < iDef ) ) ) {
				iDef = DefPower  ;  iCAS = pNID;  MAD_ON = false;		// отвоевываем свой сектор
			}
			bool find = false;
			for(int i=0; i < allyN ; i++) if( m_pDipl->CheckContract(WAR, owner , allies[i] ) ) find = true;
			if( find && MyPower >= DefPower &&  m_pSect->GetHistoryOwner( iCAS ) != AI_NI  ) {
				iDef = DefPower  ;  iCAS = pNID;  MAD_ON = false;		// помогаем союзникам!!!
			}
			// CRAZY RULE
			if( MAD_ON && rand()%10 == 0 && secDef < MAX_DEFENCE ) { 
				iCAS = pNID;  break; } // атака - pNID не наш сектор. (и нет Пакта)
		}
	}
	return iCAS;
}
//----------------------------------------------
int		CROOTDRIVER::FindPathForHero( CHero* pHero, int iCS0, int AI_NI, bool& attack ) // iCS0 - Сектор из которого начали атаку.
{
	int   EMPTY = 0xFFFF;		
	int   diff  = pSAVE->m_iDifficulty;
	if( !pHero || iCS0 < 0 || iCS0 >= m_pSect->GetSectorsNum() )	return EMPTY; 

	int	  iCAS  = EMPTY ;								// Текущий атакуемый сектор.
	int   NN    = m_pSect->GetNeighborsNum( iCS0 );		// Кол-во соседей у сектора плацдарма.
	int	  PACT  = m_pDipl->m_pDiplData->GetPactId(); 
	int	  PASS  = m_pDipl->m_pDiplData->GetPassId();
	int	  ALLY  = m_pDipl->m_pDiplData->GetAllyId();
	int   findAS[50], SecPower[50];	bool  attackS[50];		
	for(int i=0; i < 50 ;i++) { findAS[i] = SecPower[i] = EMPTY; attackS[i] = false; }


	int MyRank	= pHero->GetRank() ;					// звание героя
	int MyPower = pHero->GetARMY_Power() ; 				// силы атакующего героя
	if( m_inCurTurn >  4 - 2*diff  &&  MyRank < 2  &&  rand()%10 > 0  ) {
		MyPower *= 10 ;		m_pPlayer[AI_NI]->SetAggressive( 1000 ); } // сломя голову метемся в атаку
	if( m_inCurTurn > 14 - 2*diff  &&  MyRank < 4  &&  rand()%10 > 1  ) {
		MyPower *= 20 ;		m_pPlayer[AI_NI]->SetAggressive( 1000 ); } // сломя голову метемся в атаку


	// ATTACK
	// 1. Проверим текущий сектор ( партизаны! )
	if(  m_pSect->GetSecOwner(iCS0) != AI_NI && 
		!m_pDipl->CheckContract(PACT, m_pSect->GetSecOwner(iCS0), AI_NI ) && 
		!m_pDipl->CheckContract(PASS, m_pSect->GetSecOwner(iCS0), AI_NI ) ) {
		findAS[1] = iCS0;  attackS[1] = true;		// attack sector where Hero standing - сектор не наш!		
	}


	// 2. Пройдемся по соседям данного сектора.
	findAS[2] = FindNearEnemySector( iCS0, AI_NI, MyPower );
	if( findAS[2] != EMPTY )  attackS[2] = true;


	// 3. Пройдемся по соседям соседей данного сектора. ( все соседи - "наши"!!! )
	int fid = 3;
	for(int i=0; i < NN ; i++) {
		int pNID = m_pSect->GetNeighborID( iCS0, i ); 
		if( !CanEnterIntoSector( AI_NI , pNID ) || !CanStepIntoSector( AI_NI , pNID ) ) {//по этой терр.пройти нельзя
				continue; }													
		int id = FindNearEnemySector( pNID, AI_NI, MyPower );					//след.сек. для атаки - идем к id
		if( id != EMPTY ) { 
			findAS[ fid ] = pNID;  attackS[ fid ] = false;  fid++; }			// перейти - pNID "наш" сектор
	}

	// 1-3. ---------- checks conditions 1-3 ---------- 
	int  asID = fid;
	for(int i = fid ; i > 0 ; i--)
		if( findAS[i] != EMPTY ) {
			int  secID = findAS[ i ];
			SecPower[i] = GetSectorDefPower( secID );
			if(   SecPower[i] <= SecPower[ asID ] || SecPower[i] <= MyPower ||
				  m_pSect->GetHistoryOwner( secID ) == AI_NI  || 
				( m_pSect->GetResourceID( secID ) == 2 && MyRank > 3 && pHero->GetARMY_NSQ() > 10 ) ) //very new
				asID = i;
		}
	iCAS	= findAS [ asID ];		
	attack	= attackS[ asID ];
	// 1-3. ---------- checks conditions 1-3 ---------- 


	// DEFENCE
	// 4. ПЕРЕЙТИ НА САМЫЙ СЛАБЫЙ ИЗ ОКРУЖАЮЩИХ СВОИХ СЕКТОРОВ ( или союзника )
	if( fid < 4 )  fid = 4;
	if( iCAS == EMPTY ) {	// dont have enemy Sector	
		int minDef = EMPTY;
		for(int i=0; i < NN ; i++) 
		{												
			int pNID  = m_pSect->GetNeighborID( iCS0, i );
			int owner = m_pSect->GetSecOwner( pNID );
			if( !CanEnterIntoSector( AI_NI , pNID ) || !CanStepIntoSector( AI_NI , pNID ) )  { continue; }

			// find enemy Hero near THIS sector
			bool  find = false;
			for(int i2=0; i2 < m_pSect->GetNeighborsNum( pNID ) && !find ; i2++) {
				int  NeibID = m_pSect->GetNeighborID( pNID, i2 );
				CHero*  pH = GetHeroInSect( NeibID , AI_NI );
				if( pH && !m_pDipl->CheckContract( PACT , owner , pH->GetPlayerID() ) )  find = true;
			}
			if( find && m_pSect->GetSecDefence( pNID ) < minDef )
				if( owner == AI_NI || m_pDipl->CheckContract( ALLY , owner , AI_NI ) ) {
					findAS[ fid ] = pNID;
					minDef		  = m_pSect->GetSecDefence( pNID ); 
				}
		}
		iCAS	= findAS [ fid ];
		attack	= attackS[ fid ] = false; 	// перейти - iCAS наш сектор ( или союзника ). 
	}


	// MOVE	
	// 5. найти незащищенные сектора врага и двигаться к ним
	if( fid < 5 )  fid = 5;
	if( iCAS == EMPTY ) { 
		findAS[ fid ] = EMPTY ;
		int  min_ID   = EMPTY ,  min_num = EMPTY ;

		int  sNN = 0;
		for(int sec=0; sec < m_pSect->GetSectorsNum() && sNN < 10+2*diff ; sec++) // find enemy secId
		{
			int  owner = m_pSect->GetSecOwner( sec );
			if( owner != AI_NI  &&  !m_pDipl->CheckContract( PACT , owner , AI_NI )  &&  
				m_pSect->GetSecDefence( sec ) <= MyRank  &&  rand()%10 < 5+2*diff  ) { 
				sNN++; 
				int  PID = -1;
				int  id  = MakePathSectorToSector( AI_NI , iCS0 , sec , -1 , PID );
				if(  id != -1  &&  CanStepIntoSector( AI_NI , id )  )
					if( PID != -1  &&  min_num > Path[ PID ][0] ) { 
						min_ID	= findAS[ fid ] = id ;		
						min_num = Path[ PID ][0];		// Path[ PID ][0] - path length
					}
			}
		}
		if( min_ID != EMPTY )  
			iCAS = min_ID;
	}


	// 9x. CRAZY RULE
	if( !attack )
		if( iCAS == EMPTY || ( iCAS == iCS0 && ( m_pSect->GetSecDefence( iCAS ) > 0  || rand()%10 > 8 ) ) )
			for(int i=0; i < NN ; i++) 
			{
				int pNID = m_pSect->GetNeighborID( iCS0, i ); 
				if( !CanEnterIntoSector( AI_NI , pNID ) || !CanStepIntoSector( AI_NI , pNID ) )  { continue; }
				if( pHero->getPreviousSecID() != pNID || NN == 1 )
					if( rand()%10 > 8 || NN == 1 ) { 
						iCAS = pNID;  attack = false;  break; }
			}


	// 10. STEP BACK TO CAPITAL
	//if( m_inCurTurn%STEPBACK_TURNLIMIT == 0 && MyRank < MAX_RANKS -1 /*2*/ && rand()%100 > 40 ) //grey [25.02.2005]
	if( m_inCurTurn%STEPBACK_TURNLIMIT == 0 && (rand()%100 > (20 + 15*MyRank)) ) // 20+0=20 / 20+15*5 = 95
	{
		int  PID = -1;
		int  id  = MakePathSectorToSector( AI_NI , iCS0 , m_pSect->GetSecIDbyOwner(AI_NI) , -1 , PID );
		if(  id != -1 && PID != -1 &&   
			 Path[ PID ][0] > STEPBACK_DISTANCE + MyRank   &&  CanStepIntoSector(AI_NI, id) ) {// Path[ PID ][0] - path length
			iCAS	= id ;
			attack	= false; 	// перейти - iCAS наш сектор ( или союзника ). 
		}
	}


	//save data
	if( m_bSaveLogs ) {
		_str  txt;							
		txt = "AI_NI=";		txt+= AI_NI;	txt+= "\n";		
		txt+= iCS0;			txt+= "->";		txt+= iCAS;		txt+= "  attack=";		txt+= attack;	txt+= "  (";			
		txt+= m_pSect->GetSectorName(iCS0);	txt+= "->";		txt+= m_pSect->GetSectorName(iCAS);		txt+= ")";			
		if( iCAS == EMPTY )											 { txt+="  rule_id="; txt+= -1; }
		else { for( int i=0; i < 10 ; i++ ) if( iCAS == findAS[i] )  { txt+="  rule_id="; txt+=  i; } }
		txt+= "\nMyPower=";	txt+= MyPower;	txt+= "  SecPower=";	txt+= GetSectorDefPower( iCAS );
		
		txt += "\n\n findAS=";
		for( int i=0; i < fid +2 ; i++ ) { txt += findAS[i] ;	txt += "  "; }
		txt += "\n attackS=";	
		for( int i=0; i < fid +2 ; i++ ) { txt += attackS[i] ;	txt += "  "; }
		txt += "\n SecPower=";	
		for( int i=0; i < fid +2 ; i++ ) { txt += SecPower[i] ;	txt += "  "; }
		DString  a_DS( txt.pchar() );
		char  name[256];
		sprintf( name , "Missions\\FP4HeroAI%dT%d.log", AI_NI , this->m_inCurTurn%10 );
		a_DS.WriteToFile( name );
	}

	return  iCAS;
}
//----------------------------------------------
int		CROOTDRIVER::GetSectorDefPower( int secID )
{
	int  EMPTY = 0xFFFF;		
	int  secDef = 0 ,  DefPower = EMPTY;
	if( secID >= 0 && secID < m_pSect->GetSectorsNum() )
	{
		secDef			= m_pSect->GetSecDefence( secID ) ;
		DefPower		= GetSQNumByDefenceID( secDef , 0 , 0 , secID ) * SEC_DEF_POWER( secDef );
		CHero*  pHero	= GetHeroInSect( secID , this->m_inCurActiveAI /*0xFFFF*/ );
		if(pHero) DefPower += pHero->GetARMY_Power() ;
	}
	return  DefPower;
}
//----------------------------------------------
void	CROOTDRIVER::AI_PROCESS( int AI_NI, int PL_NI )
{ 
	if( AI_NI == PL_NI )					{ return; }
	if( !m_pPlayer[AI_NI]->m_Alive )		{ return; }
	if( Check_Surrender(AI_NI,PL_NI) )		{ return; }

	CPlayer*  pAIP = m_pPlayer[ AI_NI ];

	pAIP->SetAggressiveDefault();

	int		iAI_Hs_N = pAIP->m_inHeroNum; //Кол-во героев AI.
	CHero*	pAI_Hs_A = pAIP->m_phrHero;	  //Массив героев AI.

	//ShowInGameSector( pAI_Hs_A[ 0 ].getSectorID() , BM_SHOW_SECPAUSE /* 1   0.5*/ );

	PROCESS_Strategy( AI_NI, PL_NI );
	PROCESS_DiplRelations( AI_NI, PL_NI );
	PROCESS_Sabotage( AI_NI, PL_NI );

	// Проход по массиву героев AI. Поиск героя для атаки.
	// Поиск варианта: ГЕРОЙ - АТАКУЕМЫЙ СЕКТОР, с минимальными потерями.
	CHero*	pCAH = NULL;	//Текущий атакующий герой.
	int		iCS0 = 0xFFFF;	//Сектор из которого начали атаку.
	int		iHAN = 0;		//Кол-во отрядов у героя.		
	CHero*  pCDH = NULL;	//Возможный защитник сектора, герой.
	int		iCAS = 0xFFFF;  //Текущий атакуемый сектор.

	for(int i = m_inCurHeroMove ; i < iAI_Hs_N ; i++) 
	{
		if( !pAI_Hs_A[i].m_Alive )			{ continue; }		
		if( pAI_Hs_A[i].m_inFuel < 1 )		{ continue; }
		
		pCAH = &(pAI_Hs_A[i]);
		iCS0 = pAI_Hs_A[i].getSectorID();			//В какoм секторе стоит герой.
				
		//ShowInGameSector( iCS0 , 0 ); //0.5 );
		//if( i > 0 )  ShowInGameSector( iCS0 , BM_SHOW_SECPAUSE/2 );

		pCDH = NULL;		
		iCAS = 0xFFFF;
		iHAN = pAI_Hs_A[i].GetARMY_NSQ();			//кол-во отрядов в армии

		int NN = m_pSect->GetNeighborsNum(iCS0);	//Кол-во соседей у сектора плацдарма.

		bool attack = false;
		iCAS = FindPathForHero( pCAH , iCS0 , AI_NI , attack ); 
		pCDH = GetHeroInSect( iCAS , AI_NI ) ;

		//check cicle!
		if( !attack && iCAS == pCAH->getPreviousSecID() && pCAH->GetFuel() > 1 )  { iCAS = 0xFFFF; }
		//перейти в наш сектор + сброс сектора
		if( !attack && iCAS!=0xFFFF ) {	
			ShowInGameSector( iCS0 , 0 );
			ShowHeroMove( pCAH , iCS0 , iCAS );			
			pCAH->SetSector(m_pSect, iCAS);  
			ShowHeroMove( pCAH , iCS0 , iCAS );			
			ShowInGameSector( iCS0 , BM_SHOW_SECPAUSE/2 );//ShowInGameSector( iCAS , 0.5 );

			//very new!
			// check for valid defender
			if( pCDH )  /* &&  !ROOT_SCENARIO.CheckRightsForHeroInSector( pCDH ) //ne nado*/
				m_pPlayer[ pCDH->m_inPlayerID ]->TryRetreat( pCDH , false );
			//very new!

			//new
			if( ROOT_SCENARIO.m_inCurTurn >= 4 )
			{
			//new
				if( rand()%100 < 10 || m_pSect->m_pssdDT[ iCAS ]->m_iSabotageID >= 1000000 ) {
					if( m_pSect->m_pssdDT[ iCAS ]->m_iSabotageID < 1000000 )
						m_pSect->m_pssdDT[ iCAS ]->m_iSabotageID += 1000000;	} // bad weather
				else {
					if( rand()%100 < 10 && pCAH->GetFuel() < 2 ) {
						pCAH->SetFuel( 2 ); // good weather
						i--;
						continue;	}	}
			}
			iCAS = 0xFFFF;
		} 

		// подумать - атаковать или нет ( LOOKING AT STARS ;))) 
		bool  must_do = false;				
		if( m_pSect->GetPlayerSect(AI_NI) < 1 || iCAS == iCS0 || 
			pAIP->GetAggressive() >= 100 ||
		  ( m_pSect->GetSecOwner(iCS0) != AI_NI && 
		   !m_pDipl->CheckContract(m_pDipl->m_pDiplData->GetPassId(), AI_NI, m_pSect->GetSecOwner( iCS0 ) ) ) ) {
			must_do = true;
		}
	   if( attack && iCAS!=0xFFFF &&  !must_do ){
			if( this->m_inCurTurn > 10 || this->m_inCurTurn < 2 )
				if( m_pSect->GetHistoryOwner( iCAS ) != AI_NI ) {
					int chance = (rand()%10) / ( 10 - pAIP->m_Aggressive ); //(10 - X) - "X" парам.агрессивности страны! //4; 
					if( !chance ) 
						continue; //not attack
				}
	   }
		if( attack && pCAH->GetBranch_NSQ( 0 ) < 2 && pCAH->GetARMY_FreeNSQ() > 0 && !must_do ) {
			continue;	} //cant attack

		// Если нашли сектор для атаки то начинаем атаковать.
		if( attack && iCAS!=0xFFFF ) {
			int  WAR     = m_pDipl->m_pDiplData->GetWarId();
			int  PACT    = m_pDipl->m_pDiplData->GetPactId();
			int  ALLY    = m_pDipl->m_pDiplData->GetAllyId();
			int  OwnerId = m_pSect->GetSecOwner( iCAS ) ;
			
			if(  OwnerId != PL_NI && m_pSect->m_pssdDT[ iCAS ]->m_iSabotageID%1000 / 100 == 0 && 
				 m_pSect->GetSecDefence( iCAS ) > 0 && 
				(m_pSect->GetSecDefence( iCAS ) < MAX_DEFENCE || rand()%100 > 94) &&
				 rand()%100 > 75 ) {								// virtual pre-attack "Sabotage"
					m_pSect->m_pssdDT[ iCAS ]->m_inDefence--;		// decrease defence
					ShowInGameSector( iCS0 , BM_SHOW_SECPAUSE/2 );//ShowInGameSector( iCS0 , 0.5 );
				}													// virtual pre-attack "Sabotage"
			if(  m_pPlayer[PL_NI]->GetHeroInSector( iCAS ) &&  !must_do  ){ // check Resources
				int  RES_FOOD = 1000 ,  RES_COAL = 1000 ;
				if( AI_NI == 4 || AI_NI == 5 )  RES_COAL = 250;
				if( !pAIP->EnoughResourceAmount( FOOD , RES_FOOD * iHAN ) ||
					!pAIP->EnoughResourceAmount( COAL , RES_COAL * iHAN ) )	{ 
						continue; }
			}

			if( pCDH && m_pDipl->CheckContract( PACT, AI_NI, pCDH->m_inPlayerID ) &&  !must_do  )  { 
				continue ; }
			
			if(  pCDH && OwnerId != pCDH->m_inPlayerID && 
				!m_pDipl->CheckContract( ALLY, OwnerId , pCDH->m_inPlayerID ) ) { //new
				 m_pPlayer[ pCDH->m_inPlayerID ]->TryRetreat( pCDH , false ); 
				 pCDH = NULL;
			}

			if( !m_pDipl->CheckContract(WAR, AI_NI, OwnerId ) && OwnerId != -1 ) { 
				 m_pDipl->AddContract(WAR, AI_NI, OwnerId, 5 );
			}
			if(  pCDH && pCDH->m_inPlayerID != OwnerId && 
				!m_pDipl->CheckContract(WAR, AI_NI, pCDH->m_inPlayerID ) && OwnerId != -1 ) { 
				 m_pDipl->AddContract(WAR, AI_NI, pCDH->m_inPlayerID, 5 );
				}

			ShowInGameSector( iCS0 , 0 );
			ShowHeroMove( pCAH , iCS0 , iCAS );			
			pCAH->SetSector(m_pSect, iCAS );
			ShowHeroMove( pCAH , iCS0 , iCAS );			
			ShowInGameSector( iCS0 , BM_SHOW_SECPAUSE ); //ShowInGameSector( iCAS , 1 );

			//Это сектор другого AI
			if( ( OwnerId != PL_NI && ( !pCDH || ( pCDH && pCDH->GetPlayerID() != PL_NI ) ) ) || 
				( OwnerId == PL_NI && ( !pCDH || ( pCDH && pCDH->GetPlayerID() != PL_NI ) ) )	) 
			{
				//Атакуем сектор с вражеским героем.
				bool ATisWIN = SimulateBattle(pCAH, pCDH, m_pSect->m_pssdDT[iCAS]);

				if( m_pSect->GetSecOwner(iCAS) == PL_NI ) {
					PlaySound( "INTERF_ATTACK" );//new
					char text[256],  tmp[256];   sprintf( tmp, "#CWM_SectBattle%d" , !ATisWIN );
					sprintf(text, GetTextByID( tmp ) , m_pSect->GetSectorName(iCAS) , pCAH->GetName() );
					ShowBigMapGlobalMessage(1, 33 , 31 + ATisWIN  , text , 0 ); // (1, 18 + ATisWIN , 11 , text , 0 );
					SetBigMapMenuEnabled( false );// Enable of All Menu elements
				}

				if( ATisWIN ) {		// победа
					m_pSect->SetSecDefeatDefence( iCAS );			//сектор захвачен-Defence=0!
					(m_pSect->m_psddDT[ iCAS ])->SetOwner( AI_NI );
					if(pCDH) { 
						m_pPlayer[pCDH->GetPlayerID()]->TryRetreat( pCDH , false ); 
						if( pCDH->GetARMY_NSQ() <= 0  && pCDH->m_Alive ) //last add
							pCDH->SetHeroDefeatArmy(); 
					}
				} else {			// поражение
					m_pPlayer[pCAH->GetPlayerID()]->TryRetreat( pCAH , false );
					if( pCAH->GetARMY_NSQ() <= 0  && pCAH->m_Alive ) //last add
						pCAH->SetHeroDefeatArmy();	
				}
	
				// повышение званий героям
				pCAH->GetRewardsStatus();
				pCAH->UpdateRankWithExperience();
				if(pCDH) pCDH->GetRewardsStatus();
				if(pCDH) pCDH->UpdateRankWithExperience();

				PROCESS_FillArmy( AI_NI, PL_NI, 0 , 70 );
				if( ATisWIN && rand()%100 > 45 ) PROCESS_UpgradeDefence( AI_NI, PL_NI, -1, 2, 2 );
				//PROCESS_FillArmy( AI_NI, PL_NI, 0 , 70 );
				//if( ATisWIN ) PROCESS_UpgradeDefence( AI_NI, PL_NI, -1, 2, 2 );

				//ShowInGameSector( pCAH->getSectorID() , 0.5 );
				ShowInGameSector( iCS0 , BM_SHOW_SECPAUSE/2 );
			}
			//Это сектор игрока.
			else {		
				//Заполнить данными CMISSIONINIT* pMIS_INT, для начала миcсии против игрока.
				if( pCDH ) {
					m_Scenario.m_inAttackedSector	= iCAS ;
					m_Scenario.m_bBriefingEnabled	= true ;

					MissionInitData.PlayerStat		= 1 ;		// игрок - в защите
					MissionInitData.SetSector_ID( iCAS );	
					MissionInitData.setHeroPresent(true);

					MissionInitData.setAttackerNation( AI_NI );	
					MissionInitData.setDefenderNation( PL_NI ); 

					m_inCurHeroMove = i+1 ;
					return ;
				}
			} // атака сектора iCAS 
		} // if (attack)
	} // for

	m_inCurHeroMove = 0;

	PROCESS_Market( AI_NI, PL_NI );
}
/*
void	CROOTDRIVER::AI_PROCESS( int AI_NI, int PL_NI, CMISSIONINIT* pMIS_INT ) 
{ 
	if (AI_NI==PL_NI) { return; }
	
	PROCESS_Strategy( AI_NI, PL_NI );
	PROCESS_DiplRelations( AI_NI, PL_NI );

	int		iAI_Hs_N = m_pPlayer[AI_NI]->m_inHeroNum; //Кол-во героев AI.
	CHero*	pAI_Hs_A = m_pPlayer[AI_NI]->m_phrHero;	  //Массив героев AI.
	int		iPL_Hs_N = m_pPlayer[PL_NI]->m_inHeroNum; //Кол-во героев игрока.
	CHero*	pPL_Hs_A = m_pPlayer[PL_NI]->m_phrHero;	  //Массив героев игрока.

	// Проход по массиву героев AI. Поиск героя для атаки.
	// Поиск варианта: ГЕРОЙ - АТАКУЕМЫЙ СЕКТОР, с минимальными потерями.
	CHero*	pCAH = NULL;	//Текущий атакующий герой.
	int		iCS0 = 0xFFFF;	//Сектор из которого начали атаку.
	int		iHAN = 0;		//Кол-во отрядов у героя.							int iSAN = 0; //Кол-во отрядов обороняющих сектор.	int iSDT = 0; //Текущая разница отрядов.
	CHero*  pCDH = NULL;	//Возможный защитник сектора, герой.
	int		iCAS = 0xFFFF;  //Текущий атакуемый сектор.
	//TEMP

	for (int i=0; i<iAI_Hs_N; i++) {
		//Инициализация.
		pCAH = &(pAI_Hs_A[i]);
		iCS0 = pAI_Hs_A[i].getSectorID();			//В какoм секторе стоит герой.
		pCDH = NULL;		//iSDT = 0;
		iCAS = 0xFFFF;
		iHAN = pAI_Hs_A[i].GetARMY_NSQ();			//кол-во отрядов в армии
		if(iHAN<2) { continue; }//grey				//hero cant attack

		int NN = m_pSect->GetNeighborsNum(iCS0);	//Кол-во соседей у сектора плацдарма.

		bool attack = false;
		iCAS = FindPathForHero( m_pSect, iCS0, AI_NI , &attack );

		if(!attack) { pCAH->SetSector(m_pSect,iCAS); iCAS = 0xFFFF;	} //перейти в наш сектор + сброс сектора

		//Если нашли сектор для атаки то начинаем атаковать.
		if(attack) {												//if (iCAS!=0xFFFF) {
			pCDH = GetHeroInSect( iCAS ,0xFFFF );

			//Это сектор другого AI
			if (m_pSect->GetSecOwner(iCAS)!=PL_NI || ( m_pSect->GetSecOwner(iCAS)==PL_NI && pCDH==NULL ) ) 
			{
                //Атакуем сектор с вражеским героем.
				bool ATisWIN = SimulateBattle(pCAH, pCDH, m_pSect->m_pssdDT[iCAS]);

				if (ATisWIN) {					// победа
					(m_pSect->m_psddDT[ iCAS ])->SetOwner(AI_NI);
					m_pSect->SetSecDefeatDefence( iCAS ); //сектор захвачен-Defence=0!
					pCAH->SetSector(m_pSect, iCAS );
					if (pCDH) { m_pPlayer[pCDH->GetPlayerID()]->TryRetreat(pCDH,false); pCDH->SetHeroDefeatArmy(); }
				}else {			
					pCAH->SetHeroDefeatArmy();	// поражение
				}
				// повышение званий героям
				pCAH->UpdateRankWithExperience();
				if(pCDH) pCDH->UpdateRankWithExperience();
			}
			//Это сектор игрока.
			else {		
				//Заполнить данными CMISSIONINIT* pMIS_INT, для начала миcсии против игрока.
				if (pCDH) {
					pCAH->SetSector( m_pSect, iCAS );

					MissionInitData.PlayerStat = 1 ;		// игрок - в защите
					MissionInitData.SetSector_ID( iCAS );	
					MissionInitData.setHeroPresent(true);

					pAtackerHero = pCAH ;	
					pDefendeHero = pCDH ;
					MissionInitData.setAttackerArmy( pCAH );
					MissionInitData.setDefenderArmy( pCDH );
					
					MissionInitData.setAttackerNation( AI_NI );	
					MissionInitData.setDefenderNation( PL_NI ); 
					MissionInitData.setAttackerDirection_ID((reinterpret_cast<CDirectonDataBase*>(SCRIPT_DIRECTION.ScriptType[0]))->getDirection(pCAH->getPreviousSecID(),pCAH->getSectorID()));
				}
			} // атака сектора iCAS 
		} // if (attack)
	} // for

}
*/
//---------------------------------------------- 

void	CROOTDRIVER::PROCESS_Market(int AI_NI, int PL_NI)
{
	int		SUM_NSQ	 = 0;	
	CHero*	pAI_Hs_A = m_pPlayer[AI_NI]->m_phrHero;		// массив героев AI
	int		iAI_Hs_N = m_pPlayer[AI_NI]->m_inHeroNum;	// кол-во героев AI
	for(int h=0; h < iAI_Hs_N ; h++) {
		SUM_NSQ += pAI_Hs_A[h].GetARMY_NSQ();			// кол-во отрядов в армии
	}

	int  MIN_RES[10] = { 0,0,0,0,0,0,0,0,0,0 };
	// minimum resources needed at the end of turn
	MIN_RES[FOOD] = 2000 * SUM_NSQ;
	MIN_RES[COAL] = 1000 * SUM_NSQ;
	MIN_RES[WOOD] = MIN_RES[STONE] = 1000;
	// nations correction 
	switch( AI_NI ) {
		case 4:
		case 5:
			MIN_RES[COAL] /= 3;		break;
	}
	// check resources amount
	for(int res=0; res <= COAL ; res++) {
		if( m_pPlayer[ AI_NI ]->GetResource(res) < MIN_RES[res] )
			m_pPlayer[ AI_NI ]->EnoughResourceAmount( res , MIN_RES[res] );
	}
}
//----------------------------------------------

void	CROOTDRIVER::PROCESS_DiplRelations(int AI_NI, int PL_NI)
{
	int  diff  = pSAVE->m_iDifficulty;

	int  WAR   = m_pDipl->m_pDiplData->GetWarId();
	int  PEACE = m_pDipl->m_pDiplData->GetPeaceId(); 
	int  PACT  = m_pDipl->m_pDiplData->GetPactId();
	int	 PASS  = m_pDipl->m_pDiplData->GetPassId();
	int  ALLY  = m_pDipl->m_pDiplData->GetAllyId();


	if( !m_pPlayer[AI_NI]->EnoughResourceAmount( GOLD, m_pDipl->GetConCost() ) )	return; // нет рес-в


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// -----   ПОДГОТОВКА - выяснение сил врагов   -----
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// список секторов AI_NI
	int  aiSecARR[1000];
	int  AISecNum  = m_pSect->GetPlayerSect( aiSecARR , 1000 , AI_NI );

	// ищем кол-во вражеск.секторов у наших границ
	int EnemySec[MAX_PLAYERS] = { -1, -1, -1, -1, -1, -1 } ;			/* { 0, 0, 0, 0, 0, 0} ;*/
	int NNum = 0 ,  OwnerOne = -1 ; // for PASS
	int id = -1 ;
	for(int i=0; i < AISecNum && i < 1000 ; i++) 
	{
		id = aiSecARR[ i ];
		int psecIDN[100];
		int n = m_pSect->GetNeighborsNum( id );
		m_pSect->GetNeighborsIDs( id , psecIDN , n );
		for(int j=0; j < n ; j++) {
			int Owner = m_pSect->GetSecOwner( psecIDN[j] );
			if( Owner >= 0 && Owner != AI_NI ) {
				if( !m_pDipl->CheckContract( PACT , AI_NI , Owner ) )  EnemySec[ Owner ]++;
				if( OwnerOne != Owner )  NNum++;	 
				if( OwnerOne < 0 )		 OwnerOne = Owner;
			}
		}
	}
	// учитыванием ранги враж.героев
	for(int nat=0; nat < MAX_PLAYERS ; nat++) {//new 
		CPlayer*  pPN = m_pPlayer[ nat ];
		for(int h=0; h < pPN->m_inHeroNum ; h++) // find all heroes  
			if( pPN->m_phrHero[h].m_Alive )  EnemySec[ nat ] += pPN->m_phrHero[h].GetRank(); //new
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  4.  проверка необходимости PASS
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if( NNum == 1 && 
		m_pDipl->CheckContract(PACT, AI_NI , OwnerOne ) && !m_pDipl->CheckContract(PASS, AI_NI , OwnerOne ) )   
		m_pDipl->AddContract( PASS, AI_NI , OwnerOne , 5);	// make PASS contract


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Check Ally/Non-A Possibility
	bool  allyPSB = true && ( rand()%100 > 35 );  //40  //30
	bool  nonaPSB = true && ( rand()%100 > 25 );  //25  //20
	// Check Ally/Non-A Possibility


	allyPSB = allyPSB && ( m_pPlayer[AI_NI]->EnoughResourceAmount( GOLD, 2*m_pDipl->GetConCost() ) );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  1.  предложить союз самой сильной стране на карте							//самому сильному врагу 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(  allyPSB  && AISecNum < m_pSect->GetSectorsNum()/MAX_PLAYERS &&		 // осталось мало секторов!
		( diff == 0 || rand()%100 > 55 ) )
	{
		int max    = 0 ;
		int ememy  = -1 ;	// сильнейшая страна на карте			/* 0;*/
		for(int nat=0; nat < MAX_PLAYERS ; nat++) 
			if( nat != AI_NI ) {
				int num = m_pSect->GetPlayerSect( nat ) + m_pPlayer[ nat ]->m_phrHero[0].GetRank();
				if( max < num ) { max = num;	ememy = nat; }
			}	
		max = 0 ;
		int ememy2 = -1 ;	// 2-я сильнейшая страна на карте 
		for(int nat=0; nat < MAX_PLAYERS ; nat++) 
			if( nat != ememy && nat != AI_NI ) {
				int num = m_pSect->GetPlayerSect( nat ) + m_pPlayer[ nat ]->m_phrHero[0].GetRank();
				if( max < num ) { max = num;	ememy2 = nat; }
			}	

		bool added = false ;
		if( ememy != -1 ) 
		{
			if( m_pDipl->CheckContract(WAR, AI_NI, ememy ) )	m_pDipl->AddContract(PEACE, AI_NI, ememy , 5);
			// предложить союз самому сильному врагу
			if(  m_pDipl->CheckContract(PEACE, AI_NI, ememy ) &&
				!m_pDipl->CheckContract(ALLY,  AI_NI, ememy ) )	added = m_pDipl->AddContract(ALLY, AI_NI, ememy , 5);

			if( !added &&  ememy2 != -1 && ememy2 != ememy ) {
				if( m_pDipl->CheckContract(WAR, AI_NI, ememy2 ) )	m_pDipl->AddContract(PEACE, AI_NI, ememy2 , 5);
				// предложить союз другому сильному врагу
				if(  m_pDipl->CheckContract(PEACE, AI_NI, ememy2 ) &&
					!m_pDipl->CheckContract(ALLY,  AI_NI, ememy2 ) ) added = m_pDipl->AddContract(ALLY, AI_NI, ememy2 , 5);
			}
		}
		if( added )   { return; }
	}


	allyPSB = allyPSB && ( m_pPlayer[AI_NI]->EnoughResourceAmount( GOLD, 2*m_pDipl->GetConCost() ) );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  2.  предложить союз самому опасному врагу своего врага
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int   maxEnemy = 1 ; // france!!!
	bool  findME   = false;
	for(int i=0; i < MAX_PLAYERS ; i++)  
		if( i != AI_NI && EnemySec[i] > EnemySec[maxEnemy] ) { maxEnemy = i; findME = true; } //макс.кол-во

	// ищем кол-во вражеск.секторов у границ maxEnemy 
	if(  allyPSB  && findME ) {
		int FriendSec[MAX_PLAYERS] = { -1, -1, -1, -1, -1, -1 } ; /* { 0, 0, 0, 0, 0, 0} ;*/
		for(int id=0; id < m_pSect->GetSectorsNum() ; id++)	 {
			if( m_pSect->GetSecOwner(id) == maxEnemy )  {
				int n = m_pSect->GetNeighborsNum( id );
				int	psecIDN[10000];
				m_pSect->GetNeighborsIDs( id ,psecIDN,n);
				
				for(int i=0; i < n ; i++) {
					int Owner = m_pSect->GetSecOwner( psecIDN[i] );
					if( Owner >= 0 && Owner != AI_NI && m_pDipl->CheckContract( WAR , maxEnemy , Owner ) ) 
						FriendSec[ Owner ]++;
				}
			}
		}

		bool quit = true ;
		int maxFriend = 0;
		do {
			maxFriend = 0;
			for(int i=0; i < MAX_PLAYERS ; i++) 
				if( FriendSec[ i ] > FriendSec[maxFriend] ) { maxFriend = i; } //макс.кол-во
			
			bool added = false ;
			if( FriendSec[ maxFriend ] != -1 &&  rand()%100 > 55  ) { 
				// предложить союз самому опасному врагу своего врага
				if(  m_pDipl->CheckContract(PEACE, AI_NI, maxFriend ) &&
					!m_pDipl->CheckContract(ALLY,  AI_NI, maxFriend ) && 
					 m_pDipl->m_pDiplData->GetRequestId(ALLY, AI_NI, maxFriend ) < 0 )
					 added = m_pDipl->AddContract(ALLY, AI_NI, maxFriend, 5);
			}
			if( added )  return;

			quit = true;
			FriendSec[ maxFriend ] = -1;
			for(int fr=0; fr < MAX_PLAYERS ; fr++) if( FriendSec[ fr ] > 0 ) quit = false;
		}
		while( !quit );
	}
    

	nonaPSB = nonaPSB && ( m_pPlayer[AI_NI]->EnoughResourceAmount( GOLD , m_pDipl->GetConCost() ) );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  3.  предложить NON-A самому опасному врагу
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int  maxEnemySecNum  = m_pSect->GetPlayerSect( maxEnemy );
	if(  nonaPSB  &&  findME  && AISecNum - maxEnemySecNum/2 <= 0 )
	{
		if( m_pDipl->CheckContract(WAR, AI_NI, maxEnemy ) && 
			m_pDipl->m_pDiplData->GetRequestId(PEACE, AI_NI, maxEnemy ) < 0 )
			m_pDipl->AddContract(PEACE, AI_NI, maxEnemy ,5);
		// предложить пакт самому сильному врагу
		if(  m_pDipl->CheckContract(PEACE, AI_NI, maxEnemy ) && 
			!m_pDipl->CheckContract(PACT, AI_NI, maxEnemy ) ) 
		{
			m_pDipl->AddContract(PACT, AI_NI, maxEnemy ,5);

			//new
			//  4'.  проверка необходимости PASS
			if(  m_pDipl->CheckContract(PACT, AI_NI, maxEnemy ) && 
				!m_pDipl->CheckContract(PASS, AI_NI, maxEnemy ) && 
				 m_pDipl->m_pDiplData->GetRequestId(PASS, AI_NI, maxEnemy ) < 0 )
					m_pDipl->AddContract( PASS, AI_NI, maxEnemy , 5);	// make PASS contract
			//new
		}
	}

	return; 
}
//----------------------------------------------

void	CROOTDRIVER::PROCESS_Strategy(int AI_NI, int PL_NI) 
{
	//new
	if( rand()%100 > 50 ) {
		PROCESS_FillArmy( AI_NI, PL_NI );
	}
	
	PROCESS_UpgradeDefence( AI_NI, PL_NI );

	PROCESS_FillArmy( AI_NI, PL_NI );

	//PROCESS_FillArmy( AI_NI, PL_NI );
	//PROCESS_UpgradeDefence( AI_NI, PL_NI );
}

//----------------------------------------------

bool	CROOTDRIVER::PROCESS_FillArmy( int AI_NI, int PL_NI, int HID, int PC ) 
{
	int  diff = pSAVE->m_iDifficulty;

	//  ------------------------ пополнить армии ------------------------ 
	CHero*	pAI_Hs_A = m_pPlayer[AI_NI]->m_phrHero;	  //Массив героев AI.
	int		iAI_Hs_N = m_pPlayer[AI_NI]->m_inHeroNum; //Кол-во героев AI.

	bool  done = false;
	//for(int h=0; h < iAI_Hs_N && !done ; h++) 
	for(int h = HID>0?HID:0 ; h < iAI_Hs_N && !done ; h++)
	{
		if( HID != -1 )  done = true;

		CHero*	pH = &(pAI_Hs_A[h]);
		if( !pH->m_Alive )  continue;
		pH->UpdateRankWithExperience();					// на случай, если уже успел повоевать
		int  rank  = pH->GetRank();
		int  Sec   = pH->getSectorID();					// В какoм секторе стоит герой.
		int  Nsq   = pH->GetARMY_NSQ();
		int  NFree = pH->GetARMY_FreeNSQ();
		if( NFree == 0 )  continue;

		int  SLOTS[3] = { 3, 1, 1 }; // слоты войск в соот.:  3пех - 1кав - 1арт 
		if(rank < 1) SLOTS[1] = 0;
		if(rank < 2) SLOTS[2] = 0;

		int  sn = 0;
		for(int br=0; br < 3; br++) sn += SLOTS[br];

		int  MaxSqd0 = pH->pArmyLimit->GetMaxSqd( rank , AI_NI );
		int  MaxSqd  = MaxSqd0 / sn ;		if( MaxSqd <= 0 )  MaxSqd = 1;

		for(int br=0; br < 3 ; br++)  SLOTS[br] = MaxSqd * SLOTS[br] - pH->GetBranch_NSQ( br ) + 1 ;
		SLOTS[0] += MaxSqd0 % sn;	SLOTS[0];	SLOTS[1];	SLOTS[2];

		for(int sq= pH->ARMY.GetAmountOfElements()-1 ; sq >= 0 ; sq-- )
		{														
			int  br = GetObjectBranch( pH->ARMY[sq]->getTypeGlobID() );
			if( br < 0 || br > 2 ) continue;			
			for(int num=pH->ARMY[sq]->getSQ_NUM(); SLOTS[br] > 0 && num < pH->ARMY[sq]->getSQ_MAX() && pH->GetARMY_FreeNSQ() > 0 ;num++)
			{
				int pcstate = 100*pH->GetARMY_NSQ() / (pH->GetARMY_NSQ() + pH->GetARMY_FreeNSQ() );
				if( pcstate >= PC )  
					break;
				// get Squad COST				
				int  sqNUM = 1,  sqCOST[8] ;
				GetBigMapSquadCost( AI_NI , pH->ARMY[ sq ]->getTypeGlobID() , sqNUM , sqCOST );
				// check Resources
				if( m_pPlayer[ AI_NI ]->GetResource( RECRT ) < sqNUM )   { continue; }
				for(int res=0; res<=COAL ; res++)
					if( sqCOST[res] > 0 && !m_pPlayer[ AI_NI ]->EnoughResourceAmount( res , sqCOST[res] ) ) { continue; }
				//very new
				int FoodRest = m_pPlayer[ AI_NI ]->GetResource(FOOD) - sqCOST[FOOD];
				if( FoodRest < Safe_FOOD_Amount/2 ) { 
					if( pcstate > 50 || // =f(PC) //last add!!!
						FoodRest <= Safe_FOOD_Amount/10 ) 
						continue; 
				}
				// добавить отряд
				pH->AddSQbyArrID( sq );
				SLOTS[br]--;
				// take Squad COST
				for(int res=0; res<=COAL ; res++)   m_pPlayer[ AI_NI ]->AddResource( res, -sqCOST[res] );
				// decrease recruits in sectors
				DelRecruitsInSectors( AI_NI , Sec , sqNUM ); 
			}
		}


	} // пополнение армии

	return false;
}

//----------------------------------------------

bool	CROOTDRIVER::PROCESS_UpgradeDefence( int AI_NI, int PL_NI, int SecID, int SecNN, int MaxDefID )
{
	bool retVal = false;

	int  diff   = pSAVE->m_iDifficulty;
	int  PACT   = m_pDipl->m_pDiplData->GetPactId(); 

	float dmin	= 2 ;	// 0-sect; 1-hero; 2-neig-hero; 3-neigsect/another
	int   DANGER[10] = { 3 * dmin ,  15 * dmin ,  8 * dmin ,  2 * dmin ,  dmin }; 
	int   DANGER_ARR[1000];										
	for(int i=0; i < 1000; i++) DANGER_ARR[i] = 0; // danger for secID

	int  secARR[1000];
	int  AISecNum = m_pSect->GetPlayerSect( secARR , 1000 , AI_NI );

	// ------------------------ поиск секторов для Upgrade ------------------------ 
	for(int si=0; si < AISecNum ; si++)
	{
		int  sec	= secARR[si];									//DANGER_ARR[ sec ] = 0;
		int  SecDef = m_pSect->m_pssdDT[ sec ]->m_inDefence ;

		if( sec == SecID )  { DANGER_ARR[ sec ] = 0xEEEE; continue; }

		//CHero* pAIH = m_pPlayer[AI_NI]->GetHeroInSector( sec ) ; // ai hero in the sector //if(pAIH) DANGER_ARR[ sec ] -= pAIH->GetARMY_NSQ() ;

		if( SecDef < MaxDefID && SecDef < MAX_DEFENCE ) 
		{		
			int  nn = m_pSect->GetNeighborsNum( sec );
			for(int ni=0; ni < nn ; ni++) {
				int  neigID = m_pSect->GetNeighborID( sec , ni );
				int  Owner  = m_pSect->GetSecOwner( neigID );
				if(  Owner >= 0 && Owner != AI_NI ) {
					if( !m_pDipl->CheckContract(PACT, AI_NI, Owner ) ) //enemy sector
						DANGER_ARR[ sec ] += DANGER[0]; 

					CHero* pH0 = GetHeroInSect( neigID , AI_NI );
					if( pH0 && !m_pDipl->CheckContract(PACT, AI_NI, pH0->GetPlayerID() ) ) { //enemy hero in the sec.
						int  heroDanger = (	pH0->GetARMY_NSQ() + pH0->GetARMY_FreeNSQ() ) - 
											GetSQNumByDefenceID( SecDef, AI_NI, 0 , sec ) + pH0->GetARMY_NSQ() ;
						DANGER_ARR[ sec ] += ( DANGER[1] + heroDanger ); 
					}
				}
				int  curSect = neigID;
				int  NN = m_pSect->GetNeighborsNum( curSect ); //check neighbors of sector
				for(int j=0; j < NN ; j++) {	
					int  Neig = m_pSect->GetNeighborID( curSect , j ) ;
					int  Owner = m_pSect->GetSecOwner( Neig );
					if(  Owner >= 0 && Owner != AI_NI ) {
						if( !m_pDipl->CheckContract(PACT, AI_NI, Owner ) ) //enemy sector
							DANGER_ARR[ sec ] += DANGER[3]; 

						CHero* pH2 = GetHeroInSect( Neig , AI_NI );
						if( pH2 && !m_pDipl->CheckContract(PACT, AI_NI, pH2->GetPlayerID() ) ) {//enemy hero in the sector
							int  heroDanger = ( pH2->GetARMY_NSQ() + pH2->GetARMY_FreeNSQ() ) - 
												GetSQNumByDefenceID( SecDef, AI_NI, 0 , sec ) + pH2->GetARMY_NSQ() ;
							DANGER_ARR[ sec ] += ( DANGER[2] + heroDanger / 2 ); 
						}
					}
				}
			}
			// additional
			if( m_pSect->GetResourceID( sec ) == 2 )  
				DANGER_ARR[ sec ]  = 10 * (DANGER_ARR[sec]>0 ? DANGER_ARR[sec] : (DANGER[0] + DANGER[1]) ); //Golden
			if( SecDef == 0 )  
				DANGER_ARR[ sec ] += 2 * DANGER[0] ; //new // = dmin;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////
	int  RiseList[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, max=-1;
	for(int i=0; i < 10 ; i++) {
		max = -1;
		int m;
		for(m=0; m < m_pSect->GetSectorsNum(); m++)
			if( DANGER_ARR[ m ] > 0 ) {
				max = m;
				break;
			}
		if( max < 0 ) 
			break;  

		for(int j=m+1; j < m_pSect->GetSectorsNum(); j++)
			if( DANGER_ARR[ j ] > DANGER_ARR[ max ] )  max = j;

		RiseList[ i ]	  = max;
		DANGER_ARR[ max ] = -1;
	}

	// ------------------------ поднять защиту секторов ------------------------
	int  RisedNN = 0;
	bool  done = false;
	for(int rInx = 0; rInx < 10 && !done ; rInx++)
	{
		int riseID = RiseList[ rInx ];
		if( riseID >= 0 && riseID < m_pSect->GetSectorsNum() ) 
		{		
			int  def = m_pSect->m_pssdDT[ riseID ]->m_inDefence + 1 ;
			int  mult = (def<2)? 1 :( (def==2)? SECT_DEF_multiplier2 : SECT_DEF_multiplier3 );

			int dCOST[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
			dCOST[ WOOD  ] = SECT_DEFENCE[ WOOD  ] * mult ;
			dCOST[ STONE ] = SECT_DEFENCE[ STONE ] * mult ;
			dCOST[ GOLD  ] = SECT_DEFENCE[ GOLD  ] * mult ;
			dCOST[ RECRT ] = SECT_DEFENCE[ RECRT ] * mult ; 

			done = true;
			if( m_pPlayer[ AI_NI ]->GetResource( RECRT ) < dCOST[ RECRT ] )  done = false;
			for(int res=0; res<=COAL && done ; res++) {
				if( !m_pPlayer[ AI_NI ]->EnoughResourceAmount( res , dCOST[ res ] ) ) { done = false; }				
			}
			if( done ) { 
				for(int res=0; res<=COAL ; res++)  m_pPlayer[ AI_NI ]->AddResource( res , -dCOST[ res ] );				
				DelRecruitsInSectors( AI_NI , riseID , dCOST[ RECRT ] );
				m_pSect->m_pssdDT[ riseID ]->m_inDefence++ ;	// up defence

				RisedNN++;
				if( riseID == SecID )					retVal = true;
				if( SecNN != -1 && RisedNN >= SecNN )	break;
			}
			if( done && rand()%100 < 45*diff )  done = false;
			//new
			if( done && RisedNN <= (diff+1) )   done = false;
		}
	} //защита секторов.

	if( SecID == -1 && SecNN != -1 && RisedNN >= SecNN )  retVal = true; //very new!
	if( SecID == -1 && SecNN == -1 && RisedNN > 0 )		  retVal = true; //very new!
	
	return  retVal;
}


//void	CROOTDRIVER::PROCESS_Strategy(int AI_NI, int PL_NI) 
//{
//	int  diff = pSAVE->m_iDifficulty;
//	//////if( m_pPlayer[AI_NI]->GetResource( GOLD ) <  SECT_DEFENCE[ GOLD  ]  )   return; // экономим   /*1000*/	
//
//	//  ------------------------ пополнить армии ------------------------ 
//	CHero*	pAI_Hs_A = m_pPlayer[AI_NI]->m_phrHero;	  //Массив героев AI.
//	int		iAI_Hs_N = m_pPlayer[AI_NI]->m_inHeroNum; //Кол-во героев AI.
//	
//	for(int h=0; h<iAI_Hs_N; h++) 
//	{
//		CHero*	pH    = &(pAI_Hs_A[h]);
//		pH->UpdateRankWithExperience();					// на случай, если уже успел повоевать
//		int  rank  = pH->GetRank();
//		int  Sec   = pH->getSectorID();					// В какoм секторе стоит герой.
//		int  Nsq   = pH->GetARMY_NSQ();
//		int  NFree = pH->GetARMY_FreeNSQ();
//		if( pH->GetARMY_FreeNSQ() == 0 )   { continue; }
//
//		int  SLOTS[3] = { 3, 1, 1 }; // слоты войск в соот.:  3пех - 1кав - 1арт 
//		if(rank < 1) SLOTS[1] = 0;
//		if(rank < 2) SLOTS[2] = 0;
//
//		int  sn = 0;
//		for(int br=0; br < 3; br++) sn += SLOTS[br];
//		
//		int  MaxSqd0 = pH->pArmyLimit->GetMaxSqd( rank , AI_NI );
//		int  MaxSqd  = MaxSqd0 / sn ;		if( MaxSqd <= 0 )  MaxSqd = 1;
//
//		for(int br=0; br < 3 ; br++)  SLOTS[br] = MaxSqd * SLOTS[br] - pH->GetBranch_NSQ( br ) + 1 ;
//		SLOTS[0] += MaxSqd0 % sn ;
//
//		for(int sq= pH->ARMY.GetAmountOfElements()-1 ; sq >= 0 ; sq-- ) 		SLOTS[0];	SLOTS[1];	SLOTS[2];
//
//		{
//			int  br = GetObjectBranch( pH->ARMY[sq]->getTypeGlobID() );
//			if( br < 0 || br > 2 ) continue;			
//			for(int num=pH->ARMY[sq]->getSQ_NUM(); SLOTS[br] > 0 && num < pH->ARMY[sq]->getSQ_MAX() && pH->GetARMY_FreeNSQ() > 0 ;num++)
//			{
//				// get Squad COST				
//				int  sqNUM = 1,  sqCOST[8] ;
//				GetBigMapSquadCost( AI_NI , pH->ARMY[ sq ]->getTypeGlobID() , sqNUM , sqCOST );
//				// check Resources
//				if( m_pPlayer[ AI_NI ]->GetResource( RECRT ) < sqNUM )   { continue; }
//				for(int res=0; res<=COAL ; res++)
//					if( sqCOST[res] > 0 && !m_pPlayer[ AI_NI ]->EnoughResourceAmount( res , sqCOST[res] ) ) { continue; }
//				// добавить отряд
//				pH->AddSQbyArrID( sq );
//				SLOTS[br]--;
//				// take Squad COST
//				for(int res=0; res<=COAL ; res++)   m_pPlayer[ AI_NI ]->AddResource( res, -sqCOST[res] );
//				// decrease recruits in sectors
//				DelRecruitsInSectors( AI_NI , Sec , sqNUM ); 
//			}
//		}
//
//
//	} // пополнение армии
//
//
//	//////
//	// ------------------------ поднять защиту 3x секторов ------------------------ 
//	int	 PACT	 = m_pDipl->m_pDiplData->GetPactId(); 
//
//	float dmin	  = 1.5 ;	// 0-sect; 1-hero; 2-neig-hero; 3-neigsect/another
//	int   DANGER[10] = { 3 * dmin ,  15 * dmin ,  5 * dmin ,  1 * dmin }; 
//	int   DANGER_ARR[1000];	// danger for secID
//	for(int dd=0; dd < 1000; dd++) DANGER_ARR[dd] = 0;
//
//	int  secARR[1000];
//	int  AISecNum = m_pSect->GetPlayerSect( secARR , 1000 , AI_NI );
//
//	for(int si=0; si < AISecNum ; si++)
//	{
//		int		sec = secARR[si];	DANGER_ARR[ sec ] = 0;
//		int  SecDef = m_pSect->m_pssdDT[ sec ]->m_inDefence ;
//
//		CHero* pAIH = m_pPlayer[ AI_NI ]->GetHeroInSector( sec ) ; // ai hero in the sector
//		//if( pAIH )  DANGER_ARR[ sec ] -= pAIH->GetARMY_NSQ() ;
//
//		if( SecDef < MAX_DEFENCE ) 
//		{
//			int  nn = m_pSect->GetNeighborsNum( sec );
//			for(int ni=0; ni < nn ; ni++) {
//				int  neigID = m_pSect->GetNeighborID( sec , ni );
//				int  Owner = m_pSect->GetSecOwner( neigID );
//				if(  Owner >= 0 && Owner != AI_NI ) {
//					if( !m_pDipl->CheckContract(PACT, AI_NI, Owner ) ) //enemy sector
//						DANGER_ARR[ sec ] += DANGER[0]; 
//
//					CHero* pH0 = GetHeroInSect( neigID , AI_NI );
//					if( pH0 && !m_pDipl->CheckContract(PACT, AI_NI, pH0->GetPlayerID() ) ) //enemy hero in the sector
//					{
//						int  heroDanger = (	pH0->GetARMY_NSQ() + pH0->GetARMY_FreeNSQ() ) - 
//											GetSQNumByDefenceID( SecDef, AI_NI, 0 , sec ) + pH0->GetARMY_NSQ() ;
//						DANGER_ARR[ sec ] += ( DANGER[1] + heroDanger ); 
//					}
//				}
//				int  curSect = neigID;
//				int  NN = m_pSect->GetNeighborsNum( curSect ); //check neighbors of sector
//				for(int j=0; j < NN ; j++) {	
//					int  Neig = m_pSect->GetNeighborID( curSect , j ) ;
//					int  Owner = m_pSect->GetSecOwner( Neig );
//					if(  Owner >= 0 && Owner != AI_NI ) {
//						if( !m_pDipl->CheckContract(PACT, AI_NI, Owner ) ) //enemy sector
//							DANGER_ARR[ sec ] += DANGER[3]; 
//
//						CHero* pH2 = GetHeroInSect( Neig , AI_NI );
//						if( pH2 && !m_pDipl->CheckContract(PACT, AI_NI, pH2->GetPlayerID() ) ) //enemy hero in the sector
//						{
//							int  heroDanger = ( pH2->GetARMY_NSQ() + pH2->GetARMY_FreeNSQ() ) - 
//												GetSQNumByDefenceID( SecDef, AI_NI, 0 , sec ) + pH2->GetARMY_NSQ() ;
//							DANGER_ARR[ sec ] += ( DANGER[2] + heroDanger / 2 ); 
//						}
//					}
//				}
//			}
//			if( this->m_pSect->GetResourceID( sec ) == 2 )  
//				DANGER_ARR[ sec ] = 10 * (DANGER_ARR[ sec ]>0 ? DANGER_ARR[ sec ] : (DANGER[0] + DANGER[1]) ) ; // Golden
//		}
//	}
//	int  MaxDam = -1 ,  MaxDam2 = -1 ;
//	for(int mm=0; mm < m_pSect->GetSectorsNum(); mm++) {
//		if( DANGER_ARR[ mm ] > 0 && MaxDam == -1 )   MaxDam = MaxDam2 = mm;
//		if( MaxDam != -1 && DANGER_ARR[ mm ] > DANGER_ARR[ MaxDam ] )  { 
//			MaxDam2 =  MaxDam;
//			MaxDam  =  mm;
//		}
//	}
//
//	int riseID = -1;
//	int min    = 0xffff;
//	for(int id = m_pSect->GetSectorsNum()-1 ; id >= 0 ; id--) { 
//		if( m_pSect->GetSecOwner(id)==AI_NI && m_pSect->m_pssdDT[ id ]->m_inDefence < min ) {
//#ifdef DemoAI	//DEMO
//			if( !(id == 1 || id == 2 || id == 5 || id == 6) ) ) { continue ; }
//#endif			//DEMO
//			riseID = id; 
//			min = m_pSect->m_pssdDT[ riseID ]->m_inDefence ; }
//	}
//
//	if( riseID >= 0 && m_pSect->m_pssdDT[ riseID ]->m_inDefence < MAX_DEFENCE ) 
//	{ 
//		int  RiseList[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
//		RiseList[0] = MaxDam;	
//		RiseList[1] = MaxDam2;	
//		RiseList[2] = riseID;
//		for(int rInx=9; rInx > 0 ; rInx--)
//			for(int ii=0; ii < rInx && RiseList[ rInx ] >= 0 ; ii++)
//				if( RiseList[ ii ] == RiseList[ rInx ] )  RiseList[ rInx ] = -1; //kill dupes
//
//				bool  done = false;
//				for(int rInx = 0; rInx < 10 && !done ; rInx++)
//					if( RiseList[ rInx ] >= 0 && RiseList[ rInx ] < m_pSect->GetSectorsNum() ) {
//						riseID = RiseList[ rInx ];
//
//						int  def = m_pSect->m_pssdDT[ riseID ]->m_inDefence + 1 ;
//						int  mult = (def<2)? 1 :( (def==2)? SECT_DEF_multiplier2 : SECT_DEF_multiplier3 );
//
//						int dCOST[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
//						dCOST[ WOOD  ] = SECT_DEFENCE[ WOOD  ] * mult ;
//						//dCOST[ FOOD  ] = SECT_DEFENCE[ FOOD  ] * mult ; 
//						dCOST[ STONE ] = SECT_DEFENCE[ STONE ] * mult ;
//						dCOST[ GOLD  ] = SECT_DEFENCE[ GOLD  ] * mult ;
//						dCOST[ RECRT ] = SECT_DEFENCE[ RECRT ] * mult ; 
//
//						done = true;
//						if( m_pPlayer[ AI_NI ]->GetResource( RECRT ) < dCOST[ RECRT ] )  done = false;
//						for(int res=0; res<=COAL && done ; res++) {
//							if( !m_pPlayer[ AI_NI ]->EnoughResourceAmount( res , dCOST[ res ] ) ) { done = false; }				
//						}
//						if(done) { 
//							// TakeResources( dCOST[10] );
//							for(int res=0; res<=COAL ; res++) m_pPlayer[ AI_NI ]->AddResource( res , -dCOST[ res ] );				
//							DelRecruitsInSectors( AI_NI , riseID , dCOST[ RECRT ] );
//							// up defence
//							m_pSect->m_pssdDT[ riseID ]->m_inDefence++ ; 
//						}
//						if( done && rand()%100 < 45*diff )  done = false;
//					}
//	} //защита секторов.
//
//	return;
//}

//void	CROOTDRIVER::PROCESS_Strategy(int AI_NI, int PL_NI) //OLDER VER.
//{
//	if(m_pPlayer[AI_NI]->GetResource( GOLD  ) < 1000 )				{ return; } // экономим	/*5000*/
//	if(m_pPlayer[AI_NI]->GetResource( RECRT ) <  SQ_Amount0 )		{ return; } // экономим	/* 123 */
//
//	// поднять защиту 1го сектора
//	int min    = 0xffff;
//	int riseID = -1;
//	for(int id = m_pSect->GetSectorsNum()-1 ; id >= 0 ; id--) { 
//		if( m_pSect->GetSecOwner(id)==AI_NI && m_pSect->m_pssdDT[ id ]->m_inDefence < min ) {
//
//			#ifdef DemoAI	//DEMO
//				if( !(id == 1 || id == 2 || id == 5 || id == 6) ) ) { continue ; }
//            #endif			//DEMO
//			riseID = id; 
//			min = m_pSect->m_pssdDT[ riseID ]->m_inDefence ; }
//	}
//	if( riseID >= 0 && m_pSect->m_pssdDT[ riseID ]->m_inDefence < MAX_DEFENCE ) { 
//		int  def = m_pSect->m_pssdDT[ riseID ]->m_inDefence + 1 ;
//		
//		int  mult = (def<2)? 1 :( (def==2)? SECT_DEF_multiplier2 : SECT_DEF_multiplier3 );
//		//int mult = 1;		if(def==2)   { mult = SECT_DEF_multiplier2 ; }  		if(def==MAX_DEFENCE)   { mult = SECT_DEF_multiplier3 ; }
//
//		int dCOST[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
//		dCOST[ WOOD  ] = SECT_DEFENCE[ WOOD  ] * mult ;
//		//dCOST[ FOOD  ] = SECT_DEFENCE[ FOOD  ] * mult ; 
//		dCOST[ STONE ] = SECT_DEFENCE[ STONE ] * mult ;
//		dCOST[ GOLD  ] = SECT_DEFENCE[ GOLD  ] * mult ;
//		dCOST[ RECRT ] = SECT_DEFENCE[ RECRT ] * mult ; 
//
//		bool b = true;
//		for(int res=0; res<=COAL && b ; res++) {
//			if( !m_pPlayer[ AI_NI ]->EnoughResourceAmount( res , dCOST[ res ] ) ) { b = false; }				
//		}
//		if(b) { 
//			// TakeResources( dCOST[10] );
//			for(int res=0; res<=COAL ; res++) m_pPlayer[ AI_NI ]->AddResource( res , -dCOST[ res ] );				
//			DelRecruitsInSectors( AI_NI , dCOST[ RECRT ] );
//			// up defence
//			m_pSect->m_pssdDT[ riseID ]->m_inDefence++ ; 
//		}
//	}	//защита секторов.
//
//	// пополнить армии
//	CHero*	pAI_Hs_A = m_pPlayer[AI_NI]->m_phrHero;	  //Массив героев AI.
//	int		iAI_Hs_N = m_pPlayer[AI_NI]->m_inHeroNum; //Кол-во героев AI.
//	
//	for(int h=0; h<iAI_Hs_N; h++) 
//	{
//		CHero*	pH  = &(pAI_Hs_A[h]);
//		int		Sec = pAI_Hs_A[h].getSectorID();			//В какoм секторе стоит герой.
//		int		Nsq = pAI_Hs_A[h].GetARMY_NSQ();
//		pH->UpdateRankWithExperience();						// на случай, если уже успел повоевать
//		if( pH->GetARMY_FreeNSQ() == 0 )   { continue; }
//
//		for(int sq2 =0; sq2 < pH->ARMY.GetAmountOfElements() ; sq2++ )
//		{
//			for(int num=pH->ARMY[sq2]->getSQ_NUM(); num < pH->ARMY[sq2]->getSQ_MAX() && pH->GetARMY_FreeNSQ()>0 ; num++ )
//			{
//				// units COST arrays
//				int COST4[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 } ;	
//				int sqCOST[8] = { 0, 0, 0, 0, 0, 0, 0, 0 } ; 
//				// кол-во солдат в отряде по типу войск
//				int sqNUM = 1 ;						//1 ; //120 ;		/* 30 //62 */
//				if( GetObjectBranch( pH->ARMY[ sq2 ]->getTypeGlobID() ) == 0 )   { sqNUM = SQ_Amount0 ; } //120 ;
//				if( GetObjectBranch( pH->ARMY[ sq2 ]->getTypeGlobID() ) == 1 )   { sqNUM = SQ_Amount1 ; } //30  ;
//				//if( GetObjectBranch( pH->ARMY[ sq2 ]->getTypeGlobID() ) == 2 ) { sqNUM = 1 ; }
//
//				if( m_pPlayer[ AI_NI ]->GetResource( RECRT ) < sqNUM )   { break; }
//
//				if( sqNUM == 1 ) { ; }					// artilery
//				else									// cavalerya
//					if( sqNUM == SQ_Amount1 /*30*/ ) { sqCOST[FOOD] += 150 ; sqCOST[GOLD] +=  50 ; } //FLAGI!!!!
//					else {								// infantry
//						sqCOST[FOOD] += 100 ; sqCOST[GOLD] +=  50 ; //OFFECIERERS!!!!
//						sqCOST[FOOD] += 100 ; sqCOST[GOLD] +=  50 ; //BARABANS!!!!
//						sqCOST[FOOD] += 100 ; sqCOST[GOLD] += 300 ; //FLAGI!!!!
//					}
//				GetUnitCost( AI_NI , pH->ARMY[ sq2 ]->getTypeGlobID() , COST4 ,-1,-1);
//				for(int res=0; res<=COAL ; res++) 
//				{
//					sqCOST[res] += COST4[res] * sqNUM ;
//					//if( m_pPlayer[ AI_NI ]->GetResource( res ) < sqCOST[res] ) { return; }
//					if( sqCOST[res] > 0 && !m_pPlayer[ AI_NI ]->EnoughResourceAmount( res , sqCOST[res] ) ) { return; }					
//				}
//				if( sqNUM == 1 )		{ sqNUM = SQ_Amount2 ; }	// artilery		//{ sqNUM += 3 ; }	
//				//else									
//				//	if( sqNUM == 30 )	{ sqNUM += 1 ; }	// cavalerya
//				//	else				{ sqNUM += 3 ; }	// infantry
//
//				// добавить отряд
//				pH->AddSQbyArrID( sq2 );
//
//				// take units COST
//				for(int res=0; res<=COAL ; res++) 
//					if( sqCOST[res] > 0 ) {
//						m_pPlayer[ AI_NI ]->AddResource( res, -sqCOST[res] );
//					}
//				// decrease recruits in sectors
//				DelRecruitsInSectors( AI_NI , sqNUM ); 
//			}
//		}
//	} // пополнение армии
//
//	// + отступление слабых героев
//
//	return;
//}
//----------------------------------------------
void	CROOTDRIVER::PROCESS_Sabotage( int AI_NI, int PL_NI )
{
	int  WAR   = m_pDipl->m_pDiplData->GetWarId();		
	int  ALLY  = m_pDipl->m_pDiplData->GetAllyId();

	// сектора AI_NI
	int  secARR[1000];
	int  SecNum = m_pSect->GetPlayerSect( secARR , 1000 , AI_NI );

	// найти врага
	bool  done = false;
	for(int nat=0; nat < MAX_PLAYERS && !done ; nat++)
		if( nat != AI_NI  &&  m_pDipl->CheckContract( WAR, AI_NI , nat ) ) // && nat != PL_NI )
		{
			if( nat == PL_NI ) {
				int  money21 = SECT_SABOTAGE[ 2 ][ 1 ];
				m_pQuest->Next_Turn = false;//new
				if( m_pPlayer[AI_NI]->GetResource(GOLD) >= money21 && rand()%100 > 80 && 
					m_pQuest->DeleteDefence( PL_NI, -1 ) ) 
				{
					done = true && rand()%100 > 50;
					m_pPlayer[ AI_NI ]->AddResource( GOLD , - money21 );//def = 1;	sabID = 2;
				}
			}
			else {
				//find target sector
				for(int i=0; i < SecNum && !done ; i++)	
				{ 
					int secID = secARR [ i ];  
					if( m_pPlayer[AI_NI]->GetResource( GOLD ) <  2000  )   return; // экономим

					// в секторе свой или союзный герой
					bool  aiHeroPresent = false;
					CHero*	pH = GetHeroInSect( secID , 0xFFFF );
					if( pH ) {
						int  playerId = pH->GetPlayerID();
						if( playerId == AI_NI || m_pDipl->CheckContract(ALLY, AI_NI , playerId ) ) 
							aiHeroPresent = true;
						aiHeroPresent = aiHeroPresent && rand()%100 > 25 ;
					}

					int  psecIDN[1000];
					int  nn = m_pSect->GetNeighborsIDs( secID , psecIDN , 1000 );				
					for(int i=0; i < nn ; i++) { // просмотр секторов-соседей. 
						int  secID		 =  psecIDN[i] ;
						int  def		 =  m_pSect->GetSecDefence( secID ) ;
						int  SabotageID  =  m_pSect->m_pssdDT[ secID ]->m_iSabotageID ;
						
						if( nat == m_pSect->GetSecOwner( secID ) &&  rand()%100 > 40  ) 
						{ 
							int sabID = -1;
							if( SabotageID%1000 / 100 == 0 && ( rand()%100 > 70 || aiHeroPresent ) )
								if( m_pSect->m_pssdDT[ secID ]->m_inDefence > 0 &&			// (defence - 1)
									( m_pSect->GetSecDefence( secID ) < MAX_DEFENCE || rand()%100 > 91 ) ) 
								{ 
									sabID = 2 ;
									m_pSect->m_pssdDT[ secID ]->m_inDefence-- ;
									m_pSect->m_pssdDT[ secID ]->m_iSabotageID += 100 ;
								}
							if( ( rand()%100 > 80 || aiHeroPresent ) && GetHeroInSect( secID , 0xFFFF ) == NULL &&
								( m_pSect->GetSecDefence( secID ) < MAX_DEFENCE || rand()%100 > 92 ) ) 
								if(secID > 0 && secID != 6 && secID != 21 && secID != 22)	// (nation = -1 ) 
								{
									sabID = 3 ;
									m_pSect->m_pssdDT[ secID ]->m_inDefence = 0 ;
									(m_pSect->m_psddDT[ secID ])->SetOwner( -1 ); //not: Scotland;Paris;Moscow;Cairo;) 
								}
							// another
							if( sabID < 0 ) {
								if( SabotageID%10 == 0 && rand()%100 > 50 )					// (recruits = 0)
								{
									sabID = 0 ;
									m_pSect->m_pssdDT[ secID ]->m_iRecruits	   = 0 ;
									m_pSect->m_pssdDT[ secID ]->m_iSabotageID += 1 ;
								}
								if( SabotageID%100 / 10 == 0 && rand()%100 > 50 )			// (all resources = 0)	
								{
									sabID = 1 ;
									m_pSect->m_pssdDT[ secID ]->m_iSabotageID += 10 ;
								}
							}
							if( sabID >= 0 ) { // if( done )
								_str txt;   txt="AI: "; txt+=AI_NI;   txt+="   SABID= "; txt+=sabID;  
								txt+="/name:"; txt+=m_pSect->GetSectorName(secID);   txt+=" (secID= "; txt+=secID; 
								//ShowBigMapGlobalMessage(1,2, 11, txt.pchar() , 0);
								done = true;
								if( rand()%100 > 50 )  done = false;
								def = m_pSect->GetSecDefence( secID );	if( def < 0 || def > MAX_DEFENCE )  def = 0;
								m_pPlayer[ AI_NI ]->AddResource( GOLD , - SECT_SABOTAGE[ sabID ][ def ] );
							}
						}
					}
				}
			}
		}

	// + virtual "rebellion"
	if( rand()%100 > 97 )
	{
		int  ID = rand()%MAX_PLAYERS ;
		if( ID == AI_NI || ID == PL_NI || ID >= MAX_PLAYERS )						return;
		if( m_pSect->GetPlayerSect( ID ) <= m_pSect->GetSectorsNum()/MAX_PLAYERS )	return;

		int  minDef = 0xFFFF ,  secID = -1 ;
		for(int id=0; id < m_pSect->GetSectorsNum() ; id++)
			if( m_pSect->GetSecOwner(id) == ID && GetHeroInSect( id , 0xFFFF ) == NULL &&
				m_pSect->GetSecDefence(id) < MAX_DEFENCE && m_pSect->GetSecDefence(id) < minDef ) 
			{
				minDef = m_pSect->GetSecDefence(id) ;
				secID  = id ;
			}
		if( secID > 0 && secID != 6 && secID != 21 && secID != 22 ) 
		{
			m_pSect->m_pssdDT[ secID ]->m_inDefence = 0 ;
			(m_pSect->m_psddDT[ secID ])->SetOwner( -1 );
		}
	}
	// virtual "rebellion"
}
//void	CROOTDRIVER::PROCESS_Sabotage( int AI_NI, int PL_NI )
//{
//	// virtual "rebellion"
//	if( rand()%100 > 70 )
//	{
//		int  ID = rand()%MAX_PLAYERS ;
//		if( ID == PL_NI && ID < MAX_PLAYERS-1 )  ID++;
//		if( m_pSect->GetPlayerSect( ID ) <= m_pSect->GetSectorsNum()/MAX_PLAYERS )  return;
//
//		int  minDef = 0xFFFF ,  secID = -1 ;
//		for(int id=0; id < m_pSect->GetSectorsNum() ; id++)
//
//			if( m_pSect->GetSecOwner(id) == ID && GetHeroInSect( id  , 0xFFFF ) == NULL &&
//				m_pSect->GetSecDefence(id) < minDef && m_pSect->GetSecDefence(id) < MAX_DEFENCE )  
//			{
//				minDef = m_pSect->GetSecDefence(id) ;
//				secID  = id ;
//			}
//
//		if( secID > 0 && secID != 6 && secID != 21 && secID != 22 ) 
//		{
//			m_pSect->m_pssdDT[ secID ]->m_inDefence = 0 ;
//			(m_pSect->m_psddDT[ secID ])->SetOwner( -1 );
//		}
//	}
//	// virtual "rebellion"	//new
//}
//----------------------------------------------

bool	CROOTDRIVER::Check_Surrender( int AI_NI , int PL_NI )
{
	int   diff = pSAVE->m_iDifficulty;
	
	// countries surrender
	bool	Surrend	   = false;
	int		AISectNum  = m_pSect->GetPlayerSect( AI_NI );
	int		PLSectNum  = m_pSect->GetPlayerSect( PL_NI );

	bool	find	   = false;
	CPlayer*  pPL = m_pPlayer[ PL_NI ];
	if(  m_pDipl->CheckContract( m_pDipl->m_pDiplData->GetWarId(),  AI_NI, PL_NI ) &&
		 PLSectNum >= AISectNum * (3 + diff/2) &&  pPL )
	{
		for(int h=0; h < pPL->m_inHeroNum && !find ; h++) // find where PL_NI heroes  
			if( pPL->m_phrHero[h].m_Alive && pPL->m_phrHero[h].GetRank() >= (MAX_RANKS/2 + diff/2) ) {
				int  secID = pPL->m_phrHero[h].getSectorID();
				for(int n=0; n < m_pSect->GetNeighborsNum( secID ) && !find ; n++) {
					int  secID2 = m_pSect->GetNeighborID(secID, n );
					for(int e=0; e < m_pSect->GetNeighborsNum( secID2 ) && !find ; e++) // second level
						if( AI_NI == m_pSect->GetSecOwner( secID2 ) || 
							AI_NI == m_pSect->GetSecOwner( m_pSect->GetNeighborID(secID2, e ) ) )
							find = true;
				}
			}
	}

	//
	int count = 0, AI_NSQ = 0;
	for(int i=0; i < MAX_PLAYERS ; i++)  
		if( m_pPlayer[i] && m_pPlayer[i]->m_Alive ) count++;
	if( pPL ) {
		for(int h=0; h < pPL->m_inHeroNum ; h++) // find PL_NI heroes  AI_NSQ
			if( pPL->m_phrHero[h].m_Alive ) {
				AI_NSQ += pPL->m_phrHero[h].GetARMY_NSQ();
			}
	}
	if( count <= MAX_PLAYERS/2  &&  AISectNum == 0  &&  AI_NSQ <= 3 )		Surrend = true;

	if( count < 4 && AISectNum <= 2 &&  AISectNum < PLSectNum && find )		Surrend = true; // 2 AI vs. Player
	//

	if( count < 3 && AISectNum <= 4 &&  AISectNum < PLSectNum  )			Surrend = true;	// final - AI vs. Player
	

	if( Surrend || ( find && rand()%100 > ( 20 + diff*15 )  &&  rand()%100 > 15  ) ) {
		char  text[256];
		sprintf(text, GetTextByID("#CWM_CSurrend"), m_pPlayer[AI_NI]->GetPlayerName() );
		Surrend = false;
		if( ShowBigMapGlobalMessage(2, 33 , 16 , text , 0 ) == 1 ) {
			Surrend = true;
			for(int id=0; id < m_pSect->GetSectorsNum() ; id++)				// union sectors
				if( m_pSect->GetSecOwner( id ) == AI_NI ) 
					(m_pSect->m_psddDT[ id ])->SetOwner( PL_NI );
			m_pPlayer[AI_NI]->m_Alive = false ;								// kill country
			for(int h=0; h < m_pPlayer[AI_NI]->m_inHeroNum ; h++)			// kill alive heroes  
				m_pPlayer[AI_NI]->DeleteHero( h );
			sprintf(text, GetTextByID("#CWM_CSurrend2"), m_pPlayer[AI_NI]->GetPlayerName() );
			int picID = 11;  if( AI_NI == 5 )  picID = 15;
			ShowBigMapGlobalMessage(1, picID , 11 , text , 0 );
		}
	}
	return  Surrend;

	//#ifdef DemoVer4Journals	//DEMO
	//	if( AI_NI == 2 && AISectNum <= 2 &&  AISectNum <= PLSectNum  )  Surrend = true;
	//	//if( AI_NI == 0 && AISectNum <= 1 )  Surrend = true;	//if( AI_NI == 2 && AISectNum <= 3 )  Surrend = true;
	//#endif					//DEMO
}
//----------------------------------------------
bool	CROOTDRIVER::CHECK_DEFEAT( int PL_NI )
{
	if( ROOT_SCENARIO.m_bEndGame )  return true;

	bool  check[MAX_PLAYERS];
	for(int nat=0; nat < MAX_PLAYERS ; nat++) {
		check[nat] = m_pPlayer[nat]->m_Alive ;   
		m_pPlayer[nat]->m_Alive = false; 
		if( m_pSect->GetPlayerSect(nat) > 0 )  m_pPlayer[nat]->m_Alive = true;
	}	
	for(int nat=0; nat < MAX_PLAYERS ; nat++)
		if( !m_pPlayer[nat]->m_Alive ) {
			bool   find = false;
			for(int h=0; h < m_pPlayer[nat]->m_inHeroNum && !find ; h++) // find alive heroes  
				if( m_pPlayer[nat]->m_phrHero[h].m_Alive )   find = true;
			m_pPlayer[nat]->m_Alive = find;
		}

	// checkin big Victory-Loose
	char  text[256];
	bool  Victory = true;
	for(int nat=0; nat < MAX_PLAYERS ; nat++) 
		if( nat != PL_NI ) {
			if( check[nat] && !m_pPlayer[nat]->m_Alive ) {
				sprintf(text, GetTextByID("#CWM_CountryDef"), m_pPlayer[nat]->GetPlayerName() ); //Country Defeated
				ShowBigMapGlobalMessage(1, 33 , 16 , text , 0 );
			}
			Victory = Victory && !m_pPlayer[nat]->m_Alive; 
		}

	ROOT_SCENARIO.m_bEndGame = false;
	if( MissionInitData.getSector_ID() < 0 ) {
		if( !m_pPlayer[PL_NI]->m_Alive ) {			// ------- LOOSE!!! -------
			ROOT_SCENARIO.m_bEndGame = true;
			sprintf(text, GetTextByID("#CWM_TotalLoose"), m_pPlayer[ PL_NI ]->GetPlayerName() );
			ShowBigMapGlobalMessage(1, 29 , 29 , text , 0 );
			ProcessBigMapPlayVideo( 0 );
			return  true;
		}
			#ifdef DemoAI // - DEMO Victory Conditions -
				if(!m_pPlayer[1]->m_Alive)  Victory = true;//DEMO
			#endif					
			#ifdef DemoVer4Journals	
				//if( m_pSect->GetPlayerSect( 0 ) + m_pSect->GetPlayerSect( 2 ) == 0 )  Victory = true;//DEMO
				if( m_pSect->GetPlayerSect( 2 ) == 0 )  Victory = true;//DEMO
			#endif		 // - DEMO Victory Conditions -

		if( Victory ) {								// ------- VICTORY!!! -------
			ROOT_SCENARIO.m_bEndGame = true;
			ShowBigMapGlobalMessage(1, 30 , 30 , GetTextByID("#CWM_TotalVict") , 0 );
			// && сохранение результатов 
			SaveBigMapDataXML();
			ProcessBigMapPlayVideo( 1 );
			return true;
		}
	} 
	return  false; 
}
//----------------------------------------------
void	CROOTDRIVER::UpdateResources()
{
	CResource_BM	OldRes = m_pPlayer[gi_bmSAVE_Nation]->Resource ; 

	for(int id=0; id < m_pSect->GetSectorsNum() ; id++) {
		// рост населения 
		if( m_pSect->GetSecPopulation(id) < 2 && m_pSect->GetSecPopulation(id) < m_pSect->GetSecDefence(id) ) { 
			m_pSect->m_pssdDT[id]->m_inPopulation++ ; 
		}	
		// добавление рекрутов
		int pop = m_pSect->m_pssdDT[id]->m_inPopulation + 1 ;
		m_pSect->m_pssdDT[id]->m_iRecruits += pop * SECT_REG_RECRTS ;
		if( m_pSect->m_pssdDT[id]->m_iRecruits > pop * SECT_MAX_RECRTS ) {
			m_pSect->m_pssdDT[id]->m_iRecruits = pop * SECT_MAX_RECRTS ;
		}
	}
	// добавление ресурсов
	for(int nat=0; nat < MAX_PLAYERS ; nat++)	{ m_pPlayer[nat]->UpdateResources(); }

	for(int id=0; id < m_pSect->GetSectorsNum() ; id++) m_pSect->m_pssdDT[id]->m_iSabotageID = 0;

	// ----- ADD STATISTICS INFO! -----
	AddStatisticsInfo( OldRes );
}
//----------------------------------------------
void	CROOTDRIVER::AddStatisticsInfo(CResource_BM& OldResAmount)
{
	// ----- ADD STATISTICS INFO! -----
	int   PL_NI = gi_bmSAVE_Nation;
	_str  text, tBlack, tBrown, tRed;  tBlack = "{C FF101010}";  tBrown = "{C FF502515}";  tRed = "{C FF640100}";

	text  = tBlack;		text += GetTextByID("#CWT_CurrentTurn");	text += " ";	
	text += tRed;		text += m_inCurTurn + 2;
	
	//text += tBlack;		text += ". \\";		text += GetTextByID("#CWT_NumOfSectors");	text += " ";	
	//text += tRed;		text += m_pSect->GetPlayerSect(PL_NI);

	text += tBlack;		text += ". \\";		text += GetTextByID("#CWT_ResourSupply");	text += " \\ "; /*text += " ";*/ 
	
	int   list[7] = { WOOD, FOOD, STONE, GOLD, IRON, COAL } ;
	for(int res=0; res <= COAL ; res++) {
		int curR = list[res] ;
		text += tBlack;  
		if( res > 0 ) text += ", ";
		text += GetResourseName( curR ) ;
		text += " - ";
		text += tRed;
		text += m_pPlayer[PL_NI]->Resource.GetResource( curR ) - OldResAmount.GetResource( curR ) ;
	}
	text += tBlack;   
	text += ", ";		
	text += GetTextByID("ResName9") ;
	text += " - ";	
	text += tRed;
	text += m_pPlayer[PL_NI]->Resource.GetResource( RECRT ) - OldResAmount.GetResource( RECRT );
	text += tBlack;   
	text += ".";
	
	Enumerator*   pMessHeads = ENUM.Get( "MessHead_ENUM" );
	int  picID = 21 ;	if( PL_NI == 5 ) picID = 22 ;
	m_pDipl->AddMessage( -100, 0, picID , "_" , pMessHeads->Get( 11 ) , text.pchar() );
}
//----------------------------------------------
bool	CROOTDRIVER::CheckAddHero()
{
	//выясним кол-во секторов у страны
	int SecN[MAX_PLAYERS] = { 1, 1, 1, 1, 1, 1 } ; /* 'N = 1' -- THATS RIGHT!!! */ 
	for(int pl=0; pl < MAX_PLAYERS ; pl++)
		for(int id=0; id < m_pSect->GetSectorsNum() ; id++) if( m_pSect->GetSecOwner(id) == pl ) SecN[pl]++ ;
	
	// добавить героя если надо
	for(int pl=0; pl < MAX_PLAYERS ; pl++)
	{
		if( m_pPlayer[pl]->m_Alive && m_pPlayer[pl]->m_inHeroNum < MAX_HEROES &&
			SecN[pl]/m_pPlayer[pl]->m_inHeroNum  > 2 ) 		  /* ( SecN[ pl ]-1 ) > 2*/
			{
				m_pPlayer[pl]->AddHero( m_pSect->GetSecIDbyOwner(pl), m_pPlayer[pl]->GetNewHeroID() );//m_pPlayer[ pl ]->m_inHeroNum );
				
				if( pl != gi_bmSAVE_Nation /*this->m_pDipl->m_pDiplData->m_iPlayerId */) {
					m_pPlayer[pl]->m_phrHero[m_pPlayer[pl]->m_inHeroNum-1].SetDefaultARMY(); //TEMP!!!
				}
			}
	}
	return true;
}
//----------------------------------------------

int		CROOTDRIVER::GetSectList( int player, int startSecId, int* list )//grey
{
	int n = 0;

	return n;
}

//----------------------------------------------
int		CROOTDRIVER::AddRecruitsInSectors( int player, int secID, int recruits )
{
	int Rec = recruits;

	bool  done = false;
	int   DIV = m_pSect->GetSectorsNum()/2;	// 12
	int   ID0 = rand()%DIV + 1;				// 0-11 + 1		    = 1-12
	int   IDN = rand()%DIV + ID0 + 1 ;		// 0-11 + 1-12 + 1  = 2-24
	if( IDN >= m_pSect->GetSectorsNum() )  IDN = m_pSect->GetSectorsNum();


	int  psecIDN[1000] ;
	int  n = m_pSect->GetNeighborsIDs( secID , psecIDN , n );
	psecIDN[ n ] = secID;
	for(int i=n; i >= 0 && !done ; i--) // add in Sector and his Neighbors
		if( m_pSect->GetSecOwner( psecIDN[i] ) == player ) 
		{
			int id = psecIDN[i];
			int secRec = m_pSect->m_pssdDT[id]->m_iRecruits ;
			int delta  = (m_pSect->GetSecPopulation( id ) + 1) * SECT_MAX_RECRTS - secRec ;
			if( Rec > delta ) { 
				m_pSect->m_pssdDT[id]->m_iRecruits += delta ; 
				Rec -= delta;	
			}
			else { m_pSect->m_pssdDT[id]->m_iRecruits += Rec ;	Rec = 0;  done = true; }
			if( rand()%100 > 50 )  i--; //crazy!
		}	

	//int  id = secID;
	//if( m_pSect->GetSecOwner( id ) == player ) 
	//{
	//	int secRec = m_pSect->m_pssdDT[id]->m_iRecruits ;
	//	int delta  = (m_pSect->GetSecPopulation( id ) + 1) * SECT_MAX_RECRTS - secRec ;
	//	if( Rec > delta ) { 
	//		m_pSect->m_pssdDT[id]->m_iRecruits += delta ; 
	//		Rec -= delta;	
	//	}
	//	else { m_pSect->m_pssdDT[id]->m_iRecruits += Rec ;	Rec = 0;  done = true; }
	//}

	for(int tr=0; Rec > 0 && !done && tr < 100 ; tr++)
	{
		for(int id= ID0 ; id < IDN && !done ; id++) {
			if( m_pSect->GetSecOwner( id ) == player && rand()%100 > 40 ) {
				int secRec = m_pSect->m_pssdDT[id]->m_iRecruits ;
				int delta  = (m_pSect->GetSecPopulation( id ) + 1) * SECT_MAX_RECRTS - secRec ;
				if( Rec > delta ) { 
					m_pSect->m_pssdDT[id]->m_iRecruits += delta ; 
					Rec -= delta;	
				}
				else { m_pSect->m_pssdDT[id]->m_iRecruits += Rec ;	Rec = 0;  done = true; }
			}	
		}
		ID0 = 0;
		IDN = m_pSect->GetSectorsNum();
	}

	if( Rec > 0 ) { // add rest to cur Recrts[ secID ]
		m_pSect->m_pssdDT[ secID ]->m_iRecruits += Rec;	  int MAX = SECT_MAX_RECRTS* MAX_DEFENCE* (m_pSect->GetSecPopulation(secID)+1 );
		if( m_pSect->m_pssdDT[ secID ]->m_iRecruits > MAX )  m_pSect->m_pssdDT[ secID ]->m_iRecruits = MAX;
	}

	m_pPlayer[ player ]->SetRecruit() ;
	UPDATE_SCENARIO();

	return Rec ;

	//int Rec = recruits;
	//for(int id=0; id < m_pSect->GetSectorsNum() ; id++) {
	//	if( m_pSect->GetSecOwner( id ) == player ) {
	//		int secRec = m_pSect->m_pssdDT[id]->m_iRecruits ;
	//		int delta  = (m_pSect->GetSecPopulation( id ) + 1) * SECT_MAX_RECRTS - secRec ;
	//		if( Rec > delta ) { 
	//			m_pSect->m_pssdDT[id]->m_iRecruits += delta ; 
	//			Rec -= delta;	
	//		}
	//		else { m_pSect->m_pssdDT[id]->m_iRecruits += Rec ;	break; }
	//	}	
	//}
	//m_pPlayer[ player ]->SetRecruit() ;
	//UPDATE_SCENARIO();

	//return Rec ;
}
//----------------------------------------------
int		CROOTDRIVER::DelRecruitsInSectors( int player, int secID, int recruits )
{
	int  Rec = recruits;

	bool  done = false;
	int   DIV = m_pSect->GetSectorsNum()/2;									// 12
	int   ID0 = rand()%DIV + 1;												// 0-11 + 1		    = 1-12
	int   IDN = rand()%DIV + ID0 + 1 ;										// 0-11 + 1-12 + 1  = 2-24
	if( IDN >= m_pSect->GetSectorsNum() )  IDN = m_pSect->GetSectorsNum();	// 24


	int  psecIDN[1000] ;
	int  n = m_pSect->GetNeighborsIDs( secID , psecIDN , n );
	psecIDN[ n ] = secID;
	for(int i=n; i >= 0 && !done ; i--) // delete in Sector and his Neighbors
		if( m_pSect->GetSecOwner( psecIDN[i] ) == player ) 
		{
			int id = psecIDN[i];
			int secRec = m_pSect->m_pssdDT[id]->m_iRecruits ;
			if( Rec > secRec ) { 
				m_pSect->m_pssdDT[id]->m_iRecruits = 0 ; 
				Rec -= secRec;	
			}
			else { m_pSect->m_pssdDT[id]->m_iRecruits -= Rec ;	Rec = 0;  done = true; }
			if( rand()%100 > 50 )  i--; //crazy!
		}	

	//int  id = secID;
	//if( m_pSect->GetSecOwner( id ) == player ) 
	//{
	//	int secRec = m_pSect->m_pssdDT[id]->m_iRecruits ;
	//	if( Rec > secRec ) 
	//	{ 
	//		m_pSect->m_pssdDT[id]->m_iRecruits = 0 ; 
	//		Rec -= secRec;	
	//	}
	//	else { m_pSect->m_pssdDT[id]->m_iRecruits -= Rec ;	Rec = 0;  done = true; }
	//}	

	for(int tr=0; Rec > 0 && !done && tr < 100 ; tr++)
	{
		for(int id= ID0 ; id < IDN && !done ; id++) {
			if( m_pSect->GetSecOwner( id ) == player && rand()%100 > 40 ) {
				int secRec = m_pSect->m_pssdDT[id]->m_iRecruits ;
				if( Rec > secRec ) { 
					m_pSect->m_pssdDT[id]->m_iRecruits = 0 ; 
					Rec -= secRec;	
				}
				else { m_pSect->m_pssdDT[id]->m_iRecruits -= Rec ;	Rec = 0;  done = true; }
			}	
		}
		ID0 = 0;
		IDN = m_pSect->GetSectorsNum();
	}

	m_pPlayer[ player ]->SetRecruit() ;
	UPDATE_SCENARIO();

	return Rec ;

	//int  Rec = recruits;
	//for(int id=0; id < m_pSect->GetSectorsNum() ; id++) {
	//	if( m_pSect->GetSecOwner( id ) == player ) {
	//		int secRec = m_pSect->m_pssdDT[id]->m_iRecruits ;
	//		if( Rec > secRec ) { 
	//			m_pSect->m_pssdDT[id]->m_iRecruits = 0 ; 
	//			Rec -= secRec;	
	//		}
	//		else { m_pSect->m_pssdDT[id]->m_iRecruits -= Rec ;	break; }
	//	}	
	//}
	//m_pPlayer[ player ]->SetRecruit() ;
	//UPDATE_SCENARIO();

	//return Rec ;
}
//----------------------------------------------

//new
int		CROOTDRIVER::MakePathSectorToSector( int AI_NI , int secID , int secID2 , int pLimit , int& path_ID ) // ret next SecID
{
	int   NEXT_ID = -1;			

	const int MaxTry  =  50;  // первые MaxTry путей			
	const int MaxPath = 200;
	for(int i=0; i < MaxPath ; i++)  Path[i][0] = -1;

	int  curId	= secID;
	int  NN		= m_pSect->GetNeighborsNum( curId );		// kол-во соседей у сектора

	bool  done = false;

	// prepare
	for(int sec=0; sec < NN ; sec++ ) {		
		int  pNID  = m_pSect->GetNeighborID( curId, sec );
		int  owner = m_pSect->GetSecOwner( pNID );
		if( !CanEnterIntoSector( AI_NI , pNID ) )	{ continue; }
		Path[sec][0] = 1 ;
		Path[sec][1] = pNID ;
	}

	// find path
	for(int search_nn=0; search_nn < MaxTry ; search_nn++)
	{
		done = false;
		for( int i=0; i < MaxPath ; i++ )
		{
			int num = Path[i][0];//1

			if( num > 0 && num < MaxPath  && ( num <= pLimit || pLimit <= 0 )  &&  Path[i][ num ] != secID2 )
			{
				curId	= Path[i][ num ];//1

				int  NN = m_pSect->GetNeighborsNum( curId );//4		// kол-во соседей у сектора

				for(int sec=0; sec < NN ; sec++ )
				{
					int  pNID  = m_pSect->GetNeighborID( curId, sec );
					int  owner = m_pSect->GetSecOwner( pNID );
					if( !CanEnterIntoSector( AI_NI , pNID ) )	{ continue; }

					bool  find = false;
					for(int ca=1; ca < num + 1 && !find ; ca++)		// check array
						if( Path[i][ca] == pNID )  find = true;

					if( !find && pNID != secID ) {
						if( Path[i][0] == num ) {
							Path[i][0]++ ;
							Path[i][ Path[i][0] ] = pNID ;
						}
						else {
							bool  find = false;
							int ca;
							for( ca=i; ca < MaxPath && !find ; ca++ )		// check array
								if( Path[ca][0] == -1 )  { find = true; break; }
							if( find ) 
							{
								for(int k=0; k < num + 1 ; k++)	Path[ca][k] = Path[i][k]; // dublicate array
								Path[ca][ num + 1 ] = pNID ; 
							}
						}
					}
				}
				if( Path[i][0] == num )  Path[i][0] = -1 ;  //тупик!!! 
			}
		}
	}


	int  min = -1;
	for( int i=0; i < MaxPath ; i++ ) {
		int num = Path[i][0];		
		if( num > 0 && num < MaxPath && Path[i][ num ] == secID2 && ( num <= pLimit || pLimit <= 0 ) )
		{
			if( min < 0 || Path[min][0] > num ) 
				min = i;
		}
	}
	path_ID = min;
	if( min >= 0 && Path[min][0] > 0 )  NEXT_ID = Path[min][1];

	if( path_ID < 0 || path_ID >= MaxPath )						path_ID = -1;
	if( NEXT_ID < 0 || NEXT_ID >= m_pSect->GetSectorsNum() )	NEXT_ID = -1;

	//save data
	if( m_bSaveLogs ) { 
		_str  txt;							txt = secID;		txt+= "->";		txt+= secID2;	
											txt+=" /NEXT_ID=";	txt+=NEXT_ID;	txt+=" /path_ID=";	txt += path_ID;
		for( int i=0; i < MaxPath ; i++ )			
			if(Path[i][0] > 0 ) {
				txt += "\n";
				txt += i;
				txt += " n=";
				for( int k=0; k < MaxPath && k < Path[i][0] + 1  /*+1*/  ; k++ ) {
					txt += Path[i][k];   
					if(k==0)  txt += ";";   
					txt += "  ";
				}
			}
		DString  a_DS( txt.pchar() );
		a_DS.WriteToFile( "Missions\\S2SPaths.log" );
	}
	//save
	return  NEXT_ID;
}
//----------------------------------------------
bool	CROOTDRIVER::CheckHotKeys()
{
	//
	if( !this->m_EnabledMapClick )		{ return false; }
	//
	if( KeyPressed )
	{
		// switch between  pages
		if( LastKey == VK_TAB /*|| LastKey == VK_RIGHT || LastKey == VK_LEFT*/ ) {
			int add  = 1;
			if( LastKey == VK_LEFT ||  (LastKey == VK_TAB && (GetKeyState(VK_CONTROL)&0x8000))  ) // Ctrl Tab  
				add = -1;//new
			int next = m_Scenario.m_inActivMenu + add ;
			if( next < 0 )  next = 4;	if( next > 4 )  next = 0;

			SimpleDialog* psd = (SimpleDialog*) m_pMenu->Find( 0x40 + next );
			LastKey = KeyPressed = 0;
			ChangeActiveMenu( psd );

			PlayEffect( psd->ClickSound ,0,0);
		}
		// show  m_inLastLMCSecID  sector
		if( LastKey == VK_SPACE ) {									
			LastKey = KeyPressed = 0;
			if( m_Scenario.m_inActivMenu < 2 ){
				ROOT_SCENARIO.ShowInGameSector();
			}
			PlaySound( "INTERF_SECTOR" );
		}

		// show  USER  hero
		if( LastKey == VK_HOME /*|| LastKey == VK_END || LastKey == VK_INSERT || LastKey == VK_NUMPAD0*/ ) {
			//int  nextID = 0 ;
			//if( LastKey == VK_HOME ) {
			//	for(int ni = 0; ni < MAX_PLAYERS && !m_pPlayer[ nextID ]->m_Alive; ni++)  nextID = ni;
			//}
			//else {
			//	nextID = MAX_PLAYERS - 1 ;
			//	for(int ni = MAX_PLAYERS - 1; ni >= 0 && !m_pPlayer[ nextID ]->m_Alive; ni-- )  nextID = ni;
			//}
			//if( m_Scenario.m_inActivMenu < 2 || LastKey == VK_INSERT || LastKey == VK_NUMPAD0 )  nextID = gi_bmSAVE_Nation ;
			int  nextID = gi_bmSAVE_Nation ;
			LastKey = KeyPressed = 0;
			if( m_Scenario.m_inActivMenu < 3 ) {
				//if( m_Scenario.m_inActivMenu < 2 || LastKey == VK_INSERT ) nextID = gi_bmSAVE_Nation ;
				if( nextID >= 0 && nextID < MAX_PLAYERS ) {
					CHero*  pH = &(m_pPlayer[ nextID ]->m_phrHero[ 0 ]) ;
					int  heroID = 0 ;
					if( pH )   heroID = pH->GetHeroID();
					pH = m_pPlayer[ nextID ]->GetHeroByPID( heroID );		// END - last alive hero id
					if( pH ) {
						m_Scenario.m_inPlayerInUseHero	 =  nextID ;		// END - last alive hero id
						m_Scenario.m_inLastHeroClick	 =  heroID + nextID * 100 ;
						m_Scenario.m_inLastLMCSecID		 =  pH->getSectorID() ;
						m_pSect->m_LastLMCSecID			 =  m_Scenario.m_inLastLMCSecID ;	
							m_phpDesk->m_iCurPlayer  = -1; // personal
							m_phpDesk->m_Visible	 = false;
							m_pHmm->SetAll( pH );
							m_pHPers->setPlayerID( nextID );
							
							PlayEffect( pH->m_psdMapPic->ClickSound ,0,0);//new
						if(!m_Scenario.m_bHeroMiniMap) m_Scenario.m_bHeroMiniMap = (m_Scenario.m_inActivMenu==0);//new
						ROOT_SCENARIO.ShowInGameSector();
					}
				}
			}
		}
		// scroll between  Heroes ( UP/DONW)  VK_PRIOR && VK_NEXT
		//if( LastKey == VK_UP || LastKey == VK_DOWN || LastKey == VK_PRIOR || LastKey == VK_NEXT ) {
		//	int  curPL  = m_Scenario.m_inLastHeroClick/100;
		//	int  curHR  = m_Scenario.m_inLastHeroClick%100;
		//	if(m_Scenario.m_inActivMenu==1)  curPL = m_pSect->GetSecOwner(m_Scenario.m_inLastLMCSecID);//new
		//	int  heroID = 0;			//curHR ;
		//	int  nextID = curPL + 1;
		//	int  next = -1;
		//	if( LastKey == VK_DOWN || LastKey == VK_NEXT || LastKey == VK_RIGHT ) {	// find next alive	// PAGE DOWN
		//		for(int i=nextID; i < MAX_PLAYERS && next < 0 ; i++) 
		//			if(  m_pPlayer[i]->m_Alive &&
		//				(m_Scenario.m_inActivMenu!=1 || m_pSect->GetPlayerSect(i)!=0) ) next = i ;//new IF condition
		//		nextID = next;
		//	}
		//	int  prev = -1;
		//	if( LastKey == VK_UP || LastKey == VK_PRIOR || LastKey == VK_LEFT ) {	//find prev alive	// PAGE UP
		//		for(int i=0; i < curPL ; i++)
		//			if(  m_pPlayer[i]->m_Alive &&
		//				(m_Scenario.m_inActivMenu!=1 || m_pSect->GetPlayerSect(i)!=0) )  prev = i; //new IF condition
		//		nextID = prev;
		//	}
		//	LastKey = KeyPressed = 0;
		//	if( nextID >= 0 && nextID < MAX_PLAYERS  && m_Scenario.m_inActivMenu < 3 ) {
		//		//CHero* pH = m_pPlayer[ nextID ]->GetHeroByPID( heroID );
		//		CHero*  pH = &(m_pPlayer[ nextID ]->m_phrHero[ 0 ]);
		//		if( pH )  heroID = pH->GetHeroID();
		//		pH = m_pPlayer[ nextID ]->GetHeroByPID( heroID );
		//		if( pH ) {
		//			m_Scenario.m_inPlayerInUseHero	  =  nextID ;
		//			m_Scenario.m_inLastHeroClick	  =  heroID + nextID * 100 ;
		//			m_Scenario.m_inLastLMCSecID		  =  pH->getSectorID() ;
		//			if(m_Scenario.m_inActivMenu==1) m_Scenario.m_inLastLMCSecID = m_pSect->GetSecIDbyOwner(nextID);
		//			if(m_Scenario.m_inLastLMCSecID < 0 || m_Scenario.m_inLastLMCSecID >= m_pSect->GetSectorsNum() ) {//new
		//				m_Scenario.m_inLastLMCSecID	  =  pH->getSectorID(); } //new
		//			m_pSect->m_LastLMCSecID			  =  m_Scenario.m_inLastLMCSecID ;
		//				m_phpDesk->m_iCurPlayer  = -1; // personal
		//				m_phpDesk->m_Visible	 = false;
		//				m_pHmm->SetAll( pH );
		//				m_pHPers->setPlayerID( nextID );
		//				
		//				PlayEffect( pH->m_psdMapPic->ClickSound ,0,0);//new
		//			if(!m_Scenario.m_bHeroMiniMap)  m_Scenario.m_bHeroMiniMap = (m_Scenario.m_inActivMenu==0);
		//			ROOT_SCENARIO.ShowInGameSector();
		//		}
		//	}
		//}
		// ESCAPE  capture  hero
		if( LastKey == VK_ESCAPE ) { 									
			if( m_Scenario.m_inActivMenu < 2 && ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture ) {
				int  curPL = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick/100;
				int  curHR = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick%100;
				if( curPL >=0 && curPL < MAX_PLAYERS && curHR >= 0 && curHR < MAX_HEROES ) {
					CHero*	pH = m_pPlayer[ curPL ]->GetHeroByPID( curHR );
					if( pH && pH->m_CurStatus == CHero::CAPTURE ) {
						LastKey = KeyPressed = 0;
						ClipCursor(NULL);
						pH->m_CurStatus	= CHero::FREE;
						m_Scenario.m_bMapMouseMove		 = true;
						m_Scenario.m_bSectMouseMove		 = true;
						m_Scenario.m_bSectSetPlayerColor = true;
						m_Scenario.m_bPlayerHeroCapture	 = false;
						m_Scenario.m_inPlayerInUseHero	 = -1;
						m_Scenario.m_inSectNeighbor		 = -1;
						UPDATE_SCENARIO();
					}
				}
			}
		}

		// BIG MAP CHEATS
		int start0 = 1, endN = start0+5;
 		if( this->m_bSaveLogs && ( LastKey == 'X' || LastKey == 'x' || m_CheatsStage > start0 ) ) {		
			_str  text;   
			if( m_CheatsStage < start0 ) { // enter
				m_CheatsStage++;
				m_ptbText->SetMessage( " " );
			} 
			else { 
				// quit
				if( ( m_CheatsStage > start0 && ( LastKey == 'X' || LastKey == 'x' ) ) ||
					( m_CheatsStage > endN   &&   LastKey == VK_BACK ) ) {
					m_CheatsStage = 0;
				}
				// process 
				if( m_CheatsStage == start0+5 && ((LastKey >= 0x30 && LastKey < 0x30 + MAX_RANKS) || LastKey == VK_BACK ) ) //+			
				{
					m_CheatsStage++;	// Add Hero Rank	
					if(m_ptbText)	text += m_ptbText->Message;			text += " \\ =";
					CHero*  pH = NULL;
					int curPL = gi_bmSAVE_Nation;
					if( curPL >= 0 && curPL < MAX_PLAYERS ) {	
						int RankExp[MAX_RANKS] = { LTN_EXPS_LIMIT, CPT_EXPS_LIMIT, MJR_EXPS_LIMIT, COL_EXPS_LIMIT, GEN_EXPS_LIMIT, MAR_EXPS_LIMIT };
						//pH = m_pPlayer[ curPL ]->GetHeroByPID( 0 );
						pH = &(ROOT_SCENARIO.m_pPlayer[ curPL ]->m_phrHero[ 0 ]);
						if( LastKey != VK_BACK && pH ) {							
							pH->m_inExperience = RankExp[ LastKey - 0x30 ]-100;
							pH->UpdateRankWithExperience();
						}
					}
					if(pH)  text += GetRankByID( pH->GetRank() );		
					text += "\\ {CG}done.";						
					if(m_ptbText)	m_ptbText->SetMessage( text.pchar() );
				}
				if( m_CheatsStage == start0+4 && ( LastKey == 0x6b || LastKey == 0xbb || LastKey == VK_BACK ) ) //+			
				{
					m_CheatsStage++;	// Add Hero Fuel	
					if(m_ptbText)	text += m_ptbText->Message;			text += " \\ =";
					CHero*  pH = NULL;
					int curPL = gi_bmSAVE_Nation;
					if( curPL >= 0 && curPL < MAX_PLAYERS ) {
						//pH = m_pPlayer[ curPL ]->GetHeroByPID( 0 );
						pH = &(ROOT_SCENARIO.m_pPlayer[ curPL ]->m_phrHero[ 0 ]);
						if( LastKey != VK_BACK && pH ) {
							pH->SetFuel( 1000 );
						}
					}
					if(pH)  text += pH->GetFuel();						text += " \\Hero Rank: ";
					if(m_ptbText)	m_ptbText->SetMessage( text.pchar() );
				}
				if( m_CheatsStage == start0+3 && ( LastKey == 0x6b || LastKey == 0xbb || LastKey == VK_BACK ) ) //+
				{
					m_CheatsStage++;	// Add Recruits	
					if(m_ptbText)	text += m_ptbText->Message;			text += " \\ =";
					int curPL = gi_bmSAVE_Nation;
					if( LastKey != VK_BACK ) {							
						for(int sec=0; sec < m_pSect->GetSectorsNum() ; sec++) 
							if( m_pSect->GetSecOwner(sec) == curPL )
								m_pSect->m_pssdDT[sec]->m_iRecruits=(m_pSect->m_pssdDT[sec]->m_inPopulation+1)*SECT_MAX_RECRTS;
						m_pPlayer[ curPL ]->SetRecruit();
					}
					text += m_pPlayer[ curPL ]->GetResource( RECRT );	text += " \\Hero Fuel: ";
					if(m_ptbText)	m_ptbText->SetMessage( text.pchar() );
				}
				if( m_CheatsStage == start0+2 && ( LastKey == 0x6b || LastKey == 0xbb || LastKey == VK_BACK ) ) //+
				{
					m_CheatsStage++;	// Add Resources	
					if(m_ptbText)	text += m_ptbText->Message;			text += " \\ +";
					int ADDRES = 0;
					if( LastKey != VK_BACK ) {							
						ADDRES = 100000;
						for(int res=0; res<=COAL ; res++) 
							ROOT_SCENARIO.m_pPlayer[gi_bmSAVE_Nation]->AddResource( res , ADDRES ); 
					}
					text += ADDRES;										text += " \\Recruits: "; 
					if(m_ptbText)	m_ptbText->SetMessage( text.pchar() );
				}
				if( m_CheatsStage == start0+1 && ((LastKey >= 0x30 && LastKey < 0x30 + MAX_PLAYERS) || LastKey == VK_BACK ) )	// Nation ID
				{
					m_CheatsStage++;	// Nation ID	
					if(m_ptbText)  text += m_ptbText->Message;			text += " \\ ";
					if( LastKey != VK_BACK )  
						gi_bmSAVE_Nation = LastKey - 0x30;
					text += gi_bmSAVE_Nation;							text += " \\res: ";
					if(m_ptbText)	m_ptbText->SetMessage( text.pchar() );
				}
				if( m_CheatsStage == start0 ) { //prepare
					m_CheatsStage++;
					if(m_ptbText)  m_ptbText->SetMessage( "{CW}ON \\nation: " );
				}
			}
			// end processing
			LastKey = KeyPressed = 0;
			if(m_ptbText)  m_ptbText->Visible = ( m_CheatsStage > start0-1 ); 
		}

		m_Scenario.m_bMenuIsChange = true;
		return true;
	}
	return false;
}
//----------------------------------------------
//----------------------------------------------
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
////////////// CPERSONAL ///////////////
////////////////////////////////////////
//----------------------------------------------
CPERSONAL::CPERSONAL()
{
	INIT();
}
//----------------------------------------------
CPERSONAL::CPERSONAL( int PlayerID )
{
	INIT();
	setPlayerID( PlayerID );
}
//----------------------------------------------
CPERSONAL::CPERSONAL( int PlayerID, int HeroID )
{
	INIT();
	m_PlayerID = PlayerID;
	//m_iHumanNat		= -1 ;
	//m_HeroID = HeroID;
}
//----------------------------------------------
CPERSONAL::~CPERSONAL()
{
	INIT();
	//if( m_ppUnitsPic != NULL ) {
	//	delete[]m_ppUnitsPic;
	//	m_ppUnitsPic = NULL;
	//}
}
//----------------------------------------------
void	CPERSONAL::INIT()
{
	m_utUNITS.UNIT_NID.Clear();
	m_utUNITS.UNIT_PIC.Clear();

	m_PlayerID		=  0 ;
	m_HeroID		=  0 ;
	m_iHumanNat		= -1 ;				
	m_ActPage		=  0 ;
	m_CurHeroId		= -1 ;

	for(int i=0; i < m_nFH ; i++)			m_pgppFrameH[i]  = NULL;
	
	for(int i=0; i < m_nFU ; i++)			m_pgppFrameU[i]  = NULL;
	for(int i=0; i < m_nFU ; i++)			m_ptbFrameUN[i]  = NULL;
	for(int i=0; i < m_nBT ; i++)			m_pgpbButton[i]  = NULL;
	for(int i=0; i < m_nPgs ; i++)			m_pgpbPages[i] = m_pgpbPagBak[i] = NULL;

	for(int i=0; i < m_nTXT ; i++)			m_ptbHeroPar[i]  = NULL;
	for(int i=0; i < MAX_PLAYERS ; i++)		m_pPLAYERS[i]	 = NULL;

	m_pgppBigPort	= NULL;
	for(int i=0; i < m_nFU ; i++)			m_ppUnitsPic[i] = NULL ;

	HeroDesc		= NULL;
	InfoDesk		= NULL;
	ptbHeroDesc		= NULL ;

	m_Visible = false ;
}
//----------------------------------------------
void	CPERSONAL::setPLAYERS(CPlayer** pPL){
	if ( pPL!=NULL ) {
		for ( int i=0; i<MAX_PLAYERS; i++ ){
			m_pPLAYERS[i] = pPL[i];
		}
	}
}
//----------------------------------------------
void	CPERSONAL::setPlayerID( int id ) {
	if( 0 <= id && id < MAX_PLAYERS ) {
		setVisible(false);
		m_PlayerID = id;
		m_HeroID   = 0;
		UpdateData();
	}
}
//----------------------------------------------
void	CPERSONAL::setVisible( bool state, bool type )
{
	m_Visible = state;

	if ( state == false ) {
		int i=0;
		for ( i=0; i<m_nFH; i++ ){
			if (m_pgppFrameH[i]!=NULL) {
				m_pgppFrameH[i]->Visible = false;
			}	
		}
		for ( i=0; i<m_nFU; i++ ){
			if (m_pgppFrameU[i]!=NULL) {
				m_pgppFrameU[i]->Visible = false;
			}
			if (m_ptbFrameUN[i]!=NULL) {
				m_ptbFrameUN[i]->Visible = false;
			}
		}

		//grey
		for ( i=0; i<m_nBT; i++ ){
			if (m_pgpbButton[i]!=NULL) {
				m_pgpbButton[i]->Visible = false;
			}
		}
		for ( i=0; i<m_nTXT; i++ ){
			if (m_ptbHeroPar[i]!=NULL) {
				m_ptbHeroPar[i]->Visible = false;
			}
		}
		for(i=0; i < m_nPgs ; i++) {	
			if (m_pgpbPages[i]!=NULL) {
				m_pgpbPages[i]->Visible = false;
			}
			if (m_pgpbPagBak[i]!=NULL) {
				m_pgpbPagBak[i]->Visible = false;
			}
		}


		if (m_pgppBigPort!=NULL) {
			m_pgppBigPort->Visible = false;
		}
		setUVisible( false );

		if(InfoDesk)	{ InfoDesk->Visible		= false;	InfoDesk->Enabled	 = false; }
		if(HeroDesc)	{ HeroDesc->Visible		= false;	HeroDesc->Enabled	 = false; }
		if(ptbHeroDesc) { ptbHeroDesc->Visible  = false;	ptbHeroDesc->Enabled = false; }
	}
	else {
		if (!type) {
			UpdateData();
		}
		else{
			UpdateData(m_HeroID);
		}
	}
}
//----------------------------------------------
void	CPERSONAL::setUVisible( bool state )
{
	for(int i=0; i < m_utUNITS.UNIT_PIC.GetAmount() ; i++) {
		if( m_utUNITS.UNIT_PIC[i] )    m_utUNITS.UNIT_PIC[i]->Visible = false; 
	}
}
//----------------------------------------------
void	CPERSONAL::setFarme( DialogsSystem* pMenu, LocalGP* pFR , CArmyLimit* pAL )
{
	if ( pMenu==NULL ) { return; }

	if ( pFR!=NULL ) 
	{
		m_pgppFrameH[0] = pMenu->addGPPicture(NULL,0,0,pFR->GPID, 15 ) ; // 1 ); 
		m_pgppFrameH[0]->ShapeFileID = pFR->GPID;		m_pgppFrameH[0]->ShapeSpriteID = 15 ; // 1 ); 
		//m_pgppFrameH[1] = pMenu->addGPPicture(NULL,0,0,pFR->GPID, 16 ) ; // 1 ); 
		//m_pgppFrameH[1]->ShapeFileID = pFR->GPID;		m_pgppFrameH[1]->ShapeSpriteID = 16 ; // 1 ); 
		//m_pgppFrameH[2] = pMenu->addGPPicture(NULL,0,0,pFR->GPID, 1 ) ; 
		//m_pgppFrameH[2]->ShapeFileID = pFR->GPID;		m_pgppFrameH[2]->ShapeSpriteID = 1 ; 
		
		m_pgppFrameH[3] = pMenu->addGPPicture(NULL,0,0,pFR->GPID, 7 ) ; //0);
		m_pgppFrameH[3]->ShapeFileID = pFR->GPID;		m_pgppFrameH[3]->ShapeSpriteID = 7 ; //0;
		
		setUnis( pMenu , pAL );//!!!!!
		//for ( i=0; i<m_nFH; i++ ){
			//m_pgppFrameH[i] = pMenu->addGPPicture(NULL,0,0,pFR->GPID, 1 ) ; //0);
			//m_pgppFrameH[i]->ShapeFileID	 = pFR->GPID;
			//m_pgppFrameH[i]->ShapeSpriteID = 1 ; //0;
		//}
		for(int i=0; i < m_nFU ; i++ ) {
			m_pgppFrameU[i] = pMenu->addGPPicture(NULL,0,0,pFR->GPID, 8 ) ; 
			m_pgppFrameU[i]->ShapeFileID	= m_pgppFrameU[i]->FileID ; //pFR->GPID;
			m_pgppFrameU[i]->ShapeSpriteID	= m_pgppFrameU[i]->SpriteID ; //8 ;
			//m_ptbFrameUN[i] = pMenu->addTextButton(NULL,0,0,"0",&WhiteFont,&WhiteFont,&WhiteFont,1);
		}

		_str	a_HintAdd ,  a_HintDel ;
		a_HintAdd  = "{FS}{C FF502515}" ;					a_HintDel  = "{FS}{C FF502515}" ;
		a_HintAdd += GetTextByID( "#CHINT_AddSquad" );		a_HintDel += GetTextByID( "#CHINT_DelSquad" );	

		for(int i=0; i < m_nBT ; i+=2 ) {
			m_pgpbButton[i] = pMenu->addGP_Button(NULL,0,0,pFR->GPID, 13 , 12 ) ;	// -   #CHINT_DelSquad
			m_pgpbButton[i]->ShapeFileID	= pFR->GPID;
			m_pgpbButton[i]->ShapeSpriteID	=  12 ;
			DYNHINT_RA( m_pgpbButton[i] , a_HintDel.pchar() );
			m_pgpbButton[i+1] = pMenu->addGP_Button(NULL,0,0,pFR->GPID, 10 , 9 ) ;	// +   #CHINT_AddSquad
			m_pgpbButton[i+1]->ShapeFileID	= pFR->GPID;
			m_pgpbButton[i+1]->ShapeSpriteID=  9 ;
			DYNHINT_RA( m_pgpbButton[i+1] , a_HintAdd.pchar() );
			//m_pgpbButton[i]->ActiveID		= 10 ;	m_pgpbButton[i]->PassiveID		= 11 ;
		}

		m_pgpbPagBak[0] = pMenu->addGP_Button(NULL,0,0,pFR->GPID, 18 , 18 ) ; // pas=17   act=18   /dis=19
		m_pgpbPagBak[1] = pMenu->addGP_Button(NULL,0,0,pFR->GPID, 17 , 17 ) ;
		m_pgpbPagBak[2] = pMenu->addGP_Button(NULL,0,0,pFR->GPID, 17 , 17 ) ;
		m_pgpbPages[0]  = pMenu->addGP_Button(NULL,0,0,pFR->GPID, 21 , 21 ) ; // pas=20   act=21   /dis=22
		m_pgpbPages[1]  = pMenu->addGP_Button(NULL,0,0,pFR->GPID, 23 , 23 ) ; // pas=23   act=24   /dis=25
		m_pgpbPages[2]  = pMenu->addGP_Button(NULL,0,0,pFR->GPID, 26 , 26 ) ; // pas=26   act=27   /dis=28
		for(int i=0; i < m_nPgs ; i++) {
			m_pgpbPagBak[i]->UserParam	 = 0 ;
			m_pgpbPagBak[i]->OnUserClick = &OnPIButtonClicked ;
			m_pgpbPagBak[i]->AssignSound( "INTERF_PAGE", CLICK_SOUND );
		}
		
		_str	a_Hint;
		a_Hint = "{FS}{C FF502515}";				a_Hint += GetTextByID( "#CPT_Infantry" );		
		DYNHINT_RA( m_pgpbPages[0] , a_Hint.pchar() );
		a_Hint.DString::Clear();		
		a_Hint = "{FS}{C FF502515}";				a_Hint += GetTextByID( "#CPT_Cavalry" );		
		DYNHINT_RA( m_pgpbPages[1] , a_Hint.pchar() );
		a_Hint.DString::Clear();
		a_Hint = "{FS}{C FF502515}";				a_Hint += GetTextByID( "#CPT_Artillery" );		
		DYNHINT_RA( m_pgpbPages[2] , a_Hint.pchar() );
	}

	CHero*	pH = &(m_pPLAYERS[m_PlayerID]->m_phrHero[ 0 ]) ; //m_HeroID]);
	if( pH ) { m_pgppBigPort = reinterpret_cast<GPPicture*>( pH->GetBigPortret() ); }
		
	for(int i=0; i < m_nFU ; i++ ) {// visible SQ num?? 
		m_ptbFrameUN[i] = pMenu->addTextButton(NULL,0,0,"0/0",&RedFont,&RedFont,&RedFont, 0 /*1*/ ); //&RedFont,&RedFont,&RedFont, 1 );
	}

	for(int i=0; i < m_nTXT ; i++) {
		m_ptbHeroPar[i] = pMenu->addTextButton(NULL,0,0, "     " /*"00000"*/ ,&BlackFont,&BlackFont,&BlackFont, 0 );
	}

	// инф-я по герою					//heroinf00.txt
	//int  idH = m_pgppFrameH[1]->y1 - m_pgppFrameH[1]->y ;
	//InfoDesk	= pMenu->AddDialogsDesk( 381, 112, 301, idH -1, "BD" ); ///"EmptyBorder");
	InfoDesk	= pMenu->AddDialogsDesk( 382, 112, 276, 392 , "BD" ); ///"EmptyBorder");
	ptbHeroDesc	= InfoDesk->addTextButton(NULL, 17 /*15*/, 15 /*10*/, GetTextByID("#HeroInfo00") ,&BlackFont,&BlackFont,&BlackFont,0); //NEED NEW FONTS 
	ptbHeroDesc->MaxWidth = InfoDesk->x1 - InfoDesk->x -24 /*-55*/ /*-51*/ ;

	setVisible(false);
}
//----------------------------------------------
bool	CPERSONAL::setEnabledButtons( bool state ) //new ( from  ::setVisible( bool state, bool type ) )
{
	for(int i=0; i<m_nFH; i++ )
		if( m_pgppFrameH[i] && m_pgppFrameH[i]->Visible )	m_pgppFrameH[i]->Enabled = state;

	for(int i=0; i < m_nFU ; i++ ) {
		if( m_pgppFrameU[i] && m_pgppFrameU[i]->Visible )   m_pgppFrameU[i]->Enabled = state;
		if( m_ptbFrameUN[i] && m_ptbFrameUN[i]->Visible )   m_ptbFrameUN[i]->Enabled = state;
		if( m_ppUnitsPic[i] && m_ppUnitsPic[i]->Visible )	m_ppUnitsPic[i]->Enabled = state;   
	}
	for(int i=0; i<m_nBT; i++ )
		if( m_pgpbButton[i] && m_pgpbButton[i]->Visible )	m_pgpbButton[i]->Enabled = state;

	for(int i=0; i<m_nTXT; i++ )
		if( m_ptbHeroPar[i] && m_ptbHeroPar[i]->Visible )	m_ptbHeroPar[i]->Enabled = state;

	for(int i=0; i < m_nPgs ; i++) {	
		if( m_pgpbPages[i]  && m_pgpbPages[i]->Visible )	m_pgpbPages[i]->Enabled  = state;
		if( m_pgpbPagBak[i] && m_pgpbPagBak[i]->Visible )	m_pgpbPagBak[i]->Enabled = state;
		}
	if( m_pgppBigPort && m_pgppBigPort->Visible )			m_pgppBigPort->Enabled   = state;

	if( InfoDesk	&& InfoDesk->Visible )		{ InfoDesk->Enabled		= state; } //(*InfoDesk).EnableVScroll = (*InfoDesk).EnableHScroll = state; }
	if( HeroDesc	&& HeroDesc->Visible )		{ HeroDesc->Enabled		= state; }
	if( ptbHeroDesc	&& ptbHeroDesc->Visible )	{ ptbHeroDesc->Enabled  = state; }

	return state ;
}
//----------------------------------------------
void	CPERSONAL::setUserClick(pCULL_FUN_CLICK pf, pCULL_FUN_CLICK pfR ) 
{
	for(int i=0; i < m_nBT ; i+=2 )  
		if( m_pgpbButton[i] && m_pgpbButton[i+1] ) {
			m_pgpbButton[i]->UserParam		= i/2;
			m_pgpbButton[i]->OnUserClick	= pfR  ;
			m_pgpbButton[i+1]->UserParam	= i/2;
			m_pgpbButton[i+1]->OnUserClick	= pf ;
		}
	for(int i=0; i < m_utUNITS.UNIT_PIC.GetAmount() ; i++) 
		if( m_utUNITS.UNIT_PIC[i] ) {
			m_utUNITS.UNIT_PIC[i]->OnUserClick		= pf  ;
			m_utUNITS.UNIT_PIC[i]->OnUserRightClick = pfR ;
		}
}
//----------------------------------------------
void	CPERSONAL::CreateUnitsHint( SimpleDialog* psd , int Nation , word NIndex )
{	
	//
	//int    list[7]		= { WOOD, FOOD, STONE, GOLD, IRON, COAL } ;
	//char*  resName[50]	= { "WOOD", "FOOD", "STONE", "GOLD" , "IRON" , "COAL" };
	//EngSettings.Resource.SetHint(NI,Cost,&txt);
	//

	bool    fade = false, noRECRT = false ;
	CHero*  pH   = m_pPLAYERS[Nation]->GetHeroByPID(m_HeroID);	//m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
	int		BrID = GetObjectBranch( NIndex );

	_str	tBlack,tBrown,tRed; tBlack = "{FS}{C FF101010}";  tBrown = "{FS}{C FF502515}";  tRed = "{C FF640100}";

	char	a_MonChar[512];
	GetChar(NATIONS[0].Mon[ NIndex ], a_MonChar ); 

	int		sqCOST[8], sqNUM = 1;	
	GetBigMapSquadCost( Nation , NIndex , sqNUM , sqCOST );

	//
	_str	a_MonCost;
	EngSettings.Resource.SetHint( Nation, sqCOST, &a_MonCost);//new
	//

	//_str	a_MonCost;
	for(int res=0; res<=COAL ; res++) 
		if( sqCOST[res] > 0 ) {
			//a_MonCost += tBrown ;
			//a_MonCost += "\\ " ; 
			//a_MonCost += RDS[res].Name ;
			//a_MonCost += ":  " ;
			//a_MonCost += tBlack ;
			//a_MonCost += sqCOST[res] ;	
			if( m_pPLAYERS[Nation]->GetResource( res ) < sqCOST[res] )  fade = true;
		}
	if( m_pPLAYERS[Nation]->GetResource( RECRT ) < sqNUM )  noRECRT = true;


	// ++ GET ATACK-DEFENCE-GERNADES PARAMs For NIndex-Unit !!! new
	_str	a_Hint;			a_Hint.DString::Clear();
	bool	find = false;
	if( pH ) {
		for(int sq=0 ; sq < pH->ARMY.GetAmountOfElements() ; sq++)
			if( pH->ARMY[ sq ]->getTypeGlobID() == NIndex ) {
				find = true;
				//a_Hint = "{FM}";
				//a_Hint += tRed;
				//a_Hint += "     ";
				//a_Hint += pH->ARMY[sq]->TypeName.pchar(); 
				//a_Hint += "\\ ";									// a_Hint += " \\  \\ " ;
				a_Hint  = tBlack;
				a_Hint += a_MonChar;
				a_Hint += " \\ ";
				a_Hint += tBlack;
				if( pH->ARMY[sq]->MeleeDMG > 0  && BrID != 2 ) {
					a_Hint += GetTextByID( "#CPT_MeleeDMG" );
					a_Hint += tRed;	
					a_Hint += "  ";
					a_Hint += pH->ARMY[sq]->MeleeDMG ;
					a_Hint += "{C}";
				}
				if( pH->ARMY[sq]->RifleDMG > 0  && BrID != 2 ) {
					a_Hint += GetTextByID( "#CPT_RifleDMG" );					//a_Hint += " / " ;
					//a_Hint += tRed;	
					a_Hint += "  ";
					a_Hint += pH->ARMY[sq]->RifleDMG ;
					//a_Hint += "{C}";
				}
				//if( pH->ARMY[sq]->GrenadeDMG > 0  && BrID != 2 ) {
				//	a_Hint += GetTextByID( "#CPT_GrenadDMG" );
				//	//a_Hint += tRed;	
				//	a_Hint += "  ";
				//	a_Hint += pH->ARMY[sq]->GrenadeDMG ;
				//	//a_Hint += "{C}";
				//}
				break;
			}
	}
	if( !pH || !find )
		a_Hint += a_MonChar ;
	// ++ GET ATACK-DEFENCE-GERNADES PARAMs For NIndex-Unit !!! new

	//a_Hint += a_MonChar ; //new
	//a_Hint += " \\  \\ " ;
	a_Hint += " \\ " ;
	a_Hint += a_MonCost ;
	a_Hint += BIGMAP_RECRTICON ;	
	//a_Hint += tBrown ;	//a_Hint += " \\ " ;
	//a_Hint += GetTextByID( "ResName9" ) ;
	//a_Hint += ": " ;
	a_Hint += tBlack ;
	if( Nation == gi_bmSAVE_Nation && noRECRT ){
		a_Hint += "{CR}"; //a_Hint += tRed ;
	}
	a_Hint += sqNUM ;
	a_Hint += tBrown ;

	//int  BrID = GetObjectBranch( NIndex );
	//CHero*  pH = m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
	if( pH ) {						//&& GetObjectBranch( NIndex ) < 2 ) {
		a_Hint += " \\  \\ " ;		
		a_Hint += GetTextByID( "#CPT_Kills" ) ;		
		a_Hint += tBlack ;
		a_Hint += "  " ;
		for(int sq=0 ; sq < pH->ARMY.GetAmountOfElements() ; sq++)
			if( pH->ARMY[ sq ]->getTypeGlobID() == NIndex ) { 
					int  kills = 0;
					for(int k=0; k < pH->ARMY[sq]->Kills.GetAmountOfElements() ; k++) kills += pH->ARMY[sq]->Kills[k];
					if( kills > 0 ) {
						for(int k=0; k < pH->ARMY[sq]->getSQ_NUM() ; k++) {
							if( k != 0 ) a_Hint += " / " ;
							if( k < pH->ARMY[sq]->Kills.GetAmountOfElements() ) {	
								a_Hint += pH->ARMY[sq]->Kills[ k ] ; 
							} else { a_Hint += "0" ; }							
						}
					} else { a_Hint += "0" ; }							
					break;
				}
	}
	if( Nation == gi_bmSAVE_Nation ) {
		a_Hint += tBrown ;
		a_Hint += " \\  \\ " ;		a_Hint += GetTextByID( "#CWM_Left_Click_OnSqd" );
		a_Hint += " \\ " ;			a_Hint += GetTextByID( "#CWM_Right_Click_OnSqd" );
		
		// chekin Recruits											// m_pPLAYERS[Nation]->GetResource( RECRT ) < sqNUM 
		psd->Diffuse = 0xFFFFFFFF;
		if( fade || noRECRT )					psd->Diffuse = 0x65FFFFFF; // 0x99FFFFFF;
		// chekin GetARMY_FreeNSQ 
		//
		if( m_pPLAYERS[m_PlayerID]->m_inHeroNum > 0 ){
			CHero*	pH = &(m_pPLAYERS[m_PlayerID]->m_phrHero[ 0 ]) ;
			if( pH && pH->GetARMY_FreeNSQ() == 0 )	psd->Diffuse = 0x65FFFFFF; //  0xbd00B500; // 0x8800B500
		}
		//
	}
	DYNHINT_RA( psd , a_Hint.pchar() );
}
//----------------------------------------------
void	CPERSONAL::setUnis( DialogsSystem* pMenu, CArmyLimit* pAL )
{
	if ( pMenu==NULL || pAL==NULL )			{ return; }

	int				uid = 0;
	SimpleDialog*	psd = NULL;
	int				fid = 0;
	int				sid = 0;
	for(int rank=0; rank < MAX_RANKS; rank++ ) {
		for(int cont=0; cont < MAX_COUNTRYS; cont++ ) {
			for(int unit=0; unit < pAL->GetMaxDiffSqd(rank,cont); unit++ ) 
			{
				uid = pAL->GetNSqdNID(rank,cont,unit);
				if( uid < 0 ) continue;

				fid = NATIONS[0].Mon[uid]->newMons->MinIconFile ;		//InMenuIconFile;			//fid = NATIONS[0].Mon[uid]->newMons->InMenuIconFile;
				sid = NATIONS[0].Mon[uid]->newMons->MinIconIndex ;		//InMenuIconIndex;			//sid = NATIONS[0].Mon[uid]->newMons->InMenuIconIndex;

				if( fid == 0xFFFF ) {									//|| sid == 0xFFFF ) { 
					fid = NATIONS[0].Mon[uid]->newMons->IconFileID;
					sid = NATIONS[0].Mon[uid]->newMons->IconID;
				}
				if( fid == 0xFFFF ) { 
					fid = NATIONS[0].Mon[uid]->newMons->InMenuIconFile;
					sid = NATIONS[0].Mon[uid]->newMons->InMenuIconIndex;
				}
				if( fid != 0xFFFF && sid != 0xFFFF ) { 
					psd = pMenu->addGPPicture(NULL,0,0,fid,sid);
					psd->Visible		= false;
					psd->ShapeFileID	= fid;
					psd->ShapeSpriteID	= sid;
					psd->UserParam = unit ; // --- FOR CLICKS ON UNITS ICONS					
				}
				else	{ continue; } //psd = NULL; }
				
				m_utUNITS.UNIT_NID.Add( uid );
				m_utUNITS.UNIT_PIC.Add( psd );

				// SQUAD COST hint
				if( psd && uid >= 0 ) {					
					CreateUnitsHint( psd , cont , uid );
				}
			}
		}
	}
	
	//for ( int i=0; i<m_nFU; i++ ) {
	//	m_ptbFrameUN[i] = pMenu->addTextButton(NULL,0,0,"0/0",&RedFont,&RedFont,&RedFont, 1 );
	//}

	setVisible(false);
}
//----------------------------------------------
void	CPERSONAL::UpdateData()
{
	int i = 0;
	//	CHero* pH = m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
	CHero*	pH = &(m_pPLAYERS[m_PlayerID]->m_phrHero[ 0 ]) ; //m_HeroID]);
	if( !pH )	{ return; }
	//UpdateInfo( pH );

	for ( i=0; i<m_nFH; i++ ){
		if( m_pgppFrameH[i]!=NULL )	m_pgppFrameH[i]->Visible = true;//TEMP
	}
	for ( i=0; i<m_nFU; i++ ) {
		m_pgppFrameU[i]->Visible = false;
		m_ptbFrameUN[i]->Visible = false;

		int a = 2*i , b = a + 1 ;
		if( m_pgpbButton[ a ] && m_pgpbButton[ b ] ) { 
				m_pgpbButton[ a ]->Visible = false;
				m_pgpbButton[ b ]->Visible = false;
		}
	}
	if (pH!=NULL && m_pgppBigPort ) {
		m_pgppBigPort = reinterpret_cast<GPPicture*>(pH->GetBigPortret());
		m_pgppBigPort->Visible = true;
	}

	for(i=0; i < m_nTXT ; i++)	if( m_ptbHeroPar[i] ) m_ptbHeroPar[i]->Visible = true;

	for(i=0; i < m_nPgs ; i++) {
		if( m_pgpbPages[i] )	m_pgpbPages[i]->Visible  = true;
		if( m_pgpbPagBak[i] )	m_pgpbPagBak[i]->Visible = true;
	}
	
	UpdateInfo( pH );
}
//----------------------------------------------
void	CPERSONAL::UpdateData(int heroPID)
{
	m_HeroID = heroPID;
	int i = 0;
	CHero* pH = m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
	if( !pH )	{ return; }

	UpdateSQ_NUM();
	for ( i=0; i<m_nFH; i++ ){
		if( m_pgppFrameH[i] )	m_pgppFrameH[i]->Visible = true;//TEMP
	}
	for ( i=0; i<m_nFU; i++ ){
		//if ( (pH!=NULL) && (i < pH->pArmyLimit->GetMaxDiffSqd(pH->GetRank(),pH->GetPlayerID())) ) {
		//	m_pgppFrameU[i]->Visible = true;
		//	m_ptbFrameUN[i]->Visible = true;
		//	//grey
		//	if ( pH->GetPlayerID() == m_iHumanNat ) {
		//		int a = 2*i , b = a + 1 ;
		//		if( m_pgpbButton[ a ] && m_pgpbButton[ b ] ) { 
		//			m_pgpbButton[ a ]->Visible = true;
		//			m_pgpbButton[ b ]->Visible = true;
		//		}
		//	}
		//}
		//else
		{
			m_pgppFrameU[i]->Visible = false;
			m_ptbFrameUN[i]->Visible = false;
			//grey
			int a = 2*i , b = a + 1 ;
			if( m_pgpbButton[ a ] && m_pgpbButton[ b ] ) { 
				m_pgpbButton[ a ]->Visible = false;
				m_pgpbButton[ b ]->Visible = false;
			}
		}
	}
	if( pH ) {
		m_pgppBigPort = reinterpret_cast<GPPicture*>(pH->GetBigPortret());
		m_pgppBigPort->Visible = true;
	}

	for(i=0; i < m_nTXT ; i++)	if( m_ptbHeroPar[i] ) m_ptbHeroPar[i]->Visible = true;

	for(i=0; i < m_nPgs ; i++) {
		if( m_pgpbPages[i] )	m_pgpbPages[i]->Visible = true;
		if( m_pgpbPagBak[i] )	m_pgpbPagBak[i]->Visible = true;
	}

	UpdateInfo( pH );

	UpdateUCoord();

	if( m_Visible ) 
	{
		m_pgpbPagBak[ m_ActPage ]->UserParam = 1;
		if( !CheckUnitsPages() ) {
			m_ActPage = 0;
			m_pgpbPagBak[0]->UserParam = 1;
			CheckUnitsPages();
			m_pgpbPages[0]->ActiveFrame  = m_pgpbPages[0]->PassiveFrame		= 21 ; // active
			m_pgpbPagBak[0]->ActiveFrame = m_pgpbPagBak[0]->PassiveFrame	= 18 ; // active
		}
	}
}

//void	CPERSONAL::UpdateData(int heroPID)
//{
//	m_HeroID = heroPID;
//	int i = 0;
//	CHero* pH = m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
//	if( !pH )	{ return; }
//	//UpdateInfo( pH );
//	//CHero*	pH = &(m_pPLAYERS[m_PlayerID]->m_phrHero[m_HeroID]);
//
//	UpdateSQ_NUM();
//	for ( i=0; i<m_nFH; i++ ){
//		if( m_pgppFrameH[i] )	m_pgppFrameH[i]->Visible = true;//TEMP
//	}
//	for ( i=0; i<m_nFU; i++ ){
//		//if ( (pH!=NULL) && (i < pH->pArmyLimit->GetMaxDiffSqd(pH->GetRank(),pH->GetPlayerID())) ) {
//		//	m_pgppFrameU[i]->Visible = true;
//		//	m_ptbFrameUN[i]->Visible = true;
//		//	//grey
//		//	if ( pH->GetPlayerID() == m_iHumanNat ) {
//		//		int a = 2*i , b = a + 1 ;
//		//		if( m_pgpbButton[ a ] && m_pgpbButton[ b ] ) { 
//		//			m_pgpbButton[ a ]->Visible = true;
//		//			m_pgpbButton[ b ]->Visible = true;
//		//		}
//		//	}
//		//}
//		//else
//		{
//			m_pgppFrameU[i]->Visible = false;
//			m_ptbFrameUN[i]->Visible = false;
//			//grey
//			int a = 2*i , b = a + 1 ;
//			if( m_pgpbButton[ a ] && m_pgpbButton[ b ] ) { 
//				m_pgpbButton[ a ]->Visible = false;
//				m_pgpbButton[ b ]->Visible = false;
//			}
//		}
//	}
//	if( pH ) {
//		m_pgppBigPort = reinterpret_cast<GPPicture*>(pH->getBigPortret());
//		m_pgppBigPort->Visible = true;
//	}
//
//	for(i=0; i < m_nTXT ; i++)	if( m_ptbHeroPar[i] ) m_ptbHeroPar[i]->Visible = true;
//
//	for(i=0; i < m_nPgs ; i++) {
//		if( m_pgpbPages[i] )	m_pgpbPages[i]->Visible = true;
//		if( m_pgpbPagBak[i] )	m_pgpbPagBak[i]->Visible = true;
//	}
//
//	UpdateInfo( pH );
//
//	UpdateUCoord();
//
//	if( m_Visible ) //!!!VERY NEW!!!
//	{
//		m_pgpbPagBak[ m_ActPage ]->UserParam = 1;
//		if( !CheckUnitsPages() ) 
//		{
//			m_ActPage = 0;
//			m_pgpbPagBak[0]->UserParam = 1;
//			CheckUnitsPages();
//			m_pgpbPages[0]->ActiveFrame  = m_pgpbPages[0]->PassiveFrame		= 21 ; // active
//			m_pgpbPagBak[0]->ActiveFrame = m_pgpbPagBak[0]->PassiveFrame	= 18 ; // active
//		}
//	}
//
//
//}

//----------------------------------------------
void	CPERSONAL::UpdateSQ_NUM() 
{
	//CHero* pH = m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
	//int NDIF = pH->ARMY.GetAmount();
	//CSquard*	pSQ = NULL;
	//int curID = 0;
	//char n[10];//grey	
	////char n[5];
	//for (int i=0;i<NDIF;i++) {
	//	pSQ = pH->ARMY[i];
	//	if (pSQ->getSQ_MAX()>0){
	//		sprintf(n,  "%d/%d", pSQ->getSQ_NUM() , pSQ->getSQ_MAX() );//grey	/sprintf(n,  "%d/%d\n", pSQ->getSQ_NUM() , pSQ->getSQ_MAX() );//grey	
	//		m_ptbFrameUN[curID]->SetMessage( n );
	//		//sprintf(n,"%d\n",pSQ->getSQ_NUM());
	//		//if (m_ptbFrameUN[curID]->Message)	delete[](m_ptbFrameUN[curID]->Message);
	//		//m_ptbFrameUN[curID]->Message = new char[strlen(n)+1];
	//		//strcpy(m_ptbFrameUN[curID]->Message, n );
	//		curID++;
	//	}
	//}
	//
}
//----------------------------------------------

void	CPERSONAL::UpdateUCoord()	// !!!UNDER CONSTRUCTION!!!!   //grey [25.02.2004]
{
	CHero* pH = m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
	if( !pH )			{ return; }

	int	rank = pH->GetRank();
	int cont = pH->GetPlayerID();
	int ndif = pH->pArmyLimit->GetMaxDiffSqd(rank,cont);

	for(int i=0; i < m_nFU ; i++)   m_ppUnitsPic[i] = NULL ; //grey [25.02.2004]
	
	int  id = 0;
	setUVisible( false );

	int  picID = 0 ;
	for(int ut=0; ut < ndif && ut < m_nFU ; ut++)
	{
		int typeID	= pH->pArmyLimit->GetNSqdNID(rank,cont, ut) ;
		int	id		= m_utUNITS.UNIT_NID.find( typeID );
		if( id < 0 )  continue;

		m_ppUnitsPic[picID]	= m_utUNITS.UNIT_PIC[ id ];

		if( !m_ppUnitsPic[ picID ] )					{ continue; }
		if( GetObjectBranch( typeID ) != m_ActPage )	{ continue; }

		if( m_pgppFrameU[ picID ] ) setCoordSD( m_ppUnitsPic[picID], m_pgppFrameU[picID]->x -1 , m_pgppFrameU[picID]->y -1 ); 
		m_pgppFrameU[ picID ]->Visible		= true ;
		m_ppUnitsPic[ picID ]->Visible		= true ;

		CreateUnitsHint( m_ppUnitsPic[ picID ] , cont , typeID );
		
		int	sqID = -1 ;
		for(int sq=0 ; sq < pH->ARMY.GetAmountOfElements() ; sq++)
			if( pH->ARMY[ sq ]->getTypeGlobID() == typeID )   { sqID = sq ; break; }

		if( sqID >= 0 ) {
			char n[10] ,  zero[10];
			if( pH->ARMY[ sqID ]->getSQ_NUM() == 1 )	{ sprintf(zero, " " ); /*"."*/ } else { sprintf(zero, ""); }
			sprintf(n,  "%s%d/%d", zero, pH->ARMY[ sqID ]->getSQ_NUM() , pH->ARMY[ sqID ]->getSQ_MAX() );

			m_ptbFrameUN[ picID ]->SetMessage( n );
			m_ptbFrameUN[ picID ]->Visible = true ;

			if ( pH->GetPlayerID() == m_iHumanNat ) {
				int a = 2*picID , b = a + 1 ;
				if( m_pgpbButton[ a ] ) { 
					m_pgpbButton[ a ]->UserParam	= sqID ;
					m_pgpbButton[ a ]->Visible		= true ;
				}
				if( m_pgpbButton[ b ] ) { 
					m_pgpbButton[ b ]->UserParam	= sqID ;
					m_pgpbButton[ b ]->Visible		= true ;
				}
			}
		}
		picID++ ;
	}
	UpdateSQ_NUM();
}
//void	CPERSONAL::UpdateUCoord()
//{
//	CHero* pH = m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
//	if( !pH )			{ return; }
//
//	int	rank = pH->GetRank();
//	int cont = pH->GetPlayerID();
//	int ndif = pH->pArmyLimit->GetMaxDiffSqd(rank,cont);
//
//	//grey [25.02.2004]
//	for(int i=0; i < m_nFU ; i++)   m_ppUnitsPic[i] = NULL ;
//	//m_ppUnitsPic = new SimpleDialog*[ndif];
//	//if (m_ppUnitsPic==NULL)			{ return; }
//	
//	int id = 0;
//	setUVisible( false );	
//	for(int ut=0; ut < ndif && ut < m_nFU ; ut++) {			// for(int ut=0; ut < ndif ; ut++) { 
//		id					= m_utUNITS.UNIT_NID.find(pH->pArmyLimit->GetNSqdNID(rank,cont,ut));
//		m_ppUnitsPic[ut]	= m_utUNITS.UNIT_PIC[id];
//
//		if( m_ppUnitsPic[ut] == NULL ) { continue; }	
//
//		if( m_pgppFrameU[ut] ) {
//			setCoordSD( m_ppUnitsPic[ut], m_pgppFrameU[ut]->x -1 , m_pgppFrameU[ut]->y -1 ); 
//		}
//		m_ppUnitsPic[ut]->Visible = true;
//	}
//
//	UpdateSQ_NUM();
//}

//----------------------------------------------
void	CPERSONAL::UpdateCoord()
{
	if( m_pgppBigPort ) {
		setCoordSD(dynamic_cast<SimpleDialog*>(m_pgppBigPort),    73, 112 ); //76, 115 );
	}
	if( m_pgppFrameH[0]) {
		setCoordSD(dynamic_cast<SimpleDialog*>(m_pgppFrameH[0]),  72, 111 );
	}
	if( m_pgppFrameH[1] ) {
		setCoordSD(dynamic_cast<SimpleDialog*>(m_pgppFrameH[1]), 381, 111 );
	}
	if( m_pgppFrameH[2] ) {
		setCoordSD(dynamic_cast<SimpleDialog*>(m_pgppFrameH[2]), 381, 350 );
	}
	if( m_pgppFrameH[3] ) {
		setCoordSD(dynamic_cast<SimpleDialog*>(m_pgppFrameH[3]),  72, 510 );
	}
	
	int IH = 0, IW = 0;
	if( m_pgppFrameU[0] ) {
		IH = GPS.GetGPHeight(m_pgppFrameU[0]->ShapeFileID,m_pgppFrameU[0]->ShapeSpriteID);
		IW = GPS.GetGPWidth (m_pgppFrameU[0]->ShapeFileID,m_pgppFrameU[0]->ShapeSpriteID);
	}
	int  bH = 0, bW = 0;
	if( m_pgpbButton[0] ) {
		bH = GPS.GetGPHeight(m_pgpbButton[0]->ShapeFileID,m_pgpbButton[0]->ShapeSpriteID);
		bW = GPS.GetGPWidth (m_pgpbButton[0]->ShapeFileID,m_pgpbButton[0]->ShapeSpriteID);
	}
	for ( int i=0; i<m_nFU; i++ ) { 
		if (m_pgppFrameU[i] != NULL) {
			if (i!=0) {
                setCoordSD(dynamic_cast<SimpleDialog*>(m_pgppFrameU[i]), m_pgppFrameU[i-1]->x1 +12, 517 ) ;
			}
			else{
				setCoordSD(dynamic_cast<SimpleDialog*>(m_pgppFrameU[i]), 84 , 517 ) ;
			}
			if( m_ptbFrameUN[i] ) {
				//new
				setCoordSD( m_ptbFrameUN[i],			0 , 0 );	
				TextButton_OnDraw( m_ptbFrameUN[i] );

				int udX = IW - (m_ptbFrameUN[i]->x1 + 1);			
				udX		= (udX + 1)/2 + 1 ;							// udX = (udX + 1)/2 ;

				//int udX = IW - m_ptbFrameUN[i]->x1 ;				// udX = IW - ( m_ptbFrameUN[i]->x1 + 1 );
				//udX		= udX / 2 + udX % 2 + 1 ;

				setCoordSD( m_ptbFrameUN[i],			m_pgppFrameU[i]->x  + udX ,	
														m_pgppFrameU[i]->y1 - 14 );	
				
				//m_pgppFrameU[i]->Diffuse = 0x99ffffff; //tmp


				//setCoordSD( dynamic_cast<SimpleDialog*>(m_ptbFrameUN[i]), 
				//	m_pgppFrameU[i]->x  + IW/2 - 9 ,	
				//	m_pgppFrameU[i]->y1 - 15 );			
				//new
			}
			int a = 2*i , b = a + 1 ;
			if( m_pgpbButton[ a ] && m_pgpbButton[ b ] ) { 
				setCoordSD(dynamic_cast<SimpleDialog*>(m_pgpbButton[a]),
					m_pgppFrameU[i]->x  - 2 , 
					m_pgppFrameU[i]->y1 - bH );
				setCoordSD(dynamic_cast<SimpleDialog*>(m_pgpbButton[b]),
					m_pgppFrameU[i]->x1 - bW + 2 ,
					m_pgpbButton[a]->y );
			}
		}
	}

	for(int i=0; i < m_nTXT ; i++)
		if( m_ptbHeroPar[i] ) {
			setCoordSD(dynamic_cast<SimpleDialog*>(m_ptbHeroPar[i]), 706 , 270 + 20*(i+1) );
			//setCoordSD(dynamic_cast<SimpleDialog*>(m_ptbHeroPar[i]), 390 , 320 + 25*(i+1) );
		}
	int xP = 632 , yP = 510 ;
	if( m_pgppFrameH[3] ) { xP = m_pgppFrameH[3]->x1 ; yP = m_pgppFrameH[3]->y ; }
	for(int i=0; i < m_nPgs ; i++) {
		if( m_pgpbPages[i] && m_pgpbPagBak[i] )
		{
			if(i!=0) {  if( m_pgpbPages[i] )	{ yP = m_pgpbPages[i-1]->y1+2; } 
						else					{ yP = 510 + i*31 ; }   }
			setCoordSD(dynamic_cast<SimpleDialog*>(m_pgpbPages[i]),  xP , yP ); 
			setCoordSD(dynamic_cast<SimpleDialog*>(m_pgpbPagBak[i]), xP , yP ); 
		}
	}
	UpdateUCoord();
}
//----------------------------------------------
void	CPERSONAL::UpdateInfo( CHero* pH )
{ 
	int  id = m_PlayerID * 10 + m_HeroID ;
	if( ptbHeroDesc && m_CurHeroId != id ) {
		m_CurHeroId = id ;
		_str		text;
		DString		a_DS , a_DS2;
		char		path[256];
		int  nationID = m_PlayerID ;
		for(int i=0; i < NNations && i < MAX_PLAYERS ; i++) {
			char*  NName = ROOT_SCENARIO.m_chNationID[ m_PlayerID ];
			if( !strcmp(NatCharLo[ i ], NName) || !strcmp(NatCharHi[ i ], NName) ) { //char NatCharLo[32][8]; char NatCharHi[32][8];
				nationID = i;  break;
			}
		}
		pSAVE->GetHeroDescription( text, nationID , m_HeroID );
		ptbHeroDesc->SetMessage( text.pchar() ) ;
		if( InfoDesk && InfoDesk->VScroller ) InfoDesk->VScroller->SPos = 0 ;
	}
	//if( HeroDesc )    { HeroDesc->Visible	 = true;	HeroDesc->Enabled	 = true; }	
	if( InfoDesk )    { InfoDesk->Visible	 = true;	InfoDesk->Enabled	 = true; }	
	if( ptbHeroDesc ) { ptbHeroDesc->Visible = true;	ptbHeroDesc->Enabled = true; }	

	if( m_ptbHeroPar[0] ) m_ptbHeroPar[0]->Visible = false ;
	if( m_ptbHeroPar[1] ) m_ptbHeroPar[1]->Visible = false ;
	if( m_ptbHeroPar[2] ) m_ptbHeroPar[2]->Visible = false ;
	if( m_ptbHeroPar[4] ) m_ptbHeroPar[4]->Visible = false ;
	if( m_ptbHeroPar[5] ) m_ptbHeroPar[5]->Visible = false ;

	char  name[256] ,  s[256] ;
	if( pH->GetStat() >= 0 ) {
		sprintf( s, "%s %d" , GetTextByID("#AwardsNum") ,	pH->GetStat() );
		m_ptbHeroPar[0]->SetMessage( s );
	}
	sprintf( s, "%s %d" , GetTextByID("#Battles") ,		pH->m_iBatlN );
	m_ptbHeroPar[1]->SetMessage( s );

	sprintf( s, "%s %d" , GetTextByID("#Victories") ,		pH->m_iVictN );
	m_ptbHeroPar[2]->SetMessage( s );

	if( m_ptbHeroPar[3] ) m_ptbHeroPar[3]->Visible = false ;

	sprintf( s, "%s %d" , GetTextByID("#SquadsNum") ,		pH->GetARMY_NSQ() );
	m_ptbHeroPar[4]->SetMessage( s );

	sprintf( s, "%s:%d" , GetTextByID("#MaxSquadsNum") ,	pH->GetARMY_NSQ() + pH->GetARMY_FreeNSQ() );
	m_ptbHeroPar[5]->SetMessage( s );
}
//----------------------------------------------
bool	CPERSONAL::CheckUnitsPages()
{
	bool  find = false;
	int i;
	for(i=0; i < m_nPgs; i++) 
		if( m_pgpbPagBak[i]->UserParam == 1 ) { find = true; break; }

	if( find ) 
	{
		m_pgpbPagBak[i]->UserParam = 0 ;

		// check number of Units Pics
		CHero* pH = m_pPLAYERS[m_PlayerID]->GetHeroByPID(m_HeroID);
		if( !pH )		{ return false; }

		int	rank = pH->GetRank() ,  cont = pH->GetPlayerID() ,  ndif = pH->pArmyLimit->GetMaxDiffSqd(rank,cont);
		int  picID = 0 ,  pic[3] = { 0, 0, 0 };
		for(int ut=0; ut < ndif && ut < m_nFU ; ut++) {
			int typeID	= pH->pArmyLimit->GetNSqdNID(rank,cont, ut) ;
			int	id		= m_utUNITS.UNIT_NID.find( typeID );
			if( id < 0 )						{ continue; }

			m_ppUnitsPic[picID]	= m_utUNITS.UNIT_PIC[ id ];
			
			if( !m_ppUnitsPic[ picID ] )		{ continue; }
			
			for(int br=0; br < 3 ; br++)
				if( GetObjectBranch( typeID ) == br ) { pic[br]++ ; break; }

			//for(int id=0; id < 3 ; id++)
			//	if( GetObjectBranch( typeID ) == id ) { pic[id]++ ; break; }
		}
		if( pic[i] == 0 )		{ return false; }

		for(int p=0; p < m_nPgs ; p++) { 
			m_pgpbPages[p]->ActiveFrame  = m_pgpbPages[p]->PassiveFrame  = 21 + p*3 ; // passive 
			m_pgpbPagBak[p]->ActiveFrame = m_pgpbPagBak[p]->PassiveFrame = 17 ;		  // passive

			if( pic[p] == 0 ) m_pgpbPages[p]->ActiveFrame = m_pgpbPages[p]->PassiveFrame = 20 + p*3 ; // disable
		}
		m_ActPage = i ;
		m_pgpbPages[i]->ActiveFrame  = m_pgpbPages[i]->PassiveFrame		 = 21 + i*3 ;	// active
		m_pgpbPagBak[i]->ActiveFrame = m_pgpbPagBak[i]->PassiveFrame	 = 18 ;			// active

		UpdateCoord();
		//UpdateUCoord();

		//new
		for(int br=0; br < 3 ; br++) 
		{
			m_pgpbPagBak[ br ]->UserParam = 0;
			if( pic[ br ] > 0 && pH->GetBranch_NSQ( br ) == 0 && pH->GetARMY_FreeNSQ() > 0 ) {
				m_iBlinkTime = 0;
				m_pgpbPagBak[ br ]->UserParam = 10; // blink this page!			COMPAGE_BLINKCOLOR
			}
		}
		//return true;
	}
	
	//new
	CheckPageBlink();

	return  find;
	//return false;
}
//----------------------------------------------

bool	CPERSONAL::CheckPageBlink() //new
{
	int  BLINK_TIME_VALUE = 400 ; //500 ;

	int  bt = GetTickCount();
	if( m_iBlinkTime == 0 || bt > m_iBlinkTime + BLINK_TIME_VALUE ) //100 )  
	{
		bool  blink = false;
		m_iBlinkTime = bt ;
		for(int br=0; br < 3 ; br++)
		{
			blink = ( m_PlayerID == gi_bmSAVE_Nation && m_ActPage != br ); 
			blink = blink && ( m_pgpbPagBak[br]->UserParam == 10 && m_pgpbPagBak[br]->Diffuse != COMPAGE_BLINKCOLOR );

			m_pgpbPagBak[ br ]->Diffuse = 0xFFFFFFFF;
			if( blink )  
				m_pgpbPagBak[ br ]->Diffuse = COMPAGE_BLINKCOLOR; // blink this page!			COMPAGE_BLINKCOLOR
		}
	}

	return  true;
}

//----------------------------------------------
//----------------------------------------------
//----------------------------------------------

//**********************************************************************************
////////////////////////////////////////
////////////// CPicesPict //////////////
////////////////////////////////////////
//----------------------------------------------
CPicesPict::CPicesPict(DialogsSystem* pMenu,int NX,int NY,int FrOffset){
	m_pMenu = pMenu;
	m_NX = NX;
	m_NY = NY;
	m_FrOffset = FrOffset;
	X0 = Y0 = 0;
	m_x0 = m_y0 = 0;
	m_H = 768;	m_W = 1024;
	m_PW = m_PH = 0;
	SimpleDialog*	a_psdSec = NULL;
	m_mxo = m_myo = 0;
	m_bBegMove = false;
	for (int j=0; j<NX; j++){
		a_psdSec = m_pMenu->Find(m_FrOffset+j);
		m_PW += GPS.GetGPWidth(a_psdSec->ShapeFileID,a_psdSec->ShapeSpriteID);
	}
	for (int i=0; i<NY; i++){
		a_psdSec = m_pMenu->Find(m_FrOffset+i*NX);
		m_PH += GPS.GetGPHeight(a_psdSec->ShapeFileID,a_psdSec->ShapeSpriteID);
	}
		
	CalculateCoord();

	pgppMapBorder = NULL ;
	CreateMapBorder();
}
//----------------------------------------------

void	CPicesPict::CreateMapBorder()
{
	LocalGP		a_lgpElems("Interf3\\TotalWarGraph\\bmElements");
	pgppMapBorder = m_pMenu->addGPPicture(NULL,0,0,a_lgpElems.GPID, 0);
}

//----------------------------------------------
void	CPicesPict::SetScreen(int x0,int y0,int H, int W){
	X0 = x0;
	Y0 = y0;
	m_x0 = x0;
	m_y0 = y0;
	m_H = H;
	m_W = W;

	MoveX(m_x0);
	MoveY(m_y0);
	//grey
	int dx = 2 ;
	pgppMapBorder->x = x0 - dx ;
	pgppMapBorder->y = y0 - dx /*-1*/ ;//very new!!!
	//pgppMapBorder->x1 += x0 ;
	//pgppMapBorder->y1 += y0 ;
}
//----------------------------------------------
void	CPicesPict::CalculateCoord(){
	SimpleDialog*	a_psdCur  = NULL;
	SimpleDialog*	a_psdPrv  = NULL;
	SimpleDialog*	a_psdPrvY = NULL;
	for (int i=0; i<m_NY; i++){
	for (int j=0; j<m_NX; j++){
		a_psdCur = m_pMenu->Find(m_FrOffset+i*m_NY+j);
		if (i==0&&j==0){
			a_psdCur->x = 0;
			a_psdCur->y = 0;
		}
		if (i>0&&j>0){
			a_psdPrv  = m_pMenu->Find(m_FrOffset+(i-1)*m_NY+j);
			a_psdPrvY = m_pMenu->Find(m_FrOffset+i*m_NY+(j-1));
			a_psdCur->x = a_psdPrvY->x + GPS.GetGPWidth(a_psdPrvY->ShapeFileID,a_psdPrvY->ShapeSpriteID);
			a_psdCur->y = a_psdPrv->y + GPS.GetGPHeight(a_psdPrv->ShapeFileID,a_psdPrv->ShapeSpriteID);
		}
		if (i>0&&j==0){
			a_psdPrv = m_pMenu->Find(m_FrOffset+(i-1)*m_NY+j);
			a_psdCur->y = a_psdPrv->y + GPS.GetGPHeight(a_psdPrv->ShapeFileID,a_psdPrv->ShapeSpriteID);
		}
		if (i==0&&j>0){
			a_psdPrv = m_pMenu->Find(m_FrOffset+i*m_NY+(j-1));
			a_psdCur->x = a_psdPrv->x + GPS.GetGPWidth(a_psdPrv->ShapeFileID,a_psdPrv->ShapeSpriteID);
		}
		a_psdCur->Visible = true;
	}
	}
}
//----------------------------------------------
bool	CPicesPict::AddX(int dx){
	SimpleDialog*	a_psdSec = NULL;
	int dx0 = dx;
	
	if ((dx0>0) && ((m_x0+dx0)>X0)){
		dx0 = X0-m_x0;
	}
	else{	//dx<0
		if ( (m_x0+dx0) < (m_W-m_PW+X0) ){
			dx0 = m_W-m_PW-m_x0+X0;
		}
	}
	m_x0 += dx0;
	for (int i=0; i<m_NX*m_NY; i++){
		a_psdSec = m_pMenu->Find(m_FrOffset+i);
		a_psdSec->x += dx0;
	}
	
	return (dx0!=0);
}
//----------------------------------------------
bool	CPicesPict::AddY(int dy){
	SimpleDialog*	a_psdSec = NULL;
	int dy0 = dy;
	
	if ((dy0>0) && ((m_y0+dy0)>Y0)){
		dy0 = Y0-m_y0;
	}
	else{	//dx<0
		if ( (m_y0+dy0) < (m_H-m_PH+Y0) ){
			dy0 = m_H-m_PH-m_y0+Y0;
		}
	}
	m_y0 += dy0;
	for (int i=0; i<m_NX*m_NY; i++){
		a_psdSec = m_pMenu->Find(m_FrOffset+i);
		a_psdSec->y += dy0;
	}

	return (dy0!=0);
}
//----------------------------------------------
void	CPicesPict::MoveX(int dx){
	SimpleDialog*	a_psdSec = NULL;
	for (int i=0; i<m_NX*m_NY; i++){
		a_psdSec = m_pMenu->Find(m_FrOffset+i);
		a_psdSec->x += dx;
	}
}
//----------------------------------------------
void	CPicesPict::MoveY(int dy){
	SimpleDialog*	a_psdSec = NULL;
	for (int i=0; i<m_NX*m_NY; i++){
		a_psdSec = m_pMenu->Find(m_FrOffset+i);
		a_psdSec->y += dy;
	}
}
//----------------------------------------------
void	CPicesPict::GetMapSize(int &dx,int &dy,int &w,int &h)
{
	dx = m_x0;
	dy = m_y0;
	w  = m_PW;
	h  = m_PH;
}
//----------------------------------------------
bool	CPicesPict::SetPosition(int dx,int dy)
{
	bool res = AddX(dx);
	res = AddY(dy) || res;
	return	res;
}
//----------------------------------------------
void	CPicesPict::ScrollPic(int mx,int my){
	if (mx<=g_MAP_RECT.left+3){	//Скролинг в лево.
		AddX(5);
	}
	if (mx>=g_MAP_RECT.right-3){	//Скролинг в прво.
		AddX(-5);
	}
	if (my<=g_MAP_RECT.top+3){	//Скролинг в верх.
		AddY(5);
	}
	if (my>=g_MAP_RECT.bottom-3){	//Скролинг в низ.
		AddY(-5);
	}
}
//----------------------------------------------
void	CPicesPict::MoveWithMouse(){
	SimpleDialog*	a_psdSec = m_pMenu->Find(m_FrOffset);
	//
	//if (a_psdSec->Enabled==false)	{ return; }
	if(a_psdSec->Visible==false)	{ return; }
	//

	if (!m_bBegMove && mouseX>=X0 && mouseX<X0+m_W &&
		 			   mouseY>=Y0 && mouseY<Y0+m_H && realRpressed){
		m_bBegMove = true;
		m_mxo = mouseX;
		m_myo = mouseY;
	}
	if (m_bBegMove && mouseX>=X0 && mouseX<X0+m_W &&
				      mouseY>=Y0 && mouseY<Y0+m_H && realRpressed){
		AddX(mouseX-m_mxo);
		AddY(mouseY-m_myo);
		m_mxo = mouseX;
		m_myo = mouseY;
	}
	else{
		m_bBegMove = false;
	}
}
//----------------------------------------------
void	CPicesPict::SetVisible(bool state){
	SimpleDialog*	a_psdSec = NULL;
	for (int i=0; i<m_NX*m_NY; i++){
		a_psdSec = m_pMenu->Find(m_FrOffset+i);
		if(a_psdSec)a_psdSec->Visible = state;
	}
	pgppMapBorder->Visible = state;
}
//----------------------------------------------
void	CPicesPict::SetEnable(bool state){
	SimpleDialog*	a_psdSec = NULL;
	for (int i=0; i<m_NX*m_NY; i++){
		a_psdSec = m_pMenu->Find(m_FrOffset+i);
		a_psdSec->Enabled = state;
	}
}
//----------------------------------------------------------------------------------

//**********************************************************************************
////////////////////////////////////////
/////////////// CTYPENUM ///////////////
////////////////////////////////////////
//----------------------------------------------
CTYPENUM::CTYPENUM								()
{
	Kills.Clear();

	TypeName_sID	= "NOTYPE";
	TypeIndex_ID	= 0;
	nSquarnd		= 0;
}
//----------------------------------------------
CTYPENUM::~CTYPENUM								()
{
	Kills.Clear();
}
//----------------------------------------------
void		CTYPENUM::setTypeName					( const char* TypeName ){
	if ( TypeName!=NULL ) {
		TypeName_sID = (char*)TypeName;
	}
}
//----------------------------------------------
void		CTYPENUM::setTypeName					( const _str& TypeName ){
	//if ( TypeName.pchar()!=NULL ) {
		TypeName_sID = (_str&)TypeName;
	//}
}
//----------------------------------------------
void		CTYPENUM::setTypeIndx( const int TypeIndx ){
	if ( TypeIndx>=0 ) {
		TypeIndex_ID = TypeIndx;
	}
}
//----------------------------------------------
void		CTYPENUM::setNSquard					( const int n ){
	if ( n>=0 ) {
		nSquarnd = n;
	}
}
//----------------------------------------------
void		CTYPENUM::SetAll(const char* TypeName, const int n ){
	setTypeName( TypeName );
	setTypeIndx( g_pALL_UNIT_TYPE->Get( (char*)TypeName ) );
	setNSquard ( n );
}
//----------------------------------------------
void		CTYPENUM::SetAll(const _str& TypeName, const int n ){
	setTypeName( TypeName );
	setTypeIndx( g_pALL_UNIT_TYPE->Get( (char*)TypeName.cpchar() ) );
	setNSquard ( n );
}
//----------------------------------------------
void		CTYPENUM::SetAll						( const char* TypeName, const int id, const int n ){
	SetAll( TypeName, n );
}
//----------------------------------------------
void		CTYPENUM::SetAll						( const _str& TypeName, const int id, const int n ){
	SetAll( TypeName, n );
}
//----------------------------------------------
_str*		CTYPENUM::getTypeName(){
	return	&TypeName_sID;
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
CTYPENUM&	CTYPENUM::operator=					( const char* TypeName ){
	setTypeName( TypeName );
	return	*this;
}
//----------------------------------------------
CTYPENUM&	CTYPENUM::operator=					( const _str& TypeName ){
	setTypeName( TypeName );
	return	*this;
}
//----------------------------------------------
CTYPENUM&	CTYPENUM::operator=					( const int n ){
	setNSquard ( n );
	return	*this;
}
//----------------------------------------------
CTYPENUM&	CTYPENUM::operator=					( const CTYPENUM& te ){
	SetAll( te.TypeName_sID, te.TypeIndex_ID, te.nSquarnd );
	return	*this;
}
//----------------------------------------------------------------------------------
//**********************************************************************************
////////////////////////////////////////
///////////// CMISSIONINIT /////////////
////////////////////////////////////////
//----------------------------------------------
CMISSIONINIT::CMISSIONINIT						(){
	Init();
	Clear();
}
//----------------------------------------------
CMISSIONINIT::~CMISSIONINIT						(){
	Init();
	Clear();
}
//----------------------------------------------
CTYPENUM*	CMISSIONINIT::createSquard			( const char*	TypeName_sID, const int n ){
	if ( TypeName_sID!=NULL && n>=0 ) {
		CTYPENUM*	pTE = new	CTYPENUM();
		pTE->SetAll( TypeName_sID, n );
		return	pTE;
	}
	return	NULL;
}
//----------------------------------------------
CTYPENUM*	CMISSIONINIT::createSquard			( const _str TypeName_sID, const int n ){
	if ( TypeName_sID.cpchar()[0] && n>=0 ) {
		CTYPENUM*	pTE = new	CTYPENUM();
		pTE->SetAll( TypeName_sID, n );
		return	pTE;
	}
	return	NULL;
}
//----------------------------------------------
bool		CMISSIONINIT::addSquard				( CTYPENUM* pNew, ClassArray<CTYPENUM>* pBase ){
	if ( pNew!=NULL && pBase!=NULL ) {
		pBase->Add(pNew);
		return true;
	}
	return false;
}
//----------------------------------------------
bool		CMISSIONINIT::addSquard				( char* TypeName_sID, int n, ClassArray<CTYPENUM>* pBase ){
	if ( pBase!=NULL ) {
		CTYPENUM* pNS = createSquard( TypeName_sID, n );
		if ( pNS!=NULL ) {
			pBase->Add( pNS );
			return	true;
		}
	}
	return	false;
}
//----------------------------------------------
bool		CMISSIONINIT::addSquard				( _str  TypeName_sID, int n, ClassArray<CTYPENUM>* pBase ){
	if ( pBase!=NULL ) {
		CTYPENUM* pNS =createSquard( TypeName_sID, n );
		if ( pNS!=NULL ) {
			pBase->Add( pNS );
			return	true;
		}
	}
	return	false;
}
//----------------------------------------------
bool		CMISSIONINIT::addAttackerSquard		( CTYPENUM* pNew ){
	return	addSquard( pNew, &AttackerForce );
}
//----------------------------------------------
bool		CMISSIONINIT::addAttackerSquard		( char* TypeName_sID, int n ){
	return	addSquard( TypeName_sID, n, &AttackerForce );
}
//----------------------------------------------
bool		CMISSIONINIT::addAttackerSquard		( _str  TypeName_sID, int n ){
	return  addSquard( TypeName_sID, n, &AttackerForce );
}
//----------------------------------------------
bool		CMISSIONINIT::addDefenderSquard		( CTYPENUM* pNew ){
	return	addSquard( pNew, &DefenderForce );
}
//----------------------------------------------
bool		CMISSIONINIT::addDefenderSquard		( char* TypeName_sID, int n ){
	return	addSquard( TypeName_sID, n, &DefenderForce );
}
//----------------------------------------------
bool		CMISSIONINIT::addDefenderSquard		( _str  TypeName_sID, int n ){
	return  addSquard( TypeName_sID, n, &DefenderForce );
}
//----------------------------------------------
void		CMISSIONINIT::SetARMYData			( CArmyLimit* pAL ){
	if ( pAL!=NULL ) {
		pArmyData = pAL;
	}
}
//----------------------------------------------
void		CMISSIONINIT::setAttackerNation		( int nat ){
	if (nat!=AttackerNation_ID)	g_bEDITOR_CHENGE=true;
	if ( nat!=AttackerNation_ID && -1<=nat && nat<8 ) {
		if (nat==-1) {
			AttackerNation_ID = 6;
		}else{
			AttackerNation_ID = nat;
		}
		//ClearAttacker();
		//int nDIF = pArmyData->GetMaxDiffSqd( AttackerRank_ID, AttackerNation_ID );
		//for ( int i=0; i<nDIF; i++ ){
		//	addAttackerSquard( pArmyData->GetNSqdID ( AttackerRank_ID, AttackerNation_ID, i ),
		//		pArmyData->GetNSqdNum( AttackerRank_ID, AttackerNation_ID, i )   );

		//}
	}
}
//----------------------------------------------
void		CMISSIONINIT::setDefenderNation		( int nat ){
	if (nat!=DefenderNation_ID)	g_bEDITOR_CHENGE=true;
	if ( nat!=DefenderNation_ID && -1<=nat && nat<8 ) {
		if (nat==-1) {
			DefenderNation_ID = 6;
		}else{
			DefenderNation_ID = nat;
		}
		//ClearDefender();
		//int nDIF = pArmyData->GetMaxDiffSqd( DefenderRank_ID, DefenderNation_ID );
		//for ( int i=0; i<nDIF; i++ ){
		//	addDefenderSquard( pArmyData->GetNSqdID ( DefenderRank_ID, DefenderNation_ID, i ),
		//		pArmyData->GetNSqdNum( DefenderRank_ID, DefenderNation_ID, i )   );

		//}
	}
}
//----------------------------------------------
void		CMISSIONINIT::SetSector_ID			( int sID ){
	if ( -1<=sID && sID<=100 ) {
		Sector_ID = sID;
	}
}
//----------------------------------------------
void		CMISSIONINIT::setAttackerDirection_ID( int dID ){
	if ( 0<=dID && dID<8 ) {
		AttackDirection_ID = dID;
	}
}
//----------------------------------------------
void		CMISSIONINIT::setAttackerDirection_ID( char* sID ){
	if ( sID!=NULL ) {
		Enumerator* SDIR_ENUM = ENUM.Get( "SDIR_ENUM" );
		AttackDirection_ID = SDIR_ENUM->Get( sID );
	}
}
//----------------------------------------------
void		CMISSIONINIT::setAttackerRank_ID	( int rID ){
	if ( AttackerRank_ID!=rID && 0<=rID && rID<=MAX_RANKS ) {
		AttackerRank_ID = rID;
		ClearAttacker();
		int nDIF = pArmyData->GetMaxDiffSqd( AttackerRank_ID, AttackerNation_ID );
		for ( int i=0; i<nDIF; i++ ){
			addAttackerSquard( pArmyData->GetNSqdID ( AttackerRank_ID, AttackerNation_ID, i ),
							   pArmyData->GetNSqdNum( AttackerRank_ID, AttackerNation_ID, i )   );

		}
	}
}
//----------------------------------------------
void		CMISSIONINIT::setDefenderRank_ID	( int rID ){
	if ( DefenderRank_ID!=rID && 0<=rID && rID<=MAX_RANKS ) {
		DefenderRank_ID = rID;
		ClearDefender();
		int nDIF = pArmyData->GetMaxDiffSqd( DefenderRank_ID, DefenderNation_ID );
		for ( int i=0; i<nDIF; i++ ){
			addDefenderSquard( pArmyData->GetNSqdID ( DefenderRank_ID, DefenderNation_ID, i ),
							   pArmyData->GetNSqdNum( DefenderRank_ID, DefenderNation_ID, i )   );

		}
	}
}
//----------------------------------------------
void		CMISSIONINIT::setAttackerArmy( CHero* pH ){
	if (pH) {
		AttackerRank_ID = pH->GetRank();
		ClearAttacker();
		for (int i=0; i<pH->ARMY.GetAmountOfElements(); i++ ){
			addAttackerSquard(g_pALL_UNIT_TYPE->Get(pH->ARMY[i]->TypeGlobID),pH->ARMY[i]->SQ_NUM);
		}
	}
}
//----------------------------------------------
void		CMISSIONINIT::setDefenderArmy( CHero* pH ){
	if (pH) {
		DefenderRank_ID = pH->GetRank();
		ClearDefender();
		for (int i=0; i<pH->ARMY.GetAmountOfElements(); i++ ){
			addDefenderSquard(g_pALL_UNIT_TYPE->Get(pH->ARMY[i]->TypeGlobID),pH->ARMY[i]->SQ_NUM);
		}
	}
}
//----------------------------------------------
void		CMISSIONINIT::createDefenderArmyBySectorParam(int populatin,int defence,int rase)
{
	if (DefenderForce.GetAmountOfElements()==0) 
	{
		int Nat = ( (DefenderNation_ID==6) ? (rase) : (DefenderNation_ID) );
		//Нет армии обороняющихся.
		int defence00 = ((defence<=populatin+1) ? (defence) : (populatin+1));
		
		for ( int i=0; i<=defence00; i++ ) {
			addDefenderSquard( pArmyData->GetNSqdID ( 5, Nat, i ), populatin+1 );
		}
	}
}
//----------------------------------------------

void		CMISSIONINIT::addArmyBySectorDefence( int SecDef , int SecPop )
{
	int  SQ_TYPE[5] =  { 0, 1, 2, 3, 4 }; //int  SQ_TYPE[5] =  { 0, 1, 2, 8, 9 };
	int  SQ_NUMS[5] =  { 0, 4, 3, 4, 1 };
	
	SQ_NUMS[0] = 0; 
	if(SecDef == 0)  { SQ_NUMS[1] = 1; }														/*3*/
	if(SecDef == 1)  { SQ_NUMS[1] = 2;  SQ_NUMS[2] = 1;	}										/*3*/
	if(SecDef == 2)  { SQ_NUMS[1] = 3;  SQ_NUMS[2] = 2;  SQ_NUMS[3] = 2;  SQ_NUMS[4] = 1; }		/*4*/ //if(SecDef >= 3)  { SQ_NUMS[0] = 0; SQ_NUMS[1] = 4;  SQ_NUMS[2] = 3;  SQ_NUMS[3] = 4;  SQ_NUMS[4] = 1; }/*6*/


	int DEF_NI = DefenderNation_ID;
	if( DEF_NI == 5 ) { SQ_TYPE[3] = 7; SQ_TYPE[4] = 8; }

	int SEC_DEF_SQ	= GetSQNumByDefenceID( SecDef , DEF_NI , SecPop , Sector_ID );
	
	int NonDefaultSQN = SEC_DEF_SQ - GetSQNumByDefenceID( SecDef , DEF_NI , -1 , -1 );
	if( NonDefaultSQN > 0 ) { // have addded squads: CAPITAL SECTOR (main res - Gold) 
		if( SQ_NUMS[1] > 0 )  SQ_NUMS[1] += NonDefaultSQN/3 ;
		if( SQ_NUMS[2] > 0 )  SQ_NUMS[2] += NonDefaultSQN/3 ;
		if( DEF_NI == 0 )	  SQ_NUMS[1] += NonDefaultSQN/3 ; //chauseurs	// GOLDen bonus
		if( DEF_NI == 4 )	  SQ_NUMS[4] += NonDefaultSQN/3 ; //grenadiers
		if( DEF_NI == 5 )	  SQ_NUMS[4] += NonDefaultSQN/3 ; //mamlukes
	}
	// correct SQ_NUMS[0] - add SQ_TYPE[0]
	int sum = 0;
	for(int nn=0; nn < SecDef + 2 ; nn++)  sum += SQ_NUMS[ nn ];
	SQ_NUMS[0] = SEC_DEF_SQ - sum;

	if( DEF_NI != -1 ) {
		for(int nn=0; nn < SecDef + 2 ; nn++)   
			addDefenderSquard( pArmyData->GetNSqdID ( 5, DEF_NI, SQ_TYPE[ nn ] ), SQ_NUMS[ nn ] );
	}
	else { 
		addDefenderSquard( "TempMent(NE)" , SEC_DEF_SQ ); // rebellion squad type
	}
}
//void		CMISSIONINIT::addArmyBySectorDefence( int SecDef , int SecPop ) //old working  succeesfull ver!
//{
//	int SEC_DEF_SQ	= GetSQNumByDefenceID( SecDef , DefenderNation_ID , SecPop , Sector_ID );
//
//	if( DefenderNation_ID != -1 ) {
//		int sqNum = SEC_DEF_SQ/( SecDef + 1 );
//		addDefenderSquard( pArmyData->GetNSqdID ( 5, DefenderNation_ID, 0 ), SEC_DEF_SQ - sqNum * SecDef  );
//		for(int nn=1; nn <= SecDef; nn++)   
//			addDefenderSquard( pArmyData->GetNSqdID ( 5, DefenderNation_ID, nn ), sqNum );
//	}
//	else { 
//		addDefenderSquard( "TempMent(NE)" , SEC_DEF_SQ ); // rebellion		// TempMent(NE)	// TempMent(FR)
//	}
//}

//void		CMISSIONINIT::addArmyBySectorDefence( int SecDef )
//{
//	//int SEC_DEF_SQ = 4 ; // 0		4 - 6 - 8 - 10     ////gec - 0 - 4 - 9 - 18		
//	//if( SecDef==1 ) SEC_DEF_SQ +=  2 ; // 4
//	//if( SecDef==2 ) SEC_DEF_SQ +=  6 ; // 9
//	//if( SecDef==3 ) SEC_DEF_SQ += 10 ; //18
//	int sqNum = 2;
//	sqNum+=SecDef/2;	// 0 - 2 *1=2 // 1 - 2 *2=4 // 2 - 3 *3=9 // 3 - 3 *4=12
//	if(SecDef==0)	{ sqNum = 4 ; } // 4*1=4
//	if(SecDef==1)	{ sqNum = 3 ; } // 3*2=6
//	for ( int i=0; i < SecDef+1 ; i++ ) {
//			addDefenderSquard( pArmyData->GetNSqdID ( 5, DefenderNation_ID, i ), /* i/2 */
//							   sqNum /*4*/ );
//		}
//}

//void		CMISSIONINIT::addArmyBySectorDefence( int SecDef ) //!!!OLD!!! Vitya ver.
//{
//	if (DefenderForce.GetAmountOfElements()==0) 
//	{
//		int Nat = ( (DefenderNation_ID==6) ? (rase) : (DefenderNation_ID) );
//		//Нет армии обороняющихся.
//		int defence00 = ((defence<=populatin+1) ? (defence) : (populatin+1));
//		
//		for ( int i=0; i<=defence00; i++ ) {
//			addDefenderSquard( pArmyData->GetNSqdID ( 5, Nat, i ), populatin+1 );
//		}
//	}
//}
//----------------------------------------------
int			CMISSIONINIT::getSector_ID			(){
	return Sector_ID;
}
//----------------------------------------------
int			CMISSIONINIT::getAttackerNation		(){
	return	AttackerNation_ID;
}
//----------------------------------------------
int			CMISSIONINIT::getDefenderNation		(){
	return	DefenderNation_ID;
}
//----------------------------------------------
int			CMISSIONINIT::getAttackerRank_ID	(){
	return AttackerRank_ID;
}
//----------------------------------------------
int			CMISSIONINIT::getDefenderRank_ID	(){
	return DefenderRank_ID;
}
//----------------------------------------------
void		CMISSIONINIT::Init					(){
	Sector_ID			= 0;
	AttackDirection_ID	= 0;
	AttackerNation_ID	= 0;
	AttackerRank_ID		= -1;
	DefenderNation_ID	= 0;
	DefenderRank_ID		= -1;
	HeroInSect			= false;
}
//----------------------------------------------
void		CMISSIONINIT::Clear					( ClassArray<CTYPENUM>* pBase )
{
	if( pBase != NULL ) {
		pBase->Clear();
	}
}
//----------------------------------------------
void		CMISSIONINIT::Clear					(){
	Clear( &AttackerForce );
	Clear( &DefenderForce );
}
//----------------------------------------------
void		CMISSIONINIT::ClearAttacker			(){
	Clear( &AttackerForce );
}
//----------------------------------------------
void		CMISSIONINIT::ClearDefender			(){
	Clear( &DefenderForce );
}

//**********************************************************************************
////////////////////////////////////////
/////////// CSAVE_MIS_STRUCT ///////////
////////////////////////////////////////
//----------------------------------------------
CSAVE_MIS_STRUCT::CSAVE_MIS_STRUCT(){
	Init();
}
//----------------------------------------------
CSAVE_MIS_STRUCT::~CSAVE_MIS_STRUCT(){
	DeleteArrays();
	Init();
}	
//----------------------------------------------
void	CSAVE_MIS_STRUCT::Init(){
	Sector_ID			= 0;
	AttackDirection_ID	= "NO_DATA";
	AttackerNation_ID	= 0;
	AttackerRank_ID		= "NO_DATA";
	if (AttackerForce.GetAmount()!=0) {
		AttackerForce.Clear();
	}
	DefenderNation_ID	= 0;
	DefenderRank_ID		= "NO_DATA";
	if (DefenderForce.GetAmount()!=0) {
		DefenderForce.Clear();
	}
	PlayerStat=0;

	for(int res=0; res<=RECRT ; res++) {
		AttackerResource.SetResource(res, 0);
		DefenderResource.SetResource(res, 0);
	}
}
//----------------------------------------------
void	CSAVE_MIS_STRUCT::DeleteArrays(){
	if (AttackerForce.GetAmount()!=0) {
		//for (int i=0; i<AttackerForce.GetAmount(); i++){
		//	if (AttackerForce[i]!=NULL) {
		//		delete (AttackerForce[i]);
		//		AttackerForce[i] = NULL;
		//	}
		//}
		AttackerForce.Clear();
	}
	if (DefenderForce.GetAmount()!=0) {
		//for (int i=0; i<DefenderForce.GetAmount(); i++){
		//	if (DefenderForce[i]!=NULL) {
		//		delete (DefenderForce[i]);
		//		DefenderForce[i] = NULL;
		//	}
		//}
		DefenderForce.Clear();
	}
}
//----------------------------------------------
void	CSAVE_MIS_STRUCT::operator +=( const CMISSIONINIT& MIS_INT ){
	Enumerator*	pRANK_ENUM = ENUM.Get( "RANK_ENUM" );
	Enumerator*	pSDIR_ENUM = ENUM.Get( "SDIR_ENUM" );
	
	Init();

	Sector_ID			=	MIS_INT.Sector_ID;
	AttackDirection_ID	=	pSDIR_ENUM->Get(MIS_INT.AttackDirection_ID);
	AttackerNation_ID	=	MIS_INT.AttackerNation_ID;
	AttackerRank_ID		=	pRANK_ENUM->Get(MIS_INT.AttackerRank_ID);
	DefenderNation_ID	=	MIS_INT.DefenderNation_ID;
	DefenderRank_ID		=	pRANK_ENUM->Get(MIS_INT.DefenderRank_ID);
	PlayerStat			=	MIS_INT.PlayerStat;
	HeroPresent			=	MIS_INT.HeroInSect;

	DeleteArrays();
	CTYPENUM*	pCT = NULL;
	CTYPENUM*	pTE = NULL;
	_str		str;
    int n = MIS_INT.AttackerForce.GetAmount();
    if (n>0) {
		for ( int i=0; i<n; i++ ){
			pCT = MIS_INT.AttackerForce[i];
			str = (pCT->getTypeName())->pchar();
			pTE = createSquard( str.pchar(), pCT->getNSquard() );
			AttackerForce.Add(pTE);
			pTE = NULL;
		}
	}
	n = MIS_INT.DefenderForce.GetAmount();
	if (n>0) {
		for ( int i=0; i<n; i++ ){
			pCT = MIS_INT.DefenderForce[i];
			str = (pCT->getTypeName())->pchar();
			pTE = createSquard( str.pchar(), pCT->getNSquard() );
			DefenderForce.Add(pTE);
			pTE = NULL;
		}
	}
}
//----------------------------------------------
CTYPENUM*	CSAVE_MIS_STRUCT::createSquard		( const char*	TypeName, const int n ){
	CTYPENUM*	pTE = new	CTYPENUM();
	pTE->SetAll( TypeName, n );
	return	pTE;
}
//----------------------------------------------
CTYPENUM*	CSAVE_MIS_STRUCT::createSquard		( const _str	TypeName, const int n ){
	CTYPENUM*	pTE = new	CTYPENUM();
	pTE->SetAll( TypeName, n );
	return	pTE;
}
//----------------------------------------------------------------------------------
void	CSAVE_MIS_STRUCT::SetNKills( CHero* pAttack , CHero* pDefend )
{
	// Set Attacker Kills
	for(int sq=0; sq < pAttack->ARMY.GetAmountOfElements() ; sq++)
	{
		bool quit = false ;
		int  type = pAttack->ARMY[ sq ]->getTypeGlobID() ;

		for(int i=0; i < AttackerForce.GetAmountOfElements() && !quit ; i++)
			if( AttackerForce[i]->TypeIndex_ID == type ) 
			{
				quit = true ; 
				AttackerForce[i]->Kills.Clear();
				// set squads NKills
				for(int k=0; k < pAttack->ARMY[ sq ]->Kills.GetAmountOfElements() ; k++)
					AttackerForce[i]->Kills.Add( pAttack->ARMY[ sq ]->Kills[k] );
			}
	}

	// Set Defender Kills
	if( !pDefend ) { return; }
	for(int sq=0; sq < pDefend->ARMY.GetAmountOfElements() ; sq++)
	{
		bool quit = false ;
		int  type = pDefend->ARMY[ sq ]->getTypeGlobID() ;

		for(int i=0; i < DefenderForce.GetAmountOfElements() && !quit ; i++)
			if( DefenderForce[i]->TypeIndex_ID == type ) 
			{
				quit = true ; 
				DefenderForce[i]->Kills.Clear();
				// set squads NKills
				for(int k=0; k < pDefend->ARMY[ sq ]->Kills.GetAmountOfElements() ; k++)
					DefenderForce[i]->Kills.Add( pDefend->ARMY[ sq ]->Kills[k] );
			}
	}
}
//----------------------------------------------------------------------------------
void	CSAVE_MIS_STRUCT::SetResources(CPlayer* pAttPL, CPlayer* pDefPL)
{
	for(int res=0; res<=RECRT; res++) {
		AttackerResource.SetResource(res, pAttPL->GetResource(res)); 
		DefenderResource.SetResource(res, pDefPL->GetResource(res)); 
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CResource_BM   //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
CResource_BM::CResource_BM()
{ 
	Init(); 
	return; 
};
//----------------------------------------------------------------------------------
CResource_BM::~CResource_BM()
{ 
	Init(); 
	return; 
};
//----------------------------------------------------------------------------------
void	CResource_BM::Init()
{
	wood=food=stone=gold=iron=coal=recruit=0; 
}
//----------------------------------------------------------------------------------
void	CResource_BM::create(CSectData* pSD,int NI)
{
}
//----------------------------------------------------------------------------------
void	CResource_BM::updateADD(CSectData* pSD,int NI)
{
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
}
//----------------------------------------------------------------------------------
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



////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CHeroPersDesk   //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
CHeroPersDesk::CHeroPersDesk(DialogsSystem* pMenu , 
							 CHERO_PORTRAIT* pPortrait , 
							 CHERO_MINI_MENU* pHeroMiniMenu )
{ 
	Init(); 
	//X0 = 10 ;
	X0 = 20 ;   
	Y0 = 10 ;

	m_pMenu		 = pMenu;
	m_pHPortrait = pPortrait;
	m_pHMiniMenu = pHeroMiniMenu;

	m_iCurPlayer = -1 ;

	CreateElements();
}
//----------------------------------------------------------------------------------
CHeroPersDesk::~CHeroPersDesk()
{ 
}
//----------------------------------------------------------------------------------
void	CHeroPersDesk::Init()
{
	m_pCounName = NULL; 
	m_pFlag		= NULL;
	for(int i=0; i < MAX_PLAYERS ; i++) { //MAX_HEROES
 		pgppMenuBack[i]  = NULL;		// BackGround for heroes on m_pPersDesk
 		pgppPortrait[i]  = NULL;		// Portrait
		ptbName[i]		 = NULL;		// Имя игрока.
		ptbRank[i]		 = NULL;		// Звание игрока.
		ptbExpa[i]		 = NULL;		// expa
		ptbSQNum[i]		 = NULL;		// SQ-num
		ptbVictN[i]		 = NULL;		// Vict-Battle Num

		pgppExpBar[i]	 = NULL;		// expa-BAR
		pgppExpBarPIC[i] = NULL;		// expa-BAR

		pgppExpBack[i]	 = NULL;		// BackGround for expa
		//new
		ptbStat[i]		 = NULL;	
		ptbFlag[i]		 = NULL;	
	}
	m_Visible	= false;
	m_Enabled	= false;
	m_Visited	= false;
	
	//new
	m_dxSF = -1;

	m_iCurPortrait = 0;
	m_iPortraitNum = 0;
}
//----------------------------------------------------------------------------------
void	CHeroPersDesk::CreateElements() 
{
	char sTMP[256]; strcpy(sTMP , "..." ) ;	
	int	ddx = 674 ,  ddy = 145 ; 
	int dLx = 282 ,  dLy = 527 ; 
	
	// назв.страны // from dipl
	//m_pCounName	= m_pMenu->addTextButton(NULL, 698 , 114 ,"",&fonMenuTitle2[1],&fonMenuTitle2[1],&fonMenuTitle2[1],0);//&BigRedFont,&BigRedFont,&BigRedFont,0);//"_empty_"
	if(m_pCounName)  m_pCounName->Visible = false; 
	if(m_pCounName)  m_pCounName->SetMessage( sTMP ) ;

	// флaг страны // from dipl
	LocalGP		a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");
	//m_pFlag		= m_pMenu->addGPPicture(NULL, 904  , 114 ,a_lgpFlags.GPID, 0 );
	if(m_pFlag)  m_pFlag->Visible = false; 

	// панель с героями
	ddy = 110;	//ddy = 112;
	dLy = 568;	//dLy = 564;	+= 4
	m_pDesk = m_pMenu->AddDialogsDesk( ddx, ddy, dLx, dLy, "EmptyBorder");
 	//LocalGP*	pLGP = m_pHPortrait->getLGPSmallP( 0 );	
	
	LocalGP		a_lgpElems("Interf3\\TotalWarGraph\\bmElements");
	LocalGP		a_lgpAward( "Interf3\\TotalWarGraph\\lva_Awards" );
	for (int i=0; i < MAX_PLAYERS ; i++)	//MAX_HEROES
	{
 		LocalGP*	pLGP = m_pHPortrait->getLGPSmallP( i );

		// картинки героев
		pgppPortrait[i] = m_pDesk->addGPPicture(NULL,0,0, pLGP->GPID , 0 );
		pgppPortrait[i]->Visible = false;

		// BackGround героев
		pgppMenuBack[i] = m_pDesk->addGPPicture(NULL,0,0, a_lgpElems.GPID  , 2 ) ;
		pgppMenuBack[i]->UserParam	 = 0 ;
		pgppMenuBack[i]->OnUserClick = &ON_HERO_R_CLICK ;
		pgppMenuBack[i]->Visible = false;
		pgppMenuBack[i]->AssignSound( "INTERF_NAGAL", CLICK_SOUND );//new


		// Имя игрока.
		ptbName[i] = m_pDesk->addTextButton(NULL,0,0, "_" ,&RedFont,&RedFont,&RedFont, 0);
		ptbName[i]->Visible = false;
		BigMapSetMessage( ptbName[i] , sTMP );

		// Звание игрока.
		ptbRank[i] = m_pDesk->addTextButton(NULL,0,0, "_" ,&SpecialBlackFont,&SpecialBlackFont,&SpecialBlackFont, 0); //1); //SpecialBlackFont
		ptbRank[i] ->Visible = false;
		BigMapSetMessage( ptbRank[i] , sTMP );

		// expa игрока. 
		ptbExpa[i] = m_pDesk->addTextButton(NULL,0,0, "_" ,&SmallGrayFont1,&SmallGrayFont1,&SmallGrayFont1, 1); 
		ptbExpa[i]->Visible = false;
		BigMapSetMessage( ptbExpa[i] , sTMP );

		// Vict-Battle Num 
		ptbVictN[i] = m_pDesk->addTextButton(NULL,0,0, "_" ,&SmallGrayFont1,&SmallGrayFont1,&SmallGrayFont1, 1); 
		ptbVictN[i]->Visible = false;
		BigMapSetMessage( ptbVictN[i] , sTMP );

		// SQ-num игрока. 
		ptbSQNum[i] = m_pDesk->addTextButton(NULL,0,0, "_" ,&GrayFont,&GrayFont,&GrayFont, 1); 
		ptbSQNum[i]->Visible = false;
		BigMapSetMessage( ptbSQNum[i] , sTMP );

		// BackGround for expa
		LocalGP		a_lgpExpBar("Interf3\\TotalWarGraph\\bmExpBar");
		pgppExpBack[i] = m_pDesk->addGPPicture(NULL,0,0, a_lgpExpBar.GPID , 0 ) ;
		pgppExpBack[i]->Visible = false;

		// expa-BAR
		pgppExpBar[i]  = m_pDesk->AddDialogsDesk( 0, 0, pgppExpBack[i]->x1, pgppExpBack[i]->y1 ,"NullBorder");
		pgppExpBar[i]->Visible	 = false;
		pgppExpBarPIC[i] = pgppExpBar[i]->addGPPicture(NULL,0,0, a_lgpExpBar.GPID , 1 ) ;

		ptbStat[i] = m_pDesk->addGPPicture(NULL,0,0, a_lgpAward.GPID , 0 );
		ptbFlag[i] = m_pDesk->addGPPicture(NULL,0,0, a_lgpFlags.GPID , 0 );
		ptbFlag[i]->Diffuse = 0x9FFFFFFF;
	}

	SetCoords( 0 );
	SetVisible( false , MAX_PLAYERS); 
}
//----------------------------------------------------------------------------------

void	CHeroPersDesk::SetCoords( int num )
{
	int TX = X0 + 131 , TY = Y0 + 6 ; //Y0 + 3 ;
	int delta = 10 /*8*/ , dx = 4 , dy = 4 ; 

	int  H = 0 ,  dH = 0 ; //dH - new
	if( pgppMenuBack[0] )  H = GPS.GetGPHeight( pgppMenuBack[0]->FileID, pgppMenuBack[0]->SpriteID );
	for (int i=0; i < MAX_PLAYERS ; i++)
		if( ROOT_SCENARIO.m_pPlayer[i] && ROOT_SCENARIO.m_pPlayer[i]->m_Alive && ROOT_SCENARIO.m_pPlayer[i]->m_phrHero[ 0 ].m_Alive )
		{
			setCoordSD( pgppMenuBack[i],  X0 , Y0 + dH );
			setCoordSD( pgppPortrait[i],  pgppMenuBack[i]->x +4 , pgppMenuBack[i]->y +4 ); /*+5 == граница к границе спрайтов*/
			setCoordSD( ptbName[i],       TX , TY + dH );
			setCoordSD( ptbRank[i],       TX , TY + dH +16 );
			setCoordSD( ptbStat[i],		  TX , TY + dH +BIGMAP_STATS_DIST  );				

			setCoordSD( ptbVictN[i],	  TX , TY + dH +54 );
			setCoordSD( ptbExpa[i],       TX , TY + dH +68 );
			setCoordSD( pgppExpBar[i],    TX , TY + dH +82 );
			setCoordSD( pgppExpBack[i],   TX , pgppExpBar[i]->y );

			setCoordSD( ptbSQNum[i],	  TX , TY + dH +110 ); 
			dH += H + delta ;

			//int  dYF = ( ptbExpa[i]->y - ptbRank[i]->y1  - 2*dy - ( ptbStat[i]->y1 - ptbStat[i]->y - 6 ) )/2 +1 ;
			//setCoordSD( ptbStat[i],		  TX ,		ptbRank[i]->y1		+dYF  );				
			setCoordSD( ptbFlag[i],		  X0 +10 , pgppMenuBack[i]->y1 -32 ); /*TX +10 , Y0 + dH +99*/
		}
		else {
			setCoordSD( pgppMenuBack[i], X0 , Y0  );
			setCoordSD( pgppPortrait[i], X0 , Y0  );
			setCoordSD( ptbName[i],      X0 , Y0  );
			setCoordSD( ptbRank[i],      X0 , Y0  );
			setCoordSD( ptbExpa[i],      X0 , Y0  );
			setCoordSD( pgppExpBar[i],   X0 , Y0  );
			setCoordSD( pgppExpBack[i],  X0 , Y0  );
			setCoordSD( ptbVictN[i],	 X0 , Y0  );
			setCoordSD( ptbSQNum[i],	 X0 , Y0  );
			setCoordSD( ptbStat[i],		 X0 , Y0  );
			setCoordSD( ptbFlag[i],		 X0 , Y0  );
		}
}

//----------------------------------------------------------------------------------
void	CHeroPersDesk::Refresh( CHero* pHero , int num )
{
	if( !pHero || num < 0 )  return;
	//if( m_Visited )			{ return; }
	//m_Visited = true;

	_str  tBlack, tBrown, tRed;  tBlack = "{C FF101010}";  tBrown = "{C FF502515}";  tRed = "{C FF640100}";

	LocalGP*	pLGP = NULL;

	m_iPortraitNum = 0;

	for (int i=0; i < MAX_PLAYERS ; i++)
	if( ROOT_SCENARIO.m_pPlayer[i] && ROOT_SCENARIO.m_pPlayer[i]->m_Alive && ROOT_SCENARIO.m_pPlayer[i]->m_phrHero[ 0 ].m_Alive )
	{
		m_iPortraitNum++;

		pHero = &(ROOT_SCENARIO.m_pPlayer[i]->m_phrHero[ 0 ]) ;

		pLGP = m_pHPortrait->getLGPSmallP( i );
		pgppMenuBack[i]->UserParam	 = reinterpret_cast<int>(&pHero[ 0 ]); //pgppMenuBack[i]->OnUserClick = &ON_HERO_R_CLICK ;
		pgppPortrait[i]->FileID		 = pLGP->GPID ;	
		pgppPortrait[i]->SpriteID	 = pHero[ 0 ].m_inHeroID ;

		char	s[256];
		int		iRank = pHero[ 0 ].GetRank() , iExpa = pHero[ 0 ].m_inExperience , iExpPercent = 0 ,  iNLExpa = 0 ;

		ptbName[i]->SetMessage( pHero[ 0 ].GetName() );	
		ptbRank[i]->SetMessage( GetRankByID(iRank) ); 

		sprintf(s, BIGMAP_EXPSFORMAT , GetTextByID("#Experience"), iExpa ) ;
		ptbExpa[i]->SetMessage( s );

		//ptbVictN
		sprintf(s, BIGMAP_VICTFORMAT , GetTextByID("#Victories"), pHero[ 0 ].m_iVictN, pHero[ 0 ].m_iBatlN );
		ptbVictN[i]->SetMessage( s );

		_str	a_Hint;				a_Hint = "{FS}";		a_Hint += tBrown;	
		a_Hint += GetTextByID("#CHINT_Victories");			DYNHINT_RA( ptbVictN[i] , a_Hint.pchar() );


		int  SQ_NN	= pHero[ 0 ].GetARMY_NSQ() ;
		int  SQ_MAX	= pHero[ 0 ].GetARMY_NSQ() + pHero[ 0 ].GetARMY_FreeNSQ() ;
		sprintf(s, BIGMAP_SQNNFORMAT , GetTextByID("#SquadsN"), SQ_NN , SQ_MAX );
		ptbSQNum[i]->SetMessage( s );
		a_Hint.DString::Clear();	a_Hint = "{FS}";		a_Hint += tBrown;	
		a_Hint += GetTextByID("#SquadsNum");	a_Hint += " / ";		a_Hint += GetTextByID("#MaxSquadsNum");	  
		DYNHINT_RA( ptbSQNum[i] , a_Hint.pchar() );

		iExpPercent = GetExpPercentByRankID( iExpa, iRank , iNLExpa );
		pgppExpBar[i]->x1 = pgppExpBar[i]->x + iExpPercent ; 

		a_Hint.DString::Clear();	
		a_Hint = "{FS}{C FF502515}" ;
		if( iNLExpa != 0xFFFF )  { a_Hint += GetTextByID("#CHINT_Next_Exp_Level"); a_Hint += " "; a_Hint += tRed; a_Hint += iNLExpa; } 
		else					 { a_Hint += tRed; a_Hint += GetTextByID( "#CHINT_MaxExpLevel" ) ; }
		DYNHINT_RA( ptbExpa[i] ,	 a_Hint.pchar() );
		DYNHINT_RA( pgppExpBack[i] , a_Hint.pchar() );

		int  PID = pHero[ 0 ].m_inPlayerID;
		int  SNN = pHero[ 0 ].m_inStatID;
		ptbFlag[i]->SpriteID = PID + 1;
		ptbStat[i]->SpriteID = SNN;
		// flag hint
		a_Hint.DString::Clear();	
		a_Hint = "{FS}";	
		if( PID >= 0 && PID < MAX_PLAYERS && ROOT_SCENARIO.m_pPlayer[ PID ] ) {
			a_Hint += tRed;		a_Hint += ROOT_SCENARIO.m_pPlayer[ PID ]->GetPlayerName();
		}
		DYNHINT_RA( ptbFlag[i] , a_Hint.pchar());
		// awards hint
		a_Hint.DString::Clear();	
		a_Hint = "{FS}";	
		a_Hint += tBrown;	a_Hint += GetTextByID("#CWT_Quantity");	a_Hint += " ";	
		a_Hint += tRed;		a_Hint += SNN;				
		DYNHINT_RA( ptbStat[i] , a_Hint.pchar());
	}
	SetCoords( num ) ;
}
//----------------------------------------------------------------------------------
bool	CHeroPersDesk::Show( CPlayer* pPlayer[MAX_PLAYERS] , 
							 CHero* pHero , int num , int heroId , bool PersVisible )
{
	if( !PersVisible && m_Visible )		//выключить
	{
		SetVisible( false, MAX_PLAYERS );
		SetEnabled( false, MAX_PLAYERS );
		m_Visited = false;
		return true;
	}

	if( PersVisible && !m_Visible )		//включить и сменить инфу (если надо)
	{
		int PID = pHero[ 0 ].m_inPlayerID;
		if( m_iCurPlayer != PID && PID >= 0 && PID < MAX_PLAYERS && pPlayer[ PID ] ) { 
			//if( m_pDesk->VScroller ) m_pDesk->VScroller->SPos = 0;
			m_iCurPlayer = PID ;

			Refresh( pHero , num ) ;

			if(m_pFlag)		 m_pFlag->SpriteID = PID + 1 ;
			if(m_pCounName)  m_pCounName->SetMessage( pPlayer[ PID ]->GetPlayerName() ) ;
		}
		ChangeActive( pgppMenuBack[ m_iCurPlayer ]->UserParam );
		SetVisible( true, num );
		SetEnabled( true, num );

		if( m_pDesk->VScroller ) 
		{ 
			int  n = m_iCurPlayer;
			m_pDesk->VScroller->SPos = 0;//very new!
			if( m_iPortraitNum > 4 ) 
			{
				m_pDesk->VScroller->SPos = pgppMenuBack[ n ]->y  -4 ;
				if( m_iPortraitNum - m_iCurPortrait < 4 )
					m_pDesk->VScroller->SPos = 288 /*300*/ /*290*/; 
				//m_pDesk->VScroller->SMaxPos ; //((pgppMenuBack[ n ]->y1 - pgppMenuBack[ n ]->y)/4 -5 );//290; //
			}

			//if( pgppMenuBack[ n ]->y1 < m_pDesk->y1 - m_pDesk->y ) m_pDesk->VScroller->SPos = pgppMenuBack[ n ]->y  -4 ;

			//if( pgppMenuBack[ n ]->y1 < m_pDesk->y1 )			  m_pDesk->VScroller->SPos = pgppMenuBack[ n ]->y -1	;
			//if( pgppMenuBack[ n ]->y1 > m_pDesk->VScroller->SPos ) m_pDesk->VScroller->SPos = pgppMenuBack[ n ]->y1/2;//very new //y1 +2 ;
			////if( pgppMenuBack[ n ]->y1 < m_pDesk->y1 )			  m_pDesk->VScroller->SPos = pgppMenuBack[ n ]->y -1	;
		}

		return true;
	}
	if( PersVisible )  Refresh( pHero , num ) ;
	return true;
}
//----------------------------------------------------------------------------------
void	CHeroPersDesk::ChangeActive( int heroUP )
{
	bool find = false;
	m_iCurPortrait = 0;

	for (int i=0; i < MAX_PLAYERS ; i++)	// select target Hero
		if( pgppMenuBack[i]->UserParam == heroUP ) { 
			find = true;
			pgppMenuBack[i]->SpriteID = 3 ;
		} else { 
			if( !find )  m_iCurPortrait++;
			pgppMenuBack[i]->SpriteID = 2 ; 
		}
	
	if( m_iCurPortrait < 0 )  m_iCurPortrait = 0;

	// scroll desk to target Hero
	CHero*	pHero  = reinterpret_cast<CHero*>(heroUP);		
	int PID = pHero->m_inPlayerID;
	if( pHero && m_iCurPlayer != PID && PID >=0 && PID < MAX_PLAYERS ) { 
		//m_Visible = false;
		m_iCurPlayer = PID;
		if(m_pFlag)		 m_pFlag->SpriteID = PID + 1 ;
		if(m_pCounName&&ROOT_SCENARIO.m_pPlayer[ PID ])  m_pCounName->SetMessage( ROOT_SCENARIO.m_pPlayer[ PID ]->GetPlayerName() );

		Show( ROOT_SCENARIO.m_pPlayer , ROOT_SCENARIO.m_pPlayer[ m_iCurPlayer ]->m_phrHero , 
			  ROOT_SCENARIO.m_pPlayer[ m_iCurPlayer ]->m_inHeroNum , ROOT_SCENARIO.m_Scenario.m_inLastHeroClick%100 , 
			  ROOT_SCENARIO.m_Scenario.m_bPERSONAL_HERO_MENU );

		PlayEffect( pHero->m_psdMapPic->ClickSound ,0,0);//new
	}

}
//void	CHeroPersDesk::ChangeActive( int pHero ) /*CHero* pHero*/
//{
//	for (int i=0; i < MAX_HEROES ; i++)		
//		if( pgppMenuBack[i]->UserParam == pHero )	{ pgppMenuBack[i]->SpriteID = 3 ; }
//		else										{ pgppMenuBack[i]->SpriteID = 2 ; }
//}
//----------------------------------------------------------------------------------
void	CHeroPersDesk::SetVisible( bool	state , int num )
{
 	for (int i=0; i < MAX_PLAYERS ; i++)
		if( ROOT_SCENARIO.m_pPlayer[i] && ROOT_SCENARIO.m_pPlayer[i]->m_Alive && ROOT_SCENARIO.m_pPlayer[i]->m_phrHero[ 0 ].m_Alive ) 
		{
			pgppMenuBack[i]->Visible = state;		// BackGround for heroes on m_pPersDesk
 			pgppPortrait[i]->Visible = state;		// Portrait
			ptbName[i]->Visible		 = state;		// Имя игрока.
			ptbRank[i]->Visible 	 = state;		// Звание игрока.
			ptbExpa[i]->Visible 	 = state;		// expa
			ptbVictN[i]->Visible 	 = state;		// ptbVictN
			ptbSQNum[i]->Visible 	 = state;		// SQ-num
			pgppExpBar[i]->Visible 	 = state;		// expa-BAR
			pgppExpBack[i]->Visible	 = state;		// BackGround for expa
			ptbStat[i]->Visible		 = state;
			ptbFlag[i]->Visible		 = state;
		}
		else {
 			pgppMenuBack[i]->Visible = false;		// BackGround for heroes on m_pPersDesk
 			pgppPortrait[i]->Visible = false;		// Portrait
			ptbName[i]->Visible		 = false;		// Имя игрока.
			ptbRank[i]->Visible 	 = false;		// Звание игрока.
			ptbExpa[i]->Visible 	 = false;		// expa
			ptbVictN[i]->Visible 	 = false;		// ptbVictN
			ptbSQNum[i]->Visible 	 = false;		// SQ-num
			pgppExpBar[i]->Visible 	 = false;		// expa-BAR
			pgppExpBack[i]->Visible	 = false;		// BackGround for expa
			ptbStat[i]->Visible		 = false;
			ptbFlag[i]->Visible		 = false;
		}
	m_pDesk->Visible		= state;
	if(m_pFlag)		 m_pFlag->Visible	  = state;
	if(m_pCounName)  m_pCounName->Visible = state;

	m_Visible				= state;
}
//----------------------------------------------------------------------------------
void	CHeroPersDesk::SetEnabled( bool state , int num )
{
	for (int i=0; i < MAX_PLAYERS ; i++)
		if( ROOT_SCENARIO.m_pPlayer[i] && ROOT_SCENARIO.m_pPlayer[i]->m_Alive && ROOT_SCENARIO.m_pPlayer[i]->m_phrHero[ 0 ].m_Alive ) 
		{
			pgppMenuBack[i]->Enabled = state;		// BackGround for heroes on m_pPersDesk
 			pgppPortrait[i]->Enabled = state;		// Portrait
			ptbName[i]->Enabled		 = state;		// Имя игрока.
			ptbRank[i]->Enabled 	 = state;		// Звание игрока.
			ptbExpa[i]->Enabled 	 = state;		// expa
			ptbVictN[i]->Enabled 	 = state;		// ptbVictN
			ptbSQNum[i]->Enabled 	 = state;		// SQ-num
			pgppExpBar[i]->Enabled   = state;		// expa-BAR
			pgppExpBack[i]->Enabled  = state;		// BackGround for expa
			ptbStat[i]->Enabled		 = state;
			ptbFlag[i]->Enabled		 = state;
		}
		else {
			pgppMenuBack[i]->UserParam = 0 ;
			pgppMenuBack[i]->Enabled = false;		// BackGround for heroes on m_pPersDesk
 			pgppPortrait[i]->Enabled = false;		// Portrait
			ptbName[i]->Enabled		 = false;		// Имя игрока.
			ptbRank[i]->Enabled 	 = false;		// Звание игрока.
			ptbExpa[i]->Enabled 	 = false;		// expa
			ptbVictN[i]->Enabled 	 = false;		// ptbVictN
			ptbSQNum[i]->Enabled 	 = false;		// SQ-num
			pgppExpBar[i]->Enabled   = false;		// expa-BAR
			pgppExpBack[i]->Enabled  = false;		// BackGround for expa
			ptbStat[i]->Enabled		 = false;
			ptbFlag[i]->Enabled		 = false;
		}
	m_pDesk->Enabled		= state;
	if(m_pFlag)		 m_pFlag->Enabled	  = state;
	if(m_pCounName)  m_pCounName->Enabled = state;

	m_Enabled				= state;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CHeroPersDesk   /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
CResPanel_BM::CResPanel_BM( DialogsSystem* pMenu )
{
	Init();
	
	m_pMenu = pMenu;

	CreateElements();
}
//----------------------------------------------------------------------------------
void	CResPanel_BM::Init()
{
}
//----------------------------------------------------------------------------------
void	CResPanel_BM::CreateElements()
{
	// ПАНЕЛЬ РЕСУРСОВ
	LocalGP		a_lgpResPanel("Interf3\\res_pic");

	int  list[7] = { WOOD, FOOD, STONE, GOLD, IRON, COAL, COAL +1 };

	int   x0 = 0,  curR = 0;
	for(int res=0; res <= COAL +1 ; res++) 
	{ 
		curR = list[res] ;
		x0   =  75 + 130*res ;
		// picture
		pgppResPic[ curR ] = m_pMenu->addGPPicture( NULL,  x0     , -4 ,a_lgpResPanel.GPID, curR +7 );
		// text value
		pgppResTxt[ curR ] = m_pMenu->addTextButton( NULL, pgppResPic[ curR ]->x1 +4 , 7 /*5*/ ,"0",&fonMenuText[3],&fonMenuText[3],&fonMenuText[3], 0 ); //,&WhiteFont,&WhiteFont,&WhiteFont, 0 );
		// hint
		_str  a_Hint;
		a_Hint = "{FS}{C FF502515}" ;
		char  text[256];
		sprintf( text, "#CHINT_Res%d" , curR );
		a_Hint += GetTextByID( text );

		DYNHINT_RA( pgppResPic[ curR ] , a_Hint.pchar() );		DYNHINT_RA( pgppResTxt[ curR ] , a_Hint.pchar() );
	}
}
//----------------------------------------------------------------------------------
bool	CResPanel_BM::Refresh( CPlayer* pPlayer[MAX_PLAYERS], int Nation )
{
	char   	  text[256];
	CPlayer*  pP = pPlayer[ Nation ];
	for(int res=0; res<=COAL ; res++) { 
		int  ammo = pP->GetResource( res );
		sprintf(text, "%d", ammo );	
		pgppResTxt[ res  ]->SetMessage( text );
		//		void SetResource(byte Nat,byte ID,int Amount);
		SetResource( Nation /*GetMyNation()*/ , res , ammo );
	}
	sprintf(text, "%d", pP->GetResource( RECRT ) );		
	pgppResTxt[ 6 ]->SetMessage( text );
	return  true;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

bool	CResPanel_BM::SetEnabled( bool state )//new
{
	for(int res=0; res < 7 ; res++) { 
		this->pgppResPic[ res ]->Enabled = state;
		this->pgppResTxt[ res ]->Enabled = state;
	}
	return  m_Enabled = state;
}
//----------------------------------------------------------------------------------

bool	CResPanel_BM::SetVisible( bool state )//new
{
	for(int res=0; res < 7 ; res++) { 
		this->pgppResPic[ res ]->Visible = state;
		this->pgppResTxt[ res ]->Visible = state;
	}
	return  m_Visible = state;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CMarketDesk   ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
CMarketDesk::CMarketDesk( DialogsSystem* pMenu , int MX0 , int MY0 , int MX1, int MY1 )
{ 
	Init(); 

	m_pMenu = pMenu;

	X0		= MX0 ;
	Y0		= MY0 ;
	X1		= MX1 ;
	Y1		= MY1 ;

	CreateElements();	
}
//----------------------------------------------------------------------------------
CMarketDesk::~CMarketDesk()
{ 
}
//----------------------------------------------------------------------------------
void	CMarketDesk::Init()
{
	X0 = Y0 = X1 = Y1 = 0 ;
	m_pDesk		=  NULL ;
	m_Market	=  NULL ;
	pgppMarketBack = pgppPicture	= pgppPictBak	= NULL ;
	ptbMarketTitle = ptbMarketInfo  = ptbActionMess = ptbPricesMess = NULL ; 
	for(int res=0; res<=COAL ; res++)  CurRes[res]  = -1;
	m_Visible	  = true;
	m_Enabled	  = true;		
	m_Visited	  = false;
	m_ShowActText = false;
	//
	m_ResSellPrev = -1;
	m_ShowATTime  = -1;
}
//----------------------------------------------------------------------------------
void	CMarketDesk::CreateElements() 
{
	// панель
	m_pDesk	= m_pMenu->AddDialogsDesk( X0 -1, Y0 -3 , 900, 658 /*X1 - X0 +277, Y1 - Y0 +6*/ ,"NullBorder"); 

	// картинкa-фон для рынка
	LocalGP  a_lgpMarket("Interf3\\TotalWarGraph\\bmMarket2");
	pgppMarketBack	= m_pDesk->addGPPicture(NULL, 0, 0, a_lgpMarket.GPID, 0 ); 
	// фото купцов (market identify picture)
	pgppPicture		= m_pDesk->addGPPicture(NULL, 0 /*630 +38*/ , 0 /*10 +5*/ , a_lgpMarket.GPID, 1 );
	// картинкa-фон для фото 
	LocalGP  a_lgpElems("Interf3\\TotalWarGraph\\bmElements");
	LocalGP  a_lgpElem2("Interf3\\TotalWarGraph\\bmElements2");//new
	pgppPictBak		= m_pDesk->addGPPicture(NULL, 638 , 10 , a_lgpElems.GPID, 5 ); // 748 148 // 630 31
	pgppPicture->x += pgppPictBak->x +42 ;			pgppPicture->x1 += pgppPicture->x ;
	pgppPicture->y += pgppPictBak->y +5 ;			pgppPicture->y1 += pgppPicture->y ;
	// ptbMarketTitle
	ptbMarketTitle	= m_pDesk->addTextButton(NULL,10,9,GetTextByID("#CWT_MarketTitle"),&RedFont,&RedFont,&RedFont,0);
	// рыночная инфа
	//pgppInfoBak		= m_pDesk->addGPPicture(NULL, 618, pgppPictBak->y1 +15, a_lgpElem2.GPID, 1 );
	//pddInfoDesk		= m_pDesk->AddDialogsDesk( 618, pgppPictBak->y1 +16, 264 -22 , 375 +3 , "BD" );//"EmptyBorder"); 
	pddInfoDesk		= m_pDesk->AddDialogsDesk( 618, pgppPictBak->y1 +16, 242, 378, "BD" ); //"EmptyBorder"); 

	ptbMarketInfo	= pddInfoDesk->addTextButton(NULL, 10 , 10 ," ",&BlackFont,&BlackFont,&BlackFont,0);
	//ptbMarketInfo->MaxWidth = pgppInfoBak->x1 - pgppInfoBak->x - 2*ptbMarketInfo->x ; 
	ptbMarketInfo->MaxWidth = pddInfoDesk->x1 - pddInfoDesk->x - 2*ptbMarketInfo->x ; 

	m_Market		= v_addMarket(m_pMenu, X0 , Y0 +282 , GetBigMapResource, AddBigMapResource );


	// help
	pgppActionBak	= m_pDesk->addGPPicture(NULL, 0, 454, a_lgpElem2.GPID, 0 );	// bmElements2.g17 
	pddActionDesk	= m_pDesk->AddDialogsDesk( 0, 454, 608 , 72 ,"EmptyBorder");   

	ptbActionMess	= pddActionDesk->addTextButton(NULL, 11 /*5*/ , 5 /*15,454*/ ," ",&BlackFont,&BlackFont,&BlackFont,0);
	ptbActionMess->MaxWidth = pgppMarketBack->x1 - pgppMarketBack->x -10 - 2*( ptbActionMess->x - pgppMarketBack->x );

	// help-2	/*BlackFont-Yeeee!!!*/
	ptbPricesMess	= m_pDesk->addTextButton(NULL, 11 /*15*/ , 484 ," ",&BlackFont,&BlackFont,&BlackFont,0);
	ptbPricesMess->MaxWidth = ptbActionMess->MaxWidth;

	SetVisible( false );
	SetEnabled( false );
}
//----------------------------------------------------------------------------------
bool	CMarketDesk::Show( CPlayer* pPlayer[MAX_PLAYERS] , int Nation , bool state )
{
	if( !pPlayer )   return false;

	if( !state && m_Visible ) {			// выключить
		SetVisible( false );
		SetEnabled( false );
		m_Visited = false;
		return true;
	}
	if( state && !m_Visible ) {			// включить и сменить инфу (если надо)
		SetVisible( true );
		SetEnabled( true );
	}
	if( state )	
		Refresh( pPlayer , Nation ) ;

	return true;
}
//----------------------------------------------------------------------------------
void	CMarketDesk::Refresh( CPlayer* pPlayer[MAX_PLAYERS] , int Nation )
{
	//
	//if( !ptbActionMess->Visible )
	//	m_ResSellPrev = m_ResSell;
	//
	m_ResBuy	 = m_Market->EcResID[ m_Market->ResBuy  ];			// vMarket->ResSell,vMarket->ResBuy
	m_ResSell	 = m_Market->EcResID[ m_Market->ResSell ];			// EcResID[ vMarket->ResSell ]
	m_BuyAmount  = m_Market->GetNeedToBuy();
	m_SellAmount = m_Market->NeedToSell;

	//
	const int TIME_VALUE = 1400; //2000;
	//m_ShowATTime = GetTickCount() + TIME_VALUE;
	//

	_str  a_text;
	int   NI = gi_bmSAVE_Nation;
	if( NI >= 0 && NI < MAX_PLAYERS && m_ResSell >= 0 && m_ResSell <= COAL )
	{
		m_ShowActText = false;
		if( m_SellAmount > pPlayer[ NI ]->GetResource( m_ResSell ) ) {
			a_text = GetTextByID( "#CWM_MarketNoEnoughRes" );
			m_ShowActText = true;	
			ptbActionMess->SetMessage( a_text.pchar() );
		}
		//
		if( m_ShowActText /*&& m_ResSellPrev != m_ResSell*/ ) {
			m_ResSellPrev = m_ResSell;
			m_ShowATTime  = GetTickCount() + TIME_VALUE;
		}
		if( m_ShowATTime - GetTickCount() < 0 ) {
			m_ShowATTime = -1;
		}
	}

	ptbActionMess->Visible = m_ShowActText;
	int  bt = GetTickCount();
	if( m_ShowATTime - bt > 0 ) { 
		ptbActionMess->Visible = true;
	}
	else { 
		m_ShowATTime = -1; }

	//if( NI >= 0 && NI < MAX_PLAYERS && m_ResSell >= 0 && m_ResSell <= COAL )
	//{
	//	m_ShowActText = false;
	//	if( m_SellAmount > pPlayer[ NI ]->GetResource( m_ResSell ) ) {
	//		a_text = GetTextByID( "#CWM_MarketNoEnoughRes" );
	//		m_ShowActText = true;	
	//		ptbActionMess->SetMessage( a_text.pchar() );
	//	}
	//}
	//ptbActionMess->Visible = m_ShowActText;
	//

	if( m_Visited )			{ return; }

	m_Visited = true;
	m_ShowActText = false;
	ptbMarketInfo->SetMessage( " " ) ;
	ptbActionMess->SetMessage( " " );

	char  text10[256] ,  tmp[256] ;
	strcpy( text10, GetTextByID( "#CWM_MarketInfo1" ) ) ;

	_str  mess ,  text ;
	mess  = GetTextByID( "#CWM_MarketInfo0" ) ;
	mess +=  " \\  \\" ; 

	bool  find = false;
	for(int i=0; i < MAX_PLAYERS ; i++)
		if( i != Nation && pPlayer[i]->m_Alive ) {
			text.DString::Clear();
			text = " ";
			bool  add = false;
			for(int res=0; res<=COAL ; res++) {
				int  RMIN = SECT_INCOME[ res ] + SECT_ADDING[ res ];
				if( pPlayer[i]->GetResource( res ) < RMIN || pPlayer[i]->GetResource( res ) < 1000 ) {
					if( add ) text += ", " ;
					text += GetResourseName( res );
					add = true;
				}
			}
			if( add && pPlayer[i] ) {
				text += "." ;
				sprintf( tmp, text10 , pPlayer[i]->GetPlayerName() , text.pchar() );
				mess += tmp ;
				//mess +=  " \\  \\" ; 
				find = true ;
			}
		}
	if( find )   
		ptbMarketInfo->SetMessage( mess.pchar() ) ;
}
//----------------------------------------------------------------------------------
void	CMarketDesk::SetPriceMessage( char* mess , bool state )
{
	ptbPricesMess->SetMessage( " " );
	if( state ) ptbPricesMess->SetMessage( mess );
}
//----------------------------------------------------------------------------------
void	CMarketDesk::SetVisible( bool state )
{
	m_pDesk->Visible			= state;
	m_Market->mDialog->Visible	= state;
	m_Visible					= state;
}
//----------------------------------------------------------------------------------
void	CMarketDesk::SetEnabled( bool state )
{
	m_pDesk->Enabled			= state;
	m_Market->mDialog->Enabled	= state;
	m_Enabled					= state;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//---------------------------  CBigMapHelp   ---------------------------------------
//----------------------------------------------------------------------------------
CBigMapHelp::CBigMapHelp(DialogsSystem* pMenu)
{
	Init();
	m_pMenu = pMenu;
	CreateElements();
}
//----------------------------------------------------------------------------------
CBigMapHelp::~CBigMapHelp()
{	
	Init();
}
//----------------------------------------------------------------------------------
void	CBigMapHelp::Init()
{
	m_pMenu		 = NULL;
	m_pDesk		 = NULL;
	m_pTextDesk	 = NULL;
	m_Enabled	 = true;
	m_Visible	 = true;
	for(int i=0; i < m_VisNum ; i++) m_Visited[i] = false;
	pgppBlackScreen	= pgppHelpWindow	= NULL;
	ptbHelpTitle	= ptbHelpText		= NULL;
	pgptbClose		= NULL;
}
//----------------------------------------------------------------------------------
void	CBigMapHelp::CreateElements()
{
	// BlackScreen
	LocalGP		a_lgpScreen("Interf3\\TotalWarGraph\\blackscr");			
	pgppBlackScreen	= m_pMenu->addGPPicture(NULL,0,0,a_lgpScreen.GPID,0);	//pgppBlackScreen	= m_pDesk->addGPPicture(NULL,0,0,a_lgpScreen.GPID,0);
	pgppBlackScreen->Diffuse	= 0x88202020;
	pgppBlackScreen->OnUserClick = pgppBlackScreen->OnUserRightClick = pgppBlackScreen->OnMouseOver = &EmptyClick ;


	// Help dialogs desk
	m_pMenu->ProcessDialogs();
	m_pDesk			= new DialogsDesk;
	m_pMenu->ProcessDialogs();
	//m_pDesk->ReadFromFile("Dialogs\\BM_Help.DialogsDesk.Dialogs.xml");//??
	m_pMenu->ProcessDialogs();
	m_pDesk->SafeReadFromFile( "Dialogs\\BM_Help.DialogsDesk.Dialogs.xml" );
	m_pMenu->ProcessDialogs();
	m_pMenu->DSS.Add(m_pDesk);
	/*ProcessMessages();*/
	ProcessMessages();
	m_pMenu->ProcessDialogs();	
	StdKeys();
	m_pMenu->RefreshView();

	// help text
	int   dx = 75 ,  dy = 78 ;
	int   tw  = m_pDesk->x1 - m_pDesk->x - 2*dx +10 ;
	int   th  = m_pDesk->y1 - m_pDesk->y - 2*dy -4 ;
	m_pTextDesk	= m_pDesk->AddDialogsDesk( dx, dy, tw , th , "EmptyBorder" );
	if( m_pTextDesk ) ptbHelpText = m_pTextDesk->addTextButton(NULL,0,0," ",&fonMenuText[0],&fonMenuText[0],&fonMenuText[0], 0);
	if( ptbHelpText ) ptbHelpText->MaxWidth = m_pTextDesk->x1 - m_pTextDesk->x -30 ; //-15 ;

	SetVisible( false );
	SetEnabled( false );
	/*ProcessMessages();*/
	ProcessMessages();				
	m_pMenu->ProcessDialogs();		
	StdKeys();
	m_pMenu->RefreshView();
}

//----------------------------------------------------------------------------------
bool	CBigMapHelp::CheckKey()
{
	if( KeyPressed && ( LastKey==VK_F1 || LastKey=='H' || LastKey=='h' ) ) { //if( KeyPressed && LastKey==0x48 ) {
		LastKey		= 0;
		KeyPressed	= false;
		return true;												//ROOT_SCENARIO.m_Scenario.m_bHelpVisible = true ;			
	}
	return false;
}
//----------------------------------------------------------------------------------
bool	CBigMapHelp::Show( int page , bool visible )
{
	//
	if( !ROOT_SCENARIO.m_EnabledMapClick )			return false;
	//
	if( page < 0 || page >= m_NPages || !visible )	return false;

	if( CheckKey() || !m_Visited[page] )	SetVisible( true );// check for visit page - work //if( CheckKey() )	SetVisible( true ); //if( CheckKey() || !m_Visited[page] )	SetVisible( true );// check for visit page - work

	if( m_Visible )		Refresh( page );
	if( !m_Visible )	SetVisible( false );

	return true;
}
//----------------------------------------------------------------------------------
bool	CBigMapHelp::Refresh( int page )
{
	if( !ptbHelpText )	return false;
	SetVisit( page , true );
	if( m_pTextDesk && m_pTextDesk->VScroller ) m_pTextDesk->VScroller->SPos = 0 ;

	UnPress();					//Global
	KeyPressed = LastKey = 0;   //Global

	char  text[256];
	sprintf( text, "#BigMapHelp%d" , page );
	
	_str  mess;
	mess = GetTextByID( text ) ;
	ptbHelpText->SetMessage( mess.pchar() ) ;

	 //for SetBigMapMenuEnabled
	ProcessMessages();				StdKeys();	//grey [23.02.2005]!!!
	m_pMenu->ProcessDialogs();		m_pMenu->RefreshView();	//GREY [12.05.2004]

	SetBigMapMenuEnabled( false );	//GREY [15.03.2004]

	ItemChoose = - 1;
	do { 
		ProcessMessages(); 
		if(CheckKey()) 
			ItemChoose = mcmOk;
		StdKeys();
		m_pMenu->ProcessDialogs();
		m_pMenu->RefreshView();
	} 
	while( ItemChoose!=mcmOk && ItemChoose!=mcmCancel && ItemChoose!=1 ); //while( ItemChoose!=1 ); //
	
	ItemChoose = - 1;

	LastKey		= 0;
	KeyPressed	= false;

	SetBigMapMenuEnabled( true );	//GREY [15.03.2004]

	m_Visible = false;
	return true;
}
//----------------------------------------------------------------------------------
bool	CBigMapHelp::SetVisit( int page , bool state )
{
	m_Visited[page] = state ;
	return true;
}
//----------------------------------------------------------------------------------
bool	CBigMapHelp::SetVisible(bool state)
{
	if( m_pDesk )			m_pDesk->Visible = state;
	if( pgppBlackScreen )	pgppBlackScreen->Visible = state;
	m_Visible = state;
	return true;
}
//----------------------------------------------------------------------------------
bool	CBigMapHelp::SetEnabled(bool state)
{
	if( m_pDesk )			m_pDesk->Enabled = state;
	if( pgppBlackScreen )	pgppBlackScreen->Enabled = state;
	m_Enabled = state;
	return true;
}
//----------------------------------------------------------------------------------

void	CBigMapHelp::PROCESS_HELPSYSTEM( int campID )
{
	if( ROOT_SCENARIO.m_inCurTurn < 1 ) return;
	
	int  Nation = gi_bmSAVE_Nation;
	if( Nation < 0 || Nation >= MAX_PLAYERS || !ROOT_SCENARIO.m_pPlayer[ Nation ]->m_Alive )  return;
	
	int  vid = m_NPages ; 
	// #BigMapHelp_Commander
	if( !m_Visited[ vid ] &&
		ROOT_SCENARIO.m_pPlayer[ Nation ]->m_phrHero && ROOT_SCENARIO.m_pPlayer[ Nation ]->m_inHeroNum > 0 && 
		ROOT_SCENARIO.m_pPlayer[ Nation ]->m_phrHero[ 0 ].m_Alive )
	{
		CHero* pH = &(ROOT_SCENARIO.m_pPlayer[ Nation ]->m_phrHero[ 0 ]);
		if( pH && pH->GetARMY_FreeNSQ() > 0 ) {
			m_Visited[ vid ] = true;
			ShowBigMapGlobalMessage(1, 36 ,11,GetTextByID( "#BigMapHelp_Commander" ),0);
			return;
		}
	}
	// #BigMapHelp_Resources
	if( !m_Visited[ vid + 1 ] ) 
	{
		for(int res=0; res<=COAL ; res++) 
			if( ROOT_SCENARIO.m_pPlayer[ Nation ]->GetResource( res ) < 2000 ) { 
				m_Visited[ vid + 1 ] = true;
				ShowBigMapGlobalMessage(1, 26 ,11,GetTextByID( "#BigMapHelp_Resources" ),0); 
				return;
			}
	}
	// #BigMapHelp_Diplomacy
	if( !m_Visited[ vid + 2 ] && ROOT_SCENARIO.m_pDipl && 
		ROOT_SCENARIO.m_pDipl->GetContractsNumByType( ROOT_SCENARIO.m_pDipl->m_pDiplData->GetWarId() , Nation ) > 1 ) 
	{
		m_Visited[ vid + 2 ] = true;
		ShowBigMapGlobalMessage(1, 6 ,11,GetTextByID( "#BigMapHelp_Diplomacy" ),0); 
		return;
	}
	// #BigMapHelp_Messages
	if( !m_Visited[ vid + 3 ] && ROOT_SCENARIO.m_pDipl && 
		ROOT_SCENARIO.m_pDipl->m_pDiplData->GetReqNum() > 0 )
	{
		m_Visited[ vid + 3 ] = true;
		ShowBigMapGlobalMessage(1, 11 ,11,GetTextByID( "#BigMapHelp_Messages" ),0); 
		return;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

