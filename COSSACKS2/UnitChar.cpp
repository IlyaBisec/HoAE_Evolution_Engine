#include "stdheader.h"
#include "unitability.h"
#include "UnitChar.h"
#include "ClassEditorsRegistration.h"
#include "GameExtension.h"

void ModifyOneObject(OneObject* OB);
char* newstr(char* s);

HeroVariableStorage* DetectHero(OneObject* OB);

AblName::AblName(){
	ItemDrop=10;
	CommonRoomStartParam=0;
};
int nextint(char** s){
    if((*s)[0]==' '){
        while((*s)[0]==' ')(*s)++;
    }
    int v=atoi(*s);
    if((*s)[0]!=0){
        while( (*s)[0]!=' ' && (*s)[0]!=0)(*s)++;
    }
    return v;
}
void ReadXY(_str& s,word& N,char** x,char** y){
    if(s.pchar()&&s.pchar()[0]){
        char* str=s.pchar();
        int n=nextint(&str);
        *x=znew(char,n);
        *y=znew(char,n);
        for(int i=0;i<n;i++){
            (*x)[i]=nextint(&str);
            (*y)[i]=nextint(&str);
        }
        N=n;
    }
}
void ReadXY(_str& s,short& N,short** x,short** y){
    if(s.pchar()&&s.pchar()[0]){
        char* str=s.pchar();
        int n=nextint(&str);
        *x=znew(short,n);
        *y=znew(short,n);
        for(int i=0;i<n;i++){
            (*x)[i]=nextint(&str);
            (*y)[i]=nextint(&str);
        }
        N=n;
    }
}
void ReadXY(_str& s,DynArray<short>* x,DynArray<short>* y){
    if(s.pchar()&&s.pchar()[0]){
        char* str=s.pchar();
        int n=nextint(&str);
        for(int i=0;i<n;i++){
            (*x).Add(nextint(&str));
            (*y).Add(nextint(&str));
        }
    }
}
void ReadXYZ(_str& s,DynArray<short>* x,DynArray<short>* y,DynArray<short>* z){
    if(s.pchar()&&s.pchar()[0]){
        char* str=s.pchar();
        int n=nextint(&str);
        for(int i=0;i<n;i++){
            (*x).Add(nextint(&str));
            (*y).Add(nextint(&str));
            (*z).Add(nextint(&str));
        }
    }
}
const char* AblName::GetElementView(int Index,const char* LocalName){
	char* pVIEW = GetGlobalBuffer();
	_str PREFIX = "";
	if ( LocalName!=NULL && LocalName[0]!=0 ){
		if			(strcmp(LocalName,"be_Process")==0) {
			PREFIX = "{C ff00FF00}PROC:\n";
		}else if	(strcmp(LocalName,"be_LeftClick")==0) {
			PREFIX = "{C ff00FF00}LCLICK:\n";
		}else if	(strcmp(LocalName,"be_EnterDialog")==0) {
			PREFIX = "{C ff00FF00}ENTER:\n";
		};
	};
	PREFIX += BaseClass::GetElementView(Index,LocalName);
	sprintf(pVIEW,"%s",PREFIX.pchar());
	return pVIEW;
};
template <class X>
void restore_vftbl(BaseClass* BC){
	X* x=new X;
	memcpy(BC,x,4);
	delete(x);
}

