#pragma once
#define MaxArtDep 16
#define NBRANCH 4
struct DIALOGS_API FogRec{
	word  NWeap;
	int   WProb;
	word* Weap;
};
class AdvCharacter;
class MonsterAbility;
class DIALOGS_API OrderClassDescription{
public:
	char* ID;
	char* Message;
	int IconPos;
	int IconID;
	OrderClassDescription();
	~OrderClassDescription();
};
struct DIALOGS_API SingleGroup{
	int ClassIndex;
	int NCommon;
	byte* IDX;
	int NForms;
	word* Forms;
};
class DIALOGS_API FormGroupDescription{
public:
	int NGrp;
	SingleGroup* Grp;
	FormGroupDescription();
	~FormGroupDescription();
	void Load(GFILE* f);
};
extern FormGroupDescription FormGrp;
class DIALOGS_API OrderDescription{
public:
	char* ID;
	int NLines;
	short** Lines;
	word*   LineNU;
	byte*   Opt;
	int NCom;
	short* ComX;
	short* ComY;
	short YShift;
	int NUnits;
	short BarX0;
	short BarY0;
	short BarX1;
	short BarY1;
	//symmetry groups
	word* Sym4f;
	word* Sym4i;
	word* SymInv;
	//additional parameters
	char AddDamage1;
	char AddShield1;
	char AddDamage2;
	char AddShield2;
	char FAddDamage;
	char FAddShield;
	word StandGroundBonus;
	byte GroupID;
	byte OrdUsage;
	bool DirectionalBonus;
	//------special for COSSACKS2-----
	int FirstActualLine;
	int NActualLines;
	int Width;//V pikselah
	int Hight;
	//---------------
	OrderDescription();
	~OrderDescription();
};
class DIALOGS_API StroiDescription{
public:
	byte ID;
	word NAmount;
	word* Amount;
	word* LocalID;
	word NUnits;
	word* Units;
};
class DIALOGS_API OfficerRecord{
public:
	word BarabanID;
	word FlagID;
	word NStroi;
	StroiDescription SDES[5];
};
struct DIALOGS_API Flags3D{
	int N;
	short Xr;
	short Points[48];
};
struct DIALOGS_API OneAddSprite{
	word SpriteID;
	short SortX;
	short SortY;
};
struct DIALOGS_API OneAddStage{
	word GPID;

	OneAddSprite Empty;
	OneAddSprite Stage1;
	OneAddSprite Ready;
	OneAddSprite Dead;

	word AddPoints;

	int NExplPoints;
	short* ExplCoor;

	short* FireX[2];
	short* FireY[2];
	short  NFires[2];

	int Cost[8];
};
#define MaxAStages 5
struct DIALOGS_API ComplexBuilding{
	byte Mask;
	OneAddStage Stages[MaxAStages];
};
struct DIALOGS_API ComplexUnitRecord:public BaseClass{
	bool CanTakeExRes;
	byte GoWithStage [16];
	byte TakeResStage[16];
	byte TransformTo[16];
	SAVE(ComplexUnitRecord);
		REG_MEMBER(_bool,CanTakeExRes);
		for(int i=0;i<16;i++){
            char c[32];
			sprintf(c,"GoWithStage%d",i);
			REG_MEMBER_EX2(_BYTE,GoWithStage[i],c);
			sprintf(c,"TakeResStage%d",i);
			REG_MEMBER_EX2(_BYTE,TakeResStage[i],c);
			sprintf(c,"TransformTo%d",i);
			REG_MEMBER_EX2(_BYTE,TransformTo[i],c);
		}
	ENDSAVE;
};
struct DIALOGS_API ExRect{
	int NRects;
	int Coor[4];
};
struct DIALOGS_API WeaponInSector{
	int AttIndex;
	int RMin,RMax,Angle;
	int MaxDamage,MinDamage,AnglFactor;
};

#define NAttTypes 12 
#define NFEARSUBJ 16
class MassSprite:public BaseClass{
public:
	int dx;
	int dy;
	int Sprite;
	SAVE(MassSprite){
        REG_MEMBER(_int,dx);
		REG_MEMBER(_int,dy);
		REG_ENUM(_index,Sprite,TREESTYPES);
	}ENDSAVE
};
class CommonParameters:public BaseClass{
public:
	float LOD1_Scale;
	float LOD2_Scale;
    float BlendingTime;
    bool  TakeResToBuildpoints;
	int   Level;

