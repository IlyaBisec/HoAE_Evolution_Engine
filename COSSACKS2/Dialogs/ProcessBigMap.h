
char* CutStrForMaxLen(char* St, int MaxLen, RLCFont* font)
{
	_str text;								
	text = St;
	int cutL = 0, TLen=0;
	do {
		TLen = GetRLen( text.pchar() , font ) - 1;
		cutL = strlen( text.pchar() ) -1 ; // -5 -4; 
		if( cutL > 0 && TLen > MaxLen ) {
			text.pchar()[cutL--] = 0;	
			for(int space=0; space < 3 && cutL > 0; space++)
				text.pchar()[cutL--] = 46; // "."
		}
	} 
	while( TLen > MaxLen );
	return  text.pchar();
}


//*******************************************************************************************
// Class registration for animate hero and sity
/*
//////////////////////////////////////////////////////////////////////////
// CstrID
//////////////////////////////////////////////////////////////////////////
CstrID::CstrID(){
	SetEmpty();
};
//------------------------------------------------------------------------
CstrID::CstrID( int ID ){
	*this = ID;
};
//------------------------------------------------------------------------
CstrID::CstrID( const CstrID& ID ){ 
	*this = ID;
};
//------------------------------------------------------------------------
void	CstrID::operator=( int _id ){ 
	if ( ID == UNIVERSAL_EMPTY_ID )	SetEmpty();
	else							strID = ID;
};
//------------------------------------------------------------------------
bool	CstrID::operator==( int _id ){ 
	if ( _id == UNIVERSAL_EMPTY_ID && GetEmpty() ) return true;
	return false;
};
//------------------------------------------------------------------------
bool	CstrID::operator!=( int _id ){ 
	return !( *this == _id );
};
//------------------------------------------------------------------------
void	CstrID::operator=( const CstrID&	UID ){ 
	*this = UID.strID;
};
//------------------------------------------------------------------------
bool	CstrID::operator==( const CstrID&	UID ){ 
	return ( *this == UID.strID );
};
//------------------------------------------------------------------------
bool	CstrID::operator!=( const CstrID&	UID ){ 
	return !( *this == UID );
};
//------------------------------------------------------------------------
void	CstrID::operator=( const _str	UID ){
	*this = ( UID.pchar() );
};
//------------------------------------------------------------------------
bool	CstrID::operator==( const _str	UID ){
	return ( *this == UID.strID.pchar() );
};
//------------------------------------------------------------------------
bool	CstrID::operator!=( const _str	UID ){
	return !( *this == UID );
};
//------------------------------------------------------------------------
void	CstrID::operator=( const char*	UID ){
	if ( UID==NULL )	SetEmpty();
	else				strID = UID;
};
//------------------------------------------------------------------------
bool	CstrID::operator==( const char*	UID ){
	if ( UID == NULL )	return false;
	return ( strcmp( UID.strID.pchar(), UID ) == 0 );
};
//------------------------------------------------------------------------
bool	CstrID::operator!=( const char*	UID ){
	return !( *this == UID );
};
//------------------------------------------------------------------------
void	CstrID::SetEmpty(){ 
	strID = EMPTY_str;
};
//------------------------------------------------------------------------
bool	CstrID::GetEmpty(){ 
	if ( strID.pchar() == NULL )	return true;
	return ( strcmp( strID.pchar(), EMPTY_str ) == 0 );
};
//////////////////////////////////////////////////////////////////////////


void	RegAnimateClassForBigMap(){
	REG_CLASS( CstrID );

	REG_CLASS( CSATE_PARA );
	REG_CLASS( ClonesArray< CSATE_PARA > );

	REG_CLASS(CANIM_STATE);
};

CANIM_STATE	g_TEST_ASS;
void	TEST_EDIT_ASS(){
	void SimpleEditClass(char* Name,BaseClass* BC,const char* XmlName="",bool Autosave=false);
	SimpleEditClass("Animete Picture store editor", (BaseClass*)(&g_TEST_ASS) );
};
*/
//*******************************************************************************************

//-------------------------------------------------------------------------------------------
//-----   Прорисовка основного меню выбора следующей мисси.   -------------------------------
//-------------------------------------------------------------------------------------------
// Смещение по кнопкам перехода в другие меню 0x40
// Смещение по карте	0x50	+16
// Смещение по секторам 0x80	+24
// Смещение по городам  0x180   +24
// Смещение по отрядам	0xFFFF	+0xFFF = 0x10FFE
//
// Значения выбранных меню.
// 0 - World Map
// 1 - Diplomacy
// 2 - Briefing
// 3 - Personal

extern CPlayerSAVE* Profiles;//vital Profiles
CPlayerSAVE* BM_SAVE = NULL;//BM- Profiles 4test

#define	On_BigMapReturn(value)	{ BigMap_OnReturn(); return value; }

bool	BigMap_OnReturn() //new [12/03/2005]
{
	UnLoading();

	if(Profiles) {
		Profiles->Init();
		Profiles->LoadXML();
	}
	if(ROOT_SCENARIO.m_pPLAYER_SAVE) {
		ROOT_SCENARIO.m_pPLAYER_SAVE->Init();
	}
	if(BM_SAVE) {
		BM_SAVE = NULL;
	}
	return true;
}

int		g_dXM = 0, g_dYM = 0;								// Координаты верхнего левого угла карты.
int		g_WM = 0,  g_HM  = 0;								// Реальная ширина и высота карты.
RECT	g_MAP_RECT;

int		CUnit::m_inLastID			= 0;							// Последний не использованный индекс CUnit.
int		CSectData::m_LastLMCSecID	= START_SECTOR_ID; //6; //0;	// Последний сектор по которому кликнули мышкой.

bool	ChangeActiveSector(SimpleDialog* psdSec);
bool	ChangeActiveMenu(SimpleDialog* pSD);
void	ChangeActiveButton(int ActGPID,int PasGPID,RLCFont* pActF,RLCFont* pPasF,DialogsSystem* pdsMenu);
bool 	DiplActionPressed(SimpleDialog* pSD);
bool	InformationYesClick(SimpleDialog* pSD);
bool	InformationNoClick(SimpleDialog* pSD);
bool	MapButtonPressed(SimpleDialog* pSD);
bool	ON_HERO_CLICK(SimpleDialog* psdHero);				// Обрабатывает клик по герою.
bool	ON_HERO_R_CLICK(SimpleDialog* spdHero);				// Обработка правого клика по герою.
bool	ON_HERO_R_CLICK_VIRTUAL(CHero* pH);					// Виртуальная обработка правого клика по герою.
bool	ON_SQ_CLICK(SimpleDialog* spdSQ_U);					// Клик по рамке отряда.
bool	ON_SQ_R_CLICK(SimpleDialog* spdSQ_U);				// Правый лик по рамке отряда.
bool	ProcessBigMapBriefing();
bool	ProcessBigMapStatistics();
void	ProcessBigMapPlayVideo( int id = -1 );
void	ProcessMissionForPlayer(int _campainID_, bool needsave = false );
void	SetBigMapGlobalVariables();
void	SetBigMapGlobalVariablesFF();
void	ReadBriefingDirectionsFromFile();
void	test_class(BaseClass* BC);							// ***DREW test***
//
void	test_class2(BaseClass* BC);							// ***DREW test***
void	test_bmClassArr(CPlayerSAVE* PTR);				// ***grey test***
//void	test_bmClassArr(BaseClass* BC);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ДЛЯ ПОИСКА НУЖНЫХ ЮНИТОВ ПО ИДЕНТИФИКАТОРАМ.
Enumerator*	g_pALL_UNIT_TYPE = NULL;
bool	g_bEDITOR_CHENGE	 = false;	//Произошли изменения в ClassEditor ( Big Map ), обновить.
void	CREATE_COUNTRY_ENUM(CArmyLimit* pAL);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Пределы кол-ва экспы на звания 
int		LTN_EXPS_LIMIT	= 500;			
int		CPT_EXPS_LIMIT	= 3500;						//+3		//1000
int		MJR_EXPS_LIMIT	= 10000;					//+6		//4000
int		COL_EXPS_LIMIT	= 20000;					//+12		//10000
int		GEN_EXPS_LIMIT	= 40000;					//+24		//25000
int		MAR_EXPS_LIMIT	= 99999;					//			//55000
// ЭКСПА in missions
int		DisbandSQExp		= 100;					// экспа за убитый отряд
int		SecCaptExp			= 500;					// экспа за захват сектора	
int		HeroDefExp			= 500;					// экспа за победу над героем
int		QuestDisbSquadExp	= 25;					// экспа за убитый отряд-quest
int		QuestCompliteExp	= 100;					// экспа за выполн. quest
// ресурсы
int		SECT_INCOME[10] = {1,1,1,1,1,1,1,1,1,1};	// доход от сектора на каждом ходу
int		SECT_ADDING[10] = {1,1,1,1,1,1,1,1,1,1};	// доход от основного ресурса сектора
int		SECT_MAX_RECRTS  = 0 ;						// макс.кол-во рекрутов
int		SECT_REG_RECRTS  = 0 ;						// восстановление рекрутов в секторе
int		SECT_DEFENCE[10];							// затрaты на defence сектора
float	SECT_DEF_multiplier2 = 0 ;
float	SECT_DEF_multiplier3 = 0 ;
int		SECT_SABOTAGE[10][5];						// затраты for SABOTAGE
int		SECT_SAB_PERCENT[10][5];					// sabotage percent
int		BriefDIR_XY[24][8][4];						// базы армий на карте сектора (в брифинг)
int		gi_bmMISS_FRAG_LIMIT = 1000 ;				// frag-limit для отрядов в миссиях

DWORD	SELECTED_SECT_COLOR	= 0x88000000;			// selected sector
DWORD	RED_SECT_COLOR		= 0xFFFF0000;			// сост.войны - дипломатия
DWORD	GREEN_SECT_COLOR	= 0xFF00AA00;			// договора   - дипломатия
DWORD	COMPAGE_BLINKCOLOR;							// commanders Page blink color

int		STEPBACK_TURNLIMIT = 2;  //4;			// 5;
int		STEPBACK_DISTANCE  = 2;					// 2;


//*****************Amimate State******************



//************************************************

//*********************TEST***********************
//#ifdef TEST_LVA
//	DialogsSystem	DS_01,DS_02,DS_SEC_SD,DS_SEC_DD;
//	ClassEditor		CE_01,CE_02,CE_SEC_SD,CE_SEC_DD;
//	//bool			bCE_01_SHOW = false;
//	//bool			bCE_02_SHOW = false;
//	bool			bCE_SEC_SHOW= false;
//#endif
//Задать энумератор для рангов героев.						//Enumerator*		RANK_ENUM = NULL;
//Задать энумератор для направлений ударов по секторам.		//Enumerator*		SDIR_ENUM = NULL;
// энумераторы к сообщениям
Enumerator* MessHead_ENUM = NULL;							//Enumerator* MessText_ENUM = NULL; Enumerator* InfoLtr_ENUM  = NULL; Enumerator* InfoTxt_ENUM  = NULL; Enumerator* BUTTON_ENUM   = NULL;
//************************************************


//Указатели на основные управляющие классы.
CROOTDRIVER		ROOT_SCENARIO;
SFOR_PLAYER		CPlayer::DataForAddHero;
CHero*			pAtackerHero = NULL;					// Герой атакующий сектор.
CHero*			pDefendeHero = NULL;					// Герой защитник.

//Данные по инициализации миссии.
CMISSIONINIT		MissionInitData;
CSAVE_MIS_STRUCT	smsSAVE;
CEndMissionData		EndMissionData;
//Данные по направлениям ударов.
CScript				SCRIPT_DIRECTION;
//Структура с данными по игроку.
CPlayerSAVE_STRUCT*	pSAVE = NULL;						// указатель на структурy-save игрока
int  gi_bmSAVE_Nation  = 0;								// id нации игрока в Big Map Camp-s


bool	ProcessBigMap(int campainID)
{

	// ***** TEST *****
	//save rand() random	//if( m_bSaveLogs ) 
	//{
	//	_str  txt;							
	//	for(int i=0; i < 100; i++){
	//		txt += i ;
	//		txt += " - ";
	//		txt += rand()%100 ;
	//		txt+= "\n";				
	//	}
	//	DString  a_DS( txt.pchar() );
	//	char  name[256];
	//	sprintf( name , "Missions\\rand%d.log", GetTickCount()%100 );
	//	a_DS.WriteToFile( name );
	//}
	// ***** TEST *****

	if(Profiles) {
		//
		Profiles->Init(); //new
		//
		test_bmClassArr( Profiles ); //new 
	}
	// set res
	for(int nati =0; nati < MAX_PLAYERS; nati++){
		for(int res=0; res<=COAL ; res++) { 
			SetResource( nati , res , 100000 );
		}
	}

	bool	EndOfTurn = true;
	v_Exit2MainMenu   = false;   //Global

	//CPlayerSAVE		PLAYER_SAVE;						// Загрузить данные из файла игрока.
	//test_bmClassArr( &PLAYER_SAVE );//new
	//pSAVE = PLAYER_SAVE.m_pPlayer[0];
	//ROOT_SCENARIO.m_pPLAYER_SAVE = &PLAYER_SAVE ;
	//BM_SAVE = &PLAYER_SAVE ;//4TEST

	ROOT_SCENARIO.Init();
	ROOT_SCENARIO.m_Scenario.m_inBigMapCampID = campainID;  
	//ROOT_SCENARIO.m_pPLAYER_SAVE = &PLAYER_SAVE ;
	//BM_SAVE = &PLAYER_SAVE ;//4TEST
	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles ); //new 


	// change nation
	//DWORD mc;
	//int	sp = gi_bmSAVE_Nation = GetBigMapNationID(pSAVE->m_iNation,mc);

#ifdef DemoVer4Journals	//DEMO
	sp = gi_bmSAVE_Nation = 1; // global
#endif					//DEMO

	SetBigMapGlobalVariables(); // Set BigMap-campaign Global variables

	MessHead_ENUM = ENUM.Get("MessHead_ENUM");	// энумератор заголовков сообщений игроку.
	if( !MessHead_ENUM->GetAmount() ) { 
		int  n = 34 ;		char  s[256] ;
		for(int i=0; i <= n ; i++) { sprintf(s, "#CDT_MessHead%d", i); MessHead_ENUM->Add(GetTextByID(s), i); }   
	}

	//*************************TEST***************************
	//RANK_ENUM	= ENUM.Get("RANK_ENUM");	//Задать энумератор для рангов героев.
	//if (!RANK_ENUM->Values.GetAmount()) { RANK_ENUM->Add( GetTextByID( "#LIEUTENANT"	),	0 );
	//										RANK_ENUM->Add( GetTextByID( "#CAPTAIN"		),	1 );
	//										RANK_ENUM->Add( GetTextByID( "#MAJOR"		),	2 );
	//										RANK_ENUM->Add( GetTextByID( "#COLONEL"		),	3 );
	//										RANK_ENUM->Add( GetTextByID( "#GENERAL"		),	4 );
	//										RANK_ENUM->Add( GetTextByID( "#MARSHAL"		),	5 );   }
	//********************************************************
	//MessText_ENUM = ENUM.Get("MessageTexts");	// энумератор текстов дипл. действий игрока.
	//if (!MessText_ENUM->Values.GetAmount()) { int n=6; char s[256];
	//											for(int i=0; i<n; i++) { sprintf(s, "#CDM_LetAContract%d", i ); MessText_ENUM->Add( GetTextByID(s), i );   }
	//											MessText_ENUM->Add( GetTextByID("#CDM_NoEnoughMoney"),  n );   }
	//********************************************************


    DialogsSystem	a_dsMenu(0,0);					//Меню
	a_dsMenu.SetWidth(1024);
	a_dsMenu.SetHeight(768);

	GPPicture*	a_pgppBlackScreen = ShowBlackScreen(&a_dsMenu);
	//ProcessMessages();		StdKeys();		a_dsMenu.ProcessDialogs();		a_dsMenu.RefreshView();


	//Размеры меню карты.
	int a_inMX0 = 75;	// 75x113
	int a_inMY0 = 113;
	int a_inMW  = 600 ;	//600x531
	int a_inMH  = 530 ;

	int a_inMX1 = a_inMX0 + a_inMW ; //675x643
	int a_inMY1 = a_inMY0 + a_inMH ;

	g_MAP_RECT.left		= a_inMX0+2;
	g_MAP_RECT.top		= a_inMY0+2;
	g_MAP_RECT.right	= a_inMX1-2;
	g_MAP_RECT.bottom	= a_inMY1-2;

	//Background
	SQPicture   a_sqpBack("Interf3\\TotalWarGraph\\lva_Turn_Map.bmp"); //Фон
	a_dsMenu.addPicture(NULL,0,0,&a_sqpBack,&a_sqpBack,&a_sqpBack);

	//Установка хинтов. Прорисовка под мышкой.
	a_dsMenu.HintX		= -1;
	a_dsMenu.HintY		= 200;
	a_dsMenu.HintLX		= 200;
	a_dsMenu.HintLY		= 200;
	a_dsMenu.HintStyle	= 0;
	a_dsMenu.HintFont	= &BlackFont;

	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	
	//Сама карта
	LocalGP		a_lgpMap00("Interf3\\TotalWarGraph\\lva_Europe00");		//Map

	GPPicture*	a_pgppMap = NULL;
	a_dsMenu.addClipper(a_inMX0,a_inMY0,a_inMX1,a_inMY1);
	for (int i=0; i<9; i++) { //Загрузка кусков карты в a_dsMenw.
		a_pgppMap = a_dsMenu.addGPPicture(NULL,0,0,a_lgpMap00.GPID,i);
		a_pgppMap->ShapeFileID = a_lgpMap00.GPID;
		a_pgppMap->ShapeSpriteID = i;

		a_pgppMap->Diffuse = 0xFFFFFFFF;
		a_pgppMap->AssignID(i+0x50);
		a_pgppMap->Visible = false;
	}
	a_dsMenu.addClipper(0,0,1024,768);
	CPicesPict	a_MapPic(&a_dsMenu,3,3,0x50);
	a_MapPic.SetScreen(a_inMX0,a_inMY0,a_inMH,a_inMW);
	a_MapPic.GetMapSize(g_dXM,g_dYM,g_WM,g_HM);

	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// Данные по направлениям ударов.
	//xmlQuote	xmlScript_Dir( "CScript" );
	//xmlScript_Dir.ReadFromFile( "Missions\\MissionEngin\\DirectionDataBase.script.xml" );
	//ErrorPager Err;
	//SCRIPT_DIRECTION.Load( xmlScript_Dir, &SCRIPT_DIRECTION, &Err );

	SCRIPT_DIRECTION.SafeReadFromFile( "Missions\\MissionEngin\\DirectionDataBase.script.xml" );
	
	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// База по секторам.
	CSectData	sdSecDat("Missions\\StatData\\Sectors.dat");
	//PLAYER_SAVE.setSECTORA(&sdSecDat);

	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	sdSecDat.CreateTempDataFiles();
	sdSecDat.ReCountCoord(g_WM,g_HM);

	LocalGP		a_lgpSec("Interf3\\TotalWarGraph\\lva_Sectors");			//Sectors
	GPPicture*	a_pgppSec = NULL;
	a_dsMenu.addClipper(a_inMX0,a_inMY0,a_inMX1,a_inMY1);
	for (int i=0; i<sdSecDat.GetSectorsNum(); i++) {	//Загрузка кусков карты в a_dsMenw.
		a_pgppSec = a_dsMenu.addGPPicture(NULL,0,0,a_lgpSec.GPID,i);
		
		a_pgppSec->Diffuse = 0x55000000 + 0x00FFFFFF;
		a_pgppSec->ShapeFileID = a_lgpSec.GPID;
		a_pgppSec->ShapeSpriteID = i;
		a_pgppSec->AssignID(i+0x80);
		a_pgppSec->OnUserClick = &ChangeActiveSector;
		a_pgppSec->UserParam = i;
		a_pgppSec->Visible = true;

		a_pgppSec->AssignSound( "INTERF_SECTOR", CLICK_SOUND );
	}
	a_dsMenu.addClipper(0,0,1024,768);
	sdSecDat.SetSecOnMap(0x80,&a_dsMenu);
	
	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// Cоздание и установка городов и башен в секторах.
	//LocalGP		a_lgpSity("Interf3\\TotalWarGraph\\lva_SityTower");			// City and Tower
	//LocalGP		a_lgpFORT("Interf3\\TotalWarGraph\\lva_SityTower");			// Towers
	LocalGP		a_lgpSity("Interf3\\TotalWarGraph\\bmPopulat");			// City
	GPPicture*	a_pgppSity = NULL;
	LocalGP		a_lgpFORT("Interf3\\TotalWarGraph\\bmDefence");			// Towers
	GPPicture*	a_pgppFORT = NULL;
	a_dsMenu.addClipper(a_inMX0,a_inMY0,a_inMX1,a_inMY1);
	float  SclX = 0.84500000  ,  SclY = 0.86000000 ;						//SclX = 0.80000000 , SclY = 0.65000000 ;
	for(int i=0; i < sdSecDat.GetSectorsNum() ; i++) {
		a_pgppSity = a_dsMenu.addGPPicture(NULL,0,0,a_lgpSity.GPID,0);
		//a_pgppSity->Rotated = true ;	//a_pgppSity->ScaleX  = SclX ;	// 0.84500000;	// 0.86;	//0.88;	//a_pgppSity->ScaleY  = SclY ;	// 0.86000000;	// 0.86;	//0.88;	//a_pgppSity->Diffuse = 0xFF000000 + 0x00FFFFFF;
		a_pgppSity->Visible		  = true; // !!!! NEEDED !!!! 
		a_pgppSity->ShapeFileID   = a_lgpSity.GPID;
		a_pgppSity->ShapeSpriteID = 0;
		a_pgppSity->AssignID( 2*i   + 0x180 ); //( i + 0x180 );
		//new
		a_pgppFORT = a_dsMenu.addGPPicture(NULL,0,0,a_lgpFORT.GPID,0);
		a_pgppFORT->Visible		  = true;
		a_pgppFORT->ShapeFileID   = a_lgpFORT.GPID;
		a_pgppFORT->ShapeSpriteID = 0;
		a_pgppFORT->AssignID( 2*i+1 + 0x180 );
		//new
	}
	a_dsMenu.addClipper(0,0,1024,768);
	sdSecDat.SetSityOnMap(0x180,&a_dsMenu);

	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	////Стрелочки движения отрядов и героев.
	////LocalGP		a_lgpArrow("Interf3\\TotalWarGraph\\lva_Arrow");
	//LocalGP		a_lgpNewArrow("Interf3\\TotalWarGraph\\lva_NewArrow");
	//a_dsMenu.addClipper(a_inMX0,a_inMY0,a_inMX1,a_inMY1);
	////sdSecDat.AddArrowPictures(&a_lgpArrow,0);
	//sdSecDat.AddNewArrowPictures(&a_lgpNewArrow,84);
	//a_dsMenu.addClipper(0,0,1024,768);

	//Create sector menu.
	CSectorMenu		a_smSectMenu(&sdSecDat);
	LocalGP			a_lgpMiniMaps("Interf3\\TotalWarGraph\\lva_SectMiniMP");
	a_smSectMenu.CreateMiniMaps(&a_dsMenu,&a_lgpMiniMaps,sdSecDat.GetSectorsNum());
	LocalGP			a_lgpPicButt("Interf3\\TotalWarGraph\\lva_SectMN");
	LocalGP			a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");
	a_smSectMenu.CreatePictureAndButton(&a_dsMenu,&a_lgpPicButt,&a_lgpFlags);

	a_smSectMenu.SetMenuPosition();
		
	
	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	//Конструируем кнопки перехода в другие меню.
	LocalGP		a_lgpPageButton("Interf3\\TotalWarGraph\\lva_Pages");
	RLCFont*	a_prlcfPgAct = &OrangeFont;
	RLCFont*	a_prlcfPgPas = &GrayFont;

	//int			a_inPBX0 = 449;	//Начало кнопок по X
	//int			a_inPBY0 = 629;	//Начало кнопок по Y
	int			a_inPBX0 = a_inMX0 +  3 /* 75 +  3*/ ;	//Начало кнопок по X
	int			a_inPBY0 = a_inMY1 + 10 /*643 + 10*/ ;	//Начало кнопок по Y

	int			a_inPBBW = GPS.GetGPWidth (a_lgpPageButton.GPID,3);	//Ширина кнопки.
	int			a_inPBBH = GPS.GetGPHeight(a_lgpPageButton.GPID,3);	//Высота кнопки.
	char		a_chPageName[5][256],  a_chDBHint[5][256],  a_chPageSnd[5][256];	
	strcpy(a_chPageName[0],GetTextByID( "#CWV_WorldMap" ));
	strcpy(a_chPageName[1],GetTextByID( "#CWV_Diplomacy"));
	strcpy(a_chPageName[2],GetTextByID( "#CWV_Personal" )); 
	strcpy(a_chPageName[3],GetTextByID( "#CWV_Market"	));	
	strcpy(a_chPageName[4],GetTextByID( "#CWV_Messages" ));	//strcpy(a_chPageName[5],GetTextByID( "#CWV_Briefing"	)); 

	strcpy(a_chDBHint[0],GetTextByID("#CHINT_WorldMap")); 	//hint к кнопкам дипл.действий
	strcpy(a_chDBHint[1],GetTextByID("#CHINT_Diplomacy")); 
	strcpy(a_chDBHint[2],GetTextByID("#CHINT_Personal")); 
	strcpy(a_chDBHint[3],GetTextByID("#CHINT_Market")); 
	strcpy(a_chDBHint[4],GetTextByID("#CHINT_Messages")); //strcpy(a_chDBHint[5],GetTextByID("#CHINT_Briefing")); 

	strcpy(a_chPageSnd[0], "INTERF_PAGE_WMAP"  ); 	//hint к кнопкам дипл.действий
	strcpy(a_chPageSnd[1], "INTERF_PAGE_DIPL" ); 
	strcpy(a_chPageSnd[2], "INTERF_PAGE_PERS"  ); 
	strcpy(a_chPageSnd[3], "INTERF_PAGE_MARK"    ); 
	strcpy(a_chPageSnd[4], "INTERF_PAGE_MESS"  );	//strcpy(a_chPageSnd[5], "#CHINT_Briefing" ); 


	_str	a_Hint;
	GP_TextButton* a_pgptbWM = NULL;
	for(int i=0; i < 5 ; i++) {	
		a_pgptbWM = a_dsMenu.addGP_TextButton(NULL,0,0,a_chPageName[i],a_lgpPageButton.GPID,3,a_prlcfPgAct,a_prlcfPgPas);
		a_pgptbWM->AssignID( 0x40+i );

		a_pgptbWM->AssignSound( a_chPageSnd[i] , CLICK_SOUND );
		//a_pgptbWM->AssignSound( "INTERF_PAGE", CLICK_SOUND );

		//if( i == ROOT_SCENARIO.m_Scenario.m_inActivMenu ) {
		//	a_pgptbWM->Sprite = a_pgptbWM->Sprite1 = 2;
		//	a_pgptbWM->ActiveFont = a_pgptbWM->PassiveFont = a_prlcfPgAct;   } 
		//else {
		//	a_pgptbWM->Sprite = a_pgptbWM->Sprite1 = 3;
		//	a_pgptbWM->ActiveFont = a_pgptbWM->PassiveFont = a_prlcfPgPas;   }
		a_pgptbWM->x  = a_inPBX0+i*(a_inPBBW-1) /*-3 */;
		a_pgptbWM->y  = a_inPBY0;
		a_pgptbWM->x1 = a_inPBX0+(i+1)*(a_inPBBW-1) /*-3*/ ;
		a_pgptbWM->y1 = a_inPBY0+a_inPBBH;
		if( i == 0 ) {
			a_pgptbWM->x  = a_inPBX0+i*a_inPBBW ;
			a_pgptbWM->y  = a_inPBY0;
			a_pgptbWM->x1 = a_inPBX0+(i+1)*a_inPBBW ;
			a_pgptbWM->y1 = a_inPBY0+a_inPBBH;
		}
		a_pgptbWM->x			-=  3 ;			a_pgptbWM->x1	-= 3 ;
		a_pgptbWM->FontDy		 = -2 ;
		a_pgptbWM->UserParam	 =  i ;
		a_pgptbWM->OnUserClick	 =  &ChangeActiveMenu;

		a_Hint.DString::Clear();  a_Hint+="{FS}{C FF502515}";  a_Hint+=a_chDBHint[i];  DYNHINT(a_pgptbWM,a_Hint.pchar());
		//if( i == 4 ) {
		//	a_dsMenu.addGPPicture(NULL,a_pgptbWM->x1+1,a_pgptbWM->y+1,a_lgpPageButton.GPID,4);
		//	a_dsMenu.addGPPicture(NULL,a_pgptbWM->x1+10,a_pgptbWM->y+1,a_lgpPageButton.GPID,4);
		//	a_dsMenu.addGPPicture(NULL,a_inMX1-GPS.GetGPWidth(a_lgpPageButton.GPID,4)+1,a_pgptbWM->y+1,a_lgpPageButton.GPID,4); }
	}

	//Подгрузим различные типы юнитов.
	UnLoading();
	LOADNATMASK = 0xFFFFFFFF;
	LoadAllNations(1,0);

	//Количества отрядов в армии в зависимости от звания героя.
	CArmyLimit	Rank_Type_Country("Missions\\StatData\\HeroArmy.dat");
	CREATE_COUNTRY_ENUM(&Rank_Type_Country);  //Расставить индексы из NATIONS[0].Mon[...]
	
	//Создать портреты героев для всех игроков.
	LocalGP  lgpPORTRAIT_EG( "Interf3\\TotalWarGraph\\lva_EG" );   LocalGP  lgpPORTRAIT_EGs( "Interf3\\TotalWarGraph\\lva_EGs" );
	LocalGP  lgpPORTRAIT_FR( "Interf3\\TotalWarGraph\\lva_FR" );   LocalGP  lgpPORTRAIT_FRs( "Interf3\\TotalWarGraph\\lva_FRs" );
	LocalGP  lgpPORTRAIT_AS( "Interf3\\TotalWarGraph\\lva_AS" );   LocalGP  lgpPORTRAIT_ASs( "Interf3\\TotalWarGraph\\lva_ASs" );
	LocalGP  lgpPORTRAIT_PR( "Interf3\\TotalWarGraph\\lva_PR" );   LocalGP  lgpPORTRAIT_PRs( "Interf3\\TotalWarGraph\\lva_PRs" );
	LocalGP  lgpPORTRAIT_RS( "Interf3\\TotalWarGraph\\lva_RS" );   LocalGP  lgpPORTRAIT_RSs( "Interf3\\TotalWarGraph\\lva_RSs" );
	LocalGP  lgpPORTRAIT_EP( "Interf3\\TotalWarGraph\\lva_EP" );   LocalGP  lgpPORTRAIT_EPs( "Interf3\\TotalWarGraph\\lva_EPs" );
	CHERO_PORTRAIT	PORTRAIT;
    PORTRAIT.addPortrait( &lgpPORTRAIT_EG, &lgpPORTRAIT_EGs, 0 );
	PORTRAIT.addPortrait( &lgpPORTRAIT_FR, &lgpPORTRAIT_FRs, 1 );
	PORTRAIT.addPortrait( &lgpPORTRAIT_AS, &lgpPORTRAIT_ASs, 2 );
	PORTRAIT.addPortrait( &lgpPORTRAIT_PR, &lgpPORTRAIT_PRs, 3 );
	PORTRAIT.addPortrait( &lgpPORTRAIT_RS, &lgpPORTRAIT_RSs, 4 );
	PORTRAIT.addPortrait( &lgpPORTRAIT_EP, &lgpPORTRAIT_EPs, 5 );
    PORTRAIT.addToMenu( &a_dsMenu );

	//Малое меню для героя.
	LocalGP		lgpMMBTN	( "Interf3\\TotalWarGraph\\lva_Hmm"	 );
	LocalGP		lgpAwards	( "Interf3\\TotalWarGraph\\lva_Awards" );
	CHERO_MINI_MENU	HERO_MM;	//Мини меню для героя.
	HERO_MM.AddBTNS_MENU( &lgpMMBTN,   &a_dsMenu );
	HERO_MM.AddSTAT_MENU( &lgpAwards,  &a_dsMenu );
	HERO_MM.AddFLAG_MENU( &a_lgpFlags, &a_dsMenu );
	//HERO_MM.setMPos( 190, 530 );
	HERO_MM.SetMPos( 705 , 542 );//grey		// 694 , 540 );	// 830, 530 );

	HERO_MM.SetVisible( true );
	
	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	//Создание игроков, учасников игры.
	CPlayer			PLAYER0(0),PLAYER1(1),PLAYER2(2),
					PLAYER3(3),PLAYER4(4),PLAYER5(5);
	CPlayer*		PLAYERS[MAX_PLAYERS];
	PLAYERS[0]	=	&PLAYER0;	PLAYERS[1]	=	&PLAYER1;
	PLAYERS[2]	=	&PLAYER2;	PLAYERS[3]	=	&PLAYER3;
	PLAYERS[4]	=	&PLAYER4;	PLAYERS[5]	=	&PLAYER5;

	LocalGP		a_lgpArmy("Interf3\\TotalWarGraph\\lva_Army");

	//-------------------------------------------
	CPlayer::DataForAddHero.mx0 = a_inMX0;		CPlayer::DataForAddHero.my0 = a_inMY0;
	CPlayer::DataForAddHero.mx1 = a_inMX1;		CPlayer::DataForAddHero.my1 = a_inMY1;
	CPlayer::DataForAddHero.sx0 = 0;			CPlayer::DataForAddHero.sy0 = 0;
	CPlayer::DataForAddHero.sx1 = 1024;			CPlayer::DataForAddHero.sy1 = 768;
	CPlayer::DataForAddHero.pMenu		 = &a_dsMenu;
	CPlayer::DataForAddHero.pSectData	 = &sdSecDat;
	CPlayer::DataForAddHero.pfHeroClick	 = &ON_HERO_CLICK;
	CPlayer::DataForAddHero.pfHeroRClick = &ON_HERO_R_CLICK;
	CPlayer::DataForAddHero.pMapPic		 = &a_lgpArmy;
	CPlayer::DataForAddHero.pArmyLimit	 = &Rank_Type_Country;
	CPlayer::DataForAddHero.pPORTRAIT	 = &PORTRAIT;
	CPlayer::DataForAddHero.pHMiniMENU	 = &HERO_MM;
	//-------------------------------------------

	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	PLAYERS[0]->SetPlayerName( GetTextByID( "#ENGLAND" ) );
	PLAYERS[1]->SetPlayerName( GetTextByID( "#FRANCE"  ) );
	PLAYERS[2]->SetPlayerName( GetTextByID( "#AUSTRIA" ) );
	PLAYERS[3]->SetPlayerName( GetTextByID( "#PRUSSIA" ) );
	PLAYERS[4]->SetPlayerName( GetTextByID( "#RUSSIA"  ) );
	PLAYERS[5]->SetPlayerName( GetTextByID( "#EGYPT"   ) );
	for(int i=0; i < MAX_PLAYERS; i++) {
		PLAYERS[i]->SetSetcData(&sdSecDat);
		PLAYERS[i]->AddPortH();//TEMP!!! //Old Vic Ver.
	}


	//test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	CPlayerSAVE		PLAYER_SAVE;						// Загрузить данные из файла игрока.
	test_bmClassArr( &PLAYER_SAVE );//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	ROOT_SCENARIO.m_pPLAYER_SAVE = &PLAYER_SAVE ;
	PLAYER_SAVE.setSECTORA(&sdSecDat);

	pSAVE	= PLAYER_SAVE.m_pPlayer[0];
	BM_SAVE	= &PLAYER_SAVE ;//4TEST

	// change nation
	DWORD mc;
	int	sp = gi_bmSAVE_Nation = GetBigMapNationID(pSAVE->m_iNation,mc);

	// set ress
	SetBigMapGlobalVariables(); // Set BigMap-campaign Global variables

