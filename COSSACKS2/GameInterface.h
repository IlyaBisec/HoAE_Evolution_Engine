#pragma once

#include "VB_enum.h"

#ifdef DIALOGS_DIP_USER
typedef	ReferableBaseClass be_CParamList;
typedef ReferableBaseClass be_CNODE;
typedef ReferableBaseClass be_CGROUP;
typedef ReferableBaseClass be_CARRAY;
typedef ReferableBaseClass be_CSTR_DIALOG;
typedef	BaseClass be_CMsgObject;
typedef	ReferableBaseClass be_CSMARTOBJECT;
typedef ReferableBaseClass ActiveGroup;
//typedef ReferableBaseClass UnitActiveAbilityList;
class ClassEditor;
#endif //DIALOGS_DIP_USER

#include "ClassEditorDefines.h"
//#include "ClassEditor.h"
//#include "ClassEditorsRegistration.h"

//#include "ua_Item.h"

struct iVector2D{
	//int x,y;
    union{
        int x,Min;
    };
	union{
		int y,Max;
	};
	iVector2D(iVector2D& v) { x=v.x; y=v.y; }
	iVector2D(int _x,int _y) { x=_x; y=_y; }
	iVector2D(){}

	//iVector2D& operator-(iVector2D& v) { x-=v.x; y-=v.y; return *this; }
	//iVector2D& operator+(iVector2D& v) { x+=v.x; y+=v.y; return *this; }
	friend iVector2D         operator-( const iVector2D& a, const iVector2D& b ) {
		iVector2D res;
		res.x = a.x - b.x; res.y = a.y - b.y; //res.z = a.z - b.z;
		return res; 
	}

	friend iVector2D         operator+( const iVector2D& a, const iVector2D& b ) {
		iVector2D res;
		res.x = a.x + b.x; res.y = a.y + b.y; //res.z = a.z + b.z;
		return res; 
	}

};
struct iVector3D{
    int x,y,z;
	iVector3D(int _x,int _y,int _z) { x=_x; y=_y; z=_z; }
	iVector3D(){};
};
struct iRect{
	int xmin,ymin;
	int xmax,ymax;
	iRect(){
		xmin=ymin=xmax=ymax=0;
	}
	iRect(int _x0,int _y0,int _x1,int _y1){
		xmin=_x0;ymin=_y0;xmax=_x1;ymax=_y1;
	}
	void	set(int _x0,int _y0,int _x1,int _y1){
		xmin=_x0;ymin=_y0;xmax=_x1;ymax=_y1;
	};
};
//priory constants
const int high_prio=128+16;      //order will be don anyway and will have high priory
const int low_prio=1;            //order will be done only if unit is not busy high priory task
const int order_low_prio=128+1;  //order will be done anyway but next time it will have low priory
//order type constants
const int ot_overall=0;          //all previous tasks will be cancelled and this order will be done
const int ot_beforeall=1;        //other orders will remain, but this order will be done first
const int ot_afterall=2;         //other orders will remain and this order will be done last
//////////////////////
enum directions_list{
	DIR_AUTO			= 512,
	DIR_RIGHT			= 0,
	DIR_RIGHT_DOWN		= 32,
	DIR_DOWN			= 64,
	DIR_LEFT_DOWN		= 96,
	DIR_LEFT			= 128,
	DIR_LEFT_UP			= 160,
	DIR_UP				= 192,
    DIR_RIGHT_UP		= 224
};
enum Lock_options{
	LOCK_LAND   = 1,
	LOCK_WATER  = 2,
	LOCK_LOWAIR = 4,
	LOCK_HIAIR  = 8,
	LOCK_ALL    = 15
};
enum collect_option{
	COLLECT_UNITS=1,
	COLLECT_BUILDINGS=2,
	COLLECT_ALL=3,
	COLLECT_MINES=4
};
class pUnitsList;
class pUnit;
class pParamList;
class pGroup;
class pCollectorGroup;
class iZone;
class iNode;
class iRect;
class RoundZone;
class SquareZone;
class pARRAY;
class pSTR_DIALOG;
class pABILITY;
class pWEAPONMOD;
class pSMARTOBJECT;
class pCAMERA;
class DIALOGS_API pUnitType:public BaseClass{
	int Type;
public:
	//construction
	pUnitType();
	pUnitType(int id);
	pUnitType(const char* Name);
	pUnitType(pUnit& unit);
    //setting
	void			SetUnitType(int id);
	void			SetUnitTypeName(const char* name);
	void			OfUnit(pUnit& unit);
	pUnitType& operator = (pUnitType&  op);
	pUnitType& operator = (const char* s);
	pUnitType& operator = (int id);
	pUnitType& operator = (pUnit& unit);
	//compare operators
	bool operator == (pUnitType& op);
	bool operator == (int id);
	bool operator == (const char* name);
	bool operator == (pUnit& unit);
    //getting	
	int				GetType(){return Type;}
	const char*		GetName();
	const char*		GetFullName();
	const char*		GetMDFileName();
	const char*		GetNationPreffix();
	const char*		GetNationName();
	int				GetNationID();
	iVector2D		GetAttackRange();
	iVector2D		GetAttackRange(int AttType);
	int				GetAdditionalRadius();
	void			GetCost(byte NI,/*word NIndex,*/int* Cost/*,int x,int y*/);

	//low-level functions
	NewMonster*		GetNewMonster();
	GeneralObject*	GetGeneralObject(byte NI=0);	
	AdvCharacter*	GetAdvCharacter(byte NI=0);	
	
};
class pUnitTypeList;
class pUpgradeList;
class HeroVariableStorage;
///////////////////////////////////////////////////////////
//////////////////UNIT MANAGEMENT//////////////////////////
///////////////////////////////////////////////////////////
struct DIALOGS_API RawUnit{
	RawUnit();
	RawUnit(word _id,word _sn);	
	word ID;
	word SN;
	OneObject* object();
};
class DIALOGS_API pUnit:public BaseClass,public RawUnit{    
	//word ID;
	//word SN;
public:
										pUnit(word  ID,word SN);
										pUnit(word ID);
										pUnit(OneObject* OB);
										pUnit();
										pUnit(DWORD UniqID);
	static pUnit						invalid();
	void 								Set(word ID,word SN);
	void 								Set(word ID);
	void 								Set(OneObject* OB);
	void 								SetUniqID(DWORD UniqID);
	pUnit& 								operator = (pUnit& unit);
	pUnit& 								operator = (word ID);
	bool								operator == (pUnitType& Type);
	bool								operator != (pUnitType& Type);
	bool								operator == (pUnit& unit);

	// for LUA export
	void 								Set_ID	( int	ID  );

	int 								Get_SN	() const;
	int 								Get_ID	() const;
	DWORD								Get_UID	() const;
    // --------------


	OneObject*							object	() const;
	int									index	(){return ID;}
	//action functions
	void SendUnitFarTo					(int x,int y,int dx=0,int dy=0,int Prio=high_prio,int OrdType=ot_overall);
	void SendUnitNearAndPrecise			(int	x,int y,int Prio=high_prio,int OrdType=ot_overall);
	bool AttackUnit						(pUnit& Victim,int NTimes=0,int Prio=high_prio,int OrdType=ot_overall);
	bool AttackPoint					(int x,int y,int NTimes=0,int Prio=high_prio,int OrdType=ot_overall);
    void RotateUnit                     (byte Dir,int OrdType=ot_overall);
    void SetUnitDirection               (byte Dir);
    