void  BasicObjectChars::ConvertFromNewMonster (NewMonster* NM){
	FormFactor=NM->FormFactor;
	Building=NM->Building;
	UnitMD=NM->MD_File;
	if(UnitMD.pchar())strupr(UnitMD.pchar());
	UnitID=MonNames[NM->MyIndex];
	IconFileID=NM->IconFileID;
	IconID=NM->IconID;
	Hitpoints=NM->Life;
	ProduceStages=NM->ProduceStages;
	DontShowALT_Info=NM->DontShowALT_Info;
	AttackSpeed=NM->AttackSpeed;
	Speed=NM->Speed;
	for(int i=0;i<8;i++){
		Cost[i]=NM->NeedRes[i];
		KillerAward[i]=NM->KillerAward[i];
		KillerAwardRandom[i]=NM->KillerAwardRandom[i];
	}
	for(int i=0;i<16;i++)Protection[i]=NM->Protection[i];
	MatherialMask=NM->MathMask;
	KillingMask=NM->KillMask;
	VisionRange=NM->VisionType;
	LockType=NM->LockType;
	if(NM->Ability){
		MonsterAbility* MA=NM->Ability;
		for(int i=0;i<MA->AbilityNames.GetAmount();i++){
			AblName* AN=new AblName;
			AN->AbilityName=(MA->AbilityNames[i])->pchar();

			AN->ItemDrop=MA->ItemDrop[i];
			AN->CommonRoomStartParam=MA->CommonRoomStartParam[i];

			if(i<MA->Names.GetAmount()){
				AN->Name=MA->Names[i]->pchar();
				AN->PicFile=MA->PicFiles[i];
				AN->PicSprite=MA->PicSprites[i];
				AN->Hint=MA->Hints[i]->pchar();

				AN->DeleteAfterUse=MA->DeleteAfterUse[i];

				MA->be_Processes[i]->Copy(&AN->be_Process,false);
				MA->be_LeftClicks[i]->Copy(&AN->be_LeftClick,false);
				MA->be_EnterDialogs[i]->Copy(&AN->be_EnterDialog,false);
			}

			Ability.Add(AN);
		}
	}
	Message=NM->Message;
	LongMessage=NM->LongMessage;
	PieceName=NM->PieceName;
	MaxMana=NM->MaxMana;
	ExpForKillingThisUnit=NM->Expa;
	DontAnswerOnAttack=NM->DontAnswerOnAttack;
	DontAffectFogOfWar=NM->DontAffectFogOfWar;
	InvisibleOnMinimap=NM->InvisibleOnMinimap;
	Like2DOnMinimap=NM->Like2DOnMinimap;
	CanBeInFocusOfFormation=NM->CanBeInFocusOfFormation;
	CanBeCaptured=NM->Capture;
	CantCapture=NM->CantCapture;
	NeverCapture=NM->NeverCapture;
	ShowAttackDelay=NM->ShowDelay;
	NotSelectable=NM->NotSelectable;
	RectDx=NM->RectDx;
	RectDy=NM->RectDy;
	RectLx=NM->RectLx;
	RectLy=NM->RectLy;
	UnitCylinderRadius=NM->UnitCylinderRadius;
	UnitCylinderHeight=NM->UnitCylinderHeight;
	DontConsumerLivingPlaces=NM->NoFarm;
	ExtraConsumingResource=NM->ResConsID;
	ExtraConsumingResourceSpeed=NM->ResConsumer;
	SelectionType=NM->selIndex;
	SelectionTypeInFormation=NM->selIndexBR;
	SelectionScaleX=NM->selScaleX;
	SelectionScaleY=NM->selScaleY;
	SelectionShift=NM->selShift;
	ColorVariation=NM->ColorVariation;
	SearchEnemyRadius=NM->VisRange>>4;
	Scale=float(NM->StartScale)/256.0f;
    NoBlending=NM->NoBlending;
    FreeAdd=NM->FreeAdd;
    PeasantAdd=NM->PeasantAdd;

	TFactor=NM->TFactor;

	Immortal=NM->Immortal;
	Incorporeal=NM->Incorporeal;

	AddShotRadius=NM->AddShotRadius;

	InviteMask=NM->InviteMask;

	DamageOnBuildingMultiplicator=NM->DamageOnBuildingMultiplicator;

	FastClickSoundID=NM->FastClickSoundID;
	AttackSoundID=NM->AttackSoundID;
	SelectSoundID=NM->SelectSoundID;
	BornSoundID=NM->BornSoundID;
	OrderSoundID=NM->OrderSoundID;
	ReadySoundID=NM->ReadySoundID;
	UpgradeSoundID=NM->UpgradeSoundID;
	DeathSoundID=NM->DeathSoundID;	
	MotionOnRoadSound=NM->MotionOnRoadSound;
	ShowInsideUnits=NM->ShowInsideUnits;

	//restore_vftbl<ExtendedIcon>(&NM->ExIcon);
	restore_vftbl<ClonesArray<OneEmotion> >(&NM->EmotIcons);
	restore_vftbl<ClonesArray<OneBoneInfo> >(&NM->ActiveBones);

	//NM->ExIcon.Copy(&ExIcon,false);
	NM->EmotIcons.Copy(&EmotIcons,false);
	NM->ActiveBones.Copy(&ActiveBones,false);

	UnitAbsorber=NM->UnitAbsorber;
	PeasantAbsorber=NM->PeasantAbsorber;
	MaxInside=NM->MaxInside;

    restore_vftbl<CommonParameters>(&NM->NewParams);
    NM->NewParams.Copy(&NewParams);

    ShipRotate90 = NM->MultiWp.GetAmount()!=0;
}
void  BasicObjectChars::ConvertToNewMonster   (NewMonster* NM){
	NM->Animations=NewMon[NM->MyIndex]->Animations;
	NM->FormFactor=FormFactor;
	NM->Building=Building;
	NM->MD_File=newstr(UnitMD.pchar());
	if(NM->MD_File)strupr(NM->MD_File);
	//if(!NM->CharacterWasModified){
	//	if(MonNames[NM->MyIndex])free(MonNames[NM->MyIndex]);
	//	MonNames[NM->MyIndex]=newstr(UnitID.pchar());
	//}
	NM->IconFileID=IconFileID;
	NM->IconID=IconID;
	NM->Life=Hitpoints;
	NM->ProduceStages=ProduceStages;
	NM->DontShowALT_Info=DontShowALT_Info;
	NM->AttackSpeed=AttackSpeed;
	NM->Speed=Speed;
	for(int i=0;i<8;i++){
		NM->NeedRes[i]=Cost[i];
		NM->KillerAward[i]=KillerAward[i];
		NM->KillerAwardRandom[i]=KillerAwardRandom[i];
	}
	for(int i=0;i<16;i++)NM->Protection[i]=Protection[i];
	NM->MathMask=MatherialMask;
	NM->KillMask=KillingMask;
	NM->VisionType=VisionRange;
	NM->LockType=LockType;
	if(Ability.GetAmount()){
		MonsterAbility* MA=new MonsterAbility;
		for(int i=0;i<Ability.GetAmount();i++){
			AblName* AN=Ability[i];

			//_str* s=new _str;
			//*s=AN->AbilityName;
			MA->AbilityNames.Add(new _str(AN->AbilityName));

			MA->ItemDrop.Add(AN->ItemDrop);
			MA->CommonRoomStartParam.Add(AN->CommonRoomStartParam);
			
			//ClassRef<be_CSMARTOBJECT>* SO=new ClassRef<be_CSMARTOBJECT>;
			//ClassRef<be_CMSG_FROMUSER>* SC= new ClassRef<be_CMSG_FROMUSER>;

			//be_CSO_REF* SO=new be_CSO_REF;
			//be_CMSG_REF* SC=new be_CMSG_REF;
			//SO->Object.SetRef( AN->SmartObject.Object.pchar(), 0xffffffff );
			//SC->Message.SetRef( AN->SmartCommand.Message.pchar(), 0xffffffff );
			//MA->SmartObjects.Add( SO );
			//MA->SmartCommands.Add( SC );

			//

			_str* s = new _str;
			s->Assign(AN->Name.pchar());
			MA->Names.Add(s);

			s = new _str;
			s->Assign(AN->Hint);
			MA->Hints.Add(s);

			MA->PicFiles.Add(AN->PicFile);
			MA->PicSprites.Add(AN->PicSprite);

			MA->DeleteAfterUse.Add(AN->DeleteAfterUse);

			//

			be_CRefParamFunction* beP=new be_CRefParamFunction;
			AN->be_Process.Copy(beP,false);
			MA->be_Processes.Add(beP);

			be_CRefParamFunction* beLC=new be_CRefParamFunction;
			AN->be_LeftClick.Copy(beLC,false);
			MA->be_LeftClicks.Add(beLC);

			be_CRefParamFunction* beED=new be_CRefParamFunction;
			AN->be_EnterDialog.Copy(beED,false);
			MA->be_EnterDialogs.Add(beED);			

		}
		NM->Ability=MA;
	}
	NM->Message=newstr(Message.pchar());
	NM->LongMessage=newstr(LongMessage.pchar());
	NM->PieceName=newstr(PieceName.pchar());
	NM->MaxMana=MaxMana;
	NM->Expa=ExpForKillingThisUnit;
	NM->DontAnswerOnAttack=DontAnswerOnAttack;
	NM->DontAffectFogOfWar=DontAffectFogOfWar;
	NM->InvisibleOnMinimap=InvisibleOnMinimap;
	NM->Like2DOnMinimap=Like2DOnMinimap;
	NM->CanBeInFocusOfFormation=CanBeInFocusOfFormation;
	NM->NotSelectable=NotSelectable;
	NM->Capture=CanBeCaptured;
	NM->CantCapture=CantCapture;
	NM->NeverCapture=NeverCapture;
	NM->ShowDelay=ShowAttackDelay;
	NM->RectDx=RectDx;
	NM->RectDy=RectDy;
	NM->RectLx=RectLx;
	NM->RectLy=RectLy;
	NM->UnitCylinderRadius=UnitCylinderRadius;
	NM->UnitCylinderHeight=UnitCylinderHeight;
	NM->NoFarm=DontConsumerLivingPlaces;
	NM->ResConsID=ExtraConsumingResource;
	NM->ResConsumer=ExtraConsumingResourceSpeed;
	NM->selIndex=SelectionType;
	NM->selIndexBR=SelectionTypeInFormation;
	NM->selScaleX=SelectionScaleX;
	NM->selScaleY=SelectionScaleY;
	NM->selShift=SelectionShift;
	NM->ColorVariation=ColorVariation;
	NM->VisRange=SearchEnemyRadius<<4;
	NM->StartScale=floorf(Scale*256.0f+0.5);
    NM->NoBlending=NoBlending;

	NM->Immortal=Immortal;
	NM->Incorporeal=Incorporeal;

	NM->AddShotRadius=AddShotRadius;

	NM->InviteMask=InviteMask;

	NM->DamageOnBuildingMultiplicator=DamageOnBuildingMultiplicator;

	NewMonster* BaseNM=NewMon[NM->MyIndex];
	if(BaseNM->NShotPt){
		int p1=BaseNM->NShotPt;
		NM->NShotPt=p1;
		NM->ShotPtX=znew(short,p1);
		NM->ShotPtY=znew(short,p1);
		if(BaseNM->ShotPtYG){
			NM->ShotPtYG=znew(short,p1);
			NM->ShotPtZ=znew(short,p1);
			NM->ShotDelay=znew(short,p1);
			NM->ShotAttType=znew(byte,p1);
			NM->ShotDir=znew(byte,p1);
			NM->ShotDiff=znew(byte,p1);
			NM->ShotMinR=znew(word,p1);
			NM->ShotMaxR=znew(word,p1);
		}
		for(int i=0;i<p1;i++){
			NM->ShotPtX[i]=BaseNM->ShotPtX[i];
			NM->ShotPtY[i]=BaseNM->ShotPtY[i];
			if(BaseNM->ShotPtYG){
				NM->ShotPtYG[i]=BaseNM->ShotPtYG[i];
				NM->ShotPtZ[i]=BaseNM->ShotPtZ[i];
				NM->ShotMinR[i]=BaseNM->ShotMinR[i];
				NM->ShotMaxR[i]=BaseNM->ShotMaxR[i];
				NM->ShotDir[i]=BaseNM->ShotDir[i];
				NM->ShotDiff[i]=BaseNM->ShotDiff[i];
				NM->ShotDelay[i]=BaseNM->ShotDelay[i];
				NM->ShotAttType[i]=BaseNM->ShotAttType[i];
			}
		};
	}
	NM->FastClickSoundID=FastClickSoundID;
	NM->AttackSoundID=AttackSoundID;
	NM->SelectSoundID=SelectSoundID;
	NM->BornSoundID=BornSoundID;
	NM->OrderSoundID=OrderSoundID;
	NM->ReadySoundID=ReadySoundID;
	NM->UpgradeSoundID=UpgradeSoundID;
	NM->DeathSoundID=DeathSoundID;	
	NM->MotionOnRoadSound=MotionOnRoadSound;
	NM->ShowInsideUnits=ShowInsideUnits;

	//restore_vftbl<ExtendedIcon>(&NM->ExIcon);
	restore_vftbl<ClonesArray<OneEmotion> >(&NM->EmotIcons);
	restore_vftbl<ClonesArray<OneBoneInfo> >(&NM->ActiveBones);

	//ExIcon.Copy(&NM->ExIcon,false);
	EmotIcons.Copy(&NM->EmotIcons,false);
	ActiveBones.Copy(&NM->ActiveBones,false);
	
	NM->UnitAbsorber=UnitAbsorber;
	NM->PeasantAbsorber=PeasantAbsorber;
	NM->MaxInside=MaxInside;

    NM->FreeAdd=FreeAdd;
    NM->PeasantAdd=PeasantAdd;

	NM->TFactor=TFactor;

    restore_vftbl<CommonParameters>(&NM->NewParams);
    NewParams.Copy(&NM->NewParams);

    if(ShipRotate90){
        ComplexFireSource* CFS=new ComplexFireSource;
        CFS->x=0;
        CFS->y=0;
        CFS->z=20;        
        CFS->Dir=64;
        CFS->DirDiff=2;
        CFS->AttTypeMask=0;
        NM->MultiWp.Add(CFS);
        CFS=new ComplexFireSource;
        CFS->x=0;
        CFS->y=0;
        CFS->z=20;
        word GetDir(int dx,int dy);
        CFS->Dir=64;
        CFS->DirDiff=2;
        CFS->AttTypeMask=0;
        NM->MultiWp.Add(CFS);
    }
}
void BasicUnitChars::ConvertFromNewMonster(NewMonster* NM){	
	Razbros=NM->Razbros;	
	/*
	for(int i=0;i<NM->Animations->ANM.GetAmount();i++){
		NewAnimation* NA=new NewAnimation;
		NM->Animations->ANM[i]->Copy(NA,false);
		Animations->Add(NA);
	}
	*/
	int na=0;
	for(int i=0;i<NAttTypes;i++){
		if(NM->MaxDamage[i]||NM->AttackRadius1[i]||NM->AttackRadius2[i]||NM->DamWeap[i])na=i+1;			
	}
	Enumerator* WE=ENUM.Get("WEAPONS");
	for(int i=0;i<na;i++){
		OneAttStateInfo* AT=new OneAttStateInfo;
		AT->MinAttackRadius=NM->AttackRadius1[i];
		AT->MaxAttackRadius=NM->AttackRadius2[i];
		AT->MinDetRadius=NM->DetRadius1[i];
		AT->MaxDetRadius=NM->DetRadius2[i];		
		AT->Weapon=WE->Get((DWORD)NM->DamWeap[i]);
		AT->MotionRate=NM->Rate[i];
		AT->AttackPause=NM->AttackPause[i];
		AT->Damage=NM->MaxDamage[i];
		AT->WeaponKind=NM->WeaponKind[i];
		AT->DamageDecrementRadius=NM->DamageDecr[i];
		AT->AttackMask=NM->AttackMask[i];
		AT->FearType=NM->FearType[i];
		AT->FearRadius=NM->FearRadius[i];
		AT->NoPausedAttack=(NM->NoWaitMask&(1<<i))!=0;
		AttackTypes.Add(AT);
	}			
	SrcZPoint=NM->SrcZPoint;
	DstZPoint=NM->DstZPoint;
	RedRadius=NM->VisibleRadius1;
	YellowRadius=NM->VisibleRadius2;

	CanStorm=NM->CanStorm;
	VisitorMask=NM->VisitorMask;
}
void BasicUnitChars::ConvertToNewMonster(NewMonster* NM){	
	BasicObjectChars::ConvertToNewMonster(NM);
	NM->Razbros=Razbros;	
	/*
	for(int i=0;i<Animations->GetAmount();i++){
		NewAnimation* N=new NewAnimation;
		Animations[i]->Copy(N,false);
		if(N->AnimationType==0 || N->AnimationType==3)N->Scale=0.8;
		NM->Animations->Add(N,N->Code);
	}
	*/
	Enumerator* WE=ENUM.Get("WEAPONS");
	NM->NoWaitMask=0;
	for(int i=0;i<AttackTypes.GetAmount();i++){
		OneAttStateInfo* AT=AttackTypes[i];
		NM->AttackRadius1[i]=AT->MinAttackRadius;
		NM->AttackRadius2[i]=AT->MaxAttackRadius;
		NM->DetRadius1[i]=AT->MinDetRadius;
		NM->DetRadius2[i]=AT->MaxDetRadius;
		NM->DamWeap[i]=NULL;
		DWORD W=WE->Get(AT->Weapon.pchar()?AT->Weapon.pchar():"");
		if(W!=0xFFFFFFFF)NM->DamWeap[i]=(Weapon*)W;
		else NM->DamWeap[i]=NULL;
		NM->Rate[i]=AT->MotionRate;
		NM->AttackPause[i]=AT->AttackPause;
		NM->MaxDamage[i]=AT->Damage;
		NM->WeaponKind[i]=AT->WeaponKind;
		NM->DamageDecr[i]=AT->DamageDecrementRadius;
		NM->AttackMask[i]=AT->AttackMask;
		NM->FearType[i]=AT->FearType;
		NM->FearRadius[i]=AT->FearRadius;		
		if(AT->NoPausedAttack)NM->NoWaitMask|=1<<i;
	}	
	NM->SrcZPoint=SrcZPoint;
	NM->DstZPoint=DstZPoint;
	NM->VisibleRadius1=RedRadius;
	NM->VisibleRadius2=YellowRadius;		

	NM->CanStorm=CanStorm;
	NM->VisitorMask=VisitorMask;
}
void UnitChars::ConvertFromNewMonster (NewMonster* NM){
	Building=false;
	MotionStyle=NM->MotionStyle;
	UnitSpeed=NM->MotionDist;
	UnitRadius=NM->UnitRadius;
	Radius2=NM->Radius2>>4;	
	RotationSpeed=NM->MinRotator;
	StartFlyHeight=NM->StartFlyHeight;
	FlyHeight=NM->FlyHeight;

	Officer=NM->Officer;
	Drummer=NM->Baraban;
	Peasant=NM->Peasant;
	Transport=NM->Transport;
	Priest=NM->Priest;
	Shaman=NM->Shaman;
	BornBehindBuilding=NM->BornBehindBuilding;
	DontRotateOnDeath=NM->DontRotateOnDeath;	
	DontStuckInEnemy=NM->DontStuckInEnemy;
	NikakixMam=NM->NikakixMam;
	HighUnit=NM->HighUnit;
	Animal=NM->Animal;
	UnitCanShoot=NM->CanShoot;
	UnitsColumnIndex=NM->UnitsColumnIndex;
	
	SpeedScale=(128+NM->SpeedScale*100)/256;
	SpeedScaleOnTrees=(128+NM->SpeedScaleOnTrees*100)/256;

	CanSitInFormation=NM->SitInFormations;
	DontTransformToChargeState=NM->DontTransformToChargeState;

	CollisionRadius=NM->CollisionRadius;
	LowCollision=NM->LowCollision;
	NoCollision=NM->NoCollision;

	RadiusOfArmAttack=NM->ArmRadius;
	ArmAttack=NM->ArmAttack;

    ComingInPoints="";
    ComingInPoints.print("%d  ",NM->ConcPtX.GetAmount());
    for(int i=0;i<NM->ConcPtX.GetAmount();i++){
        int z=0;
        if(i<NM->ConcPtZ.GetAmount()) z=NM->ConcPtZ[i];
        ComingInPoints.print("%d %d %d ",NM->ConcPtX[i],NM->ConcPtY[i],z);
    }

    PositionsOfUnits="";
    PositionsOfUnits.print("%d  ",NM->PosPtX.GetAmount());
    for(int i=0;i<NM->PosPtX.GetAmount();i++)PositionsOfUnits.print("%d %d ",NM->PosPtX[i],NM->PosPtY[i]);	

    GoingOutPoins="";
    GoingOutPoins.print("%d  ",NM->BornPtX.GetAmount());
    for(int i=0;i<NM->BornPtX.GetAmount();i++){
        int z=0;
        if(i<NM->BornPtZ.GetAmount()) z=NM->BornPtZ[i];
        GoingOutPoins.print("%d %d %d ",NM->BornPtX[i],NM->BornPtY[i],z);
    }

    GoingOutPoins2="";
    GoingOutPoins2.print("%d  ",NM->CraftPtX.GetAmount());
    for(int i=0;i<NM->CraftPtX.GetAmount();i++)GoingOutPoins2.print("%d %d ",NM->CraftPtX[i],NM->CraftPtY[i]);
	if(NM->CompxUnit){
        ComplexUnitRecord* CR=new ComplexUnitRecord;
		NM->CompxUnit->Copy(CR,false);
		Res.Set(CR);
	}
	for(int i=0;i<6;i++){
		MaxResPortion[i]=NM->MaxResPortion[i];
	}
}
void UnitChars::ConvertToNewMonster (NewMonster* NM){	
	BasicUnitChars::ConvertToNewMonster(NM);
	NM->Building=false;
	NM->MotionStyle=MotionStyle;
	NM->MotionDist=UnitSpeed;
	NM->UnitRadius=UnitRadius;
	NM->Radius2=Radius2<<4;
	NM->MinRotator=RotationSpeed;
	NM->StartFlyHeight=StartFlyHeight;
	NM->FlyHeight=FlyHeight;

	NM->Officer=Officer;
	NM->Baraban=Drummer;
	NM->Peasant=Peasant;
	NM->Transport=Transport;
	NM->Priest=Priest;
	NM->Shaman=Shaman;
	NM->BornBehindBuilding=BornBehindBuilding;
	NM->DontRotateOnDeath=DontRotateOnDeath;	
	NM->DontStuckInEnemy=DontStuckInEnemy;
	NM->NikakixMam=NikakixMam;
	NM->HighUnit=HighUnit;
	NM->Animal=Animal;
	NM->CanShoot=UnitCanShoot;
	NM->UnitsColumnIndex=UnitsColumnIndex;

	NM->SpeedScale=(50+SpeedScale*256)/100;
	NM->SpeedScaleOnTrees=(50+SpeedScaleOnTrees*256)/100;
	
	NM->SitInFormations=CanSitInFormation;
	NM->DontTransformToChargeState=DontTransformToChargeState;

	NM->CollisionRadius=CollisionRadius;
	NM->LowCollision=LowCollision;
	NM->NoCollision=NoCollision;

	NM->ArmRadius=RadiusOfArmAttack;
	NM->ArmAttack=ArmAttack;
	ComplexUnitRecord* CR=(ComplexUnitRecord*)Res.Get();
	if(CR){
		NM->CompxUnit=new ComplexUnitRecord;
		CR->Copy(NM->CompxUnit,false);
	}
	for(int i=0;i<6;i++){
		NM->MaxResPortion[i]=MaxResPortion[i];
	}
    ReadXYZ(ComingInPoints,&NM->ConcPtX,&NM->ConcPtY,&NM->ConcPtZ);
    ReadXY(PositionsOfUnits,&NM->PosPtX,&NM->PosPtY);
    ReadXYZ(GoingOutPoins,&NM->BornPtX,&NM->BornPtY,&NM->BornPtZ);
    ReadXY(GoingOutPoins2,&NM->CraftPtX,&NM->CraftPtY);    
}
void BuildingChars::ConvertFromNewMonster (NewMonster* NM){
	PictureCenterX=-NM->PicDx;
	PictureCenterY=-NM->PicDy;
	Building=true;
	Lockpoints="";
	Lockpoints.print("%d  ",NM->NLockPt);
	for(int i=0;i<NM->NLockPt;i++)Lockpoints.print("%d %d ",NM->LockX[i],NM->LockY[i]);

	LockpointsDuringBuildStages="";
	LockpointsDuringBuildStages.print("%d  ",NM->NBLockPt);
	for(int i=0;i<NM->NBLockPt;i++)LockpointsDuringBuildStages.print("%d %d ",NM->BLockX[i],NM->BLockY[i]);

	CheckPoints="";
	CheckPoints.print("%d  ",NM->NCheckPt);
	for(int i=0;i<NM->NCheckPt;i++)CheckPoints.print("%d %d ",NM->CheckX[i],NM->CheckY[i]);		
	
	BuildPoints="";
	BuildPoints.print("%d  ",NM->BuildPtX.GetAmount());
	for(int i=0;i<NM->BuildPtX.GetAmount();i++)BuildPoints.print("%d %d ",NM->BuildPtX[i],NM->BuildPtY[i]);		

	ComingInPoints="";
	ComingInPoints.print("%d  ",NM->ConcPtX.GetAmount());
	for(int i=0;i<NM->ConcPtX.GetAmount();i++){
		int z=0;
		if(i<NM->ConcPtZ.GetAmount()) z=NM->ConcPtZ[i];
		ComingInPoints.print("%d %d %d ",NM->ConcPtX[i],NM->ConcPtY[i],z);
	}

	PositionsOfUnits="";
	PositionsOfUnits.print("%d  ",NM->PosPtX.GetAmount());
	for(int i=0;i<NM->PosPtX.GetAmount();i++)PositionsOfUnits.print("%d %d ",NM->PosPtX[i],NM->PosPtY[i]);	

	GoingOutPoins="";
	GoingOutPoins.print("%d  ",NM->BornPtX.GetAmount());
	for(int i=0;i<NM->BornPtX.GetAmount();i++){
		int z=0;
		if(i<NM->BornPtZ.GetAmount()) z=NM->BornPtZ[i];
		GoingOutPoins.print("%d %d %d ",NM->BornPtX[i],NM->BornPtY[i],z);
	}

	GoingOutPoins2="";
	GoingOutPoins2.print("%d  ",NM->CraftPtX.GetAmount());
	for(int i=0;i<NM->CraftPtX.GetAmount();i++)GoingOutPoins2.print("%d %d ",NM->CraftPtX[i],NM->CraftPtY[i]);

	SmokePoints="";
	SmokePoints.print("%d  ",NM->NFires[0]);
	for(int i=0;i<NM->NFires[0];i++)SmokePoints.print("%d %d ",NM->FireX[0][i],NM->FireY[0][i]);

	FirePoints="";
	FirePoints.print("%d  ",NM->NFires[1]);
	for(int i=0;i<NM->NFires[1];i++)FirePoints.print("%d %d ",NM->FireX[1][i],NM->FireY[1][i]);

	//NM->MultiWp.Copy(&MultiWp);
	
	MineRadius=NM->MineRadius;
	MineDamage=NM->MineDamage;
	BuildNearBuildingRadius=NM->BuildNearBuildingRadius;
	UnitsCanEnter=NM->UnitAbsorber;
	PeasantsCanEnter=NM->PeasantAbsorber;
	HighUnitCantEnter=NM->HighUnitCantEnter;
	CanBeUsedLikeStorage=NM->Producer;
	StorageMask=NM->ProdType;
	Port=NM->Port;
	Wall=NM->Wall;
	Farm=NM->Farm;
	NInFarm=NM->NInFarm;
	SpriteObject=NM->SpriteObject;
	Market=NM->Rinok;
	CommandCenter=NM->CommandCenter;
	GlobalCommandCenter=NM->GlobalCommandCenter;	
    UseLikeGate=NM->UseLikeGate;
	ArtDepo=NM->ArtDepo;
	for(int i=0;i<MaxArtDep;i++){
		ArtCap[i]=NM->ArtCap[i];
	}
	ArtSet=NM->ArtSet;
	NInArtDepot=NM->NInArtDepot;
}


