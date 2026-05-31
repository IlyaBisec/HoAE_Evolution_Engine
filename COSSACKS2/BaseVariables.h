#ifndef	__BASE__VARIABLES__ 
#define	__BASE__VARIABLES__

bool	g_beCheckChar(const char* lpszSTR);

class	be_CVariableBase;
class	be_CBOOL;
class	be_CDWORD;
class	be_CINT;
class	be_CFLOAT;
class	be_CSTR;
class	be_CGROUP;
class	be_CNODE;
class	be_CUnitType;
class	be_CFurmationType;
class	be_CARRAY;
class	be_CSMARTOBJECT;

// Flag for quick enum creation
struct be_OBJECT_SVITCHER {

	~be_OBJECT_SVITCHER()	{ ClassNameList.Clear(); };

	ClassArray<_str>	ClassNameList;
	
	int		FindState	(const char* _ClassName){
		int N = ClassNameList.GetAmount();
		int StID = -1;
		while (N--&&StID==-1) {
			if ( strcmp(_ClassName,(ClassNameList[N])->pchar())==0 ) StID=N;
		};
		return StID;
	};
	void	SetState	(const char* _ClassName){
		int StID = FindState(_ClassName);
		if (StID==-1) {
			StID = FindState("");
			_str* pNewSate = NULL;
			if (StID==-1) {
				pNewSate = new _str();
				*pNewSate = _ClassName;
				ClassNameList.Add(pNewSate);
				pNewSate = NULL;
			}else{
				pNewSate = ClassNameList[StID];
				*pNewSate = _ClassName;
			};
		};
	};
	bool	GetState	(const char* _ClassName){
		return ( FindState(_ClassName) != -1 );
	};
	void	DropState	(const char* _ClassName){
		int StID = FindState(_ClassName);
		if (StID!=-1) {
			_str* pNewSate = ClassNameList[StID];
			*pNewSate = "";
		};
	};
};
extern	be_OBJECT_SVITCHER	g_beOBJ_CONTROLER;

#include "VariableBase.h"

template<class T>
class be_CBaseVariable : public be_CVariableBase
{
public:
	T		Value;
public:
	virtual	bool	GetValueAsSTR(_str& ValueSTR)		{ ValueSTR = Value;	return true; };
public:
	virtual	void	operator=(const bool	&_Value)	{ Value = (T)(_Value); };
	virtual	void	operator=(const int		&_Value)	{ Value = (T)(_Value); };
	virtual	void	operator=(const DWORD	&_Value)	{ Value = (T)(_Value); };
	virtual	void	operator=(const float	&_Value)	{ Value = (T)(_Value); };
	virtual	void	operator=(const double	&_Value)	{ Value = (T)(_Value); };

	virtual			operator bool  ()	const			{ return  (T)(Value);  };
	virtual			operator int   ()	const			{ return  (T)(Value);  };
	virtual			operator DWORD ()	const			{ return  (T)(Value);  };
	virtual			operator float ()	const			{ return  (T)(Value);  };
};

// BASE SIMPLE TYPES /////////////////////////////////////////////////////
class be_CBOOL : public be_CBaseVariable<bool>
{
public:
	be_CBOOL();
	be_CBOOL(const bool _Value);
	be_CBOOL(const char* _Name);
	be_CBOOL(const char* _Name,const bool _Value);
	~be_CBOOL();
public:
	virtual	void	SetValue(const bool val)			{ Value = val;  };
	virtual	bool	GetValue() const					{ return Value; };
public:
	virtual	void	operator=(const bool	&_Value)	{ be_CBaseVariable<bool>::operator=(_Value); };
	virtual	void	operator=(const int		&_Value)	{ be_CBaseVariable<bool>::operator=(_Value); };
	virtual	void	operator=(const DWORD	&_Value)	{ be_CBaseVariable<bool>::operator=(_Value); };
	virtual	void	operator=(const float	&_Value)	{ be_CBaseVariable<bool>::operator=(_Value); };
	virtual	void	operator=(const double	&_Value)	{ be_CBaseVariable<bool>::operator=(_Value); };

	virtual			operator bool  ()	const			{ return  be_CBaseVariable<bool>::operator bool	(); };
	virtual			operator int   ()	const			{ return  be_CBaseVariable<bool>::operator int	(); };
	virtual			operator DWORD ()	const			{ return  be_CBaseVariable<bool>::operator DWORD(); };
	virtual			operator float ()	const			{ return  be_CBaseVariable<bool>::operator float(); };
public:
	SAVE(be_CBOOL);
		REG_PARENT(be_CVariableBase);
		REG_MEMBER(_bool,Value);
	ENDSAVE;
};

class be_CDWORD : public be_CBaseVariable<DWORD> 
{
public:
	be_CDWORD();
	be_CDWORD(const DWORD _Value);
	be_CDWORD(const char* _Name);
	be_CDWORD(const char* _Name,const DWORD _Value);
	~be_CDWORD();
public:
	virtual	void	SetValue(const DWORD val)			{ Value = val;  };
	virtual	DWORD	GetValue() const					{ return Value; };
public:
	virtual	void	operator=(const bool	&_Value)	{ be_CBaseVariable<DWORD>::operator=(_Value); };
	virtual	void	operator=(const int		&_Value)	{ be_CBaseVariable<DWORD>::operator=(_Value); };
	virtual	void	operator=(const DWORD	&_Value)	{ be_CBaseVariable<DWORD>::operator=(_Value); };
	virtual	void	operator=(const float	&_Value)	{ be_CBaseVariable<DWORD>::operator=(_Value); };
	virtual	void	operator=(const double	&_Value)	{ be_CBaseVariable<DWORD>::operator=(_Value); };

