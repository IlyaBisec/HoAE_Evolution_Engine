#ifndef __CBigMap_Class_Declaration__
#define __CBigMap_Class_Declaration__

#include "..\cvi_market.h"

//Классы для чтения и хранения данных по игре.

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

extern	bool			g_bEDITOR_CHENGE;		// Произошли изменения в ClassEditor, обновить.
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
extern int		LTN_EXPS_LIMIT;	// 0			// Пределы кол-ва экспы на звания 
extern int		CPT_EXPS_LIMIT; // 1
extern int		MJR_EXPS_LIMIT; // 2
extern int		COL_EXPS_LIMIT; // 3 
extern int		GEN_EXPS_LIMIT; // 4
extern int		MAR_EXPS_LIMIT; // 5

extern int		DisbandSQExp;					// Экспа за убитый отряд
extern int		SecCaptExp;						// Экспа за захват/защиту сектора	
extern int		HeroDefExp;						// Экспа за победу над героем
extern int		QuestDisbSquadExp;				// Экспа за убитый quest-отряд
extern int		QuestCompliteExp;					// Экспа за выполн. quest

extern int		SECT_INCOME[10];				// доход от сектора на каждом ходу
extern int		SECT_ADDING[10];				// доход от основного ресурса сектора
extern int		SECT_MAX_RECRTS;				// макс.кол-во рекрутов
extern int		SECT_REG_RECRTS;				// восстановление рекрутов в секторе
extern int		SECT_DEFENCE[10];				// затрaты на defence сектора
extern float	SECT_DEF_multiplier2;
extern float	SECT_DEF_multiplier3;
extern int		SECT_SABOTAGE[10][5];			// затраты for SABOTAGE
extern int		SECT_SAB_PERCENT[10][5];		// sabotage percent
extern DWORD	SELECTED_SECT_COLOR;			// selected sector
extern DWORD	RED_SECT_COLOR;					// сост.войны - дипломатия
extern DWORD	GREEN_SECT_COLOR;				// договора   - дипломатия
extern DWORD	COMPAGE_BLINKCOLOR;				// commanders Page blink color

//**********************************************************************************
// CONSTANTS

extern int		STEPBACK_TURNLIMIT;					// 5;
extern int		STEPBACK_DISTANCE;					// 2;

#define DIFF_LEVELS_N		 3	//4			// кол-во уровней сложности ( Big Map )
#define	MAX_PLAYERS			 6				// Максимальное кол-во игроков.
#define MAX_COUNTRYS		 6				// Максимальное кол-во стран.
#define MAX_HEROES			10				// Максимальное кол-во героев.
#define MAX_RANKS			 6				// Максимальное кол-во званий.
#define MAX_H_STATUS		20				// Максимальное кол-во статусов, наград.
#define MAX_ARROW			10				// Максимальное кол-во стрелочек.
#define MAX_CONT_VALPERIOD	10				// время действия дипл.договоров
#define	MAX_DEFENCE			 3				// Максимальное зн-е defence сектора
#define	MAX_POPULATION		 2				// Максимальное зн-е Population сектора

#define	MAX_AGGRESSIVE		 9				// 

#define START_SECTOR_ID		6
#define	Safe_FOOD_Amount	50000			// мин.кол-во еды АИ

// Ресурсы в игре
#define WOOD  0
#define GOLD  1
#define STONE 2
#define FOOD  3
#define IRON  4
#define COAL  5
#define RECRT 9

// COLORS
#define	ALPHA_SECT			0x20000000			// прозрачность для секторов  //38	// 5B	// 0x66000000
//#define	SELECTED_SECT_COLOR	0x88000000		// selected sector			//#define SELECTED_SECT_COLOR	0x5BB3B3A9
//#define	RED_SECT_COLOR		0xFFFF0000		// сост.войны - дипломатия
//#define	GREEN_SECT_COLOR	0xFF00AA00		// договора   - дипломатия
////#define	DIPL_RELBAR_COLOR	0x5B000000		// цвет+прозр. полоски в таб.дипл.отн-й

#define	SQ_Amount0	120						// Максимальное кол-во рекрутов(юнитов) в отряде.			// 123
#define	SQ_Amount1	 30						// Максимальное кол-во рекрутов(юнитов) в cavalry отряде.	//  31
#define	SQ_Amount2	  4						// Максимальное кол-во рекрутов(юнитов) для пушек.			//   4

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
int		GetBigMapPlayerColor(int PID);					// PID - номер игрока.
bool	PushOnTop(DialogsSystem* pdsMenu,int psdPic);	// Вытолкнуть картинку наверх.
float	GetAngle(float x,float y);
void	RoundAngle(int Fi);								// Загнать значение угла в диапазон [0,255].
int		GetGoldForRess(int resID);						// Сколько дениг приносит ресурс.
char*	GetRankByID(int rank);							// Получить строку со званием.
char*	GetContByID(int cont);							// Получить строку с названием страны.
void	setCoordSD( SimpleDialog* psd, int x, int y );
bool	OnRulonClick(SimpleDialog* pSD);				// клик на кнопке доски сообщений 
bool	EmptyClick(SimpleDialog* pSD);					// пустой обработчик клика на pSD
bool	OnPIButtonClicked(SimpleDialog* pSD);			// обработчик нажатия на кнопку (pSD->UserParam = 1)
bool	OnDiversionClicked(SimpleDialog* pSD);			// grey [11.03.2004] обработчик нажатия на кнопку Diversion
bool	ChangeActiveMenu(SimpleDialog* pSD);			// Grey [04/06/2004]
//----------------------------------------------------------------------------------
// pre
int		GetBigMapResource( int res );
int		SetBigMapResource( int res, int amount );
int		AddBigMapResource( int res, int amount );
void	SaveBigMapDataXML();								// запись всех данных по Big Map Campaign
int		GetBigMapNationID(int NID,DWORD& MapColor);			// return Big Map Nation ID && Color
GPPicture*	ShowBlackScreen(DialogsSystem* dsMenu); 
bool	FailSound();
int		SEC_DEF_POWER( int defence ); 

