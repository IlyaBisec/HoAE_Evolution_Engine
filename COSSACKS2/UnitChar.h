#pragma once
//#include ""
//
class OneAttStateInfo:public BaseClass{
public:
	int   MinAttackRadius;
	int   MaxAttackRadius;
	int   MinDetRadius;
	int   MaxDetRadius;
    _str  Weapon;
	int   MotionRate;
	int   AttackPause;
	bool  NoPausedAttack;
	int   Damage;
	int   WeaponKind;
	int   DamageDecrementRadius;	
	DWORD AttackMask;
	int   FearType;
    int   FearRadius;
	int   ShotCost[8];

	SAVE(OneAttStateInfo);
		REG_MEMBER(_int,MinAttackRadius);
		REG_MEMBER(_int,MaxAttackRadius);
		REG_MEMBER(_int,MinDetRadius);
		REG_MEMBER(_int,MaxDetRadius);
		REG_ENUM(_strindex,Weapon,WEAPONS);
		REG_MEMBER(_int,MotionRate);
		REG_MEMBER(_int,AttackPause);
		REG_MEMBER(_bool,NoPausedAttack);
		REG_MEMBER(_int,Damage);
		REG_MEMBER(_int,WeaponKind);
		REG_MEMBER(_int,DamageDecrementRadius);
		REG_MEMBER(_DWORD,AttackMask);
		REG_MEMBER(_int,FearType);
		REG_MEMBER(_int,FearRadius);		
		REG_MEMBER_EX(_int,ShotCost[0],ShotRes0_Cost);
		REG_MEMBER_EX(_int,ShotCost[1],ShotRes1_Cost);
		REG_MEMBER_EX(_int,ShotCost[2],ShotRes2_Cost);
		REG_MEMBER_EX(_int,ShotCost[3],ShotRes3_Cost);
		REG_MEMBER_EX(_int,ShotCost[4],ShotRes4_Cost);
		REG_MEMBER_EX(_int,ShotCost[5],ShotRes5_Cost);
	ENDSAVE;
};
class AblName:public BaseClass{
public:
	_str AbilityName;
	int ItemDrop;
	int CommonRoomStartParam;
	//ClassRef<be_CSMARTOBJECT> SmartObject;
	//ClassRef<be_CMSG_FROMUSER> SmartCommand;
	//be_CSO_REF SmartObject;
	//be_CMSG_REF SmartCommand;

	_str Name;
	short PicFile;
	int PicSprite;
	char* Hint;
	//_str Hint;

	bool DeleteAfterUse;

	be_CRefParamFunction be_Process;
	be_CRefParamFunction be_LeftClick;
	be_CRefParamFunction be_EnterDialog;

	SAVE(AblName){
		REG_ENUM(_strindex,AbilityName,ABILKI);
		REG_MEMBER(_int,ItemDrop);
		REG_MEMBER(_int,CommonRoomStartParam);

		//REG_AUTO(SmartObject);
		//REG_AUTO(SmartCommand);

		//REG_ENUM(_strindex,SmartObject,BE_SMARTOBJECT_ENUM);
		//REG_ENUM(_strindex,SmartCommand,BE_USMSG_ENUM);

		REG_AUTO(Name);
		REG_MEMBER(_gpfile,PicFile);
		REG_SPRITE(PicSprite,PicFile);
		REG_MEMBER(_textid,Hint);
		//REG_AUTO(Hint);

		REG_MEMBER(_bool,DeleteAfterUse);

		REG_AUTO(be_Process);
		REG_AUTO(be_LeftClick);
		REG_AUTO(be_EnterDialog);

	}ENDSAVE;

	AblName();
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$AbilityName$%";
		if(ItemDrop>0) dest+=", ItemDrop=%$ItemDrop$%";
		return true;
	}
	virtual const char* GetElementView(int Index,const char* LocalName);
};
class AblNameArray: public ClonesArray<AblName>{
	SAVE(AblNameArray){
		REG_PARENT(ClonesArray<AblName>);
	}ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="ActiveAbility:";
		if(!ClassEditor::CheckIfOpen(this)){
			dest+="\n%$this,  \n$%";
		}
		return true;
	}
};
class BasicObjectChars:public BaseClass{
public:

