#include "stdheader.h"
#include "CQuest.h"

//Указатели на основные управляющие классы.
extern	CROOTDRIVER			ROOT_SCENARIO;
extern	CMISSIONINIT		MissionInitData;			// Данные по инициализации миссии.
extern	CSAVE_MIS_STRUCT	smsSAVE;
extern	CEndMissionData		EndMissionData;
extern	CScript				SCRIPT_DIRECTION;			// Данные по направлениям ударов.
extern	CPlayerSAVE_STRUCT*	pSAVE;						// Структура с данными по игроку.
extern	int		gi_bmSAVE_Nation;						// nation id (from player Save)
extern	int		gi_bmMISS_FRAG_LIMIT;					// frag-limit для отрядов в миссиях

//--------------------------------------------------------------------------------------------------------------------
//----------------------   CBigMapQuest   ----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
CBigMapQuest::CBigMapQuest() 
{
	Init();
	GetQuestsData();
	GetNumber();
}
//--------------------------------------------------------------------------------------------------------------------
CBigMapQuest::CBigMapQuest( int root_turn ) 
{
	Init();
	GetQuestsData();
	GetNumber();
	ReCountTurnLimits( root_turn );
}
//--------------------------------------------------------------------------------------------------------------------
CBigMapQuest::~CBigMapQuest() 
{
	Init(); 
}
//--------------------------------------------------------------------------------------------------------------------
void	CBigMapQuest::Init()
{
	Quest.Clear();
	number	= 0 ;
	QuestID = -1;
	QuestGived = QuestComplete = false ;
	QuestOwner = QuestSecId = QuestTarNI = QuestParam  =-1;
	for(int i=0; i < 50 ; i++) ports[i]= -1 ; 

	//pROOT	= NULL;	//pSect			= NULL;	//pDipl			= NULL;	//pDiplData		= NULL;	//for(int i=0; i < MAX_PLAYERS ; i++) pPlayer[i] = NULL ;
	CROOTDRIVER*  pRTSC = &(ROOT_SCENARIO);
	pROOT		= pRTSC;		
	pSect		= pROOT->m_pSect;
	pDipl		= pROOT->m_pDipl ;		
	pDiplData	= pROOT->m_pDipl->m_pDiplData;
	for(int i=0; i<MAX_PLAYERS; i++)  pPlayer[i] = pROOT->m_pPlayer[i];

	//new
	this->ProcessQuests( pROOT , gi_bmSAVE_Nation , false );
	//Next_Turn = false;
	//new
}
//--------------------------------------------------------------------------------------------------------------------
void	CBigMapQuest::GetQuestsData()
{
	char*	quote[50] = {	"[QUEST]", "name", "owner", "recipient", "probability", "turn_limit", 
							"gold" , "main", "[END]" } ;

	int  line = -1 ;	
	char s[256];

	DString		a_DS;
	a_DS.ReadFromFile("Missions\\StatData\\Quests.dat");

	DString		a_DS2;

	CQuest_STRUCT*	pQ = NULL;

	int  n = 0 ;
	bool quit = false;
	do
	{
		a_DS.ExtractWord(a_DS2);

		// skip line
		if( a_DS2.pchar()[0] == 47 /*"/"*/ ) { a_DS.ExtractLine(a_DS2); line++; continue; }   //if( a_DS2 == "/" || a_DS2 == "//"  || a_DS2 == "///" ) { a_DS.ExtractLine(a_DS2); continue; }
		// ports
		if( a_DS2 == "ports" ) {
			a_DS.ExtractWord(a_DS2);
			int num = atoi(a_DS2.pchar());
			for(int i=0; i < num && i < 50 ; i++) {
				a_DS.ExtractWord(a_DS2);
				this->ports[i] = atoi(a_DS2.pchar()) ;
			}
		}
		// next item
		if( a_DS2 == "[QUEST]" ) {
			pQ = new CQuest_STRUCT();
			Quest.Add( pQ );
			pQ->id = n++;
		}
		else {
			if( pQ ) {
				// name
				if( a_DS2 == "name" ) {
					a_DS.ExtractWord(a_DS2);
					strcpy( pQ->name , a_DS2.pchar() );
				}
				// owner
				if( a_DS2 == "owner" ) {
					a_DS.ExtractWord(a_DS2);
					int num = atoi(a_DS2.pchar());
					for(int i=0; i < num && i < MAX_PLAYERS ; i++) {
						a_DS.ExtractWord(a_DS2);
						pQ->owner[i] = atoi(a_DS2.pchar()) ;
					}
				}
				// recipient
				if( a_DS2 == "recipient" ) {
					a_DS.ExtractWord(a_DS2);
					int num = atoi(a_DS2.pchar());
					for(int i=0; i < num && i < MAX_PLAYERS ; i++) {
						a_DS.ExtractWord(a_DS2);
						pQ->taker[i] = atoi(a_DS2.pchar()) ;
					}
				}
				// probability
				if( a_DS2 == "probability" ) {
					a_DS.ExtractWord(a_DS2);
					pQ->prob = atoi(a_DS2.pchar());
				}
				//turn_limit
				if( a_DS2 == "turn_limit" ) {
					a_DS.ExtractWord(a_DS2);
					pQ->turn_limit = atoi(a_DS2.pchar());
					pQ->wait = pQ->turn_limit/2 ? pQ->turn_limit/2 : 1 ; 
				}
				// gold
				if( a_DS2 == "gold" ) {
					a_DS.ExtractWord(a_DS2);
					pQ->gold = atoi(a_DS2.pchar());
				}
				// main
				if( a_DS2 == "main" ) {
					a_DS.ExtractWord(a_DS2);
					pQ->main = atoi(a_DS2.pchar());
				}
			}
		}
		// end
		if( a_DS2 == "[END]" || a_DS2.pchar()[0] == 0 || line > 0xFFFF )  quit = true;

		line++;
	}
	while ( !quit );

	//if( pQ->prob == -1 || pQ->turn_limit == -1 ) { 
	//	sprintf(s, "\\StatData\\Quests.dat - error at line %d." , line );
	//	VityaError(s);
	//}
	// проверка
	for(int id=0; id < GetNumber() ; id++ ) 
	{
		bool  find = false;
		for(int i=0; i < MAX_PLAYERS && !find ; i++) if( Quest[ id ]->taker[i] >= 0 ) find = true ;
		if( !find || !strcmp( Quest[id]->name , "null") || Quest[ id ]->prob < 0 || Quest[ id ]->turn_limit < 0 ) {
			sprintf(s, "\\StatData\\Quests.dat - error in file. Quest N %d." , id+1 );   VityaError(s); }
	}
	pQ = NULL;
}
//--------------------------------------------------------------------------------------------------------------------
bool	CBigMapQuest::CheckValidityOfQuest( int PlayerNI ) // проверка достоверности выданного квеста
{
	if( QuestGived && QuestID >= 0 ) 
	{
		if( !strcmp( Quest[QuestID]->name , "FallPrices" ) )   return  true; //very new!!!
		if( !strcmp( Quest[QuestID]->name , "RisePrices" ) )   return  true; //very new!!!

		if(   Quest[ QuestID ]->wait < 0 ||
			( QuestOwner >= 0 && QuestOwner < MAX_PLAYERS && !pPlayer[ QuestOwner ]->m_Alive ) ||
			( QuestTarNI >= 0 && QuestTarNI < MAX_PLAYERS && !pPlayer[ QuestTarNI ]->m_Alive ) ||
			( QuestOwner >= 0 && QuestOwner < MAX_PLAYERS && !pDipl->CheckContract( pDiplData->GetPeaceId() , QuestOwner , PlayerNI ) )	) 
		{
			// отмена выданного квеста 
			Quest[ QuestID ]->wait = Quest[ QuestID ]->turn_limit ;
			QuestID = QuestOwner = QuestSecId = QuestTarNI = QuestParam = -1 ;

			QuestGived		= false;
			QuestComplete	= true;

			return  false;
		}
	}
	return  true;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::CheckQuest( int PlayerNI , int id )
{
	// check - Gived QuestID
	if( QuestGived && QuestID == id )			{ return false; }

	// check - is main Quest Gived
	if( QuestGived && Quest[id]->main != 0 )	{ return false; }

	// check Limit of Turns
	if( Quest[id]->wait > 0 )					{ return false; }

	// check probability
	if( rand()%100 > Quest[id]->prob )			{ return false; }

	// check is quest available for Player Nation
	for(int i=0; i < MAX_PLAYERS ; i++)
		if( Quest[id]->taker[i] >= 0 && Quest[id]->taker[i] == PlayerNI )	{ return true; }

	return false;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::ProcessQuests( CROOTDRIVER* pROOT_SC , int PlayerNI , bool next_turn )
{
	Next_Turn = next_turn ;

	if( !pPlayer[ PlayerNI ]->m_Alive )		{ return false; }

	// проверка достоверности выданного квеста
	CheckValidityOfQuest( PlayerNI );										

	// wait--
	if( next_turn )  NextTurn(); //VERY NEW!!!!!!!	

	// проверка условий выполнения квеста
	QuestComplete = false;
	if( QuestGived )  QuestComplete = ChooseQuest( PlayerNI , QuestID );	

	// квест выполнен
	if( QuestComplete ) { //VERY NEW!!!!!!!
		pROOT->m_pMessages->DeleteMessage2(false, (QuestParam==1) );
		QuestID		= -1; 
		QuestParam  = -1;
		QuestGived  = false; 
		//pROOT->m_pMessages->DeleteMessage2(false);
	}		

	// пробуем дать новый квест
	if( next_turn ) {
		int  count = 0;
		for(int i=0; i < MAX_PLAYERS ; i++)  if( pPlayer[i] && pPlayer[i]->m_Alive ) count++;
		if( count < 3 )				{ return false; }

		//NextTurn();//new	
		bool gived = false;		
		for(int id=0; id < GetNumber() && !gived ; id++)					
			if( CheckQuest( PlayerNI , id ) ) 
				gived = ChooseQuest( PlayerNI , id );// id = 0 (1)
	}

	//// квест выполнен
	//if( QuestComplete ) { 
	//	pROOT->m_pMessages->DeleteMessage2(false, (QuestParam==1) );
	//	QuestID		= -1; 
	//	QuestParam  = -1;
	//	QuestGived  = false; 
	//	//pROOT->m_pMessages->DeleteMessage2(false);
	//}		

	return  QuestGived || QuestComplete ;
}
//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::ChooseQuest( int PlayerNI , int id )
{
	bool choose = false;


	if( !strcmp( Quest[id]->name , "AttackNation"   ) )		return   AttackNation	(  PlayerNI , id  );

	if( !strcmp( Quest[id]->name , "CaptureSector"  ) )		return   CaptureSector  (  PlayerNI , id  );
	if( !strcmp( Quest[id]->name , "DefendSector"   ) )		return   DefendSector	(  PlayerNI , id  );
	if( !strcmp( Quest[id]->name , "SellSector"     ) )		return   SellSector		(  PlayerNI , id  );

	if( !strcmp( Quest[id]->name , "AddGold"	    ) )		return   AddGold		(  PlayerNI , id  );
	if( !strcmp( Quest[id]->name , "DeleteGold"	    ) )		return   DeleteGold		(  PlayerNI , id  );

	if( !strcmp( Quest[id]->name , "AddRecruits"    ) )		return   AddRecruits	(  PlayerNI , id  );
	if( !strcmp( Quest[id]->name , "DeleteRecruits" ) )		return   DeleteRecruits (  PlayerNI , id  );

	if( !strcmp( Quest[id]->name , "Rebellion"		) )		return   Rebellion		(  PlayerNI , id  );
	if( !strcmp( Quest[id]->name , "DeleteResources") )		return   DeleteResources(  PlayerNI , id  );
	//if( !strcmp( Quest[id]->name , "DeleteDefence"	) ) return   DeleteDefence	(  PlayerNI , id  );

	if( !strcmp( Quest[id]->name , "AddSquad"	    ) )		return   AddSquad		(  PlayerNI , id  );


	//new!!!
	if( !strcmp( Quest[id]->name , "FallPrices"	    ) )		return   FallPrices		(  PlayerNI , id  );
	if( !strcmp( Quest[id]->name , "RisePrices"	    ) )		return   RisePrices		(  PlayerNI , id  );
	//new!!!

	return false;
}
//--------------------------------------------------------------------------------------------------------------------
bool	CBigMapQuest::AttackNation( int PlayerNI , int id ) // КВЕСТ - НАПАСТЬ НА СТРАНУ
{
	if( !QuestGived )
	{
		// find nation to attack
		int  WAR   = pDiplData->GetWarId();
		int  PEACE = pDiplData->GetPeaceId();
		int	 PACT  = pDiplData->GetPactId();

		QuestOwner = -1 ;
		QuestTarNI = -1 ;
		for(int i=0; i < MAX_PLAYERS && QuestOwner < 0 ; i++)
		{
			int  owner = Quest[id]->owner[i];
			if( owner != PlayerNI && owner >= 0 && owner < MAX_PLAYERS && pPlayer[ owner ]->m_Alive &&
				pDipl->CheckContract( PEACE , owner , PlayerNI ) ) 
			{
				for(int nat=0; nat < MAX_PLAYERS && QuestTarNI < 0 ; nat++)
					if(  nat != PlayerNI && nat != owner && pPlayer[ nat ]->m_Alive && 
						 pDipl->CheckContract( WAR  , owner    , nat ) && 
						!pDipl->CheckContract( PACT , PlayerNI , nat ) )
					{
						QuestOwner = owner ;
						QuestTarNI = nat ;
					}
			}
		}
		if( QuestOwner < 0 || !pPlayer[ QuestOwner ]->m_Alive )		{ return false; }
		if( QuestTarNI < 0 || !pPlayer[ QuestTarNI ]->m_Alive )		{ return false; }
		if( pDipl->CheckContract( PACT , PlayerNI , QuestTarNI ) )	{ return false; }


		// give quest
		char text[256], tmp[256] ;
		sprintf(text,GetTextByID( "#CQM_AttackNation0" ),pPlayer[ QuestOwner ]->GetPlayerName(),pPlayer[ QuestTarNI ]->GetPlayerName(),Quest[id]->gold);
		AddInfoMessage( text , true );
		ShowBigMapGlobalMessage( 1, 33 , 25 , text , 0 );


		//Quest[id]->wait = 9 ; // !!! 10 ходов !!!
		Quest[id]->wait = 5 ; // !!! 5 ходов !!!
		
		pROOT->m_inAttackedNation = -1 ;

		QuestID = id ;

		return   QuestGived = true ;
	}
	else
	{
		if( pROOT->m_inAttackedNation == QuestTarNI )
		{
			// quest complete!
			QuestParam = 1;//very new
			pPlayer[ PlayerNI ]->AddMoney( Quest[id]->gold );

			char text[256];
			sprintf( text, GetTextByID( "#CQM_AttackNation1" ) ,pPlayer[ QuestOwner ]->GetPlayerName(),pPlayer[ QuestTarNI ]->GetPlayerName(),Quest[id]->gold);
			int pic = 21 ; if( PlayerNI == 5 ) pic = 22;
			ShowBigMapGlobalMessage( 1, pic , 26 , text , 0 );

			QuestID = QuestOwner = QuestTarNI = -1 ;

			Quest[id]->wait = Quest[id]->turn_limit ;


			return   QuestComplete = true ;
		}
	}
		

	// add QuestMessage
	if( QuestGived && Next_Turn ) {
		char text[256] ;
		sprintf(text,GetTextByID( "#CQM_AttackNation0" ),pPlayer[ QuestOwner ]->GetPlayerName(),pPlayer[ QuestTarNI ]->GetPlayerName(),Quest[id]->gold);
		AddInfoMessage( text );
	}

	return false ;
}
//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::CaptureSector( int PlayerNI , int id ) // КВЕСТ - ЗАХВАТИТЬ СЕКТОР
{
	if( !QuestGived )
	{
		// find sectors to capture   [distance = 1-3 moves]
		int  PEACE = pDiplData->GetPeaceId();
		int	 PACT  = pDiplData->GetPactId();
		int  ALLY  = pDiplData->GetAllyId();

		int  secARR[10000],  idx = 0;
		for(int sec=0; sec < pSect->GetSectorsNum(); sec++)			// сектора игрока
			if( pSect->GetSecOwner( sec ) == PlayerNI )   secARR[ idx++ ] = sec ;

		int  secCapt[10000], NCapt = 0;
		for(int sec=0; sec < idx ; sec++) {							// ищем ближайший сектор к игроку
			int  curSect = secARR[ sec ];
			int  NN = pSect->GetNeighborsNum( curSect );
			for(int j=0; j < NN ; j++) {	
				int  Neig = pSect->GetNeighborID( curSect , j );
				//new
				if( pROOT->m_pPlayer[PlayerNI]->GetHeroInSector( Neig ) || 
					pROOT->m_pPlayer[PlayerNI]->GetHeroInSector( curSect ) ) {
					secCapt[ NCapt++ ] = Neig ;				
					int  N2 = pSect->GetNeighborsNum( Neig );
					for(int k=0; k < N2 ; k++) 
					{
						int  Neig3 = secCapt[ NCapt++ ] = pSect->GetNeighborID( Neig , k );
						int  N3 = pSect->GetNeighborsNum( Neig3 );
						for(int m=0; m < N3 ; m++)  secCapt[ NCapt++ ] = pSect->GetNeighborID( Neig3 , m );
					}
				}
				//secCapt[ NCapt++ ] = Neig ;				
				//int  N2 = pSect->GetNeighborsNum( Neig );
				//for(int k=0; k < N2 ; k++)   secCapt[ NCapt++ ] = pSect->GetNeighborID( Neig , k ) ;
				//new
			}
		}
		QuestSecId = -1 ;
		QuestOwner = -1 ;
		int  min   = 0xffff ;
		for(int sec=0; sec < NCapt ; sec++) {						// ищем сектор для захвата
			int  curSect = secCapt[ sec ];
			if(  pSect->GetSecOwner( curSect ) != PlayerNI && 
				 pSect->GetSecOwner( curSect ) >= 0 && pSect->GetSecOwner( curSect ) < MAX_PLAYERS &&
				 pSect->GetSecDefence( curSect ) < min && 
				!pDipl->CheckContract( PACT , PlayerNI , pSect->GetSecOwner( curSect ) ) ) {
					// find owner
					for(int i=0; i < MAX_PLAYERS && QuestOwner < 0 ; i++) {
						int  owner = Quest[id]->owner[i] ;
						if(  owner >= 0 && owner != PlayerNI && pPlayer[ owner ]->m_Alive &&  
							 owner != pSect->GetSecOwner( curSect ) && 
							 pDipl->CheckContract( PEACE , owner , PlayerNI ) &&
							!pDipl->CheckContract( ALLY  , owner , pSect->GetSecOwner( curSect ) ) ) 
						{
							int  NN = pSect->GetNeighborsNum( curSect );
							for(int nb=0; nb < NN ; nb++) 
								if( pSect->GetSecOwner( pSect->GetNeighborID( curSect , nb ) ) == owner ) 
								{
									QuestOwner = owner; 
									QuestSecId = curSect; 
									min = pSect->GetSecDefence( curSect ); 
								}
						}
					}
				}
		}
		if( QuestSecId <= 0 )										{ return false; } // ==0 very new!
		if( QuestOwner < 0 || !pPlayer[ QuestOwner ]->m_Alive )		{ return false; }


		// даем квест
		char text[256], tmp[256] ;
		sprintf( text, GetTextByID( "#CQM_CaptureSector0" ) ,		 
			           pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ),Quest[id]->gold); //sprintf(text, "Give Quest! Quest sector - %d   Quest owner - %d", QuestSecId , QuestOwner ) ;
		AddInfoMessage( text , true );
		ShowBigMapGlobalMessage( 1, 33 , 25 , text , 0 );


		Quest[id]->wait = 5 ; // !!! 5 ходов !!!

		QuestID = id ;

		return   QuestGived = true ;
	}
	else
	{
		// проверка условий выполнения квеста
		if( pSect->GetSecOwner( QuestSecId ) == PlayerNI )
		{
			char text[256];
			sprintf(text, GetTextByID( "#CQM_CaptureSector1" ) ,pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ),Quest[id]->gold);
			int pic = 21; if( PlayerNI == 5 ) pic = 22;

			//ShowBigMapGlobalMessage( 1, pic , 26 , text , 0 );
			if( ShowBigMapGlobalMessage( 2, pic , 26 , text , 0 ) == 1 ) {
				// quest complete!
				QuestParam = 1;//very new
				(pSect->m_psddDT[ QuestSecId ])->SetOwner( QuestOwner );
				// даем деньги за квест
				pPlayer[ PlayerNI ]->AddMoney( Quest[id]->gold );
			}


			QuestID = QuestSecId = QuestOwner = -1 ;

			Quest[id]->wait = Quest[id]->turn_limit ;

			return   QuestComplete = true ;
		}
	}
		

	// add QuestMessage
	if( QuestGived && Next_Turn ) {
		char text[256] ;
		sprintf(text,GetTextByID( "#CQM_CaptureSector0" ),pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ),Quest[id]->gold);
		AddInfoMessage( text );
	}

	return false ;
}
//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::DefendSector( int PlayerNI , int id ) // КВЕСТ - ЗАЩИТИТЬ СЕКТОР
{
	if( !QuestGived )
	{
		int  PEACE = pDiplData->GetPeaceId();
		int  PACT  = pDiplData->GetPactId();

		// find sectors to DEFEND   [distance = 1-2 moves]
		int  secARR[10000],   idx = 0;
		for(int sec=0; sec < pSect->GetSectorsNum(); sec++)			// сектора игрока
			if( pSect->GetSecOwner( sec ) == PlayerNI )   secARR[ idx++ ] = sec ;

		int  secDef[10000],  NDef = 0;
		for(int sec=0; sec < idx ; sec++) {							// ищем ближайшие к игроку сектора
			int  curSect = secARR[ sec ];
			int  NN = pSect->GetNeighborsNum( curSect );
			for(int j=0; j < NN ; j++) {	
				int  Neig = pSect->GetNeighborID( curSect , j );
				//new
				if( this->pROOT->m_pPlayer[PlayerNI]->GetHeroInSector( Neig ) || 
					this->pROOT->m_pPlayer[PlayerNI]->GetHeroInSector( curSect ) ) {
					secDef[ NDef++ ] = Neig ;
					int  N2 = pSect->GetNeighborsNum( Neig );
					for(int k=0; k < N2 ; k++)  secDef[ NDef++ ] = pSect->GetNeighborID( Neig , k );
				}
				//secDef[ NDef++ ] = Neig ;
				//int  N2 = pSect->GetNeighborsNum( Neig );
				//for(int k=0; k < N2 ; k++)  secDef[ NDef++ ] = pSect->GetNeighborID( Neig , k );
				//new
			}
		}
		QuestSecId = -1 ;
		QuestOwner = -1 ;
		// find owner
		for(int i=0; i < MAX_PLAYERS && QuestOwner < 0 ; i++) {
			int  owner = Quest[id]->owner[i];
			if( owner >= 0 && owner != PlayerNI && pPlayer[ owner ]->m_Alive && 
				pDipl->CheckContract( PEACE , owner , PlayerNI ) ) {
				int  min   = 0xffff ;
				for(int sec=0; sec < NDef ; sec++) {	// ищем сектор для ЗАЩИТЫ (c мин.защитой; угроза атаки)
					int  curSect = secDef[ sec ];
					if(  pSect->GetSecOwner( curSect ) == owner && pSect->GetSecDefence( curSect ) < min &&
						!pROOT->GetHeroInSect( curSect , 0xFFFF ) )
					{
						//new
						bool find = false;
						for(int pla=0; pla < MAX_PLAYERS && !find ; pla++) 
							if(  pla != owner && pla != PlayerNI && pPlayer[ pla ]->m_Alive &&
								!pDipl->CheckContract( PACT , pla , owner ) )
							{
								CPlayer*  pPL = pPlayer[ pla ];
								for(int h=0; h < pPL->m_inHeroNum && !find ; h++) // find where PL_NI heroes  
									if( pPL->m_phrHero[h].m_Alive && pPL->m_phrHero[h].GetRank() > 0 ) 
									{
										int  secID = pPL->m_phrHero[h].getSectorID();

										for(int n=0; n < pSect->GetNeighborsNum( secID ) && !find ; n++) {
											int  secID2 = pSect->GetNeighborID(secID, n );
											for(int e=0; e < pSect->GetNeighborsNum( secID2 ) && !find ; e++) // second level
												//if( owner == pSect->GetSecOwner( secID2 ) || 
												//	owner == pSect->GetSecOwner( pSect->GetNeighborID(secID2, e ) ) )
												if( curSect == secID2 || 
													curSect == pSect->GetNeighborID(secID2, e ) )
													find = true;
										}
									}
							}
						if( find ) {
							QuestOwner = owner ;   
							QuestSecId = curSect; 
							min = pSect->GetSecDefence( curSect ); 
						}
						//QuestOwner = owner ;   
						//QuestSecId = curSect; 
						//min = pSect->GetSecDefence( curSect ); 
						//new
					}
				}
			}
		}
		if( QuestSecId <= 0 )										{ return false; } // ==0 very new!
		if( QuestOwner < 0 || !pPlayer[ QuestOwner ]->m_Alive )		{ return false; }


		// даем квест
		pDiplData->AddContract( pDiplData->GetPassId(), PlayerNI, QuestOwner, 5 );

		char text[256], tmp[256] ;
		sprintf( text, GetTextByID( "#CQM_DefendSector0" ) ,		 
			           pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ),
					   pPlayer[ QuestOwner ]->GetPlayerName(),Quest[id]->gold);
		AddInfoMessage( text , true );
		ShowBigMapGlobalMessage( 1, 33 , 25 , text , 0 );


		Quest[id]->wait = 5 ; // !!! 5 ходов !!!

		QuestID = id ;

		return   QuestGived = true ;
	}
	else 
	{
		// проверка условий выполнения квеста
		if( Quest[id]->wait <= 0 ) 
		{
			if( pPlayer[ PlayerNI ]->GetHeroInSector( QuestSecId ) &&
				( pSect->GetSecOwner( QuestSecId ) == QuestOwner || pSect->GetSecOwner( QuestSecId ) == PlayerNI ) )
			{
				// quest complete?
				bool  complete = false;
				if( pSect->GetSecOwner( QuestSecId ) == QuestOwner )  complete = true;

				char  text[256];
				if( complete )  sprintf( text, GetTextByID( "#CQM_DefendSector1" ) ,pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ),Quest[id]->gold);
				else			sprintf( text, GetTextByID( "#CQM_DefendSector11" ) ,pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ),Quest[id]->gold);
				int pic = 21;  if( PlayerNI == 5 )  pic = 22;

				//new
				if( ShowBigMapGlobalMessage( 2 - complete, pic , 26 , text , 0 ) == 1 )	{
					complete = true;
				}
				if( complete ) {
					// quest complete!
					QuestParam = 1;//very new
					(pSect->m_psddDT[ QuestSecId ])->SetOwner( QuestOwner );
					// даем деньги за квест
					pPlayer[ PlayerNI ]->AddMoney( Quest[id]->gold );
				}
				//new
                

				QuestID = QuestSecId = QuestOwner = -1 ;

				Quest[id]->wait = Quest[id]->turn_limit ;

				return   QuestComplete = true ;
			}
			else
			{
				QuestID = QuestSecId = QuestOwner = -1 ;

				Quest[id]->wait = Quest[id]->turn_limit ;


				return   QuestComplete = true ;
			}
		}
	}


	// add QuestMessage
	if( QuestGived && Next_Turn ) {
		char text[256] ;
		sprintf(text,GetTextByID( "#CQM_DefendSector0" ),pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ),pPlayer[ QuestOwner ]->GetPlayerName(),Quest[id]->gold);
		AddInfoMessage( text );
	}

	return false ;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::SellSector( int PlayerNI , int id ) // КВЕСТ - ПРОДАТЬ СЕКТОР
{
	if( !QuestGived )
	{
		int  PEACE = pDiplData->GetPeaceId();

		// check conditions
		if( pSect->GetPlayerSect( PlayerNI ) < 3 )			{ return false; }		
		//if( num < pSect->GetSectorsNum()/MAX_PLAYERS )	{ return false; }

		// get sector for buy   [distance = 1 move (only neighbor)]
		int  secARR[10000],  idx = 0;
		for(int sec=0; sec < pSect->GetSectorsNum(); sec++)				// сектора игрока
			if( pSect->GetSecOwner( sec ) == PlayerNI ) //&& pSect->GetSecIDbyOwner( PlayerNI ) != sec )   
				secARR[ idx++ ] = sec;

		int  secSell[10000],  NSell = 0;
		for(int i=0; i < MAX_PLAYERS && QuestOwner < 0 ; i++) 
		{
			int  owner = Quest[id]->owner[i];
			if( owner >= 0 && owner != PlayerNI && pPlayer[ owner ]->m_Alive &&
				pDiplData->CheckContract( PEACE, owner, PlayerNI ) ) { //new
				QuestSecId = -1;
				NSell = 0;
				for(int sec=0; sec < idx ; sec++) {								// ищем ближайшие сектора к owner
					int  curSect = secARR[ sec ];					
					//new
					if( pSect->GetHistoryOwner( curSect ) == owner || rand()%100 < 60 ) {
						int  NN = pSect->GetNeighborsNum( curSect );
						for(int j=0; j < NN ; j++) 
							if(  pSect->GetSecOwner( pSect->GetNeighborID( curSect , j ) ) == owner &&
								!pROOT->GetHeroInSect( curSect , 0xFFFF ) ) 
							{
									QuestOwner = owner ;  
									QuestSecId = curSect ; 
									secSell[ NSell++ ] = curSect ;
							}
					}
					//int  NN = pSect->GetNeighborsNum( curSect );
					//for(int j=0; j < NN ; j++) 
					//	if(  pSect->GetSecOwner( pSect->GetNeighborID( curSect , j ) ) == owner &&
					//		!pROOT->GetHeroInSect( curSect , 0xFFFF ) ) 
					//	{
					//		QuestOwner = owner ;  
					//		QuestSecId = curSect ; 
					//		secSell[ NSell++ ] = curSect ;
					//	}
					//new
				}
			}
		}	
		if( QuestSecId < 0 )										{ return false; }	
		if( QuestOwner < 0 || !pPlayer[ QuestOwner ]->m_Alive )		{ return false; }


		// give quest
		int  money = Quest[id]->gold * ( pSect->GetSecDefence( QuestSecId ) + 1 ); // Get Money Ammount!
		if( pSect->GetResourceID( QuestSecId ) == 2 ) money += Quest[id]->gold ;

		char text[256],  tmp[256] ;
		sprintf( text, GetTextByID( "#CQM_SellSector0" ) , 
					   pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ), money );

		// add message on desk
		pDipl->m_pMessages->AddMessage( -11 , 2 , 33 ,
										pDipl->m_pMessLtrs->Get(11),pDipl->m_pMessHeads->Get(11),text, -0x5e11 , true );


		sprintf( text, GetTextByID( "#CQM_SellSector00" ) , 
					   pPlayer[ QuestOwner ]->GetPlayerName(),pSect->GetSectorName( QuestSecId ), money );

		// show mess
		ShowBigMapGlobalMessage( 1, 33 , 11 , text , 0 );


		Quest[id]->wait = 0 ; // !!! 0 ходов !!!

		QuestID = id ;

		return   QuestGived = true ;
	}
	else {
		// проверка условий выполнения квеста
		if( QuestParam == 1 ) 
		{
			int  money = Quest[id]->gold * ( pSect->GetSecDefence( QuestSecId ) + 1 ); // Get Money Ammount!
			if( pSect->GetResourceID( QuestSecId ) == 2 ) money += Quest[id]->gold ;

			// quest complete!
			QuestParam = 1;//very new
			(pSect->m_psddDT[ QuestSecId ])->SetOwner( QuestOwner );
			
			// add money
			pPlayer[ PlayerNI ]->AddMoney( money );

			char text[256],  tmp[256] ;
			sprintf( text, GetTextByID( "#CQM_SellSector1" ) ,pPlayer[ QuestOwner ]->GetPlayerName(), money );
			int pic = 21;  if( PlayerNI == 5 )  pic = 22;
			
			ShowBigMapGlobalMessage( 1, pic , 11 , text , 0 );
		}
		//else { ShowBigMapGlobalMessage( 1, 33 , 11 , GetTextByID( "#CQM_SellSector2" ) , 0 );	}


		QuestID = QuestSecId = QuestOwner = -1 ;

		Quest[id]->wait = Quest[id]->turn_limit ;


		return  QuestComplete = true ;
	}
	
	//!!!NEW!!!

	return  true ;
}

