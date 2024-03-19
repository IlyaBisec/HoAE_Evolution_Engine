#pragma once

#define anm_MotionL			1
#define anm_MotionR			2
#define anm_MotionLB		3
#define anm_MotionRB		4
#define anm_MiniStepL		5
#define anm_MiniStepR		6
#define anm_MiniStepLB		7
#define anm_MiniStepRB		8
#define anm_Fist			9
#define anm_Death			10
#define anm_DeathLie1		11
#define anm_DeathLie2		12
#define anm_DeathLie3		13
#define anm_Stand			14
#define anm_Work			15
#define anm_Trans01			16
#define anm_Trans10			17
#define anm_FallDown        18
#define anm_StandUp         19
#define anm_StandHi			18
#define anm_StandLo			19
#define anm_Build			20
#define anm_BuildHi			21
#define anm_Damage			22

#define anm_Rest			23
#define anm_Rest1			24
#define anm_Rest2			25
#define anm_Rest3			26

#define anm_RotateR			27
#define anm_RotateL			28

#define anm_WorkTree		29
#define anm_WorkStone		30
#define anm_WorkField		31

#define anm_TransX3			32
#define anm_Trans3X			33

#define anm_RotateAtPlace   34
#define anm_Greeting        35
#define anm_Scare			36

#define anm_RestA1          37

#define anm_CastPrepare     38
#define anm_CastDirect      39

#define anm_Summon			40
#define anm_Skeleton		41

#define anm_PickUp			42

#define anm_Attack			100
#define anm_PAttack			200
#define anm_UAttack			300
#define anm_PStand			400
#define anm_PMotionL		500
#define anm_PMotionR		600
#define anm_PMotionLB		700
#define anm_PMotionRB		800
#define anm_PMiniStepL		900
#define anm_PMiniStepR		1000
#define anm_PMiniStepLB		1100
#define anm_PMiniStepRB		1200
#define anm_BuildLo         1300
#define anm_Trans           1400
//#TRANSXY   x=0..9 y=0..9
#define anm_Temp			1500

class DIALOGS_API NewFrame : public BaseClass {
public:
	short FileID;
	int SpriteID;
	int dx;
	int dy;
	SAVE(NewFrame);
	REG_MEMBER(_gpfile,FileID);
	REG_MEMBER(_int,SpriteID);
	REG_MEMBER(_int,dx);
	REG_MEMBER(_int,dy);
	ENDSAVE;
};
class DIALOGS_API ComplexFireSource:public BaseClass{
public:
	int  x,y,z;
	byte Dir;
	byte DirDiff;
	word AttTypeMask;
	word CoolDown;
};
class NewAnimation;
class DIALOGS_API AnimationExtension:public BaseClass{
public:
	NewAnimation* NA;
	float dx,dy,dz;
	float Scale,dDir,dFi;
	int   Period;
	SAVE(AnimationExtension);
	//AnimationExtension
	ENDSAVE;
};