void BuildingChars::ConvertToNewMonster (NewMonster* NM){
	BasicUnitChars::ConvertToNewMonster(NM);
	NM->Building=true;
	NM->PicDx=-PictureCenterX;
	NM->PicDy=-PictureCenterY;
	ReadXY(Lockpoints,NM->NLockPt,&NM->LockX,&NM->LockY);
	ReadXY(LockpointsDuringBuildStages,NM->NBLockPt,&NM->BLockX,&NM->BLockY);
	ReadXY(CheckPoints,NM->NCheckPt,&NM->CheckX,&NM->CheckY);

	ReadXY(BuildPoints,&NM->BuildPtX,&NM->BuildPtY);
	ReadXYZ(ComingInPoints,&NM->ConcPtX,&NM->ConcPtY,&NM->ConcPtZ);
	ReadXY(PositionsOfUnits,&NM->PosPtX,&NM->PosPtY);
	ReadXYZ(GoingOutPoins,&NM->BornPtX,&NM->BornPtY,&NM->BornPtZ);
	ReadXY(GoingOutPoins2,&NM->CraftPtX,&NM->CraftPtY);

	ReadXY(FirePoints,NM->NFires[0],&NM->FireX[0],&NM->FireY[0]);
	ReadXY(SmokePoints,NM->NFires[1],&NM->FireX[1],&NM->FireY[1]);
	//MultiWp.Copy(&NM->MultiWp);

	NM->MineRadius=MineRadius;
	NM->MineDamage=MineDamage;
	NM->BuildNearBuildingRadius=BuildNearBuildingRadius;
	NM->UnitAbsorber=UnitsCanEnter;
	NM->PeasantAbsorber=PeasantsCanEnter;
	NM->HighUnitCantEnter=HighUnitCantEnter;
	NM->Producer=CanBeUsedLikeStorage;
	NM->ProdType=StorageMask;
	NM->Port=Port;
	NM->Wall=Wall;
	NM->Farm=Farm;
	NM->NInFarm=NInFarm;
	NM->SpriteObject=SpriteObject;
	NM->Rinok=Market;
	NM->CommandCenter=CommandCenter;
	NM->GlobalCommandCenter=GlobalCommandCenter;	
	NM->TFactor=TFactor;
    NM->UseLikeGate=UseLikeGate;
	NM->ArtDepo=ArtDepo;
	for(int i=0;i<MaxArtDep;i++){
		NM->ArtCap[i]=ArtCap[i];
	}
	NM->ArtSet=ArtSet;
	NM->NInArtDepot=NInArtDepot;
}
void ComplexObjectChar::ConvertToNewMonster(NewMonster* NM){
	BasicObjectChars::ConvertToNewMonster(NM);
    NM->ComplexObjIndex=ComplexObjectName;
	NM->CO_MathMask=MatherialMaskForComplexObject;
}
void ComplexObjectChar::ConvertFromNewMonster(NewMonster* NM){
	ComplexObjectName=NM->ComplexObjIndex;
	MatherialMaskForComplexObject=NM->CO_MathMask;
}
BasicObjectChars* CreateCharacterFromNewMonster(NewMonster* NM){
	BasicObjectChars* C=NULL;
	if(NM->ComplexObjIndex!=0xFFFF){
		ComplexObjectChar* COC=new ComplexObjectChar;
		COC->BasicObjectChars::ConvertFromNewMonster(NM);
		COC->ConvertFromNewMonster(NM);
		C=COC;
	}else{
		if(NM->Building){
			BuildingChars* BC=new BuildingChars;
            BC->BasicObjectChars::ConvertFromNewMonster(NM);			
			BC->BasicUnitChars::ConvertFromNewMonster(NM);
			BC->ConvertFromNewMonster(NM);
			C=BC;
		}else{
            UnitChars* UC=new UnitChars;
			UC->BasicObjectChars::ConvertFromNewMonster(NM);
			UC->BasicUnitChars::ConvertFromNewMonster(NM);
			UC->ConvertFromNewMonster(NM);
			C=UC;
		}
	}
	return C;
}
ClassArray<BasicObjectChars> AllUnitsChars;
void InitClasses(){
	REG_CLASS(OneAttStateInfo);
	REG_CLASS(BasicObjectChars);
	REG_CLASS(BasicUnitChars);
	REG_CLASS(UnitChars);
	REG_CLASS(BuildingChars);
	REG_CLASS(ComplexObjectChar);

	REG_CLASS(AblName);
	REG_CLASS(AblNameArray);

	REG_CLASS(OneEmotion);
	REG_CLASS(OneBoneInfo);

	REG_CLASS(OneExIcon);
	REG_CLASS(OneSpriteExIcon);
	REG_CLASS(OneSpriteExIconSet);
	REG_CLASS(OneSpriteExIcon3D);
	REG_CLASS(ClassArray<OneSpriteExIcon>);
}
void CreateAllChars(){	
    AllUnitsChars.Clear();
	for(int i=0;i<NNewMon;i++){
		BasicObjectChars* BOC=CreateCharacterFromNewMonster(NewMon[i]);
		AllUnitsChars.Add(BOC);
	}
}
bool rce_CharsCallback(ClassEditor* CE,BaseClass* BC,int Options){//Options=1-init, 2-process, 3-ok pressed, 4-cancel pressed 
	if(Options==1){
		CreateAllChars();		
	}
	if(Options==3){
		AllUnitsChars.WriteToFile("allunits.xml");
	}
	return false;
}
extern NewMonster* NewMon[2048];
void LoadAllUnitsFromXML(){
	InitClasses();
	AllUnitsChars.SafeReadFromFile("allunits.xml");
    NNewMon=AllUnitsChars.GetAmount();
	for(int i=0;i<NNewMon;i++){
		BasicObjectChars* BOC=AllUnitsChars[i];
		NewMonster* NM=NewMon[i];
		NM->MyIndex=i;
        NM->InitNM(BOC->UnitMD.pchar());
		NM->MyIndex=i;
		BOC->ConvertToNewMonster(NM);
	}
}
void AddAllCharsEditor(){
	//AddStdEditor("UnitsParams",&AllUnitsChars,"",RCE_DEFAULT,rce_CharsCallback);
}

