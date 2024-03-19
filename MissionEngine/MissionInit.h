

//----------------------------------------------------------------------------------
//--------------------------- MissionInit. h ---------------------------------------
//----------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    SXYF    ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
struct SXYF : public BaseClass {
	SXYF(){}
	SXYF( int _X, int _Y, int _F ) : X(_X), Y(_Y), F(_F) {}
	int X;
	int Y;
	int	F;
	SXYF operator=(const SXYF& xyf) { X=xyf.X; Y=xyf.Y; F=xyf.F; return *this; }
	void	set( int _X, int _Y, int _F ) { X=_X; Y=_Y; F=_F; }

	// comment by Grey [18/02/2004]
	//SAVE( SXYF );
	//	REG_MEMBER( _int, X );
	//	REG_MEMBER( _int, Y );
	//	REG_MEMBER( _int, F );
	//ENDSAVE;
};

//----------------------------------------------------------------------------------
void	deleteSXYFarr( ClassArray<SXYF>* pXYF );
void	deleteCTYPENUMarr( ClassArray<CTYPENUM>* pTN );
SXYF	getArmyStatrtPos(int sect_id, _str& dir_id);
void	getDefenderPos( ClassArray<SXYF>* pPrm, ClassArray<SXYF>* pCnt, ClassArray<SXYF>* pRsv, _str& dir_id );
void	getAttackerPos( ClassArray<SXYF>* pPrm, ClassArray<SXYF>* pCnt, ClassArray<SXYF>* pRsv, _str& dir_id );
void	prepareAI_ZONE(_str& dir_id,LinearArray<int,_int>* pZID,LinearArray<int,_int>* pZALL);

void	SetMissionStartPoint( CSAVE_MIS_STRUCT* pIMD );//grey
bool	SetEnemyPosPoint( CSAVE_MIS_STRUCT* pIMD );//grey
void	getArmyStartPos( CSAVE_MIS_STRUCT* pIMD , ClassArray<SXYF>* pPrm );//grey
//----------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CTYPENUM    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// Инициализирует сценарий для миссии.
class CTYPENUM : public BaseClass
{
public:
	CTYPENUM();
	CTYPENUM(char* strTN,int TI,int NS);
	~CTYPENUM();
public:
	_str	TypeName_sID;				// Строковой идентификатор типа отряда.
	int		TypeIndex_ID;				// Текущий индекс типа отряда.
	int		nSquarnd;					// Кол-во отрядов/юнитов.

	LinearArray<int,_int>	Kills;		// кол-во kills у отрядов

	_str	getTypeName();
	int		getTypeIndx();
	int		getNSquard ();

	CTYPENUM	operator=(const CTYPENUM& teSQ);

	SAVE( CTYPENUM );
		REG_AUTO	(		TypeName_sID );
		REG_MEMBER	( _int, TypeIndex_ID );
		REG_MEMBER	( _int, nSquarnd	 );
		REG_AUTO	(       Kills		 );				
	ENDSAVE
};
//----------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CResource_BM    /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ресурсы в миссии (и для передачи в глобальную карту "Total War" )
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
	int		recruit;					// Рекруты для призыва в армию.

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

	int		GetResource(int res);
	int		SetResource0(int res, int amount);
	int		SetResource(int res, int amount);
	int		AddResource(int res, int amount);

};
//----------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    CSAVE_MIS_STRUCT    /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class CSAVE_MIS_STRUCT : public BaseClass
{
public:
	CSAVE_MIS_STRUCT();
	~CSAVE_MIS_STRUCT();
public:
	// CMISSIONINIT
	int						Sector_ID;			//Индекс сектора.
	int						DifficultyLevel;	//Уровень сложности.
	_str					AttackDirection_ID;	//Индекс напрвления удара по сектору. (E,N-E,N,N-W,W,S-W,S,S-E)

	int						AttackerNation_ID;	//Индекс атакующей нации.
	_str					AttackerRank_ID;	//Ранг атакующего.
	ClassArray<CTYPENUM>	AttackerForce;		//Отряды атакующей стороны.
	CResource_BM			AttackerResource;	// Resource	

	int						DefenderNation_ID;	//Индекс обороняющейся нации.
	_str					DefenderRank_ID;	//Ранг атакующего.
	ClassArray<CTYPENUM>	DefenderForce;		//Отряды обороняющейся стороны.
	CResource_BM			DefenderResource;	// Resource	

	int						PlayerStat;			//Кем будет играть человек - атакующим(=0) или защитником(=1).
	bool					HeroPresent;		//Присутствует вражеский герой в мисси.

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
	void	SetResources() { return; };

	//For Create Mission Only.
	int		getSector_ID();
	char*	getDirection_sID();
	int		getPlayerStat();					//Игрок атакует ==0, обороняется ==1.
    
	int		getAttackerNat();
	_str	getAttackerRank();
	int		getAttackerNdif();					//Кол-во разных типов отрядов.
	int		getAttackerN_SQ();					//Кол-во отрядов.
	_str	getAttackerType_sID(int n);			//Строковый идентификатор типа.
	int		getAttackerType_iID(int n);			//Индекс идентификатора типа.
	int		getAttackerType_N(int n);			//Кол-во отрядов данного типа.

	int		getDefenderNat();
	_str	getDefenderRank();
	int		getDefenderNdif();					//Кол-во разных типов отрядов.
	int		getDefenderN_SQ();					//Кол-во отрядов.
	_str	getDefenderType_sID(int n);			//Строковый идентификатор типа.
	int		getDefenderType_iID(int n);			//Индекс идентификатора типа.
	int		getDefenderType_N(int n);			//Кол-во отрядов данного типа.
	//For Create Mission Only.

};