#ifdef DemoVer4Journals	//DEMO
	sp = gi_bmSAVE_Nation = 1; // global
#endif					//DEMO
#ifdef DemoAI			//DEMO
	int ai = DemoAI( sp) ;
	PLAYERS[ sp  ]->AddHero( sdSecDat.GetSecIDbyOwner( sp )	, pSAVE->m_iCurHeroId );		
	PLAYERS[ ai ]->AddHero( sdSecDat.GetSecIDbyOwner( ai )	, 0 );
	goto BMlabel;
#endif					//DEMO


	int pid = sdSecDat.GetSecIDbyOwner( sp );
	if( pid < 0 )  pid = 6;
	PLAYERS[ sp ]->AddHero( pid , pSAVE->m_iCurHeroId );
	//PLAYERS[ sp ]->AddHero( sdSecDat.GetSecIDbyOwner( sp ), pSAVE->m_iCurHeroId );
	//if(sdSecDat.GetSecIDbyOwner( sp )==-1)  PLAYERS[ sp ]->m_phrHero = NULL;

	if(Profiles)  test_bmClassArr( Profiles );//new

	for(int i=0; i < MAX_PLAYERS ; i++) 
		if( i != sp ) {
			//int heroID = rand()%10 ;	if( heroID > 6 ) heroID = 6 ;
			PLAYERS[ i ]->AddHero( sdSecDat.GetSecIDbyOwner( i ) , 0 ) ;//heroID );

			if(Profiles)  test_bmClassArr( Profiles );//new
		}

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

BMlabel:

	// Army + Resources
	int  foodR = ROOT_SCENARIO.m_iBigMap_FOOD_RATIO ; 
	int	 resPL = ROOT_SCENARIO.m_iBigMap_User_RESOURCES ;
	int  resAI = ROOT_SCENARIO.m_iBigMap_AI___RESOURCES ; 
	for(int pa=0; pa < MAX_PLAYERS ; pa++) {
		PLAYERS[pa]->SetDefaultARMY();
		for(int res=0; res<=COAL ; res++) {  
			if( pa != sp )				{ PLAYERS[pa]->SetResource(res, resAI); }
			else						{ PLAYERS[pa]->SetResource(res, resPL); }
		}
		if( pa != sp )					{ PLAYERS[pa]->SetResource( FOOD, foodR * resAI ); } // 4 * resAI ); }
		else							{ PLAYERS[pa]->SetResource( FOOD, foodR * resPL ); } // 4 * resPL ); }
		PLAYERS[pa]->SetRecruit();
	}


	// PERSONAL HERO MENU
	//LocalGP		lgpFrame( "Interf3\\TotalWarGraph\\lva_HFRM" );//vic ver
	CPERSONAL	HERO_PERSONAL;/*( sp );*/
	HERO_PERSONAL.m_PlayerID  = HERO_PERSONAL.m_iHumanNat = sp ;
	HERO_PERSONAL.setPLAYERS(PLAYERS);
	//HERO_PERSONAL.setFarme(&a_dsMenu,&lgpFrame);//vic ver
	LocalGP		lgpElems( "Interf3\\TotalWarGraph\\bmElements" );
	HERO_PERSONAL.setFarme( &a_dsMenu , &lgpElems , &Rank_Type_Country );
	HERO_PERSONAL.setUserClick( &ON_SQ_CLICK, &ON_SQ_R_CLICK );
	//HERO_PERSONAL.setUnis(&a_dsMenu,&Rank_Type_Country);//vic ver
	HERO_PERSONAL.setVisible(false);
	HERO_PERSONAL.UpdateCoord();
	HERO_PERSONAL.UpdateData();

	if(Profiles)  test_bmClassArr( Profiles );//new

    // --------------------------------------------------------------------------------------------------------------
	// ---------------     ПАНЕЛЬ РЕСУРСОВ  &&  ЗАГОЛОВОК МЕНЮ  &&  Кнопки OK/NO     --------------------------------
	// --------------------------------------------------------------------------------------------------------------

	CResPanel_BM	a_ResPanel( &a_dsMenu );						// ПАНЕЛЬ РЕСУРСОВ
	ROOT_SCENARIO += &a_ResPanel;

	// ЗАГОЛОВОК МЕНЮ
	TextButton*  ptbBigMapHead = a_dsMenu.addTextButton(NULL,0,0,"EC:",&fonMenuTitle[3],&fonMenuTitle[3],&fonMenuTitle[3],0);//NEED NEW FONTS!!!
	DialogsDesk* pddFlagDesk   = a_dsMenu.AddDialogsDesk(0,0,    28 , 18 , "NullBorder");			// 32x24-flag
	GPPicture*   ptbBigMapFlag = pddFlagDesk->addGPPicture(NULL, -2 , -3 , a_lgpFlags.GPID , sp +1 );
	//GPPicture*   ptbBigMapFlag = pddFlagDesk->addGPPicture(NULL, -3 , -4 , a_lgpFlags.GPID , sp +1 );
	//GPPicture*  ptbBigMapFlag = a_dsMenu.addGPPicture(NULL,0,0, a_lgpFlags.GPID , sp +1 ); 

	//_str testtxt;
	//testtxt = CutStrForMaxLen( pSAVE->m_chName.pchar() , 100 , ptbBigMapHead->PassiveFont );

	int   HeadLimit = 535; //450-550;
	int   cutL = 0, Tlen=0, Flen = ptbBigMapFlag->x1 - ptbBigMapFlag->x;
	char  head[256], name[256];												
	strcpy( name , pSAVE->m_chName.pchar() );
	do {
		sprintf( head , GetTextByID("#CWT_BigMapHead") , name , PLAYERS[ sp ]->GetPlayerName() );
		ptbBigMapHead->SetMessage( head );
		int rlen = GetRLen( head , ptbBigMapHead->PassiveFont );
		ptbBigMapHead->x1 = ptbBigMapHead->x+rlen-1 ;
		Tlen = ptbBigMapHead->x1 - ptbBigMapHead->x;
		cutL = strlen( name ) - 2 ; //- 5; //- 7 - 5;
		if( cutL > 0 && Tlen + Flen > HeadLimit) {
			name[cutL--] = 0;	
			for(int space=0; space < 3; space++)  { name[cutL--] = 46; } /*name[cutL--] = 32;*/
		}
	} while( Tlen + Flen > HeadLimit );
	

	int  bmHX = 512,  bmHY = 67 ; 
	ptbBigMapHead->Align = 1;		ptbBigMapHead->x = bmHX - Flen/2 - 5;		ptbBigMapHead->y = bmHY;
	int rlen=GetRLen( head , ptbBigMapHead->PassiveFont );
	ptbBigMapHead->x  -= rlen>>1 ;
	ptbBigMapHead->x1  = ptbBigMapHead->x+rlen-1 ;
	ptbBigMapHead->y1  = ptbBigMapHead->y+GetRLCHeight(ptbBigMapHead->PassiveFont->RLC,'W') ;
	//new
	//setCoordSD( ptbBigMapFlag , ptbBigMapHead->x1 + 10 , bmHY -4 );
	pddFlagDesk->Setx( ptbBigMapHead->x1 + 10 );
	pddFlagDesk->Sety( 64 /*bmHY -4*/ );
	//new

	// Кнопки OK/NO
	int			OKx = 287 +2 ,	OKy = 709 +4 ;						
	int			NOx = 530 +2 ,	NOy = OKy ;
	LocalGP		a_lgpOK_F("Interf3\\elements\\button_back");
	LocalGP		a_lgpOKNO("Interf3\\TotalWarGraph\\dMessage");
	GPPicture*	pgpbOKfon  = a_dsMenu.addGPPicture(NULL, 0 , 680 ,a_lgpOK_F.GPID, 1 );
	GP_TextButton*  pgpbOK = a_dsMenu.addGP_TextButton(NULL,OKx,OKy,GetTextByID("#CWB_EndOfTurn"),a_lgpOKNO.GPID,6, &fonMenuText[3],&fonMenuText[2]);//&WhiteFont,&YellowFont);//grey
	GP_TextButton*  pgpbNO = a_dsMenu.addGP_TextButton(NULL,NOx,NOy,GetTextByID("#CWB_Quit"),a_lgpOKNO.GPID,6, &fonMenuText[3],&fonMenuText[2]);//&WhiteFont,&YellowFont);//grey
	pgpbOK->FontDy		= pgpbNO->FontDy	  = -1 ;
	pgpbOK->OnUserClick = pgpbNO->OnUserClick = &MMItemChoose;
	pgpbOK->UserParam	= mcmOk;				
	pgpbNO->UserParam	= mcmCancel;

	pgpbOK->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
	pgpbOK->AssignSound( "INTERF_NAGAL", CLICK_SOUND );

	pgpbNO->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
	pgpbNO->AssignSound( "INTERF_NAGAL", CLICK_SOUND );

	{ _str	a_Hint , a_Hint2 ;	
	 a_Hint  += "{FS}{C FF502515}";  a_Hint  += GetTextByID("#CHINT_EndOfTurn");	DYNHINT(pgpbOK, a_Hint.pchar() );
	 a_Hint2 += "{FS}{C FF502515}";  a_Hint2 += GetTextByID("#CHINT_Quit");			DYNHINT(pgpbNO, a_Hint2.pchar()); }

	
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// --------------------------------------------------------------------------------------------------------------
	// ---------------     Briefing  &  CHeroPersDesk   -------------------------------------------------------------
	// Смещение для элементов меню брифинга 0xFFF000
	// --------------------------------------------------------------------------------------------------------------

	CBriefing		a_Briefing( &a_dsMenu, 0xFFF000 );
	ROOT_SCENARIO += &a_Briefing;
	CHeroPersDesk	a_hpDesk( &a_dsMenu, &PORTRAIT, &HERO_MM );
	ROOT_SCENARIO += &a_hpDesk;

	// --------------------------------------------------------------------------------------------------------------
	// ---------------     Market   ---------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------------------------------

	CMarketDesk		a_Market( &a_dsMenu, a_inMX0 , a_inMY0 , a_inMX1 , a_inMY1 );
	ROOT_SCENARIO += &a_Market;


	// !!!SCROLLER INFO TEXT IN MAIN MENU (TEMP, WORK)!!!
	TextButton* a_ptbVScrTMP = a_dsMenu.addTextButton(NULL,0,40,"0",&WhiteFont,&WhiteFont,&WhiteFont,0);// u need it!
	a_ptbVScrTMP->Visible	 = false;
	ROOT_SCENARIO.m_ptbText  = a_ptbVScrTMP;
	// !!!SCROLLER INFO TEXT IN MAIN MENU (TEMP, WORK)!!!
	//TextButton* a_ptbCName = a_dsMenu.addTextButton(NULL, 698,112 ,"_empty_", &BigRedFont,&BigRedFont,&BigRedFont ,0); //temp 
	//GPPicture*  a_pgppFlag = a_dsMenu.addGPPicture(NULL, dx0+154 /*150*/ , dy0-1/*+0*/ ,a_lgpFlags.GPID,0); //temp

	// --------------------------------------------------------------------------------------------------------------
	// ---------------     ProcessDiplomacy     ---------------------------------------------------------------------
	// --------------------------------------------------------------------------------------------------------------
	// Смещение для элементов меню дипломатии 0xFF0000
	// --------------------------------------------------------------------------------------------------------------
	
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// создание окна дипломатии (2е окно + кнопки карты)
	// Размеры окна дипломатии
	int dx0  = 743,		dy0  = 112;					// координаты левого верхнего угла		
	int dx1  = 1000,	dy1  = 643;					// коорд. прав. нижн. угла
	int dtx0 = 743,		dty0 = 300;					// коорд. лев. верхн. позиции текста	

	int dxi = 110;

	// отрисовка дипл. договоров по сектору
	dtx0 = 691,  dty0 = 350 ;						// коорд. лев. верхн. позиции текста
	int dtHx1 = dtx0 +  57 ;
	int dtHx2 = dtx0 + 165 ;

	dtHx1 = dtx0 +  57 ; dtHx2 = dtx0 + 165 ;

	char a_chStateNames[5][256];
	strcpy(a_chStateNames[0],GetTextByID("#CDT_InWAR"));	strcpy(a_chStateNames[1],GetTextByID("#CDT_InPEACE"));
	strcpy(a_chStateNames[2],GetTextByID("#CDT_InNONA"));	strcpy(a_chStateNames[3],GetTextByID("#CDT_InPASS"));
	strcpy(a_chStateNames[4],GetTextByID("#CDT_InALLY"));

	TextButton* a_ptb  = NULL; TextButton* a_ptb2 = NULL;	
	a_ptb  = a_dsMenu.addTextButton(NULL, dtHx1 ,dty0,a_chStateNames[0],a_prlcfPgPas,a_prlcfPgPas,a_prlcfPgPas,1);
	a_ptb->AssignID(0xFF0100);		// --- Смещение   0xFF0100 ---			
	a_ptb->Visible = false;
	a_ptb  = a_dsMenu.addTextButton(NULL, dtHx2 ,dty0,a_chStateNames[1],a_prlcfPgPas,a_prlcfPgPas,a_prlcfPgPas,1);
	a_ptb->AssignID(0xFF0201);		// --- Смещение   0xFF0201 ---	
	a_ptb->Visible = false;
	a_ptb  = a_dsMenu.addTextButton(NULL, dtHx2 ,dty0,a_chStateNames[2],a_prlcfPgPas,a_prlcfPgPas,a_prlcfPgPas,1);
	a_ptb->AssignID(0xFF0202);		// --- Смещение   0xFF0202 ---	
	a_ptb->Visible = false;
	a_ptb  = a_dsMenu.addTextButton(NULL, dtHx2,dty0,a_chStateNames[3],a_prlcfPgPas,a_prlcfPgPas,a_prlcfPgPas,1);
	a_ptb->AssignID(0xFF0203);		// --- Смещение   0xFF0203 ---	
	a_ptb->Visible = false;
	a_ptb  = a_dsMenu.addTextButton(NULL, dtHx2 ,dty0,a_chStateNames[4],a_prlcfPgPas,a_prlcfPgPas,a_prlcfPgPas,1);
	a_ptb->AssignID(0xFF0204);		// --- Смещение   0xFF0204 ---	
	a_ptb->Visible = false;

	dxi += 10;
	dty0 = dty0 + 20;
	for (int i=0; i<5; i++) {
		a_ptb  = a_dsMenu.addTextButton(NULL,0,0, "_" , a_prlcfPgPas,a_prlcfPgPas,a_prlcfPgPas, 0);
		a_ptb2 = a_dsMenu.addTextButton(NULL,0,0, "_" , a_prlcfPgPas,a_prlcfPgPas,a_prlcfPgPas, 0); 
		
		a_ptb->AssignID(0xFF0101 + i);			// --- Смещение   0xFF0101 ---			
		a_ptb->Visible = false;
		a_ptb->x  = dtx0 ;
		a_ptb->y  = dty0 +i*20;			
		a_ptb->x1 = dtx0 + a_inPBBW;
		a_ptb->y1 = dty0 + (i+1)*20;	

		a_ptb2->AssignID(0xFF0205 + i);			// --- Смещение   0xFF0205 ---			
		a_ptb2->Visible = false;
		a_ptb2->x  = a_ptb->x + dxi ;
		a_ptb2->y  = a_ptb->y;
		a_ptb2->x1 = a_ptb->x1 + dxi ;
		a_ptb2->y1 = a_ptb->y1; 

		a_ptb->UserParam  = i;
		a_ptb2->UserParam = i;
	}
	CDiplMenuCountries a_WarsTexts ( &a_dsMenu, 6 , 0xFF0100 );  
	CDiplMenuCountries a_DealsTexts( &a_dsMenu, 9 , 0xFF0201 ); 

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// отрисовка кнопок действий дипл.
	LocalGP		a_lgpTextButton("Interf3\\TotalWarGraph\\dActions");

	int  dbx0 = 705 /*702*/,  dby0 = 491 /*499*/ ;					// коорд. лев. верхн. позиции кнопок
	a_inPBBH  = 27; //25
	int  a_inPBBW2 = dx1-dx0;
	
	const int dmbNUM = 6 /*5*/ ; 
	char  a_chButtName[dmbNUM][256];			//подписи к кнопкам дипл.действий
	strcpy(a_chButtName[0],GetTextByID("#CDB_Button1"));	strcpy(a_chButtName[1],GetTextByID("#CDB_Button2"));
	strcpy(a_chButtName[2],GetTextByID("#CDB_Button3"));	strcpy(a_chButtName[3],GetTextByID("#CDB_Button4"));
	strcpy(a_chButtName[4],GetTextByID("#CDB_Button5"));	strcpy(a_chButtName[5],GetTextByID("#CDB_Button6"));

	a_pgptbWM = NULL; 
	for (int i=0; i < dmbNUM ; i++) {
		a_pgptbWM = a_dsMenu.addGP_TextButton(NULL,0,0,a_chButtName[i],a_lgpTextButton.GPID,0,&RedFont,&OrangeFont);//NEED NEW FONTS 
		
		a_pgptbWM->AssignID(0xFF1000 + i);			// --- Смещение по карте 0xFF1000 ---		
		a_pgptbWM->Visible = false;
		a_pgptbWM->Sprite  = 1; //active
		a_pgptbWM->Sprite1 = 0; //passive
		a_pgptbWM->DisabledFont = &GrayFont ;//NEED NEW FONTS 		
		#ifdef DemoAI		//DEMO
			a_pgptbWM->Sprite  = 0; //active
			a_pgptbWM->ActiveFont = a_pgptbWM->PassiveFont = &GrayFont; //OrangeFont;
		#endif				//DEMO
		
		a_pgptbWM->x   = dbx0;						
		a_pgptbWM->y   = dby0 + (i+1)*(a_inPBBH); //+3 );	//- 1);//- 2 ) ; 
		a_pgptbWM->x1 += a_pgptbWM->x ; 		
		a_pgptbWM->y1 += a_pgptbWM->y ; 
		
		a_pgptbWM->FontDy		= -2; //-1 ; //
		a_pgptbWM->UserParam	= i; 
		a_pgptbWM->OnUserClick	= &DiplActionPressed;

		a_pgptbWM->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
		a_pgptbWM->AssignSound( "INTERF_CONTR", CLICK_SOUND );
	}

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	CDiplMenuButtons a_ActButt( &a_dsMenu, dmbNUM /*5*/, 0xFF1000 );	

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	
	// объявление дипл. инфы
	test_bmClassArr(&PLAYER_SAVE);//new
	CDiplomacyData	a_DiplData( &sdSecDat );
	test_bmClassArr(&PLAYER_SAVE);//new
	PLAYER_SAVE.SetDiplData( &a_DiplData );
	test_bmClassArr(&PLAYER_SAVE);//new

	a_DiplData.m_iPlayerId = gi_bmSAVE_Nation ;

	// объявление информационных сообщений
	//DialogsDesk* a_pDesk = NULL;			//a_dsMenu.AddDialogsDesk(a_inMX1,a_inMY0,80,480,"EmptyBorder");//desk для размещ.инф.сообщ. 
	//CDiplInfoMessBox	a_MessBox( &a_dsMenu, 0xFF1300 );
	//CDiplInfoButtons	a_InfoButtons( a_pDesk, a_pgppRul, 0xFF1200 );


	// закладка информашион			
	CDiplInfoPanel		a_Information( &a_dsMenu, 0xFFFF00 );	
	CDiplInfoMessages	a_InfoMessages( NULL, NULL, &a_DiplData, NULL, &a_Information, 0 );
	ROOT_SCENARIO += &a_InfoMessages;
	PLAYER_SAVE.SetMessages( &a_InfoMessages );									
	test_bmClassArr(&PLAYER_SAVE);//new

	a_Information.SetInfoMessages( &a_InfoMessages );


	//объявление меню дипломатии
	CDiplMenu a_DiplMenu(&a_dsMenu, pgpbOK, pgpbNO, &sdSecDat, &a_DiplData,
						 NULL , &a_WarsTexts, &a_DealsTexts,
						 &a_ActButt, NULL , &a_InfoMessages, NULL , NULL , 
						 &a_smSectMenu );
	ROOT_SCENARIO += &a_DiplMenu;
	a_DiplData.SetDiplMenu( &a_DiplMenu );


	//// -------------- End Of ProcessDiplomacy ---------------------------------------------------------------------
	//// ------------------------------------------------------------------------------------------------------------


	MissionInitData.SetARMYData( &Rank_Type_Country );

	ROOT_SCENARIO += &a_dsMenu;
	ROOT_SCENARIO += &a_MapPic;
	ROOT_SCENARIO += &sdSecDat;
	ROOT_SCENARIO += &a_smSectMenu;
	ROOT_SCENARIO += &HERO_MM;
	ROOT_SCENARIO += &PORTRAIT;
	ROOT_SCENARIO += &HERO_PERSONAL;	
	
	//ROOT_SCENARIO += &a_hpDesk ;
	//ROOT_SCENARIO += &a_DiplMenu;
	//ROOT_SCENARIO += &a_Briefing;
	//ROOT_SCENARIO += &a_Quests;
	//ROOT_SCENARIO += &a_ResPanel;
	//ROOT_SCENARIO += &a_InfoMessages;
	//ROOT_SCENARIO += &a_Market; 

	for(int i=0; i < MAX_PLAYERS ; i++) { ROOT_SCENARIO += PLAYERS[i];   a_DiplMenu += PLAYERS[i]; }

	//Set main player, hero visible. 
	pAtackerHero = &(PLAYERS[sp]->m_phrHero[0]);
	if ( PLAYERS[sp]->m_inHeroNum > 0 ){
		ROOT_SCENARIO.m_Scenario.m_inLastHeroClick = PLAYERS[sp]->m_phrHero[0].GetHeroID() + PLAYERS[sp]->m_phrHero[0].GetPlayerID()*100;
		ROOT_SCENARIO.m_pHmm->SetAll( PLAYERS[sp]->m_phrHero[0].GetName(),
									  PLAYERS[sp]->m_phrHero[0].GetRank(),
									  PLAYERS[sp]->m_phrHero[0].GetStat(),
									  PLAYERS[sp]->m_phrHero[0].GetPlayerID()+1, &(PLAYERS[sp]->m_phrHero[0]) );
	}
	HERO_PERSONAL.setPlayerID(sp);
    ROOT_SCENARIO.UPDATE_SCENARIO();

	//Выставить типы хинтов для менюх.
	a_dsMenu.SetHintStyle(true,true,false,0,3,3, 1 , 1 ,450,350, &SmallBlackFont, 0xFFFFFFFF, 0xFFFFFFFF );
	//a_dsMenu.SetHintStyle( true, true, true, 30, 0, 0, 25, 25, 250, 300, &SmallBlackFont, 0x55000099, 0xFFFFFFFF );


	//////////////		FOR VIC!!! TEMP!!!  ---------------------------------------------------------------------------------
	//////
	//if( pSAVE->m_inCurTurn == 0 ) {//add if new player
	//// добавление контракта (+ сообщениe на desk сообщений) 
	//a_DiplMenu.AddContract( 4, 4, 0, 5 );	
	//a_DiplMenu.AddContract( 2, 3, 0, 5 );	
	//a_DiplMenu.AddContract( 0, 2, 0, 5 );	
	//a_DiplMenu.AddContract( 3, 3, 0, 5 );	
	//a_DiplMenu.AddContract( 0, 1, 0, 5 );	
	//a_DiplMenu.AddContract( 4, 5, 0, 5 );	
	//
	//a_DiplMenu.AddContract( 1, 2, 0, 5 );
	//
	//a_DiplMenu.AddContract( 4, 3, 0, 5 );	
	//
	//a_DiplMenu.AddContract( 4, 1, 2, 5 ); //ALLIANCE	
	////
	////// добавление сообщения на desk сообщений
	//a_DiplMenu.AddMessage( 10, 2, 0, "button" , "Message Head" , "Message Box Text" ); 
	//Enumerator* MessHead_ENUM = ENUM.Get("MessHead_ENUM");
	//a_DiplMenu.AddMessage( 20, 2, 0, "info" , MessHead_ENUM->Get( 5 ) , "Information." ); 
	////!!!
	//PLAYERS[0]->AddHero( sdSecDat.GetSecIDbyOwner( 0 ), PLAYERS[0]->GetNewHeroID() );
	////PLAYERS[0]->AddHero( sdSecDat.GetSecIDbyOwner(0) , 0 );		
	//}
	//////
	//////////////		FOR VIC!!! TEMP!!!  ---------------------------------------------------------------------------------

	int MapDxTMP, MapDyTMP, iLastLMCSecID, heroIdTMP, iActivMenu ;

	PLAYER_SAVE.SetMenuState( &MapDxTMP, &MapDyTMP, &iLastLMCSecID, &heroIdTMP , &iActivMenu, 
		&ROOT_SCENARIO.m_inCurActiveAI , &ROOT_SCENARIO.m_inCurHeroMove , ROOT_SCENARIO.m_inCurTurn ); //PLAYER_SAVE.SetMenuState( &MapDxTMP, &MapDyTMP, &ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID, &heroIdTMP , &ROOT_SCENARIO.m_Scenario.m_inActivMenu);
	test_bmClassArr(&PLAYER_SAVE);//new


	for(int i=0; i<MAX_PLAYERS ; i++) 
		for(int h=0; h<PLAYERS[i]->m_inHeroNum ; h++)  PLAYERS[i]->m_phrHero[h].SetRank( 5 );

	PLAYER_SAVE.SetCountry( PLAYERS );		// считывание PLAYER_SAVE-a
	test_bmClassArr(&PLAYER_SAVE);//new

	for(int i=0; i < MAX_PLAYERS ; i++)		// расставить героев по секторам после считывания PLAYER_SAVE-a
		for(int h=0; h < PLAYERS[i]->m_inHeroNum ; h++) {
			PLAYERS[i]->m_phrHero[h].SetSector( &sdSecDat, PLAYERS[i]->m_phrHero[h].getSectorID() );
		}

	if( pSAVE->COUNTRY.GetAmountOfElements() <= 0 ) {
		//TEMP
		//PLAYERS[0]->AddHero( sdSecDat.GetSecIDbyOwner( 0 ), PLAYERS[0]->GetNewHeroID() );//0
		//TEMP
		for(int i=0 ; i<MAX_PLAYERS ; i++) 
				for(int h=0; h<PLAYERS[i]->m_inHeroNum ; h++) {
					PLAYERS[i]->m_phrHero[h].UpdateRankWithExperience();
					PLAYERS[i]->m_phrHero[h].SetDefaultARMY();
				}
	}
	//Set main player, hero visible.
	pAtackerHero = &(PLAYERS[sp]->m_phrHero[0]);
	if ( PLAYERS[sp]->m_inHeroNum > 0 ){
		ROOT_SCENARIO.m_Scenario.m_inLastHeroClick = PLAYERS[sp]->m_phrHero[0].GetHeroID() + PLAYERS[sp]->m_phrHero[0].GetPlayerID()*100;
		ROOT_SCENARIO.m_pHmm->SetAll( PLAYERS[sp]->m_phrHero[0].GetName(),
									  PLAYERS[sp]->m_phrHero[0].GetRank(),
									  PLAYERS[sp]->m_phrHero[0].GetStat(),
									  PLAYERS[sp]->m_phrHero[0].GetPlayerID()+1, &(PLAYERS[sp]->m_phrHero[0]) );
	}
	HERO_PERSONAL.setPlayerID(sp);


	//-----------------------------------------------------------------------------------------------------
	//Стрелочки движения отрядов и героев.
	//LocalGP		a_lgpArrow("Interf3\\TotalWarGraph\\lva_Arrow");
	LocalGP		a_lgpNewArrow("Interf3\\TotalWarGraph\\lva_NewArrow");
	a_dsMenu.addClipper(a_inMX0,a_inMY0,a_inMX1,a_inMY1);
	//sdSecDat.AddArrowPictures(&a_lgpArrow,0);
	sdSecDat.AddNewArrowPictures(&a_lgpNewArrow,86);
	a_dsMenu.addClipper(0,0,1024,768);
	//-----------------------------------------------------------------------------------------------------


    //-----------------------------------------------------------------------------------------------------
	// инфа по сектору
	CDiplMenuInfo a_InfoTexts( &a_dsMenu, &sdSecDat, 0 , 0xFF0000 );
	a_DiplMenu     += &a_InfoTexts ;
	a_InfoMessages += &a_InfoTexts ;

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	//-----------------------------------------------------------------------------------------------------
	// отрисовка кнопок над картой
	CDiplMenuButOnMap	a_MapButt( &a_dsMenu, 5, 0, ROOT_SCENARIO.m_Scenario.m_inMapButtonPressed, 0xFF1100 );
	a_DiplMenu += &a_MapButt ;

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// --------------------------------------------------------------------------------------------------------------
	// ---------------     Big Map Help     -------------------------------------------------------------------------
	CBigMapHelp		a_Help( &a_dsMenu );
	ROOT_SCENARIO += &a_Help; 
	PLAYER_SAVE.SetHelpState( &a_Help );

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// --------------------------------------------------------------------------------------------------------------
	// ---------------     Loading Big Map Quests (CBigMapQuest)   --------------------------------------------------
	CBigMapQuest	a_Quests( pSAVE->m_inCurTurn );
	ROOT_SCENARIO += &a_Quests;
	PLAYER_SAVE.SetQuestsData( &a_Quests ) ;

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	// --------------------------------------------------------------------------------------------------------------


	//// ------------------------------------------------------------------------------------------------------------
	//  ----------------------------   PREPARE GAME   ---------------------------------------------------------------
	//// ------------------------------------------------------------------------------------------------------------
	//Все герои, всех игроков могут ходить.
	//for(int i=0; i < MAX_PLAYERS ; i++) { PLAYERS[i]->SetHeroesFuel(1); } //TEMP!!!

	//test_bmClassArr(&PLAYER_SAVE);//new
	//if(Profiles)  test_bmClassArr( Profiles );//new

	//new
	ROOT_SCENARIO.PushHeroOnTop2(); //very new!!!
	ROOT_SCENARIO.m_Scenario.m_inAttackedSector  = -1 ; //very new!!!	//last add
	//new

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	MissionInitData.SetSector_ID( -1 ); 
	MissionInitData.setAttackerDirection_ID( 0 ); 
	MissionInitData.setAttackerNation( 0 ); 
	MissionInitData.setDefenderNation( 1  ); 
	MissionInitData.setAttackerRank_ID( 0 ); 
	MissionInitData.setDefenderRank_ID( 0 );
	PLAYER_SAVE.SetMissionInitData( &MissionInitData ) ;
	
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new


	if( MissionInitData.getSector_ID() >= 0 && MissionInitData.getSector_ID() < ROOT_SCENARIO.m_pSect->GetSectorsNum() )
		ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID = MissionInitData.getSector_ID();
	ChangeActiveSector( ROOT_SCENARIO.m_pSect->m_pssdDT[ ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID ]->m_psdSec );

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	

	ROOT_SCENARIO.UPDATE_SCENARIO();
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	ROOT_SCENARIO.PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , pSAVE->m_iDifficulty );
	
	//
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	ProcessMessages();
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	a_dsMenu.ProcessDialogs();
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	StdKeys();								//Обрaботка Enter - mcmOK, и ESC - mcmCancel
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	a_dsMenu.RefreshView();// INTEFFACE NEED!!!
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	//MissionInitData.SetSector_ID(-1); MissionInitData.setHeroPresent(false); MissionInitData.ClearAttacker(); MissionInitData.ClearDefender();

	// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//#ifdef TEST_LVA
	////if(&(PLAYERS[sp]->m_phrHero[0])) {
	////	CE_01.ClearAll();
	////	CE_01.CreateFromClass( &DS_01, 500, 50, 800, 300, &(PLAYERS[sp]->m_phrHero[0]), CED_MEMBERS|CED_TREE );
	////}
	////CE_02.ClearAll();
	////CE_02.CreateFromClass( &DS_02, 50,50,400,600, &MissionInitData, CED_MEMBERS|CED_TREE );
	//CE_SEC_SD.ClearAll(); //TEMP!!!
	//CE_SEC_SD.CreateFromClass( &DS_SEC_SD, 50, 50, 450, 150, sdSecDat.m_pssdDT[0], CED_MEMBERS|CED_TREE ); //TEMP!!!
	//CE_SEC_DD.ClearAll(); //TEMP!!!
	//CE_SEC_DD.CreateFromClass( &DS_SEC_DD, 50, 200, 450, 250, sdSecDat.m_psddDT[0], CED_MEMBERS|CED_TREE ); //TEMP!!!