class DIALOGS_API AnimFrame3D:public BaseClass{
public:
	int Model;
	int Animation;
	int NFrames;
	float StartAnmTime;
	float FinalAnmTime;
	float Scale;
	float AddDir;
	SAVE(AnimFrame3D);
	REG_MEMBER(_ModelID,Model);
	REG_MEMBER(_ModelID,Animation);
	REG_MEMBER(_int,NFrames);
	REG_MEMBER(_float,StartAnmTime);
	REG_MEMBER(_float,FinalAnmTime);
	REG_MEMBER(_float,Scale);
	REG_MEMBER(_float,AddDir);
	ENDSAVE;
};
class OneObject;
class DIALOGS_API OneBoundEffect:public BaseClass{
public:
	_str EffectFile;
	_str EffectName;
	int EffectFileID;
	int EffectNameID;
	int SoundID;
	SAVE(OneBoundEffect);
	REG_FILEPATH(EffectFile,"*.eff");
	REG_AUTO(EffectName);
	ENDSAVE;
	virtual DWORD GetAlpha(OneObject* OBJ){return 0;};
	virtual float GetIntensity(OneObject* OBJ){return 1.0f;};
	virtual bool Parse(char* s,_str& ErrLog);
};
class DIALOGS_API LifeDependentEffect:public OneBoundEffect{
public:
	int LifePercent;
	SAVE(LifeDependentEffect);
	REG_PARENT(OneBoundEffect);
	REG_MEMBER(_int,LifePercent);
	ENDSAVE;
	virtual DWORD GetAlpha(OneObject* OBJ);	
	virtual bool Parse(char* s,_str& ErrLog);
};
class DIALOGS_API MoveDependentEffect:public OneBoundEffect{
public:	
	SAVE(MoveDependentEffect);
	REG_PARENT(OneBoundEffect);
	ENDSAVE;
	virtual DWORD GetAlpha(OneObject* OBJ);	
	virtual float GetIntensity(OneObject* OBJ);
	virtual bool Parse(char* s,_str& ErrLog);
};
class DIALOGS_API OneActivePt:public BaseClass{
public:
	int x;
	int y;
	SAVE(OneActivePt);
	REG_MEMBER(_int,x);
	REG_MEMBER(_int,y);
	ENDSAVE;
	const char* GetThisElementView(const char*){
		char* s=GetGlobalBuffer();
		sprintf(s,"(%d,%d)",x,y);
		return s;
	}
};
class DIALOGS_API APtList:public LinearClonesArray<OneActivePt>{
	void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL){
		APtList* List=(APtList*)ClassPtr;
		_str s;
		s.print("%d ",List->GetAmount());
		for(int i=0;i<List->GetAmount();i++){
			s.print("%d %d ",(*List)[i]->x,(*List)[i]->y);
		}
		xml.Assign_string(s);
	}
	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		int nextint(char** s);
		char* s=(char*)xml.Get_string();
		if(s){
			int n=nextint(&s);
			APtList* List=(APtList*)ClassPtr;
			OneActivePt* p=new OneActivePt;
			List->Add(p,n);
			for(int i=0;i<n;i++){
				(*List)[i]->x=nextint(&s);
				(*List)[i]->y=nextint(&s);
			}
		}
		return true;
	}
};
class DIALOGS_API NewAnimation:public BaseClass{
public:
	union{
		struct{
			word Code1;//used for indexsation
			word Code2;
		};
		DWORD Code;
	};
	DWORD SrcCode;
	NewAnimation* Next;//for hash
	word NSuchAnimations;

	bool Enabled;
	byte AnimationType;//0-2d, 1-3d 2-patch
	bool Reflected;
	int  CanBeBrokenAfterFrame;
	bool MoveBreak;
	bool Inverse;
	byte DoubleShot;
	bool DoubleAnm;
	int  NFrames;
	byte Parts;
	byte PartSize;
	int ActiveFrame;
	float Hardness;

	int SoundID;
	int SoundProbability;
	int  HotFrame;
	int  HotFrame2;

	short StartDx;
	short StartDy;
	short* ActivePtX;
	short* ActivePtY;
	APtList ActivePt;
	short* LineInfo;
	char* Name;
	int Rotations;
	ClonesArray<NewFrame> Frames;
	//for 3D models!
	int ModelID;
	int ModelID_iso;
	int AnimationID;
	SubSection ExtendedParams;
	int ReflectionID;
	int TimeAnimationID;
	int TimeAnimationFrames;
	int TimeAnimationVariation;
	ClonesArray<AnimFrame3D> AnimSet3D;

	int SecondAnimationID;
	float DirFactor;
	float Scale;
	int AddDirection;
	int  AddHeight;

	int HotRadius;
	int HotHeight;
	int HotShift;

	NewAnimation();
	~NewAnimation();
	int firstFrame;
	int lastFrame;
	int TiringChange;
	int SlowFrameStart;
	int SlowFrameEnd;
	int SlowFrameSpeed;
	//for patch
	int SelectionTypeName;
	ClonesArray<AnimationExtension> AnmExt;
	ClassArray<OneBoundEffect> Effects;
	//ClassArray<ComplexFireSource> ComplexFire;
	void AllocFrames(int n){
		NewFrame* F=new NewFrame[n];
		for(int i=0;i<n;i++)Frames.Add(F+i);
	}
	
	void DrawSpriteUnit( OneObject* OB, const Vector3D& pos, int frame, float Dir, byte NI , float Scale);
	void DrawShadowedSpriteUnit( OneObject* OB, const Vector3D& pos, int frame, float Dir, byte NI , float Scale);