	void MakeStepTo						(int x,int y);
    void								Die();
	void								Erase();
	void								MakeDamageByAttackType(int Damage,int AttType,pUnit& AttackingUnit);
	void								MakeDamageByDamageType(int Damage,int DamType,pUnit& AttackingUnit);
	bool								TakeResource(int ResID);
	bool								TakeResource(int x,int y,int ResID);
	bool								TakeResourceRelative(int dx,int dy,int ResID);
	void								DetonateUnit(int dx,int dy,int Force);
	//produce/upgrade
	void								ProduceUnit(pUnitType& Type,pCollectorGroup* Dest=NULL,int Time=0);
	bool								ProduceDefaultUnit(pUnitType& Type,pCollectorGroup* Dest=NULL);
	void								GetProduceList(pUnitType& Producer,pUnitTypeList& List);
	void								GetEnabledProduceList(pUnit& Producer,pUnitTypeList& List);
	void								GetUpgradeList(pUpgradeList& List);
	void								GetEnabledUpgradeList(pUpgradeList& List);	
	//container management
	void								GoToContainer(pUnit& container);
	void								PushOutUnit(pUnitType& Type);
	void								PushOutUnit(pUnit& Unit);
	void								PushOutUnit();
	void								PushOutAllUnits();
	int									GetInsideAmount();
	void								GetInsideUnits(pUnitsList& units);
	int									GetAmountOfMovingTo(pUnit& container);
	void								GetMovingTo(pUnitsList& Units);
	bool								RemoveFromContainer();
	bool								RemoveFromContainer(pUnit& Container);
	void								InsertToContainer(pUnit& Container);
	void								Hide();
	void								UnHide();
	bool								Hidden();
		// for LUA export
		void							PushOutUnit00(pUnit* Unit);
		void							PushOutUnit01();
		bool							RemoveFromContainer01(pUnit* Container);
		bool							RemoveFromContainer00();
		void							SetHideState( bool state );
		bool							GetHideState() const;
		int								GetInsideUnitID(int LID);
		// --------------
	//get state functions
	int									GetLiveStatus();
	bool								CheckAttackPossibility(pUnit& Victim);
	bool								CheckVulnerabilityFrom(byte EnemyNI, pUnitType* UT=NULL);
	int									GetLifeAfterNextShot();
    
	// order state
	bool								IsBusy();
	bool								IsStand();
	bool								IsBuilding();
	bool								IsAttacking();
	bool								IsTakingResources();
	bool								IsRepairing();
	bool IsVisible(byte NI);
	
	int									GetStandTicks();
	pUnit								GetCurrentVictim();
	void								ClearOrders();
	bool								Valid();
	//animation control
	DWORD								GetCurrentAnimationType();//see NewAnimation.h to see all animation types
	int									GetCurrentAnimationFrame();
	int									GetPrevAnimationFrame();
	bool								CheckIfFrameIsNow(int Frame);
	bool								IsFrameFinished();
	bool								AnimationCanBeBroken();
	//global animations control
	NewAnimation*						GetAnimation(int Code,int Index=-1);
	NewAnimation*						GetCurrentAnimation();
	void								CopyAnimation(int DestCode,NewAnimation* Anm);	
	void								SwapAnimations(int SrcCode,int DstCode);
	void								DelAnimation(int Code,int Index=-1);
	int									GetAnimationsAmount(int Code);
	bool								CheckAnimationPresence(int Code);
	static DWORD						GetAnimationSource(NewAnimation* NA);

	DWORD								GetMask();
	int									GetSpeed();
	iVector2D							GetCoor();
	iVector3D							GetCoor3D();
	int									GetZ();
	float								GetScale();
	byte								GetNation();
	void								GetUnitCylinder(int& CylinderRarius,int& CylinderHeight,int& UnitHeight);
	iVector2D							GetLocalDesination();
	iVector2D							GetGlobalDesination();
	int									GetLife();
	int									GetMaxLife();
    int                                 GetUnitZ();
    int                                 GetUnitOverEarth();
	//change state functions
	void								SetCurrentAnimation(DWORD Code);
	void								ApplyAbility(const char* AbilityName);
	void								BornWeaponUnderUnit(const char* WeaponName,int extraZ=0,pUnit* DestUnit=NULL);
    void								SetGroupSpeed(int Speed);
	void								SetMask(DWORD Mask);
	void								SetCoor(iVector2D pos);
	void								SetZ(int z);
	void								SetScale(float Scale);
	void								SetNation(byte NI);
	void								SetLife(int Value);
	void								SetSearchVictim(bool SearchState);
	bool								GetSearchVictim();
    void                                SetUnitZ(int z);
    void                                SetUnitOverEarth(int z);
	//info functions
	int									GetDamage(int AttType);
	int									GetBasicDamage(int AttType);
	int									GetMaxDamage();
	int									GetProtection(int AttType);	
    bool								CheckInvisibility(DWORD	MaskOfWatchingUnit);
	iVector2D							GetBasicAttackRange();
	iVector2D							GetAttackRange();
	iVector2D							GetBasicAttackRange(int Type);
	iVector2D							GetAttackRange(int Type);
	iVector2D							GetOutputPoint(int AdditionalDistance);
	iVector2D							GetInsidePoint();
	int									DistTo(pUnit& OtherUnit);
	int									DistTo(iVector2D V);
	int									GetAdditionalRadius();
	//low-level functions
	NewMonster*							GetNewMonster();
	GeneralObject*						GetGeneralObject();
	AdvCharacter*						GetAdvCharacter();
	bool								CheckIfModified();
	void								ModifyUnit();
	void								UnModifyUnit();
    void                                SaveUnitCharacter(xmlQuote& XML);
    void                                LoadUnitCharacter(xmlQuote& XML);

	// герои
	bool	isHero() const;
	int		GetHeroLevel() const;
	bool	GetHeroCollectExpaStatus() const;
	void	SetHeroCollectExpaStatus(const bool CollectExpaStatus);
	void	GiveExpa(int amount);

	HeroVariableStorage* DetectHero() const;

	// PickUp
	//bool IsTouchingUnit();
	ActiveUnitAbility* GetPickUpAbility();					// проверка €вл€етс€ ли этот юнит сундучком
	bool PickUpUnit(pUnit& Chest, byte OrdType=ot_overall);	// приказ подн€ть вещь
	pUnit IsPickUping();									// выполн€етс€ приказ подн€ти€ вещи

	bool DetectChestNotEmpty() const;
	bool StartDialog(byte OrdType, OneObject* DialogOB);

	// ability state
	bool isStuned();

	// Dialog System
	void ShowOpinion(char* Text, DWORD Color=0xFFFFFF00);
};

//////////////////////////////////////////////////////////////////////////
class DIALOGS_API pParamList{
public:
	be_CParamList*		_params;
public:
	pParamList();
	pParamList(pParamList* pPL);
	pParamList(be_CParamList* pPL);
	be_CParamList*	params();
	bool			Valid();

	// For corect inERROR
	bool	getERROR(bool NotError,char* Type=NULL,char* Name=NULL);
	bool	addERROR(bool NotError,char* Type=NULL);
	char*	getParentFunctionName();
    
	void	CLEAR			();

	void	CreateOBJ_PL	();
	void	DeleteOBJ_PL	();

	bool		check			( char* Name );
	bool		IsLocal			( char* Name );

	pGroup		getGROUP		( char* Name );
	iNode		getNODE			( char* Name );
	float&		getFLOAT		( char* Name );
	_str&		getSTR			( char* Name );
    int&		getINT			( char* Name );
	DWORD&		getDWORD		( char* Name );
	bool&		getBOOL			( char* Name );
	pARRAY		getARRAY		( char* Name );
	pSTR_DIALOG	getDIALOG		( char* Name );
	pABILITY	getABILILTY		( char* Name );
	pWEAPONMOD	getWEAPONMOD	( char* Name );
	pCAMERA		getCAMERA		( char* Name );

	char*		getValueName	( char* Name );
	int			getSIZE			();

	bool	setREF		( char* Name, pGroup&	GR );
	bool	setREF		( char* Name, iNode&	ND );
	bool	setREF		( char* Name, pARRAY&	AR );

	pGroup&	addGROUP	( char* Name );
	iNode&	addNODE		( char* Name );
	float&	addFLOAT	( char* Name );
	_str&	addSTR		( char* Name );
	int&	addINT		( char* Name );
	DWORD&	addDWORD	( char* Name );
	bool&	addBOOL		( char* Name );
	pARRAY&	addARRAY	( char* Name );

