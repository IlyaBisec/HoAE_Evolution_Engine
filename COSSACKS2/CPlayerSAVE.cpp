
#include "stdheader.h"
#include "AI_Scripts.h"
#include "Dialogs\MISSIONSCRIPT\MissionScript.h"
#include "Dialogs\MISSIONSCRIPT\EndMissionData.h"
#include "Dialogs\BigMapQuest\CQuest.h"				//Grey		[24.03.2004]
#include "Dialogs\ClasDeclaration.h"				//Grey		[06.08.2004]
#include "Dialogs\CPlayerSave.hpp"

//--------------------------------------------------------------------------------------------------------------------
//----------------------------- REALIZATIONS -------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------

bool	CPlayerSAVE_STRUCT::LoadXML( int id /*=0*/ )
{
	if( id >= 0 ) 
	{
		char name[256];
		sprintf(name, m_chFilePath , id );

		this->reset_class(this);//

		return  SafeReadFromFile( name );			// loading from XML

		//xmlQuote xml("");
		//xml.ReadFromFile( name );	
		//ErrorPager Error;
		//return  Load(xml,this,&Error);			// loading from XML
	}
	return  false;
}
//--------------------------------------------------------------------------------------------------------------------

bool	CPlayerSAVE_STRUCT::SaveXML( int id /*=0*/ )
{
	if( id >= 0 ) 
	{
		char name[256];
		sprintf(name, m_chFilePath , id );

		xmlQuote xml("");
		Save(xml,this);
		xml.WriteToFile( name );					// saving to XML
		return  true;
	}
	return  false;
}

//--------------------------------------------------------------------------------------------------------------------

bool	CPlayerSAVE::LoadXML()			
{
	this->m_pPlayer.Clear();
	//this->reset_class(this);

	//xmlQuote xml("");	
	//xml.ReadFromFile(m_chFilePath);	
	//ErrorPager Error;
	//bool result = Load(xml,this,&Error);		// loading from XML
	bool result = SafeReadFromFile( m_chFileName );

	if( result ) {
		for(int i=0; i < pr ; i++) 
		{
			CPlayerSAVE_STRUCT*  pPS = new CPlayerSAVE_STRUCT();
			pPS->Init();
			result = result && pPS->LoadXML( i );
			m_pPlayer.Add(pPS);
		}
	}
	return  result;
}
//bool	CPlayerSAVE::LoadXML()			/* LOAD "bmdsav.xml" (BigMap Data Save) */
//{
//	xmlQuote xml("CPlayerSAVE");	
//	xml.ReadFromFile(m_chFilePath);	
//	ErrorPager Error;
//	return   Load(xml,this,&Error);	// loading from XML
//}

//--------------------------------------------------------------------------------------------------------------------
bool	CPlayerSAVE::SaveXML()
{
	pr = m_pPlayer.GetAmountOfElements();
	if( pr < 0 )  pr = 0 ;

	CreateDirectory(m_chSavePath,0);				// DirPath = "Save\\Battle";

	xmlQuote xml("");
	Save(xml,this);
	xml.WriteToFile( m_chFileName );				// saving to XML	

	for(int i=0; i < pr ; i++) {
		m_pPlayer[i]->SaveXML( i );
	}
	return true;
}
//bool	CPlayerSAVE::SaveXML()
//{
//	xmlQuote xml("CPlayerSAVE");
//	Save(xml,this);
//	xml.WriteToFile(m_chFilePath);				// saving to XML	
//	return true;
//}

