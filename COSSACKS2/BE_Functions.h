#ifndef __BE_FUNCTIONS__
#define __BE_FUNCTIONS__

class lvCSquardsOnMap;
class _UnitType;
// be_CGROUP
class be_CNODE;
class be_CGROUP;
// vVALUE
class vvBASE;
class be_CBOOL;
class be_CDWORD;
class be_CINT;
class vvTEXT;
class vvPOINT2D;
class vvPOINT_SET;
class vvVector3D;
class vvDIALOG;
class vvMissionLOG;
class vvMISSMGR;
class vvTASKS_CII;
// GraphObject
class lvCGraphObject;
class lvCDeffFilmMenu;
// Conditions
class lvCCondition;
// Film
class lvCFilm;

// GLOBAL - use for define param list for user
extern DWORD	g_CO_USER_FLAGS; // for user set 0x00000010

// GLOBAL ////////////////////////////////////////////////////////////////
// 0 - archer
// 1 - short range unit
// 2 - shooter
// 3 - pesants
// 4 - not hidden
// 5 - tomagavk
void	FilterUnitsByCategory(be_CGROUP* pvg_Src,be_CGROUP* pvg_Dst,int Ctg);	// Ctg = (0,1,2,3,4,5)
void	AddStorm(be_CGROUP* Grp, byte Owner, int Diff,bool remove);
void	AddFirers(be_CGROUP* Grp,byte Owner,bool remove);
void	AddPsKillers(be_CGROUP* Grp,byte Owner,bool remove,bool SeakMine/*=true*/);
void	AddTomahawks(be_CGROUP* Grp,byte Owner,bool remove,word Base/*=0*/,int DriftRadius/*=0*/, int CriticalMass/*=0*/);

bool	CheckFilePath(char* _FilePath);
bool	CheckFilePath(_str& _FilePath);

//**********************************************************************//
//*****************    FOR EXPORT IN LUA		************************//
//**********************************************************************//

// OPERATION /////////////////////////////////////////////////////////////
int KillNatinInPOS_lua		(int nat,int x,int y,int R);
int SelectAll_lua			(int nat);
int SelectUnits_lua			(int nat,int x,int y,int R,bool add);
int SelectUnitsType_lua		(int nat,int UnitType,int x,int y,int R,bool add);
int ChangeAS_lua			(int nat,int state);
int SelSendTo_lua			(int nat,int x,int y,int dir,int type);
int ChangeFriends_lua		(int nat, int state);
int SetFriends_lua			(int nat0,int nat1);
int SetLightSpot_lua		(int x,int y,int R,int index);
int ClearLightSpot_lua		(int index);
int SetStartPoint_lua		(int x,int y);
int ShowVictory_lua			(int nat,const char* TextID);
int ShowVictory_lua			(int nat,const char* TextID);
int LooseGame_lua			(int nat,const char* TextID);
int SavePosition_lua		(be_CGROUP* pGrp,vvPOINT2D* pPos);
int SavePositionArr_lua		(be_CGROUP* pGrp,vvPOINT_SET* pPosArr);
int SetResource_lua			(int nat,int resID,int Amount);
int AddRessource_lua		(int nat,int resID,int Amount);
int ActivateTacticalAI_lua	(int nat);
int StartAIEx_lua			(int nat,const char* file,int lend,int mony,int res,int diff);
int SetAIEnableState_lua	(int nat,bool state);
int	SetTired_lua			(be_CGROUP* pGRP,int VAL);
int ShowDialog_lua			(vvDIALOG* pDLG);
int AddTextToDlg_lua		(vvDIALOG* pDLG,vvTEXT* pTXT);
int ClearDialog_lua			(vvDIALOG* pDLG);
int SetScrollLimit00_lua	(int x0,int y0,int x1, int y1,bool LockAroud);
int SetScrollLimit_lua		(vvPOINT2D* pPosLT,vvPOINT2D* pPosRB,bool LockAroud);
int SetScrollLimitNation00_lua		(int x0,int y0,int x1, int y1, int Nation, bool LockAroud);
int SetScrollLimitNation_lua		(vvPOINT2D* pPosLT, vvPOINT2D* pPosRB, int Nation, bool LockAroud);
int PutNewSquad_lua			(be_CGROUP* pGRP,int nat,int uType,int size,int x,int y,int dir);
int PutNewFormation_lua		(be_CGROUP* pGRP,int nat,int uType,int form,int x,int y,int dir);
int PutRandFormation_lua	(be_CGROUP* pGRP,int nat,int uType,int UN,int x,int y,int r);
int SetUnitStateCII_lua		(be_CGROUP* pGrp,bool shtiki,bool Fire,bool LI,bool LII,bool LIII);
int GroupHoldPOS_AI_lua		(be_CGROUP* pGRP,int x,int y,const char* FNane);
int GetNation_lua			(const char* strID);
int ChengeNMaskInNode_lua	(int Nat,int NMask,be_CNODE* pGR);
int SetAddFarms_lua			(int Nat,int Farms,bool set);
int	TeleportGroup_lua		(be_CGROUP* pvGrp,be_CNODE* pNode,int Direction,int dX,int dY);

// CONDITION /////////////////////////////////////////////////////////////
int GetUTypeByName_lua				(const char* UTName);
int GetFormationID_lua				(const char* formID);
int	GetAmount_lua					(int nat,bool buildings);
int GetUnitsAmount0_lua				(int nat,int x,int y,int R);		// Units of nation in location
int GetUnitsAmount2_lua				(int nat,int UT,int x,int y,int R);
int GetEnemyUnitsAmount_lua			(int mynat,int x,int y,int R);
int GetEnemyBuildindsAmount_lua		(int mynat,int x,int y,int R);
int GetTotalAmount1_lua				(int nat,int UT);
int GetReadyAmount_lua				(int nat,int UT);
int GetResource_lua					(int nat,int resid);
int	GetMoney_lua					(int nat);
int GetDiff_lua						(int nat);
int Trigg_lua						(int trigid);
int ogSTOP_lua						(int canal);
int CameraSTOP_lua					();
int GetLMode_lua					();
int CheckButton_lua					(int vkid);
int GetFormationType_lua			(be_CGROUP* pGrp);
int TestFillingAbility_lua			(be_CGROUP* pGrp);
int CInStandGround_lua				(be_CGROUP* pGrp,int AllBrigades);
int VillageOwner_lua				(const char* vilname);
int GetNofBrigInNode_lua			(int nat,int x,int y,int R);
int LoadingCoplite_lua				(be_CGROUP* pvGrp);
int GetTrueTime						();

// SOUND
void			ov_Play_lua(const char* pcszFileName,int ucStream);
void			ov_Stop_lua(int ucStream);
void			ov_SetVolume_lua(int ucVolume,int ucStream);
void			ov_Cyclic_lua(bool bCyclic,int ucStream);
bool			ov_StreamFinished_lua(int ucStream);

//////////////////////////////////////////////////////////////////////////

//************************************************************************
//************************************************************************
//
//	NEW OPERATION INHERIT FROM be_CFunctionBase 
//
//************************************************************************
//************************************************************************

class lvCKillNatinZone	: public be_CFunctionBase
{
public:
	int			parNat;
	_str_ref	parNode;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCKillNatinZone);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
	ENDSAVE;
};

class lvCKillNatNear	: public be_CFunctionBase
{
public:
	int			parNat;
	_str_ref	vGrp;
	int			parRad;
public:
	virtual int CallFunction();
public:
	SAVE(lvCKillNatNear);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_MEMBER(_int,parRad);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCEraseNation	: public be_CFunctionBase
{
public:
	int			parNat;
public:
	virtual int CallFunction();
public:
	SAVE(lvCEraseNation);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
	ENDSAVE;
};

class lvCSelectAll	: public be_CFunctionBase
{
public:
	int	parNat;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSelectAll);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
	ENDSAVE;
};

class lvCSelectNation	: public be_CFunctionBase
{
public:
	int		parNat;
	bool	parAdd;
	bool	parDom;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSelectNation);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_MEMBER(_bool,parAdd);
		REG_MEMBER(_bool,parDom);
	ENDSAVE;
};

class lvCChangeAS	: public be_CFunctionBase
{
public:
	int	parNat;
	int parState;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCChangeAS);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_ENUM(_index,parState,BE_UNIT_MOVE_MODE);
	ENDSAVE;
};

class lvCSelSendTo	: public be_CFunctionBase
{
public:
	int			parNat;
	_str_ref	parNode;
	int			parDir;
	int			parType;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSelSendTo);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);		
		REG_MEMBER(_int,parDir);
		REG_MEMBER(_int,parType);
	ENDSAVE;
};

class lvCGroupSendTo	: public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	_str_ref	parNode;
	int			parDir;
	int			parType;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCGroupSendTo);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);		
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,parDir);
		REG_MEMBER(_int,parType);
	ENDSAVE;
};

class lvCRotateGroup	: public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	int			parDir;
	int			prior;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCRotateGroup);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,parDir);
		REG_MEMBER(_int,prior);
	ENDSAVE;
};

class lvCSelectUnits	: public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	bool		parAdd;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSelectUnits);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,parAdd);
	ENDSAVE;
};

class lvCSetUnitState	: public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	int			parmode;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetUnitState);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_index,parmode,BE_UNIT_MOVE_MODE);
	ENDSAVE;
};

class lvCGetUnitMoveType : public be_CFunctionBase
{
public:
	_str_ref	parGrp;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCGetUnitMoveType);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCIncreaseLevel : public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	int			amount;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCIncreaseLevel);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,amount);
	ENDSAVE;
};

class lvCSetUnitMoveType : public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	int			movetype;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetUnitMoveType);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,movetype);
	ENDSAVE;
};

class lvCBrigReformation	: public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	int			TypeForm;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCBrigReformation);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,TypeForm);
	ENDSAVE;
};

class lvCChangeFriends	: public be_CFunctionBase
{
public:
	int		Nation;
	int		Value;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCChangeFriends);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nation);
		REG_MEMBER(_int,Value);
	ENDSAVE;
};

class lvCSetFriends	: public be_CFunctionBase
{
public:
	int		fstNat;
	int		secNat;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetFriends);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,fstNat);
		REG_MEMBER(_int,secNat);
	ENDSAVE;
};

class lvCChangeNationG	: public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	int			nwNat;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCChangeNationG);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,nwNat);
	ENDSAVE;
};

class lvCSetLightSpot	: public be_CFunctionBase
{
public:
	_str_ref	parNode;
	int			Radius;
	int			Index;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetLightSpot);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,Index);
	ENDSAVE;
};

class lvCClearLightSpot	: public be_CFunctionBase
{
public:
	int		Index;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCClearLightSpot);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Index);
	ENDSAVE;
};