	virtual			operator bool  ()	const			{ return  be_CBaseVariable<DWORD>::operator bool (); };
	virtual			operator int   ()	const			{ return  be_CBaseVariable<DWORD>::operator int  (); };
	virtual			operator DWORD ()	const			{ return  be_CBaseVariable<DWORD>::operator DWORD(); };
	virtual			operator float ()	const			{ return  be_CBaseVariable<DWORD>::operator float(); };
public:
	SAVE(be_CDWORD);
		REG_PARENT(be_CVariableBase);
		REG_MEMBER(_DWORD,Value);
	ENDSAVE;
};

class be_CINT : public be_CBaseVariable<int>
{
public:
	be_CINT();
	be_CINT(const int _Value);
	be_CINT(const char* _Name);
	be_CINT(const char* _Name,const int _Value);
	~be_CINT();
public:
	virtual	void	SetValue(const int val)				{ Value = val;  };
	virtual	int		GetValue() const					{ return Value; };
public:
	virtual	void	operator=(const bool	&_Value)	{ be_CBaseVariable<int>::operator=(_Value); };
	virtual	void	operator=(const int		&_Value)	{ be_CBaseVariable<int>::operator=(_Value); };
	virtual	void	operator=(const DWORD	&_Value)	{ be_CBaseVariable<int>::operator=(_Value); };
	virtual	void	operator=(const float	&_Value)	{ be_CBaseVariable<int>::operator=(_Value); };
	virtual	void	operator=(const double	&_Value)	{ be_CBaseVariable<int>::operator=(_Value); };

	virtual			operator bool  ()	const			{ return  be_CBaseVariable<int>::operator bool	(); };
	virtual			operator int   ()	const			{ return  be_CBaseVariable<int>::operator int	(); };
	virtual			operator DWORD ()	const			{ return  be_CBaseVariable<int>::operator DWORD	(); };
	virtual			operator float ()	const			{ return  be_CBaseVariable<int>::operator float	(); };
public:
	SAVE(be_CINT);
		REG_PARENT(be_CVariableBase);
		REG_MEMBER(_int,Value);
	ENDSAVE;
};

class be_CFLOAT : public be_CBaseVariable<float>
{
public:
	be_CFLOAT();
	be_CFLOAT(const float _Value);
	be_CFLOAT(const char* _Name);
	be_CFLOAT(const char* _Name,const float _Value);
	~be_CFLOAT();
public:
	virtual	void	SetValue(const float val)			{ Value = val;  };
	virtual	float	GetValue() const					{ return Value; };
public:
	virtual	void	operator=(const bool	&_Value)	{ be_CBaseVariable<float>::operator=(_Value); };
	virtual	void	operator=(const int		&_Value)	{ be_CBaseVariable<float>::operator=(_Value); };
	virtual	void	operator=(const DWORD	&_Value)	{ be_CBaseVariable<float>::operator=(_Value); };
	virtual	void	operator=(const float	&_Value)	{ be_CBaseVariable<float>::operator=(_Value); };
	virtual	void	operator=(const double	&_Value)	{ be_CBaseVariable<float>::operator=(_Value); };

	virtual			operator bool  ()	const			{ return  be_CBaseVariable<float>::operator bool (); };
	virtual			operator int   ()	const			{ return  be_CBaseVariable<float>::operator int	 (); };
	virtual			operator DWORD ()	const			{ return  be_CBaseVariable<float>::operator DWORD(); };
	virtual			operator float ()	const			{ return  be_CBaseVariable<float>::operator float(); };
public:
	SAVE(be_CFLOAT);
		REG_PARENT(be_CVariableBase);
		REG_MEMBER(_float,Value);
	ENDSAVE;
};
class be_CSTR : public be_CVariableBase
{
public:
	be_CSTR();
	be_CSTR(const char* _Name);
	be_CSTR(const char* _Name,const char* _Value);
	~be_CSTR();
public:
	_str			Value;
	virtual	bool	GetValueAsSTR(_str& ValueSTR)		{ ValueSTR = Value;	return true; };
	virtual	void	SetValue(const char* val);
	virtual	char*	GetValue() const;
public:
	//virtual	void	operator=(const bool	&_Value)	{ be_CBaseVariable<_str>::operator=(_Value); };
	//virtual	void	operator=(const int		&_Value)	{ be_CBaseVariable<_str>::operator=(_Value); };
	//virtual	void	operator=(const DWORD	&_Value)	{ be_CBaseVariable<_str>::operator=(_Value); };
	//virtual	void	operator=(const float	&_Value)	{ be_CBaseVariable<_str>::operator=(_Value); };
	//virtual	void	operator=(const double	&_Value)	{ return; };

	//virtual			operator bool  ()	const			{ return  false;		};
	//virtual			operator int   ()	const			{ return  0;			};
	//virtual			operator DWORD ()	const			{ return  0x00000000;	};
	//virtual			operator float ()	const			{ return  0.0f;			};
public:
	SAVE(be_CSTR);
		REG_PARENT(be_CVariableBase);
		REG_AUTO(Value);
	ENDSAVE;
};