//#endif
	// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
	
	bool  RESTARTED_MISSION = false;
	if( pSAVE->m_MIDataSector_ID >= 0 && pSAVE->m_MIDataPlayerStat == 1 )
		RESTARTED_MISSION = true;

	// ----------------------------   PREPARE GAME   ----------------------------------------------------------------
	//// ------------------------------------------------------------------------------------------------------------
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	if( ROOT_SCENARIO.m_inCurTurn == 0 ) SaveBigMapDataXML(); //new //last add!!! // for SaveStatistics

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	//
	if( ROOT_SCENARIO.m_pPlayer[sp]->m_phrHero )
		ROOT_SCENARIO.m_pPlayer[sp]->m_phrHero->PrepareBlink();

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles) {
		Profiles->Init();
		Profiles->LoadXML();
		test_bmClassArr( Profiles );//new
	}
	//
	
BEGIN:
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new


#ifdef DemoVer4Journals	//DEMO
	int  ALLY = 4, PACT = 2;
	a_DiplMenu.m_pDiplData->AddContract( PACT, sp , 0 , 1000 );
	a_DiplMenu.m_pDiplData->AddContract( PACT, sp , 3 , 1000 );
	a_DiplMenu.m_pDiplData->AddContract( PACT, sp , 4 , 1000 );
	a_DiplMenu.m_pDiplData->AddContract( PACT, sp , 5 , 1000 );

	a_DiplMenu.m_pDiplData->AddContract( PACT,  2 , 0 , 1000 );
	a_DiplMenu.m_pDiplData->AddContract( PACT,  2 , 3 , 1000 );
	a_DiplMenu.m_pDiplData->AddContract( PACT,  2 , 4 , 1000 );
	a_DiplMenu.m_pDiplData->AddContract( PACT,  2 , 5 , 1000 );	//a_DiplMenu.m_pDiplData->AddContract( ALLY , sp , 3 , 1000 );	//a_DiplMenu.m_pDiplData->AddContract( PACT , sp , 4 , 1000 );	//a_DiplMenu.m_pDiplData->AddContract( PACT , sp , 5 , 1000 );
#endif					//DEMO


	//
	ROOT_SCENARIO.m_EnabledMapClick = true;
	//sdSecDat.SetEnable(bool state)
	//sdSecDat.SetEnable( true );
	//


	ROOT_SCENARIO.m_Scenario.m_bPlayerEnable	 = true ;
	ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled  = false ;	//ROOT_SCENARIO.m_Scenario.m_inAttackedSector  = -1 ;

	ProcessMissionForPlayer(campainID, false );
	test_bmClassArr(&PLAYER_SAVE);//new

	if( EndOfTurn ) {
		//
		test_bmClassArr(&PLAYER_SAVE);//new
		//
		int  newSecID = ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID;

		if( PLAYERS[ sp ]->m_phrHero && PLAYERS[ sp ]->m_inHeroNum != 0 ) { // выбрать сектор героя игрока (0-го)
			newSecID = PLAYERS[ sp ]->m_phrHero->getSectorID() ;
			test_bmClassArr(&PLAYER_SAVE);//new
			//int SecID = PLAYERS[ sp ]->m_phrHero->getSectorID(); //CSectData::m_LastLMCSecID = SecID; //ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID = SecID;
		}
		//ChangeActiveSector( ROOT_SCENARIO.m_pSect->m_pssdDT[ ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID ]->m_psdSec ); 
		if( newSecID < 0 || newSecID >= sdSecDat.GetSectorsNum() )  newSecID = START_SECTOR_ID;
		test_bmClassArr(&PLAYER_SAVE);//new
		ChangeActiveSector( ROOT_SCENARIO.m_pSect->m_pssdDT[ newSecID ]->m_psdSec ); 
		test_bmClassArr(&PLAYER_SAVE);//new

		// установка сектора игрока по центру экрана	(сектор героя игрока (0-го))
		int  secID = ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID ;
		int  secX  = 0 ,  secY  = 0 ;												
		ROOT_SCENARIO.m_pSect->GetSecSityXY( secID , secX , secY );
		test_bmClassArr(&PLAYER_SAVE);//new

		int  dMX = a_inMW / 2  - secX ;			
		int  dMY = a_inMH / 2  - secY ;
		a_MapPic.SetPosition( 1000 , 1000 );
		test_bmClassArr(&PLAYER_SAVE);//new
		a_MapPic.SetPosition( dMX , dMY );	
		test_bmClassArr(&PLAYER_SAVE);//new
	}
	test_bmClassArr(&PLAYER_SAVE);//new

	// хинты по секторам
	sdSecDat.CreateHint(); 	
	test_bmClassArr(&PLAYER_SAVE);//new
	// хинты по героям
	for(int i=0 ; i < MAX_PLAYERS ; i++) {
		for(int h=0; h < PLAYERS[i]->m_inHeroNum ; h++) {
			PLAYERS[i]->m_phrHero[h].CreateHint(); 
			test_bmClassArr(&PLAYER_SAVE);//new
		}
	}

	// обновление меню
	a_pgppBlackScreen->Visible = a_pgppBlackScreen->Enabled = false ;
	ChangeActiveButton(2,3,a_prlcfPgAct,a_prlcfPgPas,&a_dsMenu); 
	test_bmClassArr(&PLAYER_SAVE);//new
	ROOT_SCENARIO.UPDATE_SCENARIO();
	test_bmClassArr(&PLAYER_SAVE);//new
	ROOT_SCENARIO.PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , pSAVE->m_iDifficulty );
	test_bmClassArr(&PLAYER_SAVE);//new
	ProcessMessages();
	test_bmClassArr(&PLAYER_SAVE);//new
	a_dsMenu.ProcessDialogs();		
	test_bmClassArr(&PLAYER_SAVE);//new
	StdKeys();								//Обрaботка Enter - mcmOK, и ESC - mcmCancel
	test_bmClassArr(&PLAYER_SAVE);//new
	a_dsMenu.RefreshView();// INTEFFACE NEED!!!
	test_bmClassArr(&PLAYER_SAVE);//new


	// проверка оставшихся стран
	if( ROOT_SCENARIO.m_bEndGame )								On_BigMapReturn(true); //return true; 
	if( ROOT_SCENARIO.CHECK_DEFEAT( gi_bmSAVE_Nation ) )		On_BigMapReturn(true); //return true; // VICTORY-LOOSE!!!
	test_bmClassArr(&PLAYER_SAVE);//new
	if( !ROOT_SCENARIO.m_pPlayer[gi_bmSAVE_Nation]->m_Alive )	On_BigMapReturn(true); //return true; // LOOSE!!!
	// выдаем игроку текущее хелп-сообщение (только одно за ход)
	if( MissionInitData.getSector_ID() < 0 )	{ a_Help.PROCESS_HELPSYSTEM( campainID ); }
	test_bmClassArr(&PLAYER_SAVE);//new
	

	//***********************TEST**********************
	/*MissionInitData.setAttackerDirection_ID( 0 );	MissionInitData.setAttackerNation( 0 );	MissionInitData.setDefenderNation( 1  );
	MissionInitData.SetSector_ID( -1 );	MissionInitData.setAttackerRank_ID( 0 );	MissionInitData.setDefenderRank_ID( 0 );*/
	// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
	//if(&(PLAYERS[sp]->m_phrHero[0])) {
	//	CE_01.ClearAll();
	//	CE_01.CreateFromClass( &DS_01, 500, 50, 800, 300, &(PLAYERS[sp]->m_phrHero[0]), CED_MEMBERS|CED_TREE );
	//}
	//CE_02.ClearAll();
	//CE_02.CreateFromClass( &DS_02, 50,50,400,600, &MissionInitData, CED_MEMBERS|CED_TREE );
	//CE_SEC_SD.ClearAll();
	//CE_SEC_SD.CreateFromClass( &DS_SEC_SD, 50, 50, 450, 150, sdSecDat.m_pssdDT[0], CED_MEMBERS|CED_TREE );
	//CE_SEC_DD.ClearAll();
	//CE_SEC_DD.CreateFromClass( &DS_SEC_DD, 50, 200, 450, 250, sdSecDat.m_psddDT[0], CED_MEMBERS|CED_TREE );
	// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
	//*************************************************
	
 
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new

	UnPress();					//Global
	KeyPressed = LastKey = 0;   //Global

	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	//test_class(&a_dsMenu);

	ItemChoose	= - 1;
	do
	{
		sp  = a_DiplData.m_iPlayerId = HERO_PERSONAL.m_iHumanNat = gi_bmSAVE_Nation;

		ProcessMessages();
		a_dsMenu.ProcessDialogs();
		ROOT_SCENARIO.CheckHotKeys();
		StdKeys();								//Обрaботка Enter - mcmOK, и ESC - mcmCancel

		if( ROOT_SCENARIO.m_bEndGame )	On_BigMapReturn(true); //return true; // VICTORY-LOOSE!!! //VERY NEW! last add

//#ifdef TEST_LVA
//		////***********TEST************
//		////-------------
//		if( ROOT_SCENARIO.m_bSaveLogs ) {
//			//// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//			////if (KeyPressed&&LastKey==0x56) {
//			////	LastKey = 0;
//			////	KeyPressed = false;
//			////	bCE_01_SHOW = !bCE_01_SHOW;			}
//			////if (bCE_01_SHOW) {
//			////	CE_01.Process();
//			////	DS_01.ProcessDialogs();				}
//			//////------------
//			////if (KeyPressed&&LastKey==0x41) {
//			////	LastKey = 0;
//			////	KeyPressed = false;
//			////	bCE_02_SHOW = !bCE_02_SHOW;			}
//			////if (bCE_02_SHOW) {
//			////	if (g_bEDITOR_CHENGE) { g_bEDITOR_CHENGE=false; CE_02.ExpInfChanged=true; }
//			////	CE_02.Process();
//			////	DS_02.ProcessDialogs();				}
//			////------------
//			if( KeyPressed&&LastKey==0x42 ) {
//				LastKey = 0;
//				KeyPressed = false;
//				bCE_SEC_SHOW = !bCE_SEC_SHOW;
//			}
//			if( bCE_SEC_SHOW ) {
//				CE_SEC_SD.Process();
//				CE_SEC_DD.Process();
//				DS_SEC_SD.ProcessDialogs();
//				DS_SEC_DD.ProcessDialogs();
//			}
//			//// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//		}
//		////***********TEST************
//#endif

		////*****temp*****//
		//// !!!SCROLLER INFO TEXT IN MAIN MENU (TEMP, WORK)!!!
		////sprintf( s2,"%d", a_Information.m_pDesk->VScroller->SPos );		//strcpy ( a_ptbVScrTMP->Message, s2 );
		//// !!!SCROLLER INFO TEXT IN MAIN MENU (TEMP, WORK)!!!
		////*****temp*****//


		ChangeActiveButton(2,3,a_prlcfPgAct,a_prlcfPgPas,&a_dsMenu); 

		ROOT_SCENARIO.PROCESS(g_dXM,g_dYM,g_WM,g_HM, sp, pSAVE->m_iDifficulty );

		ROOT_SCENARIO.m_Scenario.m_bHelpVisible = true; // help

		a_dsMenu.RefreshView();

		test_bmClassArr(&PLAYER_SAVE);//new

		if(Profiles)  test_bmClassArr( Profiles );//new
		//test_class(&a_dsMenu);

		if( MissionInitData.getSector_ID() >= 0 || RESTARTED_MISSION )  { ItemChoose = mcmOk; } //!very last!

		if( ItemChoose==mcmCancel && !ROOT_SCENARIO.m_bEndGame &&
			ShowBigMapGlobalMessage(2, 42 ,13,GetTextByID("#CWM_Quit"),ROOT_SCENARIO.m_Scenario.m_inActivMenu) == 1 ) { ItemChoose = mcmCancel ; }		
	}
	while( ItemChoose!=mcmOk && ItemChoose!=mcmCancel );

	ClipCursor(NULL);


	//
	ROOT_SCENARIO.m_EnabledMapClick = false;
	//sdSecDat.SetEnable( false );
	//

	//test_class2(&a_dsMenu);//new
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new


	if( ROOT_SCENARIO.m_bSaveLogs )  a_DiplData.SaveXML();
	if( ROOT_SCENARIO.m_bSaveLogs )  a_InfoMessages.SaveXML();


	EndOfTurn = false;

	if( ItemChoose==mcmOk )
	{
		if( MissionInitData.getSector_ID() < 0 ) { EndOfTurn = true ; }
		int pic = 36; if( sp == 1 ) pic = 37; if( sp == 2 ) pic = 38; if( sp == 3 ) pic = 39; if( sp == 4 ) pic = 19; if( sp == 5 ) pic = 41;
		if( EndOfTurn && PLAYERS[sp]->GetPlayerMoves() > 0 &&  !RESTARTED_MISSION  && //!very last!
			ShowBigMapGlobalMessage(2, pic, 12, GetTextByID("#CWM_EndOfTurn"),ROOT_SCENARIO.m_Scenario.m_inActivMenu) != 1 ) { EndOfTurn = false; }

		//// && сохранение результатов 
		//SaveBigMapDataXML(); 

		ProcessMissionForPlayer(campainID , true );

		// проверка оставшихся стран
		if( ROOT_SCENARIO.CHECK_DEFEAT( gi_bmSAVE_Nation ) )		On_BigMapReturn(true); //return true; // VICTORY-LOOSE!!!
		if( !ROOT_SCENARIO.m_pPlayer[gi_bmSAVE_Nation]->m_Alive )	On_BigMapReturn(true); //return true; // LOOSE!!!

		//test_class(&a_dsMenu);
		if( EndOfTurn || RESTARTED_MISSION ) //!very last!
		{
			//a_DiplMenu.ClearData();
			if( !RESTARTED_MISSION )  a_DiplMenu.ClearData(); //!very last!
			RESTARTED_MISSION = false; //!very last!

			//*******************ВЫБРАН КОНЕЦ ХОДА*********************
			ROOT_SCENARIO.m_Scenario.m_bMenuIsChange = true;
			{
				ROOT_SCENARIO.m_Scenario.m_inActivMenu = 0 ;
				ROOT_SCENARIO.m_Scenario.m_bBriefingVisible		= false;
				//ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled		= false;
				
				ROOT_SCENARIO.m_Scenario.m_bDiplVisible			= false;
				//ROOT_SCENARIO.m_Scenario.m_bInfoVisible			= true;

				ROOT_SCENARIO.m_Scenario.m_bMessagesVisible		= false;
				ROOT_SCENARIO.m_Scenario.m_bMarketVisible		= false;

				ROOT_SCENARIO.m_Scenario.m_bMapEnable			= true;
				ROOT_SCENARIO.m_Scenario.m_bMapMouseMove		= true;
				ROOT_SCENARIO.m_Scenario.m_bMapVisible			= true;
					
				ROOT_SCENARIO.m_Scenario.m_bPlayerEnable		= true;
				ROOT_SCENARIO.m_Scenario.m_bPlayerVisible		= true;
				//ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap			= true;
					
				ROOT_SCENARIO.m_Scenario.m_bSectEnable			= true;
				ROOT_SCENARIO.m_Scenario.m_bSectMouseMove		= true;
				ROOT_SCENARIO.m_Scenario.m_bSectSetPlayerColor	= true;
				ROOT_SCENARIO.m_Scenario.m_bSectVisible			= true;
				ROOT_SCENARIO.m_Scenario.m_bSectMenuVisible		= true;

				ROOT_SCENARIO.m_Scenario.m_bPERSONAL_HERO_MENU	= false;
			}
			ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap = ROOT_SCENARIO.GetHeroInSect(ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID, 0xFFFF) && ROOT_SCENARIO.m_Scenario.m_inActivMenu==0; 

			ROOT_SCENARIO.UPDATE_SCENARIO();

			ChangeActiveButton(2,3,a_prlcfPgAct,a_prlcfPgPas,&a_dsMenu); 
			a_dsMenu.RefreshView();
		
			SetBigMapMenuEnabled( false );	//GREY [26.03.2004]

			//ОБРАБОТКА ХОДОВ ИГРОКА и КОМПЬЮТЕРНЫХ ИГРОКОВ.
			if(ROOT_SCENARIO.m_inCurActiveAI >= MAX_PLAYERS )	ROOT_SCENARIO.m_inCurActiveAI = 0;
			if(ROOT_SCENARIO.m_inCurActiveAI <  0 )				ROOT_SCENARIO.m_inCurActiveAI = 0;

			// && сохранение результатов 
			SaveBigMapDataXML();
			do { 
				#ifdef DemoAI			//DEMO
					ProcessDemoAI( sp );  break;
				#endif					//DEMO
				ProcessMissionForPlayer(campainID , true );

				ROOT_SCENARIO.AI_PROCESS( ROOT_SCENARIO.m_inCurActiveAI , sp );// ХОД AI(m_inCurActiveAI)	
				// проверка оставшихся стран
				if( ROOT_SCENARIO.CHECK_DEFEAT( sp ) )			On_BigMapReturn(true); //return true; // VICTORY-LOOSE!!!
				if( !ROOT_SCENARIO.m_pPlayer[sp]->m_Alive )		On_BigMapReturn(true); //return true; // LOOSE!!!

				if( MissionInitData.getSector_ID() < 0 ) ROOT_SCENARIO.m_inCurActiveAI++ ;
			} 
			while( ROOT_SCENARIO.m_inCurActiveAI < MAX_PLAYERS /*&& MissionInitData.getSector_ID() < 0*/ );


			//if( PLAYERS[sp]->m_phrHero && PLAYERS[sp]->m_phrHero->m_Alive ) {
			//	ROOT_SCENARIO.ShowInGameSector( PLAYERS[sp]->m_phrHero->getSectorID() , 0.01 ) ; 
			//}

			if( EndOfTurn ) 
			{
				////*********************************************************
				//НАЧИНАЕМ НОВЫЙ ХОД.
				for(int i=0; i < MAX_PLAYERS ; i++)  { PLAYERS[i]->SetHeroesFuel( 1 ); }
				for(int i=0; i < PLAYERS[ sp ]->m_inHeroNum; i++) {	// bad weather!
					int  SecId = PLAYERS[ sp ]->m_phrHero[i].getSectorID() ;
					if( ROOT_SCENARIO.m_pSect->m_pssdDT[ SecId ]->m_iSabotageID%10000000 / 1000000 > 0 ) {
						PLAYERS[ sp ]->m_phrHero[i].SetFuel( 0 );
						char  text[256];
						sprintf(text,GetTextByID("#CWM_Bad_Weather2"), PLAYERS[sp]->m_phrHero[i].GetName());
						ShowBigMapGlobalMessage( 1 , 33 , 11 , text, 0);
					}
				} // bad weather!


				ROOT_SCENARIO.UpdateResources();
				// Give Quests for PLAYER gi_bmSAVE_Nation (sp)
				a_Quests.ProcessQuests( &ROOT_SCENARIO , gi_bmSAVE_Nation , true );

				ROOT_SCENARIO.m_inCurTurn++ ;
				a_smSectMenu.m_inCurSectID = -1 ;
				a_DiplMenu.PrepareNextRound( ROOT_SCENARIO.m_inCurTurn );

				//new
				if( ROOT_SCENARIO.m_pPlayer[sp]->m_phrHero )
					ROOT_SCENARIO.m_pPlayer[sp]->m_phrHero->PrepareBlink();
				//new

				SetBigMapMenuEnabled( true );
				//test_class(&a_dsMenu);

				// && сохранение результатов 
				SaveBigMapDataXML(); 
			}
		}

		ChangeActiveSector( ROOT_SCENARIO.m_pSect->m_pssdDT[ ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID ]->m_psdSec );

		//test_class(&a_dsMenu);
		test_bmClassArr(&PLAYER_SAVE);//new
		if(Profiles)  test_bmClassArr( Profiles );//new

		goto BEGIN;

	}	//if (ItemChoose==mcmOk)


	// !!!!!TEEEEEEEEEEEEEEST!!!!!!
	//a_Briefing.~CBriefing() ;
	//a_hpDesk.~CHeroPersDesk() ;
	//a_InfoTexts.~CDiplMenuInfo() ;
	//a_WarsTexts.~CDiplMenuCountries() ;  
	//a_DealsTexts.~CDiplMenuCountries() ;
	//a_ActButt.~CDiplMenuButtons() ;
	//a_MapButt.~CDiplMenuButOnMap() ;
	//a_DiplData.~CDiplomacyData() ;
	//a_MessBox.~CDiplInfoMessBox() ;
	//a_InfoButtons.~CDiplInfoButtons() ;
	//a_Information.~CDiplInfoPanel() ;	
	//a_InfoMessages.~CDiplInfoMessages() ;
	//test_class(&a_dsMenu);
	//// end dipl data
	//sdSecDat.~CSectData() ;
	//test_class(&a_dsMenu);
	// !!!!!TEEEEEEEEEEEEEEST!!!!!!


	// && сохранение результатов 
	SaveBigMapDataXML(); 
		

	//test_class(&a_dsMenu);

	//
	//test_class2( &a_dsMenu);//new
	test_bmClassArr(&PLAYER_SAVE);//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	//test_class2( &a_dsMenu );//new
	//BM_SAVE = NULL;
	//

	On_BigMapReturn(true); //return	true;
}

