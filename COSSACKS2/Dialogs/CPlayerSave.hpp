
//-------------------------------------------------------------------------------------------------------------------
//			������ � ��������� ��� ���������� ������ �� ������ ���� "Total War" ( ����� CPlayerSAVE )
//-------------------------------------------------------------------------------------------------------------------

void	test_class2(BaseClass* BC);							// ***DREW test***
//extern CSectData	sdSecDat; //new
//-------------------------------------------------------------------------------------------------------------------
//----------------------------- CPlayerSAVE STRUCTURES --------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//������ ������ � �������� �� �����
class CSectSAVE : public BaseClass
{
	public:
		CSectSAVE()		{ Init(); };
		~CSectSAVE()	{ Init(); };
		void Init() {
				NI = POP = DEF = REC = SAB = 0;	};

		int		NI;				// �����.
		int		POP;			// ���������.
		int		DEF;			// ������.
		int		REC;			// �������
		int		SAB;			// divertion id

		SAVE( CSectSAVE );
			REG_MEMBER( _int, NI  );
			REG_MEMBER( _int, POP );
			REG_MEMBER( _int, DEF );
			REG_MEMBER( _int, REC );
			REG_MEMBER( _int, SAB );
		ENDSAVE;
};
//--------------------------------------------------------------------------------------------------------------------
//������ ������ �� ����� ����� 
class CSquardSAVE : public BaseClass
{
	public:
		CSquardSAVE() { 
			Init(); 
		};
		~CSquardSAVE() { 
			Init(); 
		};
		void Init() {
				KILLS.Clear();
				TypeGlobID = SQ_MAX = SQ_NUM = 0; 
		};

		int		TypeGlobID;				// ���������� ������ ���� ������.
		int		SQ_MAX;					// ������������ ���-�� ������� ������� ����.
		int		SQ_NUM;					// ���-�� ������� ������� ����.

		LinearArray<int,_int>	KILLS;	// ���-�� kills � �������

		SAVE( CSquardSAVE );
			REG_MEMBER( _int, TypeGlobID );
			REG_MEMBER( _int, SQ_MAX );
			REG_MEMBER( _int, SQ_NUM );
			REG_AUTO  (       KILLS  );
		ENDSAVE;
};
//--------------------------------------------------------------------------------------------------------------------
//������ ������ � ������ ������
class CHeroSAVE : public BaseClass	
{
	public:
		CHeroSAVE() { 
			Init(); 
		};
		~CHeroSAVE() { 
			Init(); 
		};
		void Init() {
				SQUARDS.Clear();
				mAlive=false;	
				mHeroID=mSecID=mPrSecID=mExpa=mBatlN=mVictN=mRankID=mStatID=mFuel=mLead=mLuck=mTalant=0; /*mNI=0*/
		};
	
		int		mHeroID;		// ����� ����� (id).
		int		mSecID;			// id ������� !
		int		mPrSecID;		// previd ������� !
		int		mExpa;			// �����
		int		mBatlN;			// ���-�� ��������
		int		mVictN;			// ���-�� �����
		int		mFKill;
		int		mFLost;

		int		mRankID;		//������ ������.
		int		mFuel;			//����� �������� ��� �����. (���-�� �����)

		int		mStatID;		//������ �������������.
		int		mLead;			//new - leadership/���������
		int		mLuck;			//new - luck/�����/������� � �����
		int		mTalant;		//new - talant/������/���������

		bool	mAlive;			// � ���� ��� ���

		//������ ������� � �����
		ClassArray<CSquardSAVE> SQUARDS;

		SAVE( CHeroSAVE );
			REG_MEMBER( _bool, mAlive   );
			REG_MEMBER( _int,  mHeroID  );
			REG_MEMBER( _int,  mRankID  );
			REG_MEMBER( _int,  mStatID  );
			REG_MEMBER( _int,  mExpa    );
			REG_MEMBER( _int,  mBatlN   );
			REG_MEMBER( _int,  mVictN   );
			REG_MEMBER( _int,  mFKill   );
			REG_MEMBER( _int,  mFLost   );
			REG_MEMBER( _int,  mSecID   );
			REG_MEMBER( _int,  mPrSecID );
			REG_MEMBER( _int,  mFuel    );
			//REG_MEMBER( _int,  mFreeSQ  );
			REG_CLASS (CSquardSAVE);
			REG_AUTO  ( SQUARDS );
		ENDSAVE;
		//REG_MEMBER( _int, mNI );	REG_MEMBER( _int, mLead );	REG_MEMBER( _int, mLuck );	REG_MEMBER( _int, mTalant );
};
//--------------------------------------------------------------------------------------------------------------------
//������ ������ � ������� �� �����
class CCountrySAVE : public BaseClass
{
	public:
		CCountrySAVE() { 
			Init(); 
		};
		~CCountrySAVE() { 
			Init(); 
		};
		void Init() {
				HEROES.Clear();	
				mRes.Init();	
				mAlive = false; 
		}
	