	pParamList&	operator=(pParamList& PL);
	pParamList&	set(be_CParamList* pPL);

	void*	getVOID		( char* Name );
	bool	setREF		(  char* par_name, void* pData  );
};

///////////////////////////////////////////////////////////
/////////////////GROUPS MANAGEMENT/////////////////////////
///////////////////////////////////////////////////////////
class IUnitsIterator;
class DIALOGS_API IGroupOrder:public BaseClass{	
public:
	ClassRef<IUnitsIterator> GroupOwner;
	IUnitsIterator* GetGroup();
	virtual bool Process(){return true;}//should return false to cancel order
	virtual	void OnUnitDamage(pUnit& Victim,pUnit& Damager){};
	virtual void OnUnitDie(pUnit& Victim,pUnit& Killer){};
	void CancelThisOrder();
	///////////////////////////////////////////////////
    int StartTime;
	int NTimes;
	int TimesDone;
	int Period;
};
class DIALOGS_API IUnitsIterator:public ReferableBaseClass{
	int curpos;	
public:
	ClassArray<IGroupOrder> Orders;
	virtual RawUnit		_getunit(int idx) { return RawUnit(0xFFFF,0xFFFF); }	
	virtual void		_setunit(int idx,RawUnit& r){}
	virtual int			_getamount() { return 0; }
	virtual void		_add(RawUnit& r){}
	virtual void		_del(int pos,int n){}
	virtual void		_addnelem(int n);
	OneObject*			object(int pos);
	pUnit				unit(int Pos);
	virtual int			_find(RawUnit& r) { return -1; }
	/////////////////////////////////////////////////////////////
    void AddNewOrder(IGroupOrder* Order,int NTimes=0,int Period=0);
	///////////////////Group management//////////////////////////		
	void				ToBegin(int* pos=NULL);
	void				ToN(int pos);
	bool				Next(pUnit& u,int* pos=NULL);
	bool				NextLive(pUnit& u,int* pos=NULL);
	OneObject*			Next(int* pos=NULL);
	OneObject*			NextLive(int* pos=NULL);
	void				DelCurrent(int* pos=NULL);
	bool				HaveUnits();
	bool				HaveLiveUnits();
	void				RemoveDeadUnits();
	void				RemoveFinallyDeadUnits();
	void				Clear();
	int					GetAmount();
	pUnit				operator [] (int pos);	
	IUnitsIterator& 	operator += (pUnit& op);
	IUnitsIterator& 	operator -= (pUnit& op);
	IUnitsIterator& 	operator += (IUnitsIterator& op);//unification
	IUnitsIterator& 	operator -= (IUnitsIterator& op);//subtraction
	IUnitsIterator& 	operator &= (IUnitsIterator& op);//intersection	
	void				Assign(IUnitsIterator& op);//assignment	
	void				CreateFromZone(iZone& Zone,bool Add=false);
	void				CreateFromZone(int x,int y,int R,bool Add=false);
	void				CreateFromSquare(int xmin,int ymin,int xmax,int ymax,bool Add=false);
	void				CreateFromAreaID(int Layer,int AreaID,collect_option Flags=COLLECT_UNITS,bool Add=false);
	//limitation
	void				LimitByNation(int NI);
	void				LimitByNMask(DWORD NMask);
	void				LimitByType(int Type);
	void				LimitByType(pUnitType& Type);
	void				LimitByRound(int x,int y,int R);
	void				LimitBySquare(int xmin,int ymin,int xmax,int ymax);
	void				LimitByTZone(int Layer,int TZone);
	//excluding
	void				ExcludeByNation(int NI);
	void				ExcludeByNMask(DWORD NMask);
	void				ExcludeByType(int Type);
	void				ExcludeByType(pUnitType& Type);
	void				ExcludeByRound(int x,int y,int R);
	void				ExcludeBySquare(int xmin,int ymin,int xmax,int ymax);
	void				ExcludeByTZone(int Layer,int TZone);	
	///////////////////Group orders///////////////////////
	//Note: many of group orders can be done using macro command
	//ForEachUnit(GroupName,unit.command(...))
	//only commands, that can not be done in this way will be implemented there
	//////////////////////////////////////////////////////
	void				SendGroupTo(int x,int y,word Direction=DIR_AUTO,bool WithAttack=false,int type=0);
	void				OrderToAttackEnemyUnitsInZone(iZone& Zone,bool AttackBuildingsAlso=true);	
	void				MoveUnitsOnOneStepToPosition(int x,int y,bool WithAttack=false);

	iVector2D			GetCenter(); // geometry center
};
//very useful macros to order equal commands to every unit in group
#define ForEachUnit(group,command) {group.ToBegin();pUnit unit;while(group.Next(unit)){command;};};
//////////////////////////////////////////////////////////////////////////
//It is simply aray(list) of units. Group exists while exists object pUnitsList.
class DIALOGS_API pUnitsList: public IUnitsIterator{
	DynArray<RawUnit> units;
public:
	virtual RawUnit		_getunit(int idx);	
	virtual void		_setunit(int pos,RawUnit& r);	
	virtual int			_getamount();
	virtual void		_add(RawUnit& r);
	virtual void		_del(int pos,int n);
	virtual int			_find(RawUnit& r);

	void Select(byte NI, bool add );

	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);

	AUTONEW(pUnitsList);
};

//pGroups are groups that exist on map. They usually are created in Battle editor.
//Usually you see them in map editor like units with blue lines.
//It is better to use existing pGroups. If uou want to create new pGroup, you shoud
//think before, because for simple list of units you can use pUnitsList. 
//Group exist on map independently of exising of object pGroup.Object is really
//reference to this group.
class DIALOGS_API pGroup:public IUnitsIterator{
	ClassRef<be_CGROUP>		_group;
public:
	~pGroup();
    pGroup(pGroup& GR);
	pGroup(be_CGROUP* pGR);
    
	be_CGROUP*			group();
    virtual RawUnit		_getunit(int idx);	
	virtual void		_setunit(int idx,RawUnit& r);	
	virtual int			_getamount();
	virtual void		_add(RawUnit& r);
	virtual void		_del(int pos,int n);
						pGroup();
						pGroup(const char* Name);
	void				AttachTo(const char* GroupName);	
	bool				Valid();
	const char*			Name();
	void				Rename(const char* NewName);
	void				AttachOrCreateNew(const char* name);
	void				CreateLocal(char* Name=NULL);
	bool				isLocal();
	void				Free();
	static int			amount();//amount of pGroups on map
	static pGroup		getgroup(int idx);//getting specific pGroup

	pGroup&				operator=(pGroup& pGR);
	pGroup&				set(be_CGROUP* pGR);