//// ----------------------------------------------------------------------------------------------------------
//// ----------------------------------------------------------------------------------------------------------
//// -----   END OF ProcessBigMap   ---------------------------------------------------------------------------
//// ----------------------------------------------------------------------------------------------------------
//// ----------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void	CREATE_COUNTRY_ENUM(CArmyLimit* pAL) {
	g_pALL_UNIT_TYPE = ENUM.Get( "ALL_TYPE" );
	if (!g_pALL_UNIT_TYPE->GetAmount()) {
		for ( int mon=0; mon<NATIONS[0].NMon; mon++ ) {
			g_pALL_UNIT_TYPE->Add( NATIONS[0].Mon[mon]->MonsterID, mon );
		}
	}

	for(int rank=0; rank < MAX_RANKS ; rank++)
		for(int cont=0; cont < MAX_COUNTRYS ; cont++)
			for(int unit=0; unit < pAL->GetMaxDiffSqd(rank,cont) ; unit++) {
				pAL->SetNSqdNID( rank, cont, unit, g_pALL_UNIT_TYPE->Get( pAL->GetNSqdID(rank,cont,unit) ) );
			}
}
//------------------------------------------------------------------------------------
bool	ChangeActiveSector(SimpleDialog* psdSec)
{
//#ifdef TEST_LVA				// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//	CE_SEC_SD.ClearAll();
//	CE_SEC_SD.CreateFromClass( &DS_SEC_SD, 50, 50, 450, 150, ROOT_SCENARIO.m_pSect->m_pssdDT[psdSec->UserParam], CED_MEMBERS|CED_TREE );
//	CE_SEC_DD.ClearAll();
//	CE_SEC_DD.CreateFromClass( &DS_SEC_DD, 50, 200, 450, 250, ROOT_SCENARIO.m_pSect->m_psddDT[psdSec->UserParam], CED_MEMBERS|CED_TREE );
//#endif						// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	


	CSectData::m_LastLMCSecID = psdSec->UserParam;
	ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID = psdSec->UserParam;


	int LastSecID = psdSec->UserParam;
	CHero*	pHero = ROOT_SCENARIO.GetHeroInSect( LastSecID, 0xFFFF ); 
	if(pHero)	{ ON_HERO_R_CLICK_VIRTUAL( pHero ); } //синхронизировать сектор с героем, если есть
	else		{ ROOT_SCENARIO.SetHeroMiniMenuVisible(false); }

	ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap = pHero && ROOT_SCENARIO.m_Scenario.m_inActivMenu==0;
	return true;
}
//------------------------------------------------------------------------------------
bool	ChangeActiveMenu(SimpleDialog* pSD)
{
	if( !ROOT_SCENARIO.m_EnabledMapClick  )					{ return false; }

	if( !pSD )														{ return false; }

	#ifdef DemoAI	//DEMO
		if( pSD->UserParam==3 ||  pSD->UserParam==4 )				{ return true; }
	#endif			//DEMO

	if( pSD->UserParam == ROOT_SCENARIO.m_Scenario.m_inActivMenu )	{ return true; }

	//if (pSD->UserParam==2&&ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled==false)			{ return true; }
	//if ( MissionInitData.getSector_ID()>=0 && (pSD->UserParam==1 || pSD->UserParam==4) )	{ return true; }

	//if( pSD->UserParam != ROOT_SCENARIO.m_Scenario.m_inActivMenu )
	{
		ROOT_SCENARIO.m_Scenario.m_inActivMenu		= pSD->UserParam;
		ROOT_SCENARIO.m_Scenario.m_bMenuIsChange	= true;

		switch( ROOT_SCENARIO.m_Scenario.m_inActivMenu )
		{
			case	0:										// EUROPE MAP
				{
				ROOT_SCENARIO.m_Scenario.m_bBriefingVisible		= false;
				//ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled		= false;
				
				ROOT_SCENARIO.m_Scenario.m_bDiplVisible			= false;
				//ROOT_SCENARIO.m_Scenario.m_bInfoVisible			= true;

				ROOT_SCENARIO.m_Scenario.m_bMessagesVisible		= false;
				ROOT_SCENARIO.m_Scenario.m_bMarketVisible		= false;

				ROOT_SCENARIO.m_Scenario.m_bMapEnable			= true;
				ROOT_SCENARIO.m_Scenario.m_bMapMouseMove		= true;
				ROOT_SCENARIO.m_Scenario.m_bMapVisible			= true;
				
				ROOT_SCENARIO.m_Scenario.m_bPlayerEnable		= true;
				ROOT_SCENARIO.m_Scenario.m_bPlayerVisible		= true;
				//ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap			= true;
				
				ROOT_SCENARIO.m_Scenario.m_bSectEnable			= true;
				ROOT_SCENARIO.m_Scenario.m_bSectMouseMove		= true;
				ROOT_SCENARIO.m_Scenario.m_bSectSetPlayerColor	= true;
				ROOT_SCENARIO.m_Scenario.m_bSectVisible			= true;
				ROOT_SCENARIO.m_Scenario.m_bSectMenuVisible		= true;

				ROOT_SCENARIO.m_Scenario.m_bPERSONAL_HERO_MENU	= false;
				}
			break;
			case	1:										// DIPLOMACY
				{
				ROOT_SCENARIO.m_Scenario.m_bBriefingVisible		= false;
				//ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled		= false;
				
				ROOT_SCENARIO.m_Scenario.m_bDiplVisible			= true;
				//ROOT_SCENARIO.m_Scenario.m_bInfoVisible			= true;

				ROOT_SCENARIO.m_Scenario.m_bMessagesVisible		= false;
				ROOT_SCENARIO.m_Scenario.m_bMarketVisible		= false;

				ROOT_SCENARIO.m_Scenario.m_bMapEnable			= true;
				ROOT_SCENARIO.m_Scenario.m_bMapMouseMove		= true;
				ROOT_SCENARIO.m_Scenario.m_bMapVisible			= true;
				
				ROOT_SCENARIO.m_Scenario.m_bPlayerEnable		= true; //false; //!!!VERY NEW!!!
				ROOT_SCENARIO.m_Scenario.m_bPlayerVisible		= true;
				//ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap			= false;
				
				ROOT_SCENARIO.m_Scenario.m_bSectEnable			= true;
				ROOT_SCENARIO.m_Scenario.m_bSectMouseMove		= true;
				ROOT_SCENARIO.m_Scenario.m_bSectSetPlayerColor	= false;
				ROOT_SCENARIO.m_Scenario.m_bSectVisible			= true;
				ROOT_SCENARIO.m_Scenario.m_bSectMenuVisible		= false;

				ROOT_SCENARIO.m_Scenario.m_bPERSONAL_HERO_MENU	= false;
				}
			break;
			case	2:										// PERSONAL
				{
				ROOT_SCENARIO.m_Scenario.m_bBriefingVisible		= false;
				//ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled		= false;
				
				ROOT_SCENARIO.m_Scenario.m_bDiplVisible			= false;
				//ROOT_SCENARIO.m_Scenario.m_bInfoVisible			= false;

				ROOT_SCENARIO.m_Scenario.m_bMessagesVisible		= false;
				ROOT_SCENARIO.m_Scenario.m_bMarketVisible		= false; //!!!VERY NEW!!!

				ROOT_SCENARIO.m_Scenario.m_bMapEnable			= false;
				ROOT_SCENARIO.m_Scenario.m_bMapMouseMove		= false;
				ROOT_SCENARIO.m_Scenario.m_bMapVisible			= false;
				
				ROOT_SCENARIO.m_Scenario.m_bPlayerEnable		= false;
				ROOT_SCENARIO.m_Scenario.m_bPlayerVisible		= false;
				//ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap			= false;
				
				ROOT_SCENARIO.m_Scenario.m_bSectEnable			= false;
				ROOT_SCENARIO.m_Scenario.m_bSectMouseMove		= false;
				ROOT_SCENARIO.m_Scenario.m_bSectSetPlayerColor	= false;
				ROOT_SCENARIO.m_Scenario.m_bSectVisible			= false;
				ROOT_SCENARIO.m_Scenario.m_bSectMenuVisible		= false;

				ROOT_SCENARIO.m_Scenario.m_bPERSONAL_HERO_MENU	= true;
				}
			break;
			case	3:										//MARKET
				{
				ROOT_SCENARIO.m_Scenario.m_bBriefingVisible		= false;
				//ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled		= false;
				
				ROOT_SCENARIO.m_Scenario.m_bDiplVisible			= false;
				//ROOT_SCENARIO.m_Scenario.m_bInfoVisible			= false;

				ROOT_SCENARIO.m_Scenario.m_bMessagesVisible		= false;
				ROOT_SCENARIO.m_Scenario.m_bMarketVisible		= true;

				ROOT_SCENARIO.m_Scenario.m_bMapEnable			= false;
				ROOT_SCENARIO.m_Scenario.m_bMapMouseMove		= false;
				ROOT_SCENARIO.m_Scenario.m_bMapVisible			= false;
				
				ROOT_SCENARIO.m_Scenario.m_bPlayerEnable		= false;
				ROOT_SCENARIO.m_Scenario.m_bPlayerVisible		= false;
				//ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap			= false;//
				
				ROOT_SCENARIO.m_Scenario.m_bSectEnable			= false;
				ROOT_SCENARIO.m_Scenario.m_bSectMouseMove		= false;
				ROOT_SCENARIO.m_Scenario.m_bSectSetPlayerColor	= false;
				ROOT_SCENARIO.m_Scenario.m_bSectVisible			= false;
				ROOT_SCENARIO.m_Scenario.m_bSectMenuVisible		= false;

				ROOT_SCENARIO.m_Scenario.m_bPERSONAL_HERO_MENU	= false;
				}
			break;
			case	4:										// MESSAGES
				{
					ROOT_SCENARIO.m_Scenario.m_bBriefingVisible		= false;
					//ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled		= false;

					ROOT_SCENARIO.m_Scenario.m_bDiplVisible			= false;
					//ROOT_SCENARIO.m_Scenario.m_bInfoVisible			= false;

					ROOT_SCENARIO.m_Scenario.m_bMessagesVisible		= true;
					ROOT_SCENARIO.m_Scenario.m_bMarketVisible		= false;

					ROOT_SCENARIO.m_Scenario.m_bMapEnable			= false;
					ROOT_SCENARIO.m_Scenario.m_bMapMouseMove		= false;
					ROOT_SCENARIO.m_Scenario.m_bMapVisible			= false;

					ROOT_SCENARIO.m_Scenario.m_bPlayerEnable		= false;
					ROOT_SCENARIO.m_Scenario.m_bPlayerVisible		= false;
					//ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap			= false;

					ROOT_SCENARIO.m_Scenario.m_bSectEnable			= false;
					ROOT_SCENARIO.m_Scenario.m_bSectMouseMove		= false;
					ROOT_SCENARIO.m_Scenario.m_bSectSetPlayerColor	= false;
					ROOT_SCENARIO.m_Scenario.m_bSectVisible			= false;
					ROOT_SCENARIO.m_Scenario.m_bSectMenuVisible		= false;

					ROOT_SCENARIO.m_Scenario.m_bPERSONAL_HERO_MENU	= false;
				}
				break;
		}
	}


	if( MissionInitData.getSector_ID() >= 0 ) // доступность перед запуском миссии 
	{
		ROOT_SCENARIO.m_Scenario.m_bPlayerEnable = false;
	}

	ROOT_SCENARIO.m_Scenario.m_bHeroMiniMap = ROOT_SCENARIO.GetHeroInSect(ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID, 0xFFFF) && ROOT_SCENARIO.m_Scenario.m_inActivMenu==0; 

	ROOT_SCENARIO.UPDATE_SCENARIO();
	return true;
}
//------------------------------------------------------------------------------------
void	ChangeActiveButton(int ActGPID,int PasGPID,RLCFont* pActF,RLCFont* pPasF,DialogsSystem* pdsMenu)
{
	if( !ROOT_SCENARIO.m_Scenario.m_bMenuIsChange )			{ return; }

	GP_TextButton* a_pgptbPB = NULL;
	for (int i=0; i < 5 ; i++) {
		a_pgptbPB = (GP_TextButton*) pdsMenu->Find( 0x40+i );
		if( i == ROOT_SCENARIO.m_Scenario.m_inActivMenu ) {
			a_pgptbPB->Sprite     = a_pgptbPB->Sprite1		= ActGPID;
			a_pgptbPB->ActiveFont = a_pgptbPB->PassiveFont	= pActF;
		}
		else {
			a_pgptbPB->Sprite     = a_pgptbPB->Sprite1		= PasGPID;
			a_pgptbPB->ActiveFont = a_pgptbPB->PassiveFont	= pPasF;
		}
	}
	ROOT_SCENARIO.m_Scenario.m_bMenuIsChange = false;
}
//------------------------------------------------------------------------------------
bool	ON_HERO_CLICK(SimpleDialog* psdHero)
{
	if( !ROOT_SCENARIO.m_EnabledMapClick  )					{ return false; }	

	if( !ON_HERO_R_CLICK(psdHero) )							{ return false; }	
	if( !ROOT_SCENARIO.m_Scenario.m_bPlayerEnable )			{ return false; }
	
	CHero*	pH = reinterpret_cast<CHero*>(psdHero->UserParam);

	if( pH->m_inPlayerID != gi_bmSAVE_Nation )					{ return true; }

	if( pH->m_inPlayerID == gi_bmSAVE_Nation && pH->GetFuel() <= 0 ) {
		char  text[256];
		sprintf(text, GetTextByID("#CWM_HeroCantMove"), pH->GetName() );
		ROOT_SCENARIO.PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , 0 );
		ShowBigMapGlobalMessage( 1, 33 , 11 , text) ;
	}

	if( pH->m_inPlayerID == gi_bmSAVE_Nation && pH->GetFuel() > 0 && pH->GetARMY_NSQ() < 1 ) {
		char  text[256];
		sprintf(text, GetTextByID("#CWM_NoArmy"), pH->GetName() );
		ROOT_SCENARIO.PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , 0 );
		ShowBigMapGlobalMessage( 1, 33 , 11 , text) ;
		pH->m_CurStatus	= CHero::FREE;
		return true;
	}

	if( pH->m_CurStatus == CHero::FREE && !ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture && pH->GetFuel() > 0 )
	{
		pH->m_CurStatus = CHero::CAPTURE;
		pH->CreateEmptyHint(); 

		ROOT_SCENARIO.PushHeroOnTop(pH->getMapPicPt());
		ROOT_SCENARIO.m_Scenario.m_bMapMouseMove		= false;
		ROOT_SCENARIO.m_Scenario.m_bSectMouseMove		= false;
		//ROOT_SCENARIO.m_Scenario.m_bSectSetPlayerColor	= false; //VERY NEW!!!
		ROOT_SCENARIO.m_Scenario.m_inSectLastMouseOver	= -1;
		ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture	= true;
		ROOT_SCENARIO.m_Scenario.m_inPlayerInUseHero	= pH->GetPlayerID();
		ROOT_SCENARIO.m_Scenario.m_inLastHeroClick		= pH->GetHeroID()+pH->GetPlayerID()*100;
		ROOT_SCENARIO.m_Scenario.m_inSectNeighbor		= pH->getSectorID();

		ClipCursor(&g_MAP_RECT);
	}
	else 
		if( pH->m_CurStatus == CHero::CAPTURE && ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture ) 
		{
			pH->CreateHint();
			
			int n = ROOT_SCENARIO.m_pSect->GetNeighborsNum( pH->getSectorID() );
			int psecIDN[1000] ;
			ROOT_SCENARIO.m_pSect->GetNeighborsIDs( pH->getSectorID() , psecIDN , n );
			psecIDN[ n++ ] = pH->getSectorID() ; //!!!GUERILLAS!!! ( n = n+1 )
			for(int i=0; i < n ; i++)
			{
				int  WAR	 = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetWarId();
				int	 PACT    = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetPactId();
				int	 PASS    = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetPassId();
				int  ALLY    = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetAllyId();
				int  SecId   = ROOT_SCENARIO.m_Scenario.m_inSectLastMouseOver;
				int  OwnerId = ROOT_SCENARIO.m_pSect->GetSecOwner( SecId ) ;

				if( ROOT_SCENARIO.m_Scenario.m_inSectLastMouseOver == psecIDN[i] ) 
				{																						
					ClipCursor(NULL);

					if( !CanEnterIntoSector( pH->m_inPlayerID , SecId ) )				{ break; }

					if( ROOT_SCENARIO.m_pDipl->CheckRequests( WAR , 0 , SecId )	)		{ break; }

					int  pic = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetDiplActionPicture(WAR,pH->m_inPlayerID,OwnerId);
					if( OwnerId != pH->m_inPlayerID && 
						!ROOT_SCENARIO.m_pDipl->CheckContract( PASS , pH->m_inPlayerID , OwnerId ) && 
						ShowBigMapGlobalMessage(2, pic, 14,GetTextByID("#CWM_AttackSect")) != 1 ) { break; }

					if( pH->m_inSectorID == SecId && OwnerId == pH->m_inPlayerID )		{ break; } //very new
					if( pH->m_inSectorID == SecId && 
						ROOT_SCENARIO.m_pDipl->CheckContract( PASS , pH->m_inPlayerID , OwnerId ) ) { break; } //very new
					//if( OwnerId == pH->m_inPlayerID && pH->m_inSectorID == SecId )		{ break; } //very new

					// weather conditions in sector
					pH->m_CurStatus	= CHero::FREE;//VERY NEW
					if( ROOT_SCENARIO.m_inCurTurn >= 4 ) //new
					{
						if( rand()%100 < 10 || ROOT_SCENARIO.m_pSect->m_pssdDT[ SecId ]->m_iSabotageID >= 1000000 ) {
							if( ROOT_SCENARIO.m_pSect->m_pssdDT[ SecId ]->m_iSabotageID < 1000000 )
								ROOT_SCENARIO.m_pSect->m_pssdDT[ SecId ]->m_iSabotageID += 1000000 ; // bad weather
							char  text[256];  sprintf(text, GetTextByID("#CWM_Bad_Weather"), pH->GetName() );
							if( ShowBigMapGlobalMessage(2, 33 , 11 , text ,0) != 1 )		{ break; }
						} 
						else {
							if( rand()%100 < 10 ) {
								pH->SetFuel( 2 ); // good weather
								char  text[256];  sprintf(text, GetTextByID("#CWM_Good_Weather"), pH->GetName() );
								ShowBigMapGlobalMessage(1, 33 , 11 , text ,0) ;
							}
						}
					}

					if( pH->AddFuel( -1 ) ) 
					{
						pH->SetSector( ROOT_SCENARIO.m_pSect , SecId );
						ChangeActiveSector( ROOT_SCENARIO.m_pSect->m_pssdDT[SecId]->m_psdSec );

						// check for valid defender
						CHero* pDef = ROOT_SCENARIO.GetHeroInSect( SecId , pH->m_inPlayerID );
						if( pDef &&	 !ROOT_SCENARIO.CheckRightsForHeroInSector( pDef )  &&
							pDef->m_inPlayerID >= 0 && pDef->m_inPlayerID < MAX_PLAYERS )
							ROOT_SCENARIO.m_pPlayer[ pDef->m_inPlayerID ]->TryRetreat( pDef , false );

						if( OwnerId == pH->m_inPlayerID ||
							ROOT_SCENARIO.m_pDipl->CheckContract( PASS , pH->m_inPlayerID , OwnerId ) ) { 
							PlaySound( "INTERF_MOVE" );//new
							break; 
						}

						//Prepare data for init mission   ATTACKIN SECTOR SecId
						if( OwnerId != pH->m_inPlayerID ) 
						{
							PlaySound( "INTERF_ATTACK" );//new

							if(OwnerId != -1 && !ROOT_SCENARIO.m_pDipl->CheckContract(WAR, pH->m_inPlayerID, OwnerId)) 
								ROOT_SCENARIO.m_pDipl->AddContract(WAR, pH->m_inPlayerID, OwnerId, 5);

							ROOT_SCENARIO.m_Scenario.m_inAttackedSector = SecId ;
							ROOT_SCENARIO.m_Scenario.m_bPlayerEnable	= false ;
							ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled = true ;

							MissionInitData.PlayerStat = 0 ; // игрок - атакует
							MissionInitData.SetSector_ID( SecId );
							MissionInitData.setAttackerNation( pH->m_inPlayerID );	
							MissionInitData.setDefenderNation( OwnerId );

							//// Присутствует ли вражеский герой в секторе.
							CHero* pDH = ROOT_SCENARIO.GetHeroInSect( SecId , pH->m_inPlayerID );

							if( pDH ) {
								int DEF_NI = pDH->m_inPlayerID;
								if( OwnerId == DEF_NI || ROOT_SCENARIO.m_pDipl->CheckContract(ALLY, OwnerId, DEF_NI ) ) { 
									MissionInitData.setHeroPresent(true);
									if(OwnerId != -1 && !ROOT_SCENARIO.m_pDipl->CheckContract(WAR, pH->m_inPlayerID, DEF_NI)) 
										ROOT_SCENARIO.m_pDipl->AddContract(WAR, pH->m_inPlayerID, DEF_NI, 5);
								}
								else
									ROOT_SCENARIO.m_pPlayer[ DEF_NI ]->TryRetreat( pDH , false );
							}
							//if( pDH ) { 
							//	MissionInitData.setHeroPresent(true);
							//	ROOT_SCENARIO.m_pDipl->AddContract(WAR, pH->m_inPlayerID, DH_NI, 5);
							//}

							SetBigMapMenuEnabled( false );

							pH->m_CurStatus	= CHero::FREE;
							ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture	= false;
							ROOT_SCENARIO.m_Scenario.m_inSectNeighbor		= -1;
							ROOT_SCENARIO.PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , 0 );
							ProcessMessages();		StdKeys();
							ROOT_SCENARIO.m_pMenu->ProcessDialogs();    
							ROOT_SCENARIO.m_pMenu->RefreshView();

							// Run Mission!!!
							ProcessMissionForPlayer( ROOT_SCENARIO.m_Scenario.m_inBigMapCampID , true );
							ROOT_SCENARIO.CHECK_DEFEAT( gi_bmSAVE_Nation );
							ON_HERO_R_CLICK( psdHero );	
							break;
						} // if( OwnerId != pH->m_inPlayerID )
					} // if( pH->AddFuel( -1 ) )
				} // if
			} //for

			ROOT_SCENARIO.m_Scenario.m_bMapMouseMove		= true;
			ROOT_SCENARIO.m_Scenario.m_bSectMouseMove		= true;
			//ROOT_SCENARIO.m_Scenario.m_bSectSetPlayerColor	= true;
			ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture	= false;
			ROOT_SCENARIO.m_Scenario.m_inPlayerInUseHero	= -1;
			ROOT_SCENARIO.m_Scenario.m_inSectNeighbor		= -1;
			
			ClipCursor(NULL);
			pH->m_CurStatus	= CHero::FREE;
		}
	ROOT_SCENARIO.UPDATE_SCENARIO();

//#ifdef TEST_LVA			// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//		CE_01.ClearAll();
//		CE_01.CreateFromClass( &DS_01, 500, 50, 800, 300, pH, CED_MEMBERS|CED_TREE );
//#endif						// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
	return true;
}
//------------------------------------------------------------------------------------
bool	ON_HERO_R_CLICK(SimpleDialog* spdHero)
{
	if( spdHero->UserParam <= 0 )							{ return false; }

	if( ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture )		{ return true; } 
	if( ROOT_SCENARIO.m_Scenario.m_inActivMenu > 2 )		{ return false; }

	CHero*	pH = reinterpret_cast<CHero*>(spdHero->UserParam);
	
	ChangeActiveSector( ROOT_SCENARIO.m_pSect->m_pssdDT[ pH->m_inSectorID ]->m_psdSec ); //выбрать сектор этого героя 
	if( ROOT_SCENARIO.m_Scenario.m_bPERSONAL_HERO_MENU ) { ROOT_SCENARIO.m_phpDesk->ChangeActive( spdHero->UserParam ); }
	
	for(int i=0; i < MAX_PLAYERS ; i++) {
		ROOT_SCENARIO.m_pPlayer[i]->SetHeroMiniMenuVisible(false);
	}
	pH->SetVisibleMiniMenu(true);

	ROOT_SCENARIO.m_pHmm->SetAll( pH );
	ROOT_SCENARIO.m_pHPers->setPlayerID(pH->GetPlayerID());
	ROOT_SCENARIO.m_Scenario.m_inLastHeroClick = pH->GetHeroID() + pH->GetPlayerID()*100;
	ROOT_SCENARIO.UPDATE_SCENARIO();

//#ifdef TEST_LVA			// UNCOMMENT THIS IF NEEDED		//!!!!! - if u NEED it - uncomment!
//	CE_01.ClearAll();
//	CE_01.CreateFromClass( &DS_01, 500, 50, 800, 300, pH, CED_MEMBERS|CED_TREE );	
//#endif						// UNCOMMENT THIS IF u NEED IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	

	return true;
}
//------------------------------------------------------------------------------------
bool	ON_HERO_R_CLICK_VIRTUAL(CHero* pH)
{
	//if( ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture || 
	//	( ROOT_SCENARIO.m_Scenario.m_inActivMenu != 0 && ROOT_SCENARIO.m_Scenario.m_inActivMenu !=3 ) ) {
	//	if( ROOT_SCENARIO.m_Scenario.m_inActivMenu != 1 )   return true;
	//}
	if( ROOT_SCENARIO.m_Scenario.m_bPlayerHeroCapture )		{ return true; } 
	if( ROOT_SCENARIO.m_Scenario.m_inActivMenu > 2 )		{ return true; }

	for(int i=0; i < MAX_PLAYERS ; i++ ) {
		ROOT_SCENARIO.m_pPlayer[i]->SetHeroMiniMenuVisible(false);
	}
	pH->SetVisibleMiniMenu(true);

	ROOT_SCENARIO.m_pHmm->SetAll( pH );
	ROOT_SCENARIO.m_pHPers->setPlayerID( pH->GetPlayerID() );
	ROOT_SCENARIO.m_Scenario.m_inLastHeroClick = pH->GetHeroID() + pH->GetPlayerID()*100;
	ROOT_SCENARIO.UPDATE_SCENARIO();

	return true;
}
//------------------------------------------------------------------------------------
// увеличить кол-во отрядов на единицу
bool	ON_SQ_CLICK(SimpleDialog* spdSQ_U)
{
	int curPL = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick/100;
	int curHR = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick%100;

	if( curPL != gi_bmSAVE_Nation )			{ return true; }

	CHero* pH = ROOT_SCENARIO.m_pPlayer[curPL]->GetHeroByPID(curHR);
	if( pH ) 
	{
		int id = spdSQ_U->UserParam ;
		if( id < 0 )																{ return FailSound(); } //return true; } //INTERF_BADTHING 
		//new
		//if( pH->GetARMY_FreeNSQ() == 0 )											{ return FailSound(); } //return true; } 
		if( pH->GetARMY_FreeNSQ() == 0 ) { 
			FailSound();
			ShowBigMapGlobalMessage( 1, 33 , 11 , GetTextByID("#MaxSquads") , 2 ); 
			return true;
		}
		if( pH->ARMY[id]->getSQ_NUM() >= pH->ARMY[id]->getSQ_MAX() )				{ return FailSound(); } //return true; }

		// get Squad COST				
		int sqNUM = 1,  sqCOST[8] ;
		GetBigMapSquadCost( curPL , pH->ARMY[ id ]->getTypeGlobID() , sqNUM , sqCOST );

		// chekin Resources
		if( ROOT_SCENARIO.m_pPlayer[curPL]->GetResource( RECRT ) < sqNUM )			{ return FailSound(); } //return true; } 

		for(int res=0; res<=COAL ; res++) 
			if( ROOT_SCENARIO.m_pPlayer[curPL]->GetResource( res ) < sqCOST[res] )	{ return FailSound(); } //return true; } 

		// добавить отряд
		pH->AddSQbyArrID( id );

		// take Squad COST
		for(int res=0; res<=COAL ; res++)   ROOT_SCENARIO.m_pPlayer[curPL]->AddResource( res, -sqCOST[res] );

		// decrease recruits in sectors
		ROOT_SCENARIO.DelRecruitsInSectors( curPL , pH->getSectorID() , sqNUM ) ;

		pH->CreateHint();
		ROOT_SCENARIO.m_pHmm->SetAll( pH );

		//new
		int	   BrID = GetObjectBranch( pH->ARMY[ id ]->getTypeGlobID() );
		char*  SNDID[50] = { "INTERF_PEHOTA", "INTERF_KONI", "INTERF_PUSHKI" };
		if( BrID >= 0 && BrID < 3 )
			PlaySound( SNDID[ BrID ] );
	}

	ROOT_SCENARIO.UPDATE_SCENARIO();
	return true;
}
//bool	ON_SQ_CLICK(SimpleDialog* spdSQ_U)
//{
//	int curPL = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick/100;
//	int curHR = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick%100;
//
//	if( curPL !=  gi_bmSAVE_Nation )			{ return true; }
//
//	CHero* pH = ROOT_SCENARIO.m_pPlayer[curPL]->GetHeroByPID(curHR);
//	if( pH ) 
//	{
//		int id = spdSQ_U->UserParam ;
//
//		if( pH->GetARMY_FreeNSQ() == 0 )								{ return true; }
//		if( pH->ARMY[id]->getSQ_NUM() >= pH->ARMY[id]->getSQ_MAX() )	{ return true; }
//
//
//		// !!!!!!MUST REPLAYSED by GetBigMapSquadCost()!!!
//
//		// кол-во солдат в отряде по типу войск
//		int sqNUM = 1 ;
//		if( GetObjectBranch( pH->ARMY[ id ]->getTypeGlobID() ) == 0 )	sqNUM = SQ_Amount0 ; //sqNUM = 120 ;
//		if( GetObjectBranch( pH->ARMY[ id ]->getTypeGlobID() ) == 1 )	sqNUM = SQ_Amount1 ; //sqNUM = 30  ;
//
//		if( ROOT_SCENARIO.m_pPlayer[curPL]->GetResource( RECRT ) < sqNUM )			{ return true; }
//
//		// units COST
//		int COST4[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 } ;	
//		int sqCOST[8] = { 0, 0, 0, 0, 0, 0, 0, 0 } ; 
//		if( sqNUM == 1 ) { ; }					// artilery
//		else									// cavalerya
//			if( sqNUM == SQ_Amount1 /*30*/ ) { sqCOST[FOOD] += 150 ; sqCOST[GOLD] +=  50 ; } //FLAGI!!!!
//			else {								// infantry
//				sqCOST[FOOD] += 100 ; sqCOST[GOLD] +=  50 ; //OFFECIERERS!!!!
//				sqCOST[FOOD] += 100 ; sqCOST[GOLD] +=  50 ; //BARABANS!!!!
//				sqCOST[FOOD] += 100 ; sqCOST[GOLD] += 300 ; //FLAGI!!!!
//			}
//		GetUnitCost( curPL , pH->ARMY[id]->getTypeGlobID() , COST4 ,-1,-1);
//		for(int res=0; res<=COAL ; res++) {
//			sqCOST[res] += COST4[res] * sqNUM ;
//		}
//		if( sqNUM == 1 )		{ sqNUM = SQ_Amount2 ; }	// artilery
//		//if( sqNUM == 1 )		{ sqNUM += 3 ; }	// artilery
//		//else									
//		//	if( sqNUM == 30 )	{ sqNUM += 1 ; }	// cavalerya
//		//	else				{ sqNUM += 3 ; }	// infantry
//
//		// !!!!!!MUST REPLAYSED by GetBigMapSquadCost()!!!
//
//
//		// chekin resources
//		for(int res=0; res<=COAL ; res++) 
//			if( ROOT_SCENARIO.m_pPlayer[curPL]->GetResource( res ) < sqCOST[res] ) { return true; }
//
//		// добавить отряд
//		pH->addSQbyArrID( id );
//
//		// take units COST
//		for(int res=0; res<=COAL ; res++) 
//			if( sqCOST[res] > 0 ) { ROOT_SCENARIO.m_pPlayer[curPL]->AddResource( res, -sqCOST[res] ); }
//
//		// decrease recruits in sectors
//		ROOT_SCENARIO.DelRecruitsInSectors( curPL , sqNUM ) ;
//
//		pH->CreateHint();
//	}
//
//	//if(pH) pH->addSQbyArrID( spdSQ_U->UserParam ); //+ AvailableSQs-- ; (глобал - доступные для мобилизации отряды)
//	ROOT_SCENARIO.UPDATE_SCENARIO();
//	return true;
//}

//------------------------------------------------------------------------------------ 
// уменьшить кол-во отрядов на единицу
bool	ON_SQ_R_CLICK(SimpleDialog* spdSQ_U)// Grey [10.11.2003]
{
	int curPL = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick/100;
	int curHR = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick%100;

	if( curPL != gi_bmSAVE_Nation )			{ return true; }

	CHero* pH = ROOT_SCENARIO.m_pPlayer[curPL]->GetHeroByPID(curHR);

	if( pH )
	{
		int id = spdSQ_U->UserParam ;
		if( id < 0 )										{ return FailSound(); } //return true; } 
		if( pH->ARMY[id]->getSQ_NUM() <= 0 )				{ return FailSound(); } //return true; } 

		// get Squad COST				
		int sqNUM = 1,  sqCOST[8] ;
		GetBigMapSquadCost( curPL , pH->ARMY[ id ]->getTypeGlobID() , sqNUM , sqCOST );

		// decrease Squad
		pH->DelSQbyArrID( id );

		// add Squad COST
		for(int res=0; res<=COAL ; res++) ROOT_SCENARIO.m_pPlayer[curPL]->AddResource( res, sqCOST[res] );
		
		// add recruits in sectors
		ROOT_SCENARIO.AddRecruitsInSectors( curPL , pH->getSectorID() , sqNUM ) ;

		pH->CreateHint();
		ROOT_SCENARIO.m_pHmm->SetAll( pH ); 

		//new
		int	   BrID = GetObjectBranch( pH->ARMY[ id ]->getTypeGlobID() );
		char*  SNDID[50] = { "INTERF_PEHOTA", "INTERF_KONI", "INTERF_PUSHKI" };
		if( BrID >= 0 && BrID < 3 )
			PlaySound( SNDID[ BrID ] );
	}

	ROOT_SCENARIO.UPDATE_SCENARIO();
	return true;
}
//bool	ON_SQ_R_CLICK(SimpleDialog* spdSQ_U)// Grey [10.11.2003]
//{
//	int curPL = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick/100;
//	int curHR = ROOT_SCENARIO.m_Scenario.m_inLastHeroClick%100;
//
//	if( curPL !=  gi_bmSAVE_Nation )			{ return true; }
//
//	CHero* pH = ROOT_SCENARIO.m_pPlayer[curPL]->GetHeroByPID(curHR);
//
//	if( pH )
//	{
//		int id = spdSQ_U->UserParam ;
//
//		if(pH->ARMY[id]->getSQ_NUM()<=0)			{ return true; }
//
//
//		// !!!!!!MUST REPLAYSED by GetBigMapSquadCost()!!!
//
//		// кол-во солдат в отряде по типу войск
//		int sqNUM = 1 ;
//		if( GetObjectBranch( pH->ARMY[ id ]->getTypeGlobID() ) == 0 )	sqNUM = SQ_Amount0 ; //sqNUM = 120 ;
//		if( GetObjectBranch( pH->ARMY[ id ]->getTypeGlobID() ) == 1 )	sqNUM = SQ_Amount1 ; //sqNUM = 30  ;
//
//		// units COST
//		int COST4[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 } ;	
//		int sqCOST[8] = { 0, 0, 0, 0, 0, 0, 0, 0 } ; 
//		if( sqNUM == 1 ) { ; }					// artilery
//		else									// cavalerya
//			if( sqNUM == SQ_Amount1 /*30*/ ) { sqCOST[FOOD] += 150 ; sqCOST[GOLD] +=  50 ; } //FLAGI!!!!
//			else {								// infantry
//				sqCOST[FOOD] += 100 ; sqCOST[GOLD] +=  50 ; //OFFECIERERS!!!!
//				sqCOST[FOOD] += 100 ; sqCOST[GOLD] +=  50 ; //BARABANS!!!!
//				sqCOST[FOOD] += 100 ; sqCOST[GOLD] += 300 ; //FLAGI!!!!
//			}
//		GetUnitCost( curPL , pH->ARMY[id]->getTypeGlobID() , COST4 ,-1,-1);
//		for(int res=0; res<=COAL ; res++) {
//			sqCOST[res] += COST4[res] * sqNUM ;
//		}
//		if( sqNUM == 1 )		{ sqNUM = SQ_Amount2 ; }	// artilery
//
//		// !!!!!!MUST REPLAYSED by GetBigMapSquadCost()!!!
//
//
//		// decrease squard
//		pH->delSQbyArrID( id );
//
//		// add units COST
//		for(int res=0; res<=COAL ; res++) 
//			if( sqCOST[res] > 0 ) {	ROOT_SCENARIO.m_pPlayer[curPL]->AddResource( res, sqCOST[res] ); }
//
//		// add recruits in sectors
//		ROOT_SCENARIO.AddRecruitsInSectors( curPL , sqNUM ) ;
//
//		pH->CreateHint();
//	}
//
//	//pH->addSQbyArrID( spdSQ_U->UserParam ); //+ AvailableSQs++ ;  (глобал - доступные для мобилизации отряды)
//	ROOT_SCENARIO.UPDATE_SCENARIO();
//	return true;
//}