		bool			mAlive;					// ������ �� ����������
		CResource_BM	mRes;					// ������� ������
		
		//������ ������
		ClassArray<CHeroSAVE> HEROES;

		SAVE( CCountrySAVE );
			REG_MEMBER( _bool, mAlive  );
			REG_CLASS (CHeroSAVE);
			REG_AUTO  ( HEROES );
			REG_CLASS (CResource_BM);
			REG_AUTO  ( mRes );
		ENDSAVE;
};
//--------------------------------------------------------------------------------------------------------------------
//������ ������ ���������� �� ����� (CStatsSAVE)
class CStatsSAVE : public BaseClass 
{
	public:
		CStatsSAVE() { 
			Init(); 
		};
		~CStatsSAVE() { 
			Init(); 
		};
		void Init() {
			SecNum.Clear();
			ResSup.Clear();
			ResAmo.Clear(); 
			RecrtN.Clear();
            ArmPow.Clear(); 	
			NatPow.Clear(); 	
			GenExp.Clear(); 	
			FKill.Clear();	
			FLost.Clear(); 
			indx = 0; 
		};
	
		int		indx;								// indx

		// ���������� �� ������ ��� (0..m_inCurTurn-1)
		LinearArray<int,_int>	SecNum;				// sectors num
		LinearArray<int,_int>	ResSup;				// supply
		LinearArray<int,_int>	ResAmo;				// resources amount
		LinearArray<int,_int>	RecrtN;				// recruits number
		LinearArray<int,_int>	ArmPow;				// power of army
		LinearArray<int,_int>	NatPow;				// power of nation
		LinearArray<int,_int>	GenExp;				// commander exp.amount
		LinearArray<int,_int>	FKill;				// formation killed
		LinearArray<int,_int>	FLost;				// formation lost

		SAVE( CStatsSAVE );
			REG_MEMBER( _int,  indx );
			REG_AUTO( SecNum );		
			REG_AUTO( ResSup );		
			REG_AUTO( ResAmo );		
			REG_AUTO( RecrtN );		
			REG_AUTO( ArmPow );		
			REG_AUTO( NatPow );		
			REG_AUTO( GenExp );		
			REG_AUTO( FKill  );		
			REG_AUTO( FLost  );		
		ENDSAVE;
};
//������ ������ ���������� ���� (CNatStatSAVE)
//class CNatStatSAVE : public BaseClass //very new
//{
//	public:
//		CNatStatSAVE() { 
//			Init(); 
//		};
//		~CNatStatSAVE()	{ 
//			Init(); 
//		};
//		Init() { 
//				STATS.Clear();
//				indx=LastUpdate=0; 
//		};
//	
//		int		indx;								// indx
//		int		LastUpdate;							// last update Turn
//
//		// ������ ����� ����������
//		ClassArray<CStatsSAVE>	STATS;
//
//		SAVE( CNatStatSAVE );
//			REG_MEMBER( _int,  indx			);
//			REG_MEMBER( _int,  LastUpdate	);
//			REG_CLASS (CStatsSAVE);
//			REG_AUTO  ( STATS );
//		ENDSAVE;
//};
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------