//////////////////////////////////////////////////////////////////////////
///////////////////          NATIONS PARSING      ////////////////////////
//////////////////////////////////////////////////////////////////////////

ClonesArray<NationParams> AllNations;

extern char** NatNames;
extern char** NatScripts;
extern int NNations;
extern char** NatsIDS;

void ConvertToAllNations(){
	AllNations.Clear();
	for(int i=0;i<NNations;i++){
        NationParams* np=new NationParams;
		np->NationName=NatNames[i];
		np->NationalSuffix=NatsIDS[i];
		AllNations.Add(np);
	}
}
void ConvertFromAllNations(){

}
//////////////////////////////////////////////////////////////////////////
bool RemoveXmlDefaults(xmlQuote& XML,xmlQuote& Default){
	int n=XML.GetNSubQuotes();
	int nd=Default.GetNSubQuotes();
	byte* delit=new byte[n];
	memset(delit,0,n);
	char* SameName=NULL;
	for(int i=0,int j=0;i<n && j<nd;i++,j++){
		xmlQuote* x=XML.GetSubQuote(i);
		xmlQuote* xd=Default.GetSubQuote(j);
		bool del=false;
		char* name=(char*)x->GetQuoteName();
		if(name){
			if(!SameName)SameName=name;
			else if(strcmp(SameName,name))SameName="";
		}
		if(!strcmp(x->GetQuoteName(),xd->GetQuoteName())){
			if(x->GetNSubQuotes() && xd->GetNSubQuotes()){
				if(!RemoveXmlDefaults(*x,*xd))del=true;
			}else{
				if(x->GetNSubQuotes()==0 && xd->GetNSubQuotes()==0){
					if(x->Get_string() && xd->Get_string() && !strcmp(x->Get_string(),xd->Get_string()))del=true;
					else if(x->Get_string()==NULL && xd->Get_string()==NULL)del=true;
				}
			}
		}
		if(del){
			delit[i]=1;			
		}
	}
	bool delall=true;
	for(int i=0;i<n;i++)if(!delit[i]){
		delall=false;
		break;
	}
	if(n!=nd)delall=false;
	if(!(delall==false && SameName && SameName[0]!=0)){
		for(int i=0,int j=0;i<n;i++,j++)if(delit[j]){
			XML.DelSubQuote(i);
			i--;n--;
		}
	}
	delete[]delit;
	return XML.GetNSubQuotes()>0 && Default.GetNSubQuotes()>0;
}
void RestoreXmlFromShortForm(xmlQuote& Short,xmlQuote& Full){
    int n=Full.GetNSubQuotes();	
	char* SameName=NULL;
	for(int i=0;i<n;i++){
		xmlQuote* x=Full.GetSubQuote(i);
		char* name=(char*)x->GetQuoteName();
		if(name){
			if(!SameName)SameName=name;
			else if(strcmp(SameName,name))SameName="";
		}
	}
	if(SameName && SameName[0]){
		int ns=Short.GetNSubQuotes();	
		for(int i=0;i<ns && i<n;i++){
			xmlQuote* sub=Short.GetSubQuote(i);
			xmlQuote* subf=Full.GetSubQuote(i);
			RestoreXmlFromShortForm(*sub,*subf);			
		}
	}else{
		for(int i=0;i<n;i++){
			xmlQuote* sub=Full.GetSubQuote(i);
			const char* qname=sub->GetQuoteName();
			xmlQuote* X=Short.GetSubQuote((char*)qname);
			if(X){
				if(X->GetNSubQuotes()){
					RestoreXmlFromShortForm(*X,*sub);
				}
			}else{			
				xmlQuote* t=Full.RemoveSubQuote(i);
				i--;n--;
				Short.AddSubQuote(t);
			}
		}
	}
}
void SaveClassDifference(BaseClass* BC,BaseClass* Original,xmlQuote& xml){
	BC->Save(xml,BC,NULL);
	xmlQuote XML;
	Original->Save(XML,Original,NULL);	
	RemoveXmlDefaults(xml,XML);
}
void LoadClassDifference(BaseClass* BC,BaseClass* Original,xmlQuote& xml){
	
}
void CreateCopyOfUnitCharacter(OneObject* OB){
	if(!OB->newMons->CharacterWasModified){
		BasicObjectChars* BOC=CreateCharacterFromNewMonster(OB->newMons);
		NewMonster* NM=new NewMonster;
		NM->MyIndex=OB->newMons->MyIndex;
		NM->InitNM(OB->newMons->MD_File);
		NM->CharacterWasModified=true;
        NM->MyIndex=OB->newMons->MyIndex;
		BOC->ConvertToNewMonster(NM);
		//NM->MyIndex=OB->newMons->MyIndex;
		OB->newMons=NM;
		delete(BOC);
	}
}
bool SaveCharacterChangeToXml(OneObject* OB,xmlQuote& xml){
	if(OB->newMons->CharacterWasModified){
		NewMonster* NM0=NewMon[OB->newMons->MyIndex];
		//NewMonster* NM0=OB->Ref.General->newMons;
		BasicObjectChars* BOCF=CreateCharacterFromNewMonster(NM0);
        xmlQuote XF;
		BOCF->Save(XF,BOCF,NULL);
		BasicObjectChars* BOC=CreateCharacterFromNewMonster(OB->newMons);
		xml.SetQuoteName(BOC->GetClassName());
		BOC->Save(xml,BOC,NULL);
        RemoveXmlDefaults(xml,XF);
		delete(BOC);
		delete(BOCF);
		xml.WriteToFile("Temp\\temp_w.xml");
		return true;
	}
	return false;
}
bool SaveMod=false;
void SaveAllChangedCharactersToXml(xmlQuote& xml){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB && OB->newMons->CharacterWasModified){
            xmlQuote* XML=new_xmlQuote();
			SaveCharacterChangeToXml(OB,*XML);
			char cc[32];
			sprintf(cc,"%d %s",OB->UniqID,XML->GetQuoteName());
			XML->SetQuoteName(cc);
			if(XML->GetNSubQuotes())xml.AddSubQuote(XML);
			else delete_xmlQuote(XML);
		}
	}
	_chdir(IRM->GetHomeDirectory());
	char s[256];
	sprintf(s,"%d modified units saved.",xml.GetNSubQuotes());
	AssignHint1(s,256);
	xml.WriteToFile("Temp\\temp.xml");
	SaveMod=true;
}