	// From BE
		// get set seme params
	int				getUN();
	int				getUN(int NI);
	int				getUN(int NI, int UT);
	int				getUN(iZone& ZN);
	int				getUN(int x, int y, int r);
	int				getDeadUN();
	bool			getReady();
	int				getHP();			void	setHP(int HP);
	int				getMP();
	bool			getSearchEnemy();	void	setSearchEnemy(bool state);
	bool			getUnderAttack();	void	setUnderAttack(bool	state);
	int				getNO_ORDERS_UN	();
	int				getMOVE_UN		();
	int				getATTACK_UN	();
	int				getXc	();
	int				getYc	();
	bool			getXcYc	(int& xc,int& yc);
	int				getDIR	();
	int				getXi	(int lid);
	int				getYi	(int lid);
	int				getDIRi	(int lid);
	int				getGIDi	(int lid);
	int				getNation();		void	setNation(int NI);
	int				getNMASK();			void	setNMASK(DWORD NMASK);
	int				getDestDelta();		void	setDestDelta(int DD);
	bool			getAIGuard();		void	setAIGuard(int state);
	int				getLockType();		void	setLockType(int LT);		// !!!
	en_vGroup_Order	getORDER();												// !!!
	en_vGroup_Order	getORDERi(int lid);										// !!!
	char*			getName();
	int				getNew_UN(bool clearList=false);
	int				getRem_UN(bool clearList=false);
	int				getInside_UN(bool MaxUN=false);
	bool			getLeaveAbility();
	int				getEnemyAround_UN(int r);
	bool			getAttackPossibility(pGroup& GR);
	bool			getAttackPossibility(int x, int y, int r);
	bool			getAttackPossibility(iNode& node,int addR=0);
	bool			getStandGround();	void	setStandGround(bool state);
	bool			getCapturability();	void	setCapturability(bool state);
	int				getRandomPower();	void	setRandomPower(int power);
		// check functions
	bool			IsGroupAttackMe(pGroup& GR);
		// add rem functions
	void			RemDeadUnits	();
	void			RemAllUnits		();
	int				RemUnitsByNation(int NI);
	int				RemNUnitsToGroup(pGroup& GR, int N=-1);
	int				RemNReadyUnitsToGroup(pGroup& GR, int N=-1);
	int				RemUnitsInZone	(iNode& ZN, pGroup& GR);
	int				AddSelectedUnits	(int NI=0xFF);
    void			AddUnitsInZone		(int NI, iNode& ZN, bool add);
	void			AddUnitsInZone		(int NI, int x, int y, int r, bool add);
	void			AddUnitsTypeInZone	(int NI, int UT, iNode& ZN, bool add);
	void			AddUnitsTypeInZone	(int NI, int UT, int x, int y, int r, bool add);
	void			AddEnemyUnitsInZone	(int NI, iNode& ZN, bool add);
	void			AddEnemyUnitsInZone	(int NI, iNode& ZN, int addR, bool add);
	void			AddEnemyUnitsInZone	(int NI, int x, int y, int r, bool add);
	bool			AddUnitGID			(int gid);
	void			AddUnitsFromGroup	(pGroup& GR);
		// selections
	void			Select(bool add=false);
	void			Select(iZone& ZN, bool add=false);
	void			Select(int x, int y, int r, bool add=false);
		// commands
	void			ClearOrders();
	
	void			KillUnits(int N=-1);
	void			EraseUnits(int N=-1);
	
	void			TakeRess(int RID);			// !!!
	void			TakeFood();
	void			TakeWood();
	void			TakeStone();
	
	void			Produce(int UT, int UN);	// !!!
	void			Produce(int UT, int UN, pGroup& GR);

	void			Attack			(pGroup& GR, int power);
	void			AttackUseIDS	(int OverOrders,bool nearest=false,int addR=0);
	void			AttackUseIDS	(int OverOrders,bool nearest,int addR,pARRAY& ARR);
	void			AttackUseNearest(pGroup& GR);
	
	void			Send(int x, int y, int dir=DIR_AUTO, int type=0);
	void			Send(iNode& ZN, int dir=DIR_AUTO, int type=0);
	void			SendID(int lid, int x, int y, int dir=DIR_AUTO, int type=0,bool withAttack=false);
	void			SendID(int lid, iNode& ZN, int dir=DIR_AUTO, int type=0,bool withAttack=false);
	void			SendID(int x, int y, DWORD OverOrders, int lid, int dir=DIR_AUTO, int type=0,bool withAttack=false,bool donotCheckPos=false);
	void			SendID(iNode& ZN, DWORD OverOrders, int lid, int dir=DIR_AUTO, int type=0,bool withAttack=false,bool donotCheckPos=false);
	void			SendUseDisperse(int x, int y);
	void			SendUseDisperse(iNode& ZN);
	
	void			ChangeDirection(int dir, int type=0);

	void			MarkTime(int x, int y, int destD, int powerRND);
	void			MarkTime(iNode& node, int powerRND);
		// ability
	void			DeleteItemDrops();
	void			GiveExpa(int ExpaAmount);
	void			SetRallyPoint(int x, int y);
	void			SetRallyPoint(iNode& ZN);
		// spesial
	void			SendSmartly(int x, int y, int stepsN, iNode& destZN);
	void			SendSmartly(iNode& sZN, int stepsN, iNode& destZN);
	void			ExploringMap();
	bool			FillTN_array(pARRAY AR);
	bool			FillDIFF_TN_array(pARRAY ARRdest,pARRAY ARRsupport,int maxUN);
};	
//Collector is usually used only to collect units, that where built in some building
//Group exist on map independently of exising of object pCollectorGroup. Object is really
//reference to this group.
class DIALOGS_API pCollectorGroup:public IUnitsIterator{
	ClassRef<ActiveGroup> _group;
public:	
	ActiveGroup*		group();
	virtual RawUnit		_getunit(int idx);	
	virtual void		_setunit(int idx,RawUnit& r);	
	virtual int			_getamount();
	virtual void		_add(RawUnit& r);
	virtual void		_del(int pos,int n);
						pCollectorGroup();
						pCollectorGroup(const char* Name);
	void				AttachTo(const char* GroupName);
	bool				Valid();
	const char*			Name();
	void				Rename(const char* NewName);
	void				AttachOrCreateNew(const char* name);
	void				CreateUniq();
	void				Free();
	int					GetCollectorIndex();//for internal usage
	static int			amount();//amount of pCollectorGroup in game
	static pCollectorGroup getgroup(int idx);//getting specific pCollectorGroup
};
//really selected units - user does noes not see that units are sselected,
//but really controls them. To control and see selected units, you should
//fill pImSelected like pSelected or syncronise them in other way.
//It is better to use pSelected::units(ni) to refer selected units.
class DIALOGS_API pSelected:public IUnitsIterator{
	byte NI;
public:
	virtual RawUnit		_getunit(int idx);	
	virtual void		_setunit(int idx,RawUnit& r);	
	virtual int			_getamount();
	virtual void		_add(RawUnit& r);
	virtual void		_del(int pos,int n);
	virtual void		_addnelem(int n);
	pSelected			(){NI=0;}
	pSelected			(byte _NI){NI=_NI;}
	static pSelected&	units(int NI);
};
//imaginary selected units - this selection is visible on units in interface,
//but user does not controls them.
//It is better to use pImSelected::units(ni) to refer selected units.
class DIALOGS_API pImSelected:public IUnitsIterator{
	byte NI;
public:
	virtual RawUnit		_getunit(int idx);	
	virtual void		_setunit(int idx,RawUnit& r);	
	virtual int			_getamount();
	virtual void		_add(RawUnit& r);
	virtual void		_del(int pos,int n);	
	virtual void		_addnelem(int n);
	pImSelected			(){NI=0;}
	pImSelected			(byte _NI){NI=_NI;}
	static pImSelected&	units(int NI);
};
//formation from Cossacks & Cossacks2
class DIALOGS_API pBrigade:public IUnitsIterator{
	byte NI;
	int  Index;
public:
	virtual RawUnit		_getunit(int idx);	
	virtual void		_setunit(int idx,RawUnit& r);	
	virtual int			_getamount();
	virtual void		_add(RawUnit& r);
	virtual void		_del(int pos,int n);
	virtual void		_addnelem(int n);
	Brigade*			Brig();
	pBrigade			();
	pBrigade			(byte NI,int Index);
};
//this class does not contains units, but can be used 
//to calculate units withiot using more complex groups.
//It does not uses dynamical memory and so it is mutch faster
//to use it instead of usual group.
class DIALOGS_API pCounter:public IUnitsIterator{
	int  NUnits;
public:
						pCounter(){NUnits=0;}
	virtual RawUnit		_getunit(int idx){return RawUnit(0xFFFF,0xFFFF);}	
	virtual void		_setunit(int idx,RawUnit& r){if(r.object())NUnits++;}	
	virtual int			_getamount(){return NUnits;}
	virtual void		_add(RawUnit& r){if(r.object())NUnits++;}
	virtual void		_del(int pos,int n){NUnits-=n;}
	virtual void		_addnelem(int n){};	
};