//------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------
//-------------------------------- ProcessMissionForPlayer --------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void	ProcessMissionForPlayer(int _campainID_, bool needsave )
{
	pAtackerHero		= NULL ;
	pDefendeHero		= NULL ;	
	CSectData* pSecDat	= ROOT_SCENARIO.m_pSect;
	
	int a_inASID = MissionInitData.getSector_ID();	//Номер атакованного сектора.
	if( a_inASID != -1 ) 
	{
		if( MissionInitData.DefenderNation_ID < 0 || MissionInitData.DefenderNation_ID >= MAX_PLAYERS ) {
			MissionInitData.DefenderNation_ID = -1 ; //!!!!!//
		}

		// && сохранение результатов 
		if( needsave )  SaveBigMapDataXML();

		int a_inASID = MissionInitData.getSector_ID();	//Номер атакованного сектора.

		// --- Prepare data for init mission ---
		pAtackerHero = ROOT_SCENARIO.m_pPlayer[ MissionInitData.getAttackerNation() ]->GetHeroInSector( a_inASID );
		pDefendeHero = ROOT_SCENARIO.GetHeroInSect( a_inASID , MissionInitData.getAttackerNation() ); 

		MissionInitData.setAttackerDirection_ID((reinterpret_cast<CDirectonDataBase*>(SCRIPT_DIRECTION.ScriptType[0]))->getDirection( pAtackerHero->getPreviousSecID(), a_inASID )); //pAtackerHero->getSectorID()));

		int  PACT = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetPactId();
		if( pDefendeHero && // попытка pDefendeHero отвоевать восст.(чужой)сектор
			( ( pDefendeHero->GetPlayerID() != MissionInitData.DefenderNation_ID && 
			   !ROOT_SCENARIO.m_pDipl->CheckContract(PACT, pDefendeHero->GetPlayerID(), MissionInitData.DefenderNation_ID) ) ||
			    MissionInitData.DefenderNation_ID == -1 ) )
		{
			pAtackerHero->SetSector( pSecDat , pAtackerHero->getPreviousSecID() );
			ROOT_SCENARIO.ShowInGameSector( a_inASID , 0.3 );

			bool win_battle = ROOT_SCENARIO.SimulateBattle(pDefendeHero,NULL,ROOT_SCENARIO.m_pSect->m_pssdDT[a_inASID]);
			if( win_battle ) {	// победа
				MissionInitData.DefenderNation_ID = pDefendeHero->GetPlayerID();
				(pSecDat->m_psddDT[a_inASID])->SetOwner( pDefendeHero->GetPlayerID() );	//Перекрасить сектор.
			} else {			// поражение
				ROOT_SCENARIO.m_pPlayer[ pDefendeHero->GetPlayerID() ]->TryRetreat( pDefendeHero ,false);
				if( pDefendeHero->GetARMY_NSQ() <= 0  && pDefendeHero->m_Alive ) //last add   
					pDefendeHero->SetHeroDefeatArmy() ;	
			}
			pAtackerHero->SetSector( pSecDat , a_inASID );
			ROOT_SCENARIO.ShowInGameSector( a_inASID , 1 );
		}

		if( !ProcessBigMapBriefing() )  
		{
			if( MissionInitData.PlayerStat == 0 ) { 
				if(pAtackerHero) ROOT_SCENARIO.m_pPlayer[pAtackerHero->GetPlayerID()]->TryRetreat( pAtackerHero ,false);
			} else {
				if(pDefendeHero) ROOT_SCENARIO.m_pPlayer[pDefendeHero->GetPlayerID()]->TryRetreat( pDefendeHero ,false);

				bool ATisWIN = ROOT_SCENARIO.SimulateBattle(pAtackerHero,NULL,ROOT_SCENARIO.m_pSect->m_pssdDT[a_inASID]);
				PlaySound( "INTERF_ATTACK" );//new
				char text[256],  tmp[256];	
				sprintf(tmp, "#CWM_SectBattle%d", !ATisWIN );
				sprintf(text,GetTextByID( tmp ),ROOT_SCENARIO.m_pSect->GetSectorName(a_inASID),pAtackerHero->GetName());
				ShowBigMapGlobalMessage(1, 33 , 31 + ATisWIN  , text , 0 ); //ShowBigMapGlobalMessage(1, 33 , 11 , text , 0 );
				if( ATisWIN ) { // победа
					(pSecDat->m_psddDT[MissionInitData.getSector_ID()])->SetOwner( MissionInitData.getAttackerNation() );// Перекрасить сектор.
					pSecDat->SetSecDefeatDefence( MissionInitData.getSector_ID() ); //сектор захвачен-Defence=0!
				}else {			// поражение
					ROOT_SCENARIO.m_pPlayer[ pAtackerHero->GetPlayerID() ]->TryRetreat( pAtackerHero ,false);
					if( pAtackerHero->GetARMY_NSQ() <= 0  && pAtackerHero->m_Alive ) //last add  
						pAtackerHero->SetHeroDefeatArmy() ;	
				}
			}
			//Выставить все для начала следующей миссии.
			MissionInitData.SetSector_ID(-1);
			MissionInitData.setHeroPresent(false);
			ROOT_SCENARIO.m_Scenario.m_inAttackedSector = -1    ;
			ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled = false ;
			ROOT_SCENARIO.m_Scenario.m_bPlayerEnable	= true ;
			MissionInitData.ClearAttacker();	MissionInitData.ClearDefender();
			pAtackerHero = NULL;				pDefendeHero = NULL;
			//GREY [15.03.2004]
			// Enable of All Menu elements
			SetBigMapMenuEnabled( true );
			// && сохранение результатов 
			SaveBigMapDataXML();
			return;
		}

		// - for quests
		if( MissionInitData.PlayerStat == 0 ) { 
			ROOT_SCENARIO.m_inAttackedNation = ROOT_SCENARIO.m_pSect->GetSecOwner( a_inASID );
		}
		// - for quests

		MissionInitData.ClearAttacker();
		MissionInitData.ClearDefender();

		MissionInitData.setAttackerArmy( pAtackerHero );
		if(pDefendeHero) MissionInitData.setDefenderArmy( pDefendeHero );
		
		MissionInitData.setAttackerDirection_ID((reinterpret_cast<CDirectonDataBase*>(SCRIPT_DIRECTION.ScriptType[0]))->getDirection( pAtackerHero->getPreviousSecID(), a_inASID )); //pAtackerHero->getSectorID()));
	
		// долить дефендеру гарнизон сектора [ F(defence) ]
		MissionInitData.addArmyBySectorDefence( pSecDat->GetSecDefence(a_inASID) , pSecDat->GetSecPopulation(a_inASID) );


		//Сохранить данные для инициализации миссии.
		xmlQuote	xmlMIS( "CSAVE_MIS_STRUCT" );		//xmlQuote	xmlMIS( "smsSAVE" );
		smsSAVE += MissionInitData; 
		smsSAVE.DifficultyLevel = pSAVE->m_iDifficulty;

		// сохр.ресурсов
		if( smsSAVE.DefenderNation_ID >= 0 && smsSAVE.DefenderNation_ID < MAX_PLAYERS ) {
			smsSAVE.SetResources( ROOT_SCENARIO.m_pPlayer[smsSAVE.AttackerNation_ID], ROOT_SCENARIO.m_pPlayer[smsSAVE.DefenderNation_ID] ); 
		}else {
			smsSAVE.SetResources( ROOT_SCENARIO.m_pPlayer[smsSAVE.AttackerNation_ID], ROOT_SCENARIO.m_pPlayer[smsSAVE.AttackerNation_ID] ); 
		}

		smsSAVE.SetNKills( pAtackerHero , pDefendeHero );

		EndMissionData.Init();
		if( ROOT_SCENARIO.m_bSaveLogs )  EndMissionData.SaveXML(); // -=== !!!NEEDED 4 TEST!!! ===-

		smsSAVE.Save( xmlMIS, &smsSAVE ); 
		xmlMIS.WriteToFile( "Missions\\MissionEngin\\smsSAVE.xml" );

		// Run Mission
		SaveState = 6 ; //disable autosave
		RunMission( CAMPAGINS.SCamp[_campainID_].Miss[MissionInitData.Sector_ID], pSAVE->m_iDifficulty );


		// Precache Units for Game
		byte  ATT_NI = 0, DEF_NI = 1 ;
		if( MissionInitData.PlayerStat == 0 )  {   ATT_NI = 1;   DEF_NI = 0;   }
		GAMEOBJ Type;
		for(int i=0; i <  MissionInitData.AttackerForce.GetAmountOfElements()  ; i++) {
			if( MissionInitData.AttackerForce[i]->nSquarnd > 0 )
				PrecacheOneUnit( MissionInitData.AttackerForce[i]->TypeIndex_ID ,  ATT_NI  ) ;
		}
		for(int i=0; i < MissionInitData.DefenderForce.GetAmountOfElements() ; i++) {
			if( MissionInitData.DefenderForce[i]->nSquarnd > 0 )
				PrecacheOneUnit( MissionInitData.DefenderForce[i]->TypeIndex_ID ,  DEF_NI  ) ;
		}


		PlayGame();


		if( v_Exit2MainMenu ) { 
			ROOT_SCENARIO.m_bEndGame = true; // from vital F12-Menu
			return; 
		}

		//set res
		for(int nati =0; nati < MAX_PLAYERS; nati++){
			for(int res=0; res<=COAL ; res++) { 
				SetResource( nati , res , 100000 );
			}
		}


		int  Attacker_NSQ = 0 ,  Defender_NSQ = 0 ;
		// переопределить ID типов войск (юнитов) после миссии	ReIDENTIFYUnitType();
		//GAMEOBJ Type;
		for(int i=0; i < EndMissionData.AttackerArmy.GetAmountOfElements() ; i++) {
			RegisterUnitType( &Type, EndMissionData.AttackerArmy[i]->TypeName_sID.pchar() );
			EndMissionData.AttackerArmy[i]->inTypeID = Type.Index ;
			Attacker_NSQ += EndMissionData.AttackerArmy[i]->inAmount ;
		}
		for(int i=0; i < EndMissionData.DefenderArmy.GetAmountOfElements() ; i++) {
			RegisterUnitType(&Type, EndMissionData.DefenderArmy[i]->TypeName_sID.pchar() );
			EndMissionData.DefenderArmy[i]->inTypeID = Type.Index ;
			Defender_NSQ += EndMissionData.DefenderArmy[i]->inAmount ;
		}

		pAtackerHero->AddBattle();
		if( pDefendeHero )  pDefendeHero->AddBattle();


		//int startPlayerRank = 0 ;
		//if( MissionInitData.PlayerStat == 0 )   { startPlayerRank = pAtackerHero->GetRank() ; }
		//else if( pDefendeHero )					{ startPlayerRank = pDefendeHero->GetRank() ; }

		//ProcessBigMapPlayVideo();
		//ProcessBigMapStatistics();

		//// decrease defence if needed
		//int  SecDef		= pSecDat->GetSecDefence( a_inASID ) ;
		//int  defendNSQ	= 0 ;
		//if( pDefendeHero )   defendNSQ = pDefendeHero->GetARMY_NSQ() ;

		//int  SEC_DEF_SQ  = Defender_NSQ - defendNSQ ;
		//if( SEC_DEF_SQ < 0 )   SEC_DEF_SQ = 0;
		//int  RES_SEC_DEF = GetSQNumByDefenceID( SecDef , 0 , 0 , a_inASID )/( SecDef + 1 ) ;
		//RES_SEC_DEF = SEC_DEF_SQ / ( (RES_SEC_DEF!=0)?RES_SEC_DEF:1 ) ; 
		//if( RES_SEC_DEF < 0 )  RES_SEC_DEF = 0;
		//if( SecDef > RES_SEC_DEF )  ROOT_SCENARIO.m_pSect->m_pssdDT[a_inASID]->m_inDefence = RES_SEC_DEF;


		//----------------------------------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------------------------------
		// Обработка данных по окончании миссии.
		if( EndMissionData.AttackerNI != EndMissionData.DefenderNI ) 
		{
			////===ИГРОК   (pAtackerHero) ===

			////Добавим игроку опыт за миссию.
			//pAtackerHero->AddExperience(EndMissionData.AttackerExperience);
			////Корекция нового звания игрока.
			//pAtackerHero->UpdateRankWithExperience();
			

			//Выставить оставшуюся армию.
			bool find ;
			//if( EndMissionData.AttackerArmy.GetAmountOfElements() == 0 )   { pAtackerHero->SetHeroDefeatArmy(); }
			//else 
			//{
				for(int sq=0; sq < pAtackerHero->ARMY.GetAmountOfElements() ; sq++ ) 	
				{
					find = false ;

					pAtackerHero->ARMY[sq]->Kills.Clear();
					for(int i=0; i < EndMissionData.AttackerArmy.GetAmountOfElements() && !find ; i++)	
					{
						int n	 = EndMissionData.AttackerArmy[i]->inAmount ;
						int type = EndMissionData.AttackerArmy[i]->inTypeID ;
						if( pAtackerHero->ARMY[sq]->getTypeGlobID() == type )  
						{
							if( n < pAtackerHero->ARMY[sq]->getSQ_NUM() ) { 
								pAtackerHero->ARMY[sq]->setSQ_NUM( n ); 
							}
							//
							if( GetObjectBranch( type ) == 2 ){
								if( n > pAtackerHero->ARMY[sq]->getSQ_MAX() ){  
									n = pAtackerHero->ARMY[sq]->getSQ_MAX(); 
								}
								pAtackerHero->ARMY[sq]->setSQ_NUM( n ); 
							}
							//
							pAtackerHero->SetNKills( EndMissionData.AttackerArmy[i]->Kills , type );
							find = true ;
						}
					}
					if( !find )   { pAtackerHero->ARMY[sq]->setSQ_NUM( 0 ) ; }
				} 
			//}


			//выделение атаковавшего героя(игрока)
			ON_HERO_R_CLICK_VIRTUAL(pAtackerHero);

			////=== КОМП   (pDefendeHero) ===

			////Добавить опыт виртуальному игроку.
			//if(pDefendeHero)	pDefendeHero->AddExperience(EndMissionData.DefenderExperience);
			////Корекция нового звания виртуального игрока.
			//if(pDefendeHero)	pDefendeHero->UpdateRankWithExperience();
			////if (pDefendeHero)	pDefendeHero->AddBattle();//grey

			//Выставить оставшуюся армию.
			if( pDefendeHero ) {
			//if( EndMissionData.DefenderArmy.GetAmountOfElements() == 0 )   { pDefendeHero->SetHeroDefeatArmy(); }
			//else 
			//{		
				for(int sq=0; sq < pDefendeHero->ARMY.GetAmountOfElements() ; sq++ ) 	
				{
					find = false ;

					pDefendeHero->ARMY[sq]->Kills.Clear();
					for(int i=0; i < EndMissionData.DefenderArmy.GetAmountOfElements() && !find ; i++)	
					{
						int n	 = EndMissionData.DefenderArmy[i]->inAmount ;
						int type = EndMissionData.DefenderArmy[i]->inTypeID ;
						if( pDefendeHero->ARMY[sq]->getTypeGlobID() == type )  
						{
							if( n < pDefendeHero->ARMY[sq]->getSQ_NUM() ){
								pDefendeHero->ARMY[sq]->setSQ_NUM( n ) ; 
							}
							//
							if( GetObjectBranch( type ) == 2 ){
								if( n > pDefendeHero->ARMY[sq]->getSQ_MAX() ){
									n = pDefendeHero->ARMY[sq]->getSQ_MAX(); 
								}
								pDefendeHero->ARMY[sq]->setSQ_NUM( n ); 
							}
							//
							pDefendeHero->SetNKills( EndMissionData.DefenderArmy[i]->Kills , type );
							find = true ;
						}
					}
					if( !find )   { pDefendeHero->ARMY[sq]->setSQ_NUM( 0 ) ; }
				}
			//} 
			}
            

			if( EndMissionData.WinnerNI == MissionInitData.getAttackerNation() ) 
			{
				//---------- Победил атакер ----------
				pAtackerHero->AddVictory();//grey
				//Перекрасить сектор.
				(pSecDat->m_psddDT[MissionInitData.getSector_ID()])->SetOwner( EndMissionData.WinnerNI );
				pSecDat->SetSecDefeatDefence(MissionInitData.getSector_ID());//сектор захвачен-Defence=0!
				//Удалить героя виртуального игрока.
				if(pDefendeHero) ROOT_SCENARIO.m_pPlayer[pDefendeHero->GetPlayerID()]->TryRetreat(pDefendeHero,false);
				//new
				if( pDefendeHero && pDefendeHero->GetARMY_NSQ() <= 0  && pDefendeHero->m_Alive ) //last add 
					pDefendeHero->SetHeroDefeatArmy(); 
			} 
			else {
				//----- Победил защитник сектора ------
				if(pDefendeHero) pDefendeHero->AddVictory();//grey
				//pAtackerHero->SetSector(pSecDat,pAtackerHero->getPreviousSecID());//vic
				ROOT_SCENARIO.m_pPlayer[pAtackerHero->GetPlayerID()]->TryRetreat( pAtackerHero ,false);//grey
				if( pAtackerHero->GetARMY_NSQ() <= 0  && pAtackerHero->m_Alive ) //last add
					pAtackerHero->SetHeroDefeatArmy(); 
			}
			
			// Ресурсы из миссии
			ROOT_SCENARIO.m_pPlayer[smsSAVE.AttackerNation_ID]->Resource = EndMissionData.AttackerRes;
			if( smsSAVE.DefenderNation_ID >= 0 && smsSAVE.DefenderNation_ID < MAX_PLAYERS ) {
				ROOT_SCENARIO.m_pPlayer[smsSAVE.DefenderNation_ID]->Resource = EndMissionData.DefenderRes;   }
		}
		else
			if( EndMissionData.WinnerNI < 0 ) // миссия не завершена //СЮДА НЕ ДОЛЖНЫ ПОПАДАТЬ!!!
			{
				//grey
				if( MissionInitData.PlayerStat == 0 ) { 
					pAtackerHero->SetHeroDefeatArmy(); // очистить все отряды!!!!!!
					ROOT_SCENARIO.m_pPlayer[ pAtackerHero->GetPlayerID() ]->TryRetreat( pAtackerHero ,false);
				}
				else { 
					if(pDefendeHero) pDefendeHero->SetHeroDefeatArmy(); // очистить все отряды!!!!!!
					if(pDefendeHero) ROOT_SCENARIO.m_pPlayer[ pDefendeHero->GetPlayerID() ]->TryRetreat( pDefendeHero ,false);
				}
			}
	
		//----------------------------------------------------------------------------------------------------------------------------
		// decrease defence if needed
		int  SecDef		= pSecDat->GetSecDefence( a_inASID ) ;
		int  SecPop		= pSecDat->GetSecPopulation( a_inASID ) ;
		int  defendNSQ	= 0 ;
		if( pDefendeHero )   defendNSQ = pDefendeHero->GetARMY_NSQ() ;

		int  SEC_DEF_SQ  = Defender_NSQ - defendNSQ ;
		if( SEC_DEF_SQ < 0 )   SEC_DEF_SQ = 0;
		int  RES_SEC_DEF = GetSQNumByDefenceID( SecDef , 0 , 0 , a_inASID )/( SecDef + 1 );
		RES_SEC_DEF = SEC_DEF_SQ / ( (RES_SEC_DEF!=0)?RES_SEC_DEF:1 ) ; 
		if( RES_SEC_DEF < 0 )  RES_SEC_DEF = 0;
		if( SecDef > RES_SEC_DEF )  ROOT_SCENARIO.m_pSect->m_pssdDT[a_inASID]->m_inDefence = RES_SEC_DEF;
		if( SecPop > RES_SEC_DEF &&
		  ( SecPop > 1 || rand()%10 > 5 ) )  ROOT_SCENARIO.m_pSect->m_pssdDT[a_inASID]->m_inPopulation-- ;
		//----------------------------------------------------------------------------------------------------------------------------

		ProcessBigMapPlayVideo();
		ProcessBigMapStatistics();

		ROOT_SCENARIO.m_pSectMenu->m_inCurSectID = -1 ;
		ROOT_SCENARIO.UPDATE_SCENARIO();
		ROOT_SCENARIO.PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , 0 );

		// проверка оставшихся стран
		ROOT_SCENARIO.CHECK_DEFEAT( gi_bmSAVE_Nation );//GREY [16.03.2004]

		// check Quests Complete for PLAYER gi_bmSAVE_Nation
		ROOT_SCENARIO.m_pQuest->ProcessQuests( &ROOT_SCENARIO , gi_bmSAVE_Nation , false );
		
		// Выставить все для начала следующей миссии.
		pAtackerHero = NULL;
		pDefendeHero = NULL;
		MissionInitData.SetSector_ID(-1);
		MissionInitData.setHeroPresent(false);
		MissionInitData.ClearAttacker();
		MissionInitData.ClearDefender();
		
		ROOT_SCENARIO.m_Scenario.m_inAttackedSector = -1    ;
		ROOT_SCENARIO.m_Scenario.m_bPlayerEnable	= true ;
		ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled = false ;

		// Enable of All Menu elements
		SetBigMapMenuEnabled( true );//GREY [15.03.2004]

		// && сохранение результатов
		SaveBigMapDataXML();
	}
}
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
//-------------------------------- ProcessBigMapBriefing --------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
bool	ProcessBigMapBriefing()
{
	
	//test
	int i5500 = GSets.CGame.ArcadeMode; // = true;//!!!!! (default=false;)
	//test

	_str  tBlack, tBrown, tRed;  tBlack += "{C FF101010}";  tBrown += "{C FF502515}";  tRed += "{C FF640100}";

	int  X0 = 79 , Y0 = 99 ;	
	int  dy = 10 , dx = 25 ;
	int  secID			= MissionInitData.getSector_ID();
	int  AttackNationID	= MissionInitData.getAttackerNation();
	int  DefendNationID	= MissionInitData.getDefenderNation();

	RLCFont* font	 = &BlackFont;		RLCFont* Rfont	 = &RedFont; 
	RLCFont* ActFont = &WhiteFont;		RLCFont* PasFont = &YellowFont;
	//RLCFont* font	 = &BlackFont;		RLCFont* Rfont	 = &RedFont;	RLCFont* ActFont = &WhiteFont;	RLCFont* PasFont = &YellowFont;
	
	DialogsSystem	a_dsMenu(0,0);												//новое меню
	a_dsMenu.SetWidth(1024);
	a_dsMenu.SetHeight(768);

	//BlackScreen
	LocalGP			a_lgpScreen("Interf3\\TotalWarGraph\\blackscr");			
	GPPicture*		a_pgppScr = a_dsMenu.addGPPicture(NULL,0,0,a_lgpScreen.GPID,0);
	a_pgppScr->Diffuse = 0x88202020;
	a_pgppScr->Enabled = true ;
	a_pgppScr->OnUserClick = &EmptyClick ;
	ProcessMessages();		StdKeys();		a_dsMenu.ProcessDialogs();		a_dsMenu.RefreshView();

	//Background
	SQPicture		a_sqpBack("Interf3\\TotalWarGraph\\turn_map_briefing.jpg"); //Фон
	a_dsMenu.addPicture(NULL,0,0,&a_sqpBack,&a_sqpBack,&a_sqpBack);
	// BriefingHead
	TextButton* ptbBigMapHead  = a_dsMenu.addTextButton(NULL, 512, 47 ,GetTextByID("#BriefingHead"),&fonMenuTitle[3],&fonMenuTitle[3],&fonMenuTitle[3],1);//NEED NEW FONTS!!!

	// ********************* preview карты сектора *********************
	LocalGP			a_lgpSecMap("Interf3\\TotalWarGraph\\bmSectMap");			//sectors maps
	int  sprID = 2*secID ;
	if( AttackNationID == gi_bmSAVE_Nation && DefendNationID != -1 ) sprID++ ;
	GPPicture*	SecMap = a_dsMenu.addGPPicture(NULL, 337, 97, a_lgpSecMap.GPID, sprID ); 
	SecMap->Diffuse = 0xffefefef ; /* 0xffffffff */

	// ********************* базы армий на карте сектора ***************  
	int  BX0 = SecMap->x ,  BY0 = SecMap->y ;
	LocalGP			a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");
	GPPicture*	Base1 = a_dsMenu.addGPPicture(NULL, BX0, BY0,a_lgpFlags.GPID, AttackNationID + 1) ;
	GPPicture*	Base2 = a_dsMenu.addGPPicture(NULL, BX0, BY0,a_lgpFlags.GPID, DefendNationID + 1) ;
	if( DefendNationID == -1 )   Base2->Visible = false ;

	int  dirID_A	= MissionInitData.AttackDirection_ID ;	// SecMap - xy= 338 98	x1y1= 944 410  (w=606 h=312)
	int  dirID_D	= dirID_A<4 ? dirID_A+4 : dirID_A-4 ;
	//{"East","North-East","North","North-West","West","South-West","South","South-East",""}

	Base1->x += BriefDIR_XY[ secID ][dirID_A][0];	Base1->y += BriefDIR_XY[ secID ][dirID_A][1];	//BriefDIR_XY[24][8][4]
	Base2->x += BriefDIR_XY[ secID ][dirID_A][2];	Base2->y += BriefDIR_XY[ secID ][dirID_A][3];
	if( Base2->x == BX0 || Base2->y == BY0 ) { Base2->x += 10 ;  Base2->y += 10 ; }

	// дополнительная информация по сектору - задачи + квесты (инф-я справа)
	char path2[256] , zero[10] , secname[256] ; 
	if( secID < 10 ) { sprintf(zero, "0"); } else { sprintf(zero, ""); }
	if( AttackNationID ==  gi_bmSAVE_Nation ) {
		sprintf( path2, "Missions\\StatData\\Sector%s%d\\SecObj%s%d.txt", zero, secID, zero, secID ); 
	}
	else { 
		sprintf( path2, "Missions\\StatData\\Sector%s%d\\SecObjDef%s%d.txt", zero, secID, zero, secID ); 
	}
	if( DefendNationID == -1 )  sprintf( path2, "Missions\\StatData\\SecRevoltObj.txt" );

	_str		text;
	sprintf( secname, GetTextByID( "#CBT_SectorName" ) , ROOT_SCENARIO.m_pSect->GetSectorName( secID ) ); 
	text += secname;

	DString		a_DS , a_DS2;
	a_DS.ReadFromFile(path2);
	bool	quit = false;		int  line = 0;
	do {
		line++;
		a_DS.ExtractLine(a_DS2);
		text += a_DS2.pchar() ;
		if( a_DS2 == "[END]" || a_DS2.pchar()[0] == 0 )  quit = true; // end
	} while ( !quit && line < 10000 );

	//DialogsDesk*	ObjDesk = a_dsMenu.AddDialogsDesk( 332, 460, 622, 215 , "BD" );//"EmptyBorder"); // AddDialogsDesk( 332, 424, 622, 250 ,"EmptyBorder");
	DialogsDesk*	ObjDesk = a_dsMenu.AddDialogsDesk( 333, 460, 595, 215 , "BD" );//"EmptyBorder"); // AddDialogsDesk( 332, 424, 622, 250 ,"EmptyBorder");
	TextButton*		BriefText = ObjDesk->addTextButton(NULL, 17, 17 ,"",&fonMenuText[0],&fonMenuText[0],&fonMenuText[0], 0 );
	BriefText->MaxWidth = ObjDesk->x1 - ObjDesk->x -24 /*-51*/ ;
	BriefText->SetMessage( text.pchar() ) ;

	// ********************* attacker forces *********************
	TextButton*		AttForce = a_dsMenu.addTextButton(NULL, X0, Y0 , GetTextByID("#AttackForce"),&fonMenuTitle2[1],&fonMenuTitle2[1],&fonMenuTitle2[1],0); //Rfont,Rfont,Rfont,0);
	// attacker country
	TextButton*	Country1 = a_dsMenu.addTextButton(NULL, X0, 122 , "",&fonMenuText[0],&fonMenuText[0],&fonMenuText[0],0); //font,font,font,0);
	Country1->SetMessage( ROOT_SCENARIO.m_pPlayer[AttackNationID]->GetPlayerName() ); 
	// attacker flag
	GPPicture*	Flag11 = a_dsMenu.addGPPicture(NULL, X0 +200 , 107 ,a_lgpFlags.GPID, AttackNationID+1 );


	// ********************* attacker hero
	CHero*		pAttackHero = ROOT_SCENARIO.m_pPlayer[ AttackNationID ]->GetHeroInSector( secID ) ;
	LocalGP*	pLGP	= ROOT_SCENARIO.m_pHp->getLGPSmallP( AttackNationID );
	GPPicture*	Hero1	= a_dsMenu.addGPPicture(NULL, X0 +3 , 158 , pLGP->GPID, pAttackHero->m_inHeroID ); //GPPicture*		Hero1 = a_dsMenu.addGPPicture(NULL, X0 +4 , HeroBack1->y +4 , pLGP->GPID, pAttackHero->m_inHeroID );
	// attacker hero frame
	LocalGP		a_lgpElems("Interf3\\TotalWarGraph\\bmElements");
	GPPicture*	HeroBack1 =	a_dsMenu.addGPPicture(NULL, Hero1->x -4 , Hero1->y -4 ,a_lgpElems.GPID, 2 ) ;

	// attacker nation flag
	GPPicture*	Flag1	= a_dsMenu.addGPPicture(NULL, HeroBack1->x +10 , HeroBack1->y1 -32 ,a_lgpFlags.GPID, pAttackHero->m_inPlayerID+1 );
	Flag1->Diffuse = 0x9FFFFFFF;

	Y0 = HeroBack1->y +6; //5;

	// name
	TextButton*	Name1	= a_dsMenu.addTextButton(NULL, HeroBack1->x +131 , Y0 ,"",Rfont,Rfont,Rfont,0);
	Name1->SetMessage( pAttackHero->GetName() ); 
	// rank
	TextButton*	Rank1   = a_dsMenu.addTextButton(NULL, Name1->x , Y0 +16 , "",&SpecialBlackFont,&SpecialBlackFont,&SpecialBlackFont,0);
	Rank1->SetMessage( GetRankByID( pAttackHero->GetRank() ) );

	// stats
	LocalGP		a_lgpAwards("Interf3\\TotalWarGraph\\lva_Awards");
	GPPicture*	Stat1	= a_dsMenu.addGPPicture(NULL,  Name1->x, Y0 +BIGMAP_STATS_DIST ,a_lgpAwards.GPID, pAttackHero->m_inStatID );
	//Stat1->y = Y0 +25 + 2*(MAX_H_STATUS - pAttackHero->m_inStatID)/10;

	// expa
	LocalGP		a_lgpExpBar("Interf3\\TotalWarGraph\\bmExpBar");
	TextButton*	Expa1	= a_dsMenu.addTextButton(NULL,  Name1->x, Y0 +68 , "" ,&SmallGrayFont1,&SmallGrayFont1,&SmallGrayFont1,0);
	char s[256];
	sprintf(s, BIGMAP_EXPSFORMAT , GetTextByID("#Experience") , pAttackHero->m_inExperience ) ;
	Expa1->SetMessage( s );
	GPPicture*	ExpBack1 = a_dsMenu.addGPPicture(NULL, Name1->x, Y0 +82 , a_lgpExpBar.GPID, 0 ) ;
	
	DialogsDesk* ExpBar1	= a_dsMenu.AddDialogsDesk( ExpBack1->x, ExpBack1->y, 10 , 10 ,"NullBorder");
	GPPicture*	 ExpBarPIC1 = ExpBar1->addGPPicture(NULL,0,0, a_lgpExpBar.GPID , 1 ) ;

	int  next_lvl = 0 ;
	int iExpPercent = GetExpPercentByRankID( pAttackHero->m_inExperience, pAttackHero->GetRank() , next_lvl );
	ExpBar1->x1 = ExpBar1->x + iExpPercent ; 

	_str	a_Hint;	
	a_Hint += "{FS}{C FF502515}" ;
	if( next_lvl != 0xFFFF ) { a_Hint += GetTextByID("#CHINT_Next_Exp_Level");  a_Hint += " ";  a_Hint += tRed; a_Hint += next_lvl; }
	else					 { a_Hint += tRed; a_Hint += GetTextByID( "#CHINT_MaxExpLevel" ) ; }
	DYNHINT( ExpBack1 , a_Hint.pchar() );

	// attacker Vict/Batl Num
	TextButton*		VictN1 = a_dsMenu.addTextButton(NULL, Name1->x , Y0 +54 ,"",&SmallGrayFont1,&SmallGrayFont1,&SmallGrayFont1,0);
	sprintf(s, BIGMAP_VICTFORMAT , GetTextByID("#Victories"), pAttackHero->m_iVictN, pAttackHero->m_iBatlN );
	VictN1->SetMessage( s );

	// attacker squads
	TextButton*		Squads1 = a_dsMenu.addTextButton(NULL, Name1->x , Y0 +110 ,"",&GrayFont,&GrayFont,&GrayFont,0);
	sprintf(s, BIGMAP_SQNNFORMAT1 , GetTextByID("#SquadsN") , pAttackHero->GetARMY_NSQ() );
	Squads1->SetMessage( s ); 


	//X0 -= 3 ;
	// ********************* BORDER border_GB 17
	LocalGP			a_lgpBorder("Interf3\\TotalWarGraph\\bmElements");
	GPPicture*		Border = a_dsMenu.addGPPicture(NULL, X0, 344 ,a_lgpBorder.GPID, 29 );
	// ********************* BORDER border_GB 17


	Y0 = 489;

	// ********************* defender forces *********************
	TextButton*		DefForce = a_dsMenu.addTextButton(NULL, AttForce->x, Y0 ,GetTextByID("#DefendForce"),&fonMenuTitle2[1],&fonMenuTitle2[1],&fonMenuTitle2[1],0); //&fonMenuTitle[1] //font,font,font,0);
	// defender country
	TextButton*		Country2 = a_dsMenu.addTextButton(NULL, Country1->x , 512 ,"",&fonMenuText[0],&fonMenuText[0],&fonMenuText[0],0); //fonMenuTitle //font,font,font,0);
	if( DefendNationID != -1 )  Country2->SetMessage( ROOT_SCENARIO.m_pPlayer[ DefendNationID ]->GetPlayerName() ); 
	else						Country2->SetMessage( GetTextByID("#REBEL") ); 

	// defender flag
	GPPicture*		Flag2 = a_dsMenu.addGPPicture(NULL, Flag11->x, 497 ,a_lgpFlags.GPID, DefendNationID+1 );
	if( DefendNationID == -1 )   Flag2->Visible = false ;

	// ********************* defender hero
	int SecDef = ROOT_SCENARIO.m_pSect->GetSecDefence( secID ) ; 
	int SecPop = ROOT_SCENARIO.m_pSect->GetSecPopulation( secID ) ; 
	int sqNUM = GetSQNumByDefenceID( SecDef , DefendNationID , SecPop , secID  );

	int XX = AttForce->x ;
	int YY = Country2->y1 +2*dy +4 ;
	CHero*	pDefendHero = ROOT_SCENARIO.GetHeroInSect( secID , AttackNationID );
	if( pDefendHero ) 
	{
		// portrait
		pLGP  = ROOT_SCENARIO.m_pHp->getLGPSmallP( pDefendHero->m_inPlayerID ) ;
		GPPicture*	Hero2 = a_dsMenu.addGPPicture(NULL, Hero1->x , 548 ,pLGP->GPID,pDefendHero->m_inHeroID);
		// defender hero frame
		LocalGP		a_lgpElems("Interf3\\TotalWarGraph\\bmElements");
		GPPicture*  HeroBack2 =	a_dsMenu.addGPPicture(NULL, HeroBack1->x , Hero2->y -4 ,a_lgpElems.GPID, 2  ) ;

		// defender nation flag
		GPPicture*	Flag2	= a_dsMenu.addGPPicture(NULL, HeroBack2->x +10 , HeroBack2->y1 -32 ,a_lgpFlags.GPID, pDefendHero->m_inPlayerID+1 );
		Flag2->Diffuse = 0x9FFFFFFF;

		Y0 = HeroBack2->y +6; //5; 	

		// name	
		TextButton*	Name2 = a_dsMenu.addTextButton(NULL, Name1->x , Y0 ,"",Rfont,Rfont,Rfont,0);
		Name2->SetMessage( pDefendHero->GetName() ); 
		// rank
		TextButton*	Rank2   = a_dsMenu.addTextButton(NULL, Rank1->x , Y0 +16 , "",&SpecialBlackFont,&SpecialBlackFont,&SpecialBlackFont,0);
		Rank2->SetMessage( GetRankByID( pDefendHero->GetRank() ) );

		// stats
		GPPicture*	Stat2	= a_dsMenu.addGPPicture(NULL,  Name2->x , Y0 +BIGMAP_STATS_DIST ,a_lgpAwards.GPID, pDefendHero->m_inStatID );
		//Stat2->y = Y0 +25 + 2*(MAX_H_STATUS - pDefendHero->m_inStatID)/10;

		// expa
		LocalGP		a_lgpExpBar("Interf3\\TotalWarGraph\\bmExpBar");
		TextButton*	Expa2 =  a_dsMenu.addTextButton(NULL, Name2->x , Y0 +68 ,"",&SmallGrayFont1,&SmallGrayFont1,&SmallGrayFont1,0);/*GrayFont SmallGrayFont*/
		char s2[256];
		sprintf(s2, BIGMAP_EXPSFORMAT , GetTextByID("#Experience") , pDefendHero->m_inExperience ) ;
		Expa2->SetMessage( s2 );
		GPPicture*	ExpBack2 = a_dsMenu.addGPPicture(NULL, ExpBack1->x , Y0 +82 ,a_lgpExpBar.GPID,0) ;

		DialogsDesk* ExpBar2    = a_dsMenu.AddDialogsDesk( ExpBar1->x , ExpBack2->y ,10,10,"NullBorder");
		GPPicture*	 ExpBarPIC2 = ExpBar2->addGPPicture(NULL, 0,0 ,a_lgpExpBar.GPID, 1 ) ;

		int  next_lvl = 0;
		int  iExpPercent = GetExpPercentByRankID( pDefendHero->m_inExperience, pDefendHero->GetRank() , next_lvl );
		ExpBar2->x1 = ExpBar2->x + iExpPercent ; 

		_str	a_Hint;	
		a_Hint += "{FS}{C FF502515}" ;
		if( next_lvl != 0xFFFF ) { a_Hint += GetTextByID( "#CHINT_Next_Exp_Level" ); a_Hint += " "; a_Hint += tRed; a_Hint += next_lvl; }
		else					 { a_Hint += tRed; a_Hint += GetTextByID( "#CHINT_MaxExpLevel" ) ; }
		DYNHINT( ExpBack2 , a_Hint.pchar() );

		// defender Vict/Batl Num
		TextButton*		VictN2 = a_dsMenu.addTextButton(NULL, Name2->x , Y0 +54 ,"",&SmallGrayFont1,&SmallGrayFont1,&SmallGrayFont1,0);
		sprintf(s, BIGMAP_VICTFORMAT , GetTextByID("#Victories"), pDefendHero->m_iVictN, pDefendHero->m_iBatlN );
		VictN2->SetMessage( s );

		XX	   = Name2->x ;		
		YY	   = HeroBack2->y1 -15 ;		
		sqNUM += pDefendHero->GetARMY_NSQ();
		sprintf(s, BIGMAP_SQNNFORMAT1 , GetTextByID("#SquadsN") , sqNUM );
	}
	else {
		if( DefendNationID != -1 )  sprintf(s, "%s  {R FF000000}%d{C}", GetTextByID("#Garrison") ,	   sqNUM );
		else						sprintf(s, "%s  {R FF000000}%d{C}", GetTextByID("#RebelGarrison") , sqNUM );
	}

	// defender squads
	TextButton*		Squads2 = a_dsMenu.addTextButton(NULL, XX , YY , "", &GrayFont,&GrayFont,&GrayFont,0);
	Squads2->SetMessage( s );



	// *********************  Buttons START & STEP BACK ********************* 
	int			OKx = 287 +2 ,	OKy = 709 +4 ;
	int			NOx = 530 +2 ,	NOy = OKy ;
	LocalGP		a_lgpOK("Interf3\\TotalWarGraph\\dMessage");
	LocalGP		a_lgpOK_F("Interf3\\elements\\button_back");

	GPPicture*	pgpbOKfon  = a_dsMenu.addGPPicture(NULL, 0 , 680 ,a_lgpOK_F.GPID, 1 );
	GP_TextButton*  pgpbOK = a_dsMenu.addGP_TextButton(NULL,OKx,OKy,GetTextByID("#CBB_StartMiss"),a_lgpOK.GPID, 6 ,&fonMenuText[3],&fonMenuText[2]);//ActFont,PasFont);
	GP_TextButton*  pgpbNO = a_dsMenu.addGP_TextButton(NULL,NOx,NOy,GetTextByID("#CBB_StepBack"), a_lgpOK.GPID, 6 ,&fonMenuText[3],&fonMenuText[2]);//ActFont,PasFont);
	pgpbOK->FontDy		= pgpbNO->FontDy	  = -1 ;
	pgpbOK->OnUserClick = pgpbNO->OnUserClick = &MMItemChoose;
	pgpbOK->UserParam	= mcmOk;
	pgpbNO->UserParam	= mcmCancel;

	pgpbOK->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );	
	pgpbOK->AssignSound( "INTERF_NAGAL", CLICK_SOUND );	

	pgpbNO->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
	pgpbNO->AssignSound( "INTERF_NAGAL", CLICK_SOUND );

	_str	a_Hint1 , a_Hint2;	a_Hint1 += "{FS}{C FF502515}";	a_Hint2 += "{FS}{C FF502515}";
	a_Hint1+=GetTextByID("#CHINT_StartMis"); a_Hint1+=" "; a_Hint1+=GetTextByID("#CHINT_ENTERKEY"); DYNHINT(pgpbOK,a_Hint1.pchar());
	a_Hint2+=GetTextByID("#CHINT_StepBack"); a_Hint2+=" "; a_Hint2+=GetTextByID("#CHINT_ESCKEY");   DYNHINT(pgpbNO,a_Hint2.pchar());


	//CheckBox* CHK = new CheckBox();
	//a_dsMenu.AddDialog( CHK );			
	//CHK->Setx( 800 );		CHK->Sety( 723 );
	//CHK->State = GSets.CGame.ArcadeMode;
	//_str  a_text;						
	//a_text += GetTextByID("#MO_ArcadeMode");
	//CHK->Message = znew(char,strlen( a_text.pchar() )+1);
	//strcpy(CHK->Message,a_text.pchar()); //"ARCADE"

	////CHK->SetMessage( "ARCADE" );
	////  a_dsMenu.addGP_CheckBox(NULL, 800, 709 +4, "ARCADE",&OrangeFont,&OrangeFont,0, false, GP, 0,0,0);

	int  arcX = 337 , arcY = 425; //arcX = 332 , arcY = 429; // arcX = 680 , arcY = 650;
	DialogsDesk* ddArcadeMode = addArcadeMode(&a_dsMenu, arcX , arcY );


	bool	choose = true;
	
	UnPress();					//Global
	KeyPressed = LastKey = 0;   //Global

	ItemChoose = - 1;

	do { // SecMap->Diffuse
		ProcessMessages();
		StdKeys();
		a_dsMenu.ProcessDialogs();
		//a_dsMenu.x = a_dsMenu.y = 0;
		a_dsMenu.RefreshView();
		//GSets.CGame.ArcadeMode = CHK->State;
	} while ( ItemChoose!=mcmOk && ItemChoose!=mcmCancel );
	
	//GSets.CGame.ArcadeMode = CHK->State;

	if( ItemChoose == mcmCancel )	{ choose = false; }
	ItemChoose = - 1;
	return choose;
}
//-----------------------------------------------------------------------------------------------------------------------------
bool	ProcessBigMapStatistics()
{
	//EndMissionData.LoadXML();// !!!!! TEEEEEEEEEMPPPP !!!!!

	int  X0 = 77 , Y0 = 87 ;	// 80, 112	
	int  dy = 10 , dx = 25 ;
	RLCFont* font = &BlackFont;	RLCFont* Rfont = &RedFont;	RLCFont* ActFont = &WhiteFont;	RLCFont* PasFont = &YellowFont;

	int  secID			= MissionInitData.getSector_ID();
	int  AttackNationID	= MissionInitData.getAttackerNation();
	int  DefendNationID	= MissionInitData.getDefenderNation();

	int  WinnerNat		= EndMissionData.WinnerNI;
	
	bool Win = false;	
	if( WinnerNat == gi_bmSAVE_Nation ) Win = true;	// победил ли игрок (да/нет)

	CHero*	pAttackHero	= pAtackerHero ; 
	CHero*	pDefendHero = pDefendeHero ; 
	if( !pAttackHero )	return false; 

	DialogsSystem	a_dsMenu(0,0);													//новое меню
	// Background
	SQPicture		a_sqpBack("Interf3\\TotalWarGraph\\turn_map_statistics.bmp");	//Фон
	a_dsMenu.addPicture(NULL,0,0,&a_sqpBack,&a_sqpBack,&a_sqpBack);
	// Main Desks
	DialogsDesk*	pddDesk1 = a_dsMenu.AddDialogsDesk( 0, 0, 1024, 768, "NullBorder");// attacker desk 1
	DialogsDesk*	pddDesk2 = a_dsMenu.AddDialogsDesk( 0, 0, 1024, 768, "NullBorder");// defender desk 2
	// Head 
	TextButton* ptbHead  = a_dsMenu.addTextButton(NULL, 512, 47 /*36*/ ,GetTextByID("#CWT_StatsHead"),&fonMenuTitle[3],&fonMenuTitle[3],&fonMenuTitle[3],1);//NEED NEW FONTS!!!
	// Status picture 
	LocalGP			a_lgpStat("Interf3\\TotalWarGraph\\bmStatistics"); // x,y = 79,130
	GPPicture*		Stat	= pddDesk1->addGPPicture(NULL,79,130,a_lgpStat.GPID, WinnerNat==AttackNationID?0:1 ); 
	GPPicture*		Stat2	= pddDesk2->addGPPicture(NULL,79,130,a_lgpStat.GPID, WinnerNat==DefendNationID?0:1 ); //Win?0:1 ); 
	
	// hero & country info 
	LocalGP		a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");
	LocalGP		a_lgpBordr("Interf3\\elements\\portraits_border");
	LocalGP*	pLGP  = ROOT_SCENARIO.m_pHp->getLGPSmallP( AttackNationID );
	// Attack
	_str		a_name;
	if( AttackNationID == WinnerNat )	{ a_name += GetTextByID("#CWT_Stats_Winner"); }
	else								{ a_name += GetTextByID("#CWT_Stats_Loser"); }
	a_name += pAttackHero->GetName() ;
	a_name += " - " ;
	a_name += ROOT_SCENARIO.m_pPlayer[ pAttackHero->GetPlayerID() ]->GetPlayerName() ;
	TextButton* ptbAttackName  = pddDesk1->addTextButton(NULL, 78, 368, a_name.pchar() ,&fonMenuTitle2[1],&fonMenuTitle2[1],&fonMenuTitle2[1],0);//NEED NEW FONTS!!! &fonMenuTitle[1]
	GPPicture*  gppAttackFoto  = pddDesk1->addGPPicture(NULL,  78, 405, pLGP->GPID, pAttackHero->m_inHeroID );
	GPPicture*	gppAttFotoBak  = pddDesk1->addGPPicture(NULL,  74, 401, a_lgpBordr.GPID, 0 ) ;
	GPPicture*  gppFlag		   = pddDesk1->addGPPicture(NULL, 472, 362, a_lgpFlags.GPID, pAttackHero->GetPlayerID()+1 );
	// Defend
	if( pDefendHero ) {
		pLGP	= ROOT_SCENARIO.m_pHp->getLGPSmallP( pDefendHero->m_inPlayerID ) ;
		_str    a_name2;
		if( DefendNationID == WinnerNat )	{ a_name2 += GetTextByID("#CWT_Stats_Winner"); }
		else								{ a_name2 += GetTextByID("#CWT_Stats_Loser"); }
		a_name2 += pDefendHero->GetName() ;
		a_name2 += " - " ;
		a_name2 += ROOT_SCENARIO.m_pPlayer[ pDefendHero->GetPlayerID() ]->GetPlayerName() ;
		TextButton* ptbDefendName  = pddDesk2->addTextButton(NULL, 78, 368, a_name2.pchar() ,&fonMenuTitle2[1],&fonMenuTitle2[1],&fonMenuTitle2[1],0);//NEED NEW FONTS!!!
		GPPicture*  gppDefendFoto  = pddDesk2->addGPPicture(NULL,  78, 405, pLGP->GPID,pDefendHero->m_inHeroID);
		GPPicture*	gppDefFotoBak  = pddDesk2->addGPPicture(NULL,  74, 401, a_lgpBordr.GPID, 0 ) ;
		GPPicture*  gppFlag2	   = pddDesk2->addGPPicture(NULL, 472, 362, a_lgpFlags.GPID, pDefendHero->GetPlayerID()+1 );
	}


	// EXPA
	int  ExpX  = 218 ,  ExpY = 412 ,  ExpDy = 17 ;
	// text
	TextButton* ptbExpTxt1 = a_dsMenu.addTextButton(NULL, ExpX, ExpY ,GetTextByID("#CWT_StatsExps"),font,font,font,0);
	ExpY += 22;
	TextButton* ptbExpTxt2 = a_dsMenu.addTextButton(NULL, ExpX, ExpY ,GetTextByID("#CWT_StatsForm"),&GrayFont,&GrayFont,&GrayFont,0);

	ExpY += ExpDy;
	//TextButton* ptbExpTxt3 = a_dsMenu.addTextButton(NULL, ExpX, ExpY ,GetTextByID("#CWT_StatsSect"),&GrayFont,&GrayFont,&GrayFont,0);
	TextButton* ptbExpTxt31 = pddDesk1->addTextButton(NULL, ExpX, ExpY ,GetTextByID( "#CWT_StatsSect"  ),&GrayFont,&GrayFont,&GrayFont,0);
	TextButton* ptbExpTxt32 = pddDesk2->addTextButton(NULL, ExpX, ExpY ,GetTextByID( "#CWT_StatsSect2" ),&GrayFont,&GrayFont,&GrayFont,0);

	ExpY += ExpDy;
	TextButton* ptbExpTxt4 = a_dsMenu.addTextButton(NULL, ExpX, ExpY ,GetTextByID("#CWT_StatsHero"),&GrayFont,&GrayFont,&GrayFont,0);
	ExpY += ExpDy;
	TextButton* ptbExpTxt5 = a_dsMenu.addTextButton(NULL, ExpX, ExpY ,GetTextByID("#CWT_StatsQExp"),&GrayFont,&GrayFont,&GrayFont,0);
	ExpY += 22 ;
	TextButton* ptbExpTxt6 = a_dsMenu.addTextButton(NULL, ExpX, ExpY ,GetTextByID("#CWT_StatsTotalExp"),Rfont,Rfont,Rfont,0);
	

	// exp-values
	int  TotalExp   = ROOT_SCENARIO.FormKilledNum  * DisbandSQExp ;	// attaker
	int  TotalExp2  = ROOT_SCENARIO.FormKilledNum2 * DisbandSQExp ;	// defender
	// TotalExp		= ROOT_SCENARIO.FormKilledNum * DisbandSQExp + SecCaptExp + HeroDefExp + ROOT_SCENARIO.QuestEXP ;
	if( MissionInitData.DefenderNation_ID == -1 || MissionInitData.DefenderNation_ID == 6 )
		TotalExp   = ROOT_SCENARIO.FormKilledNum  * QuestDisbSquadExp * 2 ;


	int  AddSectExp	  = 0 ,  AddHeroExp   = 0 ;
	int  AddSectExp2  = 0 ,  AddHeroExp2  = 0 ;
	// player - attacker
	if( AttackNationID == gi_bmSAVE_Nation ) {
		if(Win)		{	AddSectExp   = SecCaptExp;	}		// Экспа за захват сектора
		else		{	AddSectExp2  = SecCaptExp;	}

		if(pDefendHero) {
			if(Win)	{	AddHeroExp   = HeroDefExp;	}		// Экспа за победу над героем
			else	{	AddHeroExp2  = HeroDefExp;	}
		}
	}
	// player - defender
	if( DefendNationID == gi_bmSAVE_Nation && pDefendHero ) {
		if(Win)		{	AddSectExp2  = SecCaptExp;	}		// Экспа за захват сектора
		else		{	AddSectExp   = SecCaptExp;	}

		if(pAttackHero) { 
			if(Win) {	AddHeroExp2  = HeroDefExp;	}		// Экспа за победу над героем
			else	{	AddHeroExp   = HeroDefExp;	}
		}
	}
	TotalExp		+= AddSectExp  + AddHeroExp  + ROOT_SCENARIO.QuestEXP ;
	TotalExp2		+= AddSectExp2 + AddHeroExp2 ;


	// Attack
	ExpX = 489 ,  ExpY = 412  ;
	// values
	_str  ExpsTxt;
	ExpsTxt += pAttackHero->GetExperience() ;
	ExpsTxt += "{C FF640100} +" ;
	ExpsTxt += TotalExp ; 							
	TextButton* ptbExpVal1 = pddDesk1->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
	ptbExpVal1->SetMessage( ExpsTxt.pchar() ) ;

	ExpY += 22;
	_str  FormTxt;
	FormTxt += ROOT_SCENARIO.FormKilledNum ;
	FormTxt += "x" ;
	//FormTxt += DisbandSQExp ;
	if( MissionInitData.DefenderNation_ID == -1 || MissionInitData.DefenderNation_ID == 6 ) {
		FormTxt += 2 * QuestDisbSquadExp ; }
	else {
		FormTxt += DisbandSQExp ;
	}
	TextButton* ptbExpVal2 = pddDesk1->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
	ptbExpVal2->SetMessage( FormTxt.pchar() ) ;

	ExpY += ExpDy;
	_str  SectTxt;
	SectTxt += AddSectExp ;		
	TextButton* ptbExpVal3 = pddDesk1->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
	ptbExpVal3->SetMessage( SectTxt.pchar() ) ;

	ExpY += ExpDy;
	_str  HeroTxt;
	HeroTxt += AddHeroExp ;		
	TextButton* ptbExpVal4 = pddDesk1->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
	ptbExpVal4->SetMessage( HeroTxt.pchar() ) ;

	ExpY += ExpDy;
	_str  QuestTxt;
	QuestTxt += ROOT_SCENARIO.QuestEXP  ;		/*   200/1000 (1/5)   */
	QuestTxt += "/" ;
	QuestTxt += ( ROOT_SCENARIO.QuestNum * QuestCompliteExp + ROOT_SCENARIO.QuestSQNum * QuestDisbSquadExp ) ;
	QuestTxt += " (" ;
	QuestTxt += ROOT_SCENARIO.QuestComplete ;
	QuestTxt += "/" ;
	QuestTxt += ROOT_SCENARIO.QuestNum ;
	QuestTxt += ")" ;

	TextButton* ptbExpVal5 = pddDesk1->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
	ptbExpVal5->SetMessage( QuestTxt.pchar() ) ;

	ExpY += 22 ;
	_str  TotalExpTxt;
	TotalExpTxt += TotalExp  ;
	TextButton* ptbExpVal6 = pddDesk1->addTextButton(NULL, ExpX, ExpY , " " ,Rfont,Rfont,Rfont,2);
	ptbExpVal6->SetMessage( TotalExpTxt.pchar() ) ;

	// Defend
	if( pDefendHero ) {
		ExpX = 489 ,  ExpY = 412  ;
		// values
		_str  ExpsTxt2;
		ExpsTxt2 += pDefendHero->GetExperience() ;			
		ExpsTxt2 += "{C FF640100} +" ;
		ExpsTxt2 += TotalExp2 ;
		TextButton* ptbExpVal12 = pddDesk2->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
		ptbExpVal12->SetMessage( ExpsTxt2.pchar() ) ;

		ExpY += 22;
		_str  FormTxt2;
		FormTxt2 += ROOT_SCENARIO.FormKilledNum2 ; 
		FormTxt2 += "x" ;
		FormTxt2 += DisbandSQExp ;
		TextButton* ptbExpVal22 = pddDesk2->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
		ptbExpVal22->SetMessage( FormTxt2.pchar() ) ;

		ExpY += ExpDy;
		_str  SectTxt2;
		SectTxt2 += AddSectExp2 ;
		TextButton* ptbExpVal32 = pddDesk2->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
		ptbExpVal32->SetMessage( SectTxt2.pchar() ) ;

		ExpY += ExpDy;
		_str  HeroTxt2;
		HeroTxt2 += AddHeroExp2 ;
		TextButton* ptbExpVal42 = pddDesk2->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
		ptbExpVal42->SetMessage( HeroTxt2.pchar() ) ;

		ExpY += ExpDy;
		_str  QuestTxt2;
		QuestTxt2 += "0/0" ;
		TextButton* ptbExpVal52 = pddDesk2->addTextButton(NULL, ExpX, ExpY , " " ,font,font,font,2);
		ptbExpVal52->SetMessage( QuestTxt2.pchar() ) ;

		ExpY += 22 ;
		_str  TotalExpTxt2;
		TotalExpTxt2 += TotalExp2 ;
		TextButton* ptbExpVal62 = pddDesk2->addTextButton(NULL, ExpX, ExpY , " " ,Rfont,Rfont,Rfont,2);
		ptbExpVal62->SetMessage( TotalExpTxt2.pchar() ) ;
	}



	// RESOURCES
	TextButton* ptbResHead = a_dsMenu.addTextButton(NULL, 87, 550 , GetTextByID("#CWT_StatsResHead") ,&GrayFont,&GrayFont,&GrayFont,0);

	TextButton*   pgppIncRes;	TextButton*   pgppOutRes;	TextButton*   pgppTotal;
	TextButton*   pgppIncRes2;	TextButton*   pgppOutRes2;	TextButton*   pgppTotal2;
	int   list[7] = { WOOD, FOOD, STONE, GOLD, IRON, COAL } ;
	int   resX    = 0,  curR = 0;

	for(int res=0; res <= COAL ; res++)
	{ 
		curR = list[res] ;
		resX = 145 + 69*res ; //resX = 132 + 69*res ;

		// PAGE 1	- income		// Attack
		_str  incRes;
		incRes += "+" ;
		incRes += ROOT_SCENARIO.IncomeRes[ curR ] ;	
		pgppIncRes = pddDesk1->addTextButton(NULL, resX , 616 ,"0",font,font,font,2);
		pgppIncRes->SetMessage( incRes.pchar() );
		//			- out
		_str  outRes;		int R = 0;	if( gi_bmSAVE_Nation != MissionInitData.AttackerNation_ID ) R = -ROOT_SCENARIO.AddAIRes[curR];
		R += EndMissionData.AttackerRes.GetResource(curR) -smsSAVE.AttackerResource.GetResource(curR) -ROOT_SCENARIO.IncomeRes[curR] ;
		if( R < 0 )		{ outRes += R    ; } 
		else			{ outRes += "-0" ; }
		pgppOutRes = pddDesk1->addTextButton(NULL, resX , pgppIncRes->y +18 ,"0",font,font,font,2);
		pgppOutRes->SetMessage( outRes.pchar() );
		//			- res amount
		_str  Total;
		int  tR = EndMissionData.AttackerRes.GetResource( curR );
		if( MissionInitData.AttackerNation_ID == gi_bmSAVE_Nation || tR == 0 )	{ Total += tR  ; }
		else																	{ Total += "?" ; }
		pgppTotal = pddDesk1->addTextButton(NULL,  resX , pgppOutRes->y +18 ,"0",Rfont,Rfont,Rfont,2);
		if( MissionInitData.AttackerNation_ID == gi_bmSAVE_Nation || 
			(MissionInitData.AttackerNation_ID != gi_bmSAVE_Nation && tR > 999 ) )  pgppTotal->SetMessage( Total.pchar() );
		//pgppTotal->SetMessage( Total.pchar() );

		// PAGE 2	- income		// Defend
		if( pDefendHero ) {
			_str  incRes2;
			incRes2 += "+" ;
			incRes2 += ROOT_SCENARIO.IncomeRes2[ curR ] ;
			pgppIncRes2 = pddDesk2->addTextButton(NULL, resX , pgppIncRes->y ,"0",font,font,font,2);
			pgppIncRes2->SetMessage( incRes2.pchar() );
			//			- out
			_str  outRes2;		int  R2 = 0;	if( gi_bmSAVE_Nation != MissionInitData.DefenderNation_ID  ) R2 = -ROOT_SCENARIO.AddAIRes[curR];
			R2 += EndMissionData.DefenderRes.GetResource(curR) -smsSAVE.DefenderResource.GetResource(curR) -ROOT_SCENARIO.IncomeRes2[curR] ;
			if( R2 < 0 )	{ outRes2 += R2   ; }
			else			{ outRes2 += "-0" ; }
			pgppOutRes2 = pddDesk2->addTextButton(NULL, resX , pgppOutRes->y ,"0",font,font,font,2);
			pgppOutRes2->SetMessage( outRes2.pchar() );
			//			- res amount
			_str  Total2;
			int  tR = EndMissionData.DefenderRes.GetResource( curR );
			if( MissionInitData.DefenderNation_ID == gi_bmSAVE_Nation || tR == 0 )	{ Total2 += tR  ; }
			else																	{ Total2 += "?" ; }
			pgppTotal2 = pddDesk2->addTextButton(NULL,  resX , pgppTotal->y ,"0",Rfont,Rfont,Rfont,2);
			if(  MissionInitData.DefenderNation_ID == gi_bmSAVE_Nation  || 
				(MissionInitData.DefenderNation_ID != gi_bmSAVE_Nation && tR > 999 ) ) pgppTotal2->SetMessage( Total2.pchar() );
			//pgppTotal2->SetMessage( Total2.pchar() );
		}
	}


	// KILLED head
	TextButton* ptbFormLost = a_dsMenu.addTextButton(NULL, 535, 368, GetTextByID("#CWT_StatsLostHead")  ,&GrayFont,&GrayFont,&GrayFont,0);
	TextButton* ptbFLostTot = a_dsMenu.addTextButton(NULL, 882, 368, GetTextByID("#CWT_StatsLostTotal") ,font,font,font,2);

	// KILLED PAGE 1
	LocalGP			lgpElems( "Interf3\\TotalWarGraph\\bmElements" );
	DialogsDesk*	pddKill  = pddDesk1->AddDialogsDesk( 523, 385, 430, 290, "EmptyBorder");//desk 1
		
	// Attack
	int  UX = 15 ,  UY = 15 ,  UN = 0 ,     TotalLost = 0 ;

	for(int br=0; br < 3 ; br++) 
		for(int sq=0 ; sq < pAttackHero->ARMY.GetAmountOfElements() ; sq++) 
		if( GetObjectBranch( pAttackHero->ARMY[sq]->getTypeGlobID() ) == br ) { 

		int  sqN0   = 0;											// кол-во отр. до миссии
		int  sqN1	= 0;											// кол-во отр. после миссии
		int  uid	= pAttackHero->ARMY[ sq ]->getTypeGlobID();		// тип юнитов

		for(int i=0; i < smsSAVE.AttackerForce.GetAmountOfElements() ; i++) 
			if( uid   == smsSAVE.AttackerForce[i]->TypeIndex_ID ) {
				sqN0  += smsSAVE.AttackerForce[i]->nSquarnd;
			}
		for(int i=0; i < EndMissionData.AttackerArmy.GetAmountOfElements() ; i++)
			if( uid   == EndMissionData.AttackerArmy[i]->inTypeID ) { 
				sqN1  += EndMissionData.AttackerArmy[i]->inAmount ; 
			}
		if( sqN0 > sqN1 && uid >=0 ) {	/*find &&*/
			int  fid ,  sid ;
			fid = NATIONS[0].Mon[uid]->newMons->MinIconFile ;		//InMenuIconFile;	
			sid = NATIONS[0].Mon[uid]->newMons->MinIconIndex ;		//InMenuIconIndex;	
			if( fid == 0xFFFF ) {									//|| sid == 0xFFFF ) { 
				fid = NATIONS[0].Mon[uid]->newMons->IconFileID;
				sid = NATIONS[0].Mon[uid]->newMons->IconID;
			}
			if( fid == 0xFFFF ) { 
				fid = NATIONS[0].Mon[uid]->newMons->InMenuIconFile;
				sid = NATIONS[0].Mon[uid]->newMons->InMenuIconIndex;
			}
			if( fid != 0xFFFF && sid != 0xFFFF ) { 
				GPPicture*   UPic	= pddKill->addGPPicture(NULL, UX ,	  UY ,    fid,sid);
				_str  a_hint; /*"{FS}{C FF502515}"*/  
				a_hint += "{FS}{CR}";   a_hint += NATIONS[0].Mon[uid]->Message;   DYNHINT(UPic, a_hint.pchar() );	
				GPPicture*   UFrame	= pddKill->addGPPicture(NULL, UX +1 , UY +1 , lgpElems.GPID, 8 ) ;
				_str  SQNTxt;	
				SQNTxt += ( sqN0 - sqN1 ) ;
				TextButton*	 SQNum	= pddKill->addTextButton(NULL, UFrame->x+(UFrame->x1-UFrame->x+4)/2, UFrame->y1 -14 ,SQNTxt.pchar(),Rfont,Rfont,Rfont, 1 );
				//TextButton*	 SQNum	= pddKill->addTextButton(NULL,UX +32 /*31*/, UFrame->y1 - 14 /*15*/,SQNTxt.pchar(),Rfont,Rfont,Rfont, 1 );
				UN++;
				UX = UFrame->x1 + 17 ;
				if( UN > 4 )  { UN = 0 ;  UX = 15 ;  UY += 130 ; }
				TotalLost += ( sqN0 - sqN1 ) ;
			}
		}
	}
	_str  LostTxt;   LostTxt += TotalLost;
	TextButton* ptbTotalLost = pddDesk1->addTextButton(NULL, 916, 368 , LostTxt.pchar() ,Rfont,Rfont,Rfont,2);


	// KILLED PAGE 2
	DialogsDesk*	pddKill2 = pddDesk2->AddDialogsDesk( 523, 385, 430, 290, "EmptyBorder");//desk 2
	if( pDefendHero ) {
		UX = 15 ;  UY = 15 ;  UN = 0 ;     TotalLost = 0 ;

		int  RankID = pDefendHero->GetRank();
		pDefendHero->SetRank(5);
		
		// Defend
		for(int br=0; br < 3 ; br++) 
			for(int sq=0 ; sq < pDefendHero->ARMY.GetAmountOfElements() ; sq++)
			if( GetObjectBranch( pDefendHero->ARMY[sq]->getTypeGlobID() ) == br ) { 

			int  sqN0   = 0;											// кол-во отр. до миссии
			int  sqN1	= 0;											// кол-во отр. после миссии
			int  uid	= pDefendHero->ARMY[ sq ]->getTypeGlobID();		// тип юнитов
			
			for(int i=0; i < smsSAVE.DefenderForce.GetAmountOfElements() ; i++) 
				if( uid   == smsSAVE.DefenderForce[i]->TypeIndex_ID ) {
					sqN0  += smsSAVE.DefenderForce[i]->nSquarnd;
				}
			for(int i=0; i < EndMissionData.DefenderArmy.GetAmountOfElements() ; i++)
				if( uid   == EndMissionData.DefenderArmy[i]->inTypeID ) { 
					sqN1  += EndMissionData.DefenderArmy[i]->inAmount ; 
				}
			if( sqN0 > sqN1 && uid >=0 ) {
				int  fid ,  sid  ;
				fid = NATIONS[0].Mon[uid]->newMons->MinIconFile ;		//InMenuIconFile;	
				sid = NATIONS[0].Mon[uid]->newMons->MinIconIndex ;		//InMenuIconIndex;	
				if( fid == 0xFFFF ) {									//|| sid == 0xFFFF ) { 
					fid = NATIONS[0].Mon[uid]->newMons->IconFileID;
					sid = NATIONS[0].Mon[uid]->newMons->IconID;
				}
				if( fid == 0xFFFF ) { 
					fid = NATIONS[0].Mon[uid]->newMons->InMenuIconFile;
					sid = NATIONS[0].Mon[uid]->newMons->InMenuIconIndex;
				}
				if( fid != 0xFFFF && sid != 0xFFFF ) { 
					GPPicture*   UPic	= pddKill2->addGPPicture(NULL, UX ,	   UY ,    fid,sid);
					_str  a_hint;   /*"{FS}{C FF502515}"*/
					a_hint += "{FS}{CR}";   a_hint += NATIONS[0].Mon[uid]->Message;   DYNHINT(UPic, a_hint.pchar() );	
					GPPicture*   UFrame	= pddKill2->addGPPicture(NULL, UX +1 , UY +1 , lgpElems.GPID, 8 ) ;
					_str  SQNTxt;	SQNTxt += ( sqN0 - sqN1 ) ;
					TextButton*	 SQNum	= pddKill2->addTextButton(NULL, UFrame->x+(UFrame->x1-UFrame->x+4)/2, UFrame->y1 -14 ,SQNTxt.pchar(),Rfont,Rfont,Rfont, 1 );
					//TextButton*	 SQNum	= pddKill2->addTextButton(NULL,UX +32, UFrame->y1 - 14 ,SQNTxt.pchar(),Rfont,Rfont,Rfont, 1 );
					UN++;
					UX = UFrame->x1 + 17 ;
					if( UN > 4 )  { UN = 0 ;  UX = 15 ;  UY += 130 ; }
					TotalLost += ( sqN0 - sqN1 ) ;
				}
			}
		}
		pDefendHero->SetRank( RankID );

		_str  LostTxt2;   LostTxt2 += TotalLost;
		TextButton* ptbTotalLost = pddDesk2->addTextButton(NULL, 916, 368, LostTxt2.pchar() ,Rfont,Rfont,Rfont,2);
	}


	// Button OK  
	LocalGP		a_lgpOK("Interf3\\TotalWarGraph\\dMessage");
	GP_TextButton*  pgpbOK = a_dsMenu.addGP_TextButton(NULL, 410, 724 ,GetTextByID("#CDB_Close"),a_lgpOK.GPID,6,&fonMenuText[3],&fonMenuText[2]);
	pgpbOK->FontDy		= 0;
	pgpbOK->OnUserClick = &MMItemChoose;
	pgpbOK->UserParam	= mcmOk;

	pgpbOK->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );	
	pgpbOK->AssignSound( "INTERF_NAGAL", CLICK_SOUND );	

	_str a_hint;	a_hint.DString::Clear();  a_hint += "{FS}{C FF502515}";  
	a_hint += pgpbOK->Message;  a_hint+=" ";  a_hint += GetTextByID("#CHINT_ESCKEY");  DYNHINT(pgpbOK,a_hint.pchar());


	// PAGE BUTTONS		// ptbPage1 - Attack		// ptbPage2 - Defend 
	LocalGP		a_lgpPages("Interf3\\elements\\tab"); 
	GP_TextButton*  ptbPage1 = a_dsMenu.addGP_TextButton(NULL,  78, 91, GetTextByID("#CWT_StatsAttackPage") ,a_lgpPages.GPID,2,&OrangeFont,&OrangeFont);
	GP_TextButton*  ptbPage2 = a_dsMenu.addGP_TextButton(NULL, 209, 91, GetTextByID("#CWT_StatsDefendPage") ,a_lgpPages.GPID,3,&GrayFont,&GrayFont);
	ptbPage1->FontDy		= ptbPage2->FontDy		=  1 ; //-2 ; 
	ptbPage1->UserParam		= ptbPage2->UserParam	=  0 ;
	ptbPage1->OnUserClick	= ptbPage2->OnUserClick	= &OnPIButtonClicked;

	ptbPage1->AssignSound( "INTERF_PAGE", CLICK_SOUND );
	ptbPage2->AssignSound( "INTERF_PAGE", CLICK_SOUND );

	if( !pDefendHero ) ptbPage2->Visible = false;

	
	UnPress();					//Global
	KeyPressed = LastKey = 0;   //Global

	ItemChoose = - 1;
	
	bool  show_page1 = true;
	if( AttackNationID == gi_bmSAVE_Nation )	ptbPage1->UserParam = 1;
	else										ptbPage2->UserParam = 1;


	do {
		ProcessMessages();
		StdKeys();
		a_dsMenu.ProcessDialogs();
		a_dsMenu.RefreshView();

		// checks Visible Of Desks
		if( ptbPage1->UserParam > 0 ) { 
			show_page1 = true  ;
			ptbPage1->UserParam = 0;		
			ptbPage1->Sprite = ptbPage1->Sprite1 = 2; ptbPage1->ActiveFont = ptbPage1->PassiveFont = &OrangeFont; 
			ptbPage2->Sprite = ptbPage2->Sprite1 = 3; ptbPage2->ActiveFont = ptbPage2->PassiveFont = &GrayFont; 
		}
		if( ptbPage2 && pDefendHero && ptbPage2->UserParam > 0 ) { 
			show_page1 = false ;
			ptbPage2->UserParam = 0;  
			ptbPage2->Sprite = ptbPage2->Sprite1 = 2; ptbPage2->ActiveFont = ptbPage2->PassiveFont = &OrangeFont; 
			ptbPage1->Sprite = ptbPage1->Sprite1 = 3; ptbPage1->ActiveFont = ptbPage1->PassiveFont = &GrayFont; 
		}
		pddDesk1->Visible = pddDesk1->Enabled =  show_page1;
		pddDesk2->Visible = pddDesk2->Enabled = !show_page1;

	} while ( ItemChoose!=mcmOk && ItemChoose!=mcmCancel );
	
	ItemChoose = - 1;


	// СООБЩЕНИЯ С РЕЗУЛЬТАТАМИ МИССИИ   ( статистика + повышение звания, получ.бонусов и т.п. )
	int  PlayerExp	= 0, nonPlayerExp = 0;
	int  PKilledSQN	= ROOT_SCENARIO.FormKilledNum ;
	int  PLostSQN	= ROOT_SCENARIO.FormKilledNum2 ;
	//new
	int  SecDef   = ROOT_SCENARIO.m_pSect->GetSecDefence( MissionInitData.Sector_ID ); if(SecDef < 0)  SecDef = 0;
	int  AIRankID = 0;
	if( pDefendHero )  AIRankID = pDefendHero->m_inRankID;
	//new

	CHero*	pPlayerHero = NULL ;
	if( MissionInitData.PlayerStat == 0 ) {
		// player - attacker
		if( pAtackerHero )	pPlayerHero = pAtackerHero;
		PlayerExp		= EndMissionData.AttackerExperience;
		nonPlayerExp	= EndMissionData.DefenderExperience;
		//new
		AIRankID	   += SecDef;
	} else {
		// player - defender
		if( pDefendeHero )	pPlayerHero = pDefendeHero;
		PlayerExp		= EndMissionData.DefenderExperience;
		nonPlayerExp	= EndMissionData.AttackerExperience;
		//new
		PKilledSQN	= ROOT_SCENARIO.FormKilledNum2 ;
		PLostSQN	= ROOT_SCENARIO.FormKilledNum ;
		if( pAtackerHero )  AIRankID = pAtackerHero->m_inRankID;
	}

	int RankID = 0, StatID = 0, ExpAmount = 0;
	if( pPlayerHero ) { 
		RankID = pPlayerHero->GetRank();  StatID = pPlayerHero->GetStat();  ExpAmount = pPlayerHero->GetExperience(); 
	}

	if( EndMissionData.AttackerNI != EndMissionData.DefenderNI ) 
	{
		// Добавим опыт за миссию & disband SQN
		if(pAtackerHero && EndMissionData.AttackerExperience > 0 ) {
			pAtackerHero->AddExperience( EndMissionData.AttackerExperience );
			//new
			pAtackerHero->AddFormKilled( ROOT_SCENARIO.FormKilledNum  );
			pAtackerHero->AddFormLost  ( ROOT_SCENARIO.FormKilledNum2 );
		}
		if(pDefendeHero && EndMissionData.DefenderExperience > 0 ) { 
			pDefendeHero->AddExperience( EndMissionData.DefenderExperience );
			//new
			pDefendeHero->AddFormKilled( ROOT_SCENARIO.FormKilledNum2 );
			pDefendeHero->AddFormLost  ( ROOT_SCENARIO.FormKilledNum  );
		}

		int attRew = 0;
		// выясним приращение экспы (>20% - дать награду)
		if( pPlayerHero && nonPlayerExp!=0 && PlayerExp - nonPlayerExp > ExpAmount/(RankID+3) ) 
			attRew++;
		// выясним потери в бою и сравним звания игроков
		if( pPlayerHero && AIRankID > pPlayerHero->m_inRankID - 2 && //new
			PKilledSQN > 4 && PKilledSQN > PLostSQN )
			attRew++;

		//if( pPlayerHero && pPlayerHero->GetExperience() - ExpAmount > ExpAmount/3 )  
		//	attRew++;

		// Корекция нового звания
		if(pAtackerHero)  pAtackerHero->GetRewardsStatus( MissionInitData.PlayerStat==0 ? attRew:0 );//new
		if(pAtackerHero)  pAtackerHero->UpdateRankWithExperience();
		if(pDefendeHero)  pDefendeHero->GetRewardsStatus( MissionInitData.PlayerStat!=0 ? attRew:0 );//new
		if(pDefendeHero)  pDefendeHero->UpdateRankWithExperience();

		//if(pAtackerHero)  pAtackerHero->UpdateRankWithExperience();
		//if(pAtackerHero)  pAtackerHero->GetRewardsStatus();
		//if(pDefendeHero)  pDefendeHero->UpdateRankWithExperience();
		//if(pDefendeHero)  pDefendeHero->GetRewardsStatus();
		//new
	}

	if( pPlayerHero && pPlayerHero->GetRank() > RankID ) {
		char text[256];
		sprintf(text,GetTextByID("#CWM_UpdateHeroRank"),pPlayerHero->GetName(),GetRankByID( pPlayerHero->GetRank() ));
		int pic = 32;	if( gi_bmSAVE_Nation == 5 ) pic = 31;
		ShowBigMapGlobalMessage(1, pic , 33 , text );
		RankID = pPlayerHero->GetRank();
	}

	if( pPlayerHero && pPlayerHero->GetStat() > StatID ) {
		char text[256];
		int  TID = pPlayerHero->GetStat()%5 + rand()%7;
		if( TID == StatID%5 )  TID += 2;
		if( TID > 4 )  TID = 0;
		char  tmp[256];		
		sprintf(tmp,  "#CWM_HeroGetAward%d" , TID );
		sprintf(text, GetTextByID( tmp ) , pPlayerHero->GetName() , pPlayerHero->GetStat() , LTN_EXPS_LIMIT );
		int pic = 32;	if( gi_bmSAVE_Nation == 5 ) pic = 31; //33
		ShowBigMapGlobalMessage(1, pic , 34 , text );
	}
	if( pPlayerHero && pPlayerHero->GetRank() > RankID ) {
		char text[256];
		sprintf(text,GetTextByID("#CWM_UpdateHeroRank"),pPlayerHero->GetName(),GetRankByID( pPlayerHero->GetRank() ));
		int pic = 32;	if( gi_bmSAVE_Nation == 5 ) pic = 31;
		ShowBigMapGlobalMessage(1, pic , 33 , text );
	}

	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------
