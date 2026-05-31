 
//#include	"ClassDeclaration.h"

//-------------------------------------------------------------------------------------------------------------------
// Смещение в меню для дипломатии 0xFF0000
// 0xFF0000  - country identify picture
// 0xFF0001  - текст с дипл. стат. инфой по сектору
// 0xFF0100  - текст - список стран в состоянии войны
// 0xFF0200  - текст - список стран с дипл.договорами 
// 0xFF1000  - кнопки действий +256
// 0xFF1100  - кнопки над картой +256
// 0xFF1200  - кнопки с инф.сообщениями +256
// 0xFF1300  - инф.сообщения +256
//
// Смещение в меню для бриффингa 0xFFF000
// 0xFFF000  - бриффинг 
//-------------------------------------------------------------------------------------------------------------------
// Смещение в меню для сообщений 0xFFFF00
// 0xFFFF00  - сообщения 
//-------------------------------------------------------------------------------------------------------------------
// id дипл.договоров (type)
//    0 - война
//    1 - мир
//    2 - пакт о ненапад.
//    3 - право прохода
//    4 - союз
//	  5 - ...
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------


bool 	DiplActionPressed(SimpleDialog* pSD);
bool	MapButtonPressed(SimpleDialog* pSD);
bool	InformationYesClick(SimpleDialog* pSD);
bool	InformationNoClick(SimpleDialog* pSD);
bool 	InformationMessagePressed(SimpleDialog* pSD);