class be_CSTR_DIALOG : public be_CVariableBase
{
public:
	be_CSTR_DIALOG();
	be_CSTR_DIALOG(const char* _Name);
	be_CSTR_DIALOG(const char* _Name,const char* _Value);
	~be_CSTR_DIALOG();
public:
	_str			TextID;		// Text id in text file
	_str			Speaker;	// Speaker Name
	_str			Spirit;		// Spirit of speker
	_str			ogFile;		// Sound treck file
	_str			Text;		// Text from text file
	bool			TooLong;
	_str_ref		Group;		// Group from map for identufy speaker

	bool	update;
	void	Update();
	void	getStringForSave(_str& _string);
	bool	Test();

	// ROF PROPERTI IN LUA
	char*			Get_ogFile()	const;
	be_CGROUP*		Get_Group()		const;
public:
	SAVE(be_CSTR_DIALOG);
		REG_PARENT(be_CVariableBase);
		REG_ENUM(_strindex,Group,ALL_be_CGROUPS_ON_MAP);
		REG_AUTO(TextID);
		NOSAVE REG_ENUM(_strindex,Speaker,BE_SPEAKERS_ENUM);
		NOSAVE REG_ENUM(_strindex,Spirit,BE_SPIRIT_ENUM);
		NOSAVE REG_FILEPATH(ogFile,".ogg");
		NOSAVE REG_AUTO(Text);
		INVISIBLE NOSAVE REG_MEMBER(_bool,TooLong);
	ENDSAVE;
};
#include "Camera\Track.h"
class be_CCAMERA : public be_CVariableBase
{
public:
	CameraTrack CameraTracks;
public:
	be_CCAMERA();
	be_CCAMERA(const char* _Name);
	~be_CCAMERA();
public:
	static bool	HideCameraTracks;
	DWORD		GetClassMask();
public:
	SAVE(be_CCAMERA);
		REG_PARENT(be_CVariableBase);
		SAVE_SECTION(0x00000100);
			REG_AUTO(CameraTracks);
		SAVE_SECTION(0x00000200);
			INVISIBLE REG_AUTO(CameraTracks);
	ENDSAVE;
};
// BASE MAP TYPE /////////////////////////////////////////////////////////
// TEMP //
class vvPOINT_SET;
//////////
void	RemoveDublicateInList( LinearArray<int,_int>& _list );
enum	en_vGroup_AttackType
{
	vgNO_TYPE	 = 0x00000000,
	vgNEAREST	 = 0x00000001,	// nearest object in Top destenation	// ALL			   -> last  1 unit
	vgWEAKATT	 = 0x00000002,	// unit whit weakest attack				// ---	one of --- -> last  5 units
	vgWEAKDEF	 = 0x00000004,	// unit whit weakest defence			// ---	one of --- -> last  5 units
	vgSICKEST	 = 0x00000008,	// unit whit minimum hitpoints			// ---	one of --- -> last  5 units
	vgONLINC	 = 0x00000010,	// object what direct on me				// +++	one of +++ -> last 10 units
	vgHERO		 = 0x00000020,	// only hero							// +++	one of +++ -> last 10 units
	vgWORKER	 = 0x00000040,	// only workers							// +++	one of +++ -> last 10 units
	vgBUILDING	 = 0x00000080	// only building						// +++	one of +++ -> last 10 units
	// ...
};
#include "VB_enum.h"
class	be_CALINC : public BaseClass
{
public:
	// Attaker		// Target
	int	Aid;		int Tid;
	int	Asn;		int Tsn;
	bool	CheckLinc();

	// Personal data
	float	distPower;	// Use 10*2^(-d/50), d - in pixels
	int		prioPower;	// Use priority attack table
	void	Fill1vs1Data();

	// Array data
	int		AttackerNUM;// Number of attakers from list
	be_CALINC();
};
class	be_CLINC_LIST : public BaseClass
{
public:	
	~be_CLINC_LIST();
	ClonesArray<be_CALINC>	curLincs;
	int						curSize;
	ClonesArray<be_CALINC>	newLincs;
	int						newSize;
	void	CreateCurList( int _N );
	void	FillCurList  ( be_CGROUP* pAttaker, bool fillPower=false );
	void	UpdateCur1vs1( be_CGROUP* pAttaker, be_CGROUP* pTarget );
	void	UpdateCurNvs1();
	
	void	CreateNew( be_CGROUP* pAttaker, be_CGROUP* pTarget );
    
	// Helper
	void	ClearCur();
	void	ClearNew();
	int		GetNofCur(int _id);
};
class be_CGROUP : public be_CVariableBase
{
public:
	be_CGROUP();
	be_CGROUP(const char* name);
	~be_CGROUP();
public:
	bool						GetValueAsSTR(_str& ValueSTR);
	// Spesial operation
	virtual	en_vGroup_Order		GetORDER();						// Текуший приказ
	virtual	en_vGroup_Order		GetUnitORDER(int uid);			// Текущий приказ для юнита в локальном списке
	virtual	bool				CheckUnitORDER(int uid,en_vGroup_Order Order);	// Текущий приказ для юнита в локальном списке

	virtual	int		GetORDER_NUM(en_vGroup_Order order);
	virtual	int		NO_ORDERS_NUM	() const;							// N of unists wihtout orders
	virtual	int		MOVE_NUM		() const;
	virtual	int		ATTACK_NUM		() const;
	
