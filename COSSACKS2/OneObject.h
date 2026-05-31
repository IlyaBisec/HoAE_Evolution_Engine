
#if !defined(ONEOBJECT_H___INCLUDED_)
#define ONEOBJECT_H___INCLUDED_

#include "StructuresExtensions.h"
#include ".\cvi_alert.h"

class Nation;
class OneObjectEx;
class SelGroup;
class FireInfo;
class GOrder;
struct Coor3D;

typedef enum {ls_LiveBody, ls_DieAnimation, ls_DeadBody, ls_Bones, ls_Erased} eLiveState;

class DIALOGS_API OneObject{
public:
	~OneObject() { if (pGRP_IDS!=NULL){ delete pGRP_IDS; pGRP_IDS=NULL; }; GRP_N=0; }; 

	Nation* Nat;
	union{
		GeneralObject* General;
		Visuals* Visual;
		ObjectUpgrade* OUpgrade;
		ParameterUpgrade* PUpgrade;
	} Ref;
	NewMonster*   newMons;
	AdvCharacter* MoreCharacter;
	//-----------new path algoritm variables----------------
    Order1* LocalOrder;
    NewAnimation* HiLayer;
    NewAnimation* NewAnm;
    NewAnimation* LoLayer;

	short* PathX;
	short* PathY;
	word Index;//номер этого монстра
	//--------------------------------------------//
	//-----------begin of saving cluster----------//
	//--------------------------------------------//
    int  RealX;//1 pixel=16 length units
    int  RealY;    
    int  DestX;
    int  DestY;

    word Sdoxlo;
    word SdoxloLow;        

    bool WasDrawn:1;
    bool IsInFog:1;
    bool Attack:1;
    bool UnlimitedMotion:1;
    bool Excited:1;//always searches victim
    bool Hidden:1;    
    bool NoCollision:1;
    bool AI_Guard:1;	

    bool NotSelectable:1;
    bool FrameFinished:1;
    bool NewBuilding:1;    
    bool Borg:1;	
    bool TempFlag:1;//временный флаг, используемый для сортировки отряда	
    bool Ready:1;
    bool NoSearchVictim:1;
    bool SafeWall:1;

    bool StandGround:1;	
    bool Zombi:1;
    bool Transport:1;
    bool InFire:1;
    bool NewMonst:1;
    bool InMotion:1;//Выполняется перемещениe(New)
    bool BackMotion:1;//шаг назад		
    bool Wall:1;

    byte  CollisionRadius;   

    word Serial;
    word NIndex;//индекс в национальном списке
    short NewCurSprite;
    short NewCurSpritePrev;

    int  CurrentFrameLong;    

    byte NNUM;
    byte NewState;
    byte LocalNewState;
    byte LockType;//0-Land,1-Water

    byte Usage;    
    byte NMask;    
    char Lx;
    byte PathDelay;    

	int CleanDamage;
    word Life;
    word MaxLife;
    word Mana;
    word MaxMana;    

    int  x;
    int  y;
    int  DstX;
    int  DstY;

    word delay;    
    word MaxDelay;    
    short NStages;
    word Stage;    

    word EnemyID;
    word EnemySN;    
    word BrigadeID;
    word BrIndex;   

    word CObjIndex;
    word CObjOwner;
    word RealDirPrecise;
    short RZ;

    short OverEarth;
    word Kills;
    short ObjectScale;//256 = scale(1.0)
    word NInside;

    int TotalPath;

    byte StandTime;
    char AddDamage;
    char AddShield;
    byte Selected;

    byte ImSelected;
    byte MathMask;
    byte MotionStyle;
    byte OctantInfo;

    byte RealDir;
    byte GraphDir;
    byte PrioryLevel;    
    byte GroundState;
    
    byte UnitSpeed;
    byte CurUnitSpeed;
    byte GroupSpeed;
    byte ActivityState;//0-normal, 1-peacefull, 2-aggressive

	word CPdestX;
	word CPdestY;
	word NIPoints;	
	word BirthNIndex;

    short Phase;    

    bool Mobilised:1;
    bool LLock:1;
    bool GLock:1;
    bool InternalLock:1;
    bool TurnOff:1;
    bool InArmy:1;
    bool InPatrol:1;
    bool FriendlyFire:1;

	bool ArmAttack:1;		
    bool UsesDestDidection:1;
    bool DestDirectionIsActive:1;
	bool NoBuilder:1;
	bool DoNotCall:1;
	bool AutoKill:1;    
    bool Invisible:1;
	bool InvisibleForEnemy:1;
    
	word Guard;		
	word LockedInBuilding;
	byte NZalp;
	byte FiringStage;
	
