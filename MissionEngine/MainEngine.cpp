
//=========================================================================================
//==========================  КОЗАКИ II ДВИЖОК ДЛЯ МИССИЙ =================================
//=========================================================================================

#include "MainEngine.h"

HINSTANCE hLib=NULL;
void UnregisterAllClasses(); //grey [18/02/2004]
DLLEXPORT void SaveDataOnExit(); //grey [18/02/2004]

BOOL APIENTRY DllMain( HANDLE hModule, 
					   DWORD  ul_reason_for_call, 
					   LPVOID lpReserved ){
	switch (ul_reason_for_call){
			case DLL_PROCESS_ATTACH:
				OnInit();
				ReLoadMessCampCII();
				g_delTask(NULL);
				break;
			case DLL_PROCESS_DETACH:
				SaveDataOnExit();
                UnregisterAllClasses();
				g_delTask(NULL);
                break;          
	};
	return TRUE;
}
void UnregisterAllClasses() { //grey [18/02/2004]
	UNREG_ALL_CLASS();
	//UNREG_CLASS("CKillEnemy00SPROC");	//UNREG_CLASS("SARMY_DESCRIPTION");
	//UNREG_CLASS("SXYF");	//UNREG_CLASS("CTYPENUM");	//UNREG_CLASS("CResource_BM");
	//UNREG_CLASS("CScript");	//UNREG_CLASS("CSAVE_MIS_STRUCT");
	//UNREG_CLASS("CRestArmy_LV");	//UNREG_CLASS("ClassArray.CRestArmy_LV");	//UNREG_CLASS("CEndMissionData");
	//UNREG_CLASS("ClassName");	//UNREG_CLASS("");
}



//ИНИЦИАЛИЗАЦИЯ++++++++++++++++++++++++++++++
bool				bMastInit			= true;
CSAVE_MIS_STRUCT	InitData;							// Данные по инициализации миссии.
CScript				SCRIPT;								// Скрипты для миссий.
CEndMissionData		EndData;							// Данные по окончании миссии.
SARMY_DESCRIPTION	AttackerArmy;
SARMY_DESCRIPTION	DefenderArmy;
char*				pPlayerName			= NULL ;		// имя для обращения к игроку
bool				Win					= false ;		// Игрок ПОБЕДИЛ!!!
bool				EndGame				= false ;		// Игра завершилась
int					TRAPS_SQ_LIMIT		= 0;			// MAX кол-во отрядов в засадах!!!
byte				Real_Attacker_NI = 0,  Real_Defender_NI = 0,  Real_AI_NI = 0,  Real_PLAYER_NI = 0;
byte				AI_NI=0,  PLAYER_NI=1,  QUESTS_NI=4,  GUARDS_NI=0;
int					AI_RankId = 0, PLAYER_RankId = 0;	// ranks
bool				PlayerAttack		= true;			// игрок - атакует
bool				REBEL				= false ;		// в секторе бунт
bool				RetreatAI			= false ;		// AI Retreat with troops
bool				CheckRatioRetreat	= true;			// AI Hero Retreat with troops
bool				CaptTribe[1000] ;					// capture tribes on map
bool				CheckZones			= false ;		// zones are captured!
int					StratZone_NN		= 0 ;			// кол-во стратежных зон в миссии
stCheckPoint		StratZone[CaptureZoneNum];			// capture zones in miss ( стратежные зоны )
int					TowerZone_NN		= 0 ;			// кол-во зон для башен
stTowerBldZone		TowerZone[1000];					// zones in miss ( for build tower&blokhause )
byte				RandomCondsType		= 0;			// тип случайных событий в миссии [15.12.2004]

//new
bool				NotFoodMessage = true;
bool				NotCoalMessage = true;
int					GetAttackerSQ_NUM = 0;
int					GetDefenderSQ_NUM = 0;

// Use AI Timers
int					RunAIWaitTime		= 0;
int					_AI_WaitTime_		= 350;			// мин. задержка АИ
int					CheckRetreatTime	= 250;

// обозначения квестов (n=7) ("текущие задания") в F1
char*  missID[50] = { "Q", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", 0 };//char* missID[50] = { "Q", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", 0 };//обозначения квестов в F1
// обозначения типов миссии в F1 (атт/деф/восст.)
char*  objID[50]  = { "x", "y", "z", 0 };
// text nations IDs
char*  natsID[50] = { "(EN)", "(FR)", "(AU)", "(PR)", "(RU)", "(EG)", 0 };
// for Build Bld on Map
char*  towers[50] = { "BldBlg%s", "BldTow%s", 0 }; //BldTow(EN)
int    TowersNum  = 2;
// Units Types For Traps
char* UnType2[100] = {	"UnitFuz(EN)",	 "UnitKEgr(EN)",	"UnitEgrNG(EN)", 
						"UnitFuzNG(FR)", "UnitKEgr(FR)",	"UnitEgrNG(FR)",
						"UnitFuzNG(AU)", "UnitKEgr(AU)",	"UnitEgrNG(AU)", 						
						"UnitFuz(PR)",	 "UnitKEgr(PR)",	"UnitEgrNG(PR)",  
						"UnitPar(RU)",	 "UnitKEgr(RU)",	"UnitEgr(RU)", 
						"UnitVol(EG)",	 "UnitKDrg(EG)",	"UnitGre(EG)",
						"UnitFuz(EN)",  "UnitFuz(FR)",		"UnitFuz(AU)", 
						"UnitFuz(PR)",  "UnitFuz(RU)",		"UnitFuz(EG)",  "TempMent(FR)", 						

						"UnitFlg(EN)",	 "UnitFlg(FR)",		"UnitFlg(AU)", 
						"UnitFlg(PR)",	 "UnitFlg(RU)",		"UnitFlg(EG)",
						"UnitOf(EN)",	 "UnitOf(FR)",		"UnitOf(AU)", 
						"UnitOf(PR)",	 "UnitOf(RU)",		"UnitOf(EG)",
						"UnitBar(EN)",	 "UnitBar(FR)",		"UnitBar(AU)", 
						"UnitBar(PR)",	 "UnitBar(RU)",		"UnitBar(EG)",
						"UnitKFlg(EN)",	 "UnitKFlg(FR)",	"UnitKFlg(AU)", 
						"UnitKFlg(PR)",  "UnitKFlg(RU)",	"UnitKFlg(EG)", 
						0 };