void InitUnitAbilities(OneObject* OB);

void LoadCharacterFromXml(OneObject* OB,xmlQuote& xml){
	//xml.WriteToFile("temp_r.xml");

	int id=OB->newMons->MyIndex;
	if(OB->newMons->CharacterWasModified){
		delete(OB->newMons);
		delete(OB->MoreCharacter);
	}

	//dirty hack
	xmlQuote* x=NULL;
	do{
		x=xml.GetSubQuote("Lockpoints");
		if(x)x->SetQuoteName("");
	}while(x);
	do{
		x=xml.GetSubQuote("LockpointsDuringBuildStages");
		if(x)x->SetQuoteName("");
	}while(x);
	do{
		x=xml.GetSubQuote("CheckPoints");
		if(x)x->SetQuoteName("");
	}while(x);
	do{
		x=xml.GetSubQuote("Animations");
		if(x)x->SetQuoteName("");
	}while(x);
	do{		
		x=xml.GetSubQuote("ActiveBones");
		if(x)x->SetQuoteName("");
	}while(x);
	do{		
		x=xml.GetSubQuote("EmotIcons");
		if(x)x->SetQuoteName("");
	}while(x);
	//////////////////////////////////////////////////////////////////////////	
	NewMonster* NM=OB->newMons=new NewMonster;
	NM->CharacterWasModified=true;
	NM->MyIndex=id;
	assert(id<NNewMon);
    const char* clname=xml.GetQuoteName();
	char* nn=strstr(clname," ");
	if(nn)clname=nn+1;
	OneClassStorage* OCS=CGARB.GetClass(clname);
	if(OCS && OCS->OneMemb){
		NewMonster* NM0=NewMon[OB->newMons->MyIndex];
		//NewMonster* NM0=OB->Ref.General->newMons;
		BasicObjectChars* BOCF=CreateCharacterFromNewMonster(NM0);
		xmlQuote XF;
		BOCF->Save(XF,BOCF,NULL);
		RestoreXmlFromShortForm(xml,XF);
		BasicObjectChars* BOC=(BasicObjectChars*)OCS->OneMemb->new_element();
		ErrorPager ep(0);
		BOC->Load(xml,BOC,&ep,NULL);
		//hack
		//if(BOC->Hitpoints<OB->MaxLife)BOC->Hitpoints=OB->MaxLife;
		//
		NM->InitNM(BOC->UnitMD.pchar());
		BOC->ConvertToNewMonster(NM);
		NM->CharacterWasModified=true;
		NM->MyIndex=id;		
		ModifyOneObject(OB);
		InitUnitAbilities(OB);
		NM->Animations=NM0->Animations;
		delete(BOC);
		delete(BOCF);
		xml.ClearAll();
	}
}
void LoadAllCharactersFromXml(xmlQuote& xml){
	int n=xml.GetNSubQuotes();
	for(int i=0;i<n;i++){
		xmlQuote* XML=xml.GetSubQuote(i);
		DWORD UID=atoi(XML->GetQuoteName());
		if(XML->GetNSubQuotes()){
			for(int j=0;j<MAXOBJECT;j++){
				OneObject* OB=Group[j];
				if(OB && OB->UniqID==UID){
					int LoIdx=-1;
					int AnIdx=-1;
					if(OB->LoLayer)LoIdx=OB->LoLayer->Code;
					if(OB->NewAnm)AnIdx=OB->NewAnm->Code;
					LoadCharacterFromXml(OB,*XML);
					if(LoIdx!=-1)OB->LoLayer=OB->newMons->GetAnimation(LoIdx);
					if(AnIdx!=-1)OB->NewAnm=OB->newMons->GetAnimation(AnIdx);

				}
			}
		}
	}
}
class _ClassPtr_BasicObjectChars:public ClassPtr<BasicObjectChars>{
	bool ForceSimplification(){
		return true;	
	}
};
class UnitInfoContainer:public BaseClass{
public:
	_ClassPtr_BasicObjectChars UnitParams;
	SAVE(UnitInfoContainer);
		REG_AUTO(UnitParams);
	ENDSAVE;
	bool ForceSimplification(){
		return true;
	}
};
OneObject* CurUnit;
UnitInfoContainer UContainer;
//Options=1-init, 2-process, 3-ok pressed, 4-cancel pressed 
bool ProcessCurUnitEditor(ClassEditor* CE,BaseClass* BC,int Options){
	
	static HeroVariableStorage HVS;

	if(Options==1){//init
		// Call function from BE FLAG CORRECTOR
		g_BE_USAGE_FLAG = BE_IF_USE;
		///////////////////////////////////////
		CurUnit=NULL;
		if(ImNSL[MyNation]>0){
			int id=ImSelm[MyNation][0];
			if(id!=0xFFFF){
                CurUnit=Group[id];
				BasicObjectChars* BOC=CreateCharacterFromNewMonster(CurUnit->newMons);
				UContainer.UnitParams.Set(BOC);

				HeroVariableStorage* CurHero=DetectHero(CurUnit);
				if(CurHero){
					CurHero->Copy(&HVS,false);
				}

				return false;
			}
		}
		return true;
	}
	if(Options==2){//process
        return CurUnit==NULL;
	}
	if(Options==3&&CurUnit){//ok
		// Call function from BE FLAG CORRECTOR
		g_BE_USAGE_FLAG = BE_IF_CALL;
		///////////////////////////////////////
		BasicObjectChars* BOC=UContainer.UnitParams.Get();
		if(BOC){
			itr_GetSelected.Create(MyNation);
			while(OneObject* OB=itr_GetSelected.Next()){
				if(OB->NIndex==CurUnit->NIndex){
					NewMonster* NM=new NewMonster;
					NM->InitNM(BOC->UnitMD.pchar());
					NM->CharacterWasModified=true;
					NM->MyIndex=CurUnit->newMons->MyIndex;
					BOC->ConvertToNewMonster(NM);
					if(OB->newMons->CharacterWasModified){
						free(OB->newMons);
					}
					OB->newMons=NM;
					ModifyOneObject(OB);
					InitUnitAbilities(OB);
				}
			}
		}
		if(CurUnit){
			HeroVariableStorage* CurHero=DetectHero(CurUnit);
			if(CurHero){
				HVS.Copy(CurHero,false);
			}
		}
	}
	if(Options==4){//cancel
		// Call function from BE FLAG CORRECTOR
		g_BE_USAGE_FLAG = BE_IF_CALL;
		///////////////////////////////////////
        UContainer.reset_class(&UContainer);
	}
	return true;
}	
void AddCurrentUnitEditor(){
	InitClasses();
	AddStdEditor("Selected unit character editor",&UContainer,NULL,RCE_DEFAULT,&ProcessCurUnitEditor,NULL,'U');
}
class ModifiedUnitsSaver:public GameExtension{
public:
	virtual bool OnMapSaving(xmlQuote& xml){
		SaveAllChangedCharactersToXml(xml);
		return true;
	}
	virtual bool OnMapLoading(xmlQuote& xml){
		LoadAllCharactersFromXml(xml);
		return true;
	}
};
void RegModifiedUnitsSaver(){
	InstallExtension(new ModifiedUnitsSaver,"saver for changed units");
}
void CreateAdvCharacter(AdvCharacter* AC,NewMonster* NM);
void ModifyOneObject(OneObject* OB){
    AdvCharacter* AC=new AdvCharacter;
	CreateAdvCharacter(AC,OB->newMons);
	AC->Allocated     = true;
	OB->MoreCharacter = AC;

	if(OB->MaxLife==0)OB->MaxLife=1;
	OB->Life=int(AC->Life)*OB->Life/OB->MaxLife;
	OB->MaxLife=AC->Life;
	
	if(OB->MaxMana){
		OB->Mana=int(AC->MaxMana)*OB->Mana/OB->MaxMana;
	}else{
		OB->Mana=AC->MaxMana;
	}
	OB->MaxMana=AC->MaxMana;
	
	// propagation - инициализация OneObject из NewMonster
	OB->NewAnm=OB->newMons->GetAnimation(anm_Stand);
	OB->SetZeroFrame();
	OB->ObjectScale=OB->newMons->StartScale;
	OB->GroupSpeed=OB->newMons->MotionDist;
	OB->NotSelectable=OB->newMons->NotSelectable;
}