void	ProcessBigMapPlayVideo( int id /*= -1*/ )
{
	if( id == 0 )  
		return;
	_str  name;
	name = "Video\\InGame\\Campaign\\";
	bool  winner = false;
	if( id < 0 ) { // end mission video
		if( gi_bmSAVE_Nation == EndMissionData.WinnerNI )	{  name += "Vict" ;  }
		else												{  name += "Deft";   }
		switch( gi_bmSAVE_Nation ) {
			case 0 :	name +=  "EN" ;	break;	// ENGLAND
			case 1 :	name +=  "FR" ;	break;	// FRANCE	
			case 2 :	name +=  "AU" ;	break;	// AUSTRIA	
			case 3 :	name +=  "PR" ;	break;	// PRUSSIA	
			case 4 :	name +=  "RU" ;	break;	// RUSSIA	
			case 5 :	name +=  "EG" ;	break;	// EGYPT	
			default:	name +=  "FR" ;	
		}
	}
	if( id == 0 || id == 1 ) { // end Campaign video
		name += "CampEnd";
	}
	name += ".bik";
	UnPress();					//Global
	KeyPressed = LastKey = 0;   //Global
	ItemChoose = - 1;			//Global
	PlayFullscreenVideo( name.pchar() , 0 , 0.17 ); // bik resolution = 800x448
	UnPress();					//Global
	KeyPressed = LastKey = 0;   //Global
	ItemChoose = - 1;			//Global
	//void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
	//PlayFullscreenVideo( "Video\\InGame\\Victory\\Vic_Aus.bik" , 0 , 0.17 ); // bik resolution = 800x448
}