class lvCSetStartPoint	: public be_CFunctionBase
{
public:
	bool		Use_VVal;
	_str_ref	parNode;
	_str_ref	sX;
	_str_ref	sY;
public:
	virtual int		CallFunction();
	DWORD	GetClassMask();
public:
	SAVE(lvCSetStartPoint);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,Use_VVal);
		SAVE_SECTION(0x01000000);
			REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		SAVE_SECTION(0x02000000);
			REG_ENUM(_strindex,sX,ALL_be_CINT_ON_MAP);
			REG_ENUM(_strindex,sY,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCShowVictory	: public be_CFunctionBase
{
public:
//	_str_ref	TextID;
	_str		TextID;
	int			Nat;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCShowVictory);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_AUTO(TextID);
//		REG_ENUM(_strindex,TextID,ALL_vvTEXT_ON_MAP);
	ENDSAVE;
};

class lvCLooseGame	: public be_CFunctionBase
{
public:
//	_str_ref	TextID;
	_str		TextID;
	int			Nat;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCLooseGame);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_AUTO(TextID);
//		REG_ENUM(_strindex,TextID,ALL_vvTEXT_ON_MAP);
	ENDSAVE;
};

class lvCSetTrigg	: public be_CFunctionBase
{
public:
	int		TID;
	int		NewVal;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetTrigg);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,TID);
		REG_MEMBER(_int,NewVal);
	ENDSAVE;
};

class lvCSavePlayersProfile	: public be_CFunctionBase
{
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSavePlayersProfile);
		REG_BASE(be_CFunctionBase);
	ENDSAVE;
};

class lvCTakeFood	: public be_CFunctionBase
{
public:
	_str_ref	GrpID;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCTakeFood);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCTakeWood	: public be_CFunctionBase
{
public:
	_str_ref	GrpID;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCTakeWood);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCTakeStone	: public be_CFunctionBase
{
public:
	_str_ref	GrpID;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCTakeStone);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCSetValue : public be_CFunctionBase
{
public:
	int						TypeID;
	bool					UseVV;

	_str_ref				Variable;
	_str_ref				Value;

	// triger
	bool					SetTG;	
	// word
	DWORD					SetWD;
	// integer
	int						SetIN;
public:
	virtual int				CallFunction();
	virtual bool			AskParentForUsingExpansionClass(char* MemberName,char* ClassName);
	DWORD			GetClassMask();
public:
	SAVE(lvCSetValue);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_index,TypeID,BE_VALUE_TYPE);
		REG_MEMBER(_bool,UseVV);

		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,Variable,ALL_be_CBOOL_ON_MAP);
		SAVE_SECTION(0x00000100);
			REG_MEMBER(_bool,SetTG);
		SAVE_SECTION(0x00001000);
			REG_ENUM(_strindex,Value,ALL_be_CBOOL_ON_MAP);

		SAVE_SECTION(0x00020000);
			REG_ENUM(_strindex,Variable,ALL_be_CDWORD_ON_MAP);
		SAVE_SECTION(0x00000200);
			REG_MEMBER(_DWORD,SetWD);
		SAVE_SECTION(0x00002000);
			REG_ENUM(_strindex,Value,ALL_be_CDWORD_ON_MAP);

		SAVE_SECTION(0x00040000);
			REG_ENUM(_strindex,Variable,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00000400);
			REG_MEMBER(_int,SetIN);
		SAVE_SECTION(0x00004000);
			REG_ENUM(_strindex,Value,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};
class lvCSetValueEX : public be_CFunctionBase
{
public:
	int						TypeID;
	_str_ref				Variable;
	be_CBaseUnaryOperator		Function;
public:
	virtual int				CallFunction();
	DWORD					GetClassMask();
public:
	SAVE(lvCSetValueEX);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_index,TypeID,BE_VALUE_TYPE);

		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,Variable,ALL_be_CBOOL_ON_MAP);
		
		SAVE_SECTION(0x00020000);
			REG_ENUM(_strindex,Variable,ALL_be_CDWORD_ON_MAP);
		
		SAVE_SECTION(0x00040000);
			REG_ENUM(_strindex,Variable,ALL_be_CINT_ON_MAP);
		
		SAVE_SECTION(0xFFFFFFFF);
			REG_AUTO(Function);
	ENDSAVE;
};
class lvCAddToInt : public be_CFunctionBase
{
public:
	_str_ref	IntValue;
	int			IntAdd;
public:	
	virtual int	CallFunction();
public:
	SAVE(lvCAddToInt);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,IntValue,ALL_be_CINT_ON_MAP);
		REG_MEMBER(_int,IntAdd);
	ENDSAVE;
};

class lvCAddToIntEx : public be_CFunctionBase
{
public:
	_str_ref	IntValue;
	int			IntAdd;
public:
	virtual int	CallFunction();
public:	
	SAVE(lvCAddToIntEx);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,IntValue,ALL_be_CINT_ON_MAP);
		REG_MEMBER(_int,IntAdd);
	ENDSAVE;
};

class lvCKillNUnits	: public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	int			UCount;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCKillNUnits);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,UCount);
	ENDSAVE;
};

class lvCEraseNUnits	: public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	int			UCount;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCEraseNUnits);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,UCount);
	ENDSAVE;
};

class lvCSavePosition : public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	bool		UseVV;
	_str_ref	VVpPos;
	_str_ref	vvPoint;
public:
	virtual int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCSavePosition);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,UseVV);
		SAVE_SECTION(0x00020000);
			REG_ENUM(_strindex,vvPoint,ALL_vvPOINT2D_ON_MAP);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,VVpPos,ALL_vvPOINT_SET_ON_MAP);
	ENDSAVE;
};

class lvCSendToPosition	: public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	_str_ref	VVpPos;
	bool		UsePoint;
	_str_ref	vvPoint;
public:
	virtual int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCSendToPosition);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,UsePoint);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,VVpPos,ALL_vvPOINT_SET_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_ENUM(_strindex,vvPoint,ALL_vvPOINT2D_ON_MAP);
	ENDSAVE;
};

class lvCSetRessource : public be_CFunctionBase
{
public:
	int			Nation;
	_str_ref	refNation;
	bool	UseVV;
	int		Food;			_str_ref		FoodVV;
	int		Wood;			_str_ref		WoodVV;
	int		Stown;			_str_ref		StownVV;
	int		Gold;			_str_ref		GoldVV;
	int		Iron;			_str_ref		IronVV;
	int		Coal;			_str_ref		CoalVV;
	int		Money;			_str_ref		MoneyVV;
public:
	virtual int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCSetRessource);
		REG_BASE(be_CFunctionBase);
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_int,Nation);
		SAVE_SECTION(0x00000002);
			REG_ENUM(_strindex,refNation,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00000003);
			REG_MEMBER(_bool,UseVV);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,Food);
			REG_MEMBER(_int,Wood);
			REG_MEMBER(_int,Stown);
			REG_MEMBER(_int,Gold);
			REG_MEMBER(_int,Iron);
			REG_MEMBER(_int,Coal);
			REG_MEMBER(_int,Money);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,FoodVV,ALL_be_CINT_ON_MAP);
			REG_ENUM(_strindex,WoodVV,ALL_be_CINT_ON_MAP);
			REG_ENUM(_strindex,StownVV,ALL_be_CINT_ON_MAP);
			REG_ENUM(_strindex,GoldVV,ALL_be_CINT_ON_MAP);
			REG_ENUM(_strindex,IronVV,ALL_be_CINT_ON_MAP);
			REG_ENUM(_strindex,CoalVV,ALL_be_CINT_ON_MAP);
			REG_ENUM(_strindex,MoneyVV,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCAddRessource : public be_CFunctionBase
{
public:
	int			RessType;
	int			Nat;
	_str_ref	refNat;
	bool		Use_VV;
	int			Value;
	_str_ref	vValue;
public:
	virtual			int		CallFunction();
	virtual			DWORD	GetClassMask();
public:	
	SAVE(lvCAddRessource);
		REG_BASE(be_CFunctionBase);
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_int,Nat);
		SAVE_SECTION(0x00000002);
			REG_ENUM(_strindex,refNat,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00000003);
			REG_ENUM(_index,RessType,RESTYPE);
			REG_MEMBER(_bool,Use_VV);
			SAVE_SECTION(0x00010000);
				REG_MEMBER(_int,Value);
			SAVE_SECTION(0x00020000);
				REG_ENUM(_strindex,vValue,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCStartAIEx : public be_CFunctionBase
{
public:
	int			Nation;
	_str		NameXML;
	int			Land;
	int			Money;
	int			ResOnMap;
	bool		Use_VV;
	_str_ref	vDifficulty;
	int			Difficulty;
public:
	virtual int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCStartAIEx);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nation);
		REG_FILEPATH(NameXML,".xml");
		REG_MEMBER(_int,Land);
		REG_MEMBER(_int,Money);
		REG_MEMBER(_int,ResOnMap);
		REG_MEMBER(_bool,Use_VV);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,vDifficulty,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,Difficulty);
	ENDSAVE;
};

class lvCSetAIEnableState : public be_CFunctionBase
{
public:
	int		Nat;
	bool	State;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetAIEnableState);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_MEMBER(_bool,State);
	ENDSAVE;
};

class lvCShowDialog : public be_CFunctionBase
{
public:
	_str_ref		Dialog;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCShowDialog);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,Dialog,ALL_vvDIALOG_ON_MAP);
	ENDSAVE;
};

class lvCAddTextToDlg : public be_CFunctionBase
{
public:
	_str_ref		Dialog;
	_str_ref		Text;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCAddTextToDlg);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,Dialog,ALL_vvDIALOG_ON_MAP);
		REG_ENUM(_strindex,Text,ALL_vvTEXT_ON_MAP);
	ENDSAVE;
};

class lvCClearDialog : public be_CFunctionBase
{
public:
	_str_ref		Dialog;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCClearDialog);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,Dialog,ALL_vvDIALOG_ON_MAP);
	ENDSAVE;
};

class lvCSetScrollLimit : public be_CFunctionBase
{
public:
	_str_ref		PosLT;
	_str_ref		PosRB;
	bool			LockAroud;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetScrollLimit);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,PosLT,ALL_be_CNODES_ON_MAP);
		REG_ENUM(_strindex,PosRB,ALL_be_CNODES_ON_MAP);
		REG_MEMBER(_bool,LockAroud);
	ENDSAVE;
};

class lvCSetScrollLimitNation : public be_CFunctionBase
{
public:
	_str_ref		PosLT;
	_str_ref		PosRB;
	int				Nation;
	bool			LockAroud;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetScrollLimitNation);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,PosLT,ALL_be_CNODES_ON_MAP);
		REG_ENUM(_strindex,PosRB,ALL_be_CNODES_ON_MAP);
		REG_MEMBER(_int,Nation);
		REG_MEMBER(_bool,LockAroud);
	ENDSAVE;
};