//������ ������ �� ������� ��� CPlayerSAVE 
class CPlayerSAVE_STRUCT : public BaseClass
{
	public:
		CPlayerSAVE_STRUCT() {	
			Init();	
		}
		~CPlayerSAVE_STRUCT() {	
			Init();	
		}
		void Init() {
				SECTORA.Clear(); 
				COUNTRY.Clear(); 
				CONT.Clear(); 
				REQ.Clear(); 
				MESS.Clear(); 
				help.Clear(); 
				STATS.Clear();
				LastStUpdate = -1;
				m_iId = m_iNation = m_iDifficulty = m_inCurTurn = m_iConCost = 0;
				m_inCurActiveAI=m_inCurHeroMove=m_iCurMX0=m_iCurMY0=m_iCurSecId=m_iCurHeroId=m_iCurMenuId = 0;
				m_MIDataSector_ID = m_MIDataAttNation=m_MIDataDefNation=m_MIDataPlayerStat = -1;	
				m_QuestGived=m_MIDataHeroInSect=false ;	
				m_QuestID = m_QuestOwner = m_QuestSecId = m_QuestTarNI = m_QuestWait = -1;
				DisableTutorialMessage=false;
				DatFile.DString::Clear();
				DatFile.DString::ReadFromFile( "Missions\\Heroes\\heroinf.dat" );
				//strcpy(m_chFilePath, "Save\\bmdat%d.sav" );	//"Players\\bmdat0.xml"			
				strcpy(m_chFilePath, "Save\\Battle\\bmdat%d.sav" );	//"Players\\bmdat0.xml"			
		}
		bool	LoadXML(int id=0);
		bool	SaveXML(int id=0);
	
		char	m_chFilePath[256];				//save/load path	

		int		m_iId;							// ���������� ����� ������

		_str	m_chName;						// ���
		int		m_iNation;						// �����
		int		m_iDifficulty;					// ������� �����.
		int		m_iCurHeroId;					// hero sprite id

		int		m_iCurMX0;						//new	current map x
		int		m_iCurMY0;						//new	current map y
		int		m_iCurSecId;					//new	current sector		
		int		m_iCurMenuId;					//new	current menu
		int		m_inCurActiveAI;				// ��� ROOT_SCENARIO.AI_PROCESS
		int		m_inCurHeroMove;				// ��� ROOT_SCENARIO.AI_PROCESS
		int		m_inCurTurn;					// ����� �������� ���� ( ��� ���� �������� )

		// mission init data  
		int		m_MIDataSector_ID;
		int		m_MIDataAttNation;
		int		m_MIDataDefNation;
		int		m_MIDataPlayerStat; 
		bool	m_MIDataHeroInSect;

		// quests data  
		bool	m_QuestGived;
		int		m_QuestID;
		int		m_QuestOwner;
		int		m_QuestSecId; 
		int		m_QuestTarNI;
		int		m_QuestWait;

		// tutorial message before skirmish or campaign
		bool	DisableTutorialMessage;

		// ���� �� ��������. (���������� ����� ������������� ID �������)
		ClassArray<CSectSAVE>	 SECTORA;

		// ���� �� �������. (���������� ����� ������������� ID ������)
		ClassArray<CCountrySAVE> COUNTRY;

		// ���� �� ����.������ (DiplomacyData)
		LinearArray<int,_int>	 CONT;			//������ ���������
		LinearArray<int,_int>	 REQ;			//������ �������� � ������ �� �������
		int						 m_iConCost;	//������� ��������� ���������� ��������

		// ���� �� �������������� ���������� ������
		ClassArray<CDiplMessage_STRUCT>  MESS;	//������ ���.�����.

		// help visited
		LinearArray<bool,_bool>	 help;			//help visited

		// ���� �� ���������� �� ������ [0 .. MAX_PLAYERS]
		int		LastStUpdate;						// last update Turn
		ClassArray<CStatsSAVE>	 STATS;				// STATS[0..MAX_PLAYERS]

		// Hero Info .dat-file data
		_str	DatFile;//grey [04.08.2004]
		
		// campaign		// by vital
		int c_Difficulty;
		int c_NDone;


		// ������. � ����������� �������� �������
		bool	GetCapturedSectors(int& SecCapt, int& SecTotal){//new
			DWORD mc;
			int NID = GetBigMapNationID( m_iNation , mc);

			SecCapt  = 0;
			SecTotal = SECTORA.GetAmountOfElements();
			if( SecTotal > 0 ) {
				for(int i=0; i < SecTotal ; i++) 
					if( SECTORA[i]->NI == NID )  SecCapt++;
			} 
			else {
				CSectData  a_SecDat("Missions\\StatData\\Sectors.dat");
				SecTotal = a_SecDat.GetSectorsNum();
				SecCapt  = a_SecDat.GetPlayerSect( NID );
				if( SecTotal < 0 )  SecTotal = 0;
				if( SecCapt < 0 || SecCapt > SecTotal )  SecCapt = 0;
			}
			return true; 
		}
		//bool	GetCapturedSectors(int& SecCapt, int& SecTotal) {
		//	SecCapt  = 0;
		//	SecTotal = SECTORA.GetAmountOfElements();
		//	for(int i=0; i < SecTotal ; i++) if(SECTORA[i]->NI == NID) SecCapt++;
		//	return true; 
		//}