	virtual	void	RemoveDeadUnits();
	virtual	void	ScreenOnGroup();
	virtual	bool	IsGroupOnScreen();
	virtual	void	vDraw(bool forceDraw=false);
	virtual	word	_CheckGID(word id);
	virtual	word	_CheckGPT(OneObject* pOB);
	virtual	OneObject*	GetOneObj_gID(word gID);				// OneObject* по глобальному индексу
	virtual	OneObject*	GetOneObj_lID(word lID);				// OneObject* по локальному  индексу
	// Adding operation
	virtual	void	UpdateOB_GID_inGROUP();						// update OB->pGRP_ID, only after load
	virtual	bool	AddUnitGID(word id);						// Добавить по глобальному ID
	virtual	bool	AddUnitGPT(OneObject* pOB);					// Добавить по глобальному указателю
	virtual	int		AddSelectedUnits(byte NI=0xFF);				// Добавить выделенных юнитов.
	virtual	void 	AddUnitsInZone(byte NI,int x,int y,int R,bool _add); // Add units nation-NI from current location to group
	virtual	void	AddUnitsTypeInZone(byte NI,int UnitType,int x,int y,int R,bool _add);
	virtual	void	AddEnemyUnitsInZone(int Nat,int x,int y,int R,bool _add);
	virtual	void	AddBrigad(int BrigID, int Nat);				// Добавить юнитов из бригады.
	virtual void	AddGObjectsInArea(int nation,int Layer,int AreaID,bool bulidings=false,bool add=false);
	virtual	void	AddUnitsFromGroup(be_CGROUP* pGR);			// add units from pGR in current group
	virtual	int		RemSelectedUnits(byte NI=0xFF);				// Добавить выделенных юнитов.
	virtual	void	RemUnitGID(word id);						// Убрать по глобальному ID
	virtual	void	RemUnitGPT(OneObject* pOB);					// Убрать по глобальному указателю
	virtual	void	RemUnitID(word id);							// Убрать по локальному ID
	virtual	void	RemAllUnits();
	virtual	int		RemUnitsByNation(int NI);
	// Remove operation
	virtual	int		RemoveNUnitsToCGroup(be_CGROUP* pGR,int N=0);		// Переместить N юнитов из группы в группу id
	virtual	int		RemoveNReadyUnitsToCGroup(be_CGROUP* pGR,int N/*=-1*/);
	virtual	int		RemoveUnitsInZoneToCGroup(be_CNODE* pND,be_CGROUP* pGR);
	virtual	int		RemoveInvisibleForNMask(int _NMask);
	// Operation on units in group
	virtual	int		KillUnits(int N=-1);						// Убить N юнитов (N==-1 - всех)
	virtual	int		EraseUnits(int N=-1);						// Убрать (исчезнут) N юнитов (N==-1 - всех)
	virtual	void	SetNation(byte NI);							// Выставить цвет
	virtual	void	SelectUnits(bool add/*=0*/);				// Выделить группу (0-перевыделить, 1-добавить к выделенной)
	virtual	void	SelectUnitsInZone(int x,int y,int R,bool add=0);		// Выделить юнитов группы в зоне (0-перевыделить, 1-добавить к выделенной)
	virtual	void	UnSelect();									// Снять выделение
	// Move
	virtual	void	SendTo(int x,int y,int dir,int type=0);		// if dir==512 move by cur direction
	virtual	void	SendToI(int x,int y,int dir,int type,int i,bool withAttack=false);// if dir==512 move by cur direction
	virtual	void	SendToDispersePosition(int x,int y);
	virtual	void	MarkTime(int _x,int _y,int _destD,int _powerRND);

	virtual	void	SendSmartlyTo(int x, int y, int stepsN,be_CNODE* pDestNode);