class lvTypeNum : public BaseClass
{
public:
	lvTypeNum(lvTypeNum* pTypeNum);
	lvTypeNum()	{ InfID = _lvTypeNum_; UnitType=0; };
	~lvTypeNum(){};
public:
	DWORD		InfID;
	_str_ref	vgUnitsID;		// Группа в которую строим юнитов
	int			UnitType;
	bool		Use_VV;
	int			Num;
	_str_ref	vNum;
public:
	virtual			DWORD	GetClassMask();
					bool	Prepare();
	virtual			void	GetCopy(lvTypeNum** pCopy);
	virtual const	char*	GetThisElementView(const char* LocalName);
public:
	SAVE(lvTypeNum);
		REG_ENUM(_strindex,vgUnitsID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_UnitType,UnitType);
		REG_MEMBER(_bool,Use_VV);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,vNum,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,Num);
	ENDSAVE;
};

class lvCSpotNUnits : public be_CFunctionBase
{
public:
	lvCSpotNUnits()		{ Status = 0; UGRP_ID=0xFFFF; };
public:
	_str_ref				vgBildsID;		// Здания преднозначенные для постройки юнитов (ОДНО ЗДАНИЕ!!!)
	_str_ref				parNode;
	bool					Use_VV;
	int						ProduceTime;	// Врея постройки юнита.
	_str_ref				vProduceTime;
	ClonesArray<lvTypeNum>	TypeNum;		// Тип юнита - кол-во

	UnitsGroup*				pUnitsGRP;		// Группа в которую строим юнитов.
	int						UGRP_ID;
	int						LastProdTime;	// Время последнего заказа юнита.
	int						Status;
	int						TypePID;
public:
	virtual			DWORD	GetClassMask();
	virtual int				CallFunction();
	void			Create();

	SAVE(lvCSpotNUnits);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vgBildsID,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_AUTO(TypeNum);
		REG_MEMBER(_bool,Use_VV);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,vProduceTime,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,ProduceTime);
	ENDSAVE;
};

class lvGrpNumBld : public BaseClass
{
public:
	lvGrpNumBld(lvGrpNumBld* pGrpNumBld);
	lvGrpNumBld()	{ Diff=0; RemoveAfterSend=true; };

	be_CBaseUnaryOperator		Cond;		// Условие
	_str_ref					vgUnits;	// Группа на отправку в бой.
	_str_ref					vgBildg;	// Здание в котором строятся войска.
	bool						Use_VV;
	int							Num;		// Сколько юнитов для отправки в бой должно быть в группе.
	_str_ref					vNum;
	int							AttackerType;
	int							Diff;
	bool						RemoveAfterSend;

	void						CallFunction();
	bool						Prepare();
	virtual	void				GetCopy(lvGrpNumBld** pCopy);
	virtual const	char*		GetThisElementView(const char* LocalName);

	virtual	DWORD				GetClassMask();

	SAVE(lvGrpNumBld);
		REG_AUTO(Cond);
		REG_ENUM(_strindex,vgUnits,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,vgBildg,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_index,AttackerType,BE_ATTACKER_TYPE);
		REG_MEMBER(_bool,RemoveAfterSend);
		REG_MEMBER(_int,Diff);
		REG_MEMBER(_bool,Use_VV);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,vNum,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,Num);
	ENDSAVE;
};

class lvCGoInBattle : public be_CFunctionBase
{
public:
	ClonesArray<lvGrpNumBld>	GrpNumBld;
public:
	virtual int				CallFunction();
public:
	SAVE(lvCGoInBattle);
		REG_BASE(be_CFunctionBase);
		REG_AUTO(GrpNumBld);
	ENDSAVE;
};

class lvCArtAttack : public be_CFunctionBase
{
public:
	_str_ref	vgArtID;
	_str_ref	vgTargID;
	bool		AttackImid;
	_str_ref	AttackActive;
public:	
	virtual int			CallFunction();
public:
	SAVE(lvCArtAttack);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vgArtID,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,vgTargID,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,AttackActive,ALL_be_CBOOL_ON_MAP);
	ENDSAVE;
};

class lvCPutNewSquad : public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	_str_ref	parNode;
	int			Nat;
	int			UnitType;
	int			SizeType;
	int			dir;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCPutNewSquad);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_MEMBER(_UnitType,UnitType);
		REG_MEMBER(_int,SizeType);
		REG_MEMBER(_int,dir);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCPutNewFormation : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;
	int			Nat;
	int			Form;
	int			UType;
	int			dir;
	int			ReadyPercent;
	bool		Use_Zone;
	_str_ref	parNode;
	_str_ref	Point;
public:
	virtual			int		CallFunction();
	virtual			DWORD	GetClassMask();
public:
	SAVE(lvCPutNewFormation);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);	
		REG_MEMBER(_int,Nat);
		REG_ENUM(_index,Form,ALL_FORMATIONS);
		REG_MEMBER(_UnitType,UType);	
		REG_MEMBER(_int,dir);
		REG_ISLIDER(ReadyPercent,0,100,100);
		REG_MEMBER(_bool,Use_Zone);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_ENUM(_strindex,Point,ALL_vvPOINT2D_ON_MAP);
	ENDSAVE;
	__constructor(lvCPutNewFormation){
		ReadyPercent=100;
	}
};

class lvCPutNewFormationParametric : public be_CFunctionBase
{
public:
	int			ArrIndex;
	_str_ref	parIndex;
	bool		UseVar;
	int			Nat;
	int			UNum;
	int			UType;
	int			dir;
	int			ReadyPercent;
	_str_ref	parNode;
	_str		arrayName;

public:
	virtual			int		CallFunction();
	virtual			DWORD	GetClassMask();
public:
	SAVE(lvCPutNewFormationParametric);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_MEMBER(_int,UNum);
		REG_MEMBER(_UnitType,UType);	
		REG_MEMBER(_int,dir);
		REG_ISLIDER(ReadyPercent,0,100,100);
		REG_AUTO(arrayName);
		REG_MEMBER(_bool,UseVar);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,parIndex,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,ArrIndex);
	ENDSAVE;
	__constructor(lvCPutNewFormationParametric){
		ReadyPercent=100;
	}
};

class lvCRepairBuildings : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;
	_str_ref	vBuildings;	
public:
	virtual			int		CallFunction();	
public:
	SAVE(lvCRepairBuildings);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,vBuildings,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;	
};


class lvCSetUnitStateCII	: public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	bool		Fire;		// Разрешить стрельбу. 
	bool		LineI;		// Разрешить стрельбу первой  линии.
	bool		LineII;		// Разрешить стрельбу второй  линии.
	bool		LineIII;	// Разрешить стрельбу третьей линии.
	bool		Stiki;		// Опустить, поднять штыки - только если запрещенно стрелять.
	bool		SetSG_Immediately;	
public:
	virtual int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCSetUnitStateCII);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,SetSG_Immediately);
		REG_MEMBER(_bool,Fire);
		SAVE_SECTION(0x00010000);
			REG_MEMBER(_bool,LineI);
			REG_MEMBER(_bool,LineII);
			REG_MEMBER(_bool,LineIII);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_bool,Stiki);
	ENDSAVE;
};

class lvCSendStikiToZone : public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	_str_ref	parNode;
	int			dir;
	int			OrdType;
public:
	virtual			int		CallFunction();
public:
	SAVE(lvCSendStikiToZone);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);	
		REG_MEMBER(_int,dir);
		REG_MEMBER(_int,OrdType);
	ENDSAVE;
};

class lvCSetUnitEnableState : public be_CFunctionBase
{
public:
	int		Nat;
	int		TypeID;
	int		State;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCSetUnitEnableState);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_MEMBER(_UnitType,TypeID);
		REG_MEMBER(_int,State);
	ENDSAVE;
};

class lvCSetUpgradeEnableStatus : public be_CFunctionBase
{
public:
	int	Nat;
	int UpgradeID;
	int	State;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCSetUpgradeEnableStatus);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_ENUM(_index,UpgradeID,ALL_UPGRADES);
		REG_MEMBER(_int,State);
	ENDSAVE;
};

class lvCSetUpgradeDone : public be_CFunctionBase
{
public:
	_str_ref	GrpID;
	int			UpgradeID;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCSetUpgradeDone);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_index,UpgradeID,ALL_UPGRADES);
	ENDSAVE;
};

class lvCTeleport : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;		// Телкпортируемая группа.
	int			Direction;	// Направление после телепорта (512 - не разворачивать)
	int			dX,dY;
	bool		Use_VVal;	
	_str_ref	parNode;
	_str_ref	Point;
public:
	virtual			int		CallFunction();
	virtual			DWORD	GetClassMask();
public:
	SAVE(lvCTeleport);	
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,Direction);
		REG_MEMBER(_int,dX);
		REG_MEMBER(_int,dY);
		REG_MEMBER(_bool,Use_VVal);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_ENUM(_strindex,Point,ALL_vvPOINT2D_ON_MAP);
	ENDSAVE;
};

class lvCDisband : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCDisband);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCScare : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;
public:	
	virtual		int		CallFunction();
public:	
	SAVE(lvCScare);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCClearSG : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCClearSG);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCUnloadSquad : public be_CFunctionBase
{
public:
	_str_ref	vGRP;
public:	
	virtual		int		CallFunction();
public:
	SAVE(lvCUnloadSquad);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGRP,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCSpotGrpByUType : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;
	int			UnitType;
	int			Nat;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCSpotGrpByUType);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_UnitType,UnitType);
	ENDSAVE;
};

class lvCRemoveNUnitsTo : public be_CFunctionBase
{
public:
	_str_ref	vGrpSource;
	_str_ref	vGrpDest;
	int	N;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCRemoveNUnitsTo);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpSource,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,vGrpDest,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,N);
	ENDSAVE;
};

class lvCSetMyNation : public be_CFunctionBase
{
public:
	int			Nat;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCSetMyNation);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
	ENDSAVE;
};

class lvCEqualizeSpeed : public be_CFunctionBase
{
public:
	_str_ref	vGrp0;
	_str_ref	vGrp1;
	int			EqType;	// Тип усреднения
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCEqualizeSpeed);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp0,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,vGrp1,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_index,EqType,BE_ASTIMATE_TYPE);
	ENDSAVE;
};

class lvCCreateBrigade : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
	_str_ref	vBrigGrp;
	bool		Use_VV;
	int			iSize;
	_str_ref	vSize;
public:
	virtual			int		CallFunction();
	virtual			DWORD	GetClassMask();
	int		GetSize();
public:
	SAVE(lvCCreateBrigade);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,vBrigGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,Use_VV);
		SAVE_SECTION(0x00010000);
			REG_MEMBER(_int,iSize);
		SAVE_SECTION(0x00020000);
			REG_ENUM(_strindex,vSize,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCAddWallSegment : public be_CFunctionBase
{
public:
	int			Nat;
	int			Type;
	_str_ref	BegPos;
	_str_ref	EndPos;
public:	
	virtual			int		CallFunction();
public:
	SAVE(lvCAddWallSegment);	
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_ENUM(_strindex,BegPos,ALL_vvPOINT2D_ON_MAP);
		REG_ENUM(_strindex,EndPos,ALL_vvPOINT2D_ON_MAP);
		REG_MEMBER(_int,Type);
	ENDSAVE;
};

class lvCUnitLimit : public be_CFunctionBase
{
public:
	int		Farms;
public:
	virtual			int		CallFunction();
public:
	SAVE(lvCUnitLimit);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Farms);
	ENDSAVE;
};