		bool GetHeroDescription(_str& Desc) {
			return  GetHeroDescription(Desc,m_iNation,m_iCurHeroId);//grey [04.08.2004]
		}

		bool GetHeroDescription(_str& HeroDesc, int NationId , int HeroId) {//grey [04.08.2004]
			if( !DatFile.pchar() )  return false;
			_str	datf;
			datf += DatFile;
			DString	a_DS2;
			bool	quit = false;		
			int		natID = NationId , nn = 0;
			do { 
				datf.DString::ExtractWord(a_DS2);		nn++;	
				if( !strcmp(NatCharLo[NationId],a_DS2.pchar()) || !strcmp(NatCharHi[NationId],a_DS2.pchar()) ) {
					datf.DString::ExtractWord(a_DS2);	nn++;
					natID = atoi(a_DS2.pchar());			quit = true;									
				}
			} while ( !quit && nn < 1000 );

			char	path[256];
			sprintf(path, "Missions\\Heroes\\heroinf%d%d.txt", natID, HeroId);
			HeroDesc.DString::ReadFromFile(path);
			return true;
		}
		//bool GetHeroDescription(_str& Desc){
		//	char		path[256];
		//	sprintf(path, "Missions\\Heroes\\heroinf%d%d.txt", m_iNation , m_iCurHeroId);
		//	Desc.DString::ReadFromFile(path);
		//	return true;
		//}

		SAVE( CPlayerSAVE_STRUCT );
			REG_MEMBER( _int,  c_Difficulty	);
			REG_MEMBER( _int,  c_NDone );

			REG_MEMBER( _int,  m_iId		);
			REG_AUTO  (		   m_chName		);  
			REG_MEMBER( _int,  m_iNation	);
			REG_MEMBER( _int,  m_iDifficulty );
			REG_MEMBER( _int,  m_iConCost	);
			REG_MEMBER( _int,  m_inCurTurn	);

			REG_MEMBER( _int,  m_iCurMX0		);
			REG_MEMBER( _int,  m_iCurMY0		);
			REG_MEMBER( _int,  m_iCurSecId		);
			REG_MEMBER( _int,  m_iCurHeroId		);
			REG_MEMBER( _int,  m_iCurMenuId		);
			REG_MEMBER( _int,  m_inCurActiveAI	);
			REG_MEMBER( _int,  m_inCurHeroMove	);

			REG_MEMBER( _int,  m_MIDataSector_ID  ) ;
			REG_MEMBER( _int,  m_MIDataPlayerStat ) ;
			REG_MEMBER( _int,  m_MIDataAttNation  ) ;
			REG_MEMBER( _int,  m_MIDataDefNation  ) ;
			REG_MEMBER( _bool, m_MIDataHeroInSect ) ;

			REG_MEMBER( _bool, m_QuestGived );
			REG_MEMBER( _int,  m_QuestID    );
			REG_MEMBER( _int,  m_QuestOwner );
			REG_MEMBER( _int,  m_QuestSecId );
			REG_MEMBER( _int,  m_QuestTarNI );
			REG_MEMBER( _int,  m_QuestWait  );
			
			REG_MEMBER(_bool, DisableTutorialMessage);

			REG_CLASS ( CSectSAVE			);
			REG_AUTO  ( SECTORA				);
			REG_CLASS ( CCountrySAVE		);
			REG_AUTO  ( COUNTRY				);
			REG_AUTO  ( CONT				);
			REG_AUTO  ( REQ					);
			REG_CLASS ( CDiplMessage_STRUCT );
			REG_AUTO  ( MESS				);
			REG_AUTO  ( help				);
			
			REG_MEMBER( _int,  LastStUpdate );
			REG_CLASS ( CStatsSAVE		); 
			REG_AUTO  ( STATS			); 
		ENDSAVE;

}; // end of struct CPlayerSAVE_STRUCT
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------
//------------------------------CPlayerSAVE---------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------