	char VeraVPobedu;	
	byte NothingTime;
	word BlockInfo;	
	word KineticPower;
	byte NMisses;//skolko raz etot unit promaxnetsa pri udare
    DWORD AttachedEffectMask;
	word AbRes;
	short WallX;
	short WallY;

	word NUstages;
	word Ustage;	
	byte Kind;	
	
	byte RStage;
	byte RType;
	byte RAmount;
	word NearBase;
	
	//byte Media;//=0-terrain,=1-on water,=2-on air
	word AddInside;
	byte PersonalDelay;
	
	word StageState;//if have stages 32768+3 bits/stage, if no then 0
	int* TempDelay;	
	int  RealVx;
	int  RealVy;	
	//int	 Height;
	short  ForceX;
	short  ForceY;
	short  Radius1;
	short  Radius2;
	short  Speed;

    byte DestDirection;	
	byte SingleUpgLevel;	
	
	byte MotionDir;	
	
	int Morale;
	int MaxMorale;
	int GetTired;
	word SearchOnlyThisBrigadeToKill;
	//word			AnimFrame3D;
	
    bool RifleAttack:1;
	bool StopBuildMode:1;
	bool WatchNInside:1;	
	bool AutoZasev:1;
	bool Illusion:1;
	bool NotSavable:1;		
	bool CantRestrictMotion:1;
	bool Disabled:1;

    bool notSelectable();	

    void SetZeroFrame();
    void SetNextFrame();
    void SetNextFrameCycled();
	
	byte AddFarms;
	byte FireOwner;	
	
	OneObjectExtension Ext;
	
	DWORD UniqID;

	// поля характеризующие дополнительные свойства обьекта
	word Stuff;	// - указатель на список параметров
    

	//
	//-----------------------------------------------//
	//-------------end of saving cluster-------------//
	//-----------------------------------------------//
	//Octant* CurAnm;				// Текущая выполняемая анимация
	word* Inside;


	//for all objects
	//Legion* Wars;
	//byte bx;
	//byte by;
	SelGroup* GroupIndex;
	FireInfo* IFire;
	//Указатель на текущий приказ 1-го уровня
	//Типы приказов:
	//	Перейти(x,y)->(x1,y1) по оптимальному пути;
	//	Атаковать объект(obj_index)- удаленный или близкий
	//	Умереть
	//	Создать экземпляр объекта(obj)...
	
	GOrder* GlobalOrder;
	//Weapon* Weap;
	//New animation settings
    
	bool SoundWasEmit;
	byte HighlightMask;//bits of highliting : 1 - RED, 2 - GREEN, 4 - BLUE 8 - blinking effect


	int GetAttackSpeedBonus();
	int GetMaxAttackRadius(int AttType);
	int GetMaxAttackRadius();

	//Methods declaration
	void DefaultSettings(GeneralObject* GO);
	void NewMonsterSendTo(int x,int y,byte Prio,byte OrdType,OneObject* Enemy,int NSteps);
	void NewMonsterSendTo(int x,int y,byte Prio,byte OrdType);
	void NewMonsterPreciseSendTo(int x,int y,byte Prio,byte OrdType);
	void NewMonsterPreciseSendTo3D(int x,int y,int z0,int z1,byte Prio,byte OrdType);
	void NewMonsterSmartSendTo(int x,int y,int dx,int dy,byte Prio,byte OrdType);
	void MakeProcess();
	void MakePreProcess();
	void WSendTo(int x2,int y2,int Prio);
	//void AttackObj(word OID,int Prio,byte OrdType);
	bool AttackObj(word OID,int Prio);
	bool AttackObj(word OID,int Prio,byte OrdType);
	bool AttackObj(word OID,int Prio,byte OrdType,word NTimes);
	void WAttackObj(word OID,int Prio);
	void FlyAttack(word OID,byte Prio);
	void AttackPoint(byte x,byte y,byte wep,int Prio);
	bool AttackPoint(int x,int y,int z,byte Times,byte Flags,byte OrdType,byte Prio);
	bool NewAttackPoint(int x,int y,int Prio1,byte OrdType,word NTimes);
	void ProcessFly();
	void CreatePath(int x1,int y1);
	void ReallyCreatePath(int x1,int y1);
	void CreateSimplePath(int x1,int y1);
	void FreeAsmLink();
	
	void Die(bool Erase=false);
	void Eliminate();

	inline eLiveState GetLiveState();

