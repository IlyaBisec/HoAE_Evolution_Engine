#ifndef __UnitsInterface__
#define __UnitsInterface__

// Object Interface

enum ve_UnitType {ve_UT_hero=0, ve_UT_cannon=1, ve_UT_brigade=2, ve_UT_units=3, ve_UT_building=4};

struct vui_WeaponInfo{
	int Damage;
	int DamageBonus;
	int Charged;
	int WeaponID;
	void Clear(){memset(this,0,sizeof(*this));}
};
struct vui_ProduceInfo{
	bool Enabled;
	bool Disabled; // manual, для миссий
	word NProduce;
	word NUnlimit;	//  по бесконечности
	word NIndex;	
	int Stage;
	int MaxStage;
	bool Building;
	//
	char* AccessControl[8];
	int NAccessControl;
	//
	byte AIndex;
	int x,y;		// координаты кнопки
	byte HotKey;	
};
struct vui_UpgradeInfo{
	bool Enable;
	NewUpgrade* Upg;
	word Index;
	int FileID;
	int SpriteID;
	char* Message;
	//
	char IconPosition;
	int x,y;
	byte HotKey;
};
struct vui_CannInfo{
	OneObject* OB;
	int ChargeType, ChargeStage;
	int Shield;
	int NKills;
	short Damage[3];
	int NShots;
};
struct BrigParam{
	int Life;
	int MaxLife;
	int Morale;
	int MaxMorale;
	int NMembers;
	int NLiveMembers;
	int NShots;
	int NGrenades;
	int NKills;
	int Tiring;
	bool RifleAttack;
	int ReadyPercent;
};

struct vui_UnitInfo{
	bool GroundState;
	byte ActiveState;
	word Amount;
	int Life;
	word LifeMax;
	bool Peasant;
	int WeapType[3];
	int Damage[3];
	int Defence[4];
	int WeapFileID;
	int WeapColdSprite;
	int WeapFireSprite;
	int Shield;
	int NKills;
	bool RifleAttack;
	int NShots;
	int Delay;
	int DelayMax;
	int Morale;
	int MoraleMax;
	int		Speed;
	word	Spread;
	word	BuildingTime;
	int		Price[8];
	int		Cost;
	int		Vision;
	float	AttackSpeed;
	float	AttackSpeed2;
	int		AttackRadius;
	short	MoraleRegeneration;
	byte	StrikeProbability;
	short	VeteranKills;
	short	ExpertKills;
	int		AttackUpgrades[3];	// [Level: II, III, IV]
	// [Defence from: chopping, piercing, crushing][Level: II, III, IV]
	int		DefenceUpgrades[3][3];
	MonsterAbility	*pAbilities;
	// RemainUpgrades
	int		RUNumber;
#define UI_RUMAXNUMBER	10
	char*	RUHints[UI_RUMAXNUMBER];
	word	RUIconFileIDs[UI_RUMAXNUMBER], RUIconSpriteIDs[UI_RUMAXNUMBER];
	//
	int Kinetic;
	int KineticMax;
	//
	bool Patrol;
	bool HoldPosition;
};
class vui_SelPoint;
struct vui_BrigInfo{
	bool SetFromBrig(vui_SelPoint* SP);
	Brigade* BR;
	word BrigID;
	byte ActiveState;
	int Life;
	int MaxLife;
	int Morale;
	int MaxMorale;
	int NMembers;
	int NLiveMembers;
	int NShots;
	int ReadyPercent;
	int Grenades;
	int GrenadesMax;
	int NKills;
	int Tiring;
	bool RifleAttack;
	bool isGrenaders;
	int WeapFileID;
	int WeapColdSprite;
	int WeapFireSprite;
	int Shield;
	int ShieldAdd;
	int WeapType[3];
	int Damage[3];
	int DamageAdd[3];
	int Defence[4];
	int DefenceAdd[4];
	bool isFillable;
	byte FormID[3];
	word CurForm;
	word ShotLine[3];	// 0xFFFF - not allowed, + BrigID*8
	int  BrigDelay;		// stand ground
	int  BrigDelayMax;
	bool AttEnm;		// опущены штыки
	bool NoOrder;
	vui_UnitInfo	UI;
	int	MaxDamageAdd[3];
	int	MaxDefenceAdd[4];
	int ScaleFactor;
	int x,y;	
};

struct vui_HeroInfo: public vui_UnitInfo{
};

struct vui_BuildInfo{
	word Amount;
	bool Ready;
	int Life;
	word LifeMax;
	word Stage;
	word StageMax;
	word Places;
	word Population;
	word PopulationMax;
	byte SingleUpgLevel;
	OneObject* OB[10];
	vui_UnitInfo	UI;
	bool AllowShoot;
};
union vu_SelPointInfo{
	vui_CannInfo Cannon;
	vui_BrigInfo Brigade;
	vui_UnitInfo Units;
	vui_BuildInfo Buildings;
};