//bool	CBigMapQuest::SellSector( int PlayerNI , int id ) // КВЕСТ - ПРОДАТЬ СЕКТОР
//{
//	// check conditions
//	int  num = 0;
//	for(int sec=0; sec < pSect->GetSectorsNum() ; sec++) if( pSect->GetSecOwner( sec ) == PlayerNI ) num++;
//	if( num < pSect->GetSectorsNum()/MAX_PLAYERS )	{ return false; }
//
//	// get quest sector
//	int  secARR[10000],  idx = 0;
//	for(int sec=0; sec < pSect->GetSectorsNum(); sec++)				// сектора игрока
//		if( pSect->GetSecOwner( sec ) == PlayerNI && 
//			pSect->GetSecIDbyOwner( PlayerNI ) != sec )   secARR[ idx++ ] = sec ;
//
//	int  OwnerId = -1 ;
//	int  SecId   = -1;
//	int  secSell[10000],  NSell = 0;
//	for(int i=0; i < MAX_PLAYERS && OwnerId < 0 ; i++) 
//	{
//		int  owner = Quest[id]->owner[i];
//		if( owner >= 0 && owner != PlayerNI && pPlayer[ owner ]->m_Alive ) {
//			SecId = -1;
//			NSell = 0;
//			for(int sec=0; sec < idx ; sec++) {								// ищем ближайшие сектора к owner
//				int  curSect = secARR[ sec ];
//				int  NN = pSect->GetNeighborsNum( curSect );
//				for(int j=0; j < NN ; j++) 
//					if(  pSect->GetSecOwner( pSect->GetNeighborID( curSect , j ) ) == owner &&
//						!this->pROOT->GetHeroInSect( curSect , 0xFFFF ) ) 
//					{
//							OwnerId = owner ;  
//							SecId	= curSect ; 
//							secSell[ NSell++ ] = curSect ;
//					}
//			}
//		}
//	}	
//	if( SecId < 0 )											{ return false; }	
//	if( OwnerId < 0 || !pPlayer[ OwnerId ]->m_Alive )		{ return false; }
//
//	// give quest
//	int  money = Quest[id]->gold * ( pSect->GetSecDefence( SecId ) + 1 );
//	if( pSect->GetResourceID( SecId ) == 2 ) money += Quest[id]->gold ;
//
//	char text[256],  tmp[256] ;
//	sprintf( text, GetTextByID( "#CQM_SellSector0" ) , 
//			       pPlayer[ OwnerId ]->GetPlayerName(),pSect->GetSectorName( SecId ), money );
//
//	if( ShowBigMapGlobalMessage( 2, 33 , 11 , text , 0 ) == 1 ) // ok
//	{
//		// quest complete!
//		QuestParam = 1;//very new
//		(pSect->m_psddDT[ SecId ])->SetOwner( OwnerId );
//		
//		// add money
//		pPlayer[ PlayerNI ]->AddMoney( money );
//
//		sprintf( text, GetTextByID( "#CQM_SellSector1" ) ,pPlayer[ OwnerId ]->GetPlayerName(), money );
//		int pic = 21;  if( PlayerNI == 5 )  pic = 22;
//		ShowBigMapGlobalMessage( 1, pic , 11 , text , 0 );
//	}
//	else { ShowBigMapGlobalMessage( 1, 33 , 11 , GetTextByID( "#CQM_SellSector2" ) , 0 );	}
//
//
//	Quest[id]->wait = Quest[id]->turn_limit ;
//
//	return   true ;
//}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::AddGold( int PlayerNI , int id ) // КВЕСТ - ДАЮТСЯ РЕСУРСЫ
{
	int  money = Quest[id]->gold + ( rand()%10 + ( rand()%10 ) / 6 ) * 100 ;

	// check Port/not port param
	bool  port = false ;
	for(int i=0; i < 50 && !port ; i++) 
		if( rand()%100 < 15 && pSect->GetSecOwner( ports[i] ) == PlayerNI )  port = true;

	// check - if !port
	if( !port && rand()%100 < 50 )							{ return false; }

	// random message id
	int  messId = rand()%10 / 2 ; // 0 - 1 - 2 - 3 - 4

	// give quest - ADD Gold
	char text[256],  tmp[256] ;
	sprintf( tmp, "#CQM_LandAddGold%d" , messId ); 
	if( port )   { sprintf( tmp, "#CQM_SeaAddGold%d" , messId ); money = money * 1.5 / 100 ; money *= 100 ; } /*2*/

	// add money
	pPlayer[ PlayerNI ]->AddMoney( money );

	// give quest
	sprintf( text, GetTextByID( tmp ) , money ); 
	int pic = 21;  if( PlayerNI == 5 )  pic = 22;
	ShowBigMapGlobalMessage( 1, pic , 11 , text , 0 );


	Quest[id]->wait = Quest[id]->turn_limit ;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::DeleteGold( int PlayerNI , int id ) // КВЕСТ - ОТНИМАЮТСЯ РЕСУРСЫ
{
	int  money = Quest[id]->gold + ( rand()%10 - ( rand()%10 ) / 6 ) * 100 ;

	// check Port/not port param
	bool  port = false ;
	for(int i=0; i < 50 && !port ; i++) 
		if( rand()%100 < 15 && pSect->GetSecOwner( ports[i] ) == PlayerNI )   port = true ;

	// check - if !port
	if( !port && rand()%100 < 50 )							{ return false; }

	// random message id
	int  messId = rand()%10 / 2 ; // 0 - 1 - 2 - 3 - 4

	// check conditions
	if( pPlayer[ PlayerNI ]->GetResource( GOLD ) < 1000  )	{ return false; }
	if( pPlayer[ PlayerNI ]->GetResource( GOLD ) < money )	{ money = pPlayer[ PlayerNI ]->GetResource( GOLD )/2 ; }

	// give quest - DEC Gold
	char text[256],  tmp[256] ;
	sprintf( tmp, "#CQM_LandDelGold%d" , messId ); 
	if( port )   sprintf( tmp, "#CQM_SeaDelGold%d" , messId ); 

	// delete money
	pPlayer[ PlayerNI ]->AddMoney( - money );

	// give quest
	sprintf( text, GetTextByID( tmp ) , money ); 
	int pic = 16;  if( PlayerNI == 5 )  pic = 17;
	ShowBigMapGlobalMessage( 1, pic , 11 , text , 0 );


	Quest[id]->wait = Quest[id]->turn_limit ;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::AddRecruits( int PlayerNI , int id ) // КВЕСТ - ДАЮТСЯ РЕКРУТЫ
{
	// find target sector
	int  secID = -1 ,  recrt = 0xFFFF ;
	for(int sec=0; sec < pSect->GetSectorsNum(); sec++)				// сектора игрока
		if( pSect->GetSecOwner( sec ) == PlayerNI && pSect->m_pssdDT[ sec ]->m_iRecruits <= recrt ) { 
			secID = sec ;   recrt = pSect->m_pssdDT[ secID ]->m_iRecruits ;   
		}
	if( secID < 0 )										{ return false; }

	int  pop	 = pSect->GetSecPopulation( secID ) + 1 ;
	int  add_rec = pop * SECT_MAX_RECRTS ;	
	if( add_rec - recrt < SECT_REG_RECRTS + 10 )		{ return false; }

	// give quest - ADD Recruits
	pSect->m_pssdDT[ secID ]->m_iRecruits = add_rec ;
	pPlayer[ PlayerNI ]->SetRecruit() ;

	// show message
	char  text[256];
	sprintf( text, GetTextByID( "#CQM_AddRecruits" ) , 	pSect->GetSectorName( secID ) , add_rec );
	ShowBigMapGlobalMessage( 1, 33 , 11 , text , 0 );


	Quest[id]->wait = Quest[id]->turn_limit ;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::DeleteRecruits( int PlayerNI , int id ) // КВЕСТ - ОТНИМАЮТСЯ РЕКРУТЫ
{
	// find target sector
	int  secID = -1 ,  recrt = 0xFFFF ;
	for(int sec=0; sec < pSect->GetSectorsNum(); sec++)				// сектора игрока
		if( pSect->GetSecOwner( sec ) == PlayerNI && pSect->m_pssdDT[ sec ]->m_iRecruits <= recrt ) { 
			secID = sec ;   recrt = pSect->m_pssdDT[ secID ]->m_iRecruits ;   
		}
	if( secID < 0 )						{ return false; }
	if( recrt < 10 )					{ return false; }

	// give quest - KILL Recruits
	pSect->m_pssdDT[ secID ]->m_iRecruits = 0 ;
	pPlayer[ PlayerNI ]->SetRecruit() ;
	
	// show message
	char  text[256];
	sprintf( text, GetTextByID( "#CQM_DeleteRecruits" ) , pSect->GetSectorName( secID ) , recrt );
	ShowBigMapGlobalMessage( 1, 33 , 11 , text , 0 );


	Quest[id]->wait = Quest[id]->turn_limit ;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::Rebellion( int PlayerNI , int id ) // КВЕСТ - БУНТ В СЕКТОРЕ
{
	// check conditions
	if( pSect->GetPlayerSect( PlayerNI ) < 7 )	{ return false; }
	if( rand()%100 < 50 )						{ return false; }

	// find target sector
	int  secID = -1 ,  min = 0xFFFF ;
	for(int sec=0; sec < pSect->GetSectorsNum() ; sec++)			// ищем сектор для БУНТА (c мин.защитой)
		if( pSect->GetSecOwner( sec ) == PlayerNI && pROOT->GetHeroInSect( sec , 0xFFFF ) == NULL && 
			pSect->GetSecDefence( sec ) <= min ) { 
			secID = sec ;   min = pSect->GetSecDefence( sec ) ;   
		}
	if( secID < 0 )								{ return false; }

	int  money = Quest[id]->gold + ( rand()%10 + ( rand()%10 ) / 6 ) * 100 ; // + 0-1000
	money = money * ( min + 1 );

	// show message
	char  text[256];
	if( pPlayer[ PlayerNI ]->GetResource( GOLD ) >= money )
	{
		sprintf( text, GetTextByID( "#CQM_SectorRebellion0" ) , pSect->GetSectorName( secID ) , money );
		
		if( ShowBigMapGlobalMessage( 2, 33 , 11 , text , 0 ) == 1 ) { // ok
			// delete money
			pPlayer[ PlayerNI ]->AddMoney( -money );

			// check complete chance
			bool  done = rand()%100 < 90 ;	// 50 ;
			// quest complete!
			if( done ) { 
				sprintf( text, GetTextByID( "#CQM_SectorRebellion1" ) , pSect->GetSectorName( secID ) );
				ShowBigMapGlobalMessage( 1, 33 , 11 , text , 0 ); 
			}
			else { 
				(pSect->m_psddDT[ secID ])->SetOwner( -1 );
				pSect->m_pssdDT[ secID ]->m_inDefence = 0 ;
				ShowBigMapGlobalMessage( 1, 24 , 11 , GetTextByID( "#CQM_SectorRebellion2" ) , 0 ); 
			}
		}
		else { 
				(pSect->m_psddDT[ secID ])->SetOwner( -1 );
				pSect->m_pssdDT[ secID ]->m_inDefence = 0 ;
				ShowBigMapGlobalMessage( 1, 24 , 11 , GetTextByID( "#CQM_SectorRebellion3" ) , 0 );	
		}
	}
	else { 
		(pSect->m_psddDT[ secID ])->SetOwner( -1 );
		pSect->m_pssdDT[ secID ]->m_inDefence = 0 ;
		sprintf( text, GetTextByID( "#CQM_SectorRebellion4" ) , pSect->GetSectorName( secID ) );
		ShowBigMapGlobalMessage( 1, 24 , 11 , text , 0 ); 
	}


	Quest[id]->wait = Quest[id]->turn_limit ;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::DeleteResources( int PlayerNI , int id )  // КВЕСТ - ОТНИМАЮТСЯ РЕСУРСЫ
{
	// find target sector
	int  secID = -1 ,  min = 0xFFFF ;
	for(int sec=0; sec < pSect->GetSectorsNum() ; sec++)			// ищем сектор (c мин.защитой)
		if( pSect->GetSecOwner( sec ) == PlayerNI && pSect->GetSecDefence( sec ) <= min ) { 
			secID = sec ;   min = pSect->GetSecDefence( secID );   
		}
	if( secID < 0 )						{ return false; }

	// give quest - DEL Resources		// (all resources = 0)
	pSect->m_pssdDT[ secID ]->m_iSabotageID += 10 ;
	
	// show message
	char  text[256];
	sprintf( text, GetTextByID( "#CQM_DeleteResources" ) , pSect->GetSectorName( secID ) );
	ShowBigMapGlobalMessage( 1, 24 , 11 , text , 0 );


	Quest[id]->wait = Quest[id]->turn_limit ;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::DeleteDefence( int PlayerNI , int id )  // КВЕСТ - ЗАЩИТА СЕКТОРА -1
{
	if( !Next_Turn && rand()%100 > 50 )  return false;//very new

	int  WAR  = pDiplData->GetWarId();
	int  ALLY = pDiplData->GetAllyId();

	// find PlayerNI sectors
	int  secARR[1000];
	int  idx = pSect->GetPlayerSect( secARR , 1000 , PlayerNI );

	// find target sector
	bool  aiHeroPresent = false;
	int   secID = -1 ,  secID0 = -1 ;
	for(int i=0; i < idx && !aiHeroPresent ; i++) 
	{	
		// find neighbors
		int  curSect = secARR[ i ];
		int  NN		 = pSect->GetNeighborsNum( curSect );
		for(int j=0; j < NN && !aiHeroPresent ; j++) {	
			int Neig  = pSect->GetNeighborID( curSect , j );
			int owner = pSect->GetSecOwner( Neig );
			// find enemy neighbors (in WAR!)
			if(	  owner != PlayerNI && pSect->GetSecDefence( curSect ) > 0 &&
				  pDipl->CheckContract( WAR , PlayerNI , owner ) && rand()%100 > 60 &&
				( pSect->GetSecDefence( curSect ) < MAX_DEFENCE || rand()%10 == 0 ) ) 
			{  
				secID = curSect;
				if( secID0 < 0 )  secID0 = curSect;// another				
				// check AI-hero in sector
				if( secID > 0 ) {
					CHero*	pH = pROOT->GetHeroInSect( Neig , PlayerNI );
					if( pH ) {
						int AI_NI = pH->GetPlayerID();
						if( owner == AI_NI || pDipl->CheckContract(ALLY, AI_NI , owner ) ) 
							aiHeroPresent = true;
						aiHeroPresent = aiHeroPresent && rand()%100 > 25;
					}
				}
			}
		}
	}
	if( secID < 0 )  secID = secID0;
	if( secID < 0 )						{ return false; }
	

	// give quest - sector Defence - 1
	pSect->m_pssdDT[ secID ]->m_inDefence-- ;
	pSect->m_pssdDT[ secID ]->m_iSabotageID += 100 ;

	// show message
	char  text[256];
	sprintf( text, GetTextByID( "#CQM_DeleteDefence" ) , pSect->GetSectorName( secID ) );
	ShowBigMapGlobalMessage( 1, 24 , 11 , text , 0 );


	if( id >= 0 )  Quest[id]->wait = Quest[id]->turn_limit;

	return true;
}
//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::AddSquad( int PlayerNI , int id )		// КВЕСТ - ПРИСОЕДИНИТЬ ОТРЯД
{
	int  sq = 0 ; // 1й тип войск

	// check  m_iFreeSQNum
	CHero*	pH  = &(pPlayer[ PlayerNI ]->m_phrHero[ 0 ]) ;

	if( !pH || !pH->m_Alive || !pH->GetARMY_FreeNSQ() )					{ return false; }
	if(	pH->ARMY[ sq ]->getSQ_NUM() >= pH->ARMY[ sq ]->getSQ_MAX() )	{ return false; }

	// check  GOLD
	int  add_m = ( rand()%10 - rand()%10 ) * 5 ; //very new 
	int  money = Quest[id]->gold + add_m ;		if( money < 10 )  money = Quest[id]->gold; //very new     

	if( pPlayer[ PlayerNI ]->GetResource( GOLD ) < money  )				{ return false; }

	// unit name
	char  name[256] ;
	strcpy( name , NATIONS[0].Mon[ pH->ARMY[ sq ]->getTypeGlobID() ]->Message ) ;

	// show message
	char  text[256] , tmp[256] ;
	int   messId = rand()%10 / 4 ; // 0 - 1 - 2
	sprintf( tmp, "#CQM_AddSquad%d" , messId ); 
	sprintf( text, GetTextByID( tmp ) , money , name ); 

	if( ShowBigMapGlobalMessage( 2, 33 , 11 , text , 0 ) == 1 ) // ok
	{
		// delete money
		pPlayer[ PlayerNI ]->AddMoney( -money );
		// quest complete! - добавить отряд
		pH->AddSQbyArrID( sq );
		// show message
		sprintf( text, GetTextByID( "#CQM_AddSquadD" ) , name ); 
		ShowBigMapGlobalMessage( 1, 33 , 11 , text , 0 );
	}


	Quest[id]->wait = Quest[id]->turn_limit ;

	return true;
}
//--------------------------------------------------------------------------------------------------------------------
bool	CBigMapQuest::FallPrices( int PlayerNI , int id )  // КВЕСТ - СНИЖАЮТСЯ ЦЕНЫ РЕСУРСОВ НА РЫНКЕ
{
	if( !QuestGived )
	{
		// rand target Price (Resource)
		int  resID = rand()%6;			if( resID == GOLD )  resID++; 
		QuestOwner = resID*10;//new
		
		// rand target division
		const int  nn = 10;
		int  ResDiv[nn] = { 10, 10, 8, 6, 5, 5, 4, 2, 2, 2 };
		int  divID = ResDiv[ rand()%nn ];
		QuestTarNI = divID;


		//Changer = 0.5 - значит ресурс подешевел в 2 раза	( Changer < 1 )
		//Changer = 2.5 - подорожал в 2.5 раза				( Changer > 1 )
		//void RestoreMarketPrice();
		//bool ChangeMarketPrice(byte ResID, float Changer);

		// give quest - Fall Prices
		ChangeMarketPrice( resID, 1 / (float(divID)) );
		//pROOT->m_pMarket->m_Market->ChangeResPrice( resID, 1 / (float(divID))  );


		// show message										// #CQM_RisePrices  #CQM_FallPrices
		char  text[256], textID[256];
		int  messID = rand()%2;
		QuestOwner += messID;//new
		sprintf( textID, "#CQM_FallPrices%d%d" , resID , messID ); 

		sprintf( text, GetTextByID( textID ) , divID );
		ShowBigMapGlobalMessage( 1, 26 , 11 , text , 0 );

		pROOT->m_pMarket->SetPriceMessage( text );


		Quest[id]->wait = 1 ; // !!! 1 ход !!!

		QuestID = id ;

		return   QuestGived = true ;
	}
	else 
	{
		// проверка условий выполнения квеста
		if( Quest[id]->wait <= 0 ) 
		{
			// reLoad - вернуть рынок в исходное состояние
			RestoreMarketPrice();
			//pROOT->m_pMarket->m_Market->Load();
			pROOT->m_pMarket->SetPriceMessage( NULL , false );

			// quest complete!
			QuestID = QuestOwner = QuestTarNI = -1 ;

			Quest[id]->wait = Quest[id]->turn_limit ;


			return   QuestComplete = true ;
		}
	}


	// поставить текущую цену на рынке
	if(	QuestGived && !Next_Turn ) {
		// reLoad
		RestoreMarketPrice();
		//pROOT->m_pMarket->m_Market->Load();

		// again Fall Prices
		ChangeMarketPrice( QuestOwner/10 /*new*/ , 1 / (float(QuestTarNI)) );
		//pROOT->m_pMarket->m_Market->ChangeResPrice( QuestOwner/10 /*new*/ , 1 / (float(QuestTarNI))  );


		// show message
		char  text[256], textID[256];
		sprintf( textID, "#CQM_FallPrices%d%d" , QuestOwner/10 ,   QuestOwner%10   /*messID*/ ); 

		sprintf( text, GetTextByID( textID ) , QuestTarNI );
		pROOT->m_pMarket->SetPriceMessage( text );

	}

	return true;
}
//--------------------------------------------------------------------------------------------------------------------

bool	CBigMapQuest::RisePrices( int PlayerNI , int id )  // КВЕСТ - ПОВЫШАЮТСЯ ЦЕНЫ РЕСУРСОВ НА РЫНКЕ
{
	if( !QuestGived )
	{
		// rand target Price (Resource)
		int  resID = rand()%6;			if( resID == GOLD )  resID++;
		QuestOwner = resID*10;//new

		// rand target division
		const int  nn = 10;
		int  ResDiv[nn] = { 10, 10, 8, 6, 5, 5, 4, 2, 2, 2 };
		int  divID = ResDiv[ rand()%nn ];
		QuestTarNI = divID;


		//Changer = 0.5 - значит ресурс подешевел в 2 раза	( Changer < 1 )
		//Changer = 2.5 - подорожал в 2.5 раза				( Changer > 1 )
		//void RestoreMarketPrice();
		//bool ChangeMarketPrice(byte ResID, float Changer);

		// give quest - Fall Prices
		ChangeMarketPrice( resID, (float(divID)) );
		//pROOT->m_pMarket->m_Market->ChangeResPrice( resID, (float(divID))  );


		// show message										// #CQM_RisePrices  #CQM_FallPrices
		char  text[256], textID[256];
		int  messID = rand()%2;
		QuestOwner += messID;//new
		sprintf( textID, "#CQM_RisePrices%d%d" , resID , messID ); 

		sprintf( text, GetTextByID( textID ) , int(divID) );
		ShowBigMapGlobalMessage( 1, 26 , 11 , text , 0 );

		pROOT->m_pMarket->SetPriceMessage( text );


		Quest[id]->wait = 1 ; // !!! 1 ход !!!

		QuestID = id ;

		return   QuestGived = true ;
	}
	else 
	{
		// проверка условий выполнения квеста
		if( Quest[id]->wait <= 0 ) 
		{
			// reLoad - вернуть рынок в исходное состояние
			RestoreMarketPrice();
			//pROOT->m_pMarket->m_Market->Load();
			pROOT->m_pMarket->SetPriceMessage( NULL , false );

			// quest complete!
			QuestID = QuestOwner = QuestTarNI = -1 ;

			Quest[id]->wait = Quest[id]->turn_limit ;


			return   QuestComplete = true ;
		}
	}


	// поставить текущую цену на рынке
	if(	QuestGived && !Next_Turn ) {
		// reLoad
		RestoreMarketPrice();
		//pROOT->m_pMarket->m_Market->Load();

		// again Rise Prices
		ChangeMarketPrice( QuestOwner/10 ,  float(QuestTarNI) );
		//pROOT->m_pMarket->m_Market->ChangeResPrice( QuestOwner/10 , float(QuestTarNI)  );


		// show message
		char  text[256], textID[256];
		sprintf( textID, "#CQM_RisePrices%d%d" , QuestOwner/10 ,   QuestOwner%10   /*messID*/ ); 

		sprintf( text, GetTextByID( textID ) , QuestTarNI );
		pROOT->m_pMarket->SetPriceMessage( text );
	}

	return true;
}
//--------------------------------------------------------------------------------------------------------------------
int		CBigMapQuest::GetTrueOwner( int PlayerNI , int id ) 
{
	int X = -1;	  
	int PEACE = pDiplData->GetPeaceId();
	for(int i=0; i < MAX_PLAYERS; i++) {
		int owner = Quest[id]->owner[i] ;
		if( owner>=0&&owner<MAX_PLAYERS && owner != PlayerNI && pPlayer[owner]->m_Alive && rand()%100 > 30 && //~33.3%
			pDipl->CheckContract( PEACE , PlayerNI , owner ) )  X = owner;
	}
	return  X; 
}
//--------------------------------------------------------------------------------------------------------------------
int		CBigMapQuest::GetRandOwner( int PlayerNI , int id )
{
	int X = -1;	  
	for(int i=0; i < MAX_PLAYERS; i++){
		int owner = Quest[id]->owner[i];
		if( owner>=0&&owner<MAX_PLAYERS && owner != PlayerNI && pPlayer[owner]->m_Alive && rand()%100 > 55 ) { //~50%
			X = owner;
			if( rand()%100 > 50 )  return owner;
		}
	}
	return  X; 
}
//--------------------------------------------------------------------------------------------------------------------
bool	CBigMapQuest::AddInfoMessage( char* text , bool NewMess )
{
	Enumerator*   pMessHeads = ENUM.Get( "MessHead_ENUM" );

	return   pDipl->AddMessage( -10, 0, 33 , "_" , pMessHeads->Get( 25 ) , text , NewMess );
}
//--------------------------------------------------------------------------------------------------------------------
void	CBigMapQuest::NextTurn()
{
	for(int i=0; i < GetNumber() ; i++) 
		if( Quest[i]->wait >= 0 )   Quest[i]->wait--;
}
//--------------------------------------------------------------------------------------------------------------------
void	CBigMapQuest::ReCountTurnLimits( int root_turn )
{
	if( root_turn <= 0 )			{ return; }

	for(int i=0; i < GetNumber() ; i++) {
		Quest[i]->wait = 0;
		if( Quest[i]->turn_limit != 0 )  
			Quest[i]->wait =  root_turn - ( root_turn / Quest[i]->turn_limit ) * Quest[i]->turn_limit ; 
	}
}
//--------------------------------------------------------------------------------------------------------------------
int		CBigMapQuest::GetNumber() 
{
	return number = Quest.GetAmountOfElements(); 
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