void MarkObjectLikeModified(OneObject* OB){
	if(!OB->newMons->CharacterWasModified){
		NewMonster* NM=new NewMonster;			
		NM->InitNM(OB->newMons->MD_File);
		NM->CharacterWasModified=true;	
		NM->MyIndex=OB->newMons->MyIndex;
		BasicObjectChars* BOC=CreateCharacterFromNewMonster(OB->newMons);
		BOC->ConvertToNewMonster(NM);
		if(OB->newMons->CharacterWasModified){
			free(OB->newMons);
		}
		OB->newMons=NM;
		ModifyOneObject(OB);
		InitUnitAbilities(OB);
		if(OB->ActiveAbility){
			OB->ActiveAbility->SetBornNIndex();
		}
	}
}
void MarkObjectLikeModified(OneObject* OB, bool InitAbility){
	if(!OB->newMons->CharacterWasModified){
		NewMonster* NM=new NewMonster;			
		NM->InitNM(OB->newMons->MD_File);
		NM->CharacterWasModified=true;	
		NM->MyIndex=OB->newMons->MyIndex;
		BasicObjectChars* BOC=CreateCharacterFromNewMonster(OB->newMons);
		BOC->ConvertToNewMonster(NM);
		if(OB->newMons->CharacterWasModified){
			free(OB->newMons);
		}
		OB->newMons=NM;
		ModifyOneObject(OB);
		if(InitAbility){
			InitUnitAbilities(OB);
			if(OB->ActiveAbility){
				OB->ActiveAbility->SetBornNIndex();
			}
		}
	}
}
void DisableCapturability(OneObject* OB){
	MarkObjectLikeModified(OB);
	OB->newMons->Capture=false;
}
void EnableCapturability(OneObject* OB){
	MarkObjectLikeModified(OB);
	OB->newMons->Capture=true;
}
//----modified unit interface----//
void CreateObjectXmlImage(OneObject* OB,xmlQuote* xml){
	xmlQuote* x=new_xmlQuote();
	xml->SetQuoteName(OB->Ref.General->MonsterID);
	SaveCharacterChangeToXml(OB,*x);
	xml->AddSubQuote(x);    
}
OneObject* CreateObjectFromXmlImage(byte NI,int x,int y,bool Anyway,xmlQuote* xml){
    const char* id=xml->GetQuoteName();
	if(id && xml->GetNSubQuotes()==1){
		for(int i=0;i<NATIONS->NMon;i++){
			if(!strcmp(id,NATIONS->Mon[i]->MonsterID)){
				int uid=NATIONS[NI].CreateNewMonsterAt(x<<4,y<<4,i,Anyway);
				if(uid!=-1){
					OneObject* OB=Group[uid];
                    LoadCharacterFromXml(OB,*xml->GetSubQuote(0));
                    return OB;
				}
				return NULL;
			}
		}
	}
	return NULL;
}
#define copyarr(Type,Arr,N)\
	if(NM->Arr){\
		NM2->Arr=znew(Type,NM->N);\
		memcpy(NM2->Arr,NM->Arr,NM->N*sizeof(Type));\
	}