///////////////////////////////////////////////////////////
/////////////////ZONES MANAGEMENT//////////////////////////
///////////////////////////////////////////////////////////
class RoundZone;
class SquareZone;
class DIALOGS_API iZone:public BaseClass{//interface of zone
public:
	virtual bool	CheckIfInside(int x,int y,int z){return false;};
	virtual bool	CheckIfInside(iVector2D& vec){return false;};
	virtual iRect	GetDimensions(){return iRect(0,0,0,0);};
	virtual void	CollectUnits(IUnitsIterator& it,collect_option Flags=COLLECT_UNITS);
	bool			IsIn(pUnit& unit);
	pure_declaration(iZone);
};
class DIALOGS_API iNode:public iZone{
public:
	~iNode();
	iNode();
	iNode(iNode& ND);
	iNode(be_CNODE* pND);

	ClassRef<be_CNODE>	_node;
	be_CNODE*		node();
	bool			Valid();
	virtual bool	CheckIfInside(int x,int y,int z);
	virtual iRect	GetDimensions();
	virtual void	CollectUnits(IUnitsIterator& it,collect_option Flags=COLLECT_UNITS);

	// From BE
	int		getX	();		void	setX	(int x);
	int		getY	();		void	setY	(int y);
    int		getR	();		void	setR	(int R);
	int		getDIR	();		void	setDIR	(int DIR);
	int		getSFR	();		void	setSFR	(int SFR);

	char*	getName	();		void	setName	(char* Name);	

	iNode&			operator=(iNode& ND);
	iNode&			set(be_CNODE* pND);
};
class DIALOGS_API RoundZone:public iZone{
public:
	int x,y,r;
	virtual bool	CheckIfInside(int x,int y,int z);
	virtual	iRect	GetDimensions();
	virtual void	CollectUnits(IUnitsIterator& it,collect_option Flags=COLLECT_UNITS);
	/////////////////////////////////////////////////
					RoundZone();
					RoundZone(const char* Name);
					RoundZone(int x,int y,int r);
					RoundZone(iNode& ND);
	void			AttachTo(const char* Name);
	RoundZone&		operator = (SquareZone& zone);
	RoundZone&		operator = (RoundZone& zone);
	void			SetXYR(int x,int y,int r);
    iVector2D		GetXY();
	int				GetR();
	declare_class(RoundZone);
};
class DIALOGS_API SquareZone:public iZone{
public:
	iRect r;	
	virtual bool	CheckIfInside(int x,int y,int z);
	virtual	iRect	GetDimensions();
	virtual void	CollectUnits(IUnitsIterator& it,collect_option Flags=COLLECT_UNITS);
	/////////////////////////////////////////////////
					SquareZone();
					SquareZone(const char* Name);
					SquareZone(iRect& rect);
					SquareZone(iNode& ND);
    void			AttachTo(const char* Name);
	SquareZone&		operator = (const char* Name);
	SquareZone&		operator = (SquareZone& zone);
	SquareZone&		operator = (RoundZone& zone);
	void			SetRect(iRect& rect);
	iRect			GetRect();
	declare_class(SquareZone);
};
class DIALOGS_API MultiZone:public iZone{
	ClassArray<iZone> zones;
public:
	virtual bool	CheckIfInside(int x,int y,int z);
	virtual	iRect	GetDimensions();
	virtual void	CollectUnits(IUnitsIterator& it,collect_option Flags=COLLECT_UNITS);
	/////////////////////////////////////////////////	
					MultiZone();
	MultiZone&		operator += (const char* name);
	MultiZone&		operator += (iZone& zone);
	MultiZone&		operator = (RoundZone& z);
	MultiZone&		operator = (SquareZone& z);
	MultiZone&		operator = (MultiZone& z);
	MultiZone&		operator = (const char* name);
	declare_class(MultiZone);
};
///////////////////////////////////////////////////////////
class DIALOGS_API pUnitTypeList:public BaseClass{
public:
	pUnitType&			operator [] (int idx);
	iVector2D			GetPosition (int idx);
	void				SetPosition (int idx,iVector2D& pos);
};
class DIALOGS_API pUpgrade:public BaseClass{
	byte				NI;
	int					Idx;
public:
	const char*			GetID();
	const char*			GetName();
	bool				GetEnableState();
	void				SetForceEnable(bool State);
    void				SetDefaultEnable();	
};
class DIALOGS_API pUpgradeList:public BaseClass{
	LinearClonesArray<pUpgrade> List;
public:
	pUpgrade&			operator [] (int idx);
	iVector2D			GetPosition (int idx);
	void				SetPosition (int idx,iVector2D& pos);
};
class DIALOGS_API pABILITY : public BaseClass
{
	ReferableBaseClass*			_ability;
public:
	pABILITY();
	pABILITY(char* name);
	pABILITY(pABILITY& ABLILITY);

	ReferableBaseClass*			ability();
	bool						Valid();
	pABILITY&					operator=(pABILITY& AR);
	pABILITY&					set(_str  AbilName);
	pABILITY&					set(char* AbilName);
	pABILITY&					set(ReferableBaseClass* pABL);
};
class DIALOGS_API pWEAPONMOD : public BaseClass
{
	ReferableBaseClass*			_weaponmod;
public:
	pWEAPONMOD();
	pWEAPONMOD(pWEAPONMOD& WEAPONMOD);

	ReferableBaseClass*			weaponmod();
	bool						Valid();
	pWEAPONMOD&					operator=(pWEAPONMOD& WM);
	pWEAPONMOD&					set(ReferableBaseClass* pWM);
};
///////////////////////////////////////////////////////////
/////////////////CMERA MANAGEMENT//////////////////////////
///////////////////////////////////////////////////////////
class DIALOGS_API pCAMERA : public BaseClass
{
	ClassRef<ReferableBaseClass>	_camera;
public:
	ReferableBaseClass* camera();
	bool				Valid();
	void				set(ReferableBaseClass* pC);
};
//////////////////////////////////////////////////////////////////////////
// pARRAY
//////////////////////////////////////////////////////////////////////////
class DIALOGS_API pARRAY : public BaseClass
{
public:
	ClassRef<be_CARRAY>		_array;

	pARRAY();
	pARRAY(pARRAY& AR);
	pARRAY(be_CARRAY* pAR);

	be_CARRAY*		array();
	bool			Valid();

	// For corect inERROR
	bool	getERROR(bool NotError,char* Type,int lid=-1);
	
	void			CLEAR();

	void			del(int lid);

	pGroup			getGROUP	(int lid);
	iNode			getNODE		(int lid);
	float&			getFLOAT	(int lid);
	_str&			getSTR		(int lid);
	int&			getINT		(int lid);
	DWORD&			getDWORD	(int lid);
	bool&			getBOOL		(int lid);
	pARRAY			getARRAY	(int lid);
	pSTR_DIALOG		getDIALOG	(int lid);

	pParamList		getParamList();		

	int				getSIZE();

	pARRAY&			operator=(pARRAY& AR);
	pARRAY&			set(be_CARRAY* pAR);
};
//////////////////////////////////////////////////////////////////////////
// pSTR_DIALOG
//////////////////////////////////////////////////////////////////////////
class DIALOGS_API pSTR_DIALOG : public BaseClass
{
	ClassRef<be_CSTR_DIALOG>	_dialog;
public:
	pSTR_DIALOG();
	pSTR_DIALOG(pSTR_DIALOG& DIALOG);

	be_CSTR_DIALOG*		dialog();
	bool				Valid();
	pSTR_DIALOG&		operator=(pSTR_DIALOG& DLG);
	pSTR_DIALOG&		set(be_CSTR_DIALOG* pDLG);

	char*				get_ogFile();
	pGroup				get_Group();
};
//////////////////////////////////////////////////////////////////////////
/////////////////////	FSM		//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// pMSG
class DIALOGS_API	pMSG : public BaseClass
{
public:
	be_CMsgObject*		_msg;
	