struct vu_UpgInfo{
	word ID;
	int Stage;
	int StageMax;
	byte SingleUpgLevel;
};
//
class ActiveUnitAbility;
//

class DynAbilityArray : public DynArray<ActiveUnitAbility*>{
public:
	int Add(ActiveUnitAbility* V);
};

class MonsterAbility;
class UnitActiveAbilityList;

class vui_SelPoint {
public:	
	byte NI;
	word NIndex;
	ve_UnitType Type;
	ve_UnitType Sort;
	vu_SelPointInfo Inf;
	//
	DynArray<vu_UpgInfo> Upg;
	OneObject* CanUpg;
	//
	MonsterAbility* Abl;
	UnitActiveAbilityList* ActAbl;
	//
	bool Hero;
	//
	DynAbilityArray Effects;
	DynAbilityArray Passive;			// контролы (Run)
	DynAbilityArray Buttons;			// активные маг. карты
	DynAbilityArray ButtonAuras;		// ауры
	DynAbilityArray LevelUps;
	DynAbilityArray Items;				// пояс с бутылками
	// inventory
	DynAbilityArray Inventory[10];		// одетые шмотки
	DynAbilityArray ItemsTakeOff;		// рюкзак шмоток
	// library
	DynAbilityArray LibShop;
	DynAbilityArray LibHero;
	// dialog
	DynAbilityArray Dialog;
	//
	DynAbilityArray Debug;
	//
	DynAbilityArray BasketMagicCards;	// рюкзак карточек
	DynAbilityArray BasketBottles;		// рюкзак бутылок

	DynAbilityArray AntiMageSkills;
	
	//ActiveUnitAbility* TankCannon;
	//ActiveUnitAbility* TankMachineGun;

	void SetActiveUnitAbility(UnitActiveAbilityList* UAAL);
	ActiveUnitAbility* GetActiveUnitAbility(int Index, int Type);
	int GetActiveUnitAbilityAmount(int Type);

	int GetAbilitySlotAmount(int Type);

	DynAbilityArray* GetActiveUnitAbilityArray(int Type);

	//
	OneObject* OB;
	//
	byte NatID;
	bool SearchVictim;
	int rX,rY;

	vui_SelPoint() { NIndex=0xFFFF; Abl=NULL; };
	
	void Init(OneObject* Obj);
	void PreInit(OneObject* Obj);

	int Cmp(vui_SelPoint* SP);	// 1 - SP<this, 0 - equal, -1 - SP>this
	bool Add(vui_SelPoint* SP);

	int TimeLastActivationDefence[NAttTypes];

};

//
class oi_UnitInfo: public BaseClass {
public:
	bool PriceShow;
	int Price[6];
	int Attack[2];
	int Defence[4];
	int KillCount;
	int LifeMax;
	int ManaMax;
	int AttackRate;
	int AttackDistance;
	int MotionSpeed;
	char* Message;
	SAVE(oi_UnitInfo){
		REG_MEMBER(_bool,PriceShow);
		//REG_MEMBER_EX(_int, Price[0], Price0);
		REG_MEMBER(_int, Price[0]);
		REG_MEMBER(_int, Price[1]);
		REG_MEMBER(_int, Price[2]);
		REG_MEMBER(_int, Price[3]);
		REG_MEMBER(_int, Price[4]);
		REG_MEMBER(_int, Price[5]);
		REG_MEMBER(_int, Attack[0]);
		REG_MEMBER(_int, Attack[1]);
		REG_MEMBER(_int, Defence[0]);
		REG_MEMBER(_int, Defence[1]);
		REG_MEMBER(_int, Defence[2]);
		REG_MEMBER(_int, Defence[3]);
		REG_MEMBER(_int, KillCount);
		REG_MEMBER(_int, LifeMax);
		REG_MEMBER(_int, ManaMax);
		REG_MEMBER(_int, AttackRate);
		REG_MEMBER(_int, AttackDistance);
		REG_MEMBER(_int, MotionSpeed);
		REG_MEMBER(_textid, Message);
	}ENDSAVE;
};
//

//////////////////////////////////////////////////////////////////////////

class DIALOGS_API OISelection: public ReferableBaseClass {
public:
	OISelection();
	~OISelection() {
		if(SelBr)free(SelBr); if(SelObj)free(SelObj); if(SelObjA)free(SelObjA);
		Clear();
	}		
	virtual bool CheckIfObjectIsGlobal() { return true; }
	
	void AddObj(OneObject* OB); // добавить обьект по индексу
	void Clear();
	void Process(byte NI);
	void CreateFromSelection(byte NI);

	byte SelNation;
	word MaxSelBr;
	word NSelBr;
	word* SelBr;		// индексы бригад

	word MaxSelObj;
	word NSelObj;
	word* SelObj;		// типы юнитов
	word* SelObjA;		// к-во юнитов

	struct OIS_Bld{
		word NIndex;
		DynArray<word> ID;
	};
	DynArray<OIS_Bld*> Bld;

