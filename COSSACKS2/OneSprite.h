#pragma once
class ObjCharacter;
struct BornRef;
class DIALOGS_API ObjCharRef:public ClassRef<ObjCharacter>{
public:
	SAVE(ObjCharRef);
	REG_PARENT(ClassRef<ObjCharacter>);
	ENDSAVE;
	const char* GetThisElementView(const char*);
};
class DIALOGS_API BornRef:public BaseClass{
public:
	ObjCharRef ObjectToBorn;
	//int BornID;
	int BDx;
	int BDy;
	SAVE(BornRef);
	REG_AUTO(ObjectToBorn);
	REG_MEMBER(_int,BDx);
	REG_MEMBER(_int,BDy);
	ENDSAVE;
};
class DIALOGS_API OneConnector:public BaseClass{
public:
	int x,y;
	int ConnType;
	SAVE(OneConnector);
	REG_MEMBER(_int,x);
	REG_MEMBER(_int,y);
	REG_MEMBER(_int,ConnType);
	ENDSAVE;
};
struct DIALOGS_API AlignInfo{
	float xp,yp;//coordinates on picture
	float wx,wy,wz;//coordinates in world space relative to center of the sprite
};
class DIALOGS_API LockInfoPoint:public BaseClass{
public:
	int x,y;
	SAVE(LockInfoPoint);
	REG_MEMBER(_int,x);
	REG_MEMBER(_int,y);
	ENDSAVE;
	const char* GetThisElementView(const char*){
		static char cc[32];
		sprintf(cc,"(%d,%d)",x,y);
		return cc;
	}
};
class DIALOGS_API TreeModificationParam:public BaseClass{
public:
	int		LandType;
	float	Scale;
	float	Angle;
	DWORD	Color;
	float	RotAngle;
	int		ModelID;	
	int		LeafsShaderID;
	int		LeafsRiseVariable;
	int		LeafsDeathVariable;
	int		RiseTime;
	int		DeathTime;
	int     SpriteForShadowID;
	int		SpriteForIsomerticMode;
	float   ShadowScale;
	int		SoundID;
	int		SoundProbability;
	SAVE(TreeModificationParam);
	ENDSAVE;
};
class DIALOGS_API ObjCharacter:public ReferableBaseClass{
public:
	byte ViewType;
	byte RenderType;
	short FileID;
	int SpriteID;
	short FileID_forBackground;
	int SpriteID_forBackground;
	short FileID_forPreview;
	int SpriteID_forPreview;
	int Radius;
	int CenterX;
	int CenterY;

	int ParentIndex;

	ClassArray<TreeModificationParam> TreeMods;

	byte ResType;//0-wood,1-gold,2-stone,3-food,0xFE-no resource,0xFF-removable
	int WorkRadius;
	int ResPerWork;
	int Amplitude;
	int WorkAmount;
	ClassRef<ObjCharacter> NextWorkObj;
	word WNextObj;
	word DamageAmount;
	word DNextObj;
	int TimeAmount;
	ClassRef<ObjCharacter> NextTimeObj;
	word TNextObj;
	byte IntResType;
	int IntResPerWork;
	int IntWorkRadius;
	NewAnimation* Stand;
	NewAnimation* Shadow;
	int SoundID;
	int SoundProb;
	short Z0;
	short DZ;
	byte Parts;
	byte Delay;
	byte Frames;
	byte Discret;
	byte LockRadius;
	word OnGround;
	//word NLockPt;
	word NRandom;
	word SpriteIndex;
	short GroupIndex;
	short IndexInGroup;
	//short* LockX;
	//short* LockY;
	ClonesArray<LockInfoPoint> LockInfo;
	ClonesArray<BornRef> BREF;
	//int NBorn;
	//char* Name;
	int ShieldRadius;
	int  ShieldProbability;
	short FixDx;
	short FixDy;
	byte FixDir;
	byte FixR;
	//connectors
	ClonesArray<OneConnector> CONN;
	int NOutConn;
	byte EquGroup;
	//aligning info
	byte Aligning;
	bool HaveAligning;
	//for v-aligning:
	int va_x1;
	int va_y1;
	int va_x2;
	int va_y2;
	AlignInfo P1;//3 points determine plane in space
	AlignInfo P2;
	AlignInfo P3;
	bool UseTexture;
	float uL,vL,uR,vR,uC,vC;
	//IMediaManager
	int ModelManagerID;
	float MShiftX;
	float MShiftY;
	bool EditableModel;