    float ShadowScaleX;
    float ShadowScaleY;
    int   ShadowSet;
    bool  ShadowHAligning;
	bool  GiveExpaDisable;
	int   SkirmishAwardPercent;
	byte  SelPrio;
	bool  DeadBody;

	bool  PortretDisable;
	short PortretHeroFile;

	int   PickGroundDX;
	int   PickGroundDY;

	bool  ControlButtonsDisable;

	char* Message;
	char* MessageLong;
	char* MessageProduce;

	bool AntiMage;

	int PickUp_KillingAward_Gold;
	int PickUp_KillingAward_Iron;
	int PickUp_KillingAward_Coal;

	ClonesArray<MassSprite> MassSprites;

	bool VulnerableOnlyForAntimage;

	SAVE(CommonParameters){
        REG_MEMBER(_float,LOD1_Scale);
		REG_MEMBER(_float,LOD2_Scale);
        REG_MEMBER(_float,BlendingTime);
        REG_MEMBER(_bool,TakeResToBuildpoints);
		REG_MEMBER(_int,Level);
        REG_MEMBER(_float,ShadowScaleX);
        REG_MEMBER(_float,ShadowScaleY);
        REG_MEMBER(_int,ShadowSet);
        REG_MEMBER(_bool,ShadowHAligning);
		REG_MEMBER(_bool,GiveExpaDisable);
		REG_MEMBER(_int,SkirmishAwardPercent);
		REG_MEMBER(_BYTE,SelPrio);
		REG_MEMBER(_bool,DeadBody);

		REG_MEMBER(_bool,PortretDisable);
		REG_MEMBER(_gpfile,PortretHeroFile);

		REG_MEMBER(_bool,ControlButtonsDisable);

		REG_MEMBER(_int,PickGroundDX);
		REG_MEMBER(_int,PickGroundDY);

		REG_MEMBER(_textid,Message);
		REG_MEMBER(_textid,MessageLong);
		REG_MEMBER(_textid,MessageProduce);

		REG_MEMBER(_bool,AntiMage);

		REG_MEMBER(_int,PickUp_KillingAward_Gold);
		REG_MEMBER(_int,PickUp_KillingAward_Iron);
		REG_MEMBER(_int,PickUp_KillingAward_Coal);
		
		REG_AUTO(MassSprites);

		REG_MEMBER(_bool,VulnerableOnlyForAntimage);
	}ENDSAVE;

    __constructor(CommonParameters){
		LOD1_Scale=0;
		LOD2_Scale=0;
        BlendingTime=0.5;
        TakeResToBuildpoints=true;
        ShadowScaleX=1.0f;
        ShadowScaleY=1.0f;
        ShadowSet=0;
        ShadowHAligning=false;
		SkirmishAwardPercent=0;
		PortretHeroFile=-1;
		SelPrio=0;
		DeadBody=false;
		Message=NULL;
		MessageLong=NULL;
		MessageProduce=NULL;
		PickGroundDX=0;
		PickGroundDY=0;
    }

    //virtual bool ForceSimplification(){
    //    return true;
    //}
};
class DIALOGS_API NewMonster:public BaseClass{
public:
	AnimationsScope* Animations;
	__forceinline NewAnimation* GetAnimation(DWORD ID){
		return Animations->Get(ID);
	}
	__forceinline NewAnimation* GetFirstAnimation(DWORD ID){
		return Animations->Get(ID,0);
	}
	NewAnimation* CreateAnimation(char* Name);
	byte TransXMask;
	word AttackRadius1[NAttTypes];   //???????? ??????????? ??
	word AttackRadius2[NAttTypes];   //???? ??? ??????????? ??
	word DetRadius1[NAttTypes];      //???, ? ???? ?????? ??????????????
	word DetRadius2[NAttTypes];
	word AttackRadiusAdd[NAttTypes]; //?????????????????? 
	//??????????????? ????
	word VisibleRadius1;  //additional red radius, shown near unit, it is only visible feature
	word VisibleRadius2;  //additional yellow radius, shown near unit, it is only visible feature

	Weapon* DamWeap[NAttTypes];   //??? ? ????(??,??...)

	int ItemDropValue;