	const char* GetThisElementView(const char* x){
		char* s=GetGlobalBuffer();
		sprintf(s,"%s: %s",x,UnitID.pchar());
		return s;
	}    
	virtual void  ConvertToNewMonster   (NewMonster* NM);
	virtual void  ConvertFromNewMonster (NewMonster* NM);

	//internal, not for registration
	bool						Building;
	//for registration
	_str   						UnitID;
	_str						UnitMD;
	short  						IconFileID;
	int    						IconID;

	short						FormFactor;

	int							KillerAward[8];
	int							KillerAwardRandom[8];

	int    						Hitpoints;
	int							ProduceStages;
	float						Scale;
	int                         Cost[8];
	int							Protection[16];
	DWORD 						MatherialMask;
	DWORD 						KillingMask;
	int   						VisionRange;
	int   						LockType;
	_str						Message;
	_str						LongMessage;
	_str						PieceName;
	int							MaxMana;
    int                         AttackSpeed;
    int                         Speed;
	int							ExpForKillingThisUnit;
	int							SearchEnemyRadius;
	bool						DontAnswerOnAttack;
	bool						DontAffectFogOfWar;
	bool						InvisibleOnMinimap;
	bool						Like2DOnMinimap;
	bool						CanBeInFocusOfFormation;
	bool						NotSelectable;	
	bool  						CanBeCaptured;
	bool  						CantCapture;
	bool  						NeverCapture;
	bool						ShowAttackDelay;
	bool  						DontConsumeFood;
	bool						DontConsumerLivingPlaces;
	bool						ShowInsideUnits;
	bool						DontShowALT_Info;
    bool                        NoBlending;
    bool                        ShipRotate90;
    int							ExtraConsumingResource;
	int							ExtraConsumingResourceSpeed;
	AblNameArray				Ability;
	int							RectDx;
	int							RectDy;
	int							RectLx;
	int							RectLy;
	int							UnitCylinderRadius;
	int							UnitCylinderHeight;
	int							SelectionType;
	int							SelectionTypeInFormation;
	float						SelectionScaleX;
	float						SelectionScaleY;
	int							SelectionShift;
	int							ColorVariation;
	
	bool	Immortal;
	bool	Incorporeal;

	int		AddShotRadius;

	int		InviteMask;
	int FastClickSoundID;
	int AttackSoundID;
	int SelectSoundID;	
	int BornSoundID;
	int OrderSoundID;
	int ReadySoundID;
	int UpgradeSoundID;
	int DeathSoundID;	
	int MotionOnRoadSound;

	int DamageOnBuildingMultiplicator;

	ExtendedIcon ExIcon;
	ClonesArray<OneEmotion> EmotIcons;
	ClonesArray<OneBoneInfo> ActiveBones;

	bool UnitAbsorber;
	bool PeasantAbsorber;
	word MaxInside;
    int FreeAdd;
    int PeasantAdd;

	DWORD						TFactor;

    CommonParameters NewParams;

	SAVE(BasicObjectChars);
		REG_AUTO(UnitID);
		REG_AUTO(UnitMD);		
		REG_AUTO(Message);
		REG_AUTO(LongMessage);
		REG_AUTO(PieceName);
		REG_SPRITE(IconID,IconFileID);

		REG_MEMBER(_short,FormFactor);

		REG_MEMBER_EX(_int,KillerAward[0],KillerAward0);
		REG_MEMBER_EX(_int,KillerAward[1],KillerAward1);
		REG_MEMBER_EX(_int,KillerAward[2],KillerAward2);
		REG_MEMBER_EX(_int,KillerAward[3],KillerAward3);
		REG_MEMBER_EX(_int,KillerAward[4],KillerAward4);
		REG_MEMBER_EX(_int,KillerAward[5],KillerAward5);

		REG_MEMBER_EX(_int,KillerAwardRandom[0],KillerAwardRandom0);
		REG_MEMBER_EX(_int,KillerAwardRandom[1],KillerAwardRandom1);
		REG_MEMBER_EX(_int,KillerAwardRandom[2],KillerAwardRandom2);
		REG_MEMBER_EX(_int,KillerAwardRandom[3],KillerAwardRandom3);
		REG_MEMBER_EX(_int,KillerAwardRandom[4],KillerAwardRandom4);
		REG_MEMBER_EX(_int,KillerAwardRandom[5],KillerAwardRandom5);

