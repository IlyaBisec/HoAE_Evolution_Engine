
#define	QZ_PulseTime	2000 //17000		// продолжительность подсветки квестов (миллисекунды) //15000 //30000

#define	_STANDARD_			0		// обычный квест (стандартный)
#define	_TRAP_SQUADS_		1		// засада
#define	_CHEST_SQUAD_		2		// отряд с полковой казной
#define	_CHEST_CLOSED_		3		// полковая казна
#define	_STZONE_GUARD_		4		// охрана страт.зон

//-------------------------------------------------------------------------
class	UNIT_LV : public BaseClass
{
public:
	UNIT_LV()		{};
	UNIT_LV(int ID,int SR)	{ Index=ID; Serial=SR; };
	~UNIT_LV()		{};
public:
	int		Index;
	int		Serial;
public:
	void	create(GAMEOBJ* pG);
	bool	live();
};
//-------------------------------------------------------------------------
class  SQUARD_LV : public BaseClass
{
public:
	SQUARD_LV()   { Init(); };
	SQUARD_LV(int NI,int ID);
	~SQUARD_LV()  { Init(); };
public:
	int		Index;
	int		Serial;

	ClassArray<UNIT_LV>	Units;
public:
	void	Create(int NI,int ID);
	void	Init()   { Index=Serial=0 ; Units.Clear() ; };
};
//-------------------------------------------------------------------------
class	CKillEnemy00SPROC : public BaseClass
{
public:
	CKillEnemy00SPROC()   { Init(); }
	CKillEnemy00SPROC(CKillEnemy00* pKE00,int dif);
	~CKillEnemy00SPROC();
public:
	CDIFFICULTY*			pDIFF_DATA;		//Данные из скрипта.
	byte					NI;				//Нация за которую генерим квест.
	bool					QuestGived;		//Квест дан игроку.
	bool					QuestComplite;	//Квест окончен.
	int						DifficultyLevel;//Уровень сложности.
	ClassArray<SQUARD_LV>	SQUARDS;		//Отряды.
	ClassArray<UNIT_LV>		UNITS;			//Одинокие юниты.

	int						Expa;
	bool					Pulse;
	int						pulseTime;

	// traps states
	int						STATE;
	bool					SetTrap;
	bool					PutFuziler;

	void	Init();
	void	CreateQuest( int ai_ni, int& tSqLimit );
	void	PROCESS( int NATION, int indx, char* pName );

	int		GetExperience();
	int		GetEnabledSQUARDS();

	bool	CheckQuestActivity();
};
//-------------------------------------------------------------------------