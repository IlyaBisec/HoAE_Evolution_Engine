
#include "../MainEngine.h"


//////////////////////////////////////////////////////////////////////////
//						CKillEnemy00SPROC 
//////////////////////////////////////////////////////////////////////////
//-------------------------------------
void	UNIT_LV::create(GAMEOBJ* pG)
{
	Index = pG->Index;
	Serial= pG->Serial;
}
//-------------------------------------
bool	UNIT_LV::live()
{
	OneUnit OU;
	GAMEOBJ	GB; GB.Type = 'UNIT';
	GB.Index = Index;	GB.Serial = Serial;
	return	GetUnitInfo(&GB,0,&OU)&&OU.Life>0;
}
//-------------------------------------
SQUARD_LV::SQUARD_LV(int NI,int ID)
{
	Init();
	Create(NI,ID);
}
//-------------------------------------
void	SQUARD_LV::Create(int NI,int ID)
{
	Index	= ID;
	int NM = GetNUnitsInBrigade(NI,ID);
	Brigade_LV	BRIG;
	BRIG.Memb	= new word[NM]; 
	BRIG.MembSN = new word[NM];
	GetBrigadeLV(NI,ID,&BRIG);
	Serial = BRIG.SN;
	
	UNIT_LV* pU = NULL;
	for (int i=0; i<NM; i++){
		pU = new UNIT_LV(BRIG.Memb[i],BRIG.MembSN[i]);
		Units.Add(pU);
		pU = NULL;
	}
	if (BRIG.MembSN){
		delete[](BRIG.MembSN);
		BRIG.MembSN = NULL;
	}
	if (BRIG.Memb) {
		delete[](BRIG.Memb);
		BRIG.Memb = NULL;
	}
}
//-------------------------------------
//////////////////////////////////////////////////////////////////////////
//						CKillEnemy00SPROC
//////////////////////////////////////////////////////////////////////////
//-------------------------------------
CKillEnemy00SPROC::CKillEnemy00SPROC(CKillEnemy00* pKE00,int dif) 
{
	Init(); //grey [18/02/2004]

	NI	= 0;								//NI = pKE00->Nation;
	QuestGived		= false;
	QuestComplite	= false;
	DifficultyLevel = 0 ;					//DifficultyLevel = ( (0<=dif&&dif<3) ? (dif) : (2) );
	switch(DifficultyLevel) {
	case 0:
		pDIFF_DATA = 	&(pKE00->Easy);
		break;
	case 1:
		pDIFF_DATA = 	&(pKE00->Normal);
		break;
	default:
		pDIFF_DATA = 	&(pKE00->Hard);
	}
	Expa		= 0;
	Pulse		= false;	
	pulseTime	= 0;
	// trap conditions
	STATE		= _STANDARD_ ;
	SetTrap		= true;
	PutFuziler	= true;
}
//-------------------------------------
CKillEnemy00SPROC::~CKillEnemy00SPROC() 
{
	Init();
}
//-------------------------------------
void	CKillEnemy00SPROC::Init() 
{
	SQUARDS.Clear();
	UNITS.Clear();
}
//-------------------------------------
void	CKillEnemy00SPROC::CreateQuest( int ai_ni , int& tSqLimit ) 
{
	ClassArray<CBASE_ZONE>*	pZONES = &(pDIFF_DATA->ZONES);
	
	int  iZN  = pZONES->GetAmountOfElements() - 1;
	
	int  aSID = -1;
	SQUARD_LV*		pSQ = NULL;
	CZone00*		pZ00= NULL;
	CZoneGiveQuest*	pZGQ= NULL;
	while (iZN>=0) {
		if ((*pZONES)[iZN]->InfID		== _CZone00_) { // боевая зона
			pZ00 = reinterpret_cast<CZone00*>( (*pZONES)[iZN] );
			if (pZ00->FormType==0) { //один юнит
				GAMEOBJ Z;	Z.Type = 0x40000000+250;
				Z.Index = GetZoneXbyName(pZ00->ZoneName.str);
				Z.Serial= GetZoneYbyName(pZ00->ZoneName.str);
				GAMEOBJ UNT; UNT.Type ='UNIT';
				GAMEOBJ TYPE; TYPE.Type = 'UTYP';
				TYPE.Index = pZ00->UnitType;
				TYPE.Serial= 0;
				CreateObject0(	&UNT,
								&fALONE_LV,
								&TYPE,
								NI,
								&Z,
								pZ00->Direction);
				UNIT_LV* pUNIT = new UNIT_LV();
				pUNIT->create(&UNT);
				UNITS.Add(pUNIT);
				pUNIT = NULL;
			}else if (pZ00->FormType==1) { //отряд
				
				if( STATE != _TRAP_SQUADS_ && rand()%100 < 30 ) { // 50/50 - traps 
					SetTrap = false; 
				}
				if( pZ00->Direction >= 1000 ) { 
					STATE = _TRAP_SQUADS_ ; 
				} 
				aSID = -1;
				if( STATE == _STANDARD_ ) {
					char* UnType[10] = { "UnitFuz(EN)", "UnitFuz(FR)", "UnitFuz(AU)", 
										 "UnitFuz(PR)", "UnitFuz(RU)", "UnitFuz(EG)", "TempMent(FR)" }; //"TempMent(FR)"	//"UnitFuz(EN)"		
					aSID = PutNewFormationBySizeIndex2(	NI, 
														UnType[ai_ni] ,
														0, 
														GetZoneXbyName(pZ00->ZoneName.str)<<4,
														GetZoneYbyName(pZ00->ZoneName.str)<<4, 
														pZ00->Direction );
				}				
				else { //---------- ЗАСАДЫ ----------
					if( STATE == _TRAP_SQUADS_ && SetTrap && TRAPS_SQ_LIMIT > 0 && rand()%100 < 50 ) {//50/50 + Squad Limit for Trap
						//char* UnType2[18] = {	"UnitFuz(EN)",	 "UnitKEgr(EN)",	"UnitEgrNG(EN)", 
						//						"UnitFuzNG(FR)", "UnitKEgr(FR)",	"UnitEgrNG(FR)",
						//						"UnitFuzNG(AU)", "UnitKEgr(AU)",	"UnitEgrNG(AU)", 
						//						"UnitFuz(PR)",	 "UnitKEgr(PR)",	"UnitEgrNG(PR)",  
						//						"UnitPar(RU)",	 "UnitKEgr(RU)",	"UnitEgr(RU)", 
						//						"UnitVol(EG)",	 "UnitKDrg(EG)",	"UnitGre(EG)"	};
						int  type = 0 ; 
						int  prob = rand()%100;
						if( prob < 20 )			{ type = 1 ; } // 20% - Cavalry
						else if( prob < 30 )	{ type = 2 ; } // 10% - Egr

						if( type == 0 && !PutFuziler ) {
							type = 1;
							if( rand()%100 > 50 ) type = 2;
						}
						if( type == 0 )  PutFuziler = false;

						aSID = PutNewFormationBySizeIndex(	NI, 
															UnType2[ ai_ni*3 + type ],
															0, 
															GetZoneXbyName(pZ00->ZoneName.str)<<4,
															GetZoneYbyName(pZ00->ZoneName.str)<<4, 
															64 );
						if( aSID != -1 )  TRAPS_SQ_LIMIT-- ;
					} //---------- ЗАСАДЫ ----------
				}
		
				if( aSID != -1 ) {
					pSQ = new SQUARD_LV( NI, aSID );
					SQUARDS.Add(pSQ);

					SetBrigadeNoMorale( NI, aSID, true );					// No Panic Squadrons
					SetBrigadeAutoFormID( NI, aSID, 0xEEEE );				// не использовать отряд тактич.ai (CITY[NI].Brigs[aSID].AutoFormID = 0xEEEE;)

					if( STATE != _TRAP_SQUADS_ ) {
						// DEFEND ZONE!!! OneObject* OB->newMons->NoMorale = true ( Morale.cpp: void PanicUnit(OneObject* OB) )
						SetBrigadeNewMorale( NI, aSID, 0xEEEE );				// новая мораль отряда

						int polkId = CreatePolk();							// создаем новый полк
						Polk* pPolk = GetPolkByIndex( polkId );
						if(pPolk) { 
							pPolk->AddBrigadeToPolk( NI, aSID );	// отряд - в новый полк
							pPolk->MovePolkTo(  GetZoneXbyName(pZ00->ZoneName.str) +200 ,  //!!!!! +200 - not right
												GetZoneYbyName(pZ00->ZoneName.str) +200 , pZ00->Direction );  //!!!!! +200 - not right
						}
					}
				}
			}
			pSQ  = NULL;		
			pZ00 = NULL;
		} else if ((*pZONES)[iZN]->InfID	== _CZoneGiveQuest_) { // зона выдачи квеста
			pZGQ = reinterpret_cast<CZoneGiveQuest*>( (*pZONES)[iZN] );
			
			if ( pZGQ->Condition == 0 && pZGQ->UnitType != 0 ) { //"UnitInZone"
				GAMEOBJ Z; 
				Z.Type		= 0x40000000 + GetZoneRbyName(pZGQ->ZoneName.str);
				Z.Index		= GetZoneXbyName(pZGQ->ZoneName.str);
				Z.Serial	= GetZoneYbyName(pZGQ->ZoneName.str);
				GAMEOBJ UNT; UNT.Type ='UNIT';
				GAMEOBJ TYPE; TYPE.Type = 'UTYP';
				TYPE.Index = pZGQ->UnitType;
				TYPE.Serial= 0;
				CreateObject0(	&UNT,
								&fALONE_LV,
								&TYPE,
								NI /*7*/,
								&Z,
								pZGQ->Direction);

				OneUnit U;
				if( GetUnitInfo(&UNT,0,&U) ) {
					U.NMask = 191 ;			/*255*/
					SetUnitInfoNew( &U ) ;	//SetUnitInfoNew(OneUnit* Uni) ;
				}
			} 
			pZGQ = NULL;
		}
		iZN--;
	}
}
//-------------------------------------
void	CKillEnemy00SPROC::PROCESS(int NATION, int indx, char* pName ) 
{
	if( !QuestComplite &&  SQUARDS.GetAmountOfElements() + UNITS.GetAmountOfElements() > 0 )
	{
		ClassArray<CBASE_ZONE>*	pZONES = &(pDIFF_DATA->ZONES);
		ClassArray<CBASE_RESS>*	pPRIZS = &(pDIFF_DATA->PRIZS);

		//Выдать сообщение о получении квеста.
		if( !QuestGived ) {
			if( pZONES->GetAmountOfElements() == 0 ) {	//Дать квест по умолчанию.
				QuestGived = true;
			}
			for(int i=0; !QuestGived && i<pZONES->GetAmountOfElements() ; i++) {
				if((*pZONES)[i]->InfID==_CZoneGiveQuest_) {
					CZoneGiveQuest*	pZGQ= reinterpret_cast<CZoneGiveQuest*>((*pZONES)[i]);
					if (pZGQ->Condition == 0) {	//Юнит игрока зашел в зону. "UnitInZone"
						GAMEOBJ Z; 
						Z.Type		= 0x40000000 + GetZoneRbyName(pZGQ->ZoneName.str);
						Z.Index		= GetZoneXbyName(pZGQ->ZoneName.str);
						Z.Serial	= GetZoneYbyName(pZGQ->ZoneName.str);
						if( GetUnitsAmount0(&Z,NATION) > 0 ) 
						{
							QuestGived = true;
							if( STATE == _STANDARD_ ) {
								EnableMission( *missID[0] );
								if( indx < 7 ) EnableMission( *missID[1+2*indx] );	//SetStartPoint(&Z); ShowPageParam(  pZGQ->PageName.str , pName  ); // IN LETTERS 
					
								Pulse = true;
								pulseTime = GetTickCount() ;
								// подсветка зоны квеста на миникарте
								for (int i=0; i < pZONES->GetAmountOfElements(); i++) { // выбрать первую зону квеста
									if((*pZONES)[i]->InfID == _CZone00_) { break; }
								}
								CZone00* pZ00 = reinterpret_cast<CZone00*>((*pZONES)[i]);
								int realX = GetZoneXbyName(pZ00->ZoneName.str);
								int realY = GetZoneYbyName(pZ00->ZoneName.str);
								AddPulseBox(realX, realY); // here && IN LETTERS!!! 

								AddMessageIntoMessCampCII(pZGQ->PageName.str,pName,0xDEF,indx,0,realX,realY,"Quest");//0xDEF

								//new
								_str txt;  txt = indx;
								g_addTask(7, realX, realY, txt.str );

								//PlaySound( "" );
								//PlayInMissionSoundCII( "INTERF_QUEST" );
								PlayInMissionSoundCII( "INTERF_MESSAGE" );//very new
							}
							// ------- ЗАСАДЫ -----  ( send all squads to attack )
							if( STATE == _TRAP_SQUADS_ && SQUARDS.GetAmountOfElements() > 0 ) {
								int polkId  = CreatePolk();			  // создаем новый полк
								Polk* pPolk = GetPolkByIndex(polkId);
								if(pPolk) {
									for(int sq=0; sq < SQUARDS.GetAmountOfElements() ; sq++ ) {
										pPolk->AddBrigadeToPolk( NI, SQUARDS[ sq ]->Index ); // отряд - в новый полк
									}
									pPolk->MovePolkTo(  Z.Index  +200 ,		   //!!!!! +200 - not right
														Z.Serial +200 , 64 );  //!!!!! +200 - not right
								}
							}
							// ------- ЗАСАДЫ -----   
						}
					}
				}
			}
		}

		// поднять мораль отрядов
		if( STATE == _STANDARD_ )
			for(int i=0; i < SQUARDS.GetAmountOfElements() ; i++) {
				//SetBrigadeNoMorale ( NI , SQUARDS[i]->Index , true ); // No Panic Squadrons
				SetBrigadeNewMorale( NI , SQUARDS[i]->Index , 0xEEEE );
			}

		//проверка выполнения условий квеста
		bool  Victory = true;
        for(int i=0; i < SQUARDS.GetAmountOfElements() && Victory ; i++) {	// разгон отряда
			Victory = Victory && !(GetBrigadeEnable(NI,SQUARDS[i]->Index));						//WarType==0  //}
		}
		for(int i=0; i < UNITS.GetAmountOfElements() && Victory ; i++) {	// убийство юнита				
			Victory = Victory && !(UNITS[i]->live());
		}
		if( Victory ) { 
			QuestComplite = true; }

		if( QuestGived && QuestComplite ) {
			// Раздача призов.
			for (int i=0; i<pPRIZS->GetAmountOfElements(); i++) {
				if ((*pPRIZS)[i]->InfID == _CMinerals_) {								//ресурс	_CMinerals_					
					CMinerals* pPM = reinterpret_cast<CMinerals*>((*pPRIZS)[i]);
					AddResource(NATION,pPM->MineralsType,pPM->MineralsAmount);

					AttackerIncomeRes[ pPM->MineralsType ] += pPM->MineralsAmount ; // very new - 4 statistics
				}
				else if ((*pPRIZS)[i]->InfID == _CTroops_) {							//отряд		_CTroops_
					CTroops*   pPT = reinterpret_cast<CTroops*>((*pPRIZS)[i]);
					if( pPT->UnitType == 0 ) {
						char* typeId = GiveMeCoolAttackerSquadType( Real_PLAYER_NI );
						if( typeId ){ 
							int id = PutNewFormationBySizeIndex(	NATION,								
														typeId , //UnType[ Real_PLAYER_NI ] , 
														0, 
														GetZoneXbyName(pPT->ZoneName.str)<<4,
														GetZoneYbyName(pPT->ZoneName.str)<<4, 
														pPT->Direction );
							if( id != -1 ){
								GetAttackerSQ_NUM++;
							}
						}
					}
					else {
						if( pPT->Direction < 2000 ) {   // for Ships(NE)
							PutNewFormationBySizeIndex(	NATION,								
														pPT->UnitType , 
														0, 
														GetZoneXbyName(pPT->ZoneName.str)<<4,
														GetZoneYbyName(pPT->ZoneName.str)<<4, 
														pPT->Direction );
						}
						else {						   // Ships(NE)   pPT->Direction >= 2000
							GAMEOBJ  UNT;	UNT.Type	= 'UNIT';
							GAMEOBJ  TYPE;	TYPE.Type	= 'UTYP';   TYPE.Index = pPT->UnitType;   TYPE.Serial = 0;
							GAMEOBJ  Z;		Z.Type		= 0x40000000+250;	
							Z.Index  = GetZoneXbyName(pPT->ZoneName.str) ;		
							Z.Serial = GetZoneYbyName(pPT->ZoneName.str) ;
							CreateObject0(	&UNT, &fALONE_LV, &TYPE, NATION , &Z, pPT->Direction -2000 ) ;
						}
					}
				}
				else if ((*pPRIZS)[i]->InfID == _COpenZone_) {							//видимая зона _COpenZone_
					COpenZone* pPZ = reinterpret_cast<COpenZone*>((*pPRIZS)[i]);
					GAMEOBJ Z;	Z.Type = 0x40000000 + GetZoneRbyName(pPZ->ZoneName.str);
					Z.Index	 = GetZoneXbyName(pPZ->ZoneName.str);
					Z.Serial = GetZoneYbyName(pPZ->ZoneName.str);
					SetLightSpot( &Z, 100, (i+1)*(NATION+1)*(indx+1) );				
				}				
				else if ((*pPRIZS)[i]->InfID == _CGiveGroup_) {							//группa	_CGiveGroup_
					CGiveGroup* pPG = reinterpret_cast<CGiveGroup*>((*pPRIZS)[i]);
					GAMEOBJ G;
					RegisterUnits( &G, pPG->GroupName.str );
					for(int i=0; i<8; i++) if(i!=NATION) { ClearSelection(i); }
					SelectUnits(&G, 0);
					for(int i=0; i<8; i++) if(i!=NATION) { SelChangeNation(i, NATION); }
					for(int i=0; i<8; i++) if(i!=NATION) { ClearSelection (i); }
				}
			}
			//Сообщение о окончании квеста, если есть.
			for (int i=0; i<pZONES->GetAmountOfElements(); i++) {
				if ((*pZONES)[i]->InfID==_CZoneGiveQuest_) {
					CZoneGiveQuest*	pZGQ= reinterpret_cast<CZoneGiveQuest*>((*pZONES)[i]); 
					if (pZGQ->Condition == 1) {					//Квест окончен.	"QuestComplit"
						if( STATE != _TRAP_SQUADS_ ) {
							//PlaySound( "QUESTOK" );
							PlayInMissionSoundCII( "QUESTOK", 60 );

							//ShowPageParam(  pZGQ->PageName.str , pName  ); // IN LETTERS 
							if( indx < 7 ) DisableMission( *missID[1+2*indx] );
							if( indx < 7 ) EnableMission ( *missID[2+2*indx] );

							// подсветка зоны квеста на миникарте
							for (int i=0; i < pZONES->GetAmountOfElements(); i++) { // выбрать первую зону квеста
								if((*pZONES)[i]->InfID == _CZone00_) { break; }
							}
							CZone00* pZ00 = reinterpret_cast<CZone00*>((*pZONES)[i]);
							int realX = GetZoneXbyName(pZ00->ZoneName.str);
							int realY = GetZoneYbyName(pZ00->ZoneName.str);

							AddMessageIntoMessCampCII(pZGQ->PageName.str,pName,0xDEF,indx,1,realX,realY,"Quest"); //0xDEF

							//new
							_str txt;  txt = indx;
							g_delTask( txt.str );
						}
						else {
							int rss[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // ресурсы игроку

							for(int i=0; i < pPRIZS->GetAmountOfElements() ; i++) {
								if( (*pPRIZS)[i]->InfID == _CMinerals_ ) { //ресурс	_CMinerals_					
									CMinerals* pPM = reinterpret_cast<CMinerals*>( (*pPRIZS)[i] );
									rss[ pPM->MineralsType ] = pPM->MineralsAmount ; 
									//AttackerIncomeRss[ pPM->MineralsType ] += pPM->MineralsAmount ;
								}
							}
							char  s[256], tmp[256], text[256];		sprintf( text, "" );
							for(int i=0; i < 10 ; i++)
								if( rss[i] > 0 ) { 
									if(strlen(text) == 0)	{ sprintf(tmp, "" );   } 
									else					{ sprintf(tmp, ", " ); }
									sprintf( s  , "%s{CR}%d{C} %s" , tmp , rss[i] , GetResourseName( i ) ); 
									sprintf( tmp, "%s%s" , text, s ); 
									sprintf( text, tmp ); 
								}
							sprintf( tmp, GetTextByID( "Traps_mess" ) , text ); 

							AddMessageIntoMessCampCII( "#RessPage", tmp, 0xAAA ,-1,-1,-1,-1, "Ress" );
							//ShowPageParam(  "#RessPage" , tmp  ); // IN LETTERS
						}
					}
				}
			}

			//new
			//повышение экспы армии, если будет.
			CheckAddExperience( NATION );//new
			//new

		} // Раздача призов.

		// сброс подсветки
		int ttime = -1;
		if( Pulse ) 
			ttime = pulseTime + QZ_PulseTime - GetTickCount();
		if( Pulse && ttime < 0 ) { 
			Pulse = false; }
		// подсветка квеста на карте (из F1) 
		if( QuestGived && GetMissionPulseZone()-0x42 == 2*indx ) { 
			Pulse = true; 
			pulseTime = GetTickCount() ;
			SetMissionPulseZone(0);
		}
		// подсветка квеста на карте (из messages) 
		if( QuestGived && GetActiveMessCampCII() == indx ) {
			Pulse = true; 
			pulseTime = GetTickCount() ;
			SetActiveMessCampCII(-1);
		}
		if( Pulse && pulseTime > 0 ) {
			for(int i=0; i<pZONES->GetAmountOfElements(); i++) //выбрать первую зону квеста
				if((*pZONES)[i]->InfID == _CZone00_) { break; }
			CZone00* pZ00 = reinterpret_cast<CZone00*>((*pZONES)[i]);
			if( pZ00 && pZ00->ZoneName.str ) {
				int x = GetZoneXbyName(pZ00->ZoneName.str) ;
				int y = GetZoneYbyName(pZ00->ZoneName.str) ;
				AddPulseBox(x, y);   }
		}
	}
}
//-------------------------------------
int		CKillEnemy00SPROC::GetExperience()
{ 
	//експа за разогнанные отряды
	Expa += ( SQUARDS.GetAmountOfElements() - GetEnabledSQUARDS() ) * GetBigMapQuestDisbSquadExp();
	//експа за вып. квест
	if( QuestComplite )  Expa += GetBigMapQuestCompliteExp();
	return  Expa;  
}
//---------------------------------------------------------------------------------------------------------------
int		CKillEnemy00SPROC::GetEnabledSQUARDS()
{
	int nn = 0;
    for(int i=0; i < SQUARDS.GetAmountOfElements() ; i++) 
		if( GetBrigadeEnable( NI, SQUARDS[i]->Index ) )  nn++;
	return  nn;
}
//---------------------------------------------------------------------------------------------------------------
bool	CKillEnemy00SPROC::CheckQuestActivity()
{
	//if( STATE == _TRAP_SQUADS_ && !SetTrap )  return false;
	if( SQUARDS.GetAmountOfElements() > 0 )  return true;
	return false;
}
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------











