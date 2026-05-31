#ifndef	__BE__HASH__TOP__ 
#define	__BE__HASH__TOP__ 

class pGroup;

extern	word FULLWAY[128];
extern  int NWPTS;

bool	g_beNeighbor(int MFIELD,int Area0, int Area1);

class DIALOGS_API be_CAreaPowerData : public BaseClass
{
public:
	be_CAreaPowerData();

	int		AreaID;
	// Buildings
	int		BuildingAttackPOWER;	// Building attack Power 
	int		BuildingDefencePOWER;	// Building protection Power  
	// Units
	int		MeleeAttackPOWER;		// Units attack Power
	int		RangeAttackPOWER;		// Units attack Power
	int		UnitsDefencePOWER;		// Units protection Power 
	// Numbers
	int			MeleeUnitsN;
	int			RangeUnitsN;
	int			BuildingsN;

	void	resetData();

	Delimiter	delimeter00;

	SAVE(be_CAreaPowerData);
		REG_MEMBER(_int,BuildingAttackPOWER);
		REG_MEMBER(_int,BuildingDefencePOWER);
		REG_AUTO(delimeter00);
		REG_MEMBER(_int,MeleeAttackPOWER);
		REG_MEMBER(_int,RangeAttackPOWER);
		REG_MEMBER(_int,UnitsDefencePOWER);
	ENDSAVE;
};

class DIALOGS_API be_CAreaListPowerData : public be_CAreaPowerData
{
public:
	be_CAreaListPowerData() : be_CAreaPowerData() {};
	be_CAreaListPowerData(int layer,int nat);
	LinearArray<int,_int>	AreasList;

	int		m_layer;
	int		m_nation;

	// for use in lua, only GET() functions
	int		getAreaNumber()				const;		// get N of areas in areas list
	int		getArea(int localID)		const;		// get area id from list
	int		getMeleeAttackPOWER()		const;
	int		getMeleeUnitsN()			const;
	int		getRangeAttackPOWER()		const;
	int		getRangeUnitsN()			const;
	int		getUnitsDefencePOWER()		const;
	int		getBuildingAttackPOWER()	const;
	int		getBuildingsN()				const;
	int		getBuildingDefencePOWER()	const;
	// ------------------------------------

	int		getWeakestAST_AID()			const;

	bool	create( be_CGROUP* pGRP );
	bool	create( pGroup& pGRP );

	SAVE(be_CAreaListPowerData);
		REG_BASE(be_CAreaPowerData);
		REG_AUTO(AreasList);
		REG_AUTO(delimeter00);
		REG_MEMBER(_int,MeleeAttackPOWER);
		REG_MEMBER(_int,MeleeUnitsN);
		REG_MEMBER(_int,RangeAttackPOWER);
		REG_MEMBER(_int,RangeUnitsN);
		REG_MEMBER(_int,UnitsDefencePOWER);
		REG_AUTO(delimeter00);
		REG_MEMBER(_int,BuildingAttackPOWER);
		REG_MEMBER(_int,BuildingsN);
		REG_MEMBER(_int,BuildingDefencePOWER);
	ENDSAVE;
};

class DIALOGS_API be_CTotalALPD : public ClassArray<be_CAreaListPowerData>
{
public:
	be_CTotalALPD();
	be_CTotalALPD(int layer,int nation,int power);

	int						getN() const;
	be_CAreaListPowerData*	getALPD(int i);
	bool					create(int layer,int nation,int power);

	SAVE( be_CTotalALPD );
		REG_PARENT( ClassArray<be_CAreaListPowerData> );
	ENDSAVE;
};

class be_HashTopTable
{
	friend bool	g_beGET_WAY(be_HashTopTable& HTT,int sAreaID,int dAreaID,LinearArray<int,_int>& AreasList);
	friend void	g_beDRAW_WAY(be_HashTopTable& HTT,int sAreaID,int dAreaID);
	friend void	g_beDRAW_HashTopTable(be_HashTopTable& pHTT);
	friend bool	g_beCheckForShowTopArea(be_HashTopTable& pHTT,int iAreaID);

	friend word	g_beSCOUT_AI(int AID);
	friend void	g_beSCOUT_UPDATE_HTT();
	friend bool	g_beFillFULLWAY_SCOUT(int x,int y);
	friend bool	g_beFillFULLWAY_ATTACK(int start,int finish,int myNation,int& MaxDanger,int DangSteps,int layer);

	friend class be_CAreaListPowerData;

public:
	be_HashTopTable();
	be_HashTopTable( int l_iNat, int l_iMFIELD );
	be_HashTopTable( int l_iNAreas, int l_iNat, int l_iMFIELD );
	~be_HashTopTable();