	virtual	void	ChangeDirection(int dir,int type=0);		// Повернуть юнитов в заданном направлении
	virtual	bool	SetAgresiveST(int stID);					// Уровень агресивности
	virtual	void	ChengeFormation(int formID);				// Поменять формацию
	virtual	void	TakeRess(int RessID);						// Собираем ресурс RessID
	virtual	void	TakeFood();									// Собираем еду
	virtual	void	TakeWood();									// Собираем дерево
	virtual	void	TakeStone();								// Собираем камень
	virtual	void	ClearOrders();								// Clear all orders for group
	virtual	void	SetInStandGround();							// Set all brigades in SG
	virtual void	SetStandGround(bool state);					// OneObject::StandGround = state
	virtual	void	SetHP(const int HP);						// Set units curent value of hit points
	virtual	void	SetLockType(int LockTypeID);				// 0-lend,1-sea,2-low fly,3-hight fly
	virtual	void	SetSearchWict(const bool _flag);			// Set SearchWict flag for units in group
	virtual	bool	SetRallyPoint(int x, int y);
	virtual	void	SetAIGuard();
	virtual	void	setAIGuard(bool _state);
	virtual	bool	getAIGuard()const;
	// For MORE ADD (DAN)
	virtual	int		GetTotalAmount2(int UnitType, int Nat=0xff);// Количество определенных юнитов в нации
	virtual void	SetBrFlagbearers();						// Назначить флагоносцев в бригаде
	virtual void	ClearBrFlagbearers();					// Убрать флагоносцев из бригады
	// Not in code
	virtual	void	SendToPosition(vvPOINT_SET* pPosArr);
	virtual	bool	ChekPosition(vvPOINT_SET* pPosArr);
			bool	CheckOB_pos(OneObject* pOB,int _x,int _y);
	// Attack
	virtual	bool	CheckAttackPossibilityG(be_CGROUP* pEnemyGroup);
	virtual	bool	CheckAttackPossibilityXYR(int _x,int _y,int _R);
	virtual	bool	CheckAttackPossibilityN(be_CNODE*  pNode);
	virtual	void	AttackGroup_NearestOB(be_CGROUP* pEnemyGroup);		// Set targets for group for attack pEnemyGroup
	virtual	void	AttackGroup_NearestOB00(be_CGROUP* pEnemyGroup);
	virtual	void	AttackEnemy00(be_CGROUP* pEnemyGroup,int pow);
	// Comand for only for units with OverOrders orders in group
	int		last_idAttackEnemy_ID;
	virtual	void	idAttackEnemy( DWORD OverOrders,bool nearest=false,int addR=0, be_CARRAY* pARR=NULL );
	virtual	void	idSendTo( int x, int y, int dir, DWORD OverOrders, int _id, int type=0,bool withAttack=false,bool donotCheckPos=false );
	// produce
	virtual	void	Produse00(int UnitType, int UnitNUM);					// Order -> Produse units uniformly
	virtual	void	Produse00(int UnitType, int UnitNUM, be_CGROUP* pGR);	// Order -> Produse units acoording to data in map options and pGR
	// detect
	virtual	bool	IsGRPAttackMe(be_CGROUP* pGRP);
	// set
	virtual	void	SetNMASK(const DWORD _mask);
	virtual	int		GetNMASK() const;
	virtual	void	DeleteItemDrops();
	virtual	void	GiveExpa(int Expa);
	virtual	void	IncreaseLevel(int LevelAmount);
	// SCOUT
	virtual	void	ExploringMAP();
	// Get some data
	virtual	int		GetHP() const;								// Return average value of units HP
	virtual	int		GetMP() const;								// Get amount of mana points
	virtual	bool	GetSearchWict()	const;						// Set SearchWict flag for units in group
    virtual	int		GetTotalAmount() const;						// Кол-во юнитов в группе
	virtual	int		GetNationAmout(int Nat);					// Кол-во юнитов данной нации в группе
	virtual	int		GetAmountInZone(int x, int y, int R);		// Кол-во юнитов группы в зоне
	virtual	void	GetGroupCenter(int& x,int& y);				// Центр масс группы
	virtual	int		GetGroupX() const;
	virtual	int		GetGroupY() const;
	virtual	int		GetXi(int i) const;
	virtual	int		GetYi(int i) const;
	virtual	int		GetGID(int i);								// Get Global unit Index use local group Index
	virtual	int		GetDIRi(int i) const;
	virtual	int		GetGIDi(int i) const;						// Get global index use local index in group
	virtual	int		GetDirection();								// Под каким углом стоят юниты (0xFFFF - стоят в разнобой)
	virtual	byte	GetNation() const;
	virtual	int		GetAgresiveState();							// 0-Normal, 1-Peacefull, 2-Aggressive.
	virtual bool	GetBrigadeList(bool checkCOM=false,LinearArray<int,_int>* pBrigList=NULL);	// List of BgigadeID in group, true if brig in group (checkCOM==true -> fill list)
	virtual	int		GetAmountOfNewUnits(bool clearList=false);	// Кол-во новых юнитов в группе (clearLit==true выставить что новых нет)
	virtual	int		GetAmountOfRemUnits(bool clearList=false);	
	virtual	bool	GetIsTired();								// true if first squad in group is tired
	virtual	bool	GetReady() const;							// true if all units on outdors
	virtual	int		GetEnemyUnitsAroundNumber(int R);
	virtual	int		GetLockType() const;						// 0-lend,1-sea,2-low fly,3-hight fly
	virtual	bool	GetStandGround();							// true if all units in StandGround
	virtual	bool	FillTN_array(be_CARRAY* pARR);
	virtual	bool	FillDIFF_TN_array(be_CARRAY* pARRdest,be_CARRAY* pARRsupport,int maxUN);
	virtual	bool	GetCapturability();							// true if all units can be capture
	virtual	void	DisableCapturability();
	virtual	void	EnableCapturability();
	// COSSACKS II
	virtual	int		GetNofBRLoadedGun();

	// transports
	virtual	int		GetNInside(bool Max=false);		// кол-во юнитов в транспорте
	virtual	int		GetLeaveAbility();				// причалили ли все транспорты в оруппе к берегу?
	virtual	void	PushNUnitAway(int NU=0);		// высадить N юнитов (N==0 всех)

	// units speed
	virtual	void	GetSpeed(int& maxS,int& minS,int& average);
	virtual	int		GetMaxSpeed();
	virtual	int		GetAverageSpeed();
	virtual	int		GetMinSpeed();
	virtual	void	SetSpeed(int speed);

	virtual	int 	GetMinX();
	virtual	int		GetMaxX();
	virtual	int 	GetMinY();
	virtual	int		GetMaxY();

	// Operators
	OneObject*		operator[](int _idx);

	// HELPER FUNCTIONS
	be_CLINC_LIST	lincs;

	// Base group units data
	LinearArray<OneUS,_OneUS>	Units;		// units index and serial - use for save

	// ------------------------------------------
	// group of die units (can be return in group)
	LinearArray<OneUS,_OneUS>	DeadUnits;	// list of units that die in batle but not erased
	word	isInDeadList(OneObject* pOB);	
	void	addDead(OneObject* pOB);		// remove dead units from Units in DeadUnits
	void	restoreResurrect();				// add Sunday unins back in active group list
	int		getDeadUN();
	// ------------------------------------------

	// Group varables
	bool						visible;	// visibility of group set on map
	int							NofNE;		// кол-во новых элементов в группе
	int							NofRE;		// number of removed units from group
	en_vGroup_Order				ORDER;		// current group order

