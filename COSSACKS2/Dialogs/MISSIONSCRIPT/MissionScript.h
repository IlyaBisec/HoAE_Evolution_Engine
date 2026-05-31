
#ifndef __MISSION_SCRIPT_LV__
#define __MISSION_SCRIPT_LV__

//*************************************************************************************
//************************  ADITIONAL CLASS FOR SCRIPT   ******************************
//*************************************************************************************

#define		_CBASE_ZONE_		0xfff0
#define		_CZone00_			0xfff1
#define		_CZoneGiveQuest_	0xfff2
//Характеристики зон.
class	CBASE_ZONE : public BaseClass
{
public:
	DWORD	InfID;
	CBASE_ZONE()		{ InfID=_CBASE_ZONE_; };
public:
	SAVE( CBASE_ZONE );
	ENDSAVE;
};
class	CZone00	: public CBASE_ZONE
{
public:
	CZone00()			{ InfID=_CZone00_; };

	_str	ZoneName;		//Название зоны на карте.
	int		UnitType;		//Тип юнита которого нужно вставить.
	int		Direction;		//Направление созданного отряда.
	int		FormType;		//Тип генерируемой формации. 
	int		VictoryCond;	//Должен ли отряд умереть для победы.
public:
	SAVE( CZone00 );
		REG_PARENT( CBASE_ZONE );
		REG_AUTO( ZoneName );
#ifdef __MISSION_DLL_ANABLED__
		REG_MEMBER(_int,UnitType);
#endif
#ifndef __MISSION_DLL_ANABLED__
		REG_MEMBER(_UnitType,UnitType);
#endif
		REG_MEMBER( _int, Direction );
		REG_ENUM  ( _index,	FormType,	 FORMATION_TYPE );
		REG_ENUM  ( _index, VictoryCond, VICTORY_COND );
	ENDSAVE;
};
class	CZoneGiveQuest : public CBASE_ZONE
{
public:
	CZoneGiveQuest()		{ InfID=_CZoneGiveQuest_; };
	
	_str	ZoneName;		//Название зоны.

	int		UnitType;		//Тип юнита которого нужно вставить.//NEW
	int		Direction;		//Направление созданного юнита.    //NEW

	_str	PageName;		//Страница с текстом сообщения.
	int		Condition;		//Условие при котором её отображать.
public:
	SAVE( CZoneGiveQuest );
		REG_PARENT( CBASE_ZONE );
		REG_AUTO  ( ZoneName );

#ifdef __MISSION_DLL_ANABLED__
		REG_MEMBER(_int,UnitType);//NEW
#endif
#ifndef __MISSION_DLL_ANABLED__
		REG_MEMBER(_UnitType,UnitType);//NEW
#endif

		REG_MEMBER( _int, Direction );
		REG_AUTO  ( PageName );
		REG_ENUM  ( _index, Condition, QUEST_PAGE_COND );
	ENDSAVE;
};

#define		_CBASE_RESS_	0
#define		_CMinerals_		1
#define		_CTroops_		2
#define		_COpenZone_		3 //damn new
#define		_CGiveGroup_	4 //damn new //grey

#define		_COffFogOfWar_	9 //damn new

//Приз за выполнение миссии. (ресурсы)
class	CBASE_RESS : public BaseClass
{
public:
	DWORD	InfID;
	CBASE_RESS()	{ InfID=_CBASE_RESS_; };
public:
	SAVE( CBASE_RESS );
	ENDSAVE;
};

//Просто ресурс.
class CMinerals : public CBASE_RESS
{
public:	
	CMinerals()		{ InfID=_CMinerals_; };

	int		MineralsType;		//Wood,Gold,Stone,Food,Iron,Coal
	int		MineralsAmount;		//Кол-во данного ресурса.

public:
	SAVE( CMinerals );
		REG_PARENT( CBASE_RESS );
		REG_ENUM( _index, MineralsType, RESTYPE );
		REG_MEMBER( _int, MineralsAmount );
	ENDSAVE;
};

// Сделать видимой зону.
class COpenZone : public CBASE_RESS //damn new
{
public:	
	COpenZone()		{ InfID=_COpenZone_; };

	_str		ZoneName;		//Название зоны на карте.
	int			id;

public:
	SAVE( COpenZone );
		REG_PARENT( CBASE_RESS );
		REG_AUTO( ZoneName );
		REG_MEMBER( _int, id );
	ENDSAVE;
};


// отдать группу игроку .
class 	CGiveGroup : public CBASE_RESS //damn new
{
public:	
	CGiveGroup()	{ InfID=_CGiveGroup_; };

	_str		GroupName;		//Название группы на карте.
	int			id;

public:
	SAVE( CGiveGroup );
		REG_PARENT( CBASE_RESS );
		REG_AUTO( GroupName );
		REG_MEMBER( _int, id );
	ENDSAVE;
};

// Сделать видимой всю карту!!!. //damn new(TEMP)
class 	COffFogOfWar : public CBASE_RESS
{
public:	
	COffFogOfWar()		{ InfID=_COffFogOfWar_; };

	int			id;

public:
	SAVE( COffFogOfWar );
		REG_PARENT( CBASE_RESS );
		REG_MEMBER( _int, id );
	ENDSAVE;
};

//Отряд юнитов.
class 	CTroops : public CBASE_RESS
{
public:
	CTroops()		{ InfID=_CTroops_; };