	DWORD TFactor;
	float Scale;
	float RotX;
	float RotY;
	float RotFI;
	float ModelDZ;
	void SetModelDefaults();

    int   LoQShadowID;
    float LoQShadowSX;
    float LoQShadowSY;

	void GetMatrix4D(Matrix4D& M4,int x,int y,int z) const;
	int FixHeight;
	bool RoadCenter;	
	//auto deep params
	int AutoDeep;
	int AutoDeepR;
	int AutoDeepR1;

	int 	GrassRadius;
	int 	GrassType;
	int 	GrassHeight;

	ObjCharacterExtension Ext;
	SAVE(ObjCharacter);
	SAVE_SECTION(0xFFFFFFFF);
	REG_AUTO(Name);
	REG_ENUM(_byte_index,ViewType,SpriteViewType);

	REG_MEMBER(_int,GrassRadius);
	REG_MEMBER(_int,GrassType);
	REG_MEMBER(_int,GrassHeight);

	SAVE_SECTION(1);

	REG_ENUM(_byte_index,RenderType,SprRenderType);
	REG_MEMBER(_gpfile,FileID);
	REG_MEMBER(_int,SpriteID);

	SAVE_SECTION(0xFFFFFFFF);

	REG_MEMBER(_gpfile,FileID_forBackground);
	REG_MEMBER(_int,SpriteID_forBackground);
	REG_MEMBER(_gpfile,FileID_forPreview);
	REG_MEMBER(_int,SpriteID_forPreview);

	SAVE_SECTION(1);

	REG_MEMBER(_int,CenterX);
	REG_MEMBER(_int,CenterY);

	SAVE_SECTION(2);

	REG_MEMBER(_float,uL);
	REG_MEMBER(_float,vL);
	REG_MEMBER(_float,uR);
	REG_MEMBER(_float,vR);
	REG_MEMBER(_float,uC);
	REG_MEMBER(_float,vC);

	SAVE_SECTION(16);

	REG_MEMBER(_ModelID,ModelManagerID);
	REG_MEMBER(_int,MShiftX);
	REG_MEMBER(_int,MShiftY);
	REG_MEMBER(_bool,EditableModel);

	SAVE_SECTION(0xFFFFFFFF);

	REG_MEMBER(_int,Radius);		
	REG_ENUM(_byte_index,ResType,ResType);
	REG_MEMBER(_int,WorkRadius);
	REG_MEMBER(_int,Amplitude);
	REG_MEMBER(_int,WorkAmount);
	REG_AUTO(NextWorkObj);
	REG_MEMBER(_int,TimeAmount);
	REG_AUTO(NextTimeObj);
	REG_ENUM(_byte_index,IntResType,ResType);
	REG_MEMBER(_int,IntResPerWork);
	REG_MEMBER(_int,IntWorkRadius);
	REG_AUTO(CONN);
	REG_AUTO(LockInfo);
	REG_AUTO(BREF);

	REG_CLASS(LockInfoPoint);
	REG_CLASS(BornRef);
	REG_CLASS(OneConnector);

	REG_MEMBER(_int,ShieldRadius);
	REG_MEMBER(_int,ShieldProbability);
	REG_MEMBER(_int,AutoDeep);
	REG_MEMBER(_int,AutoDeepR);
	REG_MEMBER(_int,AutoDeepR1);
	REG_MEMBER(_int,FixHeight);

    REG_MEMBER(_int,LoQShadowID);
    REG_MEMBER(_float,LoQShadowSX);
    REG_MEMBER(_float,LoQShadowSY);

	SAVE_SECTION(4);

	REG_ENUM(_byte_index,Aligning,SpriteAlignType);

	SAVE_SECTION(8);

	REG_MEMBER(_int,va_x1);
	REG_MEMBER(_int,va_y1);
	REG_MEMBER(_int,va_x2);
	REG_MEMBER(_int,va_y2);