	byte Rate[NAttTypes];            //16=x1 rate 
	word AttackPause[NAttTypes];
	short AngleUp[NAttTypes];         //64=45degrees,32=arctan(1/2)degrees
	short AngleDn[NAttTypes];
	short MinDamage[NAttTypes];
	short MaxDamage[NAttTypes];
	short DamageRadius[NAttTypes];
	word  DamageDecr[NAttTypes];
	byte  WeaponKind[NAttTypes];
	byte  AttackMask[NAttTypes];
//#ifdef NEWMORALE
	short       MoraleDecSpeed;
	int         StartMorale;
//#else //NEWMORALE
//	word FearFactor[NFEARSUBJ];
//#endif //NEWMORALE
	//----Market economy control-------
	int EcResID[6];
	int EcResIcn[6];
	double ResCostBuy[6];
	double ResCostSell[6];
	double ResCostBuy0[6];
	double ResCostSell0[6];
	double ResCostBuy1[6];
	double ResCostSell1[6];
	double ResCostBuy2[6];
	double ResCostSell2[6];
	double ExpBUY[6];
	double ExpSELL[6];
	double TimeBUY[6];
	double TimeSELL[6];
	double Nalog[6];
	int NalogResID[6];
	//---------------------
	byte FearType  [NAttTypes];
	byte FearRadius[NFEARSUBJ];

	word  MyIndex;
	short SrcZPoint;				 //additional height of the weapon
	short DstZPoint;
	word NLockPt;
	char* LockX;
	char* LockY;

	word NSLockPt[MaxAStages];
	char* SLockX[MaxAStages];
	char* SLockY[MaxAStages];

	word NBLockPt;
	char* BLockX;
	char* BLockY;

	word NCheckPt;
	char* CheckX;
	char* CheckY;

	ComplexBuilding* CompxCraft;
	ComplexUnitRecord* CompxUnit;
	ExRect* Doors;

	char* Message;
	char* LongMessage;
	char* GetLongMessage();

	char* PieceName;
	bool Officer:1;
	bool Baraban:1;
	bool Building:1;
	bool Peasant:1;
	bool UnitAbsorber:1;
	bool PeasantAbsorber:1;
	bool Transport:1;
	bool Producer:1;
	bool SpriteObject:1;
	bool Wall:1;
	bool RiseUp:1;
	bool SelfProduce:1;
	bool WaterActive:1;
	bool TwoParts:1;
	bool Port:1;
	bool Farm:1;
	bool ShowDelay:1;
	bool Capture:1;
	bool CantCapture:1;
	bool NotHungry:1;
	bool ShotForward:1;
	bool Artilery:1;
	bool Rinok:1;
	bool SlowDeath:1;
	bool AutoNoAttack:1;
	bool AutoStandGround:1;
	bool AttBuild:1;
	bool CanStandGr:1;
	bool Priest:1;
	bool Shaman:1;
	bool ResSubst:1;
	bool Archer:1;
	bool ArtDepo:1;
	bool Artpodgotovka:1;
	bool CanBeKilledInside:1;
	bool CanBeCapturedWhenFree:1;
	bool CanShoot:1;
	bool CanStorm:1;
	bool NoDestruct:1;
	bool SlowRecharge:1;
	bool PeasantConvertor:1;
	bool ExField:1;
	bool BReflection:1;
	bool FullRotation:1;
	bool LikeCannon:1;
	bool FriendlyFire:1;
	bool CommandCenter:1;
	bool ArmAttack:1;
	bool CanFire:1;
	bool Nedorazvitoe:1;
	bool HighUnit:1;
	bool HighUnitCantEnter:1;
	bool StandAnywhere:1;
	bool NikakixMam:1;
	bool No25:1;
	bool CanStopBuild:1;
	bool FormLikeShooters:1;
	bool ShowIdlePeasants:1;
	bool ShowIdleMines:1;
	bool Animal:1;
	bool AI_use_against_buildings:1;
	bool NoFarm:1;
	bool NoMorale:1;
	bool HaveRotateAnimations:1;
	bool SelfTransform:1;
	bool UseLikeGate:1;
	bool OldStyleSelection:1;
	bool LongDeath:1;
	bool NotSelectable:1;	
	bool CanBeInFocusOfFormation:1;
	bool UnbeatableWhenFree:1;
	bool DontTransformToChargeState:1;
	bool ShowInsideUnits:1;
	bool DontStuckInEnemy:1;
	bool NeverCapture:1;
	bool InvisibleOnMinimap:1;
	bool Like2DOnMinimap:1;
	bool Immortal:1;
	bool LowCollision:1;
	bool NoCollision:1;
	bool DontFillCannon:1;
	bool CanBuild:1;
	bool DontAffectFogOfWar:1;
	bool DontAnswerOnAttack:1;
	bool DontRotateOnDeath:1;
	bool NoInfinity:1;
	bool BornBehindBuilding:1;
	bool GlobalCommandCenter:1;
	bool CharacterWasModified:1;
	bool DontShowALT_Info:1;
    bool NoBlending:1;