bool	ON_HERO_R_CLICK( SimpleDialog* spdHero );			// Обработка правого клика по герою. // Grey [29.01.2004]
extern	class	CHero;
bool	ON_HERO_R_CLICK_VIRTUAL( CHero* pH );				// Виртуальная обработка правого клика по герою. // Grey [29.01.2004]
//Битва между разными AI.
extern	class	CSectStatData;
bool	SimulateBattle( CHero* pAttackHero, CHero* pDefendHero, CSectStatData* pSector ); // Победил ли атакующий.
// add recruits
int		AddRecruitsInSectors( int nation, int secID, int amount );	// Grey [12.01.2004]
// delete recruits
int		DeleteRecruitsInSectors( int nation, int secID, int amount ); // Grey [14.01.2004]
// глоб.сообщение в интерфейсе
int		ShowBigMapGlobalMessage( int type, int pictId, int headId, char* text, int pageN = -1 ); // Grey [16.01.2004]
// Enable of All Menu elements
bool	SetBigMapMenuEnabled( bool state );					// Grey [15.03.2004]
// проверка возм-ти входа в сектор
bool	CanEnterIntoSector( int nation , int secID );		// Grey [06.02.2004]
bool	CanStepIntoSector( int nation , int secID );		// Grey [01.03.2004]
// проверка возм-ти атаки сектора
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
// Хранит обрабатываемый сценарий.
class	CScenarioLV 
	{ 
	public:
		int			m_inBigMapCampID;			// id кампании

		// MENU
		// 0 - World Map		1 - Diplomacy		2 - Personal		3 - Messages		4 - Market
		int			m_inActivMenu;				// Текущее активное меню.
		bool		m_bMenuIsChange;			// Изменили меню.

		// MAP
		bool		m_bMapVisible;				// Карту видно или нет.
		bool		m_bMapEnable;				// Отрабатывать клики и тд. тп. на карте.
		bool		m_bMapMouseMove;			// Тягать ли карту за мышкой.

		// SECTORS
		bool		m_bSectVisible;				// Секторы видно или нет.
		bool		m_bSectEnable;				// Отрабатывать клики и тд. тп. на секторах.
		bool		m_bSectMouseMove;			// Тягать ли секторы за мышкой.
		bool		m_bSectSetPlayerColor;		// Перекрасить в цвета игроков.
		bool		m_bSectMenuVisible;			// Is sector menu visible.
		int			m_inLastLMCSecID;			// Последний сектор по которому кликнули мышкой.
		int			m_inSectLastMouseOver;		// Сектор над каторым в последний раз была мышка.
		int			m_inSectNeighbor;			// Соседей какого сектора подсветить.
	    
		// PLAYERS
		bool		m_bPlayerVisible;			// Героев видно или нет.
		bool		m_bPlayerEnable;			// Отрабатывать клики и тд. тп. на героях.
		bool		m_bPlayerHeroCapture;		// Герой схвачен мышкой.
		int			m_inPlayerInUseHero;		// Чей герой сейчас заюзан.
		int			m_inLastHeroClick;			// Последний герой по которому кликнули мышкой.
		bool		m_bHeroMiniMap;				// Отображать или не отображать малое мню героя.

		// PERSONAL HERO MENU
		bool		m_bPERSONAL_HERO_MENU;		// Отрисовка меню героя.

		// BRIEFING
		bool		m_bBriefingVisible;				// map-бриффинг: видно или нет.
		bool		m_bBriefingEnabled;				// map-бриффинг: активeн или нет.
		int			m_inAttackedSector;				// индекс сектора, атакованого игроком.
		
		// DIPLOMACY
		bool		m_bDiplVisible;				// дипломатия: активна или нет.
		int			m_inDiplActionPressed;		// нажатая кнопка дипл.действия (-1 - не нажата)
		int			m_inMapButtonPressed;		// нажатая кнопка отображения дипл.инфы (над картой) (-1 - не нажата) 

		// INFO DESK
		bool		m_bInfoVisible;				// desk c сообщениями: видны или нет.
		int			m_inInfoButtonPressed;		// нажатая кнопка сообщений

		// MESSAGES
		bool		m_bMessagesVisible;			// сообщения: видны или нет.

		// MARKET
		bool		m_bMarketVisible;			// рынок: активен или нет.

		// HELP
		bool		m_bHelpVisible;				// help: активен или нет.

		void	Init();	
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CStatusDRV    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Класс который является родителем для всех остальных.
class	CStatusDRV 
	{
	public:
		virtual	void	SetVisible(bool	state)	{};		//Вкл - выкл видимость объекта
		virtual	void	SetEnable(bool	state)	{};		//Вкл - выкл активности.
		virtual	void	CreateHint()			{};		//Создать хинт для объекта.
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CError    ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Класс хранения и обработки ошибок.
class	CError
	{
	public:
		CError();						//Записует NO_ERROR
		CError(char* pchError);			//Записует pchError, или NO_ERROR
	private:
		char	m_chErrorStatus[256];
	public:
		virtual	void	ResetError();						//Сбросить ошибки.	
		virtual	bool	IsError();							//Есть ошибка.	
		virtual	void	AddErrorMessage(char* pchError);	//Добавить к соощению о ошибке строку pchError.
		virtual	void	SendError(char* pchError);			//Заполнить значением ошибки.
		virtual	void	ShowError();						//Прервать выполнение, высветить ошибку.
		virtual	void	ShowError(char* pchError);			//Прервать выполнение, высветить ошибку pchError.
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////    CStorageData    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Клас предназначенный для сохранения данных на диск и их загрузку.
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
// Хранит не изменые данные полученные с диска.
class	CSectStatData : public  BaseClass	//CStatusDRV, public CError, public CStorageData
{
	public:
		CSectStatData();
		CSectStatData(DString* pDS);
		~CSectStatData();

		int					m_inID;						// Индекс сектора
		int					m_inDeffOwner;				// Начальный хозяин
		int					m_inMapOwner;				// Нация исторически находящаяся в секторе
		int					x0,y0;						// Координаты верхнего левого угла относительно карты
		int					m_curX,m_curY;				// Текуще пересчитанные координаты
		int					m_SityX,m_SityY;			// Пересчитанные координаты города
		int					m_FortX,m_FortY;			// Пересчитанные координаты точки
		_str				m_chSectorName;				// Имя сектора
		int					m_inNON;					// Кол-во соседей

		int					m_inNV;						// кол-во селений
		int					m_inSRID;					// индекс ресурса
		_str				m_chRes;					// название ресурса
		int					m_inPopulation;				// населенность сектора {0,1,2}
		int					m_inDefence;				// защита сектора {0,1,2,3}
		int					m_iRecruits ;				// рекруты
		int					m_iSabotageID;				// id саботажа на секторе +погода

		LinearArray<int,_int>	m_pinNIDS;				// Индексы соседей
		LinearArray<int,_int>	m_pinArrowD;			// Данные по стрелочкам
		int						m_inArrowOffset;		// Смещение по стрелочкам ля данного сектора

		SimpleDialog*		m_psdSec;					// адрес сектора в меню
		SimpleDialog*		m_psdSity;					// адрес города в меню
		SimpleDialog*		m_psdFort;					// адрес крепости в меню

	public:
		void	Init();								// Забить все нулями (не очищая).

		void	SetSecAddress(SimpleDialog* pSD);
		void	SetSityAddress(SimpleDialog* pSD);
		void	SetFortAddress(SimpleDialog* pSD);

		void	SetVissible(bool flag);
		void	SetDeffOwner(int PID);				// Выставить нового игрока по умолчанию.
		void	SetSectColor(int PID);				// Выставить цвет по игроку.
		void	SetColor(int color);				// Цвет без alpfa
		void	SetSectAColor(int color);			// Задать цвет с alpha.
		void	SetSectAlpha(int  a);				// Выставить alpha.	
        void	SetDeffColor();						// Выставить deffolt цвет
		void	SetSityType();						// Выставить нужный город.
		void	SetNewArrowOffset(int offset);		// Выставить смещение для стрелочек для этого сектора.
		
		bool	GetData(DString* pDS);				// Парсим файл данных на сектора.
        bool	ConvertCenters(int wnew,int hnew,int wold,int hold);	// Пересчитать координаты центров по реальной карте.

		void	DeposeTo(int x,int y);				// сместить вместе с картой.
		void	VissibleIfMouse();
		
		char*	GetSectorName();							// Название сектора. "NONEME"
		int		GetSecDefence();
		int		GetSecPopulation();

		char*	GetResourceName();							// Название ресурса. "NO"
		int		GetSectID();								// Получить ID сектора.
		int		GetResourceID();							// Индекс ресурса.
		int		GetVillageNUM();							// Кол-во селений.
		int		GetNNeighbor();								// Кол-во соседей.
		int		GetNeighborID(int idx);						// Индекс idx-овго соседа.			
		bool	GetMouseOver();								// Наведена мышка.
		int		GetDeffOwner();								// Получить начального хозяина.
		int		GetHistoryOwner() { return m_inMapOwner; };
		void	GetSityXY(int &x,int &y);					// Получить координаты города.
		void	GetCurXY(int &x,int &y);					// Пересчитанный центр сектора.
		bool	GetEnable();								// Состояние активности.
		void	GetAroowCoord(int& x,int& y,int& a,int n);
		int		GetNewArrowOffset();						// Получить смещение для стрелочек для этого сектора.
		SimpleDialog*	GetSityPic();						// Указатель на картинку сеектора.

		void	SetVisible(bool	state);						// Вкл - выкл видимость объекта.
		void	SetEnable(bool	state);						// Вкл - выкл активности.
		void	SetSectVisible(bool state);					// Задать видимость сектора.
		void	SetSityVisible(bool state);					// Задать видимость города.
		void	CreateHint();								// Создать хинт для объекта.
		void	CreateHint2();								// Создать NULL-хинт для объекта.

		SAVE( CSectStatData );
			REG_MEMBER	( _int, m_inPopulation	);
			REG_MEMBER	( _int, m_inDefence		);
		ENDSAVE;

};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    CSectDynmData    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Хранит данные изменяемые во время игры.
class	CSectDynmData : public BaseClass		//CError, public CStorageData		
{
	public:
		CSectDynmData();
		~CSectDynmData();

		int		m_inOwner;					// Владелец сектора.

		void	SetOwner(int PID);			// Задать владельца.
		int		GetOwner();					// Получить индекс владельца.

		SAVE( CSectDynmData );
			REG_MEMBER	( _int, m_inOwner );
		ENDSAVE;
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////    CSectData    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Хранит данные по всем секторам.
class	CSectData	: public BaseClass		//CStatusDRV, public CError, public CStorageData 
{
	public:
		CSectData();
		CSectData(char* DataFileName);
		~CSectData();

		DialogsSystem*	m_pdsMenu;				// Само меню.

		int				m_inSecOffset;			// Смещение по индексам в меню для секторов.
		int				m_inSityOffset;			// Смещение по индексам в меню для городов.
		int				m_inNOS;				// Кол-во секторов.
		int				m_fWM,m_fHM;			// Размеры карты из фотошопа.

		ClassArray<CSectStatData>	m_pssdDT;	// Статические данные  по сектору.		//CSectStatData*	m_pssdDT;
		ClassArray<CSectDynmData>	m_psddDT;	// Динамические данные по сектору.		//CSectDynmData*	m_psddDT;	
		
		int				m_inNewArrowsNum ;
		GPPicture*		m_pgppArows[ MAX_ARROW ];
		GPPicture*		m_pgppNewArrows[ MAX_ARROW * MAX_ARROW ];
		GPPicture*		m_pgppNewArrowsShad[ MAX_ARROW * MAX_ARROW ];				
		
		static	int		m_LastLMCSecID;							// Идекс последнего сектора по которому кликнули мышкой.

	public:
		void	Init();											// Забить все нулями (не очищая).

		void	UPDATE();

		void	CreateTempDataFiles();							// Создать файлы с данными по секторам.
		void	AddArrowPictures(LocalGP* pPic,int sprID);		// Добавить картинка стрелочек.
		void	AddNewArrowPictures(LocalGP* pPic,int n);		// Добавить картинка новых стрелочек.
		void	ReCountCoord(int wnew,int hnew);				// Перевести координаты в масштаб карты.
		void	DeposeTo(int x,int y,bool setDefColor);			// Сместить вместе с картой.
		void	SetSecOnMap(int offset, DialogsSystem* psdMenu);
		void	SetSityOnMap(int offset, DialogsSystem* psdMenu);
		
		void	DriveMouse(int x,int y);
		void	ViewNeighbor(int SecID,int Player,CPlayer* ppPlayer); // Подсветка секторов + большие стрелы.
		void	ViewNeighbor(int dx,int dy,int SecID,int Player,CPlayer* ppPlayer); // Стрелки маленькие //функция отрисовки стрелок
		void	InVisibleNeighbour(int SecID);

		int		GetSectorsNum();								// Кол-во секторов.
		void	GetSecSityXY(int ID,int &x,int &y);				// Координаты города по id сектора.
		int		GetNeighborsNum(int id);						// Получить кол-во соседей сектора.
		int		GetNeighborsIDs(int id,int* ids,int n);			// получить массив размера n, заполненный индексами соседей сектора
		int		GetSecOwner(int id);							// Идекс игрока владельца.
		int		GetHistoryOwner(int id);

		int		GetPlayerSect(int PID);							// Кол-во секторов игрока.
		int		GetPlayerSect(int* pSecID,int n,int PID);		// Получить массив секторов игрока.
		int		GetSecIDbyOwner(int NI);						// Получть "какой-то" сектор по владельцу территории.

		int		GetMouseOverSect();								// Сектор над которым сейчас мышка.
		SimpleDialog*	GetSityPic(int id);
		        				
		void	SetVisible(bool	state);							// Вкл - выкл видимость объекта
		void	SetEnable(bool	state);							// Вкл - выкл активности.
		void	SetSecVisible(int id,bool state);				// Сделать сектор видимым или невидимым.
		void	SetSecColor(int id,int color,bool visible);		// Задать цвет сектора и состояние видимости.
		void	SetDeffColor();									// Покрасить сектора в цвета владельцев.
		
		void	setRedDip	 (int PlayerId,int* pPID,int n);	// pPID - массив игроков которых надо покрасить.
		void	setGreenDip	 (int PlayerId,int* pPID,int n);	// pPID - массив игроков которых надо покрасить.
		//void	setRedDip(int* pPID,int n);		//void	setGreenDip(int* pPID,int n);
		void	setOwnerColor(int PID);							// закрасить все сектора игрока PID его цветом.
		void	setOwnerColor(int PID,int color,int alpha);		// закрасить все сектора игрока PID цветом.
		void	setArrowCoord(int x1,int y1,int x2,int y2,int id); // Выставить правильные координаты стрелочкам.
		void	setArrowVisible(bool state);					// Видимость стрелочек.

        void	CreateHint();									// Создать хинт для объекта.
        void	CreateHint2();									// Создать NULL-хинт для объекта.
   
		char*	GetSectorName(int id);							//
		int		GetSecDefence(int id);							//
		int		GetSecPopulation(int id);						//
		int		GetSecVillageNUM(int id);						// Кол-во селений.

		char*	GetResourceName(int id);						// Название ресурса. "NO"

		int		GetResourceID(int id);							// Индекс ресурса.
		int		GetNNeighbor(int id);							// Кол-во соседей.
		int		GetNeighborID(int id,int idx);					// Индекс idx-овго соседа.			
		int		GetDeffOwner(int id);							// Получить начального хозяина.
		void	GetSityXY(int id,int &x,int &y);				// Получить координаты города.

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
// Клас создания и отрисовки меню по секторам.
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

	GPPicture*		m_pDeLimit;			// разделитель
    
	int				m_iResNum;			// кол-во рес-в
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
// Класс с общими данными по отрядам и героям.
class	CUnit : public BaseClass //, public CStorageData
	{
	public:
		CUnit();
		~CUnit();

		static int	m_inLastID;			// Последний использованный индекс.

		_str		m_chName;			// Имя героя, название отряда.
		int			m_inID;				// Уникальный индекс.

		int			m_inPlayerID;		// Индекс игрока кому принадлежит юнит.
		int			m_inSectorID;		// Индекс сектора в котором находится отряд.
		int			m_inPreviousSecID;	//Сектор из которого сделан ход.

		bool		m_Alive;

		bool		m_Blink;			// мигание for unit - if(fuel>0)
		int			m_iBlinkStat;
		int			m_iBlinkTime;
		int			m_iBlinkLimit;

		SimpleDialog*	m_psdMapPic;	// Картинка которая отображается на карте.

		SimpleDialog*	m_psdPotret;	// Портрет героя, картинка юнита.
		SimpleDialog*	m_psdSector;	// Указатель на сектор, в котором находится юнит.

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

		virtual	void	Init();				//Инициализация.

		static	void	ResetLastID()		{ m_inLastID=0;						};
		static	void	AddLastID()			{ m_inLastID++;						};
		static	int		GetLastID()			{ AddLastID(); return  m_inLastID;	};
		static	void	SetLastID(int id)	{ m_inLastID=id;					};
		
		void	SetVisible(bool	state);			// Задать видимость.
		void	SetEnabled(bool	state);			// Задать доступность.

		void	SetName(char* name);			//Задать имя.
		void	setPlayerID(int id);			//Задать id игрока хозяина.
		void	SetSectorID(int id);			//id сектора где находится.
		void	setMapPic(SimpleDialog* psd);	// Картинка которая отображается на карте.
		void	SetPotret(SimpleDialog* psd);	// Портрет героя, картинка юнита.
		void	setMapPicXY(int x,int y);		// Установить координаты юнита на карте.
		void	SetPotretXY(int x,int y);		// Установить координаты в меню.
		void	setMapPicRR(SimpleDialog* psd);	//Поставить рядом с картинкой города.
		void	setMapPicRR();					//Поставить рядом с картинкой города.
		void	setMapPicLL(SimpleDialog* psd);	//Поставить рядом с картинкой города.
		void	setMapPicLL();					//Поставить рядом с картинкой города.
		void	setVisibleMapPic(bool stat);	// Выставить видимость отряда на карте.
		void	SetSector(CSectData* pSec,int id=-1);	// Установить сектор.
		void	SetSector(CSectData* psdS);		// Выставить первый сектор, какой найдем.
		void	setDeffColor();					// Задать цвет игрока - владельца.

		int				getSectorID();			// Индекс сектора в котором находится юнит.
		int				getPreviousSecID();		// Предыдущий сектор в котором мы ныходились.
		int				getUniqueID();			// Уникальный индекс.
		int				GetPlayerID();			// Индекс игрока владельца.
		char*			GetName();				// Имя героя, название отряда.
		SimpleDialog*	getMapPicPt();			// Указатель на элемент SipleDialog в меню.

		bool			CheckBlink();
		void			PrepareBlink();

		virtual	void	SetOnClickMap(pCULL_FUN_CLICK pf,int p);	//Задать обработчик клика на картинке юнита на карте.
		
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
// Отряд и все что с ним связанно.
class	CSquard	: public BaseClass
{
	public:
		CSquard();
		~CSquard();
	
		int		TypeGlobID;					// Глобальный индекс типа отряда.
		int		SQ_MAX;						// Максимальное кол-во отрядов данного типа.
		int		SQ_NUM;						// Кол-во отрядов данного типа.

		LinearArray<int,_int>	Kills;		// NKills

		_str	TypeName;			// Название типа отряда.		// Характеристики данного отряда.
		int		MeleeDMG;			// Повреждения от рукопашной атаки.
		int		RifleDMG;			// Повреждения от выстрела.
		int		GrenadeDMG;			// Повреждение от гранаты.
		//int Shield;		int StartMorale;
		//int		Cost[8];			// Стоимость одного отряда.		//int		Price;

		//struct  SDESCRIPTION {
		//	_str	TypeName;			// Название типа отряда.
		//	int		MeleeDMG;			// Повреждения от рукопашных атак.
		//	int		RifleDMG;			// Повреждения от выстрела.
		//	//int	GrenadeDMG;			// Повреждение от гранаты.
		//	//int	Shield; int	StartMorale;
		//	//int	Cost[8];			// Стоимость одного отряда.		//int		Price;

		//	void	Init();
		//	void	UPDATE(int NIndex);	// Обновить по NIndex (TypeGlobID) типа отряда.
		//}		DESCRIPTION;			// Характеристики данного отряда.
		//new

		void	Init();							//Сбросить все значения.
		
		//void	setFreeSQinArmy(int N);			//Кол-во свободных мест в армии.
		void	setTypeGlobID(int id);			//Выставить глобальный индекс тида.
		void	setSQ_MAX(int n);				//Поставить максимальное кол-во юнитов данного типа.
		void	setSQ_NUM(int n);				//Поставить кол-во отрядов данного типа.
	        	
		int		getTypeGlobID();				//Получить глобальный индекс типа отряда.
		int		getSQ_MAX();					//Получить максимальное кол-во юнитов данного типа.
		int		getSQ_NUM();					//Получить кол-во отрядов данного типа.

		//bool	addSQ();						//Попробовать добавить один отряд.
		bool	addSQ(int freeSQNum);			//Попробовать добавить один отряд.
		void	killSQ()		{ if( SQ_NUM > 0 ) SQ_NUM--; };					

		void	DecreaseARMY(bool st);			//Где-то добавлен отряд, уменьшить все остальные.
		void	DecreaseARMY(int n);			//Где-то добавлен отряд, уменьшить все остальные.
		void	IncreaseARMY(int n);			//убран отряд

		void	UPDATE();

		CSquard	operator=(CSquard& sq);			//Операция присвоения.

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
// Хранит лимиты по войскам для разных званий.
struct	SSquardData {
	SSquardData()		{ unitName = "NONAME";/*strcpy(unitName,"NONAME");*/ maxSquards=0; curID=0; };

	void	operator=(SSquardData& sdOld)	{	unitName = sdOld.unitName;
												/*strcpy(unitName,sdOld.unitName);*/ 
												maxSquards=sdOld.maxSquards; 
												curID=sdOld.curID;				};

	_str	unitName;			//Наименование отряда.
	int		maxSquards;			//Максимальное колво отрядов.
	int		curID;				//Текущий индекс в массиве NATIONS[0].Mon[curID]
};
//----------------------------------------------------------------------------------
struct	SRank {
	SRank();
	~SRank();

	void		Init();
	void		DeleteTYPE(int cid);			//Очистить по индексу страны.
	void		DeleteTYPE();					//Очистить все.
	void		CreateNTYPES(int cid,int n);	//Создать для страны с индексом cid, n типов отрядов.
	void		AddTYPE(int cid);				//Добавить стране один тип.
	
	_str			rankName;		//Название звания.
	int				rankID;			//Индекс ранга.
	struct SCUType {
		_str			country;		//Наименование страны.
		int				maxSquards;		//Кол-во отрядов всех типов разрешенных для использования.
		SSquardData*	TYPES;			//Отряды разрешенные для использования.
		int				TN;				//Кол-во разных типов отрядов.

		void	DeleteTYPES();			//Освободить память.
		void	DeleteZeroTYPE();		//Выкидываем типы с количеством войск = 0.
		void	operator =(SCUType& cut);
		void	operator+=(SSquardData& sd);
	}				TYPES_CONT[MAX_COUNTRYS];
};
//----------------------------------------------------------------------------------
class	CArmyLimit
{
public:
	CArmyLimit(char* fName);

	void	SetNSqdNID   ( int rankID, int countryID, int n, int id ); //Числовой идентификатор типа n-го разрешенного отряда.

	int		GetMaxDiffSqd( int rankID, int countryID );	//Максимамальное кол-во разных трядов.
	int		GetMaxSqd    ( int rankID, int countryID );	//Максимальное кол-во отрядов у героя.
	int		GetNSqdNum	 ( int rankID, int countryID, int n ); //Максимальное кол-во отрядов n-го разрешенного отряда.
	char*	GetNSqdID    ( int rankID, int countryID, int n ); //Идентификатор типа n-го разрешенного отряда.
	int		GetNSqdNID   ( int rankID, int countryID, int n ); //Числовой идентификатор типа n-го разрешенного отряда.
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

    void				INIT();												// Иннициализация.
	void				INIT( int x0, int y0 );								// Иннициализация, с установкой X0,Y0.

	void				AddBTNS_MENU( LocalGP*	pGP, DialogsSystem*	pMN );	// Добавить в меню кнопки.
	void				AddSTAT_MENU( LocalGP*	pGP, DialogsSystem*	pMN );	// Добавить в меню награды.
	void				AddFLAG_MENU( LocalGP*	pGP, DialogsSystem*	pMN );	// Добавить в меню знамена.
	
	void				SetMPos		( int x0, int y0 );						// Просто установить новую позицию меню.
	void				UpdateMenu	();										// Выставить на элементах новые надписии тд и тп.
	void				CheckVisible();										// Проверка видимости.

	void				SetVisible	( bool  state );						// Установить состояние видимости.
	void				SetEnabled	( bool  state );						// Установить состояние Enabled.

	void				SetName		( char* name );							// Задать имя героя.
	void				SetRank		( int	rank );							// Задать индекс звания.
	void				SetStat		( int	stat );							// Задать уровень наград.
	void				SetFlag		( int   flag );							// Задать знамя.
	void				SetAll		( CHero* pHero ) ;						// Задать все параметры героя в меню.
	void				SetAll		( char* name, int rank, int stat, int flag , CHero* pHero );// Задать все параметры героя в меню.

	void				SetAllBtn	( bool	stat );							//Статус пополнения и апгрейда для героя.

	void				GetMPos		( int& x0, int& y0 );					//Получить координаты меню.
	char*				GetName		();										//Получить Имя героя.
	char*				GetRank		();										//Получить индекс звания.
	int					GetStat		();										//Получить индекс уровня наград.
	int					GetFlag		();										//Получить знамя.
	void				GetAll		( char* name, char* rank, int& stat, int& flag );	//Получить все параметры.

	void				ArrangeIcons();				// Выставить координаты частей меню по X0, Y0.
	
	void				SetStatVisible(bool stat);
	void				SetFlagVisible(bool stat);

public:
	CHero*				m_pHero;

	int					X0,Y0;						//Начальная позиция интерфейса.

	_str				lpszName;					// Имя игрока.
	_str				lpszRank;					// Звание игрока.

	int					iSQ_NN;						// SQ-num
	int					iSQ_MAX;					// max SQ-num

	int					iExpa ;						// expa
	int					iExpPercent ;				// exp-percent for next level

	int					iNLExpa ;					// next level expa

	int					inStat;						// Уровени награжденности.
	int					inFlag;						// Знамя игрока.

	GPPicture*			pgppMenuBack ;				// BackGround for HERO_MINI_MENU
	GPPicture*			pgppStat[MAX_H_STATUS];		// Награды для игрока.
	GPPicture*			pgppFlag[MAX_PLAYERS+1];	// Знамена игроков.

	TextButton*			pgptbName;					// Имя игрока.
	TextButton*			pgptbRank;					// Звание игрока.
	
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
// Портреты героев, малеькие и большие.
class	CHERO_PORTRAIT
{
public:
	CHERO_PORTRAIT();
	~CHERO_PORTRAIT();

	void		addPortrait( LocalGP* pGP, LocalGP* pGPs, int nat );	//Добавить для конкретного игрока портреты.
	void		addToMenu  ( DialogsSystem* pMenu );	//Добавить картинки в меню.

	SimpleDialog*	getSmallP( int p, int n );	//Получить указатель на маленький портрет,p-Игрок,n-Герой.
	SimpleDialog*	getBigP  ( int p, int n );	//Получить указатель на большой портрет,p-Игрок,n-Герой.

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
	LocalGP*	pPORTRAIT  [ MAX_PLAYERS ];	//Портреты героев.
	LocalGP*	pPORTRAITs [ MAX_PLAYERS ]; //Маленькие портреты героев.

	GPPicture*	ppPORTsmall[ MAX_PLAYERS ][ MAX_HEROES ];	//Маленькие портреты, в меню.
	GPPicture*	ppPORTbig  [ MAX_PLAYERS ][ MAX_HEROES ];	//Большие портреты, в меню.

	//GPPicture**	ppPORTsmall[ MAX_PLAYERS ];	//Маленькие портреты, в меню.
	//GPPicture**	ppPORTbig  [ MAX_PLAYERS ];	//Большие портреты, в меню.
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////    CHero    ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Герой и все что с ним связанно.
class	CHero : public CUnit
	{
	public:
		CHero();
		~CHero();

	public:
		enum	CHeroState 
		{
				FREE,				//Просто отрисовать.
				CAPTURE				//Захвачен мышкой.
		}	m_CurStatus;			//Как обрабатывать в данный момент героя.

		CArmyLimit*	pArmyLimit;		//Характеристики разрешенной армии для героя.


		int		m_inRankID;				//Индекс звания.
		int		m_inStatID;				//Индекс заслуженности.
		int		m_inHeroID;				//Номер использованого героя.
		int		m_inFuel;				//Запас движения для героя.
		int		m_inExperience;			//Опыт героя, полученный в боях.

		SimpleDialog*	bigPotret;		//Большой портрет героя.

		ClassArray<CSquard>	ARMY;		//Армия.

		int		m_iFreeSQNum;			//Кол-во свободных мест в армии.

		int		m_iBatlN;				//Кол-во битв  (статистика)
		int		m_iVictN;				//Кол-во побед (статистика)

		int		m_iFormLost;				// lost   squads (статистика)
		int		m_iFormKilled;				// killed squads  (статистика)

		void	Init();			//Инициализация.

		void	SetOnClickMap(pCULL_FUN_CLICK pf,int p=0);	// Задать обработчик клика на картинке юнита на карте.
		void	SetOnRClickMap(pCULL_FUN_CLICK pf,int p=0);	// Задать правый клик по картинке юнита на карте.
		void	SetHeroID(int id);							// Выставить индекс использованого героя.
		void	SetBigPotret(SimpleDialog* pPic);			// Выставить большой портрет.
		void	SetVisibleMiniMenu(bool status);			// Состояние видимости малого меню.
		
		void	DisableHero();								// Отключить все клики на герое, + все сделать не видимым.
		void	ReEnableHero();								// восстановление существ.героя
		
		//---ARMY---
		void	SetARMY();
		void	SetDefaultARMY();							// Выставить армию по умолчанию.
		void	SetHeroDefeatArmy();
		void	SetSquardNumByType(int n, int type);
		void	AddSQbyArrID(int id);						// Добавить отряд по индексу в массиве.
		void	DelSQbyArrID(int id);  
		bool	KillOneSQUARD();							// Грохнуть один отряд в армии.

		int		GetARMY_NSQ();								// Получить кол-во отрядов в армии.
		int		GetBranch_NSQ( int branch );				// кол-во отрядов по типу войск
		int		GetARMY_FreeNSQ();							// Получить кол-во свободн.отрядов в армии.
		int		GetARMY_Power( bool realPower=false );		// сила отрядов в армии

		bool	DecreaseARMY(bool st);						// Где-то добавлен отряд, уменьшить все остальные.
		bool	DecreaseARMY(int n);						// добавлен отряд
		bool	IncreaseARMY(int n);						// убран отряд

		void	SetBattleNKills();
		void	SetNKills( LinearArray<int,_int>& NKills , int type );

		//---MOVEMENT---
		void	SetFuel(int Dist);							// Выставить горючего на передвижение.
		bool	AddFuel(int Dist);							// Добавить, отнять дистанцию от горючего.
    
		//---EXPERIENCE---
		void	AddExperience(int EXP);						// Добавить герою опыта.
		int		GetExperience();							// Получить значения опыта героя.
		void	UpdateRankWithExperience();					// Обновить звание по текущему значению опыта.
		
		//---HERO STATUS---
		void	SetRank(int rID);							// Выставить ранг герою.
		void	RaiseRank();								// Повысить в звании.
		void	LowerRank();								// Понизить в звании.
		void	AddBattle();
		void	AddVictory();
		int		GetRewardsStatus( int addStat = 0 );

		int		AddFormLost( int nn = 1 );
		int		AddFormKilled( int nn = 1 );
		int		GetFormLost();
		int		GetFormKilled();

		int		GetHeroID();								// Получить индекс героя.
		int		GetRank();
		int		GetStat();
		int		GetFuel();									// Получить запас хода героя.
				
		void	UpdatePic(CSectData* psdData);
		void	CreateMiniMenu();							// Малое меню для героя в левом углу.
		void	CreateHint();								// Создать хинт для героя.
		void	CreateEmptyHint();							// убрать хинт (создать пустой для героя)
		SimpleDialog*	GetBigPortret();

		bool	operator=(CHero& other);					// Операция присвоения.

		bool	HaveChance() { //подумать - делать или нет (шанс - одинаков для всех a la "c'est la vie")
			int  chance = ( rand()%10 )/ 4 ; // ( 10 - 4 ) - "4" хар-ка героя
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
	int		recruit;				// Рекруты для призыва в армию.

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
//	int				Treasury;	//Казна игрока.
//	CResource_LV	Resource;	//Ресурсы.
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
	int					mx0,my0,mx1,my1;		// Координаты карты для клипанья.
	int					sx0,sy0,sx1,sy1;		// Координаты всего экрана.
	DialogsSystem*		pMenu;					// Указатель на меню.
    CSectData*			pSectData;				// Указатель на сектора со всеми необходимыми данными.
	LocalGP*			pMapPic;				// Картинки для отрисовки на карте.
	CHERO_PORTRAIT*		pPORTRAIT;				// Портреты героев.
	CHERO_MINI_MENU*	pHMiniMENU;				// Мини меню для героя.
	CArmyLimit*			pArmyLimit;				// Данные по лимитам для армии героя.
	pCULL_FUN_CLICK		pfHeroClick;			// Клик на герое, адресс функции.
	pCULL_FUN_CLICK		pfHeroRClick;			// Правый клик на герое, адресс функции.
	};
//----------------------------------------------------------------------------------
// Хранит и отвечает за данные связанные с конкретным игроком.
class	CPlayer	: public CStatusDRV, public CError, public CStorageData
	{
	public:
		CPlayer();
		CPlayer(int ID);							// Задать индекс игрока 1..7
		CPlayer(int ID, char* Name);				// Задать индекс игрока 1..7, а также имя.
		~CPlayer();
	public:
		//BASE
		int				m_PlayerID;					// Индекс игрока, 1..7
		_str			m_chPlayerName;				// Имя игрока.

		GPPicture*		m_pgppPortH[ MAX_HEROES ];	// Массив указателей на мелкие портреты героев.

		DialogsDesk*	m_pDD;						//
		CSectData*		m_psdSectData;				// Указатель на массив секторов.

		static SFOR_PLAYER  DataForAddHero;
				
		// RESOURCES
		CResource_BM	Resource;					// Ресурсы игрока.	

		// HEROES
		int				m_inHeroNum;				// Кол-во героев игрока.
		CHero*			m_phrHero;					// Указатель на массив героев игрока.
		CHero			m_OneHero;					// герой игрока ( -Emergency Hero- )
		//CHero			m_phrHero[ MAX_HEROES ];	// массив героев игрока.

		// Player Individual Characteristics
		int				m_Aggressive ;				// парам.агрессивности страны
		bool			m_Alive;					// страна не уничтожена

		// HEROES
		//CHero*		m_phrHero;					// Указатель на массив героев игрока.
		// SQUADS
		//CSquard*		m_psdSquard;				// Указатель на массив отрядов игрока.
		//int			m_inSquardNum;				// Кол-во отрядов игрока.

	protected:
		void	Init();							//Инициализировать.

		bool	LoadSquard();					//Загрузить данные по отрядам игрока.
		bool	SaveSquard();					//Сохранить данные по отрядам игрока.
		bool	LoadHero();						//Загрузить данные по героям игрока.
		bool	SaveHero();						//Сохранить данные по героям игрока.

		bool	InitMemNHero	(int n);		//Выделить память под n героев.
		bool	InitMemNSquard	(int n);		//Выделить память под n героев.
		//void	DeleteHeroes();					//Удалить всех героев.
		void	DeleteSquard();					//Удалить все отряды.

		bool	IsHeroIDPresent(int id);		//Присутствует у игрока данный герой или нет.
	public:
		//BASE
		bool	Load();							//Загрузить все данные по армиям игрока.
		bool	Save();							//Сохранить все данные по армиям игрока.
		bool	UpdateSectID();					//Обновить указатели на сектора для героев и отрядов по ID секторов.
		bool	UpdatePic();					//Обновить все картинки.
        void	SetSetcData(CSectData* psd);	//Выставить указатель на сектора.
		
		void	AddPortH();						//Вставить картинки героев в меню сосокролером.
		void	SetPortHVisible(bool state);	//Выставить видимость для всех.
		void	ShowPortH();					//Отобразить только существующих героев.

		void	SetVisible(bool	state);			// Задать видимость.
		void	SetEnabled(bool	state);			// Задать доступность.

		bool	SetPlayerID(int ID);			//Задать ID игрока.
		bool	SetPlayerName(char* Name);		//Задать имя игрока.
		bool	AddMoney(int m);				//Прибавить или отнять определенное кол-во денег у игрока.

		char*	GetPlayerName();				// Имя игрока.
		int		GetPlayerMoney();				// Узнать сколько у игрока денег.
		
		bool	IsUnitInSect(int sid);			//Есть ли юниты героя в этом секторе.
		
		//SQUARD
		bool	AddSquard();					// Добавить отряд.
		bool	DeleteSquard(int ID);			// Удалить отряд.

		//HERO
		int		AddHero();									// Добавить героя.
		bool	AddHero(DialogsSystem* pds,LocalGP* mlgp,int mid);
		bool	AddHero(int sectID, int heroID=-1);			// Добавить героя и поставить в сектор sectID
		void	SetNewHeroData(int heroID=-1);
		bool	DeleteHero(int ID);							// Удалить героя.
		void	TryRetreat(CHero* pH,bool die);				// Попробовать отступить если не убили.
		void	SetHeroMiniMenuVisible(bool status);		// Статус отображения меню героев.
		void	TryShowSPortret(int idH);
		CHero*	GetHeroByPID(int pid);						// Получить указатель на героя по персональному индексу.
		CHero*	GetHeroInSector(int sid);					// Получить героя находящегося в секторе.
		bool	IsHeroPresentInSector(int id);				// Есть ли герой игрока в этом секторе.
		void	SetHeroFuel(int HID, int Dist);				// Залить герою горючего.
		void	SetHeroesFuel(int Dist);						// Залить героям горючего.
		void	SetDefaultARMY(int heroID=-1);				// Выставить армии по умолчанию.
		int		GetPlayerMoves();							// получить кол-во ходов у игрока - Sum( m_inFuel[i] )

		int		GetNewHeroID();
		void	DeleteHeroes();								// Удалить всех героев.
		
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
				CHero*	pH = &(this->m_phrHero[h]) ; //Массив героев
				if( pH->m_Alive )
					power += pH->GetARMY_Power(true);		// +army power
			}
			return  power;
		};

		bool	HaveChance() {
			int  chance = ( rand()%10 )/ (10 - m_Aggressive) ; // ( 10 - 4 ) - "4" парам.агрессивности страны!
			if( !chance ) return false;
			return true; 
		}

		bool	AGetTrue() { return true; }
	};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CPERSONAL    //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Меню героев игрока.
class	CPERSONAL 
{
public:
	CPERSONAL();
	CPERSONAL( int PlayerID );
	CPERSONAL( int PlayerID, int HeroID );
	~CPERSONAL();
	void			INIT();

	void			setPLAYERS(CPlayer** pPL);		//Выставть указатели на игроков учасников (всех).
    void			setPlayerID( int id );			//Выставить игрока для которого рисуем меню.
	void			setVisible( bool state, bool type=true );			//Установить состояние видимости.
	void			setUVisible( bool state );							//Состояние видимости юнитов.
	//void			setFarme( DialogsSystem* pMenu, LocalGP* pFR );		//Добавить рамки.
	void			setFarme(  DialogsSystem* pMenu, LocalGP* pFR , CArmyLimit* pAL ); //Добавить визуал.эл-ты.
	void			setUserClick(pCULL_FUN_CLICK pf, pCULL_FUN_CLICK pfR );	//Клик по иконке типа юнитов.
	void			setUnis( DialogsSystem* pMenu, CArmyLimit* pAL );	//Добавить картинки юнитов.

	void			UpdateCoord();					//Выставить координаты всех менюшек.
	void			UpdateUCoord();					//Выставить координаты для картинок юнитов.
	void			UpdateData();					//По игроку обновить всю менюху.
	void			UpdateData(int heroPID);		//Обновить по персональному индексу героя.
	void			UpdateSQ_NUM();					//Обновить кол-во отрядов у героя.

	void			UpdateInfo( CHero* pH ); 
	void			CreateUnitsHint( SimpleDialog* psd , int Nation , word NIndex );
	
	bool			CheckUnitsPages();

	bool	setEnabledButtons( bool state );//!!!new (VERY MUCH:) add if veeery need it!)

public:

	bool			m_Visible;						// видимость 

	int				m_PlayerID;						// Текущий отображаемый игрок.
	int				m_HeroID;						// Текущий отображаемый герой игрока m_PlayerID.
	int				m_iHumanNat;					// player nation from Save
	int				m_CurHeroId;
	
	int				m_iBlinkTime;
	bool			CheckPageBlink();


	static const int	m_nFH  =  4;				// Кол-во статических рамок.
	
	static const int	m_nFU  = 20;				// Кол-во рамок с юнитами в меню.
	static const int	m_nBT  = 2*m_nFU;			// Кол-во кнопок к юнитам.
	static const int	m_nTXT = 6;					// параметры-инфа героя

	static const int	m_nPgs = 3;					// Кол-во кнопок к юнитам.

	GPPicture*		m_pgppFrameH[ m_nFH  ];			// Статические рамки.
	TextButton*		m_ptbHeroPar[ m_nTXT ];			// параметры-инфа героя.

	GPPicture*		m_pgppFrameU[ m_nFU  ];			// Рамки отображающие юнитов в армии.
	TextButton*		m_ptbFrameUN[ m_nFU  ];			// Текущее кол-во юнитов у героя.
	GP_Button*		m_pgpbButton[ m_nBT  ];			// кнопки к юнитам.
	
	int				m_ActPage;						// активная закладка юнитов.
	GP_Button*		m_pgpbPages[  m_nPgs ];			// кнопки к закладкам юнитов.
	GP_Button*		m_pgpbPagBak[ m_nPgs ];			// кнопки к закладкам юнитов.

	GPPicture*		m_pgppBigPort;					// Большой портрет героя.
	DialogsDesk*	InfoDesk;						// DialogsDesk for text info about hero
	TextViewer*		HeroDesc;						// textview
	TextButton*		ptbHeroDesc;					// text info about hero
	
	SimpleDialog*	m_ppUnitsPic[ m_nFU ] ;			// Текущие отображаемые юниты.

	struct SUNIT_TYPE {
		DynArray<SimpleDialog*>		UNIT_PIC;		// Картинки юнитов.
		DynArray<int>				UNIT_NID;		// Индексы картинок юнитов.
	}				m_utUNITS;

	CPlayer*		m_pPLAYERS[MAX_PLAYERS];		// Все игроки на карте.
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////    CPicesPict    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Отрисовывает карту.
class	CPicesPict : public CStatusDRV, public CError
	{
	private:
		CPicesPict();
	public:
		CPicesPict(DialogsSystem* pMenu,int NX,int NY,int FrOffset);
	//	~CPicesPict();
	private:
		DialogsSystem*	m_pMenu;			//Само меню.
		int				m_NX,m_NY;			//Кол-во картинок, ширина и высота.
		int				m_FrOffset;			//Смешение картинок в меню.
		int				X0,Y0;				//Левый верхний угол экрана.
		int				m_x0,m_y0;			//Левый верхний угол карты.
		int				m_H,m_W;			//Ширина и высота экрана.
		int				m_PW,m_PH;			//Реальная ширина и высота картинки.

		int				m_mxo,m_myo;		//Прежние координаты мышки.
		bool			m_bBegMove;			//Начать движение карты.
		
		//grey
		GPPicture*	pgppMapBorder ;		// Map Border
		
	private:
		void	CalculateCoord();			//Расставить картинки.
		bool	AddX(int dx);		//Прибавить по x
		bool	AddY(int dy);		//Прибавить по y
		void	MoveX(int dx);
		void	MoveY(int dy);
	public:
		void	SetScreen(int x0=0,int y0=0,int H=768, int W=1024); //Установить ширину и высоту экрана.
		void	GetMapSize(int &dx,int &dy,int &w,int &h);

		void	ScrollPic(int mx,int my);
		void	MoveWithMouse();		//Тянем карту за мышкой.
		void	SetVisible(bool	state);		//Вкл - выкл видимость объекта
		void	SetEnable(bool	state);		//Вкл - выкл активности.

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
		int					m_inCurTurn;						// номер текущего хода ( для всех желающих )
		int					m_inCurActiveAI;					// для AI_PROCESS
		int					m_inCurHeroMove;		 			// для AI_PROCESS
		int					m_inAttackedNation;
		
		int					m_iBigMap_FOOD_RATIO;				// соотн.еды на страте
		int					m_iBigMap_AI___RESOURCES;			// кол-во рес-в на стаpте
		int					m_iBigMap_User_RESOURCES;			// кол-во рес-в на стаpте
		float				UpdateResDiv[DIFF_LEVELS_N];		// делитель для дохода ресурсов - f(diff)

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
		DWORD				m_iMapColor[  2*MAX_PLAYERS ];		// цвета раскраски карты
		int					m_iNationID[  2*MAX_PLAYERS ];		// Nation identify sprites color

		int					m_CheatsStage;						// current cheats stage
		TextButton*			m_ptbText;							// cheats interface console
};
////////////////////////////////////////////////////////////////////////////////////
// Обрабатывает и выставляет сценарий.
class	CROOTDRIVER : public CROOT_CONST_DATA
	{
	public:
		CROOTDRIVER();
		~CROOTDRIVER();
	public:
		DialogsSystem*		m_pMenu;				// Меню.

		CPicesPict*			m_pMap;					// Карта.
		CSectData*			m_pSect;				// Сектора.
		CSectorMenu*		m_pSectMenu;			// Sectors menu.
		CHERO_PORTRAIT*		m_pHp;					// Hero portrait.
		CHERO_MINI_MENU*	m_pHmm;					// Мини меню для героя.
		CPERSONAL*			m_pHPers;				// Персональное меню героя.
		CHeroPersDesk*		m_phpDesk;				// DialogsDesk для CPERSONAL 
		CPlayer*			m_pPlayer[MAX_PLAYERS];	// Игроки.
		CDiplMenu*			m_pDipl;				// Дипломатия.
		CDiplInfoMessages*	m_pMessages;			// инф.сообщения.
		CBriefing*			m_pBriefing;			// Брифинг по миссии.
		CBigMapQuest*		m_pQuest;				// global quests on Big Map
		CResPanel_BM*		m_pResPanel;			// resource panel in Main Menu
		CMarketDesk*		m_pMarket;				// global market on Big Map
		CBigMapHelp*		m_pHelp;				// global bigmap help

		CScenarioLV			m_Scenario;				// Текущий сценарий обработки менюх.
		CPlayerSAVE*		m_pPLAYER_SAVE;			// указатель на SAVE игрока

		bool				m_bSaveLogs;			// нужны ли log-файлы 
		bool				m_bEndGame;			

		//new
		bool	m_ShowHeroMove0;
		bool	m_EnabledMapClick;

	public:
		void	Init();									// Полный сброс всех параметров.
		int		GetFreePlayerPos();						// Позиция свободная для заполнения.
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

		CHero*	GetHeroInSect( int sid ,int NOT_NAT );				// Получить героя в этом секторе.
		int		GetSectList( int player, int startSecId, int* list );

		// единичная обработка (вызов по необходимости) 
		void	UPDATE_SCENARIO();									//Выставить всем классам их состояния
		// постоянная обработка действий соответственно текущему сценарию (в цикле)
		void	PROCESS(int& g_dXM,int& g_dYM,int& g_WM,int& g_HM, int playerNation, int Diff );//Выполнить действия соответcвующие текущему сценарию 

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

		bool	CheckHotKeys();										// проверка горячих клавиш в меню!

		bool	CheckRightsForHeroInSector(CHero* pHero);			// проверка на право находиться в секторе: это сектор героя либо есть право прохода

		bool AGetTrue() { return true; }
	};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// Класcы для чтения и хранения данных по дипломатии ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------CDiplomacyData--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Хранит дипл.данные изменяемые во время игры.
class	CDiplomacyData : public BaseClass //, public CError
	{
	public:
		CDiplomacyData(CSectData* pSectData);
		CDiplomacyData();
		~CDiplomacyData();

		int			m_iPlayerId;		// id страны игрока (PlayerId)
		int			m_iConCost;			// стоимость заключения договора 
		int			m_iMinimumCost;		// мин.стоим.договора

		LinearArray<int,_int>	LACon;	// массив договоров
		LinearArray<int,_int>	LAReq;	// массив запросов к игроку на договор

	private:
		CSectData*	m_pSectData;		// секторы
		CDiplMenu*  m_pDiplMenu;		// меню дипломатии (только для денег за дипл.договора! <для AddContractMoney&&SetContractCost> )

		int			m_iFields;			// кол-во полей в записи договора = 4
		int			m_iTypeNum;			// кол-во типов договоров = 5

		int			m_iWarId;			// id объявления войны = 0
		int			m_iPeaceId;			// = 1
		int			m_iPactId;			// = 2
		int			m_iPassId;			// = 3
		int			m_iAllyId;			// = 4	
		int			m_iUnionId ;		// = 5
		
		int			m_iSecWarId;		// = 10		
		int			m_iSecPeaceId;		// = 11		
		int			m_iSecPassId;		// = 13


		int		DeleteContract(int type, int country1, int country2);			// удалить договор
		int		AddNewContract(int type, int country1, int country2, int time); // добавить договор
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
		
		void	OnFirstInit();								//заполняет договорами о мире "все со всеми"
		void	SetDiplMenu(CDiplMenu* pDiplMenu); 

		int		GetPlayerNId();								//возвр.id страны игрока - m_iPlayerId
		int		GetWarId();									//возвр.id объявления войны =0
		int		GetPeaceId();		
		int		GetPactId();		
		int		GetPassId();		
		int		GetAllyId();	
		int		GetUnionId();	

		int		GetSecWarId();	
		int		GetSecPeaceId();	
		int		GetSecPassId();	

		int		GetDiplActionPicture(int type,int country1,int country2);
		
		// договора между странами
		int		GetContNum();								// возвр.кол-во эл-тов массива договоров
		int		GetContractsSize();							// возвр.кол-во эл-тов,занимаемых полями действующих дог-в
		int		GetContractsNumber();						// возвр.кол-во контрактов

		int		SetContractCost( int turn );				// увелич.стоим.контракта
		int		GetContractCost(int type);					// возвр.стоим.контракта
		int		GetContractCost2( int type, bool change ) ;	// возвр.стоим.контракта в зависимости от денег игрока 
		int		GetConCost();								// возвр. m_iConCost

		int		GetContractsById(int type, int playerId, int* Id); //возвр.кол-во n дог-в,и массив id стран,с кот.закл.контр.(Id[n])
		int		GetContTypesById(int playerId, int secondId, int* types); //возвр.кол-во n разных типов дог-в(types[n])
		int		GetIdByContType (int type, int* players);				  //возвр.кол-во n id (PlayerId[n])

		int		GetContractsNumByType( int type, int playerId );
		bool	CheckContract( int Type, int playerId, int secondId );
		int		GetContractPeriod( int type, int playerId, int secondId );

		void	RecountContractsPeriod(int* conts, int& n);
		
		int		AddContractForPlayer(int type, int time);
		int		AddContract(int type, int country1, int country2, int time);

		//запросы к игроку на договор 
		int		GetReqNum();												// возвр.кол-во эл-тов массива запросов
		bool	ClearRequests();
		int		AddRequestForContract(int type, int country1, int time);	// добавить запрос на контракт
		int		ConfirmRequest(int requestId);								// закл.конракт по запросу
		int		GetRequestId(int type, int country, bool both = false );	// получить id запроса на контракт 
		int		GetRequestId(int type, int country1, int country2);//new	// получить id запроса на контракт 
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenuInfo-----------------------------------------
//----------------------------------------------------------------------------------

//хранит данные о textbuttonах и картинках дипл. инфы в меню дипл.
class	CDiplMenuInfo : public CStatusDRV, public CError	
	{
	public:
		CDiplMenuInfo(DialogsSystem* pMenu, CSectData* pSectData, int N, int ButOffset);
		~CDiplMenuInfo() {
			return ;
		}
		//CDiplMenuInfo(DialogsSystem* pMenu,CSectData* pSectData, GPPicture* pDipBack ,GPPicture* pPicture,int N,int ButOffset); 
	public:
		DialogsSystem*	m_pMenu;								// само меню.
		CSectData*		m_pSectData;							// секторы

		TextButton*		m_pCounName;							// назв.страны
		GPPicture*		m_pFlag;								// флaг страны
		GPPicture*		m_pDipBack;								// картинкa-фон для фото дипломата
		GPPicture*		m_pPicture;								// картинкa страны
		GPPicture*		m_pDeLimit;								// разделитель
		DialogsDesk*	m_pStateVerBar;							// vertical   полоскa в таб.дипл.отн-й	
		DialogsDesk*	m_pStateHorBar;							// horizontal полоскa в таб.дипл.отн-й	
		GPPicture*		m_pStateBack;							// картинкa состояний дипл.отнош.
		GPPicture*		m_pStateFlags[MAX_PLAYERS*2];			// flags

		TextButton*		m_pSTATES[MAX_PLAYERS][MAX_PLAYERS];	// назв.дипл.отнош.

		int				m_iN;									// кол-во  
		int				m_iPicOffset;							// смещение в меню картинки
		int				m_iTxtOffset;							// смещение в меню полей инфы
		int				m_iCurrSecId;							// id отображаемого сектора 

		_str			m_chCountryName;						// поле #1
		_str			m_chResource;							//		#2 
		_str			m_chDefence;							//		#3
		_str			m_chPopulation;							//		#4
		_str			m_chVillages;							//		#5
		_str			m_chState;								//		#

		char			m_chStat[6][50];						// краткое назв. состояний дипл.отнош. #CDT_ST_WAR
		char			m_chStatHint[6][256];						// hint состояний дипл.отнош. #CHINT_ST_WAR

		bool			m_Visible;								// видимость 
		bool			m_Enabled;								// доступность 

	public:
		void	CreateElements();

		void	SetVisible(bool	state);					// вкл-выкл видимость 
		void	SetEnabled(bool	state);					// вкл-выкл доступность 
		
		int		SetCurrSecId(int id);					// запомнить id отображаемого сектора

		bool	ChangeSecInfo( int id , CPlayer* pPlayer[MAX_PLAYERS] ); //установка инфы по	SecId
		//bool	ChangeSecInfo(int id);					//установка инфы по	SecId

		void	SetCountryPicture(int id);				// картинкa страны
		void	SetCountryName(char* name);				// назв.страны
		void	SetResource(char* name);				// назв.ресурса
		void	SetState(int value);					// состояние
		void	SetDefence(int value);					// оборона
		void	SetPopulation(int value);				// население
		void	SetVillages(int value);					// кол-во селений
		
		bool	ShiftTableBars( int nation );	

		bool	SetValueById(int id, char* value);		// установить Message = value
		int		GetTxtOffset();							// значение смещения полей инфы
		
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenuCountries------------------------------------
//----------------------------------------------------------------------------------

//хранит данные о textbuttonах дипл. инфы в меню дипл.
class	CDiplMenuCountries : public CStatusDRV, public CError	
	{
	public:
		CDiplMenuCountries(DialogsSystem* pMenu,int N,int TxtOffset); 
	private:
		DialogsSystem*	m_pMenu;		//Само меню.

		int				m_iN;			//Кол-во  
		int				m_iTxtOffset;	//Смещение  в меню.

		bool			m_Visible;		//видимость 

	public:
		void	SetVisible(bool	state);						//Вкл-выкл видимость 
		void	SetVisible(bool	state, int param);			//Вкл-выкл видимость 2
		
		bool	SetValueById(int id, char* value);			//присваивание текста в textbutton
		void	SetValues(char** ppNames);					//присв.списка назв.стран в сост.войны	
		void	SetDealValues(int param, char** ppNames);	//присв.списка назв.стран с текущим дипл.договором	
		int		GetTxtOffset();								//значение смещения полей инфы

};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenuButtons--------------------------------------
//----------------------------------------------------------------------------------

//хранит данные о кнопках дипл. действий 
class	CDiplMenuButtons : public CStatusDRV, public CError		
	{
	public:
		CDiplMenuButtons(DialogsSystem* pMenu,int N,int ButOffset);

		DialogsSystem*	m_pMenu;			//Само меню
		GP_TextButton*	m_pMainOK;			//кнопка OK меню
		GP_TextButton*	m_pMainNO;			//кнопка NO меню

		Enumerator*		m_pMessHeads;		//enum "MessageHeads"	
		Enumerator*		m_pMessTexts;		//enum "MessageTexts"

		int				m_iN;				//Кол-во кнопок 
		int				m_iButOffset;		//Смещение кнопок в меню.
		int				m_iTxtOffset;		//Смещение подписи к кнопкам в меню.
		int				m_iActive;			//активная кнопка			
		//int			m_iPressed;			//нажатая кнопка				//bool m_IsActive; //активность кнопок //bool m_IsPressed; //нажата кнопка //bool m_Redraw; //необходимость перерисовки кнопок	

		bool			m_ChangeFont;		//флаг смены фонтов на кнопках
		bool			m_Visible;			//видимость кнопок
		bool			m_Enabled;			//доступность кнопок

		//bool	SetDisabledFont(); 
		//bool	MainMenuEnabled(CSectData* pSectData,bool state);				//доступность основного меню кампании

	public:
		bool	SetMainOK(GP_TextButton* pGPMainOK);							//кнопка OK меню
		bool	SetMainNO(GP_TextButton* pGPMainNO);							//кнопка NO меню
		
		int		GetOffset();													//значение смещения кнопок в меню

		void	SetVisible(bool	state);											//Вкл-выкл видимость 
		void	SetEnabled(bool	state);											//Вкл-выкл доступность 
		void	SetDisabledButtons(CDiplomacyData* pDiplData, int* type, int n);//выборочный disable кнопок
		int		SetActive(int active); 

		bool	DiplActionPressed(int ActionActive);
		bool	StartDiplAction(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],CDiplMenuButOnMap* pMapButt,int DiplAction); 	 
		bool	ShowDiplMessage(CSectData* pSectData,CDiplomacyData* pDiplData,CPlayer* pPlayer[MAX_PLAYERS],int ContMoney,int MessId); 

		bool	SetDisabledFont(); 
		bool	MainMenuEnabled(CSectData* pSectData,bool state, int pageN );	//доступность основного меню кампании

		void	CreateHint();
	
};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenuButOnMap-------------------------------------
//----------------------------------------------------------------------------------

//хранит данные о кнопках отображения дипл. инфы на карте
class	CDiplMenuButOnMap : public CStatusDRV, public CError	
	{
	public:
		CDiplMenuButOnMap(DialogsSystem* pMenu,int N,int Pressed1,int Pressed2,int ButOffset); 
	private:
		DialogsSystem*	m_pMenu;				// Само меню.

		int				m_iN;					// Кол-во кнопок 
		int				m_iButOffset;			// Смещение кнопок в меню.
		int				m_iPressed1;			// первая нажатая кнопка		
		int				m_iPressed2;			// вторая нажатая кнопка		
		int				m_iActive;				// активная	кнопка			//bool m_IsActive; //активность кнопок

		bool			m_Visible;				// видимость кнопок
		bool			m_Enabled;				// доступность кнопок
		
		//new
		GPPicture*		m_pFramePic;


	public:
		void	CreateElements();

		void	SetVisible(bool	state);								// Вкл-выкл видимость 
		void	SetEnabled(bool	state);								// Вкл-выкл доступность 

		int		ShowActiveButton(int MapActiveButton);				// смена активной кнопки
		bool	ShowPressedButton(int* PressedButton, int SecID);	// смена нажатой кнопки
		void	ShowHighlightSectors();								// подсветка секторов

		int		GetButOffset();										// значение смещения кнопок в меню
		int		GetFirstPressed();									// возвр.N - первая нажатая кнопка
		int		GetSecondPressed();									// возвр.N - вторая нажатая кнопка

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplProposes-----------------------------------------
//----------------------------------------------------------------------------------

// ---CDiplProposes--- хранит данные об окнах дипл. предложений игроку
class	CDiplProposes	//NOT USED!!!
	{
	public:
		CDiplProposes(DialogsSystem* pMenu,int N,int ProOffset);
	private:
		DialogsSystem*	m_pMenu;				//cамо меню.

		int				m_iN;					//Кол-во окон
		int				m_iButOffset;			//смещение в меню.
		int				m_iActive;				//активное окно
		
		bool			m_Visible;				//видимость 
		bool			m_Enabled;				//доступность

	public:
		void	SetVisible(bool	state);					//Вкл-выкл видимость 
		void	SetEnabled(bool	state);					//Вкл-выкл доступность 

		void	OpenNewWindow( int DiplAction, int PlayerId );
		void	OnCloseWindow();
	};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplInfoMessages-------------------------------------
//----------------------------------------------------------------------------------

//структура для хранения сообщений игроку
class	CDiplMessage_STRUCT : public BaseClass
{ 
public:
	CDiplMessage_STRUCT();
	~CDiplMessage_STRUCT();
public:
	_str	Letter;				// надпись на кнопке сообщения		
	_str	MessHead;			// заголовок сообщения				
	_str	MessText;			// текст сообщения					

	int		pictId;				// id картинки к сообщ.
	int		MessType;			// 0 - no buttons.  2 - ok/close.  3 - agree/no/close.  4 - ok/no/close.
	int		reqId;				// id запроса на контракт в DiplomacyData
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
		_str	Letter;			//надпись на кнопке сообщения		
		_str	MessHead;		//заголовок сообщения				
		_str	MessText;		//текст сообщения					
		int		pictId;
		int		MessType;		// 2 - ok/close; 3 - agree/no/close; 4 - ok/no/close
		int		reqId;			// id запроса на контракт в DiplomacyData
		int		status;			// readed (yes/no) = -1  /  canceled (cancel) = 1
		int		sortId;			// sorting by sortId
};*/

//----------------------------------------------------------------------------------

//хранит данные об информационных сообщениях игроку
class	CDiplInfoMessages : public BaseClass		//, public CError //, public CStorageData
	{
	public:
		CDiplInfoMessages( CDiplInfoMessBox* pMessBox,  CDiplInfoButtons* pInfoButtons,
						   CDiplomacyData*   pDiplData, CDiplMenuInfo*    pInfoTexts,   CDiplInfoPanel*	pInform, 
						   int n );
		CDiplInfoMessages();
		~CDiplInfoMessages();
	public:
		ClassArray<CDiplMessage_STRUCT>   CAMess;	//массив инф.сообщ.

		CDiplomacyData*		m_pDiplData;  
		CDiplMenuInfo*		m_pInfoTexts; 
		CDiplInfoPanel*		m_pInform;
		
		int					m_iActive;				//активное сообщение
		bool				m_NextRound;

		bool				m_Visited;				// просмотрен или нет
		bool				m_Blink;				// мигание закладки "Messages"
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
		void	SortMessages();						// сортировка сообщений		

		void	Process( int Pressed );
		void	DrawButtons();
		void	ShowMessage(int Pressed);			// отобразить сообщение на экран
		bool	ShowMessageDesk(bool state);		// вкл-выкл активность desk-a
		int		SetActiveMessage(int active);
		int		SetPassiveMessage();
		
		int		GetMessageId(int requestId);

		bool	AddMessage(int priority,int MessageType,int PictureId,char* ButtonLetter,char* MessageHead,char* MessageText, bool NewMess=false );
		bool	AddMessage(int priority,int MessageType,int PictureId,char* ButtonLetter,char* MessageHead,char* MessageText,int reqId, bool NewMess=false );

		bool	ClearMessages();					// удаление всех сообщений
		bool	ConfirmMessage();					// подтвержд.заключения дог. + удал.текущ.сообщ.
		bool	DeleteMessage();					// закрыть текущ.сообщ.					/*удалить*/
		bool	CloseMessage();						// закрыть текущ.сообщ.

		//new
		//bool	DeleteMessage2();					// закрыть текущ.сообщ. + удал.текущ.сообщ.
		bool	DeleteMessage2(bool del=true,bool accept=false,int sid=-1); // закрыть текущ.сообщ. + удал.текущ.сообщ.

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

// данные о Desk с сообщениями - visual class!!! (с инфой для игрока по глобальной карте)
class	CDiplInfoPanel 
	{
	public:
		CDiplInfoPanel( DialogsSystem* pMenu, int Offset); 
	public:
		DialogsSystem*		m_pMenu;				// само меню
		DialogsDesk*		m_pMainDesk;			// desk for All page elems
		DialogsDesk*		m_pDesk;				// desk for messages

		GP_TextButton*		m_pgptbStatx;			// кнопка статистики
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

		int					m_iOffset;				// смещение кнопок в меню.
		int					m_iX0;					// координата x0 лев.верхн.угла позиции кнопок
		int					m_iY0;					// координата y0 лев.верхн.угла позиции кнопок
		int					m_iStepY;			
		int					m_iN;					// кол-во кнопок				
		int					m_iFree;				// порядковый номер свободной кнопки; при m_iN=0 - m_iFree=0

		bool				m_Visible;				// видимость кнопок
		bool				m_Enabled;				// доступность кнопок

		int					m_iMessElem;
		int					m_iInfoMessages;

	public:
		void	Init();

		int		GetOffset();										//значение смещения кнопок в меню
		int		GetNumber();										//значение m_iN

		void	SetVisible(bool	state);								//Вкл-выкл видимость 
		void	SetEnabled(bool	state);								//Вкл-выкл доступность 

		void	SetActive(int n);
		void	SetPassive(int n);

		int		Add();												//добавить новoe
		void	AddMessage( CDiplMessage_STRUCT* pMess, int n );	//добавить сообщениe		
		void	Close();											//очистить

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

//информационные кнопки (с инфой для игрока по глобальной карте)
class	CDiplInfoButtons	//NOT USED!!!
	{
	public:
		CDiplInfoButtons(DialogsDesk* pDesk, GPPicture* pRulon, int ButOffset); 
	private:
		DialogsDesk*		m_pDesk;				//desk
		GPPicture*			m_pRulon;				//кнопка вкл/выкл. активности desk
		
		int					m_iButOffset;			//смещение кнопок в меню.
		int					m_iX0;					//координата x0 лев.верхн.угла позиции кнопок
		int					m_iY0;					//координата y0 лев.верхн.угла позиции кнопок
		int					m_iN;					//кол-во кнопок				
		int					m_iFree;				//порядковый номер свободной кнопки; при m_iN=0 - m_iFree=0

		bool				m_Visible;				//видимость кнопок
		bool				m_Enabled;				//доступность кнопок

	public:
		int		GetButOffset();							//значение смещения кнопок в меню
		int		GetNumber();							//значение m_iN

		void	SetVisible(bool	state);					//Вкл-выкл видимость 
		void	SetEnabled(bool	state);					//Вкл-выкл доступность 

		void	SetActive(int n);
		void	SetPassive(int n);

		int		AddButton();							//добавить новую кнопку
		void	AddMessageButton(int n, char* mess);	//добавить кнопку сообщения		
		void	CloseButtons();							//очистить

		bool	CheckRulonVisible(bool state);
		void	SetDeskVisibled();

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplInfoMessBox--------------------------------------
//----------------------------------------------------------------------------------

//информационные окна
class	CDiplInfoMessBox	//NOT USED!!!
	{
	public:
		CDiplInfoMessBox(DialogsSystem* pMenu,int BoxOffset); 
		CDiplInfoMessBox()  { return; }
		~CDiplInfoMessBox() { return; }
	private:
		DialogsSystem*		m_pMenu;				//меню
		GPPicture*			m_pgppBox;				//спрайт окна сообщения
		GPPicture*			m_pgppPic;				//картинка
		TextButton*			m_ptbHead;				//заголовок 
		//TextButton*			m_ptbText;				//текст сообщения
		GP_TextButton*		m_pgptbYes;				//кнопка Yes-Agree-Ok
		GP_TextButton*		m_pgptbNo;				//кнопка No
		GP_TextButton*		m_pgptbClose;			//кнопка Close

		TextViewer*			m_ptvText;				//текст сообщения

		_str				m_chOK;
		_str				m_chYes;		

		int					m_iBoxOffset;			//смещение в меню.
		int					m_iN;					//кол-во элементов окна
		int					m_iX0;					//координата x0 лев.верхн.угла  
		int					m_iY0;					//координата y0 лев.верхн.угла  
		int					m_iMessType;			//id внеш.вида окна

		int					m_iCurrMess;			//id текущего сообщения 

		bool				m_Visible;				//видимость 
		bool				m_Enabled;				//доступность 

	public:
		int		GetBoxOffset();						//значение смещения эл-в окна в меню

		void	Init();								//
		void	PrepareMessBox();					//
		bool	SetCallFunClicks(CDiplInfoMessages* pMs, pCULL_FUN_CLICK yes, pCULL_FUN_CLICK no, pCULL_FUN_CLICK close);

		void	Show();								//
		void	Close();
		void	SetVisible(bool	state);				//Вкл-выкл видимость 
		void	SetEnabled(bool	state);				//Вкл-выкл доступность 

		bool	SetMessage( CDiplMessage_STRUCT* pMess, int n );
		
		bool	ShowGlobalMessage(int x, int y, CDiplMessage_STRUCT* pMess, int n ) { return true;}

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------CDiplMenu---------------------------------------------
//----------------------------------------------------------------------------------

//хранит данные об элементах управления меню дипл.   (диспетчерский класс для меню дипломатии)
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
		DialogsSystem*			m_pMenu;					// cамо меню.

		CSectData*				m_pSectData;				// секторы	
		CSectorMenu*			m_pSectMenu;				// меню сектора
		CDiplomacyData*			m_pDiplData;				// дипл.данные	
	
		CDiplMenuInfo*			m_pInfoTexts;				// информация по сектору
		CDiplMenuCountries*		m_pWarsTexts;				// список стран (войны)
		CDiplMenuCountries*		m_pDealsTexts;				// список стран
		CDiplMenuButtons*		m_pActButtons;				// кнопки дипл.действий
		CDiplMenuButOnMap*		m_pMapButtons;				// кнопки над картой
		CDiplInfoMessages*		m_pMessages;				// информационные сообщения игроку
		CDiplInfoButtons*		m_pInfoButtons;				// кнопки инф. сообщений игроку
		CDiplInfoMessBox*		m_pInfoBox;					// окно инф. сообщений игроку
		CPlayer*				m_pPlayer[MAX_PLAYERS];		// игроки

		Enumerator*				m_pMessLtrs;				// enum "InfoLetters" 
		Enumerator*				m_pMessInfo;				// enum "InfoTexts"  
		Enumerator*				m_pMessHeads;				// enum "MessageHeads" 
		Enumerator*				m_pMessT;					// enum "MessageTexts"

		int						m_iST[MAX_PLAYERS][MAX_PLAYERS]; // hints

		bool					m_Visible;					// видимость меню
		bool					m_Enabled;					// доступность меню

	public:
		bool	operator+=	(CPlayer*	pPlayer);					// добавить указатели на игроков
		bool	operator+=	(CDiplMenuInfo*		pInfoTexts);		// добавить указатель
		bool	operator+=	(CDiplMenuButOnMap*	pMapButt);			// добавить указатель


		bool	Show( bool state, bool act );						// Вкл-выкл активность меню
		void	SetVisible( bool state, bool act );					// Вкл-выкл видимость 
		void	SetEnabled( bool state, bool act );					// Вкл-выкл доступность 
		
		void	Process( int* MapPressedButton,int DiplAction, int ActiveMenu,bool MenuIsChange , bool ActEn );//обработка событий в меню
		
		void	SetSectorsColor(int state1, int state2);			// раскраска секторов цветами;)
		void	CheckEnabledDiplButtons();							// disable ненужных кнопок дипл. действий

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
		int		GetContractsById(int type, int playerId, int* Id);	// возвр.кол-во n дог-в,и массив id стран,с кот.закл.контр.(Id[n])
		int		GetContTypesById(int playerId, int secondId, int* types); // возвр.кол-во n разных типов дог-в(types[n])
		int		GetIdByContType (int type, int* players);			// возвр.кол-во n id (PlayerId[n])
		int		GetContractCost(int type);							// возвр.стоим.контракта

		int		GetConCost();										// возвр. CDiplomacyData::m_iConCost

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
// Инициализирует сценарий для миссии.
class	CTYPENUM : public BaseClass
{
public:
	CTYPENUM();
	~CTYPENUM();
public:
	_str	TypeName_sID;		//Строковой идентификатор типа отряда.
	int		TypeIndex_ID;		//Текущий индекс типа отряда.
	int		nSquarnd;			//Кол-во отрядов/юнитов.

	LinearArray<int,_int>	Kills;	//grey //кол-во kills у отрядов

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

	CArmyLimit*				pArmyData;				// Данные на основе которых мы формируем отряды.

	int						Sector_ID;				// Индекс сектора.
	int						AttackDirection_ID;		// Индекс напрвления удара по сектору. (E,N-E,N,N-W,W,S-W,S,S-E)

	int						AttackerNation_ID;		// Индекс атакующей нации.
	int						AttackerRank_ID;		// Ранг атакующего.
	ClassArray<CTYPENUM>	AttackerForce;			// Отряды атакующей стороны.

	int						DefenderNation_ID;		// Индекс обороняющейся нации.
	int						DefenderRank_ID;		// Ранг атакующего.
	ClassArray<CTYPENUM>	DefenderForce;			// Отряды обороняющейся стороны.

	int						PlayerStat;				// Кем будет играть человек - атакующим(=0) или защитником(=1).
	bool					HeroInSect;				// Присутствует вражеский герой в мисси.
	
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
	int						Sector_ID;				// Индекс сектора.
	int						DifficultyLevel;		// Уровень сложности.
	_str					AttackDirection_ID;		// Индекс напрвления удара по сектору. (E,N-E,N,N-W,W,S-W,S,S-E)

	int						AttackerNation_ID;		// Индекс атакующей нации.
	_str					AttackerRank_ID;		// Ранг атакующего.
	ClassArray<CTYPENUM>	AttackerForce;			// Отряды атакующей стороны.
	CResource_BM			AttackerResource;		// Resource	

	int						DefenderNation_ID;		// Индекс обороняющейся нации.
	_str					DefenderRank_ID;		// Ранг атакующего.
	ClassArray<CTYPENUM>	DefenderForce;			// Отряды обороняющейся стороны.
	CResource_BM			DefenderResource;		// Resource	

	int						PlayerStat;				// Кем будет играть человек - атакующим(=0) или защитником(=1).
	bool					HeroPresent;			// Присутствует вражеский герой в мисси.

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

//хранит данные о map-брифинге
class	CBriefing : public CStatusDRV, public CError 	
	{
	public:
		CBriefing(DialogsSystem* pMenu, int offset); 
	public:
		DialogsDesk*	BriefDesk;				//temp
		DialogsDesk*	ObjDesk;				//temp
	private:
		DialogsSystem*	m_pMenu;				//Само меню.
		GPPicture*		SecMap;					//preview карты сектора
		//DialogsDesk*	BriefDesk;				//тактич. хар-ки по сектору	
		//DialogsDesk*	ObjDesk;				//доп. инф. & objectives по сектору - задачи+квесты
		TextViewer*		SecDesc;				//textview
		TextViewer*		SecDesc2;				//textview2

		int				m_iOffset;

		bool			m_Visible;				//видимость 
		bool			m_Enabled;				//доступность 

		bool	m_Visited; // просмотрен или нет

	public:
		void	Init();										//создание эл-тов

		void	SetVisible(bool	state);						//Вкл-выкл видимость 
		void	SetEnabled(bool	state);						//Вкл-выкл доступность 

		bool	Refresh(bool BriefingVisible, int secId);	//обновить 

		bool SetVisit();
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//--------------------------- CHeroPersDesk ----------------------------------------
//----------------------------------------------------------------------------------

//хранит данные о DialogsDesk для CPERSONAL 
class	CHeroPersDesk   	
	{
	public:
		CHeroPersDesk( DialogsSystem* pMenu , CHERO_PORTRAIT* pPortrait , CHERO_MINI_MENU* pHeroMiniMenu ); 
		~CHeroPersDesk(); 
	public:
		DialogsSystem*		m_pMenu;
		CHERO_PORTRAIT*		m_pHPortrait;
		CHERO_MINI_MENU*	m_pHMiniMenu;					//

		DialogsDesk*		m_pDesk;						// панель с героями

		bool				m_Visible;						// видимость 
		bool				m_Enabled;						// доступность 
		bool				m_Visited;						//  

		TextButton*			m_pCounName;					// назв.страны
		GPPicture*			m_pFlag;						// флaг страны

		// массивы для перc.инфы по героям
 		GPPicture*			pgppMenuBack[MAX_PLAYERS];		// BackGround for HERO_MINI_MENU [MAX_PLAYERS*MAX_HEROES]
 		GPPicture*			pgppPortrait[MAX_PLAYERS];		// BackGround for HERO_MINI_MENU

		TextButton*			ptbName[MAX_PLAYERS];			// Имя игрока.
		TextButton*			ptbRank[MAX_PLAYERS];			// Звание игрока.
		TextButton*			ptbExpa[MAX_PLAYERS];			// expa
		TextButton*			ptbSQNum[MAX_PLAYERS];			// SQ-num
		
		TextButton*			ptbVictN[MAX_PLAYERS];			// SQ-num

		GPPicture*			ptbStat[MAX_PLAYERS];
		GPPicture*			ptbFlag[MAX_PLAYERS];
		int					m_dxSF;

		DialogsDesk*		pgppExpBar[MAX_PLAYERS] ;		// expa-BAR-DESK!!!
		GPPicture*			pgppExpBack[MAX_PLAYERS];		// BackGround for expa
		GPPicture*			pgppExpBarPIC[MAX_PLAYERS] ;	// expa-BAR

		int					X0, Y0;							//Начальная позиция интерфейса.
		int					m_iCurPlayer;					// тек.игрок

		//new
		int					m_iCurPortrait, m_iPortraitNum;

	public:
		void	Init();
		void	CreateElements();

		void	SetVisible( bool state, int n );			//Вкл-выкл видимость 
		void	SetEnabled( bool state, int n );			//Вкл-выкл видимость 
		//void	SetVisible(bool	state);	//void	SetEnabled(bool	state);

		void	SetCoords( int num );						// установить новые координаты
		bool	Show( CPlayer* pPlayer[MAX_PLAYERS] , CHero* pHero , int num , int heroId , bool PersVisible );		
		void	Refresh( CHero* pHero , int num  );

		void	ChangeActive( int pHero ); //void	ChangeActive( CHero* pHero ); 
	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//---------------------------- CResPanel_BM ----------------------------------------
//----------------------------------------------------------------------------------

//хранит данные о ресурсной строке в  меню.
class	CResPanel_BM
	{
	public:
		CResPanel_BM( DialogsSystem* pMenu ); 
	public:
		DialogsSystem*	m_pMenu;					// cамо меню.
		
		GPPicture*		pgppResPic[ 7 ] ;			//
		TextButton*		pgppResTxt[ 7 ] ;			//

		int				m_iOffset;

		bool			m_Visible;					// видимость 
		bool			m_Enabled;					// доступность 

	public:
		void	Init();	
		void	CreateElements();										// создание эл-тов

		bool	SetEnabled( bool state );
		bool	SetVisible( bool state );

		bool	Refresh( CPlayer* pPlayer[MAX_PLAYERS], int Nation );	//обновить 

	};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//---------------------------- CMarketDesk -----------------------------------------
//----------------------------------------------------------------------------------

// хранит данные об интерфейсе рынка
class	CMarketDesk   	
	{
	public:
		CMarketDesk( DialogsSystem* pMenu , int MX0 , int MY0 , int MX1, int MY1 ); 
		~CMarketDesk(); 
	public:
		DialogsSystem*		m_pMenu;
		
		DialogsDesk*		m_pDesk;				// панель с рынком
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

		int					X0, Y0;					// начальная позиция(x0, y0) 
		int					X1, Y1;					// позиция (x1, y1)
		bool				m_Visible;				// видимость 
		bool				m_Enabled;				// доступность 
		bool				m_Visited;				// статус посещения: true/false

	public:
		void	Init();
		void	CreateElements();							// созд.эл-тов

		void	SetVisible( bool state );					// вкл-выкл видимость 
		void	SetEnabled( bool state );					// вкл-выкл видимость 
		
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

//хранит данные о help.
class	CBigMapHelp 
	{
	public:
		CBigMapHelp(DialogsSystem* pMenu); 
		~CBigMapHelp(); 
	public:
		DialogsSystem*	m_pMenu;									// само меню

		DialogsDesk*	m_pDesk;									// панель с help

		GPPicture*		pgppBlackScreen;							// 
		GPPicture*		pgppHelpWindow;								// 

		TextButton*		ptbHelpTitle;								//
		DialogsDesk*	m_pTextDesk;								// панель с текстом
		TextButton*		ptbHelpText;								//

		GP_TextButton*	pgptbClose;									//

		bool			m_Visible;									// видимость 
		bool			m_Enabled;									// доступность 

		static const int   m_NPages = 5 ;							// kол-во закладок (text for F1)
		static const int   m_NMess  = 4 ;							// kол-во сообщений (help-сообщения)
		static const int   m_VisNum = m_NPages + m_NMess ;			// общее kол-во сообщений
	
		bool			m_Visited[ m_VisNum ];						// просмотрен или нет

	public:
		void	Init();												// init
		void	CreateElements();									// создание эл-тов

		bool	SetVisible(bool	state);								// вкл-выкл видимость 
		bool	SetEnabled(bool	state);								// вкл-выкл доступность

		bool	CheckKey();											// 	

		bool	Show( int page , bool visible );						//
		bool	Refresh( int page );								// обновить 
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

