class lvCAddFarms : public be_CFunctionBase
{
// Добавить жилые места данной нации <addFarms> или установить конкретное значение <setFarms>
public:
	int		Nat;
	int		Farms;
	bool	SET;	// if ==-1 do nothing
public:
	virtual			int		CallFunction();
public:
	SAVE(lvCAddFarms);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_MEMBER(_int,Farms);
		REG_MEMBER(_bool,SET);
	ENDSAVE;
};

class lvCSetSerchWFlag : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
	bool		SearchWict;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCSetSerchWFlag);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,SearchWict);
	ENDSAVE;
};

class lvCClearOrders : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCClearOrders);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCClearDead : public be_CFunctionBase
{
public:
	virtual			int		CallFunction();
public:
	SAVE(lvCClearDead);
		REG_BASE(be_CFunctionBase);
	ENDSAVE;
};

class lvCGroupMovement : public be_CFunctionBase
{
public:
	int			MovementState;
	_str_ref	vGrp;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCGroupMovement);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_index,MovementState,BE_NODE_ACTION);
	ENDSAVE;
};

class lvCSetTired : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
	int			SetedTiredValue;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCSetTired);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,SetedTiredValue);
	ENDSAVE;
};

//=====================	 FUNCTION FOR TRANSPORT	 =======================//
class lvCPushNUnitAway : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;
	int			N;
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCPushNUnitAway);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,N);
	ENDSAVE;
};

class lvCSendUnitsToTransport : public be_CFunctionBase
{
public:
	_str_ref	vGrpTransID;	// Транспорты
	_str_ref	vGrpUnitsID;	// Юниты
public:
	virtual		int		CallFunction();
public:
	SAVE(lvCSendUnitsToTransport);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpUnitsID,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,vGrpTransID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

//===================	FUNCTION FOR FILM		========================//
class lvCSetFGV : public be_CFunctionBase
{
public:
	_str_ref	GraphObj;
	_str_ref	newGraphObj;
	bool		State;
	bool		Immediately;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetFGV);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,newGraphObj,ALL_vVALUES_ON_MAP);
		REG_ENUM(_strindex,GraphObj,ALL_lvCGraphObject_ON_MAP);
		REG_MEMBER(_bool,State);
		REG_MEMBER(_bool,Immediately);
	ENDSAVE;
};

class lvCPlayOGMiss : public be_CFunctionBase
{
public:
	_str		pFileName;
	DWORD		pStream;
	bool		pCyclic;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCPlayOGMiss);
		REG_BASE(be_CFunctionBase);
		REG_FILEPATH(pFileName,".ogg");
		REG_MEMBER(_DWORD,pStream);
		REG_MEMBER(_bool,pCyclic);
	ENDSAVE;
};

class lvCStopOGMiss : public be_CFunctionBase
{
public:
	DWORD			pStream;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCStopOGMiss);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_DWORD,pStream);
	ENDSAVE;
};

class lvCOGSetVolume : public be_CFunctionBase
{
public:
	int			Volume;
	DWORD		pStream;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCOGSetVolume);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_DWORD,pStream);
		REG_MEMBER(_int,Volume);
	ENDSAVE;
};

class lvCOGFinishMiss : public be_CFunctionBase
{
public:
	DWORD		pStream;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCOGFinishMiss);
		REG_PARENT(be_CFunctionBase);
		REG_MEMBER(_DWORD,pStream);
	ENDSAVE;
};

class lvCFreezeGame : public be_CFunctionBase
{
public:
	virtual int		CallFunction();
public:
	SAVE(lvCFreezeGame);
		REG_BASE(be_CFunctionBase);
	ENDSAVE;
};

class lvCUnFreezeGame : public be_CFunctionBase
{
public:
	virtual int		CallFunction();
public:
	SAVE(lvCUnFreezeGame);
		REG_BASE(be_CFunctionBase);
	ENDSAVE;
};

class lvCUnFreezeGroup : public be_CFunctionBase
{
public:
	_str_ref		GrpID;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCUnFreezeGroup);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCFreezeAndHidden : public be_CFunctionBase
{
public:
	bool		bHome;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCFreezeAndHidden);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,bHome);
	ENDSAVE;
};

class lvCFreezeAndHiddenGame : public be_CFunctionBase
{
public:
	_str_ref	parNode;
	bool		bHome;
public:
	virtual int	CallFunction();
public:
	SAVE(lvCFreezeAndHiddenGame);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_MEMBER(_bool,bHome);
	ENDSAVE;
};

class lvCUnFreezeAndUnHidden : public be_CFunctionBase
{
public:
	virtual int		CallFunction();
public:
	SAVE(lvCUnFreezeAndUnHidden);
		REG_BASE(be_CFunctionBase);
	ENDSAVE;
};

class lvCUnFreezeAndUnHiddenGame : public be_CFunctionBase
{
public:
	_str_ref		parNode;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCUnFreezeAndUnHiddenGame);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
	ENDSAVE;
};

class lvCUnFreezeAndUnHiddenGroup : public be_CFunctionBase
{
public:
	_str_ref		GrpID;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCUnFreezeAndUnHiddenGroup);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCSetLeftPort : public be_CFunctionBase
{
public:
	_str_ref		MENU;
	word			FileID;
	int				SpriteID;
	_str_ref		HName;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetLeftPort);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,MENU,ALL_lvCDeffFilmMenu_ON_MAP);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_ENUM(_strindex,HName,ALL_vvTEXT_ON_MAP);		// Only for use in CossacsII interface!!!
	ENDSAVE;
};

class lvCSetRightPort : public be_CFunctionBase
{
public:
	_str_ref		MENU;
	word			FileID;
	int				SpriteID;
	_str_ref		HName;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetRightPort);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,MENU,ALL_lvCDeffFilmMenu_ON_MAP);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_ENUM(_strindex,HName,ALL_vvTEXT_ON_MAP);		// Only for use in CossacsII interface!!!
	ENDSAVE;
};

class lvCPlayText : public be_CFunctionBase
{
public:
	_str_ref	MENU;
	int			canal;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCPlayText);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,MENU,ALL_lvCDeffFilmMenu_ON_MAP);
		REG_MEMBER(_int,canal);
	ENDSAVE;
};

class lvCSetText : public be_CFunctionBase
{
public:
	_str_ref		MENU;
	_str_ref		TEXT;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetText);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,MENU,ALL_lvCDeffFilmMenu_ON_MAP);
		REG_ENUM(_strindex,TEXT,ALL_vvTEXT_ON_MAP);
	ENDSAVE;
};

class lvCSetActivFrame : public be_CFunctionBase
{
public:
	_str_ref		MENU;
	int				STATE;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetActivFrame);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,MENU,ALL_lvCDeffFilmMenu_ON_MAP);
		REG_ENUM(_index,STATE,BE_FRAME_STATE);
	ENDSAVE;
};

class lvCRunTimer : public be_CFunctionBase
{
public:
	_str_ref		TimerID;
	int				Time;
	bool			UseVV;
	_str_ref		VVpInt;
	bool			TrTime;
public:
	virtual int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCRunTimer);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,TimerID,ALL_be_CINT_ON_MAP);
		REG_MEMBER(_bool,TrTime);
		REG_MEMBER(_bool,UseVV);
		SAVE_SECTION(0x00010000);
			REG_MEMBER(_int,Time);
		SAVE_SECTION(0x00020000);
			REG_ENUM(_strindex,VVpInt,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCSetGameMode : public be_CFunctionBase
{
public:
	int		ModeST;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetGameMode);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,ModeST);
	ENDSAVE;
};

class lvCSetCamera : public be_CFunctionBase
{
public:
	_str_ref		camera;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCSetCamera);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,camera,ALL_be_CCAMERAS_ON_MAP);
	ENDSAVE;
};

class lvCScrollCamera : public be_CFunctionBase
{
public:
	_str_ref		DestPos;
	int				ScrollTime;
	bool			ScrollFromCurPos;
	_str_ref		StartPos;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCScrollCamera);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,DestPos,ALL_be_CNODES_ON_MAP);
		REG_MEMBER(_int,ScrollTime);
		REG_MEMBER(_bool,ScrollFromCurPos);
		REG_ENUM(_strindex,StartPos,ALL_be_CNODES_ON_MAP);
	ENDSAVE;
};

class lvCMoveCamera : public be_CFunctionBase
{
public:
	_str_ref		POS0;
	_str_ref		POS1;
	_str_ref		DIR0;
	_str_ref		DIR1;
	bool			useMapXY;
	int				Time;
public:
	virtual int		CallFunction();
	int				MoveType();
public:
	SAVE(lvCMoveCamera);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,POS0,ALL_vvVector3D_ON_MAP);
		REG_ENUM(_strindex,POS1,ALL_vvVector3D_ON_MAP);
		REG_ENUM(_strindex,DIR0,ALL_vvVector3D_ON_MAP);
		REG_ENUM(_strindex,DIR1,ALL_vvVector3D_ON_MAP);
		REG_MEMBER(_int,Time);
		REG_MEMBER(_bool,useMapXY);
	ENDSAVE;
};

class lvCAttachCameraToGroup : public be_CFunctionBase
{
public:
	_str_ref		vGrpID;		// Двигаться за группой
public:
	virtual int		CallFunction();
public:
	SAVE(lvCAttachCameraToGroup);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCFreeCamera : public be_CFunctionBase
{
public:
	virtual int		CallFunction();
public:
	SAVE(lvCFreeCamera);
		REG_BASE(be_CFunctionBase);
	ENDSAVE;
};

class lvCSetLMode : public be_CFunctionBase
{
public:
	_str_ref		vMode;
	int				iMode;
	bool			Use_VV;
public:
	virtual int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCSetLMode);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,Use_VV);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,vMode,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,iMode);
	ENDSAVE;
};

class lvCSetFogMode : public be_CFunctionBase
{
public:
	int				Mode;	// 0-нет, 1-есть.
public:	
	virtual int		CallFunction();
public:
	SAVE(lvCSetFogMode);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Mode);
	ENDSAVE;
};

class lvCSaveScreenPos : public be_CFunctionBase
{
public:
	_str_ref	vCameraPos;
	_str_ref	vCameraDir;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCSaveScreenPos);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vCameraPos,ALL_vvVector3D_ON_MAP);
		REG_ENUM(_strindex,vCameraDir,ALL_vvVector3D_ON_MAP);
	ENDSAVE;
};

class lvCFilmCopliteState : public be_CFunctionBase
{
public:
	ClassRef<lvCFilm>	Film;
	bool				Complite;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCFilmCopliteState);
		REG_BASE(be_CFunctionBase);
		//REG_ENUM(_strindex,Film,ALL_lvCFilm_ON_MAP);
		REG_MEMBER(_bool,Complite);
	ENDSAVE;
};