	int							SendDispersion;	// Размыть позицию следования на SendDispersion пикселей
	void						SetSendDispersion(int _SD);
	int							GetSendDispersion() const;
	void						GetDispersePosition(int _x, int _y, int& _dx, int& _dy, OneObject* pOB);
	void						SetOB_useDispersePosition(int px,int py,OneObject* pOB,byte Prio,byte OrdType,bool donotCheckPos=false);

	bool			UnderAttack;// Group under attack
	virtual	bool	GetUA() const;
	virtual	void	SetUA(const bool UA);
	
	
	void			GetStructNameList( LinearArray<int,_int>& _list );
public:
	SAVE(be_CGROUP);
		REG_PARENT(be_CVariableBase);
		REG_AUTO(Units);
		REG_AUTO(DeadUnits);
		REG_MEMBER(_bool,visible);
		REG_MEMBER(_int,NofNE);
		REG_MEMBER(_int,NofRE);
	ENDSAVE;
};
class be_CNODE : public be_CVariableBase 
{
public:
	be_CNODE();
	be_CNODE(const char* _Name);
	be_CNODE(const char* _Name,const int _x,const int _y);
	be_CNODE(const char* _Name,const int _x,const int _y,const int _R);
	be_CNODE(const char* _Name,const int _x,const int _y,const int _R,const int _Dir);
	be_CNODE(const char* _Name,const int _x,const int _y,const int _R,const int _Dir,const int _SegmFR);
	be_CNODE(const int _x,const int _y);
	be_CNODE(const int _x,const int _y,const int _R);
	be_CNODE(const int _x,const int _y,const int _R,const int _Dir);
	be_CNODE(const int _x,const int _y,const int _R,const int _Dir,const int _SegmFR);
	virtual ~be_CNODE();

	bool			GetValueAsSTR(_str& ValueSTR);
public:
	// Base node data
	int			x,y;				// Координаты узла на карте.
	int			R;					// Radius
	int			Dir;				// Sehgment direction [0,255]
	int			SegmFR;				// Segmen angle size
	// Data for external use
	bool		selected;
	int			type;				// Begin=0 || End=1
public:
			void	GetTextCoordLT	(int&_x,int&_y);
			void	GetTextCoordRT	(int&_x,int&_y);
			void	GetNodeRect		(int&_x,int&_y,int&_x1,int&_y1);

	virtual	void	ScreenOnNode();

	virtual	void	vDrawID		();
	virtual	void	vDrawRect	(int r=100);
	virtual void	vDrawBuoy	();
	virtual	void	vDrawCircle	();
	virtual	void	vDrawXY		(DWORD pos=0,int dy=10,int dx=10,int r=100);
	virtual	void	vDrawNAME	(DWORD pos=1,int dy=30,int dx=10);
	virtual	void	vDraw		(bool forceDraw=false);

	virtual	void	vAddX	(int dx=1);
	virtual	void	vAddY	(int dy=1);
	virtual	void	vAddXY	(int dx=1,int dy=1);

	virtual	void	vSetX		(int _x);
	virtual	void	vSetY		(int _y);
	virtual	void	vSetXY		(int _x,int _y);
	virtual	void	vSetR		(int _R);
	virtual	void	vSetDir		(int _Dir);
	virtual	void	vSetSegmFR	(int _SegmFR);
	virtual	void	vSetGParam	(int _x,int _y,int _R,int _Dir,int _SegmFR);

	virtual	float	vGetNodeDist(int _x,int _y);

	virtual	int		vGetX		()	const;
	virtual	int		vGetY		()	const;
	virtual	int		vGetR		()	const;
	virtual	int		vGetDir		()	const;
	virtual	int		vGetSegmFR	()	const;
private:
	bool	InSector(float Fi);
public:
	SAVE(be_CNODE);
		REG_PARENT(be_CVariableBase);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_int,R);
		REG_MEMBER(_int,Dir);
		REG_MEMBER(_int,SegmFR);
	ENDSAVE;
};
class be_CBLANK	: public be_CNODE
{
public:
	be_CBLANK();
	be_CBLANK(const char* _Name,const int _x,const int _y);
	virtual	~be_CBLANK();
public:
	LinearArray<int,_UnitType>	UT_LIST;
public:
	virtual	void	vDraw		();

			void	DrawUT_List(int x0,int y0);	// Use screen coords
public:
	SAVE(be_CBLANK);
		REG_PARENT(be_CVariableBase);
		REG_BASE(be_CNODE);
		REG_AUTO(UT_LIST);
		REG_MEMBER(_int,x);				// Position oX
		REG_MEMBER(_int,y);				// Position oY
		REG_MEMBER_EX(_int,R,"D");		// Diagonal of blank position size
		REG_MEMBER_EX(_int,Dir,"Fi");	// Rot Fi round oZ
		REG_MEMBER_EX(_int,SegmFR,"H");	// Height
	ENDSAVE;
};
class be_CUnitType : public be_CINT
{
public:
	virtual	const	char*	GetThisElementView(const char* LoaclName);
	virtual bool	GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
	virtual	bool	GetValueAsSTR(_str& ValueSTR);