		REG_MEMBER(_int,Hitpoints);
		REG_MEMBER(_int,ProduceStages);
		REG_MEMBER(_float0_10,Scale);
		REG_MEMBER(_int,MaxMana);
        REG_MEMBER(_int,AttackSpeed);
        REG_MEMBER(_int,Speed);
		REG_MEMBER(_int,ExpForKillingThisUnit);	
		REG_MEMBER(_int,SearchEnemyRadius);
		REG_AUTO(Ability);
		REG_MEMBER(_int,RectDx);
		REG_MEMBER(_int,RectDy);
		REG_MEMBER(_int,RectLx);
		REG_MEMBER(_int,RectLy);
		REG_MEMBER(_int,UnitCylinderRadius);
		REG_MEMBER(_int,UnitCylinderHeight);
		
		REG_MEMBER_EX(_int,Cost[0],Res0_Cost);
		REG_MEMBER_EX(_int,Cost[1],Res1_Cost);
		REG_MEMBER_EX(_int,Cost[2],Res2_Cost);
		REG_MEMBER_EX(_int,Cost[3],Res3_Cost);
		REG_MEMBER_EX(_int,Cost[4],Res4_Cost);
		REG_MEMBER_EX(_int,Cost[5],Res5_Cost);

		REG_MEMBER_EX(_int,Protection[0],Protection0);
		REG_MEMBER_EX(_int,Protection[1],Protection1);
		REG_MEMBER_EX(_int,Protection[2],Protection2);
		REG_MEMBER_EX(_int,Protection[3],Protection3);
		REG_MEMBER_EX(_int,Protection[4],Protection4);
		REG_MEMBER_EX(_int,Protection[5],Protection5);

		REG_MEMBER(_DWORD,MatherialMask);
		REG_MEMBER(_DWORD,KillingMask);
		REG_MEMBER(_int,VisionRange);
		REG_ENUM(_index,LockType,LOCKTYPE);				

		REG_MEMBER(_bool,DontAnswerOnAttack);
		REG_MEMBER(_bool,DontAffectFogOfWar);
		REG_MEMBER(_bool,InvisibleOnMinimap);
		REG_MEMBER(_bool,Like2DOnMinimap);
		REG_MEMBER(_bool,CanBeInFocusOfFormation);
		REG_MEMBER(_bool,NotSelectable);
		REG_MEMBER(_bool,CanBeCaptured);
		REG_MEMBER(_bool,CantCapture);
		REG_MEMBER(_bool,ShowAttackDelay);
		REG_MEMBER(_bool,DontConsumeFood);
		REG_MEMBER(_bool,DontConsumerLivingPlaces);

		REG_MEMBER(_bool,ShowInsideUnits);
		REG_MEMBER(_bool,DontShowALT_Info);
		REG_MEMBER(_bool,NoBlending);
        REG_MEMBER(_bool,ShipRotate90);

		REG_ENUM(_index,ExtraConsumingResource,RESOURCES);
		REG_MEMBER(_int,ExtraConsumingResourceSpeed);
		REG_ENUM(_index,SelectionType,TEXTURE_CURSOR_TYPES);
		REG_ENUM(_index,SelectionTypeInFormation,TEXTURE_CURSOR_TYPES);
		REG_MEMBER(_float,SelectionScaleX);
		REG_MEMBER(_float,SelectionScaleY);
		REG_MEMBER(_int,SelectionShift);
		REG_MEMBER(_int,ColorVariation);

		REG_MEMBER(_bool,Immortal);
		REG_MEMBER(_bool,Incorporeal);

		REG_MEMBER(_int,AddShotRadius);

		REG_MEMBER(_int,InviteMask);

		REG_ENUM(_index,FastClickSoundID,ALL_SOUNDS);
		REG_ENUM(_index,AttackSoundID,ALL_SOUNDS);
		REG_ENUM(_index,SelectSoundID,ALL_SOUNDS);
		REG_ENUM(_index,BornSoundID,ALL_SOUNDS);
		REG_ENUM(_index,OrderSoundID,ALL_SOUNDS);
		REG_ENUM(_index,ReadySoundID,ALL_SOUNDS);
		REG_ENUM(_index,UpgradeSoundID,ALL_SOUNDS);
		REG_ENUM(_index,DeathSoundID,ALL_SOUNDS);
		REG_ENUM(_index,MotionOnRoadSound,ALL_SOUNDS);