	byte DirectFight;

	int KillerAward[8];
	int KillerAwardRandom[8];

	short AI_PreferredAttR_Min;
	short AI_PreferredAttR_Max;

	byte WaterCheckDist1;
	byte WaterCheckDist2;
	word SingleShot;

	byte Category;
	byte NInArtDepot;
	byte MeatTransformIndex;
	byte NInFarm;
	byte ArtSet;
	byte TransMask[MaxAStages];
	NewAnimation* UpperPart;
	short UpperPartShift;
	NewAnimation* BuiAnm;
	byte ArtCap[MaxArtDep];
	short BuiDist;
	short MaxPortDist;
	byte NRiseFrames;
	byte RiseStep;
	byte ProdType;
	byte SelfProduceStep;
	word FreeAdd;
	word PeasantAdd;
	word MaxInside;
	word ResConcentrator;
	byte MaxZalp;
	byte NoWaitMask;
	byte UnitRadius;
	byte TempDelay;
	byte FireLimit;
	byte SkillDamageBonus;
	byte SkillDamageMask;
	short StartScale;//scale of unit 256 = scale(1.0)
	short FormFactor;//scale factor for effects on unit, 256 = scale(1.0)
	short SkillDamageFormationBonus;
	short SkillDamageFormationBonusStep;
	short Psixoz;
	int Ves;
	short LowCostRadius;
	short StopDistance;
	short SpeedScaleOnTrees;

	short AddShotRadius;
	byte  PromaxPercent[8];
	byte  PromaxCategory;

	DynArray<short> BuildPtX;
	DynArray<short> BuildPtY;

	DynArray<short> ConcPtX;
	DynArray<short> ConcPtY;
	DynArray<short> ConcPtZ;

	DynArray<short> PosPtX;
	DynArray<short> PosPtY;
	DynArray<short> PosPtZ;

	DynArray<short> CraftPtX;
	DynArray<short> CraftPtY;
	DynArray<short> CraftPtZ;

	DynArray<short> DamPtX;
	DynArray<short> DamPtY;
	DynArray<short> DamPtZ;

	DynArray<short> BornPtX;
	DynArray<short> BornPtY;
	DynArray<short> BornPtZ;

	word NShotPt;
	short* ShotPtX;
	short* ShotPtY;
	short* ShotPtYG;
	short* ShotPtZ;
	short* ShotDelay;
	byte* ShotAttType;

	byte* ShotDir;
	byte* ShotDiff;
	word* ShotMinR;
	word* ShotMaxR;

	short* FireX[2];
	short* FireY[2];
	short  NFires[2];
	byte MaxResPortion[8];
	int  NeedRes[8];
	int  UMS_Res[8];
	int CenterMX;
	int CenterMY;
	int BRadius;
	word ProduceStages;
	word IconFileID;
	word IconID;

	word Page1Icon;
	word Page2Icon;
	word Page3Icon;
	word Page1IconON;
	word Page2IconON;
	word Page3IconON;

	char* Page1Hint;
	char* Page2Hint;
	char* Page3Hint;
	byte CurrPage;