//=================  GLOBAL APPLYNING FUNCTION  ========================//
class lvCSetGameSpeed : public be_CFunctionBase
{
public:
	bool			Use_VV;
	int				iSpeed;
	_str_ref		vSpeed;
public:
	virtual	int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCSetGameSpeed);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,Use_VV);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,vSpeed,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,iSpeed);
	ENDSAVE;
};

class lvCSetGameType : public be_CFunctionBase
{
public:
	_str_ref		vType;
	int				iType;
	bool			Use_VV;
public:
	virtual int		CallFunction();
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCSetGameType);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,Use_VV);
		SAVE_SECTION(0x00010000);
			REG_ENUM(_strindex,vType,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,iType);
	ENDSAVE;
};

class lvCGetGameSpeed : public be_CFunctionBase
{
public:
	_str_ref		vSpeed;
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCGetGameSpeed);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vSpeed,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCShowPanel : public be_CFunctionBase
{
public:
	_str_ref	TextID;
	int			OpenTime;
	int			ShowTime;
	int			CloseTime;
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCShowPanel);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,TextID,ALL_vvTEXT_ON_MAP);
		REG_MEMBER(_int,OpenTime);
		REG_MEMBER(_int,ShowTime);
		REG_MEMBER(_int,CloseTime);
	ENDSAVE;
};

class lvCQuestData : public be_CFunctionBase
{
public:
	_str_ref	Data;

	int			QuestN;					// Кол-во квестов на карте
	int			QuestTotalExperience;	// Экспириенс за все квесты
	int			KillsTotalExperience;	// Полный экспириенс за убитых
	int			TimeEverage;			// Среднее время прохождения миссии
	int			TimeEverageExperience;	// Экспа за среднее время прохождения мисии

	int			QuestComleteExperience;	// Полученный экспириенс

	int			OperType;				// Set new data, add comlete quest, save data
public:
	virtual	DWORD			GetClassMask();
	virtual			int		CallFunction();
public:
	SAVE(lvCQuestData);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,Data,ALL_vvMissionLOG_ON_MAP);
		REG_ENUM(_index,OperType,BE_QUEST_COM_TYPE);
		SAVE_SECTION(0x00010000);
			REG_MEMBER(_int,QuestN);
			REG_MEMBER(_int,QuestTotalExperience);
			REG_MEMBER(_int,KillsTotalExperience);
			REG_MEMBER(_int,TimeEverage);
			REG_MEMBER(_int,TimeEverageExperience);
		SAVE_SECTION(0x00020000);
			REG_MEMBER(_int,QuestComleteExperience);
		SAVE_SECTION(0x00040000);
	ENDSAVE;
};

class lvCPAUSE : public be_CFunctionBase
{
public:
	bool			state;
public:
	virtual int		CallFunction();
public:
	SAVE(lvCPAUSE);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,state);
	ENDSAVE;
};
class lvCSetSilence : public be_CFunctionBase
{
public:
	bool			State;
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCSetSilence);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,State);
	ENDSAVE;
};

//========================  COSSAKS II  ================================//
class lvCShowMessageII : public be_CFunctionBase
{
public:
	bool				autoHideElse;
	word				FileID;			// Файл с картинкой (0xFFFF - ничего нет)
	int					SpriteID;		// Индекс картинки	(-1 - ничего нет)
	_str				TextID;
	be_CParamList		paramList;
public:	
	virtual	int		CallFunction();
	bool	CheckState();
	void	PrepareString(_str& FullString);
public:
	SAVE(lvCShowMessageII);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,autoHideElse);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_AUTO(TextID);
		REG_AUTO(paramList);
	ENDSAVE;
};

extern	struct cvs_BrigPanel;
class lvCBrigPanelSet : public be_CFunctionBase
{
public:
	virtual	int		CallFunction();
	void	ApplyParams(cvs_BrigPanel& BP);
public:
	SubSection		Settings;
	bool			Bayonet;		
	bool			Rifle;
	bool			Grenade;
	bool			Formation;
	bool			Disband;
	bool			Fill;
	bool			Stop;
public:	
	SAVE(lvCBrigPanelSet);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,Bayonet);
		REG_MEMBER(_bool,Rifle);
		REG_MEMBER(_bool,Grenade);
		REG_MEMBER(_bool,Formation);
		REG_MEMBER(_bool,Disband);
		REG_MEMBER(_bool,Fill);
		REG_MEMBER(_bool,Stop);
	ENDSAVE;
};

class lvCGroupHoldNode : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
	_str_ref	parNode;
	_str		RulesFile;
public:
	virtual			int		CallFunction();
public:
	SAVE(lvCGroupHoldNode);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_FILEPATH(RulesFile,".sia");
	ENDSAVE;
};

class lvCArtChangeCharge : public be_CFunctionBase
{
/*
Change curent charge for artilery, 0/1 - variants
*/
public:
	_str_ref		vGrp;
	int				Charge;
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCArtChangeCharge);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,Charge);
	ENDSAVE;
};

class lvCArtAttackPoint : public be_CFunctionBase
{
/*
Attack point by artirely
*/
public:
	_str_ref		vGrp;
	_str_ref		parNode;
	int				NTimes;
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCArtAttackPoint);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_MEMBER(_int,NTimes);
	ENDSAVE;
};
//========================  STATE FUNC  ================================//
class lvCSUBF_AddAbility : public be_CFunctionBase
{
public:
	_str_ref		refGROUP;	// Group for aplay ability from SUBF ability list
	_str			nameSUBF;	// SUB FUNCTION name
	_str			nameABIL;	// Ability Name
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCSUBF_AddAbility);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,refGROUP,ALL_be_CGROUPS_ON_MAP);
		REG_AUTO(nameSUBF);
		REG_AUTO(nameABIL);
	ENDSAVE;
};
//========================  STATE FUNC  ================================//
class lvCGetValue : public be_CFunctionBase
{
public:
	int			TypeID;
	_str_ref	Value;
public:
	virtual		int		CallFunction();
public:
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCGetValue);
		REG_BASE(be_CFunctionBase);
		SAVE_SECTION(0x00000001);
			REG_ENUM(_strindex,Value,ALL_vVALUES_ON_MAP);
		SAVE_SECTION(0x00000002);
			REG_ENUM(_index,TypeID,BE_VALUE_TYPE);
			SAVE_SECTION(0x00000010);
				REG_ENUM(_strindex,Value,ALL_be_CBOOL_ON_MAP);
			SAVE_SECTION(0x00000020);
				REG_ENUM(_strindex,Value,ALL_be_CDWORD_ON_MAP);
			SAVE_SECTION(0x00000040);
				REG_ENUM(_strindex,Value,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCBool	: public be_CFunctionBase
{
public:
	bool			lvB;
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCBool);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,lvB);
	ENDSAVE;
};

class lvCInt	: public be_CFunctionBase
{
public:
	int				lvI;
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCInt);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,lvI);
	ENDSAVE;
};

class lvCGetScreenXY : public be_CFunctionBase
{
public:
	_str_ref	sX;
	_str_ref	sY;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetScreenXY);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,sX,ALL_be_CINT_ON_MAP);
		REG_ENUM(_strindex,sY,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCChkTime	: public be_CFunctionBase
{
public:
	int			timeOt;
	int			timeDo;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCChkTime);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,timeOt);
		REG_MEMBER(_int,timeDo);
	ENDSAVE;
};

class lvCGetAmount : public be_CFunctionBase
{
public:
	int			parNat;
	bool		Buildings;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetAmount);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_MEMBER(_bool,Buildings);
	ENDSAVE;
};

class lvCGetUnitsAmount0 : public be_CFunctionBase
{
public:
	_str_ref	parNode;
	int			parNat;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetUnitsAmount0);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
	ENDSAVE;
};

class lvCGetUnitsAmount1 : public be_CFunctionBase
{
public:
	_str_ref	parNode;
	_str_ref	parGrp;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetUnitsAmount1);
		REG_BASE(be_CFunctionBase);	
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCGetUnitsAmountPar : public be_CFunctionBase
{
public:
	_str_ref	parNode;
	_str		arrayName;
	_str_ref	parInd;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetUnitsAmountPar);
		REG_BASE(be_CFunctionBase);	
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_AUTO(arrayName);
		REG_ENUM(_strindex,parInd,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCGetUnitsAmount2 : public be_CFunctionBase
{
public:
	_str_ref	parNode;
	int			UnitType;
	int			parNat;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetUnitsAmount2);
		REG_BASE(be_CFunctionBase);	
		REG_MEMBER(_int,parNat);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
		REG_MEMBER(_UnitType,UnitType);
	ENDSAVE;
};

class lvCGetUnitsAmount3 : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
	int			parRad;
	int			parNat;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetUnitsAmount3);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,parRad);
	ENDSAVE;
};

class lvCGetTotalAmount0 : public be_CFunctionBase
{
public:
	_str_ref	parGrp;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetTotalAmount0);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCGetTotalAmount1 : public be_CFunctionBase
{
public:
	int			UnitType;
	int			parNat;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetTotalAmount1);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_MEMBER(_UnitType,UnitType);
	ENDSAVE;
};

class lvCGetTotalAmount2 : public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	int			UnitType;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetTotalAmount2);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_UnitType,UnitType);
	ENDSAVE;
};

class lvCGetReadyAmount : public be_CFunctionBase
{
public:
	int			UnitType;
	int			parNat;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetReadyAmount);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNat);
		REG_MEMBER(_UnitType,UnitType);
	ENDSAVE;
};

class lvCGetResource : public be_CFunctionBase
{
public:
	int			parNat;
	_str_ref	refNat;
	int			parID;
public:
	virtual	int		CallFunction();
public:
	virtual	DWORD	GetClassMask();
public:
	SAVE(lvCGetResource);
		REG_BASE(be_CFunctionBase);
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_int,parNat);
		SAVE_SECTION(0x00000002);
			REG_ENUM(_strindex,refNat,ALL_be_CINT_ON_MAP);
		SAVE_SECTION(0x00000003);
			REG_ENUM(_index,parID,RESTYPE);
	ENDSAVE;
};

class lvCGetDiff : public be_CFunctionBase
{
public:
	int			parNI;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetDiff);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,parNI);
	ENDSAVE;
};

class lvCProbably	: public be_CFunctionBase
{
public:
	int			parVer;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCProbably);
		REG_PARENT(be_CFunctionBase);
		REG_MEMBER(_int,parVer);
	ENDSAVE;
};

class lvCGetUnitState	: public be_CFunctionBase
{
public:
	_str_ref	parGrp;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetUnitState);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCTrigg : public be_CFunctionBase
{
public:
	int			TID;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCTrigg);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,TID);
	ENDSAVE;
};