	void MakeDamage(int Fundam,int Persist,OneObject* Sender);
	int GetDamage(int Fundam,OneObject* Sender,byte AttType);
	int MakeDamage(int Fundam,int Persist,OneObject* Sender,byte AttType,bool Act);
	void MakeDamage(int Fundam,int Persist,OneObject* Sender,byte AttType){
		MakeDamage(Fundam,Persist,Sender,AttType,1);
	};
	void SearchSupport(word OBJ);//Поиск помощников замочить OBJ
	void SearchVictim();
	void FreeOrdBlock(Order1* p );
	void ClearOrders();
	void ProcessMotion();
	void ProcessAttackMotion();
	void SendInGroup(byte tx,byte ty,byte x0,byte y0,byte x1,byte y1);
	void NextStage();
	bool BuildObj(word OID,int Prio,bool LockPoint,byte OrdType);
	int CheckAbility(word ID);
	bool Produce(word ID);
	bool Produce(word ID,word GroupID);	
	bool DamageWall(word OID,int Prio);
	void PerformUpgrade(word NewU,word MakerID);
	void SetDstPoint(int x,int y);
	void Patrol(int x1,int y1,int x2,int y2,byte prio);
	//void Repair(int x,int y,byte prio);
	void EnableDoubleForce();
	void DisableDoubleForce();
	void EnableTripleForce();
	void DiasableTripleForce();
	void EnableQuadraForce();
	void DisableQuadraForce();
	void ContinueAttackPoint(byte x,byte y,int Prio);
	void ContinueAttackWall(byte x,byte y,int Prio);
	void MakeMeUFO();
	void WaitForRepair();
	//inline int GetMinDam(){
	//	return Ref.Visual->info.Basic.MinDamage;
	//};
	//inline int GetMaxDam(){
	//	return Ref.Visual->info.Basic.MaxDamage;
	//}
	void MakeMeSit();
	//-------New---------
	void BlockUnit();
	void WeakBlockUnit();
	void UnBlockUnit();
	void DeletePath();
	void CheckState();
	bool CheckLocking(int dx,int dy);
	void SetDestCoor(int x,int y);
	void EscapeLocking();
	bool FindPoint(int* x1,int* y1,byte Flags);
	void ClearBuildPt();
	bool CheckBlocking();
	void DeleteBlocking();
	//returns points for damage
	bool GetDamagePoint(int x,int y,Coor3D* dp,int Precise);
	void TakeResourceFromSprite(int SID);
	void SetOrderedUnlimitedMotion(byte OrdType);
	void ClearOrderedUnlimitedMotion(byte OrdType);
	void ClearOrderedUnlimitedMotion(byte OrdType,word GroupID);
	int TakeResource(int x,int y,byte ResID,int Prio,byte OrdType);
	word FindNearestBase();
	//Type:
	//0 - single order (previous orders will be erased)
	//1 - add order to the head of link
	//2 - add order to the tile of link
	Order1* CreateOrder(byte Type);
	void DeleteLastOrder();
	void GetCornerXY(int* x,int* y);
	void GetPreciseCornerXY(int* x,int* y);
	bool GoToMine(word ID,byte Prio);
	bool GoToMine(word ID,byte Prio,byte Type);
	bool GoToTransport(word ID,byte Prio);
	void LeaveMine(word Type);
	void LeaveTransport(word Type);
	void HideMe();
	void ShowMe();
	bool CheckOrderAbility(int LParam,int RParam);
	bool CheckOrderAbility();
	void GlobLock();
	void GlobUnlock();
	void Fishing();
	int GetAttX();
	int GetAttY();
	void GetCylinder(int& CylinderRarius,int& CylinderHeight,int& UnitHeight);
    void GetPureCylinder(int& CylinderRarius,int& CylinderHeight,int& UnitHeight);

	Brigade* GetBrigade();
#pragma warning( disable : 4035 )
	inline int DistTo(int xx,int yy){
		__asm{
			mov		eax,xx
				mov		ebx,this
				mov		edx,[ebx]this.x
				sub		eax,edx
				jge		uui
				neg		eax
uui:		mov		ecx,yy
			mov		edx,[ebx]this.y
			sub		ecx,edx
			jge		uux
			neg		ecx
uux:		cmp		ecx,eax
			jl		uuz
			mov		eax,ecx
uuz:
		};
	};
	void CloseObject();

	UnitActiveAbilityList* ActiveAbility; 
	cvi_Alert Alert;

	//--------- for be_CGROUP ------------//
	DWORD*	pGRP_IDS;
	byte	GRP_N;
	void	AddInCGROUP		(DWORD	GID);
	void	RemFromCGROUP	(DWORD	GID);
	//---------------end------------------//

	byte KillMask;
};
#endif