	int Radius1;
	int Radius2;
	int MotionDist;
	//int OneStepDX[256];
	//int OneStepDY[256];
	//int POneStepDX[256];
	//int POneStepDY[256];
	int PicDx,PicDy,PicLx,PicLy;
	short BuildX0,BuildY0,BuildX1,BuildY1;
	byte KillMask;
	byte MathMask;
	byte CO_MathMask;//for complex object - default mathmask
	//
	char BodyStyle;
	//
	byte ExitPause;
	//rectangle around the monster
	int RectLx,RectLy,RectDx,RectDy;
	//Monster characteristics
	word Res1cost,Res2cost,Res3cost;
	word Life,Shield;
	word AttRange,VisRange;
	word VisRangeShift;
	word MinAttRange,MaxAttRange,MaxNearRange;
	word AttType;
	word Time,Kind;
	short AnmUpShift;
	short* Bars3D;
	short  NBars;
	char* Name;
	//fogging&fire
	FogRec Fogging;
	FogRec Destruct;
	FogRec Fire;
	short Protection[16];
	word Sprite;
	word SpriteVisual;
	short AutoBornSprite;
	short BornOnSprite;
	byte ExplosionMedia;
	byte EMediaRadius;
	byte LockType;//0-Land,1-Water
	byte MotionStyle;//0-Soldiers,1-Cavalery,2-Sheeps,3-Fly
	NewAnimation* Veslo;
	NewAnimation* Reflection;
	byte VisionType;
	short VesloRotX;
	short VesloRotY;
	byte NVesel;
	short* VesloX;
	short* VesloY;
	short LinearLength;


	ClassArray<ComplexFireSource> MultiWp;
	/*short* MultiWpX;
	short* MultiWpY;
	short* MultiWpZ;
	short NMultiWp;	*/


	short ResEff;
	short SelectSoundID;
	short AttackSoundID;
	short BornSoundID;
	short OrderSoundID;
	short ReadySoundID;
	short UpgradeSoundID;
	short DeathSoundID;
	short StrikeSoundID;
	short FastClickSoundID;
	short MotionOnRoadSound;
    short AttackSpeed;
    short Speed;

	short* HideTriX;
	short* HideTriY;
	short NHideTri;

	word  Razbros;
	word  ExplRadius;

	short ResConsumer;
	byte ResConsID;

	byte Behavior;
	byte ResAttType;
	byte ResAttType1;
	byte NShotRes;
	word* ShotRes;
	word CostPercent;
	byte VesStart;
	byte VesFin;
	byte MinRotator;
	byte FishSpeed; 
	word FishAmount;
	byte InfType;
	word PictureID;
	byte Force;//for AI
	//for strongholds siege
	byte MinOposit;
	byte MaxOposit;
	byte StormForce;

	byte IdlePPos;
	byte IdleMPos;

	byte EuropeanMask;

	WeaponInSector* WSECT;
	short* WavePoints;
	byte NWaves;
	char WaveDZ;
	byte Usage;
	byte BattleForce;
	Flags3D* FLAGS;
	char* MD_File;
	short ArmRadius;
	NewMonster();
	bool CreateFromFile(char* Name);
	void InitNM(char* name);
	NewAnimation* LoadNewAnimationByName(char* Name,int Index=0);	
	//AdvCharacter* AdvChar;
	~NewMonster();

	word ComplexObjIndex;
	byte GrenadeRechargeTime;
	byte MaxGrenadesInFormation;
	word PortionLimit;
	word BigIconFile;
	word BigIconIndex;
	word MinIconFile;
	word MinIconIndex;	
	word InMenuIconFile;
	word InMenuIconIndex;	

	byte BRandomPos; //????? ????????? (points)
	byte BRandomSpeed; //???? ???? ?????(%)
	word FreeShotDist; //? ???? ? ???(points)

	word VUI;	// Vital Unit Interface	
	byte BigColdWeapSprite;
	byte BigFireWeapSprite;
	word BigWeapFile;
	word BuildHotKey;
	int PortBackColor;
	word PortBranch;
	word PortBackSprite;
	word StandGroundTime;
	short FlagIndex;
	short FlagX;
	short FlagY;
	short StartFlyHeight;
	short FlyHeight;
	//-----mine----
	short MineRadius;
	short MineDamage;
	short BuildNearBuildingRadius;
	//-------------
	short Directional3DBarsEffect;
	//-
	bool SpecialSel;	
	int DetonationForce;
	int MaxAttackersAmount;
	int ColorVariation;
	byte StrikeProbability;

	MonsterAbility* Ability;