//--------------------------------------------------------------------------------------------------------------------
bool	CPlayerSAVE::SaveCurProfile( int current /*=0*/ )
{
	if( current < m_pPlayer.GetAmountOfElements() ) {
		CreateDirectory(m_chSavePath,0);				// DirPath = "Save\\Battle";
		return  m_pPlayer[ current ]->SaveXML( current );
	}
	return  false;
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::getSECTORA( CSectData* pSD ) //save
{
	int secN = pSD->m_pssdDT.GetAmountOfElements();
	m_pPlayer[0]->SECTORA.Clear();
	CSectSAVE* pSAVE_SD = NULL;
	for (int i=0; i < secN ; i++) {
		pSAVE_SD = new CSectSAVE();
		pSAVE_SD->NI	= pSD->m_psddDT[i]->GetOwner();
		pSAVE_SD->POP	= pSD->m_pssdDT[i]->m_inPopulation;
		pSAVE_SD->DEF	= pSD->m_pssdDT[i]->m_inDefence;
		pSAVE_SD->REC	= pSD->m_pssdDT[i]->m_iRecruits;
		pSAVE_SD->SAB	= pSD->m_pssdDT[i]->m_iSabotageID;

		m_pPlayer[0]->SECTORA.Add(pSAVE_SD);
		pSAVE_SD = 0;
	}
}
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::setSECTORA( CSectData* pSD ) //load
{
	int secN = m_pPlayer[0]->SECTORA.GetAmountOfElements();
	if( secN > 0 ) {
		for(int i=0; i < secN ; i++) {
			pSD->m_psddDT[i]->SetOwner( m_pPlayer[0]->SECTORA[i]->NI );
			pSD->m_pssdDT[i]->m_inPopulation	= m_pPlayer[0]->SECTORA[i]->POP;
			pSD->m_pssdDT[i]->m_inDefence		= m_pPlayer[0]->SECTORA[i]->DEF;
			pSD->m_pssdDT[i]->m_iRecruits		= m_pPlayer[0]->SECTORA[i]->REC;
			pSD->m_pssdDT[i]->m_iSabotageID		= m_pPlayer[0]->SECTORA[i]->SAB;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::GetCountry( CPlayer* pCountry[MAX_PLAYERS] ) //save
{
	SaveStatistics( pCountry );

	CCountrySAVE* pCoun = NULL;
	CHeroSAVE*	  pHero = NULL;
	CSquardSAVE*  pSqrd = NULL;
	m_pPlayer[0]->COUNTRY.Clear();

	// страны - COUNTRY
	for (int i=0; i < MAX_PLAYERS ; i++) 
	{
		CPlayer*  pC = pCountry[i];

		pCoun = new CCountrySAVE();		
		pCoun->HEROES.Clear();

		pCoun->mAlive	= pC->m_Alive;
		pCoun->mRes		= pC->Resource ;									
		// герои - HEROES
		for(int h=0; h < pC->m_inHeroNum ; h++)
		{							
			CHero*	pH = &(pC->m_phrHero[h]) ;	  //Массив героев		

			pHero = new CHeroSAVE();	
			pHero->SQUARDS.Clear();

			pHero->mAlive	= pH->m_Alive ;
			pHero->mSecID   = pH->m_inSectorID ;
			pHero->mPrSecID = pH->m_inPreviousSecID ;
			pHero->mHeroID  = pH->m_inHeroID ;
			pHero->mRankID  = pH->m_inRankID ;
			pHero->mStatID  = pH->m_inStatID ;
			pHero->mFuel    = pH->m_inFuel ;
			pHero->mExpa    = pH->m_inExperience ;
			pHero->mBatlN	= pH->m_iBatlN ; 
			pHero->mVictN	= pH->m_iVictN ; 
			pHero->mFKill	= pH->m_iFormKilled ; 
			pHero->mFLost	= pH->m_iFormLost ; 

			// отряды - SQUARDS
			for(int s=0; s < pH->ARMY.GetAmountOfElements() ; s++)
			{
				CSquard*  pS = pH->ARMY[s] ;		  //Массив отрядов

				pSqrd = new CSquardSAVE();	
				pSqrd->KILLS.Clear();

				pSqrd->TypeGlobID = pS->TypeGlobID ;
				pSqrd->SQ_MAX	  = pS->SQ_MAX ;
				pSqrd->SQ_NUM     = pS->SQ_NUM ;

				// set squads NKills
				for(int k=0; k < pH->ARMY[s]->Kills.GetAmountOfElements() ; k++){
					pSqrd->KILLS.Add( pH->ARMY[s]->Kills[k] );
				}
				pHero->SQUARDS.Add( pSqrd );			
			}
			pCoun->HEROES.Add( pHero );			
		}
		m_pPlayer[0]->COUNTRY.Add( pCoun );
		pCoun = NULL;
		pHero = NULL;
		pSqrd = NULL;
	}
}
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::SetCountry( CPlayer* pCountry[MAX_PLAYERS] ) //load
{
	if( m_pPlayer[0]->COUNTRY.GetAmountOfElements() <= 0 ) { 
		return; 
	}

	CCountrySAVE* pCoun = NULL;	
	CHeroSAVE*	  pHero = NULL;	
	CSquardSAVE*  pSqrd = NULL;

	// страны - COUNTRY
	for (int i=0; i < MAX_PLAYERS ; i++) 
	{
		CPlayer*  pC = pCountry[i];		

		pC->m_Alive  = m_pPlayer[0]->COUNTRY[i]->mAlive;
		pC->Resource = m_pPlayer[0]->COUNTRY[i]->mRes;

		// герои - HEROES
		pC->DeleteHeroes();
		// восстановить сейв героев
		for(int h=0; h < m_pPlayer[0]->COUNTRY[i]->HEROES.GetAmountOfElements() ; h++) 
		{	
			pHero      = m_pPlayer[0]->COUNTRY[i]->HEROES[h];//		// герой h //pC->AddHero( pC->m_psdSectData->GetSecIDbyOwner(i) , pHero->mHeroID );
			pC->AddHero( 0 , pHero->mHeroID );									
			CHero*	pH = &(pC->m_phrHero[h]) ;						// массив героев

			pH->m_Alive			  = pHero->mAlive ;
			pH->m_inSectorID	  = pHero->mSecID ;
			pH->m_inPreviousSecID = pHero->mPrSecID ;
			pH->m_inHeroID		  = pHero->mHeroID ;
			pH->m_inRankID		  = pHero->mRankID ;
			pH->m_inStatID		  = pHero->mStatID ;
			pH->m_inFuel		  = pHero->mFuel ;
			pH->m_inExperience    = pHero->mExpa ;
			pH->m_iBatlN		  = pHero->mBatlN ;
			pH->m_iVictN		  = pHero->mVictN ;
			pH->m_iFormKilled	= pHero->mFKill ; 
			pH->m_iFormLost		= pHero->mFLost ; 

			pH->UpdateRankWithExperience();

			// отряды - SQUARDS
			// восстановить сейв отрядов
			for(int s=0; s < pH->ARMY.GetAmountOfElements() && s < pHero->SQUARDS.GetAmountOfElements() ; s++) 
			{
				CSquard*  pS = pH->ARMY[s] ;		  //Массив отрядов
				pSqrd        = pHero->SQUARDS[s];

				pS->TypeGlobID	= pSqrd->TypeGlobID ;
				pS->SQ_MAX		= pSqrd->SQ_MAX ;
				pS->SQ_NUM		= pSqrd->SQ_NUM ;

				pS->Kills.Clear();//
				// set squads NKills
				for(int k=0; k < pSqrd->KILLS.GetAmountOfElements() ; k++){   
					pS->Kills.Add( pSqrd->KILLS[k] );
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::GetDiplData( CDiplomacyData* pDipl ) //save
{
	m_pPlayer[0]->CONT.Clear();		
	m_pPlayer[0]->REQ.Clear();
	for(int i=0; i < pDipl->LACon.GetAmountOfElements() ; i++) {  
		m_pPlayer[0]->CONT.Add( pDipl->LACon[i] );
	}
	for(int i=0; i < pDipl->LAReq.GetAmountOfElements() ; i++) {   
		m_pPlayer[0]->REQ.Add ( pDipl->LAReq[i] );
	}
	m_pPlayer[0]->m_iConCost = pDipl->m_iConCost;
}
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::SetDiplData( CDiplomacyData* pDipl )
{
	if( m_pPlayer[0]->CONT.GetAmountOfElements() > 0 ) { 
		pDipl->LACon.Clear(); 
	}
	if( m_pPlayer[0]->REQ.GetAmountOfElements()  > 0 ) { 
		pDipl->LAReq.Clear(); 
	}
	for(int i=0; i < m_pPlayer[0]->CONT.GetAmountOfElements() ; i++) {
		pDipl->LACon.Add( m_pPlayer[0]->CONT[i] );		
	}
	for(int i=0; i < m_pPlayer[0]->REQ.GetAmountOfElements()  ; i++) {
		pDipl->LAReq.Add( m_pPlayer[0]->REQ[i]  );		
	}
	if( m_pPlayer[0]->m_iConCost > 0 )  pDipl->m_iConCost = m_pPlayer[0]->m_iConCost;
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::GetMessages(CDiplInfoMessages* pMess) //save
{
	m_pPlayer[0]->MESS.Clear();
	for(int i=0; i < pMess->GetMesNum() ; i++) {
		CDiplMessage_STRUCT* DMS = new CDiplMessage_STRUCT;  
		DMS->Letter   = pMess->CAMess[i]->Letter;
		DMS->MessHead = pMess->CAMess[i]->MessHead;
		DMS->MessText = pMess->CAMess[i]->MessText;
		DMS->pictId   = pMess->CAMess[i]->pictId;
		DMS->MessType = pMess->CAMess[i]->MessType;
		DMS->reqId	  = pMess->CAMess[i]->reqId;
		DMS->status	  = pMess->CAMess[i]->status ; 
		DMS->sortId   = pMess->CAMess[i]->sortId;
		m_pPlayer[0]->MESS.Add(DMS); 
	}
}
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::SetMessages(CDiplInfoMessages* pMess)
{
	if( m_pPlayer[0]->MESS.GetAmountOfElements() > 0 ) { 
		pMess->CAMess.Clear(); 
	}
	for(int i=0; i < m_pPlayer[0]->MESS.GetAmountOfElements() ; i++) {
		CDiplMessage_STRUCT* DMS = new CDiplMessage_STRUCT;  
		DMS->Letter   = m_pPlayer[0]->MESS[i]->Letter;
		DMS->MessHead = m_pPlayer[0]->MESS[i]->MessHead;
		DMS->MessText = m_pPlayer[0]->MESS[i]->MessText;
		DMS->pictId   = m_pPlayer[0]->MESS[i]->pictId;
		DMS->MessType = m_pPlayer[0]->MESS[i]->MessType;
		DMS->reqId	  = m_pPlayer[0]->MESS[i]->reqId;
		DMS->status	  = m_pPlayer[0]->MESS[i]->status;
		DMS->sortId   = m_pPlayer[0]->MESS[i]->sortId;
		pMess->CAMess.Add(DMS); 
	}
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::GetMissionInitData( CMISSIONINIT* pInitData ) //save
{
	m_pPlayer[0]->m_MIDataSector_ID  = pInitData->Sector_ID ;
	m_pPlayer[0]->m_MIDataAttNation  = pInitData->AttackerNation_ID ;
	m_pPlayer[0]->m_MIDataDefNation  = pInitData->DefenderNation_ID ;
	m_pPlayer[0]->m_MIDataPlayerStat = pInitData->PlayerStat ;
	m_pPlayer[0]->m_MIDataHeroInSect = pInitData->HeroInSect ;
}
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::SetMissionInitData( CMISSIONINIT* pInitData )
{
	if( m_pPlayer[0]->m_MIDataSector_ID == -1 ) { 
		return; 
	}
	pInitData->Sector_ID			= m_pPlayer[0]->m_MIDataSector_ID ;
	pInitData->AttackerNation_ID 	= m_pPlayer[0]->m_MIDataAttNation ;
	pInitData->DefenderNation_ID	= m_pPlayer[0]->m_MIDataDefNation ;
	pInitData->PlayerStat			= m_pPlayer[0]->m_MIDataPlayerStat ;
	pInitData->HeroInSect			= m_pPlayer[0]->m_MIDataHeroInSect ;
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::GetQuestsData( CBigMapQuest* pQuests ) //save
{
	int  id = pQuests->QuestID ;
	m_pPlayer[0]->m_QuestID		= id ;
	m_pPlayer[0]->m_QuestGived	= pQuests->QuestGived ;
	m_pPlayer[0]->m_QuestOwner	= pQuests->QuestOwner ;
	m_pPlayer[0]->m_QuestSecId	= pQuests->QuestSecId ;
	m_pPlayer[0]->m_QuestTarNI	= pQuests->QuestTarNI ;
	if( id >= 0 ) m_pPlayer[0]->m_QuestWait = pQuests->Quest[ id ]->wait ;
}
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::SetQuestsData( CBigMapQuest* pQuests ) //load
{
	//if( !m_pPlayer[0]->m_QuestGived )   { return; }
	int  id = m_pPlayer[0]->m_QuestID ;
	pQuests->QuestID	 = id ;
	pQuests->QuestGived	 = m_pPlayer[0]->m_QuestGived ;
	pQuests->QuestOwner	 = m_pPlayer[0]->m_QuestOwner ;
	pQuests->QuestSecId	 = m_pPlayer[0]->m_QuestSecId ;
	pQuests->QuestTarNI	 = m_pPlayer[0]->m_QuestTarNI ;
	if( id >= 0 ) pQuests->Quest[ id ]->wait = m_pPlayer[0]->m_QuestWait ;
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
bool	CPlayerSAVE::SaveStatistics( CPlayer* pCountry[MAX_PLAYERS], bool save_all )
{
	CPlayerSAVE_STRUCT*	pPS = m_pPlayer[0];
	if( !pPS )  return false;

	if( !save_all && pPS->LastStUpdate != -1 && pPS->LastStUpdate == pPS->m_inCurTurn )  return false;
	
	pPS->LastStUpdate = pPS->m_inCurTurn;

	// страны - COUNTRY
	for(int nat=0; nat < MAX_PLAYERS ; nat++) 
	{
		if( pPS->STATS.GetAmountOfElements() <= nat ) {
			CStatsSAVE* pST = new CStatsSAVE; 
			pST->indx = nat;
			pPS->STATS.Add(pST); 
		}

		CPlayer*  pC = pCountry[nat];
		if( !pC )  return false;

		pPS->STATS[nat]->SecNum.Add( pC->m_psdSectData->GetPlayerSect( nat ) );
 		
		float res_div[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		res_div[GOLD] = 1;			res_div[COAL] = 2;			res_div[FOOD] = 9;
		res_div[IRON] = 4;			res_div[WOOD] = 7;			res_div[STONE]= 7;

		int supp = 0, power = 0, secN = pC->m_psdSectData->GetSectorsNum();
		if( secN > 0 ) {
			for(int sec=0; sec < secN ; sec++) {
				if( pC->m_psdSectData->GetSecOwner(sec) == nat ) {
					power += (pC->m_psdSectData->GetSecDefence(sec) + 1);

					int res = pC->m_psdSectData->GetResourceID(sec);
					if( res != 0 ) {
						if(res==1) { res = FOOD; } else
						if(res==2) { res = GOLD; } else
						if(res==3) { res = IRON; } else
						if(res==4) { res = COAL; }
						supp += SECT_ADDING[ res ] / res_div[ res ] ;
					}
					supp += SECT_INCOME[ FOOD ] / res_div[ FOOD ] ;
					supp += SECT_INCOME[ WOOD ] / res_div[ WOOD ] ;
					supp += SECT_INCOME[ STONE] / res_div[ STONE] ;
					supp += SECT_INCOME[ GOLD ] / res_div[ GOLD ] ;
					supp += SECT_INCOME[ IRON ] / res_div[ IRON ] ;
					supp += SECT_INCOME[ COAL ] / res_div[ COAL ] ;
				}
			}
		}
		pPS->STATS[nat]->NatPow.Add( power );
		pPS->STATS[nat]->ResSup.Add( supp );

		int resAll = 0;
		for(int res=0; res <= COAL ; res++) {	
			resAll += ( pC->GetResource( res ) / res_div[ res ] ); 
		}
		pPS->STATS[nat]->ResAmo.Add( resAll );

		pPS->STATS[nat]->RecrtN.Add( pC->GetResource(RECRT) );
		
		int exp = 0, apower	= 0, Fkill = 0, Flost = 0;
		for(int h=0; h < pC->m_inHeroNum ; h++) {							
			CHero*	pH = &(pC->m_phrHero[h]) ;	  //Массив героев
			if( pH->m_Alive ) {
				exp		+= pH->GetExperience();
				apower	+= pH->GetARMY_Power(true);
				Flost	+= pH->GetFormLost();
				Fkill	+= pH->GetFormKilled();
			}
		}
		pPS->STATS[nat]->ArmPow.Add( apower );
		pPS->STATS[nat]->GenExp.Add( exp );

		for(int k=0; k < pPS->STATS[nat]->FKill.GetAmountOfElements()&&k < pPS->STATS[nat]->FLost.GetAmountOfElements(); k++ ) {
			Fkill -= pPS->STATS[nat]->FKill[ k ];
			Flost -= pPS->STATS[nat]->FLost[ k ];
		}
		pPS->STATS[nat]->FKill.Add( Fkill<0? 0 : Fkill );		
		pPS->STATS[nat]->FLost.Add( Flost<0? 0 : Flost );		

		//pNat->ArmPow	= pC->m_Alive;
		//pNat->mRes		= pC->Resource ;
		//if( m_pPlayer[0]->STATS->GEt)
	}
	return true;
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::GetMenuState( int  dXM, int dYM, int secId, int heroId, int menuId , 
								   int actAI , int curHeroMove , int curTurn ) //save
{
	m_pPlayer[0]->m_iCurMX0      = dXM ;							// current map x
	m_pPlayer[0]->m_iCurMY0      = dYM ;							// current map y
	m_pPlayer[0]->m_iCurSecId    = secId ;							// current sector
	//m_pPlayer[0]->m_iCurHeroId   = heroId ;						// current hero
	m_pPlayer[0]->m_iCurMenuId   = menuId ;							// current menu

	m_pPlayer[0]->m_inCurActiveAI	= actAI ;				
	m_pPlayer[0]->m_inCurHeroMove	= curHeroMove ;			
	m_pPlayer[0]->m_inCurTurn		= curTurn ; 
}
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::SetMenuState( int* dXM,int* dYM,int* secId,int* heroId,int* menuId,
								  int* actAI, int* curHeroMove , int& curTurn )
{
	*dXM			= m_pPlayer[0]->m_iCurMX0 ;
	*dYM			= m_pPlayer[0]->m_iCurMY0 ;
	*secId			= m_pPlayer[0]->m_iCurSecId ;
	*heroId			= m_pPlayer[0]->m_iCurHeroId ;
	*menuId			= m_pPlayer[0]->m_iCurMenuId ;

	*actAI			= m_pPlayer[0]->m_inCurActiveAI ;
	*curHeroMove	= m_pPlayer[0]->m_inCurHeroMove ;
	curTurn			= m_pPlayer[0]->m_inCurTurn ;
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::GetHelpState( CBigMapHelp* pHelp ) //save
{
	m_pPlayer[0]->help.Clear();	
	for(int i=0; i < pHelp->m_VisNum ; i++) {
		m_pPlayer[0]->help.Add( pHelp->m_Visited[i] );
	}
}
//--------------------------------------------------------------------------------------------------------------------
void	CPlayerSAVE::SetHelpState( CBigMapHelp* pHelp ) //load
{
	for(int i=0; i < m_pPlayer[0]->help.GetAmountOfElements() && i < pHelp->m_VisNum ; i++) {  
		pHelp->m_Visited[i] = m_pPlayer[0]->help[i];
	}
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
int		CPlayerSAVE::GetNextId() 
{
	int maxId = -1;
	for(int i=0; i < m_pPlayer.GetAmountOfElements() ; i++) {
		if( m_pPlayer[i]->m_iId > maxId )  maxId = m_pPlayer[i]->m_iId;
	}
	return  maxId + 1 ;
}
//--------------------------------------------------------------------------------------------------------------------
CPlayerSAVE_STRUCT*	CPlayerSAVE::AddProfile(CPlayerSAVE_STRUCT* Player)
{
	if( Player ) {
		Player->m_iId = GetNextId() ; //grey
		if( m_pPlayer.Insert(0,Player) ) {
			return m_pPlayer[0];
		}
	};
	return NULL;

};
//	CPlayerSAVE_STRUCT*	CPlayerSAVE::AddProfile(CPlayerSAVE_STRUCT* Player)
//	{
//	if(Player&&m_pPlayer.Insert(0,Player)){
//		return m_pPlayer[0];
//	};
//	return NULL;
//};
//--------------------------------------------------------------------------------------------------------------------
bool	CPlayerSAVE::DeleteProfile(CPlayerSAVE_STRUCT* &Player)
{
	for(int i=0;i<m_pPlayer.GetAmount();i++){
		if(m_pPlayer[i]==Player){
			delete Player;
			Player=NULL;
			m_pPlayer.Del(i,1);
			return true;
		}
	}
	return false;
};
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//---------------------------End of CPlayerSAVE-----------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//********************************************************************************************************************
//********************************************************************************************************************