	word Settlement;
	word Oboz;
	bool RifleAttEnabled;
	bool RifleAttackAllowed;

	int ActiveState;

	class COnBeginDragFn: public BaseFunction {

		bool Shop;
		bool Basket;
		bool Body;

		void EvaluateFunction();
		SAVE(COnBeginDragFn){
			REG_PARENT(BaseFunction);
			REG_MEMBER(_bool,Shop);
			REG_MEMBER(_bool,Basket);
			REG_MEMBER(_bool,Body);
		}ENDSAVE;
	};

	class COnDropFn: public BaseFunction {
		
		bool Shop;
		bool Basket;
		bool Body;
		bool Ground;
		
		void EvaluateFunction();
		SAVE(COnDropFn){
			REG_PARENT(BaseFunction);
            REG_MEMBER(_bool,Shop);
			REG_MEMBER(_bool,Basket);
			REG_MEMBER(_bool,Body);
			REG_MEMBER(_bool,Ground);
		}ENDSAVE;
	};

	int HoldPosition;

	class HoldPositionLeftClick: public BaseFunction {
	public:
		SAVE(HoldPositionLeftClick){
			REG_PARENT(BaseFunction);
		}ENDSAVE;
		void EvaluateFunction();
	};
	HoldPositionLeftClick HoldPositionOnLeftClick;
	
	int DragState; // 0-Shop, 1-Basket, 2-Body

	COnBeginDragFn OnBeginDrag;
	COnDropFn OnDrop;

	int LastSP;
	DynArray<vui_SelPoint> SelPoint;
	DynArray<vui_ProduceInfo> Produce;	
	DynArray<vui_UpgradeInfo> Upgrade;
	
	int SPSideLx;

	bool UnitInfoToggle;
	bool UnitInfoVisible;
	oi_UnitInfo UnitInfo;

	bool SetLastSP(word LSP);
	word GetNIndex(word SelPointID);	
	GeneralObject* GetGeneralObject(word SelPointID);
	GeneralObject* GetGeneralObject(ParentFrame* PF);
	vui_SelPoint* GetSelPoint(ParentFrame* PF);
	vui_SelPoint* GetLastSelPoint();

	void SetProduce();
	vui_ProduceInfo* GetProduceInfo(word ID);
	int	GetProduceAmount();
	vui_ProduceInfo* GetProduceInfoByAIndex(byte AIndex);
	int	GetProduceAmountByAIndex();


	void SetUpgrade();
	vui_UpgradeInfo* GetUpgradeInfo(word ID);
	int	GetUpgradeAmount();
	vui_UpgradeInfo* GetUpgradeInfoByIconPosition(word IconPosition);
	int GetUpgradeAmountByIconPosition();

	bool TankCannon;
	bool TankMachineGun;
	int TankCannonReload; // in percent
	_str TankMachineGunCharge; // amount of bullets
	int TankMachineGunReload; // in percent

	bool AntiMage;
	bool NotAntiMage;
	_str AntiMageSkillPoints;

	SAVE(OISelection){

		REG_PARENT(ReferableBaseClass);

		REG_AUTO(OnBeginDrag);
		REG_AUTO(OnDrop);

		REG_AUTO(HoldPositionOnLeftClick);

		REG_MEMBER(_int,HoldPosition);

		REG_MEMBER(_bool,UnitInfoToggle);
		REG_MEMBER(_bool,UnitInfoVisible);
		REG_AUTO(UnitInfo);

		REG_MEMBER(_bool,TankCannon);
		REG_MEMBER(_bool,TankMachineGun);
		REG_MEMBER(_int,TankCannonReload);
		//REG_MEMBER(_int,TankMachineGunCharge);
		REG_AUTO(TankMachineGunCharge);
		REG_MEMBER(_int,TankMachineGunReload);

		REG_MEMBER(_bool,AntiMage);
		REG_MEMBER(_bool,NotAntiMage);
		REG_AUTO(AntiMageSkillPoints);

	}ENDSAVE;

};
extern DIALOGS_API OISelection OIS;

class cDialogsHistory : public BaseClass {
public:
	bool Income;
	_str Text;
	SAVE(cDialogsHistory){
		REG_MEMBER(_bool,Income);
		REG_AUTO(Text);
	}ENDSAVE;
	void SetMessage(SimpleDialog* SD);
};
extern cDialogsHistory DialogsHistory;

//////////////////////////////////////////////////////////////////////////

struct cvs_BrigPanel{	
	bool Weapon[3];	
	bool Formation;
	bool Disband;
	bool Fill;
	bool Stop;
	cvs_BrigPanel(){
		Restore();
	}
	void Restore(){
		Weapon[0]=true;
		Weapon[1]=true;
		Weapon[2]=true;
		Formation=true;
		Disband=true;
		Fill=true;
		Stop=true;
	}
};
extern cvs_BrigPanel vBrigPanel;

//////////////////////////////////////////////////////////////////////////////////
#endif __UnitsInterface__