class FG_Visible : public be_CFunctionBase
{
public:
	_str_ref	GraphObj;
public:
	virtual	int	CallFunction();
public:
	SAVE(FG_Visible);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GraphObj,ALL_lvCGraphObject_ON_MAP);
	ENDSAVE;
};

class FG_InVisible : public be_CFunctionBase
{
public:
	_str_ref	GraphObj;
public:
	virtual	int	CallFunction();
public:
	SAVE(FG_InVisible);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,GraphObj,ALL_lvCGraphObject_ON_MAP);
	ENDSAVE;
};

class ogSTOP : public be_CFunctionBase
{
public:
	int			canal;
public:
	virtual	int	CallFunction();
public:
	SAVE(ogSTOP);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,canal);
	ENDSAVE;
};

class lvCTimerDone : public be_CFunctionBase
{
public:
	_str_ref	TimerID;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCTimerDone);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,TimerID,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCGetTime : public be_CFunctionBase
{
public:
	_str_ref	TimerID;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetTime);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,TimerID,ALL_be_CINT_ON_MAP);
	ENDSAVE;
};

class lvCChekPosition : public be_CFunctionBase
{
public:
	_str_ref	parGrp;
	_str_ref	VVpPos;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCChekPosition);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,parGrp,ALL_be_CGROUPS_ON_MAP);
		REG_ENUM(_strindex,VVpPos,ALL_vvPOINT_SET_ON_MAP);
	ENDSAVE;
};

class lvCCameraSTOP : public be_CFunctionBase
{
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCCameraSTOP);
		REG_BASE(be_CFunctionBase);
	ENDSAVE;
};

class lvCNationIsErased : public be_CFunctionBase
{
public:
	int			Nat;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCNationIsErased);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
	ENDSAVE;
};

class lvCGetLMode : public be_CFunctionBase
{
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetLMode);
		REG_BASE(be_CFunctionBase);
	ENDSAVE;
};

class lvCCheckButton : public be_CFunctionBase
{
public:
	int			vkID;
	virtual	int	CallFunction();
public:
	SAVE(lvCCheckButton);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,vkID);
	ENDSAVE;
};

class lvCIsBrigade : public be_CFunctionBase
{
public:
	_str_ref	vgGrpID;
	bool		checkKOM;	// Проверить наличие командного состава.
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCIsBrigade);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vgGrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,checkKOM);
	ENDSAVE;
};

class lvCGetFormationType : public be_CFunctionBase
{
public:
	_str_ref	vgGrpID;
public:
	virtual	int	CallFunction();		// 0,1,2 - типы формаций, -1 - это не возможно узнать.
public:
	SAVE(lvCGetFormationType);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vgGrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCGetNofNewUnitInGrp : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
	bool		ClearNew;
public:
	virtual	int	CallFunction();
public:	
	SAVE(lvCGetNofNewUnitInGrp);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
	REG_MEMBER(_bool,ClearNew);
	ENDSAVE;
};

class lvCGetNofMyVillage : public be_CFunctionBase
{
public:
	int			Owner;
	int			ResType;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetNofMyVillage);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Owner);
		REG_ENUM(_index,ResType,RESTYPE);
	ENDSAVE;
};

class lvCCheckRBBP : public be_CFunctionBase // lvCCheckReadyBuildingBetwinPosition
{
public:
	_str_ref	P0;
	_str_ref	P1;
	int			MaxR;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCCheckRBBP);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,P0,ALL_vvPOINT2D_ON_MAP);
		REG_ENUM(_strindex,P1,ALL_vvPOINT2D_ON_MAP);
		REG_MEMBER(_int,MaxR);
	ENDSAVE;
};

class lvCIsTired : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCIsTired);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCBrigadesAmount : public be_CFunctionBase
{
public:
	int			Nat;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCBrigadesAmount);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
	ENDSAVE;
};

class lvCTestFillingAbility : public be_CFunctionBase
{
/*
Retern true if enabele to fill one or more object in group (for cannons,...)
*/
public:
	_str_ref	vGrp;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCTestFillingAbility);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCInStandGround : public be_CFunctionBase
{
/*
Return true if all brigades in group in stand ground state
*/
public:
	_str_ref	vGrp;
	bool		AllBrigades;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCInStandGround);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,AllBrigades);
	ENDSAVE;
};

class lvCVillageOwner : public be_CFunctionBase
{
public:
	_str		VillageName;	// Central group name for village
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCVillageOwner);
		REG_BASE(be_CFunctionBase);
		REG_AUTO(VillageName);
	ENDSAVE;
};

class lvCGetNofBrigInNode : public be_CFunctionBase
{
public:
	int			Nat;
	_str_ref	parNode;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetNofBrigInNode);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_int,Nat);
		REG_ENUM(_strindex,parNode,ALL_be_CNODES_ON_MAP);
	ENDSAVE;
};

class lvCGetNInside : public be_CFunctionBase
{
public:
	bool		Max;
	_str_ref	vGrpID;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetNInside);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_bool,Max);
	ENDSAVE;
};

class lvCCheckLeaveAbility : public be_CFunctionBase
{
public:
	_str_ref	vGrpID;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCCheckLeaveAbility);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,vGrpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCLoadingCoplite : public be_CFunctionBase
{
public:
	_str_ref	vGrpTransport;	// Транспорты
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCLoadingCoplite);
		REG_BASE(be_CFunctionBase);		
		REG_ENUM(_strindex,vGrpTransport,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCGetCurGrpORDER : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetCurGrpORDER);
		REG_BASE(be_CFunctionBase);		
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCGetNofBRLoadedGun : public be_CFunctionBase
{
public:
	_str_ref	vGrp;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCGetNofBRLoadedGun);
		REG_BASE(be_CFunctionBase);		
		REG_ENUM(_strindex,vGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};
class lvCSendMSG : public be_CFunctionBase
{
public:
	be_CMSG_FROMUSER	FSM_MSG;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCSendMSG);
		REG_BASE(be_CFunctionBase);		
		REG_AUTO(FSM_MSG);
	ENDSAVE;
};
class lvCSetInfo : public be_CFunctionBase
{
public:
    _str				TitleID;
	be_CParamList		titlePL;	// Addional parm for wartify Title string
	_str				DescrID;
	be_CParamList		descrPL;	// Addional parm for wartify Descr string
	_str				TaskID;
	be_CParamList		taskPL;		// Addional parm for wartify Task string
	_str				Name;		// Task Name for do not add again
public:	
	virtual	int		CallFunction();
			bool	PrepareString(_str& FullString);
public:
	SAVE(lvCSetInfo);
		REG_BASE(be_CFunctionBase);

		REG_AUTO(Name);

		REG_AUTO(TitleID);
		REG_AUTO(titlePL);

		REG_AUTO(DescrID);
		REG_AUTO(descrPL);

		REG_AUTO(TaskID);
		REG_AUTO(taskPL);
	ENDSAVE;
};
class lvCDelInfo : public be_CFunctionBase
{
public:
	_str				Name;		// Task Name for do not add again
public:	
	virtual	int		CallFunction();
public:
	SAVE(lvCDelInfo);
		REG_BASE(be_CFunctionBase);
		REG_AUTO(Name);
	ENDSAVE;
};
class lvCInfoVisibility : public be_CFunctionBase
{
public:
	_str_ref			state;
public:
	virtual	int		CallFunction();
public:
	SAVE(lvCInfoVisibility)
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,state,ALL_be_CBOOL_ON_MAP);
	ENDSAVE;
};
class lvCShowHint : public be_CFunctionBase
{
public:
	_str_ref		Time;
	_str			TextID;
	be_CParamList	paramList;
public:
	virtual	int		CallFunction();
			bool	PrepareString(_str& FullString);
public:
	SAVE(lvCShowHint);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,Time,ALL_be_CINT_ON_MAP);
		REG_AUTO(TextID);
		REG_AUTO(paramList);
	ENDSAVE;
};
//************************************************************************
//*****************	ABILITY FUNCTION SPESIAL ADDITION ********************
//************************************************************************
class lvCABIL_SetVisibility : public be_CFunctionBase
{
public:
	bool		Visible;
public:
	virtual	int	CallFunction();
public:
	SAVE(lvCABIL_SetVisibility);
		REG_BASE(be_CFunctionBase);
		REG_MEMBER(_bool,Visible);
	ENDSAVE;
};

class lcCABIL_SetHint : public be_CFunctionBase
{
public:
	_str_ref	HINT;
public:
	virtual	int	CallFunction();
public:
	SAVE(lcCABIL_SetHint);
		REG_BASE(be_CFunctionBase);
		REG_ENUM(_strindex,HINT,ALL_be_CSTR_ON_MAP);
	ENDSAVE;
};

//**********************************************************************//
//**********************************************************************//
class lvCBaseOperCond : public BaseClass
{
public:
	lvCBaseOperCond();
	lvCBaseOperCond(lvCBaseOperCond* BaseOperCond);
	~lvCBaseOperCond()	{};

	DWORD	InfID;
	_str	Descr;
	int		x0,y0;
	int		x1,y1;
	DWORD	squardID;
	DWORD	timeInProc;
	bool	use_vGroup;
	bool	first;

	// Use node instade zone
	bool	UseNode;
	int		parNode;

	virtual const	char*	GetThisElementView(const char* LocalName);

	virtual	void	Draw()	{};
	virtual	void	GetCopy(lvCBaseOperCond** pCopy);
	virtual	void	SetEdgeXY(int _x0,int _y0,int _x1,int _y1)	{ 
		x0 = _x0; y0 = _y0;
		x1 = _x1; y1 = _y1;
		first = true;
	};
	virtual	void	SetSquardID(DWORD sqID){
		squardID = sqID;
		first = true;
	};
	virtual	DWORD	GetClassMask(){
						if (g_CO_USER_FLAGS==0x00000010)	return 0x00000010;
						return	0x00000002;
					};

	virtual	int		Power()	{ return 0; };

	virtual int		Process	(int time)		{ return 0; };
	virtual	int		GetValue(int time)		{ return 0; };

	class CPRESENTATION	fShowPresentation;

	SAVE(lvCBaseOperCond);
		
	SAVE_SECTION(0x00000003);
		REG_AUTO(fShowPresentation);
		REG_MEMBER(_int,x0);
		REG_MEMBER(_int,y0);
		REG_MEMBER(_int,x1);
		REG_MEMBER(_int,y1);
		REG_MEMBER(_DWORD,squardID);
		REG_MEMBER(_DWORD,timeInProc);
    	REG_MEMBER(_bool,use_vGroup);
		REG_MEMBER(_bool,first);
		REG_MEMBER(_bool,UseNode);
		REG_ENUM(_index,parNode,ALL_be_CNODES_ON_MAP);

	ENDSAVE;
};