	be_CMsgObject*		msg();
	bool				Valid();

	_str&				str();
	void				set_str(_str& name);
	void				set_str(char* name);

	_str&				getSender();

	pParamList			getPARAMS();

	pMSG(be_CMsgObject* msg);
	pMSG(pMSG& MS);
	pMSG&				operator=(pMSG& msg);
};
// pSMARTOBJECT
class DIALOGS_API	pSMARTOBJECT : public BaseClass
{
public:
	pSMARTOBJECT();

	ClassRef<be_CSMARTOBJECT>	_obj;

	be_CSMARTOBJECT*			obj();
	bool						Valid();

	void						setSTATE	( char* stName );
	void						setSTATE	( _str& stName );
	_str&						getSTATE	();
	void						next_STATE	( char* stName );
	void						next_STATE	( _str& stName );
	void						FSC			( bool  type   );

	pGroup						getOBJ_GROUP();

	bool						setOBJ(char* _name);
	bool						setOBJ(pGroup _group);
	pParamList					getPARAMS();
	char*						getName();
	void						setName(char* name);

	pSMARTOBJECT(be_CSMARTOBJECT* obj);
	pSMARTOBJECT(pSMARTOBJECT& SO);
	pSMARTOBJECT&				operator=(pSMARTOBJECT& obj);
	pSMARTOBJECT&				set(be_CSMARTOBJECT* pobj);
};

//////////////////////////////////////////////////////////////////////////
/////////////// D I A L O G S  M A N A G E M E N T ///////////////////////
//////////////////////////////////////////////////////////////////////////

class DIALOGS_API OneDialogTopic:public BaseClass{
public:
	_str ID;
	_str Message;
	word IconFileID;
	int  IconSpriteID;
	_str VoiceFileReference;
	SAVE(OneDialogTopic);
		REG_AUTO(ID);
		REG_AUTO(Message);
		REG_MEMBER(_gpfile,IconFileID);
		REG_SPRITE(IconSpriteID,IconFileID);
		REG_FILEPATH(VoiceFileReference,".ogg");
	ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
class DIALOGS_API OneDialogItem:public BaseClass{
public:
	SAVE(OneDialogItem);		
	ENDSAVE;
};
class DIALOGS_API OneDialog:public OneDialogItem{
public:
	_str						DialogID;
	OneDialogTopic				Topic;
	ClonesArray<OneDialogTopic> Answers;
	SAVE(OneDialog);
		REG_PARENT(OneDialogItem);
		REG_AUTO(DialogID);
		REG_AUTO(Topic);
		REG_AUTO(Answers);
	ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
class ItemsList:public ClassArray<OneDialogItem>{
public:
	virtual bool ForceSimplification(){
		return true;
	}
};
class DIALOGS_API OneDialogsFolder:public OneDialogItem{
public:
	_str						FolderName;
	ItemsList					Items;
	OneDialog*					FindDialog(const char* ID);
	SAVE(OneDialogsFolder);
		REG_PARENT(OneDialogItem);
		REG_AUTO(FolderName);
		REG_AUTO(Items);
	ENDSAVE;
	const char* GetThisElementView(const char* Local){
		char* b=GetGlobalBuffer();
		sprintf(b,"{P Interf3\\btnz 26 12 3 -1 15}%s",FolderName.pchar());
		return b;
	}
};
class DIALOGS_API AnswerFn:public BaseFunction{
	virtual void EvaluateFunction();
	declare_class(AnswerFn);
};
class DIALOGS_API DialogInfo:public ReferableBaseClass{
public:
							DialogInfo();
							~DialogInfo();
	OneDialog				CurrentDialog;
	bool					Visible;
	pUnit					MsgSource;
	iVector3D				MsgPosition;
	int						CurrentAnswer;
	DialogsSystem			DLG;
	DialogsDesk*			Coor;
	AnswerFn				OnAnswer;
	pUnit					Answerer;
	int						StartTime;
	int						AnswerTime;
	int						HideTimeout;
	int						AnswerHideTimeout;
	int						DefaultAnswerID;
	//////////////////////////////////////////////////////////////////////////	
	void Process();
	void Draw();
	static void				DrawAndProcessAll();
	declare_class(DialogInfo);	
};
class DIALOGS_API pDialogInfo:public ClassRef<DialogInfo>{
	DialogInfo&				_di();
public:
	bool					Create(const char* ID);
	int						GetAnswer();
	void					ClearAnswerState();
	void					Show();
    void					Hide();
	bool					GetVisibleState();	
	void					ShowQuestionNearUnit(pUnit& Unit);
	void					ShowAnswerNearUnit(pUnit& Unit);	
	void					ForceSetAnswer(const char* ID,int Timeout=0);
	void					ForceSetAnswer(int idx,int Timeout=0);
	void					Close();
	void					SetHideTimeout(int Timeout);
	void					SetAnswerHideTimeout(int Timeout);
	pDialogInfo&			operator = (pDialogInfo& op);
	void					SetActiveQuest(const char* QuestName);
};
class DIALOGS_API pTrigger:public BaseClass{
	bool					State;
public:
							pTrigger()						{State=false;}
	_inline bool			Done()							{return State;}
	_inline bool			DoOnce()						{
		if(!State){
            State=true;
			return true;
		}
		return false;
	}
	_inline bool			NotYetDone()					{bool s=State;State=true;return !s;}
	_inline void			Set(bool value)					{State=value;}
	declare_class(pTrigger);
};
class DIALOGS_API pTimer:public BaseClass{
	int						StartTime;
	int						DoneTime;
	bool					Picked;
public:
							pTimer()						{StartTime=-1;DoneTime=0;Picked=false;}
	void					RunTimer(int RequiredPeriod);//ms
	bool					Done();	//time elapsed since running time is more then RequiredPeriod
									//If timer was not run it will return false.
	bool					DoneOnce(); //time elapsed since running time is more then RequiredPeriod
										//and it will return true only once.
										//If timer was not run it will return false.
	int						Elapsed();//time elapsed since thie timer was run. If timer was not run it returns 0.
	void					Reset();//resets all timer states to default.
	declare_class(pTimer);
};
class IExchangeInterface:public BaseClass{
public:
	virtual DWORD			GetUniqID() = 0;
};
/*
template <class _IExchangeInterface>
class IArrayInterface<_IExchangeInterface>:public IExchangeInterface{
    virtual _IExchangeInterface* operator [] (int index) = 0;
};
class IIndexedExtension:public IExchangeInterface{
public:
	virtual int& _getStaticIndex() = 0;
};
*/
struct TLinkInfo{
	word  NextAreaID;
	word  NextAreaDist;	
	DWORD LinkNatMask;
};

class WeaponParams;
class UnitAbility;
class ActiveUnitAbility;

class DIALOGS_API GameInterface:public GameExtension{	
public:
	virtual void OnUnloading();
	virtual void OnDrawOnMapOverAll();
	static _str NextLoadMap;
	static _str InfoMessage;
	static int  InfoMessageHideTime;
	static ClonesArray<ClassRef<IUnitsIterator> > ActiveGroups;
	static ClonesArray<DialogInfo> ActiveDialogs;
	//utilitary functions
	static int					random();//gives 0..32767
	static int					random(int NValues);//gives 0..NValues-1
	static int					random(int MinValue,int MaxValue);//gives MinValue..MaxValue
    static int                  height(int x,int y);
	//topological zones management
	static int					GetTLayersAmount();
	static int					GetTZonesAmount(int Layer);
    static iVector2D			GetTZoneCoor(int Layer,int Index);
	static int					GetTZoneIndex(int Layer,int x,int y);
	static int					GetTZoneNLinks(int Layer,int Index);
    static TLinkInfo*			GetTZoneLink(int Layer,int Index,int LinkIndex);
	static int					GetNextZoneOnTheWay(int Layer,int StartIndex,int FinalIndex,byte NI);
	static int					GetTZoneApproxRadius(int Layer,int Index);
	static int					GetAreaX(int AreaID, int layerID);
	static int					GetAreaY(int AreaID, int layerID);
	static bool					ControlNodeArray(pARRAY& ARR,int LT,int NI,bool circle=false);
	static bool					MarkBadNode(pARRAY& ARR,int LT,int NI,bool circle=false);
	//vectors management
	static	int					NORMA(int x1,int x2,int y1, int y2);
	static	int					NORMA(iVector2D& v);
	static	int					NORMA(int Layer,int StartTop,int FinalTop);
	static	float				DOT(iVector2D& v0,iVector2D& v1);
	static	int GetHeight(int x, int y);
	//camera managment
	static	void				CAMERA_Play(pCAMERA& c);
	static	bool				CAMERA_IsPlaying(pCAMERA& c);
	static	void				CAMERA_Stop(pCAMERA& c);
	static	void				CAMERA_Free(pCAMERA& c);
	static  void				CAMERA_SwitchToSelectedUnit(bool MoveInto,bool ManageUnit);
    //units management
	static int					GetUnitsAmount();
    static pUnit				GetUnit(int Index);
    static int					GetUnitsAmountInNation(int NI);
	static pUnit				GetUnitInNation(int NI,int Index);
	static pUnit				CreateUnit(int x,int y,byte NI,pUnitType& Type,bool Anyway=false,directions_list direction=DIR_AUTO);
	static int					CreateRandomUnits(int N,int x,int y,int R,byte NI,pUnitType& Type,pUnitsList* List=NULL,int MinDistBetweenUnits=0,bool Anyway=false,directions_list direction=DIR_AUTO);	
	static bool					CreateFormation(pGroup& storeGR,int x,int y,byte NI,pUnitType& Type,int FormationType,bool Anyway=false,directions_list direction=DIR_AUTO);
	static void					SetProduceList(byte NI,pUnitType& UnitType,pUnitTypeList& List);//--
	static void					SetUpgradeList(byte NI,pUnitType& UnitType,pUpgradeList& List);	//--
	static void					SetUnitForceEnable(byte NI,pUnitType& Type,bool state);//--
	static void					SetUnitDefaultEnable(byte NI,pUnitType& Type);//--
	static void					SetUpgradeForceEnable(byte NI,pUpgrade& Type,bool state);//--
	static void					SetUpgradeDefaultEnable(byte NI,pUpgrade& Type);//--
    static void					GetListOfAllUpgrades(byte NI,pUpgradeList& List);//--
    static void					GetListOfAllUnitTypes(byte NI,pUnitTypeList& List);//--
	static bool					GetInsideDialogState();
	static void					SetInsideDialogState(bool state);
	//dialogs management
	static pDialogInfo			GetDialog(const char* DialogID);//--	
	static void					PrintConsoleMessage(const char* mask,...);
	static void					PrintConsoleMessage2(const char* message, int time=255);
	static void					PrintCentralMessage(const char* mask,...);
	static void					PrintCentralMessage2(const char* message);	
	static void					ActivatePromptBar();
	static const char*			GetTextByID(const char* ID);
	static bool					PrepareString(_str& destSTR, _str& srcSTR, pARRAY& params);
	static void					PrintInfoMessage(int Duration,const char* mask,...);