int		AttackerIncomeRes[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int		DefenderIncomeRes[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int		AddAIRes[10]		  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


// ТРИГЕРЫ ++++++++++++++++++++++++++++++++++++
#define tgSTART			0
#define tgSTARTMESS		1
#define tgRUN_AI		2
#define tgdelTask		3

// ТАЙМЕРЫ ++++++++++++++++++++++++++++++++++++
#define tmDELTA			0					// паузa после старта, для условий победы и поражения.
#define tmRunAI			1					// паузa перед стартом AI
#define tmCheckTime		2					
#define tmRepairTime	3	
#define tm_delTask		4					// g_delTask Timer	

// CONSTS ++++++++++++++++++++++++++++++++++++
#define REPAIR_TIME		100*( 10 - 3 * InitData.DifficultyLevel )	// 100*10=1000  100*7=700  100*4=400  100*1=100
#define Flag_delTask_TIME	1000

#define Check_FOOD_AMO		1000
#define Check_COAL_AMO		1000

#define DOWN_ENEMY_MORAL	1
#define ADD_KILLS			2
// CONSTS ++++++++++++++++++++++++++++++++++++


// ЗОНЫ ПОДСВЕТКИ +++++++++++++++++++++++++++++

// АРМИИ ПРОТИВHИКОВ ++++++++++++++++++++++++++
// ЭКСПА ++++++++++++++++++++++++++++++++++++ 
int		DisbandSQExp = 100;					// экспа за убитый отряд
int		SecCaptExp	 = 500;					// экспа за захват сектора	
int		HeroDefExp	 = 1000;				// экспа за победу над героем


//#ifdef MIS_TEST_LVA 
//	LinearArray<int,_int>	zoneID,zoneALL;
//#endif


// QUESTS ++++++++++++++++++++++++++++++++++++++++++++++
ClassArray<CKillEnemy00SPROC>	KillEnemy00_SCPR;	//Обработчики миссий.

// StratZone GUARDS ++++++++++++++++++++++++++++++++++++
ClassArray<SQUARD_LV>			SZ_GUARD;			// GUARD squadrons

// объекты GAMEOBJ +++++++++++++++++++++++++++++++++++++
GAMEOBJ		fALONE_LV, GNoFire, GTown, GTown2, GTown3, GEngineers, GAI_SelSave;


DLLEXPORT						
void OnInit()
{
	// Engineers RepairBuilding Group
	RegisterDynGroup(&GEngineers);
	RegisterDynGroup(&GAI_SelSave);
	// FORMATIONS
	RegisterFormation(&fALONE_LV,"#ALONE");
	// nofire groups
	RegisterUnits(&GNoFire, "nofire" );
	// owner town groups
	RegisterUnits(&GTown,    "town" );
	RegisterUnits(&GTown2,   "Town" );
	RegisterUnits(&GTown3,   "TOWN" );
	// capture tribes
	for(word tr=0; tr < GetNTribes() && tr < 1000 ; tr++)   CaptTribe[tr] = false ;

	REG_ALL_CLASS();
}


DLLEXPORT
void ProcessScenary()
{
	SaveSelectedUnits(AI_NI, &GAI_SelSave, 0);

	//---------------------------- Инициализация ------------------------------------------------------------------------
	if(bMastInit) 
	{
		bMastInit = false;

		DisbandSQExp = GetBigMapDisbandSQExp();		// экспа за убитый отряд
		SecCaptExp	 = GetBigMapSecCaptExp();		// экспа за захват сектора	
		HeroDefExp	 = GetBigMapHeroDefExp();		// экспа за победу над героем

		// откл. "текущие задания" в F1
		for(int i=0; missID[i]!=0 ; i++)	{ DisableMission( *missID[i] ); }		//char* missID[50] = { "Q", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", 0 };//обозначения квестов в F1
		// откл. "задания" на миссию в F1 (атт/деф/восст.)
		for(int i=0; objID[i]!=0 ; i++)		{ DisableMission( *objID[i] ); }		//char* objID[50] = { "x", "y", "z", 0 };
		// откл.стрельбы зданиям в группе "nofire"
		for(int i=0; i<8; i++) { ClearSelection(i);	}
		SelectUnits(&GNoFire, 0);
		for(int i=0; i<GetTotalAmount0(&GNoFire) ; i++) {
			OneUnit U;
			if(GetUnitInfo(&GNoFire, i, &U)) {
				U.NMask = 191; /*255*/
				SetUnitInfoNew( &U );
			}
		}
		// initial upgrades
		GAMEOBJ  UPGR;
		char     *upgrades[50] = {  "UPG17%s", "UPG18%s", "UPG20%s", 0  };		// RAZBROS ArtPus(EN) ArtGau(EN) -35%
		for(int nat=0; nat < 6 ; nat++) { // for all nations expect 6(=white)
			RegisterUpgrade(&UPGR, "UPGUNIQ00(EG)" );							// PIKA UnitVol(EG) +5
			InitialUpgradeNew(nat, &UPGR, 0xFFFF);
			for(int k=0; natsID[k]!=0 ; k++) { //for all CharLo = "(EN)"
				for(int u=0; upgrades[u]!=0; u++) { // init upgrade
					char  tmp[50] = "";
					sprintf(tmp, upgrades[ u ] , natsID[ k ] );
					RegisterUpgrade(&UPGR, tmp );
					InitialUpgradeNew(nat, &UPGR, 0xFFFF);			
				}
			}
		}
		// enable buildigs for engineers
		GAMEOBJ  BLD;
		char     *bld[50] = {  "BldBlg%s", "BldTow%s", "Bochka%s", 0 , "BldKre%s", "BldKro%s", "BldFor%s" , 0  }; // Blg &Tower &Bochka / Kre &Kro &For 
		for(int nat=0; nat < 6 ; nat++) { // for all nations expect 6(=white)
			for(int k=0; natsID[k] != 0 ; k++) { //for all CharLo = "(EN)"
				int st  =  1; //state
				int st2 = -1; /*0;*/
				for(int u=0; bld[u] != 0 || st != st2 ; u++) { // enable(st=1)/disable(st=0) bld
					if( bld[u] == 0 && st > 0 ) {
						st = st2;   continue; 
					}
					char  tmp[50] = "";
					sprintf(tmp, bld[ u ] , natsID[ k ] );
					RegisterUnitType(&BLD, tmp );					
					SetUnitEnableState(nat, &BLD, st );		//EnableUnit(nat, &BLD, (st>0 ? st : 0) );
					if( st == 0 )  st = st2;
				}
			}
		} 


		// Для инициализации миссии.
		xmlQuote	xmlInit( "CSAVE_MIS_STRUCT" ); 
		xmlInit.ReadFromFile( "Missions\\MissionEngin\\smsSAVE.xml" );
		ErrorPager Err;
		InitData.Load( xmlInit, &InitData, &Err );

		// статус игрока - атакует/защищается
		if( InitData.PlayerStat == 0 ) { 
			PlayerAttack = true  ;							// атакует
			pPlayerName  = InitData.AttackerRank_ID.str ;	// имя для обращения к игроку 
		}
		else { 
			PlayerAttack = false ; 
			pPlayerName  = InitData.DefenderRank_ID.str ;
		}

		// статус сектора - бунт или нет
		if( InitData.DefenderNation_ID == -1 )   { InitData.DefenderNation_ID = 6;  REBEL = true; }

		// Для инициализации скрипта по миссии.
		xmlQuote	xmlScript( "CScript" );
		char	chScriptName[256];
		sprintf(chScriptName,"%s%d%s","Missions\\MissionEngin\\Sector",InitData.getSector_ID(),".script.xml");
		xmlScript.ReadFromFile( chScriptName );
		SCRIPT.Load( xmlScript, &SCRIPT, &Err );

		// Инициализация квестов.
		CKillEnemy00SPROC* pKE00SPR = NULL;
		KillEnemy00_SCPR.Clear();
		SZ_GUARD.Clear();//new

		// вкл.квесты, если игрок - атакует
		if( PlayerAttack && !REBEL )
		{
			for(int i=0; i < SCRIPT.ScriptType.GetAmountOfElements(); i++) 
				if (SCRIPT.ScriptType[i]->InfID==_CKillEnemy00_) {
					pKE00SPR = new CKillEnemy00SPROC(reinterpret_cast<CKillEnemy00*>(SCRIPT.ScriptType[i]),InitData.DifficultyLevel);
					KillEnemy00_SCPR.Add(pKE00SPR);
					pKE00SPR=NULL;
				}
		} 

		// инит стратежных зон
		StratZone_NN = 0;
		if( PlayerAttack && !REBEL ) 
		{
			int   IDS[CaptureZoneNum];			
			int   STATE[CaptureZoneNum];
			for(int i=0; i < CaptureZoneNum ; i++)  IDS[i] = STATE[i] = -1; 
			getStrategicZones( IDS , STATE , CaptureZoneNum ) ; // for CHECK ZONES
			for(int i=0; i < CaptureZoneNum ; i++) {
				StratZone[i].Id		= IDS[i] ;
				StratZone[i].State	= STATE[i] ;
				if( IDS[i] > 0 ) {
					StratZone[i].rX		= GetZoneX( IDS[i] );								
					StratZone[i].rY		= GetZoneY( IDS[i] );
					StratZone_NN++ ;
					StratZone[i].Captured = false ;
				}
			}
		}
		// инит TOWERS зон
		TowerZone_NN = 0;
		if( !REBEL )
		{	
			int   IDS[1000];			
			int   STATE[1000];
			int   WEIGHT[1000];
			for(int i=0; i < 1000 ; i++)  IDS[i] = STATE[i] = WEIGHT[i] = -1; 
			getStrategicTowerZones( IDS , STATE , WEIGHT ); // for TOWER ZONES
			for(int i=0; i < 1000 ; i++) {
				TowerZone[i].Id		  = IDS[i] ;
				TowerZone[i].State	  = STATE[i] ;
				TowerZone[i].Weight	  = WEIGHT[i] ;
				TowerZone[i].BldTime  = 0;
				TowerZone[i].BldIndex = -1;
				if( IDS[i] > 0 ) {
					TowerZone[i].rX		= GetZoneX( IDS[i] );								
					TowerZone[i].rY		= GetZoneY( IDS[i] );
					TowerZone_NN++ ;
				}
			}
		}


		// "задание" на миссию в F1 (атт/деф/восст.)
		int  TypeID = 0;	// тип миссии
		if( !PlayerAttack ) TypeID = 1;
		if( PlayerAttack && REBEL ) TypeID = 2;
		for(int i=0; objID[i]!=0 ; i++)  
			if( i == TypeID )  EnableMission( *objID[ TypeID ] );
	} // ---------- End Of IF(MastInit) ----------
	

	//---------------------------- Стартовые установки ------------------------------------------------------------------
	if( Trigg( tgSTART ) ) 
	{
		SetTrigg( tgSTART, 0 );
		srand( (unsigned)time( NULL ) );

		// определим тип случайных событий в миссии [15.12.2004]
		RandomCondsType = rand()%100;
		if( RandomCondsType < 11 )					{ RandomCondsType = DOWN_ENEMY_MORAL ; } //1 
		else if( RandomCondsType < 22 )				{ RandomCondsType = ADD_KILLS ; } //2
			else if( RandomCondsType < 60 )			{ RandomCondsType = 3 ; } //3
				else if( RandomCondsType < 80 )		{ RandomCondsType = 4 ; } //4
		if( RandomCondsType > 9 || REBEL )			{ RandomCondsType = 0 ; } //0


		// переопределить ID типов войск (юнитов)	ReIDENTIFYUnitType();
		GAMEOBJ Type;
		for(int i=0; i < InitData.AttackerForce.GetAmountOfElements() ; i++) {
			RegisterUnitType(&Type, InitData.AttackerForce[i]->TypeName_sID.str );
			InitData.AttackerForce[i]->TypeIndex_ID = Type.Index ;
		}
		for(int i=0; i < InitData.DefenderForce.GetAmountOfElements() ; i++) {
			RegisterUnitType(&Type, InitData.DefenderForce[i]->TypeName_sID.str );
			InitData.DefenderForce[i]->TypeIndex_ID = Type.Index ;
		}

		// Дать хавку, шоб не померли.
		int mR = 10000000;
		for(int i=0; i < 8 ; i++) setResource( i, mR,mR,mR,mR,mR,mR );

		// звания игроков
		int  att_rankID = 0,  def_rankID = 0;
		GetMissionRanks( att_rankID , def_rankID );

		// Цвет и ресурсы игроков.
		Real_Attacker_NI = InitData.AttackerNation_ID; // !SET NEW NATION ID!
		Real_Defender_NI = InitData.DefenderNation_ID; // !SET NEW NATION ID!
		if( PlayerAttack )  { InitData.AttackerNation_ID = PLAYER_NI;	InitData.DefenderNation_ID = AI_NI;	} // !SET NEW NATION ID!
		else				{ InitData.DefenderNation_ID = PLAYER_NI;	InitData.AttackerNation_ID = AI_NI;	} // !SET NEW NATION ID!

		CResource_BM* pAIRes;			// ресурсы компа
		CResource_BM* pPlayerRes;		// ресурсы игрока
		if( PlayerAttack )	{	AI_NI			=  InitData.DefenderNation_ID;
								pAIRes			= &InitData.DefenderResource;
								AI_RankId		=  def_rankID;
								Real_AI_NI		=  Real_Defender_NI;

								PLAYER_NI		=  InitData.AttackerNation_ID;
								pPlayerRes		= &InitData.AttackerResource;
								PLAYER_RankId	=  att_rankID;
								Real_PLAYER_NI	=  Real_Attacker_NI;
		}
		else				{	AI_NI			=  InitData.AttackerNation_ID;
								pAIRes			= &InitData.AttackerResource;
								AI_RankId		=  att_rankID;
								Real_AI_NI		=  Real_Attacker_NI;

								PLAYER_NI		=  InitData.DefenderNation_ID;
								pPlayerRes		= &InitData.DefenderResource;		
								PLAYER_RankId	=  def_rankID;
								Real_PLAYER_NI	=  Real_Defender_NI;
		}
		SetPlayerNation( PLAYER_NI );

		// ресурсы игрока и ресурсы AI
		int Min_Res = 1987 ;
		for(int res=0; res<=COAL ; res++) {
			SetResource( AI_NI,		res , pAIRes->GetResource( res )	 ); 
			SetResource( PLAYER_NI, res , pPlayerRes->GetResource( res ) );
			//if( pAIRes->GetResource( res ) < Min_Res )  { 
			//	Min_Res = Min_Res/2;
			//	AddResource( AI_NI, res , Min_Res ); 

			//	AddAIRes[ res ] += Min_Res ;
			//}
		}

		// запуск AI.
		_str	DIRECTION; DIRECTION.operator=( InitData.getDirection_sID() ); 
		//prepareAI_ZONE(DIRECTION,&zoneID,&zoneALL);
		//ActivateTacticalAI( AI_NI );
		//if( PlayerAttack ) { ActivateTacticalAI(QUESTS_NI); }		//SetCOMPSTART( AI_NI ) ; 

		//Создание лагерей. 
		createCamp(&InitData,DIRECTION);
		//Создание армий. 	
		createAttackerArmy(&InitData);
		createDefenderArmy(&InitData);
		//Сохранить текущее кол-во отрядов в армиях противников. 
		AttackerArmy.setArmy(&(InitData.AttackerForce));
		DefenderArmy.setArmy(&(InitData.DefenderForce));

		// цвет квестовых юнитов
		//GUARDS_NI = AI_NI; // !SET NEW NATION ID!
		//QUESTS_NI = GetQuestsNation(AI_NI, PLAYER_NI); // !SET NEW NATION ID!
		// MAX кол-во отрядов в засадах
		TRAPS_SQ_LIMIT = GetTrapsSquadLimit( AttackerArmy.SQ_NUM , InitData.DifficultyLevel );

		// устан. NKills
		int  ATT_NI = InitData.AttackerNation_ID;
		int  DEF_NI = InitData.DefenderNation_ID;
		SetBrigadeNKills( ATT_NI , DEF_NI );//new

		//new
		int AddKills = 30 ;
		if( PlayerAttack && RandomCondsType == ADD_KILLS ) { // всем  отрядам - +30 kills
			for(int i=0; i < InitData.AttackerForce.GetAmountOfElements() ; i++) 
				if( GetObjectBranch(InitData.AttackerForce[i]->TypeIndex_ID) == 2 ) { // toka - ПУШКАМ!!! 
					for(int j=0; j < InitData.AttackerForce[i]->Kills.GetAmountOfElements(); j++) {
						InitData.AttackerForce[i]->Kills[j] += AddKills ; 
					}
					while( InitData.AttackerForce[i]->nSquarnd > InitData.AttackerForce[i]->Kills.GetAmountOfElements() ) {
						InitData.AttackerForce[i]->Kills.Add( AddKills );
					}
				}
			//for(int i=0; i < GetCITYNBrigs( ATT_NI ) ; i++)
			//	SetBrigadeNewMorale( ATT_NI , i , 0xAAAA , AddKills );
			//InitData.AttackerNation_ID;int AddKills = 30 ;
		}
		//new

		//SetBrigadeNKills( ATT_NI , DEF_NI );//new

		// Set Cannons Kills
		GAMEOBJ  CannonGrp;
		RegisterDynGroup( &CannonGrp );
		ClearSelection(ATT_NI);
		for(int sq=0; sq < InitData.AttackerForce.GetAmountOfElements() ; sq++) {
			char*  TName = InitData.AttackerForce[sq]->TypeName_sID.str;
			if( GetObjectBranch( TName ) == 2 && InitData.AttackerForce[sq]->nSquarnd > 0 ) {
				GAMEOBJ  CType;
				RegisterUnitType(&CType, TName );
				SelectUnitsType(&CType, ATT_NI , 0 );
				SaveSelectedUnits( ATT_NI, &CannonGrp , 0 );
				OneUnit  U;
				for(int id=0; id < GetTotalAmount0( &CannonGrp ) && 
							  id < InitData.AttackerForce[sq]->Kills.GetAmountOfElements() ; id++)
					if( InitData.AttackerForce[sq]->Kills[id] > 0 && GetUnitInfo( &CannonGrp, id , &U ) ) {
						U.Kills = InitData.AttackerForce[sq]->Kills[id];
						SetUnitInfo( &U );
					}
			}
		}
		ClearSelection(DEF_NI);
		for(int sq=0; sq < InitData.DefenderForce.GetAmountOfElements() ; sq++) {
			char*  TName = InitData.DefenderForce[sq]->TypeName_sID.str;
			if( GetObjectBranch( TName ) == 2 && InitData.DefenderForce[sq]->nSquarnd > 0 ) {
				GAMEOBJ  CType;
				RegisterUnitType( &CType,  TName );
				SelectUnitsType(  &CType,  DEF_NI , 0 );
				SaveSelectedUnits( DEF_NI, &CannonGrp , 0 );
				OneUnit  U;
				for(int id=0; id < GetTotalAmount0( &CannonGrp ) && 
							  id < InitData.DefenderForce[sq]->Kills.GetAmountOfElements() ; id++)
					if( InitData.DefenderForce[sq]->Kills[id] > 0 && GetUnitInfo( &CannonGrp, id , &U ) ) {
						U.Kills = InitData.DefenderForce[sq]->Kills[id];
						SetUnitInfo( &U );
					}
			}
		}
		DeleteDynGroup(&CannonGrp);

		// всем друзья (кроме игрока)	/* и нейтралов(nation=7) ) */		/* БЕЛОГО(nation=6) */
		byte f = 0; 
		for(int i=0; i < 8 ; i++) {		/*7*/
			byte df = 0;
			if( i != PLAYER_NI ) {		//&& i != 7 ) {			
				df = 1;
				for(int s=1; s <= i ; s++) df = 2*df;	}
			f += df;	
		}
		ChangeFriends( QUESTS_NI, f );
		
		// SET START POINT
		SetMissionStartPoint( &InitData ); 
		// POINT ENEMY POSITION on minimap
		SetEnemyPosPoint( &InitData );//very new

		// вкл.квесты, если игрок - атакует 
		if( PlayerAttack  && !REBEL ) {
			for (int iKE=0; iKE<KillEnemy00_SCPR.GetAmountOfElements(); iKE++){
				KillEnemy00_SCPR[iKE]->NI = QUESTS_NI; 
				KillEnemy00_SCPR[iKE]->CreateQuest( Real_Defender_NI , TRAPS_SQ_LIMIT ); //AI_NI , TRAPS_SQ_LIMIT );
			}
		} 

		// ОТДАТЬ ДЕФЕНДЕРУ здания в группе "town"
		for(int i=0; i<8; i++) { ClearSelection(i);	}
		SelectUnits(&GTown,  0);
		SelectUnits(&GTown2, 1);
		SelectUnits(&GTown3, 1);
		for(int i=0; i<8; i++) { SelChangeNation(i, InitData.DefenderNation_ID ); }
		for(int i=0; i<8; i++) { ClearSelection (i); }


		// Player Names
		SetPlayerName( AI_NI,	  "AI" );
		SetPlayerName( PLAYER_NI, "PLAYER" );
		SetPlayerName( QUESTS_NI, "QUESTS" );

		RunAIWaitTime = rand()%251 ;													//	  0 - 15cek.			
		if( PlayerAttack ) { 
			if( rand()%100 > 40 )						RunAIWaitTime += rand()%1001 ;	//	  0 - 1min		0 - 1000
			
			if( Real_AI_NI != 1 && Real_AI_NI != 4 )	RunAIWaitTime += 1000 ;			//		  1min		1000 		
			
			int  secID = InitData.Sector_ID ; 
			if( secID == 0 || secID == 6 || secID == 21 || secID == 22 )  RunAIWaitTime = rand()%171 ;

			//very new!!!
			if( !REBEL && rand()%100 > 34 ) 
				RunAIWaitTime = 3*RunAIWaitTime + 1000; // +2-9min= 0-8.45min
		}	
		else {						
			RunAIWaitTime += _AI_WaitTime_ ; //250;	//VERY NEW													//	   0 - 15cek.	
		}
		//new
		RunAIWaitTime /=  (InitData.DifficultyLevel + 1);
		RunAIWaitTime += _AI_WaitTime_ ; //VERY NEW
		if( InitData.DifficultyLevel > 1 && RunAIWaitTime > 1000 )  RunAIWaitTime = 1000;
		// summary: wait = 0s+0+15(60) - 15s+60+15(60) = 15(60) - 90(135)  [def=~15] sec


		RunTimer( tmDELTA,		150					);  
		RunTimer( tmRunAI,		RunAIWaitTime		);
		RunTimer( tmCheckTime,	CheckRetreatTime	);
		RunTimer( tmRepairTime, REPAIR_TIME			);

		//
		RunTimer( tm_delTask,   Flag_delTask_TIME	);
		//

	} // ---------- End Of IF( Trigg( tgSTART ) ) ----------


	if(TimerDoneFirst(tmDELTA)) 
	{
		GetAttackerSQ_NUM = GetSquadsAmount( InitData.AttackerNation_ID );
		GetDefenderSQ_NUM = GetSquadsAmount( InitData.DefenderNation_ID );

		// START MESSAGE
		ShowPageParam("#STARTPAGE" , pPlayerName );

		// СЛУЧАЙНЫЕ СОБЫТИЯ НА КАРТЕ
		char  textID[256], mText[256];	
		int   messId = (rand()%10)/4 +1 ; //0-1-2 +1=1..3
		if( RandomCondsType == DOWN_ENEMY_MORAL )  					// Мораль вражеским отрядам - 33
		{
			int CompNI = InitData.AttackerNation_ID;
			if( PlayerAttack )  CompNI = InitData.DefenderNation_ID;
			for(int i=0; i < GetCITYNBrigs( CompNI ) ; i++) {
				SetBrigadeNewMorale( CompNI , i , 33 );
			}

			sprintf( textID, "#DownMorale2SQ%d", messId );
			strcpy(  mText,  GetTextByID( textID ) );
			sprintf( textID, mText, pPlayerName );
			ShowPageParam("#InfoPage", textID );
		}
		if( PlayerAttack && RandomCondsType == ADD_KILLS )			// всем отрядам - +30 kills
		{
			int AddKills = 30;
			int ATT_NI = InitData.AttackerNation_ID;
			for(int i=0; i < GetCITYNBrigs( ATT_NI ) ; i++) {
				SetBrigadeNewMorale( ATT_NI , i , 0xAAAA , AddKills );
			}

			sprintf( textID, "#AddExp2SQ%d", messId );
			strcpy(  mText,  GetTextByID( textID ) );
			sprintf( textID, mText, pPlayerName );
			ShowPageParam("#InfoPage", textID );
		}
		
		// ОТДАТЬ ДЕФЕНДЕРУ ВСЕ СЕЛА НА КАРТЕ
		int rss[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for(word tr=0; tr < GetNTribes() ; tr++) { 
				SetTribeOwner( tr, InitData.DefenderNation_ID );
				byte resId,npeas,npmax;
				GetTribeGoods( tr , resId ,npeas,npmax);
				if( resId == FOOD )  rss[ resId ] += 5000 ;
				if( resId == COAL )  rss[ resId ] += 2000 ;
		}	
		if( !PlayerAttack ) {
			char s[256], tmp[256], text[256];		sprintf( text, "" );
			for(int i=0; i < 10 ; i++)
				if( rss[i] > 0 ) { 
					if(strlen(text) == 0)	{ sprintf(tmp, "" );   } 
					else					{ sprintf(tmp, ", " ); }
					sprintf( s  , "%s%d %s" , tmp , rss[i] , GetResourseName( i ) ); 
					sprintf( tmp, "%s%s" , text, s ); 
					sprintf( text, tmp ); 
				}
				sprintf( tmp, GetTextByID("#GetDefenderRes") , text ); 
				ShowPageParam( "#RessPage" , tmp );
		}
		// ДАТЬ ДЕФЕНДЕРУ РЕСУРСЫ С СЕЛ!!!
		for(int i=0; i < 10 ; i++) 
			if( rss[i] > 0 ) {
				AddResource( InitData.DefenderNation_ID , i , rss[i] ); 

				DefenderIncomeRes[ i ] += rss[i] ; // very new - 4 statistics
			}

		// УСИЛИМ Defender-AI 
		if( PlayerAttack ) {
			int  diff = InitData.DifficultyLevel + 1;
			int  add_food =  500 * DefenderArmy.SQ_NUM * diff;		// 1000		// 5000 * SQ_NUM , 3000 * SQ_NUM
			int  add_coal =  250 * DefenderArmy.SQ_NUM * diff;		//  500
			if( GetResource( AI_NI, FOOD ) < add_food ) { 
				AddResource( AI_NI, FOOD , add_food );		AddAIRes[FOOD] += add_food ;
			}
			if( GetResource( AI_NI, COAL ) < add_coal ) { 
				AddResource( AI_NI, COAL , add_coal );		AddAIRes[COAL] += add_coal ;
			}
		}
	}


	if( Trigg( tgRUN_AI ) )
	{
		bool  quit = false; 
		// check AI respoun zones
		Enumerator*	pSDIR_ENUM = ENUM.Get("SDIR_ENUM");		Enumerator*	pSDCT_ENUM = ENUM.Get("SDCT_ENUM");
		//POSITION DESCRIPTION
		_str	sDir; 
		sDir.operator=( InitData.getDirection_sID() );
		if ( pSDIR_ENUM->GetAmount() > 0 || pSDCT_ENUM->GetAmount()> 0 ) {
		int			zDIR = -1;
		int			cDIR = pSDIR_ENUM->Get(sDir.str);
		SXYF*		pDD	 = NULL;
		CZM_DRS_LV*	pZ   = NULL;
		int	  zN = GetAmountOfZones();
		for(int i=0; i < zN && !quit ; i++) {
			if( PlayerAttack )	{ pZ = reinterpret_cast<CZM_DRS_LV*>(GetExZoneInfo(GetZoneHandle(i),_CZM_DRS_LV,0)); }
			else				{ pZ = reinterpret_cast<CZM_DRS_LV*>(GetExZoneInfo(GetZoneHandle(i),_CZM_ARS_LV,0)); }
			if( pZ ) 
				for(int ii=0; ii < pZ->m_DIR_TYPE.GetAmount() && !quit ; ii++ ) {
					zDIR = (pZ->m_DIR_TYPE[ii])->m_CoverDirs;
					if( zDIR == cDIR ) {
						GAMEOBJ Z; 
						Z.Type		= 0x40000000 + 1200;
						Z.Index		= GetZoneX(i);			
						Z.Serial	= GetZoneY(i);
						if( GetUnitsAmount0( &Z, AI_NI ) > 0 && GetUnitsAmount0( &Z, PLAYER_NI ) > 0 )
							quit = true;
						}		}		}
		}

		// check AI Food amount
		if( TimerDone( tmDELTA ) && GetResource( AI_NI, FOOD ) < 5000 * DefenderArmy.SQ_NUM )  
			quit = true;
					
		if( quit ) {
			FreeTimer( tmRunAI );
			RunTimer ( tmRunAI, 10 );
			SetTrigg( tgRUN_AI, 0 );
		}
	}   


	if(TimerDoneFirst(tmRunAI))
	{
		SetTrigg( tgRUN_AI, 0 );
		// start tactical AI.
		ActivateTacticalAI( AI_NI );
		// START DEVELOPMENT AI
		if( PlayerAttack && !REBEL && DefenderArmy.SQ_NUM >= 6 )
			StartAIEx( AI_NI , "" ,0,0,0, InitData.DifficultyLevel );		 //StartAIEx(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty)
	}
	
	if( Trigg(tgdelTask) && TimerDone(tmRunAI) && TimerDone(tm_delTask))
	{
		SetTrigg( tgdelTask, 0 );
		// delete Enemy Primary Point Flag
		g_delTask( "EnemyStart" );//very new
	}

	//---------------------------- Обработка данных в миссии ------------------------------------------------------------
	if(TimerDone(tmDELTA) && !EndGame ) 
	{
		CheckAllResources( QUESTS_NI );
		if( REBEL )  CheckAllResources( AI_NI );

		// обработчик квестов
		for(int q=0; q<KillEnemy00_SCPR.GetAmountOfElements(); q++) {
			KillEnemy00_SCPR[q]->PROCESS( PLAYER_NI, q , pPlayerName );
		}

		// check captured tribes
		int AttackerNI = AI_NI;
		if( PlayerAttack )  AttackerNI = PLAYER_NI;

		for(word tr=0; tr < GetNTribes() && tr < 1000 ; tr++) 
			if( !CaptTribe[tr] && GetTribeOwner(tr) == AttackerNI ) 
			{ 
				CaptTribe[tr] = true;
				byte resId,npeas,npmax;
				GetTribeGoods( tr , resId ,npeas,npmax);

				int  amo = ( rand()%10 + 1 )*100 ;  // от 100 до 1000	

				// FOR -GEC-
				if( resId == FOOD ) { amo *= 8;  if( amo < 3000 ) amo = 3000; } // food    3000-8000  
				else				{ amo *= 5;  if( amo > 3000 ) amo = 3000; } // another  500-3000 (ft.gold&coal)
				// FOR -GEC- 

				int rnd0 = rand()%100;
				if( rnd0 < 20 )  amo +=  4000 ;
				if( rnd0 < 15 )  amo  =  4000 ;
				if( rnd0 <  3 )  amo  = 10000 ;

				if( AttackerNI == PLAYER_NI ) 
				{
					// выдать сообщение игроку
					char text[256];					//int  messId = rand()%10 / 2 ; // 0 - 1 - 2 - 3 - 4
					sprintf( text, GetTextByID("#GetTribe0"), amo , GetResourseName( resId ) );				

					int  trX=-1, trY=-1;
					GetTribeCenter(tr, trX, trY);
					AddMessageIntoMessCampCII( "#RessPage" , text , 0xADD ,-1,-1, trX , trY , "Ress" );  //0xDEF
					//ShowPageParam( "#RessPage" , text );

					//повышение экспы армии, если будет.
					CheckAddExperience( AttackerNI );
				}

				AddResource( AttackerNI , resId , amo ) ;

				AttackerIncomeRes[ resId ] += amo ; // very new - 4 statistics
			}	
		//if( PlayerAttack ) 
		//{ 
		//	for(word tr=0; tr < GetNTribes() && tr < 1000 ; tr++) 
		//		if( !CaptTribe[tr] && GetTribeOwner(tr) == PLAYER_NI ) { 
		//			CaptTribe[tr] = true;
		//			byte resId,npeas,npmax;
		//			GetTribeGoods( tr , resId ,npeas,npmax);
		//			int  amo = ( rand()%10 + 1 )*100 ;  // от 100 до 1000	

		//			// FOR -GEC-
		//			if( resId == FOOD ) { amo *= 8;  if( amo < 3000 ) amo = 3000; } // food    3000-8000  
		//			else				{ amo *= 5;  if( amo > 3000 ) amo = 3000; } // another  500-3000 (ft.gold&coal)
		//			// FOR -GEC- 
		//			int  rnd0 = rand()%100;
		//			if( rnd0 < 20 ) amo += 4000 ;
		//			if( rnd0 < 15 ) amo  = 4000 ;
		//			if( rnd0 <  5 ) amo  = 10000 ;
		//			// выдать сообщение игроку
		//			char text[256];
		//			//int  messId = rand()%10 / 2 ; // 0 - 1 - 2 - 3 - 4
		//			sprintf( text, GetTextByID("#GetTribe0"), amo , GetResourseName( resId ) );				
		//			
		//			int  trX=-1, trY=-1;
		//			GetTribeCenter(tr, trX, trY);
		//			AddMessageIntoMessCampCII( "#RessPage" , text , 0xADD ,-1,-1, trX , trY , "Ress" );  //0xDEF

		//			//ShowPageParam( "#RessPage" , text );
		//			AddResource( PLAYER_NI , resId , amo ) ;

		//			AttackerIncomeRes[ resId ] += amo ; // very new - 4 statistics
		//		}	
		//}
		//if( !PlayerAttack ) 
		//{ 
		//	for(word tr=0; tr < GetNTribes() && tr < 1000 ; tr++) 
		//		if( !CaptTribe[tr] && GetTribeOwner(tr) == AI_NI ) { // AI captured tribes
		//			CaptTribe[tr] = true;
		//			byte resId,npeas,npmax;
		//			GetTribeGoods( tr , resId ,npeas,npmax);
		//			int  amo = ( rand()%10 + 1 )*100 ;  // от 100 до 1000	
		//			if( resId == FOOD ) { amo *= 8;  if( amo < 3000 ) amo = 3000; } // food    3000-8000  
		//			else				{ amo *= 5;  if( amo > 3000 ) amo = 3000; } // another  500-3000 (ft.gold&coal)
		//			int  rnd0 = rand()%100;
		//			if( rnd0 < 20 ) amo += 4000 ;
		//			if( rnd0 < 10 ) amo  = 4000 ;
		//			if( rnd0 <  5 ) amo  = 10000 ;
		//			
		//			AddResource( AI_NI , resId , amo ) ;

		//			AttackerIncomeRes[ resId ] += amo ; // very new - 4 statistics
		//		}
		//}

		// ************************* check Victory conditions ************************* //
		// check:   Defend-Zones
		CheckZones = false ;				
		if( PlayerAttack && StratZone_NN > 0 && !REBEL ) // игрок - атакует
		{
			CheckZones	= true  ;	// check AND zones
			bool Check0	= false ;	// check OR  zones
			for(int i=0; i < StratZone_NN ; i++) {
				int id  = StratZone[i].Id ;
				if( id > 0 && !StratZone[i].Captured ) {
					GAMEOBJ Z; 
					Z.Type		= 0x40000000 + GetZoneRbyName( GetZoneName( id ) );
					Z.Index		= GetZoneX( id );								
					Z.Serial	= GetZoneY( id );								
					if( GetUnitsAmount0(&Z, PLAYER_NI ) > 10 && GetUnitsAmount0(&Z, GUARDS_NI ) < 15 ) {
						StratZone[i].Captured = true;
						int NN = 0; int orNN = 0; int orADD = 0;
						for(int n=0; n < StratZone_NN ; n++) {
							if( StratZone[n].Captured && StratZone[n].State == 1 )  NN++ ; // captured AND zones
							if( StratZone[n].State == 0 )	orNN++ ;					   // number of OR zones	
						}
						if( orNN == 0 ) Check0 = true ;
						for(int n=0; n < StratZone_NN && orNN > 0 ; n++) {  
							orADD  = 1 ;
							Check0 = false ;
							if( StratZone[n].State == 0 && StratZone[n].Captured ) {
								orADD  = 0 ;	
								Check0 = true ;											// OR zone IS captured
								break;			
							}
						}
						NN = NN + orNN - orADD;  // capt + OR-Zones - Non-capt OR-Zone
						NN = StratZone_NN - NN ; // остаток зон
						if( NN < 0 ) NN = 0 ;						
						char tmp[256], text[256];
						if( NN == 0) { /*sprintf( text, GetTextByID("#StratCheckOK") )*/; }
						else		 { sprintf( tmp,  GetTextByID("#StratCheckAdd") ,  NN );
									   sprintf( text, GetTextByID("#StratCheckZone") , tmp );	}
						//ShowPageParam("#InfoPage", text );
						if( NN != 0 )  
							AddMessageIntoMessCampCII( "#InfoPage" , text , 0xD79 ,			 // 0x 1234567890ABCDEF
								0xd79 , GetTickCount() /*NN*/ , 
								StratZone[i].rX , StratZone[i].rY , "Quest" ); // Defend Zone Guards
						//new
					}
					if( StratZone[i].State == 1 )   CheckZones	= CheckZones && StratZone[i].Captured  ; // AND
				}
			}
			CheckZones = CheckZones && Check0 ;
			if( CheckZones ) {
 				ShowPageParam("#InfoPage", GetTextByID("#StratCheckOK") );

				//new
				//повышение экспы армии, если будет.
				CheckAddExperience( PLAYER_NI );//new
				//new
			}
		}
		// check:   AI Retreat conditions
		if( TimerDone( tmCheckTime ) )
		{
			// check:   AI Retreat - not enought RESOURCES
			RetreatAI = false;
			int   MinRes	= 2000;
			bool  need_coal = false;
			if( Real_AI_NI != 4 && Real_AI_NI != 5 )  need_coal = true;
			if( !REBEL && 
				( GetResource(AI_NI, FOOD) < MinRes || ( need_coal && GetResource(AI_NI, COAL) < MinRes ) ) &&
				  GetSquadsAmount( AI_NI ) > 0 && GetSquadsAmount( AI_NI ) <= GetSquadsAmount( PLAYER_NI )/2 )
			{
				if( rand()%100 > 70 ||  GetBrigadsAmount0( AI_NI ) < 2  ) { // попытка отступить
					RetreatAI = true;
					char  tmp[256], text[256];
					if( GetResource( AI_NI, COAL ) < MinRes )  { sprintf( tmp, GetTextByID("#MT_ResCoal") ); }
					if( GetResource( AI_NI, FOOD ) < MinRes )  { sprintf( tmp, GetTextByID("#MT_ResFood") ); }
					if( InitData.HeroPresent )	{ sprintf( text, GetTextByID("#PlayerRetreat"),  pPlayerName, tmp ); }
					else						{ sprintf( text, GetTextByID("#PlayerHRetreat"), pPlayerName, tmp ); }
					ShowPageParam("#InfoPage", text );
				}
			}

			// check:   AI Retreat - осталось мало войск
			if( CheckRatioRetreat && InitData.HeroPresent && GetSquadsAmount( AI_NI ) > 0 && 
				( HeroCanEscape( Real_AI_NI ) ||   GetBrigadsAmount0( AI_NI ) < 2   ) )
			{
				float  RATIO	= 1.5 ; // = 3 ;
				int	   AI_SQ_NN	= AttackerArmy.SQ_NUM ;
				if( PlayerAttack )  { AI_SQ_NN = DefenderArmy.SQ_NUM ; }
				if( GetSquadsAmount( AI_NI ) * RATIO <= AI_SQ_NN &&					// потерял 1/3 войск
					GetSquadsAmount( AI_NI ) <= GetSquadsAmount( PLAYER_NI )/2 ) {	// перевес сил игрока в 2 раза
					bool  have_to = false;
					if(  GetBrigadsAmount0( AI_NI ) < 2 || Real_AI_NI == 1 || Real_AI_NI == 4  )  
						have_to = true;
					if( rand()%100 > 55 &&
						( have_to || ( rand()%100 < ( 40 + 10*AI_RankId ) ) ) ) 
					{ 
						RetreatAI = true; 
						char text[256];
						sprintf( text , GetTextByID("#PlayerRetreat"), pPlayerName , "." );
						ShowPageParam("#InfoPage", text );
					}
					//if( rand()%100 > 50 + 10*AI_RankId )  CheckRatioRetreat = false; 
				}
			}

			// check:   Revolted Sector Conditions
			if( REBEL ) 
			{
				float  RATIO  = 2 ; // 50%		// RATIO  = 5 - 80%
				RetreatAI	  = false;
				if( GetSquadsAmount(AI_NI) <= DefenderArmy.SQ_NUM / RATIO )  RetreatAI = true; // 50%
				int  VNum = 0;
				for(word tr=0; tr < GetNTribes() && tr<1000 ; tr++) if( GetTribeOwner(tr) == AI_NI ) VNum++;
				RetreatAI = RetreatAI || VNum <= GetNTribes() / RATIO ;							// 50%		
				RetreatAI = RetreatAI && rand()%100 > 75 ; // попытка отступить - 1 шанс из 4
				if( GetSquadsAmount(AI_NI) == 0 || VNum == 0 )  RetreatAI = true ;

				// Армия разгромила основные силы повстанцев! Остатки их формирований сложили оружие. Бунт подавлен. // 2. Армия захватила большую часть сел сектора! Моральных дух повстанцев сломлен и они сложили оружие и разошлись по домам. Бунт подавлен.
				if( RetreatAI ) {
					if( VNum <= GetNTribes() / RATIO )	{ ShowPageParam("#InfoPage" , GetTextByID("#RebelDefeat2") ); }
					else								{ ShowPageParam("#InfoPage" , GetTextByID("#RebelDefeat1") ); }
				}
			}

			RunTimer( tmCheckTime , CheckRetreatTime );
		} // if( tmCheckTime )
		
		// ***** end of check Victory conditions ***** //


		// show victory/loose Message
		if( CheckZones || RetreatAI  || GetSquadsAmount(AI_NI) == 0 || GetSquadsAmount(PLAYER_NI) == 0 ) 
		{
			EndGame = true; // Игра завершилась

			Win = true;
			// УСЛОВИЯ ПОРАЖЕНИЯ
			if( GetSquadsAmount(PLAYER_NI) == 0 ) { 
				Win = false;
				ShowPageParam("#InfoPage",GetTextByID("#PlayerLooseSquads")); 
			}
			if( GetSquadsAmount(AI_NI) == 0 ) 
			{ 
				ShowPageParam("#InfoPage",GetTextByID("#EnemyLooseSquads"));  
				
				//После сражения солдаты захватили трофеи: {CR}%d{C} Food, {CR}%d{C} Gold, {CR}%d{C} Coal.
				if( rand()%100 > 40 ) {
					char tmp[256];		
					//int  SN   = AI_SQN +1 ;   if( SN <= 0 )  SN = 1;
					int  SN = AttackerArmy.SQ_NUM;
					if( PlayerAttack )  SN = DefenderArmy.SQ_NUM;
					SN++;   if( SN <= 0 )  SN = 1;
					int  food = (rand()%SN)* 1000 +500 , gold = (rand()%SN)* 100 +500 , coal = (rand()%SN)* 500 +500 ;
					sprintf( tmp, GetTextByID("#CapturedRes") , food, gold, coal ); //#CapturedRes	
					ShowPageParam( "#RessPage" , tmp );

					AddResource( PLAYER_NI , FOOD , food ) ;
					AddResource( PLAYER_NI , GOLD , gold ) ;
					AddResource( PLAYER_NI , COAL , coal ) ;

					//new
					int* ResArr = DefenderIncomeRes;
					if( PlayerAttack )  ResArr = AttackerIncomeRes;
					ResArr[ FOOD ] += food ; // very new - 4 statistics	
					ResArr[ GOLD ] += gold ; // very new - 4 statistics	
					ResArr[ COAL ] += coal ; // very new - 4 statistics
				}
			}

			// Cообщения о ПОБЕДЕ/ПОРАЖЕНИИ
			if( Win ) {
				if( PlayerAttack )	{ ShowVictory( GetTextByID("#AttVictory") ); }
				else				{ ShowVictory( GetTextByID("#DefVictory") ); }
			} 
			else {
				if( PlayerAttack )	{ LooseGame( GetTextByID("#AttLoose") ); }
				else				{ LooseGame( GetTextByID("#DefLoose") ); }
			}
			//if( Win ) { 
			//	ShowVictory();
			//	if( PlayerAttack )	{ ShowPageParam("#AttVictory" , pPlayerName ) ; }
			//	else				{ ShowPageParam("#DefVictory" , pPlayerName ) ; }
			//} 
			//else {
			//LooseGame();
			//if( PlayerAttack )	{ ShowPageParam("#AttLoose" , pPlayerName ) ; }
			//else					{ ShowPageParam("#DefLoose" , pPlayerName ) ; }
			//}
		} 
		// ---------- End Of Victory Conditions! ----------

	} //------------------------ End Of Обработка данных в миссии ------------------------------------------------------------
	
	

	//---------------------------- Подсветка страт. зон на миникарте в миссии ------------------------------------------------------------
	if( PlayerAttack && StratZone_NN > 0 && !REBEL ) // игрок - атакует	
	{
		for(int i=0; i < StratZone_NN ; i++)
			if( StratZone[i].Id > 0 ) {
				DWORD  color0 = CaptureZoneColor0;
				if( StratZone[i].Captured )  color0 = CaptureZoneColor1;
				Add_VI_Zone( StratZone[i].rX , StratZone[i].rY , color0 );
		}

		// Strategic Zones GUARD squads
		GUARDS_NI = QUESTS_NI ; // !!!NOT RIGHT - TMP!!! ---> GUARDS_NI = AI_NI;	
		for(int i=0; i < SZ_GUARD.GetAmountOfElements() ; i++) {
			//SetBrigadeNoMorale ( GUARDS_NI, SZ_GUARD[i]->Index, true ); // No Panic Squadrons
			SetBrigadeNewMorale( GUARDS_NI, SZ_GUARD[i]->Index, 0xEEEE ); // rase Moral
		}
		if( SZ_GUARD.GetAmountOfElements() == 0 && StratZone_NN > 0 ) { // create Guard
			int  rankID = PLAYER_RankId ; 
			if( rankID < 0 )  rankID = 0;
			int guardSQN	= ( rankID + 2 ) / StratZone_NN + 1 ;
			if( guardSQN < 2 )   guardSQN = 2 ;			if( guardSQN > 10 )  guardSQN = 10 ;

			char* UnType[50] = { "UnitVol(EN)", "UnitGre(EN)",   "UnitVol(FR)", "UnitGre(FR)", 
								 "UnitVol(AU)", "UnitGre(AU)",   "UnitVol(PR)", "UnitGre(PR)", 
								 "UnitFuz(RU)", "UnitGre(RU)",	 "UnitFuz(EG)", "UnitFuz(EG)",   "TempMent(NE)", "TempMent(NE)", "TempMent(NE)", "TempMent(NE)",  0 };

			SQUARD_LV*	pSQ = NULL;
			//int  aSID		= -1 ,  dir = 0 ,  dX = 50 ,  dY = 125 ; //75;		//50; 
			int  aSID		= -1 ,  dir = 192 ,  dX = 100 ,  dY = 80 ; // dX/dY pix per.sq.
			for(int i=0; i < StratZone_NN ; i++)
				if( StratZone[i].Id > 0 ) {
					int  CurSQi = SZ_GUARD.GetAmountOfElements();
					//int  dx = -dX , dy = -dY ;  
					dir = 192; 
					int  dx = dX , dy = -dY * guardSQN/2 + dY , delta = dy/2;
					for(int s=0; s < guardSQN ; s++) {
						pSQ  = NULL;
						dx = -dx;
						dy = delta + (s/2)*dY;
						if( s == guardSQN/2 )  { dir = 64; }
						aSID = PutNewFormationBySizeIndex2(	GUARDS_NI, 
															UnType[ Real_Defender_NI*2 + s%2 ] ,  
															0, 
															(StratZone[i].rX + dx)<<4 ,
															(StratZone[i].rY + dy)<<4 , 
															dir );
						if( aSID != -1 ) {
							pSQ = new SQUARD_LV(GUARDS_NI,aSID);
							SZ_GUARD.Add(pSQ);
							SetBrigadeNoMorale(GUARDS_NI, aSID, true);		// No Panic Squadrons
							SetBrigadeAutoFormID(GUARDS_NI, aSID, 0xEEEE ); // не использовать отряд тактич.ai (CITY[NI].Brigs[aSID].AutoFormID = 0xEEEE;)
						}
					}
					if( CurSQi < SZ_GUARD.GetAmountOfElements() ) {
						int polkId  = CreatePolk();			  // создаем новый полк
						Polk* pPolk = GetPolkByIndex(polkId);
						if(pPolk) {
							for(int sq= CurSQi ; sq < SZ_GUARD.GetAmountOfElements() ; sq++ ) {
								pPolk->AddBrigadeToPolk( GUARDS_NI, SZ_GUARD[ sq ]->Index ); // отряд - в новый полк
							}
							pPolk->MovePolkTo(  StratZone[i].rX  +200 ,		    //!!!!! +200 - not right
												StratZone[i].rY  +200 , dir );  //!!!!! +200 - not right
						}
					}
				}
		}
	} // --------- страт. зоны
	


	//---------------------------- build Blg && Towers в миссии ------------------------------------------------------------
	if( TowerZone_NN > 0 && !REBEL ) // 
	{
		// find all Engineers(AI_NI)
		char     Saper[256];
		GAMEOBJ  STYPE;
		ClearSelection(AI_NI);
		for(int k=0; natsID[k]!=0 ; k++) {
			sprintf(Saper, "UnitSap%s" , natsID[ k ] );
			RegisterUnitType( &STYPE, Saper );
			SelectUnitsType(  &STYPE, AI_NI , 1 );
		}
		SaveSelectedUnits( AI_NI, &GEngineers , 1 );

		bool  build = false;
		for(int i=0; i < TowerZone_NN ; i++)
			if( TowerZone[i].Id > 0 ) 
			{
				GAMEOBJ  Z, Z4;		
				Z.Type	  = 0x40000000 + 1200;		Z.Index  = TowerZone[i].rX ;	Z.Serial  = TowerZone[i].rY ;
				Z4.Type	  = 0x40000000 + 4000;		Z4.Index = Z.Index ;			Z4.Serial = Z.Serial ;

				// Try to Create BlockHauze or Tower ( for AI_NI with Saper(ALL_NI) )	
				if( TowerZone[i].BldTime == 0 &&									//TowerZone[i].BldTime-GetTickCount() < 0
					GetUnitsAmount1( &Z, &GEngineers ) > 0 && 
					GetUnitsAmount0( &Z , PLAYER_NI ) == 0 && GetUnitsAmount0( &Z4 , PLAYER_NI ) > 160 ) 
				{
						GAMEOBJ  BldTYPE;
						char     chUnit[50] = "";
						int		 indx = -1;
						int		 TN = TowersNum;
						for(int TID = (rand()%100 < 100/TN ); TID < TN && indx < 0 ; TID++) { // 0-1 -> TowersNum
							sprintf(chUnit, towers[ TID ] , natsID[ Real_AI_NI ] );
							RegisterUnitType(&BldTYPE, chUnit );
							indx = CreateUnitByTypeAt(AI_NI, BldTYPE.Index ,TowerZone[i].rX<<4,TowerZone[i].rY<<4);
							if( indx >= 0 ) {
								build = true;
								TowerZone[i].BldIndex = indx;
								TowerZone[i].BldTime  = GetTickCount() + 1200000 ;	// ~20 min
							}
						}
				}
				// Build or Repaire Blgs
				if(   TowerZone[i].BldTime != 0 && TowerZone[i].BldIndex >= 0 && 
					( build || ( TimerDone(tmRepairTime) && GetUnitsAmount1( &Z4 , &GEngineers ) > 0 ) ) )
				{
					int      indx = TowerZone[i].BldIndex;
					OneUnit  U;
					if( GetUnitGlobalInfo( indx , &U ) && ( build || U.Life < U.MaxLife ) ) {
						GAMEOBJ  RepGroup;
						ClearSelection(AI_NI);
						for(int k=0; natsID[k]!=0 ; k++) {
							sprintf(Saper, "UnitSap%s" , natsID[ k ] );
							RegisterUnitType( &STYPE, Saper );
							SelectTypeOfUnitsInZone( &Z4 , &STYPE , AI_NI , 1 );
						}
						RegisterDynGroup( &RepGroup );
						SaveSelectedUnits( AI_NI, &RepGroup , 1 );

						SGP_RepairBuilding( &RepGroup , 1 , indx ); // go REPAIR
					}
				}
			}

		// Try to Repaire ALL Damaged Blgs
		if( TimerDone(tmRepairTime) )
		{					
			RunTimer( tmRepairTime, REPAIR_TIME ); 

			char     chUnit[50] = "";
			GAMEOBJ  BldTYPE, BldGroup;
			ClearSelection(AI_NI);
			for(int k=0; natsID[k]!=0 ; k++)						
				for(int TID=0; TID < TowersNum ; TID++) {
					sprintf(chUnit, towers[ TID ] , natsID[ k ] ); 
					RegisterUnitType( &BldTYPE, chUnit );
					SelectUnitsType(  &BldTYPE, AI_NI , 1 );
				}
			RegisterDynGroup( &BldGroup );
			SaveSelectedUnits( AI_NI, &BldGroup , 1 );

			OneUnit U;
			for(int id=0; id < GetTotalAmount0( &BldGroup ) ; id++) 
				if( GetUnitInfo( &BldGroup, id , &U ) && U.Life < U.MaxLife && U.Life > 111 ) {
					SGP_RepairBuilding( &GEngineers, 1, U.Index ); // go REPAIR
					FreeTimer( tmRepairTime );
					RunTimer ( tmRepairTime, 200 ); // ~12 sec
					break;
				}
		}

		ClearSelection(AI_NI);
	}
	//---------------------------- build Blg && Towers в миссии ------------------------------------------------------------



	//---------------------------- Change Quest single units Color в миссии ------------------------------------------------------------
	if( PlayerAttack && !REBEL )
	{
		GAMEOBJ  CType, Grp, GSaveSel;
		RegisterDynGroup( &GSaveSel );
		SaveSelectedUnits(QUESTS_NI, &GSaveSel, 0);

		RegisterDynGroup( &Grp );
		for(int i=0; UnType2[i]!=0 ; i++)
		{
			RegisterUnitType(&CType, UnType2[ i ] ); // UnType2[ Real_Defender_NI*3 + i ]   //UnType2[ ai_ni*3 + type ]
			
			ClearSelection ( QUESTS_NI );
			SelectUnitsType( &CType, QUESTS_NI, 0 );
			SaveSelectedUnits( QUESTS_NI, &Grp, 0 );

			OneUnit  U; // OB->BrigadeID = 0xFFFF;
			//for(int id=0; id < GetTotalAmount0( &Grp ) ; id++)
			for(int id=GetTotalAmount0( &Grp ); id >= 0  ; id--)
				if( GetUnitInfo( &Grp, id, &U ) && GetUnitBrigadeID( U.Index ) != 0xFFFF ) { 
					RemoveUnitFromGroup( &Grp, id );
				}
			
			ClearSelection ( QUESTS_NI );
			SelectUnits( &Grp, 0 );
			//int GrpNN = GetTotalAmount0( &Grp );
			SelChangeNation( QUESTS_NI, AI_NI );

			//
			//OneUnit  U;
			for(int id=GetTotalAmount0( &Grp ); id >= 0  ; id--) {
				if( GetUnitInfo( &Grp, id, &U ) ) {
					PanicUnitByIndex( U.Index );
				}
			}
			//void PanicUnitByIndex(int Index);
			//
		}

		ClearSelection ( QUESTS_NI );
		SelectUnits(&GSaveSel, 0);
		DeleteDynGroup(&GSaveSel);
		DeleteDynGroup(&Grp);
	}
	//---------------------------- Change Quest units Color в миссии ------------------------------------------------------------

	
	//---------------------------- Check Player Food&Coal в миссии ------------------------------------------------------------
	if( TimerDone(tmDELTA) )
	{
		if( NotFoodMessage && GetResource( PLAYER_NI , FOOD ) <= Check_FOOD_AMO )
		{
			NotFoodMessage = false;
			ShowPageParam("#InfoPage", GetTextByID( "#SEC_FOODRETREAT" ) );
		}
		if( NotCoalMessage && GetResource( PLAYER_NI , COAL ) <= Check_COAL_AMO ) 
		{
			NotCoalMessage = false;
			ShowPageParam("#InfoPage", GetTextByID( "#SEC_COALRETREAT" ) );
		}

		//FoodCoalMessage = false;
		//ShowPageParam("#InfoPage", GetTextByID( "#SECRETREAT" ) );
	}
	//---------------------------- Check Player Food&Coal в миссии ------------------------------------------------------------


	SelectUnits(&GAI_SelSave, 0);


	//#ifdef MIS_TEST_LVA 
	//if (TimerDoneFirst(10)){
	//	CAI_TOTAL_WAR*	pZD = NULL;	
	//	for (int i=0; i<zoneALL.GetAmountOfElements(); i++){
	//		pZD = reinterpret_cast<CAI_TOTAL_WAR*>(GetExZoneInfo( GetZoneHandle(zoneALL[i]), AI_IMPORTANT_POINT, 0 ));
	//		if (pZD)	pZD->Amount=0;
	//	}
	//	for (i=0; i<zoneID.GetAmountOfElements(); i++){
	//		pZD = reinterpret_cast<CAI_TOTAL_WAR*>(GetExZoneInfo( GetZoneHandle(zoneID[i]), AI_IMPORTANT_POINT, 0 ));
	//		if (pZD)	pZD->Amount=10000;
	//	}
	//}
	//#endif

	//HMODULE	H=LoadLibrary("Missions\\MissionEngin\\CaptureVillage.dll");
	//if (H!=NULL) {
	//typedef	void	(*pCALL_FUN)();
	//	pCALL_FUN pF = (pCALL_FUN)GetProcAddress(H,"CaptureVillage");
	//	pF();
	//}
	//return;
}



//------------------------ Сохранение данных после игры ( миссии ) ------------------------------------------------------
DLLEXPORT void SaveDataOnExit()  //grey [18/02/2004]
{
	EndData.WinnerNI			= Real_AI_NI;	
	EndData.AttackerNI			= InitData.AttackerNation_ID;
	EndData.DefenderNI			= InitData.DefenderNation_ID;
	EndData.AttackerExperience	= 0;									
	EndData.DefenderExperience	= 0;

	// УСЛОВИЯ ПОБЕДЫ/ПОРАЖЕНИЯ.
	if( Win )	EndData.WinnerNI = Real_PLAYER_NI;	

	// calc captured cannons
	GetAttackerSQ_NUM += GetCapturedCannons( InitData.AttackerNation_ID, Real_Attacker_NI );
	GetDefenderSQ_NUM += GetCapturedCannons( InitData.DefenderNation_ID, Real_Defender_NI );

	// calc looses
	int  AttLooseSQ = GetAttackerSQ_NUM - GetSquadsAmount( InitData.AttackerNation_ID );
	int  DefLooseSQ = GetDefenderSQ_NUM - GetSquadsAmount( InitData.DefenderNation_ID );
	//int  AttLooseSQ = AttackerArmy.SQ_NUM - GetSquadsAmount( InitData.AttackerNation_ID );	//int  AttLooseSQ = AttackerArmy.SQ_NUM - GetBrigadsAmount0( InitData.AttackerNation_ID ) - GetCannonsAmount( InitData.AttackerNation_ID ) ;
	//int  DefLooseSQ = DefenderArmy.SQ_NUM - GetSquadsAmount( InitData.DefenderNation_ID );	//int  DefLooseSQ = DefenderArmy.SQ_NUM - GetBrigadsAmount0( InitData.DefenderNation_ID ) - GetCannonsAmount( InitData.DefenderNation_ID ) ;
	
	// Strategic Zones GUARD squads
	int  GuardAliveSQ = 0 ,  GuardLooseSQ = 0 ;
	if( PlayerAttack && !REBEL ) {
		for(int i=0; i < SZ_GUARD.GetAmountOfElements() ; i++) { 
			if( GetBrigadeEnable( GUARDS_NI , SZ_GUARD[i]->Index ) )  GuardAliveSQ++;
		}
		GuardLooseSQ = SZ_GUARD.GetAmountOfElements() - GuardAliveSQ ;
		if( GUARDS_NI == AI_NI )  DefLooseSQ += GuardAliveSQ ;
	}
	
	// Экспа за разогнаные отряды.
	int  AttackEXPA = DefLooseSQ * DisbandSQExp ;
	int  DefendEXPA = AttLooseSQ * DisbandSQExp ;
	if( AttackEXPA < 0 )  AttackEXPA = 0;
	if( DefendEXPA < 0 )  DefendEXPA = 0;
	if( REBEL )  
		AttackEXPA = DefLooseSQ * GetBigMapQuestDisbSquadExp() * 2 ;
	
	// EXPA from Mission
	if( PlayerAttack ) {
		if(Win)		{	EndData.AttackerExperience += SecCaptExp;	}		// Экспа за захват сектора
		else		{	EndData.DefenderExperience += SecCaptExp;	}

		if(InitData.HeroPresent) { 
			if(Win)	{	EndData.AttackerExperience += HeroDefExp;	}		// Экспа за победу над героем
			else	{	EndData.DefenderExperience += HeroDefExp;	}
		}
	}
	if( !PlayerAttack ) {
		if(Win)		{	EndData.DefenderExperience += SecCaptExp;	}		// Экспа за захват сектора
		else		{	EndData.AttackerExperience += SecCaptExp;	}

		if(InitData.HeroPresent) { 
			if(Win) {	EndData.DefenderExperience += HeroDefExp;	}		// Экспа за победу над героем
			else	{	EndData.AttackerExperience += HeroDefExp;	}
		}
	}

	// EXPA from Quests
	int  QuestsNum		= 0;
	int  QuestsSQNum	= 0;
	int  QuestsEXPA		= 0;
	int  CompleteNumber	= 0;

	if( PlayerAttack && SZ_GUARD.GetAmountOfElements() > 0 ) { // GUARDS experience
		QuestsNum++ ;
		QuestsSQNum += SZ_GUARD.GetAmountOfElements() ;
		QuestsEXPA	+= GuardLooseSQ * GetBigMapQuestDisbSquadExp(); //експа за разогнанные отряды
		if( GuardLooseSQ > 0 && GuardLooseSQ > GuardAliveSQ ) {
			CompleteNumber++ ;
			QuestsEXPA += GetBigMapQuestCompliteExp();
		}
	}

	for(int q=0; q < KillEnemy00_SCPR.GetAmountOfElements() ; q++) {
		CKillEnemy00SPROC* pQSCR = KillEnemy00_SCPR[q];
		QuestsEXPA   += pQSCR->GetExperience();
		QuestsSQNum  += pQSCR->SQUARDS.GetAmountOfElements();
		if( pQSCR->CheckQuestActivity() ) {
			QuestsNum++;
			if( pQSCR->QuestComplite )  CompleteNumber++ ;
		}
	}

	// EXPA from captured tribes	
	int  TribesEXPA = 0;
	// ! new  comment !
	//for(word tr=0; tr < GetNTribes() ; tr++)
	//	if( GetTribeOwner(tr) == Real_Attacker_NI )   TribesEXPA += 31 + rand()%10 ; 
	// ! new  comment !


	SetStatisticsData(  DefLooseSQ , AttLooseSQ , 
						QuestsEXPA , QuestsNum , CompleteNumber , QuestsSQNum );
	SetResStatisticsData( AttackerIncomeRes , DefenderIncomeRes , AddAIRes );


	EndData.AttackerExperience += AttackEXPA ;
	EndData.AttackerExperience += QuestsEXPA ;
	EndData.AttackerExperience += TribesEXPA ;

	EndData.DefenderExperience += DefendEXPA ;

	// RESOURSES
	EndData.AttackerRes.wood	= GetResource( InitData.AttackerNation_ID , WOOD );
	EndData.AttackerRes.food	= GetResource( InitData.AttackerNation_ID , FOOD );
	EndData.AttackerRes.stone	= GetResource( InitData.AttackerNation_ID , STONE );
	EndData.AttackerRes.gold	= GetResource( InitData.AttackerNation_ID , GOLD );
	EndData.AttackerRes.iron	= GetResource( InitData.AttackerNation_ID , IRON );
	EndData.AttackerRes.coal	= GetResource( InitData.AttackerNation_ID , COAL );
	EndData.AttackerRes.recruit = InitData.AttackerResource.GetResource( RECRT );

	EndData.DefenderRes.wood	= GetResource( InitData.DefenderNation_ID , WOOD );
	EndData.DefenderRes.food	= GetResource( InitData.DefenderNation_ID , FOOD );
	EndData.DefenderRes.stone	= GetResource( InitData.DefenderNation_ID , STONE );
	EndData.DefenderRes.gold	= GetResource( InitData.DefenderNation_ID , GOLD );
	EndData.DefenderRes.iron	= GetResource( InitData.DefenderNation_ID , IRON );
	EndData.DefenderRes.coal	= GetResource( InitData.DefenderNation_ID , COAL );
	EndData.DefenderRes.recruit = InitData.DefenderResource.GetResource( RECRT );

	// save data 
	SaveEndMissionData(EndData);
	
	//Убираем мусор.
	SZ_GUARD.Clear();
	KillEnemy00_SCPR.Clear();
	
	return;
}

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