//void	ProcessBigMapPlayVideo( int id /*= -1*/ )
//{
//	_str   name;
//	name += "Video\\InGame\\Campaign\\";
//	bool  winner = false;
//	if( id < 0 ) { // end mission video
//		if( gi_bmSAVE_Nation == EndMissionData.WinnerNI )	{ name += "Vic_" ; } //winner = true;
//		else												{ name += "Def_"; }
//		switch( gi_bmSAVE_Nation ) {
//			case 0 :	name +=  "Eng" ;	break; // ENGLAND
//			case 1 :	name +=  "Frn" ;	break; // FRANCE	
//			case 2 :	name +=  "Aus" ;	break; // AUSTRIA	
//			case 3 :	name +=  "Pru" ;	break; // PRUSSIA	
//			case 4 :	name +=  "Rus" ;	break; // RUSSIA	
//			case 5 :	name +=  "Egp" ;	break; // EGYPT	
//			default:	name +=  "Frn" ;							// default   return;
//		}
//	}
//	if( id == 0 || id == 1 ) { // end Campaign video
//		if( id == 1 )	{ name += "Vict" ; } //winner = true;
//		else			{ name += "Deft"; }
//		switch( gi_bmSAVE_Nation ) {
//			case 0 :	name +=  "EN" ;	break; // ENGLAND
//			case 1 :	name +=  "FR" ;	break; // FRANCE	
//			case 2 :	name +=  "AU" ;	break; // AUSTRIA	
//			case 3 :	name +=  "PR" ;	break; // PRUSSIA	
//			case 4 :	name +=  "RU" ;	break; // RUSSIA	
//			case 5 :	name +=  "EG" ;	break; // EGYPT	
//			default:	name +=  "FR" ;								// default   return;
//		}
//	}
//	name += ".bik";
//	UnPress();					//Global
//	KeyPressed = LastKey = 0;   //Global
//	ItemChoose = - 1;			//Global
//	PlayFullscreenVideo( name.pchar() , 0 , 0.17 ); // bik resolution = 800x448
//	//void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
//	//PlayFullscreenVideo( "Video\\InGame\\Victory\\Vic_Aus.bik" , 0 , 0.17 ); // bik resolution = 800x448
//}