	_str		ZoneName;		//Название зоны на карте.
	int			UnitType;		//Тип юнита которого нужно вставить.
	int			Direction;		//Направление созданного отряда.

public:
	SAVE( CTroops );
		REG_PARENT( CBASE_RESS );
		REG_AUTO( ZoneName );
#ifdef __MISSION_DLL_ANABLED__
		REG_MEMBER(_int,UnitType);
#endif
#ifndef __MISSION_DLL_ANABLED__
		REG_MEMBER(_UnitType,UnitType);
#endif
		REG_MEMBER( _int, Direction );
	ENDSAVE;
};

//*************************************************************************************
//*************************  BASE QUEST SCRIPT CLASS   ********************************
//*************************************************************************************
#define		_CSCRIPT_TYPE_		0
#define		_CKillEnemy00_		1
#define		_CDIFFICULTY_		2
#define		_CDirectonDataBase_	3

class		CSCRIPT_TYPE : public BaseClass
{
public:
	DWORD	InfID;
	CSCRIPT_TYPE()	{ InfID=_CSCRIPT_TYPE_; };
public:
	SAVE( CSCRIPT_TYPE );
	ENDSAVE;
};

//If you kill all anemy in primary crete zone, you vill vin.
class	CDIFFICULTY	: public BaseClass
{
public:
	DWORD	InfID;
	CDIFFICULTY()	{ InfID=_CDIFFICULTY_; };
public:
	ClassArray<CBASE_ZONE>		ZONES;
	ClassArray<CBASE_RESS>		PRIZS;
public:
	SAVE( CDIFFICULTY );
		REG_AUTO( ZONES );
		REG_AUTO( PRIZS );
	ENDSAVE;
};
class		CKillEnemy00 : public CSCRIPT_TYPE
{
public:
	CKillEnemy00()	{ InfID=_CKillEnemy00_; };

	int							Nation;
	CDIFFICULTY					Easy;
	CDIFFICULTY					Normal;
	CDIFFICULTY					Hard;

public:
	SAVE( CKillEnemy00 );
		REG_PARENT( CSCRIPT_TYPE );
		REG_MEMBER( _int, Nation );
		REG_AUTO( Easy	 );
		REG_AUTO( Normal );
		REG_AUTO( Hard	 );
	ENDSAVE;
};

//FOR CREATE DIRECTION DATA BASE, SECTORS ATTACK.
class	 CDirElement : public BaseClass
{
public:
	int		FromSecID;
	int		ToSecID;
	int		Direction;
public:
	SAVE( CDirElement );
		REG_MEMBER( _int, FromSecID );
		REG_MEMBER( _int, ToSecID   );
		REG_ENUM( _index, Direction, SDIR_ENUM );
	ENDSAVE;
};
class	 CDirElement_Array : public ClassArray<CDirElement>
{
public:
	virtual int GetExpansionRules(){			
		return 1;						//Добавить только сам класс, без наследников.
	}
};
class	 CDirectonDataBase : public CSCRIPT_TYPE 
{
public:
	CDirectonDataBase() { InfID=_CDirectonDataBase_; };

	CDirElement_Array	ELEM;
public:
	int	getDirection(int from, int to);

	SAVE( CDirectonDataBase );
		REG_PARENT( CSCRIPT_TYPE );
        REG_AUTO( ELEM );
	ENDSAVE;
};

//*************************************************************************************
//**********************      BASE SCRIPT CLASS		***********************************
//*************************************************************************************

//Sctip for curren mission. 
//Contain all init information for mission.
class 	CScript : public BaseClass
{
public:
	_str						ScriptName;
	ClassArray<CSCRIPT_TYPE>	ScriptType;
public:
	SAVE( CScript );
		REG_LOADSAVE("Missions\\MissionEngin\\","*.script.xml");
		REG_AUTO( ScriptName );
		REG_AUTO( ScriptType );
	ENDSAVE;
};
class 	CScript_Array:public ClassArray<CScript>{
public:
	virtual int GetExpansionRules(){			
		return 1;						//Добавить только сам класс, без наследников.
	}
};

class 	MISSION_SCRIPTS : public BaseClass
{
public:
	CScript_Array	SCRIPTS;

	SAVE( MISSION_SCRIPTS );
		REG_AUTO( SCRIPTS );
	ENDSAVE;
};

//*************************************************************************************
void	CREATE_SCRIPT();

_inline	void	RegMissionScriptClass(){
	REG_CLASS( CBASE_ZONE );
	REG_CLASS( CZone00 );
	REG_CLASS( CZoneGiveQuest );
	REG_CLASS( CBASE_RESS );
	REG_CLASS( CMinerals );
	//REG_CLASS( CTroops );
	REG_CLASS( COpenZone ); //damn new
	REG_CLASS( CGiveGroup ); //damn new
	//REG_CLASS( COffFogOfWar ); //damn new
	REG_CLASS( CTroops );

	REG_CLASS( CSCRIPT_TYPE );
	REG_CLASS( CDIFFICULTY );
	REG_CLASS( CKillEnemy00 );
	REG_CLASS( CDirElement);
	REG_CLASS( CDirectonDataBase );

	REG_CLASS( CScript );
	REG_CLASS( MISSION_SCRIPTS );
}

#endif	//__MISSION_SCRIPT_LV__