#define copystr(str)\
	if(NM->str){\
		int L=strlen(NM->str);\
		NM2->str=znew(char,L+1);\
		memcpy(NM2->str,NM->str,L+1);\
	}
#define copydyn(dyn){\
    memset(&NM2->dyn,0,sizeof NM2->dyn);\
	int N=NM->dyn.GetAmount();\
	for(int i=0;i<N;i++){\
		NM2->dyn.Add(NM->dyn[i]);\
	}}
#define copyca(Arr){\
	memset(((BYTE*)&NM2->Arr)+4,0,sizeof(NM->Arr)-4);\
	int N=NM->Arr.GetAmount();\
	NM->Arr.Copy(&NM2->Arr);}
#define restore_vf(Type,field){\
	static Type Var;\
	memcpy(&NM->field,&Var,4);\
	memcpy(&NM2->field,&Var,4);\
}
#define restore_NM(Type){static Type Var;memcpy(NM2,&Var,4);}

void ModifyNM_Fast(OneObject* OB){	
	NewMonster* NM=OB->newMons;
	if(!NM->CharacterWasModified){
		NewMonster* NM2=new NewMonster;
		memcpy(NM2,NM,sizeof NewMonster);
		NM2->CharacterWasModified=true;
		copyarr(char,LockX,NLockPt);
		copyarr(char,LockY,NLockPt);
		for(int i=0;i<MaxAStages;i++){
			copyarr(char,SLockX[i],NSLockPt[i]);
			copyarr(char,SLockY[i],NSLockPt[i]);
		}
		restore_vf(ExtendedIcon,ExIcon);
		restore_vf(ClassArray<OneExIcon>,ExIcon.Icons);
		restore_vf(ClonesArray<OneEmotion>,EmotIcons);
		restore_vf(ClonesArray<OneBoneInfo>,ActiveBones);
		restore_vf(CommonParameters,NewParams);
		restore_vf(ClassArray<ComplexFireSource>,MultiWp);
		restore_NM(NewMonster);

		copyarr(char,BLockX,NBLockPt);
		copyarr(char,BLockY,NBLockPt);
		copyarr(char,CheckX,NCheckPt);
		copyarr(char,CheckY,NCheckPt);
		copyarr(char,CheckX,NCheckPt);
		copyarr(char,CheckY,NCheckPt);
		copystr(Message);
		copystr(LongMessage);
		copystr(PieceName);
		copydyn(BuildPtX);
		copydyn(BuildPtY);
		copydyn(ConcPtX);
		copydyn(ConcPtY);
		copydyn(ConcPtZ);
		copydyn(PosPtX);
		copydyn(PosPtY);
		copydyn(PosPtZ);
		copydyn(CraftPtX);
		copydyn(CraftPtY);
		copydyn(CraftPtZ);
		copydyn(DamPtX);
		copydyn(DamPtY);
		copydyn(DamPtZ);
		copydyn(BornPtX);
		copydyn(BornPtY);
		copydyn(BornPtZ);
		copyarr(short,ShotPtX,NShotPt);
		copyarr(short,ShotPtY,NShotPt);
		copyarr(short,ShotPtYG,NShotPt);
		copyarr(short,ShotPtZ,NShotPt);
		copyarr(short,ShotDelay,NShotPt);
		copyarr(byte,ShotAttType,NShotPt);
		copyarr(byte,ShotDir,NShotPt);
		copyarr(byte,ShotDiff,NShotPt);
		copyarr(word,ShotMinR,NShotPt);
		copyarr(word,ShotMaxR,NShotPt);
		copystr(Page1Hint);
		copystr(Page2Hint);
		copystr(Page3Hint);
		copyarr(short,Bars3D,NBars);
		copystr(Name);
		copyca(MultiWp);
		copyca(ExIcon.Icons);
		NM2->WSECT=NULL;
		copyarr(short,WavePoints,NWaves);
		NM2->FLAGS=NULL;
		copystr(MD_File);
		//if(NM->Ability){
		//	NM2->Ability=new MonsterAbility;
		//	NM->Ability->Copy(NM2->Ability);
		//}
		copyca(EmotIcons);
		copyca(ActiveBones);
		NM->NewParams.Copy(&NM->NewParams,false);
		AdvCharacter* AC=new AdvCharacter;
		OB->newMons=NM2;
		CreateAdvCharacter(AC,OB->newMons);
		AC->Allocated=true;
		OB->MoreCharacter=AC;		
		restore_vf(ClassArray<OneExIcon>,ExIcon.Icons);
        NM2->Ability=NULL;
	}
}
#define savediff(Type,val)\
	if(NM->val!=NM0->val){\
		FLAGS|=1<<p;\
		BS->Write##Type(NM->val);\
	}\
	p++;