//----------------------------------------------------------------------------------

//class CSAVE_MIS_STRUCT : public BaseClass
//	{
//	public:
//		CSAVE_MIS_STRUCT();
//		~CSAVE_MIS_STRUCT();
//	public:
//		// CMISSIONINIT
//		int						Sector_ID;			//Индекс сектора.
//		int						DifficultyLevel;	//Уровень сложности.
//		_str					AttackDirection_ID;	//Индекс напрвления удара по сектору. (E,N-E,N,N-W,W,S-W,S,S-E)
//		int						AttackerNation_ID;	//Индекс атакующей нации.
//		_str					AttackerRank_ID;	//Ранг атакующего.
//		ClassArray<CTYPENUM>	AttackerForce;		//Отряды атакующей стороны.
//		int						DefenderNation_ID;	//Индекс обороняющейся нации.
//		_str					DefenderRank_ID;	//Ранг атакующего.
//		ClassArray<CTYPENUM>	DefenderForce;		//Отряды обороняющейся стороны.
//		int						PlayerStat;			//Кем будет играть человек - атакующим(=0) или защитником(=1).
//		bool					HeroPresent;		//Присутствует вражеский герой в мисси.
//
//
//		SAVE( CSAVE_MIS_STRUCT );
//			REG_CLASS	( CTYPENUM );
//			REG_MEMBER	( _int, Sector_ID			);
//			REG_MEMBER  ( _int, DifficultyLevel		);
//			REG_AUTO	( AttackDirection_ID		);  
//			REG_MEMBER	( _int, AttackerNation_ID	);
//			REG_AUTO	( AttackerRank_ID			);
//			REG_AUTO	( AttackerForce				);
//			REG_MEMBER	( _int, DefenderNation_ID	);
//			REG_AUTO	( DefenderRank_ID			);
//			REG_AUTO	( DefenderForce				);
//			REG_MEMBER	( _int,	PlayerStat			);
//			REG_MEMBER	( _bool, HeroPresent		);
//		ENDSAVE
//	public:
//		void	Init();
//		void	DeleteArrays();
//
//		//For Create Mission Only.
//		int		getSector_ID();
//		char*	getDirection_sID();
//		int		getPlayerStat();			//Игрок атакует ==0, обороняется ==1.
//        
//		int		getAttackerNat();
//		_str	getAttackerRank();
//		int		getAttackerNdif();					//Кол-во разных типов отрядов.
//		int		getAttackerN_SQ();					//Кол-во отрядов.
//		_str	getAttackerType_sID(int n);			//Строковый идентификатор типа.
//		int		getAttackerType_iID(int n);			//Индекс идентификатора типа.
//		int		getAttackerType_N(int n);			//Кол-во отрядов данного типа.
//
//		int		getDefenderNat();
//		_str	getDefenderRank();
//		int		getDefenderNdif();					//Кол-во разных типов отрядов.
//		int		getDefenderN_SQ();					//Кол-во отрядов.
//		_str	getDefenderType_sID(int n);			//Строковый идентификатор типа.
//		int		getDefenderType_iID(int n);			//Индекс идентификатора типа.
//		int		getDefenderType_N(int n);			//Кол-во отрядов данного типа.
//	};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------