//**********************************************************************//
//*****************    OPERATION FOR SCRIPT		************************//
//**********************************************************************//
// lvCOperation //////////////////////////////////////////////////////////
class lvCOperation : public lvCBaseOperCond
{
public:
	lvCOperation();
	lvCOperation(lvCOperation* pOperation) : lvCBaseOperCond(dynamic_cast<lvCBaseOperCond*>(pOperation)) {};
	~lvCOperation()	{};
public:
	virtual	void	Draw()	{};
	virtual int		Process(int time);
	virtual	int		Complite()			{ return 1; };
	virtual	void	GetCopy(lvCOperation** pCopy);

	virtual	DWORD	GetClassMask(){
						return lvCBaseOperCond::GetClassMask();
					};

	bool			RepeatInTime;
	int				CurIter;
	void			AddIteration()		{ if (RepeatInTime) CurIter++; };
	void			CheckIteration();

	virtual char*	GetSourceCode(int shift = 0);

	// for pro mission filer
	int myID;				// -1

public:
	SAVE(lvCOperation);

	SAVE_SECTION(0x00000003);
		REG_PARENT(lvCBaseOperCond);
		REG_MEMBER(_bool,RepeatInTime);

	// For user usage
	SAVE_SECTION(0x00000010);
		REG_BASE(lvCBaseOperCond);	

	ENDSAVE;
};










class lvCSelSendToNode	: public lvCOperation
{
public:
	lvCSelSendToNode(lvCSelSendToNode* pSelSendToNode);
	lvCSelSendToNode()	{ InfID = _lvCSelSendToNode_; parDir=512; };
	~lvCSelSendToNode()	{};
public:
	int parNat;
	int	parDir;
	int	parType;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual int		Process(int time);
	virtual	void	GetCopy(lvCOperation** pCopy);
public:
	SAVE(lvCSelSendToNode);
		REG_PARENT(lvCOperation);
		REG_MEMBER(_int,parNat);
		REG_MEMBER(_int,parDir);
		REG_MEMBER(_int,parType);
	ENDSAVE;
};
class lvCGroupSendToNode	: public lvCOperation
{
public:
	lvCGroupSendToNode(lvCGroupSendToNode* pGroupSendToNode);
	lvCGroupSendToNode()	{ InfID = _lvCGroupSendToNode_; parDir=512; first=true; };
	~lvCGroupSendToNode()	{};
public:
	int	parGrp;
	int	parDir;
	int	parType;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual int		Process(int time);
	virtual	void	GetCopy(lvCOperation** pCopy);
public:
	SAVE(lvCGroupSendToNode);
			REG_PARENT(lvCOperation);
			REG_MEMBER(_int,parDir);
			REG_MEMBER(_int,parType);
		SAVE_SECTION(0x00000001);
			REG_ENUM(_index,parGrp,ALL_GROUPS_ON_MAP);
		SAVE_SECTION(0x00000002);
			REG_ENUM(_index,parGrp,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};

class lvCRotate	: public lvCOperation
{
public:
	lvCRotate(lvCRotate* pRotate);
	lvCRotate()	{ InfID = _lvCRotate_; parDir=512; first=true; };
	~lvCRotate()	{};
public:
	int	parDir;
	int	prior;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual int		Process(int time);
	virtual	int		Complite();
	virtual	void	GetCopy(lvCOperation** pCopy);

public:
	SAVE(lvCRotate);
		REG_PARENT(lvCOperation);
		REG_MEMBER(_int,parDir);
		REG_MEMBER(_int,prior);
	ENDSAVE;
};
class lvCSendToNode	: public lvCOperation
{
public:
	lvCSendToNode(lvCSendToNode* pSendToNode);
	lvCSendToNode()	{ InfID = _lvCSendToNode_; parDir=512; first=true; RepeatInTime=true; };
	~lvCSendToNode()	{};
public:
	int	parDir;
	int	parType;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual int		Process(int time);
	virtual	int		Complite();
	virtual	void	GetCopy(lvCOperation** pCopy);

public:
	SAVE(lvCSendToNode);
		REG_PARENT(lvCOperation);
		REG_MEMBER(_int,parDir);
		REG_MEMBER(_int,parType);
	ENDSAVE;
};


class lvCSetState	: public lvCOperation
{
public:
	lvCSetState(lvCSetState* pSetState);
	lvCSetState()	{ InfID = _lvCSetState_; parmode=0;};
	~lvCSetState()	{};
public:
	int		parmode;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual int		Process(int time);
	virtual	void	GetCopy(lvCOperation** pCopy);
public:
	SAVE(lvCSetState);
		REG_PARENT(lvCOperation);
		REG_ENUM(_index,parmode,BE_UNIT_MOVE_MODE);
	ENDSAVE;
};
class lvCReformation	: public lvCOperation
{
public:
	lvCReformation(lvCReformation* pReformation);
	lvCReformation()	{ InfID = _lvCReformation_; TypeForm=0; first=true; UseInNode=true; };
	~lvCReformation()	{};
public:
	int		TypeForm;
	bool	UseInNode;
	int		vGrp;
//	int		Dir;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual int		Process(int time);
	virtual	void	GetCopy(lvCOperation** pCopy);
public:
	SAVE(lvCReformation);
		REG_PARENT(lvCOperation);
		REG_MEMBER(_int,TypeForm);
		REG_MEMBER(_bool,UseInNode);
		REG_ENUM(_index,vGrp,ALL_be_CGROUPS_ON_MAP);
//		REG_MEMBER(_int,Dir);
	ENDSAVE;
};





class lvCChangeNation	: public lvCOperation
{
public:
	lvCChangeNation(lvCChangeNation* pChangeNation);
	lvCChangeNation()	{ InfID = _lvCChangeNation_; nwNat=0;};
	~lvCChangeNation()	{};
public:
	int		nwNat;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual int		Process(int time);
	virtual	void	GetCopy(lvCOperation** pCopy);
public:
	SAVE(lvCChangeNation);
		REG_PARENT(lvCOperation);
		REG_MEMBER(_int,nwNat);
	ENDSAVE;
};

class lvCTeraforming;
class lvCApplyTerafoming : public lvCOperation
{
public:
	lvCApplyTerafoming()	{ InfID=_lvCApplyTerafoming_; LastUpdateTime=0; xV=yV=rV=RV=hV=HV=0.f; x0=x1=y1=y0; };
	lvCApplyTerafoming(lvCApplyTerafoming* pApplyTerafoming);
	~lvCApplyTerafoming()	{};

	bool									visible;
	DWORD									color;
	ClonesArray< ClassRef<lvCTeraforming> >	TerraPointsArr;
	int										MinStep;
	int										MinFromDest;

	bool									Use_VV;
	int										Speed;		// Pixels per sec (1pixel==1000) by max distance
	ClassRef<be_CINT>						vSpeed;		// Pixels per sec (1pixel==1000) by max distance
	ClassRef<be_CINT>						vNOfComplitePoints;	// Кол-во пройденных точек.

	int	x0,y0,x1,y1;									// For update all ways

	DWORD									LastUpdateTime;
	float									xV;
	float									yV;
	float									rV;
	float									RV;
	float									hV;
	float									HV;

	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		Process(int time);
	virtual			void	GetCopy(lvCOperation** pCopy);
	virtual			void	Draw();
	virtual			void	SetSpeed(int _speed);

					bool			TestIndexPos			(int index);
					lvCTeraforming*	GetPossition			(int index);
					float			Get_Speed				();
					float			Get_DT					(int p0=0,int p1=1);
					float			Get_DS					(int p0=0,int p1=1);
					float			Get_Dx					(int p0=0,int p1=1);
					float			Get_Dy					(int p0=0,int p1=1);
					float			Get_Dr					(int p0=0,int p1=1);
					float			Get_DR					(int p0=0,int p1=1);
					float			Get_Dh					(int p0=0,int p1=1);
					float			Get_DH					(int p0=0,int p1=1);
					float			Get_MaxShift			(float dt,int p0=0,int p1=1);
					float			Get_MaxDist				(int p0=0,int p1=1);
					void			Set_xV					(int p0=0,int p1=1);
					void			Set_yV					(int p0=0,int p1=1);
					void			Set_rV					(int p0=0,int p1=1);
					void			Set_RV					(int p0=0,int p1=1);
					void			Set_hV					(int p0=0,int p1=1);
					void			Set_HV					(int p0=0,int p1=1);

	SAVE(lvCApplyTerafoming);
		REG_PARENT(lvCOperation);
		REG_MEMBER(_bool,visible);
		REG_MEMBER(_color,color);
		REG_AUTO(TerraPointsArr);
		REG_MEMBER(_int,MinStep);
		REG_MEMBER(_int,MinFromDest);
		REG_MEMBER(_bool,Use_VV);
		REG_MEMBER(_int,Speed);
		REG_AUTO(vSpeed);
		REG_MEMBER(_int,x0);
		REG_MEMBER(_int,x1);
		REG_MEMBER(_int,y0);
		REG_MEMBER(_int,y1);
		REG_AUTO(vNOfComplitePoints);
	ENDSAVE;
};

//=================		OPERATION FOR FILM		========================//
class lvCSetMessageState : public be_CFunctionBase
{
public:
	ClassRef<CSingleMessage>	MESSAGE;
	ClassRef<CPhraseChunk>		TALK_LST;
	ClassRef<COneMissHint>		ONE_HINT;
public:
	bool		Visible;		//Текс отображается.
	bool		Deleted;		//Текст зачеркнут.

	bool		TaskListVisible;

	int			MessType;		// Тип сообщения 0-Task, 1-Talk List, 2-Hint List.
public:
	virtual			int		CallFunction();
	virtual			DWORD	GetClassMask();
public:
	SAVE(lvCSetMessageState);
		REG_BASE(be_CFunctionBase);

		REG_ENUM(_index,MessType,BE_MESSAGE_TYPE_EDITE);

		SAVE_SECTION(0x00010000);		// TASK
			REG_AUTO(MESSAGE);
			REG_MEMBER(_bool,Visible);
			REG_MEMBER(_bool,Deleted);

		SAVE_SECTION(0x00020000);		// TALK LIST
			REG_AUTO(TALK_LST);
			REG_MEMBER(_bool,Visible);

		SAVE_SECTION(0x00040000);		// HINT LIST
			REG_AUTO(ONE_HINT);
			REG_MEMBER(_bool,Visible);

		SAVE_SECTION(0x00080000)
			REG_MEMBER(_bool,TaskListVisible);
		
	ENDSAVE;
};

//=================  GLOBAL APPLYNING FUNCTION  ========================//
class lvCReStartSquadShema : public lvCOperation
{
public:
	lvCReStartSquadShema()	{ InfID=_lvCReStartSquadShema_; NodeID=-1; };
	lvCReStartSquadShema(lvCReStartSquadShema* pReStartSquadShema);
	~lvCReStartSquadShema()	{};

	int	vGroup;
	int NodeID;	// -1 if not used

	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		Process(int time);
	virtual			void	GetCopy(lvCOperation** pCopy);