////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CDiplomacyData  /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
CDiplomacyData::CDiplomacyData(CSectData* pSectData)
{
	Init();

	m_iPlayerId		= 0 ;
	m_iMinimumCost	= m_iConCost = 1000 ; 

	m_pSectData = pSectData;
	m_pDiplMenu = NULL; 

	// contracts constants
	m_iFields   = 4;
	m_iTypeNum  = 5;
	m_iWarId    = 0;
	m_iPeaceId  = 1;
	m_iPactId   = 2;
	m_iPassId   = 3;
	m_iAllyId   = 4;
	m_iUnionId  = 5 ;
	m_iSecWarId = 10;	m_iSecPeaceId = 11;		m_iSecPassId = 13;

	strcpy(m_chFilePath, "Save\\dipldata.xml");

	OnFirstInit();
}
//----------------------------------------------------------------------------------
CDiplomacyData::CDiplomacyData()
{
	Init();//new
}
//----------------------------------------------------------------------------------
CDiplomacyData::~CDiplomacyData()
{
	Init();//new
}
//----------------------------------------------------------------------------------
void	CDiplomacyData::Init()//new
{
	LACon.Clear();
	LAReq.Clear();
}
//----------------------------------------------------------------------------------
void	CDiplomacyData::SetDiplMenu(CDiplMenu* pDiplMenu) 
{
	m_pDiplMenu = pDiplMenu;
}
//----------------------------------------------------------------------------------
void	CDiplomacyData::OnFirstInit() 
{
	for (int i=0; i < MAX_PLAYERS-1 ; i++)												
		for (int j=i+1; j < MAX_PLAYERS ; j++)  AddContract(m_iPeaceId, i, j, 5);

	//m_iConCost = 1000; 
	// добавление мирных договоров с нейтральными областями		//*****test
	//int SectorsNum = this->m_pSectData->GetSectorsNum();
	//for (int i=0; i < MAX_PLAYERS ; i++)												// MAX_PLAYERS = 6
	//	for (int sec=0; sec < SectorsNum ; sec++) 
	//		if (m_pSectData->GetSecOwner(sec)==-1) { AddContract(m_iSecPeaceId, i, sec, 5); }		//*****test
}
//----------------------------------------------------------------------------------
bool	CDiplomacyData::LoadXML()
{
	//loading from XML			/* LOAD SAVED "dipldata.xml" (Diplomacy Data Save) */
	xmlQuote xml;
	xml.ReadFromFile( m_chFilePath );
	ErrorPager Error;
	return this->Load(xml,this,&Error);			
}
//----------------------------------------------------------------------------------
bool	CDiplomacyData::SaveXML()
{
	//saving to XML				/* SAVE "dipldata.xml" (Diplomacy Data Save) */
	xmlQuote xml2("CDiplomacyData");
	this->Save(xml2,this);
	xml2.WriteToFile( m_chFilePath );			
	return true;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetDiplActionPicture( int type , int country1 ,int country2 )
{
	switch( type ) {
		case 0:										// war
				//if( country1 == 4 )	return  2 ;
				//if( country1 == 5 )	return  3 ;
				//if( country2 == 4 )	return  2 ;
				//if( country2 == 5 )	return  3 ;

				if( country1 == 0 )	return  0  ;
				if( country1 == 1 )	return  1  ;
				if( country1 == 2 )	return  34 ;
				if( country1 == 3 )	return  35 ;
				if( country1 == 4 )	return  2  ;
				if( country1 == 5 )	return  3  ;
				return	0 ;						
		case 1:										// peace					
				if( country1 == 5 )	return  6 ;
				if( country2 == 5 )	return  6 ;

				if( country1 == 0 )	return  4 ;	
				if( country1 == 1 )	return  5 ;
				if( country1 == 2 )	return  5 ;
				if( country1 == 3 )	return  4 ;
				if( country1 == 4 )	return  5 ;
				return	4 ;
		case 2:										// pact
				if( country1 == 5 ) return	12 ;
				if( country2 == 5 ) return	12 ;
				return	11 ;
		case 3:										// pass
				if( country1 == 0 )	return  7 ;		
				if( country1 == 1 )	return  8 ;
				if( country1 == 2 )	return  8 ;
				if( country1 == 3 )	return  8 ;
				if( country1 == 4 )	return  9 ;
				if( country1 == 5 )	return  10 ;
				return	7 ;
		case 4:										// ally
				if( country1 == 0 )	return  13 ;	
				if( country1 == 4 )	return  14 ;
				if( country1 == 5 )	return  15 ;
				if( country2 == 0 )	return  13 ;	
				if( country2 == 4 )	return  14 ;
				if( country2 == 5 )	return  15 ;

				if( country1 == 1 )	return  14 ;
				if( country1 == 2 )	return  13 ;
				if( country1 == 3 )	return  13 ;
				return	14 ;
		case 5:										// union
				if( country1 == 5 ) return	15 ;
				if( country2 == 5 ) return	15 ;
				return	11 ;
		//--------------------------------------
		case 10:									// no money
				if( country1 == 5 ) return	17 ;
				return	16 ; 							
		case 11:									// information
				return	33 ; //2				
		case 15:									// get money
				if( country1 == 5 ) return	22 ;
				return	21 ; //2				
	}
	return 0;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContractCost2( int type, bool change )	//возвр.стоим.контракта
{
	//исп.для игрока, чтобы учитывать кол-во денег у игрока
	if( !change )   { return GetContractCost( type ); }

	int  money	= 1000*(m_iConCost/1000) ; //MONEY

	int  curMoney = 0 ;
	// изменение стоим.дипл.договора в зависимости от ресурсов игрока ( money = F(GOLD,WOOD,...,COAL игрока) )
	for(int res=0; res <= COAL ; res++) {
		if( res == GOLD ) { curMoney += m_pDiplMenu->m_pPlayer[ m_iPlayerId ]->GetResource(res)       ; continue; }
		if( res == FOOD ) { curMoney += m_pDiplMenu->m_pPlayer[ m_iPlayerId ]->GetResource(res) / 100 ; continue; }
							curMoney += m_pDiplMenu->m_pPlayer[ m_iPlayerId ]->GetResource(res) /  10 ; 
	}
	curMoney = curMoney / 2 ;

	if( curMoney < m_iMinimumCost )   curMoney = m_iMinimumCost ;
	if( curMoney < money )			  m_iConCost = curMoney ; 

	money = GetContractCost( type );


	if( type == m_iUnionId ) { // --- Union Countries ---
		int  n	   = 0;
		int  owner = m_pSectData->GetSecOwner( m_pSectData->m_LastLMCSecID );
		for(int id=0; id < m_pSectData->GetSectorsNum() ; id++) 
			if( m_pSectData->GetSecOwner( id ) == owner ) {
				n += m_pSectData->GetSecDefence( id ) + 1;	
				if( m_pSectData->GetResourceID( id ) == 2 ) n += SECT_ADDING[ GOLD ]/SECT_INCOME[ GOLD ] ;
			}
		money = money * n ;
	}

	return money;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContractCost( int type )	// возвр.стоим.контракта
{
	int money = 1000*(m_iConCost/1000); //MONEY

	if( type == m_iWarId )		{	return  0 ;			}			// объявление войны стране

	if( type == m_iPeaceId )	{	return  money * 5 ;	}			// мирный договор со страной

	if( type == m_iPactId )		{	return  money * 3 ;	}			// пакт о ненап. со страной

	if( type == m_iPassId )		{	return  money * 1 ;	}			// право прохода по терр.страны

	if( type == m_iAllyId )		{	return  money * 5 ;	}			// договор о союзе со страной
	
	int AddMoney = SECT_DEF_multiplier2 * SECT_DEFENCE[ GOLD ] - m_iMinimumCost ;
	if( type == m_iUnionId )	{	return  money * 1 + AddMoney ; } // договор о союзе со страной /*4000*/

	return money * 100 ;
}
//int		CDiplomacyData::GetContractCost(int type)	//возвр.стоим.контракта
//{
//	int money = 1000*(m_iConCost/1000); //MONEY
//
//	
//	//( все рес.игрока / 5 )
//	//	< 1000
//	int MIN_CON_COST = 1000 ;
//
//	int curMoney = 0 ;
//	// изменение стоим.дипл.договора в зависимости от ресурсов игрока ( money=F(GOLD,WOOD,...,COAL игрока) )
//	for(int res=0; res<=COAL ; res++) {
//		if( res == GOLD ) { curMoney += m_pDiplMenu->m_pPlayer[ m_iPlayerId ]->GetResource(res)       ; continue; }
//		if( res == FOOD ) { curMoney += m_pDiplMenu->m_pPlayer[ m_iPlayerId ]->GetResource(res) / 100 ; continue; }
//							curMoney += m_pDiplMenu->m_pPlayer[ m_iPlayerId ]->GetResource(res) /  10 ; 
//	}
//	if( curMoney < MIN_CON_COST )	curMoney = MIN_CON_COST ;
//
//	if( curMoney < money )			money = curMoney ;
//
//	////for(int res=0; res<=COAL ; res++) { m_pDiplMenu->m_pPlayer[ m_iPlayerId ]->GetResource(res); }
//	//
//
//	if (type==m_iWarId)		{	return 0;			}				//объявление войны стране
//
//	if (type==m_iPeaceId)	{	return money * 5;	}				//мирный договор со страной
//
//	if (type==m_iPactId)	{	return money * 3;	}				//пакт о ненап. со страной
//
//	if (type==m_iPassId)	{	return money * 1;	}				//право прохода по терр.страны
//
//	if (type==m_iAllyId)	{	return money * 5;	}				//договор о союзе со страной
//	
//	return money;
//}

//----------------------------------------------------------------------------------
int		CDiplomacyData::GetConCost() // возвр. m_iConCost
{
	return  m_iConCost;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::SetContractCost( int turn ) // увелич.стоим.контракта
{
	return  m_iConCost = m_iMinimumCost + turn * 150; // = m_iMinimumCost + turn * 100;
}
//int		CDiplomacyData::SetContractCost()			//увелич.стоим.контракта
//{
//	return m_iConCost += 100 ; //+=500; //1000 
//}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetRequestId( int type, int country, bool both ) 
{
	for(int i=0; i < GetReqNum() ; i+=m_iFields) {
		if( LAReq[i] == type && LAReq[i+1] == country )		return i;
		//if( LAReq[i] == type && both &&
		//   ( LAReq[i+1] == country || LAReq[i+2] == country ) )		return i;
	}
	return -1;	     
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetRequestId( int type, int country1 , int country2 )//new
{	
	for(int i=0; i < GetReqNum() ; i+=m_iFields) {
		if( LAReq[i] == type && LAReq[i+1] == country1 && LAReq[i+2] == country2 )		return i;
		if( LAReq[i] == type && LAReq[i+1] == country2 && LAReq[i+2] == country1 )		return i;
	}
	return -1;	     
}
//int		CDiplomacyData::GetRequestId( int type, int country ) 
//{
//	for(int i=0; i < GetReqNum() ; i+=m_iFields)
//		if( LAReq[i] == type && LAReq[i+1] == country )   { return i; }
//		//if( LAReq[i] == type && 
//		   //( LAReq[i+1] == country || LAReq[i+2] == country ) )   { return i; }
//	return -1;	     
//	//if (type == m_iPassId)		{ return -1; }
//}
//----------------------------------------------------------------------------------
int		CDiplomacyData::AddRequestForContract( int type, int country1, int time ) 
{
	int NextId = GetReqNum();
	for(int i=0; i < GetReqNum() ; i+=m_iFields )
		if(LAReq[i] == -1) { NextId = i; break; }

	if (NextId == GetReqNum()) 
		for(int i=0; i < m_iFields ; i++) { 
			int next=0;
			LAReq.Add(next);		
		} 

	LAReq[NextId]     = type;
	LAReq[NextId + 1] = country1;
	LAReq[NextId + 2] = GetPlayerNId() ; //new /*country2;*/
	LAReq[NextId + 3] = time;

	return NextId;

/*	if ( m_ReqClosed )		{ 
		for (int i=0; i < m_iReqNum ; i+=m_iFields ) m_piRequest[i] = -1;	//if(m_iReqNum > 0){delete[]m_piRequests;} 
		m_ReqClosed = false;	}
	int NextId = m_iReqNum;
	for (int i=0; i < m_iReqNum ; i+=m_iFields )	if(m_piRequest[i] == -1) { NextId = i; break; }
	if (NextId == m_iReqNum) {
		int* arr = m_piRequest;
		m_piRequest = new int[m_iReqNum + m_iFields];
		for (int i=0; i < m_iReqNum ; i++) m_piRequest[i] = arr[i]; 
		delete[]arr;
		m_iReqNum+=m_iFields;	}
	m_piRequest[NextId]     = type;
	m_piRequest[NextId + 1] = country1;
	m_piRequest[NextId + 2] = country2;
	m_piRequest[NextId + 3] = time;
	return NextId;*/
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::ConfirmRequest(int Id) 
{
	if ( Id < 0 )			{ return false; }
	if ( Id >= GetReqNum())	{ return false; }

	AddContract( LAReq[Id], 
				 LAReq[Id + 1], LAReq[Id + 2], LAReq[Id + 3] );

	m_pDiplMenu->AddContractMoney( LAReq[Id], 
								   LAReq[Id + 1], LAReq[Id + 2] );
	//m_pDiplMenu->AddRequestMoney( int country1, int country2, int money ) ; < money = LAReq[ moneyID ] >

	return LAReq[Id + 1];
	//return true;

/*	if ( Id < 0 )			{ return false; }
	if ( Id >= m_iReqNum )	{ return false; }
	AddContract( m_piRequest[Id], m_piRequest[Id + 1], m_piRequest[Id + 2], m_piRequest[Id + 3]);
	m_pDiplMenu->AddContractMoney( m_piRequest[Id], m_piRequest[Id + 1], m_piRequest[Id + 2] );
	return true;	*/
}
//----------------------------------------------------------------------------------
bool	CDiplomacyData::ClearRequests()
{
	for(int i=0; i < GetReqNum() ; i+=m_iFields) LAReq[i] = -1;	
	return true;
	//LAReq.Clear();	//if(GetReqNum()==0) { return true; }	//return false;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetReqNum()
{
	return LAReq.GetAmountOfElements();
}
//----------------------------------------------------------------------------------
//bool	CDiplomacyData::CloseRequests()
//{
//	return	m_ReqClosed = true;
//}
//----------------------------------------------------------------------------------
int		CDiplomacyData::AddContractForPlayer(int type, int time) 
{
	int SecId    = m_pSectData->m_LastLMCSecID;
	int PlayerId = m_pSectData->GetSecOwner(SecId);

	if (PlayerId == -1) { return -1;
		//type+=10;		//PlayerId = SecId; 
	} 

	return AddContract( type, GetPlayerNId() , PlayerId , time );		//AddContract( type, m_iPlayerId , m_pSectData->GetSecOwner(id) , time);
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::AddContract(int type, int country1, int country2, int time)
{
	time = MAX_CONT_VALPERIOD;	//time = 10;

	//договора со странами
	if( type == m_iWarId ) {										//объявление войны стране
		ClearAllContracts(country1, country2);
		AddNewContract(m_iWarId, country1, country2, time);			
	}
	else
	if( type == m_iPeaceId ) {										//мирный договор со страной
		ClearAllContracts(country1, country2);
		AddNewContract(m_iPeaceId, country1, country2, time);		
	}
	else 
	if( type == m_iPactId ) {										//пакт о ненап. со страной
		DeleteContract(m_iPactId, country1, country2);
		AddNewContract(m_iPactId, country1, country2, time);		
	} 
	else
	if( type == m_iPassId ) {										//право прохода по терр.страны
		DeleteContract(m_iPassId, country1, country2);
		AddNewContract(m_iPassId, country1, country2, time);		
	}
	else
	if( type == m_iAllyId ) {										//договор о союзе со страной
		ClearAllContracts(country1, country2);
		AddNewContract(m_iPeaceId, country1, country2, time);		
		AddNewContract(m_iPactId,  country1, country2, time);	
		AddNewContract(m_iPassId,  country1, country2, time);		
		AddNewContract(m_iPassId,  country2, country1, time);
		AddNewContract(m_iAllyId,  country1, country2, time);		
	}
	
	//else
	//договора с секторами
	//if (type==m_iSecPassId) {									//право прохода по терр.сектора
	//	AddNewContract(m_iSecPassId, country1, country2, time);		}
	//else
	//if (type==m_iSecPeaceId) {								//мирный договор с сектором
	//	DeleteContract(m_iSecWarId, country1, country2);
	//	AddNewContract(m_iSecPeaceId, country1, country2, time);		}

	return type;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::AddNewContract(int type, int country1, int country2, int time)
{
	int NextId = GetContNum();
	for(int i=0; i < GetContNum(); i+=m_iFields)   if(LACon[i] == -1) { NextId = i; break; }

	if( NextId == GetContNum() ) 
		for(int i=0; i < m_iFields; i++) { 
			int next=0;
			LACon.Add(next);		
		} 

	LACon[NextId]     = type;
	LACon[NextId + 1] = country1;
	LACon[NextId + 2] = country2;
	LACon[NextId + 3] = time;

	return GetContNum();
/*
	int NextId = m_iConNum;
	for (int i=0; i < m_iConNum ; i+=m_iFields )
		if(m_piContract[i] == -1) { NextId = i; break; }

	if (NextId == m_iConNum) {
		int* arr = m_piContract;

		m_piContract = new int[m_iConNum + m_iFields];
		for (int i=0; i < m_iConNum ; i++) m_piContract[i] = arr[i]; 

		delete[]arr;

		m_iConNum+=m_iFields;
	}

	m_piContract[NextId]     = type;
	m_piContract[NextId + 1] = country1;
	m_piContract[NextId + 2] = country2;
	m_piContract[NextId + 3] = time;

	return m_iConNum;
*/
}
//----------------------------------------------------------------------------------
//int		CDiplomacyData::AddNewContract(int type, int country1, int country2, int time) 
//{
//	int* arr = m_piContract;
//
//	m_piContract = new int[m_iConNum + m_iFields];
//	
//	for (int i=0; i < m_iConNum ; i++) m_piContract[i] = arr[i]; 
//
//	m_piContract[m_iConNum]     = type;
//	m_piContract[m_iConNum + 1] = country1;
//	m_piContract[m_iConNum + 2] = country2;
//	m_piContract[m_iConNum + 3] = time;
//
//	m_iConNum+=m_iFields;
//
//	delete[]arr;
//
//	return m_iConNum;
//}
//----------------------------------------------------------------------------------
int		CDiplomacyData::DeleteContract(int type, int country1, int country2)
{
	//безкомпромисное удаление контракта!
	int i;
	for(i=0; i < GetContNum() ; i+=m_iFields ) 
		if( LACon[i] == type ) {
			if( ( LACon[i+1] == country1 && LACon[i+2] == country2 )	||
				( LACon[i+1] == country2 && LACon[i+2] == country1 && LACon[i] != m_iPassId  ) ) { //new
				LACon[i] = -1; 
				//break;
			}
		}
	return i;
}
//----------------------------------------------------------------------------------
void	CDiplomacyData::ClearAllContracts(int country1, int country2)
{
	for(int i=0; i < m_iTypeNum ; i++) 
		DeleteContract(i, country1, country2);
	DeleteContract(m_iPassId, country2, country1);
	//for(int i=0; i<m_iTypeNum; i++)  DeleteContract(i, country1, country2);
	//DeleteContract(m_iPassId, country1, country2, time);		
	//DeleteContract(m_iPassId, country2, country1, time);
}

//----------------------------------------------------------------------------------
int		CDiplomacyData::GetWarId() 
{
	return m_iWarId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetPeaceId()
{
	return m_iPeaceId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetPactId() 
{
	return m_iPactId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetPassId()
{
	return m_iPassId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetAllyId() 
{
	return  m_iAllyId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetUnionId() 
{
	return  m_iUnionId ;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetSecWarId() 
{
	return m_iSecWarId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetSecPeaceId() 
{
	return m_iSecPeaceId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetSecPassId() 
{
	return m_iSecPassId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetPlayerNId() 
{
	return m_iPlayerId;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContNum() 
{
	return LACon.GetAmountOfElements();
	//return m_iConNum;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContractsSize() 
{
	int n=0;

	for (int i=0; i < GetContNum() ; i+=m_iFields) 
        if (LACon[i]!=-1) n+=m_iFields;

	return n;

/*	int n=0;
	for (int i=0; i < m_iConNum ; i+=m_iFields)   if (m_piContract[i]!=-1) n+=m_iFields;
	return n;	*/
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContractsNumber() 
{
	int n=0;
	for (int i=0; i < GetContNum() ; i+=m_iFields) 
        if (LACon[i]!=-1) n++;

	return n;

/*	int n=0;
	for (int i=0; i < m_iConNum ; i+=m_iFields)   if (m_piContract[i]!=-1) n++;
	return n;	*/
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContractsById(int type, int PlayerId, int* Id) // возвр.массив id стран,с кот.закл.контр.
{
	int n = 0;
	for (int i=0; i < GetContNum() && n < 100  ; i+=m_iFields ) 
	{ 
		if( LACon[i] == type && LACon[i+1] == PlayerId )  
		{ 
			if( LACon[i+2] != -1 ) Id[n++] = LACon[i+2];			//>=0
		}
		else 
		{
			if( type!=m_iPassId && type!=m_iSecPassId && 
				LACon[i] == type && LACon[i+2] == PlayerId ) 
			{ 
				if( LACon[i+1] != -1 ) Id[n++] = LACon[i+1];	//>=0
			}
		}
	}

	return n;
	
/*	int n=0;
	for (int i=0; i < m_iConNum && n < 100  ; i+=m_iFields )					
	{ 
		if (m_piContract[i] == type && m_piContract[i+1] == PlayerId) 
		{ 
			if(m_piContract[i+2]>=0) Id[n++] = m_piContract[i+2]; 
		}
		else 
		{
			if (type!=m_iPassId && type!=m_iSecPassId && 
				m_piContract[i] == type && m_piContract[i+2] == PlayerId) 
			{ 
				if(m_piContract[i+1]>=0) Id[n++] = m_piContract[i+1]; 
			}
		}
	}
	return n;	*/
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContractsNumByType( int type, int playerId )
{
	int Ids[1000]; 
	int num = GetContractsById( type, playerId, Ids ) ;
	return num ;
}

//----------------------------------------------------------------------------------
bool	CDiplomacyData::CheckContract( int Type, int playerId, int secondId )
{
	int  types[1000];
	int  n = GetContTypesById(playerId, secondId, types);

	for(int i=0; i < n ; i++)
		if( Type == types[i] )  return  true;
	
	return  false;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContTypesById(int PlayerId, int SecondId, int* types)
{
	//конктракты для игрока
	int n=0;
	for(int i=0; i < GetContNum() && n < 100 ; i+=m_iFields )					
		if( LACon[i] != -1 ) 
			if( ( LACon[i+1] == PlayerId && LACon[i+2] == SecondId ) || 
				( LACon[i+1] == SecondId && LACon[i+2] == PlayerId && LACon[i] != m_iPassId ) ) {
				types[n++] = LACon[i]; 
			}
	return n;
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetIdByContType(int type, int* players)//new //for texts - not used
{
	int n=0;
	for(int i=0; i < GetContNum() && n < 100 ; i+=m_iFields) 
		if( LACon[i] == type ) {
			players[n++] = LACon[i+1]; 
			players[n++] = LACon[i+2]; 
		}
	return n;

/*	int n=0;

	for (int i=0; i < m_iConNum && n < 100 ; i+=m_iFields ) 
		if (m_piContract[i] == type)
		{
			PlayerId[n++] = m_piContract[i+1]; 
			PlayerId[n++] = m_piContract[i+2]; 
		}

	return n;*/
}
//----------------------------------------------------------------------------------
int		CDiplomacyData::GetContractPeriod( int type, int playerId, int secondId ) //new
{
	// контракты
	for(int i=0; i < GetContNum() ; i+=m_iFields ) {					
		if( LACon[i] == type )
			if( ( LACon[i+1] == playerId && LACon[i+2] == secondId ) || 
				( LACon[i+1] == secondId && LACon[i+2] == playerId && LACon[i] != m_iPassId ) ) 
			  return  LACon[i+3];
	}
	return  -1;
}
//----------------------------------------------------------------------------------
void	CDiplomacyData::RecountContractsPeriod( int* conts, int& n ) 
{
	for(int i=0; i < GetContNum() ; i+=m_iFields)
		if( LACon[i] != -1 ) 
		{
			LACon[i+3] -= 1; 								// пересчет срока договора
			
			int type = LACon[i];
			if( type == m_iWarId || type == m_iPeaceId )	continue;

			if(	LACon[i+3] <= 0 ) 							// срок договора истек после хода
			{
				if( LACon[i+1] == GetPlayerNId() || ( LACon[i+2] == GetPlayerNId() && type != m_iPassId ) ) {
					conts[n]   = type;
					conts[n+1] = LACon[i+1];
					conts[n+2] = LACon[i+2];
					n += 3 ;
				}
				LACon[i] = -1;								// отменить договор
			}
		}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CDiplMenuInfo   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
CDiplMenuInfo::CDiplMenuInfo( DialogsSystem* pMenu, CSectData* pSectData, int N, int TxtOffset )
{
	m_pMenu			= pMenu;
	m_pSectData		= pSectData;
	m_pCounName		= NULL; // назв.
	m_pFlag			= NULL; // флаг
	m_pDipBack		= NULL; 
	m_pPicture		= NULL; 
	m_pDeLimit		= NULL; 
	m_pStateBack 	= NULL; // картинкa состояний дипл.отнош.
	m_pStateHorBar	= m_pStateVerBar = NULL;
	for(int i=0; i < MAX_PLAYERS ; i++) 
		for(int j=0; j < MAX_PLAYERS ; j++) {
			m_pSTATES[i][j] = NULL; // назв.дипл.отнош.
			m_pStateFlags[ i ] = m_pStateFlags[ MAX_PLAYERS+i ] = NULL;
		}
	m_iN = N;
	m_iPicOffset = 0xFF0000;
	m_iTxtOffset = TxtOffset;

	m_Visible = true;
	m_Enabled = true;

	m_iCurrSecId = -1;

	char* statID[50] = {  "#CDT_ST_WAR",   "#CDT_ST_PEACE",  "#CDT_ST_NONA", 
						  "#CDT_ST_PASS",  "#CDT_ST_ALLY",   "#CDT_ST_NONA_PASS", 0 };
	for(int i=0; i <= 5 && statID[i]!=0 ;i++) {
		_str  text;
		text += GetTextByID(statID[i]);
		strcpy( m_chStat[i], text.pchar() );	   
	}

	char* hintID[50] = {  "#CHINT_ST_WAR",   "#CHINT_ST_PEACE",  "#CHINT_ST_NONA", 
						  "#CHINT_ST_PASS",  "#CHINT_ST_ALLY",   "#CHINT_ST_NONA_PASS", 0 };
	for(int i=0; i <= 5 && hintID[i]!=0 ;i++) {
		_str  a_Hint;
		a_Hint  += "{FS}{C FF502515}";		a_Hint  += GetTextByID(hintID[i]);
        strcpy( m_chStatHint[i], a_Hint.pchar() );	   
	}
	
	//strcpy( m_chStat[0], GetTextByID("#CDT_ST_WAR"));   strcpy( m_chStat[1], GetTextByID("#CDT_ST_PEACE"));
	//strcpy( m_chStat[2], GetTextByID("#CDT_ST_NONA"));  strcpy( m_chStat[3], GetTextByID("#CDT_ST_PASS"));
	//strcpy( m_chStat[4], GetTextByID("#CDT_ST_ALLY"));  strcpy( m_chStat[5], GetTextByID("#CDT_ST_NONA_PASS"));
	//strcpy( m_chStatHint[0], GetTextByID("#CHINT_ST_WAR"));   strcpy( m_chStatHint[1], GetTextByID("#CHINT_ST_PEACE"));
	//strcpy( m_chStatHint[2], GetTextByID("#CHINT_ST_NONA"));  strcpy( m_chStatHint[3], GetTextByID("#CHINT_ST_PASS"));
	//strcpy( m_chStatHint[4], GetTextByID("#CHINT_ST_ALLY"));  strcpy( m_chStatHint[5], GetTextByID("#CHINT_ST_NONA_PASS"));

	CreateElements();
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::CreateElements()
{
	int	  X0 = 690 ,   Y0 = 112 ;				// координаты левого верхнего угла	

	// назв.страны
	m_pCounName	= m_pMenu->addTextButton(NULL, X0 +15 /*+8*/ , Y0 +7 ,"-",&fonMenuTitle2[1],&fonMenuTitle2[1],&fonMenuTitle2[1],0); //&BigRedFont,&BigRedFont,&BigRedFont,0);//"_empty_"
	// флaг страны
	LocalGP		a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");
	m_pFlag		= m_pMenu->addGPPicture(NULL, X0 +214 , Y0 +2 ,a_lgpFlags.GPID,1);
	// фото дипломата (country identify picture)
	LocalGP		a_lgpPict("Interf3\\TotalWarGraph\\dCounPic");	 
	m_pPicture	= m_pMenu->addGPPicture(NULL, X0 +58 , Y0 +36 ,a_lgpPict.GPID,0);
	// картинкa-фон для фото дипломата
	LocalGP		a_lgpElems("Interf3\\TotalWarGraph\\bmElements"); 
	m_pDipBack	= m_pMenu->addGPPicture(NULL, X0 +16 , Y0 +31 ,a_lgpElems.GPID, 5 );

	//  таблица дипл.отнош. - картинкa-фон с хинтами
	int  stX = X0 +16 ,  stY = m_pDipBack->y1 +10 ,  stDX = 33 ,  stDY = 24 ;

	int  a_tabH  = GPS.GetGPHeight( a_lgpElems.GPID, 6 );
	int  a_tabW  = GPS.GetGPWidth ( a_lgpElems.GPID, 6 );
	int  a_flagH = 24 ; 									//GPS.GetGPHeight( a_lgpFlags.GPID, 1 );//
	int  a_flagW = 33 ; 									//GPS.GetGPWidth ( a_lgpFlags.GPID, 1 );//

	LocalGP		a_lgpBar("Interf3\\TotalWarGraph\\dActions");
	m_pStateHorBar = m_pMenu->AddDialogsDesk(stX + a_flagW , stY + a_flagH, a_tabW - a_flagW , a_flagH ,"NullBorder");
	m_pStateVerBar = m_pMenu->AddDialogsDesk(stX + a_flagW , stY + a_flagH, a_flagW , a_tabH - a_flagH ,"NullBorder");
	GPPicture* a_pgppHor = m_pStateHorBar->addGPPicture(NULL, -10 , -40 , a_lgpBar.GPID , 7 ); //-7 , -7 , a_lgpElems.GPID , 16 );
	GPPicture* a_pgppVer = m_pStateVerBar->addGPPicture(NULL, -5 , -5 , a_lgpBar.GPID , 7 ); //-7 , -7 , a_lgpElems.GPID , 16 );
	a_pgppVer->Rotated = true;			a_pgppVer->Angle = 64; //90;
	//a_pgppHor->Diffuse = a_pgppVer->Diffuse = DIPL_RELBAR_COLOR;

	for(int i=0; i < MAX_PLAYERS && ROOT_SCENARIO.m_pPlayer[i] ; i++) {
		int  stX2 = stX + 34 ,  stY2 = stY + 25 ;
        m_pStateFlags[ i ]				= m_pMenu->addGPPicture(NULL, stX , stY2 + i*stDY ,a_lgpFlags.GPID,i+1); 
		m_pStateFlags[ MAX_PLAYERS+i ]	= m_pMenu->addGPPicture(NULL, stX2 + i*stDX , stY ,a_lgpFlags.GPID,i+1); 
		_str  hint;		
		hint += "{FS}{C FF640100}";		hint += ROOT_SCENARIO.m_pPlayer[i]->GetPlayerName();
		DYNHINT_RA( m_pStateFlags[ i ] , hint.pchar() );		DYNHINT_RA( m_pStateFlags[ MAX_PLAYERS+i ] , hint.pchar() );
		m_pStateFlags[ i ]->Rotated = m_pStateFlags[ MAX_PLAYERS+i ]->Rotated = true;
		m_pStateFlags[ i ]->ScaleX  = m_pStateFlags[ MAX_PLAYERS+i ]->ScaleX  = 0.9;
		m_pStateFlags[ i ]->Visible = m_pStateFlags[ MAX_PLAYERS+i ]->Visible = false;
	}
	m_pStateBack = m_pMenu->addGPPicture(NULL, stX , stY ,a_lgpElems.GPID, 6 );

	// разделитель
	m_pDeLimit = m_pMenu->addGPPicture(NULL, X0 +16 +4 , m_pStateBack->y1 +12 /*+16*/ ,a_lgpElems.GPID, 30 );
	// подписи для дипл.отнош.
	for(int i=0; i < MAX_PLAYERS ; i++) 
		for(int j=0; j < MAX_PLAYERS ; j++) {
			int xx = m_pStateBack->x +49 + j*33 ,   yy = m_pStateBack->y +30 + i*24 ; //+29 + i*24 ;
			m_pSTATES[i][j] = m_pMenu->addTextButton(NULL, xx , yy ,"-",&BlackFont,&BlackFont,&BlackFont, 1 );
		}
	SetVisible(false);
	SetEnable(false);
}
//----------------------------------------------------------------------------------
bool	CDiplMenuInfo::ChangeSecInfo( int id , CPlayer* pPlayer[MAX_PLAYERS] )
{
	if( id < 0 )							    { return false; }
	if( id >= m_pSectData->GetSectorsNum() )	{ return false; }

	SetCountryPicture(id);

	int ownerId = m_pSectData->GetSecOwner(id);
	if( ownerId >= 0 && ownerId < MAX_PLAYERS ) {
		m_pFlag->Visible = true ;
		SetCountryName( pPlayer[ ownerId ]->GetPlayerName() );
	} 
	else {	
		m_pFlag->Visible = false ;
		SetCountryName( GetTextByID( "#REBEL" ) ) ;
	}

	if( ownerId != m_pSectData->GetSecOwner( m_iCurrSecId ) )  ShiftTableBars( ownerId );

	m_iCurrSecId = id;
	return true;

	//SetCountryName( pPlayer[ m_pSectData->GetSecOwner( id ) ]->GetPlayerName() );
	//SetCountryName(m_pSectData->GetSectorName(id));
	//SetResource(m_pSectData->GetResourceName(id));
	//SetDefence(m_pSectData->GetSecDefence(id));
	//SetPopulation(m_pSectData->GetSecPopulation(id));
	//SetVillages(m_pSectData->GetSecVillageNUM(id));
	//SetState(id);
}
//----------------------------------------------------------------------------------

bool	CDiplMenuInfo::ShiftTableBars( int nation )//new
{
	if( nation >= 0 && nation < MAX_PLAYERS ) {
		int  a_flagH = 24 ;	
		int  a_flagW = 33 ;	//33 ;
		int  pos	 = nation + 1;

		int  posY1 = m_pStateBack->y + pos * a_flagH ;
		setCoordSD( m_pStateHorBar , m_pStateHorBar->x , posY1 );

		int  posX2 = m_pStateBack->x + pos * a_flagW ;
		setCoordSD( m_pStateVerBar , posX2 , m_pStateVerBar->y );
		
		m_pStateHorBar->Visible = m_pStateVerBar->Visible = m_Visible;
		m_pStateHorBar->Enabled = m_pStateVerBar->Enabled = m_Enabled;
		//m_pStateVerBar->Visible = m_pStateVerBar->Enabled = false;//new
		return  true;
	}
	else {
		m_pStateHorBar->Visible = m_pStateVerBar->Visible = false;
		m_pStateHorBar->Enabled = m_pStateVerBar->Enabled = false;
	}
	return  false;
}

//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetCountryPicture(int id)
{
	int PicId = m_pSectData->GetSecOwner(id) + 1; 
	if( m_pPicture->SpriteID != PicId ) { 
		if( PicId != -1 )  m_pFlag->SpriteID = m_pPicture->SpriteID = PicId ;	
	}
	//if (m_pPicture->SpriteID!=PicId) { m_pPicture->SpriteID = PicId; }
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetCountryName(char* name)
{
	BigMapSetMessage( m_pCounName , name ) ;
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetResource(char* name)	
{
	m_chResource = name;						//strcpy(m_chResource.pchar(),name); 
	SetValueById( 4, m_chResource.pchar());
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetState(int value)		
{
	char s[256];
	sprintf(s,"SecID=%d", value);
	m_chState = s;								//sprintf(m_chState.pchar(),"SecID=%d", value);
	SetValueById( 4, m_chState.pchar());
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetDefence(int value)
{
	char s[256];
	sprintf(s,"%d", value);
	m_chDefence = s;							//sprintf(m_chDefence.pchar(),"%d", value);
	SetValueById( 6, m_chDefence.pchar());
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetPopulation(int value)
{
	char s[256];
	sprintf(s,"%d", value);
	m_chPopulation = s;							//sprintf(m_chPopulation.pchar(),"%d", (value+1)*5000 ); //+1)*5000
	SetValueById( 8, m_chPopulation.pchar());
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetVillages(int value)		
{
	char s[256];
	sprintf(s,"%d", value);
	m_chVillages = s;
	SetValueById( 10, m_chVillages.pchar() );
}
//----------------------------------------------------------------------------------
bool	CDiplMenuInfo::SetValueById(int id, char* value)
{
	TextButton*	a_ptb = (TextButton*) m_pMenu->Find( m_iTxtOffset + id );
	if( !a_ptb ) { return false; }
	BigMapSetMessage( a_ptb , value ) ;
	return true;
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetVisible(bool state)
{
	if( m_Visible == state)			return;
	
	m_pCounName->Visible	= state;	
	m_pFlag->Visible		= state;
	m_pDipBack->Visible		= state;
	m_pPicture->Visible		= state;
	m_pDeLimit->Visible		= state;
	m_pStateBack->Visible	= state; // картинкa состояний дипл.отнош.
	m_pStateHorBar->Visible	= m_pStateVerBar->Visible = state; //new
	for(int i=0; i < MAX_PLAYERS ; i++) 
		for(int j=0; j < MAX_PLAYERS ; j++) {
			m_pSTATES[i][j]->Visible	= state; // назв.дипл.отнош.
			m_pStateFlags[ i ]->Visible = m_pStateFlags[ MAX_PLAYERS+i ]->Visible = state;//new
		}
	m_Visible = state;
}
//----------------------------------------------------------------------------------
void	CDiplMenuInfo::SetEnabled(bool state) 
{
	if( m_Enabled == state )			return;

	m_pCounName->Enabled	= state;
	m_pFlag->Enabled		= state;
	m_pDipBack->Enabled		= state;
	m_pPicture->Enabled		= state;
	m_pDeLimit->Enabled		= state;
	m_pStateBack->Enabled	= state; // картинкa состояний дипл.отнош.
	m_pStateHorBar->Enabled	= m_pStateVerBar->Enabled = state; //new
	for(int i=0; i < MAX_PLAYERS ; i++) 
		for(int j=0; j < MAX_PLAYERS ; j++) {
			m_pSTATES[i][j]->Enabled	= state; // назв.дипл.отнош.
			m_pStateFlags[ i ]->Enabled = m_pStateFlags[ MAX_PLAYERS+i ]->Enabled = state;//new
		}
	m_Enabled=state;
}
//----------------------------------------------------------------------------------
int		CDiplMenuInfo::SetCurrSecId(int id)
{
	return	m_iCurrSecId = id;
}
//----------------------------------------------------------------------------------
int		CDiplMenuInfo::GetTxtOffset()
{
	return m_iTxtOffset;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CDiplMenuButtons   //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
CDiplMenuButtons::CDiplMenuButtons(DialogsSystem* pMenu,int N,int ButOffset)
{
	m_pMenu		 = pMenu;
	m_iN		 = N;
	m_iButOffset = ButOffset;
	m_iTxtOffset = ButOffset - 1;

	m_iActive	 = -1; 
	//m_iPressed = -1;

	m_ChangeFont = false;

	m_pMessHeads = ENUM.Get( "MessHead_ENUM" ); 
	m_pMessTexts = ENUM.Get( "MessText_ENUM" ); 

	m_Visible	 = false;
	m_Enabled	 = true;

	//CreateHint(); 
}
//----------------------------------------------------------------------------------
bool	CDiplMenuButtons::SetMainOK(GP_TextButton* pGPMainOK)
{
	if (pGPMainOK==0x00000000) { return false; }
	m_pMainOK = pGPMainOK;
	return true;
}
//----------------------------------------------------------------------------------
bool	CDiplMenuButtons::SetMainNO(GP_TextButton* pGPMainNO)
{
	if (pGPMainNO==0x00000000) { return false; }
	m_pMainNO = pGPMainNO;
	return true;
}
//----------------------------------------------------------------------------------
void	CDiplMenuButtons::CreateHint()
{
	char  a_chDBHint[6][256]; //hint к кнопкам дипл.действий
	strcpy(a_chDBHint[0],GetTextByID("#CHINT_Button1"));	strcpy(a_chDBHint[1],GetTextByID("#CHINT_Button2")); 
	strcpy(a_chDBHint[2],GetTextByID("#CHINT_Button3"));	strcpy(a_chDBHint[3],GetTextByID("#CHINT_Button4")); 
	strcpy(a_chDBHint[4],GetTextByID("#CHINT_Button5"));	strcpy(a_chDBHint[5],GetTextByID("#CHINT_Button6"));

	_str  a_Hint, a_Red, tBlack, tBrown;		
	a_Red   = "{CR}";	  tBlack = "{C FF101010}";   tBrown = "{C FF502515}";

    SimpleDialog*	a_pdb = NULL;
	for(int i=0; i < m_iN ; i++) {
		a_pdb = m_pMenu->Find(m_iButOffset + i);	//кнопки
		if( !a_pdb )	{ continue; }
		a_Hint.DString::Clear();
		//
		a_Hint += "{FS}{C FF502515}";    
		a_Hint += a_chDBHint[i];
		if( i > 0 && ROOT_SCENARIO.m_pDipl && ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ] ) {
			a_Hint += " (";    
			int money = ROOT_SCENARIO.m_pDipl->m_pDiplData->GetContractCost2( i , true );
			if( money > ROOT_SCENARIO.m_pPlayer[ gi_bmSAVE_Nation ]->GetResource( GOLD ) )  a_Hint += a_Red;
			a_Hint += money;
			a_Hint += " ";    
			a_Hint += GetTextByID("ResName4");
			a_Hint += tBrown;
			a_Hint += ")";    
		}
		DYNHINT_RA(a_pdb,a_Hint.pchar());
		
		//a_Hint += "{FS}{C FF502515}";    a_Hint += a_chDBHint[i];    DYNHINT_RA(a_pdb,a_Hint.pchar());
		//
	}
}
//----------------------------------------------------------------------------------
int		CDiplMenuButtons::SetActive(int active)
{
	return  m_iActive = active;
}
//----------------------------------------------------------------------------------
void	CDiplMenuButtons::SetVisible(bool state)
{
	if( m_Visible == state )	{ return; }
	
    SimpleDialog*	a_pdb = NULL;
	for (int i=0; i < m_iN ; i++) {
		a_pdb = m_pMenu->Find(m_iButOffset + i);	//кнопки
		if( !a_pdb )			{ return; }
		a_pdb->Visible = state;
	}
	a_pdb = m_pMenu->Find(m_iTxtOffset);			//подпись "Действия"
	if( a_pdb ) a_pdb->Visible = state;

	m_Visible = state;
}
//----------------------------------------------------------------------------------
void	CDiplMenuButtons::SetEnabled(bool state) 
{
	//if (m_Enabled==state) { return; } 
	
    SimpleDialog*	a_pdb = NULL;
	for (int i=0; i<m_iN; i++) {
		a_pdb = m_pMenu->Find(m_iButOffset + i);	//кнопки
		if( !a_pdb )			{ return; }
		a_pdb->Enabled = state;
	}
	a_pdb = m_pMenu->Find(m_iTxtOffset);			//подпись "Действия"
	if( a_pdb ) a_pdb->Enabled = state;

	m_Enabled = state;
}
//----------------------------------------------------------------------------------
void	CDiplMenuButtons::SetDisabledButtons(CDiplomacyData* pDiplData,int* type,int n) 
{
	SetEnabled(true);

	int WarId		= pDiplData->GetWarId();		//0; 
	int PactId		= pDiplData->GetPactId();		//2;
	int AllyId		= pDiplData->GetAllyId();		//4;
	int SecWarId	= pDiplData->GetSecWarId();		//10;  
	int SecPeaceId	= pDiplData->GetSecPeaceId();	//11;
	int SecPassId	= pDiplData->GetSecPassId();	//13;

	int state = -1;
	const int StateNum  = 11; 
	const int ButtonNum = 6 ; //=m_iN; 
	//int st[6*5]					/*war */     /*peace*/     /*pact*/      /*pass */     /*ally*/      /*SecWar*/    /*SecPeace*/  /*SecPass*/   /*other*/   
	int st[StateNum*ButtonNum] = { 0,1,0,0,0,0,  1,0,1,1,1,1,  0,0,0,1,1,1,  1,0,1,0,1,1,  0,0,0,0,0,0,  0,1,0,0,0,0,  0,0,0,1,0,0,  0,0,0,0,0,0,  0,0,0,0,0,0,  0,0,0,0,0,0,  1,0,1,1,1,0  };
	//int st[StateNum*ButtonNum] = { 0,1,0,0,0,  1,0,1,1,1,  0,0,0,1,1,  1,0,1,0,1,  0,0,0,0,0,  0,1,0,0,0,  0,0,0,1,0,  0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0,  1,0,1,1,1 };

	SimpleDialog*	a_pdb = NULL;
	for(int i=0; i<n; i++) //кнопки
	{
		if (type[i]==WarId || type[i]==PactId || type[i]==AllyId) state = type[i];
		
		if (type[i]==SecWarId || type[i]==SecPeaceId || type[i]==SecPassId) { state = type[i]-10 + ButtonNum; continue; }

		a_pdb = m_pMenu->Find(m_iButOffset + type[i]);
		if (a_pdb==0x00000000) { return; }
		a_pdb->Enabled = false;
	}

	if (state >= 0 )
		for(int i=0; i<ButtonNum; i++) 
			if ( st[i + state*ButtonNum] == 0 ) {	
				a_pdb = m_pMenu->Find(m_iButOffset + i); //кнопки
				if (a_pdb==0x00000000) { return; }
				a_pdb->Enabled = false;
			}
	
	return; 
}
//----------------------------------------------------------------------------------
bool	CDiplMenuButtons::SetDisabledFont()
{
	m_ChangeFont = !m_ChangeFont;	// 1:  false -> true

	GP_TextButton*	a_ptb = NULL;
	for (int i=0; i < m_iN ; i++)	//кнопки
	{
		a_ptb = reinterpret_cast<GP_TextButton*>( m_pMenu->Find(m_iButOffset + i) );
		if( !a_ptb )	{ continue; }					//{ return false; }

		if( !m_ChangeFont && a_ptb->DisabledFont == &GrayFont ) { a_ptb->Enabled = false; }

		if( a_ptb->Enabled && m_ChangeFont )	{ a_ptb->DisabledFont = &OrangeFont;	}
		else									{ a_ptb->DisabledFont = &GrayFont;		}

		if( i == m_iActive && m_ChangeFont )	{	a_ptb->Sprite = a_ptb->Sprite1 = 2 ; 
													a_ptb->DisabledFont = &RedFont;		}
		else									{	a_ptb->Sprite  = 1; /*active*/ 
													a_ptb->Sprite1 = 0; /*passive*/		}
	}

	return true;
}
//----------------------------------------------------------------------------------
//void	CDiplMenuButtons::Redraw()		//перерисовать кнопки
//{
//	for (int i=0; i < m_iN; i++)
//	{
//		GP_TextButton* a_pgptbPB = (GP_TextButton*) m_pMenu->Find(m_iButOffset + i);
//		if (a_pgptbPB==0x00000000) { continue; }
//		if (i!=m_iActive)
//		{
//			a_pgptbPB->Sprite = a_pgptbPB->Sprite1 = 0;
//			a_pgptbPB->PassiveFont = &BlackFont;
//		}
//	}
//}
//----------------------------------------------------------------------------------																				 
bool	CDiplMenuButtons::DiplActionPressed(int ActionActive)
{
	if( ActionActive == -1 )			{ return false; }
	if( ActionActive >= m_iN )			{ return false; }
	if( ActionActive == m_iActive )		{ return false; }
	m_iActive = ActionActive;
	return true;
}
//----------------------------------------------------------------------------------
bool	CDiplMenuButtons::StartDiplAction(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],CDiplMenuButOnMap* pMapButt ,int ActionActive) //,bool ActionPressed) /*DialogsSystem* pdsMenu,CDiplMenuButtons* pDiplBut,CDiplMenuButOnMap* pMapButt,CDiplomacyData* pDiplData*/ 
{	
	//if( ActionActive > 4 ) { return false; }

	int  ownerId = pSectData->GetSecOwner( pSectData->m_LastLMCSecID );
	if( ownerId < 0 || ownerId >= MAX_PLAYERS )   return false;

	m_iActive = ActionActive;

	bool  addCont     = false;
	int   ContMoney   = pDiplData->GetContractCost2( m_iActive , true );
	int   PlayerMoney = pPlayer[ pDiplData->GetPlayerNId() ]->GetPlayerMoney();
	
	char  text[256];
	if( ContMoney <= PlayerMoney ) 
	{ 
		int  picID = pDiplData->GetDiplActionPicture( m_iActive, pDiplData->GetPlayerNId(), ownerId );
		// Message text
		char*  st = m_pMessTexts->Get( m_iActive );
		char*  pPlayerName = pPlayer[ ownerId ]->GetPlayerName();

		sprintf(text, st, pPlayerName, ContMoney/1000 );
		int  ret = ShowBigMapGlobalMessage( 2 , picID , m_iActive, text , 1 ) ;
		
		if( ret == 1 ) {
			addCont = true;

			if( m_iActive != pDiplData->GetUnionId() )
				pDiplData->AddContractForPlayer( m_iActive, 5 );

			PlaySound( "INTERF_LIKE" );	//new		
		}
		else {
			PlaySound( "INTERF_FAIL" );	//new		
		}
	} 
	else {
		PlaySound( "INTERF_FAIL" );	//new		
		int  picID = pDiplData->GetDiplActionPicture( 10, pDiplData->GetPlayerNId(), ownerId );
		// Message text
		char* st = m_pMessTexts->Get( 6 );
		sprintf(text, st, ContMoney/1000 ); 
		ShowBigMapGlobalMessage( 1 , picID , m_iActive, text , 1 ) ;
	}

	m_iActive = -1;

	return addCont; 
}
//----------------------------------------------------------------------------------																				 
bool	CDiplMenuButtons::ShowDiplMessage(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],int ContMoney,int MessId)
{
	DialogsSystem	a_dsMenu(0,0);				//новое меню

	//BlackScreen
	LocalGP		a_lgpScreen("Interf3\\TotalWarGraph\\blackscr");			
	GPPicture*	a_pgppScr = a_dsMenu.addGPPicture(NULL,0,0,a_lgpScreen.GPID,0);
	a_pgppScr->Diffuse = 0x88202020; 

	Enumerator* BUTTON_ENUM = ENUM.Get("BUTTON_ENUM");

	if (MessId==1)								//cообщ. о закл.контракта
	{
		//Message box
		LocalGP		a_lgpMess("Interf3\\TotalWarGraph\\dMessage");				
		a_dsMenu.addGPPicture(NULL,206,150,a_lgpMess.GPID,2);				// x=611 y=381
		//Message box picture
		LocalGP		a_lgpPict("Interf3\\TotalWarGraph\\dMesspic");		
		a_dsMenu.addGPPicture(NULL,272,226,a_lgpPict.GPID, pDiplData->GetDiplActionPicture( m_iActive, pDiplData->GetPlayerNId(), pSectData->GetSecOwner(pSectData->m_LastLMCSecID) ) /*m_iActive*/ );		// x=475 y=142 // x0=66 y0=76
		//Message head
		TextButton* a_ptb = NULL;
		//a_ptb=a_dsMenu.addTextButton(NULL,510,181,m_pMessHeads->Get(m_iActive),&YellowFont,&YellowFont,&YellowFont,1);		
		a_ptb = a_dsMenu.addTextButton(NULL,510,181,"",&YellowFont,&YellowFont,&YellowFont,1);
		BigMapSetMessage( a_ptb , m_pMessHeads->Get(m_iActive) ) ;
		        
		//Message text
		char* st = m_pMessTexts->Get(m_iActive);
		char* pPlayerName = pPlayer[pSectData->GetSecOwner(pSectData->m_LastLMCSecID)]->GetPlayerName();

		char a_chMessage[256];
		if(m_iActive==0) {   sprintf(a_chMessage, st, pPlayerName);   } //"\n", 
		else			 {   sprintf(a_chMessage, st, pPlayerName, ContMoney);   }

		ResFile RF=RRewrite("tmp.txt"); 
		RBlockWrite(RF, a_chMessage ,strlen( a_chMessage ));
		RClose(RF);
		
		TextViewer* a_ptv;
		a_ptv = a_dsMenu.addTextViewer(NULL, 300 , 371 , 420 , 110 ,"tmp.txt",&BlackFont);
		a_ptv->LoadFile("tmp.txt");
		a_ptv->SymSize  = 25; //27 ; 
		a_ptv->PageSize = div(a_ptv->y1 - a_ptv->y, a_ptv->SymSize).quot; //div( 100 , a_ptv->SymSize ).quot;
		a_ptv->y += a_ptv->SymSize*(4 - a_ptv->NLines)/2 - (4 - a_ptv->NLines) ; 

		//Buttons Yes/No
		GP_TextButton* a_ptbYes;	int n=0;
		a_ptbYes=a_dsMenu.addGP_TextButton(NULL,286,485,BUTTON_ENUM->Get(n),a_lgpMess.GPID,6,&WhiteFont,&YellowFont);
		a_ptbYes->OnUserClick = &MMItemChoose; 
		a_ptbYes->UserParam	  = mcmOk;

		GP_TextButton* a_ptbNo;
		a_ptbNo=a_dsMenu.addGP_TextButton(NULL,529,485,BUTTON_ENUM->Get(1),a_lgpMess.GPID,6,&WhiteFont,&YellowFont);
		a_ptbNo->OnUserClick = &MMItemChoose; 
		a_ptbNo->UserParam	 = mcmCancel;   
	}
	else
	if (MessId==0)								//cообщ. о нехватке денег
	{
		//Message box
		LocalGP		a_lgpMess("Interf3\\TotalWarGraph\\dMessage");				
		a_dsMenu.addGPPicture(NULL,206,150,a_lgpMess.GPID,1);
		//Message box picture
		LocalGP		a_lgpPict("Interf3\\TotalWarGraph\\dMesspic");		
		a_dsMenu.addGPPicture(NULL,272,226,a_lgpPict.GPID, pDiplData->GetDiplActionPicture( 10 ,0,0));  /*5,0,0*/ 
		//Message head
		TextButton* a_ptb = NULL;
		//a_ptb=a_dsMenu.addTextButton(NULL,510,181,m_pMessHeads->Get(m_iActive),&YellowFont,&YellowFont,&YellowFont,1);		
		a_ptb=a_dsMenu.addTextButton(NULL,510,181,"",&YellowFont,&YellowFont,&YellowFont,1);

		BigMapSetMessage( a_ptb , m_pMessHeads->Get(m_iActive) ) ;//!!!!!DAMN NEW!!!!!

		//Message text
		//strcpy( a_chMessage, m_pMessTexts->Get( 6 )); /*GetTextByID("#CDM_NoEnoughMoney")*/ 

		char* st = m_pMessTexts->Get( 6 );
		char a_chMessage[256];
		sprintf(a_chMessage, st, ContMoney); 

		ResFile RF=RRewrite("tmp.txt"); 
		RBlockWrite(RF, a_chMessage ,strlen( a_chMessage ));
		RClose(RF);
		
		TextViewer* a_ptv;
		a_ptv = a_dsMenu.addTextViewer(NULL, 300 , 371 , 420 , 110 ,"tmp.txt",&BlackFont);
		a_ptv->LoadFile("tmp.txt");
		a_ptv->SymSize  = 25; 
		a_ptv->PageSize = div(a_ptv->y1 - a_ptv->y, a_ptv->SymSize).quot; 
		a_ptv->y += a_ptv->SymSize*(4 - a_ptv->NLines)/2 - (4 - a_ptv->NLines) ; 

		//Button Ok
		GP_TextButton* a_ptbOk;						
		a_ptbOk=a_dsMenu.addGP_TextButton(NULL,408,485, BUTTON_ENUM->Get(2) ,a_lgpMess.GPID,6,&WhiteFont,&YellowFont); /*GetTextByID("#CDB_OkButton")*/
		a_ptbOk->OnUserClick = &MMItemChoose;    
		a_ptbOk->UserParam	 = mcmOk;
	}


	ItemChoose = - 1;

	do	{
		ProcessMessages();
		StdKeys();
		m_pMenu->ProcessDialogs();
		a_dsMenu.ProcessDialogs();
		a_dsMenu.RefreshView();
	}
	while (ItemChoose!=mcmOk && ItemChoose!=mcmCancel);

	if( ItemChoose==mcmOk && MessId == 1 ) 
	{
		if( m_iActive != pDiplData->GetUnionId() )
			pDiplData->AddContractForPlayer( m_iActive, 5 );

		ItemChoose = - 1;
		return true;
	}	

	ItemChoose = - 1;

	return false;
}
//bool	CDiplMenuButtons::StartDiplAction(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],CDiplMenuButOnMap* pMapButt ,int ActionActive) //,bool ActionPressed) /*DialogsSystem* pdsMenu,CDiplMenuButtons* pDiplBut,CDiplMenuButOnMap* pMapButt,CDiplomacyData* pDiplData*/ 
//{	
//	//if( ActionActive > 4 ) { return false; }
//
//	m_iActive = ActionActive;
//	
//	//GP_TextButton* a_pgptb = NULL;
//	//a_pgptb = (GP_TextButton*) m_pMenu->Find(GetOffset() + m_iActive); //активная кнопка
//	//if (a_pgptb==0x00000000) { return false; }
//	//a_pgptb->Sprite = a_pgptb->Sprite1 = 2 ; // 1 ;		//active	
//	//a_pgptb->ActiveFont = a_pgptb->PassiveFont = &RedFont;
//
//	MainMenuEnabled(pSectData, false, 1 );
//	pMapButt->SetEnabled(false);
//	SetDisabledFont(); 
//	SetEnabled(false);
//	//a_pgptb->Enabled = true;
//
//	bool  addCont    = false;
//	int   ContMoney  = pDiplData->GetContractCost2( m_iActive , true ) ;
//	
//	//if( m_iActive == pDiplData->GetUnionId() ) { // --- Union Countries ---
//	//	int  n = 0;
//	//	for(int id=0; id < pSectData->GetSectorsNum() ; id++)
//	//		if( pSectData->GetSecOwner( pSectData->m_LastLMCSecID ) == pSectData->GetSecOwner( id ) )
//	//			(pSectData->GetResourceID( id ) == 2) ? n+=2 : n++;
//	//	ContMoney = ContMoney * n ;
//	//}
//
//	int   PlayerMoney = pPlayer[pDiplData->GetPlayerNId()]->GetPlayerMoney();
//	
//	if(ContMoney<=PlayerMoney){ addCont = ShowDiplMessage(pSectData,pDiplData,pPlayer, ContMoney/1000, 1 ); }// MONEY
//	else					  {			  ShowDiplMessage(pSectData,pDiplData,pPlayer, ContMoney/1000, 0 ); }// MONEY
//
//	MainMenuEnabled(pSectData, true, 1 );
//	pMapButt->SetEnabled(true);
//	SetEnabled(true);
//	SetDisabledFont(); 
//
//	//a_pgptb->Sprite  = 1; //active
//	//a_pgptb->Sprite1 = 0; //passive
//	//a_pgptb->ActiveFont	 = &RedFont;	a_pgptb->PassiveFont = &OrangeFont;
//
//	m_iActive = -1;
//
//	return addCont; 
//}
////----------------------------------------------------------------------------------																				 
//bool	CDiplMenuButtons::ShowDiplMessage(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],int ContMoney,int MessId)
//{
//	DialogsSystem	a_dsMenu(0,0);				//новое меню
//
//	//BlackScreen
//	LocalGP		a_lgpScreen("Interf3\\TotalWarGraph\\blackscr");			
//	GPPicture*	a_pgppScr = a_dsMenu.addGPPicture(NULL,0,0,a_lgpScreen.GPID,0);
//	a_pgppScr->Diffuse = 0x88202020; 
//
//	Enumerator* BUTTON_ENUM = ENUM.Get("BUTTON_ENUM");
//
//	if (MessId==1)								//cообщ. о закл.контракта
//	{
//		//Message box
//		LocalGP		a_lgpMess("Interf3\\TotalWarGraph\\dMessage");				
//		a_dsMenu.addGPPicture(NULL,206,150,a_lgpMess.GPID,2);				// x=611 y=381
//		//Message box picture
//		LocalGP		a_lgpPict("Interf3\\TotalWarGraph\\dMesspic");		
//		a_dsMenu.addGPPicture(NULL,272,226,a_lgpPict.GPID, pDiplData->GetDiplActionPicture( m_iActive, pDiplData->GetPlayerNId(), pSectData->GetSecOwner(pSectData->m_LastLMCSecID) ) /*m_iActive*/ );		// x=475 y=142 // x0=66 y0=76
//		//Message head
//		TextButton* a_ptb = NULL;
//		//a_ptb=a_dsMenu.addTextButton(NULL,510,181,m_pMessHeads->Get(m_iActive),&YellowFont,&YellowFont,&YellowFont,1);		
//		a_ptb = a_dsMenu.addTextButton(NULL,510,181,"",&YellowFont,&YellowFont,&YellowFont,1);
//		BigMapSetMessage( a_ptb , m_pMessHeads->Get(m_iActive) ) ;//!!!!!DAMN NEW!!!!!
//		        
//		//Message text
//		char* st = m_pMessTexts->Get(m_iActive);
//		char* pPlayerName = pPlayer[pSectData->GetSecOwner(pSectData->m_LastLMCSecID)]->GetPlayerName();
//
//		char a_chMessage[256];
//		if(m_iActive==0) {   sprintf(a_chMessage, st, pPlayerName);   } //"\n", 
//		else			 {   sprintf(a_chMessage, st, pPlayerName, ContMoney);   }
//
//		ResFile RF=RRewrite("tmp.txt"); 
//		RBlockWrite(RF, a_chMessage ,strlen( a_chMessage ));
//		RClose(RF);
//		
//		TextViewer* a_ptv;
//		a_ptv = a_dsMenu.addTextViewer(NULL, 300 , 371 , 420 , 110 ,"tmp.txt",&BlackFont);
//		a_ptv->LoadFile("tmp.txt");
//		a_ptv->SymSize  = 25; //27 ; 
//		a_ptv->PageSize = div(a_ptv->y1 - a_ptv->y, a_ptv->SymSize).quot; //div( 100 , a_ptv->SymSize ).quot;
//		a_ptv->y += a_ptv->SymSize*(4 - a_ptv->NLines)/2 - (4 - a_ptv->NLines) ; 
//
//		//Buttons Yes/No
//		GP_TextButton* a_ptbYes;	int n=0;
//		a_ptbYes=a_dsMenu.addGP_TextButton(NULL,286,485,BUTTON_ENUM->Get(n),a_lgpMess.GPID,6,&WhiteFont,&YellowFont);
//		a_ptbYes->OnUserClick = &MMItemChoose; 
//		a_ptbYes->UserParam	  = mcmOk;
//
//		GP_TextButton* a_ptbNo;
//		a_ptbNo=a_dsMenu.addGP_TextButton(NULL,529,485,BUTTON_ENUM->Get(1),a_lgpMess.GPID,6,&WhiteFont,&YellowFont);
//		a_ptbNo->OnUserClick = &MMItemChoose; 
//		a_ptbNo->UserParam	 = mcmCancel;   
//	}
//	else
//	if (MessId==0)								//cообщ. о нехватке денег
//	{
//		//Message box
//		LocalGP		a_lgpMess("Interf3\\TotalWarGraph\\dMessage");				
//		a_dsMenu.addGPPicture(NULL,206,150,a_lgpMess.GPID,1);
//		//Message box picture
//		LocalGP		a_lgpPict("Interf3\\TotalWarGraph\\dMesspic");		
//		a_dsMenu.addGPPicture(NULL,272,226,a_lgpPict.GPID, pDiplData->GetDiplActionPicture( 10 ,0,0));  /*5,0,0*/ 
//		//Message head
//		TextButton* a_ptb = NULL;
//		//a_ptb=a_dsMenu.addTextButton(NULL,510,181,m_pMessHeads->Get(m_iActive),&YellowFont,&YellowFont,&YellowFont,1);		
//		a_ptb=a_dsMenu.addTextButton(NULL,510,181,"",&YellowFont,&YellowFont,&YellowFont,1);
//
//		BigMapSetMessage( a_ptb , m_pMessHeads->Get(m_iActive) ) ;//!!!!!DAMN NEW!!!!!
//
//		//Message text
//		//strcpy( a_chMessage, m_pMessTexts->Get( 6 )); /*GetTextByID("#CDM_NoEnoughMoney")*/ 
//
//		char* st = m_pMessTexts->Get( 6 );
//		char a_chMessage[256];
//		sprintf(a_chMessage, st, ContMoney); 
//
//		ResFile RF=RRewrite("tmp.txt"); 
//		RBlockWrite(RF, a_chMessage ,strlen( a_chMessage ));
//		RClose(RF);
//		
//		TextViewer* a_ptv;
//		a_ptv = a_dsMenu.addTextViewer(NULL, 300 , 371 , 420 , 110 ,"tmp.txt",&BlackFont);
//		a_ptv->LoadFile("tmp.txt");
//		a_ptv->SymSize  = 25; 
//		a_ptv->PageSize = div(a_ptv->y1 - a_ptv->y, a_ptv->SymSize).quot; 
//		a_ptv->y += a_ptv->SymSize*(4 - a_ptv->NLines)/2 - (4 - a_ptv->NLines) ; 
//
//		//Button Ok
//		GP_TextButton* a_ptbOk;						
//		a_ptbOk=a_dsMenu.addGP_TextButton(NULL,408,485, BUTTON_ENUM->Get(2) ,a_lgpMess.GPID,6,&WhiteFont,&YellowFont); /*GetTextByID("#CDB_OkButton")*/
//		a_ptbOk->OnUserClick = &MMItemChoose;    
//		a_ptbOk->UserParam	 = mcmOk;
//	}
//
//
//	ItemChoose = - 1;
//
//	do 
//	{
//		ProcessMessages();
//		StdKeys();
//		m_pMenu->ProcessDialogs();
//		a_dsMenu.ProcessDialogs();
//		a_dsMenu.RefreshView();
//	}
//	while (ItemChoose!=mcmOk && ItemChoose!=mcmCancel);
//
//	if( ItemChoose==mcmOk && MessId == 1 ) 
//	{
//		if( m_iActive != pDiplData->GetUnionId() )
//			pDiplData->AddContractForPlayer( m_iActive, 5 );
//
//		ItemChoose = - 1;
//		return true;
//	}	
//
//	ItemChoose = - 1;
//
//	return false;
//}
//----------------------------------------------------------------------------------
//bool	CDiplMenuButtons::ProposeDiplAction(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],int DiplAction,int PlayerId)
//{
//	bool addCont    = false;
//	int ContMoney   = pDiplData->GetContractCost(DiplAction);
//	int PlayerMoney = pPlayer[pDiplData->GetPlayerNId()]->GetPlayerMoney();
//	
//	return ShowProposeMessage( pSectData, pDiplData, pPlayer, ContMoney , DiplAction, PlayerId);
//
//	//if (ContMoney<=PlayerMoney) { addCont = ShowProposeMessage( pSectData, pDiplData, pPlayer, ContMoney , DiplAction, PlayerId); }
//	//else						{			ShowProposeMessage( pSectData, pDiplData, pPlayer, ContMoney , DiplAction, 0 ); }
//	//return addCont;
//}
//----------------------------------------------------------------------------------
//bool	CDiplMenuButtons::ShowProposeMessage(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],int ContMoney,int DiplAction,int PlayerId)
//{
//	/////////////////////////////////
//	//ПРЕДЛОЖЕНИЕ КОНТРАКТА ОТ КОМПА
//	/////////////////////////////////
//	//Message box
//	LocalGP		a_lgpMess("Interf3\\dMessage");				
//	this->m_pMenu->addGPPicture(NULL,206,150,a_lgpMess.GPID,2);
//	//Message box picture
//	LocalGP		a_lgpPict("Interf3\\dMesspic");		
//	this->m_pMenu->addGPPicture(NULL,272,226,a_lgpPict.GPID, DiplAction );

	////Message head
	//TextButton* a_ptb = NULL;
	//a_ptb = this->m_pMenu->addTextButton(NULL,510,180, m_MessageHeads[ DiplAction ].pchar() ,&YellowFont,&YellowFont,&YellowFont,1);		
	//
	////Message text
	//char st[256];
	//sprintf(st, "#CDM_ProposeContr%d", DiplAction );
	//strcpy (st, GetTextByID(st)); 
 //   
	//char a_chMessage[256];
	//char* pPlayerName = pPlayer[ PlayerId ]->GetPlayerName();

	//if( DiplAction ==0) {   sprintf(a_chMessage, st, pPlayerName);   }
	//else			    {   sprintf(a_chMessage, st, pPlayerName, ContMoney/1000);   }

	//a_ptb =  this->m_pMenu->addTextButton(NULL, 510, 400,a_chMessage,&BlackFont,&BlackFont,&BlackFont,1); 

	////Buttons Agree/No
	//GP_TextButton* a_ptbYes;								
	//a_ptbYes = this->m_pMenu->addGP_TextButton(NULL,286,485,GetTextByID("#CDB_AgrButton"),a_lgpMess.GPID,5,&WhiteFont,&YellowFont);
	//a_ptbYes->OnUserClick = &MMItemChoose;
	//a_ptbYes->UserParam	  = mcmOk;
	//
	//GP_TextButton* a_ptbNo;
	//a_ptbNo = this->m_pMenu->addGP_TextButton(NULL,529,485,GetTextByID("#CDB_NoButton"),a_lgpMess.GPID,5,&WhiteFont,&YellowFont);
	//a_ptbNo->OnUserClick = &MMItemChoose;
	//a_ptbNo->UserParam	 = mcmCancel;   
//
//	return true;
//}
//------------------------------------------------------------------------------------
bool	CDiplMenuButtons::MainMenuEnabled( CSectData* pSectData, bool state, int pageN )
{
	m_pMainOK->Enabled = state;					// enable кнопок OK/NO
	m_pMainNO->Enabled = state;

	pSectData->SetEnable(state);				// enable секторов 
	//pSectData->SetEnable(state);	//ROOT_SCENARIO.m_Scenario.m_bPlayerEnable == false;	// enable героев

	GP_TextButton*	a_pdb = NULL;
	for (int i=0; i < 5 ; i++) {				// enable кнопок перехода
		a_pdb = reinterpret_cast<GP_TextButton*>(m_pMenu->Find(0x40+i));
		if(!a_pdb)			{ return false; }
		a_pdb->Enabled = state;
		if( i == pageN ) 
			if(state)	{ a_pdb->DisabledFont = &GrayFont ;   } // доступна
			else		{ a_pdb->DisabledFont = &OrangeFont ; } // не доступна
	}
	return true;

	//SimpleDialog*	a_pdb = NULL; 
	//for (int i=0; i < 5 ; i++) {					// enable кнопок перехода
	//	a_pdb = m_pMenu->Find(0x40+i);
	//	if (a_pdb==0x00000000) {return false;}
	//	if (i!=pageN) a_pdb->Enabled = state; //if (i!=1) a_pdb->Enabled = state;
	//}
}
//------------------------------------------------------------------------------------
int		CDiplMenuButtons::GetOffset()
{
	return m_iButOffset;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CDiplMenuButOnMap   ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
CDiplMenuButOnMap::CDiplMenuButOnMap(DialogsSystem* pMenu,int N,int Pressed1,int Pressed2,int ButOffset) 
{
	m_pMenu = pMenu;
	
	m_iN = N;
	m_iButOffset = ButOffset;

	m_iActive	 = -1;		
	m_iPressed1  = Pressed1;	
	m_iPressed2  = Pressed2;	
	//m_IsActive  = false;

	m_Visible = false;
	m_Enabled = true;

	CreateElements();
}
//----------------------------------------------------------------------------------
void	CDiplMenuButOnMap::CreateElements()
{
	int		a_inMW  = 600 ;	// 600x531
	int		a_inMH  = 530 ;

	int		a_inMX0 = 81;	// 75x113
	int		a_inMY0 = 119;
	//int		a_inMX1 = a_inMX0 + a_inMW ; // 675x643
	//int		a_inMY1 = a_inMY0 + a_inMH ;

	//int		a_iWidth  = 52 ; /*50*/		// ширина кнопки	//GPS.GetGPWidth (a_lgpMapButton.GPID,0);
	//int		a_iHeight = 25 ; /*27*/		// высота кнопки	//GPS.GetGPHeight(a_lgpMapButton.GPID,0);

	int		db2x0= 86  /*a_inMX0*/ /*+10*/ , db2y0= 124  /*a_inMY0*/ /*+10*/ ;		// кнопки над картой
	//int		db2x0= a_inMX1 - 5*a_iWidth -14 , db2y0= a_inMY0 +14 ;		// кнопки над картой

	char		a_chMButName[10][256] ,  a_chHINT[10][256] ;
	for(int i=0; i < 10 /*m_iN*/ ; i++) { sprintf(a_chMButName[i],GetTextByID("#CDB_PEACE")); sprintf(a_chHINT[i],GetTextByID("#CDB_PEACE")); }
	strcpy(a_chMButName[0],GetTextByID("#CDB_WAR"));	strcpy(a_chMButName[1],GetTextByID("#CDB_PEACE"));
	strcpy(a_chMButName[2],GetTextByID("#CDB_NONA"));	strcpy(a_chMButName[3],GetTextByID("#CDB_PASS"));
	strcpy(a_chMButName[4],GetTextByID("#CDB_ALLY"));	strcpy(a_chMButName[5],GetTextByID("#CDB_INFO"));
	// - hints
	strcpy(a_chHINT[0],GetTextByID("#CHINT_WAR"));	strcpy(a_chHINT[1],GetTextByID("#CHINT_PEACE"));
	strcpy(a_chHINT[2],GetTextByID("#CHINT_NONA"));	strcpy(a_chHINT[3],GetTextByID("#CHINT_PASS"));
	strcpy(a_chHINT[4],GetTextByID("#CHINT_ALLY")); 
	// - hints

	//GPPicture* a_ptb = NULL;
	LocalGP		a_lgpMapButton("Interf3\\TotalWarGraph\\dOverMap");
	m_pFramePic = m_pMenu->addGPPicture(NULL, db2x0, db2y0 , a_lgpMapButton.GPID, 0 );		
	m_pFramePic->AssignID( m_iButOffset + m_iN );
	m_pFramePic->Visible   = false;						
	GP_TextButton* a_ptb = NULL;
	for(int i=0; i < m_iN ; i++) {
		//a_ptb = m_pMenu->addGP_TextButton( NULL,0,0,a_chMButName[i],a_lgpMapButton.GPID,0, 
		//									&SpecialRedFont , &SpecialBlackFont);//NEED NEW FONTS	
		a_ptb = m_pMenu->addGP_TextButton(NULL, db2x0 +i*41  +6 /*41 +6*/ , db2y0 +6 , " ", a_lgpMapButton.GPID, 1 ,&SpecialBlackFont,&SpecialBlackFont);
		if( i == m_iN-1 )  a_ptb->x += 1;
		a_ptb->AssignID( m_iButOffset + i );		// --- Смещение по карте 0xFF1100 ---		
		a_ptb->UserParam = i;	
		a_ptb->Visible   = false;
		//a_ptb->Sprite    = a_ptb->Sprite1 = 0;	
		//if( i != m_iPressed1 )  a_ptb->Sprite = 1; 
		a_ptb->Sprite    = a_ptb->Sprite1 = i +1;	
		if( i != m_iPressed1 )  a_ptb->Sprite = i +6 ;//+6 ;	
		if( i == m_iPressed1 ) { 
			a_ptb->Diffuse		 = RED_SECT_COLOR & 0x00FFFFFF;
			a_ptb->Diffuse		+= 0xFF000000;
			//a_ptb->ActiveFont	 = &SpecialBlackFont;		
		}
		if( i == m_iPressed2 ) { 
			a_ptb->Diffuse		 = GREEN_SECT_COLOR & 0x00FFFFFF;
			a_ptb->Diffuse		+= 0xFF000000;
		}
		//a_ptb->x  =  db2x0 + i*a_iWidth ;
		//a_ptb->y  =  db2y0 ;		
		//a_ptb->x1 =  db2x0 + (i+1)*a_iWidth;		
		//a_ptb->y1 =  db2y0 + a_iHeight ; 

		////a_ptb->Align = 1 ;
		//a_ptb->FontDy =  1;		//0 ; //2 ; 
		/////a_ptb->FontDx = -8 ;	//2 ; //5;	
		
		a_ptb->OnUserClick = &MapButtonPressed;

		a_ptb->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
		a_ptb->AssignSound( "INTERF_NAGAL", CLICK_SOUND );	

		_str  a_Hint ;	
		a_Hint  += "{FS}{C FF502515}";   a_Hint  += a_chHINT[i];   DYNHINT_RA(a_ptb, a_Hint.pchar() );
	}
}
//----------------------------------------------------------------------------------
void	CDiplMenuButOnMap::SetVisible(bool state){
	if( m_Visible==state )	{ return; }
    SimpleDialog*	a_pdb = NULL;
	for(int i=0; i < m_iN +1 ; i++) {
		a_pdb = m_pMenu->Find( m_iButOffset + i ); //кнопки
		if(!a_pdb)	{ return; }
		a_pdb->Visible = state;
	}
	m_Visible=state;
}
//----------------------------------------------------------------------------------
void	CDiplMenuButOnMap::SetEnabled(bool state)
{
	if( m_Enabled==state )	{ return; }
    SimpleDialog*	a_pdb = NULL;
	for (int i=0; i < m_iN +1 ; i++) {
		a_pdb = m_pMenu->Find( m_iButOffset + i ); //кнопки
		if(!a_pdb)	{ return; }
		a_pdb->Enabled = state;
	}

	m_Enabled=state;
}
//----------------------------------------------------------------------------------
int		CDiplMenuButOnMap::ShowActiveButton(int MapActiveButton)	// смена активной кнопки 
{
	return 0;

	//if (MapActiveButton==-1 || MapActiveButton==m_iPressed1)
	//{
	//	if (m_IsActive){
	//		GP_TextButton*	a_pgptb = (GP_TextButton*) m_pMenu->Find(m_iButOffset + m_iActive);
	//		a_pgptb->Sprite = a_pgptb->Sprite1 = 2*m_iActive; //set passive
	//		//a_pgptb->PassiveFont = &SpecialBlackFont;		
	//		m_iActive  = 0; 
	//		m_IsActive = false;
	//	}
	//	return m_iPressed2;
	//}
	//if (MapActiveButton!=m_iActive) 
	//{ 
	//	GP_TextButton*	a_pgptb;
	//	a_pgptb = (GP_TextButton*) m_pMenu->Find(m_iButOffset + m_iActive);
	//	a_pgptb->Sprite = a_pgptb->Sprite1 = 2*m_iActive; 
	//	//a_pgptb->PassiveFont = &SpecialBlackFont; //set passive
	//	a_pgptb = (GP_TextButton*) m_pMenu->Find(m_iButOffset + MapActiveButton);
	//	a_pgptb->Sprite = a_pgptb->Sprite1 = 2*MapActiveButton + 1;
	//	//a_pgptb->ActiveFont = &SpecialRedFont; //set active		
	//	m_IsActive = true;
	//	m_iActive  = MapActiveButton;
	//}
	//if (Lpressed){
	//	//ShowPressedButton();
	//}
	//return m_iPressed2;


	//m_IsActive = false;
	//SimpleDialog*	a_pdb = NULL;
	//for (int i=0; i < m_iN; i++) //кнопки
	//{
	//	if (i==m_iFirActive) { continue; }
	//	//if (i==m_iSecActive) { continue; }
	//	
	//	a_pdb = m_pMenu->Find(m_iButOffset + i);
	//	if (a_pdb->MouseOver && m_iSecActive!=i){ //test
	//		m_iSecPassive = m_iSecActive;
	//		m_iSecActive = i;
	//		m_IsActive = true;
	//	}
	//}
	//if (m_IsActive){  // отрисовка активной кнопки
	////	a_pgptb->Sprite = a_pgptb->Sprite1 = 2*m_iSecActive + 1 ; //test //0
	////	a_pgptb->ActiveFont = a_pgptb->PassiveFont = a_prlcfPgAct;		
	//}
	//else {return;}
	//if (Lpressed){
	//	ShowPressedButton();
	//}
	//return;
}
//----------------------------------------------------------------------------------
bool	CDiplMenuButOnMap::ShowPressedButton(int* PressedButton, int SecID) 
{
	if(SecID<0)						 { return false; } 
	if(*PressedButton==m_iPressed2)  { return false; }
	if(*PressedButton==m_iPressed1)  { *PressedButton = m_iPressed2; return false; } 

	GP_TextButton*	a_ptb;
	a_ptb = (GP_TextButton*) m_pMenu->Find(m_iButOffset + m_iPressed2);
	a_ptb->Diffuse = -1;														//unpressed

	a_ptb = (GP_TextButton*) m_pMenu->Find(m_iButOffset + *PressedButton);
	a_ptb->Diffuse  = GREEN_SECT_COLOR & 0x00FFFFFF;							//presssed	
	a_ptb->Diffuse += 0xFF000000;	
	//a_ptb->Diffuse = GREEN_SECT_COLOR ; //0xFF60A05A   0xFF000000 + GREEN_SECT_COLOR ;	//presssed					

	m_iPressed2 = *PressedButton;

	return true;
}
//----------------------------------------------------------------------------------
void	CDiplMenuButOnMap::ShowHighlightSectors() 
{
	return;
	////*** temp code
	//char* name[5] = { "Austria", "Bavaria", "Germany", "Saxony", "Turkey"}; //temp
	//for (int i=4; i>m_iPressed2-1 ; i--) {
	//	name[i] = ""; //"..";//temp
	//}
	//if ( m_iPressed2==1 ){
	//	m_pdmcTexts->SetValues(GetTextByID("#CDT_InPEACE"), name);
	//}
	//else{
	//	if ( m_iPressed2==2 ){
	//		m_pdmcTexts->SetValues(GetTextByID("#CDT_InNONA"), name);
	//	}
	//	else{
	//		if ( m_iPressed2==3 ){
	//			m_pdmcTexts->SetValues(GetTextByID("#CDT_InPASS"), name);
	//		}
	//		else{
	//			if ( m_iPressed2==4 ){
	//				m_pdmcTexts->SetValues(GetTextByID("#CDT_InALLY"), name);
	//			}
	//		}
	//	}
	//}
	////*** temp code
}
//----------------------------------------------------------------------------------
int		CDiplMenuButOnMap::GetButOffset()
{
	return m_iButOffset;
}
//----------------------------------------------------------------------------------
int		CDiplMenuButOnMap::GetFirstPressed() 
{
	return m_iPressed1;
}
//----------------------------------------------------------------------------------
int		CDiplMenuButOnMap::GetSecondPressed() 
{
	return m_iPressed2;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CDiplMenuCountries   ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
CDiplMenuCountries::CDiplMenuCountries(DialogsSystem* pMenu,int N,int TxtOffset)
{
	m_pMenu = pMenu;

	m_iN = N;
	m_iTxtOffset = TxtOffset;

	m_Visible = false;
}
//----------------------------------------------------------------------------------
void	CDiplMenuCountries::SetVisible(bool state) 
{
	if (m_Visible==state) { return; }
	
    SimpleDialog*	a_pdb = NULL;
	for (int i=0; i < m_iN; i++) //тексты
	{
		a_pdb = m_pMenu->Find(m_iTxtOffset + i);
		if (a_pdb==0x00000000) { return; }
		a_pdb->Visible = state;
	}

	m_Visible=state;
}
//----------------------------------------------------------------------------------
void	CDiplMenuCountries::SetVisible(bool state, int param) 
{
	if (m_Visible==state) { return; }
	
    SimpleDialog*	a_pdb = NULL;
	for (int i=0; i < m_iN; i++) //тексты
	{
		a_pdb = m_pMenu->Find(m_iTxtOffset + i);
		if (a_pdb==0x00000000) { return; }
		a_pdb->Visible = state;
		//if (state && i<4 && i!=param-1) a_pdb->Visible = false;//new
	}

	m_Visible=state;
}
//----------------------------------------------------------------------------------
bool	CDiplMenuCountries::SetValueById(int id, char* value) 
{
	TextButton*	a_ptb;
	a_ptb = (TextButton*) m_pMenu->Find(m_iTxtOffset + id);
	if (a_ptb==0x00000000) { return false;}
	//a_ptb->Message = value;
	BigMapSetMessage( a_ptb , value ) ;

	return true;
}
//----------------------------------------------------------------------------------
void	CDiplMenuCountries::SetValues(char** ppNames) /*char* name[5]*/
{
	int j=0;
	for (int i=1; i<m_iN; i++)
	{
		if (!SetValueById(i, ppNames[j++])) { return; }
	}
}
//----------------------------------------------------------------------------------
void	CDiplMenuCountries::SetDealValues(int param, char** ppNames)
{
	for (int i=0; i<4; i++)
	{
		TextButton*	a_ptb;
		a_ptb = (TextButton*) m_pMenu->Find(m_iTxtOffset + i);
		if (a_ptb==0x00000000) { break; }
		
		if (i+1==param) { a_ptb->Visible = true;  }
		else		    { a_ptb->Visible = false; }
	}

	int j=0;
	for (int i=4; i<m_iN; i++)
	{
		if (!SetValueById(i, ppNames[j++])) { return; }
	}
}
//----------------------------------------------------------------------------------
int		CDiplMenuCountries::GetTxtOffset()
{
	return this->m_iTxtOffset;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////
///////////////////////////   CDiplMenu   ////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////
//CDiplMenu::CDiplMenu(DialogsSystem* pMenu, GP_Button* pMainOK, GP_Button* pMainNO,
CDiplMenu::CDiplMenu( DialogsSystem*pMenu, GP_TextButton*pMainOK, GP_TextButton* pMainNO ,
					 CSectData* pSectData, CDiplomacyData* pDiplData,
					 CDiplMenuInfo* pInfoTexts, CDiplMenuCountries* pWarsTexts, CDiplMenuCountries* pDealsTexts,
					 CDiplMenuButtons* pActButtons, CDiplMenuButOnMap* pMapButtons,
					 CDiplInfoMessages*	pMessages, CDiplInfoButtons* pInfoButtons, CDiplInfoMessBox* pMessBox ,
					 CSectorMenu*		pSectMenu )
{
	m_pMenu        = pMenu;
	//m_pMainOK = pMainOK; 	//m_pMainNO = pMainNO;	

	m_pSectMenu    = pSectMenu ; 

	m_pSectData    = pSectData;
	m_pDiplData    = pDiplData;

	m_pInfoTexts   = pInfoTexts;
	m_pWarsTexts   = pWarsTexts;
	m_pDealsTexts  = pDealsTexts;

	m_pActButtons  = pActButtons;
	m_pMapButtons  = pMapButtons;

	m_pMessages	   = pMessages;
	m_pInfoButtons = pInfoButtons;
	m_pInfoBox	   = pMessBox;
	
	m_pActButtons->SetMainOK(pMainOK); 
	m_pActButtons->SetMainNO(pMainNO); 

	for(int i=0;i<MAX_PLAYERS;i++) m_pPlayer[i] = NULL; 

	m_pMessLtrs  = ENUM.Get( "InfoLtr_ENUM"  );
	m_pMessInfo  = ENUM.Get( "InfoTxt_ENUM"	 ); 
	m_pMessHeads = ENUM.Get( "MessHead_ENUM" ); 
	m_pMessT	 = ENUM.Get( "MessText_ENUM" ); 

	m_Visible = false;
	m_Enabled = true;
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::operator +=(CPlayer* pPlayer) 
{
	if (pPlayer!=NULL)
		{
		int pos = -1;
		for (int i=0; (i<MAX_PLAYERS)&&(pos==-1); i++)
			{
			if (m_pPlayer[i]==NULL)
				{
				pos = i;
				}
			}
		if (pos!=-1)
			{
			m_pPlayer[pos] = pPlayer;
			return true;
			}
        }
	return false;
} // Vitya Ver.
//----------------------------------------------------------------------------------
bool	CDiplMenu::operator +=(CDiplMenuButOnMap* pMapButt)
{
	if( pMapButt ) {
		this->m_pMapButtons = pMapButt;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::operator +=(CDiplMenuInfo* pInfoTexts)
{
	if( pInfoTexts ) {
		this->m_pInfoTexts = pInfoTexts;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::Show(bool state, bool act ) 
{
	SetVisible(state, act );
	SetEnabled(state, act );

	return true;
}
//----------------------------------------------------------------------------------
void	CDiplMenu::SetVisible(bool state, bool act ) 
{
	if ( m_Visible==state && act ) { return; }

	m_pInfoTexts->SetVisible(state);

	//m_pWarsTexts->SetVisible(state);
	//m_pDealsTexts->SetVisible(state);			//, this->m_pMapButtons->GetSecondPressed() );

	m_pActButtons->SetVisible( state && act );  //&& act );	//m_pActButtons->SetVisible(state);
	m_pMapButtons->SetVisible(state);

	m_Visible=state;
}
//----------------------------------------------------------------------------------
void	CDiplMenu::SetEnabled(bool state, bool act ) 
{
	if ( m_Enabled==state /*&& act*/ ) { return; }

	//m_pInfoTexts->SetEnabled(state);
	//m_pWarsTexts->SetEnabled(state);
	//m_pDealsTexts->SetEnabled(state);

	m_pActButtons->SetEnabled( state /*&& act*/ );  //&& act );	//m_pActButtons->SetEnabled(state);
	m_pMapButtons->SetEnabled(state);

    m_Enabled=state;
}
//----------------------------------------------------------------------------------
void	CDiplMenu::Process(int* MapPressedButton, int DiplAction, int ActiveMenu, bool MenuIsChange , bool ActEn )	
{
	bool	changed = false;
	int		SecID   = m_pSectData->m_LastLMCSecID;
	int		owner	= m_pSectData->GetSecOwner( SecID );					
	
	if( MenuIsChange || m_pInfoTexts->ChangeSecInfo( SecID , m_pPlayer ) )		{ changed = true; }

	if( m_pMapButtons->ShowPressedButton( MapPressedButton, SecID ) )			{ changed = true; }

	#ifdef DemoAI	//DEMO
		goto proEND; 
	#endif

	if(  ActEn && m_pActButtons->DiplActionPressed( DiplAction ) && owner >= 0 && owner < MAX_PLAYERS &&
		!CheckRequests( DiplAction, ActiveMenu , SecID ) ) 
		{ 
			if( ContractEnabled( DiplAction, m_pDiplData->GetPlayerNId(), owner ) ) 
			{ 
				if( m_pActButtons->StartDiplAction(m_pSectData,m_pDiplData,m_pPlayer,m_pMapButtons, DiplAction ) )
				{
					changed = true;
					AddContractMoney( DiplAction, m_pDiplData->GetPlayerNId(), owner );
					
					if( DiplAction == m_pDiplData->GetUnionId() ) { // union
						for(int id=0; id < m_pSectData->GetSectorsNum() ; id++)			// union sectors
							if( m_pSectData->GetSecOwner( id ) == owner ) 
								(m_pSectData->m_psddDT[ id ])->SetOwner( m_pDiplData->GetPlayerNId() );

						m_pPlayer[owner]->m_Alive = false ;								// kill country
						for(int h=0; h < m_pPlayer[owner]->m_inHeroNum ; h++)			// kill alive heroes  
							m_pPlayer[owner]->DeleteHero( h ) ;
					}
				}
			}
			else {
				char text[256]; 
				sprintf(text, m_pMessT->Get( 8 ) , m_pPlayer[ owner ]->GetPlayerName() ); 
				ShowBigMapGlobalMessage(1, 33 , 11 , text , 1 ); /* "CONTRACT NOT ENABLED" <turned you down> */
			}
			//very new
			m_pActButtons->CreateHint();
		}

	m_pActButtons->SetActive( -1 );

proEND: //DEMO

	if( MenuIsChange || changed ) {
		SetSectorsColor( m_pMapButtons->GetFirstPressed() , m_pMapButtons->GetSecondPressed() );
		//#ifdef DemoAI		//DEMO
		//	return ;
		//#endif			//DEMO
		if( ActEn ) CheckEnabledDiplButtons();
	}
	return;
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::CheckRequests( int type, int ActiveMenu, int SecID )
{
	int types[4] = { type, 0, 0, 0 } ,  n = 1 ; 		
	if(type == m_pDiplData->GetWarId())   { types[0] = m_pDiplData->GetPeaceId(); types[1] = m_pDiplData->GetPactId();  types[2] = m_pDiplData->GetPassId(); types[3] = m_pDiplData->GetAllyId(); n = 4; }
	if(type == m_pDiplData->GetPeaceId()) { types[0] = m_pDiplData->GetPeaceId(); n = 1; }
	if(type == m_pDiplData->GetPactId())  { types[0] = m_pDiplData->GetPactId();  types[1] = m_pDiplData->GetAllyId(); n = 2; }
	if(type == m_pDiplData->GetPassId())  { types[0] = m_pDiplData->GetAllyId();  n = 1; }
	if(type == m_pDiplData->GetAllyId())  { types[0] = m_pDiplData->GetPactId();  types[1] = m_pDiplData->GetPassId(); types[2] = m_pDiplData->GetAllyId(); n = 3; }
	if(type == m_pDiplData->GetUnionId()) { types[0] = m_pDiplData->GetPactId();  types[1] = m_pDiplData->GetPassId(); types[2] = m_pDiplData->GetAllyId(); n = 3; }

	int id ;
	for(int i=0; i < n ; i++) 
	{
		id = m_pDiplData->GetRequestId( types[i], m_pSectData->GetSecOwner( SecID ) );
		if( id >= 0 ) {
			id = m_pMessages->GetMessageId( id );
			if( id >= 0 ) {
				m_pMessages->PrepareBlink() ;	// blink ON
				m_pMessages->ShowMessage( id );
				m_pMessages->ScrollInformation();

				char text[256];
				sprintf(text, m_pMessT->Get( 7 ) , GetTextByID("#CWV_Messages") ); 
				ShowBigMapGlobalMessage( 1 , 33 , 11 , text , ActiveMenu );
				return true;
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
void	CDiplMenu::SetSectorsColor(int state1, int state2)
{
	const int MaxN = 100;
	int   PlayerId = m_pSectData->GetSecOwner( m_pSectData->m_LastLMCSecID );			//m_pSectData->GetSecOwner(id)

	//if (PlayerId == -1) {	//если сектор - нейтральный (ничейный)
	//	state1 += 10;	//	state2 += 10;	//	//PlayerId = m_pSectData->m_LastLMCSecID;   }

	int  war[MaxN] ,  deal[MaxN]; 


	int  nwar  = GetContractsById( state1, PlayerId , war  );
	int  ndeal = GetContractsById( state2, PlayerId , deal );

	//раскраска секторов
	m_pSectData->setRedDip  ( PlayerId , war  , nwar  ); 
	m_pSectData->setGreenDip( PlayerId , deal , ndeal ); 

	//char* warname[MaxN];							
	//char* dealname[MaxN];							
	//for(int i=0; i < MaxN ; i++) { warname[i]  = ""; dealname[i] = ""; }
	//списки стран с дипл.договорами	
	//for(int i=0; i < nwar ; i++)   warname[i] = m_pPlayer[war[i]]->GetPlayerName();
	//for(int i=0; i < ndeal ; i++)  dealname[i] = m_pPlayer[deal[i]]->GetPlayerName();	
	//сортировка списков стран
	//for(int i=0; i<nwar-1; i++) { 
	//	int min = i;
	//	for(int j=i+1; j<nwar; j++) {
	//		for(int c=0; c<3 ; c++ )	// ; c<=4 ;
	//			if ( *(warname[min] + c )  > *(warname[j] + c ) ) { min = j; break; } 
	//			else	
	//				if ( *(warname[min] + c ) != *(warname[j] + c ) ) { break; }
	//	}
	//	char* s = warname[i];
	//       warname[i] = warname[min];
	//	warname[min] = s;
	//}
	//for(int i=0; i<ndeal-1; i++) { 
	//	int min = i;
	//	for(int j=i+1; j<ndeal; j++) {
	//		for(int c=0; c<3 ; c++ )	// ; c<=4 ;
	//			if ( *(dealname[min] + c )  > *(dealname[j] + c ) ) { min = j; break; }	
	//			else
	//				if ( *(dealname[min] + c ) != *(dealname[j] + c ) ) { break; }	}
	//	char* s = dealname[i];
	//       dealname[i] = dealname[min];
	//	dealname[min] = s;
	//}
	//вывод списков стран
	//m_pWarsTexts->SetValues (warname);
	//if (state2>10) state2-=10;
	//m_pDealsTexts->SetDealValues(state2, dealname);

	
	// W- WAR    - PEACE   N-  PACT   P-  PASS   A- ALLY   N+P- PACT+PASS
	//    В		   М		   Н		  П		    А	        Н+П	
	for(int i=0; i < MAX_PLAYERS ; i++) 
		for(int j=0; j < MAX_PLAYERS ; j++) {
			if( i != j )	m_iST[i][j] = -1 ;
			else			m_iST[i][j] = 99 ;
		}
	int  id   = 0;
	int  type = 0;
	for(int i=0; i < MAX_PLAYERS ; i++) 
		if( m_pPlayer[i]->m_Alive ) 
		{
			type  = m_pDiplData->GetWarId();
			nwar  = GetContractsById( type , i , war  );
			for(int j=0; j < nwar ; j++)  { id = war[j] ;   if( m_pPlayer[id]->m_Alive )  m_iST[i][ id ] = type; }

			// non-a - pass - ally
			type  = m_pDiplData->GetPactId(); // non-a
			ndeal = GetContractsById( type , i , deal );
			for(int j=0; j < ndeal ; j++) { id = deal[j] ;  if( m_pPlayer[id]->m_Alive )  m_iST[i][ id ] = type ; }

			type  = m_pDiplData->GetPassId(); // pass
			ndeal = GetContractsById( type , i , deal );
			for(int j=0; j < ndeal ; j++) { 
				id = deal[j] ;  
				if( m_pPlayer[id]->m_Alive ) {
					if( m_iST[i][ id ] < 0 )		m_iST[i][ id ] = type ; 
					else							m_iST[i][ id ] = 5 ; // non-a + pass
				}
			}
			type  = m_pDiplData->GetAllyId(); // ally
			ndeal = GetContractsById( type , i , deal );
			for(int j=0; j < ndeal ; j++) { id = deal[j] ;  if( m_pPlayer[id]->m_Alive )  m_iST[i][ id ] = type ; }
	}

	_str  a_Hint ;	
	char  text[256];
	for(int i=0; i < MAX_PLAYERS ; i++) 
		for(int j=0; j < MAX_PLAYERS ; j++) 
		{
			a_Hint.DString::Clear();		a_Hint += "";
			strcpy( text, " " );

			type = m_iST[i][j];
			if( type >= 0 && type < 99 ) {
				strcpy( text, m_pInfoTexts->m_chStat[ type ] );
				// hints
				a_Hint += m_pInfoTexts->m_chStatHint[ type ] ;
				if( type == 5 )  type = m_pDiplData->GetPassId();
				if( type > 1 && ( i == gi_bmSAVE_Nation || j == gi_bmSAVE_Nation ) && 
					GetContractPeriod( type , i , j ) >= 0 ) {
					int  period = GetContractPeriod( type , i , j );
					char  add_txt[256];		sprintf( add_txt, GetTextByID( "#CHINT_TurnsLeft" ) , period );
					a_Hint += " ";			a_Hint += add_txt ;		} // (%d)
			}
			m_pInfoTexts->m_pSTATES[i][j]->SetMessage( text );
			//new
			//m_pInfoTexts->m_pSTATES[i][j]->Hint = m_pInfoTexts->m_chStatHint[ type ]; //strcpy(x->Hint,y);
			DYNHINT_RA( m_pInfoTexts->m_pSTATES[i][j] , a_Hint.pchar() );
		}
}
//----------------------------------------------------------------------------------
void	CDiplMenu::CheckEnabledDiplButtons()
{
	const int MaxN = 100;

	int SecPlayerId = m_pSectData->GetSecOwner( m_pSectData->m_LastLMCSecID );
//	if (SecPlayerId < 0) m_pMapButtons->SetEnabled(false);//	else m_pMapButtons->SetEnabled(true);

	if (SecPlayerId == m_pDiplData->GetPlayerNId()  || SecPlayerId < 0 ) { 
		m_pActButtons->SetVisible( false );
		m_pActButtons->SetEnabled( false ); 
		return;	
	}

	m_pActButtons->SetVisible( true );
	//if (SecPlayerId == -1)  SecPlayerId = m_pSectData->m_LastLMCSecID;	//если сектор - нейтральный (ничейный)

	//DEMO
	#ifdef DemoAI
		return ;
	#endif
	//DEMO

	int n=0;
	int types[MaxN]; 
	n = GetContTypesById( m_pDiplData->GetPlayerNId() , SecPlayerId , types );
	m_pActButtons->SetDisabledButtons( m_pDiplData , types , n );
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::ContractEnabled( int type, int country1, int country2 ) // war // peace pact pass ally union
{
	int   PL_NI		= gi_bmSAVE_Nation;
	int	  WAR		= m_pDiplData->GetWarId(); 
	int	  PEACE		= m_pDiplData->GetPeaceId(); 
	int	  PACT		= m_pDiplData->GetPactId(); 
	int	  PASS		= m_pDiplData->GetPassId();
	int	  ALLY		= m_pDiplData->GetAllyId();
	int	  UNION		= m_pDiplData->GetUnionId(); 

	if( type == WAR )										{ return true; }

	// check Allies
	if(	type == ALLY ) {
		if(	GetContractsNumByType( ALLY, country1 ) > 1 )	{ return false; } // not more than 2 ALLY-contracts
		if(	GetContractsNumByType( ALLY, country2 ) > 1 )	{ return false; }
	}

	// check Non-A.PACT number	//if( type == ALLY || type == PACT ) 
	if( type == PACT || 
	  ( type == ALLY && !CheckContract( PACT, country1, country2 ) ) ) {
		int  MaxN = MAX_PLAYERS/2 ;
		if(	GetContractsNumByType( PACT, country1 ) >= MaxN ) { return false; } // not more than N PACT-contracts
		if(	GetContractsNumByType( PACT, country2 ) >= MaxN ) { return false; } 
	}

	// check Sectors Number
	int  SecNN   = m_pSectData->GetSectorsNum();
	int  SecNum1 = m_pSectData->GetPlayerSect( country1 );
	int  SecNum2 = m_pSectData->GetPlayerSect( country2 );	
	if( SecNum1 > SecNN/2 || SecNum2 > SecNN/2 )			{ return false; } // country have too much sectors

	// check countries number
	int  count = 0;
	for(int i=0; i < MAX_PLAYERS; i++) if( m_pPlayer[i] && m_pPlayer[i]->m_Alive )  count++;
	if( count < 3 )											{ return false; } // there is only 2 countries on map

	// check Resources for country2
	if( count > 1 && SecNum2 > SecNN / (count - 1) )
	{
		int resAll = 0;
		for(int res=0; res <= COAL ; res++) {	
			int am = m_pPlayer[ country2 ]->GetResource( res ) ; 
			if( res == FOOD ) am /= 100;
			resAll += am; 
		}
		resAll = resAll / 7 ;
		if( resAll > 35000 )								{ return false; } // country2 too strong (very much res.)
	}

	// check Allies Conflicts
	if(	type == ALLY ) {
		int  CountId[100],  num;  CountId[0] = country1;  CountId[1] = country2;  num = 2;
		int  arr[100]; 
		for(int i=0; i < num ; i++) {
			int n = GetContractsById( ALLY , CountId[i] , arr ); // find all Allies for both countries
			if( n > 0 )	
				for(int k1=0; k1 < n ; k1++) {
					bool  find = false;
					for(int k2=0; k2 < num && !find ; k2++)   if( arr[ k1 ] == CountId[ k2] ) find = true;
					if( !find ) CountId[ num++ ] = arr[ k1 ];
				}
		}
		for(int i=0; i < num; i++) 
			if( CheckContract(WAR, country1 ,CountId[i]) || 
				CheckContract(WAR, country2 ,CountId[i]) )	{ return false; } // WAR with one of Allies 
	}


	// Emergency Rules
	// 1. if PEACE && country2 == PL_NI  -  check War cont Period ( < MAX_CONT_VALPERIOD - 1 /*9*/ )
	if( type == PEACE && country2 == PL_NI &&
		GetContractPeriod( WAR , country1, country2 ) >= MAX_CONT_VALPERIOD - 1 ) {  /* < 9 */
			return false;// WAR declared just now
		}
	// end of Emergency Rules

	return true;
}

//----------------------------------------------------------------------------------
bool	CDiplMenu::AddContract(int type, int country1, int country2, int time) // Contract Type, Country N1, Country N2, Period
{
	if( !ContractEnabled( type , country1, country2) )		return false;

	int  playerId = m_pDiplData->GetPlayerNId();
	int  warId    = m_pDiplData->GetWarId();

	if( ( country1 != playerId && country2 != playerId ) || type == warId ) 
	{
		m_pDiplData->AddContract(type, country1, country2, time);
	
		if( type != warId )  { AddContractMoney( type, country1, country2 ); }
		
		if( country2 == playerId && type == warId ) { AddRequestMessage( type, 0 , type, country1, country2, time ); }
	}
	else	
	{
		int  requestId = m_pDiplData->AddRequestForContract( type, country1, time );	

		AddRequestMessage( type, 2 , type, country1, country2, time, requestId );
	}

	return true;
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::AddMessage(int priority,int MessageType,int MessagePictId, //MessageType = 2 - ok/close; MessageType = 3 - ok/no/close
							  char* ButtonLetter,char* MessageTitle,char* MessageText, bool NewMess )
{
	// MessageType = 2 - ok/close;	MessageType = 3 - ok/no/close
	return	m_pMessages->AddMessage(priority, MessageType, MessagePictId, ButtonLetter, MessageTitle, MessageText, NewMess);
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::AddRequestMessage(int priority,int MessageType,int type,int country1,int country2,int time)   
{
	return	AddRequestMessage( priority, MessageType, type, country1, country2, time, -1 );
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::AddRequestMessage(int priority,int MessageType,int type, int country1,int country2,int time,int reqId)   
{
	if( country1 < 0 || country1 >= MAX_PLAYERS )  return  false;

	char   text[256];
	char*  pPlayerName  = m_pPlayer[country1]->GetPlayerName();
	char*  pMessageText = m_pMessInfo->Get(type);

	if( type == 0 )	{ sprintf(text, pMessageText, pPlayerName ); }
	else			{ sprintf(text, pMessageText, pPlayerName, m_pDiplData->GetContractCost(type)/1000 ); } //MONEY
    
	int pic = m_pDiplData->GetDiplActionPicture( type, country1, country2 );

	return m_pMessages->AddMessage(priority,MessageType,pic,m_pMessLtrs->Get(type),m_pMessHeads->Get(type),text,reqId, true );
}
//----------------------------------------------------------------------------------
void	CDiplMenu::AddContractMoney(int type, int country1, int country2)
{
	#ifdef DemoAI	//DEMO
		return ;
	#endif			//DEMO

	bool  ChangeCost = false ;
	if( country1 == m_pDiplData->GetPlayerNId() )  ChangeCost = true;

	int ContMoney = m_pDiplData->GetContractCost2( type , ChangeCost );

	m_pPlayer[ country1 ]->AddMoney( -ContMoney );			// отнять деньги у country1
	m_pPlayer[ country2 ]->AddMoney(  ContMoney );			// добавить деньги country2
}
//----------------------------------------------------------------------------------
void	CDiplMenu::AddRequestMoney( int country1, int country2, int money )
{
	m_pPlayer[ country1 ]->AddMoney( -money );				// отнять деньги у country1
	m_pPlayer[ country2 ]->AddMoney(  money );				// добавить деньги country2
}
//----------------------------------------------------------------------------------
void	CDiplMenu::ClearData() 
{
	m_pMessages->ClearMessages();			
	m_pDiplData->ClearRequests();	
}

//----------------------------------------------------------------------------------
void	CDiplMenu::PrepareNextRound( int turn )
{
	int n = 0; 
	int conts[400]; 
	m_pDiplData->SetContractCost( turn );

	m_pDiplData->RecountContractsPeriod( conts, n ); 	
	SetContractEndsMess( conts, n ); 

	m_pMessages->PrepareBlink(); // blink ON
}
//----------------------------------------------------------------------------------
void	CDiplMenu::SetContractEndsMess(int* conts, int n)
{
	for(int i=0; i < n ; i+=3)
	{
		int country1;
		if(conts[i+1]==m_pDiplData->GetPlayerNId()) { country1 = conts[i+2]; } else { country1 = conts[i+1]; }

		if( m_pPlayer[ country1 ]->m_Alive ) {
			char text[256];
			char* pPlayerName  = m_pPlayer[country1]->GetPlayerName();
			char* pMessageText = m_pMessInfo->Get( 4 + conts[i] );
			sprintf( text, pMessageText, pPlayerName  );

			AddMessage( conts[i]-10 , 0 , m_pDiplData->GetDiplActionPicture( conts[i], conts[i+1], conts[i+2] ) ,
										  m_pMessLtrs->Get( 10 ) , m_pMessHeads->Get( 10 ) , text , true );						
		}
	}
}
//----------------------------------------------------------------------------------
int		CDiplMenu::ShowGlobalMessage( int type, int pictId, int headId, char* text, int pageN , SimpleDialog* pSD )//-EDITED!!!-
{
	pSD->Enabled = false;
	int n = ShowGlobalMessage( type, pictId, headId, text, pageN ) ;	
	pSD->Enabled = true; 
	return n;
}
//----------------------------------------------------------------------------------
int		CDiplMenu::ShowGlobalMessage( int type, int pictId, int headId, char* text, int pageN ) 
{
	// type=0(-)   type=1(OK)   type=2(YES/NO)   type=3(YES/NO/CANCEL)   type=4(AGREE/NO/CANCEL)   
	// type=10(Diversions)

	//if(!(LockMouse||EditMode||TotalUnPress)){
	UnPress();								//Global
	Lpressed = KeyPressed = LastKey = 0;	//Global

	DialogsSystem	a_dsMenu;									// новое меню

	a_dsMenu.SetWidth(1024);//new bug add!!!
	a_dsMenu.SetHeight(768);//new bug add!!!

	char sTMP[256];
	int mDX = 7 /*15*/ ,  mDY = 2 /*5*/ /*7*/ ,  mMaxH = 98 - 2*mDY ;	

	RLCFont*	ActFont = &fonMenuText[3] ;									//&WhiteFont ;
	RLCFont*	PasFont = &fonMenuText[2] ;									//&YellowFont ;
	Enumerator* BUTTON_ENUM = ENUM.Get("BUTTON_ENUM");				// buttons text Enumerator
	
	// обычный текст
	TextButton* ptbText  = NULL;
	// multi-ask текст									
	TextButton* ptbText0 = NULL;	TextButton* ptbText1 = NULL;	TextButton* ptbText2	 = NULL;	TextButton* ptbText3	 = NULL; 
	// кнопки
	GP_TextButton* ptbOk = NULL;	GP_TextButton* ptbNo = NULL;	GP_TextButton* ptbCancel = NULL;

	// BlackScreen
	LocalGP		a_lgpScreen("Interf3\\TotalWarGraph\\blackscr");			
	GPPicture*	a_pgppScr = a_dsMenu.addGPPicture(NULL,0,0,a_lgpScreen.GPID,0);
	a_pgppScr->Diffuse = 0x88202020;
	a_pgppScr->Enabled = true ;
	a_pgppScr->OnUserClick = a_pgppScr->OnUserRightClick = a_pgppScr->OnMouseOver = &EmptyClick ;

	// Message box
	int		X0 = 206 , Y0 = 150 ;									//if( type == 10 ) { X0 -= 50 ;   Y0 -= 50 ; }
	LocalGP		a_lgpMess("Interf3\\TotalWarGraph\\dMessage");	
	GPPicture*	pgppBox = a_dsMenu.addGPPicture(NULL, X0, Y0 ,a_lgpMess.GPID, (type<10)?type:12 ); //(type<10)?type:1 );
	// Message box picture
	LocalGP		a_lgpPict("Interf3\\TotalWarGraph\\dMesspic");	/*+66  +86*/	/*+76  +96*/
	GPPicture*	pgppPict = a_dsMenu.addGPPicture(NULL, pgppBox->x +64, pgppBox->y +76 ,a_lgpPict.GPID, pictId );
	//ramka
	GPPicture*	pgppFrame= a_dsMenu.addGPPicture(NULL, pgppPict->x +2, pgppPict->y +2,a_lgpMess.GPID, 8 ); 
	// Message head
	strcpy( sTMP, m_pMessHeads->Get( headId ) ) ;				
	a_dsMenu.addTextButton(NULL, pgppBox->x +304, pgppBox->y +32 , sTMP ,&fonMenuTitle[3],&fonMenuTitle[3],&fonMenuTitle[3], 1 );		
	//a_dsMenu.addTextButton(NULL, pgppBox->x +304, pgppBox->y +33 , sTMP ,PasFont,PasFont,PasFont, 1 );		


	// Message text
	int  tbX = pgppBox->x + (pgppBox->x1-pgppBox->x)/2 ,  tbY = pgppBox->y +221  + mDY  ; //+76 +145 ;
	if( pgppPict->y1 > 0 && pgppPict->y1 < 1000 )  tbY = pgppPict->y1 + mDY ; //pgppPict->y1 = 160  ( pic H!!! )
	int OkParam = 0 ; // yes
	if( type < 10 ) {
		ptbText = a_dsMenu.addTextButton( NULL, tbX , tbY , text ,
										  &fonMenuText[0],&fonMenuText[0],&fonMenuText[0], 1 ) ; //0); //NEED NEW FONTS?
		ptbText->MaxWidth = pgppPict->x1 - pgppPict->x - 2*mDX ;
		if( ptbText->MaxWidth <= 0 || ptbText->MaxWidth > 480 ) ptbText->MaxWidth = 480 ;
		TextButton_OnDraw( ptbText ) ;
		ptbText->Visible  = false ;

		// choose buttons
		//int OkParam = 0 ; // yes
		if(type==1) OkParam = 2 ; // ok
		if(type==4) OkParam = 3 ; // agree

		if( type > 0 ) {
			// Button Ok =N1
			strcpy( sTMP, BUTTON_ENUM->Get( OkParam ) ) ;	//408, 485
			ptbOk = a_dsMenu.addGP_TextButton(NULL, pgppBox->x+202,pgppBox->y+335 ,sTMP,a_lgpMess.GPID,6,ActFont,PasFont); 
			ptbOk->FontDy		= -1 ;//1 ;
			ptbOk->UserParam	= 0;
			ptbOk->OnUserClick	= &OnPIButtonClicked;
			ptbOk->AssignSound( "INTERF_NAVEL", MOUSE_SOUND ); //new
			ptbOk->AssignSound( "INTERF_NAGAL", CLICK_SOUND );	

			_str	a_Hint1;	a_Hint1 += "{FS}{C FF502515}";  
			a_Hint1 += sTMP;	a_Hint1 += " ";		a_Hint1 += GetTextByID("#CHINT_ENTERKEY");
			DYNHINT(ptbOk,a_Hint1.pchar());
		}
		if( type > 1 ) {
			// Button No =N2		//ok286,485 //n529,485
			strcpy( sTMP, BUTTON_ENUM->Get( 1 ) ) ;
			ptbNo	= a_dsMenu.addGP_TextButton(NULL,529,485, sTMP ,a_lgpMess.GPID,6, ActFont,PasFont); 
			ptbNo->FontDy			= -1 ;//1 ;
			ptbNo->UserParam		= 0 ;
			ptbNo->OnUserClick		= &OnPIButtonClicked;
			ptbNo->AssignSound( "INTERF_NAVEL", MOUSE_SOUND ); //new
			ptbNo->AssignSound( "INTERF_NAGAL", CLICK_SOUND );		

			_str	a_Hint1;	a_Hint1 += "{FS}{C FF502515}";  
			a_Hint1 += sTMP ;   a_Hint1 += " ";   a_Hint1 += GetTextByID("#CHINT_ESCKEY");
			DYNHINT(ptbNo,a_Hint1.pchar());

			ptbOk->x1 = 286 + (ptbOk->x1 - ptbOk->x);		ptbOk->x  = 286; // NEW Ok-position
		}
		if( type > 2 ) { 
			// Button Cancel =N3
			strcpy( sTMP, BUTTON_ENUM->Get( 4 ) ) ;
			ptbCancel	= a_dsMenu.addGP_TextButton(NULL,729,485, sTMP ,a_lgpMess.GPID,6, ActFont,PasFont); 
			ptbCancel->FontDy		= -1 ;//1 ;
			ptbCancel->UserParam	= 0 ;
			ptbCancel->OnUserClick	= &OnPIButtonClicked;
			ptbCancel->AssignSound( "INTERF_NAVEL", MOUSE_SOUND ); //new
			ptbCancel->AssignSound( "INTERF_NAGAL", CLICK_SOUND );

			_str	a_Hint1;	a_Hint1 += "{FS}{C FF502515}";	a_Hint1 += sTMP ;
			DYNHINT(ptbCancel,a_Hint1.pchar());

			// + NEW Ok-position && NEW No-position
		}
	}

	GPPicture*	pgppDivBox = NULL;
	if( type == 10 ) {
		// Diversions
		//
		int dFrX = -10;
		pgppPict->y += dFrX;		pgppFrame->y += dFrX;		pgppFrame->y1 += dFrX; //-= 6;		
		pgppDivBox = a_dsMenu.addGPPicture(NULL, pgppFrame->x +1 , pgppFrame->y1 +8 ,a_lgpMess.GPID, 9 );

		//
		if( !m_pSectMenu->ShowDiversionActions( &a_dsMenu , pgppDivBox->x , pgppDivBox->y ) ) { return 3; }
		// Button Cancel =N3
		strcpy( sTMP, BUTTON_ENUM->Get( 5 ) ) ;	
		ptbCancel	= a_dsMenu.addGP_TextButton(NULL, pgppBox->x+202,pgppBox->y+335,sTMP,a_lgpMess.GPID,6,ActFont,PasFont); 
		ptbCancel->FontDy		= -1 ;//1 ;
		ptbCancel->UserParam	= 0 ;
		ptbCancel->OnUserClick	= &OnPIButtonClicked;
		ptbCancel->AssignSound( "INTERF_NAVEL", MOUSE_SOUND ); //new
		ptbCancel->AssignSound( "INTERF_NAGAL", CLICK_SOUND );

		_str	a_Hint1;	a_Hint1 += "{FS}{C FF502515}";  
		a_Hint1 += sTMP ;   a_Hint1 += " ";   a_Hint1 += GetTextByID("#CHINT_ESCKEY");
		DYNHINT(ptbCancel,a_Hint1.pchar());
	}

	int  ActButActive = m_pActButtons->m_iActive;
	ROOT_SCENARIO.m_pSectMenu->m_inCurSectID = -1 ;
	ROOT_SCENARIO.UPDATE_SCENARIO();
	ROOT_SCENARIO.PROCESS(g_dXM,g_dYM,g_WM,g_HM, gi_bmSAVE_Nation , 0 );	//GREY [27.07.2004]
	m_pActButtons->m_iActive = ActButActive;

	SetBigMapMenuEnabled( false );	//GREY [15.03.2004]

	m_pMenu->ProcessDialogs();		ProcessMessages();		StdKeys();		a_dsMenu.ProcessDialogs();

	UnPress();					//Global
	KeyPressed = LastKey = 0;   //Global

	ItemChoose = - 1;

	int   quit	   = 0;
	bool  changeY  = true ;
	do {
		ProcessMessages();
		int LPr=Lpressed;
		m_pMenu->ProcessDialogs();
		Lpressed=LPr;
		a_dsMenu.ProcessDialogs();
		StdKeys();
		a_dsMenu.RefreshView();
		// center message text
		if( ptbText && pgppPict ) {
			int h = ptbText->y1 - ptbText->y ;
			if( changeY && h < mMaxH &&  pgppPict->y1 > 0  ) {
				int dH = ( mMaxH - h ) / 2 ;
				ptbText->y = pgppPict->y1 + dH + mDY ; 
				changeY = false ;
			} 
			ptbText->Visible = true ;
		}
		//
		// close Diversions
		if( type == 10 ) {
			bool  DivDisabled = true;	
			for(int i=0; i < m_pSectMenu->m_iDTN; i++) {
				if( m_pSectMenu->m_ptbDText[i]->Visible )  DivDisabled = false;
			}
			if( DivDisabled ) {
				ShowBigMapGlobalMessage(1, 33 , 20 , GetTextByID("#SABSECDONE") ,0); 
				ItemChoose = mcmCancel;
			}
		}
		//
		if( ItemChoose == mcmOk	    && ptbOk )		ptbOk->UserParam	 = 1 ;		// Ok
		if( ItemChoose == mcmCancel && ptbNo )		ptbNo->UserParam	 = 1 ;		// No
		if( ItemChoose == mcmCancel && ptbCancel )	ptbCancel->UserParam = 1 ;		// Cancel
		quit = (ptbOk?ptbOk->UserParam:0) + (ptbNo?ptbNo->UserParam:0) + (ptbCancel?ptbCancel->UserParam:0) ;
	} 
	while ( ItemChoose!=mcmOk && ItemChoose!=mcmCancel && !quit );

	SetBigMapMenuEnabled( true );	//GREY [15.03.2004]

	ItemChoose = - 1;
	
	UnPress();					//Global
	KeyPressed = LastKey = 0;   //Global

	if( ptbCancel && ptbCancel->UserParam )		return 3 ; // Cancel
	if( ptbOk	  && ptbOk->UserParam )			return 1 ; // Ok
	if( ptbNo	  && ptbNo->UserParam )			return 2 ; // No

	return 1 ; // Ok button
}
//----------------------------------------------------------------------------------
// ------------- Diplomacy Data functions ------------------------------------------ 
//----------------------------------------------------------------------------------
int		CDiplMenu::GetContractCost(int type)
{
	return	m_pDiplData->GetContractCost( type );
}
//----------------------------------------------------------------------------------
int		CDiplMenu::GetConCost() // возвр. m_iConCost
{
	return  m_pDiplData->m_iConCost;
}
//----------------------------------------------------------------------------------
int		CDiplMenu::GetContractsById(int type, int playerId, int* Id)
{
	return	m_pDiplData->GetContractsById( type, playerId, Id );
}
//----------------------------------------------------------------------------------
int		CDiplMenu::GetContractsNumByType( int type, int playerId )
{
	return	m_pDiplData->GetContractsNumByType( type, playerId );
}
//----------------------------------------------------------------------------------
int		CDiplMenu::GetContTypesById(int playerId, int secondId, int* types)
{
	return	m_pDiplData->GetContTypesById( playerId, secondId, types );
}
//----------------------------------------------------------------------------------
int		CDiplMenu::GetIdByContType(int type, int* players)
{
	return	m_pDiplData->GetIdByContType( type, players );
}
//----------------------------------------------------------------------------------
bool	CDiplMenu::CheckContract( int Type, int playerId, int secondId )
{
	return	m_pDiplData->CheckContract( Type, playerId, secondId );
}
//----------------------------------------------------------------------------------
int		CDiplMenu::GetContractPeriod( int type, int playerId, int secondId ) //new
{
	return  m_pDiplData->GetContractPeriod( type, playerId, secondId );
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////
///////////////////////////   CDiplMessage_STRUCT   //////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////
CDiplMessage_STRUCT::CDiplMessage_STRUCT()
{
	return;
}
//----------------------------------------------------------------------------------
CDiplMessage_STRUCT::~CDiplMessage_STRUCT()
{
	return;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////
///////////////////////////   CDiplInfoMessages   ////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////
//CDiplInfoMessages::CDiplInfoMessages(CDiplInfoMessBox* pMessBox,CDiplInfoButtons* pInfoButtons,CDiplomacyData* pDiplData,CDiplMenuInfo* pInfoTexts, CDiplInfoPanel* pInform,int n)
CDiplInfoMessages::CDiplInfoMessages( CDiplInfoMessBox* pMessBox,  CDiplInfoButtons* pInfoButtons,
									  CDiplomacyData*   pDiplData, CDiplMenuInfo*    pInfoTexts,  
									  CDiplInfoPanel*	pInform,   int n )
{
	CAMess.Clear();//new

	m_iActive	= -1;
	m_NextRound = true;

	m_pInform	 = pInform;
	m_pDiplData	 = pDiplData;   
	m_pInfoTexts = pInfoTexts; 

	strcpy(m_chFilePath, "Save\\diplmess.xml"); 

	PrepareBlink();// blink ON
	
	//
	m_NewMessage = false;
}
//----------------------------------------------------------------------------------
CDiplInfoMessages::CDiplInfoMessages()
{
	CAMess.Clear();//new
}
//----------------------------------------------------------------------------------
CDiplInfoMessages::~CDiplInfoMessages()
{
	CAMess.Clear();
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::operator +=(CDiplMenuInfo* pInfoTexts)
{
	if( pInfoTexts ) {
		m_pInfoTexts = pInfoTexts;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::LoadXML()
{
	//loading from XML			/* LOAD SAVED "diplmess.xml" (BigMap Info Messages Save) */
	xmlQuote xml;
	xml.ReadFromFile( m_chFilePath ); 
	ErrorPager Error;
	return	Load( xml, this, &Error );
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::SaveXML()
{
	//saving to XML				/* SAVE "diplmess.xml" (BigMap Info Messages Save) */
	xmlQuote xml2("CDiplInfoMessages");
	Save(xml2,this);
	xml2.WriteToFile( m_chFilePath ); 
	return true;
}
//----------------------------------------------------------------------------------
int		CDiplInfoMessages::GetMesNum()
{
	return	CAMess.GetAmountOfElements();
}
//----------------------------------------------------------------------------------
int		CDiplInfoMessages::GetMessageId( int requestId )
{
	if( requestId < 0 )   return -1;
	for(int i=0; i < GetMesNum() ; i++) 
		if( CAMess[i]->status > 0 && CAMess[i]->reqId == requestId ) { return i; }
		//if ( CAMess[i]->status != -1  && CAMess[i]->reqId == requestId) { return i; }
	return -1;
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::AddMessage(int priority, int MessageType, int PictureId,
									  char* ButtonLetter, char* MessageHead, char* MessageText, bool NewMess )
{
	return	AddMessage( priority,  MessageType, PictureId, ButtonLetter, MessageHead, MessageText, -1 , NewMess );
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::AddMessage(int priority, int MessageType, int PictureId,
									  char* ButtonLetter, char* MessageHead, char* MessageText, int reqId, bool NewMess )
{
	CDiplMessage_STRUCT* DMS = new CDiplMessage_STRUCT;  
	DMS->Letter   = ButtonLetter;
	DMS->MessHead = MessageHead;
	DMS->MessText = MessageText;
	DMS->pictId   = PictureId;
	DMS->MessType = MessageType;
	DMS->reqId	  = reqId;
	DMS->status	  = 1;
	DMS->sortId   = priority;  //MessageType;

	CAMess.Add(DMS); 

	//
	if( NewMess )
		this->m_NewMessage = true;
	//

	return true;

/*	int NextId = m_iMesNum;
	for(int i=0; i < m_iMesNum ; i++ )
		if(m_pMess[i].status == -1) { NextId = i;
									  delete[]m_pMess[i].Letter.pchar();    m_pMess[i].Letter.pchar()   = NULL;
									  delete[]m_pMess[i].MessHead.pchar();	m_pMess[i].MessHead.pchar() = NULL;
									  delete[]m_pMess[i].MessText.pchar();	m_pMess[i].MessText.pchar() = NULL; 
									  break; }

	if (NextId == m_iMesNum) 
	{
		SDiplMessage* arr = m_pMess ; 
		
		m_pMess = new SDiplMessage[m_iMesNum + 1];
		for(int i=0; i < m_iMesNum ; i++) 
		{   
			m_pMess[i].Letter.pchar()   = znew(char,strlen(arr[i].Letter.pchar())+1); 
			strcpy(m_pMess[i].Letter.pchar(),   arr[i].Letter.pchar());

			m_pMess[i].MessHead.pchar() = new char[256]; 
			strcpy(m_pMess[i].MessHead.pchar(), arr[i].MessHead.pchar()); 

			m_pMess[i].MessText.pchar() = znew(char,strlen(arr[i].MessText.pchar())+1); 
			strcpy(m_pMess[i].MessText.pchar(), arr[i].MessText.pchar()); 

			delete[]arr[i].Letter.pchar();	  arr[i].Letter.pchar()   = NULL;
			delete[]arr[i].MessHead.pchar();  arr[i].MessHead.pchar() = NULL;
			delete[]arr[i].MessText.pchar();  arr[i].MessText.pchar() = NULL;

			m_pMess[i].MessType = arr[i].MessType; 
			m_pMess[i].pictId   = arr[i].pictId;
			m_pMess[i].reqId	= arr[i].reqId;
			m_pMess[i].status   = arr[i].status;		  

			//strcpy(m_pMess[i].Letter.pchar(),arr[i].Letter.pchar());//			strcpy(m_pMess[i].MessText,arr[i].MessText);			strcpy(m_pMess[i].MessHead,arr[i].MessHead); 
		}
		delete[]arr;

		m_iMesNum++;
	}
	
	m_pMess[NextId].Letter.pchar()   = znew(char,strlen(ButtonLetter)+1); 
	strcpy(m_pMess[NextId].Letter.pchar(),   ButtonLetter ); 
	
	m_pMess[NextId].MessHead.pchar() = znew(char,strlen(MessageHead)+1); 
	strcpy(m_pMess[NextId].MessHead.pchar(), MessageHead ); 
	
	m_pMess[NextId].MessText.pchar() = znew(char,strlen(MessageText)+1); 
	strcpy(m_pMess[NextId].MessText.pchar(), MessageText ); 

	m_pMess[NextId].pictId   = PictureId;
	m_pMess[NextId].MessType = MessageType;
	m_pMess[NextId].reqId	 = reqId;
	m_pMess[NextId].status	 = 1;

	//m_pMess[NextId].Letter.pchar()   = ButtonLetter;	//m_pMess[NextId].MessText.pchar() = MessageText;	//m_pMess[NextId].MessHead.pchar() = MessageHead;
	//strcpy(m_pMess[NextId].Letter,ButtonLetter);	strcpy(m_pMess[NextId].MessText,MessageText);	strcpy(m_pMess[NextId].MessHead,MessageHead); 
	return true; */
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessages::Process( int Pressed )
{
	SortMessages();
	DrawButtons(); 
	ShowMessage( Pressed );
	
	return;
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessages::SortMessages()
{
	if (!m_NextRound)	{ return; }
	// sortin by "sortId" from 0 to max
	for(int i=0; i < GetMesNum()-1 ; i++) 
	{
		int min = i;
		for(int k=i+1; k < GetMesNum(); k++) 
			if ( CAMess[min]->sortId > CAMess[k]->sortId ) min = k;

		if (min!=i) for(int n=min; n>i; n--)   CAMess.MoveElement(n, -1);
	} 
}
//----------------------------------------------------------------------------------
//void	CDiplInfoMessages::ReGroupMessages()
//{
//	//m_pDiplData->CloseRequests();
//}
//----------------------------------------------------------------------------------
void	CDiplInfoMessages::DrawButtons()
{
	if (!m_NextRound)	{ return; }

	m_pInform->Close();

	for(int i=0; i < GetMesNum() ; i++)
		m_pInform->AddMessage( CAMess[i] , i ) ;

	m_pInform->SetEnabled(true);
	m_pInform->SetVisible(true);

	m_NextRound = false;

	return;
}
//----------------------------------------------------------------------------------
int		CDiplInfoMessages::SetActiveMessage(int active)	//SetActive(int n)
{
	if( m_iActive == active )	{ return m_iActive; } 
	SetPassiveMessage();
	
	if( active == -1 )			{ return m_iActive; }
	m_pInform->SetActive( active ) ; 
	//m_pInfoButtons->SetActive(active); //WORK - old ver.
	return m_iActive = active;
}
//----------------------------------------------------------------------------------
int		CDiplInfoMessages::SetPassiveMessage()	//SetActive(int n)
{
	m_pInform->SetPassive( m_iActive ) ; 
	//m_pInfoButtons->SetPassive(m_iActive); //WORK - old ver.
	return m_iActive = -1;
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::ShowMessageDesk(bool state)
{
	CheckBlink();

	m_pInform->SetEnabled(state);
	m_pInform->SetVisible(state);
	m_pInform->RefreshStats(state);

	return true;

	//m_pInfoButtons->CheckRulonVisible(state); //WORK - old ver.
	//if( !state || m_iActive != -1 ) { 
		//m_pMessBox->SetEnabled(state);  //WORK - old ver.
		//m_pMessBox->SetVisible(state);  //WORK - old ver.
	//}
	//return true;
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessages::ShowMessage(int Pressed)
{
	if (Pressed < 0 || Pressed >= GetMesNum())	{ return; }
	
	SetActiveMessage( Pressed ); 

	//m_pMessBox->SetMessage( CAMess[Pressed], Pressed );   //WORK - old ver.
	//m_pMessBox->Show(); //WORK - old ver.

	//if (Pressed < 0 || Pressed > m_iMesNum)	{ return; }
	////this->m_pMessBox->SetMessage( m_pMess[Pressed].MessHead , m_pMess[Pressed].MessText, Pressed );
	//m_pMessBox->SetMessage( &m_pMess[Pressed], Pressed );
	//m_pMessBox->Show();	
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::ConfirmMessage( int offset ) 
{
	m_iActive = m_pInform->GetParam( offset );
	ConfirmMessage();
	return true;
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::ConfirmMessage()
{
	if( m_iActive == -1 )			{ return false; }
	if( GetMesNum() == 0 )			{ return false; }

	if( CAMess[m_iActive]->reqId >= 0 ) 
	{
		int count = m_pDiplData->ConfirmRequest( CAMess[m_iActive]->reqId );
		if( CAMess[m_iActive]->sortId == m_pDiplData->GetAllyId() )  
		{
			int r1 = GetMessageId( m_pDiplData->GetRequestId( m_pDiplData->GetPactId() , count ) );
			if( r1 >= 0 )  CAMess[ r1 ]->status = -1;		//if (r1>=0) CAMess.DelElement( r1 );

			int r2 = GetMessageId( m_pDiplData->GetRequestId( m_pDiplData->GetPassId() , count ) );
			if( r2 >= 0 )  CAMess[ r2 ]->status = -1;		//if (r2>=0) CAMess.DelElement( r2 );
			
			m_iActive = GetMessageId( m_pDiplData->GetRequestId( m_pDiplData->GetAllyId() , count ) );
		} 
	}

	CAMess[m_iActive]->status = -2 ; //-1 ;

	RefreshDiplMenu(); 
	SetPassiveMessage(); 

	m_pInform->Close();

	m_NextRound = true;
	DrawButtons(); 

	return true;
}
//bool	CDiplInfoMessages::ConfirmMessage()
//{
//	if( m_iActive == -1 )			{ return false; }
//	if( GetMesNum() == 0 )			{ return false; }
//
//	if (CAMess[m_iActive]->reqId != -1) {
//		int count = m_pDiplData->ConfirmRequest( CAMess[m_iActive]->reqId );
//
//		if ( CAMess[m_iActive]->sortId == m_pDiplData->GetAllyId() )
//		{
//			int r1, r2;
//			r1 = GetMessageId( m_pDiplData->GetRequestId( m_pDiplData->GetPactId() , count ) );
//			if (r1>=0) CAMess.DelElement( r1 );
//			r2 = GetMessageId( m_pDiplData->GetRequestId( m_pDiplData->GetPassId() , count ) );
//			if (r2>=0) CAMess.DelElement( r2 );
//			m_iActive = GetMessageId( m_pDiplData->GetRequestId( m_pDiplData->GetAllyId() , count ) );
//		} 
//	}
//
//	CAMess.DelElement(m_iActive);	/*CAMess[m_iActive]->status = -1 ;*/
//	//ReGroupMessages();
//
//	RefreshDiplMenu(); 
//
//	//m_pMessBox->Close(); //WORK - old ver.
//	SetPassiveMessage(); 
//
//	//m_pInfoButtons->CloseButtons(); //WORK - old ver.
//
//	m_pInform->Close();
//
//	m_NextRound = true;
//	DrawButtons(); 
//
//	return true;
//}

/*	if	(m_pMess==NULL)		{ return false; }
	if	(m_iActive==-1)		{ return false; }

	if (m_pMess[m_iActive].reqId != -1) m_pDiplData->ConfirmRequest( m_pMess[m_iActive].reqId );

	m_pMess[ m_iActive ].status = -1 ;
	//ReGroupMessages();

	RefreshDiplMenu(); 

	m_pMessBox->Close();
	SetPassiveMessage();

	m_pInfoButtons->CloseButtons();
	m_NextRound = true;
	DrawButtons(); 
	return true; */

//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::DeleteMessage( int offset ) 
{
	m_iActive = m_pInform->GetParam( offset );
	DeleteMessage();
	return true;
}
//----------------------------------------------------------------------------------

bool	CDiplInfoMessages::DeleteMessage()
{
	if( m_iActive == -1 )		{ return false; }
	if( GetMesNum() == 0 )		{ return false; }

	CAMess[m_iActive]->status = -1 ;

	SetPassiveMessage();

	m_pInform->Close();

	m_NextRound = true;
	DrawButtons(); 

	return true;
}
//bool	CDiplInfoMessages::DeleteMessage()
//{
//	if (m_iActive==-1)		{ return false; }
//	if (GetMesNum()==0)		{ return false; }
//
//	CAMess.DelElement(m_iActive);	/*CAMess[m_iActive]->status = -1 ;*/
//	//ReGroupMessages();
//
//	//m_pMessBox->Close(); //WORK - old ver.
//	SetPassiveMessage();
//
//	//m_pInfoButtons->CloseButtons(); //WORK - old ver.
//
//	m_pInform->Close();
//
//	m_NextRound = true;
//	DrawButtons(); 
//
//	return true;
//}
/*	if	(m_pMess==NULL)		{ return false; }
	if	(m_iActive==-1)		{ return false; }

	m_pMess[m_iActive].status = -1 ;
	//ReGroupMessages();

	m_pMessBox->Close();
	SetPassiveMessage();

	m_pInfoButtons->CloseButtons();
	m_NextRound = true;
	DrawButtons(); 
	return true; */

//----------------------------------------------------------------------------------

bool	CDiplInfoMessages::CloseMessage()
{
	if( m_iActive == -1 )		{ return false; }
	if( GetMesNum() == 0 )		{ return false; }

	CAMess[m_iActive]->status = 0; //-1 ;

	SetPassiveMessage();

	return true;
}
//bool	CDiplInfoMessages::CloseMessage()
//{
//	if	(m_iActive==-1)		{ return false; }
//	if	(GetMesNum()==0)	{ return false; }
//
//	CAMess[m_iActive]->status = 0 ; 
//
//	//m_pMessBox->Close(); //WORK - old ver.
//	SetPassiveMessage();
//
//	return true;
//}
/*	
	if	(m_pMess==NULL)		{ return false; }
	if	(m_iActive==-1)		{ return false; }

	m_pMess[m_iActive].status = 0 ;

	m_pMessBox->Close();
	SetPassiveMessage();
	return true;
*/

//----------------------------------------------------------------------------------

bool	CDiplInfoMessages::DeleteMessage2( bool del , bool accepted , int sid )
{
	if( GetMesNum() == 0 )		{ return false; }

	int   id   = -1;
	bool  quit = false;
	for(int i=0; i < GetMesNum() && !quit ; i++) {
		if( CAMess[i]->sortId == -10 || CAMess[i]->sortId == -11 )  { id = i; }//quest mess
		//if( CAMess[i]->sortId == -11 )  { id = i;  quit = true; }
	}
	if( id == -1 )		{ return false; }

	if( del || CAMess[id]->sortId == -10 ) { //if( del ) { // 
		CAMess.DelElement(id); }//delete message from list
	else {
		if( accepted )  { ConfirmMessage(); }
		else			{ DeleteMessage(); }
	}

	SetPassiveMessage();

	m_NextRound = true;
	DrawButtons(); 

	return true;
}

//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::ClearMessages()
{
	CAMess.Clear();
	SetPassiveMessage();

	m_pInform->Close();

	m_NextRound = true;

	if( GetMesNum() == 0 )	{ return true; }
	return false;
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessages::ScrollInformation()
{
	m_pInform->ScrollDesk( m_iActive );
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::RefreshDiplMenu()
{
	if( m_pInfoTexts )  m_pInfoTexts->SetCurrSecId( -1 );
	return true;
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessages::CheckBlink()
{
	//if( m_pInform->m_Visible )  m_Visited = true;
	if( m_pInform->m_Visible )  { m_Visited = true; m_NewMessage = false; }

	int  BLINK_TIME_VALUE = 400 ; //500 ;
	//if( GetMesNum() > 1 && !m_Visited )
	if( GetMesNum() > 1 && !m_Visited && m_NewMessage )
	{
		GP_TextButton* a_pgptb = (GP_TextButton*) m_pInform->m_pMenu->Find( 0x40 + 4 ); // page "Messages" ID 
		if( !a_pgptb )	return false;
		
		int  bt = GetTickCount();
		if( m_iBlinkTime == 0 || bt > m_iBlinkTime + BLINK_TIME_VALUE ) //100 )  
		{
			m_iBlinkTime = bt ;
			if( !m_Blink )   { a_pgptb->ActiveFont = a_pgptb->PassiveFont = &RedFont;  m_Blink = true;  }
			else		     { a_pgptb->ActiveFont = a_pgptb->PassiveFont = &GrayFont; m_Blink = false; }
		}
	}
	return  m_Visited;
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessages::PrepareBlink()
{
	m_iBlinkTime = 0;
	m_Blink		 = false;
	m_Visited	 = false;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////
///////////////////////////   CDiplInfoButtons   /////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////
CDiplInfoButtons::CDiplInfoButtons(DialogsDesk* pDesk, GPPicture* pRulon, int ButOffset) //(DialogsDesk* pDesk,int ButOffset)
{
	m_pDesk	 = pDesk;
	m_pRulon = pRulon;
	
	m_iButOffset = ButOffset;

	m_iX0 = 10;
	m_iY0 = 10;

	m_iN    = 0;		// N;
	m_iFree = 0;		// N;
	
	m_Visible = false;
	m_Enabled = false;
}
//----------------------------------------------------------------------------------
int		CDiplInfoButtons::AddButton()
{
	LocalGP			a_lgpButtonOverMap("Interf3\\TotalWarGraph\\dOverMap"); //NEED NEW BUTTON?
	GP_TextButton*	pGPTB;

	pGPTB = m_pDesk->addGP_TextButton(NULL, m_iX0, m_iY0 /*40*m_iN*/,"i",a_lgpButtonOverMap.GPID, 1 ,
									  &SpecialRedFont,&SpecialBlackFont);
	pGPTB->AssignID( m_iButOffset + m_iN );
	//pGPTB->Sprite  = 1;
	pGPTB->Sprite1 = 0;	
	//pGPTB->x  = 0 ;	//pGPTB->y  = 40*i + 10;	//pGPTB->x1 = 50 ; // 50 ;	//pGPTB->y1 = 40*i + 10 + 27 ; 

	pGPTB->UserParam = m_iN ;	
	//pGPTB->OnUserClick = &InfoDesksButtonPressed ;

	m_iN++;
	m_iFree = m_iN - 1;

	return m_iN;
}
//----------------------------------------------------------------------------------
void	CDiplInfoButtons::AddMessageButton( int n, char* mess ) 	
{
	if (m_iFree >= m_iN)	{ AddButton(); }

	GP_TextButton*	a_ptb;
	a_ptb = reinterpret_cast<GP_TextButton*>(m_pDesk->Find(m_iButOffset + m_iFree )); 
	if (a_ptb==0x00000000) { return; }
	a_ptb->x  = m_iX0 ;							// 0 ;
	a_ptb->y  = m_iY0 + 40*m_iFree ;			// 40*n ;
	a_ptb->x1 = m_iX0 + 50 ;					// 50 ;	
	a_ptb->y1 = m_iY0 + 40*m_iFree + 27 ;		//40*n + 27 ; 
	a_ptb->UserParam = n;
	//strcpy(a_ptb->Message,mess);
	//a_ptb->Message = mess;
	BigMapSetMessage( a_ptb , mess ) ;

	a_ptb->Visible = a_ptb->Enabled = true;

	m_iFree++;
}
//----------------------------------------------------------------------------------
void	CDiplInfoButtons::CloseButtons()
{
	GP_TextButton*	a_ptb;
	for(int i=0; i < m_iN ; i++)
	{
		a_ptb = reinterpret_cast<GP_TextButton*>(m_pDesk->Find(m_iButOffset + i));
		if (a_ptb==0x00000000) { continue; }
		a_ptb->x  = m_iX0 ;			// 0 ;
		a_ptb->y  = m_iY0 ;			// 0 ;
		a_ptb->UserParam = -1 ;
		//a_ptb->Message   = NULL ;
	}
	SetEnabled(false);
	SetVisible(false);

	m_iFree = 0;
}
//----------------------------------------------------------------------------------
void	CDiplInfoButtons::SetEnabled(bool state)
{
	if (m_Enabled == state)   { return; }
	m_pDesk->Enabled = state;
	GP_TextButton*	a_ptb;
	for(int i=0; i < m_iN ; i++)
	{
		a_ptb = reinterpret_cast<GP_TextButton*>(m_pDesk->Find(m_iButOffset + i));
		if (a_ptb==0x00000000)					{ continue; }
		if (state && a_ptb->UserParam == -1)	{ continue; }
		a_ptb->Enabled = state;
	}
	m_Enabled = state;
}
//----------------------------------------------------------------------------------
void	CDiplInfoButtons::SetVisible(bool state)
{
	if (m_Visible == state)   { return; }
	m_pDesk->Visible = state;
	GP_TextButton*	a_ptb;
	for(int i=0; i < m_iN ; i++)
	{
		a_ptb = reinterpret_cast<GP_TextButton*>(m_pDesk->Find(m_iButOffset + i));
		if (a_ptb==0x00000000)					{ continue; }
		if (state && a_ptb->UserParam == -1)	{ continue; }
		a_ptb->Visible = state;
	}
	m_Visible = state;
}
//----------------------------------------------------------------------------------
void	CDiplInfoButtons::SetActive(int n)
{
	if(n<0)  return;
	GP_TextButton*	a_ptb;
	for(int i=0; i<m_iN ; i++) {
		a_ptb = (GP_TextButton*) m_pDesk->Find(m_iButOffset + i);
		if (a_ptb==0x00000000)			{ continue; }
		if (a_ptb->UserParam == n )		{ break;	}
	}
	a_ptb->Diffuse  = GREEN_SECT_COLOR & 0x00FFFFFF;//very new //presssed   //0xFF000000 + GREEN_SECT_COLOR ;					
	a_ptb->Diffuse += 0xFF000000;//very new
}
//----------------------------------------------------------------------------------
void	CDiplInfoButtons::SetPassive(int n)
{
	if(n<0)  return;
	GP_TextButton*	a_ptb;
	for(int i=0; i<m_iN ; i++) {
		a_ptb = (GP_TextButton*) m_pDesk->Find(m_iButOffset + i);
		if (a_ptb==0x00000000)			{ continue; }
		if (a_ptb->UserParam == n )		{ break;	}
	}
	a_ptb->Diffuse = -1;								//unpressed
}
//----------------------------------------------------------------------------------
bool	CDiplInfoButtons::CheckRulonVisible(bool state) 
{
	if (state  && m_pRulon->SpriteID != 1)    { m_pRulon->SpriteID = 1; }
	if (!state && m_pRulon->SpriteID != 0)    { m_pRulon->SpriteID = 0; }

	if (m_iFree == 0 &&  m_pRulon->Visible)   { m_pRulon->Visible = m_pRulon->Enabled = false; }
	if (m_iFree != 0 && !m_pRulon->Visible)   { m_pRulon->Visible = m_pRulon->Enabled = true;  }

	return m_pRulon->Visible;
}
//----------------------------------------------------------------------------------
void	CDiplInfoButtons::SetDeskVisibled()
{
	//if(!m_pDesk->Visible) OnRulonClick(reinterpret_cast<SimpleDialog*>(m_pRulon));
}
//----------------------------------------------------------------------------------
int		CDiplInfoButtons::GetButOffset()
{
	return	m_iButOffset;
}
//----------------------------------------------------------------------------------
int		CDiplInfoButtons::GetNumber()
{
	return	m_iN;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//**********************************************************************************************************



////////////////////////////////////////////////////////////////////////////////////
///////////////////////////   CDiplInfoPanel   /////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////
CDiplInfoPanel::CDiplInfoPanel( DialogsSystem* pMenu, 
							    int Offset )
{
	m_pMenu   = pMenu;
	m_iOffset = Offset;

	Init();
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::Init()
{
	m_iX0		= 0;
	m_iY0		= 0;
	m_iStepY	= 0;		

	m_iN		= 0;
	m_iFree		= 0;

	m_iMessElem = 8;

	//desk для размещ. на закладке
	m_pMainDesk = m_pMenu->AddDialogsDesk( 72 , 110 , 900, 658 ,"NullBorder");

	//desk для размещ.инф.сообщ.
	m_pDesk		= m_pMainDesk->AddDialogsDesk(0, 0, 612 , 537 ,"EmptyBorder");

	//
	int HIDE_DY = 1000;
	//
	LocalGP  a_lgpMess("Interf3\\TotalWarGraph\\dActions");
	LocalGP  a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");
	LocalGP  a_lgpElem2("Interf3\\TotalWarGraph\\bmElements2");
	m_pgptbStatx = m_pMainDesk->addGP_TextButton(NULL, 635 , HIDE_DY /*290*/  /*625,540*/ ,GetTextByID("#CDB_Statistics"),a_lgpMess.GPID,0,&RedFont,&OrangeFont);
	m_pgptbStatx->Sprite		=  1; //active
	m_pgptbStatx->Sprite1		=  0; //passive
	m_pgptbStatx->FontDy		= -2;
	m_pgptbStatx->UserParam		=  0; 
	m_pgptbStatx->OnUserClick	= &Draw_EW2_CampaignStats;//&DiplActionPressed;
	_str   a_Hint;		
	a_Hint += GetTextByID("#CHINT_Statistics");		DYNHINT_RA(m_pgptbStatx, a_Hint.pchar());

	m_pgptbStatx->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
	m_pgptbStatx->AssignSound( "INTERF_NAGAL", CLICK_SOUND );	

	// flags/name/score/sectNum
	RLCFont*  font = &SmallGrayFont1; //&SpecialGrayFont;
	m_pgppRow  = m_pMainDesk->addGPPicture(NULL,  0, HIDE_DY /*0*/, a_lgpElem2.GPID , 2 ); 
	m_ptbName  = m_pMainDesk->addTextButton(NULL, 0, HIDE_DY ,GetTextByID("#CDT_Country"),font,font,font, 1 );
	m_ptbSecN  = m_pMainDesk->addTextButton(NULL, 0, HIDE_DY ,GetTextByID("#CDT_StatSecNum"),font,font,font, 1 );
	m_ptbScore = m_pMainDesk->addTextButton(NULL, 0, HIDE_DY ,GetTextByID("#CDT_StatScore"),font,font,font, 1 );

	// bmElements2
	for(int i=0; i < MAX_PLAYERS ; i++) {
		m_pRow[i]	= m_pMainDesk->addGPPicture(NULL,  0, HIDE_DY ,a_lgpElem2.GPID , 5 ); 
		m_pFlag[i]  = m_pMainDesk->addGPPicture(NULL,  0, HIDE_DY ,a_lgpFlags.GPID , 0 ); 
		m_pName[i]  = m_pMainDesk->addTextButton(NULL, 0, HIDE_DY ," ",&BlackFont,&BlackFont,&BlackFont, 1 );
		m_pScore[i] = m_pMainDesk->addTextButton(NULL, 0, HIDE_DY ," ",&BlackFont,&BlackFont,&BlackFont, 1 );
		m_pSecN[i]  = m_pMainDesk->addTextButton(NULL, 0, HIDE_DY ," ",&BlackFont,&BlackFont,&BlackFont, 1 );
	}

	// разделитель
	LocalGP		a_lgpElems("Interf3\\TotalWarGraph\\bmElements");
	m_pDeLimiter = m_pMainDesk->addGPPicture(NULL, 638, HIDE_DY /*260*/ /*625, 250*/ ,a_lgpElems.GPID, 30 );

	m_Visible = true; 
	m_Enabled = true;
	
	////
	//this->RefreshStats( true );
	//ProcessMessages();
	//m_pMenu->ProcessDialogs();	
	//StdKeys();
	//m_pMenu->RefreshView();
	////

	SetEnabled(false);
	SetVisible(false);
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::SetInfoMessages(CDiplInfoMessages* pMs)
{
	m_iInfoMessages	= reinterpret_cast<int>( pMs );
}
//----------------------------------------------------------------------------------
int		CDiplInfoPanel::Add()
{
	LocalGP		a_lgpMess("Interf3\\TotalWarGraph\\dActions");
	LocalGP		a_lgpPict("Interf3\\TotalWarGraph\\dMesspic_s");

	// Message Box													// Set coords ONLY for:   x & x1
	GPPicture*	pgppBox   = m_pDesk->addGPPicture(NULL, m_iX0 , 0 ,a_lgpMess.GPID, 6);
	pgppBox->AssignID( m_iOffset + m_iN );			
	pgppBox->OnUserClick = &InformationMessagePressed ;
	// Message Picture 
	GPPicture*	pgppPic   = m_pDesk->addGPPicture(NULL, m_iX0 +12 , 0 ,a_lgpPict.GPID, 0 );
	pgppPic->AssignID( m_iOffset + m_iN + 5 );
	// Picture border
	GPPicture*	pgppFrame = m_pDesk->addGPPicture(NULL, pgppPic->x -4 , 0 ,a_lgpMess.GPID, 8);
	pgppFrame->AssignID( m_iOffset + m_iN + 6 );

	// Message head
	TextButton*	 ptbHead = m_pDesk->addTextButton(NULL, m_iX0 +15 , 0 ," ",&RedFont,&RedFont,&RedFont,0);
	ptbHead->AssignID( m_iOffset + m_iN + 4 ); 
	// Message text
	int mDX = 11 ,   mDY = 4 ,   mMaxH = 98 - 2*mDY ;		
	TextButton*  ptbText = m_pDesk->addTextButton( NULL, 0 , 0 ," ", &BlackFont,&BlackFont,&BlackFont, 0); //&fonMenuText[0],&fonMenuText[0],&fonMenuText[0],0);//NEED NEW FONTS? // &BlackFont
	ptbText->x			= pgppFrame->x1 + mDX ;
	ptbText->MaxWidth	= pgppBox->x1 - ptbText->x - mDX ;
	ptbText->AssignID( m_iOffset + m_iN + 1 );

	//Buttons Yes/No
	Enumerator* BUTTON_ENUM = ENUM.Get("BUTTON_ENUM");
	GP_TextButton*  ptbYes = m_pDesk->addGP_TextButton( NULL, m_iX0 + 317 , 0 ,
													    BUTTON_ENUM->Get( 6 ),a_lgpMess.GPID,3,&RedFont,&OrangeFont);
	ptbYes->AssignID( m_iOffset + m_iN + 2 );
	ptbYes->Sprite		=  4 ; //active
	ptbYes->Sprite1		=  3 ; //passive
	ptbYes->Align       =  1 ;
	ptbYes->FontDy      = -2; 
	ptbYes->OnUserClick = &InformationYesClick;

	ptbYes->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
	ptbYes->AssignSound( "INTERF_PERO",  CLICK_SOUND );

	GP_TextButton*  ptbNo = m_pDesk->addGP_TextButton( NULL, ptbYes->x1 +10 /*+1*/ , 0 ,
													   BUTTON_ENUM->Get(1),a_lgpMess.GPID,3,&RedFont,&OrangeFont);
	ptbNo->AssignID( m_iOffset + m_iN + 3 );
	ptbNo->Sprite		=  4 ; //active
	ptbNo->Sprite1		=  3 ; //passive
	ptbNo->Align		=  1 ;
	ptbNo->FontDy		= -2;  
	ptbNo->OnUserClick  = &InformationNoClick;

	ptbNo->AssignSound( "INTERF_NAVEL", MOUSE_SOUND );
	ptbNo->AssignSound( "INTERF_BREAK", CLICK_SOUND );

	// Accept/Reject text
	TextButton*  ptbDone = m_pDesk->addTextButton(NULL, ptbNo->x , 0 ," ",&RedFont,&RedFont,&RedFont,0);
	ptbDone->AssignID( m_iOffset + m_iN + 7 ); 

	m_iFree  = m_iN;
	m_iN	+= m_iMessElem;

	return  m_iN;
}
//----------------------------------------------------------------------------------
int		CDiplInfoPanel::GetParam( int offset )
{
	TextButton*	 a_ptb = reinterpret_cast<TextButton*>(m_pDesk->Find( offset - 2 )); //message head
	if( !a_ptb )   { return  -1; }
	return  a_ptb->UserParam;
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::AddMessage( CDiplMessage_STRUCT* pMess, int n )
{
	if(m_iFree >= m_iN)   Add();

	// Message Box
	SimpleDialog*	pgppBox = reinterpret_cast<SimpleDialog*>( m_pDesk->Find( m_iOffset + m_iFree ) );
	if( pgppBox ){
		pgppBox->UserParam	 = n ;
		pgppBox->y			 = m_iY0 + m_iStepY ;
		pgppBox->y1			+= pgppBox->y ;			
	}
	// Message Picture
	GPPicture*		pgppPic = reinterpret_cast<GPPicture*>( m_pDesk->Find( m_iOffset + m_iFree + 5 ) );
	if( pgppPic && pgppBox ) {
		pgppPic->UserParam	 = n ;
		pgppPic->SpriteID	 = pMess->pictId ;
		pgppPic->y			 = pgppBox->y + 40 ;
		pgppPic->y1			+= pgppPic->y ;			
	}
	// Picture border
	SimpleDialog*	pgppFrame = reinterpret_cast<SimpleDialog*>( m_pDesk->Find( m_iOffset + m_iFree + 6 ) );
	if( pgppFrame && pgppPic ) {
		pgppFrame->UserParam = n ;
		pgppFrame->y		 = pgppPic->y  -4 ;
		pgppFrame->y1		 = pgppPic->y1 -4 ;		
	}
	// message head
	TextButton*			a_ptb = reinterpret_cast<TextButton*>( m_pDesk->Find( m_iOffset + m_iFree  + 4 ) );
	if( a_ptb && pgppBox ) {
		a_ptb->UserParam	 = n ;
		a_ptb->y			 = pgppBox->y + 9 ; 	
		a_ptb->y1			+= a_ptb->y ;	
		if( pMess->MessHead.pchar() )  a_ptb->SetMessage( pMess->MessHead.pchar() ) ;
	}	
	// message text   
	TextButton*			a_pTV = reinterpret_cast<TextButton*>( m_pDesk->Find( m_iOffset + m_iFree + 1 ) );
	if( a_pTV && pgppBox ) { 
		a_pTV->UserParam	 = n ;
		a_pTV->y			 = pgppBox->y + 35 ; 
		a_pTV->y1			+= a_pTV->y ;
		if( pMess->MessText.pchar() )  a_pTV->SetMessage( pMess->MessText.pchar() ) ;
	}
	// Button YES
	GP_TextButton*		a_pYES = reinterpret_cast<GP_TextButton*>( m_pDesk->Find( m_iOffset + m_iFree + 2 ) );
	if( a_pYES && pgppBox ) { 
		int h = a_pYES->y1 - a_pYES->y ;
		a_pYES->y			= pgppBox->y1 - h - 3 ; //very new	//pgppBox->y1 - h - 1 ;	
		a_pYES->y1			= a_pYES->y + h  ;
		a_pYES->UserParam	= m_iInfoMessages ;
	}
	// Button NO
	GP_TextButton*		a_pNO = reinterpret_cast<GP_TextButton*>( m_pDesk->Find( m_iOffset + m_iFree + 3 ) );
	if( a_pNO && a_pYES ) { 
		a_pNO->y			= a_pYES->y ;	
		a_pNO->y1			= a_pYES->y1 ;
		a_pNO->UserParam	= m_iInfoMessages ;
	}
	// Accept/Reject text
	TextButton*			a_pDone = reinterpret_cast<TextButton*>( m_pDesk->Find( m_iOffset + m_iFree + 7 ) );
	if( a_pDone && a_pNO ) { 
		a_pDone->UserParam	= -1 ;
		int  dy = 7; //(a_pNO->y1 - a_pNO->y)/4;
		a_pDone->y			 = a_pNO->y  + dy ;
		a_pDone->y1			+= a_pDone->y;
	}

	// correction data
	if( pMess->MessType == 0 || pMess->status < 0 ) {							//pMess->status == -1 ) { 
		if( a_pYES && a_pNO )  a_pYES->UserParam = a_pNO->UserParam = -100 ; // OK-NO - inVisible
		if( a_pDone && pMess->MessType != 0 ) {
			a_pDone->UserParam = n ; // set Visible Done text
			_str  a_txt;
			if( pMess->status == -2 )  a_txt += GetTextByID("#CDT_Accepted");
			if( pMess->status == -1 )  a_txt += GetTextByID("#CDT_Rejected");
			a_pDone->SetMessage( a_txt.pchar() );
		}
	}

	m_iStepY = pgppBox->y1 + 4 ;									//6 ;	//5 ;	//4 ;
	m_iFree += m_iMessElem ;
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::Close()
{
	for(int i=0 ; i < m_iN ; i++ ) {
		SimpleDialog*  a_ptb = reinterpret_cast<SimpleDialog*>(m_pDesk->Find( m_iOffset + i ));
		if( !a_ptb )	{ continue; }
		int h = a_ptb->y1 - a_ptb->y;
		a_ptb->y  = 0 ;												//m_iY0 ;		
		a_ptb->y1 = h ;												//a_ptb->y + h ;
		a_ptb->UserParam = -1 ;										//a_ptb->Message   = NULL ; 
	}	
	SetEnabled(false);
	SetVisible(false);
	
	m_iFree  = 0;
	m_iStepY = 0;
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::SetEnabled(bool state)
{
	m_pDesk->Enabled = state;
	if( m_Enabled == state )		{ return; }
	m_pMainDesk->Enabled = state;
	m_pDeLimiter->Enabled = state;
	m_pgptbStatx->Enabled = state;
	//m_pDesk->Enabled = state;
	for(int i=0 ; i < m_iN ; i++ ) {
		SimpleDialog* a_ptb = reinterpret_cast<SimpleDialog*>(m_pDesk->Find(m_iOffset + i));
		if( !a_ptb )				{ continue; }
		if( a_ptb->UserParam < 0 )	{ a_ptb->Enabled = false; continue; }

		a_ptb->Enabled = state;
	}
	m_Enabled = state;
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::SetVisible(bool state)
{
	if( m_Visible == state )		{ return; }
	//m_pDesk->Visible = state;
	m_pMainDesk->Visible = state;
	for(int i=0 ; i < m_iN ; i++ ) {
		SimpleDialog* a_ptb = reinterpret_cast<SimpleDialog*>(m_pDesk->Find(m_iOffset + i));
		if( !a_ptb )				{ continue; }
		if( a_ptb->UserParam < 0 )	{ a_ptb->Visible = false; continue; }

		a_ptb->Visible = state;
	}
	m_Visible = state;
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::SetActive(int n)
{
	if( n < 0 )			return;
	if( m_iN <= 0 )		return;

	GPPicture*	a_ptb;
	for(int i=0; i < m_iN ; i += m_iMessElem ) {
		a_ptb = (GPPicture*) m_pDesk->Find(m_iOffset + i);
		if( !a_ptb )    { continue; }
		if( a_ptb->UserParam == n )   { a_ptb->SpriteID = 7;  break; } //presssed	
	}
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::SetPassive(int n)
{
	if( n < 0 )			return;

	GPPicture*	a_ptb;
	for(int i=0; i < m_iN ; i += m_iMessElem ) {
		a_ptb = (GPPicture*) m_pDesk->Find(m_iOffset + i);
		if( !a_ptb)   { continue; }
		a_ptb->SpriteID = 6 ;										//unpressed		
	}
}
//----------------------------------------------------------------------------------
void	CDiplInfoPanel::ScrollDesk(int pos)
{
	if( pos >= m_iFree )					return;
	//new it!
	if( !m_pDesk || !m_pDesk->VScroller )	return;
	//new it!
	
	if( pos < 5 )	{ m_pDesk->VScroller->SPos = 0; return; } //if( pos == 0 ) { m_pDesk->VScroller->SPos = 0; return; }

	SimpleDialog* a_ptb = (SimpleDialog*) m_pDesk->Find(m_iOffset + pos * m_iMessElem - 1 );
	if( !a_ptb )	return;

	int newSPos = a_ptb->y1 /*+ 30*/ ;
	if( newSPos > m_iStepY - m_pDesk->y1 + m_pDesk->y )	{ newSPos = m_iStepY - m_pDesk->y1 + m_pDesk->y /*- 30*/; } //- 40; }//{ return; }
	m_pDesk->VScroller->SPos = newSPos ;
	//int newSPos = pos * m_iStepY*m_iMessElem/m_iFree; //n*( m_iStepY/( (m_iFree+1)/m_iMessElem ) )
}
//----------------------------------------------------------------------------------

bool	CDiplInfoPanel::RefreshStats(bool state)
{
	int  scr[MAX_PLAYERS]  = { 0, 0, 0, 0, 0, 0 }; // { -1, -1, -1, -1, -1, -1 }; 
	if( state ) 	
	{
		for(int i=0; i < MAX_PLAYERS; i++) 
			if( ROOT_SCENARIO.m_pPlayer[i] && ROOT_SCENARIO.m_pPlayer[i]->m_Alive && ROOT_SCENARIO.m_pSect ) {
				scr[i] = 0;
				if( ROOT_SCENARIO.m_inCurTurn > 0 ) {
					for(int sec=0; sec < ROOT_SCENARIO.m_pSect->GetSectorsNum() ; sec++) 
						if( ROOT_SCENARIO.m_pSect->GetSecOwner(sec) == i ) {
							scr[i] += (ROOT_SCENARIO.m_pSect->GetSecDefence(sec) + 1);			 // +defence +SecNum
							scr[i] += SEC_DEF_POWER( ROOT_SCENARIO.m_pSect->GetSecDefence(sec) );// +SecDefPower
						}
					for(int h=0; h < ROOT_SCENARIO.m_pPlayer[i]->m_inHeroNum ; h++) {							
						CHero*	pH = &(ROOT_SCENARIO.m_pPlayer[i]->m_phrHero[h]) ; //Массив героев
						if( pH->m_Alive )
							scr[i] += pH->GetARMY_Power(true);									// +army power
					}
				}
			}

		// flags/name/score/sectNum
		_str  text;		
		int   x0 = 620,  y0 = 5,  dx = 9 ,  dy = 15 ; 
		int	 ndx = 3*dx,  sdx = 14*dx,  cdx = 19*dx;

		m_pgppRow->x  = x0;				m_pgppRow->y  = y0 ; //new

		int  rowStartSpr = 4 , rowEndSpr = 6 ;			
		int  rowSprID = rowStartSpr ; //new

		y0 = m_pgppRow->y1 + 1;	//new 
		for(int i=0; i < MAX_PLAYERS; i++) 
		{	
			int MaxPID = i;
			for(int pid=0; pid < MAX_PLAYERS; pid++) 
				if( scr[ pid ] > scr[ MaxPID ] )
					MaxPID = pid;

			if( MaxPID >= 0 && MaxPID < MAX_PLAYERS && scr[ MaxPID ] >= 0 ) 
			{
				m_pRow[i]->x = x0;									
				m_pRow[i]->y = y0;								
				m_pRow[i]->SpriteID = rowSprID;

				m_pFlag[i]->x = x0 +15 ;
				m_pFlag[i]->y = y0 +6  ;
				m_pFlag[i]->SpriteID = MaxPID+1 ;
				
				y0 = m_pRow[i]->y + 11;	

				text.DString::Clear();				
				text = " ";
				if( ROOT_SCENARIO.m_pPlayer[ MaxPID ] )  text = ROOT_SCENARIO.m_pPlayer[ MaxPID ]->GetPlayerName();
				setCoordSD( m_pName[i],  m_pFlag[i]->x1 + ndx , y0 );
				m_pName[i]->SetMessage( text.pchar() );
				//m_pName[i]->SetMessage( ROOT_SCENARIO.m_pPlayer[ MaxPID ]->GetPlayerName() );

				text.DString::Clear();				
				text = " ";
				if( ROOT_SCENARIO.m_pSect )  text = ROOT_SCENARIO.m_pSect->GetPlayerSect( MaxPID );
				setCoordSD( m_pSecN[i],  m_pFlag[i]->x1 + sdx , y0 );
				m_pSecN[i]->SetMessage( text.pchar() );

				text.DString::Clear();
				text = scr[ MaxPID ];
				setCoordSD( m_pScore[i], m_pFlag[i]->x1 + cdx , y0 );
				m_pScore[i]->SetMessage( text.pchar() );
				
				scr[ MaxPID ] = -1;
				y0 = m_pRow[i]->y1 + 1;

				rowSprID++;  
				if( rowSprID > rowEndSpr )  rowSprID = rowStartSpr;
				if( i == MAX_PLAYERS - 1 )  
					m_pRow[i]->SpriteID = 3;
			} 
			else {	
				if( i > 0 )  
					m_pRow[i-1]->SpriteID = 3;
				m_pRow[i]->Visible=m_pFlag[i]->Visible=m_pName[i]->Visible=m_pScore[i]->Visible=m_pSecN[i]->Visible=false;
			}
		}
			
		int  stDy = 16;
		m_pDeLimiter->y = y0 +  stDy; 
		m_pgptbStatx->y = y0 + (2.5)*stDy;

		x0 = m_pFlag[0]->x1;			
		y0 = m_pgppRow->y -3 ; 
		m_ptbName->x  = x0 + ndx;		m_ptbName->y  = y0 +dy;		m_ptbName->x  +=  3;
		m_ptbSecN->x  = x0 + sdx;		m_ptbSecN->y  = y0 +dy;		m_ptbSecN->x  -= 11;
		m_ptbScore->x = x0 + cdx;		m_ptbScore->y = y0 +dy;		m_ptbScore->x -=  3;

		return true;
	}
	return  false;
}

//----------------------------------------------------------------------------------
int		CDiplInfoPanel::GetOffset()
{
	return	m_iOffset;
}
//----------------------------------------------------------------------------------
int		CDiplInfoPanel::GetNumber()
{
	return	m_iN;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------





//**********************************************************************************************************

////////////////////////////////////////////////////////////////////////////////////
///////////////////////////   CDiplInfoMessBox   /////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////
CDiplInfoMessBox::CDiplInfoMessBox(DialogsSystem* pMenu,int BoxOffset)
{
	m_pMenu			= pMenu;
	m_pgppBox		= NULL;
	m_pgppPic		= NULL;
	m_ptbHead		= NULL;
	m_ptvText		= NULL;	//m_ptbText		= NULL;	//m_ptbText2		= NULL;//test
	m_pgptbYes		= NULL;
	m_pgptbNo		= NULL;
	m_pgptbClose	= NULL;
	
	m_iBoxOffset	= BoxOffset;
	m_iN = 0 ;

	m_iX0 = 358;	//370 ;						// 500; // 457 ;
	m_iY0 = 150 ;								// 150; // 180 ;
	m_iCurrMess = -1;
	m_iMessType = 0 ; 
	
	m_Visible = false;
	m_Enabled = false;

	//Enumerator* BUTTON_ENUM = ENUM.Get("Buttons");
	Enumerator* BUTTON_ENUM = ENUM.Get("BUTTON_ENUM");
	m_chOK  = BUTTON_ENUM->Get(2);				//m_chOK  = GetTextByID("#CDB_OkButton");
	m_chYes = BUTTON_ENUM->Get(6);//(3);				//m_chYes = GetTextByID("#CDB_Agree");

	Init();
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessBox::Init()
{
	//Message box
	LocalGP		a_lgpMess("Interf3\\TotalWarGraph\\dMessage");				
	m_pgppBox = m_pMenu->addGPPicture(NULL, m_iX0 , m_iY0 ,a_lgpMess.GPID, 3 );						        
	m_pgppBox->AssignID(m_iBoxOffset + m_iN++);
	m_pgppBox->Enabled = m_pgppBox->Visible = false; 
	//Message box picture
	LocalGP		a_lgpPict("Interf3\\TotalWarGraph\\dMesspic");		
	m_pgppPic = m_pMenu->addGPPicture(NULL, m_iX0 + 66 , m_iY0 + 76 ,a_lgpPict.GPID, 0 ); // x0=66 y0=76     
	m_pgppPic->AssignID(m_iBoxOffset + m_iN++); 
	m_pgppPic->Enabled = m_pgppPic->Visible = false; 
	//Message head
	m_ptbHead = m_pMenu->addTextButton(NULL, m_iX0+304,m_iY0+31, "" ,&YellowFont,&YellowFont,&YellowFont,1);
	m_ptbHead->AssignID(m_iBoxOffset + m_iN++);
	m_ptbHead->Visible = false; 
	
	//Message text						  /* 300 , 371 , 420 , 110 */
	m_ptvText = m_pMenu->addTextViewer(NULL, m_iX0+90 , m_iY0+221/*235/*250*/ , 420 , 110 /*70*/ ,"tmp.txt",&BlackFont); //x=65 Lx=475
	m_ptvText->SymSize  = 25; //27;
	m_ptvText->UserParam = m_ptvText->y;
	m_ptvText->AssignID(m_iBoxOffset + m_iN++);
	m_ptvText->Enabled = m_ptvText->Visible = false; 

	//m_ptbText = m_pMenu->addTextButton(NULL, m_iX0+304,m_iY0+220 /*250*/ , "" ,&BlackFont,&BlackFont,&BlackFont,1);
	//m_ptbText->AssignID(m_iBoxOffset + m_iN++);
	//m_ptbText->Visible = false; 
	//m_ptbText2 = m_pMenu->addTextButton(NULL, m_iX0+304,m_iY0+275, "" ,&BlackFont,&BlackFont,&BlackFont,1);//temp
	//m_ptbText2->AssignID(m_iBoxOffset + m_iN++);//temp
	//m_ptbText2->Visible = false; //temp
	////GMM.addTextViewer(NULL,TX0,TY0,TX1-SBW-5-TX0,TY1-TY0,"tmp.txt",FText);
	
	//Buttons Yes/No/Cancel
	m_pgptbYes = m_pMenu->addGP_TextButton(NULL,m_iX0+76,m_iY0+335, m_chYes.pchar() ,a_lgpMess.GPID,7,&WhiteFont,&YellowFont);
	m_pgptbYes->AssignID(m_iBoxOffset + m_iN++);
	m_pgptbYes->Align   = 1;
	m_pgptbYes->FontDy  = -2; 
	m_pgptbYes->Visible = m_pgptbYes->Enabled = false;
	
	//Enumerator* BUTTON_ENUM = ENUM.Get("Buttons");
	Enumerator* BUTTON_ENUM = ENUM.Get("BUTTON_ENUM");
	m_pgptbNo = m_pMenu->addGP_TextButton(NULL,m_iX0+231,m_iY0+335,BUTTON_ENUM->Get(1),a_lgpMess.GPID,7,&WhiteFont,&YellowFont);/*GetTextByID("#CDB_NoButton")*/
	m_pgptbNo->AssignID(m_iBoxOffset + m_iN++);
	m_pgptbNo->Align   = 1;
	m_pgptbNo->FontDy  = -2; 
	m_pgptbNo->Visible = m_pgptbNo->Enabled = false; 

	m_pgptbClose = m_pMenu->addGP_TextButton(NULL,m_iX0+399,m_iY0+335,BUTTON_ENUM->Get(5),a_lgpMess.GPID,7,&WhiteFont,&YellowFont);/*GetTextByID("#CDB_Close")*/
	m_pgptbClose->AssignID(m_iBoxOffset + m_iN++);
	m_pgptbClose->Align   = 1;
	m_pgptbClose->FontDy  = -2; 
	m_pgptbClose->Visible = m_pgptbClose->Enabled = false; 
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessBox::PrepareMessBox()
{
	if( m_iMessType == 3 ) {
		m_pgppBox->SpriteID = 3 ; // 0 ;
		
		m_pgptbYes->Sprite = 7;						// 6;
		m_pgptbYes->x  = m_iX0 + 63 ;				// 45 ;
		m_pgptbYes->x1 = m_pgptbYes->x + 144 ;
		m_pgptbYes->y  = m_iY0 + 335 ;
		m_pgptbYes->y1 = m_pgptbYes->y + 22 ;
		//m_pgptbYes->Message = m_chYes.pchar() ;
		BigMapSetMessage( m_pgptbYes , m_chYes.pchar() );

		m_pgptbClose->Sprite = 7;					// 6;
		m_pgptbClose->x  = m_iX0 + 399 ;			// 353 ;
		m_pgptbClose->x1 = m_pgptbClose->x + 144 ;
		m_pgptbClose->y  = m_iY0 + 335 ;
		m_pgptbClose->y1 = m_pgptbClose->y + 22 ;

		m_pgptbNo->Enabled = m_pgptbNo->Visible = true; 
	}
	else
	if( m_iMessType == 2 ) {
		m_pgppBox->SpriteID = 2;
		
		m_pgptbYes->Sprite = 6;						// 5;
		m_pgptbYes->x  = m_iX0 + 80 ;				// 45 ;
		m_pgptbYes->x1 = m_pgptbYes->x + 204 ;
		m_pgptbYes->y  = m_iY0 + 335 ;
		m_pgptbYes->y1 = m_pgptbYes->y + 22 ;
		//m_pgptbYes->Message = m_chOK.pchar() ;
		BigMapSetMessage( m_pgptbYes , m_chOK.pchar() );
		
		m_pgptbClose->Sprite = 6;					// 5;
		m_pgptbClose->x  = m_iX0 + 323 ;			// 353 ;
		m_pgptbClose->x1 = m_pgptbClose->x + 204 ;
		m_pgptbClose->y  = m_iY0 + 335 ;
		m_pgptbClose->y1 = m_pgptbClose->y + 22 ;

		m_pgptbNo->Enabled = m_pgptbNo->Visible = false; 
	}
	return;
}
//----------------------------------------------------------------------------------
bool	CDiplInfoMessBox::SetCallFunClicks(CDiplInfoMessages* pMs,pCULL_FUN_CLICK yes,pCULL_FUN_CLICK no,pCULL_FUN_CLICK close)
{
	m_pgptbYes->OnUserClick		= yes ;
	m_pgptbNo->OnUserClick		= no ;
	m_pgptbClose->OnUserClick	= close ;
	m_pgptbYes->UserParam		= reinterpret_cast<int>( pMs ) ;
	m_pgptbNo->UserParam		= reinterpret_cast<int>( pMs ) ;
	m_pgptbClose->UserParam		= reinterpret_cast<int>( pMs ) ;
	m_pgppBox->OnUserClick      = &EmptyClick; 
	return true;
}
//---------------------------------------------------------------------------------- 
bool	CDiplInfoMessBox::SetMessage( CDiplMessage_STRUCT* pMess, int n ) 
{ 
	m_iCurrMess = n ;
	m_iMessType = pMess->MessType;
	//m_ptbHead->Message = pMess->MessHead.pchar();
	BigMapSetMessage( m_ptbHead , pMess->MessHead.pchar() );

	//message text   
	ResFile RF=RRewrite("tmp.txt"); // void RewriteTextIntoFile(char* filename, char* text) 
	RBlockWrite(RF, pMess->MessText.pchar() ,strlen( pMess->MessText.pchar() ));
	RClose(RF);
	m_ptvText->LoadFile("tmp.txt");
	m_ptvText->y = m_ptvText->UserParam;  /*371*/
	m_ptvText->PageSize = div(m_ptvText->y1 - m_ptvText->y,m_ptvText->SymSize).quot;/*100*/
	m_ptvText->y += m_ptvText->SymSize*(4 - m_ptvText->NLines)/2 - (4 - m_ptvText->NLines);

	m_pgppPic->SpriteID = pMess->pictId;

	return true;

	//TextViewer* TV=GMM.addTextViewer(NULL,TX0,TY0,TX1-SBW-5-TX0,TY1-TY0,"tmp.txt",FText); //  /Dialogs/Messages.h
	//TV->SymSize=21;
	//TV->PageSize=div(TY1-TY0,TV->SymSize).quot;
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessBox::Show() 
{
	SetEnabled(true);
	SetVisible(true);

	PrepareMessBox();
}						
//----------------------------------------------------------------------------------
void	CDiplInfoMessBox::Close() 
{
	//m_ptbHead->Message = NULL;

	ResFile RF=RRewrite("tmp.txt"); 	//m_ptbText->Message = NULL;
	RBlockWrite(RF, "" ,strlen( "" ));
	RClose(RF);
	m_ptvText->LoadFile("tmp.txt");

	SetEnabled(false);
	SetVisible(false);
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessBox::SetVisible(bool state) 
{
	if (m_Visible==state) { return; }
	m_Visible=state;
	SimpleDialog*	a_pdb = NULL;
	for (int i=0; i<m_iN ; i++) //элементы окна
	{
		a_pdb = m_pMenu->Find(m_iBoxOffset + i);
		if (a_pdb==0x00000000) { return; }
		a_pdb->Visible = state;
	}
	if( state && this->m_iMessType == 2 ) { m_pgptbNo->Visible = false; }
	if( state && this->m_iMessType == 3 ) { m_pgptbNo->Visible = true;  }
	
	//m_pgppBox->Visible =  state;//false; // 	//m_pgppPic->Visible = state;// false; // 	//m_ptbHead->Visible = state;// false; // 	//m_ptbText->Visible = state;// false; // 	//m_pgptbYes->Visible = state;// false; // 	//m_pgptbNo->Visible = state;// false; // 	//m_pgptbClose->Visible = state;// false; // 
}
//----------------------------------------------------------------------------------
void	CDiplInfoMessBox::SetEnabled(bool state) 
{
	if (m_Enabled==state) { return; }
	m_Enabled=state;
	SimpleDialog*	a_pdb = NULL;
	for (int i=0; i<m_iN ; i++) //элементы окна
	{
		a_pdb = m_pMenu->Find(m_iBoxOffset + i);
		if (a_pdb==0x00000000) { return; }
		a_pdb->Enabled = state;
	}
	if (state && this->m_iMessType==2) { m_pgptbNo->Enabled = false; }
	if (state && this->m_iMessType==3) { m_pgptbNo->Enabled = true; }
}
//----------------------------------------------------------------------------------
int		CDiplInfoMessBox::GetBoxOffset() 
{ 
	return m_iBoxOffset; 
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////
///////////////////////////   CBriefing   ////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////
CBriefing::CBriefing(DialogsSystem* pMenu, int offset)
{
	m_pMenu   = pMenu;
	m_iOffset = offset;
	SecMap    = NULL;
	BriefDesk = NULL;
	ObjDesk   = NULL;
	SecDesc   = NULL;
	SecDesc2  = NULL;
	m_Visible = true;
	m_Enabled = true;

	m_Visited = false;//

	Init();
}
//----------------------------------------------------------------------------------
void	CBriefing::Init()
{
	int id=0;
	// preview карты сектора
	LocalGP		a_lgpSecMap("Interf3\\TotalWarGraph\\bSectMap");			//sectors maps
	SecMap = m_pMenu->addGPPicture(NULL, 445,168 ,a_lgpSecMap.GPID,0); 
	SecMap->SpriteID = -1 ; 
	SecMap->AssignID(m_iOffset + id++);
	
	//тактические характеристики по сектору (инф-я слева)					//Sec00.txt
	BriefDesk = m_pMenu->AddDialogsDesk( 80, 170, 245, 490 ,"EmptyBorder");
	BriefDesk->AssignID(m_iOffset + id++);
	SecDesc   = BriefDesk->addTextViewer(NULL, 0, 0, 210, 490 ,"tmp.txt",&BlackFont); //NEED NEW FONTS 
	SecDesc->AssignID(m_iOffset + id++);
	
	//дополнительная информация по сектору - задачи+квесты (инф-я справа)	//SecObj00.txt
	ObjDesk  = m_pMenu->AddDialogsDesk( 450, 440, 507, 180 ,"EmptyBorder");
	ObjDesk->AssignID(m_iOffset + id++);
	SecDesc2 = ObjDesk->addTextViewer(NULL, 0, 0, 472, 180 ,"tmp.txt",&BlackFont); //NEED NEW FONTS 
	SecDesc2->AssignID(m_iOffset + id++);

	SecDesc->SymSize = SecDesc2->SymSize = 21 ;	//27 ; 

	SetVisible(false);
	SetEnabled(false);
}
//----------------------------------------------------------------------------------
void	CBriefing::SetVisible(bool state)
{
	SecMap->Visible    = state;
	SecDesc->Visible   = SecDesc2->Enabled  = state;
	BriefDesk->Visible = ObjDesk->Visible   = state; 
	m_Visible = state;
}
//----------------------------------------------------------------------------------
void	CBriefing::SetEnabled(bool state)
{
	SecMap->Enabled    = state;
	SecDesc->Enabled   = SecDesc2->Enabled  = state;
	BriefDesk->Enabled = ObjDesk->Enabled   = state; 
	m_Enabled = state;
}
//----------------------------------------------------------------------------------
bool	CBriefing::SetVisit()
{
	// Find(0x40+ i ); PassiveFont = RED!!!
	m_Visited = true;
	return true;
}
//----------------------------------------------------------------------------------
bool	CBriefing::Refresh(bool BriefingVisible, int secId)
{
	if( secId < 0 ) { return false; }

	if( !BriefingVisible && m_Visible )	//выключить
	{
		SetVisible(false);
		SetEnabled(false);
		return true;
	}

	if( BriefingVisible && !m_Visible )	//включить и сменить инфу (если надо)
	{
		SetVisit();

		if( SecMap->SpriteID != secId ) 
		{ 
			SecMap->SpriteID = secId; 

			char zero[10]; 
			if (secId<10) { sprintf(zero, "0"); } else { sprintf(zero, ""); }

			char path[256];
			sprintf( path, "Missions\\StatData\\Sector%s%d\\Sec%s%d.txt", zero, secId, zero, secId );
			SecDesc->LoadFile( path );
			SecDesc->y1 = SecDesc->y + SecDesc->NLines*SecDesc->SymSize;
			SecDesc->PageSize = div(SecDesc->y1 - SecDesc->y, SecDesc->SymSize).quot;
				
			char path2[256];
			sprintf( path2, "Missions\\StatData\\Sector%s%d\\SecObj%s%d.txt", zero, secId, zero, secId );
			SecDesc2->LoadFile( path2 );
			SecDesc2->y1 = SecDesc2->y + SecDesc2->NLines*SecDesc2->SymSize;
			SecDesc2->PageSize = div(SecDesc2->y1 - SecDesc2->y, SecDesc2->SymSize).quot;
			if(ObjDesk->VScroller)	  ObjDesk->VScroller->SPos = 0; 
			if(BriefDesk->VScroller)  BriefDesk->VScroller->SPos = 0;
		}
		SetVisible(true);
		SetEnabled(true);
		return true;
	} 		

	return false;
}
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------
// *******************************************************************************************************************
// *******************************************************************************************************************