		REG_MEMBER(_int,DamageOnBuildingMultiplicator);

		REG_AUTO(ExIcon);
		REG_AUTO(EmotIcons);
		REG_AUTO(ActiveBones);

		REG_MEMBER(_bool,UnitAbsorber);
		REG_MEMBER(_bool,PeasantAbsorber);
		REG_MEMBER(_WORD,MaxInside);
        INVISIBLE REG_MEMBER(_int,FreeAdd);
        INVISIBLE REG_MEMBER(_int,PeasantAdd);

		REG_MEMBER(_color,TFactor);
        
		REG_AUTO(NewParams);

	ENDSAVE;
};
class BasicUnitChars:public BasicObjectChars{
public:	

	int							Razbros;
	//ClonesArray<NewAnimation> Animations;
	ClonesArray<OneBoneInfo>	ActiveBones;
	ClonesArray<OneAttStateInfo> AttackTypes;	
	int							SrcZPoint;
	int							DstZPoint;
	int							RedRadius;
	int							YellowRadius;

	bool	CanStorm;
	int		VisitorMask;

	SAVE(BasicUnitChars){
		REG_PARENT(BasicObjectChars);
		REG_MEMBER(_int,Razbros);
		//REG_AUTO(Animations);
		REG_AUTO(ActiveBones);
		REG_AUTO(AttackTypes);		
		REG_MEMBER(_int,SrcZPoint);
		REG_MEMBER(_int,DstZPoint);
		REG_MEMBER(_int,RedRadius);
		REG_MEMBER(_int,YellowRadius);

		REG_MEMBER(_bool,CanStorm);
		REG_MEMBER(_int,VisitorMask);

	}ENDSAVE;

	virtual void  ConvertToNewMonster   (NewMonster* NM);
	virtual void  ConvertFromNewMonster (NewMonster* NM);
};
class UnitChars:public BasicUnitChars{
public:
	int							MotionStyle;
	int							UnitRadius;
	int							Radius2;
	int							CollisionRadius;	
	int    						UnitSpeed;
	int							RotationSpeed;
	int							StartFlyHeight;
	int							FlyHeight;
	//flags
	bool						LowCollision;
	bool						NoCollision;
	bool						Officer;
	bool						Drummer;
	bool						Peasant;
	bool						Transport;
	bool						Priest;
	bool						Shaman;
	bool						BornBehindBuilding;
	bool						DontRotateOnDeath;	
	bool						DontStuckInEnemy;
	bool						NikakixMam;
	bool						HighUnit;
	bool						Animal;
	bool						Cossacks2AttackStyle;
	bool						Cossacks2RechargeStyle;
	bool						UnitCanShoot;
	bool						CanSitInFormation;
	bool						DontTransformToChargeState;
	
	int							SpeedScale;
	int							SpeedScaleOnTrees;

	int							RadiusOfArmAttack;
	bool						ArmAttack;
	int							UnitsColumnIndex;
	ClassPtr<ComplexUnitRecord> Res;
	int							MaxResPortion[6];

    _str						ComingInPoints;
    _str						PositionsOfUnits;
    _str						GoingOutPoins;
    _str						GoingOutPoins2;
	
	virtual void  ConvertToNewMonster   (NewMonster* NM);
	virtual void  ConvertFromNewMonster (NewMonster* NM);

	SAVE(UnitChars);
		REG_PARENT(BasicUnitChars);
		REG_BASE(BasicObjectChars);
		REG_ENUM(_index,MotionStyle,MOTIONSTYLE);
		REG_MEMBER(_int,UnitSpeed);
		REG_MEMBER(_int,UnitRadius);
		REG_MEMBER(_int,Radius2);
		REG_MEMBER(_int,CollisionRadius);
		REG_MEMBER(_bool,LowCollision);
		REG_MEMBER(_bool,NoCollision);
		REG_MEMBER(_int,RotationSpeed);
		REG_MEMBER(_int,StartFlyHeight);
		REG_MEMBER(_int,FlyHeight);
		//flags
		REG_MEMBER(_bool,Officer);
		REG_MEMBER(_bool,Drummer);
		REG_MEMBER(_bool,Peasant);
		REG_MEMBER(_bool,Transport);
		REG_MEMBER(_bool,Priest);
		REG_MEMBER(_bool,Shaman);
		REG_MEMBER(_bool,BornBehindBuilding);
		REG_MEMBER(_bool,DontRotateOnDeath);	
		REG_MEMBER(_bool,DontStuckInEnemy);
		REG_MEMBER(_bool,NikakixMam);
		REG_MEMBER(_bool,HighUnit);
		REG_MEMBER(_bool,Animal);
		REG_MEMBER(_bool,Cossacks2AttackStyle);
		REG_MEMBER(_bool,Cossacks2RechargeStyle);
		REG_MEMBER(_bool,UnitCanShoot);
		REG_MEMBER(_bool,CanSitInFormation);
		REG_MEMBER(_bool,DontTransformToChargeState);
		