	ENDSAVE;
	DWORD GetClassMask(){
		if(ViewType==0){//Sprite
			DWORD M=1+4;
			if(Aligning==2)M|=8;
			return M;
		}else
			if(ViewType==1){//3D-animation
				return 16;
			}else
				if(ViewType==2){//field path
					return 0x80000000;
				}else
					if(ViewType==3){//part of texture
						return 2;
					}else return 0xFFFFFFFF;
	}
	const char* GetThisElementView(const char* LocalName){
		static char cc[128];
		if(!Name.pchar())return LocalName;
		sprintf(cc,"%s : %s",LocalName,Name.pchar());
		return cc;
	}
};
class DIALOGS_API OneSprSection:public BaseClass{
public:
	_str SectionName;
	ClonesArray<ObjCharRef> ObjectsList;
	SAVE(OneSprSection);
	REG_CLASS(ObjCharRef);
	REG_AUTO(SectionName);
	REG_AUTO(ObjectsList);
	ENDSAVE;
};
class DIALOGS_API ObjList:public ClonesArray<ObjCharacter>{
public:
	void Arrange(){
		for(int i=0;i<GetAmount();i++)(*this)[i]->ParentIndex=i;
	}
	virtual int ExpandWith(const char* ElmName,void* base){
		int N=ClassArray<ObjCharacter>::ExpandWith(ElmName,base);
		Arrange();
		return N;
	}	
	virtual bool MoveElement(int index,char opType){
		bool r=ClassArray<ObjCharacter>::MoveElement(index,opType);
		if(opType)Arrange();
		return r;
	}
	virtual bool DelElement(int index){		
		bool r=ClassArray<ObjCharacter>::DelElement(index);
		Arrange();
		return r;
	}
};
class DIALOGS_API SprGroup:public BaseClass{
public:
	ObjList Objects;
	ClonesArray<OneSprSection> Sections;

	SAVE(SprGroup);
	REG_AUTO(Objects);
	REG_AUTO(Sections);
	ENDSAVE;
	int GetNSprites(){
		return Objects.GetAmount();
	}	
	SprGroup();
	~SprGroup();
	void LoadSprites(char* fname);
	int GetIndexByName(char* Name);
};
class OneSprite;
class DIALOGS_API ActionOverSprite:public BaseClass{
public:
	virtual bool Process(OneSprite* OS){return false;};//returns false if delete required
	virtual DWORD GetActionID(){return 0;};
	SAVE(ActionOverSprite);
	ENDSAVE;
};
//sometimes peasants push trees slightly
class DIALOGS_API PushSprite3D:public ActionOverSprite{
	Matrix4D StartMatrix;
	Matrix4D FinalMatrix;
	int TitalTime;
	int CurrentTime;
	float NOscl;
public:
	bool Create(OneSprite* OS,int PusherX,int PusherY,float PushAngle,int ReturnBackTime,float NOscillations);
	virtual DWORD GetActionID(){return 'PUSH';}
	virtual bool Process(OneSprite* OS);
};
//changes tree type according to land
class DIALOGS_API ChangeTreeLandType:public ActionOverSprite{
	int DestTreeLandType;
	int SrcLandType;
	int TimePassed;
public:
	void Create(int DestLand);
	virtual DWORD GetActionID(){return 'LAND';}
	virtual bool Process(OneSprite* OS);
};

class DIALOGS_API OneSprite{
public:
	bool Enabled:1;
	bool Surrounded:1;
	byte Locking;	
	int x;
	int y;
	short z;
	word Radius;
	SprGroup* SG;
	ObjCharacter* OC;
	int Index;
	word SGIndex;
	byte WorkOver;
	int TimePassed;
	byte Damage;
	Matrix4D* M4;

	int CurrentModIndex;
	DWORD Color;
	float RiseStage;
	float DeathStage;

#ifdef CONQUEST
	word NIndex;
	byte Direction;
	bool Reflected:1;
#endif
	int PerformWork(word);
	int PerformIntWork(int work);
	void PerformDamage(int Dam);
	void CreateMatrix();

	ClassArray<ActionOverSprite> SpriteActions;
	void AddSpriteAction(ActionOverSprite* Action);
	bool CheckActionPresence(DWORD Code);

	static DynArray<int> ActiveSprites;
	static void ClearActiveSprites();
	static void ProcessActiveSprites();
};