//-----------------------------------------------------------------------------------------------------------------------------
void	SetBigMapGlobalVariables()
{
	ReadBriefingDirectionsFromFile();
	SetBigMapGlobalVariablesFF();
}
//-----------------------------------------------------------------------------------------------------------------------------
void	ReadBriefingDirectionsFromFile() //added Grey [23.04.2004]
{	
	//BriefDIR_XY[24][8][4]						//BriefDIR_XY[24][8][8];
	int  SECTS = 24 ,  DIRS = 8 ,  WORDS = 4 ;
	for(int i=0; i<SECTS; i++)
		for(int j=0; j<DIRS; j++)
			for(int k=0; k<WORDS; k++)  BriefDIR_XY[i][j][k] = 0 ;

	DString		a_DS;
	a_DS.ReadFromFile("Missions\\StatData\\Briefing.dat");

	if(!a_DS.pchar()) return;//VERY NEW //FOR FUNTASIA

	DString		a_DS2;
	char name[256];

	int   sec = 0 ,  line = -1 ;
	bool  quit = false , find = false ;
	do
	{
		a_DS.ExtractWord(a_DS2);

		// skip line
		if( a_DS2.pchar()[0] == 47 /*"/"*/ ) { a_DS.ExtractLine(a_DS2); line++; continue; }

		for(sec=0; sec < 24 ; sec++) { 
			sprintf(name, "sector%d" , sec );
			// next item
			if( a_DS2 == name ) {
				for(int dir=0; dir < 8 ; dir++)	{	//BriefDIR_XY[24][8][4]; // [secid][dir][xy]
					a_DS.ExtractWord(a_DS2);
                    BriefDIR_XY[ sec ][dir][0] = atoi(a_DS2.pchar()) ; // x0  A
					a_DS.ExtractWord(a_DS2);
                    BriefDIR_XY[ sec ][dir][1] = atoi(a_DS2.pchar()) ; // y0  A
					a_DS.ExtractWord(a_DS2);
                    BriefDIR_XY[ sec ][dir][2] = atoi(a_DS2.pchar()) ; // x0  D
					a_DS.ExtractWord(a_DS2);
                    BriefDIR_XY[ sec ][dir][3] = atoi(a_DS2.pchar()) ; // y0  D
					line++;
				}
				break;
			}
		}

		// end
		if( a_DS2 == "[END]" || line > 0xFFFF )  quit = true;

		line++;
	}
	while ( !quit );
}
//-----------------------------------------------------------------------------------------------------------------------------
void	SetBigMapGlobalVariablesFF() //added Grey [23.04.2004]
{	
	for(int i=0; i < 10 ; i++) {
		SECT_INCOME[i] = SECT_ADDING[i] = SECT_DEFENCE[i] = 0 ; //init
        for(int k=0; k < 5 ; k++)  SECT_SABOTAGE[i][k] = SECT_SAB_PERCENT[i][k] = 0; //init
	}

	DString		a_DS;
	a_DS.ReadFromFile("Missions\\StatData\\BigMapConst.dat");

	if(!a_DS.pchar()) return;//VERY NEW //FOR FUNTASIA

	DString		a_DS2, a_DS3;

	char namePL[256] ,  nameAI[256] , txt[256] ;
	//
	int diff = 0;
	if(pSAVE)  diff = pSAVE->m_iDifficulty;
	sprintf( namePL , "#PLAYER_RES%d" , diff );
	sprintf( nameAI , "#ENEMY__RES%d" , diff );
	//sprintf( namePL , "#PLAYER_RES%d" , pSAVE->m_iDifficulty );
	//sprintf( nameAI , "#ENEMY__RES%d" , pSAVE->m_iDifficulty );
	//

	int   line = -1 ;
	bool  quit = false ;
	do
	{
		a_DS.ExtractWord(a_DS2);
		// skip line
		if( a_DS2.pchar()[0] == 47 /*"/"*/ ) { a_DS.ExtractLine(a_DS2); line++; continue; }

		// next items
		int ijk = 0;
		// start resources
		if( a_DS2 == "#FOOD_RATIO" )   { 
			a_DS.ExtractWord(a_DS2);	ROOT_SCENARIO.m_iBigMap_FOOD_RATIO = atoi(a_DS2.pchar()); 
		}
		if( a_DS2 == nameAI )   { 
			a_DS.ExtractWord(a_DS2);	ROOT_SCENARIO.m_iBigMap_AI___RESOURCES	= atoi(a_DS2.pchar()); 
		}
		if( a_DS2 == namePL )   { 
			a_DS.ExtractWord(a_DS2);	ROOT_SCENARIO.m_iBigMap_User_RESOURCES	= atoi(a_DS2.pchar()); 
		}
		// делитель для дохода ресурсов - f(diff)
		for(int i=0; i < DIFF_LEVELS_N ; i++) {
			sprintf(txt, "#Res_Div_%d_f" , i );
			if( !strcmp( a_DS2.pchar() , txt ) ) {
				a_DS.ExtractWord(a_DS2);	ROOT_SCENARIO.UpdateResDiv[i] = atof(a_DS2.pchar());
			}
		}
		// доход от сектора на каждом ходу
		if( a_DS2 == "#SECT_INCOME_WOOD" )   { 
			a_DS.ExtractWord(a_DS2);	SECT_INCOME[ WOOD ]	= atoi(a_DS2.pchar()); 
		}
		if( a_DS2 == "#SECT_INCOME_FOOD" )   {
			a_DS.ExtractWord(a_DS2);	SECT_INCOME[ FOOD ] = atoi(a_DS2.pchar()); 
		}
		if( a_DS2 == "#SECT_INCOME_STONE" ) {
			a_DS.ExtractWord(a_DS2);	SECT_INCOME[ STONE] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_INCOME_GOLD" ) {
			a_DS.ExtractWord(a_DS2);	SECT_INCOME[ GOLD ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_INCOME_IRON" ) {
			a_DS.ExtractWord(a_DS2);	SECT_INCOME[ IRON ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_INCOME_COAL" ) {
			a_DS.ExtractWord(a_DS2);	SECT_INCOME[ COAL ] = atoi(a_DS2.pchar());
		}
		// доход от основного ресурса сектора
		if( a_DS2 == "#SECT_ADDING_FOOD" ) {				
			a_DS.ExtractWord(a_DS2);	SECT_ADDING[ FOOD ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_ADDING_GOLD" ) {
			a_DS.ExtractWord(a_DS2);	SECT_ADDING[ GOLD ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_ADDING_IRON" ) {
			a_DS.ExtractWord(a_DS2);	SECT_ADDING[ IRON ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_ADDING_COAL" ) {
			a_DS.ExtractWord(a_DS2);	SECT_ADDING[ COAL ] = atoi(a_DS2.pchar());
		}
		// макс.кол-во рекрутов в секторе
		if( a_DS2 == "#SECT_MAX_RECRTS" ) {
			a_DS.ExtractWord(a_DS2);	SECT_MAX_RECRTS		= atoi(a_DS2.pchar());
		}
		// восстановление рекрутов в секторе
		if( a_DS2 == "#SECT_REG_RECRTS" ) {
			a_DS.ExtractWord(a_DS2);	SECT_REG_RECRTS		= atoi(a_DS2.pchar());
		}
		// затраты на defence сектора
		if( a_DS2 == "#SECT_DEFENCE_WOOD" ) {
			a_DS.ExtractWord(a_DS2);	SECT_DEFENCE[ WOOD  ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_DEFENCE_FOOD" ) {
			a_DS.ExtractWord(a_DS2);	SECT_DEFENCE[ FOOD  ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_DEFENCE_STONE" ) {
			a_DS.ExtractWord(a_DS2);	SECT_DEFENCE[ STONE ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_DEFENCE_GOLD" ) {
			a_DS.ExtractWord(a_DS2);	SECT_DEFENCE[ GOLD  ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_DEFENCE_RECRTS" ) {
			a_DS.ExtractWord(a_DS2);	SECT_DEFENCE[ RECRT ] = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_DEF_mult2_f" ) {
			a_DS.ExtractWord(a_DS2);	SECT_DEF_multiplier2 = atof(a_DS2.pchar());
		}
		if( a_DS2 == "#SECT_DEF_mult3_f" ) {
			a_DS.ExtractWord(a_DS2);	SECT_DEF_multiplier3 = atof(a_DS2.pchar());
		}
		// затраты for SABOTAGE	
		for(int i=0; i < 4 ; i++) {
			sprintf(txt, "#SECT_SABOTAGE%d" , i );
			if( !strcmp( a_DS2.pchar() , txt ) ) {
				bool  done = false;
				for(int k=0; k < 4 && !done ; k++) {
					a_DS.ExtractWord(a_DS2);	SECT_SABOTAGE[i][k] = atoi(a_DS2.pchar());		
					if( atoi(a_DS2.pchar()) == 0 )  done = true; 
				}
			}
			sprintf(txt, "#SECT_SAB_PERCENT%d" , i );
			if( !strcmp( a_DS2.pchar() , txt ) ) {
				bool  done = false;
				for(int k=0; k < 4 && !done ; k++) {
					a_DS.ExtractWord(a_DS2);	SECT_SAB_PERCENT[i][k] = atoi(a_DS2.pchar());		
					if( atoi(a_DS2.pchar()) == 0 )  done = true; 
				}
			}
		}
		// пределы уровня экспы для текущего звания
		if( a_DS2 == "LTN_EXPS_LIMIT" ) {
			a_DS.ExtractWord(a_DS2);	LTN_EXPS_LIMIT		= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "CPT_EXPS_LIMIT" ) {
			a_DS.ExtractWord(a_DS2);	CPT_EXPS_LIMIT		= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "MJR_EXPS_LIMIT" ) {
			a_DS.ExtractWord(a_DS2);	MJR_EXPS_LIMIT		= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "COL_EXPS_LIMIT" ) {
			a_DS.ExtractWord(a_DS2);	COL_EXPS_LIMIT		= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "GEN_EXPS_LIMIT" ) {
			a_DS.ExtractWord(a_DS2);	GEN_EXPS_LIMIT		= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "MAR_EXPS_LIMIT" ) {
			a_DS.ExtractWord(a_DS2);	MAR_EXPS_LIMIT		= atoi(a_DS2.pchar());
		}
		// Exp. in Miss 
		if( a_DS2 == "DisbandSquadExp" ) {
			a_DS.ExtractWord(a_DS2);	DisbandSQExp		= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "SectCaptureExp" ) {
			a_DS.ExtractWord(a_DS2);	SecCaptExp			= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "HeroDefeatExp" ) {
			a_DS.ExtractWord(a_DS2);	HeroDefExp			= atoi(a_DS2.pchar());
		}
		// quest exp.
		if( a_DS2 == "QuestDisbSquadExp" ) {
			a_DS.ExtractWord(a_DS2);	QuestDisbSquadExp	= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "QuestCompliteExp" ) {
			a_DS.ExtractWord(a_DS2);	QuestCompliteExp	= atoi(a_DS2.pchar());
		}
		if( a_DS2 == "MISS_FRAG_LIMIT" ) { // BigMap NKills Limit for squads
			a_DS.ExtractWord(a_DS2);	gi_bmMISS_FRAG_LIMIT = atoi(a_DS2.pchar());
		}
		// Map Colors			NATCOLOR%d
		DWORD col;   
		for(int i=0; i < 2*MAX_PLAYERS ; i++) {
			sprintf(txt, "NATCOLOR%d" , i );
			if( !strcmp( a_DS2.pchar() , txt ) ) {
				a_DS.ExtractWord(a_DS2);
				sscanf( a_DS2.pchar(), "%X", &col );
				if( col!=0 )  ROOT_SCENARIO.m_iMapColor[i] = col ;
			}
		}
		if( a_DS2 == "SELECTED_SECT_COLOR" ) { // selected sector
			a_DS.ExtractWord(a_DS2);	
			sscanf( a_DS2.pchar(), "%X", &col );
			if( col!=0 )  SELECTED_SECT_COLOR = col;
		}
		if( a_DS2 == "DIPL_WAR_SECT_COLOR" ) { // сост.войны - дипломатия
			a_DS.ExtractWord(a_DS2);
			sscanf( a_DS2.pchar(), "%X", &col );
			if( col!=0 )  RED_SECT_COLOR = col;
		}
		if( a_DS2 == "DIPL_PCT_SECT_COLOR" ) { // договора   - дипломатия
			a_DS.ExtractWord(a_DS2);
			sscanf( a_DS2.pchar(), "%X", &col );
			if( col!=0 )  GREEN_SECT_COLOR = col;
		}
		// Nation identify sprites color
		for(int i=0; i < MAX_PLAYERS+1 ; i++) {
			sprintf(txt, "nat%s" , ROOT_SCENARIO.m_chNationID[i] );
			if( !strcmp( a_DS2.pchar() , txt ) ) {
				a_DS.ExtractWord(a_DS2);
				ROOT_SCENARIO.m_iNationID[i] = atoi(a_DS2.pchar());
			}
		}
		// Sector defence squads/params
		for(int i=0; i <= MAX_DEFENCE ; i++) {
			sprintf(txt, "SectGarrisonSquads%d" , i );
			if( !strcmp( a_DS2.pchar() , txt ) ) {
				a_DS.ExtractWord(a_DS2);
				ROOT_SCENARIO.m_iGarrSqN[i] = atoi(a_DS2.pchar());
			}
			sprintf(txt, "SectGarrisonAdd%d" , i );
			if( !strcmp( a_DS2.pchar() , txt ) ) {
				a_DS.ExtractWord(a_DS2);
				ROOT_SCENARIO.m_iGarrAdd[i] = atoi(a_DS2.pchar());
			}
			sprintf(txt, "SectRevoltedSquads%d" , i );
			if( !strcmp( a_DS2.pchar() , txt ) ) {
				a_DS.ExtractWord(a_DS2);
				ROOT_SCENARIO.m_iRebelAdd[i] = atoi(a_DS2.pchar());
			}
		}
		if( a_DS2 == "MinSectDefSquadPower" ) {
			a_DS.ExtractWord(a_DS2);	ROOT_SCENARIO.m_iMinSecDefPower = atoi(a_DS2.pchar());
		}
		if( a_DS2 == "AddSectDefSquadPower" ) {
			a_DS.ExtractWord(a_DS2);	ROOT_SCENARIO.m_iAddSecDefPower = atoi(a_DS2.pchar());
		}
		
		// Commanders Page Blink color		COMPAGE_BLINKCOLOR
		if( a_DS2 == "COMPAGE_BLINKCOLOR" ) {
			a_DS.ExtractWord(a_DS2);	
			sscanf( a_DS2.pchar(), "%X", &col );
			if( col!=0 )  
				COMPAGE_BLINKCOLOR = col;
		}


		//if( a_DS2 == "#" ) {
		//	a_DS.ExtractWord(a_DS2);	 = atoi(a_DS2.pchar());
		//}


		// end
		if( a_DS2 == "[END]" || line > 0xFFFF )  quit = true;
		line++;
	}
	while ( !quit );
}

//-----------------------------------------------------------------------------------------------------------------------------
GPPicture*	ShowBlackScreen( DialogsSystem*	dsMenu ) 
{
	// BlackScreen
	LocalGP		a_lgpScreen("Interf3\\TotalWarGraph\\blackscr");			
	GPPicture*	a_pgppScr = dsMenu->addGPPicture(NULL,0,0,a_lgpScreen.GPID,0);
	a_pgppScr->Diffuse = 0x88202020;
	a_pgppScr->Visible = a_pgppScr->Enabled = true;
	a_pgppScr->OnUserClick		= &EmptyClick; 
	a_pgppScr->OnUserRightClick	= &EmptyClick; 
	a_pgppScr->OnMouseOver		= &EmptyClick;
	ProcessMessages();				
	dsMenu->ProcessDialogs();
	StdKeys();		
	dsMenu->RefreshView();
	return  a_pgppScr;
}
//-----------------------------------------------------------------------------------------------------------------------------
int		ShowBigMapGlobalMessage( int type, int pictId, int headId, char* text, int pageN )//added Grey [16.01.2004]
{
	pageN = ROOT_SCENARIO.m_Scenario.m_inActivMenu;
	return	ROOT_SCENARIO.m_pDipl->ShowGlobalMessage( type, pictId, headId, text, pageN );
}
//-------------------------------------------------------------------------------------------
bool	SimulateBattle( CHero* pAttackHero, CHero* pDefendHero, CSectStatData* pSector )//added Grey [12.01.2004]
{
	return  ROOT_SCENARIO.SimulateBattle( pAttackHero , pDefendHero , pSector );
}
//-----------------------------------------------------------------------------------------------------------------------------
bool	SetBigMapMenuEnabled( bool state )
{
	ROOT_SCENARIO.SetMenuEnabled( state ); // Enable of All Menu elements
	return state;
}
//-----------------------------------------------------------------------------------------------------------------------------
int		AddRecruitsInSectors( int nation, int secID, int amount ) // Grey [12.01.2004]
{
	return	ROOT_SCENARIO.AddRecruitsInSectors( nation , secID , amount );
}
int		DeleteRecruitsInSectors( int nation, int secID, int amount ) // Grey [12.01.2004]
{
	return	ROOT_SCENARIO.DelRecruitsInSectors( nation , secID , amount );
}
//-----------------------------------------------------------------------------------------------------------------------------
///// воз-ть войти в сектор с атакой или без атаки
bool	CanEnterIntoSector( int nation , int secID )
{
	if( secID < 0 || secID >= ROOT_SCENARIO.m_pSect->GetSectorsNum() )							{ return false; }
	#ifdef DemoAI	//DEMO
		if( secID==1 || secID==2 || secID==5 || secID==6 )   { return true; }	return false ;	
	#endif			//DEMO

	// checks DIPLOMACY states
	int	PACT     = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetPactId();
	int	PASS     = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetPassId();
	int secOwner = ROOT_SCENARIO.m_pSect->GetSecOwner( secID ) ;


	// ********** 0 - запрет из квестов   = false ********** 
	if( ROOT_SCENARIO.m_pQuest->QuestOwner == nation &&
		ROOT_SCENARIO.m_pQuest->QuestSecId == secID )											{ return false; }
	if( ROOT_SCENARIO.m_pQuest->QuestOwner == nation && secOwner == gi_bmSAVE_Nation )			{ return false; }


	CHero* pHero = ROOT_SCENARIO.GetHeroInSect( secID , 0xFFFF );

	// ********** 1 - это наш сектор и там нет героя (есть не-PASS герой)   = true **********
	//if(  secOwner == nation && 
	//	ROOT_SCENARIO.GetHeroInSect( secID , 0xFFFF ) == NULL )									{ return true; }	
	if( nation == secOwner ) {
		if( !pHero )																			{ return true; }	
		if(  pHero->m_inPlayerID == nation )													{ return true; }//new	

		if( !ROOT_SCENARIO.CheckRightsForHeroInSector( pHero ) )								{ return true; }
		//if(  pHero->m_inPlayerID != secOwner &&
		//	!ROOT_SCENARIO.m_pDipl->CheckContract( PASS , pHero->m_inPlayerID , secOwner ) )	{ return true; }	
	}


	// ********** 2 - сектор не наш, есть право прохода и там нет героя   = true **********
	if( nation != secOwner &&	
		ROOT_SCENARIO.m_pDipl->CheckContract( PASS , nation , secOwner ) )
	{
		if( !pHero )																			{ return true; }
		if(  pHero->m_inPlayerID == nation )													{ return true; }//new

		if( !ROOT_SCENARIO.CheckRightsForHeroInSector( pHero ) )								{ return true; }
		//if(  pHero->m_inPlayerID != secOwner &&
		//	!ROOT_SCENARIO.m_pDipl->CheckContract( PASS , pHero->m_inPlayerID , secOwner ) )	{ return true; }	
	}
	//if(  secOwner != nation &&	
	//	ROOT_SCENARIO.m_pDipl->CheckContract( PASS , nation , secOwner ) &&
	//	ROOT_SCENARIO.GetHeroInSect( secID , 0xFFFF ) == NULL )									{ return true; }

		
	// ********** 3 - сектор не наш и нет пакта (нет героя/нет прохода,owner герой/не owner герой,нет пакта) = true **********
	if(  nation != secOwner && 
		!ROOT_SCENARIO.m_pDipl->CheckContract( PACT , nation , secOwner ) ) 
	{
		if( !pHero )																			{ return true; }
		if(  pHero->m_inPlayerID == nation )													{ return true; }//new	

		if( !ROOT_SCENARIO.m_pDipl->CheckContract( PASS , nation , secOwner ) ) // война
		{
			if( pHero->m_inPlayerID == secOwner )												{ return true; }

			if( !ROOT_SCENARIO.m_pDipl->CheckContract( PACT , nation , pHero->m_inPlayerID ) )	{ return true; }

			//new
			if( !ROOT_SCENARIO.CheckRightsForHeroInSector( pHero ) )							{ return true; }
			//if(  pHero->m_inPlayerID != secOwner &&
			//	!ROOT_SCENARIO.m_pDipl->CheckContract( PASS , pHero->m_inPlayerID , secOwner ) ) { return true; }	
		}
	}
	return false ;
}
//-----------------------------------------------------------------------------------------------------------------------------
///// воз-ть войти в сектор без атаки - find path & retreat heroes
bool	CanStepIntoSector( int nation , int secID ) //grey [01.03.2004]	
{
	if( secID < 0 || secID >= ROOT_SCENARIO.m_pSect->GetSectorsNum() )							{ return false; }

	#ifdef DemoAI	//DEMO
		if(ROOT_SCENARIO.m_pSect->GetSecOwner(secID)==nation&&(secID==1 || secID==2 || secID==5 || secID==6)) { return true; } return false ;
	#endif			//DEMO

	int secOwner = ROOT_SCENARIO.m_pSect->GetSecOwner( secID ) ;

	if( ROOT_SCENARIO.GetHeroInSect( secID , 0xFFFF ) == NULL )
	{
		if( nation == secOwner )													{ return true; }

		int	PASS	= ROOT_SCENARIO.m_pDipl->m_pDiplData->GetPassId();
		if( nation != secOwner &&	
			ROOT_SCENARIO.m_pDipl->CheckContract( PASS , nation , secOwner ) )		{ return true; }
	}
	return false ;
}
//-----------------------------------------------------------------------------------------------------------------------------
////// only for ARROWS ( CSectData::ViewNeighbor )
bool	CanAttackSector( int nation , int secID ) // grey [26.02.2004]
{
	if( secID < 0 || secID >= ROOT_SCENARIO.m_pSect->GetSectorsNum() )				{ return false; }

	int secOwner = ROOT_SCENARIO.m_pSect->GetSecOwner( secID ) ;

	if(  secOwner == nation )														{ return false; }

	int	PASS     = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetPassId();
	if(  secOwner != nation &&	
		ROOT_SCENARIO.m_pDipl->CheckContract( PASS , nation , secOwner ) )			{ return false; }

	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------
////// only for Right-Left align ( CUnit::UpdatePic )
bool	CanSetRightHeroAlign( int nation , int secID ) //grey [16/04/2004]	
{
	if( secID < 0 || secID >= ROOT_SCENARIO.m_pSect->GetSectorsNum() )			{ return false; }

	int secOwner = ROOT_SCENARIO.m_pSect->GetSecOwner( secID ) ;
	if( secOwner == nation )													{ return true; }

	int	PASS     = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetPassId();
	if( secOwner != nation &&	
		ROOT_SCENARIO.m_pDipl->CheckContract( PASS , nation , secOwner ) )		{ return true; }

	return false ;
}
//-----------------------------------------------------------------------------------------------------------------------------
// ------------------------------- ProcessDiplomacy ---------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
bool	DiplActionPressed(SimpleDialog* pSD) {
	if (ROOT_SCENARIO.m_Scenario.m_inActivMenu == 1) {
		ROOT_SCENARIO.m_Scenario.m_inDiplActionPressed = pSD->UserParam;	}
	return true;
}
bool	MapButtonPressed(SimpleDialog* pSD) {
	if (ROOT_SCENARIO.m_Scenario.m_inActivMenu == 1) {
		ROOT_SCENARIO.m_Scenario.m_inMapButtonPressed = pSD->UserParam;		}
	return true;
}
bool 	InformationMessagePressed(SimpleDialog* pSD) {
	ROOT_SCENARIO.m_Scenario.m_inInfoButtonPressed = pSD->UserParam;		
	return true;
}
//-------------------------------------------------------------------------------------------
bool	InformationYesClick(SimpleDialog* pSD)
{
	if( pSD->UserParam <= 0 )	{ return false; }
	CDiplInfoMessages* pMs = reinterpret_cast<CDiplInfoMessages*>(pSD->UserParam);
	pMs->ConfirmMessage( pSD->ID );		

	int  actID = ROOT_SCENARIO.m_pMessages->m_pInform->GetParam( pSD->ID );
	if( ROOT_SCENARIO.m_pMessages->GetMesNum() > 0 && actID >= 0 && 
		ROOT_SCENARIO.m_pMessages->CAMess[ actID ]->reqId  == -0x5e11 ) 
	{
		ROOT_SCENARIO.m_pQuest->QuestParam = 1;
		ROOT_SCENARIO.m_pQuest->ProcessQuests( &ROOT_SCENARIO , gi_bmSAVE_Nation , false );
	}

	return true;
}
bool	InformationNoClick(SimpleDialog* pSD)
{
	if( pSD->UserParam <= 0 )	{ return false; }
	CDiplInfoMessages* pMs = reinterpret_cast<CDiplInfoMessages*>(pSD->UserParam);
	pMs->DeleteMessage( pSD->ID );		
	return true;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
bool	OnDiversionClicked(SimpleDialog* pSD)
{
	if( !pSD )						{ return false; }
	if( pSD->UserParam < 0 )		{ return false; }

	 // Diversions
	int   secID		= ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID ;
	int   ownerID	= ROOT_SCENARIO.m_pSect->GetSecOwner( secID );
	int   def		= ROOT_SCENARIO.m_pSect->GetSecDefence( secID ) ;
	
	char  text[256];
	// отнять деньгу!
	int   money = 10 * ( pSD->UserParam / 10 ) ;
	if( ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->GetResource( GOLD ) >= money ) {
		ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->AddMoney( - money );
	} 
	else {
		//int pic = 16 ; if( gi_bmSAVE_Nation == 5 ) pic = 17;
		//_str	a_Cost ;
		//a_Cost += money ;	a_Cost += " " ;		a_Cost += RDS[ GOLD  ].Name ;
		//sprintf(text, GetTextByID("#CWM_NoEnoughRes") , a_Cost.pchar() );
		//ShowBigMapGlobalMessage(1, pic , 11 , text ,0); 
		//// Enable of All Menu elements
		//SetBigMapMenuEnabled( false );
		ROOT_SCENARIO.m_pSectMenu->SetDiversionTexts( def );
		return false;
	}

	//
	ROOT_SCENARIO.m_pSectMenu->SetDiversionTexts( def );
	//for(int i=0; i < ROOT_SCENARIO.m_pSectMenu->m_iDTN; i++) 
	//{
	//	TextButton* a_ptb = ROOT_SCENARIO.m_pSectMenu->m_ptbDText[i];
	//	if( a_ptb ) 
	//	{
	//		int   money2 = 10 * ( a_ptb->UserParam / 10 ) ;
	//		if( ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->GetResource( GOLD  ) < money2 )  
	//			a_ptb->Enabled = false;
	//	}
	//}
	//if( ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->GetResource( GOLD  ) < money )  pSD->Enabled = false;
	//

	int	  action = pSD->UserParam%10 ;

	if( action >= 0 && action < 4 && def >= 0 && def < 4 ) {
		int   pt = rand()%100 -5 ;
		bool  done = ( pt < SECT_SAB_PERCENT[action][def] );
		if( done ) {
			if( action == 0 )		// (recruits = 0)	
			{
				//PlaySound( "INTERF_NAGAL" ); //new
				PlaySound( "INTERF_SAB0" ); //new
				ROOT_SCENARIO.m_pSect->m_pssdDT[ secID ]->m_iRecruits	 = 0 ;
				ROOT_SCENARIO.m_pSect->m_pssdDT[ secID ]->m_iSabotageID += 1 ;
				if( ownerID >= 0 && ownerID < MAX_PLAYERS )  ROOT_SCENARIO.m_pPlayer[ ownerID ]->SetRecruit() ;
			}
			if( action == 1 )		// (all resources = 0)		
			{
				PlaySound( "INTERF_SAB1" ); //new
				ROOT_SCENARIO.m_pSect->m_pssdDT[ secID ]->m_iSabotageID += 10 ;
			}
			if( action == 2 )		// (defence - 1)
			{
				PlaySound( "INTERF_SAB2" ); //new
				if(def) ROOT_SCENARIO.m_pSect->m_pssdDT[ secID ]->m_inDefence-- ;
				ROOT_SCENARIO.m_pSect->m_pssdDT[ secID ]->m_iSabotageID += 100 ;
			}
			if( action == 3 )		// (nation = -1 )
			{
				PlaySound( "INTERF_SAB3" ); //new
				(ROOT_SCENARIO.m_pSect->m_psddDT[ secID ])->SetOwner( -1 );
				ROOT_SCENARIO.m_pSect->m_pssdDT[ secID ]->m_inDefence    = 0 ;
				ROOT_SCENARIO.m_pSect->m_pssdDT[ secID ]->m_iSabotageID += 1000 ;
			}
			
			sprintf( text, "#CWM_DiversionDone%d" , action ); 
			ShowBigMapGlobalMessage( 1, 24 , 21 , GetTextByID( text ) , 0 ) ;

			//pSD->Enabled = false;
			pSD->Visible = false;
		}
		else {
			PlaySound( "INTERF_FAIL" );

			int  messId = rand()%10 / 2 ; // 0 - 1 - 2 - 3 - 4
			sprintf( text, "#CWM_DiversionFail%d" , messId );
			ShowBigMapGlobalMessage( 1, 25 , 22 , GetTextByID( text ) , 0 ) ;
		}
	}
	// Enable of All Menu elements
	SetBigMapMenuEnabled( false );
	return true;
}
//-------------------------------------------------------------------------------------------------------------------
bool	OnPIButtonClicked(SimpleDialog* pSD) {
	if(!pSD)  return false;
	pSD->UserParam = 1;		//активная кнопка
	return  true;
}
//-------------------------------------------------------------------------------------------------------------------
bool	EmptyClick(SimpleDialog* pSD) {
	return  true;
}
//-------------------------------------------------------------------------------------------------------------------
bool	FailSound() {//new
	PlaySound( "INTERF_BADTHING" ); //NEED NEW SOUND!!!
	return true;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//DEMO
void	ProcessDemoAI( int PL_NI )
{
		int AI_NI = 1 ;
		#ifdef DemoAI
			AI_NI = DemoAI(PL_NI);
		#endif

		CSectData*	m_pSect = ROOT_SCENARIO.m_pSect ;
		CDiplMenu*  m_pDipl = ROOT_SCENARIO.m_pDipl ;

		// strategy
		ROOT_SCENARIO.PROCESS_Strategy( AI_NI, PL_NI );

		//for ai attack aginst player
		int PACT  = m_pDipl->m_pDiplData->GetPactId();
		for(int i=2; i < MAX_PLAYERS ; i++) { m_pDipl->AddContract( PACT , AI_NI , i ,0); }

		CHero*	pCAH = ROOT_SCENARIO.m_pPlayer[ AI_NI ]->m_phrHero;	  
		CHero*	pCDH = ROOT_SCENARIO.m_pPlayer[ PL_NI  ]->m_phrHero;	  

		int		iCAS = 0xFFFF;					// Текущий атакуемый сектор.
		int		iHAN = pCAH[0].GetARMY_NSQ() ;	// Кол-во отрядов у героя.		
		if(iHAN<4) { return; }					// hero cant attack

		//Инициализация.
		int iCS0	= pCAH[0].getSectorID();			// В какoм секторе стоит герой.
		int iCS_PL	= pCDH[0].getSectorID();			// В какoм секторе стоит PLAYER!!!

			bool attack = false;
			iCAS = ROOT_SCENARIO.FindPathForHero( pCAH , iCS0, AI_NI , attack ); 
		
			if(!attack && iCAS != 1 && iCAS != 2 && iCAS != 5 && iCAS != 6 ) { return ; }

			if( AI_NI == 1 ) 	//france
			{
				if( iCS_PL==1 && iCS0==6 )		{ return ; } //dont attack first
			}
			if( AI_NI == 0 ) {	//england
				if( iCS_PL==6 && iCS0==1 )		{ return ; } //dont attack first
			}

			if(!attack && iCAS!=0xFFFF) { pCAH->SetSector(m_pSect,iCAS); iCAS = 0xFFFF;	} //перейти в наш сектор + сброс сектора

			// подумать - атаковать или нет ( LOOKING AT STARS ;)))
			if(attack && iCAS!=0xFFFF) {
				int chance = ( rand()%10 )/ 4 ; // ( 10 - 4 ) - "4" парам.агрессивности страны!
				if( !chance ) return ;
			}

			//Если нашли сектор для атаки то начинаем атаковать.
			if(attack && iCAS!=0xFFFF) {												
				int  WAR     = m_pDipl->m_pDiplData->GetWarId();
				int  OwnerId = m_pSect->GetSecOwner( iCAS ) ;
				
				pCDH = ROOT_SCENARIO.GetHeroInSect( iCAS , AI_NI ) ;
				if(!m_pDipl->CheckContract( WAR, AI_NI, OwnerId ) ) m_pDipl->AddContract( WAR, AI_NI, OwnerId, 5 );
				if (m_pSect->GetSecOwner(iCAS)!=PL_NI || ( m_pSect->GetSecOwner(iCAS)==PL_NI && pCDH==NULL ) ) 
				{
					//Атакуем сектор с вражеским героем.
					bool ATisWIN = SimulateBattle(pCAH, pCDH, m_pSect->m_pssdDT[iCAS]);

					if (ATisWIN) {					// победа
						(m_pSect->m_psddDT[ iCAS ])->SetOwner(AI_NI);
						m_pSect->SetSecDefeatDefence( iCAS );		//сектор захвачен-Defence=0!
						pCAH->SetSector(m_pSect, iCAS );
						if( pCDH ) { 
							ROOT_SCENARIO.m_pPlayer[pCDH->GetPlayerID()]->TryRetreat(pCDH,false); 
							if( pCDH  && pCDH->m_Alive ) //last add
								 pCDH->SetHeroDefeatArmy(); }
					}else {			
						if( m_pSect->GetSecOwner(iCS0)!=AI_NI ) { ROOT_SCENARIO.m_pPlayer[AI_NI]->TryRetreat(pCAH,false); } ;
						if( pCAH  && pCAH->m_Alive ) //last add
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

						ROOT_SCENARIO.m_Scenario.m_inAttackedSector = iCAS ;
						ROOT_SCENARIO.m_Scenario.m_bBriefingEnabled = true ;

						MissionInitData.PlayerStat = 1 ;		// игрок - в защите
						MissionInitData.SetSector_ID( iCAS );	
						MissionInitData.setHeroPresent(true);

						MissionInitData.setAttackerNation( AI_NI );	
						MissionInitData.setDefenderNation( PL_NI ); 

						return ;
					}
				} // атака сектора iCAS 
			} // if (attack)

}
//DEMO
//-------------------------------------------------------------------------------------------
void	SetBrigadeNKills( CHero* pHero , int NI ) 
{
	if(!pHero)  return;
	// Set Squads Kills
	for(int sq=0; sq < pHero->ARMY.GetAmountOfElements() ; sq++) {
		int  type = pHero->ARMY[ sq ]->getTypeGlobID() ;
		int  k    = 0;
		for(int i=0; i < CITY[NI].NBrigs && k < pHero->ARMY[ sq ]->Kills.GetAmountOfElements() ; i++) {
			if( CITY[NI].Brigs[i].MembID == type && CITY[NI].Brigs[i].Enabled && CITY[NI].Brigs[i].NKills == 0 )
			{
				CITY[NI].Brigs[i].NKills = pHero->ARMY[ sq ]->Kills[k] * 100 ;
				CITY[NI].Brigs[i].Morale = CITY[NI].Brigs[i].MaxMorale = 10000*10000 ;
				k++ ;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------
////// UPDATE NKILLS IN MISSION ( squads expa )
DLLEXPORT	void	SetBrigadeNKills( int  AttackerNationID, int DefenderNationID ) {
	int  secID  = MissionInitData.getSector_ID();					//Номер атаков.сектора.
	int  ATT_NI = MissionInitData.getAttackerNation();
	int  DEF_NI = MissionInitData.getDefenderNation();
	// Set Attacker Kills 
	SetBrigadeNKills( ROOT_SCENARIO.m_pPlayer[ ATT_NI ]->GetHeroInSector( secID ) , AttackerNationID ); // ATT_NI );
	// Set Defender Kills 
	SetBrigadeNKills( ROOT_SCENARIO.GetHeroInSect( secID , ATT_NI ) , DefenderNationID ); //DEF_NI );
}
//-------------------------------------------------------------------------------------------
DLLEXPORT	bool	HeroCanEscape( int nation ) { //grey [02.03.2004]
	if( nation < 0 || nation >= MAX_PLAYERS )  return false;
	int num = ROOT_SCENARIO.m_pSect->GetPlayerSect( nation );
	if( num == 1 )  return false;
	return  true;
	//if( num < 3 )  return false;
}
//-------------------------------------------------------------------------------------------
DLLEXPORT	int		GetSectorDefence( int secID ) { //grey [12.07.2004]
	return  ROOT_SCENARIO.m_pSect->GetSecDefence( secID );
}
//-------------------------------------------------------------------------------------------
DLLEXPORT	int		GetMissionRanks( int& attacker_RankID, int& defenderRank_ID ) { //grey [12.07.2004]
	defenderRank_ID = attacker_RankID = -1;
	int  secID  = MissionInitData.getSector_ID();					//Номер атаков.сектора.
	int  ATT_NI = MissionInitData.getAttackerNation();
	int  DEF_NI = MissionInitData.getDefenderNation();

	CHero*  pAttack = ROOT_SCENARIO.m_pPlayer[ ATT_NI ]->GetHeroInSector( secID );
	if( pAttack )  attacker_RankID = pAttack->GetRank();

	CHero*  pDefend = ROOT_SCENARIO.GetHeroInSect( secID , ATT_NI );
	if( pDefend )  {
		defenderRank_ID = pDefend->GetRank();
		return 2;
	}
	return  1;
}
//-------------------------------------------------------------------------------------------
DLLEXPORT	int		GetBigMapDisbandSQExp() {
	return DisbandSQExp;
}
DLLEXPORT	int		GetBigMapSecCaptExp() {
	return SecCaptExp;
}
DLLEXPORT	int		GetBigMapHeroDefExp() {
	return HeroDefExp;
}
DLLEXPORT	int		GetBigMapQuestDisbSquadExp() {
	return QuestDisbSquadExp;
}
DLLEXPORT	int		GetBigMapQuestCompliteExp() {
	return QuestCompliteExp;
}
//-------------------------------------------------------------------------------------------
// mission statistics data
DLLEXPORT void SetStatisticsData( int AttSQKilled, int DefSQKilled,
								  int QuestExp, int QuestNum, int QuestComplete, int QuestSQNum ) {//grey [14.05.2004]
	ROOT_SCENARIO.FormKilledNum	= ROOT_SCENARIO.FormKilledNum2	= ROOT_SCENARIO.QuestEXP	= 0 ;
	ROOT_SCENARIO.QuestNum		= ROOT_SCENARIO.QuestComplete	= ROOT_SCENARIO.QuestSQNum	= 0 ;

	if( AttSQKilled > 0 )		ROOT_SCENARIO.FormKilledNum  = AttSQKilled ;
	if( DefSQKilled > 0 )		ROOT_SCENARIO.FormKilledNum2 = DefSQKilled ;
	if( QuestExp > 0 )			ROOT_SCENARIO.QuestEXP		 = QuestExp ;
	if( QuestNum > 0 )			ROOT_SCENARIO.QuestNum		 = QuestNum ; 
	if( QuestComplete > 0 )		ROOT_SCENARIO.QuestComplete	 = QuestComplete ;
	if( QuestSQNum > 0 )		ROOT_SCENARIO.QuestSQNum	 = QuestSQNum ;
}
DLLEXPORT void SetResStatisticsData( int* AttIncResources, int* DefIncResources, int* AddRes ) {//grey [14.05.2004]
	for(int i=0; i < 10; i++) {
		ROOT_SCENARIO.IncomeRes[i]	= AttIncResources[i];
		ROOT_SCENARIO.IncomeRes2[i]	= DefIncResources[i];
		ROOT_SCENARIO.AddAIRes[i]	= AddRes[i];
	}
}
//-------------------------------------------------------------------------------------------
void AddUnlimitedEffect(int x,int y,int id);
DLLEXPORT void PlayInMissionSoundCII( char* sound, int sndID /* = -1*/) {//grey [04.03.2004]
	if( sndID != -1 ) {
		ACT( sndID ); //ACT(60);
		return;
	}
	if( sound ) {
		AddUnlimitedEffect(0,0,GetSound( sound ));
		return;
	}
}
//-------------------------------------------------------------------------------------------
int		GetBigMapResource(int res) {
	if( res < 0 || res > COAL )											return -1;
	if( gi_bmSAVE_Nation < 0 || gi_bmSAVE_Nation >= MAX_PLAYERS )		return -1;
	if( !ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ] )					return -1;
	return   ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->GetResource(res);
}
int		SetBigMapResource(int res, int amount) {
	if( res < 0 || res > COAL )											return -1;
	if( gi_bmSAVE_Nation < 0 || gi_bmSAVE_Nation >= MAX_PLAYERS )		return -1;
	if( !ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ] )					return -1;
	return   ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->SetResource(res, amount);
}
int		AddBigMapResource(int res, int amount) {
	if( res < 0 || res > COAL )											return -1;
	if( gi_bmSAVE_Nation < 0 || gi_bmSAVE_Nation >= MAX_PLAYERS )		return -1;
	if( !ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ] )					return -1;
	return   ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->AddResource(res, amount);
}
//-------------------------------------------------------------------------------------------
void	SaveBigMapDataXML() // запись всех данных по Big Map Campaign
{
	CPlayer* a_Player[MAX_PLAYERS];
	for(int i=0; i<MAX_PLAYERS; i++)  a_Player[i] = ROOT_SCENARIO.m_pPlayer[i];
	
	test_bmClassArr( ROOT_SCENARIO.m_pPLAYER_SAVE );//new

	ROOT_SCENARIO.m_pPLAYER_SAVE->GetMenuState( g_dXM, g_dYM, 
		ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID, 0, ROOT_SCENARIO.m_Scenario.m_inActivMenu, 
		ROOT_SCENARIO.m_inCurActiveAI, ROOT_SCENARIO.m_inCurHeroMove, ROOT_SCENARIO.m_inCurTurn );
	ROOT_SCENARIO.m_pPLAYER_SAVE->GetCountry( a_Player );
	ROOT_SCENARIO.m_pPLAYER_SAVE->getSECTORA( ROOT_SCENARIO.m_pSect );
	ROOT_SCENARIO.m_pPLAYER_SAVE->GetDiplData( ROOT_SCENARIO.m_pDipl->m_pDiplData );
	ROOT_SCENARIO.m_pPLAYER_SAVE->GetMessages( ROOT_SCENARIO.m_pDipl->m_pMessages );
	ROOT_SCENARIO.m_pPLAYER_SAVE->GetMissionInitData( &MissionInitData );
	ROOT_SCENARIO.m_pPLAYER_SAVE->GetQuestsData( ROOT_SCENARIO.m_pQuest );
	ROOT_SCENARIO.m_pPLAYER_SAVE->GetHelpState( ROOT_SCENARIO.m_pHelp );
	//ROOT_SCENARIO.m_pPLAYER_SAVE->GetMenuState( g_dXM, g_dYM, ROOT_SCENARIO.m_Scenario.m_inLastLMCSecID, 0 , 
	//	ROOT_SCENARIO.m_Scenario.m_inActivMenu , 
	//	ROOT_SCENARIO.m_inCurActiveAI , ROOT_SCENARIO.m_inCurHeroMove , ROOT_SCENARIO.m_inCurTurn );

	//ROOT_SCENARIO.m_pPLAYER_SAVE->SaveXML();			

	// FOR -GEC- : чтобы не сохранялась игра в Big Map - закомментируй эту строчку:
	ROOT_SCENARIO.m_pPLAYER_SAVE->SaveCurProfile();
	// FOR -GEC-


	//test_bmClassArr( ROOT_SCENARIO.m_pPLAYER_SAVE );//new
	test_bmClassArr( ROOT_SCENARIO.m_pPLAYER_SAVE );//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	test_bmClassArr( ROOT_SCENARIO.m_pPLAYER_SAVE );//new
	if(Profiles)  test_bmClassArr( Profiles );//new
	//test_bmClassArr( ROOT_SCENARIO.m_pPLAYER_SAVE );//new
}
//-------------------------------------------------------------------------------------------
int		GetBigMapNationID(int NID,DWORD& MapColor)
{
	for(int i=0; ROOT_SCENARIO.m_chNationID[i] != 0 ; i++) {
		char*  NName = ROOT_SCENARIO.m_chNationID[i];
		if( !strcmp(NatCharLo[NID], NName) || !strcmp(NatCharHi[NID], NName) ) {//char NatCharLo[32][8]; char NatCharHi[32][8];
			MapColor = ROOT_SCENARIO.m_iMapColor[i];
			return i;
		}
	}
	return NID;
}
//-------------------------------------------------------------------------------------------

void test_bmClassArr( CPlayerSAVE* PTR ) { // 4 test - MUST BE COMMENTED!!!!!
	//PTR->TestSelfPtr(); 
}

//void test_class2(BaseClass* BC){
//	if(BC->CheckDirectCasting()){
//		int N=BC->GetAmountOfElements();
//		for(int j=0;j<N;j++){
//			BaseClass* BE=BC->GetElementClass(j);
//			if(BE&&BE->CheckDirectCasting()){
//				BaseClass* BE1=(BaseClass*)BC->GetElementPtr(j,BC);
//				test_class2(BE1);
//			}
//		}
//	}
//}



//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