//������ ������ �� ������� (save)
class CPlayerSAVE : public BaseClass
{ 
	public:
		CPlayerSAVE() { 
			Init(); 
			LoadXML(); 
		}
		~CPlayerSAVE() {
			Init(); 
		}
		void Init() {
			m_pPlayer.Clear();
			pr = 0; 

			strcpy( m_chSavePath, "Save\\Battle" );					// Save Path
			strcpy( m_chFileName, "Save\\Battle\\bmdat.sav" );		// Save file name
			//strcpy(m_chFileName, "Save\\bmdat.sav" );				
			//strcpy(m_chFileName, "Save\\Battle\\bmdat.sav" );		// Save Path
		}

		int		pr;
		char	m_chSavePath[256];						//save/load path		
		char	m_chFileName[256];						//save/load path		
		
		ClassArray<CPlayerSAVE_STRUCT>		m_pPlayer;	//������ ���� �� �������		

		SAVE( CPlayerSAVE );							
			REG_MEMBER( _int,  pr );
		ENDSAVE;

		//SAVE( CPlayerSAVE );							
		//	REG_CLASS( CPlayerSAVE_STRUCT );			
		//	REG_AUTO ( m_pPlayer );	
		//ENDSAVE;

	
		bool	TestSelfPtr() {
			//for(int i=0; i < m_pPlayer.GetAmountOfElements() ; i++) {
			//	test_class2( m_pPlayer[i] );
			//}
			return true;
		};

		bool	SaveXML();
		bool	LoadXML();

		bool	SaveCurProfile(int cur=0);							// save current profile (m_pPlayer[0])
		
		// ������� ���������� ������ � ������� ��������  (m_pPlayer[0])
		void	getSECTORA( CSectData* pSD ); //save				// ��������� ���� �� �������� �� pSD
		void	setSECTORA( CSectData* pSD ); //load				// ��������� ����� ������� pSD

		void	GetCountry( CPlayer* pCountry[MAX_PLAYERS] );		// ��������� ������ (Players)
		void	SetCountry( CPlayer* pCountry[MAX_PLAYERS] );		// ��������� ������ (Players)

		void	GetDiplData( CDiplomacyData* pDipl );				// ��������� ����.���� ��� ����������
		void	SetDiplData( CDiplomacyData* pDipl );				// ��������� ������ � ����.����

		void	GetMessages( CDiplInfoMessages* pMess );			// ��������� ���������
		void	SetMessages( CDiplInfoMessages* pMess );			// ��������� ������ � ���������

		void	GetMissionInitData( CMISSIONINIT* pInitData );		// ��������� ������ ��� ������ ������
		void	SetMissionInitData( CMISSIONINIT* pInitData );		// ��������� ������ ��� ������ ������

		void	GetQuestsData( CBigMapQuest* pQuests );				// ��������� ������ �������
		void	SetQuestsData( CBigMapQuest* pQuests );				// ��������� ������ �������

		void	GetMenuState( int  dXM, int  dYM, int  secId, int  heroId, int  menuId , int  actAI, int  curHeroMove , int  curTurn  ); //��������� �����. ����.���� 
		void	SetMenuState( int* dXM, int* dYM, int* secId, int* heroId, int* menuId , int* actAI, int* curHeroMove , int& curTurn  ); //������������ ����.���� 
		
		void	GetMenuState( int  dXM, int  dYM, int  secId, int  heroId, int  menuId ); //��������� �����. ���� ��� ����������
		void	SetMenuState( int* dXM, int* dYM, int* secId, int* heroId, int* menuId ); //������������ ��� ����

		void	GetHelpState( CBigMapHelp* pHelp ); //save
		void	SetHelpState( CBigMapHelp* pHelp );	//load

		bool	SaveStatistics( CPlayer* pCountry[MAX_PLAYERS] , bool save_all=false );	// ��������� //save 

		// ������� ������ � ����������
		int		GetNextId();												// ����. Id ������

		CPlayerSAVE_STRUCT* AddProfile(CPlayerSAVE_STRUCT* Player);			// ��������� ������ � ������		
		bool				DeleteProfile(CPlayerSAVE_STRUCT* &Player);		// ���� � ������� ������
};
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------