	bool BoidsMoving;
	int BoidsMovingMinDist;
	int BoidsMovingWeight;
	bool SitInFormations;
	int FormationDistanceScale;//100=usual size
	int StrikeFlySpeed;//Speed of unit that flies after strike
	int StrikeFlyMaxSpeed;//MaxSpeed of unit that flies after strike
	int StrikeForce;//modulator of striken unit speed 100
	int StrikeRotate;
	int Expa;
	word RotationAtPlaceSpeed;
	bool DiscreteRotationDirections;
	word MaxMana;
	int SpeedScale;
	//NewMonsterExtension Ext;
	//new transport/mine params
	byte InviteMask;
	byte VisitorMask;
	short NPlacesInTransport;
	//morale for Rome-style game
	short VeteranKills;
	short ExpertKills;
	short VeteranExtraDamage;
	short ExpertExtraDamage;
	short VeteranExtraShield;
	short ExpertExtraShield;
	short MoraleRegenerationSpeed;
	short KillDelay;
	word  BrigadeWaitingCycles;
	word  RetreatRadius;
	word  MinTopDistanceToEnterRoad;
	word  MinDistForLineFormations;
	word  MinDistanceToEnterRoad;

	bool ShotAlwaysOn;
	float PlaneFactor;
	word KineticLimit;
	byte MissProbabilityForInsideUnits;
	byte MissProbabilityOnHeightDiff100;
	byte MaxMissProbabilityOnHeightDiff;

	DWORD TFactor;

	float selScaleX;
	float selScaleY;
	short selShift;
	char  selIndex;
	char  selIndexBR;

	bool Use3pAlign;

	int AlignPt1x;
	int AlignPt1y;
	int AlignPt1z;

	int AlignPt2x;
	int AlignPt2y;
	int AlignPt2z;

	int AlignPt3x;
	int AlignPt3y;
	int AlignPt3z;

	short CollisionRadius;

	ExtendedIcon ExIcon;

	ClonesArray<OneEmotion> EmotIcons;

	int DamageOnBuildingMultiplicator;
	bool Incorporeal;
	ClonesArray<OneBoneInfo> ActiveBones;

	byte UnitsColumnIndex;
	int UnitCylinderRadius;
	int UnitCylinderHeight;

    CommonParameters NewParams;
};
class DIALOGS_API AdvCharacter: public BaseClass {
public:
	AdvCharacter();
	~AdvCharacter();
	word		AttackRadius1[NAttTypes];
	word		AttackRadius2[NAttTypes];
	word		DetRadius1[NAttTypes];
	word		DetRadius2[NAttTypes];
	//Weapon*		DamWeap[NAttTypes];
	word		AttackPause[NAttTypes];
	short		MaxDamage[NAttTypes];
	byte		WeaponKind[NAttTypes];
	byte		Rate[NAttTypes];
	byte		Protection[16];
	int			NeedRes[8];
	word		MaxInside;
	word		ProduceStages;
	word		Life;
	word		BirthLife;
	word		Shield;
	byte		FishSpeed;
	word		FishAmount;
	word        Razbros;

	word        MinR_Attack;
	word		MaxR_Attack;
	word        MaxDam;
	word		NInFarm;
	short       ResEff;

//#ifdef NEWMORALE
	short       MoraleDecSpeed;
	int         StartMorale;
//#else //NEWMORALE
//	word		FearFactor[NFEARSUBJ];	
//#endif //NEWMORALE

	bool		Changed;
	bool        Allocated;
	int			Speed;	
	int			AttackSpeed;
	byte		VisionType;
	int			VisRange;
	word		MaxMana;	

	//NewMonsterExtension Ext;