void SaveModifieadChar(OneObject* OB,BinStream* BS){
	if(OB->newMons->CharacterWasModified){
		BS->WriteBYTE(1);
		NewMonster* NM=OB->newMons;
		NewMonster* NM0=OB->Ref.General->newMons;
		DWORD FLAGS=0;
		int wpos=BS->GetWritePos();
		BS->WriteDWORD(FLAGS);
		int p=0;		

		savediff(WORD,Life);
		savediff(WORD,MaxMana);
		savediff(BYTE,VisionType);
		savediff(WORD,AttackSpeed);
		savediff(WORD,Speed);
        savediff(WORD,StartScale);
		savediff(DWORD,TFactor);
		savediff(WORD,MaxDamage[0]);
		savediff(WORD,AttackRadius2[0]);
		savediff(WORD,MaxDamage[1]);
		savediff(WORD,AttackRadius2[1]);
		savediff(WORD,Protection[0]);
		savediff(WORD,Protection[1]);
		savediff(WORD,Protection[2]);
		savediff(WORD,Protection[3]);
		savediff(WORD,Protection[4]);
		savediff(WORD,Protection[5]);

		*((DWORD*)(BS->GetData()+wpos))=FLAGS;
	}else BS->WriteBYTE(0);
}
#define readdif(Type,val)\
	if(FLAG&(1<<p)){\
		NM->val=BS->Read##Type();\
	}\
	p++;

void LoadModifiedChar(OneObject* OB,BinStream* BS){
	if(BS->ReadBYTE()){
		ModifyNM_Fast(OB);
        DWORD FLAG=BS->ReadDWORD();
        int p=0;
        NewMonster* NM=OB->newMons;

		readdif(WORD,Life);
		readdif(WORD,MaxMana);
		readdif(BYTE,VisionType);
		readdif(WORD,AttackSpeed);
		readdif(WORD,Speed);
		readdif(WORD,StartScale);
		readdif(DWORD,TFactor);
		readdif(WORD,MaxDamage[0]);
		readdif(WORD,AttackRadius2[0]);
		readdif(WORD,MaxDamage[1]);
		readdif(WORD,AttackRadius2[1]);
		readdif(WORD,Protection[0]);
		readdif(WORD,Protection[1]);
		readdif(WORD,Protection[2]);
		readdif(WORD,Protection[3]);
		readdif(WORD,Protection[4]);
		readdif(WORD,Protection[5]);

        AdvCharacter* AC=OB->MoreCharacter;
		CreateAdvCharacter(AC,NM);

		if(OB->MaxLife==0)OB->MaxLife=1;
		OB->Life=int(AC->Life)*OB->Life/OB->MaxLife;
		OB->MaxLife=AC->Life;		
		
		OB->NotSelectable=OB->newMons->NotSelectable;
	}
}