	SAVE(lvCReStartSquadShema);
		REG_PARENT(lvCOperation);
		REG_ENUM(_index,vGroup,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,NodeID);
	ENDSAVE;
};
//========================  COSSAKS II  ================================//
class lvCAddElemTHE_CII : public lvCOperation
{
public:
	lvCAddElemTHE_CII();
	lvCAddElemTHE_CII(lvCAddElemTHE_CII* pAddElemTHE_CII);
	~lvCAddElemTHE_CII(){};

	ClassRef<vvTASKS_CII>	OBJECT;

	SubSection	Task;
	bool		add_TASK;
	bool		TASK_Dublicate;
	int			TASK_POS;
	_ClassIndex TASK;
	
	SubSection	Hint;
	bool		add_HINT;
	bool		HINT_Dublicate;
	int			HINT_POS;
	_ClassIndex HINT;

	SubSection	Else;
	bool		add_ELSE;
	bool		ELSE_Dublicate;
	int			ELSE_POS;
	_ClassIndex ELSE;

	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		Process(int time);
	virtual			void	GetCopy(lvCOperation** pCopy);

	SAVE(lvCAddElemTHE_CII);
		
		SAVE_SECTION(0x00000003);
			REG_PARENT(lvCOperation);
			REG_AUTO(OBJECT);
			REG_AUTO(Task);
				REG_MEMBER(_bool,add_TASK);
				REG_MEMBER(_bool,TASK_Dublicate);
				REG_MEMBER(_int,TASK_POS);
				REG_AUTO2(TASK,"OBJECT^REF/TASK Name");
			REG_AUTO(Hint);
				REG_MEMBER(_bool,add_HINT);
				REG_MEMBER(_bool,HINT_Dublicate);
				REG_MEMBER(_int,HINT_POS);
				REG_AUTO2(HINT,"OBJECT^REF/HINT Name");
			REG_AUTO(Else);
				REG_MEMBER(_bool,add_ELSE);
				REG_MEMBER(_bool,ELSE_Dublicate);
				REG_MEMBER(_int,ELSE_POS);
				REG_AUTO2(ELSE,"OBJECT^REF/ELSE Name");

		// For user usage
		SAVE_SECTION(0x00000010);
			REG_BASE(lvCOperation);
			REG_AUTO(OBJECT);
			REG_AUTO(Task);
				REG_MEMBER(_bool,add_TASK);
				REG_MEMBER(_bool,TASK_Dublicate);
				REG_MEMBER(_int,TASK_POS);
				REG_AUTO2(TASK,"OBJECT^REF/TASK Name");
			REG_AUTO(Hint);
				REG_MEMBER(_bool,add_HINT);
				REG_MEMBER(_bool,HINT_Dublicate);
				REG_MEMBER(_int,HINT_POS);
				REG_AUTO2(HINT,"OBJECT^REF/HINT Name");
			REG_AUTO(Else);
				REG_MEMBER(_bool,add_ELSE);
				REG_MEMBER(_bool,ELSE_Dublicate);
				REG_MEMBER(_int,ELSE_POS);
				REG_AUTO2(ELSE,"OBJECT^REF/ELSE Name");

	ENDSAVE;
};
class lvCDelElemTHE_CII : public lvCOperation
{
public:
	lvCDelElemTHE_CII(){ InfID=_lvCDelElemTHE_CII_; FULL_DELETE=false; TASK_USE=HINT_USE=ELSE_USE=false; };
	lvCDelElemTHE_CII(lvCDelElemTHE_CII* pDelElemTHE_CII);
	~lvCDelElemTHE_CII(){};

	ClassRef<vvTASKS_CII>	OBJECT;

	SubSection	TASK_S;
	bool		TASK_USE;
	_ClassIndex TASK;
	bool		FULL_DELETE;

	SubSection	HINT_S;
	bool		HINT_USE;
	_ClassIndex HINT;

	SubSection	ELSE_S;
	bool		ELSE_USE;
	_ClassIndex ELSE;

	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		Process(int time);
	virtual			void	GetCopy(lvCOperation** pCopy);

	SAVE(lvCDelElemTHE_CII);

		SAVE_SECTION(0x00000003);
			REG_PARENT(lvCOperation);
			REG_AUTO(OBJECT);

			REG_AUTO(TASK_S);
				REG_MEMBER(_bool,TASK_USE);
				REG_AUTO2(TASK,"OBJECT^REF/TASK Name");
				REG_MEMBER(_bool,FULL_DELETE);

			REG_AUTO(HINT_S);
				REG_MEMBER(_bool,HINT_USE);
				REG_AUTO2(HINT,"OBJECT^REF/HINT Name");

			REG_AUTO(ELSE_S);
				REG_MEMBER(_bool,ELSE_USE);
				REG_AUTO2(ELSE,"OBJECT^REF/ELSE Name");

		// For user usage
		SAVE_SECTION(0x00000010);
			REG_BASE(lvCOperation);
			REG_AUTO(OBJECT);
			REG_AUTO(TASK_S);
				REG_AUTO2(TASK,"OBJECT^REF/TASK Name");
				REG_MEMBER(_bool,TASK_USE);
				REG_MEMBER(_bool,FULL_DELETE);
			REG_AUTO(HINT_S);
				REG_AUTO2(HINT,"OBJECT^REF/HINT Name");
				REG_MEMBER(_bool,HINT_USE);
			REG_AUTO(ELSE_S);
				REG_AUTO2(ELSE,"OBJECT^REF/ELSE Name");
				REG_MEMBER(_bool,ELSE_USE);

	ENDSAVE;
};
class lvCSET_MISS_MANAGER : public lvCOperation
{
public:
	lvCSET_MISS_MANAGER(){InfID=_lvCSET_MISS_MANAGER_; PAUSE_Animate=RESTART_Animate=NEXT_Animate=false;};
	lvCSET_MISS_MANAGER(lvCSET_MISS_MANAGER* pSET_MISS_MANAGER);
	~lvCSET_MISS_MANAGER(){};

	ClassRef<vvMISSMGR>	MISS_MANAGER;
	bool	PAUSE,		PAUSE_Animate;
	bool	RESTART,	RESTART_Animate;
	bool	NEXT,		NEXT_Animate;

//	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		Process(int time);
	virtual			void	GetCopy(lvCOperation** pCopy);

	SAVE(lvCSET_MISS_MANAGER);

		SAVE_SECTION(0x00000003);
			REG_PARENT(lvCOperation);
			REG_AUTO(MISS_MANAGER);
			REG_MEMBER(_bool,PAUSE);
			REG_MEMBER(_bool,PAUSE_Animate);
			REG_MEMBER(_bool,RESTART);
			REG_MEMBER(_bool,RESTART_Animate);
			REG_MEMBER(_bool,NEXT);
			REG_MEMBER(_bool,NEXT_Animate);

		// For user usage
		SAVE_SECTION(0x00000010);
			REG_BASE(lvCOperation);
			REG_AUTO(MISS_MANAGER);
			REG_MEMBER(_bool,PAUSE);
			REG_MEMBER(_bool,PAUSE_Animate);
			REG_MEMBER(_bool,RESTART);
			REG_MEMBER(_bool,RESTART_Animate);
			REG_MEMBER(_bool,NEXT);
			REG_MEMBER(_bool,NEXT_Animate);

	ENDSAVE;
};
//**********************************************************************//
//*****************    FUNCTION FOR CONDITION	************************//
//**********************************************************************//
class lvCBaseFunction	: public lvCBaseOperCond
{
public:
	lvCBaseFunction();
	lvCBaseFunction(lvCBaseFunction* pBaseFunction);
	~lvCBaseFunction()		{};
public:
	virtual	void	GetCopy(lvCBaseFunction** pCopy);
	virtual	int		GetValue(int time)		{ return 0; };
	virtual	int		Power()					{ return 0; };
	virtual	DWORD	GetClassMask()			{ return lvCBaseOperCond::GetClassMask(); };
	virtual bool ForceSimplification()		{ return true; }

public:
	SAVE(lvCBaseFunction);

		SAVE_SECTION(0x00000003);
			REG_PARENT(lvCBaseOperCond);
		
		// For user usage
		SAVE_SECTION(0x00000010);
			REG_BASE(lvCBaseOperCond);	

	ENDSAVE;
};
class lvCGrpInNode : public lvCBaseFunction
{
public:
	lvCGrpInNode(lvCGrpInNode* pGrpInNode);
	lvCGrpInNode() { InfID = _lvCGrpInNode_; };
	~lvCGrpInNode(){};
public:
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		GetValue(int time);
	virtual			void	GetCopy(lvCBaseFunction** pCopy);
public:
	SAVE(lvCGrpInNode);
		REG_PARENT(lvCBaseFunction);
	ENDSAVE;
};
class lvCGrpInNodeFree : public lvCBaseFunction
{
public:
	lvCGrpInNodeFree(lvCGrpInNodeFree* pGrpInNodeFree);
	lvCGrpInNodeFree() { InfID = _lvCGrpInNodeFree_; };
	~lvCGrpInNodeFree(){};
public:
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		GetValue(int time);
	virtual			void	GetCopy(lvCBaseFunction** pCopy);
public:
	SAVE(lvCGrpInNodeFree);
		REG_PARENT(lvCBaseFunction);
	ENDSAVE;
};
class lvCAllGrpInNode : public lvCBaseFunction
{
public:
	lvCAllGrpInNode(lvCAllGrpInNode* pAllGrpInNode);
	lvCAllGrpInNode() { InfID = _lvCAllGrpInNode_; };
	~lvCAllGrpInNode(){};
public:
	int		grpID;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		GetValue(int time);
	virtual			void	GetCopy(lvCBaseFunction** pCopy);
public:
	SAVE(lvCAllGrpInNode);
		REG_PARENT(lvCBaseFunction);
		SAVE_SECTION(0x00000001);
			REG_ENUM(_index,grpID,ALL_GROUPS_ON_MAP);
		SAVE_SECTION(0x00000002);
			REG_ENUM(_index,grpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};
class lvCAllGrpInNodeFree : public lvCBaseFunction
{
public:
	lvCAllGrpInNodeFree(lvCAllGrpInNodeFree* pAllGrpInNodeFree);
	lvCAllGrpInNodeFree() { InfID = _lvCAllGrpInNodeFree_; };
	~lvCAllGrpInNodeFree(){};
public:
	int		grpID;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		GetValue(int time);
	virtual			void	GetCopy(lvCBaseFunction** pCopy);
public:
	SAVE(lvCAllGrpInNodeFree);
		REG_PARENT(lvCBaseFunction);
		SAVE_SECTION(0x00000001);
			REG_ENUM(_index,grpID,ALL_GROUPS_ON_MAP);
		SAVE_SECTION(0x00000002);
			REG_ENUM(_index,grpID,ALL_be_CGROUPS_ON_MAP);
	ENDSAVE;
};
//////////////////////////////////////////////////////////////////////////
void REG_BE_FUNCTIONS_class();
//////////////////////////////////////////////////////////////////////////
#endif//__BE_FUNCTIONS__