		REG_MEMBER(_int,SpeedScale);
		REG_MEMBER(_int,SpeedScaleOnTrees);

		REG_MEMBER(_int,RadiusOfArmAttack);
		REG_MEMBER(_bool,ArmAttack);
		REG_MEMBER(_int,UnitsColumnIndex);

		REG_CLASS(ComplexUnitRecord);
		REG_AUTO(Res);
		REG_MEMBER(_int,MaxResPortion[0]);
		REG_MEMBER(_int,MaxResPortion[1]);
		REG_MEMBER(_int,MaxResPortion[2]);
		REG_MEMBER(_int,MaxResPortion[3]);
		REG_MEMBER(_int,MaxResPortion[4]);
		REG_MEMBER(_int,MaxResPortion[5]);
        INVISIBLE REG_AUTO(ComingInPoints);
        INVISIBLE REG_AUTO(PositionsOfUnits);
        INVISIBLE REG_AUTO(GoingOutPoins);
        INVISIBLE REG_AUTO(GoingOutPoins2);

	ENDSAVE;
};
class BuildingChars:public BasicUnitChars{
public:
	int							PictureCenterX;
	int							PictureCenterY;
	//lockpoints
	_str						Lockpoints;
	_str						LockpointsDuringBuildStages;
	_str						CheckPoints;
	//other "points"
	_str						BuildPoints;
	_str						ComingInPoints;
	_str						PositionsOfUnits;
	_str						GoingOutPoins;
	_str						GoingOutPoins2;
	_str						FirePoints;
	_str						SmokePoints;
	ClassArray<ComplexFireSource> MultiWp;     
	int							MineRadius;
	int	 						MineDamage;
	int 						BuildNearBuildingRadius;
	bool						UnitsCanEnter;
	bool						PeasantsCanEnter;
    bool                        CanBeUsedLikeStorage;
	DWORD                       StorageMask;
    bool						Port;
	bool						Wall;
	bool						Farm;
	byte						NInFarm;
	bool						SpriteObject;
	bool						Market;
	bool						CommandCenter;
	bool						GlobalCommandCenter;
	bool						HighUnitCantEnter;
    bool                        UseLikeGate;

	bool ArtDepo;
	byte ArtCap[MaxArtDep];
	byte ArtSet;
	byte NInArtDepot;

	SAVE(BuildingChars);
		REG_PARENT(BasicUnitChars);
		REG_BASE(BasicObjectChars);
		//lockpoints
		INVISIBLE REG_AUTO(Lockpoints);
		INVISIBLE REG_AUTO(LockpointsDuringBuildStages);
		INVISIBLE REG_AUTO(CheckPoints);
		//other "points"
		INVISIBLE REG_AUTO(BuildPoints);
		INVISIBLE REG_AUTO(ComingInPoints);
		INVISIBLE REG_AUTO(PositionsOfUnits);
		INVISIBLE REG_AUTO(GoingOutPoins);
		INVISIBLE REG_AUTO(GoingOutPoins2);
		INVISIBLE REG_AUTO(FirePoints);
		INVISIBLE REG_AUTO(SmokePoints);
		REG_AUTO(MultiWp);
		REG_MEMBER(_int,MineRadius);
		REG_MEMBER(_int,MineDamage);
		REG_MEMBER(_int,BuildNearBuildingRadius);
		REG_MEMBER(_bool,UnitsCanEnter);
		REG_MEMBER(_bool,PeasantsCanEnter);
		REG_MEMBER(_bool,CanBeUsedLikeStorage);
		REG_MEMBER(_DWORD,StorageMask);
		REG_MEMBER(_bool,Port);
		REG_MEMBER(_bool,Wall);
		REG_MEMBER(_bool,Farm);
		REG_MEMBER(_BYTE,NInFarm);
		REG_MEMBER(_bool,SpriteObject);
		REG_MEMBER(_bool,Wall);
		REG_MEMBER(_bool,Market);
		REG_MEMBER(_bool,CommandCenter);
		REG_MEMBER(_bool,GlobalCommandCenter);
		REG_MEMBER(_bool,HighUnitCantEnter);
        REG_MEMBER(_bool,UseLikeGate);
		REG_MEMBER(_bool,ArtDepo);
		for(int i=0;i<MaxArtDep;i++){
			//DString txt;
			//txt="ArtCap";
			//txt.Add(i);
			//REG_MEMBER_EX(_BYTE,ArtCap[i],txt.pchar());
			REG_MEMBER(_BYTE,ArtCap[i]);
		}
		REG_MEMBER(_BYTE,ArtSet);
		REG_MEMBER(_BYTE,NInArtDepot);
	ENDSAVE;

