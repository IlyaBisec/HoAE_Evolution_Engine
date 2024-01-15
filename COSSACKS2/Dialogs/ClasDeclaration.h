#ifndef __CBigMap_Class_Declaration__
#define __CBigMap_Class_Declaration__

#include "..\cvi_market.h"

//������ ��� ������ � �������� ������ �� ����.

//#define TEST_LVA

//#define DemoVer4Journals
//#define DemoAI		1		//DEMO
//#define DemoAI(sp)	sp?0:1	//DEMO

/*
#include "FSMstate.hpp"
#include "FSMclass.hpp"

//////////////////////////////////////////////////////////////////////////
// Declaration state and FSM for animetion hero and sity on bigmap
//////////////////////////////////////////////////////////////////////////

#define EMPTY_str		"EMPTY"
class	CstrID : public CID_BASE
{
public:
	CstrID(){};
	CstrID( int ID );
	CstrID( const CstrID& ID );

	virtual void	operator=	( int _id );
	virtual bool	operator==	( int _id );
	virtual bool	operator!=	( int _id );

	virtual void	operator=	( const CstrID&	UID );
	virtual bool	operator==	( const CstrID&	UID );
	virtual bool	operator!=	( const CstrID&	UID );

	virtual void	operator=	( const _str	UID );
	virtual bool	operator==	( const _str	UID );
	virtual bool	operator!=	( const _str	UID );

	virtual void	operator=	( const char*	UID );
	virtual bool	operator==	( const char*	UID );
	virtual bool	operator!=	( const char*	UID );

	virtual	void	SetEmpty();
	virtual	bool	GetEmpty();

	_str	strID;		// string name 

	SAVE(CstrID);
		REG_AUTO( strID );
	ENDSAVE;
};

class	CSATE_PARA : public paraBC< CstrID, CstrID >
{
public:
	CSATE_PARA(){};
	CSATE_PARA( const CstrID& first, const CstrID& second ) : paraBC<CstrID,CstrID>(first,second) {};

	SAVE( CSATE_PARA );
		REG_AUTO( m_first );
		REG_AUTO( m_second );
	ENDSAVE;
};

class	CANIM_STATE : public FSMstate< CstrID, CstrID, CSATE_PARA >
{
public:
	// Additionl data for sity animation
	CANIM_STATE(){};

	// Data for animation
	word	AnimFileID;
	int		AnimSpriteID;
	int		AnimSpriteNUM;
	int		AnimPause;


	SAVE( CANIM_STATE );
		REG_AUTO( m_StateID );
		REG_AUTO( m_Input_vs_OutputState );
	ENDSAVE;
};


void	TEST_EDIT_ASS();


void	RegAnimateClassForBigMap();
*/
//////////////////////////////////////////////////////////////////////////

typedef	bool (*pCULL_FUN_CLICK)(SimpleDialog*);

extern bool v_Exit2MainMenu;

extern	bool			g_bEDITOR_CHENGE;		// ��������� ��������� � ClassEditor, ��������.
extern	Enumerator*		g_pALL_UNIT_TYPE;

// EXTERN FONTS
extern RLCFont  OrangeFont;
extern RLCFont  GrayFont;
extern RLCFont  SmallGrayFont;
extern RLCFont  SpecialGrayFont;
extern RLCFont  SmallBlackFont1;
extern RLCFont	fonMenuText[7];// menu
extern RLCFont	fonMenuTitle[7];
extern RLCFont	fonMenuTitle2[7];
extern RLCFont	fonMenuGold;
// EXTERN Big Map Global Variables
extern int		LTN_EXPS_LIMIT;	// 0			// ������� ���-�� ����� �� ������ 
extern int		CPT_EXPS_LIMIT; // 1
extern int		MJR_EXPS_LIMIT; // 2
extern int		COL_EXPS_LIMIT; // 3 
extern int		GEN_EXPS_LIMIT; // 4
extern int		MAR_EXPS_LIMIT; // 5

extern int		DisbandSQExp;					// ����� �� ������ �����
extern int		SecCaptExp;						// ����� �� ������/������ �������	
extern int		HeroDefExp;						// ����� �� ������ ��� ������
extern int		QuestDisbSquadExp;				// ����� �� ������ quest-�����
extern int		QuestCompliteExp;					// ����� �� ������. quest

extern int		SECT_INCOME[10];				// ����� �� ������� �� ������ ����
extern int		SECT_ADDING[10];				// ����� �� ��������� ������� �������
extern int		SECT_MAX_RECRTS;				// ����.���-�� ��������
extern int		SECT_REG_RECRTS;				// �������������� �������� � �������
extern int		SECT_DEFENCE[10];				// ����a�� �� defence �������
extern float	SECT_DEF_multiplier2;
extern float	SECT_DEF_multiplier3;
extern int		SECT_SABOTAGE[10][5];			// ������� for SABOTAGE
extern int		SECT_SAB_PERCENT[10][5];		// sabotage percent
extern DWORD	SELECTED_SECT_COLOR;			// selected sector
extern DWORD	RED_SECT_COLOR;					// ����.����� - ����������
extern DWORD	GREEN_SECT_COLOR;				// ��������   - ����������
extern DWORD	COMPAGE_BLINKCOLOR;				// commanders Page blink color

//**********************************************************************************
// CONSTANTS

extern int		STEPBACK_TURNLIMIT;					// 5;
extern int		STEPBACK_DISTANCE;					// 2;

#define DIFF_LEVELS_N		 3	//4			// ���-�� ������� ��������� ( Big Map )
#define	MAX_PLAYERS			 6				// ������������ ���-�� �������.
#define MAX_COUNTRYS		 6				// ������������ ���-�� �����.
#define MAX_HEROES			10				// ������������ ���-�� ������.
#define MAX_RANKS			 6				// ������������ ���-�� ������.
#define MAX_H_STATUS		20				// ������������ ���-�� ��������, ������.
#define MAX_ARROW			10				// ������������ ���-�� ���������.
#define MAX_CONT_VALPERIOD	10				// ����� �������� ����.���������
#define	MAX_DEFENCE			 3				// ������������ ��-� defence �������
#define	MAX_POPULATION		 2				// ������������ ��-� Population �������

#define	MAX_AGGRESSIVE		 9				// 

#define START_SECTOR_ID		6
#define	Safe_FOOD_Amount	50000			// ���.���-�� ��� ��

// ������� � ����
#define WOOD  0
#define GOLD  1
#define STONE 2
#define FOOD  3
#define IRON  4
#define COAL  5
#define RECRT 9

// COLORS
#define	ALPHA_SECT			0x20000000			// ������������ ��� ��������  //38	// 5B	// 0x66000000
//#define	SELECTED_SECT_COLOR	0x88000000		// selected sector			//#define SELECTED_SECT_COLOR	0x5BB3B3A9
//#define	RED_SECT_COLOR		0xFFFF0000		// ����.����� - ����������
//#define	GREEN_SECT_COLOR	0xFF00AA00		// ��������   - ����������
////#define	DIPL_RELBAR_COLOR	0x5B000000		// ����+�����. ������� � ���.����.���-�

#define	SQ_Amount0	120						// ������������ ���-�� ��������(������) � ������.			// 123
#define	SQ_Amount1	 30						// ������������ ���-�� ��������(������) � cavalry ������.	//  31
#define	SQ_Amount2	  4						// ������������ ���-�� ��������(������) ��� �����.			//   4

#define	BIGMAP_STATS_DIST	26
#define	BIGMAP_EXPSFORMAT	"%s {R FF000000}%d{C}"
#define	BIGMAP_VICTFORMAT	"%s {R FF000000}%d/%d{C}"
#define	BIGMAP_SQNNFORMAT	"%s {R FF000000}%d/%d{C}"
#define	BIGMAP_SQNNFORMAT1	"%s {R FF000000}%d{C}"
//
#define	BIGMAP_RECRTICON	"{IBMP_IRECRT} "

#define	BM_SHOW_SECPAUSE	1.2 