	SAVE(be_CUnitType);
		REG_PARENT(be_CVariableBase);
		REG_MEMBER(_UnitType,Value);
	ENDSAVE;
};
class be_CFurmationType : public be_CINT
{
public:
	SAVE(be_CFurmationType);
		REG_PARENT(be_CVariableBase);
		REG_ENUM(_index,Value,ALL_FORMATIONS);
	ENDSAVE;
};
// ARRAY /////////////////////////////////////////////////////////////////
class be_CARRAY	: public be_CVariableBase
{
public:
	be_CARRAY();
	be_CARRAY(const char* _Name);
	~be_CARRAY();
public:
	be_CParamList					ValuesArray;
public:
	virtual	bool					GetValueAsSTR(_str& ValueSTR)	{ ValueSTR = "Array";	return true; };
public:
	virtual	void					AddValue(be_CVariableBase* val);	
	virtual	void					InsertValue(be_CVariableBase* val,int pos);	// pos = 0,...,arrN-1
	virtual	be_CVariableBase*		GetValue(int pos);							// pos = 1,...,arrN
	virtual	be_CVariableBase*		strGetValue(const char* pos);				// pos = 1,...,arrN
	virtual	void					DelValue(int pos);							// pos = 1,...,arrN
	virtual	int						GetAmount();	
	virtual	be_CParamList*			GetParmList();

	be_CVariableBase*				operator[](int index);						// pos = 0,...,arrN-1

	Delimiter	D0;
	Delimiter	D1;

	virtual	void					vDraw();

	// Lazy array filler
	class CA_CLAZY_FILLER : public BaseFunction
	{
	public:
		void	EvaluateFunction(){};
		SAVE(CA_CLAZY_FILLER);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}LazyFiller;

public:
	SAVE(be_CARRAY);
		REG_BASE(be_CVariableBase);
		REG_AUTO(Name);
		REG_AUTO(D0);
//		REG_AUTO(LazyFiller);
//		REG_AUTO(D1);
		REG_AUTO(ValuesArray);
	ENDSAVE;
};
// SMART GROUP ///////////////////////////////////////////////////////////
class be_CMSG : public be_CMsgObject
{
public:
	be_CMSG();
	be_CMSG( const char* _name, const char* _sender_id, const char* _receiver_id );
	be_CMSG( const char* _name, const int _sender_id, const int _receiver_id );
	be_CMSG( be_CMsgObject* pmsg );
	char*	GetMSGName() const;
	void	SetMSGName(const char* _Name);

	const char* GetSender() const;
	const char* GetReceiver() const;

	virtual	int	GetType() { return 1; };

	be_CParamList				SomeData;
	be_CParamList*				GET_PARAMS() { return &SomeData; };

	void						ADD_PARAM	(const char* _name, const bool			_value);
	void						ADD_PARAM	(const char* _name, const int			_value);
	void						ADD_PARAM	(const char* _name, const DWORD			_value);
	void						ADD_PARAM	(const char* _name, const float			_value);
	void						ADD_PARAM	(const char* _name, const char*			_value);

	void						ADD_PARAM_REF(const char* _type,const char* _name,const char* _valueName);
};
// ONLY FOR TEST
class be_CMSG_FROMUSER : public ReferableBaseClass
{
public:
	be_CMSG_FROMUSER();
	~be_CMSG_FROMUSER();

	virtual			bool	CheckIfObjectIsGlobal(){ return true; };
		
	_str			msgName;		//name of message (an enumerated type works well)
	_str			sender_id;		//unique id of sender
	_str			receiver_id;	//unique id of receiver
	be_CParamList	SomeData;
	void						ADD_PARAM	(const char* _name, const bool			_value);
	void						ADD_PARAM	(const char* _name, const int			_value);
	void						ADD_PARAM	(const char* _name, const DWORD			_value);
	void						ADD_PARAM	(const char* _name, const float			_value);
	void						ADD_PARAM	(const char* _name, const char*			_value);

	bool						GET_PARAM	(const char* _name, bool	_value);
	bool						GET_PARAM	(const char* _name, int		_value);
	bool						GET_PARAM	(const char* _name, DWORD	_value);
	bool						GET_PARAM	(const char* _name, float	_value);
	bool						GET_PARAM	(const char* _name, _str&	_value);

	class CGetFromExample	: public BaseFunction
	{
	public:
		_str_ref	Example;
		void		EvaluateFunction();
		SAVE(CGetFromExample);
		REG_PARENT(BaseFunction);
		REG_ENUM( _strindex,Example		, BE_USMSG_ENUM );
		ENDSAVE;
	}GetFromExample;

	SAVE(be_CMSG_FROMUSER);
		REG_AUTO(Name);
		REG_AUTO(GetFromExample);
		REG_AUTO(msgName);
		REG_ENUM( _strindex,sender_id	, BE_SMARTOBJECT_ENUM );
		REG_ENUM( _strindex,receiver_id	, BE_SMARTOBJECT_ENUM );
		REG_AUTO(SomeData);
	ENDSAVE;
};
void g_beRouteMessage		( be_CMSG_FROMUSER* msg );
////////////////
class be_CFSM : public be_CVariableBase
{
public:

	be_CFSM();
	~be_CFSM();

	_str			FileName;
	_str			FunctionName;

	be_CParamList		Params;
	
	ClonesArray<be_CMSG_FROMUSER>	HendledMSG;
    
	void	LoadLuaFiles		(  );
	void	FSMInitialize		( be_CSMARTOBJECT* pSO );
	bool	ProcessStateMachine	( be_CSMARTOBJECT* pSO, char* State, be_CMsgObject* msg );
	
	void*	pDLL_FSM_FN;
	int		DLL_ProcessStateMachine	( be_CSMARTOBJECT* pSO, char* State, be_CMsgObject* msg );
	bool	DLL_VAILID();