	virtual void  ConvertToNewMonster   (NewMonster* NM);
	virtual void  ConvertFromNewMonster (NewMonster* NM);

};
class ComplexObjectChar:public BasicObjectChars{
public:
	int ComplexObjectName;
	int MatherialMaskForComplexObject;
	virtual void  ConvertToNewMonster   (NewMonster* NM);
	virtual void  ConvertFromNewMonster (NewMonster* NM);
	SAVE(ComplexObjectChar);
		REG_PARENT(BasicObjectChars);
		REG_ENUM(_index,ComplexObjectName,"COMPLEXOBJECTS");
		REG_MEMBER(_int,MatherialMaskForComplexObject);
	ENDSAVE;
};
//===========  NATION REGISTARTION ===============//
class OneUnitOfNation;
class OneUnitToProduce:public BaseClass{
public:
    ClassRef<OneUnitOfNation> Unit;
	int IconPosX;
	int IconPosY;
};
class OneUpgradeOfNation:public BaseClass{
public:
	_str			Name;
	_str			Hint;    
	short			IconGP_file;
	int				IconSprite;
	int				IconX;
	int				IconY;
	//int				UpgradeType;
	SAVE(OneUpgradeOfNation);
	ENDSAVE;
};
class OneUnitOfNation:public ReferableBaseClass{
public:
	int NIndex;
	ClassPtr<BasicObjectChars>					Unit;
	ClonesArray<OneUnitToProduce>				CanProduce;
    int											MaxAmountOfSpecificUnitType;
	ClassRef<OneUnitOfNation>					SpecificUnitType;
	bool										Enabled;
	bool										CanSetDestination;    
	ClassArray<ClassRef<OneUnitOfNation> >		RequiredUnitsToEnable;
	ClassArray<ClassRef<OneUpgradeOfNation> >	RequiredUpgradesDoneToEnable;
	SAVE(OneUnitOfNation);
		REG_CLASS(OneUnitOfNation);
		REG_AUTO(Unit);
		REG_AUTO(CanProduce);
		REG_MEMBER(_int,MaxAmountOfSpecificUnitType);
		REG_AUTO(SpecificUnitType);
		REG_MEMBER(_bool,Enabled);
		REG_MEMBER(_bool,CanSetDestination);
		REG_AUTO(RequiredUnitsToEnable);
		REG_AUTO(RequiredUpgradesDoneToEnable);
	ENDSAVE;
};
class OneUnitsSection:public BaseClass{
public:
	_str SectionName;
	ClassArray<BasicObjectChars> Units;
	SAVE(OneUnitsSection);
		REG_AUTO(SectionName);
		REG_AUTO(Units);
	ENDSAVE;
};
class NationParams:public BaseClass{
public:
	_str NationName;
	_str NationalSuffix;
    ClonesArray<OneUnitsSection> UnitsList;
	SAVE(NationParams);
		REG_CLASS(OneUnitsSection);
		REG_AUTO(NationName);
		REG_AUTO(UnitsList);
	ENDSAVE;
};

//////////////////////////////////////////////////////////////////////////
///////////////         OneObject modification        ////////////////////
//////////////////////////////////////////////////////////////////////////