	SAVE(AdvCharacter){
		char cc[32];
		for(int i=0;i<NAttTypes;i++){
			sprintf(cc,"AttackRadius1_%d",i);
			REG_MEMBER_EX2(_WORD,AttackRadius1[i],cc);
			sprintf(cc,"AttackRadius2_%d",i);
			REG_MEMBER_EX2(_WORD,AttackRadius2[i],cc);
			sprintf(cc,"DetRadius1_%d",i);
			REG_MEMBER_EX2(_WORD,DetRadius1[i],cc);
			sprintf(cc,"DetRadius2_%d",i);
			REG_MEMBER_EX2(_WORD,DetRadius2[i],cc);
			sprintf(cc,"AttackPause_%d",i);
			REG_MEMBER_EX2(_WORD,AttackPause[i],cc);

			sprintf(cc,"MaxDamage_%d",i);
			REG_MEMBER_EX2(_short,MaxDamage[i],cc);

			sprintf(cc,"WeaponKind_%d",i);
			REG_MEMBER_EX2(_BYTE,WeaponKind[i],cc);
			sprintf(cc,"Rate_%d",i);
			REG_MEMBER_EX2(_BYTE,Rate[i],cc);
		}
		for(int i=0;i<16;i++){
			sprintf(cc,"Protection_%d",i);
			REG_MEMBER_EX2(_BYTE,Protection[i],cc);
		}

		//int			NeedRes[8];
		//word		MaxInside;
		//word		ProduceStages;
		REG_MEMBER(_WORD,Life);
		REG_MEMBER(_WORD,BirthLife);
		REG_MEMBER(_WORD,Shield);
		//byte		FishSpeed;
		//word		FishAmount;
		REG_MEMBER(_WORD,Razbros);

		REG_MEMBER(_WORD,MinR_Attack);
		REG_MEMBER(_WORD,MaxR_Attack);
		REG_MEMBER(_WORD,MaxDam);
		REG_MEMBER(_WORD,NInFarm);

		REG_MEMBER(_short,ResEff);

		//#ifdef NEWMORALE
		//		short       MoraleDecSpeed;
		//		int         StartMorale;
		//#else //NEWMORALE
		//		word		FearFactor[NFEARSUBJ];	
		//#endif //NEWMORALE

		REG_MEMBER(_bool,Changed);
		REG_MEMBER(_bool,Allocated);
		REG_MEMBER(_int,Speed);
		REG_MEMBER(_int,AttackSpeed);
		REG_MEMBER(_BYTE,VisionType);
		REG_MEMBER(_int,VisRange);
		REG_MEMBER(_WORD,MaxMana);

		//NewMonsterExtension Ext;

	}ENDSAVE;
};

class DIALOGS_API NewUpgrade{
public:
	//information
	char* Name;
	char* Message;
	byte Level;
	byte Branch;
	word IconFileID;
	word IconSpriteID;
	char IconPosition;
	byte NatID;
	word  Cost[8];
	byte CtgUpgrade;
	byte CtgType;
	union{
		int  NCtg;
		int  CtgValue;
	};
	word*    CtgGroup;
	byte UnitType;
	union{
		int NUnits;
		int UnitValue;
	};
	word* UnitGroup;
	byte ValueType;
	union{
		int  Value;
		int  NValues;
	};
	int* ValGroup;
	//mechanics
	bool Done:1;
	bool Enabled:1;
	bool PermanentEnabled:1;
	bool IsDoing:1;
	bool Gray:1;
	bool Individual:1;
	bool ManualDisable:1;
	bool ManualEnable:1;
	bool StageUp:1;
	word NStages;
	word CurStage;
	byte NAutoPerform;
	byte NAutoEnable;
	word* AutoPerform;
	word* AutoEnable;
	byte Options;
	byte StageMask;	
};
//???? ???? ??, ???? ?? ????
class DIALOGS_API GeneralObject{
public:
	char* Message;

	bool NoSearchVictim:1;
	bool Enabled:1;
	bool CondEnabled:1;
	bool Transport:1;	
	bool WATT:1;
	bool AATT:1;
	bool P100:1;
	bool T3X3:1;
	bool FWEAP:1;//???? ???
	bool AGold:1;
	bool AWood:1;
	bool Submarine:1;
	bool CanFly:1;
	bool CanAttWall:1;
	bool CanFear:1;
	//bool UseMagic:1;
	bool canNucAttack:1;
	bool UFO:1;
	bool UFOTrans:1;
	bool CanRepair:1;
	bool ExtMenu:1;
	bool CanDest:1;
	bool ManualDisable:1;
	bool ManualEnable:1;
	byte Options;
	byte NatID;
	byte Country;
	OfficerRecord* OFCR;
	NewMonster* newMons;
	char* MonsterID;
	word MagAgainst;
	word MagWith;
	word IDforUFO;
	//word capMagic;
	byte Kind;
	word MaxAutoAmount;
	word Useful[NBRANCH];//????? ??? ? ??? ? ????
	word SpUsef[NBRANCH];//??? ?????
	word AbsNeedPrio;
	word AbsNeedCount;
	word LifeShotLost;
	int	 cost;
	word delay;
	//short wepX;
	//short wepY;
	byte WepSpeed;
	byte WepDelay;
	//byte  VisRadius;
	//byte  VisSpots;
	//byte  SpotType;
	//byte  SpotSize;
	//byte  DangerZone;
	word NUpgrades;
	word* Upg;
	char Wdx[8];
	char Wdy[8];
	byte NIcons;
	word* IRefs;
	word NWeap;
	short NStages;
	Weapon* MWeap[12];
	byte SWPIndex[12];
	short HitSound;
	short ClickSound;
	short OrderSound;
	short DeathSound;
	short BornSound;
	short AttackSound;
	short TreeSound;
	short GoldSound;
	word LockID;
	byte NLockUnits;
	byte Branch;