	SAVE(be_CFSM);
		REG_LOADSAVE("Mission\\","FMS.xml");
		REG_BASE(be_CVariableBase);
		REG_AUTO(Name);					// State machin name
		REG_FILEPATH(FileName,".script,.lua,.luc");
		REG_AUTO(FunctionName);
		REG_AUTO(Params);
		REG_AUTO(HendledMSG);
	ENDSAVE;
};
class be_CSMARTOBJECT : public be_CVariableBase
{
public:
	be_CSMARTOBJECT();
	~be_CSMARTOBJECT();

	_str_ref	state_machine;		//the state machine that this object running
	_str		stateName;			//the current state of the state machine
	_str		next_stateName;		//the next state when a state change was requested
	bool		force_state_change;	//whether a state change has been requested
	
	char*	GetState() const				{ return (char*)stateName.cpchar();		};
	void	SetState(const char* _Name)		{ stateName = _Name;		};	
	char*	GetNextState() const			{ return (char*)next_stateName.cpchar();};
	void	SetNextState(const char* _Name)	{ next_stateName = _Name;	};
	bool	GetForceSTST() const			{ return force_state_change;};
	void	SetForceSTST(const bool _FST)	{ force_state_change=_FST;	};

	void	FSMInitialize();
	void	Update();

    bool	ProcessStateMachine	( char* State, be_CMsgObject* msg );

	// DRAW
	virtual	void				vDraw();

	_str_ref					ControledObject;
	be_CVariableBase*			GET_OBJECT() { return (be_CVariableBase*)(ControledObject.Get()); };
	bool						SET_OBJECT(const char* _name);
	bool						SET_OBJECT(pGroup& _group);
	be_CParamList				SomeData;
	be_CParamList*				GET_PARAMS() { return &SomeData; };
	void						CREATE_PRAM_FROM_FSM();
	class	CCreateParams : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(CCreateParams);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}CreateParams;
public:
	SAVE(be_CSMARTOBJECT);
		REG_BASE(be_CVariableBase);

		REG_AUTO(Name);								// Name of current smart object
		REG_ENUM(_strindex,state_machine,ALL_FSM);
		REG_ENUM(_strindex,ControledObject,ALL_vVALUES_ON_MAP);
		REG_AUTO(SomeData);
		REG_AUTO(CreateParams);

		INVISIBLE REG_AUTO(stateName);
		INVISIBLE REG_AUTO(next_stateName);
        INVISIBLE REG_MEMBER(_bool,force_state_change);
	ENDSAVE;
};
be_CSMARTOBJECT* g_beGetSMARTOBJECT(const char*			_name		);
be_CSMARTOBJECT* g_beGetSMARTOBJECT(const int			_OB_Index	);
be_CSMARTOBJECT* g_beGetSMARTOBJECT(be_CVariableBase*	pVAR		);
void			 g_beUPDATESMARTOBJ();
void			 g_beINITSMARTOBJECTS();
// SUB_PROGRAM ///////////////////////////////////////////////////////////

class AblName;

class be_CSUBFUNCTION : public be_CVariableBase
{
public:
	be_CSUBFUNCTION() : be_CVariableBase() { ACTIVE=true; };
	bool						ACTIVE;		// Call in game loop
	ClonesArray<AblName>		AbilityList;// Ability list for subFunction
	be_CParamList				InArgList;	// Local variables used in function
	be_CBaseUnaryStatement		BODY;		// Function body

	be_CBaseUnaryStatement		INIT;		// Load user defined functions and add some value for user seted

	Delimiter			D00;
	Delimiter			D01;
	
	int					Process();
	bool				IsMyOneObject(OneObject* pOB);

	void				ClearEntity();
public:
	SAVE(be_CSUBFUNCTION);
		REG_LOADSAVE("Mission\\","SUBF.xml");
		REG_BASE(be_CVariableBase);
		REG_AUTO(Name);					// Name of subfunction
		REG_MEMBER(_bool,ACTIVE);
		REG_AUTO(AbilityList);
		REG_AUTO(InArgList);
		REG_AUTO(D00);
		REG_AUTO(BODY);
		REG_AUTO(D01);
		REG_AUTO(INIT);
	ENDSAVE;
};
// CONVERTORS ////////////////////////////////////////////////////////////
be_CBOOL*			g_beConvBOOL(be_CVariableBase* _obj);
be_CDWORD*			g_beConvDWORD(be_CVariableBase* _obj);
be_CINT*			g_beConvINT(be_CVariableBase* _obj);
be_CFLOAT*			g_beConvFLOAT(be_CVariableBase* _obj);
be_CSTR*			g_beConvSTR(be_CVariableBase* _obj);
be_CGROUP*			g_beConvGROUP(be_CVariableBase* _obj);
be_CNODE*			g_beConvNODE(be_CVariableBase* _obj);
be_CARRAY*			g_beConvARRAY(be_CVariableBase* _obj);
be_CSTR_DIALOG*		g_beConvSTR_DIALOG(be_CVariableBase* _obj);
UnitAbility*		g_beConvABIL(be_CVariableBase* _obj);
WeaponModificator*	g_beConvWEAPONMOD(be_CVariableBase* _obj);
// CALSS REGISTRATION ////////////////////////////////////////////////////
void be_REG_CLASS_Variables();
//////////////////////////////////////////////////////////////////////////

#endif//__BASE__VARIABLES__



