	static void					AddDialogHistoryTalker(const char* chr);
	static void					AddDialogHistoryMessage(const char* chr);
	static void					AddDialogHistoryEndChapter();
	static void					AddDialogToHistory(pARRAY& dARR);

	//playing video
	static void					PlayFullscreen(const char* FileName,float px=0,float py=0);
	//geometry/locking management	
	//low-level functions - x,y,R,Lx,Ly are measured in lockpoints; 1 lickpoint=16 pixels
	static void					SetLockPt(int x,int y,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static void					ClrLockPt(int x,int y,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static void					XorLockPt(int x,int y,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static bool					ChkLockPt(int x,int y,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	//Lx<=24,Ly<=24
	static void					SetLockBar(int x,int y,int Lx,int Ly,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static void					ClrLockBar(int x,int y,int Lx,int Ly,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static void					XorLockBar(int x,int y,int Lx,int Ly,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static bool					ChkLockBar(int x,int y,int Lx,int Ly,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	//D<=24
    static void					SetLockRound(int x,int y,int D,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static void					ClrLockRound(int x,int y,int D,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static void					XorLockRound(int x,int y,int D,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	static bool					ChkLockRound(int x,int y,int D,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
    //Lx,Ly - any values 
	static void					RefreshTopology(int x,int y,int Lx,int Ly,DWORD opt=LOCK_LAND|LOCK_LOWAIR);
	//camera management - especially see OnSetCamera in parent class  GameExtension
    static iRect				GetMapLimitations();
	static void					SetMapLimitations(int x,int y,int x1,int y1);
	static void					SetDefaultLimitation();
	static void					SetPositionOnMap(float x,float y);
	static void					ScrollToPositionOnMap(float x,float y,int time);
	static iVector2D			GetCursorPosOnMap();
	static ICamera*				GetCamera();
    static void					SetCameraMovingStage(float Stage);
	_inline int 				ScrSizeX(){return RealLx;}
	_inline int					ScrSizeY(){return RealLy;}
	//camera track API - managing tracks, that are stored in MapOptions
	static	void				RunCameraTrack(const char* Name);
	static  bool				CheckIfCameraIsMoving();
	static	void				StopCameraTrack();
	static  bool				CheckIfTrackExists(const char* Name);
	static	int					GetAmountOfTracks();
	static	const char*			GetTrackName(int Idx);
	// nation 
	static	int					GetNKilled( int NI, pUnitType& UT );
	static	int					GetResource(int nat,int resid);
	static	bool				AddRessource(int nat,int resid,int Amount);
	static	int					SetAddFarms(int Nat,int Farms,bool set);
	static	int					GetNationMask(int Nat);
	//special effects
	static void					CreateSpecialEffect(const char* name,int srcx,int srcy,int srcz,int dstx,int dsty,int dstz);
	static void					CreateSpecialEffect(const char* name,pUnit& src,int ExtraSrcZ=0,pUnit& dst=pUnit::invalid(),int ExtraDstZ=0);
	static void					CreateAbility(const char* name,pUnit& unit);
	static void					ApplyAbility(const char* name,pUnit& unit,pUnit& caster);
	static void					AlertOnMap(int _x,int _y,int _delay,int _nat);
	static void					AddTomahawksAttackers(pGroup& Grp,byte Owner,bool remove,word Base=0,int DriftRadius=0, int CriticalMass=0);
	static void					ActivateObjectState(const char* ObjectName,const char* StateName);
	// UNITS ABILITY
	static bool					AddCarryAbility(int Index, pABILITY& UA);
	static bool					DeleteCarryAbility(int Index, pABILITY& UA);
	//static bool				DetectCarryAbility(int Index, pABILITY& UA);
	static ActiveUnitAbility*	DetectCarryAbility(int Index, pABILITY& UA);
	static ActiveUnitAbility*	DetectAbilityEffect(int Index, pABILITY& UA);
	static void					SetAbilityState(pGroup& GR,_str& AbilName,int state);
	static int					GetAbilityState(pGroup& GR,_str& AbilName);
	static int					GetAbilityState(pGroup& GR,pABILITY& Ability);
	static int					GetAbilityOBNation();
	static int					GetAbilityOwnerObject(pGroup& GR);
	static int					GetAbilityActivatorObject(pGroup& GR);
	static int					CheckAbilityActivatorObject(pGroup& GR);
	static bool					AbilityExecute(pABILITY& ABIL,pGroup& CASTER,pGroup& TARGET,int x,int y);
	static ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
	static bool CheckInfluenceMask(ActiveUnitAbility* V, DWORD Mask);

	static int GetAmountAbility(int UnitIndex, char* UnitAbilityName);
	static bool SetAbilityState(int UnitIndex, char* UnitAbilityName, int State); // State - enum enABL_STATE
	static bool SetAbilityPriceMoney(int UnitIndex, char* UnitAbilityName, int Price);	

	static UnitAbility* GetUnitAbility(char* UnitAbilityName);

	//static int GetClothesNTypes();
	static int GetClothesTypeCapacity(int TypeClothe);

	// WEAPON MODIFICATOR
	static void					CreateNewActiveWeaponMod(pWEAPONMOD& WM,int Index,int sx,int sy,int DestIndex,int dx,int dy);
	// CLOUD DIALOGS
	static int					dl_GetLineCountInCloudDialog(pSTR_DIALOG& DLG);
	static int					GetGlobalTime();
	static void					dl_ShowCloudDialog(pSTR_DIALOG& DLG,pGroup& GR);
	static void					dl_HideCloudDialog();
	static bool					FilePresent(char* FName);
	static bool					dl_GetCloudDilogCancel();
	static void					dl_SetCloudDilogCancel(bool state);
	static bool					dl_GetCloudDilogCenter();
	static void					dl_SetCloudDilogCenter(bool state);
	static void					dl_ClearCloudDilogSPEAKERS();
	static void					dl_AddCloudDilogSPEAKERS(pGroup GR);
	static bool					dl_getCenterOfSPEAKERS(int& x,int& y,pGroup& GR);
	static bool					dl_getCurrentSpeakerPosition(int& x,int& y);
	static int					dl_getAdditionalDelay();
	// SOUND
	static bool					ogg_Play(char* pFName);
	static void					ogg_Play(char* pFName,int stream);
	static void					ogg_Stop(int stream);
	static bool					ogg_StreamFinished(int stream);
	static int					ogg_GetSoundVolume();
	// DLL FUNCTIONS managnent
	static void					dllfnClearList();
	static void					modulfnClearList();
	// FSM managnent
	static bool					FreeSO_pVAR(pGroup& GR);
	static pSMARTOBJECT			FindSO(char* name);
	static void					SendMSG(char* name,pSMARTOBJECT& sender,pSMARTOBJECT& receiver);
	static void					SendMSG(char* name,int delay,pSMARTOBJECT& sender,pSMARTOBJECT& receiver);
	static bool					CheckObjectFSM(char* ObjectName,char* FSM_Name);
	static pSMARTOBJECT			GET_SO(char* FSM_Name);
	static pSMARTOBJECT			FindSObyName(char* SO_Name);
	static int					GetMsgSenderNation(_str strID);
    //class editor registration
    static void                 ProcessEditor(char* EditorName);
    static void                 ReplaceEditor(char* EditorName, BaseClass* pClass);
    static void                 AddStdEditor(char* Name,BaseClass* BC,const char* XmlName,DWORD Options,rce_ProcessClassCallback* Process=NULL,rce_InputCallback* Input=NULL,word HotKey=0);
    static void                 DelStdEditor(char* Name);
    static void                 SimpleEditClass(char* Name,BaseClass* BC,const char* XmlName="",bool Autosave=false);
    static void                 EditClass(char* Name,BaseClass* BC,const char* XmlName,DWORD Options,rce_ProcessClassCallback* Process,rce_InputCallback* Input);
	// Only for DLL add function decription managment
	static void					AddFunctionToDeclarationList(char* fn_name);
	static void					AddParamsToFunctionInList(char* fn_name,char* par_type,char* par_name);

	// Draw
	static void					WorldToScreenSpace( Vector3D& vec );
	static void					DrawString(char* Str, iVector2D& v, DWORD col );
	static void					DrawLine(iVector2D& v0, iVector2D& v1/*, int dZ*/, DWORD col );
	static void					DrawUnitsRanges(pUnitsList& units);
	static void					DrawSmartArrow(iVector2D& v0, iVector2D& v1/*, byte StartDirection, DWORD Color1, DWORD Color2*//*,
											int StartThickness, int EndThickness*/);

	static bool					ClassEditorCheckIfOpen(BaseClass* BC);

	//
	static void					SmoothMap(DynArray<short>& Map, int Iteration);
	static void SmoothDangerMap(short* DangMap, int ItrAmount);
	//

	//...shold be more...
	//
    //procesing script
	virtual void				ProcessScript(){};
	virtual void				Save(xmlQuote& Dest){};
	virtual void				Load(xmlQuote& Src){};
	static void					ProcessActiveGroups();
	static void					RunScript();
	static void					PauseScript();
	static void					SetLockPause(bool state);
	static void					Victory();
	static void					Victory(byte NI);
	static void					Defeat();
	static void					Defeat(byte NI);
	// TANK
	static void					SwithTankMode();
	static bool					GetTankMode();

	//general management
	static void					RunGameOnMap(const char* name);
	static const char*			GetCurrentMapName();
	static void					SwitchFogOfWar(bool State);
	static void					SwitchStopMode(bool State);
	static void					LogWarning( const char* format, ... );
	static bool					ShowOpenFileDialog(_str& FileName,const char* mask,const char* StartDir);
	static bool					ShowSaveFileDialog(_str& FileName,const char* mask,const char* StartDir);
	//mass player interface
	static void					CreateMassRoom(const char* MapName,const char* PlName,int HeroType);
	static bool					JoinMassRoom(const char* PlName,const char* ServerAddr,int HeroType);

	//dll-s exchange management	
	//if you want to expose(share) your interface, you should create
	//#define GUID_IMyInterface 0x12345678
	//class IMyInterface:public IExchangeInterface{//example
	//public:	
	//		virtual int  GetSomeParam() = 0;
	//		virtual void SetSomeParam(int Param) = 0;
	//};
	//this class should contain ONLY pure virtual functions.
	//the header, that contains this class can be shared between different DLL-s.
	//But also you should create implementation of all pure virtual functions:	
	//class MyInterface:public IMyInterface{
	//		int SomeParam;
    //public:
	//		virtual DWORD			GetUniqID(){
	//			return GUID_IMyInterface;
	//		}
	//		virtual int  GetSomeParam(){
	//			return SomeParam;
	//		}
	//		virtual void SetSomeParam(int Param){
	//				SomeParam=Param;
	//		}
	//};
	//MyInterface MyInterfaceVariable;
	//function ExposeMyInterfaces should return pointer &MyInterfaceVariable
	//example:
	//virtual IExchangeInterface*		ExposeMyInterfaces(int InterfaceIndex){
	//		if(InterfaceIndex==0)return &MyInterfaceVariable;
	//		return NULL;
	//}
	//You also can expose several classes:
	//MyInterface1 MyInterfaceVariable1;
	//MyInterface2 MyInterfaceVariable2;
	//virtual IExchangeInterface*		ExposeMyInterfaces(int InterfaceIndex){
	//		if(InterfaceIndex==0)return &MyInterfaceVariable1;
	//		if(InterfaceIndex==1)return &MyInterfaceVariable2;
	//		return NULL;
	//}
	//engine should call ExposeMyInterfaces until you will return NULL;
	virtual IExchangeInterface*		ExposeMyInterfaces(int InterfaceIndex){return NULL;}
	//you can use IExchangeInterface from oher DLL-s if you know its GlobalID.
	static IExchangeInterface*		GetExchangeInterface(DWORD GlobalID);	

	static bool isHumanPlayer(byte Color);
};    
/*
extern DIALOGS_API IIndexedExtension* _getIIE(int ClassIndex,int Index);


template <class derived_from_IIndexedExtension>
derived_from_IIndexedExtension* getUnitExt<derived_from_IIndexedExtension>(pUnit& unit){
	static derived_from_IIndexedExtension* Exp = GameInterface::GetExchangeInterface()
		return (derived_from_IIndexedExtension*)_getIIE()
}
template <class derived_from_IIndexedExtension>
derived_from_IIndexedExtension* getNTypeExt<derived_from_IIndexedExtension>(byte NI,pUnitType& type){


}
template <class derived_from_IIndexedExtension>
derived_from_IIndexedExtension* getTypeExt<derived_from_IIndexedExtension>(pUnitType& type){


}
*/