	void DrawSpriteBuilding( OneObject* OB, const Vector3D& pos, int frame, byte NI );
	void DrawAt(int frame,byte NI,
		float x,float y,float z,
		float Dir,float Scale,DWORD Color,
		float fiDir,float fiOrt,
		OneObject* OB);//frame - multiplied on 256


	//bool GetHotSpot(int& x,int& y,int& z);
	SAVE(NewAnimation){
		//REG_CLASS(AnimationExtension);
		REG_CLASS(OneActivePt);
		REG_ENUM_EX(_index,Code,MON_ANIMATIONS,AnimationCode);
		SAVE_SECTION(1);//common section
		REG_MEMBER(_bool,Enabled);	
		REG_MEMBER(_int,CanBeBrokenAfterFrame);
		REG_MEMBER(_bool,MoveBreak);
		REG_MEMBER(_int,TiringChange);
		REG_ENUM(_byte_index,AnimationType,AnimationType);

		REG_ENUM(_index,SoundID,ALL_SOUNDS);
		REG_MEMBER(_int,SoundProbability);
		REG_MEMBER_EX(_int,HotFrame,HotFrameForSound);	
		REG_MEMBER_EX(_int,HotFrame2,HotFrame2ForSound);	

		REG_MEMBER_EX(_int,ActiveFrame,ActiveFrameForAttack);
		SAVE_SECTION(2);//for 2d animation
		REG_MEMBER(_bool,Inverse);
		REG_MEMBER(_bool,DoubleShot);
		REG_MEMBER(_bool,DoubleAnm);	
		REG_AUTO(ActivePt);
		REG_MEMBER(_int,NFrames);
		REG_AUTO(Frames);
		REG_MEMBER(_int,Rotations);

		SAVE_SECTION(4);//for 3d animation
		REG_AUTO(AnimSet3D);
		REG_MEMBER(_float,Scale);
		REG_MEMBER(_int,NFrames);
		REG_MEMBER(_ModelID,ModelID);
		REG_MEMBER(_ModelID,AnimationID);
		REG_AUTO(ExtendedParams);
		REG_MEMBER(_ModelID,SecondAnimationID);
		REG_MEMBER(_ModelID,ReflectionID);
		REG_MEMBER(_ModelID,TimeAnimationID);
		REG_MEMBER(_ModelID,ModelID_iso);
		REG_MEMBER(_int,TimeAnimationFrames);
		REG_MEMBER(_int,TimeAnimationVariation)
			REG_MEMBER(_float,DirFactor);
		REG_MEMBER(_int,AddDirection);
		REG_MEMBER(_int,AddHeight);	
		REG_MEMBER(_int,SlowFrameStart);
		REG_MEMBER(_int,SlowFrameEnd);
		REG_MEMBER(_int,SlowFrameSpeed);

		SAVE_SECTION(8);
		REG_ENUM(_index,SelectionTypeName,TEXTURE_CURSOR_TYPES);
		REG_MEMBER(_float,Scale);
		SAVE_SECTION(1);
		REG_AUTO(AnmExt);	
		REG_MEMBER(_int,HotRadius);
		REG_MEMBER(_int,HotHeight);
		REG_MEMBER(_int,HotShift);

		REG_MEMBER(_short,StartDx);
		REG_MEMBER(_short,StartDy);

	}ENDSAVE;

	DWORD GetClassMask(){
		if(AnimationType==1)return 1+4;
		else if(AnimationType==2)return 1+8;
		else return 1+2;
	}

	const char* GetThisElementView(const char*){
		//Enumerator* E=ENUM.Get("MON_ANIMATIONS");
		//if(E)return E->Get(Code);
		//else return NULL;
		return NULL;
	}

	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);

};
#define AnmHashSize 67
class DIALOGS_API AnimationsScope:public BaseClass{
public:
	ClassArray<NewAnimation> ANM;
	NewAnimation* AHASH[AnmHashSize];
	void Add(NewAnimation* NA,DWORD Code);
	NewAnimation* Get(DWORD Code);
	NewAnimation* Get(DWORD Code,int Index);
	void CreateHashTable();
	SAVE(AnimationsScope);
	REG_AUTO(ANM);
	ENDSAVE;
};