	byte SizeX;
	byte SizeY;
	byte StageMask;

	int NeedRes[8];//cost
	int CostPercent;//cost growing
	bool CostGrowsOnlyForReadyBuildings;//cost grows only when building is ready
	int NFarmsPerSettlement;
	int NewNFarm;

	word FlagBearerID;//used for formations in Rome, sometimes usual unit becomes flagbearer
	word FlagBearerID2;//used for formations in Rome, sometimes usual unit becomes flagbearer
	word UsualFormID;//back transformation
	word NIndex;

	byte ResourceID[4];     //??? ?????? ????
	word ResAmount[4];      //??? ????? ? ?????
	AdvCharacter* MoreCharacter;


	void GetMonsterCostString(char* st);
	void CloseGO();
};
//??????????????,???
class DIALOGS_API Visuals : public GeneralObject
{
	
};
//Upgrade-??? ??? ?? ????? ???
class DIALOGS_API ObjectUpgrade : public GeneralObject
{
public:
	word SourceTypeIndex[8];
	word FinalTypeIndex[8];
};
//Upgrade - ? Visuals-???? ????? ? ??? ????
class DIALOGS_API ParameterUpgrade : public GeneralObject
{
	word ObjectIndex[8];
	byte ChangedParameter[8];
	byte AdditionalValue[8];
};

class OneObject;
typedef void ReportFn(OneObject* Sender);
#define MaxFillObj 4
//???? ????? ???
//??? 1-? ???
struct DIALOGS_API Order1{
	//???? ? ????????, ?? NULL ? ??
	//??????????
	Order1* NextOrder;
	byte Used;
	//??? ????? ????? ???
	//0-???? ??? ?????
	//??? ????????? ???? ??? ??????,
	//?? ????????? ??????? ?????
	byte PrioryLevel;
	byte OrderType;
	byte OrderTime;
	//????? ? ?????? ????
	ReportFn* DoLink;
	union{
		struct{
			byte VisibilityRadius;
		} Stand;
		struct{
			int x;
			int y;
			short z0;
			short z1;
			word PrevDist;
			byte Times;
			word EnemyID;
			word EnemySN;
			word Steps;
		} MoveToXY;
		struct{
			byte xd;
			byte yd;
			byte time;
			word BuildID;
			word BSN;
		} UFO;
		struct{
			word ox;
			word oy;
			word x,y,z;
			byte wep;
		} AttackXY;
		struct{
			word ObjIndex;
			word SN;
			word PrevDist;
			byte wep;
		} MoveToObj;
		struct{
			int ObjIndex;
			word SN;
			short ObjX;
			short ObjY;
			byte AttMethod;
		}BuildObj;
		struct{
			short x;
			short y;
			short x1;
			short y1;
			byte dir;
		} Patrol;
		struct{
			word ObjIndex;
			int Progress;
			int NStages;
			word ID;
			byte PStep;
			word Power;
		}Produce;
		struct{
			word OldUpgrade;
			word NewUpgrade;
			int Stage;
			int NStages;
		}PUpgrade;
		struct{
			byte dir;
		}MoveFrom;
		struct{
			int x;
			int y;
			int SprObj;
			byte ResID;
		}TakeRes;
		struct{
			short LockX;
			short LockY;
			short EndX;
			short EndY;
		}DelBlock;
		struct{
			word x,y;
			short dx,dy;
			word NextX,NextY,NextTop;
		}SmartSend;
		struct{
			word IDS[MaxFillObj];
			word SNS[MaxFillObj];
		}FillObject;
		struct{
			DWORD Serial;
			word TargetID;
			word TargetSN;
			int Tx;
			int Ty;
			int Dist;
			int Radius;
			int CursorID;			
		}Spell;
	}info;
};