	void		Initialize( int l_iNat, int l_iMFIELD );

	bool		Create();
	void		Clear();
	bool		Init();
	void		Update();
	void		Update(int delta);
	void		SafeUpdate(int delta);

	// Exploring
	void		EXP_Create();
	void		EXP_Clear();
	void		EXP_Init(int l_iAreaID);		// set only -1, 0 or 100
	void		EXP_Update(int l_iAreaID);
	void		EXP_ReduceRate(int l_iRR);		// safe reduce exploring rate by l_iRR

	// Assist
	void		AST_Create();
	void		AST_Clear();
	void		AST_Init(int l_iAreaID);
	void		AST_Update();
	void		AST_ADD_Update();
	be_CAreaPowerData*	getAST_PD(int l_iAreaID);
	void		AST_ClearPD();

	bool		AST_getConectedList(int power,ClonesArray< LinearArray<int,_int> >& refAreasArray);
	bool		AST_createPCL(be_CTotalALPD* pDATA,int power=1);
	void		AST_showPCL(int power=1);
	
	// Visibility
	void		VSB_Create();
	void		VSB_Clear();
	void		VSB_Init(int l_iAreaID);
	void		VSB_UpdateAround(int l_iAreaID);
	bool		VSB_UpdateTRIADA();

private:
	bool		m_iCreate;
	bool		m_iInit;

	int			m_iNation;
	int			m_iMFIELD;
	int			m_iNAreas;
	short*		m_piExploringRate;	// —тепень изучености зоны ( -1   - мертва€ зона (невозможно туда попасть)
									//							  0   - никто ниразу небыл, 
									//							  1   - кто-то там уже был, очень давно,
									//							  100 - присутствуют юниты					)
	int*		m_piAssist;			//  ол-во юнитов данной нации в Area
	int*		m_piAssistAdd;		// ƒобавить 10 в зонах видимости и 5 в прилежащих
	ClonesArray< be_CAreaPowerData >	m_piAssistPower;
    bool*		m_piVisible;		// ¬идит ли данна€ наци€ данную Area

	DWORD		lastTick;
};

bool	g_beCheckForShowTopArea(be_HashTopTable& pHTT,int iAreaID);
bool	g_beGET_WAY(be_HashTopTable& HTT,int sAreaID,int dAreaID,LinearArray<int,_int>& AreasList);
void	g_beDRAW_WAY(be_HashTopTable& HTT,int sAreaID,int dAreaID);
void	g_beDRAW_STORED_WAY(int lockType);	// Draw way stored in FULLWAY areas array
void	g_beDRAW_HashTopTable(be_HashTopTable& pHTT);

//////////////////////////////////////////////////////////////////////////
// Class for store and manage HT for all nations
//////////////////////////////////////////////////////////////////////////
#define	HTT_LAND	0
#define	HTT_WATER	1
#define	HTT_LOWAIR	2
#define	HTT_HIGHAIR	3
struct be_NationHTT
{
	be_NationHTT()	{};
	~be_NationHTT()	{};

	void				Initialize(int _Nation);
	void				SafeUpdate(int delta);

	int m_Nation;
	// LAND
	be_HashTopTable		LAND_HTT;
	// WATER
	be_HashTopTable		WATER_HTT;
	// LOW  AIR
	be_HashTopTable		LOWAIR_HTT;
	// HIGH AIR
	be_HashTopTable		HIGHAIR_HTT;

	be_HashTopTable&	operator[](int _idx);
};

extern int   NtNUnits[8];
class be_HashTopTableList
{
public:
	be_HashTopTableList();		// Create and init all HTT for all nations
	~be_HashTopTableList();		// Free memory

	void	SafeUpdate(int delta);

	be_NationHTT&	operator[](int _idx){ return HTT[_idx]; };
protected:
private:
	be_NationHTT		HTT[8];
};
extern	be_HashTopTableList		g_beHTT_LIST;	// Hash Top Table for all nation
//////////////////////////////////////////////////////////////////////////
// searching functions 
//////////////////////////////////////////////////////////////////////////
word	g_beSCOUT_AI(int AID);
void	g_beSCOUT_UPDATE_HTT();
bool	g_beFillFULLWAY_SCOUT(int x,int y);

bool	g_beFillFULLWAY_ATTACK(int start,int finish,int myNation,int& MaxDanger,int DangSteps,int layer=HTT_LAND);

//////////////////////////////////////////////////////////////////////////
void	be_REG_CLASS_HashTop();
//////////////////////////////////////////////////////////////////////////


#endif//__BE__HASH__TOP__ 