//----------------------------------------------------------------------------------
#define STATHINT(x,y)\
{\
	static const char* y=GetTextByID(#y);\
	x->Hint=(char*)y;\
}
//----------------------------------------------------------------------------------
#define DYNHINT(x,y)\
{\
	x->Hint=NEW(char,strlen(y)+1);\
	strcpy(x->Hint,y);\
	x->AllocHint=1;\
}
//----------------------------------------------------------------------------------
#define DYNHINT_RA(x,y)\
{\
	x->Hint=(char*)realloc(x->Hint,strlen(y)+1);\
	strcpy(x->Hint,y);\
	x->AllocHint=1;\
}
//----------------------------------------------------------------------------------
// EXTERN FUNCTIONS
void	SetBigMapGlobalVariables();

void	VityaError(char* Mess);
bool	CreateDirectoryInCur(char* DirName);
int		GetBigMapPlayerColor(int PID);					// PID - ����� ������.
bool	PushOnTop(DialogsSystem* pdsMenu,int psdPic);	// ���������� �������� ������.
float	GetAngle(float x,float y);
void	RoundAngle(int Fi);								// ������� �������� ���� � �������� [0,255].
int		GetGoldForRess(int resID);						// ������� ����� �������� ������.
char*	GetRankByID(int rank);							// �������� ������ �� �������.
char*	GetContByID(int cont);							// �������� ������ � ��������� ������.
void	setCoordSD( SimpleDialog* psd, int x, int y );
bool	OnRulonClick(SimpleDialog* pSD);				// ���� �� ������ ����� ��������� 
bool	EmptyClick(SimpleDialog* pSD);					// ������ ���������� ����� �� pSD
bool	OnPIButtonClicked(SimpleDialog* pSD);			// ���������� ������� �� ������ (pSD->UserParam = 1)
bool	OnDiversionClicked(SimpleDialog* pSD);			// grey [11.03.2004] ���������� ������� �� ������ Diversion
bool	ChangeActiveMenu(SimpleDialog* pSD);			// Grey [04/06/2004]
//----------------------------------------------------------------------------------
// pre
int		GetBigMapResource( int res );
int		SetBigMapResource( int res, int amount );
int		AddBigMapResource( int res, int amount );
void	SaveBigMapDataXML();								// ������ ���� ������ �� Big Map Campaign
int		GetBigMapNationID(int NID,DWORD& MapColor);			// return Big Map Nation ID && Color
GPPicture*	ShowBlackScreen(DialogsSystem* dsMenu); 
bool	FailSound();
int		SEC_DEF_POWER( int defence ); 

bool	ON_HERO_R_CLICK( SimpleDialog* spdHero );			// ��������� ������� ����� �� �����. // Grey [29.01.2004]
extern	class	CHero;
bool	ON_HERO_R_CLICK_VIRTUAL( CHero* pH );				// ����������� ��������� ������� ����� �� �����. // Grey [29.01.2004]
//����� ����� ������� AI.
extern	class	CSectStatData;
bool	SimulateBattle( CHero* pAttackHero, CHero* pDefendHero, CSectStatData* pSector ); // ������� �� ���������.
// add recruits
int		AddRecruitsInSectors( int nation, int secID, int amount );	// Grey [12.01.2004]
// delete recruits
int		DeleteRecruitsInSectors( int nation, int secID, int amount ); // Grey [14.01.2004]
// ����.��������� � ����������
int		ShowBigMapGlobalMessage( int type, int pictId, int headId, char* text, int pageN = -1 ); // Grey [16.01.2004]
// Enable of All Menu elements
bool	SetBigMapMenuEnabled( bool state );					// Grey [15.03.2004]
// �������� ����-�� ����� � ������
bool	CanEnterIntoSector( int nation , int secID );		// Grey [06.02.2004]
bool	CanStepIntoSector( int nation , int secID );		// Grey [01.03.2004]
// �������� ����-�� ����� �������
bool	CanAttackSector( int nation , int secID );			// Grey [27.02.2004]
bool	CanSetRightHeroAlign( int nation , int secID );		// Grey [16/04/2004]	
// IMPORT
DWORD	GetNatColor( int i );
void	GetUnitCost(byte NI,word NIndex,int* Cost,int x,int y); // Grey [08.01.2004] 
bool	GetOfficersType(byte NI, word UnitType, word &OffID, word &BarabanID, word &FlagID);
bool	TextButton_OnDraw( SimpleDialog* SD );
void	SaveBigMapDataXML();
void	ProcessBigMapPlayVideo( int id );
void	PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
bool	Draw_EW2_CampaignStats(SimpleDialog* SD);			//vital [12/01/05]
DLLEXPORT void SetResource(byte Nat,byte ID,int Amount);
DLLEXPORT int GetMyNation();

//new
DIALOGS_API void PlaySound(char* Name);						//grey [31.01.2005]
DLLEXPORT void PlayInMissionSoundCII( char* sound , int sndID = -1 );		//grey [04.03.2004]
CEXPORT void PrecacheOneUnit(int Nindex,byte NI);			// drew [7.02.2005]
void	RestoreMarketPrice();								//vital [14/01/05]
bool	ChangeMarketPrice(byte ResID, float Changer);		//vital [14/01/05]
DialogsDesk* addArcadeMode(DialogsSystem* DS, int x, int y); //vital [14/01/05]

//----------------------------------------------------------------------------------


//**********************************************************************************
// EXTERN CLASSES
extern	class	CStatusDRV;
extern	class	CError;
extern	class	CStorageData;
extern	class	CSectStatData;
extern	class	CSectDynmData;
extern	class	CSectData;
extern	class	CSectorMenu;
extern	class	CHeroPersDesk;
extern	class	CUnit;
extern	class	CSquard;
extern	class	CHero;
extern	class	CResource_BM;
extern	class	CResource_LV;
extern	class	CStorage_LV;
extern	struct	SFOR_PLAYER;
extern	class	CPlayer;
extern	class	CPicesPict;
extern	class	CScenarioLV;
extern	class	CROOTDRIVER;
extern	class	CDiplomacyData;
extern	class	CDiplMenu;
extern	class	CDiplMenuButtons;
extern	class	CDiplMenuButOnMap;
extern	class	CDiplMenuCountries;
extern	class	CDiplMenuInfo;
extern	class	CDiplInfoButtons;
extern	class	CDiplInfoMessages;
extern	class	CDiplInfoMessBox;
extern	class	CDiplInfoPanel;
extern	class	CBriefing;
extern	class	CMISSIONINIT;
extern	class	CSAVE_MIS_STRUCT;
extern	class	CResourceBM;
extern	class	CEndMissionData;
extern	class	CBigMapQuest;
extern	class	CResPanel_BM;
extern	class	CMarketDesk;
extern	class	CBigMapHelp;

extern	class	CPlayerSAVE_STRUCT;
extern	class	CPlayerSAVE;


//**********************************************************************************
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CScenarioLV    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ������ �������������� ��������.
class	CScenarioLV 
	{ 
	public:
		int			m_inBigMapCampID;			// id ��������

		// MENU
		// 0 - World Map		1 - Diplomacy		2 - Personal		3 - Messages		4 - Market
		int			m_inActivMenu;				// ������� �������� ����.
		bool		m_bMenuIsChange;			// �������� ����.

		// MAP
		bool		m_bMapVisible;				// ����� ����� ��� ���.
		bool		m_bMapEnable;				// ������������ ����� � ��. ��. �� �����.
		bool		m_bMapMouseMove;			// ������ �� ����� �� ������.

		// SECTORS
		bool		m_bSectVisible;				// ������� ����� ��� ���.
		bool		m_bSectEnable;				// ������������ ����� � ��. ��. �� ��������.
		bool		m_bSectMouseMove;			// ������ �� ������� �� ������.
		bool		m_bSectSetPlayerColor;		// ����������� � ����� �������.
		bool		m_bSectMenuVisible;			// Is sector menu visible.
		int			m_inLastLMCSecID;			// ��������� ������ �� �������� �������� ������.
		int			m_inSectLastMouseOver;		// ������ ��� ������� � ��������� ��� ���� �����.
		int			m_inSectNeighbor;			// ������� ������ ������� ����������.
	    
		// PLAYERS
		bool		m_bPlayerVisible;			// ������ ����� ��� ���.
		bool		m_bPlayerEnable;			// ������������ ����� � ��. ��. �� ������.
		bool		m_bPlayerHeroCapture;		// ����� ������� ������.
		int			m_inPlayerInUseHero;		// ��� ����� ������ ������.
		int			m_inLastHeroClick;			// ��������� ����� �� �������� �������� ������.
		bool		m_bHeroMiniMap;				// ���������� ��� �� ���������� ����� ��� �����.

		// PERSONAL HERO MENU
		bool		m_bPERSONAL_HERO_MENU;		// ��������� ���� �����.

		// BRIEFING
		bool		m_bBriefingVisible;				// map-��������: ����� ��� ���.
		bool		m_bBriefingEnabled;				// map-��������: �����e� ��� ���.
		int			m_inAttackedSector;				// ������ �������, ����������� �������.
		
		// DIPLOMACY
		bool		m_bDiplVisible;				// ����������: ������� ��� ���.
		int			m_inDiplActionPressed;		// ������� ������ ����.�������� (-1 - �� ������)
		int			m_inMapButtonPressed;		// ������� ������ ����������� ����.���� (��� ������) (-1 - �� ������) 

		// INFO DESK
		bool		m_bInfoVisible;				// desk c �����������: ����� ��� ���.
		int			m_inInfoButtonPressed;		// ������� ������ ���������

		// MESSAGES
		bool		m_bMessagesVisible;			// ���������: ����� ��� ���.

		// MARKET
		bool		m_bMarketVisible;			// �����: ������� ��� ���.

		// HELP
		bool		m_bHelpVisible;				// help: ������� ��� ���.

		void	Init();	
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CStatusDRV    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ����� ������� �������� ��������� ��� ���� ���������.
class	CStatusDRV 
	{
	public:
		virtual	void	SetVisible(bool	state)	{};		//��� - ���� ��������� �������
		virtual	void	SetEnable(bool	state)	{};		//��� - ���� ����������.
		virtual	void	CreateHint()			{};		//������� ���� ��� �������.
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CError    ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ����� �������� � ��������� ������.
class	CError
	{
	public:
		CError();						//�������� NO_ERROR
		CError(char* pchError);			//�������� pchError, ��� NO_ERROR
	private:
		char	m_chErrorStatus[256];
	public:
		virtual	void	ResetError();						//�������� ������.	
		virtual	bool	IsError();							//���� ������.	
		virtual	void	AddErrorMessage(char* pchError);	//�������� � �������� � ������ ������ pchError.
		virtual	void	SendError(char* pchError);			//��������� ��������� ������.
		virtual	void	ShowError();						//�������� ����������, ��������� ������.
		virtual	void	ShowError(char* pchError);			//�������� ����������, ��������� ������ pchError.
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////    CStorageData    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ���� ��������������� ��� ���������� ������ �� ���� � �� ��������.
class	CStorageData
	{
	public:
		void CStorage();
		void CStorage(char* pchFilePath);
	private:
		char	m_chFilePath[256];
	public:
		virtual	bool	SetFilePath(char* pchFilePath)	{return true;};

	//	virtual	bool	Save()							{return true;};
	//	virtual	bool	Save(char* chFilePath)			{return true;};
	//	virtual	bool	Load()							{return true;};
	//	virtual	bool	Load(char* chFilePath)			{return true;};
	};
//----------------------------------------------------------------------------------
//**********************************************************************************
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    CSectStatData    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ������ �� ������� ������ ���������� � �����.
class	CSectStatData : public  BaseClass	//CStatusDRV, public CError, public CStorageData
{
	public:
		CSectStatData();
		CSectStatData(DString* pDS);
		~CSectStatData();

		int					m_inID;						// ������ �������
		int					m_inDeffOwner;				// ��������� ������
		int					m_inMapOwner;				// ����� ����������� ����������� � �������
		int					x0,y0;						// ���������� �������� ������ ���� ������������ �����
		int					m_curX,m_curY;				// ������ ������������� ����������
		int					m_SityX,m_SityY;			// ������������� ���������� ������
		int					m_FortX,m_FortY;			// ������������� ���������� �����
		_str				m_chSectorName;				// ��� �������
		int					m_inNON;					// ���-�� �������

		int					m_inNV;						// ���-�� �������
		int					m_inSRID;					// ������ �������
		_str				m_chRes;					// �������� �������
		int					m_inPopulation;				// ������������ ������� {0,1,2}
		int					m_inDefence;				// ������ ������� {0,1,2,3}
		int					m_iRecruits ;				// �������
		int					m_iSabotageID;				// id �������� �� ������� +������

		LinearArray<int,_int>	m_pinNIDS;				// ������� �������
		LinearArray<int,_int>	m_pinArrowD;			// ������ �� ����������
		int						m_inArrowOffset;		// �������� �� ���������� �� ������� �������

		SimpleDialog*		m_psdSec;					// ����� ������� � ����
		SimpleDialog*		m_psdSity;					// ����� ������ � ����
		SimpleDialog*		m_psdFort;					// ����� �������� � ����

	public:
		void	Init();								// ������ ��� ������ (�� ������).

		void	SetSecAddress(SimpleDialog* pSD);
		void	SetSityAddress(SimpleDialog* pSD);
		void	SetFortAddress(SimpleDialog* pSD);

		void	SetVissible(bool flag);
		void	SetDeffOwner(int PID);				// ��������� ������ ������ �� ���������.
		void	SetSectColor(int PID);				// ��������� ���� �� ������.
		void	SetColor(int color);				// ���� ��� alpfa
		void	SetSectAColor(int color);			// ������ ���� � alpha.
		void	SetSectAlpha(int  a);				// ��������� alpha.	
        void	SetDeffColor();						// ��������� deffolt ����
		void	SetSityType();						// ��������� ������ �����.
		void	SetNewArrowOffset(int offset);		// ��������� �������� ��� ��������� ��� ����� �������.
		
		bool	GetData(DString* pDS);				// ������ ���� ������ �� �������.
        bool	ConvertCenters(int wnew,int hnew,int wold,int hold);	// ����������� ���������� ������� �� �������� �����.

		void	DeposeTo(int x,int y);				// �������� ������ � ������.
		void	VissibleIfMouse();
		
		char*	GetSectorName();							// �������� �������. "NONEME"
		int		GetSecDefence();
		int		GetSecPopulation();

		char*	GetResourceName();							// �������� �������. "NO"
		int		GetSectID();								// �������� ID �������.
		int		GetResourceID();							// ������ �������.
		int		GetVillageNUM();							// ���-�� �������.
		int		GetNNeighbor();								// ���-�� �������.
		int		GetNeighborID(int idx);						// ������ idx-���� ������.			
		bool	GetMouseOver();								// �������� �����.
		int		GetDeffOwner();								// �������� ���������� �������.
		int		GetHistoryOwner() { return m_inMapOwner; };
		void	GetSityXY(int &x,int &y);					// �������� ���������� ������.
		void	GetCurXY(int &x,int &y);					// ������������� ����� �������.
		bool	GetEnable();								// ��������� ����������.
		void	GetAroowCoord(int& x,int& y,int& a,int n);
		int		GetNewArrowOffset();						// �������� �������� ��� ��������� ��� ����� �������.
		SimpleDialog*	GetSityPic();						// ��������� �� �������� ��������.

		void	SetVisible(bool	state);						// ��� - ���� ��������� �������.
		void	SetEnable(bool	state);						// ��� - ���� ����������.
		void	SetSectVisible(bool state);					// ������ ��������� �������.
		void	SetSityVisible(bool state);					// ������ ��������� ������.
		void	CreateHint();								// ������� ���� ��� �������.
		void	CreateHint2();								// ������� NULL-���� ��� �������.

		SAVE( CSectStatData );
			REG_MEMBER	( _int, m_inPopulation	);
			REG_MEMBER	( _int, m_inDefence		);
		ENDSAVE;

};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    CSectDynmData    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ������ ������ ���������� �� ����� ����.
class	CSectDynmData : public BaseClass		//CError, public CStorageData		
{
	public:
		CSectDynmData();
		~CSectDynmData();

		int		m_inOwner;					// �������� �������.

		void	SetOwner(int PID);			// ������ ���������.
		int		GetOwner();					// �������� ������ ���������.

		SAVE( CSectDynmData );
			REG_MEMBER	( _int, m_inOwner );
		ENDSAVE;
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////    CSectData    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ������ ������ �� ���� ��������.
class	CSectData	: public BaseClass		//CStatusDRV, public CError, public CStorageData 
{
	public:
		CSectData();
		CSectData(char* DataFileName);
		~CSectData();

		DialogsSystem*	m_pdsMenu;				// ���� ����.

		int				m_inSecOffset;			// �������� �� �������� � ���� ��� ��������.
		int				m_inSityOffset;			// �������� �� �������� � ���� ��� �������.
		int				m_inNOS;				// ���-�� ��������.
		int				m_fWM,m_fHM;			// ������� ����� �� ��������.

		ClassArray<CSectStatData>	m_pssdDT;	// ����������� ������  �� �������.		//CSectStatData*	m_pssdDT;
		ClassArray<CSectDynmData>	m_psddDT;	// ������������ ������ �� �������.		//CSectDynmData*	m_psddDT;	
		
		int				m_inNewArrowsNum ;
		GPPicture*		m_pgppArows[ MAX_ARROW ];
		GPPicture*		m_pgppNewArrows[ MAX_ARROW * MAX_ARROW ];
		GPPicture*		m_pgppNewArrowsShad[ MAX_ARROW * MAX_ARROW ];				
		
		static	int		m_LastLMCSecID;							// ����� ���������� ������� �� �������� �������� ������.

	public:
		void	Init();											// ������ ��� ������ (�� ������).

		void	UPDATE();

		void	CreateTempDataFiles();							// ������� ����� � ������� �� ��������.
		void	AddArrowPictures(LocalGP* pPic,int sprID);		// �������� �������� ���������.
		void	AddNewArrowPictures(LocalGP* pPic,int n);		// �������� �������� ����� ���������.
		void	ReCountCoord(int wnew,int hnew);				// ��������� ���������� � ������� �����.
		void	DeposeTo(int x,int y,bool setDefColor);			// �������� ������ � ������.
		void	SetSecOnMap(int offset, DialogsSystem* psdMenu);
		void	SetSityOnMap(int offset, DialogsSystem* psdMenu);
		
		void	DriveMouse(int x,int y);
		void	ViewNeighbor(int SecID,int Player,CPlayer* ppPlayer); // ��������� �������� + ������� ������.
		void	ViewNeighbor(int dx,int dy,int SecID,int Player,CPlayer* ppPlayer); // ������� ��������� //������� ��������� �������
		void	InVisibleNeighbour(int SecID);

		int		GetSectorsNum();								// ���-�� ��������.
		void	GetSecSityXY(int ID,int &x,int &y);				// ���������� ������ �� id �������.
		int		GetNeighborsNum(int id);						// �������� ���-�� ������� �������.
		int		GetNeighborsIDs(int id,int* ids,int n);			// �������� ������ ������� n, ����������� ��������� ������� �������
		int		GetSecOwner(int id);							// ����� ������ ���������.
		int		GetHistoryOwner(int id);

		int		GetPlayerSect(int PID);							// ���-�� �������� ������.
		int		GetPlayerSect(int* pSecID,int n,int PID);		// �������� ������ �������� ������.
		int		GetSecIDbyOwner(int NI);						// ������� "�����-��" ������ �� ��������� ����������.

		int		GetMouseOverSect();								// ������ ��� ������� ������ �����.
		SimpleDialog*	GetSityPic(int id);
		        				
		void	SetVisible(bool	state);							// ��� - ���� ��������� �������
		void	SetEnable(bool	state);							// ��� - ���� ����������.
		void	SetSecVisible(int id,bool state);				// ������� ������ ������� ��� ���������.
		void	SetSecColor(int id,int color,bool visible);		// ������ ���� ������� � ��������� ���������.
		void	SetDeffColor();									// ��������� ������� � ����� ����������.
		
		void	setRedDip	 (int PlayerId,int* pPID,int n);	// pPID - ������ ������� ������� ���� ���������.
		void	setGreenDip	 (int PlayerId,int* pPID,int n);	// pPID - ������ ������� ������� ���� ���������.
		//void	setRedDip(int* pPID,int n);		//void	setGreenDip(int* pPID,int n);
		void	setOwnerColor(int PID);							// ��������� ��� ������� ������ PID ��� ������.
		void	setOwnerColor(int PID,int color,int alpha);		// ��������� ��� ������� ������ PID ������.
		void	setArrowCoord(int x1,int y1,int x2,int y2,int id); // ��������� ���������� ���������� ����������.
		void	setArrowVisible(bool state);					// ��������� ���������.

        void	CreateHint();									// ������� ���� ��� �������.
        void	CreateHint2();									// ������� NULL-���� ��� �������.
   
		char*	GetSectorName(int id);							//
		int		GetSecDefence(int id);							//
		int		GetSecPopulation(int id);						//
		int		GetSecVillageNUM(int id);						// ���-�� �������.

		char*	GetResourceName(int id);						// �������� �������. "NO"

		int		GetResourceID(int id);							// ������ �������.
		int		GetNNeighbor(int id);							// ���-�� �������.
		int		GetNeighborID(int id,int idx);					// ������ idx-���� ������.			
		int		GetDeffOwner(int id);							// �������� ���������� �������.
		void	GetSityXY(int id,int &x,int &y);				// �������� ���������� ������.

		void	SetSecDefeatDefence( int id ) ;
		void	SetSelectedColor( int id , bool select ) ;		

		SAVE( CSectData );
			REG_CLASS	( CSectStatData	);
			REG_CLASS	( CSectDynmData	);
			REG_AUTO	( m_pssdDT		);
			REG_AUTO	( m_psddDT		);
		ENDSAVE;
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CSectorMenu    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ���� �������� � ��������� ���� �� ��������.
class	CSectorMenu : public CStatusDRV, public CError
{
public:
	CSectorMenu();
	CSectorMenu(CSectData* psdSectors);
	~CSectorMenu();

	//For create menu.
	void		CreateMiniMaps(DialogsSystem* pdsMenu,LocalGP* plgpMap,int size);	//Add MiniMap in menu and class. 
	void		CreatePictureAndButton(DialogsSystem* pdsMenu,LocalGP* plgpPic,LocalGP* pFlags);//Add all picture and button for menu.

	//Manipulation with menu.
	void		SetMenuPosition();			//Recount all menu elements coord.
	//void		SetMenuData(int sectID);	//Set data about sector in menu.
	
	void		SetMenuData( int sectID , CPlayer* pPlayer[MAX_PLAYERS] );	//Set data about sector in menu.
  	
	void		SetEnabled(bool status);	//Enabled status.
	void		SetVisible(bool status);	//Visible status.

	void		SetEnabledState( bool status ) ; //grey

	bool		CheckDefenceButton( CDiplMenu* pDiplMenu , CPlayer* pPlayer , int sectID , int playerNation );
	bool		CheckDiversionButton( CDiplMenu* pDiplMenu , CPlayer* pPlayer , int sectID , int playerNation );

	bool		ShowDiversionActions( DialogsSystem* pMenu , int x0, int y0 ); //grey
	void		SetDiversionTexts( int def ); //grey

	int				m_inCurSectID;			//Current drawing sector ID.

	//Parametrs for menu.
	int		X0;		//Menu X - start.
	int		Y0;		//Menu Y - start.
	int		dX;		//Standoff by X coord.
	int		dY;		//Standoff by Y coord.
	int		delta;	//Standoff between menu objects.

	//Data for update.
	CSectData*		m_psdSectData;			//Point to all sectors (data).

	//All graphics for menu.
	//SimpleDialog*		m_psdSectName;	//Sector name button
    //SimpleDialog**		m_ppsdMiniMap;	//Points to objects in menu, MiniMap picture.
    GPPicture*			m_ppsdMiniMap;	//Points to objects in menu, MiniMap picture.
	//int					m_inNMap;		//Number of MiniMap in class.
	//SimpleDialog**		m_ppsdFlag;		//Point to players flags.
	GPPicture*			m_ppsdFlag;		//Point to players flags.
	//int					m_inFlagN;		//Number of flags.
	//SimpleDialog**		m_ppsdPic;		//Points to picture.
	//int					m_inPicN;		//Number of picture.
	//SimpleDialog**		m_ppsdTextBt;	//Point to text button.
	//int					m_inTextBtN;	//Number of text button.
	//GP_Button*			m_pgpbBT01;		//Point to first button.
	//GP_Button*			m_pgpbBT02;		//Point to second button.
	TextButton*		m_ptbCounName ;		// country Name text
	TextButton*		m_ptbSectName ;		// sector Name text
	TextButton*		m_ptbPopulation ;	// sector Population text
	TextButton*		m_ptbRecruits ;		// sector Recruits text
	
	GPPicture*		m_pgppMMapBord ;	// mini-map border
	GPPicture*		m_pgppResPict ;		// resource pict

	GP_TextButton*	m_pgpbDefence ;		// Defence button
	TextButton*		m_ptbDefLevel ;		// Defence Level text

	GP_TextButton*	m_pgpbDiversion ;	// Diversions button

	GPPicture*		m_pDeLimit;			// �����������
    
	int				m_iResNum;			// ���-�� ���-�
	TextButton*		m_ptbResADD[7];		// + resource[i]

	//
	static const int	m_iDTN = 4;
	GP_TextButton*		m_ptbDText[m_iDTN];	// diversion part
	//TextButton*		 m_ptbDText[m_iDTN];	// diversion part
	//

	//Halp functin.
    void	Init();						//Set all parametrs in NULL.

};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CUnit    ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ����� � ������ ������� �� ������� � ������.
class	CUnit : public BaseClass //, public CStorageData
	{
	public:
		CUnit();
		~CUnit();

		static int	m_inLastID;			// ��������� �������������� ������.

		_str		m_chName;			// ��� �����, �������� ������.
		int			m_inID;				// ���������� ������.

		int			m_inPlayerID;		// ������ ������ ���� ����������� ����.
		int			m_inSectorID;		// ������ ������� � ������� ��������� �����.
		int			m_inPreviousSecID;	//������ �� �������� ������ ���.

		bool		m_Alive;

		bool		m_Blink;			// ������� for unit - if(fuel>0)
		int			m_iBlinkStat;
		int			m_iBlinkTime;
		int			m_iBlinkLimit;

		SimpleDialog*	m_psdMapPic;	// �������� ������� ������������ �� �����.

		SimpleDialog*	m_psdPotret;	// ������� �����, �������� �����.
		SimpleDialog*	m_psdSector;	// ��������� �� ������, � ������� ��������� ����.

		//*********************TEST***********************
		SAVE( CUnit );
			REG_MEMBER( _int ,  m_inLastID   );
			REG_AUTO  ( m_chName );
			REG_MEMBER( _int ,  m_inID	     );
            REG_MEMBER( _int ,  m_inPlayerID );
			REG_MEMBER( _int ,  m_inSectorID );
			REG_MEMBER(_int  ,  m_inPreviousSecID )
		ENDSAVE;
		//************************************************

		virtual	void	Init();				//�������������.

		static	void	ResetLastID()		{ m_inLastID=0;						};
		static	void	AddLastID()			{ m_inLastID++;						};
		static	int		GetLastID()			{ AddLastID(); return  m_inLastID;	};
		static	void	SetLastID(int id)	{ m_inLastID=id;					};
		
		void	SetVisible(bool	state);			// ������ ���������.
		void	SetEnabled(bool	state);			// ������ �����������.

		void	SetName(char* name);			//������ ���.
		void	setPlayerID(int id);			//������ id ������ �������.
		void	SetSectorID(int id);			//id ������� ��� ���������.
		void	setMapPic(SimpleDialog* psd);	// �������� ������� ������������ �� �����.
		void	SetPotret(SimpleDialog* psd);	// ������� �����, �������� �����.
		void	setMapPicXY(int x,int y);		// ���������� ���������� ����� �� �����.
		void	SetPotretXY(int x,int y);		// ���������� ���������� � ����.
		void	setMapPicRR(SimpleDialog* psd);	//��������� ����� � ��������� ������.
		void	setMapPicRR();					//��������� ����� � ��������� ������.
		void	setMapPicLL(SimpleDialog* psd);	//��������� ����� � ��������� ������.
		void	setMapPicLL();					//��������� ����� � ��������� ������.
		void	setVisibleMapPic(bool stat);	// ��������� ��������� ������ �� �����.
		void	SetSector(CSectData* pSec,int id=-1);	// ���������� ������.
		void	SetSector(CSectData* psdS);		// ��������� ������ ������, ����� ������.
		void	setDeffColor();					// ������ ���� ������ - ���������.

		int				getSectorID();			// ������ ������� � ������� ��������� ����.
		int				getPreviousSecID();		// ���������� ������ � ������� �� ����������.
		int				getUniqueID();			// ���������� ������.
		int				GetPlayerID();			// ������ ������ ���������.
		char*			GetName();				// ��� �����, �������� ������.
		SimpleDialog*	getMapPicPt();			// ��������� �� ������� SipleDialog � ����.

		bool			CheckBlink();
		void			PrepareBlink();

		virtual	void	SetOnClickMap(pCULL_FUN_CLICK pf,int p);	//������ ���������� ����� �� �������� ����� �� �����.
		
		virtual	void	UpdatePic(CSectData* psdData);
		virtual	void	MoveWithMouse();
		
		bool	operator=(CUnit& other);

	//	virtual	bool	Load()				{ return true; };
	//	virtual	bool	Save()				{ return true; };			
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CSquard    //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ����� � ��� ��� � ��� ��������.
class	CSquard	: public BaseClass
{
	public:
		CSquard();
		~CSquard();
	
		int		TypeGlobID;					// ���������� ������ ���� ������.
		int		SQ_MAX;						// ������������ ���-�� ������� ������� ����.
		int		SQ_NUM;						// ���-�� ������� ������� ����.

		LinearArray<int,_int>	Kills;		// NKills

		_str	TypeName;			// �������� ���� ������.		// �������������� ������� ������.
		int		MeleeDMG;			// ����������� �� ���������� �����.
		int		RifleDMG;			// ����������� �� ��������.
		int		GrenadeDMG;			// ����������� �� �������.
		//int Shield;		int StartMorale;
		//int		Cost[8];			// ��������� ������ ������.		//int		Price;

		//struct  SDESCRIPTION {
		//	_str	TypeName;			// �������� ���� ������.
		//	int		MeleeDMG;			// ����������� �� ���������� ����.
		//	int		RifleDMG;			// ����������� �� ��������.
		//	//int	GrenadeDMG;			// ����������� �� �������.
		//	//int	Shield; int	StartMorale;
		//	//int	Cost[8];			// ��������� ������ ������.		//int		Price;

		//	void	Init();
		//	void	UPDATE(int NIndex);	// �������� �� NIndex (TypeGlobID) ���� ������.
		//}		DESCRIPTION;			// �������������� ������� ������.
		//new

		void	Init();							//�������� ��� ��������.
		
		//void	setFreeSQinArmy(int N);			//���-�� ��������� ���� � �����.
		void	setTypeGlobID(int id);			//��������� ���������� ������ ����.
		void	setSQ_MAX(int n);				//��������� ������������ ���-�� ������ ������� ����.
		void	setSQ_NUM(int n);				//��������� ���-�� ������� ������� ����.
	        	
		int		getTypeGlobID();				//�������� ���������� ������ ���� ������.
		int		getSQ_MAX();					//�������� ������������ ���-�� ������ ������� ����.
		int		getSQ_NUM();					//�������� ���-�� ������� ������� ����.

		//bool	addSQ();						//����������� �������� ���� �����.
		bool	addSQ(int freeSQNum);			//����������� �������� ���� �����.
		void	killSQ()		{ if( SQ_NUM > 0 ) SQ_NUM--; };					

		void	DecreaseARMY(bool st);			//���-�� �������� �����, ��������� ��� ���������.
		void	DecreaseARMY(int n);			//���-�� �������� �����, ��������� ��� ���������.
		void	IncreaseARMY(int n);			//����� �����

		void	UPDATE();

		CSquard	operator=(CSquard& sq);			//�������� ����������.

		SAVE( CSquard );
			REG_MEMBER( _int, TypeGlobID		 );
			REG_MEMBER( _int, SQ_MAX			 );
			REG_MEMBER( _int, SQ_NUM			 );
			REG_AUTO  (       Kills );				
		ENDSAVE;
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CArmyLimit    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ������ ������ �� ������� ��� ������ ������.
struct	SSquardData {
	SSquardData()		{ unitName = "NONAME";/*strcpy(unitName,"NONAME");*/ maxSquards=0; curID=0; };

	void	operator=(SSquardData& sdOld)	{	unitName = sdOld.unitName;
												/*strcpy(unitName,sdOld.unitName);*/ 
												maxSquards=sdOld.maxSquards; 
												curID=sdOld.curID;				};

	_str	unitName;			//������������ ������.
	int		maxSquards;			//������������ ����� �������.
	int		curID;				//������� ������ � ������� NATIONS[0].Mon[curID]
};
//----------------------------------------------------------------------------------
struct	SRank {
	SRank();
	~SRank();

	void		Init();
	void		DeleteTYPE(int cid);			//�������� �� ������� ������.
	void		DeleteTYPE();					//�������� ���.
	void		CreateNTYPES(int cid,int n);	//������� ��� ������ � �������� cid, n ����� �������.
	void		AddTYPE(int cid);				//�������� ������ ���� ���.
	
	_str			rankName;		//�������� ������.
	int				rankID;			//������ �����.
	struct SCUType {
		_str			country;		//������������ ������.
		int				maxSquards;		//���-�� ������� ���� ����� ����������� ��� �������������.
		SSquardData*	TYPES;			//������ ����������� ��� �������������.
		int				TN;				//���-�� ������ ����� �������.

		void	DeleteTYPES();			//���������� ������.
		void	DeleteZeroTYPE();		//���������� ���� � ����������� ����� = 0.
		void	operator =(SCUType& cut);
		void	operator+=(SSquardData& sd);
	}				TYPES_CONT[MAX_COUNTRYS];
};
//----------------------------------------------------------------------------------
class	CArmyLimit
{
public:
	CArmyLimit(char* fName);

	void	SetNSqdNID   ( int rankID, int countryID, int n, int id ); //�������� ������������� ���� n-�� ������������ ������.

	int		GetMaxDiffSqd( int rankID, int countryID );	//�������������� ���-�� ������ ������.
	int		GetMaxSqd    ( int rankID, int countryID );	//������������ ���-�� ������� � �����.
	int		GetNSqdNum	 ( int rankID, int countryID, int n ); //������������ ���-�� ������� n-�� ������������ ������.
	char*	GetNSqdID    ( int rankID, int countryID, int n ); //������������� ���� n-�� ������������ ������.
	int		GetNSqdNID   ( int rankID, int countryID, int n ); //�������� ������������� ���� n-�� ������������ ������.
protected:
	SRank	HERO_RANKS[MAX_RANKS];
private:
	bool	Load(char* fName);
	void	DelZoroTYPE();
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    CHERO_MINI_MENU    //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class	CHERO_MINI_MENU
{
public:
	CHERO_MINI_MENU();

    void				INIT();												// ��������������.
	void				INIT( int x0, int y0 );								// ��������������, � ���������� X0,Y0.

	void				AddBTNS_MENU( LocalGP*	pGP, DialogsSystem*	pMN );	// �������� � ���� ������.
	void				AddSTAT_MENU( LocalGP*	pGP, DialogsSystem*	pMN );	// �������� � ���� �������.
	void				AddFLAG_MENU( LocalGP*	pGP, DialogsSystem*	pMN );	// �������� � ���� �������.
	
	void				SetMPos		( int x0, int y0 );						// ������ ���������� ����� ������� ����.
	void				UpdateMenu	();										// ��������� �� ��������� ����� �������� �� � ��.
	void				CheckVisible();										// �������� ���������.

	void				SetVisible	( bool  state );						// ���������� ��������� ���������.
	void				SetEnabled	( bool  state );						// ���������� ��������� Enabled.

	void				SetName		( char* name );							// ������ ��� �����.
	void				SetRank		( int	rank );							// ������ ������ ������.
	void				SetStat		( int	stat );							// ������ ������� ������.
	void				SetFlag		( int   flag );							// ������ �����.
	void				SetAll		( CHero* pHero ) ;						// ������ ��� ��������� ����� � ����.
	void				SetAll		( char* name, int rank, int stat, int flag , CHero* pHero );// ������ ��� ��������� ����� � ����.

	void				SetAllBtn	( bool	stat );							//������ ���������� � �������� ��� �����.

	void				GetMPos		( int& x0, int& y0 );					//�������� ���������� ����.
	char*				GetName		();										//�������� ��� �����.
	char*				GetRank		();										//�������� ������ ������.
	int					GetStat		();										//�������� ������ ������ ������.
	int					GetFlag		();										//�������� �����.
	void				GetAll		( char* name, char* rank, int& stat, int& flag );	//�������� ��� ���������.

	void				ArrangeIcons();				// ��������� ���������� ������ ���� �� X0, Y0.
	
	void				SetStatVisible(bool stat);
	void				SetFlagVisible(bool stat);

public:
	CHero*				m_pHero;

	int					X0,Y0;						//��������� ������� ����������.

	_str				lpszName;					// ��� ������.
	_str				lpszRank;					// ������ ������.

	int					iSQ_NN;						// SQ-num
	int					iSQ_MAX;					// max SQ-num

	int					iExpa ;						// expa
	int					iExpPercent ;				// exp-percent for next level

	int					iNLExpa ;					// next level expa

	int					inStat;						// ������� ��������������.
	int					inFlag;						// ����� ������.

	GPPicture*			pgppMenuBack ;				// BackGround for HERO_MINI_MENU
	GPPicture*			pgppStat[MAX_H_STATUS];		// ������� ��� ������.
	GPPicture*			pgppFlag[MAX_PLAYERS+1];	// ������� �������.

	TextButton*			pgptbName;					// ��� ������.
	TextButton*			pgptbRank;					// ������ ������.
	
	TextButton*			ptbExpa ;					// expa

	DialogsDesk*		pgppExpBar ;				// expa-BAR-DESK!!!
	GPPicture*			pgppExpBarPIC ;				// expa-BAR

	GPPicture*			pgppExpBack ;				// BackGround for expa
	
	TextButton*			ptbSQNum ;					// SQ-num

	TextButton*			ptbVictN ;					// Victories num
};

//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    CHERO_PORTRAIT    ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// �������� ������, �������� � �������.
class	CHERO_PORTRAIT
{
public:
	CHERO_PORTRAIT();
	~CHERO_PORTRAIT();

	void		addPortrait( LocalGP* pGP, LocalGP* pGPs, int nat );	//�������� ��� ����������� ������ ��������.
	void		addToMenu  ( DialogsSystem* pMenu );	//�������� �������� � ����.

	SimpleDialog*	getSmallP( int p, int n );	//�������� ��������� �� ��������� �������,p-�����,n-�����.
	SimpleDialog*	getBigP  ( int p, int n );	//�������� ��������� �� ������� �������,p-�����,n-�����.

	LocalGP*		getLGPSmallP( int p );
	LocalGP*		getLGPBigP  ( int p );

	void			setVisibleS(bool	state);
	void			setVisibleB(bool	state);
protected:
	void		Init();
	void		CreateArrays();
	void		DeleteSmallP();
	void		DeleteBigP();
	void		DeleteAll();
private:
	LocalGP*	pPORTRAIT  [ MAX_PLAYERS ];	//�������� ������.
	LocalGP*	pPORTRAITs [ MAX_PLAYERS ]; //��������� �������� ������.

	GPPicture*	ppPORTsmall[ MAX_PLAYERS ][ MAX_HEROES ];	//��������� ��������, � ����.
	GPPicture*	ppPORTbig  [ MAX_PLAYERS ][ MAX_HEROES ];	//������� ��������, � ����.

	//GPPicture**	ppPORTsmall[ MAX_PLAYERS ];	//��������� ��������, � ����.
	//GPPicture**	ppPORTbig  [ MAX_PLAYERS ];	//������� ��������, � ����.
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////    CHero    ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ����� � ��� ��� � ��� ��������.
class	CHero : public CUnit
	{
	public:
		CHero();
		~CHero();

	public:
		enum	CHeroState 
		{
				FREE,				//������ ����������.
				CAPTURE				//�������� ������.
		}	m_CurStatus;			//��� ������������ � ������ ������ �����.

		CArmyLimit*	pArmyLimit;		//�������������� ����������� ����� ��� �����.


		int		m_inRankID;				//������ ������.
		int		m_inStatID;				//������ �������������.
		int		m_inHeroID;				//����� �������������� �����.
		int		m_inFuel;				//����� �������� ��� �����.
		int		m_inExperience;			//���� �����, ���������� � ����.

		SimpleDialog*	bigPotret;		//������� ������� �����.

		ClassArray<CSquard>	ARMY;		//�����.

		int		m_iFreeSQNum;			//���-�� ��������� ���� � �����.

		int		m_iBatlN;				//���-�� ����  (����������)
		int		m_iVictN;				//���-�� ����� (����������)

		int		m_iFormLost;				// lost   squads (����������)
		int		m_iFormKilled;				// killed squads  (����������)

		void	Init();			//�������������.

		void	SetOnClickMap(pCULL_FUN_CLICK pf,int p=0);	// ������ ���������� ����� �� �������� ����� �� �����.
		void	SetOnRClickMap(pCULL_FUN_CLICK pf,int p=0);	// ������ ������ ���� �� �������� ����� �� �����.
		void	SetHeroID(int id);							// ��������� ������ �������������� �����.
		void	SetBigPotret(SimpleDialog* pPic);			// ��������� ������� �������.
		void	SetVisibleMiniMenu(bool status);			// ��������� ��������� ������ ����.
		
		void	DisableHero();								// ��������� ��� ����� �� �����, + ��� ������� �� �������.
		void	ReEnableHero();								// �������������� �������.�����
		
		//---ARMY---
		void	SetARMY();
		void	SetDefaultARMY();							// ��������� ����� �� ���������.
		void	SetHeroDefeatArmy();
		void	SetSquardNumByType(int n, int type);
		void	AddSQbyArrID(int id);						// �������� ����� �� ������� � �������.
		void	DelSQbyArrID(int id);  
		bool	KillOneSQUARD();							// �������� ���� ����� � �����.

		int		GetARMY_NSQ();								// �������� ���-�� ������� � �����.
		int		GetBranch_NSQ( int branch );				// ���-�� ������� �� ���� �����
		int		GetARMY_FreeNSQ();							// �������� ���-�� �������.������� � �����.
		int		GetARMY_Power( bool realPower=false );		// ���� ������� � �����

		bool	DecreaseARMY(bool st);						// ���-�� �������� �����, ��������� ��� ���������.
		bool	DecreaseARMY(int n);						// �������� �����
		bool	IncreaseARMY(int n);						// ����� �����

		void	SetBattleNKills();
		void	SetNKills( LinearArray<int,_int>& NKills , int type );

		//---MOVEMENT---
		void	SetFuel(int Dist);							// ��������� �������� �� ������������.
		bool	AddFuel(int Dist);							// ��������, ������ ��������� �� ��������.
    
		//---EXPERIENCE---
		void	AddExperience(int EXP);						// �������� ����� �����.
		int		GetExperience();							// �������� �������� ����� �����.
		void	UpdateRankWithExperience();					// �������� ������ �� �������� �������� �����.
		
		//---HERO STATUS---
		void	SetRank(int rID);							// ��������� ���� �����.
		void	RaiseRank();								// �������� � ������.
		void	LowerRank();								// �������� � ������.
		void	AddBattle();
		void	AddVictory();
		int		GetRewardsStatus( int addStat = 0 );

		int		AddFormLost( int nn = 1 );
		int		AddFormKilled( int nn = 1 );
		int		GetFormLost();
		int		GetFormKilled();

		int		GetHeroID();								// �������� ������ �����.
		int		GetRank();
		int		GetStat();
		int		GetFuel();									// �������� ����� ���� �����.
				
		void	UpdatePic(CSectData* psdData);
		void	CreateMiniMenu();							// ����� ���� ��� ����� � ����� ����.
		void	CreateHint();								// ������� ���� ��� �����.
		void	CreateEmptyHint();							// ������ ���� (������� ������ ��� �����)
		SimpleDialog*	GetBigPortret();

		bool	operator=(CHero& other);					// �������� ����������.

		bool	HaveChance() { //�������� - ������ ��� ��� (���� - �������� ��� ���� a la "c'est la vie")
			int  chance = ( rand()%10 )/ 4 ; // ( 10 - 4 ) - "4" ���-�� �����
			if( !chance )  return false;
			return true; 
		}

		////*********************TEST***********************
		//INT_PROPERTY( CHero, GetRank, SetRank);
		//SAVE( CHero );
		//REG_PARENT( CUnit );
		//REG_INDEXPROP( _index, m_inRankID, GetRank, SetRank, RANK_ENUM);
		//REG_MEMBER( _int,	m_inStatID	);
		//REG_MEMBER( _int,	m_inFuel	);
		//REG_MEMBER( _int,	m_inExperience );
		//REG_MEMBER( _int,	m_inHeroID	);
		//REG_CLASS ( CSquard );
		//REG_AUTO  ( ARMY );
		//REG_MEMBER( _int,	m_iFreeSQNum );
		//REG_MEMBER( _int,	m_iBatlN	);
		//REG_MEMBER( _int,	m_iVictN	);
		//ENDSAVE;
		////************************************************
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CResource_BM    /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class	CResource_BM : public BaseClass
{
public:
	CResource_BM();
	~CResource_BM();
public:
	int		wood;		
	int		food;
	int		stone;
	int		gold;
	int		iron;
	int		coal;
	int		recruit;				// ������� ��� ������� � �����.

	SAVE( CResource_BM );
		REG_MEMBER( _int, wood );
		REG_MEMBER( _int, food );
		REG_MEMBER( _int, stone );
		REG_MEMBER( _int, gold );
		REG_MEMBER( _int, iron );
		REG_MEMBER( _int, coal );
		REG_MEMBER( _int, recruit );
	ENDSAVE;

public:
	void	Init();

	void	operator=(CResourceBM& RES) {
		wood	= RES.wood;
		food	= RES.food;
		stone	= RES.stone;
		gold	= RES.gold;
		iron	= RES.iron;
		coal	= RES.coal;
		//recruit = RES.recruit;
	}
	void	operator=(CResource_BM& RES) {
		wood	= RES.wood;
		food	= RES.food;
		stone	= RES.stone;
		gold	= RES.gold;
		iron	= RES.iron;
		coal	= RES.coal;
	}

	void	create(CSectData* pSD,int NI);
	void	updateADD(CSectData* pSD,int NI);

	int		GetResource(int res);
	int		SetResource0(int res, int amount);
	int		SetResource(int res, int amount);
	int		AddResource(int res, int amount);

};
//CStorage_LV : public BaseClass
//{
//public:
//	CStorage_LV(){};
//public:
//	int				Treasury;	//����� ������.
//	CResource_LV	Resource;	//�������.
//public:
//	void	create(CSectData* pSD,int NI);
//	void	updateADD(CSectData* pSD,int NI);
//};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CPlayer    //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
struct	SFOR_PLAYER
	{
	int					mx0,my0,mx1,my1;		// ���������� ����� ��� ��������.
	int					sx0,sy0,sx1,sy1;		// ���������� ����� ������.
	DialogsSystem*		pMenu;					// ��������� �� ����.
    CSectData*			pSectData;				// ��������� �� ������� �� ����� ������������ �������.
	LocalGP*			pMapPic;				// �������� ��� ��������� �� �����.
	CHERO_PORTRAIT*		pPORTRAIT;				// �������� ������.
	CHERO_MINI_MENU*	pHMiniMENU;				// ���� ���� ��� �����.
	CArmyLimit*			pArmyLimit;				// ������ �� ������� ��� ����� �����.
	pCULL_FUN_CLICK		pfHeroClick;			// ���� �� �����, ������ �������.
	pCULL_FUN_CLICK		pfHeroRClick;			// ������ ���� �� �����, ������ �������.
	};
//----------------------------------------------------------------------------------
// ������ � �������� �� ������ ��������� � ���������� �������.
class	CPlayer	: public CStatusDRV, public CError, public CStorageData
	{
	public:
		CPlayer();
		CPlayer(int ID);							// ������ ������ ������ 1..7
		CPlayer(int ID, char* Name);				// ������ ������ ������ 1..7, � ����� ���.
		~CPlayer();
	public:
		//BASE
		int				m_PlayerID;					// ������ ������, 1..7
		_str			m_chPlayerName;				// ��� ������.

		GPPicture*		m_pgppPortH[ MAX_HEROES ];	// ������ ���������� �� ������ �������� ������.

		DialogsDesk*	m_pDD;						//
		CSectData*		m_psdSectData;				// ��������� �� ������ ��������.

		static SFOR_PLAYER  DataForAddHero;
				
		// RESOURCES
		CResource_BM	Resource;					// ������� ������.	

		// HEROES
		int				m_inHeroNum;				// ���-�� ������ ������.
		CHero*			m_phrHero;					// ��������� �� ������ ������ ������.
		CHero			m_OneHero;					// ����� ������ ( -Emergency Hero- )
		//CHero			m_phrHero[ MAX_HEROES ];	// ������ ������ ������.

		// Player Individual Characteristics
		int				m_Aggressive ;				// �����.������������� ������
		bool			m_Alive;					// ������ �� ����������

		// HEROES
		//CHero*		m_phrHero;					// ��������� �� ������ ������ ������.
		// SQUADS
		//CSquard*		m_psdSquard;				// ��������� �� ������ ������� ������.
		//int			m_inSquardNum;				// ���-�� ������� ������.

	protected:
		void	Init();							//����������������.

		bool	LoadSquard();					//��������� ������ �� ������� ������.
		bool	SaveSquard();					//��������� ������ �� ������� ������.
		bool	LoadHero();						//��������� ������ �� ������ ������.
		bool	SaveHero();						//��������� ������ �� ������ ������.

		bool	InitMemNHero	(int n);		//�������� ������ ��� n ������.
		bool	InitMemNSquard	(int n);		//�������� ������ ��� n ������.
		//void	DeleteHeroes();					//������� ���� ������.
		void	DeleteSquard();					//������� ��� ������.

		bool	IsHeroIDPresent(int id);		//������������ � ������ ������ ����� ��� ���.
	public:
		//BASE
		bool	Load();							//��������� ��� ������ �� ������ ������.
		bool	Save();							//��������� ��� ������ �� ������ ������.
		bool	UpdateSectID();					//�������� ��������� �� ������� ��� ������ � ������� �� ID ��������.
		bool	UpdatePic();					//�������� ��� ��������.
        void	SetSetcData(CSectData* psd);	//��������� ��������� �� �������.
		
		void	AddPortH();						//�������� �������� ������ � ���� ������������.
		void	SetPortHVisible(bool state);	//��������� ��������� ��� ����.
		void	ShowPortH();					//���������� ������ ������������ ������.

		void	SetVisible(bool	state);			// ������ ���������.
		void	SetEnabled(bool	state);			// ������ �����������.

		bool	SetPlayerID(int ID);			//������ ID ������.
		bool	SetPlayerName(char* Name);		//������ ��� ������.
		bool	AddMoney(int m);				//��������� ��� ������ ������������ ���-�� ����� � ������.

		char*	GetPlayerName();				// ��� ������.
		int		GetPlayerMoney();				// ������ ������� � ������ �����.
		
		bool	IsUnitInSect(int sid);			//���� �� ����� ����� � ���� �������.
		
		//SQUARD
		bool	AddSquard();					// �������� �����.
		bool	DeleteSquard(int ID);			// ������� �����.

		//HERO
		int		AddHero();									// �������� �����.
		bool	AddHero(DialogsSystem* pds,LocalGP* mlgp,int mid);
		bool	AddHero(int sectID, int heroID=-1);			// �������� ����� � ��������� � ������ sectID
		void	SetNewHeroData(int heroID=-1);
		bool	DeleteHero(int ID);							// ������� �����.
		void	TryRetreat(CHero* pH,bool die);				// ����������� ��������� ���� �� �����.
		void	SetHeroMiniMenuVisible(bool status);		// ������ ����������� ���� ������.
		void	TryShowSPortret(int idH);
		CHero*	GetHeroByPID(int pid);						// �������� ��������� �� ����� �� ������������� �������.
		CHero*	GetHeroInSector(int sid);					// �������� ����� ������������ � �������.
		bool	IsHeroPresentInSector(int id);				// ���� �� ����� ������ � ���� �������.
		void	SetHeroFuel(int HID, int Dist);				// ������ ����� ��������.
		void	SetHeroesFuel(int Dist);						// ������ ������ ��������.
		void	SetDefaultARMY(int heroID=-1);				// ��������� ����� �� ���������.
		int		GetPlayerMoves();							// �������� ���-�� ����� � ������ - Sum( m_inFuel[i] )

		int		GetNewHeroID();
		void	DeleteHeroes();								// ������� ���� ������.
		
		//RESOURCES
		void	SetRecruit();
		int		GetResource(int res);
		int		SetResource(int res, int amount);
		int		AddResource(int res, int amount);
		void	UpdateResources();
		bool	EnoughResourceAmount( int res , int amount );
		bool	BuyResource( int res );
		bool	BuyResourceForAmount( int res , int amount );
		bool	AddResources( int wood, int food, int stone, int gold, int iron, int coal );

		// Aggressive
		int		GetAggressive();		
		int		SetAggressive( int value );
		int		GetAggressiveDefault();
		int		SetAggressiveDefault();

		// Player Power
		int		GetNation_Power() //new [16.03.2005]
		{
			int  power = 0;
			power += GetSecDef_Power();
			power += GetArmies_Power();
			return  power;
		};

		int		GetSecDef_Power() //new [16.03.2005]
		{
			const int  DefPowerMult	= 4;

			int  power = 0;
			for(int sec=0; sec < m_psdSectData->GetSectorsNum() ; sec++) 
				if( m_psdSectData->GetSecOwner(sec) == m_PlayerID ) {
					power += ( m_psdSectData->GetSecDefence(sec) + 1 ) * DefPowerMult;		// +defence +SecNum
					power += SEC_DEF_POWER( m_psdSectData->GetSecDefence(sec) );				// +SecDefPower
				}
			return  power;
		};

		int		GetArmies_Power() //new [16.03.2005]
		{
			int  power = 0;
			for(int h=0; h < m_inHeroNum ; h++) {							
				CHero*	pH = &(this->m_phrHero[h]) ; //������ ������
				if( pH->m_Alive )
					power += pH->GetARMY_Power(true);		// +army power
			}
			return  power;
		};

		bool	HaveChance() {
			int  chance = ( rand()%10 )/ (10 - m_Aggressive) ; // ( 10 - 4 ) - "4" �����.������������� ������!
			if( !chance ) return false;
			return true; 
		}

		bool	AGetTrue() { return true; }
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CPERSONAL    //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ���� ������ ������.
class	CPERSONAL 
{
public:
	CPERSONAL();
	CPERSONAL( int PlayerID );
	CPERSONAL( int PlayerID, int HeroID );
	~CPERSONAL();
	void			INIT();

	void			setPLAYERS(CPlayer** pPL);		//�������� ��������� �� ������� ��������� (����).
    void			setPlayerID( int id );			//��������� ������ ��� �������� ������ ����.
	void			setVisible( bool state, bool type=true );			//���������� ��������� ���������.
	void			setUVisible( bool state );							//��������� ��������� ������.
	//void			setFarme( DialogsSystem* pMenu, LocalGP* pFR );		//�������� �����.
	void			setFarme(  DialogsSystem* pMenu, LocalGP* pFR , CArmyLimit* pAL ); //�������� ������.��-��.
	void			setUserClick(pCULL_FUN_CLICK pf, pCULL_FUN_CLICK pfR );	//���� �� ������ ���� ������.
	void			setUnis( DialogsSystem* pMenu, CArmyLimit* pAL );	//�������� �������� ������.

	void			UpdateCoord();					//��������� ���������� ���� �������.
	void			UpdateUCoord();					//��������� ���������� ��� �������� ������.
	void			UpdateData();					//�� ������ �������� ��� ������.
	void			UpdateData(int heroPID);		//�������� �� ������������� ������� �����.
	void			UpdateSQ_NUM();					//�������� ���-�� ������� � �����.

	void			UpdateInfo( CHero* pH ); 
	void			CreateUnitsHint( SimpleDialog* psd , int Nation , word NIndex );
	
	bool			CheckUnitsPages();

	bool	setEnabledButtons( bool state );//!!!new (VERY MUCH:) add if veeery need it!)

public:

	bool			m_Visible;						// ��������� 

	int				m_PlayerID;						// ������� ������������ �����.
	int				m_HeroID;						// ������� ������������ ����� ������ m_PlayerID.
	int				m_iHumanNat;					// player nation from Save
	int				m_CurHeroId;
	
	int				m_iBlinkTime;
	bool			CheckPageBlink();


	static const int	m_nFH  =  4;				// ���-�� ����������� �����.
	
	static const int	m_nFU  = 20;				// ���-�� ����� � ������� � ����.
	static const int	m_nBT  = 2*m_nFU;			// ���-�� ������ � ������.
	static const int	m_nTXT = 6;					// ���������-���� �����

	static const int	m_nPgs = 3;					// ���-�� ������ � ������.

	GPPicture*		m_pgppFrameH[ m_nFH  ];			// ����������� �����.
	TextButton*		m_ptbHeroPar[ m_nTXT ];			// ���������-���� �����.

	GPPicture*		m_pgppFrameU[ m_nFU  ];			// ����� ������������ ������ � �����.
	TextButton*		m_ptbFrameUN[ m_nFU  ];			// ������� ���-�� ������ � �����.
	GP_Button*		m_pgpbButton[ m_nBT  ];			// ������ � ������.
	
	int				m_ActPage;						// �������� �������� ������.
	GP_Button*		m_pgpbPages[  m_nPgs ];			// ������ � ��������� ������.
	GP_Button*		m_pgpbPagBak[ m_nPgs ];			// ������ � ��������� ������.

	GPPicture*		m_pgppBigPort;					// ������� ������� �����.
	DialogsDesk*	InfoDesk;						// DialogsDesk for text info about hero
	TextViewer*		HeroDesc;						// textview
	TextButton*		ptbHeroDesc;					// text info about hero
	
	SimpleDialog*	m_ppUnitsPic[ m_nFU ] ;			// ������� ������������ �����.

	struct SUNIT_TYPE {
		DynArray<SimpleDialog*>		UNIT_PIC;		// �������� ������.
		DynArray<int>				UNIT_NID;		// ������� �������� ������.
	}				m_utUNITS;

	CPlayer*		m_pPLAYERS[MAX_PLAYERS];		// ��� ������ �� �����.
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CPicesPict    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ������������ �����.
class	CPicesPict : public CStatusDRV, public CError
	{
	private:
		CPicesPict();
	public:
		CPicesPict(DialogsSystem* pMenu,int NX,int NY,int FrOffset);
	//	~CPicesPict();
	private:
		DialogsSystem*	m_pMenu;			//���� ����.
		int				m_NX,m_NY;			//���-�� ��������, ������ � ������.
		int				m_FrOffset;			//�������� �������� � ����.
		int				X0,Y0;				//����� ������� ���� ������.
		int				m_x0,m_y0;			//����� ������� ���� �����.
		int				m_H,m_W;			//������ � ������ ������.
		int				m_PW,m_PH;			//�������� ������ � ������ ��������.

		int				m_mxo,m_myo;		//������� ���������� �����.
		bool			m_bBegMove;			//������ �������� �����.
		
		//grey
		GPPicture*	pgppMapBorder ;		// Map Border
		
	private:
		void	CalculateCoord();			//���������� ��������.
		bool	AddX(int dx);		//��������� �� x
		bool	AddY(int dy);		//��������� �� y
		void	MoveX(int dx);
		void	MoveY(int dy);
	public:
		void	SetScreen(int x0=0,int y0=0,int H=768, int W=1024); //���������� ������ � ������ ������.
		void	GetMapSize(int &dx,int &dy,int &w,int &h);

		void	ScrollPic(int mx,int my);
		void	MoveWithMouse();		//����� ����� �� ������.
		void	SetVisible(bool	state);		//��� - ���� ��������� �������
		void	SetEnable(bool	state);		//��� - ���� ����������.

		//**** grey ****//
		bool	SetPosition(int dx,int dy); 
		void	CreateMapBorder();
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CROOTDRIVER    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class	CROOT_CONST_DATA
{
	public:
		CROOT_CONST_DATA();
		~CROOT_CONST_DATA();

		void Init();
	public:
		int					m_inCurTurn;						// ����� �������� ���� ( ��� ���� �������� )
		int					m_inCurActiveAI;					// ��� AI_PROCESS
		int					m_inCurHeroMove;		 			// ��� AI_PROCESS
		int					m_inAttackedNation;
		
		int					m_iBigMap_FOOD_RATIO;				// �����.��� �� ������
		int					m_iBigMap_AI___RESOURCES;			// ���-�� ���-� �� ���p��
		int					m_iBigMap_User_RESOURCES;			// ���-�� ���-� �� ���p��
		float				UpdateResDiv[DIFF_LEVELS_N];		// �������� ��� ������ �������� - f(diff)

		//int					m_iGarrSqN0;						// NSQ for defence = 0
		//int					m_iGarrSqN1;						// NSQ for defence = 1
		//int					m_iGarrSqN2;						// NSQ for defence = 2
		//int					m_iGarrSqN3;						// NSQ for defence = 3
		int					m_iGarrSqN[  MAX_DEFENCE+1 ];			// NSQ for defence levels
		int					m_iGarrAdd[  MAX_DEFENCE+1 ];			// add NSQ by rule
		int					m_iRebelAdd[ MAX_DEFENCE+1 ];			// revolted

		int					m_iMinSecDefPower;					// min sector defence power (for  def=0)
		int					m_iAddSecDefPower;					// add sector defence power (from def=1)

		int					FormKilledNum;						// attacker
		int					FormKilledNum2;						// defender
		int					QuestEXP;
		int					QuestNum;
		int					QuestComplete;
		int					QuestSQNum;
		int					IncomeRes[10];
		int					IncomeRes2[10];
		int					AddAIRes[10];						// cheats add-ress for AI

		int					Path[1000][1000];					// finded paths by Path Maker

		char*				m_chNationID[ 2*MAX_PLAYERS ];		// nation char id
		DWORD				m_iMapColor[  2*MAX_PLAYERS ];		// ����� ��������� �����
		int					m_iNationID[  2*MAX_PLAYERS ];		// Nation identify sprites color

		int					m_CheatsStage;						// current cheats stage
		TextButton*			m_ptbText;							// cheats interface console
};
////////////////////////////////////////////////////////////////////////////////////
// ������������ � ���������� ��������.
class	CROOTDRIVER : public CROOT_CONST_DATA
	{
	public:
		CROOTDRIVER();
		~CROOTDRIVER();
	public:
		DialogsSystem*		m_pMenu;				// ����.

		CPicesPict*			m_pMap;					// �����.
		CSectData*			m_pSect;				// �������.
		CSectorMenu*		m_pSectMenu;			// Sectors menu.
		CHERO_PORTRAIT*		m_pHp;					// Hero portrait.
		CHERO_MINI_MENU*	m_pHmm;					// ���� ���� ��� �����.
		CPERSONAL*			m_pHPers;				// ������������ ���� �����.
		CHeroPersDesk*		m_phpDesk;				// DialogsDesk ��� CPERSONAL 
		CPlayer*			m_pPlayer[MAX_PLAYERS];	// ������.
		CDiplMenu*			m_pDipl;				// ����������.
		CDiplInfoMessages*	m_pMessages;			// ���.���������.
		CBriefing*			m_pBriefing;			// ������� �� ������.
		CBigMapQuest*		m_pQuest;				// global quests on Big Map
		CResPanel_BM*		m_pResPanel;			// resource panel in Main Menu
		CMarketDesk*		m_pMarket;				// global market on Big Map
		CBigMapHelp*		m_pHelp;				// global bigmap help

		CScenarioLV			m_Scenario;				// ������� �������� ��������� �����.
		CPlayerSAVE*		m_pPLAYER_SAVE;			// ��������� �� SAVE ������

		bool				m_bSaveLogs;			// ����� �� log-����� 
		bool				m_bEndGame;			

		//new
		bool	m_ShowHeroMove0;
		bool	m_EnabledMapClick;

	public:
		void	Init();									// ������ ����� ���� ����������.
		int		GetFreePlayerPos();						// ������� ��������� ��� ����������.
		void	PushHeroOnTop(SimpleDialog* psdHero);
		void	PushHeroOnTop2();

		bool	operator+=	(DialogsSystem*		pMenu  );
		bool	operator+=	(CPicesPict*		pMap   );
		bool	operator+=	(CSectData*			pSect  );
		bool	operator+=	(CSectorMenu*		pSectMN);
		bool	operator+=	(CPlayer*			pPlayer);
		bool	operator+=	(CDiplMenu*			pDipl  );
		bool	operator+=	(CHERO_MINI_MENU*	pHMM );
		bool	operator+=	(CHERO_PORTRAIT*	pHp  );
		bool	operator+=	(CPERSONAL*			pHP  );
		bool	operator+=	(CHeroPersDesk*		phpDesk );
		bool	operator+=	(CDiplInfoMessages*	pMessages);
		bool	operator+=	(CBriefing*			pBriefing);
		bool	operator+=	( CBigMapQuest*		pQuest );
		bool	operator+=	( CResPanel_BM*		pResPanel );
		bool	operator+=	( CMarketDesk*		pMarket );
		bool	operator+=	( CBigMapHelp*		pHelp );

		CHero*	GetHeroInSect( int sid ,int NOT_NAT );				// �������� ����� � ���� �������.
		int		GetSectList( int player, int startSecId, int* list );

		// ��������� ��������� (����� �� �������������) 
		void	UPDATE_SCENARIO();									//��������� ���� ������� �� ���������
		// ���������� ��������� �������� �������������� �������� �������� (� �����)
		void	PROCESS(int& g_dXM,int& g_dYM,int& g_WM,int& g_HM, int playerNation, int Diff );//��������� �������� �������c������ �������� �������� 

		void	AI_PROCESS( int AI_NI, int PL_NI );
		void	PROCESS_Strategy(int AI_NI, int PL_NI);
		bool	PROCESS_FillArmy( int AI_NI, int PL_NI, int HID=-1, int PC=100 );
		bool	PROCESS_UpgradeDefence( int AI_NI, int PL_NI, int SecID=-1, int SecNN=-1, int MaxDefID=MAX_DEFENCE );
		void	PROCESS_DiplRelations(int AI_NI, int PL_NI);
		void	PROCESS_Market(int AI_NI, int PL_NI);
		void	PROCESS_Sabotage(int AI_NI, int PL_NI);
		bool	CheckAddHero();										
		bool	Check_Surrender(int AI_NI, int PL_NI);

		bool	CHECK_DEFEAT( int PL_NI );
		void	UpdateResources();

		bool	SimulateBattle(CHero* pAttackHero, CHero* pDefendHero, CSectStatData* pSector); // Grey [17.12.2003]
		int		GetSectorDefPower( int secID );	
		int		FindNearEnemySector( int iCS0, int AI_NI , int MyPower ); // Grey [12.01.2004]
		int		FindPathForHero( CHero* pHero, int iCS0, int AI_NI, bool& attack );	// Grey [17.12.2003]
		int		MakePathSectorToSector( int AI_NI , int secID , int secID2 , int pLimit , int& path_ID );//ret next SecID

		int		AddRecruitsInSectors( int player, int secID, int recruits );
		int		DelRecruitsInSectors( int player, int secID, int recruits );	
		void	AddStatisticsInfo( CResource_BM& OldResAmount );

		void	ShowInGameSector();									
		void	ShowInGameSector( int secID  , float time , bool standard = true , bool slow = true );
		bool	ShowHeroMove( CHero* pHero , int fromSecID , int toSecID );
		void	SetMenuEnabled( bool state );						
		bool	SetHeroMiniMenuVisible(bool state);

		bool	CheckHotKeys();										// �������� ������� ������ � ����!

		bool	CheckRightsForHeroInSector(CHero* pHero);			// �������� �� ����� ���������� � �������: ��� ������ ����� ���� ���� ����� �������

		bool AGetTrue() { return true; }
	};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// ����c� ��� ������ � �������� ������ �� ���������� ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------CDiplomacyData--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ������ ����.������ ���������� �� ����� ����.
class	CDiplomacyData : public BaseClass //, public CError
	{
	public:
		CDiplomacyData(CSectData* pSectData);
		CDiplomacyData();
		~CDiplomacyData();

		int			m_iPlayerId;		// id ������ ������ (PlayerId)
		int			m_iConCost;			// ��������� ���������� �������� 
		int			m_iMinimumCost;		// ���.�����.��������

		LinearArray<int,_int>	LACon;	// ������ ���������
		LinearArray<int,_int>	LAReq;	// ������ �������� � ������ �� �������

	private:
		CSectData*	m_pSectData;		// �������
		CDiplMenu*  m_pDiplMenu;		// ���� ���������� (������ ��� ����� �� ����.��������! <��� AddContractMoney&&SetContractCost> )

		int			m_iFields;			// ���-�� ����� � ������ �������� = 4
		int			m_iTypeNum;			// ���-�� ����� ��������� = 5

		int			m_iWarId;			// id ���������� ����� = 0
		int			m_iPeaceId;			// = 1
		int			m_iPactId;			// = 2
		int			m_iPassId;			// = 3
		int			m_iAllyId;			// = 4	
		int			m_iUnionId ;		// = 5
		
		int			m_iSecWarId;		// = 10		
		int			m_iSecPeaceId;		// = 11		
		int			m_iSecPassId;		// = 13


		int		DeleteContract(int type, int country1, int country2);			// ������� �������
		int		AddNewContract(int type, int country1, int country2, int time); // �������� �������
		void	ClearAllContracts(int country1, int country2);//very new

		char		m_chFilePath[256];	//save/load path		// SAVE TO: "Save\\dipldata.xml" (Diplomacy Data Save) 

		SAVE( CDiplomacyData ); 
			REG_MEMBER( _int, m_iPlayerId );
			REG_MEMBER( _int, m_iConCost );
			REG_AUTO( LACon );				
			REG_AUTO( LAReq );				
		ENDSAVE;

	public:
		bool	SaveXML();
		bool	LoadXML();

		void	Init();//new
		
		void	OnFirstInit();								//��������� ���������� � ���� "��� �� �����"
		void	SetDiplMenu(CDiplMenu* pDiplMenu); 

		int		GetPlayerNId();								//�����.id ������ ������ - m_iPlayerId
		int		GetWarId();									//�����.id ���������� ����� =0
		int		GetPeaceId();		
		int		GetPactId();		
		int		GetPassId();		
		int		GetAllyId();	
		int		GetUnionId();	

		int		GetSecWarId();	
		int		GetSecPeaceId();	
		int		GetSecPassId();	

		int		GetDiplActionPicture(int type,int country1,int country2);
		
		// �������� ����� ��������
		int		GetContNum();								// �����.���-�� ��-��� ������� ���������
		int		GetContractsSize();							// �����.���-�� ��-���,���������� ������ ����������� ���-�
		int		GetContractsNumber();						// �����.���-�� ����������

		int		SetContractCost( int turn );				// ������.�����.���������
		int		GetContractCost(int type);					// �����.�����.���������
		int		GetContractCost2( int type, bool change ) ;	// �����.�����.��������� � ����������� �� ����� ������ 
		int		GetConCost();								// �����. m_iConCost

		int		GetContractsById(int type, int playerId, int* Id); //�����.���-�� n ���-�,� ������ id �����,� ���.����.�����.(Id[n])
		int		GetContTypesById(int playerId, int secondId, int* types); //�����.���-�� n ������ ����� ���-�(types[n])
		int		GetIdByContType (int type, int* players);				  //�����.���-�� n id (PlayerId[n])

		int		GetContractsNumByType( int type, int playerId );
		bool	CheckContract( int Type, int playerId, int secondId );
		int		GetContractPeriod( int type, int playerId, int secondId );

		void	RecountContractsPeriod(int* conts, int& n);
		
		int		AddContractForPlayer(int type, int time);
		int		AddContract(int type, int country1, int country2, int time);

		//������� � ������ �� ������� 
		int		GetReqNum();												// �����.���-�� ��-��� ������� ��������
		bool	ClearRequests();
		int		AddRequestForContract(int type, int country1, int time);	// �������� ������ �� ��������
		int		ConfirmRequest(int requestId);								// ����.������� �� �������
		int		GetRequestId(int type, int country, bool both = false );	// �������� id ������� �� �������� 
		int		GetRequestId(int type, int country1, int country2);//new	// �������� id ������� �� �������� 
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenuInfo-----------------------------------------
//----------------------------------------------------------------------------------

//������ ������ � textbutton�� � ��������� ����. ���� � ���� ����.
class	CDiplMenuInfo : public CStatusDRV, public CError	
	{
	public:
		CDiplMenuInfo(DialogsSystem* pMenu, CSectData* pSectData, int N, int ButOffset);
		~CDiplMenuInfo() {
			return ;
		}
		//CDiplMenuInfo(DialogsSystem* pMenu,CSectData* pSectData, GPPicture* pDipBack ,GPPicture* pPicture,int N,int ButOffset); 
	public:
		DialogsSystem*	m_pMenu;								// ���� ����.
		CSectData*		m_pSectData;							// �������

		TextButton*		m_pCounName;							// ����.������
		GPPicture*		m_pFlag;								// ��a� ������
		GPPicture*		m_pDipBack;								// �������a-��� ��� ���� ���������
		GPPicture*		m_pPicture;								// �������a ������
		GPPicture*		m_pDeLimit;								// �����������
		DialogsDesk*	m_pStateVerBar;							// vertical   ������a � ���.����.���-�	
		DialogsDesk*	m_pStateHorBar;							// horizontal ������a � ���.����.���-�	
		GPPicture*		m_pStateBack;							// �������a ��������� ����.�����.
		GPPicture*		m_pStateFlags[MAX_PLAYERS*2];			// flags

		TextButton*		m_pSTATES[MAX_PLAYERS][MAX_PLAYERS];	// ����.����.�����.

		int				m_iN;									// ���-��  
		int				m_iPicOffset;							// �������� � ���� ��������
		int				m_iTxtOffset;							// �������� � ���� ����� ����
		int				m_iCurrSecId;							// id ������������� ������� 

		_str			m_chCountryName;						// ���� #1
		_str			m_chResource;							//		#2 
		_str			m_chDefence;							//		#3
		_str			m_chPopulation;							//		#4
		_str			m_chVillages;							//		#5
		_str			m_chState;								//		#

		char			m_chStat[6][50];						// ������� ����. ��������� ����.�����. #CDT_ST_WAR
		char			m_chStatHint[6][256];						// hint ��������� ����.�����. #CHINT_ST_WAR

		bool			m_Visible;								// ��������� 
		bool			m_Enabled;								// ����������� 

	public:
		void	CreateElements();

		void	SetVisible(bool	state);					// ���-���� ��������� 
		void	SetEnabled(bool	state);					// ���-���� ����������� 
		
		int		SetCurrSecId(int id);					// ��������� id ������������� �������

		bool	ChangeSecInfo( int id , CPlayer* pPlayer[MAX_PLAYERS] ); //��������� ���� ��	SecId
		//bool	ChangeSecInfo(int id);					//��������� ���� ��	SecId

		void	SetCountryPicture(int id);				// �������a ������
		void	SetCountryName(char* name);				// ����.������
		void	SetResource(char* name);				// ����.�������
		void	SetState(int value);					// ���������
		void	SetDefence(int value);					// �������
		void	SetPopulation(int value);				// ���������
		void	SetVillages(int value);					// ���-�� �������
		
		bool	ShiftTableBars( int nation );	

		bool	SetValueById(int id, char* value);		// ���������� Message = value
		int		GetTxtOffset();							// �������� �������� ����� ����
		
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenuCountries------------------------------------
//----------------------------------------------------------------------------------

//������ ������ � textbutton�� ����. ���� � ���� ����.
class	CDiplMenuCountries : public CStatusDRV, public CError	
	{
	public:
		CDiplMenuCountries(DialogsSystem* pMenu,int N,int TxtOffset); 
	private:
		DialogsSystem*	m_pMenu;		//���� ����.

		int				m_iN;			//���-��  
		int				m_iTxtOffset;	//��������  � ����.

		bool			m_Visible;		//��������� 

	public:
		void	SetVisible(bool	state);						//���-���� ��������� 
		void	SetVisible(bool	state, int param);			//���-���� ��������� 2
		
		bool	SetValueById(int id, char* value);			//������������ ������ � textbutton
		void	SetValues(char** ppNames);					//�����.������ ����.����� � ����.�����	
		void	SetDealValues(int param, char** ppNames);	//�����.������ ����.����� � ������� ����.���������	
		int		GetTxtOffset();								//�������� �������� ����� ����

};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenuButtons--------------------------------------
//----------------------------------------------------------------------------------

//������ ������ � ������� ����. �������� 
class	CDiplMenuButtons : public CStatusDRV, public CError		
	{
	public:
		CDiplMenuButtons(DialogsSystem* pMenu,int N,int ButOffset);

		DialogsSystem*	m_pMenu;			//���� ����
		GP_TextButton*	m_pMainOK;			//������ OK ����
		GP_TextButton*	m_pMainNO;			//������ NO ����

		Enumerator*		m_pMessHeads;		//enum "MessageHeads"	
		Enumerator*		m_pMessTexts;		//enum "MessageTexts"

		int				m_iN;				//���-�� ������ 
		int				m_iButOffset;		//�������� ������ � ����.
		int				m_iTxtOffset;		//�������� ������� � ������� � ����.
		int				m_iActive;			//�������� ������			
		//int			m_iPressed;			//������� ������				//bool m_IsActive; //���������� ������ //bool m_IsPressed; //������ ������ //bool m_Redraw; //������������� ����������� ������	

		bool			m_ChangeFont;		//���� ����� ������ �� �������
		bool			m_Visible;			//��������� ������
		bool			m_Enabled;			//����������� ������

		//bool	SetDisabledFont(); 
		//bool	MainMenuEnabled(CSectData* pSectData,bool state);				//����������� ��������� ���� ��������

	public:
		bool	SetMainOK(GP_TextButton* pGPMainOK);							//������ OK ����
		bool	SetMainNO(GP_TextButton* pGPMainNO);							//������ NO ����
		
		int		GetOffset();													//�������� �������� ������ � ����

		void	SetVisible(bool	state);											//���-���� ��������� 
		void	SetEnabled(bool	state);											//���-���� ����������� 
		void	SetDisabledButtons(CDiplomacyData* pDiplData, int* type, int n);//���������� disable ������
		int		SetActive(int active); 

		bool	DiplActionPressed(int ActionActive);
		bool	StartDiplAction(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],CDiplMenuButOnMap* pMapButt,int DiplAction); 	 
		bool	ShowDiplMessage(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],int ContMoney,int MessId); 

		bool	SetDisabledFont(); 
		bool	MainMenuEnabled(CSectData* pSectData,bool state, int pageN );	//����������� ��������� ���� ��������

		void	CreateHint();
	
};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenuButOnMap-------------------------------------
//----------------------------------------------------------------------------------

//������ ������ � ������� ����������� ����. ���� �� �����
class	CDiplMenuButOnMap : public CStatusDRV, public CError	
	{
	public:
		CDiplMenuButOnMap(DialogsSystem* pMenu,int N,int Pressed1,int Pressed2,int ButOffset); 
	private:
		DialogsSystem*	m_pMenu;				// ���� ����.

		int				m_iN;					// ���-�� ������ 
		int				m_iButOffset;			// �������� ������ � ����.
		int				m_iPressed1;			// ������ ������� ������		
		int				m_iPressed2;			// ������ ������� ������		
		int				m_iActive;				// ��������	������			//bool m_IsActive; //���������� ������

		bool			m_Visible;				// ��������� ������
		bool			m_Enabled;				// ����������� ������
		
		//new
		GPPicture*		m_pFramePic;


	public:
		void	CreateElements();

		void	SetVisible(bool	state);								// ���-���� ��������� 
		void	SetEnabled(bool	state);								// ���-���� ����������� 

		int		ShowActiveButton(int MapActiveButton);				// ����� �������� ������
		bool	ShowPressedButton(int* PressedButton, int SecID);	// ����� ������� ������
		void	ShowHighlightSectors();								// ��������� ��������

		int		GetButOffset();										// �������� �������� ������ � ����
		int		GetFirstPressed();									// �����.N - ������ ������� ������
		int		GetSecondPressed();									// �����.N - ������ ������� ������

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplProposes-----------------------------------------
//----------------------------------------------------------------------------------

// ---CDiplProposes--- ������ ������ �� ����� ����. ����������� ������
class	CDiplProposes	//NOT USED!!!
	{
	public:
		CDiplProposes(DialogsSystem* pMenu,int N,int ProOffset);
	private:
		DialogsSystem*	m_pMenu;				//c��� ����.

		int				m_iN;					//���-�� ����
		int				m_iButOffset;			//�������� � ����.
		int				m_iActive;				//�������� ����
		
		bool			m_Visible;				//��������� 
		bool			m_Enabled;				//�����������

	public:
		void	SetVisible(bool	state);					//���-���� ��������� 
		void	SetEnabled(bool	state);					//���-���� ����������� 

		void	OpenNewWindow( int DiplAction, int PlayerId );
		void	OnCloseWindow();
	};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplInfoMessages-------------------------------------
//----------------------------------------------------------------------------------

//��������� ��� �������� ��������� ������
class	CDiplMessage_STRUCT : public BaseClass
{ 
public:
	CDiplMessage_STRUCT();
	~CDiplMessage_STRUCT();
public:
	_str	Letter;				// ������� �� ������ ���������		
	_str	MessHead;			// ��������� ���������				
	_str	MessText;			// ����� ���������					

	int		pictId;				// id �������� � �����.
	int		MessType;			// 0 - no buttons.  2 - ok/close.  3 - agree/no/close.  4 - ok/no/close.
	int		reqId;				// id ������� �� �������� � DiplomacyData
	int		status;				// readed (yes/no) = -1  /  canceled (cancel) = 1

	int		sortId;				// sorting by sortId ( priority AND Contract Type AND questINF! )

	SAVE( CDiplMessage_STRUCT );
		REG_AUTO  ( Letter );  
		REG_AUTO  ( MessHead );  
		REG_AUTO  ( MessText ); 

		REG_MEMBER( _int, pictId );
		REG_MEMBER( _int, MessType );
		REG_MEMBER( _int, reqId );
		REG_MEMBER( _int, status );

		REG_MEMBER( _int, sortId );
	ENDSAVE;
}; //end of struct CDiplMessage_STRUCT

/*struct	SDiplMessage {
		_str	Letter;			//������� �� ������ ���������		
		_str	MessHead;		//��������� ���������				
		_str	MessText;		//����� ���������					
		int		pictId;
		int		MessType;		// 2 - ok/close; 3 - agree/no/close; 4 - ok/no/close
		int		reqId;			// id ������� �� �������� � DiplomacyData
		int		status;			// readed (yes/no) = -1  /  canceled (cancel) = 1
		int		sortId;			// sorting by sortId
};*/

//----------------------------------------------------------------------------------

//������ ������ �� �������������� ���������� ������
class	CDiplInfoMessages : public BaseClass		//, public CError //, public CStorageData
	{
	public:
		CDiplInfoMessages( CDiplInfoMessBox* pMessBox,  CDiplInfoButtons* pInfoButtons,
						   CDiplomacyData*   pDiplData, CDiplMenuInfo*    pInfoTexts,   CDiplInfoPanel*	pInform, 
						   int n );
		CDiplInfoMessages();
		~CDiplInfoMessages();
	public:
		ClassArray<CDiplMessage_STRUCT>   CAMess;	//������ ���.�����.

		CDiplomacyData*		m_pDiplData;  
		CDiplMenuInfo*		m_pInfoTexts; 
		CDiplInfoPanel*		m_pInform;
		
		int					m_iActive;				//�������� ���������
		bool				m_NextRound;

		bool				m_Visited;				// ���������� ��� ���
		bool				m_Blink;				// ������� �������� "Messages"
		int					m_iBlinkTime;
		//int					m_MessPageOffset;

		//new
		bool				m_NewMessage;

		char				m_chFilePath[256];		//save/load path
		// SAVE TO: "Save\\diplmess.xml" (BigMap Info Messages Save) 
		SAVE( CDiplInfoMessages );
			REG_CLASS ( CDiplMessage_STRUCT );
			REG_AUTO  ( CAMess );				
		ENDSAVE;

	public:
		bool	operator +=(CDiplMenuInfo* pInfoTexts);

		bool	SaveXML();
		bool	LoadXML();

		int		GetMesNum();
		void	SortMessages();						// ���������� ���������		

		void	Process( int Pressed );
		void	DrawButtons();
		void	ShowMessage(int Pressed);			// ���������� ��������� �� �����
		bool	ShowMessageDesk(bool state);		// ���-���� ���������� desk-a
		int		SetActiveMessage(int active);
		int		SetPassiveMessage();
		
		int		GetMessageId(int requestId);

		bool	AddMessage(int priority,int MessageType,int PictureId,char* ButtonLetter,char* MessageHead,char* MessageText, bool NewMess=false );
		bool	AddMessage(int priority,int MessageType,int PictureId,char* ButtonLetter,char* MessageHead,char* MessageText,int reqId, bool NewMess=false );

		bool	ClearMessages();					// �������� ���� ���������
		bool	ConfirmMessage();					// ���������.���������� ���. + ����.�����.�����.
		bool	DeleteMessage();					// ������� �����.�����.					/*�������*/
		bool	CloseMessage();						// ������� �����.�����.

		//new
		//bool	DeleteMessage2();					// ������� �����.�����. + ����.�����.�����.
		bool	DeleteMessage2(bool del=true,bool accept=false,int sid=-1); // ������� �����.�����. + ����.�����.�����.

		bool	ConfirmMessage( int offset );
		bool	DeleteMessage( int offset );

		bool	RefreshDiplMenu();
		void	ScrollInformation();

		bool	CheckBlink();
		void	PrepareBlink();

	};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//**********************************************************************************************

//----------------------------------------------------------------------------------
//----------------------------CDiplInfoPanel--------------------------------------
//----------------------------------------------------------------------------------

// ������ � Desk � ����������� - visual class!!! (� ����� ��� ������ �� ���������� �����)
class	CDiplInfoPanel 
	{
	public:
		CDiplInfoPanel( DialogsSystem* pMenu, int Offset); 
	public:
		DialogsSystem*		m_pMenu;				// ���� ����
		DialogsDesk*		m_pMainDesk;			// desk for All page elems
		DialogsDesk*		m_pDesk;				// desk for messages

		GP_TextButton*		m_pgptbStatx;			// ������ ����������
		//new
		GPPicture*			m_pDeLimiter;			// delimiter

		TextButton*			m_ptbName ;
		TextButton*			m_ptbSecN ;
		TextButton*			m_ptbScore ;
		//new
		GPPicture*			m_pgppRow;				// rows head

		//new
		GPPicture*			m_pRow[MAX_PLAYERS];	// rows

		GPPicture*			m_pFlag[MAX_PLAYERS];	// flags
		TextButton*			m_pName[MAX_PLAYERS];	// names
		TextButton*			m_pSecN[MAX_PLAYERS];	// SecNum
		TextButton*			m_pScore[MAX_PLAYERS];	// score

		int					m_iOffset;				// �������� ������ � ����.
		int					m_iX0;					// ���������� x0 ���.�����.���� ������� ������
		int					m_iY0;					// ���������� y0 ���.�����.���� ������� ������
		int					m_iStepY;			
		int					m_iN;					// ���-�� ������				
		int					m_iFree;				// ���������� ����� ��������� ������; ��� m_iN=0 - m_iFree=0

		bool				m_Visible;				// ��������� ������
		bool				m_Enabled;				// ����������� ������

		int					m_iMessElem;
		int					m_iInfoMessages;

	public:
		void	Init();

		int		GetOffset();										//�������� �������� ������ � ����
		int		GetNumber();										//�������� m_iN

		void	SetVisible(bool	state);								//���-���� ��������� 
		void	SetEnabled(bool	state);								//���-���� ����������� 

		void	SetActive(int n);
		void	SetPassive(int n);

		int		Add();												//�������� ���oe
		void	AddMessage( CDiplMessage_STRUCT* pMess, int n );	//�������� ��������e		
		void	Close();											//��������

		int		GetParam(int offset);
		void	SetInfoMessages(CDiplInfoMessages* pMs);
		void	ScrollDesk(int pos);

		//new
		bool	RefreshStats(bool state);

	};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//************************************************************************************************

//----------------------------------------------------------------------------------
//----------------------------CDiplInfoButtons--------------------------------------
//----------------------------------------------------------------------------------

//�������������� ������ (� ����� ��� ������ �� ���������� �����)
class	CDiplInfoButtons	//NOT USED!!!
	{
	public:
		CDiplInfoButtons(DialogsDesk* pDesk, GPPicture* pRulon, int ButOffset); 
	private:
		DialogsDesk*		m_pDesk;				//desk
		GPPicture*			m_pRulon;				//������ ���/����. ���������� desk
		
		int					m_iButOffset;			//�������� ������ � ����.
		int					m_iX0;					//���������� x0 ���.�����.���� ������� ������
		int					m_iY0;					//���������� y0 ���.�����.���� ������� ������
		int					m_iN;					//���-�� ������				
		int					m_iFree;				//���������� ����� ��������� ������; ��� m_iN=0 - m_iFree=0

		bool				m_Visible;				//��������� ������
		bool				m_Enabled;				//����������� ������

	public:
		int		GetButOffset();							//�������� �������� ������ � ����
		int		GetNumber();							//�������� m_iN

		void	SetVisible(bool	state);					//���-���� ��������� 
		void	SetEnabled(bool	state);					//���-���� ����������� 

		void	SetActive(int n);
		void	SetPassive(int n);

		int		AddButton();							//�������� ����� ������
		void	AddMessageButton(int n, char* mess);	//�������� ������ ���������		
		void	CloseButtons();							//��������

		bool	CheckRulonVisible(bool state);
		void	SetDeskVisibled();

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplInfoMessBox--------------------------------------
//----------------------------------------------------------------------------------

//�������������� ����
class	CDiplInfoMessBox	//NOT USED!!!
	{
	public:
		CDiplInfoMessBox(DialogsSystem* pMenu,int BoxOffset); 
		CDiplInfoMessBox()  { return; }
		~CDiplInfoMessBox() { return; }
	private:
		DialogsSystem*		m_pMenu;				//����
		GPPicture*			m_pgppBox;				//������ ���� ���������
		GPPicture*			m_pgppPic;				//��������
		TextButton*			m_ptbHead;				//��������� 
		//TextButton*			m_ptbText;				//����� ���������
		GP_TextButton*		m_pgptbYes;				//������ Yes-Agree-Ok
		GP_TextButton*		m_pgptbNo;				//������ No
		GP_TextButton*		m_pgptbClose;			//������ Close

		TextViewer*			m_ptvText;				//����� ���������

		_str				m_chOK;
		_str				m_chYes;		

		int					m_iBoxOffset;			//�������� � ����.
		int					m_iN;					//���-�� ��������� ����
		int					m_iX0;					//���������� x0 ���.�����.����  
		int					m_iY0;					//���������� y0 ���.�����.����  
		int					m_iMessType;			//id ����.���� ����

		int					m_iCurrMess;			//id �������� ��������� 

		bool				m_Visible;				//��������� 
		bool				m_Enabled;				//����������� 

	public:
		int		GetBoxOffset();						//�������� �������� ��-� ���� � ����

		void	Init();								//
		void	PrepareMessBox();					//
		bool	SetCallFunClicks(CDiplInfoMessages* pMs, pCULL_FUN_CLICK yes, pCULL_FUN_CLICK no, pCULL_FUN_CLICK close);

		void	Show();								//
		void	Close();
		void	SetVisible(bool	state);				//���-���� ��������� 
		void	SetEnabled(bool	state);				//���-���� ����������� 

		bool	SetMessage( CDiplMessage_STRUCT* pMess, int n );
		
		bool	ShowGlobalMessage(int x, int y, CDiplMessage_STRUCT* pMess, int n ) { return true;}

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenu---------------------------------------------
//----------------------------------------------------------------------------------

//������ ������ �� ��������� ���������� ���� ����.   (������������� ����� ��� ���� ����������)
class	CDiplMenu : public CStatusDRV, public CError
	{
	public:
		CDiplMenu( DialogsSystem*		pMenu,	GP_TextButton* pMainOK , GP_TextButton* pMainNO ,
				   CSectData*			pSectData,
				   CDiplomacyData*		pDiplData,
				   CDiplMenuInfo*		pInfoTexts, 
				   CDiplMenuCountries*	pWarsTexts,	CDiplMenuCountries*	pDealsTexts,
				   CDiplMenuButtons*	pActButtons,CDiplMenuButOnMap*	pMapButtons,
				   CDiplInfoMessages*	pMessages,	CDiplInfoButtons*	pInfoButtons, CDiplInfoMessBox* pMessBox ,
				   CSectorMenu*			pSectMenu );
	public:
		DialogsSystem*			m_pMenu;					// c��� ����.

		CSectData*				m_pSectData;				// �������	
		CSectorMenu*			m_pSectMenu;				// ���� �������
		CDiplomacyData*			m_pDiplData;				// ����.������	
	
		CDiplMenuInfo*			m_pInfoTexts;				// ���������� �� �������
		CDiplMenuCountries*		m_pWarsTexts;				// ������ ����� (�����)
		CDiplMenuCountries*		m_pDealsTexts;				// ������ �����
		CDiplMenuButtons*		m_pActButtons;				// ������ ����.��������
		CDiplMenuButOnMap*		m_pMapButtons;				// ������ ��� ������
		CDiplInfoMessages*		m_pMessages;				// �������������� ��������� ������
		CDiplInfoButtons*		m_pInfoButtons;				// ������ ���. ��������� ������
		CDiplInfoMessBox*		m_pInfoBox;					// ���� ���. ��������� ������
		CPlayer*				m_pPlayer[MAX_PLAYERS];		// ������

		Enumerator*				m_pMessLtrs;				// enum "InfoLetters" 
		Enumerator*				m_pMessInfo;				// enum "InfoTexts"  
		Enumerator*				m_pMessHeads;				// enum "MessageHeads" 
		Enumerator*				m_pMessT;					// enum "MessageTexts"

		int						m_iST[MAX_PLAYERS][MAX_PLAYERS]; // hints

		bool					m_Visible;					// ��������� ����
		bool					m_Enabled;					// ����������� ����

	public:
		bool	operator+=	(CPlayer*	pPlayer);					// �������� ��������� �� �������
		bool	operator+=	(CDiplMenuInfo*		pInfoTexts);		// �������� ���������
		bool	operator+=	(CDiplMenuButOnMap*	pMapButt);			// �������� ���������


		bool	Show( bool state, bool act );						// ���-���� ���������� ����
		void	SetVisible( bool state, bool act );					// ���-���� ��������� 
		void	SetEnabled( bool state, bool act );					// ���-���� ����������� 
		
		void	Process( int* MapPressedButton,int DiplAction, int ActiveMenu,bool MenuIsChange , bool ActEn );//��������� ������� � ����
		
		void	SetSectorsColor(int state1, int state2);			// ��������� �������� �������;)
		void	CheckEnabledDiplButtons();							// disable �������� ������ ����. ��������

		bool	AddContract(int type, int country1, int country2, int time); 
		bool	AddMessage(int priority,int MessageType,int MessagePictId,char* ButtonLetter,char* MessageTitle,char* MessageText, bool NewMess=false );
		void	SetContractEndsMess(int* conts, int n);
		void	PrepareNextRound( int turn );
		void	ClearData();

		void	AddContractMoney(int type, int country1, int country2); 
		void	AddRequestMoney( int country1, int country2, int money );
	
		bool	ContractEnabled( int type, int country1, int country2 );

		int		ShowGlobalMessage( int type, int pictId, int headId, char* text, int pageN );
		int		ShowGlobalMessage( int type, int pictId, int headId, char* text, int pageN , SimpleDialog* pSD );

		// Diplomacy Data functions
		int		GetContractsById(int type, int playerId, int* Id);	// �����.���-�� n ���-�,� ������ id �����,� ���.����.�����.(Id[n])
		int		GetContTypesById(int playerId, int secondId, int* types); // �����.���-�� n ������ ����� ���-�(types[n])
		int		GetIdByContType (int type, int* players);			// �����.���-�� n id (PlayerId[n])
		int		GetContractCost(int type);							// �����.�����.���������

		int		GetConCost();										// �����. CDiplomacyData::m_iConCost

		int		GetContractPeriod( int type, int playerId, int secondId );

		int		GetContractsNumByType( int type, int playerId );
		bool	CheckContract( int Type, int playerId, int secondId ); 

		bool	CheckRequests( int type, int ActiveMenu, int SecID );
		bool	AddRequestMessage(int priority,int type,int country1,int country2,int time,int MessageType);
		bool	AddRequestMessage(int priority,int type,int country1,int country2,int time,int reqId,int MessageType);
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CTYPENUM    ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// �������������� �������� ��� ������.
class	CTYPENUM : public BaseClass
{
public:
	CTYPENUM();
	~CTYPENUM();
public:
	_str	TypeName_sID;		//��������� ������������� ���� ������.
	int		TypeIndex_ID;		//������� ������ ���� ������.
	int		nSquarnd;			//���-�� �������/������.

	LinearArray<int,_int>	Kills;	//grey //���-�� kills � �������

	SAVE( CTYPENUM );
		REG_AUTO	(		TypeName_sID  );
		REG_MEMBER	( _int, TypeIndex_ID  );
		REG_MEMBER	( _int, nSquarnd	  );
		REG_AUTO    (       Kills		  );
	ENDSAVE;

public:
	_str*	getTypeName();
	int		getTypeIndx();
	int		getNSquard ();

	void	setTypeName( const char* TypeName );
	void	setTypeName( const _str& TypeName );
	void	setTypeIndx( const int	 TypeIndx );
	void	setNSquard ( const int   n );
	void	SetAll	   ( const char* TypeName, const int n );
	void	SetAll	   ( const _str& TypeName, const int n );
	void	SetAll	   ( const char* TypeName, const int id, const int n );
	void	SetAll	   ( const _str& TypeName, const int id, const int n );

	CTYPENUM&	operator=  ( const char*		TypeName );
	CTYPENUM&	operator=  ( const _str&		TypeName );
	CTYPENUM&	operator=  ( const int			n        );	
	CTYPENUM&	operator=  ( const CTYPENUM&	te		 );

};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CMISSIONINIT    ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class	CMISSIONINIT : public BaseClass
{
public:
	CMISSIONINIT();
	~CMISSIONINIT();

	CArmyLimit*				pArmyData;				// ������ �� ������ ������� �� ��������� ������.

	int						Sector_ID;				// ������ �������.
	int						AttackDirection_ID;		// ������ ���������� ����� �� �������. (E,N-E,N,N-W,W,S-W,S,S-E)

	int						AttackerNation_ID;		// ������ ��������� �����.
	int						AttackerRank_ID;		// ���� ����������.
	ClassArray<CTYPENUM>	AttackerForce;			// ������ ��������� �������.

	int						DefenderNation_ID;		// ������ ������������� �����.
	int						DefenderRank_ID;		// ���� ����������.
	ClassArray<CTYPENUM>	DefenderForce;			// ������ ������������� �������.

	int						PlayerStat;				// ��� ����� ������ ������� - ���������(=0) ��� ����������(=1).
	bool					HeroInSect;				// ������������ ��������� ����� � �����.
	
	CTYPENUM*	createSquard		( const char*	TypeName_sID, const int n );
	CTYPENUM*	createSquard		( const _str	TypeName_sID, const int n );

	bool		addSquard			( CTYPENUM* pNew, ClassArray<CTYPENUM>* pBase );
	bool		addSquard			( char* TypeName_sID, int n, ClassArray<CTYPENUM>* pBase );
	bool		addSquard			( _str  TypeName_sID, int n, ClassArray<CTYPENUM>* pBase );
	bool		addAttackerSquard	( CTYPENUM* pNew );
	bool		addAttackerSquard	( char* TypeName_sID, int n );
	bool		addAttackerSquard	( _str  TypeName_sID, int n );
	bool		addDefenderSquard	( CTYPENUM* pNew );
	bool		addDefenderSquard	( char* TypeName_sID, int n );
	bool		addDefenderSquard	( _str  TypeName_sID, int n );
	
	void		SetARMYData				( CArmyLimit* pAL );
	void		setAttackerNation		( int nat );
	void		setDefenderNation		( int nat );
	void		SetSector_ID			( int sID );
	void		setAttackerDirection_ID	( int dID );
	void		setAttackerDirection_ID	( char* sID );
	void		setAttackerRank_ID		( int rID );
	void		setDefenderRank_ID		( int rID );
	void		setAttackerArmy			( CHero* pH );
	void		setDefenderArmy			( CHero* pH );
	void		setHeroPresent			( bool	stat )	{ HeroInSect = stat; };

	void		createDefenderArmyBySectorParam(int populatin,int defence,int rase);

	void		addArmyBySectorDefence( int SecDef , int SecPop = 0 );

	int			getSector_ID			();
	int			getAttackerNation		();
	int			getDefenderNation		();
	int			getAttackerRank_ID		();
	int			getDefenderRank_ID		();
	bool		GetHeroPresent			()	{ return HeroInSect; };

	void		Init();
	void		Clear( ClassArray<CTYPENUM>* pBase );
	void		Clear();
	void		ClearAttacker();
	void		ClearDefender();

	INT_PROPERTY( CMISSIONINIT,	getAttackerNation,  setAttackerNation );
	INT_PROPERTY( CMISSIONINIT,	getDefenderNation,  setDefenderNation );
	INT_PROPERTY( CMISSIONINIT, getAttackerRank_ID, setAttackerRank_ID);
	INT_PROPERTY( CMISSIONINIT, getDefenderRank_ID, setDefenderRank_ID);
	SAVE( CMISSIONINIT );
		REG_CLASS ( CTYPENUM );
		//REG_LOADSAVE("Dip\\","*.victor.xml");
		REG_MEMBER( _int, Sector_ID );
		REG_ENUM  ( _index, AttackDirection_ID,	SDIR_ENUM );
		REG_INTPROP  ( _int, AttackerNation_ID, getAttackerNation,  setAttackerNation  );
		REG_INDEXPROP( _index, AttackerRank_ID, getAttackerRank_ID, setAttackerRank_ID, RANK_ENUM);
		REG_AUTO( AttackerForce );
		REG_INTPROP	 ( _int, DefenderNation_ID, getDefenderNation,  setDefenderNation )
		REG_INDEXPROP( _index, DefenderRank_ID, getDefenderRank_ID, setDefenderRank_ID, RANK_ENUM);
		REG_AUTO( DefenderForce	);
		REG_ENUM  ( _index, PlayerStat, PLAYER_STAT_ENUM );
		REG_MEMBER( _bool, HeroInSect );
	ENDSAVE;
};

//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    CSAVE_MIS_STRUCT    /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

class	CSAVE_MIS_STRUCT : public BaseClass
{
public:
	CSAVE_MIS_STRUCT();
	~CSAVE_MIS_STRUCT();
public:
	// CMISSIONINIT
	int						Sector_ID;				// ������ �������.
	int						DifficultyLevel;		// ������� ���������.
	_str					AttackDirection_ID;		// ������ ���������� ����� �� �������. (E,N-E,N,N-W,W,S-W,S,S-E)

	int						AttackerNation_ID;		// ������ ��������� �����.
	_str					AttackerRank_ID;		// ���� ����������.
	ClassArray<CTYPENUM>	AttackerForce;			// ������ ��������� �������.
	CResource_BM			AttackerResource;		// Resource	

	int						DefenderNation_ID;		// ������ ������������� �����.
	_str					DefenderRank_ID;		// ���� ����������.
	ClassArray<CTYPENUM>	DefenderForce;			// ������ ������������� �������.
	CResource_BM			DefenderResource;		// Resource	

	int						PlayerStat;				// ��� ����� ������ ������� - ���������(=0) ��� ����������(=1).
	bool					HeroPresent;			// ������������ ��������� ����� � �����.

	SAVE( CSAVE_MIS_STRUCT );
		REG_CLASS	( CTYPENUM );
		REG_CLASS	( CResource_BM ); 

		REG_MEMBER	( _int, Sector_ID			);
		REG_MEMBER  ( _int, DifficultyLevel		);
		REG_AUTO	( AttackDirection_ID		);  

		REG_MEMBER	( _int, AttackerNation_ID	);
		REG_AUTO	( AttackerRank_ID			);
		REG_AUTO	( AttackerForce				);
		REG_AUTO	( AttackerResource			); 
		
		REG_MEMBER	( _int, DefenderNation_ID	);
		REG_AUTO	( DefenderRank_ID			);
		REG_AUTO	( DefenderForce				);
		REG_AUTO	( DefenderResource			); 
		
		REG_MEMBER	( _int,	PlayerStat			);
		REG_MEMBER	( _bool, HeroPresent		);
	ENDSAVE;
public:
	void	Init();
	void	DeleteArrays();

	CTYPENUM*	createSquard		( const char*	TypeName, const int n );
	CTYPENUM*	createSquard		( const _str	TypeName, const int n );

	void	operator+=	( const CMISSIONINIT& MIS_INT );

	void	SetResources(CPlayer* pAttPL, CPlayer* pDefPL);
	void	SetNKills( CHero* pAttack , CHero* pDefend );
};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CBriefing-----------------------------------------
//----------------------------------------------------------------------------------

//������ ������ � map-��������
class	CBriefing : public CStatusDRV, public CError 	
	{
	public:
		CBriefing(DialogsSystem* pMenu, int offset); 
	public:
		DialogsDesk*	BriefDesk;				//temp
		DialogsDesk*	ObjDesk;				//temp
	private:
		DialogsSystem*	m_pMenu;				//���� ����.
		GPPicture*		SecMap;					//preview ����� �������
		//DialogsDesk*	BriefDesk;				//������. ���-�� �� �������	
		//DialogsDesk*	ObjDesk;				//���. ���. & objectives �� ������� - ������+������
		TextViewer*		SecDesc;				//textview
		TextViewer*		SecDesc2;				//textview2

		int				m_iOffset;

		bool			m_Visible;				//��������� 
		bool			m_Enabled;				//����������� 

		bool	m_Visited; // ���������� ��� ���

	public:
		void	Init();										//�������� ��-���

		void	SetVisible(bool	state);						//���-���� ��������� 
		void	SetEnabled(bool	state);						//���-���� ����������� 

		bool	Refresh(bool BriefingVisible, int secId);	//�������� 

		bool SetVisit();
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//--------------------------- CHeroPersDesk ----------------------------------------
//----------------------------------------------------------------------------------

//������ ������ � DialogsDesk ��� CPERSONAL 
class	CHeroPersDesk   	
	{
	public:
		CHeroPersDesk( DialogsSystem* pMenu , CHERO_PORTRAIT* pPortrait , CHERO_MINI_MENU* pHeroMiniMenu ); 
		~CHeroPersDesk(); 
	public:
		DialogsSystem*		m_pMenu;
		CHERO_PORTRAIT*		m_pHPortrait;
		CHERO_MINI_MENU*	m_pHMiniMenu;					//

		DialogsDesk*		m_pDesk;						// ������ � �������

		bool				m_Visible;						// ��������� 
		bool				m_Enabled;						// ����������� 
		bool				m_Visited;						//  

		TextButton*			m_pCounName;					// ����.������
		GPPicture*			m_pFlag;						// ��a� ������

		// ������� ��� ���c.���� �� ������
 		GPPicture*			pgppMenuBack[MAX_PLAYERS];		// BackGround for HERO_MINI_MENU [MAX_PLAYERS*MAX_HEROES]
 		GPPicture*			pgppPortrait[MAX_PLAYERS];		// BackGround for HERO_MINI_MENU

		TextButton*			ptbName[MAX_PLAYERS];			// ��� ������.
		TextButton*			ptbRank[MAX_PLAYERS];			// ������ ������.
		TextButton*			ptbExpa[MAX_PLAYERS];			// expa
		TextButton*			ptbSQNum[MAX_PLAYERS];			// SQ-num
		
		TextButton*			ptbVictN[MAX_PLAYERS];			// SQ-num

		GPPicture*			ptbStat[MAX_PLAYERS];
		GPPicture*			ptbFlag[MAX_PLAYERS];
		int					m_dxSF;

		DialogsDesk*		pgppExpBar[MAX_PLAYERS] ;		// expa-BAR-DESK!!!
		GPPicture*			pgppExpBack[MAX_PLAYERS];		// BackGround for expa
		GPPicture*			pgppExpBarPIC[MAX_PLAYERS] ;	// expa-BAR

		int					X0, Y0;							//��������� ������� ����������.
		int					m_iCurPlayer;					// ���.�����

		//new
		int					m_iCurPortrait, m_iPortraitNum;

	public:
		void	Init();
		void	CreateElements();

		void	SetVisible( bool state, int n );			//���-���� ��������� 
		void	SetEnabled( bool state, int n );			//���-���� ��������� 
		//void	SetVisible(bool	state);	//void	SetEnabled(bool	state);

		void	SetCoords( int num );						// ���������� ����� ����������
		bool	Show( CPlayer* pPlayer[MAX_PLAYERS] , CHero* pHero , int num , int heroId , bool PersVisible );		
		void	Refresh( CHero* pHero , int num  );

		void	ChangeActive( int pHero ); //void	ChangeActive( CHero* pHero ); 
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//---------------------------- CResPanel_BM ----------------------------------------
//----------------------------------------------------------------------------------

//������ ������ � ��������� ������ �  ����.
class	CResPanel_BM
	{
	public:
		CResPanel_BM( DialogsSystem* pMenu ); 
	public:
		DialogsSystem*	m_pMenu;					// c��� ����.
		
		GPPicture*		pgppResPic[ 7 ] ;			//
		TextButton*		pgppResTxt[ 7 ] ;			//

		int				m_iOffset;

		bool			m_Visible;					// ��������� 
		bool			m_Enabled;					// ����������� 

	public:
		void	Init();	
		void	CreateElements();										// �������� ��-���

		bool	SetEnabled( bool state );
		bool	SetVisible( bool state );

		bool	Refresh( CPlayer* pPlayer[MAX_PLAYERS], int Nation );	//�������� 

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//---------------------------- CMarketDesk -----------------------------------------
//----------------------------------------------------------------------------------

// ������ ������ �� ���������� �����
class	CMarketDesk   	
	{
	public:
		CMarketDesk( DialogsSystem* pMenu , int MX0 , int MY0 , int MX1, int MY1 ); 
		~CMarketDesk(); 
	public:
		DialogsSystem*		m_pMenu;
		
		DialogsDesk*		m_pDesk;				// ������ � ������
		cvi_Market*			m_Market;

		TextButton*			ptbMarketTitle;			//
		GPPicture*			pgppMarketBack;			//
		GPPicture*			pgppPicture;			//
		GPPicture*			pgppPictBak;			//
		DialogsDesk*		pddInfoDesk; //new
		GPPicture*			pgppInfoBak; //new
		TextButton*			ptbMarketInfo;			// Merchants Info
		DialogsDesk*		pddActionDesk; //new
		GPPicture*			pgppActionBak; //new
		TextButton*			ptbActionMess;			// reaction help mess after player action
		
		TextButton*			ptbPricesMess;			// tb for m_chPricesMess
		
		int					CurRes[COAL+1];
		int					m_ResBuy;
		int					m_ResSell;
		int					m_BuyAmount;
		int					m_SellAmount;
		bool				m_ShowActText;
		int					m_ShowATTime;
		int					m_ResSellPrev;

		int					X0, Y0;					// ��������� �������(x0, y0) 
		int					X1, Y1;					// ������� (x1, y1)
		bool				m_Visible;				// ��������� 
		bool				m_Enabled;				// ����������� 
		bool				m_Visited;				// ������ ���������: true/false

	public:
		void	Init();
		void	CreateElements();							// ����.��-���

		void	SetVisible( bool state );					// ���-���� ��������� 
		void	SetEnabled( bool state );					// ���-���� ��������� 
		
		bool	Show( CPlayer* pPlayer[MAX_PLAYERS] , int Nation , bool state );	
		void	Refresh( CPlayer* pPlayer[MAX_PLAYERS] , int Nation );

		//new
		void	SetPriceMessage( char* mess , bool state=true );

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//---------------------------- CBigMapHelp -----------------------------------------
//----------------------------------------------------------------------------------

//������ ������ � help.
class	CBigMapHelp 
	{
	public:
		CBigMapHelp(DialogsSystem* pMenu); 
		~CBigMapHelp(); 
	public:
		DialogsSystem*	m_pMenu;									// ���� ����

		DialogsDesk*	m_pDesk;									// ������ � help

		GPPicture*		pgppBlackScreen;							// 
		GPPicture*		pgppHelpWindow;								// 

		TextButton*		ptbHelpTitle;								//
		DialogsDesk*	m_pTextDesk;								// ������ � �������
		TextButton*		ptbHelpText;								//

		GP_TextButton*	pgptbClose;									//

		bool			m_Visible;									// ��������� 
		bool			m_Enabled;									// ����������� 

		static const int   m_NPages = 5 ;							// k��-�� �������� (text for F1)
		static const int   m_NMess  = 4 ;							// k��-�� ��������� (help-���������)
		static const int   m_VisNum = m_NPages + m_NMess ;			// ����� k��-�� ���������
	
		bool			m_Visited[ m_VisNum ];						// ���������� ��� ���

	public:
		void	Init();												// init
		void	CreateElements();									// �������� ��-���

		bool	SetVisible(bool	state);								// ���-���� ��������� 
		bool	SetEnabled(bool	state);								// ���-���� �����������

		bool	CheckKey();											// 	

		bool	Show( int page , bool visible );						//
		bool	Refresh( int page );								// �������� 
		bool	SetVisit( int page , bool state );					//

		void	PROCESS_HELPSYSTEM( int campID );
	};

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

#endif	//__CBigMap_Class_Declaration__

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//-------------------------- End of Classes ---------------------------------------------------------------------------
//*********************************************************************************************************************

















